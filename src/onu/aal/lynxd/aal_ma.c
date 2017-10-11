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

#include "aal_ma.h"
#include "aal_util.h"
#include "aal.h"

#define __RX_BUF_HEAD_LEN 4

#define __RX_BUF_LEN_MAX 2048
#define __TX_BUF_LEN_MAX 2048

#define __MII_RX_IPG_MAX 8
#define __MII_TX_IPG_MAX 14
#define __MII_TX_IPG_MIN 11

#define __TX_BUF_TIMEOUT 0x1000

#define __MA_TX_PRI_MAX 7

#define __MA_TX_MIN_SIZE 60

#define __MIB_ACCESS_TIMEOUT 0x1000
#define __MIB_OPCODE_PLAIN_READ           0
#define __MIB_OPCODE_READ_CLEAR_MSB       1
#define __MIB_OPCODE_READ_CLEAR_ALL       2
#define __MIB_READ                        0
#define __MIB_WRITE                       1

typedef enum
{
    __MA_DPID_MII0  = 0,
    __MA_DPID_NI    = 1,
}__ma_dpid;

/* PMAC mib access mutex */
cs_uint32 __ma_mib_mutex;
cs_uint32 __ma_tx_mutex;


cs_status aal_ma_rx_buf_read(cs_aal_ma_rx_buf_id_t buf_id, cs_aal_ma_rx_buf_t *buf_to, cs_uint16 len)
{
    cs_uint32 buf_addr;
    cs_uint32 *p;
    volatile cs_uint32 *buf_from;
    int i;
    if(buf_id > AAL_MA_RX_BUF1) 
    {
        AAL_MIN_LOG("buf_id invalid.(%d) FILE: %s, LINE: %d\n", buf_id, __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    if(NULL == buf_to)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    if(NULL == buf_to->pkt)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(len > __RX_BUF_LEN_MAX)
    {
        AAL_MIN_LOG("buffer length invalid.(%d) FILE: %s, LINE: %d\n", len, __FILE__, __LINE__);
        return CS_E_PARAM;
    }


    /* get ASIC buffer address base on buffer id */
    if(AAL_MA_RX_BUF0 == buf_id)
    {
        REG_ADDR_GET(MA_RXBUF0, buf_addr);
    }
    else
    {
        REG_ADDR_GET(MA_RXBUF1, buf_addr);
    }

    buf_to->head = *(volatile cs_uint32 *)buf_addr;
    buf_addr += __RX_BUF_HEAD_LEN;
    len -= 4;
    buf_from = (volatile cs_uint32 *)buf_addr;

    /* copy data from asic to memory */
    p = (cs_uint32 *)(&buf_to->pkt->data+CS_PKT_OFFSET);

    for (i = 0; i < ((len >> 2) + (0 != (len&0x3))); ++i)
    {
        *p = *buf_from;
        ++p;
        ++buf_from;
    }

    return CS_E_OK;
}

cs_status aal_ma_rx_buf_clear(cs_aal_ma_rx_buf_id_t buf_id)
{
    MA_RX_BUF0_STAT_t   data;
    if(buf_id > AAL_MA_RX_BUF1)
    {
        AAL_MIN_LOG("buf_id invalid.(%d) FILE: %s, LINE: %d\n", buf_id, __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if (AAL_MA_RX_BUF0 == buf_id)
    {   
        REG_READ(MA_RX_BUF0_STAT, data.wrd);
        data.bf.len_err = 0;
        data.bf.turn = 0;
        data.bf.pkt_rdy = 0;
        REG_WRITE(MA_RX_BUF0_STAT, data.wrd);
    }
    else
    {
        REG_READ(MA_RX_BUF1_STAT, data.wrd);
        data.bf.len_err = 0;
        data.bf.turn = 0;
        data.bf.pkt_rdy = 0;
        REG_WRITE(MA_RX_BUF1_STAT, data.wrd);
    }
    
    return CS_E_OK;
}

cs_status aal_ma_rx_buf_status_get(cs_aal_ma_rx_buf_id_t buf_id, cs_aal_ma_rx_buf_status_t *status)
{
    MA_RX_BUF0_STAT_t   data;

    if(buf_id > AAL_MA_RX_BUF1)    
    {
        AAL_MIN_LOG("buf_id invalid.(%d) FILE: %s, LINE: %d\n", buf_id, __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(NULL==status) 
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if (AAL_MA_RX_BUF0 == buf_id)
    {   
        REG_READ(MA_RX_BUF0_STAT, data.wrd);
    }
    else
    {
        REG_READ(MA_RX_BUF1_STAT, data.wrd);
    }
    
    status->pkt_rdy = data.bf.pkt_rdy ;
    status->turn    = data.bf.turn ;
    status->len_err = data.bf.len_err ;
    status->spid    = data.bf.spid ;
    status->pkt_len = data.bf.pkt_len ;
    
    return CS_E_OK;
}

cs_status aal_ma_tx(cs_uint8 *buf, cs_uint16 len, cs_aal_port_id_t port, cs_uint8 pri)
{
    cs_status ret = CS_E_OK;
    cs_mutex_lock(__ma_tx_mutex);
    ret = aal_ma_tx_no_mutex(buf, len, port, pri);
    cs_mutex_unlock(__ma_tx_mutex); 
    return ret;
}

cs_status aal_ma_tx_no_mutex(cs_uint8 *buf, cs_uint16 len, cs_aal_port_id_t port, cs_uint8 pri)
{
    cs_uint32 data = 0;
    cs_uint32 temp = 0;
    __ma_dpid dpid;
    cs_boolean busy;
    cs_uint32 time_out = __TX_BUF_TIMEOUT;
    int i, j;
    cs_uint32 buf_addr;
    volatile cs_uint32 *buf_to;
    cs_uint32 len_wrd = (len >> 2);

    if(len > (__TX_BUF_LEN_MAX-4)) 
    {
        AAL_MIN_LOG("packet length invalid.(%d) FILE: %s, LINE: %d\n", len, __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    if(NULL==buf)
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(pri > __MA_TX_PRI_MAX) 
    {
        AAL_MIN_LOG("packet priority invalid.(%d) FILE: %s, LINE: %d\n", pri, __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    // port to dpid mapping
    switch(port)
    {
    case AAL_PORT_ID_GE:
    case AAL_PORT_ID_PON:
        dpid = __MA_DPID_NI;
        break;
    case AAL_PORT_ID_MII0:
        dpid = __MA_DPID_MII0;
        break;
    default:
        AAL_MIN_LOG("port id invalid.(%d) FILE: %s, LINE: %d\n", port, __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    REG_FIELD_WRITE_NO_MUTEX(MA_TX_BUF_CTRL, dpid, dpid);
    REG_FIELD_WRITE_NO_MUTEX(MA_CPU_CTRL, cpu_dpid, port);
    REG_FIELD_WRITE_NO_MUTEX(MA_CPU_CTRL, cpu_cos, pri);

    // wait for buffer empty
    do {REG_FIELD_READ(MA_TX_BUF_CTRL, pkt_rdy, busy);}
    while(busy && --time_out);
    if(busy)
    {
        AAL_CRI_LOG("wait for buffer empty timeout. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_TIMEOUT;
    }

    // copy buffer to HW
    REG_ADDR_GET(MA_TXBUF, buf_addr);
    buf_to = (volatile cs_uint32 *)buf_addr;
    for (i = 0; i < len_wrd; ++i) // 4-bytes alignment copy
        *buf_to++ = buf[i*4+0]|(buf[i*4+1]<<8)|(buf[i*4+2]<<16)|(buf[i*4+3]<<24);

    // last word process
    for (j = 0; j < (len&0x3); ++j)
    {
        temp = (cs_uint32)buf[i*4+j];
        data |= (temp<<(8*j));
    }
    *buf_to++ = data;
    
    REG_FIELD_WRITE_NO_MUTEX(MA_TX_BUF_CTRL, pkt_len, len+4);

    // padding if less than 60 bytes
    if(len<__MA_TX_MIN_SIZE)
    {
        // modifiy last non-zero word
        //*(buf_to-1) &= (cs_uint32)(~0)<<(8*(len&3));
        for (i = 0; i < (__MA_TX_MIN_SIZE>>2)-len_wrd; ++i)
            *buf_to++ = 0;
        REG_FIELD_WRITE_NO_MUTEX(MA_TX_BUF_CTRL, pkt_len, __MA_TX_MIN_SIZE+4);
    }

    // trigger send
    REG_FIELD_WRITE_NO_MUTEX(MA_TX_BUF_CTRL, pkt_rdy, 1);

    return CS_E_OK;
}


cs_status aal_ma_mii_drop_set(cs_aal_ma_mii_drop_t msk, cs_aal_ma_mii_drop_t *cfg)
{
    cs_uint16 drop;
    if(NULL == cfg) 
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(MA_CPU_DST1_MII0, pkt_typ_drop, drop);
    drop = (drop | msk.u16) & (msk.u16 & cfg->u16);
    REG_FIELD_WRITE(MA_CPU_DST1_MII0, pkt_typ_drop, drop);

    return CS_E_OK;
}

cs_status aal_ma_mii_drop_get(cs_aal_ma_mii_drop_t *cfg)
{
    if(NULL == cfg) 
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    REG_FIELD_READ(MA_CPU_DST1_MII0, pkt_typ_drop, cfg->u16);
    return CS_E_OK;
}

cs_status aal_ma_mii_forward_set(cs_aal_ma_mii_fw_t msk, cs_aal_ma_mii_fw_t *cfg)
{
    cs_uint16 fw;

    if(NULL == cfg) 
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(MA_CPU_DST_EN_MII0, pkt_type, fw);
    fw = (fw | msk.u16) & (msk.u16 & cfg->u16);
    REG_FIELD_WRITE(MA_CPU_DST_EN_MII0, pkt_type, fw);

    return CS_E_OK;
}

cs_status aal_ma_mii_forward_get(cs_aal_ma_mii_fw_t *cfg)
{
    if(NULL == cfg) 
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    REG_FIELD_READ(MA_CPU_DST_EN_MII0, pkt_type, cfg->u16);
    return CS_E_OK;
}

cs_status aal_ma_mii_cfg_set(cs_aal_ma_mii_msk_t msk, cs_aal_ma_mii_cfg_t *cfg)
{
    cs_uint8 ipg;
    if(NULL == cfg) 
    {
        AAL_MIN_LOG("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(msk.s.mode)
    {
        if(cfg->mode > AAL_MA_MII_MODE_PHY) return CS_E_PARAM;
        REG_FIELD_WRITE(MA_MII0_TX_CFG, smii_phy_mode, cfg->mode);
    }

    if(msk.s.phy_mode)
    {
        if(cfg->phy_mode > AAL_MA_MII_PHY_MODE_RMII) return CS_E_PARAM;
        REG_FIELD_WRITE(MA_MII0_RX_CTRL1, rmii_smii, cfg->phy_mode);
    }

    if(msk.s.speed)
    {
        if(cfg->speed > AAL_MA_MII_SPEED_100M) return CS_E_PARAM;
        REG_FIELD_WRITE(MA_MII0_TX_CTRL, hd_speed, cfg->speed);
    }

    if(msk.s.rx_en)
    {
        REG_FIELD_WRITE(MA_MII0_RX_CTRL, rxen, (cfg->rx_en!=0));
    }

    if(msk.s.tx_en)
    {
        REG_FIELD_WRITE(MA_MII0_TX_CFG, txdat_en, (cfg->tx_en!=0));
    }

    if(msk.s.rx_ipg)
    {
        if(cfg->rx_ipg > __MII_RX_IPG_MAX) return CS_E_PARAM;
        else if(__MII_RX_IPG_MAX == cfg->rx_ipg)
        {
            REG_FIELD_WRITE(MA_MII0_RX_CTRL, ipg_set, 0);
        }
        else
        {
            REG_FIELD_WRITE(MA_MII0_RX_CTRL, ipg_set, 1);
            REG_FIELD_WRITE(MA_MII0_RX_CTRL, ipg_sel, cfg->rx_ipg);
        }
    }

    if(msk.s.tx_ipg)
    {
        if( (cfg->tx_ipg>__MII_TX_IPG_MAX) || (cfg->tx_ipg<__MII_TX_IPG_MIN) )
            return CS_E_PARAM;

        switch(cfg->tx_ipg)
        {
            case 12: ipg=0; break;
            case 11: ipg=1; break;
            case 14: ipg=2; break;
            case 13: ipg=3; break;
            default: return CS_E_ERROR;
        }
        REG_FIELD_WRITE(MA_MII0_TX_CTRL, ipg_sel, ipg);
    }

    if(msk.s.rx_flow_ctrl_en)
    {
        REG_FIELD_WRITE(MA_MII0_RX_CTRL, flow_ctrl, (cfg->rx_flow_ctrl_en!=0));
    }

    if(msk.s.tx_flow_ctrl_en)
    {
        REG_FIELD_WRITE(MA_MII0_TX_CFG, tx_flow_en, (cfg->tx_flow_ctrl_en!=0));
    }

    if(msk.s.crc_regen_en)
    {
        REG_FIELD_WRITE(MA_MII0_TX_CTRL, crc_bypass, (cfg->crc_regen_en==0));
    }

    if(msk.s.loopback_en)
    {
        REG_FIELD_WRITE(MA_MII0_RX_CTRL, lcal_lpbk, (cfg->loopback_en!=0));
    }

    return CS_E_OK;
}

cs_status aal_ma_mii_cfg_get(cs_aal_ma_mii_cfg_t *cfg)
{
    MA_MII0_TX_CTRL_t   mii_tx_ctrl;
    MA_MII0_TX_CFG_t    mii_tx_cfg;
    MA_MII0_RX_CTRL_t   mii_rx_ctrl;
    MA_MII0_RX_CTRL1_t  mii_rx_ctrl1;
    cs_uint8 rx_ipg, tx_ipg;

    REG_READ(MA_MII0_TX_CTRL, mii_tx_ctrl.wrd);
    REG_READ(MA_MII0_TX_CFG, mii_tx_cfg.wrd);
    REG_READ(MA_MII0_RX_CTRL, mii_rx_ctrl.wrd);
    REG_READ(MA_MII0_RX_CTRL1, mii_rx_ctrl1.wrd);

    if(0==mii_rx_ctrl.bf.ipg_set) rx_ipg = __MII_RX_IPG_MAX;
    else rx_ipg = mii_rx_ctrl.bf.ipg_sel;

    switch(mii_tx_ctrl.bf.ipg_sel)
    {
        case 0: tx_ipg=12; break;
        case 1: tx_ipg=11; break;
        case 2: tx_ipg=14; break;
        case 3: tx_ipg=13; break;
        default: return CS_E_ERROR;
    }

    cfg->mode               = (cs_aal_ma_mii_mode_t)mii_tx_cfg.bf.smii_phy_mode;
    cfg->phy_mode           = (cs_aal_ma_mii_phy_mode_t)mii_rx_ctrl1.bf.rmii_smii;
    cfg->speed              = (cs_aal_ma_mii_speed_t)mii_tx_ctrl.bf.hd_speed;
    cfg->rx_en              = mii_rx_ctrl.bf.rxen;
    cfg->tx_en              = mii_tx_cfg.bf.txdat_en;
    cfg->rx_ipg             = rx_ipg;
    cfg->tx_ipg             = tx_ipg;
    cfg->rx_flow_ctrl_en    = mii_rx_ctrl.bf.flow_ctrl;
    cfg->tx_flow_ctrl_en    = mii_tx_cfg.bf.tx_flow_en;
    cfg->crc_regen_en       = !mii_tx_ctrl.bf.crc_bypass;
    cfg->loopback_en        = mii_rx_ctrl.bf.lcal_lpbk;

    return CS_E_OK;
}

cs_status aal_ma_mii_pause_send(cs_mac_t mac, cs_uint16 slot_time)
{
    cs_uint16 mac_hi = (mac.addr[0] << 8) & mac.addr[1];
    cs_uint32 mac_lo = (mac.addr[2] << 24) & (mac.addr[3] << 16) & (mac.addr[4] << 8) & mac.addr[5];

    if(mac_hi + mac_lo == 0)
    {        
        AAL_MIN_LOG("mac address invalid. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    // configure pause frame parameters
    REG_FIELD_WRITE(MA_MII0_FLOW_SA1, mac_addr, mac_hi);
    REG_FIELD_WRITE(MA_MII0_FLOW_SA0, mac_addr, mac_lo);
    REG_FIELD_WRITE(MA_MII0_FLOW_SA1, slot, slot_time);

    if(0==slot_time) // trigger send pause off
    {
        REG_FIELD_WRITE(MA_MII0_TX_CFG, flow_off_req, 1);
        REG_FIELD_WRITE(MA_MII0_TX_CFG, flow_off_req, 0);
    }
    else // trigger send pause on
    {
        REG_FIELD_WRITE(MA_MII0_TX_CFG, flow_on_req, 1);
        REG_FIELD_WRITE(MA_MII0_TX_CFG, flow_on_req, 0);
    }

    return CS_E_OK;
}

void aal_ma_int_en_set(cs_aal_ma_int_msk_t mask)
{
    cs_uint32 int_en;
    REG_READ(MA_INT_EN, int_en);
    int_en |= mask.u32;
    REG_WRITE(MA_INT_EN, int_en);
}

void aal_ma_int_en_get(cs_aal_ma_int_msk_t *mask)
{
    cs_uint32 int_en;
    if(NULL == mask) return;
    REG_READ(MA_INT_EN, int_en);
    mask->u32 = int_en;
}
void aal_ma_int_disable(cs_aal_ma_int_msk_t mask)
{
    cs_uint32 int_en;
    REG_READ(MA_INT_EN, int_en);
    int_en &= (~mask.u32);
    REG_WRITE(MA_INT_EN, int_en);
}

void aal_ma_int_clr(cs_aal_ma_int_msk_t mask)
{
    REG_WRITE(MA_INT, mask.u32);
}

void aal_ma_int_src_get(cs_aal_ma_int_msk_t *mask)
{
    cs_uint32 int_src;
    if(NULL == mask) return;
    REG_READ(MA_INT, int_src);
    mask->u32 = int_src;
}

cs_status __ma_mib_read
(
    cs_uint8   op_code,
    cs_uint16  mib_id, 
    cs_uint32* value_lo, 
    cs_uint32* value_hi
)
{
    cs_boolean access;
    cs_uint32 timeout = __MIB_ACCESS_TIMEOUT;
    
    // trigger read
    cs_mutex_lock(__ma_mib_mutex);
    REG_FIELD_WRITE(MA_MIB_ACCESS, rbw, __MIB_READ);
    REG_FIELD_WRITE(MA_MIB_ACCESS, op_code, op_code);
    REG_FIELD_WRITE(MA_MIB_ACCESS, counter, mib_id);
    REG_FIELD_WRITE(MA_MIB_ACCESS, access, 1);
    
    // wait for reading done
    do
    {
        REG_FIELD_READ(MA_MIB_ACCESS, access, access);
    }while(access && (--timeout));

    if(access)
    {
        cs_mutex_unlock(__ma_mib_mutex);
        AAL_MAJ_LOG("ma mib read timeout. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_ERROR;
    }

    // read data
    REG_FIELD_READ(MA_MIB_DATA0, cnt_val_lo, *value_lo);
    REG_FIELD_READ(MA_MIB_DATA1, cnt_val_hi, *value_hi);

    cs_mutex_unlock(__ma_mib_mutex);
    return CS_E_OK;
}

cs_status aal_ma_mib_get(cs_boolean read_clear, cs_aal_ma_mib_id_t mib_id, cs_uint32 *value_l, cs_uint32 *value_h)
{
    cs_status rt = CS_E_OK;
    cs_uint32 value_lo,value_hi;

    rt = __ma_mib_read((read_clear?__MIB_OPCODE_READ_CLEAR_ALL:__MIB_OPCODE_PLAIN_READ), mib_id, &value_lo, &value_hi);

    if(CS_E_OK == rt)
    {
        *value_l = value_lo;
        *value_h = value_hi;
    }

    return rt;
}

cs_status aal_ma_mib_reset(void)
{
    cs_mutex_lock(__ma_mib_mutex);
    REG_FIELD_WRITE(MA_INIT, mib, 1);
    cs_mutex_unlock(__ma_mib_mutex);

    return CS_E_OK;
}

cs_status ma_init(void)
{    
    cs_status rt = CS_E_OK;
    cs_aal_ma_mii_drop_t drop;
    cs_aal_ma_mii_fw_t fw;

    rt = cs_mutex_init(&__ma_mib_mutex, "MA MIB mutex", 0);
    if(rt!=0)
    {
        cs_halt("cs_mutex_init return error.(%d) FILE: %s, LINE: %d\n", rt, __FILE__, __LINE__);
    }
    
    rt = cs_mutex_init(&__ma_tx_mutex,  "MA TX mutex",0);
    if(rt!=0)
    {
        cs_halt("cs_mutex_init return error.(%d) FILE: %s, LINE: %d\n", rt, __FILE__, __LINE__);
    }
    // set mii0 drop type
    drop.u16 = 0;
    REG_FIELD_WRITE(MA_CPU_DST1_MII0, pkt_typ_drop, drop.u16);

    // set mii0 forward type
    fw.u16 = 0;
    fw.s.normal_pkt_fw  = AAL_MA_FW_CPU; // ASIC bug, this bit should be set always
    fw.s.bpdu_pkt_fw    = AAL_MA_FW_CPU;
    fw.s.dot1x_pkt_fw   = AAL_MA_FW_CPU;
    fw.s.arp_pkt_fw     = AAL_MA_FW_CPU;
    fw.s.oam_pkt_fw     = AAL_MA_FW_CPU;
    fw.s.mymac_pkt_fw   = AAL_MA_FW_CPU;
    fw.s.ipv4_pkt_fw    = AAL_MA_FW_CPU;
    fw.s.ipv6_pkt_fw    = AAL_MA_FW_CPU;
    fw.s.hello_pkt_fw   = AAL_MA_FW_CPU;
    REG_FIELD_WRITE(MA_CPU_DST_EN_MII0, pkt_type, fw.u16);

    // disable rx/tx flow control
    REG_FIELD_WRITE(MA_MII0_RX_CTRL, flow_ctrl, 0);
    REG_FIELD_WRITE(MA_MII0_TX_CFG, tx_flow_en, 0);

    // enable receiving packet from ES module
    // enable rx/tx byte swap
    REG_FIELD_WRITE(MA_CPU_CTRL, es, 1);
    REG_FIELD_WRITE(MA_CPU_CTRL, rx_byte_swap_en, 1);
    REG_FIELD_WRITE(MA_CPU_CTRL, tx_byte_swap_en, 1);

    return CS_E_OK;
}

