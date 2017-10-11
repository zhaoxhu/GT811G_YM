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
 * $Id: aal_pon.h,v 1.1.2.8.2.6.16.1 2011/04/19 05:55:13 jli Exp $
 *
 * This file is used for PON MAC driver code
 */

#ifndef __AAL_PON_H__
#define __AAL_PON_H__

#include "cs_types.h"
#include "lynxd_registers.h"

#define LASER_CTRL_BURST          1
#define LASER_CTRL_ALWAYS_OFF     2
#define LASER_CTRL_ALWAYS_ON      3

#define LASER_ON_TIME_MAX         0x3fe
#define LASER_OFF_TIME_MAX        0xfe

#define LASER_TX_DELAY_MAX        0xfe

#define LASER_WAKEUP_TIME_MAX     0xffffff
#define LASER_SHUTDOWN_TIME_MAX   0xffffff
#define LASER_KEEP_TIME_MAX       0xfffffff


#ifdef CS_BIG_ENDIAN 
#define BYPASS_MASK_ERR_MAC_CTRL            (1<<0)
#define BYPASS_MASK_MPCP_REG                (1<<1)
#define BYPASS_MASK_MPCP_NORMAL_GATE        (1<<2)
#define BYPASS_MASK_MPCP_DISCOVERY          (1<<3)
#define BYPASS_MASK_PAUSE                   (1<<4)
#define BYPASS_MASK_SLOW_PROTOCOL           (1<<5)
#define BYPASS_MASK_MAC_CTRL                (1<<6)
#define BYPASS_MASK_RESERVED_OPCODE         (1<<7)
#define BYPASS_MASK_EXTENSION               (1<<8)
#else /* CS_LITTLE_ENDIAN */
#define BYPASS_MASK_ERR_MAC_CTRL            (1<<8)
#define BYPASS_MASK_MPCP_REG                (1<<7)
#define BYPASS_MASK_MPCP_NORMAL_GATE        (1<<6)
#define BYPASS_MASK_MPCP_DISCOVERY          (1<<5)
#define BYPASS_MASK_PAUSE                   (1<<4)
#define BYPASS_MASK_SLOW_PROTOCOL           (1<<3)
#define BYPASS_MASK_MAC_CTRL                (1<<2)
#define BYPASS_MASK_RESERVED_OPCODE         (1<<1)
#define BYPASS_MASK_EXTENSION               (1<<0)
#endif


typedef enum
{
    AAL_PON_CRC8_MODE_NORMAL                        = 0, // not reverse crc8, not reverse data order 
    AAL_PON_CRC8_MODE_REV_DATA                      = 1, // not reverse crc8, reverse data order 
    AAL_PON_CRC8_MODE_REV_CRC8                      = 2, // reverse crc8, not reverse data order     
    AAL_PON_CRC8_MODE_REV_CRC8_DATA                 = 3, // reverse crc8, reverse data order         
} cs_aal_pon_crc8_order_t;

typedef enum 
{
    AAL_PON_SFD_MODE_ONE_55_D5                      = 0, // minimum one 0x55 plus 0xD5
    AAL_PON_SFD_MODE_ZERO_55_D5                     = 1, // minimum zero 0x55 plus 0xD5
} cs_aal_pon_sfd_mode_t;

typedef enum 
{
    AAL_PON_MIB_MODE_LEGACY                         = 0,
    AAL_PON_MIB_MODE_RFC2819                        = 1,
}cs_aal_pon_mib_mode_t;

typedef enum 
{
    AAL_PON_LASER_MODE_OFF                          = 0, // laser is always off
    AAL_PON_LASER_MODE_BURST                        = 1, // normal burst mode
    AAL_PON_LASER_MODE_ON                           = 3, // laser is always on
} cs_aal_pon_laser_mode_t;

typedef enum 
{
    AAL_PON_LASER_POWER_MODE_SW                     = 0,
    AAL_PON_LASER_POWER_MODE_HW                     = 1,
} cs_aal_pon_laser_power_mode_t;

typedef enum 
{
    AAL_PON_BYPASS_MODE_GE                          = 0,
    AAL_PON_BYPASS_DPID_PON                         = 1,
    AAL_PON_BYPASS_DPID_CPU                         = 4,
    AAL_PON_BYPASS_DPID_MII0                        = 5,
    AAL_PON_BYPASS_DPID_DMA                         = 6,
} cs_aal_pon_bypass_dpid_t;

/* Filter ID */
typedef enum
{
    AAL_PON_FILTER_ID_0                             = 0,
    AAL_PON_FILTER_ID_1                             = 1,
    AAL_PON_FILTER_ID_2                             = 2,
    AAL_PON_FILTER_ID_3                             = 3,
    AAL_PON_FILTER_ID_4                             = 4,
    AAL_PON_FILTER_ID_5                             = 5,
    AAL_PON_FILTER_ID_6                             = 6,
    AAL_PON_FILTER_ID_7                             = 7,
}cs_aal_pon_filterid_t;

