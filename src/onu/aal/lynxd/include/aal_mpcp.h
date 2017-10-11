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
* $Id: aal_mpcp.h,v 1.1.2.5.2.7.6.1.2.1 2011/06/28 06:20:29 chuang Exp $
*
* This file is used for mpcp code
*/

#ifndef __AAL_MPCP_H__
#define __AAL_MPCP_H__

#include "cs_types.h"

/* structure define */
typedef enum
{
    AAL_MPCP_QUEUE_ID_0                 = 0,
    AAL_MPCP_QUEUE_ID_1                 = 1,
    AAL_MPCP_QUEUE_ID_2                 = 2,
    AAL_MPCP_QUEUE_ID_3                 = 3,
    AAL_MPCP_QUEUE_ID_4                 = 4,
    AAL_MPCP_QUEUE_ID_5                 = 5,
    AAL_MPCP_QUEUE_ID_6                 = 6,
    AAL_MPCP_QUEUE_ID_7                 = 7,
} cs_aal_mpcp_queue_id_t;

typedef enum
{
    AAL_MPCP_DEREG_UNKNOWN              = 0, /* no deregistration from last time (or last clear) */
    AAL_MPCP_DEREG_TIMEDRFT             = 1, /* caused by time drift */
    AAL_MPCP_DEREG_OLT                  = 2, /* caused by remote OLT deregistration */
    AAL_MPCP_DEREG_LOCAL                = 3, /* caused by local deregistration */
    AAL_MPCP_DEREG_REREG                = 4, /* caused by receiving re-reg flag */
    AAL_MPCP_DEREG_TIMEOUT              = 5, /* caused by MPCP timeout */
    AAL_MPCP_DEREG_GATE                 = 6, /* caused by gate timer timeout */
    AAL_MPCP_DEREG_SW                   = 7, /* caused by software disbling the MPCP */
} cs_aal_mpcp_dereg_case_t;

typedef enum
{
    AAL_MPCP_RPT_FMT_0                  = 0, /* report the total byte-count for each of the queues in the first Q-Set, then the under-threshold byte-count in the second Q-Set;*/
    AAL_MPCP_RPT_FMT_1                  = 1, /* report the under-threshold byte-count for each of the queues in the first Q-Set, then the total byte-count in the second Q-Set */
    AAL_MPCP_RPT_FMT_2                  = 2, /*  propietary reporting format */
} cs_aal_mpcp_rpt_fmt_t;

typedef enum
{
    AAL_MPCP_RPT_ORDER_REV              = 0, /* Reverse order: scan resulu of physical queue0 will be reported as queue4, in both Q-Set1 and Q-Set2 */
    AAL_MPCP_RPT_ORDER_SEQ              = 1, /* Sequential order: scan resulu of physical queue0 will be reported as queue0, in both Q-Set1 and Q-Set2; */
} cs_aal_mpcp_rpt_order_t;

typedef enum
{
    AAL_MPCP_RPT_NUM_1                  = 1, /* Single Queue Set */
    AAL_MPCP_RPT_NUM_2                  = 2, /* 2 Queue Set */
} cs_aal_mpcp_rpt_num_t;

typedef enum
{
    AAL_MPCP_TS_UP_LOC_MIDDLE           = 0, /* update timestamp when ONU receives d/s MPCP frame */
    AAL_MPCP_TS_UP_LOC_BEGINNING        = 1, /* update timestamp at end of MPCP frame */
    AAL_MPCP_TS_UP_LOC_END              = 2, /* update timestamp at beginning of MPCP fram */
}cs_aal_mpcp_ts_up_loc_t;

typedef enum
{
    AAL_MPCP_REQ_LLID_ASSIGNED          = 0, /* use assigned LLID value as LLID of MPCP Register Request Frame */
    AAL_MPCP_REQ_LLID_0X7FFF            = 1, /* use 0x7FFF as LLID of MPCP Register Request Frame */
}cs_aal_mpcp_req_llid_t;

typedef enum
{
    AAL_MPCP_REREG_MODE_DEREGISTER      = 0, /* when ONU receives Register frame with rereg flag, keep ONU registered */
    AAL_MPCP_REREG_MODE_KEEP            = 1, /* when ONU receives Register frame with rereg flag, deregister ONU */
}cs_aal_mpcp_rereg_mode_t;

typedef enum
{
    AAL_MPCP_CS_MODE_RPT_OPT_Q0         = 0, /* The lump-sum of all queues is reported as Q0 */
    AAL_MPCP_CS_MODE_RPT_OPT_Q7         = 1, /* The lump-sum of all queues is reported as Q7 */
}cs_aal_mpcp_cs_mode_rpt_opt_t;

typedef enum
{
    AAL_MPCP_LE_MODE_RPT_OPT_2QSET      = 0, /* two queue set reporting */
    AAL_MPCP_LE_MODE_RPT_OPT_1QSET      = 1, /* one queue set reporting */
}cs_aal_mpcp_le_mode_rpt_opt_t;

