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

	This file is used for management port control code
	
*/


#ifndef __AAL_MA_H__
#define __AAL_MA_H__

#include "cs_types.h"
#include "aal.h"
#include "registers.h"

typedef enum 
{
    AAL_MA_RX_BUF0 = 0,
    AAL_MA_RX_BUF1 = 1,
}cs_aal_ma_rx_buf_id_t;

typedef enum 
{
    AAL_MA_FW_FE    = 0,
    AAL_MA_FW_CPU   = 1,
}cs_aal_ma_fw_t;

typedef enum
{
    AAL_MA_MII_SPEED_10M  = 0,
    AAL_MA_MII_SPEED_100M = 1,
}cs_aal_ma_mii_speed_t;

typedef enum
{
    AAL_MA_MII_PHY_MODE_MII  = 0,
    AAL_MA_MII_PHY_MODE_RMII = 1,
}cs_aal_ma_mii_phy_mode_t;

typedef enum
{
    AAL_MA_MII_MODE_MAC2MAC = 0,
    AAL_MA_MII_MODE_PHY     = 1,
}cs_aal_ma_mii_mode_t;

typedef struct 
{
    cs_boolean      pkt_rdy  ; 
    cs_boolean      turn     ; 
    cs_boolean      len_err  ; 
    cs_uint8        spid     ; 
    cs_uint16       pkt_len  ; 
}cs_aal_ma_rx_buf_status_t;

typedef struct
{
    cs_uint32 head;
    cs_pkt_t *pkt;
}cs_aal_ma_rx_buf_t;

typedef union
{
    struct
    {
#ifdef CS_BIG_ENDIAN
    cs_uint16       reserved6           : 1;
    cs_uint16       reserved5           : 1;
    cs_uint16       reserved4           : 1;
    cs_uint16       reserved3           : 1;
    cs_uint16       hello_pkt_drop      : 1;
    cs_uint16       ipv6_pkt_drop       : 1;
    cs_uint16       ipv4_pkt_drop       : 1;
    cs_uint16       mymac_pkt_drop      : 1;
    cs_uint16       reserved2           : 1;
    cs_uint16       oam_pkt_drop        : 1;
    cs_uint16       arp_pkt_drop        : 1;
    cs_uint16       reserved1           : 1;
    cs_uint16       reserved            : 1;
    cs_uint16       dot1x_pkt_drop      : 1;
    cs_uint16       bpdu_pkt_drop       : 1;
    cs_uint16       normal_pkt_drop     : 1;
#else
    cs_uint16       normal_pkt_drop     : 1;
    cs_uint16       bpdu_pkt_drop       : 1;
    cs_uint16       dot1x_pkt_drop      : 1;
    cs_uint16       reserved            : 1;
    cs_uint16       reserved1           : 1;
    cs_uint16       arp_pkt_drop        : 1;
    cs_uint16       oam_pkt_drop        : 1;
    cs_uint16       reserved2           : 1;
    cs_uint16       mymac_pkt_drop      : 1;
    cs_uint16       ipv4_pkt_drop       : 1;
    cs_uint16       ipv6_pkt_drop       : 1;
    cs_uint16       hello_pkt_drop      : 1;
    cs_uint16       reserved3           : 1;
    cs_uint16       reserved4           : 1;
    cs_uint16       reserved5           : 1;
    cs_uint16       reserved6           : 1;
#endif
    }s;
    cs_uint16 u16;
}cs_aal_ma_mii_drop_t;

