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


#include "aal_uni.h" 
#include "aal_util.h" 
#include "startup_cfg.h"
#include "aal.h"

#define __MIB_MAX                   AAL_MIB_TxFrmCount
#define __RGMII_GE                  0
#define __RGMII_FE                  1
#define __FE_100M                   0
#define __FE_10M                    1
#define __DUPLEX_FULL               0
#define __MIN_PKT_SIZE              64
#define __MAX_PKT_SIZE              1518
#define __MIN_IPG                   8
#define __FIFO_THRESHOLD_MODE       0

cs_uint32  __uni_mutex;


/******************************************************************************/

cs_status aal_uni_mac_cfg_set(cs_aal_uni_cfg_mask_t mask, cs_aal_uni_cfg_t cfg)
{
    if(mask.u32 == 0) {
        AAL_MIN_LOG("In %s, parameter wrong\n", __FUNCTION__);
        return CS_E_PARAM; 
    }
       
    if(mask.s.rx_en)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, rx_en, cfg.rx_en);            
    if(mask.s.tx_en)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, tx_en, cfg.tx_en);            
    if(mask.s.speed){
        if(cfg.speed < AAL_UNI_SPEED_1000M){
            REG_FIELD_WRITE(GE_INTF_RST_CONFIG, rgmii_speed, __RGMII_FE);  
            REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, speed, (cfg.speed == AAL_UNI_SPEED_100M) ? __FE_100M : __FE_10M);     
        }   
        else
            REG_FIELD_WRITE(GE_INTF_RST_CONFIG, rgmii_speed, __RGMII_GE);       
    }
    if(mask.s.duplex)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, duplex, cfg.duplex);            
    if(mask.s.rx_fc_dis)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, rx_flow_disable, cfg.rx_fc_dis);            
    if(mask.s.tx_fc_dis)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, tx_flow_disable, cfg.tx_fc_dis);            
    if(mask.s.max_size)
        REG_FIELD_WRITE(GE_PKT_LEN_CONFIG, max_size, (cfg.max_size > __MIN_PKT_SIZE) ? cfg.max_size : __MIN_PKT_SIZE);            
    if(mask.s.min_size)
        REG_FIELD_WRITE(GE_PKT_LEN_CONFIG, min_size, (cfg.min_size > __MIN_PKT_SIZE) ? cfg.min_size : __MIN_PKT_SIZE);            
    if(mask.s.mib_mode)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG1, mib_op_mode, cfg.mib_mode);            
    if(mask.s.mib_clr_all)
        REG_FIELD_WRITE(GE_INIT, mib, cfg.mib_clr_all);
    if(mask.s.tx_ipg_sel)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, tx_ipg_sel, (cfg.tx_ipg_sel < __MIN_IPG)? 0 : (cfg.tx_ipg_sel - __MIN_IPG)); /*set less than 8, select 8*/           

    return CS_E_OK; 
}

cs_status aal_uni_mac_cfg_get(cs_aal_uni_cfg_t* cfg)
{
    cs_boolean rgmii_speed = 0, speed = 0;
    if(cfg == NULL) {
        AAL_MIN_LOG("In %s, parameter wrong\n", __FUNCTION__);
        return CS_E_PARAM; 
    }
       
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, rx_en, cfg->rx_en);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, tx_en, cfg->tx_en);   
    REG_FIELD_READ(GE_INTF_RST_CONFIG, rgmii_speed, rgmii_speed);   
    if (rgmii_speed == __RGMII_GE) {
        cfg->speed = AAL_UNI_SPEED_1000M;
    } else {
        REG_FIELD_READ(GE_ETH_MAC_CONFIG0, speed, speed);
        if (speed) {
            cfg->speed = AAL_UNI_SPEED_10M;
        } else {
            cfg->speed = AAL_UNI_SPEED_100M;
        }
    }
        
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, duplex, cfg->duplex);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, rx_flow_disable, cfg->rx_fc_dis);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, tx_flow_disable, cfg->tx_fc_dis);            
    REG_FIELD_READ(GE_PKT_LEN_CONFIG, max_size, cfg->max_size);            
    REG_FIELD_READ(GE_PKT_LEN_CONFIG, min_size, cfg->min_size);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG1, mib_op_mode, cfg->mib_mode);            
    REG_FIELD_READ(GE_INIT, mib, cfg->mib_clr_all);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, tx_ipg_sel, cfg->tx_ipg_sel);            

    return CS_E_OK; 
}

