#include "rtk_api_ext.h"
#include "cs_utils.h"
#include "plat_common.h"

extern int diag_printf(const char *fmt, ...);
void __sw_cmd_rtk_acl_init(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rt = rtk_switch_init();    
    diag_printf("configure return %x\n", rt);  
}

rtk_filter_cfg_t __cmd_filter_cfg;
void __sw_cmd_rtk_field_clear()
{
    memset((cs_uint8 *)(&__cmd_filter_cfg), 0, sizeof(__cmd_filter_cfg));
}

void __sw_cmd_rtk_acl_field_add_da(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    cs_uint8 index;    
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_DMAC;
    filter_field.filter_pattern_union.dmac.dataType = FILTER_FIELD_DATA_MASK;
    for(index = 0; index < 6; index++){
        filter_field.filter_pattern_union.dmac.value.octet[index] = iros_strtol(argv[index]);
        filter_field.filter_pattern_union.dmac.mask.octet[index] = 0xff;
    }
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[6]);
    filter_cfg.activeport.dataType = iros_strtol(argv[7]);
    filter_cfg.activeport.value = iros_strtol(argv[8]);
    filter_cfg.activeport.mask = iros_strtol(argv[9]);
    filter_cfg.invert = iros_strtol(argv[10]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[11]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[12]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}
void __sw_cmd_rtk_acl_field_add_sa(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    cs_uint8 index;    
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
   //  rtk_filter_care_tag_index_t care_tag;
   memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
   memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
   memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_SMAC;
    filter_field.filter_pattern_union.smac.dataType = FILTER_FIELD_DATA_MASK;
    for(index = 0; index < 6; index++){
        filter_field.filter_pattern_union.smac.value.octet[index] = iros_strtol(argv[index]);
        filter_field.filter_pattern_union.smac.mask.octet[index] = 0xff;
    }
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[6]);
    filter_cfg.activeport.dataType = iros_strtol(argv[7]);
    filter_cfg.activeport.value = iros_strtol(argv[8]);
    filter_cfg.activeport.mask = iros_strtol(argv[9]);
    filter_cfg.invert = iros_strtol(argv[10]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[11]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[12]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}


void __sw_cmd_rtk_acl_field_add_ethtype(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_ETHERTYPE;
    filter_field.filter_pattern_union.etherType.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.etherType.value = iros_strtol(argv[0]);
    filter_field.filter_pattern_union.etherType.mask = 0xffff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[6]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[7]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
            
}

void __sw_cmd_rtk_acl_field_add_vlanid(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_CTAG;
    filter_field.filter_pattern_union.stag.vid.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.stag.vid.value = iros_strtol(argv[0]);
    filter_field.filter_pattern_union.stag.vid.mask = 0xfff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[6]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[7]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
        
}
void __sw_cmd_rtk_acl_field_add_dot1p(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_CTAG;
    filter_field.filter_pattern_union.stag.pri.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.stag.pri.value = iros_strtol(argv[0]);
    filter_field.filter_pattern_union.stag.pri.mask = 7;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[6]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[7]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}

void __sw_cmd_rtk_acl_field_add_sip(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_IPV4_SIP;
    filter_field.filter_pattern_union.sip.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.sip.value = iros_strtol(argv[0]);
    filter_field.filter_pattern_union.sip.mask = 0xffffffff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[6]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[7]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}

void __sw_cmd_rtk_acl_field_add_dip(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_IPV4_DIP;
    filter_field.filter_pattern_union.dip.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.dip.value = iros_strtol(argv[0]);
    filter_field.filter_pattern_union.dip.mask = 0xffffffff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[6]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[7]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}

void __sw_cmd_rtk_acl_field_add_protocol(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_IPV4_PROTOCOL;
    filter_field.filter_pattern_union.protocol.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.protocol.value =  iros_strtol(argv[0]);
    filter_field.filter_pattern_union.protocol.mask = 0xff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[6]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[7]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}

