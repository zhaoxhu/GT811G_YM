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
 
DEFINITIONS:  "DEVICE" means the Cortina Systems LynxD product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems software.
 
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
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING CS_OUT
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
 * $Id: aal_mpcp.c,v 1.1.2.15.6.5.2.7.2.5.2.2.2.1.2.8.2.4.2.20.2.8.2.1.2.2.20.1 2012/05/25 07:39:33 hli2 Exp $
 */

#include "aal_mpcp.h"
#include "cs_types.h"
#include "aal_util.h"
#include "aal.h"

/* macro define */

#define __ZEROED_QUEUE_MAP_MAX                            0x1f

#define __GATE_TIMER_THRESHOLD_MAX                        0xfff
#define __MPCP_TIMER_THRESHOLD_MAX                        0xfff
#define __REPORT_TIMER_THRESHOLD_MAX                      0xfff

#define __MIN_PROCESS_TIME_MAX                            0x3ff
#define __MAX_FUTURE_GRANT_TIME_MAX                       0x3ff

#define __RTT_COMP_RXFEC_AES_MASK                         0xff000000
#define __RTT_COMP_RXFEC_CHURN_MASK                       0xff0000
#define __RTT_COMP_NON_RXFEC_AES_MASK                     0xff00
#define __RTT_COMP_NON_RXFEC_CHURN_MASK                   0xff

cs_status aal_mpcp_timer_cfg_set(
    CS_IN cs_aal_mpcp_timer_msk_t msk,
    CS_IN cs_aal_mpcp_timer_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(msk.s.gatetimer_en)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_TIMERS_CFG, gate_timer_enable, (cfg->gatetimer_en!=0));
    }

    if(msk.s.mpcptimer_en)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_TIMERS_CFG, mpcp_timer_enable, (cfg->mpcptimer_en!=0));
    }

    if(msk.s.reporttimer_en)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_REPORT_TIMER_CFG, report_timer_enable, (cfg->reporttimer_en!=0));
    }

    if(msk.s.gatetimer_dereg_en)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_CONTROL, gate_timer_expiration_deregister, (cfg->gatetimer_dereg_en!=0));
    }

    if(msk.s.mpcptimer_dereg_en)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_CONTROL, disable_mpcp_timer_deregister, (cfg->mpcptimer_dereg_en==0));
    }

    if(msk.s.timedrift_dereg_en)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_CONTROL, timestamp_drift_deregister, (cfg->timedrift_dereg_en!=0));
    }

    if(msk.s.gatetimer_threshold)
    {
        if ( cfg->gatetimer_threshold > __GATE_TIMER_THRESHOLD_MAX ) 
        {
            AAL_MIN_LOG("gatetimer_threshold invalid.(%d) FILE: %s, LINE: %d\n", cfg->gatetimer_threshold, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_MPCP_TIMERS_CFG, gate_timer_threshold_value_select, cfg->gatetimer_threshold);
    }

    if(msk.s.mpcptimer_threshold)
    {
        if ( cfg->mpcptimer_threshold > __MPCP_TIMER_THRESHOLD_MAX ) 
        {
            AAL_MIN_LOG("mpcptimer_threshold invalid.(%d) FILE: %s, LINE: %d\n", cfg->mpcptimer_threshold, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_MPCP_TIMERS_CFG, mpcp_timer_threshold_value_select, cfg->mpcptimer_threshold);
    }

    if(msk.s.timedrift_threshold)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_CFG, guardThresholdONU, cfg->timedrift_threshold);
    }

    if(msk.s.reporttimer_threshold)
    {
        if ( cfg->reporttimer_threshold > __REPORT_TIMER_THRESHOLD_MAX ) 
        {
            AAL_MIN_LOG("reporttimer_threshold invalid.(%d) FILE: %s, LINE: %d\n", cfg->reporttimer_threshold, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_MPCP_REPORT_TIMER_CFG, report_timer_threshold_value_select, cfg->reporttimer_threshold);
    }

    return CS_E_OK;
}


