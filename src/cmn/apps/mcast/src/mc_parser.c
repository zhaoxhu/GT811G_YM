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
#include "mc_type.h"
#include "mc_timer.h"
#include "mc_fdb.h"
#include "mc_core.h"
#include "mc_util.h"
#include "mc_src.h"
#include "mc_grp.h"
#include "mc_adapter.h"
#include "mc_parser.h"
#include "mc_test.h"
#define OPT_HOP_BY_HOP 0
#define OPT_DST_OPT_HDR 60
#define OPT_ROUTING_HDR 43
#define OPT_FRAGMENT_HDR 44
#define OPT_AUTHENTICATION_HDR 51
#define OPT_ESP_HDR 50

cs_status mc_pkt_object_build(mc_node_t *node, cs_pkt_t *pkt, mc_object_t *message)
{
    mc_port_t *port_cfg = node->port[pkt->port];
    
    cs_uint8 *pBuf = pkt->data + pkt->offset;       

    memset(message, 0, sizeof(mc_object_t));

    message->ether_type = pkt->eth_type;
    message->port = port_cfg;
    message->frame = pBuf;
    message->len = pkt->len;

    /* l2 parsing */
    message->ether_head_len = sizeof(mc_eth_hdr_t) + pkt->tag_num * sizeof(mc_vlan_hdr_t);
    message->vlanid = pkt->svlan;    

    /* l3 parsing */
    pBuf += message->ether_head_len;
    message->ip_hdr = pBuf;
    
    if(message->ether_type == MC_TYPE_IPV4){
        igmp_ip_header_t *ipv4_header = (igmp_ip_header_t *)pBuf;
        
        message->ip_type = MC_IPV4;        
        message->ip_head_len = 4 * (ipv4_header->version_ihl & 0x0f);
        message->pdu_len = ntohs(ipv4_header->tot_len) - message->ip_head_len;
    }
    else {
        if(node->ipv6_aware) {
            mld_ip_header_t *ipv6_header = (mld_ip_header_t *)pBuf;
            cs_uint16 router_alert_option_hdr_len = 0;
            
            message->ip_type = MC_IPV6;        
            #if 1
            {
                cs_uint8 *payload;
                cs_uint32 opt_cnt = 0;
                cs_uint8 *next_hdr = NULL;
                cs_uint8 opts[6] = {OPT_HOP_BY_HOP, OPT_DST_OPT_HDR,
                    OPT_ROUTING_HDR, OPT_FRAGMENT_HDR, OPT_AUTHENTICATION_HDR,OPT_ESP_HDR};
                cs_uint8 opts_len[6] = {0, 0, 0, 0, 0,12};
                cs_uint8 opts_hdr_len[6] = {8, 8, 8, 8, 16,16};
                payload = pkt->data + pkt->offset + 14 + pkt->tag_num * 4;
    

                if (payload[6] == 58)  {    /* next_header */
                    router_alert_option_hdr_len = 0;
                }
                else {
                    next_hdr = &payload[6] ;
                    while(opt_cnt < sizeof(opts)/sizeof(cs_uint8)) {
                        MC_LOG(IROS_LOG_LEVEL_INF,"%s: opt %d, opt_cnt%d, router_alert_option_hdr_len %d\n",
                            __func__, *next_hdr, opt_cnt, router_alert_option_hdr_len);
                        if(*next_hdr == 58) {
                            break;
                        }
                        else if(*next_hdr == opts[opt_cnt]) {
                            //with correct option                
                            next_hdr = &payload[40+opt_cnt*8+opts_len[opt_cnt]];
                            router_alert_option_hdr_len += opts_hdr_len[opt_cnt];
                            opt_cnt++;
                        }
                        else {
                            break;
                        }
                    }
                }
            }
            #else
            if (ipv6_header->nexthdr == 0x0) {
                router_alert_option_hdr_len = 8;
            }        
            #endif
            MC_LOG(IROS_LOG_LEVEL_INF,"%s: router_alert_option_hdr_len %d, ipv6_header->payload_len %d\n", 
            __func__, router_alert_option_hdr_len,ntohs( ipv6_header->payload_len));

            message->ip_head_len = 40 + router_alert_option_hdr_len;
            message->pdu_len = ntohs(ipv6_header->payload_len) - router_alert_option_hdr_len;
        }
        else {
            return CS_E_NOT_SUPPORT;
        }
    }

    /* pdu parsing */
    pBuf += message->ip_head_len;
    message->pdu = (mc_pdu_t *)pBuf;
    message->pdu_type = message->pdu->type;

    /* add dump code here */
    
    /* port stats */
    switch(message->pdu_type) { 
        case IGMP_V1_REPORT:
            port_cfg->in_igmpv1_report++;
            break;

        case IGMP_V2_REPORT:
            port_cfg->in_igmpv2_report++;
            break;

        case IGMP_V3_REPORT:
            port_cfg->in_igmpv3_report++;
            break;

        case IGMP_V2_LEAVE:
            port_cfg->in_igmpv2_leave++;
            break;

        case MLD_V1_REPORT:
            port_cfg->in_mldv1_report++;
            break;

        case MLD_V2_REPORT:
            port_cfg->in_mldv2_report++;
            break;

        case MLD_V1_DONE: 
            port_cfg->in_mldv1_done++;
            break;

        case IGMP_QUERY:
            port_cfg->in_igmp_query++;
            break;

        case MLD_QUERY:
            port_cfg->in_mld_query++;
            break;

        default:
            return CS_E_NOT_SUPPORT;
    }
    
    cs_lst_init(&message->record_list, NULL);
    return CS_E_OK;    
}

