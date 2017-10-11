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

DEFINITIONS:  "DEVICE" means the Cortina Systems? Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems? SDK software.

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

Copyright (c) 2011 by Cortina Systems Incorporated
****************************************************************************/

#include <stdio.h>
#include "oam_common.h"
#include "oam_plat_if.h"
#include "oam_core.h"

static oam_node_t g_oam_node;

/*****************************************************************************/
/* $rtn_hdr_start  oam_intf_find                                             */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
oam_if_t * oam_intf_find(cs_uint32 port)
/*                                                                           */
/* INPUTS  : o port - onu llid                                               */
/* OUTPUTS : llid interface info                                             */
/* DESCRIPTION:                                                              */
/* API to find the llid interface information                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint32 i;

    for (i=0;i<OAM_MAX_IF;i++) {
        if ((g_oam_node.intf[i].valid) 
                &&(g_oam_node.intf[i].port == port)) {
            return &g_oam_node.intf[i];
        }
    }

#ifndef HAVE_OOB_MGMT
    return NULL;
#else
    return &g_oam_node.intf[OAM_MAX_IF];
#endif
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_intf_get                                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
oam_if_t * oam_intf_get(cs_uint32 index)
/*                                                                           */
/* INPUTS  : o index - index of the intf                                     */
/* OUTPUTS : llid interface info                                             */
/* DESCRIPTION:                                                              */
/* API to get the llid interface information                                 */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    OAM_ASSERT_RET(index < OAM_MAX_IF,NULL);
    return &g_oam_node.intf[index];
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_intf_alloc                                            */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
oam_if_t * oam_intf_alloc()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : alloc llid interface info                                       */
/* DESCRIPTION:                                                              */
/* API to alloc the llid interface information                               */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint32 i;
    for (i=0;i<OAM_MAX_IF;i++) {
        if (!g_oam_node.intf[i].valid) {
            g_oam_node.intf[i].valid = OAM_TRUE;
            g_oam_node.intf[i].pdu_func = NULL;
            return &g_oam_node.intf[i];
        }
    }

    return &g_oam_node.intf[0];
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_intf_free                                             */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_intf_free(oam_if_t *intf)
/*                                                                           */
/* INPUTS  : o intf - llid interface info                                    */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* API to free the llid interface information                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    intf->valid = 0;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_intf_init                                             */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_intf_init(oam_if_t *intf, 
        cs_port_id_t port, 
        oam_mode_t oam_mode)
/*                                                                           */
/* INPUTS  : o intf - llid interface info                                    */
/*           o port - llid                                                   */
/*           o oam_mode - oam mode                                           */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* API to init the llid interface information                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    memset(intf, 0, sizeof(oam_if_t));

    intf->valid = OAM_TRUE;
    intf->d2_compatible = oam_plat_d2_compatible_get();
    intf->port = port;
    intf->local_oampdu_config = OAM_MAX_OAMPDU_SIZE;
    intf->local_link_status = OAM_LINK_STATUS_OK;
    intf->local_mux_action = OAM_MUX_ACTION_FWD;
    intf->local_oam_mode = oam_mode;
    intf->local_par_action = OAM_PAR_ACTION_FWD;
    /* set to false for interop */
    if (intf->d2_compatible) {
        intf->local_unidir = OAM_FALSE;
    }
    else {
        intf->local_unidir = OAM_TRUE;
    }

    intf->pdu_req = OAM_PDU_REQ_NONE;
    intf->seq_num_last_rx = 0xffff;
    intf->revision_last_rx = 0xffff;
    intf->lb_state_self = OAM_LB_STATE_SELF_NORM;
    intf->lb_state_peer = OAM_LB_STATE_PEER_NORM;
    intf->remote_pdu_flags = 0;
    intf->frame_lost_seconds = 0;
    intf->frame_rx = 0;
    intf->frame_tx = 0;
    intf->reply = OAM_FALSE;

    oam_disable(intf);
}


