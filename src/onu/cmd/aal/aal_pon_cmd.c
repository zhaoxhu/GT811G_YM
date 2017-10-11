#include "aal_pon.h"
#include "plat_common.h"

extern int cmd_cmp(char *str, const char *mode);
extern int cs_atoi(char *str);

#if 0
(1<<0)
(1<<1)
(1<<2)
(1<<3)
(1<<4)
(1<<5)
(1<<6)
(1<<7)
(1<<8)
(1<<9)
(1<<10)
(1<<11)
(1<<12)
(1<<13)
(1<<14)
(1<<15)
(1<<16)
(1<<17)
(1<<18)
(1<<19)
(1<<20)
(1<<21)
(1<<22)
(1<<23)
(1<<24)
(1<<25)
(1<<26)
(1<<27)
(1<<28)
(1<<29)
(1<<30)
(1<<31)
#endif


void __pon_help(void)
{
    cs_printf("aal pon laser [mask] [value]\n");
    cs_printf("aal pon mac   [mask] [value]\n");
    cs_printf("aal pon llid  [mask] [value]\n");
    cs_printf("aal pon drop  [mask] [value]\n");
    cs_printf("aal pon int   [mask] [value]\n");
}

void __laser_show(cs_aal_pon_laser_cfg_t *cfg)
{
    aal_pon_laser_cfg_get(cfg);
    
    cs_printf("\nLASER CONFIGURATION\n");
    cs_printf("==========================================\n");
    cs_printf("%-26s(0x%-8x): %u\n", "tx_en            ", (1<<0), cfg->tx_en            );
    cs_printf("%-26s(0x%-8x): %u\n", "laser_mode       ", (1<<1), cfg->laser_mode       );
    cs_printf("%-26s(0x%-8x): %u\n", "laser_on_off_time", (1<<2), cfg->laser_on_time    );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_delay         ", (1<<3), cfg->tx_delay         );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_power_en      ", (1<<4), cfg->tx_power_en      );
    cs_printf("%-26s(0x%-8x): %u\n", "sig_loss_polarity", (1<<5), cfg->sig_loss_polarity);
    cs_printf("%-26s(0x%-8x): %u\n", "tx_burst_polarity", (1<<6), cfg->tx_burst_polarity);
    cs_printf("%-26s(0x%-8x): %u\n", "output_en        ", (1<<7), cfg->output_en        );
    cs_printf("%-26s(0x%-8x): %u\n", "power_mode       ", (1<<8), cfg->power_mode       );
    cs_printf("%-26s(0x%-8x): %u\n", "wakeup_time      ", (1<<9), cfg->wakeup_time      );
    cs_printf("%-26s(0x%-8x): %u\n", "shutdown_time    ", (1<<10), cfg->shutdown_time    );
    cs_printf("%-26s(0x%-8x): %u\n", "keep_time        ", (1<<11), cfg->keep_time        );
    cs_printf("==========================================\n");
}

void __mac_show(cs_aal_pon_mac_cfg_t *cfg)
{
    aal_pon_mac_cfg_get(cfg);
    
    cs_printf("\nMAC CONFIGURATION\n");
    cs_printf("==========================================\n");
    cs_printf("%-26s(0x%-8x): %u\n", "rx_en            ", (1<<0), cfg->rx_en            );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_en            ", (1<<1), cfg->tx_en            );
    cs_printf("%-26s(0x%-8x): %u\n", "rx_ipg           ", (1<<2), cfg->rx_ipg           );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_ipg           ", (1<<3), cfg->tx_ipg           );
    cs_printf("%-26s(0x%-8x): %u\n", "mtu              ", (1<<4), cfg->mtu              );
    cs_printf("%-26s(0x%-8x): %u\n", "mtu_tag_en       ", (1<<5), cfg->mtu_tag_en       );
    cs_printf("%-26s(0x%-8x): %u\n", "mtu_tag_pattern  ", (1<<6), cfg->mtu_tag_pattern  );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_max_size      ", (1<<7), cfg->tx_max_size      );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_max_limit_en  ", (1<<8), cfg->tx_max_limit_en  );
    cs_printf("%-26s(0x%-8x): %u\n", "rx_min_size      ", (1<<9), cfg->rx_min_size      );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_drain_en      ", (1<<10),cfg->tx_drain_en      );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_fix_8b_en     ", (1<<11),cfg->tx_fix_8b_en     );
    cs_printf("%-26s(0x%-8x): %u\n", "user_fifo_hi     ", (1<<12), cfg->user_fifo_hi     );
    cs_printf("%-26s(0x%-8x): %u\n", "user_fifo_lo     ", (1<<13), cfg->user_fifo_lo     );
    cs_printf("%-26s(0x%-8x): %u\n", "pcs_loop_cnt_hi  ", (1<<14), cfg->pcs_loop_cnt_hi  );
    cs_printf("%-26s(0x%-8x): %u\n", "pcs_loop_cnt_lo  ", (1<<15), cfg->pcs_loop_cnt_lo  );
    cs_printf("%-26s(0x%-8x): %u\n", "crc32_regen_en   ", (1<<16), cfg->crc32_regen_en   );
    cs_printf("%-26s(0x%-8x): %u\n", "crc8_check_en    ", (1<<17), cfg->crc8_check_en    );
    cs_printf("%-26s(0x%-8x): %u\n", "pcs_10b_aliagn_en", (1<<18), cfg->pcs_10b_aliagn_en);
    cs_printf("%-26s(0x%-8x): %u\n", "fec_sync_en      ", (1<<19), cfg->fec_sync_en      );
    cs_printf("%-26s(0x%-8x): %u\n", "sfd_mode2        ", (1<<20), cfg->sfd_mode2        );
    cs_printf("%-26s(0x%-8x): %u\n", "crc8_order_sel   ", (1<<21), cfg->crc8_order_sel   );
    cs_printf("==========================================\n");
}

