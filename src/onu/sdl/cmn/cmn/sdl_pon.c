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
 * $Id: sdl_pon.c,v 1.1.2.5.2.16.2.1.4.1.12.3 2013/08/30 12:18:54 yding Exp $
 */


#include "sdl_pon.h"
#include "sdl_event_cmn.h"
#include "sdl_init.h"
#include "sdl_int.h"
#include "aal_pon.h"
#include "aal_serdes.h"
#include "startup_cfg.h"
#include "sdl.h"

#define __PON_MTU_MAX 1900

cs_status epon_request_onu_pon_laser_on_time_get(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32            device,
    CS_IN cs_int32            llidport,
    CS_OUT cs_uint16          *time
)
{
    cs_aal_pon_laser_cfg_t cfg;

    if(NULL==time)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    aal_pon_laser_cfg_get(&cfg);
    *time = cfg.laser_on_time/2;

    return CS_E_OK;
}

cs_status epon_request_onu_pon_laser_off_time_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_uint16              *time
)
{
    cs_aal_pon_laser_cfg_t cfg;

    if(NULL==time)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    aal_pon_laser_cfg_get(&cfg);

    *time = cfg.laser_off_time/2;

    return CS_E_OK;
}



cs_status epon_request_onu_pon_stats_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              read_clear,
    CS_OUT cs_pon_stats_t             *stats
)
{
    cs_status rt = CS_E_OK;
    cs_uint32 value_h, value_l;
    cs_uint64 temp;

    if(NULL == stats)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_BYTECNT           , &value_l, &value_h);
    temp = value_h;
    stats->pon_byte_cnt =  (temp<<32)|value_l;
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_TXBYTECNT         , &value_l, &value_h);
    temp = value_h;
    stats->pon_tx_byte_cnt =  (temp<<32)|value_l;
    rt |= aal_pon_mib_get(read_clear, MIB_ID_UCFRAMECNT            , &stats->pon_frame_cnt        , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_TXUCFRAMECNT          , &stats->pon_txframe_cnt      , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_CRCERRCNT         , &stats->pon_crcerr_cnt       , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MCFRAMECNT        , &stats->pon_mcframe_cnt      , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_BCFRAMECNT        , &stats->pon_bcframe_cnt      , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_TXMCFRAMECNT      , &stats->pon_txmcframe_cnt    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_TXBCFRAMECNT      , &stats->pon_txbcframe_cnt    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_CTRLFRAMECNT      , &stats->pon_ctrlframe_cnt    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_TXCTRLFRAMECNT    , &stats->pon_txctrlframe_cnt  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_PAUSEFRAMECNT     , &stats->pon_pauseframe_cnt   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_UNKNOWNOPCNT      , &stats->pon_unknownop_cnt    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_RUNTCNT           , &stats->pon_runt_cnt         , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_OVERSIZECNT       , &stats->pon_oversize_cnt     , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_RMON64CNT         , &stats->pon_rmon64_cnt       , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_RMON65_127CNT     , &stats->pon_rmon65_127_cnt   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_RMON128_255CNT    , &stats->pon_rmon128_255_cnt  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_RMON256_511CNT    , &stats->pon_rmon256_511_cnt  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_RMON512_1023CNT   , &stats->pon_rmon512_1023_cnt , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_RMON1024_1518CNT  , &stats->pon_rmon1024_1518_cnt, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_TXRMON64CNT       , &stats->pon_txrmon64_cnt       , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_TXRMON65_127CNT   , &stats->pon_txrmon65_127_cnt   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_TXRMON128_255CNT  , &stats->pon_txrmon128_255_cnt  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_TXRMON256_511CNT  , &stats->pon_txrmon256_511_cnt  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_TXRMON512_1023CNT , &stats->pon_txrmon512_1023_cnt , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_TXRMON1024_1518CNT, &stats->pon_txrmon1024_1518_cnt, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_UC_REV_CNT    , &stats->pon_mpcp_uc_rev_cnt    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_UC_REG_ACK    , &stats->pon_mpcp_uc_reg_ack    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_UC_REG_NACK   , &stats->pon_mpcp_uc_reg_nack   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_UC_REG_DEREG  , &stats->pon_mpcp_uc_reg_dereg  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_UC_REG_REREG  , &stats->pon_mpcp_uc_reg_rereg  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_UC_REG_REG_REV, &stats->pon_mpcp_uc_reg_reg_rev, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_UC_GAT_REV    , &stats->pon_mpcp_uc_gat_rev    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_UC_GAT_NORM   , &stats->pon_mpcp_uc_gat_norm   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_UC_GAT_FRPT   , &stats->pon_mpcp_uc_gat_frpt   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_UC_GAT_UDIS   , &stats->pon_mpcp_uc_gat_udis   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_UC_GAT_BDIS   , &stats->pon_mpcp_uc_gat_bdis   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_MC_REV_CNT    , &stats->pon_mpcp_mc_rev_cnt    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_MC_REG_ACK    , &stats->pon_mpcp_mc_reg_ack    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_MC_REG_NACK   , &stats->pon_mpcp_mc_reg_nack   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_MC_REG_DEREG  , &stats->pon_mpcp_mc_reg_dereg  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_MC_REG_REREG  , &stats->pon_mpcp_mc_reg_rereg  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_MC_REG_REG_REV, &stats->pon_mpcp_mc_reg_reg_rev, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_MC_GAT_REV    , &stats->pon_mpcp_mc_gat_rev    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_MC_GAT_NORM   , &stats->pon_mpcp_mc_gat_norm   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_MC_GAT_FRPT   , &stats->pon_mpcp_mc_gat_frpt   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_MC_GAT_UDIS   , &stats->pon_mpcp_mc_gat_udis   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_MC_GAT_BDIS   , &stats->pon_mpcp_mc_gat_bdis   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_BC_REV_CNT    , &stats->pon_mpcp_bc_rev_cnt    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_BC_REG_ACK    , &stats->pon_mpcp_bc_reg_ack    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_BC_REG_NACK   , &stats->pon_mpcp_bc_reg_nack   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_BC_REG_DEREG  , &stats->pon_mpcp_bc_reg_dereg  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_BC_REG_REREG  , &stats->pon_mpcp_bc_reg_rereg  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_BC_REG_REG_REV, &stats->pon_mpcp_bc_reg_reg_rev, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_BC_GAT_REV  , &stats->pon_mpcp_bc_gat_rev  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_BC_GAT_NORM , &stats->pon_mpcp_bc_gat_norm , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_BC_GAT_FRPT , &stats->pon_mpcp_bc_gat_frpt , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_BC_GAT_UDIS , &stats->pon_mpcp_bc_gat_udis , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_BC_GAT_BDIS , &stats->pon_mpcp_bc_gat_bdis , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_TX_REQ_DEREG, &stats->pon_mpcp_tx_req_dereg, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_TX_REQ_REG  , &stats->pon_mpcp_tx_req_reg  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_TX_ACK_ACK  , &stats->pon_mpcp_tx_ack_ack  , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_TX_ACK_NACK , &stats->pon_mpcp_tx_ack_nack , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_TX_RPT    , &stats->pon_mpcp_tx_rpt    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_TX_OAM    , &stats->pon_mpcp_tx_oam    , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_DA_MC_NSLF, &stats->pon_mpcp_da_mc_nslf, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_DA_MC_SLF , &stats->pon_mpcp_da_mc_slf , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_MPCP_COUNTER_MPCP_DA_UC     , &stats->pon_mpcp_da_uc     , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_FRAMECNT                    , &stats->pon_framecnt   , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_TXOVERSIZECNT               , &stats->pon_txoversizecnt     , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_RX1519_MAXCNT               , &stats->pon_rx1519_maxcnt     , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_TX1519_MAXCNT               , &stats->pon_tx1519_maxcnt     , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PON_TXFRAMECNT                  , &stats->pon_txframecnt        , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_TXPAUSECNT                      , &stats->pon_txpausecnt        , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_UNDERSIZECNT                    , &stats->pon_undersizecnt      , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_JABBERCNT                       , &stats->pon_jabbercnt         , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_EXTENSIONCNT                    , &stats->pon_extensioncnt      , &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_TXEXTENSIONCNT                  , &stats->pon_txextensioncnt    , &value_h);
  
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PCS_ERROR_SYMBOL, &stats->pon_err_symbol, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PCS_OUTOFSYNC_CNT, &stats->pon_outofsync, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_SLD_ERR_CNT, &stats->pon_sld_err, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_CRC8_ERR_CNT, &stats->pon_crc8_err, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_MAC_DROP_CNT, &stats->pon_mac_drop, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_IPG_DROP_CNT, &stats->pon_ipg_drop, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_PACKET_DRAIN_CNT, &stats->pon_drain, &value_h);
    rt |= aal_pon_mib_get(read_clear, MIB_ID_TX_CRCERR_CNT, &stats->pon_tx_crc_err, &value_h);

    return rt;
}