typedef enum
{
    MIB_ID_PON_BYTECNT                           = counter_id_PON_BYTECNT           ,
    MIB_ID_PON_TXBYTECNT                         = counter_id_PON_TXBYTECNT         ,
    MIB_ID_UCFRAMECNT                            = counter_id_UCFRAMECNT            ,
    MIB_ID_TXUCFRAMECNT                          = counter_id_TXUCFRAMECNT          ,
    MIB_ID_PON_CRCERRCNT                         = counter_id_PON_CRCERRCNT         ,
    MIB_ID_PON_MCFRAMECNT                        = counter_id_PON_MCFRAMECNT        ,
    MIB_ID_PON_BCFRAMECNT                        = counter_id_PON_BCFRAMECNT        ,
    MIB_ID_PON_TXMCFRAMECNT                      = counter_id_PON_TXMCFRAMECNT      ,
    MIB_ID_PON_TXBCFRAMECNT                      = counter_id_PON_TXBCFRAMECNT      ,
    MIB_ID_PON_CTRLFRAMECNT                      = counter_id_PON_CTRLFRAMECNT      ,
    MIB_ID_PON_TXCTRLFRAMECNT                    = counter_id_PON_TXCTRLFRAMECNT    ,
    MIB_ID_PON_PAUSEFRAMECNT                     = counter_id_PON_PAUSEFRAMECNT     ,
    MIB_ID_PON_UNKNOWNOPCNT                      = counter_id_PON_UNKNOWNOPCNT      ,
    MIB_ID_PON_RUNTCNT                           = counter_id_PON_RUNTCNT           ,
    MIB_ID_PON_OVERSIZECNT                       = counter_id_PON_OVERSIZECNT       ,
    MIB_ID_PON_RMON64CNT                         = counter_id_PON_RMON64CNT         ,
    MIB_ID_PON_RMON65_127CNT                     = counter_id_PON_RMON65_127CNT     ,
    MIB_ID_PON_RMON128_255CNT                    = counter_id_PON_RMON128_255CNT    ,
    MIB_ID_PON_RMON256_511CNT                    = counter_id_PON_RMON256_511CNT    ,
    MIB_ID_PON_RMON512_1023CNT                   = counter_id_PON_RMON512_1023CNT   ,
    MIB_ID_PON_RMON1024_1518CNT                  = counter_id_PON_RMON1024_1518CNT  ,
    MIB_ID_PON_TXRMON64CNT                       = counter_id_PON_TXRMON64CNT       ,
    MIB_ID_PON_TXRMON65_127CNT                   = counter_id_PON_TXRMON65_127CNT   ,
    MIB_ID_PON_TXRMON128_255CNT                  = counter_id_PON_TXRMON128_255CNT  ,
    MIB_ID_PON_TXRMON256_511CNT                  = counter_id_PON_TXRMON256_511CNT  ,
    MIB_ID_PON_TXRMON512_1023CNT                 = counter_id_PON_TXRMON512_1023CNT ,
    MIB_ID_PON_TXRMON1024_1518CNT                = counter_id_PON_TXRMON1024_1518CNT,
    MIB_ID_PON_MPCP_COUNTER_MPCP_UC_REV_CNT      = counter_id_PON_MPCP_COUNTER_MPCP_UC_REV_CNT    ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_UC_REG_ACK      = counter_id_PON_MPCP_COUNTER_MPCP_UC_REG_ACK    ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_UC_REG_NACK     = counter_id_PON_MPCP_COUNTER_MPCP_UC_REG_NACK   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_UC_REG_DEREG    = counter_id_PON_MPCP_COUNTER_MPCP_UC_REG_DEREG  ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_UC_REG_REREG    = counter_id_PON_MPCP_COUNTER_MPCP_UC_REG_REREG  ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_UC_REG_REG_REV  = counter_id_PON_MPCP_COUNTER_MPCP_UC_REG_REG_REV,
    MIB_ID_PON_MPCP_COUNTER_MPCP_UC_GAT_REV      = counter_id_PON_MPCP_COUNTER_MPCP_UC_GAT_REV    ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_UC_GAT_NORM     = counter_id_PON_MPCP_COUNTER_MPCP_UC_GAT_NORM   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_UC_GAT_FRPT     = counter_id_PON_MPCP_COUNTER_MPCP_UC_GAT_FRPT   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_UC_GAT_UDIS     = counter_id_PON_MPCP_COUNTER_MPCP_UC_GAT_UDIS   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_UC_GAT_BDIS     = counter_id_PON_MPCP_COUNTER_MPCP_UC_GAT_BDIS   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_MC_REV_CNT      = counter_id_PON_MPCP_COUNTER_MPCP_MC_REV_CNT    ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_MC_REG_ACK      = counter_id_PON_MPCP_COUNTER_MPCP_MC_REG_ACK    ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_MC_REG_NACK     = counter_id_PON_MPCP_COUNTER_MPCP_MC_REG_NACK   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_MC_REG_DEREG    = counter_id_PON_MPCP_COUNTER_MPCP_MC_REG_DEREG  ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_MC_REG_REREG    = counter_id_PON_MPCP_COUNTER_MPCP_MC_REG_REREG  ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_MC_REG_REG_REV  = counter_id_PON_MPCP_COUNTER_MPCP_MC_REG_REG_REV,
    MIB_ID_PON_MPCP_COUNTER_MPCP_MC_GAT_REV      = counter_id_PON_MPCP_COUNTER_MPCP_MC_GAT_REV    ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_MC_GAT_NORM     = counter_id_PON_MPCP_COUNTER_MPCP_MC_GAT_NORM   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_MC_GAT_FRPT     = counter_id_PON_MPCP_COUNTER_MPCP_MC_GAT_FRPT   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_MC_GAT_UDIS     = counter_id_PON_MPCP_COUNTER_MPCP_MC_GAT_UDIS   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_MC_GAT_BDIS     = counter_id_PON_MPCP_COUNTER_MPCP_MC_GAT_BDIS   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_BC_REV_CNT      = counter_id_PON_MPCP_COUNTER_MPCP_BC_REV_CNT    ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_BC_REG_ACK      = counter_id_PON_MPCP_COUNTER_MPCP_BC_REG_ACK    ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_BC_REG_NACK     = counter_id_PON_MPCP_COUNTER_MPCP_BC_REG_NACK   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_BC_REG_DEREG    = counter_id_PON_MPCP_COUNTER_MPCP_BC_REG_DEREG  ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_BC_REG_REREG    = counter_id_PON_MPCP_COUNTER_MPCP_BC_REG_REREG  ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_BC_REG_REG_REV  = counter_id_PON_MPCP_COUNTER_MPCP_BC_REG_REG_REV,
    MIB_ID_PON_MPCP_COUNTER_MPCP_BC_GAT_REV      = counter_id_PON_MPCP_COUNTER_MPCP_BC_GAT_REV    ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_BC_GAT_NORM     = counter_id_PON_MPCP_COUNTER_MPCP_BC_GAT_NORM   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_BC_GAT_FRPT     = counter_id_PON_MPCP_COUNTER_MPCP_BC_GAT_FRPT   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_BC_GAT_UDIS     = counter_id_PON_MPCP_COUNTER_MPCP_BC_GAT_UDIS   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_BC_GAT_BDIS     = counter_id_PON_MPCP_COUNTER_MPCP_BC_GAT_BDIS   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_TX_REQ_DEREG    = counter_id_PON_MPCP_COUNTER_MPCP_TX_REQ_DEREG  ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_TX_REQ_REG      = counter_id_PON_MPCP_COUNTER_MPCP_TX_REQ_REG    ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_TX_ACK_ACK      = counter_id_PON_MPCP_COUNTER_MPCP_TX_ACK_ACK    ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_TX_ACK_NACK     = counter_id_PON_MPCP_COUNTER_MPCP_TX_ACK_NACK   ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_TX_RPT          = counter_id_PON_MPCP_COUNTER_MPCP_TX_RPT    ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_TX_OAM          = counter_id_PON_MPCP_COUNTER_MPCP_TX_OAM    ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_DA_MC_NSLF      = counter_id_PON_MPCP_COUNTER_MPCP_DA_MC_NSLF,
    MIB_ID_PON_MPCP_COUNTER_MPCP_DA_MC_SLF       = counter_id_PON_MPCP_COUNTER_MPCP_DA_MC_SLF ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_DA_UC           = counter_id_PON_MPCP_COUNTER_MPCP_DA_UC     ,
    MIB_ID_PON_MPCP_COUNTER_MPCP_DA_BC           = counter_id_PON_MPCP_COUNTER_MPCP_DA_BC     ,
    MIB_ID_PON_FRAMECNT                          = counter_id_PON_FRAMECNT                    ,
    MIB_ID_PON_TXOVERSIZECNT                     = counter_id_PON_TXOVERSIZECNT               ,
    MIB_ID_PON_RX1519_MAXCNT                     = counter_id_PON_RX1519_MAXCNT               ,
    MIB_ID_PON_TX1519_MAXCNT                     = counter_id_PON_TX1519_MAXCNT               ,
    MIB_ID_PON_TXFRAMECNT                        = counter_id_PON_TXFRAMECNT                  ,
    MIB_ID_TXPAUSECNT                            = counter_id_TXPAUSECNT                      ,
    MIB_ID_UNDERSIZECNT                          = counter_id_UNDERSIZECNT                    ,
    MIB_ID_JABBERCNT                             = counter_id_JABBERCNT                       ,
    MIB_ID_EXTENSIONCNT                          = counter_id_EXTENSIONCNT                    ,
    MIB_ID_TXEXTENSIONCNT                        = counter_id_TXEXTENSIONCNT                  ,

    MIB_ID_PCS_ERROR_SYMBOL     = 0xf001,
    MIB_ID_PCS_OUTOFSYNC_CNT    = 0xf002,
    MIB_ID_SLD_ERR_CNT          = 0xf003,
    MIB_ID_CRC8_ERR_CNT         = 0xf004,
    MIB_ID_MAC_DROP_CNT         = 0xf005,
    MIB_ID_IPG_DROP_CNT         = 0xf006,
    MIB_ID_PACKET_DRAIN_CNT     = 0xf007,
    MIB_ID_TX_CRCERR_CNT        = 0xf008,

}cs_aal_pon_mib_id_t;