void __llid_filter_show(cs_aal_pon_llid_filter_cfg_t *cfg)
{
    aal_pon_llid_filter_cfg_get(cfg);
    
    cs_printf("\nLLID FILTER CONFIGURATION\n");
    cs_printf("==========================================\n");
    cs_printf("%-26s(0x%-8x): %u\n", "drop_ffff  ", (1<<0), cfg->drop_ffff  );
    cs_printf("%-26s(0x%-8x): %u\n", "drop_7fff  ", (1<<1), cfg->drop_7fff  );
    cs_printf("%-26s(0x%-8x): %u\n", "drop_p2p   ", (1<<2), cfg->drop_p2p   );
    cs_printf("%-26s(0x%-8x): %u\n", "drop_myslef", (1<<3), cfg->drop_myslef);
    cs_printf("%-26s(0x%-8x): %u\n", "accept_all ", (1<<4), cfg->accept_all );
    cs_printf("%-26s(0x%-8x): %u\n", "accept_cs  ", (1<<5), cfg->accept_cs  );
    cs_printf("==========================================\n");
}

void __drop_show(cs_aal_pon_drop_t *cfg)
{
    aal_pon_drop_policy_get(cfg);
    
    cs_printf("\nDROP POLICY CONFIGURATION\n");
    cs_printf("==========================================\n");
    cs_printf("%-26s(0x%-8x): %u\n", "extention_frame_permit", (1<<0), cfg->s.extention_frame_permit);
    cs_printf("%-26s(0x%-8x): %u\n", "maccontrol_permit     ", (1<<1), cfg->s.maccontrol_permit     );
    cs_printf("%-26s(0x%-8x): %u\n", "slow_protocol_permit  ", (1<<2), cfg->s.slow_protocol_permit  );
    cs_printf("%-26s(0x%-8x): %u\n", "pause_frame_permit    ", (1<<3), cfg->s.pause_frame_permit    );
    cs_printf("%-26s(0x%-8x): %u\n", "undersize_permit      ", (1<<4), cfg->s.undersize_permit      );
    cs_printf("%-26s(0x%-8x): %u\n", "oversize_permit       ", (1<<5), cfg->s.oversize_permit       );
    cs_printf("%-26s(0x%-8x): %u\n", "fcs_err_permit        ", (1<<6), cfg->s.fcs_err_permit        );
    cs_printf("%-26s(0x%-8x): %u\n", "discovery_gate_permit ", (1<<7), cfg->s.discovery_gate_permit );
    cs_printf("%-26s(0x%-8x): %u\n", "non_discovery_permit  ", (1<<8), cfg->s.non_discovery_permit  );
    cs_printf("%-26s(0x%-8x): %u\n", "register_permit       ", (1<<9), cfg->s.register_permit       );
    cs_printf("%-26s(0x%-8x): %u\n", "err_maccontrol_permit ", (1<<10),cfg->s.err_maccontrol_permit );
    cs_printf("==========================================\n");
}

