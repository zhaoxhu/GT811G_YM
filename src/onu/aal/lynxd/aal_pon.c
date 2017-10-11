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
 * $Id: aal_pon.c,v 1.1.2.4.6.1.16.1.2.20.2.16.4.9.18.2 2012/07/10 05:17:04 chuang Exp $
 */

#include "aal_pon.h"
#include "cs_types.h"
#include "aal_util.h"
#include "aal.h"

#define __RX_IPG_MIN                      5
#define __RX_IPG_MAX                      12

#define __RX_MAX_PKT_SIZE                 2047
#define __TX_MAX_PKT_SIZE                 2047

#define __RX_MIN_SIZE_DISABLE             0
#define __RX_MIN_SIZE_MIN                 2
#define __RX_MIN_SIZE_MAX                 64

#define __LOOP_CNT_MAX                    0xf
#define __LOOP_CNT_MIN                    0x1
 
#define __USER_FIFO_MAX                   1000
#define __USER_FIFO_MIN                   1

#define __MIB_OPCODE_PLAIN_READ           0
#define __MIB_OPCODE_READ_CLEAR_MSB       1
#define __MIB_OPCODE_READ_CLEAR_ALL       2

#define __DEFAULT_MPCP_ENGINE             0

#define __MIB_READ                        0
#define __MIB_WRITE                       1

#define __MIB_ACCESS_TIMEOUT              0x1000

#define __MIB_SHORT_LIMIT_MAX             64
#define __MIB_RMON_TOP_MAX                2047

/* PMAC mib access mutex */
cs_uint32 __pon_mib_mutex;

cs_uint32 __stats_pon_err_symbol;
cs_uint32 __stats_pon_outofsync;
cs_uint32 __stats_pon_sld_err;
cs_uint32 __stats_pon_crc8_err;
cs_uint32 __stats_pon_mac_drop;
cs_uint32 __stats_pon_ipg_drop;
cs_uint32 __stats_pon_drain;
cs_uint32 __stats_pon_tx_crc_err;

