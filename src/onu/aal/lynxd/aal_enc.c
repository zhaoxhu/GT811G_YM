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
 
DEFINITIONS:  "DEVICE" means the Cortina Systems? Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems? SDK software.
 
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
 
Copyright (c) 2010 by Cortina Systems Incorporated
****************************************************************************/

#include "aal_enc.h"
#include "aal_util.h"
#include "aal.h"


#define __ENC_OUI_MAX                        (0xffffff)
#define __ENC_CTC_OAM_OUI                    (0x111111)
#define __ENC_NTT_OAM_OUI                    (0x0000d9)
#define __ENC_OAM_FLAG                       (0x50)

#define __ENC_TX_TRIGGER_MAX                 (0x7)
#define __ENC_TX_TRIGGER_DEFAULT             (0x3)

#define __ENC_RX_PATH_BYPASS                 (0x4)
#define __ENC_TX_PATH_BYPASS                 (0x4)

#define __ENC_OAM_RPT_LEN                    (128)

#define __ENC_DISABLE                        (0x0)
#define __ENC_ENABLE                         (0x1)

#define __ENC_TX_LASER_DLY                   (0x1e)


volatile EPON_t *g_epon_base = &g_lynxd_reg_ptr->epon;

void __enc_dbg_bus_init(void)
{
    /*
    s m 0x2c32035c 0x1b1b  debug bus seletion
    s m 0x2c30c028 0x0     debug control  
    d m 0x2c30c038         debug bus data, 28 bit is enc bit 
    */
 
    REG_WRITE(ONU_MAC_DBG_SEL, 0x1b1b); 
    REG_WRITE(GLOBAL_DBG_CTRL, 0x0);
}