/* PON configuration */
typedef struct
{
    cs_boolean                                          rx_en;             // Enable RX MAC
    cs_boolean                                          tx_en;             // Enable TX MAC
    cs_uint8                                            rx_ipg;            // (default=12) range: 5~12
    cs_uint8                                            tx_ipg;            // (default=12) range: 10~13
    cs_uint16                                           mtu;               // range: 0 ~ 1900
    cs_boolean                                          mtu_tag_en;        // if mut_tag_en==1, 4 more bytes will be added into "mtu"
    cs_uint16                                           mtu_tag_pattern;   // user-define TPID for "mtu_tag_en"
    cs_uint16                                           tx_max_size;       // range: 0 ~ 1900
    cs_boolean                                          tx_max_limit_en;   // Enable or disable TX MAX packet limit.
    cs_uint8                                            rx_min_size;       // Packet size smaller than this number will be drop.
                                                                           // range: 2~64 : Enable
                                                                           //            0: Disable short packet filtering
    cs_boolean                                          tx_drain_en;       // Enable or disable TX drain
    cs_boolean                                          tx_fix_8b_en;      // configured preamble as fixed 8B or auto-adjust as 7B/8B

    cs_uint16                                           user_fifo_hi;      // (default=1000) This the upper threshold for user packet buffer to generate back pressure signal to BM
    cs_uint16                                           user_fifo_lo;      // (default=8) This the lower threshold for user packet buffer to generate back pressure signal to BM
    cs_uint8                                            pcs_loop_cnt_hi;   // Read-only: 1~15  Loop count for error detection
    cs_uint8                                            pcs_loop_cnt_lo;   // Read-only: 1~15  Loop count for sync detection
    cs_boolean                                          crc32_regen_en;    // Enable or disable CRC32 regeneration
    cs_boolean                                          crc8_check_en;     // 0: bypass crc8 check; 1: enable crc8 check; 
    cs_boolean                                          pcs_10b_aliagn_en; // Enable PCS 10b alignment or not
    cs_boolean                                          fec_sync_en;       // 1: FEC mode PCS synchronization 0: non-FEC mode synchronization
    cs_aal_pon_sfd_mode_t                               sfd_mode2;         // (default=AAL_PON_SFD_MODE_ONE_55_D5) SLD detection mode.
    cs_aal_pon_crc8_order_t                             crc8_order_sel;    //  (default=AAL_PON_CRC8_MODE_NORMAL) Reverse or not CRC8 and data order
}cs_aal_pon_mac_cfg_t;

typedef union 
{
    struct 
    {
    cs_uint32 rx_en                                     : 1;          
    cs_uint32 tx_en                                     : 1;          
    cs_uint32 rx_ipg                                    : 1;         
    cs_uint32 tx_ipg                                    : 1;         
    cs_uint32 mtu                                       : 1;    
    cs_uint32 mtu_tag_en                                : 1;    
    cs_uint32 mtu_tag_pattern                           : 1;    
    cs_uint32 tx_max_size                               : 1;    
    cs_uint32 tx_max_limit_en                           : 1;
    cs_uint32 rx_min_size                               : 1;    
    cs_uint32 tx_drain_en                               : 1;        
    cs_uint32 tx_fix_8b_en                              : 1;    
    cs_uint32 user_fifo_hi                              : 1;    
    cs_uint32 user_fifo_lo                              : 1;    
    cs_uint32 pcs_loop_cnt_hi                           : 1;    
    cs_uint32 pcs_loop_cnt_lo                           : 1;    
    cs_uint32 crc32_regen_en                            : 1;    
    cs_uint32 crc8_check_en                             : 1;    
    cs_uint32 pcs_10b_aliagn_en                         : 1;    
    cs_uint32 fec_sync_en                               : 1;    
    cs_uint32 sfd_mode2                                 : 1;
    cs_uint32 crc8_order_sel                            : 1;
    } s;
    cs_uint32 u32;
}cs_aal_pon_mac_msk_t;

/* PON llid filtering configuration */
typedef struct {
    cs_boolean        drop_ffff;              // discard traffic from LLID 0xffff when it's true
    cs_boolean        drop_7fff;              // discard traffic from LLID 0x7fff when it's true
    cs_boolean        drop_p2p;               // discard traffic from P2P LLID (excluding my sending P2P traffic)
    cs_boolean        drop_myslef;            // discard traffic to ONU self's unicast LLID
    cs_boolean        accept_all;             // accept all downstream traffic (including all unicast and non-unicast LLIDs even for other ONUs)
    cs_boolean        accept_cs;              // accept Cortina defined LLID filtering scheme
}cs_aal_pon_llid_filter_cfg_t;

typedef union 
{
    struct 
    {
    cs_uint32 drop_ffff         : 1;
    cs_uint32 drop_7fff         : 1;
    cs_uint32 drop_p2p          : 1;
    cs_uint32 drop_myslef       : 1;
    cs_uint32 accept_all        : 1;
    cs_uint32 accept_cs         : 1;
    } s;
    cs_uint32 u32;
}cs_aal_pon_llid_filter_msk_t;

