/****************************************************************************
            Software License for Customer Use of Cortina Software
                          Grant Terms and Conditions

IMPORTANT NOTICE - READ CAREFULLY: This Software License for Customer Use
of Cortina Software ("LICENSE") is the agreement which governs use of
software of Cortina Systems, Inc. and its subsidiaries ("CORTINA"),
including computer software (source code and object code) and associated
printed materials ("SOFTWARE").  The SOFTWARE is protected by copyright laws
and international copyright treaties, as well as other intellectual property
laws and treaties.  The SOFTWARE is not sold, and instead is only licensed
for use, strictly in accordance with this document.  Any hardware sold by
CORTINA is protected by various patents, and is sold but this LICENSE does
not cover that sale, since it may not necessarily be sold as a package with
the SOFTWARE.  This LICENSE sets forth the terms and conditions of the
SOFTWARE LICENSE only.  By downloading, installing, copying, or otherwise
using the SOFTWARE, you agree to be bound by the terms of this LICENSE.
If you do not agree to the terms of this LICENSE, then do not download the
SOFTWARE.

DEFINITIONS:  "DEVICE" means the Cortina Systems? Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems? SDK software.

GRANT OF LICENSE:  Subject to the restrictions below, CORTINA hereby grants
CUSTOMER a non-exclusive, non-assignable, non-transferable, royalty-free,
perpetual copyright license to (1) install and use the SOFTWARE for
reference only with the DEVICE; and (2) copy the SOFTWARE for your internal
use only for use with the DEVICE.

RESTRICTIONS:  The SOFTWARE must be used solely in conjunction with the
DEVICE and solely with Your own products that incorporate the DEVICE.  You
may not distribute the SOFTWARE to any third party.  You may not modify
the SOFTWARE or make derivatives of the SOFTWARE without assigning any and
all rights in such modifications and derivatives to CORTINA.  You shall not
through incorporation, modification or distribution of the SOFTWARE cause
it to become subject to any open source licenses.  You may not
reverse-assemble, reverse-compile, or otherwise reverse-engineer any
SOFTWARE provided in binary or machine readable form.  You may not
distribute the SOFTWARE to your customers without written permission
from CORTINA.

OWNERSHIP OF SOFTWARE AND COPYRIGHTS. All title and copyrights in and the
SOFTWARE and any accompanying printed materials, and copies of the SOFTWARE,
are owned by CORTINA. The SOFTWARE protected by the copyright laws of the
United States and other countries, and international treaty provisions.
You may not remove any copyright notices from the SOFTWARE.  Except as
otherwise expressly provided, CORTINA grants no express or implied right
under CORTINA patents, copyrights, trademarks, or other intellectual
property rights.

DISCLAIMER OF WARRANTIES. THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY
EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING ANY IMPLIED WARRANTIES
OF MERCHANTABILITY, NONINFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE,
TITLE, AND NON-INFRINGEMENT.  CORTINA does not warrant or assume
responsibility for the accuracy or completeness of any information, text,
graphics, links or other items contained within the SOFTWARE.  Without
limiting the foregoing, you are solely responsible for determining and
verifying that the SOFTWARE that you obtain and install is the appropriate
version for your purpose.

LIMITATION OF LIABILITY. IN NO EVENT SHALL CORTINA OR ITS SUPPLIERS BE
LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, LOST
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING OUT
OF THE USE OF OR INABILITY TO USE OF OR INABILITY TO USE THE SOFTWARE, EVEN
IF CORTINA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
TERMINATION OF THIS LICENSE. This LICENSE will automatically terminate if
You fail to comply with any of the terms and conditions hereof. Upon
termination, You will immediately cease use of the SOFTWARE and destroy all
copies of the SOFTWARE or return all copies of the SOFTWARE in your control
to CORTINA.  IF you commence or participate in any legal proceeding against
CORTINA, then CORTINA may, in its sole discretion, suspend or terminate all
license grants and any other rights provided under this LICENSE during the
pendency of such legal proceedings.
APPLICABLE LAWS. Claims arising under this LICENSE shall be governed by the
laws of the State of California, excluding its principles of conflict of
laws.  The United Nations Convention on Contracts for the International Sale
of Goods is specifically disclaimed.  You shall not export the SOFTWARE
without first obtaining any required export license or other approval from
the applicable governmental entity, if required.  This is the entire
agreement and understanding between You and CORTINA relating to this subject
matter.
GOVERNMENT RESTRICTED RIGHTS. The SOFTWARE is provided with "RESTRICTED
RIGHTS." Use, duplication, or disclosure by the Government is subject to
restrictions as set forth in FAR52.227-14 and DFAR252.227-7013 et seq. or
its successor. Use of the SOFTWARE by the Government constitutes
acknowledgment of CORTINA's proprietary rights therein. Contractor or
Manufacturer is CORTINA.

Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/
#include <stdio.h>
#include "plat_common.h"
#include "db_api.h"
#include "sdl_api.h"
#include "sys_cfg.h"
#include "app_ip_api.h"
#include "sdl_gpio.h"
#include "mc_api.h"
#include "mc_core.h"
#include "mc_type.h"
#include "mc_ctcctrl.h"
#include "app_oam_ctc_adapt.h"
#include "db_instance.h"
#include "loop_detect.h"
#include "app_alarm_ctc.h"
#ifdef HAVE_PPPOE
#include "pppoe.h"
#endif
#ifdef HAVE_ONU_RSTP
#include "stp_api.h"
#endif
#ifdef HAVE_DB_MANAGMENT


db_data_confg_t g_ctcCfgTable[] = 
{
    /* name                        id                               type                maxlen                          */
    {"loid passwd",         DB_LOID_PASSWD_ID,              DB_TYPE_BYTES,        sizeof(db_loid_pwd_t),                    0  },
    {"admin user",          DB_ADMIN_INFO_ID,               DB_TYPE_BYTES,        sizeof(db_user_t)*PER_LEVEL_MAX_ACCOUNT,  0  },
    {"power user",          DB_USER_INFO_ID,                DB_TYPE_BYTES,        sizeof(db_user_t)*PER_LEVEL_MAX_ACCOUNT,  0  },
    {"mgmt info",           DB_MGMT_INFO_ID,                DB_TYPE_BYTES,        sizeof(db_mgmt_t),                        0  },
    {"sn",                  DB_SN_ID,                       DB_TYPE_BYTES,        sizeof(db_sn_t),                          0  },
    {"backup battery",      DB_BACKUP_BATTERY_ID,           DB_TYPE_BYTES,        sizeof(db_backup_battery_t),              0  },
    {"onu service info",    DB_ONU_SVC_INFO_ID,             DB_TYPE_BYTES,        sizeof(db_onu_svc_t),                     0  },
    {"port mirror",         DB_PORT_MIRROR_ID,              DB_TYPE_BYTES,        sizeof(db_mirror_t),                      0  },
    {"qos voq",             DB_QOS_VOQ_ID,                  DB_TYPE_BYTES,        sizeof(db_voq_t),                         0  },
    {"qos remark",          DB_QOS_REMARK_ID,               DB_TYPE_BYTES,        sizeof(db_remark_t),                      0  },
    {"port info",           DB_PORT_INFO_ID,                DB_TYPE_BYTES,        sizeof(db_port_t),                        0  },
    {"mac bind",            DB_MAC_BIND_ID,                 DB_TYPE_BYTES,        sizeof(db_mac_bind_t),                    0  },
    {"vlan",                DB_VLAN_ID,                     DB_TYPE_BYTES,        sizeof(db_vlan_t),                        0  },
    {"profile",             DB_PROFILE_ID,                  DB_TYPE_BYTES,        sizeof(db_profile_struct_t),              0  },
    {"guset user",          DB_GUEST_INFO_ID,               DB_TYPE_BYTES,        sizeof(db_user_t)*PER_LEVEL_MAX_ACCOUNT,  0  },
    {"telnet admin",        DB_TEL_ADMIN_INFO_ID,           DB_TYPE_BYTES,        sizeof(db_user_t),  0  },
    {"telnet user",         DB_TEL_USER_INFO_ID,            DB_TYPE_BYTES,        sizeof(db_user_t),  0  },
    {"telnet guest",        DB_TEL_GUEST_INFO_ID,           DB_TYPE_BYTES,        sizeof(db_user_t),  0  },
    {"mac_limit",           DB_MAC_LIMIT_ID,                DB_TYPE_BYTES,        sizeof(db_mac_limit_t), 0  },
    {"mac_filter",          DB_MAC_FILTER_ID,               DB_TYPE_BYTES,        sizeof(db_mac_bind_t), 0  },
    {"static_mac",          DB_STATIC_MAC_ID,               DB_TYPE_BYTES,        sizeof(db_static_mac_t), 0  },
    {"cls",                 DB_CLS_ID,                      DB_TYPE_BYTES,        sizeof(db_cls_t), 0  },
    {"mc_vlan",             DB_MC_VLAN_ID,                  DB_TYPE_BYTES,        sizeof(db_mc_vlan_t), 0  },
    {"mc_tagstrip",         DB_MC_TAGSTRIP_ID,              DB_TYPE_BYTES,        sizeof(db_mc_tagstrip_t), 0  },
    {"mc_maxgrp",           DB_MC_MAX_GRP_ID,               DB_TYPE_BYTES,        sizeof(db_mc_grp_t), 0  },
    {"fast_leave",          DB_FAST_LEAVE_ID,               DB_TYPE_BYTES,        sizeof(db_fast_leave_t), 0  },
    {"onu_alarm",           DB_ONU_ALARM_ID,                DB_TYPE_BYTES,        sizeof(db_onu_alarm_t), 0  },
    {"port_alarm",          DB_PORT_ALARM_ID,               DB_TYPE_BYTES,        sizeof(db_port_alarm_t), 0  },
    {"timeout",             DB_TIMEOUT_ID,                  DB_TYPE_BYTES,        sizeof(db_timeout_t), 0  },
};

