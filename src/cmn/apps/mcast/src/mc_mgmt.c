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
its successor. Use of the SOFTWARE by the Government  itutes
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
#include "mc_devm.h"

/*
 * port manage
 */
 
mc_port_t *mc_port_create()
{
    mc_port_t *port = NULL;

    port = (mc_port_t *)mc_malloc(sizeof(mc_port_t));
    if(port == NULL) {
        MC_LOG(IROS_LOG_LEVEL_CRI, "%s,no memory\n",__func__);
    }
    else {
        memset(port, 0, sizeof(mc_port_t));
    }
    
    return port;

}

void mc_port_init (cs_port_id_t portid, mc_port_t *port)
{    
    port->portid = portid;
    port->leave_mode = PORT_NON_FAST_LEAVE;     
    port->igmp_version_level = IGMP_V2;                      
    port->mld_version_level = MLD_V1;                     
    port->max_grp = MC_PORT_DEFAULT_GROUP;                     
    port->grp_count = 0;         
}

cs_status mc_port_add(mc_port_t *port, mc_node_t *mc_node)
{
    cs_uint32  i;
    if(port == NULL || mc_node == NULL){
        MC_LOG(IROS_LOG_LEVEL_INF, "%s,invalid parameter\n",__func__);
        return CS_E_ERROR;
    }
    for (i=0; i<mc_node->port_num; i++) {
        if (mc_node->port[i] == NULL) {
            port->mc_node = (void *)mc_node;
            mc_node->port[i] = port;
            return CS_E_OK;
        }
    }
    
    return CS_E_RESOURCE;
     
}


mc_port_t *mc_port_find(mc_node_t *mc_node, cs_port_id_t port_id)
{
    if(port_id >= mc_node->port_num) {
        return NULL;
    }
    
    return mc_node->port[port_id];
}



cs_boolean mc_is_uplink_port(mc_node_t *mc_node, cs_port_id_t port_id)
{

    if(mc_node->up_port && mc_node->up_port ->portid == port_id) {
        return 1;
    }

    return 0;
    
}

/* 
* set & get mode of current node.
*/
mc_mode_t mc_node_get_mode(const mc_node_t *mc_node)
{    
    return mc_node->mode;
}

/* device level */
cs_status mc_node_set_mode(mc_node_t *mc_node, mc_mode_t mode)
{
    cs_status     ret = CS_E_OK;

    if(mc_node->mode == mode) {
        return CS_E_OK;
    }

    if(mode == MC_DISABLE) {
        mc_ctrl_enable(mc_node->device, FALSE);
    }
    else {
        if(mc_node->mode == MC_DISABLE) {
            mc_ctrl_enable(mc_node->device, TRUE);
        }
    }

    mc_fdb_flush(mc_node);

    mc_node->mode = mode;
    
    return ret;
}

mc_fwd_engine_t mc_node_get_fwd_engine(const mc_node_t *mc_node)
{    
    return mc_node->fwd_engine;
}


cs_status mc_node_set_fwd_engine(mc_node_t *mc_node, mc_fwd_engine_t engine)
{   
    if(mc_node->fwd_engine != engine) {
        mc_node->fwd_engine = engine;
        mc_fdb_flush(mc_node);
    }
    
    return CS_E_OK;
}

/* 
* get & set vlan learn mode 
* device level 
*/
mc_vlan_learn_t mc_node_get_vlan_lrn(const mc_node_t *mc_node)
{
    return mc_node->vlan_lrn_mode;
}

cs_status mc_node_set_vlan_lrn(mc_node_t *mc_node, mc_vlan_learn_t lrn_mode)
{    
    if (mc_node->vlan_lrn_mode != lrn_mode) {
        mc_node->vlan_lrn_mode = lrn_mode;
        mc_fdb_flush(mc_node);
    }
    
    return CS_E_OK;
}


/* 
* get & set up port configuration 
* device level 
*/
cs_status mc_node_get_up_port(const mc_node_t *mc_node, mc_up_port_cfg_t *up_cfg)
{
    up_cfg->up_port = mc_node->up_port->portid;
    up_cfg->up_port_lrn = mc_node->up_port_learn;
    
    return CS_E_OK;
}

