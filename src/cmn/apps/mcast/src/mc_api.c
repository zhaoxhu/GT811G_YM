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
#include "mc_pkt.h"
#include "mc_core.h"
#include "mc_util.h"
#include "mc_src.h"
#include "mc_fdb.h"
#include "mc_adapter.h"
#include "mc_mgmt.h"
#include "mc_devm.h"
#include "mc_test.h"

#include "mc_api.h"
#include "startup_cfg.h"

 
extern mc_fwd_action_t mc_snoop_ingress_filter(cs_pkt_t *pkt, cs_uint8 type, cs_uint8 *chg);
extern mc_fwd_action_t mc_ctrl_ingress_filter(cs_pkt_t *pkt, cs_uint8 type, cs_uint8 *chg);
extern cs_status mc_egress_filter(cs_port_id_t port, cs_uint8 type, cs_uint8 *frame, cs_uint16 *len);
extern  void mc_ctrl_init();
extern cs_status epon_request_onu_mc_vlan_clr
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id
);


cs_status  mc_port_fdb_entry_get(
    mc_node_t *mc_node,
    cs_port_id_t  portid,     
    cs_uint32      offset,
    cs_uint8        *mac,
    cs_uint16     *vlanid);

/* device level */
cs_status mc_mode_get(cs_dev_id_t device, mc_mode_t *mode)
{
    mc_node_t   *mc_node = NULL;
    
    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *mode = mc_node_get_mode(mc_node);
    }
    else {
        return CS_E_ERROR;
    }
    
    return CS_E_OK;

}

cs_status mc_mode_set(cs_dev_id_t device, mc_mode_t mode)
{
    cs_status     ret = CS_E_OK;
    mc_node_t   *mc_node = NULL;
    cs_uint8        port_num=0;
    cs_uint8        port=0;
    mc_node = MC_GET_NODE(device);
    if(!mc_node) {
        return CS_E_ERROR;
    }
    if(mc_node->mode == mode) {
        MC_LOG(IROS_LOG_LEVEL_INF,"%s, mode %d not change\n",__func__, mode);
        return CS_E_OK;
    }

    switch ( mode) {
        case    MC_SNOOPING:
                    mc_vlan_lrn_set(0, MC_SVL);
                    mc_set_port_ingress_filter(0, mc_snoop_ingress_filter);
                    mc_set_port_egress_filter(0, mc_egress_filter);

                    /* set max group to max value */
                    startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &port_num);
                    port_num++;
                    
                    for(port = 1; port < port_num; port++) {
                        mc_port_max_grp_set(0, port, mc_node->port_grp_cap);
                    }
                    break;

        case    MC_MANUAL:
                    mc_ctrl_init();
                    mc_set_port_ingress_filter(0, mc_ctrl_ingress_filter);
                    mc_set_port_egress_filter(0, mc_egress_filter);

                    /* set max group to max value */
                    startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &port_num);
                    port_num++;
                    
                    for(port = 1; port < port_num; port++) {
                        mc_port_max_grp_set(0, port, MC_PORT_DEFAULT_GROUP);
                    }
            
                    break;
                    
        case     MC_PROXY:
                    /*innore*/
                    break;


        case    MC_DISABLE:
                    break;
    

        default:
                    break;

    }


    /* clear mc vlan */
    if ( mode !=MC_DISABLE) {
        startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &port_num);
        for(port = 1; port < port_num+1; port++) {
            cs_callback_context_t  context;
            epon_request_onu_mc_vlan_clr(context, 0, 0, port);
        }
    }
    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_set_mode(mc_node, mode);
    }
    else {
        return CS_E_ERROR;
    }

    return ret;
    
}

/* 
* get & set vlan learn mode 
* device level 
*/
cs_status mc_vlan_lrn_get(cs_dev_id_t device, mc_vlan_learn_t *lrn_mode)
{
    mc_node_t *mc_node = NULL;
    
    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *lrn_mode = mc_node_get_vlan_lrn(mc_node);
    }
    else {
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
}