typedef enum
{
    AAL_MPCP_COMP_MODE_NONE             = 0, /* no compansation */
    AAL_MPCP_COMP_MODE_WITHOUT_AES      = 1, /* compansation without AES */
    AAL_MPCP_COMP_MODE_WITH_AES         = 2, /* compansation with AES */
}cs_aal_mpcp_compan_mode_t;

typedef enum
{
    AAL_MPCP_PENDING_GRANT_1            = 1,
    AAL_MPCP_PENDING_GRANT_2            = 2,
    AAL_MPCP_PENDING_GRANT_3            = 3,
    AAL_MPCP_PENDING_GRANT_4            = 4,
    AAL_MPCP_PENDING_GRANT_5            = 5,
    AAL_MPCP_PENDING_GRANT_6            = 6,
    AAL_MPCP_PENDING_GRANT_7            = 7,
    AAL_MPCP_PENDING_GRANT_8            = 8,
} cs_aal_mpcp_pending_grant_t;

typedef struct
{
    cs_aal_mpcp_pending_grant_t             pending_grant;              // Read-only: 1~8 is valid
    cs_uint8                                min_grant_size;             // recommand set to 0x2a (64+8+12)byte = 42TQ
    cs_aal_mpcp_req_llid_t                  request_frame_llid;         // 1: assigned; 0: 0x7fff
    cs_boolean                              mpcp_frame_len_check_en;    // 1: only 64byte accepted; 0: accept >64byte mpcp frame
    cs_boolean                              min_process_time_check_en;  // 1: ONU will compare start time and local timestamp
    cs_uint16                               min_process_time;           // config min processing time, default is 1024 (protocol also defined 1024)
    cs_uint16                               max_future_grant_time;      // if (StartTime-LocalTime)>THIS, ignore this grant, default is 1024
    cs_uint8                                discovery_pkt_size;         // unit is TQ, exclude FEC overhead
    cs_boolean                              onu_mac_addr_check_en;      // 0: ONU doesn't check whether DA in receiving MPCP frame matches with ONUs MAC address
                                                                        // 1: ONU checks every receiving MPCP frames DA,
                                                                        //    (01-80-c2-00-00-01) and DA matched with ONUs MAC address can be accepted
    cs_uint8                                mpcp_frame_fec_overhead;    // 0~255. Unit is TQ. used when TxFEC is enabled
    cs_boolean                              strict_starttime_check_en;  // Only when one grant's start time exactly matches local timer, that grant window can be started
    cs_aal_mpcp_rereg_mode_t                rereg_mode;                 // 1: when ONU receives Register frame with rereg flag, keep ONU registered.
                                                                        // 0: when ONU receives Register frame with rereg flag, deregister ONU.
    cs_boolean                              force_rpt_data_en;          // 1: allow BM transmit user data packet in force-report grant window if there are extra window space
                                                                        // 0: not allow BM transmit user data packet in force-report grant window
    cs_uint8                                tx_window_adjust_time;      // debug only
    cs_aal_mpcp_ts_up_loc_t                 timestamp_update_location;  // debug only
    cs_boolean                              direct_registration_en;     // Allow the MPCP engine to accept a registration frame even if it is in the WAIT state
    cs_uint8                                drain_window_guard_time;    // debug only
    cs_boolean                              mpcp_to_cpu_en;             // debug only
} cs_aal_mpcp_misc_cfg_t;

typedef union
{
    struct
    {
    cs_uint32  pending_grant                : 1;
    cs_uint32  min_grant_size               : 1;
    cs_uint32  request_frame_llid           : 1;
    cs_uint32  mpcp_frame_len_check_en      : 1;
    cs_uint32  min_process_time_check_en    : 1;
    cs_uint32  min_process_time             : 1;
    cs_uint32  max_future_grant_time        : 1;
    cs_uint32  discovery_pkt_size           : 1;
    cs_uint32  onu_mac_addr_check_en        : 1;
    cs_uint32  mpcp_frame_fec_overhead      : 1;
    cs_uint32  strict_starttime_check_en    : 1;
    cs_uint32  rereg_mode                   : 1;
    cs_uint32  force_rpt_data_en            : 1;
    cs_uint32  tx_window_adjust_time        : 1;
    cs_uint32  timestamp_update_location    : 1;
    cs_uint32  direct_registration_en       : 1;
    cs_uint32  drain_window_guard_time      : 1;
    cs_uint32  mpcp_to_cpu_en               : 1;
    } s;
    cs_uint32 u32;
}cs_aal_mpcp_misc_msk_t;

