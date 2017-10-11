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
#include "plat_common.h"
#include "iros_config.h"
#include "aal_flow.h"
#include "aal_util.h"

cs_status aal_flow_dot1p_mapping_set(cs_aal_port_id_t port_id, cs_aal_flow_mapping_msk_t *msk,cs_aal_flow_mapping_t *mapping)
{
    cs_status rt = CS_E_OK;
    
    if(!mapping){
        rt = CS_E_PARAM;
        goto end; 
    }

    if(msk->s.dot1p){
        REG_WRITE_PORT(FE_LE_PR_RX_8021P_TX_8021P_MAP,port_id,mapping->dot1p.u);
    }
    if(msk->s.cos){        
        REG_WRITE_PORT(FE_LE_PR_RX_8021P_TX_COS_MAP,port_id,mapping->cos.u);
    }
    if(msk->s.flow_id){        
        REG_WRITE_PORT(FE_LE_PR_RX_8021P_FLOW_ID_MAP_0,port_id,mapping->flow_id.flow0.u);
        REG_WRITE_PORT(FE_LE_PR_RX_8021P_FLOW_ID_MAP_1,port_id,mapping->flow_id.flow1.u);
    }    
    if(msk->s.dscp){        
        REG_WRITE_PORT(FE_LE_PR_RX_8021P_TX_DSCP_MAP_0,port_id,mapping->dscp.dscp_tc0.u);        
        REG_WRITE_PORT(FE_LE_PR_RX_8021P_TX_DSCP_MAP_1,port_id,mapping->dscp.dscp_tc1.u);
    }    
    if(msk->s.tc){
        REG_WRITE_PORT(FE_LE_PR_RX_8021P_TX_TC_MAP_0,port_id,mapping->tc.dscp_tc0.u);        
        REG_WRITE_PORT(FE_LE_PR_RX_8021P_TX_TC_MAP_1,port_id,mapping->tc.dscp_tc1.u);
    }
  end:
    return rt;
}

cs_status aal_flow_dot1p_mapping_get(cs_aal_port_id_t port_id, cs_aal_flow_mapping_t *mapping)
{
    cs_status rt = CS_E_OK;

    if(!mapping){
        rt = CS_E_PARAM;
        goto end; 
    }

    REG_READ_PORT(FE_LE_PR_RX_8021P_TX_8021P_MAP,port_id,mapping->dot1p.u);
    REG_READ_PORT(FE_LE_PR_RX_8021P_TX_COS_MAP,port_id,mapping->cos.u);
    REG_READ_PORT(FE_LE_PR_RX_8021P_FLOW_ID_MAP_0,port_id,mapping->flow_id.flow0.u);
    REG_READ_PORT(FE_LE_PR_RX_8021P_FLOW_ID_MAP_1,port_id,mapping->flow_id.flow1.u);
    REG_READ_PORT(FE_LE_PR_RX_8021P_TX_DSCP_MAP_0,port_id,mapping->dscp.dscp_tc0.u);        
    REG_READ_PORT(FE_LE_PR_RX_8021P_TX_DSCP_MAP_1,port_id,mapping->dscp.dscp_tc1.u);
    REG_READ_PORT(FE_LE_PR_RX_8021P_TX_TC_MAP_0,port_id,mapping->tc.dscp_tc0.u);        
    REG_READ_PORT(FE_LE_PR_RX_8021P_TX_TC_MAP_1,port_id,mapping->tc.dscp_tc1.u);
    end:
      return rt;
}

cs_status aal_flow_cos_mapping_set(cs_aal_port_id_t port_id, cs_aal_flowid_map_t *mapping)
{
    cs_status rt = CS_E_OK;
    
     if(!mapping){
         rt = CS_E_PARAM;
         goto end; 
     }

    REG_WRITE_PORT(FE_LE_PR_COS_FLOW_ID_MAP_0,port_id,mapping->flow0.u);
    REG_WRITE_PORT(FE_LE_PR_COS_FLOW_ID_MAP_1,port_id,mapping->flow1.u);  
end:
    return rt;
}

