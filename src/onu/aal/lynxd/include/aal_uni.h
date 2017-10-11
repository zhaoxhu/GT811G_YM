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
 
Copyright (c) 2010 by Cortina Systems Incorporated
****************************************************************************/

/*

    This file is used for UNI MAC driver code
    
*/
#include "cs_types.h"

#ifndef __AAL_UNI_H__
#define __AAL_UNI_H__

/*UNI MAC interrupt macro*/
#define   AAL_UNI_INT_LINK_CHANGE                0x00000001
#define   AAL_UNI_INT_TX_UNDERRUN                0x00000002
#define   AAL_UNI_INT_TX_OVERRUN                 0x00000004
#define   AAL_UNI_INT_RM_OVERRUN                 0x00000008
#define   AAL_UNI_INT_MIB_MSB_TRIGER             0x00000020
#define   AAL_UNI_INT_PTP_IN_LATCHED             0x00010000
#define   AAL_UNI_INT_PTP_OUT_LATCHED            0x00020000
#define   AAL_UNI_INT_PHY                        0x80000000


typedef enum {
    
    AAL_UNI_SPEED_10M                =  0x0, 
    AAL_UNI_SPEED_100M               =  0x1, 
    AAL_UNI_SPEED_1000M              =  0x2 

} cs_aal_uni_speed_t;


typedef enum {
    AAL_MIB_RxUCPktCnt               = 0x0 ,
    AAL_MIB_RxMCFrmCnt               = 0x1 ,
    AAL_MIB_RxBCFrmCnt               = 0x2 ,
    AAL_MIB_RxOAMFrmCnt              = 0x3 ,
    AAL_MIB_RxPauseFrmCnt            = 0x4 ,
    AAL_MIB_RxUnKnownOCFrmCnt        = 0x5 ,
    AAL_MIB_RxCrcErrFrmCnt           = 0x6 ,
    AAL_MIB_RxUndersizeFrmCnt        = 0x7 ,
    AAL_MIB_RxRuntFrmCnt             = 0x8 ,
    AAL_MIB_RxOvSizeFrmCnt           = 0x9 ,
    AAL_MIB_RxJabberFrmCnt           = 0xa ,
    AAL_MIB_RxStatsFrm64Oct          = 0xb ,
    AAL_MIB_RxStatsFrm65to127Oct     = 0xc ,
    AAL_MIB_RxStatsFrm128to255Oct    = 0xd ,
    AAL_MIB_RxStatsFrm256to511Oct    = 0xe ,
    AAL_MIB_RxStatsFrm512to1023Oct   = 0xf ,
    AAL_MIB_RxStatsFrm1024to1518Oct  = 0x10,
    AAL_MIB_RxStatsFrm1519toMaxOct   = 0x11,
    AAL_MIB_RxByteCount              = 0x12,
    AAL_MIB_RxFrmCount               = 0x14,
    /* TX MIB,  NOTE: not continous */
    AAL_MIB_TxUCPktCnt               = 0x20,
    AAL_MIB_TxMCFrmCnt               = 0x21,
    AAL_MIB_TxBCFrmCnt               = 0x22,
    AAL_MIB_TxOAMFrmCnt              = 0x23,
    AAL_MIB_TxPauseFrmCnt            = 0x24,
    AAL_MIB_TxCrcErrFrmCnt           = 0x25,
    AAL_MIB_TxOvSizeFrmCnt           = 0x26,
    AAL_MIB_TxSingleColFrm           = 0x27,
    AAL_MIB_TxMultiColFrm            = 0x28,
    AAL_MIB_TxLateColFrm             = 0x29,
    AAL_MIB_TxExessColFrm            = 0x2a,
    AAL_MIB_TxEStatsFrm64Oct         = 0x2b,
    AAL_MIB_TxEStatsFrm65to127Oct    = 0x2c,
    AAL_MIB_TxEStatsFrm128to255Oct   = 0x2d,
    AAL_MIB_TxEStatsFrm256to511Oct   = 0x2e,
    AAL_MIB_TxEStatsFrm512to1023Oct  = 0x2f,
    AAL_MIB_TxEStatsFrm1024to1518Oct = 0x30,
    AAL_MIB_TxEStatsFrm1519toMaxOct  = 0x31,
    AAL_MIB_TxByteCount              = 0x32,
    AAL_MIB_TxFrmCount               = 0x34
    
}cs_aal_uni_mib_t;

