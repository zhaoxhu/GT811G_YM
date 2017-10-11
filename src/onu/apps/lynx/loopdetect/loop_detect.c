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

DEFINITIONS:  "DEVICE" means the Cortina Systems?Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems?SDK software.

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
#ifdef HAVE_LOOP_DETECT
#include "plat_common.h"
#include "sdl_api.h"
#include "sdl_l2.h"
#include "app_vlan.h"
#include "frame.h"
#include "packet.h"
#include "loop_detect.h"

static loop_detect_cb_t g_ldCb;
static cs_uint8 g_ldTrace = 0;
static cs_uint32 g_ld_msgq = 0;

static cs_uint8* block_string[] = {"global", "port based"};
static cs_uint8* unblock_string[] = {"expired-unblock", "tx-unblock"};
static cs_uint8* clear_string[] = {"next-round", "immediate"};

cs_status loop_detect_parser(cs_pkt_t *pPkt);
                               
#if 1
							   //added by zhuxh
							   extern cs_status set_loop_alarm_config(
									   cs_port_id_t port_id,
									   cs_boolean enable);
#endif

void loop_detect_interval_set(cs_uint32 interval)
{
    if(g_ldCb.interval_timer.timer_value != interval)
    {
        g_ldCb.interval_timer.timer_value = interval;
        if(g_ldCb.enable && g_ldCb.interval_timer.timer_handler != 0)
        {
            cs_timer_del(g_ldCb.interval_timer.timer_handler);
            g_ldCb.interval_timer.timer_handler = cs_msg_circle_timer_add(g_ld_msgq, interval, 
                                                                    (void*)&(g_ldCb.interval_timer));
        }
        
    }
}

cs_uint32 loop_detect_interval_get()
{
    return g_ldCb.interval_timer.timer_value;
}

cs_status loop_clear_check_time_set(cs_port_id_t port, cs_uint32 time)
{
    cs_uint8 i = 0;
    LOOP_DETECT_PORT_CHECK(port);

    for(i = 0; i < LOOP_DETECT_PORT_MAX; i++)
    {
        if(port == CS_ALL_UNI_PORT_ID || port == i+1)
        {
            g_ldCb.detect_ports[i].clear_timer.timer_value = time;
        }
    }

    return CS_E_OK;
}

cs_status loop_detect_block_time_set(cs_port_id_t port, cs_uint32 time)
{ 
    cs_uint8 i = 0;

    LOOP_DETECT_PORT_CHECK(port);
    if(g_ldCb.block_obj == LOOP_DETECT_BLOCK_GLOBAL)
    {
        if(g_ldCb.block_timer.timer_value != time)
        {
            g_ldCb.block_timer.timer_value = time;
            /*check whether need restart*/
            if(g_ldCb.block_state && g_ldCb.block_timer.timer_handler != 0)
            {
               
                cs_timer_del(g_ldCb.block_timer.timer_handler);       
                g_ldCb.block_timer.timer_handler = cs_msg_timer_add(g_ld_msgq, time, 
                                                                                        (void*)&(g_ldCb.block_timer), NULL);
            }
        }
    }
    else if(g_ldCb.block_obj == LOOP_DETECT_BLOCK_PORT_BASE)
    {
        for(i = 0; i < LOOP_DETECT_PORT_MAX; i++)
        {
            if(port == CS_ALL_UNI_PORT_ID || port == i+1)
            {
                /*set current port time*/
                if(g_ldCb.detect_ports[i].block_timer.timer_value != time)
                {
                    g_ldCb.detect_ports[i].block_timer.timer_value = time;
                    /*check whether need restart*/
                    if(g_ldCb.detect_ports[i].block_state && g_ldCb.detect_ports[i].block_timer.timer_handler != 0)
                    {
                        cs_timer_del(g_ldCb.detect_ports[i].block_timer.timer_handler);   
                        g_ldCb.detect_ports[i].block_timer.timer_handler = cs_msg_timer_add(g_ld_msgq, time, 
                                                                                                       (void*)&(g_ldCb.detect_ports[i].block_timer), NULL);
                    }
                }
            }
        }
    }

    return CS_E_OK;
}

cs_uint32 loop_detect_block_time_get(cs_port_id_t port)
{
    LOOP_DETECT_PORT_CHECK(port)
        
    if(g_ldCb.block_obj == LOOP_DETECT_BLOCK_GLOBAL)
    {
        return g_ldCb.block_timer.timer_value;
    }
    else
    {
        if(port != CS_ALL_UNI_PORT_ID)
        {
            return g_ldCb.detect_ports[port-1].block_timer.timer_value;
        }
        else
        {
            return g_ldCb.detect_ports[0].block_timer.timer_value;
        }
    }
}

cs_status loop_detect_oper_get(cs_port_id_t port, cs_boolean *oper)
{
    LOOP_DETECT_PORT_CHECK(port);

    if(!oper)
        return CS_E_PARAM;

    /*get port status:detect enable/disable*/
    if(port == CS_ALL_UNI_PORT_ID)
    {
        *oper = g_ldCb.enable;
    }
    else
    {
        *oper = g_ldCb.detect_ports[port-1].enable;
    }
    return CS_E_OK;  
}