typedef struct
{
// J-mode
    cs_uint16                               t_threshold;            // (default=774) 0~0xffff
    cs_uint8                                zeroq_map;              // (default=0) zeroed queue map
    cs_uint16                               zeroq_threshold;        // (default=65535) zeroed queue threshold
    cs_boolean                              aggregation_mode_en;    // (default=true) True: Q-set threshold "T" is aggregated thresholds for reporting
                                                                    // false: Q-set threshold "T" is independent thresholds for reporting
    cs_aal_mpcp_rpt_fmt_t                   report_format;          // (default=AAL_MPCP_REPT_FMT_0) report format
    cs_aal_mpcp_rpt_order_t                 rpt_order;              // (default=AAL_MPCP_REPT_ORDER_REVRS) LynxD Mode Reporting Order Select
    cs_aal_mpcp_rpt_num_t                   rpt_number;             // (default=AAL_MPCP_REPT_NUM_2) MPCP Reporting Number
    cs_uint8                                rpt_bmp_qset1;          // report bitmap for qset1
    cs_uint8                                rpt_bmp_qset2;          // report bitmap for qset2
    cs_boolean                              schedule_fit_en;        // (default=true) True : schedule a lower-priority packet if the packet from higher priority can not
                                                                    // fit in the remaining grant window;
                                                                    // False: Disable, stop egress traffic if the packet from the higest priority queue
                                                                    // can not fit into the remaining grant window
    cs_boolean                              report_fit_en;          // Enable reporting fit or not
    cs_boolean                              quick_rpt_en;           // (default=false) True: force quick report, False: no force quick report
    cs_boolean                              ignore_rpt_en;          // 1: The egress scheduler ignors the content (QSet2 and QSet1) of MPCP reportand just dequeues packet on SP basis
                                                                    // 0: Strictly follows the content (QSet2 and QSet1) of MPCP report
    cs_boolean                              single_window_en;       // 1 : The report and data are in single window;
                                                                    // 0 :The report and data are in seperate window;
                                                                    // Note : This only works with force_quick_rpt_en=1;    
// K-mode
    cs_boolean                              cs_mode_en;             // Enable K-mode report    
    cs_aal_mpcp_cs_mode_rpt_opt_t           cs_mode_opt;            // 1 : The lump-sum of all queues is reported as Q7
                                                                    // 0 : The lump-sum of all queues is reported as Q0;
    cs_uint16                               cs_mode_threshold;      // 0~0xffff
// C-mode
    cs_boolean                              le_mode_en;             // Enable C-mode report
    cs_aal_mpcp_le_mode_rpt_opt_t           le_mode_opt;            // 0 : 2 Q set mode, each queue set has 8 priorities.
                                                                    // 1: One queue set mode, the bitmap is specified at "le_mode_bmp"
    cs_uint8                                le_mode_bmp;            // One queue set bitmap, see "le_mode_opt"


}cs_aal_mpcp_report_cfg_t;

typedef union 
{
    struct
    {
    // J-mode
    cs_uint32  t_threshold                  : 1;
    cs_uint32  zeroq_map                    : 1;
    cs_uint32  zeroq_threshold              : 1;
    cs_uint32  aggregation_mode_en          : 1;
    cs_uint32  report_format                : 1;
    cs_uint32  rpt_order                    : 1;
    cs_uint32  rpt_number                   : 1;
    cs_uint32  rpt_bmp_qset1                : 1;
    cs_uint32  rpt_bmp_qset2                : 1;
    cs_uint32  schedule_fit_en              : 1;
    cs_uint32  report_fit_en                : 1;
    cs_uint32  quick_rpt_en                 : 1;
    cs_uint32  ignore_rpt_en                : 1;
    cs_uint32  single_window_en             : 1;
    // K-mode
    cs_uint32  cs_mode_en                   : 1;
    cs_uint32  cs_mode_opt                  : 1;
    cs_uint32  cs_mode_threshold            : 1;
    // C-mode
    cs_uint32  le_mode_en                   : 1;
    cs_uint32  le_mode_opt                  : 1;
    cs_uint32  le_mode_bmp                  : 1;
    } s;
    cs_uint32 u32;
}cs_aal_mpcp_report_msk_t;

typedef struct
{
    cs_boolean                              gatetimer_en;           // (default=true) Enable gate timer or not
    cs_boolean                              mpcptimer_en;           // (default=true) Enable mpcp timer or not
    cs_boolean                              reporttimer_en;         // (default=true) Enable report timer or not
    cs_boolean                              gatetimer_dereg_en;     // (default=true) Enable derigistration when gate timer expiration
    cs_boolean                              mpcptimer_dereg_en;     // (default=true) Enable deregistration when mpcp timer expiration
    cs_boolean                              timedrift_dereg_en;     // (default=true) Enable deregistration when timestamp drift detection
    cs_uint16                               gatetimer_threshold;    // 0~4095(default=50): config the threshold for gate timer expiration in unit of ms
    cs_uint16                               mpcptimer_threshold;    // 0~4095(default=1000): config the threshold for mpcp timer expiration in unit of ms
    cs_uint8                                timedrift_threshold;    // 0~255(default=8): config the threshold for timestamp drift detection in unit of TQ
    cs_uint16                               reporttimer_threshold;  // 0~4095(default=50ms) config the threshold for report timer in unit of ms
} cs_aal_mpcp_timer_cfg_t;

