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
#include "iros_config.h"
#include "plat_common.h"
#include "aal_bm.h"
#include "aal_util.h"
#include "aal_bm_util.h"
#include "aal_l2.h"

cs_uint64 g_fe_drp[3] = {0};
cs_uint64 g_bm_drp[3] = {0};

// move to SAL
// cs_uint16 g_us_max_que_size;
// cs_uint16 g_ds_max_que_size;


/************************ private part ******************************/

cs_uint32 __reg_value_get(cs_uint32 address)
{
    cs_uint32 value;
    REG_READ(address, value);
    return value;
}

cs_status __bm_ext_cfg_set(__bm_ext_cfg_msk_t mask, __bm_ext_cfg_t *cfg)
{   
    if (!cfg || mask.u == 0)
        return CS_E_PARAM;

    if(mask.s.bm_mode) 
    {
        REG_FIELD_WRITE(GLOBAL_CTRL,rd_clr,1);
    
        if (cfg->bm_mode == __FIFO_MODE) 
        {
            REG_FIELD_WRITE(GLOBAL_CTRL,legacy_mode,0);
            REG_FIELD_WRITE(GLOBAL_CTRL,bm_sram_cfg,0);
        } 
        else 
        {
            REG_FIELD_WRITE(GLOBAL_CTRL,legacy_mode,1);
        }
    }

    //sch_cfg
    if (mask.s.burst_mode_rsvd)
        REG_FIELD_WRITE(BM_SCH_CFG,burst_mode_rsvd,cfg->burst_mode_rsvd);   
    if (mask.s.cpu_ql_en)
        REG_FIELD_WRITE(BM_SCH_CFG,cpu_ql_en,cfg->cpu_ql_en);
    if (mask.s.dis_win_fit_nsp_rsvd) 
        REG_FIELD_WRITE(BM_SCH_CFG,dis_win_fit_nsp_rsvd,cfg->dis_win_fit_nsp_rsvd);   
    if (mask.s.dis_win_fit_sp_rsvd) 
        REG_FIELD_WRITE(BM_SCH_CFG,dis_win_fit_sp_rsvd,cfg->dis_win_fit_sp_rsvd);
    if (mask.s.ds_abr_en) 
        REG_FIELD_WRITE(BM_SCH_CFG,ds_abr_en,cfg->ds_abr_en);
    if (mask.s.ds_dp_en)
        REG_FIELD_WRITE(BM_SCH_CFG,ds_dp_en,cfg->ds_dp_en);
    if (mask.s.ge_cpu_pkt_en) 
        REG_FIELD_WRITE(BM_SCH_CFG,ge_cpu_pkt_en,cfg->ge_cpu_pkt_en);
    if (mask.s.ge_ql_en) 
        REG_FIELD_WRITE(BM_SCH_CFG,ge_ql_en,cfg->ge_ql_en);
    if (mask.s.kt_mode_rsvd) 
        REG_FIELD_WRITE(BM_SCH_CFG,kt_mode_rsvd,cfg->kt_mode_rsvd);
    if (mask.s.pon_cpu_pkt_en) 
        REG_FIELD_WRITE(BM_SCH_CFG,pon_cpu_pkt_en,cfg->pon_cpu_pkt_en);
    if (mask.s.pon_ql_en)
        REG_FIELD_WRITE(BM_SCH_CFG,pon_ql_en,cfg->pon_ql_en);
    if (mask.s.unknown_dpid) 
        REG_FIELD_WRITE(BM_SCH_CFG,unknown_dpid,cfg->unknown_dpid);
    if (mask.s.us_abr_en)
        REG_FIELD_WRITE(BM_SCH_CFG,us_abr_en,cfg->us_abr_en);
    if (mask.s.us_dp_en)
        REG_FIELD_WRITE(BM_SCH_CFG,us_dp_en,cfg->us_dp_en);
    if (mask.s.voq8_en) 
        REG_FIELD_WRITE(BM_SCH_CFG,voq8_en,cfg->voq8_en);

    //share buf
    if (mask.s.ppbm_en_cpu)
        REG_FIELD_WRITE(BM_SHARED_BUFFER_CFG,ppbm_en_cpu,cfg->ppbm_en_cpu);
    if (mask.s.ppbm_en ) 
        REG_FIELD_WRITE(BM_SHARED_BUFFER_CFG,ppbm_en,cfg->ppbm_en);
    if (mask.s.slmt_cpu )
        REG_FIELD_WRITE(BM_SHARED_BUFFER_CFG,slmt_cpu,cfg->slmt_cpu);
    if (mask.s.slmt ) 
        REG_FIELD_WRITE(BM_SHARED_BUFFER_CFG,slmt,cfg->slmt);
    
    //shaper cfg
    if (mask.s.cpu_cnt_option_rsvd) 
        REG_FIELD_WRITE(BM_PORT_SHAPER_CFG,cpu_cnt_option_rsvd,cfg->cpu_cnt_option_rsvd);
    if (mask.s.cpu_shaper_en)
        REG_FIELD_WRITE(BM_PORT_SHAPER_CFG,cpu_shaper_en,cfg->cpu_shaper_en);    
    if (mask.s.ge_shaper_en)
        REG_FIELD_WRITE(BM_PORT_SHAPER_CFG,ge_shaper_en,cfg->ge_shaper_en);    
    if (mask.s.pon_shaper_en)
        REG_FIELD_WRITE(BM_PORT_SHAPER_CFG,pon_shaper_en,cfg->pon_shaper_en);    
    if (mask.s.w_rate) 
        REG_FIELD_WRITE(BM_PORT_SHAPER_CFG,w_rate,cfg->w_rate);
   
    //abr cfg
    if (mask.s.abr_ds_thrsld)
        REG_FIELD_WRITE(BM_ABR_CFG,ds_thrsld,cfg->abr_ds_thrsld);
    if (mask.s.abr_us_thrsld)
        REG_FIELD_WRITE(BM_ABR_CFG,us_thrsld,cfg->abr_us_thrsld);
    //dp cfg
     if (mask.s.dp_ds_thrsld)
        REG_FIELD_WRITE(BM_ABR_CFG,ds_thrsld,cfg->dp_ds_thrsld);
    if (mask.s.dp_us_thrsld)
        REG_FIELD_WRITE(BM_ABR_CFG,us_thrsld,cfg->dp_us_thrsld);  

    if (mask.s.win_chk_en_rsvd)
        REG_FIELD_WRITE(BM_MPCP_CFG,win_chk_en_rsvd,cfg->win_chk_en_rsvd);  
    if (mask.s.edwrr_en_rsvd)
        REG_FIELD_WRITE(BM_MPCP_CFG,edwrr_en_rsvd,cfg->edwrr_en_rsvd);  

    if(mask.s.linklist_size)
        REG_FIELD_WRITE(BM_LINKLIST_SIZE,size,cfg->linklist_size);

    if(mask.s.pon_tx_lmt_en)
        REG_FIELD_WRITE(BM_PON_PORT_TX_BUFFER_LMT,tx_lmt_en,cfg->pon_tx_lmt_en);
    if(mask.s.pon_tx_buffer_thrsld)
        REG_FIELD_WRITE(BM_PON_PORT_TX_BUFFER_LMT,tx_buffer_thrsld,cfg->pon_tx_buffer_thrsld);
    if(mask.s.ge_tx_lmt_en)
        REG_FIELD_WRITE(BM_GE_PORT_TX_BUFFER_LMT,tx_lmt_en,cfg->ge_tx_lmt_en);    
    if(mask.s.ge_tx_buffer_thrsld)
        REG_FIELD_WRITE(BM_GE_PORT_TX_BUFFER_LMT,tx_buffer_thrsld,cfg->ge_tx_buffer_thrsld);
    if(mask.s.ddr_weight_base)
        REG_FIELD_WRITE(BM_DRR_WEIGHT_BASE,base,cfg->ddr_weight_base);

    return CS_E_OK;
}

cs_status __bm_ext_cfg_get(__bm_ext_cfg_t *cfg)
{
    BM_SCH_CFG_t bm_sch_cfg;
    BM_SHARED_BUFFER_CFG_t share_buf_cfg;
    BM_PORT_SHAPER_CFG_t port_shaper_cfg;
    BM_ABR_CFG_t abr_cfg;
    BM_DP_CFG_t dp_cfg;
    BM_MPCP_CFG_t mpcp_cfg;
    GLOBAL_CTRL_t glb_ctrl;
    BM_LINKLIST_SIZE_t ll_size;
    BM_PON_PORT_TX_BUFFER_LMT_t pon_tx_buf_lmt;
    BM_GE_PORT_TX_BUFFER_LMT_t ge_tx_buf_lmt;
    BM_DRR_WEIGHT_BASE_t  weight_base;
    
    if (!cfg)
        return CS_E_PARAM;

    REG_READ(BM_SCH_CFG,bm_sch_cfg.wrd);
    cfg->burst_mode_rsvd = bm_sch_cfg.bf.burst_mode_rsvd;
    cfg->cpu_ql_en = bm_sch_cfg.bf.cpu_ql_en;
    cfg->dis_win_fit_nsp_rsvd = bm_sch_cfg.bf.dis_win_fit_nsp_rsvd;
    cfg->dis_win_fit_sp_rsvd = bm_sch_cfg.bf.dis_win_fit_sp_rsvd;
    cfg->ds_abr_en = bm_sch_cfg.bf.ds_abr_en;
    cfg->ds_dp_en = bm_sch_cfg.bf.ds_dp_en;
    cfg->ge_cpu_pkt_en = bm_sch_cfg.bf.ge_cpu_pkt_en;
    cfg->ge_ql_en = bm_sch_cfg.bf.ge_ql_en;
    cfg->kt_mode_rsvd = bm_sch_cfg.bf.kt_mode_rsvd;
    cfg->pon_cpu_pkt_en = bm_sch_cfg.bf.pon_cpu_pkt_en;
    cfg->pon_ql_en = bm_sch_cfg.bf.pon_ql_en;
    cfg->unknown_dpid = bm_sch_cfg.bf.unknown_dpid;
    cfg->us_abr_en = bm_sch_cfg.bf.us_abr_en;
    cfg->us_dp_en = bm_sch_cfg.bf.us_dp_en;
    cfg->voq8_en = bm_sch_cfg.bf.voq8_en;

    REG_READ(BM_SHARED_BUFFER_CFG,share_buf_cfg.wrd);
    cfg->ppbm_en_cpu = share_buf_cfg.bf.ppbm_en_cpu;
    cfg->ppbm_en = share_buf_cfg.bf.ppbm_en_cpu;
    cfg->slmt_cpu = share_buf_cfg.bf.slmt_cpu;
    cfg->slmt = share_buf_cfg.bf.slmt;

    REG_READ(BM_PORT_SHAPER_CFG,port_shaper_cfg.wrd);
    cfg->cpu_cnt_option_rsvd = port_shaper_cfg.bf.cpu_cnt_option_rsvd ;
    cfg->cpu_shaper_en  = port_shaper_cfg.bf.cpu_shaper_en;
    cfg->ge_shaper_en  = port_shaper_cfg.bf.ge_shaper_en;
    cfg->pon_shaper_en = port_shaper_cfg.bf.pon_shaper_en;
    cfg->w_rate = port_shaper_cfg.bf.w_rate;

    REG_READ(BM_ABR_CFG,abr_cfg.wrd);
    cfg->abr_ds_thrsld = abr_cfg.bf.ds_thrsld;
    cfg->abr_us_thrsld = abr_cfg.bf.us_thrsld;

    REG_READ(BM_DP_CFG,dp_cfg.wrd);
    cfg->dp_ds_thrsld = dp_cfg.bf.ds_thrsld;
    cfg->dp_us_thrsld = dp_cfg.bf.us_thrsld;

    REG_READ(BM_MPCP_CFG,mpcp_cfg.wrd);
    cfg->win_chk_en_rsvd = mpcp_cfg.bf.win_chk_en_rsvd;
    cfg->edwrr_en_rsvd = mpcp_cfg.bf.edwrr_en_rsvd;

    REG_READ(GLOBAL_CTRL,glb_ctrl.wrd);
    cfg->bm_mode = (glb_ctrl.bf.legacy_mode == 1)? __MEMORY_MODE : __FIFO_MODE;

    REG_READ(BM_LINKLIST_SIZE,ll_size.wrd);
    cfg->linklist_size = ll_size.bf.size;

    REG_READ(BM_PON_PORT_TX_BUFFER_LMT,pon_tx_buf_lmt.wrd);
    cfg->pon_tx_lmt_en = pon_tx_buf_lmt.bf.tx_lmt_en;
    cfg->pon_tx_buffer_thrsld = pon_tx_buf_lmt.bf.tx_buffer_thrsld;

    REG_READ(BM_GE_PORT_TX_BUFFER_LMT,ge_tx_buf_lmt.wrd);
    cfg->ge_tx_lmt_en = ge_tx_buf_lmt.bf.tx_lmt_en;
    cfg->ge_tx_buffer_thrsld = ge_tx_buf_lmt.bf.tx_buffer_thrsld;

    REG_READ(BM_DRR_WEIGHT_BASE,weight_base.wrd);
    cfg->ddr_weight_base = weight_base.bf.base;
   
    return CS_E_OK;
}

cs_status __bm_port_tx_ctrl_set(__bm_port_tx_ctrl_msk_t mask, __bm_port_tx_ctrl_t *cfg)
{
    BM_PORT_TX_CTRL_t port_tx_ctrl;
    BM_PORT_TX_CTRL_t port_tx_ctrl_msk;
    
    if (!cfg || mask.u32 == 0) 
        return CS_E_PARAM;

    port_tx_ctrl.wrd = 0;
    port_tx_ctrl_msk.wrd = 0;
    
    if (mask.s.cpu_tx_en > 0) 
    {
        port_tx_ctrl.bf.cpu_tx_en = cfg->cpu_tx_en;
        port_tx_ctrl_msk.bf.cpu_tx_en = mask.s.cpu_tx_en;
    }
    if (mask.s.ge_tx_en > 0) 
    {
        port_tx_ctrl.bf.ge_tx_en = cfg->ge_tx_en;
        port_tx_ctrl_msk.bf.ge_tx_en = mask.s.ge_tx_en;
    }
    if (mask.s.pon_tx_en > 0) 
    {
        port_tx_ctrl.bf.pon_tx_en = cfg->pon_tx_en;
        port_tx_ctrl_msk.bf.pon_tx_en = mask.s.pon_tx_en;
    }
    if (mask.s.stop_cpu_rx) 
    {
        port_tx_ctrl.bf.stop_cpu_rx = cfg->stop_cpu_rx;
        port_tx_ctrl_msk.bf.stop_cpu_rx = 1;
    }
    if (mask.s.stop_ge_rx) 
    {
        port_tx_ctrl.bf.stop_ge_rx = cfg->stop_ge_rx;
        port_tx_ctrl_msk.bf.stop_ge_rx = 1;
    }
    if (mask.s.stop_pon_rx) 
    {
        port_tx_ctrl.bf.stop_pon_rx = cfg->stop_pon_rx;
        port_tx_ctrl_msk.bf.stop_pon_rx = 1;
    }
    REG_MASK_WRITE(BM_PORT_TX_CTRL, port_tx_ctrl_msk.wrd, port_tx_ctrl.wrd);

    return CS_E_OK;
}

cs_status __bm_port_tx_ctrl_get(__bm_port_tx_ctrl_t *cfg)
{
    BM_PORT_TX_CTRL_t port_tx_ctrl;

    if (!cfg)
        return CS_E_PARAM;

    REG_READ(BM_PORT_TX_CTRL,port_tx_ctrl.wrd);
    cfg->cpu_tx_en = port_tx_ctrl.bf.cpu_tx_en;
    cfg->ge_tx_en = port_tx_ctrl.bf.ge_tx_en;
    cfg->pon_tx_en = port_tx_ctrl.bf.pon_tx_en;
    cfg->stop_cpu_rx = port_tx_ctrl.bf.stop_cpu_rx;
    cfg->stop_ge_rx = port_tx_ctrl.bf.stop_ge_rx;
    cfg->stop_pon_rx = port_tx_ctrl.bf.stop_pon_rx;
    
    return CS_E_OK;
}