cs_status loop_detect_status_get(cs_port_id_t port, cs_uint8 *status)
{
    if(port < CS_UNI_PORT_ID1 || port > LOOP_DETECT_PORT_MAX)
    {
        return CS_E_PARAM;
    }

    *status = g_ldCb.detect_ports[port-1].status;
    return CS_E_OK;
}

cs_boolean loop_detect_block_state_get(cs_port_id_t port)
{

    if(g_ldCb.block_obj == LOOP_DETECT_BLOCK_GLOBAL)
    {
        return g_ldCb.block_state;
    }
    else
    {
        if(port < CS_UNI_PORT_ID1 || port > LOOP_DETECT_PORT_MAX)
        {
            return FALSE;
        }
        return g_ldCb.detect_ports[port-1].block_state;
    }
}

cs_status loop_detect_unblock(cs_port_id_t port)
{
    cs_status result = CS_E_OK;
    cs_callback_context_t     context;

    if(g_ldCb.block_obj == LOOP_DETECT_BLOCK_GLOBAL)
    {
        if(g_ldCb.block_state == LOOP_DETECT_BLOCKED)
        {
            g_ldCb.block_state = LOOP_DETECT_NOT_BLOCK;
            cs_timer_del(g_ldCb.block_timer.timer_handler);
            g_ldCb.block_timer.timer_handler = 0;

            result = epon_request_onu_loopdetect_block(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
                                                                               CS_ALL_UNI_PORT_ID, 0);

        }
    }
    else
    {
        cs_uint8 i = 0;
        
        LOOP_DETECT_PORT_CHECK(port);
        for(i = 0; i < LOOP_DETECT_PORT_MAX; i++)
        {
            if((i == port-1) || (CS_ALL_UNI_PORT_ID == port))
            {
                if(g_ldCb.detect_ports[i].block_state == LOOP_DETECT_BLOCKED)
                {
                    g_ldCb.detect_ports[i].block_state = LOOP_DETECT_NOT_BLOCK;
                    if(g_ldCb.detect_ports[i].block_timer.timer_handler != 0)
                        cs_timer_del(g_ldCb.detect_ports[i].block_timer.timer_handler);
                    g_ldCb.detect_ports[i].block_timer.timer_handler = 0;
                    /*open port*/
                    result = epon_request_onu_loopdetect_block(context, ONU_DEVICEID_FOR_API, 
                                                                                       ONU_LLIDPORT_FOR_API, i+1, 0);
                }
            }
        }
    }
    return result;
}

void loop_detect_block_enable(cs_boolean enable)
{
    g_ldCb.need_block = enable;
}

cs_status loop_detect_port_block_enable(cs_port_id_t port, cs_boolean enable)
{
    cs_uint8 i = 0;
    LOOP_DETECT_PORT_CHECK(port);

    for(i = 0; i < LOOP_DETECT_PORT_MAX; i++)
    {
        if(i == port-1 || port == CS_ALL_UNI_PORT_ID)
        {
            g_ldCb.detect_ports[i].need_block = enable;
        }
    }
    return CS_E_OK;
}

void loop_detect_unblock_policy_set(loop_detect_unblock_policy_t policy)
{
    g_ldCb.unblock_policy = policy;
}

loop_detect_unblock_policy_t loop_detect_unblock_policy_get()
{
    return g_ldCb.unblock_policy;
}

void loop_detect_block_object_set(loop_detect_block_object_t obj)
{
    g_ldCb.block_obj = obj;
}

loop_detect_block_object_t loop_detect_block_object_get()
{
    return g_ldCb.block_obj;
}

void loop_clear_policy_set(loop_clear_policy_t policy)
{
    g_ldCb.clear_policy = policy;
}

loop_clear_policy_t loop_clear_policy_get()
{
    return g_ldCb.clear_policy;
}