cs_status aal_mpcp_timer_cfg_get(
    CS_OUT cs_aal_mpcp_timer_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(ONU_MAC_MPCP_TIMERS_CFG, gate_timer_enable, cfg->gatetimer_en);
    REG_FIELD_READ(ONU_MAC_MPCP_TIMERS_CFG, mpcp_timer_enable, cfg->mpcptimer_en);
    REG_FIELD_READ(ONU_MAC_MPCP_REPORT_TIMER_CFG, report_timer_enable, cfg->reporttimer_en);
    REG_FIELD_READ(ONU_MAC_MPCP_CONTROL, gate_timer_expiration_deregister, cfg->gatetimer_dereg_en);
    REG_FIELD_READ(ONU_MAC_MPCP_CONTROL, disable_mpcp_timer_deregister, cfg->mpcptimer_dereg_en);
    cfg->mpcptimer_dereg_en = !cfg->mpcptimer_dereg_en;
    REG_FIELD_READ(ONU_MAC_MPCP_CONTROL, timestamp_drift_deregister, cfg->timedrift_dereg_en);
    REG_FIELD_READ(ONU_MAC_MPCP_TIMERS_CFG, gate_timer_threshold_value_select, cfg->gatetimer_threshold);
    REG_FIELD_READ(ONU_MAC_MPCP_TIMERS_CFG, mpcp_timer_threshold_value_select, cfg->mpcptimer_threshold);
    REG_FIELD_READ(ONU_MAC_MPCP_CFG, guardThresholdONU, cfg->timedrift_threshold);
    REG_FIELD_READ(ONU_MAC_MPCP_REPORT_TIMER_CFG, report_timer_threshold_value_select, cfg->reporttimer_threshold);

    return CS_E_OK;
}


