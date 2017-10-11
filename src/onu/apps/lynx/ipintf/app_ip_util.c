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
#include <errno.h>
#include "plat_common.h"
#include "oam_api.h"
#include "plat_common.h"

#include "packet.h"
#include "frame.h"
#include "sdl_api.h"
#include "cs_cmn.h"
#include "app_timer.h"
#include "osal_api_core.h"

//#include "mempool.h"

#include "app_vlan.h"
#include "app_ip_types.h"
#include "app_ip_init.h"
#include "app_ip_mgnt.h"
#include "app_ip_api.h"
#include "app_ip_adapter.h"
#include "app_ip_util.h"
#include "app_oam_gwd.h"
#include "gwd_types.h"
#include "gwd_utils.h"

#define ONU_SLOT_NUM    1
#define ONU_SUBSLOT_NUM 0


/*
* if return code of these routines > 0, just return from the calling place.
* otherwise, contine the code from the calling place
*/
app_ipintf_ip_rx_filter_routine_t  app_ipintf_ip_rx_filter = NULL;
app_ipintf_arp_rx_filter_routine_t app_ipintf_arp_rx_filter = NULL;
app_ipintf_ip_tx_filter_routine_t app_ipintf_ip_tx_filter = NULL;
app_ipintf_arp_tx_filter_routine_t app_ipintf_arp_tx_filter = NULL;
app_ipintf_enable_inband_routine_t app_ipintf_enable_inband = NULL;
app_ipintf_enable_inband_routine_t app_ipintf_enable_outband = NULL;
cs_uint32 app_ip_changed = 0;
//extern     ipintf_info_t ipintf_info;
extern ipoe_state_t g_ipoe_state;
extern void netdev_pkt_recv(cs_pkt_t * event_buf);
#ifdef HAVE_TCP_UDP_NETWORK_CARD
//added by zhuxh
extern void netdev_eth1_pkt_recv(cs_pkt_t * event_buf);
#endif
#ifdef HAVE_ZTE_SIJIE
#define EOC_PON_VLAN_ID 4088
#endif /*END_HAVE_ZTE_SIJIE*/
cs_status app_ipoe_port_lpbk_set(
        cs_port_id_t port,
        cs_boolean enable)
{
    cs_callback_context_t context;
    cs_status ret = CS_E_OK;
  	cs_uint8 port_num = 0 ;
	
    startup_config_read(CFG_ID_SWITCH_PORT_NUM, sizeof(port_num), &port_num);
	
	if(4 == port_num){
		cs_sdl_port_loopback_t mode;
		
		if(enable){
			mode = SDL_PORT_LOOPBACK_TX2RX;
		}
		else {
			mode = SDL_PORT_LOOPBACK_NONE;
		}
		
		ret = epon_request_onu_port_lpbk_set(context, 0, 0, port, mode);
	}
	else {
		ret = epon_request_onu_phy_loopback_set(context, 0, 0, enable);
	}
	
    if(CS_E_OK != ret) {
        cs_printf("%s() --- set port %d loopback fail!!! ret=%d\n", __FUNCTION__, port, ret);
    }
 
    return ret;
}

/*
cs_boolean app_dhcp_packet_parse(cs_pkt_t *pkt)
{
    cs_uint8 *data = pkt->data + pkt->offset + pkt->tag_num*4;
    cs_ether_header_t *eth_hdr = NULL; 
    ip_hdr_t iphdr, *ip_hdr = NULL;
    udp_hdr_t udphdr, *udp_hdr = NULL;
    
    eth_hdr = (cs_ether_header_t *)(data); 
    if(!IS_IP_PKT(ntohs(eth_hdr->ethertype))) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: eth type 0x%x\n", __func__, ntohs(eth_hdr->ethertype));
        return FALSE;
    }
    ip_hdr = (ip_hdr_t *)(eth_hdr + 1);
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: ip mem addr 0x%x\n", __func__, (int)ip_hdr);
    memcpy(&iphdr, ip_hdr, sizeof(ip_hdr_t));
    if(iphdr.protocol != IP_UDP_PROTOCOL_ID) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: ip_hdr protocol\n", __func__, iphdr.protocol);
        return FALSE;
    }
    udp_hdr = (udp_hdr_t *)(ip_hdr + 1);
    
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: udp mem addr 0x%x\n", __func__, (int)udp_hdr);
    memcpy(&udphdr, udp_hdr, sizeof(udp_hdr_t));
    if((ntohs(udphdr.sport) == DHCP_UDP_SRC_PORT && ntohs(udphdr.dport) == DHCP_UDP_DST_PORT) ||
        (ntohs(udphdr.dport) == DHCP_UDP_SRC_PORT && (udphdr.sport) == DHCP_UDP_DST_PORT)){
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: DHCP packet detected\n", __func__);
        return TRUE;
    }
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: udp source/destination port %d/%d\n",
        __func__, udphdr.sport, udphdr.dport);
    return FALSE;
}
*/
/*
*   PROTOTYPE    cs_uint8 *app_ipintf_get_my_macaddr(void)
*   INPUT            None
*   OUTPUT         MAC address of current ONU.
*   Description     Get my mac address.
*   Pre-condition  The ip stack interface module should be already initialized.
*   Post-condition
*   Note  
*/
const cs_uint8 *app_ipintf_get_my_macaddr(void)
{
    return &(ipintf_info.mac[0]);
}

/*
*   PROTOTYPE    void app_ipintf_init_pkt(cs_uint32 len,cs_pkt_t *pkt)
*   INPUT            len  total length of packet include header and payload.
*                       pkt  
*   OUTPUT         None.
*   Description     Set default value to the fields of pkt. 
*   Pre-condition  
*   Post-condition
*   Note             
*/
void app_ipintf_init_pkt(cs_uint32 len,cs_pkt_t *pkt)
{
    memset(pkt,0,len);
    pkt->offset = CS_PKT_OFFSET;
    pkt->len = len-CS_PKT_OFFSET-(sizeof(cs_pkt_t)-1);
}

/*
*   PROTOTYPE    cs_boolean app_ipintf_macaddr_equal(const cs_uint8 *,const cs_uint8 *,cs_uint8 )
*   INPUT            
*   OUTPUT         None.
*   Description     If two mac address are equal, return TRUE, otherwise, return FALSE.
*   Pre-condition  
*   Post-condition
*   Note  
*/
cs_boolean app_ipintf_macaddr_equal(const cs_uint8 *a, const cs_uint8 *b, cs_uint8 len)
{
    cs_uint8 i;

    if (!a || !b) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%s, null pointer\n", __func__);
        return FALSE;
    }

    if (len > 6)
        len = 6;

    for (i=0; i<len; i++) {
        if (a[i] != b[i])
            return FALSE;
    }
    
    return TRUE;
    
}

/*
*   PROTOTYPE    cs_boolean app_ipintf_equal_my_ipaddr(cs_uint32 target_ip)
*   INPUT            target_ip
*   OUTPUT         None.
*   Description     If target ip equal to local ip, return TRUE, otherwise, return FALSE.
*   Pre-condition  
*   Post-condition
*   Note  
*/
cs_boolean app_ipintf_my_ipaddr_equal(cs_uint32 target_ip)
{
	#ifdef HAVE_TCP_UDP_NETWORK_CARD
	//added by zhuxh
	cs_uint32	eth0_ip = 0;
	cs_uint32	eth0_mask = 0;
	cs_uint32	eth0_gateway = 0;
	cs_uint32	eth1_ip = 0;
	cs_uint32	eth1_mask = 0;
	cs_uint32	eth1_gateway = 0;
	app_ipintf_get_ip_address(&eth0_ip, &eth0_mask, &eth0_gateway);
	network_card_eth1_ip_get(&eth1_ip, &eth1_mask, &eth1_gateway);
	if((eth0_ip == target_ip) || (eth1_ip == target_ip))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	#else
    if(LOCAL_IPADDR == target_ip)
        return TRUE;
    else
        return FALSE;
	#endif
}

cs_int32 app_ipintf_mac2port_get(cs_uint8 *mac)
{
    cs_uint32 i = 0;

    for(i = 0; i < IPINTF_MAC_ENTRY_MAX; i++) {
        if(MACTBL[i].used) {
            /* Have already had this mac, just update the entry. */
            if(app_ipintf_macaddr_equal(MACTBL[i].mac, mac, 6)) {
                return MACTBL[i].dpid;
            }
        }
    }
    
    return -1;
    
}

/*
*   PROTOTYPE    void app_ipintf_port_mac_flush(cs_port_id_t port)
*   INPUT            port
*   OUTPUT         None.
*   Description     Flush all mac address entries on this port.
*   Pre-condition  
*   Post-condition
*   Note  
*/
void app_ipintf_port_mac_flush(cs_port_id_t port)
{
    cs_uint32 i = 0;
    if(port >= ipintf_info.maxport) {
         APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"%s, invalid port %d\n", __func__, port);
         return;
    }
    
    for(i = 0; i < IPINTF_MAC_ENTRY_MAX; i++) {
        if(MACTBL[i].dpid == port) {
            MACTBL[i].used = 0;
        }
    }

    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s, port %d\n", __func__, port);
    
    return;
    
}

/*
*   PROTOTYPE    void app_ipintf_mac_aging(void)
*   INPUT            None
*   OUTPUT         None.
*   Description     Age mac table.
*   Pre-condition  
*   Post-condition
*   Note             This functions is called periodically.
*/
void app_ipintf_mac_aging(void)
{
    cs_uint32 i = 0;

    for(i = 0; i < IPINTF_MAC_ENTRY_MAX; i++) {
        if(MACTBL[i].hit == 0) {
            MACTBL[i].used = 0;
        }
        MACTBL[i].hit = 0;
    }
    
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s.\n", __func__);
    
    return;
    
}

/*
*   PROTOTYPE    void app_ipintf_mac_learning(cs_pkt_t *pkt)
*   INPUT            pkt
*   OUTPUT         None.
*   Description     Learn source mac address.
*                      If mac address has already been in the table, update the "hit" flag.
*                      otherwise, add source mac to the table if the table is not full.
*   Pre-condition  
*   Post-condition
*   Note             
*/
void app_ipintf_mac_learning(cs_pkt_t *pkt)
{
    cs_uint32 i = 0;
    epon_macaddr_header_t *macHdr = NULL;

    /* sanity check */
    if(pkt->port >= ipintf_info.maxport) {
         APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s, invalid port %d\n", __func__, pkt->port );
         return;
    }
    
    macHdr =(epon_macaddr_header_t*)(pkt->data+pkt->offset);
    for(i = 0; i < IPINTF_MAC_ENTRY_MAX; i++) {
        if(MACTBL[i].used) {
            /* Have already had this mac, just update the entry. */
            if(app_ipintf_macaddr_equal(MACTBL[i].mac, macHdr->src, 6)) {
                MACTBL[i].vlanid = (pkt->svlan&0xfff);
                MACTBL[i].dpid = pkt->port;
                MACTBL[i].hit = 1;
                return;
            }
        }
    }

    /* mac learning */
    for(i = 0; i < IPINTF_MAC_ENTRY_MAX; i++) {
        if(MACTBL[i].used == 0) {
            MACTBL[i].used = 1;
            MACTBL[i].hit = 1;
            MACTBL[i].vlanid = pkt->svlan & 0xfff;
            MACTBL[i].dpid =  pkt->port;
            memcpy(MACTBL[i].mac, macHdr->src, 6*sizeof(cs_uint8));
            return;
        }
    }

    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s, mac table is full\n", __func__);
    
}

/*
*   PROTOTYPE    cs_status app_ipintf_link_status_chk(cs_port_id_t dport)
*   INPUT            dport
*   OUTPUT         None.
*   Description    Check the port link status.
*                      If it is active, return 0, otherwise, return -1.
*   Pre-condition  
*   Post-condition
*   Note             For PON port, if OAM discovery is not finished, return -1.
*/
cs_status app_ipintf_link_status_chk(cs_port_id_t dport)
{
    cs_callback_context_t     context;
    
    if(dport == CS_PON_PORT_ID) {
        if(!(oam_sm_discovery_completed(0))) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s, PON port is not active\n", __func__);
            return CS_E_ERROR;
        }
    }
    else {
        cs_sdl_port_link_status_t portStatus = SDL_PORT_LINK_STATUS_DOWN;
        epon_request_onu_port_link_status_get(context, ONU_DEVICEID_FOR_API,
                                                        			ONU_LLIDPORT_FOR_API,dport,&portStatus);
        if(!portStatus) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s, port %d is not active\n", __func__, dport);
            return CS_E_ERROR;
        }
    }
    
    return CS_E_OK;
    
}

/*
*   PROTOTYPE    cs_status send_pkt_with_vlan_check(cs_port_id_t dport, cs_pkt_t *pkt)
*   INPUT            dport
*                       pkt
*   OUTPUT         None.
*   Description     Send packet out.
*                       Before sending it out, apply vlan egress checking.
*   Pre-condition  
*   Post-condition
*   Note             
*/
cs_status app_ipintf_send_pkt_with_vlan_check(cs_port_id_t dport, cs_pkt_t *pkt, cs_uint8 *buf)
{
    cs_status ret = CS_E_OK;
    cs_uint8 priority = 7;
    cs_uint8 *data = NULL;
    //cs_uint8 *buf = NULL;
    cs_uint32 len = 0;
    cs_callback_context_t     context;

    if(app_ipintf_link_status_chk(dport)) {
        return CS_E_ERROR;
    }
    len = pkt->len;    
    data = pkt->data + pkt->offset;
    ret = vlan_egress_handle(pkt->port, dport, data, buf, &len);
    if(ret == CS_E_ERROR) {
        return CS_E_ERROR;
    }
    
    if(ret == CS_E_OK) {
        ret = epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
		                                                  data, len<60?60:len, dport, priority);
    }
    else {
        ret = epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
		                                                  buf, len<60?60:len, dport, priority);

    }

    IPINTF_STATIS_INC(SEND, dport);
    return ret;
    
}

/*
*   PROTOTYPE    cs_status send_pkt(cs_port_id_t dport, cs_pkt_t *pkt)
*   INPUT            dport
*                       pkt
*   OUTPUT         None.
*   Description     Send packet out without vlan egress checking.
*   Pre-condition  
*   Post-condition
*   Note             
*/
cs_status app_ipintf_send_pkt(cs_port_id_t dport, cs_pkt_t *pkt)
{
    cs_status rc = CS_E_OK;
    cs_uint8 priority = 7;
    cs_uint8 *hdr = NULL;
    cs_uint32 len = 0;
    cs_callback_context_t     context;

    if(app_ipintf_link_status_chk(dport)) {
        IPINTF_STATIS_INC(DROP, dport);
        return CS_E_OK;
    }
    hdr = ETH_GET_FRAME_BUFFER(pkt);
    len = pkt->len;
    
    rc = epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
		                                                  hdr,len<60?60:len,dport,priority);
    IPINTF_STATIS_INC(SEND, dport);
    //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s, dport %d,len %d\n", __func__, dport, len);
    return rc;
    
}

/*
*   PROTOTYPE    cs_status app_ipintf_pkt_send(cs_pkt_t* pkt, cs_uint32 len)
*   INPUT            pkt
*   OUTPUT         None.
*   Description    
*   Pre-condition  
*   Post-condition
*   Note : when app_ipintf_arp_egress_filter return code != 0, it means the whole 
*            process is finished or error, we should return from here.
*            If return code == 0, we should not return and continue the following code. 
*/
cs_status app_ipintf_packet_tx(cs_pkt_t* pkt)
{
    cs_uint16 i = 0;
    cs_uint32 len = pkt->len;
    cs_uint8 *hdr = NULL;
    app_vlan_double_tag_header_t eth_header;
        
    hdr = pkt->data + pkt->offset;
    if(BITON(hdr[0],0)) {
        memset(&eth_header, 0, sizeof(app_vlan_double_tag_header_t));
        vlan_header_parse(hdr, &eth_header);
        if(eth_header.ethertype == EPON_ETHERTYPE_ARP) {
            if(app_ipintf_arp_tx_filter) {
                if(app_ipintf_arp_tx_filter(pkt) == IPINTF_DROP) {
                    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s drop, arp tx filter\n", __func__);
                    return CS_E_ERROR;
                }
            }
         }
        /* flooding packets to all ports */
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "BC flooding, len %d\n", len);
#ifdef HAVE_ZTE_SIJIE
        push_vlan_ingress(EOC_PON_VLAN_ID, 0, pkt);
#endif /*END_HAVE_ZTE_SIJIE*/
        for(i=0;i<ipintf_info.maxport;i++) {
            /* if is in IPOE testing state, sending packet to UNI port ONLY */
            //if(PORT_IPOE_INPROGESS(ipintf_info.port[i].port) 
            if (IPOE_INPROGESS()){
                if(app_dhcp_packet_parse(pkt)) {
                    //app_dhcp_clr_srcip(pkt)
                }
                APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"%s: send BC DHCP with vlan %d to port %d, %d\n", 
                    __func__, g_ipoe_state.vlan, g_ipoe_state.port, __LINE__);
                push_vlan_ingress(g_ipoe_state.vlan, 0, pkt);
                app_ipintf_send_pkt(g_ipoe_state.port,pkt);
                break;
            }
            app_ipintf_send_pkt(PTABLE[i].port,pkt);
        }
    }
    else
    {
        for(i=0; i<IPINTF_MAC_ENTRY_MAX; i++) {
            if(IPOE_INPROGESS()) {
                if(app_dhcp_packet_parse(pkt)) {
                    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: send UC DHCP with vlan %d to port %d, %d\n", 
                        __func__, g_ipoe_state.vlan, g_ipoe_state.port, __LINE__);
                    push_vlan_ingress(g_ipoe_state.vlan, 0, pkt);
                    app_ipintf_send_pkt(g_ipoe_state.port, pkt);
                    return CS_E_OK;
                }
            }

            if(MACTBL[i].used && app_ipintf_macaddr_equal(hdr, MACTBL[i].mac, 6)) {
                /* For known unicast packet, push vlan and pri */
                if(MACTBL[i].vlanid != 0) {
                    push_vlan_ingress(MACTBL[i].vlanid, 0, pkt);
                }
                //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "UC send to %d, len %d\n", MACTBL[i].dpid, pkt->len);
                return app_ipintf_send_pkt(MACTBL[i].dpid, pkt);
            }
        }
        /* unknown unicast : flooding */
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "DLF UC flooding , len %d\n", pkt->len);
        for(i=0;i<ipintf_info.maxport;i++) {
            app_ipintf_send_pkt(PTABLE[i].port,pkt);
        }
    }

    return CS_E_OK;
    
}