cs_status aal_flow_cos_mapping_get(cs_aal_port_id_t port_id, cs_aal_flowid_map_t *mapping)
{
    cs_status rt = CS_E_OK;
    
     if(!mapping){
         rt = CS_E_PARAM;
         goto end; 
     }
    
    REG_READ_PORT(FE_LE_PR_COS_FLOW_ID_MAP_0,port_id,mapping->flow0.u);
    REG_READ_PORT(FE_LE_PR_COS_FLOW_ID_MAP_1,port_id,mapping->flow1.u);  
end:
    return rt;
}

cs_status aal_flow_dscp_mapping_set(cs_aal_port_id_t port_id,cs_aal_dscp_map_t *msk, cs_aal_dscp_map_t *mapping)
{
    cs_uint8 index;
    cs_status rt = CS_E_OK;
    
     if(!mapping){
         rt = CS_E_PARAM;
         goto end; 
     }

    for(index= 0; index<AAL_MAX_DSCP; index++){
        if(msk->s.dscp_map){
            msk->s.dscp_map = 0x3f;
        }
        if(msk->s.dot1p_map){
            msk->s.dot1p_map = 0x7;
        }
        if(msk->s.cos_map){
            msk->s.cos_map = 0x7;
        }
        if(msk->s.flowid_map){
            msk->s.flowid_map = 0x1f;
        }       
        REG_INDIR_WRITE_2DATA(port_id,FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_ACCESS,index,msk->u,mapping[index].u,0,0);
    }
end:
    return rt;
}

cs_status aal_flow_dscp_mapping_get(cs_aal_port_id_t port_id, cs_aal_dscp_map_t *mapping)
{
    cs_uint8 index;
    cs_uint32 temp_data;
    cs_status rt = CS_E_OK;
    
    if(!mapping){
        rt = CS_E_PARAM;
        goto end; 
    }
    
    for(index= 0; index<AAL_MAX_DSCP; index++){
        REG_INDIR_READ_2DATA(port_id,FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_ACCESS,index,mapping[index].u,temp_data);
    }
end:
    return rt;
}

cs_status aal_flow_tc_mapping_set(cs_aal_port_id_t port_id,cs_aal_dscp_map_t *msk, cs_aal_dscp_map_t *mapping)
{
    cs_uint8 index;
    cs_uint32 mask,mask1, data1, data2;
    cs_aal_dscp_map_t temp_mask;
    cs_status rt = CS_E_OK;

    
    if(NULL == msk || NULL == mapping){
        rt = CS_E_PARAM;
        goto end; 
    }
    mask1 = 0;
    temp_mask.u = 0;
        if(msk->s.dscp_map){
        temp_mask.s.dscp_map = 0x3f;
    }
    if(msk->s.dot1p_map){
        temp_mask.s.dot1p_map = 0x7;
    }
    if(msk->s.cos_map){
        temp_mask.s.cos_map = 0x7;
    }
    if(msk->s.flowid_map){
        temp_mask.s.flowid_map = 0x7;
        mask1 = 3;
    }
    
    mask = temp_mask.u << 17;
    // Write each DSCP mapping table
    for(index= 0; index<AAL_MAX_DSCP; index++){

        data1 = ((mapping[index].u) << 17);
        
        data2 = (mapping[index].s.flowid_map >> 3) & 0x3;

        REG_INDIR_WRITE_2DATA(port_id,FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_ACCESS,index,mask,data1,mask1,data2);
    }
end:
    
    return rt;
}

cs_status aal_flow_tc_mapping_get(cs_aal_port_id_t port_id, cs_aal_dscp_map_t *mapping)
{
    cs_uint8 index;
    cs_uint32 temp_data = 0;
    cs_status rt = CS_E_OK;
    
    if(!mapping){
        rt = CS_E_PARAM;
        goto end; 
    }
    
    for(index= 0; index<AAL_MAX_DSCP; index++){
        REG_INDIR_READ_2DATA(port_id,FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_ACCESS,index,mapping[index].u,temp_data);
        mapping[index].u >>= 17;
        mapping[index].s.flowid_map |= ((temp_data&0x3)<<3);
    }    
end:
        return rt;
}