cs_status aal_mpcp_report_cfg_set(
    CS_IN cs_aal_mpcp_report_msk_t msk,
    CS_IN cs_aal_mpcp_report_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

// J-mode
    if(msk.s.t_threshold)
    {
        REG_FIELD_WRITE(BM_MPCP_QSET_2, rpt_thrshld, cfg->t_threshold);
    }

    if(msk.s.zeroq_map)
    {
        if( cfg->zeroq_map > __ZEROED_QUEUE_MAP_MAX )
        {
            AAL_MIN_LOG("zeroq_map invalid.(%d) FILE: %s, LINE: %d\n", cfg->zeroq_map, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(BM_LYNXD_MPCP_CFG, zeroed_qs_en, cfg->zeroq_map);
    }

    if(msk.s.zeroq_threshold)
    {
        REG_FIELD_WRITE(BM_MPCP_QSET_2, zqs_thrshld, cfg->zeroq_threshold);
    }

    if(msk.s.aggregation_mode_en)
    {
        REG_FIELD_WRITE(BM_LYNXD_MPCP_CFG, agg_qs_thrshld_en, (cfg->aggregation_mode_en!=0));
    }

    if(msk.s.report_format)
    {
        if(cfg->report_format>AAL_MPCP_RPT_FMT_2)
        {
            AAL_MIN_LOG("report_format invalid.(%d) FILE: %s, LINE: %d\n", cfg->report_format, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(BM_LYNXD_MPCP_CFG, rpt_format, cfg->report_format);
    }

    if(msk.s.rpt_order)
    {
        if(cfg->rpt_order>AAL_MPCP_RPT_ORDER_SEQ)
        {
            AAL_MIN_LOG("rpt_order invalid.(%d) FILE: %s, LINE: %d\n", cfg->rpt_order, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(BM_LYNXD_MPCP_CFG, rpt_order, cfg->rpt_order);
    }

    if(msk.s.rpt_number)
    {
        if(cfg->rpt_number>AAL_MPCP_RPT_NUM_2)
        {
            AAL_MIN_LOG("rpt_number invalid.(%d) FILE: %s, LINE: %d\n", cfg->rpt_number, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(BM_LYNXD_MPCP_CFG, rpt_number, cfg->rpt_number);
    }

    if(msk.s.rpt_bmp_qset1)
    {
        REG_FIELD_WRITE(BM_MPCP_RPT_BIT_MAP, q_set1, cfg->rpt_bmp_qset1);
    }

    if(msk.s.rpt_bmp_qset2)
    {
        REG_FIELD_WRITE(BM_MPCP_RPT_BIT_MAP, q_set2, cfg->rpt_bmp_qset2);
    }

    if(msk.s.schedule_fit_en)
    {
        REG_FIELD_WRITE(BM_LYNXD_MPCP_CFG, window_fit_en, (cfg->schedule_fit_en!=0));
        REG_FIELD_WRITE(BM_LYNXD_MPCP_CFG, fill_unrpt_pkt_en, (cfg->schedule_fit_en!=0));
    }

    if(msk.s.report_fit_en)
    {
        REG_FIELD_WRITE(BM_SCH_CFG, ds_dp_en, (cfg->report_fit_en==0));
    }

    if(msk.s.quick_rpt_en)
    {
        REG_FIELD_WRITE(BM_LYNXD_MPCP_CFG, force_quick_rpt_en, (cfg->quick_rpt_en!=0));
    }

    if(msk.s.ignore_rpt_en)
    {
        REG_FIELD_WRITE(BM_LYNXD_MPCP_CFG, ignor_rpt_en, (cfg->ignore_rpt_en!=0));
    }

    if(msk.s.single_window_en)
    {
        REG_FIELD_WRITE(BM_LYNXD_MPCP_CFG, single_window_en, (cfg->single_window_en!=0));
    }

// K-mode
    if(msk.s.cs_mode_en)
    {
        if (cfg->cs_mode_en!=0) // Enable K-mode should disable C-mode first
        {
            REG_FIELD_WRITE(BM_MPCP_CFG, rpt_en_rsvd, 0);        
        }
        REG_FIELD_WRITE(BM_MPCP_CFG, kt_rpt_mode_en, (cfg->cs_mode_en!=0));
    }

    if(msk.s.cs_mode_opt)
    {
        if (cfg->cs_mode_opt > AAL_MPCP_CS_MODE_RPT_OPT_Q7) 
        {
            AAL_MIN_LOG("cs_mode_opt invalid.(%d) FILE: %s, LINE: %d\n", cfg->cs_mode_opt, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(BM_MPCP_CFG, kt_rpt_option, cfg->cs_mode_opt);
    }

    if(msk.s.cs_mode_threshold)
    {
        REG_FIELD_WRITE(BM_MPCP_Q2_THRESHOLD_1_0, rpt_thrsld_cos0, cfg->cs_mode_threshold);
    }

// C-mode
    if(msk.s.le_mode_en)
    {
        if (cfg->le_mode_en!=0) // Enable C-mode should disable K-mode first
        {
            REG_FIELD_WRITE(BM_MPCP_CFG, kt_rpt_mode_en, 0);        
        }
        REG_FIELD_WRITE(BM_MPCP_CFG, rpt_en_rsvd, (cfg->le_mode_en!=0));
    }

    if(msk.s.le_mode_opt)
    {
        if (cfg->le_mode_opt > AAL_MPCP_LE_MODE_RPT_OPT_1QSET)
        {
            AAL_MIN_LOG("le_mode_opt invalid.(%d) FILE: %s, LINE: %d\n", cfg->le_mode_opt, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_MPCP_CFG, rpt_mode, cfg->le_mode_opt);
    }

    if(msk.s.le_mode_bmp)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_CFG, rpt_pri, cfg->le_mode_bmp);
    }

    return CS_E_OK;
}

cs_status aal_mpcp_report_cfg_get(
    CS_OUT cs_aal_mpcp_report_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

// J-mode
    REG_FIELD_READ(BM_MPCP_QSET_2, rpt_thrshld, cfg->t_threshold);
    REG_FIELD_READ(BM_LYNXD_MPCP_CFG, zeroed_qs_en, cfg->zeroq_map);
    REG_FIELD_READ(BM_MPCP_QSET_2, zqs_thrshld, cfg->zeroq_threshold);
    REG_FIELD_READ(BM_LYNXD_MPCP_CFG, agg_qs_thrshld_en, cfg->aggregation_mode_en);
    REG_FIELD_READ(BM_LYNXD_MPCP_CFG, rpt_format, cfg->report_format);
    REG_FIELD_READ(BM_LYNXD_MPCP_CFG, rpt_order, cfg->rpt_order);
    REG_FIELD_READ(BM_LYNXD_MPCP_CFG, rpt_number, cfg->rpt_number);
    REG_FIELD_READ(BM_MPCP_RPT_BIT_MAP, q_set1, cfg->rpt_bmp_qset1);
    REG_FIELD_READ(BM_MPCP_RPT_BIT_MAP, q_set2, cfg->rpt_bmp_qset2);
    REG_FIELD_READ(BM_LYNXD_MPCP_CFG, window_fit_en, cfg->schedule_fit_en);
    REG_FIELD_READ(BM_SCH_CFG, ds_dp_en, cfg->report_fit_en);
    cfg->report_fit_en = !cfg->report_fit_en;
    REG_FIELD_READ(BM_LYNXD_MPCP_CFG, force_quick_rpt_en, cfg->quick_rpt_en);
    REG_FIELD_READ(BM_LYNXD_MPCP_CFG, ignor_rpt_en, cfg->ignore_rpt_en);
    REG_FIELD_READ(BM_LYNXD_MPCP_CFG, single_window_en, cfg->single_window_en);
// K-mode
    REG_FIELD_READ(BM_MPCP_CFG, kt_rpt_mode_en, cfg->cs_mode_en);
    REG_FIELD_READ(BM_MPCP_CFG, kt_rpt_option, cfg->cs_mode_opt);
    REG_FIELD_READ(BM_MPCP_Q2_THRESHOLD_1_0, rpt_thrsld_cos0, cfg->cs_mode_threshold);
// C-mode
    REG_FIELD_READ(BM_MPCP_CFG, rpt_en_rsvd, cfg->le_mode_en);
    REG_FIELD_READ(ONU_MAC_MPCP_CFG, rpt_mode, cfg->le_mode_opt);
    REG_FIELD_READ(ONU_MAC_MPCP_CFG, rpt_pri, cfg->le_mode_bmp);

    return CS_E_OK;
}


cs_status aal_mpcp_misc_cfg_set(
    CS_IN cs_aal_mpcp_misc_msk_t msk,
    CS_IN cs_aal_mpcp_misc_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(msk.s.pending_grant)
    {
        if(cfg->pending_grant > AAL_MPCP_PENDING_GRANT_8)
        {
            AAL_MIN_LOG("pending_grant invalid.(%d) FILE: %s, LINE: %d\n", cfg->pending_grant, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_MPCP_CFG, pending_grant_select, cfg->pending_grant);
    }

    if(msk.s.min_grant_size)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_PKT_SIZE_CFG, min_grant_size, cfg->min_grant_size);
    }

    if(msk.s.request_frame_llid)
    {
        if(cfg->request_frame_llid > AAL_MPCP_REQ_LLID_0X7FFF) 
        {
            AAL_MIN_LOG("request_frame_llid invalid.(%d) FILE: %s, LINE: %d\n", cfg->request_frame_llid, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_MPCP_CONTROL, register_request_frame_LLID_select, cfg->request_frame_llid);
    }

    if(msk.s.mpcp_frame_len_check_en)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_CONTROL, disable_mpcp_frame_length_checking, (cfg->mpcp_frame_len_check_en==0));
    }

    if(msk.s.min_process_time_check_en)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_CONTROL, disable_minimum_processing_time_check, (cfg->min_process_time_check_en==0));
    }

    if(msk.s.min_process_time)
    {
        if (cfg->min_process_time > __MIN_PROCESS_TIME_MAX)
        {
            AAL_MIN_LOG("min_process_time invalid.(%d) FILE: %s, LINE: %d\n", cfg->min_process_time, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_MPCP_GRANT_PROCESS_TIME, min_processing_time, cfg->min_process_time);
    }

    if(msk.s.max_future_grant_time)
    {
        if (cfg->max_future_grant_time > __MAX_FUTURE_GRANT_TIME_MAX)
        {
            AAL_MIN_LOG("max_future_grant_time invalid.(%d) FILE: %s, LINE: %d\n", cfg->max_future_grant_time, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_MPCP_GRANT_PROCESS_TIME, max_future_grant_time, cfg->max_future_grant_time);
    }

    if(msk.s.discovery_pkt_size)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_PKT_SIZE_CFG, discovery_mpcp_pkt_size, cfg->discovery_pkt_size);
    }

    if(msk.s.mpcp_frame_fec_overhead)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_Configuration, mpcp_frame_fec_overhead, cfg->mpcp_frame_fec_overhead);
    }

    if(msk.s.onu_mac_addr_check_en)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_CONTROL, onu_mac_address_checking, (cfg->onu_mac_addr_check_en==0));
    }

    if(msk.s.strict_starttime_check_en)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_CONTROL, strict_starttime_check, (cfg->strict_starttime_check_en!=0));
    }

    if(msk.s.rereg_mode)
    {
        if (cfg->rereg_mode > AAL_MPCP_REREG_MODE_KEEP) 
        {
            AAL_MIN_LOG("rereg_mode invalid.(%d) FILE: %s, LINE: %d\n", cfg->rereg_mode, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_MPCP_CFG, rereg_mode, cfg->rereg_mode);
    }

    if(msk.s.force_rpt_data_en)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_CONTROL, en_rpt_data, (cfg->force_rpt_data_en!=0));
    }

    if(msk.s.tx_window_adjust_time)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_Configuration, transmit_window_starttime_adjust, cfg->tx_window_adjust_time);
    }

    if(msk.s.timestamp_update_location)
    {
        if (cfg->timestamp_update_location > AAL_MPCP_TS_UP_LOC_END)
        {
            AAL_MIN_LOG("timestamp_update_location invalid.(%d) FILE: %s, LINE: %d\n", cfg->timestamp_update_location, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_MPCP_CONTROL, timestamp_update_location, cfg->timestamp_update_location);
    }

    if(msk.s.direct_registration_en)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_CONTROL, allow_direct_registration, (cfg->direct_registration_en!=0));
    }

    if(msk.s.drain_window_guard_time)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_Configuration, drain_window_guard_time, cfg->drain_window_guard_time);
    }

    if(msk.s.mpcp_to_cpu_en)
    {
        REG_FIELD_WRITE(ONU_MAC_RXCTRL, mpcp_to_cpu, (cfg->mpcp_to_cpu_en!=0));
    }

    return CS_E_OK;
}

