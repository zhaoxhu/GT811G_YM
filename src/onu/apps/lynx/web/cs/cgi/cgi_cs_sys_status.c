#include <stdio.h>
#include "cgi_cmn.h" 
#include "mc_api.h"
#include "sdl_api.h"
#include "startup_cfg.h"
#include "app_ip_api.h"
#include "cs_utils.h"
#include "oam_ctc_pdu.h"
#include "app_oam_ctc_adapt.h"
#include "loop_detect.h"
#include "cgi_cs_sys_status.h"
#include "sys_cfg.h"

typedef struct {
        cs_uint8  oper;
        cs_uint32 cir;
        cs_uint32 cbs;
        cs_uint32 ebs;
}cgi_cs_port_us_policy_t;
typedef struct {
        cs_uint8  oper;
        cs_uint32 cir;
        cs_uint32 pir;
}cgi_cs_port_ds_rate_limit_t;

typedef struct{
    cs_uint8 admin_status;
    cs_uint8 link_status;
    cs_uint8 work_mode;
    cs_uint8 auto_neg_mode;
    cs_uint8 pause;
    cs_uint8 loopdected;
    cs_uint8 mc_tag_strip;
    cs_uint16 mc_max_group;
    cgi_cs_port_us_policy_t     policy; 
    cgi_cs_port_ds_rate_limit_t ds_rate;

    cs_uint8  vlan_mode; 
    oam_ctc_onu_vlan_tag_t def_vlan_tag;
    cs_uint16 translate_num;
    oam_ctc_onu_vlan_xlate_entry translate_vlan[CTC_VLAN_MODE_TRANSLATE_MAX];
    cs_uint16 trunk_num ;
    oam_ctc_onu_vlan_tag_t trunk_vlan[CTC_VLAN_MODE_TRUNK_MAX];
}cgi_cs_port_info_t;