cs_status aal_flow_rate_limit_set(cs_aal_rate_limit_type_t flow_id,cs_aal_rate_limit_msk_t *msk, cs_aal_rate_limit_t *cfg)
{
    TE_PIR_BUCKET_DATA0_t bkt_data0, bkt_data0_msk;
    TE_PIR_BUCKET_DATA1_t bkt_data1, bkt_data1_msk;
    TE_FLOW_CNT_COUNTER1_t flow_cnt, flow_cnt_msk;
    cs_status rt = CS_E_OK;
    
    if(!cfg){
        rt = CS_E_PARAM;
        goto end;
    }
    
    bkt_data0_msk.wrd = 0;
    bkt_data1_msk.wrd = 0;
    bkt_data0.wrd = 0;
    bkt_data1.wrd = 0;

    if(msk->s.enable){
        bkt_data1_msk.bf.enb = 0x3;
        if(cfg->ctrl.s.enable){
            bkt_data1.bf.enb = 0;
        }else{
            bkt_data1.bf.enb = 2;// PASS ALL
        }
    }
    
    if(msk->s.rlmode){
        bkt_data1_msk.bf.rlmode = 1;
        bkt_data1.bf.rlmode = cfg->ctrl.s.rlmode;
    }
    
    if(msk->s.cbs){
        bkt_data1_msk.bf.cbs = 0x3ff;
        bkt_data1.bf.cbs = cfg->cbs;
        /*walk around, if cbs = 0, drop all packets*/
        bkt_data1_msk.bf.enb = 0x3;
        if(cfg->cbs == 0){
            bkt_data1.bf.enb = 1; // DROP ALL
        }else if(cfg->ctrl.s.enable == 0){
            bkt_data1.bf.enb = 2;//  pass all         
        }else{
            bkt_data1.bf.enb = 0;//  Normally         
        }
    }
    if(msk->s.rate){
        bkt_data1_msk.bf.rate_m = 0x1ff;
        bkt_data1.bf.rate_m = ((cfg->rate/1000)>>1)&0x1ff;
        bkt_data0_msk.bf.rate_m = 0x1;
        bkt_data0.bf.rate_m = (cfg->rate/1000)&1;
        bkt_data0_msk.bf.rate_k = 0x3ff;
        bkt_data0.bf.rate_k = (cfg->rate%1000);
    }

    REG_INDIR_WRITE_2DATA(0,TE_PIR_BUCKET_ACCESS,flow_id,bkt_data0_msk.wrd,bkt_data0.wrd,bkt_data1_msk.wrd,bkt_data1.wrd);

    if(msk->s.acc_msk_a){
        REG_FIELD_WRITE(TE_ACC_FLOW_A_MSK,mask,cfg->acc_msk_a);
    }

    
    if(msk->s.acc_msk_b){
        REG_FIELD_WRITE(TE_ACC_FLOW_B_MSK,mask,cfg->acc_msk_b);
    }

    
    if(msk->s.acc_msk_c){
        REG_FIELD_WRITE(TE_ACC_FLOW_C_MSK,mask,cfg->acc_msk_c);
    }

    if(msk->s.acc_msk_d){
        REG_FIELD_WRITE(TE_ACC_FLOW_D_MSK,mask,cfg->acc_msk_d);
    }
    
    flow_cnt_msk.wrd = 0;
    flow_cnt_msk.bf.cntmode = 1;

    if(msk->s.fwdcnt){
        flow_cnt.bf.cntmode = cfg->ctrl.s.fwdcntmod;
        if(flow_id >= AAL_RATE_LIMIT_ACC_FLOW_A){
            REG_INDIR_WRITE_2DATA(0,TE_FLOW_CNT_ACCESS,AAL_RATE_LIMIT_ACC_FLOW_A*3+(flow_id-AAL_RATE_LIMIT_ACC_FLOW_A)*2,0,0,flow_cnt_msk.wrd,flow_cnt.wrd);    
        }else{
            REG_INDIR_WRITE_2DATA(0,TE_FLOW_CNT_ACCESS,flow_id*3,0,0,flow_cnt_msk.wrd,flow_cnt.wrd);    
        }
    }
    if(msk->s.pircnt){
        flow_cnt.bf.cntmode = cfg->ctrl.s.pircntmod;
        
        if(flow_id >= AAL_RATE_LIMIT_ACC_FLOW_A){
            REG_INDIR_WRITE_2DATA(0,TE_FLOW_CNT_ACCESS,AAL_RATE_LIMIT_ACC_FLOW_A*3+(flow_id-AAL_RATE_LIMIT_ACC_FLOW_A)*2+1,0,0,flow_cnt_msk.wrd,flow_cnt.wrd);    
        }else{
            REG_INDIR_WRITE_2DATA(0,TE_FLOW_CNT_ACCESS,flow_id*3+1,0,0,flow_cnt_msk.wrd,flow_cnt.wrd);    
        }
    }
    if(msk->s.tdcnt){
        flow_cnt.bf.cntmode = cfg->ctrl.s.tdcntmod;
        REG_INDIR_WRITE_2DATA(0,TE_FLOW_CNT_ACCESS,flow_id*3+2,0,0,flow_cnt_msk.wrd,flow_cnt.wrd);    
    }

end:    
    return rt;     
}

