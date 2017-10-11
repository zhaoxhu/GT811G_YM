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
#include "aal_sys.h"
#include "aal_util.h"

#include "aal.h"
#include "aal_mpcp.h"
#include "aal_pon.h"
#include "aal_l2.h"
#include "aal_ma.h"
#include "aal_uni.h"
#include "aal_pon.h"
#include "aal_ma.h"
#include "aal_ptp.h"
#include "aal_bm.h"
#include "aal_serdes.h"
#ifdef HAVE_VOIP
#include "aal_pcm.h"
#endif

/* external function */
cs_status mpcp_init(void);
cs_status pon_init(void);
cs_status ma_init(void);
cs_status phy_init(void);
cs_status uni_init(void);
cs_status bm_init(void);
cs_status cls_init(void);
cs_status enc_init(void);
cs_status fec_init(void);
cs_status l2_init(void);
cs_status flow_init(void);


/* global variable */

cs_uint32  aal_reg_mutex;

/*1. Disable Tx/Rx on PON, UNI and MA.    */
/*2. Disable all interrupt.               */
/*3. Reset aal module after step 1 and 2. */
static cs_status __service_down(void)
{
    cs_status ret = CS_E_OK;

    cs_aal_pon_mac_msk_t        pon_mac_mask;
    cs_aal_pon_mac_cfg_t        pon_mac_cfg;
    cs_aal_ma_mii_msk_t         ma_mask;
    cs_aal_ma_mii_cfg_t         ma_cfg;
    cs_aal_ma_int_msk_t         ma_int_mask;
    cs_aal_pon_int_msk_t        pon_int_mask;
    cs_aal_uni_int_msk_t        uni_int_mask;
    cs_aal_bm_int_msk_t         bm_int_mask;
    cs_aal_ptp_int_msk_t        ptp_int_mask;
    cs_aal_fe_int_msk_t         fe_int_mask;
    cs_aal_module_reset_mask_t  reset_mask;
    cs_aal_uni_cfg_mask_t       uni_mac_mask;
    cs_aal_uni_cfg_t            uni_mac;

    // make aal module in reset state */
    reset_mask.u32 = 0;
    reset_mask.s.reset_te = 1;
    reset_mask.s.reset_mii0 = 1;
    reset_mask.s.reset_ma = 1;
    reset_mask.s.reset_fe = 1;
    reset_mask.s.reset_bm = 1;
    reset_mask.s.reset_ge = 1;
    reset_mask.s.reset_pon = 1;
    aal_sys_reset_ctrl_set(reset_mask,0);

#ifdef HAVE_VOIP
    onu_aal_pcm_disable();
#endif

    /*1. Disable Tx/Rx on PON, UNI and MA.    */
    pon_mac_cfg.rx_en = 0;
    pon_mac_cfg.tx_en = 0;
    pon_mac_mask.u32 = 0;
    pon_mac_mask.s.rx_en = 1;
    pon_mac_mask.s.tx_en = 1;
    aal_pon_mac_cfg_set(pon_mac_mask, &pon_mac_cfg);

    uni_mac.rx_en = 0;
    uni_mac.tx_en = 0;
    uni_mac_mask.u32 = 0;
    uni_mac_mask.s.rx_en = 1;
    uni_mac_mask.s.tx_en = 1;
    aal_uni_mac_cfg_set(uni_mac_mask, uni_mac);

    ma_cfg.rx_en = 0;
    ma_cfg.tx_en = 0;
    ma_mask.u32 = 0;
    ma_mask.s.rx_en = 1;
    ma_mask.s.tx_en = 1;
    aal_ma_mii_cfg_set(ma_mask, &ma_cfg);

    /*2. Disable and clear all interrupt */
    ma_int_mask.u32 = 0xFFFFFFFF;
    aal_ma_int_disable(ma_int_mask);
    aal_ma_int_clr(ma_int_mask);

    pon_int_mask.u32 = 0xFFFFFFFF;
    aal_pon_int_disable(pon_int_mask);
    aal_pon_int_clr(pon_int_mask);

    uni_int_mask.u32 = 0xFFFFFFFF;
    aal_uni_int_disable(uni_int_mask);
    aal_uni_int_status_clr(uni_int_mask);

    bm_int_mask.u32 = 0xFFFFFFFF;
    aal_bm_int_disable(bm_int_mask);
    aal_bm_int_status_clr(bm_int_mask);

    ptp_int_mask.u32 = 0xFFFFFFFF;
    aal_ptp_int_disable(ptp_int_mask);
    aal_ptp_int_status_clr(ptp_int_mask);

    fe_int_mask.u32[0] = 0xFFFFFFFF;
    fe_int_mask.u32[1] = 0xFFFFFFFF;
    aal_fe_int_disable(fe_int_mask);
    aal_fe_int_status_clr(fe_int_mask);

    // Release aal module out of reset state */
    reset_mask.u32 = 0xFFFFFFFF;
    aal_sys_reset_ctrl_set(reset_mask,1);

    return ret;
}