/*
*   PROTOTYPE    cs_status app_ipintf_ip_pre_process(cs_pkt_t* pkt)
*   INPUT            pkt
*   OUTPUT         None.
*   Description     For different customers, there may be different process for IP 
*                       packet before sending to IP stack.
*                       This function calls the special process rountine.
*   Pre-condition  
*   Post-condition
*   Note             e.g. for NTT, all IP packets should be discarded except http packets.
*/
cs_status app_ipintf_ip_pre_process(cs_pkt_t* pkt)
{        
    IPINTF_STATIS_INC(IP,pkt->port);
    if(app_ipintf_ip_rx_filter == NULL ) {
        return CS_E_OK;
    }
    
    if(app_ipintf_ip_rx_filter(pkt) == IPINTF_DROP) {
        return CS_E_ERROR;
    }
    
    return CS_E_OK;

}

/*
*   PROTOTYPE    cs_uint8 app_ipintf_pkt_recv(cs_pkt_t* pkt)
*   INPUT            pkt
*   OUTPUT         None.
*   Description     receive packet from HW.
*   Pre-condition  
*   Post-condition
*   Note             special: only packets from PON need vlan id check
*                      general: allow ARP from GE/FE, without checking vlanid  
*/
cs_uint8 app_ipintf_pkt_recv(cs_pkt_t* pkt)
{
    cs_status rc = CS_E_OK;
    cs_uint32 len = 0;
    cs_uint8 *pbuf = NULL;

    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"%s, sport %d, len %d, vlan %d, type %d\n", 
        __func__, pkt->port, pkt->len, pkt->svlan, pkt->eth_type);
    
    IPINTF_STATIS_INC(RECV, pkt->port);
    //app_ipintf_mac_learning(pkt);
#ifdef DHCP_OPTION_PPPOE_PLUS_SUPPORT    
    if(pkt->u2Reserved & IPVX_DHCP_MASK) {
        if(pkt->port == 0) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%s: recv DHCP from PON port\n", __func__);
        }
        else if(pkt->u2Reserved == IPV4_DHCP) {
            return dhcpv4_add(pkt);
        }
        else if(pkt->u2Reserved == IPV6_DHCP) {
            return dhcpv6_add(pkt);
        }
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%s: invalid packet type %hd\n", __func__, pkt->pkt_type);
    }
#endif    
    if(pkt->eth_type == EPON_ETHERTYPE_ARP) {
        rc = app_ipintf_arp_pre_process(pkt);
    }
    else {
        rc = app_ipintf_ip_pre_process(pkt);
    }

    if(rc != 0) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"%s, does not send to IP stack, 0x%x\n", __func__, (int)pkt);
        IPINTF_STATIS_INC(DROP, pkt->port);
        return rc;
    }
    
    app_ipintf_mac_learning(pkt);
    if(pkt->svlan) {
        len = (cs_uint32)pkt->len;
        pbuf = ETH_GET_FRAME_BUFFER(pkt);
        pop_vlan(pbuf,pbuf,&len);
        //pkt->stpid = 0;
        //pkt->svlan = 0;
        pkt->len = len;
        if(pkt->tag_num >= 1)
            pkt->tag_num -= 1;
    }
    //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"%s, forward pkt to IP stack, len %d\n", __func__, pkt->len);
    netdev_pkt_recv(pkt);
	#ifdef HAVE_TCP_UDP_NETWORK_CARD
	//added by zhuxh
	netdev_eth1_pkt_recv(pkt);
	#endif
	
    return CS_E_OK;
 
}

/*
*   PROTOTYPE    void ethdrv_tm_handle(void)
*   INPUT            None
*   OUTPUT         None.
*   Description     timer routine.
*                       every ETH_TM_TICKS, this function will be called.
*   Pre-condition  
*   Post-condition
*   Note             
*/
void ethdrv_tm_handle(void)
{
    static cs_uint32 ticks = 0;
    
    ticks += IPINTF_TM_TICKS;

    /* mac address aging */
    if(ticks % ipintf_info.ageinterval == 0) {
        app_ipintf_mac_aging();
    }
    
    /* arp keep alive */
    if(ticks % ipintf_info.keepalive == 0) {
        app_ipintf_arp_keep_alive();
    }
    
  //  APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s, %d seconds\n", __func__, ticks);
    
}

/*
*   PROTOTYPE    cs_status app_ipintf_set_wan_cfg(cs_uint8,cs_uint8,cs_uint16,cs_uint32,cs_uint32)
*   INPUT            enable
*                       pri
*                       vlanid  
*                       remote_subnet  
*                       subnet_mask  
*   OUTPUT         None.
*   Description     
*   Pre-condition  
*   Post-condition
*   Note             
*/
cs_status app_ipintf_set_wan_cfg(
                                                    cs_uint8 enable, 
                                                    cs_uint8 pri,
                                                    cs_uint16 vlanid,
                                                    cs_uint32 remote_subnet, 
                                                    cs_uint32 subnet_mask,
                                                    cs_uint32 gw)
{
    cs_callback_context_t     context;

    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"%s inband, pri %d, vlan %d - 0x%08x, 0x%08x\n", 
        enable==APP_IPINTF_INBNAD_MGNT?"enable":"disable", pri,vlanid,remote_subnet, subnet_mask);
    app_ip_changed = 1;
    ipintf_info.inband = enable==APP_IPINTF_INBNAD_MGNT ? 1:0;
    ipintf_info.keepalive = ipintf_info.inband?(IPINTF_ARP_KEEPALIVE_ZTE):(IPINTF_ARP_KEEPALIVE_DFT);
    ipintf_info.pri = pri;
    ipintf_info.vlanid = vlanid;
    ipintf_info.remoteip = remote_subnet;
    ipintf_info.remotemask = subnet_mask;
    ipintf_info.gateway = gw;
    if(ipintf_info.inband) {
        epon_request_onu_spec_pkt_state_set(context, 0, 0, S_DOWNSTREAM, CS_PKT_ARP, DST_CPU);
        epon_request_onu_spec_pkt_state_set(context, 0, 0, S_UPSTREAM, CS_PKT_ARP, DST_FE);
        epon_request_onu_spec_pkt_state_set(context, 0, 0, S_DOWNSTREAM, CS_PKT_MYMAC, DST_CPU);
        epon_request_onu_spec_pkt_state_set(context, 0, 0, S_UPSTREAM, CS_PKT_MYMAC, DST_FE);
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"%s, down MYMAC/ARP : CPU, up MYMAC/ARP : FE, \n", __func__);

        app_ipintf_arp_keep_alive();
    }
    else {
        epon_request_onu_spec_pkt_state_set(context, 0, 0, S_DOWNSTREAM, CS_PKT_ARP, DST_FE);
        epon_request_onu_spec_pkt_state_set(context, 0, 0, S_UPSTREAM, CS_PKT_ARP, DST_CPU);
        epon_request_onu_spec_pkt_state_set(context, 0, 0, S_DOWNSTREAM, CS_PKT_MYMAC, DST_FE);
        epon_request_onu_spec_pkt_state_set(context, 0, 0, S_UPSTREAM, CS_PKT_MYMAC, DST_CPU);
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"%s, down MYMAC/ARP : FE, up MYMAC/ARP : CPU, \n", __func__);

    }
    
    return CS_E_OK;
    
}

/*
*   PROTOTYPE    cs_status app_ipintf_get_ip_address(cs_uint32 *, cs_uint32 *, cs_uint32 *)
*   INPUT            None
*   OUTPUT         *local_ip
*                       *mask
*                       *gw_ip
*   Description     
*   Pre-condition  
*   Post-condition
*   Note             
*/
cs_status app_ipintf_get_ip_address(cs_uint32 *local_ip, cs_uint32 *mask, cs_uint32 *gw_ip)
{
    /*
    *local_ip = ipintf_info.localip;
    *mask = ipintf_info.ipmask;
    *gw_ip = ipintf_info.gateway;
    */
    memcpy(local_ip, &ipintf_info.localip, sizeof(cs_uint32));
    memcpy(mask, &ipintf_info.ipmask, sizeof(cs_uint32));
    memcpy(gw_ip, &ipintf_info.gateway, sizeof(cs_uint32));
    return CS_E_OK;
    
}

/*
*   PROTOTYPE    cs_status app_ipintf_get_wan_cfg(cs_uint8*,cs_uint8*,cs_uint16*,cs_uint32*,cs_uint32*)
*   INPUT           None.
*   OUTPUT         *enable
*                       *pri
*                       *vlanid  
*                       *remote_subnet  
*                       *subnet_mask  
*   Description     
*   Pre-condition  
*   Post-condition
*   Note             
*/
cs_status app_ipintf_get_wan_cfg(
                                                    cs_uint8 *enable, 
                                                    cs_uint8 *pri,
                                                    cs_uint16 *vlanid,
                                                    cs_uint32 *remote_subnet, 
                                                    cs_uint32 *remote_mask,
                                                    cs_uint32 *gw)
{
    cs_uint8 inband = ipintf_info.inband?APP_IPINTF_INBNAD_MGNT:APP_IPINTF_OUTBNAD_MGNT;
    memcpy(enable, &inband, sizeof(cs_uint8));
    memcpy(pri, &ipintf_info.pri, sizeof(cs_uint8));
    memcpy(vlanid, &ipintf_info.vlanid, sizeof(cs_uint16));
    memcpy(remote_subnet, &ipintf_info.remoteip, sizeof(cs_uint32));
    memcpy(remote_mask, &ipintf_info.remotemask, sizeof(cs_uint32));
    memcpy(gw, &ipintf_info.gateway, sizeof(cs_uint32));
    
    return CS_E_OK;
    
}

/*
*   PROTOTYPE    cs_status app_ipintf_get_mtu(cs_uint32 *mtu)
*   INPUT           None.
*   OUTPUT         *mtu
*   Description     
*   Pre-condition  
*   Post-condition
*   Note             
*/
cs_status app_ipintf_get_mtu(cs_uint32 *mtu)
{
    memcpy(mtu, &ipintf_info.mtu, sizeof(cs_uint32));
    return CS_E_OK;
}

cs_status app_ipintf_check_ip_address(cs_uint32 ip, cs_uint32 mask, cs_uint32 gw)
{
    cs_int32 lan_len = 0;
    cs_int32 non_zero_num = 0;
    cs_int32 i;
    cs_int32 mask_1 = 0;

    /* check ip address validity */
    if( ip == 0 || ip == (cs_uint32)-1) {
        return 1;
    }

    /* check multicast address */
    if((ip >> 28) == 0xe) {
        return 1;
    }

    /* check loopback address(127.0.0.1) */
    if(ip == 0x7f000001) {
        return 1;
    }

    /* check netmask validity */
    if( mask == 0 || mask == 0xffffffff ) {
        return 1;
    }
    for(i=0; i<32; i++) {
        if(mask & (1<<i)) {
            mask_1 = i;
            break;
        }
    }
    for(i=mask_1; i<32; i++) {
        if(mask & (1<<i)) {
            continue;
        }
        return 1;
    }

    /* check subnet address */
    if(0 == (ip & (~mask)) ) {
        return 1;
    }

    /* check broadcast address */
    for(i=0; i<32; i++) {
        if(mask & (1<<i)) {
            continue;
        }

        lan_len++;
        if(ip & (1<<i)) {
            non_zero_num++;
        }
    }

    if(lan_len == non_zero_num) {
        return 1;
    }

    /* all 0 is subnet address */
    for(i=0; i<lan_len; i++) {
        if( (ip>>i) & 1) {
            non_zero_num++;
        }
    }

    if(non_zero_num == 0) {
        return 1;
    }

    /* input gateway, need to check gateway validity */
    if(gw) {
        if( (gw&mask) != (ip&mask) ) {
            return 1;
        }

        non_zero_num = 0;
        for(i=0; i<lan_len; i++) {
            if( (gw>>i) & 1) {
                non_zero_num++;
            }
        }

        // gw ip should not be brocast or all zero
        if(non_zero_num == lan_len || non_zero_num == 0) {
            return 1;
        }
    }

    return 0;
    
}    


cs_status app_ipintf_arp_keepalive_interval_set(cs_uint32 interval)
{
    if(interval == 0)
        ipintf_info.keepalive = IPINTF_ARP_KEEPALIVE_DFT;
    else
        ipintf_info.keepalive = interval;
    
    return CS_E_OK;
    
}

cs_status app_ipintf_arp_del(cs_uint8 *host)
{
    return ipintf_arp_del(host);
}

cs_status app_ipintf_arp_show(cs_uint8 *host)
{
    return ipintf_arp_show(host);
}

/*
*   PROTOTYPE    cs_status app_ipintf_delete_ip_address(cs_uint32 ip_addr, cs_uint32 mask)
*   INPUT            ip_addr
*                       mask  
*   OUTPUT         None.
*   Description     
*   Pre-condition  
*   Post-condition
*   Note             
*/
cs_status app_ipintf_delete_ip_address(cs_uint32 ip_addr, cs_uint32 mask)
{
    return ipintf_delete_ip_address(ip_addr, mask);
}

/*
*   PROTOTYPE    cs_status app_ipintf_add_ip_address(cs_uint32 localIp, cs_uint32 gwIp, cs_uint32 mask)
*   INPUT            localIp
*                       gwIp    gateway ip address
*                       mask  
*   OUTPUT         None.
*   Description     
*   Pre-condition  
*   Post-condition
*   Note             
*/
cs_status app_ipintf_add_ip_address(cs_uint32 localIp, cs_uint32 gwIp, cs_uint32 mask)
{
    return ipintf_add_ip_address(localIp, gwIp, mask);
}

/*
*   PROTOTYPE    cs_status app_ipintf_set_mtu(cs_uint32 mtu)
*   INPUT            mtu
*   OUTPUT         None.
*   Description     
*   Pre-condition  
*   Post-condition
*   Note             
*/
cs_status app_ipintf_set_mtu(cs_uint32 mtu)
{
    return ipintf_set_mtu(mtu);
}

cs_status app_ipintf_ip_config_set(cs_uint32 local_ip, cs_uint32 ip_mask, cs_uint32 gw_ip)
{
    cs_status ret = CS_E_OK;

    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "Config ONU IP - 0x%08x, 0x%08x, 0x%08x\n", 
                                                        local_ip, ip_mask, gw_ip);
    /* check parameter sanity 
    ret = app_ipintf_check_ip_address(local_ip, ip_mask, gw_ip);
    if(ret != 0) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%s, invalid IP parameter\n", __func__);
        return CS_E_ERROR;
    }*/

    if((local_ip == ipintf_info.localip) &&
        (ip_mask == ipintf_info.ipmask)&&
        (gw_ip == ipintf_info.gateway)) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "IP does change - ip 0x%08x, mask 0x%08x, gw 0x%08x\n", 
                                                        ipintf_info.localip, ipintf_info.ipmask, ipintf_info.gateway);
        return CS_E_OK;
    }
    
    /* delete current ip address */
    ret = app_ipintf_delete_ip_address(ipintf_info.localip, ipintf_info.ipmask);
    if(ret != 0) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "Failed to delete IP - 0x%08x, 0x%08x\n", 
                                                        ipintf_info.localip, ipintf_info.ipmask);
    }
    
    /* add new ip addr */
    ret = app_ipintf_add_ip_address(local_ip, gw_ip, ip_mask);
    if(ret != 0) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"Failed to config ONU IP - 0x%08x, 0x%08x, 0x%08x\n", 
                                                        local_ip, ip_mask, gw_ip);
        return CS_E_ERROR;
    }
    app_ip_changed = 1;
    return CS_E_OK;
}
cs_status app_ipintf_ping(char *host)
{
    return ipintf_ping(host);
}