cs_status aal_mpcp_misc_cfg_get(
    CS_OUT cs_aal_mpcp_misc_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(ONU_MAC_MPCP_CFG, pending_grant_select, cfg->pending_grant);
    REG_FIELD_READ(ONU_MAC_MPCP_PKT_SIZE_CFG, min_grant_size, cfg->min_grant_size);
    REG_FIELD_READ(ONU_MAC_MPCP_CONTROL, register_request_frame_LLID_select, cfg->request_frame_llid);
    REG_FIELD_READ(ONU_MAC_MPCP_CONTROL, disable_mpcp_frame_length_checking, cfg->mpcp_frame_len_check_en);
    cfg->mpcp_frame_len_check_en = !cfg->mpcp_frame_len_check_en;
    REG_FIELD_READ(ONU_MAC_MPCP_CONTROL, disable_minimum_processing_time_check, cfg->min_process_time_check_en);
    cfg->min_process_time_check_en = !cfg->min_process_time_check_en;
    REG_FIELD_READ(ONU_MAC_MPCP_GRANT_PROCESS_TIME, min_processing_time, cfg->min_process_time);
    REG_FIELD_READ(ONU_MAC_MPCP_GRANT_PROCESS_TIME, max_future_grant_time, cfg->max_future_grant_time);
    REG_FIELD_READ(ONU_MAC_MPCP_PKT_SIZE_CFG, discovery_mpcp_pkt_size, cfg->discovery_pkt_size);
    REG_FIELD_READ(ONU_MAC_MPCP_Configuration, mpcp_frame_fec_overhead, cfg->mpcp_frame_fec_overhead);
    REG_FIELD_READ(ONU_MAC_MPCP_CONTROL, onu_mac_address_checking, cfg->onu_mac_addr_check_en);
    cfg->onu_mac_addr_check_en = !cfg->onu_mac_addr_check_en;
    REG_FIELD_READ(ONU_MAC_MPCP_CONTROL, strict_starttime_check, cfg->strict_starttime_check_en);
    REG_FIELD_READ(ONU_MAC_MPCP_CFG, rereg_mode, cfg->rereg_mode);
    REG_FIELD_READ(ONU_MAC_MPCP_CONTROL, en_rpt_data, cfg->force_rpt_data_en);
    REG_FIELD_READ(ONU_MAC_MPCP_Configuration, transmit_window_starttime_adjust, cfg->tx_window_adjust_time);
    REG_FIELD_READ(ONU_MAC_MPCP_CONTROL, timestamp_update_location, cfg->timestamp_update_location);
    REG_FIELD_READ(ONU_MAC_MPCP_CONTROL, allow_direct_registration, cfg->direct_registration_en);
    REG_FIELD_READ(ONU_MAC_MPCP_Configuration, drain_window_guard_time, cfg->drain_window_guard_time);
    REG_FIELD_READ(ONU_MAC_RXCTRL, mpcp_to_cpu, cfg->mpcp_to_cpu_en);

    return CS_E_OK;
}