typedef union
{
    struct
    {
#ifdef CS_BIG_ENDIAN
    cs_aal_ma_fw_t       reserved6         : 1;
    cs_aal_ma_fw_t       reserved5         : 1;
    cs_aal_ma_fw_t       reserved4         : 1;
    cs_aal_ma_fw_t       reserved3         : 1;
    cs_aal_ma_fw_t       hello_pkt_fw      : 1;
    cs_aal_ma_fw_t       ipv6_pkt_fw       : 1;
    cs_aal_ma_fw_t       ipv4_pkt_fw       : 1;
    cs_aal_ma_fw_t       mymac_pkt_fw      : 1;
    cs_aal_ma_fw_t       reserved2         : 1;
    cs_aal_ma_fw_t       oam_pkt_fw        : 1;
    cs_aal_ma_fw_t       arp_pkt_fw        : 1;
    cs_aal_ma_fw_t       reserved1         : 1;
    cs_aal_ma_fw_t       reserved          : 1;
    cs_aal_ma_fw_t       dot1x_pkt_fw      : 1;
    cs_aal_ma_fw_t       bpdu_pkt_fw       : 1;
    cs_aal_ma_fw_t       normal_pkt_fw     : 1;
#else
    cs_aal_ma_fw_t       normal_pkt_fw     : 1;
    cs_aal_ma_fw_t       bpdu_pkt_fw       : 1;
    cs_aal_ma_fw_t       dot1x_pkt_fw      : 1;
    cs_aal_ma_fw_t       reserved          : 1;
    cs_aal_ma_fw_t       reserved1         : 1;
    cs_aal_ma_fw_t       arp_pkt_fw        : 1;
    cs_aal_ma_fw_t       oam_pkt_fw        : 1;
    cs_aal_ma_fw_t       reserved2         : 1;
    cs_aal_ma_fw_t       mymac_pkt_fw      : 1;
    cs_aal_ma_fw_t       ipv4_pkt_fw       : 1;
    cs_aal_ma_fw_t       ipv6_pkt_fw       : 1;
    cs_aal_ma_fw_t       hello_pkt_fw      : 1;
    cs_aal_ma_fw_t       reserved3         : 1;
    cs_aal_ma_fw_t       reserved4         : 1;
    cs_aal_ma_fw_t       reserved5         : 1;
    cs_aal_ma_fw_t       reserved6         : 1;
#endif
    }s;
    cs_uint16 u16;
}cs_aal_ma_mii_fw_t;

typedef volatile union {
  struct {
#ifdef CS_BIG_ENDIAN 
    cs_uint32 cpu_rx_buff1_rdy     :  1 ; /* bits 31:31 */
    cs_uint32 cpu_rx_buff0_rdy     :  1 ; /* bits 30:30 */
    cs_uint32 cpu_tx_buff_rdy      :  1 ; /* bits 29:29 */
    cs_uint32 rsrvd2               :  5 ;
    cs_uint32 mii0_rx_pkt_os       :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 auto_xfer_done       :  1 ; /* bits 12:12 */
    cs_uint32 dma1_tx_done         :  1 ; /* bits 11:11 */
    cs_uint32 dma0_tx_done         :  1 ; /* bits 10:10 */
    cs_uint32 mib_msb_int_set      :  1 ; /* bits 9:9 */
    cs_uint32 pkt_xfer_done        :  1 ; /* bits 8:8 */
    cs_uint32 pkt_xfer2mem_done    :  1 ; /* bits 7:7 */
    cs_uint32 mii0_rx_ff_ol        :  1 ; /* bits 6:6 */
    cs_uint32 mii0_tx_stop_done    :  1 ; /* bits 5:5 */
    cs_uint32 mii0_rx_ff_full      :  1 ; /* bits 4:4 */
    cs_uint32 mii0_tx_stop         :  1 ; /* bits 3:3 */
    cs_uint32 mii0_rx_pkt_drop     :  1 ; /* bits 2:2 */
    cs_uint32 mii0_tx_ff_full      :  1 ; /* bits 1:1 */
    cs_uint32 mii0_tx_pkt_done     :  1 ; /* bits 0:0 */
#else /* CS_LITTLE_ENDIAN */
    cs_uint32 mii0_tx_pkt_done     :  1 ; /* bits 0:0 */
    cs_uint32 mii0_tx_ff_full      :  1 ; /* bits 1:1 */
    cs_uint32 mii0_rx_pkt_drop     :  1 ; /* bits 2:2 */
    cs_uint32 mii0_tx_stop         :  1 ; /* bits 3:3 */
    cs_uint32 mii0_rx_ff_full      :  1 ; /* bits 4:4 */
    cs_uint32 mii0_tx_stop_done    :  1 ; /* bits 5:5 */
    cs_uint32 mii0_rx_ff_ol        :  1 ; /* bits 6:6 */
    cs_uint32 pkt_xfer2mem_done    :  1 ; /* bits 7:7 */
    cs_uint32 pkt_xfer_done        :  1 ; /* bits 8:8 */
    cs_uint32 mib_msb_int_set      :  1 ; /* bits 9:9 */
    cs_uint32 dma0_tx_done         :  1 ; /* bits 10:10 */
    cs_uint32 dma1_tx_done         :  1 ; /* bits 11:11 */
    cs_uint32 auto_xfer_done       :  1 ; /* bits 12:12 */
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 mii0_rx_pkt_os       :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd2               :  5 ;
    cs_uint32 cpu_tx_buff_rdy      :  1 ; /* bits 29:29 */
    cs_uint32 cpu_rx_buff0_rdy     :  1 ; /* bits 30:30 */
    cs_uint32 cpu_rx_buff1_rdy     :  1 ; /* bits 31:31 */
#endif
  } s;
  cs_uint32     u32;
} cs_aal_ma_int_msk_t;