void __bm_packet_header_address_get(cs_uint32 size, cs_uint16 pre_eh, cs_uint32* start, cs_uint32* end)
{
    cs_uint32 tmp, start_p, start_h, end_p, end_h;

    if ((start == NULL) || (end == NULL))
        return;

    /*1. packet size enlarge 4% percent*/
    tmp = size * __BM_KB_NUM;
    
    /*2. calculate header address*/
    if (pre_eh % 4 == 0) 
    {
        start_h = pre_eh;
    } 
    else 
    {
        start_h = pre_eh + 1;
    }
    
    if (start_h > (0x47ff - 12))   /*too close the boundary*/
    {
        end_h = start_h + 1;
        start_p = start_h * 64 / 12;
        end_p = start_p + 3;
    } 
    else 
    {
        if (tmp) 
        {
            start_h = ((pre_eh + 11) / 12) * 12;
            end_h = (tmp + 63) / 64 + start_h;
            /*3. align header end address*/
            end_h = (end_h / 12) * 12 + 11;
            end_h = (end_h > 0x47ff) ? 0x47ff : end_h;
        } 
        else 
        {
            end_h = start_h;
        }
        
        /*4. calculate packet address*/
        start_p = start_h * 64 / 12;
        if (!tmp)
            end_p = start_p;
        else 
        {
            end_p = start_p + tmp / 12;
            end_p = end_p / 4 * 4 + 3;
        }
    }

    *start = start_p + (start_h << 17);
    *end = end_p + (end_h << 17);
}

cs_status __bm_calc_que_len(cs_uint8 *weight, cs_uint8 *ql)
{
    cs_uint8 index1, index;
    cs_uint16 temp_ql, total_weight = 0;
    cs_uint32 que_buffer_size;
    
    if (!weight || !ql) 
        return CS_E_PARAM;

    for (index1 = 0; index1 < 8; index1++) 
    {
        if (weight[index1] == 0)
            break;

        total_weight += weight[index1];
    }

    for (index = 0; index < 8; index++) 
    {
        if (index1 < 7) 
            que_buffer_size = 100 * 1024 * index1 * weight[index] * 32 / 8 / total_weight;
        else 
            que_buffer_size = 150 * 1024 * weight[index] * 32 / total_weight;
       
        if (que_buffer_size < (128*1024)) 
            que_buffer_size = 128 * 1024;
        
        temp_ql = (que_buffer_size / 48 - 255) / 512;
        if (temp_ql > 255) 
            ql[index]  = 0xff;
        else 
            ql[index] = (cs_uint8)temp_ql;
    }
    
    return CS_E_OK;
}

cs_status __bm_re_dir_enable(cs_aal_port_id_t port_id, cs_boolean enable)
{
    FE_LE_PR_DPID_PRMS_CTRL_t mask, data;
    
    mask.wrd = 0;
    data.wrd = 0;
    
    mask.bf.dpid_redirect0_en = 1;
    mask.bf.redirect0_compare_dpid = 7;
    mask.bf.redirect0_fwd_dpid = 7;
    
    if (enable) 
    {
        data.bf.dpid_redirect0_en = 1;
        if (port_id == AAL_PORT_ID_GE) 
        {
            data.bf.redirect0_compare_dpid = 1;
        } 
        else 
        {
            data.bf.redirect0_compare_dpid = 0;
        }
        data.bf.redirect0_fwd_dpid = 3;
    } 
    else 
    {
        data.bf.dpid_redirect0_en = 0;
        data.bf.redirect0_compare_dpid = 0;
        data.bf.redirect0_fwd_dpid = 0;
    }
    REG_MASK_WRITE_PORT(FE_LE_PR_DPID_PRMS_CTRL,port_id, mask.wrd, data.wrd);


    return CS_E_OK;
}

cs_status __bm_flush_done_check(cs_aal_port_id_t port)
{
    BM_FLUSH_STATUS_t flush_done;
    cs_uint32 counter = 0;
    
    switch (port) 
    {
    case AAL_PORT_ID_CPU:
        for (counter = 0; counter < __BM_FLUSH_CHECK_TIME; counter++) 
        {
            REG_READ(BM_FLUSH_STATUS,flush_done.wrd);
            if (flush_done.bf.cpu_flush_done) 
                return CS_E_OK;
        }
        break;
    case AAL_PORT_ID_GE:
        for (counter = 0; counter < __BM_FLUSH_CHECK_TIME; counter++)
        {
            REG_READ(BM_FLUSH_STATUS,flush_done.wrd);
            if (flush_done.bf.ge_flush_done) 
                return CS_E_OK;
        }
        break;
    case AAL_PORT_ID_PON:
        for (counter = 0; counter < __BM_FLUSH_CHECK_TIME; counter++) 
        {
            REG_READ(BM_FLUSH_STATUS,flush_done.wrd);
            if (flush_done.bf.pon_flush_done) 
                return CS_E_OK;
        }
        break;
    default:
        return CS_E_PARAM;
    }
    
    return CS_E_ERROR;
}

cs_status __bm_flush(cs_aal_port_id_t port)
{
    cs_status rt = CS_E_OK;
    __bm_port_tx_ctrl_t port_tx_ctrl;    
    __bm_port_tx_ctrl_msk_t port_tx_ctrl_msk;    
    __bm_ext_cfg_t ext_cfg;
    __bm_ext_cfg_msk_t ext_cfg_msk;
    cs_uint8 us_abr, win_chck;
    
    port_tx_ctrl_msk.u32 = 0;
    ext_cfg_msk.u = 0;

    switch (port) {
    case AAL_PORT_ID_CPU:
        port_tx_ctrl.cpu_tx_en  = 0;
        port_tx_ctrl.stop_ge_rx = 1;
        port_tx_ctrl.stop_pon_rx = 1;
        port_tx_ctrl_msk.s.cpu_tx_en = 0xff;
        port_tx_ctrl_msk.s.stop_ge_rx = 1;
        port_tx_ctrl_msk.s.stop_pon_rx = 1;
        rt = __bm_port_tx_ctrl_set(port_tx_ctrl_msk, &port_tx_ctrl);
        if (rt != CS_E_OK) 
            return rt;

        REG_FIELD_WRITE(BM_FLUSH_CTRL,ma_flush_en,1);
        if (__bm_flush_done_check(port) != CS_E_OK) 
        {
            AAL_MIN_LOG("MA flush isn't done!!\n");
        }      
        REG_FIELD_WRITE(BM_FLUSH_CTRL,ma_flush_en,0);
        
        port_tx_ctrl.cpu_tx_en  = 0xff;
        port_tx_ctrl.stop_ge_rx = 0;
        port_tx_ctrl.stop_pon_rx = 0;
        port_tx_ctrl_msk.s.cpu_tx_en = 0xff;
        port_tx_ctrl_msk.s.stop_ge_rx = 1;
        port_tx_ctrl_msk.s.stop_pon_rx = 1;
        rt = __bm_port_tx_ctrl_set(port_tx_ctrl_msk, &port_tx_ctrl);
        if (rt != CS_E_OK) 
            return rt;

        break;
    case AAL_PORT_ID_PON:
        port_tx_ctrl.pon_tx_en  = 0;
        port_tx_ctrl.stop_ge_rx = 1;
        port_tx_ctrl.stop_cpu_rx = 1;
        port_tx_ctrl_msk.s.pon_tx_en = 0xff;
        port_tx_ctrl_msk.s.stop_ge_rx = 1;
        port_tx_ctrl_msk.s.stop_cpu_rx = 1;
        rt = __bm_port_tx_ctrl_set(port_tx_ctrl_msk, &port_tx_ctrl);
        if (rt != CS_E_OK) 
            return rt;
        
        rt = __bm_ext_cfg_get(&ext_cfg);
        if (rt != CS_E_OK) 
            return rt;
  
        us_abr = ext_cfg.us_abr_en;
        win_chck = ext_cfg.win_chk_en_rsvd;

        ext_cfg_msk.s.us_abr_en = 1;
        ext_cfg_msk.s.win_chk_en_rsvd = 1;
        ext_cfg.us_abr_en = 0;
        ext_cfg.win_chk_en_rsvd = 0;
        rt = __bm_ext_cfg_set(ext_cfg_msk, &ext_cfg);
        if (rt != CS_E_OK) 
            return rt;

        REG_FIELD_WRITE(BM_FLUSH_CTRL,pon_flush_en,1);
        if (__bm_flush_done_check(port) != CS_E_OK) 
        {
            AAL_MIN_LOG("PON flush isn't done!!");
        }
        REG_FIELD_WRITE(BM_FLUSH_CTRL,pon_flush_en,0);

        ext_cfg_msk.s.us_abr_en = 1;
        ext_cfg_msk.s.win_chk_en_rsvd = 1;
        ext_cfg.us_abr_en = us_abr;
        ext_cfg.win_chk_en_rsvd = win_chck;
        rt = __bm_ext_cfg_set(ext_cfg_msk, &ext_cfg);
        if (rt != CS_E_OK) 
            return rt;
            
        port_tx_ctrl.pon_tx_en  = 0xff;
        port_tx_ctrl.stop_ge_rx = 0;
        port_tx_ctrl.stop_cpu_rx = 0;
        port_tx_ctrl_msk.s.pon_tx_en = 0xff;
        port_tx_ctrl_msk.s.stop_ge_rx = 1;
        port_tx_ctrl_msk.s.stop_cpu_rx = 1;
        rt = __bm_port_tx_ctrl_set(port_tx_ctrl_msk, &port_tx_ctrl);
        if (rt != CS_E_OK) 
            return rt;
       
        break;
    case AAL_PORT_ID_GE:
        port_tx_ctrl.ge_tx_en  = 0;
        port_tx_ctrl.stop_pon_rx = 1;
        port_tx_ctrl.stop_cpu_rx = 1;
        port_tx_ctrl_msk.s.pon_tx_en = 0xff;
        port_tx_ctrl_msk.s.stop_pon_rx = 1;
        port_tx_ctrl_msk.s.stop_cpu_rx = 1;
        rt = __bm_port_tx_ctrl_set(port_tx_ctrl_msk, &port_tx_ctrl);
        if (rt != CS_E_OK) 
            return rt;
     
        REG_FIELD_WRITE(BM_FLUSH_CTRL,ge_flush_en,1);
        if (__bm_flush_done_check(port) != CS_E_OK) 
        {
            AAL_MIN_LOG("GE flush isn't done!!");
        }
        REG_FIELD_WRITE(BM_FLUSH_CTRL,ge_flush_en,0);
        
        port_tx_ctrl.ge_tx_en  = 0xff;
        port_tx_ctrl.stop_pon_rx = 0;
        port_tx_ctrl.stop_cpu_rx = 0;
        port_tx_ctrl_msk.s.pon_tx_en = 0xff;
        port_tx_ctrl_msk.s.stop_pon_rx = 1;
        port_tx_ctrl_msk.s.stop_cpu_rx = 1;
        rt = __bm_port_tx_ctrl_set(port_tx_ctrl_msk, &port_tx_ctrl);
        if (rt != CS_E_OK) 
            return rt;
       
        break;
    default:
        return CS_E_PARAM;
    }
    
    return rt;
}

cs_status __bm_que_flush_enable(cs_aal_port_id_t port, cs_uint8 cos, cs_boolean enable)
{
    BM_Q_FLUSH_EN_t data;

    REG_READ(BM_Q_FLUSH_EN, data.wrd);

    switch (port) 
    {
    case AAL_PORT_ID_GE:
        if (cos > 4)
            return CS_E_PARAM;
      
        if (enable)
            data.bf.ds_ptr_updt |= (1 << cos);
        else
            data.bf.ds_ptr_updt &= ~(1 << cos);
        
        REG_FIELD_WRITE(BM_Q_FLUSH_EN,ds_ptr_updt,data.bf.ds_ptr_updt);
        break;
    case AAL_PORT_ID_PON:
        if (cos > 4)
            return CS_E_PARAM;
       
        if (enable)
            data.bf.us_ptr_updt |= (1 << cos);
        else
            data.bf.us_ptr_updt &= ~(1 << cos);
        
        REG_FIELD_WRITE(BM_Q_FLUSH_EN,us_ptr_updt,data.bf.us_ptr_updt);
        break;
    case AAL_PORT_ID_CPU:
        if (cos > 7)
            return CS_E_PARAM;
    
        if (enable)
            data.bf.ma_ptr_updt = 1;
        else
            data.bf.ma_ptr_updt = 0;
     
        REG_FIELD_WRITE(BM_Q_FLUSH_EN,ma_ptr_updt,data.bf.ma_ptr_updt);
        break;
    default:
        return CS_E_PARAM;
    }

    return CS_E_OK;
}

void  __bm_re_dir_flush_bm(void)
{
    GLOBAL_CTRL_t glb_ctrl;
    __bm_port_tx_ctrl_t bm_tx_ctrl;
    __bm_port_tx_ctrl_msk_t port_tx_msk;
    cs_uint8 cos;

    port_tx_msk.u32 = 0;

    __bm_re_dir_enable(AAL_PORT_ID_GE, TRUE);
    __bm_re_dir_enable(AAL_PORT_ID_PON, TRUE);

    REG_READ(GLOBAL_CTRL, glb_ctrl.wrd);
    
    if (glb_ctrl.bf.legacy_mode == 0)
    {
        __bm_port_tx_ctrl_get(&bm_tx_ctrl);
        bm_tx_ctrl.ge_tx_en &= 0xe0;
        bm_tx_ctrl.pon_tx_en &= 0xe0;
        port_tx_msk.s.ge_tx_en = 0xff;
        port_tx_msk.s.pon_tx_en = 0xff;
        __bm_port_tx_ctrl_set(port_tx_msk, &bm_tx_ctrl);
        
        for (cos = 0;cos < 5;cos++) 
        {
            __bm_que_flush_enable(AAL_PORT_ID_PON, cos, 1);
            __bm_que_flush_enable(AAL_PORT_ID_GE, cos, 1);
        }
        
        for (cos = 0;cos < 5;cos++) 
        {
            __bm_que_flush_enable(AAL_PORT_ID_PON, cos, 0);
            __bm_que_flush_enable(AAL_PORT_ID_GE, cos, 0);
        }
        
        bm_tx_ctrl.ge_tx_en |= 0x1f;
        bm_tx_ctrl.pon_tx_en |= 0x1f;
        port_tx_msk.s.ge_tx_en = 0xff;
        port_tx_msk.s.pon_tx_en = 0xff;
        __bm_port_tx_ctrl_set(port_tx_msk, &bm_tx_ctrl);
    } 
    else 
    {
        __bm_flush(AAL_PORT_ID_PON);
        __bm_flush(AAL_PORT_ID_GE);
    }
}