void loop_detect_init()
{
    cs_uint8 i = 0;
    
    memset(&g_ldCb, 0, sizeof(g_ldCb));
    
    g_ldCb.enable = LOOP_DETECT_DISABLE;

    /*set default timer*/
    g_ldCb.interval_timer.timer_value = LOOP_DETECT_SEND_INTERVAL_DEFAULT;
    g_ldCb.interval_timer.type.msg.msg_type = IROS_MSG_TIME_OUT;
    g_ldCb.interval_timer.type.msg.timer_type = APP_TIMER_LOOPDETECT;
    g_ldCb.interval_timer.type.name = LOOP_DETECT_PERIOD_TIMER;
    g_ldCb.interval_timer.port = CS_ALL_UNI_PORT_ID;
    
    g_ldCb.block_timer.timer_value = LOOP_DETECT_BLOCK_DEFAULT;
    g_ldCb.block_timer.type.msg.msg_type = IROS_MSG_TIME_OUT;
    g_ldCb.block_timer.type.msg.timer_type = APP_TIMER_LOOPDETECT;
    g_ldCb.block_timer.type.name = LOOP_DETECT_BLOCK_TIMER;
    g_ldCb.block_timer.port = CS_ALL_UNI_PORT_ID;
    
    g_ldCb.need_block = LOOP_DETECT_BLOCK_ENABLE;
    g_ldCb.block_state = LOOP_DETECT_NOT_BLOCK;

    if(CS_E_OK != startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &(g_ldCb.max_port)))
    {
        g_ldCb.max_port = 1;
    }
    g_ldCb.detect_ports = (loop_detect_port_check_t*)iros_malloc(IROS_MID_APP, 
                                                                               (g_ldCb.max_port) * sizeof(loop_detect_port_check_t));
    if(!(g_ldCb.detect_ports))
    {
        IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ, IROS_MID_APP, "malloc fail\n");
        return;
    }
    memset(g_ldCb.detect_ports, 0, (g_ldCb.max_port) * sizeof(loop_detect_port_check_t));
    for(i = 0; i < LOOP_DETECT_PORT_MAX; i++)
    {
        g_ldCb.detect_ports[i].port = i+1;
        g_ldCb.detect_ports[i].enable = LOOP_DETECT_DISABLE;
        g_ldCb.detect_ports[i].status = NO_LOOP;
        g_ldCb.detect_ports[i].block_state = LOOP_DETECT_NOT_BLOCK;
        g_ldCb.detect_ports[i].need_block = LOOP_DETECT_BLOCK_DISABLE;
        
        g_ldCb.detect_ports[i].block_timer.port = i+1;
        g_ldCb.detect_ports[i].block_timer.timer_value = LOOP_DETECT_BLOCK_DEFAULT;
        g_ldCb.detect_ports[i].block_timer.type.msg.msg_type = IROS_MSG_TIME_OUT;
        g_ldCb.detect_ports[i].block_timer.type.msg.timer_type = APP_TIMER_LOOPDETECT;
        g_ldCb.detect_ports[i].block_timer.type.name = LOOP_DETECT_BLOCK_TIMER;

        g_ldCb.detect_ports[i].clear_timer.port = i+1;
        g_ldCb.detect_ports[i].clear_timer.timer_value = LOOP_DETECT_CLEAR_CHECK_DEFAULT;
        g_ldCb.detect_ports[i].clear_timer.type.msg.msg_type = IROS_MSG_TIME_OUT;
        g_ldCb.detect_ports[i].clear_timer.type.msg.timer_type = APP_TIMER_LOOPDETECT;
        g_ldCb.detect_ports[i].clear_timer.type.name = LOOP_DETECT_CLEAR_CHECK_TIMER;
    }

    g_ld_msgq = app_msg_q;

    app_pkt_reg_handler(CS_PKT_LOOPDETECT, loop_detect_frame_handle);
    app_pkt_reg_parser(CS_PKT_LOOPDETECT, loop_detect_parser);
}

void loop_detect_tmfunc_interval_check(void)
{
    loop_detect_event_handler(CS_ALL_UNI_PORT_ID, LOOPDETECT_TIMER_EXPIRED_EVENT);
}

void loop_detect_tmfunc_block(cs_port_id_t port)
{
    loop_detect_event_handler(port, UNBLOCK_TIMER_EXPIRED_EVENT);
}

void loop_detect_tmfunc_clear_check(cs_port_id_t port)
{
    loop_detect_event_handler(port, CLEAR_CHECK_TIMER_EXPIRED_EVENT);
}

void loop_detect_timer_handle(app_timer_msg_t *msg)
{
    if(!msg)
        return;
    
    loop_detect_timer_t *ptimer = (loop_detect_timer_t*)msg;
    switch(ptimer->type.name)
    {
        case LOOP_DETECT_PERIOD_TIMER:
            loop_detect_tmfunc_interval_check();
            break;
            
        case LOOP_DETECT_BLOCK_TIMER:
            loop_detect_tmfunc_block(ptimer->port);
            break;
            
        case LOOP_DETECT_CLEAR_CHECK_TIMER:
            loop_detect_tmfunc_clear_check(ptimer->port);
            break;
            
        default:
            break;
    }
}

