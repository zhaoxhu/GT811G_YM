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
#include "sdl_api.h"
#include "app_vlan.h"
#include "sdl_vlan.h"
#include "app_ip_types.h"
/*
*   PROTOTYPE    cs_status vlan_mac2vlan_get(cs_uint8 *mac, cs_uint16 *vlan, cs_uint32 *port)
*   INPUT            mac
*   OUTPUT         vlan:
*                       port
*   Description     Get vlan id and port id for this MAC.
*                       
*   Pre-condition  
*   Post-condition
*   Note  
*/
cs_status vlan_mac2vlan_get(cs_uint8 *mac, cs_uint16 *vlan, cs_port_id_t *port)
{
    cs_status ret = CS_E_OK;
    cs_uint16 index = 0;
    cs_uint16 next = 0;
    cs_sdl_fdb_entry_t entry;
    cs_callback_context_t context;

    memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));
    while(1) {
        ret = epon_request_onu_fdb_entry_getnext(context, ONU_DEVICEID_FOR_API, 
                    ONU_LLIDPORT_FOR_API, SDL_FDB_ENTRY_GET_MODE_ALL, index, &next, &entry);
        if(ret != CS_E_OK) {
            IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3, IROS_MID_APP, "%s ,get fdb failed %d\n", __func__, ret);
            break;
        }
        if(memcmp(mac, &(entry.mac), 6) == 0) {
            *vlan = entry.vlan_id;
            *port = entry.port;
            IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3, IROS_MID_APP, "%s ,vlan %d, port %d\n",
                                    __func__, entry.vlan_id, entry.port);
            return CS_E_OK;
        }
        index = next+1;
    }
    IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3, IROS_MID_APP, 
                                                        "%s ,no vlan for mac %02X %02X %02X %02X %02X %02X\n",
                                                        __func__, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return CS_E_ERROR;
    
}

/*
*   PROTOTYPE    cs_status vlan_ingress_stacking_mode(cs_pkt_t *pkt, cs_sdl_vlan_tag_t *dft_vlan)
*   INPUT            pkt
*                       dft_vlan
*   OUTPUT         pkt.
*   Description     Apply stacking rule on packet.
*                       1) If default vlan id is zero, drop all packets.
*                       2) If packet has no vlan, push default vlan with default priority.
*                       3) If packet has vlan, push default vlan, the priority should be the same as inner vlan.
*   Pre-condition  
*   Post-condition
*   Note             ZTE only.
*   Return code    CS_E_ERROR  any error occur.
*                       APP_VLAN_RULE_PUSH_FORWARD 
*/
cs_status vlan_ingress_stacking_mode(cs_pkt_t *pkt, cs_sdl_vlan_tag_t *dft_vlan)
{ 
    cs_uint8 *buf;
    cs_uint8 pri = 0;
    cs_uint16 vlanid;
    app_vlan_header_t *pVlanHeader = NULL;

    if(dft_vlan->vid == 0) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_MIN, "%s drop,default vlan id is zero\n", __func__);
        return CS_E_ERROR;
    }
    
    if(pkt->tag_num == 0) {
        push_vlan_ingress(dft_vlan->vid, dft_vlan->pri, pkt);
        APP_VLAN_LOG(IROS_LOG_LEVEL_MIN, "%s ,push default vlan %d\n", __func__, dft_vlan->vid);
        return APP_VLAN_RULE_PUSH_FORWARD;
    }

    /* get the outest vlan tag */
    buf = pkt->data + pkt->offset;
    pVlanHeader = (app_vlan_header_t *)(buf + ETH_L2_HEADER_LEN);
    pri = (ntohs(pVlanHeader->vlan&(htons(PRI_MASK_BIT))))>>PRI_BIT_OFFSET;
    vlanid = dft_vlan->vid;
    push_vlan_ingress(vlanid, pri, pkt);
    APP_VLAN_LOG(IROS_LOG_LEVEL_MIN, "%s ,push vlan %d\n", __func__, dft_vlan->vid);
    
    return APP_VLAN_RULE_PUSH_FORWARD;

}

/*
*   PROTOTYPE    cs_status vlan_ingress_QinQ_mode(
                                            cs_pkt_t *pkt,
                                            cs_sdl_vlan_tag_t *dft_vlan,
                                            cs_sdl_vlan_cfg_t *vlan_cfg,
                                            cs_uint16 num)
*   INPUT            
*                       
*   OUTPUT         
*   Description     Apply QinQ rule on packet.
*   Pre-condition  
*   Post-condition
*   Note             HuaWei only.
*   Return code    
*/
cs_status vlan_ingress_QinQ_mode(
                                            cs_pkt_t *pkt,
                                            cs_sdl_vlan_tag_t *dft_vlan,
                                            cs_sdl_vlan_cfg_t *vlan_cfg,
                                            cs_uint16 num)
{
    cs_uint16 i = 0;
    
    /* If no vlan pair configured, drop all packets(IN and OUT) */
    if(num == 0) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_MIN, "%s drop, vlan pair is empty\n", __func__);
        return CS_E_ERROR;
    }
    
    if(pkt->tag_num == 0) {
        if(dft_vlan->vid == 0) {
            APP_VLAN_LOG(IROS_LOG_LEVEL_MIN, "%s drop,default vlan id is zero\n", __func__);
            return CS_E_ERROR;
        }
        push_vlan_ingress(dft_vlan->vid, dft_vlan->pri, pkt);
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, push default vlan 0x%x\n", __func__, pkt->svlan);
        return APP_VLAN_RULE_PUSH_FORWARD;
    }
    
    for(i=0; i<num; i++) {
        if(pkt->svlan == vlan_cfg[i].c_vlan.vid) {
            APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, push svlan 0x%x\n", 
                                 __func__, vlan_cfg[i].s_vlan.vid);
            push_vlan_ingress(vlan_cfg[i].s_vlan.vid,vlan_cfg[i].s_vlan.pri, pkt);
            return APP_VLAN_RULE_PUSH_FORWARD;
        }
    }
    
    return CS_E_OK;

}

/*
*   PROTOTYPE    cs_status vlan_ingress_tagged_mode(cs_pkt_t *pkt, cs_sdl_vlan_tag_t *dft_vlan)
*   INPUT            pkt
*                       dft_vlan
*   OUTPUT         pkt.
*   Description     Apply tagged rule on packet.
*                       1) If default vlan id is zero, drop all packets.
*                       2) If packet has vlan, drop it.
*                       3) If packet has no vlan, push default vlan and default priority.
*   Pre-condition  
*   Post-condition
*   Note             
*   Return code    CS_E_ERROR  any error occur.
*                       APP_VLAN_RULE_PUSH_FORWARD 
*/
cs_status vlan_ingress_tagged_mode(cs_pkt_t *pkt, cs_sdl_vlan_tag_t *dft_vlan)
{
    /*
    * Under tagged mode, when default vlan id is zero,
    * no traffic can come into.
    */
    if(dft_vlan->vid == 0) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_MIN, "%s drop,default vlan id is zero\n", __func__);
        return CS_E_ERROR;
    }
    
    if(pkt->tag_num > 0) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_MIN, "%s drop, vlan 0x%x, tag number %d\n", 
                                           __func__, pkt->svlan, pkt->tag_num);
        return CS_E_ERROR;
    }
    
    /* push default vlan id with default tpid*/
    push_vlan_ingress(dft_vlan->vid, dft_vlan->pri, pkt);
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, push default vlan 0x%x\n", __func__, pkt->svlan);
    
    return APP_VLAN_RULE_PUSH_FORWARD;

}