cs_status __bm_reset(void)
{
    cs_status rt = CS_E_OK;
    cs_uint16 counter = 0;
    GLOBAL_INIT_STS_t glb_init_sts;
    cs_aal_redirect_cfg_t  ge_old_redir, pon_old_redir, new_redir;
    cs_aal_redirect_cfg_msk_t    cfg_msk;

    memset(&ge_old_redir, 0, sizeof(ge_old_redir));
    memset(&pon_old_redir, 0, sizeof(pon_old_redir));
    memset(&new_redir, 0, sizeof(new_redir));
    
    aal_redirect_cfg_get(AAL_PORT_ID_GE, &new_redir);
    ge_old_redir = new_redir;
    
    cfg_msk.u32 = 0;
    cfg_msk.s.dpid_redirect0_en = 1;
    cfg_msk.s.redirect0_fwd_dpid = 1;
    cfg_msk.s.redirect0_original_dpid = 1;
    new_redir.redirect0_fwd_dpid.dst_op = AAL_SPEC_DST_DROP;        
    new_redir.redirect0_original_dpid.dst_op = AAL_SPEC_DST_PORT;
    new_redir.redirect0_original_dpid.dpid= AAL_PORT_ID_PON;
    new_redir.dpid_redirect0_en = 1;        

    rt = aal_redirect_cfg_set(AAL_PORT_ID_GE, cfg_msk, &new_redir);
    if (rt != CS_E_OK)
    {
        goto __end;
    }

    aal_redirect_cfg_get(AAL_PORT_ID_GE, &new_redir);
    pon_old_redir = new_redir;

    cfg_msk.u32 = 0;
    cfg_msk.s.dpid_redirect0_en = 1;
    cfg_msk.s.redirect0_fwd_dpid = 1;
    cfg_msk.s.redirect0_original_dpid = 1;
    new_redir.redirect0_fwd_dpid.dst_op = AAL_SPEC_DST_DROP;        
    new_redir.redirect0_original_dpid.dst_op = AAL_SPEC_DST_PORT;
    new_redir.redirect0_original_dpid.dpid= AAL_PORT_ID_GE;
    new_redir.dpid_redirect0_en = 1;        

    rt = aal_redirect_cfg_set(AAL_PORT_ID_PON, cfg_msk, &new_redir);

    if (rt != CS_E_OK)
    {
        goto __end;
    }

    //__bm_flush(AAL_PORT_ID_PON);
    
    //__bm_flush(AAL_PORT_ID_GE);

    REG_FIELD_WRITE(BM_PORT_TX_CTRL, ge_tx_en, 0);
    REG_FIELD_WRITE(BM_PORT_TX_CTRL, pon_tx_en, 0);
    REG_FIELD_WRITE(BM_PORT_TX_CTRL, cpu_tx_en, 0);
    REG_FIELD_WRITE(BM_PORT_TX_CTRL, stop_ge_rx, 1);
    REG_FIELD_WRITE(BM_PORT_TX_CTRL, stop_pon_rx, 1);
    REG_FIELD_WRITE(BM_PORT_TX_CTRL, stop_cpu_rx, 1);

    REG_FIELD_WRITE(GLOBAL_RST_CTRL, bm_rst_n, 0);
    REG_FIELD_WRITE(GLOBAL_RST_CTRL, bm_rst_n, 1);

    while (1) 
    {
        REG_READ(GLOBAL_INIT_STS, glb_init_sts.wrd);
        if (glb_init_sts.bf.bm_init_done || counter == __BM_INIT_DONE_TIME) 
            break;
       
        counter++;
    }

    if (counter == __BM_INIT_DONE_TIME) 
    {
        goto __end;
    }

    REG_FIELD_WRITE(BM_PORT_TX_CTRL, ge_tx_en, 0xff);
    REG_FIELD_WRITE(BM_PORT_TX_CTRL, pon_tx_en, 0xff);
    REG_FIELD_WRITE(BM_PORT_TX_CTRL, cpu_tx_en, 0xff);
    REG_FIELD_WRITE(BM_PORT_TX_CTRL, stop_ge_rx, 0);
    REG_FIELD_WRITE(BM_PORT_TX_CTRL, stop_pon_rx, 0);
    REG_FIELD_WRITE(BM_PORT_TX_CTRL, stop_cpu_rx, 0);

__end:
    cfg_msk.u32 = 0;
    cfg_msk.s.dpid_redirect0_en = 1;
    cfg_msk.s.redirect0_fwd_dpid = 1;
    cfg_msk.s.redirect0_original_dpid = 1;

    aal_redirect_cfg_set(AAL_PORT_ID_GE, cfg_msk, &ge_old_redir);
    aal_redirect_cfg_set(AAL_PORT_ID_PON, cfg_msk, &pon_old_redir);

    return rt;
    
}

cs_status __bm_us_que_size_get(cs_aal_bm_q_size_t *cfg)
{
    BM_US_Q0_START_t us_q0_start;
    BM_US_Q0_END_t us_q0_end;
    BM_US_Q1_START_t us_q1_start;
    BM_US_Q1_END_t us_q1_end;
    BM_US_Q2_START_t us_q2_start;
    BM_US_Q2_END_t us_q2_end;
    BM_US_Q3_START_t us_q3_start;
    BM_US_Q3_END_t us_q3_end;

    if (!cfg)
        return CS_E_PARAM;

    us_q0_end.wrd = __reg_value_get(BM_US_Q0_END);
    us_q0_start.wrd = __reg_value_get(BM_US_Q0_START);
    us_q1_end.wrd = __reg_value_get(BM_US_Q1_END);
    us_q1_start.wrd = __reg_value_get(BM_US_Q1_START);
    us_q2_end.wrd = __reg_value_get(BM_US_Q2_END);
    us_q2_start.wrd = __reg_value_get(BM_US_Q2_START);
    us_q3_end.wrd = __reg_value_get(BM_US_Q3_END);
    us_q3_start.wrd = __reg_value_get(BM_US_Q3_START);

    cfg->qx_size[0] = (us_q0_end.bf.packet - us_q0_start.bf.packet + 1) * __BM_NUM_KB;
    cfg->qx_size[1] = (us_q1_end.bf.packet - us_q1_start.bf.packet + 1) * __BM_NUM_KB;
    cfg->qx_size[2] = (us_q2_end.bf.packet - us_q2_start.bf.packet + 1) * __BM_NUM_KB;
    cfg->qx_size[3] = (us_q3_end.bf.packet - us_q3_start.bf.packet + 1) * __BM_NUM_KB;

    if ((cfg->qx_size[3] > 128) && (cfg->qx_size[2] > 128) 
        && (cfg->qx_size[1] > 128) && (cfg->qx_size[0] > 128))
    {
        cfg->qx_size[0] = 128;
        cfg->qx_size[1] = 128;
        cfg->qx_size[2] = 128;
        cfg->qx_size[3] = 128;
    }
    
    return CS_E_OK;
}

cs_status __bm_ds_que_size_get(cs_aal_bm_q_size_t *cfg)
{
    BM_DS_Q0_START_t ds_q0_start;
    BM_DS_Q0_END_t ds_q0_end;
    BM_DS_Q1_START_t ds_q1_start;
    BM_DS_Q1_END_t ds_q1_end;
    BM_DS_Q2_START_t ds_q2_start;
    BM_DS_Q2_END_t ds_q2_end;
    BM_DS_Q3_START_t ds_q3_start;
    BM_DS_Q3_END_t ds_q3_end;

    if (!cfg) 
        return CS_E_PARAM;

    ds_q0_end.wrd = __reg_value_get(BM_DS_Q0_END);
    ds_q0_start.wrd = __reg_value_get(BM_DS_Q0_START);
    ds_q1_end.wrd = __reg_value_get(BM_DS_Q1_END);
    ds_q1_start.wrd = __reg_value_get(BM_DS_Q1_START);
    ds_q2_end.wrd = __reg_value_get(BM_DS_Q2_END);
    ds_q2_start.wrd = __reg_value_get(BM_DS_Q2_START);
    ds_q3_end.wrd = __reg_value_get(BM_DS_Q3_END);
    ds_q3_start.wrd = __reg_value_get(BM_DS_Q3_START);

    cfg->qx_size[0] = (ds_q0_end.bf.packet - ds_q0_start.bf.packet + 1) * __BM_NUM_KB;
    cfg->qx_size[1] = (ds_q1_end.bf.packet - ds_q1_start.bf.packet + 1) * __BM_NUM_KB;
    cfg->qx_size[2] = (ds_q2_end.bf.packet - ds_q2_start.bf.packet + 1) * __BM_NUM_KB;
    cfg->qx_size[3] = (ds_q3_end.bf.packet - ds_q3_start.bf.packet + 1) * __BM_NUM_KB;
    if ((cfg->qx_size[0] > 128) && (cfg->qx_size[1] > 128) 
        && (cfg->qx_size[2] > 128) && (cfg->qx_size[3] > 128))
    {
        cfg->qx_size[0] = 128;
        cfg->qx_size[1] = 128;
        cfg->qx_size[2] = 128;
        cfg->qx_size[3] = 128;
    }
    
    return CS_E_OK;
}

cs_status __bm_ds_que_size_set(cs_aal_bm_q_size_t *cfg)
{
    BM_DS_Q0_START_t ds_q0_start;
    BM_DS_Q0_END_t ds_q0_end;
    BM_DS_Q1_START_t ds_q1_start;
    BM_DS_Q1_END_t ds_q1_end;
    BM_DS_Q2_START_t ds_q2_start;
    BM_DS_Q2_END_t ds_q2_end;
    BM_DS_Q3_START_t ds_q3_start;
    BM_DS_Q3_END_t ds_q3_end;
    BM_DS_Q4_END_t ds_q4_end;
    BM_LYNXD_TMU_STATUS_t tmu_sts;
    cs_uint32 total_buf_size = 0;
    cs_uint8 cos;
    cs_uint16 counter;
    cs_aal_bm_q_size_t elder_size;
    cs_status rt = CS_E_OK;

    if (!cfg)
        return CS_E_PARAM;

    for (cos = 0; cos < __BM_MAX_QUE_NUM; cos++) 
    {
        total_buf_size += cfg->qx_size[cos];
    }
    
    if (total_buf_size > __BM_MAX_BUF_SIZE) 
        return CS_E_PARAM;
    

    rt = __bm_ds_que_size_get(&elder_size);
    if (CS_E_OK != rt) 
        return rt;
    
#if 1  
    if (memcmp(cfg, &elder_size, sizeof(cs_aal_bm_q_size_t)) == 0) {
        return CS_E_OK;
    }
#endif
    ds_q4_end.wrd = __reg_value_get(BM_DS_Q4_END);
    __bm_packet_header_address_get(cfg->qx_size[3], ds_q4_end.bf.header, (cs_uint32*)&(ds_q3_start.wrd), (cs_uint32*)&(ds_q3_end.wrd));
    __bm_packet_header_address_get(cfg->qx_size[2], ds_q3_end.bf.header, (cs_uint32*)&(ds_q2_start.wrd), (cs_uint32*)&(ds_q2_end.wrd));
    __bm_packet_header_address_get(cfg->qx_size[1], ds_q2_end.bf.header, (cs_uint32*)&(ds_q1_start.wrd), (cs_uint32*)&(ds_q1_end.wrd));
    __bm_packet_header_address_get(cfg->qx_size[0], ds_q1_end.bf.header, (cs_uint32*)&(ds_q0_start.wrd), (cs_uint32*)&(ds_q0_end.wrd));

    //__bm_re_dir_enable(AAL_PORT_ID_PON, TRUE);
#if 0
for (cos = 0;cos < __BM_MAX_QUE_NUM;cos++) 
    {
        for (counter = 0; counter < 10000;counter++) 
        {
            bm_counter  = __reg_value_get(BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS0 - cos);
            if (!bm_counter) 
                break;
        }
        if (counter == 10000) 
        {
            cs_printf("\nthere will be some downstream loss in queue %d \n", cos);
        }
    }
#endif
    for (counter = 0; counter < 10;counter++) 
    {
        tmu_sts.wrd = __reg_value_get(BM_LYNXD_TMU_STATUS);
        if ((tmu_sts.bf.status&0xff) == 0) 
            break;
        //HAL_DELAY_US(50000); // delay 50ms
        cs_thread_delay(50);
    }
    if(counter == 10){
       // cs_printf("\n TMU STATUS = 0x%x \n", tmu_sts.bf.status);
        return CS_E_ERROR;
    }
    
    for (cos = 0;cos < 4;cos++) 
    {
        __bm_que_flush_enable(AAL_PORT_ID_GE, cos, TRUE);
    }

    REG_WRITE(BM_DS_Q0_START, ds_q0_start.wrd);
    REG_WRITE(BM_DS_Q1_START, ds_q1_start.wrd);
    REG_WRITE(BM_DS_Q2_START, ds_q2_start.wrd);
    REG_WRITE(BM_DS_Q3_START, ds_q3_start.wrd);
    REG_WRITE(BM_DS_Q3_END, ds_q3_end.wrd);
    REG_WRITE(BM_DS_Q2_END, ds_q2_end.wrd);
    REG_WRITE(BM_DS_Q1_END, ds_q1_end.wrd);
    REG_WRITE(BM_DS_Q0_END, ds_q0_end.wrd);

    for (cos = 0;cos < 4;cos++)
    {
        __bm_que_flush_enable(AAL_PORT_ID_GE, cos, FALSE);
    }

    //__bm_re_dir_enable(AAL_PORT_ID_PON, FALSE);
    return CS_E_OK;
}

cs_status __bm_us_que_size_set(cs_aal_bm_q_size_t *cfg)
{
    BM_US_Q0_START_t us_q0_start;
    BM_US_Q0_END_t us_q0_end;
    BM_US_Q1_START_t us_q1_start;
    BM_US_Q1_END_t us_q1_end;
    BM_US_Q2_START_t us_q2_start;
    BM_US_Q2_END_t us_q2_end;
    BM_US_Q3_START_t us_q3_start;
    BM_US_Q3_END_t us_q3_end;
    BM_US_Q4_END_t us_q4_end;
    BM_LYNXD_TMU_STATUS_t tmu_sts;
    cs_uint32  total_buf_size = 0;
    cs_uint8 cos;
    cs_uint16 counter;
    cs_aal_bm_q_size_t elder_size;
    cs_status rt = CS_E_OK;

    if (!cfg)
        return CS_E_PARAM;

    for (cos = 0; cos < __BM_MAX_QUE_NUM; cos++) 
    {
        total_buf_size += cfg->qx_size[cos];
    }
    
    if (total_buf_size > __BM_MAX_BUF_SIZE)
        return CS_E_PARAM;

    rt = __bm_us_que_size_get(&elder_size);
    if (CS_E_OK != rt) 
        return rt;

#if 1   
    if (memcmp(cfg, &elder_size, sizeof(cs_aal_bm_q_size_t)) == 0) {
        return CS_E_OK;
    }
#endif
    us_q4_end.wrd = __reg_value_get(BM_US_Q4_END);
    __bm_packet_header_address_get(cfg->qx_size[3], us_q4_end.bf.header, (cs_uint32*)&(us_q3_start.wrd), (cs_uint32*)&(us_q3_end.wrd));
    __bm_packet_header_address_get(cfg->qx_size[2], us_q3_end.bf.header, (cs_uint32*)&(us_q2_start.wrd), (cs_uint32*)&(us_q2_end.wrd));
    __bm_packet_header_address_get(cfg->qx_size[1], us_q2_end.bf.header, (cs_uint32*)&(us_q1_start.wrd), (cs_uint32*)&(us_q1_end.wrd));
    __bm_packet_header_address_get(cfg->qx_size[0], us_q1_end.bf.header, (cs_uint32*)&(us_q0_start.wrd), (cs_uint32*)&(us_q0_end.wrd));

    //__bm_re_dir_enable(AAL_PORT_ID_GE, TRUE);
#if 0
    for (cos = 0;cos < 4;cos++) 
    {
        for (counter = 0; counter < 10000;counter++) 
        {
            bm_counter  = __reg_value_get(BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS0 - cos);
            if (!bm_counter) 
                break;           
        }
        
        if (counter == 10000) 
        {
            cs_printf("\nthere will be some upstream loss in queue %d \n", cos);
        }
    }
#endif
    for (counter = 0; counter < 10;counter++) 
    {
        tmu_sts.wrd = __reg_value_get(BM_LYNXD_TMU_STATUS);
        if ((tmu_sts.bf.status&0xff00) == 0) 
            break;       
      //  HAL_DELAY_US(50000); // delay 50ms
        cs_thread_delay(50);
    }    
    if(counter == 10){
        //cs_printf("\n TMU STATUS = 0x%x \n", tmu_sts.bf.status);
        return CS_E_ERROR;
    }

    for (cos = 0;cos < 4;cos++) 
    {
        __bm_que_flush_enable(AAL_PORT_ID_PON, cos, TRUE);
    }

    REG_WRITE(BM_US_Q0_START, us_q0_start.wrd);
    REG_WRITE(BM_US_Q1_START, us_q1_start.wrd);
    REG_WRITE(BM_US_Q2_START, us_q2_start.wrd);
    REG_WRITE(BM_US_Q3_START, us_q3_start.wrd);
    REG_WRITE(BM_US_Q3_END, us_q3_end.wrd);
    REG_WRITE(BM_US_Q2_END, us_q2_end.wrd);
    REG_WRITE(BM_US_Q1_END, us_q1_end.wrd);
    REG_WRITE(BM_US_Q0_END, us_q0_end.wrd);

    for (cos = 0;cos < 4;cos++) 
    {
        __bm_que_flush_enable(AAL_PORT_ID_PON, cos, FALSE);
    }

    //__bm_re_dir_enable(AAL_PORT_ID_GE, FALSE);
    
    return CS_E_OK;
}