cs_status mc_node_set_up_port(mc_node_t *mc_node, const mc_up_port_cfg_t *up_cfg)
{
    mc_port_t   *port = NULL;
    
    port = mc_port_find(mc_node, up_cfg->up_port);
    if (NULL == port) {
        return CS_E_NOT_FOUND;
    }

    mc_node->up_port = port;
    mc_node->up_port_learn = up_cfg->up_port_lrn;

    mc_fdb_flush(mc_node);

    return CS_E_OK;
}

/* 
* get & set max group number 
* device level 
*/
cs_uint16 mc_node_get_max_grp(const mc_node_t *mc_node)
{    
    return mc_node->max_grp_num;
}

cs_status mc_node_set_max_grp(mc_node_t *mc_node, cs_uint16 max_grp)
{
    if(max_grp > mc_node->grp_cap) {
        return CS_E_ERROR;
    }

    if(mc_node->max_grp_num != max_grp) {
        mc_node->max_grp_num = max_grp;
        if(mc_node->cur_grp_num > max_grp) {
        mc_fdb_flush(mc_node);
        }
    }

    return CS_E_OK;
}

cs_uint16 mc_node_get_grp_num(const mc_node_t *mc_node)
{
    return mc_node->cur_grp_num;
}


/* 
* IGMP protocol related set&get functions 
*/

cs_uint32 mc_node_get_gmi(const mc_node_t *mc_node)
{
    return mc_node->member_interval;
}

cs_uint32 mc_node_set_gmi(mc_node_t *mc_node, cs_uint32 gmi)
{
    if(mc_node->member_interval != gmi) {
        mc_node->member_interval = gmi;
        mc_fdb_flush(mc_node);
    }

    return CS_E_OK;
}

/* 
* get & set robustness number 
*/
cs_uint32 mc_node_get_robustness(const mc_node_t *mc_node)
{
    return mc_node->robustness;
}

cs_status mc_node_set_robustness(mc_node_t *mc_node, cs_uint32 robustness)
{
    if (mc_node->robustness != robustness) {
        mc_node->robustness = robustness;
        mc_node->member_interval =
        mc_node->robustness * mc_node->query_interval + mc_node->query_rsp_interval;
        mc_fdb_flush(mc_node);
    }
    
    return CS_E_OK;
    
}


/* 
* get & set query interval 
*/
cs_uint32 mc_node_get_query_interval(const mc_node_t *mc_node)
{    
    return mc_node->query_interval;
}

cs_status mc_node_set_query_interval(mc_node_t *mc_node, cs_uint32 interval)
{
    if (mc_node->query_interval != interval) {
        mc_node->query_interval = interval;
        mc_node->member_interval =
        mc_node->robustness * mc_node->query_interval + mc_node->query_rsp_interval;
        mc_fdb_flush(mc_node);
    }
    
    return CS_E_OK;
    
}


/* 
* get & set query response interval 
*/
cs_uint32 mc_node_get_query_rsp_interval(const mc_node_t *mc_node)
{    
    return mc_node->query_rsp_interval;
}

cs_status mc_node_set_query_rsp_interval(mc_node_t *mc_node, cs_uint32 interval)
{
    if (mc_node->query_rsp_interval != interval) {
        mc_node->query_rsp_interval = interval;
        mc_node->member_interval =
        mc_node->robustness * mc_node->query_interval + mc_node->query_rsp_interval;
        mc_fdb_flush(mc_node);
    }
    
    return CS_E_OK;
    
}


/* 
* get & set last member query interval 
*/
cs_uint32 mc_node_get_last_member_query(const mc_node_t *mc_node)
{    
    return mc_node->last_member_query_interval;
}

cs_status mc_node_set_last_member_query(mc_node_t *mc_node, cs_uint32 interval)
{
    if(mc_node->last_member_query_interval != interval) {
        mc_node->last_member_query_interval = interval;
        mc_fdb_flush(mc_node);
    }
    
    return CS_E_OK;
    
}