cs_status aal_flow_rate_limit_get(cs_aal_rate_limit_type_t flow_id,cs_aal_rate_limit_msk_t *msk, cs_aal_rate_limit_t *cfg)
{
    TE_PIR_BUCKET_DATA0_t bkt_data0;
    TE_PIR_BUCKET_DATA1_t bkt_data1;    
    TE_FLOW_CNT_COUNTER1_t flow1_cnt;
    TE_FLOW_CNT_COUNTER0_t flow0_cnt;    
    cs_status rt = CS_E_OK;

    if(!cfg){
        rt = CS_E_PARAM;
        goto end;
    }

    REG_INDIR_READ_2DATA(0,TE_PIR_BUCKET_ACCESS,flow_id,bkt_data0.wrd,bkt_data1.wrd);

    if(bkt_data1.bf.enb == 0){
        cfg->ctrl.s.enable = 1;
    }else if(bkt_data1.bf.enb == 1){
        cfg->ctrl.s.enable = 0;
    } 
    cfg->ctrl.s.rlmode = bkt_data1.bf.rlmode;
    if(msk->s.cbs){
        cfg->cbs = bkt_data1.bf.cbs;
    }
    
    if(msk->s.rate){
        cfg->rate = ((bkt_data1.bf.rate_m<<1)|bkt_data0.bf.rate_m)*1000 + bkt_data0.bf.rate_k; 
    }
    
    if(msk->s.tbc){
        cfg->tbc = bkt_data0.bf.tbc;
    }
    
    if(msk->s.acc_msk_a){
        REG_FIELD_READ(TE_ACC_FLOW_A_MSK,mask,cfg->acc_msk_a);
    }
    
    if(msk->s.acc_msk_b){
        REG_FIELD_READ(TE_ACC_FLOW_B_MSK,mask,cfg->acc_msk_b);    
    }
    if(msk->s.acc_msk_c){
        REG_FIELD_READ(TE_ACC_FLOW_C_MSK,mask,cfg->acc_msk_c);
    }
    if(msk->s.acc_msk_d){
        REG_FIELD_READ(TE_ACC_FLOW_D_MSK,mask,cfg->acc_msk_d);
    } 
    
    if(msk->s.fwdcnt){
        if(flow_id >= AAL_RATE_LIMIT_ACC_FLOW_A){
            REG_INDIR_READ_2DATA(0,TE_FLOW_CNT_ACCESS,AAL_RATE_LIMIT_ACC_FLOW_A*3+(flow_id-AAL_RATE_LIMIT_ACC_FLOW_A)*2,flow0_cnt.wrd,flow1_cnt.wrd);
        }else{
            REG_INDIR_READ_2DATA(0,TE_FLOW_CNT_ACCESS,flow_id*3,flow0_cnt.wrd,flow1_cnt.wrd);
        }
        cfg->ctrl.s.fwdcntmod = flow1_cnt.bf.cntmode;
        cfg->fwdcnt = flow0_cnt.bf.counter|(((cs_uint64)flow1_cnt.bf.counter<<32));
    }    
    
    if(msk->s.pircnt){
        
        if(flow_id >= AAL_RATE_LIMIT_ACC_FLOW_A){
            REG_INDIR_READ_2DATA(0,TE_FLOW_CNT_ACCESS,AAL_RATE_LIMIT_ACC_FLOW_A*3+(flow_id-AAL_RATE_LIMIT_ACC_FLOW_A)*2+1,flow0_cnt.wrd,flow1_cnt.wrd);
        }else{
            REG_INDIR_READ_2DATA(0,TE_FLOW_CNT_ACCESS,flow_id*3+1,flow0_cnt.wrd,flow1_cnt.wrd);
        }
        cfg->ctrl.s.pircntmod = flow1_cnt.bf.cntmode;
        cfg->pircnt = flow0_cnt.bf.counter|((cs_uint64)flow1_cnt.bf.counter<<32);
    }    

    if(msk->s.tdcnt){
        REG_INDIR_READ_2DATA(0,TE_FLOW_CNT_ACCESS,flow_id*3+2,flow0_cnt.wrd,flow1_cnt.wrd);
        cfg->ctrl.s.tdcntmod = flow1_cnt.bf.cntmode;
        cfg->tdcnt = flow0_cnt.bf.counter|((cs_uint64)flow1_cnt.bf.counter<<32);
    }    
end:    
    return rt;     
}