cs_status __bm_port_que_weight_ratio_set(cs_aal_port_id_t port, cs_aal_bm_queue_msk_t mask, cs_aal_bm_queue_val_t *cfg)
{
    if(!cfg || mask.u32 == 0)
        return CS_E_PARAM;
    
    switch (port) {
    case AAL_PORT_ID_GE:
        if(mask.s.queue & 0x1)
            REG_FIELD_WRITE(BM_GE_WEIGHT_RATIO_COS3_0,cos0,cfg->val[0]);
        if(mask.s.queue & (0x1 << 1))
            REG_FIELD_WRITE(BM_GE_WEIGHT_RATIO_COS3_0,cos1,cfg->val[1]);
        if(mask.s.queue & (0x1 << 2))
            REG_FIELD_WRITE(BM_GE_WEIGHT_RATIO_COS3_0,cos2,cfg->val[2]);
        if(mask.s.queue & (0x1 << 3))
            REG_FIELD_WRITE(BM_GE_WEIGHT_RATIO_COS3_0,cos3,cfg->val[3]);
        if(mask.s.queue & (0x1 << 4))
            REG_FIELD_WRITE(BM_GE_WEIGHT_RATIO_COS7_4,cos4,cfg->val[4]);
        if(mask.s.queue & (0x1 << 5))
            REG_FIELD_WRITE(BM_GE_WEIGHT_RATIO_COS7_4,cos5,cfg->val[5]);
        if(mask.s.queue & (0x1 << 6))
            REG_FIELD_WRITE(BM_GE_WEIGHT_RATIO_COS7_4,cos6,cfg->val[6]);
        if(mask.s.queue & (0x1 << 7))
            REG_FIELD_WRITE(BM_GE_WEIGHT_RATIO_COS7_4,cos7,cfg->val[7]);
        
        break;
    case AAL_PORT_ID_PON:
        if(mask.s.queue & 0x1)
            REG_FIELD_WRITE(BM_PON_WEIGHT_RATIO_COS3_0,cos0,cfg->val[0]);
        if(mask.s.queue & (0x1 << 1))
            REG_FIELD_WRITE(BM_PON_WEIGHT_RATIO_COS3_0,cos1,cfg->val[1]);
        if(mask.s.queue & (0x1 << 2))
            REG_FIELD_WRITE(BM_PON_WEIGHT_RATIO_COS3_0,cos2,cfg->val[2]);
        if(mask.s.queue & (0x1 << 3))
            REG_FIELD_WRITE(BM_PON_WEIGHT_RATIO_COS3_0,cos3,cfg->val[3]);
        if(mask.s.queue & (0x1 << 4))
            REG_FIELD_WRITE(BM_PON_WEIGHT_RATIO_COS7_4,cos4,cfg->val[4]);
        if(mask.s.queue & (0x1 << 5))
            REG_FIELD_WRITE(BM_PON_WEIGHT_RATIO_COS7_4,cos5,cfg->val[5]);
        if(mask.s.queue & (0x1 << 6))
            REG_FIELD_WRITE(BM_PON_WEIGHT_RATIO_COS7_4,cos6,cfg->val[6]);
        if(mask.s.queue & (0x1 << 7))
            REG_FIELD_WRITE(BM_PON_WEIGHT_RATIO_COS7_4,cos7,cfg->val[7]);

        break;
    default:
        return CS_E_PARAM;
    }
    return CS_E_OK;
}

cs_status __bm_port_que_weight_ratio_get(cs_aal_port_id_t port, cs_aal_bm_queue_val_t *cfg)
{
    BM_GE_WEIGHT_RATIO_COS7_4_t ge_cos7_4;
    BM_GE_WEIGHT_RATIO_COS3_0_t ge_cos3_0;
    BM_PON_WEIGHT_RATIO_COS7_4_t pon_cos7_4;
    BM_PON_WEIGHT_RATIO_COS3_0_t pon_cos3_0;

    if (!cfg)
        return CS_E_PARAM;
 
    switch (port) {
    case AAL_PORT_ID_GE:
        REG_READ(BM_GE_WEIGHT_RATIO_COS3_0,ge_cos3_0.wrd);
        REG_READ(BM_GE_WEIGHT_RATIO_COS7_4,ge_cos7_4.wrd);
        cfg->val[0] = ge_cos3_0.bf.cos0;
        cfg->val[1] = ge_cos3_0.bf.cos1;
        cfg->val[2] = ge_cos3_0.bf.cos2;
        cfg->val[3] = ge_cos3_0.bf.cos3;
        cfg->val[4] = ge_cos7_4.bf.cos4;
        cfg->val[5] = ge_cos7_4.bf.cos5;
        cfg->val[6] = ge_cos7_4.bf.cos6;
        cfg->val[7] = ge_cos7_4.bf.cos7;
        break;
    case AAL_PORT_ID_PON:
        REG_READ(BM_PON_WEIGHT_RATIO_COS3_0,pon_cos3_0.wrd);
        REG_READ(BM_PON_WEIGHT_RATIO_COS7_4,pon_cos7_4.wrd);
        cfg->val[0] = pon_cos3_0.bf.cos0;
        cfg->val[1] = pon_cos3_0.bf.cos1;
        cfg->val[2] = pon_cos3_0.bf.cos2;
        cfg->val[3] = pon_cos3_0.bf.cos3;
        cfg->val[4] = pon_cos7_4.bf.cos4;
        cfg->val[5] = pon_cos7_4.bf.cos5;
        cfg->val[6] = pon_cos7_4.bf.cos6;
        cfg->val[7] = pon_cos7_4.bf.cos7;
        break;
    default:
        return CS_E_PARAM;
    }
    return CS_E_OK;
}

cs_status __bm_dp_cfg_stage_set(cs_uint8 level, cs_uint16 threshold)
{
    BM_DP_CFG_STAGE2_1_t lev_2_1, lev_2_1_msk;
    BM_DP_CFG_STAGE4_3_t lev_4_3, lev_4_3_msk;
    BM_DP_CFG_STAGE6_5_t lev_6_5, lev_6_5_msk;
    BM_DP_CFG_STAGE8_7_t lev_8_7, lev_8_7_msk;

    if (threshold > 0x3600) {
        return CS_E_PARAM;
    }
    
    lev_2_1_msk.wrd = 0;
    lev_4_3_msk.wrd = 0;
    lev_6_5_msk.wrd = 0;
    lev_8_7_msk.wrd = 0;
    
    switch (level) {
    case 1:
        lev_2_1_msk.bf.dp_thrsld_1 = 0x3fff;
        lev_2_1.bf.dp_thrsld_1 = threshold;
        break;
    case 2:
        lev_2_1_msk.bf.dp_thrsld_2 = 0x3fff;
        lev_2_1.bf.dp_thrsld_2 = threshold;
        break;
    case 3:
        lev_4_3_msk.bf.dp_thrsld_3 = 0x3fff;
        lev_4_3.bf.dp_thrsld_3 = threshold;
        break;
    case 4:
        lev_4_3_msk.bf.dp_thrsld_4 = 0x3fff;
        lev_4_3.bf.dp_thrsld_4 = threshold;
        break;
    case 5:
        lev_6_5_msk.bf.dp_thrsld_5 = 0x3fff;
        lev_6_5.bf.dp_thrsld_5 = threshold;
        break;
    case 6:
        lev_6_5_msk.bf.dp_thrsld_6 = 0x3fff;
        lev_6_5.bf.dp_thrsld_6 = threshold;
        break;
    case 7:
        lev_8_7_msk.bf.dp_thrsld_7 = 0x3fff;
        lev_8_7.bf.dp_thrsld_7 = threshold;
        break;
    case 8:
        lev_8_7_msk.bf.dp_thrsld_8 = 0x3fff;
        lev_8_7.bf.dp_thrsld_8 = threshold;
        break;
    default:
        return CS_E_PARAM;
    }
    
    REG_MASK_WRITE(BM_DP_CFG_STAGE2_1, lev_2_1_msk.wrd, lev_2_1.wrd);
    REG_MASK_WRITE(BM_DP_CFG_STAGE4_3, lev_4_3_msk.wrd, lev_4_3.wrd);
    REG_MASK_WRITE(BM_DP_CFG_STAGE6_5, lev_6_5_msk.wrd, lev_6_5.wrd);
    REG_MASK_WRITE(BM_DP_CFG_STAGE8_7, lev_8_7_msk.wrd, lev_8_7.wrd);
    
    return CS_E_OK;
}

cs_status __bm_dp_cfg_stage_get(cs_uint8 level, cs_uint16 *threshold)
{
    BM_DP_CFG_STAGE2_1_t lev_2_1;
    BM_DP_CFG_STAGE4_3_t lev_4_3;
    BM_DP_CFG_STAGE6_5_t lev_6_5;
    BM_DP_CFG_STAGE8_7_t lev_8_7;

    if (!threshold) 
        return CS_E_PARAM;

    REG_READ(BM_DP_CFG_STAGE2_1,lev_2_1.wrd);
    REG_READ(BM_DP_CFG_STAGE4_3,lev_4_3.wrd);
    REG_READ(BM_DP_CFG_STAGE6_5,lev_6_5.wrd);
    REG_READ(BM_DP_CFG_STAGE8_7,lev_8_7.wrd);

    switch (level) {
    case 1:
        *threshold = lev_2_1.bf.dp_thrsld_1;
        break;
    case 2:
        *threshold = lev_2_1.bf.dp_thrsld_2;
        break;
    case 3:
        *threshold = lev_4_3.bf.dp_thrsld_3;
        break;
    case 4:
        *threshold = lev_4_3.bf.dp_thrsld_4;
        break;
    case 5:
        *threshold = lev_6_5.bf.dp_thrsld_5;
        break;
    case 6:
        *threshold = lev_6_5.bf.dp_thrsld_6;
        break;
    case 7:
        *threshold = lev_8_7.bf.dp_thrsld_7;
        break;
    case 8:
        *threshold = lev_8_7.bf.dp_thrsld_8;
        break;
    default:
        return CS_E_PARAM;
    }

    return CS_E_OK;
}

cs_status aal_bm_ma_dpid_cos_set(cs_uint8 dpid, cs_uint8 ing_cos, cs_uint8 map_que)
{
    BM_MA_DPID4_COS_CFG_t dpid4_cos_cfg, dpid4_cos_cfg_msk;
    BM_MA_DPID5_COS_CFG_t dpid5_cos_cfg, dpid5_cos_cfg_msk;
    BM_MA_DPID6_COS_CFG_t dpid6_cos_cfg, dpid6_cos_cfg_msk;
    BM_MA_COS_DPID_CFG_t cos_dpid_cfg, cos_dpid_cfg_mak;
    
    dpid4_cos_cfg_msk.wrd = 0;
    dpid5_cos_cfg_msk.wrd = 0;
    dpid6_cos_cfg_msk.wrd = 0;
    cos_dpid_cfg_mak.wrd = 0;

    if (map_que > 7) {
        return CS_E_PARAM;
    }
    
    switch (dpid) {
    case 4:
        switch (ing_cos) {
        case 0:
            dpid4_cos_cfg_msk.bf.dpid_cos0 = 7;
            dpid4_cos_cfg.bf.dpid_cos0 = map_que;
            break;
        case 1:
            dpid4_cos_cfg_msk.bf.dpid_cos1 = 7;
            dpid4_cos_cfg.bf.dpid_cos1 = map_que;
            break;
        case 2:
            dpid4_cos_cfg_msk.bf.dpid_cos2 = 7;
            dpid4_cos_cfg.bf.dpid_cos2 = map_que;
            break;
        case 3:
            dpid4_cos_cfg_msk.bf.dpid_cos3 = 7;
            dpid4_cos_cfg.bf.dpid_cos3 = map_que;
            break;
        case 4:
            dpid4_cos_cfg_msk.bf.dpid_cos4 = 7;
            dpid4_cos_cfg.bf.dpid_cos4 = map_que;
            break;
        case 5:
            dpid4_cos_cfg_msk.bf.dpid_cos5 = 7;
            dpid4_cos_cfg.bf.dpid_cos5 = map_que;
            break;
        case 6:
            dpid4_cos_cfg_msk.bf.dpid_cos6 = 7;
            dpid4_cos_cfg.bf.dpid_cos6 = map_que;
            break;
        case 7:
            dpid4_cos_cfg_msk.bf.dpid_cos7 = 7;
            dpid4_cos_cfg.bf.dpid_cos7 = map_que;
            break;
        default:
            return CS_E_PARAM;
        }
        break;
    case 5:
        switch (ing_cos) {
        case 0:
            dpid5_cos_cfg_msk.bf.dpid_cos0 = 7;
            dpid5_cos_cfg.bf.dpid_cos0 = map_que;
            break;
        case 1:
            dpid5_cos_cfg_msk.bf.dpid_cos1 = 7;
            dpid5_cos_cfg.bf.dpid_cos1 = map_que;
            break;
        case 2:
            dpid5_cos_cfg_msk.bf.dpid_cos2 = 7;
            dpid5_cos_cfg.bf.dpid_cos2 = map_que;
            break;
        case 3:
            dpid5_cos_cfg_msk.bf.dpid_cos3 = 7;
            dpid5_cos_cfg.bf.dpid_cos3 = map_que;
            break;
        case 4:
            dpid5_cos_cfg_msk.bf.dpid_cos4 = 7;
            dpid5_cos_cfg.bf.dpid_cos4 = map_que;
            break;
        case 5:
            dpid5_cos_cfg_msk.bf.dpid_cos5 = 7;
            dpid5_cos_cfg.bf.dpid_cos5 = map_que;
            break;
        case 6:
            dpid5_cos_cfg_msk.bf.dpid_cos6 = 7;
            dpid5_cos_cfg.bf.dpid_cos6 = map_que;
            break;
        case 7:
            dpid5_cos_cfg_msk.bf.dpid_cos7 = 7;
            dpid5_cos_cfg.bf.dpid_cos7 = map_que;
            break;
        default:
            return CS_E_PARAM;
        }
        break;
    case 6:
        switch (ing_cos) {
        case 0:
            dpid6_cos_cfg_msk.bf.dpid_cos0 = 7;
            dpid6_cos_cfg.bf.dpid_cos0 = map_que;
            break;
        case 1:
            dpid6_cos_cfg_msk.bf.dpid_cos1 = 7;
            dpid6_cos_cfg.bf.dpid_cos1 = map_que;
            break;
        case 2:
            dpid6_cos_cfg_msk.bf.dpid_cos2 = 7;
            dpid6_cos_cfg.bf.dpid_cos2 = map_que;
            break;
        case 3:
            dpid6_cos_cfg_msk.bf.dpid_cos3 = 7;
            dpid6_cos_cfg.bf.dpid_cos3 = map_que;
            break;
        case 4:
            dpid6_cos_cfg_msk.bf.dpid_cos4 = 7;
            dpid6_cos_cfg.bf.dpid_cos4 = map_que;
            break;
        case 5:
            dpid6_cos_cfg_msk.bf.dpid_cos5 = 7;
            dpid6_cos_cfg.bf.dpid_cos5 = map_que;
            break;
        case 6:
            dpid6_cos_cfg_msk.bf.dpid_cos6 = 7;
            dpid6_cos_cfg.bf.dpid_cos6 = map_que;
            break;
        case 7:
            dpid6_cos_cfg_msk.bf.dpid_cos7 = 7;
            dpid6_cos_cfg.bf.dpid_cos7 = map_que;
            break;
        default:
            return CS_E_PARAM;
        }
        break;
    default:
        return CS_E_PARAM;
    }
    
    switch (map_que) {
    case 0:
        cos_dpid_cfg_mak.bf.dpid_cos0  = 7;
        cos_dpid_cfg.bf.dpid_cos0  = dpid;
        break;
    case 1:
        cos_dpid_cfg_mak.bf.dpid_cos1  = 7;
        cos_dpid_cfg.bf.dpid_cos1  = dpid;
        break;
    case 2:
        cos_dpid_cfg_mak.bf.dpid_cos2  = 7;
        cos_dpid_cfg.bf.dpid_cos2  = dpid;
        break;
    case 3:
        cos_dpid_cfg_mak.bf.dpid_cos3  = 7;
        cos_dpid_cfg.bf.dpid_cos3  = dpid;
        break;
    case 4:
        cos_dpid_cfg_mak.bf.dpid_cos4  = 7;
        cos_dpid_cfg.bf.dpid_cos4  = dpid;
        break;
    case 5:
        cos_dpid_cfg_mak.bf.dpid_cos5  = 7;
        cos_dpid_cfg.bf.dpid_cos5  = dpid;
        break;
    case 6:
        cos_dpid_cfg_mak.bf.dpid_cos6  = 7;
        cos_dpid_cfg.bf.dpid_cos6  = dpid;
        break;
    case 7:
        cos_dpid_cfg_mak.bf.dpid_cos7  = 7;
        cos_dpid_cfg.bf.dpid_cos7  = dpid;
        break;
    default:
        return CS_E_PARAM;

    }

    REG_MASK_WRITE(BM_MA_DPID4_COS_CFG, dpid4_cos_cfg_msk.wrd, dpid4_cos_cfg.wrd);
    REG_MASK_WRITE(BM_MA_DPID5_COS_CFG, dpid5_cos_cfg_msk.wrd, dpid5_cos_cfg.wrd);
    REG_MASK_WRITE(BM_MA_DPID6_COS_CFG, dpid6_cos_cfg_msk.wrd, dpid6_cos_cfg.wrd);
    REG_MASK_WRITE(BM_MA_COS_DPID_CFG, cos_dpid_cfg_mak.wrd, cos_dpid_cfg.wrd);

    return CS_E_OK;
}