cgi_status cgi_cs_sys_info_get(cgi_handler_param_t * p)
{
    cs_int8 buff[CGI_MAX_BUFF_LEN];
    cs_int8 tmpBuff[CGI_MAX_BUFF_LEN];
    cs_uint8 hwver[CGI_MAX_START_CFG_LEN];
    cs_uint32 i ; 
    cs_uint8 * pVer = ONU_SW_VERSION;
    cs_uint32 ip, mask ,gw;
    cs_int8 strIp[CGI_IP_ADDRESS_LEN + 1];
    cs_uint8 fec_mode;
    cs_uint8 fec_ability;
    cs_callback_context_t     context;
    cs_uint16 llid; 
    cs_uint8 reg_status;
    cs_uint8 haveBattery = 1;
    cs_uint64 time_end, lSecs, lMins;
    cs_uint32 nDays, nHours;
    cs_uint8 portNum;
    cs_mac_t mac;
	
#ifdef HAVE_MPORTS
    cs_uint32 len = 0;
#endif
    cs_uint32 nEepromCheckSum = 0x1234;
    cs_uint8 vendorId[CGI_MAX_START_CFG_LEN ] = {0};
    cs_uint8 model_name[CGI_MAX_START_CFG_LEN] = {0};
    

    CGI_DEBUG("enter \n");
    memset(buff, 0, sizeof(buff));
    memset(hwver, 0, sizeof(hwver));
    sprintf(buff, "%s", "{");

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff,"type:\"%s\"", ONU_CLIENT_PRODUCT_MODEL);
    strcat(buff,tmpBuff);

    startup_config_read(CFG_ID_VENDOR_ID, CGI_MAX_START_CFG_LEN, vendorId);
    startup_config_read(CFG_ID_ONU_MOD_ID, CGI_MAX_START_CFG_LEN, model_name);
    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",onu_vendor_id:\"%s\"", vendorId);
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",onu_model:\"%s\"", model_name);
    strcat(buff, tmpBuff);

    sprintf(hwver, "v1.0_01");

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",onu_hw_ver:\"%s\"",hwver);
    strcat(buff, tmpBuff);
    
    i = 0; 
    while(i < strlen(pVer)){
        if(pVer[i] == '_'){
            pVer[i] = '\0';
            break;
        }
        i++;
    }
    
    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",app_ver:\"%s\"",pVer);
    strcat(buff, tmpBuff);
    
    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",onu_fm_ver:\"%d.%d.%d.%d\"",
            atoi(IROS_ONU_APP_VER_MAJOR),
            atoi(IROS_ONU_APP_VER_MINOR),
            atoi(IROS_ONU_APP_VER_REVISION),
            atoi(IROS_ONU_APP_VER_BUILD));
    strcat(buff, tmpBuff);


    memset(tmpBuff, 0, sizeof(tmpBuff));
    epon_request_onu_mpcp_llid_get( context,
            ONU_DEVICEID_FOR_API,
            ONU_LLIDPORT_FOR_API,
            &llid);
    sprintf(tmpBuff, ",llid:\"0x%04x\"", (llid&0xFFFF));
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",chipset_vendor_id:\"%s\"",ONU_CHIPSET_VENDOR_ID);
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",chipset_chip_model:\"%s\"",ONU_CHIPSET_MODE);
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",chipset_rev:\"%s\"",ONU_CHIPSET_VER);
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",chipset_ic_ver:\"%s\"",ONU_CHIPSET_MADE_DATE);
    strcat(buff, tmpBuff);

    startup_config_read(CFG_ID_MAC_ADDRESS,CS_MACADDR_LEN, &mac.addr[0]);
    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",onu_mac:\"%02X:%02X:%02X:%02X:%02X:%02X\"",
                            mac.addr[0], 
                            mac.addr[1], 
                            mac.addr[2], 
                            mac.addr[3], 
                            mac.addr[4], 
                            mac.addr[5]);
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",onu_id:\"%02X%02X%02X%02X%02X%02X\"",
                            mac.addr[0], 
                            mac.addr[1], 
                            mac.addr[2], 
                            mac.addr[3], 
                            mac.addr[4], 
                            mac.addr[5]);
    strcat(buff, tmpBuff);

    epon_request_onu_ip_config_get(&ip,&mask,&gw);
    memset(strIp,0,sizeof(strIp));
    cs_ip2str(strIp,ip);
    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",onu_ip:\"%s\"",strIp);
    strcat(buff, tmpBuff);

    memset(strIp,0,sizeof(strIp));
    cs_ip2str(strIp,mask);
    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",onu_netmask:\"%s\"",strIp);
    strcat(buff, tmpBuff);


    memset(tmpBuff, 0, sizeof(tmpBuff));
    time_end = cs_current_time();
    lSecs = time_end/1000;
    lMins = lSecs/60;
    nHours = lMins/60;
    nDays = nHours/24;
    sprintf(tmpBuff, ",uptime:\"%d,%d,%d,%d\"", nDays, nHours % 24, (int)(lMins % 60), (int)(lSecs % 60));
    strcat(buff, tmpBuff);

    startup_config_read(
		CFG_ID_SWITCH_PORT_NUM, sizeof(portNum), &portNum);
#ifdef HAVE_MPORTS
	cs_uint8 sw_type;  ;	
	startup_config_read(CFG_ID_SWITCH_TYPE, 1, &sw_type);


if(1 == sw_type)  /*F400G with RTL8365  */{
		len = 0;
		memset(tmpBuff, 0, sizeof(tmpBuff));
		sprintf(tmpBuff, ",capabi_service_supported:\"%s\"","GE");
		strcat(buff, tmpBuff);
		
		memset(tmpBuff, 0, sizeof(tmpBuff));
		sprintf(tmpBuff, ",capabi_ge_num:\"4\"");
		strcat(buff, tmpBuff);

		memset(tmpBuff, 0, sizeof(tmpBuff));
		sprintf(tmpBuff, ",capabi_ge_bitmap:\"1,2,3,4\"");
		strcat(buff, tmpBuff);
				
	    memset(tmpBuff, 0, sizeof(tmpBuff));
	    sprintf(tmpBuff, ",capabi_fe_num:\"0\"");
	    strcat(buff, tmpBuff);

		memset(tmpBuff, 0, sizeof(tmpBuff));
		sprintf(tmpBuff, ",capabi_fe_bitmap:\"0\"");
		strcat(buff, tmpBuff);	}