/*
*   PROTOTYPE    cs_status vlan_ingress_translate_mode(
                                            cs_pkt_t *pkt,
                                            cs_sdl_vlan_tag_t *dft_vlan,
                                            cs_sdl_vlan_cfg_t *vlan_cfg,
                                            cs_uint16 num)
*   INPUT            
*                       
*   OUTPUT         
*   Description     Apply tagged rule on packet.
*                       1) If packet has no vlan and default vlan is zero, drop it.
*                       2) If packet has no vlan, push default vlan and default priority.
*                       3) If packet has vlan and this vlan is in the vlan list, translate it to svlan.
*   Pre-condition  
*   Post-condition
*   Note             If the number of vlan in list is zero, this port has the same behavior as tagged mode.
*   Return code    CS_E_ERROR  any error occur.
*                       APP_VLAN_RULE_PUSH_FORWARD 
*                       APP_VLAN_RULE_SWAP_FORWARD
*/
cs_status vlan_ingress_translate_mode(
                                            cs_pkt_t *pkt,
                                            cs_sdl_vlan_tag_t *dft_vlan,
                                            cs_sdl_vlan_cfg_t *vlan_cfg,
                                            cs_uint16 num)
{
    cs_uint16 i = 0;
    cs_uint32 len = pkt->len;
    cs_uint8 *buf = pkt->data + pkt->offset;

    if(pkt->tag_num == 0) {
        if(dft_vlan->vid == 0) {
            /* when default vlan id is zero, drop all the untagged packets. */
            APP_VLAN_LOG(IROS_LOG_LEVEL_MIN, "%s drop,default vlan id is zero\n", __func__);
            return CS_E_ERROR;
        }
        push_vlan_ingress(dft_vlan->vid, dft_vlan->pri, pkt);
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, push default vlan 0x%x\n", __func__, pkt->svlan);
        return APP_VLAN_RULE_PUSH_FORWARD;
    }
    
    for(i=0; i<num; i++) {
        if(pkt->svlan == vlan_cfg[i].c_vlan.vid) {
            /* swap cvlan to svlan */
            APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, swap cvlan 0x%x to svlan 0x%x\n", 
                                 __func__, pkt->svlan,vlan_cfg[i].s_vlan.vid);
            swap_vlan(pkt->svlan, vlan_cfg[i].s_vlan.vid, buf, buf, len);
            pkt->svlan = vlan_cfg[i].s_vlan.vid;
            return APP_VLAN_RULE_SWAP_FORWARD;
        }
    }
        
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s drop, vlan 0x%x is not in the list\n", __func__, pkt->svlan);
    
    return CS_E_ERROR;
    
}

/*   PROTOTYPE    cs_status vlan_ingress_aggregated_mode(
                                            cs_pkt_t *pkt,
                                            cs_sdl_vlan_tag_t *dft_vlan,
                                            cs_sdl_vlan_cfg_t *vlan_cfg,
                                            cs_uint16 num)
*   INPUT            
*                       
*   OUTPUT         
*   Description      it has the same behavior as translation mode .
*                       1) If packet has no vlan and default vlan is zero, drop it.
*                       2) If packet has no vlan, push default vlan and default priority.
*                       3) If packet has vlan and this vlan is in the vlan list, translate it to svlan.
*   Pre-condition  
*   Post-condition
*   Note             If the number of vlan in list is zero, this port has the same behavior as tagged mode.
*   Return code    CS_E_ERROR  any error occur.
*                       APP_VLAN_RULE_PUSH_FORWARD 
*                       APP_VLAN_RULE_SWAP_FORWARD
*/
cs_status vlan_ingress_aggregated_mode(
                                            cs_pkt_t *pkt,
                                            cs_sdl_vlan_tag_t *dft_vlan,
                                            cs_sdl_vlan_cfg_t *vlan_cfg,
                                            cs_uint16 num)
{
    return vlan_ingress_translate_mode(pkt, dft_vlan, vlan_cfg, num);
}

/*   PROTOTYPE    cs_status vlan_ingress_trunk_mode(
                                            cs_pkt_t *pkt,
                                            cs_sdl_vlan_tag_t *dft_vlan,
                                            cs_sdl_vlan_cfg_t *vlan_cfg,
                                            cs_uint16 num)
*   INPUT            
*                       
*   OUTPUT         
*   Description      Apply trunk rule on packet..
*                       1) If packet has no vlan and default vlan is zero, drop it.
*                       2) If packet has no vlan, push default vlan and default priority.
*                       3) If packet has vlan and this vlan is in the vlan list, no action.
*   Pre-condition  
*   Post-condition
*   Note             
*   Return code    CS_E_ERROR  any error occur.
*                       CS_E_OK 
*                       APP_VLAN_RULE_PUSH_FORWARD
*/
cs_status vlan_ingress_trunk_mode(
                                            cs_pkt_t *pkt,
                                            cs_sdl_vlan_tag_t *dft_vlan,
                                            cs_sdl_vlan_cfg_t *vlan_cfg,
                                            cs_uint16 num)
{
    cs_uint16 i = 0;
    
    if(pkt->tag_num == 0) {
        if(dft_vlan->vid == 0) {
            /* when default vlan id is zero, drop all the untagged packets. */
            APP_VLAN_LOG(IROS_LOG_LEVEL_MIN, "%s drop,default vlan id is zero\n", __func__);
            return CS_E_ERROR;
        }
        push_vlan_ingress(dft_vlan->vid, dft_vlan->pri, pkt);
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, push default vlan 0x%x\n", __func__, pkt->svlan);
        return APP_VLAN_RULE_PUSH_FORWARD;
    }
    
    for(i=0; i<num; i++) {
        if(pkt->svlan == vlan_cfg[i].c_vlan.vid) {
            APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s , vlan 0x%x is in the list\n", __func__, pkt->svlan);
            return CS_E_OK;
        }
    }
        
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s drop, vlan 0x%x is not in the list\n", __func__, pkt->svlan);
    
    return CS_E_ERROR;
    
}
/*
cs_status epon_request_onu_ext_qinq_get
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_OUT cs_sdl_ext_qinq_cfg_t	    *qinq_cfg_table,		
    CS_OUT cs_int32	    				*table_len
)
*/
//YOTC
//match : 0; error : -1; should be contine : 1
#define IP_PKT_TYPE_CHK(pkt) \
    do{\
        if(pkt->eth_type != 0x0800) {\
            APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s not ip packet 0x%x\n", __func__, pkt->eth_type);\
            return 1;\
        }\
    }while(0)
#define MATTH_ERROR -1
#define MATCH_OK 0
#define MATCH_CONTINUE 1
#define IP_TOS_GET(dscp) ((dscp) >> 2) 

void push_vlan_tag_pkt(app_vlan_header_t *tag, cs_pkt_t *pkt)
{
    cs_uint8 * buf = pkt->data+pkt->offset;
    
    memmove(buf-sizeof(app_vlan_header_t),buf,ETH_L2_HEADER_LEN);
    memmove(buf+ETH_L2_HEADER_LEN-sizeof(app_vlan_header_t),tag,sizeof(app_vlan_header_t));

    pkt->len += sizeof(app_vlan_header_t);
    pkt->offset -= sizeof(app_vlan_header_t);
    if(pkt->tag_num) {
        pkt->ctpid = pkt->stpid;
        pkt->cvlan = pkt->svlan;
    }
    pkt->svlan = ntohs(tag->vlan);
    pkt->stpid = ntohs(tag->tpid);
    pkt->tag_num++;
}
void cs_pkt_t_dump(const cs_pkt_t *pkt) 
{
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"pkt dump: \n");
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"port %d\n", pkt->port);
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"tag number %d\n", pkt->tag_num);
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"cvlan %d\n", pkt->cvlan);
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"ctpid 0x%x\n", pkt->ctpid);
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"svlan %d\n", pkt->svlan);
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"stpid 0x%x\n", pkt->stpid);
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"eth type 0x%x\n", pkt->eth_type);
}
void cs_sdl_ext_qinq_cfg_t_dump(cs_sdl_ext_qinq_cfg_t *entry)
{
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"QinQ rule entry :\n");
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"match type %d\n", entry->match_type);
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"match value %d\n", entry->match_value);
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"attach vid %d\n", entry->attach_vid);
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"tpid_type %d\n", entry->tpid_type);
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"priority %d\n", entry->pri);
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"\n");
}
cs_int32 vlan_ingress_qinq_mode_(cs_sdl_ext_qinq_cfg_t *entry, cs_pkt_t *pkt)
{
    cs_boolean match = FALSE;
    ip_hdr_t *iphdr = NULL;
    const cs_uint16 tpid[] = {0x8100, 0x9100, 0x88A8};
    app_vlan_header_t vlanHeader, *vhdr = NULL;;

    if(!entry) {
        return MATTH_ERROR;
    }
    cs_sdl_ext_qinq_cfg_t_dump(entry);
    switch(entry->match_type) {
        case SDL_EXT_QINQ_MATCH_ETHER_TYPE:
            if(pkt->eth_type == entry->match_value) {
                match = TRUE;
            }
            break;
        case SDL_EXT_QINQ_MATCH_CVLAN_TYPE: {
                if(!pkt->tag_num) {
                    goto MATCH_CONTINUE_EXIT;
                }
                vhdr = (app_vlan_header_t *)(pkt->data + pkt->offset+12);
                if((ntohs(vhdr->vlan)&0xFFF) == entry->match_value) {
                    match = TRUE;
                }
            }
            break;
        case SDL_EXT_QINQ_MATCH_IP_PRO_TYPE:
            IP_PKT_TYPE_CHK(pkt);
            iphdr = (ip_hdr_t *)(pkt->data+pkt->offset+14+pkt->tag_num*4);
            if(iphdr->protocol == entry->match_value) {
                match = TRUE;
            }
            break;
        case SDL_EXT_QINQ_MATCH_IP_TOS_TYPE:
            IP_PKT_TYPE_CHK(pkt);
            iphdr = (ip_hdr_t *)(pkt->data+pkt->offset+14+pkt->tag_num*4);
            if(IP_TOS_GET(iphdr->tos) == entry->match_value) {
                match = TRUE;
            }
            break;
        case SDL_EXT_QINQ_MATCH_SRC_IP_TYPE:
            IP_PKT_TYPE_CHK(pkt);
            iphdr = (ip_hdr_t *)(pkt->data+pkt->offset+14+pkt->tag_num*4);
            if(ntohl(iphdr->saddr) == entry->match_value) {
                match = TRUE;
            }
            break;
        case SDL_EXT_QINQ_MATCH_DST_IP_TYPE:
            IP_PKT_TYPE_CHK(pkt);
            if(ntohl(iphdr->daddr) == entry->match_value) {
                match = TRUE;
            }
            break;
        /*below is option match type , now oam not support it*/
        case SDL_EXT_QINQ_MATCH_SRC_MAC_TYPE: {
                cs_uint8 *smac = (cs_uint8 * )(pkt->data+pkt->offset+6);
                cs_uint8 *key = (cs_uint8 * )&entry->match_value + 2;
                if(memcmp(smac, key, 6) == 0){
                    match = TRUE;
                }
            }
            break;
        case SDL_EXT_QINQ_MATCH_DST_MAC_TYPE:{
                cs_uint8 *dmac = (cs_uint8 * )(pkt->data+pkt->offset);
                cs_uint8 *key = (cs_uint8 * )&entry->match_value + 2;
                if(memcmp(dmac, key, 6) == 0){
                    match = TRUE;
                }
            }
            
            break;
        case SDL_EXT_QINQ_MATCH_COS_TYPE: {
                if(!pkt->tag_num) {
                    goto MATCH_CONTINUE_EXIT;
                }
                vhdr = (app_vlan_header_t *)(pkt->data+pkt->offset+12);
                if((ntohs(vhdr->vlan))>>PRI_BIT_OFFSET == entry->match_value) {
                    match = TRUE;
                }
            }
            break;
        case SDL_EXT_QINQ_MATCH_L4_SRC_PORT_TYPE: {
                IP_PKT_TYPE_CHK(pkt);
                iphdr = (ip_hdr_t *)(pkt->data+pkt->offset+14+pkt->tag_num*4);
                if(iphdr->protocol == IP_UDP_PROTOCOL_ID || iphdr->protocol == IP_TCP_PROTOCOL_ID){
                    udp_hdr_t *udp = (udp_hdr_t *)((cs_uint8 *)iphdr + iphdr->ihl*4);
                    if(udp->sport == entry->match_value) {
                        match = TRUE;
                    }
                }
            }
            break;
        case SDL_EXT_QINQ_MATCH_L4_DST_PORT_TYPE:{
                IP_PKT_TYPE_CHK(pkt);
                iphdr = (ip_hdr_t *)(pkt->data+pkt->offset+14+pkt->tag_num*4);
                if(iphdr->protocol == IP_UDP_PROTOCOL_ID || iphdr->protocol == IP_TCP_PROTOCOL_ID) {
                    udp_hdr_t *udp = (udp_hdr_t *)((cs_uint8 *)iphdr + iphdr->ihl*4);
                    if(udp->dport == entry->match_value) {
                        match = TRUE;
                    }
                }
            }
            break;
        case SDL_EXT_QINQ_MATCH_END_TYPE:
            //fall through
        default:
            goto MATCH_CONTINUE_EXIT;
    }
    
    if(!match) {
        goto MATCH_CONTINUE_EXIT;
    }
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"QinQ rule matched\n");
    memset(&vlanHeader, 0, sizeof(app_vlan_header_t));
    vlanHeader.tpid = htons(tpid[entry->tpid_type%3]);
    vlanHeader.vlan = htons(entry->attach_vid & VLAN_MASK_BIT);
    vlanHeader.vlan |= htons(entry->pri << PRI_BIT_OFFSET);
    push_vlan_tag_pkt(&vlanHeader, pkt);

    return MATCH_OK;
    
