#include "aal_mpcp.h"
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


void __mpcp_help(void)
{
    cs_printf("aal mpcp timer  [mask] [value]\n");
    cs_printf("aal mpcp report [mask] [value]\n");
    cs_printf("aal mpcp misc   [mask] [value]\n");
    cs_printf("aal mpcp rtt    [mask] [value]\n");
    cs_printf("aal mpcp queue  [id]   [thre]  [max_thre]\n");
}

void __timer_show(cs_aal_mpcp_timer_cfg_t *cfg)
{
    aal_mpcp_timer_cfg_get(cfg);
    
    cs_printf("\nTIMER CONFIGURATION\n");
    cs_printf("==========================================\n");
    cs_printf("%-26s(0x%-8x): %u\n", "gatetimer_en          ", (1<<0), cfg->gatetimer_en          );
    cs_printf("%-26s(0x%-8x): %u\n", "mpcptimer_en          ", (1<<1), cfg->mpcptimer_en          );
    cs_printf("%-26s(0x%-8x): %u\n", "reporttimer_en        ", (1<<2), cfg->reporttimer_en        );
    cs_printf("%-26s(0x%-8x): %u\n", "gatetimer_dereg_en    ", (1<<3), cfg->gatetimer_dereg_en    );
    cs_printf("%-26s(0x%-8x): %u\n", "mpcptimer_dereg_en    ", (1<<4), cfg->mpcptimer_dereg_en    );
    cs_printf("%-26s(0x%-8x): %u\n", "timedrift_dereg_en    ", (1<<5), cfg->timedrift_dereg_en    );
    cs_printf("%-26s(0x%-8x): %u\n", "gatetimer_threshold   ", (1<<6), cfg->gatetimer_threshold   );
    cs_printf("%-26s(0x%-8x): %u\n", "mpcptimer_threshold   ", (1<<7), cfg->mpcptimer_threshold   );
    cs_printf("%-26s(0x%-8x): %u\n", "timedrift_threshold   ", (1<<8), cfg->timedrift_threshold   );
    cs_printf("%-26s(0x%-8x): %u\n", "reporttimer_threshold ", (1<<9), cfg->reporttimer_threshold );
    cs_printf("==========================================\n");
}

void __report_show(cs_aal_mpcp_report_cfg_t *cfg)
{
    aal_mpcp_report_cfg_get(cfg);
    
    cs_printf("\nREPORT CONFIGURATION\n");
    cs_printf("==========================================\n");
    cs_printf("%-26s(0x%-8x): %u\n", "t_threshold         ", (1<<0), cfg->t_threshold         );
    cs_printf("%-26s(0x%-8x): %u\n", "zeroq_map           ", (1<<1), cfg->zeroq_map           );
    cs_printf("%-26s(0x%-8x): %u\n", "zeroq_threshold     ", (1<<2), cfg->zeroq_threshold     );
    cs_printf("%-26s(0x%-8x): %u\n", "aggregation_mode_en ", (1<<3), cfg->aggregation_mode_en );
    cs_printf("%-26s(0x%-8x): %u\n", "report_format       ", (1<<4), cfg->report_format       );
    cs_printf("%-26s(0x%-8x): %u\n", "rpt_order           ", (1<<5), cfg->rpt_order           );
    cs_printf("%-26s(0x%-8x): %u\n", "rpt_number          ", (1<<6), cfg->rpt_number          );
    cs_printf("%-26s(0x%-8x): %u\n", "rpt_bmp_qset1       ", (1<<7), cfg->rpt_bmp_qset1       );
    cs_printf("%-26s(0x%-8x): %u\n", "rpt_bmp_qset2       ", (1<<8), cfg->rpt_bmp_qset2       );
    cs_printf("%-26s(0x%-8x): %u\n", "schedule_fit_en     ", (1<<9), cfg->schedule_fit_en     );
    cs_printf("%-26s(0x%-8x): %u\n", "report_fit_en       ", (1<<10),cfg->report_fit_en       );
    cs_printf("%-26s(0x%-8x): %u\n", "quick_rpt_en        ", (1<<11),cfg->quick_rpt_en        );
    cs_printf("%-26s(0x%-8x): %u\n", "ignore_rpt_en       ", (1<<12),cfg->ignore_rpt_en       );
    cs_printf("%-26s(0x%-8x): %u\n", "single_window_en    ", (1<<13),cfg->single_window_en    );
    cs_printf("%-26s(0x%-8x): %u\n", "cs_mode_en          ", (1<<14),cfg->cs_mode_en          );
    cs_printf("%-26s(0x%-8x): %u\n", "cs_mode_opt         ", (1<<15),cfg->cs_mode_opt         );
    cs_printf("%-26s(0x%-8x): %u\n", "cs_mode_threshold   ", (1<<16),cfg->cs_mode_threshold   );
    cs_printf("%-26s(0x%-8x): %u\n", "le_mode_en          ", (1<<17),cfg->le_mode_en          );
    cs_printf("%-26s(0x%-8x): %u\n", "le_mode_opt         ", (1<<18),cfg->le_mode_opt         );
    cs_printf("%-26s(0x%-8x): %u\n", "le_mode_bmp         ", (1<<19),cfg->le_mode_bmp         );
    cs_printf("==========================================\n");
}