void mc_pkt_obj_update(cs_pkt_t *pkt, mc_object_t *msg)
{
    cs_uint8 *pBuf = pkt->data + pkt->offset;
    msg->frame = pBuf;
    msg->vlanid = pkt->svlan;
    msg->len = pkt->len;
    msg->ether_head_len = sizeof(mc_eth_hdr_t) + pkt->tag_num * sizeof(mc_vlan_hdr_t);
    pBuf += msg->ether_head_len;
    msg->ip_hdr = pBuf;     
	pBuf += msg->ip_head_len;
	msg->pdu = (mc_pdu_t *)pBuf;
    MC_LOG(IROS_LOG_LEVEL_INF, "recv msg: vlan = %d, len = %d\n", msg->vlanid, msg->len);
}

/* 
 * record parser 
 */

/* report for IGMP V1, V2 or MLD V1 */
cs_status mc_asm_record_parse(mc_node_t *node,mc_object_t *pObj)
{
    mc_pdu_t *pdu = pObj->pdu;
    mc_group_record_list_t *record = NULL;
    mc_record_type_t    record_type = 0;
    cs_uint8               *ip_data = NULL;
    
    MC_LOG(IROS_LOG_LEVEL_DBG0, "%s,entering\n", __func__);
    record = mc_record_create();
    if (record == NULL) {
        MC_LOG(IROS_LOG_LEVEL_CRI, "%s,no memory\n", __func__);
        return CS_E_ERROR;
    }

    record->entry.vlanid = pObj->vlanid;
    record->entry.pdu_type = pdu->type;
    if (!MC_IS_LEAVE_PKT(pdu->type)) {
        record_type = MODE_IS_EXCLUDE;
    } 
    else {
        record_type = CHANGE_TO_INCLUDE_MODE;
    }
    record->entry.record_type = record_type;
    record->entry.src_list.count = 0;
    if(pObj->ip_type == MC_IPV4) {
        ip_data = (cs_uint8*)&(pdu->grp_key.ipv4_grp);
    } 
    else {
        ip_data = (cs_int8 *)pdu + sizeof(mc_pdu_t);
    }
    if(node->fwd_engine != MC_ENGINE_IP && node->mode == MC_MANUAL) {
        mc_set_dft_ip(pObj->ip_type,ip_data,&record->entry.mc_grp);
    }
    else {
        mc_data_to_ip(pObj->ip_type,ip_data,&record->entry.mc_grp);
    }
    mc_record_add(&(pObj->record_list), record);   

    MC_LOG(IROS_LOG_LEVEL_DBG0, "%s,exit\n", __func__);
    return CS_E_OK;
}


cs_status mc_ssm_record_src_parse(
       mc_ip_type_t ip_type,
       mc_group_record_pdu_t *record_pdu,
       cs_list *src_list)
{
    cs_uint16 src_ip_num;
    cs_uint8 *src_table ;
    cs_uint32  i = 0;
    cs_uint32 offset = 0;
    mc_source_record_list_t  *src_record = NULL;
    
    src_ip_num = ntohs(record_pdu->num_sources);
    if(src_ip_num == 0) {
        return CS_E_OK;
    }
    /* sanity check */
    if(src_ip_num > MC_MAX_SRC_IP_NUM) {
        MC_LOG(IROS_LOG_LEVEL_MAJ, "%s,too many source ip address %d\n", __func__,src_ip_num);
        return CS_E_ERROR;
    }
    
    offset = MC_SRC_OFFSET_PER_GRP(ip_type);
    for (i=0; i<src_ip_num; i++) {
        src_record = mc_src_create();
        if (src_record == NULL) {
            MC_LOG(IROS_LOG_LEVEL_CRI, "%s,no memory\n", __func__);
            return CS_E_ERROR;
        }
        
        src_table = (cs_uint8*)record_pdu + offset;
        mc_data_to_ip(ip_type, src_table, &src_record->entry.src_ip);
        if(mc_is_grp_ip(&src_record->entry.src_ip)) {
            /* sanity check: group ip address can not be the source ip */
            mc_free(src_record);
            MC_LOG(IROS_LOG_LEVEL_MAJ, "%s,invalid source ip address\n", __func__);
            return CS_E_ERROR;
        }
        mc_src_add(src_list, src_record);
        
        /* parse next source ip address */
        offset += MC_IP_ADDR_LEN(ip_type);
    }
    
    return CS_E_OK;
}

