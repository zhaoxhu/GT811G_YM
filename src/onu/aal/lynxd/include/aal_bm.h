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

    This file is used for buffer management and scheduling code
    
*/

#ifndef __AAL_BM_H__
#define __AAL_BM_H__

#include "cs_types.h"
#include "aal.h"
#include "aal_bm_util.h"

typedef enum{
    ONU_BM_QUEUE_US_0 = 0,
    ONU_BM_QUEUE_US_1 = 1,
    ONU_BM_QUEUE_US_2 = 2,
    ONU_BM_QUEUE_US_3 = 3,
    ONU_BM_QUEUE_DS_0 = 4,
    ONU_BM_QUEUE_DS_1 = 5,
    ONU_BM_QUEUE_DS_2 = 6,
    ONU_BM_QUEUE_DS_3 = 7,
}cs_aal_bm_static_queue_t;

typedef enum{
    ONU_BM_SCHED_SP     = 0,
    ONU_BM_SCHED_SP_WRR = 1,
    ONU_BM_SCHED_WRR    = 2,
}cs_aal_bm_sched_type_t;

typedef enum{
    ONU_BM_SCHED_WEI_32B  = 0,  //32Bytes weight base
    ONU_BM_SCHED_WEI_64B  = 1,  //64Bytes weight base
    ONU_BM_SCHED_WEI_128B = 2,  //128Bytes weight base
    ONU_BM_SCHED_WEI_256B = 3,  //256Bytes weight base
}cs_aal_bm_sched_weight_t;


typedef union {
    struct {
        cs_uint32  direction:                1;
        cs_uint32  ge_td_drop_en:            1;
        cs_uint32  pon_td_drop_en:           1;
        cs_uint32  ge_drop_thrshld:          8; /**** Bit[0~7] represent Class[0-7] threshold */
        cs_uint32  pon_drop_thrshld:         8; /**** Bit[0~7] represent Class[0-7] threshold */
        cs_uint32  resrv:                    13;
    } s;
    cs_uint32 u32;
}cs_aal_td_cfg_msk_t;


/* TD config stucture */
typedef struct {
    cs_uint8  direction;          /****if 0, TD select single direction buffer, if 1, TD select bi-direction buffer, ********/
    cs_uint8  ge_td_drop_en;      /****if 1, GE port TD is enable, if 0, GE port TD is disable******/
    cs_uint8  pon_td_drop_en;     /****if 1, PON port TD is enable, if 0, PON port TD is disable******/
    cs_uint8  ge_drop_thrshld[8]; /**** GE Class [0-7] all drop threshold ****/
    cs_uint8  pon_drop_thrshld[8];/**** PON Class [0-7] all drop threshold ****/
}cs_aal_td_cfg_t;

#define CS_AAL_BM_Q_SIZE_MAX    4
typedef struct {
    cs_uint16  qx_size[CS_AAL_BM_Q_SIZE_MAX];  /***queue 0~3 buffer size****/
}cs_aal_bm_q_size_t;

/* Static Cfg stucture */
typedef struct {
    cs_aal_bm_q_size_t  us;  /***us queue buffer size****/
    cs_aal_bm_q_size_t  ds;  /***ds queue buffer size****/
}cs_aal_bm_static_cfg_t;

typedef union {
    struct {
        cs_uint32  us:                1;
        cs_uint32  ds:                1;
        cs_uint32  resrv:             30;
    } s;
    cs_uint32 u32;
}cs_aal_bm_static_cfg_msk_t;


typedef struct {
    cs_uint64  fe_drp_cnt;       /***statistic of FE drop packets, ingress drop****/
    cs_uint64  bm_drp_cnt;       /***statistic of BM drop packets, egress drop****/
    cs_uint32  bm_pkt_cnt[4];    /***packets number in each queue****/
}cs_aal_bm_stats_t;

typedef struct{
    cs_aal_bm_sched_type_t mode;
    cs_uint8 weight[8];
}cs_aal_bm_weight_t;

/* Queue Val stucture */
typedef struct {
    cs_uint8  val[8];             /***queue 0~7 val to be set****/
}cs_aal_bm_queue_val_t;

typedef union {
    struct {
        cs_uint32  queue:     8;  /**** Bit[0~7] represent Queue[0~7] mask to be set ***/
        cs_uint32  resrv:     24;
    } s;
    cs_uint32 u32;
}cs_aal_bm_queue_msk_t;

typedef struct{
    cs_boolean  ge_intI;
    cs_boolean  pon_intI;
    cs_boolean  cpu_intI;
}cs_aal_bm_intr_status_t;

