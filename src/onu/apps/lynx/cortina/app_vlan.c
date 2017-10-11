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
#include "plat_common.h"
#include "packet.h"
#include "osal_api_packet.h"
#include "sdl_api.h"
#include "app_vlan.h"
extern cs_status vlan_ingress_rule_chk(cs_pkt_t *buf);
extern cs_status vlan_egress_rule_handle(
         cs_port_id_t sport, cs_port_id_t dport,cs_uint8 *in_pkt,cs_uint8 *out_pkt,cs_uint32 *len);
extern cs_status vlan_flooding_pkt_per_port(cs_port_id_t dport,cs_uint8 pri,cs_uint8 *in,cs_uint16 len);

/*
*   PROTOTYPE    cs_status vlan_ingress_filter(cs_pkt_t *pkt)
*   INPUT            pkt
*   OUTPUT         None.
*   Description     Apply ingress rule on packet.
*                       If the packet match the ingress vlan rule, return CS_E_OK;
*                       Otherwise, return CS_E_ERROR.
*   Pre-condition  
*   Post-condition
*   Note            If the packet is modified(pop, push or swap, etc.), the return value is CS_E_OK, too.
*/
cs_status vlan_ingress_filter(cs_pkt_t *pkt)
{
    cs_status ret = CS_E_OK;
    if(!pkt)
    {
        return -1;
    }

    ret = vlan_ingress_rule_chk(pkt);
    if(ret == CS_E_ERROR) {
        return CS_E_ERROR;
    }
    
    return CS_E_OK;

}

/*
*   PROTOTYPE    cs_status vlan_egress_handle(cs_port_id_t, cs_port_id_t,cs_uint8*,cs_uint8*,cs_uint32*)
*   INPUT            dport
*                       *in
*                       *out
*                       *len
*   OUTPUT         None.
*   Description     Apply egress rule on packet.
*   Pre-condition  
*   Post-condition
*   Note             If there is any action on the packet, the OUT parameter(out) will be updated, and
*                      the return code is bigger than 0. If the packet match the vlan rule and does not modify 
*                      at all, the return code is 0.
*/
cs_status vlan_egress_handle(
                                        cs_port_id_t sport, 
                                        cs_port_id_t dport ,
                                        cs_uint8 *in,
                                        cs_uint8*out,
                                        cs_uint32 *len)
{
    cs_status ret = CS_E_OK;
    if(!in || !out || !len)
    {
         return -1;
    }
    
    ret= vlan_egress_rule_handle(sport, dport,in,out,len);
    if(ret == CS_E_ERROR)
        return CS_E_ERROR;
    
    return ret;
}

cs_status vlan_header_parse(cs_uint8 *buf,app_vlan_double_tag_header_t *vlanHdr)
{
    cs_ether_header_t *pEthHeader = NULL;
    cs_uint16 ethtype = 0;
    cs_uint8 *pbuf = buf;
    pEthHeader = (cs_ether_header_t *)pbuf;
    ethtype = ntohs(pEthHeader->ethertype);
    if(ethtype == EPON_ETHERTYPE_DOT1Q || ethtype == EPON_ETHERTYPE_TPID_SVLAN||
        ethtype == EPON_ETHERTYPE_TPID_SVLAN_OLD)
    {
        pbuf += 2;
        pEthHeader = (cs_ether_header_t *)pbuf;
        vlanHdr->stpid = ethtype;
        vlanHdr->svlan = ntohs(pEthHeader->ethertype);
        vlanHdr->svlan &= 0xfff;
        pbuf += 2;
        pEthHeader = (cs_ether_header_t *)pbuf;
        ethtype = ntohs(pEthHeader->ethertype);
        if(ethtype == EPON_ETHERTYPE_DOT1Q || ethtype == EPON_ETHERTYPE_TPID_SVLAN||
            ethtype == EPON_ETHERTYPE_TPID_SVLAN_OLD)
        {
            pbuf += 2;
            pEthHeader = (cs_ether_header_t *)pbuf;
            vlanHdr->ctpid = ethtype;
            vlanHdr->cvlan = ntohs(pEthHeader->ethertype);
            vlanHdr->cvlan &= 0xfff;
            pbuf += 2;
            pEthHeader = (cs_ether_header_t *)pbuf;
            ethtype = ntohs(pEthHeader->ethertype);
        }
    }
    vlanHdr->ethertype = ethtype;
    
    return CS_E_OK;
}

