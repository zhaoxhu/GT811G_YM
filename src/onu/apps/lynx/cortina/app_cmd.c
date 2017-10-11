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
#include "plat_common.h"
#include "sdl_api.h"
#include "cs_utils.h"
#ifdef HAVE_DB_MANAGMENT
#include "db_instance.h"
#endif
#include "sys_cfg.h"
#include "oam_api.h"
#include "oam_std_pdu.h"
#include "oam_event.h"
#ifdef HAVE_ONU_8021X
#include "port_dot1x.h"
#endif

#ifdef HAVE_ONU_RSTP
#include "stp_api.h"
#endif
#include "packet.h"
#include "cs_cmd.h"
#include "port_stats.h"
#ifdef HAVE_DHCP
#include "dhcp.h"
#endif
#ifdef HAVE_PPPOE
#include "pppoe.h"
#include "pppoe_session.h"
#endif
#include "power_saving.h"

cs_uint8 pktTrace;

cs_uint8 pktNttTrace;
cs_uint8 pktKtTrace;

extern cs_uint8 oam_pkt_proc(cs_pkt_t *pPkt);
void app_cmd_help()
{
    cs_printf("app errevt - Set errored event parameters\n");
    cs_printf("app eesim - Simulated OAM alarm for event generation\n");
    cs_printf("app dg - Set to force ONU to send dying gasp\n");
    cs_printf("app crtcevt - Set to force ONU to send critical event\n");
#ifdef HAVE_NTT_OAM
    cs_printf("app sysmib des - Set system description\n");
    cs_printf("app sysmib oid - Set system oid\n");
    cs_printf("app sysmib con - Set system contact\n");
    cs_printf("app sysmib name - Set system name\n");
    cs_printf("app sysmib location - Set system location\n");
    cs_printf("app sysmib serv - Set system service\n");
#endif
    cs_printf("app lpbk - Set loopback\n");
    cs_printf("app show - Show application configurations\n");
    cs_printf("app pktr - set pkt trace\n");
    cs_printf("app ps show  - show power saving cfg\n");
    cs_printf("app ps ctrl [sleep_dur] [wake_dur] [sleep_flag] [sleep_mode]  - power saving control\n");
    cs_printf("app ps cfg [early_wakeup] [max_sleep_dur] - power saving config\n");
    cs_printf("app capture <enable> - packet remote capture enable/disable \n");
#ifdef HAVE_DB_MANAGMENT
	cs_printf("app db <save>/<reset> - save db or reset db \n");
#endif
}

#ifdef HAVE_LOOP_DETECT
extern void loop_detect_test(cs_uint8 type, cs_port_id_t port, cs_uint32 value);
extern void loop_detect_show();
#endif
#ifdef HAVE_ONU_RSTP
extern void stp_bridge_state_show();
extern void stp_port_state_show(cs_port_id_t port);
extern void stp_set_port_debug(cs_uint8 type, cs_uint8  value );
#endif
extern cs_status vlan_cmd_proc(int argc , char **argv);

#ifdef HAVE_ZTE_SIJIE
extern cs_status manage_vlan_cmd_proc(int argc , char **argv);
#endif /*END_HAVE_ZTE_SIJIE*/