cs_status __bm_sp_mode_set(cs_aal_port_id_t port)
{
    cs_status rt = CS_E_OK;
    __bm_ext_cfg_t cfg;
    __bm_ext_cfg_msk_t cfg_msk;
    cs_aal_td_cfg_t td_cfg;
    cs_aal_td_cfg_msk_t td_msk;       
    cs_aal_bm_queue_msk_t q_mask;
    cs_aal_bm_queue_val_t q_cfg;
    cs_uint8 que;

    cfg_msk.u = 0;
    td_msk.u32 = 0;

    switch (port) {
    case AAL_PORT_ID_PON:
        cfg.us_dp_en = 0;
        cfg.us_abr_en = 1;
        cfg.edwrr_en_rsvd = 0;
        cfg.dis_win_fit_sp_rsvd = 1;
        cfg.dis_win_fit_nsp_rsvd = 0;
        cfg.abr_us_thrsld = 0x2380;
        cfg.pon_tx_lmt_en = 1;
        cfg.pon_tx_buffer_thrsld = 0x2aaa;        
        cfg_msk.s.us_dp_en = 1;
        cfg_msk.s.us_abr_en = 1;
        cfg_msk.s.edwrr_en_rsvd = 1;
        cfg_msk.s.dis_win_fit_sp_rsvd = 1;
        cfg_msk.s.dis_win_fit_nsp_rsvd = 1;
        cfg_msk.s.abr_us_thrsld = 1;
        cfg_msk.s.pon_tx_lmt_en = 1;
        cfg_msk.s.pon_tx_buffer_thrsld = 1;        
        rt = __bm_ext_cfg_set(cfg_msk, &cfg);
        if (rt != CS_E_OK)
            return rt;

        for (que = 0; que < 8; que++)
            q_cfg.val[que] = 0xf8;
        q_mask.s.queue = 0xff;
        rt |= aal_bm_share_queue_max_length_set(port,q_mask,&q_cfg);
        
        for (que = 0; que < 8; que++)
            td_cfg.pon_drop_thrshld[que] = 0xff;
        td_msk.s.pon_drop_thrshld = 0xff;
        rt |= aal_td_cfg_set(td_msk, &td_cfg);
       
        break;
    case AAL_PORT_ID_GE:
        cfg.ds_abr_en = 1;
        cfg.ds_dp_en = 0;
        cfg.abr_ds_thrsld = 0x1320;
        cfg.ge_tx_lmt_en = 1;
        cfg.ge_tx_buffer_thrsld = 0x1430;        
        cfg_msk.s.ds_abr_en = 1;
        cfg_msk.s.ds_dp_en = 1;
        cfg_msk.s.abr_ds_thrsld = 1;
        cfg_msk.s.ge_tx_lmt_en = 1;
        cfg_msk.s.ge_tx_buffer_thrsld = 1;
        rt = __bm_ext_cfg_set(cfg_msk, &cfg);  
        if (rt != CS_E_OK)
            return rt;

        for (que = 0; que < 8; que++)
            q_cfg.val[que] = 0x80;
        q_mask.s.queue = 0xff;
        rt |= aal_bm_share_queue_max_length_set(port,q_mask,&q_cfg);
        
        break;
    default:
        return CS_E_PARAM;
    }
    
    return rt;
}

cs_status __bm_drr_mode_set(cs_aal_port_id_t port)
{
    cs_status rt = CS_E_OK;
    __bm_ext_cfg_t cfg;
    __bm_ext_cfg_msk_t cfg_msk;
    cs_aal_td_cfg_t td_cfg;
    cs_aal_td_cfg_msk_t td_msk;   
    cs_aal_bm_queue_msk_t q_mask;
    cs_aal_bm_queue_val_t q_cfg;    
    cs_uint8 que;

    cfg_msk.u= 0;
    td_msk.u32 = 0;

    switch (port) {
    case AAL_PORT_ID_PON:
        cfg.us_abr_en = 0;
        cfg.us_dp_en = 1;
        cfg.dis_win_fit_sp_rsvd = 0;
        cfg.dp_us_thrsld = 0x1000;
        cfg.pon_tx_lmt_en = 1;
        cfg.pon_tx_buffer_thrsld = 0x1750;
        cfg.edwrr_en_rsvd = 1;       
        cfg_msk.s.us_abr_en = 1;
        cfg_msk.s.us_dp_en = 1;
        cfg_msk.s.dis_win_fit_sp_rsvd = 1;
        cfg_msk.s.dp_us_thrsld = 1;
        cfg_msk.s.pon_tx_lmt_en = 1;
        cfg_msk.s.pon_tx_buffer_thrsld = 1;
        cfg_msk.s.edwrr_en_rsvd = 1;
        rt = __bm_ext_cfg_set(cfg_msk, &cfg);   
        if (rt != CS_E_OK)
            return rt;

        rt |= __bm_dp_cfg_stage_set(1, 0x1000);
        rt |= __bm_dp_cfg_stage_set(2, 0x1050);
        rt |= __bm_dp_cfg_stage_set(3, 0x1100);
        rt |= __bm_dp_cfg_stage_set(4, 0x1150);
        rt |= __bm_dp_cfg_stage_set(5, 0x1200);
        rt |= __bm_dp_cfg_stage_set(6, 0x1250);
        rt |= __bm_dp_cfg_stage_set(7, 0x1500);
        rt |= __bm_dp_cfg_stage_set(8, 0x1700);

        
        for (que = 0; que < 8; que++)
            td_cfg.pon_drop_thrshld[que] = 0xff;
        td_msk.s.pon_drop_thrshld = 0xff;
        rt |= aal_td_cfg_set(td_msk, &td_cfg);

        break;
    case AAL_PORT_ID_GE:
        cfg.ds_abr_en = 0;
        cfg.ds_dp_en = 0;
        cfg.ge_tx_lmt_en = 1;
        cfg.ge_tx_buffer_thrsld = 0x2900;
        cfg_msk.s.ds_abr_en = 1;
        cfg_msk.s.ds_dp_en = 1;
        cfg_msk.s.ge_tx_lmt_en = 1;
        cfg_msk.s.ge_tx_buffer_thrsld = 1;
        rt = __bm_ext_cfg_set(cfg_msk, &cfg);  
        if (rt != CS_E_OK)
            return rt;

        for (que = 0; que < 8; que++)
            q_cfg.val[que] = 0x28;
        q_mask.s.queue = 0xff;
        rt |= aal_bm_share_queue_max_length_set(port,q_mask,&q_cfg);
        
        break;
    default:
        return CS_E_PARAM;
    }

    return rt;
}

cs_status __bm_sp_drr_mode_set(cs_aal_port_id_t port)
{
    cs_status rt = CS_E_OK;
    __bm_ext_cfg_t cfg;
    __bm_ext_cfg_msk_t cfg_msk;
    cs_aal_td_cfg_t td_cfg;
    cs_aal_td_cfg_msk_t td_msk;   
    cs_aal_bm_queue_msk_t q_mask;
    cs_aal_bm_queue_val_t q_cfg;    
    cs_uint8 que;

    cfg_msk.u= 0;
    td_msk.u32 = 0;

    switch (port) {
    case AAL_PORT_ID_PON:
        cfg.us_abr_en = 0;
        cfg.us_dp_en = 1;
        cfg.dis_win_fit_sp_rsvd = 0;
        cfg.dp_us_thrsld = 0x1000;
        cfg.pon_tx_lmt_en = 1;
        cfg.pon_tx_buffer_thrsld = 0x1a00;
        cfg.edwrr_en_rsvd = 1;
        cfg_msk.s.us_abr_en = 1;
        cfg_msk.s.us_dp_en = 1;
        cfg_msk.s.dis_win_fit_sp_rsvd = 1;
        cfg_msk.s.dp_us_thrsld = 1;
        cfg_msk.s.pon_tx_lmt_en = 1;
        cfg_msk.s.pon_tx_buffer_thrsld = 1;
        cfg_msk.s.edwrr_en_rsvd = 1;
        rt = __bm_ext_cfg_set(cfg_msk, &cfg);
        if (rt != CS_E_OK)
            return rt;

        rt |= __bm_dp_cfg_stage_set(1, 0x1000);
        rt |= __bm_dp_cfg_stage_set(2, 0x1100);
        rt |= __bm_dp_cfg_stage_set(3, 0x1200);
        rt |= __bm_dp_cfg_stage_set(4, 0x1300);
        rt |= __bm_dp_cfg_stage_set(5, 0x1400);
        rt |= __bm_dp_cfg_stage_set(6, 0x1500);
        rt |= __bm_dp_cfg_stage_set(7, 0x1700);
        rt |= __bm_dp_cfg_stage_set(8, 0x1900);
  
        for (que = 0; que < 6; que++)
            td_cfg.pon_drop_thrshld[que] = 0x40;
        td_cfg.pon_drop_thrshld[6] = 0x60;
        td_cfg.pon_drop_thrshld[7] = 0x80;
        td_msk.s.pon_drop_thrshld = 0xff;
        rt |= aal_td_cfg_set(td_msk, &td_cfg);
        
        break;
    case AAL_PORT_ID_GE:
        cfg.ds_abr_en = 0;
        cfg.ds_dp_en = 0;
        cfg.ge_tx_lmt_en = 1;
        cfg.ge_tx_buffer_thrsld = 0x2380;        
        cfg_msk.s.ds_abr_en = 1;
        cfg_msk.s.ds_dp_en = 1;
        cfg_msk.s.ge_tx_lmt_en = 1;
        cfg_msk.s.ge_tx_buffer_thrsld = 1;        
        rt = __bm_ext_cfg_set(cfg_msk, &cfg);        
        if (rt != CS_E_OK)
            return rt;

        for (que = 0; que < 8;que++) 
            q_cfg.val[que] = 0x23;
        q_mask.s.queue = 0xff;
        rt |= aal_bm_share_queue_max_length_set(port,q_mask,&q_cfg);
        
        break;
    default:
        return CS_E_PARAM;
    }

    return rt;
}

#define BASE    0x2c300000
#define printf cs_printf
void reg_write(cyg_uint32 addr, cyg_uint32 wdata) {
  HAL_WRITE_UINT32(BASE + (addr << 2), wdata);
}

cyg_uint32 reg_read(cyg_uint32 addr) {
  cyg_uint32    rdata = 0;

  HAL_READ_UINT32(BASE + (addr << 2), rdata);

  return(rdata);
}

cyg_uint32 get_next_buf_ptr(cyg_uint32 cur_ptr) {
  cyg_uint32    nxt_ptr;
  cyg_uint32    rdata;
  cyg_uint32    loop_cnt = 0;

  reg_write(BM_BM_HEADER_MEMORY_ACCESS, 0x80000000 | cur_ptr);

  rdata = reg_read(BM_BM_HEADER_MEMORY_ACCESS);
  while (loop_cnt < 100 && (rdata & 0x80000000) != 0) {
    rdata = reg_read(BM_BM_HEADER_MEMORY_ACCESS);
    loop_cnt++;
  }

  if (loop_cnt == 100) {
    printf("ERROR: get_next_buf_ptr: reading BM_BM_HEADER_MEMORY_ACCESS got stuck\n");
    return -1;
  }

  rdata = reg_read(BM_BM_HEADER_MEMORY_DATA0);
  nxt_ptr = (((rdata >> 26) & 0xF) << 10) | ((rdata >> 11) & 0x3FF);

  return nxt_ptr;
}

void aal_check_link_list(cyg_uint32 print_ll) {
  cyg_uint32    i, j, rdata;

  cyg_uint32    head_ptr;
  cyg_uint32    tail_ptr;
  cyg_uint32    port_ptr[3];

  cyg_uint32    cur_ptr;
  cyg_uint32    nxt_ptr;
  cyg_uint32    free_buf_cnt = 0;
  cyg_uint32    err_cnt = 0;

  cyg_uint32    index, offset;
  cyg_uint32    buf_is_used;

  // slot[0] is buf0, buf1... buf31
  // slot[1] is buf32, buf33... buf63
  // each bit represent 1 buffer
  cyg_uint32    slot[512];
  
  for (i=0; i < 512; i++) {
    slot[i] = 0;
  }

  reg_write(GLOBAL_DBG_CTRL                 , 0x103     );
  reg_write(BM_CPU_ACCESS_OFFSET            , 0x2       );
  rdata = reg_read(GLOBAL_DBG_DATA0                     );
  port_ptr[2] = (rdata >> 2) & 0x3FFF;
  port_ptr[0] = (rdata >> 18) & 0x3FFF;
  reg_write(BM_CPU_ACCESS_OFFSET            , 0x3       );
  rdata = reg_read(GLOBAL_DBG_DATA0                     );
  head_ptr    = rdata & 0x3FFF;
  port_ptr[1] = (rdata >> 18) & 0x3FFF;
  reg_write(BM_CPU_ACCESS_OFFSET            , 0x1       );
  rdata = reg_read(GLOBAL_DBG_DATA1                     );
  tail_ptr    = rdata & 0x3FFF;

  printf("\nHead pointer: 0x%04x, Tail pointer: 0x%04x\n", head_ptr, tail_ptr);

  for (i=0; i < 3; i++) {
    index   = port_ptr[i] >> 5;
    offset  = port_ptr[i] & 0x1F;
    slot[index] |= (1 << offset);
    printf("Port %0d occupied buffer 0x%04x\n", i, port_ptr[i]);
    free_buf_cnt++;
  }
  printf("\n");
  
  cur_ptr = head_ptr;
  while (free_buf_cnt <= 0x4000 && cur_ptr != tail_ptr) {
    index   = cur_ptr >> 5;
    offset  = cur_ptr & 0x1F;
    buf_is_used = (slot[index] >> offset) & 0x1;
    if (buf_is_used) {
      printf("\nERROR: buffer 0x%04x is already used\n", cur_ptr);
      return;
    } else {
      if (free_buf_cnt < 100 || print_ll == 1) {
        printf("%4x -> ", cur_ptr);
        if (((free_buf_cnt-2) % 10) == 0) {
          printf("\n");
        }
      }
      slot[index] |= (1 << offset);
    }
    nxt_ptr = get_next_buf_ptr(cur_ptr);
    cur_ptr = nxt_ptr;
    free_buf_cnt++;
  }

  if (cur_ptr == tail_ptr) {
    index   = cur_ptr >> 5;
    offset  = cur_ptr & 0x1F;
    slot[index] |= (1 << offset);
    free_buf_cnt++;
  }

  if (free_buf_cnt != 0x4000) {
    printf("ERROR: The following buffers are not in free buffer link-list:\n");
    for (i=0; i < 512; i++) {
      if (slot[i] != 0xFFFFFFFF) {
        for (j=0; j < 32; j++) {
          if (((slot[i] >> j) & 0x1) != 0x1) {
            if (err_cnt < 100 || print_ll == 1) {
              printf("%4x ", (i << 5) | j);
              if (((err_cnt +1) % 10) == 0) {
                printf("\n");
              }
            }
            err_cnt++;
          }
        }
      }
    }
  }
  printf("\n\nFree buffer linked slots=%0d, un-linked buffer count=%0d\n\n", free_buf_cnt, err_cnt);
}