cs_status db_set_default()
{
    cs_status ret = CS_E_OK;
    cs_uint32 i, j;
    db_user_t temp_user[PER_LEVEL_MAX_ACCOUNT];
    db_user_t telnet_user;
    db_mgmt_t  mgmt_info; 
    db_sn_t   sn; 
    db_backup_battery_t battery; 
    db_onu_svc_t   svc;
    db_port_t   port_info;
    db_voq_t voq;
    db_mirror_t mirror;
    db_vlan_t vlan;
    db_remark_t remark;
    db_loid_pwd_t loid;
    cs_mac_t mac;
    cs_uint8 max_port;
    db_mac_limit_t  db_mac_limit;
    db_mc_grp_t         db_mc_grp;
    db_fast_leave_t    db_fast_leave;
    db_timeout_t        db_timeout;
    cs_uint8* dbbuf = NULL; 
    cs_uint32 max_db_num = sizeof(g_ctcCfgTable)/sizeof(db_data_confg_t);
    cs_uint32 max_db_len = 0;

    startup_config_read(
        CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);
    max_port = max_port >DB_MAX_PORT ? DB_MAX_PORT:max_port;

    for(i=0;i<max_db_num;i++)
    {
        if(max_db_len < g_ctcCfgTable[i].max_len)
            max_db_len = g_ctcCfgTable[i].max_len;
    }
    dbbuf = iros_malloc(IROS_MID_APP,max_db_len);
    if(dbbuf == NULL)
    {
        cs_printf("DB Memory fail.\n");
        return CS_E_RESOURCE;
    }
    memset(dbbuf,0x00,max_db_len);
    /* init the admin account*/
    memset(temp_user,0x00,sizeof(temp_user)); 
    temp_user[0].valid=1;
    strcpy(temp_user[0].usr,DB_DEFAULT_ADMIN_NAME);
    strcpy(temp_user[0].pwd,DB_DEFAULT_ADMIN_PWD);
    db_write(DB_ADMIN_INFO_ID,(cs_uint8*)temp_user,sizeof(temp_user));

    /* init the user account*/
    memset(temp_user,0x00,sizeof(temp_user)); 
    temp_user[0].valid=1;
    strcpy(temp_user[0].usr,DB_DEFAULT_USR_NAME);
    strcpy(temp_user[0].pwd,DB_DEFAULT_USR_PWD);
    db_write(DB_USER_INFO_ID,(cs_uint8*)temp_user,sizeof(temp_user));

    /*Init the guest account*/
    memset(temp_user,0x00,sizeof(temp_user)); 
    temp_user[0].valid=1;
    strcpy(temp_user[0].usr,DB_DEFAULT_GUEST_NAME);
    strcpy(temp_user[0].pwd,DB_DEFAULT_GUEST_PWD);
    db_write(DB_GUEST_INFO_ID,(cs_uint8*)temp_user,sizeof(temp_user));
   
    /*Init all telnet account*/
    memset((char *)&telnet_user,0x00,sizeof(telnet_user)); 
    strcpy(telnet_user.usr,DB_DEFAULT_ADMIN_NAME);
    strcpy(telnet_user.pwd,DB_DEFAULT_ADMIN_PWD);
    db_write(DB_TEL_ADMIN_INFO_ID,(cs_uint8*)&telnet_user,sizeof(telnet_user));

    memset((char *)&telnet_user,0x00,sizeof(telnet_user)); 
    strcpy(telnet_user.usr,DB_DEFAULT_USR_NAME);
    strcpy(telnet_user.pwd,DB_DEFAULT_USR_PWD);
    db_write(DB_TEL_USER_INFO_ID,(cs_uint8*)&telnet_user,sizeof(telnet_user));

    memset((char *)&telnet_user,0x00,sizeof(telnet_user)); 
    strcpy(telnet_user.usr,DB_DEFAULT_GUEST_NAME);
    strcpy(telnet_user.pwd,DB_DEFAULT_GUEST_PWD);
    db_write(DB_TEL_GUEST_INFO_ID,(cs_uint8*)&telnet_user,sizeof(telnet_user));
    
    /* init the management information*/
    memset(&mgmt_info,0x00,sizeof(mgmt_info));
    mgmt_info.ip = DB_DEFAULT_IP;
    mgmt_info.mask = DB_DEFAULT_MASK;
    mgmt_info.inband_ip = DB_DEFAULT_IP;
    mgmt_info.inband_mask = DB_DEFAULT_MASK;
    mgmt_info.gw = DB_DEFAULT_GW;
    mgmt_info.mgmt_mtu = DB_DEFAULT_MGMT_MTU;
    mgmt_info.mgmt_net = DB_DEFAULT_MGMT_NET;
    mgmt_info.mgmt_mask = DB_DEFAULT_MGMT_MASK;
    mgmt_info.inband_vlan = DB_DEFAULT_INBAND_VLAN;
    mgmt_info.inband_pri = DB_DEFAULT_INBAND_PRI;
    mgmt_info.inband_enable = DB_DEFAULT_INBAND_MODE;
    db_write(DB_MGMT_INFO_ID,(cs_uint8*) &mgmt_info,sizeof(mgmt_info));

    /*sn no need restore to factory*/
    memset(&sn,0x00,sizeof(sn));
    ret = db_read(DB_SN_ID,(cs_uint8 *) &sn, sizeof(sn));
    startup_config_read(CFG_ID_MAC_ADDRESS,CS_MACADDR_LEN, &mac.addr[0]);
    /*if not exist in db, restore to mac address*/
    if(CS_E_OK != ret){

        memset(&sn,0x00,sizeof(sn));
        sprintf(sn.sn, "%02X%02X%02X%02X%02X%02X", mac.addr[0], 
            mac.addr[1], 
            mac.addr[2], 
            mac.addr[3], 
            mac.addr[4], 
            mac.addr[5]);
        db_write(DB_SN_ID,(cs_uint8 *) &sn, sizeof(sn));
    }
    
    /* Loid and passwd */ 
    memset(&loid,0x00,sizeof(loid));
    ret = db_read(DB_LOID_PASSWD_ID,(cs_uint8 *) &loid, sizeof(loid));
    if(ret != CS_E_OK){
        memset(&loid,0x00,sizeof(loid));
        sprintf (loid.loid, "%02x%02x%02x%02x%02x%02x",mac.addr[0], 
            mac.addr[1], 
            mac.addr[2], 
            mac.addr[3], 
            mac.addr[4], 
            mac.addr[5]);
        sprintf(loid.passwd,"1111");
        db_write(DB_LOID_PASSWD_ID,(cs_uint8 *) &loid, sizeof(loid));
    }

    /* init the backup battery  */
    memset(&battery,0x00,sizeof(battery));
    for(i = 0; i < DB_BATTERY_CFG_NUM; i++){
        battery.battery[i].alarm_sig = i;
        battery.battery[i].log_state = 1;
    }
    db_write(DB_BACKUP_BATTERY_ID,(cs_uint8 *) &battery, sizeof(battery));
    
    db_timeout.web_timeout = DB_TIMEOUT_DEFAULT;
    db_timeout.telnet_timeout = DB_TIMEOUT_DEFAULT;

    db_write(DB_TIMEOUT_ID,(cs_uint8 *) &db_timeout, sizeof(db_timeout));

    /* init the service data*/
    memset(&svc,0x00,sizeof(svc));
    svc.fec_mode = DB_DEFAULT_FEC_MODE;
    svc.port_iso = DB_DEFAULT_PORT_ISO;
    svc.uplink_flow_ctrl = DB_DEFAULT_UPLINK_FLOW_CTRL;
    svc.max_mtu = DB_DEFAULT_MAX_MTU;
    svc.qos_sched_mode = DB_DEFAULT_QOS_MODE; 
    svc.mac_aging_time = DB_DEFAULT_MAC_AGING_TIME; 
    svc.mc_time = DB_DEFAULT_MC_AGING_TIME;
    svc.mc_mode =  DB_DEFAULT_MC_MODE;
    svc.ppoe_plus_en = FALSE;
    svc.ds_shape_en = FALSE;
    svc.rstp = FALSE;
    svc.tcp = TRUE;
    svc.cfg_counter = 0;
    for(i = 0 ; i < DB_QUEUE_PRIO_LEN; i++){
        svc.pri_cos_map[i] = i;
    }
    db_write(DB_ONU_SVC_INFO_ID,(cs_uint8 *) &svc, sizeof(svc));

    /* init the mac bind */
    db_write(DB_MAC_BIND_ID, dbbuf, sizeof(db_mac_bind_t));

    /* init the voq size  */
    memset(&voq,0x00,sizeof(voq));
    for(i = 0; i < DB_MAX_VOQ_SIZE; i++){
        voq.voq_cfg[0][i] = DB_DEFAULT_DN_VOQ_THRESH;
        voq.voq_cfg[1][i] = DB_DEFAULT_UP_VOQ_THRESH;
    }
    db_write(DB_QOS_VOQ_ID,(cs_uint8 *) &voq, sizeof(voq));

    /*init the port mirror*/
    memset(&mirror,0x00,sizeof(mirror));
    mirror.enable = CS_UNI_PORT_ID1;
    db_write(DB_PORT_MIRROR_ID,(cs_uint8 *) &mirror, sizeof(mirror));
    
    /*init the remark */
    memset(&remark,0x00,sizeof(remark));
    for(i = 0; i < max_port; i++){
        remark.remark[i].remark_en = FALSE;
        for(j = 0; j < DB_QUEUE_PRIO_LEN; j++){
            remark.remark[i].remark[j] = j;
        }
    }
    db_write(DB_QOS_REMARK_ID,(cs_uint8 *) &remark, sizeof(remark));

    memset(&port_info,0x00,sizeof(port_info));
    for(i = 0 ; i < max_port; i++){
        port_info.port[i].admin= SDL_PORT_ADMIN_UP;
        port_info.port[i].auto_neg = SDL_PORT_AUTONEG_UP;
        port_info.port[i].work_mode = SDL_PORT_AUTO_10_100_1000;
        port_info.port[i].loop_detect = FALSE;
        port_info.port[i].pause = FALSE; 
        port_info.port[i].bc_ctrl_mode = SDL_STORM_TYPE_DISABLE;
        port_info.port[i].policy_enable = FALSE;
        port_info.port[i].ds_rate_limit_enable = FALSE;
    }

    db_write(DB_PORT_INFO_ID,(cs_uint8 *) &port_info, sizeof(port_info));

    
    /*init the vlan */
    memset(&vlan,0x00,sizeof(vlan));
    for(i = 0; i < max_port; i++){
       vlan.vlan[i].vlan_mode = SDL_VLAN_MODE_TRANSPARENT;
    }
    db_write(DB_VLAN_ID,(cs_uint8 *) &vlan, sizeof(vlan));

    db_write(DB_PROFILE_ID, dbbuf, sizeof(db_profile_struct_t)); 

    memset (& db_mac_limit, 0, sizeof(db_mac_limit_t));
    for(i = 0; i < max_port; i++){
       db_mac_limit.mac[i].num = __FDB_NO_MAC_LIMIT;
    }
    db_write (DB_MAC_LIMIT_ID , (cs_uint8 *)&db_mac_limit, sizeof(db_mac_limit_t));

    db_write(DB_MAC_FILTER_ID, dbbuf, sizeof(db_mac_bind_t));

    db_write(DB_STATIC_MAC_ID, dbbuf, sizeof(db_static_mac_t));    

    db_write(DB_CLS_ID ,dbbuf, sizeof(db_cls_t));

    db_write(DB_MC_VLAN_ID ,dbbuf, sizeof(db_mc_vlan_t));
    
    db_write(DB_MC_TAGSTRIP_ID ,dbbuf, sizeof(db_mc_tagstrip_t));
    
    memset (& db_mc_grp, 0, sizeof(db_mc_grp_t));
    for(i = 0; i < max_port; i++){
       db_mc_grp.mc_grp[i]= MC_PORT_DEFAULT_GROUP;
    }
    db_write (DB_MC_MAX_GRP_ID  , (cs_uint8 *)&db_mc_grp, sizeof(db_mc_grp_t));

    memset (&db_fast_leave, 0, sizeof (db_fast_leave_t));
    db_fast_leave.fast_leave=PORT_NON_FAST_LEAVE;
    db_write(DB_FAST_LEAVE_ID ,(cs_uint8 *) &db_fast_leave, sizeof(db_fast_leave_t));

    db_write(DB_ONU_ALARM_ID ,dbbuf, sizeof (db_onu_alarm_t));  

    db_write(DB_PORT_ALARM_ID ,dbbuf,  sizeof (db_port_alarm_t));  
 
    iros_free(dbbuf);
    
    return CS_E_OK;
    
}