cs_status loop_detect_period_common_handle(cs_port_id_t port)
{
    cs_uint8 macaddr[CS_MACADDR_LEN] = {0};
    cs_sdl_port_admin_t port_admin_status;
    cs_callback_context_t     context;
    cs_uint8 i =0;
    cs_status result = CS_E_OK;
    
    loop_detect_ether_header_t frame;
  
    memset(&frame, 0, sizeof(frame));


    epon_request_onu_pon_mac_addr_get(context, ONU_DEVICEID_FOR_API, 
                                                               ONU_LLIDPORT_FOR_API, (cs_mac_t *)macaddr);
    /*fill frame header*/
    memset(frame.dst, 0xff, CS_MACADDR_LEN);
    memcpy(frame.src, macaddr, CS_MACADDR_LEN);
    frame.ethertype = htons(EPON_ETHERTYPE_LOOP_DETECT);


    for(i = 0; i < LOOP_DETECT_PORT_MAX; i++)
    {
        if(g_ldCb.detect_ports[i].enable)
        {
            epon_request_onu_port_admin_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
                                                                  i+1, &port_admin_status);
            if (port_admin_status == SDL_PORT_ADMIN_UP)
            {
                if(g_ldCb.block_obj == LOOP_DETECT_BLOCK_GLOBAL)
                {
                    if(g_ldCb.unblock_policy == LOOP_DETECT_EXPIRE_UNBLOCK && g_ldCb.block_state)
                    {
                        return result;
                    }
                }
                else
                {
                    if(g_ldCb.unblock_policy == LOOP_DETECT_EXPIRE_UNBLOCK && 
                        g_ldCb.detect_ports[i].block_state)
                    {
                        continue;
                    }
                }
                /*open for this port*/
                epon_request_onu_loopdetect_block(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, i+1, 0);
                g_ldCb.detect_ports[i].block_state = LOOP_DETECT_NOT_BLOCK;
                IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, 
                                        "open port %d in period time out\n", i+1);
                
                frame.lb_port = htonl(i+1);
                DUMPLOOPPKT("ld send", (cs_uint8*)&frame, sizeof(frame));

                /*send frame with vlan*/
                vlan_flooding_per_port(i+1, ONU_CPU_PKT_PRIO, (cs_uint8*)&frame, sizeof(frame));

                if(g_ldCb.detect_ports[i].status == LOOP_DETECTED)
                {
                    IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP,  "need loop check \n");
                    if(g_ldCb.clear_policy == LOOP_CLEAR_IMMEDIATE_CHECK)
                    {
                        g_ldCb.detect_ports[i].status = LOOP_CHECK;
                        /*start  timer to check status*/
                        g_ldCb.detect_ports[i].clear_timer.timer_handler = cs_msg_timer_add(g_ld_msgq, g_ldCb.detect_ports[i].clear_timer.timer_value,
                                                                                               (void*)&(g_ldCb.detect_ports[i].clear_timer), NULL);
                    }
                    else if(g_ldCb.clear_policy == LOOP_CLEAR_NEXT_ROUND_CHECK)
                    {
                        g_ldCb.detect_ports[i].status = LOOP_CHECK;
                    }
                }
                else if(g_ldCb.detect_ports[i].status == LOOP_CHECK)
                {
                    /*prepare handle*/
                    LOOP_DETECT_CALL_BACK(i+1, LOOP_CLEARED_PREPARE_EVENT)
                    g_ldCb.detect_ports[i].status = NO_LOOP;
                    /*open this port anyway*/
                    result = epon_request_onu_loopdetect_block(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, i+1, 0);
                    /*after handler*/
                    LOOP_DETECT_CALL_BACK(i+1, LOOP_CLEARED_AFTER_EVENT);
                    IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP,  "Loop cleared \n");
                    
                }
            }
            
        }
    }

    return result;
    
}

cs_status loop_detect_special_handler_reg(loop_detect_event_handler_t special)
{
    g_ldCb.special_handler = special;
    
    return CS_E_OK;
}

cs_status loop_detect_enable_common_handle(cs_port_id_t port)
{
    cs_uint8 i = 0;
    cs_callback_context_t     context;

    LOOP_DETECT_PORT_CHECK(port);
    if(!g_ldCb.enable)
    {
        /*enable prepare handle*/
        LOOP_DETECT_CALL_BACK(port, LOOPDETECT_ENABLE_PREPARE_EVENT);
       
        g_ldCb.enable = LOOP_DETECT_ENABLE;
        
        g_ldCb.interval_timer.timer_handler = cs_msg_circle_timer_add(g_ld_msgq, g_ldCb.interval_timer.timer_value, 
                                                                    (void*)&(g_ldCb.interval_timer));

        epon_request_onu_loopdetect_set(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 1);

        /*enable after handle*/
        LOOP_DETECT_CALL_BACK(port, LOOPDETECT_ENABLE_AFTER_EVENT);
    }

    for(i = 0; i < LOOP_DETECT_PORT_MAX; i++)
    {
        if((i == port-1) || (CS_ALL_UNI_PORT_ID == port))
        {
            g_ldCb.detect_ports[i].enable = LOOP_DETECT_ENABLE;
			set_loop_alarm_config(port, true);
        }
    }

    return CS_E_OK;
}

cs_status loop_clear_check_common_handle(cs_port_id_t port)
{
    cs_callback_context_t     context;

    if(port < CS_UNI_PORT_ID1 || port > LOOP_DETECT_PORT_MAX)
        return CS_E_PARAM;
    
    g_ldCb.detect_ports[port-1].clear_timer.timer_handler = 0;
    if(g_ldCb.detect_ports[port-1].status == LOOP_CHECK)
    {
        /*loop cleared prepare handle*/
        LOOP_DETECT_CALL_BACK(port, LOOP_CLEARED_PREPARE_EVENT);
        
        /*open this port anyway*/
        epon_request_onu_loopdetect_block(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, 0);
        g_ldCb.detect_ports[port-1].status = NO_LOOP;

        /*loop cleared after handle*/
        LOOP_DETECT_CALL_BACK(port, LOOP_CLEARED_AFTER_EVENT);
        IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP,  "Loop cleared when  immediate check\n");
    }

    return CS_E_OK;
}