else /*F400M with RTL8305  */{
    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_service_supported:\"%s\"","FE");
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_ge_num:\"0\"");
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_ge_bitmap:\"0\"");
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_fe_num:\"%d\"",portNum);
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    for (i = 0; i < portNum; i++){
        if (i == 0){
            len += sprintf(tmpBuff + len, ",capabi_fe_bitmap:\" %d", i + 1);
        }
        else{
            len += sprintf(tmpBuff +len, " ,%d",i + 1);
        }
    }
    strcat(tmpBuff,"\"");
    strcat(buff, tmpBuff);
}
#else
    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_service_supported:\"%s\"","GE");
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_ge_num:\"%d\"",portNum);
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_ge_bitmap:\"1\"");
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_fe_num:\"0\"");
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_fe_bitmap:\"0\"");
    strcat(buff, tmpBuff);
#endif

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_pots_num:\"0\"");
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_e1_num:\"0\"");
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_us_que_num:\"8\"");
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_max_us_queue:\"8\"");
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_ds_que_num:\"4\"");
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_max_ds_queue:\"4\"");
    strcat(buff, tmpBuff);

    startup_config_read(CFG_ID_BAK_BATTERY_MODE,sizeof(haveBattery), &haveBattery);
    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",capabi_battery_backup:%d",haveBattery);
    strcat(buff, tmpBuff);

    epon_request_onu_fec_ability_get(context,
            ONU_DEVICEID_FOR_API,
            ONU_LLIDPORT_FOR_API, 
            &fec_ability);
    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",afec_ability:%d",fec_ability?2:3);
    strcat(buff, tmpBuff);

    epon_request_onu_fec_get(
            context,
            ONU_DEVICEID_FOR_API,
            ONU_LLIDPORT_FOR_API,
            &fec_mode);
    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",fec_mode:%d",fec_mode?2:3);
    strcat(buff, tmpBuff);

    //TODO:Geesun get the checksum from plat
    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",eeprom_chksum:\"0x%08X\"", nEepromCheckSum);
    strcat(buff, tmpBuff);

    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",flash_chksum:\"0x%08X\"", nEepromCheckSum);
    strcat(buff, tmpBuff);
    
    epon_request_onu_mpcp_reg_status_get( context,
            ONU_DEVICEID_FOR_API,
            ONU_LLIDPORT_FOR_API,
            &reg_status);
    memset(tmpBuff, 0, sizeof(tmpBuff));
    sprintf(tmpBuff, ",reg_st:%d", reg_status);
    strcat(buff, tmpBuff);

    strcat(buff," }");

    CGI_DEBUG("send buf = %s \n",buff);

    cgi_send_str(p, buff,strlen(buff)); 

    return CS_E_OK;
}