cs_status epon_request_onu_pon_stats_get_pwrsv(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              read_clear,
    CS_OUT cs_pon_stats_t             *stats
)
{
    cs_status rt = CS_E_OK;
    cs_uint32 value_h;

    rt |= aal_pon_mib_get(0, MIB_ID_UCFRAMECNT            , &stats->pon_frame_cnt        , &value_h);
    rt |= aal_pon_mib_get(0, MIB_ID_PON_MCFRAMECNT        , &stats->pon_mcframe_cnt      , &value_h);
    rt |= aal_pon_mib_get(0, MIB_ID_PON_BCFRAMECNT        , &stats->pon_bcframe_cnt      , &value_h);   

    return rt;
}

cs_status epon_request_onu_pon_stats_clr(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport
)
{
    return aal_pon_mib_reset();
}

cs_status epon_request_onu_pon_mac_addr_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_mac_t                    mac
)
{
    if(mac.addr[0] & 0x1)
    {
        SDL_MIN_LOG("invalid unicast mac address. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    aal_pon_mac_addr_set(mac);

    return CS_E_OK;
}

cs_status epon_request_onu_pon_mac_addr_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_mac_t                    *mac
)
{
    if(NULL==mac)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    } 

    aal_pon_mac_addr_get(mac);

    return CS_E_OK;
}