/* PON stats */
typedef struct 
{
    cs_uint64 pon_byte_cnt           ;
    cs_uint64 pon_tx_byte_cnt        ;
    cs_uint32 pon_ucframe_cnt        ;
    cs_uint32 pon_txucframe_cnt      ;
    cs_uint32 pon_crcerr_cnt         ;
    cs_uint32 pon_mcframe_cnt        ;
    cs_uint32 pon_bcframe_cnt        ;
    cs_uint32 pon_txmcframe_cnt      ;
    cs_uint32 pon_txbcframe_cnt      ;
    cs_uint32 pon_ctrlframe_cnt      ;
    cs_uint32 pon_txctrlframe_cnt    ;
    cs_uint32 pon_pauseframe_cnt     ;
    cs_uint32 pon_unknownop_cnt      ;
    cs_uint32 pon_runt_cnt           ;
    cs_uint32 pon_oversize_cnt       ;
    cs_uint32 pon_rmon64_cnt         ;
    cs_uint32 pon_rmon65_127_cnt     ;
    cs_uint32 pon_rmon128_255_cnt    ;
    cs_uint32 pon_rmon256_511_cnt    ;
    cs_uint32 pon_rmon512_1023_cnt   ;
    cs_uint32 pon_rmon1024_1518_cnt  ;
    cs_uint32 pon_txrmon64_cnt       ;
    cs_uint32 pon_txrmon65_127_cnt   ;
    cs_uint32 pon_txrmon128_255_cnt  ;
    cs_uint32 pon_txrmon256_511_cnt  ;
    cs_uint32 pon_txrmon512_1023_cnt ;
    cs_uint32 pon_txrmon1024_1518_cnt;
    cs_uint32 pon_mpcp_uc_rev_cnt     ;
    cs_uint32 pon_mpcp_uc_reg_ack     ;
    cs_uint32 pon_mpcp_uc_reg_nack    ;
    cs_uint32 pon_mpcp_uc_reg_dereg   ;
    cs_uint32 pon_mpcp_uc_reg_rereg   ;
    cs_uint32 pon_mpcp_uc_reg_reg_rev ;
    cs_uint32 pon_mpcp_uc_gat_rev     ;
    cs_uint32 pon_mpcp_uc_gat_norm    ;
    cs_uint32 pon_mpcp_uc_gat_frpt    ;
    cs_uint32 pon_mpcp_uc_gat_udis    ;
    cs_uint32 pon_mpcp_uc_gat_bdis    ;
    cs_uint32 pon_mpcp_mc_rev_cnt     ;
    cs_uint32 pon_mpcp_mc_reg_ack     ;
    cs_uint32 pon_mpcp_mc_reg_nack    ;
    cs_uint32 pon_mpcp_mc_reg_dereg   ;
    cs_uint32 pon_mpcp_mc_reg_rereg   ;
    cs_uint32 pon_mpcp_mc_reg_reg_rev ;
    cs_uint32 pon_mpcp_mc_gat_rev     ;
    cs_uint32 pon_mpcp_mc_gat_norm    ;
    cs_uint32 pon_mpcp_mc_gat_frpt    ;
    cs_uint32 pon_mpcp_mc_gat_udis    ;
    cs_uint32 pon_mpcp_mc_gat_bdis    ;
    cs_uint32 pon_mpcp_bc_rev_cnt     ;
    cs_uint32 pon_mpcp_bc_reg_ack     ;
    cs_uint32 pon_mpcp_bc_reg_nack    ;
    cs_uint32 pon_mpcp_bc_reg_dereg   ;
    cs_uint32 pon_mpcp_bc_reg_rereg   ;
    cs_uint32 pon_mpcp_bc_reg_reg_rev ;
    cs_uint32 pon_mpcp_bc_gat_rev     ;
    cs_uint32 pon_mpcp_bc_gat_norm    ;
    cs_uint32 pon_mpcp_bc_gat_frpt    ;
    cs_uint32 pon_mpcp_bc_gat_udis    ;
    cs_uint32 pon_mpcp_bc_gat_bdis    ;
    cs_uint32 pon_mpcp_tx_req_dereg   ;
    cs_uint32 pon_mpcp_tx_req_reg     ;
    cs_uint32 pon_mpcp_tx_ack_ack     ;
    cs_uint32 pon_mpcp_tx_ack_nack    ;
    cs_uint32 pon_mpcp_tx_rpt    ;
    cs_uint32 pon_mpcp_tx_oam    ;
    cs_uint32 pon_mpcp_da_mc_nslf;
    cs_uint32 pon_mpcp_da_mc_slf ;
    cs_uint32 pon_mpcp_da_uc     ;
    cs_uint32 pon_framecnt       ;
    cs_uint32 pon_txoversizecnt  ;
    cs_uint32 pon_rx1519_maxcnt  ;
    cs_uint32 pon_tx1519_maxcnt  ;
    cs_uint32 pon_txframecnt     ;
    cs_uint32 pon_txpausecnt     ;
    cs_uint32 pon_undersizecnt   ;
    cs_uint32 pon_jabbercnt      ;
    cs_uint32 pon_extensioncnt   ;
    cs_uint32 pon_txextensioncnt ;
    cs_uint32 pon_err_symbol     ;
    cs_uint32 pon_outofsync      ;
    cs_uint32 pon_sld_err        ;
    cs_uint32 pon_crc8_err       ;
    cs_uint32 pon_mac_drop       ;
    cs_uint32 pon_ipg_drop       ;
    cs_uint32 pon_drain          ;
    cs_uint32 pon_tx_crc_err     ;
}cs_aal_pon_stats_t;


typedef union 
{
    struct {
#ifdef CS_BIG_ENDIAN
    cs_uint32 reserved                  : 21;
    cs_uint32 err_maccontrol_permit     : 1;
    cs_uint32 register_permit           : 1;
    cs_uint32 non_discovery_permit      : 1;
    cs_uint32 discovery_gate_permit     : 1;
    cs_uint32 fcs_err_permit            : 1;
    cs_uint32 oversize_permit           : 1;
    cs_uint32 undersize_permit          : 1;
    cs_uint32 pause_frame_permit        : 1;
    cs_uint32 slow_protocol_permit      : 1;
    cs_uint32 maccontrol_permit         : 1;
    cs_uint32 extention_frame_permit    : 1; 
#else /* CS_LITTLE_ENDIAN */
    cs_uint32 extention_frame_permit    : 1; 
    cs_uint32 maccontrol_permit         : 1;
    cs_uint32 slow_protocol_permit      : 1;
    cs_uint32 pause_frame_permit        : 1;
    cs_uint32 undersize_permit          : 1;
    cs_uint32 oversize_permit           : 1;
    cs_uint32 fcs_err_permit            : 1;
    cs_uint32 discovery_gate_permit     : 1;
    cs_uint32 non_discovery_permit      : 1;
    cs_uint32 register_permit           : 1;
    cs_uint32 err_maccontrol_permit     : 1;
    cs_uint32 reserved                  : 21;
#endif
    } s;
    cs_uint32 u32;
}cs_aal_pon_drop_t;


typedef struct 
{
    cs_boolean                              tx_en;                    // (default=false) Enable transceiver TX
    cs_aal_pon_laser_mode_t                 laser_mode;               // (default=AAL_PON_LASER_MODE_BURST) laser control mode
    cs_uint16                               laser_on_time;            // (default=64) 0~1022, in unit of 8ns (1/2 TQ), should be an even number
    cs_uint16                               laser_off_time;           // (default=64) 0~254, in unit of 8ns (1/2 TQ), should be an even number
    cs_uint8                                tx_delay;                 // (default=30) 0~254, in unit of 8ns (1/2 TQ), should be an even number
    cs_boolean                              tx_power_en;              // (default=0) active TX_PWR_DOWN signal
    cs_boolean                              sig_loss_polarity;        // (default=1) "ERX_SIGLOSS" signal polarity
    cs_boolean                              tx_burst_polarity;        // (default=1) "TX_BURST" signal polarity
    cs_boolean                              output_en;                // (default=1) output of signals TX_BURST, TX_DISABLE, TX_PWR_DOWN
    cs_aal_pon_laser_power_mode_t           power_mode;               // (default=0) 1: HW power control mode; 0: SW power control mode
    cs_uint32                               wakeup_time;              // (default=0x20)time duration from supply power to enters normal working mode, unit is TQ
    cs_uint32                               shutdown_time;            // (default=0x10)time duration from shut down power to transmitter fully disabled, unit is TQ
    cs_uint32                               keep_time;                // (default=0xa0000) keep timer expiration value, unit is TQ
} cs_aal_pon_laser_cfg_t;

typedef union {
    struct {
    cs_uint32  tx_en                        : 1;
    cs_uint32  laser_mode                   : 1;
    cs_uint32  laser_on_off_time            : 1;
    cs_uint32  tx_delay                     : 1;
    cs_uint32  tx_power_en                  : 1;
    cs_uint32  sig_loss_polarity            : 1;
    cs_uint32  tx_burst_polarity            : 1;
    cs_uint32  output_en                    : 1;
    cs_uint32  power_mode                   : 1;
    cs_uint32  wakeup_time                  : 1;
    cs_uint32  shutdown_time                : 1;
    cs_uint32  keep_time                    : 1;
    } s;
    cs_uint32 u32;
}cs_aal_pon_laser_msk_t;


typedef struct 
{
    cs_aal_pon_mib_mode_t                   mib_mode;       // 1: RFC 2819 compliant mode; 0:legacy MIB counter mode
                                                            // NOTE: This bit should be set before ONU starts to work.
    cs_uint8                                short_limit;    // Unit is byte. Packet size less than this number will not be counted into MIB counter
    cs_uint16                               rmon_top;       // Size falling into range of 1024 bytes to this number will be counted into "PON_RMON1024_1518CNT" and "PON_TXRMON1024_1518CNT"
} cs_aal_pon_mib_cfg_t;