cs_status aal_uni_mac_cfg_ext_set(cs_aal_uni_cfg_ext_mask_t mask, cs_aal_uni_cfg_ext_t cfg)
{

    if(mask.u32 == 0) {
        AAL_MIN_LOG("In %s, parameter wrong\n", __FUNCTION__);
        return CS_E_PARAM; 
    }
       
    if(mask.s.phy_mode)
        REG_FIELD_WRITE(GE_INTF_RST_CONFIG, phy_mode, cfg.phy_mode);            
    if(mask.s.link_stat_mask)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, link_stat_mask, cfg.link_stat_mask);            
    if(mask.s.link_active_mask)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, link_active_mask, cfg.link_active_mask);            
    if(mask.s.tx_drain)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, tx_drain, cfg.tx_drain);            
    if(mask.s.txfifo_mode)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, txfifo_mode, cfg.txfifo_mode);            
    if(mask.s.txfifo_thrshld)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, txfifo_thrshld, cfg.txfifo_thrshld);            
    if(mask.s.tx_auto_xon)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, tx_auto_xon, cfg.tx_auto_xon);            
    if(mask.s.phy_intr_inv)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, phy_intr_inv, cfg.phy_intr_inv);            
    if(mask.s.bypass_header)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, pass_thru_hdre, cfg.bypass_header);            
    if(mask.s.tx_update_crc)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, tx_update_crc, cfg.tx_update_crc);            
    if(mask.s.pause_uc_only)
        REG_FIELD_WRITE(GE_RX_MAC_CONFIG0, rx_pause_uc, cfg.pause_uc_only);            

    if(mask.s.tag_exclude)
        REG_FIELD_WRITE(GE_RX_MAC_CONFIG0, tag_exclude, cfg.tag_exclude);            
    if(mask.s.fifo_thrshld_low)
        REG_FIELD_WRITE(GE_TX_CNTRL_CONFIG0, rdy_low_thld, cfg.fifo_thrshld_low);            
    if(mask.s.fifo_thrshld_high)
        REG_FIELD_WRITE(GE_TX_CNTRL_CONFIG0, rdy_high_thld, cfg.fifo_thrshld_high);            

    if(mask.s.user_tpid0)
        REG_FIELD_WRITE(GE_RX_MAC_CONFIG0, tag_type0, cfg.user_tpid0);            
    if(mask.s.user_tpid1)
        REG_FIELD_WRITE(GE_RX_MAC_CONFIG1, tag_type1, cfg.user_tpid1);            
    if(mask.s.user_tpid2)
        REG_FIELD_WRITE(GE_RX_MAC_CONFIG1, tag_type2, cfg.user_tpid2);            

    return CS_E_OK; 

}

cs_status aal_uni_mac_cfg_ext_get(cs_aal_uni_cfg_ext_t* cfg)
{

    if(cfg == NULL) {
        AAL_MIN_LOG("In %s, parameter wrong\n", __FUNCTION__);
        return CS_E_PARAM; 
    }
      
    REG_FIELD_READ(GE_INTF_RST_CONFIG, phy_mode, cfg->phy_mode);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, link_stat_mask, cfg->link_stat_mask);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, link_active_mask, cfg->link_active_mask);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, tx_drain, cfg->tx_drain);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, txfifo_mode, cfg->txfifo_mode);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, txfifo_thrshld, cfg->txfifo_thrshld);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, tx_auto_xon, cfg->tx_auto_xon);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, phy_intr_inv, cfg->phy_intr_inv);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, pass_thru_hdre, cfg->bypass_header);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, tx_update_crc, cfg->tx_update_crc);            
    REG_FIELD_READ(GE_RX_MAC_CONFIG0, rx_pause_uc, cfg->pause_uc_only);            
    REG_FIELD_READ(GE_RX_MAC_CONFIG0, tag_exclude, cfg->tag_exclude);            
    REG_FIELD_READ(GE_TX_CNTRL_CONFIG0, rdy_low_thld, cfg->fifo_thrshld_low);            
    REG_FIELD_READ(GE_TX_CNTRL_CONFIG0, rdy_high_thld, cfg->fifo_thrshld_high);            
    REG_FIELD_READ(GE_RX_MAC_CONFIG0, tag_type0, cfg->user_tpid0);            
    REG_FIELD_READ(GE_RX_MAC_CONFIG1, tag_type1, cfg->user_tpid1);            
    REG_FIELD_READ(GE_RX_MAC_CONFIG1, tag_type2, cfg->user_tpid2);            

    return CS_E_OK; 

}

