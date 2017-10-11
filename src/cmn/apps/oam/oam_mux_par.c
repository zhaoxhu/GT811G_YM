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
#include "oam_sdl_if.h"
#include "oam_core.h"
#include "oam_supp.h"
#include "oam_mux_par.h"

/*
 * OAM send pkt flag:
 * true - will send the oam pkt to OLT.
 * false - will not send the oam pkt to OLT 
 */
static cs_boolean g_oam_send_flag = TRUE;

/*****************************************************************************/
/* $rtn_hdr_start  oam_set_send_flag                                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_set_send_flag(cs_boolean send_flag)
/*                                                                           */
/* INPUTS  : o send_flag - true or false to set the flag                     */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* set the oam send flag                                                     */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    g_oam_send_flag = send_flag;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_send                                                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_send(
        cs_llid_t llid,
        cs_port_id_t port, 
        cs_uint8 *buf, 
        cs_uint32 len)
/*                                                                           */
/* INPUTS  : o port - llid port                                              */
/*           o buf - oam packet                                              */
/*           o len - oam packet len                                          */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* send the packet to the olt                                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_if_t *intf = oam_intf_find(llid);
    if(intf == NULL){
        OAM_LOG("intf not found \n");
        return;
    }

    if(!g_oam_send_flag){
        return;
    }

    oam_dump_pkt("send OAM", port, buf, len);

    intf->frame_tx++;
    oam_sdl_send_pkt(port,buf,len);
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_mux_par_ctl_req                                       */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_mux_par_ctl_req(
        oam_if_t *intf, 
        oam_parm_mux_par_ctl_t *parm)
/*                                                                           */
/* INPUTS  : o intf - oam core interface for the llid                        */
/*           o parm - paramters for oam ctrl                                 */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* According to oam param, set it to SDL                                     */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 set_action = 0;
    oam_mux_action_t mux_action = intf->local_mux_action;
    oam_par_action_t par_action = intf->local_par_action;

    /* communicate with the data path */
    if (parm->mask & OAM_MUX_PAR_CTL_MASK_UNIDIR) {
        OAM_DEBUG("oam_mux_par_ctl_req: unidir %d\n",
                parm->local_unidir);
    }
    if (parm->mask & OAM_MUX_PAR_CTL_MASK_LINK_STATUS) {
        OAM_DEBUG("oam_mux_par_ctl_req: link_status %d\n",
                parm->local_link_status);
    }
    if (parm->mask & OAM_MUX_PAR_CTL_MASK_MUX_ACTION) {
        set_action = 1;
        mux_action = parm->local_mux_action;
    }
    if (parm->mask & OAM_MUX_PAR_CTL_MASK_PAR_ACTION) {
        set_action = 1;
        par_action = parm->local_par_action;
    }
    if (set_action) {
        OAM_DEBUG("set action at port 0x%08x: mux %d par %d\n",
                intf->port, mux_action, par_action);
        oam_sdl_port_lpbk_set(intf->port, mux_action, par_action);
    }
}


/*****************************************************************************/
/* $rtn_hdr_start  oam_req_oami                                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_req_oami(
        oam_if_t *intf, 
        cs_uint8 *pdu, 
        cs_uint32 len)
/*                                                                           */
/* INPUTS  : o intf - oam core interface for the llid                        */
/*           o pdu - oam packet                                              */
/*           o len - oam packet len                                          */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* send the packet to the olt                                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_send(intf->port,CS_PON_PORT_ID, pdu, len);
}