cs_status loop_detect_disable_common_handle(cs_port_id_t port)
{
    cs_uint8 i = 0;
    cs_uint8 exist = 0;
    cs_status result = CS_E_OK;
    cs_callback_context_t     context;
    
    LOOP_DETECT_PORT_CHECK(port);
    if(!g_ldCb.enable)
    {
        return CS_E_ERROR;
    }

    for(i = 0; i < LOOP_DETECT_PORT_MAX; i++)
    {
        if((i == port-1) || (CS_ALL_UNI_PORT_ID == port))
        {
            g_ldCb.detect_ports[i].enable = LOOP_DETECT_DISABLE;
			#if 1
			//added by zhuxh,when loop detect disable, up the port that loop detect module down and clear the alarm
			{
				if(LOOP_DETECTED == g_ldCb.detect_ports[i].status)
				{
					cs_callback_context_t context;
					epon_request_onu_port_admin_set(context, 0, 0, port, SDL_PORT_ADMIN_UP);

					set_loop_alarm_config(port, true);
					LOOP_DETECT_CALL_BACK(port, LOOP_CLEARED_AFTER_EVENT);
					set_loop_alarm_config(port, false);
				}
			}
			#endif
            g_ldCb.detect_ports[i].status = NO_LOOP;
        }
        if( g_ldCb.detect_ports[i].enable == LOOP_DETECT_ENABLE)
            exist = 1;
    }
    if(!exist)
    {
        /*disable prepare handle*/
        LOOP_DETECT_CALL_BACK(port, LOOPDETECT_DISABLE_PREPARE_EVENT);
        /*stop timer*/
        cs_timer_del(g_ldCb.interval_timer.timer_handler);
        g_ldCb.interval_timer.timer_handler = 0;
        g_ldCb.enable = LOOP_DETECT_DISABLE;

        epon_request_onu_loopdetect_set(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 0);        

        /*disable after handle*/
        LOOP_DETECT_CALL_BACK(port, LOOPDETECT_DISABLE_AFTER_EVENT);
    }
    return result;
}

cs_status loop_detect_enable(cs_port_id_t port)
{
    return loop_detect_event_handler(port, LOOPDETECT_ENABLE_EVENT);
}

cs_status loop_detect_disable(cs_port_id_t port)
{
    return loop_detect_event_handler(port, LOOPDETECT_DISABLE_EVENT);
}

cs_status loop_detected_common_handle(cs_port_id_t port)
{
    cs_callback_context_t     context;
    cs_status result = CS_E_OK;
    cs_boolean need_block = FALSE;
    
    if(!g_ldCb.enable)
        return CS_E_OK;
    
    if(port < CS_UNI_PORT_ID1 || port > LOOP_DETECT_PORT_MAX)
        return CS_E_PARAM;

    cs_timer_del(g_ldCb.detect_ports[port-1].clear_timer.timer_handler);

    if(g_ldCb.detect_ports[port-1].status == LOOP_DETECTED)
        return CS_E_OK;

    if(g_ldCb.block_obj == LOOP_DETECT_BLOCK_GLOBAL && g_ldCb.need_block)
    {
        need_block = TRUE;
    }
    else if(g_ldCb.block_obj == LOOP_DETECT_BLOCK_PORT_BASE &&
        g_ldCb.detect_ports[port-1].need_block)
    {
        need_block = TRUE;
    }
        

    /*blcok handle*/
    if(need_block)
    {
        /*block port prepare handle*/
        LOOP_DETECT_CALL_BACK(port, LOOP_DETECT_PORT_BLOCK_PREPARE_EVENT);
        
        if(g_ldCb.block_obj == LOOP_DETECT_BLOCK_GLOBAL)
        {
            if(g_ldCb.block_state != LOOP_DETECT_BLOCKED)
            {
                result = epon_request_onu_loopdetect_block(context, ONU_DEVICEID_FOR_API, 
                                                                                  ONU_LLIDPORT_FOR_API, CS_ALL_UNI_PORT_ID, 1);
                IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, "block uni for receive frame\n");

                g_ldCb.block_state = LOOP_DETECT_BLOCKED;
                 
                if(g_ldCb.block_timer.timer_value != 0 && g_ldCb.unblock_policy == LOOP_DETECT_EXPIRE_UNBLOCK)
                {
                    /*need timer to restore*/
                    g_ldCb.block_timer.timer_handler = cs_msg_timer_add(g_ld_msgq, g_ldCb.block_timer.timer_value, 
                                                                                        (void*)&(g_ldCb.block_timer), NULL);
                }
                /*block port after handle*/
                LOOP_DETECT_CALL_BACK(port, LOOP_DETECT_PORT_BLOCK_AFTER_EVENT);
            }
            
        }
        else
        {
            if(g_ldCb.detect_ports[port-1].block_state != LOOP_DETECT_BLOCKED)
            {
                result = epon_request_onu_loopdetect_block(context, ONU_DEVICEID_FOR_API, 
                                                                                  ONU_LLIDPORT_FOR_API, port, 1);
                IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, 
                                       "block uni:%d for receive frame, result:%d\n", port, result);
                
                g_ldCb.detect_ports[port-1].block_state = LOOP_DETECT_BLOCKED;
                if(g_ldCb.detect_ports[port-1].block_timer.timer_value != 0 &&
                    g_ldCb.unblock_policy == LOOP_DETECT_EXPIRE_UNBLOCK)
                {
                    /*need timer to restore*/
                    g_ldCb.detect_ports[port-1].block_timer.timer_handler = cs_msg_timer_add(g_ld_msgq, 
                                                                                        g_ldCb.detect_ports[port-1].block_timer.timer_value, 
                                                                                        (void*)&(g_ldCb.detect_ports[port-1].block_timer), NULL);
                }
                /*block port after handle*/
                LOOP_DETECT_CALL_BACK(port, LOOP_DETECT_PORT_BLOCK_AFTER_EVENT);
            }
        } 
         
    }

    if(g_ldCb.detect_ports[port-1].status == NO_LOOP)
    {
        /*loop detected prepare handle*/
        LOOP_DETECT_CALL_BACK(port, LOOP_DETECTED_PREPARE_EVENT);
        
        /*loop handle*/
        g_ldCb.detect_ports[port-1].status = LOOP_DETECTED;
        g_ldCb.detect_ports[port-1].loop_counter++;

        /*loop detected after handle*/
        LOOP_DETECT_CALL_BACK(port, LOOP_DETECTED_AFTER_EVENT);
        IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP,  "port(%d) loop detected \n", port);
    }
    else
    {
        g_ldCb.detect_ports[port-1].status = LOOP_DETECTED;
    }
	
	#if 1
	//added by zhuxh,when port loop was found, down the port
	{
		cs_callback_context_t context;
		if(g_ldCb.detect_ports[port-1].need_block)
		{
			epon_request_onu_port_admin_set(context, 0, 0, port, SDL_PORT_ADMIN_DOWN);
		}
	}
	#endif

    return result;
}