MATCH_CONTINUE_EXIT:
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"QinQ rule not matched\n");
    return MATCH_CONTINUE;
    
}
//#define YOTC_QINQ_DEBUG 1
#ifdef YOTC_QINQ_DEBUG //debug only
void cs_sdl_ext_qinq_cfg_t_set(cs_sdl_ext_qinq_cfg_t *cfg, cs_uint32 *table_size)
{
    //
    cfg->match_type = SDL_EXT_QINQ_MATCH_IP_TOS_TYPE;
    cfg->match_value = 10;
    cfg->attach_vid = 200;
    cfg->tpid_type = SDL_EXT_QINQ_TPID_9100_TYPE;
    cfg->pri = 3;
    //
    cfg++;
    cfg->match_type = SDL_EXT_QINQ_MATCH_IP_PRO_TYPE;
    cfg->match_value = 17;
    cfg->attach_vid = 400;
    cfg->tpid_type = SDL_EXT_QINQ_TPID_9100_TYPE;
    cfg->pri = 3;
    
    *table_size = 2;
}
#endif
cs_status vlan_ingress_qinq_mode(cs_pkt_t *pkt)
{
#ifdef HAVE_MPORTS
    cs_status ret = CS_E_OK;
    cs_int32 table_size = 0, i = 0;
    cs_sdl_ext_qinq_cfg_t cfg[8];
    cs_callback_context_t context;
    
    ret = epon_request_onu_ext_qinq_get(context, 0, 0, cfg, &table_size);
    if(ret) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_MAJ, "%s get qinq vlan configuration error, %d\n", __func__, ret);
        return CS_E_OK;
    }
#ifdef YOTC_QINQ_DEBUG //debug only
    cs_sdl_ext_qinq_cfg_t_set(cfg, &table_size);
#endif
    if(!table_size) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_INF, "%s get qinq vlan configuration NULL\n", __func__);
        return CS_E_OK;
    }
    //cs_pkt_t_dump(pkt);
    for(i=table_size-1; i>=0; i--) {
        if(vlan_ingress_qinq_mode_(&cfg[i], pkt) == 0) {
            break;
        }
    }
    //cs_pkt_t_dump(pkt);
#endif

    return CS_E_OK;
}
cs_status vlan_ingress_rule_chk(cs_pkt_t *pkt)
{
    cs_status ret = CS_E_OK;
    cs_uint16 vlan_num = 0;
    cs_sdl_vlan_mode_t mode;
    cs_sdl_vlan_tag_t dft_vlan;
    cs_sdl_vlan_cfg_t vlan_cfg[SDL_VLAN_MAX_ENTRY];
    cs_callback_context_t context;
    
    if(pkt->port == CS_PON_PORT_ID) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_MIN, "%s pon port\n",__func__);
        return CS_E_OK;
    }

    ret = epon_request_onu_vlan_get(context,ONU_DEVICEID_FOR_API,ONU_LLIDPORT_FOR_API,
                                        pkt->port, &dft_vlan, &mode, vlan_cfg, &vlan_num);
    if(ret != CS_E_OK) {
        return CS_E_ERROR;
    }
    
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, port %d, vlan mode %d\n",__func__,pkt->port,mode);
    
    switch(mode) {
        case SDL_VLAN_MODE_TRANSPARENT:
            break;
        case SDL_VLAN_MODE_TAG:
            ret = vlan_ingress_tagged_mode(pkt, &dft_vlan);
            break;
        case SDL_VLAN_MODE_TRANSLATION:
            ret = vlan_ingress_translate_mode(pkt, &dft_vlan, vlan_cfg, vlan_num);
            break;
        case SDL_VLAN_MODE_AGGREGATION:
            ret = vlan_ingress_aggregated_mode(pkt, &dft_vlan, vlan_cfg, vlan_num);
            break;
        case SDL_VLAN_MODE_TRUNK:
            ret = vlan_ingress_trunk_mode(pkt, &dft_vlan, vlan_cfg, vlan_num);
            break;
        case SDL_VLAN_MODE_STACKING:
            ret = vlan_ingress_stacking_mode(pkt, &dft_vlan);
            break;
        default :
            APP_VLAN_LOG(IROS_LOG_LEVEL_MIN, "%s, invalid vlan mode %d\n", __func__, mode);
            return CS_E_ERROR;
    }

    vlan_ingress_qinq_mode(pkt);
    return ret;     
    
}