typedef struct{
    cs_uint32  stop_cpu_rx       :  1;   /**  Stop MA port receiving: 1-stop receiving, 0-enable**/
    cs_uint32  stop_pon_rx       :  1;   /**  Stop PON port receiving: 1-stop receiving, 0-enable **/
    cs_uint32  stop_ge_rx        :  1;   /**  Stop GE port receiving: 1-stop receiving, 0-enable **/
    cs_uint32  cpu_tx_en         :  8;   /**  CPU Queue[0~7] transmission status: 
                                             0-disable COSx queue for transmission; 
                                             1-enable COSx queue fortransmission; **/
    cs_uint32  pon_tx_en         :  8;   /**  PON Queue[0~7] transmission status: 
                                             0-disable COSx queue for transmission; 
                                             1-enable COSx queue fortransmission; **/
    cs_uint32  ge_tx_en          :  8;   /**  GE Queue[0~7] transmission status: 
                                             0-disable COSx queue for transmission; 
                                             1-enable COSx queue fortransmission; **/
    cs_uint32  resrv             :  5;
}cs_aal_bm_port_tx_ctrl_t;

typedef union {
    struct {
        cs_uint32  stop_cpu_rx      :  1 ; 
        cs_uint32  stop_pon_rx      :  1 ; 
        cs_uint32  stop_ge_rx       :  1 ; 
        cs_uint32  cpu_tx_en        :  8; /**** Bit[0~7] represent CPU Queue[0-7] ***/
        cs_uint32  pon_tx_en        :  8; /**** Bit[0~7] represent PON Queue[0-7] ***/
        cs_uint32  ge_tx_en         :  8; /**** Bit[0~7] represent GE Queue[0-7] ***/
        cs_uint32  resrv            :  5;
    } s;
    cs_uint32 u32;
}cs_aal_bm_port_tx_ctrl_msk_t;

typedef volatile union 
{
    struct
    {
#ifdef CS_BIG_ENDIAN 
    cs_uint32 pon_rx_no_buf_drop                 :  1 ; 
    cs_uint32 pon_rx_ql_drop                     :  1 ;
    cs_uint32 pon_rx_dp_lmt_drop                 :  1 ; 
    cs_uint32 pon_rx_non_cpu_drop                :  1 ; 
    cs_uint32 ge_rx_no_buf_dropE                 :  1 ; 
    cs_uint32 ge_rx_ql_drop                      :  1 ; 
    cs_uint32 ge_rx_dp_lmt_drop                  :  1 ; 
    cs_uint32 ge_rx_non_cpu_drop                 :  1 ; 
    cs_uint32 rsrvd1                             :  3 ;
    cs_uint32 pon_eee_wake_up                    :  1 ; 
    cs_uint32 pon_eee_sleep                      :  1 ; 
    cs_uint32 eee_wake_up                        :  1 ; 
    cs_uint32 eee_sleep                          :  1 ; 
    cs_uint32 header_error_corrected             :  1 ; 
    cs_uint32 unexpected_quick_report_dectection :  1 ; 
    cs_uint32 hdmem_collision_dectection         :  1 ; 
    cs_uint32 q2mem_collision_dectection         :  1 ; 
    cs_uint32 ge_rmu_non_idle_aft_eop_error      :  1 ; 
    cs_uint32 ge_rmu_fsm_non_idle_error          :  1 ; 
    cs_uint32 ge_rmu_collision_error             :  1 ; 
    cs_uint32 pon_rmu_non_idle_aft_eop_error     :  1 ; 
    cs_uint32 pon_rmu_fsm_non_idle_error         :  1 ; 
    cs_uint32 pon_rmu_collision_error            :  1 ; 
    cs_uint32 cpu_rmu_non_idle_aft_eop_error     :  1 ;
    cs_uint32 cpu_rmu_fsm_non_idle_error         :  1 ; 
    cs_uint32 cpu_rmu_collision_error            :  1 ; 
    cs_uint32 header_error                       :  1 ; 
    cs_uint32 cpu_tx_crc_error                   :  1 ; 
    cs_uint32 pon_tx_crc_error                   :  1 ;
    cs_uint32 ge_tx_crc_error                    :  1 ;
#else
    cs_uint32 ge_tx_crc_error                    :  1 ;
    cs_uint32 pon_tx_crc_error                   :  1 ;
    cs_uint32 cpu_tx_crc_error                   :  1 ; 
    cs_uint32 header_error                       :  1 ; 
    cs_uint32 cpu_rmu_collision_error            :  1 ; 
    cs_uint32 cpu_rmu_fsm_non_idle_error         :  1 ; 
    cs_uint32 cpu_rmu_non_idle_aft_eop_error     :  1 ;
    cs_uint32 pon_rmu_collision_error            :  1 ; 
    cs_uint32 pon_rmu_fsm_non_idle_error         :  1 ; 
    cs_uint32 pon_rmu_non_idle_aft_eop_error     :  1 ; 
    cs_uint32 ge_rmu_collision_error             :  1 ; 
    cs_uint32 ge_rmu_fsm_non_idle_error          :  1 ; 
    cs_uint32 ge_rmu_non_idle_aft_eop_error      :  1 ; 
    cs_uint32 q2mem_collision_dectection         :  1 ; 
    cs_uint32 hdmem_collision_dectection         :  1 ; 
    cs_uint32 unexpected_quick_report_dectection :  1 ; 
    cs_uint32 header_error_corrected             :  1 ; 
    cs_uint32 eee_sleep                          :  1 ; 
    cs_uint32 eee_wake_up                        :  1 ; 
    cs_uint32 pon_eee_sleep                      :  1 ; 
    cs_uint32 pon_eee_wake_up                    :  1 ; 
    cs_uint32 rsrvd1                             :  3 ;
    cs_uint32 ge_rx_non_cpu_drop                 :  1 ; 
    cs_uint32 ge_rx_dp_lmt_drop                  :  1 ; 
    cs_uint32 ge_rx_ql_drop                      :  1 ; 
    cs_uint32 ge_rx_no_buf_dropE                 :  1 ; 
    cs_uint32 pon_rx_non_cpu_drop                :  1 ; 
    cs_uint32 pon_rx_dp_lmt_drop                 :  1 ; 
    cs_uint32 pon_rx_ql_drop                     :  1 ;
    cs_uint32 pon_rx_no_buf_drop                 :  1 ; 
#endif
    }s;
    cs_uint32 u32;
} cs_aal_bm_int_msk_t;

