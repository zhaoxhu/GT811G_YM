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
#include "oam_mux_par.h"
#include "oam_control.h"


static oam_core_handler_t g_oam_core_hdr;

void oam_control_handler_init(
        oam_handler_t link_hdr, 
        oam_handler_t lost_hdr,
        oam_build_oampdu_t info_hdr)
{
    g_oam_core_hdr.oam_link_hdr = link_hdr; 
    g_oam_core_hdr.oam_lost_hdr = lost_hdr; 
    g_oam_core_hdr.oam_build_info_hdr = info_hdr;
}

void oam_start_pdu_timer(oam_if_t *intf)
{
    intf->pdu_timer = OAM_PDU_TIMER;
    intf->pdu_timer_done = OAM_FALSE;
}

void oam_start_local_lost_link_timer(oam_if_t *intf)
{
    intf->local_lost_link_timer = OAM_LINK_LOST_TIMER; 
    intf->local_lost_link_timer_done = OAM_FALSE;
}


void oam_req_oampdu(oam_if_t *intf, oam_build_oampdu_t func, void *parm)
{
    if (intf->local_dying_gasp ||
            (intf->local_link_status == OAM_LINK_STATUS_FAIL) ||
            intf->local_critical_event) {
        intf->pdu_req = OAM_PDU_REQ_CRITICAL;
    }
    else {
        intf->pdu_req = OAM_PDU_REQ_NORMAL;
    }

    if ((intf->local_pdu == OAM_LOCAL_PDU_INFO) ||
            (intf->local_pdu == OAM_LOCAL_PDU_LF_INFO))
    {
        if (func != g_oam_core_hdr.oam_build_info_hdr)
        {
            OAM_LOG("bad param port 0x%x (0x%x)\n",
                    intf->port, (cs_uint32) intf->pdu_func);
        }
    }


    if (intf->pdu_func) {
        OAM_LOG("None null pdu_func port 0x%x (0x%x)\n",
                intf->port, (cs_uint32) intf->pdu_func);
        return;
    }
    intf->pdu_func = func;
    intf->pdu_parm = parm;

    oam_sm_run(intf);
}