cs_status cgi_cs_get_port_info(cs_port_id_t port, cgi_cs_port_info_t * info)
{
    cs_status ret;
    cs_sdl_port_admin_t port_admin = 0;
    cs_sdl_port_link_status_t    link_status = 0;
    cs_sdl_port_autoneg_status_t auto_neg = 0;
    cs_boolean pause;
    cs_sdl_port_speed_cfg_t cfg;
    cs_boolean loopdetect;
    cs_sdl_policy_t       policy;
    cs_sdl_policy_t       rate;
    cs_callback_context_t context;
    cs_uint16 def_tpid,def_tag;
    cs_uint8 vlan_mode = 1 ;

    cs_uint16 translate_num;
    oam_ctc_onu_vlan_xlate_entry translate_vlan[CTC_VLAN_MODE_TRANSLATE_MAX];
    cs_uint16 trunk_num ;
    oam_ctc_onu_vlan_tag_t trunk_vlan[CTC_VLAN_MODE_TRUNK_MAX];
    
    cs_sdl_mc_vlan_act_t          vlan_act = 0 ;
    cs_sdl_vlan_cfg_t             swap_rule;
    cs_uint16                     rule_nums;


    ret = epon_request_onu_port_admin_get(context, 
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, &port_admin);
    if(ret != CS_E_OK){
        CGI_LOG("get port admin failed, ret = %d \n",ret);
        return CS_E_ERROR;
    }
    info->admin_status = port_admin;

    ret = epon_request_onu_port_link_status_get(context, 
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, &link_status);
    if(ret != CS_E_OK){
        CGI_LOG("get port link failed, ret = %d \n",ret);
        return CS_E_ERROR;
    }
    info->link_status = link_status; 

    ret = epon_request_onu_port_status_get(context, 
        ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, &cfg);

    if(ret != CS_E_OK){
        CGI_LOG("get port port speed failed, ret = %d \n",ret);
        return CS_E_ERROR;
    }
    info->work_mode = cfg;

    ret = epon_request_onu_port_auto_neg_get(context, 
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, &auto_neg);
    if(ret != CS_E_OK){
        CGI_LOG("get port auto neg failed, ret = %d \n",ret);
        return CS_E_ERROR;
    }
    info->auto_neg_mode = auto_neg;
    
    ret = epon_request_onu_port_flow_ctrl_get(context, 
        ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, &pause);
    if(ret != CS_E_OK){
        CGI_LOG("get port auto failed, ret = %d \n",ret);
        return CS_E_ERROR;
    }
    info->pause = pause;
#ifdef HAVE_LOOP_DETECT
    ret = loop_detect_oper_get(port,&loopdetect);
#endif
    if(ret != CS_E_OK){
        CGI_LOG("get port loopdetect failed, ret = %d \n",ret);
        return CS_E_ERROR;
    }
    info->loopdected = loopdetect;

    ret = epon_request_onu_port_policy_get(context, 
        ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, &policy);
    if(ret != CS_E_OK){
        CGI_LOG("get port policy failed, ret = %d \n",ret);
        return CS_E_ERROR;
    }
    info->policy.oper = policy.enable; 
    info->policy.cir = policy.rate;
    info->policy.cbs = policy.cbs;
    info->policy.ebs = policy.ebs;

    ret = epon_request_onu_port_ds_rate_limit_get(context, 
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, &rate);
    if(ret != CS_E_OK){
        CGI_LOG("get port rate failed, ret = %d \n",ret);
        return CS_E_ERROR;
    }
    info->ds_rate.oper = rate.enable;
    info->ds_rate.cir = rate.rate;
    info->ds_rate.pir = rate.cbs;

    ret = epon_request_onu_mc_vlan_action_get(
            context,0,0, port, &vlan_act,&swap_rule,&rule_nums);
    if(ret != CS_E_OK){
        CGI_LOG("get port mc tag strip failed, ret = %d \n",ret);
        return CS_E_ERROR;
    }
    
    CGI_DEBUG("tag strip = %d \n",vlan_act);

    info->mc_tag_strip = vlan_act;

    ret =  mc_port_max_grp_get(0, port, &info->mc_max_group);
    if(ret != CS_E_OK){
        CGI_LOG("get port mc max grp failed, ret = %d \n",ret);
        return CS_E_ERROR;
    }

    ret = ctc_oam_onu_vlan_mode_get_adapt(port,&vlan_mode);
    if(ret != CS_E_OK){
        CGI_LOG("get port vlan mode failed, ret = %d \n",ret);
        return CS_E_ERROR;
    }

    CGI_DEBUG("vlan  mode = %d \n",vlan_mode);
    info->vlan_mode = vlan_mode;
    
    ret = ctc_oam_onu_default_vlan_tag_get_adapt(port,
            &def_tpid, &def_tag);
    if(ret != CS_E_OK){
        CGI_LOG("get port default failed vlan, ret = %d \n",ret);
        return CS_E_ERROR;
    }
    
    info->def_vlan_tag.tpid = def_tpid;
    info->def_vlan_tag.tag = def_tag;

    switch(info->vlan_mode){
        case CTC_VLAN_MODE_TRANSPARENT:
        case CTC_VLAN_MODE_TAG:
            break;
        case CTC_VLAN_MODE_TRANSLATE:
            ret = ctc_oam_onu_translate_vlan_get_adapt(port, &translate_num,
                    translate_vlan);
            if(ret != CS_E_OK){
                CGI_LOG("get port translate vlan failed, ret = %d \n",ret);
                return CS_E_ERROR;
            }
            info->translate_num = translate_num;
            memcpy((cs_uint8*) info->translate_vlan,(cs_uint8*)translate_vlan,sizeof(translate_vlan));

            break;
        case CTC_VLAN_MODE_TRUNK:
            ret = ctc_oam_onu_trunk_vlan_get_adapt(port,&trunk_num,
                    trunk_vlan);
            if(ret != CS_E_OK){
                CGI_LOG("get port trunk vlan failed, ret = %d \n",ret);
                return CS_E_ERROR;
            }
            info->trunk_num = trunk_num;
            memcpy((cs_uint8*) info->trunk_vlan,(cs_uint8*)trunk_vlan,sizeof(trunk_vlan));
            break;
        default:
            break;
    }

    return CS_E_OK;
}

