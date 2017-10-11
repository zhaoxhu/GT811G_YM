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
#ifndef __AAL_BM__UTIL_H__
#define __AAL_BM__UTIL_H__

/*

    This file is used for buffer management and scheduling code
    Private part
    
*/

#define __BM_FLUSH_CHECK_TIME   100000
#define __BM_INIT_DONE_TIME     50000
#define __BM_KB_NUM             1024 * 68/65
#define __BM_NUM_KB             65*12/1024/68
#define __BM_DEF_WGT_BASE       3
#define __BM_MAX_QUE_NUM        4
#define __BM_MAX_BUF_SIZE       512

typedef enum{
    __MEMORY_MODE,
    __FIFO_MODE    
}__bm_mode_t;

typedef struct {
    cs_uint32 bm_mode              :1 ; /** 0 - FIFO mode, 1 -  Memory mode */                                       
    /*sch_cfg */                      
    cs_uint32 ge_ql_en             :1 ; /** GE Port Egress Queue Maximum Length Limit Enable **/
    cs_uint32 pon_ql_en            :1 ; /** PON Port Egress Queue Maximum Length Limit Enable **/
    cs_uint32 cpu_ql_en            :1 ; /** CPU Port Egress Queue Maximum Length Limit Enable **/
    cs_uint32 voq8_en              :1 ; /** Special Queue (the 9th Queue) Enable **/
    cs_uint32 ge_cpu_pkt_en        :1 ; /** GE Port Special Packet Handling **/
    cs_uint32 pon_cpu_pkt_en       :1 ; /** PON Port Special Packet Handling **/
    cs_uint32 dis_win_fit_sp_rsvd  :1 ; /** Disable Window Fitting in Strict Priority Mode **/
    cs_uint32 dis_win_fit_nsp_rsvd :1 ; /** Disable Window Fitting in Non-Strict Priority Mode **/
    cs_uint32 burst_mode_rsvd      :1 ; /** Burst Mode Enable **/
    cs_uint32 kt_mode_rsvd         :1 ; /** This was used for some legacy KT ONUs which 
                                            support Pause Frame. Now it is
                                            for debug only, please do not change this bit **/
    cs_uint32 us_dp_en             :1 ; /** Upstream Dynamic Partitioning Enable **/
    cs_uint32 ds_dp_en             :1 ; /** Downstream Dynamic Partitioning Enable **/
    cs_uint32 us_abr_en            :1 ; /** Upstream ABR Enable **/
    cs_uint32 ds_abr_en            :1 ; /** Downstream ABR Enable **/
    cs_uint32 unknown_dpid         :2 ; /** Default DPID for Packet with Unknow DPID **/                                                                            
    /* shaper_cfg */                  
    cs_uint32 cpu_cnt_option_rsvd  :1 ; /** CPU Port Counter Option **/
    cs_uint32 cpu_shaper_en        :1 ; /** CPU Port Shaper Enable **/
    cs_uint32 pon_shaper_en        :1 ; /** PON Port Shaper Enable **/
    cs_uint32 ge_shaper_en         :1 ; /** GE Port Shaper Enable **/
    cs_uint32 w_rate               :5 ; /** Wire Rate Offset **/
    /* mpcp cfg */                    
    cs_uint32 win_chk_en_rsvd      :1 ; /** MPCP Window Check Enable **/
    cs_uint32 edwrr_en_rsvd        :1 ; /** Enhanced-DWRR Scheduling Enable **/
    cs_uint32 ddr_weight_base      :2 ; /** DRR Weight Base used for DRR scheduler in GE and PON Schedulers **/
    cs_uint32 resv1                :2 ;


    /* share_buf */                   
    cs_uint32 ppbm_en_cpu          :1 ; /** Per Port Buffer Mode Enable for CPU Port **/
    cs_uint32 ppbm_en              :1 ; /** Per Port Buffer Mode Enable for GE and PON Port **/
    cs_uint32 slmt_cpu             :14; /** Shared buffer Drop limit for Packet to CPU **/
    cs_uint32 slmt                 :14; /** Shared Buffer Limit **/
    cs_uint32 resv2                :2 ;

    
                                    
    /* abr cfg */                     
    cs_uint32 abr_ds_thrsld        :14; /** Downstream ABR Threshold **/
    cs_uint32 abr_us_thrsld        :14; /** Upstream ABR Threshold **/
    cs_uint32 resv3                :4 ;

    
    /* dp cfg */                      
    cs_uint32 dp_ds_thrsld         :14; /** Downstream DP Threshold **/
    cs_uint32 dp_us_thrsld         :14; /** Upstream DP Threshold **/
    cs_uint32 resv4                :4 ;
                                     

    /* tx buff limit */
    cs_uint32 pon_tx_lmt_en        :1 ; /** PON Port Transmit Buffer Limit Enable **/
    cs_uint32 pon_tx_buffer_thrsld :14; /** PON Port Transmit Buffer Limit Threshold **/
    cs_uint32 ge_tx_lmt_en         :1 ; /** GE Port Transmit Buffer Limit Enable **/
    cs_uint32 ge_tx_buffer_thrsld  :14; /** GE Port Transmit Buffer Limit Threshold **/
    cs_uint32 resv5                :2 ;

    cs_uint32 linklist_size        :14; /** Total size of the BM linklis **/
    cs_uint32 resv6                :18 ;
    
    
}__bm_ext_cfg_t;