/************************ shared part ******************************/
void bm_init(void)
{
    __bm_port_tx_ctrl_t port_tx_ctrl;
    __bm_ext_cfg_t cfg;
    __bm_port_tx_ctrl_msk_t tx_ctrl_msk;
    __bm_ext_cfg_msk_t cfg_msk;

    cfg_msk.u = 0;
    tx_ctrl_msk.u32 = 0;

    cfg.cpu_cnt_option_rsvd = 1;
    cfg.ge_shaper_en = 1;
    cfg_msk.s.cpu_cnt_option_rsvd = 1;
    cfg_msk.s.ge_shaper_en = 1;
    __bm_ext_cfg_set(cfg_msk, &cfg);

    port_tx_ctrl.cpu_tx_en = 0xff;
    port_tx_ctrl.pon_tx_en = 0xff;
    port_tx_ctrl.ge_tx_en = 0xff;
    tx_ctrl_msk.s.cpu_tx_en = 0xff;
    tx_ctrl_msk.s.ge_tx_en = 0xff;
    tx_ctrl_msk.s.pon_tx_en = 0xff;
    __bm_port_tx_ctrl_set(tx_ctrl_msk, &port_tx_ctrl);
}

#if 0
/*
static void bm_clear_intr()
{
    __bm_interrupt_enable(TRUE); 
}
*/

void bm_interrupt_process()
{
//    cs_uint8 int_statu = 1;
    __bm_interrupt_enable(FALSE);

//    onu_sdl_event_send(EPON_EVENT_BM_CRC,sizeof(int_statu), &int_statu);
//    cs_timer_add(1000, aal_bm_clear_intr, NULL);

}
#endif

/************************ public part ******************************/

void aal_bm_static_mode_init(
    cs_uint8 cpu_acc_ram_mode
)
{
    __bm_ext_cfg_t cfg;
    __bm_ext_cfg_msk_t cfg_msk;
    cs_aal_bm_int_msk_t bm_intr_msk;
    cs_aal_bm_q_size_t us_q_size, ds_q_size;
    cs_uint8 index;

    cfg_msk.u = 0;
    cfg_msk.s.bm_mode = 1;
    cfg.bm_mode = __FIFO_MODE;
    __bm_ext_cfg_set(cfg_msk, &cfg);

    for(index = 0; index<__BM_MAX_QUE_NUM; index++){
        us_q_size.qx_size[index] = 128;
        ds_q_size.qx_size[index] = 128;
    }

    switch (cpu_acc_ram_mode) {
    case 0:
        REG_WRITE(BM_MA_Q7_START, 0x00000000);
        REG_WRITE(BM_MA_Q7_END, 0x00BE01FF);
        REG_WRITE(BM_MA_Q6_END, 0x017E03FF);
        REG_WRITE(BM_MA_Q5_END, 0x023E05FF);
        REG_WRITE(BM_MA_Q4_END, 0x02FE07FF);
        REG_WRITE(BM_MA_Q3_END, 0x03BE09FF);
        REG_WRITE(BM_MA_Q2_END, 0x047E0BFF);
        REG_WRITE(BM_MA_Q1_END, 0x053E0DFF);
        REG_WRITE(BM_MA_Q0_END, 0x05FE0FFF);
        REG_WRITE(BM_US_Q4_START, 0x6001000);
        REG_WRITE(BM_US_Q4_END, 0x06BE11FF);
        REG_WRITE(BM_DS_Q4_START, 0x4b00c800);
        REG_WRITE(BM_DS_Q4_END, 0x4BBEC9FF);

//        g_us_max_que_size = 512;
//        g_ds_max_que_size = 512;
        break;
    case 1:
        REG_WRITE(BM_MA_Q7_START, 0x0c002000);
        REG_WRITE(BM_MA_Q7_END, 0x0cBE21FF);
        REG_WRITE(BM_MA_Q6_END, 0x0d7E23FF);
        REG_WRITE(BM_MA_Q5_END, 0x0e3E25FF);
        REG_WRITE(BM_MA_Q4_END, 0x0eFE27FF);
        REG_WRITE(BM_MA_Q3_END, 0x0fBE29FF);
        REG_WRITE(BM_MA_Q2_END, 0x107E2BFF);
        REG_WRITE(BM_MA_Q1_END, 0x113E2DFF);
        REG_WRITE(BM_MA_Q0_END, 0x11FE2FFF);
        REG_WRITE(BM_US_Q4_START, 0x12003000);
        REG_WRITE(BM_US_Q4_END, 0x12BE31FF);
        REG_WRITE(BM_DS_Q4_START, 0x4b00c800);
        REG_WRITE(BM_DS_Q4_END, 0x4BBEC9FF);
        us_q_size.qx_size[3] = 32;
//        g_us_max_que_size = 416;
//        g_ds_max_que_size = 512;
        break;
    case 2:
        REG_WRITE(BM_MA_Q7_START, 0x18004000);
        REG_WRITE(BM_MA_Q7_END, 0x18BE41FF);
        REG_WRITE(BM_MA_Q6_END, 0x197E43FF);
        REG_WRITE(BM_MA_Q5_END, 0x1a3E45FF);
        REG_WRITE(BM_MA_Q4_END, 0x1aFE47FF);
        REG_WRITE(BM_MA_Q3_END, 0x1bBE49FF);
        REG_WRITE(BM_MA_Q2_END, 0x1c7E4BFF);
        REG_WRITE(BM_MA_Q1_END, 0x1d3E4DFF);
        REG_WRITE(BM_MA_Q0_END, 0x1dFE4FFF);
        REG_WRITE(BM_US_Q4_START, 0x1e005000);
        REG_WRITE(BM_US_Q4_END, 0x1eBE31FF);
        REG_WRITE(BM_DS_Q4_START, 0x5400E000);
        REG_WRITE(BM_DS_Q4_END, 0x54BEE1FF);
        us_q_size.qx_size[3] = 32;
        us_q_size.qx_size[2] = 64;
        ds_q_size.qx_size[3] = 32;
//        g_us_max_que_size = 384;
//        g_ds_max_que_size = 416;
        break;

    case 3:
        REG_WRITE(BM_MA_Q7_START, 0x8A017000);
        REG_WRITE(BM_MA_Q7_END, 0x8ABF71FF);
        REG_WRITE(BM_MA_Q6_END, 0x8B7F73FF);
        REG_WRITE(BM_MA_Q5_END, 0x8B3F75FF);
        REG_WRITE(BM_MA_Q4_END, 0x8BFF77FF);
        REG_WRITE(BM_MA_Q3_END, 0x8CBF79FF);
        REG_WRITE(BM_MA_Q2_END, 0x8D7F7BFF);
        REG_WRITE(BM_MA_Q1_END, 0x8E3F7DFF);
        REG_WRITE(BM_MA_Q0_END, 0x8EFF7FFF);
        REG_WRITE(BM_US_Q4_START, 0x30008000);
        REG_WRITE(BM_US_Q4_END, 0x30BE81FF);
        REG_WRITE(BM_DS_Q4_START, 0x60010000);
        REG_WRITE(BM_DS_Q4_END, 0x60BF01ff);

        us_q_size.qx_size[3] = 32;
        us_q_size.qx_size[2] = 64;
        ds_q_size.qx_size[3] = 32;
        ds_q_size.qx_size[2] = 32;
//        g_us_max_que_size = 350;
//        g_ds_max_que_size = 300;
        break;
    default:
        break;
    }
    __bm_us_que_size_set(&us_q_size);
    __bm_ds_que_size_set(&ds_q_size);



    bm_intr_msk.u32 = 0;
    bm_intr_msk.s.cpu_tx_crc_error = 1;
    bm_intr_msk.s.ge_tx_crc_error = 1;
    bm_intr_msk.s.pon_tx_crc_error = 1;

    aal_bm_int_status_clr(bm_intr_msk);
    aal_bm_int_enable(bm_intr_msk);  
  
    __bm_reset();

    __bm_re_dir_enable(AAL_PORT_ID_GE, TRUE);
    __bm_re_dir_enable(AAL_PORT_ID_PON, TRUE);  
    
}

void aal_bm_share_mode_init(void)
{
    __bm_ext_cfg_t cfg;
    __bm_ext_cfg_msk_t cfg_msk;
    cs_aal_bm_queue_msk_t qmsk;
    cs_aal_bm_queue_val_t qval;    
    cs_uint8 que;
    
    cfg_msk.u = 0;
    REG_FIELD_WRITE(BM_CPU_PORT_BUFFER_CFG,private_lmt,0x535);
    REG_FIELD_WRITE(BM_MA_PORT_TX_BUFFER_LMT,tx_buffer_thrsld,0x535);
    REG_FIELD_WRITE(BM_MA_PORT_TX_BUFFER_LMT,tx_lmt_en,1);

    cfg.bm_mode = __MEMORY_MODE;
    cfg.ds_abr_en = 1;
    cfg.us_abr_en = 1;
    cfg.ge_ql_en = 1;
    cfg.pon_ql_en = 1;
    cfg.dis_win_fit_sp_rsvd = 1;
    cfg.slmt_cpu = 0x3d2a;
    cfg.slmt  = 0x3dfa;
    cfg.ddr_weight_base = __BM_DEF_WGT_BASE;
    cfg.abr_ds_thrsld = 0x1200;
    cfg.abr_us_thrsld = 0x2580;
    cfg.pon_tx_lmt_en = 1;
    cfg.pon_tx_buffer_thrsld = 0x2aaa;
    cfg.ge_tx_lmt_en = 1;
    cfg.ge_tx_buffer_thrsld = 0x1380;    
    cfg.edwrr_en_rsvd = 0;
    cfg_msk.s.bm_mode = 1;
    cfg_msk.s.ds_abr_en = 1;
    cfg_msk.s.us_abr_en = 1;
    cfg_msk.s.ge_ql_en = 1;
    cfg_msk.s.pon_ql_en = 1;
    cfg_msk.s.dis_win_fit_sp_rsvd = 1;
    cfg_msk.s.slmt_cpu = 1;
    cfg_msk.s.slmt = 1;
    cfg_msk.s.ddr_weight_base = 1;
    cfg_msk.s.abr_us_thrsld = 1;
    cfg_msk.s.abr_ds_thrsld = 1;
    cfg_msk.s.pon_tx_lmt_en = 1;
    cfg_msk.s.pon_tx_buffer_thrsld = 1;
    cfg_msk.s.ge_tx_lmt_en = 1;
    cfg_msk.s.ge_tx_buffer_thrsld = 1;    
    cfg_msk.s.edwrr_en_rsvd = 1;
    __bm_ext_cfg_set(cfg_msk, &cfg);

 //   port_buf_cfg.private_lmt = 0xc0;
//    onu_aal_bm_port_buf_cfg_set(BM_CPU_PORT, &port_buf_cfg);
        
    for (que = 0; que < 8;que++) 
    {
        qval.val[que] = 0xf8;
    }
    qmsk.s.queue = 0xff;
    aal_bm_share_queue_max_length_set(AAL_PORT_ID_GE,qmsk,&qval);
    aal_bm_share_queue_max_length_set(AAL_PORT_ID_PON,qmsk,&qval);

    __bm_reset();
    AAL_MAJ_LOG("aal bm init done!\n");
}


cs_status aal_bm_share_queue_max_length_set(
    CS_IN cs_aal_port_id_t port,
    CS_IN cs_aal_bm_queue_msk_t mask,
    CS_IN cs_aal_bm_queue_val_t *cfg
)
{
    if(!cfg || mask.u32 == 0)
        return CS_E_PARAM;
    
    switch (port) {
    case AAL_PORT_ID_GE:
        if(mask.s.queue & 0x1)
            REG_FIELD_WRITE(BM_GE_QUEUE_LENGTH_LIMIT_COS3_0,ql_cos0,cfg->val[0]);
        if(mask.s.queue & (0x1 << 1))
            REG_FIELD_WRITE(BM_GE_QUEUE_LENGTH_LIMIT_COS3_0,ql_cos1,cfg->val[1]);
        if(mask.s.queue & (0x1 << 2))
            REG_FIELD_WRITE(BM_GE_QUEUE_LENGTH_LIMIT_COS3_0,ql_cos2,cfg->val[2]);
        if(mask.s.queue & (0x1 << 3))
            REG_FIELD_WRITE(BM_GE_QUEUE_LENGTH_LIMIT_COS3_0,ql_cos3,cfg->val[3]);
        if(mask.s.queue & (0x1 << 4))
            REG_FIELD_WRITE(BM_GE_QUEUE_LENGTH_LIMIT_COS7_4,ql_cos4,cfg->val[4]);
        if(mask.s.queue & (0x1 << 5))
            REG_FIELD_WRITE(BM_GE_QUEUE_LENGTH_LIMIT_COS7_4,ql_cos5,cfg->val[5]);
        if(mask.s.queue & (0x1 << 6))
            REG_FIELD_WRITE(BM_GE_QUEUE_LENGTH_LIMIT_COS7_4,ql_cos6,cfg->val[6]);
        if(mask.s.queue & (0x1 << 7))
            REG_FIELD_WRITE(BM_GE_QUEUE_LENGTH_LIMIT_COS7_4,ql_cos7,cfg->val[7]);
        break;
    case AAL_PORT_ID_PON:    
        if(mask.s.queue & 0x1)
            REG_FIELD_WRITE(BM_PON_QUEUE_LENGTH_LIMIT_COS3_0,ql_cos0,cfg->val[0]);
        if(mask.s.queue & (0x1 << 1))
            REG_FIELD_WRITE(BM_PON_QUEUE_LENGTH_LIMIT_COS3_0,ql_cos1,cfg->val[1]);
        if(mask.s.queue & (0x1 << 2))
            REG_FIELD_WRITE(BM_PON_QUEUE_LENGTH_LIMIT_COS3_0,ql_cos2,cfg->val[2]);
        if(mask.s.queue & (0x1 << 3))
            REG_FIELD_WRITE(BM_PON_QUEUE_LENGTH_LIMIT_COS3_0,ql_cos3,cfg->val[3]);
        if(mask.s.queue & (0x1 << 4))
            REG_FIELD_WRITE(BM_PON_QUEUE_LENGTH_LIMIT_COS7_4,ql_cos4,cfg->val[4]);
        if(mask.s.queue & (0x1 << 5))
            REG_FIELD_WRITE(BM_PON_QUEUE_LENGTH_LIMIT_COS7_4,ql_cos5,cfg->val[5]);
        if(mask.s.queue & (0x1 << 6))
            REG_FIELD_WRITE(BM_PON_QUEUE_LENGTH_LIMIT_COS7_4,ql_cos6,cfg->val[6]);
        if(mask.s.queue & (0x1 << 7))
            REG_FIELD_WRITE(BM_PON_QUEUE_LENGTH_LIMIT_COS7_4,ql_cos7,cfg->val[7]);
        break;
    case AAL_PORT_ID_CPU:  
        if(mask.s.queue & 0x1)
            REG_FIELD_WRITE(BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0,ql_cos0,(cfg->val[0] & 0xF));
        if(mask.s.queue & (0x1 << 1))
            REG_FIELD_WRITE(BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0,ql_cos1,(cfg->val[1] & 0xF));
        if(mask.s.queue & (0x1 << 2))
            REG_FIELD_WRITE(BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0,ql_cos2,(cfg->val[2] & 0xF));
        if(mask.s.queue & (0x1 << 3))
            REG_FIELD_WRITE(BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0,ql_cos3,(cfg->val[3] & 0xF));
        if(mask.s.queue & (0x1 << 4))
            REG_FIELD_WRITE(BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0,ql_cos4,(cfg->val[4] & 0xF));
        if(mask.s.queue & (0x1 << 5))
            REG_FIELD_WRITE(BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0,ql_cos5,(cfg->val[5] & 0xF));
        if(mask.s.queue & (0x1 << 6))
            REG_FIELD_WRITE(BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0,ql_cos6,(cfg->val[6] & 0xF));
        if(mask.s.queue & (0x1 << 7))
            REG_FIELD_WRITE(BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0,ql_cos7,(cfg->val[7] & 0xF));
        break;
    default:
        return CS_E_PARAM;
        }
    
    return CS_E_OK;
}

