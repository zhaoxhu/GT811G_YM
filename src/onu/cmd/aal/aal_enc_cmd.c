#include "aal_enc.h"
#include "plat_common.h"
#include "registers.h"


cs_boolean enc_dbg;
void __enc_cfg_show(void)
{
    cs_aal_enc_cfg_t cfg;

    aal_enc_cfg_get(&cfg);

    cs_printf("\n******Decryption/Encryption Configuration******\n\n");
    cs_printf("Decryption Mode          : %d\n",  cfg.mode);
    cs_printf("Decryption Enable        : %s\n\n", cfg.en ? "Enable" : "Disable");
    cs_printf("OAM OUI                  : 0x%x\n",  cfg.oam_oui);
    cs_printf("OAM flag                 : 0x%x\n",  cfg.oam_flag);
    cs_printf("OAM Rpt Length           : 0x%x\n",  cfg.extra_oam_rpt_len);
    cs_printf("Single Churning Enable   : %s\n\n",  cfg.churn_mode ? "Enable" : "Disable");
    cs_printf("key generated Mode       : %s\n\n",  cfg.key_gen_mode ? "SW" : "HW");
    cs_printf("key Exchange  Mode       : %s\n\n",  cfg.key_exchange_mode ? "HW" : "SW");

}

void __enc_tx_cfg_show(void)
{
    cs_aal_enc_tx_cfg_t tx_cfg;

    aal_enc_tx_cfg_get(&tx_cfg);
    cs_printf("\n******Encryption TX Configuration******\n");
    cs_printf("Encryption Enable        : %s\n",  tx_cfg.en ? "Enable" : "Disable");
    cs_printf("Encryption Trigger cnt   : %d\n",  tx_cfg.threshold);
    cs_printf("Encryption Index Select  : %s\n",  tx_cfg.key_idx_sel_mode ? "From CSR" : "From Rx Decrypted");
    cs_printf("Encryption Index         : %d\n",  tx_cfg.key_idx);
    cs_printf("Key index notify         : %s\n",  tx_cfg.key_idx_notify_en ? "Enable" : "Disable");
    cs_printf("CRC8 Regeneration Enable : %s\n",  tx_cfg.crc8_regen_en ? "Enable" : "Disable");
    cs_printf("CRC8 ReverseIn           : %s\n",  tx_cfg.crc8_in_reverse_en ? "Enable" : "Disable");
    cs_printf("CRC8 ReverseOut          : %s\n\n",tx_cfg.crc8_out_reverse_en ? "Enable" : "Disable");
    cs_printf("Encryption RCC B0 Select : %d\n",  tx_cfg.rcc_b0_sel_mode);
    cs_printf("Encryption key Select    : %d\n",  tx_cfg.key_sel_mode);


}

void __enc_timer_show(void)
{
    cs_aal_enc_timer_cfg_t timer;

    aal_enc_timer_cfg_get(&timer);
    cs_printf("\n******Encryption Timer Configuration******\n");
    cs_printf("Rx Timer Adjust Direction: %s\n",  timer.rx_dir ? "Add" : "Subtract");
    cs_printf("Rx Timer Adjust Value    : %d\n",  timer.rx_delta);
    cs_printf("Tx Timer Adjust Direction: %s\n",  timer.tx_dir ? "Add" : "Subtract");
    cs_printf("Timer Adjust Value       : %d\n",  timer.tx_delta);
    cs_printf("AES Timer2               : 0x%x\n", timer.timer_hi16);
    cs_printf("JAES Timer               : 0x%x\n\n", timer.jaes_timer);

}