void oam_req_oam_ctl(oam_if_t *intf,  oam_parm_req_oam_ctl_t *parm)
{
    oam_parm_mux_par_ctl_t mux_par_parm;
    cs_uint8 run = OAM_FALSE;

    mux_par_parm.mask = 0;
    if ((parm->mask & OAM_CTL_REQ_MASK_LOCAL_UNIDIR) &&
            (intf->local_unidir != parm->local_unidir)) {
        intf->local_unidir = parm->local_unidir;
        mux_par_parm.mask |= OAM_MUX_PAR_CTL_MASK_UNIDIR;
        mux_par_parm.local_unidir = parm->local_unidir;
    }
    if ((parm->mask & OAM_CTL_REQ_MASK_LOCAL_LINK_STATUS) &&
            (intf->local_link_status != parm->local_link_status)) {
        intf->local_link_status = parm->local_link_status;
        mux_par_parm.mask |= OAM_MUX_PAR_CTL_MASK_LINK_STATUS;
        mux_par_parm.local_link_status = parm->local_link_status;
        run = OAM_TRUE;
    }
    if ((parm->mask & OAM_CTL_REQ_MASK_LOCAL_DYING_GASP) &&
            (intf->local_dying_gasp != parm->local_dying_gasp)) {
        intf->local_dying_gasp = parm->local_dying_gasp;
    }
    if ((parm->mask & OAM_CTL_REQ_MASK_LOCAL_CRITICAL_EVENT) &&
            (intf->local_critical_event != parm->local_critical_event)) {
        intf->local_critical_event = parm->local_critical_event;
    }
    if ((parm->mask & OAM_CTL_REQ_MASK_LOCAL_SATISFIED) &&
            (intf->local_satisfied != parm->local_satisfied)) {
        intf->local_satisfied = parm->local_satisfied;
        run = OAM_TRUE;
    }
    if ((parm->mask & OAM_CTL_REQ_MASK_REMOTE_STATE_VALID) &&
            (intf->remote_state_valid != parm->remote_state_valid)) {
        intf->remote_state_valid = parm->remote_state_valid;
        run = OAM_TRUE;
    }
    if ((parm->mask & OAM_CTL_REQ_MASK_REMOTE_STABLE) &&
            (intf->remote_stable != parm->remote_stable)) {
        intf->remote_stable = parm->remote_stable;
        run = OAM_TRUE;
        if(parm->remote_stable)
            intf->reply = OAM_TRUE;
    }
    if ((parm->mask & OAM_CTL_REQ_MASK_LOCAL_MUX_ACTION) &&
            (intf->local_mux_action != parm->local_mux_action)) {
        intf->info_tlv_changed = 1;
        intf->local_mux_action = parm->local_mux_action;
        mux_par_parm.mask |= OAM_MUX_PAR_CTL_MASK_MUX_ACTION;
        mux_par_parm.local_mux_action = parm->local_mux_action;
    }
    if ((parm->mask & OAM_CTL_REQ_MASK_LOCAL_PAR_ACTION) &&
            (intf->local_par_action != parm->local_par_action))
    {
        intf->info_tlv_changed = 1;
        intf->local_par_action = parm->local_par_action;
        mux_par_parm.mask |= OAM_MUX_PAR_CTL_MASK_PAR_ACTION;
        mux_par_parm.local_par_action = parm->local_par_action;
    }

    if (mux_par_parm.mask) {
        oam_mux_par_ctl_req(intf, &mux_par_parm);
    }
    if (run) {
        oam_sm_run(intf);
    }

    if (((parm->mask & OAM_CTL_REQ_MASK_LOCAL_LINK_STATUS) &&
                (intf->local_link_status == OAM_LINK_STATUS_FAIL)) ||
            ((intf->local_pdu == OAM_LOCAL_PDU_ANY) &&
             (((parm->mask & OAM_CTL_REQ_MASK_LOCAL_DYING_GASP) &&
               parm->local_dying_gasp) ||
              ((parm->mask & OAM_CTL_REQ_MASK_LOCAL_CRITICAL_EVENT) &&
               parm->local_critical_event)))) {
        oam_req_oampdu(intf, g_oam_core_hdr.oam_build_info_hdr,
                NULL);
    }
}

/* ---> mux interface */

static void oam_tx_by_rule(oam_if_t *intf)
{
    cs_uint32 len;
    cs_uint8  pdu[OAM_MAX_OAMPDU_SIZE];

    if (intf->pdu_func == NULL) {
        OAM_LOG("Null pdu_func\n");
        return;
    }

    if (intf->local_pdu == OAM_LOCAL_PDU_RX_INFO) {
        OAM_LOG("Not tx due to RX_INFO\n");
        return;
    }
    if ((intf->local_pdu == OAM_LOCAL_PDU_INFO) ||
            (intf->local_pdu == OAM_LOCAL_PDU_LF_INFO)) {
        if (intf->pdu_func != g_oam_core_hdr.oam_build_info_hdr) {
            OAM_LOG("OAM PDU %d %d %d port 0x%x (0x%x) dropped\n",
                    intf->local_pdu, intf->local_satisfied,
                    intf->remote_stable, intf->port,
                    (cs_uint32) intf->pdu_func);
            intf->pdu_func = NULL;
            intf->pdu_parm = NULL;
            return;
        }
    }

    memset(pdu, 0, OAM_MAX_OAMPDU_SIZE);
    len = intf->pdu_func(intf, intf->pdu_parm, pdu);
    if (len > 0) {
        oam_req_oami(intf, pdu, len);
    }

    intf->pdu_func = NULL;
}

/* ---> parser interface */

cs_uint8 oam_rx_rule_passed(oam_if_t *intf, oam_pdu_hdr_t *pdu)
{
    if(intf->oam_src == OAM_SOURCE_SC_OOB)
        return OAM_TRUE;
    if ((intf->local_pdu == OAM_LOCAL_PDU_ANY) ||
            (pdu->code == OAM_PDU_CODE_INFO)) {
        return OAM_TRUE;
    }
    else {
        OAM_LOG_MAJ( "OAM Rx rule not passed (%d %d)\n", intf->local_pdu, pdu->code);
        return OAM_FALSE;
    }
}