cs_status aal_init(void)
{    
    cs_mutex_init(&aal_reg_mutex, "aal reg mutex",0);    
    /* Phy init should be handled in SDL since multi-ports device doesn't need it */
    serdes_init();
    __service_down();
    bm_init();
    flow_init();
    mpcp_init();
    pon_init();
    ma_init();
    uni_init();
    cls_init();
    enc_init();
    fec_init();
    l2_init();
    
    return CS_E_OK;
}

cs_status aal_sys_reset_ctrl_set(cs_aal_module_reset_mask_t mask, cs_boolean en)
{
    if(mask.s.reset_pcm){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, pcm_rst_n, (en!=0));
    }
    if(mask.s.reset_arm){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, arm_rst_n, (en!=0));
    }
    if(mask.s.reset_arm_peri){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, arm_peri_rst_n, (en!=0));
    }
    if(mask.s.reset_sdram){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, sdm_rst_n, (en!=0));
    }
    if(mask.s.reset_pll){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, cpll_rst_n, (en!=0));
    }
    if(mask.s.reset_te){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, te_rst_n, (en!=0));
    }
    if(mask.s.reset_mii0){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, mii0_rst_n, (en!=0));
    }
    if(mask.s.reset_ma){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, ma_rst_n, (en!=0));
    }
    if(mask.s.reset_fe){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, fe_rst_n, (en!=0));
    }
    if(mask.s.reset_bm){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, bm_rst_n, (en!=0));
    }
    if(mask.s.reset_ge_tx){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, ge_tx_rst_n, (en!=0));
    }
    if(mask.s.reset_ge_rx){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, ge_rx_rst_n, (en!=0));
    }
    if(mask.s.reset_ge){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, ge_rst_n, (en!=0));
    }
    if(mask.s.reset_serdes){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, sds_rst_n, (en!=0));
    }
    if(mask.s.reset_pon_tx){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, pon_tx_rst_n, (en!=0));
    }
    if(mask.s.reset_pon_rx){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, pon_rx_rst_n, (en!=0));
    }
    if(mask.s.reset_pon){
        REG_FIELD_WRITE(GLOBAL_RST_CTRL, pon_rst_n, (en!=0));
    }
    return CS_E_OK;
}

cs_status aal_sys_pin_ctrl_set(cs_aal_pin_ctrl_cfg_mask_t mask, cs_aal_pin_ctrl_cfg_t cfg)
{
    if(mask.s.override_en){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, override_mode, cfg.override_en);
    }
    if(mask.s.share_pin_sel4){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, glb_share_pin_gp4, cfg.share_pin_sel4);
    }
    if(mask.s.share_pin_sel3){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, glb_share_pin_gp3, cfg.share_pin_sel3);
    }
    if(mask.s.share_pin_sel2){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, glb_share_pin_gp2, cfg.share_pin_sel2);
    }
    if(mask.s.share_pin_sel1){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, glb_share_pin_gp1, cfg.share_pin_sel1);
    }
    if(mask.s.ptp_1pps_sel){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, glb_ptp_1pps_seln, cfg.ptp_1pps_sel);
    }
    if(mask.s.sdm_f_share){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, cp_f_sd_en, cfg.sdm_f_share);
    }
    if(mask.s.mii0_if_mode){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, mii0_if_mode, cfg.mii0_if_mode);
    }
    if(mask.s.int_clk_sel){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, int_clk_sel, cfg.int_clk_sel);
    }
    if(mask.s.xcpu_sel){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, xcpu_sel, cfg.xcpu_sel);
    }
    if(mask.s.mem_addr_sel){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, mem_addr_sel, cfg.mem_addr_sel);
    }
    if(mask.s.image_download_mode){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, image_download_mode, cfg.image_download_mode);
    }
    if(mask.s.ge_if_mode){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, ge_if_mode, cfg.ge_if_mode);
    }
    if(mask.s.pon_if_mode){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, pon_if_mode, cfg.pon_if_mode);
    }
    if(mask.s.boot_mode){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, boot_mode, cfg.boot_mode);
    }
    
    return CS_E_OK;
}

cs_status aal_sys_pin_status_get(cs_aal_pin_status_t *status)
{
    REG_FIELD_READ(GLOBAL_PIN_STS, ext_clk_sel, status->ext_clk_sel);    
    REG_FIELD_READ(GLOBAL_PIN_STS, sdm_sram_share, status->sdm_f_share);    
    REG_FIELD_READ(GLOBAL_PIN_STS, mii0_if_mode, status->mii0_if_mode);    
    REG_FIELD_READ(GLOBAL_PIN_STS, int_clk_sel, status->int_clk_sel);    
    REG_FIELD_READ(GLOBAL_PIN_STS, xcpu_sel, status->xcpu_sel);    
    REG_FIELD_READ(GLOBAL_PIN_STS, mem_addr_sel, status->mem_addr_sel);    
    REG_FIELD_READ(GLOBAL_PIN_STS, image_download_mode, status->image_download_mode);    
    REG_FIELD_READ(GLOBAL_PIN_STS, ge_if_mode, status->ge_if_mode);    
    REG_FIELD_READ(GLOBAL_PIN_STS, pon_if_mode, status->pon_if_mode);    
    REG_FIELD_READ(GLOBAL_PIN_STS, boot_mode, status->boot_mode);    
    
    return CS_E_OK;
}

