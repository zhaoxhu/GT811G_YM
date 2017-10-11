#include "plat_common.h"
#include "cli_common.h"
#include "db_instance.h"
#include "sys_cfg.h"
#include "sdl_api.h"
#include "cs_utils.h"
#include "mc_type.h"
#include "mc_api.h"
#include "app_oam_ctc_adapt.h"
#include "mc_ctcctrl.h"
#include "sdl_pon.h"
#include "port_stats.h"
#include "app_ip_api.h"
#include "app_gpio.h"


#ifdef HAVE_TELNET_CLI
#define DFT_ENABLE_PASSWD           "enable"

#define USR_INDEX 0
#define PWD_INDEX 1

#define  CLI_MAX_CBS                        511744       
char admin_level[2][DB_MAX_USR_LEN+1]= {"admin","greenway"};
char user_level[2][DB_MAX_USR_LEN+1] = {"user","user"};
char guest_level[2][DB_MAX_USR_LEN+1]= {"guest","guest"};
char g_set_username[DB_MAX_USR_LEN+1]={0};
char g_set_privilege = 0;

#define ONU_HW_VER_LEN          32


unsigned int regular_count = 0;
unsigned int debug_regular = 0;

extern 
    void onu_hw_version_get(cs_uint8 *hw_vstr, cs_int32 len);
extern int cmd_show_run(struct cli_def *cli, char *command, char *argv[], int argc);
extern int cmd_show_run_system(struct cli_def *cli, char *command, char *argv[], int argc);
extern int cmd_show_run_ctc_vlan(struct cli_def *cli, char *command, char *argv[], int argc);
extern int cmd_show_run_igmp(struct cli_def *cli, char *command, char *argv[], int argc);
extern int cmd_show_run_qos(struct cli_def *cli, char *command, char *argv[], int argc);
extern int cmd_show_run_stp(struct cli_def *cli, char *command, char *argv[], int argc);
extern int do_telnet_legacy_cmd(struct cli_def *cli,char * p);

extern cs_uint16 mc_device_port_grp_cap(cs_dev_id_t dev);

extern cs_status ctc_onu_schedule_mode_set_adapt (
    cs_sdl_sched_type_t       sche_mode
    );
cs_status ctc_oam_onu_port_mode_set_adapt(cs_port_id_t port,cs_uint8  mode);



char *storm2string[] = 
{
    "BC",
    "BC+UMC",
    "BC+UMC+UUC",
    "disable"
};

char *mode2string[] = 
{
    "reserved",
    "1, Auto(10/100/1000Mbps)",
    "2, 10Mbps(half)",
    "3, 10Mbps(full)",
    "4, 100Mbps(half)",
    "5, 100Mbps(full)",
    "6, 1000Mbps(full)"
};


 extern const char *actualspeed2string[];

 extern const char *actualduplex2string[];

extern  const char *autoneg2string[];
 


cs_status ctc_port_storm_control_set_adapt (
    cs_port_id_t            port_id,
    cs_sdl_storm_ctrl_e storm_type,
    cs_boolean             enable,
    cs_uint32               rate
    );

int cli_int_configure_terminal(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    cli_set_configmode(cli, MODE_CONFIG, NULL);
    return CLI_OK;
}

int cmd_debug_mode_int(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    cli_set_configmode(cli, MODE_DEBUG, "debug");
    return CLI_OK;
}

int cmd_config_vlan_mode_int(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    cli_set_configmode(cli, MODE_CONFIG_VLAN, "vlan");
    return CLI_OK;
}

#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
int cmd_config_fdb_mode_int(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    cli_set_configmode(cli, MODE_CONFIG_FDB, "fdb");
    return CLI_OK;
}
#endif

int cmd_config_igmp_mode_int(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    cli_set_configmode(cli, MODE_CONFIG_IGMP, "igmp");
    
    return CLI_OK;
}


int cmd_config_uni_mode_int(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    cli_set_configmode(cli, MODE_CONFIG_UNI, "uni");
    
    return CLI_OK;
}

int cmd_config_qos_mode_int(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    cli_set_configmode(cli, MODE_CONFIG_QOS , "qos");
    
    return CLI_OK;
}

int cmd_config_pon_mode_int(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    cli_set_configmode(cli, MODE_CONFIG_PON , "pon");
    
    return CLI_OK;
}
extern char cli_cmd_reset[32];
extern int cli_cmd_state;
extern cs_uint8 cmd_reset_confirm;
int cmd_reset(struct cli_def *cli, char *command, char *argv[], int argc)
{
    
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }
    iros_system_reset(RESET_TYPE_FORCE);

    return CLI_OK;
}


extern int restore_factory_setting_reset();
int cmd_reset_factory(struct cli_def *cli, char *command, char *argv[], int argc)
{
    
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

  
    db_set_default();

    db_save_to_flash();
    cli_print(cli, "%% Restore to factory");
    cs_thread_delay(20);

    iros_system_reset(RESET_TYPE_FORCE);
   
    return CLI_OK;
}


static int g_cli_led_status = 1;
int cmd_led(struct cli_def *cli, char *command, char *argv[], int argc)
{
    unsigned char  led_mode=0;
        
    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,
                "on | off | normal", "Led on or Led off or normal behavior",
                 NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(1 == argc)
    {   
        if(strcasecmp(argv[0],"on") == 0){
            led_mode = LED_ON;
        }
        else if(strcasecmp(argv[0],"off") == 0){
            led_mode = LED_OFF;
        }
        else if(strcasecmp(argv[0],"normal") == 0){
            led_mode = LED_NORMAL;
        }
        else{
            cli_print(cli, "%% Invalid input. Correct format: led on|off|normal");
            return CLI_ERROR;
        }


        //TODO: ADD the LED API HERE
        cs_led_all_set(led_mode);

        
        g_cli_led_status = led_mode;
    } 
    else
    {
        cli_print(cli, "%% Invalid input. Correct format: led on|off|normal");
    }
    
    return CLI_OK;
}


int cmd_laser(struct cli_def *cli, char *command, char *argv[], int argc)
{
    unsigned char  laser_mode=0;
    cs_callback_context_t context;
    
        
    /* deal with help*/
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,
                "on | off | normal", "Laser on or off or normal",
                 NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(1 == argc)
    {   
        if(strcasecmp(argv[0],"on") == 0){
            laser_mode = SDL_PON_LASER_MODE_ON;
        }
        else if(strcasecmp(argv[0],"normal") == 0){
            laser_mode = SDL_PON_LASER_MODE_BURST;
        }
        else if(strcasecmp(argv[0],"off") == 0){
            laser_mode = SDL_PON_LASER_MODE_OFF;
        }
        else{
            cli_print(cli, "%% Invalid input.");
            return CLI_ERROR;
        }
        
        epon_request_onu_pon_laser_mode_set(context, ONU_DEVICEID_FOR_API,
                ONU_LLIDPORT_FOR_API, laser_mode);
        
    } else
    {
        cli_print(cli, "%% Invalid input.");
    }
    
    return CLI_OK;
}


int cmd_pon_mac(struct cli_def *cli, char *command, char *argv[], int argc)
{
    int mac1[CS_MACADDR_LEN];
    unsigned char mac2[CS_MACADDR_LEN];
    int ret ;

    int i;
    
    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,
                "<Mac>", "ONU  MAC Address",
                 NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }


    if(1 == argc)
    {   
        if(strlen(argv[0]) > 18){
            cli_print(cli,"MAC address configuration is not OK\n");
            return CLI_ERROR;
        }
        
        ret = sscanf(argv[0], "%x:%x:%x:%x:%x:%x", 
            &mac1[0], &mac1[1], &mac1[2], &mac1[3],&mac1[4],&mac1[5]);
        
        if(ret != 6 || mac1[0]&0x01){
            cli_print(cli,"Input MAC is not a unicast MAC\n");
            return CLI_ERROR;
        }

        for(i = 0 ; i < 6; i ++){
            mac2[i] = (unsigned char)mac1[i];
        }
/* Set ONU mac addr.*/
        startup_config_write(CFG_ID_MAC_ADDRESS, CS_MACADDR_LEN, mac2);
        
    } 
    else
    {
        cli_print(cli, "%% Invalid input.");
    }
    
    return CLI_OK;
}

int cli_get_onu_mac_addr(char *mac)
{
    cs_status ret;

    ret = startup_config_read(CFG_ID_MAC_ADDRESS, CS_MACADDR_LEN, mac);
    
    if (CS_E_OK != ret)
    {
        return 1;
    }

    sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", 
                               mac[0], 
                            mac[1], 
                            mac[2], 
                            mac[3], 
                            mac[4], 
                            mac[5]);
    return 0;

}


int cmd_show_pon_mac(struct cli_def *cli, char *command, char *argv[], int argc)
{
    char strMac[32];

    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }
    
    cli_get_onu_mac_addr(strMac);

    cli_print(cli, "Onu mac address is %s",strMac);
    return CLI_OK;
}

int cmd_show_version(struct cli_def *cli, char *command, char *argv[], int argc)
{
    char hd_version[32] = "V1.0.0";
    char sw_version[32]= ONU_SW_VERSION;

    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }
    
    strcpy(sw_version, ONU_SW_VERSION);
    cli_print(cli, "hardware version: %s", hd_version);

    cli_print(cli,  "software version: %s", sw_version);

    cli_print(cli, "firmware version: %d.%d.%d.%d",
                     cs_atoi(IROS_ONU_APP_VER_MAJOR),
                     cs_atoi(IROS_ONU_APP_VER_MINOR),
                     cs_atoi(IROS_ONU_APP_VER_REVISION),
                     cs_atoi(IROS_ONU_APP_VER_BUILD));
    
    return CLI_OK;
}


int cmd_show_led(struct cli_def *cli, char *command, char *argv[], int argc)
{
    unsigned int  led_mode = 0;
        
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    led_mode = g_cli_led_status;

    if(g_cli_led_status==LED_NORMAL)
        cli_print(cli,"The Led is Normal \n");
    else
        cli_print(cli,"The Led is %s \n",led_mode == 1?"on":"off");

    return CLI_OK;
}


int cmd_show_ups(struct cli_def *cli, char *command, char *argv[], int argc)
{
    unsigned char  ups_mode = 0;
    unsigned char  ups_status = 0;
    cs_callback_context_t context;
        
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    cs_plat_gpio_read(context, 0, 0, GPIO_PIN7, &ups_mode);

    cli_print(cli,"The UPS is %s \n",ups_mode == 0?"on":"off");

    if(ups_mode == 0){
        cs_plat_gpio_read(context, 0, 0,GPIO_PIN4, &ups_status);
        
        cli_print(cli,"The UPS voltage alarm is %s \n",ups_status == 0?"off":"on");
    }
    
    return CLI_OK;
}

int cmd_show_laser(struct cli_def *cli, char *command, char *argv[], int argc)
{
    unsigned int  laser_mode = 0;
    cs_callback_context_t context;
    
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    epon_request_onu_pon_laser_mode_get(context, ONU_DEVICEID_FOR_API,
            ONU_LLIDPORT_FOR_API, &laser_mode);

    if(laser_mode == SDL_PON_LASER_MODE_ON){
        cli_print(cli,"The Laser is on\n");
    }
    else if(laser_mode == SDL_PON_LASER_MODE_OFF){
        cli_print(cli,"The Laser is off \n");
    }
    else if(laser_mode == SDL_PON_LASER_MODE_BURST){
        cli_print(cli,"The Laser is normal status\n");
    }
    else{
        cli_print(cli,"The Laser is invalid status\n");
    }
    
    return CLI_OK;
}