cs_status mc_vlan_ingress_rule_chk(cs_pkt_t *pkt)
{
    cs_status ret = CS_E_OK;
    cs_uint16 vlan_num = 0;
	cs_int16 mc_vlan[__MC_VLAN_PER_PORT_MAX] = {0};
	cs_uint8 index = 0;
    cs_callback_context_t context;
    
    if(pkt->port == CS_PON_PORT_ID) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_MIN, "%s pon port\n",__func__);
        return CS_E_OK;
    }

	ret = epon_request_onu_mc_vlan_get(context,ONU_DEVICEID_FOR_API,ONU_LLIDPORT_FOR_API,
                                        pkt->port, mc_vlan, &vlan_num);

	if(ret != CS_E_OK) {
        return CS_E_ERROR;
    }

	if(0 == pkt->tag_num && 1 == vlan_num)
	{
		cs_uint32 m_len = pkt->len;
		char *buf = pkt->data + pkt->offset;
		push_vlan(mc_vlan[0],buf,buf,&m_len);
		pkt->len = m_len;
		pkt->tag_num++;
		
		APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, push default vlan 0x%x\n", __func__, pkt->svlan);
        vlan_ingress_qinq_mode(pkt);
		return APP_VLAN_RULE_PUSH_FORWARD;
	}
	
	for(index = 0;index < vlan_num;index++)
	{
		if(pkt->svlan == mc_vlan[index]) 
		{
			APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s , vlan 0x%x is in the mc vlan list\n", __func__, pkt->svlan);
	        vlan_ingress_qinq_mode(pkt);
			return CS_E_OK;
        }
	}

	APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s drop, vlan 0x%x is not in the mc vlan list\n", __func__, pkt->svlan);
    
    return CS_E_ERROR;    
}


/*
*   PROTOTYPE    cs_int32 vlan_egress_tagged_mode(
                                                        cs_port_id_t dport,
                                                        cs_uint8 *in,
                                                        cs_uint8 *out,
                                                        cs_uint32 *len,
                                                        cs_sdl_vlan_tag_t *dft_vlan)
*   INPUT            
*                       
*   OUTPUT         
*   Description     Apply tagged rule(egress) on packet.
*                       1) If packet has no vlan, drop it.
*                       2) If packet has default vlan, pop default vlan.
*                       3) Otherwise, drop it.
*   Pre-condition  
*   Post-condition
*   Note             
*   Return code    APP_VLAN_RULE_DROP
*                       APP_VLAN_RULE_POP_FORWARD 
*/
cs_int32 vlan_egress_tagged_mode(
                                                        cs_port_id_t dport,
                                                        cs_uint8 *in,
                                                        cs_uint8 *out,
                                                        cs_uint32 *len,
                                                        cs_sdl_vlan_tag_t *dft_vlan)
{
    app_vlan_double_tag_header_t vlanHdr;
    
    memset(&vlanHdr,0,sizeof(app_vlan_double_tag_header_t));
    vlan_header_parse(in,&vlanHdr);
    
    /* if has no vlan or vlan id is zero, drop and warning here */
    if(vlanHdr.svlan == 0/* && vlanHdr.stpid == 0*/) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_MIN, "%s drop, no vlan tag\n",__func__);
        return APP_VLAN_RULE_DROP;
    }
    
    if(vlanHdr.svlan == dft_vlan->vid) {
        pop_vlan(in, out, len);
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, pop vlan tag 0x%x\n",__func__,vlanHdr.svlan);
        return APP_VLAN_RULE_POP_FORWARD;
    }

    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s drop, vlan 0x%x is not in the list\n", __func__, vlanHdr.svlan);
    
    return APP_VLAN_RULE_DROP;
    
}

/*
*   PROTOTYPE    cs_int32 vlan_egress_stacking_mode(
                                                        cs_port_id_t dport,
                                                        cs_uint8 *in,
                                                        cs_uint8 *out,
                                                        cs_uint32 *len,
                                                        cs_sdl_vlan_tag_t *dft_vlan)
*   INPUT            
*                       
*   OUTPUT         
*   Description     It has the same behavior as tagged mode
*                       1) If packet has no vlan, drop it.
*                       2) If packet has default vlan, pop default vlan.
*                       3) Otherwise, drop it.
*   Pre-condition  
*   Post-condition
*   Note             
*   Return code    APP_VLAN_RULE_DROP
*                       APP_VLAN_RULE_POP_FORWARD 
*/
cs_int32 vlan_egress_stacking_mode(
                                                        cs_port_id_t dport,
                                                        cs_uint8 *in,
                                                        cs_uint8 *out,
                                                        cs_uint32 *len,
                                                        cs_sdl_vlan_tag_t *dft_vlan)
{
    return vlan_egress_tagged_mode(dport, in, out, len ,dft_vlan);
}

/*
*   PROTOTYPE    cs_int32 vlan_egress_translate_mode(
                                                        cs_port_id_t dport,
                                                        cs_uint8 *in,
                                                        cs_uint8 *out,
                                                        cs_uint32 *len,
                                                        cs_sdl_vlan_tag_t *dft_vlan,
                                                        cs_sdl_vlan_cfg_t *vlan_cfg,
                                                        cs_uint16 num)
*   INPUT            
*                       
*   OUTPUT         
*   Description     Apply translated rule(egress) on packet.
*                       1) If packet has no vlan, drop it.
*                       2) If packet has default vlan, pop default vlan.
*                       3) If packet has vlan which is in the vlan list, translate it to cvlan.
*   Pre-condition  
*   Post-condition
*   Note             When the number of vlan configured is zero, it has the same behavior as tagged mode. 
*   Return code    APP_VLAN_RULE_DROP
*                       APP_VLAN_RULE_POP_FORWARD 
*                       APP_VLAN_RULE_SWAP_FORWARD
*/
cs_int32 vlan_egress_translate_mode(
                                                        cs_port_id_t dport,
                                                        cs_uint8 *in,
                                                        cs_uint8 *out,
                                                        cs_uint32 *len,
                                                        cs_sdl_vlan_tag_t *dft_vlan,
                                                        cs_sdl_vlan_cfg_t *vlan_cfg,
                                                        cs_uint16 num)
{
    cs_uint16 i = 0;
    app_vlan_double_tag_header_t vlanHdr;
    
    memset(&vlanHdr,0,sizeof(app_vlan_double_tag_header_t));
    vlan_header_parse(in,&vlanHdr); 
    
    /* if no vlan, drop it */
    if(vlanHdr.svlan == 0 /*&& vlanHdr.stpid == 0*/) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, no vlan tag\n",__func__);
        return APP_VLAN_RULE_DROP;
    }
    
    /* if having default vlan, pop and forward it */
    if(dft_vlan->vid == vlanHdr.svlan) {
        pop_vlan(in, out, len);
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, pop vlan tag 0x%x\n",__func__,vlanHdr.svlan);
        return APP_VLAN_RULE_POP_FORWARD;
    }
    
    for(i=0; i<num; i++) {
        if(vlanHdr.svlan == vlan_cfg[i].s_vlan.vid) {
            /* if vlan is within the list, swap and forward */    
            APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, swap svlan 0x%x to cvlan 0x%x\n", 
                                                          __func__,vlanHdr.svlan, vlan_cfg[i].c_vlan.vid);
            swap_vlan(vlanHdr.svlan, vlan_cfg[i].c_vlan.vid, in, out, *len);
            return APP_VLAN_RULE_SWAP_FORWARD;
        }
    }
    
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s drop, vlan 0x%x is not in the list\n", __func__, vlanHdr.svlan);
    
    return APP_VLAN_RULE_DROP;
    
}