typedef union 
{
    struct {
    cs_aal_pon_mib_mode_t  mib_mode                     : 1;
    cs_uint32  short_limit                              : 1;
    cs_uint32  rmon_top                                 : 1;
    } s;
    cs_uint32 u32;
}cs_aal_pon_mib_msk_t;


typedef struct 
{
    cs_uint8                                rx_latency; // 	SerDes latency in receiving direction in terms of 8ns unit.
    cs_uint8                                tx_latency; // 	SerDes latency in sending direction in terms of 8ns unit.
} cs_aal_pon_serdes_cfg_t;

typedef union 
{
    struct 
    {
    cs_uint32  rx_latency                   : 1;
    cs_uint32  tx_latency                   : 1;
    } s;
    cs_uint32 u32;
}cs_aal_pon_serdes_msk_t;


typedef struct {
    cs_boolean                              bypass_en;          // Enable bypass function or not
    cs_aal_pon_bypass_dpid_t                dpid;               // Destination port ID in Bypass Mode.
                                                                // 0: GE port;
                                                                // 1: EPON port;
                                                                // 2: Reserved port;
                                                                // 3: Reserved port;
                                                                // 4: CPU port;
                                                                // 5: MII0 port (VoIP) port;
                                                                // 6: DMA port port;
                                                                // 7: Reserved ports
    cs_uint8                                cos;                // Destination port COS in Bypass Mode
    cs_uint8                                bypass_mask;        // Permit/not allow bypass function for some special packets.
    cs_uint16                               ext_ethertype;      // EXTENSION frame ethertype.
    cs_uint16                               ext_opcode;         // EXTENSION frame opcode.
    cs_uint32                               ext_oui;            // EXTENSION frame oui.
    cs_boolean                              ext_oui_check_en;   // 0: ignore OUI checking 1: check oui 
    cs_mac_t                                ext_da;             // EXTENSION frame DA
} cs_aal_pon_bypass_fe_cfg_t;


typedef union 
{
    struct 
    {
    cs_uint32  bypass_en                    : 1;
    cs_uint32  dpid                         : 1;
    cs_uint32  cos                          : 1;
    cs_uint32  bypass_mask                  : 1;
    cs_uint32  ext_ethertype                : 1;
    cs_uint32  ext_opcode                   : 1;
    cs_uint32  ext_oui                      : 1;
    cs_uint32  ext_oui_check_en             : 1;
    cs_uint32  ext_da                       : 1;
    } s;
    cs_uint32 u32;
}cs_aal_pon_bypass_fe_msk_t;


typedef volatile union 
{
    struct
    {
#ifdef CS_BIG_ENDIAN 
    cs_uint32 pps_cap                           :  1 ;
    cs_uint32 sig_loss                          :  1 ;
    cs_uint32 discovery_frm                     :  1 ;
    cs_uint32 enc_en_trig                       :  1 ;
    cs_uint32 grant_drop_tm                     :  1 ;
    cs_uint32 grant_drop_ts                     :  1 ;
    cs_uint32 grant_drop_tf                     :  1 ;
    cs_uint32 rtt_rx                            :  1 ;
    cs_uint32 mpcp_timer_expiration             :  1 ;
    cs_uint32 gate_timer_expiration             :  1 ;
    cs_uint32 timestamp_drift                   :  1 ;
    cs_uint32 mpcp_grant_fifo_access_err        :  1 ;
    cs_uint32 mpcp_grant_fifo_exceeding         :  1 ;
    cs_uint32 rx_register_frame                 :  1 ;
    cs_uint32 registration_status_change        :  1 ;
    cs_uint32 report_timer_expiration           :  1 ;
    cs_uint32 bm_pon_tx_mon                     :  1 ;
    cs_uint32 ptp_ig_lts                        :  1 ;
    cs_uint32 tx_packet_uflow                   :  1 ;
    cs_uint32 ptp_eg_lts                        :  1 ;
    cs_uint32 tx_fail                           :  1 ;
    cs_uint32 tx_overrun                        :  1 ;
    cs_uint32 tx_pkt_cut                        :  1 ;
    cs_uint32 tx_non_empty                      :  1 ;
    cs_uint32 mib                               :  1 ;
    cs_uint32 pcs_rx_sync                       :  1 ;
    cs_uint32 pps_timeout                       :  1 ;
    cs_uint32 cwb_full                          :  1 ;
    cs_uint32 cmd_full                          :  1 ;
    cs_uint32 prty_full                         :  1 ;
    cs_uint32 pack_full                         :  1 ;
    cs_uint32 cntr_half_full                    :  1 ;
#else
    cs_uint32 cntr_half_full                    :  1 ;
    cs_uint32 pack_full                         :  1 ;
    cs_uint32 prty_full                         :  1 ;
    cs_uint32 cmd_full                          :  1 ;
    cs_uint32 cwb_full                          :  1 ;
    cs_uint32 pps_timeout                       :  1 ;
    cs_uint32 pcs_rx_sync                       :  1 ;
    cs_uint32 mib                               :  1 ;
    cs_uint32 tx_non_empty                      :  1 ;
    cs_uint32 tx_pkt_cut                        :  1 ;
    cs_uint32 tx_overrun                        :  1 ;
    cs_uint32 tx_fail                           :  1 ;
    cs_uint32 ptp_eg_lts                        :  1 ;
    cs_uint32 tx_packet_uflow                   :  1 ;
    cs_uint32 ptp_ig_lts                        :  1 ;
    cs_uint32 bm_pon_tx_mon                     :  1 ;
    cs_uint32 report_timer_expiration           :  1 ;
    cs_uint32 registration_status_change        :  1 ;
    cs_uint32 rx_register_frame                 :  1 ;
    cs_uint32 mpcp_grant_fifo_exceeding         :  1 ;
    cs_uint32 mpcp_grant_fifo_access_err        :  1 ;
    cs_uint32 timestamp_drift                   :  1 ;
    cs_uint32 gate_timer_expiration             :  1 ;
    cs_uint32 mpcp_timer_expiration             :  1 ;
    cs_uint32 rtt_rx                            :  1 ;
    cs_uint32 grant_drop_tf                     :  1 ;
    cs_uint32 grant_drop_ts                     :  1 ;
    cs_uint32 grant_drop_tm                     :  1 ;
    cs_uint32 enc_en_trig                       :  1 ;
    cs_uint32 discovery_frm                     :  1 ;
    cs_uint32 sig_loss                          :  1 ;
    cs_uint32 pps_cap                           :  1 ;
#endif
    }s;
    cs_uint32 u32;
} cs_aal_pon_int_msk_t;

typedef enum
{
    AAL_PON_CLK_REFERENCE = 0,
    AAL_PON_CLK_RECOVERY,
}cs_aal_pon_clk_t;

typedef enum
{
    AAL_PON_CLK_SWITCH_SW = 0,
    AAL_PON_CLK_SWITCH_HW,
}cs_aal_pon_clk_switch_t;