/* ---> discovery state machine */

static void oam_sm_disc_into_fault(oam_if_t *intf)
{

    OAM_LOG_INFO( "OAM with OLT: into FAULT\n");
    if(g_oam_core_hdr.oam_lost_hdr != NULL){
        g_oam_core_hdr.oam_lost_hdr(); 
    }

    if ((OAM_LINK_STATUS_OK != intf->local_link_status) &&
            (OAM_LINK_STATUS_FAIL != intf->local_link_status)) {
        OAM_LOG_MAJ( "Invalid local link status %d\n",
                intf->local_link_status);
        /* intf->local_link_status = OAM_LINK_STATUS_FAIL; */
    }
    intf->local_satisfied = OAM_FALSE;
    intf->remote_stable = OAM_FALSE;
    intf->remote_state_valid = OAM_FALSE;

    /* my hack */
    oam_start_local_lost_link_timer(intf);

    intf->state_disc = OAM_SM_DISC_STATE_FAULT;
    intf->local_pdu = (intf->local_link_status == OAM_LINK_STATUS_FAIL) ?
        OAM_LOCAL_PDU_LF_INFO : OAM_LOCAL_PDU_RX_INFO;
    intf->local_stable = OAM_FALSE;

    intf->seq_num_next_tx = 0;
    intf->seq_num_last_rx = 0xffff;
    intf->revision_next_tx = 0;
    intf->revision_last_rx = 0xffff;
    intf->info_tlv_changed = 0;
}

static void oam_sm_disc_into_active_send_local(oam_if_t *intf)
{

    OAM_LOG_INFO("OAM with OLT: into ACTIVE_SEND_LOCAL\n");
    intf->state_disc = OAM_SM_DISC_STATE_ACTIVE_SEND_LOCAL;
    intf->local_pdu = OAM_LOCAL_PDU_INFO;
}

static void oam_sm_disc_into_passive_wait(oam_if_t *intf)
{

    OAM_LOG_INFO("OAM with OLT: into PASSIVE_WAIT\n");
    intf->state_disc = OAM_SM_DISC_STATE_PASSIVE_WAIT;
    intf->local_pdu = OAM_LOCAL_PDU_RX_INFO;
}

static void oam_sm_disc_into_send_local_remote(oam_if_t *intf)
{

    OAM_LOG_INFO("OAM with OLT: into SEND_LOCAL_REMOTE\n");

    intf->state_disc = OAM_SM_DISC_STATE_SEND_LOCAL_REMOTE;
    intf->local_pdu = OAM_LOCAL_PDU_INFO;
    intf->local_stable = OAM_FALSE;
}

static void oam_sm_disc_into_send_local_remote_ok(oam_if_t *intf)
{

    OAM_LOG_INFO("OAM with OLT: into SEND_LOCAL_REMOTE_OK\n");

    intf->state_disc = OAM_SM_DISC_STATE_SEND_LOCAL_REMOTE_OK;
    intf->local_pdu = OAM_LOCAL_PDU_INFO;
    intf->local_stable = OAM_TRUE;
}

static void oam_sm_disc_into_send_any(oam_if_t *intf)
{
    OAM_LOG_INFO( "OAM with OLT: into SEND_ANY\n");
    if(g_oam_core_hdr.oam_link_hdr != NULL){
        g_oam_core_hdr.oam_link_hdr(); 
    }

    intf->state_disc = OAM_SM_DISC_STATE_SEND_ANY;
    intf->local_pdu = OAM_LOCAL_PDU_ANY;
}