int show_stat_pon(struct cli_def *cli)
{
    cs_callback_context_t context;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_pon_stats_t stats;

    if(CS_E_OK !=epon_request_onu_pon_stats_get(context, 0, port, 0, &stats))
    {
        return CLI_ERROR;
    }

	#if 1
	//added by zhuxh
	{
		cs_uint32 in_bytes_prev = stats.pon_byte_cnt;
		cs_uint32 out_bytes_prev = stats.pon_tx_byte_cnt;
		cs_uint32 delay_second = 1;
		cs_thread_delay(delay_second*1000);
		if(CS_E_OK == epon_request_onu_pon_stats_get(context, 0, port, 0, &stats))
		{
			char not_support[] = "NA";
			cs_uint32 in_bytes_last = stats.pon_byte_cnt;
			cs_uint32 out_bytes_last = stats.pon_tx_byte_cnt;
			cs_uint32 in_bytes_rate = 0;
			cs_uint32 out_bytes_rate = 0;
			cs_uint32 in_good_pkts = 0;
			cs_uint32 out_good_pkts = 0;
			cs_uint32 in_mpcp_Gate = 0;
			cs_uint32 in_mpcp_RegAck = 0;
			cs_uint32 in_mpcp_Register = 0;
			cs_uint32 in_mpcp_RegRequest = 0;
			cs_uint32 in_mpcp_Report = 0;
			cs_uint32 out_mpcp_Gate = 0;
			cs_uint32 out_mpcp_RegAck = 0;
			cs_uint32 out_mpcp_Register = 0;
			cs_uint32 out_mpcp_RegRequest = 0;
			cs_uint32 out_mpcp_Report = 0;
			
			in_bytes_rate = (in_bytes_last - in_bytes_prev)/delay_second;
			out_bytes_rate = (out_bytes_last - out_bytes_prev)/delay_second;
			
			in_good_pkts = stats.pon_frame_cnt + stats.pon_mcframe_cnt + stats.pon_bcframe_cnt;
			out_good_pkts = stats.pon_txframe_cnt + stats.pon_txmcframe_cnt + stats.pon_txbcframe_cnt;
			in_mpcp_Gate =	stats.pon_mpcp_uc_gat_rev + stats.pon_mpcp_uc_gat_norm + stats.pon_mpcp_uc_gat_frpt + stats.pon_mpcp_uc_gat_udis + stats.pon_mpcp_uc_gat_bdis 
						  + stats.pon_mpcp_mc_gat_rev + stats.pon_mpcp_mc_gat_norm + stats.pon_mpcp_mc_gat_frpt + stats.pon_mpcp_mc_gat_udis + stats.pon_mpcp_mc_gat_bdis 
						  + stats.pon_mpcp_bc_gat_rev + stats.pon_mpcp_bc_gat_norm + stats.pon_mpcp_bc_gat_frpt + stats.pon_mpcp_bc_gat_udis + stats.pon_mpcp_bc_gat_bdis;

			in_mpcp_Register =	stats.pon_mpcp_uc_reg_ack + stats.pon_mpcp_uc_reg_nack + stats.pon_mpcp_uc_reg_dereg + stats.pon_mpcp_uc_reg_rereg + stats.pon_mpcp_uc_reg_reg_rev 
						  	  + stats.pon_mpcp_mc_reg_ack + stats.pon_mpcp_mc_reg_nack + stats.pon_mpcp_mc_reg_dereg + stats.pon_mpcp_mc_reg_rereg + stats.pon_mpcp_mc_reg_reg_rev 
						  	  + stats.pon_mpcp_bc_reg_ack + stats.pon_mpcp_bc_reg_nack + stats.pon_mpcp_bc_reg_dereg + stats.pon_mpcp_bc_reg_rereg + stats.pon_mpcp_bc_reg_reg_rev;

			out_mpcp_RegAck = stats.pon_mpcp_tx_ack_ack + stats.pon_mpcp_tx_ack_nack;
			
			out_mpcp_RegRequest = stats.pon_mpcp_tx_req_dereg + stats.pon_mpcp_tx_req_reg;
			
			out_mpcp_Report = stats.pon_mpcp_tx_rpt;
			
			cs_printf("Interface PON: \n");
			cs_printf("%-20s:%-20s %-20s:%-20s\n",		"In Bytes Error",		not_support,					"Out Bytes Error",		not_support);
			cs_printf("%-20s:%-20lld %-20s:%-20lld\n",	"In Bytes Good",		stats.pon_byte_cnt, 			"Out Bytes Total",		stats.pon_tx_byte_cnt);
			cs_printf("%-20s:%-20u %-20s:%-20u\n",		"In Bytes Rate",		in_bytes_rate, 					"Out Bytes Rate",		out_bytes_rate);
			cs_printf("%-20s:%-20u %-20s:%-20u\n",		"In Good Pkts", 		in_good_pkts,					"Out Total Pkts",		out_good_pkts);
			cs_printf("%-20s:%-20u %-20s:%-20u\n",		"In Unicast Pkts",		stats.pon_frame_cnt,			"Out Unicast Pkts", 	stats.pon_txframe_cnt);
			cs_printf("%-20s:%-20u %-20s:%-20u\n",		"In Multicast Pkts",	stats.pon_mcframe_cnt,			"Out Multicast Pkts",	stats.pon_txmcframe_cnt);
			cs_printf("%-20s:%-20u %-20s:%-20u\n",		"In Broadcast Pkts",	stats.pon_bcframe_cnt,			"Out Broadcast Pkts",	stats.pon_txbcframe_cnt);
			cs_printf("%-20s:%-20u %-20s:%-20u\n",		"In Control Frames",	stats.pon_ctrlframe_cnt,		"Out Control Frames",	stats.pon_txctrlframe_cnt);
			cs_printf("%-20s:%-20u %-20s:%-20u\n",		"In Pause Frames",		stats.pon_pauseframe_cnt,		"Out Pause Frames", 	stats.pon_txpausecnt);
			cs_printf("%-20s:%-20u %-20s:%-20u\n",		"In CRC Error Pkts",	stats.pon_crcerr_cnt,			"Out CRC Error Pkts",	stats.pon_tx_crc_err);
			cs_printf("%-20s:%-20u %-20s:%-20u\n",		"In Jumbo Pkts",		stats.pon_oversize_cnt, 		"Out Jumbo Pkts",		stats.pon_txoversizecnt);
			cs_printf("%-20s:%-20s %-20s:%-20s\n",		"In Dropped Pkts",		not_support,					"Out Dropped Pkts", 	not_support);
			cs_printf("%-20s:%-20d %-20s:%-20d\n",		"In MPCP Gate", 		in_mpcp_Gate,					"Out MPCP Gate",		out_mpcp_Gate);
			cs_printf("%-20s:%-20d %-20s:%-20d\n",		"In MPCP RegAck",		in_mpcp_RegAck,					"Out MPCP RegAck",		out_mpcp_RegAck);
			cs_printf("%-20s:%-20d %-20s:%-20d\n",		"In MPCP Register", 	in_mpcp_Register,				"Out MPCP Register",	out_mpcp_Register);
			cs_printf("%-20s:%-20d %-20s:%-20d\n",		"In MPCP RegRequest",	in_mpcp_RegRequest,				"Out MPCP RegRequest",	out_mpcp_RegRequest);
			cs_printf("%-20s:%-20d %-20s:%-20d\n",		"In MPCP Report",		in_mpcp_Report,					"Out MPCP Report",		out_mpcp_Report);
		}
	}
	#else
	//sdk use, modified by zhuxh
    cli_print(cli,"\nPON STATISTICS");
    cli_print(cli,"========================================");    
    cli_print(cli,"%-30s: %llu", "pon_byte_cnt         ", stats.pon_byte_cnt           );
    cli_print(cli,"%-30s: %llu", "pon_tx_byte_cnt      ", stats.pon_tx_byte_cnt        );
    cli_print(cli,"%-30s: %u", "pon_frame_cnt          ", stats.pon_frame_cnt          );
    cli_print(cli,"%-30s: %u", "pon_txframe_cnt        ", stats.pon_txframe_cnt        );
    cli_print(cli,"%-30s: %u", "pon_crcerr_cnt         ", stats.pon_crcerr_cnt         );
    cli_print(cli,"%-30s: %u", "pon_mcframe_cnt        ", stats.pon_mcframe_cnt        );
    cli_print(cli,"%-30s: %u", "pon_bcframe_cnt        ", stats.pon_bcframe_cnt        );
    cli_print(cli,"%-30s: %u", "pon_txmcframe_cnt      ", stats.pon_txmcframe_cnt      );
    cli_print(cli,"%-30s: %u", "pon_txbcframe_cnt      ", stats.pon_txbcframe_cnt      );
    cli_print(cli,"%-30s: %u", "pon_ctrlframe_cnt      ", stats.pon_ctrlframe_cnt      );
    cli_print(cli,"%-30s: %u", "pon_txctrlframe_cnt    ", stats.pon_txctrlframe_cnt    );
    cli_print(cli,"%-30s: %u", "pon_pauseframe_cnt     ", stats.pon_pauseframe_cnt     );
    cli_print(cli,"%-30s: %u", "pon_unknownop_cnt      ", stats.pon_unknownop_cnt      );
    cli_print(cli,"%-30s: %u", "pon_runt_cnt           ", stats.pon_runt_cnt           );
    cli_print(cli,"%-30s: %u", "pon_oversize_cnt       ", stats.pon_oversize_cnt       );
    cli_print(cli,"%-30s: %u", "pon_rmon64_cnt         ", stats.pon_rmon64_cnt         );
    cli_print(cli,"%-30s: %u", "pon_rmon65_127_cnt     ", stats.pon_rmon65_127_cnt     );
    cli_print(cli,"%-30s: %u", "pon_rmon128_255_cnt    ", stats.pon_rmon128_255_cnt    );
    cli_print(cli,"%-30s: %u", "pon_rmon256_511_cnt    ", stats.pon_rmon256_511_cnt    );
    cli_print(cli,"%-30s: %u", "pon_rmon512_1023_cnt   ", stats.pon_rmon512_1023_cnt   );
    cli_print(cli,"%-30s: %u", "pon_rmon1024_1518_cnt  ", stats.pon_rmon1024_1518_cnt  );
    cli_print(cli,"%-30s: %u", "pon_txrmon64_cnt       ", stats.pon_txrmon64_cnt       );
    cli_print(cli,"%-30s: %u", "pon_txrmon65_127_cnt   ", stats.pon_txrmon65_127_cnt   );
    cli_print(cli,"%-30s: %u", "pon_txrmon128_255_cnt  ", stats.pon_txrmon128_255_cnt  );
    cli_print(cli,"%-30s: %u", "pon_txrmon256_511_cnt  ", stats.pon_txrmon256_511_cnt  );
    cli_print(cli,"%-30s: %u", "pon_txrmon512_1023_cnt ", stats.pon_txrmon512_1023_cnt );
    cli_print(cli,"%-30s: %u", "pon_txrmon1024_1518_cnt", stats.pon_txrmon1024_1518_cnt);
    cli_print(cli,"%-30s: %u", "pon_mpcp_uc_rev_cnt     ", stats.pon_mpcp_uc_rev_cnt     );
    cli_print(cli,"%-30s: %u", "pon_mpcp_uc_reg_ack     ", stats.pon_mpcp_uc_reg_ack     );
    cli_print(cli,"%-30s: %u", "pon_mpcp_uc_reg_nack    ", stats.pon_mpcp_uc_reg_nack    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_uc_reg_dereg   ", stats.pon_mpcp_uc_reg_dereg   );
    cli_print(cli,"%-30s: %u", "pon_mpcp_uc_reg_rereg   ", stats.pon_mpcp_uc_reg_rereg   );
    cli_print(cli,"%-30s: %u", "pon_mpcp_uc_reg_reg_rev ", stats.pon_mpcp_uc_reg_reg_rev );
    cli_print(cli,"%-30s: %u", "pon_mpcp_uc_gat_rev     ", stats.pon_mpcp_uc_gat_rev     );
    cli_print(cli,"%-30s: %u", "pon_mpcp_uc_gat_norm    ", stats.pon_mpcp_uc_gat_norm    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_uc_gat_frpt    ", stats.pon_mpcp_uc_gat_frpt    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_uc_gat_udis    ", stats.pon_mpcp_uc_gat_udis    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_uc_gat_bdis    ", stats.pon_mpcp_uc_gat_bdis    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_mc_rev_cnt     ", stats.pon_mpcp_mc_rev_cnt     );
    cli_print(cli,"%-30s: %u", "pon_mpcp_mc_reg_ack     ", stats.pon_mpcp_mc_reg_ack     );
    cli_print(cli,"%-30s: %u", "pon_mpcp_mc_reg_nack    ", stats.pon_mpcp_mc_reg_nack    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_mc_reg_dereg   ", stats.pon_mpcp_mc_reg_dereg   );
    cli_print(cli,"%-30s: %u", "pon_mpcp_mc_reg_rereg   ", stats.pon_mpcp_mc_reg_rereg   );
    cli_print(cli,"%-30s: %u", "pon_mpcp_mc_reg_reg_rev ", stats.pon_mpcp_mc_reg_reg_rev );
    cli_print(cli,"%-30s: %u", "pon_mpcp_mc_gat_rev     ", stats.pon_mpcp_mc_gat_rev     );
    cli_print(cli,"%-30s: %u", "pon_mpcp_mc_gat_norm    ", stats.pon_mpcp_mc_gat_norm    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_mc_gat_frpt    ", stats.pon_mpcp_mc_gat_frpt    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_mc_gat_udis    ", stats.pon_mpcp_mc_gat_udis    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_mc_gat_bdis    ", stats.pon_mpcp_mc_gat_bdis    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_bc_rev_cnt     ", stats.pon_mpcp_bc_rev_cnt     );
    cli_print(cli,"%-30s: %u", "pon_mpcp_bc_reg_ack     ", stats.pon_mpcp_bc_reg_ack     );
    cli_print(cli,"%-30s: %u", "pon_mpcp_bc_reg_nack    ", stats.pon_mpcp_bc_reg_nack    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_bc_reg_dereg   ", stats.pon_mpcp_bc_reg_dereg   );
    cli_print(cli,"%-30s: %u", "pon_mpcp_bc_reg_rereg   ", stats.pon_mpcp_bc_reg_rereg   );
    cli_print(cli,"%-30s: %u", "pon_mpcp_bc_reg_reg_rev ", stats.pon_mpcp_bc_reg_reg_rev );
    cli_print(cli,"%-30s: %u", "pon_mpcp_bc_gat_rev     ", stats.pon_mpcp_bc_gat_rev     );
    cli_print(cli,"%-30s: %u", "pon_mpcp_bc_gat_norm    ", stats.pon_mpcp_bc_gat_norm    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_bc_gat_frpt    ", stats.pon_mpcp_bc_gat_frpt    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_bc_gat_udis    ", stats.pon_mpcp_bc_gat_udis    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_bc_gat_bdis    ", stats.pon_mpcp_bc_gat_bdis    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_tx_req_dereg   ", stats.pon_mpcp_tx_req_dereg   );
    cli_print(cli,"%-30s: %u", "pon_mpcp_tx_req_reg     ", stats.pon_mpcp_tx_req_reg     );
    cli_print(cli,"%-30s: %u", "pon_mpcp_tx_ack_ack     ", stats.pon_mpcp_tx_ack_ack     );
    cli_print(cli,"%-30s: %u", "pon_mpcp_tx_ack_nack    ", stats.pon_mpcp_tx_ack_nack    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_tx_rpt    ", stats.pon_mpcp_tx_rpt    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_tx_oam    ", stats.pon_mpcp_tx_oam    );
    cli_print(cli,"%-30s: %u", "pon_mpcp_da_mc_nslf", stats.pon_mpcp_da_mc_nslf);
    cli_print(cli,"%-30s: %u", "pon_mpcp_da_mc_slf ", stats.pon_mpcp_da_mc_slf );
    cli_print(cli,"%-30s: %u", "pon_mpcp_da_uc     ", stats.pon_mpcp_da_uc     );
    cli_print(cli,"%-30s: %u", "pon_framecnt       ", stats.pon_framecnt       );
    cli_print(cli,"%-30s: %u", "pon_txoversizecnt  ", stats.pon_txoversizecnt  );
    cli_print(cli,"%-30s: %u", "pon_rx1519_maxcnt  ", stats.pon_rx1519_maxcnt  );
    cli_print(cli,"%-30s: %u", "pon_tx1519_maxcnt  ", stats.pon_tx1519_maxcnt  );
    cli_print(cli,"%-30s: %u", "pon_txframecnt     ", stats.pon_txframecnt     );
    cli_print(cli,"%-30s: %u", "pon_txpausecnt     ", stats.pon_txpausecnt     );
    cli_print(cli,"%-30s: %u", "pon_undersizecnt   ", stats.pon_undersizecnt   );
    cli_print(cli,"%-30s: %u", "pon_jabbercnt      ", stats.pon_jabbercnt      );
    cli_print(cli,"%-30s: %u", "pon_extensioncnt   ", stats.pon_extensioncnt   );
    cli_print(cli,"%-30s: %u", "pon_txextensioncnt ", stats.pon_txextensioncnt );
    cli_print(cli,"%-30s: %u", "pon_err_symbol     ", stats.pon_err_symbol     );
    cli_print(cli,"%-30s: %u", "pon_outofsync      ", stats.pon_outofsync      );
    cli_print(cli,"%-30s: %u", "pon_sld_err        ", stats.pon_sld_err        );
    cli_print(cli,"%-30s: %u", "pon_crc8_err       ", stats.pon_crc8_err       );
    cli_print(cli,"%-30s: %u", "pon_mac_drop       ", stats.pon_mac_drop       );
    cli_print(cli,"%-30s: %u", "pon_ipg_drop       ", stats.pon_ipg_drop       );
    cli_print(cli,"%-30s: %u", "pon_drain          ", stats.pon_drain          );
    cli_print(cli,"%-30s: %u", "pon_tx_crc_err     ", stats.pon_tx_crc_err     );
    cli_print(cli,"========================================");
	#endif
	
    return CLI_OK;
}

int show_stat_cpu(struct cli_def *cli)
{
	cli_print(cli,"%-30s: %d", "frm_rx_eapol", app_pkt_get_counter(CS_PKT_8021X));
    cli_print(cli,"%-30s: %d", "frm_rx_igmp", app_pkt_get_counter(CS_PKT_GMP));
    cli_print(cli,"%-30s: %d", "frm_rx_imstar", app_pkt_get_counter(CS_PKT_MYMAC));
    cli_print(cli,"%-30s: %d", "frm_rx_ip", app_pkt_get_counter(CS_PKT_IP));
    cli_print(cli,"%-30s: %d", "frm_rx_arp",app_pkt_get_counter(CS_PKT_ARP));
    cli_print(cli,"%-30s: %d", "frm_rx_dhcp",app_pkt_get_counter(CS_PKT_DHCP));
    cli_print(cli,"%-30s: %d", "frm_rx_stp",app_pkt_get_counter(CS_PKT_BPDU));
	
	return CLI_OK;
}

void stats_reset_cpu()
{
	 int i = 0;
	 cs_pkt_type_t pkt_type;
	 for(i=0; i<CS_PKT_TYPE_NUM; i++)
	 {
	 	pkt_type = i;
		app_pkt_clr_counter(pkt_type);
	 }

	 return;
}


int show_stat_uni(struct cli_def *cli, cs_uint8 port_id)
{
    oam_port_uni_stats_t uni_stats;
    cs_uint64 zero_cnt = 0;
    cs_uint8 max_uni_port=0;
    
    if(CS_E_OK != startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &max_uni_port))
    {
        max_uni_port = 1;
    }
    
    if ((port_id > max_uni_port)|| (port_id < 1) ) {
        cli_print(cli, "%% Invalid port ID.");
        return CLI_ERROR;
    }

    if(CS_E_OK != app_onu_port_stats_get(port_id, &uni_stats))
    {
        return CLI_ERROR;
    }

	#if 1
	//added by zhuxh
	cli_print(cli, "\n-------------------port :%d-------------------", port_id);
	//cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu",	"In bytes rate",		rxrate,							"Out bytes rate",		txrate);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In bytes",				uni_stats.rxbyte_cnt,			"Out bytes",			uni_stats.txbyte_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In total pkts",		uni_stats.rxfrm_cnt,			"Out total pkts",		uni_stats.txfrm_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In unicast pkts",		uni_stats.rxucfrm_cnt,			"Out unicast pkts",		uni_stats.txucfrm_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In multicast pkts",	uni_stats.rxmcfrm_cnt,			"Out multicast pkts",	uni_stats.txmcfrm_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In broadcast pkts",	uni_stats.rxbcfrm_cnt,			"Out broadcast pkts",	uni_stats.txbcfrm_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In pause pkts",		uni_stats.rxpausefrm_cnt,		"Out pause pkts",		uni_stats.txpausefrm_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In crc error pkts",	uni_stats.rxcrcerrfrm_cnt,		"Out crc error pkts",	uni_stats.txcrcerrfrm_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In jumbo pkts",		uni_stats.rxoversizefrm_cnt,	"Out jumbo pkts",		(cs_uint64)zero_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In undersize pkts",	uni_stats.rxundersizefrm_cnt,	"Out undersize pkts",	(cs_uint64)zero_cnt);
	#else
	//sdk use, modified by zhuxh
	cli_print(cli, "\n-------------------port :%d-------------------", port_id);
    cli_print(cli,"%-30s: %llu", "RxFramesOk", uni_stats.rxfrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxUnicasts" , uni_stats.rxucfrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxMulticasts", uni_stats.rxmcfrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxBroadcasts", uni_stats.rxbcfrm_cnt);
    cli_print(cli,"%-30s: %llu", "Rx64Octets",  uni_stats.rxstatsfrm64_cnt);
    cli_print(cli,"%-30s: %llu", "Rx127Octets",  uni_stats.rxstatsfrm65_127_cnt);
    cli_print(cli,"%-30s: %llu", "Rx255Octets",uni_stats.rxstatsfrm128_255_cnt);
    cli_print(cli,"%-30s: %llu", "Rx511Octets", uni_stats.rxstatsfrm256_511_cnt);
    cli_print(cli,"%-30s: %llu", "Rx1023Octets", uni_stats.rxstatsfrm512_1023_cnt);
    cli_print(cli,"%-30s: %llu", "RxMaxOctets",  uni_stats.rxbyte_cnt);
    cli_print(cli,"%-30s: %llu", "RxJumboOctets", zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxUndersize", uni_stats.rxundersizefrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxOversize", uni_stats.rxoversizefrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxFragments", zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxJabber",  uni_stats.rxjabberfrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxFCSErrors",  (cs_uint64)uni_stats.fcs_errors);
    cli_print(cli,"%-30s: %llu", "RxDiscards", zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxAlignErrors", zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxIntMACErrors", zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxPppoes",  zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxQueueFull", zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxPause" , uni_stats.rxpausefrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxOctetsOkMsb",  zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxOctetsOkLsb", uni_stats.rxbyte_cnt);
    cli_print(cli,"%-30s: %llu", "TxFramesOk",  uni_stats.txfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxUnicasts" , uni_stats.txucfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxMulticasts", uni_stats.txmcfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxBroadcasts", uni_stats.txbcfrm_cnt);
    cli_print(cli,"%-30s: %llu", "Tx64Octets", uni_stats.txstatsfrm64_cnt);
    cli_print(cli,"%-30s: %llu", "Tx127Octets", uni_stats.txstatsfrm65_127_cnt);
    cli_print(cli,"%-30s: %llu", "Tx255Octets", uni_stats.txstatsfrm128_255_cnt);
    cli_print(cli,"%-30s: %llu", "Tx511Octets" ,  uni_stats.txstatsfrm256_511_cnt);
    cli_print(cli,"%-30s: %llu", "Tx1023Octets", uni_stats.txstatsfrm512_1023_cnt);
    cli_print(cli,"%-30s: %llu", "TxMaxOctets",  uni_stats.txbyte_cnt);
    cli_print(cli,"%-30s: %llu", "TxJumboOctets", zero_cnt);
    cli_print(cli,"%-30s: %llu", "TxDeferred",  zero_cnt);
    cli_print(cli,"%-30s: %llu", "TxTooLongFrames",  uni_stats.txoversizefrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxCarrierErrFrames",  zero_cnt);
    cli_print(cli,"%-30s: %llu", "TxSqeErrFrames", zero_cnt);
    cli_print(cli,"%-30s: %llu", "TxSingleCollisions", uni_stats.txsinglecolfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxMultipleCollisions",  uni_stats.txmulticolfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxExcessiveCollisions", uni_stats.txexesscolfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxLateCollisions", uni_stats.txlatecolfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxMacErrFrames", zero_cnt);
    cli_print(cli,"%-30s: %llu", "TxQueueFull ", uni_stats.txexesscolfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxPause",  uni_stats.txpausefrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxOctetsOk" , uni_stats.txbyte_cnt);
	#endif
	
    return CLI_OK;
    
}


#ifdef HAVE_STATISTICS_NNI
int show_stat_nni(struct cli_def *cli)
{
    oam_port_uni_stats_t uni_stats;
    cs_uint64 zero_cnt = 0;

    if(CS_E_OK != app_onu_nni_stats_get(&uni_stats))
    {
        return CLI_ERROR;
    }

	#if 1
	//added by zhuxh
	cli_print(cli,	"Interface nni: \n");
	//cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu",	"In bytes rate",		rxrate,							"Out bytes rate",		txrate);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In bytes",				uni_stats.rxbyte_cnt,			"Out bytes",			uni_stats.txbyte_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In total pkts",		uni_stats.rxfrm_cnt,			"Out total pkts",		uni_stats.txfrm_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In unicast pkts",		uni_stats.rxucfrm_cnt,			"Out unicast pkts",		uni_stats.txucfrm_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In multicast pkts",	uni_stats.rxmcfrm_cnt,			"Out multicast pkts",	uni_stats.txmcfrm_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In broadcast pkts",	uni_stats.rxbcfrm_cnt,			"Out broadcast pkts",	uni_stats.txbcfrm_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In pause pkts",		uni_stats.rxpausefrm_cnt,		"Out pause pkts",		uni_stats.txpausefrm_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In crc error pkts",	uni_stats.rxcrcerrfrm_cnt,		"Out crc error pkts",	uni_stats.txcrcerrfrm_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In jumbo pkts",		uni_stats.rxoversizefrm_cnt,	"Out jumbo pkts",		(cs_uint64)zero_cnt);
	cli_print(cli,	"%-30s: %-20llu \t %-30s: %-20llu", "In undersize pkts",	uni_stats.rxundersizefrm_cnt,	"Out undersize pkts",	(cs_uint64)zero_cnt);
	#else
	//sdk use, modified by zhuxh
	cli_print(cli, "\n-------------------port :%d-------------------", port_id);
    cli_print(cli,"%-30s: %llu", "RxFramesOk", uni_stats.rxfrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxUnicasts" , uni_stats.rxucfrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxMulticasts", uni_stats.rxmcfrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxBroadcasts", uni_stats.rxbcfrm_cnt);
    cli_print(cli,"%-30s: %llu", "Rx64Octets",  uni_stats.rxstatsfrm64_cnt);
    cli_print(cli,"%-30s: %llu", "Rx127Octets",  uni_stats.rxstatsfrm65_127_cnt);
    cli_print(cli,"%-30s: %llu", "Rx255Octets",uni_stats.rxstatsfrm128_255_cnt);
    cli_print(cli,"%-30s: %llu", "Rx511Octets", uni_stats.rxstatsfrm256_511_cnt);
    cli_print(cli,"%-30s: %llu", "Rx1023Octets", uni_stats.rxstatsfrm512_1023_cnt);
    cli_print(cli,"%-30s: %llu", "RxMaxOctets",  uni_stats.rxbyte_cnt);
    cli_print(cli,"%-30s: %llu", "RxJumboOctets", zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxUndersize", uni_stats.rxundersizefrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxOversize", uni_stats.rxoversizefrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxFragments", zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxJabber",  uni_stats.rxjabberfrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxFCSErrors",  (cs_uint64)uni_stats.fcs_errors);
    cli_print(cli,"%-30s: %llu", "RxDiscards", zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxAlignErrors", zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxIntMACErrors", zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxPppoes",  zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxQueueFull", zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxPause" , uni_stats.rxpausefrm_cnt);
    cli_print(cli,"%-30s: %llu", "RxOctetsOkMsb",  zero_cnt);
    cli_print(cli,"%-30s: %llu", "RxOctetsOkLsb", uni_stats.rxbyte_cnt);
    cli_print(cli,"%-30s: %llu", "TxFramesOk",  uni_stats.txfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxUnicasts" , uni_stats.txucfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxMulticasts", uni_stats.txmcfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxBroadcasts", uni_stats.txbcfrm_cnt);
    cli_print(cli,"%-30s: %llu", "Tx64Octets", uni_stats.txstatsfrm64_cnt);
    cli_print(cli,"%-30s: %llu", "Tx127Octets", uni_stats.txstatsfrm65_127_cnt);
    cli_print(cli,"%-30s: %llu", "Tx255Octets", uni_stats.txstatsfrm128_255_cnt);
    cli_print(cli,"%-30s: %llu", "Tx511Octets" ,  uni_stats.txstatsfrm256_511_cnt);
    cli_print(cli,"%-30s: %llu", "Tx1023Octets", uni_stats.txstatsfrm512_1023_cnt);
    cli_print(cli,"%-30s: %llu", "TxMaxOctets",  uni_stats.txbyte_cnt);
    cli_print(cli,"%-30s: %llu", "TxJumboOctets", zero_cnt);
    cli_print(cli,"%-30s: %llu", "TxDeferred",  zero_cnt);
    cli_print(cli,"%-30s: %llu", "TxTooLongFrames",  uni_stats.txoversizefrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxCarrierErrFrames",  zero_cnt);
    cli_print(cli,"%-30s: %llu", "TxSqeErrFrames", zero_cnt);
    cli_print(cli,"%-30s: %llu", "TxSingleCollisions", uni_stats.txsinglecolfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxMultipleCollisions",  uni_stats.txmulticolfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxExcessiveCollisions", uni_stats.txexesscolfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxLateCollisions", uni_stats.txlatecolfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxMacErrFrames", zero_cnt);
    cli_print(cli,"%-30s: %llu", "TxQueueFull ", uni_stats.txexesscolfrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxPause",  uni_stats.txpausefrm_cnt);
    cli_print(cli,"%-30s: %llu", "TxOctetsOk" , uni_stats.txbyte_cnt);
	#endif
	
    return CLI_OK;
    
}
#endif

int cmd_statistics_uni(struct cli_def *cli, char *command, char *argv[], int argc)
{
     cs_port_id_t portid=0;

     
    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
#ifdef HAVE_MPORTS
        case 1:
            return cli_arg_help(cli, 0,
                "<uni id>", "value in  (1, 4)",
                 NULL);
#else
        case 1:
            return cli_arg_help(cli, 0,
                "<uni id>", "value in  (1, 1)",
                 NULL);
#endif


        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }


    if(1 == argc)
    {
        portid=atoi(argv[0]);
        show_stat_uni(cli,portid);
    } 
    else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}

int cmd_statistics_cpu(struct cli_def *cli, char *command, char *argv[], int argc)
{

    // deal with help
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;
    
    if(0 == argc)
    {
        cli_print(cli,"%-30s: %d", "frm_rx_eapol", app_pkt_get_counter(CS_PKT_8021X));
        cli_print(cli,"%-30s: %d", "frm_rx_igmp", app_pkt_get_counter(CS_PKT_GMP));
        cli_print(cli,"%-30s: %d", "frm_rx_imstar", app_pkt_get_counter(CS_PKT_MYMAC));
        cli_print(cli,"%-30s: %d", "frm_rx_ip", app_pkt_get_counter(CS_PKT_IP));
        cli_print(cli,"%-30s: %d", "frm_rx_arp",app_pkt_get_counter(CS_PKT_ARP));
        cli_print(cli,"%-30s: %d", "frm_rx_dhcp",app_pkt_get_counter(CS_PKT_DHCP));
        cli_print(cli,"%-30s: %d", "frm_rx_stp",app_pkt_get_counter(CS_PKT_BPDU));
    } 
    else
    {
        cli_print(cli, "%% Invalid input.");
    }


    return CLI_OK;
}

int cmd_statistics_show(struct cli_def *cli, char *command, char *argv[], int argc)
{

     cs_port_id_t portid=0;
    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
#ifdef HAVE_MPORTS
        case 1:
            return cli_arg_help(cli, 0,
                "<port id>", "Pon:0, UNI:1-4",
                 NULL);
#else
        case 1:
            return cli_arg_help(cli, 0,
                "<port id>", "Pon:0, UNI:1",
                 NULL);
#endif
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(0 == argc)
    {
        cs_uint8 max_uni_port=0;

        show_stat_pon(cli);
        
        if(CS_E_OK != startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &max_uni_port))
        {
            max_uni_port = 1;
        }
        
        for(portid=1;portid<=max_uni_port;portid++)
        {
            show_stat_uni(cli,portid);
        }
    } 
    else if(1 == argc)
    {
        portid=atoi(argv[0]);
        if(portid)
            show_stat_uni(cli,portid);
        else
            show_stat_pon(cli);
    } 
    else
    {
        cli_print(cli, "%% Invalid input.");
    }
    
    return CLI_OK;
}

int cmd_statistics_clear(struct cli_def *cli, char *command, char *argv[], int argc)
{

    cs_port_id_t portid=0;
    cs_uint8 max_uni_port=0;
    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
#ifdef HAVE_MPORTS
        case 1:
            return cli_arg_help(cli, 0,
                "<port id>", "Pon:0, UNI:1-4",
                 NULL);
#else
        case 1:
            return cli_arg_help(cli, 0,
                "<port id>", "Pon:0, UNI:1",
                 NULL);
#endif
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }
    
    if(CS_E_OK != startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &max_uni_port))
    {
        max_uni_port = 1;
    }
    
    if(0 == argc)
    {
        app_stats_reset_pon();
               
        for(portid=1;portid<=max_uni_port;portid++)
        {
            app_stats_reset_uni((cs_port_id_t)portid);
        }
    } 
    else if(1 == argc)
    {
        portid=atoi(argv[0]);
        
        if(portid == 0)
        {
            app_stats_reset_pon();
        }
        else if(portid<=max_uni_port)
        {
            app_stats_reset_uni((cs_port_id_t)portid);
        }
    } 
    else
    {
        cli_print(cli, "%% Invalid input.");
    }
    return CLI_OK;
}


int cmd_serial_number(struct cli_def *cli, char *command, char *argv[], int argc)
{
    db_sn_t   sn_t; 
    int i;
    cs_status ret = CS_E_OK;
    
    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,
                "<String>", "ONU serial number",
                 NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }
 
    if(1 == argc)
    {   
        if(strlen(argv[0]) > 128){
            cli_print(cli,"Serial number configuration is not OK\n");
            return CLI_ERROR;
        }

        for(i = 0; i < strlen(argv[0]); i ++){
            if((argv[0][i] >= '0' &&  argv[0][i] <= '9')
                ||(argv[0][i] >= 'a' &&  argv[0][i] <= 'z')
                ||(argv[0][i] >= 'A' &&  argv[0][i] <= 'Z'))
                continue;

            cli_print(cli,"Serial number configuration is not OK\n");
            return CLI_ERROR;
        }

        memset(&sn_t,0x00,sizeof(sn_t));
        memcpy(sn_t.sn, argv[0], strlen(argv[0]));
        ret = db_write(DB_SN_ID,(cs_uint8 *) &sn_t, sizeof(sn_t));

        if(CS_E_OK != ret){
            cli_print(cli,"Serial number configuration is not OK\n");
            return CLI_ERROR;
        }

        ret = db_save_to_flash();
        if(CS_E_OK != ret){
            cli_print(cli,"Serial number configuration save is not OK\n");
            return CLI_ERROR;
        }
        
    } 
    else
    {
        cli_print(cli, "%% Invalid input.");
    }
    
    return CLI_OK;
}

int cmd_show_serial_number(struct cli_def *cli, char *command, char *argv[], int argc)
{
    db_sn_t   sn; 
    cs_uint8 onu_sn[DB_SN_LEN + 1];
    
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    memset(onu_sn,0x00,sizeof(onu_sn));
    memset(&sn,0x00,sizeof(sn));
    db_read(DB_SN_ID,(cs_uint8 *)&sn, sizeof(db_sn_t));
    memcpy(onu_sn,sn.sn,strlen(sn.sn));

    cli_print(cli, "ONU Serial number is %s \n", onu_sn);
    
    return CLI_OK;
}

#ifndef HAVE_MPORTS
extern void cs_gpio_laseron_detect_interval_set(cs_uint32 interval);
extern void cs_gpio_laseron_detect_interval_get(cs_uint32 *interval);
int cmd_laser_on_detect_interval_set(struct cli_def *cli, char *command, char *argv[], int argc)
{
    int     value=0;

    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,"<50-65535>", "Laser-on detect CLK interval",NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }
    
    if(1 == argc)
    {
        value= iros_strtol(argv[0]);
        if ((value > 0xFFFF)&&(value < 50)) {
            cli_print(cli, "%% Invalid input.");
            return CLI_ERROR;
        }

        cs_gpio_laseron_detect_interval_set(value);
        cli_print(cli, "Laser CLK interval set to %d ",value);

    } else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}

int cmd_laser_on_detect_interval_get(struct cli_def *cli, char *command, char *argv[], int argc)
{
    int value=0;

    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }
    
    cs_gpio_laseron_detect_interval_get(&value);
    cli_print(cli, "Laser CLK interval: %d ",value);

    return CLI_OK;
}

#endif



int cmd_web_timeout_interval(struct cli_def *cli, char *command, char *argv[], int argc)
{
    int value=0;
    db_timeout_t db_timeout;

    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 0:
            return cli_arg_help(cli, 0,"<60-65535>", "Get Web Timeout interval",NULL);
        case 1:
            return cli_arg_help(cli, 0,"<60-65535>", "Set Web Timeout interval",NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }
    
    db_read(DB_TIMEOUT_ID,(cs_uint8 *) &db_timeout, sizeof(db_timeout));
    if(0 == argc)
    {
        cli_print(cli, "%% Web Timeout interval: %d ",db_timeout.web_timeout);
    }else if(1 == argc)
    {
        value= iros_strtol(argv[0]);
        if ((value > 0xFFFF)||(value < 60)) {
            cli_print(cli, "%% Out of range.");
            return CLI_ERROR;
        }
        db_timeout.web_timeout = value;
        db_write(DB_TIMEOUT_ID,(cs_uint8 *) &db_timeout, sizeof(db_timeout));
        cli_print(cli, "%% Web Timeout interval set to %d ",value);
    }
    else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}



int cmd_telnet_timeout_interval(struct cli_def *cli, char *command, char *argv[], int argc)
{
    int value=0;
    db_timeout_t db_timeout;

    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 0:
            return cli_arg_help(cli, 0,"<60-65535>", "Get Telnet Timeout interval",NULL);
        case 1:
            return cli_arg_help(cli, 0,"<60-65535>", "Set Telnet Timeout interval",NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }
    
    db_read(DB_TIMEOUT_ID,(cs_uint8 *) &db_timeout, sizeof(db_timeout));
    if(0 == argc)
    {
        cli_print(cli, "%% Telnet Timeout interval: %d ",db_timeout.telnet_timeout);
    }else if(1 == argc)
    {
        value= iros_strtol(argv[0]);
        if ((value > 0xFFFF)||(value < 60)) {
            cli_print(cli, "%% Out of range.");
            return CLI_ERROR;
        }
        db_timeout.telnet_timeout = value;
        db_write(DB_TIMEOUT_ID,(cs_uint8 *) &db_timeout, sizeof(db_timeout));
        cli_print(cli, "%%Telnet Timeout interval set to %d ",value);
        extern void cli_set_idle_timeout(struct cli_def *cli, unsigned int seconds);
        cli_set_idle_timeout(cli,value);
    }
    else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}



int cmd_ctc_loid_password(struct cli_def *cli, char *command, char *argv[], int argc)
{
    int i;
    cs_status ret = CS_E_OK;
    db_loid_pwd_t db_loid;

    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,
                "<loid> <password>", "ONU ctc loid and password",
                 NULL);
        default:
            return cli_arg_help(cli, argc > 2, NULL);
        }
    }

    if(2 == argc)
    {   
        if(strlen(argv[0]) > DB_LOID_MAX_LEN){
            cli_print(cli,"User Loid configuration is not OK\n");
            return CLI_ERROR;
        }
        
        if(strlen(argv[1]) > DB_PWD_MAX_LEN){
            cli_print(cli,"User Password configuration is not OK\n");
            return CLI_ERROR;
        }
        
        for(i = 0; i < strlen(argv[0]); i ++){
            if((argv[0][i] >= '0' &&  argv[0][i] <= '9')
                ||(argv[0][i] >= 'a' &&  argv[0][i] <= 'z')
                ||(argv[0][i] >= 'A' &&  argv[0][i] <= 'Z'))
                continue;

            cli_print(cli,"User Loid configuration is not OK\n");
            return CLI_ERROR;
        }

        for(i = 0; i < strlen(argv[1]); i ++){
            if((argv[1][i] >= '0' &&  argv[1][i] <= '9')
                ||(argv[1][i] >= 'a' &&  argv[1][i] <= 'z')
                ||(argv[1][i] >= 'A' &&  argv[1][i] <= 'Z'))
                continue;

            cli_print(cli,"User Password configuration is not OK\n");
            return CLI_ERROR;
        }


        
        memset(&db_loid,0x00,sizeof(db_loid));
        strncpy(db_loid.loid, argv[0],DB_LOID_MAX_LEN);
        strncpy(db_loid.passwd,argv[1],DB_PWD_MAX_LEN);
        
        ret = db_write(DB_LOID_PASSWD_ID,(cs_uint8 *) &db_loid, sizeof(db_loid));
        if(ret != CS_E_OK){
            cli_print(cli, "set loid and password failed ,ret = %d \n",ret);
            return CLI_ERROR;
        }
    } 
    else
    {
        cli_print(cli, "%% Invalid input.");
    }
    
    return CLI_OK;
}

int cmd_show_ctc_loid_password(struct cli_def *cli, char *command, char *argv[], int argc)
{
    db_loid_pwd_t db_loid;
    cs_int8 loid[DB_LOID_MAX_LEN + 1];
    cs_int8 password[DB_PWD_MAX_LEN + 1];

    
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    memset(&db_loid,0x00,sizeof(db_loid));
    db_read(DB_LOID_PASSWD_ID,(cs_uint8 *) &db_loid, sizeof(db_loid));
    memset (loid, 0, sizeof ( loid));
    memset (password, 0, sizeof(password));

    strncpy(loid, db_loid.loid,DB_LOID_MAX_LEN);
    strncpy(password,db_loid.passwd,DB_PWD_MAX_LEN);
    
    cli_print(cli, "ONU user Loid is %s \n", loid);
    cli_print(cli, "ONU user Password is %s \n", password);
    
    return CLI_OK;
}

int cmd_ftp_download(struct cli_def *cli, char *command, char *argv[], int argc)
{    
    if(CLI_HELP_REQUESTED)
    {
        cli_print(cli,"ip-address directory username username password password");
        cli_print(cli,"ip-address xxx.xxx.xxx.xxx");/*argv[0]*/
        cli_print(cli,"directory  The firmware file path and name in FTP server.");/*argv[1]*/
        cli_print(cli,"           If the file path and name include space, ");
        cli_print(cli,"           please use \"\" to contain the directory");
        cli_print(cli,"username   FTP user name");/*argv[2 3]*/
        cli_print(cli,"password   FTP user password"); /*argv[4 5]*/
        return CLI_OK;        
    }
    if((argc !=6 ) || (strcasecmp(argv[2],"username") != 0)  || (strcasecmp(argv[4],"password") != 0))
    {
        cli_print(cli, "%% Invalid argvs.");
        return CLI_OK;  
    }

    /*call ftp download here*/
    cli_print(cli,"Ftp download %s:%s@%s %s\n",(char *)argv[3],(char *)argv[5],(char *)argv[0],(char *)argv[1]);    
    //ftp_get((char *)argv[0],(char *)argv[3],(char *)argv[5],(char *)argv[1],NULL,0, ftpclient_printf);          
  
    return CLI_OK;
}

int cmd_tftp_download(struct cli_def *cli, char *command, char *argv[], int argc)
{
//    int  tftp_err; 
    if(CLI_HELP_REQUESTED)
    {
        cli_print(cli,"Ip-address Directory");/*argv[0]*/
        cli_print(cli,"Ip-address xxx.xxx.xxx.xxx");/*argv[1]*/
        cli_print(cli,"Directory  The firmware file path and name in TFTP server.");
        cli_print(cli,"           If the file path and name include space,");
        cli_print(cli,"           please use \"\" to contain the directory");
        return CLI_OK;
    }
    if(argc !=2 )
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;  
    }
    /*call tftp download here*/
    cli_print(cli, "tftp download %s %s\n",argv[1],argv[0]);
   // tftp_client_get(argv[1], argv[0], TFTP_STD_PORT, NULL, 0, TFTP_OCTET, &tftp_err);
    
    return CLI_OK;
}


int cmd_show_version_detail(struct cli_def *cli, char *command, char *argv[], int argc)
{
    char hd_version[32] = "v1.0.0";
    char sw_version[32] = ONU_SW_VERSION;

    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }


    cli_print(cli,  "Software Version  : %s", sw_version);
    cli_print(cli,  "Firmware Version  : %d.%d.%d.%d",
                    cs_atoi(IROS_ONU_APP_VER_MAJOR),
                    cs_atoi(IROS_ONU_APP_VER_MINOR),
                    cs_atoi(IROS_ONU_APP_VER_REVISION),
                    cs_atoi(IROS_ONU_APP_VER_BUILD));
    cli_print(cli,  "Hardware Version  : %s", hd_version);
    cli_print(cli,  "Compiled          : %s %s", __DATE__,__TIME__);    
    return CLI_OK;
}


int cmd_save_config(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status ret = CS_E_OK;
    extern int save_config();
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }
    
    ret = db_save_to_flash();
    
    if(ret != CS_E_OK)
    cli_print(cli, "Save to flash fail");
    else
    cli_print(cli, "Save to flash success");
    return CLI_OK;
}