/*
*   PROTOTYPE    cs_int32 vlan_egress_aggregated_mode(
                                                        cs_port_id_t dport,
                                                        cs_uint8 *in,
                                                        cs_uint8 *out,
                                                        cs_uint32 *len,
                                                        cs_sdl_vlan_tag_t *dft_vlan,
                                                        cs_sdl_vlan_cfg_t *vlan_cfg,
                                                        cs_uint16 num)
*   INPUT            
*                       
*   OUTPUT         
*   Description     Apply aggregated rule(egress) on packet.
*                       1) If packet has no vlan, drop it.
*                       2) If packet has default vlan, pop default vlan.
*                       3) If packet has vlan which is not aggregated vlan, drop it.
*                       4) If packet has vlan and it is aggregated vlan, retrive cvlan from FDB table by 
*                           destination mac, if find the entry and cvlan is in the list, swap them.
*   Pre-condition  
*   Post-condition
*   Note             
*   Return code    APP_VLAN_RULE_DROP
*                       APP_VLAN_RULE_POP_FORWARD 
*                       APP_VLAN_RULE_SWAP_FORWARD
*/
cs_int32 vlan_egress_aggregated_mode(
                                                        cs_port_id_t dport,
                                                        cs_uint8 *in,
                                                        cs_uint8 *out,
                                                        cs_uint32 *len,
                                                        cs_sdl_vlan_tag_t *dft_vlan,
                                                        cs_sdl_vlan_cfg_t *vlan_cfg,
                                                        cs_uint16 num)
{
    cs_uint16 i = 0;
    cs_int32 ret = 0;
    cs_uint16 vlan;
    cs_port_id_t port;
    app_vlan_double_tag_header_t vlanHdr;
    
    memset(&vlanHdr,0,sizeof(app_vlan_double_tag_header_t));
    vlan_header_parse(in,&vlanHdr); 

    /* if no vlan, drop it */
    if(vlanHdr.svlan == 0 /*&& vlanHdr.stpid == 0*/) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, no vlan tag\n",__func__);
        return APP_VLAN_RULE_DROP;
    }

    /* If it has default vlan, pop */
    if(dft_vlan->vid == vlanHdr.svlan) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, pop vlan %d\n",__func__, vlanHdr.svlan);
        pop_vlan(in, out, len);
        return APP_VLAN_RULE_POP_FORWARD;
    }
    
    /* if vlan in the packet is not aggregated vlan, drop it. */
    for(i=0; i<num; i++) {
        /*if(vlan_cfg[i].c_vlan.vid == 0) */{
            APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s,aggregated vlan %d\n",
                __func__, vlan_cfg[i].s_vlan.vid);
            break;
        }
    }
    if(i >= num) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s,no aggregated vlan, num %d\n",
                __func__, num);
        return APP_VLAN_RULE_DROP;
    }
    
    if(vlanHdr.svlan != vlan_cfg[i].s_vlan.vid) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, svlan vlan %d,aggregated vlan %d\n",
              __func__, vlanHdr.svlan, vlan_cfg[i].s_vlan.vid);
        return APP_VLAN_RULE_DROP;
    }
    
    /* find FDB entry by dmac */
    ret = vlan_mac2vlan_get(in, &vlan, &port);
    if(ret != CS_E_OK) {
        return CS_E_ERROR;
    }

    /* if the vlan is in the vlan list, swap them. */
    for(i=0; i<num; i++) {
        if(vlan_cfg[i].c_vlan.vid == vlan) {
            APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, swap vlan %d(%d)\n",
                __func__, vlan, vlanHdr.svlan);
            swap_vlan(vlanHdr.svlan, vlan, in, out, *len);
            return APP_VLAN_RULE_SWAP_FORWARD;
        }
    }
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s drop, no cvlan for aggregated vlan %d\n",
          __func__, vlanHdr.svlan);
    
    return APP_VLAN_RULE_DROP;
    
}

/*
*   PROTOTYPE    cs_int32 vlan_egress_trunk_mode(
                                                        cs_port_id_t dport,
                                                        cs_uint8 *in,
                                                        cs_uint8 *out,
                                                        cs_uint32 *len,
                                                        cs_sdl_vlan_tag_t *dft_vlan,
                                                        cs_sdl_vlan_cfg_t *vlan_cfg,
                                                        cs_uint16 num)
*   INPUT            
*                       
*   OUTPUT         
*   Description     Apply trunk rule(egress) on packet.
*                       1) If packet has no vlan, drop it.
*                       2) If packet has default vlan, pop default vlan.
*                       3) If packet has vlan and it is in the list, no action. 
*   Pre-condition  
*   Post-condition
*   Note             
*   Return code    APP_VLAN_RULE_DROP
*                       APP_VLAN_RULE_POP_FORWARD 
*                       APP_VLAN_RULE_NOOP_FORWARD
*/
cs_int32 vlan_egress_trunk_mode(
                                                        cs_port_id_t dport,
                                                        cs_uint8 *in,
                                                        cs_uint8 *out,
                                                        cs_uint32 *len,
                                                        cs_sdl_vlan_tag_t *dft_vlan,
                                                        cs_sdl_vlan_cfg_t *vlan_cfg,
                                                        cs_uint16 num)
{
    cs_uint16 i = 0;
    app_vlan_double_tag_header_t vlanHdr;
    
    memset(&vlanHdr,0,sizeof(app_vlan_double_tag_header_t));
    vlan_header_parse(in,&vlanHdr); 
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, vlan %d, default vlan %d, trunk vlan count %d\n",
        __func__, vlanHdr.svlan, dft_vlan->vid, num);
    
    /* if no vlan, drop it */
    if(vlanHdr.svlan == 0/* && vlanHdr.stpid == 0*/) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, no vlan tag\n",__func__);
        return APP_VLAN_RULE_DROP;
    }
    
    /* if has default vlan, pop and forward */    
    if(dft_vlan->vid == vlanHdr.svlan) {
        pop_vlan(in, out, len);
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, pop vlan tag 0x%x\n",__func__,vlanHdr.svlan);
        return APP_VLAN_RULE_POP_FORWARD;
    }
    
    /* if vlan is within the list, forward */    
    for(i=0; i< num; i++) {
        if(vlanHdr.svlan == vlan_cfg[i].c_vlan.vid) {
            if(in != out) {
                memcpy(out, in, *len);
            }
            APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, vlan 0x%x match\n",__func__,vlanHdr.svlan);
            return APP_VLAN_RULE_NOOP_FORWARD;
        }
    }
    
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s drop, vlan 0x%x is not in the list\n", __func__, vlanHdr.svlan);
    
    return APP_VLAN_RULE_DROP;
    
}

cs_int32 vlan_egress_qinq_mode_(cs_sdl_ext_qinq_cfg_t *entry, cs_pkt_t *pkt, cs_uint8 *in)
{
    cs_boolean match = FALSE;
    ip_hdr_t *iphdr = NULL;
    cs_uint8 *pkt_data = in;
    app_vlan_header_t *vhdr = NULL;

    if(!entry) {
        return MATTH_ERROR;
    }
    cs_sdl_ext_qinq_cfg_t_dump(entry);
    //
    if(!pkt->tag_num) {
        goto MATCH_CONTINUE_EXIT;
    }
    //
    vhdr = (app_vlan_header_t *)(pkt_data + 12);
    if((ntohs(vhdr->vlan) & 0xFFF) != entry->attach_vid) {
        goto MATCH_CONTINUE_EXIT;
    }
    //
    switch(entry->match_type) {
        case SDL_EXT_QINQ_MATCH_ETHER_TYPE:
            if(pkt->eth_type == entry->match_value) {
                match = TRUE;
            }
            break;
        case SDL_EXT_QINQ_MATCH_CVLAN_TYPE:
            //If match cvlan, there are at least 2 tags before any process.
            if(pkt->tag_num < 2) {
                    goto MATCH_CONTINUE_EXIT;
            }
            //match cvlan
            if((pkt->cvlan&0xFFF) == entry->match_value) {
                match = TRUE;
            }
            break;
        case SDL_EXT_QINQ_MATCH_IP_PRO_TYPE:
            IP_PKT_TYPE_CHK(pkt);
            iphdr = (ip_hdr_t *)(pkt_data + 14 + pkt->tag_num*4);
            if(iphdr->protocol == entry->match_value) {
                match = TRUE;
            }
            break;
        case SDL_EXT_QINQ_MATCH_IP_TOS_TYPE:
            IP_PKT_TYPE_CHK(pkt);
            iphdr = (ip_hdr_t *)(pkt_data+14+pkt->tag_num*4);
            if(IP_TOS_GET(iphdr->tos) == entry->match_value) {
                match = TRUE;
            }
            break;
        case SDL_EXT_QINQ_MATCH_SRC_IP_TYPE:
            IP_PKT_TYPE_CHK(pkt);
            iphdr = (ip_hdr_t *)(pkt_data + 14 + pkt->tag_num*4);
            if(ntohl(iphdr->daddr) == entry->match_value) {
                //match destination ip address
                match = TRUE;
            }
            break;
        case SDL_EXT_QINQ_MATCH_DST_IP_TYPE:
            IP_PKT_TYPE_CHK(pkt);
            if(ntohl(iphdr->saddr) == entry->match_value) {
                //match source ip address
                match = TRUE;
            }
            break;
        /*below is option match type , now oam not support it*/
        case SDL_EXT_QINQ_MATCH_SRC_MAC_TYPE: {
                cs_uint8 *dmac = pkt_data;
                cs_uint8 *key = (cs_uint8 * )&entry->match_value + 2;
                //match destination mac address
                if(memcmp(dmac, key, 6) == 0){
                    match = TRUE;
                }
            }
            break;
        case SDL_EXT_QINQ_MATCH_DST_MAC_TYPE:{
                cs_uint8 *smac = (cs_uint8 * )(pkt_data + 6);
                cs_uint8 *key = (cs_uint8 * )&entry->match_value + 2;
                //match source mac address
                if(memcmp(smac, key, 6) == 0){
                    match = TRUE;
                }
            }
            
            break;
        case SDL_EXT_QINQ_MATCH_COS_TYPE: {
                if(pkt->tag_num < 2) {
                    goto MATCH_CONTINUE_EXIT;
                }
                //match cos within cvlan
                if(((pkt->cvlan&PRI_MASK_BIT) >> PRI_BIT_OFFSET) == entry->match_value) {
                    match = TRUE;
                }
            }
            break;
        case SDL_EXT_QINQ_MATCH_L4_SRC_PORT_TYPE: {
                IP_PKT_TYPE_CHK(pkt);
                iphdr = (ip_hdr_t *)(pkt_data+14+pkt->tag_num*4);
                if(iphdr->protocol == IP_UDP_PROTOCOL_ID || iphdr->protocol == IP_TCP_PROTOCOL_ID) {
                    udp_hdr_t *udp = (udp_hdr_t *)((cs_uint8 *)iphdr + iphdr->ihl*4);
                    if(udp->dport == entry->match_value) {
                        //match destination l4 port
                        match = TRUE;
                    }
                }
            }
            break;
        case SDL_EXT_QINQ_MATCH_L4_DST_PORT_TYPE:{
                IP_PKT_TYPE_CHK(pkt);
                iphdr = (ip_hdr_t *)(pkt_data+14+pkt->tag_num*4);
                if(iphdr->protocol == IP_UDP_PROTOCOL_ID || iphdr->protocol == IP_TCP_PROTOCOL_ID) {
                    udp_hdr_t *udp = (udp_hdr_t *)((cs_uint8 *)iphdr + iphdr->ihl*4);
                    if(udp->sport == entry->match_value) {
                        //match destination l4 port
                        match = TRUE;
                    }
                }
            }
            break;
        case SDL_EXT_QINQ_MATCH_END_TYPE:
            //fall through
        default:
            goto MATCH_CONTINUE_EXIT;
    }
    
    if(!match) {
        goto MATCH_CONTINUE_EXIT;
    }
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"QinQ rule matched\n");
    memmove(pkt_data+12, pkt_data+16,  pkt->len-16);
    pkt->len -= 4;
    return MATCH_OK;