void __sw_cmd_rtk_acl_field_add_tos(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_IPV4_TOS;
    filter_field.filter_pattern_union.ipTos.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.ipTos.value =  iros_strtol(argv[0]);
    filter_field.filter_pattern_union.ipTos.mask = 0xff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[6]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[7]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}

void __sw_cmd_rtk_acl_field_add_tc(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_IPV6_TRAFFIC_CLASS;
    filter_field.filter_pattern_union.ipv6TrafficClass.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.ipv6TrafficClass.value =  iros_strtol(argv[0]);
    filter_field.filter_pattern_union.ipv6TrafficClass.mask = 0xff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[6]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[7]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}

void __sw_cmd_rtk_acl_field_add_l4_tcp_sport(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_TCP_SPORT;
    filter_field.filter_pattern_union.tcpSrcPort.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.tcpSrcPort.value = iros_strtol(argv[0]);
    filter_field.filter_pattern_union.tcpSrcPort.mask = 0xffff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[6]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[7]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}
void __sw_cmd_rtk_acl_field_add_l4_tcp_dport(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_TCP_DPORT;
    filter_field.filter_pattern_union.tcpDstPort.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.tcpDstPort.value = iros_strtol(argv[0]);
    filter_field.filter_pattern_union.tcpDstPort.mask = 0xffff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[6]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[7]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}
void __sw_cmd_rtk_acl_field_add_l4_udp_sport(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_UDP_SPORT;
    filter_field.filter_pattern_union.udpSrcPort.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.udpSrcPort.value = iros_strtol(argv[0]);
    filter_field.filter_pattern_union.udpSrcPort.mask = 0xffff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[6]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[7]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}
void __sw_cmd_rtk_acl_field_add_l4_udp_dport(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_UDP_DPORT;
    filter_field.filter_pattern_union.udpDstPort.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.udpDstPort.value = iros_strtol(argv[0]);
    filter_field.filter_pattern_union.udpDstPort.mask = 0xffff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[6]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[7]);
   
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}
void __sw_cmd_rtk_acl_field_add_ipv6_flowLabel(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_IPV6_FLOW_LABEL;
    filter_field.filter_pattern_union.flowLabel.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.flowLabel.value = iros_strtol(argv[0]);
    filter_field.filter_pattern_union.flowLabel.mask = 0xffff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[6]);
   
   filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
   filter_action.filterRedirectPortmask = 0x4f;
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}

void __sw_cmd_rtk_acl_field_add_ipv6_next_header(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_IPV6_NEXT_HEADER;
    filter_field.filter_pattern_union.ipv6NextHeader.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.ipv6NextHeader.value = iros_strtol(argv[0]);
    filter_field.filter_pattern_union.ipv6NextHeader.mask = 0xff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[6]);
   
   filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
   filter_action.filterRedirectPortmask = 0x4f;
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}

void __sw_cmd_rtk_acl_field_add_ipv6_traffic_class(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_IPV6_TRAFFIC_CLASS;
    filter_field.filter_pattern_union.ipv6TrafficClass.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.ipv6TrafficClass.value = iros_strtol(argv[0]);
    filter_field.filter_pattern_union.ipv6TrafficClass.mask = 0xff;
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[1]);
    filter_cfg.activeport.dataType = iros_strtol(argv[2]);
    filter_cfg.activeport.value = iros_strtol(argv[3]);
    filter_cfg.activeport.mask = iros_strtol(argv[4]);
    filter_cfg.invert = iros_strtol(argv[5]);

    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[6]);
   
   filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
   filter_action.filterRedirectPortmask = 0x4f;
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}


