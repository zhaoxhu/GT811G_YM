#include "rtk_api_ext.h"
#include "cs_utils.h"
#include "plat_common.h"

void __sw_cmd_rtk_int_ctrl_set(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_int_type_t int_type;
    rtk_enable_t enable;

    int_type = (rtk_int_type_t)iros_strtol(argv[0]);
    enable = (rtk_enable_t)iros_strtol(argv[1]);

    rt = rtk_int_control_set(int_type,enable);
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_int_ctrl_get(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_int_type_t int_type;
    rtk_enable_t enable;

    int_type = (rtk_int_type_t)iros_strtol(argv[0]);

    rt = rtk_int_control_get(int_type,&enable);
    if(rt==0){
        diag_printf("interrupt control :%s. \n", enable ? "enable":"disable");
    }else{
        diag_printf("configure return %x\n", rt);
    }
}

void __sw_cmd_rtk_int_status_set(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_int_status_t int_type;

    int_type.value[0] =  iros_strtol(argv[0]);
    int_type.value[1] =  iros_strtol(argv[1]);

    rt = rtk_int_status_set(int_type);
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_int_status_get(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_int_status_t int_type;
    cs_uint32 ii = 0;
    cs_uint16 status = 0;

char *rtk_int_type_string[] = 
{
    "INT_TYPE_LINK_STATUS",
    "INT_TYPE_METER_EXCEED",
    "INT_TYPE_LEARN_LIMIT",
    "INT_TYPE_LINK_SPEED",
    "INT_TYPE_CONGEST",
    "INT_TYPE_GREEN_FEATURE",
    "INT_TYPE_LOOP_DETECT",
    "INT_TYPE_8051",
    "INT_TYPE_CABLE_DIAG",
    "INT_TYPE_ACL",
    "INT_TYPE_UPS",
    "INT_TYPE_SLIENT",
    "INT_TYPE_END"
};

    rt = rtk_int_status_get(&int_type);
    status = (cs_uint16)(int_type.value[0]) + ((cs_uint16)(int_type.value[1]) << 8);
    if(rt==0){
        diag_printf("Top interrupt status :value[0] 0x%08x value[1] 0x%08x. \n", 
            int_type.value[0],int_type.value[1]);
        for (ii = 0; ii < INT_TYPE_END; ii++) {
            if (status & (0x1 << ii)) {
                diag_printf("Interrupt %30s occured\n", rtk_int_type_string[ii]);
            }
        }
    }else{
        diag_printf("configure return %x\n", rt);
    }

}

void __sw_cmd_rtk_int_adv_get(int argc, char **argv)
{
    rtk_int_advType_t adv_type;
    rtk_int_info_t info;

char *rtk_int_advType_string[] = 
{
    "ADV_L2_LEARN_PORT_MASK",
    "ADV_SPEED_CHANGE_PORT_MASK",
    "ADV_SPECIAL_CONGESTION_PORT_MASK",
    "ADV_PORT_LINKDOWN_PORT_MASK",
    "ADV_PORT_LINKUP_PORT_MASK",
    "ADV_METER0_15_MASK",
    "ADV_METER16_31_MASK",
    "ADV_METER32_47_MASK",
    "ADV_METER48_63_MASK",
    "ADV_RLDP_LOOPED",
	"ADV_RLDP_RELEASED",
    "ADV_END",
};
    
    diag_printf("==========interrupt advanced status: ============\n"); 
    for (adv_type = 0; adv_type < ADV_END; adv_type++) {
        rtk_int_advanceInfo_get(adv_type, &info);
        diag_printf("%40s value-- 0x%08x \n", 
            rtk_int_advType_string[adv_type], info);
    }

}

typedef void (*rtk_int_cmd_func_t)(int, char **);
#define __RTK_INT_PRINT_CMD_HELP(func,cmd,help)  diag_printf("%s %s %-20s --- %s\n","sw",(func),(cmd),(help))

static struct {
    cs_uint8 *name;
    rtk_int_cmd_func_t      exe_func;
    int                     argc;
    cs_uint8                *para;
    cs_uint8                *help;
    }rtk_int_cmds[] = {
        {"intctrl-set" , __sw_cmd_rtk_int_ctrl_set, 2, "[type][enable]","enable/disable interrupt"},
        {"intctrl-get" , __sw_cmd_rtk_int_ctrl_get, 1 , "[type]","get interrupt control"},
        {"intstatus-set" , __sw_cmd_rtk_int_status_set, 2, "[type]","clear interrupt"},
        {"intstatus-get" , __sw_cmd_rtk_int_status_get, 0, "<cr>","get interrupt status"},
        {"adv-get" , __sw_cmd_rtk_int_adv_get, 0, "<cr>","get advanced intr status"},
    };
    

void __rtk_int_cmd_help(void)
{
    int i;
    for (i = 0; i < sizeof(rtk_int_cmds) / sizeof(rtk_int_cmds[0]); i++) {  
        __RTK_INT_PRINT_CMD_HELP("int", rtk_int_cmds[i].name, rtk_int_cmds[i].help);    
    }
}

static rtk_int_cmd_func_t  __rtk_int_find_cmd(
    const char *cmd_str,
    int   argc
)
{
    int i = 0;

    if (NULL == cmd_str) {
        return NULL;
    }

    for (i = 0;
        i < (sizeof(rtk_int_cmds)/ sizeof(rtk_int_cmds[0]));
        ++i) {
        if (cmd_cmp(rtk_int_cmds[i].name, cmd_str)){

            if ((argc - 3) == rtk_int_cmds[i].argc) {
                return rtk_int_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", rtk_int_cmds[i].argc);
                diag_printf("parameters:  %s\n", rtk_int_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __rtk_int_cmd_help();

    return (NULL);

}
    
 void rtk_int_cmd(int argc, char **argv)
 {
    rtk_int_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal fdb  ..."*/

        p_cmd_func = __rtk_int_find_cmd(argv[2], argc);

    }
    else {
        __rtk_int_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}


