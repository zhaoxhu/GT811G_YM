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
#include "mc_core.h"
#include "mc_util.h"
#include "mc_src.h"
#include "mc_adapter.h"
#include <string.h>
/*
* IGMPv1 query
* IGMPv2 query
*/
static mc_query_type_t igmp_get_query_version(cs_uint32 pdu_len, const mc_pdu_t *pdu)
{
    if (pdu_len > IGMP_V1V2_QUERY_PDU_LEN) {
        return IGMP_V3_QUERY;
    } 
    else {
        if (pdu->time_key.max_resp_time == 0) {
            return IGMP_V1_QUERY;
        } 
        else {
            return IGMP_V2_QUERY;
        }
    }
}

cs_status  igmp_query_src_parse (cs_uint8 * src_table, cs_uint16 src_ip_num, cs_list * src_list)
{
    cs_uint32  i = 0;
    mc_source_record_list_t  *srecord = NULL;

    for (i = 0; i < src_ip_num; i++) {
        srecord = (mc_source_record_list_t *)mc_malloc(sizeof(mc_source_record_list_t));
        if (srecord == NULL) {
            return CS_E_ERROR;
        }
        memset(srecord, 0, sizeof(mc_source_record_list_t));
        
        /*parse mc group */
        /* src->entry.flag |=MC_NODE_USED; */
        mc_data_to_ip(MC_IPV4, src_table, &srecord->entry.src_ip);
        mc_src_add(src_list, srecord);
        src_table += MC_IPV4_IP_LEN;
    }

    return CS_E_OK;
}

/* called by igmp_query_parse to parse V3 query packets */
void igmp_query_v3_parse(mc_pdu_t *pdu, mc_group_record_list_t  *record)
{
    cs_uint8                       *ip_data = NULL;
    cs_uint32                      src_num = 0;
    igmp_v3_query_pdu_t  *igmpv3_pdu = NULL;

    igmpv3_pdu = (igmp_v3_query_pdu_t *)pdu;
    src_num = ntohs(igmpv3_pdu->num_sources);
    ip_data = (cs_uint8 *)pdu + sizeof(igmp_v3_query_pdu_t);
    
    /*parse source ip list*/
    if (src_num != 0) {
        /*g-s-s-q*/
        igmp_query_src_parse(ip_data, src_num, &record->entry.src_list);
        record->entry.record_type = MODE_IS_INCLUDE;
    }
}

/* IGMP V1, V2 and V3 query */
cs_status igmp_query_parse(mc_node_t *node, mc_object_t * pObj)
{
    mc_query_type_t          pdu_type;
    mc_pdu_t                    *pdu  = pObj->pdu;
    mc_group_record_list_t *record = NULL;

    MC_LOG(IROS_LOG_LEVEL_DBG0, "%s,entering\n", __func__);
    record = mc_record_create();
    if(record == NULL) {
        return CS_E_ERROR;
    }

    pdu_type = igmp_get_query_version(pObj->pdu_len, pdu);
    record->entry.vlanid = pObj->vlanid;
    record->entry.record_type = MODE_IS_EXCLUDE;
    record->entry.pdu_type = pdu_type;

    /*parse source ip list*/
    #if 0   /* disable src parser */
    if (pdu_type == IGMP_V3_QUERY) {
        /* could modify record_type */
        igmp_query_v3_parse(pdu, record);
    }
    #endif
    
    /*parse mc group */
    if(pdu->grp_key.ipv4_grp && node->fwd_engine != MC_ENGINE_IP && node->mode == MC_MANUAL) {
        mc_set_dft_ip(MC_IPV4, (cs_uint8 *)&(pdu->grp_key.ipv4_grp), &record->entry.mc_grp);
    }
    else {
        mc_data_to_ip(MC_IPV4, (cs_uint8 *)&(pdu->grp_key.ipv4_grp), &record->entry.mc_grp);
    }
           
    mc_record_add(&(pObj->record_list), record);

    MC_LOG(IROS_LOG_LEVEL_DBG0, "%s,exit\n", __func__);
    return CS_E_OK;
}