sal_cmd_result_t app_cmd_proc(int argc , char **argv)
{
    cs_status ret = CS_E_OK;
    
    if(strcmp(argv[0],"app"))
        return SAL_CMD_FAIL;
    if(argc == 1)
    {
        app_cmd_help();
    }
    else
    {
        if(!strcmp(argv[1], "errevt") && argc == 5)
        {
            cs_uint8 type = cs_atoi(argv[2]);
            cs_uint32 win = cs_atoi(argv[3]);
            cs_uint32 threshold = cs_atoi(argv[4]);
            if(CS_E_PARAM == oam_link_event_window_set(type,win*100)){
                    cs_printf("set failed:wrong param \n");
                    cs_printf("    type:1 - error symbol, win:%d - %d \n",
                            OAM_LINK_ERROR_SYMBOL_PERIOD_WIN_MIN/100,
                            OAM_LINK_ERROR_SYMBOL_PERIOD_WIN_MAX/100);
                    cs_printf("    type:2 - error frame, win:%d - %d \n",
                            OAM_LINK_ERROR_FRAME_WIN_MIN/100,
                            OAM_LINK_ERROR_FRAME_WIN_MAX/100);
                    cs_printf("    type:3 - error frame period, win:%d - %d \n",
                            OAM_LINK_ERROR_PERIOD_WIN_MIN/100,
                            OAM_LINK_ERROR_PERIOD_WIN_MAX/100);
                    cs_printf("    type:4 - error frame summary, win:%d - %d \n",
                            OAM_LINK_ERROR_FRAME_SECOND_SUMMERY_WIN_MIN/100,
                            OAM_LINK_ERROR_FRAMESECOND_SUMMERY_WIN_MAX/100);
                    return SAL_CMD_FAIL;
            }
            cs_printf("set type = %d , really win = %d success\n", type, 
                    type == OAM_EVENT_TLV_ERR_SYM_PERIOD ? win/10*OAM_LINK_ERROR_SYMBOL_PER_SECOND:
                    type == OAM_EVENT_TLV_ERR_FRAME_PERIOD ? win *OAM_LINK_ERROR_PERIOD_PER_SECOND/10:
                    win*100 );
            oam_link_event_threshold_set(type,threshold);
            cs_printf("set type = %d , threshold = %d success \n", type, threshold);
        }
        #ifdef HAVE_LOOP_DETECT
        else if(!strcmp(argv[1], "loop"))
        {
            if(!strcmp(argv[2], "show"))
                loop_detect_show();
            else
                loop_detect_test(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
        }
        #endif
        #ifdef HAVE_DHCP
        else if(!strcmp(argv[1], "dhcp"))
        {
            if(argc == 4 && !strcmp(argv[2], "enable"))
            {
                dhcp_enable(atoi(argv[3]));
            }
            else if(argc == 4 && !strcmp(argv[2], "test"))
            {
                dhcp_test(atoi(argv[3]));
            }
            else if(argc == 3 && !strcmp(argv[2], "show"))
            {
                dhcp_show();
            }
        }
        #endif
        
        #ifdef HAVE_PPPOE
        else if(!strcmp(argv[1], "pppoe"))
        {   

            if(!strcmp(argv[2], "auth")){
                /*Set auth mode, username and password*/
                pppoe_auth_mode_set(atoi(argv[3]), argv[4], argv[5]);
            }
            else if(!strcmp(argv[2], "start")){
                /*start test*/
                pppoe_discovery_test_start(atoi(argv[3]), atoi(argv[4]));;
            }
            else if(!strcmp(argv[2], "stop")){
                /*stop pppoe simulation test*/
                pppoe_simulation_stop(atoi(argv[3]));
            }
            else if(!strcmp(argv[2], "rslt")){
                pppoe_cmd_result_get(atoi(argv[3]));
            }
            else{
                pppoe_cmd(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
            }
        }
        #endif

        else if(!strcmp(argv[1], "eesim"))
        {
            //sendOAMNotification(atoi(argv[2]), 0x28, 0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000);
        }
        else if(!strcmp(argv[1], "dg"))
        {
             oam_send_dying_gasp(0);
        }
        else if(!strcmp(argv[1], "crtcevt") && argc == 3)
        {
            //postOAMCriticalEvent(atoi(argv[2]));
        }
#ifdef HAVE_NTT_OAM
        else if(!strcmp(argv[1], "sysmib"))
        {
            if(!strcmp(argv[2], "des")  && argc == 4)
            {
                cs_uint8 des[32];
                strncpy(des, argv[3], 32);
                db_common_write(DB_USER_DATA_MIB_SYS_DESC_ID, des, sizeof(des));
            }
            else if(!strcmp(argv[2], "oid")  && argc > 4)
            {
                cs_uint8 oid[12];
                cs_uint8 new_oid[24];
                cs_uint8 i, j;
                cs_uint32 len;

                for(i = 0; i<argc-3; i++)
                {
                    oid[i] = atoi(argv[3+i]);
                    if(i == 11)
                        break;
                }
                len = (argc-3)<12? argc-3 : 12;
                new_oid[0] = oid[0]*40 + oid[1];
                j = 1;
                for(i = 2; i < len; i++)
                {
                    if(oid[i]>127)
                    {
                        new_oid[j] = (oid[i] >> 7) | 0x80;
                        j++;
                        new_oid[j] = oid[i] & 0x7f;
                        j++;
                    }
                    else
                    {
                        new_oid[j] = oid[i];
                        j++;
                    }
                }
                db_common_write(DB_USER_DATA_MIB_SYS_OBJ_ID, new_oid, j);
            }
            else if(!strcmp(argv[2], "con")  && argc == 4)
            {
                cs_uint8 con[32];
                strncpy(con, argv[3], 32);
                db_common_write(DB_USER_DATA_MIB_SYS_CONT_ID, con, sizeof(con));
            }
            else if(!strcmp(argv[2], "name")  && argc == 4)
            {
                cs_uint8 nam[32];
                strncpy(nam, argv[3], 32);
                db_common_write(DB_USER_DATA_MIB_SYS_NAME_ID, nam, sizeof(nam));
            }
            else if(!strcmp(argv[2], "loc")  && argc == 4)
            {
                cs_uint8 loc[32];
                strncpy(loc, argv[3], 32);
                db_common_write(DB_USER_DATA_MIB_SYS_LOCA_ID, loc, sizeof(loc));
            }
            else if(!strcmp(argv[2], "serv")  && argc == 4)
            {
                cs_uint8 ser;
                ser = atoi(argv[3]);
                db_common_write(DB_USER_DATA_MIB_SYS_SERV_ID, &ser, sizeof(ser));
            }
            
        }
#endif
        else if(!strcmp(argv[1], "lpbk"))
        {
            cs_uint8 lp;
            lp = atoi(argv[2]);
            startup_config_write(CFG_ID_LOOPBACK_DEPTH, sizeof(lp), &lp);
        }
        else if(!strcmp(argv[1], "disable"))
        {
            cs_uint8 lp;
            lp = atoi(argv[2]);
            startup_config_write(CFG_ID_LOOPBACK_DEPTH, sizeof(lp), &lp);
        }        
        else if(!strcmp(argv[1], "pktprt"))
        {
            if(!strcmp(argv[2], "trace")) {
                cs_pkt_type_t type = atoi(argv[3]);

                if(type > CS_PKT_TYPE_NUM) {
                    for(type = 0; type <= CS_PKT_TYPE_NUM; type++) {
                        app_pkt_toggle_trace(type);
                    }                
                }
                else {
                    app_pkt_toggle_trace(type);
                }
            }
            else if(!strcmp(argv[2], "oam") || !strcmp(argv[2], "all"))
            {
                oam_set_pkt_dump(OAM_DUMP_TYPE_ALL);
            }
        #ifdef HAVE_ONU_8021X
            else if(!strcmp(argv[2], "dot1x"))
            {
                
                if(pktDot1xTrace) pktDot1xTrace = 0;
                else pktDot1xTrace = 1;
                cs_printf("Set pktTrace to %d\n", pktDot1xTrace);
            }
        #endif	
            #ifdef HAVE_ONU_RSTP
            else if(!strcmp(argv[2], "rstp"))
            {
                if(pktRstpTrace) 
                    pktRstpTrace = 0;
                else 
                    pktRstpTrace = 1;
                cs_printf("Set pktTrace to %d\n", pktRstpTrace);
            }
            #endif
            else if(!strcmp(argv[2], "noam"))
            {
                oam_set_pkt_dump(OAM_DUMP_TYPE_ORG);
            }          
        }
        else if(!strcmp(argv[1], "show"))
        {
#ifdef HAVE_DB_MANAGMENT
            if(!strcmp(argv[2], "db"))
            {
                db_config_dump();
            }
 #endif
            if(!strcmp(argv[2], "oam-sts") && argc == 4)
            {
                oam_intf_dump(atoi(argv[3]));
                oam_dump_link_event_table();
            }
            else if(!strcmp(argv[2], "stats"))
            {
                if(!strcmp(argv[3], "pon"))
                {
                    app_show_pon_stats_cmd();
                }
                else if(!strcmp(argv[3], "uni") && argc == 5)
                {
                    app_show_uni_counter((cs_port_id_t)atoi(argv[4]));
                }
            }
            #ifdef HAVE_ONU_8021X
            if(!strcmp(argv[2], "1x-sts"))
            {
                app_8021x_show_status();
            }
            #endif
            #ifdef HAVE_IP_STACK
            /*
            extern void eth_show_statistics(void);
            if(!strcmp(argv[2], "eth-sts"))
            {
                eth_show_statistics();
            }
            */
            #endif
        }
        #ifdef HAVE_ONU_8021X
        else if(!strcmp(argv[1], "1xen"))
        {
            cs_uint32 value = atoi(argv[2]);
            if(argc <= 2)
            {
                cs_printf("app 1xen - invalid parameter\n");
		  return 1;
            }
            else if(value>1)
            {
                cs_printf("app 1xen - invalid parameter,should be 0 or 1\n");
		  return 1;
            }
            app_8021x_enable(value);
        }
        else if(!strcmp(argv[1], "1xst"))
        {
            if(argc <= 3)
            {
                cs_printf("app 1xht - invalid parameter\n");
            }
            else
            {
                app_8021x_set_start_param(atoi(argv[2]), atoi(argv[3]));
            }
        }
        else if(!strcmp(argv[1], "1xht"))
        {
            if(argc <= 2)
            {
                cs_printf("app 1xht - invalid parameter\n");
            }
            else
            {
                app_8021x_set_held_period(atoi(argv[2]),sizeof(cs_uint8));
            }
        }
        else if(!strcmp(argv[1], "1xrsd"))
        {
            if(argc <= 2)
            {
                cs_printf("app 1xrsd - invalid parameter\n");
            }
            else
            {
                app_8021x_set_delay(atoi(argv[2]));
            }
        }
        else if(!strcmp(argv[1], "uid"))
        {
            int len = strlen(argv[2]);
            if(argc <= 2)
            {
                char uid[1] = {0};
                app_8021x_set_user_name(uid, 1);
            }
            else
            {
			app_8021x_set_user_name(argv[2], len>USER_NAME_LEN?USER_NAME_LEN:len);
            }
        }
#ifdef HAVE_VOIP
        else if(!strcmp(argv[1], "slic"))
        {
            extern int voip_cmd_handle(int argc , char **argv);
            voip_cmd_handle(argc,argv);
        }
#endif
        else if(!strcmp(argv[1], "pwd"))
        {
            int len = strlen(argv[2]);
            if(argc <= 2)
            {
                char uid[1] = {0};
                app_8021x_set_user_pwd(uid, 1);
            }
            else
            {
                app_8021x_set_user_pwd(argv[2], len>PASSWORD_LEN?PASSWORD_LEN:len);
            }
        }
        #endif
#ifdef HAVE_ONU_RSTP
        else if(!strcmp(argv[1], "rstp"))
        {
            if(!strcmp(argv[2], "show"))
                {
                    stp_bridge_state_show();
                }
            else if(!strcmp(argv[2], "shps"))
                {
                    stp_port_state_show(atoi(argv[3]));
                }
            else if(!strcmp(argv[2], "debug"))
                {
                    stp_set_port_debug(atoi(argv[3]), atoi(argv[4]));
                }
            else if(!strcmp(argv[2], "setb"))
                {
                    cs_onu_stp_config_t   bridge_config;
                    cs_uint16 index = 3;
                    bridge_config.mask = 0x7f;
                    
                    bridge_config.enable = atoi(argv[index++]);
                    bridge_config.bridge_priority = atoi(argv[index++]);
                    bridge_config.max_age = atoi(argv[index++]);
                    bridge_config.hello_time = atoi(argv[index++]);
                    bridge_config.forward_delay = atoi(argv[index++]);
                    bridge_config.force_version = atoi(argv[index++]);
                    bridge_config.hold_time = atoi(argv[index]);
                    
                    app_stp_bridge_cfg_set(&bridge_config);
                }
            else if(!strcmp(argv[2], "setp"))
                {
                    cs_onu_port_stp_config_t  port_config;
                    cs_uint8 index = 3;

                    memset(&port_config, 0, sizeof(cs_onu_port_stp_config_t));
                    port_config.mask=0x1f;
                    port_config.port=atoi(argv[index++]);
                    port_config.admin_non_stp=atoi(argv[index++]);
                    port_config.port_priority=atoi(argv[index++]);
                    port_config.admin_pcost=atoi(argv[index++]);
                    port_config.admin_p2p=atoi(argv[index++]);
                    port_config.admin_edge=atoi(argv[index++]);
        
                    app_stp_port_cfg_set(atoi(argv[3]), &port_config);
                }
            else if(!strcmp(argv[2], "shpc"))
                {
                    cs_onu_port_stp_config_t  port_cfg;

                    memset(&port_cfg, 0, sizeof(cs_onu_port_stp_config_t));
                    app_stp_port_cfg_get(atoi(argv[3]), &port_cfg);
                    
                    cs_printf("\r\n port=%d, mask=0x%x, priority=%d, pcost=%d, p2p=%d, edge=%d, admin_non_stp =%d\r\n"
                                                     ,port_cfg.port 
                                                     ,port_cfg.mask 
                                                     ,port_cfg.port_priority
                                                     ,port_cfg.admin_pcost
                                                     ,port_cfg.admin_p2p
                                                     ,port_cfg.admin_edge
                                                     ,port_cfg.admin_non_stp);
                }
            else if(!strcmp(argv[2], "shbc"))
                {
                    cs_onu_stp_config_t   bridge_config;
                    cs_status retCode = CS_E_OK;

                    memset(&bridge_config, 0, sizeof(cs_onu_stp_config_t));
                    retCode = app_stp_bridge_cfg_get(&bridge_config);
                    cs_printf("\r\n mask=0x%x, stp_enable=%d, bridge_priority=%d, max_age=%d, hello_time=%d, forward_delay=%d, force_version =%d hold_time =%d\r\n"
                                                            ,bridge_config.mask
                                                            ,bridge_config.enable
                                                            ,bridge_config.bridge_priority
                                                            ,bridge_config.max_age
                                                            ,bridge_config.hello_time
                                                            ,bridge_config.forward_delay
                                                            ,bridge_config.force_version
                                                            ,bridge_config.hold_time);
                     cs_printf("\r\n retCode =%d\r\n", retCode);
                }
        }
#endif
        else if(!strcmp(argv[1], "vlan"))
        {
            vlan_cmd_proc( argc , argv);
        }
        
#ifdef HAVE_ZTE_SIJIE
        else if(!strcmp(argv[1], "manage_vlan"))
        {

            manage_vlan_cmd_proc( argc , argv);
        }
#endif  /*END_HAVE_ZTE_SIJIE*/
#ifdef HAVE_CTC_OAM
        else if(!strcmp(argv[1], "ps"))
        {
            if(!strcmp(argv[2], "show"))
            {
                pwrs_show();  
            }
            else if(!strcmp(argv[2], "ctrl"))
            {
                if(argc < 7)
                {
                    cs_printf("app ps ctrl [sleep_dur] [wake_dur] [sleep_flag] [sleep_mode]  - power saving control\n");
                }
                else
                {
                    oam_pwrs_ctrl_t pwrs_ctrl;
                    pwrs_ctrl.sleep_duration = atoi(argv[3]);
                    pwrs_ctrl.wait_duration = atoi(argv[4]);
                    pwrs_ctrl.sleep_flag = atoi(argv[5]);
                    pwrs_ctrl.sleep_mode = atoi(argv[6]);

                    pwrs_event(PWRS_MSG_EOAM_CONTROL, (cs_uint8 *)&pwrs_ctrl, sizeof(pwrs_ctrl));
                }
            }
            else if(!strcmp(argv[2], "cfg"))
            {
                if(argc < 5)
                {
                    cs_printf("app ps cfg [early_wakeup] [max_sleep_dur] - power saving config\n");
                }
                else
                {
                    oam_pwrs_config_t pwrs_cfg;
                    pwrs_cfg.early_wakeup = atoi(argv[3]);
                    pwrs_cfg.max_sleep_duration = atoll(argv[4]);

                    pwrs_event(PWRS_MSG_EOAM_CFG, (cs_uint8 *)&pwrs_cfg, sizeof(pwrs_cfg));
                }
            }
        }
#endif
        else if(!strcmp(argv[1], "capture"))
        {
            if(argc != 3)
            {
                cs_printf("app capture <enable> - packet capture to cpu enable/disable \n");
            }
            else
            {
                cs_callback_context_t     context;
                cs_boolean enable;
                
                enable = (cs_boolean)iros_strtol(argv[2]);
                if(enable)
                {
                    cs_log_remote_level_set(IROS_LOG_LEVEL_DBG0, IROS_MID_CAPTURE); 
                }
                else
                {
                    cs_log_remote_level_set(IROS_LOG_LEVEL_CRI, IROS_MID_CAPTURE); 
                }
                ret = epon_request_onu_pkt_remote_capture_set(context, 0, 0, enable);
                if(ret != CS_E_OK){
                    cs_printf("epon_request_onu_pkt_remote_capture_set failed! \n");    
                    return cs_status_2_cmd_rc_map(ret);       
                }

                return cs_status_2_cmd_rc_map(ret);
            }
        }
#ifdef HAVE_DB_MANAGMENT
		//added by zhuxh,add db cmd
        else if(!strcmp(argv[1], "db"))
        {
             if(!strcmp(argv[2], "save")){
                db_save_to_flash();
             }
             else if(!strcmp(argv[2], "reset")){
                db_reset_to_factory();
             }else{
                app_cmd_help();
             }
        }
#endif		
        else
        {
            app_cmd_help();
        }
    }
    return SAL_CMD_OK;
}