//interface IFNAME
int cmd_config_int(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,
                "IFNAME", "Interface name",
                NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(argc > 1)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    if (strcasecmp(argv[0], "0") == 0)
        cli_set_configmode(cli, MODE_CONFIG_INTF, "if[0]");
    else if (strcasecmp(argv[0], "1") == 0)
        cli_set_configmode(cli, MODE_CONFIG_INTF, "if[1]");
    else
        cli_print(cli, "Unknown interface %s", argv[0]);

    return CLI_OK;
}


//  igmp snooping enable max_ch <1-16>
//  igmp snooping enable

int cmd_snoop_enable(struct cli_def *cli, char *command, char *argv[], int argc)
{
    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,
                "max_ch", "Maximum number of channels",
                "<cr>", "",
                NULL);
        case 2:
            return cli_arg_help(cli, 0,
                "<1-16>", "default value 16",
                 NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(0 == argc)
    {
        cli_print(cli, "enable igmp snooping with default channel num");
    }
    else if(2 == argc && MATCH("max_ch", argv[0]))
    {
        cli_print(cli, "enable igmp snooping with max_ch %d", atoi(argv[1]));
    }
    else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}

int cmd_show_regular(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    cli_print(cli, "cli_regular() has run %u times", regular_count);
    return CLI_OK;
}



int cmd_debug_regular(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    debug_regular = !debug_regular;
    cli_print(cli, "cli_regular() debugging is %s", debug_regular ? "enabled" : "disabled");
    return CLI_OK;
}

cs_status check_inputdata(char *str,int max_len)
{
    int i = 0;
    int len=strlen(str);
  
    if(len>max_len)
        return CS_E_ERROR;
    for(i=0;i<len;i++)
    {
        char key = *(str+i);       
        if ((key <=32 && key !=8 ) || 
            (key >126 ) ||(key == 34 )||
            (key == 37 )||(key == 38 )||
            (key == 43) ||(key == 63))
        {        
            return CS_E_ERROR;
        }

    }

    return CS_OK;
}

int set_auth(int privilege,char *name, char *password,struct cli_def *cli)
{
    if(!name)
        name = g_set_username;

    if(privilege&&(privilege>cli->privilege))
        return CLI_ERROR; 
    
    if((strlen(name)>32)||(strlen(password)>32)||(!strlen(name))||(!strlen(password))){
        cli_print(cli,"\nUser name and password len must less than 32 \n");
        return CLI_ERROR; 
    }

    if(check_inputdata(password,DB_MAX_PWD_LEN))
        return CLI_ERROR; 
    
    if(g_set_privilege==PRIVILEGE_ADMIN)
    {
        memcpy(admin_level[USR_INDEX],0,DB_MAX_USR_LEN+1);
        memcpy(admin_level[PWD_INDEX],0,DB_MAX_PWD_LEN+1);    
        strcpy(admin_level[USR_INDEX],name);
        strcpy(admin_level[PWD_INDEX],password);
        {
            db_user_t telusr;        
            memset(&telusr,0,sizeof(db_user_t));
            memcpy(telusr.usr,admin_level[USR_INDEX],DB_MAX_USR_LEN);
            memcpy(telusr.pwd,admin_level[PWD_INDEX],DB_MAX_PWD_LEN);
            db_write(DB_TEL_ADMIN_INFO_ID,(cs_uint8 *)&telusr,sizeof(db_user_t));
            db_save_to_flash();
        }
        return CLI_OK;   
    }
    else if(g_set_privilege==PRIVILEGE_USER)
    {
        memcpy(user_level[USR_INDEX],0,DB_MAX_USR_LEN+1);
        memcpy(user_level[PWD_INDEX],0,DB_MAX_PWD_LEN+1); 
        strcpy(user_level[USR_INDEX],name);
        strcpy(user_level[PWD_INDEX],password);
        {
            db_user_t telusr;        
            memset(&telusr,0,sizeof(db_user_t));
            memcpy(telusr.usr,user_level[USR_INDEX],DB_MAX_USR_LEN);
            memcpy(telusr.pwd,user_level[PWD_INDEX],DB_MAX_PWD_LEN);
            db_write(DB_TEL_USER_INFO_ID,(cs_uint8 *)&telusr,sizeof(db_user_t));
            db_save_to_flash();
        }
        return CLI_OK;    
    }
    else if(g_set_privilege==PRIVILEGE_GUEST)
    {
        memcpy(guest_level[USR_INDEX],0,DB_MAX_USR_LEN+1);
        memcpy(guest_level[PWD_INDEX],0,DB_MAX_PWD_LEN+1); 
        strcpy(guest_level[USR_INDEX],name);
        strcpy(guest_level[PWD_INDEX],password);
        {
            db_user_t telusr;        
            memset(&telusr,0,sizeof(db_user_t));
            memcpy(telusr.usr,guest_level[USR_INDEX],DB_MAX_USR_LEN);
            memcpy(telusr.pwd,guest_level[PWD_INDEX],DB_MAX_PWD_LEN);
            db_write(DB_TEL_GUEST_INFO_ID,(cs_uint8 *)&telusr,sizeof(db_user_t));
            db_save_to_flash();
        }
        return CLI_OK;   
    }
    return CLI_ERROR;
}

int cmd_set_admin(struct cli_def *cli, char *command, char *argv[], int argc)
{

    if(CLI_HELP_REQUESTED)
    {
        return cli_arg_help(cli, 0,
                "<name>", "ONU admin name",NULL);
    }

    if(1 == argc)
    {
        if(cli->privilege < PRIVILEGE_ADMIN)
        {
            return cli_arg_help(cli, 0, "User Privilege is too low",NULL);
        }
        g_set_privilege = PRIVILEGE_ADMIN;
        if(strlen(argv[0])>DB_MAX_USR_LEN)
            return CLI_ERROR;
        if(check_inputdata(argv[0],DB_MAX_USR_LEN))
            return CLI_ERROR;
        memset(g_set_username,0,sizeof(g_set_username));
        strcpy(g_set_username,argv[0]);
        cli->state=STATE_CONFIRM_PASSWORD;
    }
    else
    {
        cli_print(cli, "%% set password after the username..");
    }
    return CLI_OK;
}

int cmd_set_user(struct cli_def *cli, char *command, char *argv[], int argc)
{

    if(CLI_HELP_REQUESTED)
    {
        return cli_arg_help(cli, 0,
                "<name>", "ONU user name",NULL);
    }

    if(1 == argc)
    {
        if(cli->privilege < PRIVILEGE_USER)
        {
            return cli_arg_help(cli, 0, "User Privilege is too low",NULL);
        }
        g_set_privilege = PRIVILEGE_USER;
        if(strlen(argv[0])>DB_MAX_USR_LEN)
            return CLI_ERROR;
        if(check_inputdata(argv[0],DB_MAX_USR_LEN))
            return CLI_ERROR;
        memset(g_set_username,0,sizeof(g_set_username));
        strcpy(g_set_username,argv[0]);
        cli->state=STATE_CONFIRM_PASSWORD;
    }
    else
    {
        cli_print(cli, "%% set password after the username..");
    }
    return CLI_OK;
}

int cmd_set_guest(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if(CLI_HELP_REQUESTED)
    {
        return cli_arg_help(cli, 0,
                "<name>>", "ONU guest name",NULL);
    }

    if(1 == argc)
    {
        if(cli->privilege < PRIVILEGE_GUEST)
        {
            return cli_arg_help(cli, 0, "User Privilege is too low",NULL);
        }
        g_set_privilege = PRIVILEGE_GUEST;
        if(strlen(argv[0])>DB_MAX_USR_LEN)
            return CLI_ERROR;
        if(check_inputdata(argv[0],DB_MAX_USR_LEN))
            return CLI_ERROR;
        memset(g_set_username,0,sizeof(g_set_username));
        strcpy(g_set_username,argv[0]);
        cli->state=STATE_CONFIRM_PASSWORD;
    }
    else
    {
        cli_print(cli, "%% set password after the username.");
    }
    return CLI_OK;
}

#if defined(HAVE_MINI_CLI)
extern void do_help(void);
extern int imst_cli_start(char *p);

#ifdef HAVE_GWD_OAM_PTY
extern int do_oam_pty_legacy_cmd(struct cli_def *cli,char *p);
#endif

int cmd_debug_legacy(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    if(CHANNEL_SERIAL == cli->channel)
    {
        cli_print(cli, "go to legacy CLI menu...");

        do_help();

#ifdef CYG_HAL_ARM_IMST_OLT
        imst_cli_start("olt->");
#else
        imst_cli_start("onu->");
#endif
    }
    
#ifdef HAVE_TELNET_CLI
     else if(CHANNEL_TCP == cli->channel)
        {
            cli_print(cli, "go to legacy CLI menu...");

            do_help();
        if(CLI_QUIT == do_telnet_legacy_cmd(cli,"onu->"))
          return CLI_QUIT;
        }
#endif /* HAVE_TELNET_CLI */
#ifdef HAVE_GWD_OAM_PTY
		else if(CHANNEL_PTY == cli->channel)
		{
			cli_print(cli, "go to legacy CLI menu...");
	
			do_help();
			
			if(CLI_QUIT == do_oam_pty_legacy_cmd(cli,"onu->"))
			{
				return CLI_QUIT;
			}
		}
#endif

    return CLI_OK;
}
#endif

void pc(struct cli_def *cli, char *string)
{
    printf("%s\n", string);
}

#ifdef HAVE_MPORTS
#define CLI_MAX_UNI_PORT_ID     4
#else
#define CLI_MAX_UNI_PORT_ID     1
#endif

#if 1
static char *_cmd_usage_vlan_mode_set = 
{
    "mode set <port ID> <mode> <pvid> [vlan1] [vlan2]...\
    \n    port ID: UNI port, 1\
    \n    mode: VLAN mode, 0~4. Mode 3 is not supportted currently.\
    \n        0--TRANSPARENT, format: mode set <port ID> <mode>\
    \n        1--TAG, format: mode set <port ID> <mode> <pvid>\
    \n        2--TRANSLATION, format: mode set <port ID> <mode> <pvid> [cvlan_x] [svlanx] ... \
    \n        4--TRUNK, format: mode set <port ID> <mode> <pvid> [vlan_x]  ...\
    \n    pvid: default vlan ID, 0~4095, invalid for transparent mode \
    \n    vlan_x: vlan ID, 1~4094, max value of x is 8 ",
};
/*\n        3--AGGREGATION, format: mode set <port ID> <mode> <pvid> [agg_vlan] [vlan_x] ...\ */

static char *_cmd_usage_vlan_mode_get = 
{
    "mode get [port ID]\
     \n    port ID: UNI port, 1 ",
};

#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
static char *_cmd_usage_igmp_mode_set = 
{
    "mode set <mode>\
    \n  mode: igmp mode, 0:disable, 1:enable ctc igmp, 2:enable igmp snooping",
};

static char *_cmd_usage_igmp_mode_get = 
{
    "mode get",
};
#endif

static char *_cmd_usage_igmp_aging_time_set = 
{
    "aging-time set <time>\
    \n  time: aging time, 1~260, uint is second ",
};
static char *_cmd_usage_igmp_aging_time_get = 
{
    "aging-time get",
};

static char *_cmd_usage_mc_tagstrip_set = 
{
    "strip set <port ID> <mode> [vid1] [iptv1] [vid2] [iptv2] [vid3] [iptv3] [vid4] [iptv4]\
    \n    port ID: UNI port, 1\
    \n  mode: tagstrip operate mode, 0~2, 0:no tagstrip, 1:tagstrip, 2:tag swap",
};

static char *_cmd_usage_mc_tagstrip_get = 
{
    "strip get <port ID>\
    \n    port ID: UNI port, 1",
};
#ifdef HAVE_MPORTS
static char *_cmd_usage_mc_max_grp_set = 
{
    "max-group set <port ID> <num> \
     \n    port ID: UNI port, 1 \
     \n  num: port max igmp group, 0~255",
};
#else
static char *_cmd_usage_mc_max_grp_set = 
{
    "max-group set <port ID> <num> \
     \n    port ID: UNI port, 1 \
     \n  num: port max igmp group, 0~64",
};
#endif
static char *_cmd_usage_mc_max_grp_get = 
{
    "max-group get <port ID>\
    \n    port ID: UNI port, 1",
};