cs_status db_update_handle(cs_uint32 version)
{
    int i=0;
    char *buff=NULL;
    int len=0;
    int max_db_num=sizeof(g_ctcCfgTable)/sizeof(db_data_confg_t);
    cs_uint32 max_db_len = 0;
    
    for(i=0;i<max_db_num;i++)
    {
        if(max_db_len < g_ctcCfgTable[i].max_len)
            max_db_len = g_ctcCfgTable[i].max_len;
    }
    buff = iros_malloc(IROS_MID_APP,max_db_len);
    if(buff == NULL)
    {
        cs_printf("DB Memory fail.\n");
        return CS_E_RESOURCE;
    }

    for(i=0;i<max_db_num;i++)
    {
        len=db_cfg_len_get(g_ctcCfgTable[i].id);  
        if(len&&(len!=g_ctcCfgTable[i].max_len))
        {
            memset(buff,0,max_db_len);
            db_read(g_ctcCfgTable[i].id,buff,max_db_len);
            db_write(g_ctcCfgTable[i].id,buff,g_ctcCfgTable[i].max_len);
        }
    }

    iros_free(buff);
    return CS_E_OK;
}


void db_init()
{
    db_core_init(DB_DATA_OFFSET,FALSE,FALSE);
    db_table_init(
            g_ctcCfgTable,
            sizeof(g_ctcCfgTable)/sizeof(g_ctcCfgTable[0]),
            DB_CUR_VERSION,
            db_set_default,
            db_update_handle);
}


