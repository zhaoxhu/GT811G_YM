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
#ifndef _MC_API_H
#define _MC_API_H

#include "mc_type.h"


extern cs_status mc_init();
extern cs_status mc_mode_get(cs_dev_id_t device, mc_mode_t *mode);
extern cs_status mc_mode_set(cs_dev_id_t device, cs_uint32 mode);
extern cs_status mc_vlan_lrn_get(cs_dev_id_t device, mc_vlan_learn_t *lrn_mode);
extern cs_status mc_vlan_lrn_set(cs_dev_id_t device, mc_vlan_learn_t lrn_mode);
extern cs_status mc_fwd_engine_set(cs_dev_id_t device, mc_fwd_engine_t engine);
extern cs_status mc_fwd_engine_get(cs_dev_id_t device, mc_fwd_engine_t *engine);
extern cs_status mc_up_port_get(cs_dev_id_t device, mc_up_port_cfg_t *up_cfg);
extern cs_status mc_up_port_set(cs_dev_id_t device, const mc_up_port_cfg_t *up_cfg);
extern cs_status mc_max_grp_get(cs_dev_id_t device, cs_uint16 *max_grp);
extern cs_status mc_max_grp_set(cs_dev_id_t device, cs_uint16 max_grp);
extern cs_status mc_grp_num_get(cs_dev_id_t device, cs_uint16 *grp_num);
extern cs_status mc_robustness_get(cs_dev_id_t device, cs_uint32 *robustness);
extern cs_status mc_robustness_set(cs_dev_id_t device, cs_uint32 robustness);
extern cs_status mc_query_interval_get(cs_dev_id_t device, cs_uint32 *interval);
extern cs_status mc_query_interval_set(cs_dev_id_t device, cs_uint32 interval);
extern cs_status mc_query_rsp_interval_get(cs_dev_id_t device, cs_uint32 *interval);
extern cs_status mc_query_rsp_interval_set(cs_dev_id_t device, cs_uint32 interval);
extern cs_status mc_gmi_get(cs_dev_id_t device, cs_uint32 *gmi);
extern cs_status mc_gmi_set(cs_dev_id_t device, cs_uint32 gmi);
extern cs_status mc_last_member_query_get(cs_dev_id_t device, cs_uint32 *interval);
extern cs_status mc_last_member_query_set(cs_dev_id_t device, cs_uint32 interval);
extern cs_status mc_last_member_count_get(cs_dev_id_t device, cs_uint32 *count);
extern cs_status mc_last_member_count_set(cs_dev_id_t device, cs_uint32 count);
extern cs_status mc_leave_mode_get(
                                                    cs_dev_id_t device, 
                                                    cs_port_id_t portid, 
                                                    mc_leave_mode_t *mode);
extern cs_status mc_leave_mode_set(
                                                    cs_dev_id_t device, 
                                                    cs_port_id_t portid, 
                                                    mc_leave_mode_t mode);
extern cs_status mc_port_max_grp_get(cs_dev_id_t device, cs_port_id_t portid, cs_uint16 *max_grp);
extern cs_status mc_port_max_grp_set(cs_dev_id_t  device, cs_port_id_t portid, cs_uint16 max_grp);
extern cs_status mc_port_grp_num_get(cs_dev_id_t device, cs_port_id_t portid, cs_uint16 *grp_num);
extern cs_status mc_set_port_ingress_filter(
    cs_dev_id_t  device, 
    mc_port_ingress_filter_routine_t filter);
extern cs_status mc_set_port_egress_filter(
    cs_dev_id_t  device, 
    mc_port_egress_filter_routine_t filter);
extern cs_uint8 mc_recv_frame(cs_pkt_t *pkt);
extern cs_status mc_get_grp_tbl(
    cs_dev_id_t  device,
    mc_ipv4_grp_t *v4_tbl, 
    cs_uint32 *v4_tbl_len,
    mc_ipv6_grp_t *v6_tbl,
    cs_uint32 *v6_tbl_len);

/*
 * debug APIs list
 */

cs_status mc_dump_set(cs_uint8 enable);
cs_status mc_port_fdb_list(cs_dev_id_t device, cs_port_id_t port_id);
cs_status mc_vlan_fdb_list(cs_dev_id_t device, cs_uint16 vid);
cs_status mc_fdb_list(cs_dev_id_t device);
cs_status mc_port_stats_show(cs_dev_id_t device, cs_port_id_t port_id);
cs_status mc_port_stats_clr(cs_dev_id_t device, cs_port_id_t port_id);

#endif /* #ifndef _MC_API_H */