void  __enc_key_show(void)
{
    cs_aal_enc_key_t uc_key0, uc_key1, hw_key1, hw_key0;
    cs_uint8 *key = NULL;
    cs_uint32 i;

    aal_enc_key_get(0, &uc_key0);
    aal_enc_key_get(1, &uc_key1);
    aal_enc_key_hw_get(0, &hw_key0);
    aal_enc_key_hw_get(1, &hw_key1);

    cs_printf("\n******UC Key Status******\n");
    key = (cs_uint8 *)uc_key0;
    for(i=0; i<4; i++)
        cs_printf("UC Key Index 0 Key %d: 0x%02x%02x%02x%02x\n",  i, key[i*4+3], key[i*4+2], key[i*4+1], key[i*4]);
    key = (cs_uint8 *)uc_key1;
    for(i=0; i<4; i++)
        cs_printf("UC Key Index 1 Key %d: 0x%02x%02x%02x%02x\n",  i, key[i*4+3], key[i*4+2], key[i*4+1], key[i*4]);
    cs_printf("\n");

    cs_printf("*HW Generated Key Status*\n");
    key = (cs_uint8 *)hw_key0;
    for(i=0; i<4; i++)
        cs_printf("UC Key Index 0 Key %d: 0x%02x%02x%02x%02x\n",  i, key[i*4+3], key[i*4+2], key[i*4+1], key[i*4]);
    key = (cs_uint8 *)hw_key1;
    for(i=0; i<4; i++)
        cs_printf("UC Key Index 0 Key %d: 0x%02x%02x%02x%02x\n",  i, key[i*4+3], key[i*4+2], key[i*4+1], key[i*4]);
    cs_printf("\n");

}
void __enc_cnt_show(void)
{
    cs_aal_enc_cnt_t stats;

    aal_enc_cnt_get(&stats);
    cs_printf("\n********ENC statistics********\n");
    cs_printf("AES Timer                : 0x%x\n", stats.aes_timer);
    cs_printf("Local Timer              : 0x%x\n", stats.local_timer);
    cs_printf("Key Request OAM          : %d\n", stats.request_cnt);
    cs_printf("UC Debug Counter         : 0x%x\n", stats.local_cnt);
    cs_printf("Key toggle counter       : %d\n", stats.key_idx_tog_cnt);
    cs_printf("Packet header            : 0x%x\n", stats.rx_aes_flag);
    cs_printf("Enc last packet rcc_b0   : %d\n", stats.rcc_b0);
    cs_printf("crc32 err cnt (even key) : %d\n", stats.crc_err_cnt_even_key);
    cs_printf("crc32 good cnt (even key): %d\n", stats.crc_good_cnt_even_key);
    cs_printf("crc32 err cnt (odd key)  : %d\n", stats.crc_err_cnt_odd_key);
    cs_printf("crc32 good cnt (odd key) : %d\n", stats.crc_good_cnt_odd_key);
    cs_printf("Rx Key Index             : %d\n", stats.rx_index);
    cs_printf("Tx Key Index             : %d\n", stats.tx_index);

}