void db_voq_restore()
{
    db_voq_t  voq;
    cs_uint32 i = 0, j = 0;
               cs_status ret = CS_E_OK;
    cs_callback_context_t context;
    
    memset(&voq,0x00,sizeof(voq));
    db_read(DB_QOS_VOQ_ID,(cs_uint8 *) &voq, sizeof(voq));

    for(i = 0; i< 2; i++){
        for(j = 0; j < DB_MAX_VOQ_SIZE; j++){
            ret = epon_request_onu_que_size_set(context,0 , 0 , i,j,voq.voq_cfg[i][j]);

            if(ret != CS_E_OK){
                cs_printf("set qos threshold to sdl failed, port = %d, q = %d , v = %d ret = %d ",
                        i, j,voq.voq_cfg[i][j],ret);
            }
        }
    }
}


extern cs_uint8 CGI_MAX_UNI_PORT ;

void db_svc_restore()
{
    db_onu_svc_t   svc;
    cs_status ret = CS_E_OK;
    cs_callback_context_t context;
    cs_port_id_t port;
    cs_pri_cos_map_t map;
    cs_sdl_policy_t       ds_rate;
#ifdef HAVE_ONU_RSTP
    cs_onu_stp_config_t  rstp_cfg;
#endif
    cs_uint8 max_port;
    int i; 

    startup_config_read(
        CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);

    memset(&svc,0x00,sizeof(svc));
    db_read(DB_ONU_SVC_INFO_ID,(cs_uint8 *) &svc, sizeof(svc));

    ret = epon_request_onu_fdb_age_set(context,
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,svc.mac_aging_time);

    if(ret != CS_E_OK){
        cs_printf("set mac aging time from sdl failed, ret = %d ",ret);
    }

    ret = mc_mode_set(0, svc.mc_mode);
    if(ret != CS_E_OK){
        cs_printf("set mc mode failed ,ret = %d \n",ret);
    }

    ret = mc_gmi_set(0,svc.mc_time);
    if(ret != CS_E_OK){
        cs_printf("set mc time failed ,ret = %d \n",ret);
    }

    for(i=0; i<max_port; i++){
        ret = epon_request_onu_port_mtu_set(
                context,ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
                i+1, svc.max_mtu);
        if(ret != CS_E_OK ) {
            cs_printf("set uni mgmt from SDL failed, ret = %d \n",ret);
        }
    }
    ret = epon_request_onu_port_mtu_set(
            context,ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
            CS_PON_PORT_ID, svc.max_mtu);

    if(ret != CS_E_OK ) {
        cs_printf("set  pon mgmt from SDL failed, ret = %d \n",ret);
    }

#ifdef HAVE_MPORTS
    port = CS_UPLINK_PORT;
#else
    port = CS_UNI_PORT_ID1;
#endif

    ret = epon_request_onu_port_flow_ctrl_set(context, ONU_DEVICEID_FOR_API, 
            ONU_LLIDPORT_FOR_API, port, svc.uplink_flow_ctrl);

    if(ret != CS_E_OK){
        cs_printf("set flow ctrl fail, ret=%d\n", ret);
    }
    ret = epon_request_onu_port_isolation_set(context, ONU_DEVICEID_FOR_API, 
            ONU_LLIDPORT_FOR_API, svc.port_iso);

    if(ret != CS_E_OK){
        cs_printf("set port iso fail, ret=%d\n", ret);
    }
#ifdef HAVE_PPPOE
    ret = pppoe_plus_enable(svc.ppoe_plus_en);

    if(ret != CS_E_OK){
        cs_printf("set pppoe fail, ret=%d\n", ret);
    }
#endif

    ret=epon_request_onu_fec_set ( context,  ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API,  svc.fec_mode);
    if(ret != CS_E_OK){
        cs_printf("set fec mode failed ,ret = %d \n",ret);
    }
    
    ret = epon_request_onu_schedule_mode_set(context,0,0,svc.qos_sched_mode );
    if(ret != CS_E_OK){
        cs_printf("set qos algo to sdl failed, ret = %d ",ret);
    }

    memcpy(map.cos,svc.pri_cos_map,DB_QUEUE_PRIO_LEN);
    ret = epon_request_onu_dot1p_map_set(context,
            0 , 0 , &map);
    if(ret != CS_E_OK){
        cs_printf("set qos user_pri_que to sdl failed, ret = %d ",ret);
    }

    ds_rate.enable = svc.ds_shape_en; 
    ds_rate.cbs = svc.ds_cbs;
    ds_rate.rate = svc.ds_shape_rate; 
    ret = epon_request_onu_port_ds_rate_limit_set(context, 
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, CS_DOWNLINK_PORT, &ds_rate);
    if(ret != CS_E_OK){
        cs_printf("set ds rate to sdl failed, port = %d , ret = %d \n",CS_DOWNLINK_PORT,ret);
    }

#ifdef HAVE_ONU_RSTP
    memset(&rstp_cfg, 0, sizeof(rstp_cfg));
    rstp_cfg.enable = svc.rstp;
    rstp_cfg.mask = ONU_MASK_STP_BRIDGE_ENABLE;
    
    ret = app_stp_bridge_cfg_set(&rstp_cfg);
    if(ret != CS_E_OK ) {
        cs_printf("set rstp to SDL failed, ret = %d \n",ret);
    }
#endif

#ifdef HAVE_MPORTS
    ret = epon_request_onu_big_burst_size_set(
            context,ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,svc.tcp);
    if(ret != CS_E_OK ) {
        cs_printf("set tcp to SDL failed, ret = %d \n",ret);
    }
#endif
}