cs_status loop_cleared_common_handle(cs_port_id_t port)
{
    return CS_E_OK;
}

cs_status loop_detect_parser(cs_pkt_t *pPkt)
{
    if(pPkt->eth_type == EPON_ETHERTYPE_LOOP_DETECT) {
        pPkt->pkt_type = CS_PKT_LOOPDETECT;
        return CS_E_OK;
    }
    else {
        return CS_E_NOT_SUPPORT;
    }
}

cs_uint8 loop_detect_frame_handle(cs_pkt_t *pPkt)
{
    cs_uint8* frame;
    cs_uint32 len;
    cs_port_id_t port;
    
    loop_detect_ether_header_t *plb_frame = NULL;
    loop_detect_ether_header_vlan_t *plb_vlan_frame = NULL;
    cs_port_id_t temp_lb_port = CS_ALL_UNI_PORT_ID;
    cs_uint16 ether_type;
    cs_uint8 macaddr[CS_MACADDR_LEN];
    cs_uint8 dest_macaddr[CS_MACADDR_LEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    cs_callback_context_t     context;

    frame = pPkt->data + pPkt->offset;
    len = pPkt->len;
    port = pPkt->port;

    epon_request_onu_pon_mac_addr_get(context, ONU_DEVICEID_FOR_API, 
                                                               ONU_LLIDPORT_FOR_API, (cs_mac_t *)macaddr);

    DUMPLOOPPKT("ld rec", frame, len);

    if((port < CS_UNI_PORT_ID1) || (port > LOOP_DETECT_PORT_MAX))
    {
        return 1;
    }

    if(g_ldCb.detect_ports[port-1].status == LOOP_DETECTED)
    {
        return 1;
    }

    plb_frame = (loop_detect_ether_header_t *)frame;
    ether_type = ntohs(plb_frame->ethertype);
    if (ether_type == EPON_ETHERTYPE_DOT1Q)    
    {
        plb_vlan_frame = (loop_detect_ether_header_vlan_t*)frame;
        temp_lb_port = ntohl(plb_vlan_frame->lb_port);
    }
    else
    {
        temp_lb_port = ntohl(plb_frame->lb_port);
    }

    /*check pkt  mac*/
    if(memcmp(macaddr, frame+6, CS_MACADDR_LEN) != 0 || 
        memcmp(dest_macaddr, frame, CS_MACADDR_LEN) != 0)
    {
        return 1;
    }

    if((temp_lb_port < CS_UNI_PORT_ID1) || (port > LOOP_DETECT_PORT_MAX))
    {
        return 1;
    }

    if(port != temp_lb_port)
    {
        g_ldCb.detect_ports[temp_lb_port-1].reason = LOOP_REASON_INNER;
    }
    else
    {
        g_ldCb.detect_ports[temp_lb_port-1].reason = LOOP_REASON_OUT;
    }
 
    loop_detect_event_handler(temp_lb_port, LOOP_DETECTED_EVENT);

    return 1;
}

cs_status loop_detect_block_expired_common_handle(cs_port_id_t port)
{
    cs_callback_context_t     context;
    cs_status result = CS_E_OK;

    LOOP_DETECT_PORT_CHECK(port);

    /*unblock port prepare handle*/
    LOOP_DETECT_CALL_BACK(port, LOOP_DETECT_PORT_UNBLOCK_PREPARE_EVENT);

    result = epon_request_onu_loopdetect_block(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, 0);
    IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, 
                           "open port(%d) when expired, result:%d  \n", port, result);

    if(CS_E_OK == result)
    {
        if(g_ldCb.block_obj == LOOP_DETECT_BLOCK_GLOBAL)
        {
            g_ldCb.block_state = LOOP_DETECT_NOT_BLOCK;
            g_ldCb.block_timer.timer_handler = 0;
        }
        else
        {
            if(port != CS_ALL_UNI_PORT_ID)
            {
                g_ldCb.detect_ports[port-1].block_state = LOOP_DETECT_NOT_BLOCK;
                g_ldCb.detect_ports[port-1].block_timer.timer_handler = 0;
            }
        }

        /*unblock port after handle*/
        LOOP_DETECT_CALL_BACK(port, LOOP_DETECT_PORT_UNBLOCK_AFTER_EVENT)
    }

    return result;
    
}

