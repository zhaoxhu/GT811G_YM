#include "rtk_api_ext.h"
#include "cs_utils.h"
#include "plat_common.h"

extern int diag_printf(const char *fmt, ...);
void __sw_cmd_rtk_init(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rt = rtk_switch_init();    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_max_len_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_switch_maxPktLen_t max_length;
    max_length = (rtk_switch_maxPktLen_t)iros_strtol(argv[0]);
    rt = rtk_switch_maxPktLen_set(max_length);
    diag_printf("configure return %x\n", rt);     
}

void __sw_cmd_rtk_max_len_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_switch_maxPktLen_t max_length;
    rt = rtk_switch_maxPktLen_get(&max_length);
    diag_printf("configure return %x\n", rt);
    switch(max_length){
        case MAXPKTLEN_1522B:            
            diag_printf("max length is %s\n", "1522B");
            break;
        case MAXPKTLEN_1536B:            
            diag_printf("max length is %s\n", "1536B");
            break;
        case MAXPKTLEN_1552B:            
            diag_printf("max length is %s\n", "1552B");
            break;
        case MAXPKTLEN_16000B:            
            diag_printf("max length is %s\n", "16000B");
            break;
        default:
            break;
    }   
}

void __sw_cmd_rtk_green_ethernet_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_enable_t en;
    en = (rtk_enable_t)iros_strtol(argv[0]);
    rt = rtk_switch_greenEthernet_set(en);
    diag_printf("configure return %x\n", rt);     
}

void __sw_cmd_rtk_green_ethernet_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_enable_t en;
    rt = rtk_switch_greenEthernet_get(&en);
    diag_printf("configure return %x\n", rt);
    diag_printf("green ethernet is %s\n", en ? "enable":"disable");    
}

typedef void (*rtk_glb_cmd_func_t)(int, char **);
#define __RTK_GLB_PRINT_CMD_HELP(func,cmd,help)  diag_printf("%s %s %-20s --- %s\n","sw",(func),(cmd),(help))

static struct {
    cs_uint8 *name;
    rtk_glb_cmd_func_t      exe_func;
    int                     argc;
    cs_uint8                *para;
    cs_uint8                *help;
    }rtk_glb_cmds[] = {
    {"init" , __sw_cmd_rtk_init, 0, "cr","init rtk"},
    {"maxlen-set" , __sw_cmd_rtk_max_len_set, 1 , "max_len","set maximum packet length"},
    {"maxlen-get" , __sw_cmd_rtk_max_len_get, 0 , "cr","get maximum packet length"},
    {"green-set" , __sw_cmd_rtk_green_ethernet_set, 1 , "enable","green ethernet enable"},
    {"green-get" , __sw_cmd_rtk_green_ethernet_get, 0 , "cr","get green ethernet"},
    };

void __rtk_glb_cmd_help(void)
{
    int i;
    for (i = 0; i < sizeof(rtk_glb_cmds) / sizeof(rtk_glb_cmds[0]); i++) {  
        __RTK_GLB_PRINT_CMD_HELP("glb", rtk_glb_cmds[i].name, rtk_glb_cmds[i].help);    
    }
}

static rtk_glb_cmd_func_t  __rtk_glb_find_cmd(
    const char *cmd_str,
    int   argc
)
{
    int i = 0;

    if (NULL == cmd_str) {
        return NULL;
    }

    for (i = 0;
        i < (sizeof(rtk_glb_cmds)/ sizeof(rtk_glb_cmds[0]));
        ++i) {
        if (cmd_cmp(rtk_glb_cmds[i].name, cmd_str)){

            if ((argc - 3) == rtk_glb_cmds[i].argc) {
                return rtk_glb_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", rtk_glb_cmds[i].argc);
                diag_printf("parameters:  %s\n", rtk_glb_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __rtk_glb_cmd_help();

    return (NULL);

}
    
 void rtk_glb_cmd(int argc, char **argv)
 {
    rtk_glb_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal glb  ..."*/

        p_cmd_func = __rtk_glb_find_cmd(argv[2], argc);

    }
    else {
        __rtk_glb_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}