void db_mac_bind_restore()
{
    cs_uint8 i = 0;
    cs_uint32 j = 0;
    cs_status ret = CS_E_OK;
    cs_callback_context_t     context;
    cs_sdl_mac_tbl_entry_t    mac_entry;
    db_mac_bind_t mac_bind;
    cs_uint8 max_port;

    startup_config_read(
        CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);

    memset(&mac_bind,0x00,sizeof(mac_bind));
    db_read(DB_MAC_BIND_ID,(cs_uint8 *) &mac_bind, sizeof(mac_bind));
    max_port = max_port >DB_MAX_PORT ? DB_MAX_PORT:max_port;
    
    for(i = 0; i < max_port; i ++){
        epon_request_onu_mac_bind_clr_entry(context, 
                ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,i+1);
        for(j =0; j < mac_bind.bind[i].num; j++){
            memcpy(mac_entry.mac.addr,mac_bind.bind[i].mac[j].addr,CS_MACADDR_LEN);
            mac_entry.mac_type = SDL_SRC_MAC;
            ret = epon_request_onu_mac_bind_add_entry(
                    context, 
                    ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, i+1, 1, &mac_entry);
            if(ret != CS_E_OK){
                cs_printf("set from sdl failed, ret = %d \n",ret);
            }
        }
    }
}
#ifdef HAVE_MPORTS
void db_port_mirror_restore()
{
    db_mirror_t mirror;
    cs_callback_context_t     context;
    cs_status ret = CS_E_OK;

    memset(&mirror,0x00,sizeof(mirror));
    db_read(DB_PORT_MIRROR_ID,(cs_uint8 *) &mirror, sizeof(mirror));
    ret = epon_request_onu_port_mirror_set(context, 0, 0, mirror.enable, mirror.rx_mask, mirror.tx_mask);
    if(CS_E_OK != ret){
        cs_printf("mirror set fail, ret=%d\n", ret);
    }
}
#endif


void db_qos_remark_restore()
{
#if 0
    cs_uint8 i = 0;
    cs_uint32 j = 0;
    cs_status ret = CS_E_OK;
    cs_callback_context_t     context;
    cs_pri_remark_t remark;
    db_remark_t db_cfg;

    cs_uint8 max_port;

    startup_config_read(
        CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);
    max_port = max_port >DB_MAX_PORT ? DB_MAX_PORT:max_port;
    memset(&db_cfg,0x00,sizeof(db_cfg));
    db_read(DB_QOS_REMARK_ID,(cs_uint8 *) &db_cfg, sizeof(db_cfg));

    for( i = 0; i < max_port; i++){
        remark.en = db_cfg.remark[i].remark_en;
        for(j = 0; j < DB_QUEUE_PRIO_LEN; j++){
            remark.priority[j] = db_cfg.remark[i].remark[j];
        }
    
        ret = epon_request_onu_dot1p_remark_set(
                context,0 , 0 , i + 1,&remark);
        if(ret != CS_E_OK){
            cs_printf("set qos user_pri_regen to sdl failed, port = %d  ret = %d ",i + 1 ,ret);
        }
    }
#endif
}

void db_vlan_restore()
{
    cs_uint16 translate_num;
    oam_ctc_onu_vlan_xlate_entry translate_vlan[CTC_VLAN_MODE_TRANSLATE_MAX];
    cs_uint16 trunk_num ;
    oam_ctc_onu_vlan_tag_t trunk_vlan[CTC_VLAN_MODE_TRUNK_MAX];
    ctc_oam_agg_vlan_t  agg_vlan;
    cs_uint8 vlan_mode;
    oam_ctc_onu_vlan_tag_t def_vlan;
    db_vlan_t vlan;

    cs_uint8 i = 0;
    cs_uint32 j = 0;
    cs_status ret = CS_E_OK;
    cs_uint8 max_port;

    startup_config_read(
            CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);
    max_port = max_port >DB_MAX_PORT ? DB_MAX_PORT:max_port;

    memset(&vlan,0x00,sizeof(vlan));
    db_read(DB_VLAN_ID,(cs_uint8 *) &vlan, sizeof(vlan));

    for(i = 0 ; i < max_port; i++){
        vlan_mode = vlan.vlan[i].vlan_mode ;
        def_vlan.tpid = vlan.vlan[i].def_vlan.tpid ;
        def_vlan.tag = vlan.vlan[i].def_vlan.tag ;
        switch(vlan_mode){
            case CTC_VLAN_MODE_TRANSPARENT:
            ret = ctc_oam_onu_transparent_vlan_set_adapt(i + 1);
            if(ret != CS_E_OK){
                cs_printf("set vlan transparent falied, ret = %d ",ret);
            }
            break;
        case CTC_VLAN_MODE_TAG:
            ret = ctc_oam_onu_tag_vlan_set_adapt(i + 1,def_vlan);
            if(ret != CS_E_OK){
                cs_printf("set vlan tag falied, ret = %d ",ret);
            }
            break;
        case CTC_VLAN_MODE_TRANSLATE:
            translate_num =  vlan.vlan[i].vlan_num/2;
            if(translate_num <= DB_VLAN_MAX)
            {
                for(j = 0; j < translate_num; j++){
                    translate_vlan[j].tag1.tpid = vlan.vlan[i].vlan[j*2].tpid ;
                    translate_vlan[j].tag1.tag = vlan.vlan[i].vlan[j*2].tag ;
                    translate_vlan[j].tag2.tpid = vlan.vlan[i].vlan[j*2 + 1].tpid ;
                    translate_vlan[j].tag2.tag = vlan.vlan[i].vlan[j*2 + 1].tag ;
                }

                ret = ctc_oam_onu_translate_vlan_set_adapt(i + 1,def_vlan,translate_num,translate_vlan);
            }
            else
            {
                ret = CS_E_ERROR;
            }
            if(ret != CS_E_OK){
                cs_printf("set vlan translate falied, ret = %d ",ret);
            }
            break;
        case CTC_VLAN_MODE_TRUNK:
             trunk_num = vlan.vlan[i].vlan_num;
            for(j = 0 ; j < trunk_num; j++){
                trunk_vlan[j].tpid = vlan.vlan[i].vlan[j].tpid;
                trunk_vlan[j].tag = vlan.vlan[i].vlan[j].tag;
            }
            ret = ctc_oam_onu_trunk_vlan_set_adapt( i + 1,def_vlan,trunk_num,trunk_vlan);
            if(ret != CS_E_OK){
                cs_printf("set vlan translate falied, ret = %d ",ret);
            }
            break;
        case CTC_VLAN_MODE_AGG:
            agg_vlan.agg_vlan_num = vlan.vlan[i].agg_vlan_num;
            agg_vlan.agg_dst_vlan.tpid =  vlan.vlan[i].vlan[0].tpid;
            agg_vlan.agg_dst_vlan.tag =  vlan.vlan[i].vlan[0].tag;
            for(j = 0; j < agg_vlan.agg_vlan_num  && j < CTC_ONU_AGGR_VLAN_NUM; j++){
                agg_vlan.agg_vlan[j].tpid =  vlan.vlan[i].vlan[j+1].tpid;
                agg_vlan.agg_vlan[j].tag =  vlan.vlan[i].vlan[j+1].tag;
            }
            ret = ctc_oam_onu_agg_vlan_set_adapt(i+1,def_vlan,1,&agg_vlan);
            if(ret != CS_E_OK){
                cs_printf("set vlan agg falied, ret = %d ",ret);
            }

            break;
        }
    }
}