cs_status mc_vlan_lrn_set(cs_dev_id_t device, mc_vlan_learn_t lrn_mode)
{
    cs_status     ret = CS_E_OK;
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_set_vlan_lrn(mc_node, lrn_mode);
    }
    else {
        return CS_E_ERROR;
    }
        
    return ret;
}

/*
 * fwd engine
 */

cs_status mc_fwd_engine_set(cs_dev_id_t device, mc_fwd_engine_t engine)
{
    cs_status     ret = CS_E_OK;
    mc_node_t *mc_node = NULL;

    if(engine > MC_ENGINE_IP) {
        return CS_E_PARAM;
    }

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_set_fwd_engine(mc_node, engine);
    }
    else {
        return CS_E_ERROR;
    }
        
    return ret;
}

cs_status mc_fwd_engine_get(cs_dev_id_t device, mc_fwd_engine_t *engine)
{
    mc_node_t *mc_node = NULL;
    
    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *engine = mc_node_get_fwd_engine(mc_node);
    }
    else {
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
}


/* 
* get & set up port configuration 
* device level 
*/
cs_status mc_up_port_get(cs_dev_id_t device, mc_up_port_cfg_t *up_cfg)
{
    cs_status     ret = CS_E_OK;
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_get_up_port(mc_node, up_cfg);
    }
    else {
        return CS_E_ERROR;
    }
    
    return ret;
}

cs_status mc_up_port_set(cs_dev_id_t device, const mc_up_port_cfg_t *up_cfg)
{
    cs_status     ret = CS_E_OK;
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_set_up_port(mc_node, up_cfg);
    }
    else {
        return CS_E_ERROR;
    }
    
    return ret;
}

/* 
* get & set max group number 
* device level 
*/
cs_status mc_max_grp_get(cs_dev_id_t device, cs_uint16 *max_grp)
{
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *max_grp = mc_node_get_max_grp(mc_node);
    }
    else {
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

cs_status mc_max_grp_set(cs_dev_id_t device, cs_uint16 max_grp)
{
    cs_status     ret = CS_E_OK;
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_set_max_grp(mc_node, max_grp);
    }
    else {
        return CS_E_ERROR;
    }

    return ret;
}

cs_status mc_grp_num_get(cs_dev_id_t device, cs_uint16 *grp_num)
{
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *grp_num = mc_node_get_grp_num(mc_node);
    }
    else {
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

/* 
* IGMP protocol related set&get functions 
*/

/* 
* get & set robustness number 
*/
cs_status mc_robustness_get(cs_dev_id_t device, cs_uint32 *robustness)
{
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *robustness = mc_node_get_robustness(mc_node);
    }
    else {
        return CS_E_ERROR;
    }

    return CS_E_OK;
    
}

cs_status mc_robustness_set(cs_dev_id_t device, cs_uint32 robustness)
{
    cs_status     ret = CS_E_OK;
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_set_robustness(mc_node, robustness);
    }
    else {
        return CS_E_ERROR;
    }

    return ret;
    
}

/* 
* get & set query interval 
*/
cs_status mc_query_interval_get(cs_dev_id_t device, cs_uint32 *interval)
{
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *interval = mc_node_get_query_interval(mc_node);
    }
    else {
        return CS_E_ERROR;
    }

    return CS_E_OK;
    
}

cs_status mc_query_interval_set(cs_dev_id_t device, cs_uint32 interval)
{
    cs_status     ret = CS_E_OK;
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_set_query_interval(mc_node, interval);
    }
    else {
        return CS_E_ERROR;
    }

    return ret;
    
}

/* 
* get & set query response interval 
*/
cs_status mc_query_rsp_interval_get(cs_dev_id_t device, cs_uint32 *interval)
{
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *interval = mc_node_get_query_rsp_interval(mc_node);
    }
    else {
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
    
}