cs_status aal_flow_storm_ctrl_set(cs_aal_port_id_t port_id, cs_aal_storm_ctrl_mod_t mod)
{
    cs_uint8 msk;
    cs_status rt = CS_E_OK;

    switch(mod){
        case AAL_STORM_CTRL_MODE_BC:
            msk = 2;
            break;
        case AAL_STORM_CTRL_MODE_BC_UMC:
            msk = 3;
            break;
        case AAL_STORM_CTRL_MODE_BC_UMC_UUC:
            msk = 7;
            break;
        case AAL_STORM_CTRL_MODE_DIS:
            msk = 0;
            break;
            
        default :
            rt = CS_E_PARAM;
            goto end;
    }
    switch(port_id){
        case AAL_PORT_ID_GE:
            REG_FIELD_WRITE(TE_GE_SPEC_CFG,spec_mask1,msk);
            break;
        case AAL_PORT_ID_PON:           
            REG_FIELD_WRITE(TE_PON_SPEC_CFG,spec_mask1,msk);
            break;
        case AAL_PORT_ID_CPU:
            REG_FIELD_WRITE(TE_VOIP_SPEC_CFG,spec_mask1,msk);
            break;
        default:
            rt = CS_E_PARAM;
            goto end;
    }
    
 end:    
    return rt;     
}