void db_port_info_restore()
{
    db_port_t   port_info;
    cs_sdl_policy_t policy;
    cs_sdl_port_speed_cfg_t cfg = 0;
    cs_callback_context_t     context;
    cs_uint8 i = 0;
    cs_status ret = CS_E_OK;
    cs_uint8 max_port;
    db_onu_svc_t   svc;

    startup_config_read(
            CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);
    max_port = max_port >DB_MAX_PORT ? DB_MAX_PORT:max_port;

    memset(&port_info,0x00,sizeof(port_info));
    db_read(DB_PORT_INFO_ID,(cs_uint8 *) &port_info, sizeof(port_info));
    for(i = 0; i < max_port;i ++){ 
        memset(&policy, 0, sizeof(policy));
        policy.rate = port_info.port[i].bc_ctrl_rate;
        policy.enable = port_info.port[i].ctrl_enable;
        
        ret = epon_request_onu_port_storm_ctrl_set(context, ONU_DEVICEID_FOR_API,
                ONU_LLIDPORT_FOR_API, i+1, SDL_RL_MOD_BPS, port_info.port[i].bc_ctrl_mode, &policy);
        if(ret != CS_E_OK){
            cs_printf("set storm ctrl fail %d\n", ret);
        }

        memset(&policy, 0, sizeof(policy));
        /*set up police*/
        policy.enable =  port_info.port[i].policy_enable;
        policy.rate = port_info.port[i].policy_rate;
        
        policy.cbs = port_info.port[i].policy_cbs;
        policy.ebs = port_info.port[i].policy_ebs;
        

        ret = epon_request_onu_port_policy_set(context, ONU_DEVICEID_FOR_API,
                ONU_LLIDPORT_FOR_API, i+1, &policy);
        if(ret != CS_E_OK){
            cs_printf("policy set fail\n");
        }

        db_read(DB_ONU_SVC_INFO_ID,(cs_uint8 *) &svc, sizeof(svc));

        if (svc.ds_shape_en==0) {
            memset(&policy, 0, sizeof(policy));
            /*set rate limit*/
            policy.enable = port_info.port[i].ds_rate_limit_enable;
            policy.rate = port_info.port[i].ds_rate_limit;
            policy.cbs=port_info.port[i].ds_cbs;

            ret = epon_request_onu_port_ds_rate_limit_set(context, ONU_DEVICEID_FOR_API,
                    ONU_LLIDPORT_FOR_API, i+ 1, &policy);
            if(ret != CS_E_OK){
                cs_printf("rate limit set fail\n");
            }
        }

        /*set flow contrl*/
        ret = epon_request_onu_port_flow_ctrl_set(context, ONU_DEVICEID_FOR_API,
                ONU_LLIDPORT_FOR_API, i+1, port_info.port[i].pause);
        if(ret != CS_E_OK){
            cs_printf("flow ctrl set fail\n");
        }
        
#ifdef HAVE_LOOP_DETECT
        if(port_info.port[i].loop_detect){
            ret = loop_detect_enable(i+1);
        }
        else{
            ret = loop_detect_disable(i+1);
        }

        if(ret != CS_E_OK)
        {
            cs_printf("set loop detect fail, ret=%d\n", ret);
        }

#endif
        /* set port admin */
        ret = epon_request_onu_port_admin_set(context, 
                ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, i+1, port_info.port[i].admin);
        if(ret != CS_E_OK){
            cs_printf("set port admin status failed ,ret = %d \n",ret);
        }

        /* set port auto neg */
        epon_request_onu_port_status_get(context, 
                ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, i+1, &cfg);

        if (port_info.port[i].auto_neg == SDL_PORT_AUTONEG_DOWN) {
            if (cfg == SDL_PORT_AUTO_10_100_1000 || 
                    cfg == SDL_PORT_AUTO_10_100 || 
                    cfg == SDL_PORT_1000_FULL) {
                cfg = SDL_PORT_100_FULL;
                ret = epon_request_onu_port_status_set(context, 
                        ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, i+1, cfg);
            }
        } else {
            if (cfg != SDL_PORT_AUTO_10_100_1000 && 
                    cfg != SDL_PORT_AUTO_10_100 &&
                    cfg != SDL_PORT_1000_FULL) {
                cfg = SDL_PORT_AUTO_10_100_1000;
                ret = epon_request_onu_port_status_set(context, 
                        ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, i+1, cfg);
            }
        }

        if(ret != CS_E_OK){
            cs_printf("set port auto status failed ,ret = %d \n",ret);
        }

        ret = epon_request_onu_port_status_set(context, 
                ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, i+1, port_info.port[i].work_mode);
        if(ret != CS_E_OK){
            cs_printf("set port working mode failed ,ret = %d \n",ret);
        }
    }
}