/* 
* get & set last member query count 
*/
cs_uint32 mc_node_get_last_member_count(const mc_node_t *mc_node)
{    
    return mc_node->last_member_query_count;
    
}

cs_status mc_node_set_last_member_count(mc_node_t *mc_node, cs_uint32 count)
{
    if(mc_node->last_member_query_count != count) {
        mc_node->last_member_query_count = count;
        mc_fdb_flush(mc_node);
    }
    
    return CS_E_OK;
    
}


/* get & set port configuration */
cs_status mc_node_set_port_version(
                                                    mc_node_t *mc_node, 
                                                    cs_port_id_t portid, 
                                                    cs_uint32 version)
{
    mc_port_t     *port = NULL;

    port = mc_port_find(mc_node, portid);
    if (NULL == port) {
        return CS_E_NOT_FOUND;
    }
    
    if (port->igmp_version_level != version) {
        cs_uint32 mld_ver;
        switch(version)
        {
            case IGMP_V1:
                mld_ver = MC_VER_UNDEF;
                break;
            case IGMP_V2:
                mld_ver = MLD_V1;
                break;
            case IGMP_V3:
                mld_ver = MLD_V2;
                break;
            default:
                return CS_E_ERROR;
        }

        port->igmp_version_level = version;
        port->mld_version_level = mld_ver;
        /* TODO : flush port fdb ? flush all fdb ? */
        mc_fdb_flush(mc_node);
    }
    
    return CS_E_OK;
}


/* 
* get & set leave mode
*/
mc_leave_mode_t mc_node_get_leave_mode(mc_node_t *mc_node, cs_port_id_t portid)
{    
    mc_port_t     *port = NULL;

    port = mc_port_find(mc_node, portid);
    if (NULL == port) {
        return PORT_UNDEFINED_LEAVE_MODE;
    }
    
    return port->leave_mode;
    
}

cs_status mc_node_set_leave_mode(mc_node_t *mc_node,cs_port_id_t portid, mc_leave_mode_t mode)
{
    mc_port_t     *port = NULL;

    port = mc_port_find(mc_node, portid);
    if (NULL == port) {
        return CS_E_NOT_FOUND;
    }

    if(port->leave_mode != mode) {
        port->leave_mode = mode;
    }
    
    return CS_E_OK;
    
}



/* 
* get & set max group number per port
*/
cs_uint16 mc_node_get_port_max_grp(mc_node_t *mc_node, cs_port_id_t portid)
{   
    mc_port_t     *port = NULL;

    port = mc_port_find(mc_node, portid);
    if (NULL == port) {
        return 0;
    }
    
    return port->max_grp;
}


cs_status mc_node_set_port_max_grp(mc_node_t *mc_node, cs_port_id_t portid, cs_uint16 max_grp)
{
    mc_port_t     *port = NULL;

    if(max_grp > mc_node->port_grp_cap) {
        return CS_E_PARAM;
    }

    port = mc_port_find(mc_node, portid);
    if (NULL == port) {
        return CS_E_NOT_FOUND;
    }
    
    if(port->max_grp != max_grp) {
        port->max_grp = max_grp;
        if(port->grp_count > max_grp) {
        mc_port_fdb_flush(port);
        }
    }
    
    return CS_E_OK;
    
}

cs_uint32 mc_node_get_port_grp_num(mc_node_t *mc_node, cs_port_id_t portid)
{   
    mc_port_t     *port = NULL;

    port = mc_port_find(mc_node, portid);
    if (NULL == port) {
        return 0;
    }
    
    return port->grp_count;
}

cs_status mc_node_set_port_ingress_filter(
    mc_node_t *node, 
    mc_port_ingress_filter_routine_t filter)
{
    node->ing_filter = filter;
    
    return CS_E_OK;
}

cs_status mc_node_set_port_egress_filter(
    mc_node_t *node, 
    mc_port_egress_filter_routine_t filter)
{
    node->eg_filter = filter;
    
    return CS_E_OK;
}