cs_status aal_flow_storm_ctrl_get(cs_aal_port_id_t port_id, cs_aal_storm_ctrl_mod_t *mod)
{
    cs_uint32 msk;
    cs_status rt = CS_E_OK;
    
    if( !mod){        
        rt = CS_E_PARAM;
        goto end;
    }  
    switch(port_id){
        case AAL_PORT_ID_GE:
            REG_FIELD_READ(TE_GE_SPEC_CFG,spec_mask1,msk);
            break;
        case AAL_PORT_ID_PON:           
            REG_FIELD_READ(TE_PON_SPEC_CFG,spec_mask1,msk);
            break;
        case AAL_PORT_ID_CPU:
            REG_FIELD_READ(TE_VOIP_SPEC_CFG,spec_mask1,msk);
            break;
        default:            
            rt = CS_E_PARAM;
            goto end;
    }
    
    switch(msk){
        case 0:
            *mod = AAL_STORM_CTRL_MODE_DIS;
            break;           
        case 2:
            *mod = AAL_STORM_CTRL_MODE_BC;
            break;
        case 3:
            *mod = AAL_STORM_CTRL_MODE_BC_UMC;
            break;
        case 7:
            *mod = AAL_STORM_CTRL_MODE_BC_UMC_UUC;
            break;
        default :
            *mod = AAL_STORM_CTRL_MODE_DIS;
    }
    
 end:    
    return rt;                        
}

cs_status aal_flow_shaper_set(cs_aal_port_id_t port_id,cs_aal_rate_limit_t *rate)
{
    TE_GE_SHP_CTRL_t shaper;
    cs_status rt = CS_E_OK;

    if (!rate) {
        goto shaper_end;
    }
    if (((rate->rate > AAL_MAX_RATE) || (rate->cbs > AAL_MAX_CBS)) && (rate->ctrl.s.enable == 1)) {
       // cs_printf("error! rate %d, CBS %d\n", rate->rate, rate->cbs);       
       rt = CS_E_PARAM;
        goto shaper_end;
    }
    
    if(rate->ctrl.s.enable == 1){
        shaper.bf.enb = 0;
    }else if(rate->ctrl.s.enable == 0){
        shaper.bf.enb = 2;
    }

    shaper.bf.rate_m = rate->rate/1000;
    shaper.bf.rate_k = rate->rate%1000;
    shaper.bf.cbs    = rate->cbs;
    
    switch(port_id){
        case AAL_PORT_ID_GE:
            REG_WRITE(TE_GE_SHP_CTRL, shaper.wrd);        
            break;
        case AAL_PORT_ID_PON:
            REG_WRITE(TE_MIR_SHP_CTRL, shaper.wrd);        
            break;
        case AAL_PORT_ID_CPU:
            REG_WRITE(TE_VOIP_SHP_CTRL, shaper.wrd);        
            break;
        default:
            rt = CS_E_PARAM;
            goto shaper_end;
    }
 shaper_end:   
    return rt;

}

cs_status aal_flow_shaper_get(cs_aal_port_id_t port_id,cs_aal_rate_limit_t *rate)
{
    TE_GE_SHP_CTRL_t shaper;
    TE_GE_SHP_TIM_STS_t tbc;
    cs_uint32 rt = CS_E_OK;

    if (!rate) {
        rt = CS_E_PARAM;
        goto shaper_end;
    }
        
    switch(port_id){
        case AAL_PORT_ID_GE:
            REG_READ(TE_GE_SHP_CTRL, shaper.wrd);        
            REG_READ(TE_GE_SHP_TIM_STS, tbc.wrd);        
            break;
        case AAL_PORT_ID_PON:
            REG_READ(TE_MIR_SHP_CTRL, shaper.wrd);        
            REG_READ(TE_MIR_SHP_TIM_STS, tbc.wrd);        
            break;
        case AAL_PORT_ID_CPU:
            REG_READ(TE_VOIP_SHP_CTRL, shaper.wrd);        
            REG_READ(TE_VOIP_SHP_TIM_STS, tbc.wrd);        
            break;
        default:
            rt = CS_E_PARAM;
            goto shaper_end;
    }
    
    if( shaper.bf.enb == 0){
        rate->ctrl.s.enable = 1;
    }else if(shaper.bf.enb == 2){
        rate->ctrl.s.enable = 0;
    }
    rate->rate = shaper.bf.rate_m*1000 + shaper.bf.rate_k;
    rate->cbs = shaper.bf.cbs;
    rate->tbc = tbc.bf.tbc;
 shaper_end:   
    return rt;
}

