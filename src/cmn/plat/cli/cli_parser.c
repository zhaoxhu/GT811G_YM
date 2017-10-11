
#include <ctype.h>
#include <network.h>
#include "plat_common.h"
#include "cli_common.h"
#include "cyg/kernel/kapi.h"
#include "db_api.h"

#ifdef HAVE_TELNET_CLI
#define perror(s) cs_printf(s)
void telnet_diag_print_unregister(void);
extern cs_uint32 app_ip_changed;
externC void (*_putc)(char c, void **param);
static void (*_putcFunc)(char c, void **param) = NULL;

extern struct cli_def *telnet_cli;

extern void mon_read_char(char *c);
extern bool mon_read_char_with_timeout(char *c);
extern void mon_write_chars(char* s, int n);
extern int cmd_exe(int argc, char **argv);
extern int set_auth(int privilege,char *name, char *password,struct cli_def *cli);
int cur_chan = CHANNEL_SERIAL;
char g_parser_word[MAX_WORDS_NUM][MAX_WORDS_LEN];
struct cli_def g_cli_telnet;
char g_cli_telnet_buffer[MAX_PRINT_BUF_LEN];
char cli_cmd_reset[32];
int cli_cmd_state;
cs_uint8 cmd_reset_confirm = 0;
/*
 * To support none C/POSIX interface asprintf() on ecos
 */
int vasprintf(char **strp, const char *fmt, va_list args) {
    int size;

    size = vsnprintf(NULL,0,fmt,args);
    if ((*strp=malloc(size+1))==NULL) {
        return -1;
    }

    size = vsnprintf(*strp,size+1,fmt,args);
    return size;
}

int asprintf(char **strp, const char *fmt, ...) {
    va_list args;
    int size;

    va_start(args,fmt);
    size = vasprintf(strp,fmt,args);

    va_end(args);
    return size;
}

/*
 * Dummy definitions to allow compilation on ecos
 */
int regex_dummy() {return 0;};
#define regfree(...) regex_dummy()
#define regexec(...) regex_dummy()
#define regcomp(...) regex_dummy()
#define regex_t int
#define REG_NOSUB   0
#define REG_EXTENDED    0
#define REG_ICASE   0


struct unp {
    char *username;
    char *password;
    struct unp *next;
};

struct cli_filter_cmds
{
    char *cmd;
    char *help;
};


int cli_match_filter_init(struct cli_def *cli, int argc, char **argv, struct cli_filter *filt);
int cli_range_filter_init(struct cli_def *cli, int argc, char **argv, struct cli_filter *filt);
int cli_count_filter_init(struct cli_def *cli, int argc, char **argv, struct cli_filter *filt);
int cli_match_filter(struct cli_def *cli, char *string, void *data);
int cli_range_filter(struct cli_def *cli, char *string, void *data);
int cli_count_filter(struct cli_def *cli, char *string, void *data);

#ifdef HAVE_CLI_FILTER
static struct cli_filter_cmds filter_cmds[] =
{
    { "begin",   "Begin with lines that match" },
    { "between", "Between lines that match" },
    { "count",   "Count of lines"   },
    { "exclude", "Exclude lines that match" },
    { "include", "Include lines that match" },
    { "grep",    "Include lines that match regex,only support options: -i, -ex,not support regex" },
    { "egrep",   "Include lines that match extended regex,only support options: -i, -ex,not support regex" },
    { NULL, NULL}
};
#endif /* #ifdef HAVE_CLI_FILTER */

char *cli_command_name(struct cli_def *cli, struct cli_command *command)
{
    int l;
    char name[MAX_LINE_LENTH];
    memset(cli->commandname, 0,MAX_LINE_LENTH);

    while (command)
    {
        strcpy(name,cli->commandname);
        if(strlen(command->command) + strlen(name)+2 > MAX_LINE_LENTH) 
        {
            diag_printf("command line too long");
            return NULL;
        }
        sprintf(cli->commandname, "%s %s", command->command, name);
        command = command->parent;
    }

    l = strlen(cli->commandname);
    if (l) name[l - 1] = 0;
    return cli->commandname;

}

void cli_set_auth_callback(struct cli_def *cli, int (*auth_callback)(char *, char *, struct cli_def *))
{
    cli->auth_callback = auth_callback;
}

void cli_set_enable_callback(struct cli_def *cli, int (*enable_callback)(char *))
{
    cli->enable_callback = enable_callback;
}

void cli_allow_user(struct cli_def *cli, char *username, char *password)
{
    struct unp *u, *n;
    if (!(n = malloc(sizeof(struct unp))))
    {
        fprintf(stderr, "Couldn't allocate memory for user: %s", strerror(errno));
        return;
    }
    if (!(n->username = strdup(username)))
    {
        fprintf(stderr, "Couldn't allocate memory for username: %s", strerror(errno));
        free_z(n);
        return;
    }
    if (!(n->password = strdup(password)))
    {
        fprintf(stderr, "Couldn't allocate memory for password: %s", strerror(errno));
        free_z(n->username);
        free_z(n);
        return;
    }
    n->next = NULL;

    if (!cli->users)
        cli->users = n;
    else
    {
        for (u = cli->users; u && u->next; u = u->next);
        if (u) u->next = n;
    }
}

void cli_allow_enable(struct cli_def *cli, char *password)
{
    free_z(cli->enable_password);
    if (!(cli->enable_password = strdup(password)))
    {
        fprintf(stderr, "Couldn't allocate memory for enable password: %s", strerror(errno));
    }
}

void cli_deny_user(struct cli_def *cli, char *username)
{
    struct unp *u, *p = NULL;
    if (!cli->users) return;
    for (u = cli->users; u; u = u->next)
    {
        if (strcmp(username, u->username) == 0)
        {
            if (p)
                p->next = u->next;
            else
                cli->users = u->next;
            break;
        }
        p = u;
    }
}

void cli_set_banner(struct cli_def *cli, char *banner)
{
    if (banner && *banner)
        cli->banner = banner;
}

void cli_set_hostname(struct cli_def *cli, char *hostname)
{
    if (hostname && *hostname)
        cli->hostname = hostname;
}

void cli_set_promptchar(struct cli_def *cli, char *promptchar)
{
    cli->promptchar = promptchar;
}

int cli_build_shortest(struct cli_command *cmd_root, struct cli_command *commands)
{
    struct cli_command *c, *p;
    char *cp, *pp;
    int len;

    for (c = commands; c; c = c->next)
    {
        c->unique_len = 1;
        for (p = commands; p; p = p->next)
        {
            if (c == p)
                continue;
            
            /*Add for the same mode cmd,Dingyi*/
            if (c->mode != p->mode)
                continue;
            
            cp = c->command;
            pp = p->command;
            len = 1;

            while (*cp && *pp && *cp++ == *pp++)
                len++;

            if (len > c->unique_len)
                c->unique_len = len;
        }

        if (c->children)
            cli_build_shortest(cmd_root, c->children);
    }

    return CLI_OK;
}

int cli_set_privilege(struct cli_def *cli, int priv)
{
    int old = cli->privilege;
    cli->privilege = priv;

    if (priv != old)
    {
        cli_set_promptchar(cli, priv == PRIVILEGE_ADMIN ? "# " : "> ");
    }

    return old;
}

void cli_set_modestring(struct cli_def *cli, char *modestring)
{
        cli->modestring = modestring;
}

int cli_set_configmode(struct cli_def *cli, int mode, char *config_desc)
{
    int old = cli->mode;
    cli->mode = mode;

    if (mode != old)
    {
        if (!cli->mode)
        {
            // enable mode
            cli_set_modestring(cli, NULL);
        }
        else if (config_desc && *config_desc)
        {
            static char string[64];
            snprintf(string, sizeof(string), "(config-%s)", config_desc);
            cli_set_modestring(cli, string);
        }
        else
        {
            cli_set_modestring(cli, "(config)");
        }

    }

    return old;
}

struct cli_command *cli_register_command(struct cli_command **cmd_root, struct cli_command *parent, char *command, int (*callback)(struct cli_def *cli, char *, char **, int), int privilege, int mode, char *help)
{
    struct cli_command *c, *p;
    int i=0;
    
    if (!command) return NULL;
    
    for(i=0;i<MAX_CLI_COMMAND;i++)
    {
        if(g_cli_command[i].usage == CLI_COMMAND_FREE)
            break;            
    }
    if(i==MAX_CLI_COMMAND)
    {
        diag_printf("no free CLI command record\n");
        return NULL;
    }
    c = &g_cli_command[i];
    c->usage = CLI_COMMAND_BUSY;
    c->callback = callback;
    c->next = NULL;
    c->command = command;
    c->parent = parent;
    c->privilege = privilege;
    c->mode = mode;
    c->help = help;

    if (parent)
    {
        if (!parent->children)
        {
            parent->children = c;
        }
        else
        {
            for (p = parent->children; p && p->next; p = p->next);
            if (p) p->next = c;
        }
    }
    else
    {
        if (!*cmd_root)
        {
            *cmd_root = c;
        }
        else
        {
            for (p = *cmd_root; p && p->next; p = p->next);
            if (p)
                p->next = c;
        }
    }

    cli_build_shortest(*cmd_root, (parent) ? parent : *cmd_root);

    return c;
}

static void cli_free_command(struct cli_command *cmd)
{
    struct cli_command *c,*p;

    if(!cmd) return;
    for (c = cmd->children; c;)
    {
        p = c->next;
        cli_free_command(c);
        c = p;
    }
    memset(cmd,0,sizeof(struct cli_command));
}

int cli_unregister_command(struct cli_command **cmd_root, char *command)
{
    struct cli_command *c, *p = NULL;

    if (!command) return -1;
    if (!*cmd_root) return CLI_OK;

    for (c = *cmd_root; c; c = c->next)
    {
        if (strcmp(c->command, command) == 0)
        {
            if (p)
                p->next = c->next;
            else
                *cmd_root = c->next;

            cli_free_command(c);
            return CLI_OK;
        }
        p = c;
    }

    return CLI_OK;
}


int cli_arg_help(struct cli_def *cli, int cr_ok, char *entry, ...)
{
    va_list ap;
    char *desc;
    char buf[20];
    char *p;

    va_start(ap, entry);
    while (entry)
    {
        /* allow one %d */
        if ((p = strchr(entry, '%')) && !strchr(p+1, '%') && p[1] == 'd')
        {
            int v = va_arg(ap, int);
            snprintf(buf, sizeof(buf), entry, v);
            p = buf;
        }
        else
            p = entry;

        desc = va_arg(ap, char *);
        if (desc && *desc)
            cli_print(cli, "  %-20s %s", p, desc);
        else
            cli_print(cli, "  %s", p);

        entry = desc ? va_arg(ap, char *) : 0;
    }

    va_end(ap);
    if (cr_ok)
        cli_print(cli, "  <cr>");

    return CLI_OK;
}


