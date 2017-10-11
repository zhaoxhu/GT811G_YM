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
#include "plat_common.h"
#include "packet.h"
#include "osal_api_core.h"
#include "app_vlan.h"
#include "app_ip_util.h"
#include "app_ip_init.h"
#include "app_ip_arp.h"
#ifdef HAVE_QINQ
#include "gwd_qinq.h"
#endif

static cs_uint8 packet_buf[CS_ETHER_MTU];

/*
*   PROTOTYPE    void app_ipintf_build_arp_packet(cs_pkt_t *pkt)
*   INPUT            pkt
*   OUTPUT         pkt.
*   Description     
*   Pre-condition  
*   Post-condition
*   Note             
*/
void app_ipintf_build_arp_packet(cs_pkt_t *pkt)
{
    cs_uint32 ip_addr;
    cs_uint8 *buf = NULL;
    cs_uint8 local_mac[6];
    cs_uint8 dst_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    struct ether_arp *arp = NULL;
    struct ether_header *ether = NULL;
    
    /* get my mac */
    memcpy(local_mac, app_ipintf_get_my_macaddr(), ETHER_ADDR_LEN);
    
    /* fill ethernet header field */
    buf = pkt->data + pkt->offset;
    ether = (struct ether_header *)buf;
    memcpy(ether->ether_dhost, dst_mac, ETHER_ADDR_LEN);
    memcpy(ether->ether_shost, local_mac, ETHER_ADDR_LEN);
    ether->ether_type = htons(EPON_ETHERTYPE_ARP);

    /* fill arp STD header field */
    arp = (struct ether_arp *) (buf+sizeof(struct ether_header));
    arp->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
    arp->ea_hdr.ar_pro = htons(0x0800);
    arp->ea_hdr.ar_hln = 6;
    arp->ea_hdr.ar_pln = 4;
    arp->ea_hdr.ar_op  = htons(ARPOP_REQUEST);

    /* set local ip address and mac address */
    memcpy(arp->arp_sha, local_mac, ETHER_ADDR_LEN);
    ip_addr = htonl(LOCAL_IPADDR);
    memcpy(arp->arp_spa, &ip_addr, sizeof(ip_addr));
    
    /* set target ip address (set mac address to zero) */
    ip_addr = htonl(GATEWAY_IPADDR);
    memcpy(arp->arp_tpa, &ip_addr, sizeof(ip_addr));
    pkt->len = sizeof(struct ether_header) + sizeof(struct ether_arp);
    
    return;
    
}

/*
*   PROTOTYPE    cs_status app_ipintf_arp_keep_alive(void)
*   INPUT            void
*   OUTPUT         None.
*   Description     Send ARP packet with vlanid periodically.
*   Pre-condition  
*   Post-condition
*   Note            
*/
cs_status app_ipintf_arp_keep_alive(void)
{
    cs_status ret = 0;
    cs_uint32 len;
    cs_pkt_t *pkt = NULL;
    
    ret = app_ipintf_check_ip_address(ipintf_info.localip, ipintf_info.ipmask, ipintf_info.gateway);
    if(ret != CS_E_OK) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, " %s, invalid, 0x%8x,0x%8x,0x%8x\n",
            ipintf_info.localip, ipintf_info.ipmask, ipintf_info.gateway);
        return ret;
    }
    
    len = CS_PKT_OFFSET + (sizeof(cs_pkt_t)-1) + ETH_ARP_PKT_LEN;
    pkt = (cs_pkt_t *)iros_malloc(IROS_MID_ETHDRV, len);
    if (NULL == pkt) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "arp keep alive,no memory\n");
        return -1;
    }
    app_ipintf_init_pkt(len, pkt);
    app_ipintf_build_arp_packet(pkt);

    /* TODO : If there is dedicated vlan for keeping alive, push vlan here */
    push_vlan_ingress(ipintf_info.vlanid, ipintf_info.pri, pkt);
    /* send packet to aal */
    ret = app_ipintf_send_pkt(CS_PON_PORT_ID, pkt);
    iros_free(pkt);
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s, send packet, ret %d\n", __func__,ret);
    return ret;
    
}

/*
*   PROTOTYPE    cs_int32 app_ipintf_arp_pre_process(cs_pkt_t* pkt)
*   INPUT            pkt
*   OUTPUT         None.
*   Description     
*   Pre-condition  
*   Post-condition
*   Note            When target IP addres is myself, forward packet to IP stack when one of the following cases match: 
*                     The destination MAC is my own(e.g. ARP response packet);
*                     The destination MAC is broadcast address(e.g. ARP request);
*                      For any other cases, drop it.
          
*/
cs_status app_ipintf_arp_pre_process(cs_pkt_t* pkt)
{
    cs_int32 ret = CS_E_OK;
    cs_uint8 *ptr;
    cs_uint16 i;
    cs_uint32 targetip = 0;
    cs_uint8 bc_mac[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    epon_macaddr_header_t *macHdr;

	#ifdef HAVE_QINQ
	pkt_qinq_vlan_translate(pkt);
	#endif
    IPINTF_STATIS_INC(ARP, pkt->port);
    ptr = pkt->data + pkt->offset;
    
    /* parse target ip address */
    memcpy((cs_uint8*)&targetip, (cs_uint8*)(ptr + pkt->tag_num*4+IPINTF_TARGET_IP_OFFSET), 4);
    targetip = ntohl(targetip);
    
    macHdr = (epon_macaddr_header_t*)ptr;
    if(app_ipintf_my_ipaddr_equal(targetip)) {
        if(app_ipintf_arp_rx_filter) {
            if(app_ipintf_arp_rx_filter(pkt) == IPINTF_DROP) {
                return CS_E_ERROR;
            }
        }
        if(!((app_ipintf_macaddr_equal(macHdr->dst, app_ipintf_get_my_macaddr(), 6))||
            (app_ipintf_macaddr_equal(macHdr->dst, bc_mac, 6)))) {
                APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"Drop due to invalid mac\n");
                return CS_E_ERROR;
        }
    }
    else
    {
        /* 
        * forward broadcast packets
        * For upstream, forward packets to PON port only.
        * For downstream, forward packets to all active UNI ports.
        */
        ret = vlan_ingress_filter(pkt);
        if(ret != CS_E_OK) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"Drop due to invalid vlan\n");
            return ret;
        }

        for(i=0; i<ipintf_info.maxport; i++) {
            if(pkt->port == i) {
                continue;
            }
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "RX to TX len %d,sport %d, dport %d\n", 
                                                                                  pkt->len,pkt->port, PTABLE[i].port);
            ret |= app_ipintf_send_pkt_with_vlan_check(PTABLE[i].port, pkt, packet_buf);
        }
        
        return CS_E_ERROR;
    }

    return ret;
    
}

#endif

