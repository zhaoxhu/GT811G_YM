#include "rtk_api_ext.h"
#include "cs_utils.h"
#include "plat_common.h"

extern int diag_printf(const char *fmt, ...);

void __sw_cmd_rtk_led_mode_set(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_led_mode_t mode;

    mode = (rtk_led_mode_t)iros_strtol(argv[0]);
    
    rt = rtk_led_mode_set(mode);
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_led_mode_get(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_led_mode_t mode;
    
    rt = rtk_led_mode_get(&mode);
    diag_printf("configure return %x\n", rt);  
    diag_printf("mode = %x\n", mode);  
}

void __sw_cmd_rtk_led_group_set(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_led_group_t group;
    rtk_led_congig_t config;

    group = (rtk_led_group_t)iros_strtol(argv[0]);
    config = (rtk_led_congig_t)iros_strtol(argv[1]);
    
    rt = rtk_led_groupConfig_set(group, config);
    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_led_group_get(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_led_group_t group;
    rtk_led_congig_t config;
    
    group = (rtk_led_group_t)iros_strtol(argv[0]);
    
    rt = rtk_led_groupConfig_get(group, &config);
    
    diag_printf("configure return %x\n", rt); 
    diag_printf("group = %x\n", group);  
    diag_printf("mode = %x\n", config);  
}


typedef void (*rtk_led_cmd_func_t)(int, char **);
#define __RTK_LED_PRINT_CMD_HELP(func,cmd,help)  diag_printf("%s %s %-20s --- %s\n","sw",(func),(cmd),(help))

static struct {
    cs_uint8 *name;
    rtk_led_cmd_func_t      exe_func;
    int                     argc;
    cs_uint8                *para;
    cs_uint8                *help;
    }rtk_led_cmds[] = {
    {"mode-set" , __sw_cmd_rtk_led_mode_set, 1, "[mode]","led mode"},
    {"mode-get" , __sw_cmd_rtk_led_mode_get, 0 , "","get led mode"},
    {"group-set" , __sw_cmd_rtk_led_group_set, 2, "[group] [config]","led group config"},
    {"group-get" , __sw_cmd_rtk_led_group_get, 1 , "","get led group config"},
    };
    

void __rtk_led_cmd_help(void)
{
    int i;
    for (i = 0; i < sizeof(rtk_led_cmds) / sizeof(rtk_led_cmds[0]); i++) {  
        __RTK_LED_PRINT_CMD_HELP("led", rtk_led_cmds[i].name, rtk_led_cmds[i].help);    
    }
}

static rtk_led_cmd_func_t  __rtk_led_find_cmd(
    const char *cmd_str,
    int   argc
)
{
    int i = 0;

    if (NULL == cmd_str) {
        return NULL;
    }

    for (i = 0;
        i < (sizeof(rtk_led_cmds)/ sizeof(rtk_led_cmds[0]));
        ++i) {
        if (cmd_cmp(rtk_led_cmds[i].name, cmd_str)){

            if ((argc - 3) == rtk_led_cmds[i].argc) {
                return rtk_led_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", rtk_led_cmds[i].argc);
                diag_printf("parameters:  %s\n", rtk_led_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __rtk_led_cmd_help();

    return (NULL);

}
    
 void rtk_led_cmd(int argc, char **argv)
 {
    rtk_led_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal fdb  ..."*/

        p_cmd_func = __rtk_led_find_cmd(argv[2], argc);

    }
    else {
        __rtk_led_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}