void __int_show(cs_aal_pon_int_msk_t *cfg)
{
    aal_pon_int_en_get(cfg);
    
    cs_printf("\nINTERRUPT CONFIGURATION\n");
    cs_printf("==========================================\n");
    cs_printf("%-26s(0x%-8x): %u\n", "cntr_half_full            ", (1<<0), cfg->s.cntr_half_full            );
    cs_printf("%-26s(0x%-8x): %u\n", "pack_full                 ", (1<<1), cfg->s.pack_full                 );
    cs_printf("%-26s(0x%-8x): %u\n", "prty_full                 ", (1<<2), cfg->s.prty_full                 );
    cs_printf("%-26s(0x%-8x): %u\n", "cmd_full                  ", (1<<3), cfg->s.cmd_full                  );
    cs_printf("%-26s(0x%-8x): %u\n", "cwb_full                  ", (1<<4), cfg->s.cwb_full                  );
    cs_printf("%-26s(0x%-8x): %u\n", "pps_timeout               ", (1<<5), cfg->s.pps_timeout               );
    cs_printf("%-26s(0x%-8x): %u\n", "pcs_rx_sync               ", (1<<6), cfg->s.pcs_rx_sync               );
    cs_printf("%-26s(0x%-8x): %u\n", "mib                       ", (1<<7), cfg->s.mib                       );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_non_empty              ", (1<<8), cfg->s.tx_non_empty              );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_pkt_cut                ", (1<<9), cfg->s.tx_pkt_cut                );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_overrun                ", (1<<10),cfg->s.tx_overrun                );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_fail                   ", (1<<11),cfg->s.tx_fail                   );
    cs_printf("%-26s(0x%-8x): %u\n", "ptp_eg_lts                ", (1<<12),cfg->s.ptp_eg_lts                );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_packet_uflow           ", (1<<13),cfg->s.tx_packet_uflow           );
    cs_printf("%-26s(0x%-8x): %u\n", "ptp_ig_lts                ", (1<<14),cfg->s.ptp_ig_lts                );
    cs_printf("%-26s(0x%-8x): %u\n", "bm_pon_tx_mon             ", (1<<15),cfg->s.bm_pon_tx_mon             );
    cs_printf("%-26s(0x%-8x): %u\n", "report_timer_expiration   ", (1<<16),cfg->s.report_timer_expiration   );
    cs_printf("%-26s(0x%-8x): %u\n", "registration_status_change", (1<<17),cfg->s.registration_status_change);
    cs_printf("%-26s(0x%-8x): %u\n", "rx_register_frame         ", (1<<18),cfg->s.rx_register_frame         );
    cs_printf("%-26s(0x%-8x): %u\n", "mpcp_grant_fifo_exceeding ", (1<<19),cfg->s.mpcp_grant_fifo_exceeding );
    cs_printf("%-26s(0x%-8x): %u\n", "mpcp_grant_fifo_access_err", (1<<20),cfg->s.mpcp_grant_fifo_access_err);
    cs_printf("%-26s(0x%-8x): %u\n", "timestamp_drift           ", (1<<21),cfg->s.timestamp_drift           );
    cs_printf("%-26s(0x%-8x): %u\n", "gate_timer_expiration     ", (1<<22),cfg->s.gate_timer_expiration     );
    cs_printf("%-26s(0x%-8x): %u\n", "mpcp_timer_expiration     ", (1<<23),cfg->s.mpcp_timer_expiration     );
    cs_printf("%-26s(0x%-8x): %u\n", "rtt_rx                    ", (1<<24),cfg->s.rtt_rx                    );
    cs_printf("%-26s(0x%-8x): %u\n", "grant_drop_tf             ", (1<<25),cfg->s.grant_drop_tf             );
    cs_printf("%-26s(0x%-8x): %u\n", "grant_drop_ts             ", (1<<26),cfg->s.grant_drop_ts             );
    cs_printf("%-26s(0x%-8x): %u\n", "grant_drop_tm             ", (1<<27),cfg->s.grant_drop_tm             );
    cs_printf("%-26s(0x%-8x): %u\n", "enc_en_trig               ", (1<<28),cfg->s.enc_en_trig               );
    cs_printf("%-26s(0x%-8x): %u\n", "discovery_frm             ", (1<<29),cfg->s.discovery_frm             );
    cs_printf("%-26s(0x%-8x): %u\n", "sig_loss                  ", (1<<30),cfg->s.sig_loss                  );
    cs_printf("%-26s(0x%-8x): %u\n", "pps_cap                   ", (1<<31),cfg->s.pps_cap                   );
    cs_printf("==========================================\n");
}