void loop_detect_port_oper_down_handler(cs_port_id_t port)
{
    #if 0
    cs_callback_context_t     context;
    
    if(port < CS_UNI_PORT_ID1 || port > LOOP_DETECT_PORT_MAX)
    {
        return;
    }

    IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, "port(%d) oper down handle\n", port);

    if(g_ldCb.detect_ports == NULL)
    {
        IROS_LOG_MSG(IROS_LOG_LEVEL_MIN, IROS_MID_APP, "not ready\n");
        return;
    }

    if(!g_ldCb.detect_ports[port-1].enable)
    {
        IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, "ignore it\n");
        return;
    }

    if(g_ldCb.block_obj == LOOP_DETECT_BLOCK_PORT_BASE)
    {
        if(g_ldCb.detect_ports[port-1].status != NO_LOOP)
        {
            /*prepare handle*/
            LOOP_DETECT_CALL_BACK(port, LOOP_CLEARED_PREPARE_EVENT);
            g_ldCb.detect_ports[port-1].status = NO_LOOP;
            /*open this port anyway*/
            epon_request_onu_loopdetect_block(context, ONU_DEVICEID_FOR_API, 
                                                                               ONU_LLIDPORT_FOR_API, port, 0);
            /*after handler*/
            LOOP_DETECT_CALL_BACK(port, LOOP_CLEARED_AFTER_EVENT);

            /*check timer*/
            if(g_ldCb.detect_ports[port-1].block_state != LOOP_DETECT_NOT_BLOCK)
            {
                g_ldCb.detect_ports[port-1].block_state = LOOP_DETECT_NOT_BLOCK;
                cs_timer_del(g_ldCb.detect_ports[port-1].block_timer.timer_handler);
                g_ldCb.detect_ports[port-1].block_timer.timer_handler = 0;
            }
        }
    }
    #endif
}
    
cs_status loop_detect_event_handler(cs_port_id_t port, loop_detect_event_t  type)
{
    cs_status result = CS_E_OK;
    return 0;
    LOOP_DETECT_PORT_CHECK(port);
    switch(type)
    {
        case LOOP_DETECTED_EVENT:

            IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, "LOOP_DETECTED:port(%d)\n", port);
            result = loop_detected_common_handle(port);
            
            break;

        case LOOP_CLEARED_EVENT:
            IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, "LOOP_CLEARED:port(%d)\n", port);
            result = loop_cleared_common_handle(port);
            break;
            
        case UNBLOCK_TIMER_EXPIRED_EVENT:
            IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, "UNBLOCK_TIMER_EXPIRED:port(%d)\n", port);
            result = loop_detect_block_expired_common_handle(port);
            break;
            
            
        case LOOPDETECT_TIMER_EXPIRED_EVENT:
            IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, "LOOPDETECT_TIMER_EXPIRED:port(%d)\n", port);
            result = loop_detect_period_common_handle(port);
            break;
            
        case LOOPDETECT_ENABLE_EVENT:
            IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, "LOOPDETECT_ENABLE:port(%d)\n", port);
            result = loop_detect_enable_common_handle(port);
            break;
            
        case LOOPDETECT_DISABLE_EVENT:
            IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, "LOOPDETECT_DISABLE:port(%d)\n", port);
            loop_detect_disable_common_handle(port);
            break;

        case CLEAR_CHECK_TIMER_EXPIRED_EVENT:
            IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, "CLEAR_TIMER_EXPIRED:port(%d)\n", port);
            loop_clear_check_common_handle(port);
            break;
            
        default:
            IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_APP, "UNKNOWN EVENT:port(%d)\n", port);
            result = CS_E_PARAM;
            break;
    }

    
    return result;
}