MATCH_CONTINUE_EXIT:
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3,"QinQ rule not matched\n");
    return MATCH_CONTINUE;
}


cs_status vlan_egress_qinq_mode(/* INOUT */cs_uint8 *in, /* INOUT */cs_uint32 *len)
{
#ifdef HAVE_MPORTS
    cs_status ret = CS_E_OK;
    cs_int32 table_size = 0, i = 0;
    cs_sdl_ext_qinq_cfg_t cfg[8];
    cs_pkt_t pkt;
    cs_callback_context_t context;
    
    ret = epon_request_onu_ext_qinq_get(context, 0, 0, cfg, &table_size);
    if(ret) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_MAJ, "%s get qinq vlan configuration error, %d\n", __func__, ret);
        return CS_E_OK;
    }
#ifdef YOTC_QINQ_DEBUG //debug only
    cs_sdl_ext_qinq_cfg_t_set(cfg, &table_size);
#endif
    
    if(!table_size) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_INF, "%s get qinq vlan configuration NULL\n", __func__);
        return CS_E_OK;
    }
    memset(&pkt, 0, sizeof(cs_pkt_t));
    vlan_header_parse_2pkt(in, *len, &pkt);
    for(i=table_size-1; i>=0; i--) {
        if(vlan_egress_qinq_mode_(&cfg[i], &pkt, in) == 0) {
            *len = pkt.len;
            break;
        }
    }
#endif
    
    return CS_E_OK;
}

cs_int32 vlan_egress_rule_handle(
                                                            cs_port_id_t sport,
                                                            cs_port_id_t dport,
                                                            cs_uint8 *in,
                                                            cs_uint8 *out,
                                                            cs_uint32 *len)
{
    cs_status ret = CS_E_OK;
    cs_uint16 vlan_num = 0;
    cs_sdl_vlan_mode_t mode;
    cs_sdl_vlan_tag_t dft_vlan;
    cs_sdl_vlan_cfg_t vlan_cfg[SDL_VLAN_MAX_ENTRY];
    cs_callback_context_t context;

    /* vlan rule does not apply to PON port. */
    if(dport == CS_PON_PORT_ID) {
        if(in != out) {
            memcpy(out, in, *len);
        }
        return CS_E_OK;
    }
    vlan_egress_qinq_mode(in, len);
    
    ret = epon_request_onu_vlan_get(context,ONU_DEVICEID_FOR_API,ONU_LLIDPORT_FOR_API,
                                        dport, &dft_vlan, &mode, vlan_cfg ,&vlan_num);
    if(ret != CS_E_OK) {
        return CS_E_ERROR;
    }
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, dport %d, vlan mode %d\n", __func__, dport, mode);
    switch(mode) {
        case SDL_VLAN_MODE_TRANSPARENT:
            if(in != out) {
                memcpy(out, in, *len);
            }
            break;
        case SDL_VLAN_MODE_TAG:
            ret = vlan_egress_tagged_mode(dport, in, out, len, &dft_vlan);
            break;
        case SDL_VLAN_MODE_TRANSLATION:
            ret = vlan_egress_translate_mode(dport, in, out, len, &dft_vlan, vlan_cfg, vlan_num);
            break;
        case SDL_VLAN_MODE_AGGREGATION:
            ret = vlan_egress_aggregated_mode(dport, in, out, len, &dft_vlan, vlan_cfg, vlan_num);
            break;
        case SDL_VLAN_MODE_TRUNK:
            ret = vlan_egress_trunk_mode(dport, in, out, len, &dft_vlan, vlan_cfg, vlan_num);
            break;
        case SDL_VLAN_MODE_STACKING:
            ret = vlan_egress_stacking_mode(dport, in, out, len, &dft_vlan);
            break;
        default :
            APP_VLAN_LOG(IROS_LOG_LEVEL_MAJ, "%s ,port %d,invalid vlan mode\n", __func__,dport,mode);
            return CS_E_ERROR;
    }
    
    return ret;     
    
}

/*
*   PROTOTYPE    cs_status mc_vlan_ingress_snoop_filter(cs_pkt_t *, cs_uint8 , cs_uint8 *)
*   INPUT            
*   OUTPUT         changed: 1 if the packet is modified in the function.
*   Description     for igmp snooping mode, multicast packet has the same rule as unicast packet.
*   Pre-condition  
*   Post-condition
*   Note             
*   Return code    CS_E_ERROR
*                       CS_E_OK 
*/
cs_status mc_vlan_ingress_snoop_filter(cs_pkt_t *pkt, cs_uint8 type, cs_uint8 *changed)
{
    cs_status ret = CS_E_OK;
    *changed = 0;
    
    ret = mc_vlan_ingress_rule_chk(pkt);
    if(ret == CS_E_ERROR) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, drop\n", __func__);
        return ret;
    }
    if(ret > CS_E_OK) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, packet is changed\n", __func__);
        *changed = 1;
    }
    
    return CS_E_OK;
    
}

/*
*   PROTOTYPE    cs_status mc_vlan_ingress_mcctrl_filter(cs_pkt_t *, cs_uint8 , cs_uint8 *)
*   INPUT            
*   OUTPUT         changed: 1 if the packet is modified in the function.
*   Description     for multicast control mode, 
*                       1) if packet has no vlan, push vlan(port index).
*                       2) if packet has vlan, swap vlan to port index.
*   Pre-condition  
*   Post-condition
*   Note             this packet is always modified in the function.
*   Return code    
*                       CS_E_OK 
*/
cs_status mc_vlan_ingress_mcctrl_filter(cs_pkt_t *pkt, cs_uint8 type, cs_uint8 *changed)
{
    /* TODO : how to fill pri and TPID ? */
    if(pkt->tag_num == 0) {
        cs_uint16 new_vlan = pkt->port;
        /* */
        push_vlan_ingress(new_vlan, 0, pkt);
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s , push vlan %d\n", __func__, new_vlan);
    }
    else {
        cs_uint8 *buf = pkt->data + pkt->offset;
        cs_uint16 new_vlan = pkt->port;

        /* TODO : if it has invalid unicast vlan tag, drop packet here */
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, swap vlan %d(%d)\n", __func__, new_vlan, pkt->svlan);
        swap_vlan(pkt->svlan, new_vlan, buf, buf, pkt->len);
        pkt->svlan = new_vlan;
    }
    *changed = 1;
    
    return CS_E_OK;
    
}

