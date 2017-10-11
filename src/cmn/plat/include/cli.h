/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#ifndef _CLI_H_
#define _CLI_H_

#include <cyg/kernel/kapi.h>
#include <cyg/hal/hal_diag.h>

extern int diag_printf(const char *fmt, ...);

#define cli_printf  diag_printf

extern void cs_thread_show();
extern int gpio_cmd_proc(int argc, char **argv);
extern int plat_cmd_proc(int argc , char **argv);
extern void show_system_status(void);
extern void parse_prep(char **line, int *argc, char **argv);
extern int cmd_exe(int argc, char **argv);
extern void do_help(void);
extern void do_show_version(void);
int imst_cli_start(char *prompt);


extern char *cli_argv[];
extern int cli_argc;
extern int shell_thread_count;


#endif /* _CLI_H_ */