typedef struct
{
    cs_aal_ma_mii_mode_t        mode;
    cs_aal_ma_mii_phy_mode_t    phy_mode;
    cs_aal_ma_mii_speed_t       speed;
    cs_boolean                  rx_en;
    cs_boolean                  tx_en;
    cs_uint8                    rx_ipg;
    cs_uint8                    tx_ipg;
    cs_boolean                  rx_flow_ctrl_en;
    cs_boolean                  tx_flow_ctrl_en;
    cs_boolean                  crc_regen_en;
    cs_boolean                  loopback_en;
}cs_aal_ma_mii_cfg_t;

typedef union
{
    struct
    {
    cs_uint32              mode             : 1;
    cs_uint32              phy_mode         : 1;
    cs_uint32              speed            : 1;
    cs_uint32              rx_en            : 1;
    cs_uint32              tx_en            : 1;
    cs_uint32              rx_ipg           : 1;
    cs_uint32              tx_ipg           : 1;
    cs_uint32              rx_flow_ctrl_en  : 1;
    cs_uint32              tx_flow_ctrl_en  : 1;
    cs_uint32              crc_regen_en     : 1;
    cs_uint32              loopback_en      : 1;
    }s;
    cs_uint32 u32;
}cs_aal_ma_mii_msk_t;