typedef struct {
    cs_boolean tx2rx_enable;
    cs_boolean rx2tx_enable;
} cs_aal_uni_lpbk_mode_t;

typedef struct {

    cs_uint32 rx_en                :  1 ; /* 1 -- enable RX MAC */
    cs_uint32 tx_en                :  1 ; /* 1 -- enable TX MAC */
    cs_uint32 speed                :  2 ; /* 0x0--1000M; 0x1--100M 0x2--10M */// GE_INTF_RST_CONFIG_t  
    cs_uint32 duplex               :  1 ; /* 0--FULL duplex; 1--Half duplex, only for 10/100M speed mode */
    cs_uint32 rx_fc_dis            :  1 ; /* disable flowcontrol feature on RX */
    cs_uint32 tx_fc_dis            :  1 ; /* disable sending pause frame function */
    cs_uint32 max_size             : 11 ; /* maximum packet size allowed form ingress */
    cs_uint32 min_size             :  7 ; /* minimum packet size allowed form ingress */
    cs_uint32 mib_mode             :  1 ; /* 0--Counter 0x0B ~ 0x11 count good data frames only£¬1 = Counter 0x0B ~ 0x11 count all the transmit/receive frames *///GE_ETH_MAC_CONFIG1_t
    cs_uint32 mib_clr_all          :  1 ; /* bits 0:0 *///GE_INIT_t  mib
    cs_uint32 tx_ipg_sel           :  4 ; /* 8 to 16*/
    cs_uint32 rsrvd                :  2 ; 

} cs_aal_uni_cfg_t;

typedef union {
    struct {
        cs_uint32 rx_en            :  1 ; /* 1 -- enable RX MAC */                                                                                                                      
        cs_uint32 tx_en            :  1 ; /* 1 -- enable TX MAC */                                                                                                                      
        cs_uint32 speed            :  1 ; /* 0x0--10M; 0x1--100M 0x2--1000M */// GE_INTF_RST_CONFIG_t                                                                                   
        cs_uint32 duplex           :  1 ; /* 0--FULL duplex; 1--Half duplex, only for 10/100M speed mode */                                                                             
        cs_uint32 rx_fc_dis        :  1 ; /* disable flowcontrol feature on RX */                                                                                                       
        cs_uint32 tx_fc_dis        :  1 ; /* disable sending pause frame function */                                                                                                    
        cs_uint32 max_size         :  1 ; /* maximum packet size allowed form ingress */                                                                                                
        cs_uint32 min_size         :  1 ; /* minimum packet size allowed form ingress */                                                                                                
        cs_uint32 mib_mode         :  1 ; /* 0--Counter 0x0B ~ 0x11 count good data frames only¡ê?1 = Counter 0x0B ~ 0x11 count all the transmit/receive frames *///GE_ETH_MAC_CONFIG1_t
        cs_uint32 mib_clr_all      :  1 ; /* 1-- MIB reset to clear all, only need to set 1, 0 is useless *///GE_INIT_t  mib                                                                                                                
        cs_uint32 tx_ipg_sel       :  1 ; /* TX IPG = 8+tx_ipg_sel, defualt is 4*/                                                                                                      
        cs_uint32 rsrvd            : 21 ;             
    } s ;
    cs_uint32     u32 ;
} cs_aal_uni_cfg_mask_t;


