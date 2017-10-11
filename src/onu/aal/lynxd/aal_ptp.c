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

#include "aal_ptp.h"
#include "aal_util.h"
#include "aal.h"

#define __PTP_COS_MAX                          (0x7)
#define __PTP_DPID_MAX                         (0x7)
#define __PTP_IPG_MAX                          (0x7)
#define __PTP_TX_FIFO_THRSHLD                  (0x1ff)
#define __PTP_CORRECT_DELTA_MAX                (0x7fff)
#define __TOD_CHAR_NUM_MAX                     (0x3f)


cs_status aal_ptp_glb_cfg_set(cs_aal_ptp_glb_msk_t msk, cs_aal_ptp_glb_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(msk.u32 == 0){
        return CS_E_OK;
    }

    if(msk.s.ref_timer)
        REG_FIELD_WRITE(GLOBAL_PTP_LOCAL_TIMER_REF, val, cfg->ref_timer); 
    
    if(msk.s.set_timestamp)
        REG_FIELD_WRITE(GLOBAL_PTP_SET_TIMESTAMP, val, cfg->set_timestamp); 

    return CS_E_OK;

}

cs_status aal_ptp_glb_cfg_get(cs_aal_ptp_glb_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(GLOBAL_PTP_LOCAL_TIMER_REF, val, cfg->ref_timer); 
    REG_FIELD_READ(GLOBAL_PTP_SET_TIMESTAMP, val, cfg->set_timestamp); 

    return CS_E_OK;

}

cs_status aal_ptp_glb_status_get(cs_aal_ptp_glb_status_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    REG_FIELD_READ(GLOBAL_PTP_GET_TIMESTAMP, val, cfg->get_timestamp); 
    REG_FIELD_READ(GLOBAL_PTP_GET_MPCP_TIMESTAMP, val, cfg->get_mpcp_timestamp); 
    REG_FIELD_READ(GLOBAL_PTP_GET_TIMESTAMP_CMD, done, cfg->get_timestamp_cmd); 
    REG_FIELD_READ(GLOBAL_PTP_SET_TIMESTAMP_CMD, set, cfg->set_timestamp_cmd);

    return CS_E_OK;
}