/****************************************************************************/
/* $rtn_hdr_start   Set PON port parameters                                 */
/* CATEGORY   : Device                                                      */
/* ACCESS     : public                                                      */
/* BLOCK      : PON                                                         */
/* CHIP        : CS8030                                                     */
cs_status aal_pon_mac_cfg_set(
    CS_IN cs_aal_pon_mac_msk_t msk,
    CS_IN cs_aal_pon_mac_cfg_t *cfg
);
/* INPUTS     : o  msk - Parameter mask                                     */
/* o  cfg - Configure PON port parameters                                   */
/* OUTPUTS    : ----                                                        */
/* RETURNS    : o  CS_ERROR: other error occurs                             */
/*              o  CS_E_PARAM: one of parameter is not correct              */
/*              o  CS_OK: set successfully                                  */
/* DESCRIPTION:                                                             */
/*             This API is used to  configure PON port parameters, the related structure is following:*/
/*typedef struct {*/
/*    cs_boolean        rx_en; */             // Enable RX MAC
/*    cs_boolean        tx_en; */             // Enable TX MAC
/*    cs_uint8          rx_ipg; */            // (default=12) range: 5~12
/*    cs_uint8          tx_ipg; */            // (default=12) range: 10~13
/*    cs_uint16         mtu; */               // range: 0 ~ 1900
/*    cs_boolean        mtu_tag_en; */        // if mut_tag_en==1, 4 more bytes will be added into "mtu"
/*    cs_uint16         mtu_tag_pattern; */   // user-define TPID for "mtu_tag_en"
/*    cs_uint16         tx_max_size; */       // range: 0 ~ 1900
/*    cs_boolean        tx_max_limit_en; */   // Enable or disable TX MAX packet limit.
/*    cs_uint8          rx_min_size; */       // Packet size smaller than this number will be drop.
                                            // range: 2~64 : Enable
                                            //        0    : Disable short packet filtering
/*    cs_boolean        tx_drain_en; */       // Enable or disable TX drain
/*    cs_boolean        tx_fix_8b_en; */      // configured preamble as fixed 8B or auto-adjust as 7B/8B

/*    cs_uint16         user_fifo_hi; */      // (default=1000) This the upper threshold for user packet buffer to generate back pressure signal to BM
/*    cs_uint16         user_fifo_lo; */      // (default=8) This the lower threshold for user packet buffer to generate back pressure signal to BM
/*    cs_uint8          pcs_loop_cnt_hi; */   // Read-only: 1~15  Loop count for error detection
/*    cs_uint8          pcs_loop_cnt_lo; */   // Read-only: 1~15  Loop count for sync detection
/*    cs_boolean        crc32_regen_en; */    // Enable or disable CRC32 regeneration
/*    cs_boolean        crc8_check_en; */     // 0: bypass crc8 check; */ 1: enable crc8 check; */ 
/*    cs_boolean        pcs_10b_aliagn_en; */ // Enable PCS 10b alignment or not
/*    cs_boolean        fec_sync_en; */       // 1: FEC mode PCS synchronization 0: non-FEC mode synchronization
/*    cs_aal_pon_sfd_mode_e     sfd_mode2; */         // (default=AAL_PON_SFD_MODE_ONE_55_D5) SLD detection mode.
/*    cs_aal_pon_crc8_order_e   crc8_order_sel; */    //  (default=AAL_PON_CRC8_MODE_NORMAL) Reverse or not CRC8 and data order
/*}cs_aal_pon_mac_cfg_t; */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start   Get PON port parameters                                 */
/* CATEGORY   : Device                                                      */
/* ACCESS     : public                                                      */
/* BLOCK      : PON                                                         */
/* CHIP        : CS8030                                                     */
cs_status aal_pon_mac_cfg_get(
    CS_OUT cs_aal_pon_mac_cfg_t *cfg
);
/* INPUTS    : ----                                                         */
/* OUTPUTS   : o  cfg - PON port configuration                              */
/* RETURNS   : o  CS_OK: Get successfully                                   */
/*             o  CS_ERROR: Get failed                                      */
/* DESCRIPTION:                                                             */
/*             This API is used to  retrieve PON port parameters            */
/*                                                                          */
/* $rtn_hdr_end                                                             */
/****************************************************************************/


/****************************************************************************/
/* $rtn_hdr_start   Set Cortina defined LLID filter                         */
/* CATEGORY   : Device                                                      */
/* ACCESS     : public                                                      */
/* BLOCK      : PON                                                         */
/* CHIP        : CS8030                                                     */
cs_status aal_pon_cs_llid_pattern_set(
    CS_IN cs_aal_pon_filterid_t	id,
    CS_IN cs_uint16 mask,
    CS_IN cs_uint16 pattern
);
/* INPUTS     : o  id - PON LLID filter index                               */
/*              o  mask - LLID filter mask                                  */
/*                      (default=0xffff)                                    */
/*              o  pattern - LLID filter pattern                            */
/*                      (default=0xffff) */
/* OUTPUTS    : ----                                                        */
/* RETURNS    : o  CS_OK: Set successfully                                  */
/*              o  CS_ERROR: Set failed                                     */
/* DESCRIPTION:                                                             */
/*             Set each CS LLID filter entry. The LLID is matched if:       */
/*                      (LLID AND mask) == (pattern AND mask)               */
/*                                                                          */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start   Get Cortina defined LLID filter                         */
/* CATEGORY   : Device                                                      */
/* ACCESS     : public                                                      */
/* BLOCK      : PON                                                         */
/* CHIP        : CS8030                                                     */
cs_status aal_pon_cs_llid_pattern_get(
    CS_IN cs_aal_pon_filterid_t	id,
    CS_OUT cs_uint16 *mask,
    CS_OUT cs_uint16 *pattern
);
/* INPUTS     : o  id - PON LLID filter index                               */
/* OUTPUTS    : o  mask - LLID filter mask                                  */
/*              o  pattern - LLID filter pattern                            */
/* RETURNS    : o  CS_OK: Set successfully                                  */
/*              o  CS_ERROR: Set failed                                     */
/* DESCRIPTION:                                                             */
/*             Get each CS LLID filter entry.                               */
/*                                                                          */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  Set PON Laser Configuration                              */
/* CATEGORY   :   Device                                                    */
/* ACCESS     :   Public                                                    */
/* BLOCK      : PON                                                         */
/* CHIP        : CS8030                                                     */
cs_status aal_pon_laser_cfg_set(
    CS_IN cs_aal_pon_laser_msk_t msk,
    CS_IN cs_aal_pon_laser_cfg_t *cfg
);
/* INPUTS     :  o msk - Parameter mask                                     */
/*       o cfg - PON laser configuration                                    */
/* OUTPUTS    :    ----                                                     */
/* RETURNS    : o  CS_OK: Set successfully                                  */
/*              o  CS_ERROR: Set failed                                     */
/* DESCRIPTION:                                                             */
/*    Set PON Laser Configuration, configuring structure is defined following:*/
/* typedef struct { */
/*    cs_boolean  tx_en; */                    // (default=false) Enable transceiver TX
/*    cs_aal_pon_laser_mode_e laser_mode; */   // (default=AAL_PON_LASER_MODE_BURST) laser control mode
/*    cs_uint16   laser_on_time; */            // (default=64) 0~1022, in unit of 8ns (1/2 TQ), should be an even number
/*    cs_uint16   laser_off_time; */           // (default=64) 0~254, in unit of 8ns (1/2 TQ), should be an even number
/*    cs_uint8    tx_delay; */                 // (default=30) 0~254, in unit of 8ns (1/2 TQ), should be an even number
/*    cs_boolean  tx_power_en; */              // (default=0) active TX_PWR_DOWN signal
/*    cs_boolean  sig_loss_polarity; */        // (default=1) "ERX_SIGLOSS" signal polarity
/*    cs_boolean  tx_burst_polarity; */        // (default=1) "TX_BURST" signal polarity
/*    cs_boolean  output_en; */                // (default=1) output of signals TX_BURST, TX_DISABLE, TX_PWR_DOWN
/*    cs_boolean  power_mode; */               // (default=0) 1: HW power control mode; */ 0: SW power control mode
/*    cs_uint32   wakeup_time; */              // (default=0x20)time duration from supply power to enters normal working mode, unit is TQ
/*    cs_uint32   shutdown_time; */            // (default=0x10)time duration from shut down power to transmitter fully disabled, unit is TQ
/*    cs_uint32   keep_time; */                // (default=0xa0000) keep timer expiration value, unit is TQ
/* } cs_aal_pon_laser_cfg_t; */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  Get PON Laser Configuration                              */
/* CATEGORY   :   Device                                                    */
/* ACCESS     :   Public                                                    */
/* BLOCK      : PON                                                         */
/* CHIP        : CS8030                                                     */
cs_status aal_pon_laser_cfg_get(
    CS_OUT cs_aal_pon_laser_cfg_t *cfg
);
/* INPUTS     : ----                                                        */
/* OUTPUTS    : o cfg - PON laser configuration                             */
/* RETURNS    : o  CS_OK: Get successfully                                  */
/*              o  CS_ERROR: Get failed                                     */
/* DESCRIPTION:                                                             */
/*    Get PON Laser Configuration                                           */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start   Get PON link status                                     */
/* CATEGORY   : Device                                                      */
/* ACCESS     : public                                                      */
/* BLOCK      : PON                                                         */
/* CHIP        : CS8030                                                     */
cs_status aal_pon_link_get(cs_boolean *link);
/* INPUTS    :  ---- */
/* OUTPUTS   : o  link - PON port link status (true for link-up, false for link-down) */
/* RETURNS   : o  CS_OK: Get successfully                                   */
/*              o  CS_ERROR: Get failed                                     */
/* DESCRIPTION:                                                             */
/*              This API is used to retrieve PON link status.               */ 
/*                                                                          */
/* $rtn_hdr_end                                                             */
/****************************************************************************/