int cli_show_help(struct cli_def *cli, struct cli_command *c)
{
    struct cli_command *p;

    for (p = c; p; p = p->next)
    {
        if (p->command && cli->privilege >= p->privilege &&
            (p->mode == cli->mode || p->mode == MODE_ANY))
        {
            if(p->command != "Onulaser")
                cli_error(cli, "  %-20s %s", p->command, p->help ? : "");
        }

        // only to show first cmd keyword
        // if (p->children) cli_show_help(cli, p->children);
    }

    return CLI_OK;
}


struct cli_def *cli_init(struct cli_command *cmd_root)
{
    struct cli_def *cli;
    if(NULL == cmd_root){
        cs_printf(" cmd_root NULL.\n");
        return 0;
    }

    memset(g_parser_word,0,MAX_WORDS_LEN*MAX_WORDS_NUM);
    memset(&g_cli_telnet,0,sizeof(struct cli_def));    
    memset(g_cli_telnet_buffer,0,MAX_PRINT_BUF_LEN);
    cli=&g_cli_telnet;
    memset(cli, 0, sizeof(struct cli_def));    
    
    cli->buf_size = MAX_PRINT_BUF_LEN;
    cli->buffer= g_cli_telnet_buffer;
    cli_free_history(cli);

    cli->commands = cmd_root;
    cli->privilege = cli->mode = -1;
    cli_set_privilege(cli, PRIVILEGE_GUEST);
    cli_set_configmode(cli, MODE_EXEC, 0);

    // Default to 1 second timeout intervals
    cli->timeout_tm.tv_sec = 1;
    cli->timeout_tm.tv_usec = 0;

    return cli;
}

void cli_unregister_all(struct cli_def *cli, struct cli_command *command)
{
    //struct cli_command *c, *p = NULL;

    if (!command) command = cli->commands;
    if (!command) return;

    int i;

    for(i=0;i<MAX_CLI_COMMAND;i++)
    {
        if(g_cli_command[i].usage == CLI_COMMAND_FREE)
            break;            
    }
#if 0
    for (c = command; c; )
    {
        p = c->next;

        // Unregister all child commands
        if (c->children)
            cli_unregister_all(cli, c->children);

        free_z(c->command);
        free_z(c->help);
        free_z(c);

        c = p;
    }
#endif
}

int cli_done(struct cli_def *cli)
{
    if (!cli) return CLI_OK;
    
    cli_free_history(cli);

    return CLI_OK;
}

int cli_add_history(struct cli_def *cli, char *cmd)
{
    int i;
    for (i = 0; i < MAX_HISTORY; i++)
    {
        if (!strlen(cli->history[i]))
        {
            if (i == 0 || strcasecmp(cli->history[i-1], cmd))
            {
                strcpy(cli->history[i], cmd);
            }
            return CLI_OK;
        }
    }
    // No space found, drop one off the beginning of the list
    memset(cli->history[0],'\0',MAX_LINE_LENTH);
    for (i = 0; i < MAX_HISTORY-1; i++)
        strcpy(cli->history[i],cli->history[i+1]);
    /*cli->history[MAX_HISTORY - 1] = cmd;*/
    strcpy(cli->history[MAX_HISTORY - 1], cmd);
    return CLI_OK;
}

void cli_free_history(struct cli_def *cli)
{
    int i;
    for (i = 0; i < MAX_HISTORY; i++)
    {
        if (strlen(cli->history[i]))
        {
            memset(cli->history[i],'\0',MAX_LINE_LENTH);
        }
    }
}

static int cli_parse_line(char *line, char *words[], int max_words)
{
    int nwords = 0;
    char *p = line;
    char *word_start = 0;
    int inquote = 0;

    while (*p)
    {
        if (!isspace(*p))
        {
            word_start = p;
            break;
        }
        p++;
    }

    while (nwords < max_words - 1)
    {
        // disable filter function
#if 0
        if (!*p || *p == inquote || (word_start && !inquote && (isspace(*p) || *p == '|')))
#endif
        if (!*p || *p == inquote || (word_start && !inquote && (isspace(*p)) ))
        {
            if (word_start)
            {
                int len = p - word_start;
                if(len >=  MAX_WORDS_LEN || nwords >= MAX_WORDS_NUM)
                {
                    diag_printf("MAX/Real word len %d/%d\n",MAX_WORDS_LEN,len);
                    diag_printf("MAX/Real word num %d/%d\n",MAX_WORDS_NUM,nwords);
                    return 0;
                }
                words[nwords] = g_parser_word[nwords];
                memcpy(words[nwords], word_start, len);
                words[nwords++][len] = 0;
            }

            if (!*p)
                break;

            if (inquote)
                p++; /* skip over trailing quote */

            inquote = 0;
            word_start = 0;
        }

        // disable quote func, now it only recognize first words
#if 1   
        else if (*p == '"' || *p == '\'')
        {
            inquote = *p++;
            word_start = p;
        }
#endif
        else
        {
            if (!word_start)
            {
                if (*p == '|')
                {
                #if 0/*Libo for static memory usage*/
                    if (!(words[nwords++] = strdup("|")))
                        return 0;
                #else
                    words[nwords++] = "|";
                #endif                                    
                }
                else if (!isspace(*p))
                    word_start = p;
            }

            p++;
        }
    }

    return nwords;
}

static char *join_words(int argc, char **argv)
{
    char *p;
    int len = 0;
    int i;

    for (i = 0; i < argc; i++)
    {
        if (i)
            len += 1;

        len += strlen(argv[i]);
    }

    p = malloc(len + 1);
    if(!p) return NULL;
    p[0] = 0;

    for (i = 0; i < argc; i++)
    {
        if (i)
            strcat(p, " ");

        strcat(p, argv[i]);
    }

    return p;
}

static int cli_find_command(struct cli_def *cli, struct cli_command *commands, int num_words, char *words[], int start_word, int filters[])
{
    struct cli_command *c, *again = NULL;
    int c_words = num_words;

    if (filters[0])
        c_words = filters[0];

    // Deal with ? for help
    if (!words[start_word])
        return CLI_ERROR;

    if (words[start_word][strlen(words[start_word]) - 1] == '?')
    {
        int l = strlen(words[start_word])-1;

        for (c = commands; c; c = c->next)
        {
            if (strncasecmp(c->command, words[start_word], l) == 0
                && (c->callback || c->children)
                && cli->privilege >= c->privilege
                && (c->mode == cli->mode || c->mode == MODE_ANY)){
                    if(c->command != "Onulaser")
                        cli_error(cli, "  %-20s %s", c->command, c->help ? : "");
                }
        }

        // parent has a callback, output <cr>
        if (commands->parent && commands->parent->callback)
        {
        	#if 1
			// modified by wujm@2016-04-08,when parent has a callback, output callback help
        	char *help_str[1] = {"?"};
            commands->parent->callback(cli,commands->parent->command,help_str,1);
			#else
			cli_error(cli, "  %-20s %s", "<cr>", commands->parent->help ? : "");
			#endif
        }

        // cli_error(cli, "-- ? for help");
        return CLI_OK;
    }

    for (c = commands; c; c = c->next)
    {
        if (cli->privilege < c->privilege)
            continue;

        if (strncasecmp(c->command, words[start_word], c->unique_len))
            continue;

        if (strncasecmp(c->command, words[start_word], strlen(words[start_word])))
            continue;

        AGAIN:
        if (c->mode == cli->mode || c->mode == MODE_ANY)
        {
            int rc = CLI_OK;
#ifdef HAVE_CLI_FILTER
            int f;
            struct cli_filter **filt = &cli->filters;
#endif
            // Found a word!
            if (!c->children)
            {
                // Last word
                if (!c->callback)
                {
                    cli_error(cli, "No callback for \"%s\"", cli_command_name(cli, c));
                    return CLI_ERROR;
                }
            }
            else
            {
                if (start_word == c_words - 1)
                {
                    if (c->callback)
                        goto CORRECT_CHECKS;

                    cli_error(cli, "Incomplete command");
                    return CLI_ERROR;
                }
                rc = cli_find_command(cli, c->children, num_words, words, start_word + 1, filters);
                if (rc == CLI_ERROR_ARG)
                {
                    if (c->callback)
                    {
                        rc = CLI_OK;
                        goto CORRECT_CHECKS;
                    }
                    else
                    {
                        cli_error(cli, "Invalid %s \"%s\"", commands->parent ? "argument" : "command", words[start_word]);
                    }
                }
                return rc;
            }

            if (!c->callback)
            {
                cli_error(cli, "Internal server error processing \"%s\"", cli_command_name(cli, c));
                return CLI_ERROR;
            }

            CORRECT_CHECKS:
#ifdef HAVE_CLI_FILTER
            for (f = 0; rc == CLI_OK && filters[f]; f++)
            {
                int n = num_words;
                char **argv;
                int argc;
                int len;

                if (filters[f+1])
                n = filters[f+1];

                if (filters[f] == n - 1)
                {
                    cli_error(cli, "Missing filter");
                    return CLI_ERROR;
                }

                argv = words + filters[f] + 1;
                argc = n - (filters[f] + 1);
                len = strlen(argv[0]);
                if (argv[argc - 1][strlen(argv[argc - 1]) - 1] == '?')
                {
                    if (argc == 1)
                    {
                        int i;

                        for(i = 0; filter_cmds[i].cmd; i++)
                        {
                            cli_error(cli, "  %-20s %s", filter_cmds[i].cmd, filter_cmds[i].help );
                        }
                    }
                    else
                    {
                        if (argv[0][0] != 'c') // count
                            cli_error(cli, "  WORD");

                        if (argc > 2 || argv[0][0] == 'c') // count
                            cli_error(cli, "  <cr>");
                    }

                    return CLI_OK;
                }

                if (argv[0][0] == 'b' && len < 3) // [beg]in, [bet]ween
                {
                    cli_error(cli, "Ambiguous filter \"%s\" (begin, between)", argv[0]);
                    return CLI_ERROR;
                }
                *filt = malloc(sizeof(struct cli_filter));
                if(!*filt) return CLI_ERROR;

				memset(*filt, 0, sizeof(struct cli_filter));
                if (!strncmp("include", argv[0], len) ||
                    !strncmp("exclude", argv[0], len) ||
                    !strncmp("grep", argv[0], len) ||
                    !strncmp("egrep", argv[0], len))
                        rc = cli_match_filter_init(cli, argc, argv, *filt);
                else if (!strncmp("begin", argv[0], len) ||
                    !strncmp("between", argv[0], len))
                        rc = cli_range_filter_init(cli, argc, argv, *filt);
                else if (!strncmp("count", argv[0], len))
                    rc = cli_count_filter_init(cli, argc, argv, *filt);
                else
                {
                    cli_error(cli, "Invalid filter \"%s\"", argv[0]);
                    rc = CLI_ERROR;
                }

                if (rc == CLI_OK)
                {
                    filt = &(*filt)->next;
                }
                else
                {
                    free_z(*filt);
                    *filt = 0;
                }
            }
#endif

            if (rc == CLI_OK)
            {
                rc = c->callback(cli, cli_command_name(cli, c), words + start_word + 1, c_words - start_word - 1);
                // cli_error(cli, "-- cmd handler got invoked \n");
            }

            while (cli->filters)
            {
                struct cli_filter *filt = cli->filters;

                // call one last time to clean up
                filt->filter(cli, NULL, filt->data);
                cli->filters = filt->next;
                free_z(filt);
            }

            return rc;
        }
        else if (cli->mode > MODE_CONFIG && c->mode == MODE_CONFIG)
        {
            // command matched but from another mode,
            // remember it if we fail to find correct command
            again = c;
        }
    }

    // drop out of config submode if we have matched command on MODE_CONFIG
    
    if (again)
    {
        c = again;
        cli_set_configmode(cli, MODE_CONFIG, NULL);
        goto AGAIN;
    }

    if ((start_word == 0) && commands)
        cli_error(cli, "Invalid %s \"%s\"", commands->parent ? "argument" : "command", words[start_word]);

    return CLI_ERROR_ARG;
}