static char *_cmd_usage_mc_fast_leave_set = 
{
    "fast-leave set <enable>\
    \n  enable:  0~1, 0:disable, 1:enable ",
};

static char *_cmd_usage_mc_fast_leave_get = 
{
    "fast-leave get",
};

static char *_cmd_usage_mc_config_get = 
{
    "show config",
};

static char *_cmd_usage_mc_grp_get = 
{
    "show group <port ID>\
    \n    port ID: UNI port, 1",
};

#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
static char *_cmd_usage_phy_admin_set = 
{
    "phy  set <port ID> <enable>\
    \n    port ID: UNI port, 1\
    \n  enable:  admin up/down, 0~1, 0:admin down, 1:admin up ",
};

static char *_cmd_usage_phy_admin_get = 
{
    "phy get <port ID>\
     \n    port ID: UNI port, 1",
};

static char *_cmd_usage_port_mode_set = 
{
    "mode set <port ID> <mode>\
     \n    port ID: UNI port, 1\
     \n  mode:\
     \n  1:     Auto(10/100/1000Mbps)\
     \n  2:     10Mbps(half)\
     \n  3:     10Mbps(full)\
     \n  4:     100Mbps(half)\
     \n  5:     100Mbps(full)\
     \n  6:     1000Mbps(full)",
};


static char *_cmd_usage_port_mode_get = 
{
    "mode get <port ID>\
    \n    port ID: UNI port, 1",
};


static char *_cmd_usage_fdb_aging_time_set = 
{
    "aging-time set <time>\
    \n    time: mac aging time, 0~300, uint is second ",
};
static char *_cmd_usage_fdb_aging_time_get = 
{
    "aging-time get",
};
static char *_cmd_usage_fdb_entry_show = 
{
    "entry show [port ID] [type]\
    \n    port ID: UNI port, 1\
    \n    type: fdb type, dynamic/static/all",
};
static char *_cmd_usage_fdb_mac_banding_add = 
{
    "mac-banding add <port ID> <mac addr>\
    \n    port ID: UNI port, 1 \
    \n    mac addr: mac address, xx:xx:xx:xx:xx:xx ",
};
static char *_cmd_usage_fdb_mac_banding_del = 
{
    "mac-banding del <port ID> <mac addr>\
    \n    port ID: UNI port, 1 \
    \n    mac addr: mac address, xx:xx:xx:xx:xx:xx ",
};
static char *_cmd_usage_fdb_mac_banding_get = 
{
    "mac-banding get [port ID]\
    \n    port ID: UNI port, 1 ",
};


static char *_cmd_usage_port_flow_control_set = 
{
    "flow-control set <port ID> <enable>\
     \n    port ID: UNI port, 1\
     \n  enable:  0~1, 0:disable, 1:enable",
};


static char *_cmd_usage_port_flow_control_get = 
{
    "flow-control get <port ID>\
    \n    port ID: UNI port, 1",
};
#endif


static char *_cmd_usage_port_storm_control_set = 
{
    "storm-control set <port ID> < storm-type> <enable> <rate>\
    \n  port ID:        ctc portid, 1\
    \n  storm-type:  0~3, 0:bc, 1:bc_umc, 3:disable \
    \n  enable:       0~1, 0:disable, 1:enable \
    \n    rate:       0~1000000, unit is kbps",
};

static char *_cmd_usage_port_storm_control_get = 
{
    "storm-control get <port ID>\
    \n    port ID: UNI port, 1",
};


#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
static char *_cmd_usage_port_isolation_set = 
{
    "isolation set  <enable>\
    \n  enable: 0~1, 0:disable,1:enable ",
};

static char *_cmd_usage_port_isolation_get = 
{
    "isolation get",
};
#endif

static char *_cmd_usage_port_mtu_set = 
{
    "mtu set <port ID> <len>\
    \n  port ID:        0xff, all port \
    \n    len:     1518~1900, mtu len",
};

static char *_cmd_usage_port_mtu_get = 
{
    "mtu get <port ID>\
     \n    port ID: UNI port, 1",
};

static char *_cmd_usage_port_config_get = 
{
    "show config <port ID>\
    \n    port ID: UNI port, 1",
};

static char *_cmd_usage_port_status_get = 
{
    "show status <port ID>\
    \n    port ID: UNI port, 1",
};

static char *_cmd_usage_qos_schedule_set = 
{
    "schedule set <mode>\
    \n  mode: 0~1, 0:SP, 1:WRR",
};

static char *_cmd_usage_qos_schedule_get = 
{
    "schedule get",
};

static char *_cmd_usage_queue_size_set = 
{
    "queue-size set <port ID> <queue ID> <size >\
    \n  port ID:        0~1, 1:UNI port, 0:PON port\
    \n  queue ID:     0~7, queue id\
    \n  size:             0~384 for UNI port, 0-230 for PON port",
};

static char *_cmd_usage_queue_size_get = 
{
    "queue-size get <port ID> <queue ID>\
    \n  port ID:        0~1, 0:PON port, 1:UNI port\
    \n  queue ID:     0~7, queue id",
};


static char *_cmd_usage_1pcos_set = 
{
    "1pcos set <pri> <cos>\
    \n  pri:        0~7, 8021p pri\
    \n  cos:       0~7, cos value",
};

static char *_cmd_usage_1pcos_get = 
{
    "1pcos get <pri>\
    \n  pri:        0~7, 8021p pri",
};



static char *_cmd_usage_rate_limit_set = 
{
    "rate-limit set <port ID> <enable> <rate>\
    \n    port ID: UNI port, 1\
    \n  enable:         0~1, 0:disable, 1:enable\
    \n  rate:             0~1000000, uint is kbps ",
};

static char *_cmd_usage_rate_limit_get = 
{
    "rate-limit get <port ID>\
     \n    port ID: UNI port, 1"

};


static char *_cmd_usage_shaper_set = 
{
    "shaper set <port ID> <enable> <rate>\
    \n  port ID: UNI port, 1\
    \n  enable:         0~1, 0:disable, 1:enable\
    \n  rate:             0~1000000, uint is kbps ",
};

static char *_cmd_usage_shaper_get = 
{
    "shaper get <port ID>\
     \n    port ID: UNI port, 1",
};

static char *_cmd_usage_qos_config_get = 
{
    "show config",
};


static char *_cmd_usage_fec_set = 
{
    "fec set <enable>\
    \n  enable: 0~1, 0:disable, 1:enable",
};

static char *_cmd_usage_fec_get = 
{
    "fec get",
};

static char *_cmd_usage_tx_power_set = 
{
    "tx-power set <enable>\
    \n  enable: 0~1, 0:disable, 1:enable ",
};

static char *_cmd_usage_tx_power_get = 
{
    "tx-power get ",
};

static char *_cmd_usage_rx_power_set = 
{
    "rx-power set <enable>\
    \n  enable: 0~1, 0:disable, 1:enable ",
};

static char *_cmd_usage_rx_power_get = 
{
    "rx-power get ",
};


static char *_cmd_usage_pon_show_config_get = 
{
    "show pon config",
};


static char *_cmd_usage_pon_show_status_get = 
{
    "show pon status",
};

static char *_cmd_usage_capture_syslog_set = 
{
    "remote-capture syslog <level> [module] \
    \n    level: log level, 0~3: debug 0~3, 4: info, 5: minor, 6: major, 7: critical \
    \n    module: system module ID, 0~39 ",
};

static char *_cmd_usage_capture_packet_set = 
{
    "remote-capture packet <enable> \
    \n    enable: 0~1, 0:disable, 1:enable ",
};

static char *_cmd_usage_capture_show = 
{
    "remote-capture show ",
};

static char *_cmd_usage_logserver_ip_set = 
{
    "logserver-ip set <ip addr> \
    \n    ip addr: log server IP address, xxx.xxx.xxx.xxx, example: 192.168.2.2 ",
};

static char *_cmd_usage_logserver_ip_get = 
{
    "logserver-ip get ",
};


// aging-time set <time>
#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
int cmd_config_igmp_mode_set(struct cli_def *cli, char *command, char *argv[], int argc)
{   
    mc_mode_t        mode=0;
    cs_status           ret = CS_E_OK;

    if(argc != 1) {
        cli_arg_help(cli, 0, _cmd_usage_igmp_mode_set, NULL);
        return CLI_ERROR;
    }
    
    mode = iros_strtol(argv[0]);    
    if ( ! ( (mode ==0) 
            || (mode==1)
            || (mode==2)
            ) ) {
        cli_print (cli, "input mode =%d error!", mode);
         return CLI_ERROR;
    }

    if (mode ==0) {
        mode= MC_DISABLE;
    }else if(mode == 1){
        mode= MC_MANUAL;
    }else{
        mode=MC_SNOOPING;
    }

    ret=ctc_mc_mode_set_adapt ( mode);

    return ret;
}

int cmd_config_igmp_mode_get (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status ret = CS_E_OK;
    mc_mode_t  mode;
    cs_uint8   * mode_str[]={"snooping", "mc_control", "proxy","disable" };    

    if(argc != 0) {
        cli_arg_help(cli, 0, _cmd_usage_igmp_mode_get, NULL);
        return CLI_ERROR;
    }

    ret=mc_mode_get ( ONU_DEVICEID_FOR_API,  &mode);   
    if(ret != CS_E_OK)
            return CLI_ERROR;
            
    cli_print(cli, "  %-20s: %s ", "igmp mode", mode_str[mode]);

    return CLI_OK;
}
#endif /* #ifndef HAVE_INACTIVE_REDUPLICATE_CMD */
int cmd_config_igmp_aging_time_set(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status ret = CS_E_OK;
    cs_uint32 time;

    if(argc < 1)
    {
        cli_arg_help(cli, 0, _cmd_usage_igmp_aging_time_set, NULL);
        return CLI_ERROR;
    }
    
    time = iros_strtol(argv[0]);    
    if (time > 260) {
        cli_print (cli, "input time value %d exceed the max 260!", time);
         return CLI_ERROR;
    }
    if (time < 1) {
        cli_print (cli, "input time value %d less than min 1!", time);
         return CLI_ERROR;
    }

    ret =ctc_mc_gmi_set_adapt( time);
  
    return ret;
}

int cmd_config_igmp_aging_time_get(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status ret = CS_E_OK; 
    cs_uint32 time=0;

    if(argc != 0) {
        cli_arg_help(cli, 0, _cmd_usage_igmp_aging_time_get , NULL);
        return CLI_ERROR;
    }
    ret =mc_gmi_get ( ONU_DEVICEID_FOR_API, & time);

    cli_print(cli, "  %-20s: %ds ", "igmp aging time",  time);

    return ret;
}


int cmd_config_mc_tagstrip_set(struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_port_id_t       port_id=0;
    cs_uint8            tagStriped=0;
    cs_uint8            num=0;
    oam_ctc_iptv_vid_t vlan[CTC_ONU_MAX_IPTV_VLAN];
    cs_uint32           i=0;
    cs_status           ret = CS_E_OK;

    if(argc <2 ) {
        cli_arg_help(cli, 0, _cmd_usage_mc_tagstrip_set , NULL);
        return CLI_ERROR;
    }
    
    port_id = iros_strtol(argv[0]);
    tagStriped=iros_strtol(argv[1]);
    num=argc-2;

    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error!", port_id);
        return CLI_ERROR;
    }

    if ( tagStriped >2 ) {
        cli_print ( cli, "mode %d error!", tagStriped);
        return CLI_ERROR;
    }

    if (num >  CTC_ONU_MAX_IPTV_VLAN/2) {
        cli_print ( cli, "vlan num %d error!", num);
        return CLI_ERROR;
    }

    if ( (num %2) !=0) {
        cli_print ( cli, "vlan num %d error!", num);
        return CLI_ERROR;
    }
    
    num/=2;

    for (  i=0; i < num;i++) {        
        vlan[i].vid= iros_strtol (argv[i*2+2]); 
        vlan[i].iptv_vid= iros_strtol (argv[i*2+1+2]); 
    }
    
    ret=ctc_oam_onu_mctagstrip_set_adapt ( port_id, tagStriped,  num, vlan);

    return ret;
}


int cmd_config_mc_tagstrip_get (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t   context;
    cs_port_id_t                 port_id=0;
    cs_sdl_mc_vlan_act_t    vlan_act=0;
    cs_sdl_vlan_cfg_t          vlan[CTC_ONU_MAX_IPTV_VLAN];
    cs_uint16                     num=0;
    cs_status                     ret=0;
    cs_uint32                    i=0;
    cs_uint8                  *  tagstrip_str[]={"no_tagstrip" , "tagstrip", "tagswap"};    

    if(argc != 1) {
        cli_arg_help(cli, 0, _cmd_usage_mc_tagstrip_get , NULL);
        return CLI_ERROR;
    }

     port_id = iros_strtol(argv[0]);
    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error!", port_id);
        return CLI_ERROR;
    }

    ret=epon_request_onu_mc_vlan_action_get ( context,
        ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API,   port_id ,  &vlan_act, vlan , &num);
    
    cli_print(cli, "  port(%d) mc tagstrip set: ", port_id);
    cli_print(cli, "  %-20s: %s ", "tagstrip mode", tagstrip_str[vlan_act]);
    if ( vlan_act== SDL_MC_VLAN_TAG_SWAP ) {
        cli_print(cli, "swap pair (svlan, iptv vlan)" );
        for ( i=0; i < num ; i++ ) {
            cli_print(cli, "          (%d, %d) ",  vlan[i].s_vlan.vid , vlan[i].c_vlan.vid);
        }
    }

    return ret;
    
}


int cmd_config_mc_max_grp_set (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_port_id_t       port_id=0;
    cs_uint32            max_grp=0;
    cs_status           ret = CS_E_OK;

    if(argc !=2 ) {
        cli_arg_help(cli, 0, _cmd_usage_mc_max_grp_set , NULL);
        return CLI_ERROR;
    }
    
    port_id = iros_strtol(argv[0]);
    max_grp=iros_strtol(argv[1]);

    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error!", port_id);
        return CLI_ERROR;
    }
    
    if (max_grp > mc_device_port_grp_cap(0)) {
        cli_print ( cli, "input max grp  %d exceed the max %d group! ", max_grp,mc_device_port_grp_cap(0));
        return CLI_ERROR;
    }

    ret=ctc_oam_onu_grp_nmb_max_set_adapt ( port_id,  (cs_uint32 )max_grp);

    return ret;
}


int cmd_config_mc_max_grp_get (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_port_id_t                 port_id=0;
    cs_status                     ret=0;
    cs_uint16                    grp_num=0;
    cs_uint8                      buffer[32];

    if(argc != 1) {
        cli_arg_help(cli, 0, _cmd_usage_mc_max_grp_get , NULL);
        return CLI_ERROR;
    }

    port_id = iros_strtol(argv[0]);
    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error!", port_id);
        return CLI_ERROR;
    }
    ret=mc_port_max_grp_get ( ONU_DEVICEID_FOR_API,  port_id,&grp_num);

    sprintf (buffer, "port(%d) mc max grp", port_id);
    
    cli_print(cli, " %-20s: %d ", buffer, grp_num);

    return ret;
    
}


int cmd_config_mc_fast_leave_set (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_uint32          fast_leave=0;
    cs_status          ret = CS_E_OK;

    if(argc !=1  ) {
        cli_arg_help(cli, 0, _cmd_usage_mc_fast_leave_set , NULL);
        return CLI_ERROR;
    }
    
    fast_leave = iros_strtol(argv[0]);
    if ( !( (fast_leave ==0)
         || (fast_leave ==1) )) {
         cli_print (cli, "fast leave %d error!", fast_leave);
         return CLI_ERROR;
    }
    
    if ( fast_leave==0 ) {
        fast_leave=DB_NON_FAST_LEAVE;
    }else {
        fast_leave=DB_FAST_LEAVE;
    }

    ret=ctc_oam_fast_leave_ctrl_set_adapt ( fast_leave);

    return ret;
    
}



int cmd_config_mc_fast_leave_get (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status                   ret=0;
    cs_uint32                   fast_leave=0;

    if(argc != 0) {
        cli_arg_help(cli, 0, _cmd_usage_mc_fast_leave_get , NULL);
        return CLI_ERROR;
    }

    fast_leave=mc_get_fast_leave_state();
    cli_print(cli, "  %-20s: %s ", " leave mode",  fast_leave==DB_NON_FAST_LEAVE? "non fast leave mode": "fast leave mode");
   
    return ret;
    
}


int cmd_config_mc_config_get (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t         context;
    mc_mode_t                       mode=0;
    cs_uint32                           gmi=0;
    cs_uint8                           * mode_str[]={"snooping", "mc_control", "proxy","disable" }; 
    cs_uint8                           * tagstrip_str[]={"no_tagstrip" , "tagstrip", "tagswap"};   
    cs_uint32                           fast_leave=0;
    cs_uint32                           port_id=0;
    cs_sdl_mc_vlan_act_t            vlan_act=0;
    cs_sdl_vlan_cfg_t               vlan[CTC_ONU_MAX_IPTV_VLAN];
    cs_uint8                             buffer[64];
    cs_uint16                           num=0;
    cs_uint16                           max_grp_num=0;
    cs_uint32                           i=0;
    cs_status                           ret= 0;

    if(argc != 0) {
        cli_arg_help(cli, 0, _cmd_usage_mc_config_get  , NULL);
        return CLI_ERROR;
    }


    mc_mode_get ( ONU_DEVICEID_FOR_API,   &mode);
    cli_print(cli, "  %-20s: %s ", "igmp mode", mode_str[mode]);
    
    ret =mc_gmi_get ( ONU_DEVICEID_FOR_API, &gmi);
    cli_print(cli, "  %-20s: %ds ","igmp aging time",  gmi);

    fast_leave=mc_get_fast_leave_state ( );
    cli_print(cli, "  %-20s: %s ",  "leave mode", fast_leave==DB_NON_FAST_LEAVE? "non fast leave": "fast leave");
    
    for ( port_id=0; port_id < DB_MAX_PORT ; port_id++  ) {
        ret=epon_request_onu_mc_vlan_action_get ( context,
            ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API,   port_id+1  ,  &vlan_act, vlan , &num);
        sprintf(buffer, "  port %d config", port_id);
        cli_print(cli, "%s: ", buffer);
        cli_print(cli, "  %-18s %s ", "mc tagstrip mode:", tagstrip_str[vlan_act]);
        if ( vlan_act== SDL_MC_VLAN_TAG_SWAP ) {
            cli_print(cli, "  %-18s" , "swap pair (svlan, iptv vlan):");
            for ( i=0; i < num ; i++ ) {
                cli_print(cli, "          (%d, %d) ",  vlan[i].s_vlan.vid , vlan[i].c_vlan.vid);
            }
        }
        mc_port_max_grp_get ( ONU_DEVICEID_FOR_API,  port_id+1,&max_grp_num);
  
        cli_print(cli, "  %-18s: %d ", "mc max grp", max_grp_num);
    }
    
    return ret;
    
}

int cmd_config_mc_port_grp_get (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status                   ret=0;
    cs_uint32                   port_id=0;
    mc_ipv4_grp_t        * v4_tbl=NULL;
    cs_uint32                  v4_tbl_len=0;
    mc_ipv6_grp_t        * v6_tbl=NULL;
    cs_uint32                  v6_tbl_len=0;
    cs_uint32                   index=0;
    cs_uint32                   out_group=0;
    cs_uint32                   port_map=0;
    cs_uint8                  *  buffer=NULL;

    if(argc != 1) {
        cli_arg_help(cli, 0, _cmd_usage_mc_grp_get , NULL);
        return CLI_ERROR;
    }
    
    port_id = iros_strtol(argv[0]);
    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error", port_id);
        return CLI_ERROR;
    }
   
    buffer =(cs_uint8  *) iros_malloc ( IROS_MID_CLI, 1280 ) ;
    if (buffer ==NULL ) {
        goto END;
    }

    v4_tbl =(mc_ipv4_grp_t *) iros_malloc ( IROS_MID_CLI, sizeof (mc_ipv4_grp_t )* 256 ) ;
    if (v4_tbl ==NULL ) {
        goto END;
    }
    
    v6_tbl =(mc_ipv6_grp_t *) iros_malloc ( IROS_MID_CLI, sizeof (mc_ipv6_grp_t )* 256 ) ;
    if (v6_tbl ==NULL ) {
        goto END;
    }


    mc_get_grp_tbl ( ONU_DEVICEID_FOR_API,  v4_tbl, & v4_tbl_len,  v6_tbl, &v6_tbl_len);

    for(index = 0; index < v4_tbl_len; index++) {
        out_group = v4_tbl[index].grp;
        port_map = v4_tbl[index].port_map;

        if ( (port_map & (0x1 << (port_id-1)) ) ==0 ) {
            continue;
        }    
        
        sprintf(buffer, "%d.%d.%d.%d "
        , ((out_group&0xff000000) >>24), ((out_group&0xff0000) >>16), (( out_group&0xff00) >>8),  (out_group &0xff) );

        cli_print(cli, "%-30s", buffer);
    }

    for(index = 0; index < v6_tbl_len; index++) {
        port_map = v6_tbl[index].port_map;

        if ( (port_map & (0x1 << (port_id-1)) ) ==0 ) {
            continue;
        }    
        
        sprintf(buffer, "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x ", 
                                             v6_tbl[index].grp[0], v6_tbl[index].grp[1],
                                            v6_tbl[index].grp[2], v6_tbl[index].grp[3],
                                            v6_tbl[index].grp[4], v6_tbl[index].grp[5],
                                            v6_tbl[index].grp[6], v6_tbl[index].grp[7]);

        cli_print(cli, "%-30s", buffer);
    }

END:

    if ( buffer !=NULL  ) {
        iros_free ( buffer );
        v4_tbl=NULL;
    }
    
    if ( v4_tbl !=NULL  ) {
        iros_free ( v4_tbl );
        v4_tbl=NULL;
    }

    if ( v6_tbl  !=NULL  ) {
        iros_free ( v6_tbl );
        v6_tbl=NULL;
    }
    
    
    return ret;
    
}

#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
int cmd_config_phy_admin_set(struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_port_id_t       port_id=0;
    cs_uint32           phy_admin=0;
    cs_status           ret = CS_E_OK;

    if(argc != 2){
        cli_arg_help(cli, 0, _cmd_usage_phy_admin_set, NULL);
        return CLI_ERROR;
    }
    
    port_id = iros_strtol(argv[0]);    
    phy_admin= iros_strtol(argv[1]);

    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error!", port_id);
        return CLI_ERROR;
    }
    
    if ( ! ((phy_admin ==SDL_PORT_ADMIN_DOWN)
            || (phy_admin==SDL_PORT_ADMIN_UP)) ) {
        cli_print(cli, "inpu admin %d error!", phy_admin);
        return CLI_ERROR;
    }

    if ( phy_admin ==SDL_PORT_ADMIN_DOWN) {
        phy_admin=OAM_INTF_PHY_ADMIN_DIS;
    } else {
       phy_admin=OAM_INTF_PHY_ADMIN_EN;
    }
    

    ret=ctc_oam_eth_admin_state_set_adapt ( port_id,  phy_admin);
    

    return ret;
}