void app_ipintf_mac_table_dump(cs_uint32 port)
{
    cs_uint32 i = 0;
    cs_printf("ARP table:\n");
    for(i=0; i<IPINTF_MAC_ENTRY_MAX; i++){
        if(MACTBL[i].used) {
            cs_printf("index = %2d: used %d,hit %d, vlanid %4d, dpid %d,"
                "ESA = %02X %02X %02X %02X %02X %02X\n",i,
                MACTBL[i].used,MACTBL[i].hit, MACTBL[i].vlanid, MACTBL[i].dpid,
                MACTBL[i].mac[0],MACTBL[i].mac[1],MACTBL[i].mac[2],
                MACTBL[i].mac[3],MACTBL[i].mac[4],MACTBL[i].mac[5]);
        }
    }
}

void app_ipintf_reg_event_cb(cs_uint16 dev, cs_int32 type, void* msg)
{
    sdl_event_reg_t *reg;
    if(NULL == msg)
        return;
    
    //app_ipintf_port_mac_flush(CS_PON_PORT_ID);
    reg = (sdl_event_reg_t*)msg;
    if(reg->reg) {/*
        if(app_ipintf_enable_inband)
            app_ipintf_enable_inband(); */
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"No need to set in/out band mode \n");

    }
    else {
        if(app_ipintf_enable_outband)
            app_ipintf_enable_outband();
    }
    
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"pon %s \n", reg->reg?"register":"deregister");
    
}
#endif
#ifdef DHCP_OPTION_PPPOE_PLUS_SUPPORT 
cs_int32 ipv6_option_parse(const cs_pkt_t *pkt)
{
    cs_uint8 *nh = NULL;
    cs_uint32 tol_len = 0;
    cs_uint32 off = 0;
    cs_uint32 len = 0;
    cs_uint8 nexthdr = 0;
    ipv6_hdr_t *ipv6_hdr;

    if(!pkt)
        return 0;

    nh = (cs_uint8 *)(pkt->data+pkt->offset+sizeof(cs_ether_header_t)+(pkt->tag_num<<2));
    ipv6_hdr = (ipv6_hdr_t *)nh;
    nexthdr = ipv6_hdr->next;
    tol_len= ipv6_hdr->len - sizeof(ipv6_hdr_t);
    nh = (cs_uint8 *)(ipv6_hdr+1);

    while (off < tol_len) {
        cs_uint8 *ptr = NULL;
        if(nexthdr == IP_UDP) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%s: find UDP \n", __func__);
            break;
        }
        off += len;
        ptr = nh + off;
        switch(nexthdr) {
            case OPT_DST_OPT_HDR:
                nexthdr = ptr[0];
                len = (ptr[1] + 1) << 3;
                APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%s: OPT_DST_OPT_HDR detected \n", __func__);
                break;
            case OPT_ROUTING_HDR:
                nexthdr = ptr[0];
                len = (ptr[1] + 1) << 3;
                APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%s: OPT_ROUTING_HDR detected \n", __func__);
                break;
            case OPT_AUTHENTICATION_HDR:
                nexthdr = ptr[0];
                len = (ptr[1] + 2) << 2;
                APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%s: OPT_AUTHENTICATION_HDR detected \n", __func__);
                break;
            case  OPT_HOP_BY_HOP:
                //fall through
            case  OPT_FRAGMENT_HDR:
                //fall through
            case  OPT_ESP_HDR:
                nexthdr = ptr[0];
                len = (ptr[1] + 1) << 3;
                break;
            default:
                APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"%s: 0x%2x detected \n", __func__, nexthdr);
                return -1;
        }
    }
    if(off >= tol_len) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%s: NO UDP detected, offset %d, total len %d \n", __func__, off, tol_len);
        return 0;
    }
    return off;
}



//DHCP options start
#include "plat_common.h"
#include "sdl_api.h"
//#include "lwip/dhcp.h"
#include "app_ip_types.h"
#include "app_ip_util.h"


#include "packet.h"

/** BootP options */
#define DHCP_OPTION_PAD 0
#define DHCP_OPTION_SUBNET_MASK 1 /* RFC 2132 3.3 */
#define DHCP_OPTION_ROUTER 3
#define DHCP_OPTION_DNS_SERVER 6 
#define DHCP_OPTION_HOSTNAME 12
#define DHCP_OPTION_IP_TTL 23
#define DHCP_OPTION_MTU 26
#define DHCP_OPTION_BROADCAST 28
#define DHCP_OPTION_TCP_TTL 37
#define DHCP_OPTION_60 60
#define DHCP_OPTION_125 125

#define DHCP_OPTION_END 255

#define DHCPV6_OPTION_18 18
#define DHCPV6_OPTION_37 37
/** DHCP options */
#define DHCP_OPTION_REQUESTED_IP 50 /* RFC 2132 9.1, requested IP address */
#define DHCP_OPTION_LEASE_TIME 51 /* RFC 2132 9.2, time in seconds, in 4 bytes */
#define DHCP_OPTION_OVERLOAD 52 /* RFC2132 9.3, use file and/or sname field for options */

#define DHCP_OPTION_MESSAGE_TYPE 53 /* RFC 2132 9.6, important for DHCP */
#define DHCP_OPTION_MESSAGE_TYPE_LEN 1


#define DHCP_OPTION_SERVER_ID 54 /* RFC 2132 9.7, server IP address */
#define DHCP_OPTION_SERVER_ID_LEN 4


#define DHCP_OPTION_PARAMETER_REQUEST_LIST 55 /* RFC 2132 9.8, requested option types */
#define DHCP_OPTION_PARAMETER_REQUEST_LIST_LEN  1 /* minimum */


#define DHCP_OPTION_MAX_MSG_SIZE 57 /* RFC 2132 9.10, message size accepted >= 576 */
#define DHCP_OPTION_MAX_MSG_SIZE_LEN 2

#define DHCP_OPTION_T1 58 /* T1 renewal time */
#define DHCP_OPTION_T2 59 /* T2 rebinding time */
#define DHCP_OPTION_60 60
#define DHCP_OPTION_60_LEN 128
#define DHCP_OPTION_82 82

#define DHCP_OPTION_CLIENT_ID 61
#define DHCP_OPTION_TFTP_SERVERNAME 66
#define DHCP_OPTION_BOOTFILE 67

/** possible combinations of overloading the file and sname fields with options */
#define DHCP_OVERLOAD_NONE 0
#define DHCP_OVERLOAD_FILE 1
#define DHCP_OVERLOAD_SNAME  2
#define DHCP_OVERLOAD_SNAME_FILE 3

gwd_dhcp_access_identifier_t g_dhcp_access_identifier;
cs_uint8 g_onu_identifier[128] = {0};
cs_uint8 g_onu_id[50] = {0};
static cs_uint8 pkt_dummy[1600];
//{0x01, 0x80, 0xc2, 0x00, 0x00, 0x23}
const cs_uint8 bcmp_dmac[] = {0x01, 0x80, 0xc2, 0x00, 0x00, 0x23};

dhcp_option_cb_t dhcp_option_cb;
#define dhcp_my_mac_get() \
     (dhcp_option_cb.mac)
extern cs_status app_ipintf_ip_pkt_parser(cs_pkt_t *pPkt);

cs_status app_ipintf_bcmp_pkt_parser(cs_pkt_t *pPkt)
{
#ifdef DHCP_OPTION_PPPOE_PLUS_SUPPORT
    if(EPON_ETHERTYPE_BCMP == pPkt->eth_type) {
        if(dhcp_option_cb.bcmp_cb.enable) {
            IROS_LOG_MIN(IROS_MID_ETHDRV,"%s: BCMP detected\n", __func__);
            pPkt->pkt_type = CS_PKT_BCMP;
            return CS_E_OK;
        }
    }
#endif    
    return CS_E_NOT_SUPPORT;
}

cs_status local_dhcp_access_identifier_set(cs_uint8 *data)
{
	struct  access_identifier_data{
	cs_uint8 rsvd1[16];
	cs_uint8 olt_mac[CS_MACADDR_LEN*2];
	cs_uint8 rsvd2[5];
	cs_uint8 pon_slot;
	cs_uint8 rsvd3;
	cs_uint8 pon_sub_slot;
	cs_uint8 rsvd4;
	cs_uint8 pon_port;
	};

	struct  access_identifier_data access_identifier;

	int i = 0;
	
	if(NULL == data)
	{
		return CS_E_PARAM;
	}

	
	memset(&g_dhcp_access_identifier,0,sizeof(gwd_dhcp_access_identifier_t));
	memset(&access_identifier,0,sizeof(access_identifier));
	
	memcpy(&access_identifier,data,sizeof(access_identifier));
	
	for(i = 0;i < CS_MACADDR_LEN;i++)
	{
		if(gwd_is_hex_num(access_identifier.olt_mac[i*2]) && gwd_is_hex_num(access_identifier.olt_mac[i*2+1]))
		{
			g_dhcp_access_identifier.olt_mac[i] = gwd_ascii_to_num(access_identifier.olt_mac[i*2])*16 + gwd_ascii_to_num(access_identifier.olt_mac[i*2+1]);
		}
		else
		{
			return CS_E_PARAM;
		}
	}
	
	g_dhcp_access_identifier.pon_slot = access_identifier.pon_slot;
	g_dhcp_access_identifier.pon_sub_slot = access_identifier.pon_sub_slot;
	g_dhcp_access_identifier.pon_port = access_identifier.pon_port;
	
	return CS_E_OK;
}

cs_status olt_mac_get_from_local_dhcp_access_identifier(cs_uint8 *mac)
{
	if(NULL == mac)
	{
		return CS_E_PARAM;
	}

	memcpy(mac,g_dhcp_access_identifier.olt_mac,CS_MACADDR_LEN);
	
	return CS_E_OK;
}

cs_status olt_pon_port_get_from_local_dhcp_access_identifier(cs_uint8 *slot,cs_uint8 *port)
{
	if(NULL == slot || NULL == port)
	{
		return CS_E_PARAM;
	}

	*slot = gwd_ascii_to_num(g_dhcp_access_identifier.pon_slot);
	*port = gwd_ascii_to_num(g_dhcp_access_identifier.pon_port);
	
	return CS_E_OK;
}

cs_boolean app_dhcp_packet_parse(cs_pkt_t *pkt)
{
    cs_uint8 *data = pkt->data + pkt->offset + pkt->tag_num*4;
    cs_ether_header_t *eth_hdr = NULL; 
    ip_hdr_t iphdr, *ip_hdr = NULL;
    udp_hdr_t udphdr, *udp_hdr = NULL;
    eth_hdr = (cs_ether_header_t *)(data); 
    if(!IS_IP_PKT(ntohs(eth_hdr->ethertype))) {
        //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: eth type 0x%2x\n", __func__, ntohs(eth_hdr->ethertype));
        return FALSE;
    }
    ip_hdr = (ip_hdr_t *)(eth_hdr + 1);

    memcpy(&iphdr, ip_hdr, sizeof(ip_hdr_t));
    if(iphdr.protocol != IP_UDP_PROTOCOL_ID) {
        //APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: ip_hdr protocol\n", __func__, iphdr.protocol);
        return FALSE;
    }
    udp_hdr = (udp_hdr_t *)((cs_uint8*)ip_hdr + (ip_hdr->ihl <<2));
    

    memcpy(&udphdr, udp_hdr, sizeof(udp_hdr_t));
    if((ntohs(udphdr.sport) == DHCP_UDP_SRC_PORT && ntohs(udphdr.dport) == DHCP_UDP_DST_PORT) ||
        (ntohs(udphdr.dport) == DHCP_UDP_SRC_PORT && ntohs(udphdr.sport) == DHCP_UDP_DST_PORT)){
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: DHCP packet detected\n", __func__);
        return TRUE;
    }
    /*APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: udp source/destination port 0x%2x/0x%2x\n",
        __func__, udphdr.sport, udphdr.dport);*/
    return FALSE;
}

cs_boolean app_dhcpv6_packet_parse(cs_pkt_t *pkt)
{
    udp_hdr_t udp_hdr;
    cs_uint8 *data = pkt->data + pkt->offset;
    cs_uint16 sport, dport;
    cs_uint32 l2_len = 0;
    cs_uint32 ipv6_op_len = 0;
    
    ipv6_op_len = ipv6_option_parse(pkt);
    if(ipv6_op_len < 0) {
        return FALSE;
    }
    l2_len = sizeof(cs_ether_header_t) + (pkt->tag_num <<2);
    memcpy(&udp_hdr, data + l2_len+sizeof(ipv6_hdr_t) + ipv6_op_len, sizeof(udp_hdr_t));
    //udp_hdr.len = htons(pkt->len- l2_len - sizeof(ipv6_hdr_t) - ipv6_op_len);
    sport = ntohs(udp_hdr.sport);
    dport = ntohs(udp_hdr.dport);
    if((sport == DHCPV6_UDP_SRC_PORT && dport == DHCPV6_UDP_DST_PORT) ||
        (dport == DHCPV6_UDP_SRC_PORT && sport == DHCPV6_UDP_DST_PORT)){
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: DHCPV6 packet detected\n", __func__);
        return TRUE;
    }
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: sport 0x%x, dport 0x%x\n",
        __func__,sport, dport);
    return FALSE;
}

static void buffer_dump(const cs_uint8 *comment, const cs_uint8 *buf, cs_int32 len)
{
    int i;
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "\n%s ",comment);
    for(i = 0 ; i < len ; i++)
    {
        if(i%16 == 0)
        {
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "\n");
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%06x  ", i);
        }

        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%02x ",buf[i]);
    }
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "\n");
    return;
}
static void pkt_dump(const cs_uint8 *comment, const cs_uint8 *buf, cs_int32 len, cs_uint32 flags)
{
    int i;
    if(!(ipintf_info.debug & flags))
        return;
    
    cs_printf("\n%s ",comment);
    for(i = 0 ; i < len ; i++)
    {
        if(i%16 == 0)
        {
            cs_printf("\n");
            cs_printf("%06x  ", i);
        }

        cs_printf("%02x ",buf[i]);
    }
    cs_printf("\n");
    return;
}

cs_uint16 __checksum__(cs_uint16 *buffer,cs_uint32 size) 
{  
    cs_uint32 cksum = 0; 
    
    while(size>1) {  
        cksum+=*buffer++;  
        size -= sizeof(cs_uint16); 
    }  
    if(size) {  
        cksum+=*(cs_uint8 *)buffer; 
    }  

    while (cksum>>16)  
        cksum=(cksum>>16)+(cksum & 0xffff); 

    return (cs_uint16) (~cksum); 
}


 
cs_uint16 _ip_checksum(ip_hdr_t *hdr)
{
    cs_uint16 data[16];
    cs_uint16 crc = 0;
    cs_uint32 len = 0;
    
    if(!hdr)
        return 0;
    
    hdr->check = 0;
    len = hdr->ihl<<2;
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s,v %d, len %d, len %d, %d\n", __func__,hdr->version, hdr->ihl, len, __LINE__);
    memcpy(data, (cs_uint8*)hdr, len);
    crc =  __checksum__(data, len);
    hdr->check = crc;
    
    return 0;
}
 
cs_uint16 _udp_checksum(ip_hdr_t *ip_hdr)
{
    cs_uint16 buf[1500/2];
    char *ptr = (char *)buf;
    int i, chksumlen = 0;
    unsigned long zero = 0;
    udp_hdr_t *udp_hdr;
    char * payload;
    int payloadlen;
    cs_uint32 ip_hdr_len = 0;

    
    ip_hdr_len = ip_hdr->ihl << 2;
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, version %d, len %d, len %d, %d\n", __func__, ip_hdr->version, ip_hdr->ihl,ip_hdr_len,__LINE__);

    udp_hdr = (udp_hdr_t *)((cs_uint8 *)ip_hdr + ip_hdr_len);
    udp_hdr->crc = 0;
    payload = (char *)(udp_hdr + 1);
    payloadlen = ntohs(ip_hdr->tot_len) - ip_hdr_len - sizeof(udp_hdr_t);
    //
    memcpy(ptr, &ip_hdr->saddr, sizeof(ip_hdr->saddr));
    ptr += sizeof(ip_hdr->saddr);
    chksumlen += sizeof(ip_hdr->saddr);
    
    memcpy(ptr, &ip_hdr->daddr, sizeof(ip_hdr->daddr));
    ptr += sizeof(ip_hdr->daddr);
    chksumlen += sizeof(ip_hdr->daddr);

    //
    memcpy(ptr, &zero, 1);
    ptr += 1;
    chksumlen += 1;
    //
    memcpy(ptr,&ip_hdr->protocol, sizeof(ip_hdr->protocol));
    ptr += sizeof(ip_hdr->protocol);
    chksumlen += sizeof(ip_hdr->protocol);
    //
    memcpy(ptr,&udp_hdr->len,sizeof(udp_hdr->len));
    ptr += sizeof(udp_hdr->len);
    chksumlen += sizeof(udp_hdr->len);
    //
    memcpy(ptr,&udp_hdr->sport, sizeof(udp_hdr->sport));
    ptr += sizeof(udp_hdr->sport);
    chksumlen += sizeof(udp_hdr->sport);
    memcpy(ptr,&udp_hdr->dport, sizeof(udp_hdr->dport));
    ptr += sizeof(udp_hdr->dport);
    chksumlen += sizeof(udp_hdr->dport);
    //
    memcpy(ptr,&udp_hdr->len,sizeof(udp_hdr->len));
    ptr += sizeof(udp_hdr->len);
    chksumlen += sizeof(udp_hdr->len);
    //
    memcpy(ptr, &zero, sizeof(udp_hdr->crc));
    ptr += sizeof(cs_uint16);
    chksumlen += sizeof(cs_uint16);
    //
    memcpy(ptr,payload,payloadlen);
    ptr += payloadlen;
    chksumlen += payloadlen;
    //
    for(i = 0; i < payloadlen %2; i++)
    {
        *ptr = 0;
        ptr++;
        chksumlen++;
    }
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s,  %d\n", __func__, __LINE__);
    
    //
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s,  %d\n", __func__, __LINE__);
    udp_hdr->crc =  __checksum__((cs_uint16*)buf,chksumlen);
    return 0;
}