int cli_run_command(struct cli_def *cli, char *command)
{
    int r;
    unsigned int num_words, i;
    char *words[MAX_WORDS_NUM] = {0};
    int filters[MAX_FILTER_NUM] = {0};
#ifdef HAVE_CLI_FILTER
		unsigned int f;
#endif

    if (!command) return CLI_ERROR;
    while (isspace(*command))
        command++;

    if (!*command) return CLI_OK;

    num_words = cli_parse_line(command, words, sizeof(words) / sizeof(words[0]));

    // disable filter function
#ifdef HAVE_CLI_FILTER
    for (i = f = 0; i < num_words && f < sizeof(filters) / sizeof(filters[0]) - 1; i++)
    {
        if (words[i][0] == '|')
            filters[f++] = i;
    }
    filters[f] = 0;
#endif

    if (num_words)
        r = cli_find_command(cli, cli->commands, num_words, words, 0, filters);
    else
        r = CLI_ERROR;

    for (i = 0; i < num_words; i++)
    {
        words[i]=NULL;
    }

    if (r == CLI_ERROR || r == CLI_ERROR_ARG)
        cyg_thread_delay(50);

    if (r == CLI_QUIT)
        return r;

    return CLI_OK;
}

int cli_get_completions(struct cli_def *cli, char *command,char *commonstr, char **completions, int max_completions)
{
    struct cli_command *c;
    struct cli_command *n;
    int num_words, i, k=0;
    char *words[MAX_WORDS_NUM] = {0};
	char *cp;
	int len = 0;
	
#ifdef HAVE_CLI_FILTER
    int filter = 0;
#endif

	if (!command || !commonstr) return 0;
    while (isspace(*command))
        command++;

    num_words = cli_parse_line(command, words, sizeof(words)/sizeof(words[0]));
    if (!command[0] || command[strlen(command)-1] == ' ')
        num_words++;

    if (!num_words)
            return 0;

    // disable filter function
#ifdef HAVE_CLI_FILTER
    for (i = 0; i < num_words; i++)
    {
        if (words[i] && words[i][0] == '|')
            filter = i;
    }

    if (filter) // complete filters
    {
        if (filter < num_words - 1) // filter already completed
            return 0;

        if (filter == num_words - 1)
            len = strlen(words[num_words-1]);

        for (i = 0; filter_cmds[i].cmd && k < max_completions; i++)
            if (!len || (len < strlen(filter_cmds[i].cmd)
                && !strncmp(filter_cmds[i].cmd, words[num_words - 1], len)))
                    completions[k++] = filter_cmds[i].cmd;

        completions[k] = NULL;
        return k;
    }
#endif

	len = -1;
    for (c = cli->commands, i = 0; c && i < num_words && k < max_completions; c = n)
    {
        n = c->next;
        
        if (cli->privilege < c->privilege)
            continue;

        if (c->mode != cli->mode && c->mode != MODE_ANY)
            continue;
        

		if (words[i])
		{
			if ( 0 > len )
			{
				len = strlen(words[i]);
			}

			if ( strncasecmp(c->command, words[i], len) )
            continue;

        if (i < num_words - 1)
        {
			 if ( (len < c->unique_len)
					&& (c->command[len] != '\0') /* added by liwei056@2016-01-29 for find min match string */
				   )
                    continue;

            n = c->children;
            i++;
				len = -1;
            continue;
        }
		}

        completions[k++] = c->command;
    }

#if 1 
	/* added by liwei056@2016-01-29 for find min common string */
	if ( k > 1 )
	{
		strcpy(commonstr, completions[0]); 
		cp = commonstr+len;
	   
		while( *cp )
		{
			for(i = 1; i < k; i++)
			{
				if ( *cp != completions[i][len] )
				{
					break;
				}
			}

			if ( i < k )
			{
				*cp = '\0';
				break;
			}

			cp++;
			len++;
		}
	}
#endif

    // free temp words
    for (i = 0; i < num_words; i++)
        words[i]=NULL;

    return k;
}

#if 0
static int telnet_cli_raw_output(struct cli_def *cli, char* s, int len)
{
    static char buffer[MAX_LINE_LENTH];
    static int real_len = 0;
    
    if (!cli) return 0;

#ifdef HAVE_TELNET_CLI
    if(CHANNEL_TCP == cli->channel)
    {
        if(len > 0)
        {   
            buffer[real_len++] = *s;

            if( real_len > 120 || *s == '\r' || *s == '\n' )
            {   
                send(cli->sockfd, buffer, real_len, 0);
                if(*s != '\n'){
                    send(cli->sockfd, "\r\n", 2, 0);
                }
                real_len = 0;
                memset(buffer, 0, MAX_LINE_LENTH);
                if(cli->client)
                    fflush(cli->client);/*fflush the output*/
                return real_len;
            }            
            else
            {
            #if 0
                buffer[real_len-1] = *s;
                real_len ++;
            #endif
                return 0;
            }
            
            
        }
        else
            return 0;
    }
    else
#endif
    {
        mon_write_chars(s, len);
        return len;
    }
}
#endif

#if 0	//modified by zhuxh
static int cli_raw_output(struct cli_def *cli, char* s, int len)
{
    cs_uint32 i = 0;
    
    if (!cli) return 0;

    for(i = 0; i < len; i++) {
        console_put_char(s[i]);
    }

    return len;
}

#else
//solved bug :when login onu by serial port, you input a character or characters, if you don't press Enter key,there is no echo

#ifdef HAVE_GWD_OAM_PTY
extern int write_to_oam_pty(char *str,int length);
#endif
static int cli_raw_output(struct cli_def *cli, char* s, int len)
{
    if (!cli) return 0;

    if(CHANNEL_TCP == cli->channel)
    {
    	#if 0
        if(len > 0)
        {
            int real_len = write(cli->sockfd, s, len);
            if(cli->client)
                fflush(cli->client);/*fflush the output*/
            return real_len;
        }
        else
            return 0;
		#else
		//added by zhuxh,solved bug:olt login onu failed,cann't input character string
		cs_uint32 i = 0;
		for(i = 0; i < len; i++) 
		{
			console_put_char(s[i]);
		}
		return len;
		#endif
    }
	#ifdef HAVE_GWD_OAM_PTY
	else if(CHANNEL_PTY == cli->channel)
	{
		int real_len = write_to_oam_pty(s, len);
    	return real_len;
	}
	#endif
    else
    {
        mon_write_chars(s, len);
        return len;
    }
}
#endif

static void cli_clear_line(struct cli_def *cli, char *cmd, int l, int cursor)
{
    int i;

    if (cursor < l) for (i = 0; i < (l - cursor); i++) cli_raw_output(cli, " ", 1);
    for (i = 0; i < l; i++) cmd[i] = '\b';
    cli_raw_output(cli, cmd, i);
    for (i = 0; i < l; i++) cmd[i] = ' ';
    cli_raw_output(cli, cmd, i);
    for (i = 0; i < l; i++) cmd[i] = '\b';
    cli_raw_output(cli, cmd, i);

    memset(cmd, 0, l);
    l = cursor = 0;
}

void cli_reprompt(struct cli_def *cli)
{
    if (!cli) return;
    cli->showprompt = 1;
}

void cli_regular(struct cli_def *cli, int (*callback)(struct cli_def *cli))
{
    if (!cli) return;
    cli->regular_callback = callback;
}

void cli_regular_interval(struct cli_def *cli, int seconds)
{
    if (seconds < 1) seconds = 1;
    cli->timeout_tm.tv_sec = seconds;
    cli->timeout_tm.tv_usec = 0;
}

#define DES_PREFIX "{crypt}"        /* to distinguish clear text from DES crypted */
#define MD5_PREFIX "$1$"

static int pass_matches(char *pass, char *try)
{
#if 0
    int des;
    if ((des = !strncasecmp(pass, DES_PREFIX, sizeof(DES_PREFIX)-1)))
        pass += sizeof(DES_PREFIX)-1;

    /*
     * TODO - find a small crypt(3) function for use on ecos
     */
    if (des || !strncmp(pass, MD5_PREFIX, sizeof(MD5_PREFIX)-1))
        try = crypt(try, pass);
#endif

    return !strcmp(pass, try);
}



static int show_prompt(struct cli_def *cli)
{
    int len = 0;

    if (cli->hostname)
        len += cli_raw_output(cli, cli->hostname, strlen(cli->hostname));

    if (cli->modestring)
        len += cli_raw_output(cli, cli->modestring, strlen(cli->modestring));

    return len + cli_raw_output(cli, cli->promptchar, strlen(cli->promptchar));
}