void __sw_cmd_rtk_acl_field_add_sipv6(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    cs_uint8 index;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_IPV6_SIPV6;
    filter_field.filter_pattern_union.sipv6.dataType = FILTER_FIELD_DATA_MASK;
    for(index= 0; index<4; index++){
        filter_field.filter_pattern_union.sipv6.value.addr[index] = iros_strtol(argv[index]);
        filter_field.filter_pattern_union.sipv6.mask.addr[index] = 0xffffffff;
    }
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[4]);
    filter_cfg.activeport.dataType = iros_strtol(argv[5]);
    filter_cfg.activeport.value = iros_strtol(argv[6]);
    filter_cfg.activeport.mask = iros_strtol(argv[7]);
    filter_cfg.invert = iros_strtol(argv[8]);

    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[9]);
   
   filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
   filter_action.filterRedirectPortmask = 0x4f;
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}

void __sw_cmd_rtk_acl_field_add_dipv6(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    cs_uint8 index;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    
    filter_field.fieldType = FILTER_FIELD_IPV6_DIPV6;
    filter_field.filter_pattern_union.dipv6.dataType = FILTER_FIELD_DATA_MASK;
    for(index= 0; index<4; index++){
        filter_field.filter_pattern_union.dipv6.value.addr[index] = iros_strtol(argv[index]);
        filter_field.filter_pattern_union.dipv6.mask.addr[index] = 0xffffffff;
    }
    filter_field.next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);
    if(!rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    filter_id = iros_strtol(argv[4]);
    filter_cfg.activeport.dataType = iros_strtol(argv[5]);
    filter_cfg.activeport.value = iros_strtol(argv[6]);
    filter_cfg.activeport.mask = iros_strtol(argv[7]);
    filter_cfg.invert = iros_strtol(argv[8]);

    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[9]);
   
    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = 0x4f;
    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);
    
    diag_printf("configure return %x\n", rt);         
    
}

