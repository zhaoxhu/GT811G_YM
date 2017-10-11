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

#include "aal_fec.h"
#include "aal_util.h"
#include "aal_enc.h"
#include "aal.h"



#define __FEC_TX_IPG_ADJUST_MAX                       (0x7)
#define __FEC_TX_IPG_ADJUST_DEFAULT                   (0x4)

#define __FEC_ERR_INJECT_BYTE_CNT_MAX                 (0x1f)
#define __FEC_ERR_INJECT_BIT_CNT_MAX                  (0x3)

#define __FEC_TX_OVERHEAD                             (0xf)

#define __FEC_ERR_THRESHOLD_DEFAULT                   (0x3)
#define __FEC_ERR_THRESHOLD_MAX                       (0xf)

#define __FEC_RX_PATH_BYPASS                          (0x2)
#define __FEC_TX_PATH_BYPASS                          (0x2)

#define __FEC_MAX_PKT_SIZE_COMP                       (0x608)
#define __FEC_MAX_PKT_SIZE_NORMAL                     (0x7f8)

#define __FEC_MAX_LATENCY_COMP                        (0x60b)
#define __FEC_MAX_LATECNY_NORMAL                      (0x7fb)

#define __FEC_DISABLE                                 (0x0)
#define __FEC_ENABLE                                  (0x1)

#define __FEC_COMP_ENC_TIMER_ADJUST_RX                (0x1c)
#define __FEC_COMP_ENC_TIMER_ADJUST_TX                (0x15)

#define __FEC_PKT_SIZE_MIN                            (0xfff)
#define __FEC_PKT_SIZE_MAX                            (0xfff)

#define __FEC_LATENCY_FOR_DECODE                      (0x1ff)
#define __FEC_LATENCY_FOR_MAX_PKT                     (0xfff)