/*
*   PROTOTYPE    cs_int32 mc_vlan_egress_keep_strip(
                                                    cs_sdl_mc_vlan_act_t mode,
                                                    cs_port_id_t dport,
                                                    cs_uint32 *len,
                                                    cs_uint8 *buf)
*   INPUT            
*   OUTPUT         
*   Description     1) if packet has no vlan, drop it.
*                       2) if packet has vlan and it is in the list,
*                             a) if it is in KEEP mode, no action
*                             b) if it is in strip mode, pop vlan.
*   Pre-condition  
*   Post-condition
*   Note             
*   Return code    CS_E_ERROR
*                       APP_VLAN_RULE_NOOP_FORWARD
*                       APP_VLAN_RULE_POP_FORWARD 
*                       APP_VLAN_RULE_DROP
*/
cs_int32 mc_vlan_egress_keep_strip(
                                                    cs_sdl_mc_vlan_act_t mode,
                                                    cs_port_id_t dport,
                                                    cs_uint32 *len,
                                                    cs_uint8 *buf)
{
    cs_status ret = CS_E_OK;
    cs_uint32 i = 0;
    cs_uint16  vlan_num;
    cs_uint16 vlan_cfg[SDL_VLAN_MAX_ENTRY];
    app_vlan_double_tag_header_t vlanHdr;
    cs_callback_context_t context;
    
    memset(&vlanHdr, 0, sizeof(app_vlan_double_tag_header_t));
    vlan_header_parse(buf, &vlanHdr); 
    if(vlanHdr.svlan == 0) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s drop, no vlan tag\n", __func__);
        return APP_VLAN_RULE_DROP;
    }
    ret = epon_request_onu_mc_vlan_get(context,ONU_DEVICEID_FOR_API,ONU_LLIDPORT_FOR_API,
                           dport, vlan_cfg, &vlan_num);
    if(ret != CS_E_OK) {
        return ret;
    }
    
    for(i=0; i<vlan_num; i++) {
        if(vlanHdr.svlan == vlan_cfg[i]) {
            if(mode == SDL_MC_VLAN_TAG_KEEP) {
                APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, vlan %d in the list\n", __func__, vlanHdr.svlan);
                return APP_VLAN_RULE_NOOP_FORWARD;
            }
            else {
                pop_vlan(buf, buf, len);
                APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, pop vlan %d\n", __func__, vlanHdr.svlan);
                return APP_VLAN_RULE_POP_FORWARD;
            }
        }
    }
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s drop\n", __func__);
    
    return APP_VLAN_RULE_DROP;
    
}

/*
*   PROTOTYPE    cs_int32 mc_vlan_egress_swap(
                                                    cs_port_id_t dport,
                                                    cs_uint16 num,
                                                    cs_sdl_vlan_cfg_t *vlan_cfg,
                                                    cs_uint32 *len,
                                                    cs_uint8 *buf)
*   INPUT            
*   OUTPUT         
*   Description     1) if packet has no vlan, drop it.
*                       2) if packet has vlan and it is in the list,swap them.
*                       3) see note.
*                       4) otherwise, drop it.
*   Pre-condition  
*   Post-condition
*   Note              If svlan is not within vlan pair but within mc vlan list, keep the vlan.
*   Return code    APP_VLAN_RULE_SWAP_FORWARD
*                       APP_VLAN_RULE_DROP
*/
cs_int32 mc_vlan_egress_swap(
                                                    cs_port_id_t dport,
                                                    cs_uint16 num,
                                                    cs_sdl_vlan_cfg_t *vlan_cfg,
                                                    cs_uint32 *len,
                                                    cs_uint8 *buf)
{
    cs_status ret = CS_E_OK;
    cs_uint32 i = 0;
    cs_uint16 vlan_num = 0;
    cs_uint16 vlan_list[SDL_VLAN_MAX_ENTRY];
    app_vlan_double_tag_header_t vlanHdr;
    cs_callback_context_t context;
    
    memset(&vlanHdr,0,sizeof(app_vlan_double_tag_header_t));
    vlan_header_parse(buf,&vlanHdr); 
    if(vlanHdr.svlan == 0) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s drop, no vlan tag\n", __func__);
        return APP_VLAN_RULE_DROP;
    }
    for(i=0; i<num; i++) {
        if(vlanHdr.svlan == vlan_cfg[i].s_vlan.vid) {
            APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s, swap vlan %d(%d)\n",
                                                            __func__, vlan_cfg[i].c_vlan.vid, vlanHdr.svlan);
            swap_vlan(vlanHdr.svlan, vlan_cfg[i].c_vlan.vid, buf, buf, *len);
            return APP_VLAN_RULE_SWAP_FORWARD;
        }
    }
    /* If svlan is within mc vlan list, keep the vlan. */
    ret = epon_request_onu_mc_vlan_get(context,ONU_DEVICEID_FOR_API,ONU_LLIDPORT_FOR_API,
                           dport, vlan_list, &vlan_num);
    if(ret != CS_E_OK) {
        APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s drop, get mc vlan failed %d\n", __func__, ret);
        return APP_VLAN_RULE_DROP;
    }
    
    for(i=0; i<vlan_num; i++) {
        if(vlanHdr.svlan == vlan_list[i]) {
            APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s keep, vlan %d in mc list\n", __func__, vlanHdr.svlan);
            return APP_VLAN_RULE_NOOP_FORWARD;
        }
    }
    
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG0, "%s drop, vlan %d is not in the list\n", __func__, vlanHdr.svlan);
    
    return APP_VLAN_RULE_DROP;
    
}

cs_int32 mc_vlan_egress_filter(
                                                    cs_port_id_t dport,
                                                    cs_uint32 *len,
                                                    cs_uint8 *buf)
{
    cs_int32 ret = 0;
    cs_uint16  vlan_num;
    cs_sdl_mc_vlan_act_t mode;
    cs_sdl_vlan_cfg_t vlan_cfg[SDL_VLAN_MAX_ENTRY];
    cs_callback_context_t         context;
    
    /* vlan rule does not apply to PON port. */
    if(dport == CS_PON_PORT_ID) {
        return CS_E_OK;
    }
    
    ret = epon_request_onu_mc_vlan_action_get(context,ONU_DEVICEID_FOR_API,ONU_LLIDPORT_FOR_API,
                           dport, &mode, vlan_cfg, &vlan_num);
    if(ret != CS_E_OK) {
        return CS_E_ERROR;
    }
    APP_VLAN_LOG(IROS_LOG_LEVEL_DBG3, "%s, dport %d, vlan mode %d\n", __func__, dport, mode);
    switch(mode)
    {
        case SDL_MC_VLAN_TAG_KEEP:
        /* fallthrough*/
			break;
        case SDL_MC_VLAN_TAG_STRIP:
            ret = mc_vlan_egress_keep_strip(mode, dport, len, buf);
            break;
        case SDL_MC_VLAN_TAG_SWAP:
            ret = mc_vlan_egress_swap(dport, vlan_num, vlan_cfg, len, buf);
            break;
        default :
            break;
    }
    
    return ret;
    
}