cs_status aal_mpcp_rtt_comp_cfg_set(
    CS_IN cs_aal_mpcp_rtt_comp_msk_t msk,
    CS_IN cs_aal_mpcp_rtt_comp_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(msk.s.mode)
    {
        if (cfg->mode > AAL_MPCP_COMP_MODE_WITH_AES) 
        {
            AAL_MIN_LOG("rtt mode invalid.(%d) FILE: %s, LINE: %d\n", cfg->mode, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_MPCP_RTT_COMPENSATION_MODE, mode, cfg->mode);
    }

    if(msk.s.fec_comp_en)
    {
        REG_FIELD_WRITE(ONU_MAC_AES_FEC_COMP, fec_comp_enable, (cfg->fec_comp_en!=0));
    }

    if(msk.s.fec_comp_cnt_inc_en)
    {
        REG_FIELD_WRITE(ONU_MAC_AES_FEC_COMP, fec_comp_incdec, (cfg->fec_comp_cnt_inc_en!=0));
    }

    if(msk.s.fec_comp_cnt)
    {
        REG_FIELD_WRITE(ONU_MAC_AES_FEC_COMP, fec_comp_cnt, cfg->fec_comp_cnt);
    }

    if(msk.s.txmac_comp)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_RTT_COMPENSATION, ts_compensation, cfg->txmac_comp);
    }

    if(msk.s.egress_path_comp)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_RTT_COMPENSATION, output_compensation, cfg->egress_path_comp);
    }

    if(msk.s.rxmac_comp)
    {
        REG_FIELD_WRITE(ONU_MAC_MPCP_RTT_COMPENSATION, input_compensation, cfg->rxmac_comp);
    }

    if(msk.s.rxfec_aes_comp)
    {
        REG_MASK_WRITE(ONU_MAC_MPCP_RTT_COMPENSATION2, __RTT_COMP_RXFEC_AES_MASK, cfg->rxfec_aes_comp<<24);
    }

    if(msk.s.rxfec_churn_comp)
    {
        REG_MASK_WRITE(ONU_MAC_MPCP_RTT_COMPENSATION2, __RTT_COMP_RXFEC_CHURN_MASK, cfg->rxfec_churn_comp<<16);
    }

    if(msk.s.non_rxfec_aes_comp)
    {
        REG_MASK_WRITE(ONU_MAC_MPCP_RTT_COMPENSATION2, __RTT_COMP_NON_RXFEC_AES_MASK, cfg->non_rxfec_aes_comp<<8);
    }

    if(msk.s.non_rxfec_churn_comp)
    {
        REG_MASK_WRITE(ONU_MAC_MPCP_RTT_COMPENSATION2, __RTT_COMP_NON_RXFEC_CHURN_MASK, cfg->non_rxfec_churn_comp);
    }

    return CS_E_OK;
}