cs_status cgi_cs_vlan_json_str_get(
        cs_uint8 * str, 
        cs_uint32 len,
        cgi_cs_port_info_t * info)
{
    cs_uint8 tmp_str[CGI_MAX_BUFF_LEN];
    cs_uint32 i ;

    CGI_ASSERT_RET(str != NULL && info != NULL, CS_E_PARAM);

    memset(str,0x00,len);
    
    sprintf(str,"{mode:%d",info->vlan_mode);
    switch(info->vlan_mode){
        case CTC_VLAN_MODE_TRANSPARENT:
            memset(tmp_str,0x00,CGI_MAX_BUFF_LEN);
            sprintf(tmp_str,"}");
            strcat(str,tmp_str);
            break;
        case CTC_VLAN_MODE_TAG:
            memset(tmp_str,0x00,CGI_MAX_BUFF_LEN);
            sprintf(tmp_str,",Tag:[%d]}",info->def_vlan_tag.tag&0xfff);
            strcat(str,tmp_str);
            break;
        case CTC_VLAN_MODE_TRANSLATE:
            memset(tmp_str,0x00,CGI_MAX_BUFF_LEN);
            sprintf(tmp_str,",Translation:[");
            strcat(str,tmp_str);

            for(i = 0; i < info->translate_num; i++){
                memset(tmp_str,0x00,CGI_MAX_BUFF_LEN);
                if(i != 0){
                    strcat(str,",");
                }
                sprintf(tmp_str,"{uvid:%d,tvid:%d}",
                        info->translate_vlan[i].tag1.tag, 
                        info->translate_vlan[i].tag2.tag);
                strcat(str,tmp_str);
            }
            strcat(str,"]}");
            break;
        case CTC_VLAN_MODE_TRUNK:
            memset(tmp_str,0x00,CGI_MAX_BUFF_LEN);
            sprintf(tmp_str,",trunk:[%d",info->def_vlan_tag.tag&0xfff);
            strcat(str,tmp_str);
            for(i = 0; i < info->trunk_num; i++){
                memset(tmp_str,0x00,CGI_MAX_BUFF_LEN);
                sprintf(tmp_str,",%d",info->trunk_vlan[i].tag);
                strcat(str,tmp_str);
            }
            strcat(str,"]}");
            break;
        default:
             strcat(str,"}");
            break;
    }
    
    return CS_E_OK;
}