cs_uint16 ipv6_udp_checksum(cs_uint32 option_len, ipv6_hdr_t *ip_hdr)
{
    cs_uint16 buf[1500/2];
    char *ptr = (char *)buf;
    int i, chksumlen = 0;
    unsigned long zero = 0;
    udp_hdr_t *udp_hdr;
    char * payload;
    int payloadlen;

    // TODO: option should be included 
    udp_hdr = (udp_hdr_t *)((cs_uint8 *)ip_hdr + sizeof(ipv6_hdr_t)+option_len);
    udp_hdr->crc = 0;
    payload = (char *)(udp_hdr + 1);
    payloadlen = ntohs(ip_hdr->len) - option_len - sizeof(udp_hdr_t);
    // source ip
    memcpy(ptr, &ip_hdr->saddr, sizeof(ip_hdr->saddr));
    ptr += sizeof(ip_hdr->saddr);
    chksumlen += sizeof(ip_hdr->saddr);
    
    // destination ip
    memcpy(ptr, &ip_hdr->daddr, sizeof(ip_hdr->daddr));
    ptr += sizeof(ip_hdr->daddr);
    chksumlen += sizeof(ip_hdr->daddr);
	
    // payload len
    memcpy(ptr,&ip_hdr->len, sizeof(ip_hdr->len));
    ptr += sizeof(ip_hdr->len);
    chksumlen += sizeof(ip_hdr->len);
	
    // modified by wujm,next header(in this case udp,should be 2 bytes,0x00 0x11)
    *ptr = 0x00;
    ptr += 1;
    chksumlen += 1;
    memcpy(ptr,&ip_hdr->next, sizeof(ip_hdr->next));
    ptr += sizeof(ip_hdr->next);
    chksumlen += sizeof(ip_hdr->next);

	#if 0
    // udp len 
    memcpy(ptr,&udp_hdr->len,sizeof(udp_hdr->len));
    ptr += sizeof(udp_hdr->len);
    chksumlen += sizeof(udp_hdr->len);
	#endif
	
    // udp src port
    memcpy(ptr,&udp_hdr->sport, sizeof(udp_hdr->sport));
    ptr += sizeof(udp_hdr->sport);
    chksumlen += sizeof(udp_hdr->sport);

	// udp dst port
    memcpy(ptr,&udp_hdr->dport, sizeof(udp_hdr->dport));
    ptr += sizeof(udp_hdr->dport);
    chksumlen += sizeof(udp_hdr->dport);

	// udp len
    memcpy(ptr,&udp_hdr->len,sizeof(udp_hdr->len));
    ptr += sizeof(udp_hdr->len);
    chksumlen += sizeof(udp_hdr->len);
	
    // chksum
    memcpy(ptr, &zero, sizeof(udp_hdr->crc));
    ptr += sizeof(cs_uint16);
    chksumlen += sizeof(cs_uint16);

	// pay load
	payloadlen = ntohs(udp_hdr->len) - sizeof(udp_hdr->sport) - sizeof(udp_hdr->dport)
					- sizeof(udp_hdr->len) - sizeof(cs_uint16);// crc len = sizeof(cs_uint16)
	
    memcpy(ptr,payload,payloadlen);
    ptr += payloadlen;
    chksumlen += payloadlen;
    //
    for(i = 0; i < payloadlen %2; i++)
    {
        *ptr = 0;
        ptr++;
        chksumlen++;
    }
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s,  %d\n", __func__, __LINE__);
    
    //
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s,  %d\n", __func__, __LINE__);
    udp_hdr->crc =  __checksum__((cs_uint16*)buf,chksumlen);
    return 0;
}

cs_pkt_t * dhcp_packet_clone(cs_pkt_t *dst, cs_pkt_t *src)
{
    #if 0
	memcpy((cs_uint8 *)dst, (cs_uint8 *)src, src->len + src->offset + (sizeof(cs_pkt_t)-1));
	#else
	//modified by wujm@2016-04-12,incorrect len
	memcpy((cs_uint8 *)dst, (cs_uint8 *)src, src->len + src->offset + (sizeof(cs_pkt_t)));
	#endif
	return dst;
}

static cs_uint8 *dhcpv4_option_hdr_get(cs_uint8 option, cs_uint32 len, dhcpv4_hdr_t *hdr)
{
    cs_uint8 *options;
    cs_uint16 offset = 0;
    cs_uint32 overload = DHCP_OVERLOAD_NONE;

    if(!hdr)
        return NULL;

    options = hdr->options;
    while ((offset < len-sizeof(dhcpv4_hdr_t)) && (options[offset] != DHCP_OPTION_END)) {
        if (options[offset] == DHCP_OPTION_OVERLOAD) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "overloaded message detected\n");
            offset += sizeof(dhcpv4_option_hdr_t);
            overload = options[offset++];
        }
        else if (options[offset] == option) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "option found at offset %u in options\n", offset);
            return &options[offset];
        } 
        else {
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "skipping option %u in options\n", options[offset]);
            offset++;
            offset += 1 + options[offset];
        }
    }
    /* is this an overloaded message? */
    if (overload != DHCP_OVERLOAD_NONE) {
        cs_uint16 field_len;
        if (overload == DHCP_OVERLOAD_FILE) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "overloaded file field\n");
            options = (cs_uint8 *)&(hdr->file[0]);
            field_len = DHCP_FILE_LEN;
        } 
        else if (overload == DHCP_OVERLOAD_SNAME) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "overloaded sname field\n");
            options = (cs_uint8 *)&(hdr->sname[0]);
            field_len = DHCP_SNAME_LEN;
        } 
        else  if (overload == DHCP_OVERLOAD_SNAME_FILE){
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "overloaded sname and file field\n");
            options = (cs_uint8 *)&(hdr->sname[0]);
            field_len = DHCP_FILE_LEN + DHCP_SNAME_LEN;
        }
        else {
            return NULL;
        }
        offset = 0;

        /* at least 1 byte to read and no end marker */
        while ((offset < field_len) && (options[offset] != DHCP_OPTION_END)) {
            if (options[offset] == option) {
                APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "option found at offset=%u\n", offset);
                return &options[offset];
            } 
            else {
                APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "skipping option %u\n", options[offset]);
                offset++;
                offset += 1 + options[offset];
            }
        }
    }

    //for DHCP_OPTION_END
    if(offset < len && options[offset] == option)
        return &options[offset];
    return NULL;
}



dhcpv4_hdr_t *dhcpv4_hdr_get(cs_pkt_t *pkt, cs_uint32 *dhcp_len)
{
    cs_uint32 hdr_len = 0;
    cs_uint8 *data = pkt->data + pkt->offset;
    ip_hdr_t *ip_hdr = NULL;
    
    hdr_len = sizeof(cs_ether_header_t) + (pkt->tag_num <<2);
    ip_hdr = (ip_hdr_t *)(data + hdr_len);
    hdr_len += (ip_hdr->ihl << 2) +sizeof(udp_hdr_t);
    *dhcp_len = pkt->len - hdr_len;
    
    return (dhcpv4_hdr_t *)(data+hdr_len);
}

cs_int32 dhcpv4_option82_sub1_fill(cs_uint32 sport, cs_uint32 *len, cs_uint8 *sub1)
{
	// modified by wujm,change access identifier form
    #if 0
    cs_uint32 sub_len = strlen(g_onu_identifier);
    
    strcpy(sub1, g_onu_identifier);
    snprintf(&sub1[sub_len], DHCP_OPTION_PAYLOAD_MAX - sub_len-1, 
            " 1/1/%d EP", sport);
    *len = strlen(sub1);
    #else
	cs_int8 	   str_tmp[GWD_DHCP_OPTION_PAYLOAD_MAX];
	epon_macaddr_t onu_mac = {0};
	epon_macaddr_t olt_mac = {0};
	unsigned char  *p_onu_mac = (unsigned char*)onu_mac;
	unsigned char  *p_olt_mac = (unsigned char*)olt_mac;
	cs_uint8		pon_slot = 0;
	cs_uint8		pon_port = 0;
	cs_callback_context_t	  context;
	cs_sdl_vlan_tag_t	def_vlan;
	cs_sdl_vlan_mode_t	vlan_mode = -1;
	cs_sdl_vlan_cfg_t	vlan_cfg[__VLAN_RULE_PER_PORT_MAX];
	cs_uint16			cfg_nums = __VLAN_RULE_PER_PORT_MAX;
	cs_uint16			option_len = 0;
	
	memset(&def_vlan,0,sizeof(cs_sdl_vlan_tag_t));
	memset(&vlan_cfg,0,sizeof(cs_sdl_vlan_cfg_t)*__VLAN_RULE_PER_PORT_MAX);

	//getOltMacAddress(&olt_mac);
	olt_mac_get_from_local_dhcp_access_identifier(p_olt_mac);
	
	startup_config_read(CFG_ID_MAC_ADDRESS, CS_MACADDR_LEN, p_onu_mac);

         if(0 == p_olt_mac[0] && 0 == p_olt_mac[1] && 0 == p_olt_mac[2]  
            && 0 == p_olt_mac[3] && 0 == p_olt_mac[4] && 0 == p_olt_mac[5] )
         {
            getOltMacAddress(&olt_mac);
         }
         
	olt_pon_port_get_from_local_dhcp_access_identifier(&pon_slot,&pon_port);	

	epon_request_onu_vlan_get(context, 0, 0,sport,&def_vlan,&vlan_mode,vlan_cfg,&cfg_nums);
	
	/* {eth|trk} NAS_slot/NAS_subslot/NAS_port:SVLAN.CVLAN AccessNodeIdentifier/ANI_rack/ANI_frame/ANI_slot/ANI_subslot/ANI_port/ONU_ID /ONU_Slot/ONU_Subslot/Port_ID:pvid EP */

	/* {eth|trk} NAS_slot/NAS_subslot/NAS_port:SVLAN.CVLAN AccessNodeIdentifier */
	option_len += sprintf(str_tmp,"0 0/0/0:0.0 %02X%02X%02X%02X%02X%02X",p_olt_mac[0],p_olt_mac[1],p_olt_mac[2],p_olt_mac[3],p_olt_mac[4],p_olt_mac[5]);
	/* /ANI_rack/ANI_frame/ANI_slot/ANI_subslot/ANI_port */
	option_len += sprintf(&str_tmp[option_len],"/0/0/%d/0/%d",pon_slot,pon_port);
	/* ONU_ID  */
	option_len += sprintf(&str_tmp[option_len],"000000000000%02X%02X%02X%02X%02X%02X ",p_onu_mac[0],p_onu_mac[1],p_onu_mac[2],p_onu_mac[3],p_onu_mac[4],p_onu_mac[5]);
	/* /ONU_Slot/ONU_Subslot/Port_ID */
	option_len += sprintf(&str_tmp[option_len],"%d/%d/%d",ONU_SLOT_NUM,ONU_SUBSLOT_NUM,sport);
	/* :pvid EP */
	option_len += sprintf(&str_tmp[option_len],":%d EP",def_vlan.vid);
	memcpy(sub1,str_tmp,option_len);

	*len = option_len;
	#endif
	
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, %s, len %d\n", __func__, sub1, *len);

    return 0;
}

cs_int32 dhcpv4_recaculate(cs_pkt_t *pkt)
{
    ip_hdr_t *ip_hdr = NULL;
    udp_hdr_t *udp_hdr = NULL;
    cs_uint8 *data = pkt->data + pkt->offset;
    cs_uint32 hdr_len = 0;


    hdr_len = sizeof(cs_ether_header_t) + (pkt->tag_num <<2);
    ip_hdr = (ip_hdr_t *)(data + hdr_len);
    //recaculate ip total len
    ip_hdr->tot_len = htons(pkt->len - hdr_len);
    hdr_len += (ip_hdr->ihl <<2);
    
    udp_hdr = (udp_hdr_t *)(data + hdr_len);
    //recaculate udp len
    udp_hdr->len = htons(pkt->len - hdr_len);

    _ip_checksum(ip_hdr);

    _udp_checksum(ip_hdr);

    return 0;
}

cs_int32 dhcpv4_option60_sub1_fill(cs_uint32 sport, cs_uint32 *len, cs_uint8 *sub1)
{
    const cs_uint8 *value = "YOTC";
    dhcpv4_op60_sub_data_t *data = (dhcpv4_op60_sub_data_t *)sub1;
    //copy verdor name
    data->field_type = 1;
    data->field_len = strlen(value);
    strcpy(data->field_data, value);
    *len = data->field_len + sizeof(dhcpv4_op60_sub_data_t);
    buffer_dump(__func__, sub1, *len);
    return 0;
}

cs_int32 dhcpv4_option60_init(cs_pkt_t* pkt, dhcpv4_option_entry_t *hdr)
{
    cs_uint32 sub_data_len = 0;    
    dhcpv4_op60_data_t *sub;
    cs_uint8 *sub_data;
    cs_uint32 sport = pkt->port;
    if(!hdr) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, invalid parameter\n", __func__);
        return -1;
    }
    //caculate sub1 content and length
    memset(hdr->payload, 0 , sizeof(hdr->payload));
    sub= (dhcpv4_op60_data_t *)&(hdr->payload);
    sub_data = (cs_uint8 *)(sub+1);
    //set OPTION 60 sub1
    dhcpv4_option60_sub1_fill(sport, &sub_data_len, sub_data);
    sub->enterp_num = YOTC_OP60_ENTERP_NUM;
    //set OPTION 60 sub_x
    /*
    sub_data += sub_data_len;
    */
    hdr->code = DHCP_OPTION_60;
    hdr->len = sub_data_len+sizeof(dhcpv4_op60_data_t); //sub1 only
    
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, len %d, %d\n", __func__, hdr->len, __LINE__);
    return 0;
}


cs_int32 dhcpv4_option125_sub1_fill(cs_uint32 sport, cs_uint32 *len, cs_uint8 *sub1)
{
    const cs_uint8 *value = "YOTC";
    dhcpv4_op125_sub_data_t *data = (dhcpv4_op125_sub_data_t *)sub1;
    //copy verdor name
    data->sub_code = 1;
    data->sub_len = strlen(value);
    strcpy(data->data, value);
    *len = data->sub_len + sizeof(dhcpv4_op125_sub_data_t);
    buffer_dump(__func__, sub1, *len);

    return 0;
}

cs_int32 dhcpv4_option125_init(cs_pkt_t* pkt, dhcpv4_option_entry_t *hdr)
{
    cs_uint32 sub_data_len = 0;    
    dhcpv4_op125_data_t *op_data;
    dhcpv4_op125_sub_data_t *sub_data;
    cs_uint32 sport = pkt->port;
    if(!hdr) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, invalid parameter\n", __func__);
        return -1;
    }
    //caculate sub1 content and length
    memset(hdr->payload, 0 , sizeof(hdr->payload));
    op_data= (dhcpv4_op125_data_t *)&(hdr->payload);
    sub_data = (dhcpv4_op125_sub_data_t *)(op_data+1);
    dhcpv4_option125_sub1_fill(sport, &sub_data_len, (cs_uint8 *)sub_data);
    //set OPTION 82 sub1
    op_data->enterp_num = YOTC_OP125_ENTERP_NUM;
    op_data->len = sub_data_len;
    hdr->code = DHCP_OPTION_125;
    hdr->len = sub_data_len+sizeof(dhcpv4_op125_data_t); //sub1 only.
    
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, len %d, %d\n", __func__, hdr->len, __LINE__);
    return 0;
}