cs_status aal_uni_mac_dbg_set(cs_aal_uni_dbg_mask_t mask, cs_aal_uni_dbg_t cfg)
{
    if(mask.u32 == 0) {
        AAL_MIN_LOG("In %s, parameter wrong\n", __FUNCTION__);
        return CS_E_PARAM; 
    }
       
    if(mask.s.rxctrl_dis)
        REG_FIELD_WRITE(GE_RX_CNTRL_CONFIG0, rxctrl_dis, cfg.rxctrl_dis);            
    if(mask.s.overrun_drop_dis)
        REG_FIELD_WRITE(GE_RX_CNTRL_CONFIG0, ff_overrun_drop_dis, cfg.overrun_drop_dis);            
    if(mask.s.crc_drop_dis)
        REG_FIELD_WRITE(GE_RX_CNTRL_CONFIG0, crc_drop_dis, cfg.crc_drop_dis);            
    if(mask.s.ovr_drop_dis)
        REG_FIELD_WRITE(GE_RX_CNTRL_CONFIG0, ovr_drop_dis, cfg.ovr_drop_dis);            
    if(mask.s.udr_drop_dis)
        REG_FIELD_WRITE(GE_RX_CNTRL_CONFIG0, udr_drop_dis, cfg.udr_drop_dis);            
    if(mask.s.pause_drop_dis)
        REG_FIELD_WRITE(GE_RX_CNTRL_CONFIG0, flow_drop_dis, cfg.pause_drop_dis);            
    if(mask.s.ukopcode_drop_dis)
        REG_FIELD_WRITE(GE_RX_CNTRL_CONFIG0, ukopcode_drop_dis, cfg.ukopcode_drop_dis);
    if(mask.s.oam_drop_dis)
        REG_FIELD_WRITE(GE_RX_CNTRL_CONFIG0, oam_drop_dis, cfg.oam_drop_dis);
    if(mask.s.bypass_enable)
        REG_FIELD_WRITE(GE_RX_CNTRL_CONFIG0, bypass_enable, cfg.bypass_enable);            
    if(mask.s.bypass_dpid)
        REG_FIELD_WRITE(GE_RX_CNTRL_CONFIG0, bypass_dpid, cfg.bypass_dpid);            
    if(mask.s.bypass_cos)
        REG_FIELD_WRITE(GE_RX_CNTRL_CONFIG0, bypass_cos, cfg.bypass_cos);            
    if(mask.s.hd_backoff_dbg)
        REG_FIELD_WRITE(GE_DEBUG_1, hd_backoff_dbg, cfg.hd_backoff_dbg);            
    if(mask.s.hd_backoff_cnt)
        REG_FIELD_WRITE(GE_DEBUG_1, hd_backoff_cnt, cfg.hd_backoff_cnt);            
 
    return CS_E_OK; 

}