cs_status epon_request_onu_pon_laser_mode_set(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32            device,
    CS_IN cs_int32            llidport,
    CS_IN cs_sdl_pon_laser_mode_t mode
)
{
    cs_aal_pon_laser_cfg_t cfg;
    cs_aal_pon_laser_msk_t mask;

    mask.u32 = 0;
    mask.s.laser_mode = 1;
    cfg.laser_mode = mode;
    aal_pon_laser_cfg_set(mask, &cfg);

    return CS_E_OK;
}

cs_status epon_request_onu_pon_laser_mode_get(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32            device,
    CS_IN cs_int32            llidport,
    CS_OUT cs_sdl_pon_laser_mode_t *mode
)
{
    cs_aal_pon_laser_cfg_t cfg;

    if(NULL==mode)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    } 

    aal_pon_laser_cfg_get(&cfg);

    *mode = cfg.laser_mode;

    return CS_E_OK;
}

cs_status epon_request_onu_pon_link_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *link
)
{
    if(NULL==link)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    } 

    aal_pon_link_get(link);

    return CS_E_OK;
}

cs_status epon_request_onu_pon_rx_enable_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              enable
)
{
    cs_aal_pon_mac_cfg_t cfg;
    cs_aal_pon_mac_msk_t mask;

    mask.u32 = 0;
    mask.s.rx_en = 1;
    cfg.rx_en = enable;

    aal_pon_mac_cfg_set(mask, &cfg);

    return CS_E_OK;
}