#ifdef HAVE_GWD_OAM_PTY
extern int read_from_oam_pty(char *str,int length);
extern void oam_pty_shell_close(void);
#endif
int cli_loop(struct cli_def *cli)
{
    unsigned char c;
    int l, oldl = 0, esc = 0;
    int cursor = 0, insertmode = 1;
    char *oldcmd = NULL;
    int res = 0;
    char username[65], *password = NULL;
    char cmd[MAX_LINE_LENTH];
    int n, skip = 0;
    int is_telnet_option = 0;
    char *negotiate =
        "\xFF\xFB\x03"
        "\xFF\xFB\x01"
        "\xFF\xFD\x03"
        "\xFF\xFD\x01";

    cli_free_history(cli);
    memset(cmd,0,MAX_LINE_LENTH);

    memset(username,0,64);
    
    if(CHANNEL_TCP == cli->channel)
    {
        cli->state = STATE_LOGIN;
        write(cli->sockfd, negotiate, strlen(negotiate));

        /*if (!(cli->client = fdopen(cli->sockfd, "w+")))
            return CLI_ERROR;

        setbuf(cli->client, NULL);*/
    }

    if (cli->banner)
        cli_error(cli, "%s", cli->banner);

    // Set the last action now so we don't time immediately
    if (cli->idle_timeout)
        time(&cli->last_action);

    /* start off in unprivileged mode */
    cli_set_configmode(cli, MODE_EXEC, NULL);

    if(CHANNEL_TCP == cli->channel)
    {
        cli_set_privilege(cli, PRIVILEGE_GUEST);

        if (!cli->users && !cli->auth_callback)
            cli->state = STATE_NORMAL;
    }
    else
    {
        cli_set_privilege(cli, PRIVILEGE_ADMIN);
        cli->state = STATE_NORMAL;
    }

    while (1)
    {
        signed int in_history = 0;
        int lastchar = 0;

        cli->showprompt = 1;

        if (oldcmd)
        {
            l = cursor = oldl;
            oldcmd[l] = 0;
            cli->showprompt = 1;
            oldcmd = NULL;
            oldl = 0;
        }
        else
        {
            memset(cmd, 0, MAX_LINE_LENTH);
            l = 0;
            cursor = 0;
        }

#ifdef HAVE_TELNET_CLI
        struct timeval tm;
        memcpy(&tm, &cli->timeout_tm, sizeof(tm));
#endif

        while (1)
        {
            if (cli->showprompt)
            {
#if 0            
                if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD && cli->state != STATE_CONFIRM_PASSWORD)
                    cli_raw_output(cli, "\r\n", 2);
#endif
                switch (cli->state)
                {
                    case STATE_LOGIN:
                        cli_raw_output(cli, "Username: ", strlen("Username: "));
                        break;

                    case STATE_PASSWORD:
                        cli_raw_output(cli, "Password: ", strlen("Password: "));
                        break;

                    case STATE_NORMAL:
                    case STATE_ENABLE:
                        show_prompt(cli);
                        cli_raw_output(cli, cmd, l);
                        if (cursor < l)
                        {
                            int n = l - cursor;
                            while (n--)
                                cli_raw_output(cli, "\b", 1);
                        }
                        break;

                    case STATE_ENABLE_PASSWORD:
                        cli_raw_output(cli, "Password: ", strlen("Password: "));
                        break;

                    case STATE_CONFIRM_PASSWORD:
                        cli_raw_output(cli, "Enter Password: ", strlen("Enter Password: "));
                        break;

                    case STATE_SAVE:
                        cli_raw_output(cli, "Need Save? Yes/No:", strlen("Need Save? Yes/No:"));
                        break;

                    case STATE_QUIT:
                        cli_raw_output(cli, "Confirm Quit Cmd? Yes/No:", strlen("Confirm Quit Cmd? Yes/No:"));
                        break;

                }

                cli->showprompt = 0;
            }

#ifdef HAVE_TELNET_CLI
            int sr;
            fd_set r;

            if(CHANNEL_TCP == cli->channel)
            {
                 if (app_ip_changed==1)
                 {
                    app_ip_changed=0;
                    cli_error(cli, "IP change");
                    strcpy(cmd, "QUIT");
                    break;
                }

                FD_ZERO(&r);
                FD_SET(cli->sockfd, &r);

                if ((sr = select(cli->sockfd + 1, &r, NULL, NULL, &tm)) < 0)
                {
                    /* select error */
                    if (errno == EINTR)
                    {
                        cur_chan = CHANNEL_SERIAL;
                        telnet_diag_print_unregister();
                        telnet_cli = NULL; 
                        continue;
                    }

                    perror("select\n");
                    l = -1;
                    break;
                }

                if (sr == 0)
                {
                    if (cli->regular_callback && cli->regular_callback(cli) != CLI_OK)
                    {
                        strncpy(cmd, "QUIT", MAX_LINE_LENTH-1);
                        break;
                    }

                    if (cli->idle_timeout)
                    {
                        if (time(NULL) - cli->last_action >= cli->idle_timeout)
                        {
                            /* Idle timeout */
                            strncpy(cmd, "QUIT", MAX_LINE_LENTH-1);
                            break;
                        }
                    }

                    memcpy(&tm, &cli->timeout_tm, sizeof(tm));
                    continue;
                }

                if ((n = read(cli->sockfd, &c, 1)) < 0)
                {
                    if (errno == EINTR)
                    {
                        perror("ERR: sockfd broken, switch to Console\n");
                        cur_chan = CHANNEL_SERIAL;
                        telnet_diag_print_unregister();
                        telnet_cli = NULL; 
                        continue;
                    }

                    perror("read\n");
                    l = -1;
                    break;
                }


                if (cli->idle_timeout)
                    time(&cli->last_action);

                if (n == 0)
                {
                    l = -1;
                    perror("sock read 0\n");
                    break;
                }

                if (skip)
                {
                    skip--;
                    continue;
                }


                if (c == 255 && !is_telnet_option)
                {
                    is_telnet_option++;
                    continue;
                }

                if (is_telnet_option)
                {
                    if (c >= 251 && c <= 254)
                    {
                        is_telnet_option = c;
                        continue;
                    }

                    if (c != 255)
                    {
                        is_telnet_option = 0;
                        continue;
                    }

                    is_telnet_option = 0;
                }
            }
            else
#endif
			#ifdef HAVE_GWD_OAM_PTY
			if(CHANNEL_PTY == cli->channel)
			{
				n = read_from_oam_pty(&c,1);			
				if(n < 0)
				{
					continue;
				}
			}
			else
			#endif
            {
                // read from console
                while(1)
                {
                    res = mon_read_char_with_timeout(&c);
                    if (CHANNEL_SERIAL == cur_chan && res) {
                        // Got a character
                        break;
                    }
                }

                if (cli->idle_timeout)
                    time(&cli->last_action);
            }

            /* handle ANSI arrows */
            if (esc)
            {
                if (esc == '[')
                {
                    /* remap to readline control codes */
                    switch (c)
                    {
                        case 'A': /* Up */
                            c = CTRL('P');
                            break;

                        case 'B': /* Down */
                            c = CTRL('N');
                            break;

                        case 'C': /* Right */
                            c = CTRL('F');
                            break;

                        case 'D': /* Left */
                            c = CTRL('B');
                            break;

                        default:
                            c = 0;
                    }

                    esc = 0;
                }
                else
                {
                    esc = (c == '[') ? c : 0;
                    continue;
                }
            }


            if (c == 0) continue;
			#if 0
            if (c == '\n') continue;
			#else
			{
				//modified by zhuxh,for oam pty input Enter, no act
				static unsigned char ch_front = 0;
				static unsigned char ch_rear = 0;
				ch_front = ch_rear;
				ch_rear = c;

				if(('\r' == ch_front)&&('\n' == c))
				{
					continue;
				}
				else
				{
					//do nothing
				}
				if('\n' == c)
				{
					c = '\r';
				}
			}
			#endif

            if (c == '\r')
            {
                if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD && cli->state != STATE_CONFIRM_PASSWORD)
                    cli_raw_output(cli, "\r\n", 2);

                break;
            }

            if (c == 27)
            {
                esc = 1;
                continue;
            }

            if (c == CTRL('C'))
            {
                cli_raw_output(cli, "\a", 1);
                continue;
            }

            /* back word, backspace/delete */
            if (c == CTRL('W') || c == CTRL('H') || c == 0x7f)
            {
                int back = 0;

                if (c == CTRL('W')) /* word */
                {
                    int nc = cursor;

                    if (l == 0 || cursor == 0)
                        continue;

                    while (nc && cmd[nc - 1] == ' ')
                    {
                        nc--;
                        back++;
                    }

                    while (nc && cmd[nc - 1] != ' ')
                    {
                        nc--;
                        back++;
                    }
                }
                else /* char */
                {
                    if (l == 0 || cursor == 0)
                    {
                        cli_raw_output(cli, "\a", 1);
                        continue;
                    }

                    back = 1;
                }

                if (back)
                {
                    while (back--)
                    {
                        if (l == cursor)
                        {
                            cmd[--cursor] = 0;
                            cli_raw_output(cli, "\b \b", 3);
                        }
                        else
                        {
                            int i;
                            cursor--;
                            {
                                for (i = cursor; i <= l; i++) cmd[i] = cmd[i+1];
                                cli_raw_output(cli, "\b", 1);
                                if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD && cli->state != STATE_CONFIRM_PASSWORD){
                                    cli_raw_output(cli, cmd + cursor, strlen(cmd + cursor));
                                }
                                else{
                                    for(i=0; i<=(int)strlen(cmd + cursor);i++) cli_raw_output(cli, "*", 1);     
                                }                                    
                                cli_raw_output(cli, " ", 1);
                                for (i = 0; i <= (int)strlen(cmd + cursor); i++)
                                    cli_raw_output(cli, "\b", 1);
                            }
                        }
                        l--;
                    }

                    continue;
                }
            }

            /* redraw */
            if (c == CTRL('L'))
            {
                int i;
                int cursorback = l - cursor;

                if (cli->state == STATE_PASSWORD || cli->state == STATE_ENABLE_PASSWORD || cli->state == STATE_CONFIRM_PASSWORD)
                    continue;

                cli_raw_output(cli, "\r\n", 2);
                show_prompt(cli);
                cli_raw_output(cli, cmd, l);

                for (i = 0; i < cursorback; i++)
                    cli_raw_output(cli, "\b", 1);

                continue;
            }

            /* clear line */
            if (c == CTRL('U'))
            {
                if (cli->state == STATE_PASSWORD || cli->state == STATE_ENABLE_PASSWORD || cli->state == STATE_CONFIRM_PASSWORD)
                    memset(cmd, 0, l);
                else
                    cli_clear_line(cli, cmd, l, cursor);

                l = cursor = 0;
                continue;
            }

            /* kill to EOL */
            if (c == CTRL('K'))
            {
                if (cursor == l)
                    continue;

                if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD && cli->state != STATE_CONFIRM_PASSWORD)
                {
                    int c;
                    for (c = cursor; c < l; c++)
                        cli_raw_output(cli, " ", 1);

                    for (c = cursor; c < l; c++)
                        cli_raw_output(cli, "\b", 1);
                }

                memset(cmd + cursor, 0, l - cursor);
                l = cursor;
                continue;
            }

            /* EOT */
            if (c == CTRL('D'))
            {
                if (cli->state == STATE_PASSWORD || cli->state == STATE_ENABLE_PASSWORD || cli->state == STATE_CONFIRM_PASSWORD)
                    break;

                if (l)
                    continue;

                strcpy(cmd, "QUIT");
                l = cursor = strlen(cmd);
                cli_raw_output(cli, "QUIT\r\n", l + 2);
                break;
            }

            /* disable */
            if (c == CTRL('Z'))
            {
                if (cli->mode != MODE_EXEC)
                {
                    cli_clear_line(cli, cmd, l, cursor);
                    cli_set_configmode(cli, MODE_EXEC, NULL);
                    cli->showprompt = 1;
                }

                continue;
            }

            /* TAB completion */
            if (c == CTRL('I'))
            {
                char *completions[MAX_WORDS_NUM];
		char  commonstr[MAX_WORDS_LEN];
                int num_completions = 0;
                if (cli->state == STATE_LOGIN || cli->state == STATE_PASSWORD || cli->state == STATE_ENABLE_PASSWORD)
                    continue;

                if (cursor != l) continue;
		/*modified by liyang @2016-01-29 for get min common string*/
		num_completions = cli_get_completions(cli, cmd, commonstr, completions, MAX_WORDS_NUM);

                if (num_completions == 0)
                {
                    cli_raw_output(cli, "\a", 1);
                }
                else if (num_completions == 1)
                {
                    // Single completion
                    for (; l > 0; l--, cursor--)
                    {
                        if (cmd[l-1] == ' ' || cmd[l-1] == '|')
                            break;
                        cli_raw_output(cli, "\b", 1);
                    }
                    strcpy((cmd + l), completions[0]);
                    l += strlen(completions[0]);
                    cmd[l++] = ' ';
                    cursor = l;
                    cli_raw_output(cli, completions[0], strlen(completions[0]));
                    cli_raw_output(cli, " ", 1);
                }
                else if (lastchar == CTRL('I'))
                {
                    // double tab
			/* modified by liyang@2016-01-29 for output complete token and change raw display to  column display*/
#if 0  
                    int i;
                    cli_raw_output(cli, "\r\n", 2);
                    for (i = 0; i < num_completions; i++)
                    {
                        cli_raw_output(cli, completions[i], strlen(completions[i]));
                        if (i % 4 == 3)
                            cli_raw_output(cli, "\r\n", 2);
                        else
                            cli_raw_output(cli, "     ", 1);
                    }
                    if (i % 4 != 3) cli_raw_output(cli, "\r\n", 2);
                        cli->showprompt = 1;
#else	
			int i;

			/*added by liyang for output complete token after backspace */
			for (; l > 0; l--, cursor--)
			{
				if (cmd[l-1] == ' ' || cmd[l-1] == '|')
					break;
				cli_raw_output(cli, "\b", 1);
			}
			strcpy((cmd + l), commonstr);
			l += strlen(commonstr);
	
			cursor = l;
			cli_raw_output(cli, commonstr, strlen(commonstr));

			/* change raw display to  column display */
			cli_raw_output(cli, "\r\n", 2);
			for (i = 0; i < num_completions; i++)
			{
				cli_raw_output(cli,"  ",2);
				cli_raw_output(cli, completions[i], strlen(completions[i]));

				cli_raw_output(cli, "\r\n", 2);
			}
			cli->showprompt = 1;		
#endif
                }
                else
                {
                    // More than one completion
			/* modified by liyang @2016-01-29 for output completion token*/
#if 1  
			lastchar = c;
			
			for (; l > 0; l--, cursor--)
			{
				if (cmd[l-1] == ' ' || cmd[l-1] == '|')
					break;
				cli_raw_output(cli, "\b", 1);
			}
			strcpy((cmd + l), commonstr);
			l += strlen(commonstr);
				
			cursor = l;
			cli_raw_output(cli, commonstr, strlen(commonstr));
#else
                    lastchar = c;
                    cli_raw_output(cli, "\a", 1);
#endif
                }
                continue;
            }

            /* history */
            if (c == CTRL('P') || c == CTRL('N'))
            {
                int history_found = 0;

                if (cli->state == STATE_LOGIN || cli->state == STATE_PASSWORD || cli->state == STATE_ENABLE_PASSWORD || cli->state == STATE_CONFIRM_PASSWORD)
                    continue;

                if (c == CTRL('P')) // Up
                {
                    in_history--;
                    if (in_history < 0)
                    {
                        for (in_history = MAX_HISTORY-1; in_history >= 0; in_history--)
                        {
                            if (strlen(cli->history[in_history]))
                            {
                                history_found = 1;
                                break;
                            }
                        }
                    }
                    else
                    {
                        if (strlen(cli->history[in_history])) history_found = 1;
                    }
                }
                else // Down
                {
                    in_history++;
                    if (in_history >= MAX_HISTORY || !strlen(cli->history[in_history]))
                    {
                        int i = 0;
                        for (i = 0; i < MAX_HISTORY; i++)
                        {
                            if (cli->history[i])
                            {
                                in_history = i;
                                history_found = 1;
                                break;
                            }
                        }
                    }
                    else
                    {
                        if (strlen(cli->history[in_history])) history_found = 1;
                    }
                }
                if (history_found && strlen(cli->history[in_history]))
                {
                    // Show history item
                    cli_clear_line(cli, cmd, l, cursor);
                    memset(cmd, 0, MAX_LINE_LENTH);
                    strncpy(cmd, cli->history[in_history], MAX_LINE_LENTH-1);
                    l = cursor = strlen(cmd);
                    cli_raw_output(cli, cmd, l);
                }

                continue;
            }

            /* left/right cursor motion */
            if (c == CTRL('B') || c == CTRL('F'))
            {
                if (c == CTRL('B')) /* Left */
                {
                    if (cursor)
                    {
                        if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD && cli->state != STATE_CONFIRM_PASSWORD)
                            cli_raw_output(cli, "\b", 1);

                        cursor--;
                    }
                }
                else /* Right */
                {
                    if (cursor < l)
                    {
                        if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD && cli->state != STATE_CONFIRM_PASSWORD)
                            cli_raw_output(cli, &cmd[cursor], 1);

                        cursor++;
                    }
                }

                continue;
            }

            /* start of line */
            if (c == CTRL('A'))
            {
                if (cursor)
                {
                    if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD && cli->state != STATE_CONFIRM_PASSWORD)
                    {
                        cli_raw_output(cli, "\r", 1);
                        show_prompt(cli);
                    }

                    cursor = 0;
                }

                continue;
            }

            /* end of line */
            if (c == CTRL('E'))
            {
                if (cursor < l)
                {
                    if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD&& cli->state != STATE_CONFIRM_PASSWORD)
                        cli_raw_output(cli, &cmd[cursor], l - cursor);

                    cursor = l;
                }

                continue;
            }

            /* normal character typed */
            if (cursor == l)
            {
                 /* append to end of line */
                cmd[cursor] = c;
                if (l < MAX_LINE_LENTH-1)
                {
                    l++;
                    cursor++;
                }
                else
                {
                    cmd[MAX_LINE_LENTH-1] = 0;
                    cli_raw_output(cli, "\a", 1);
                    continue;
                    //cli_raw_output(cli, "\r\n", 2);
                    //break;
                }
            }
            else
            {
                // Middle of text
                if (insertmode)
                {
                    int i;
                    // Move everything one character to the right
                    if (l >= MAX_LINE_LENTH-2) l--;
                    for (i = l; i >= cursor; i--)
                        cmd[i + 1] = cmd[i];
                    // Write what we've just added
                    cmd[cursor] = c;

                    cli_raw_output(cli, &cmd[cursor], l - cursor + 1);
                    for (i = 0; i < (l - cursor + 1); i++)
                        cli_raw_output(cli, "\b", 1);
                    l++;
                }
                else
                {
                    cmd[cursor] = c;
                }
                cursor++;
            }

            if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD && cli->state != STATE_CONFIRM_PASSWORD)
            {
                if (c == '?' && cursor == l)
                {
                    
                    cli_raw_output(cli, "\r\n", 2);
                    oldcmd = cmd;
                    oldl = cursor = l - 1;
                    break;
                }
                
                cli_raw_output(cli, &c, 1);
            }
            else
            {
                /*Output * for passwd*/
                char star='*';
                cli_raw_output(cli, &star, 1);
            }
            oldcmd = 0;
            oldl = 0;
            lastchar = c;
        }

        /*one cmd enter,parse it now.*/
        if (l < 0) break;
        
		#if 0
        if (!strcasecmp(cmd, "quit") && CHANNEL_TCP == cli->channel) break;
        #else
		//modified by zhuxh,ecept serial mode,when input quit,end command process
		if (!strcasecmp(cmd, "quit") && CHANNEL_SERIAL != cli->channel)
		{
			break;
		}
		#endif
		
        if (cli->state == STATE_LOGIN)
        {
            if (l == 0) continue;

            /* require login */
            #if 0/*Libo for static memory usage*/
            free_z(username);
            if (!(username = strdup(cmd)))
                return 0;
            #else
            //username = cmd;
            memset(username,0x00,sizeof(username));
            memcpy(username, cmd, 64);
            #endif
            
            cli->state = STATE_PASSWORD;
            cli->showprompt = 1;
        }
        else if (cli->state == STATE_PASSWORD)
        {
            /* require password */
            int allowed = 0;
            #if 0/*Libo for static memory usage*/
            free_z(password);
            if (!(password = strdup(cmd)))
                return 0;
            #else
            password = cmd;
            #endif
            if (cli->auth_callback)
            {
                if (cli->auth_callback(username, password, cli) == CLI_OK)
                    allowed++;
            }

            if (!allowed)
            {
                struct unp *u;
                for (u = cli->users; u; u = u->next)
                {
                    if (!strcmp(u->username, username) && pass_matches(u->password, password))
                    {
                        allowed++;
                        break;
                    }
                }
            }

            if (allowed)
            {
                cli_error(cli, "\n");
                cli->state = STATE_NORMAL;
                if(CHANNEL_TCP == cli->channel)
                    cur_chan = CHANNEL_TCP;
				#ifdef HAVE_GWD_OAM_PTY
				if(CHANNEL_PTY == cli->channel)
				{
					cur_chan = CHANNEL_PTY;
				}
				#endif
            }
            else
            {
                cli_error(cli, "\n\nAccess denied");
                //free_z(username);
                //free_z(password);
                cli->state = STATE_LOGIN;
            }

            cli->showprompt = 1;
        }
        else if (cli->state == STATE_CONFIRM_PASSWORD)
        {
            if(set_auth(0,NULL, cmd, cli)!=CLI_OK)
                cli_error(cli, "\n\nAdd user account fail\n");
            cli->state = STATE_NORMAL;
            if(CHANNEL_TCP == cli->channel)
                cur_chan = CHANNEL_TCP;

        }
        else if (cli->state == STATE_ENABLE_PASSWORD)
        {
            int allowed = 0;
            if (cli->enable_password)
            {
                /* check stored static enable password */
                if (pass_matches(cli->enable_password, cmd))
                    allowed++;
            }

            if (!allowed && cli->enable_callback)
            {
                /* check callback */
                if (cli->enable_callback(cmd))
                    allowed++;
            }

            if (allowed)
            {
                cli->state = STATE_ENABLE;
                cli_set_privilege(cli, PRIVILEGE_ADMIN);
            }
            else
            {
                cli_error(cli, "\n\nAccess denied");
                cli->state = STATE_NORMAL;
            }
        }
        else
        {
            if (l == 0) continue;
            if (cmd[l - 1] != '?' && strcasecmp(cmd, "history") != 0)
            {
                int his_ret = cli_add_history(cli, cmd);
                if(his_ret != CLI_OK)
                    diag_printf("add history failed.\n");
            }

			#if 0
            if (CLI_QUIT == cli_run_command(cli, cmd) && CHANNEL_TCP == cli->channel)
                break;
			#else
			//modified by zhuxh,ecept serial mode,when input quit,end command process
			if (CLI_QUIT == cli_run_command(cli, cmd) && CHANNEL_SERIAL != cli->channel)
			{
				break;
			}
			#endif

        }
        
        
        
        // Update the last_action time now as the last command run could take a
        // long time to return
        if (cli->idle_timeout)
            time(&cli->last_action);
    }

    cli_free_history(cli);
    #if 0/*Libo for static memory usage*/
    free_z(cli->commandname);
    free_z(username);
    free_z(password);
    free_z(cmd);
    #endif