cs_status aal_mpcp_rtt_comp_cfg_get(
    CS_OUT cs_aal_mpcp_rtt_comp_cfg_t *cfg
)
{
    cs_uint32 wrd;
    
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(ONU_MAC_MPCP_RTT_COMPENSATION_MODE, mode, cfg->mode);
    REG_FIELD_READ(ONU_MAC_AES_FEC_COMP, fec_comp_enable, cfg->fec_comp_en);
    REG_FIELD_READ(ONU_MAC_AES_FEC_COMP, fec_comp_incdec, cfg->fec_comp_cnt_inc_en);
    REG_FIELD_READ(ONU_MAC_AES_FEC_COMP, fec_comp_cnt, cfg->fec_comp_cnt);
    REG_FIELD_READ(ONU_MAC_MPCP_RTT_COMPENSATION, ts_compensation, cfg->txmac_comp);
    REG_FIELD_READ(ONU_MAC_MPCP_RTT_COMPENSATION, output_compensation, cfg->egress_path_comp);
    REG_FIELD_READ(ONU_MAC_MPCP_RTT_COMPENSATION, input_compensation, cfg->rxmac_comp);
    REG_FIELD_READ(ONU_MAC_MPCP_RTT_COMPENSATION2, input_compensation2, wrd);
    cfg->rxfec_aes_comp = (wrd & __RTT_COMP_RXFEC_AES_MASK)>>24;
    cfg->rxfec_churn_comp = (wrd & __RTT_COMP_RXFEC_CHURN_MASK)>>16;
    cfg->non_rxfec_aes_comp = (wrd & __RTT_COMP_NON_RXFEC_AES_MASK)>>8;
    cfg->non_rxfec_churn_comp = wrd & __RTT_COMP_NON_RXFEC_CHURN_MASK;

    return CS_E_OK;
}