void __misc_show(cs_aal_mpcp_misc_cfg_t *cfg)
{
    aal_mpcp_misc_cfg_get(cfg);
    
    cs_printf("\nMISC CONFIGURATION\n");
    cs_printf("==========================================\n");
    cs_printf("%-26s(0x%-8x): %u\n", "pending_grant             ", (1<<0), cfg->pending_grant             );
    cs_printf("%-26s(0x%-8x): %u\n", "min_grant_size            ", (1<<1), cfg->min_grant_size            );
    cs_printf("%-26s(0x%-8x): %u\n", "request_frame_llid        ", (1<<2), cfg->request_frame_llid        );
    cs_printf("%-26s(0x%-8x): %u\n", "mpcp_frame_len_check_en   ", (1<<3), cfg->mpcp_frame_len_check_en   );
    cs_printf("%-26s(0x%-8x): %u\n", "min_process_time_check_en ", (1<<4), cfg->min_process_time_check_en );
    cs_printf("%-26s(0x%-8x): %u\n", "min_process_time          ", (1<<5), cfg->min_process_time          );
    cs_printf("%-26s(0x%-8x): %u\n", "max_future_grant_time     ", (1<<6), cfg->max_future_grant_time     );
    cs_printf("%-26s(0x%-8x): %u\n", "discovery_pkt_size        ", (1<<7), cfg->discovery_pkt_size        );
    cs_printf("%-26s(0x%-8x): %u\n", "onu_mac_addr_check_en     ", (1<<8), cfg->onu_mac_addr_check_en     );
    cs_printf("%-26s(0x%-8x): %u\n", "mpcp_frame_fec_overhead   ", (1<<9), cfg->mpcp_frame_fec_overhead   );
    cs_printf("%-26s(0x%-8x): %u\n", "strict_starttime_check_en ", (1<<10),cfg->strict_starttime_check_en );
    cs_printf("%-26s(0x%-8x): %u\n", "rereg_mode                ", (1<<11),cfg->rereg_mode                );
    cs_printf("%-26s(0x%-8x): %u\n", "force_rpt_data_en         ", (1<<12),cfg->force_rpt_data_en         );
    cs_printf("%-26s(0x%-8x): %u\n", "tx_window_adjust_time     ", (1<<13),cfg->tx_window_adjust_time     );
    cs_printf("%-26s(0x%-8x): %u\n", "timestamp_update_location ", (1<<14),cfg->timestamp_update_location );
    cs_printf("%-26s(0x%-8x): %u\n", "direct_registration_en    ", (1<<15),cfg->direct_registration_en    );
    cs_printf("%-26s(0x%-8x): %u\n", "drain_window_guard_time   ", (1<<16),cfg->drain_window_guard_time   );
    cs_printf("%-26s(0x%-8x): %u\n", "mpcp_to_cpu_en            ", (1<<17),cfg->mpcp_to_cpu_en            );
    cs_printf("==========================================\n");
}