#ifdef HAVE_TELNET_CLI
    if(CHANNEL_TCP == cli->channel)
    {
        /*fclose(cli->client);*/
        cli->client = 0;
        cur_chan = CHANNEL_SERIAL;
        telnet_diag_print_unregister();
        telnet_cli = NULL; 
    }
#endif

#ifdef HAVE_GWD_OAM_PTY
	if(CHANNEL_PTY == cli->channel)
	{
		//�ر�oam pty ģʽ
		oam_pty_shell_close();
		
		//����shell ģʽ
		cli->channel = CHANNEL_SERIAL;
		cur_chan = CHANNEL_SERIAL;
	}
#endif

    return CLI_OK;
}

int cli_file(struct cli_def *cli, FILE *fh, int privilege, int mode)
{
    int oldpriv = cli_set_privilege(cli, privilege);
    int oldmode = cli_set_configmode(cli, mode, NULL);
    char buf[MAX_LINE_LENTH];

    while (1)
    {
        char *p;
        char *cmd;
        char *end;

        if (fgets(buf, sizeof(buf), fh) == NULL)
            break; /* end of file */

        if ((p = strpbrk(buf, "#\r\n")))
            *p = 0;

        cmd = buf;
        while (isspace(*cmd))
            cmd++;

        if (!*cmd)
            continue;

        for (p = end = cmd; *p; p++)
            if (!isspace(*p))
                end = p;

        *++end = 0;
        if (strcasecmp(cmd, "quit") == 0)
            break;

        if (cli_run_command(cli, cmd) == CLI_QUIT)
            break;
    }

    cli_set_privilege(cli, oldpriv);
    cli_set_configmode(cli, oldmode, NULL /* didn't save desc */);

    return CLI_OK;
}