/*==================Functions=================*/

cs_status aal_bm_share_queue_max_length_set(
    CS_IN cs_aal_port_id_t port,
    CS_IN cs_aal_bm_queue_msk_t mask,
    CS_IN cs_aal_bm_queue_val_t *cfg
);

cs_status aal_bm_share_queue_max_length_get(
    CS_IN CS_IN cs_aal_port_id_t port,
    CS_OUT cs_aal_bm_queue_val_t *cfg
);

cs_status aal_bm_static_cfg_set(
    CS_IN cs_aal_bm_static_cfg_msk_t mask,
    CS_IN cs_aal_bm_static_cfg_t *cfg
);

cs_status aal_bm_static_cfg_get(
    CS_OUT cs_aal_bm_static_cfg_t *cfg
);

cs_status aal_bm_share_wrr_weight_base_set(
    CS_IN cs_aal_bm_sched_weight_t weight_base
);

cs_status aal_bm_share_wrr_weight_base_get(
    CS_IN cs_aal_bm_sched_weight_t *weight_base
);

cs_status aal_bm_share_scheduler_set(
    CS_IN cs_aal_port_id_t port,
    CS_IN cs_aal_bm_weight_t *weight
);

cs_status aal_bm_share_scheduler_get(
    CS_IN cs_aal_port_id_t port,
    CS_OUT cs_aal_bm_weight_t *weight
);

cs_status aal_bm_static_queue_flush(
    CS_IN cs_aal_port_id_t port,
    CS_IN cs_aal_bm_static_queue_t queue
);

cs_status aal_bm_share_buffer_flush(
    CS_IN cs_aal_port_id_t port
);

cs_status aal_bm_stats_get(
    CS_IN cs_aal_port_id_t port_id, 
    CS_OUT cs_aal_bm_stats_t *bm_stats
);

cs_status aal_td_cfg_set
(
    CS_IN cs_aal_td_cfg_msk_t msk,
    CS_IN cs_aal_td_cfg_t *cfg
);

cs_status aal_td_cfg_get
(
    CS_OUT cs_aal_td_cfg_t *cfg
);

cs_status aal_bm_int_enable(
    CS_IN  cs_aal_bm_int_msk_t mask
);

cs_status aal_bm_int_disable(
    CS_IN  cs_aal_bm_int_msk_t mask
);

cs_status aal_bm_int_en_get(
    CS_IN  cs_aal_bm_int_msk_t *mask
);

cs_uint32 aal_bm_drp_cnt_get(void);

cs_status aal_bm_int_status_get(
    CS_OUT cs_aal_bm_int_msk_t* mask
);

cs_status aal_bm_int_status_clr(
    CS_IN  cs_aal_bm_int_msk_t mask
);
void aal_bm_static_mode_init(
    cs_uint8 cpu_acc_ram_mode
);
cs_status __bm_port_tx_ctrl_set(__bm_port_tx_ctrl_msk_t mask, __bm_port_tx_ctrl_t *cfg);


cs_status __bm_port_tx_ctrl_get(__bm_port_tx_ctrl_t *cfg);


void aal_bm_share_mode_init(void);

cs_status aal_eee_save(void);
cs_status aal_eee_restore(void);
cs_status aal_eee_thre_set(cs_uint8 que, cs_uint16 thre);
cs_boolean aal_eee_stauts_get(cs_uint8 que);

cs_uint32 aal_get_bm_cnt(cs_uint32 address);
void  aal_clr_bm_drop(cs_uint8  index);

void aal_bm_de_register_handler(void);
void aal_bm_register_handler(void);
cs_status aal_bm_port_buf_size_set(
    CS_IN cs_aal_port_id_t port,
    CS_IN cs_uint16        size
 );
cs_status aal_bm_port_buf_size_get(
    CS_IN cs_aal_port_id_t port,
    CS_OUT cs_uint16       *size
 );
cs_status aal_bm_ma_dpid_cos_set(cs_uint8 dpid, cs_uint8 ing_cos, cs_uint8 map_que);



#endif /* __AAL_BM_H__ */