void __rtt_show(cs_aal_mpcp_rtt_comp_cfg_t *cfg)
{
    aal_mpcp_rtt_comp_cfg_get(cfg);
    
    cs_printf("\nRTT COMPANSATION CONFIGURATION\n");
    cs_printf("==========================================\n");
    cs_printf("%-26s(0x%-8x): %u\n", "mode                ", (1<<0), cfg->mode                );
    cs_printf("%-26s(0x%-8x): %u\n", "fec_comp_en         ", (1<<1), cfg->fec_comp_en         );
    cs_printf("%-26s(0x%-8x): %u\n", "fec_comp_cnt_inc_en ", (1<<2), cfg->fec_comp_cnt_inc_en );
    cs_printf("%-26s(0x%-8x): %u\n", "fec_comp_cnt        ", (1<<3), cfg->fec_comp_cnt        );
    cs_printf("%-26s(0x%-8x): %u\n", "txmac_comp          ", (1<<4), cfg->txmac_comp          );
    cs_printf("%-26s(0x%-8x): %u\n", "egress_path_comp    ", (1<<5), cfg->egress_path_comp    );
    cs_printf("%-26s(0x%-8x): %u\n", "rxmac_comp          ", (1<<6), cfg->rxmac_comp          );
    cs_printf("%-26s(0x%-8x): %u\n", "rxfec_aes_comp      ", (1<<7), cfg->rxfec_aes_comp      );
    cs_printf("%-26s(0x%-8x): %u\n", "rxfec_churn_comp    ", (1<<8), cfg->rxfec_churn_comp    );
    cs_printf("%-26s(0x%-8x): %u\n", "non_rxfec_aes_comp  ", (1<<9), cfg->non_rxfec_aes_comp  );
    cs_printf("%-26s(0x%-8x): %u\n", "non_rxfec_churn_comp", (1<<10),cfg->non_rxfec_churn_comp);
    cs_printf("==========================================\n");
}

void __queue_show(void)
{
    cs_aal_mpcp_queue_id_t id;
    cs_uint16 thre, max_thre;

    cs_printf("\nQUEUE THRESHOLD CONFIGURATION\n");
    cs_printf("==========================================\n");
    for(id = 0; id < 8; ++id)
    {
        aal_mpcp_queue_thrshld_get(id, &thre, &max_thre);
        cs_printf("queue %u : thre = %u, max_thre = %u\n", id, thre, max_thre);
    }
    cs_printf("==========================================\n");
}