cs_status aal_enc_cfg_set(cs_aal_enc_msk_t msk, cs_aal_enc_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("aal_enc_cfg_set():error param!\n");
        return CS_E_PARAM; 
    }
       
    if(msk.s.en)
        REG_FIELD_WRITE(ONU_MAC_AES_CHURNING_CTRL, enable_decryption_function_churning_or_AES, (cfg->en>0));            

    if(msk.s.mode){
        if(cfg->mode>AAL_ENC_ALG_AES_SPECIAL){
            AAL_MIN_LOG("aal_enc_cfg_set():error mode param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, AesModeSel, cfg->mode); 
    }
        
    if(msk.s.extra_oam_rpt_len)     
        REG_FIELD_WRITE(ONU_MAC_CHURNING_OAM_OUI, oam_rpt_len, cfg->extra_oam_rpt_len);            

    if(msk.s.oam_oui){
        if(cfg->oam_oui > __ENC_OUI_MAX){
            AAL_MIN_LOG("aal_enc_cfg_set():error oam_oui param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_CHURNING_OAM_OUI, Churn_oam_OUI, cfg->oam_oui);            
    }

    if(msk.s.oam_flag)
        REG_FIELD_WRITE(ONU_MAC_CHURNING_OAM_FLAG, churning_oam_flag, cfg->oam_flag);            
        
    if(msk.s.churn_mode){
        if(cfg->churn_mode>AAL_ENC_CHURN_MODE_SINGLE){
            AAL_MIN_LOG("aal_enc_cfg_set():error churn_mode param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_AES_CHURNING_CTRL, enable_single_de_churning_function, cfg->churn_mode);            
    }
    
    if(msk.s.key_gen_mode){
        if(cfg->key_gen_mode>AAL_ENC_KEY_GEN_MODE_SW){
            AAL_MIN_LOG("aal_enc_cfg_set():error key_gen_mode param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_AES_CHURNING_CTRL, HW_key_mode, cfg->key_gen_mode); 
    }

    if(msk.s.key_exchange_mode){
        if(cfg->key_exchange_mode>AAL_ENC_KEY_EXCHANGE_HW){
            AAL_MIN_LOG("aal_enc_cfg_set():error key_exchange_mode param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_CHURN_CONFIG_CONTROL, enable_Transmit_churning_OAM, cfg->key_exchange_mode);         
        REG_FIELD_WRITE(ONU_MAC_CHURN_CONFIG_CONTROL, rsv15, cfg->key_exchange_mode);            
    }

    return CS_E_OK;

}

cs_status aal_enc_cfg_get(cs_aal_enc_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("aal_enc_cfg_get():error  param!\n");
        return CS_E_PARAM; 
    }

    REG_FIELD_READ(ONU_MAC_AES_CHURNING_CTRL, enable_decryption_function_churning_or_AES, cfg->en);            
    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, AesModeSel, cfg->mode);                 
    REG_FIELD_READ(ONU_MAC_CHURNING_OAM_OUI, oam_rpt_len, cfg->extra_oam_rpt_len);            
    REG_FIELD_READ(ONU_MAC_CHURNING_OAM_OUI, Churn_oam_OUI, cfg->oam_oui);            
    REG_FIELD_READ(ONU_MAC_CHURNING_OAM_FLAG, churning_oam_flag, cfg->oam_flag);            
    REG_FIELD_READ(ONU_MAC_AES_CHURNING_CTRL, enable_single_de_churning_function, cfg->churn_mode);            
    REG_FIELD_READ(ONU_MAC_AES_CHURNING_CTRL, HW_key_mode, cfg->key_gen_mode);            
    REG_FIELD_READ(ONU_MAC_CHURN_CONFIG_CONTROL, enable_Transmit_churning_OAM, cfg->key_exchange_mode);
    
    return CS_E_OK;
}


cs_status aal_enc_tx_cfg_set( cs_aal_enc_tx_msk_t msk, cs_aal_enc_tx_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("aal_enc_tx_cfg_set():error param!\n");
        return CS_E_PARAM; 
    }

    if(msk.s.en) 
        REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, tx_port_decryptonly_en, ((cfg->en>0)?0:1));            
    
    if(msk.s.threshold){
        if(cfg->threshold > __ENC_TX_TRIGGER_MAX){
            AAL_MIN_LOG("aal_enc_tx_cfg_set():error threshold param!\n");
            return CS_E_PARAM;  
        }
        
        REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, rx_enc_en_cnt_lmt, cfg->threshold);            
    }

    if(msk.s.crc8_regen_en)
        REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, tx_crc8_gen_disable, ((cfg->crc8_regen_en>0)?0:1));            
    
    if(msk.s.crc8_in_reverse_en)   
        REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, tx_ReverseCrc8Out, (cfg->crc8_in_reverse_en>0));            
    
    if(msk.s.crc8_out_reverse_en)       
        REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, tx_ReverseCrc8En, (cfg->crc8_out_reverse_en>0));            

    if(msk.s.key_idx_sel_mode){ 
        if(cfg->key_idx_sel_mode>AAL_ENC_KEY_IDX_SEL_MANUAL){
            AAL_MIN_LOG("aal_enc_tx_cfg_set():error key_idx_sel_mode param!\n");
            return CS_E_PARAM; 
        }
        
        REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, port_encrypt_index_sel, cfg->key_idx_sel_mode);                     
    }
    
    if(msk.s.key_idx){ 
        if(cfg->key_idx > AAL_ENC_KEY_INDEX_1){
            AAL_MIN_LOG("aal_enc_tx_cfg_set():error key_idx param!\n");
            return CS_E_PARAM; 
        }
                
        REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, csr_port_encrypt_index, cfg->key_idx);
    }
    
    if(msk.s.key_idx_notify_en)        
        REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, tx_port_encrypt_en, (cfg->key_idx_notify_en>0));            
    
    if(msk.s.rcc_b0_sel_mode){    
        if(cfg->rcc_b0_sel_mode>AAL_ENC_RCC_SEL_LOCAL){
            AAL_MIN_LOG("aal_enc_tx_cfg_set():error rcc_b0_sel_mode param!\n");
            return CS_E_PARAM; 
        }
        
        REG_FIELD_WRITE(ONU_MAC_UC_TX_KEY_CTRL, tx_rcc_b0_sel, cfg->rcc_b0_sel_mode);
    }

    if(msk.s.key_sel_mode){    
        if(cfg->key_sel_mode>AAL_ENC_KEY_SEL_MANUAL){
            AAL_MIN_LOG("aal_enc_tx_cfg_set():error key_sel_mode param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_UC_TX_KEY_CTRL, tx_key_sel, cfg->key_sel_mode); 
    }

    return CS_E_OK;

}

cs_status aal_enc_tx_cfg_get(cs_aal_enc_tx_cfg_t *cfg)
{
    cs_uint32 value;

    if(NULL == cfg){
        AAL_MIN_LOG("aal_enc_tx_cfg_get():error param!\n");
        return CS_E_PARAM;
    }


    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, tx_port_decryptonly_en, value);
    cfg->en = !value;            
    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, rx_enc_en_cnt_lmt, cfg->threshold);            
    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, tx_crc8_gen_disable, value);
    cfg->crc8_regen_en = !value;            
    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, tx_ReverseCrc8Out, cfg->crc8_in_reverse_en );                 
    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, tx_ReverseCrc8En, cfg->crc8_out_reverse_en);                   
    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, port_encrypt_index_sel, cfg->key_idx_sel_mode);                          
    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, csr_port_encrypt_index, cfg->key_idx);                  
    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, tx_port_encrypt_en, cfg->key_idx_notify_en);               
    REG_FIELD_READ(ONU_MAC_UC_TX_KEY_CTRL, tx_rcc_b0_sel, cfg->rcc_b0_sel_mode);                   
    REG_FIELD_READ(ONU_MAC_UC_TX_KEY_CTRL, tx_key_sel, cfg->key_sel_mode);            
 
    return CS_E_OK;
}