void __sw_cmd_rtk_acl_rule_add(int argc, char **argv)
{ 
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field[3];
    cs_uint8 index;    
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    //cs_uint8 offset = 0;
    memset((cs_uint8 *)filter_field, 0 , 3*sizeof(rtk_filter_field_t));
    memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));

    filter_field[0].fieldType = iros_strtol(argv[0]);
    
    switch(filter_field[0].fieldType){
        case FILTER_FIELD_DMAC:
        case FILTER_FIELD_SMAC:
            filter_field[0].filter_pattern_union.mac.dataType = FILTER_FIELD_DATA_MASK;
            for(index = 0; index < 6; index++){
                filter_field[0].filter_pattern_union.mac.value.octet[index] = iros_strtol(argv[index+1]);
                filter_field[0].filter_pattern_union.mac.mask.octet[index] = 0xff;
            }
           // offset += 7;
            break;
           
        case FILTER_FIELD_CTAG:
        case FILTER_FIELD_STAG:
            filter_field[0].filter_pattern_union.l2tag.vid.dataType = FILTER_FIELD_DATA_MASK;
            filter_field[0].filter_pattern_union.l2tag.vid.value = iros_strtol(argv[1]);
            filter_field[0].filter_pattern_union.l2tag.vid.mask = 0xfff;
            filter_field[0].filter_pattern_union.l2tag.pri.value = iros_strtol(argv[2]);
            if(iros_strtol(argv[2]) != 0xff){
                filter_field[0].filter_pattern_union.l2tag.pri.mask = 7;
            }
           break; 
        case FILTER_FIELD_ETHERTYPE:
        case FILTER_FIELD_IPV4_DIP:           
        case FILTER_FIELD_IPV4_SIP: 
        case FILTER_FIELD_TCP_SPORT:
        case FILTER_FIELD_TCP_DPORT:
        case FILTER_FIELD_UDP_SPORT:
        case FILTER_FIELD_UDP_DPORT:
            filter_field[0].filter_pattern_union.inData.dataType = FILTER_FIELD_DATA_MASK;
            filter_field[0].filter_pattern_union.inData.value = iros_strtol(argv[1]);
            filter_field[0].filter_pattern_union.inData.mask = 0xffffffff;
            //offset += 2;
            break;
        default:
        break;
    }
    filter_field[0].next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field[0]);
    if(rt){
        cs_printf("add field error rt  = %d\n", rt);
    }
    
    filter_field[1].fieldType = iros_strtol(argv[7]);
    
    switch(filter_field[1].fieldType){
        case FILTER_FIELD_DMAC:
        case FILTER_FIELD_SMAC:
            filter_field[1].filter_pattern_union.mac.dataType = FILTER_FIELD_DATA_MASK;
            for(index = 0; index < 6; index++){
                filter_field[1].filter_pattern_union.mac.value.octet[index] = iros_strtol(argv[index+8]);
                filter_field[1].filter_pattern_union.mac.mask.octet[index] = 0xff;
            }
           // offset += 7;
            break;
           
           case FILTER_FIELD_CTAG:
           case FILTER_FIELD_STAG:
               filter_field[0].filter_pattern_union.l2tag.vid.dataType = FILTER_FIELD_DATA_MASK;
               filter_field[0].filter_pattern_union.l2tag.vid.value = iros_strtol(argv[8]);
               filter_field[0].filter_pattern_union.l2tag.vid.mask = 0xfff;
               filter_field[0].filter_pattern_union.l2tag.pri.value = iros_strtol(argv[9]);
               if(iros_strtol(argv[9]) != 0xff){
                   filter_field[0].filter_pattern_union.l2tag.pri.mask = 7;
               }
              break; 
        case FILTER_FIELD_ETHERTYPE:
        case FILTER_FIELD_IPV4_DIP:           
        case FILTER_FIELD_IPV4_SIP: 
        case FILTER_FIELD_TCP_SPORT:
        case FILTER_FIELD_TCP_DPORT:
        case FILTER_FIELD_UDP_SPORT:
        case FILTER_FIELD_UDP_DPORT:
            filter_field[1].filter_pattern_union.inData.dataType = FILTER_FIELD_DATA_MASK;
            filter_field[1].filter_pattern_union.inData.value = iros_strtol(argv[8]);
            filter_field[1].filter_pattern_union.inData.mask = 0xffffffff;
            //offset += 2;
            break;
        default:
        break;
    }
    filter_field[1].next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field[1]);
    if(rt){
        cs_printf("add field error rt  = %d\n", rt);
    }

    filter_field[2].fieldType = iros_strtol(argv[14]);
    
    switch(filter_field[2].fieldType){
        case FILTER_FIELD_DMAC:
        case FILTER_FIELD_SMAC:
            filter_field[2].filter_pattern_union.mac.dataType = FILTER_FIELD_DATA_MASK;
            for(index = 0; index < 6; index++){
                filter_field[2].filter_pattern_union.mac.value.octet[index] = iros_strtol(argv[index+15]);
                filter_field[2].filter_pattern_union.mac.mask.octet[index] = 0xff;
            }
           // offset += 7;
            break;
       
       case FILTER_FIELD_CTAG:
       case FILTER_FIELD_STAG:
           filter_field[0].filter_pattern_union.l2tag.vid.dataType = FILTER_FIELD_DATA_MASK;
           filter_field[0].filter_pattern_union.l2tag.vid.value = iros_strtol(argv[15]);
           filter_field[0].filter_pattern_union.l2tag.vid.mask = 0xfff;
           filter_field[0].filter_pattern_union.l2tag.pri.value = iros_strtol(argv[16]);
           if(iros_strtol(argv[16]) != 0xff){
               filter_field[0].filter_pattern_union.l2tag.pri.mask = 7;
           }
          break; 
        case FILTER_FIELD_ETHERTYPE:
        case FILTER_FIELD_IPV4_DIP:           
        case FILTER_FIELD_IPV4_SIP: 
        case FILTER_FIELD_TCP_SPORT:
        case FILTER_FIELD_TCP_DPORT:
        case FILTER_FIELD_UDP_SPORT:
        case FILTER_FIELD_UDP_DPORT:
            filter_field[2].filter_pattern_union.inData.dataType = FILTER_FIELD_DATA_MASK;
            filter_field[2].filter_pattern_union.inData.value = iros_strtol(argv[15]);
            filter_field[2].filter_pattern_union.inData.mask = 0xffffffff;
            //offset += 2;
            break;
        default:
        break;
    }
    filter_field[2].next = NULL;
    rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field[2]);
    if(rt){
        cs_printf("add field error rt  = %d\n", rt);
    }

    filter_id = iros_strtol(argv[21]);
    filter_cfg.activeport.dataType = iros_strtol(argv[22]);
    filter_cfg.activeport.value = iros_strtol(argv[23]);
    filter_cfg.activeport.mask = iros_strtol(argv[24]);
    filter_cfg.invert = iros_strtol(argv[25]);

    filter_action.actEnable[FILTER_ENACT_REDIRECT] = TRUE;   
    filter_action.filterRedirectPortmask = iros_strtol(argv[26]);
    filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
    filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
    filter_action.filterPriority = iros_strtol(argv[27]);

    rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);

    diag_printf("configure return %x\n", rt);         
 
}