cs_int32 dhcpv4_option82_init(cs_pkt_t* pkt, dhcpv4_option_entry_t *hdr)
{
    cs_uint32 sub1_data_len = 0;    
    dhcpv4_op82_data_t *sub1;
#ifdef OPTION_82_SUB2_SUPPORT
    dhcpv4_op82_data_t *sub2;
#endif
    cs_uint8 *sub1_data;
    cs_uint32 sport = pkt->port;
    
    if(!hdr) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, invalid parameter\n", __func__);
        return -1;
    }
    //caculate sub1 content and length
    memset(hdr->payload, 0 , sizeof(hdr->payload));
    sub1= (dhcpv4_op82_data_t *)&(hdr->payload);
    sub1_data = (cs_uint8 *)(sub1+1);
    dhcpv4_option82_sub1_fill(sport, &sub1_data_len, sub1_data);
    //set OPTION 82 sub1
    sub1->sub_op = 1;
    sub1->sub_len = sub1_data_len;
#ifdef OPTION_82_SUB2_SUPPORT
    /*set OPTION 82 sub2 */
    sub2 = (dhcpv4_op82_data_t *)(sub1_data + sub1_data_len);
    sub2->sub_op = 2;
    sub2->sub_len = 0;
#endif
    hdr->code = DHCP_OPTION_82;
    hdr->len = sub1_data_len+sizeof(dhcpv4_op82_data_t); //sub1 only, sub2 is not included.
#ifdef OPTION_82_SUB2_SUPPORT
    hdr->len += sizeof(dhcpv4_op82_data_t);
#endif    
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, len %d, %d\n", __func__, hdr->len, __LINE__);
    return 0;
}


static cs_uint32 dhcpv4_option_xxx_clr(dhcpv4_hdr_t * dhcp, cs_uint32 len, cs_uint8 *data)
{
    cs_uint32 op_len, other_len = 0;
    cs_uint8 *end = NULL;

    end = dhcpv4_option_hdr_get(DHCP_OPTION_END, len, dhcp);
    if(!end) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s,NO END\n", __func__);
        return 0;
    }
    op_len = *(data+1) + sizeof(dhcpv4_option_hdr_t);
    if(data+op_len!= end) {
        other_len = end-(data+op_len);
        //If there are other options present, clear option 82, and move other options forward.
        memmove(data, data+op_len, other_len);
        data += other_len;
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, existed option len %d, other option len %d\n", __func__, op_len, other_len);
    }
    
    *data = DHCP_OPTION_END;

    return op_len;
}

cs_int32 dhcpv4_option_xxx_add(cs_pkt_t *pkt, dhcpv4_option_entry_t *hdr)
{
    cs_uint32 dhcp_len = 0;
    cs_uint32 option_len = 0;
    cs_uint8 *option = NULL;
    dhcpv4_hdr_t *dhcp = NULL;
    
    if(!pkt)
        return -1;

    dhcp = dhcpv4_hdr_get(pkt, &dhcp_len);
    if(!dhcp) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, invalid packet, %d\n", __func__, __LINE__);
        return -1;
    }
    option = dhcpv4_option_hdr_get(hdr->code, dhcp_len, dhcp);
    if(option){
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, OPTION %d already existed, %d\n", __func__, hdr->code, __LINE__);
        option_len = dhcpv4_option_xxx_clr(dhcp, dhcp_len, option);
        if(option_len) {
            pkt->len -= option_len;
            dhcp_len -= option_len;
        }
    }
    
    option = dhcpv4_option_hdr_get(DHCP_OPTION_END, dhcp_len, dhcp);
    if(!option) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s,  NO END, %d\n", __func__, __LINE__);
        return 0;
    }
    //option_len = hdr->len + 2;
    *option++ = hdr->code;
    *option++ = hdr->len;
    memcpy(option, hdr->payload, hdr->len);
    //set OPTION 255
    option += hdr->len;
    *option = DHCP_OPTION_END;
    pkt->len += (hdr->len+2);
    //dhcp_buffer_dump(__func__, hdr, option_len);
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s,  option len %d\n", __func__, hdr->len);
    return 0;
}

cs_int32 dhcpv6_option18_init(cs_pkt_t* pkt, dhcpv6_option_entry_t *hdr)
{
    cs_uint32 op_len = 0;    
    cs_uint32 sport;
    
    if(!hdr || !pkt) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, invalid parameter\n", __func__);
        return -1;
    }
    sport = pkt->port;
    memset(hdr->payload, 0 , sizeof(hdr->payload));
    dhcpv4_option82_sub1_fill(sport, &op_len, hdr->payload);
    hdr->code = DHCPV6_OPTION_18;
    hdr->len = op_len;
    
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, len %d, %d\n", __func__, hdr->len, __LINE__);
    return 0;
}
cs_int32 dhcpv6_option37_init(cs_pkt_t* pkt, dhcpv6_option_entry_t *hdr)
{
    if(!hdr || !pkt) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, invalid parameter\n", __func__);
        return -1;
    }
    hdr->code = DHCPV6_OPTION_37;
    //remote id not included, enterprise number is 0.
    hdr->len = sizeof(dhcpv6_option_hdr_t)+4;
    
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, len %d, %d\n", __func__, hdr->len, __LINE__);
    
    return 0;
}


cs_int32 dhcpv6_recaculate(cs_pkt_t *pkt)
{
    udp_hdr_t *udp_hdr = NULL;
    cs_uint8 *data = pkt->data + pkt->offset;
    cs_uint32 l2_len = 0;
    ipv6_hdr_t *ipv6_hdr;
    cs_uint32 ipv6_op_len = 0;
    
    ipv6_op_len = ipv6_option_parse(pkt);
    if(ipv6_op_len < 0) {
        return -1;
    }
    l2_len = sizeof(cs_ether_header_t) + (pkt->tag_num <<2);
    ipv6_hdr = (ipv6_hdr_t *)(data + l2_len);
    ipv6_hdr->len = htons(pkt->len- l2_len - sizeof(ipv6_hdr_t));
    udp_hdr =  (udp_hdr_t *)(data + l2_len+sizeof(ipv6_hdr_t) + ipv6_op_len);
    udp_hdr->len = htons(pkt->len- l2_len - sizeof(ipv6_hdr_t) - ipv6_op_len);
    //udp_hdr->ctc = 0;

    ipv6_udp_checksum(ipv6_op_len, ipv6_hdr);

    return 0;
}


dhcpv6_hdr_t *dhcpv6_hdr_get(cs_pkt_t *pkt, cs_uint32 *dhcp_len)
{
    cs_uint32 hdr_len = 0;
    cs_uint32 ipv6_op_len = 0;
    
    ipv6_op_len = ipv6_option_parse(pkt);
    if(ipv6_op_len < 0) {
        return NULL;
    }
    hdr_len = sizeof(cs_ether_header_t) + (pkt->tag_num <<2);
    hdr_len += sizeof(ipv6_hdr_t) +sizeof(udp_hdr_t) + ipv6_op_len;
    *dhcp_len = pkt->len - hdr_len;
    
    return (dhcpv6_hdr_t *)(pkt->data + pkt->offset + hdr_len);
}
static cs_uint8 *dhcpv6_option_hdr_get(cs_uint16 option, cs_uint32 len, dhcpv6_hdr_t *hdr)
{
    cs_uint16 offset = 0;
    dhcpv6_option_hdr_t *op_hdr;
    
    if(!hdr)
        return NULL;

    offset = sizeof(dhcpv6_hdr_t);
    while (offset < len) {
        op_hdr = (dhcpv6_option_hdr_t *)((cs_uint8 *)hdr+offset);
        if (ntohs(op_hdr->code) == option) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "option found at offset %u in options\n", offset);
            return (cs_uint8 *)op_hdr;
        } 
        else {
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "skipping option %u in options\n", ntohs(op_hdr->code));
            offset += (sizeof(dhcpv6_option_hdr_t) + ntohs(op_hdr->len));
        }
    }
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, No option %u in packet\n", __func__, option);

    return NULL;
}
static cs_uint32 dhcpv6_option_xxx_clr(dhcpv6_hdr_t * dhcp, cs_uint32 len, cs_uint8 *data)
{
    cs_uint32 other_len = 0, op_len = 0;
    cs_uint8 *end = NULL;
    dhcpv6_option_hdr_t *op_hdr;
    
    end = (cs_uint8 *)dhcp + len;
    if(!end) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s,NO END\n", __func__);
        return 0;
    }
    op_hdr = (dhcpv6_option_hdr_t *)data;
    if(ntohs(op_hdr->len) < len) {
        op_len = ntohs(op_hdr->len);
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, existed option len %d, other option len %d\n", 
            __func__, op_len, other_len);
        other_len = end-(data+ntohs(op_hdr->len));
        //If there are other options present, clear option 82, and move other options forward.
        memmove(data, data+ntohs(op_hdr->len), other_len);
    }
    
    return op_len;
}
cs_int32 dhcpv6_option_xxx_add(cs_pkt_t *pkt, dhcpv6_option_entry_t *hdr)
{
    cs_uint16 padding = 0;
    cs_uint32 dhcp_len = 0;
    cs_uint32 option_len = 0;
    cs_uint8 *option = NULL;
    dhcpv6_hdr_t *dhcp = NULL;
    
    if(!pkt)
        return -1;
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s,  option len %d\n", __func__, hdr->len);
    dhcp = dhcpv6_hdr_get(pkt, &dhcp_len);
    if(!dhcp) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, invalid packet, %d\n", __func__, __LINE__);
        return -1;
    }
    option = dhcpv6_option_hdr_get(hdr->code, dhcp_len, dhcp);
    if(option){
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, OPTION %d already existed, %d\n", __func__, hdr->code, __LINE__);
        option_len = dhcpv6_option_xxx_clr(dhcp, dhcp_len, option);
        if(option_len) {
            pkt->len -= option_len;
            dhcp_len -= option_len;
        }
    }
    option = (cs_uint8 *)dhcp + dhcp_len;
    //option_len = hdr->len + 2;
    padding = htons(hdr->code);
    memcpy(option, &(padding), 2);
    option+=2;
    padding = htons(hdr->len);
    memcpy(option, &(padding), 2);
    option+=2;
    memcpy(option, hdr->payload, hdr->len);
    pkt->len += (hdr->len+4);
    //dhcp_buffer_dump(__func__, hdr, option_len);
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s,  option len %d\n", __func__, hdr->len);
    return 0;
}



cs_int32 dhcpv4_add(cs_pkt_t *pkt)
{
    cs_int32 ret = 0;
    dhcp_option_cb_t *dhcp_cb = &dhcp_option_cb;
    cs_pkt_t *packet_tmp;
    cs_callback_context_t     context;
    
    if(!pkt)
        return 1;
    
    ret = vlan_ingress_filter(pkt);
    if(ret != CS_E_OK) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"DHCP: %s Drop due to invalid vlan\n", __func__);
        return 1;
    }

    packet_tmp = dhcp_packet_clone((cs_pkt_t *)pkt_dummy, pkt);
    if(dhcp_cb->op60.enable) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, adding DHCP OPTION 60\n", __func__);
        dhcpv4_option60_init(pkt,  &(dhcp_cb->op60));
        dhcpv4_option_xxx_add(packet_tmp, &(dhcp_cb->op60));
    }

    if(dhcp_cb->op125.enable) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, adding DHCP OPTION 125\n", __func__);
        dhcpv4_option125_init(pkt,  &(dhcp_cb->op125));
        dhcpv4_option_xxx_add(packet_tmp, &(dhcp_cb->op125));
    }
    if(dhcp_cb->op82.enable) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, adding DHCP OPTION 82\n", __func__);
        dhcpv4_option82_init(pkt,  &(dhcp_cb->op82));
        dhcpv4_option_xxx_add(packet_tmp, &(dhcp_cb->op82));
    }

    dhcpv4_recaculate(packet_tmp);

    buffer_dump(__func__, packet_tmp->data+packet_tmp->offset, packet_tmp->len);

    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s,  len %d\n", __func__,packet_tmp->len);
    epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
		     packet_tmp->data+packet_tmp->offset,packet_tmp->len,0,7);

    return 1;
}

cs_int32 dhcpv6_add(cs_pkt_t *pkt)
{
    cs_int32 ret = 0;
    dhcp_option_cb_t *dhcp_cb = &dhcp_option_cb;
    cs_pkt_t *packet_tmp;
    cs_callback_context_t     context;
    
    if(!pkt)
        return 1;
    ret = vlan_ingress_filter(pkt);
    if(ret != CS_E_OK) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"DHCP: %s Drop due to invalid vlan\n", __func__);
        return 1;
    }
	
    memset(&pkt_dummy,0,sizeof(pkt_dummy));
    packet_tmp = dhcp_packet_clone((cs_pkt_t *)pkt_dummy, pkt);
    if(dhcp_cb->op18.enable) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, adding DHCP OPTION 18\n", __func__);
        dhcpv6_option18_init(packet_tmp, &(dhcp_cb->op18));
        dhcpv6_option_xxx_add(packet_tmp, &(dhcp_cb->op18));
    }

    if(dhcp_cb->op37.enable) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s, adding DHCP OPTION 37\n", __func__);
        dhcpv6_option37_init(packet_tmp, &(dhcp_cb->op37));
        dhcpv6_option_xxx_add(packet_tmp, &(dhcp_cb->op37));
    }
    dhcpv6_recaculate(packet_tmp);
    buffer_dump(__func__, packet_tmp->data+packet_tmp->offset, packet_tmp->len);

    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP: %s,  len %d\n", __func__,packet_tmp->len);
    epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
		     packet_tmp->data+packet_tmp->offset,packet_tmp->len,0,7);

    return 1;
}



cs_int32 pppoe_sub1_fill(cs_uint32 sport, cs_uint32 *len, cs_uint8 *sub1)
{

    cs_uint32 sub_len = strlen(g_onu_identifier);
    
    strcpy(sub1, g_onu_identifier);
    snprintf(&sub1[sub_len], DHCP_OPTION_PAYLOAD_MAX - sub_len-1, 
            " 1/1/%d EP", sport);
    *len = strlen(sub1);
    
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "PPPOE+: %s, %s, len %d\n", __func__, sub1, *len);
    
    return 0;
}


//add tag to the end of packet
cs_int32 pppoe_plus_add(cs_pkt_t *pkt)
{
    cs_uint32 len = 0;
    cs_uint8 pppoe_tag[128] = {0x01, 0x10, 0x00, 0x00};
    cs_pkt_t *packet_tmp;
    pppoe_hdr_t *pppoe_hdr = NULL;
    cs_uint32 l2_len = 0;
    cs_uint8 *data = NULL;
    cs_uint8 *payload = NULL;
    cs_int32 ret = 0;
    cs_callback_context_t     context;
    pppoe_sub1_fill(pkt->port, &len, &pppoe_tag[4]);
    pppoe_tag[3] = len;
    len+=4;

    ret = vlan_ingress_filter(pkt);
    if(ret != CS_E_OK) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"DHCP: %s Drop due to invalid vlan\n", __func__);
        return ret;
    }
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "PPPOE: %s, tag len %d\n", __func__, len);
    packet_tmp = dhcp_packet_clone((cs_pkt_t *)pkt_dummy, pkt);
    l2_len = sizeof(cs_ether_header_t) + (packet_tmp->tag_num<<2);
    data = packet_tmp->data+packet_tmp->offset;
    pppoe_hdr = (pppoe_hdr_t *)(data + l2_len);
    payload = data + l2_len+sizeof(pppoe_hdr_t)+ntohs(pppoe_hdr->length);
    
    memcpy(payload, pppoe_tag, len);

    pppoe_hdr->length  = ntohs(pppoe_hdr->length) + len;
    pppoe_hdr->length  = htons(pppoe_hdr->length );
    packet_tmp->len += len;
    buffer_dump(__func__, packet_tmp->data+packet_tmp->offset, packet_tmp->len);

    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "PPPOE: %s,  len %d, pppoe_hdr->length %d\n", __func__,packet_tmp->len, pppoe_hdr->length );
    epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
		     packet_tmp->data+packet_tmp->offset,packet_tmp->len,0,7);

    return 0;
}


static cs_status pppoe_pkt_parser(cs_pkt_t *pPkt)
{
    if(pPkt->eth_type == EPON_ETHERTYPE_PPPOE_DISC)
    {
        pPkt->pkt_type = CS_PKT_PPPOE_DIS;
        return CS_E_OK;
    }

    if(pPkt->eth_type == EPON_ETHERTYPE_PPPOE_SESSION)
    {
        pPkt->pkt_type = CS_PKT_PPPOE_SES;
        return CS_E_OK;
    }
    
    return CS_E_NOT_SUPPORT;
}
static cs_uint8 pppoe_pkt_recv(cs_pkt_t* hdr)
{

    /*handle pppoe packet*/
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PPPOE: %s source port %d, type 0x%x\n", __func__, hdr->port, hdr->eth_type);
    if(hdr->port != CS_PON_PORT_ID)
    {
        if(hdr->eth_type == EPON_ETHERTYPE_PPPOE_DISC){
            pppoe_plus_add(hdr);
        }
        else{
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PPPOE: %s invalid packet type %d\n", __func__, hdr->eth_type);
            return 1;
        }
    }

    return 1;
}

