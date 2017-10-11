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
#include "mc_core.h"
#include "mc_mgmt.h"
#include "mc_adapter.h"

#include "mc_devm.h"

mc_node_t *g_mc_dev = NULL;

cs_status mc_dev_init()
{    
#ifdef MC_MULTI_DEV_SUPPORT
    cs_uint8 dev_num = mc_dev_num_get();
    cs_uint8 dev_id;

    g_mc_dev = mc_malloc(dev_num * sizeof(mc_node_t));

    for(dev_id = 0; dev_id < dev_num; dev_id++) {
        if(mc_node_init(&g_mc_dev[dev_id], dev_id)) {
            mc_free(g_mc_dev);
            return CS_E_ERROR;
        }
    }

#else
    g_mc_dev = mc_node_create(0);
#endif

    if(g_mc_dev) {
        return CS_E_OK;
    }
    else {
        return CS_E_ERROR;
    }
}

cs_status mc_node_init(mc_node_t *mc_node, cs_dev_id_t device)
{
    cs_port_id_t   portid = 0;
    mc_port_t     *port = NULL;
    
    memset(mc_node, 0, sizeof(mc_node_t));
    mc_node->device = device;
    mc_node->mode = MC_DISABLE;

    mc_node->ipv6_aware = 1;
    mc_node->vlan_lrn_mode = MC_SVL;
    mc_node->grp_cap = mc_device_grp_cap(device);
    mc_node->port_grp_cap = mc_device_port_grp_cap(device);
    mc_node->max_grp_num = mc_node->grp_cap;
    mc_node->cur_grp_num = 0;
    mc_node->member_interval = GRP_MEMBERSHIP_INTERVAL;
    mc_node->query_rsp_interval = QUERY_RESPONSE_INTERVAL;
    mc_node->query_interval = QUERY_INTERVAL;
    mc_node->last_member_query_count = LAST_MEMBER_QUERY_COUNT;
    mc_node->last_member_query_interval = LAST_MEMBER_QUERY_INTERVAL;
    mc_node->robustness = ROBUSTNESS_VARIABLE ;
    mc_node->up_port_learn = 0;

    mc_node->port_num = mc_device_port_num_get(device);
   
   /*add port */
    for(portid = 0; portid < mc_node->port_num; portid++) {
        port = mc_port_create();
        if (port == NULL) {
            return CS_E_ERROR;
        }
        
        mc_port_init(portid, port);
        mc_port_add(port,mc_node); 
        port->max_grp = mc_node->port_grp_cap;
    }

    return CS_E_OK;
}

mc_node_t *mc_node_create(cs_dev_id_t device)
{    
    mc_node_t   *mc_node = NULL;
    
    mc_node = (mc_node_t *)mc_malloc(sizeof(mc_node_t));
    if(mc_node == NULL) {
        MC_LOG(IROS_LOG_LEVEL_CRI,"%s,no memory\n",__func__);
        return NULL;
    }        

    if(mc_node_init(mc_node, device)) {
        mc_free(mc_node);
        return NULL;
    }
    
    return mc_node;

}