cs_status aal_enc_timer_cfg_set(cs_aal_enc_timer_msk_t msk, cs_aal_enc_timer_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("aal_enc_timer_cfg_set():error param!\n");
        return CS_E_PARAM;
    }
       
    if(msk.s.rx_dir){     
        if(cfg->rx_dir>AAL_ENC_TIMER_ADJUST_DIR_ADD){
            AAL_MIN_LOG("aal_enc_timer_cfg_set():error rx_dir param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, CntIncDec, cfg->rx_dir);
    }
        
    if(msk.s.rx_delta)
        REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, CntDiff, cfg->rx_delta);            
        
    if(msk.s.tx_dir){     
        if(cfg->tx_dir>AAL_ENC_TIMER_ADJUST_DIR_ADD){ 
            AAL_MIN_LOG("aal_enc_timer_cfg_set():error tx_dir param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, tx_CntIncDec, cfg->tx_dir); 
    }

    if(msk.s.tx_delta)
        REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, tx_CntDiff, cfg->tx_delta);            
        
    if(msk.s.timer_hi16)
        REG_FIELD_WRITE(ONU_MAC_AES_TIMER2, AES_TIMER_47_32, cfg->timer_hi16);

    if(msk.s.jaes_timer)
        REG_FIELD_WRITE(ONU_MAC_JAES_TIMER, H32, cfg->jaes_timer);

    return CS_E_OK;

}


cs_status aal_enc_timer_cfg_get(cs_aal_enc_timer_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("aal_enc_timer_cfg_get():error param!\n");
        return CS_E_PARAM;
    }

    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, CntIncDec, cfg->rx_dir);            
    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, CntDiff, cfg->rx_delta);            
    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, tx_CntIncDec, cfg->tx_dir);            
    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, tx_CntDiff, cfg->tx_delta);            
    REG_FIELD_READ(ONU_MAC_AES_TIMER2, AES_TIMER_47_32, cfg->timer_hi16);            
    REG_FIELD_READ(ONU_MAC_JAES_TIMER, H32, cfg->jaes_timer);            

    return CS_E_OK;
}


cs_status aal_enc_key_set(cs_aal_enc_key_idx_t index, cs_aal_enc_key_t *key)
{    
    cs_uint32 *key_mem = NULL;
    cs_uint8 *onu_key = (cs_uint8 *)key;
    cs_uint8 i;

    if((NULL==key)||(index>AAL_ENC_KEY_INDEX_1)){
        AAL_MIN_LOG("aal_enc_key_set():error param!\n");
        return CS_E_PARAM;
    } 
    
    if(index) 
        key_mem = (cs_uint32 *)(&g_epon_base->UC_KEY10);              
    else
        key_mem = (cs_uint32 *)(&g_epon_base->UC_KEY00);

    for(i=0;i<4;i++)
        *(key_mem - i) = (onu_key[i*4+3] << 24) + (onu_key[i*4+2] << 16)+ (onu_key[i*4+1] << 8) + onu_key[i*4];
    
    return CS_E_OK;
}


cs_status aal_enc_key_get(cs_aal_enc_key_idx_t index, cs_aal_enc_key_t *key)
{    
    cs_uint32 *key_mem = NULL;
    cs_uint8 *onu_key = (cs_uint8 *)key;

    cs_uint8 i;
    
    if((NULL==key)||(index>AAL_ENC_KEY_INDEX_1)){
        AAL_MIN_LOG("aal_enc_key_get():error param!\n");
        return CS_E_PARAM;
    } 
    
    if(index) 
        key_mem = (cs_uint32 *)(&g_epon_base->UC_KEY10);              
    else
        key_mem = (cs_uint32 *)(&g_epon_base->UC_KEY00);
 
    for(i=0;i<16;i++){
        onu_key[i] = (*(key_mem - (i/4)) >> (i%4) * 8) & 0xff;
    }
    
    return CS_E_OK;
}