cs_status mc_query_rsp_interval_set(cs_dev_id_t device, cs_uint32 interval)
{
    cs_status     ret = CS_E_OK;
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_set_query_rsp_interval(mc_node, interval);
    }
    else {
        return CS_E_ERROR;
    }

    return ret;
    
}

cs_status mc_gmi_get(cs_dev_id_t device, cs_uint32 *gmi)
{
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *gmi = mc_node_get_gmi(mc_node);
    }
    else {
        return CS_E_ERROR;
    }

    return CS_E_OK;
    
}

cs_status mc_gmi_set(cs_dev_id_t device, cs_uint32 gmi)
{
    cs_status     ret = CS_E_OK;
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_set_gmi(mc_node, gmi);
    }
    else {
        return CS_E_ERROR;
    }
    
    return ret;
    
}

/* 
* get & set last member query interval 
*/
cs_status mc_last_member_query_get(cs_dev_id_t device, cs_uint32 *interval)
{
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *interval = mc_node_get_last_member_query(mc_node);
    }
    else {
        return CS_E_ERROR;
    }

    return CS_E_OK;
    
}

cs_status mc_last_member_query_set(cs_dev_id_t device, cs_uint32 interval)
{
    cs_status     ret = CS_E_OK;
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_set_last_member_query(mc_node,interval);
    }
    else {
        return CS_E_ERROR;
    }
    
    return ret;
    
}

/* 
* get & set last member query count 
*/
cs_status mc_last_member_count_get(cs_dev_id_t device, cs_uint32 *count)
{
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *count = mc_node_get_last_member_count(mc_node);
    }
    else {
        return CS_E_ERROR;
    }

    return CS_E_OK;
    
}

cs_status mc_last_member_count_set(cs_dev_id_t device, cs_uint32 count)
{
    cs_status     ret = CS_E_OK;
    mc_node_t *mc_node = NULL;

    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_set_last_member_count(mc_node,count);
    }
    else {
        return CS_E_ERROR;
    }
    
    return ret;
    
}


/* 
* get & set leave mode
*/
cs_status mc_leave_mode_get(cs_dev_id_t device, cs_port_id_t portid, mc_leave_mode_t *mode)
{
    mc_node_t   *mc_node = NULL;
    
    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *mode = mc_node_get_leave_mode(mc_node, portid);
    }
    else {
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
    
}

cs_status mc_leave_mode_set(cs_dev_id_t device, cs_port_id_t portid, mc_leave_mode_t mode)
{
    cs_status     ret = CS_E_OK;
    mc_node_t   *mc_node = NULL;
    
    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_set_leave_mode(mc_node, portid,mode);
    }
    else {
        return CS_E_ERROR;
    }
    
    return ret;
    
}

/* 
* get & set max group number per port
*/
cs_status mc_port_max_grp_get(cs_dev_id_t device, cs_port_id_t portid, cs_uint16 *max_grp)
{
    mc_node_t   *mc_node = NULL;
    
    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *max_grp = mc_node_get_port_max_grp(mc_node,portid);
    }
    else {
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
    
}


cs_status mc_port_max_grp_set(cs_dev_id_t  device, cs_port_id_t portid, cs_uint16 max_grp)
{
    cs_status     ret = CS_E_OK;
    mc_node_t   *mc_node = NULL;
    
    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        ret = mc_node_set_port_max_grp(mc_node, portid, max_grp);
    }
    else {
        return CS_E_ERROR;
    }
    return ret;
    
}