int cmd_config_phy_admin_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t         context;
    cs_port_id_t                       port_id=0;
    cs_sdl_port_admin_t           admin=0;
    cs_uint8                            buffer[64];
    cs_status                           ret = CS_E_OK;

    if(argc != 1)
    {
        cli_arg_help(cli, 0, _cmd_usage_phy_admin_get  , NULL);
        return CLI_ERROR;
    }

    port_id = iros_strtol(argv[0]);
    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error!", port_id);
        return CLI_ERROR;
    }
    

    epon_request_onu_port_admin_get ( context,  ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API ,  port_id, &admin);
    sprintf (buffer, "port(%d)  ", port_id);
    
    cli_print(cli, "  %-20s: %s ", buffer, admin==SDL_PORT_ADMIN_DOWN? "admin down":"admin up");

    return ret;
    
}



int cmd_config_port_mode_set(struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_port_id_t       port_id=0;
    cs_uint8            mode=0;
    cs_status           ret = CS_E_OK;

    if(argc != 2)
    {
        cli_arg_help(cli, 0, _cmd_usage_port_mode_set, NULL);
        return CLI_ERROR;
    }
    
    port_id = iros_strtol(argv[0]);    
    mode= iros_strtol(argv[1]);

    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error!", port_id);
        return CLI_ERROR;
    }

    if ( ! ( (mode==CTC_OAM_PORT_MODE_AUTO)
        || (mode==CTC_OAM_PORT_MODE_10M_HALF)
        || (mode==CTC_OAM_PORT_MODE_10M_FULL)
        || (mode==CTC_OAM_PORT_MODE_100M_HALF)
        ||( mode==CTC_OAM_PORT_MODE_100M_FULL)
        || ( mode ==CTC_OAM_PORT_MODE_1000M_FULL)) ) {
        cli_print ( cli, "input mode %d error!", mode);
        return CLI_ERROR;
    }
    ret=ctc_oam_onu_port_mode_set_adapt(port_id,  mode);
    return ret;
}


int cmd_config_port_mode_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t         context;
    cs_port_id_t                       port_id=0;
    cs_uint32                          speed_cfg=0;
    cs_uint8                            buffer[64];
    cs_status                           ret = CS_E_OK;

    if(argc != 1) {
        cli_arg_help(cli, 0, _cmd_usage_port_mode_get  , NULL);
        return CLI_ERROR;
    }

    port_id = iros_strtol(argv[0]);

    ret=epon_request_onu_port_status_get ( context,  ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,  port_id,(cs_sdl_port_speed_cfg_t *) &speed_cfg );
    switch (speed_cfg) {
        case SDL_PORT_AUTO_10_100_1000:
        case SDL_PORT_AUTO_10_100:
            speed_cfg = CTC_OAM_PORT_MODE_AUTO;
            break;
        case SDL_PORT_10_HALF:
            speed_cfg = CTC_OAM_PORT_MODE_10M_HALF;
            break;
        case SDL_PORT_10_FULL:
            speed_cfg= CTC_OAM_PORT_MODE_10M_FULL;
            break;
        case SDL_PORT_100_HALF:
            speed_cfg= CTC_OAM_PORT_MODE_100M_HALF;
            break;
        case SDL_PORT_100_FULL:
            speed_cfg = CTC_OAM_PORT_MODE_100M_FULL;
            break;
        case SDL_PORT_1000_FULL:
            speed_cfg = CTC_OAM_PORT_MODE_1000M_FULL;
            break;
        default:
            break;
    } 

    
    sprintf (buffer,  "port(%d)", port_id );
    cli_print(cli, "  %-20s: %s", buffer, mode2string[speed_cfg]);

    return ret;
    
}


int cmd_config_port_flow_control_set(struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_port_id_t      port_id=0;
    cs_uint8            flow_control=0;
    cs_status          ret = CS_E_OK;

    if(argc != 2) {
        cli_arg_help(cli, 0, _cmd_usage_port_flow_control_set, NULL);
        return CLI_ERROR;
    }
    
    port_id = iros_strtol(argv[0]);    
    flow_control= iros_strtol(argv[1]);

    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error!", port_id);
        return CLI_ERROR;
    }

    if ( !((flow_control ==0 )
        ||( flow_control==1)) ) {
        cli_print ( cli, "input flow control %d error!", flow_control);
        return CLI_ERROR;
    }
    
    ret=ctc_oam_eth_port_pause_set_adapt ( port_id,  flow_control);

    return ret;
}

int cmd_config_port_flow_control_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t         context;
    cs_port_id_t                       port_id=0;
    cs_boolean                        enable=0;
    cs_uint8                            buffer[64];
    cs_status                           ret = CS_E_OK;

    if(argc != 1)
    {
        cli_arg_help(cli, 0, _cmd_usage_port_flow_control_get  , NULL);
        return CLI_ERROR;
    }

    port_id = iros_strtol(argv[0]);

    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error!", port_id);
        return CLI_ERROR;
    }
    
    epon_request_onu_port_flow_ctrl_get ( context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API , port_id , &enable);
    sprintf (buffer, "port(%d) flow control", port_id);

    cli_print(cli, "  %-20s: %s", buffer, enable==0? "disable":"enable" );

    return ret;
    
}
#endif /* #ifndef HAVE_INACTIVE_REDUPLICATE_CMD */


int cmd_config_port_storm_control_set(struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_port_id_t                    port_id=0;
    cs_sdl_storm_ctrl_e         storm_type=0;
    cs_boolean                     enable=0;
    cs_uint32                        rate=0;
    cs_status                        ret = CS_E_OK;

    if(argc != 4)
    {
        cli_arg_help(cli, 0, _cmd_usage_port_storm_control_set, NULL);
        return CLI_ERROR;
    }
    
    port_id = iros_strtol(argv[0]);    
    storm_type= iros_strtol(argv[1]);
    enable= iros_strtol(argv[2]);
    rate=iros_strtol(argv[3]);

    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error!", port_id);
        return CLI_ERROR;
    }

    if ( !((storm_type ==SDL_STORM_TYPE_BC )
         || (storm_type ==SDL_STORM_TYPE_BC_UMC)
        /*|| ( storm_type ==SDL_STORM_TYPE_BC_UMC_UUC) */
         || (storm_type ==SDL_STORM_TYPE_DISABLE)) ) {
        cli_print ( cli, "input storm type  %d error!", storm_type);
        return CLI_ERROR;
    }

    if ( ! ((enable ==0 )
            || (enable ==1)) ) {
        cli_print ( cli, "input enable  %d error!", enable);
        return CLI_ERROR;
    }

    if ( rate > DB_MAX_RATE ) {
        cli_print ( cli, "input rate  %d error!", rate);
        return CLI_ERROR;
    }

    ret=ctc_port_storm_control_set_adapt ( port_id,  storm_type,  enable,  rate);    

    return ret;
}


int cmd_config_port_storm_control_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t        context;
    cs_port_id_t                      port_id=0;
    cs_sdl_rl_mod_e               storm_mod=0;
    cs_sdl_storm_ctrl_e           storm_type=0;
    cs_sdl_policy_t                  storm_rate;
    cs_status                          ret = CS_E_OK;

    if(argc != 1)
    {
        cli_arg_help(cli, 0, _cmd_usage_port_storm_control_get  , NULL);
        return CLI_ERROR;
    }
    memset ( &storm_rate, 0, sizeof(cs_sdl_policy_t ) );

    port_id = iros_strtol(argv[0]);
    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error!", port_id);
        return CLI_ERROR;
    }
    

    ret=epon_request_onu_port_storm_ctrl_get ( context, 
        ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API ,  port_id, &storm_mod, &storm_type, &storm_rate);


     cli_print(cli,"  %-20s: %s\n", "storm type", storm2string[storm_type]);
     cli_print(cli,"  %-20s: %d kbps\n", "storm rate", storm_rate.rate);
     cli_print(cli,"  %-20s: %d\n","storm burst",  storm_rate.cbs); 


    return ret;
    
}


#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
int cmd_config_port_isolation_set(struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_boolean                     enable=0;
    cs_status                        ret = CS_E_OK;

    if (DB_MAX_PORT ==1) {
        cli_print(cli, "\r\n no need set isolation in 1port ONU!\r\n");
        return CLI_OK;
    }

    if(argc != 1)
    {
        cli_arg_help(cli, 0, _cmd_usage_port_isolation_set , NULL);
        return CLI_ERROR;
    }

    enable= iros_strtol(argv[0]);
    {
        cs_callback_context_t cxt;
        ret = epon_request_onu_port_isolation_set(cxt,
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, enable);
    }


    
    return ret;
}



int cmd_config_port_isolation_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t        context;
    cs_boolean                       enable=0;
    cs_status                          ret = CS_E_OK;

    if(argc != 0)
    {
        cli_arg_help(cli, 0, _cmd_usage_port_isolation_get  , NULL);
        return CLI_ERROR;
    }
 
    ret=epon_request_onu_port_isolation_get (context, ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API , &enable);
    
    cli_print(cli,"\r\n %-20s: %s", "isolation", enable==1? "enable":"disable");

    return ret;
    
}
#endif

int cmd_config_port_mtu_set(struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_port_id_t                   port_id=0;
    cs_uint16                       mtu=0;
    cs_status                       ret = CS_E_OK;
    
    if(argc != 2) {
        cli_arg_help(cli, 0, _cmd_usage_port_mtu_set , NULL);
        return CLI_ERROR;
    }

    port_id= iros_strtol(argv[0]);
    mtu=iros_strtol(argv[1]);
    if ( port_id != 0xff ) {
        cli_print ( cli, "input port %d error!", port_id);
        return CLI_ERROR;
    }

    if ((mtu>DB_MAX_MTU_LEN)||(mtu<DB_MIN_MTU_LEN) ) {
        cli_print ( cli, "input mtu %d error!", mtu);
        return CLI_ERROR;
    }

    ret=ctc_onu_mtu_set_adapt ( mtu);
    
    return ret;
}


int cmd_config_port_mtu_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t        context;
    cs_port_id_t                      port_id=0;
    cs_uint16                          mtu=0;
    cs_uint8                            buffer[64];
    cs_status                          ret = CS_E_OK;

    if(argc != 1)
    {
        cli_arg_help(cli, 0, _cmd_usage_port_mtu_get  , NULL);
        return CLI_ERROR;
    }

    port_id= iros_strtol(argv[0]);
    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error!", port_id);
        return CLI_ERROR;
    }
    
    ret=epon_request_onu_port_mtu_get ( context, ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API ,  port_id, &mtu);
    sprintf (buffer, "port(%d) mtu", port_id);
    
    cli_print(cli,"  %-20s: %d", buffer, mtu);

    return ret;
    
}


int cmd_config_ip_mtu_get(struct cli_def *cli, char *command, char *argv[], int argc)
{

    cs_uint32  mtu =  0;
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    epon_request_onu_ip_mtu_get(&mtu);

    cli_print(cli,"IP Mtu: %d",mtu);
    return CS_E_OK;
}

int cmd_config_ip_mtu_set(struct cli_def *cli, char *command, char *argv[], int argc)
{

    cs_uint32  mtu =  0;
    
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,"<1440-1500>", "ip management mtu",NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }
    if(1 == argc)
    {
        mtu= iros_strtol(argv[0]);
        if ((mtu > 1500)&&(mtu < 1440)) {
            cli_print(cli, "%% Invalid input.");
            return CLI_ERROR;
        }

        epon_request_onu_ip_mtu_set(mtu);
        cli_print(cli,"IP Mtu set to: %d",mtu);
    } else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CS_E_OK;
}



int cmd_config_port_config_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t       context;
    cs_port_id_t                      port_id=0;
    cs_sdl_port_admin_t          admin=0;
    cs_sdl_port_speed_cfg_t    speed_cfg;
    cs_boolean                        enable=0;
    cs_sdl_rl_mod_e               storm_mod=0;
    cs_sdl_storm_ctrl_e           storm_type=0;
    cs_sdl_policy_t                  storm_rate;
    cs_uint16                          mtu=0;
    cs_status                          ret = CS_E_OK;

    if(argc != 1) {
        cli_arg_help(cli, 0, _cmd_usage_port_config_get  , NULL);
        return CLI_ERROR;
    }

    port_id= iros_strtol(argv[0]);
    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error", port_id);
        return CLI_ERROR;
    }
    

    epon_request_onu_port_admin_get ( context,  ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API ,  port_id, &admin);
    
    cli_print(cli, " %-20s: %s ", "port admin", admin==SDL_PORT_ADMIN_DOWN? "down":"up");

    ret=epon_request_onu_port_status_get ( context,  ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,  port_id, &speed_cfg );
    switch (speed_cfg) {
    case SDL_PORT_AUTO_10_100_1000:
    case SDL_PORT_AUTO_10_100:
        speed_cfg = CTC_OAM_PORT_MODE_AUTO;
        break;
    case SDL_PORT_10_HALF:
        speed_cfg = CTC_OAM_PORT_MODE_10M_HALF;
        break;
    case SDL_PORT_10_FULL:
        speed_cfg= CTC_OAM_PORT_MODE_10M_FULL;
        break;
    case SDL_PORT_100_HALF:
        speed_cfg= CTC_OAM_PORT_MODE_100M_HALF;
        break;
    case SDL_PORT_100_FULL:
        speed_cfg = CTC_OAM_PORT_MODE_100M_FULL;
        break;
    case SDL_PORT_1000_FULL:
        speed_cfg = CTC_OAM_PORT_MODE_1000M_FULL;
        break;
    default:
        break;
} 

    cli_print(cli, " %-20s: %s", "port mode", mode2string[speed_cfg]);

    epon_request_onu_port_flow_ctrl_get ( context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API , port_id , &enable);

    cli_print(cli, " %-20s: %s", "port flow control" , enable==0? "disable":"enable" );
    

    ret=epon_request_onu_port_storm_ctrl_get ( context, 
        ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API ,  port_id, &storm_mod, &storm_type, &storm_rate);

     cli_print(cli," %-20s: %s", "storm type", storm2string[storm_type]);
     cli_print(cli," %-20s: %d kbps", "storm rate", storm_rate.rate);
     cli_print(cli," %-20s: %d","storm burst" ,storm_rate.cbs); 


    ret=epon_request_onu_port_isolation_get (context, ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API , &enable);
    
    cli_print(cli,"\r\n %-20s: %s", "isolation",  enable==1? "enable":"disable");
    
    ret=epon_request_onu_port_mtu_get ( context, ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API ,  port_id, &mtu);
    
    cli_print(cli," %-20s: %d",   "port MTU ", mtu);
    

    return ret;
    
}


int cmd_config_port_status_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_port_id_t                      port_id=0;
    cs_callback_context_t           context;
    cs_status               status = 0;
    cs_sdl_port_link_status_t    link_status;
    cs_sdl_port_ether_speed_t    speed;
    cs_sdl_port_ether_duplex_t   duplex;
    cs_sdl_port_autoneg_status_t auto_neg;
    
    cs_status                          ret = CS_E_OK;

    if(argc != 1)
    {
        cli_arg_help(cli, 0, _cmd_usage_port_status_get  , NULL);
        return CLI_ERROR;
    }

    port_id= iros_strtol(argv[0]);
    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error", port_id);
        return CLI_ERROR;
    }

    status = epon_request_onu_port_link_status_get(context, 0, 0, port_id, &link_status);
    if(status != CS_E_OK){
        cli_print(cli, "epon_request_onu_port_link_status_get failed \n");    
         return CLI_ERROR;     
    }

    status = epon_request_onu_port_speed_get(context, 0, 0, port_id, &speed);
    if(status != CS_E_OK){
        cli_print(cli, "epon_request_onu_port_speed_get failed \n");    
         return CLI_ERROR;      
    }

    status = epon_request_onu_port_duplex_get(context, 0, 0, port_id, &duplex);
    if(status != CS_E_OK){
        cli_print(cli, "epon_request_onu_port_duplex_get failed \n");    
         return CLI_ERROR;    
    }

    status = epon_request_onu_port_auto_neg_get(context, 0, 0, port_id, &auto_neg);
    if(status != CS_E_OK){
        cli_print(cli,  "epon_request_onu_port_auto_neg_get failed \n");    
        return CLI_ERROR;    
    }

    cli_print(cli, "----------------------------------\n");
    cli_print(cli, " %-20s: %s\n", "Link status", (link_status == SDL_PORT_ADMIN_UP) ? "up" : "down");
    cli_print(cli, " %-20s: %s\n", "Actual speed", actualspeed2string[speed]);
    cli_print(cli, " %-20s: %s\n", "Actual duplex", actualduplex2string[duplex]);
    cli_print(cli, " %-20s:%s\n", "Auto neg", autoneg2string[auto_neg]);
    cli_print(cli, "----------------------------------\n");
    


    return ret;
    
}

int cmd_config_onu_schedule_set (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_sdl_sched_type_t       sche_mode=0;
    cs_status                       ret = CS_E_OK;
    
    if(argc != 1)
    {
        cli_arg_help(cli, 0, _cmd_usage_qos_schedule_set , NULL);
        return CLI_ERROR;
    }
    
    sche_mode= iros_strtol(argv[0]);
    if ( !((sche_mode==SDL_SCHD_SP_MODE )
          ||(sche_mode==SDL_SCHD_WRR_MODE) )  ) {
        cli_print(cli, "input mode value should be 0:sp mode or 1:wrr mode");
        return CLI_ERROR;
    }

    ret=ctc_onu_schedule_mode_set_adapt ( sche_mode);
   
    return ret;
    
}


int cmd_config_onu_schedule_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t        context;
    cs_sdl_sched_type_t         sche_mode=0;
    cs_status                          ret = CS_E_OK;

    if(argc != 0)
    {
        cli_arg_help(cli, 0, _cmd_usage_qos_schedule_get  , NULL);
        return CLI_ERROR;
    }
    
    ret=epon_request_onu_schedule_mode_get ( context, ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API, &sche_mode);
    cli_print(cli," %-20s: %s",  "ONU schedule", sche_mode==SDL_SCHD_SP_MODE? "SP_MODE":"WRR_MODE");

    return ret;
}


int cmd_config_queue_size_set (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_port_id_t                   port_id=0;
    cs_uint32                       queue_id=0;
    cs_uint32                       queue_size=0;
    cs_status                       ret = CS_E_OK;
    
    if(argc !=3) {
        cli_arg_help(cli, 0, _cmd_usage_queue_size_set , NULL);
        return CLI_ERROR;
    }

    port_id = iros_strtol(argv[0]);
    queue_id = iros_strtol(argv[1]);
    queue_size = iros_strtol(argv[2]);
    

    if ( ! ((port_id ==CS_UNI_PORT_ID1)
         || (port_id==CS_PON_PORT_ID)) ) {
         cli_print(cli," input portid=%d error", port_id  );
         return CLI_ERROR;
    }

    if ( queue_id >=DB_MAX_VOQ_SIZE) {
         cli_print(cli," input queue id=%d error", queue_id  );
         return CLI_ERROR;
    }

    if (port_id==CS_PON_PORT_ID ) {
        if ( queue_size > DS_MAX_QUE_BUFFER_SIZE ) {
             cli_print(cli," input queue_size =%d error", queue_size  );
             return CLI_ERROR;
        }
    }

    if (port_id==CS_UNI_PORT_ID1 ) {
        if ( queue_size > US_MAX_QUE_BUFFER_SIZE ) {
             cli_print(cli," input queue_size =%d error", queue_size  );
             return CLI_ERROR;
        }
    }

    if (port_id==CS_PON_PORT_ID ) {
        if ( queue_size > DS_MAX_QUE_BUFFER_SIZE ) {
             cli_print(cli," input queue_size =%d error", queue_size  );
             return CLI_ERROR;
        }
    }
    
    
    ret=ctc_port_queue_size_set_adapt ( port_id, queue_id,  queue_size);
   
    return ret;
    
}

int cmd_config_queue_size_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t     context;
    cs_port_id_t                   port_id=0;
    cs_uint32                       queue_id=0;
    cs_uint16                       queue_size=0;
    cs_uint8                         buffer[64];
    cs_status                       ret = CS_E_OK;

    if(argc != 2) {
        cli_arg_help(cli, 0, _cmd_usage_queue_size_get  , NULL);
        return CLI_ERROR;
    }

    port_id = iros_strtol(argv[0]);
    queue_id = iros_strtol(argv[1]);

    if ( ! ((port_id ==CS_UNI_PORT_ID1)
         || (port_id==CS_PON_PORT_ID)) ) {
         cli_print(cli," input portid=%d error", port_id  );
         return CLI_ERROR;
    }

    if ( queue_id >=DB_MAX_VOQ_SIZE) {
         cli_print(cli," input queue id=%d error", queue_id  );
         return CLI_ERROR;
    }
    
    
    ret=epon_request_onu_que_size_get ( context, 
        ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API , port_id,  queue_id,  &queue_size);
    sprintf (buffer, "queue %d size", queue_id );
    cli_print(cli," %-20s: %d",  buffer, queue_size );

    return ret;
    
}

int cmd_config_1pcos_set (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_uint32                       pri=0;
    cs_uint32                       cos=0;
    cs_status                       ret = CS_E_OK;
    
    if(argc !=2) {
        cli_arg_help(cli, 0, _cmd_usage_1pcos_set , NULL);
        return CLI_ERROR;
    }
    
    pri = iros_strtol(argv[0]);
    cos = iros_strtol(argv[1]);

    if (pri > 7 ){
        cli_print(cli," input pri=%d error", pri  );
        return CLI_ERROR;
    }

    if (cos > 7 ) {
        cli_print(cli," input cos=%d error", cos  );
        return CLI_ERROR;
    }
    

    ret=ctc_onu_1pcos_set_adapt ( pri,  cos);

   
    return ret;
    
}


int cmd_config_1pcos_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t     context;
    cs_uint32                       pri=0;
    cs_pri_cos_map_t          queue_map;
    cs_uint8                        buffer[64];
    cs_status                       ret = CS_E_OK;

    if(argc != 1) {
        cli_arg_help(cli, 0, _cmd_usage_1pcos_get  , NULL);
        return CLI_ERROR;
    }

    pri = iros_strtol(argv[0]);
    if (pri > 7 ) {
        cli_print(cli," input pri=%d error", pri  );
        return CLI_ERROR;
    }
    
    ret=epon_request_onu_dot1p_map_get ( context, 
        ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API , &queue_map);

    sprintf ( buffer, "8021p pri %d map cos", pri);
    

    cli_print(cli," %-20s: %d",  buffer, queue_map.cos[pri] );

    return ret;
    
}


int cmd_config_rate_limit_set  (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_port_id_t                   port_id=0;
    cs_uint8                         oper=0;
    cs_uint32                       cir=0;
    cs_uint32                       cbs=0;
    cs_uint32                       ebs=0;
    cs_status                       ret = CS_E_OK;
    
    if(argc !=3) {
        cli_arg_help(cli, 0, _cmd_usage_rate_limit_set , NULL);
        return CLI_ERROR;
    }

    port_id = iros_strtol(argv[0]);
    oper = iros_strtol(argv[1]);
    cir = iros_strtol(argv[2]);

    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error", port_id);
        return CLI_ERROR;
    }

    if (! ((oper == CTC_PORT_POLICY_OPER_ENBALE)
         || (oper== CTC_PORT_POLICY_OPER_DISABLE) )) {
        cli_print ( cli, "input enable %d error", oper);
        return CLI_ERROR;
    }

   if (cir > DB_MAX_RATE) {
        cli_print ( cli, "input rate %d error", cir);
        return CLI_ERROR;
    } 

    if (port_id ==CTC_PORT_POLICY_OPER_ENBALE ) {
        cbs = 10000;
    }
   
    ret=ctc_oam_eth_port_policing_set_adapt ( port_id,  oper,  cir,  cbs, ebs);
   
    return ret;
    
}