cs_status aal_enc_key_hw_get(cs_aal_enc_key_idx_t index, cs_aal_enc_key_t *key)
{
    cs_uint32 *key_mem = NULL;
    cs_uint8 *onu_key = (cs_uint8 *)key;
    cs_uint8 i;

    if(NULL == key){
        AAL_MIN_LOG("aal_enc_key_hw_get():error param!\n");
        return CS_E_PARAM;
    }
    
    if(index) 
        key_mem = (cs_uint32 *)(&g_epon_base->HW_GEN_KEY10);              
    else
        key_mem = (cs_uint32 *)(&g_epon_base->HW_GEN_KEY00);
 
    for(i=0;i<16;i++){
        onu_key[i] = (*(key_mem - (i/4)) >> (i%4) * 8) & 0xff;
    }
    
    return CS_E_OK;    
}

cs_status aal_enc_cnt_get(cs_aal_enc_cnt_t *cnt)
{
    if(NULL == cnt){
        AAL_MIN_LOG("aal_enc_cnt_get():error param!\n");
        return CS_E_PARAM;
    }
    
    REG_FIELD_READ(ONU_MAC_AES_TIMER, AES_TIMER_47_16, cnt->aes_timer); 
    REG_FIELD_READ(ONU_MAC_LOCAL_TIMER, local_timer, cnt->local_timer);            
    REG_FIELD_READ(ONU_MAC_KEYREQUEST_OAM_CNT, keyrequest_oam_cnt, cnt->request_cnt);            
    REG_FIELD_READ(ONU_MAC_UC_DEC_DBG_CNT, mpcp_cnt_dbg, cnt->local_cnt);   
    REG_FIELD_READ(ONU_MAC_AES_DBG_CNT0, cnt, cnt->crc_err_cnt_even_key);  
    REG_FIELD_READ(ONU_MAC_AES_DBG_GOOD_CNT0, cnt, cnt->crc_good_cnt_even_key);
    REG_FIELD_READ(ONU_MAC_AES_DBG_CNT1, cnt, cnt->crc_err_cnt_odd_key);
    REG_FIELD_READ(ONU_MAC_AES_DBG_GOOD_CNT1, cnt, cnt->crc_good_cnt_odd_key);
    REG_FIELD_READ(ONU_MAC_PKT_MON2, last_rx_key_index, cnt->rx_index);
    REG_FIELD_READ(ONU_MAC_AES_MODE_CTL, csr_port_encrypt_index, cnt->tx_index);
    REG_FIELD_READ(ONU_MAC_PKT_MON2, key_ind_tog_cnt, cnt->key_idx_tog_cnt);    
    REG_FIELD_READ(ONU_MAC_PKT_MON2, rx_aes_flag, cnt->rx_aes_flag);
    REG_FIELD_READ(ONU_MAC_PKT_MON2, RCC_b0, cnt->rcc_b0);

    return CS_E_OK;     
}

cs_status aal_enc_cnt_clr(void)
{
    REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, rx_enc_en_cnt_clr, 1);   
    REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, rx_enc_en_cnt_clr, 0);

    return CS_E_OK;
}
                                               
