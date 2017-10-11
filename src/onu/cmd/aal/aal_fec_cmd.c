#include "aal_fec.h"
#include "plat_common.h"
#include "registers.h"



void __fec_cfg_cmd_help(void)
{

    cs_printf("fec cfg cmd msk:\n");
    cs_printf("std_mode         : 0x1   \n");
    cs_printf("rx_rep_mode      : 0x2   \n");
    cs_printf("rx_preamble_mode : 0x4   \n");
    cs_printf("rx_bypass        : 0x8   \n");  
    cs_printf("rx_en            : 0x10  \n"); 
    cs_printf("tx_rep_mode      : 0x20  \n");
    cs_printf("tx_preamble_mode : 0x40  \n"); 
    cs_printf("tx_en            : 0x80  \n");
} 
void __fec_ext_cfg_cmd_help(void)
{

    cs_printf("fec ext_cfg cmd msk:\n");
    cs_printf("rx_path_clr         : 0x1    \n");
    cs_printf("tx_path_clr         : 0x2    \n");
    cs_printf("correct_cnt_clr     : 0x4    \n");
    cs_printf("uncorrect_cnt_clr   : 0x8    \n"); 
    cs_printf("stop_correct_cnt    : 0x10   \n"); 
    cs_printf("stop_uncorrect_cnt  : 0x20   \n"); 
    cs_printf("cnt_rd_clr_mode     : 0x40   \n"); 
    cs_printf("rx_status_clr       : 0x80   \n");
    cs_printf("rx_protection_window: 0x100  \n");
    cs_printf("err_threshold       : 0x200  \n");
    cs_printf("encode_mode         : 0x400  \n"); 
    cs_printf("tx_ipg_adjust       : 0x800  \n"); 
} 
void __fec_errinj_cfg_cmd_help(void)
{

    cs_printf("fec err_inj cmd msk:\n");
    cs_printf("en        : 0x1    \n");
    cs_printf("burst_en: 0x2    \n");
    cs_printf("packet_cnt: 0x4    \n");
    cs_printf("byte_cnt  : 0x8    \n");  
    cs_printf("bit_cnt   : 0x10   \n"); 
    cs_printf("tfec1_en  : 0x20   \n"); 
    cs_printf("tfec2_en  : 0x40   \n"); 
    cs_printf("data_en   : 0x80   \n");
    cs_printf("sfec_en   : 0x100  \n");
    cs_printf("parity_en : 0x200  \n");
} 

void __fec_pktsize_cfg_cmd_help(void)
{

    cs_printf("fec pkt_size cmd msk:\n");
    cs_printf("min      : 0x1    \n");
    cs_printf("max      : 0x2    \n");
} 
void __fec_latency_cfg_cmd_help(void)
{

    cs_printf("fec latency cmd msk:\n");
    cs_printf("decode   : 0x1    \n");
    cs_printf("max_pkt  : 0x2    \n");
}

void __fec_comp_cfg_cmd_help(void)
{

    cs_printf("fec err_inj cmd msk:\n");
    cs_printf("en               : 0x1    \n");
    cs_printf("mode             : 0x2    \n");
    cs_printf("direction        : 0x4    \n");
    cs_printf("counter          : 0x8    \n"); 
    cs_printf("egress           : 0x10   \n"); 
    cs_printf("ingress          : 0x20   \n"); 
    cs_printf("timestamp        : 0x40   \n"); 
    cs_printf("aes_rx_path      : 0x80   \n");
    cs_printf("churn_rx_path    : 0x100  \n");
    cs_printf("aes_non_rx_path  : 0x200  \n");
    cs_printf("churn_non_rx_path: 0x400  \n");   
} 