void aal_mpcp_cmd(int argc, char **argv)
{
    cs_uint32 val,addr,tmp;
    int i = 0;

    if(2==argc)
    {
        __mpcp_help();
        return;
    }

    if(cmd_cmp(argv[2], "timer"))
    {
        cs_aal_mpcp_timer_cfg_t cfg;
        void* offset[] = 
        {
            &cfg.gatetimer_en         ,
            &cfg.mpcptimer_en         ,
            &cfg.reporttimer_en       ,
            &cfg.gatetimer_dereg_en   ,
            &cfg.mpcptimer_dereg_en   ,
            &cfg.timedrift_dereg_en   ,
            &cfg.gatetimer_threshold  ,
            &cfg.mpcptimer_threshold  ,
            &cfg.timedrift_threshold  ,
            &cfg.reporttimer_threshold
        };
        if(5==argc)
        {
            cs_aal_mpcp_timer_msk_t msk;
            msk.u32 = cs_atoi(argv[3]);
            tmp = msk.u32;
            val = cs_atoi(argv[4]);
            while(tmp) {tmp>>=1; ++i;}
            if(i)
            {
                addr = (cs_uint32)offset[i-1];
                if(!(addr&0x3)) *(cs_uint32*)addr = val;
                else if(!(addr&0x1)) *(cs_uint16*)addr = val;
                else *(cs_uint8*)addr = val;
                aal_mpcp_timer_cfg_set(msk, &cfg);
            }
        }
        __timer_show(&cfg);
    }
    else if(cmd_cmp(argv[2], "report"))
    {
        cs_aal_mpcp_report_cfg_t cfg;
        void* offset[] = 
        {
            &cfg.t_threshold        ,
            &cfg.zeroq_map          ,
            &cfg.zeroq_threshold    ,
            &cfg.aggregation_mode_en,
            &cfg.report_format      ,
            &cfg.rpt_order          ,
            &cfg.rpt_number         ,
            &cfg.rpt_bmp_qset1      ,
            &cfg.rpt_bmp_qset2      ,
            &cfg.schedule_fit_en    ,
            &cfg.report_fit_en      ,
            &cfg.quick_rpt_en       ,
            &cfg.ignore_rpt_en      ,
            &cfg.single_window_en   ,
            &cfg.cs_mode_en         ,
            &cfg.cs_mode_opt        ,
            &cfg.cs_mode_threshold  ,
            &cfg.le_mode_en         ,
            &cfg.le_mode_opt        ,
            &cfg.le_mode_bmp
        };
        if(5==argc)
        {
            cs_aal_mpcp_report_msk_t msk;
            msk.u32 = cs_atoi(argv[3]);
            tmp = msk.u32;
            val = cs_atoi(argv[4]);
            while(tmp) {tmp>>=1; ++i;}
            if(i)
            {
                addr = (cs_uint32)offset[i-1];
                if(!(addr&0x3)) *(cs_uint32*)addr = val;
                else if(!(addr&0x1)) *(cs_uint16*)addr = val;
                else *(cs_uint8*)addr = val;
                aal_mpcp_report_cfg_set(msk, &cfg);
            }
        }
        __report_show(&cfg);
    }
    else if(cmd_cmp(argv[2], "misc"))
    {
        cs_aal_mpcp_misc_cfg_t cfg;
        void* offset[] = 
        {
            &cfg.pending_grant             ,
            &cfg.min_grant_size            ,
            &cfg.request_frame_llid        ,
            &cfg.mpcp_frame_len_check_en   ,
            &cfg.min_process_time_check_en ,
            &cfg.min_process_time          ,
            &cfg.max_future_grant_time     ,
            &cfg.discovery_pkt_size        ,
            &cfg.onu_mac_addr_check_en     ,
            &cfg.mpcp_frame_fec_overhead   ,
            &cfg.strict_starttime_check_en ,
            &cfg.rereg_mode                ,
            &cfg.force_rpt_data_en         ,
            &cfg.tx_window_adjust_time     ,
            &cfg.timestamp_update_location ,
            &cfg.direct_registration_en    ,
            &cfg.drain_window_guard_time   ,
            &cfg.mpcp_to_cpu_en            
        };
        if(5==argc)
        {
            cs_aal_mpcp_misc_msk_t msk;
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
                aal_mpcp_misc_cfg_set(msk, &cfg);
            }
        }
        __misc_show(&cfg);
    }
    else if(cmd_cmp(argv[2], "rtt"))
    {
        cs_aal_mpcp_rtt_comp_cfg_t cfg;
        void* offset[] = 
        {
            &cfg.mode                ,
            &cfg.fec_comp_en         ,
            &cfg.fec_comp_cnt_inc_en ,
            &cfg.fec_comp_cnt        ,
            &cfg.txmac_comp          ,
            &cfg.egress_path_comp    ,
            &cfg.rxmac_comp          ,
            &cfg.rxfec_aes_comp      ,
            &cfg.rxfec_churn_comp    ,
            &cfg.non_rxfec_aes_comp  ,
            &cfg.non_rxfec_churn_comp
        };
        if(5==argc)
        {
            cs_aal_mpcp_rtt_comp_msk_t msk;
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
                aal_mpcp_rtt_comp_cfg_set(msk, &cfg);
            }
        }
        __rtt_show(&cfg);
    }
    else if(cmd_cmp(argv[2], "queue"))
    {        
        if(6==argc)
        {
            cs_aal_mpcp_queue_id_t id = cs_atoi(argv[3]);
            cs_uint16 thre = cs_atoi(argv[4]);
            cs_uint16 max_thre= cs_atoi(argv[5]);
            aal_mpcp_queue_thrshld_set(id, thre, max_thre);
        }
        __queue_show();
    }
    else
    {
        __mpcp_help();
    }
}


