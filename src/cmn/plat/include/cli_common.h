#ifndef __CLI_COMMON_H__
#define __CLI_COMMON_H__

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include "plat_common.h"

#define CLI_OK              0
#define CLI_ERROR           -1
#define CLI_QUIT            -2
#define CLI_ERROR_ARG       -3

#define MAX_HISTORY             10
#define MAX_WORDS_NUM           64


#define MAX_WORDS_LEN           128
#define MAX_LINE_LENTH         256
#define MAX_FILTER_NUM           8
#define MAX_PRINT_BUF_LEN     1024
#define MAX_CLI_COMMAND       256

#define PRIVILEGE_GUEST     1
#define PRIVILEGE_USER      7
#define PRIVILEGE_ADMIN     15

#define CLI_COMMAND_FREE        0x0
#define CLI_COMMAND_BUSY        0xffffffff
#define MODE_ANY                -1
#define MODE_EXEC               0
#define MODE_CONFIG             1
#define MODE_DIAGNOSE           2
#define MODE_CONFIG_INTF        10
#define MODE_DEBUG              11

#define MODE_CONFIG_VLAN          13
#define MODE_CONFIG_FDB           14

#define MODE_CONFIG_IGMP          15
#define MODE_CONFIG_UNI           16
#define MODE_CONFIG_QOS           17
#define MODE_CONFIG_PON           18
#define MODE_CONFIG_MGMT          19

#define PRINT_PLAIN             0x00
#define PRINT_FILTERED          0x01
#define PRINT_BUFFERED          0x02

#define CHANNEL_SERIAL          1
#define CHANNEL_TCP             2
#ifdef HAVE_GWD_OAM_PTY
#define CHANNEL_PTY             3
#define PTY_BANNER				"\r\nONU OAM PTY"
#endif

#define TELNETD_PORT            23
#define DFT_SESSION_TIMEOUT     300 //in seconds for telnet

#define CLI_BANNER              "iROS ONU CLI"

#ifdef CYG_HAL_ARM_IMST_OLT
#define HOST_NAME               "OLT"
#else
#define HOST_NAME               "ONU"
#endif

enum cli_states {
    STATE_LOGIN,
    STATE_PASSWORD,
    STATE_NORMAL,
    STATE_ENABLE_PASSWORD,
    STATE_ENABLE,
    STATE_CONFIRM_PASSWORD,
    STATE_QUIT,
    STATE_SAVE,
};


// macros for ? helper in cmd handler
#define CLI_HELP_REQUESTED  (argc > 0 && argv[argc-1][strlen(argv[argc-1])-1] == '?')
#define CLI_HELP_NO_ARGS    ((argc > 1 || argv[0][1]) ? CLI_OK : cli_arg_help(cli, 1, NULL))

/* match if b is a substr of a */
#define MATCH(a,b) (!strncmp((a), (b), strlen(b)))

/* free and zero (to avoid double-free) */
#define free_z(p) do { if (p) { free(p); (p) = 0; } } while (0)

#define CTRL(c) (c - '@')

#define CS_CONSOLE_BUF_LEN  8192

typedef struct {
    cs_uint8 buf[CS_CONSOLE_BUF_LEN];

    cs_uint32 lock; /* console lock */    
    cs_uint32 rd_avail;

    cs_uint16 rd_ptr;
    cs_uint16 empty;
    
    cs_uint16 wr_ptr;
    cs_uint16 full;    
} cs_console_t;


extern struct cli_command *gpst_cmd_tree;

struct cli_def {
    int completion_callback;
    struct cli_command *commands;
    int (*auth_callback)(char *, char *, struct cli_def *);
    int (*regular_callback)(struct cli_def *cli);
    int (*enable_callback)(char *);
    char *banner;
    struct unp *users;
    char *enable_password;
    char history[MAX_HISTORY][MAX_LINE_LENTH];
    char showprompt;
    char *promptchar;
    char *hostname;
    char *modestring;
    int privilege;
    int mode;
    int state;
    struct cli_filter *filters;
    void (*print_callback)(struct cli_def *cli, char *string);
#ifdef HAVE_TELNET_CLI
    int sockfd;
    FILE *client;
#endif
    int channel;
    /* internal buffers */
    void *conn;
    void *service;
    char commandname[MAX_LINE_LENTH];  // temporary buffer for cli_command_name() to prevent leak
    char *buffer;
    unsigned buf_size;

#ifdef HAVE_TELNET_CLI
    struct timeval timeout_tm;
#endif
    unsigned int idle_timeout;
    time_t last_action;
};

struct cli_filter {
    int (*filter)(struct cli_def *cli, char *string, void *data);
    void *data;
    struct cli_filter *next;
};

struct cli_command {
    char *command;
    int (*callback)(struct cli_def *, char *, char **, int);
    unsigned int unique_len;
    char *help;
    int privilege;
    int mode;
    int usage;
    struct cli_command *next;
    struct cli_command *children;
    struct cli_command *parent;
};

struct cli_command *cli_tree_init();
struct cli_def *cli_init(struct cli_command *cmd_root);
int cli_done(struct cli_def *cli);
struct cli_command *cli_register_command(struct cli_command **cmd_root, struct cli_command *parent, char *command, int (*callback)(struct cli_def *cli, char *, char **, int), int privilege, int mode, char *help);
int cli_unregister_command(struct cli_command **cmd_root, char *command);
int cli_run_command(struct cli_def *cli, char *command);
int cli_loop(struct cli_def *cli);
int cli_file(struct cli_def *cli, FILE *fh, int privilege, int mode);
void cli_set_auth_callback(struct cli_def *cli, int (*auth_callback)(char *, char *, struct cli_def *));
void cli_set_enable_callback(struct cli_def *cli, int (*enable_callback)(char *));
void cli_allow_user(struct cli_def *cli, char *username, char *password);
void cli_allow_enable(struct cli_def *cli, char *password);
void cli_deny_user(struct cli_def *cli, char *username);
void cli_set_banner(struct cli_def *cli, char *banner);
void cli_set_hostname(struct cli_def *cli, char *hostname);
void cli_set_promptchar(struct cli_def *cli, char *promptchar);
void cli_set_modestring(struct cli_def *cli, char *modestring);
int cli_set_privilege(struct cli_def *cli, int privilege);
int cli_set_configmode(struct cli_def *cli, int mode, char *config_desc);
void cli_reprompt(struct cli_def *cli);
void cli_regular(struct cli_def *cli, int (*callback)(struct cli_def *cli));
void cli_regular_interval(struct cli_def *cli, int seconds);
void cli_print(struct cli_def *cli, char *format, ...) __attribute__((format (printf, 2, 3)));
void cli_bufprint(struct cli_def *cli, char *format, ...) __attribute__((format (printf, 2, 3)));
void cli_vabufprint(struct cli_def *cli, char *format, va_list ap);
void cli_error(struct cli_def *cli, char *format, ...) __attribute__((format (printf, 2, 3)));
void cli_print_callback(struct cli_def *cli, void (*callback)(struct cli_def *, char *));
void cli_free_history(struct cli_def *cli);
void cli_set_idle_timeout(struct cli_def *cli, unsigned int seconds);
int cli_arg_help(struct cli_def *cli, int cr_ok, char *entry, ...);

void console_put_char(char c);

extern struct cli_command g_cli_command[MAX_CLI_COMMAND];


#endif