/*Start of debug functions */
cs_status aal_fec_ext_cfg_set(cs_aal_fec_ext_msk_t msk, cs_aal_fec_ext_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("aal_fec_ext_cfg_set():error param!\n");
        return CS_E_PARAM;
    }
        
    if(msk.s.rx_path_clr)
        REG_FIELD_WRITE(ONU_MAC_FEC_CTRL, fec_rx_clr, (cfg->rx_path_clr>0));            

    if(msk.s.tx_path_clr)
        REG_FIELD_WRITE(ONU_MAC_FEC_CTRL, fec_tx_clr, (cfg->tx_path_clr>0));            

    if(msk.s.correct_cnt_clr)
        REG_FIELD_WRITE(ONU_MAC_FEC_CTRL, cor_ctr_clr, (cfg->correct_cnt_clr>0));            
    
    if(msk.s.uncorrect_cnt_clr)    
        REG_FIELD_WRITE(ONU_MAC_FEC_CTRL, unc_ctr_clr, (cfg->uncorrect_cnt_clr>0));            
        
    if(msk.s.stop_correct_cnt)
        REG_FIELD_WRITE(ONU_MAC_FEC_CTRL, halt_cor, (cfg->stop_correct_cnt>0));            

    if(msk.s.stop_uncorrect_cnt)       
        REG_FIELD_WRITE(ONU_MAC_FEC_CTRL, halt_unc, (cfg->stop_uncorrect_cnt>0));            
        
    if(msk.s.rx_protection_window)
        REG_FIELD_WRITE(ONU_MAC_FEC_CFG, rx_window_on, (cfg->rx_protection_window>0));        
      
    if(msk.s.cnt_rd_clr_mode){
        if(cfg->cnt_rd_clr_mode>AAL_FEC_CNT_READ_CLEAR_MSB){
            AAL_MIN_LOG("aal_fec_ext_cfg_set():error cnt_rd_clr_mode param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_FEC_Counter_Control, FEC_CNTR_rd_clr_mode, cfg->cnt_rd_clr_mode);                    
    }
           
    if(msk.s.rx_status_clr)
        REG_FIELD_WRITE(ONU_MAC_FEC_CTRL, frx_sta_clr, (cfg->rx_status_clr>0));            
        
    if(msk.s.err_threshold){
        if(cfg->err_threshold > __FEC_ERR_THRESHOLD_MAX){
            AAL_MIN_LOG("aal_fec_ext_cfg_set():error err_threshold param!\n");
            return CS_E_PARAM;  
        }
        REG_FIELD_WRITE(ONU_MAC_FEC_CFG, thrshld, cfg->err_threshold);               
    }

    if(msk.s.encode_mode){
        if(cfg->encode_mode>AAL_FEC_ENCODE_CORTINA){
            AAL_MIN_LOG("aal_fec_ext_cfg_set():error encode_mode param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_FEC_CFG, tx_fec8B, cfg->encode_mode);
    }

    if(msk.s.tx_ipg_adjust){
        if(cfg->tx_ipg_adjust>__FEC_TX_IPG_ADJUST_MAX){
            AAL_MIN_LOG("aal_fec_ext_cfg_set():error tx_ipg_adjust param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_FEC_CFG, tx_ipg_adj, cfg->tx_ipg_adjust);        
    }
    
    if(msk.s.err_send_mode){
        if(cfg->err_send_mode>AAL_FEC_ERR_SEND_CODE){
            AAL_MIN_LOG("aal_fec_ext_cfg_set():error err_send_mode param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_FEC_CFG, in_valid_cg, cfg->err_send_mode);
    }

    return CS_E_OK;
}

cs_status aal_fec_ext_cfg_get(cs_aal_fec_ext_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("aal_fec_ext_cfg_get():error param!\n");
        return CS_E_PARAM;
    } 
    
    REG_FIELD_READ(ONU_MAC_FEC_CTRL, fec_rx_clr, cfg->rx_path_clr);            
    REG_FIELD_READ(ONU_MAC_FEC_CTRL, fec_tx_clr, cfg->tx_path_clr);  
    REG_FIELD_READ(ONU_MAC_FEC_CTRL, cor_ctr_clr, cfg->correct_cnt_clr);            
    REG_FIELD_READ(ONU_MAC_FEC_CTRL, unc_ctr_clr, cfg->uncorrect_cnt_clr );                    
    REG_FIELD_READ(ONU_MAC_FEC_CTRL, halt_cor, cfg->stop_correct_cnt);            
    REG_FIELD_READ(ONU_MAC_FEC_CTRL, halt_unc, cfg->stop_uncorrect_cnt);      
    REG_FIELD_READ(ONU_MAC_FEC_CFG, rx_window_on, cfg->rx_protection_window);        
    REG_FIELD_READ(ONU_MAC_FEC_Counter_Control, FEC_CNTR_rd_clr_mode, cfg->cnt_rd_clr_mode);                    
    REG_FIELD_READ(ONU_MAC_FEC_CTRL, frx_sta_clr, cfg->rx_status_clr);              
    REG_FIELD_READ(ONU_MAC_FEC_CFG, thrshld, cfg->err_threshold);             
    REG_FIELD_READ(ONU_MAC_FEC_CFG, tx_fec8B, cfg->encode_mode);                 
    REG_FIELD_READ(ONU_MAC_FEC_CFG, tx_ipg_adj, cfg->tx_ipg_adjust);        
    REG_FIELD_READ(ONU_MAC_FEC_CFG, in_valid_cg, cfg->err_send_mode);

    return CS_E_OK;
}

cs_status aal_fec_err_inj_cfg_set(cs_aal_fec_err_inj_msk_t msk, cs_aal_fec_err_inj_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("aal_fec_err_inj_cfg_set():error param!\n");
        return CS_E_PARAM;
    }
    
    if(msk.s.en)
        REG_FIELD_WRITE(ONU_MAC_FEC_TX_ERRINJ, err_en, (cfg->en>0));

    if(msk.s.burst_en){        
        REG_FIELD_WRITE(ONU_MAC_FEC_TX_ERRINJ, err_burst, 0);
        
        if(cfg->burst_en)
            REG_FIELD_WRITE(ONU_MAC_FEC_TX_ERRINJ, err_burst, 1);
    }

    if(msk.s.packet_cnt)        
        REG_FIELD_WRITE(ONU_MAC_FEC_TX_ERRINJ, err_pktcnt, cfg->packet_cnt);

    if(msk.s.byte_cnt){
        if(cfg->byte_cnt > __FEC_ERR_INJECT_BYTE_CNT_MAX){
            AAL_MIN_LOG("aal_fec_err_inj_cfg_set():error byte_cnt param!\n");
            return CS_E_PARAM; 
        }
        REG_FIELD_WRITE(ONU_MAC_FEC_TX_ERRINJ, err_bytecnt, cfg->byte_cnt);
    }
    
    if(msk.s.bit_cnt){
        if(cfg->bit_cnt > __FEC_ERR_INJECT_BIT_CNT_MAX){
            AAL_MIN_LOG("aal_fec_err_inj_cfg_set():error bit_cnt param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_FEC_TX_ERRINJ, err_bitcnt, cfg->bit_cnt);
    }
    
    if(msk.s.tfec1_en)
        REG_FIELD_WRITE(ONU_MAC_FEC_TX_ERRINJ, err_tfec1, (cfg->tfec1_en>0));

    if(msk.s.tfec2_en)
        REG_FIELD_WRITE(ONU_MAC_FEC_TX_ERRINJ, err_tfec2, (cfg->tfec2_en>0));

    if(msk.s.data_en)
        REG_FIELD_WRITE(ONU_MAC_FEC_TX_ERRINJ, err_data, (cfg->data_en>0));
    
    if(msk.s.sfec_en)
        REG_FIELD_WRITE(ONU_MAC_FEC_TX_ERRINJ, err_sfec, (cfg->sfec_en>0));
            
    if(msk.s.parity_en)
        REG_FIELD_WRITE(ONU_MAC_FEC_TX_ERRINJ, err_prty, (cfg->parity_en>0));
    
    return CS_E_OK;
}

cs_status aal_fec_err_inj_cfg_get(cs_aal_fec_err_inj_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("aal_fec_err_inj_cfg_get():error param!\n");
        return CS_E_PARAM;
    }

    REG_FIELD_READ(ONU_MAC_FEC_TX_ERRINJ, err_en, cfg->en);
    REG_FIELD_READ(ONU_MAC_FEC_TX_ERRINJ, err_burst, cfg->burst_en);
    REG_FIELD_READ(ONU_MAC_FEC_TX_ERRINJ, err_pktcnt, cfg->packet_cnt);
    REG_FIELD_READ(ONU_MAC_FEC_TX_ERRINJ, err_bytecnt, cfg->byte_cnt);        
    REG_FIELD_READ(ONU_MAC_FEC_TX_ERRINJ, err_bitcnt, cfg->bit_cnt);
    REG_FIELD_READ(ONU_MAC_FEC_TX_ERRINJ, err_tfec1, cfg->tfec1_en );
    REG_FIELD_READ(ONU_MAC_FEC_TX_ERRINJ, err_tfec2, cfg->tfec2_en);
    REG_FIELD_READ(ONU_MAC_FEC_TX_ERRINJ, err_data, cfg->data_en);
    REG_FIELD_READ(ONU_MAC_FEC_TX_ERRINJ, err_sfec, cfg->sfec_en);
    REG_FIELD_READ(ONU_MAC_FEC_TX_ERRINJ, err_prty, cfg->parity_en);

    return CS_E_OK;
}

cs_status aal_fec_pktsize_cfg_set(cs_aal_fec_pktsize_msk_t msk, cs_aal_fec_pktsize_cfg_t *cfg
)
{
    if(NULL == cfg){
        AAL_MIN_LOG("aal_fec_pktsize_cfg_set():error param!\n");
        return CS_E_PARAM;
    }
    
    if(msk.s.min){
        if(cfg->min > __FEC_PKT_SIZE_MIN){
            AAL_MIN_LOG("aal_fec_pktsize_cfg_set():error min param!\n");
            return CS_E_PARAM;
        }

        REG_FIELD_WRITE(ONU_MAC_FEC_DEFINE_PKTSIZE, min_pkt_size, cfg->min);       
    }

    if(msk.s.max){
        if(cfg->max > __FEC_PKT_SIZE_MAX){ 
            AAL_MIN_LOG("aal_fec_pktsize_cfg_set():error max param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_FEC_DEFINE_PKTSIZE, max_pkt_size, cfg->max);       
    }        
    
    return CS_E_OK;

}

cs_status aal_fec_pktsize_cfg_get(cs_aal_fec_pktsize_cfg_t *cfg)
{    
    if(NULL == cfg){
        AAL_MIN_LOG("aal_fec_pktsize_cfg_get():error param!\n");
        return CS_E_PARAM;
    }
    
    REG_FIELD_READ(ONU_MAC_FEC_DEFINE_PKTSIZE, min_pkt_size, cfg->min);       
    REG_FIELD_READ(ONU_MAC_FEC_DEFINE_PKTSIZE, max_pkt_size, cfg->max);       

    return CS_E_OK;
    
}

cs_status aal_fec_latency_cfg_set(cs_aal_fec_latency_msk_t msk, cs_aal_fec_latency_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("aal_fec_latency_cfg_set():error param!\n");
        return CS_E_PARAM;
    }
    
    if(msk.s.decode){
        if(cfg->decode > __FEC_LATENCY_FOR_DECODE){
            AAL_MIN_LOG("aal_fec_latency_cfg_set():error decode param!\n");
            return CS_E_PARAM;
        }

        REG_FIELD_WRITE(ONU_MAC_FEC_DEFINE_LATENCY, fdec_latency, cfg->decode);             
    }

    if(msk.s.max_pkt){
        if(cfg->max_pkt > __FEC_LATENCY_FOR_MAX_PKT){
            AAL_MIN_LOG("aal_fec_latency_cfg_set():error max_pkt param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_FEC_DEFINE_LATENCY, pckt_latency, cfg->max_pkt);       
    }
    
    return CS_E_OK;

}

cs_status aal_fec_latency_cfg_get(cs_aal_fec_latency_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("aal_fec_latency_cfg_get():error param!\n");
        return CS_E_PARAM;
    }
    
    REG_FIELD_READ(ONU_MAC_FEC_DEFINE_LATENCY, fdec_latency, cfg->decode);       
    REG_FIELD_READ(ONU_MAC_FEC_DEFINE_LATENCY, pckt_latency, cfg->max_pkt);       
    
    return CS_E_OK;

}

cs_status aal_fec_comp_cfg_set(cs_aal_fec_comp_msk_t msk, cs_aal_fec_comp_cfg_t *cfg)
{
    cs_uint8 aes_rx_path;
    cs_uint8 churn_rx_path;
    cs_uint8 aes_non_rx_path;
    cs_uint8 churn_non_rx_path;
    cs_uint32 comp2;

    if(NULL == cfg){
        AAL_MIN_LOG("aal_fec_comp_cfg_set():error param!\n");
        return CS_E_PARAM;
    }

    REG_READ(ONU_MAC_MPCP_RTT_COMPENSATION2, comp2);       
    aes_rx_path = (comp2 >> 24) & 0xff;
    churn_rx_path = (comp2 >> 16) & 0xff;
    aes_non_rx_path = (comp2 >> 8) & 0xff;
    churn_non_rx_path = comp2 & 0xff;

    if(msk.s.en)        
        REG_FIELD_WRITE(ONU_MAC_AES_FEC_COMP, fec_comp_enable,(cfg->en>0));       
    
    if(msk.s.mode){
        if(cfg->mode > AAL_FEC_COMP_MODE_3){
            AAL_MIN_LOG("aal_fec_comp_cfg_set():error mode param!\n");
            return CS_E_PARAM;
        }
        
        if(cfg->mode == AAL_FEC_COMP_MODE_2){
            /*MPCP counter adjust for comp*/
            REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, CntIncDec, AAL_ENC_TIMER_ADJUST_DIR_SUB);            
            REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, CntDiff, __FEC_COMP_ENC_TIMER_ADJUST_RX);            
            REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, tx_CntIncDec, AAL_ENC_TIMER_ADJUST_DIR_ADD);            
            REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, tx_CntDiff, __FEC_COMP_ENC_TIMER_ADJUST_TX);
            
            /*max pkt size for comp*/
            REG_FIELD_WRITE(ONU_MAC_FEC_DEFINE_PKTSIZE, max_pkt_size, __FEC_MAX_PKT_SIZE_COMP);
            
            /*max pkt latency for comp*/
            REG_FIELD_WRITE(ONU_MAC_FEC_DEFINE_LATENCY, pckt_latency, __FEC_MAX_LATENCY_COMP);       
        }
        else{
            /*MPCP counter adjust normal*/
            REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, CntIncDec, AAL_ENC_TIMER_ADJUST_DIR_ADD);            
            REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, CntDiff, AAL_ENC_TIMER_ADJUST_RX_DEFAULT);            
            REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, tx_CntIncDec, AAL_ENC_TIMER_ADJUST_DIR_SUB);            
            REG_FIELD_WRITE(ONU_MAC_AES_MODE_CTL, tx_CntDiff, AAL_ENC_TIMER_ADJUST_TX_DEFAULT);

            /*max pkt size for normal*/
            REG_FIELD_WRITE(ONU_MAC_FEC_DEFINE_PKTSIZE, max_pkt_size, __FEC_MAX_PKT_SIZE_NORMAL);
            
            /*max pkt latency for normal*/
            REG_FIELD_WRITE(ONU_MAC_FEC_DEFINE_LATENCY, pckt_latency, __FEC_MAX_LATECNY_NORMAL);       
        }
            
        REG_FIELD_WRITE(ONU_MAC_MPCP_RTT_COMPENSATION_MODE, mode, cfg->mode);       
    }

    if(msk.s.direction){
        if(cfg->direction > AAL_FEC_COMP_ADD){
            AAL_MIN_LOG("aal_fec_comp_cfg_set():error direction param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_AES_FEC_COMP, fec_comp_incdec,cfg->direction);  
    }
    
    if(msk.s.counter)
        REG_FIELD_WRITE(ONU_MAC_AES_FEC_COMP, fec_comp_cnt, cfg->counter);       
    
    if(msk.s.egress)
        REG_FIELD_WRITE(ONU_MAC_MPCP_RTT_COMPENSATION, output_compensation, cfg->egress);       

    if(msk.s.ingress)
        REG_FIELD_WRITE(ONU_MAC_MPCP_RTT_COMPENSATION, input_compensation, cfg->ingress);       

    if(msk.s.timestamp)
        REG_FIELD_WRITE(ONU_MAC_MPCP_RTT_COMPENSATION, ts_compensation, cfg->timestamp);       

    if(msk.s.aes_rx_path)
        aes_rx_path = cfg->aes_rx_path;

    if(msk.s.churn_rx_path)
        churn_rx_path = cfg->churn_rx_path;

    if(msk.s.aes_non_rx_path)
        aes_non_rx_path = cfg->aes_non_rx_path;

    if(msk.s.churn_non_rx_path){
        churn_non_rx_path = cfg->churn_non_rx_path;
    }
    comp2 = (aes_rx_path << 24) + (churn_rx_path << 16) 
                + (aes_non_rx_path << 8) + churn_non_rx_path;
    
    REG_WRITE(ONU_MAC_MPCP_RTT_COMPENSATION2, comp2);       

    return CS_E_OK;
}

cs_status aal_fec_comp_cfg_get(cs_aal_fec_comp_cfg_t *cfg)
{
    cs_uint32   comp2;

    if(NULL == cfg){
        AAL_MIN_LOG("aal_fec_comp_cfg_get():error param!\n");
        return CS_E_PARAM;
    }

    REG_READ(ONU_MAC_MPCP_RTT_COMPENSATION2, comp2);       
    REG_FIELD_READ(ONU_MAC_AES_FEC_COMP, fec_comp_enable,cfg->en);       
    REG_FIELD_READ(ONU_MAC_MPCP_RTT_COMPENSATION_MODE, mode, cfg->mode);       
    REG_FIELD_READ(ONU_MAC_AES_FEC_COMP, fec_comp_incdec,cfg->direction);       
    REG_FIELD_READ(ONU_MAC_AES_FEC_COMP, fec_comp_cnt, cfg->counter); 
    REG_FIELD_READ(ONU_MAC_MPCP_RTT_COMPENSATION, output_compensation, cfg->egress);       
    REG_FIELD_READ(ONU_MAC_MPCP_RTT_COMPENSATION, input_compensation, cfg->ingress);       
    REG_FIELD_READ(ONU_MAC_MPCP_RTT_COMPENSATION, ts_compensation, cfg->timestamp); 
    cfg->aes_rx_path = (comp2 >> 24) & 0xff;
    cfg->churn_rx_path = (comp2 >> 16) & 0xff;
    cfg->aes_non_rx_path = (comp2 >> 8) & 0xff;
    cfg->churn_non_rx_path = comp2 & 0xff;

    return CS_E_OK;
}

cs_status aal_fec_overhead_set(cs_uint8 overhead)
{    
    REG_FIELD_WRITE(ONU_MAC_MPCP_Configuration, mpcp_frame_fec_overhead, overhead); 
    
    return CS_E_OK;
}

cs_status aal_fec_overhead_get(cs_uint8 *overhead)
{
    REG_FIELD_READ(ONU_MAC_MPCP_Configuration, mpcp_frame_fec_overhead, *overhead); 

    return CS_E_OK;

}

cs_status aal_fec_status_get(cs_aal_fec_status_t *status)
{      
    if(NULL == status){
        AAL_MIN_LOG("aal_fec_status_get():error param!\n");
        return CS_E_PARAM;
    } 
    
    REG_FIELD_READ(ONU_MAC_FEC_MISC_STATUS, fec_dacor_ack_q, status->rx_data_corr_ack); 
    REG_FIELD_READ(ONU_MAC_FEC_MISC_STATUS, prty_full, status->parity_buffer_full); 
    REG_FIELD_READ(ONU_MAC_FEC_MISC_STATUS, cmd_full, status->cmd_buffer_full); 
    REG_FIELD_READ(ONU_MAC_FEC_MISC_STATUS, cwb_full, status->code_buffer_full); 
    REG_FIELD_READ(ONU_MAC_FEC_MISC_STATUS, pack_full, status->pkt_buffer_full); 
    REG_FIELD_READ(ONU_MAC_FEC_MISC_STATUS, pipe_stall, status->pipe_stuck);
    REG_FIELD_READ(ONU_MAC_FEC_MISC_STATUS, los_tfec1, status->los_tfec1_boundary); 
    REG_FIELD_READ(ONU_MAC_FEC_MISC_STATUS, los_tfec2, status->los_tfec2_boundary);  
    REG_FIELD_READ(ONU_MAC_FEC_MISC_STATUS, fec_errors, status->errors); 
    REG_FIELD_READ(ONU_MAC_FEC_MISC_STATUS, fec_uncorr, status->uncorrected); 
    REG_FIELD_READ(ONU_MAC_FEC_MISC_STATUS, fec_pkt_st, status->pkt_state);        

    return CS_E_OK;
}

/*end of debug functions*/

cs_status aal_fec_cfg_set(cs_aal_fec_msk_t msk,cs_aal_fec_cfg_t *cfg)
{
    cs_uint8 fec_rx_bypass;

    if(NULL == cfg){
        AAL_MIN_LOG("aal_fec_cfg_set():error param!\n");
        return CS_E_PARAM; 
    }
        
    if(msk.s.std_mode){
        if(cfg->std_mode>AAL_FEC_STD_2005){
            AAL_MIN_LOG("aal_fec_cfg_set():error std_mode param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_FEC_CFG, tfec_rep,  cfg->std_mode);
    }
    
    if(msk.s.rx_rep_mode){
        if(cfg->rx_rep_mode>AAL_FEC_REP_ENABLE){
            AAL_MIN_LOG("aal_fec_cfg_set():error rx_rep_mode param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_FEC_CFG, rx_rep_mode, cfg->rx_rep_mode);
    }
        
    if(msk.s.rx_preamble_mode){
        if(cfg->rx_preamble_mode>AAL_FEC_PREAMBLE_EXCLUDE){
            AAL_MIN_LOG("aal_fec_cfg_set():error rx_preamble_mode param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_FEC_CFG, rx_1st_preamble_mode, cfg->rx_preamble_mode);
    }
    
    if(msk.s.rx_bypass){        
        REG_FIELD_WRITE(ONU_MAC_GLOBAL_GLB_SYS_CFG, pon_fec_sync_en, ((cfg->rx_bypass>0)?0:1));           
        if(cfg->rx_bypass){
            REG_FIELD_READ(ONU_MAC_DATAPATH_BYPASS, rxpath_bypass, fec_rx_bypass);
            fec_rx_bypass |= __FEC_RX_PATH_BYPASS;
            REG_FIELD_WRITE(ONU_MAC_DATAPATH_BYPASS, rxpath_bypass, fec_rx_bypass);            
        }
        else{
            REG_FIELD_READ(ONU_MAC_DATAPATH_BYPASS, rxpath_bypass, fec_rx_bypass);
            fec_rx_bypass &= ~__FEC_RX_PATH_BYPASS;
            REG_FIELD_WRITE(ONU_MAC_DATAPATH_BYPASS, rxpath_bypass, fec_rx_bypass);
        }
    }
    
    if(msk.s.rx_en){ 
        REG_FIELD_WRITE(ONU_MAC_FEC_CTRL, fec_decode_enable, (cfg->rx_en>0));
        REG_FIELD_WRITE(ONU_MAC_GLOBAL_GLB_SYS_CFG, pon_fec_sync_en, (cfg->rx_en>0));
    }

    if(msk.s.tx_rep_mode){
        if(cfg->tx_rep_mode>AAL_FEC_REP_ENABLE){
            AAL_MIN_LOG("aal_fec_cfg_set():error tx_rep_mode param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_FEC_CFG, tx_rep_mode, cfg->tx_rep_mode);
    }

    if(msk.s.tx_preamble_mode){
        if(cfg->tx_preamble_mode>AAL_FEC_PREAMBLE_EXCLUDE){
            AAL_MIN_LOG("aal_fec_cfg_set():error tx_preamble_mode param!\n");
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_FEC_CFG, tx_1st_preamble_mode, cfg->tx_preamble_mode);
    }
   
    if(msk.s.tx_en)
        REG_FIELD_WRITE(ONU_MAC_FEC_CTRL, fec_encode_enable, (cfg->tx_en > 0)); 

    return CS_E_OK;
}

cs_status aal_fec_cfg_get(cs_aal_fec_cfg_t *cfg)
{
    cs_uint8 fec_rx_bypass;
    
    if(NULL == cfg){
        AAL_MIN_LOG("aal_fec_cfg_get():error param!\n");
        return CS_E_PARAM;
    }   

    REG_FIELD_READ(ONU_MAC_FEC_CFG, tfec_rep,  cfg->std_mode);
    
    REG_FIELD_READ(ONU_MAC_FEC_CFG, rx_rep_mode, cfg->rx_rep_mode);
    REG_FIELD_READ(ONU_MAC_FEC_CFG, rx_1st_preamble_mode, cfg->rx_preamble_mode);
    REG_FIELD_READ(ONU_MAC_DATAPATH_BYPASS, rxpath_bypass, fec_rx_bypass);
    cfg->rx_bypass  = fec_rx_bypass & __FEC_RX_PATH_BYPASS;
    REG_FIELD_READ(ONU_MAC_FEC_CTRL, fec_decode_enable, cfg->rx_en);
    REG_FIELD_READ(ONU_MAC_FEC_CFG, tx_rep_mode, cfg->tx_rep_mode);
    REG_FIELD_READ(ONU_MAC_FEC_CFG, tx_1st_preamble_mode, cfg->tx_preamble_mode);
    REG_FIELD_READ(ONU_MAC_FEC_CTRL, fec_encode_enable, cfg->tx_en);
    
    return CS_E_OK;
}

cs_status aal_fec_cnt_get(cs_aal_fec_cnt_t *cnt)
{  
    if(NULL == cnt){
        AAL_MIN_LOG("aal_fec_cnt_get():error param!\n");
        return CS_E_PARAM;
    } 
            
    REG_FIELD_READ(ONU_MAC_FEC_Block_Counter, FEC_BLK_CNT, cnt->rx_blks);
    REG_FIELD_READ(ONU_MAC_FEC_Corr_Byte_Counter, FEC_CORR_BYTE_CNT, cnt->rx_correct_bytes);
    REG_FIELD_READ(ONU_MAC_FEC_Block_Correctable_Error_Counter, FEC_RCNT, cnt->rx_correct_blks);
    REG_FIELD_READ(ONU_MAC_FEC_Block_Uncorrectable_Error_Counter,FEC_UCNT, cnt->rx_uncorrect_blks); 
    REG_FIELD_READ(ONU_MAC_FEC_Block_No_Error_Counter, FEC_NOERR, cnt->rx_good_blks);
    
    REG_FIELD_READ(ONU_MAC_SFEC_CNT, sfecCnt, cnt->rx_sfec);
    REG_FIELD_READ(ONU_MAC_TFEC_CNT, tfecCnt, cnt->rx_tfec);    
    REG_FIELD_READ(ONU_MAC_SPKT_CNT, spktCnt, cnt->rx_spkt);  
    REG_FIELD_READ(ONU_MAC_EPKT_CNT, epktCnt, cnt->rx_epkt);
    
    return CS_E_OK;
}



cs_status fec_init(void)
{
    cs_aal_fec_cfg_t cfg; 
    cs_aal_fec_msk_t cfg_msk;
    cs_aal_fec_ext_cfg_t ext_cfg; 
    cs_aal_fec_ext_msk_t ext_msk;
    cs_aal_fec_comp_cfg_t comp_cfg; 
    cs_aal_fec_comp_msk_t comp_msk; 

    cfg.std_mode = AAL_FEC_STD_2005;  
    cfg.rx_rep_mode = AAL_FEC_REP_ENABLE;             
    cfg.rx_preamble_mode = AAL_FEC_PREAMBLE_EXCLUDE;
    cfg.rx_bypass = __FEC_DISABLE;
    cfg.rx_en = __FEC_DISABLE;    
    cfg.tx_rep_mode = AAL_FEC_REP_ENABLE;                    
    cfg.tx_preamble_mode = AAL_FEC_PREAMBLE_EXCLUDE;    
    cfg.tx_en = __FEC_DISABLE;

    cfg_msk.u32 = 0;
    cfg_msk.s.std_mode = 1;  
    cfg_msk.s.rx_rep_mode = 1;             
    cfg_msk.s.rx_preamble_mode = 1;
    cfg_msk.s.rx_bypass = 1;
    cfg_msk.s.rx_en = 1;    
    cfg_msk.s.tx_rep_mode = 1;                    
    cfg_msk.s.tx_preamble_mode = 1;    
    cfg_msk.s.tx_en = 1;
    aal_fec_cfg_set(cfg_msk, &cfg);

    ext_cfg.rx_path_clr = __FEC_DISABLE;    
    ext_cfg.tx_path_clr = __FEC_DISABLE;
    ext_cfg.correct_cnt_clr = __FEC_DISABLE;
    ext_cfg.uncorrect_cnt_clr = __FEC_DISABLE;
    ext_cfg.stop_correct_cnt= __FEC_DISABLE;
    ext_cfg.stop_uncorrect_cnt = __FEC_DISABLE;
    ext_cfg.cnt_rd_clr_mode = AAL_FEC_CNT_READ_KEEP;   
    ext_cfg.rx_status_clr = __FEC_DISABLE;    
    ext_cfg.rx_protection_window = __FEC_ENABLE;
    ext_cfg.err_threshold = __FEC_ERR_THRESHOLD_DEFAULT;
    ext_cfg.encode_mode = AAL_FEC_ENCODE_CORTINA;
    ext_cfg.err_send_mode = AAL_FEC_ERR_SEND_DATA;
    ext_cfg.tx_ipg_adjust = __FEC_TX_IPG_ADJUST_DEFAULT; 

    ext_msk.u32 = 0;
    ext_msk.s.rx_path_clr = 1;    
    ext_msk.s.tx_path_clr = 1;
    ext_msk.s.correct_cnt_clr = 1;
    ext_msk.s.uncorrect_cnt_clr = 1;
    ext_msk.s.stop_correct_cnt= 1;
    ext_msk.s.stop_uncorrect_cnt = 1;
    ext_msk.s.cnt_rd_clr_mode = 1;   
    ext_msk.s.rx_status_clr = 1;    
    ext_msk.s.rx_protection_window = 1;
    ext_msk.s.err_threshold = 1;
    ext_msk.s.encode_mode = 1;
    ext_msk.s.err_send_mode = 1;
    ext_msk.s.tx_ipg_adjust = 1;    
    aal_fec_ext_cfg_set(ext_msk,&ext_cfg);
    
    comp_msk.u32 = 0;
    comp_msk.s.mode = 1;
    comp_cfg.mode = AAL_FEC_COMP_MODE_0;
    aal_fec_comp_cfg_set(comp_msk, &comp_cfg);

    aal_fec_overhead_set(__FEC_TX_OVERHEAD);

    return CS_E_OK;
}