cs_status aal_uni_mac_dbg_get(cs_aal_uni_dbg_t* cfg)
{
    if(cfg == NULL) {
        AAL_MIN_LOG("In %s, parameter wrong\n", __FUNCTION__);
        return CS_E_PARAM; 
    }
       
    REG_FIELD_READ(GE_RX_CNTRL_CONFIG0, rxctrl_dis, cfg->rxctrl_dis);            
    REG_FIELD_READ(GE_RX_CNTRL_CONFIG0, ff_overrun_drop_dis, cfg->overrun_drop_dis);            
    REG_FIELD_READ(GE_RX_CNTRL_CONFIG0, crc_drop_dis, cfg->crc_drop_dis);            
    REG_FIELD_READ(GE_RX_CNTRL_CONFIG0, ovr_drop_dis, cfg->ovr_drop_dis);            
    REG_FIELD_READ(GE_RX_CNTRL_CONFIG0, udr_drop_dis, cfg->udr_drop_dis);            
    REG_FIELD_READ(GE_RX_CNTRL_CONFIG0, flow_drop_dis, cfg->pause_drop_dis);            
    REG_FIELD_READ(GE_RX_CNTRL_CONFIG0, ukopcode_drop_dis, cfg->ukopcode_drop_dis);            
    REG_FIELD_READ(GE_RX_CNTRL_CONFIG0, bypass_enable, cfg->bypass_enable);            
    REG_FIELD_READ(GE_RX_CNTRL_CONFIG0, bypass_dpid, cfg->bypass_dpid);            
    REG_FIELD_READ(GE_RX_CNTRL_CONFIG0, bypass_cos, cfg->bypass_cos);            
    REG_FIELD_READ(GE_DEBUG_1, hd_backoff_dbg, cfg->hd_backoff_dbg);            
    REG_FIELD_READ(GE_DEBUG_1, hd_backoff_cnt, cfg->hd_backoff_cnt);            
 
    return CS_E_OK; 

}

cs_status aal_uni_mac_status_get(cs_aal_uni_status_t* cfg)
{
    if(cfg == NULL) {
        AAL_MIN_LOG("In %s, parameter wrong\n", __FUNCTION__);
        return CS_E_PARAM; 
    }
       
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, crs_polarity, cfg->crs_polarity);            
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, link_status, cfg->link_status);
    REG_FIELD_READ(GE_ETH_RCV_STAT, rcv_speed, cfg->rcv_speed);            
    REG_FIELD_READ(GE_ETH_RCV_STAT, rcv_duplex, cfg->rcv_duplex);            
    REG_FIELD_READ(GE_ETH_RCV_STAT, rcv_lnk_stat, cfg->rcv_lnk_stat);            
 
    return CS_E_OK; 

}

#if 0

cs_status aal_uni_int_mask(cs_uint32 en)
{
    cs_uint32 src;
    REG_READ(ONU_MAC_INTERRUPT_ENABLE, src);
    REG_WRITE(ONU_MAC_INTERRUPT_ENABLE, ( (~en) & src));

    return CS_E_OK; 

}


cs_status aal_uni_int_unmask(cs_uint32 en)
{

    cs_uint32 src;
    REG_READ(ONU_MAC_INTERRUPT_ENABLE, src);
    REG_WRITE(ONU_MAC_INTERRUPT_ENABLE, (en | src));


    return CS_E_OK; 

}
#else
cs_status aal_uni_int_disable(cs_aal_uni_int_msk_t mask)
{
    cs_uint32 src;
    REG_READ(GE_INTENABLE, src);
    REG_WRITE(GE_INTENABLE, ((~mask.u32) & src));

    return CS_E_OK; 

}


cs_status aal_uni_int_enable(cs_aal_uni_int_msk_t mask)
{

    cs_uint32 src;
    REG_READ(GE_INTENABLE, src);
    REG_WRITE(GE_INTENABLE, (mask.u32 | src));

    return CS_E_OK; 

}

cs_status aal_uni_int_en_get(cs_aal_uni_int_msk_t *mask)
{

    cs_uint32 int_en;
    if(NULL == mask){
        AAL_MIN_LOG("In %s, parameter wrong\n", __FUNCTION__);
        return CS_E_ERROR;
    }
    REG_READ(GE_INTENABLE, int_en);
    mask->u32 = int_en;
    return CS_E_OK; 
}
#endif

cs_status aal_uni_int_status_get(cs_aal_uni_int_msk_t* mask)
{
    if(NULL == mask){
        AAL_MIN_LOG("In %s, parameter wrong\n", __FUNCTION__);
        return CS_E_ERROR;
    }

    REG_READ(GE_INTERRUPT, mask->u32);

    return CS_E_OK; 
}

cs_status aal_uni_int_status_clr(cs_aal_uni_int_msk_t mask)
{

    REG_WRITE(GE_INTERRUPT, mask.u32); /*write 1 to clear*/

    return CS_E_OK; 
}

/*
 The correct procedure to turn on internal loopback should be
    1.	Disable TX MAC
    2.	Set internal loopback
    3.	Enable TX MAC
 */