/****************************************************************************/
/* $rtn_hdr_start  Set llid filtering Configuration                         */
/* CATEGORY   :   Device                                                    */
/* ACCESS     :   Public                                                    */
/* BLOCK      : PON                                                         */
/* CHIP        : CS8030                                                     */
cs_status aal_pon_llid_filter_cfg_set(
    CS_IN cs_aal_pon_llid_filter_msk_t msk,
    CS_IN cs_aal_pon_llid_filter_cfg_t *cfg
);
/* INPUTS     :  o msk - Parameter mask                                     */
/*       o cfg - PON laser configuration                                    */
/* OUTPUTS    :    ----                                                     */
/* RETURNS    : o  CS_OK: Set successfully                                  */
/*              o  CS_ERROR: Set failed                                     */
/* DESCRIPTION:                                                             */
/*    Set PON llid filtering, configuring structure is defined following:   */
/* typedef struct { */
/*    cs_boolean        drop_ffff; */     
/*    cs_boolean        drop_7fff; */              
/*    cs_boolean        drop_p2p; */               
/*    cs_boolean        drop_myslef; */            
/*    cs_boolean        accept_all; */             
/*    cs_boolean        accept_cs; */              
/* }cs_aal_pon_llid_filter_cfg_t; */
/* $rtn_hdr_end                                                             */
/****************************************************************************/


/****************************************************************************/
/* $rtn_hdr_start  Set PON llid filtering Configuration                     */
/* CATEGORY   :   Device                                                    */
/* ACCESS     :   Public                                                    */
/* BLOCK      :   PON                                                       */
/* CHIP       :   CS8030                                                    */
cs_status aal_pon_llid_filter_cfg_get(
    CS_OUT cs_aal_pon_llid_filter_cfg_t *cfg
);
/* INPUTS     : ----                                                        */
/* OUTPUTS    : o cfg - PON llid filtering configuration                    */
/* RETURNS    : o  CS_OK: Get successfully                                  */
/*              o  CS_ERROR: Get failed                                     */
/* DESCRIPTION:                                                             */
/*    Get PON llid filtering configuration                                  */
/* $rtn_hdr_end                                                             */
/****************************************************************************/


/******************************************************************************/
/* $rtn_hdr_start  Set PON drop policy                                        */
/* CATEGORY   :   Device                                                      */
/* ACCESS     :   Public                                                      */
/* BLOCK      :   PON                                                         */
/* CHIP       :   CS8030                                                      */
cs_status aal_pon_drop_policy_set(
    CS_IN cs_aal_pon_drop_t msk,
    CS_IN cs_aal_pon_drop_t *cfg
);
/* INPUTS     : o msk - Parameter mask */
/*              o cfg - PON drop policy configuration                         */
/* OUTPUTS    :    ----                                                       */
/* RETURNS    : o  CS_OK: Set successfully                                    */
/*              o  CS_ERROR: Set failed                                       */
/* DESCRIPTION:                                                               */
/*    Set PON drop policy                                                     */
/* $rtn_hdr_end                                                               */
/******************************************************************************/


/******************************************************************************/
/* $rtn_hdr_start  Get PON drop policy                                        */
/* CATEGORY   :   Device                                                      */
/* ACCESS     :   Public                                                      */
/* BLOCK      :   PON                                                         */
/* CHIP       :   CS8030                                                      */
cs_status aal_pon_drop_policy_get(
    CS_OUT cs_aal_pon_drop_t *cfg
);
/* INPUTS     : ----                                                          */
/* OUTPUTS    : o cfg - PON drop policy configuration                         */
/* RETURNS    : o  CS_OK: Get successfully                                    */
/*              o  CS_ERROR: Get failed                                       */
/* DESCRIPTION:                                                               */
/*    Get PON drop policy                                                     */
/* $rtn_hdr_end                                                               */
/******************************************************************************/


/******************************************************************************/
/* $rtn_hdr_start  Set PON MIB configuration                                  */
/* CATEGORY   :   Device                                                      */
/* ACCESS     :   Public                                                      */
/* BLOCK      :   PON                                                         */
/* CHIP       :   CS8030                                                      */
cs_status aal_pon_mib_cfg_set(
    CS_IN cs_aal_pon_mib_msk_t msk,
    CS_IN cs_aal_pon_mib_cfg_t *cfg
);
/* INPUTS     : o msk - Parameter mask */
/*              o cfg - PON MIB configuration                                 */
/* OUTPUTS    :    ----                                                       */
/* RETURNS    : o  CS_OK: Set successfully                                    */
/*              o  CS_ERROR: Set failed                                       */
/* DESCRIPTION:                                                               */
/*    Set MIB configuration                                                   */
/* $rtn_hdr_end                                                               */
/******************************************************************************/


/******************************************************************************/
/* $rtn_hdr_start  Get PON MIB configuration                                  */
/* CATEGORY   :   Device                                                      */
/* ACCESS     :   Public                                                      */
/* BLOCK      :   PON                                                         */
/* CHIP       :   CS8030                                                      */
cs_status aal_pon_mib_cfg_get(
    CS_OUT cs_aal_pon_mib_cfg_t *cfg
);
/* INPUTS     : ----                                                          */
/* OUTPUTS    : o cfg - PON MIB configuration                                 */
/* RETURNS    : o  CS_OK: Get successfully                                    */
/*              o  CS_ERROR: Get failed                                       */
/* DESCRIPTION:                                                               */
/*    Set MIB configuration                                                   */
/* $rtn_hdr_end                                                               */
/******************************************************************************/