static cs_status pppoe_plus_enable(cs_boolean enable)
{
    cs_status ret = CS_E_OK;  
    cs_sdl_pkt_state_t state;
    cs_callback_context_t context;
    
    state = enable?DST_CPU:DST_FE;
    ret = epon_request_onu_spec_pkt_state_set(context, 0, 0, S_UPSTREAM, CS_PKT_PPPOE_DIS, state);
    ret |= epon_request_onu_spec_pkt_state_set(context, 0, 0, S_DOWNSTREAM, CS_PKT_PPPOE_DIS, DST_FE);
    if(ret) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PPPOE: %s trap failed %d\n", __func__, ret);
        return -1;
    }
    if(enable) {
        app_pkt_reg_parser(CS_PKT_PPPOE_DIS, pppoe_pkt_parser);
        app_pkt_reg_handler(CS_PKT_PPPOE_DIS, pppoe_pkt_recv);
    }
    else {
        app_pkt_reg_parser(CS_PKT_PPPOE_DIS, NULL);
        app_pkt_reg_handler(CS_PKT_PPPOE_DIS, NULL);
    }
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%s: %s PPPoE trapping\n", __func__, enable?"enable":"disable");
    return CS_E_OK;
}

void bcmp_ip_hdr_build(cs_uint8 type, cs_uint16 seq, const cs_uint8 *mac, cs_uint8 *ddata)
{    
    cs_uint16 ip_len;
    bcmp_ip_hdr_t *hdr = (bcmp_ip_hdr_t *)ddata;
    
    memset(hdr, 0, sizeof(bcmp_ip_hdr_t));
    hdr->ver = BCMP_VERION;
    hdr->type = type;
    hdr->seq = seq;
    hdr->crc = 0;
    ip_len = sizeof(bcmp_ip_hdr_t) + sizeof(bcmp_ip_cluster_onu_data_t);
    hdr->len = htons(ip_len);
    memcpy(hdr->bcmp_mac, mac, 6);
    hdr->crc = __checksum__((cs_uint16*)ddata, ip_len);
}
void bcmp_ip_payload_build(const cs_pkt_t *pkt, const cs_uint8 *mac, cs_uint8 *ddata)
{    
    bcmp_ip_cluster_onu_data_t *bcmp_payload = (bcmp_ip_cluster_onu_data_t *)ddata;
    
    bcmp_payload->hdr.len = sizeof(bcmp_ip_cluster_onu_data_t);
    bcmp_payload->hdr.type = BCMP_DEV_TYPE_CBAT;
    bcmp_payload->port_id = htonl(pkt->port);
    bcmp_payload->olt_id = htonl(dhcp_option_cb.bcmp_cb.olt_id); //PON-ID
    bcmp_payload->slot_id = htonl(dhcp_option_cb.bcmp_cb.slot_id);
    bcmp_payload->onu_id = htonl(dhcp_option_cb.bcmp_cb.onu_id);
    memcpy(bcmp_payload->cbat_mac, mac, 6);
    memcpy(bcmp_payload->onu_mac, dhcp_option_cb.mac, 6);
}


void bcmp_l2_hdr_build(cs_uint8 *ddata, const cs_uint8 *cbat_mac, const cs_pkt_t *pkt)
{
    cs_uint32 tag_len = pkt->tag_num<<2;
    cs_ether_header_t *eth_hdr = (cs_ether_header_t *)ddata;;
    
    memcpy(&(eth_hdr->dst), bcmp_dmac, 6);
    // TODO: source mac should be the same as CBAT mac address
    //memcpy(&(eth_hdr->src), pkt->data+pkt->offset+6, 6);
    memcpy(&(eth_hdr->src), cbat_mac, 6);
    if(tag_len) {
        cs_uint8 *tag_pos = (cs_uint8 *)&(eth_hdr->ethertype);
        //add vlan tag if there is present on original packet
        memcpy(tag_pos, pkt->data + pkt->offset + 12, tag_len);
        eth_hdr = (cs_ether_header_t *)(ddata+tag_len);
    }
    eth_hdr->ethertype = htons(ETHER_BCMP_TYPE);
}


//for testing only
cs_uint32 cli_bcmp_l2_hdr_build(cs_uint8 *data)
{
    bcmp_l2_hdr_t *bcmp_l2_hdr = (bcmp_l2_hdr_t *)data;
    bcmp_l2_hdr->seq = htons(1);
    bcmp_l2_hdr->ver = BCMP_VERSION;
    bcmp_l2_hdr->type = BCMP_REG;
    bcmp_l2_hdr->len = htons(sizeof(bcmp_l2_hdr_t) + sizeof(bcmp_cluster_cbat_data_t));
    bcmp_l2_hdr->crc = 0;
    bcmp_l2_hdr->cluster = htons(1);
    buffer_dump(__func__, data, sizeof(bcmp_l2_hdr_t));
    return sizeof(bcmp_l2_hdr_t);
}

cs_uint8 bcmp_pkt_proc(cs_pkt_t *pkt)
{
    cs_uint8 *ddata;
    bcmp_l2_hdr_t *bcmp_hdr;
    cs_uint32 l2_len = 0;
    cs_pkt_t *packet_tmp;
    bcmp_cluster_cbat_data_t *cbat_data;
    cs_int32 ret = 0;
    cs_uint8 *smac, zero_mac[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
    cs_callback_context_t     context;

    pkt_dump("BCMP: receive\n", pkt->data + pkt->offset, pkt->len, APP_IPINTF_LOG_PKT_BCMP);
    ret = vlan_ingress_filter(pkt);
    if(ret != CS_E_OK) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"BCMP: %s Drop due to invalid vlan\n", __func__);
        return 1;
    }
    //Get CBAT mac address
    l2_len = sizeof(cs_ether_header_t) + (pkt->tag_num << 2);
    bcmp_hdr = (bcmp_l2_hdr_t*)(pkt->data + pkt->offset + l2_len);
    if(bcmp_hdr->type == BCMP_TICK) {
        //forward it only without any modification
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "BCMP: forward TICK packet\n");
        epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
		     pkt->data+pkt->offset,pkt->len, 0, 7);
    }
    if(ntohs(bcmp_hdr->len) <= sizeof(bcmp_l2_hdr_t)) {
        //return -1;*
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "BCMP: %s,  len %d, %d\n", __func__,ntohs(bcmp_hdr->len), sizeof(bcmp_l2_hdr_t));
    }
    cbat_data = (bcmp_cluster_cbat_data_t *)(bcmp_hdr + 1);
    // CBAT mac address valid checking
    smac = pkt->data+pkt->offset+6;
    if((memcmp(pkt->data+pkt->offset+6, zero_mac, 6)== 0) ||
        (BITON(smac[0], 0))) {
        cs_uint8 *mac = cbat_data->mac;
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "BCMP: %s, invalid mac address\n", __func__);
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "BCMP: %02x%02x%02x%02x%02x%02x", 
            mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
        mac = pkt->data+pkt->offset+6;
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "BCMP: %02x%02x%02x%02x%02x%02x", 
            mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
        return 1;
    }
    /*
    * Build BCMP IP format
    * 1. Build l2 header:dmac/src/eth_type
    * 2. Build BCMP IP header, 4.2 for reference
    * 3. Build BCMP IP payload, 4.2.1 for reference
    */
    memset(pkt_dummy, 0 ,sizeof(pkt_dummy));
    packet_tmp = (cs_pkt_t *)pkt_dummy;
    packet_tmp->offset = CS_PKT_OFFSET;
    ddata = packet_tmp->data+packet_tmp->offset;
    //Build l2 header
    bcmp_l2_hdr_build(ddata, cbat_data->mac, pkt);
    ddata += l2_len;
    
    /* Build BCMP IP payload*/
    bcmp_ip_payload_build(pkt, cbat_data->mac, ddata+sizeof(bcmp_ip_hdr_t));

    /* Build BCMP IP header*/
    bcmp_ip_hdr_build(IP_BCMP_REG, bcmp_hdr->seq, cbat_data->mac, ddata);
    
    packet_tmp->len = l2_len + sizeof(bcmp_ip_hdr_t) + sizeof(bcmp_ip_cluster_onu_data_t);
    //buffer_dump(__func__, packet_tmp->data+packet_tmp->offset, packet_tmp->len);
    pkt_dump("BCMP: send\n", packet_tmp->data + packet_tmp->offset, packet_tmp->len, APP_IPINTF_LOG_PKT_BCMP);
    epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
		     packet_tmp->data+packet_tmp->offset,packet_tmp->len,0,7);

    return 1;
}


//e.g. R#!   /0/0/10/0/2/001a69007211 1/1/2 EP
cs_boolean bcmp_node_cfg_equal(const dhcp_access_node_id_t *id)
{
    bcmp_cb_t *bcmp_cb = &dhcp_option_cb.bcmp_cb;
    if(!bcmp_cb->used) {
        return TRUE;
    }
    if(memcmp(g_onu_id, id->id, sizeof(g_onu_id))){
        return FALSE;
    }
    if((bcmp_cb->onu_id != id->onu_id) ||
        (bcmp_cb->olt_id != id->ani_port)||
        (bcmp_cb->slot_id != id->ani_slot)||
        (bcmp_cb->sub_slot_id != id->ani_subslot)||
        (bcmp_cb->ani_rack != id->ani_rack)||
        (bcmp_cb->ani_frame != id->ani_frame)) {
        return FALSE;
    }

    return TRUE;
}
cs_int32 bcmp_node_set(const dhcp_access_node_id_t *id)
{
    //{eth|trk} NAS_slot/NAS_subslot/NAS_port:SVLAN.CVLAN 
    cs_boolean equal = FALSE;
    cs_uint8 *mac = dhcp_option_cb.mac;
    cs_uint32 len = 0;
    bcmp_cb_t *bcmp_cb = &dhcp_option_cb.bcmp_cb;

    equal = bcmp_node_cfg_equal(id);
    memset(g_onu_identifier, 0, DHCP_OPTION_PAYLOAD_MAX);
    memset(g_onu_id, 0, sizeof(g_onu_id));
    
    strcpy(g_onu_identifier, id->id);
    strcpy(g_onu_id, id->id);
    
    len = strlen(g_onu_identifier);
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP:%s, %s, len %d, %d\n", 
        __func__, g_onu_identifier, len, __LINE__);
    snprintf(&(g_onu_identifier[len]), DHCP_OPTION_PAYLOAD_MAX - len-1, 
            "/%d/%d/%d/%d/%d/%02x%02x%02x%02x%02x%02x",
            /*id->id, */id->ani_rack,id->ani_frame,id->ani_slot,id->ani_subslot,id->ani_port,
            mac[0], mac[1],mac[2], mac[3], mac[4],mac[5]);    
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "DHCP:%s, %s, len %d, %d\n", 
        __func__, g_onu_identifier, strlen(g_onu_identifier), __LINE__);

    bcmp_cb->port_id= 0;//id->ani_port;//should be the actual port id
    bcmp_cb->onu_id =  id->onu_id;
    bcmp_cb->olt_id = id->ani_port;//PON-ID
    bcmp_cb->slot_id = id->ani_slot;
    bcmp_cb->sub_slot_id = id->ani_subslot;
    bcmp_cb->ani_rack = id->ani_rack;
    bcmp_cb->ani_frame = id->ani_frame;
    memcpy(bcmp_cb->id, id->id, 50);
    bcmp_cb->used = TRUE;
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "BCMP: port id %d, onu id %d, olt id(PON-ID) %d, slot id %d\n", 
        bcmp_cb->port_id, bcmp_cb->onu_id, bcmp_cb->olt_id, bcmp_cb->slot_id);

    return 0;
}
//e.g. R#!   /0/0/10/0/2/001a69007211 1/1/2 EP
cs_int32 bcmp_node_get(dhcp_access_node_id_t *id)
{
    //{eth|trk} NAS_slot/NAS_subslot/NAS_port:SVLAN.CVLAN 
    bcmp_cb_t *bcmp_cb = &dhcp_option_cb.bcmp_cb;
    
    id->ani_port = bcmp_cb->olt_id;//bcmp_cb->port_id;
    id->onu_id = bcmp_cb->onu_id;
    id->ani_subslot = bcmp_cb->sub_slot_id;
    id->ani_slot = bcmp_cb->slot_id;
    id->ani_rack = bcmp_cb->ani_rack;
    id->ani_frame = bcmp_cb->ani_frame;
    memcpy(id->id, bcmp_cb->id, 50);

    return 0;
}

void dhcp_option_init(void)
{
    dhcp_option_cb_t *dh_op = &dhcp_option_cb;

    cs_printf("%s\n", __func__);
    memset(dh_op, 0, sizeof(dhcp_option_cb_t));
    dh_op->op60.code = DHCP_OPTION_60;
    dh_op->op60.len = DHCP_OPTION_PAYLOAD_MAX;
    dh_op->op60.enable = FALSE;
    dh_op->op60.init = dhcpv4_option60_init;
    dh_op->op60.proc = dhcpv4_option_xxx_add;

    dh_op->op82.code = DHCP_OPTION_82;
    dh_op->op82.len = DHCP_OPTION_PAYLOAD_MAX;
    dh_op->op82.enable = FALSE;
    dh_op->op82.init = dhcpv4_option82_init;
    dh_op->op82.proc = dhcpv4_option_xxx_add;
    
    dh_op->op125.code = DHCP_OPTION_125;
    dh_op->op125.len = DHCP_OPTION_PAYLOAD_MAX;
    dh_op->op125.enable = FALSE;
    dh_op->op125.init = dhcpv4_option125_init;
    dh_op->op125.proc = dhcpv4_option_xxx_add;


    dh_op->op18.code = DHCPV6_OPTION_18;
    dh_op->op18.len = DHCP_OPTION_PAYLOAD_MAX;
    dh_op->op18.enable = FALSE;
    dh_op->op18.init = NULL;
    dh_op->op18.proc = dhcpv6_option_xxx_add;
    
    dh_op->op37.code = DHCPV6_OPTION_37;
    dh_op->op37.len = DHCP_OPTION_PAYLOAD_MAX;
    dh_op->op37.enable = FALSE;
    dh_op->op37.init= NULL;
    dh_op->op37.proc = dhcpv6_option_xxx_add;
    
    dh_op->pppoe.enable = FALSE;
    dh_op->bcmp_cb.enable = FALSE;
    dh_op->bcmp_cb.used = FALSE;
    startup_config_read(CFG_ID_MAC_ADDRESS , ETHER_ADDR_LEN,dh_op->mac);
#if 0 //testing only
{
    //cs_uint8 my_mac_[] = {0x00 ,0x1a ,0x69 ,0xcc ,0xcd ,0xdc};
    const cs_uint8 *yotc = "YOTC";
    dhcp_access_node_id_t id;

    memset(&id, 0, sizeof(id));
    strcpy(id.id, yotc);
    id.ani_rack = 5;
    id.ani_frame = 4;
    id.ani_slot = 3;
    id.ani_subslot = 2;
    id.ani_port = 1;
    id.onu_id = 6;
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%s: %s\n", __func__, id.id);
    bcmp_node_set(&id);
    dh_op->bcmp_cb.enable = 1;
    dh_op->bcmp_cb.slot_id = 0xe;
    dh_op->bcmp_cb.onu_id = 2;
    dh_op->bcmp_cb.olt_id = 4;
    dh_op->bcmp_cb.port_id = 1;
    //memcpy(dh_op->mac, my_mac_, 6);
    }
#endif
    return;
}

/*
cs_uint8 app_dhcp_recv(cs_pkt_t* pkt)
{
    if(pkt->pkt_type == IPV4_DHCP) {
        dhcpv4_add(pkt);
    }
    else {
        dhcpv6_add(pkt);
    }
    return 0;
}
*/
cs_status app_ipintf_pon_eoc_pkt_parser(cs_pkt_t *pPkt)
{
#ifdef DHCP_OPTION_PPPOE_PLUS_SUPPORT
    if(EPON_ETHERTYPE_PON_EOC == pPkt->eth_type) {
        IROS_LOG_MIN(IROS_MID_ETHDRV,"%s: PON EOC detected\n", __func__);
        pPkt->pkt_type = CS_PKT_PON_EOC;
        return CS_E_OK;
    }
#endif    
    return CS_E_NOT_SUPPORT;
}
#define PON_EOC_TYPE_QUERY 0x01
#define PON_EOC_TYPE_RESP 0x02
#define PON_EOC_TYPE_RESET 0x03
typedef enum {
    CLT_INFO_STATE_INVALID = 0,
    CLT_INFO_STATE_SUCC ,
    CLT_INFO_STATE_RESET,
}clt_info_state_t;
typedef struct {
    clt_info_state_t state;
    cs_uint8 port;
    cs_uint8 retry;
    cs_uint8 mac[6];
    cs_uint32 dw_clt_ip;
}clt_info_t;