cs_status aal_bm_share_queue_max_length_get(
    CS_IN  cs_aal_port_id_t port,
    CS_OUT cs_aal_bm_queue_val_t *cfg
)
{
    BM_GE_QUEUE_LENGTH_LIMIT_COS7_4_t ge_cos7_4;
    BM_GE_QUEUE_LENGTH_LIMIT_COS3_0_t ge_cos3_0;
    BM_PON_QUEUE_LENGTH_LIMIT_COS7_4_t pon_cos7_4;
    BM_PON_QUEUE_LENGTH_LIMIT_COS3_0_t pon_cos3_0;
    BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0_t cpu_cos7_0;

    if (NULL == cfg)
        return CS_E_PARAM;

    switch (port) {
    case AAL_PORT_ID_GE:
        REG_READ(BM_GE_QUEUE_LENGTH_LIMIT_COS3_0,ge_cos3_0.wrd);
        REG_READ(BM_GE_QUEUE_LENGTH_LIMIT_COS7_4,ge_cos7_4.wrd);
        cfg->val[0] = ge_cos3_0.bf.ql_cos0;
        cfg->val[1] = ge_cos3_0.bf.ql_cos1;
        cfg->val[2] = ge_cos3_0.bf.ql_cos2;
        cfg->val[3] = ge_cos3_0.bf.ql_cos3;
        cfg->val[4] = ge_cos7_4.bf.ql_cos4;
        cfg->val[5] = ge_cos7_4.bf.ql_cos5;
        cfg->val[6] = ge_cos7_4.bf.ql_cos6;
        cfg->val[7] = ge_cos7_4.bf.ql_cos7;
        break;
    case AAL_PORT_ID_PON:
        REG_READ(BM_PON_QUEUE_LENGTH_LIMIT_COS3_0,pon_cos3_0.wrd);
        REG_READ(BM_PON_QUEUE_LENGTH_LIMIT_COS7_4,pon_cos7_4.wrd);
        cfg->val[0] = pon_cos3_0.bf.ql_cos0;
        cfg->val[1] = pon_cos3_0.bf.ql_cos1;
        cfg->val[2] = pon_cos3_0.bf.ql_cos2;
        cfg->val[3] = pon_cos3_0.bf.ql_cos3;
        cfg->val[4] = pon_cos7_4.bf.ql_cos4;
        cfg->val[5] = pon_cos7_4.bf.ql_cos5;
        cfg->val[6] = pon_cos7_4.bf.ql_cos6;
        cfg->val[7] = pon_cos7_4.bf.ql_cos7;
        break;
    case AAL_PORT_ID_CPU:
        REG_READ(BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0,cpu_cos7_0.wrd);
        cfg->val[0] = cpu_cos7_0.bf.ql_cos0;
        cfg->val[1] = cpu_cos7_0.bf.ql_cos1;
        cfg->val[2] = cpu_cos7_0.bf.ql_cos2;
        cfg->val[3] = cpu_cos7_0.bf.ql_cos3;
        cfg->val[4] = cpu_cos7_0.bf.ql_cos4;
        cfg->val[5] = cpu_cos7_0.bf.ql_cos5;
        cfg->val[6] = cpu_cos7_0.bf.ql_cos6;
        cfg->val[7] = cpu_cos7_0.bf.ql_cos7;        
        break;
    default:
        return CS_E_PARAM;
    }

    return CS_E_OK;
}


cs_status aal_bm_static_cfg_set(
    CS_IN cs_aal_bm_static_cfg_msk_t mask,
    CS_IN cs_aal_bm_static_cfg_t *cfg
)
{
    cs_status rt = CS_E_OK;
    cs_aal_redirect_cfg_t  old_redir, new_redir;
    cs_aal_redirect_cfg_msk_t    cfg_msk;
    
    if((mask.u32 == 0)||(NULL == cfg))
        return CS_E_PARAM; 

    memset(&old_redir, 0, sizeof(old_redir));
    memset(&new_redir, 0, sizeof(new_redir));
    
    if(mask.s.us)
    {
        aal_redirect_cfg_get(AAL_PORT_ID_GE, &new_redir);
        old_redir = new_redir;
        
        cfg_msk.u32 = 0;
        cfg_msk.s.dpid_redirect0_en = 1;
        cfg_msk.s.redirect0_fwd_dpid = 1;
        cfg_msk.s.redirect0_original_dpid = 1;
        new_redir.redirect0_fwd_dpid.dst_op = AAL_SPEC_DST_DROP;        
        new_redir.redirect0_original_dpid.dst_op = AAL_SPEC_DST_PORT;
        new_redir.redirect0_original_dpid.dpid= AAL_PORT_ID_PON;
        new_redir.dpid_redirect0_en = 1;        

        aal_redirect_cfg_set(AAL_PORT_ID_GE, cfg_msk, &new_redir);
        
        rt = __bm_us_que_size_set(&cfg->us); 
        
        cfg_msk.u32 = 0;
        cfg_msk.s.dpid_redirect0_en = 1;
        cfg_msk.s.redirect0_fwd_dpid = 1;
        cfg_msk.s.redirect0_original_dpid = 1;

        aal_redirect_cfg_set(AAL_PORT_ID_GE, cfg_msk, &old_redir);
    }

    if(mask.s.ds)
    {
        aal_redirect_cfg_get(AAL_PORT_ID_PON, &new_redir);
        old_redir = new_redir;
        
        cfg_msk.u32 = 0;
        cfg_msk.s.dpid_redirect0_en = 1;
        cfg_msk.s.redirect0_fwd_dpid = 1;
        cfg_msk.s.redirect0_original_dpid = 1;
        new_redir.redirect0_fwd_dpid.dst_op = AAL_SPEC_DST_DROP;        
        new_redir.redirect0_original_dpid.dst_op = AAL_SPEC_DST_PORT;
        new_redir.redirect0_original_dpid.dpid= AAL_PORT_ID_GE;
        new_redir.dpid_redirect0_en = 1;        

        aal_redirect_cfg_set(AAL_PORT_ID_PON, cfg_msk, &new_redir);
        
        rt |= __bm_ds_que_size_set(&cfg->ds);

        cfg_msk.u32 = 0;
        cfg_msk.s.dpid_redirect0_en = 1;
        cfg_msk.s.redirect0_fwd_dpid = 1;
        cfg_msk.s.redirect0_original_dpid = 1;

        aal_redirect_cfg_set(AAL_PORT_ID_PON, cfg_msk, &old_redir);
    }
    
    return rt;
}

cs_status aal_bm_static_cfg_get(
    CS_OUT cs_aal_bm_static_cfg_t *cfg
)
{
    cs_status rt = CS_E_OK;

    if (!cfg)
        return CS_E_PARAM;

    rt  = __bm_us_que_size_get(&cfg->us);
    rt |= __bm_ds_que_size_get(&cfg->ds);

    return rt;
}


cs_status aal_bm_share_wrr_weight_base_set(
    CS_IN cs_aal_bm_sched_weight_t weight_base
)
{
    __bm_ext_cfg_t cfg;
    __bm_ext_cfg_msk_t cfg_msk;  

    cfg_msk.u = 0;
    cfg_msk.s.ddr_weight_base = 1;
    cfg.ddr_weight_base = (cs_uint8)weight_base;
    return __bm_ext_cfg_set(cfg_msk,&cfg);
}

cs_status aal_bm_share_wrr_weight_base_get(
    CS_IN cs_aal_bm_sched_weight_t *weight_base
)
{
    cs_status rt = CS_E_OK;
    __bm_ext_cfg_t cfg;

    if (!weight_base)
        return CS_E_PARAM;
    
    rt = __bm_ext_cfg_get(&cfg);
    if (rt != CS_E_OK) 
        return rt;

    *weight_base = (cs_aal_bm_sched_weight_t)cfg.ddr_weight_base;

    return rt;
}

cs_status aal_bm_share_scheduler_set(
    CS_IN cs_aal_port_id_t port,
    CS_IN cs_aal_bm_weight_t *weight
)
{
    cs_aal_bm_queue_msk_t qmsk;
    cs_aal_bm_queue_val_t qval;
    cs_uint8 index;
    cs_uint8 ql[8];
    
    if (!weight) 
        return CS_E_PARAM;

    qmsk.u32 = 0;

    switch (weight->mode) {
    case ONU_BM_SCHED_SP:
        __bm_sp_mode_set(port);
        
        for (index = 0; index < 8; index++) 
            qval.val[index] = 0;
        qmsk.s.queue = 0xFF;
        __bm_port_que_weight_ratio_set(port, qmsk, &qval);
        
        break;
    case ONU_BM_SCHED_SP_WRR:
        __bm_sp_drr_mode_set(port);
        weight->weight[6] = 0;
        weight->weight[7] = 0;
              
        qmsk.s.queue = 0xFF;
        __bm_port_que_weight_ratio_set(port, qmsk, (cs_aal_bm_queue_val_t *)&weight->weight);

        if(port == AAL_PORT_ID_PON)
        {
            __bm_calc_que_len(weight->weight, ql);
            qmsk.s.queue = 0xFF;
            aal_bm_share_queue_max_length_set(port,qmsk,(cs_aal_bm_queue_val_t *)&ql);
        }

        qval.val[6] = 0x80;
        qval.val[7] = 0xF0;
        qmsk.s.queue = 0xC0;
        aal_bm_share_queue_max_length_set(AAL_PORT_ID_PON,qmsk,&qval);

        break;
    case ONU_BM_SCHED_WRR:
        __bm_drr_mode_set(port);

        qmsk.s.queue = 0xFF;
        __bm_port_que_weight_ratio_set(port, qmsk, (cs_aal_bm_queue_val_t *)&weight->weight);

        if(port == AAL_PORT_ID_PON)
        {
            __bm_calc_que_len(weight->weight, ql);
            qmsk.s.queue = 0xFF;
            aal_bm_share_queue_max_length_set(port,qmsk,(cs_aal_bm_queue_val_t *)&ql);
        }
        break;
    default:
        return CS_E_PARAM;
    }
    
    return CS_E_OK;
}

cs_status aal_bm_share_scheduler_get(
    CS_IN cs_aal_port_id_t port,
    CS_OUT cs_aal_bm_weight_t *weight
)
{
    cs_status rt = CS_E_OK;
    cs_aal_bm_queue_val_t qval;
    cs_uint8 index;
    cs_uint16 total_weight = 0;

    if (!weight) 
        return CS_E_PARAM;

    rt = __bm_port_que_weight_ratio_get(port, &qval);
    if (rt != CS_E_OK) 
        return rt;
    
    for (index = 0; index < 8; index++) {
        weight->weight[index] = qval.val[index];
        total_weight += weight->weight[index];
    }
    
    if (total_weight == 0) 
    {
        weight->mode = ONU_BM_SCHED_SP;
    } 
    else if (weight->weight[6] == 0 && weight->weight[7] == 0) 
    {
        weight->mode = ONU_BM_SCHED_SP_WRR;
    } 
    else 
    {
        weight->mode = ONU_BM_SCHED_WRR;
    }
    
    return CS_E_OK;
}

cs_status aal_bm_static_queue_flush(
    CS_IN cs_aal_port_id_t port,
    CS_IN cs_aal_bm_static_queue_t queue
)
{
    return __bm_que_flush_enable(port, (cs_uint8)queue, TRUE);
}

cs_status aal_bm_share_buffer_flush(
    CS_IN cs_aal_port_id_t port
)
{
    return __bm_flush(port);
}

cs_status aal_bm_stats_get(
    CS_IN cs_aal_port_id_t port_id, 
    CS_OUT cs_aal_bm_stats_t *bm_stats
)
{
    if (!bm_stats) 
        return CS_E_PARAM;
    
    switch (port_id) {
    case AAL_PORT_ID_GE:
        g_fe_drp[0] += __reg_value_get(BM_RMU_PT0_DROP_CNT2);
        g_bm_drp[0] += __reg_value_get(BM_RMU_PT0_DROP_CNT1);       
        bm_stats->fe_drp_cnt = g_fe_drp[0];//+ __reg_value_get(BM_RMU_PT0_DROP_CNT2);
        bm_stats->bm_drp_cnt = g_bm_drp[0];// + __reg_value_get(BM_RMU_PT0_DROP_CNT1);
        bm_stats->bm_pkt_cnt[0] = __reg_value_get(BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS0);
        bm_stats->bm_pkt_cnt[1] = __reg_value_get(BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS1);
        bm_stats->bm_pkt_cnt[2] = __reg_value_get(BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS2);
        bm_stats->bm_pkt_cnt[3] = __reg_value_get(BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS3);
        break;
    case AAL_PORT_ID_PON:
        g_fe_drp[1] += __reg_value_get(BM_RMU_PT1_DROP_CNT2);
        g_bm_drp[1] += __reg_value_get(BM_RMU_PT1_DROP_CNT1);
        bm_stats->fe_drp_cnt = g_fe_drp[1];// + __reg_value_get(BM_RMU_PT1_DROP_CNT2);
        bm_stats->bm_drp_cnt = g_bm_drp[1];// + __reg_value_get(BM_RMU_PT1_DROP_CNT1);
        bm_stats->bm_pkt_cnt[0] = __reg_value_get(BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS0);
        bm_stats->bm_pkt_cnt[1] = __reg_value_get(BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS1);
        bm_stats->bm_pkt_cnt[2] = __reg_value_get(BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS2);
        bm_stats->bm_pkt_cnt[3] = __reg_value_get(BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS3);
        break;
    case AAL_PORT_ID_CPU:
        g_fe_drp[2] += __reg_value_get(BM_RMU_PT2_DROP_CNT2);
        g_bm_drp[2] += __reg_value_get(BM_RMU_PT2_DROP_CNT1);
        bm_stats->fe_drp_cnt = g_fe_drp[1] ;//+ __reg_value_get(BM_RMU_PT2_DROP_CNT2);
        bm_stats->bm_drp_cnt = g_bm_drp[2];// + __reg_value_get(BM_RMU_PT2_DROP_CNT1);
        bm_stats->bm_pkt_cnt[0] = __reg_value_get(BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS4) +
                                 __reg_value_get(BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS4);
        bm_stats->bm_pkt_cnt[1] = 0;
        bm_stats->bm_pkt_cnt[2] = 0;
        bm_stats->bm_pkt_cnt[3] = 0;
        break;
    default:
        return CS_E_PARAM;
    }

    return CS_E_OK;
}