static void oam_sm_disc_transition(oam_if_t *intf)
{
    if (intf->BEGIN || intf->local_lost_link_timer_done ||
            (intf->local_link_status == OAM_LINK_STATUS_FAIL)) {
        OAM_LOG_INFO( "Into FAULT: BEGIN %d, timer_done %d, status %d\n",
                intf->BEGIN, intf->local_lost_link_timer_done,
                intf->local_link_status);
        oam_sm_disc_into_fault(intf);
        return;
    }

    switch (intf->state_disc) {
        case OAM_SM_DISC_STATE_FAULT:
            if (intf->local_link_status == OAM_LINK_STATUS_OK) {
                if (intf->local_oam_mode == OAM_MODE_ACTIVE) {
                    oam_sm_disc_into_active_send_local(intf);
                }
                else {
                    oam_sm_disc_into_passive_wait(intf);
                }
            }
            break;

        case OAM_SM_DISC_STATE_ACTIVE_SEND_LOCAL:
            if (intf->remote_state_valid) {
                oam_sm_disc_into_send_local_remote(intf);
            }
            break;

        case OAM_SM_DISC_STATE_PASSIVE_WAIT:
            if (intf->remote_state_valid) {
                oam_sm_disc_into_send_local_remote(intf);
            }
            break;

        case OAM_SM_DISC_STATE_SEND_LOCAL_REMOTE:
            if (intf->local_satisfied) {
                oam_sm_disc_into_send_local_remote_ok(intf);
            }
            break;

        case OAM_SM_DISC_STATE_SEND_LOCAL_REMOTE_OK:
            if (!intf->local_satisfied) {
                oam_sm_disc_into_send_local_remote(intf);
            }
            else if (intf->remote_stable) {
                oam_sm_disc_into_send_any(intf);
            }
            break;

        case OAM_SM_DISC_STATE_SEND_ANY:
            if (!intf->local_satisfied) {
                oam_sm_disc_into_send_local_remote(intf);
            }
            else if (!intf->remote_stable) {
                oam_sm_disc_into_send_local_remote_ok(intf);
            }
            break;

        default:
            OAM_LOG_MAJ("oam_sm_disc_transition: unknown state %d\n",
                    intf->state_disc);
            break;
    }
}

/* ---> transmit state machine */

static void oam_sm_tx_into_reset(oam_if_t *intf)
{

    intf->state_tx = OAM_SM_TX_STATE_RESET;
    oam_start_pdu_timer(intf);
    intf->pdu_cnt = OAM_PKT_COUNT_PER_SECOND;
}

static void oam_sm_tx_into_wait_for_tx(oam_if_t *intf)
{

    intf->state_tx = OAM_SM_TX_STATE_WAIT_FOR_TX;
}

static void oam_sm_tx_into_dec_pdu_cnt(oam_if_t *intf)
{

    intf->state_tx = OAM_SM_TX_STATE_DEC_PDU_CNT;
    if (intf->pdu_req == OAM_PDU_REQ_NORMAL) {
        if (intf->disable_oampdu_limit) {
            if (intf->pdu_cnt == OAM_PKT_COUNT_PER_SECOND) {
                intf->pdu_cnt--;
            }
        }
        else {
            if(intf->pdu_cnt != 0) {
                intf->pdu_cnt--;
            }
        }
    }
}

static void oam_sm_tx_into_tx_oampdu(oam_if_t *intf)
{

    intf->state_tx = OAM_SM_TX_STATE_TX_OAMPDU;
    oam_tx_by_rule(intf);
}