cs_status aal_uni_mac_loopback_set(cs_aal_uni_lpbk_mode_t mode)
{
    if((mode.tx2rx_enable > 1) || (mode.rx2tx_enable > 1)){
        AAL_MIN_LOG("In %s, parameter wrong\n", __FUNCTION__);
        return CS_E_PARAM;
    }
//    REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, tx_en, 0);            
    
    REG_FIELD_WRITE(GLOBAL_CTRL, ge_lcl_lpbk, mode.tx2rx_enable);            
    REG_FIELD_WRITE(GLOBAL_CTRL, ge_rmte_lpbk, mode.rx2tx_enable);            

//    REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, tx_en, 1);            
    
    return CS_E_OK; 
}
    
cs_status aal_uni_mac_loopback_get(cs_aal_uni_lpbk_mode_t *mode)
{
    if(!mode){
        AAL_MIN_LOG("In %s, parameter wrong\n", __FUNCTION__);
        return CS_E_PARAM;
    }
    
    REG_FIELD_READ(GLOBAL_CTRL, ge_lcl_lpbk, mode->tx2rx_enable);            
    REG_FIELD_READ(GLOBAL_CTRL, ge_rmte_lpbk, mode->rx2tx_enable);            
    
    return CS_E_OK; 
}
 
_CS_INLINE_ cs_status __mib_table_access(void)
{
    cs_uint16 i = 0, access;
    
    while(i++ < 1000) {
        REG_FIELD_READ(GE_MIB_ACCESS, access, access);            
        if (0 == access) 
            return CS_E_OK;
    }

    AAL_MAJ_LOG("GE_MIB_ACCESS time out!!!\n");

    return CS_E_ERROR;
}

/*rc: 1 -- read clear, 0 -- read keep;                                                       */
/*hi: high 32 bit data;                                                                      */
/*lo: low 32 bit data;                                                                       */
cs_status aal_uni_mib_get(cs_aal_uni_mib_t  mib_id, cs_boolean  rc, cs_uint32 *hi, cs_uint32 *lo)
{
    GE_MIB_ACCESS_t  access;
    cs_status rt = CS_E_OK;


    if (!hi || !lo || mib_id > __MIB_MAX) 
        return CS_E_PARAM;

    cs_mutex_lock(__uni_mutex);
    access.bf.counter = mib_id;
    access.bf.rbw     = 0;             
    access.bf.op_code = (rc ? 2 : 0); 
    access.bf.access  = 1;

    if (CS_OK != __mib_table_access()) {
        rt = CS_E_TIMEOUT;
        goto mib_get_end;
    }
    
    REG_WRITE(GE_MIB_ACCESS, access.wrd);  

    if (CS_OK != __mib_table_access()) {
        rt = CS_E_TIMEOUT;
        goto mib_get_end;
    }

    REG_READ(GE_MIB_DATA0, *lo); 
    REG_READ(GE_MIB_DATA1, *hi); 

mib_get_end:   
    cs_mutex_unlock(__uni_mutex);
    return rt;
    
}


/* GE MAC EEE (802.3az) function set */
cs_status aal_uni_eee_set
(
    cs_aal_uni_eee_mask_t  mask,
    cs_aal_uni_eee_t       eee
)
{
    if(mask.u32 == 0) {
        AAL_MIN_LOG("In %s, parameter wrong\n", __FUNCTION__);
        return CS_E_PARAM; 
    }

    if (mask.s.enable) {
        REG_FIELD_WRITE(GE_EEE_CONFIG, enable, eee.enable);            
    }
    
    if (mask.s.tck_off) {
        REG_FIELD_WRITE(GE_EEE_CONFIG, tck_off, eee.tck_off);
    }
    
    if (mask.s.tck_extra) {
        REG_FIELD_WRITE(GE_EEE_CONFIG, tck_extra, eee.tck_extra);
    }
    
    return CS_E_OK;
    
}

cs_status aal_uni_eee_get(cs_aal_uni_eee_t  *eee)
{
    if(NULL == eee) {
        AAL_MIN_LOG("In %s, parameter wrong\n", __FUNCTION__);
        return CS_E_PARAM; 
    }

    REG_FIELD_READ(GE_EEE_CONFIG, enable, eee->enable);            
    REG_FIELD_READ(GE_EEE_CONFIG, tck_off, eee->tck_off);
    REG_FIELD_READ(GE_EEE_CONFIG, tck_extra, eee->tck_extra);
    
    return CS_E_OK;
}