cs_status vlan_header_parse_2pkt(cs_uint8 *buf,cs_uint32 len, cs_pkt_t *pkt)
{
    cs_ether_header_t *pEthHeader = NULL;
    cs_uint16 ethtype = 0;
    cs_uint8 *pbuf = buf;
    pEthHeader = (cs_ether_header_t *)pbuf;
    ethtype = ntohs(pEthHeader->ethertype);
    if(ethtype == EPON_ETHERTYPE_DOT1Q || ethtype == EPON_ETHERTYPE_TPID_SVLAN||
        ethtype == EPON_ETHERTYPE_TPID_SVLAN_OLD)
    {
        pbuf += 2;
        pEthHeader = (cs_ether_header_t *)pbuf;
        pkt->stpid = ethtype;
        //Note: svlan has the priority information
        pkt->svlan = ntohs(pEthHeader->ethertype);
        //pkt->svlan &= 0xfff;
        pkt->tag_num++;
        pbuf += 2;
        pEthHeader = (cs_ether_header_t *)pbuf;
        ethtype = ntohs(pEthHeader->ethertype);
        if(ethtype == EPON_ETHERTYPE_DOT1Q || ethtype == EPON_ETHERTYPE_TPID_SVLAN||
            ethtype == EPON_ETHERTYPE_TPID_SVLAN_OLD)
        {
            pbuf += 2;
            pEthHeader = (cs_ether_header_t *)pbuf;
            pkt->ctpid = ethtype;
            //Note: cvlan has the priority information
            pkt->cvlan = ntohs(pEthHeader->ethertype);
            //pkt->cvlan &= 0xfff;
            pkt->tag_num++;
            pbuf += 2;
            pEthHeader = (cs_ether_header_t *)pbuf;
            ethtype = ntohs(pEthHeader->ethertype);
        }
    }
    pkt->eth_type = ethtype;
    pkt->len = len;
    
    return CS_E_OK;
}

void pop_vlan(cs_uint8 *in, cs_uint8 *out,cs_uint32 *len)
{
	if(in != out) {
		memcpy(out, in, ETH_L2_HEADER_LEN);	/* copy da&sa */
		memcpy(out+ETH_L2_HEADER_LEN, in+16, *len-16);
	}
	else {	
		memmove(out+ETH_L2_HEADER_LEN,out+16,*len -16);
	}
	
	*len -= sizeof(app_vlan_header_t);
}

void push_vlan(cs_uint16 vlan, cs_uint8 *in, cs_uint8 *out,cs_uint32* len)
{
	app_vlan_header_t vlanHeader;

	vlanHeader.tpid = htons(0x8100);
	vlanHeader.vlan = htons(vlan);

	if(in == out) {
		memmove(out+16,out+ETH_L2_HEADER_LEN,*len - ETH_L2_HEADER_LEN);
		memmove(out+ETH_L2_HEADER_LEN,&vlanHeader,sizeof(app_vlan_header_t));
	}
	else {
		memcpy(out, in, ETH_L2_HEADER_LEN);
		memcpy(out+ETH_L2_HEADER_LEN,&vlanHeader,sizeof(app_vlan_header_t));
		memcpy(out+16, in+ETH_L2_HEADER_LEN, *len-ETH_L2_HEADER_LEN);
	}
	
	*len += sizeof(app_vlan_header_t);
}