typedef union
{
    struct
    {
    cs_uint32  gatetimer_en                 : 1;
    cs_uint32  mpcptimer_en                 : 1;
    cs_uint32  reporttimer_en               : 1;
    cs_uint32  gatetimer_dereg_en           : 1;
    cs_uint32  mpcptimer_dereg_en           : 1;
    cs_uint32  timedrift_dereg_en           : 1;
    cs_uint32  gatetimer_threshold          : 1;
    cs_uint32  mpcptimer_threshold          : 1;
    cs_uint32  timedrift_threshold          : 1;
    cs_uint32  reporttimer_threshold        : 1;
    } s;
    cs_uint32 u32;
}cs_aal_mpcp_timer_msk_t;

typedef struct
{
    cs_aal_mpcp_compan_mode_t               mode;                   // specified the compensation scheme mode
    cs_boolean                              fec_comp_en;
    cs_boolean                              fec_comp_cnt_inc_en;
    cs_uint16                               fec_comp_cnt;
    cs_uint8                                txmac_comp;             // when mode == AAL_MPCP_COMPAN_MODE_WITH_AES, invalid
    cs_uint8                                egress_path_comp;       // when mode == AAL_MPCP_COMPAN_MODE_WITH_AES, invalid
    cs_uint8                                rxmac_comp;             // when mode == AAL_MPCP_COMPAN_MODE_WITH_AES, invalid
    cs_uint8                                rxfec_aes_comp;
    cs_uint8                                rxfec_churn_comp;
    cs_uint8                                non_rxfec_aes_comp;
    cs_uint8                                non_rxfec_churn_comp;
} cs_aal_mpcp_rtt_comp_cfg_t;

typedef union
{
    struct 
    {
    cs_uint32  mode                         : 1;
    cs_uint32  fec_comp_en                  : 1;
    cs_uint32  fec_comp_cnt_inc_en          : 1;
    cs_uint32  fec_comp_cnt                 : 1;
    cs_uint32  txmac_comp                   : 1;
    cs_uint32  egress_path_comp             : 1;
    cs_uint32  rxmac_comp                   : 1;
    cs_uint32  rxfec_aes_comp               : 1;
    cs_uint32  rxfec_churn_comp             : 1;
    cs_uint32  non_rxfec_aes_comp           : 1;
    cs_uint32  non_rxfec_churn_comp         : 1;
    } s;
    cs_uint32 u32;
}cs_aal_mpcp_rtt_comp_msk_t;

/**************************************************************************/
/* $rtn_hdr_start  Set MPCP Enable/Disable                              */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_enable_set(
    CS_IN cs_boolean enable
);
/* INPUTS     :   o enable - enable/disable                               */
/*                          (default=enable) */
/* OUTPUTS    :    ----                                                       */
/* RETURNS    : o  CS_OK: Set successfully                                   */
/*              o  CS_ERROR: Set failed                                   */
/* DESCRIPTION:                                                           */
/*    Set MPCP adminitration control. Disable means EPON port becomes a Gigabit Ether port. */
/* $rtn_hdr_end                                                           */
/*******************************************************************************/

/**************************************************************************/
/* $rtn_hdr_start  Get MPCP Enable/Disable                              */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_enable_get(
    CS_OUT cs_boolean *enable
);
/* INPUTS     : ----                                                          */
/* OUTPUTS    :  o enable - enable/disable                                */
/* RETURNS    : o  CS_OK: Get successfully                                   */
/*              o  CS_ERROR: Get failed                                   */
/* DESCRIPTION:                                                           */
/*    Get MPCP adminitration control                                            */
/* $rtn_hdr_end                                                           */
/*******************************************************************************/

/**************************************************************************/
/* $rtn_hdr_start  Set MPCP Offline Control                             */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_offline_set(
    CS_IN cs_boolean offline
);
/* INPUTS     :   o offline - normal/offline                               */
/*                          (default=AAL_MPCP_OFFLINE_NORMAL) */
/* OUTPUTS    :    ----                                                       */
/* RETURNS    : o  CS_OK: Set successfully                                   */
/*              o  CS_ERROR: Set failed                                   */
/* DESCRIPTION:                                                           */
/*    Set MPCP offline control. The related enum structure is defined as following: */
/*typedef enum { */
/*    AAL_MPCP_OFFLINE_MIN = 0,*/
/*    AAL_MPCP_OFFLINE_NORMAL = AAL_MPCP_OFFLINE_MIN, // Keep normal MPCP state */
/*    AAL_MPCP_OFFLINE_ALWAYS = 1,       // Always keep offline (go de-registerred if online currently) */
/*    AAL_MPCP_OFFLINE_MAX = AAL_MPCP_OFFLINE_ALWAYS,*/
/*} cs_aal_mpcp_offline_e;     */
/* $rtn_hdr_end                                                           */
/*******************************************************************************/