void __sw_cmd_rtk_acl_rule_del(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_id_t filter_id;
  
    filter_id = (rtk_filter_id_t)iros_strtol(argv[0]);

   
    rt = rtk_filter_igrAcl_cfg_del(filter_id);
    
    diag_printf(" configure return %x\n",rt);     
}

void __sw_cmd_rtk_acl_rule_clr(int argc, char **argv)
{
    rtk_api_ret_t rt;
   
    rt = rtk_filter_igrAcl_cfg_delAll();
    
    diag_printf(" configure return %x\n",rt);     
}

void __sw_cmd_rtk_acl_rule_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_filter_cfg_raw_t filter_cfg;
    rtk_filter_action_t filter_act;  
    rtk_filter_id_t filter_id;
    rtk_filter_act_enable_t act_no;
    cs_uint8 index;
    rtk_filter_care_tag_index_t care_tag;
    filter_id = (rtk_filter_id_t)iros_strtol(argv[0]);
    memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
    memset((cs_uint8 *)&filter_act, 0 , sizeof(filter_act));

    rt = rtk_filter_igrAcl_cfg_get(filter_id, &filter_cfg, &filter_act); 
    
    diag_printf(" configure return %x\n",rt); 
    for(care_tag = CARE_TAG_CTAG; care_tag<CARE_TAG_END; care_tag++){  
        if(filter_cfg.careTag.tagType[care_tag].mask ){
            diag_printf(" filter_cfg.careTag.tagType[%d].mask %x\n",care_tag,filter_cfg.careTag.tagType[care_tag].mask);
            diag_printf(" filter_cfg.careTag.tagType[%d].value %x\n",care_tag,filter_cfg.careTag.tagType[care_tag].value);
        }
    }  
    diag_printf("filter_cfg.activeport.dataType %x\n",filter_cfg.activeport.dataType);
    diag_printf("filter_cfg.activeport.value %x\n",filter_cfg.activeport.value);
    diag_printf("filter_cfg.activeport.mask %x\n",filter_cfg.activeport.mask);
    diag_printf("filter_cfg.invert %x\n",filter_cfg.invert);
    diag_printf(" filter_cfg.valid %x\n",filter_cfg.valid); 
    for(index = 0; index<8; index++){
        diag_printf(" filter_cfg.dataFieldRaw[%d] %x\n",index,filter_cfg.dataFieldRaw[index]); 
        diag_printf(" filter_cfg.careFieldRaw[%d] %x\n",index,filter_cfg.careFieldRaw[index]); 
        diag_printf(" filter_cfg.fieldRawType[%d] %x\n",index,filter_cfg.fieldRawType[index]); 
    }
   for(act_no = FILTER_ENACT_INGRESS_CVLAN_INDEX; act_no <FILTER_ENACT_END; act_no++){
        if(filter_act.actEnable[act_no]){
            diag_printf("filter_act.actEnable[%d] %s\n",act_no,"TURE");
        }
   }   
   diag_printf("filter_act.filterRedirectPortmask %x, filter_act.filterPriority %x\n",filter_act.filterRedirectPortmask,filter_act.filterPriority);
}