void _print(struct cli_def *cli, int print_mode, const char *format, va_list ap)
{
    static char *buffer;
    static int size, len;
    char *p;
    int n;

    if (!cli) return; // sanity check
    if (!cli->buffer) return;

    buffer = cli->buffer;
    size = cli->buf_size;
    len = strlen(buffer);

    // print buffer will not grow
    n = vsnprintf(buffer+len, size-len-1, format, ap);
    buffer[size-1] = 0;
    if (n < 0) // vsnprintf failed
        return;

    p = buffer;
    do
    {
        char *next = strchr(p, '\n');
        struct cli_filter *f = (print_mode & PRINT_FILTERED) ? cli->filters : 0;
        int print = 1;

        if (next)
            *next++ = 0;
        else if (print_mode & PRINT_BUFFERED)
            break;

        while (print && f)
        {
            print = (f->filter(cli, p, f->data) == CLI_OK);
            f = f->next;
        }
        if (print)
        {
            if (cli->print_callback)
                cli->print_callback(cli, p);
            else
            {

                if(CHANNEL_SERIAL == cli->channel)
                {
                    mon_write_chars(p, strlen(p));
                    mon_write_chars("\r\n", 2);
                }
#ifdef HAVE_TELNET_CLI
                else if(CHANNEL_TCP == cli->channel)
                {
                    cs_uint32 i;
                    for(i = 0; i < strlen(p); i++) {
                        console_put_char(p[i]);                        
                    }
                    console_put_char('\r');
                    console_put_char('\n');
                }
#endif
#ifdef	HAVE_GWD_OAM_PTY
				else if(CHANNEL_PTY == cli->channel)
				{
					write_to_oam_pty(p, strlen(p));
					write_to_oam_pty("\r\n", 2);
				}
#endif
                else
                    fprintf(stderr, "Invalid channel %d", cli->channel);
            }
        }

        p = next;
    } while (p);

    if (p && *p)
    {
        if (p != buffer)
            memmove(buffer, p, strlen(p));
    }
    else *buffer = 0;
}

void cli_bufprint(struct cli_def *cli, char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    _print(cli, PRINT_BUFFERED|PRINT_FILTERED, format, ap);
    va_end(ap);
}

void cli_vabufprint(struct cli_def *cli, char *format, va_list ap)
{
    _print(cli, PRINT_BUFFERED, format, ap);
}

void cli_print(struct cli_def *cli, char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    _print(cli, PRINT_FILTERED, format, ap);
    va_end(ap);
}

void cli_error(struct cli_def *cli, char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    _print(cli, PRINT_PLAIN, format, ap);
    va_end(ap);
}

struct cli_match_filter_state
{
    int flags;
#define MATCH_REGEX                1
#define MATCH_INVERT                2
    union {
        char *string;
        regex_t re;
    } match;
};

int cli_match_filter_init(struct cli_def *cli, int argc, char **argv, struct cli_filter *filt)
{
    struct cli_match_filter_state *state;
    int rflags;
    int i;
    char *p;

    if (argc < 2)
    {
#ifdef HAVE_TELNET_CLI
        if (cli->client)
            fprintf(cli->client, "Match filter requires an argument\r\n");
#endif
        return CLI_ERROR;
    }

    filt->filter = cli_match_filter;
    filt->data = state = malloc(sizeof(struct cli_match_filter_state));
    if(!state) return CLI_ERROR;

	#if 0
    if (argv[0][0] == 'i' || // include/exclude
        (argv[0][0] == 'e' && argv[0][1] == 'x'))
    {
        if (argv[0][0] == 'e')
            state->flags = MATCH_INVERT;

        state->match.string = join_words(argc-1, argv+1);
        return CLI_OK;
    }
	#else
	// modified by wujm,support grep egrep
	// grep egrep
	if(strncmp(argv[0], "grep", 4) == 0 || strncmp(argv[0], "egrep", 5) == 0)
	{
		if (argv[1][0] == 'i' || // include/exclude
			(argv[1][0] == 'e' && argv[1][1] == 'x'))
		{

			if (argv[1][0] == 'e')
				state->flags = MATCH_INVERT;
			
			state->match.string = join_words(argc-2, argv+2);
			return CLI_OK;
		}
	}
	// exclude
	else if(strncmp(argv[0], "exclude", 7) == 0)
	{
		state->flags = MATCH_INVERT;
		state->match.string = join_words(argc-1, argv+1);
		return CLI_OK;
	}
	// include
	else
	{
		state->match.string = join_words(argc-1, argv+1);
		return CLI_OK;
	}
	#endif

    /*
     * No regex functions in ecos, so return an error
     */
    return CLI_ERROR;

    state->flags = MATCH_REGEX;

    // grep/egrep
    rflags = REG_NOSUB;
    if (argv[0][0] == 'e') // egrep
        rflags |= REG_EXTENDED;

    i = 1;
    while (i < argc - 1 && argv[i][0] == '-' && argv[i][1])
    {
        int last = 0;
        p = &argv[i][1];

        if (strspn(p, "vie") != strlen(p))
            break;

        while (*p)
        {
            switch (*p++)
            {
                case 'v':
                    state->flags |= MATCH_INVERT;
                    break;

                case 'i':
                    rflags |= REG_ICASE;
                    break;

                case 'e':
                    last++;
                    break;
            }
        }

        i++;
        if (last)
            break;
    }

    p = join_words(argc-i, argv+i);
    if ((i = regcomp(&state->match.re, p, rflags)))
    {
#ifdef HAVE_TELNET_CLI
        if (cli->client)
            fprintf(cli->client, "Invalid pattern \"%s\"\r\n", p);
#endif

        free_z(p);
        return CLI_ERROR;
    }

    free_z(p);
    return CLI_OK;
}

int cli_match_filter(struct cli_def *cli, char *string, void *data)
{
    struct cli_match_filter_state *state = data;
    int r = CLI_ERROR;

    if(!state) return r;
    if (!string) // clean up
    {
        if (state->flags & MATCH_REGEX)
            regfree(&state->match.re);
        else
            free_z(state->match.string);

        free_z(state);
        return CLI_OK;
    }

    if (state->flags & MATCH_REGEX)
    {
        if (!regexec(&state->match.re, string, 0, NULL, 0))
            r = CLI_OK;
    }
    else
    {
        if (strstr(string, state->match.string))
            r = CLI_OK;
    }

    if (state->flags & MATCH_INVERT)
    {
        if (r == CLI_OK)
            r = CLI_ERROR;
        else
            r = CLI_OK;
    }

    return r;
}

struct cli_range_filter_state {
    int matched;
    char *from;
    char *to;
};

int cli_range_filter_init(struct cli_def *cli, int argc, char **argv, struct cli_filter *filt)
{
    struct cli_range_filter_state *state;
    char *from = 0;
    char *to = 0;

    if (!strncmp(argv[0], "bet", 3)) // between
    {
        if (argc < 3)
        {
#ifdef HAVE_TELNET_CLI
            if (cli->client)
                fprintf(cli->client, "Between filter requires 2 arguments\r\n");
#endif
            return CLI_ERROR;
        }
		#if 1/*Libo for static memory usage*/
		// if not malloc "from",in cli_range_filter() shouldn't free "state->from"
		if (!(from = strdup(argv[1])))
            return CLI_ERROR;
        #else
		from = argv[1];    
        #endif
        to = join_words(argc-2, argv+2);
    }
    else // begin
    {
        if (argc < 2)
        {
#ifdef HAVE_TELNET_CLI
            if (cli->client)
                fprintf(cli->client, "Begin filter requires an argument\r\n");
#endif
            return CLI_ERROR;
        }

        from = join_words(argc-1, argv+1);
    }

    filt->filter = cli_range_filter;
    filt->data = state = malloc(sizeof(struct cli_range_filter_state));
	memset(state,0,sizeof(struct cli_range_filter_state));
    if(!state) return CLI_ERROR;

    state->from = from;
    state->to = to;

    return CLI_OK;
}