cs_status __pmac_mib_read
(
    cs_uint8   op_code,
    cs_uint16  mpcp_eng, 
    cs_uint16  mib_id, 
    cs_uint32* value_lo, 
    cs_uint32* value_hi
)
{
    cs_boolean access;
    cs_uint16 counter_id = (mpcp_eng<<7) | mib_id;
    cs_uint32 timeout = __MIB_ACCESS_TIMEOUT;
    
    // trigger read
    cs_mutex_lock(__pon_mib_mutex);
    REG_FIELD_WRITE(ONU_MAC_MIB_ACCESS, rbw, __MIB_READ);
    REG_FIELD_WRITE(ONU_MAC_MIB_ACCESS, op_code, op_code);
    REG_FIELD_WRITE(ONU_MAC_MIB_ACCESS, counter_id, counter_id);
    REG_FIELD_WRITE(ONU_MAC_MIB_ACCESS, access, 1);
    
    // wait for reading done
    do
    {
        REG_FIELD_READ(ONU_MAC_MIB_ACCESS, access, access);
    }while(access && (--timeout));

    if(access)
    {
        cs_mutex_unlock(__pon_mib_mutex);
        AAL_CRI_LOG("pon mib access failed. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_ERROR;
    }

    // read data
    REG_FIELD_READ(ONU_MAC_MIB_DATA0, cnt_val_lo, *value_lo);
    REG_FIELD_READ(ONU_MAC_MIB_DATA1, cnt_val_hi, *value_hi);

    cs_mutex_unlock(__pon_mib_mutex);
    return CS_E_OK;
}

cs_status aal_pon_mib_get(cs_boolean read_clear, cs_aal_pon_mib_id_t mib_id, cs_uint32 *value_l, cs_uint32 *value_h)
{
    cs_status rt = CS_E_OK;
    cs_uint32 value_lo,value_hi;

    cs_uint32 loc_pon_err_symbol;
    cs_uint32 loc_pon_outofsync;
    cs_uint32 loc_pon_sld_err;
    cs_uint32 loc_pon_crc8_err;
    cs_uint32 loc_pon_mac_drop;
    cs_uint32 loc_pon_ipg_drop;
    cs_uint32 loc_pon_drain;
    cs_uint32 loc_pon_tx_crc_err;

    if((NULL == value_l) || (NULL == value_h)) 
    {
       AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
       return CS_E_PARAM;
    }

    if(MIB_ID_PCS_ERROR_SYMBOL == mib_id)
    {
        REG_FIELD_READ(ONU_MAC_PCS_Error_Symbol_Counter, Pcs_sym_err_cnt, loc_pon_err_symbol);
        if(read_clear)
        {
            __stats_pon_err_symbol = loc_pon_err_symbol;
        }
        else
        {
            __stats_pon_err_symbol += loc_pon_err_symbol;
        }
        *value_l = __stats_pon_err_symbol;
        return CS_E_OK;
    }

    if(MIB_ID_PCS_OUTOFSYNC_CNT == mib_id)
    {
        REG_FIELD_READ(ONU_MAC_PCS_OutofSync_Cnt, Pcs_out_sync_cnt, loc_pon_outofsync);
        if(read_clear)
        {
            __stats_pon_outofsync = loc_pon_outofsync;
        }
        else
        {
            __stats_pon_outofsync += loc_pon_outofsync;
        }
        *value_l = __stats_pon_outofsync;
        return CS_E_OK;
    }

    if(MIB_ID_SLD_ERR_CNT == mib_id)
    {
        REG_FIELD_READ(ONU_MAC_SLD_ERR_CNT, sldErrCnt, loc_pon_sld_err);
        if(read_clear)
        {
            __stats_pon_sld_err = loc_pon_sld_err;
        }
        else
        {
            __stats_pon_sld_err += loc_pon_sld_err;
        }
        *value_l = __stats_pon_sld_err;
        return CS_E_OK;
    }

    if(MIB_ID_CRC8_ERR_CNT == mib_id)
    {
        REG_FIELD_READ(ONU_MAC_CRC8_ERR_CNT, crc8ErrCnt, loc_pon_crc8_err);
        if(read_clear)
        {
            __stats_pon_crc8_err = loc_pon_crc8_err;
        }
        else
        {
            __stats_pon_crc8_err += loc_pon_crc8_err;
        }
        *value_l = __stats_pon_crc8_err;
        return CS_E_OK;
    }

    if(MIB_ID_MAC_DROP_CNT == mib_id)
    {
        REG_FIELD_READ(ONU_MAC_MAC_DROP_CNT, mac_drop_cnt, loc_pon_mac_drop);
        if(read_clear)
        {
            __stats_pon_mac_drop = loc_pon_mac_drop;
        }
        else
        {
            __stats_pon_mac_drop += loc_pon_mac_drop;
        }
        *value_l = __stats_pon_mac_drop;
        return CS_E_OK;
    }

    if(MIB_ID_IPG_DROP_CNT == mib_id)
    {
        REG_FIELD_READ(ONU_MAC_IPG_DROP_CNT, ipg_drp_cnt, loc_pon_ipg_drop);
        if(read_clear)
        {
            __stats_pon_ipg_drop = loc_pon_ipg_drop;
        }
        else
        {
            __stats_pon_ipg_drop += loc_pon_ipg_drop;
        }
        *value_l = __stats_pon_ipg_drop;
        return CS_E_OK;
    }

    if(MIB_ID_PACKET_DRAIN_CNT == mib_id)
    {
        REG_FIELD_READ(ONU_MAC_PACKET_DRAIN_CNT, pkt_drain_cnt, loc_pon_drain);
        if(read_clear)
        {
            __stats_pon_drain = loc_pon_drain;
        }
        else
        {
            __stats_pon_drain += loc_pon_drain;
        }
        *value_l = __stats_pon_drain;
        return CS_E_OK;
    }

    if(MIB_ID_TX_CRCERR_CNT == mib_id)
    {
        REG_FIELD_READ(ONU_MAC_TX_CRCERR_CNT, txcrcErrCnt, loc_pon_tx_crc_err);
        if(read_clear)
        {
            __stats_pon_tx_crc_err = loc_pon_tx_crc_err;
        }
        else
        {
            __stats_pon_tx_crc_err += loc_pon_tx_crc_err;
        }
        *value_l = __stats_pon_tx_crc_err;
        return CS_E_OK;
    }


    rt = __pmac_mib_read(read_clear ? __MIB_OPCODE_READ_CLEAR_ALL : __MIB_OPCODE_PLAIN_READ, 
                        __DEFAULT_MPCP_ENGINE, mib_id, &value_lo, &value_hi);

    if(CS_E_OK == rt)
    {
        *value_l = value_lo;
        *value_h = value_hi;
    }

    return rt;
}

cs_status aal_pon_laser_cfg_set(
    CS_IN cs_aal_pon_laser_msk_t msk,
    CS_IN cs_aal_pon_laser_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(msk.s.tx_en)
    {
        REG_FIELD_WRITE(ONU_MAC_OPTICAL_XCR_CTRL, disable_laser_transmitter_polarity, 1);
        REG_FIELD_WRITE(ONU_MAC_OPTICAL_XCR_CTRL, disable_laser_transmitter, (cfg->tx_en!=0));
    }

    if(msk.s.laser_mode)
    {
        if (cfg->laser_mode > AAL_PON_LASER_MODE_ON)
        {
            AAL_MIN_LOG("laser mode invalid.(%d) FILE: %s, LINE: %d\n", cfg->laser_mode, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_OPTICAL_XCR_CTRL, laser_control, (cs_uint32)cfg->laser_mode);
    }

    if(msk.s.laser_on_off_time)
    {
        if( (cfg->laser_on_time % 2) || (cfg->laser_off_time % 2) 
            || (cfg->laser_on_time > LASER_ON_TIME_MAX) || (cfg->laser_off_time > LASER_OFF_TIME_MAX) )
        {
            AAL_MIN_LOG("laser on/off time invalid.(%d,%d) FILE: %s, LINE: %d\n", cfg->laser_on_time, cfg->laser_off_time, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_ONU_LaserOn_LaserOff_Time, laserOnTime, cfg->laser_on_time);
        REG_FIELD_WRITE(ONU_MAC_ONU_LaserOn_LaserOff_Time, laserOffTime, cfg->laser_off_time);
    }

    if(msk.s.tx_delay)
    {
        if( (cfg->tx_delay % 2) || (cfg->tx_delay > LASER_TX_DELAY_MAX) )
        {
            AAL_MIN_LOG("laser tx delay invalid.(%d) FILE: %s, LINE: %d\n", cfg->tx_delay, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_TXCTRL, laser_delay, cfg->tx_delay);
    }

    if(msk.s.tx_power_en)
    {
        REG_FIELD_WRITE(ONU_MAC_OPTICAL_XCR_CTRL, disable_laser_transmitter_power_polarity, 1);
        REG_FIELD_WRITE(ONU_MAC_OPTICAL_XCR_CTRL, disable_laser_transmitter_power, (cfg->tx_power_en==0));
    }

    if(msk.s.sig_loss_polarity)
    {
        REG_FIELD_WRITE(ONU_MAC_OPTICAL_XCR_CTRL, sig_detect_polarity, (cfg->sig_loss_polarity!=0));
    }

    if(msk.s.tx_burst_polarity)
    {
        REG_FIELD_WRITE(ONU_MAC_OPTICAL_XCR_CTRL, laser_burst_polarity, (cfg->tx_burst_polarity!=0));
    }

    if(msk.s.output_en)
    {
        REG_FIELD_WRITE(ONU_MAC_OPTICAL_XCR_CTRL, tx_oen, (cfg->output_en==0));
    }

    if(msk.s.power_mode)
    {   
        if( cfg->power_mode > AAL_PON_LASER_POWER_MODE_HW )
        {
            AAL_MIN_LOG("laser power mode invalid.(%d) FILE: %s, LINE: %d\n", cfg->power_mode, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_OPTICAL_XCR_CTRL, laser_transmitter_power_control_mode, cfg->power_mode);
    }
    
    if(msk.s.wakeup_time)
    {
        if( cfg->wakeup_time > LASER_WAKEUP_TIME_MAX )
        {
            AAL_MIN_LOG("laser wakeup time invalid.(%d) FILE: %s, LINE: %d\n", cfg->wakeup_time, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_TX_POWER_CTRL_WAKEUP_TIME, wakeup_time, cfg->wakeup_time);
    }

    if(msk.s.shutdown_time)
    {
        if( cfg->shutdown_time > LASER_SHUTDOWN_TIME_MAX )
        {
            AAL_MIN_LOG("laser shutdown time invalid.(%d) FILE: %s, LINE: %d\n", cfg->shutdown_time, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_TX_POWER_CTRL_SHUTDOWN_TIME, shutdown_time, cfg->shutdown_time);
    }

    if(msk.s.keep_time)
    {
        if( cfg->keep_time > LASER_KEEP_TIME_MAX )
        {
            AAL_MIN_LOG("laser keep time invalid.(%d) FILE: %s, LINE: %d\n", cfg->keep_time, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_TX_POWER_CTRL_KEEP_TIME, keep_time, cfg->keep_time);
    }

    return CS_E_OK;
}


cs_status aal_pon_laser_cfg_get(
    CS_OUT cs_aal_pon_laser_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(ONU_MAC_OPTICAL_XCR_CTRL, disable_laser_transmitter, cfg->tx_en); 
    REG_FIELD_READ(ONU_MAC_OPTICAL_XCR_CTRL, laser_control, cfg->laser_mode);
    REG_FIELD_READ(ONU_MAC_ONU_LaserOn_LaserOff_Time, laserOnTime, cfg->laser_on_time);
    REG_FIELD_READ(ONU_MAC_ONU_LaserOn_LaserOff_Time, laserOffTime, cfg->laser_off_time);
    REG_FIELD_READ(ONU_MAC_TXCTRL, laser_delay, cfg->tx_delay);
    REG_FIELD_READ(ONU_MAC_OPTICAL_XCR_CTRL, disable_laser_transmitter_power, cfg->tx_power_en);
    cfg->tx_power_en = !cfg->tx_power_en;
    REG_FIELD_READ(ONU_MAC_OPTICAL_XCR_CTRL, sig_detect_polarity, cfg->sig_loss_polarity);
    REG_FIELD_READ(ONU_MAC_OPTICAL_XCR_CTRL, laser_burst_polarity, cfg->tx_burst_polarity);
    REG_FIELD_READ(ONU_MAC_OPTICAL_XCR_CTRL, tx_oen, cfg->output_en);
    cfg->output_en = !cfg->output_en;
    REG_FIELD_READ(ONU_MAC_OPTICAL_XCR_CTRL, laser_transmitter_power_control_mode, cfg->power_mode);
    REG_FIELD_READ(ONU_MAC_TX_POWER_CTRL_WAKEUP_TIME, wakeup_time, cfg->wakeup_time);
    REG_FIELD_READ(ONU_MAC_TX_POWER_CTRL_SHUTDOWN_TIME, shutdown_time, cfg->shutdown_time);
    REG_FIELD_READ(ONU_MAC_TX_POWER_CTRL_KEEP_TIME, keep_time, cfg->keep_time);

    return CS_E_OK;
}


cs_status aal_pon_mac_cfg_set(
    CS_IN cs_aal_pon_mac_msk_t msk,
    CS_IN cs_aal_pon_mac_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(msk.s.rx_en)
    {
        REG_FIELD_WRITE(ONU_MAC_RXCTRL, RX_EN, (cfg->rx_en!=0));
    }

    if(msk.s.tx_en)
    {
        REG_FIELD_WRITE(ONU_MAC_TXCTRL, TX_EN, (cfg->tx_en!=0));
    }

    if(msk.s.rx_ipg)
    {
        if ( ( cfg->rx_ipg < __RX_IPG_MIN ) || ( cfg->rx_ipg > __RX_IPG_MAX ) )
        {
            AAL_MIN_LOG("rx ipg invalid.(%d) FILE: %s, LINE: %d\n", cfg->rx_ipg, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_RXCTRL, ipg_cnt, (__RX_IPG_MAX-cfg->rx_ipg));
    }

    if(msk.s.tx_ipg)
    {
        cs_uint8 ipg;
        switch(cfg->tx_ipg)
        {
            case 12: ipg=0; break;
            case 11: ipg=1; break;
            case 10: ipg=2; break;
            case 13: ipg=3; break;
            default: 
                AAL_MIN_LOG("tx ipg invalid.(%d) FILE: %s, LINE: %d\n", cfg->tx_ipg, __FILE__, __LINE__);
                return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_TXCTRL, ipg_sel, ipg);
        REG_FIELD_WRITE(BM_PORT_SHAPER_CFG, w_rate, cfg->tx_ipg+8);
    }

    if(msk.s.mtu)
    {
        if (cfg->mtu > __RX_MAX_PKT_SIZE)
        {   
            AAL_MIN_LOG("mtu invalid.(%d) FILE: %s, LINE: %d\n", cfg->mtu, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_RX_MAXPKTSIZE, rx_max_pkt_size, cfg->mtu);
    }

    if(msk.s.mtu_tag_en)
    {
        REG_FIELD_WRITE(ONU_MAC_RXCTRL, max_mtu_tag_en, (cfg->mtu_tag_en!=0));
    }

    if(msk.s.mtu_tag_pattern)
    {
        REG_FIELD_WRITE(ONU_MAC_RXMAC_CFG, mtu_tag_pattern, cfg->mtu_tag_pattern);
    }

    if(msk.s.tx_max_size)
    {
        if (cfg->tx_max_size > __TX_MAX_PKT_SIZE)
        {   
            AAL_MIN_LOG("tx max size invalid.(%d) FILE: %s, LINE: %d\n", cfg->tx_max_size, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_TX_MAXPKTSIZE, tx_max_pkt_size, cfg->tx_max_size);
    }

    if(msk.s.tx_max_limit_en)
    {
        REG_FIELD_WRITE(ONU_MAC_TXCTRL, max_pkt_size_limit, (cfg->tx_max_limit_en!=0));
    }

    if(msk.s.rx_min_size)
    {
        if ( (cfg->rx_min_size!=__RX_MIN_SIZE_DISABLE) 
            && ((cfg->rx_min_size<__RX_MIN_SIZE_MIN)||(cfg->rx_min_size>__RX_MIN_SIZE_MAX)) )
        {
            AAL_MIN_LOG("rx min size invalid.(%d) FILE: %s, LINE: %d\n", cfg->rx_min_size, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_RXMAC_CFG, filtering_threshold, cfg->rx_min_size);
    }

    if(msk.s.tx_drain_en)
    {
        REG_FIELD_WRITE(ONU_MAC_TXCTRL, drain, (cfg->tx_drain_en!=0));
    }

    if(msk.s.tx_fix_8b_en)
    {
        REG_FIELD_WRITE(ONU_MAC_TXCTRL, reserved_1, (cfg->tx_fix_8b_en!=0));
    }

    if(msk.s.user_fifo_hi)
    {
        if((cfg->user_fifo_hi<__USER_FIFO_MIN)||(cfg->user_fifo_hi>__USER_FIFO_MAX))
        {
            AAL_MIN_LOG("user_fifo_hi invalid.(%d) FILE: %s, LINE: %d\n", cfg->user_fifo_hi, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_TXMAC_CFG, usr_fifo_high_threshold, cfg->user_fifo_hi);
    }

    if(msk.s.user_fifo_lo)
    {
        if((cfg->user_fifo_lo<__USER_FIFO_MIN)||(cfg->user_fifo_lo>__USER_FIFO_MAX))
        {
            AAL_MIN_LOG("user_fifo_lo invalid.(%d) FILE: %s, LINE: %d\n", cfg->user_fifo_lo, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_TXMAC_CFG, usr_fifo_low_threshold, cfg->user_fifo_lo);
    }

    if(msk.s.pcs_loop_cnt_hi)
    {
        if((cfg->pcs_loop_cnt_hi<__LOOP_CNT_MIN)||(cfg->pcs_loop_cnt_hi>__LOOP_CNT_MAX))
        {
            AAL_MIN_LOG("pcs_loop_cnt_hi invalid.(%d) FILE: %s, LINE: %d\n", cfg->pcs_loop_cnt_hi, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_GLOBAL_GLB_PCS, PCS_LOOP_CNT_HI, cfg->pcs_loop_cnt_hi);
    }

    if(msk.s.pcs_loop_cnt_lo)
    {
        if((cfg->pcs_loop_cnt_lo<__LOOP_CNT_MIN)||(cfg->pcs_loop_cnt_lo>__LOOP_CNT_MAX))
        {
            AAL_MIN_LOG("pcs_loop_cnt_lo invalid.(%d) FILE: %s, LINE: %d\n", cfg->pcs_loop_cnt_lo, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_GLOBAL_GLB_PCS, PCS_LOOP_CNT_LO, cfg->pcs_loop_cnt_lo);
    }

    if(msk.s.crc32_regen_en)
    {
        REG_FIELD_WRITE(ONU_MAC_TXCTRL, disable_crc32_re_generation, (cfg->crc32_regen_en==0));
    }

    if(msk.s.crc8_check_en)
    {
        REG_FIELD_WRITE(ONU_MAC_LLID_FILTERING_CFG, disable_crc8_checking, (cfg->crc8_check_en==0));
    }

    if(msk.s.pcs_10b_aliagn_en)
    {
        REG_FIELD_WRITE(ONU_MAC_GLOBAL_GLB_SYS_CFG, pon_10balign_en, (cfg->pcs_10b_aliagn_en!=0));
    }

    if(msk.s.fec_sync_en)
    {
        REG_FIELD_WRITE(ONU_MAC_GLOBAL_GLB_SYS_CFG, pon_fec_sync_en, (cfg->fec_sync_en!=0));
    }

    if(msk.s.sfd_mode2)
    {
        if(cfg->sfd_mode2 > AAL_PON_SFD_MODE_ZERO_55_D5)
        {
            AAL_MIN_LOG("sfd_mode2 invalid.(%d) FILE: %s, LINE: %d\n", cfg->sfd_mode2, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_RXCTRL, sfd_mode2, (cs_uint32)(cfg->sfd_mode2));
    }

    if(msk.s.crc8_order_sel)
    {
        if(cfg->crc8_order_sel > AAL_PON_CRC8_MODE_REV_CRC8_DATA)
        {
            AAL_MIN_LOG("crc8_order_sel invalid.(%d) FILE: %s, LINE: %d\n", cfg->crc8_order_sel, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_TXCTRL, crc8_calculation_order_select, (cs_uint32)(cfg->crc8_order_sel));
    }

    return CS_E_OK;
}


cs_status aal_pon_mac_cfg_get(
    CS_OUT cs_aal_pon_mac_cfg_t *cfg
)
{
    cs_uint8 ipg;

    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(ONU_MAC_RXCTRL, RX_EN, cfg->rx_en);
    REG_FIELD_READ(ONU_MAC_TXCTRL, TX_EN, cfg->tx_en);
    REG_FIELD_READ(ONU_MAC_RXCTRL, ipg_cnt, ipg); cfg->rx_ipg = __RX_IPG_MAX-ipg;
    REG_FIELD_READ(ONU_MAC_TXCTRL, ipg_sel, ipg);
    switch(ipg)
    {
        case 0: cfg->tx_ipg=12; break;
        case 1: cfg->tx_ipg=11; break;
        case 2: cfg->tx_ipg=10; break;
        case 3: cfg->tx_ipg=13; break;
        default:
            AAL_CRI_LOG("tx ipg read error.(%d) FILE: %s, LINE: %d\n", ipg, __FILE__, __LINE__);             
            return CS_E_ERROR;
    }
    REG_FIELD_READ(ONU_MAC_RX_MAXPKTSIZE, rx_max_pkt_size, cfg->mtu);
    REG_FIELD_READ(ONU_MAC_RXCTRL, max_mtu_tag_en, cfg->mtu_tag_en);
    REG_FIELD_READ(ONU_MAC_RXMAC_CFG, mtu_tag_pattern, cfg->mtu_tag_pattern);
    REG_FIELD_READ(ONU_MAC_TX_MAXPKTSIZE, tx_max_pkt_size, cfg->tx_max_size);
    REG_FIELD_READ(ONU_MAC_TXCTRL, max_pkt_size_limit, cfg->tx_max_limit_en);
    REG_FIELD_READ(ONU_MAC_RXMAC_CFG, filtering_threshold, cfg->rx_min_size);
    REG_FIELD_READ(ONU_MAC_TXCTRL, drain, cfg->tx_drain_en);
    REG_FIELD_READ(ONU_MAC_TXCTRL, reserved_1, cfg->tx_fix_8b_en);
    REG_FIELD_READ(ONU_MAC_TXMAC_CFG, usr_fifo_high_threshold, cfg->user_fifo_hi);
    REG_FIELD_READ(ONU_MAC_TXMAC_CFG, usr_fifo_low_threshold, cfg->user_fifo_lo);
    REG_FIELD_READ(ONU_MAC_GLOBAL_GLB_PCS, PCS_LOOP_CNT_HI, cfg->pcs_loop_cnt_hi);
    REG_FIELD_READ(ONU_MAC_GLOBAL_GLB_PCS, PCS_LOOP_CNT_LO, cfg->pcs_loop_cnt_lo);
    REG_FIELD_READ(ONU_MAC_TXCTRL, disable_crc32_re_generation, cfg->crc32_regen_en);
    cfg->crc32_regen_en = !cfg->crc32_regen_en;
    REG_FIELD_READ(ONU_MAC_LLID_FILTERING_CFG, disable_crc8_checking, cfg->crc8_check_en);
    cfg->crc8_check_en = !cfg->crc8_check_en;
    REG_FIELD_READ(ONU_MAC_GLOBAL_GLB_SYS_CFG, pon_10balign_en, cfg->pcs_10b_aliagn_en);
    REG_FIELD_READ(ONU_MAC_GLOBAL_GLB_SYS_CFG, pon_fec_sync_en, cfg->fec_sync_en);
    REG_FIELD_READ(ONU_MAC_RXCTRL, sfd_mode2, cfg->sfd_mode2);
    REG_FIELD_READ(ONU_MAC_TXCTRL, crc8_calculation_order_select, cfg->crc8_order_sel);

    return CS_E_OK;
}


cs_status aal_pon_llid_filter_cfg_set(
    CS_IN cs_aal_pon_llid_filter_msk_t msk,
    CS_IN cs_aal_pon_llid_filter_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(msk.s.drop_ffff)
    {
        REG_FIELD_WRITE(ONU_MAC_LLID_FILTERING_CFG, PON_Port_only_dis_bcast_llid, (cfg->drop_ffff!=0));
    }

    if(msk.s.drop_7fff)
    {
        REG_FIELD_WRITE(ONU_MAC_LLID_FILTERING_CFG, PON_Port_only_rx_7FFF_llid_disable, (cfg->drop_7fff!=0));
    }

    if(msk.s.drop_p2p)
    {
        REG_FIELD_WRITE(ONU_MAC_LLID_FILTERING_CFG, disable_p2p_llid_filtering, (cfg->drop_p2p!=0));
    }

    if(msk.s.drop_myslef)
    {
        REG_FIELD_WRITE(ONU_MAC_LLID_FILTERING_CFG, PON_Port_only_dis_unicast_llid, (cfg->drop_myslef!=0));
    }

    if(msk.s.accept_all)
    {
        REG_FIELD_WRITE(ONU_MAC_LLID_FILTERING_CFG, disable_LLID_filtering, (cfg->accept_all!=0));
    }

    if(msk.s.accept_cs)
    {
        REG_FIELD_WRITE(ONU_MAC_LLID_FILTERING_CFG, enable_cs_llid_filtering, (cfg->accept_cs!=0));
    }

    return CS_E_OK;
}


cs_status aal_pon_llid_filter_cfg_get(
    CS_OUT cs_aal_pon_llid_filter_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(ONU_MAC_LLID_FILTERING_CFG, PON_Port_only_dis_bcast_llid, cfg->drop_ffff);
    REG_FIELD_READ(ONU_MAC_LLID_FILTERING_CFG, PON_Port_only_rx_7FFF_llid_disable, cfg->drop_7fff);
    REG_FIELD_READ(ONU_MAC_LLID_FILTERING_CFG, disable_p2p_llid_filtering, cfg->drop_p2p);
    REG_FIELD_READ(ONU_MAC_LLID_FILTERING_CFG, PON_Port_only_dis_unicast_llid, cfg->drop_myslef);
    REG_FIELD_READ(ONU_MAC_LLID_FILTERING_CFG, disable_LLID_filtering, cfg->accept_all);
    REG_FIELD_READ(ONU_MAC_LLID_FILTERING_CFG, enable_cs_llid_filtering, cfg->accept_cs);

    return CS_E_OK;
}


cs_status aal_pon_cs_llid_pattern_set(
    CS_IN cs_aal_pon_filterid_t id,
    CS_IN cs_uint16 mask,
    CS_IN cs_uint16 pattern
)
{
    if(id > AAL_PON_FILTER_ID_7)
    {
        AAL_MIN_LOG("llid filter id invalid.(%d) FILE: %s, LINE: %d\n", id, __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    switch (id)
    {
        case AAL_PON_FILTER_ID_0:
            REG_FIELD_WRITE(ONU_MAC_CS0_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, mask);
            REG_FIELD_WRITE(ONU_MAC_CS0_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, pattern);
            break;
        case AAL_PON_FILTER_ID_1:
            REG_FIELD_WRITE(ONU_MAC_CS1_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, mask);
            REG_FIELD_WRITE(ONU_MAC_CS1_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, pattern);
            break;
        case AAL_PON_FILTER_ID_2:
            REG_FIELD_WRITE(ONU_MAC_CS2_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, mask);
            REG_FIELD_WRITE(ONU_MAC_CS2_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, pattern);
            break;
        case AAL_PON_FILTER_ID_3:
            REG_FIELD_WRITE(ONU_MAC_CS3_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, mask);
            REG_FIELD_WRITE(ONU_MAC_CS3_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, pattern);
            break;
        case AAL_PON_FILTER_ID_4:
            REG_FIELD_WRITE(ONU_MAC_CS4_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, mask);
            REG_FIELD_WRITE(ONU_MAC_CS4_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, pattern);
            break;
        case AAL_PON_FILTER_ID_5:
            REG_FIELD_WRITE(ONU_MAC_CS5_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, mask);
            REG_FIELD_WRITE(ONU_MAC_CS5_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, pattern);
            break;
        case AAL_PON_FILTER_ID_6:
            REG_FIELD_WRITE(ONU_MAC_CS6_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, mask);
            REG_FIELD_WRITE(ONU_MAC_CS6_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, pattern);
            break;
        case AAL_PON_FILTER_ID_7:
            REG_FIELD_WRITE(ONU_MAC_CS7_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, mask);
            REG_FIELD_WRITE(ONU_MAC_CS7_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, pattern);
            break;
        default:
            AAL_CRI_LOG("llid filter id error.(%d) FILE: %s, LINE: %d\n", id, __FILE__, __LINE__);
            return CS_E_ERROR;
    }

    return CS_E_OK;
}


cs_status aal_pon_cs_llid_pattern_get(
    CS_IN cs_aal_pon_filterid_t id,
    CS_OUT cs_uint16 *mask,
    CS_OUT cs_uint16 *pattern
)
{
    if (id > AAL_PON_FILTER_ID_7)
    {
        AAL_MIN_LOG("llid filter id invalid.(%d) FILE: %s, LINE: %d\n", id, __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if ( (NULL == mask) || (NULL == pattern) )
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    switch (id)
    {
        case AAL_PON_FILTER_ID_0:
            REG_FIELD_READ(ONU_MAC_CS0_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, (*mask));
            REG_FIELD_READ(ONU_MAC_CS0_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, (*pattern));
            break;
        case AAL_PON_FILTER_ID_1:
            REG_FIELD_READ(ONU_MAC_CS1_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, (*mask));
            REG_FIELD_READ(ONU_MAC_CS1_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, (*pattern));
            break;
        case AAL_PON_FILTER_ID_2:
            REG_FIELD_READ(ONU_MAC_CS2_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, (*mask));
            REG_FIELD_READ(ONU_MAC_CS2_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, (*pattern));
            break;
        case AAL_PON_FILTER_ID_3:
            REG_FIELD_READ(ONU_MAC_CS3_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, (*mask));
            REG_FIELD_READ(ONU_MAC_CS3_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, (*pattern));
            break;
        case AAL_PON_FILTER_ID_4:
            REG_FIELD_READ(ONU_MAC_CS4_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, (*mask));
            REG_FIELD_READ(ONU_MAC_CS4_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, (*pattern));
            break;
        case AAL_PON_FILTER_ID_5:
            REG_FIELD_READ(ONU_MAC_CS5_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, (*mask));
            REG_FIELD_READ(ONU_MAC_CS5_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, (*pattern));
            break;
        case AAL_PON_FILTER_ID_6:
            REG_FIELD_READ(ONU_MAC_CS6_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, (*mask));
            REG_FIELD_READ(ONU_MAC_CS6_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, (*pattern));
            break;
        case AAL_PON_FILTER_ID_7:
            REG_FIELD_READ(ONU_MAC_CS7_LLID_Filtering_Pattern_Mask, CS_LLID_MSK, (*mask));
            REG_FIELD_READ(ONU_MAC_CS7_LLID_Filtering_Pattern_Mask, CS_LLID_pattern, (*pattern));
            break;
        default:
            AAL_CRI_LOG("llid filter id error.(%d) FILE: %s, LINE: %d\n", id, __FILE__, __LINE__);
            return CS_E_ERROR;
    }

    return CS_E_OK;
}

cs_status aal_pon_drop_policy_set(
    CS_IN cs_aal_pon_drop_t msk,
    CS_IN cs_aal_pon_drop_t *cfg
)
{
    cs_uint32 drop;
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;    
    }
     
    REG_FIELD_READ(ONU_MAC_RXCTRL, drop_mask, drop);
    drop &= (~msk.u32);
    drop |= (msk.u32&cfg->u32);
    REG_FIELD_WRITE(ONU_MAC_RXCTRL, drop_mask, drop);
      
    return CS_E_OK;
}

cs_status aal_pon_drop_policy_get(
    CS_OUT cs_aal_pon_drop_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    REG_FIELD_READ(ONU_MAC_RXCTRL, drop_mask, cfg->u32);
    return CS_E_OK;
}

cs_status aal_pon_link_get(cs_boolean *link)
{
    if(NULL==link)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    REG_FIELD_READ(ONU_MAC_PCS_Status_Reg, Link_status, *link);
    return CS_E_OK;
}


cs_status aal_pon_mib_cfg_set(
    CS_IN cs_aal_pon_mib_msk_t msk,
    CS_IN cs_aal_pon_mib_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(msk.s.mib_mode)
    {
        if (cfg->mib_mode > AAL_PON_MIB_MODE_RFC2819) return CS_E_PARAM;
        REG_FIELD_WRITE(ONU_MAC_MIB_CFG, mib_mode, cfg->mib_mode);
    }

    if(msk.s.short_limit)
    {
        if (cfg->short_limit > __MIB_SHORT_LIMIT_MAX) return CS_E_PARAM;
        REG_FIELD_WRITE(ONU_MAC_MIB_CFG, too_short_limit, cfg->short_limit);
    }

    if(msk.s.rmon_top)
    {
        if (cfg->rmon_top > __MIB_RMON_TOP_MAX) return CS_E_PARAM;
        REG_FIELD_WRITE(ONU_MAC_MIB_CFG, rmon_top, cfg->rmon_top);
    }

    return CS_E_OK;
}


cs_status aal_pon_mib_cfg_get(
    CS_OUT cs_aal_pon_mib_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(ONU_MAC_MIB_CFG, mib_mode, cfg->mib_mode);
    REG_FIELD_READ(ONU_MAC_MIB_CFG, too_short_limit, cfg->short_limit);
    REG_FIELD_READ(ONU_MAC_MIB_CFG, rmon_top, cfg->rmon_top);

    return CS_E_OK;
}


cs_status aal_pon_serdes_cfg_set(
    CS_IN cs_aal_pon_serdes_msk_t msk,
    CS_IN cs_aal_pon_serdes_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(msk.s.rx_latency)
    {
        REG_FIELD_WRITE(ONU_MAC_SERDES_LATENCY, rx_latency, cfg->rx_latency);
    }

    if(msk.s.tx_latency)
    {
        REG_FIELD_WRITE(ONU_MAC_SERDES_LATENCY, tx_latency, cfg->tx_latency);
    }
    
    return CS_E_OK;
}

cs_status aal_pon_serdes_cfg_get(
    CS_OUT cs_aal_pon_serdes_cfg_t *cfg
)
{
    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    REG_FIELD_READ(ONU_MAC_SERDES_LATENCY, rx_latency, cfg->rx_latency);
    REG_FIELD_READ(ONU_MAC_SERDES_LATENCY, tx_latency, cfg->tx_latency);
    
    return CS_E_OK;
}


cs_status aal_pon_bypass_fe_cfg_set(
    CS_IN cs_aal_pon_bypass_fe_msk_t msk,
    CS_IN cs_aal_pon_bypass_fe_cfg_t *cfg
)
{
    cs_uint16 mac_hi;
    cs_uint32 mac_lo;

    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(msk.s.bypass_en)
    {
        REG_FIELD_WRITE(ONU_MAC_RXCTRL, bypass, (cfg->bypass_en!=0));
    }

    if(msk.s.dpid)
    {
        if (cfg->dpid > AAL_PON_BYPASS_DPID_DMA) return CS_E_PARAM;
        REG_FIELD_WRITE(ONU_MAC_RXCTRL, dpid, cfg->dpid);
    }

    if(msk.s.cos)
    {
        if (cfg->dpid > 7)
        {
            AAL_MIN_LOG("dpid invalid.(%d) FILE: %s, LINE: %d\n", cfg->dpid, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_RXCTRL, cos, cfg->cos);
    }

    if(msk.s.bypass_mask)
    {
        REG_FIELD_WRITE(ONU_MAC_RXCTRL_BYPASS_CFG, bypass_mask, cfg->bypass_mask);
    }

    if(msk.s.ext_ethertype)
    {
        REG_FIELD_WRITE(ONU_MAC_RXMAC_EXTENSION_CFG, ethertype, cfg->ext_ethertype);
    }

    if(msk.s.ext_opcode)
    {
        REG_FIELD_WRITE(ONU_MAC_RXMAC_EXTENSION_CFG, opcode, cfg->ext_opcode);
    }

    if(msk.s.ext_oui)
    {
        REG_FIELD_WRITE(ONU_MAC_RXMAC_EXTENSION_OUI, oui, cfg->ext_oui);
    }

    if(msk.s.ext_oui_check_en)
    {
        REG_FIELD_WRITE(ONU_MAC_RXMAC_EXTENSION_OUI, check_oui, (cfg->ext_oui_check_en!=0));
    }

    if(msk.s.ext_da)
    {
        mac_hi = (cfg->ext_da.addr[0] << 8) | cfg->ext_da.addr[1];
        mac_lo = (cfg->ext_da.addr[2] << 24) | (cfg->ext_da.addr[3] << 16) | (cfg->ext_da.addr[4] << 8) | cfg->ext_da.addr[5];
        REG_FIELD_WRITE(ONU_MAC_RXMAC_EXTENSION_DA0, extda, mac_lo);
        REG_FIELD_WRITE(ONU_MAC_RXMAC_EXTENSION_DA1, extda, mac_hi);
    }

    return CS_E_OK;
}


cs_status aal_pon_bypass_fe_cfg_get(
    CS_OUT cs_aal_pon_bypass_fe_cfg_t *cfg
)
{
    cs_uint16 mac_hi;
    cs_uint32 mac_lo;

    if(NULL == cfg)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    REG_FIELD_READ(ONU_MAC_RXCTRL, bypass, cfg->bypass_en);
    REG_FIELD_READ(ONU_MAC_RXCTRL, dpid, cfg->dpid);
    REG_FIELD_READ(ONU_MAC_RXCTRL, cos, cfg->cos);
    REG_FIELD_READ(ONU_MAC_RXCTRL_BYPASS_CFG, bypass_mask, cfg->bypass_mask);
    REG_FIELD_READ(ONU_MAC_RXMAC_EXTENSION_CFG, ethertype, cfg->ext_ethertype);
    REG_FIELD_READ(ONU_MAC_RXMAC_EXTENSION_CFG, opcode, cfg->ext_opcode);
    REG_FIELD_READ(ONU_MAC_RXMAC_EXTENSION_OUI, oui, cfg->ext_oui);
    REG_FIELD_READ(ONU_MAC_RXMAC_EXTENSION_OUI, check_oui, cfg->ext_oui_check_en);
    REG_FIELD_READ(ONU_MAC_RXMAC_EXTENSION_DA0, extda, mac_lo);
    REG_FIELD_READ(ONU_MAC_RXMAC_EXTENSION_DA1, extda, mac_hi);
    cfg->ext_da.addr[5] = mac_lo & 0xff;
    cfg->ext_da.addr[4] = (mac_lo >> 8)& 0xff;
    cfg->ext_da.addr[3] = (mac_lo >> 16)& 0xff;
    cfg->ext_da.addr[2] = (mac_lo >>24)& 0xff;
    cfg->ext_da.addr[1] = mac_hi & 0xff;
    cfg->ext_da.addr[0] = (mac_hi >> 8)& 0xff;

    return CS_E_OK;
}


cs_status aal_pon_mac_addr_set(
    CS_IN cs_mac_t mac
)
{
    cs_uint16 mac_hi = (mac.addr[0] << 8) | mac.addr[1];
    cs_uint32 mac_lo = (mac.addr[2] << 24) | (mac.addr[3] << 16) | (mac.addr[4] << 8) | mac.addr[5];

    if((mac_hi == 0) &&(mac_lo == 0))
    {
        AAL_MIN_LOG("mac address invalid. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_WRITE(GLOBAL_ONU_MAC_ADDR1, sa, mac_hi);
    REG_FIELD_WRITE(GLOBAL_ONU_MAC_ADDR0, sa, mac_lo);

    return CS_E_OK;
}


cs_status aal_pon_mac_addr_get(
    CS_IN cs_mac_t *mac
)
{
    cs_uint16 mac_hi;
    cs_uint32 mac_lo;

    if(NULL==mac)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(GLOBAL_ONU_MAC_ADDR1, sa, mac_hi);
    REG_FIELD_READ(GLOBAL_ONU_MAC_ADDR0, sa, mac_lo);

    mac->addr[5] = mac_lo & 0xff;
    mac->addr[4] = (mac_lo >> 8)& 0xff;
    mac->addr[3] = (mac_lo >> 16)& 0xff;
    mac->addr[2] = (mac_lo >> 24)& 0xff;
    mac->addr[1] = mac_hi & 0xff;
    mac->addr[0] = (mac_hi >> 8)& 0xff;

    return CS_E_OK;
}

cs_status aal_pon_mib_reset(void)
{

    REG_FIELD_READ(ONU_MAC_PCS_Error_Symbol_Counter, Pcs_sym_err_cnt, __stats_pon_err_symbol);
    REG_FIELD_READ(ONU_MAC_PCS_OutofSync_Cnt, Pcs_out_sync_cnt, __stats_pon_outofsync);
    REG_FIELD_READ(ONU_MAC_SLD_ERR_CNT, sldErrCnt, __stats_pon_sld_err);
    REG_FIELD_READ(ONU_MAC_CRC8_ERR_CNT, crc8ErrCnt, __stats_pon_crc8_err);
    REG_FIELD_READ(ONU_MAC_MAC_DROP_CNT, mac_drop_cnt, __stats_pon_mac_drop);
    REG_FIELD_READ(ONU_MAC_IPG_DROP_CNT, ipg_drp_cnt, __stats_pon_ipg_drop);
    REG_FIELD_READ(ONU_MAC_PACKET_DRAIN_CNT, pkt_drain_cnt, __stats_pon_drain);
    REG_FIELD_READ(ONU_MAC_TX_CRCERR_CNT, txcrcErrCnt, __stats_pon_tx_crc_err);

    __stats_pon_err_symbol = 0;
    __stats_pon_outofsync = 0;
    __stats_pon_sld_err = 0;
    __stats_pon_crc8_err = 0;
    __stats_pon_mac_drop = 0;
    __stats_pon_ipg_drop = 0;
    __stats_pon_drain = 0;
    __stats_pon_tx_crc_err = 0;

    cs_mutex_lock(__pon_mib_mutex);

    REG_FIELD_WRITE(ONU_MAC_MIB_RESET, mpcp, 0);
    REG_FIELD_WRITE(ONU_MAC_MIB_RESET, clear, 1);

    cs_mutex_unlock(__pon_mib_mutex);

    return CS_E_OK;
}

void aal_pon_int_disable(cs_aal_pon_int_msk_t mask)
{
    cs_uint32 int_en;
    REG_READ(ONU_MAC_INTERRUPT_ENABLE, int_en);
    int_en &= (~mask.u32);
    REG_WRITE(ONU_MAC_INTERRUPT_ENABLE, int_en);
}

void aal_pon_int_clr(cs_aal_pon_int_msk_t mask)
{
    REG_WRITE(ONU_MAC_INTERRUPT_SOURCE, mask.u32);
}

void aal_pon_int_src_get(cs_aal_pon_int_msk_t *mask)
{
    cs_uint32 int_src;
    if(NULL == mask) return;
    REG_READ(ONU_MAC_INTERRUPT_SOURCE, int_src);
    mask->u32 = int_src;
}

void aal_pon_int_en_get(cs_aal_pon_int_msk_t *mask)
{
    cs_uint32 int_en;
    if(NULL == mask) return;
    REG_READ(ONU_MAC_INTERRUPT_ENABLE, int_en);
    mask->u32 = int_en;
}

void aal_pon_int_en_set(cs_aal_pon_int_msk_t mask)
{
    cs_uint32 int_en;
    REG_READ(ONU_MAC_INTERRUPT_ENABLE, int_en);
    int_en |= mask.u32;
    REG_WRITE(ONU_MAC_INTERRUPT_ENABLE, int_en);
}

cs_status aal_pon_clk_switch(cs_aal_pon_clk_t clock)
{
    if(clock > AAL_PON_CLK_RECOVERY) return CS_E_PARAM;
    REG_FIELD_WRITE(GLOBAL_ONU_CTRL, tck_sel, clock);
    return CS_E_OK;
}

cs_status aal_pon_clk_switch_mode_set(cs_aal_pon_clk_switch_t mode)
{
    if(mode > AAL_PON_CLK_SWITCH_HW) return CS_E_PARAM;
    REG_FIELD_WRITE(GLOBAL_ONU_CTRL, tck_sel_los, mode);
    return CS_E_OK;
}

cs_status aal_pon_loopback_set(cs_boolean enable)
{
    REG_FIELD_WRITE(ONU_MAC_RXCTRL, dpid, 1);
    REG_FIELD_WRITE(ONU_MAC_RXCTRL, cos, 0);
    REG_FIELD_WRITE(ONU_MAC_RXCTRL, bypass, (enable!=0));

    return CS_E_OK;
}

cs_status aal_pon_loopback_get(cs_boolean *enable)
{
    cs_boolean loopback;
    if(NULL == enable)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    REG_FIELD_READ(ONU_MAC_RXCTRL, bypass, loopback);
    *enable = loopback;

    return CS_E_OK;
}

cs_status pon_init(void)
{
    cs_status rt = CS_E_OK;
    cs_aal_pon_laser_msk_t laser_mask;
    cs_aal_pon_laser_cfg_t laser_cfg;
    
    cs_aal_pon_mac_msk_t mac_mask;
    cs_aal_pon_mac_cfg_t mac_cfg; 

    cs_aal_pon_serdes_msk_t serdes_msk;
    cs_aal_pon_serdes_cfg_t serdes_cfg;

    // init pon mib table access mutex
    rt = cs_mutex_init(&__pon_mib_mutex, "pon_mib_mutex", 0);
    if(rt!=0)
    {
        cs_halt("cs_mutex_init return error.(%d) FILE: %s, LINE: %d\n", rt, __FILE__, __LINE__);
    }

    // laser init
    laser_cfg.sig_loss_polarity = 0;
    laser_cfg.tx_burst_polarity = 0;
    laser_cfg.tx_en = 1;
    laser_cfg.laser_mode = AAL_PON_LASER_MODE_BURST;
    laser_cfg.tx_delay = 50;
    laser_cfg.output_en = 0;
    laser_mask.u32 = 0;
    laser_mask.s.sig_loss_polarity = 1;
    laser_mask.s.tx_burst_polarity = 1;
    laser_mask.s.tx_en = 1;
    laser_mask.s.laser_mode = 1;
    laser_mask.s.tx_delay = 1;
    laser_mask.s.output_en = 1;
    aal_pon_laser_cfg_set(laser_mask, &laser_cfg);

    // mac init
    mac_cfg.rx_ipg = 6;
    mac_cfg.tx_ipg = 12;
    mac_mask.u32 = 0;
    mac_mask.s.rx_ipg = 1;
    mac_mask.s.tx_ipg = 1;
    aal_pon_mac_cfg_set(mac_mask, &mac_cfg);

    // serdes init (LynxD ASIC)
    serdes_cfg.rx_latency = 12;
    serdes_cfg.tx_latency = 24;
    serdes_msk.u32 = 0;
    serdes_msk.s.rx_latency = 1;
    serdes_msk.s.tx_latency = 1;
    aal_pon_serdes_cfg_set(serdes_msk, &serdes_cfg);

    // reset mib
    aal_pon_mib_reset();

    return rt;
}



