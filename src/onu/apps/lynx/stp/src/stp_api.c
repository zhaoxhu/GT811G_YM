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
#ifdef HAVE_ONU_RSTP

#include "base.h"
#include "stp_in.h"
#include "stp_to.h"
#include "stp_api.h"
#include "stp_sdl_adapt.h"
#include "sdl_l2.h"

cs_status app_stp_bridge_cfg_set(
        CS_IN cs_onu_stp_config_t * bridge_cfg)  
{
        cs_uint32     mask;
        cs_uint32    bridge_priority = 0;
        cs_uint32    max_age = 0;
        cs_uint32    hello_time = 0;
        cs_uint32    forward_delay = 0;
        cs_uint32    force_version = 0;
        cs_uint32    hold_time = 0;
        cs_uint32    ret = CS_E_OK;
        cs_uint64   uid_cfg_mask = 0;
        UID_STP_CFG_T   uid_cfg;        
        cs_uint32   action = 0;
        cs_callback_context_t context;
        
        if ( bridge_cfg == NULL) {
                goto END;
        } 

        memset( &uid_cfg, 0 ,sizeof(UID_STP_CFG_T ));
        mask = bridge_cfg->mask;
        bridge_priority = bridge_cfg->bridge_priority;
        max_age = bridge_cfg->max_age;
        hello_time = bridge_cfg->hello_time;
        forward_delay = bridge_cfg->forward_delay;
        force_version = bridge_cfg->force_version;
        hold_time = bridge_cfg->hold_time;

        STP_LOG("\r\n mask=%x, enable=%d, priority=%d, max_age=%d, hello_time=%d, forward_delay=%d, force_version =%d, hold_time =%d\r\n"
                                         ,mask 
                                         ,bridge_cfg->enable
                                         ,bridge_priority
                                         ,max_age
                                         ,hello_time
                                         ,forward_delay
                                         ,force_version
                                         ,hold_time);
        
        if (mask & ONU_MASK_STP_BRIDGE_ENABLE) {
                cs_onu_stp_config_t old_brg_cfg;

                memset(&old_brg_cfg, 0, sizeof(old_brg_cfg));
                app_stp_bridge_cfg_get(&old_brg_cfg);
                
                if(old_brg_cfg.enable == bridge_cfg->enable){
                    goto END;
                }
            
                uid_cfg_mask |= BR_CFG_STATE;
                uid_cfg.stp_enabled = bridge_cfg->enable;

                if ( uid_cfg.stp_enabled == STP_DISABLED) {
                        action = 0;
                }else {
                        action = 1;
                }
                
                /*make received BPDU in port  to be forwarded to CPU*/	
                epon_request_onu_stp_ctrl_set(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 0, action);
        
        }
        
        if (mask & ONU_MASK_STP_BRIDGE_PRIORITY) {
                if (bridge_priority > MAX_BR_PRIO ){
                        goto END;
                }
                uid_cfg_mask |= BR_CFG_PRIO;
                uid_cfg.bridge_priority = bridge_priority;
        }

        if (mask & ONU_MASK_STP_BRIDGE_MAX_AGE) {
                if (  (max_age < MIN_BR_MAXAGE )
                        || ( max_age > MAX_BR_MAXAGE) ) {
                        goto END;
                }
                uid_cfg_mask |= BR_CFG_AGE;
                uid_cfg.max_age = max_age;             
        }
        
        if (mask & ONU_MASK_STP_BRIDGE_HELLO_TIME) {
                if ( (hello_time < MIN_BR_HELLOT )
                        || hello_time > MAX_BR_HELLOT ) {
                        goto END;
                }
                uid_cfg_mask |= BR_CFG_HELLO;
                uid_cfg.hello_time = hello_time;  
        }

        if (mask & ONU_MASK_STP_BRIDGE_FORWARD_DELAY) {
                if ( ( forward_delay < MIN_BR_FWDELAY)
                        || ( forward_delay > MAX_BR_FWDELAY ) ) {
                        goto END;
                }
                uid_cfg_mask |= BR_CFG_DELAY;
                uid_cfg.forward_delay = forward_delay;          
        }

        if (mask & ONU_MASK_STP_BRIDGE_FORCE_VERSION) {
                uid_cfg_mask |= BR_CFG_FORCE_VER;
                uid_cfg.force_version = force_version;   
        }
        
        uid_cfg.field_mask = uid_cfg_mask;
        ret = STP_IN_stpm_set_cfg (STP_DEF_VLAN, NULL, &uid_cfg);
        
END:
        return ret;
        
 }