/**************************************************************************/
/* $rtn_hdr_start  Get MPCP Enable/Disable                              */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_offline_get(
    CS_OUT cs_boolean *offline
);
/* INPUTS     : ----                                                          */
/* OUTPUTS    :  o offline - normal/offline                                */
/* RETURNS    : o  CS_OK: Get successfully                                   */
/*              o  CS_ERROR: Get failed                                   */
/* DESCRIPTION:                                                           */
/*    Get MPCP offline control, the sturcture is same as "onu_mpcp_offline_set"           */
/* $rtn_hdr_end                                                           */
/*******************************************************************************/

/**************************************************************************/
/* $rtn_hdr_start  Get MPCP Deregistaration Reason                  */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_dereg_case_get(
    CS_OUT cs_aal_mpcp_dereg_case_t *dereg_case
);
/* INPUTS     : NA                                                              */
/* OUTPUTS    : NA                                */
/* RETURNS    : o  cs_aal_mpcp_dereg_case_e                                      */
/* DESCRIPTION:                                                                 */
/*    Get the lastest MPCP deregistration reason. These reasons are defined as following */
/*    Structure. */
/*typedef enum {*/
/*    AAL_MPCP_DEREG_MIN = 0,*/
/*    AAL_MPCP_DEREG_NONE = AAL_MPCP_DEREG_MIN, //no deregistration from last time (or last clear) */
/*    AAL_MPCP_DEREG_TIMEDRFT = 1, // caused by time drift */
/*    AAL_MPCP_DEREG_OLT = 2, // caused by remote OLT deregistration */
/*    AAL_MPCP_DEREG_LOCAL = 3, // caused by local deregistration */
/*    AAL_MPCP_DEREG_REREG = 4, // caused by receiving re-reg flag */
/*    AAL_MPCP_DEREG_TIMEOUT = 5, // caused by MPCP timeout */
/*    AAL_MPCP_DEREG_GATE = 6, // caused by gate timer timeout */
/*    AAL_MPCP_DEREG_SW = 7, // caused by software disbling the MPCP */
/*    AAL_MPCP_DEREG_MAX = AAL_MPCP_DEREG_SW,*/
/*} cs_aal_mpcp_dereg_case_e; */
/* $rtn_hdr_end                                                           */
/*******************************************************************************/

/**************************************************************************/
/* $rtn_hdr_start  Get MPCP Registaration Status                  */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_reg_status_get(
    CS_OUT cs_boolean *reg
);
/* INPUTS     : ----  */
/* OUTPUTS    :   o registerred - Registerred or not                                */
/* RETURNS    : o  CS_OK: Get successfully                                   */
/*              o  CS_ERROR: Get failed                                   */
/* DESCRIPTION:                                                           */
/*    Get the lastest MPCP registration status                                           */
/* $rtn_hdr_end                                                           */
/*******************************************************************************/

/**************************************************************************/
/* $rtn_hdr_start  Set MPCP Report Queue Threshold                              */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_queue_thrshld_set(
    CS_IN cs_aal_mpcp_queue_id_t id,
    CS_IN cs_uint16 thrshld,
    CS_IN cs_uint16 max_thrshld
);
/* INPUTS     :    o id - Queue index */
/*       o thrshld - Queue threshold                               */
/*          (default=32768) */
/*       o max_thrshld - Maximum Queue threshold                               */
/*          (default=65535) */
/* OUTPUTS    :    ----                                                       */
/* RETURNS    : o  CS_OK: Set successfully                                   */
/*              o  CS_ERROR: Set failed                                   */
/* DESCRIPTION:                                                           */
/*    Set MPCP report queue threshold. This API is only supported under regular MPCP mode.   */
/* $rtn_hdr_end                                                           */
/*******************************************************************************/

/**************************************************************************/
/* $rtn_hdr_start  Get MPCP Report Queue Threshold                              */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_queue_thrshld_get(
    CS_IN cs_aal_mpcp_queue_id_t id,
    CS_OUT cs_uint16 *thrshld,
    CS_OUT cs_uint16 *max_thrshld
);
/* INPUTS     :                                                          */
/*       o id - Queue index */
/* OUTPUTS    :   o thrshld - Queue threshold                               */
/*       o max_thrshld - Maximum Queue threshold                               */
/* RETURNS    : o  CS_OK: Get successfully                                   */
/*              o  CS_ERROR: Get failed                                   */
/* DESCRIPTION:                                                           */
/*    Get MPCP report queue threshold. This API is only supported under regular MPCP mode. */
/* $rtn_hdr_end                                                           */
/*******************************************************************************/