typedef struct {

    cs_uint32 rsrvd2               : 24 ;
    cs_uint32 tck_extra            :  4 ; /* bits 7:4 , TX MAC will send clock_extra plus 9 clocks fter TX MAC start to send Low Power
Idle(LPI) then turn off transmit clock in RGMII mode.*/
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 tck_off              :  1 ; /* bits 1:1 , 	
1: Turn off transmit clock in RGMII mode after TX MAC start to send Low Power
Idle(LPI).*/
    cs_uint32 enable               :  1 ; /* bits 0:0, 	
1: Enable TX MAC start to send Low Power Idle(LPI) when it receive sleep
indication. */

} cs_aal_uni_eee_t;

typedef union {
    struct {
    cs_uint32 rsrvd2               : 29 ;
    cs_uint32 tck_extra            :  1 ; /* bits 7:4 */
    cs_uint32 tck_off              :  1 ; /* bits 1:1 */
    cs_uint32 enable               :  1 ; /* bits 0:0 */
    } s ;
    cs_uint32     u32 ;
} cs_aal_uni_eee_mask_t;


typedef struct {

    cs_uint32 phy_mode             :  1 ; /* bits 4:4     GE_INTF_RST_CONFIG_t */
    cs_uint32 link_stat_mask       :  1 ; /* give out link status led output.                                                                */
    cs_uint32 link_active_mask     :  1 ; /* 1--enable link active led output                                                                */
    cs_uint32 tx_drain             :  1 ; /* drop packets at egress direction                                                                */
    cs_uint32 txfifo_mode          :  1 ; /* 0--start sending out data when FIFO has at least programmed numberof bytes or full frame        */
                                          /*   1--Start sending out data only when FIFO has at least one full frame.                         */
    cs_uint32 txfifo_thrshld       :  9 ; /* sending out data only if (txfifo_thrshld)x4 bytes is written into the FIFO when txfifo_mode is 0*/
    cs_uint32 tx_auto_xon          :  1 ; /* 0 - XON frame is not send when the pause frame generation request isdeasserted                  */
                                          /* 1 - XON frame is send automatically when the pause frame generationrequest is deasserted        */
    cs_uint32 phy_intr_inv         :  1 ; /* Invert PHY interrupt if it is active low                                                        */
    cs_uint32 bypass_header        :  1 ; /* Pass through Header from QM to TXMAC.Header is given out and parse by PTP egress processor      */
    cs_uint32 tx_update_crc        :  1 ; /* bits 29:29                                                                                      */
    cs_uint32 pause_uc_only        :  1 ; /* Pause frame with destination address matched MAC address is accepted */ //GE_RX_MAC_CONFIG0_t
    cs_uint32 tag_exclude          :  1 ; /* Packet length will exclude four bytes TAG while checking maximal length                         */
    cs_uint32 rsrvd                : 12 ; 

    cs_uint32 fifo_thrshld_low     : 11 ; /* bits 10:0 *///GE_TX_CNTRL_CONFIG0_t
    cs_uint32 fifo_thrshld_high    : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd1               : 10 ; 

    cs_uint16 user_tpid0 ; 
    cs_uint16 user_tpid1 ; 
    cs_uint16 user_tpid2 ; 

} cs_aal_uni_cfg_ext_t;