/*****************************************************************************/
/* $rtn_hdr_start  oam_core_init                                             */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_core_init()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* API to init the all llid interface information to empty                   */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    memset(&g_oam_node, 0, sizeof(g_oam_node));
#ifdef HAVE_OOB_MGMT
    /*add a default node for oob, not belong to the std stack*/
    g_oam_node.intf[OAM_MAX_IF].valid = OAM_TRUE;;
#endif
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_intf_dump                                             */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_intf_dump(cs_llid_t llid)
/*                                                                           */
/* INPUTS  : o llid - llid                                                   */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* Dump llid interface information to to screen                              */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint32 i;
    oam_if_t *intf = NULL;

    oam_printf("------------\n");
    oam_printf("OAM STATUS\n");

    for (i=0;i<OAM_MAX_IF;i++)
    {
        intf = &g_oam_node.intf[i];
        if (!intf->valid
                ||(llid != 0 && intf->port != llid)
                ){
            continue;
        }

        oam_printf("*****port 0x%x*******\n", intf->port);
        oam_printf("---->local_oampdu_config %d\n", intf->local_oampdu_config);
        oam_printf("---->d2_compatible %d\n", intf->d2_compatible);
        oam_printf("---->local_dying_gasp %d\n", intf->local_dying_gasp);
        oam_printf("---->macaddr 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
                intf->macaddr[0],
                intf->macaddr[1],
                intf->macaddr[2],
                intf->macaddr[3],
                intf->macaddr[4],
                intf->macaddr[5]);
        oam_printf("---->macaddr_peer 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
                intf->macaddr_peer[0],
                intf->macaddr_peer[1],
                intf->macaddr_peer[2],
                intf->macaddr_peer[3],
                intf->macaddr_peer[4],
                intf->macaddr_peer[5]);
        oam_printf("---->local pdu 0x%x\n", intf->local_pdu);
        oam_printf("---->state_disc 0x%x\n", intf->state_disc);
        oam_printf("---->state_tx 0x%x\n", intf->state_tx);
        oam_printf("---->local_link_status 0x%x\n", intf->local_link_status);
        oam_printf("---->local_lost_link_timer_done 0x%x\n", intf->local_lost_link_timer_done);
        oam_printf("---->local_oam_enable 0x%x\n", intf->local_oam_enable);
        oam_printf("---->local_oam_mode 0x%x\n", intf->local_oam_mode);
        oam_printf("---->local_pdu 0x%x\n", intf->local_pdu);
        oam_printf("---->local_satisfied 0x%x\n", intf->local_satisfied);
        oam_printf("---->local_stable 0x%x\n", intf->local_stable);
        oam_printf("---->local_unidir 0x%x\n", intf->local_unidir);
        oam_printf("---->pdu_cnt 0x%x\n", intf->pdu_cnt);
        oam_printf("---->pdu_req 0x%x\n", intf->pdu_req);
        oam_printf("---->pdu_timer_done 0x%x\n", intf->pdu_timer_done);
        oam_printf("---->remote_stable 0x%x\n", intf->remote_stable);
        oam_printf("---->remote_state_valid 0x%x\n", intf->remote_state_valid);
        oam_printf("---->lb_state_self 0x%x\n", intf->lb_state_self);
        oam_printf("---->lb_state_peer 0x%x\n", intf->lb_state_peer);
        oam_printf("---->remote_flags 0x%x\n", intf->remote_flags);
        oam_printf("---->remote_oam_config 0x%x\n", intf->remote_oam_config);
        oam_printf("---->local_lost_link_timer 0x%x\n", intf->local_lost_link_timer);
        oam_printf("---->frame_rx 0x%x\n", intf->frame_rx);
        oam_printf("---->frame_tx 0x%x\n", intf->frame_tx);
        oam_printf("---->frame_lost 0x%x\n", intf->frame_lost);
        oam_printf("---->frame_lost_seconds 0x%x\n", intf->frame_lost_seconds);
    }
}