/*****************************************************************************/
/* $rtn_hdr_start   Set MPCP (DBA) Report parameters                                 */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_report_cfg_set(
    CS_IN cs_aal_mpcp_report_msk_t msk,
    CS_IN cs_aal_mpcp_report_cfg_t *cfg
);
/* INPUTS     : o  msk - Parameter mask                   */
/* o  cfg - Configure MPCP (DBA) report parameters                   */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_ERROR: other error occurs                      */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_OK: set successfully                                   */
/* DESCRIPTION:                                                              */
/*             This API is used to  configure MPCP (DBA) report parameters. */
/*             This API is only supported under special MPCP mode. The structure is defined:   */
/* typedef struct { */
/*    cs_uint16  t_threshold; */           // (default=774) 0~0xffff
/*    cs_uint8   zeroq_map; */             // (default=0) zeroed queue map
/*    cs_uint16  zeroq_threshold; */       // (default=65535) zeroed queue threshold
/*    cs_boolean aggregation_mode_en; */   // (default=true) True: Q-set threshold "T" is aggregated thresholds for reporting
                                      // false: Q-set threshold "T" is independent thresholds for reporting
/*    cs_aal_mpcp_rept_fmt_e   report_format; */ // (default=AAL_MPCP_REPT_FMT_0) report format
/*    cs_aal_mpcp_rept_order_e rpt_order; */     // (default=AAL_MPCP_REPT_ORDER_REVRS) LynxD Mode Reporting Order Select
/*    cs_aal_mpcp_rept_num_e   rpt_number; */    // (default=AAL_MPCP_REPT_NUM_2) MPCP Reporting Number
/*    cs_uint8   rpt_bmp_qset1; */         // report bitmap for qset1
/*    cs_uint8   rpt_bmp_qset2; */         // report bitmap for qset2
/*    cs_boolean window_fit_en; */         // (default=true) True : schedule a lower-priority packet if the packet from higher priority can not
                                      //    fit in the remaining grant window; */
                                      // False: Disable, stop egress traffic if the packet from the higest priority queue
                                      //    can not fit into the remaining grant window
/*    cs_boolean quick_rpt_en; */          // (default=false) True: force quick report, False: no force quick report
/*    cs_boolean ignore_rpt_en; */         // 1: The egress scheduler ignors the content (QSet2 and QSet1) of MPCP reportand just dequeues packet on SP basis
                                      // 0: Strictly follows the content (QSet2 and QSet1) of MPCP report
/*    cs_boolean single_window_en; */      // 1 : The report and data are in single window; */
                                      // 0 :The report and data are in seperate window; */
                                      //  Note : This only works with force_quick_rpt_en=1; */    
/*    cs_boolean cs_mode_en; */             //  Enable K-mode report    
/*    cs_aal_mpcp_kmode_rpt_opt_e cs_mode_opt; */ // 1 : The lump-sum of all queues is reported as Q7
                                            // 0 : The lump-sum of all queues is reported as Q0; */
/*    cs_uint16  cs_mode_threshold; */      //  0~0xffff
/*    cs_boolean le_mode_en; */             //  Enable C-mode report
/*    cs_aal_mpcp_cmode_rpt_opt_e le_mode_opt; */ //  0 : 2 Q set mode, each queue set has 8 priorities.
                                            // 1: One queue set mode, the bitmap is specified at "c_mode_bmp"
/*    cs_uint8   le_mode_bmp; */                  // One queue set bitmap, see "c_mode_opt"
/* }cs_aal_mpcp_report_cfg_t; */
/* $rtn_hdr_end                                   */
/*****************************************************************************/

/*****************************************************************************/
/* $rtn_hdr_start   Get MPCP (DBA) Report Parameters                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_report_cfg_get(
    CS_OUT cs_aal_mpcp_report_cfg_t *cfg
);
/* INPUTS    : ----                                                         */
/* OUTPUTS   : o  cfg - MPCP (DBA) report configuration                   */
/* RETURNS   : o  CS_OK: Get successfully                                   */
/*              o  CS_ERROR: Get failed                                   */
/* DESCRIPTION:                                                              */
/*             This API is used to  retrieve MPCP (DBA) report parameters. This API is only supported under special MPCP mode.    */
/*                                                                           */
/* $rtn_hdr_end                                */
/*****************************************************************************/