/******************************************************************************/
/* $rtn_hdr_start  Set PON serdes configuration                               */
/* CATEGORY   :   Device                                                      */
/* ACCESS     :   Public                                                      */
/* BLOCK      :   PON                                                         */
/* CHIP       :   CS8030                                                      */
cs_status aal_pon_serdes_cfg_set(
    CS_IN cs_aal_pon_serdes_msk_t msk,
    CS_IN cs_aal_pon_serdes_cfg_t *cfg
);
/* INPUTS     : o msk - Parameter mask                                        */
/*              o cfg - PON serdes configuration                              */
/* OUTPUTS    :    ----                                                       */
/* RETURNS    : o  CS_OK: Set successfully                                    */
/*              o  CS_ERROR: Set failed                                       */
/* DESCRIPTION:                                                               */
/*    Set PON serdes configuration                                            */
/* $rtn_hdr_end                                                               */
/******************************************************************************/


/******************************************************************************/
/* $rtn_hdr_start  Get PON serdes configuration                               */
/* CATEGORY   :   Device                                                      */
/* ACCESS     :   Public                                                      */
/* BLOCK      :   PON                                                         */
/* CHIP       :   CS8030                                                      */
cs_status aal_pon_serdes_cfg_get(
    CS_OUT cs_aal_pon_serdes_cfg_t *cfg
);
/* INPUTS     : ----                                                          */
/* OUTPUTS    : o cfg - PON serdes configuration                              */
/* RETURNS    : o  CS_OK: Get successfully                                    */
/*              o  CS_ERROR: Get failed                                       */
/* DESCRIPTION:                                                               */
/*    Get serdes configuration                                                */
/* $rtn_hdr_end                                                               */
/******************************************************************************/


/******************************************************************************/
/* $rtn_hdr_start  Set PON bypass configuration                               */
/* CATEGORY   :   Device                                                      */
/* ACCESS     :   Public                                                      */
/* BLOCK      :   PON                                                         */
/* CHIP       :   CS8030                                                      */
cs_status aal_pon_bypass_fe_cfg_set(
    CS_IN cs_aal_pon_bypass_fe_msk_t msk,
    CS_IN cs_aal_pon_bypass_fe_cfg_t *cfg
);
/* INPUTS     : o msk - Parameter mask                                        */
/*              o cfg - PON bypass configuration                              */
/* OUTPUTS    :    ----                                                       */
/* RETURNS    : o  CS_OK: Set successfully                                    */
/*              o  CS_ERROR: Set failed                                       */
/* DESCRIPTION:                                                               */
/*    Set PON bypass configuration                                            */
/* $rtn_hdr_end                                                               */
/******************************************************************************/


/******************************************************************************/
/* $rtn_hdr_start  Get PON bypass configuration                               */
/* CATEGORY   :   Device                                                      */
/* ACCESS     :   Public                                                      */
/* BLOCK      :   PON                                                         */
/* CHIP       :   CS8030                                                      */
cs_status aal_pon_bypass_fe_cfg_get(
    CS_OUT cs_aal_pon_bypass_fe_cfg_t *cfg
);
/* INPUTS     : ----                                                          */
/* OUTPUTS    : o cfg - PON bypass configuration                              */
/* RETURNS    : o  CS_OK: Get successfully                                    */
/*              o  CS_ERROR: Get failed                                       */
/* DESCRIPTION:                                                               */
/*    Get bypass configuration                                                */
/* $rtn_hdr_end                                                               */
/******************************************************************************/


/******************************************************************************/
/* $rtn_hdr_start  Set PON MAC address                                        */
/* CATEGORY   :   Device                                                      */
/* ACCESS     :   Public                                                      */
/* BLOCK      :   PON                                                         */
/* CHIP       :   CS8030                                                      */
cs_status aal_pon_mac_addr_set(
    CS_IN cs_mac_t mac
);
/* INPUTS     : o mac - ONU PON MAC address                                   */
/* OUTPUTS    :    ----                                                       */
/* RETURNS    : o  CS_OK: Set successfully                                    */
/*              o  CS_ERROR: Set failed                                       */
/* DESCRIPTION:                                                               */
/*    Set PON MAC address                                                     */
/* $rtn_hdr_end                                                               */
/******************************************************************************/


/******************************************************************************/
/* $rtn_hdr_start  Get PON MAC address                                        */
/* CATEGORY   :   Device                                                      */
/* ACCESS     :   Public                                                      */
/* BLOCK      :   PON                                                         */
/* CHIP       :   CS8030                                                      */
cs_status aal_pon_mac_addr_get(
    CS_IN cs_mac_t *mac
);
/* INPUTS     : o N/A                                                         */
/* OUTPUTS    : o mac - ONU PON MAC address                                   */
/* RETURNS    : o  CS_OK: Get successfully                                    */
/*              o  CS_ERROR: Set failed                                       */
/* DESCRIPTION:                                                               */
/*    Get PON MAC address                                                     */
/* $rtn_hdr_end                                                               */
/******************************************************************************/


/******************************************************************************/
/* $rtn_hdr_start  Reset PON MIB counters                                     */
/* CATEGORY   :   Device                                                      */
/* ACCESS     :   Public                                                      */
/* BLOCK      :   PON                                                         */
/* CHIP       :   CS8030                                                      */
cs_status aal_pon_mib_reset(void);
/* INPUTS     : o N/A                                                         */
/* OUTPUTS    : o N/A                                                         */
/* RETURNS    : o  CS_OK: Reset successfully                                  */
/*              o  CS_ERROR: SResetet failed                                  */
/* DESCRIPTION:                                                               */
/*    Reset PON MIB counters                                                  */
/* $rtn_hdr_end                                                               */
/******************************************************************************/


/******************************************************************************/
/* $rtn_hdr_start  Get PON MIB by MIB ID                                      */
/* CATEGORY   :   Device                                                      */
/* ACCESS     :   Public                                                      */
/* BLOCK      :   PON                                                         */
/* CHIP       :   CS8030                                                      */
cs_status aal_pon_mib_get(cs_boolean read_clear, cs_aal_pon_mib_id_t mib_id, cs_uint32 *value_l, cs_uint32 *value_h);
/* INPUTS     : o read_clear - 1: read and clear 0: read and keep             */
/*            : o mib_id - MIB ID                                             */
/* OUTPUTS    : o value_l - value low 32bit                                   */
/*            : o value_h - value high 32bit                                  */
/* RETURNS    : o  CS_OK: Reset successfully                                  */
/*              o  CS_ERROR: SResetet failed                                  */
/* DESCRIPTION:                                                               */
/*    Get PON MIB by MIB ID                                                   */
/* $rtn_hdr_end                                                               */
/******************************************************************************/


void aal_pon_int_disable(cs_aal_pon_int_msk_t mask);
void aal_pon_int_clr(cs_aal_pon_int_msk_t mask);
void aal_pon_int_src_get(cs_aal_pon_int_msk_t *mask);
void aal_pon_int_en_get(cs_aal_pon_int_msk_t *mask);
void aal_pon_int_en_set(cs_aal_pon_int_msk_t mask);
cs_status aal_pon_clk_switch(cs_aal_pon_clk_t clock);
cs_status aal_pon_clk_switch_mode_set(cs_aal_pon_clk_switch_t mode);
cs_status aal_pon_loopback_set(cs_boolean enable);
cs_status aal_pon_loopback_get(cs_boolean *enable);





#endif /* __AAL_PON_H__ */