typedef struct {
    cs_uint8 msg_type;// 2
    cs_uint16 onu_type;
    cs_uint8 port;
}__attribute__ ((aligned(1), packed))pon_eoc_pkt_resp_hdr_t;
typedef struct {
    cs_uint8 msg_type; // 1
    cs_uint32 clt_ip;
}__attribute__ ((aligned(1), packed))pon_eoc_pkt_req_hdr_t;
typedef struct {
    cs_uint8 shelf_id; // 1
    cs_uint8 frame_id; // 1
    cs_uint8 slot_id; // 1
    cs_uint8 subslot_id; // 1
    cs_uint8 pon_id; // 1
    cs_uint8 onu_id; // 1
}__attribute__ ((aligned(1), packed))pon_eoc_onu_info_t;

#define ONU_YOTC_FE_ONU_TYPE         (0x141)
#define ONU_YOTC_GE_ONU_TYPE         (0x155)

#define CLT_INFO_PORT_MAX 5
#define CLT_RESET_RETRY_MAX 3
#define PON_EOC_TIMER_INTERVAL 30 //second
//static clt_info_t clt_info[CLT_INFO_PORT_MAX];
//static cs_uint16 yotc_onu_type = ONU_YOTC_FE_ONU_TYPE;
typedef struct {
    cs_uint8 retry; //CLT_RESET_RETRY_MAX
    cs_uint16 onu_type;
    cs_uint32 tmr_handle;
    app_timer_msg_t tmr_msg;
    clt_info_t clt_info[CLT_INFO_PORT_MAX];
}clt_cb_t;
clt_cb_t clt_cb;
#define CLT_INFO_GET(port) clt_cb.clt_info[(port)]
#define CLT_ONU_TYPE_GET() clt_cb.onu_type
#define CLT_RESET_RETRY_GET(port) clt_cb.clt_info[(port)].retry
#define list_port_for_each(pos, start, max) \
	for (pos = start; pos < max; pos++)
        
void pon_eoc_timer_start()
{
    if(clt_cb.tmr_handle != CS_INVALID_TIMER) {
        cs_timer_del(clt_cb.tmr_handle);       
        clt_cb.tmr_handle = CS_INVALID_TIMER;
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: start timer - fix me.\n");
    }
    clt_cb.tmr_handle= cs_msg_timer_add(app_msg_q,PON_EOC_TIMER_INTERVAL*1000, 
                                                                (void *)&(clt_cb.tmr_msg), NULL);
    if(clt_cb.tmr_handle == CS_INVALID_TIMER) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: add timer failed.\n");
    }
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: timer start\n");
}
void pon_eoc_timer_stop()
{
    if(clt_cb.tmr_handle == CS_INVALID_TIMER) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: invalid timer id\n");
        return ;
    }
    cs_timer_del(clt_cb.tmr_handle);
    clt_cb.tmr_handle = CS_INVALID_TIMER;
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: timer stop\n");
}

void pon_eoc_timer_init(void)
{
    clt_cb.tmr_msg.msg_type = IROS_MSG_TIME_OUT;
    clt_cb.tmr_msg.timer_type = APP_TIMER_PON_EOC;
    clt_cb.tmr_handle = CS_INVALID_TIMER;
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: timer init\n");
}

void clt_info_dump(void) 
{
    cs_uint32 i = 0;
    const cs_uint8 *clt_state[] = {"invalid", "ok","reset"};
    list_port_for_each(i, 0, CLT_INFO_PORT_MAX) {
        cs_printf("%-10s: %d\n\r", "PORT", CLT_INFO_GET(i).port);
        cs_printf("%-10s: %s\n\r", "State", clt_state[CLT_INFO_GET(i).state%3]);
        cs_printf("%-10s: %02x%02x%02x%02x%02x%02x\n\r","CLT MAC", 
            CLT_INFO_GET(i).mac[0], CLT_INFO_GET(i).mac[1], CLT_INFO_GET(i).mac[2],
            CLT_INFO_GET(i).mac[3], CLT_INFO_GET(i).mac[4], CLT_INFO_GET(i).mac[5]);
        cs_printf("%-10s: 0x%x\n\r", "CLT IP",CLT_INFO_GET(i).dw_clt_ip);
    }
}
cs_uint8 pon_eoc_reset(cs_uint16 dport)
{
    cs_uint8 *ddata;
    cs_pkt_t *pkt;
    cs_ether_header_t *eth_hdr;
    pon_eoc_pkt_resp_hdr_t *resp;
    cs_callback_context_t     context;
    
    memset(pkt_dummy, 0 ,sizeof(pkt_dummy));
    pkt = (cs_pkt_t *)pkt_dummy;
    pkt->offset = CS_PKT_OFFSET;
    ddata = pkt->data+pkt->offset;
    //Build l2 header
    eth_hdr = (cs_ether_header_t *)ddata;
    memcpy(&(eth_hdr->dst), CLT_INFO_GET(dport).mac, 6);
    memcpy(&(eth_hdr->src), ipintf_info.mac, 6);
    eth_hdr->ethertype = htons(ETHER_PON_EOC_TYPE);
    ddata += sizeof(cs_ether_header_t);
    // Build pon eoc header
    resp = (pon_eoc_pkt_resp_hdr_t *)ddata;
    resp->msg_type = PON_EOC_TYPE_RESET;
    pkt->len = 60;//dmac/smac/eth_type/msg_type/45 padding
    epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
		     pkt->data+pkt->offset, pkt->len, dport,7);

    return 1;
}
void clt_info_clear(void)
{
    cs_uint32 i = 0;
    memset(&CLT_INFO_GET(0), 0, sizeof(clt_info_t)*CLT_INFO_PORT_MAX);
    list_port_for_each(i, 0, CLT_INFO_PORT_MAX) {
        CLT_INFO_GET(i).port = i;
    }
}
cs_uint8 pon_eoc_timer_handle(void)
{
    cs_uint32 i = 0;
    
    clt_cb.tmr_handle = CS_INVALID_TIMER;
    //APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: timer handle, send reset retry %d\n", CLT_RESET_RETRY_GET());
    list_port_for_each(i, 1, CLT_INFO_PORT_MAX) {
        if(CLT_INFO_GET(i).state == CLT_INFO_STATE_RESET) {
            //pon_eoc_reset(CLT_INFO_GET(i).port);
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: timer handle, send reset to port %d\n", CLT_INFO_GET(i).port);
        }
        else if(CLT_INFO_GET(i).state == CLT_INFO_STATE_SUCC){
            //APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: timer handle, CLT connect to port %d\n", CLT_INFO_GET(i).port);
        }
        else {
            //APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: timer handle, no CLT connect to port %d\n", CLT_INFO_GET(i).port);
        }
    }
    
    list_port_for_each(i, 1, CLT_INFO_PORT_MAX) {
        CLT_RESET_RETRY_GET(i) += 1;
        if (CLT_RESET_RETRY_GET(i) >= CLT_RESET_RETRY_MAX) {
            CLT_INFO_GET(i).state = CLT_INFO_STATE_INVALID;
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: timer handle(end), no CLT connect to port %d\n", CLT_INFO_GET(i).port);
        }
        else if(CLT_INFO_GET(i).state == CLT_INFO_STATE_SUCC){
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: timer handle, CLT connect to port %d\n", CLT_INFO_GET(i).port);
        }

    }
    
    pon_eoc_timer_start();
    return 0;
}
cs_uint8 pon_eoc_pkt_proc(cs_pkt_t *pkt)
{
    cs_uint8 *ddata;
    cs_uint32 l2_len = 0;
    cs_pkt_t *packet_tmp;
    cs_ether_header_t *eth_hdr;
    cs_int32 ret = 0;
    cs_callback_context_t     context;
    cs_uint16 sport = 0, dport = 0;
    pon_eoc_onu_info_t *onu_info;
    pon_eoc_pkt_req_hdr_t *req;
    pon_eoc_pkt_resp_hdr_t *resp;
    bcmp_cb_t *bcmp_cb = &dhcp_option_cb.bcmp_cb;
    
    if(!pkt) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: %s invalid packet\n", __func__);
        return CS_E_OK;
    }
    if(pkt->port == 0) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: %s ignore packet from PON port\n", __func__);
        return CS_E_OK;
    }
    pkt_dump("PON_EOC: receive\n", pkt->data + pkt->offset, pkt->len, APP_IPINTF_LOG_PKT_PONEOC);
    ret = vlan_ingress_filter(pkt);
    if(ret != CS_E_OK) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO,"PON_EOC: %s Drop due to invalid vlan\n", __func__);
        return 1;
    }
    sport = dport = pkt->port%CLT_INFO_PORT_MAX;
    //Get CBAT mac address
    l2_len = sizeof(cs_ether_header_t) + (pkt->tag_num << 2);
    req = (pon_eoc_pkt_req_hdr_t*)(pkt->data + pkt->offset + l2_len);
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "PON_EOC: %s,  message type %d, clt ip 0x%x\n", __func__,req->msg_type, ntohl(req->clt_ip));
    //APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "PON_EOC: recvive packet from %d\n", sport);
    //buffer_dump("", pkt->data + pkt->offset, pkt->len);
    CLT_INFO_GET(sport).dw_clt_ip = ntohl(req->clt_ip);
    memcpy(CLT_INFO_GET(sport).mac, pkt->data + pkt->offset+6, 6);
    CLT_INFO_GET(sport).port = sport;
    CLT_INFO_GET(sport).state = CLT_INFO_STATE_SUCC;
    CLT_RESET_RETRY_GET(sport) = 0;

    /*
    * Build BCMP IP format
    * 1. Build l2 header:dmac/src/eth_type
    * 2. Build BCMP IP header, 4.2 for reference
    * 3. Build BCMP IP payload, 4.2.1 for reference
    */
    memset(pkt_dummy, 0 ,sizeof(pkt_dummy));
    packet_tmp = (cs_pkt_t *)pkt_dummy;
    packet_tmp->offset = CS_PKT_OFFSET;
    ddata = packet_tmp->data+packet_tmp->offset;
    //Build l2 header
    eth_hdr = (cs_ether_header_t *)ddata;
    memcpy(&(eth_hdr->dst), CLT_INFO_GET(sport).mac, 6);
    memcpy(&(eth_hdr->src), ipintf_info.mac, 6);
    eth_hdr->ethertype = htons(ETHER_PON_EOC_TYPE);
    ddata += sizeof(cs_ether_header_t);
    // Build pon eoc header
    resp = (pon_eoc_pkt_resp_hdr_t *)ddata;
    resp->msg_type = PON_EOC_TYPE_RESP;
    resp->onu_type = htons(CLT_ONU_TYPE_GET());
    resp->port = sport;
    ddata += sizeof(pon_eoc_pkt_resp_hdr_t);
    //
    memcpy(ddata, g_onu_id, sizeof(g_onu_id));
    ddata += sizeof(g_onu_id);
    //


    
    onu_info = (pon_eoc_onu_info_t *)ddata;
    onu_info->shelf_id = bcmp_cb->ani_rack;
    onu_info->frame_id = bcmp_cb->ani_frame;
    onu_info->slot_id = bcmp_cb->slot_id;
    onu_info->subslot_id = bcmp_cb->sub_slot_id;
    onu_info->pon_id = bcmp_cb->olt_id;
    onu_info->onu_id = bcmp_cb->onu_id;
    
    packet_tmp->len = sizeof(cs_ether_header_t) + sizeof(pon_eoc_pkt_resp_hdr_t)+sizeof(g_onu_id)+sizeof(pon_eoc_onu_info_t)+8;
    //vlan_egress_handle(pkt->port, pkt->port, packet_tmp->data+packet_tmp->offset, buffer, &len);
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "PON_EOC: onu type 0x%x\n", CLT_ONU_TYPE_GET());
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "PON_EOC: onu id %s\n", g_onu_id);
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "PON_EOC: %d/%d/%d/%d/%d/%d\n", onu_info->shelf_id, onu_info->frame_id,
        onu_info->slot_id, onu_info->subslot_id, onu_info->pon_id, onu_info->onu_id);
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "PON_EOC: sending packet to %d, len %d\n", dport, packet_tmp->len);
    //buffer_dump("", packet_tmp->data+packet_tmp->offset, packet_tmp->len);
    pkt_dump("PON_EOC: send\n", packet_tmp->data + packet_tmp->offset, packet_tmp->len, APP_IPINTF_LOG_PKT_PONEOC);
    epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
		     packet_tmp->data+packet_tmp->offset, packet_tmp->len, dport,7);

    return 1;
}

static cs_int32 dhcp_packet_enable(cs_boolean enable)
{
    cs_callback_context_t context;
    
    epon_request_onu_spec_pkt_state_set(context, 0, 0, S_UPSTREAM, CS_PKT_DHCP, enable?DST_CPU:DST_FE);
    epon_request_onu_spec_pkt_state_set(context, 0, 0, S_DOWNSTREAM, CS_PKT_DHCP, DST_FE); 
    
    if(enable) {
        app_pkt_reg_parser(CS_PKT_DHCP, app_ipintf_ip_pkt_parser);
        app_pkt_reg_handler(CS_PKT_DHCP, app_ipintf_pkt_recv);
    }
    else {
        app_pkt_reg_parser(CS_PKT_DHCP, NULL);
        app_pkt_reg_handler(CS_PKT_DHCP, NULL);
    }
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "%s: %s DHCP trapping\n", __func__, enable?"enable":"disable");
    return 0;
}

cs_int32 dhcp_option_enable(const dhcp_option_enable_t *enable)
{
    dhcp_option_cb_t *dh_op = &dhcp_option_cb;
    
    if(!enable)
        return -1;
    if(enable->mask & DHCP_V4_OPTION60_MASK_BIT)
        dh_op->op60.enable = enable->v4_option60;
    if(enable->mask & DHCP_V4_OPTION82_MASK_BIT)
        dh_op->op82.enable = enable->v4_option82;
    if(enable->mask & DHCP_V4_OPTION125_MASK_BIT)    
        dh_op->op125.enable = enable->v4_option125;
    if(enable->mask & DHCP_V6_OPTION18_MASK_BIT)
        dh_op->op18.enable = enable->v6_option18;
    if(enable->mask & DHCP_V6_OPTION37_MASK_BIT)
        dh_op->op37.enable = enable->v6_option37;
    if(enable->mask & PPPOE_PLUS_MASK_BIT)
        dh_op->pppoe.enable = enable->pppoe_plus;
    
    if(enable->mask & BCMP_MASK_BIT)
        dh_op->bcmp_cb.enable = enable->bcmp;
    
    if(dh_op->op60.enable || dh_op->op82.enable || dh_op->op125.enable||
       dh_op->op18.enable || dh_op->op37.enable ) {
        dhcp_packet_enable(TRUE);
    }
    else {
        dhcp_packet_enable(FALSE);
    }
    pppoe_plus_enable(dh_op->pppoe.enable);
    
    return 0;
}

cs_int32 dhcp_option_enable_get(dhcp_option_enable_t *enable)
{
    dhcp_option_cb_t *dh_op = &dhcp_option_cb;
    
    enable->v4_option60 = dh_op->op60.enable;
    enable->v4_option82 = dh_op->op82.enable;
    enable->v4_option125 = dh_op->op125.enable;
    enable->v6_option18 = dh_op->op18.enable;
    enable->v6_option37 = dh_op->op37.enable;
    enable->pppoe_plus = dh_op->pppoe.enable;
    enable->bcmp = dh_op->bcmp_cb.enable;

    return 0;
}

#endif //DHCP_OPTION_PPPOE_PLUS_SUPPORT


#ifdef HAVE_ZTE_SIJIE

#include <errno.h>
#include <lwip/netif.h>
#include <lwip/err.h>
#include <lwip/tcpip.h>
#include <lwip/icmp.h>
#include <lwip/sockets.h>
#include <lwip/dhcp.h>

//ZTE 
typedef enum {
    EOC_PON_EVT_CONFIG_INVALID = 0x00,
    EOC_PON_EVT_CONFIG_VLANIP = 0x01,
    EOC_PON_EVT_CONFIG_RESET = 0x02,
    EOC_PON_EVT_CONFIG_MAX = 0x03
}eoc_pon_evt_config_e;

typedef enum {
    EOC_PON_EVT_ACK = 0x00,
    /*
    * eoc_pon_evt_config_e
    * 1: vlan/ip config
    * 2: reset
    */
    EOC_PON_EVT_CFG = 18001,
    EOC_PON_EVT_RESP = 18002 //EOC-->PON
}eoc_pon_evt_e;
typedef enum {
    EOC_PON_EVT_TYPE_ASYN = 0x01, //ASYN
    EOC_PON_EVT_TYPE_CHECK = 0x5a
}eoc_pon_evt_type_e;

#define EOC_PON_CLIENT_IP 0x02020214 //2.2.2.20
#define EOC_PON_SERVER_IP 0x02020202 //2.2.2.2
#define EOC_PON_SERVER_PORT 5001 //5001
#define EOC_PON_ACK_TIMEOUT_MAX 8 //8s
#define EOC_PON_QUEUE_WAIT_RETRY 50000 //5s
#define EOC_PON_SELECT_INTERVAL 1// 1s
#define EOC_PON_CONN_RETRY 5000// 5s
typedef struct {
    cs_int16 tol_len; /* header + body */
    cs_int16 evt_id; //event type: ack/config/reset, etc
    cs_int16 recv_pid; //receive process id
    cs_int16 recv_slot; //receive slot id
    cs_int16 send_pid; //send process id
    cs_int16 send_slot; //send slot id
    cs_int8 evt_type;  //eoc_pon_evt_type_e
    cs_int8 rev;//reserved
    cs_int8 module;	
    cs_int8 left;	
    cs_int8 commultset;	
    cs_int8 dummy[1];
}__attribute__((packed)) eoc_pon_msg_hdr_t;
typedef struct {
    cs_uint32 len;
    cs_uint32 mutex;
    cs_int32 pending_tm;//resend this packet after "wait"
    void *data;//dynamic allocated
}eoc_pon_evt_pending_t;