/*****************************************************************************/
/* $rtn_hdr_start   Set MPCP Timing Control Parameters                                 */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_timer_cfg_set(
    CS_IN cs_aal_mpcp_timer_msk_t msk,
    CS_IN cs_aal_mpcp_timer_cfg_t *cfg
);
/* INPUTS     : o  msk - Parameter mask                   */
/* o  cfg - Configure MPCP timing control parameters                   */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_ERROR: other error occurs                      */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_OK: set successfully                                   */
/* DESCRIPTION:                                                              */
/*             This API is used to  configure MPCP timing control parameters  */
/*             Related structure is defined as following:*/
/* typedef struct { */
/*     cs_boolean gatetimer_en;         // (default=true) Enable gate timer or not */
/*     cs_boolean mpcptimer_en;         // (default=true) Enable mpcp timer or not */
/*     cs_boolean gatetimer_dereg_en;   // (default=true) Enable derigistration when gate timer expiration */
/*     cs_boolean mpcptimer_dereg_en;   // (default=true) Enable deregistration when mpcp timer expiration */
/*     cs_boolean timedrift_dereg_en;   // (default=true) Enable deregistration when timestamp drift detection */
/*     cs_uint16  mpcptimer_threshold;  // 0~4093(default=1000): config the threshold for mpcp timer expiration in unit of ms */
/*     cs_uint16  gatetimer_threshold;  // 0~4093(default=50): config the threshold for gate timer expiration in unit of ms */
/*     cs_uint8   timedrift_threshold;  // 0~255(default=8): config the threshold for timestamp drift detection in unit of TQ */
/*     cs_uint16  reporttimer_threshold;// 0~4095(default=50ms) config the threshold for report timer in unit of ms */
/* } cs_aal_mpcp_timer_cfg_t; */
/* $rtn_hdr_end                                   */
/*****************************************************************************/

/*****************************************************************************/
/* $rtn_hdr_start   Get MPCP Timing Control Parameters                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_timer_cfg_get(
    CS_OUT cs_aal_mpcp_timer_cfg_t *cfg
);
/* INPUTS    : ----                                                         */
/* OUTPUTS   : o  cfg - MPCP timing control configuration                   */
/* RETURNS   : o  CS_OK: Get successfully                                   */
/*              o  CS_ERROR: Get failed                                   */
/* DESCRIPTION:                                                              */
/*             This API is used to  retrieve MPCP timing control parameters   */
/*                                                                           */
/* $rtn_hdr_end                                */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set MPCP misc configuration                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_misc_cfg_set(
    CS_IN cs_aal_mpcp_misc_msk_t msk,
    CS_IN cs_aal_mpcp_misc_cfg_t *cfg
);
/* INPUTS     : o  msk - Parameter mask                   */
/* o  cfg - Configure MPCP misc control parameters                   */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_ERROR: other error occurs                      */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_OK: set successfully                                   */
/* DESCRIPTION:                                                              */
/*             This API is used to  configure MPCP misc parameters  */
/*             Related structure is defined as following:*/
/* typedef struct { */
/*    cs_aal_mpcp_pending_grant_e     pending_grant; */             // Read-only: 1~8 is valid
/*    cs_uint8     min_grant_size; */            // recommand set to 0x2a (64+8+12)byte = 42TQ
/*    cs_aal_mpcp_req_llid_e     request_frame_llid; */    // 1: assigned; */ 0: 0x7fff
/*    cs_boolean   mpcp_frame_len_check_en; */   // 1: only 64byte accepted; */ 0: accept >64byte mpcp frame
/*    cs_boolean   min_process_time_check_en; */ // 1: ONU will compare start time and local timestamp
/*    cs_uint16    min_process_time; */          // config min processing time, default is 1024 (protocol also defined 1024)
/*    cs_uint16    max_future_grant_time; */     // if (StartTime-LocalTime)>THIS, ignore this grant, default is 1024
/*    cs_uint8     discovery_pkt_size; */        // unit is TQ, exclude FEC overhead
/*    cs_uint8     mpcp_frame_fec_overhead; */   // Used when TxFEC is enabled.Unit is TQ. This fixed value should be 13+8 TQ
/*    cs_boolean   onu_mac_addr_check_en; */     // 0: ONU doesn't check whether DA in receiving MPCP frame matches with ONUs MAC address
                                            // 1: ONU checks every receiving MPCP frames DA,
                                            //    (01-80-c2-00-00-01) and DA matched with ONUs MAC address can be accepted
/*    cs_uint8     mpcp_frame_fec_overhead; */   // 0~255. Unit is TQ. used when TxFEC is enabled
/*    cs_boolean   strict_starttime_check_en; */ // Only when one grant's start time exactly matches local timer, that grant window can be started
/*    cs_aal_mpcp_rereg_mode_e    rereg_mode; */ // 1: when ONU receives Register frame with rereg flag, keep ONU registered.
                                            // 0: when ONU receives Register frame with rereg flag, deregister ONU.
/*    cs_boolean   force_rpt_data_en; */         // 1: allow BM transmit user data packet in force-report grant window if there are extra window space
                                            // 0: not allow BM transmit user data packet in force-report grant window