cs_status enc_init(void)
{ 
    cs_aal_enc_cfg_t cfg;
    cs_aal_enc_msk_t cfg_msk;
    cs_aal_enc_tx_cfg_t tx_cfg;
    cs_aal_enc_tx_msk_t tx_cfg_msk;
    cs_aal_enc_timer_cfg_t timer;
    cs_aal_enc_timer_msk_t timer_msk;
    cs_uint8 value;

    cfg.en = __ENC_DISABLE;
    cfg.mode = AAL_ENC_ALG_AES_SPECIAL;
    cfg.extra_oam_rpt_len = __ENC_OAM_RPT_LEN;  
    cfg.oam_oui = __ENC_NTT_OAM_OUI; 
    cfg.oam_flag = __ENC_OAM_FLAG;  
    cfg.churn_mode = AAL_ENC_CHURN_MODE_TRIPLE;
    cfg.key_gen_mode = AAL_ENC_KEY_GEN_MODE_SW;
    cfg.key_exchange_mode = AAL_ENC_KEY_EXCHANGE_SW;

    cfg_msk.u32 = 0;
    cfg_msk.s.en = 1;
    cfg_msk.s.mode = 1;
    cfg_msk.s.extra_oam_rpt_len = 1; 
    cfg_msk.s.oam_oui = 1; 
    cfg_msk.s.oam_flag = 1;
    cfg_msk.s.churn_mode = 1;
    cfg_msk.s.key_gen_mode = 1;
    cfg_msk.s.key_exchange_mode = 1;
    aal_enc_cfg_set(cfg_msk, &cfg);
                              
    tx_cfg.en = __ENC_DISABLE;
    tx_cfg.threshold = __ENC_TX_TRIGGER_DEFAULT; 
    tx_cfg.crc8_regen_en = __ENC_ENABLE;
    tx_cfg.crc8_in_reverse_en = __ENC_DISABLE;
    tx_cfg.crc8_out_reverse_en = __ENC_DISABLE;
    tx_cfg.key_idx_sel_mode = AAL_ENC_KEY_IDX_SEL_MANUAL;
    tx_cfg.key_idx = AAL_ENC_KEY_INDEX_0;
    tx_cfg.key_idx_notify_en = __ENC_DISABLE;
    tx_cfg.rcc_b0_sel_mode = AAL_ENC_RCC_SEL_LOCAL;
    tx_cfg.key_sel_mode = AAL_ENC_KEY_SEL_AUTO;

    tx_cfg_msk.u32 = 0;
    tx_cfg_msk.s.en = 1;
    tx_cfg_msk.s.threshold = 1; 
    tx_cfg_msk.s.crc8_regen_en = 1;
    tx_cfg_msk.s.crc8_in_reverse_en = 1;
    tx_cfg_msk.s.crc8_out_reverse_en = 1;
    tx_cfg_msk.s.key_idx_sel_mode = 1;
    tx_cfg_msk.s.key_idx = 1;
    tx_cfg_msk.s.key_idx_notify_en = 1;
    tx_cfg_msk.s.rcc_b0_sel_mode = 1;
    tx_cfg_msk.s.key_sel_mode = 1;
    aal_enc_tx_cfg_set(tx_cfg_msk, &tx_cfg);
    
    timer.rx_dir = AAL_ENC_TIMER_ADJUST_DIR_ADD;
    timer.rx_delta = AAL_ENC_TIMER_ADJUST_RX_DEFAULT;
    timer.tx_dir = AAL_ENC_TIMER_ADJUST_DIR_SUB;
    timer.tx_delta = AAL_ENC_TIMER_ADJUST_TX_DEFAULT;  
    timer.timer_hi16 = 0;   
    timer.jaes_timer = 0; 
    
    timer_msk.u32 = 0;
    timer_msk.s.rx_dir = 1;
    timer_msk.s.rx_delta = 1;
    timer_msk.s.tx_dir = 1;
    timer_msk.s.tx_delta = 1;
    timer_msk.s.timer_hi16 = 1;   
    timer_msk.s.jaes_timer = 1; 
    aal_enc_timer_cfg_set(timer_msk, &timer);

    REG_FIELD_READ(ONU_MAC_DATAPATH_BYPASS, rxpath_bypass, value);
    value &= ~__ENC_RX_PATH_BYPASS;
    REG_FIELD_WRITE(ONU_MAC_DATAPATH_BYPASS, rxpath_bypass, value);
    REG_FIELD_READ(ONU_MAC_DATAPATH_BYPASS, txpath_bypass, value);
    value &= ~__ENC_TX_PATH_BYPASS;
    REG_FIELD_WRITE(ONU_MAC_DATAPATH_BYPASS, txpath_bypass, value);

    __enc_dbg_bus_init();
        
    return CS_E_OK;
}

inline cs_uint32 aal_local_timer_get(void)
{
    cs_uint32 local_timer;
    
    REG_FIELD_READ(ONU_MAC_LOCAL_TIMER, local_timer, local_timer);            

    return local_timer;     
}

/*fast clear jaes counter, plase note that call this function may affect crypto funciton and registraton*/
inline void aal_enc_jaes_timer_clear(void)
{
    ONU_MAC_AES_CHURNING_CTRL_t aes_ctrl;
    
    REG_READ(ONU_MAC_AES_CHURNING_CTRL, aes_ctrl.wrd);
    aes_ctrl.bf.enable_decryption_function_churning_or_AES = 0;
    REG_WRITE(ONU_MAC_AES_CHURNING_CTRL, aes_ctrl.wrd);            

    REG_WRITE(ONU_MAC_JAES_TIMER, 0);  
    
    REG_READ(ONU_MAC_AES_CHURNING_CTRL, aes_ctrl.wrd);
    aes_ctrl.bf.enable_decryption_function_churning_or_AES = 1;
    REG_WRITE(ONU_MAC_AES_CHURNING_CTRL, aes_ctrl.wrd);  
}

/*if register and didn't receive the first encrypted packet*/
inline cs_boolean aal_enc_jaes_clear_cond(void)
{
    cs_uint32 reg = 0, enc = 0;

    REG_FIELD_READ(ONU_MAC_MPCP_STATUS, onureg, reg);
    REG_READ(GLOBAL_DBG_DATA0, enc);

    return ((!reg) || (enc & 0x10000000));     
}