cgi_status cgi_cs_ctc_info_get(cgi_handler_param_t * p)
{
    cs_int8 * buff = NULL;
    cs_uint8 vlan_str[CGI_MAX_BUFF_LEN];
    cs_uint8 *tmp_str = NULL;
    cgi_cs_port_info_t  * port_info;
    cs_uint8 i = 0;
    cs_uint8 mc_mode;
    cs_uint32 mc_fast_leave;
    cs_status ret = CS_E_OK;

    CGI_DEBUG("enter \n");

    port_info = iros_malloc(IROS_MID_WEB,sizeof(cgi_cs_port_info_t)*CGI_MAX_UNI_PORT);
    if(port_info == NULL){
        CGI_LOG("alloc memory failed \n");
        return CS_E_ERROR;
    }

    memset(port_info,0x00,sizeof(cgi_cs_port_info_t)*CGI_MAX_UNI_PORT);

    for(i = 0; i < CGI_MAX_UNI_PORT; i++){
        ret = cgi_cs_get_port_info(i+1, &port_info[i]);
        if(ret != CS_E_OK){
            CGI_LOG("get port info failed, ret = %d \n",ret);
            goto ERR_EXIT;
        }
    }

    ret = ctc_oam_onu_fast_leave_state_get_adapt(&mc_fast_leave);
    if(ret != CS_E_OK){
        CGI_LOG("get onu fast leave failed, ret = %d \n",ret);
        goto ERR_EXIT;
    }
    ret = ctc_oam_onu_mc_switch_get_adapt(&mc_mode);
    if(ret != CS_E_OK){
        CGI_LOG("get onu mc switch failed, ret = %d \n",ret);
        goto ERR_EXIT;
    }

    buff = iros_malloc(IROS_MID_WEB,CGI_MAX_BUFF_LEN * 3);
    if(buff == NULL){
        goto ERR_EXIT;
    }

    tmp_str = iros_malloc(IROS_MID_WEB,CGI_MAX_BUFF_LEN);
    if(tmp_str == NULL){
        goto ERR_EXIT1;
    }

    memset(buff,0x00,sizeof(CGI_MAX_BUFF_LEN * 3));
    sprintf(buff,"{portInfo:[");

    for(i = 0; i < CGI_MAX_UNI_PORT; i++){  
        memset(vlan_str,0x00,sizeof(vlan_str));
        cgi_cs_vlan_json_str_get(vlan_str,CGI_MAX_BUFF_LEN,&port_info[i]);
        if(i != 0){
            strcat(buff,",");
        }
        
        memset(tmp_str,0x00,sizeof(CGI_MAX_BUFF_LEN));
        sprintf(tmp_str,"{ link:%d, us_policy_enable:%d, us_cir:%d, us_cbs:%d, us_ebs:%d, \
                rate_limit_enable:%d, ds_cir:%d, ds_pir:%d, link_rate:%d, admin_status:%d,  \
                pause:%d,VlanInfo:%s ,mc_tag_oper:%d, max_igmp_group:%d, loop_detect:%d, \
                auto_neg:%d}",
                port_info[i].link_status,
                port_info[i].policy.oper,
                port_info[i].policy.cir,
                port_info[i].policy.cbs,
                port_info[i].policy.ebs,
                port_info[i].ds_rate.oper,
                port_info[i].ds_rate.cir,
                port_info[i].ds_rate.pir,
                port_info[i].work_mode,
                port_info[i].admin_status,
                port_info[i].pause,
                vlan_str,
                port_info[i].mc_tag_strip,
                port_info[i].mc_max_group,
                port_info[i].loopdected,
                port_info[i].auto_neg_mode);

        strcat(buff, tmp_str);

    }

    strcat(buff, "],");

    memset(tmp_str,0x00,sizeof(CGI_MAX_BUFF_LEN));
    sprintf(tmp_str,"mc_mode:%d,fastLeave:%d}",mc_mode,mc_fast_leave);
    strcat(buff, tmp_str);

    //CGI_DEBUG("send buf = %s \n",buff);
    cgi_send_str(p, buff,strlen(buff)); 

    iros_free(tmp_str);
ERR_EXIT1:
    iros_free(buff);
ERR_EXIT:
    iros_free(port_info);
    return ret;
}