void aal_pon_cmd(int argc, char **argv)
{
    cs_uint32 val,addr,tmp;
    int i = 0;

    if(2==argc)
    {
        __pon_help();
        return;
    }

    if(cmd_cmp(argv[2], "laser"))
    {
        cs_aal_pon_laser_cfg_t cfg;
        void* offset[] = 
        {
            &cfg.tx_en            ,
            &cfg.laser_mode       ,
            &cfg.laser_on_time    ,
            &cfg.tx_delay         ,
            &cfg.tx_power_en      ,
            &cfg.sig_loss_polarity,
            &cfg.tx_burst_polarity,
            &cfg.output_en        ,
            &cfg.power_mode       ,
            &cfg.wakeup_time      ,
            &cfg.shutdown_time    ,
            &cfg.keep_time        
        };
        if(5==argc)
        {
            cs_aal_pon_laser_msk_t msk;
            msk.u32 = cs_atoi(argv[3]);
            tmp = msk.u32;
            val = cs_atoi(argv[4]);
            while(tmp) {tmp>>=1; ++i;}
            if(i>=sizeof(offset)/sizeof(void *))
                return;
            if(i)
            {
                addr = (cs_uint32)offset[i-1];
                if(!(addr&0x3)) *(cs_uint32*)addr = val;
                else if(!(addr&0x1)) *(cs_uint16*)addr = val;
                else *(cs_uint8*)addr = val;
                aal_pon_laser_cfg_set(msk, &cfg);
            }
        }
        __laser_show(&cfg);
    }
    else if(cmd_cmp(argv[2], "mac"))
    {
        cs_aal_pon_mac_cfg_t cfg;
        void* offset[] = 
        {
            &cfg.rx_en            ,
            &cfg.tx_en            ,
            &cfg.rx_ipg           ,
            &cfg.tx_ipg           ,
            &cfg.mtu              ,
            &cfg.mtu_tag_en       ,
            &cfg.mtu_tag_pattern  ,
            &cfg.tx_max_size      ,
            &cfg.tx_max_limit_en  ,
            &cfg.rx_min_size      ,
            &cfg.tx_drain_en      ,
            &cfg.tx_fix_8b_en     ,
            &cfg.user_fifo_hi     ,
            &cfg.user_fifo_lo     ,
            &cfg.pcs_loop_cnt_hi  ,
            &cfg.pcs_loop_cnt_lo  ,
            &cfg.crc32_regen_en   ,
            &cfg.crc8_check_en    ,
            &cfg.pcs_10b_aliagn_en,
            &cfg.fec_sync_en      ,
            &cfg.sfd_mode2        ,
            &cfg.crc8_order_sel
        };
        if(5==argc)
        {
            cs_aal_pon_mac_msk_t msk;
            msk.u32 = cs_atoi(argv[3]);
            tmp = msk.u32;
            val = cs_atoi(argv[4]);
            while(tmp) {tmp>>=1; ++i;}
            if(i>=sizeof(offset)/sizeof(void *))
                return;
            if(i)
            {
                addr = (cs_uint32)offset[i-1];
                if(!(addr&0x3)) *(cs_uint32*)addr = val;
                else if(!(addr&0x1)) *(cs_uint16*)addr = val;
                else *(cs_uint8*)addr = val;
                aal_pon_mac_cfg_set(msk, &cfg);
            }
        }
        __mac_show(&cfg);
    }
    else if(cmd_cmp(argv[2], "llid"))
    {
        cs_aal_pon_llid_filter_cfg_t cfg;
        void* offset[] = 
        {
            &cfg.drop_ffff  ,
            &cfg.drop_7fff  ,
            &cfg.drop_p2p   ,
            &cfg.drop_myslef,
            &cfg.accept_all ,
            &cfg.accept_cs
        };
        if(5==argc)
        {
            cs_aal_pon_llid_filter_msk_t msk;
            msk.u32 = cs_atoi(argv[3]);
            tmp = msk.u32;
            val = cs_atoi(argv[4]);
            while(tmp) {tmp>>=1; ++i;}
            if(i>=sizeof(offset)/sizeof(void *))
                return;
            if(i)
            {
                addr = (cs_uint32)offset[i-1];
                if(!(addr&0x3)) *(cs_uint32*)addr = val;
                else if(!(addr&0x1)) *(cs_uint16*)addr = val;
                else *(cs_uint8*)addr = val;
                aal_pon_llid_filter_cfg_set(msk, &cfg);
            }
        }
        __llid_filter_show(&cfg);
    }
    else if(cmd_cmp(argv[2], "drop"))
    {
        cs_aal_pon_drop_t cfg;
        if(5==argc)
        {
            cs_aal_pon_drop_t msk;
            msk.u32 = cs_atoi(argv[3]);
            tmp = msk.u32;
            cfg.u32 = cs_atoi(argv[4]);
            while(tmp) {tmp>>=1; ++i;}
            if(i)
            {
                cfg.u32<<=(i-1);
                aal_pon_drop_policy_set(msk, &cfg);
            }
        }
        __drop_show(&cfg);
    }
    else if(cmd_cmp(argv[2], "int"))
    {
        cs_aal_pon_int_msk_t cfg;
        if(5==argc)
        {
            cs_aal_pon_int_msk_t msk;
            msk.u32 = cs_atoi(argv[3]);
            val = cs_atoi(argv[4]);
            if(val)
                aal_pon_int_en_set(msk);
            else
                aal_pon_int_disable(msk);
        }
        __int_show(&cfg);
    }
    else
    {
        __pon_help();
    }
}