cs_status aal_ptp_ge_cfg_set(cs_aal_ptp_ge_msk_t msk, cs_aal_ptp_ge_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(msk.u32 == 0){
        return CS_E_OK;
    }
    
    if(msk.s.tx_en)
        REG_FIELD_WRITE(GE_PTP_CONFIG, tx_bypass, ((cfg->tx_en>0)?0:1)); 
    
    if(msk.s.rx_en)
        REG_FIELD_WRITE(GE_PTP_CONFIG, rx_bypass, ((cfg->rx_en>0)?0:1)); 
    
    if(msk.s.egress_bswap_en)
        REG_FIELD_WRITE(GE_PTP_CONFIG, eg_lts_bswap, (cfg->egress_bswap_en>0)); 
    
    if(msk.s.ingress_bswap_en)
        REG_FIELD_WRITE(GE_PTP_CONFIG, ig_lts_bswap, (cfg->ingress_bswap_en>0));

    if(msk.s.packet_cos){
        if(cfg->packet_cos > __PTP_COS_MAX){
            AAL_MIN_LOG("packet_cos invalid.(%d) FILE: %s, LINE: %d\n", cfg->packet_cos, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(GE_PTP_CONFIG, packet_cos, (cfg->packet_cos)); 
    }

    if(msk.s.process_en)
        REG_FIELD_WRITE(GE_PTP_CONFIG, process_en, cfg->process_en);

    if(msk.s.correct_dir){
        if(cfg->correct_dir > AAL_PTP_CORRECT_DIR_ADD){
            AAL_MIN_LOG("correct_dir invalid.(%d) FILE: %s, LINE: %d\n", cfg->correct_dir, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(GE_PTP_ASYM_DELAY, sign, cfg->correct_dir);
    }

    if(msk.s.correct_delta){
        if(cfg->correct_delta > __PTP_CORRECT_DELTA_MAX){
            AAL_MIN_LOG("correct_delta invalid.(%d) FILE: %s, LINE: %d\n", cfg->correct_delta, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(GE_PTP_ASYM_DELAY, correction, cfg->correct_delta);
    }  

    if(msk.s.tx_update_crc)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, tx_update_crc, (cfg->tx_update_crc>0)); 

    if(msk.s.pass_thru_hdr)
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, pass_thru_hdre, (cfg->pass_thru_hdr>0)); 
    
    if(msk.s.txfifo_thrshld){
        if(cfg->txfifo_thrshld > __PTP_TX_FIFO_THRSHLD){
            AAL_MIN_LOG("txfifo_thrshld invalid.(%d) FILE: %s, LINE: %d\n", cfg->txfifo_thrshld, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, txfifo_thrshld, cfg->txfifo_thrshld); 
    }

    if(msk.s.ipg_sel){
        if(cfg->ipg_sel > __PTP_IPG_MAX){
            AAL_MIN_LOG("ipg_sel invalid.(%d) FILE: %s, LINE: %d\n", cfg->ipg_sel, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
    
        REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, tx_ipg_sel, cfg->ipg_sel); 
    }
    
    return CS_E_OK;
    
}



cs_status aal_ptp_ge_cfg_get(cs_aal_ptp_ge_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(GE_PTP_CONFIG, tx_bypass, cfg->tx_en); 
    cfg->tx_en =(cfg->tx_en>0)? 0: 1;
    REG_FIELD_READ(GE_PTP_CONFIG, rx_bypass, cfg->rx_en); 
    cfg->rx_en =(cfg->rx_en>0)? 0: 1;
    REG_FIELD_READ(GE_PTP_CONFIG, eg_lts_bswap, cfg->egress_bswap_en); 
    REG_FIELD_READ(GE_PTP_CONFIG, ig_lts_bswap, cfg->ingress_bswap_en);
    REG_FIELD_READ(GE_PTP_CONFIG, packet_cos, cfg->packet_cos); 
    REG_FIELD_READ(GE_PTP_CONFIG, process_en, cfg->process_en);
    REG_FIELD_READ(GE_PTP_ASYM_DELAY, sign, cfg->correct_dir);            
    REG_FIELD_READ(GE_PTP_ASYM_DELAY, correction, cfg->correct_delta);
    
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, tx_update_crc, cfg->tx_update_crc); 
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, pass_thru_hdre, cfg->pass_thru_hdr); 
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, txfifo_thrshld, cfg->txfifo_thrshld); 
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, tx_ipg_sel, cfg->ipg_sel); 

  
    return CS_E_OK;
}

cs_status aal_ptp_ge_status_get(cs_aal_ptp_status_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_FIELD_READ(GE_PTP_ING_LATCH, local_time, cfg->ingress_timer_latch); 
    REG_FIELD_READ(ONU_MAC_PTP_ING_FWD, local_time, cfg->ingress_timer_fwd);        
    REG_FIELD_READ(GE_PTP_EG_LATCH, local_time, cfg->egress_timer_latch); 
    REG_FIELD_READ(GE_PTP_PROCESS, cf_offset, cfg->cf_offset);
    REG_FIELD_READ(GE_PTP_PROCESS, message_type, cfg->msg_type); 
    REG_FIELD_READ(GE_PTP_EG_STS, lt_valid, cfg->valid);
    
    return CS_E_OK;
}

cs_status aal_ptp_pon_cfg_set(cs_aal_ptp_pon_msk_t msk, cs_aal_ptp_pon_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(msk.u32 == 0){
        return CS_E_OK;
    }

    if(msk.s.tx_en)
        REG_FIELD_WRITE(ONU_MAC_PTP_CONFIG, ptp_tx_en, (cfg->tx_en>0)); 
    
    if(msk.s.rx_en)
        REG_FIELD_WRITE(ONU_MAC_PTP_CONFIG, ptp_rx_en, (cfg->rx_en>0)); 
    
    if(msk.s.egress_bswap_en)
        REG_FIELD_WRITE(ONU_MAC_PTP_CONFIG, eg_lts_bswap, (cfg->egress_bswap_en>0)); 
    
    if(msk.s.ingress_bswap_en)
        REG_FIELD_WRITE(ONU_MAC_PTP_CONFIG, ig_lts_bswap, (cfg->ingress_bswap_en>0));

    if(msk.s.packet_cos){
        if(cfg->packet_cos > __PTP_COS_MAX){
            AAL_MIN_LOG("packet_cos invalid.(%d) FILE: %s, LINE: %d\n", cfg->packet_cos, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        REG_FIELD_WRITE(ONU_MAC_PTP_CONFIG, packet_cos, (cfg->packet_cos)); 
    }

    if(msk.s.tx_crc_recal_en)
        REG_FIELD_WRITE(ONU_MAC_PTP_CONFIG, ptp_tx_crc_recal_en, (cfg->tx_crc_recal_en>0));

    if(msk.s.process_en)
        REG_FIELD_WRITE(ONU_MAC_PTP_CONFIG, process_en, cfg->process_en);

    if(msk.s.correct_dir){
        if(cfg->correct_dir > AAL_PTP_CORRECT_DIR_ADD){
            AAL_MIN_LOG("correct_dir invalid.(%d) FILE: %s, LINE: %d\n", cfg->correct_dir, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_PTP_ASYM_DELAY, sign, cfg->correct_dir);
    }

    if(msk.s.correct_delta){
        if(cfg->correct_delta > __PTP_CORRECT_DELTA_MAX){
            AAL_MIN_LOG("correct_delta invalid.(%d) FILE: %s, LINE: %d\n", cfg->correct_delta, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_PTP_ASYM_DELAY, correction, cfg->correct_delta);
    }
    
    if(msk.s.packet_dpid){
        if(cfg->packet_dpid > __PTP_DPID_MAX){
            AAL_MIN_LOG("packet_dpid invalid.(%d) FILE: %s, LINE: %d\n", cfg->packet_dpid, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(ONU_MAC_PTP_CONFIG, packet_dpid, cfg->packet_dpid);
    }        
    
    return CS_E_OK;
    
}

cs_status aal_ptp_pon_cfg_get(cs_aal_ptp_pon_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    REG_FIELD_READ(ONU_MAC_PTP_CONFIG, ptp_tx_en, cfg->tx_en); 
    REG_FIELD_READ(ONU_MAC_PTP_CONFIG, ptp_rx_en, cfg->rx_en);        
    REG_FIELD_READ(ONU_MAC_PTP_CONFIG, eg_lts_bswap, cfg->egress_bswap_en); 
    REG_FIELD_READ(ONU_MAC_PTP_CONFIG, ig_lts_bswap, cfg->ingress_bswap_en);
    REG_FIELD_READ(ONU_MAC_PTP_CONFIG, packet_cos, cfg->packet_cos); 
    REG_FIELD_READ(ONU_MAC_PTP_CONFIG, ptp_tx_crc_recal_en, cfg->tx_crc_recal_en);
    REG_FIELD_READ(ONU_MAC_PTP_CONFIG, process_en, cfg->process_en);
    REG_FIELD_READ(ONU_MAC_PTP_ASYM_DELAY, sign, cfg->correct_dir);
    REG_FIELD_READ(ONU_MAC_PTP_ASYM_DELAY, correction, cfg->correct_delta);
    REG_FIELD_READ(ONU_MAC_PTP_CONFIG, packet_dpid, cfg->packet_dpid);
   
    return CS_E_OK;
}

cs_status aal_ptp_pon_status_get(cs_aal_ptp_status_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    REG_FIELD_READ(ONU_MAC_PTP_ING_LATCH, local_time, cfg->ingress_timer_latch); 
    REG_FIELD_READ(GE_PTP_ING_FWD, local_time, cfg->ingress_timer_fwd);        
    REG_FIELD_READ(ONU_MAC_PTP_EG_LATCH, local_time, cfg->egress_timer_latch); 
    REG_FIELD_READ(ONU_MAC_PTP_PROCESS, cf_offset, cfg->cf_offset);
    REG_FIELD_READ(ONU_MAC_PTP_PROCESS, message_type, cfg->msg_type); 
    REG_FIELD_READ(ONU_MAC_PTP_EG_STS, lt_valid, cfg->valid);

    return CS_E_OK;
}

cs_status aal_ptp_ma_cfg_set(cs_aal_ptp_ma_msk_t msk, cs_aal_ptp_ma_cfg_t *cfg)
{
    cs_uint8 data;
    
    if(NULL == cfg){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(msk.u32 == 0){
        return CS_E_OK;
    }
    
    if(msk.s.en){
        REG_FIELD_WRITE(MA_PTP_CTRL0, ptp_en, (cfg->en>0)); 
        REG_FIELD_WRITE(MA_CPU_CTRL, ptp_enable, (cfg->en>0));
    }
        
    if(msk.s.egress_bswap_en)
        REG_FIELD_WRITE(MA_CPU_CTRL, tx_byte_swap_en, (cfg->egress_bswap_en>0));
        
    if(msk.s.ingress_bswap_en)
        REG_FIELD_WRITE(MA_CPU_CTRL, rx_byte_swap_en, (cfg->ingress_bswap_en>0)); 

    if(msk.s.es_en)
        REG_FIELD_WRITE(MA_CPU_CTRL, es, (cfg->es_en>0));
        
    if(msk.s.lookup_bypass_en){
        REG_FIELD_READ(MA_CPU_CTRL, bypass, data);
        data &= 0xf;
        
        if(cfg->lookup_bypass_en){
            data |= (1<<2);
            REG_FIELD_WRITE(MA_CPU_CTRL, bypass, data); 
        }
        else{
            data &= ~(1<<2);
            REG_FIELD_WRITE(MA_CPU_CTRL, bypass, data); 
        }
    }
    
    if(msk.s.mir_cos){
        if(cfg->mir_cos>__PTP_COS_MAX){
            AAL_MIN_LOG("mir_cos invalid.(%d) FILE: %s, LINE: %d\n", cfg->mir_cos, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(MA_CPU_CTRL, mir_cos, cfg->mir_cos);
    }
        
    if(msk.s.mir_dpid){
        if(cfg->mir_dpid>__PTP_DPID_MAX){
            AAL_MIN_LOG("mir_dpid invalid.(%d) FILE: %s, LINE: %d\n", cfg->mir_dpid, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(MA_CPU_CTRL, mir_dpid, cfg->mir_dpid); 
    }
    
    if(msk.s.cpu_cos){
        if(cfg->cpu_cos>__PTP_COS_MAX){
            AAL_MIN_LOG("cpu_cos invalid.(%d) FILE: %s, LINE: %d\n", cfg->cpu_cos, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(MA_CPU_CTRL, cpu_cos, cfg->cpu_cos);
    }
        
    if(msk.s.cpu_dpid){
        if(cfg->cpu_dpid>__PTP_DPID_MAX){
            AAL_MIN_LOG("cpu_dpid invalid.(%d) FILE: %s, LINE: %d\n", cfg->cpu_dpid, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(MA_CPU_CTRL, cpu_dpid, cfg->cpu_dpid);
    }
    
    if(msk.s.dma_cos){
        if(cfg->dma_cos>__PTP_COS_MAX){
            AAL_MIN_LOG("dma_cos invalid.(%d) FILE: %s, LINE: %d\n", cfg->dma_cos, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(MA_CPU_CTRL, dma_cos, cfg->dma_cos);
    }
        
    if(msk.s.dma_dpid){
        if(cfg->dma_dpid>__PTP_DPID_MAX){
            AAL_MIN_LOG("dma_dpid invalid.(%d) FILE: %s, LINE: %d\n", cfg->dma_dpid, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(MA_CPU_CTRL, dma_dpid, cfg->dma_dpid); 
    }

    if(msk.s.mii0_cos){
        if(cfg->mii0_cos>__PTP_COS_MAX){
            AAL_MIN_LOG("mii0_cos invalid.(%d) FILE: %s, LINE: %d\n", cfg->mii0_cos, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(MA_CPU_CTRL, mii0_cos, cfg->mii0_cos); 
    }
    
    if(msk.s.mii0_dpid){
        if(cfg->mii0_dpid>__PTP_DPID_MAX){
            AAL_MIN_LOG("mii0_dpid invalid.(%d) FILE: %s, LINE: %d\n", cfg->mii0_dpid, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(MA_CPU_CTRL, mii0_dpid, cfg->mii0_dpid);
    }
    
    return CS_E_OK;
    
}

cs_status aal_ptp_ma_cfg_get(cs_aal_ptp_ma_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    //REG_FIELD_READ(MA_PTP_CTRL0, ptp_en, cfg->en); 
    REG_FIELD_READ(MA_CPU_CTRL, ptp_enable, cfg->en);
    REG_FIELD_READ(MA_CPU_CTRL, tx_byte_swap_en, cfg->egress_bswap_en);
    REG_FIELD_READ(MA_CPU_CTRL, rx_byte_swap_en, cfg->ingress_bswap_en); 
    REG_FIELD_READ(MA_CPU_CTRL, es, cfg->es_en);
    REG_FIELD_READ(MA_CPU_CTRL, bypass, cfg->lookup_bypass_en); 
    REG_FIELD_READ(MA_CPU_CTRL, mir_cos, cfg->mir_cos); 
    REG_FIELD_READ(MA_CPU_CTRL, mir_dpid, cfg->mir_dpid); 
    REG_FIELD_READ(MA_CPU_CTRL, cpu_cos, cfg->cpu_cos);
    REG_FIELD_READ(MA_CPU_CTRL, cpu_dpid, cfg->cpu_dpid);
    REG_FIELD_READ(MA_CPU_CTRL, dma_cos, cfg->dma_cos);
    REG_FIELD_READ(MA_CPU_CTRL, dma_dpid, cfg->dma_dpid); 
    REG_FIELD_READ(MA_CPU_CTRL, mii0_cos, cfg->mii0_cos); 
    REG_FIELD_READ(MA_CPU_CTRL, mii0_dpid, cfg->mii0_dpid);
    
    return CS_E_OK;
}


cs_status ptp_init(void)
{
    cs_aal_ptp_glb_msk_t glb_msk;
    cs_aal_ptp_glb_cfg_t glb_cfg;
    cs_aal_ptp_pon_msk_t pon_msk;
    cs_aal_ptp_pon_cfg_t pon_cfg;
    cs_aal_ptp_ge_msk_t  ge_msk;
    cs_aal_ptp_ge_cfg_t  ge_cfg;
    cs_aal_ptp_ma_msk_t  ma_msk;
    cs_aal_ptp_ma_cfg_t  ma_cfg;
    cs_status rt = CS_E_OK;

    pon_cfg.tx_en = 1;
    pon_cfg.rx_en = 1;
    pon_cfg.egress_bswap_en = 0;
    pon_cfg.ingress_bswap_en = 0;
    pon_cfg.packet_cos = 0;    /*configured according to test scripts*/
    pon_cfg.process_en = 0xffff;
    pon_cfg.tx_crc_recal_en = 1;
    pon_cfg.correct_dir = 0;
    pon_cfg.correct_delta = 0;
    pon_cfg.packet_dpid = 4; /*configured according to test scripts*/
    
    pon_msk.u32 = 0;
    pon_msk.s.tx_en = 1;
    pon_msk.s.rx_en = 1;
    pon_msk.s.egress_bswap_en = 1;
    pon_msk.s.ingress_bswap_en = 1;
    pon_msk.s.packet_cos = 1;
    pon_msk.s.process_en = 1;
    pon_msk.s.tx_crc_recal_en = 1;
    pon_msk.s.correct_dir = 1;
    pon_msk.s.correct_delta = 1;
    pon_msk.s.packet_dpid = 1;

    aal_ptp_pon_cfg_set(pon_msk, &pon_cfg);
    
    ge_cfg.tx_en = 1;
    ge_cfg.rx_en = 1;
    ge_cfg.egress_bswap_en = 0;
    ge_cfg.ingress_bswap_en = 0;
    ge_cfg.packet_cos = 1;  /*configured according to test scripts*/
    ge_cfg.process_en = 0xffff;
    ge_cfg.correct_dir = 0;
    ge_cfg.correct_delta = 0;
    ge_cfg.tx_update_crc = 1; /*configured according to test scripts*/ 
    ge_cfg.pass_thru_hdr = 1;  /*configured according to test scripts*/ 
    ge_cfg.txfifo_thrshld = 0x32; /*configured according to test scripts, sdl will overwrite this value*/
    ge_cfg.ipg_sel = 0x1;         /*configured according to test scripts*/
    
    ge_msk.u32 = 0;
    ge_msk.s.tx_en = 1;
    ge_msk.s.rx_en = 1;
    ge_msk.s.egress_bswap_en = 1;
    ge_msk.s.ingress_bswap_en = 1;
    ge_msk.s.packet_cos = 1;
    ge_msk.s.process_en = 1;
    ge_msk.s.correct_dir = 1;
    ge_msk.s.correct_delta = 1;
    ge_msk.s.tx_update_crc = 1;  
    ge_msk.s.pass_thru_hdr = 1;  
    ge_msk.s.txfifo_thrshld = 1;
    ge_msk.s.ipg_sel = 1;

    aal_ptp_ge_cfg_set(ge_msk, &ge_cfg);
    
    
    //REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, tx_flow_disable, 1); (to be verified)

    ma_cfg.en = 1;  
    //ma_cfg.egress_bswap_en = 0;  
    //ma_cfg.ingress_bswap_en = 0;  
    ma_cfg.es_en = 1;     /*configured according to test scripts*/
    ma_cfg.lookup_bypass_en = 1;  /*configured according to test scripts*/
    ma_cfg.mir_cos = 0;  
    ma_cfg.mir_dpid = 0;  
    //ma_cfg.cpu_cos = 0x4;  /*configured according to test scripts*/
    //ma_cfg.cpu_dpid = 0x1;  /*configured according to test scripts*/
    ma_cfg.dma_cos = 0;  
    ma_cfg.dma_dpid= 0;  
    ma_cfg.mii0_cos = 0;
    ma_cfg.mii0_dpid = 0;  
    
    ma_msk.u32 = 0;
    ma_msk.s.en = 1;  
    //ma_msk.s.egress_bswap_en = 1;  
    //ma_msk.s.ingress_bswap_en = 1;  
    ma_msk.s.es_en = 1;
    ma_msk.s.lookup_bypass_en = 1;  
    ma_msk.s.mir_cos = 1;  
    ma_msk.s.mir_dpid= 1;  
    //ma_msk.s.cpu_cos = 1;
    //ma_msk.s.cpu_dpid = 1;  
    ma_msk.s.dma_cos = 1;  
    ma_msk.s.dma_dpid = 1;  
    ma_msk.s.mii0_cos = 1;
    ma_msk.s.mii0_dpid = 1;  
    
    aal_ptp_ma_cfg_set(ma_msk, &ma_cfg);

    glb_cfg.ref_timer = 0;
    glb_cfg.set_timestamp = 0;
    
    glb_msk.u32 = 0;
    glb_msk.s.ref_timer = 1;
    glb_msk.s.set_timestamp = 1;

    aal_ptp_glb_cfg_set(glb_msk, &glb_cfg);
    
    //aal_ptp_int_enable();

    return rt;   
}


cs_status aal_tod_cfg_set(cs_aal_tod_msk_t msk, cs_aal_tod_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(msk.u32 == 0){
        return CS_E_OK;
    }

    if(msk.s.en){
        REG_FIELD_WRITE(GLOBAL_PIN_CTRL, glb_ptp_1pps_seln, ((cfg->en?0:1))); 
       
       /*to be verified*/
       /* // Disable PPS
          glb_ctrl = reg_read( LYNXD_GLOBAL_PIN_CTRL);
          glb_ctrl &= 0xffbfffff;
          glb_ctrl |= 0x400000;
          reg_write(LYNXD_GLOBAL_PIN_CTRL, glb_ctrl);
          
          
          // Enable PPS
            glb_ctrl = reg_read( LYNXD_GLOBAL_PIN_CTRL);
            glb_ctrl ^= 0x400000;
            reg_write(LYNXD_GLOBAL_PIN_CTRL, glb_ctrl);
       */
    }

    if(msk.s.s_pps)
        REG_FIELD_WRITE(ONU_MAC_TOD_S_PPS, s_pps, cfg->s_pps); 
    
    if(msk.s.pps_width)
        REG_FIELD_WRITE(ONU_MAC_TOD_PPS_WID, pps_width, cfg->pps_width); 

    if(msk.s.s_tod)
        REG_FIELD_WRITE(ONU_MAC_TOD_S_TOD, s_tod, cfg->s_tod);

    if(msk.s.counter){
        if(cfg->counter > __TOD_CHAR_NUM_MAX){
            AAL_MIN_LOG("counter invalid.(%d) FILE: %s, LINE: %d\n", cfg->counter, __FILE__, __LINE__);
            return CS_E_PARAM;
        }
        
        REG_FIELD_WRITE(PER_TOD_CTRL, tod_counter, cfg->counter);
    }

    return CS_E_OK;

}

cs_status aal_tod_cfg_get(cs_aal_tod_cfg_t *cfg)
{
    if(NULL == cfg){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    REG_FIELD_READ(GLOBAL_PIN_CTRL, glb_ptp_1pps_seln, cfg->en);
    cfg->en = (cfg->en?0:1);
    REG_FIELD_READ(ONU_MAC_TOD_S_PPS, s_pps, cfg->s_pps); 
    REG_FIELD_READ(ONU_MAC_TOD_PPS_WID, pps_width, cfg->pps_width); 
    REG_FIELD_READ(ONU_MAC_TOD_S_TOD, s_tod, cfg->s_tod);
    REG_FIELD_READ(PER_TOD_CTRL, tod_counter, cfg->counter);

    return CS_E_OK;

}

cs_status aal_ptp_int_enable(cs_aal_ptp_int_msk_t mask)
{
    cs_uint32 int_en;

    if(mask.u32 == 0){
        return CS_E_OK;
    }
    
    REG_READ(GLOBAL_PTP_INTENABLE, int_en);
    int_en |= mask.u32;
    REG_WRITE(GLOBAL_PTP_INTENABLE, int_en);
    
    return CS_E_OK; 
}


cs_status aal_ptp_int_disable(cs_aal_ptp_int_msk_t mask)
{
    cs_uint32 int_en;
    
    if(mask.u32 == 0){
        return CS_E_OK;
    }
    
    REG_READ(GLOBAL_PTP_INTENABLE, int_en);
    int_en &= (~mask.u32);
    REG_WRITE(GLOBAL_PTP_INTENABLE, int_en);
    
    return CS_E_OK; 
}

cs_status aal_ptp_int_en_get(cs_aal_ptp_int_msk_t *mask)
{
    cs_uint32 int_en;
    
    if(NULL == mask){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_READ(GLOBAL_PTP_INTENABLE, int_en);
    mask->u32 = int_en;

    return CS_E_OK;
}

cs_status aal_ptp_int_status_get(cs_aal_ptp_int_msk_t* mask)
{

    cs_uint32 int_src;
    
    if(NULL == mask){
        AAL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    REG_READ(GLOBAL_PTP_INTERRUPT, int_src);
    mask->u32 = int_src;
    return CS_E_OK; 
}

cs_status aal_ptp_int_status_clr(cs_aal_ptp_int_msk_t src)
{
    if(src.u32 == 0){
        return CS_E_OK;
    }
    
    REG_WRITE(GLOBAL_PTP_INTERRUPT, src.u32); /*write 1 to clear*/

    return CS_E_OK; 
}

