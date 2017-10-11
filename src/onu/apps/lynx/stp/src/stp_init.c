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
#include "stpm.h"
#include "stp_in.h"
#include "stp_to.h"
#include "app_timer.h"
#include "packet.h"
#include "sdl_api.h"
#include "sdl_l2.h"
#include "stp_api.h"
#include "stp_sdl_adapt.h"

/*Global variables*/
cs_mac_t  stp_bridge_macaddr;
static app_timer_msg_t  g_stp_timer;

extern cs_uint8 max_port;
extern cs_int32 onu_evt_reg(cs_int32 evt_type , void *func , void *cookie);

void stp_port_link_chg_handler(
    cs_uint16   evt_dev,
    cs_uint32   evt_type,
    void        *data)
{
        cs_boolean   enable = False;
        STPM_T* stpm = NULL;
        PORT_T * stp_port = NULL;    
        sdl_event_port_link_t *msg = (sdl_event_port_link_t*)data;
		stp_polling_handler_t handler = NULL;
		
        STP_LOG("port(%x): refresh port state %s \n" ,  msg->port, 
            msg->link == ONU_PORT_OPER_UP ? "UP":"DOWN" );
        
        stpm = stpapi_stpm_find(STP_DEF_VLAN);
        if (stpm == NULL) {
                return ;
        }

		handler = stpm->poll_handler;
		
        if (stpm->admin_state !=STP_ENABLED) {
                return ;
        }

        stp_port =  _stpapi_port_find(stpm, msg->port);
        if (stp_port == NULL) {
                return ;
        }

        if ( msg->port ==CS_PON_PORT_ID) {
                /*pon port don't care*/
                return ;
        }

        if (  (msg->link !=ONU_PORT_OPER_UP) 
                &&  (msg->link !=ONU_PORT_OPER_DOWN) ) {
                return ;
        }

        if (msg->link ==ONU_PORT_OPER_UP) {
                enable = True;
        }else {
                enable = False;
        }

        if (stp_port->portEnabled == enable) {
                STP_LOG("Port stp is already enabled\n");
                return ;
        } 
        
        if((msg->link == ONU_PORT_OPER_DOWN)&&(handler != NULL)){
			
            handler(msg->port, NULL, STP_PORT_DOWN);
        }

        STP_IN_enable_port(msg->port, enable);

        return ;
}

static void stp_cfg_init(void )
{
        cs_onu_stp_config_t                    bridge_cfg;
        cs_onu_port_stp_config_t            port_cfg;
        cs_port_id_t                                port_index = 0;

        memset( &bridge_cfg, 0 ,sizeof(cs_onu_stp_config_t) ); 
        memset(&port_cfg, 0, sizeof(cs_onu_port_stp_config_t));
         
        bridge_cfg.mask = ONU_MASK_STP_BRIDGE_ALL;
        bridge_cfg.enable = 0;
        bridge_cfg.bridge_priority = DEF_BR_PRIO;
        bridge_cfg.max_age = DEF_BR_MAXAGE;
        bridge_cfg.hello_time = DEF_BR_HELLOT;
        bridge_cfg.forward_delay = DEF_BR_FWDELAY;
        bridge_cfg.force_version = NORMAL_RSTP;
        bridge_cfg.hold_time = 3;
        app_stp_bridge_cfg_set(&bridge_cfg); 

        port_cfg.mask = ONU_MASK_PORT_STP_ALL;
        port_cfg.port_priority = DEF_PORT_PRIO;
        port_cfg.admin_pcost = ADMIN_PORT_PATH_COST_AUTO;
        port_cfg.admin_p2p = DEF_P2P;
        port_cfg.admin_edge = DEF_ADMIN_EDGE;
        port_cfg.admin_non_stp = DEF_ADMIN_NON_STP;
        
        for ( port_index = CS_PON_PORT_ID; port_index <max_port; port_index++){
                if ( port_index == CS_PON_PORT_ID ) {
                        port_cfg.port_priority = MIN_PORT_PRIO;
                        port_cfg.admin_non_stp = True;
                 }else{
                        port_cfg.admin_non_stp = DEF_ADMIN_NON_STP;
                        port_cfg.port_priority = DEF_PORT_PRIO;
                }
                 app_stp_port_cfg_set(port_index, &port_cfg);
        }

}