typedef union {
    struct {
        cs_uint32 phy_mode             :  1 ; /*  */
        cs_uint32 link_stat_mask       :  1 ; /* give out link status led output.                                                                */
        cs_uint32 link_active_mask     :  1 ; /* 1--enable link active led output                                                                */
        cs_uint32 tx_drain             :  1 ; /* drop packets at egress direction                                                                */
        cs_uint32 txfifo_mode          :  1 ; /* 0--start sending out data when FIFO has at least programmed numberof bytes or full frame        */
                                              /*   1--Start sending out data only when FIFO has at least one full frame.                         */
        cs_uint32 txfifo_thrshld       :  1 ; /* sending out data only if (txfifo_thrshld)x4 bytes is written into the FIFO when txfifo_mode is 0*/
        cs_uint32 tx_auto_xon          :  1 ; /* 0 - XON frame is not send when the pause frame generation request isdeasserted                  */
                                              /* 1 - XON frame is send automatically when the pause frame generationrequest is deasserted        */
        cs_uint32 phy_intr_inv         :  1 ; /* Invert PHY interrupt if it is active low                                                        */
        cs_uint32 bypass_header        :  1 ; /* bits 28:28                                                                                      */
        cs_uint32 tx_update_crc        :  1 ; /* bits 29:29                                                                                      */
        cs_uint32 pause_uc_only        :  1 ; /* Pause frame with destination address matched MAC address is accepted */ //GE_RX_MAC_CONFIG0_t
        cs_uint32 tag_exclude          :  1 ; /* bits 1:1 */
        cs_uint32 fifo_thrshld_low     :  1 ; /* bits 10:0 *///GE_TX_CNTRL_CONFIG0_t
        cs_uint32 fifo_thrshld_high    :  1 ; /* bits 26:16 */
        cs_uint32 user_tpid0           :  1 ; /* bits 31:16 */
        cs_uint32 user_tpid1           :  1 ; /* bits 15:0 */
        cs_uint32 user_tpid2           :  1 ; /* bits 31:16 */
        cs_uint32 rsrvd                : 15 ; 
    } s ;
    cs_uint32     u32 ;
} cs_aal_uni_cfg_ext_mask_t;

typedef struct {

    cs_uint32 crs_polarity         :  1 ; /* CRS status */
    cs_uint32 link_status          :  1 ; /* 0--Link failed; 1--Link OK */
    cs_uint32 rcv_speed            :  2 ; /* bits 1:0 */
    cs_uint32 rcv_duplex           :  1 ; /* bits 2:2 */
    cs_uint32 rcv_lnk_stat         :  1 ; /* bits 3:3 */  //GE_ETH_RCV_STAT_t

} cs_aal_uni_status_t;

typedef struct {

    cs_uint32 rxctrl_dis           :  1 ; /* disable interrupt indicates the rxctrl state machine has gone to idle state */
    cs_uint32 overrun_drop_dis     :  1 ; /* disable fifo overrun function */
    cs_uint32 crc_drop_dis         :  1 ; /* disable CRC drop function */
    cs_uint32 ovr_drop_dis         :  1 ; /* disable PKT_LEN_CONFIG_max_size drop function */
    cs_uint32 udr_drop_dis         :  1 ; /* disable PKT_LEN_CONFIG_min_size drop function */
    cs_uint32 pause_drop_dis       :  1 ; /* allow pause fram pass through */
    cs_uint32 ukopcode_drop_dis    :  1 ; /* allow unknown opcode packets pass through */
    cs_uint32 oam_drop_dis         :  1 ; /* allow OAM without dropping ?? */
    cs_uint32 bypass_enable        :  1 ; /* enable bypass function */
    cs_uint32 bypass_dpid          :  3 ; /* dpid for bypass packets */
    cs_uint32 bypass_cos           :  3 ; /* cos for bypass packets */
    cs_uint32 hd_backoff_dbg       :  1 ; /* bits 10:10 */
    cs_uint32 hd_backoff_cnt       : 10 ; /* bits 9:0 */

} cs_aal_uni_dbg_t;