int cli_range_filter(struct cli_def *cli, char *string, void *data)
{
    struct cli_range_filter_state *state = data;
    int r = CLI_ERROR;

    if(!state) return r;
    if (!string) // clean up
    {
        free_z(state->from);
        free_z(state->to);
        free_z(state);
        return CLI_OK;
    }

    if (!state->matched)
    state->matched = !!strstr(string, state->from);

    if (state->matched)
    {
        r = CLI_OK;
        if (state->to && strstr(string, state->to))
            state->matched = 0;
    }

    return r;
}

int cli_count_filter_init(struct cli_def *cli, int argc, char **argv, struct cli_filter *filt)
{
    if (argc > 1)
    {
#ifdef HAVE_TELNET_CLI
        if (cli->client)
            fprintf(cli->client, "Count filter does not take arguments\r\n");
#endif
        return CLI_ERROR;
    }

    filt->filter = cli_count_filter;
    if (!(filt->data = malloc(sizeof(int))))
    	return CLI_ERROR;

	memset(filt->data,0,sizeof(sizeof(int)));

    return CLI_OK;
}

int cli_count_filter(struct cli_def *cli, char *string, void *data)
{
    int *count = data;

    if (!string) // clean up
    {
        // print count
        cs_printf("count :%d\n",*count);
#ifdef HAVE_TELNET_CLI
        if (cli->client)
            fprintf(cli->client, "%d\r\n", *count);
#endif
        free_z(count);
        return CLI_OK;
    }

    while (isspace(*string))
        string++;

    if (*string)
        (*count)++;  // only count non-blank lines

    return CLI_ERROR; // no output
}

void cli_print_callback(struct cli_def *cli, void (*callback)(struct cli_def *, char *))
{
    cli->print_callback = callback;
}

void cli_set_idle_timeout(struct cli_def *cli, unsigned int seconds)
{
    if (seconds < 1) seconds = 0;
    cli->idle_timeout = seconds;
    time(&cli->last_action);
}

static void parse_cmd(char **line, int *argc, char **argv)
{
        char *cp = *line;
        char *pp;
        int indx = 0;

        while (*cp) {
                // Skip leading spaces
                while (*cp && *cp == ' ') cp++;
                if (!*cp) {
                        break;  // Line ended with a string of spaces
                }
                if (*cp == ';') {
                        *cp = '\0';
                        break;
                }
                if (indx < 10) {
                        argv[indx++] = cp;
                } else {
                        perror("Err!\n");
                }
                while (*cp) {
                        if (*cp == ' ') {
                                *cp++ = '\0';
                                break;
                        } else if (*cp == ';') {
                                break;
                        } else if (*cp == '"') {
                                // Swallow quote, scan till following one
                                if (argv[indx-1] == cp) {
                                        argv[indx-1] = ++cp;
                                }
                                pp = cp;
                                while (*cp && *cp != '"') {
                                        if (*cp == '\\') {
                                                // Skip over escape - allows for escaped '"'
                                                cp++;
                                        }
                                        // Move string to swallow escapes
                                        *pp++ = *cp++;
                                }
                                if (!*cp) {
                                       perror("Err!\n");
                                } else {
                                        if (pp != cp) *pp = '\0';
                                        *cp++ = '\0';
                                        break;
                                }
                        } else {
                                cp++;
                        }
                }
        }
        
        *argc = indx;

        //return cmd_search(__RedBoot_CMD_TAB__, &__RedBoot_CMD_TAB_END__, argv[0]);
}

#if 0
static void _telnet_write_char(char c, void **param)
{
    /* Translate LF into CRLF */
    
    if( c == '\n' )
    {
        telnet_cli_raw_output(telnet_cli,"\r",1);
    }
    telnet_cli_raw_output(telnet_cli,&c,1);
}
#endif

static void _telnet_write_char(char c, void **param)
{
    /* Translate LF into CRLF */

    if(c == '\n')
    {
        console_put_char('\r');
    }
    console_put_char(c);
}

void telnet_diag_print_register(void)
{
   _putcFunc = _putc;
   _putc = _telnet_write_char;
}

void telnet_diag_print_unregister(void)
{
   if(_putcFunc != NULL)
   _putc  = _putcFunc;
}

