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

DEFINITIONS:  "DEVICE" means the Cortina Systems?Daytona SDK product.
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
/*
 * $Id: sdl_mpcp_cmn.c,v 1.1.2.2.2.15 2011/08/11 07:20:09 duchen Exp $
 */

#include "cs_types.h"
#include "aal_mpcp.h"
#include "aal_pon.h"
#include "aal_sys.h"
#include "sdl_event_cmn.h"
#include "sdl_init.h"
#include "sdl_int.h"
#include "startup_cfg.h"
#include "sdl.h"

void __dereg_cause_show(void)
{
    cs_aal_mpcp_dereg_case_t dereg_code;
    cs_boolean reg;
    aal_mpcp_reg_status_get(&reg);
    if(reg) return;

    aal_mpcp_dereg_case_get(&dereg_code);

    cs_printf("\nONU DEREGISTERED by ");

    switch (dereg_code)
    {
        case AAL_MPCP_DEREG_UNKNOWN:
            cs_printf("UNKNOWN CAUSE\n");
            break;
        case AAL_MPCP_DEREG_TIMEDRFT:
            cs_printf("TIMESTAMP DRIFT\n");
            break;
        case AAL_MPCP_DEREG_OLT:
            cs_printf("OLT\n");
            break;
        case AAL_MPCP_DEREG_LOCAL:
            cs_printf("LOCAL\n");
            break;
        case AAL_MPCP_DEREG_REREG:
            cs_printf("REGISTER FRAME WITH REREG FLAG\n");
            break;
        case AAL_MPCP_DEREG_TIMEOUT:
            cs_printf("MPCP TIMER EXPIRATION\n");
            break;
        case AAL_MPCP_DEREG_GATE:
            cs_printf("GATE TIMER EXPIRATION\n");
            break;
        case AAL_MPCP_DEREG_SW:
            cs_printf("SW DISABLE\n");
            break;
    }
}

void __reg_toogle(void *data)
{
#if 0
    cs_aal_pon_mac_msk_t        pon_mac_mask;
    cs_aal_pon_mac_cfg_t        pon_mac_cfg;
    cs_aal_module_reset_mask_t  reset_mask;
    cs_aal_pon_int_msk_t        pon_int_mask;
    cs_boolean                  pon_rx_en;
    cs_boolean                  pon_tx_en;
#endif
    cs_boolean                  reg;
    //cs_uint32 tx_reg_req_lo, tx_reg_req_hi;
    aal_mpcp_reg_status_get(&reg);
    if(!reg){
#if 0
#if 1
        /*start: Bug 27119 workaround solution for deregister sometimes when reseting ONU*/
        /* save pon mac configuration */
        aal_pon_mac_cfg_get(&pon_mac_cfg);
        pon_rx_en = pon_mac_cfg.rx_en;
        pon_tx_en = pon_mac_cfg.tx_en;

        /* Disable Tx/Rx on PON   */
        pon_mac_cfg.rx_en = 0;
        pon_mac_cfg.tx_en = 0;
        pon_mac_mask.u32 = 0;
        pon_mac_mask.s.rx_en = 1;
        pon_mac_mask.s.tx_en = 1;
        aal_pon_mac_cfg_set(pon_mac_mask, &pon_mac_cfg);

        /* Delay to make ONU de-registered */
        cs_thread_delay(1000);

        /* reset pon */
        reset_mask.u32 = 0;
        reset_mask.s.reset_pon = 1;
        aal_sys_reset_ctrl_set(reset_mask,0);
        pon_int_mask.u32 = 0xffffffff;
        aal_pon_int_clr(pon_int_mask);
        hal_delay_us(1000);
        aal_sys_reset_ctrl_set(reset_mask,1);

        /* restore PON configuration  */
        pon_mac_cfg.rx_en = pon_rx_en;
        pon_mac_cfg.tx_en = pon_tx_en;;
        pon_mac_mask.u32 = 0;
        pon_mac_mask.s.rx_en = 1;
        pon_mac_mask.s.tx_en = 1;
        aal_pon_mac_cfg_set(pon_mac_mask, &pon_mac_cfg);
        /*end: workaround solution for deregister sometimes when reseting ONU*/
#else
        aal_pon_mib_get(0, MIB_ID_PON_MPCP_COUNTER_MPCP_TX_REQ_REG, &tx_reg_req_lo, &tx_reg_req_hi);
        if(tx_reg_req_lo > 5) iros_system_reset(RESET_TYPE_FORCE);
#endif
#endif
        aal_mpcp_reg_toggle();
    }
}

void __int_discovery_handler(void)
{
    cs_aal_pon_int_msk_t mask;
    mask.u32 = 0;
    mask.s.discovery_frm = 1;
    aal_pon_int_disable(mask);  // Disable discovery frame interrupt

    aal_pon_clk_switch(AAL_PON_CLK_RECOVERY); // switch to recovery clock
    aal_pon_clk_switch_mode_set(AAL_PON_CLK_SWITCH_HW); // Let HW switch clock next link down
}