cs_status epon_request_onu_pon_rx_enable_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *enable
)
{
    cs_aal_pon_mac_cfg_t cfg;

    if(NULL==enable)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    } 
    
    aal_pon_mac_cfg_get(&cfg);

    *enable = cfg.rx_en;

    return CS_E_OK;
}

cs_status epon_request_onu_pon_tx_enable_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              enable
)
{
    cs_aal_pon_mac_cfg_t cfg;
    cs_aal_pon_mac_msk_t mask;

    mask.u32 = 0;
    mask.s.tx_drain_en = 1;
    cfg.tx_drain_en = !enable;

    aal_pon_mac_cfg_set(mask, &cfg);

    return CS_E_OK;
}

cs_status epon_request_onu_pon_tx_enable_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *enable
)
{
    cs_aal_pon_mac_cfg_t cfg;

    if(NULL==enable)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    } 
    
    aal_pon_mac_cfg_get(&cfg);
    *enable = !cfg.tx_drain_en;

    return CS_E_OK;
}

cs_status epon_request_onu_pon_mtu_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint16               mtu
)
{
    cs_aal_pon_mac_cfg_t cfg;
    cs_aal_pon_mac_msk_t mask;

    if(mtu > __PON_MTU_MAX)
    {
        SDL_MIN_LOG("mtu invalid.(%d) FILE: %s, LINE: %d", mtu, __FILE__, __LINE__);
        return CS_E_PARAM;
    } 

    mask.u32 = 0;
    mask.s.mtu = 1;
    cfg.mtu = mtu;

    aal_pon_mac_cfg_set(mask, &cfg);

    return CS_E_OK;
}

cs_status epon_request_onu_pon_mtu_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_uint16               *mtu
)
{
    cs_aal_pon_mac_cfg_t cfg;

    if(NULL==mtu)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    } 
    
    aal_pon_mac_cfg_get(&cfg);
    *mtu = cfg.mtu;

    return CS_E_OK;
}

cs_status epon_request_onu_pon_preamble_mode_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              preamble
)
{
    cs_aal_pon_mac_cfg_t cfg;
    cs_aal_pon_mac_msk_t mask;

    mask.u32 = 0;
    mask.s.tx_fix_8b_en = 1;
    cfg.tx_fix_8b_en = preamble;

    aal_pon_mac_cfg_set(mask, &cfg);

    return CS_E_OK;
}

cs_status epon_request_onu_pon_preamble_mode_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *preamble
)
{
    cs_aal_pon_mac_cfg_t cfg;

    if(NULL==preamble)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    } 
    
    aal_pon_mac_cfg_get(&cfg);
    *preamble = cfg.tx_fix_8b_en;

    return CS_E_OK;
}