cs_status aal_mpcp_queue_thrshld_set(
    CS_IN cs_aal_mpcp_queue_id_t id,
    CS_IN cs_uint16 thrshld,
    CS_IN cs_uint16 max_thrshld
)
{
    if (id > AAL_MPCP_QUEUE_ID_7)
    {
        AAL_MIN_LOG("queue id invalid.(%d) FILE: %s, LINE: %d\n", id, __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    switch (id)
    {
        case AAL_MPCP_QUEUE_ID_0:
            REG_FIELD_WRITE(BM_MPCP_Q2_THRESHOLD_1_0, rpt_thrsld_cos0, thrshld);
            REG_FIELD_WRITE(BM_MPCP_Q2_MAX_1_0, rpt_max_cos0, max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_1:
            REG_FIELD_WRITE(BM_MPCP_Q2_THRESHOLD_1_0, rpt_thrsld_cos1, thrshld);
            REG_FIELD_WRITE(BM_MPCP_Q2_MAX_1_0, rpt_max_cos1, max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_2:
            REG_FIELD_WRITE(BM_MPCP_Q2_THRESHOLD_3_2, rpt_thrsld_cos2, thrshld);
            REG_FIELD_WRITE(BM_MPCP_Q2_MAX_3_2, rpt_max_cos2, max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_3:
            REG_FIELD_WRITE(BM_MPCP_Q2_THRESHOLD_3_2, rpt_thrsld_cos3, thrshld);
            REG_FIELD_WRITE(BM_MPCP_Q2_MAX_3_2, rpt_max_cos3, max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_4:
            REG_FIELD_WRITE(BM_MPCP_Q2_THRESHOLD_5_4, rpt_thrsld_cos4, thrshld);
            REG_FIELD_WRITE(BM_MPCP_Q2_MAX_5_4, rpt_max_cos4, max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_5:
            REG_FIELD_WRITE(BM_MPCP_Q2_THRESHOLD_5_4, rpt_thrsld_cos5, thrshld);
            REG_FIELD_WRITE(BM_MPCP_Q2_MAX_5_4, rpt_max_cos5, max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_6:
            REG_FIELD_WRITE(BM_MPCP_Q2_THRESHOLD_7_6, rpt_thrsld_cos6, thrshld);
            REG_FIELD_WRITE(BM_MPCP_Q2_MAX_7_6, rpt_max_cos6, max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_7:
            REG_FIELD_WRITE(BM_MPCP_Q2_THRESHOLD_7_6, rpt_thrsld_cos7, thrshld);
            REG_FIELD_WRITE(BM_MPCP_Q2_MAX_7_6, rpt_max_cos7, max_thrshld);
            break;

        default:
            return CS_E_ERROR;
    }

    return CS_E_OK;
}


cs_status aal_mpcp_queue_thrshld_get(
    CS_IN cs_aal_mpcp_queue_id_t id,
    CS_OUT cs_uint16 *thrshld,
    CS_OUT cs_uint16 *max_thrshld
)
{
    if (id > AAL_MPCP_QUEUE_ID_7)
    {
        AAL_MIN_LOG("queue id invalid.(%d) FILE: %s, LINE: %d\n", id, __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    switch (id)
    {
        case AAL_MPCP_QUEUE_ID_0:
            REG_FIELD_READ(BM_MPCP_Q2_THRESHOLD_1_0, rpt_thrsld_cos0, *thrshld);
            REG_FIELD_READ(BM_MPCP_Q2_MAX_1_0, rpt_max_cos0, *max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_1:
            REG_FIELD_READ(BM_MPCP_Q2_THRESHOLD_1_0, rpt_thrsld_cos1, *thrshld);
            REG_FIELD_READ(BM_MPCP_Q2_MAX_1_0, rpt_max_cos1, *max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_2:
            REG_FIELD_READ(BM_MPCP_Q2_THRESHOLD_3_2, rpt_thrsld_cos2, *thrshld);
            REG_FIELD_READ(BM_MPCP_Q2_MAX_3_2, rpt_max_cos2, *max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_3:
            REG_FIELD_READ(BM_MPCP_Q2_THRESHOLD_3_2, rpt_thrsld_cos3, *thrshld);
            REG_FIELD_READ(BM_MPCP_Q2_MAX_3_2, rpt_max_cos3, *max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_4:
            REG_FIELD_READ(BM_MPCP_Q2_THRESHOLD_5_4, rpt_thrsld_cos4, *thrshld);
            REG_FIELD_READ(BM_MPCP_Q2_MAX_5_4, rpt_max_cos4, *max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_5:
            REG_FIELD_READ(BM_MPCP_Q2_THRESHOLD_5_4, rpt_thrsld_cos5, *thrshld);
            REG_FIELD_READ(BM_MPCP_Q2_MAX_5_4, rpt_max_cos5, *max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_6:
            REG_FIELD_READ(BM_MPCP_Q2_THRESHOLD_7_6, rpt_thrsld_cos6, *thrshld);
            REG_FIELD_READ(BM_MPCP_Q2_MAX_7_6, rpt_max_cos6, *max_thrshld);
            break;

        case AAL_MPCP_QUEUE_ID_7:
            REG_FIELD_READ(BM_MPCP_Q2_THRESHOLD_7_6, rpt_thrsld_cos7, *thrshld);
            REG_FIELD_READ(BM_MPCP_Q2_MAX_7_6, rpt_max_cos7, *max_thrshld);
            break;

        default:
            return CS_E_ERROR;
    }

    return CS_E_OK;
}


cs_status aal_mpcp_enable_set(
    CS_IN cs_boolean enable
)
{
    REG_FIELD_WRITE(ONU_MAC_MPCP_CONTROL, mpcp_mode, (enable!=0));
    return CS_E_OK;
}


cs_status aal_mpcp_enable_get(
    CS_OUT cs_boolean *enable
)
{
    if(NULL==enable) 
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    REG_FIELD_READ(ONU_MAC_MPCP_CONTROL, mpcp_mode, *enable);
    return CS_E_OK;
}


cs_status aal_mpcp_offline_set(
    CS_IN cs_boolean enable
)
{
    REG_FIELD_WRITE(ONU_MAC_MPCP_ENABLE_CONTROL, ONU_local_offline, (enable!=0));
    return CS_E_OK;
}


cs_status aal_mpcp_offline_get(
    CS_OUT cs_boolean *offline
)
{
    if(NULL==offline)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    REG_FIELD_READ(ONU_MAC_MPCP_ENABLE_CONTROL, ONU_local_offline, *offline);
    return CS_E_OK;
}


cs_status aal_mpcp_dereg_case_get(
    CS_OUT cs_aal_mpcp_dereg_case_t *dereg_case
)
{
    if(NULL==dereg_case)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    REG_FIELD_READ(ONU_MAC_MPCP_DEREG_CAUSE, code, *dereg_case);
    return CS_E_OK;
}


cs_status aal_mpcp_reg_status_get(
    CS_OUT cs_boolean *reg
)
{
    if(NULL==reg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    REG_FIELD_READ(ONU_MAC_MPCP_STATUS, onureg, *reg);
    return CS_E_OK;
}


cs_status aal_mpcp_grant_permit_set(
    CS_IN cs_boolean permit
)
{
    REG_FIELD_WRITE(ONU_MAC_MPCP_CONTROL, clr_pmit, !permit);
    return CS_E_OK;
}


cs_status aal_mpcp_grant_permit_get(
    CS_OUT cs_boolean *permit
)
{
    cs_boolean clr;

    if(NULL==permit) 
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(ONU_MAC_MPCP_CONTROL, clr_pmit, clr);
    *permit = !clr;

    return CS_E_OK;
}

cs_status aal_mpcp_reg_toggle(void)
{
    REG_FIELD_WRITE(ONU_MAC_MPCP_ENABLE_CONTROL, mpcp_registration_init, 0);
    REG_FIELD_WRITE(ONU_MAC_MPCP_ENABLE_CONTROL, mpcp_registration_init, 1);
    return CS_E_OK;
}

cs_status aal_mpcp_llid_get(cs_uint16 *llid)
{
    if(NULL==llid) 
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    REG_FIELD_READ(ONU_MAC_MPCP_LLID_SyncTime, LLID, *llid);
    return CS_E_OK;
}

cs_status aal_mpcp_reg_req_rsv0(cs_uint32 data)
{
    REG_WRITE(ONU_MAC_MPCP_REG_REQ_RESV0, data);
    return CS_E_OK;
}

cs_status aal_mpcp_reg_req_rsv1(cs_uint32 data)
{
    REG_WRITE(ONU_MAC_MPCP_REG_REQ_RESV1, data);
    return CS_E_OK;
}

cs_status aal_mpcp_reg_flag_get(cs_uint8 *flag)
{
    cs_uint8 reg_flag;

    if(NULL==flag) 
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(ONU_MAC_MPCP_REGISTER_PARAMETERS1, flag, reg_flag);

    *flag = reg_flag;

    return CS_E_OK;
}

cs_status mpcp_init(void)
{
    cs_status rt = CS_E_OK;
    cs_aal_mpcp_timer_msk_t timer_mask;
    cs_aal_mpcp_timer_cfg_t timer_cfg;
    cs_aal_mpcp_misc_msk_t misc_mask;
    cs_aal_mpcp_misc_cfg_t misc_cfg;
        
    // timer init
    timer_cfg.gatetimer_en = 1;
    timer_cfg.mpcptimer_en = 1;
    timer_cfg.reporttimer_en = 1;
    timer_cfg.gatetimer_dereg_en = 1;
    timer_cfg.mpcptimer_dereg_en = 1;
    timer_cfg.timedrift_dereg_en = 0;
    timer_mask.u32 = 0;
    timer_mask.s.gatetimer_en = 1;
    timer_mask.s.mpcptimer_en = 1;
    timer_mask.s.reporttimer_en = 1;
    timer_mask.s.gatetimer_dereg_en = 1;
    timer_mask.s.mpcptimer_dereg_en = 1;
    timer_mask.s.timedrift_dereg_en = 0;
    rt = aal_mpcp_timer_cfg_set(timer_mask, &timer_cfg);
    if(rt!=CS_E_OK)
    {
        cs_halt("aal_mpcp_timer_cfg_set return %d. FILE: %s, LINE: %d\n", rt, __FILE__, __LINE__);
    }

    // mpcp misc init
    misc_cfg.request_frame_llid = AAL_MPCP_REQ_LLID_ASSIGNED;
    misc_cfg.min_grant_size = 0x2a;
    misc_cfg.discovery_pkt_size = 0x2a;
    misc_mask.u32 = 0;
    misc_mask.s.request_frame_llid = 1;
    misc_mask.s.min_grant_size = 1;
    misc_mask.s.discovery_pkt_size = 1;
    rt = aal_mpcp_misc_cfg_set(misc_mask, &misc_cfg);
    if(rt!=CS_E_OK)
    {
        cs_halt("aal_mpcp_timer_cfg_set return %d. FILE: %s, LINE: %d\n", rt, __FILE__, __LINE__);
    }

    // turn off simulation
    REG_FIELD_WRITE(ONU_MAC_SIM_CTRL, fast_1ms_clk_sel, 0);
    REG_FIELD_WRITE(ONU_MAC_SIM_CTRL, false_rpt_value, 0);

    return CS_E_OK;
}