void __sw_cmd_rtk_acl_template_set(int argc, char **argv)
{
    rtk_filter_template_t temp_late;
    cs_uint8 index;    
    rtk_api_ret_t rt;
    memset((cs_uint8 *)&temp_late, 0 , sizeof(temp_late));
    temp_late.index = iros_strtol(argv[0]);
    for(index = 0; index<RTK_FILTER_RAW_FIELD_NUMBER; index++){
        temp_late.fieldType[index] = (rtk_filter_field_type_raw_t)iros_strtol(argv[index+1]);
    }
    rt = rtk_filter_igrAcl_template_set(&temp_late);
    diag_printf(" configure return %x\n",rt);         
}

void __sw_cmd_rtk_acl_template_get(int argc, char **argv)
{
    rtk_filter_template_t temp_late;
    cs_uint8 index;    
    rtk_api_ret_t rt;
    memset((cs_uint8 *)&temp_late, 0 , sizeof(temp_late));
    temp_late.index = iros_strtol(argv[0]);

    rt = rtk_filter_igrAcl_template_get(&temp_late);
    for(index = 0; index<RTK_FILTER_RAW_FIELD_NUMBER; index++){
        diag_printf("field_type[%d] = 0x%x\n", index,temp_late.fieldType[index]);
    }
    diag_printf(" configure return %x\n",rt);         
}
void __sw_cmd_rtk_acl_field_sel_set(int argc, char **argv)
{
    rtk_field_sel_t field_sel;
    cs_uint8 index; 
    cs_uint32 offset;
    rtk_api_ret_t rt;
    index = iros_strtol(argv[0]);
    field_sel = (rtk_field_sel_t)iros_strtol(argv[1]);
    offset = iros_strtol(argv[2]);

    rt = rtk_filter_igrAcl_field_sel_set(index, field_sel, offset);
    diag_printf(" configure return %x\n",rt);         
}

void __sw_cmd_rtk_acl_field_sel_get(int argc, char **argv)
{
 
 rtk_field_sel_t field_sel;
 cs_uint8 index; 
 cs_uint32 offset;
 rtk_api_ret_t rt;
 index = iros_strtol(argv[0]);
 
 rt = rtk_filter_igrAcl_field_sel_get(index, &field_sel, &offset);
 diag_printf(" configure return %x, field_sel = %x, offset = %d\n",rt, field_sel, offset);         
          
}

void __sw_cmd_rtk_acl_unmatch_set(int argc, char **argv)
{
    rtk_port_t port;
    rtk_filter_unmatch_action_t action;
    rtk_api_ret_t rt;
    
    port = iros_strtol(argv[0]);
    action = iros_strtol(argv[1]);

    rt = rtk_filter_igrAcl_unmatchAction_set( port,action);
  
    diag_printf(" configure return %x\n",rt);         
}

void __sw_cmd_rtk_acl_unmatch_get(int argc, char **argv)
{
 
    rtk_port_t port;
    rtk_filter_unmatch_action_t action;
    rtk_api_ret_t rt;
    
    port = iros_strtol(argv[0]);

    rt = rtk_filter_igrAcl_unmatchAction_get( port, &action);
    
    diag_printf(" configure return %x, port = %d, action = %d.\n",rt, port, action);         
          
}

void __sw_cmd_rtk_acl_state_set(int argc, char **argv)
{
    rtk_port_t port;
    rtk_filter_state_t action;
    rtk_api_ret_t rt;
    
    port = iros_strtol(argv[0]);
    action = iros_strtol(argv[1]);

    rt = rtk_filter_igrAcl_state_set( port,action);
  
    diag_printf(" configure return %x\n",rt);         
}