cs_status epon_request_onu_pon_trans_rx_pwr_set(
    CS_IN cs_callback_context_t   context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              en)
{
    cs_uint8  status =0;
    cs_status  rt = CS_E_OK;
    
    if(en)
    {
        status = 1;
    }
        
    rt = cs_gpio_pon_rx_set(context,device,llidport,status);
    if(rt)
    {
        SDL_MIN_LOG("onu_gpio_set_status return %d. FILE: %s, LINE: %d", rt, __FILE__, __LINE__);
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
}

cs_status epon_request_onu_pon_trans_rx_pwr_get(
    CS_IN cs_callback_context_t   context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *en)
{
    cs_uint8  status =0;
    cs_status rt = CS_E_OK;

    rt = cs_gpio_pon_rx_get(context,device,llidport,&status);
    if(rt)
    {
        SDL_MIN_LOG("onu_gpio_set_status return %d. FILE: %s, LINE: %d", rt, __FILE__, __LINE__);
        return CS_E_ERROR;
    }

    if(status){
        *en = 1;
    }
    else{
        *en = 0;
    }
    
    return CS_E_OK;
}


#ifdef HAVE_RESET_CONTROL
void onu_reset_enable_set(cs_boolean enable);
#endif
cs_status epon_request_onu_pon_trans_tx_pwr_set(
    CS_IN cs_callback_context_t   context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              en)
{
    cs_uint8  status =0;
    cs_status  rt = CS_E_OK;
	#ifdef HAVE_GWD_PON_LASER_CONTROL
	cs_boolean enable = false;
	#endif
	
    if(en)
    {
        status = 1;
		#ifdef HAVE_GWD_PON_LASER_CONTROL
		enable = true;
		#endif
    }

	#ifdef HAVE_RESET_CONTROL
	onu_reset_enable_set(enable);
	#endif
    rt = cs_gpio_pon_tx_set(context,device,llidport,status);
    if(rt)
    {
        SDL_MIN_LOG("onu_gpio_set_status return %d. FILE: %s, LINE: %d", rt, __FILE__, __LINE__);
        return CS_E_ERROR;
    }

    return CS_E_OK;
    
}

cs_status epon_request_onu_pon_trans_tx_pwr_get(
    CS_IN cs_callback_context_t   context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *en)
{
    cs_uint8  status =0;
    cs_status rt = CS_E_OK;

    rt = cs_gpio_pon_tx_get(context,device,llidport,&status);
    if(rt)
    {
        SDL_MIN_LOG("onu_gpio_set_status return %d. FILE: %s, LINE: %d", rt, __FILE__, __LINE__);
        return CS_E_ERROR;
    }
 
    if(status)
    {
        *en = 1;
    }
    else
    {
        *en = 0;
    }
    
    return CS_E_OK;
}

void __int_pon_link_change_handler(void)
{
    sdl_event_port_link_t link_event;
    cs_boolean pon_link;

    aal_pon_link_get(&pon_link);
    link_event.link = pon_link;
    link_event.port = CS_PON_PORT_ID;

    //cs_printf("PON LINK %s\n", pon_link ? "UP" : "DOWN");
    sdl_event_send(EPON_EVENT_PORT_LINK_CHANGE, sizeof(sdl_event_port_link_t), &link_event);
}

cs_status sdl_pon_init(
    CS_IN sdl_init_cfg_t    *cfg
)
{
    cs_status rt = CS_E_OK;    
    cs_aal_pon_laser_cfg_t laser_cfg;
    cs_aal_pon_laser_msk_t laser_msk;
    cs_aal_pon_mac_cfg_t mac_cfg;
    cs_aal_pon_mac_msk_t mac_msk;
    cs_uint16 laser_on_time = 0;
    cs_uint16 laser_off_time = 0;
    cs_uint8 laser_always_on = 0;
    cs_uint8 laser_polarity = 0;
    cs_uint8 vendor_id[4];
    cs_mac_t mac;

    // get laser polarity from startup config
    rt = startup_config_read(CFG_ID_PON_LASER_POLARITY, 1, &laser_polarity);
    if(rt != CS_E_OK)
    {
        cs_halt("startup_config_read reutn %u. %s, line: %d\n", rt, __FILE__, __LINE__);
    }
    cfg->laser_polarity = laser_polarity;
    
    // get laser on time from startup config 
    rt = startup_config_read(CFG_ID_LASER_ON_TIME, 2, (cs_uint8 *)&laser_on_time);
    if(rt != CS_E_OK)
    {
        cs_halt("startup_config_read reutn %u. %s, line: %d\n", rt, __FILE__, __LINE__);
    }
    cfg->laser_on_time = laser_on_time*2;

    // get laser off time from startup config 
    rt = startup_config_read(CFG_ID_LASER_OFF_TIME, 2, (cs_uint8 *)&laser_off_time);
    if(rt != CS_E_OK)
    {
        cs_halt("startup_config_read reutn %u. %s, line: %d\n", rt, __FILE__, __LINE__);
    }
    cfg->laser_off_time = laser_off_time*2;

    // get laser always on from startup config 
    rt = startup_config_read(CFG_ID_LASER_ALWAYS_ON, 1, &laser_always_on);
    if(rt != CS_E_OK)
    {
        cs_halt("startup_config_read reutn %u. %s, line: %d\n", rt, __FILE__, __LINE__);
    }
    cfg->laser_always_on= laser_always_on;

    // get vender id from startup config and 
    rt = startup_config_read(CFG_ID_VENDOR_ID, 4, vendor_id);
    if(rt != CS_E_OK)
    {
        cs_halt("startup_config_read reutn %u. %s, line: %d\n", rt, __FILE__, __LINE__);
    }

    // get mac address from startup config and 
    rt = startup_config_read(CFG_ID_MAC_ADDRESS, 6, &mac.addr[0]);
    if(rt != CS_E_OK)
    {
        cs_halt("startup_config_read reutn %u. %s, line: %d\n", rt, __FILE__, __LINE__);
    }

    laser_cfg.tx_burst_polarity = cfg->laser_polarity & 0x1; // bit 0 for tx_burst_polarity
    laser_cfg.sig_loss_polarity = cfg->laser_polarity & 0x2; // bit 1 for sig_loss_polarity
    laser_cfg.laser_on_time = cfg->laser_on_time;
    laser_cfg.laser_off_time = cfg->laser_off_time;
    laser_cfg.laser_mode = (cfg->laser_always_on ? AAL_PON_LASER_MODE_ON : AAL_PON_LASER_MODE_BURST);
    laser_cfg.output_en = 1;
    laser_msk.u32 = 0;
    laser_msk.s.tx_burst_polarity = 1;
    laser_msk.s.sig_loss_polarity = 1;
    laser_msk.s.laser_on_off_time = 1;
    laser_msk.s.laser_mode = 1;   
    laser_msk.s.output_en = 1;
    rt = aal_pon_laser_cfg_set(laser_msk, &laser_cfg);
    if(rt != CS_E_OK)
    {
        cs_halt("aal_pon_laser_cfg_set reutn %u. %s, line: %d\n", rt, __FILE__, __LINE__);
    }

    // set to ONU base on startup config
    if((vendor_id[0]==0x37) && 
        (vendor_id[1]==0x21))   // for Samsung OLT (TK3721)
    {
        mac_cfg.tx_ipg = 13;
        mac_cfg.tx_fix_8b_en = 1;
    }
    else if((vendor_id[0]==0x37) && 
            (vendor_id[1]==0x23))   // for TK3723
    {
        mac_cfg.tx_ipg = 12;
        mac_cfg.tx_fix_8b_en = 0;
    }
    else if((vendor_id[0]==0x52) && 
            (vendor_id[1]==0x01))   // for Dasan OLT (PMC chip)
    {
        mac_cfg.tx_ipg = 12;
        mac_cfg.tx_fix_8b_en = 1;
    }
    else /* default */
    {
        mac_cfg.tx_ipg = 12;
        mac_cfg.tx_fix_8b_en = 0;
    }

    mac_msk.u32 = 0;
    mac_msk.s.tx_ipg = 1;
    mac_msk.s.tx_fix_8b_en = 1;
    rt = aal_pon_mac_cfg_set(mac_msk, &mac_cfg);
    if(rt != CS_E_OK)
    {
        cs_halt("aal_pon_mac_cfg_set reutn %u. %s, line: %d\n", rt, __FILE__, __LINE__);
    }

    rt = aal_pon_mac_addr_set(mac);
    if(rt != CS_E_OK)
    {
        cs_halt("aal_pon_mac_addr_set reutn %u. %s, line: %d\n", rt, __FILE__, __LINE__);
    }

    sdl_int_handler_reg(SDL_INT_PON_LINK_CHANGE, __int_pon_link_change_handler);

    return rt; 
}