static cs_status  stp_bridge_start (void)
{
        BITMAP_T    ports;
        cs_uint32          i = 0;
        
        BitmapClear(&ports);

        if(CS_E_OK != startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &max_port))
        {
            max_port = 4;
        }

        STP_IN_init(max_port);
        
        STP_LOG("max_port is:%d\n", max_port);
        
        for (i = 2; i <= max_port; i++) {
                BitmapSetBit(&ports, i-1);
        }

        STP_IN_stpm_create(STP_DEF_VLAN , "B1", &ports);
        stp_cfg_init( );
            
        return CS_E_OK;
}

cs_status stp_pkt_parser(cs_pkt_t *pPkt)
{
    if(pPkt->eth_type == EPON_ETHERTYPE_STP ||
        pPkt->eth_type == EPON_ETHERTYPE_RSTP ||
        pPkt->eth_type == EPON_ETHERTYPE_TCN) {
        pPkt->pkt_type = CS_PKT_BPDU;
        return CS_E_OK;
    }
    else {
        return CS_E_NOT_SUPPORT;
    }
}

void stp_tm_free(void * ptr)
{
    cs_uint32 timerHandle = 0;

    IROS_LOG_MSG(IROS_LOG_LEVEL_DBG1, IROS_MID_STP, "stp_tm_free.\n");
    timerHandle = cs_msg_circle_timer_add(app_msg_q, ONU_STP_INTERVAL, 
        (void *)&g_stp_timer);
    if (timerHandle == CS_INVALID_TIMER) {
        IROS_LOG_MSG(IROS_LOG_LEVEL_CRI, IROS_MID_STP, "stp_tm_free cs_msg_circle_timer_add failed.\n");
    }
}

void stp_timer_init()
{
    cs_uint32 timerHandle = 0;
    
    g_stp_timer.msg_type = IROS_MSG_TIME_OUT;
    g_stp_timer.timer_type = APP_TIMER_RSTP;
    
    timerHandle = cs_msg_circle_timer_add(app_msg_q,
            ONU_STP_INTERVAL, (void *)&g_stp_timer);
    if(timerHandle == CS_INVALID_TIMER)
    {
        IROS_LOG_MSG(IROS_LOG_LEVEL_CRI, IROS_MID_STP, "stp_timer_init cs_msg_circle_timer_add failed.\n");
    }
}

void stp_enable()
{
    cs_callback_context_t context;

    /*init ONU bridge mac address*/
    memset(&stp_bridge_macaddr, 0 ,sizeof( stp_bridge_macaddr));
    epon_request_onu_pon_mac_addr_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, &stp_bridge_macaddr);

    /*Register port link change event handler*/
    onu_evt_reg(EPON_EVENT_PORT_LINK_CHANGE, (void*)stp_port_link_chg_handler, NULL);

    /*init STP, add PON and UNI ports to STP and run STP state machine*/
    stp_bridge_start();
    
}

cs_uint8 stp_proc(cs_pkt_t *pPkt)
{
	STPM_T* stpm = NULL;
	stp_polling_handler_t handler = NULL;
	
	stpm = stpapi_stpm_find(STP_DEF_VLAN);
	if (stpm == NULL) {
			return 1;
	}
	
	handler = stpm->poll_handler;
	
	if(handler != NULL)
	    handler(pPkt->port, pPkt->data+pPkt->offset+CS_MACADDR_LEN,STP_RECEIVE_BPDU);

    STP_IN_rx_bpdu(STP_DEF_VLAN, pPkt->port, (BPDU_T*) ( pPkt->data+pPkt->offset + sizeof (stp_mac_hdr_t)), 
    (pPkt->len - sizeof (stp_mac_hdr_t)));

    return 1;
}

cs_status  stp_init( )  
{

    stp_enable();
    stp_timer_init();
    
    app_pkt_reg_handler(CS_PKT_BPDU, stp_proc);
    app_pkt_reg_parser(CS_PKT_BPDU, stp_pkt_parser);
    
    return CS_E_OK;
}

/*STP timeout function*/
 void onu_tmfunc_rstp()
{
        STP_IN_one_second( );
}

#endif