typedef enum
{
    MIB_ID_MA_RXUCPKTCNT                            = counter_RxUCPktCnt              ,
    MIB_ID_MA_RXMCFRMCNT                            = counter_RxMCFrmCnt              ,
    MIB_ID_MA_RXBCFRMCNT                            = counter_RxBCFrmCnt              ,
    MIB_ID_MA_RXOAMFRMCNT                           = counter_RxOAMFrmCnt             ,
    MIB_ID_MA_RXPAUSEFRMCNT                         = counter_RxPauseFrmCnt           ,
    MIB_ID_MA_RXUNKNOWNOCFRMCNT                     = counter_RxUnKnownOCFrmCnt       ,
    MIB_ID_MA_RXCRCERRFRMCNT                        = counter_RxCrcErrFrmCnt          ,
    MIB_ID_MA_RXUNDERSIZEFRMCNT                     = counter_RxUndersizeFrmCnt       ,
    MIB_ID_MA_RXRUNTFRMCNT                          = counter_RxRuntFrmCnt            ,
    MIB_ID_MA_RXOVSIZEFRMCNT                        = counter_RxOvSizeFrmCnt          ,
    MIB_ID_MA_RXJABBERFRMCNT                        = counter_RxJabberFrmCnt          ,
    MIB_ID_MA_RXSTATSFRM64OCT                       = counter_RxStatsFrm64Oct         ,
    MIB_ID_MA_RXSTATSFRM65TO127OCT                  = counter_RxStatsFrm65to127Oct    ,
    MIB_ID_MA_RXSTATSFRM128TO255OCT                 = counter_RxStatsFrm128to255Oct   ,
    MIB_ID_MA_RXSTATSFRM256TO511OCT                 = counter_RxStatsFrm256to511Oct   ,
    MIB_ID_MA_RXSTATSFRM512TO1023OCT                = counter_RxStatsFrm512to1023Oct  ,
    MIB_ID_MA_RXSTATSFRM1024TO1518OCT               = counter_RxStatsFrm1024to1518Oct ,
    MIB_ID_MA_RXSTATSFRM1519TOMAXOCT                = counter_RxStatsFrm1519toMaxOct  ,
    MIB_ID_MA_RXBYTECOUNT                           = counter_RxByteCount             ,
    MIB_ID_MA_RXFRMCOUNT                            = counter_RxFrmCount              ,
    MIB_ID_MA_TXUCPKTCNT                            = counter_TxUCPktCnt              ,
    MIB_ID_MA_TXMCFRMCNT                            = counter_TxMCFrmCnt              ,
    MIB_ID_MA_TXBCFRMCNT                            = counter_TxBCFrmCnt              ,
    MIB_ID_MA_TXOAMFRMCNT                           = counter_TxOAMFrmCnt             ,
    MIB_ID_MA_TXPAUSEFRMCNT                         = counter_TxPauseFrmCnt           ,
    MIB_ID_MA_TXCRCERRFRMCNT                        = counter_TxCrcErrFrmCnt          ,
    MIB_ID_MA_TXOVSIZEFRMCNT                        = counter_TxOvSizeFrmCnt          ,
    MIB_ID_MA_TXESTATSFRM64OCT                      = counter_TxEStatsFrm64Oct        ,
    MIB_ID_MA_TXESTATSFRM65TO127OCT                 = counter_TxEStatsFrm65to127Oct   ,
    MIB_ID_MA_TXESTATSFRM128TO255OCT                = counter_TxEStatsFrm128to255Oct  ,
    MIB_ID_MA_TXESTATSFRM256TO511OCT                = counter_TxEStatsFrm256to511Oct  ,
    MIB_ID_MA_TXESTATSFRM512TO1023OCT               = counter_TxEStatsFrm512to1023Oct ,
    MIB_ID_MA_TXESTATSFRM1024TO1518OCT              = counter_TxEStatsFrm1024to1518Oct,
    MIB_ID_MA_TXESTATSFRM1519TOMAXOCT               = counter_TxEStatsFrm1519toMaxOct ,
    MIB_ID_MA_TXBYTECOUNT                           = counter_TxByteCount             ,
    MIB_ID_MA_TXFRMCOUNT                            = counter_TxFrmCount              ,
}cs_aal_ma_mib_id_t;

cs_status aal_ma_rx_buf_read(cs_aal_ma_rx_buf_id_t buf_id, cs_aal_ma_rx_buf_t *buf_to, cs_uint16 len);
cs_status aal_ma_rx_buf_clear(cs_aal_ma_rx_buf_id_t buf_id);
cs_status aal_ma_rx_buf_status_get(cs_aal_ma_rx_buf_id_t buf_id, cs_aal_ma_rx_buf_status_t *status);
cs_status aal_ma_tx(cs_uint8 *buf, cs_uint16 len, cs_aal_port_id_t port, cs_uint8 pri);
cs_status aal_ma_tx_no_mutex(cs_uint8 *buf, cs_uint16 len, cs_aal_port_id_t port, cs_uint8 pri);
void aal_ma_int_en_set(cs_aal_ma_int_msk_t mask);
void aal_ma_int_en_get(cs_aal_ma_int_msk_t *mask);
void aal_ma_int_disable(cs_aal_ma_int_msk_t mask);
void aal_ma_int_clr(cs_aal_ma_int_msk_t mask);
void aal_ma_int_src_get(cs_aal_ma_int_msk_t *mask);
cs_status aal_ma_mii_cfg_set(cs_aal_ma_mii_msk_t msk, cs_aal_ma_mii_cfg_t *cfg);
cs_status aal_ma_mib_get(cs_boolean read_clear, cs_aal_ma_mib_id_t mib_id, cs_uint32 *value_l, cs_uint32 *value_h);
cs_status aal_ma_mib_reset(void);

#endif /* __AAL_MA_H__ */

