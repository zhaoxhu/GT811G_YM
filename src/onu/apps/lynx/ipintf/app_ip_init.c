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

DEFINITIONS:  "DEVICE" means the Cortina Systems?LynxD SDK product.
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
#ifdef HAVE_IP_STACK
#include <network.h>
#include "mempool.h"
#include "osal_api_core.h"
#include "startup_cfg.h"
#include "cs_module.h"
#include "plat_common.h"
#include "packet.h"
//#include "iros_cmn_msg.h"
#include "event.h"
#include "app_timer.h"
#include "sdl_api.h"
#include "sdl_l2.h"

#include "app_ip_types.h"
#include "app_ip_mgnt.h"
#include "app_ip_init.h"
#include "app_ip_util.h"

static  app_timer_msg_t age_timer;
ipintf_info_t ipintf_info;
extern cs_status ipintf_cmd_reg();

/*
*   PROTOTYPE    cs_uint32 app_ipintf_get_max_port()
*   INPUT            None.
*   OUTPUT         None
*   Description     Get max port number from startup configure.
*   Pre-condition  
*   Post-condition
*   Note             
*/
cs_uint32 app_ipintf_get_max_port()
{
    cs_uint8 port_num;
    
    startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &port_num);
    port_num++; /* add pon port */
    
    return port_num;
}

/*
*   PROTOTYPE    cs_status app_ipintf_arp_pkt_parser(cs_pkt_t *pPkt)
*   INPUT            pPkt
*   OUTPUT         None
*   Description     
*   Pre-condition  
*   Post-condition
*   Note  
*/
cs_status app_ipintf_arp_pkt_parser(cs_pkt_t *pPkt)
{
    if(pPkt->eth_type == EPON_ETHERTYPE_ARP) {
        pPkt->pkt_type = CS_PKT_ARP;
        return CS_E_OK;
    }
    else {
        return CS_E_NOT_SUPPORT;
    }
}

/*
*   PROTOTYPE    cs_status app_ipintf_ip_pkt_parser(cs_pkt_t *pPkt)
*   INPUT            pPkt
*   OUTPUT         None
*   Description     
*   Pre-condition  
*   Post-condition
*   Note  
*/
cs_status app_ipintf_ip_pkt_parser(cs_pkt_t *pPkt)
{
    if(EPON_ETHERTYPE_IP == pPkt->eth_type) {
        cs_callback_context_t context;
        cs_mac_t mac;
#ifdef DHCP_OPTION_PPPOE_PLUS_SUPPORT
        if(app_dhcp_packet_parse(pPkt)) {        
            pPkt->u2Reserved= IPV4_DHCP;
            pPkt->pkt_type = CS_PKT_IP;
            IROS_LOG_MIN(IROS_MID_ETHDRV,"%s: IPV4_DHCP\n", __func__);
            return CS_E_OK;
        }
#endif        
        epon_request_onu_pon_mac_addr_get(context, 0, 0, &mac);
        if(!memcmp(pPkt->data + pPkt->offset, mac.addr, 6)) {
            pPkt->pkt_type = CS_PKT_IP;
            return CS_E_OK;
        }
    }

    if(EPON_ETHERTYPE_IPV6 == pPkt->eth_type) {
#ifdef DHCP_OPTION_PPPOE_PLUS_SUPPORT
        if(app_dhcpv6_packet_parse(pPkt)) {
            pPkt->u2Reserved = IPV6_DHCP;
            pPkt->pkt_type = CS_PKT_IP;
            IROS_LOG_MIN(IROS_MID_ETHDRV,"%s: IPV6_DHCP\n", __func__);
            return CS_E_OK;
        }
#endif        
    }
    IROS_LOG_MIN(IROS_MID_ETHDRV,"%s: pPkt->eth_type %d, pPkt->pkt_type %d\n", __func__, pPkt->eth_type, pPkt->pkt_type);
    return CS_E_NOT_SUPPORT;
}

/*
*   PROTOTYPE    cs_status app_ipintf_dhcp_pkt_parser(cs_pkt_t *pPkt)
*   INPUT            pPkt
*   OUTPUT         None
*   Description     
*   Pre-condition  
*   Post-condition
*   Note  
*/
cs_status app_ipintf_dhcp_pkt_parser(cs_pkt_t *pPkt)
{
    cs_printf("app_dhcp_parser entry\n");
    if((EPON_ETHERTYPE_IP == pPkt->eth_type) && (SDL_PKT_DHCP == pPkt->frame_type)) {        
        pPkt->pkt_type = CS_PKT_DHCP;
        cs_printf("DHCP pkt parser pass\n");
        return CS_E_OK;
    }
    return CS_E_NOT_SUPPORT;
}


/*
*   PROTOTYPE    void app_ipintf_link_change_cb(cs_uint16 evt_dev, cs_uint32 evt_type, void *data)
*   INPUT            
*   OUTPUT         None
*   Description     
*   Pre-condition  
*   Post-condition
*   Note  
*/
void app_ipintf_link_change_cb(cs_uint16 evt_dev, cs_uint32 evt_type, void *data)
{
    sdl_event_port_link_t *msg = (sdl_event_port_link_t*)data;
    
    if(msg == NULL)
        return;
    
    //app_ipintf_port_mac_flush(msg->port);

    return;
}

/*
*   PROTOTYPE    void app_ipintf_mac_aging_init()
*   INPUT            void
*   OUTPUT         None
*   Description     
*   Pre-condition  
*   Post-condition
*   Note  
*/
void app_ipintf_mac_aging_init(void)
{
    cs_uint32 ageHandle = 0;
    static cs_uint32 init = 0;
    if(!init)
    {
        age_timer.msg_type = IROS_MSG_TIME_OUT;
        age_timer.timer_type = APP_TIMER_ARPAGING;
        ageHandle = cs_msg_circle_timer_add(app_msg_q,IPINTF_TM_TICKS*1000, 
                                                                (void *)&age_timer);
        if(ageHandle == CS_INVALID_TIMER) {
            IROS_LOG_MIN(IROS_MID_ETHDRV, "arp aging timer initialization, add timer failed.\n");
        }
        init = 1;
    }
}