cs_status app_stp_bridge_cfg_get(
        CS_OUT        cs_onu_stp_config_t   *  bridge_cfg )  
{
        UID_STP_STATE_T uid_state;
        cs_uint32    rc = 0;
        UID_STP_CFG_T   uid_cfg;

        if(bridge_cfg == NULL)
            return CS_E_ERROR;
        
        memset(&uid_cfg, 0 ,sizeof(UID_STP_CFG_T ) );
        memset(&uid_state, 0, sizeof(UID_STP_STATE_T));

        rc = STP_IN_stpm_get_state (STP_DEF_VLAN, &uid_state);
        if (rc) {
                STP_LOG ("can't get rstp bridge state: %s\n", STP_IN_get_error_explanation (rc));
                return CS_E_ERROR;
        }
        
        rc = STP_IN_stpm_get_cfg (STP_DEF_VLAN, &uid_cfg);
        if (rc) {
                STP_LOG ("can't get rstp bridge configuration: %s\n", STP_IN_get_error_explanation (rc));
                return CS_E_ERROR;
        }

        uid_cfg.stp_enabled = uid_state.stp_enabled;
        uid_cfg.bridge_priority = uid_state.bridge_id.prio;

        bridge_cfg->enable = uid_cfg.stp_enabled;
        bridge_cfg->bridge_priority = uid_cfg.bridge_priority;
        bridge_cfg->max_age = uid_cfg.max_age;
        bridge_cfg->hello_time = uid_cfg.hello_time;
        bridge_cfg->forward_delay = uid_cfg.forward_delay;
        bridge_cfg->force_version = uid_cfg.force_version;
        bridge_cfg->hold_time = uid_cfg.hold_time;
        
        return  CS_E_OK;

}

cs_status app_stp_bridge_state_get(
        CS_OUT        cs_onu_stp_state_config_t * bridge_state )  
{
        UID_STP_STATE_T  entry;
        UID_STP_CFG_T     uid_cfg;
        
        if(bridge_state == NULL)
            return CS_E_ERROR;

        memset( &entry, 0 ,sizeof(UID_STP_STATE_T));
        memset( &uid_cfg, 0 ,sizeof(UID_STP_CFG_T));

        STP_IN_stpm_get_state(STP_DEF_VLAN , &entry );
        STP_IN_stpm_get_cfg (STP_DEF_VLAN, &uid_cfg);
        bridge_state->stp_enable = entry.stp_enabled;
        bridge_state->stp_mode = uid_cfg.force_version;
        bridge_state->bridge_prio = entry.bridge_id.prio;
        memcpy(bridge_state->bridge_addr, entry.bridge_id.addr, 6);
        bridge_state->root_bridge_prio = entry.designated_root.prio;
        memcpy(bridge_state->root_bridge_addr, entry.designated_root.addr, 6);
        bridge_state->max_age = entry.max_age;
        bridge_state->hello_time = entry.hello_time;
        bridge_state->forward_delay = entry.forward_delay;
        bridge_state->root_path_cost = entry.root_path_cost; 
        bridge_state->root_port = entry.root_port;

        return CS_E_OK;
}

cs_status app_stp_port_cfg_set(
        CS_IN cs_port_id_t   port,
        CS_IN cs_onu_port_stp_config_t *port_cfg )  
{
        cs_uint32     mask=0;
        cs_uint32    port_priority = 0;
        cs_int64    admin_pcost = 0;
        cs_uint8    admin_p2p = 0;
        cs_uint8    admin_edge = 0;
        cs_uint8    admin_non_stp = 0;
        cs_uint32    ret = CS_E_ERROR;
        UID_STP_PORT_CFG_T      uid_cfg;
        cs_uint64       field_mask = 0;

        if(port_cfg == NULL)
            return CS_E_ERROR;
        
        memset(&uid_cfg, 0 ,sizeof(UID_STP_PORT_CFG_T));
        mask = port_cfg->mask;
        port_priority = port_cfg->port_priority;
        admin_pcost = port_cfg->admin_pcost;
        admin_p2p = port_cfg->admin_p2p;
        admin_edge = port_cfg->admin_edge;
        admin_non_stp = port_cfg->admin_non_stp;

        STP_LOG("\r\n port=%d, mask=%x, priority=%d, pcost=%d, p2p=%d, edge=%d, admin_non_stp =%d\r\n"
                                         ,port 
                                         ,mask 
                                         ,port_priority
                                         ,admin_pcost
                                         ,admin_p2p
                                         ,admin_edge
                                         ,admin_non_stp);

        if (port == CS_PON_PORT_ID) {
                STP_LOG("\r\n input port(%x)  error! \r\n", port);
                goto END;
        }

        if (mask & ONU_MASK_PORT_STP_PRIORITY) {
                if (port_priority > MAX_PORT_PRIO) {
                        goto   END;
                }
            field_mask |= PT_CFG_PRIO;
            uid_cfg.port_priority = port_priority;
        }

        if (mask & ONU_MASK_PORT_STP_PATH_COST) {
                if ( ( admin_pcost < 0)
                        || (admin_pcost > 20000000 )) {
                        goto END;
                }
              field_mask |= PT_CFG_COST;
              uid_cfg.admin_port_path_cost = admin_pcost;
        }

        if (mask & ONU_MASK_PORT_STP_POINT2POINT) {
                if (  (admin_p2p > 2 ) ) {
                        goto END;
                }
                field_mask |= PT_CFG_P2P;             
                uid_cfg.admin_point2point = admin_p2p;
        }

        if (mask & ONU_MASK_PORT_STP_EDGE) {
                if (  (admin_edge > 1 ) ) {
                        goto END;
                }
              field_mask |= PT_CFG_EDGE;             
              uid_cfg.admin_edge = admin_edge;
        }

        if (mask & ONU_MASK_PORT_STP_NON_STP) {
                if (  (admin_non_stp > 1 ) ) {
                        goto END;
                }
                field_mask |= PT_CFG_NON_STP;
                uid_cfg.admin_non_stp = admin_non_stp;
        }
        
        BitmapClear(&uid_cfg.port_bmp);
        BitmapSetBit(&uid_cfg.port_bmp, port);
        uid_cfg.field_mask = field_mask;
        ret = STP_IN_set_port_cfg(STP_DEF_VLAN, &uid_cfg);
       

END:
        return ret;
}