void __sw_cmd_rtk_acl_state_get(int argc, char **argv)
{
 
    rtk_port_t port;
    rtk_filter_state_t action;
    rtk_api_ret_t rt;
    
    port = iros_strtol(argv[0]);

    rt = rtk_filter_igrAcl_state_get( port, &action);
    
    diag_printf(" configure return %x, port = %d, state = %d.\n",rt, port, action);         
          
}

typedef void (*rtk_acl_cmd_func_t)(int, char **);
#define __RTK_ACL_PRINT_CMD_HELP(func,cmd,help)  diag_printf("%s %s %-20s --- %s\n","sw",(func),(cmd),(help))

static struct {
    cs_uint8 *name;
    rtk_acl_cmd_func_t      exe_func;
    int                     argc;
    cs_uint8                *para;
    cs_uint8                *help;
    }rtk_acl_cmds[] = {
    {"fclr" , __sw_cmd_rtk_field_clear, 0, "","clear global parameter"},
    {"da" , __sw_cmd_rtk_acl_field_add_da, 13 , "[da5][da4][da3][da2][da1][da0][aclid][actporttype][port][mask][invert][dpid][priority]","add DA MAC to one rule"},
    {"sa" , __sw_cmd_rtk_acl_field_add_sa, 13 , "[sa5][sa4][sa3][sa2][sa1][sa0][aclid][actporttype][port][mask][invert][dpid][priority]","add SA MAC to one rule"},
    {"vlan" , __sw_cmd_rtk_acl_field_add_vlanid, 8 , "[vlan][aclid][actporttype][port][mask][invert][dpid][priority]","add VLAN id to one rule"},
    {"dot1p" , __sw_cmd_rtk_acl_field_add_dot1p, 8 , "[1p][aclid][actporttype][port][mask][invert][dpid][priority]","add dot1p to one rule"},

    {"sip" , __sw_cmd_rtk_acl_field_add_sip, 8 , "[ip][aclid][actporttype][port][mask][invert][dpid][priority]","add source ip to one rule"},
    {"dip" , __sw_cmd_rtk_acl_field_add_dip, 8 , "[ip][aclid][actporttype][port][mask][invert][dpid][priority]","add destion ip to one rule"},
    {"proto" , __sw_cmd_rtk_acl_field_add_protocol, 8 , "[proto][aclid][actporttype][port][mask][invert][dpid][priority]","add ip protocol to one rule"},
    {"tos" , __sw_cmd_rtk_acl_field_add_tos, 8 , "[tos][aclid][actporttype][port][mask][invert][dpid][priority]","add ip tos to one rule"},
    {"tc" , __sw_cmd_rtk_acl_field_add_tc, 8 , "[tc][aclid][actporttype][port][mask][invert][dpid][priority]","add ipv6 tc to one rule"},
    {"tcp-src" , __sw_cmd_rtk_acl_field_add_l4_tcp_sport, 8 , "[port][aclid][actporttype][port][mask][invert][dpid][priority]","add l4 tcp source port to one rule"},
    {"tcp-dst" , __sw_cmd_rtk_acl_field_add_l4_tcp_dport, 8 , "[port][aclid][actporttype][port][mask][invert][dpid][priority]","add l4 tcp dest port to one rule"},
    {"udp-src" , __sw_cmd_rtk_acl_field_add_l4_udp_sport, 8 , "[port][aclid][actporttype][port][mask][invert][dpid][priority]","add l4 udp source port to one rule"},
    {"udp-dst" , __sw_cmd_rtk_acl_field_add_l4_udp_dport, 8 , "[port][aclid][actporttype][port][mask][invert][dpid][priority]","add l4 udp dest port to one rule"},
    {"ipv6-tc" , __sw_cmd_rtk_acl_field_add_ipv6_traffic_class, 7 , "[tc][aclid][actporttype][port][mask][invert][priority]","add ip v6 traffic class to one rule"},
    {"ipv6-fl" , __sw_cmd_rtk_acl_field_add_ipv6_flowLabel, 7 , "[fl][aclid][actporttype][port][mask][invert][priority]","add ip v6 flow lab to one rule"},
    {"ipv6-nh" , __sw_cmd_rtk_acl_field_add_ipv6_next_header, 7 , "[nh][aclid][actporttype][port][mask][invert][priority]","add ipv6 next header to one rule"},
    {"ipv6-sip" , __sw_cmd_rtk_acl_field_add_sipv6, 10, "[ip3][ip2][ip1][ip0][aclid][actporttype][port][mask][invert][priority]","add ipv6 sip to one rule"},
    {"ipv6-dip" , __sw_cmd_rtk_acl_field_add_dipv6, 10, "[ip3][ip2][ip1][ip0][aclid][actporttype][port][mask][invert][priority]","add ipv6 dip to one rule"},


    {"add" , __sw_cmd_rtk_acl_rule_add, 28 , "[f-slt0][][][][][][][f-slt1][][][][][][][f-slt2][][][][][][][id][actporttype][act port][actmask][invert][redir][pri]",
                                                                    "add one rule"},
    {"del" , __sw_cmd_rtk_acl_rule_del, 1 , "[id]","del one rule"},

    {"clr" , __sw_cmd_rtk_acl_rule_clr, 0 , "","clear all rules"},
    {"get" , __sw_cmd_rtk_acl_rule_get, 1 , "[id]","get rules"},
    {"tem-set" , __sw_cmd_rtk_acl_template_set, 9 , "[id][type0][type1][type2][type3][type4][type5][type6][type7]","set template"},
    {"tem-get" , __sw_cmd_rtk_acl_template_get, 1 , "[id]","get template"},
    {"fid-set" , __sw_cmd_rtk_acl_field_sel_set, 3 , "[id][fid][offset]","set field id"},
   {"fid-get" , __sw_cmd_rtk_acl_field_sel_get, 1 , "[id]","get field id"},
    {"unmatch-set" , __sw_cmd_rtk_acl_unmatch_set, 2, "[port][action:0/1]","set unmatch ACL action:0-drop.1-permit"},
    {"unmatch-get" , __sw_cmd_rtk_acl_unmatch_get, 1 , "[port]","get unmatch ACL action per port"},
    {"state-set" , __sw_cmd_rtk_acl_state_set, 2, "[port][enable:0/1]","enable ACL function:0-disable.1-enable"},
    {"state-get" , __sw_cmd_rtk_acl_state_get, 1 , "[port]","get ACL state per port"},
    }; 
    