typedef union {
    struct {
    cs_uint32 bm_mode              :  1 ;   
    //sch cfg
    cs_uint32 dp_option            :  1 ; 
    cs_uint32 dis_self_reset       :  1 ;
    cs_uint32 unknown_dpid         :  1 ; 
    cs_uint32 ds_abr_en            :  1 ;     
    cs_uint32 us_abr_en            :  1 ; 
    cs_uint32 ds_dp_en             :  1 ; 
    cs_uint32 us_dp_en             :  1 ; 
    cs_uint32 kt_mode_rsvd         :  1 ;   
    cs_uint32 burst_mode_rsvd      :  1 ; 
    cs_uint32 dis_win_fit_nsp_rsvd :  1 ; 
    cs_uint32 dis_win_fit_sp_rsvd  :  1 ; 
    cs_uint32 pon_cpu_pkt_en       :  1 ;   
    cs_uint32 ge_cpu_pkt_en        :  1 ; 
    cs_uint32 voq8_en              :  1 ; 
    cs_uint32 cpu_ql_en            :  1 ;   
    cs_uint32 pon_ql_en            :  1 ;     
    cs_uint32 ge_ql_en             :  1 ; 
    //share buf
    cs_uint32 ppbm_en_cpu          :  1 ; 
    cs_uint32 ppbm_en              :  1 ;
    cs_uint32 slmt_cpu             :  1 ; 
    cs_uint32 slmt                 :  1 ;     
    //shaper_cfg
    cs_uint32 cpu_cnt_option_rsvd  :  1 ; 
    cs_uint32 cpu_shaper_en        :  1 ;    
    cs_uint32 pon_shaper_en        :  1 ; 
    cs_uint32 ge_shaper_en         :  1 ;
    cs_uint32 w_rate               :  1 ;
    //abr cfg
    cs_uint32 abr_ds_thrsld        :  1 ; 
    cs_uint32 abr_us_thrsld        :  1 ;
    // dp cfg
    cs_uint32 dp_ds_thrsld         :  1 ; 
    cs_uint32 dp_us_thrsld         :  1 ; 
    //mpcp
    cs_uint32 win_chk_en_rsvd      :  1 ;


    cs_uint32 edwrr_en_rsvd        :  1 ;
    cs_uint32 linklist_size        :  1 ;
    cs_uint32 pon_tx_lmt_en        :  1 ;
    cs_uint32 pon_tx_buffer_thrsld :  1 ;
    cs_uint32 ge_tx_lmt_en         :  1 ;
    cs_uint32 ge_tx_buffer_thrsld  :  1 ;
    cs_uint32 ddr_weight_base      :  1 ;
    cs_uint32 resvr                :  25;
    } s;
    cs_uint64 u;
}__bm_ext_cfg_msk_t;


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
}__bm_port_tx_ctrl_t;

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
}__bm_port_tx_ctrl_msk_t;

#endif /*End __AAL_BM__UTIL_H__*/