cs_status __enc_reg_show(void)
{
    cs_uint32 val32;
    
    volatile EPON_t *g_epon_base = &g_lynxd_reg_ptr->epon;
    val32 = g_epon_base->AES_MODE_CTL.wrd;

    cs_printf("\n************************ENC Registers Status*************************\n\n");
    cs_printf(" AES mode and adjust local timer: addr 0x%08x=0x%08x, default=0x%08x", (cs_uint32)(&g_epon_base->AES_MODE_CTL), val32, 0xd4321d0e);
    if (val32 != 0xd4321d0e) {
        cs_printf(" <---\n");
    } else {
        cs_printf("\n");
    }

    cs_printf(" Uc Key0 3 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->UC_KEY03), g_epon_base->UC_KEY03.wrd);
    cs_printf(" Uc Key0 2 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->UC_KEY02), g_epon_base->UC_KEY02.wrd);
    cs_printf(" Uc Key0 1 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->UC_KEY01), g_epon_base->UC_KEY01.wrd);
    cs_printf(" Uc Key0 0 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->UC_KEY00), g_epon_base->UC_KEY00.wrd);
    cs_printf(" Uc Key1 3 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->UC_KEY13), g_epon_base->UC_KEY13.wrd);
    cs_printf(" Uc Key1 2 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->UC_KEY12), g_epon_base->UC_KEY12.wrd);
    cs_printf(" Uc Key1 1 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->UC_KEY11), g_epon_base->UC_KEY11.wrd);
    cs_printf(" Uc Key1 0 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->UC_KEY10), g_epon_base->UC_KEY10.wrd);
    cs_printf(" PKT_MON03 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->PKT_MON03), g_epon_base->PKT_MON03.wrd);
    cs_printf(" PKT_MON02 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->PKT_MON02), g_epon_base->PKT_MON02.wrd);
    cs_printf(" PKT_MON01 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->PKT_MON01), g_epon_base->PKT_MON01.wrd);
    cs_printf(" PKT_MON00 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->PKT_MON00), g_epon_base->PKT_MON00.wrd);
    cs_printf(" PKT_MON13 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->PKT_MON13), g_epon_base->PKT_MON13.wrd);
    cs_printf(" PKT_MON12 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->PKT_MON12), g_epon_base->PKT_MON12.wrd);
    cs_printf(" PKT_MON11 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->PKT_MON11), g_epon_base->PKT_MON11.wrd);
    cs_printf(" PKT_MON10 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->PKT_MON10), g_epon_base->PKT_MON10.wrd);
    cs_printf(" PKT MON2  Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->PKT_MON2), g_epon_base->PKT_MON2.wrd);
    cs_printf(" PKT MON31  Register            : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->PKT_MON31), g_epon_base->PKT_MON31.wrd);
    cs_printf(" PKT MON30  Register            : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->PKT_MON30), g_epon_base->PKT_MON30.wrd);
    cs_printf(" Keyrequest Oam Cnt Register    : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->KEYREQUEST_OAM_CNT), g_epon_base->KEYREQUEST_OAM_CNT.wrd);
    cs_printf(" Uc Dec Dbg Cnt Register        : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->UC_DEC_DBG_CNT), g_epon_base->UC_DEC_DBG_CNT.wrd);
    cs_printf(" JAES Timer Register            : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->JAES_TIMER), g_epon_base->JAES_TIMER.wrd);

    cs_printf(" Aes Churning Ctrl Register     : addr 0x%08x=0x%08x, default=0x%08x", (cs_uint32)(&g_epon_base->AES_CHURNING_CTRL), val32 = g_epon_base->AES_CHURNING_CTRL.wrd, 0x81);
    if (val32 != 0x81) {
        cs_printf(" <---\n");
    } else {
        cs_printf("\n");
    }

    cs_printf(" Churn Config Control Register  : addr 0x%08x=0x%08x, default=0x%08x", (cs_uint32)(&g_epon_base->CHURN_CONFIG_CONTROL), val32 = g_epon_base->CHURN_CONFIG_CONTROL.wrd, 0x0);
    if (val32 != 0x0) {
        cs_printf(" <---\n");
    } else {
        cs_printf("\n");
    }

    cs_printf(" HW Key0 3 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->HW_GEN_KEY03), g_epon_base->HW_GEN_KEY03.wrd);
    cs_printf(" HW Key0 2 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->HW_GEN_KEY02), g_epon_base->HW_GEN_KEY02.wrd);
    cs_printf(" HW Key0 1 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->HW_GEN_KEY01), g_epon_base->HW_GEN_KEY01.wrd);
    cs_printf(" HW Key0 0 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->HW_GEN_KEY00), g_epon_base->HW_GEN_KEY00.wrd);
    cs_printf(" HW Key1 3 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->HW_GEN_KEY13), g_epon_base->HW_GEN_KEY13.wrd);
    cs_printf(" HW Key1 2 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->HW_GEN_KEY12), g_epon_base->HW_GEN_KEY12.wrd);
    cs_printf(" HW Key1 1 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->HW_GEN_KEY11), g_epon_base->HW_GEN_KEY11.wrd);
    cs_printf(" HW Key1 0 Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->HW_GEN_KEY10), g_epon_base->HW_GEN_KEY10.wrd);

    cs_printf(" Oam Oui Register               : addr 0x%08x=0x%08x, default=0x%08x", (cs_uint32)(&g_epon_base->CHURNING_OAM_OUI), val32 = g_epon_base->CHURNING_OAM_OUI.wrd, 0x800000d9);
    if (val32 != 0x000000d9) {
        cs_printf(" <---\n");
    } else {
        cs_printf("\n");
    }
    cs_printf(" Oam Flag Register              : addr 0x%08x=0x%08x, default=0x%08x", (cs_uint32)(&g_epon_base->CHURNING_OAM_FLAG), val32 = g_epon_base->CHURNING_OAM_FLAG.wrd, 0x50);
    if (val32 != 0x50) {
        cs_printf(" <---\n");
    } else {
        cs_printf("\n");
    }

    cs_printf(" Aes Timer Register             : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->AES_TIMER), g_epon_base->AES_TIMER.wrd);
    cs_printf(" Aes Timer2 Register            : addr 0x%08x=0x%08x\n", (cs_uint32)(&g_epon_base->AES_TIMER2), g_epon_base->AES_TIMER2.wrd);

    return CS_E_OK;
}

void __enc_cfg_cmd(cs_uint32 mask, cs_uint32 value)
{
    cs_aal_enc_cfg_t cfg;
    cs_aal_enc_msk_t msk;
    cs_status rt = CS_E_OK;

    msk.u32 = mask;
   
    if(msk.s.en)
        cfg.en = value;

    if(msk.s.mode)
        cfg.mode = value;
        
    if(msk.s.extra_oam_rpt_len)
        cfg.extra_oam_rpt_len = value;

    if(msk.s.oam_oui)
        cfg.oam_oui = value;

    if(msk.s.oam_flag)
        cfg.oam_flag = value;
        
    if(msk.s.churn_mode)
        cfg.churn_mode = value;
    
    if(msk.s.key_gen_mode)
        cfg.key_gen_mode = value ;

    if(msk.s.key_exchange_mode)
        cfg.key_exchange_mode = value ;


    rt |= aal_enc_cfg_set(msk, &cfg);
    if (rt != CS_E_OK) {
        cs_printf("Return value is %d.\n", rt);
        return;
    }
    __enc_cfg_show();
}

void __enc_tx_cfg_cmd(cs_uint32 mask, cs_uint32 value)
{
    cs_aal_enc_tx_cfg_t tx_cfg;
    cs_aal_enc_tx_msk_t msk;
    cs_status rt = CS_E_OK;

    msk.u32 = mask;

    
    if(msk.s.en) 
        tx_cfg.en = value;
    if(msk.s.threshold)
        tx_cfg.threshold = value;     

    if(msk.s.crc8_regen_en)
        tx_cfg.crc8_regen_en = value;
    
    if(msk.s.crc8_in_reverse_en)
        tx_cfg.crc8_in_reverse_en = value;
    
    if(msk.s.crc8_out_reverse_en)
        tx_cfg.crc8_out_reverse_en = value;

    if(msk.s.key_idx_sel_mode)  
        tx_cfg.key_idx_sel_mode = value;
    
    if(msk.s.key_idx)
        tx_cfg.key_idx = value;
    
    if(msk.s.key_idx_notify_en) 
        tx_cfg.key_idx_notify_en = value;
    
    if(msk.s.rcc_b0_sel_mode)
        tx_cfg.rcc_b0_sel_mode = value ;

    if(msk.s.key_sel_mode)
        tx_cfg.key_sel_mode = value;

    rt |= aal_enc_tx_cfg_set(msk, &tx_cfg);
    if (rt != CS_E_OK) {
        cs_printf("Return value is %d.\n", rt);
        return;
    }

    __enc_tx_cfg_show();

}

void __enc_timer_cfg_cmd(cs_uint32 mask, cs_uint32 value)
{
    cs_aal_enc_timer_cfg_t timer;
    cs_aal_enc_timer_msk_t msk;
    cs_status rt = CS_E_OK;

    msk.u32 = mask;

    if(msk.s.rx_dir)
        timer.rx_dir = value;
        
    if(msk.s.rx_delta)
        timer.rx_delta = value;
        
    if(msk.s.tx_dir)
        timer.tx_dir = value;

    if(msk.s.tx_delta)
        timer.tx_delta = value;
        
    if(msk.s.timer_hi16)
        timer.timer_hi16 = value;

    if(msk.s.jaes_timer)
        timer.jaes_timer = value;
    
    rt |= aal_enc_timer_cfg_set(msk, &timer);
    if (rt != CS_E_OK) {
        cs_printf("Return value is %d.\n", rt);
        return;
    }

    __enc_timer_show();
}

void __enc_key_set_cmd(cs_uint32 key_index, cs_uint32 value, cs_uint32 value2, cs_uint32 value3, 
cs_uint32 value4)
{
    cs_uint8 key[16];
    cs_status rt = CS_E_OK;

    memset(&key, 0, sizeof(key));

    key[0] = value & 0xff;
    key[1] = (value >> 8) & 0xff;
    key[2] = (value >> 16) & 0xff;
    key[3] = (value >> 24) & 0xff;

    key[4] = value2 & 0xff;
    key[5] = (value2 >> 8) & 0xff;
    key[6] = (value2 >> 16) & 0xff;
    key[7] = (value2 >> 24) & 0xff;

    key[8] = value3 & 0xff;
    key[9] = (value3 >> 8) & 0xff;
    key[10] = (value3 >> 16) & 0xff;
    key[11] = (value3 >> 24) & 0xff;

    key[12] = value4 & 0xff;
    key[13] = (value4 >> 8) & 0xff;
    key[14] = (value4 >> 16) & 0xff;
    key[15] = (value4 >> 24) & 0xff;

    switch (key_index) {
    case 0: {
        rt |= aal_enc_key_set(0, (cs_aal_enc_key_t *)key);
        break;
    }
    case 1: {
        rt |= aal_enc_key_set(1, (cs_aal_enc_key_t *)key);
        break;
    }
    default: {
        cs_printf("key_index is not supported!!\n");
        return;
    }
    }
    cs_printf("Return value is %d.\n", rt);
    __enc_key_show();

}


void __enc_cfg_cmd_help(void)
{

    cs_printf("enc cfg cmd msk:\n");
    cs_printf("en                : 0x1  \n");
    cs_printf("mode              : 0x2  \n");
    cs_printf("churn_mode        : 0x4  \n");
    cs_printf("oam_flag          : 0x8  \n");  
    cs_printf("oam_oui           : 0x10 \n"); 
    cs_printf("key_gen_mode      : 0x20 \n"); 
    cs_printf("key_exchange_mode : 0x40 \n");
    cs_printf("extra_oam_rpt_len : 0x80 \n");
                
}

void __enc_tx_cfg_cmd_help(void)
{

    cs_printf("enc tx_cfg cmd msk:\n");
    cs_printf("en                 : 0x1   \n");
    cs_printf("threshold          : 0x2   \n");
    cs_printf("key_idx_sel_mode   : 0x4   \n");
    cs_printf("key_idx            : 0x8   \n"); 
    cs_printf("key_idx_notify_en  : 0x10  \n");
    cs_printf("crc8_regen_en      : 0x20  \n"); 
    cs_printf("crc8_in_reverse_en : 0x40  \n"); 
    cs_printf("crc8_out_reverse_en: 0x80  \n");
    cs_printf("rcc_b0_sel_mode    : 0x100 \n");
    cs_printf("key_sel_mode       : 0x200 \n");
} 

void __enc_timer_cfg_cmd_help(void)
{

    cs_printf("enc timer cmd msk:\n");
    cs_printf("rx_dir    : 0x1   \n");
    cs_printf("rx_delta  : 0x2   \n");
    cs_printf("tx_dir    : 0x4   \n");
    cs_printf("tx_delta  : 0x8   \n");  
    cs_printf("timer_hi16: 0x10  \n"); 
    cs_printf("jaes_timer: 0x20  \n");
}




void aal_enc_cmd(int argc, char **argv)
{
    cs_uint32 mask = 0;
    cs_uint32 value = 0;

    if(argc < 3){
        cs_printf("\n");
        cs_printf("aal enc cfg [mask][value]\n"); 
        cs_printf("aal enc cfg get \n");        
        cs_printf("aal enc tx_cfg [mask][value]\n");
        cs_printf("aal enc tx_cfg get \n");
        cs_printf("aal enc timer [msk][value]\n");
        cs_printf("aal enc timer get \n");
        cs_printf("aal enc key [index][key0][key1][key2][key3]\n");
        cs_printf("aal enc show reg \n");       
        cs_printf("aal enc show cnt \n");
        cs_printf("aal enc show key \n");
        cs_printf("aal enc dbg\n"); 
    }
    else{
        mask = (cs_uint32)iros_strtol(argv[3]);
        value = (cs_uint32)iros_strtol(argv[4]);
        if (cmd_cmp(argv[2], "cfg")) {
            if(argc == 3)
                __enc_cfg_cmd_help();
            else if (cmd_cmp(argv[3], "get"))
                __enc_cfg_show();
            else{
                __enc_cfg_cmd(mask, value);
            }
        } 
        else if (cmd_cmp(argv[2], "tx_cfg")) {
            if(argc == 3)
                __enc_tx_cfg_cmd_help();
            else if (cmd_cmp(argv[3], "get"))
                __enc_tx_cfg_show();
            else
                __enc_tx_cfg_cmd(mask, value);
        } 
        else if (cmd_cmp(argv[2], "timer")) {
            if(argc == 3)
                __enc_timer_cfg_cmd_help();
            else if (cmd_cmp(argv[3], "get"))
                __enc_timer_show();
            else
                __enc_timer_cfg_cmd(mask, value);
        } 
        else if (cmd_cmp(argv[2], "key")) {      
            if(argc < 7)
                cs_printf("aal enc key [index][key0][key1][key2][key3]\n");
            else{              
                cs_uint32 index = (cs_uint32)iros_strtol(argv[3]);               
                cs_uint32 key0 = (cs_uint32)iros_strtol(argv[4]);
                cs_uint32 key1 = (cs_uint32)iros_strtol(argv[5]);
                cs_uint32 key2 = (cs_uint32)iros_strtol(argv[6]);
                cs_uint32 key3 = (cs_uint32)iros_strtol(argv[7]);
                
                __enc_key_set_cmd(index, key0, key1, key2, key3); 
            }
        }
        else if (cmd_cmp(argv[2], "show")) {      
            if (cmd_cmp(argv[3], "reg"))
                __enc_reg_show();
            else if (cmd_cmp(argv[3], "cnt"))
                __enc_cnt_show();
            else if (cmd_cmp(argv[3], "key"))
                __enc_key_show();
        }
        else if (cmd_cmp(argv[2], "dbg")) {  
            enc_dbg = ! enc_dbg;
        }
        else
            cs_printf("error cmd!!\n");
    }
    return;
}


