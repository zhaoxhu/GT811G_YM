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
 * $Id: sdl_mpcp.c,v 1.1.2.1.2.6.2.4 2011/10/18 05:31:43 qxu Exp $
 */

#include "sdl_mpcp.h"
#include "sdl_init.h"
#include "sdl_int.h"
#include "aal_mpcp.h"
#include "aal_pon.h"
#include "osal_api_core.h"
#include "startup_cfg.h"

#define __REG_FLAG_NACK 4

#define __HOLDOVER_TIME_MIN 50
#define __HOLDOVER_TIME_MAX 4095

#define __HOLDOVER_STATE_DEACTIVATED    1       /* defined from OAM */
#define __HOLDOVER_STATE_ACTIVATED      2       /* defined from OAM */

extern cs_status sdl_mpcp_cmn_init(sdl_init_cfg_t *cfg);
extern sdl_init_cfg_t sdl_int_cfg;

cs_uint32 g_holdover_state;
cs_uint32 g_holdover_time;

cs_status epon_request_onu_mpcp_queue_threshold_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_uint8                  queue,
    CS_IN cs_uint16                 threshold,
    CS_IN cs_uint16                 max_threshold
)
{
    cs_status rt = CS_E_OK;

    rt = aal_mpcp_queue_thrshld_set(queue, threshold, max_threshold);
    if(rt!=CS_E_OK) goto err;

err:
    return rt;
}

cs_status epon_request_onu_mpcp_queue_threshold_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_uint8                  queue,
    CS_OUT cs_uint16                 *threshold,
    CS_OUT cs_uint16                 *max_threshold
)
{
    cs_status rt = CS_E_OK;
    cs_uint16 thre, max_thre;

    if(queue > AAL_MPCP_QUEUE_ID_7)
    {
        rt = CS_E_OK;
        goto err;
    }

    rt = aal_mpcp_queue_thrshld_get(queue, &thre, &max_thre);
    if(rt!=CS_E_OK) goto err;

    *threshold = thre;
    *max_threshold = max_thre;

err:
    return rt;
}

cs_status epon_request_onu_mpcp_timer_set
(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                dereg_en,
    CS_IN cs_uint16                 timeout
)
{
    cs_aal_mpcp_timer_cfg_t tmr_cfg;
    cs_aal_mpcp_timer_msk_t tmr_msk;

    tmr_cfg.mpcptimer_dereg_en = dereg_en;
    tmr_cfg.timedrift_dereg_en = dereg_en;
    tmr_cfg.gatetimer_threshold = timeout;

    tmr_msk.u32 = 0;
    tmr_msk.s.mpcptimer_dereg_en = 1;
    tmr_msk.s.timedrift_dereg_en = 1;
    tmr_msk.s.gatetimer_threshold = 1;

    aal_mpcp_timer_cfg_set(tmr_msk, &tmr_cfg);

    return CS_E_OK;
}

cs_status epon_request_onu_mpcp_timer_get
(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                *dereg_en,
    CS_IN cs_uint16                 *timeout
)
{
    cs_aal_mpcp_timer_cfg_t tmr_cfg;

    if( (NULL == dereg_en) || (NULL == timeout) )
    {
        return CS_E_PARAM;
    }

    aal_mpcp_timer_cfg_get(&tmr_cfg);

    *dereg_en = tmr_cfg.mpcptimer_dereg_en & tmr_cfg.timedrift_dereg_en;
    *timeout = tmr_cfg.gatetimer_threshold;

    return CS_E_OK;
}

void __disable_local_offline(void *data)
{
    aal_mpcp_offline_set(EPON_FALSE);
}

void __int_reg_frame_received_handler(void)
{
    cs_uint8 reg_flag;

    aal_mpcp_reg_flag_get(&reg_flag);

    if(__REG_FLAG_NACK == reg_flag)
    {
        aal_mpcp_offline_set(EPON_TRUE);
        cs_timer_add(sdl_int_cfg.keep_silence_time*1000, __disable_local_offline, NULL);
    }
}

cs_status sdl_mpcp_init(sdl_init_cfg_t *cfg)
{
    cs_aal_mpcp_report_cfg_t rpt_cfg;
    cs_aal_mpcp_report_msk_t rpt_msk;
    cs_status rt = CS_E_OK;
    cs_uint8 keep_silence_time;
    cs_aal_pon_llid_filter_cfg_t llid_filter_cfg;
    cs_aal_pon_llid_filter_msk_t llid_filter_msk;

    if(NULL == cfg)
    {
        cs_halt("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
    }

    rt = startup_config_read(CFG_ID_KEEP_SILENCE_TIME, 1, &keep_silence_time);
    if(rt != CS_E_OK)
    {
        cs_halt("startup_config_read return %u. %s, line: %d\n", rt, __FILE__, __LINE__);
    }
    cfg->keep_silence_time = keep_silence_time;

    rpt_cfg.le_mode_en = 1;
    rpt_cfg.le_mode_opt = AAL_MPCP_LE_MODE_RPT_OPT_2QSET;
    rpt_cfg.le_mode_bmp = 0xff;
    rpt_msk.u32 = 0;
    rpt_msk.s.le_mode_en = 1;
    rpt_msk.s.le_mode_opt = 1;
    rpt_msk.s.le_mode_bmp = 1;
    aal_mpcp_report_cfg_set(rpt_msk, &rpt_cfg);

    /* allow 0x7FFF LLID Discovery */
    llid_filter_cfg.drop_7fff = 0;
    llid_filter_msk.u32= 0;
    llid_filter_msk.s.drop_7fff = 1;
    aal_pon_llid_filter_cfg_set(llid_filter_msk, &llid_filter_cfg);

    sdl_int_handler_reg(SDL_INT_REG_FRAME_RECEIVED, __int_reg_frame_received_handler);

    return sdl_mpcp_cmn_init(cfg);
}