cs_status mc_port_grp_num_get(cs_dev_id_t device, cs_port_id_t portid, cs_uint16 *grp_num)
{
    mc_node_t   *mc_node = NULL;
    
    mc_node = MC_GET_NODE(device);
    if(mc_node) {
        *grp_num = mc_node_get_port_grp_num(mc_node, portid);
    }
    else {
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
    
}

cs_status mc_set_port_ingress_filter(
    cs_dev_id_t  device, 
    mc_port_ingress_filter_routine_t filter)
{
    mc_node_t   *mc_node = MC_GET_NODE(device);

    if(mc_node) {
        return mc_node_set_port_ingress_filter(mc_node, filter);
    }
    else {
        return CS_E_ERROR;
    }
}

cs_status mc_set_port_egress_filter(
    cs_dev_id_t  device, 
    mc_port_egress_filter_routine_t filter)
{
    mc_node_t   *mc_node = MC_GET_NODE(device);

    if(mc_node) {
        return mc_node_set_port_egress_filter(mc_node, filter);
    }
    else {
        return CS_E_ERROR;
    }
}

cs_uint8 mc_recv_frame(cs_pkt_t *pkt)
{
    mc_pkt_proc(pkt, g_mc_dev);

    return 1;
}

cs_status mc_get_grp_tbl(
    cs_dev_id_t  device,
    mc_ipv4_grp_t *v4_tbl, 
    cs_uint32 *v4_tbl_len,
    mc_ipv6_grp_t *v6_tbl,
    cs_uint32 *v6_tbl_len)
{
    mc_node_t   *mc_node = MC_GET_NODE(device);

    if(mc_node) {
        mc_fdb_get_grp_tbl(mc_node, v4_tbl, v4_tbl_len, v6_tbl, v6_tbl_len);
        return CS_E_OK;
    }
    else {
        return CS_E_NOT_FOUND;
    }
}

cs_status mc_port_fdb_get(
    cs_dev_id_t                 device,
    cs_port_id_t                portid,
    cs_uint32                   offset,
    cs_uint16                    *next,
    cs_uint8                      * mac,
    cs_uint16                     * vlan_id)
{
    mc_node_t   *mc_node = MC_GET_NODE(device);
    cs_status       ret=CS_E_OK;

    ret=mc_port_fdb_entry_get(mc_node, portid, offset, mac, vlan_id);
    if (ret ==CS_E_OK ) {
        *next=offset+1;
    }
    

    return ret;
}


/* 
 * debug APIs
 */
cs_status mc_dump_set(cs_uint8 enable)
{
    g_mc_dump = enable;
    cs_printf("mc dump %s\n", g_mc_dump ? "on" : "off");
    return CS_E_OK;
}

cs_status mc_port_fdb_list(cs_dev_id_t device, cs_port_id_t port_id)
{
    mc_node_t   *mc_node = MC_GET_NODE(device);

    if(mc_node) {
        mc_port_t *port = mc_port_find(mc_node, port_id);
        if(port) {
            mc_port_fdb_show(mc_node, port);
            return CS_E_OK;
        }
    }

    return CS_E_NOT_FOUND;
}

cs_status mc_vlan_fdb_list(cs_dev_id_t device, cs_uint16 vid)
{
    mc_node_t   *mc_node = MC_GET_NODE(device);

    if(mc_node) {
        mc_vlan_fdb_show(mc_node, vid);
        return CS_E_OK;
    }

    return CS_E_NOT_FOUND;
}

cs_status mc_fdb_list(cs_dev_id_t device)
{
    mc_node_t   *mc_node = MC_GET_NODE(device);

    if(mc_node) {
        mc_fdb_show(mc_node);
        return CS_E_OK;
    }

    return CS_E_NOT_FOUND;
}

cs_status mc_port_stats_show(cs_dev_id_t device, cs_port_id_t port_id)
{
    mc_node_t   *mc_node = MC_GET_NODE(device);

    if(mc_node) {
        mc_node_port_stats_show(mc_node, port_id);
        return CS_E_OK;
    }

    return CS_E_NOT_FOUND;
}

cs_status mc_port_stats_clr(cs_dev_id_t device, cs_port_id_t port_id)
{
    mc_node_t   *mc_node = MC_GET_NODE(device);

    if(mc_node) {
        mc_node_port_stats_clr(mc_node, port_id);
        return CS_E_OK;
    }

    return CS_E_NOT_FOUND;
}