typedef union {
    struct {
        cs_uint32 rxctrl_dis           :  1 ; /* disable interrupt indicates the rxctrl state machine has gone to idle state */
        cs_uint32 overrun_drop_dis     :  1 ; /* disable fifo overrun function */
        cs_uint32 crc_drop_dis         :  1 ; /* disable CRC drop function */
        cs_uint32 ovr_drop_dis         :  1 ; /* disable PKT_LEN_CONFIG_max_size drop function */
        cs_uint32 udr_drop_dis         :  1 ; /* disable PKT_LEN_CONFIG_min_size drop function */
        cs_uint32 pause_drop_dis       :  1 ; /* allow pause fram pass through */
        cs_uint32 ukopcode_drop_dis    :  1 ; /* allow unknown opcode packets pass through */
        cs_uint32 oam_drop_dis         :  1 ; /* allow OAM without dropping ?? */
        cs_uint32 bypass_enable        :  1 ; /* enable bypass function */
        cs_uint32 bypass_dpid          :  1 ; /* dpid for bypass packets */
        cs_uint32 bypass_cos           :  1 ; /* cos for bypass packets */
        cs_uint32 hd_backoff_dbg       :  1 ; /* bits 10:10 */
        cs_uint32 hd_backoff_cnt       :  1 ; /* bits 9:0 */
        cs_uint32 rsrvd                : 19 ; 
    } s ;
    cs_uint32     u32 ;
} cs_aal_uni_dbg_mask_t;

typedef volatile union {                                                   
  struct {                                                                 
#ifdef CS_BIG_ENDIAN 
    cs_uint32 phy_int                  :  1 ; /* bits 31:31 */ 
    cs_uint32 rsrvd3                   : 13 ;
    cs_uint32 ptp_out_latched          :  1 ; /* bits 17:17 */
    cs_uint32 ptp_in_latched           :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd2                   : 10 ;
    cs_uint32 mib_msb_triger           :  1 ; /* bits 5:5 */
    cs_uint32 rsrvd1                   :  1 ;
    cs_uint32 rmfifo_overrun           :  1 ; /* bits 3:3 */
    cs_uint32 txfifo_overrun           :  1 ; /* bits 2:2 */
    cs_uint32 txfifo_underrun          :  1 ; /* bits 1:1 */
    cs_uint32 link_change              :  1 ; /* bits 0:0 */
#else
    cs_uint32 link_change              :  1 ; /* bits 0:0 */
    cs_uint32 txfifo_underrun          :  1 ; /* bits 1:1 */
    cs_uint32 txfifo_overrun           :  1 ; /* bits 2:2 */
    cs_uint32 rmfifo_overrun           :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1                   :  1 ;
    cs_uint32 mib_msb_triger           :  1 ; /* bits 5:5 */
    cs_uint32 rsrvd2                   : 10 ;
    cs_uint32 ptp_in_latched           :  1 ; /* bits 16:16 */
    cs_uint32 ptp_out_latched          :  1 ; /* bits 17:17 */
    cs_uint32 rsrvd3                   : 13 ;
    cs_uint32 phy_int                  :  1 ; /* bits 31:31 */ 
#endif    
  } s ;                                                                   
  cs_uint32     u32;                                                      
} cs_aal_uni_int_msk_t;                                              