/* report for IGMP V3 or MLD V2 */
cs_status mc_ssm_record_parse(mc_node_t *node,mc_object_t *pObj)
{
    cs_status ret = 0;
    cs_uint16  group_num ;
    cs_uint16 ip_type = pObj->ip_type;
    cs_uint32  i = 0;
    cs_uint32 offset = 0;
    cs_uint8 *record_tbl = NULL;
    mc_pdu_t *pdu = pObj->pdu;
    mc_group_record_list_t *record = NULL;
    mc_group_record_pdu_t *record_pdu = NULL;

    MC_LOG(IROS_LOG_LEVEL_DBG0,"%s,entering\n",__func__);
    /* sanity check */
    if(pObj->pdu_len < sizeof(mc_pdu_t)) {
        MC_LOG(IROS_LOG_LEVEL_MAJ,"%s,invalid packet foramt, pdu len %d\n",__func__,pObj->pdu_len);
        return CS_E_ERROR;
    }
    
    group_num = ntohs(pdu->grp_key.reserved.num_grp_records);
    offset = sizeof(mc_pdu_t);
    for (i=0; i<group_num; i++) {
        /* sanity check */
        if(offset >= pObj->pdu_len) {
            MC_LOG(IROS_LOG_LEVEL_MAJ,"%s, offset %d, pdu len %d\n",__func__, offset, pObj->pdu_len);
            return CS_E_ERROR;
        }
        
        record_tbl = (cs_uint8 *)pdu + offset;
        record = mc_record_create();
        if (record == NULL) {
            MC_LOG(IROS_LOG_LEVEL_CRI, "%s,no memory\n", __func__);
            return CS_E_ERROR;
        }
        record->entry.vlanid = pObj->vlanid;
        record->entry.pdu_type = pObj->pdu->type;
        record_pdu = (mc_group_record_pdu_t *)record_tbl;
        record->entry.record_type = record_pdu->record_type;

        /* parse mc group ip address */
        if(node->fwd_engine != MC_ENGINE_IP && node->mode == MC_MANUAL) {
            mc_set_dft_ip(ip_type,(cs_int8 *)&record_pdu->mc_grp,&record->entry.mc_grp);
        }
        else {
            mc_data_to_ip(ip_type, (cs_int8 *)&record_pdu->mc_grp, &record->entry.mc_grp);
            if(!mc_is_grp_ip(&record->entry.mc_grp)) {
                /* if group ip is not valid, ignore and return */
                MC_LOG(IROS_LOG_LEVEL_MAJ,"%s, invalid group address\n",__func__);
                mc_record_destroy(record);
                return CS_E_ERROR;
            }
        }

        /* parse source ip list */
        ret = mc_ssm_record_src_parse(ip_type, record_pdu, &record->entry.src_list);
        if(ret != CS_E_OK) {
            mc_record_destroy(record);
            return CS_E_ERROR;
        }
        mc_record_add(&(pObj->record_list), record);
        
        /* parse next group record */
        offset += MC_SIZEOF_GROUP_RECORD(ip_type, *record_pdu);
    }
    
    MC_LOG(IROS_LOG_LEVEL_DBG0,"%s,exit\n",__func__);
    return CS_E_OK;
}


cs_status mc_grp_record_parse(mc_node_t *node,mc_object_t *pObj)
{
    cs_status ret = CS_E_OK;
    if( IGMP_V1_REPORT == pObj->pdu_type ||
        IGMP_V2_REPORT == pObj->pdu_type ||
        IGMP_V2_LEAVE == pObj->pdu_type ||
        MLD_V1_REPORT == pObj->pdu_type ||
        MLD_V1_DONE == pObj->pdu_type) {
        ret = mc_asm_record_parse(node, pObj);
    }
    else {
        ret = mc_ssm_record_parse(node, pObj);
    }

    if(CS_E_OK == ret) {
        MC_REPORT_RECORD_DUMP(pObj);
    }
    
    return ret;
}

cs_status mc_query_parse(mc_node_t *node, mc_object_t *pObj)
{
    cs_status ret = CS_E_OK;
    if(pObj->ip_type == MC_IPV4) {
        ret = igmp_query_parse(node, pObj);
    }
    else {
        ret = mld_query_parse(node, pObj);
    }

    MC_QUERY_RECORD_DUMP(pObj);
        
    return ret;
}