void db_mac_limit_restore()
{
    cs_uint8         i = 0;
    cs_status       ret = CS_E_OK;
    cs_uint8         max_port=0;
    db_mac_limit_t  db_mac_limit;
    cs_callback_context_t     context;

    startup_config_read(
            CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);
    max_port = max_port >DB_MAX_PORT ? DB_MAX_PORT:max_port;

    memset(&db_mac_limit, 0x00, sizeof(db_mac_limit_t));
    ret=db_read(DB_MAC_LIMIT_ID,(cs_uint8 *) &db_mac_limit, sizeof(db_mac_limit_t));
    if (ret !=CS_E_OK ) {
        return ;
    } 

    for(i = 0 ; i < max_port; i++){
        ret = epon_request_onu_fdb_mac_limit_set(context, 0, 0, i+1, db_mac_limit.mac[i].num);
        
    }
}


void db_mac_filter_restore()
{
    cs_status       ret = CS_E_OK;
    db_mac_bind_t  db_mac_filter;
    db_mac_bind_entry_t   * db_filter_entry=NULL;
    cs_callback_context_t     context;
    cs_uint8         max_port=0;
    cs_uint32       i=0;
    cs_uint32       j=0;
    cs_sdl_mac_tbl_entry_t   mac_list[16];

    startup_config_read(
            CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);
    max_port = max_port >DB_MAX_PORT ? DB_MAX_PORT:max_port;
    

    memset(&db_mac_filter, 0x00, sizeof(db_mac_filter));

    
    ret=db_read(DB_MAC_FILTER_ID,(cs_uint8 *) &db_mac_filter, sizeof(db_mac_filter));
    if (ret !=CS_E_OK ) {
        return ;
    } 

    for(i = 0; i < max_port; i ++){
        memset (&mac_list, 0, sizeof(mac_list ) );
        epon_request_onu_mac_filter_clr_entry(context, 
                ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,i+1);
        db_filter_entry=&db_mac_filter.bind[i];
        
        for(j =0; j < db_filter_entry->num; j++){
            mac_list[j].mac_type = SDL_SRC_MAC;
            memcpy(mac_list[j].mac.addr,db_filter_entry->mac[j].addr ,CS_MACADDR_LEN);
            mac_list[j].vlanid = 0;      
        }
        ret = epon_request_onu_mac_filter_add_entry(
                context, 
                ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, i+1, db_filter_entry->num, mac_list);
        
    }


}




void db_cls_restore( )
{
    cs_uint32       i=0;
    cs_status       ret = CS_E_OK;
    cs_uint8       max_port=0;
    cs_callback_context_t     context;
    db_cls_t    db_cls;

    startup_config_read(
            CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);
    max_port = max_port >DB_MAX_PORT ? DB_MAX_PORT:max_port;
    
    ret=db_read(DB_CLS_ID,(cs_uint8 *) &db_cls, sizeof(db_cls_t));
    if (ret !=CS_E_OK ) {
        return ;
    } 

    for(i = 0; i < max_port; i ++){
        epon_request_onu_port_classification_add ( context
            , ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API , i+1, db_cls.cls[i].num , ( cs_sdl_classification_t *) db_cls.cls[i].rule);
        
    }

}


void db_mc_vlan_restore( )
{
    cs_uint32       i=0;
    cs_status       ret = CS_E_OK;

    cs_uint8       max_port=0;
    cs_callback_context_t     context;
    db_mc_vlan_t   db_mc_vlan;
    db_mc_vlan_entry_t  * db_entry=NULL;


    startup_config_read(
            CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);
    max_port = max_port >DB_MAX_PORT ? DB_MAX_PORT:max_port;
    

    ret=db_read(DB_MC_VLAN_ID,(cs_uint8 *) &db_mc_vlan, sizeof(db_mc_vlan_t));
    if (ret !=CS_E_OK ) {
        return ;
    } 

    for(i = 0; i < max_port; i ++){
        db_entry=&db_mc_vlan.entry[i];
        epon_request_onu_mc_vlan_add ( context
            , ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API , i+1, db_entry->vlan, db_entry->num);
        
    }

}


void db_mc_tagstrip_restore( )
{
    cs_uint32       i=0;
    cs_uint32       vlan_pair_index=0;
    cs_status       ret = CS_E_OK;
    cs_uint8       max_port=0;
    cs_callback_context_t     context;
    db_mc_tagstrip_t   db_mc_tagstrip;
    db_mc_tagstrip_entry_t  * db_entry=NULL;
    cs_sdl_vlan_cfg_t             swap_rule[CTC_ONU_AGGR_VLAN_NUM];
    
    startup_config_read(
            CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);
    max_port = max_port >DB_MAX_PORT ? DB_MAX_PORT:max_port;
    
    ret=db_read(DB_MC_TAGSTRIP_ID,(cs_uint8 *) &db_mc_tagstrip, sizeof(db_mc_tagstrip_t));
    if (ret !=CS_E_OK ) {
        return ;
    } 
    for(i = 0; i < max_port; i ++){
        db_entry=&db_mc_tagstrip.entry[i];
        memset ( swap_rule, 0, sizeof (swap_rule ) );
        
        if (db_entry->tagStriped==CTC_MCVLAN_TAG_OPER_SWAP) {
            for(vlan_pair_index = 0; vlan_pair_index<db_entry->num; ++vlan_pair_index) {
                swap_rule[vlan_pair_index].s_vlan.vid = db_entry->vlan[vlan_pair_index*2];
                swap_rule[vlan_pair_index].c_vlan.vid = db_entry->vlan[vlan_pair_index*2+1];
            }
        }
        epon_request_onu_mc_vlan_action_set ( context,
            ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API,i+1, (cs_sdl_mc_vlan_act_t )db_entry->tagStriped,  swap_rule, db_entry->num);
        
    }

}


void db_mc_max_grp_restore( )
{
    cs_uint32       i=0;
    cs_status       ret = CS_E_OK;
    cs_uint8       max_port=0;
    db_mc_grp_t   db_mc_grp;

    startup_config_read(
            CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);
    max_port = max_port >DB_MAX_PORT ? DB_MAX_PORT:max_port;
    
    ret=db_read (DB_MC_MAX_GRP_ID,(cs_uint8 *) &db_mc_grp, sizeof(db_mc_grp_t));
    if (ret !=CS_E_OK ) {
        return ;
    } 

    for(i = 0; i < max_port; i ++){
       mc_port_max_grp_set (ONU_DEVICEID_FOR_API ,  i+1 , db_mc_grp.mc_grp[i]);

    }

}

void db_fast_leave_restore( )
{
    db_fast_leave_t   db_fast_leave;
    cs_status       ret = CS_E_OK;

    ret=db_read(DB_FAST_LEAVE_ID,(cs_uint8 *) &db_fast_leave, sizeof(db_fast_leave_t));
    if (ret !=CS_E_OK ) {
        return ;
    } 

    mc_set_fast_leave_state( db_fast_leave.fast_leave);

}