int cmd_config_rate_limit_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t     context;
    cs_port_id_t                   port_id=0;
    cs_sdl_policy_t               policy;
    cs_uint8                        buffer[64];
    cs_status                       ret = CS_E_OK;

    if(argc != 1) {
        cli_arg_help(cli, 0, _cmd_usage_rate_limit_get  , NULL);
        return CLI_ERROR;
    }

    port_id = iros_strtol(argv[0]);
    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error", port_id);
        return CLI_ERROR;
    }
    
    memset (&policy, 0, sizeof (cs_sdl_policy_t ) );

    ret=epon_request_onu_port_policy_get ( context, 
        ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API ,  port_id, &policy);
    sprintf ( buffer, "port(%d) up rate limit", port_id);
    if (policy.enable==1) {
       
        cli_print (cli, " %-20s: %d", buffer, policy.rate);
    }else {
        cli_print(cli," %-20s: %s", buffer, "disable" );
    }
    

    return ret;
    
}


int cmd_config_shaper_set  (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_port_id_t                   port_id=0;
    cs_uint8                         oper=0;
    cs_uint32                       cir=0;
    cs_status                       ret = CS_E_OK;
    
    if(argc !=3) {
        cli_arg_help(cli, 0, _cmd_usage_shaper_set , NULL);
        return CLI_ERROR;
    }

    port_id = iros_strtol(argv[0]);
    oper = iros_strtol(argv[1]);
    cir = iros_strtol(argv[2]);

    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error", port_id);
        return CLI_ERROR;
    }
    
    if (! ((oper == CTC_PORT_POLICY_OPER_ENBALE)
         || (oper== CTC_PORT_POLICY_OPER_DISABLE) )) {
        cli_print (cli,  "input enable %d error", oper);
        return CLI_ERROR;
    }

   if (cir > DB_MAX_RATE) {
        cli_print (cli,  "input rate %d error", cir);
        return CLI_ERROR;
    } 
    

    ret=ctc_oam_eth_ds_rate_limit_set_adapt  ( port_id, oper, cir,  CLI_MAX_CBS);
   
    return ret;
    
}


int cmd_config_shaper_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t     context;
    cs_port_id_t                   port_id=0;
    cs_sdl_policy_t               rate;
    cs_uint8                        buffer[64];
    cs_status                       ret = CS_E_OK;

    if(argc != 1) {
        cli_arg_help(cli, 0, _cmd_usage_shaper_get  , NULL);
        return CLI_ERROR;
    }

    port_id = iros_strtol(argv[0]);
    if ( (port_id > DB_MAX_PORT)
        || (port_id ==0) ) {
        cli_print ( cli, "input port %d error", port_id);
        return CLI_ERROR;
    }

    memset ( &rate, 0 , sizeof (cs_sdl_policy_t ));    

    epon_request_onu_port_ds_rate_limit_get ( context, 
       ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API ,  port_id, &rate);

    sprintf (buffer, "port %d shaper rate", port_id );
    if ( rate.enable== CTC_PORT_POLICY_OPER_ENBALE) {
        
        cli_print(cli," %-20s: %d",  buffer, rate.rate);
    } else {
        cli_print(cli," %-20s: %s",  buffer, "disable");
    }

    return ret;
    
}


int cmd_config_qos_show_config_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t     context;
    cs_pri_cos_map_t          queue_map;
    cs_sdl_sched_type_t      sche_mode;
    cs_port_id_t                  port_id=0;
    cs_uint32                      queue_id=0;
    cs_uint16                      queue_size=0;
    cs_uint8                        pri=0;
    cs_sdl_policy_t              policy;
    cs_sdl_policy_t              rate;
    cs_uint8                        buffer[64];
    cs_status                      ret = CS_E_OK;

    if(argc != 0) {
        cli_arg_help(cli, 0, _cmd_usage_qos_config_get  , NULL);
        return CLI_ERROR;
    }

    memset ( &sche_mode, 0, sizeof (cs_sdl_sched_type_t ) );

    ret=epon_request_onu_schedule_mode_get ( context, ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API, &sche_mode);
    cli_print(cli," %-20s: %s",  "ONU schedule", sche_mode==SDL_SCHD_SP_MODE? "SP_MODE":"WRR_MODE");


   for (queue_id=0; queue_id < DB_MAX_VOQ_SIZE ; queue_id++ ) {
        ret=epon_request_onu_que_size_get ( context, 
                ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API , CS_PON_PORT_ID,  queue_id,  &queue_size);
        sprintf (buffer, "PON queue %d size", queue_id );
        cli_print(cli," %-20s: %d",  buffer, queue_size );
   }


    for (queue_id=0; queue_id < DB_MAX_VOQ_SIZE ; queue_id++ ) {
        ret=epon_request_onu_que_size_get ( context, 
                ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API , CS_UNI_PORT_ID1,  queue_id,  &queue_size);
        sprintf (buffer, "UNI queue %d size", queue_id );
        cli_print(cli," %-20s: %d",  buffer, queue_size );
   }

    ret=epon_request_onu_dot1p_map_get ( context, 
        ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API , &queue_map);

    for ( pri=0; pri < 8; pri++) {
        sprintf ( buffer, "8021p pri %d map cos", pri);
        cli_print (cli, " %-20s: %d", buffer, queue_map.cos[pri] );
    }
   
    for ( port_id=1; port_id < DB_MAX_PORT+1; port_id ++) {
        memset (&policy , 0, sizeof (cs_sdl_policy_t ) );
        ret=epon_request_onu_port_policy_get ( context, 
                    ONU_DEVICEID_FOR_API , ONU_LLIDPORT_FOR_API ,  port_id, &policy);
        sprintf ( buffer, "port(%d) up rate limit", port_id);
        if (policy.enable==1) {
            
            cli_print (cli, " %-20s: %d", buffer, policy.rate);
        }else {
            
            cli_print(cli," %-20s: %s", buffer, "disable" );
        }
    } 

    memset ( &rate, 0 , sizeof (cs_sdl_policy_t ));    

    epon_request_onu_port_ds_rate_limit_get ( context, 
       ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API ,  CS_UNI_PORT_ID1, &rate);
    sprintf (buffer, "port(%d) shaper rate", CS_UNI_PORT_ID1 );
    if ( rate.enable== CTC_PORT_POLICY_OPER_ENBALE) {
        cli_print(cli," %-20s: %d",  buffer, rate.rate);
    } else {
        cli_print(cli," %-20s: %s",  buffer, "disable");
    }
    

    return ret;
    
}

/*ONU(config-pon)*/
int cmd_config_fec_set  (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_uint32                       fec_mode=0;
    cs_status                       ret = CS_E_OK;
    
    if(argc !=1) {
        cli_arg_help(cli, 0, _cmd_usage_fec_set , NULL);
        return CLI_ERROR;
    }

    fec_mode = iros_strtol(argv[0]);
    
    if ( !((fec_mode ==0)
         ||(fec_mode ==1) )) {
        cli_print ( cli, "input fec mode  %d error", fec_mode);
        return CLI_ERROR;
    }

    if ( fec_mode==0 ) {
        fec_mode=OAM_INTF_FEC_MODE_DIS;
    }else {
        fec_mode=OAM_INTF_FEC_MODE_EN;
    }

    
    ret=ctc_oam_eth_fec_mode_set_adapt ( fec_mode);

   
    return ret;
    
}

int cmd_config_fec_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_callback_context_t     context;
    cs_boolean                    enable=0;
    cs_status                       ret = CS_E_OK;
    
    if(argc !=0) {
        cli_arg_help(cli, 0, _cmd_usage_fec_get , NULL);
        return CLI_ERROR;
    }

    ret=epon_request_onu_fec_get ( context,  ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API , & enable);
    cli_print(cli, " %-20s: %s", "pon fec", enable==1? "enable":"disable");

    return ret;
    
}

int cmd_config_tx_power_set  (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_uint8                    enable=0;
    cs_callback_context_t       context;
    cs_status                   ret = CS_E_OK;
    
    if(argc !=1) {
        cli_arg_help(cli, 0, _cmd_usage_tx_power_set , NULL);
        return CLI_ERROR;
    }

    enable = (cs_uint8)iros_strtol(argv[0]);
    
    if ( !((enable ==0)
         ||(enable ==1) )) {
        cli_print ( cli, "input enable  %d error", enable);
        return CLI_ERROR;
    }
    
    ret=epon_request_onu_pon_trans_tx_pwr_set ( context, ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API ,  enable);
    return ret;
    
}

int cmd_config_tx_power_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_callback_context_t     context;
    cs_boolean                    enable=0;
    cs_status                       ret = CS_E_OK;
    
    if(argc !=0) {
        cli_arg_help(cli, 0, _cmd_usage_tx_power_get , NULL);
        return CLI_ERROR;
    }

    ret=epon_request_onu_pon_trans_tx_pwr_get( context, ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API , & enable);

    cli_print(cli, " %-20s: %s", "pon tx power", enable==1? "enable":"disable");

    return ret;
    
}


int cmd_config_rx_power_set  (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_boolean                    enable=0;
    cs_callback_context_t     context;
    cs_status                       ret = CS_E_OK;
    
    if(argc !=1) {
        cli_arg_help(cli, 0, _cmd_usage_rx_power_set , NULL);
        return CLI_ERROR;
    }

    enable = iros_strtol(argv[0]);
    
    if ( !((enable ==0)
         ||(enable ==1) )) {
        cli_print ( cli, "input enable  %d error", enable);
        return CLI_ERROR;
    }
    
    ret=epon_request_onu_pon_trans_rx_pwr_set ( context, ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API ,  enable);
      
    return ret;
    
}

int cmd_config_rx_power_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_callback_context_t     context;
    cs_boolean                    enable=0;
    cs_status                       ret = CS_E_OK;
    
    if(argc !=0) {
        cli_arg_help(cli, 0, _cmd_usage_rx_power_get , NULL);
        return CLI_ERROR;
    }

    ret=epon_request_onu_pon_trans_rx_pwr_get( context, ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API , & enable);

    cli_print(cli, " %-20s: %s","pon rx power",  enable==1? "enable":"disable");

    return ret;
    
}

int cmd_config_pon_config_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_callback_context_t     context;
    cs_boolean                    enable=0;
    cs_status                       ret = CS_E_OK;
    
    if(argc !=0) {
        cli_arg_help(cli, 0, _cmd_usage_pon_show_config_get , NULL);
        return CLI_ERROR;
    }

    ret=epon_request_onu_fec_get ( context,  ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API , & enable);
    cli_print(cli, " %-20s: %s", "pon fec",enable==1? "enable":"disable");
    

    ret=epon_request_onu_pon_trans_tx_pwr_get( context, ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API , & enable);

    cli_print(cli, " %-20s: %s", "pon tx power", enable==1? "enable":"disable");
    

    ret=epon_request_onu_pon_trans_rx_pwr_get( context, ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API , & enable);

    cli_print(cli, " %-20s: %s", "pon rx power", enable==1? "enable":"disable");

    return ret;
    
}

int cmd_config_pon_status_get  (struct cli_def *cli, char *command, char *argv[], int argc)
{   
    cs_callback_context_t     context;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_boolean rx,tx,link,pm;
    cs_uint16 mtu;
    cs_sdl_port_loopback_t loopback = SDL_PORT_LOOPBACK_NONE;
    cs_status                       ret = CS_E_OK;
    
    if(argc !=0) {
        cli_arg_help(cli, 0, _cmd_usage_pon_show_status_get , NULL);
        return CLI_ERROR;
    }

    epon_request_onu_pon_rx_enable_get(context, 0, port, &rx);
    epon_request_onu_pon_tx_enable_get(context, 0, port, &tx);
    epon_request_onu_pon_link_get(context, 0, port, &link);
    epon_request_onu_pon_mtu_get(context, 0, port, &mtu);
    epon_request_onu_pon_preamble_mode_get(context, 0, port, &pm);
    epon_request_onu_port_lpbk_get(context, 0, 0, port, &loopback);

    cli_print(cli, "\nMAC CONFIGURATION\n");
    cli_print(cli, "========================================\n");
    cli_print(cli, "  %-20s: %s\n", "rx", (rx ? "enable" : "disable"));
    cli_print(cli, "  %-20s: %s\n", "tx", (tx ? "enable" : "disable"));
    cli_print(cli, "  %-20s: %u\n", "mtu", mtu);
    cli_print(cli, "  %-20s: %s\n", "preamble mode", (pm ? "fixed 8B" : "7B/8B auto-adjust"));
    cli_print(cli, "  %-20s: %s\n", "link", (link ? "up" : "down"));
    cli_print(cli, "  %-20s: %s\n", "loopback", (loopback ? "TX2RX" : "NONE"));
    cli_print(cli, "========================================\n");

    return ret;
    
}
        
#define __CMD_VID_CHECK(cli, vid)                                              \
    do{                                                             \
        if(((vid) < 1) || ((vid) > 4094))      {                                          \
            cli_print(cli, "Invalid input! vlan range is 1 ~ 4094.\r\n");    \
            return CLI_ERROR;                                          \
        }   \
    }while(0)

// ONU(config-vlan)#mode set <port ID> <mode> <pvid> [vlan1] [vlan2]...
int cmd_config_vlan_mode_set(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status ret = CS_E_OK;
    cs_callback_context_t context;    
    cs_uint8 i = 0;
    cs_port_id_t port = 0;
    cs_uint16 vid = 0;
    cs_sdl_vlan_tag_t     def_vlan;
    cs_sdl_vlan_mode_t  vlan_mode;
    cs_sdl_vlan_cfg_t      vlan_cfg[8];
    cs_uint16                 cfg_nums;
    db_vlan_t           db_vlan;
    cs_uint16                 db_nums;    

    memset(&def_vlan, 0, sizeof(def_vlan));
    memset(&vlan_mode, 0, sizeof(vlan_mode));
    memset(&vlan_cfg, 0, sizeof(vlan_cfg));

    if(CLI_HELP_REQUESTED)
    {
        cli_arg_help(cli, 0, _cmd_usage_vlan_mode_set, NULL);
        return CLI_OK;
    }
    if(argc < 2) 
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, _cmd_usage_vlan_mode_set, NULL);
        return CLI_ERROR;
    }
    
    port = iros_strtol(argv[0]);
    if(port < CS_UNI_PORT_ID1 || port > CLI_MAX_UNI_PORT_ID)
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, _cmd_usage_vlan_mode_set, NULL);
        return CLI_ERROR;
    }
    
    vlan_mode = iros_strtol(argv[1]);
    if(vlan_mode > CTC_VLAN_MODE_TRUNK)
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, _cmd_usage_vlan_mode_set, NULL);
        return CLI_ERROR;
    }
    if(vlan_mode == CTC_VLAN_MODE_AGG)
    {
        cli_print(cli, "aggregation mode is not supportted currently!\r\n");
        return CLI_OK;
    }

    if(vlan_mode == CTC_VLAN_MODE_TRANSPARENT)   // mode set <port> <mode>
    {
        if(argc > 2)
        {
            cli_print(cli, "Invalid input!Command usage:\r\n");
            cli_arg_help(cli, 0, _cmd_usage_vlan_mode_set, NULL);
            return CLI_ERROR;
        }
        def_vlan.tpid = 0;
        def_vlan.vid = 0;
    }
    else // mode set <port> <mode> <pvid> [vlan1] [vlan2] ...
    {
        if(argc < 3)
        {
            cli_print(cli, "Invalid input!Command usage:\r\n");
            cli_arg_help(cli, 0, _cmd_usage_vlan_mode_set, NULL);
            return CLI_ERROR;
        }
        else
        {
            for(i = 2; i < argc; i++)
            {
                vid = iros_strtol(argv[i]);
                __CMD_VID_CHECK(cli, vid);
            }
        }
        def_vlan.tpid = 0x8100;
        def_vlan.vid = iros_strtol(argv[2]);
    }

    cfg_nums = 0;
    db_nums = 0;
    
    switch(vlan_mode){
        case CTC_VLAN_MODE_TRANSPARENT: // mode set <port> <mode>
            break;
            
        case CTC_VLAN_MODE_TAG: // mode set <port> <mode> <pvid> 
            if(argc > 5)
            {
                cli_print(cli, "Invalid input!Command usage:\r\n");
                cli_arg_help(cli, 0, _cmd_usage_vlan_mode_set, NULL);
                return CLI_ERROR;
            }
            break;
            
        case CTC_VLAN_MODE_TRANSLATE: // mode set <port> <mode> <pvid> [cvlan1] [svlan1] ...
            if(argc > 21 || (argc%2 == 0))   // max vlan translation num is 8
            {
                cli_print(cli, "Invalid input!Command usage:\r\n");
                cli_arg_help(cli, 0, _cmd_usage_vlan_mode_set, NULL);
                return CLI_ERROR;
            }
            cfg_nums = (argc - 3)/2;
            for(i = 0; i < cfg_nums; i++)
            {
                vlan_cfg[i].c_vlan.tpid = 0x8100;
                vlan_cfg[i].c_vlan.vid = iros_strtol(argv[i*2+3]);
                vlan_cfg[i].s_vlan.tpid = 0x8100;
                vlan_cfg[i].s_vlan.vid = iros_strtol(argv[i*2+4]);
            }
            break;
#if 0
        case CTC_VLAN_MODE_AGG: // mode set <port> <mode> <pvid > [agg vlan] [vlan1]...
            if(argc > 12 )   // max vlan aggregation num is 8, 
            {
                cli_print(cli, "Invalid input!Command usage:\r\n");
                cli_arg_help(cli, 0, _cmd_usage_vlan_mode_set, NULL);
                return CLI_ERROR;
            }
            cfg_nums = argc - 4;
            for(i = 0; i < cfg_nums; i++)
            {
                vlan_cfg[i].c_vlan.tpid = 0x8100;
                vlan_cfg[i].c_vlan.vid = iros_strtol(argv[i+4]);
                vlan_cfg[i].s_vlan.tpid = 0x8100;
                vlan_cfg[i].s_vlan.vid = iros_strtol(argv[3]);
            }
            break;           
#endif
        case CTC_VLAN_MODE_TRUNK: // mode set <port> <mode> <pvid > [vlan1]...
            if(argc > 11)   // max vlan trunk num is 8, 
            {
                cli_print(cli, "Invalid input!Command usage:\r\n");
                cli_arg_help(cli, 0, _cmd_usage_vlan_mode_set, NULL);
                return CLI_ERROR;
            }
            cfg_nums = argc - 3;
            for(i = 0; i < cfg_nums; i++)
            {
                vlan_cfg[i].c_vlan.tpid = 0x8100;
                vlan_cfg[i].c_vlan.vid = iros_strtol(argv[i+3]);
                vlan_cfg[i].s_vlan.tpid = 0x8100;
                vlan_cfg[i].s_vlan.vid = iros_strtol(argv[i+3]);
            }
            break;
            
        default:
            {
                cli_print(cli, "Invalid input!Command usage:\r\n");
                cli_arg_help(cli, 0, _cmd_usage_vlan_mode_set, NULL);
                return CLI_ERROR;
            }    
    }
    ret = epon_request_onu_vlan_set(context,0,0, port, def_vlan, vlan_mode, vlan_cfg, cfg_nums);
    if(ret != CS_E_OK)
    {
        cli_print(cli, "vlan mode set failed, ret %d\r\n", ret);
        return CLI_ERROR;
    }

    // start to write DB
    memset(&db_vlan,0x00,sizeof(db_vlan));
    db_read(DB_VLAN_ID,(cs_uint8 *) &db_vlan, sizeof(db_vlan));
    db_vlan.vlan[port - 1].vlan_mode = vlan_mode;
    db_vlan.vlan[port - 1].def_vlan.tpid = def_vlan.tpid;
    db_vlan.vlan[port - 1].def_vlan.tag = def_vlan.vid;
    memset(db_vlan.vlan[port -1 ].vlan,0x00,sizeof(db_vlan.vlan[port -1 ].vlan));
    if(vlan_mode == CTC_VLAN_MODE_TRANSPARENT)
    {
        db_nums = 0;
    }
    else if(vlan_mode == CTC_VLAN_MODE_AGG) // mode set <port> <mode> 
    {
        db_nums = 1; // mode set <port> <mode> <pvid> [agg vlan]
    }
    else
    {
        db_nums = argc - 3; // mode set <port> <mode> <pvid>
    }

    db_vlan.vlan[port -1 ].vlan_num = db_nums;
    if(vlan_mode == CTC_VLAN_MODE_AGG)
    {
        db_vlan.vlan[port -1 ].agg_vlan_num = argc - 4;
        if(argc > 3)
        {
            db_vlan.vlan[port -1 ].vlan[0].tpid = 0x8100;
            db_vlan.vlan[port -1 ].vlan[0].tag = iros_strtol(argv[3]);
            for(i = 0; i < db_vlan.vlan[port -1 ].agg_vlan_num; i ++)
            {
                db_vlan.vlan[port -1 ].vlan[i+1].tpid = 0x8100;
                db_vlan.vlan[port -1 ].vlan[i+1].tag = iros_strtol(argv[i+4]);
            }
        }
    }
    else
    {
        for(i = 0 ; i < db_nums; i ++){
            db_vlan.vlan[port -1 ].vlan[i].tpid = 0x8100;
            db_vlan.vlan[port -1 ].vlan[i].tag = iros_strtol(argv[i+3]);
        }
    }
    db_write(DB_VLAN_ID,(cs_uint8 *) &db_vlan, sizeof(db_vlan));    
    
    return CLI_OK;

}