/*
*   PROTOTYPE    cs_status vlan_flooding_pkt_per_port(cs_port_id_t ,cs_uint8 ,cs_uint8 *,cs_uint16 )
*   INPUT            
*   OUTPUT         
*   Description     flooding packet per vlan
*                       1) send packet without vlan
*                       2) send packet with default vlan
*                       3) send packet with vlan configured on this port.
*   Pre-condition  
*   Post-condition
*   Note             
*   Return code    CS_E_ERROR
*                       CS_E_OK
*/
cs_status vlan_flooding_pkt_per_port(cs_port_id_t dport,cs_uint8 pri,cs_uint8 *in,cs_uint16 len)
{
    cs_status ret = CS_E_OK;
    cs_uint16 i = 0;
    cs_uint16 vlan_id;
    cs_uint16 vlan_num = 0;
    cs_uint32 pkt_len = 0;
    cs_uint8 *pbuf = NULL;
    cs_sdl_vlan_mode_t mode;
    cs_sdl_vlan_tag_t dft_vlan;
    cs_sdl_vlan_cfg_t vlan_cfg[SDL_VLAN_MAX_ENTRY];
    cs_callback_context_t context;

    /* send packet without any vlan */
    epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
                                                                           in, len,dport, pri);

    ret = epon_request_onu_vlan_get(context,ONU_DEVICEID_FOR_API,ONU_LLIDPORT_FOR_API,
                                        dport, &dft_vlan, &mode, vlan_cfg, &vlan_num);
    if(ret != CS_E_OK) {
        IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3, IROS_MID_APP, 
    		 "vlan flooding error ,dport %d, len %d,vlan count %d\n", dport, len, vlan_num);
        return CS_E_ERROR;
    }
    /**/
    pbuf = (cs_uint8*)iros_malloc(IROS_MID_APP, len + sizeof(app_vlan_header_t));
    if(pbuf == NULL)
    {
    	IROS_LOG_MSG(IROS_LOG_LEVEL_CRI, IROS_MID_APP, 
    		 "vlan flooding error ,dport %d, len %d,no memory\n",dport,len);
    	return CS_E_ERROR;
    }
    memset(pbuf,0, len + sizeof(app_vlan_header_t));
    if(dft_vlan.vid) {
        /* send packet with default vlan */
        pkt_len = len;
        vlan_id = (dft_vlan.vid&VLAN_MASK_BIT) | ((dft_vlan.pri&PRI_MASK_BIT)<<PRI_BIT_OFFSET);
        push_vlan(vlan_id, in, pbuf, &pkt_len);
        epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
                                                                           pbuf, pkt_len, dport, dft_vlan.pri);
    }
    /* send packet per vlan */
    for(i=0; i<vlan_num; i++)
    {
        pkt_len = len;
        if(vlan_cfg[i].c_vlan.vid) {
            vlan_id = (vlan_cfg[i].c_vlan.vid&VLAN_MASK_BIT) | ((vlan_cfg[i].c_vlan.pri&PRI_MASK_BIT)<<PRI_BIT_OFFSET);
            push_vlan(vlan_id, in, pbuf, &pkt_len);
            epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
                                                                               pbuf, pkt_len, dport, vlan_cfg[i].c_vlan.pri);
        }
        else {
            IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3, IROS_MID_APP, 
            		 "vlan flooding ,dport %d, vlan count %d, mode %d, cvid %d, svid %d\n",
            		 dport,vlan_num, mode,vlan_cfg[i].c_vlan.vid, vlan_cfg[i].s_vlan.vid );
        }
    }
    iros_free(pbuf);
    IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3, IROS_MID_APP, 
    		 "vlan flooding ,dport %d, vlan count %d\n",dport,vlan_num);
    
    return ret;
    
}

cs_status vlan_cmd_proc(int argc , char **argv)
{
    cs_uint32   i=0;
    cs_uint32   offset=0;
    cs_port_id_t   portid=0;
    cs_sdl_vlan_mode_t    vlan_mode=0;
    cs_sdl_vlan_tag_t   pvid;
    cs_sdl_vlan_cfg_t   vlan_table[__VLAN_RULE_PER_PORT_MAX];
    cs_uint16   temp_vlan[__VLAN_RULE_PER_PORT_MAX*2];
    cs_uint16   vlan_count=0;
    cs_callback_context_t context;
    cs_int32    device_id=0;
    cs_int32    llidport=0;
    cs_uint8 * vlan_mode_name[]={"TRANSPARENT", "TAG", "TRANSLATION", "AGGREGATION", "TRUNK", "STACKING"};
    //cs_uint16  areg_vlan_num=0;
    
    cs_status   ret=0;


    if ( argc ==2) {
        cs_printf ( "\r\n%-10s -%s \r\n", "app vlan dump [port]", "-show CTC vlan table");
        cs_printf ( "\r\n%-10s -%s \r\n", "app vlan set" , "-app vlan set [port] [Mode] [pvid] <param1> ... <param n> ");
    }

    memset(&pvid, 0, sizeof(cs_sdl_vlan_tag_t));
    memset (vlan_table, 0, sizeof(vlan_table));

    offset=2;
    if (strcmp("dump", argv[offset])==0) {
        /*dump vlan table*/ 
        offset++;
         portid=atoi (argv[ offset]);
        ret=epon_request_onu_vlan_get ( context,  device_id,  llidport,  portid , &pvid, &vlan_mode,  vlan_table , &vlan_count);

        cs_printf ("\r\n port %d : %-10s mode, pvid=%d \r\n", portid, vlan_mode_name[vlan_mode], pvid.vid);
        for ( i=0; i< vlan_count; i++  ) {
            cs_printf (" index:%d, c_vlan=%d, s_vlan=%d \r\n ", i, vlan_table[i].c_vlan.vid, vlan_table[i].s_vlan.vid);
        }
        cs_printf ("\r\n ");
        
    }

    if (strcmp("set", argv[offset])==0) {
        if ( argc < 5) {
            return  CS_E_PARAM;
        }
        /*set vlan table*/
        offset++;
        portid=atoi (argv[offset++]);

        vlan_mode=atoi (argv[offset++]);
        pvid.tpid=0x8100;
        pvid.vid=atoi (argv[offset++]); 
        cs_printf ("\r\n portid=%d, vlan_mode=%d, pvid.vid=%d, offset=%d, argc- offset=%d \r\n", portid, vlan_mode, pvid.vid, offset, argc- offset);


        if (vlan_mode==SDL_VLAN_MODE_STACKING) {
            return  CS_E_PARAM;
        }

        if ( vlan_mode !=SDL_VLAN_MODE_TRANSPARENT ) {
            vlan_count=argc- offset;
            for ( i=0; i < vlan_count; i++) {
                temp_vlan[i]=atoi (argv[offset++]);
                cs_printf ( "\r\n index:%d, vlan=%d \r\n", i,  temp_vlan[i] );
            }
           
        }
        
        switch (vlan_mode ) {
            case    SDL_VLAN_MODE_TRANSPARENT:
                        vlan_count=0;
                        break;

            case    SDL_VLAN_MODE_TAG:
                        vlan_count=0;
                        break;

            case    SDL_VLAN_MODE_TRANSLATION:
            case    SDL_VLAN_MODE_AGGREGATION:
                        vlan_count=vlan_count/2;
                        for ( i=0; i < vlan_count; i++) {
                            vlan_table[i].c_vlan.vid=temp_vlan[i*2];
                            vlan_table[i].s_vlan.vid=temp_vlan[i*2+1];
                        }
                        
                        break;

            case    SDL_VLAN_MODE_TRUNK:
                        for ( i=0; i < vlan_count; i++) {
                            vlan_table[i].c_vlan.vid=temp_vlan[i];
                            vlan_table[i].s_vlan.vid=temp_vlan[i];
                        }


                        break;



            default:
                       break;

        }
      
        cs_printf ("\r\n portid=%d, vlan_mode=%d, pvid.vid=%d , vlan_count=%d \r\n", portid, vlan_mode, pvid.vid, vlan_count);
    

        for ( i=0; i < vlan_count; i++) {
            cs_printf ("\r\n index:%d, c_vlan=%d, s_vlan=%d \r\n", i, vlan_table[i].c_vlan.vid, vlan_table[i].s_vlan.vid );
        }

        ret=epon_request_onu_vlan_set ( context,  device_id,  llidport,  portid,  pvid, vlan_mode, vlan_table, vlan_count );

        
    }



    return ret;



}

#ifdef HAVE_ZTE_SIJIE

cs_status manage_vlan_cmd_proc(int argc , char **argv)
{
    cs_uint32   offset=0;
    cs_port_id_t   portid=0;
    cs_uint16           vlan_id;
    cs_callback_context_t context;
    cs_int32    device_id=0;
    cs_int32    llidport=0;
    cs_status   ret=0;


    if ( argc ==2) {
        cs_printf ( "\r\n%-10s -%s \r\n", "app manage_vlan get [port]", "-show manage vlan");
        cs_printf ( "\r\n%-10s -%s \r\n", "app manage_vlan set" , "-app manage_vlan set [port] [manage_vlanid]");
        cs_printf ( "\r\n%-10s -%s \r\n", "app manage_vlan del" , "-app manage_vlan del [port] [manage_vlanid]");
    }


    offset=2;
    if (strcmp("get", argv[offset])==0) {
        /*dump vlan table*/ 
        offset++;
        portid=atoi (argv[ offset]);
        ret=epon_request_onu_manage_vlan_get ( context,  device_id,  llidport,  portid ,&vlan_id);

        cs_printf ("\r\n port %d manage vlanid : %d \r\n", portid, vlan_id);
 
    }

    if (strcmp("set", argv[offset])==0) {
        if ( argc < 5) {
            return  CS_E_PARAM;
        }
     
        offset++;
        portid=atoi (argv[offset++]);

        vlan_id=atoi (argv[offset]);


        ret=epon_request_onu_manage_vlan_set( context, device_id,  llidport, portid,vlan_id);
        
    }

    if (strcmp("del", argv[offset])==0) {
        offset++;
        portid=atoi (argv[offset]);

        ret=epon_request_onu_manage_vlan_del( context, device_id,  llidport, portid);

    }

    return ret;



}
#endif /*END_HAVE_ZTE_SIJIE*/