cs_status aal_flow_te_ctrl_set(cs_aal_te_ctrl_mask_t *msk, cs_aal_te_ctrl_t *cfg)
{
    TE_CTRL_t te_ctrl,te_ctrl_msk;
    cs_status rt = CS_E_OK;

    if((!msk)||(!cfg)){
        rt = CS_E_PARAM;
        goto end;
    }

    te_ctrl_msk.wrd = 0;

    if(msk->s.pir_wrate){
        te_ctrl_msk.bf.pir_wrate = 0x1f;
        te_ctrl.bf.pir_wrate = cfg->pir_wrate;
    }

    if(msk->s.shaper_wrate){
        te_ctrl_msk.bf.shaper_wrate = 0x1f;
        te_ctrl.bf.shaper_wrate = cfg->shaper_wrate;
    }

    if(msk->s.mii_cpup_en){
        te_ctrl_msk.bf.mii_cpup_en = 0x1;
        te_ctrl.bf.mii_cpup_en = cfg->mii_cpup_en;
    }

    if(msk->s.cpu_cos7_td_drop_dis){
        te_ctrl_msk.bf.cpu_cos7_td_drop_dis = 0x1;
        te_ctrl.bf.cpu_cos7_td_drop_dis = cfg->cpu_cos7_td_drop_dis;
    }
    
    REG_MASK_WRITE(TE_CTRL, te_ctrl_msk.wrd,te_ctrl.wrd);
end:
    return rt;
}

cs_status aal_flow_te_ctrl_get(cs_aal_te_ctrl_t *cfg)
{
    TE_CTRL_t te_ctrl;
    cs_status rt = CS_E_OK;
    if(!cfg){        
        rt = CS_E_PARAM;
         goto end;
    }

    REG_READ(TE_CTRL,te_ctrl.wrd);

    cfg->pir_wrate = te_ctrl.bf.pir_wrate;

    cfg->shaper_wrate = te_ctrl.bf.shaper_wrate;

    cfg->mii_cpup_en = te_ctrl.bf.mii_cpup_en;
    
    cfg->cpu_cos7_td_drop_dis = te_ctrl.bf.cpu_cos7_td_drop_dis;

end:
    return rt;
}

cs_status flow_init(void)
{
    TE_CTRL_t te_ctrl,te_ctrl_msk;
    te_ctrl_msk.wrd = 0;
    te_ctrl_msk.bf.addtok_en = 1;
    te_ctrl_msk.bf.subtok_en = 1;
    te_ctrl_msk.bf.qos_gen_en = 1;
    te_ctrl_msk.bf.pir_wrate = 0x1f;
    te_ctrl_msk.bf.shaper_wrate = 0x1f;

    te_ctrl.bf.addtok_en = 1;
    te_ctrl.bf.subtok_en = 1;
    te_ctrl.bf.qos_gen_en = 1;
    te_ctrl.bf.pir_wrate = 20;
    te_ctrl.bf.shaper_wrate = 20;
    
    REG_MASK_WRITE(TE_CTRL, te_ctrl_msk.wrd,te_ctrl.wrd);
  
    
    return CS_E_OK;
}