cs_status aal_td_cfg_set
(
    CS_IN cs_aal_td_cfg_msk_t msk,
    CS_IN cs_aal_td_cfg_t *cfg
)
{
    if( !cfg || msk.u32 == 0)
        return CS_E_PARAM;
    
    if(msk.s.direction)
        REG_FIELD_WRITE(TE_CTRL,td_all_buffer_sel,(cfg->direction > 0));   
    if(msk.s.ge_td_drop_en)
        REG_FIELD_WRITE(TE_CTRL,ge_td_drop_en, (cfg->ge_td_drop_en > 0));
    if(msk.s.pon_td_drop_en)
        REG_FIELD_WRITE(TE_CTRL,pon_td_drop_en, (cfg->pon_td_drop_en > 0));

    if(msk.s.ge_drop_thrshld > 0)
    {
        if(msk.s.ge_drop_thrshld & 0x1)
            REG_FIELD_WRITE(TE_GE_TD_CFG0,drop_thrshld_0,cfg->ge_drop_thrshld[0]);
        if(msk.s.ge_drop_thrshld & (0x1 << 1))
            REG_FIELD_WRITE(TE_GE_TD_CFG0,drop_thrshld_1,cfg->ge_drop_thrshld[1]);
        if(msk.s.ge_drop_thrshld & (0x1 << 2))
            REG_FIELD_WRITE(TE_GE_TD_CFG0,drop_thrshld_2,cfg->ge_drop_thrshld[2]);
        if(msk.s.ge_drop_thrshld & (0x1 << 3))
            REG_FIELD_WRITE(TE_GE_TD_CFG0,drop_thrshld_3,cfg->ge_drop_thrshld[3]);
        if(msk.s.ge_drop_thrshld & (0x1 << 4))
            REG_FIELD_WRITE(TE_GE_TD_CFG1,drop_thrshld_4,cfg->ge_drop_thrshld[4]);
        if(msk.s.ge_drop_thrshld & (0x1 << 5))
            REG_FIELD_WRITE(TE_GE_TD_CFG1,drop_thrshld_5,cfg->ge_drop_thrshld[5]);
        if(msk.s.ge_drop_thrshld & (0x1 << 6))
            REG_FIELD_WRITE(TE_GE_TD_CFG1,drop_thrshld_6,cfg->ge_drop_thrshld[6]);
        if(msk.s.ge_drop_thrshld & (0x1 << 7))
            REG_FIELD_WRITE(TE_GE_TD_CFG1,drop_thrshld_7,cfg->ge_drop_thrshld[7]);       
    }

    if(msk.s.pon_drop_thrshld > 0)
    {
        if(msk.s.pon_drop_thrshld & 0x1)
            REG_FIELD_WRITE(TE_PON_TD_CFG0,drop_thrshld_0,cfg->pon_drop_thrshld[0]);
        if(msk.s.pon_drop_thrshld & (0x1 << 1))
            REG_FIELD_WRITE(TE_PON_TD_CFG0,drop_thrshld_1,cfg->pon_drop_thrshld[1]);
        if(msk.s.pon_drop_thrshld & (0x1 << 2))
            REG_FIELD_WRITE(TE_PON_TD_CFG0,drop_thrshld_2,cfg->pon_drop_thrshld[2]);
        if(msk.s.pon_drop_thrshld & (0x1 << 3))
            REG_FIELD_WRITE(TE_PON_TD_CFG0,drop_thrshld_3,cfg->pon_drop_thrshld[3]);
        if(msk.s.pon_drop_thrshld & (0x1 << 4))
            REG_FIELD_WRITE(TE_PON_TD_CFG1,drop_thrshld_4,cfg->pon_drop_thrshld[4]);
        if(msk.s.pon_drop_thrshld & (0x1 << 5))
            REG_FIELD_WRITE(TE_PON_TD_CFG1,drop_thrshld_5,cfg->pon_drop_thrshld[5]);
        if(msk.s.pon_drop_thrshld & (0x1 << 6))
            REG_FIELD_WRITE(TE_PON_TD_CFG1,drop_thrshld_6,cfg->pon_drop_thrshld[6]);
        if(msk.s.pon_drop_thrshld & (0x1 << 7))
            REG_FIELD_WRITE(TE_PON_TD_CFG1,drop_thrshld_7,cfg->pon_drop_thrshld[7]);         
    }   

    return CS_E_OK;
}

cs_status aal_td_cfg_get
(
    CS_OUT cs_aal_td_cfg_t *cfg
)
{
    TE_CTRL_t              te_ctrl;
    TE_PON_TD_CFG0_t       pon_td_0_3;
    TE_PON_TD_CFG1_t       pon_td_4_7;
    TE_GE_TD_CFG0_t        ge_td_0_3;
    TE_GE_TD_CFG1_t        ge_td_4_7;

    if(!cfg)
        return CS_E_PARAM;

    REG_READ(TE_CTRL,te_ctrl.wrd);
    REG_READ(TE_PON_TD_CFG0,pon_td_0_3.wrd);
    REG_READ(TE_PON_TD_CFG1,pon_td_4_7.wrd);
    REG_READ(TE_GE_TD_CFG0,ge_td_0_3.wrd);
    REG_READ(TE_GE_TD_CFG1,ge_td_4_7.wrd);

    cfg->direction = te_ctrl.bf.td_all_buffer_sel;
    cfg->ge_td_drop_en = te_ctrl.bf.ge_td_drop_en;
    cfg->pon_td_drop_en = te_ctrl.bf.pon_td_drop_en;
    cfg->ge_drop_thrshld[0] = ge_td_0_3.bf.drop_thrshld_0;
    cfg->ge_drop_thrshld[1] = ge_td_0_3.bf.drop_thrshld_1;
    cfg->ge_drop_thrshld[2] = ge_td_0_3.bf.drop_thrshld_2;
    cfg->ge_drop_thrshld[3] = ge_td_0_3.bf.drop_thrshld_3;
    cfg->ge_drop_thrshld[4] = ge_td_4_7.bf.drop_thrshld_4;
    cfg->ge_drop_thrshld[5] = ge_td_4_7.bf.drop_thrshld_5;
    cfg->ge_drop_thrshld[6] = ge_td_4_7.bf.drop_thrshld_6;
    cfg->ge_drop_thrshld[7] = ge_td_4_7.bf.drop_thrshld_7;
    
    cfg->pon_drop_thrshld[0] = pon_td_0_3.bf.drop_thrshld_0;
    cfg->pon_drop_thrshld[1] = pon_td_0_3.bf.drop_thrshld_1;
    cfg->pon_drop_thrshld[2] = pon_td_0_3.bf.drop_thrshld_2;
    cfg->pon_drop_thrshld[3] = pon_td_0_3.bf.drop_thrshld_3;
    cfg->pon_drop_thrshld[4] = pon_td_4_7.bf.drop_thrshld_4;
    cfg->pon_drop_thrshld[5] = pon_td_4_7.bf.drop_thrshld_5;
    cfg->pon_drop_thrshld[6] = pon_td_4_7.bf.drop_thrshld_6;
    cfg->pon_drop_thrshld[7] = pon_td_4_7.bf.drop_thrshld_7;
    
    return CS_E_OK;
}


cs_status aal_bm_int_enable(cs_aal_bm_int_msk_t mask)
{
    cs_uint32 int_en;
    REG_READ(BM_INTERRUPT_E, int_en);
    int_en |= mask.u32;
    REG_WRITE(BM_INTERRUPT_E, int_en);
    return CS_E_OK; 
}


cs_status aal_bm_int_disable(cs_aal_bm_int_msk_t mask)
{
    cs_uint32 int_en;
    
    REG_READ(BM_INTERRUPT_E, int_en);
    int_en &= (~mask.u32);
    REG_WRITE(BM_INTERRUPT_E, int_en);
    return CS_E_OK; 
}

cs_status aal_bm_int_en_get(cs_aal_bm_int_msk_t *mask)
{
    cs_uint32 int_en;
    
    if(NULL == mask){
        return CS_E_PARAM;
    }
    
    REG_READ(BM_INTERRUPT_E, int_en);
    mask->u32 = int_en;

    return CS_E_OK;
}

cs_status aal_bm_int_status_get(cs_aal_bm_int_msk_t* mask)
{

    cs_uint32 int_src;
    
    if(NULL == mask){
        return CS_E_PARAM;
    }
    REG_READ(BM_INTERRUPT, int_src);
    mask->u32 = int_src;
    return CS_E_OK; 
}

cs_status aal_bm_int_status_clr(cs_aal_bm_int_msk_t src)
{

    REG_WRITE(BM_INTERRUPT, src.u32); /*write 1 to clear*/

    return CS_E_OK; 
}

void aal_bm_de_register_handler(void)
{
    __bm_re_dir_flush_bm();
}

void aal_bm_register_handler(void)
{
    GLOBAL_CTRL_t glb_ctrl;
    REG_READ(GLOBAL_CTRL,glb_ctrl.wrd);
    if(glb_ctrl.bf.legacy_mode == 1){
        __bm_re_dir_enable(AAL_PORT_ID_GE, FALSE);
        __bm_re_dir_enable(AAL_PORT_ID_PON, FALSE);
    }
}

cs_uint8 aal_eee_on = 0;
cs_uint16 aal_eee_thre = 0;
cs_uint8 aal_eee_que_thre[4] = {0};



cs_status aal_eee_save(void)
{
    BM_PON_EEE_ECFG_t eee_cfg;
    BM_PON_EEE_THRESHOLD1_t eee_thrsh;
    
    cs_status rt = CS_E_OK;
    
    REG_READ(BM_PON_EEE_ECFG, eee_cfg.wrd);
    aal_eee_on = eee_cfg.bf.eee_on;
    REG_FIELD_WRITE(BM_PON_EEE_ECFG, eee_on, 0);

    REG_READ(BM_PON_EEE_THRESHOLD3, eee_thrsh.wrd);
    aal_eee_thre = eee_thrsh.bf.eee_thres;
    
    REG_FIELD_READ(BM_PON_EEE_QUEUE0, qthres_sel, aal_eee_que_thre[0]);
    REG_FIELD_READ(BM_PON_EEE_QUEUE1, qthres_sel, aal_eee_que_thre[1]);
    REG_FIELD_READ(BM_PON_EEE_QUEUE2, qthres_sel, aal_eee_que_thre[2]);
    REG_FIELD_READ(BM_PON_EEE_QUEUE3, qthres_sel, aal_eee_que_thre[3]);
    return rt;
}

cs_status aal_eee_restore(void)
{
    cs_status rt = CS_E_OK;
    
    REG_FIELD_WRITE(BM_PON_EEE_ECFG, eee_on, aal_eee_on);
    REG_FIELD_WRITE(BM_PON_EEE_THRESHOLD3, eee_thres, aal_eee_thre);
    REG_FIELD_WRITE(BM_PON_EEE_QUEUE0, qthres_sel, aal_eee_que_thre[0]);
    REG_FIELD_WRITE(BM_PON_EEE_QUEUE1, qthres_sel, aal_eee_que_thre[1]);
    REG_FIELD_WRITE(BM_PON_EEE_QUEUE2, qthres_sel, aal_eee_que_thre[2]);
    REG_FIELD_WRITE(BM_PON_EEE_QUEUE3, qthres_sel, aal_eee_que_thre[3]);

    return rt;
}

cs_status aal_eee_thre_set(cs_uint8 que, cs_uint16 thre)
{
    cs_status rt = CS_E_OK;
    REG_FIELD_WRITE(BM_PON_EEE_THRESHOLD3, eee_thres, thre);
    switch(que){
        case 0: 
            REG_FIELD_WRITE(BM_PON_EEE_QUEUE0, qthres_sel, 3);
            break;
        case 1:
            REG_FIELD_WRITE(BM_PON_EEE_QUEUE1, qthres_sel, 3);
            break;
        case 2:
            REG_FIELD_WRITE(BM_PON_EEE_QUEUE2, qthres_sel, 3);
            break;
        case 3:
            REG_FIELD_WRITE(BM_PON_EEE_QUEUE3, qthres_sel, 3);
            break;
        default:
            break;

    }
    return rt;
}

cs_boolean aal_eee_stauts_get(cs_uint8 que)
{
    cs_uint8 eee_sta = 0;
    switch(que){
        case 0:
            REG_FIELD_READ(BM_PON_EEE_STAT,surpass_thres0, eee_sta);
        break;
        case 1:
            REG_FIELD_READ(BM_PON_EEE_STAT,surpass_thres1, eee_sta);
        break;
        case 2:
            REG_FIELD_READ(BM_PON_EEE_STAT,surpass_thres2, eee_sta);
        break;
        case 3:
            REG_FIELD_READ(BM_PON_EEE_STAT,surpass_thres3, eee_sta);
        break;
        default:
        break;
    }
    return (eee_sta)&0x01;
}

cs_uint32 aal_get_bm_cnt(cs_uint32 address)
{
    return __reg_value_get(address);
}

void  aal_clr_bm_drop(cs_uint8  index)
{
    if (index >= sizeof(g_fe_drp)/sizeof(g_fe_drp[0]))
    {
        return;
    }
    
    g_fe_drp[index] = 0;
    g_bm_drp[index] = 0;
    return;
}

cs_uint32 aal_bm_pon_tx_buf_get(void)
{
    cs_uint32 cnt;
    REG_READ(BM_PON_TX_BUFFER_CNTR, cnt);
    return cnt;
}

cs_status aal_bm_shp_ipg_set(cs_uint8 ipg)
{
    REG_FIELD_WRITE(BM_PORT_SHAPER_CFG, w_rate, ipg);
    return CS_E_OK;
}

cs_status aal_bm_port_buf_size_set(
    CS_IN cs_aal_port_id_t port,
    CS_IN cs_uint16        size
 )
{
    __bm_ext_cfg_msk_t cfg_msk;
    __bm_ext_cfg_t     cfg;
    cs_status rt = CS_E_OK;
  
    if(!((port == AAL_PORT_ID_GE)||(port == AAL_PORT_ID_PON))){
        rt = CS_E_PARAM;
        goto end;
    }
    if(size > 0x2380){
        rt = CS_E_PARAM;
        goto end;
    }
    memset((cs_uint8 *)&cfg, 0, sizeof(cfg));
      
    cfg_msk.u = 0;
    if(port == AAL_PORT_ID_PON){
        cfg_msk.s.pon_tx_buffer_thrsld = 1;
        cfg_msk.s.abr_us_thrsld = 1;
        cfg.pon_tx_buffer_thrsld = size*11/9;
        cfg.abr_us_thrsld = size; 
    }else{
        cfg_msk.s.ge_tx_buffer_thrsld = 1;
        cfg_msk.s.abr_ds_thrsld = 1;
        cfg.ge_tx_buffer_thrsld = size*11/9;
        cfg.abr_ds_thrsld = size; 
    }
    rt = __bm_ext_cfg_set(cfg_msk, &cfg);
    
end:
    return rt;
}

cs_status aal_bm_port_buf_size_get(
    CS_IN cs_aal_port_id_t port,
    CS_OUT cs_uint16       *size
 )
{
    __bm_ext_cfg_t     cfg;
    cs_status rt = CS_E_OK;
  
    if(!((port == AAL_PORT_ID_GE)||(port == AAL_PORT_ID_PON))){
        rt = CS_E_PARAM;
        goto end;
    }
    if(!size){
        rt = CS_E_PARAM;
        goto end;
    }
    memset((cs_uint8*)&cfg, 0, sizeof(cfg));
      
    rt = __bm_ext_cfg_get(&cfg);
    if(rt){
        goto end;
    }
    if(port == AAL_PORT_ID_GE){
        *size = cfg.abr_ds_thrsld;
    }else{
        *size = cfg.abr_us_thrsld;
    }
    
end:
    return rt;
}

cs_uint32 aal_bm_drp_cnt_get(void)
{
 return aal_get_bm_cnt(BM_GE_DROP_PKT_CNTR) + aal_get_bm_cnt(BM_PON_ABR_DROP_PKT_CNTR);
}
