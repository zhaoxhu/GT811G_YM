/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#include "iros_config.h"
#include <stdio.h>
#include <stdlib.h>
#include "plat_common.h"
#include "cli.h"
#include "cli_common.h"
#include "mdio.h"
#include "aal.h"
#include "registers.h"
#include "cs_utils.h"
#include "iros_version.h"

extern int tolower(int c);

//char const iros_version[] = "iROS ONU 1.00 " __TIME__ ", " __DATE__ "\n";
char const iros_version[] = "iROS ONU " \
                             IROS_ONU_APP_VER_MAJOR "." \
                             IROS_ONU_APP_VER_MINOR "." \
                             IROS_ONU_APP_VER_REVISION "." \
                             IROS_ONU_APP_VER_BUILD " " __DATE__"\n";


static unsigned int byte_word_operation = 3; /* 1 for byte; 2 for half word; 3 for word */

#define MAX_ARGV   40
char *cli_argv[MAX_ARGV];
int cli_argc;

#if defined(HAVE_FLASH_FS)
bool flash_cli_mode = false;
extern int flash_cli_cmd_init(void);
extern int flash_cli_cmd_exit(void);
extern int flash_cli_cmd_exe(int argc, char **argv);
#endif

char cli_prompt[10];

void parse_prep(char **line, int *argc, char **argv)
{
        char *cp = *line;
        char *pp;
        int indx = 0;
        int semi = 0;

        while (*cp) {
                // Skip leading spaces
                while (*cp && *cp == ' ') cp++;
                if (!*cp) {
                        break;  // Line ended with a string of spaces
                }
                if (*cp == ';') {
                        *cp = '\0';
                        semi=1;
                        break;
                }
                if (indx < MAX_ARGV) {
                        argv[indx++] = cp;
                } else {
                        cli_printf("Err!\n");
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
                                        cli_printf("Err!\n");
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
        if (semi) {
                *line = cp + 1;
        } else {
                *line = cp;
        }
        *argc = indx;

        //return cmd_search(__RedBoot_CMD_TAB__, &__RedBoot_CMD_TAB_END__, argv[0]);
}

void do_help(void) {
    cs_printf("cmds:\n");
    cs_printf("reset                 - reset chip\n");
    cs_printf("h                     - show help\n");
    cs_printf("v                     - show version and iros information\n");
    cs_printf("show                  - show configurations\n");
    cs_printf("clear                 - clear configrations\n");
    cs_printf("log                   - log module commands\n");
    cs_printf("scfg                  - startup config commands\n");
    cs_printf("per                   - platform commands\n");
    cs_printf("pon                   - pon port commands\n");
    cs_printf("vlan                  - vlan commands\n");
    cs_printf("qos                   - qos commands\n");
    cs_printf("app                   - app commands\n");
    cs_printf("fdb                   - forward table commands\n");
    cs_printf("uni                   - uni commands\n");
    cs_printf("exit                  - exit from current mode\n");
#ifdef HAVE_IP_STACK
    cs_printf("ping                  - ping host_ip\n");
    cs_printf("dhcp [start|stop]     - start|stop dhcp client\n");
#endif
#ifdef HAVE_EXCEPTION_HANDLER
    cs_printf("core [show|clear]     - show/clear core dump in flash\n");
#endif
    return;
}

void do_show_version(void) {
        cli_printf("\n");     // this is needed to fix a uart display issue
        cli_printf("%s", iros_version);
        cli_printf("HW: %x\n", g_lynxd_reg_ptr->glb.JTAG_ID.bf.jtag_id);
        return;
}

void data_dump(char *ptext, int len, char *show_addr) {
        int i;
        unsigned char linebuf[64];
        unsigned char *p;
        unsigned char c;

        //cli_printf("Dump %d\n", len);

        memset(linebuf,0,64);

        p = linebuf;
        for (i = 0; i < len; i++) {
                if ((i & 0xf) == 0) {
                        diag_sprintf(p, "%08x ", (cyg_uint32)show_addr + i);
                        p += 9;
                }
                c = *(char *)(ptext + i);
                diag_sprintf(p, "%02x ", c);
                p += 3;
                if ((i &0xf) == 0xf) {
                        cli_printf("%s\n", linebuf);
                        p = linebuf;
                        memset(linebuf, 0, 64);
                }
        }
        if ((i & 0xf) != 0x0) {
            cli_printf("%s\n", linebuf);
        }
        return;
}

void data_dump_w (char *ptext, int len, char *show_addr) {
        int i;
        unsigned char linebuf[64];
        unsigned char *p;
        unsigned int *p_cur;
        volatile unsigned int val;

        //cli_printf("Dump %d\n", len);

        memset(linebuf,0,64);

        p = linebuf;
        p_cur = (unsigned int *)(((unsigned int)ptext) & 0xfffffffc);

        for (i = 0; i < len; i++) {
                if ((i & 0x3) == 0) {
                        diag_sprintf(p, "%08x ", (cyg_uint32)show_addr + i * 4);
                        p += 9;
                }
                val = p_cur[i];
                //hal_delay_us(1);
                diag_sprintf(p, "%08x ", val);
                p += 9;
                if ((i &0x3) == 0x3) {
                        cli_printf("%s\n", linebuf);
                        p = linebuf;
                        memset(linebuf, 0, 64);
                }
        }
        if ((i & 0x3) != 0x0) {
            cli_printf("%s\n", linebuf);
        }
        return;
}

static void data_dump_h (char *ptext, int len, char *show_addr) {
        int i;
        unsigned char linebuf[64];
        unsigned char *p;
        unsigned short int *p_cur;
        volatile unsigned short int val;

        //cli_printf("Dump %d\n", len);

        memset(linebuf,0,64);

        p = linebuf;
        p_cur = (unsigned short int *)(((unsigned int)ptext) & 0xfffffffe);

        for (i = 0; i < len; i++) {
                if ((i & 0x7) == 0) {
                        diag_sprintf(p, "%08x ", (cyg_uint32)show_addr + i * 2);
                        p += 9;
                }
                val = p_cur[i];
                //hal_delay_us(1);
                diag_sprintf(p, "%04x ", val);
                p += 5;
                if ((i &0x7) == 0x7) {
                        cli_printf("%s\n", linebuf);
                        p = linebuf;
                        memset(linebuf, 0, 64);
                }
        }
        if ((i & 0x7) != 0x0) {
                cli_printf("%s\n", linebuf);
        }
        return;
}


void do_dump_memory(char **argv, int argc) {

        if (argc >= 3) {
                cyg_uint32 len;
                if (argc >= 4) {
                        len = iros_strtol(argv[3]);
            if (len > 32)
                len = 32;
                } else {
                        len = 8;
                }
                char *starting = (char *) iros_strtol(argv[2]);

                if (byte_word_operation == 1) {
                        data_dump(starting, len, starting);
                } else if (byte_word_operation == 3) {
                        data_dump_w(starting, len, starting);
                } else if (byte_word_operation == 2) {
                        //diag_printf("starting %x\n", starting);
                        data_dump_h(starting, len, starting);
                }
        }

        return;
}

bool  do_memcmp(const void *m1, const void *m2, unsigned int n)
{
    char *s1 = (char *)m1;
    char *s2 = (char *)m2;

    while (n--) {
        if (*s1 != *s2){
            diag_printf("%p[0x%02x] mismatch %p[0x%02x]\n",s1,*s1,s2,*s2 );
            return FALSE;
            }
        s1++;
        s2++;
    }
    return TRUE;
}

void do_set_memory(char **argv) {

        if ((argv[2] != 0) && (argv[3] != 0)) {
                char *addr = (char *) iros_strtol(argv[2]);
                cyg_uint32 val = iros_strtol(argv[3]);

                if (byte_word_operation == 1) {
                        addr[0] = (val & 0xff);
                } else if (byte_word_operation == 2){
                        *(unsigned short int *)addr = (val & 0xffff);
                } else if (byte_word_operation == 3){
                        *(unsigned int *)addr = val;
                }
        }

        return;
}

static inline void do_exe_at_addr(char **argv) {

        if (argv[1] != 0) {
                char *addr = (char *) iros_strtol(argv[1]);
                cli_printf("Jump to 0x%08x\n", (cyg_uint32)addr);
                ((void (*)(void))addr)();
        }
        return;
}

static inline void do_iros_info_dump(void) {
        do_show_version();
}

extern int aal_debug_cmd_proc(int argc, char **argv);
extern int sal_cmd_proc(int argc, char **argv);
extern int led_cmd_proc(int argc, char **argv);
extern int private_cfg_cmd_proc(int argc, char **argv);
extern int sw_debug_cmd_proc(int argc, char **argv);

int cmd_exe(int argc, char **argv)
{
    IROS_DPRINTF("cmd_exe %d\n", argc);

#if defined(HAVE_FLASH_FS)
    if (strncmp(argv[0], "fcli",4) == 0) {
        strcpy(cli_prompt,"jffs2_fs> ");
        flash_cli_mode = true;
        flash_cli_cmd_init();
        cs_printf("\n");
    }
    
    if (flash_cli_mode) {
        flash_cli_cmd_exe(argc, argv);
        return 0;
    }
#endif

    if(private_cfg_cmd_proc(argc, argv))
    {
        return 0;
    }

    if(strcmp(argv[0], "exit") == 0)
		return 1;
    
    if(plat_cmd_proc(argc, argv))
    {
        return 0;
    }

    if(aal_debug_cmd_proc(argc, argv))
    {
        return 0;
    }

    if(sal_cmd_proc(argc, argv))
    {
        return 0;
    }
#ifdef HAVE_RTK_CMD
        if (sw_debug_cmd_proc(argc, argv))
        {
            return 0;
        }
#endif
    do_help();
    return 0;
}



char cli_prompt_def[] = "iROS> ";
static char line[256];
int shell_thread_count = 0;
#define _GETS_TIMEOUT -1


extern int _rb_gets(char *buf, int buflen, int timeout);

extern cs_int32 is_halt;
int imst_cli_start(char *p)
{
    int res = 0;
    int i;
    char *command;

    if (p == 0)
    {
        strcpy(cli_prompt,cli_prompt_def);
    }
    else
    {
        strcpy(cli_prompt,p);
    }
    
    cli_printf("%s", cli_prompt);

    while (true)
    {
        shell_thread_count++;
        if(is_halt)
        {
            cs_thread_delay(10);
            continue;
        }
        res = _rb_gets(line, sizeof(line), 1/* 1 seconds */);

        if (res == _GETS_TIMEOUT)
        {
            // no action
        }
        else
        {
            command = (char *)&line;
            cli_argc = 0;
            for(i = 0 ; i < MAX_ARGV ; i++)
                cli_argv[i] = NULL;
            if ((*command == '#') || (*command == '='))
            {
                // Special cases
                if (*command == '=')
                {
                    // Print line on console
                    cli_printf("%s\n", &line[2]);
                }
            }
            else
            {
                while (strlen(command) > 0)
                {
                    parse_prep(&command, &cli_argc, &cli_argv[0]);
                    res = cmd_exe(cli_argc, cli_argv);
					#if 1
					//bug solved:when login onu by serial port, input command "exit", can not exit mini cli
					if(0 != res)
					{
						return res;
					}
					#endif
                }
            }
            cli_printf("\n%s", cli_prompt);
        }
    }

    return 0;
}

#if defined(HAVE_STANDARD_CLI)	//modified by zhuxh
extern int cli_loop(struct cli_def *cli);

void cli_start()
{
    struct cli_def *cli = NULL;

    // init command tree
    gpst_cmd_tree = cli_tree_init();
    if(!gpst_cmd_tree)
        return;

    // init console session
    cli = cli_init(gpst_cmd_tree);
    if(NULL == cli)
        return;

    // configure session
    cli->channel = CHANNEL_SERIAL;
    cli_set_banner(cli, CLI_BANNER);
    cli_set_hostname(cli, HOST_NAME);

    cli_loop(cli);

    cli_done(cli);

    return;
}
#endif

#ifdef HAVE_GWD_OAM_PTY
extern void oam_pty_diag_print_register(void);
extern void oam_pty_diag_print_unregister(void);

extern void oam_pty_cli_start(void)
{
	struct cli_def *cli = NULL;

    // init command tree
    gpst_cmd_tree = cli_tree_init();
    if(NULL == gpst_cmd_tree)
    {
        return;
    }

	cli = cli_init(gpst_cmd_tree);
    if(NULL == cli)
	{
		return;
	}

	// configure session
    cli->channel = CHANNEL_PTY;
	cli_set_banner(cli, PTY_BANNER);
    cli_set_hostname(cli, HOST_NAME);
	oam_pty_diag_print_register();
	
	cli_loop(cli);
	
	oam_pty_diag_print_unregister();
    cli_done(cli);
}

#endif


void shell_thread_entry( cyg_addrword_t data )
{
    do_cpuload_test();

    do_show_version();

	#if defined(HAVE_STANDARD_CLI)	//modified by zhuxh
	cli_start();
	#elif defined(HAVE_MINI_CLI)
	imst_cli_start("onu->");
	#endif
}

void shell_init()
{
    cs_uint32 shell_thread_id;
    cs_uint32 ret;

    ret = cs_thread_create(&shell_thread_id,
                            SHELL_THREAD_NAME,
                            (const void *)shell_thread_entry, (void *)0,
                             SHELL_THREAD_STACKSIZE,
                             SHELL_THREAD_PRIORITY,
                             0);
    if (ret != CS_E_OK)
    {
        cs_printf("shell thread failed to create\n");
    }
    else
    {
        cs_printf("shell thread created\n");
    }
}