cs_status loop_detect_loop_reason_get(cs_port_id_t port, loop_reason_t *reason)
{
    if((port < CS_UNI_PORT_ID1) || (port > LOOP_DETECT_PORT_MAX))
    {
        return CS_E_PARAM;
    }

    *reason = g_ldCb.detect_ports[port-1].reason;
    return CS_E_OK;
}

void loop_detect_show()
{
    cs_uint8 i = 0;
    cs_printf("loop detect enable:%d\n", g_ldCb.enable);
    cs_printf("loop detect need block:%d\n", g_ldCb.need_block);
    cs_printf("loop detect block obj:%d(%s)\n", g_ldCb.block_obj, block_string[g_ldCb.block_obj]);
    cs_printf("loop detect block state:%d\n", g_ldCb.block_state);
    cs_printf("loop detect unblock policy:%d(%s)\n", g_ldCb.unblock_policy, unblock_string[g_ldCb.unblock_policy]);
    cs_printf("loop detect clear policy:%d(%s)\n", g_ldCb.clear_policy, clear_string[g_ldCb.clear_policy]);
    cs_printf("loop detect max port:%d\n", g_ldCb.max_port);
    cs_printf("loop detect interval timer value:%d\n", g_ldCb.interval_timer.timer_value);
    cs_printf("loop detect interval timer handler:%d\n", g_ldCb.interval_timer.timer_handler);
    cs_printf("loop detect block timer value:%d\n", g_ldCb.block_timer.timer_value);
    cs_printf("loop detect block timer handler:%d\n", g_ldCb.block_timer.timer_handler);
    cs_printf("loop detect special handler:%p\n", g_ldCb.special_handler);
    for(i = 0; i < LOOP_DETECT_PORT_MAX; i++)
    {
        cs_printf("port(%d) enable:%d\n", i+1, g_ldCb.detect_ports[i].enable);
        cs_printf("port(%d) status:%d\n", i+1, g_ldCb.detect_ports[i].status);
        cs_printf("port(%d) block enable:%d\n", i+1, g_ldCb.detect_ports[i].need_block);
        cs_printf("port(%d) block state:%d\n", i+1, g_ldCb.detect_ports[i].block_state);
        cs_printf("port(%d) block timer value:%d\n", i+1, g_ldCb.detect_ports[i].block_timer.timer_value);
        cs_printf("port(%d) block timer handler:%d\n", i+1, g_ldCb.detect_ports[i].block_timer.timer_handler);
        cs_printf("port(%d) clear timer value:%d\n", i+1, g_ldCb.detect_ports[i].clear_timer.timer_value);
        cs_printf("port(%d) clear timer handler:%d\n", i+1, g_ldCb.detect_ports[i].clear_timer.timer_handler);
        cs_printf("port(%d) loop reason:%d\n", i+1, g_ldCb.detect_ports[i].reason);
    }
}

void loop_detect_test(cs_uint8 type, cs_port_id_t port, cs_uint32 value)
{
    if(type == 1)
    {
        /*enable it*/
        loop_detect_enable(port);
    }
    else if(type == 2)
    {
        /*disable it*/
        loop_detect_disable(port);
    }
    else if(type == 3)
    {
        cs_pkt_t *pkt = iros_malloc(IROS_MID_PACKET, 
            sizeof(cs_pkt_t) + sizeof(loop_detect_ether_header_t) - 1);
        loop_detect_ether_header_t *frame;

        if(pkt == NULL)
        {
            return;
        }

        memset(pkt, 0, sizeof(cs_pkt_t));
        
        pkt->pkt_type = CS_PKT_LOOPDETECT;
        pkt->port = port;
        frame = (loop_detect_ether_header_t *)pkt->data;
        
        /*receive detect frame*/
        frame->ethertype = htons(EPON_ETHERTYPE_LOOP_DETECT);
        frame->lb_port = htonl(port);
        loop_detect_frame_handle(pkt);

        iros_free(pkt);
    }
    else if(type == 4)
    {
        /*reg kt*/
        g_ldCb.special_handler = NULL;
    }
    else if(type == 5)
    {
        loop_detect_interval_set(value);
    }
    else if(type == 6)
    {
        loop_detect_block_time_set(port, value);
    }
    else if(type == 7)
    {
        if(g_ldTrace)
            g_ldTrace = 0;
        else
            g_ldTrace = 1;
    }
    else if(type == 8)
    {
        loop_detect_block_object_set(value);
    }
    else if(type == 9)
    {
        loop_detect_unblock_policy_set(value);
    }
    else if(type == 10)
    {
        loop_clear_policy_set(value);
    }
    else if(type == 11)
    {
        loop_clear_check_time_set(port, value);
    }
	else if(type == 12)
    {
        loop_detect_port_block_enable(port, value);
    }

}

void loop_detect_defaul_set(cs_uint8 en)
{
    cs_uint32 port = 0;

    for(port = 1;port <= 4;port ++)
    {
        loop_detect_enable(port);
        loop_detect_port_block_enable(port,en);
    }
}
#endif