typedef struct eoc_pon_controlbuffer_t_{
    cs_uint16 sport;//tcp port
    cs_uint16 dport; //tcp port
    cs_uint32 dip;//destination ip
    cs_uint32 rx_id;//thread id receive packet from network
    cs_uint32 tx_id; //thread id send packet out
    //fd: it should be protected
    cs_int32 fd;
    //mutex: used for protecting fd
    cs_uint32 mutex;
    //retries: this field is accessed orderly, so it is unncessary to protect
    cs_uint32 retries;
    cs_uint32 pending_bitmap;//
    cs_uint32 pending_tm;
    eoc_pon_evt_pending_t pending[EOC_PON_EVT_CONFIG_MAX];
}eoc_pon_cb_t;


#define  EOC_PON_MSG_BODY_MAX               256 
typedef struct {
    cs_uint8  msg_type; // 1
    cs_uint8  msg_state_ar; // 1
    cs_uint8  msg_state_ak; // 0
    cs_uint16 msg_len; // 14
    cs_uint8  msg_content[0];
}__attribute__((packed))eoc_pon_msg_body_t;

typedef struct {
    cs_uint32 ip;
    cs_uint32 mask;
    cs_uint32 gw;
    cs_uint16 vlan;
}__attribute__((packed))eoc_pon_msg_vlan_ip_t;
#define RESET_LEN  275 //(sizeof(eoc_pon_msg_hdr_t) + sizeof(eoc_pon_msg_body_t))
#define VLANIP_LEN 275 //(sizeof(eoc_pon_msg_hdr_t) + sizeof(eoc_pon_msg_body_t)+sizeof(eoc_pon_msg_vlan_ip_t))
#define ACK_LEN  18 //(sizeof(eoc_pon_msg_hdr_t) 


eoc_pon_cb_t eoc_pon_cb;
cs_int32 eoc_pon_conn_init(eoc_pon_cb_t *cb)
{
    cs_int32 fd;
    cs_int32 ret = 0;
    struct sockaddr_in serv_addr;

    /* create socket*/
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "socket create failed, fd %d\n", fd);
        return CS_E_ERROR;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(0);
    ret = bind(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret < 0){
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "bind fd %d failed\n", fd);
        goto CLOSE_FD_EXIT;
    }
    
   /* connect to server */
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_len = sizeof(serv_addr);
    serv_addr.sin_port = htons(cb->dport);
    serv_addr.sin_addr.s_addr = htonl(cb->dip);
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: ip addr 0x%x, port %d\n", cb->dip, cb->dport);
    ret = connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));  
    if(ret < 0) {  
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "connect failed, ret %d\n", ret);
        goto CLOSE_FD_EXIT;
    }  

    cb->fd = fd;
    cb->retries = 0;

    return 0;

CLOSE_FD_EXIT:
    close(fd);
    return -1;

}
void eoc_pon_conn_close(eoc_pon_cb_t *cb)
{
    if(cb->fd >= 0)
        close(cb->fd);
    cb->fd = -1;
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: tear down TCP connection\n");
}

static cs_int32 eoc_pon_msg_hdr_build(eoc_pon_cb_t *cb, cs_uint16 evt_id, cs_uint8 evt_type, cs_uint32 len, void *data)
{
    eoc_pon_msg_hdr_t *hdr = data;

    memset(hdr, 0, sizeof(eoc_pon_msg_hdr_t));
    hdr->tol_len = htons(len);
    hdr->evt_id = htons(evt_id);
    hdr->recv_pid = htons(cb->tx_id);
    hdr->recv_slot = htons(0x01);
    hdr->send_pid = htons(cb->tx_id);
    hdr->send_slot = htons(0x01);
    hdr->evt_type = evt_type;
    hdr->rev = 0;
    hdr->module = 0;
    hdr->left = 0;
    hdr->commultset = 0x01;
    
    return sizeof(eoc_pon_msg_hdr_t);
}

cs_uint32 eoc_pon_reboot_build(eoc_pon_cb_t *cb,void *data)
{
    eoc_pon_msg_hdr_t *hdr = data;
    eoc_pon_msg_body_t *body;

    eoc_pon_msg_hdr_build(cb, EOC_PON_EVT_CFG, EOC_PON_EVT_TYPE_ASYN, RESET_LEN, data);
    body = (eoc_pon_msg_body_t *)(hdr + 1);
    memset(body, 0, sizeof(eoc_pon_msg_body_t));
    body->msg_type = EOC_PON_EVT_CONFIG_RESET;
    body->msg_state_ar = 1;
    body->msg_state_ak = 0;
    body->msg_len = 0;//htons
    
    return RESET_LEN;
}

cs_uint32 eoc_pon_ip_cfg_build(eoc_pon_cb_t *cb, cs_uint16 vid, cs_uint32 ip, cs_uint32 mask, cs_uint32 gw, void *data)
{
    eoc_pon_msg_hdr_t *hdr = data;
    eoc_pon_msg_body_t *body;
    eoc_pon_msg_vlan_ip_t *vlan;
    
    eoc_pon_msg_hdr_build(cb, EOC_PON_EVT_CFG, EOC_PON_EVT_TYPE_ASYN, VLANIP_LEN, data);

    body = (eoc_pon_msg_body_t *)(hdr + 1);
    memset(body, 0, sizeof(eoc_pon_msg_body_t));
    body->msg_type = EOC_PON_EVT_CONFIG_VLANIP;
    body->msg_state_ar = 1;
    body->msg_state_ak = 0;
    body->msg_len = htons(sizeof(eoc_pon_msg_vlan_ip_t));
    
    vlan = (eoc_pon_msg_vlan_ip_t *)(body + 1);
    memset(vlan, 0, sizeof(eoc_pon_msg_vlan_ip_t));
    vlan->ip = htonl(ip);
    vlan->mask = htonl(mask);
    vlan->gw = htonl(gw);
    vlan->vlan = htons(vid);
    
    return VLANIP_LEN;
}  

cs_int32 eoc_pon_data_send(eoc_pon_cb_t *cb, cs_uint32 len, void *data)
{
    cs_int32 ret = 0;
    
    if(cb->fd >= 0)
        send(cb->fd, data, len, 0);

    return ret;
}

void eco_pon_evt_save(eoc_pon_cb_t *cb, cs_uint8 type, void *data)
{
    cs_uint32 lens[3] = {0, VLANIP_LEN, RESET_LEN};
    const cs_uint8 *event_name[] = {"unknown", "VLAN_IP","REBOOT"};
    //lock
    cs_mutex_lock(cb->mutex);
    if(cb->pending[type].data) {
        free(cb->pending[type].data);
    }
    cb->pending[type].len = lens[type];
    cb->pending[type].data = data;
    cb->pending[type].pending_tm = 0;
    cb->pending_bitmap |= (1<<type);
    cs_mutex_unlock(cb->mutex);
    APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: save event %s\n", event_name[type]);
}
void eoc_pon_evt_del(eoc_pon_cb_t *cb, cs_uint8 type)
{
    const cs_uint8 *event_name[] = {"unknown", "VLAN_IP","REBOOT"};
    cs_mutex_lock(cb->mutex);
    if(cb->pending_bitmap & (1<<type)) {
        cb->pending_bitmap &= ~(1<<type);
        if(cb->pending[type].data)
            free(cb->pending[type].data);
        cb->pending[type].data = NULL;
        cb->pending[type].len = 0;
        cb->pending[type].pending_tm = 0;
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: del event %s from queue\n", event_name[type]);
    }
    cs_mutex_unlock(cb->mutex);
}

void eoc_pon_evt_send(eoc_pon_cb_t *cb)
{
    cs_uint32 i = 0;
    cs_uint64 current = 0;
    const cs_uint8 *event_name[] = {"unknown", "VLAN_IP","REBOOT"};
    
    cs_mutex_lock(cb->mutex);
    current = cs_current_time();
    for(i=0; i<EOC_PON_EVT_CONFIG_MAX; i++) {
        if(cb->pending_bitmap & (1<< i)) {
            //pending_tm == 0: the first time send this configuration
            //pending_tm !=0: resend this configuration every 4s
            if(!(cb->pending[i].pending_tm) || (current - cb->pending[i].pending_tm >= 4000)) {
                APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: send pending event %s\n", event_name[i]);
                eoc_pon_data_send(cb, cb->pending[i].len, cb->pending[i].data);
                cb->pending[i].pending_tm = current;
            }
        }
    }

    cs_mutex_unlock(cb->mutex);
    return;
}

void eoc_pon_ack_send(eoc_pon_cb_t *cb)
{
    static cs_int32 len = 0;
    static cs_uint8 ack[128];
    
    if(!len) {
        len = eoc_pon_msg_hdr_build(cb, EOC_PON_EVT_ACK, EOC_PON_EVT_TYPE_CHECK, sizeof(eoc_pon_msg_hdr_t), ack);
    }
    eoc_pon_data_send(cb, len, ack);
    cb->retries++;
    return;
}

void eoc_pon_packed_resp_proc(eoc_pon_cb_t *cb, cs_int32 len, cs_uint8 *resp)
{
    cs_uint8 *data = resp;
    static cs_uint8 remains[640];
    static cs_int32 remain = 0;
    cs_int32 offset = 0;
    eoc_pon_msg_hdr_t *hdr;
    eoc_pon_msg_body_t *body;
    const cs_uint8 *event_name[] = {"unknown", "VLAN_IP","REBOOT"};
    
    if(!resp)
        return;
    if(remain > 0) {
        memcpy(&remains[remain], resp, len);
        data = remains;
        len += remain;
        remain = 0;
    }
        
    while(len > offset) {
        remain = len-offset;
        hdr = (eoc_pon_msg_hdr_t *)(data+offset);
        switch(ntohs(hdr->evt_id)) {
            case EOC_PON_EVT_ACK:
                cb->retries = 0;
                offset += ACK_LEN;
                if(len < offset) {
                    break;
                }
                APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: receive ACK\n");
                break;
                
            case EOC_PON_EVT_RESP:
                offset += VLANIP_LEN;
                if(len < offset) {
                    break;
                }
                body = (eoc_pon_msg_body_t *)(hdr + 1);
                if(body->msg_type >=  EOC_PON_EVT_CONFIG_MAX) {
                    return;
                }
                APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: recv %s response\n", event_name[body->msg_type]);
                eoc_pon_evt_del(cb, body->msg_type);

                break;
            default:
                APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: recv invalid event id %d\n", ntohs(hdr->evt_id));
                return;
        }
        
    }
    if(len < offset) {
         //less than one packet, copy it to buffer
        memmove(remains, data+(len-remain), remain);
        APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: no more data, remain %d\n", remain);
    }
    else {
        if(len > offset) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: more data\n");
        }
        else {
            remain = 0;
        }
    }
}


void eoc_pon_resp_proc(eoc_pon_cb_t *cb, cs_int32 len, cs_uint8 *resp)
{
    cs_uint8 *data = resp;
    eoc_pon_msg_hdr_t *hdr;
    eoc_pon_msg_body_t *body;
    const cs_uint8 *event_name[] = {"unknown", "VLAN_IP","REBOOT"};
    
    if(!resp)
        return;
        
    hdr = (eoc_pon_msg_hdr_t *)(data);
    switch(ntohs(hdr->evt_id)) {
        case EOC_PON_EVT_ACK:
            cb->retries = 0;
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: receive ACK\n");
            break;
            
        case EOC_PON_EVT_RESP:
            body = (eoc_pon_msg_body_t *)(hdr + 1);
            if(body->msg_type >=  EOC_PON_EVT_CONFIG_MAX) {
                return;
            }
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: recv %s response\n", event_name[body->msg_type]);
            eoc_pon_evt_del(cb, body->msg_type);

            break;
        default:
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: recv invalid event id %d\n", ntohs(hdr->evt_id));
            return;
    }
        

}




void eoc_pon_rx_entry(cs_uint32 param)
{
    fd_set in_fds;
    cs_int32 ret = 0;
    cs_int32 len = 0;
    cs_uint8 buffer[256+64];
    struct timeval timeout;
    cs_uint64 start = cs_current_time();
    eoc_pon_cb_t *cb = (eoc_pon_cb_t *)param;
    
    /* client timeout setting */
    memset(&timeout, 0, sizeof(struct timeval));
    timeout.tv_sec = EOC_PON_SELECT_INTERVAL;
    //workaround for telnet: to make sure eoc_pon can't get fd 0
    cs_thread_delay(EOC_PON_CONN_RETRY);
RETRY: 
    ret = eoc_pon_conn_init(cb);
    if(ret < 0) {
        cs_thread_delay(EOC_PON_CONN_RETRY);
        goto RETRY;
    }

    while(1) {
        FD_ZERO(&in_fds);
        FD_SET(cb->fd, &in_fds); 
        ret = select(cb->fd + 1, &in_fds, NULL, NULL, &timeout);         
        /* select error */
        if(ret < 0) {
            if (errno == EINTR){
                continue;
            }
            /* tear down the connection */
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: select error, ret %d\n", ret);
            break;
        }
        //send pending configuration request
        if(cb->pending_bitmap) {
            eoc_pon_evt_send(cb);
        }
        if(cb->retries >= EOC_PON_ACK_TIMEOUT_MAX) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: no ACK from remote\n");
            break;
        }
        //if timeout, send ACK to EOC
        if (ret == 0){
            eoc_pon_ack_send(cb);
            start = cs_current_time();
            continue;
        }
        //Got response from EOC
        len = read(cb->fd, buffer, sizeof(buffer));
        if(len <= 0){
            APP_IPINTF_LOG(APP_IPINTF_LOG_INFO, "EOC_PON: TCP connection released by remote\n");
            break;
        }
        eoc_pon_resp_proc(cb, len, buffer);
        //check if ACK missed
        if(cs_current_time()-start >= 1000) {
            eoc_pon_ack_send(cb);
            start = cs_current_time();
        }
    
    }
    
    eoc_pon_conn_close(cb);
    goto RETRY;

    return ;

}
void eoc_pon_cb_init(eoc_pon_cb_t *cb)
{
    cs_uint32 i = 0;
    memset(cb, 0, sizeof(eoc_pon_cb_t));
    cb->dip = EOC_PON_SERVER_IP;
    cb->dport = EOC_PON_SERVER_PORT;
    cb->fd = -1;
    cs_mutex_init(&(cb->mutex),"eoc_pon_mutex",0);
    for(i=EOC_PON_EVT_CONFIG_VLANIP; i<EOC_PON_EVT_CONFIG_MAX; i++) {
        cs_mutex_init(&(cb->pending[i].mutex),"eoc_data_mutex", 0);
    }
}
void eoc_pon_init()
{
    eoc_pon_cb_t *eoc_cb = &eoc_pon_cb;
    eoc_pon_cb_init(eoc_cb);
    cs_thread_create(&(eoc_cb->rx_id), "eoc_rx_pon", eoc_pon_rx_entry,(void *)eoc_cb,
        EOC_PON_THREAD_STACKSIZE, EOC_PON_THREAD_PRIORITY, 0);        
     
    cs_printf("EOC_PON: init done\n");
}

//APIs
cs_int32 eoc_pon_vlanip_set(cs_uint16 vid, cs_uint32 ip, cs_uint32 mask, cs_uint32 gw)
{
    cs_uint8 *data = NULL;

    data = (cs_uint8 *)malloc(VLANIP_LEN);
    if(!data) {
        return -1;
    }
    memset(data, 0, VLANIP_LEN);
    cs_printf("EOC_PON: vid %d, ip 0x%x, mask 0x%x, gw 0x%x\n", vid, ip, mask, gw);
    eoc_pon_ip_cfg_build(&eoc_pon_cb, vid, ip, mask, gw, data);
    eco_pon_evt_save(&eoc_pon_cb, EOC_PON_EVT_CONFIG_VLANIP, data);
    //the receiver will free this memory
    
    return 0;
}
cs_int32 eoc_pon_reset_set(void)
{
    cs_uint8 *data = NULL;

    data = (cs_uint8 *)malloc(RESET_LEN);
    if(!data) {
        return -1;
    }
    memset(data, 0, RESET_LEN);
    cs_printf("EOC_PON: reboot\n");
    eoc_pon_reboot_build(&eoc_pon_cb, data);
    eco_pon_evt_save(&eoc_pon_cb, EOC_PON_EVT_CONFIG_RESET, data);
    cs_thread_delay(2000);
    return 0;
}

#endif /*END_HAVE_ZTE_SIJIE*/