void __int_reg_change_handler(void)
{
    static sdl_event_reg_t reg_tvent;
    cs_boolean reg;
    cs_uint16 llid;
    aal_mpcp_reg_status_get(&reg);

    if(reg) // registered
    {
        aal_mpcp_llid_get(&llid);
        reg_tvent.llid = llid;
        reg_tvent.reg = 1;
        cs_printf("ONU(LLID=%u) REGISTERED\n", llid);
    }
    else // deregistered
    {
        reg_tvent.reg = 0;
        __dereg_cause_show();
    }

    sdl_event_send(EPON_EVENT_REG_CHANGE, sizeof(sdl_event_reg_t), &reg_tvent);
}

cs_status epon_request_onu_mpcp_report_status_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                enable
)
{
    return aal_mpcp_grant_permit_set(enable);
}

cs_status epon_request_onu_mpcp_report_status_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_OUT cs_boolean               *enable
)
{
    if(NULL==enable)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    return aal_mpcp_grant_permit_get(enable);
}

cs_status epon_request_onu_mpcp_enable_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                enable
)
{
    return aal_mpcp_enable_set(enable);
}

cs_status epon_request_onu_mpcp_enable_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_OUT cs_boolean               *enable
)
{
    if(NULL==enable)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    return aal_mpcp_enable_get(enable);
}

cs_status epon_request_onu_mpcp_gate_timer_threshold_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_uint16                 threshold
)
{
    cs_aal_mpcp_timer_msk_t mask;
    cs_aal_mpcp_timer_cfg_t cfg;

    mask.u32 = 0;
    mask.s.gatetimer_threshold = 1;
    cfg.gatetimer_threshold = threshold;

    return aal_mpcp_timer_cfg_set(mask, &cfg);
}

cs_status epon_request_onu_mpcp_gate_timer_threshold_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_OUT cs_uint16                *threshold
)
{
    cs_aal_mpcp_timer_cfg_t cfg;

    if(NULL==threshold)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    aal_mpcp_timer_cfg_get(&cfg);
    *threshold = cfg.gatetimer_threshold;

    return CS_E_OK;
}

cs_status epon_request_onu_mpcp_timer_threshold_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_uint16                 threshold
)
{
    cs_aal_mpcp_timer_msk_t mask;
    cs_aal_mpcp_timer_cfg_t cfg;

    mask.u32 = 0;
    mask.s.mpcptimer_threshold = 1;
    cfg.mpcptimer_threshold = threshold;

    return aal_mpcp_timer_cfg_set(mask, &cfg);
}

cs_status epon_request_onu_mpcp_timer_threshold_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_OUT cs_uint16                *threshold
)
{
    cs_aal_mpcp_timer_cfg_t cfg;

    if(NULL==threshold)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    aal_mpcp_timer_cfg_get(&cfg);
    *threshold = cfg.mpcptimer_threshold;

    return CS_E_OK;
}

cs_status epon_request_onu_mpcp_reg_status_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_OUT cs_boolean               *enable
)
{
    if(NULL==enable)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    return aal_mpcp_reg_status_get(enable);
}

cs_status epon_request_onu_mpcp_offline_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                offline
)
{
    return aal_mpcp_offline_set(offline);
}

cs_status epon_request_onu_mpcp_offline_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_OUT cs_boolean               *offline
)
{
    if(NULL==offline)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return  CS_E_PARAM;
    }

    aal_mpcp_offline_get(offline);

    return CS_E_OK;
}

cs_status epon_request_onu_mpcp_llid_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_OUT cs_uint16                *llid
)
{
    if(NULL==llid)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return  CS_E_PARAM;
    }

    aal_mpcp_llid_get(llid);

    return CS_E_OK;
}

cs_status sdl_mpcp_cmn_init(sdl_init_cfg_t *cfg)
{
    cs_status rt = CS_E_OK;
    cs_callback_context_t context;
    cs_aal_mpcp_misc_cfg_t misc_cfg;
    cs_aal_mpcp_misc_msk_t misc_msk;
    cs_uint8 gate_timeout = 0;

    if(NULL == cfg)
    {
        cs_halt("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
    }

    // set pending grant
    misc_cfg.pending_grant= cfg->pending_grant;
    misc_msk.u32 = 0;
    misc_msk.s.pending_grant = 1;
    aal_mpcp_misc_cfg_set(misc_msk, &misc_cfg);

    // get gate timeout from startup config and set to ONU
    rt = startup_config_read(CFG_ID_MPCP_TIMEOUT, 1, &gate_timeout);
    if(rt != CS_E_OK)
    {
        cs_halt("startup_config_read return %u. %s, line: %d\n", rt, __FILE__, __LINE__);
    }
    rt = epon_request_onu_mpcp_gate_timer_threshold_set(context, 0, CS_PON_PORT_ID, gate_timeout*10);
    if(rt != CS_E_OK)
    {
        cs_halt("epon_request_onu_mpcp_gate_timer_threshold_set return %u. %s, line: %d\n", rt, __FILE__, __LINE__);
    }
    cfg->gate_timeout= gate_timeout;

    rt = cs_circle_timer_add(1000, __reg_toogle, NULL);
    if(rt == 0) //CS_INVALID_TIMER
    {
        cs_halt("cs_circle_timer_add FAILED. %s, line: %d\n", __FILE__, __LINE__);
    }

    sdl_int_handler_reg(SDL_INT_REG_CHANGE, __int_reg_change_handler);

    return rt;
}