void db_alarm_restore( )
{
    db_onu_alarm_t   * db_alarm=NULL;
    cs_uint32       db_alarm_len=0;
    cs_uint32       i=0;
    db_alarm_entry_t   *  db_entry=NULL;
    cs_status       ret = CS_E_OK;

    db_alarm_len= sizeof(db_onu_alarm_t);
    db_alarm=(db_onu_alarm_t *) iros_malloc(IROS_MID_APP , db_alarm_len);
    if ( db_alarm ==NULL ) {
        ret=CS_E_ERROR;
        goto END;
    }

    ret=db_read(DB_ONU_ALARM_ID,(cs_uint8 *) db_alarm,db_alarm_len);
    if (ret !=CS_E_OK ) {
        goto END;
    } 
   
    for ( i=0; i < db_alarm->num ; i++){
        db_entry= &db_alarm->entry[i];
        ctc_oam_set_alarm_config (db_entry->alarm_type , 0, db_entry->alarm_code, db_entry->enable);
        if ( db_entry->threshold_type !=CTC_THRESH_UNSUPPORT) {
            ctc_oam_set_alarm_threshold ( db_entry->alarm_type, 0, db_entry->alarm_code , db_entry->raise_threshold,  db_entry->clear_threshold);
        }
    }
   
END:
    if ( db_alarm!=NULL) {
        iros_free(db_alarm);
        db_alarm=NULL;
    }
    
}


void db_port_alarm_restore ( )
{
    db_port_alarm_t   * db_port_alarm=NULL;
    cs_uint32       db_port_alarm_len=0;
    cs_uint32       i=0;
    db_alarm_entry_t   *  db_entry=NULL;
    cs_uint8       max_port=0;
    cs_uint32     port_index=0;
    cs_status       ret = CS_E_OK;

    startup_config_read(
            CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);
    max_port = max_port >DB_MAX_PORT ? DB_MAX_PORT:max_port;

    db_port_alarm_len= sizeof(db_port_alarm_t);

    db_port_alarm=(db_port_alarm_t *) iros_malloc(IROS_MID_APP , db_port_alarm_len);
    if ( db_port_alarm ==NULL ) {
        ret=CS_E_ERROR;
        goto END;
    }

    ret=db_read(DB_PORT_ALARM_ID,(cs_uint8 *) db_port_alarm,db_port_alarm_len);
    if (ret !=CS_E_OK ) {
        goto END;
    }
    
    for (port_index=0;port_index < max_port; port_index++) {

        for ( i=0; i < db_port_alarm->onu_entry[port_index].num  ; i++){
            db_entry= &db_port_alarm->onu_entry[port_index].entry[i];
            ctc_oam_set_alarm_config (db_entry->alarm_type , port_index+1 , db_entry->alarm_code, db_entry->enable);
            
            if ( db_entry->threshold_type !=CTC_THRESH_UNSUPPORT) {
                ctc_oam_set_alarm_threshold ( db_entry->alarm_type, port_index+1 , db_entry->alarm_code , db_entry->raise_threshold,  db_entry->clear_threshold);
            }
        }

    }
   
   
END:
    if ( db_port_alarm!=NULL) {
        iros_free(db_port_alarm);
        db_port_alarm=NULL;
    }
    
}


void db_static_mac_restore( )
{
    cs_status       ret = CS_E_OK;    
    cs_callback_context_t     context;
    cs_uint8         max_port=0;
    cs_uint32       i=0;
    cs_uint32       j=0;
    db_mac_entry_t  * db_entry=NULL;
    cs_sdl_fdb_entry_t  mac_entry;
    db_static_mac_t  db_mac_table; 

    startup_config_read(
            CFG_ID_SWITCH_PORT_NUM, sizeof(max_port), &max_port);
    max_port = max_port >DB_MAX_PORT ? DB_MAX_PORT:max_port;
    
    memset(&db_mac_table, 0x00, sizeof(db_static_mac_t));

    ret=db_read(DB_STATIC_MAC_ID ,(cs_uint8 *) &db_mac_table, sizeof(db_static_mac_t));
    if (ret !=CS_E_OK ) {
        return ;
    } 

 #ifdef HAVE_MPORTS
    for(i = 0; i < max_port; i ++){
        ret = epon_request_onu_fdb_entry_clr_per_port(context, 0, 0, SDL_FDB_CLR_STATIC, i+1);
    }
#else    
    ret = epon_request_onu_fdb_entry_clr(context, 0, 0, SDL_FDB_CLR_STATIC);
#endif   

    for(i = 0; i < max_port; i ++){
        
        db_entry= &db_mac_table.entry[i];
        
        for(j =0; j < db_entry->num; j++){ 
            memset (&mac_entry, 0, sizeof (mac_entry ) );
            mac_entry.port=i+1;
            memcpy(mac_entry.mac.addr, db_entry->mac[j].addr, CS_MACADDR_LEN);
            mac_entry.vlan_id=db_entry->vlan;
            mac_entry.type=SDL_FDB_ENTRY_STATIC;
            epon_request_onu_fdb_entry_add ( 
                        context,
                        ONU_DEVICEID_FOR_API,
                        ONU_LLIDPORT_FOR_API
                        , &mac_entry);
            
        }

        if(ret != CS_E_OK){
            cs_printf("set from sdl failed, ret = %d \n",ret);
        }
        
    }


}

#if 1
//added by zhuxh
void db_mgmt_info_restore(void)
{
    db_mgmt_t  mgmt_info; 

    memset(&mgmt_info,0x00,sizeof(mgmt_info));
    db_read(DB_MGMT_INFO_ID,(cs_uint8*) &mgmt_info,sizeof(mgmt_info));

    if(mgmt_info.inband_enable == 1){
        epon_request_onu_ip_config_set(mgmt_info.inband_ip, mgmt_info.inband_mask,mgmt_info.gw);
    }else{
        epon_request_onu_ip_config_set(mgmt_info.ip, mgmt_info.mask,0);
    }
    epon_request_onu_inband_ip_config_set(mgmt_info.inband_enable,mgmt_info.inband_pri,
            mgmt_info.inband_vlan,mgmt_info.mgmt_net,mgmt_info.mgmt_mask, mgmt_info.gw);

    epon_request_onu_ip_mtu_set(mgmt_info.mgmt_mtu);
}

void db_timeout_restore(void)
{
	cs_uint32 ret = 0 ; 
	db_timeout_t db_timeout;

	ret = db_read(DB_TIMEOUT_ID,(cs_uint8 *) &db_timeout, sizeof(db_timeout));
	if(ret != CS_E_OK){
		db_timeout.web_timeout = DB_TIMEOUT_DEFAULT;
		db_timeout.telnet_timeout = DB_TIMEOUT_DEFAULT;

		db_write(DB_TIMEOUT_ID,(cs_uint8 *) &db_timeout, sizeof(db_timeout));
	}
}
#endif

void db_restore()
{
	db_mgmt_info_restore();

    db_svc_restore();

    db_mac_bind_restore();
	
    db_voq_restore();
	
#ifdef HAVE_MPORTS
    db_port_mirror_restore();
#endif

    db_qos_remark_restore();

    db_vlan_restore();
	
    db_port_info_restore();

	db_timeout_restore();

	#if 0
    db_mac_limit_restore( );

    db_mac_filter_restore();

    db_static_mac_restore();
    
    db_cls_restore();

    db_mc_vlan_restore();

    db_mc_tagstrip_restore();

    db_mc_max_grp_restore();

    db_fast_leave_restore();
	#endif
    
}

void db_restore_to_factory()
{
    db_set_default();
    db_save_to_flash();
}

#endif