typedef struct {
    cs_uint32 RxUCPktCnt             ;
    cs_uint32 RxMCFrmCnt             ;
    cs_uint32 RxBCFrmCnt             ;
    cs_uint32 RxOAMFrmCnt            ;
    cs_uint32 RxPauseFrmCnt          ;
    cs_uint32 RxUnKnownOCFrmCnt      ;
    cs_uint32 RxCrcErrFrmCnt         ;
    cs_uint32 RxUndersizeFrmCnt      ;
    cs_uint32 RxRuntFrmCnt           ;
    cs_uint32 RxOvSizeFrmCnt         ;
    cs_uint32 RxJabberFrmCnt         ;
    cs_uint32 RxStatsFrm64Oct        ;
    cs_uint32 RxStatsFrm65to127Oct   ;
    cs_uint32 RxStatsFrm128to255Oct  ;
    cs_uint32 RxStatsFrm256to511Oct  ;
    cs_uint32 RxStatsFrm512to1023Oct ;
    cs_uint32 RxStatsFrm1024to1518Oct;
    cs_uint32 RxStatsFrm1519toMaxOct ;
    cs_uint64 RxByteCount            ;
    cs_uint32 TxUCPktCnt             ;
    cs_uint32 TxMCFrmCnt             ;
    cs_uint32 TxBCFrmCnt             ;
    cs_uint32 TxOAMFrmCnt            ;
    cs_uint32 TxPauseFrmCnt          ;
    cs_uint32 TxCrcErrFrmCnt         ;
    cs_uint32 TxOvSizeFrmCnt         ;
    cs_uint32 TxSingleColFrm         ;
    cs_uint32 TxMultiColFrm          ;
    cs_uint32 TxLateColFrm           ;
    cs_uint32 TxExessColFrm          ;
    cs_uint32 TxEStatsFrm64Oct       ;
    cs_uint32 TxEStatsFrm65to127Oct  ;
    cs_uint32 TxEStatsFrm128to255Oct ;
    cs_uint32 TxEStatsFrm256to511Oct ;
    cs_uint32 TxEStatsFrm512to1023Oct;
    cs_uint32 TxEStatsFrm1024to1518Oc;
    cs_uint32 TxEStatsFrm1519toMaxOct;
    cs_uint64 TxByteCount            ;
}cs_onu_uni_stats_t;

/*==================Functions=================*/
cs_status aal_uni_mac_cfg_set
(
    CS_IN cs_aal_uni_cfg_mask_t mask, 
    CS_IN cs_aal_uni_cfg_t cfg
);


cs_status aal_uni_mac_cfg_get
(
    CS_OUT cs_aal_uni_cfg_t* cfg
);


cs_status aal_uni_mac_cfg_ext_set
(
    CS_IN cs_aal_uni_cfg_ext_mask_t mask, 
    CS_IN cs_aal_uni_cfg_ext_t cfg
);


cs_status aal_uni_mac_cfg_ext_get
(
    CS_OUT cs_aal_uni_cfg_ext_t* cfg
);


cs_status aal_uni_mac_dbg_set
(
    CS_IN cs_aal_uni_dbg_mask_t mask, 
    CS_IN cs_aal_uni_dbg_t cfg
);


cs_status aal_uni_mac_dbg_get
(
    CS_OUT cs_aal_uni_dbg_t* cfg
);


cs_status aal_uni_mac_status_get
(
    CS_OUT cs_aal_uni_status_t* cfg
);

#if 0
cs_status aal_uni_int_mask(cs_uint32 en);
cs_status aal_uni_int_unmask(cs_uint32 en);
#else
cs_status aal_uni_int_disable(cs_aal_uni_int_msk_t mask);
cs_status aal_uni_int_enable(cs_aal_uni_int_msk_t mask);
#endif
cs_status aal_uni_int_en_get(cs_aal_uni_int_msk_t *mask);

cs_status aal_uni_int_status_get(cs_aal_uni_int_msk_t* mask);
cs_status aal_uni_int_status_clr(cs_aal_uni_int_msk_t mask);
cs_status aal_uni_mac_loopback_set(cs_aal_uni_lpbk_mode_t mode);
cs_status aal_uni_mac_loopback_get(cs_aal_uni_lpbk_mode_t *mode);

/*rc: 1 -- read clear, 0 -- read keep;                                                       */
/*hi: high 32 bit data;                                                                      */
/*lo: low 32 bit data;                                                                       */
cs_status aal_uni_mib_get
(
    CS_IN cs_aal_uni_mib_t  mib_id, 
    CS_IN cs_boolean  rc, 
    CS_OUT cs_uint32 *hi, 
    CS_OUT cs_uint32 *lo
);

cs_status aal_uni_eee_set
(
    CS_IN cs_aal_uni_eee_mask_t  mask,
    CS_IN cs_aal_uni_eee_t       eee
);

cs_status aal_uni_eee_get(CS_OUT cs_aal_uni_eee_t  *eee);

#endif /* __AAL_UNI_H__ */