int do_telnet_legacy_cmd(struct cli_def *cli,char * p)
{
    unsigned char c;
    int l, oldl = 0, esc = 0;
    int cursor = 0, insertmode = 1;
    char *cmd = NULL, *oldcmd = NULL;
    int n;
    int is_telnet_option = 0;
    char *cli_argv[10];
    int cli_argc;
    int sr;
    fd_set r;

   if ((cmd = malloc(MAX_LINE_LENTH)) == NULL)
        return CLI_ERROR;
    
    while (1)
    {
        signed int in_history = 0;
        int lastchar = 0;

        cli->showprompt = 1;

        if (oldcmd)
        {
            l = cursor = oldl;
            oldcmd[l] = 0;
            cli->showprompt = 1;
            oldcmd = NULL;
            oldl = 0;
        }
        else
        {
            memset(cmd, 0, MAX_LINE_LENTH);
            l = 0;
            cursor = 0;
        }

        struct timeval tm;
        memcpy(&tm, &cli->timeout_tm, sizeof(tm));

        while (1)
        {         
        	#if 0
            if (app_ip_changed==1)
            {
                app_ip_changed=0;
                cli_error(cli, "IP change");
                strcpy(cmd, "QUIT");
                break;
            }
			#else
			//modified by zhuxh
			if(CHANNEL_TCP == cli->channel)
        	{
        		if (app_ip_changed==1)
	            {
	                app_ip_changed=0;
	                cli_error(cli, "IP change");
	                strcpy(cmd, "QUIT");
	                break;
	            }
        	}
			#endif
			
            if (cli->showprompt)
            {
                if(cli->state==STATE_SAVE)
                {
                    cli_raw_output(cli, "Need Save? Yes/No:", strlen("Need Save? Yes/No:"));
                }
                else
                {
                    cli_raw_output(cli, p, strlen(p));
                }
                cli->showprompt = 0;
            }

			if(CHANNEL_TCP == cli->channel)
			{
                FD_ZERO(&r);
                FD_SET(cli->sockfd, &r);

                if ((sr = select(cli->sockfd + 1, &r, NULL, NULL, &tm)) < 0)
                {
                    /* select error */
                    if (errno == EINTR)
                    {
                        cur_chan = CHANNEL_SERIAL;
                        telnet_diag_print_unregister();
                        telnet_cli = NULL; 
                        continue;
                    }

                    perror("select\n");
                    l = -1;
                    break;
                }

                if (sr == 0)
                {
                    /* timeout every second */
                    if (cli->regular_callback && cli->regular_callback(cli) != CLI_OK)
                    {
                            strncpy(cmd, "QUIT", MAX_LINE_LENTH-1);
                            break;
                    }

                    if (cli->idle_timeout)
                    {
                        if (time(NULL) - cli->last_action >= cli->idle_timeout)
                        {
                                cli_error(cli, "Idle timeout");
                                strncpy(cmd, "QUIT", MAX_LINE_LENTH-1);
                            break;
                        }
                    }

                    memcpy(&tm, &cli->timeout_tm, sizeof(tm));
                    continue;
                }

                if ((n = read(cli->sockfd, &c, 1)) < 0)
                {
                    if (errno == EINTR)
                    {
                        perror("ERR: sockfd broken, switch to Console\n");
                        cur_chan = CHANNEL_SERIAL;
                        free_z(cmd);
                    return CLI_QUIT;
                    }

                    perror("read\n");
                    l = -1;
                    free_z(cmd);
                    return CLI_QUIT;
                }


                if (cli->idle_timeout)
                    time(&cli->last_action);

                if (n == 0)
                {
                    l = -1;
                    perror("sock read 0\n");
                    free_z(cmd);
                    return CLI_QUIT;
                }
        	}
			#ifdef HAVE_GWD_OAM_PTY
			else if(CHANNEL_PTY == cli->channel)
			{
				n = read_from_oam_pty(&c, 1);
				if(0 == n)
				{
					cs_printf("read 0\n");
					return CLI_QUIT;
				}
			}
			#endif
                if (c == 255 && !is_telnet_option)
                {
                    is_telnet_option++;
                    continue;
                }

                if (is_telnet_option)
                {
                    if (c >= 251 && c <= 254)
                    {
                        is_telnet_option = c;
                        continue;
                    }

                    if (c != 255)
                    {
                        is_telnet_option = 0;
                        continue;
                    }

                    is_telnet_option = 0;
                }

            /* handle ANSI arrows */
            if (esc)
            {
                if (esc == '[')
                {
                    /* remap to readline control codes */
                    switch (c)
                    {
                        case 'A': /* Up */
                            c = CTRL('P');
                            break;

                        case 'B': /* Down */
                            c = CTRL('N');
                            break;

                        case 'C': /* Right */
                            c = CTRL('F');
                            break;

                        case 'D': /* Left */
                            c = CTRL('B');
                            break;

                        default:
                            c = 0;
                    }

                    esc = 0;
                }
                else
                {
                    esc = (c == '[') ? c : 0;
                    continue;
                }
            }

            if (c == 0) continue;
			#if 0
            if (c == '\n') continue;
			#else
			{
				//modified by zhuxh,for oam pty input Enter, no act
				static unsigned char ch_front = 0;
				static unsigned char ch_rear = 0;
				ch_front = ch_rear;
				ch_rear = c;

				if(('\r' == ch_front)&&('\n' == c))
				{
					continue;
				}
				else
				{
					//do nothing
				}
				if('\n' == c)
				{
					c = '\r';
				}
			}
			#endif
			
            if (c == '\r')
            {
                cli_raw_output(cli, "\r\n", 2);
                break;
            }

            if (c == 27)
            {
                esc = 1;
                continue;
            }
            
            if (c == CTRL('C'))
            {
                cli_raw_output(cli, "\a", 1);
                continue;
            }

            /* back word, backspace/delete */
            if (c == CTRL('W') || c == CTRL('H') || c == 0x7f)
            {
                int back = 0;

                if (c == CTRL('W')) /* word */
                {
                    int nc = cursor;

                    if (l == 0 || cursor == 0)
                        continue;

                    while (nc && cmd[nc - 1] == ' ')
                    {
                        nc--;
                        back++;
                    }

                    while (nc && cmd[nc - 1] != ' ')
                    {
                        nc--;
                        back++;
                    }
                }
                else /* char */
                {
                    if (l == 0 || cursor == 0)
                    {
                        cli_raw_output(cli, "\a", 1);
                        continue;
                    }

                    back = 1;
                }

                if (back)
                {
                    while (back--)
                    {
                        if (l == cursor)
                        {
                            cmd[--cursor] = 0;
                            cli_raw_output(cli, "\b \b", 3);
                        }
                        else
                        {
                            int i;
                            cursor--;

                                for (i = cursor; i <= l; i++) cmd[i] = cmd[i+1];
                                cli_raw_output(cli, "\b", 1);
                                cli_raw_output(cli, cmd + cursor, strlen(cmd + cursor));
                                cli_raw_output(cli, " ", 1);
                                for (i = 0; i <= (int)strlen(cmd + cursor); i++)
                                    cli_raw_output(cli, "\b", 1);
                        }
                        l--;
                    }

                    continue;
                }
            }

            /* redraw */
            if (c == CTRL('L'))
            {
                int i;
                int cursorback = l - cursor;

                cli_raw_output(cli, "\r\n", 2);
              //  show_prompt(cli);
                cli_raw_output(cli, cmd, l);

                for (i = 0; i < cursorback; i++)
                    cli_raw_output(cli, "\b", 1);

                continue;
            }

            /* clear line */
            if (c == CTRL('U'))
            {
                cli_clear_line(cli, cmd, l, cursor);
                l = cursor = 0;
                continue;
            }

            /* kill to EOL */
            if (c == CTRL('K'))
            {
                if (cursor == l)
                    continue;

                    int c;
                    for (c = cursor; c < l; c++)
                        cli_raw_output(cli, " ", 1);

                    for (c = cursor; c < l; c++)
                        cli_raw_output(cli, "\b", 1);

                memset(cmd + cursor, 0, l - cursor);
                l = cursor;
                continue;
            }

            /* EOT */
            if (c == CTRL('D'))
            {
                if (l)
                    continue;

                strcpy(cmd, "QUIT");
                l = cursor = strlen(cmd);
                cli_raw_output(cli, "QUIT\r\n", l + 2);
                break;
            }

            /* disable */
            if (c == CTRL('Z'))
            {
                if (cli->mode != MODE_EXEC)
                {
                    cli_clear_line(cli, cmd, l, cursor);
                    cli_set_configmode(cli, MODE_EXEC, NULL);
                    cli->showprompt = 1;
                }

                continue;
            }

            /* TAB completion */
            if (c == CTRL('I'))
            {
                char *completions[MAX_WORDS_NUM];
		char  commonstr[MAX_WORDS_LEN];
                int num_completions = 0;

                if (cursor != l) continue;

		/*modified by liyang @2016-01-29 for get min common string*/
		num_completions = cli_get_completions(cli, cmd, commonstr, completions, MAX_WORDS_NUM);
               if (num_completions == 0)
                {
                    cli_raw_output(cli, "\a", 1);
                }
                else if (num_completions == 1)
                {
                    // Single completion
                    for (; l > 0; l--, cursor--)
                    {
                        if (cmd[l-1] == ' ' || cmd[l-1] == '|')
                            break;
                        cli_raw_output(cli, "\b", 1);
                    }
                    strcpy((cmd + l), completions[0]);
                    l += strlen(completions[0]);
                    cmd[l++] = ' ';
                    cursor = l;
                    cli_raw_output(cli, completions[0], strlen(completions[0]));
                    cli_raw_output(cli, " ", 1);
                }
                else if (lastchar == CTRL('I'))
                {
                    // double tab
			/* modified by liyang@2016-01-29 for output complete token and change raw display to  column display*/
#if 0  
                    int i;
                    cli_raw_output(cli, "\r\n", 2);
                    for (i = 0; i < num_completions; i++)
                    {
                        cli_raw_output(cli, completions[i], strlen(completions[i]));
                        if (i % 4 == 3)
                            cli_raw_output(cli, "\r\n", 2);
                        else
                            cli_raw_output(cli, "     ", 1);
                    }
                    if (i % 4 != 3) cli_raw_output(cli, "\r\n", 2);
                        cli->showprompt = 1;
#else	
			int i;

			/*added by liyang for output complete token after backspace */
			for (; l > 0; l--, cursor--)
			{
				if (cmd[l-1] == ' ' || cmd[l-1] == '|')
					break;
				cli_raw_output(cli, "\b", 1);
			}
			strcpy((cmd + l), commonstr);
			l += strlen(commonstr);
	
			cursor = l;
			cli_raw_output(cli, commonstr, strlen(commonstr));

			/* change raw display to  column display */
			cli_raw_output(cli, "\r\n", 2);
			for (i = 0; i < num_completions; i++)
			{
				cli_raw_output(cli,"  ",2);
				cli_raw_output(cli, completions[i], strlen(completions[i]));

				cli_raw_output(cli, "\r\n", 2);
			}
			cli->showprompt = 1;		
#endif
                }
                else
                {
                    // More than one completion
                    lastchar = c;
                    cli_raw_output(cli, "\a", 1);
                }
                continue;
            }

            /* history */
            if (c == CTRL('P') || c == CTRL('N'))
            {
                int history_found = 0;

                if (c == CTRL('P')) // Up
                {
                    in_history--;
                    if (in_history < 0)
                    {
                        for (in_history = MAX_HISTORY-1; in_history >= 0; in_history--)
                        {
                            if (strlen(cli->history[in_history]))
                            {
                                history_found = 1;
                                break;
                            }
                        }
                    }
                    else
                    {
                        if (strlen(cli->history[in_history])) history_found = 1;
                    }
                }
                else // Down
                {
                    in_history++;
                    if (in_history >= MAX_HISTORY || !strlen(cli->history[in_history]))
                    {
                        int i = 0;
                        for (i = 0; i < MAX_HISTORY; i++)
                        {
                            if (strlen(cli->history[i]))
                            {
                                in_history = i;
                                history_found = 1;
                                break;
                            }
                        }
                    }
                    else
                    {
                        if (strlen(cli->history[in_history])) history_found = 1;
                    }
                }
                if (history_found && strlen(cli->history[in_history]))
                {
                    // Show history item
                    cli_clear_line(cli, cmd, l, cursor);
                    memset(cmd, 0, MAX_LINE_LENTH);
                    strncpy(cmd, cli->history[in_history], MAX_LINE_LENTH-1);
                    l = cursor = strlen(cmd);
                    cli_raw_output(cli, cmd, l);
                }

                continue;
            }

            /* left/right cursor motion */
            if (c == CTRL('B') || c == CTRL('F'))
            {
                if (c == CTRL('B')) /* Left */
                {
                    if (cursor)
                    {
                       cli_raw_output(cli, "\b", 1);

                        cursor--;
                    }
                }
                else /* Right */
                {
                    if (cursor < l)
                    {
                       cli_raw_output(cli, &cmd[cursor], 1);

                        cursor++;
                    }
                }

                continue;
            }

            /* start of line */
            if (c == CTRL('A'))
            {
                if (cursor)
                {
                    cli_raw_output(cli, "\r", 1);
                    cursor = 0;
                }

                continue;
            }

            /* end of line */
            if (c == CTRL('E'))
            {
                if (cursor < l)
                {
                    cli_raw_output(cli, &cmd[cursor], l - cursor);
                    cursor = l;
                }

                continue;
            }

            /* normal character typed */
            if (cursor == l)
            {
                 /* append to end of line */
                cmd[cursor] = c;
                if (l < MAX_LINE_LENTH-1)
                {
                    l++;
                    cursor++;
                }
                else
                {
                    cmd[MAX_LINE_LENTH-1] = 0;
                    cli_raw_output(cli, "\a", 1);
                    continue;
                    //cli_raw_output(cli, "\r\n", 2);
                    //break;
                }
            }
            else
            {
                // Middle of text
                if (insertmode)
                {
                    int i;
                    // Move everything one character to the right
                    if (l >= MAX_LINE_LENTH-2) l--;
                    for (i = l; i >= cursor; i--)
                        cmd[i + 1] = cmd[i];
                    // Write what we've just added
                    cmd[cursor] = c;

                    cli_raw_output(cli, &cmd[cursor], l - cursor + 1);
                    for (i = 0; i < (l - cursor + 1); i++)
                        cli_raw_output(cli, "\b", 1);
                    l++;
                }
                else
                {
                    cmd[cursor] = c;
                }
                cursor++;
            }

                if (c == '?' && cursor == l)
                {
                    cli_raw_output(cli, "\r\n", 2);
                    oldcmd = cmd;
                    oldl = cursor = l - 1;
                    break;
                }
                cli_raw_output(cli, &c, 1);

            oldcmd = 0;
            oldl = 0;
            lastchar = c;
        }

        if (l < 0) break;

		#if 0
        if (!strcasecmp(cmd, "quit") && CHANNEL_TCP == cli->channel) break;
		#else
		//modified by zhuxh
		if (!strcasecmp(cmd, "quit") && CHANNEL_SERIAL != cli->channel)
		{
			break;
		}
		#endif
     
        if (l == 0) continue;
        if (cmd[l - 1] != '?' && strcasecmp(cmd, "history") != 0)
        {
                    int his_ret = cli_add_history(cli, cmd);
                    if(his_ret != CLI_OK)
                        cli_print(cli,"add history failed.\n");
        }

        parse_cmd(&cmd, &cli_argc, &cli_argv[0]);
		#if 0
        if (CLI_OK != cmd_exe(cli_argc, cli_argv) && CHANNEL_TCP == cli->channel)
                    break;
		#else
		//modified by zhuxh
		if (CLI_OK != cmd_exe(cli_argc, cli_argv) && CHANNEL_SERIAL != cli->channel)
		{
			break;
		}      
		#endif

         
            // Update the last_action time now as the last command run could take a
            // long time to return
            if (cli->idle_timeout)
                time(&cli->last_action);
        } // end of cmd process

        if(strcmp(cmd, "quit") == 0)
        {
            free_z(cmd);
            return CLI_QUIT;
        }

        free_z(cmd);
        return CLI_OK;
}

#ifdef HAVE_GWD_OAM_PTY
extern int write_to_oam_pty(char *str,int length);

static void _oam_pty_write_char(char c, void **param)
{
	/* Translate LF into CRLF */
	char ch = '\0';
	if(c == '\n')
	{
		ch = '\r';
		write_to_oam_pty(&ch, 1);
	}
	write_to_oam_pty(&c, 1);
}

extern void oam_pty_diag_print_register(void)
{
   _putcFunc = _putc;
   _putc = _oam_pty_write_char;
}

extern void oam_pty_diag_print_unregister(void)
{
   if(_putcFunc != NULL)
   _putc  = _putcFunc;
}
extern int do_oam_pty_legacy_cmd(struct cli_def *cli,char *p)
{
	return do_telnet_legacy_cmd(cli, p);
}
#endif

#endif