void push_vlan_ingress(cs_uint16 vlan, cs_uint8 pri, cs_pkt_t  *pkt)
{
    cs_uint8 * buf = pkt->data+pkt->offset;
    app_vlan_header_t vlanHeader;

    vlanHeader.tpid = htons(0x8100);
    vlanHeader.vlan = htons(vlan & VLAN_MASK_BIT);
    vlanHeader.vlan |= htons(pri << PRI_BIT_OFFSET);
    
    memmove(buf-sizeof(app_vlan_header_t),buf,ETH_L2_HEADER_LEN);
    memmove(buf+ETH_L2_HEADER_LEN-sizeof(app_vlan_header_t),&vlanHeader,sizeof(app_vlan_header_t));

    pkt->len +=sizeof(app_vlan_header_t);
    pkt->offset -=sizeof(app_vlan_header_t);
    if(pkt->tag_num) {
        pkt->ctpid = pkt->stpid;
        pkt->cvlan = pkt->svlan;
    }
    pkt->svlan = vlan & VLAN_MASK_BIT;
    pkt->stpid = 0x8100;
    pkt->tag_num++;
}

/* 
* swap the outest vlan.
* use default TPID and keep the pri not changed.
* TODO : Is it necessary to keep the same TPID?
*/
void swap_vlan(cs_uint16 vlan1, cs_uint16 vlan2,cs_uint8 *in, cs_uint8 *out, cs_uint32 len)
{
    app_vlan_header_t vlanHeader;
    app_vlan_header_t *pVlanHeader = NULL;

    pVlanHeader = (app_vlan_header_t *)(in + ETH_L2_HEADER_LEN);
    memset(&vlanHeader, 0, sizeof(app_vlan_header_t));
    vlanHeader.tpid = htons(0x8100);
    vlanHeader.vlan = htons(vlan2 & VLAN_MASK_BIT);
    vlanHeader.vlan |= (pVlanHeader->vlan&(htons(PRI_MASK_BIT)));
    
    if(in != out) {
        memcpy(out, in, len);
    }

    memcpy(out+ETH_L2_HEADER_LEN, &vlanHeader, sizeof(app_vlan_header_t));
    
}

/*
*   PROTOTYPE    cs_status vlan_flooding_per_port(cs_port_id_t dport,cs_uint8 pri,cs_uint8 *in,cs_uint16 len)
*   INPUT            cs_port_id_t dport : destination port id.
                          cs_uint8 pri          : p-bit for the packet.
                          cs_uint8 *in         :  address of packet without vlan.
                          cs_uint16 len       : len of the packet
*   OUTPUT         None
*   Description     1) sending packets with vlan(that configured on this port) on this port.
                          2) sending packet without vlan on this port.

*   Pre-condition  The parameter "in" should be without vlan.
*/
cs_status vlan_flooding_per_port(cs_port_id_t dport,cs_uint8 pri,cs_uint8 *in,cs_uint16 len)
{
	if(in == NULL || len == 0)
	{
		return -1;
	}
	return vlan_flooding_pkt_per_port(dport,pri,in,len);
}

/*
*   PROTOTYPE    cs_boolean vlan_vid_port_validate(cs_port_id_t port, cs_uint16 vlan)
*   INPUT            cs_port_id_t port : 
                         cs_uint16 vlan     : vlan id to be verified.
*   OUTPUT         None
*   Description     If this vlan is valid on this port, return TRUE; Otherwise, return FALSE.
*   Pre-condition  
*   Note   
*/
cs_boolean vlan_vid_port_check(cs_port_id_t port, cs_uint16 vlan)
{
    cs_status ret = CS_E_OK;
    cs_uint8 buf[MIN_BUF_LEN_FOR_PKT];
    cs_pkt_t *pkt;

    memset(buf, 0, sizeof(buf));
    
    pkt = (cs_pkt_t*)buf;
    pkt->offset = CS_PKT_OFFSET;
    pkt->len = MIN_BUF_LEN_FOR_PKT-CS_PKT_OFFSET-(sizeof(cs_pkt_t)-1);
    pkt->port = port;
    push_vlan_ingress(vlan, 0, pkt);
    ret = vlan_ingress_filter(pkt);
    if(ret == CS_E_ERROR) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, vlan %d invalid on port %d\n", 
                                                                                    __func__, vlan, port);
        return FALSE;
    }
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, vlan %d valid on port %d\n", 
                                                                                    __func__, vlan, port);
    
    return TRUE;
    
}