cs_status __fec_reg_show(void)
{
    cs_uint32 val32;
    
    volatile EPON_t *g_epon_base = &g_lynxd_reg_ptr->epon;

    cs_printf("\n*******************************FEC Registers Status Show********************************\n");
    cs_printf("FEC CFG register                        : addr 0x%08x=0x%08x, default=0x%08x", (cs_uint32)(&g_epon_base->FEC_CFG), val32 = g_epon_base->FEC_CFG.wrd, 0x0000c5f3);
    if (val32 != 0x0000c5f3) {
        cs_printf(" <---\n");
    } else {
        cs_printf("\n");
    }
    cs_printf("FEC CTRL Register                       : addr 0x%08x=0x%08x, default=0x%08x", (cs_uint32)(&g_epon_base->FEC_CTRL), val32 = g_epon_base->FEC_CTRL.wrd, 0x00000000);
    if (val32 != 0x0) {
        cs_printf(" <---\n");
    } else {
        cs_printf("\n");
    }
    cs_printf("Status Register                         : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->FEC_MISC_STATUS), g_epon_base->FEC_MISC_STATUS.wrd);
    cs_printf("Counter Control Register                : addr 0x%08x=0x%08x, default=0x%08x", (cs_uint32)(&g_epon_base->FEC_Counter_Control), val32 = g_epon_base->FEC_Counter_Control.wrd, 0x0);
    if (val32 != 0x0) {
        cs_printf(" <---\n");
    } else {
        cs_printf("\n");
    }
    cs_printf("Block Correctable Error Counter Register: addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->FEC_Block_Correctable_Error_Counter), g_epon_base->FEC_Block_Correctable_Error_Counter.wrd);
    cs_printf("Block Uncorrectable Error Counter       : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->FEC_Block_Uncorrectable_Error_Counter), g_epon_base->FEC_Block_Uncorrectable_Error_Counter.wrd);
    cs_printf("Block No Error_Counter                  : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->FEC_Block_No_Error_Counter), g_epon_base->FEC_Block_No_Error_Counter.wrd);
    cs_printf("Block Counter Register                  : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->FEC_Block_Counter), g_epon_base->FEC_Block_Counter.wrd);
    cs_printf("Corrected Byte Counter Register         : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->FEC_Corr_Byte_Counter), g_epon_base->FEC_Corr_Byte_Counter.wrd);
    cs_printf("RX CTRL Register                        : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->RXCTRL), g_epon_base->RXCTRL.wrd);
    cs_printf("TX CTRL Register                        : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->TXCTRL), g_epon_base->TXCTRL.wrd);
    cs_printf("DataPass CTRL Register                  : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->DATAPATH_BYPASS), g_epon_base->DATAPATH_BYPASS.wrd);
    cs_printf("BM CTRL Register                        : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->MPCP_CFG), g_epon_base->MPCP_CFG.wrd);
    cs_printf("FEC Define PKT size Register            : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->FEC_DEFINE_PKTSIZE), g_epon_base->FEC_DEFINE_PKTSIZE.wrd);
    cs_printf("FEC Define Latency  Register            : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->FEC_DEFINE_LATENCY), g_epon_base->FEC_DEFINE_LATENCY.wrd);
    cs_printf("Compensation Mode Register              : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->MPCP_RTT_COMPENSATION_MODE), g_epon_base->MPCP_RTT_COMPENSATION_MODE.wrd);
    cs_printf("Compensation1 Register                  : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->MPCP_RTT_COMPENSATION), g_epon_base->MPCP_RTT_COMPENSATION.wrd);
    cs_printf("Compensation2 Register                  : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->MPCP_RTT_COMPENSATION2), g_epon_base->MPCP_RTT_COMPENSATION2.wrd);
    cs_printf("AES FEC Compensation Register           : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->AES_FEC_COMP), g_epon_base->AES_FEC_COMP.wrd);
    cs_printf("AES FEC overhead Register               : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->MPCP_Configuration), g_epon_base->MPCP_Configuration.wrd);

    return CS_E_OK;
}


cs_status __fec_cfg_show(void)
{
    cs_aal_fec_cfg_t cfg;
    cs_status rt = CS_E_OK;

    rt = aal_fec_cfg_get(&cfg);
    cs_printf("\n*********FEC Configuration*********\n\n");
    cs_printf("TFEC Standard             : %s\n",   cfg.std_mode ? "2005 Std" : "2003 Std");
    cs_printf("Rx Replace Mode           : %d\n",   cfg.rx_rep_mode);
    cs_printf("Rx Preamble Mode          : %d\n",   cfg.rx_preamble_mode);
    cs_printf("Rx Bypass                 : %s\n",   cfg.rx_bypass ? "Enable" : "Disable");
    cs_printf("Rx Enable                 : %s\n",   cfg.rx_en ? "Enable" : "Disable");
    cs_printf("Tx Replace Mode           : %d\n",   cfg.tx_rep_mode);
    cs_printf("Tx Preamble Mode          : %d\n",   cfg.tx_preamble_mode);
    cs_printf("Tx Enable                 : %s\n\n", cfg.tx_en ? "Enable" : "Disable");

    return rt;
}


cs_status __fec_ext_cfg_show(void)
{
    cs_aal_fec_ext_cfg_t ext_cfg;
    cs_status rt = CS_E_OK;

    rt = aal_fec_ext_cfg_get(&ext_cfg);
    cs_printf("\n*****FEC Extension Configuration*****\n\n");
    cs_printf("Rx Path Clear             : %s\n", ext_cfg.rx_path_clr ? "Enable" : "Disable");
    cs_printf("Tx Path Clear             : %s\n", ext_cfg.tx_path_clr ? "Enable" : "Disable");
    cs_printf("Correct Counter Clear     : %s\n", ext_cfg.correct_cnt_clr ? "Enable" : "Disable");
    cs_printf("Uncorrect Counter Clear   : %s\n", ext_cfg.uncorrect_cnt_clr ? "Enable" : "Disable");
    cs_printf("Stop Correct Counter      : %s\n", ext_cfg.stop_correct_cnt ? "Enable" : "Disable");
    cs_printf("Stop Uncorrect Counter    : %s\n", ext_cfg.stop_uncorrect_cnt ? "Enable" : "Disable");
    cs_printf("Rx Counter Read Clear Mode: %d\n", ext_cfg.cnt_rd_clr_mode);
    cs_printf("Rx Status Clear           : %s\n", ext_cfg.rx_status_clr ? "Enable" : "Disable");
    cs_printf("Rx Window Protection      : %s\n", ext_cfg.rx_protection_window ? "Enable" : "Disable");
    cs_printf("Error Threshold           : %d\n", ext_cfg.err_threshold);
    cs_printf("Tx Encode Mode            : %s\n", ext_cfg.encode_mode ? "Cortina" : "Standard");
    cs_printf("Tx IPG Adjust Mode        : %d\n", ext_cfg.tx_ipg_adjust);
    cs_printf("Tx Error Send Mode        : %d\n\n", ext_cfg.err_send_mode);

    return rt;
}



cs_status __fec_errinj_cfg_show(void)
{
    cs_aal_fec_err_inj_cfg_t err_inj;
    cs_status rt = CS_E_OK;

    rt = aal_fec_err_inj_cfg_get(&err_inj);
    cs_printf("\n****FEC TX Error inject Config*****\n");
    cs_printf("Inject Enable             : %s\n", err_inj.en ? "Enable" : "Disable");
    cs_printf("Inject Burst Mode         : %s\n", err_inj.burst_en ? "Enable" : "Disable");
    cs_printf("Inject Packet Counter     : %d\n", err_inj.packet_cnt);
    cs_printf("Inject Byte Counter       : %d\n", err_inj.byte_cnt);
    cs_printf("Inject Bit Counter        : %d\n", err_inj.bit_cnt);
    cs_printf("Inject TFEC1  Enable      : %s\n", err_inj.tfec1_en ? "Enable" : "Disable");
    cs_printf("Inject TFEC2 Enable       : %s\n", err_inj.tfec2_en ? "Enable" : "Disable");
    cs_printf("Inject Data Enable        : %s\n", err_inj.data_en ? "Enable" : "Disable");
    cs_printf("Inject SFEC Enable        : %s\n", err_inj.sfec_en ? "Enable" : "Disable");
    cs_printf("Inject Parity Enable      : %s\n\n", err_inj.parity_en ? "Enable" : "Disable");

    return rt;
}


cs_status __fec_pktsize_cfg_show(void)
{
    cs_aal_fec_pktsize_cfg_t pkt_size;
    cs_status rt = CS_E_OK;

    rt = aal_fec_pktsize_cfg_get(&pkt_size);
    cs_printf("\n******FEC Packet Size Config******\n");
    cs_printf("Min Packet size           : %d\n", pkt_size.min);
    cs_printf("Max Packet size           : %d\n\n", pkt_size.max);

    return rt;
}


cs_status __fec_latency_cfg_show(void)
{
    cs_aal_fec_latency_cfg_t latency;
    cs_status rt = CS_E_OK;

    rt = aal_fec_latency_cfg_get(&latency);

    cs_printf("\n********FEC Latency Config********\n");
    cs_printf("Decode latency            : %d\n", latency.decode);
    cs_printf("Max Packet latency        : %d\n\n", latency.max_pkt);



    return rt;
}
cs_status __fec_comp_cfg_show(void)
{
    cs_aal_fec_comp_cfg_t comp;
    cs_status rt = CS_E_OK;

    rt = aal_fec_comp_cfg_get(&comp);

    cs_printf("\n********FEC Compensation Config********\n\n");
    cs_printf("Compensation Enable       : %s\n", comp.en ? "Enable" : "Disable");
    cs_printf("Compensation mode         : %d\n", comp.mode);
    cs_printf("Compensation Direction    : %s\n", comp.direction ? "Add" : "Subtract");
    cs_printf("Compensation counter      : %d\n", comp.counter);
    cs_printf("Egress compensation       : %d\n", comp.egress);
    cs_printf("Ingress compensation      : %d\n", comp.ingress);
    cs_printf("Timestamp compensation    : %d\n", comp.timestamp);
    cs_printf("AES RX PATH               : %d\n", comp.aes_rx_path);
    cs_printf("Churnning  RX PATH        : %d\n", comp.churn_rx_path);
    cs_printf("AES NON RX PATH           : %d\n", comp.aes_non_rx_path);
    cs_printf("Churning NON RX PATH      : %d\n\n", comp.churn_non_rx_path);


    return rt;
}

void __fec_cnt_show(void)
{
    cs_aal_fec_cnt_t cnt;

    aal_fec_cnt_get(&cnt);

    cs_printf("\n******FEC Counter Show******\n");
    cs_printf("Rx Blocks          : 0x%x\n", cnt.rx_blks);
    cs_printf("Rx Correct Blocks  : 0x%x\n", cnt.rx_correct_blks);
    cs_printf("Rx Uncorrect Blocks: 0x%x\n", cnt.rx_uncorrect_blks);
    cs_printf("Rx Good Blocks     : 0x%x\n", cnt.rx_good_blks);
    cs_printf("Rx Correct Bytes   : 0x%x\n", cnt.rx_correct_bytes);
    cs_printf("SFEC Counter       : 0x%x\n", cnt.rx_sfec);
    cs_printf("TFEC Counter       : 0x%x\n", cnt.rx_tfec);
    cs_printf("SPKT Counter       : 0x%x\n", cnt.rx_spkt);
    cs_printf("EPKT Counter       : 0x%x\n", cnt.rx_epkt);

}

void __fec_status_show(void)
{
    cs_aal_fec_status_t stats;

    aal_fec_status_get(&stats);

    cs_printf("\n******FEC Status Show******\n");
    cs_printf("Rx Data Correct Ack        : %s\n", stats.rx_data_corr_ack ? "Yes" : "No");
    cs_printf("Parity Buffer Full         : %s\n", stats.parity_buffer_full ? "Yes" : "No");
    cs_printf("Packet Buffer Full         : %s\n", stats.pkt_buffer_full ? "Yes" : "No");
    cs_printf("CMD buffer Full            : %s\n", stats.cmd_buffer_full ? "Yes" : "No");
    cs_printf("Code Word Buffer Full      : %s\n", stats.code_buffer_full ? "Yes" : "No");
    cs_printf("Pipe is Stall              : %s\n", stats.pipe_stuck ? "Yes" : "No");
    cs_printf("LOS TFEC1 Boundary         : %s\n", stats.los_tfec1_boundary ? "Yes" : "No");
    cs_printf("LOS TFEC2 Boundary         : %s\n", stats.los_tfec2_boundary ? "Yes" : "No");
    cs_printf("FEC Uncorrected detected   : %s\n", stats.uncorrected ? "Yes" : "No");
    cs_printf("FEC errors                 : %d\n", stats.errors);
    cs_printf("FEC Packet Status          : %d\n", stats.pkt_state);
#if 0
    cs_printf("FEC CWB FULL Counter       : %d\n", fec_cwb_full_cnt);
    cs_printf("FEC CMD FULL Counter       : %d\n", fec_cmd_full_cnt);
    cs_printf("FEC PARITY FULL Counter    : %d\n", fec_prty_full_cnt);
    cs_printf("FEC PACK FULL Counter      : %d\n", fec_pack_full_cnt);
    cs_printf("FEC CNTR HALF FULL Counter : %d\n", fec_cntr_half_full_cnt);
#endif

}


void __fec_cfg_cmd(cs_uint32 mask, cs_uint32 value)
{
    cs_aal_fec_cfg_t cfg;
    cs_aal_fec_msk_t msk;
    cs_status rt = CS_E_OK;

    msk.u32 = mask;

    if(msk.s.std_mode)
        cfg.std_mode = value;
    
    if(msk.s.rx_rep_mode)
        cfg.rx_rep_mode = value;
    
    if(msk.s.rx_preamble_mode)
        cfg.rx_preamble_mode = value;
    
    if(msk.s.rx_bypass)
        cfg.rx_bypass = value;
    
    if(msk.s.rx_en)
        cfg.rx_en = value;

    if(msk.s.tx_rep_mode)
        cfg.tx_rep_mode = value;

    if(msk.s.tx_preamble_mode)
        cfg.tx_preamble_mode = value;
    
    if(msk.s.tx_en)
        cfg.tx_en = value;

    rt |= aal_fec_cfg_set(msk, &cfg);
    if (rt != CS_E_OK) {
        cs_printf("Return value is %d.\n", rt);
        return;
    }

    __fec_cfg_show();

}


void __fec_ext_cfg_cmd(cs_uint32 mask, cs_uint32 value)
{
    cs_aal_fec_ext_cfg_t ext_cfg;
    cs_aal_fec_ext_msk_t msk;
    cs_status rt = CS_E_OK;

    msk.u32 = mask;

    if(msk.s.rx_path_clr)
        ext_cfg.rx_path_clr = value;

    if(msk.s.tx_path_clr)
        ext_cfg.tx_path_clr = value;

    if(msk.s.correct_cnt_clr)
        ext_cfg.correct_cnt_clr = value;
    
    if(msk.s.uncorrect_cnt_clr)
        ext_cfg.uncorrect_cnt_clr = value;
        
    if(msk.s.stop_correct_cnt)
        ext_cfg.stop_correct_cnt = value;

    if(msk.s.stop_uncorrect_cnt)
        ext_cfg.stop_uncorrect_cnt = value;
        
    if(msk.s.rx_protection_window)
        ext_cfg.rx_protection_window = value;
      
    if(msk.s.cnt_rd_clr_mode)
        ext_cfg.cnt_rd_clr_mode = value;
           
    if(msk.s.rx_status_clr)
        ext_cfg.rx_status_clr = value;
        
    if(msk.s.err_threshold)
        ext_cfg.err_threshold  =  value;

    if(msk.s.encode_mode)
        ext_cfg.encode_mode = value;

    if(msk.s.tx_ipg_adjust)
        ext_cfg.tx_ipg_adjust = value;
    
    if(msk.s.err_send_mode)
        ext_cfg.err_send_mode = value;

    rt |= aal_fec_ext_cfg_set(msk, &ext_cfg);
    if (rt != CS_E_OK) {
        cs_printf("Return value is %d.\n", rt);
        return;
    }
    __fec_ext_cfg_show();

}


void __fec_errinj_cfg_cmd(cs_uint32 mask, cs_uint32 value)
{
    cs_aal_fec_err_inj_cfg_t err_inj;
    cs_aal_fec_err_inj_msk_t msk;
    cs_status rt = CS_E_OK;

    msk.u32 = mask;

    if(msk.s.en)
        err_inj.en = value;
    
    if(msk.s.burst_en)
        err_inj.burst_en = value;

    if(msk.s.packet_cnt)        
        err_inj.packet_cnt = value;
    
    if(msk.s.byte_cnt)
        err_inj.byte_cnt = value;
    
    if(msk.s.bit_cnt)
        err_inj.bit_cnt = value;
    
    if(msk.s.tfec1_en)
        err_inj.tfec1_en = value;
    
    if(msk.s.tfec2_en)      
        err_inj.tfec2_en = value;
    
    if(msk.s.data_en)
        err_inj.data_en = value;
    
    if(msk.s.sfec_en)
        err_inj.sfec_en = value;
    
    if(msk.s.parity_en)
        err_inj.parity_en = value;

    rt |= aal_fec_err_inj_cfg_set(msk, &err_inj);
    if (rt != CS_E_OK) {
        cs_printf("Return value is %d.\n", rt);
        return;
    }
    __fec_errinj_cfg_show();


}


void __fec_pktsize_cfg_cmd(cs_uint32 mask, cs_uint32 value)
{
    cs_aal_fec_pktsize_cfg_t pkt_size;
    cs_aal_fec_pktsize_msk_t msk;
    cs_status rt = CS_E_OK;

    msk.u32 = mask;

    if(msk.s.min)
        pkt_size.min = value;

    if(msk.s.max)
        pkt_size.max = value;

    rt |= aal_fec_pktsize_cfg_set(msk, &pkt_size);
    if (rt != CS_E_OK) {
        cs_printf("Return value is %d.\n", rt);
        return;
    }
    __fec_pktsize_cfg_show();


}

void __fec_latency_cfg_cmd(cs_uint32 mask, cs_uint32 value)
{
    cs_aal_fec_latency_cfg_t latency;
    cs_aal_fec_latency_msk_t msk;
    cs_status rt = CS_E_OK;

    msk.u32 = mask;

    if(msk.s.decode)
        latency.decode = value;

    if(msk.s.max_pkt)
        latency.max_pkt = value;

    rt |= aal_fec_latency_cfg_set(msk, &latency);
    if (rt != CS_E_OK) {
        cs_printf("Return value is %d.\n", rt);
        return;
    }
    __fec_latency_cfg_show();

}

void __fec_comp_cfg_cmd(cs_uint32 mask, cs_uint32 value)
{
    cs_aal_fec_comp_cfg_t comp;
    cs_aal_fec_comp_msk_t msk;
    cs_status rt = CS_E_OK;

    msk.u32 = mask;


    if(msk.s.en)
        comp.en = value;

    if(msk.s.mode)
        comp.mode = value;

    if(msk.s.direction)
        comp.direction = value;

    if(msk.s.counter)
        comp.counter = value;

    if(msk.s.egress)
        comp.egress = value;
    
    if(msk.s.ingress)
        comp.ingress = value;
        
    if(msk.s.timestamp)
        comp.timestamp = value;
        
    if(msk.s.aes_rx_path)        
        comp.aes_rx_path = value;
        
    if(msk.s.churn_rx_path)        
        comp.churn_rx_path = value;

    if(msk.s.aes_non_rx_path)
        comp.aes_non_rx_path = value;

    if(msk.s.churn_non_rx_path)       
        comp.churn_non_rx_path = value;       

    rt |= aal_fec_comp_cfg_set(msk, &comp);
    if (rt != CS_E_OK) {
        cs_printf("Return value is %d.\n", rt);
        return;
    }
    __fec_comp_cfg_show();

}


void aal_fec_cmd(int argc, char **argv)
{

    cs_uint32 mask = 0;
    cs_uint32 value = 0;

    if(argc < 3){
        cs_printf("\n");
        cs_printf("aal fec cfg [mask][value]\n");
        cs_printf("aal fec ext_cfg [mask][value]\n");
        cs_printf("aal fec err_inj [mask][value]\n");
        cs_printf("aal fec pkt_size [mask][value]\n");
        cs_printf("aal fec latency [mask][value]\n");
        cs_printf("aal fec comp [mask][value]\n");
        cs_printf("aal fec cfg get\n");
        cs_printf("aal fec ext_cfg get\n");
        cs_printf("aal fec err_inj get\n");
        cs_printf("aal fec pkt_size get\n");
        cs_printf("aal fec latency get\n");
        cs_printf("aal fec comp get\n");
        cs_printf("aal fec show reg\n");
        cs_printf("aal fec show cnt\n");
    }
    else{
        mask = (cs_uint32)iros_strtol(argv[3]);
        value = (cs_uint32)iros_strtol(argv[4]);
        if (cmd_cmp(argv[2], "cfg")) {
            if(argc == 3)
                __fec_cfg_cmd_help();
            else if (cmd_cmp(argv[3], "get"))
                __fec_cfg_show();
            else{
                __fec_cfg_cmd(mask, value);
            }
        } 
        else if (cmd_cmp(argv[2], "ext_cfg")) {
            if(argc == 3)
                __fec_ext_cfg_cmd_help();
            else if (cmd_cmp(argv[3], "get"))
                __fec_ext_cfg_show();
            else
                __fec_ext_cfg_cmd(mask, value);
        } 
        else if (cmd_cmp(argv[2], "err_inj")) {
            if(argc == 3)
                __fec_errinj_cfg_cmd_help();
            else if (cmd_cmp(argv[3], "get"))
                __fec_errinj_cfg_show();
            else
                __fec_errinj_cfg_cmd(mask, value);
        } 
        else if (cmd_cmp(argv[2], "pkt_size")) {      
            if(argc == 3)
                __fec_pktsize_cfg_cmd_help();
            else if (cmd_cmp(argv[3], "get"))
                __fec_pktsize_cfg_show();
            else
                __fec_pktsize_cfg_cmd(mask, value);
        }
        else if (cmd_cmp(argv[2], "latency")) {      
            if(argc == 3)
                __fec_latency_cfg_cmd_help();
            else if (cmd_cmp(argv[3], "get"))
                __fec_latency_cfg_show();
            else
                __fec_latency_cfg_cmd(mask, value);
        }
        else if (cmd_cmp(argv[2], "comp")) {      
            if(argc == 3)
                __fec_comp_cfg_cmd_help();
            else if (cmd_cmp(argv[3], "get"))
                __fec_comp_cfg_show();
            else
                __fec_comp_cfg_cmd(mask, value);
        }
        else if (cmd_cmp(argv[2], "show")) {
            if (cmd_cmp(argv[3], "reg"))
                __fec_reg_show();
            else if (cmd_cmp(argv[3], "cnt"))
                __fec_cnt_show();              
                
        }
        else
            cs_printf("error cmd!!\n");
    }
    
    return;
}