int cmd_config_vlan_mode_get(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status ret = CS_E_OK;
    cs_callback_context_t context; 
    cs_uint8 i, j, port_num;
    cs_uint8   start_port_num=0;
    cs_port_id_t port = 0;
    cs_sdl_vlan_tag_t             def_vlan;   /* ignored for TRANSPARENT mode*/
    cs_sdl_vlan_mode_t            vlan_mode;
    cs_sdl_vlan_cfg_t             vlan_cfg[8];  /* ignored for TRANSPARENT mode */
    cs_uint16                     cfg_nums;    /* can be zero for modes */

    memset(&def_vlan, 0, sizeof(def_vlan));
    memset(&vlan_mode, 0, sizeof(vlan_mode));
    memset(&vlan_cfg, 0, sizeof(vlan_cfg));
    cfg_nums = 0;

    if(CLI_HELP_REQUESTED)
    {
        cli_arg_help(cli, 0, _cmd_usage_vlan_mode_get, NULL);
        return CLI_OK;
    }
    if(argc == 0) // show all port vlan mode
    {
        port = 0xff;
        start_port_num=CS_UNI_PORT_ID1;
        port_num = CLI_MAX_UNI_PORT_ID;
    }
    else if(argc == 1) // show one port vlan mode
    {
        port = iros_strtol(argv[0]);
        if(port < CS_UNI_PORT_ID1 || port > CLI_MAX_UNI_PORT_ID)
        {
            cli_print(cli, "Invalid input!Command usage:\r\n");
            cli_arg_help(cli, 0, _cmd_usage_vlan_mode_get, NULL);
            return CLI_ERROR;
        }
        start_port_num=port;
        port_num = port;
    }
    else
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, _cmd_usage_vlan_mode_get, NULL);
        return CLI_ERROR;
    }


    for(i = start_port_num; i <= port_num; i++)
    {
        ret = epon_request_onu_vlan_get(context,0,0, i, &def_vlan, &vlan_mode, &vlan_cfg[0], &cfg_nums);
        if(ret != CS_E_OK)
        {
            cli_print(cli, "vlan mode get failed, ret %d\r\n", ret);
            return CLI_ERROR;
        }
        switch(vlan_mode){
            case CTC_VLAN_MODE_TRANSPARENT:
                cli_print(cli, "port %d, vlan mode is transparent.", i);
                break;
                
            case CTC_VLAN_MODE_TAG:
                cli_print(cli, "port %d, vlan mode is tag.", i);
                cli_print(cli, "    pvid: %4d.\r\n", def_vlan.vid);
                break;
                
            case CTC_VLAN_MODE_TRANSLATE:
                cli_print(cli, "port %d, vlan mode is translation.", i);
                cli_print(cli, "    pvid: %4d.", def_vlan.vid);

                for(j = 0; j < cfg_nums; j++)
                {
                    cli_print(cli, "    index %d: cvlan %4d, svlan %4d.", j, vlan_cfg[j].c_vlan.vid, vlan_cfg[j].s_vlan.vid);
                }
                break;
            case CTC_VLAN_MODE_AGG:
                cli_print(cli, "port %d, vlan mode is aggregation.", i);
                cli_print(cli, "    pvid: %4d.", def_vlan.vid);

                if(cfg_nums !=0)
                {
                    cli_print(cli, "    agg vlan: %4d.", vlan_cfg[0].s_vlan.vid);
                }
                for(j = 0; j < cfg_nums; j++)
                {
                    cli_print(cli, "        index %d: vlan %4d.", j, vlan_cfg[j].c_vlan.vid);
                }
                break;
            case CTC_VLAN_MODE_TRUNK:
                cli_print(cli, "port %d, vlan mode is trunk.", i);
                cli_print(cli, "    pvid: %4d.", def_vlan.vid);

                for(j = 0; j < cfg_nums; j++)
                {
                    cli_print(cli, "        index %d: vlan %4d.", j, vlan_cfg[j].c_vlan.vid);
                }
                break;
                
            default:
                {
                    cli_print(cli, "unknow vlan mode\r\n");
                    return CLI_ERROR;
                }    
        }
    }    
    return CLI_OK;
} 

#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
// aging-time set <time>
int cmd_config_fdb_aging_time_set(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status ret = CS_E_OK;
    cs_callback_context_t context;    
    cs_uint32 time;
    db_onu_svc_t   svc;

    if(CLI_HELP_REQUESTED)
    {
        cli_arg_help(cli, 0, _cmd_usage_fdb_aging_time_set, NULL);
        return CLI_OK;
    }
    if(argc != 1)
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, _cmd_usage_fdb_aging_time_set, NULL);
        return CLI_ERROR;
    }
    
    time = iros_strtol(argv[0]);    
    ret = epon_request_onu_fdb_age_set(context, 
        ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,  time);
    if(ret != CS_E_OK)
    {
        cli_print(cli, "fdb aging time set failed, ret %d\r\n", ret);
            return CLI_ERROR;
     }       
    memset(&svc,0x00,sizeof(svc));
    db_read(DB_ONU_SVC_INFO_ID,(cs_uint8 *) &svc, sizeof(svc));
    svc.mac_aging_time = time;
    db_write(DB_ONU_SVC_INFO_ID,(cs_uint8 *) &svc, sizeof(svc));

    return CLI_OK;
}

int cmd_config_fdb_aging_time_get(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status ret = CS_E_OK;
    cs_callback_context_t context;    
    cs_uint32 time;

    if(CLI_HELP_REQUESTED)
    {
        cli_arg_help(cli, 0, _cmd_usage_fdb_aging_time_get, NULL);
        return CLI_OK;
    }
    if(argc != 0)
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, _cmd_usage_fdb_aging_time_get, NULL);
        return CLI_ERROR;
    }
    ret = epon_request_onu_fdb_age_get(context, 
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,  &time);
    if(ret != CS_E_OK)
    {
        cli_print(cli, "fdb aging time get failed, ret %d\r\n", ret);
        return CLI_ERROR;
    }      
    cli_print(cli, "mac aging time: %d ", time);

    return CLI_OK;
}

//entry show [port] [type]
int cmd_config_fdb_entry_show(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status ret = CS_E_OK;
    cs_callback_context_t context;
    cs_port_id_t port = 0;
    cs_sdl_fdb_entry_t    entry;
    cs_uint32   fdb_num = 0;
    cs_sdl_fdb_entry_get_mode_t mode;
    cs_uint16   offset = 0;
    cs_uint16 next = 0;

    if(CLI_HELP_REQUESTED)
    {
        cli_arg_help(cli, 0, _cmd_usage_fdb_entry_show, NULL);
        return CLI_OK;
    }
    if(argc == 0) // show all port fdb, all type
    {
        port = 0xff;
        mode = SDL_FDB_ENTRY_GET_MODE_ALL;
    }
    else if(argc == 1) // show all port fdb
    {
        port = iros_strtol(argv[0]);
        if(port < CS_UNI_PORT_ID1 || port > CLI_MAX_UNI_PORT_ID)
        {
            cli_print(cli, "Invalid input!Command usage:\r\n");
            cli_arg_help(cli, 0, _cmd_usage_fdb_entry_show, NULL);
            return CLI_ERROR;
        }
        mode = SDL_FDB_ENTRY_GET_MODE_ALL;
    }
    else if(argc == 2) // show one port fdb
    {
        port = iros_strtol(argv[0]);
        if(port < CS_UNI_PORT_ID1 || port > CLI_MAX_UNI_PORT_ID)
        {
            cli_print(cli, "Invalid input!Command usage:\r\n");
            cli_arg_help(cli, 0, _cmd_usage_fdb_entry_show, NULL);
            return CLI_ERROR;
        }
        if(!strcmp(argv[1], "dynamic"))
        {
            mode = SDL_FDB_ENTRY_GET_MODE_DYNAMIC;
        }
        else if(!strcmp(argv[1], "static"))
        {
            mode = SDL_FDB_ENTRY_GET_MODE_STATIC;
        }
        else if(!strcmp(argv[1], "all"))
        {
            mode = SDL_FDB_ENTRY_GET_MODE_ALL;
        }
        else
        {
            cli_print(cli, "Invalid input!Command usage:\r\n");
            cli_arg_help(cli, 0, _cmd_usage_fdb_entry_show, NULL);
            return CLI_ERROR;
        }
    }
    else
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, _cmd_usage_fdb_entry_show, NULL);
        return CLI_ERROR;
    }

    cli_print(cli, "index  port  vlan                mac     type");
    
    while (TRUE) {
#ifndef HAVE_MPORTS
        if(fdb_num >= 64)
            break;
#endif        
        memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));
        if(port != 0xff)
        {
            ret = epon_request_onu_fdb_entry_getnext_per_port(context, 0, 0, port, mode, offset, &next, &entry);
        }
        else
        {
            ret = epon_request_onu_fdb_entry_getnext(context, 0,  0, mode, offset, &next,  &entry);
        }

        if(ret == CS_E_NOT_FOUND)
            return CLI_OK;
            
        if(ret != CS_E_OK)
        {
            cli_print(cli, "fdb entry get failed, ret %d\r\n", ret);
            return CLI_ERROR;
        }   
        fdb_num ++;
        
        cli_print(cli, "   %2d     %d  %4d  %02x:%02x:%02x:%02x:%02x:%02x  %s",
            fdb_num, entry.port, entry.vlan_id, entry.mac.addr[0], entry.mac.addr[1], entry.mac.addr[2],
            entry.mac.addr[3], entry.mac.addr[4], entry.mac.addr[5], (entry.type == SDL_FDB_ENTRY_DYNAMIC)? "dynamic":" static");

        cs_thread_delay(20);
        offset = next;
    }
    
    return CLI_OK;
}

// mac-banding add <port> <mac>
int _mac_banding_op(cs_uint8 action, struct cli_def *cli, char *argv[], int argc)
{
    cs_status ret = CS_E_OK;
    cs_callback_context_t context;
    int i = 0;
    cs_port_id_t port = 0;
    char *usage;
    char mac[CS_MACADDR_LEN];
    cs_sdl_mac_tbl_entry_t mac_entry;
    cs_sdl_mac_tbl_entry_t    macs[DB_MAX_MAC_BIND_NUM];
    db_mac_bind_t mac_bind;
    cs_uint8 entry_count;
    
    memset(mac, 0, CS_MACADDR_LEN);
    memset(&mac_entry, 0, sizeof(mac_entry));
    memset(&macs, 0, sizeof(macs));

    if(action == 0)
    {
        usage = _cmd_usage_fdb_mac_banding_add;
    }
    else if(action == 1)
    {
        usage = _cmd_usage_fdb_mac_banding_del;
    }

    if(CLI_HELP_REQUESTED)
    {
        cli_arg_help(cli, 0, usage, NULL);
        return CLI_OK;
    }
    
    if(argc != 2)
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, usage, NULL);
        return CLI_ERROR;
    }
    
    port = iros_strtol(argv[0]);
    if(port < CS_UNI_PORT_ID1 || port > CLI_MAX_UNI_PORT_ID)
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, usage, NULL);
        return CLI_ERROR;
    }
        
    if(cs_str2mac(argv[1], mac) !=0 )
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, usage, NULL);
        return CLI_ERROR;
    }

    for(i = 0; i < CS_MACADDR_LEN; i++){
        mac_entry.mac.addr[i] = (cs_uint8) mac[i];
    }
    mac_entry.mac_type = SDL_SRC_MAC;
    
    if(action == 0)
    {
        ret = epon_request_onu_mac_bind_add_entry(context, 
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, 1, &mac_entry);
    }
    else if(action == 1)
    {
        ret = epon_request_onu_mac_bind_del_entry(context, 
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, 1, &mac_entry);
    }
    if(ret != CS_E_OK)
    {
        cli_print(cli, "mac banding entry operate failed, ret %d\r\n", ret);
        return CLI_ERROR;
    }
            
    /* get all mac from the sdl and save to db*/
    ret =  epon_request_onu_mac_bind_get_entry(context, 
                ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port,&entry_count, macs);

    if(ret != CS_E_OK)
    {
        cli_print(cli, "mac banding entry get failed, ret %d\r\n", ret);
        return CLI_ERROR;
    }
    memset(&mac_bind,0x00,sizeof(mac_bind));
    db_read(DB_MAC_BIND_ID,(cs_uint8 *) &mac_bind, sizeof(mac_bind));
    entry_count = entry_count > DB_MAX_MAC_BIND_NUM ?DB_MAX_MAC_BIND_NUM:entry_count;
    mac_bind.bind[port - 1].num = entry_count;
    for(i = 0; i < entry_count; i++){
        memcpy(mac_bind.bind[port - 1].mac[i].addr,macs[i].mac.addr,CS_MACADDR_LEN);
    }
    db_write(DB_MAC_BIND_ID,(cs_uint8 *) &mac_bind, sizeof(mac_bind));

    return CLI_OK;
}

// mac-banding add <port> <mac>
int cmd_config_fdb_mac_banding_add(struct cli_def *cli, char *command, char *argv[], int argc)
{
    return _mac_banding_op(0, cli, argv, argc);
}
int cmd_config_fdb_mac_banding_del(struct cli_def *cli, char *command, char *argv[], int argc)
{
    return _mac_banding_op(1, cli, argv, argc);
}

//mac-banding get [port]
int cmd_config_fdb_mac_banding_get(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status ret = CS_E_OK;
    cs_callback_context_t context;
    cs_uint8 i, j, port_num;
    cs_port_id_t port = 0;
    cs_uint8    mac_num = 0;
    cs_sdl_mac_tbl_entry_t  mac_list[16];
    
    if(argc == 0) // show all port fdb
    {
        port = 0xff;
        port_num = CLI_MAX_UNI_PORT_ID;
    }
    else if(argc == 1) // show one port fdb
    {
        port = iros_strtol(argv[0]);
        if(port < CS_UNI_PORT_ID1 || port > CLI_MAX_UNI_PORT_ID)
        {
            cli_print(cli, "Invalid input!Command usage:\r\n");
            cli_arg_help(cli, 0, _cmd_usage_fdb_mac_banding_get, NULL);
            return CLI_ERROR;
        }
        port_num = 1;
    }
    else
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, _cmd_usage_fdb_mac_banding_get, NULL);
        return CLI_ERROR;
    }

    cli_print(cli, "index  port  mac");

    for(i = CS_UNI_PORT_ID1; i <= port_num; i++)
    {
        ret = epon_request_onu_mac_bind_get_entry(context, 
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, i, &mac_num, mac_list);
        if(ret != CS_E_OK)
        {
            cli_print(cli, "mac banding entry get failed, ret %d\r\n", ret);
            return CLI_ERROR;
        }
        for(j = 0; j < mac_num; j++)
        {
            cli_print(cli, "   %02d     %d  %02x:%02x:%02x:%02x:%02x:%02x ",
                i, j, mac_list[j].mac.addr[0], mac_list[j].mac.addr[1], mac_list[j].mac.addr[2], 
                mac_list[j].mac.addr[3], mac_list[j].mac.addr[4], mac_list[j].mac.addr[5]);
        }
    }

    return CLI_OK;
}
#endif /* #ifndef HAVE_INACTIVE_REDUPLICATE_CMD */

int cmd_config_capture_syslog_set(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_uint32 level, module;
    cs_uint32 count = 0;
    cs_uint32 i = 0;
    cs_uint8 buf[25 * IROS_MID_MAX];
    cs_uint8 *ptr;

    if(CLI_HELP_REQUESTED)
    {
        cli_arg_help(cli, 0, _cmd_usage_capture_syslog_set, NULL);
        memset(buf, 0, sizeof(buf));
        ptr = &buf[0];
        for(i = 0; i < IROS_MID_MAX; i++)
        {
            if( 0 == (i%2))count+=sprintf(ptr + count, "\r\n        ");
            count+=sprintf(ptr + count, "%2d: %-8s  ", sys_mod_desc[i].mod_id, sys_mod_desc[i].mod_name);
        }
        cli_arg_help(cli, 0, buf, NULL);
        return CLI_OK;
    }
    if(argc == 1) // set log level fro all modules
    {
        level = iros_strtol(argv[0]);
        cs_log_remote_level_set_all(level);
    }
    else if(argc == 2) // set log level and module id
    {
        level = iros_strtol(argv[0]);
        module = iros_strtol(argv[1]);
        cs_log_remote_level_set(module, level);
    }
    else
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, _cmd_usage_capture_syslog_set, NULL);
  
        return CLI_ERROR;
    }
    return CLI_OK;
}

int cmd_config_capture_packet_set(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status ret = CS_E_OK;
    cs_callback_context_t        context;
    cs_boolean enable;

    if(CLI_HELP_REQUESTED)
    {
        cli_arg_help(cli, 0, _cmd_usage_capture_packet_set, NULL);
        return CLI_OK;
    }
    if(argc == 1) // set log level fro all modules
    {
        enable = (cs_boolean)iros_strtol(argv[0]);

        if(enable)
        {
            cs_log_remote_level_set(IROS_MID_CAPTURE, IROS_LOG_LEVEL_DBG0); 
        }
        else
        {
            cs_log_remote_level_set(IROS_MID_CAPTURE, IROS_LOG_LEVEL_CRI); 
        }
        
        ret = epon_request_onu_pkt_remote_capture_set(context, 0, 0, enable);
        if(ret != CS_E_OK)
        {
            cli_print(cli, "packet remote capture set failed, ret %d\r\n", ret);
            return ret;
        }
    }
    else
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, _cmd_usage_capture_packet_set, NULL);
        return CLI_ERROR;
    }
    return ret;
}

int cmd_config_capture_show(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_status ret = CS_E_OK;
    cs_callback_context_t        context;
    cs_boolean enable;

    if(CLI_HELP_REQUESTED)
    {
        cli_arg_help(cli, 0, _cmd_usage_capture_show, NULL);
        return CLI_OK;
    }
    if(argc != 0) 
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, _cmd_usage_capture_show, NULL);
        return CLI_ERROR;
    }
    
    ret = epon_request_onu_pkt_remote_capture_get(context, 0, 0, &enable);
    if(ret != CS_E_OK)
        return ret;
    cli_print(cli, "packet remote capture is %s.\r\n", (enable == TRUE)?"enabled":"disabled");
    return ret;
}

int cmd_config_logserver_ip_set(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_uint32 ip = DEF_REMOTE_SYSLOG_IP;

    if(CLI_HELP_REQUESTED)
    {
        cli_arg_help(cli, 0, _cmd_usage_logserver_ip_set, NULL);
        return CLI_OK;
    }
    if(argc == 1)
    {
        cs_str2ip(argv[0],&ip);
        cs_log_remoteip_set(ip);
    }
    else
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, _cmd_usage_logserver_ip_set, NULL);
        return CLI_ERROR;
    }
    return CLI_OK;
}

int cmd_config_logserver_ip_get(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_uint32 ip = DEF_REMOTE_SYSLOG_IP;

    if(CLI_HELP_REQUESTED)
    {
        cli_arg_help(cli, 0, _cmd_usage_logserver_ip_get, NULL);
        return CLI_OK;
    }
    if(argc == 0)
    {
        cs_log_remoteip_get(&ip);
        cli_print(cli, "log server ip is %d.%d.%d.%d.\r\n", (ip>>24)& 0xff, (ip>>16)& 0xff, (ip>>8)& 0xff, ip & 0xff);
    }
    else
    {
        cli_print(cli, "Invalid input!Command usage:\r\n");
        cli_arg_help(cli, 0, _cmd_usage_logserver_ip_get, NULL);
        return CLI_ERROR;
    }
    return CLI_OK;
}
#endif