/*
*   PROTOTYPE    void app_ipintf_device_init(eth_info_t *eth_info)
*   INPUT            eth_info
*   OUTPUT         None
*   Description     
*   Pre-condition  
*   Post-condition
*   Note  
*/
void app_ipintf_device_init(ipintf_info_t *eth_info)
{
    cs_uint8 *mac = eth_info->mac;
    memset(eth_info, 0, sizeof(ipintf_info_t));
    eth_info->localip = IPINTF_DFT_IP_ADDR;
    eth_info->ipmask = IPINTF_DFT_IP_MASK;
    eth_info->inband = 0;
    eth_info->pri = 0;
    eth_info->vlanid = 0;
    eth_info->remoteip = 0;
    eth_info->remotemask = 0;
    eth_info->gateway = 0;
    eth_info->mtu = IPINTF_DFT_MTU;
    eth_info->ageinterval = IPINTF_MAC_AGING_INTERVAL;
    eth_info->keepalive = IPINTF_ARP_KEEPALIVE_DFT;
    startup_config_read(CFG_ID_MAC_ADDRESS , ETHER_ADDR_LEN,eth_info->mac);
    cs_printf("my mac = %02X:%02X:%02X:%02X:%02X:%02X\n",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

}


/*
*   PROTOTYPE    ipintf_protocol_config(void)
*   INPUT            void
*   OUTPUT         None
*   Description     
*   Pre-condition  
*   Post-condition
*   Note  
*/
cs_status ipintf_protocol_config(void)
{

    app_pkt_reg_parser(CS_PKT_ARP, app_ipintf_arp_pkt_parser);
    app_pkt_reg_handler(CS_PKT_ARP, app_ipintf_pkt_recv);
    app_pkt_reg_parser(CS_PKT_IP, app_ipintf_ip_pkt_parser);
    app_pkt_reg_handler(CS_PKT_IP, app_ipintf_pkt_recv);    
    app_ipintf_pkt_dir_default();
    
    return CS_E_OK;
}
/*
*   PROTOTYPE    cs_status app_ipintf_init()
*   INPUT            void
*   OUTPUT         None
*   Description     
*   Pre-condition  
*   Post-condition
*   Note  
*/
#ifdef HAVE_ZTE_SIJIE
extern void eoc_pon_init();
#endif  /*END_HAVE_ZTE_SIJIE*/
cs_status app_ipintf_init(void)
{
    cs_uint32 i = 0;
    cs_uint32 len = 0;
    app_ipintf_device_init(&ipintf_info);
    ipintf_info.maxport = app_ipintf_get_max_port();
    
    /* init port */
    len = sizeof(ipintf_port_info_t) *ipintf_info.maxport;
    ipintf_info.port = (ipintf_port_info_t*)iros_malloc(IROS_MID_ETHDRV, len);
    if(!ipintf_info.port)
    {
        cs_printf("%s failed, no memory \n", __func__);
        return -1;
    }
    memset(ipintf_info.port, 0, len);

    for(i=0;i<ipintf_info.maxport;i++)
    {
        ipintf_info.port[i].port = i;
    }
    /* init mac table */
    len = sizeof(ipintf_mac_entry_t) * IPINTF_MAC_ENTRY_MAX;
    ipintf_info.mactbl= (ipintf_mac_entry_t*)iros_malloc(IROS_MID_ETHDRV,len);
    if(!ipintf_info.mactbl)
    {
        cs_printf("%s failed, no memory \n", __func__);
        return -1;
    }
    memset(ipintf_info.mactbl, 0, len);	
        
    /* register callback handle for link state change */
    onu_evt_reg(EPON_EVENT_PORT_LINK_CHANGE, app_ipintf_link_change_cb, NULL);
    onu_evt_reg(EPON_EVENT_REG_CHANGE, app_ipintf_reg_event_cb ,NULL);

    app_ipintf_pkt_special_process_reg();
    app_ipintf_mac_aging_init();
    app_ipintf_add_ip_address(ipintf_info.localip, 0, ipintf_info.ipmask);
    app_ipintf_set_mtu(ipintf_info.mtu);
    ipintf_protocol_config();
    ipintf_cmd_reg();

#ifdef DHCP_OPTION_PPPOE_PLUS_SUPPORT    
    dhcp_option_init();
#endif
#ifdef HAVE_ZTE_SIJIE
eoc_pon_init();
#endif  /*END_HAVE_ZTE_SIJIE*/
    cs_printf("%s port num %d\n", __func__, ipintf_info.maxport);
    
    return CS_E_OK;
    
}


/*
*   PROTOTYPE    cs_status init_ip_service(void)
*   INPUT            void
*   OUTPUT         None
*   Description     
*   Pre-condition  
*   Post-condition
*   Note  
*/
#ifdef CYGPKG_LWIP_IPV4_SUPPORT
extern int lwip_init(void);
#endif
cs_status init_ip_service(void)
{
    cs_status ret = 0;
    #ifdef CYGPKG_LWIP_IPV4_SUPPORT
    /* init ip stack */
    lwip_init();
    #endif
    ret = app_ipintf_init();
    if(ret != 0) {
        cs_printf("%s - app_ipintf_init, failed %d\n",__func__, ret);
        return CS_E_ERROR;
    }

    return ret;
    
}

#endif