cs_status uni_init(void)
{
    cs_aal_uni_cfg_t uni_mac;
    cs_aal_uni_cfg_mask_t uni_mac_mask;
    cs_aal_uni_cfg_ext_t uni_mac_ext;
    cs_aal_uni_cfg_ext_mask_t uni_mac_ext_mask;
    cs_uint8  tmp_val = 0;
    cs_uint8  phy_eee = 0; // EEE enable or disable
    cs_aal_uni_eee_mask_t  eee_mask;
    cs_aal_uni_eee_t       eee_cfg;
    cs_status ret = CS_E_OK;
    cs_aal_uni_dbg_mask_t dbg_mask;
    cs_aal_uni_dbg_t dbg_cfg;
    
    cs_mutex_init(&__uni_mutex, "UNI MIB mutex", 0);

    ret = startup_config_read(
        CFG_ID_PHY_EEE_MODE, 
        1, &phy_eee);

    if (ret == CS_E_NOT_FOUND){
        AAL_MIN_LOG("!!!Notification: PHY EEE startup config is not exist, default set EEE to disable!\n");
        phy_eee = 0;
    } else if (ret != CS_E_OK) {
        cs_halt("Read CFG_ID_PHY_EEE_MODE from startup-cfg FAILED\n");
    }

    eee_mask.s.tck_off = 1;
    eee_mask.s.enable  = 1;
    eee_cfg.enable     = phy_eee ? 1 : 0;
    eee_cfg.tck_off    = 0;
    
    aal_uni_eee_set(eee_mask, eee_cfg);  
    
    uni_mac_mask.u32 = 0;
    uni_mac_ext_mask.u32 = 0;

    uni_mac.mib_clr_all = TRUE; 
    uni_mac.max_size = __MAX_PKT_SIZE;
    uni_mac.min_size = __MIN_PKT_SIZE;
    uni_mac.rx_en = FALSE;
    uni_mac.tx_en = FALSE;
    uni_mac.rx_fc_dis = TRUE;
    uni_mac.tx_fc_dis = TRUE;

    uni_mac_mask.s.mib_clr_all = TRUE; 
    uni_mac_mask.s.max_size = TRUE;
    uni_mac_mask.s.min_size = TRUE;
    uni_mac_mask.s.rx_en = TRUE;
    uni_mac_mask.s.tx_en = TRUE;
    uni_mac_mask.s.rx_fc_dis = TRUE;
    uni_mac_mask.s.tx_fc_dis = TRUE;

    /* default to 1000M */
    aal_uni_mac_cfg_set(uni_mac_mask, uni_mac);

    uni_mac_ext.txfifo_mode = __FIFO_THRESHOLD_MODE;
    uni_mac_ext.tx_drain = FALSE;

    uni_mac_ext_mask.s.txfifo_mode = TRUE;
    uni_mac_ext_mask.s.tx_drain = TRUE;

    aal_uni_mac_cfg_ext_set(uni_mac_ext_mask, uni_mac_ext);

    /* UNI clock sel */
    /* In ASIC version, set GE_CLK_SEL = 0 and 
     * set PHY GTXCLK delay mode.
     * BCM5461 support SW setting. But RTL8211 can only be set by HW */
    if (startup_config_read(CFG_ID_GE_CLK_SEL, 1, &tmp_val)) {
        cs_halt("Read GE_CLK_SEL from startup-cfg FAILED\n");
    }
    
    /* Since startup cfg will impact boot-loader too, 
     * hard-code here. Enhance it after HW confirm */
    //tmp_val = 1;
    REG_FIELD_WRITE(GLOBAL_CTRL, ge_clk_sel, tmp_val ? 1 : 0); 

    /* Bug29046: oam_drp_dis = 1 for OOB management 
       If need to disable OAM, APP code should call SDL API */
    dbg_mask.u32 = 0;
    dbg_mask.s.oam_drop_dis = 1;
    dbg_cfg.oam_drop_dis = 1;
     
    aal_uni_mac_dbg_set(dbg_mask, dbg_cfg);
    
    return CS_E_OK;
    
}