void cli_reg_usr_cmd(struct cli_command **cmd_root)
{
 

    struct cli_command *c;
    struct cli_command *mode;
#if 1 /*Not Ready*/
	struct cli_command *aging_time;
#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
    struct cli_command *entry, *mac_banding;
    struct cli_command *igmp_mode=NULL;
#endif
    struct cli_command *mc_strip=NULL;
    struct cli_command *max_grp=NULL;
    struct cli_command *fast_leave=NULL;
    struct cli_command *mc_config=NULL;
#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
    struct cli_command *phy=NULL;
    struct cli_command *port_mode=NULL;
    struct cli_command *flow_control=NULL;
	struct cli_command *isolation=NULL;
#endif
    struct cli_command *storm_control=NULL;
    struct cli_command *mtu=NULL;
    struct cli_command *port_show=NULL;
    struct cli_command * qos_schedule=NULL;
    struct cli_command * qos_queue=NULL;
    struct cli_command * qos_1pcos=NULL;
    struct cli_command * qos_rate_limit=NULL;
    struct cli_command * qos_shaper=NULL;
    struct cli_command * qos_config=NULL; 
    struct cli_command * pon_fec=NULL;
    struct cli_command * pon_tx_power=NULL;
    struct cli_command * pon_rx_power=NULL;
    struct cli_command * pon_show=NULL;      
#endif   

/*Common Command*/
    c = cli_register_command(cmd_root, 0, "configure", NULL,                         PRIVILEGE_USER, MODE_EXEC, "Enter configuration mode");
        cli_register_command(cmd_root, c, "terminal", cli_int_configure_terminal,    PRIVILEGE_USER, MODE_EXEC, "Configure from the terminal");

	#ifndef HAVE_GWD_CLI
	//modefied by zhuxh, close the command "save", because it is conflicted with gwd command
    c = cli_register_command(cmd_root,  NULL, "save",NULL, PRIVILEGE_ADMIN, MODE_ANY, "Save ONU config");
        cli_register_command(cmd_root,  c, "config", cmd_save_config,PRIVILEGE_ADMIN, MODE_ANY, "Save ONU current config to flash");
	#endif

		#if 1
		//modefied by zhuxh, the command can be seen at any mode
		cli_register_command(cmd_root, 0, "reset", cmd_reset,PRIVILEGE_ADMIN, MODE_ANY, "Reset ONU");
		#else
        cli_register_command(cmd_root, 0, "reset", cmd_reset,PRIVILEGE_ADMIN, MODE_EXEC, "Reset ONU");
		#endif
        cli_register_command(cmd_root, 0, "reset-factory", cmd_reset_factory,PRIVILEGE_ADMIN, MODE_EXEC, "Reset ONU with factory setting");

        cli_register_command(cmd_root, NULL, "debug",    cmd_debug_mode_int, PRIVILEGE_ADMIN,   MODE_CONFIG,    "Enter debug mode");
        cli_register_command(cmd_root, NULL, "regular",  cmd_debug_regular,  PRIVILEGE_ADMIN,   MODE_DEBUG,     "Switch for regular callback");

		#if defined(HAVE_MINI_CLI)
        cli_register_command(cmd_root, NULL, "legacy",   cmd_debug_legacy,   PRIVILEGE_ADMIN,   MODE_DEBUG,     "Switch to legacy console CLI");
		#endif
    
/*ZTE F401M Telnet command*/

    c = cli_register_command(cmd_root, 0,       "set-auth",         NULL,                       PRIVILEGE_GUEST,    MODE_EXEC,      "Set auth User/password");
        cli_register_command(cmd_root, c,       "admin",            cmd_set_admin,              PRIVILEGE_ADMIN,    MODE_EXEC,      "Set admin user/password");
		cli_register_command(cmd_root, c,       "user",             cmd_set_user,               PRIVILEGE_USER,     MODE_EXEC,      "Set user user/password");
        cli_register_command(cmd_root, c,       "guest",            cmd_set_guest,              PRIVILEGE_GUEST,    MODE_EXEC,      "Set guest user/password");

    c = cli_register_command(cmd_root, NULL,    "serial-number",    NULL,                       PRIVILEGE_ADMIN,    MODE_CONFIG,    "ONU serial number");
        cli_register_command(cmd_root, c,       "get",              cmd_show_serial_number,     PRIVILEGE_ADMIN,    MODE_CONFIG,    "Get ONU serial number");
        cli_register_command(cmd_root, c,       "set",              cmd_serial_number,          PRIVILEGE_ADMIN,    MODE_CONFIG,    "Set ONU serial number");
    c = cli_register_command(cmd_root, NULL,    "ctc-loid-pass",    NULL,                       PRIVILEGE_ADMIN,    MODE_CONFIG,    "CTC loid and password");
        cli_register_command(cmd_root, c,       "get",              cmd_show_ctc_loid_password, PRIVILEGE_ADMIN,    MODE_CONFIG,    "Get CTC loid and password");
        cli_register_command(cmd_root, c,       "set",              cmd_ctc_loid_password,      PRIVILEGE_ADMIN,    MODE_CONFIG,    "Set CTC loid and password");

        cli_register_command(cmd_root, NULL,    "web-timeout",      cmd_web_timeout_interval,   PRIVILEGE_ADMIN,    MODE_CONFIG,    "Set Web Timeout interval (Unit:second)");  
        cli_register_command(cmd_root, NULL,    "telnet-timeout",   cmd_telnet_timeout_interval,   PRIVILEGE_ADMIN,    MODE_CONFIG,    "Set Telnet Timeout interval (Unit:second)");  

    // ONU(config-vlan)#
    cli_register_command(cmd_root, NULL,    "vlan-cfg",  cmd_config_vlan_mode_int,   PRIVILEGE_USER, MODE_CONFIG, "Enter vlan config mode");

    mode = cli_register_command(cmd_root, NULL,    "mode",  NULL, PRIVILEGE_USER, MODE_CONFIG_VLAN, "vlan configuration");
#if 1
    cli_register_command(cmd_root, mode,    "set",  cmd_config_vlan_mode_set, PRIVILEGE_USER, MODE_CONFIG_VLAN, "set vlan mode");
    cli_register_command(cmd_root, mode,    "get",  cmd_config_vlan_mode_get, PRIVILEGE_USER, MODE_CONFIG_VLAN, "get vlan mode");
#endif
	#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
    // ONU(config-fdb)#
    cli_register_command(cmd_root, NULL,    "fdb",  cmd_config_fdb_mode_int,   PRIVILEGE_USER, MODE_CONFIG, "Enter fdb config mode");
	#endif
#if 1
	#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
    aging_time = cli_register_command(cmd_root, NULL,    "aging-time",  NULL,   PRIVILEGE_USER, MODE_CONFIG_FDB, "fdb aging time configuration");
    cli_register_command(cmd_root, aging_time,    "set",  cmd_config_fdb_aging_time_set,   PRIVILEGE_USER, MODE_CONFIG_FDB, "set fdb aging time");
    cli_register_command(cmd_root, aging_time,    "get",  cmd_config_fdb_aging_time_get,   PRIVILEGE_USER, MODE_CONFIG_FDB, "get fdb aging time");
    entry = cli_register_command(cmd_root, NULL,    "entry",  NULL,   PRIVILEGE_USER, MODE_CONFIG_FDB, "fdb entry configuration");
    cli_register_command(cmd_root, entry,    "show",  cmd_config_fdb_entry_show,   PRIVILEGE_USER, MODE_CONFIG_FDB, "show fdb entry");
    mac_banding = cli_register_command(cmd_root, NULL,    "mac-banding",  NULL,   PRIVILEGE_USER, MODE_CONFIG_FDB, "mac banding configuration");
    cli_register_command(cmd_root, mac_banding,    "add",  cmd_config_fdb_mac_banding_add,   PRIVILEGE_USER, MODE_CONFIG_FDB, "add mac banding entry");
    cli_register_command(cmd_root, mac_banding,    "del",  cmd_config_fdb_mac_banding_del,   PRIVILEGE_USER, MODE_CONFIG_FDB, "delete mac banding entry");
    cli_register_command(cmd_root, mac_banding,    "get",  cmd_config_fdb_mac_banding_get,   PRIVILEGE_USER, MODE_CONFIG_FDB, "get mac banding entry");
	#endif

    /*ONU(config-igmp)#*/
    cli_register_command(cmd_root, NULL,    "igmp",  cmd_config_igmp_mode_int,   PRIVILEGE_USER, MODE_CONFIG, "Enter igmp config mode");
	#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
	igmp_mode = cli_register_command(cmd_root, NULL,    "mode",  NULL,   PRIVILEGE_USER, MODE_CONFIG_IGMP, "IGMP mode configuration");
    cli_register_command(cmd_root, igmp_mode,    "set",  cmd_config_igmp_mode_set,   PRIVILEGE_ADMIN, MODE_CONFIG_IGMP, "set igmp mode");
    cli_register_command(cmd_root, igmp_mode,    "get",  cmd_config_igmp_mode_get,   PRIVILEGE_USER, MODE_CONFIG_IGMP, "get igmp mode");
	#endif
    aging_time = cli_register_command(cmd_root, NULL,    "aging-time",  NULL,   PRIVILEGE_USER, MODE_CONFIG_IGMP, "igmp aging time configuration");
    cli_register_command(cmd_root, aging_time,    "set",  cmd_config_igmp_aging_time_set,   PRIVILEGE_ADMIN, MODE_CONFIG_IGMP, "set igmp aging time");
    cli_register_command(cmd_root, aging_time,    "get",  cmd_config_igmp_aging_time_get,   PRIVILEGE_USER, MODE_CONFIG_IGMP, "get igmp aging time");
    
    mc_strip = cli_register_command(cmd_root, NULL,    "strip",  NULL,   PRIVILEGE_USER, MODE_CONFIG_IGMP, "mc tagstrip configuration");
    cli_register_command(cmd_root, mc_strip,    "set",  cmd_config_mc_tagstrip_set,   PRIVILEGE_ADMIN, MODE_CONFIG_IGMP, "set tagstrip");
    cli_register_command(cmd_root, mc_strip,    "get",  cmd_config_mc_tagstrip_get,   PRIVILEGE_USER, MODE_CONFIG_IGMP, "get tagstrip");

    max_grp = cli_register_command(cmd_root, NULL,    "max-group",  NULL,   PRIVILEGE_USER, MODE_CONFIG_IGMP, "mc max group configuration");
    cli_register_command(cmd_root, max_grp,    "set",  cmd_config_mc_max_grp_set,   PRIVILEGE_ADMIN, MODE_CONFIG_IGMP, "set mc max group");
        cli_register_command(cmd_root, max_grp,    "get",  cmd_config_mc_max_grp_get,   PRIVILEGE_USER, MODE_CONFIG_IGMP, "get mc max group");

    fast_leave = cli_register_command(cmd_root, NULL,    "fast-leave",  NULL,   PRIVILEGE_USER, MODE_CONFIG_IGMP, "mc fast-leave configuration");
    cli_register_command(cmd_root, fast_leave,    "set",  cmd_config_mc_fast_leave_set,   PRIVILEGE_ADMIN, MODE_CONFIG_IGMP, "set fast-leave");
    cli_register_command(cmd_root, fast_leave,    "get",  cmd_config_mc_fast_leave_get,   PRIVILEGE_USER, MODE_CONFIG_IGMP, "get fast-leave");
    
    mc_config = cli_register_command(cmd_root, NULL,    "show",  NULL,   PRIVILEGE_USER, MODE_CONFIG_IGMP, "show igmp infor");
    cli_register_command(cmd_root, mc_config,    "config",  cmd_config_mc_config_get,   PRIVILEGE_USER, MODE_CONFIG_IGMP, "show igmp configuration");
    cli_register_command(cmd_root, mc_config,    "group",  cmd_config_mc_port_grp_get  ,   PRIVILEGE_USER, MODE_CONFIG_IGMP, "show igmp group");


    /*UNI command*/
    cli_register_command(cmd_root, NULL,    "uni",  cmd_config_uni_mode_int,   PRIVILEGE_USER, MODE_CONFIG, "Enter uni config mode");
	#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
	phy = cli_register_command(cmd_root, NULL,    "phy",  NULL,   PRIVILEGE_USER, MODE_CONFIG_UNI, "port phy configuration");
    cli_register_command(cmd_root, phy,    "set",  cmd_config_phy_admin_set,   PRIVILEGE_USER, MODE_CONFIG_UNI, "set phy admin");
    cli_register_command(cmd_root, phy,    "get",  cmd_config_phy_admin_get,   PRIVILEGE_USER, MODE_CONFIG_UNI, "get phy admin state");
	
    port_mode = cli_register_command (cmd_root, NULL,    "mode",  NULL,   PRIVILEGE_USER, MODE_CONFIG_UNI, "port mode configuration");
    cli_register_command (cmd_root, port_mode,    "set",  cmd_config_port_mode_set,   PRIVILEGE_USER, MODE_CONFIG_UNI, "set port mode");
    cli_register_command (cmd_root, port_mode,    "get",  cmd_config_port_mode_get,   PRIVILEGE_USER, MODE_CONFIG_UNI, "get port mode");
	
    flow_control = cli_register_command (cmd_root, NULL,    "flow-control",  NULL,   PRIVILEGE_USER, MODE_CONFIG_UNI, "port flow control configuration");
    cli_register_command (cmd_root, flow_control,    "set",  cmd_config_port_flow_control_set,   PRIVILEGE_USER, MODE_CONFIG_UNI, "set port flow control ");
    cli_register_command (cmd_root, flow_control,    "get",  cmd_config_port_flow_control_get,   PRIVILEGE_USER, MODE_CONFIG_UNI, "get port flow control ");
	#endif


    storm_control = cli_register_command (cmd_root, NULL,    "storm-control",  NULL,   PRIVILEGE_USER, MODE_CONFIG_UNI, "port storm control configuration");
    cli_register_command (cmd_root, storm_control,    "set",  cmd_config_port_storm_control_set ,   PRIVILEGE_USER, MODE_CONFIG_UNI, "set port storm control");
    cli_register_command (cmd_root, storm_control,    "get",  cmd_config_port_storm_control_get,   PRIVILEGE_USER, MODE_CONFIG_UNI, "get port storm control");

    #ifndef HAVE_INACTIVE_REDUPLICATE_CMD
	isolation = cli_register_command (cmd_root, NULL,    "isolation",  NULL,   PRIVILEGE_USER, MODE_CONFIG_UNI, "port isolation configuration");
    cli_register_command (cmd_root, isolation,    "set",  cmd_config_port_isolation_set ,   PRIVILEGE_USER, MODE_CONFIG_UNI, "set port isolation");
    cli_register_command (cmd_root, isolation,    "get",  cmd_config_port_isolation_get,   PRIVILEGE_USER, MODE_CONFIG_UNI, "get port isolation");
	#endif


    mtu = cli_register_command (cmd_root, NULL,    "mtu",  NULL,   PRIVILEGE_USER, MODE_CONFIG_UNI, "port mtu configuration");
    cli_register_command (cmd_root, mtu,    "set",  cmd_config_port_mtu_set ,   PRIVILEGE_USER, MODE_CONFIG_UNI, "set port mtu");
    cli_register_command (cmd_root, mtu,    "get",  cmd_config_port_mtu_get,   PRIVILEGE_USER, MODE_CONFIG_UNI, "get port mtu");

    port_show = cli_register_command (cmd_root, NULL,    "show",  NULL,   PRIVILEGE_USER, MODE_CONFIG_UNI, "port show");
    cli_register_command (cmd_root, port_show,    "config",  cmd_config_port_config_get ,   PRIVILEGE_USER, MODE_CONFIG_UNI, "show port config");
    cli_register_command (cmd_root, port_show,    "status",  cmd_config_port_status_get,   PRIVILEGE_USER, MODE_CONFIG_UNI, "show port status");

#ifdef HAVE_ONU_RSTP
    cli_register_command(cmd_root, port_show,    "stp",  cmd_show_run_stp,   PRIVILEGE_USER, MODE_CONFIG_UNI, "The stp information");
#endif
    /*ONU(config-qos)*/
     cli_register_command(cmd_root, NULL,    "qos",  cmd_config_qos_mode_int,   PRIVILEGE_USER, MODE_CONFIG, "Enter qos config mode");

    qos_schedule = cli_register_command(cmd_root, NULL,    "schedule",  NULL,   PRIVILEGE_USER, MODE_CONFIG_QOS, "qos algorithm configuration");
    cli_register_command(cmd_root, qos_schedule,    "set",  cmd_config_onu_schedule_set,   PRIVILEGE_USER, MODE_CONFIG_QOS, "set qos schedule");
    cli_register_command(cmd_root, qos_schedule,    "get",  cmd_config_onu_schedule_get,   PRIVILEGE_USER, MODE_CONFIG_QOS, "get qos schedule");

    qos_queue  = cli_register_command(cmd_root, NULL,    "queue-size",  NULL,   PRIVILEGE_USER, MODE_CONFIG_QOS, "qos queue size configuration");
    cli_register_command(cmd_root, qos_queue ,    "set",  cmd_config_queue_size_set,   PRIVILEGE_USER, MODE_CONFIG_QOS, "set qos queue size");
    cli_register_command(cmd_root, qos_queue ,    "get",  cmd_config_queue_size_get,   PRIVILEGE_USER, MODE_CONFIG_QOS, "get qos queue size");
    
    qos_1pcos  = cli_register_command(cmd_root, NULL,    "1pcos",  NULL,   PRIVILEGE_USER, MODE_CONFIG_QOS, "qos 1pcos configuration");
    cli_register_command(cmd_root, qos_1pcos ,    "set",  cmd_config_1pcos_set,   PRIVILEGE_USER, MODE_CONFIG_QOS, "set qos 1pcos");
    cli_register_command(cmd_root, qos_1pcos ,    "get",  cmd_config_1pcos_get,   PRIVILEGE_USER, MODE_CONFIG_QOS, "get qos 1pcos");
   
    qos_rate_limit  = cli_register_command(cmd_root, NULL,    "rate-limit",  NULL,   PRIVILEGE_USER, MODE_CONFIG_QOS, "qos UNI rate limit configuration");
    cli_register_command(cmd_root, qos_rate_limit ,    "set",  cmd_config_rate_limit_set,   PRIVILEGE_USER, MODE_CONFIG_QOS, "set qos rate limit");
    cli_register_command(cmd_root, qos_rate_limit ,    "get",  cmd_config_rate_limit_get,   PRIVILEGE_USER, MODE_CONFIG_QOS, "get qos rate limit");

    qos_shaper  = cli_register_command(cmd_root, NULL,    "shaper",  NULL,   PRIVILEGE_USER, MODE_CONFIG_QOS, "qos PON shaper configuration");
    cli_register_command(cmd_root, qos_shaper ,    "set",  cmd_config_shaper_set,   PRIVILEGE_USER, MODE_CONFIG_QOS, "set qos shaper");
    cli_register_command(cmd_root, qos_shaper ,    "get",  cmd_config_shaper_get,   PRIVILEGE_USER, MODE_CONFIG_QOS, "get qos shaper");
    

    qos_config  = cli_register_command(cmd_root, NULL,    "show",  NULL,   PRIVILEGE_USER, MODE_CONFIG_QOS, "qos show configuration");
    cli_register_command(cmd_root, qos_config ,    "config",  cmd_config_qos_show_config_get,   PRIVILEGE_USER, MODE_CONFIG_QOS, "qos show");

    /*ONU(config-pon)*/
    cli_register_command(cmd_root, NULL,    "pon",  cmd_config_pon_mode_int,   PRIVILEGE_USER, MODE_CONFIG, "Enter pon config mode");
    pon_fec = cli_register_command(cmd_root, NULL,    "fec",  NULL,   PRIVILEGE_USER, MODE_CONFIG_PON, "pon fec configuration");
    cli_register_command(cmd_root, pon_fec ,    "set",  cmd_config_fec_set,   PRIVILEGE_ADMIN, MODE_CONFIG_PON, "set fec");
    cli_register_command(cmd_root, pon_fec ,    "get",  cmd_config_fec_get,   PRIVILEGE_USER, MODE_CONFIG_PON, "get fec");

    pon_tx_power = cli_register_command(cmd_root, NULL,    "tx-power",  NULL,   PRIVILEGE_USER, MODE_CONFIG_PON, "pon tx power configuration");
    cli_register_command(cmd_root, pon_tx_power ,    "set",  cmd_config_tx_power_set,   PRIVILEGE_ADMIN, MODE_CONFIG_PON, "set tx power ");
    cli_register_command(cmd_root, pon_tx_power ,    "get",  cmd_config_tx_power_get,   PRIVILEGE_USER, MODE_CONFIG_PON, "get tx power ");
    

    pon_rx_power = cli_register_command(cmd_root, NULL,    "rx-power",  NULL,   PRIVILEGE_USER, MODE_CONFIG_PON, "pon rx power configuration");
    cli_register_command(cmd_root, pon_rx_power ,    "set",  cmd_config_rx_power_set,   PRIVILEGE_ADMIN, MODE_CONFIG_PON, "set rx power ");
    cli_register_command(cmd_root, pon_rx_power ,    "get",  cmd_config_rx_power_get,   PRIVILEGE_USER, MODE_CONFIG_PON, "get rx power ");

    pon_show = cli_register_command(cmd_root, NULL,    "show",  NULL,   PRIVILEGE_USER, MODE_CONFIG_PON, "show pon  infor");
    cli_register_command(cmd_root, pon_show ,    "config",  cmd_config_pon_config_get,   PRIVILEGE_USER, MODE_CONFIG_PON, "show pon configuration");
    cli_register_command(cmd_root, pon_show ,    "status",  cmd_config_pon_status_get,   PRIVILEGE_USER, MODE_CONFIG_PON, "show pon status");
#endif
    /*ONU(config-mgmt)*/
    c = cli_register_command(cmd_root, NULL,    "statistics",       NULL,                       PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,      "System statistics information");
        cli_register_command(cmd_root, c,       "show",             cmd_statistics_show,        PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,      "Show statistics");
        cli_register_command(cmd_root, c,       "clear",            cmd_statistics_clear,       PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,      "Clear statistics");
        cli_register_command(cmd_root, c,       "cpu",              cmd_statistics_cpu,         PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,      "cpu statistics");

    c = cli_register_command(cmd_root, NULL,    "onu-mac",          NULL,               PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,     "ONU PON mac");
        cli_register_command(cmd_root, c,       "set",              cmd_pon_mac,        PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,     "ONU PON mac Set");
        cli_register_command(cmd_root, c,       "get",              cmd_show_pon_mac,       PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,     "ONU PON mac Get");

    c = cli_register_command(cmd_root, NULL,    "laser-on",         NULL,              PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,     "ONU Laser on");
        cli_register_command(cmd_root, c,       "set",              cmd_laser,         PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,     "ONU Laser on Set");
        cli_register_command(cmd_root, c,       "get",              cmd_show_laser,    PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,     "ONU Laser on Get");

        cli_register_command(cmd_root, NULL,    "show-version", cmd_show_version_detail, PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT, "Show version");
#if 1
    c = cli_register_command(cmd_root, NULL,    "ip-mtu",         NULL,              PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,     "IP mtu");
        cli_register_command(cmd_root, c,       "set",            cmd_config_ip_mtu_set,         PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,     "IP mtu Set");
        cli_register_command(cmd_root, c,       "get",            cmd_config_ip_mtu_get,    PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,     "IP mtu Get");
#endif

#ifndef HAVE_MPORTS
    c = cli_register_command(cmd_root, NULL,    "laser-detect-interval",NULL,            PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,     "laser on detect interval");
        cli_register_command(cmd_root, c,       "set",cmd_laser_on_detect_interval_set,    PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,     "ONU Laser on Set");
        cli_register_command(cmd_root, c,       "get",cmd_laser_on_detect_interval_get,    PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,     "ONU Laser on Get");
#endif

#if 1
    c = cli_register_command(cmd_root, NULL,       "remote-capture",            NULL,       PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,      "remote capture");
    cli_register_command(cmd_root, c,       "syslog",            cmd_config_capture_syslog_set,       PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,      "remote capture syslog");
    cli_register_command(cmd_root, c,       "packet",            cmd_config_capture_packet_set,       PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,      "remote capture packets");
    cli_register_command(cmd_root, c,       "show",            cmd_config_capture_show,       PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,      "remote capture config show");

    c = cli_register_command(cmd_root, NULL,       "logserver-ip",            NULL,       PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,      "syslog server ip");
    cli_register_command(cmd_root, c,       "set",            cmd_config_logserver_ip_set,       PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,      "set syslog server ip");
    cli_register_command(cmd_root, c,       "get",            cmd_config_logserver_ip_get,       PRIVILEGE_ADMIN,    MODE_CONFIG_MGMT,      "get syslog server ip");
#endif

    c = cli_register_command(cmd_root, NULL,    "show",  NULL,   PRIVILEGE_USER, MODE_CONFIG_MGMT, "show system infor");
    cli_register_command(cmd_root, c , "system", cmd_show_run,   PRIVILEGE_USER, MODE_CONFIG_MGMT, "show run system configuration");

    cli_register_command(cmd_root, NULL, "led", cmd_led, PRIVILEGE_ADMIN, MODE_CONFIG_MGMT, "Led on/off/normal");


  
    return;
}

int check_auth(char *username, char *password, struct cli_def *cli)
{
    //check default user
    if( !strcasecmp(username, admin_level[USR_INDEX]) &&
        !strcasecmp(password, admin_level[PWD_INDEX]) )
    {
        cli_set_privilege(cli, PRIVILEGE_ADMIN);
        cli_set_configmode(cli, MODE_CONFIG, NULL);
        

        return CLI_OK;
    }

    if( !strcasecmp(username, user_level[USR_INDEX]) &&
        !strcasecmp(password, user_level[PWD_INDEX]) )
    {
        cli_set_privilege(cli, PRIVILEGE_USER);
        cli_set_configmode(cli, MODE_EXEC, NULL);
        return CLI_OK;
    }

    if( !strcasecmp(username, guest_level[USR_INDEX]) &&
        !strcasecmp(password, guest_level[PWD_INDEX]) )
    {
        cli_set_privilege(cli, PRIVILEGE_GUEST);
        cli_set_configmode(cli, MODE_EXEC, NULL);
        return CLI_OK;
    }

    return CLI_ERROR;
}



int regular_callback(struct cli_def *cli)
{
    regular_count++;
    if (debug_regular)
    {
        cli_print(cli, "Regular callback - %u times so far", regular_count);
        cli_reprompt(cli);
    }
    return CLI_OK;
}

int check_enable(char *password)
{
    // diag_printf("%s", password);
    return !strcasecmp(password, DFT_ENABLE_PASSWD);

}


void cli_usr_init(struct cli_def *cli)
{

    // config env
    cli_set_banner(cli, CLI_BANNER);
    cli_set_hostname(cli, HOST_NAME);

	#ifdef HAVE_USER_PASSWORD_RESTORE
    {
        db_user_t telusr;
        
        memset(&telusr,0,sizeof(db_user_t));
        if(db_read(DB_TEL_ADMIN_INFO_ID,(cs_uint8 *)&telusr,sizeof(db_user_t))==CS_E_OK)
        {
            memcpy(admin_level[USR_INDEX],telusr.usr,DB_MAX_USR_LEN);
            memcpy(admin_level[PWD_INDEX],telusr.pwd,DB_MAX_PWD_LEN);
        }
        
        memset(&telusr,0,sizeof(db_user_t));
        if(db_read(DB_TEL_USER_INFO_ID,(cs_uint8 *)&telusr,sizeof(db_user_t))==CS_E_OK)
        {
            memcpy(user_level[USR_INDEX],telusr.usr,DB_MAX_USR_LEN);
            memcpy(user_level[PWD_INDEX],telusr.pwd,DB_MAX_PWD_LEN);
        }
        
        memset(&telusr,0,sizeof(db_user_t));
        if(db_read(DB_TEL_GUEST_INFO_ID,(cs_uint8 *)&telusr,sizeof(db_user_t))==CS_E_OK)
        {
            memcpy(guest_level[USR_INDEX],telusr.usr,DB_MAX_USR_LEN);
            memcpy(guest_level[PWD_INDEX],telusr.pwd,DB_MAX_PWD_LEN);
        }

    }
	#endif
	
    cli_regular(cli, regular_callback);
    cli_regular_interval(cli, 5); // change to 5 from default 1 second
    
    cli_set_auth_callback(cli, check_auth);
    /*cli_set_enable_callback(cli, check_enable);*/

    return;
}

#endif