static void oam_sm_tx_transition(oam_if_t *intf)
{
    if (intf->BEGIN) {
        oam_sm_tx_into_reset(intf);
        return;
    }

    switch (intf->state_tx) {
        case OAM_SM_TX_STATE_RESET:
            oam_sm_tx_into_wait_for_tx(intf);
            break;

        case OAM_SM_TX_STATE_WAIT_FOR_TX:
            if (intf->pdu_timer_done &&
                    ((intf->local_pdu == OAM_LOCAL_PDU_RX_INFO) ||
                     (intf->pdu_cnt != OAM_PKT_COUNT_PER_SECOND) )) {
                oam_sm_tx_into_reset(intf);
            }
            else if (intf->pdu_timer_done &&
                    (intf->local_pdu != OAM_LOCAL_PDU_RX_INFO) &&
                    (intf->pdu_cnt == OAM_PKT_COUNT_PER_SECOND)) {
                intf->pdu_func = g_oam_core_hdr.oam_build_info_hdr;
                intf->pdu_parm = NULL;
                oam_sm_tx_into_tx_oampdu(intf);
            }
            else if (!intf->pdu_timer_done &&
                    (((intf->local_pdu != OAM_LOCAL_PDU_RX_INFO) &&
                      (intf->pdu_req == OAM_PDU_REQ_NORMAL) &&
                      (intf->pdu_cnt != 0)) ||
                     ((intf->local_pdu == OAM_LOCAL_PDU_ANY) &&
                      (intf->pdu_req == OAM_PDU_REQ_CRITICAL)))) {
                oam_sm_tx_into_dec_pdu_cnt(intf);
            }
#if 0
            else if (!intf->pdu_timer_done &&
                    ((intf->local_pdu != OAM_LOCAL_PDU_RX_INFO) &&
                     (intf->pdu_req == OAM_PDU_REQ_NORMAL) &&
                     (intf->d2_compatible)) ) {
                oam_sm_tx_into_dec_pdu_cnt(intf);
            }
#endif
            else if (intf->pdu_req == OAM_PDU_REQ_NORMAL) {
                intf->pdu_func = NULL;
                OAM_LOG_INFO("Clause 57: drop an OAMPDU, port 0x%x\n",
                        intf->port);
            }
            intf->pdu_req = OAM_PDU_REQ_NONE;
            break;

        case OAM_SM_TX_STATE_DEC_PDU_CNT:
            oam_sm_tx_into_tx_oampdu(intf);
            break;

        case OAM_SM_TX_STATE_TX_OAMPDU:
            if (intf->pdu_timer_done) {
                oam_sm_tx_into_reset(intf);
            }
            else {
                oam_sm_tx_into_wait_for_tx(intf);
            }
            break;

        default:
            OAM_LOG_MAJ("oam_sm_tx_transition: unknown state %d\n",
                    intf->state_tx);
            break;
    }
}

/* ---> run both state machines */

void oam_sm_run(oam_if_t *intf)
{
    oam_sm_disc_state_t prev_state_disc;
    oam_sm_tx_state_t prev_state_tx;

    do {
        prev_state_disc = intf->state_disc;
        prev_state_tx = intf->state_tx;

        oam_sm_disc_transition(intf);
        oam_sm_tx_transition(intf);

    } while ((prev_state_disc != intf->state_disc) ||
            (prev_state_tx != intf->state_tx));
}


cs_boolean oam_sm_discovery_completed(cs_llid_t llid)
{
    /*TODO:if need support mLLId, we need change here */
    oam_if_t * intf = oam_intf_find(oam_sdl_get_llid());

    if(intf != NULL 
            && OAM_SM_DISC_STATE_SEND_ANY == intf->state_disc) {
        return OAM_TRUE;
    }

    return OAM_FALSE;
}

void oam_enable(oam_if_t *intf)
{
    intf->local_oam_enable = OAM_ENABLE;
    intf->BEGIN = OAM_FALSE;
    oam_sm_run(intf);
}

void oam_disable(oam_if_t *intf)
{
    intf->local_oam_enable = OAM_DISABLE;
    intf->BEGIN = OAM_TRUE;
    oam_sm_run(intf);
}


void oam_discovery_send(oam_if_t *intf, oam_build_oampdu_t func, void *parm)
{
    if (intf->local_dying_gasp ||
            (intf->local_link_status == OAM_LINK_STATUS_FAIL) ||
            intf->local_critical_event) {
        intf->pdu_req = OAM_PDU_REQ_CRITICAL;
    }
    else {
        intf->pdu_req = OAM_PDU_REQ_NORMAL;
    }

    if ((intf->local_pdu == OAM_LOCAL_PDU_INFO) ||
            (intf->local_pdu == OAM_LOCAL_PDU_LF_INFO))
    {
        if (func != g_oam_core_hdr.oam_build_info_hdr)
        {
            OAM_LOG("bad param port 0x%x (0x%x)\n",
                    intf->port, (cs_uint32) intf->pdu_func);
        }
    }


    if (intf->pdu_func) {
        OAM_LOG("None null pdu_func port 0x%x (0x%x)\n",
                intf->port, (cs_uint32) intf->pdu_func);
        return;
    }
    intf->pdu_func = func;
    intf->pdu_parm = parm;

    oam_tx_by_rule(intf);
}