cs_status app_stp_port_cfg_get(
        CS_IN           cs_port_id_t   port,
        CS_OUT        cs_onu_port_stp_config_t *port_cfg)  
{
        UID_STP_PORT_CFG_T      uid_cfg;

        if(port_cfg == NULL)
            return CS_E_ERROR;
        
        if (port == CS_PON_PORT_ID) {
                STP_LOG("\r\n input port(%x)  error! \r\n", port);
                return CS_E_ERROR;
        }
        
        memset( &uid_cfg, 0, sizeof( UID_STP_PORT_CFG_T));
        STP_IN_port_get_cfg( STP_DEF_VLAN , port, &uid_cfg);
        port_cfg->port = port;
        port_cfg->port_priority = uid_cfg.port_priority;
        port_cfg->admin_pcost = uid_cfg.admin_port_path_cost;
        port_cfg->admin_p2p = uid_cfg.admin_point2point;
        port_cfg->admin_edge = uid_cfg.admin_edge;
        port_cfg->admin_non_stp = uid_cfg.admin_non_stp;
        
        return CS_E_OK;
        
}

cs_status app_stp_port_state_get(
        CS_IN           cs_port_id_t   port ,
        CS_OUT        cs_onu_port_stp_state_config_t * port_state)  
{
        UID_STP_PORT_STATE_T    entry;
        UID_STP_PORT_CFG_T      uid_cfg;
        cs_uint32                    ret = CS_E_ERROR;
        cs_sdl_stp_state_t         stp_state;       
        cs_callback_context_t context;

        if(port_state == NULL)
            return ret;

        memset(&entry, 0, sizeof(UID_STP_PORT_STATE_T ));
        memset(&uid_cfg, 0 , sizeof( UID_STP_PORT_CFG_T));
        
        if (port == CS_PON_PORT_ID) {
                STP_LOG("\r\n input port(%x)  error! \r\n", port);
                goto END;
        }

        entry.port_no = port;
        
        ret = STP_IN_port_get_state(STP_DEF_VLAN, &entry);
        if (ret != 0){
                goto END;
        }
        
        ret = STP_IN_port_get_cfg (STP_DEF_VLAN, entry.port_no , &uid_cfg);
        if (ret != 0){
                goto END;
        }

        ret = epon_request_onu_stp_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
        port, &stp_state);
        if (ret!=0){
                cs_printf("port %d stp get error\n", port);
                goto END;
        }

        port_state->state = stp_state;

        port_state->stp_enable = (uid_cfg.admin_non_stp==0)?1:0;
        port_state->role = entry.role;
        port_state->path_cost = entry.path_cost;
        port_state->port_prio = uid_cfg.port_priority;
        port_state->designated_bridge_prio = entry.designated_bridge.prio;	
        memcpy(port_state->designated_bridge_addr, entry.designated_bridge.addr, 6);
        port_state->oper_point2point = entry.oper_point2point;
        port_state->oper_edge = entry.oper_edge;
        port_state->rx_cfg_bpdu_cnt = entry.rx_cfg_bpdu_cnt;
        port_state->rx_rstp_bpdu_cnt = entry.rx_rstp_bpdu_cnt;
        port_state->rx_tcn_bpdu_cnt = entry.rx_tcn_bpdu_cnt;
        port_state->tx_cfg_bpdu_cnt = entry.tx_cfg_bpdu_cnt;
        port_state->tx_rstp_bpdu_cnt = entry.tx_rstp_bpdu_cnt;
        port_state->tx_tcn_bpdu_cnt = entry.tx_tcn_bpdu_cnt;	
        ret=CS_E_OK;

END:
        return ret;

}

#endif