cs_status aal_flow_control_set(cs_aal_flow_control_t *cfg)
{
    GE_ETH_MAC_CONFIG0_t mac_cfg, mac_cfg_mask;    
    TE_CTRL_t te_ctrl,te_ctrl_msk;
    BM_SHARED_BUFFER_THRESHOLD_t port_buf, port_buf_msk;
    cs_status rt = CS_E_OK;
    
    if(!cfg){
        rt = CS_E_PARAM;
         goto end;
    }
        
    te_ctrl_msk.wrd = 0;
    te_ctrl_msk.bf.ge_pause_frame_en = cfg->fc_msk.s.te_pause;
    mac_cfg_mask.wrd = 0;
    mac_cfg_mask.bf.tx_flow_disable = 1;      
    mac_cfg_mask.bf.rx_flow_disable = 1;
    mac_cfg_mask.bf.tx_auto_xon = 1;
    if(cfg[0].fc_msk.s.fifo){
        if(cfg[0].fifo_fc.s.en||cfg[1].fifo_fc.s.en||cfg[2].fifo_fc.s.en||cfg[3].fifo_fc.s.en){
            mac_cfg.bf.tx_auto_xon = 1;
            mac_cfg.bf.tx_flow_disable = 0;
            mac_cfg.bf.rx_flow_disable = 0;
        }else{
            mac_cfg.bf.tx_auto_xon = 0;
            mac_cfg.bf.tx_flow_disable = 1;
            mac_cfg.bf.rx_flow_disable = 1;        
        }        
        REG_WRITE(BM_US_Q0_FLOW_CTRL, cfg[0].fifo_fc.u);
        REG_WRITE(BM_US_Q1_FLOW_CTRL, cfg[1].fifo_fc.u);
        REG_WRITE(BM_US_Q2_FLOW_CTRL, cfg[2].fifo_fc.u);
        REG_WRITE(BM_US_Q3_FLOW_CTRL, cfg[3].fifo_fc.u);        
        te_ctrl.bf.ge_pause_frame_en = 0;
    }
    if(cfg[0].fc_msk.s.mem){
        if(cfg[0].mem_fc.tx_en){
            mac_cfg.bf.tx_flow_disable = 0;            
            mac_cfg.bf.tx_auto_xon = 1;
        }else{
            mac_cfg.bf.tx_flow_disable = 1;            
            mac_cfg.bf.tx_auto_xon = 0;
        }
        
        if(cfg[0].mem_fc.rx_en){
            mac_cfg.bf.rx_flow_disable = 0;
        }else{
            mac_cfg.bf.rx_flow_disable = 1;
        }
        
        port_buf_msk.wrd = 0;
        port_buf_msk.bf.l_thrsld = 0x3fff;
        port_buf.bf.l_thrsld = 0x1380;
        port_buf_msk.bf.u_thrsld = 0x3fff;
        port_buf.bf.u_thrsld = 0x1d00;
        te_ctrl.bf.ge_pause_frame_en = 1;
        REG_MASK_WRITE(BM_SHARED_BUFFER_THRESHOLD, port_buf_msk.wrd,port_buf.wrd);

    }
    REG_MASK_WRITE(GE_ETH_MAC_CONFIG0,mac_cfg_mask.wrd, mac_cfg.wrd);
    
    
    REG_MASK_WRITE(TE_CTRL, te_ctrl_msk.wrd,te_ctrl.wrd);
end:
    return rt;
}

cs_status aal_flow_control_get(cs_aal_flow_control_t *cfg)
{
    cs_status rt = CS_E_OK;
    if(!cfg){
         rt = CS_E_PARAM;
         goto end;
    }
    REG_READ(BM_US_Q0_FLOW_CTRL, cfg[0].fifo_fc.u);
    REG_READ(BM_US_Q1_FLOW_CTRL, cfg[1].fifo_fc.u);
    REG_READ(BM_US_Q2_FLOW_CTRL, cfg[2].fifo_fc.u);
    REG_READ(BM_US_Q3_FLOW_CTRL, cfg[3].fifo_fc.u);
end:
    return rt;
}

cs_status aal_rx_flow_control_set(cs_boolean enable)
{
    REG_FIELD_WRITE(GE_ETH_MAC_CONFIG0, rx_flow_disable, enable ? 0 : 1);            
    return CS_E_OK;
}

cs_status aal_rx_flow_control_get(cs_boolean *enable)
{
    cs_boolean rx_disable;
    if(NULL == enable){
        return CS_E_PARAM;
    }
    REG_FIELD_READ(GE_ETH_MAC_CONFIG0, rx_flow_disable, rx_disable);
    *enable = rx_disable ? 0 : 1;
    
    return CS_E_OK;
}