void __rtk_acl_cmd_help(void)
{
    int i;
    for (i = 0; i < sizeof(rtk_acl_cmds) / sizeof(rtk_acl_cmds[0]); i++) {  
        __RTK_ACL_PRINT_CMD_HELP("acl", rtk_acl_cmds[i].name, rtk_acl_cmds[i].help);    
    }
}

static rtk_acl_cmd_func_t  __rtk_acl_find_cmd(
    const char *cmd_str,
    int   argc
)
{
    int i = 0;

    if (NULL == cmd_str) {
        return NULL;
    }

    for (i = 0;
        i < (sizeof(rtk_acl_cmds)/ sizeof(rtk_acl_cmds[0]));
        ++i) {
        if (cmd_cmp(rtk_acl_cmds[i].name, cmd_str)){

            if ((argc - 3) == rtk_acl_cmds[i].argc) {
                return rtk_acl_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", rtk_acl_cmds[i].argc);
                diag_printf("parameters:  %s\n", rtk_acl_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __rtk_acl_cmd_help();

    return (NULL);

}
    
 void rtk_acl_cmd(int argc, char **argv)
 {
    rtk_acl_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal fdb  ..."*/

        p_cmd_func = __rtk_acl_find_cmd(argv[2], argc);

    }
    else {
        __rtk_acl_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}