/*    cs_uint8     tx_window_adjust_time; */     // debug only
/*    cs_aal_mpcp_ts_up_loc_e     timestamp_update_location; */ // debug only
/*    cs_boolean   direct_registration_en; */    // Allow the MPCP engine to accept a registration frame even if it is in the WAIT state
/*    cs_uint8     drain_window_guard_time; */   // debug only
/*    cs_boolean   mpcp_to_cpu_en; */            // debug only
/* } cs_aal_mpcp_misc_cfg_t; */
/* $rtn_hdr_end                                   */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get MPCP misc  Control Parameters                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_misc_cfg_get(
    CS_OUT cs_aal_mpcp_misc_cfg_t *cfg
);
/* INPUTS    : ----                                                         */
/* OUTPUTS   : o  cfg - MPCP misc control configuration                   */
/* RETURNS   : o  CS_OK: Get successfully                                   */
/*              o  CS_ERROR: Get failed                                   */
/* DESCRIPTION:                                                              */
/*             This API is used to  retrieve MPCP misc control parameters   */
/*                                                                           */
/* $rtn_hdr_end                                */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set MPCP RTT compensation configuration                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : MPCP                                                      */
/* CHIP       : CS8030                                                      */
cs_status aal_mpcp_rtt_comp_cfg_set(
    CS_IN cs_aal_mpcp_rtt_comp_msk_t msk,
    CS_IN cs_aal_mpcp_rtt_comp_cfg_t *cfg
);
/* INPUTS     : o  msk - Parameter mask                   */
/* o  cfg - Configure MPCP RTT compensation parameters                   */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_ERROR: other error occurs                      */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_OK: set successfully                                   */
/* DESCRIPTION:                                                              */
/*             This API is used to  configure MPCP misc parameters  */
/*             Related structure is defined as following:*/
/*typedef struct { */
/*    cs_aal_mpcp_compan_mode_e       mode; */ // specified the compensation scheme mode
/*    cs_boolean               fec_comp_en; */
/*    cs_boolean       fec_comp_cnt_inc_en; */
/*    cs_uint16               fec_comp_cnt; */
/*    cs_uint8                  txmac_comp; */ // when mode == AAL_MPCP_COMPAN_MODE_WITH_AES, invalid
/*    cs_uint8            egress_path_comp; */ // when mode == AAL_MPCP_COMPAN_MODE_WITH_AES, invalid
/*    cs_uint8                  rxmac_comp; */ // when mode == AAL_MPCP_COMPAN_MODE_WITH_AES, invalid
/*    cs_uint8              rxfec_aes_comp; */
/*    cs_uint8            rxfec_churn_comp; */
/*    cs_uint8          non_rxfec_aes_comp; */
/*    cs_uint8        non_rxfec_churn_comp; */
/* } cs_aal_mpcp_rtt_comp_cfg_t; */
/* $rtn_hdr_end                                   */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get MPCP RTT Conpensation Parameters                     */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : MPCP                                                      */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_rtt_comp_cfg_get(
    CS_OUT cs_aal_mpcp_rtt_comp_cfg_t *cfg
);
/* INPUTS    : ----                                                         */
/* OUTPUTS   : o  cfg - MPCP RTT compensation configuration                  */
/* RETURNS   : o  CS_OK: Get successfully                                   */
/*              o  CS_ERROR: Get failed                                   */
/* DESCRIPTION:                                                              */
/*             This API is used to  retrieve MPCP RTT compensation parameters   */
/*                                                                           */
/* $rtn_hdr_end                                */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set ONU grant permit status                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : MPCP                                                         */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_grant_permit_set(
    CS_IN cs_boolean permit
);
/* INPUTS    : permit - 1: Normal; 0: discard any received grants            */
/* OUTPUTS   : o NA                                                          */
/* RETURNS   : o  CS_OK: Get successfully                                    */
/*             o  CS_ERROR: Get failed                                       */
/* DESCRIPTION:                                                              */
/*             This API is used to set grant permit status                   */
/*                                                                           */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get ONU grant permit status                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : MPCP                                                         */
/* CHIP        : CS8030                                                      */
cs_status aal_mpcp_grant_permit_get(
    CS_OUT cs_boolean *permit
);
/* INPUTS    : o NA                                                          */
/* OUTPUTS   : o NA                                                          */                                                 
/* RETURNS   : o  permit - 1: Normal; 0: discard any received grants         */
/* DESCRIPTION:                                                              */
/*             This API is used to get grant permit status                   */
/*                                                                           */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/

cs_status aal_mpcp_llid_get(cs_uint16 *llid);
cs_status aal_mpcp_reg_toggle(void);
cs_status aal_mpcp_reg_req_rsv0(cs_uint32 data);
cs_status aal_mpcp_reg_req_rsv1(cs_uint32 data);
cs_status aal_mpcp_reg_flag_get(cs_uint8 *flag);


#endif /* __AAL_MPCP_H__ */

