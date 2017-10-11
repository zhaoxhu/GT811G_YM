#include "iros_config.h"
#ifdef HAVE_GWD_CLI
#include "gwd_cli.h"
#include "app_oam_gwd.h"
#include "sdl_fdb.h"
#include "sdl.h"
#include "sockets.h"
#include "inet.h"
#include "sdl_port.h"
#include "gwd_utils.h"
#ifdef HAVE_IP_STACK
#include "app_ip_api.h"
#endif
#ifdef HAVE_GWD_FLASH_OPER
#include "gwd_flash_oper.h"
#endif
#ifdef HAVE_GWD_IGMP_CMD
#include "mc_type.h"
#include "mc_api.h"
#endif
#ifdef HAVE_QINQ
#include "gwd_qinq.h"
#endif
#ifdef HAVE_GWD_MC_TVM
#include "gwd_mc_process.h"
#endif
#ifdef HAVE_GWD_BC_LIMIT
#include "gwd_broadstorm.h"
#endif
#include "port_stats.h"
#if (HAVE_GE_RATE_LIMIIT_CMD == MODULE_YES)
#include "app_oam_ctc_adapt.h"
#endif
#ifdef HAVE_ARP_DOWN_INTO_CPU_LIMIT
#include "sdl_l2.h"
#include "packet.h"
#endif
#ifdef HAVE_PORT_MIRROR_CMD
#include "rtk_api.h"
#include "rtk_api_ext.h"
#endif
#ifdef HAVE_EVENT_SHOW_CMD
#include "gwd_log.h"
#endif
#ifdef HAVE_VLAN_CMD
#include "sdl_vlan.h"
#endif
#ifdef HAVE_PLATFORM_UNIFY_CMD
#include "app_ip_init.h"
#include "app_ip_util.h"
#endif
#ifdef HAVE_ETH0_IP_ADDR_RESTORE
#include "app_ip_adapter.h"
#endif
#include "app_alarm_ctc.h"

extern char const iros_version[];

int ctc_version_set = 0;

#ifdef HAVE_VLAN_CMD
/* vlan dot1q cmd */
#define HAVE_VLAN_DOT1Q_CMD

/*port isolation command*/
#define HAVE_PORT_ISOLATION_CMD
#endif

#ifdef HAVE_RESET_CONTROL
static cs_boolean g_onu_offline_reset_enable = true;
void onu_reset_enable_set(cs_boolean enable)
{
	g_onu_offline_reset_enable = enable;
}

cs_boolean onu_reset_enable_get(void)
{
	return g_onu_offline_reset_enable;
}

void onu_reset_enable_init(void)
{
	onu_reset_enable_set(true);
}
#endif	/*#ifdef HAVE_RESET_CONTROL*/

#ifdef HAVE_MPCP_REG_TIMEOUT_RESET
#define TIME_OUT_DEFAULT	10000		//毫秒

extern cs_status aal_pon_link_get(cs_boolean *link);

int mpcp_reg_status = 0;
cs_status mpcp_reg_status_setter(int enable)
{
	mpcp_reg_status = enable;

	return CS_E_OK;
}

cs_status mpcp_reg_status_getter(int *enable)
{
	if(NULL == enable)
	{
		cs_printf("null pointer. FILE: %s, LINE: %d\n", __FILE__, __LINE__);
		return CS_E_ERROR;
	}

	*enable = mpcp_reg_status;
	
	return CS_E_OK;
}

void mpcp_reg_time_out_proc(void *data)
{
	cs_boolean link;
	
	if(CS_E_OK == aal_pon_link_get(&link))
	{
		if(link)
		{
			int enable;
			if(CS_E_OK == mpcp_reg_status_getter(&enable))
			{
				if(0 == enable)
				{
					#ifdef HAVE_RESET_CONTROL
					if(onu_reset_enable_get())
					{
						//reset onu
						iros_system_reset(RESET_TYPE_FORCE);
					}
					#else
					//reset onu
					iros_system_reset(RESET_TYPE_FORCE);
					#endif
				}
			}
			
		}
	}	
}

void mpcp_reg_time_out_monitor()
{
	cs_circle_timer_add(TIME_OUT_DEFAULT, mpcp_reg_time_out_proc, NULL);
}
#endif	/*(#ifdef HAVE_MPCP_REG_TIMEOUT_RESET)*/

#ifdef HAVE_ARP_DOWN_INTO_CPU_LIMIT
#define ARP_MONITOR_INTERVAL_TIME	1000	//毫秒
#define ARP_INTO_CPU_RATE_THRESHOLD	100		//每秒100个
#define ARP_INTO_CPU_DELAY_PERIOD 	10		//arp 报文延时进入cpu 的周期
void arp_down_into_cpu_limit_proc(void *data)
{
	//入口规则检查
	if(NULL == data)
	{
		return;
	}
	
	cs_uint32 timeout = (cs_uint32)data;
	static cs_uint32 count_history = 0;
	cs_uint32 count_current = 0;
	cs_uint32 arp_rate = 0;
	float interval_time = 0;
	static int oper_status = 0;
	cs_callback_context_t context;
	cs_sdl_pkt_state_t state;
	static int period_count = 0;
	interval_time = timeout/1000;
	count_current = app_pkt_get_counter(CS_PKT_ARP);
	arp_rate = (count_current - count_history)/interval_time;
	count_history = count_current;

	if(ARP_INTO_CPU_RATE_THRESHOLD < arp_rate)
	{
		//禁止下行arp 报文进入cpu
		epon_request_onu_spec_pkt_state_get(context, 0, 0, CS_DOWN_STREAM, CS_PKT_ARP, &state);
		if(DST_CPU == state)
		{
			epon_request_onu_spec_pkt_state_set(context, 0, 0, CS_DOWN_STREAM, CS_PKT_ARP, DST_FE);
			
			//更新状态
			oper_status = 1;
		}	
	}
	else
	{
		if(1 == oper_status)
		{	
			period_count++;
			if(ARP_INTO_CPU_DELAY_PERIOD <= period_count)
			{
				//允许下行arp 报文进入cpu
				epon_request_onu_spec_pkt_state_set(context, 0, 0, CS_DOWN_STREAM, CS_PKT_ARP, DST_CPU);
				period_count = 0;
				
				//更新状态
				oper_status = 0;
			}			
		}
	}	
}
void arp_down_into_cpu_monitor(void)
{
	cs_uint32 timeout = ARP_MONITOR_INTERVAL_TIME;
	
	cs_circle_timer_add(timeout,arp_down_into_cpu_limit_proc ,(void *)timeout);
}
#endif


int cmd_onu_mgt_config_product_date(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int year, month, date;
        
    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,
                "<2007-2100>", "Year",
                 NULL);
        case 2:
            return cli_arg_help(cli, 0,
                "<1-12>", "Month",
                 NULL);
        case 3:
            return cli_arg_help(cli, 0,
                "<1-31>", "date",
                 NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(3 == argc)
    {   
		year = atoi(argv[0]);
		month = atoi(argv[1]);
		date = atoi(argv[2]);

		Onu_Sysinfo_Get();
		sprintf(onu_system_info_total.hw_manufature_date, 
			   	"%d-%02d-%02d", year, month, date);
        
		if (GWD_RETURN_OK != Onu_Sysinfo_Save())
		{
			cli_print(cli, "  System information save error!\r\n");
		}
    } else
    {
        cli_print(cli, "%% Invalid input.");
    }
    
    return CLI_OK;
}

int cmd_onu_mgt_config_product_sn(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int  len, i;
	unsigned char tmpStr[18];

        
    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,
                "<string>", "Manufacture serial number(length<16)",
                 NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(1 == argc)
    {   
		if((len = strlen(argv[0])) > 16)
		{
			cli_print(cli, "  The length of serial number must be less than %d.\r\n", 16);
			return CLI_OK;
		}

		for(i=0; i<len; i++)
			tmpStr[i] = TOUPPER(argv[0][i]);
		tmpStr[i] = '\0';
		
		Onu_Sysinfo_Get();
		sprintf(onu_system_info_total.serial_no, "%s", tmpStr);

		if (GWD_RETURN_OK != Onu_Sysinfo_Save())
		{
			cli_print(cli, "  System information save error!\r\n");
		}

		return CLI_OK;
    } else
    {
        cli_print(cli, "%% Invalid input.");
    }
    
    return CLI_OK;
}

#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
int cmd_onu_mgt_config_device_name(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int  len, i;
	unsigned char tmpStr[129] = {0};
	
        
    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,
                "<string>", "Device name(length<=126)",
                 NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(1 == argc)
    {   
		if((len = strlen(argv[0])) > 126)
		{
			cli_print(cli, "  The length of device name cannot be more than %d.\r\n", 126);
			return CLI_OK;
		}

		for(i=0; i<len; i++)
			tmpStr[i] = TOUPPER(argv[0][i]);
		tmpStr[i] = '\0';
		
		Onu_Sysinfo_Get();
		sprintf(onu_system_info_total.device_name, "%s", tmpStr);

		if (GWD_RETURN_OK != Onu_Sysinfo_Save())
		{
			cli_print(cli, "  System information save error!\r\n");
		}

		return CLI_OK;
    } 
	else
    {
        cli_print(cli, "%% Invalid input.");
    }
    
    return CLI_OK;
}
#endif

int cmd_onu_mgt_config_product_hw_version(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int v_major, v_rel;
        
    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,
                "<1-9>", "Major version",
                 NULL);
        case 2:
            return cli_arg_help(cli, 0,
                "<1-9>", "Release version",
                 NULL);
	//	case 3:
          //  return cli_arg_help(cli, 0,
            //    "<1-9>", "B version",
             //    NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(2 == argc)
    {   
		v_major = atoi(argv[0]);
		v_rel = atoi(argv[1]);
	//	V_B = atoi(argv[2]);

		Onu_Sysinfo_Get();
		sprintf(onu_system_info_total.hw_version, "V%d.%d", 
			v_major, v_rel);
        
		if (GWD_RETURN_OK != Onu_Sysinfo_Save())
		{
			cli_print(cli, "  System information save error!\r\n");
		}
    } else
    {
        cli_print(cli, "%% Invalid input.");
    }
    
    return CLI_OK;
}

int cmd_display_product(struct cli_def *cli, char *command, char *argv[], int argc)
{
#if (PRODUCT_CLASS == PRODUCTS_GT811D)
	char onu_type[] = "GT811_D";
#endif

#if (PRODUCT_CLASS == PRODUCTS_GT811G)
	char onu_type[] = "GT811_G";
#endif

#if (PRODUCT_CLASS == PRODUCTS_GT873_M_4F4S)
	char onu_type[] = "GT873M_4F4S";
#endif

	long lRet = GWD_RETURN_OK;
    char strMac[32];
	extern int cli_get_onu_mac_addr(char *mac);
	extern char const iros_version[];
	
    cli_get_onu_mac_addr(strMac);
        
	lRet = Onu_Sysinfo_Get();
	if (lRet != GWD_RETURN_OK)
	{
		cli_print(cli, "  Get product information from flash with error.\r\n");
	}
	else
	{
		cli_print(cli,  "\n  Product information as following--");
		cli_print(cli,  "    ONU type         : %s", onu_type);
		cli_print(cli,  "    DeiveName        : %s", onu_system_info_total.device_name);
		cli_print(cli,  "    Hardware version : %s", onu_system_info_total.hw_version);
		cli_print(cli,  "    Software version : %s", onu_system_info_total.sw_version);
		cli_print(cli,  "    Firmware version : %s", iros_version);
		cli_print(cli,  "    Bootload version : %s", irosbootver);
		cli_print(cli,  "    Manufature date  : %s", onu_system_info_total.hw_manufature_date);
		cli_print(cli,  "\n    Serial number    : %s", onu_system_info_total.serial_no);
    	cli_print(cli,  "    Onu mac address  : %s", strMac);
	}

	return CLI_OK;
}

int fdb_learn_set(int portid, int en)
{
	int ret = 0;
	cs_callback_context_t context;
	cs_sdl_fdb_mac_learn_t  status;

	switch(en)
	{
		case 0:
			status = SDL_FDB_MAC_LEARN_DISABLE;
			break;

		case 1:
			status = SDL_FDB_MAC_LEARN_ENABLE;
			break;

		default:
			ret = -1;
			goto end;
			break;
	}

	if(CS_E_OK == epon_request_onu_mac_learn_set(context, 0, 0, portid, status))
	{
		ret = 0;
	}
	else
	{
		ret = -1;
	}

end:
	return ret;
}

int fdb_learn_get(int portid, int *en)
{
	int ret = 0;
	cs_callback_context_t context;
	cs_sdl_fdb_mac_learn_t  status;

	if(NULL == en)
	{
		ret = -1;
		goto end;
	}
	
	if(CS_E_OK == epon_request_onu_mac_learn_get(context, 0, 0, portid, &status))
	{
		if(SDL_FDB_MAC_LEARN_ENABLE == status)
		{
			*en = 1;
		}
		else
		{
			*en = 0;
		}

		ret = 0;
	}
	else
	{
		ret = -1;
	}

end:
	return ret;
}

int cmd_oam_atu_learn(struct cli_def * cli, char *command, char *argv[], int argc)
{
	int ret = CLI_OK;
	
	int en = 0;
         cs_int32 port_array[UNI_PORT_MAX] = {0};
	cs_uint32 port_num = UNI_PORT_MAX;
         cs_uint8 i = 0;

	if(CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0, 
					"<portlist>", "Input one fe port number", NULL );
				break;
			case 2:
				return cli_arg_help(cli, 0,
					"{[1|0]}*1", "1 enable; 0 disable", NULL);
				break;

			default:
				return cli_arg_help(cli, argc > 1, NULL  );
				break;
		}
	}
    
	if(argc == 2 )
	{
	    en = atoi(argv[1]);
        
	    if(CS_E_OK == gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
	    {
	        for(i = 0;i<port_num;i++)
	        {
    			    if(CS_E_OK != fdb_learn_set(port_array[i],  en))
    				cli_print(cli, "atu learning set %s fail!\r\n", en?"enable":"disable");
                     }
                  }
	}
	else if(argc == 1)
	{
	    if(CS_E_OK == gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
	    {
	        for(i = 0;i<port_num;i++)
	        {
    			if(CS_E_OK != fdb_learn_get(port_array[i], &en))
    				cli_print(cli, "get port %d atu learning fail!\r\n", port_array[i]);
    			else
    				cli_print(cli,"Port %d source mac address learn is %s",port_array[i],en?"enable":"disable");
                 }
             }
	}
	else
	{
		cli_print(cli, "input error!");
		return CLI_ERROR;
	}
	return ret;
}


int fdb_aging_set(cs_uint32 timer)
{
	int ret = CS_E_OK;
    cs_callback_context_t context;

    cs_status  status = 0;

    status = epon_request_onu_fdb_age_set(context, 0, 0, timer);
    if(status != CS_E_OK)
	{
		ret = CS_E_ERROR;
    }
	else
	{
		ret = CS_E_OK;
	}

    return ret;
}

int fdb_aging_get(cs_uint32 *timer)
{
	int ret = CS_E_OK;
    cs_callback_context_t context;

    cs_status  status = 0;

    status = epon_request_onu_fdb_age_get(context, 0, 0, timer);
    if(status != CS_E_OK)
	{
		ret = CS_E_ERROR;
    }
	else
	{
		ret = CS_E_OK;
	}

    return ret;
}

int cmd_oam_atu_age(struct cli_def * cli, char *command, char *argv[], int argc)
{
	int ret = CS_E_OK;
	int age = 0;

	if(CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
					"<0-400>", "l2 age time unit sec, 0: disable aging", NULL);

			default:
				return cli_arg_help(cli, argc > 1, NULL  );
		}
	}

	if(argc == 1)
	{
		age = atoi(argv[0]);
		if(age < 0 ||age > 400)
			{
				cli_print(cli,"set aging time error \n");
				return CLI_ERROR;
			}
		if(CS_E_OK != fdb_aging_set(age))
			cli_print(cli, "atu age set %d fail!\r\n", age);
		else
			cli_print(cli,"atu aging time set %d sucess",age);
	}
	else
	{
		if(CS_E_OK != fdb_aging_get(&age))
			cli_print(cli, "get atu aging time fail!\r\n");
		else
			cli_print(cli, "Mac table aging time is %d seconds (PAS & BCM).\r\n", age);
	}
	
	return ret;
}

int cmd_atu_show(struct cli_def * cli, char *command, char *argv[], int argc)
{
	int ret = CLI_OK;
	cs_callback_context_t context;
	cs_uint16 idx = 0, next = 0;

	cs_sdl_fdb_entry_t entry;
	cs_uint8 c = 0;
	int count = 0;

	memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));

    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        default:
            return cli_arg_help(cli, 1, NULL);
        }
    }

	if(0 != argc )
	{
		return CLI_ERROR;
	}

    cli_print(cli, "================FDB SW table is shown:================");
    cli_print(cli, "index\t mac_address\t\t vid\t port\t type");

    while(epon_request_onu_fdb_entry_get_byindex(context, 0, 0, SDL_FDB_ENTRY_GET_MODE_ALL, idx, &entry, &next) == CS_OK)
    {
    	cs_uint16 vid = entry.vlan_id?entry.vlan_id:1;
    	idx = next;
		count++;
        cli_print(cli, "%-6d\t %02x:%02x:%02x:%02x:%02x:%02x\t %-6d\t %-2d\t %-2d", count,
            entry.mac.addr[0],
            entry.mac.addr[1],
            entry.mac.addr[2],
            entry.mac.addr[3],
            entry.mac.addr[4],
            entry.mac.addr[5],
            vid,
            entry.port,
            entry.type);
        if(++c > 1)
        {
        	cs_thread_delay(100);
        	c = 0;
        }
    }
    cli_print(cli, "=================Totally %d SW entries=================\n", count);

	return ret;
}


int cmd_atu_show_mac(struct cli_def * cli, char *command, char *argv[], int argc)
{
	int ret = CLI_OK;
	cs_callback_context_t context;
	cs_uint16 idx = 0, next = 0;
	cs_sdl_fdb_entry_t entry;
	cs_uint8 c = 0;
	int count = 0;	
	cs_mac_t mac;
			
	memset(&mac,0,sizeof(mac));
	memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));

    // deal with help
    if (CLI_HELP_REQUESTED) {
		switch (argc) {
		case 1:
			return cli_arg_help(cli, 0, "xxxx.xxxx.xxxx", "Please input the mac address",
					NULL );			
		default:
			return cli_arg_help(cli, 1, NULL );

		}
	}

	if(1 == argc)
	{
		mac.addr[0] = gwd_ascii_to_num(argv[0][0])*16 + gwd_ascii_to_num(argv[0][1]);
		mac.addr[1] = gwd_ascii_to_num(argv[0][2])*16 + gwd_ascii_to_num(argv[0][3]);
		mac.addr[2] = gwd_ascii_to_num(argv[0][5])*16 + gwd_ascii_to_num(argv[0][6]);
		mac.addr[3] = gwd_ascii_to_num(argv[0][7])*16 + gwd_ascii_to_num(argv[0][8]);
		mac.addr[4] = gwd_ascii_to_num(argv[0][10])*16 + gwd_ascii_to_num(argv[0][11]);
		mac.addr[5] = gwd_ascii_to_num(argv[0][12])*16 + gwd_ascii_to_num(argv[0][13]);

	}

    cli_print(cli, "================FDB SW table is shown:================");
    cli_print(cli, "index\t mac_address\t\t vid\t port\t type");

    while(epon_request_onu_fdb_entry_get_byindex(context, 0, 0, SDL_FDB_ENTRY_GET_MODE_ALL, idx, &entry, &next) == CS_OK)
    {
    	idx = next;
    	if( !memcmp(mac.addr,entry.mac.addr,CS_MACADDR_LEN))
		{
	    	cs_uint16 vid = entry.vlan_id?entry.vlan_id:1;
	    	count++;
	        cli_print(cli, "%-6d\t %02x:%02x:%02x:%02x:%02x:%02x\t %-6d\t %-2d\t %-2d", count,
	            entry.mac.addr[0],
	            entry.mac.addr[1],
	            entry.mac.addr[2],
	            entry.mac.addr[3],
	            entry.mac.addr[4],
	            entry.mac.addr[5],
	            vid,
	            entry.port,
	            entry.type);
	        if(++c > 1)
	        {
	        	cs_thread_delay(100);
	        	c = 0;
	        }
		}
    }
    cli_print(cli, "===========Totally %d SW entries of this mac===========\n", count);
	
	return ret;
}

int cmd_atu_show_static(struct cli_def * cli, char *command, char *argv[], int argc)
{
	int ret = CLI_OK;
	cs_callback_context_t context;
	cs_uint16 idx = 0, next = 0;

	cs_sdl_fdb_entry_t entry;
	cs_uint8 c = 0;
	int count = 0;

	memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));

    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        default:
            return cli_arg_help(cli, 1, NULL);
        }
    }

    cli_print(cli, "================FDB SW table is shown:================");
    cli_print(cli, "index\t mac_address\t\t vid\t port\t type");

    while(epon_request_onu_fdb_entry_get_byindex(context, 0, 0, SDL_FDB_ENTRY_GET_MODE_ALL, idx, &entry, &next) == CS_OK)
    {
    	idx = next;
		if(SDL_FDB_ENTRY_STATIC == entry.type)
		{
			cs_uint16 vid = entry.vlan_id?entry.vlan_id:1;
			
			count++;
	        cli_print(cli, "%-6d\t %02x:%02x:%02x:%02x:%02x:%02x\t %-6d\t %-2d\t %-2d", count,
	            entry.mac.addr[0],
	            entry.mac.addr[1],
	            entry.mac.addr[2],
	            entry.mac.addr[3],
	            entry.mac.addr[4],
	            entry.mac.addr[5],
	            vid,
	            entry.port,
	            entry.type);
	        if(++c > 1)
	        {
	        	cs_thread_delay(100);
	        	c = 0;
	        }
		}
    }
    cli_print(cli, "=================Totally %d Static SW entries=================\n", count);

	return ret;
}


int cmd_atu_show_counter(struct cli_def * cli, char *command, char *argv[], int argc)
{
	int ret = CLI_OK;
	cs_callback_context_t context;
	cs_uint16 idx = 0, next = 0;
	cs_sdl_fdb_entry_t entry;
	int count = 0;

	memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));

    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        default:
            return cli_arg_help(cli, 1, NULL);
        }
    }
 
    while(epon_request_onu_fdb_entry_get_byindex(context, 0, 0, SDL_FDB_ENTRY_GET_MODE_ALL, idx, &entry, &next) == CS_OK)
    {
    	idx = next;
		count++;
    }
    cli_print(cli, "=================Totally %d SW entries=================\n", count);

	return ret;
}

int cmd_atu_show_port(struct cli_def * cli, char *command, char *argv[], int argc)
{
	int ret = CLI_OK;
	cs_callback_context_t context;
	cs_uint16 idx = 0, next = 0;
	cs_sdl_fdb_entry_t entry;
	cs_uint8 c = 0;
	int count = 0;	
	cs_int32 port_array[UNI_PORT_MAX] = {0};
	cs_uint32 port_num = UNI_PORT_MAX;
	
	memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));

	// deal with help
	if (CLI_HELP_REQUESTED) {
		switch (argc) {
		case 1:
			return cli_arg_help(cli, 0, "<portlist>", "Please input the portlist,4 ports most, range 1-4",
					NULL ); 		
		default:
			return cli_arg_help(cli, 1, NULL );

		}
	}
	
	if(1 == argc)
	{	
		if(CS_E_OK == gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
		{
			cli_print(cli, "================FDB SW table is shown:================");
			cli_print(cli, "index\t mac_address\t\t vid\t port\t type");

			while(epon_request_onu_fdb_entry_get_byindex(context, 0, 0, SDL_FDB_ENTRY_GET_MODE_ALL, idx, &entry, &next) == CS_OK)
			{
				idx = next;
				if( NULL == port_array || CS_E_OK == gwd_num_in_array(entry.port,port_array,port_num))
				{
					cs_uint16 vid = entry.vlan_id?entry.vlan_id:1;
					count++;
					cli_print(cli, "%-6d\t %02x:%02x:%02x:%02x:%02x:%02x\t %-6d\t %-2d\t %-2d", count,
						entry.mac.addr[0],
						entry.mac.addr[1],
						entry.mac.addr[2],
						entry.mac.addr[3],
						entry.mac.addr[4],
						entry.mac.addr[5],
						vid,
						entry.port,
						entry.type);
					if(++c > 1)
					{
						cs_thread_delay(100);
						c = 0;
					}
				}
			}	
			cli_print(cli, "=================Totally %d SW entries=================\n", count);
		}
		else
		{
			return CS_E_ERROR;
		}
	}
	else
	{
		cli_print(cli,"wrong params");
		return CLI_ERROR_ARG;
	}

	return ret;
}

#define GW_CMD_MAC_LIMIT_MAX 64
#define GW_CMD_MAC_LIMIT_LAS 1
#define GW_CMD_MAC_LIMIT_NO  0

int cmd_atu_limit_port(struct cli_def * cli, char *command, char *argv[], int argc)
{
	cs_callback_context_t context;
	cs_int32 port_array[UNI_PORT_MAX] = {0};
	cs_uint32 port_num = UNI_PORT_MAX;
	cs_uint8 i = 0;
	cs_port_id_t port = 0;
	cs_uint32 mac_num = 0;
	
	// deal with help
	if (CLI_HELP_REQUESTED) {
		switch (argc) {
		case 1:
			return cli_arg_help(cli, 0, "<portlist>", "Please input the portlist,4 ports most, range 1-4",
					NULL );
		case 2:
			return cli_arg_help(cli, 0, "<0-64>", "limit number,0:no limit",
					NULL );
		default:
			return cli_arg_help(cli, 1, NULL );

		}
	}

	if(CS_E_OK != gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
	{
		cli_print(cli,"operation error!");
		return CLI_ERROR;
	}
	
	if(1 == argc)
	{	
		for(i = 0;i < port_num;i++)
		{
			port = port_array[i];
			
			if(CS_E_OK == epon_request_onu_fdb_mac_limit_get(context,0,0,port,&mac_num))
			{
				if(__FDB_NO_MAC_LIMIT == mac_num)
				{
					cli_print(cli,"port %d mac limit : no limit",port);
				}
				else
				{
					cli_print(cli,"port %d mac limit : %d",port,mac_num);
				}
			}
		}	
	}
	else if(2 == argc)
	{
		mac_num = atoi(argv[1]);
		
		if(!((GW_CMD_MAC_LIMIT_LAS <= mac_num && mac_num <= GW_CMD_MAC_LIMIT_MAX) || (GW_CMD_MAC_LIMIT_NO == mac_num)))
		{
			cli_print(cli,"limit number out of range!");
			return CLI_ERROR;
		}

		mac_num = (mac_num == GW_CMD_MAC_LIMIT_NO ? __FDB_NO_MAC_LIMIT : mac_num);
		
		for(i = 0;i < port_num;i++)
		{
			port = port_array[i];
			
			if(CS_E_OK != epon_request_onu_fdb_mac_limit_set(context,0,0,port,mac_num))
			{
				cli_print(cli,"set port %d mac limit error",port);
			}
		}
	}
	else
	{
		cli_print(cli,"wrong params");
		return CLI_ERROR_ARG;
	}

	return CLI_OK;
}

int cmd_atu_limit_global(struct cli_def * cli, char *command, char *argv[], int argc)
{
		cs_callback_context_t context;
		cs_uint32 port_num = UNI_PORT_MAX;
		cs_port_id_t port = 0;
		cs_uint32 mac_num = 0;
		
		// deal with help
		if (CLI_HELP_REQUESTED) {
			switch (argc) {
				case 1:
					return cli_arg_help(cli, 0, "<0-64>", "limit number,0:no limit",
							NULL );
				default:
					return cli_arg_help(cli, 1, NULL );
			}
		}
		if(0 == argc)
		{	
			for(port= 1;port <= port_num;port++)
			{
	
				
				if(CS_E_OK == epon_request_onu_fdb_mac_limit_get(context,0,0,port,&mac_num))
				{
					if(__FDB_NO_MAC_LIMIT == mac_num)
					{
						cli_print(cli,"port %d mac limit : no limit",port);
					}
					else
					{
						cli_print(cli,"port %d mac limit : %d",port,mac_num);
					}
				}
			}	
		}
		else if(1 == argc)
		{
			mac_num = atoi(argv[0]);
			
			if(!((GW_CMD_MAC_LIMIT_LAS <= mac_num && mac_num <= GW_CMD_MAC_LIMIT_MAX) || (GW_CMD_MAC_LIMIT_NO == mac_num)))
			{
				cli_print(cli,"limit number out of range!");
				return CLI_ERROR;
			}
	
			mac_num = (mac_num == GW_CMD_MAC_LIMIT_NO ? __FDB_NO_MAC_LIMIT : mac_num);
			
			for(port = 1;port <= port_num;port++)
			{
				
				if(CS_E_OK != epon_request_onu_fdb_mac_limit_set(context,0,0,port,mac_num))
				{
					cli_print(cli,"set port %d mac limit error",port);
				}
			}
		}
		else
		{
			cli_print(cli,"wrong params");
			return CLI_ERROR_ARG;
		}
	
		return CLI_OK;
}
#ifdef HAVE_PLATFORM_UNIFY_CMD
void port_link_show(struct cli_def * cli,cs_port_id_t portid)
{
	cs_status                    status = 0;
    cs_sdl_port_link_status_t    link_status;
	cs_callback_context_t        context;
	
	memset(&link_status,0,sizeof(link_status));
	
	status = epon_request_onu_port_link_status_get(context, 0, 0, portid, &link_status);
    if(status != CS_E_OK)
	{
        cli_print(cli, "epon_request_onu_port_link_status_get failed \n");       
    }
	else
	{
		cli_print(cli, "port:%d link %s",portid,(link_status == SDL_PORT_ADMIN_UP) ? "up" : "down");
	}
	
}
int cmd_port_link_show(struct cli_def * cli, char *command, char *argv[], int argc)
{
	int ret = CLI_OK;
	cs_port_id_t portid = 0;
	cs_uint32 port_num = UNI_PORT_MAX;
		

	// deal with help
	if (CLI_HELP_REQUESTED) {
		switch (argc) {
		case 1:
			return cli_arg_help(cli, 0, "<portlist>", "Please input the portlist,4 ports most, range 1-4",
					NULL ); 		
		default:
			return cli_arg_help(cli, 1, NULL );

		}
	}

	if(0 == argc)
	{		   
		for(portid=1;portid<=port_num;portid++)
		{
			port_link_show(cli,portid);
		}
	}
	else if(1 == argc)
	{	
		cs_int32 port_array[UNI_PORT_MAX] = {0};
		cs_uint8   i = 0;

		if(CS_E_OK == gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
		{
			for(i=0;i < port_num;i++)
			{
				port_link_show(cli,port_array[i]);
			}
		}
	} 
	else
	{
		cli_print(cli, "%% Invalid input.");
	}
	
	return ret;
}


void port_admin_show(struct cli_def * cli,cs_port_id_t portid)
{
	cs_status                    status = 0;
	cs_callback_context_t        context;
	cs_sdl_port_admin_t          admin=0;
		
	status = epon_request_onu_port_admin_get ( context,	ONU_DEVICEID_FOR_API,  ONU_LLIDPORT_FOR_API ,  portid, &admin);
    if(status != CS_E_OK)
	{
        cli_print(cli, "epon_request_onu_port_admin_get failed \n");       
    }
	else
	{
		cli_print(cli, "port %d: %s ", portid, admin==SDL_PORT_ADMIN_DOWN? "admin down":"admin up");
	}
}

void port_admin_set(struct cli_def * cli,cs_port_id_t portid,cs_uint32 phy_admin)
{
	cs_status                    status = 0;

	if ( phy_admin == 0) 
	{
        phy_admin=OAM_INTF_PHY_ADMIN_DIS;
    } 
	else 
    {
        phy_admin=OAM_INTF_PHY_ADMIN_EN;
    }    

    status = ctc_oam_eth_admin_state_set_adapt ( portid,  phy_admin);

	if(status != CS_E_OK)
	{
        cli_print(cli, "ctc_oam_eth_admin_state_set_adapt failed \n");       
    }
}

int cmd_port_admin(struct cli_def * cli, char *command, char *argv[], int argc)
{
	int ret = CLI_OK;
	
	cs_port_id_t portid = 0;
	cs_uint32 port_num = UNI_PORT_MAX;

	// deal with help
	if (CLI_HELP_REQUESTED) {
		switch (argc) {
		case 1:
			return cli_arg_help(cli, 0, "<portlist>", "Please input the portlist,4 ports most, range 1-4",
					NULL ); 	
		case 2:
			return cli_arg_help(cli, 0, "<1:admin up | 0:admin down>",
					NULL ); 
		default:
			return cli_arg_help(cli, 1, NULL );
		}
	}

	if(0 == argc)
	{		   
		for(portid=1;portid<=port_num;portid++)
		{
			port_admin_show(cli,portid);
		}
	}
	else 
	{	
		cs_int32 port_array[UNI_PORT_MAX] = {0};
		cs_uint8   i = 0;

		if(CS_E_OK != gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
		{
			return CLI_ERROR_ARG;
		}
		
		if(1 == argc)
		{	
			for(i=0;i < port_num;i++)
			{
				port_admin_show(cli,port_array[i]);
			}
		}
		else if(2 == argc)
		{	
			cs_uint32           phy_admin=0;

			phy_admin = atoi(argv[1]);
			
			for(i=0;i < port_num;i++)
			{
				port_admin_set(cli,port_array[i],phy_admin);
			}
		}
		else
		{
			cli_print(cli, "%% Invalid input.");
		}	
	} 
	
	return ret;
}
#endif /* (#ifdef HAVE_PLATFORM_UNIFY_CMD) */

#if 0
#define __VLAN_MAX                      /*(8*UNI_PORT_MAX+4)*/ 64
#else
//modified by zhuxh
#define __VLAN_MAX                      (9*UNI_PORT_MAX)	/*一个端口有8个tag vlan加上一个untag vlan(default vlan)*/
#endif

int cmd_atu_show_vlan(struct cli_def * cli, char *command, char *argv[], int argc)
{
	int ret = CLI_OK;
	cs_callback_context_t context;
	cs_uint16 idx = 0, next = 0;
	cs_sdl_fdb_entry_t entry;
	cs_uint8 c = 0;
	int count = 0;	
	cs_int32 vlan_array[__VLAN_MAX] = {0};
	cs_int32 vlan_num = __VLAN_MAX;
			
	memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));

	// deal with help
	if (CLI_HELP_REQUESTED) {
		switch (argc) {
		case 1:
			return cli_arg_help(cli, 0, "<vlanlist>", "Please input the vlanlist,36 vlan most, range 1-4094",
					NULL ); 		
		default:
			return cli_arg_help(cli, 1, NULL );

		}
	}
	
	if(0 == argc)
	{
		// do nothing
	}
	else if(1 == argc)
	{
		if(CS_E_OK != gwd_parse_num_list_from_str(argv[0],vlan_array, &vlan_num))
		{
			return CS_E_ERROR;
		}
	}
	else
	{
		return CLI_ERROR_ARG;
	}

	cli_print(cli, "================FDB SW table is shown:================");
	cli_print(cli, "index\t mac_address\t\t vid\t port\t type");

	while(epon_request_onu_fdb_entry_get_byindex(context, 0, 0, SDL_FDB_ENTRY_GET_MODE_ALL, idx, &entry, &next) == CS_OK)
	{
		cs_uint16 vid = entry.vlan_id?entry.vlan_id:1;

		idx = next;
		if( NULL == vlan_array || CS_E_OK == gwd_num_in_array(vid,vlan_array,vlan_num))
		{
			count++;
			cli_print(cli, "%-6d\t %02x:%02x:%02x:%02x:%02x:%02x\t %-6d\t %-2d\t %-2d", count,
				entry.mac.addr[0],
				entry.mac.addr[1],
				entry.mac.addr[2],
				entry.mac.addr[3],
				entry.mac.addr[4],
				entry.mac.addr[5],
				vid,
				entry.port,
				entry.type);
			if(++c > 1)
			{
				cs_thread_delay(100);
				c = 0;
			}
		}
	}	
	cli_print(cli, "=================Totally %d SW entries=================\n", count);
		
	return ret;
}




#define GW_VLAN_MAX 4094
#define GW_VLAN_LAS 1

#define GW_ONUPORT_LAS 1

#define GW_PORT_PRI_MAX 7
#define GW_PORT_PRI_LAS 0

int cmd_static_mac_add_fdb(struct cli_def *cli, char *command, char *argv[], int argc)
{
	cs_uint32 gw_port;
	cs_uint16 gw_vlan;
	cs_uint32 gw_pri;
	if (CLI_HELP_REQUESTED) {
		switch (argc) {
		case 1:
			return cli_arg_help(cli, 0, "xxxx.xxxx.xxxx", "Please input the mac address",
					NULL );
		case 2:
		    return cli_arg_help(cli, 0, "<port_list>", "Please input the port_list",
					NULL );
		case 3:
			return cli_arg_help(cli, 0, "<1-4094>", "Please input vlan id",
					NULL );
		case 4:
			return cli_arg_help(cli, 0, "<0-7>", "MAC address's priority",
					NULL );			
		default:
			return cli_arg_help(cli, argc > 3, NULL );

		}
	}

	if(argc == 4)
	{
		gw_port = iros_strtol(argv[1]);
		if(gw_port > UNI_PORT_MAX || gw_port < 1)
		{
			cli_print(cli,"port error\n");
			return -1;
		}
		gw_vlan = iros_strtol(argv[2]);
		if(gw_vlan >GW_VLAN_MAX ||gw_vlan < GW_VLAN_LAS)
		{
			cli_print(cli,"vlan error\n");
			return -1;
		}	
		gw_pri = iros_strtol(argv[3]);
		if(gw_pri < GW_PORT_PRI_LAS || gw_pri > GW_PORT_PRI_MAX)
		{
			cli_print(cli,"pri error\n");
		}
		char mac[13] = {0};
		strncpy(&mac[0], &argv[0][0], 4);
		strncpy(&mac[4], &argv[0][5], 4);
		strncpy(&mac[8], &argv[0][10], 4);
		mac[12] = '\0';
		if(fdb_static_list_add(mac,gw_port,gw_vlan) != CS_E_OK)
		{
			cli_print(cli,"add static mac fail\n");
		}
		else
		{
			cli_print(cli,"add port %d static mac success\n",gw_port);
		}
	}
	else
	{
		cli_print(cli,"%%input error\n");
	}
	return CLI_OK;
}

int cmd_static_mac_del_fdb(struct cli_def *cli, char *command, char *argv[], int argc)
{
	cs_uint16 gw_vlan;
	if (CLI_HELP_REQUESTED) 
	{
		switch (argc) 
		{
			case 1:
				return cli_arg_help(cli, 0, "xxxx.xxxx.xxxx", "Please input the mac address",
						NULL );
			case 2:
				return cli_arg_help(cli, 0, "<1-4094>", "Please input vlan id",
						NULL );
			default:
				return cli_arg_help(cli, argc > 1, NULL );
		}

	}
	if(argc == 2)
	{
		gw_vlan = iros_strtol(argv[1]);
		if(gw_vlan >GW_VLAN_MAX ||gw_vlan < GW_VLAN_LAS)
		{
			cli_print(cli,"vlan error\n");
			return -1;
		}
		char mac[13] = {0};
		strncpy(&mac[0], &argv[0][0], 4);
		strncpy(&mac[4], &argv[0][5], 4);
		strncpy(&mac[8], &argv[0][10], 4);
		mac[12] = '\0';
		if(fdb_static_list_del(mac,gw_vlan) != CS_E_OK)
		{
			cli_print(cli,"del static mac fail\n");
		}
		else
		{
			cli_print(cli,"del static mac success\n");
		}
	}
	else
	{
		cli_print(cli,"%%input error\n");
	}		
	return CLI_OK;
}

#ifdef HAVE_IP_STACK
int cmd_ifconfig_eth0(struct cli_def *cli, char *command, char *argv[], int argc)
{
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<cr>", "show ip configure",
				"<IP address>", "ip configure",
				"save", "save config to flash",
				NULL);
				break;
			case 2:
				return cli_arg_help(cli, 0,
				"<Subnet Mask>", "ip configure",
				NULL);
				break;
			case 3:
				return cli_arg_help(cli, 0,
				"<GateWay>", "ip configure",
				NULL);
				break;
			case 4:
				return cli_arg_help(cli, 0,
				"<cr>", "finish ip configure",
				NULL);
				break;
			default:
			return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	if(0 == argc)
	{
		cs_status ret = CS_E_OK;
		cs_uint32	device_ip = 0;
		cs_uint32	device_mask = 0;
		cs_uint32	device_gateway = 0;
		struct sockaddr_in addrp[3];
		
		ret = epon_request_onu_ip_config_get(&device_ip, &device_mask, &device_gateway);
		if(CS_E_OK == ret)
		{
			addrp[0].sin_addr.s_addr = htonl(device_ip);
			addrp[1].sin_addr.s_addr = htonl(device_mask); 
			addrp[2].sin_addr.s_addr = htonl(device_gateway);
			
			cli_print(cli, "ip from network card:");		
			cli_print(cli, "=========================================================================");
			cli_print(cli, "IP - %s", inet_ntoa(addrp[0].sin_addr));
			cli_print(cli, "IP Submask - %s", inet_ntoa(addrp[1].sin_addr));
			cli_print(cli, "IP Gateway - %s", inet_ntoa(addrp[2].sin_addr));
			cli_print(cli, "=========================================================================");
		}
		else
		{
			cli_print(cli, "ip config fail");
		}
		
	}
	else if(3 == argc)
	{
		cs_status ret = CS_E_OK;
		cs_uint32	device_ip;
		cs_uint32	device_mask;
		cs_uint32	device_gateway;

		device_ip = ntohl(inet_addr(argv[0]));
		device_mask = ntohl(inet_addr(argv[1]));
		device_gateway = ntohl(inet_addr(argv[2]));

		ret = epon_request_onu_ip_config_set(device_ip, device_mask, device_gateway);
		if(CS_E_OK == ret)
		{
			cli_print(cli, "ip config success");
		}
		else
		{
			cli_print(cli, "ip config failed");
		}
	}
	else
	{
		cli_print(cli, "%% Invalid input");
	}
	
	return CLI_OK;
}

#ifdef HAVE_TCP_UDP_NETWORK_CARD
//added by zhuxh
int cmd_ifconfig_eth1(struct cli_def *cli, char *command, char *argv[], int argc)
{
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<cr>", "show ip configure",
				"<IP address>", "ip configure",
				"save", "save config to flash",
				NULL);
				break;
			case 2:
				return cli_arg_help(cli, 0,
				"<Subnet Mask>", "ip configure",
				NULL);
				break;
			case 3:
				return cli_arg_help(cli, 0,
				"<GateWay>", "ip configure",
				NULL);
				break;
			case 4:
				return cli_arg_help(cli, 0,
				"<cr>", "finish ip configure",
				NULL);
				break;
			default:
			return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	if(0 == argc)
	{
		cs_status ret = CS_E_OK;
		cs_uint32	device_ip = 0;
		cs_uint32	device_mask = 0;
		cs_uint32	device_gateway = 0;
		struct sockaddr_in addrp[3];
		
		ret = epon_request_onu_eth1_ip_config_get(&device_ip, &device_mask, &device_gateway);
		if(CS_E_OK == ret)
		{
			addrp[0].sin_addr.s_addr = htonl(device_ip);
			addrp[1].sin_addr.s_addr = htonl(device_mask); 
			addrp[2].sin_addr.s_addr = htonl(device_gateway);
			
			cli_print(cli, "ip from network card:");		
			cli_print(cli, "=========================================================================");
			cli_print(cli, "IP - %s", inet_ntoa(addrp[0].sin_addr));
			cli_print(cli, "IP Submask - %s", inet_ntoa(addrp[1].sin_addr));
			cli_print(cli, "IP Gateway - %s", inet_ntoa(addrp[2].sin_addr));
			cli_print(cli, "=========================================================================");
		}
		else
		{
			cli_print(cli, "ip config fail");
		}
		
	}
	else if(3 == argc)
	{
		cs_status ret = CS_E_OK;
		cs_uint32	device_ip;
		cs_uint32	device_mask;
		cs_uint32	device_gateway;

		device_ip = ntohl(inet_addr(argv[0]));
		device_mask = ntohl(inet_addr(argv[1]));
		device_gateway = ntohl(inet_addr(argv[2]));

		ret = epon_request_onu_eth1_ip_config_set(device_ip, device_mask, device_gateway);
		if(CS_E_OK == ret)
		{
			cli_print(cli, "ip config success");
		}
		else
		{
			cli_print(cli, "ip config failed");
		}
	}
	else
	{
		cli_print(cli, "%% Invalid input");
	}
	
	return CLI_OK;
}
#endif	/*(#ifdef HAVE_TCP_UDP_NETWORK_CARD)*/

#endif	/*(#ifdef HAVE_IP_STACK)*/

#ifdef HAVE_GWD_FLASH_OPER
int cmd_save(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int ret = 0;
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<cr>", "save gwd config to flash",
				NULL);
				break;

			default:
			return cli_arg_help(cli, argc > 1, NULL);
			break;
		}
	}

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }
	
	ret = save_user_tlv_data_to_flash();
	if(0 == ret)
	{
		cs_printf("save config to flash success\n");
	}
	else
	{
		cs_printf("save config to flash failed\n");
	}
	
    return CLI_OK;
}

int start_up_show(void)
{
	int ret = 0;

	ret = get_user_tlv_data_from_flash(DATA_SHOW);

	return ret;
}
int cmd_show_start_up(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int ret = 0;
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }
	
	ret = start_up_show();
	if(0 == ret)
	{
		
	}
	else
	{
		cli_print(cli, "show start-up failed\n");
	}
	
    return CLI_OK;
}
int cmd_show_running_config(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }
	
	running_config_show();
	
    return CLI_OK;
}

int cmd_erase_config(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int ret = 0;
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }
	
	ret = start_up_config_erase();
	if(0 == ret)
	{
		cs_printf("erase config success\n");
	}
	else
	{
		cs_printf("erase config failed\n");
	}
	
    return CLI_OK;
}
#endif	/*(#ifdef HAVE_GWD_FLASH_OPER)*/

extern int igmp_mode_show(mc_mode_t mc_mode)
{
	int ret = 0;
	char mode[15] = {0};
	switch(mc_mode) 
	{
        case MC_SNOOPING:
            strncpy(mode, "snooping", sizeof(mode));
            break;

        case MC_MANUAL:
            strncpy(mode, "auth", sizeof(mode));
            break;

		case MC_PROXY:
            strncpy(mode, "proxy", sizeof(mode));
            break;

		case MC_DISABLE:
            strncpy(mode, "transparent", sizeof(mode));
            break;

        default:
            strncpy(mode, "unknown", sizeof(mode));
            break;
	}
	cs_printf("igmp %s mode\n", mode);
	
	return ret;
}

#ifdef HAVE_IGMP_MODE_RESTORE
#define IGMP_MODE_DEFAULT	MC_SNOOPING

extern int igmp_mode_config_recover(mc_mode_t mc_mode)
{
	int ret = 0;
	ret = mc_mode_set(0, mc_mode);
	if(CS_E_OK == ret)
	{
		cs_printf("mc_mode_set success\n");
	}
	else
	{
		cs_printf("mc_mode_set failed\n");
	}
	return ret;
}

extern int igmp_mode_tlv_infor_get(int *len, char **value, int *free_need)
{
	int ret = 0;
	mc_mode_t *mc_mode = NULL;
	cs_status status = CS_E_OK;
	if(NULL == len)
	{
		goto error;
	}
	else
	{
		*len = 0;
	}

	if(NULL == value)
	{
		goto error;
		
	}
	else
	{
		*value = NULL;
	}
	
	if(NULL == free_need)
	{
		goto error;
	}
	else
	{
		*free_need = 0;
	}

	mc_mode = (mc_mode_t *)iros_malloc(IROS_MID_APP, sizeof(mc_mode_t));
	memset(mc_mode, 0, sizeof(mc_mode_t));
	*free_need = 1;
	status = mc_mode_get(0, mc_mode);
	if(CS_E_OK == status)
	{
		if(IGMP_MODE_DEFAULT == *mc_mode)
		{
			goto end;
		}
		else
		{
			*len = sizeof(mc_mode_t);
			*value = (char *)mc_mode;
		}
	}
	else
	{
		cs_printf("mc_mode_get failed\n");
		goto error;
	}
	
	ret = 0;
	goto end;
	
error:
	ret = -1;
	
end:
	if((0 == *len)&&(NULL != mc_mode))
	{
		iros_free(mc_mode);
		mc_mode = NULL;
	}
	return ret;
}

extern int igmp_mode_tlv_infor_handle(int len, char *data, int opcode)
{
	int ret = 0;
	mc_mode_t mc_mode;
	
	if(0 != len)
	{
		//do nothing
	}
	else
	{
		goto error;
	}
	
	if(NULL != data)
	{
		//do nothing
	}
	else
	{
		goto error;
	}
	memcpy(&mc_mode, data, len);
	
	if(DATA_RECOVER == opcode)
	{
		igmp_mode_config_recover(mc_mode);
	}
	else if(DATA_SHOW == opcode)
	{
		igmp_mode_show(mc_mode);
	}
	else
	{
		cs_printf("in %s\n", __func__);
	}
	ret = 0;
	goto end;
	
error:
	ret = -1;
	
end:
	return ret;
}

extern int igmp_mode_running_config_show(void)
{
	int ret = 0;
	mc_mode_t mc_mode;
	ret = mc_mode_get(0, &mc_mode);
	if(CS_E_OK == ret)
	{
		if(IGMP_MODE_DEFAULT != mc_mode)
		{
			igmp_mode_show(mc_mode);
		}
	}
	else
	{
		cs_printf("mc_mode_get failed\n");
	}
	
	return ret;
}
#endif	/*(#ifdef HAVE_IGMP_MODE_RESTORE)*/

#ifdef HAVE_GWD_IGMP_CMD
extern int cmd_igmp_mode(struct cli_def *cli, char *command, char *argv[], int argc)
{
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				cli_arg_help(cli, 0,
				"<cr>", "just enter to execuse command. (show igmp mode)",
				NULL);
				cli_arg_help(cli, 0,
				"0", "set igmp snooping mode",
				NULL);
				cli_arg_help(cli, 0,
				"1", "set igmp auth mode",
				NULL);
				cli_arg_help(cli, 0,
				"2", "set igmp transparent mode",
				NULL);
				return CLI_OK;
			case 2:
				return cli_arg_help(cli, 0,
				"<cr>", "just enter to execuse command. (show igmp mode)",
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
	else
	{
		//do nothing
	}

	if(0 == argc)
	{
		cs_status ret = CS_E_OK;
		mc_mode_t mc_mode;
    	ret = mc_mode_get(0, &mc_mode);
		if(CS_E_OK == ret)
		{
			igmp_mode_show(mc_mode);
		}
		else
		{
			cs_printf("mc_mode_get failed\n");
		}
	}
	else if(1 == argc)
	{
		cs_status ret = CS_E_OK;
		int mode = 0;
		mc_mode_t mc_mode;
		mode = iros_strtol(argv[0]);
		switch(mode)
		{
			case 0:
				mc_mode = MC_SNOOPING;
				break;
			case 1:
				mc_mode = MC_MANUAL;
				break;
			case 2:
				mc_mode = MC_DISABLE;
				break;
			default:
				mc_mode = MC_SNOOPING;
				break;
		}
		
		ret = mc_mode_set(0, mc_mode);
		if(CS_E_OK == ret)
		{
			cs_printf("igmp mode set success\n");
		}
		else
		{
			cs_printf("igmp mode set failed\n");
		}
	}
	else
	{
		//do nothing
	}

	return CLI_OK;
}
#endif	/*(#ifdef HAVE_GWD_FLASH_OPER)*/

#ifdef HAVE_QINQ
#define VLAN_MIN	2
#define VLAN_MAX	4094
#define ADD		1
#define DEL		0
#define QINQ_NUM_MAX	16

extern qinq_list_t *pVlan_qinq_list_running;
static int vlan_pas_rule_arg_help(struct cli_def *cli, char *argv[], int argc)
{
	switch(argc)
	{
		case 1:
			cli_arg_help(cli, 0,
			"vlan-id", "Based on vlan id",
			NULL);
			cli_arg_help(cli, 0,
			"clear", "clear table",
			NULL);
			cli_arg_help(cli, 0,
			"<cr>", "display vlan table",
			NULL);
			break;
		case 2:
			cli_arg_help(cli, 0,
			"<vid>", "Vlan id (eg. 100,200 (NOT in HEX))",
			NULL);
			cli_arg_help(cli, 0,
			"<cr>", "clear table",
			NULL);
			break;
		case 3:
			cli_arg_help(cli, 0,
			"down", "Downstream",
			NULL);
			cli_arg_help(cli, 0,
			"up", "Upstream",
			NULL);
			break;
		case 4:
			cli_arg_help(cli, 0,
			"exchange", "For vlan translation and force vlan priority",
			NULL);
			cli_arg_help(cli, 0,
			"attach", "For vlan stacking",
			NULL);
			break;
		case 5:
			cli_arg_help(cli, 0,
			"<2-4094>", "New Vlan id",
			NULL);
			break;
		case 6:
			cli_arg_help(cli, 0,
			"8100", "Vlan type field in tag = 0x8100",
			NULL);
			break;
		case 7:
			cli_arg_help(cli, 0,
			"original", "Priority in new vlan tag copied from original tag",
			NULL);
			break;
		case 8:
			cli_arg_help(cli, 0,
			"0", "Disable",
			NULL);
			cli_arg_help(cli, 0,
			"1", "Enable",
			NULL);
			break;
		default:
			cli_arg_help(cli, argc > 1, NULL);
			break;
	}
	
	return CLI_OK;
}


static int vlan_pas_rule_arg_check(char *argv[], int argc)
{
	int ret = 0;
	int vlan_old = 0;
	int vlan_new = 0;

	char *str_rule_opt = NULL;
	char *str_vlan_old = NULL;
	char *str_vlan_new = NULL;
	char *str_direction = NULL;
	char *str_vlan_oper = NULL;
	char *str_tpid = NULL;
	char *str_priority = NULL;
	char *str_enable = NULL;
	switch(argc)
	{
		case 8:
			str_enable = argv[7];
			if(0 == strcmp("0", str_enable))
			{
				//do nothing
			}
			else if(0 == strcmp("1", str_enable))
			{
				//do nothing
			}
			else
			{
				ret = CLI_ERROR;
				break;
			}
		case 7:
			str_priority = argv[6];
			if(0 == strcmp("original", str_priority))
			{
				//do nothing
			}
			else
			{
				ret = CLI_ERROR;
				break;
			}
			
		case 6:
			str_tpid = argv[5];
			if(0 == strcmp("8100", str_tpid))
			{
				//do nothing
			}
			else
			{
				ret = CLI_ERROR;
				break;
			}
		case 5:
			str_vlan_new = argv[4];
			vlan_new = atoi(str_vlan_new);
			if((vlan_new>=VLAN_MIN)&&(vlan_new<=VLAN_MAX))
			{
				//do nothing
			}
			else
			{
				ret = CLI_ERROR;
				break;
			}
		case 4:
			str_vlan_oper = argv[3];
			if(0 == strcmp("exchange", str_vlan_oper))
			{
				//do nothing
			}
			else if(0 == strcmp("attach", str_vlan_oper))
			{
				
			}
			else
			{
				ret = CLI_ERROR;
				break;
			}
		case 3:
			str_direction = argv[2];
			if(0 == strcmp("up", str_direction))
			{
				//do nothing
			}
			else if(0 == strcmp("down", argv[2]))
			{
				//do nothing
			}
			else
			{
				ret = CLI_ERROR;
				break;
			}
		case 2:
			str_vlan_old = argv[1];
			vlan_old = atoi(str_vlan_old);
			if((vlan_old>=VLAN_MIN)&&(vlan_old<=VLAN_MAX))
			{
				//do nothing
			}
			else
			{
				ret = CLI_ERROR;
				break;
			}
		case 1:
			str_rule_opt = argv[0];
			if(0 == strcmp("vlan-id", str_rule_opt))
			{
				//do nothing
			}
			else if(0 == strcmp("clear", str_rule_opt))
			{
				//do nothing
			}
			else
			{
				ret = CLI_ERROR;
				break;
			}
		case 0:
			ret = CLI_OK;
			break;
		default:
			ret = CLI_ERROR;
			break;
	}
	
	return ret;
}

int vlan_pas_rule_dump_proc(void)
{
	vlan_qinq_sw_table_dump(pVlan_qinq_list_running);
	return CLI_OK;
}

int vlan_pas_rule_clear_proc(void)
{
	//清空硬件表项
	if(CS_E_OK == vlan_qinq_hw_table_clear(pVlan_qinq_list_running))
	{
		cs_printf("clear vlan qinq hardware table success!\n");
	}
	else
	{
		cs_printf("clear vlan qinq hardware table failed!\n");
	}
	
	//清空软件表项
	if(CS_E_OK == vlan_qinq_sw_table_clear(pVlan_qinq_list_running))
	{
		cs_printf("clear vlan qinq software table success!\n");
	}
	else
	{
		cs_printf("clear vlan qinq software table failed!\n");
	}
	return CLI_OK;
}

int vlan_pas_rule_add_del_proc(char *argv[], int argc)
{
	//参数解析
	int ret = 0;
	int old_vid = 0;
	int new_vid = 0;
	QINQ_DIRECTION direction;
	QINQ_ACTION action;
	vlan_qinq_infor_t vlan_qinq_infor;
	int oper = 0;
	char *str_rule_opt = NULL;
	char *str_vlan_old = NULL;
	char *str_vlan_new = NULL;
	char *str_direction = NULL;
	char *str_vlan_oper = NULL;
	char *str_tpid = NULL;
	char *str_priority = NULL;
	char *str_enable = NULL;
	int cvid = 0;

	str_rule_opt = argv[0];
	str_vlan_old = argv[1];
	str_direction = argv[2];
	str_vlan_oper = argv[3];
	str_vlan_new = argv[4];
	str_tpid = argv[5];
	str_priority = argv[6];
	str_enable = argv[7];
	
	
	if(0 == strcmp("vlan-id", str_rule_opt))
	{
	}
	else
	{
		ret = CS_E_ERROR;
		goto end;
	}
	
	old_vid = atoi(str_vlan_old);
	
	if(0 == strcmp("up", str_direction))
	{
		direction = UP;
	}
	else if(0 == strcmp("down", str_direction))
	{
		direction = DOWN;
	}
	else
	{
		ret = CS_E_ERROR;
		goto end;
	}
	
	if(0 == strcmp("exchange", str_vlan_oper))
	{
		action = EXCHANGE;
	}
	else if(0 == strcmp("attach", str_vlan_oper))
	{
		action = ATTACH;
	}
	else
	{
		ret = CS_E_ERROR;
		goto end;
	}
	
	new_vid = atoi(str_vlan_new);

	if(0 == strcmp("8100", str_tpid))
	{
	}
	else
	{
		ret = CS_E_ERROR;
		goto end;
	}

	if(0 == strcmp("original", str_priority))
	{
	}
	else
	{
		ret = CS_E_ERROR;
		goto end;
	}
		
	if(UP == direction)
	{
		cvid = old_vid;
	}
	else if(DOWN == direction)
	{
		cvid = new_vid;
	}
	else
	{
		goto end;
	}
	
	vlan_qinq_infor.old_vid = old_vid;
	vlan_qinq_infor.direction = direction;
	vlan_qinq_infor.action = action;
	vlan_qinq_infor.new_vid = new_vid;
		
	oper = atoi(str_enable);
	if(ADD == oper)
	{
		int num = 0;
		vlan_qinq_sw_table_get_length(pVlan_qinq_list_running, &num);
		if(num < QINQ_NUM_MAX)
		{
			if(CS_E_OK == vlan_qinq_sw_table_add(pVlan_qinq_list_running, vlan_qinq_infor))
			{
				cs_printf("%s\n", "add software table success");
				if(CS_E_OK == vlan_qinq_hw_table_add(vlan_qinq_infor))
				{
					cs_printf("add update hardware success\n");
				}
				else
				{
					cs_printf("add update hardware failed\n");
				}
			}
			else
			{
				cs_printf("%s\n", "add software table failed");
			}
		}
		else
		{
			cs_printf("Connot add, QinQ table is full!\n");
		}
	}
	else if(DEL == oper)
	{
		//先删硬件表
		if(CS_E_OK == vlan_qinq_hw_table_del(vlan_qinq_infor))
		{
			cs_printf("del update hardware success\n");
			if(CS_E_OK == vlan_qinq_sw_table_del(pVlan_qinq_list_running, vlan_qinq_infor))
			{
				cs_printf("%s\n", "del software table success");
			}
			else
			{
				cs_printf("%s\n", "del software table failed");
			}
		}
		else
		{
			cs_printf("del update hardware failed\n");
		}
	}
	else
	{
		//do nothing
		cs_printf("in %s\n", __func__);
		cs_printf("wrong oper :%d\n", oper);
	}

end:
	return ret;
}
int cmd_vlan_pas_rule(struct cli_def *cli, char *command, char *argv[], int argc)
{
    // deal with help
    if(CLI_HELP_REQUESTED)
    {
    	if(CLI_OK == vlan_pas_rule_arg_check(argv, argc-1))
    	{
    		return vlan_pas_rule_arg_help(cli, argv, argc);
    	}
		else
		{
			cs_printf("%s\n", "wrong command");
			return CLI_ERROR;
		}
        
    }

	if(CLI_OK == vlan_pas_rule_arg_check(argv, argc))
	{
		switch(argc)
		{
			case 0:
				vlan_pas_rule_dump_proc();
				break;
			case 1:
				if(0 == strcmp(argv[0], "clear"))
				{
					vlan_pas_rule_clear_proc();
				}
				else
				{
					cs_printf("%s\n", "wrong command");
				}
				break;
			case 8:
				vlan_pas_rule_add_del_proc(argv, argc);
				break;
			default:
				cs_printf("%s\n", "wrong command");
				break;
		}
		
	}
	else
	{
		cs_printf("%s\n", "wrong command");
	}
	
    return CLI_OK;
}


int cmd_vlan_pas_rule_debug(struct cli_def *cli, char *command, char *argv[], int argc)
{
	// deal with help
    if(CLI_HELP_REQUESTED)
    {
    	switch(argc)
    	{
			case 1:
				return cli_arg_help(cli, 0,
				"<cr>", "show Hardware QinQ table",
				NULL);
				break;
			default:
				return cli_arg_help(cli, argc > 1, NULL);
				break;
    	}
    	
    }

	switch(argc)
	{
		case 0:
			vlan_qinq_hw_table_dump();
			break;
		default:
			cs_printf("wrong command\n");
			break;
	}
	return CLI_OK;
}
#endif

#ifdef HAVE_PORT_ISOLATION_CMD
int cmd_vlan_port_isolate(struct cli_def *cli, char *command, char *argv[], int argc)
{
	CS_IN cs_callback_context_t     context = {0};
	cs_boolean  en = 0;
	
	if(CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0, 
					"{[0|1]}*1", "isolate 1 enable; 0 disable", NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL  );
		}
	}


	if(argc >= 1)
	{
		if(argc == 1)
		{
			en = (atoi(argv[0])? 1: 0);
		}

		if(epon_request_onu_port_isolation_set(context, 0, 0, en) != CS_E_OK)
			cli_print(cli, "set all port isolate %s fail!\r\n", en?"enabled":"disabled");
		else
			{
				if(en)
					cli_print(cli,"set all port isolate enable success\n");
				else
					cli_print(cli,"set all port isolate disable success\n");
			}
	}
	else
	{	
		if(epon_request_onu_port_isolation_get(context, 0, 0, &en) != CS_E_OK)
			cli_print(cli, "get port isolate fail!\r\n");
		else
			cli_print(cli, "Port isolate is %s\r\n", en?"enabled":"disabled");

	}
	

	return CLI_OK;
}
#endif	/*(#ifdef HAVE_PORT_ISOLATION_CMD)*/

#ifdef HAVE_VLAN_MAPPING
int cmd_vlan_mapping(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_callback_context_t     context;
    cs_sdl_vlan_mapping_t mapping;
    
	if(CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0, 
					"{[0|1]}*1", " 1 enable; 0 disable", NULL);
			case 2:
                                if(atoi(argv[0]))
				return cli_arg_help(cli, 0, "<src_port>", " src port id", NULL);
                                else
				return cli_arg_help(cli, 0, "<cr>", " Just press enter to execute command!", NULL);                                    
			case 3:
                                if(atoi(argv[0]))
				return cli_arg_help(cli, 0, "<src_vid>", " src vlan id", NULL);
                                else
				return cli_arg_help(cli, 0, "<cr>", " Just press enter to execute command!", NULL);     
			case 4:
                                if(atoi(argv[0]))
				return cli_arg_help(cli, 0, "<dst_port>", " dst port id", NULL);
                                else
				return cli_arg_help(cli, 0, "<cr>", " Just press enter to execute command!", NULL);     
			case 5:
                                if(atoi(argv[0]))
				return cli_arg_help(cli, 0, "<dst_vid>", " dst vlan id", NULL);
                                else
				return cli_arg_help(cli, 0, "<cr>", " Just press enter to execute command!", NULL);     
                                   
			default:
				return cli_arg_help(cli, argc > 1, NULL  );
		}
	}

    memset(&mapping,0,sizeof(cs_sdl_vlan_mapping_t));
    if(0 == argc)
    {
        if(CS_E_OK == epon_request_vlan_mapping_get(context,0,0,&mapping))
        {
            if(mapping.valid)
            {
                cli_print(cli,"mapping is enabled,src port %d,src vid %d,dst port %d,dst vid %d",
                    mapping.src_port,mapping.src_vid,mapping.dst_port,mapping.dst_vid);
            }
            else
            {
                 cli_print(cli,"mapping is disabled");
            }
        }
        else
        {
            cli_print(cli,"error!!!");
        }
    }
    else if( 1 == argc || 5 == argc)    
    {

        if(1 == argc)
        {
            mapping.valid = (atoi(argv[0])? 1: 0);
        }
        else if(5 == argc)
        {
            mapping.valid       = (atoi(argv[0]) ?  1 :  0);
            mapping.src_port = atoi(argv[1]);
            mapping.src_vid   = atoi(argv[2]);
            mapping.dst_port = atoi(argv[3]);
            mapping.dst_vid   = atoi(argv[4]);
        }

        if(CS_E_OK != epon_request_vlan_mapping_set(context,0,0,&mapping))
        {
            cli_print(cli,"error!!!");
        }
    }
    else
    {
        cli_print(cli,"wrong args!!!");
    }

    return CLI_OK;
}
#endif

#ifdef HAVE_PORT_ISOLATION_RESTORE
static cs_boolean const g_port_isolation_mode_default = true;
extern int port_isolate_mode_config_recover(cs_boolean en)
{
	int ret = 0;
#if 0
	if(port_aal_isolation_set(en) != CS_E_OK)
#else
	CS_IN cs_callback_context_t     context = {0};
	if(epon_request_onu_port_isolation_set(context, 0, 0, en) != CS_E_OK)
#endif
	{
		cs_printf("set all port isolate %s fail!\r\n", en?"enabled":"disabled");
	}
	else
	{
		cs_printf("set all port isolate %s success\r\n", en?"enable":"disable");	
	}
	return ret;
}
extern int port_isolate_mode_config_show(cs_boolean en)
{
	int ret = 0;

	cs_printf("\n-----------------------------------\n");
	cs_printf("all port isolate is %s\r\n", en?"disabled":"disabled");
	cs_printf("-----------------------------------\n");
	
	return ret;
}

extern int port_isolate_mode_tlv_infor_get(int *len, char **value, int *free_need)
{
	int ret = 0;
	int buf_len = 0;
	cs_boolean *port_isolate_mode = NULL;
	
	if(NULL == len)
	{
		goto error;
	}
	else
	{
		*len = 0;
	}

	if(NULL == value)
	{
		goto error;
		
	}
	else
	{
		*value = NULL;
	}
	
	if(NULL == free_need)
	{
		goto error;
	}
	else
	{
		*free_need = 0;
	}

	*free_need = 1;
	buf_len = sizeof(cs_boolean);
	port_isolate_mode = (cs_boolean *)iros_malloc(IROS_MID_APP, buf_len);
#if 0
	if(port_aal_isolation_get(port_isolate_mode) != CS_E_OK)
#else
	CS_IN cs_callback_context_t     context = {0};
	if(epon_request_onu_port_isolation_get(context, 0, 0, port_isolate_mode) != CS_E_OK)
#endif
	{
		cs_printf("%s\r\n", "get port isolate fail!");
	}
	else
	{
		if((g_port_isolation_mode_default&&(*port_isolate_mode))||((!g_port_isolation_mode_default)&&(!(*port_isolate_mode))))
		{
			goto end;
		}
		else
		{
			*len = buf_len;
			*value = (char *)port_isolate_mode;
		}
	}
	ret = 0;
	goto end;
	
error:
	ret = -1;
	
end:
	return ret;
}

extern int port_isolate_mode_tlv_infor_handle(int len, char *data, int opcode)
{
	int ret = 0;
	cs_boolean port_isolate_mode = 0;
	
	if(0 != len)
	{
		//do nothing
	}
	else
	{
		goto error;
	}
	
	if(NULL != data)
	{
		//do nothing
	}
	else
	{
		goto error;
	}

	memcpy(&port_isolate_mode, data, sizeof(cs_boolean));
	if(DATA_RECOVER == opcode)
	{
		port_isolate_mode_config_recover(port_isolate_mode);
	}
	else if(DATA_SHOW == opcode)
	{
		port_isolate_mode_config_show(port_isolate_mode);
	}
	else
	{
		cs_printf("in %s, unknown opcode :%d\n", __func__, opcode);
	}
	
	ret = 0;
	goto end;
	
error:
	ret = -1;
	
end:	
	return ret;
}

extern int port_isolate_mode_running_config_show(void)
{
	int ret = 0;
	cs_boolean en = 0;
#if 0
	if(port_aal_isolation_get(&en) != CS_E_OK)
#else		
	CS_IN cs_callback_context_t     context = {0};
	if(epon_request_onu_port_isolation_get(context, 0, 0, &en) != CS_E_OK)
#endif
	{
		cs_printf("\n-----------------------------------\n");
		cs_printf("get port isolate fail!\r\n");
		cs_printf("-----------------------------------\n");
	}
	else
	{
		if((g_port_isolation_mode_default&&en)||((!g_port_isolation_mode_default)&&(!en)))
		{
			//do nothing
		}
		else
		{
			port_isolate_mode_config_show(en);
		}
	}
	
	return ret;
}
#endif	/*(#ifdef HAVE_PORT_ISOLATION_RESTORE)*/

#ifdef HAVE_GWD_MC_TVM
cs_int32 gw_igmp_tvm_dump(struct cli_def *cli, cs_int8 *command, cs_int8 *argv[], cs_int32 argc)
{
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<cr>", "show gw igmp tvm table",
				NULL);
			default:
				return cli_arg_help(cli, argc > 3, NULL);
		}
	}
	if(0 == argc)
	{
		igmp_control_table_dump();
	}
	else
	{
		cs_printf("no command!\n");
	}
	return CLI_OK;
}

#endif	/*(#ifdef HAVE_GWD_MC_TVM)*/

#ifdef HAVE_GWD_STATISTICS_CMD
extern int show_stat_uni(struct cli_def *cli, cs_uint8 port_id);
extern int show_stat_pon(struct cli_def *cli);
extern int show_stat_cpu(struct cli_def *cli);
#ifdef HAVE_STATISTICS_NNI
extern int show_stat_nni(struct cli_def *cli);
#endif

#ifdef HAVE_PLATFORM_UNIFY_CMD
#define NNISTR "nni"

int cmd_stat_port_show(struct cli_def *cli, char *command, char *argv[], int argc)
{
	cs_port_id_t portid=0;
	cs_uint32 port_num = UNI_PORT_MAX;
			
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			#ifdef HAVE_MPORTS
	        case 1:
				cli_arg_help(cli, 0,
					"<cr>", "all port",
					NULL);
				#ifdef HAVE_GWD_OAM_FAST_STATISTIC
				return cli_arg_help(cli, 0,
					"<portlist>|nni]","port range 1-4",
					NULL);
				#else
				return cli_arg_help(cli, 0,
					"<portlist>", "value range 1-4",
					NULL);
				#endif
			#else
			case 1:
				cli_arg_help(cli, 0,
					"<cr>", "all port",
					NULL);
				return cli_arg_help(cli, 0,
					"nni",
					NULL);
			#endif
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}
	
	if(0 == argc)
	{		   
		for(portid=1;portid<=port_num;portid++)
		{
			show_stat_uni(cli,portid);
		}
	}
	else if(1 == argc)
	{
		#ifdef HAVE_GWD_OAM_FAST_STATISTIC
		if(0 == strcasecmp(NNISTR,argv[0]))
		{
			show_stat_nni(cli);
		}
		else
		#endif
		{
			cs_int32 port_array[UNI_PORT_MAX] = {0};
			cs_uint8   i = 0;

			if(CS_E_OK == gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
			{
				for(i=0;i < port_num;i++)
				{
					show_stat_uni(cli,port_array[i]);
				}
			}
		}	
	} 
	else
	{
		cli_print(cli, "%% Invalid input.");
	}

	return CLI_OK;
}

int cmd_stat_port_flush(struct cli_def *cli, char *command, char *argv[], int argc)
{

	cs_port_id_t portid=0;
	cs_uint32 port_num = UNI_PORT_MAX;
			
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 0:
				return cli_arg_help(cli, 0,
					"<cr>", "all port",
					 NULL);
			#ifdef HAVE_MPORTS
			case 1:
				cli_arg_help(cli, 0,
					"<cr>", "all port",
					 NULL);
				#ifdef HAVE_STATISTICS_NNI
				return cli_arg_help(cli, 0,
					"<portlist>|nni]","port range 1-4",
					NULL);
				#else
				return cli_arg_help(cli, 0,
					"<portlist>", "value range 1-4",
					 NULL);
				#endif
			#else
			case 1:
				cli_arg_help(cli, 0,
					"<cr>", "all port",
					 NULL);
				return cli_arg_help(cli, 0,
					"<port id>", "UNI:1",
					 NULL);
			#endif
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}
	
	if(0 == argc)
	{		   
		for(portid=1;portid<=port_num;portid++)
		{
			app_stats_reset_uni((cs_port_id_t)portid);
		}
	} 
	else if(1 == argc)
	{
		#ifdef HAVE_GWD_OAM_FAST_STATISTIC
		if(0 == strcasecmp(NNISTR,argv[0]))
		{
			app_stats_reset_nni();
		}
		else
		#endif
		{
			cs_int32 port_array[UNI_PORT_MAX] = {0};
			cs_uint8   i = 0;

			if(CS_E_OK == gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
			{
				for(i=0;i < port_num;i++)
				{
					app_stats_reset_uni((cs_port_id_t)port_array[i]);
				}
			}
		}	
	} 
	else
	{
		cli_print(cli, "%% Invalid input.");
	}
	
	return CLI_OK;
}

int cmd_stat_pon_show(struct cli_def *cli, char *command, char *argv[], int argc)
{
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
					"<cr>", "Enter to show pon statistics",
					 NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	show_stat_pon(cli);

	return CLI_OK;
}

int cmd_stat_pon_flush(struct cli_def *cli, char *command, char *argv[], int argc)
{
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
			return cli_arg_help(cli, 0,
				"<cr>", "Enter to clear pon statistics",
				 NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	app_stats_reset_pon();

	return CLI_OK;
}


int cmd_stat_cpu_show(struct cli_def *cli, char *command, char *argv[], int argc)
{
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
					"<cr>", "Enter to show cpu statistics",
					 NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	show_stat_cpu(cli);

	return CLI_OK;
}

int cmd_stat_cpu_flush(struct cli_def *cli, char *command, char *argv[], int argc)
{
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
			return cli_arg_help(cli, 0,
				"<cr>", "Enter to clear cpu statistics",
				 NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	stats_reset_cpu();

	return CLI_OK;
}

#else
int cmd_statistics_port_show(struct cli_def *cli, char *command, char *argv[], int argc)
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
				cli_arg_help(cli, 0,
					"<cr>", "all port",
					NULL);
				#ifdef HAVE_GWD_OAM_FAST_STATISTIC
				return cli_arg_help(cli, 0,
					"<port id>", "uni:1-4, nni:5",
					NULL);
				#else
				return cli_arg_help(cli, 0,
					"<uni id>", "value in  (1, 4)",
					NULL);
				#endif
			#else
			case 1:
				cli_arg_help(cli, 0,
					"<cr>", "all port",
					NULL);
				return cli_arg_help(cli, 0,
					"<uni id>", "value in  (1, 1)",
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
		for(portid=1;portid<=max_uni_port;portid++)
		{
			show_stat_uni(cli,portid);
		}
	}
	else if(1 == argc)
	{
		portid=atoi(argv[0]);
		#ifdef HAVE_GWD_OAM_FAST_STATISTIC
		if(portid == (max_uni_port+1))
		{
			show_stat_nni(cli);
		}
		else
		{
			show_stat_uni(cli,portid);
		}
		#else
		show_stat_uni(cli,portid);
		#endif
	} 
	else
	{
		cli_print(cli, "%% Invalid input.");
	}

	return CLI_OK;
}

int cmd_statistics_port_clear(struct cli_def *cli, char *command, char *argv[], int argc)
{

	cs_port_id_t portid=0;
	cs_uint8 max_uni_port=0;
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 0:
				return cli_arg_help(cli, 0,
					"<cr>", "all port",
					 NULL);
			#ifdef HAVE_MPORTS
			case 1:
				cli_arg_help(cli, 0,
					"<cr>", "all port",
					 NULL);
				#ifdef HAVE_STATISTICS_NNI
				return cli_arg_help(cli, 0,
					"<port id>", "uni:1-4, nni:5",
					NULL);
				#else
				return cli_arg_help(cli, 0,
					"<port id>", "UNI:1-4",
					 NULL);
				#endif
			#else
			case 1:
				cli_arg_help(cli, 0,
					"<cr>", "all port",
					 NULL);
				return cli_arg_help(cli, 0,
					"<port id>", "UNI:1",
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
		for(portid=1;portid<=max_uni_port;portid++)
		{
			app_stats_reset_uni((cs_port_id_t)portid);
		}
	} 
	else if(1 == argc)
	{
		portid=atoi(argv[0]);

		#ifdef HAVE_STATISTICS_NNI
		if(portid == (max_uni_port+1))
		{
			app_stats_reset_nni();
		}
		else
		{
			if((1<=portid)&&(portid<=max_uni_port))
			{
				app_stats_reset_uni((cs_port_id_t)portid);
			}
			else
			{
				cli_print(cli, "%% Invalid port ID.");
			}
		}
		#else
		if((1<=portid)&&(portid<=max_uni_port))
		{
			app_stats_reset_uni((cs_port_id_t)portid);
		}
		else
		{
			cli_print(cli, "%% Invalid port ID.");
		}
		#endif
	} 
	else
	{
		cli_print(cli, "%% Invalid input.");
	}
	
	return CLI_OK;
}

int cmd_statistics_pon_show(struct cli_def *cli, char *command, char *argv[], int argc)
{
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
					"<cr>", "Enter to show pon statistics",
					 NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	show_stat_pon(cli);

	return CLI_OK;
}

int cmd_statistics_pon_clear(struct cli_def *cli, char *command, char *argv[], int argc)
{
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
			return cli_arg_help(cli, 0,
				"<cr>", "Enter to clear pon statistics",
				 NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	app_stats_reset_pon();

	return CLI_OK;
}


int cmd_statistics_cpu_show(struct cli_def *cli, char *command, char *argv[], int argc)
{
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
					"<cr>", "Enter to show cpu statistics",
					 NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	show_stat_cpu(cli);

	return CLI_OK;
}

int cmd_statistics_cpu_clear(struct cli_def *cli, char *command, char *argv[], int argc)
{
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
			return cli_arg_help(cli, 0,
				"<cr>", "Enter to clear cpu statistics",
				 NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	stats_reset_cpu();

	return CLI_OK;
}
#endif  /*(#ifdef HAVE_PLATFORM_UNIFY_CMD) */
#endif	/*(#ifdef HAVE_GWD_STATISTICS_CMD)*/

#ifdef HAVE_GWD_BC_LIMIT
#define PORT_DOWN_ENABLE 1
#define PORT_DOWN_DISABLE 0

extern 	broadcast_storm_s broad_storm;
int cmd_bsctrl_policy(struct cli_def *cli, char *command, char *argv[], int argc)
{

	//int portid = 0;
	int len;
	int storm_stat;
	
	if(CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
					"[enable|disable]", "enable: port down; disable: only limit rate", NULL );

			default:
				return cli_arg_help(cli, argc > 1, NULL  );
		}
	}
	len = strlen(argv[0]);
	if(argc == 0)
		{
			storm_stat = broad_storm.gulBcStormStat? 1:0;
			if(storm_stat)
				cli_print(cli,"port will be link down when broadcast storm happened.");
			else
				cli_print(cli,"Port will be rate limit when broadcast storm happened.");
		}
	if(argc == 1)
		{
			if((strncmp("enable",argv[0],len)) && (strncmp("disable",argv[0],len)))
				{
					cli_print(cli,"%% Invalid input.\n");
					return CLI_ERROR;
				}
			else
				{
					if(!strncmp("enable",argv[0],len))
						{
							broad_storm.gulBcStormStat = PORT_DOWN_ENABLE;
						}
					if(!strncmp("disable",argv[0],len))
						{
							broad_storm.gulBcStormStat = PORT_DOWN_DISABLE;
						}
					cli_print(cli,"set portdown stat success\n");
				}
		}
	if(argc > 1)
		{
			cli_print(cli,"%% Invalid input.\n");
			return CLI_ERROR;
		}
	return CLI_OK;
}

int cmd_bsctrl_threshold(struct cli_def *cli, char *command, char *argv[], int argc)
{

	cs_uint32 gw_threshold = 0;

	gw_threshold = atoi(argv[0]);
	if(CLI_HELP_REQUESTED)
	{
		
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
					"<10-2000000>", "unit: packets per second", NULL );
			default:
				return cli_arg_help(cli, argc > 1, NULL  );
		}

	}
	if(argc == 1)
		{
			if(gw_threshold > BC_STORM_THRESHOLD_MAX || gw_threshold < BC_STORM_THRESHOLD_LAS)
				{
					cli_print(cli,"set broadcast storm threshold fail\n");
					return CLI_ERROR;
				}
			broad_storm.gulBcStormThreshold = gw_threshold;
			cli_print(cli,"set broadcast storm threshold success\n");
		}
	else
		{
			cli_print(cli,"%% Invalid input.\n");
		}

	return CLI_OK;
}

int cmd_bsctrl_threshold_get(struct cli_def *cli, char *command, char *argv[], int argc)
{
	cs_uint64 threshold;
	
	threshold = broad_storm.gulBcStormThreshold;

	cli_print(cli,"broadcast storm threshold %lld",threshold);
	return CLI_OK;
}
#endif	/*(#ifdef HAVE_GWD_BC_LIMIT)*/

#ifdef HAVE_GWD_PORT_MODE_RESTORE
typedef struct
{
	cs_port_id_t port_id;
	cs_sdl_port_speed_cfg_t speed_cfg;
}__sdl_port_mode_t;

extern int gwd_port_mode_recover(__sdl_port_mode_t *port_mode)
{
	int ret = 0;
	cs_callback_context_t context;
	if(NULL == port_mode)
	{
		return -1;
	}
	
	if(CS_E_OK == epon_request_onu_port_status_set(context, 0, 0, port_mode->port_id, port_mode->speed_cfg))
	{
		ret = 0;
	}
	else
	{
		ret = -1;
	}
	
	return ret;
}

extern int gwd_port_mode_show(__sdl_port_mode_t *port_mode)
{
	if(NULL == port_mode)
	{
		return -1;
	}
	
	char *description[] = {"Reserved", " Auto(10/100Mbps)", "Auto(10/100/1000Mbps)", "10Mbps(full)", "10Mbps(half)", 
							"100Mbps(full)", "100Mbps(half)", "1000Mbps(full)"};
	cs_printf("port :%d, mode :%s\n", port_mode->port_id, description[port_mode->speed_cfg]);
	
	return 0;
}

extern int gwd_port_mode_tlv_info_get(int *len, char **value, int *free_need)
{
	int ret = 0;
	__sdl_port_mode_t port_mode[UNI_PORT_MAX]; 
	cs_callback_context_t context;
	cs_port_id_t port_id;
	cs_sdl_port_speed_cfg_t *speed_cfg = NULL;
	int count = 0;
	int i = 0;
	char *buf = NULL;
	int buf_size = 0;
	
	if(NULL == len)
	{
		goto error;
	}
	if(NULL == value)
	{
		goto error;
	}
	if(NULL == value)
	{
		goto error;
	}
	
	for(i=0;i<UNI_PORT_MAX; i++)
	{
		port_id = CS_UNI_PORT_ID1 + i;
		
		port_mode[count].port_id = port_id; 
		speed_cfg = &port_mode[count].speed_cfg;
		if(CS_E_OK == epon_request_onu_port_status_get(context, 0, 0, port_id, speed_cfg))
		{
			count++;
		}
	}

	if(0 == count)
	{
		goto error;
	}
	
	buf_size = sizeof(__sdl_port_mode_t) * count;	
	if(NULL != (buf = (char *)iros_malloc(IROS_MID_APP, buf_size)))
	{
		memcpy(buf, port_mode, buf_size);
		*len = buf_size;
		*value = buf;
		*free_need = 1;
		goto end;
	}
	
error:
	ret = -1;
	
end:	
	return ret;
}

extern int gwd_port_mode_tlv_info_handle(int length,char *value,int opcode)
{
	int port_num = 0;
	int i = 0;
	__sdl_port_mode_t port_mode;
        char *pvalue = value;

	if(0 == length)
	{
		return -1;
	}
	if(NULL == value)
	{
		return -1;
	}
	port_num = length / sizeof(__sdl_port_mode_t);

	for(i=0; i<port_num; i++)
	{
                  memcpy(&port_mode,pvalue,sizeof(port_mode));
                  pvalue += sizeof(port_mode);

		if(DATA_RECOVER == opcode)
		{
			gwd_port_mode_recover(&port_mode);
		}
		else if(DATA_SHOW == opcode)
		{
			gwd_port_mode_show(&port_mode);
		}
		else
		{
			cs_printf("in %s, opcode error :%d\n", __func__, opcode);
			break;
		}
	}
	
	return 0;
}

extern int gwd_port_mode_running_config_show(void)
{
	int i = 0;
	cs_port_id_t port_id;
	cs_callback_context_t context;
	cs_sdl_port_speed_cfg_t speed_cfg;
	__sdl_port_mode_t port_mode;
	
	for(i=0; i<UNI_PORT_MAX; i++)
	{
		port_id = CS_UNI_PORT_ID1 + i;
		if(CS_E_OK == epon_request_onu_port_status_get(context, 0, 0, port_id, &speed_cfg))
		{
			port_mode.port_id = port_id;
			port_mode.speed_cfg = speed_cfg;
			
			gwd_port_mode_show(&port_mode);
		}
	}
	return 0;
}
#endif	/*(#ifdef HAVE_GWD_PORT_MODE_RESTORE)*/

#ifdef HAVE_GWD_PORT_MODE_CMD
int port_mode_set(int port, int mode)
{
	cs_port_id_t port_id;
	cs_sdl_port_speed_cfg_t speed_cfg;
	int status = 0;
	int ret = 0;
	
	port_id = port;
	switch(mode)
	{
		case 0:
			#if (HAVE_SWITCH_SPEED_1000 == MODULE_YES)
			speed_cfg = SDL_PORT_AUTO_10_100_1000;
			#else
			speed_cfg = SDL_PORT_AUTO_10_100;
			#endif
			break;
			
		case 10:
			speed_cfg = SDL_PORT_10_FULL;
			break;

		case 11:
			speed_cfg = SDL_PORT_10_HALF;
			break;

		case 12:
			speed_cfg = SDL_PORT_1000_FULL;
			break;

		case 8:
			speed_cfg = SDL_PORT_100_FULL;
			break;

		case 9:
			speed_cfg = SDL_PORT_100_HALF;
			break;

		default:
			status = -1;
			break;
	}

	if(0 == status)
	{
		cs_callback_context_t context;
		
		if(CS_E_OK == epon_request_onu_port_status_set(context, 0, 0, port_id, speed_cfg))
		{
			ret = 0;
		}
		else
		{
			ret = -1;
		}
	}
	else
	{
		ret = -1;
	}
	
	return ret;
}

#ifdef HAVE_PLATFORM_UNIFY_CMD
void show_port_mode(struct cli_def *cli,cs_port_id_t port_id)
{
	cs_status ret = CS_E_OK;
	cs_callback_context_t context;
	cs_sdl_port_ether_speed_t speed;
	cs_sdl_port_link_status_t link_status;
	cs_sdl_port_admin_t admin;
	cs_sdl_port_ether_duplex_t duplex;
	cs_sdl_port_speed_cfg_t speed_cfg;
	cs_sdl_port_autoneg_status_t auto_neg;
		
	do
	{
		if(CS_E_OK != (ret = epon_request_onu_port_auto_neg_get(context, 0, 0, port_id, &auto_neg)))
		{
			break;
		}

		if(CS_E_OK != (ret = epon_request_onu_port_status_get(context, 0, 0, port_id, &speed_cfg)))
		{
			break;
		}
		
		if(CS_E_OK != (ret = epon_request_onu_port_speed_get(context, 0, 0, port_id, &speed)))
		{
			break;
		}

		if(CS_E_OK != (ret = epon_request_onu_port_duplex_get(context, 0, 0, port_id, &duplex)))
		{
			break;
		}

		if(CS_E_OK != (ret = epon_request_onu_port_link_status_get(context, 0, 0, port_id, &link_status)))
		{
			break;
		}
		
		if(CS_E_OK != (ret = epon_request_onu_port_admin_get(context, 0, 0, port_id, &admin)))
		{
			break;
		}

		ret = CS_E_OK;
	}while(0);

	if(CS_E_OK == ret)
	{
		int max_speed = 0;
		int current_speed = 0;
		cs_boolean AutoNegotiation_enable = true;
		AutoNegotiation_enable = (SDL_PORT_AUTONEG_UP==auto_neg)?true:false;

		switch(speed_cfg)
		{
			case SDL_PORT_AUTO_10_100:
				max_speed = 100;
				break;

			case SDL_PORT_AUTO_10_100_1000:
				max_speed = 1000;
				break;

			case SDL_PORT_10_FULL:		
			case SDL_PORT_10_HALF:
				max_speed = 10;
				break;
				
			case SDL_PORT_100_FULL:		
			case SDL_PORT_100_HALF:
				max_speed = 100;
				break;

			case SDL_PORT_1000_FULL:
			case SDL_PORT_1000_HALF:
				max_speed = 1000;
				break;

			default:
				max_speed = -1;
				break;
		}
		
		switch(speed)
		{
			case SDL_PORT_SPEED_10:
				current_speed = 10;
				break;
				
			case SDL_PORT_SPEED_100:
				current_speed = 100;
				break;

			case SDL_PORT_SPEED_1000:
				current_speed = 1000;
				break;

			default:
				current_speed = -1;
				break;
		}
		
		cli_print(cli, "Port :%d, AutoNegotiation :%s", port_id, AutoNegotiation_enable?"enabled":"disabled");	
		if(AutoNegotiation_enable)
		{
			cli_print(cli, "Max speed %d M, current speed %d M, Duplex %s", max_speed, current_speed, (SDL_PORT_DUPLEX_HALF == duplex)?"half":"full");
		}
		else
		{
			cli_print(cli, "speed %d M, Duplex %s", current_speed, (SDL_PORT_DUPLEX_HALF == duplex)?"half":"full");
		}
		cli_print(cli, "physical status is %s, Administrator status is %s", (SDL_PORT_LINK_STATUS_UP == link_status)?"up":"down", (SDL_PORT_ADMIN_UP == admin)?"up":"down");
	}
	else
	{
		cli_print(cli, "execuse error!");
	}
}
int cmd_port_mode(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int port = 0;
	int mode = 0;	
	cs_int32 port_array[UNI_PORT_MAX] = {0};
	cs_uint32 port_num = UNI_PORT_MAX;
	cs_uint8   i = 0;
	
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<port_list>", "Please input the port number",
				NULL);

			case 2:
				cli_arg_help(cli, 0,
				"<cr>", "Enter to show port mode",
				NULL);
				cli_arg_help(cli, 0,
				"0", "Auto negotiation",
				NULL);
				cli_arg_help(cli, 0,
				"10", "10M/FD",
				NULL);
				cli_arg_help(cli, 0,
				"11", "10M/HD",
				NULL);
				cli_arg_help(cli, 0,
				"12", "1000M/FD",
				NULL);
				cli_arg_help(cli, 0,
				"8", "100M/FD",
				NULL);
				return cli_arg_help(cli, 0,
				"9", "100M/HD",
				NULL);

			case 3:
				return cli_arg_help(cli, 0,
				"<cr>", "Just press enter to execute command!",
				NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}
	
	if(CS_E_OK != gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
	{
		cli_print(cli, "execuse error!");
		return CLI_ERROR;
	}
	
		
	if(1==argc)
	{
		cs_port_id_t port_id;
		
		for(i = 0;i < port_num;i++)
		{
			port_id = port_array[i];
			//端口合法性检查
			if(CS_E_OK != uni_port_check(port_id))
			{
				//cli_print(cli, "wrong pord id :%d", port_id);
				//return CLI_ERROR;
				continue;
			}
			
			show_port_mode(cli,port_id);
		}
	}
	else if(2==argc)
	{
		//set
		mode = atoi(argv[1]);

		for(i = 0;i < port_num;i++)
		{
			port = port_array[i];
			if(0 == port_mode_set(port, mode))
			{
				cli_print(cli, "set port %d mode success!\n",port);
			}
			else
			{
				cli_print(cli, "set port %d mode failed!\n",port);
			}
			
			cli_print(cli, "port :%d, mode :%d\n", port, mode);
		}
	}
	else
	{
		cli_print(cli, "%% Invalid input.");
	}

	return CLI_OK;
}

int cmd_port_mode_show(struct cli_def *cli, char *command, char *argv[], int argc)
{	
	cs_int32 port_array[UNI_PORT_MAX] = {0};
	cs_uint32 port_num = UNI_PORT_MAX;
	cs_uint8   i = 0;
	
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<port_list>", "Please input the port number",
				NULL);

			case 2:
				return cli_arg_help(cli, 0,
				"<cr>", "Just press enter to execute command!",
				NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}
	
	if(CS_E_OK != gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
	{
		cli_print(cli, "execuse error!");
		return CLI_ERROR;
	}
	
		
	if(1==argc)
	{
		cs_port_id_t port_id;
		
		for(i = 0;i < port_num;i++)
		{
			port_id = port_array[i];
			//端口合法性检查
			if(CS_E_OK != uni_port_check(port_id))
			{
				//cli_print(cli, "wrong pord id :%d", port_id);
				//return CLI_ERROR;
				continue;
			}
			
			show_port_mode(cli,port_id);
		}
	}
	else
	{
		cli_print(cli, "%% Invalid input.");
	}

	return CLI_OK;
}

#else

int cmd_port_mode(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int port = 0;
	int mode = 0;
	
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<port_list>", "Please input the port number",
				NULL);

			case 2:
				cli_arg_help(cli, 0,
				"<cr>", "Enter to show port mode",
				NULL);
				cli_arg_help(cli, 0,
				"0", "Auto negotiation",
				NULL);
				cli_arg_help(cli, 0,
				"10", "10M/FD",
				NULL);
				cli_arg_help(cli, 0,
				"11", "10M/HD",
				NULL);
				cli_arg_help(cli, 0,
				"12", "1000M/FD",
				NULL);
				cli_arg_help(cli, 0,
				"8", "100M/FD",
				NULL);
				return cli_arg_help(cli, 0,
				"9", "100M/HD",
				NULL);

			case 3:
				return cli_arg_help(cli, 0,
				"<cr>", "Just press enter to execute command!",
				NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	if(1==argc)
	{
		cs_status ret = CS_E_OK;
		cs_callback_context_t context;
		cs_port_id_t port_id;
		cs_sdl_port_ether_speed_t speed;
		cs_sdl_port_link_status_t link_status;
		cs_sdl_port_admin_t admin;
		cs_sdl_port_ether_duplex_t duplex;
		cs_sdl_port_speed_cfg_t speed_cfg;
		cs_sdl_port_autoneg_status_t auto_neg;
		
		port_id = atoi(argv[0]);

		//端口合法性检查
		if(CS_E_OK != uni_port_check(port_id))
		{
			cli_print(cli, "wrong pord id :%d", port_id);
			return CLI_ERROR;	
		}
		
		do
		{
			if(CS_E_OK != (ret = epon_request_onu_port_auto_neg_get(context, 0, 0, port_id, &auto_neg)))
			{
				break;
			}
	
			if(CS_E_OK != (ret = epon_request_onu_port_status_get(context, 0, 0, port_id, &speed_cfg)))
			{
				break;
			}
			
			if(CS_E_OK != (ret = epon_request_onu_port_speed_get(context, 0, 0, port_id, &speed)))
			{
				break;
			}
	
			if(CS_E_OK != (ret = epon_request_onu_port_duplex_get(context, 0, 0, port_id, &duplex)))
			{
				break;
			}
	
			if(CS_E_OK != (ret = epon_request_onu_port_link_status_get(context, 0, 0, port_id, &link_status)))
			{
				break;
			}
			
			if(CS_E_OK != (ret = epon_request_onu_port_admin_get(context, 0, 0, port_id, &admin)))
			{
				break;
			}
	
			ret = CS_E_OK;
		}while(0);
	
		if(CS_E_OK == ret)
		{
			int max_speed = 0;
			int current_speed = 0;
			cs_boolean AutoNegotiation_enable = true;
			AutoNegotiation_enable = (SDL_PORT_AUTONEG_UP==auto_neg)?true:false;
	
			switch(speed_cfg)
			{
				case SDL_PORT_AUTO_10_100:
					max_speed = 100;
					break;
	
				case SDL_PORT_AUTO_10_100_1000:
					max_speed = 1000;
					break;
	
				default:
					max_speed = -1;
					break;
			}
			
			switch(speed)
			{
				case SDL_PORT_SPEED_10:
					current_speed = 10;
					break;
					
				case SDL_PORT_SPEED_100:
					current_speed = 100;
					break;
	
				case SDL_PORT_SPEED_1000:
					current_speed = 1000;
					break;
	
				default:
					current_speed = -1;
					break;
			}
			
			cli_print(cli, "Port :%d, AutoNegotiation :%s", port_id, AutoNegotiation_enable?"enabled":"disabled");	
			if(AutoNegotiation_enable)
			{
				cli_print(cli, "Max speed %d M, current speed %d M, Duplex %s", max_speed, current_speed, (SDL_PORT_DUPLEX_HALF == duplex)?"half":"full");
			}
			else
			{
				cli_print(cli, "speed %d M, Duplex %s", current_speed, (SDL_PORT_DUPLEX_HALF == duplex)?"half":"full");
			}
			cli_print(cli, "physical status is %s, Administrator status is %s", (SDL_PORT_LINK_STATUS_UP == link_status)?"up":"down", (SDL_PORT_ADMIN_UP == admin)?"up":"down");
		}
		else
		{
			cli_print(cli, "execuse error!");
		}
	}
	else if(2==argc)
	{
		//set
		port = atoi(argv[0]);
		mode = atoi(argv[1]);
		if(0 == port_mode_set(port, mode))
		{
			cli_print(cli, "set port mode success!\n");
		}
		else
		{
			cli_print(cli, "set port mode failed!\n");
		}
		
		cli_print(cli, "port :%d, mode :%d\n", port, mode);
	}
	else
	{
		cli_print(cli, "%% Invalid input.");
	}

	return CLI_OK;
}
#endif  /* #ifdef HAVE_PLATFORM_UNIFY_CMD */

#endif	/*(#ifdef HAVE_GWD_PORT_MODE_CMD)*/

#if (HAVE_GE_RATE_LIMIIT_CMD == MODULE_YES)
#define UNI_EGRESS_RATE_SAVE		0
#define UNI_EGRESS_RATE_MAX			1000000
#define UNI_EGRESS_RATE_NO_LIMIT	UNI_EGRESS_RATE_MAX
#define UNI_INGRESS_RATE_SAVE		0
#define UNI_INGRESS_RATE_MAX		1000000
#define UNI_INGRESS_RATE_NO_LIMIT	UNI_INGRESS_RATE_MAX

typedef struct
{
	cs_port_id_t port;
	cs_uint8 enable;
	cs_uint32 egress_rate;
} uni_egress_t;

typedef struct
{
	cs_port_id_t port;
	cs_uint8 enable;
	cs_uint32 ingress_rate;
} uni_ingress_t;

int cmd_port_egress_rate_arg_check(char *argv[], int argc)
{
	int ret = 0;
	int uni_port_id = 0;
	cs_uint32 rate = 0;

	switch(argc)
	{
		case 2:
			rate = atol(argv[1]);
			if(!((0 == rate)||((rate >= 62)&&(rate <= 1000000))))
			{
				cs_printf("wrong rate :%d\n", rate);
				ret = CLI_ERROR;
				break;
			}
		case 1:
			uni_port_id = atoi(argv[0]);
			if(uni_port_check(uni_port_id) != CS_E_OK)
			{
				cs_printf("there is no port :%d\n", uni_port_id);
				ret = CLI_ERROR;
				break;
			}
		case 0:
			ret = CLI_OK;
			break;
		default:
			ret = CLI_ERROR;
			break;
			
			
	}

	if(CLI_OK != ret)
	{
		cs_printf("%s\n", "arg check err!");
	}
	return ret;
}

int uni_egress_rate_show(uni_egress_t *uni_egress_p)
{
	int ret = 0;
	int port = 0;
	cs_uint8 enable = 0;
	cs_uint32 rate = 0;
	port = uni_egress_p->port;
	enable = uni_egress_p->enable;
	rate = uni_egress_p->egress_rate;
	cs_printf("Port %d: egress rate configuration:\n", port);
	if(0 == enable)
	{
		cs_printf("egress rate : %s\n", "No limited");
	}
	else
	{
		cs_printf("egress rate : %d kbps\n", rate);
	}
	return ret;
}

cs_status port_egress_rate_set(cs_port_id_t port, cs_uint32 rate)
{	
	cs_status ret = CS_E_OK;
	cs_uint8 oper = 0;
	cs_uint32 pir = 0;
	pir = rate;
	oper = (0 == rate)?0:1;
	ctc_oam_eth_ds_rate_limit_set_adapt(port, oper, rate, pir);
	return ret;
}

cs_status port_egress_rate_get(cs_port_id_t port, cs_uint8 *enable, cs_uint32 *rate)
{
	cs_status ret = CS_E_OK;
	cs_uint32 pir = 0;
	ctc_oam_eth_ds_rate_limit_get_adapt(port, enable, rate, &pir);
	return ret;
}
#ifdef HAVE_PLATFORM_UNIFY_CMD
int cmd_port_egress_rate(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int uni_port_id = 0;
	cs_uint8 enable = 0;
	cs_uint32 rate = 0;
	cs_int32 port_array[UNI_PORT_MAX] = {0};
	cs_uint32 port_num = UNI_PORT_MAX;
	cs_uint8   i = 0;
	
	if(CLI_HELP_REQUESTED)
	{
		if(cmd_port_egress_rate_arg_check(argv, argc-1) != CS_E_OK)
		{
			return CLI_ERROR;
		}
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
					"<port_list>", "Please input the port number,range 1-4", NULL );
			case 2:
				cli_arg_help(cli, 0,
					"<0>", "0-not limit", NULL );
				cli_arg_help(cli, 0,
					"<62-1000000>", "Please input the rate( 62-1000000) kbps", NULL );
				cli_arg_help(cli, 0,
					"<cr>", "Just press enter to execute command!", NULL );
				return CLI_OK;
			case 3:
				return cli_arg_help(cli, 0,
					"<cr>", "Just press enter to execute command!", NULL );
			default:
				return cli_arg_help(cli, argc > 1, NULL  );
		}
	}

	switch(argc)
	{
		case 0:
			cs_printf("%s\n", "% Command incomplete.");
			break;
		case 1:
			{
				if(CS_E_OK == gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
				{
					for(i=0;i < port_num;i++)
					{
						uni_egress_t uni_egress;
						
						uni_port_id = port_array[i];

						if(uni_port_id > port_num)
						{
							continue;
						}
						
						port_egress_rate_get(uni_port_id, &enable, &rate);
	
						uni_egress.port = uni_port_id;
						uni_egress.enable = enable;
						uni_egress.egress_rate = rate;

						uni_egress_rate_show(&uni_egress);
					}
				}
			}
			break;
		case 2:
			{
				if(CS_E_OK == gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
				{
					rate = atoi(argv[1]);
					for(i=0;i < port_num;i++)
					{
						uni_port_id = port_array[i];

						if(uni_port_id > port_num)
						{
							continue;
						}
						
						port_egress_rate_set(uni_port_id, rate);
					}
				}
			}			
			break;
	}

	return CLI_OK;
}
#else
int cmd_port_egress_rate(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int uni_port_id = 0;
	cs_uint8 enable = 0;
	cs_uint32 rate = 0;
	if(CLI_HELP_REQUESTED)
	{
		if(cmd_port_egress_rate_arg_check(argv, argc-1) != CS_E_OK)
		{
			return CLI_ERROR;
		}
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
					"<port_list>", "Please input the port number", NULL );
			case 2:
				cli_arg_help(cli, 0,
					"<0>", "0-not limit", NULL );
				cli_arg_help(cli, 0,
					"<62-1000000>", "Please input the rate( 62-1000000) kbps", NULL );
				cli_arg_help(cli, 0,
					"<cr>", "Just press enter to execute command!", NULL );
				return CLI_OK;
			case 3:
				return cli_arg_help(cli, 0,
					"<cr>", "Just press enter to execute command!", NULL );
			default:
				return cli_arg_help(cli, argc > 1, NULL  );
		}
	}

	if(cmd_port_egress_rate_arg_check(argv, argc) != CS_E_OK)
	{
		return CLI_ERROR;
	}
	switch(argc)
	{
		case 0:
			cs_printf("%s\n", "% Command incomplete.");
			break;
		case 1:
			uni_port_id = atoi(argv[0]);
			port_egress_rate_get(uni_port_id, &enable, &rate);
			uni_egress_t uni_egress;
			uni_egress.port = uni_port_id;
			uni_egress.enable = enable;
			uni_egress.egress_rate = rate;
			uni_egress_rate_show(&uni_egress);
			
			break;
		case 2:
			uni_port_id = atoi(argv[0]);
			rate = atoi(argv[1]);
			port_egress_rate_set(uni_port_id, rate);
			break;
	}

	return CLI_OK;
}
#endif /* #ifdef HAVE_PLATFORM_UNIFY_CMD */

int cmd_port_ingress_rate_arg_check(char *argv[], int argc)
{
	int ret = 0;
	int uni_port_id = 0;
	int limit_option = 0;
	cs_uint32 rate = 0;
	char frame_state[10] = "";
	char burst_size[10] = "";
	
	switch(argc)
	{
		case 5:
			memcpy(burst_size, argv[4], sizeof(burst_size)-1);
			burst_size[sizeof(burst_size) -1] = '\0';
			if(!((strcmp(burst_size, "12k")==0)||(strcmp(burst_size, "24k")==0)||(strcmp(burst_size, "48k")==0)||(strcmp(burst_size, "96k")==0)))
			{
				cs_printf("%s\n", "% There is no matched command.");
				ret = CLI_ERROR;
				break;
			}
		case 4:
			memcpy(frame_state, argv[3], sizeof(frame_state)-1);
			frame_state[sizeof(frame_state) -1] = '\0';
			if(!((strcmp(frame_state, "drop")==0)||(strcmp(frame_state, "pause")==0)))
			{
				cs_printf("%s\n", "% There is no matched command.");
				ret = CLI_ERROR;
				break;
			}
		case 3:
			rate = atol(argv[2]);
			if(!((0 == rate)||((rate >= 62)&&(rate <= 1000000))))
			{
				cs_printf("wrong rate :%d\n", rate);
				ret = CLI_ERROR;
				break;
			}
		case 2:
			limit_option = atoi(argv[1]);
			if((limit_option < 0)||(limit_option > 3))
			{
				ret = CLI_ERROR;
				break;
			}
		case 1:
			uni_port_id = atoi(argv[0]);
			if(uni_port_check(uni_port_id) != CS_E_OK)
			{
				cs_printf("there is no port :%d\n", uni_port_id);
				ret = CLI_ERROR;
			}
			break;
		case 0:
			ret = CLI_OK;
			break;
		default:
			ret = CLI_ERROR;
			break;
	}
	
	if(CLI_OK != ret)
	{
		cs_printf("%s\n", "arg check err!");
	}
	return ret;
}

int uni_ingress_rate_show(uni_ingress_t *uni_ingress_p)
{
	int ret = 0;
	int port = 0;
	cs_uint8 enable = 0;
	cs_uint32 rate = 0;
	port = uni_ingress_p->port;
	enable = uni_ingress_p->enable;
	rate = uni_ingress_p->ingress_rate;
	cs_printf("Port %d: ingress rate configuration:\n", port);
	if(0 == enable)
	{
		cs_printf("ingress rate : %s\n", "No limited");
	}
	else
	{
		cs_printf("ingress rate : %d kbps\n", rate);
	}
	return ret;
}

cs_status port_ingress_rate_set(cs_port_id_t port, cs_uint32 rate)
{
	cs_status ret = CS_E_OK;
	cs_uint8 oper = 0;
	cs_uint32 cbs = 0;
	cs_uint32 ebs = 0;
	
	if(0 == rate)
	{
		oper = 0;
		cbs = 0;
		ebs = 0;
	}
	else
	{
		oper = 1;
		cbs = rate * 1000 / 8;
		ebs = 1522;
	}
	
	ret = ctc_oam_eth_port_policing_set_adapt(port, oper, rate, cbs, ebs);
	
	return ret;
}

cs_status port_ingress_rate_get(cs_port_id_t port, cs_uint8 *enable, cs_uint32 *rate)
{
	cs_status ret = CS_E_OK;
	cs_uint32 cbs = 0;
	cs_uint32 ebs = 0;
	
	ret = ctc_oam_eth_port_policing_get_adapt(port, enable, rate, &cbs, &ebs);
	return ret;
}

#ifdef HAVE_PLATFORM_UNIFY_CMD
int cmd_port_ingress_rate(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int uni_port_id = 0;
	cs_uint8 enable = 0;
	cs_uint32 rate = 0;
	cs_int32 port_array[UNI_PORT_MAX] = {0};
	cs_uint32 port_num = UNI_PORT_MAX;
	cs_uint8   i = 0;
	
	if(CLI_HELP_REQUESTED)
	{
		/*参数数据的合法性检查*/
		if(cmd_port_ingress_rate_arg_check(argv, argc-1) != CLI_OK)
		{
			return CLI_ERROR;
		}

		/*显示帮助信息*/
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
					"<port_list>", "Please input the port number,range 1-4", NULL );
			case 2:			
				cli_arg_help(cli, 0,
					"0", "0-Limit all frames", NULL );
				return CLI_OK;
			case 3:			
				cli_arg_help(cli, 0,
					"<0>", "0-not limit", NULL );
				cli_arg_help(cli, 0,
					"<62-1000000>", "Port ingress rate,unit:kbps", NULL );
				return CLI_OK;
			case 4:
				cli_arg_help(cli, 0,
					"drop", "Frames will be dropped when exceed limit.", NULL );
				cli_arg_help(cli, 0,
					"pause", "Port will transmit pause frame when exceed limit.", NULL );
				cli_arg_help(cli, 0,
					"<cr>", "Ambiguous command !", NULL );
				return CLI_OK;
			case 5:
				return cli_arg_help(cli, 0,
					"<cr>", "Just press enter to execute command!", NULL );
			default:
				return cli_arg_help(cli, argc > 1, NULL  );
		}
	}

	/*参数数据的合法性检查*/
	if(cmd_port_ingress_rate_arg_check(argv, argc) != CLI_OK)
	{
		return CLI_ERROR;
	}
		
	switch(argc)
	{
		case 0:
			cs_printf("%s\n", "% Command incomplete.");
			break;
		case 1:
			{
				if(CS_E_OK == gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
				{
					cs_callback_context_t         context;
					cs_boolean                    flow_ctrl_enable = 0;
					
					for(i=0;i < port_num;i++)
					{
						uni_ingress_t uni_ingress;
						
						uni_port_id = port_array[i];
						
						if(uni_port_id > port_num)
						{
							continue;
						}
						
						port_ingress_rate_get(uni_port_id, &enable, &rate);
	
						uni_ingress.port = uni_port_id;
						uni_ingress.enable = enable;
						uni_ingress.ingress_rate = rate;
						
						uni_ingress_rate_show(&uni_ingress);
						epon_request_onu_port_flow_ctrl_get ( context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API , uni_port_id , &flow_ctrl_enable);

					    cli_print(cli, "Port %d: flow control configuration:\n flow control:%s",uni_port_id,flow_ctrl_enable==0? "disable":"enable" );
					}
				}
			}		
			break;
		case 3:
		case 4:
			{
				if(CS_E_OK == gwd_parse_num_list_from_str(argv[0],port_array, &port_num))
				{
					rate = atol(argv[2]);
					cs_uint8 flow_control=0;

					if(4 == argc)
					{
						if(0 == strcmp("pause",argv[3]))
						{
							flow_control = 1;
						}
					}
					
					for(i=0;i < port_num;i++)
					{
						uni_port_id = port_array[i];

						if(uni_port_id > port_num)
						{
							continue;
						}

						port_ingress_rate_set(uni_port_id, rate);

						if(4 == argc)
						{
							ctc_oam_eth_port_pause_set_adapt( uni_port_id,  flow_control);
						}
					}
				}
			}
			break;
		default:
			cs_printf("%s\n", "there is no command!");
	}

	return CLI_OK;
}
#else
int cmd_port_ingress_rate(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int uni_port_id = 0;
	cs_uint8 enable = 0;
	cs_uint32 rate = 0;
	
	if(CLI_HELP_REQUESTED)
	{
		/*参数数据的合法性检查*/
		if(cmd_port_ingress_rate_arg_check(argv, argc-1) != CLI_OK)
		{
			return CLI_ERROR;
		}

		/*显示帮助信息*/
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
					"<port_list>", "Please input the port number", NULL );
			case 2:			
				cli_arg_help(cli, 0,
					"0", "0-Limit all frames", NULL );
				cli_arg_help(cli, 0,
					"1", "1-Limit Broadcast, Multicast and flooded unicast frames", NULL );
				cli_arg_help(cli, 0,
					"2", "2-Limit Broadcast and Multicast frames only", NULL );
				cli_arg_help(cli, 0,
					"3", "3-Limit Broadcast frames only", NULL );
				cli_arg_help(cli, 0,
					"<cr>", "Just press enter to execute command!", NULL );
				return CLI_OK;
			case 3:			
				cli_arg_help(cli, 0,
					"<0>", "0-not limit", NULL );
				cli_arg_help(cli, 0,
					"<62-1000000>", "Port ingress rate,unit:kbps", NULL );
				return CLI_OK;
			case 4:
				cli_arg_help(cli, 0,
					"drop", "Frames will be dropped when exceed limit.", NULL );
				cli_arg_help(cli, 0,
					"pause", "Port will transmit pause frame when exceed limit.", NULL );
				cli_arg_help(cli, 0,
					"<cr>", "Ambiguous command !", NULL );
				return CLI_OK;
			case 5:
				cli_arg_help(cli, 0,
					"12k", "Burst mode : Burst Size is 12k bytes.", NULL );
				cli_arg_help(cli, 0,
					"24k", "Burst mode : Burst Size is 24k bytes.", NULL );
				cli_arg_help(cli, 0,
					"48k", "Burst mode : Burst Size is 48k bytes.", NULL );
				cli_arg_help(cli, 0,
					"96k", "Burst mode : Burst Size is 96k bytes.", NULL );
				return CLI_OK;
			case 6:
				return cli_arg_help(cli, 0,
					"<cr>", "Just press enter to execute command!", NULL );
			default:
				return cli_arg_help(cli, argc > 1, NULL  );
		}
	}

	/*参数数据的合法性检查*/
	if(cmd_port_ingress_rate_arg_check(argv, argc) != CLI_OK)
	{
		return CLI_ERROR;
	}
	switch(argc)
	{
		case 0:
			cs_printf("%s\n", "% Command incomplete.");
			break;
		case 1:
			uni_port_id = atoi(argv[0]);
			port_ingress_rate_get(uni_port_id, &enable, &rate);
			uni_ingress_t uni_ingress;
			uni_ingress.port = uni_port_id;
			uni_ingress.enable = enable;
			uni_ingress.ingress_rate = rate;
			uni_ingress_rate_show(&uni_ingress);			
			break;
		case 3:
			
		case 5:
			uni_port_id = atoi(argv[0]);
			rate = atol(argv[2]);
			
			port_ingress_rate_set(uni_port_id, rate);
			break;
		default:
			cs_printf("%s\n", "there is no command!");
	}

	return CLI_OK;
}
#endif /* #ifdef HAVE_PLATFORM_UNIFY_CMD */

#if (HAVE_GE_RATE_LIMIIT_RESTORE == MODULE_YES)
extern int uni_egress_rate_config_recover(uni_egress_t *uni_egress_p)
{
	int ret = 0;
	int port = 0;
	cs_uint32 rate = 0;
	port = uni_egress_p->port;
	rate = uni_egress_p->egress_rate;

	uni_egress_rate_show(uni_egress_p);
	ret = port_egress_rate_set(port, rate);
	if(CS_E_OK == ret)
	{
		cs_printf("port_egress_rate_set success\n");
	}
	else
	{
		cs_printf("port_egress_rate_set failed\n");
	}
	return ret;
}

extern int uni_egress_rate_tlv_infor_get(int *len, char **value, int *free_need)
{
	int ret = 0;
	uni_egress_t *uni_egress_p = NULL;
	int i = 0;
	int port = 0;
	cs_uint32 rate = 0;
	cs_uint8 enable = 0;
	uni_egress_t uni_egress[UNI_PORT_MAX];
	int count = 0;
	int buf_len = 0;
	
	if(NULL == len)
	{
		goto error;
	}
	else
	{
		*len = 0;
	}

	if(NULL == value)
	{
		goto error;
		
	}
	else
	{
		*value = NULL;
	}
	
	if(NULL == free_need)
	{
		goto error;
	}
	else
	{
		*free_need = 0;
	}

	for(i = 0;i < UNI_PORT_MAX;i++)
	{
		port = i + CS_UNI_PORT_ID1;
		if(port_egress_rate_get(port, &enable, &rate) != CS_E_OK)
		{
			continue;
		}
		else
		{
			#if 0
			cs_printf("port :%d, egress rate :%d\n", port, rate);
			#endif
			if((rate >= UNI_EGRESS_RATE_SAVE)&&(rate <UNI_EGRESS_RATE_MAX ))
			{
				uni_egress[count].port = port;
				uni_egress[count].egress_rate = rate;
				count++;
			}
		}
	}
	if(0 == count)
	{
		ret = 0;
		goto end;
	}
	buf_len = count * sizeof(uni_egress_t);
	uni_egress_p = (uni_egress_t *)iros_malloc(IROS_MID_APP, buf_len);
	memset(uni_egress_p, 0, buf_len);
	memcpy(uni_egress_p, uni_egress, buf_len);
	*len = buf_len;
	*value = (char *)uni_egress_p;
	*free_need = 1;
	
	ret = 0;
	goto end;
	
error:
	ret = -1;
	
end:
	#if 0
	cs_printf("*len :%d\n", *len);
	cs_printf("count :%d\n", count);
	#endif
	return ret;
}

extern int uni_egress_rate_tlv_infor_handle(int len, char *data, int opcode)
{
	int ret = 0;
	uni_egress_t uni_egress[UNI_PORT_MAX];
	uni_egress_t *uni_egress_p = NULL;
	int port_num = 0;
	int i = 0;
	
	if(0 != len)
	{
		//do nothing
	}
	else
	{
		goto error;
	}
	
	if(NULL != data)
	{
		//do nothing
	}
	else
	{
		goto error;
	}
	
	memcpy(uni_egress, data, len);
	port_num = len / sizeof(uni_egress_t);
	#if 1
	cs_printf("port_num :%d\n", port_num);
	#endif
	for(i = 0; i < port_num; i++)
	{
		uni_egress_p = &uni_egress[i];
		if(DATA_RECOVER == opcode)
		{
			uni_egress_rate_config_recover(uni_egress_p);
		}
		else if(DATA_SHOW == opcode)
		{
			uni_egress_rate_show(uni_egress_p);
		}
		else
		{
			cs_printf("in %s\n", __func__);
		}
	}
	
	
	ret = 0;
	goto end;
	
error:
	ret = -1;
	
end:
	return ret;
}

extern int uni_egress_rate_running_config_show(void)
{
	int ret = 0;
	int i = 0;
	int port = 0;
	cs_uint8 enable = 0;
	cs_int32 rate = 0;
	uni_egress_t uni_egress;
	for(i = 0;i < UNI_PORT_MAX;i++)
	{
		port = i + CS_UNI_PORT_ID1;
		if(port_egress_rate_get(port, &enable, &rate) != CS_E_OK)
		{
			continue;
		}
		else
		{
			uni_egress.port = port;
			uni_egress.egress_rate = rate;
			if((rate >= UNI_INGRESS_RATE_SAVE)&&(rate < UNI_INGRESS_RATE_MAX))
			{
				uni_egress_rate_show(&uni_egress);
			}
		}
	}
	return ret;
}

extern int uni_ingress_rate_config_recover(uni_ingress_t *uni_ingress_p)
{
	int ret = 0;
	int port = 0;
	cs_uint32 rate = 0;
	port = uni_ingress_p->port;
	rate = uni_ingress_p->ingress_rate;

	uni_ingress_rate_show(uni_ingress_p);
	ret = port_ingress_rate_set(port, rate);
	if(CS_E_OK == ret)
	{
		cs_printf("port_egress_rate_set success\n");
	}
	else
	{
		cs_printf("port_egress_rate_set failed\n");
	}
	return ret;
}

extern int uni_ingress_rate_tlv_infor_get(int *len, char **value, int *free_need)
{
	int ret = 0;
	uni_ingress_t *uni_ingress_p = NULL;
	int i = 0;
	int port = 0;
	cs_uint8 enable = 0;
	cs_uint32 rate = 0;
	uni_ingress_t uni_ingress[UNI_PORT_MAX];
	int count = 0;
	int buf_len = 0;
	
	if(NULL == len)
	{
		goto error;
	}
	else
	{
		*len = 0;
	}

	if(NULL == value)
	{
		goto error;
		
	}
	else
	{
		*value = NULL;
	}
	
	if(NULL == free_need)
	{
		goto error;
	}
	else
	{
		*free_need = 0;
	}

	for(i = 0;i < UNI_PORT_MAX;i++)
	{
		port = i + CS_UNI_PORT_ID1;
		if(port_ingress_rate_get(port, &enable, &rate) != CS_E_OK)
		{
			continue;
		}
		else
		{
			#if 0
			cs_printf("port :%d, ingress rate :%d\n", port, rate);
			#endif
			if((rate >= UNI_INGRESS_RATE_SAVE)&&(rate < UNI_INGRESS_RATE_MAX))
			{
				uni_ingress[count].port = port;
				uni_ingress[count].ingress_rate = rate;
				count++;
			}
		}
	}
	if(0 == count)
	{
		ret = 0;
		goto end;
	}
	buf_len = count * sizeof(uni_ingress_t);
	uni_ingress_p = (uni_ingress_t *)iros_malloc(IROS_MID_APP, buf_len);
	memset(uni_ingress_p, 0, buf_len);
	memcpy(uni_ingress_p, uni_ingress, buf_len);
	*len = buf_len;
	*value = (char *)uni_ingress_p;
	*free_need = 1;
	
	ret = 0;
	goto end;
	
error:
	ret = -1;
	
end:
	#if 0
	cs_printf("*len :%d\n", *len);
	cs_printf("count :%d\n", count);
	#endif
	return ret;
}

extern int uni_ingress_rate_tlv_infor_handle(int len, char *data, int opcode)
{
	int ret = 0;
	uni_ingress_t uni_ingress[UNI_PORT_MAX];
	uni_ingress_t *uni_ingress_p = NULL;
	int port_num = 0;
	int i = 0;
	
	if(0 != len)
	{
		//do nothing
	}
	else
	{
		goto error;
	}
	
	if(NULL != data)
	{
		//do nothing
	}
	else
	{
		goto error;
	}
	
	memcpy(uni_ingress, data, len);
	port_num = len / sizeof(uni_ingress_t);
	#if 1
	cs_printf("port_num :%d\n", port_num);
	#endif
	for(i = 0; i < port_num; i++)
	{
		uni_ingress_p = &uni_ingress[i];
		if(DATA_RECOVER == opcode)
		{
			uni_ingress_rate_config_recover(uni_ingress_p);
		}
		else if(DATA_SHOW == opcode)
		{
			uni_ingress_rate_show(uni_ingress_p);
		}
		else
		{
			cs_printf("in %s\n", __func__);
		}
	}
	
	
	ret = 0;
	goto end;
	
error:
	ret = -1;
	
end:
	return ret;
}

extern int uni_ingress_rate_running_config_show(void)
{
	int ret = 0;
	int i = 0;
	int port = 0;
	cs_uint8 enable = 0;
	cs_int32 rate = 0;
	uni_ingress_t uni_ingress;
	for(i = 0;i < UNI_PORT_MAX;i++)
	{
		port = i + CS_UNI_PORT_ID1;
		if(port_ingress_rate_get(port, &enable, &rate) != CS_E_OK)
		{
			continue;
		}
		else
		{
			uni_ingress.port = port;
			uni_ingress.ingress_rate = rate;
			if(rate >= UNI_EGRESS_RATE_SAVE)
			{
				uni_ingress_rate_show(&uni_ingress);
			}
		}
	}
	return ret;
}
#endif	/*(HAVE_GE_RATE_LIMIIT_RESTORE == MODULE_YES)*/

#endif/*(HAVE_GE_RATE_LIMIIT_CMD == MODULE_YES)*/

#ifdef HAVE_PORT_MIRROR_CMD
#define L2S_PORT(port) (((port)==CS_UPLINK_PORT) ? SWITCH_UPLINK_PORT : ((port)-1))  /* convert logical port to switch physical port */
#define S2L_PORT(port) (((port)==SWITCH_UPLINK_PORT) ? CS_UPLINK_PORT : ((port)+1))  /* convert switch physical port to logical port */
extern cs_status uni_port_check(cs_port_id_t port);
cs_status cs_mirror_port_set(cs_port_id_t monitor_port, cs_port_id_t mirror_port_rx, cs_port_id_t mirror_port_tx, cs_boolean isolation_enable)
{
	cs_status ret = CS_E_OK;
	
	rtk_port_t mirroring_port;
	rtk_portmask_t mirrored_rx_portmask;
	rtk_portmask_t mirrored_tx_portmask;
	rtk_enable_t enable;
	
	if(CS_E_OK == uni_port_check(monitor_port))
	{
		mirroring_port = L2S_PORT(monitor_port);
	}
	else
	{
		ret = CS_E_ERROR;
		goto end;
	}

	//process rx mirror port
	if(mirror_port_rx == monitor_port)
	{
		mirrored_rx_portmask.bits[0] = 0;
	}
	else
	{
		mirrored_rx_portmask.bits[0] = (1<<L2S_PORT(mirror_port_rx));
	}
	

	//process tx mirror port
	if(mirror_port_tx == monitor_port)
	{
		mirrored_tx_portmask.bits[0] = 0;
	}
	else
	{
		mirrored_tx_portmask.bits[0] = (1<<L2S_PORT(mirror_port_tx));
	}

	if((0 == mirrored_rx_portmask.bits[0])&&(0 == mirrored_tx_portmask.bits[0]))
	{
		mirroring_port = 0;
	}
	
	//set port mirror
	if(RT_ERR_OK != rtk_mirror_portBased_set(mirroring_port, &mirrored_rx_portmask, &mirrored_tx_portmask))
	{
		ret = CS_E_ERROR;
		goto end;
	}

	
	#if 1
	//set port isolation
	{
		rtk_port_t port;
		rtk_portmask_t port_bmp;

		if(monitor_port != mirror_port_rx)
		{
			port = L2S_PORT(mirror_port_rx);
		}
		else
		{
			port = L2S_PORT(mirror_port_tx);
		}
		
		port_bmp.bits[0] = (1<<L2S_PORT(monitor_port));
		
    	if(RT_ERR_OK != rtk_port_isolation_set(port, port_bmp))
    	{
    		ret = CS_E_ERROR;
			goto end;
    	}
	}
	#endif
	
	//process mirror port isolation
	if(isolation_enable)
	{
		enable = ENABLED;
	}
	else
	{
		enable = DISABLED;
	}
	if(RT_ERR_OK != rtk_mirror_portIso_set(enable))
	{
		ret = CS_E_ERROR;
		goto end;
	}
	
	ret = CS_E_OK;
	
end:
	return ret;
}

cs_status cs_mirror_port_get(cs_port_id_t *monitor_port, cs_port_id_t *mirror_port_rx, cs_port_id_t *mirror_port_tx, cs_boolean *isolation_enable)
{
	if((NULL == monitor_port)||(NULL == mirror_port_rx)||(NULL == mirror_port_tx)||(NULL == isolation_enable))
	{
		return CS_E_ERROR;
	}
	
	cs_status ret = CS_E_OK;
	rtk_port_t mirroring_port;
	rtk_portmask_t mirrored_rx_portmask;
	rtk_portmask_t mirrored_tx_portmask;
	rtk_enable_t enable;
	
	if(RT_ERR_OK == rtk_mirror_portBased_get(&mirroring_port, &mirrored_rx_portmask, &mirrored_tx_portmask))
	{
		*monitor_port = S2L_PORT(mirroring_port);
		rtk_port_t mirrored_rx_port = 0;
		rtk_port_t mirrored_tx_port = 0;
		int i = 0;
		int bits_len = 0;
		cs_boolean mirrored_rx_port_found = false;
		cs_boolean mirrored_tx_port_found = false;
		bits_len = sizeof(mirrored_rx_portmask.bits)*8;
		for(i=0; i<bits_len; i++)
		{
			if(mirrored_rx_portmask.bits[0]&(1<<i))
			{
				mirrored_rx_port_found = true;
				break;
			}
		}
		if(true == mirrored_rx_port_found)
		{
			mirrored_rx_port = i;
		}
		else
		{
			mirrored_rx_port = mirroring_port;
		}
		*mirror_port_rx = S2L_PORT(mirrored_rx_port);
		
		for(i=0; i<bits_len; i++)
		{
			if(mirrored_tx_portmask.bits[0]&(1<<i))
			{
				mirrored_tx_port_found = true;
				break;
			}
		}
		if(true == mirrored_tx_port_found)
		{
			mirrored_tx_port = i;
		}
		else
		{
			mirrored_tx_port = mirroring_port;
		}
		*mirror_port_tx = S2L_PORT(mirrored_tx_port);
	}
	else
	{
		ret = CS_E_ERROR;
		goto end;
	}

	if(RT_ERR_OK == rtk_mirror_portIso_get(&enable))
	{
		if(enable)
		{
			*isolation_enable = true;
		}
		else
		{
			*isolation_enable = false;
		}
	}
	else
	{
		ret = CS_E_ERROR;
		goto end;
	}

end:	
	return ret;
}

int cmd_oam_port_mirror_group(struct cli_def *cli, char *command, char *argv[], int argc)
{
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
						"<port_from>","uni port1-4,input one port",NULL);
				break;
			case 2:
				return cli_arg_help(cli, 0,
						"<i>", "ingress direction",
						"<e>", "egress direction",
						"<a>", "all direction",
						NULL);
				break;
			case 3:
				return cli_arg_help(cli, 0,
						"<port_to>","uni port1-4,input one port",NULL);
				break;
			case 4:
				 return cli_arg_help(cli, 0, "1","Add mirror source and destiantion ports",
										     "0","Delete mirror source and destiantion ports",NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
				break;
		}
	}

	if(4 == argc)
	{
		if(1 == atoi(argv[3]))
		{
			char *direction = NULL;
			char directon_ingress[] = "i";
			char directon_egress[] = "e";
			char directon_both[] = "a";
			cs_port_id_t monitor_port;
			cs_port_id_t src_port;
			cs_port_id_t mirror_port_rx;
			cs_port_id_t mirror_port_tx;

			src_port = atoi(argv[0]);
			direction = argv[1];
			monitor_port = atoi(argv[2]);
			if(0 == strncmp(directon_ingress, direction, strlen(directon_ingress)))
			{
				mirror_port_rx = src_port;
				mirror_port_tx = monitor_port;
			}
			else if(0 == strncmp(directon_egress, direction, strlen(directon_egress)))
			{
				mirror_port_rx = monitor_port;
				mirror_port_tx = src_port;
			}
			else if(0 == strncmp(directon_both, direction, strlen(directon_both)))
			{
				mirror_port_rx = src_port;
				mirror_port_tx = src_port;
			}
			else
			{
				cli_print(cli, "wrong input!");
				goto end;
			}
			
			if(CS_E_OK == cs_mirror_port_set(monitor_port, mirror_port_rx, mirror_port_tx, atoi(argv[3])))
			{
				cli_print(cli, "port mirror enable and mirror success, monitor_port :%d", monitor_port);
			}
			else
			{
				cli_print(cli, "cs_mirror_port_set error, monitor_port :%d, mirror_port_rx :%d, mirror_port_tx :%d", monitor_port, mirror_port_rx, mirror_port_tx);
			}
		}
		else
		{
			cs_port_id_t monitor_port;
			cs_port_id_t mirror_port_rx;
			cs_port_id_t mirror_port_tx;
			monitor_port = CS_UNI_PORT_ID1;
			mirror_port_rx = monitor_port;
			mirror_port_tx = monitor_port;
			if(CS_E_OK == cs_mirror_port_set(monitor_port, mirror_port_rx, mirror_port_tx, atoi(argv[3])))
			{
				cli_print(cli, "port mirror disabled success");
			}
			else
			{
				cli_print(cli, "port mirror disabled failed, monitor_port :%d, mirror_port_rx :%d, mirror_port_tx :%d", monitor_port, mirror_port_rx, mirror_port_tx);
			}	
		}
	}
	else
	{
		cli_print(cli, "wrong input!");
	}

end:	
	return CLI_OK;
}

int cmd_port_mirror_show(struct cli_def *cli, char *command, char *argv[], int argc)
{
	cs_port_id_t monitor_port;
	cs_port_id_t mirror_port_rx;
	cs_port_id_t mirror_port_tx;
	cs_boolean isolation_enable;
	
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
						"<cr>", "Enter to execuse the command",NULL);
				break;
			default:
				return cli_arg_help(cli, argc > 1, NULL);
				break;
		}
	}

	if(CS_E_OK == cs_mirror_port_get(&monitor_port, &mirror_port_rx, &mirror_port_tx, &isolation_enable))
	{
		if((monitor_port == mirror_port_rx)&&(monitor_port == mirror_port_tx))
		{
			cs_printf("there is no port mirror!\n");
		}
		else
		{
			cs_printf("monitor_port :%d\n", monitor_port);
			if(monitor_port != mirror_port_rx)
			{
				if(CS_UPLINK_PORT == mirror_port_rx)
				{
					cs_printf("mirror direction :%s\n", "egress");
				}
				else
				{
					cs_printf("mirror_port_rx :%d\n", mirror_port_rx);
				}
			}

			if(monitor_port != mirror_port_tx)
			{
				if(CS_UPLINK_PORT == mirror_port_tx)
				{
					cs_printf("mirror direction :%s\n", "ingress");
				}
				else
				{
					cs_printf("mirror_port_tx :%d\n", mirror_port_tx);
				}
			}
		}
	}
	else
	{
		cs_printf("cs_mirror_port_get error!\n");
	}
	
	return CLI_OK;
}

int cmd_port_mirror_clear(struct cli_def *cli, char *command, char *argv[], int argc)
{
	cs_port_id_t monitor_port;
	cs_port_id_t mirror_port_rx;
	cs_port_id_t mirror_port_tx;
	
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				cli_arg_help(cli, 0,
						"<cr>", "Enter to execuse the command",NULL);
				break;
			default:
				return cli_arg_help(cli, argc > 1, NULL);
				break;
		}
	}

	monitor_port = CS_UNI_PORT_ID1;
	mirror_port_rx = monitor_port;
	mirror_port_tx = monitor_port;
	if(CS_E_OK == cs_mirror_port_set(monitor_port, mirror_port_rx, mirror_port_tx, false))
	{
		cli_print(cli, "success");
	}
	else
	{
		cli_print(cli, "cs_mirror_port_set error, monitor_port :%d, mirror_port_rx :%d, mirror_port_tx :%d", monitor_port, mirror_port_rx, mirror_port_tx);
	}
	
	return CLI_OK;
}
#endif	/*(#ifdef HAVE_PORT_MIRROR_CMD)*/

#ifdef HAVE_GWD_PON_LASER_CONTROL
cs_status onu_laser_control_set_adapt(cs_uint16 action)
{
	cs_uint8 onuId[CS_MACADDR_LEN] = {0};

	memset(onuId, 0xff, sizeof(onuId));
	return ctc_oam_onu_laser_control_set_adapt(action, onuId, 0);
}

int cmd_laser_control(struct cli_def *cli, char *command, char *argv[], int argc)
{
	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<action>", "0:(re-enable the Tx power supply),1-65534:(duration during which optical Tx,power supply is shut down, sec),65535:(permanently shutdown)",
				NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	if(1 == argc)
	{
		cs_uint16 action = 0;
		action = atoi(argv[0]);
		
		if(CS_E_OK == onu_laser_control_set_adapt(action))
		{
			cs_printf("success!\n");
		}
		else
		{
			cs_printf("ctc_oam_onu_laser_control_set_adapt error\n");
		}
	}
	else
	{
		cli_print(cli, "wrong input!");
	}
	return CLI_OK;
}
#endif	/*(#ifdef HAVE_GWD_PON_LASER_CONTROL)*/

#ifdef HAVE_GPIO_DEBUG_CMD
int cmd_onu_gpio_direction(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int num;
	gpio_mode_t direct;

	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<0-15>", "gpio number",
				NULL);
			case 2:
				return cli_arg_help(cli, 0,
				"[0|1]", "0:output,1:input",
				NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	if(1 == argc)
	{
		char c_dir[80]= "";
		num = atoi(argv[0]);
		cs_gpio_mode_get(num,&direct);
		switch (direct)
		{
			case GPIO_OUTPUT:
				strcpy(c_dir, "output");
				break;
			case GPIO_INPUT:
				strcpy(c_dir, "input");
				break;
			case GPIO_INPUT_FALLING:
				strcpy(c_dir, "failling");
				break;
			case GPIO_INPUT_RISING:
				strcpy(c_dir, "rising");
				break;
			default:
				strcpy(c_dir, "unknown");
				break;
		}
		cli_print(cli, "current direction is %s", c_dir);
	}
	else if(2 == argc)
	{
		num = atoi(argv[0]);
		direct = atoi(argv[1]);

		if(num < 0 || num > 15)
		{
			return CLI_ERROR_ARG;
		}

		if(direct > 1)
		{
			return CLI_ERROR_ARG;
		}
		
		if(cs_gpio_mode_set(num, direct) != EPON_RETURN_SUCCESS)
		{
			cli_print(cli, "gpio direction set fail!");
		}

	}
	else
	{
		cli_print(cli, "%% Invalid input.");
	}

	return CLI_OK;
}

int cmd_onu_gpio_status(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int num;
	unsigned char act;

	// deal with help
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<0-15>", "gpio number",
				NULL);
			case 2:
				return cli_arg_help(cli, 0,
				"[0|1]", "0:low level,1:high level",
				NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	if(1 == argc)
	{
		num = atoi(argv[0]);
		if(cs_gpio_read(num, &act) == EPON_RETURN_SUCCESS)
		{
			cli_print(cli, "current level is %s", act?"high":"low");
		}
		else
		{
			cli_print(cli, "gpio status get fail!");
		}
	}
	else if(2 == argc)
	{
		num = atoi(argv[0]);
		act = atoi(argv[1]);

		if(num < 0 || num > 15)
		{
			return CLI_ERROR_ARG;
		}

		if(act > 1)
		{
			return CLI_ERROR_ARG;
		}

		if(cs_gpio_write(num, act) != EPON_RETURN_SUCCESS)
		{
			cli_print(cli, "gpio status set fail!");
		}
	}
	else
	{
		cli_print(cli, "%% Invalid input.");
	}

	return CLI_OK;
}
#endif	/*(#ifdef HAVE_GPIO_DEBUG_CMD)*/

#ifdef HAVE_EVENT_SHOW_CMD
int cmd_event_show(struct cli_def *cli, char *command, char *argv[], int argc)
{
	char * pbuf = NULL;

	if(CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
					"<1-64>", "msg slot number", NULL);
				break;
			default:
				return cli_arg_help(cli, argc > 1, NULL	);
				break;
		}
	}

	if(0 == argc)
	{
		int start = 0;
		int flag = 0;

		for(start = 0; start < GW_MAX_EVENT_LOG_NUM; start++)
		{
			pbuf = gw_log_get_record(start);

			if(pbuf)
			{
				flag = 1;
				cli_print(cli, "%s", pbuf);
				cs_thread_delay(100);												 
			}
		}
		if(!flag)
		{
			cli_print(cli, "empty slot!\r\n");
		}
	}
	else if(1 == argc)
	{
		int slot = 0;

		slot = atoi(argv[0]);
		pbuf = gw_log_get_record(slot-1);

		if(pbuf)
		{
			cli_print(cli, "%s", pbuf);
		}
		else
		{
			cli_print(cli, "empty slot!\r\n");
		}
	}
	else
	{
		cli_print(cli, "wrong input!");
	}

	return CLI_OK;
}
#endif /* #ifdef HAVE_EVENT_SHOW_CMD */

#ifdef HAVE_DEVICE_NAME_CMD
void show_device_name(struct cli_def *cli)
{
	long lRet = GWD_RETURN_OK;
           
	lRet = Onu_Sysinfo_Get();
	if (lRet != GWD_RETURN_OK)
	{
		cli_print(cli, "  Get product information from flash with error.\r\n");
	}
	else
	{
		cli_print(cli,  "    DeiveName        : %s", onu_system_info_total.device_name);
	}
}

int cmd_device_name(struct cli_def *cli, char *command, char *argv[], int argc)
{
	if(CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
			case 1:
				cli_arg_help(cli, 0,
				"<cr>", "Just press enter to execute command!",
				NULL);
				return cli_arg_help(cli, 0,
					"<1-128>", "device name", NULL);
				break;
			default:
				return cli_arg_help(cli, argc > 1, NULL	);
				break;
		}
	}

	if(0 == argc)
	{
		show_device_name(cli);
	}
	else if(1 == argc)
	{
		long lRet = GWD_RETURN_OK;
           
		lRet = Onu_Sysinfo_Get();
		if (lRet == GWD_RETURN_OK)
		{
			strcpy(onu_system_info_total.device_name,argv[0]);
			lRet = Onu_Sysinfo_Save();
		}

		if(lRet != GWD_RETURN_OK)
		{
			cli_print(cli, "operation error.\r\n");
			
		}
		else
		{
			cli_print(cli, "set device name success.\r\n");
		}
	}
	else
	{
		cli_print(cli, "wrong input!");
	}

	return CLI_OK;
}

int cmd_show_device_name(struct cli_def *cli, char *command, char *argv[], int argc)
{
	if(CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<cr>", "Just press enter to execute command!",
				NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL	);
				break;
		}
	}

	if(0 == argc)
	{
		show_device_name(cli);
	}
	else
	{
		cli_print(cli, "wrong input!");
	}

	return CLI_OK;
}

#endif /* #ifdef HAVE_DEVICE_NAME_CMD */
#ifdef HAVE_VLAN_DOT1Q_CMD

typedef enum vlan_port_mode_s{
	TAGGED_PORT = 1,
	UNTAGGED_PORT = 2
}vlan_port_mode_t;

typedef struct vlan_entry_s
{
	cs_boolean valid;
	cs_uint16  vid;
	cs_uint16  tagged_port_mask;
	cs_uint16  untagged_port_mask;
}vlan_entry_t;

static vlan_entry_t g_dot1q_vlan_table[__VLAN_MAX];

cs_status vlan_dot1q_enable_get(cs_boolean *enable)
{
	cs_boolean    dot1q_enable = true;
	cs_uint8      mode = 0;
	cs_port_id_t  port_id = 0;
	cs_uint8      port_num = UNI_PORT_MAX;

	for(port_id = CS_UNI_PORT_ID1;port_id <= port_num;port_id++)
	{
		mode = 0;
		if(CS_E_OK == ctc_oam_onu_vlan_mode_get_adapt(port_id,&mode))
		{
			if(SDL_VLAN_MODE_TRUNK != mode)
			{	
				dot1q_enable = false;
				break;
			}
		}
		else
		{
			cs_printf("dot1q_enable_get error!\n");
			return CS_E_ERROR;
		}
	}
	
	*enable = dot1q_enable;

	return CS_E_OK;
}

cs_status vlan_dot1q_enable_set(cs_boolean enable)
{
	cs_port_id_t  port_id = 0;
	cs_uint8      max_uni_port=UNI_PORT_MAX;
	oam_ctc_onu_vlan_tag_t defVlan;
	oam_ctc_onu_vlan_tag_t vlan;

	memset(&defVlan,0,sizeof(defVlan));

	if(enable)
	{
		defVlan.tag = 1;

		for(port_id = CS_UNI_PORT_ID1;port_id <= max_uni_port;port_id++)
		{
			if(CS_E_OK != ctc_oam_onu_trunk_vlan_set_adapt(port_id,defVlan,0,&vlan))
			{
				cs_printf("operation error!");
				return CS_E_ERROR;
			}		
		}
	}
	else
	{
		for(port_id = CS_UNI_PORT_ID1;port_id <= max_uni_port;port_id++)
		{
			if(CS_E_OK != ctc_oam_onu_transparent_vlan_set_adapt(port_id))
			{
				cs_printf("operation error!");
				return CS_E_ERROR;
			}
		}
	}

	return CS_E_OK;
}

cs_boolean dot1q_vlan_already_exist(cs_uint16 vid)
{
	cs_uint8 i = 0;

	for(i = 0;i < __VLAN_MAX;i++)
	{
		if(g_dot1q_vlan_table[i].valid)
		{
			if(g_dot1q_vlan_table[i].vid == vid)
			{
				return true;
			}
		}
	}

	return false;
}

cs_status dot1q_vlan_table_add_vlan(cs_uint16 vid)
{
	cs_uint8 i = 0;

	for(i = 1;i < __VLAN_MAX;i++)
	{
		if(!g_dot1q_vlan_table[i].valid)
		{
			g_dot1q_vlan_table[i].vid = vid;
			g_dot1q_vlan_table[i].valid = 1;
			return CS_E_OK;
		}
	}

	cs_printf("dot1q_vlan_table is full\n");
	return CS_E_ERROR;
}

cs_status dot1q_vlan_table_del_vlan(cs_uint16 vid)
{
	cs_uint8 i = 0;

	for(i = 1;i < __VLAN_MAX;i++)
	{
		if(g_dot1q_vlan_table[i].valid && g_dot1q_vlan_table[i].vid == vid)
		{
			g_dot1q_vlan_table[i].valid = 0;
			g_dot1q_vlan_table[i].tagged_port_mask = 0;
			g_dot1q_vlan_table[i].untagged_port_mask = 0;
			return CS_E_OK;
		}
	}

	cs_printf("vlan %d not found\n",vid);
	return CS_E_ERROR;
}


cs_status dot1q_vlan_table_add_port(cs_uint16 vid,cs_port_id_t port,vlan_port_mode_t mode)
{
	cs_uint8 i = 0;
	cs_status ret = CS_E_ERROR;

	if( !(GW_VLAN_LAS < vid && vid <=GW_VLAN_MAX) 
		|| !(CS_UNI_PORT_ID1 <= port && port <= UNI_PORT_MAX)
		|| !(TAGGED_PORT == mode || UNTAGGED_PORT == mode))
	{
		return CS_E_PARAM;
	}

	/* 将port加入VLAN中 */
	for(i = 0;i < __VLAN_MAX;i++)
	{
		if( g_dot1q_vlan_table[i].valid && g_dot1q_vlan_table[i].vid == vid)
		{
			if( TAGGED_PORT == mode)
			{
				g_dot1q_vlan_table[i].tagged_port_mask |= (0x1 << (L2P_PORT(port)));
			}
			else if(UNTAGGED_PORT == mode)
			{
				g_dot1q_vlan_table[i].untagged_port_mask |= (0x1 << (L2P_PORT(port)));
			}
			ret = CS_E_OK;
			break;
		}
	}

	/* 如果是untag port 还应从其他VLAN中删掉该port*/	
	if(CS_E_OK == ret && UNTAGGED_PORT == mode)
	{
		for(i = 0;i < __VLAN_MAX;i++)
		{
			if( g_dot1q_vlan_table[i].valid && g_dot1q_vlan_table[i].vid != vid)
			{
				if(g_dot1q_vlan_table[i].untagged_port_mask &(0x1 << (L2P_PORT(port))))
				{
					g_dot1q_vlan_table[i].untagged_port_mask &= (~(0x1 << (L2P_PORT(port))));
				}
				break;
			}
		}
	}

	return ret;
}

void dot1q_vlan_table_clear_port(void)
{
	cs_uint8 i = 0;

	g_dot1q_vlan_table[0].valid = 1;
	g_dot1q_vlan_table[0].vid = 1;
	g_dot1q_vlan_table[0].untagged_port_mask = 0x0f;
	g_dot1q_vlan_table[0].tagged_port_mask = 0x00;
	
	for(i = 1;i < __VLAN_MAX;i++)
	{
		if(g_dot1q_vlan_table[i].valid)
		{
			g_dot1q_vlan_table[i].untagged_port_mask = 0;
			g_dot1q_vlan_table[i].tagged_port_mask = 0;
		}
	}
}


cs_status dot1q_vlan_table_sync_from_hw()
{
	cs_callback_context_t		  context;
	cs_dev_id_t 				  device = 0;
	cs_llid_t					  llidport = 0;
	cs_port_id_t				  port = 0;
	cs_sdl_vlan_tag_t  			  def_vlan;
	cs_sdl_vlan_mode_t 			  vlan_mode = 0;
	cs_sdl_vlan_cfg_t  	          vlan_cfg[__VLAN_RULE_PER_PORT_MAX];
	cs_uint16		   			  cfg_nums = __VLAN_RULE_PER_PORT_MAX;
	
	memset(&def_vlan,0,sizeof(cs_sdl_vlan_tag_t));
	memset(&vlan_cfg,0,sizeof(cs_sdl_vlan_cfg_t)*__VLAN_RULE_PER_PORT_MAX);

	dot1q_vlan_table_clear_port();
	
	for(port = CS_UNI_PORT_ID1;port <= UNI_PORT_MAX;port++)
	{
		if(CS_E_OK == epon_request_onu_vlan_get(context, device, llidport,port,&def_vlan,&vlan_mode,vlan_cfg,&cfg_nums))
		{
			int       i = 0;
			cs_uint16 vid = 0;

			if(SDL_VLAN_MODE_TRUNK != vlan_mode)
			{
				return CS_E_ERROR;
			}

			if(dot1q_vlan_already_exist(def_vlan.vid))
			{
				// do nothing
			}
			else
			{
				if(CS_E_OK != dot1q_vlan_table_add_vlan(def_vlan.vid))
				{
					return CS_E_ERROR;
				}
			}

			dot1q_vlan_table_add_port(def_vlan.vid,port,UNTAGGED_PORT);
			
			for(i = 0;i < cfg_nums;i++)
			{
				vid = vlan_cfg[i].c_vlan.vid;
				
				if(dot1q_vlan_already_exist(vid))
				{
					// do nothing
				}
				else
				{
					if(CS_E_OK != dot1q_vlan_table_add_vlan(vid))
					{
						return CS_E_ERROR;
					}
				}

				dot1q_vlan_table_add_port(vid,port,TAGGED_PORT);
			}
		}
		else
		{
			cs_printf("epon_request_onu_vlan_get error\n");
			return CS_E_ERROR;
		}
	}
	return CS_E_OK;
}

cs_status vlan_port_add(cs_uint16 vid,cs_port_id_t port,vlan_port_mode_t mode)
{
	cs_callback_context_t         context;
    cs_int32                      device_id = 0;
    cs_int32                      llidport = 0;
	
	if( !(GW_VLAN_LAS < vid && vid <=GW_VLAN_MAX) 
		|| !(CS_UNI_PORT_ID1 <= port && port <= UNI_PORT_MAX)
		|| !(TAGGED_PORT == mode || UNTAGGED_PORT == mode))
	{
		return CS_E_PARAM;
	}

	if(UNTAGGED_PORT == mode)
	{
		return epon_request_vlan_add_untag_port(context,device_id,llidport,port,vid);
	}
	else
	{
		return epon_request_vlan_add_tag_port(context,device_id,llidport,port,vid);
	}
}

cs_status port_mask_to_string(cs_uint16 mask,char *str,cs_uint32 strlen)
{
	unsigned long long_mask = mask << 1;
	if(NULL == str)
	{
		return CS_E_PARAM;
	}

	if(CS_E_ERROR !=gwd_port_list_long_to_string(long_mask,str,strlen))
	{
		return CS_E_OK;
	}

	return CS_E_ERROR;
}

cs_status vlan_dot1q_add(cs_uint16 new_vlan)
{
	cs_boolean    dot1q_enable = 0;
	
	if(CS_E_OK == vlan_dot1q_enable_get(&dot1q_enable))
	{
		if(dot1q_enable)
		{
			if( !(GW_VLAN_LAS < new_vlan && new_vlan <=GW_VLAN_MAX) )
			{
				cs_printf("vlan error\n");
				return CS_E_PARAM;
			}
			
			if(CS_E_OK != dot1q_vlan_table_sync_from_hw())
			{
				
				cs_printf("dot1q_vlan_table_sync_from_hw error\n");
				return CS_E_ERROR;
			}

			if(dot1q_vlan_already_exist(new_vlan))
			{
				cs_printf("vlan %d already exist\n",new_vlan);
				return CS_E_PARAM;
			}

			if(CS_E_OK == dot1q_vlan_table_add_vlan(new_vlan))
			{
				cs_printf("add vlan %d\n",new_vlan);
				return CS_E_OK;
			}
			else
			{
				cs_printf("operation error\n");
				return CS_E_ERROR;
			}
		}
		else
		{
			cs_printf("dot1q is disable,you should enable dot1q first\n");
			return CS_E_ERROR;
		}
	}
	else
	{
		cs_printf("operation error\n");
		return CS_E_ERROR;
	}
	
}

cs_status vlan_dot1q_del(cs_uint16 vid)
{
	cs_boolean dot1q_enable = 0;
	
	if(CS_E_OK == vlan_dot1q_enable_get(&dot1q_enable))
	{
		if(!dot1q_enable)
		{
			cs_printf("dot1q is disable,you should enable dot1q first\n");
			return CS_E_ERROR;
		}
	}
	
	if( !(GW_VLAN_LAS < vid && vid <=GW_VLAN_MAX) )
	{
		cs_printf("vlan error\n");
		return CS_E_PARAM;
	}
	
	if(CS_E_OK != dot1q_vlan_table_sync_from_hw())
	{
		
		cs_printf("dot1q_vlan_table_sync_from_hw error\n");
		return CS_E_ERROR;
	}

	if(!dot1q_vlan_already_exist(vid))
	{
		cs_printf("vlan %d don't exist\n",vid);
		return CS_E_PARAM;
	}

	if(CS_E_OK == dot1q_vlan_table_del_vlan(vid)
		&& CS_E_OK == epon_request_vlan_del(vid))
	{
		cs_printf("del vlan %d\n",vid);
		return CS_E_OK;
	}
	else
	{
		cs_printf("operation error\n");
		return CS_E_ERROR;
	}
}

cs_status vlan_dot1q_port_add(cs_uint16 vlan_id,cs_uint32 *port_array,cs_int32 port_num,vlan_port_mode_t tag_mode)
{
	cs_boolean dot1q_enable = 0;

	if(CS_E_OK == vlan_dot1q_enable_get(&dot1q_enable))
	{
		if(!dot1q_enable)
		{
			cs_printf("dot1q is disable,you should enable dot1q first\n");
			return CS_E_OK;
		}
	}
	
	if( !(GW_VLAN_LAS < vlan_id && vlan_id <=GW_VLAN_MAX) )
	{
		cs_printf("vlan error\n");
		return CS_E_PARAM;
	}

	if(!(TAGGED_PORT == tag_mode || UNTAGGED_PORT == tag_mode))
	{
		cs_printf("tag mode error\n");
		return CS_E_PARAM;
	}
	
	if(CS_E_OK != dot1q_vlan_table_sync_from_hw())
	{
		
		cs_printf("dot1q_vlan_table_sync_from_hw error\n");
		return CS_E_ERROR;
	}

	if(dot1q_vlan_already_exist(vlan_id))
	{
		cs_uint8   i = 0;
		cs_port_id_t uni_port_id = 0;
		
		for(i=0;i < port_num;i++)
		{
			uni_port_id = port_array[i];
			
			if(uni_port_id > UNI_PORT_MAX)
			{
				cs_printf("port_id %d error\n",uni_port_id);
				return CS_E_PARAM;
			}

			if(CS_E_OK != vlan_port_add(vlan_id,uni_port_id,tag_mode))
			{
				return CS_E_ERROR;
			}
		}
		
	}
	else
	{
		cs_printf("vlan %d not exist!\n",vlan_id);
		return CS_E_ERROR;
	}

	return CS_E_OK;
}

cs_status vlan_dot1q_port_del(cs_uint16 vlan_id,cs_uint32 *port_array,cs_int32 port_num)
{
	cs_boolean dot1q_enable = 0;

	if(CS_E_OK == vlan_dot1q_enable_get(&dot1q_enable))
	{
		if(!dot1q_enable)
		{
			cs_printf("dot1q is disable,you should enable dot1q first\n");
			return CS_E_OK;
		}
	}
	
	if( !(GW_VLAN_LAS < vlan_id && vlan_id <=GW_VLAN_MAX) )
	{
		cs_printf("vlan error\n");
		return CS_E_PARAM;
	}
	
	if(CS_E_OK != dot1q_vlan_table_sync_from_hw())
	{
		
		cs_printf("dot1q_vlan_table_sync_from_hw error\n");
		return CS_E_ERROR;
	}

	if(dot1q_vlan_already_exist(vlan_id))
	{
	    cs_callback_context_t         context;
	    cs_int32                      device_id = 0;
	    cs_int32                      llidport = 0;
		cs_port_id_t 	 			  port_id = 0;
		cs_uint8 					  i = 0;

		for(i = 0;i < port_num;i++)
		{
			port_id = port_array[i];
			if(CS_E_OK != epon_request_vlan_del_from_port(context,device_id,llidport,port_id,vlan_id))
			{
				return CS_E_ERROR;
			}
		}
	}
	else
	{
		cs_printf("vlan %d not exist!\n",vlan_id);
		return CS_E_ERROR;
	}

	return CS_E_OK;
}

void vlan_dot1q_table_show(struct cli_def *cli)
{
	cs_uint8 i = 0;
	char untagged_ports[50];
	char tagged_ports[50];
	cs_boolean    dot1q_enable = 0;

	if(CS_E_OK == vlan_dot1q_enable_get(&dot1q_enable))
	{
		if(dot1q_enable)
		{
			if(CS_E_OK == dot1q_vlan_table_sync_from_hw())
			{
				cli_print(cli, "%-9s%-35s %-30s", "Vlan ID", "tag-portlist       ", "untag-portlist     ");
			    cli_print(cli, "-------------------------------------------------------------------------------------");      
				for(i = 0;i < __VLAN_MAX;i++)
				{
					if(g_dot1q_vlan_table[i].valid)
					{
						if(g_dot1q_vlan_table[i].tagged_port_mask || g_dot1q_vlan_table[i].untagged_port_mask)
						{
							memset(untagged_ports,0,sizeof(untagged_ports));
							memset(tagged_ports,0,sizeof(tagged_ports));
							port_mask_to_string(g_dot1q_vlan_table[i].tagged_port_mask,tagged_ports,sizeof(tagged_ports));
							port_mask_to_string(g_dot1q_vlan_table[i].untagged_port_mask,untagged_ports,sizeof(untagged_ports));
							cli_print(cli, "%-9d%-35s %-30s", g_dot1q_vlan_table[i].vid, tagged_ports, untagged_ports);
						}
					}
				}
				cli_print(cli,"-------------------------------------------------------------------------------------");
				
			}
			else
			{
				cli_print(cli,"dot1q_vlan_table_sync_from_hw error\n");
			}
		}
		else
		{
			cli_print(cli,"dot1q is disable,you should enable dot1q first");
		}
	}

}

int cmd_vlan_dot1q(struct cli_def *cli, char *command, char *argv[], int argc)
{
	if(CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
			case 1:
				cli_arg_help(cli, 0,
				"[0|1]", "0:disable,1:enable",
				NULL);
				return cli_arg_help(cli, 0,
				"<cr>", "Show dot1q config!",
				NULL);
			case 2:
				return cli_arg_help(cli, 0,
				"<cr>", "Just press enter to execute command!",
				NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL	);
				break;
		}
	}

	if(0 == argc)
	{
		cs_boolean    dot1q_enable = 0;
	
		if(CS_E_OK == vlan_dot1q_enable_get(&dot1q_enable))
		{
			cli_print(cli,"vlan dot1q : %s",dot1q_enable ? "enable" : "disable");
		}
		else
		{
			cli_print(cli,"operation error\n");
		}
	}
	else if(1 == argc)
	{
		cs_uint32 enable = atoi(argv[0]);
		
		if(CS_E_OK != vlan_dot1q_enable_set(enable))
		{
			cli_print(cli,"operation error\n");
			return CLI_ERROR;
		}
	}
	else
	{
		cli_print(cli, "wrong input!");
	}

	return CLI_OK;
}

int cmd_vlan_dot1q_add(struct cli_def *cli, char *command, char *argv[], int argc)
{
	if(CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<2-4094>", "new vlan",
				NULL);
			case 2:
				return cli_arg_help(cli, 0,
				"<cr>", "Just press enter to add vlan!",
				NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL	);
				break;
		}
	}
	
	if(1 == argc)
	{
		cs_uint32      new_vlan=0;

		new_vlan = atoi(argv[0]);
	
		if(CS_E_OK == vlan_dot1q_add(new_vlan))
		{
			return CLI_OK;
		}
		else
		{
			return CLI_ERROR;
		}
	}
	else
	{
		cli_print(cli, "wrong input!");
	}

	return CLI_OK;
}

int cmd_vlan_dot1q_del(struct cli_def *cli, char *command, char *argv[], int argc)
{
	if(CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<2-4094>", "Please input the Vlan Vid",
				NULL);
			case 2:
				return cli_arg_help(cli, 0,
				"<cr>", "Just press enter to del vlan!",
				NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL	);
				break;
		}
	}

	
	
	if(1 == argc)
	{
		cs_uint32      vid=0;
	
		vid = atoi(argv[0]);

		if(CS_E_OK == vlan_dot1q_del(vid))
		{
			return CLI_OK;
		}
		else
		{
			return CLI_ERROR;
		}
	}
	else
	{
		cli_print(cli, "wrong input!");
	}

	return CLI_OK;
}

int cmd_vlan_dot1q_port_add(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
		case 1:
    		return cli_arg_help(cli, 0, "<2-4094>  Please input the Vlan Vid(2-4094)", NULL);
		case 2:
			return cli_arg_help(cli, 0,"<port_list>  Please input the port number",NULL);
		case 3:
			cli_arg_help(cli, 0,"Add 802.1Q Vlan port, 1: Tagged",NULL);
			return cli_arg_help(cli, 0,"Add 802.1Q Vlan port, 2: Untagged",
                                    NULL);
		default:
			return cli_arg_help(cli, argc > 1, NULL	);
		}
	}

	if ( 3 == argc )
	{
		cs_uint32 vlan_id = 0;
		vlan_port_mode_t tag_mode = 0;

		cs_int32 port_num = UNI_PORT_MAX;
		cs_int32 port_array[UNI_PORT_MAX] = {0};
		int ret = 0;

		vlan_id = atoi(argv[0]);
		tag_mode = atoi(argv[2]);
		
		if(CS_E_OK == gwd_parse_num_list_from_str(argv[1],port_array, &port_num))
		{
			ret = vlan_dot1q_port_add(vlan_id,port_array,port_num,tag_mode);

			if(CS_E_OK != ret)
			{
				return CLI_ERROR;
			}
		}
		else
		{
			return CLI_ERROR;
		}
	}
    else
    {
		cli_print(cli, "wrong input!");
		return CLI_ERROR;
    }

    return CLI_OK;
}


int cmd_vlan_dot1q_port_del(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int ret = CLI_OK;
	
    if (CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
		case 1:
    		return cli_arg_help(cli, 0, "<2-4094>  Please input the Vlan Vid(2-4094)", NULL);
		case 2:
			return cli_arg_help(cli, 0,"<port_list>  Please input the port number",NULL);
		default:
			return cli_arg_help(cli, argc > 1, NULL	);
		}
	}

	if ( 2 == argc )
	{
		cs_uint32 					  vlan_id = 0;
		cs_int32 					  port_num = UNI_PORT_MAX;
		cs_int32 					  port_array[UNI_PORT_MAX] = {0};
		
		vlan_id = atoi(argv[0]);

		if(CS_E_OK == gwd_parse_num_list_from_str(argv[1],port_array, &port_num))
		{
			ret = vlan_dot1q_port_del(vlan_id,port_array,port_num);

			if(CS_E_OK != ret)
			{
				return CLI_ERROR;
			}
		}
		else
		{
			return CLI_ERROR;
		}
	}
    else
    {
		cli_print(cli, "wrong input!");
		return CLI_ERROR;
    }

    return ret;
}

int cmd_vlan_dot1q_show(struct cli_def *cli, char *command, char *argv[], int argc)
{
	if(CLI_HELP_REQUESTED)
	{
		switch (argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<cr>", "Show dot1q config!",
				NULL);
			default:
				return cli_arg_help(cli, argc > 1, NULL	);
				break;
		}
	}

	if(0 == argc)
	{
		vlan_dot1q_table_show(cli);
	}
	else
	{
		cli_print(cli, "wrong input!");
	}

	return CLI_OK;
}


#endif /* #ifdef HAVE_VLAN_DOT1Q_CMD */

#ifdef HAVE_PLATFORM_UNIFY_CMD
int cmd_ip_address(struct cli_def *cli, char *command, char *argv[], int argc)
{
	cs_status   ret = CS_E_OK;
	cs_uint32	device_ip = 0;
	cs_uint32	device_mask = 0;
	cs_uint32	device_gateway = 0;
		
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				cli_arg_help(cli, 0,
				"<cr>", "show ip configure",
				NULL);
				return cli_arg_help(cli, 0,
				"<A.B.C.D/M>", "IP address and length of mask",
				NULL);
				break;
			case 2:
				return cli_arg_help(cli, 0,
				"<A.B.C.D>", "GateWay",
				NULL);
				break;
			default:
			return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	if(argc > 2)
	{
		cli_print(cli, "%% Invalid input");
		return CLI_ERROR_ARG;
	}

	if(argc >= 0)
	{	
		ret = epon_request_onu_ip_config_get(&device_ip, &device_mask, &device_gateway);

		if(CS_E_OK == ret)
		{
			if(0 == argc)
			{
				struct sockaddr_in addrp[3];
		
				addrp[0].sin_addr.s_addr = htonl(device_ip);
				addrp[1].sin_addr.s_addr = htonl(device_mask); 
				addrp[2].sin_addr.s_addr = htonl(device_gateway);
				
				cli_print(cli, "ip from network card:");		
				cli_print(cli, "=========================================================================");
				cli_print(cli, "IP - %s", inet_ntoa(addrp[0].sin_addr));
				cli_print(cli, "IP Submask - %s", inet_ntoa(addrp[1].sin_addr));
				cli_print(cli, "IP Gateway - %s", inet_ntoa(addrp[2].sin_addr));
				cli_print(cli, "=========================================================================");
				return CLI_OK;
			}
		}
		else
		{
			return CLI_ERROR;
			cli_print(cli, "ip address fail");
		}
		
	}
	
	if(argc >= 1)
	{
		char *delim = "/";
	    char *pstr = argv[0];
		cs_uint8 mask_bits = 0;

		pstr = strtok(pstr, delim);
		device_ip = ntohl(inet_addr(pstr));

		pstr = strtok(NULL, delim);
		mask_bits = atoi(pstr);
		device_mask = 0xffffffff;
		device_mask = device_mask >> (IPV4_MASK_LEN - mask_bits);
		device_mask = device_mask << (IPV4_MASK_LEN - mask_bits);
	}
	
	if(argc >= 2)
	{
		device_gateway = ntohl(inet_addr(argv[1]));
	}

	ret = epon_request_onu_ip_config_set(device_ip, device_mask, device_gateway);
	if(CS_E_OK == ret)
	{
		cli_print(cli, "ip config success");
	}
	else
	{
		cli_print(cli, "ip config failed");
		return CLI_ERROR;
	}
	
	return CLI_OK;
}

int cmd_ip_address_clear(struct cli_def *cli, char *command, char *argv[], int argc)
{
	cs_status   ret = CS_E_OK;
		
	if(CLI_HELP_REQUESTED)
	{
		switch(argc)
		{
			case 1:
				return cli_arg_help(cli, 0,
				"<cr>", "set ip address to default config",
				NULL);
				break;
			default:
				return cli_arg_help(cli, argc > 1, NULL);
		}
	}

	if(argc > 1)
	{
		cli_print(cli, "%% Invalid input");
		return CLI_ERROR_ARG;
	}

	if(0 == argc)
	{
		ipintf_info_t eth_info;
		
		memset(&eth_info,0,sizeof(ipintf_info_t));

		app_ipintf_device_init(&eth_info);
		
		ret = app_ipintf_add_ip_address(eth_info.localip, eth_info.gateway, eth_info.ipmask);
		
		if(CS_E_OK == ret)
		{
			cli_print(cli, "ip config success");
		}
		else
		{
			cli_print(cli, "ip config failed");
			return CLI_ERROR;
		}	
	}
	
	return CLI_OK;
}

#endif /* #ifdef HAVE_PLATFORM_UNIFY_CMD */


#ifdef HAVE_ETH0_IP_ADDR_RESTORE
extern int eth0_ip_config_recover(network_card_ip_t *ip_info)
{
	int ret = -1;
	
	if(NULL == ip_info)
	{
		return -1;
	}
	
	if(CS_E_OK == epon_request_onu_ip_config_set(ip_info->local_ip, ip_info->ip_mask, ip_info->gw_ip))
	{
		cs_printf("eth0 ip config recover success!\r\n");
		ret = 0;
	}
	else
	{
		cs_printf("eth0 ip config recover fail\r\n");
	}
	return ret;
}

extern int eth0_ip_config_show(network_card_ip_t *ip_info)
{
	int ret = 0;

	struct sockaddr_in addrp[3];

	memset(addrp,0,sizeof(addrp));
		
	if(NULL == ip_info)
	{
		return -1;
	}
	
	addrp[0].sin_addr.s_addr = htonl(ip_info->local_ip);
	addrp[1].sin_addr.s_addr = htonl(ip_info->ip_mask); 
	addrp[2].sin_addr.s_addr = htonl(ip_info->gw_ip);
			
	cs_printf("eth0 IP - %s\r\n", inet_ntoa(addrp[0].sin_addr));
	cs_printf("eth0 IP Submask - %s\r\n", inet_ntoa(addrp[1].sin_addr));
	cs_printf("eth0 IP Gateway - %s\r\n", inet_ntoa(addrp[2].sin_addr));
	
	return ret;
}

extern int eth0_ip_tlv_infor_get(int *len, char **value, int *free_need)
{
	int ret = -1;
	network_card_ip_t ip_info; 
	char *buf = NULL;
	int buf_size = 0;

	memset(&ip_info,0,sizeof(ip_info));
	
	if(NULL == len)
	{
		return -1;
	}
	if(NULL == value)
	{
		return -1;
	}
	if(NULL == value)
	{
		return -1;
	}	
	
	if(CS_E_OK == epon_request_onu_ip_config_get(&ip_info.local_ip, &ip_info.ip_mask, &ip_info.gw_ip))
	{
		buf_size = sizeof(ip_info);
	
		if(NULL != (buf = (char *)iros_malloc(IROS_MID_APP, buf_size)))
		{
			memcpy(buf, &ip_info, buf_size);
			*len = buf_size;
			*value = buf;
			*free_need = 1;
			ret = 0;
		}
	}
			
	return ret;
}

extern int eth0_ip_tlv_infor_handle(int len, char *data, int opcode)
{
	int ret = -1;
	network_card_ip_t ip_info; 

	memset(&ip_info,0,sizeof(ip_info));
	
	if(0 != len)
	{
		//do nothing
	}
	else
	{
		return -1;
	}
	
	if(NULL == data)
	{
		return -1;
	}

	memcpy(&ip_info, data, sizeof(ip_info));
	
	if(DATA_RECOVER == opcode)
	{
		ret = eth0_ip_config_recover(&ip_info);
	}
	else if(DATA_SHOW == opcode)
	{
		ret = eth0_ip_config_show(&ip_info);
	}
	else
	{
		cs_printf("in %s, unknown opcode :%d\n", __func__, opcode);
	}
	
	return ret;
}

extern int eth0_ip_running_config_show(void)
{
	int ret = -1;
	network_card_ip_t ip_info;

	memset(&ip_info,0,sizeof(ip_info));
	
	if(CS_E_OK == epon_request_onu_ip_config_get(&ip_info.local_ip, &ip_info.ip_mask, &ip_info.gw_ip))
	{
		ret = eth0_ip_config_show(&ip_info);
	}

	return ret;
}

#endif /* end of #ifdef HAVE_ETH0_IP_ADDR_RESTORE */

#ifdef HAVE_ETH1_IP_ADDR_RESTORE

extern int eth1_ip_config_recover(network_card_ip_t *ip_info)
{
	int ret = -1;

	if(CS_E_OK == epon_request_onu_eth1_ip_config_set(ip_info->local_ip, ip_info->ip_mask, ip_info->gw_ip))
	{
		cs_printf("eth1 ip config recover success!\r\n");
		ret = 0;
	}
	else
	{
		cs_printf("eth1 ip config recover fail\r\n");
	}
	return ret;
}

extern int eth1_ip_config_show(network_card_ip_t *ip_info)
{
	int ret = 0;

	struct sockaddr_in addrp[3];

	memset(addrp,0,sizeof(addrp));

	addrp[0].sin_addr.s_addr = htonl(ip_info->local_ip);
	addrp[1].sin_addr.s_addr = htonl(ip_info->ip_mask); 
	addrp[2].sin_addr.s_addr = htonl(ip_info->gw_ip);
			
	cs_printf("eth1 IP - %s\r\n", inet_ntoa(addrp[0].sin_addr));
	cs_printf("eth1 IP Submask - %s\r\n", inet_ntoa(addrp[1].sin_addr));
	cs_printf("eth1 IP Gateway - %s\r\n", inet_ntoa(addrp[2].sin_addr));
	
	return ret;
}

extern int eth1_ip_tlv_infor_get(int *len, char **value, int *free_need)
{
	int ret = -1;
	network_card_ip_t ip_info; 
	char *buf = NULL;
	int buf_size = 0;

	memset(&ip_info,0,sizeof(ip_info));
	
	if(NULL == len)
	{
		return -1;
	}
	if(NULL == value)
	{
		return -1;
	}
	if(NULL == value)
	{
		return -1;
	}	
	
	if(CS_E_OK == epon_request_onu_eth1_ip_config_get(&ip_info.local_ip, &ip_info.ip_mask, &ip_info.gw_ip))
	{
		buf_size = sizeof(ip_info);
	
		if(NULL != (buf = (char *)iros_malloc(IROS_MID_APP, buf_size)))
		{
			memcpy(buf, &ip_info, buf_size);
			*len = buf_size;
			*value = buf;
			*free_need = 1;
			ret = 0;
		}
	}
			
	return ret;
}

extern int eth1_ip_tlv_infor_handle(int len, char *data, int opcode)
{
	int ret = -1;
	network_card_ip_t ip_info; 

	memset(&ip_info,0,sizeof(ip_info));
	
	if(0 != len)
	{
		//do nothing
	}
	else
	{
		return -1;
	}
	
	if(NULL == data)
	{
		return -1;
	}

	memcpy(&ip_info, data, sizeof(ip_info));
	
	if(DATA_RECOVER == opcode)
	{
		ret = eth1_ip_config_recover(&ip_info);
	}
	else if(DATA_SHOW == opcode)
	{
		ret = eth1_ip_config_show(&ip_info);
	}
	else
	{
		cs_printf("in %s, unknown opcode :%d\n", __func__, opcode);
	}
	
	return ret;
}

extern int eth1_ip_running_config_show(void)
{
	int ret = -1;
	network_card_ip_t ip_info;

	memset(&ip_info,0,sizeof(ip_info));
	
	if(CS_E_OK == epon_request_onu_eth1_ip_config_get(&ip_info.local_ip, &ip_info.ip_mask, &ip_info.gw_ip))
	{
		ret = eth1_ip_config_show(&ip_info);
	}

	return ret;
}

#endif /* end of #ifdef HAVE_ETH1_IP_ADDR_RESTORE */
/*add by zhaoxh@2016-8-11*/
#ifdef HAVE_STATIC_FDB_ADDR_RESTORE
extern int static_fdb_config_recover(int len,char *data)
{
    int ret = 0;
    int count = len/sizeof(cs_sdl_fdb_entry_t);
	
	while(count)
    { 
        cs_sdl_fdb_entry_t entry;
		char buf[13];
		
		memset(&entry,0,sizeof(cs_sdl_fdb_entry_t));
        memcpy(&entry,data,sizeof(cs_sdl_fdb_entry_t));
		
		sprintf(buf,"%02x%02x%02x%02x%02x%02x",
			        entry.mac.addr[0],
					entry.mac.addr[1],
					entry.mac.addr[2],
					entry.mac.addr[3],
					entry.mac.addr[4],
					entry.mac.addr[5]);
    	if(fdb_static_list_add(buf,entry.port,entry.vlan_id) != CS_E_OK)
		{
			cs_printf("add static to flash error\n");
			return ret;
		}
		data += sizeof(cs_sdl_fdb_entry_t);
		count--;
	}
	return ret;
}
extern int static_fdb_config_show(int len,char *data)
{
	int ret = 0;
	cs_sdl_fdb_entry_t entry;
	int count = len/sizeof(cs_sdl_fdb_entry_t);
	int num = count;
	char *pdata = data;
	memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));
	
	cs_printf("================FDB SW table is shown:================\n");
		cs_printf("index\t mac_address\t\t vid\t port\t type\n");
	
		while(num)
		{
		    memcpy(&entry,pdata,sizeof(cs_sdl_fdb_entry_t));		
			cs_uint16 vid = entry.vlan_id?entry.vlan_id:1;
			cs_printf("%-6d\t %02x:%02x:%02x:%02x:%02x:%02x\t %-6d\t %-2d\t %-2d\n", count,
				entry.mac.addr[0],
				entry.mac.addr[1],
				entry.mac.addr[2],
				entry.mac.addr[3],
				entry.mac.addr[4],
				entry.mac.addr[5],
				vid,
				entry.port,
				entry.type);
			num--;
			pdata += sizeof(cs_sdl_fdb_entry_t);
			
		}
		cs_printf("=================Totally %d Static SW entries=================\n", count);
		return ret;
}
extern int static_fdb_tlv_infor_get(int *len, char **value, int *free_need)
{
	int ret = -1;
	cs_callback_context_t context;
	cs_uint16 idx = 0, next = 0;
	cs_sdl_fdb_entry_t entry;
	cs_sdl_fdb_entry_t arr_entry[64];
	cs_sdl_fdb_entry_t *buf = NULL;
	int count = 0;
	
	memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));
	memset(arr_entry, 0, 64*sizeof(cs_sdl_fdb_entry_t));
	
	while(epon_request_onu_fdb_entry_get_byindex(context, 0, 0, SDL_FDB_ENTRY_GET_MODE_ALL, idx, &entry, &next) == CS_OK)
	{
		idx = next;
		if(SDL_FDB_ENTRY_STATIC == entry.type)
		{
		    memcpy(&arr_entry[count], &entry, sizeof(cs_sdl_fdb_entry_t));
			count++;
		}
	}
	if(NULL != (buf = (cs_sdl_fdb_entry_t *)iros_malloc(IROS_MID_APP, count*sizeof(cs_sdl_fdb_entry_t))))
	{
		 memcpy(buf, &arr_entry, count*sizeof(cs_sdl_fdb_entry_t));
		 *len = count*sizeof(cs_sdl_fdb_entry_t);
		 *value = (char *)buf;
		 *free_need = 1;
		 ret = 0;
	}
	return ret;
}

extern int static_fdb_tlv_infor_handle(int len, char *data, int opcode)
{
	int ret = -1;
	if(0 != len)
	{
		//do nothing
	}
	else
	{
		return -1;
	}
	
	if(NULL == data)
	{
		return -1;
	}
	if(DATA_RECOVER == opcode)
	{
		ret = static_fdb_config_recover(len,data);
	}
	else if(DATA_SHOW == opcode)
	{
		ret = static_fdb_config_show(len,data);
	}
	else
	{
		cs_printf("in %s, unknown opcode :%d\n", __func__, opcode);
	}
	

	return ret;
}
extern int static_fdb_running_config_show(void)
{
	int ret = 0;
	cs_callback_context_t context;
	cs_uint16 idx = 0, next = 0;
	cs_sdl_fdb_entry_t entry;
	int count = 0;
	
	memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));
	
	cs_printf("================FDB SW table is shown:================\n");
		cs_printf("index\t mac_address\t\t vid\t port\t type\n");
	
		while(epon_request_onu_fdb_entry_get_byindex(context, 0, 0, SDL_FDB_ENTRY_GET_MODE_ALL, idx, &entry, &next) == CS_OK)
		{
			idx = next;
			if(SDL_FDB_ENTRY_STATIC == entry.type)
			{
				cs_uint16 vid = entry.vlan_id?entry.vlan_id:1;
				
				count++;
				cs_printf("%-6d\t %02x:%02x:%02x:%02x:%02x:%02x\t %-6d\t %-2d\t %-2d\n", count,
					entry.mac.addr[0],
					entry.mac.addr[1],
					entry.mac.addr[2],
					entry.mac.addr[3],
					entry.mac.addr[4],
					entry.mac.addr[5],
					vid,
					entry.port,
					entry.type);
			}
		}
		cs_printf("=================Totally %d Static SW entries=================\n", count);
	return ret;
}

#endif /* end of #HAVE_STATIC_FDB_ADDR_RESTORE */

#ifdef HAVE_AGING_FDB_ADDR_RESTORE

extern int aging_fdb_config_recover(int data)
{
	int ret = 0;
	if(CS_E_OK != fdb_aging_set(data))
	{
		cs_printf("set age failed\n");
		return -1;
	}
	return ret;
}
extern int aging_fdb_config_show(int data)
{
    int ret = 0;
    if(data>400 || data < 0){
		cs_printf("mac table error\n");
	    return -1;
    	}
	else 
        cs_printf("Mac table aging time is %d seconds (PAS & BCM).\r\n", data);
	return ret;
}

extern int aging_fdb_tlv_infor_get(int *len, char **value, int *free_need)
{
    int ret = -1;
	int age = 0;
	int *buf = NULL;
	if(CS_E_OK != fdb_aging_get(&age))
	{
		cs_printf("get aging failed\n");
		return ret;
	}
	if(NULL != (buf = (int *)iros_malloc(IROS_MID_APP, sizeof(int))))
	{
		memcpy(buf, &age, sizeof(int));
		*len = sizeof(int);
		*value = (char *)buf;
		*free_need = 1;
		ret = 0;
	}
    return ret;
}
	
extern int aging_fdb_tlv_infor_handle(int len, char *data, int opcode)
{
    int ret = 0;
	int buf = 0;
	if(0 != len)
	{
		//do nothing
	}
	else
	{
		return -1;
	}
	
	if(NULL == data)
	{
		return -1;
	}
	memcpy(&buf, data, len);
	if(DATA_RECOVER == opcode)
	{
		ret = aging_fdb_config_recover(buf);
	}
	else if(DATA_SHOW == opcode)
	{
		ret = aging_fdb_config_show(buf);
	}
	else
	{
		cs_printf("in %s, unknown opcode :%d\n", __func__, opcode);
	}

	return ret;
}
extern int aging_fdb_running_config_show(void)
{
	int age = 0;
	int ret = 0;
	if(CS_E_OK != fdb_aging_get(&age))
			cs_printf("get atu aging time fail!\r\n");
	else
			cs_printf("Mac table aging time is %d seconds (PAS & BCM).\r\n", age);
	return ret;
}

#endif /* end of #HAVE_AGING_FDB_ADDR_RESTORE */

/*add by zhaoxh@2016-8-17*/
#ifdef HAVE_LEARNING_FDB_ADDR_RESTORE
extern int learning_fdb_config_recover(int *data)
{
   int i;
   int portid = 1;
   int ret = 0;
    for(i = 0;i<UNI_PORT_MAX;i++)
	{
	    if(CS_E_OK != fdb_learn_set(portid,  data[i])) {
		     cs_printf("set error");
	    }
		portid++;
    }
	return ret;
}
extern int learning_fdb_config_show(int *data)
{
	int i;
	int ret= 0;
	int portid = 1;
	for(i = 0;i<UNI_PORT_MAX;i++)
    {
	     cs_printf("Port %d source mac address learn is %s\n",portid,data[i]?"enable":"disable");         
	     portid++;
     }
	 return ret; 
}

extern int learning_fdb_tlv_infor_get( int *len, char **value, int *free_need)
{
	int i;
	int ret = 0;
	int en = 0;
	int portid= 1;
	int en_array[UNI_PORT_MAX] = {0}; 
	int *buf = NULL;
    for(i = 0;i < UNI_PORT_MAX;i++)
    {
		if(CS_E_OK == fdb_learn_get(portid, &en))
		{
		    en_array[i] = en;
			portid++;
		}	
    }
	if(NULL != (buf = (int *)iros_malloc(IROS_MID_APP, UNI_PORT_MAX*sizeof(int))))
	{
		memcpy(buf, en_array,UNI_PORT_MAX*sizeof(int));
		*len = UNI_PORT_MAX*sizeof(int);
		*value = (char *)buf;
		*free_need = 1;
		ret = 0;
	}
	return ret;
}
extern int learning_fdb_tlv_infor_handle(int len, char *data, int opcode)
{
	int ret = 0;
	int buf[UNI_PORT_MAX] = {0};
	if(0 != len)
	{
		//do nothing
	}
	else
	{
		return -1;
	}
	
	if(NULL == data)
	{
		return -1;
	}
	memcpy(buf, data, len);
	if(DATA_RECOVER == opcode)
	{
		ret = learning_fdb_config_recover(buf);
	}
	else if(DATA_SHOW == opcode)
	{
		ret = learning_fdb_config_show(buf);
	}
	else
	{
		cs_printf("in %s, unknown opcode :%d\n", __func__, opcode);
	}

	return ret;
}
extern int learning_fdb_running_config_show(void)
{
    int i;
	int ret= 0;
	int en = 0;
	int portid = 1;
	for(i = 0;i<UNI_PORT_MAX;i++)
    {
		if(CS_E_OK != fdb_learn_get(portid, &en))
			cs_printf("get port %d atu learning fail!\r\n", portid);
		else
			cs_printf("Port %d source mac address learn is %s\n",portid,en?"enable":"disable");
         
	     portid++;
     }
     return ret;
}
#endif  /* HAVE_LEARNING_FDB_ADDR_RESTORE */

/* added by wujm@2016-08-04,sa block */
int cmd_atu_block_add(struct cli_def *cli, char *command, char *argv[], int argc) 
{
    cs_callback_context_t     context;
    cs_sdl_fdb_entry_t entry;
    sdl_mac_block_type_t type;

    memset(&entry,0,sizeof(entry));

    if (CLI_HELP_REQUESTED) {
        switch (argc) {
            case 1:
            return cli_arg_help(cli, 0, "xxxx.xxxx.xxxx", "Please input the mac address",
                        NULL );
        case 2:
            return cli_arg_help(cli, 0, "<0|1|2>", "0:sa block, 1:da block, 2:both",
                        NULL );
        default:
            return cli_arg_help(cli, 1, NULL );
        }
    }
    
    if(2 == argc)
    {
        cs_status ret = 0;
        cs_uint8 i = 0;
        type = iros_strtol(argv[1]);
        unsigned int mac[3];

        sscanf(argv[0], "%04x.%04x.%04x", &mac[0], &mac[1], &mac[2]);

        for (i=0; i<3; i++)
        {
            entry.mac.addr[i*2] = (mac[i] >> 8) & 0xff;
            entry.mac.addr[i*2 + 1] = mac[i]  & 0xff;
        }

        entry.port = CS_UNI_PORT_ID1;
        entry.type = SDL_FDB_ENTRY_STATIC;

        switch(type)
        {
            case 0:
                type = SA_BLOCK;
                break;
            case 1:
                type = DA_BLOCK;
                break;
            case 2:
                type = SA_DA_BOTH_BLOCK;
                break;
            default:
                cs_printf("unknown type %d\n",type);
                return CLI_OK;
        }

        ret = epon_request_onu_fdb_block_entry_add(context,0,0,&entry,type);
    }
    else
    {
        cs_printf("wrong args!\n");
    }

    return CLI_OK;
}

int cmd_atu_block_del(struct cli_def *cli, char *command, char *argv[], int argc) 
{
    cs_callback_context_t     context;
    cs_mac_t mac;

    if (CLI_HELP_REQUESTED) {
        switch (argc) {
            case 1:
            return cli_arg_help(cli, 0, "xxxx.xxxx.xxxx", "Please input the mac address",
                        NULL );
        default:
            return cli_arg_help(cli, 1, NULL );
        }
    }
    
    if(1 == argc)
    {
        cs_status ret = 0;
        cs_uint8 i = 0;
        unsigned int mac_tmp[3];

        sscanf(argv[0], "%04x.%04x.%04x", &mac_tmp[0], &mac_tmp[1], &mac_tmp[2]);

        for (i=0; i<3; i++)
        {
            mac.addr[i*2] = (mac_tmp[i] >> 8) & 0xff;
            mac.addr[i*2 + 1] = mac_tmp[i]  & 0xff;
        }

        ret = epon_request_onu_fdb_block_entry_del(context,0,0,&mac,0);
    }
    else
    {
        cs_printf("wrong args!\n");
    }

    return CLI_OK;
}

int cmd_atu_block_show(struct cli_def *cli, char *command, char *argv[], int argc) 
{
    int ret = CLI_OK;
    cs_callback_context_t context;
    cs_uint16 idx = 0, next = 0;

    cs_sdl_fdb_entry_t entry;
    sdl_mac_block_type_t type= 0;
    cs_uint8 c = 0;
    int count = 0;
    char *type_str[] = {"sa block","da block","both block"};
    char *p_type_str = NULL;

    memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));

    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        default:
            return cli_arg_help(cli, 1, NULL);
        }
    }

    cli_print(cli, "================FDB SW mac block table is shown:================");
    cli_print(cli, "index\t mac_address\t\t block type");

    while(epon_request_onu_fdb_block_entry_getnext(context, 0, 0, idx, &next, &entry,&type) == CS_OK)
    {
        switch(type)
        {
            case SA_BLOCK:
                p_type_str = type_str[0];
                break;
            case DA_BLOCK:
                p_type_str = type_str[1];
                break;
           case SA_DA_BOTH_BLOCK:
                p_type_str = type_str[2];
                break;   
           default:
                return CLI_ERROR;
        }
        
        idx = next;
        count++;
        cli_print(cli, "%-6d\t %02x:%02x:%02x:%02x:%02x:%02x\t  %-s", count,
            entry.mac.addr[0],
            entry.mac.addr[1],
            entry.mac.addr[2],
            entry.mac.addr[3],
            entry.mac.addr[4],
            entry.mac.addr[5],
            p_type_str);
        if(++c > 1)
        {
            cs_thread_delay(100);
            c = 0;
        }
    }
    cli_print(cli, "=================Totally %d SW entries=================\n", count);

    return ret;
}
/*add by zhaoxh 显示olt给onu设置的告警阈值命令行*/
extern  double log10(double x) ;
int cmd_opm_threshold_info_get(struct cli_def *cli, char *command, char *argv[], int argc)
{

    cs_uint32 raiseValue = 0;
    cs_uint32 clearValue = 0;
    int type = 0,port = 0,code = 0;
	if (CLI_HELP_REQUESTED) {
        switch (argc) {
            case 1:
            return cli_arg_help(cli, 0, "alarm type","0-pon,1-onu,2-port" ,
                        NULL );
            case 2:
            return cli_arg_help(cli, 0, "port", "pon 0;port:1,2,3,4",
                        NULL );
			 case 3:
            return cli_arg_help(cli, 0, "alarm code",
                        NULL );
                
        default:
            return cli_arg_help(cli, 1, NULL );
        }
    }
	type = iros_strtol(argv[0]);
	port = iros_strtol(argv[1]);
	code = iros_strtol(argv[2]);
    if(!ctc_oam_get_alarm_threshold(type,port,code,&raiseValue,&clearValue))
    {
        switch (code)
        {
        	case CTC_ALARM_RX_POW_HIGH:
			case CTC_ALARM_RX_POW_LOW:
			case CTC_ALARM_TX_POW_HIGH:
			case CTC_ALARM_TX_POW_LOW:
				{
					double rxdbmraise = 0.0,rxdbmclear = 0.0;
					rxdbmraise = raiseValue;
					rxdbmclear = clearValue;

					rxdbmraise = 10*log10(rxdbmraise*0.0001);
					rxdbmclear = 10*log10(rxdbmclear*0.0001);
				    cli_print(cli,"OPT:raiseValue: %1.4f dbm",rxdbmraise);
                    cli_print(cli,"OPT:clearValue: %1.4f dbm",rxdbmclear);
					cli_print(cli,"OPT:raiseValue: %1.2f",((double)raiseValue)/10000);
					cli_print(cli,"OPT:clearValue: %1.2f",((double)clearValue)/10000);
			     }
			     break;
			case CTC_ALARM_TX_BIAS_HIGH:
			case CTC_ALARM_TX_BIAS_LOW :
				{
					cli_print(cli,"Current:raiseValue: %3.0f",((double)raiseValue)/500);
					cli_print(cli,"Current:clearValue: %3.0f",((double)clearValue)/500);
				}
			    break;
		    case CTC_ALARM_VCC_HIGH:
			case CTC_ALARM_VCC_LOW:
				{
					cli_print(cli,"VCC:raiseValue: %1.2f",((double)raiseValue)/10000);
					cli_print(cli,"VCC:clearValue: %1.2f",((double)clearValue)/10000);
				}
			    break;
			case CTC_ALARM_PON_TEMP_HIGH:
			case CTC_ALARM_PON_TEMP_LOW:
				{
					cli_print(cli,"TEMP:raiseValue: %3.0f",((double)raiseValue)/256);
					cli_print(cli,"TEMP:clearValue: %3.0f",((double)clearValue)/256);
				}
			    break;
			default:
				cli_print(cli,"Not supported yet!\n");
				break;
			
				
        }
    }
	else
	{
		cli_print(cli,"get alarm_threshold failed code = %x!\n",code);
	}
    return CLI_OK;
}
int cmd_onu_ctc_op_set(struct cli_def *cli, char *command, char *argv[], int argc)
{
	   if (CLI_HELP_REQUESTED) {
           switch (argc) {
                  case 1:
                  return cli_arg_help(cli, 0, "ctc version","2.1 & 3.0" ,
                        NULL );
			      default:
                   return cli_arg_help(cli, 1, NULL );
                       }
		}
		if(0 == strcmp("2.1",argv[0]))
		{
			ctc_version_set = 1;
		}
		else if(0 == strcmp("3.0",argv[0]))
		{
			ctc_version_set = 0;
		}
		else 
		{
			cs_printf("input error!!!!!\n");
		}
	return CLI_OK;
}

/*add by zhaoxh 向olt发送告警报文命令行*/
int cmd_onu_event_alarm(struct cli_def *cli, char *command, char *argv[], int argc)
{
	int ret = 0;
	int type,port,code,op,portId;
	cs_uint64 value;
	//char ctc_ver[3] = "2.1";
	if (CLI_HELP_REQUESTED) {
        switch (argc) {
            case 1:
            return cli_arg_help(cli, 0, "alarm type","0-pon,1-onu,2-port" ,
                        NULL );
            case 2:
            return cli_arg_help(cli, 0, "port", "1,2,3,4",
                        NULL );
			 case 3:
            return cli_arg_help(cli, 0, "alarm code",
                        NULL );
			 case 4:
            return cli_arg_help(cli, 0, "alarm op",
                        NULL );
			 case 5:
            return cli_arg_help(cli, 0, "alarm value",
                        NULL );
			 case 6:
            return cli_arg_help(cli, 0, "ctc ver:2.1 & 3.0",
                        NULL );
        default:
            return cli_arg_help(cli, 1, NULL );
        }
    }
	type = iros_strtol(argv[0]);
	port = iros_strtol(argv[1]);
	code = iros_strtol(argv[2]);
	op = iros_strtol(argv[3]);
	value = iros_strtol(argv[4]);
	for(portId = 1;portId <= UNI_PORT_MAX;portId++)
		{
			ctc_oam_set_alarm_config(type,portId,code,CTC_ALARM_ON);
		}
	cs_printf("type = %d,port = %d,code = %d,op = %d\n",type,port,code,op);
	if(0 == strcmp("2.1",argv[5]))
	{
		ctc_oam_build_alarm_event(type,port,code,op,value);
	}
	else if(0 == strcmp("3.0",argv[5]))
	{
	    if(0 == op)
	    {
	    	ctc_oam_build_alarm_event(type,port,code,1,value);
	    }
		
		else
		{
			ctc_oam_build_alarm_event(type,port,code,0,value);
		}
	}
	else
	{
		cs_printf("put argc[5] error\n");
	}
	return ret;
}
void cli_reg_gwd_cmd(struct cli_command **cmd_root)
{
	struct cli_command *set = NULL;
	struct cli_command *display = NULL;
	struct cli_command *atu = NULL;
	struct cli_command *atu_show = NULL;
	struct cli_command *atu_limit = NULL;
	struct cli_command *port = NULL;
	struct cli_command *show = NULL;
	
	struct cli_command *event_alarm = NULL;
	event_alarm = cli_register_command(cmd_root, NULL, "event_alarm", NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "Event alarm");
	cli_register_command(cmd_root, event_alarm, "value",   cmd_onu_event_alarm,     PRIVILEGE_ADMIN, MODE_CONFIG, "alarm value");
    cli_register_command(cmd_root, event_alarm, "ctc",   cmd_onu_ctc_op_set,     PRIVILEGE_ADMIN, MODE_CONFIG, "ctc 2.1 & 3.0");
	
	set = cli_register_command(cmd_root, NULL, "set", NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "Set system information");
	cli_register_command(cmd_root, set, "date",    cmd_onu_mgt_config_product_date,     PRIVILEGE_ADMIN, MODE_CONFIG, "Manufacture date");
	cli_register_command(cmd_root, set, "serial",    cmd_onu_mgt_config_product_sn,     PRIVILEGE_ADMIN, MODE_CONFIG, "Manufacture serial number(<16)");
	#ifndef HAVE_INACTIVE_REDUPLICATE_CMD
	cli_register_command(cmd_root, set, "devicename",    cmd_onu_mgt_config_device_name,     PRIVILEGE_ADMIN, MODE_CONFIG, "Device name(<=126)");
	#endif
	cli_register_command(cmd_root, set, "hw-version",    cmd_onu_mgt_config_product_hw_version,     PRIVILEGE_ADMIN, MODE_CONFIG, "Hardware version");

	display  = cli_register_command(cmd_root, NULL, "display", NULL, PRIVILEGE_ADMIN, MODE_ANY, "Show information");
    cli_register_command(cmd_root, display, "product", cmd_display_product, PRIVILEGE_ADMIN, MODE_ANY, "System information");
	cli_register_command(cmd_root, display, "threshold", cmd_opm_threshold_info_get, PRIVILEGE_ADMIN, MODE_ANY, "threshold information");

	atu = cli_register_command(cmd_root, NULL, "atu", NULL, PRIVILEGE_ADMIN, MODE_EXEC, "atu command");
	cli_register_command(cmd_root, atu, "learning", cmd_oam_atu_learn, PRIVILEGE_ADMIN, MODE_EXEC, "learning enable");
	cli_register_command(cmd_root, atu, "aging", cmd_oam_atu_age, PRIVILEGE_ADMIN, MODE_EXEC, "age set");
	atu_show = cli_register_command(cmd_root, atu, "show", cmd_atu_show, PRIVILEGE_ADMIN, MODE_EXEC, "show information");
	cli_register_command(cmd_root, atu_show, "counter", cmd_atu_show_counter, PRIVILEGE_ADMIN, MODE_EXEC, "show counter");
	cli_register_command(cmd_root, atu_show, "static", cmd_atu_show_static, PRIVILEGE_ADMIN, MODE_EXEC, "show static entry");
	cli_register_command(cmd_root, atu_show, "mac", cmd_atu_show_mac, PRIVILEGE_ADMIN, MODE_EXEC, "show mac information");
	cli_register_command(cmd_root, atu_show, "port", cmd_atu_show_port, PRIVILEGE_ADMIN, MODE_EXEC, "show port information");
	cli_register_command(cmd_root, atu_show, "vlan", cmd_atu_show_vlan, PRIVILEGE_ADMIN, MODE_EXEC, "show vlan information");
	cli_register_command(cmd_root, atu, "static_add", cmd_static_mac_add_fdb, PRIVILEGE_ADMIN, MODE_EXEC, "static fdb mac add");
	cli_register_command(cmd_root, atu, "static_del", cmd_static_mac_del_fdb, PRIVILEGE_ADMIN, MODE_EXEC, "static fdb mac del");
	/*add by zhaoxh@2016-8-15*/
	atu_limit = cli_register_command(cmd_root, atu, "limit", cmd_atu_limit_global, PRIVILEGE_ADMIN, MODE_EXEC, "Show or config port mac limit");
	cli_register_command(cmd_root, atu_limit, "port", cmd_atu_limit_port, PRIVILEGE_ADMIN, MODE_EXEC, "Show or config port mac limit");
	cli_register_command(cmd_root, atu, "block_add", cmd_atu_block_add, PRIVILEGE_ADMIN, MODE_EXEC, "block fdb mac add");
	cli_register_command(cmd_root, atu, "block_del", cmd_atu_block_del, PRIVILEGE_ADMIN, MODE_EXEC, "block fdb mac del");
	cli_register_command(cmd_root, atu, "block_show", cmd_atu_block_show, PRIVILEGE_ADMIN, MODE_EXEC, "show block mac information");

	port = cli_register_command(cmd_root, NULL, "port",  NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "Show or config onu port information");
	
	#ifdef HAVE_IP_STACK
	{
		struct cli_command *ifconfig = NULL;
		ifconfig = cli_register_command(cmd_root, NULL, "ifconfig", NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "ip config");
		cli_register_command(cmd_root, ifconfig, "eth0", cmd_ifconfig_eth0, PRIVILEGE_ADMIN, MODE_CONFIG, "ifconfig eth0");
		#ifdef HAVE_TCP_UDP_NETWORK_CARD
		//added by zhuxh
		cli_register_command(cmd_root, ifconfig, "eth1", cmd_ifconfig_eth1, PRIVILEGE_ADMIN, MODE_CONFIG, "ifconfig eth0");
		#endif	/*(#ifdef HAVE_TCP_UDP_NETWORK_CARD)*/
	}
	#endif	/*(#ifdef HAVE_IP_STACK)*/
	
	#ifdef HAVE_GWD_FLASH_OPER
	{
		struct cli_command *erase = NULL;
		
		cli_register_command(cmd_root, NULL, "save", cmd_save, PRIVILEGE_ADMIN, MODE_CONFIG, "Save ONU current config to flash");

		show = cli_register_command(cmd_root, NULL, "show", NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "Show information");
		cli_register_command(cmd_root,  show, "start-up", cmd_show_start_up,PRIVILEGE_ADMIN, MODE_CONFIG, "Show ONU saved config");
		cli_register_command(cmd_root,  show, "running-config", cmd_show_running_config,PRIVILEGE_ADMIN, MODE_CONFIG, "Show ONU current config");
		erase = cli_register_command(cmd_root, NULL, "erase", NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "erase config");
		cli_register_command(cmd_root,  erase, "config", cmd_erase_config,PRIVILEGE_ADMIN, MODE_CONFIG, "Erase saved config");
	}
	#endif	/*(#ifdef HAVE_GWD_FLASH_OPER)*/

	#ifdef HAVE_GWD_IGMP_CMD
	{
		cli_register_command(cmd_root, NULL, "igmp-mode", cmd_igmp_mode, PRIVILEGE_ADMIN, MODE_CONFIG, "IGMP mode get/set");
	}
	#endif	/*(#ifdef HAVE_GWD_FLASH_OPER)*/

	#ifdef HAVE_QINQ
	{
		struct cli_command *vlan = NULL;
		struct cli_command *vlan_debug = NULL;
		
		vlan = cli_register_command(cmd_root, NULL, "vlan", NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "Config onu vlan translation table");
		cli_register_command(cmd_root, vlan, "pas-rule", cmd_vlan_pas_rule, PRIVILEGE_ADMIN, MODE_CONFIG, "Config onu vlan translation table");
		vlan_debug = cli_register_command(cmd_root, NULL, "vlan", NULL, PRIVILEGE_ADMIN, MODE_DEBUG, "debug onu vlan translation table");
		cli_register_command(cmd_root, vlan_debug, "pas-rule-debug", cmd_vlan_pas_rule_debug, PRIVILEGE_ADMIN, MODE_DEBUG, "debug onu vlan translation table");
	}
	#endif	/*(#ifdef HAVE_QINQ)*/

	#ifdef HAVE_VLAN_CMD
	{
		struct cli_command *vlan = NULL;

		vlan = cli_register_command(cmd_root, NULL, "vlan", NULL, PRIVILEGE_ADMIN, MODE_EXEC, "vlan command");
		
		#ifdef HAVE_PORT_ISOLATION_CMD
		cli_register_command(cmd_root, vlan, "port_isolate", cmd_vlan_port_isolate, PRIVILEGE_ADMIN, MODE_EXEC, "isolate command");
		cli_register_command(cmd_root, vlan, "port_mapping", cmd_vlan_mapping, PRIVILEGE_ADMIN, MODE_EXEC, "vlan mapping command");
		#endif

		#ifdef HAVE_VLAN_DOT1Q_CMD
		cli_register_command(cmd_root, vlan, "dot1q", cmd_vlan_dot1q, PRIVILEGE_ADMIN, MODE_EXEC, "Show or config onu dot1q vlan");
		cli_register_command(cmd_root, vlan, "dot1q_add", cmd_vlan_dot1q_add, PRIVILEGE_ADMIN, MODE_EXEC, "Add 802.1Q Vlan");
		cli_register_command(cmd_root, vlan, "dot1q_del", cmd_vlan_dot1q_del, PRIVILEGE_ADMIN, MODE_EXEC, "Delete 802.1Q Vlan");
		cli_register_command(cmd_root, vlan, "dot1q_port_add", cmd_vlan_dot1q_port_add, PRIVILEGE_ADMIN, MODE_EXEC, "Add 802.1Q Vlan port");
		cli_register_command(cmd_root, vlan, "dot1q_port_del", cmd_vlan_dot1q_port_del, PRIVILEGE_ADMIN, MODE_EXEC, "Delete 802.1Q Vlan port");
		cli_register_command(cmd_root, vlan, "dot1q_show", cmd_vlan_dot1q_show, PRIVILEGE_ADMIN, MODE_EXEC, "Show 802.1Q Vlan");
		#endif
	}
	#endif	/*(#ifdef HAVE_VLAN_CMD)*/
		
	#ifdef HAVE_GWD_MC_TVM
	{
		struct cli_command *gw_igmp_tvm = NULL;
		
		gw_igmp_tvm = cli_register_command(cmd_root, NULL, "gwd-igmp-tvm",    NULL, PRIVILEGE_ADMIN,   MODE_DEBUG,    "gw igmp tvm debug");
		cli_register_command(cmd_root, gw_igmp_tvm, "dump",    gw_igmp_tvm_dump, PRIVILEGE_ADMIN,   MODE_DEBUG,    "gw igmp tvm debug");
	}
	#endif	/*(#ifdef HAVE_GWD_MC_TVM)*/

	#ifdef HAVE_GWD_STATISTICS_CMD
	{
		#ifdef HAVE_PLATFORM_UNIFY_CMD
		struct cli_command *stat = NULL;
		
		stat = cli_register_command(cmd_root, NULL, "stat", NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "Show system statistics information");
		cli_register_command(cmd_root, stat, "port_show", cmd_stat_port_show, PRIVILEGE_ADMIN, MODE_CONFIG, "show port statistics");
		cli_register_command(cmd_root, stat, "port_flush", cmd_stat_port_flush, PRIVILEGE_ADMIN, MODE_CONFIG, "clear port statistics");
		cli_register_command(cmd_root, stat, "pon_show", cmd_stat_pon_show, PRIVILEGE_ADMIN, MODE_CONFIG, "show pon statistics");
		cli_register_command(cmd_root, stat, "pon_flush", cmd_stat_pon_flush, PRIVILEGE_ADMIN, MODE_CONFIG, "clear pon statistics");
		cli_register_command(cmd_root, stat, "cpu_show", cmd_stat_cpu_show, PRIVILEGE_ADMIN, MODE_CONFIG, "show cpu statistics");
		cli_register_command(cmd_root, stat, "cpu_flush", cmd_stat_cpu_flush, PRIVILEGE_ADMIN, MODE_CONFIG, "clear cpu statistics");
		#else
		struct cli_command *statistics = NULL;
		struct cli_command *statistics_port = NULL;
		struct cli_command *statistics_pon = NULL;
		struct cli_command *statistics_cpu = NULL;

		statistics = cli_register_command(cmd_root, NULL, "statistics", NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "Show system statistics information");
		statistics_port = cli_register_command(cmd_root, statistics, "port", NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "port statistics");
		statistics_pon = cli_register_command(cmd_root, statistics, "pon", NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "pon statistics");
		statistics_cpu = cli_register_command(cmd_root, statistics, "cpu", NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "cpu statistics");
		
		cli_register_command(cmd_root, statistics_port, "show", cmd_statistics_port_show, PRIVILEGE_ADMIN, MODE_CONFIG, "show port statistics");
		cli_register_command(cmd_root, statistics_port, "clear", cmd_statistics_port_clear, PRIVILEGE_ADMIN, MODE_CONFIG, "clear port statistics");

		cli_register_command(cmd_root, statistics_pon, "show", cmd_statistics_pon_show, PRIVILEGE_ADMIN, MODE_CONFIG, "show port statistics");
		cli_register_command(cmd_root, statistics_pon, "clear", cmd_statistics_pon_clear, PRIVILEGE_ADMIN, MODE_CONFIG, "clear port statistics");

		cli_register_command(cmd_root, statistics_cpu, "show", cmd_statistics_cpu_show, PRIVILEGE_ADMIN, MODE_CONFIG, "show port statistics");
		cli_register_command(cmd_root, statistics_cpu, "clear", cmd_statistics_cpu_clear, PRIVILEGE_ADMIN, MODE_CONFIG, "clear port statistics");
		#endif
	}
	#endif	/*(#ifdef HAVE_GWD_STATISTICS_CMD)*/

	#ifdef HAVE_GWD_BC_LIMIT
	{
		struct cli_command *broadcast = NULL;
		struct cli_command *broadcast_storm = NULL;
		
		broadcast = cli_register_command(cmd_root, NULL, "broadcast", NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "Broadcast config");
		broadcast_storm = cli_register_command(cmd_root, broadcast, "storm", NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "Broadcast storm config");
		cli_register_command(cmd_root, broadcast_storm, "portdown", cmd_bsctrl_policy, PRIVILEGE_ADMIN, MODE_CONFIG, "port down config");
		cli_register_command(cmd_root, broadcast_storm, "threshold", cmd_bsctrl_threshold, PRIVILEGE_ADMIN, MODE_CONFIG, "threshold config");
		cli_register_command(cmd_root, broadcast_storm, "threshold_get", cmd_bsctrl_threshold_get, PRIVILEGE_ADMIN, MODE_CONFIG, "threshold config get");
	}
	#endif	/*(#ifdef HAVE_GWD_BC_LIMIT)*/

	#ifdef HAVE_GWD_PORT_MODE_CMD
	{
		cli_register_command(cmd_root, port, "mode",  cmd_port_mode, PRIVILEGE_ADMIN, MODE_CONFIG, "Show or config onu port mode information");
		#ifdef HAVE_PLATFORM_UNIFY_CMD
		cli_register_command(cmd_root, port, "mode_show",  cmd_port_mode_show, PRIVILEGE_ADMIN, MODE_CONFIG, "Show port mode information");
		#endif
	}
	#endif	/*(#ifdef HAVE_GWD_PORT_MODE_CMD)*/

	#if (HAVE_GE_RATE_LIMIIT_CMD == MODULE_YES)
	{
		struct cli_command *egress_rate = NULL;
		struct cli_command *ingress_rate = NULL;

		egress_rate = cli_register_command(cmd_root, port, "egress_rate", cmd_port_egress_rate, PRIVILEGE_ADMIN, MODE_CONFIG, "Show or config onu FE port egress rate information");
		ingress_rate = cli_register_command(cmd_root, port, "ingress_rate", cmd_port_ingress_rate, PRIVILEGE_ADMIN, MODE_CONFIG, "Show or config onu FE port ingress rate information");
	}	
	#endif/*(HAVE_GE_RATE_LIMIIT_CMD == MODULE_YES)*/

	#ifdef HAVE_PORT_MIRROR_CMD
	{
        cli_register_command(cmd_root, port, "mirror", cmd_oam_port_mirror_group, PRIVILEGE_ADMIN, MODE_CONFIG, "port mirror config");
		cli_register_command(cmd_root, port, "mirror_show", cmd_port_mirror_show, PRIVILEGE_ADMIN, MODE_CONFIG, "port mirror show config");
		cli_register_command(cmd_root, port, "mirror_clear", cmd_port_mirror_clear, PRIVILEGE_ADMIN, MODE_CONFIG, "port mirror clear config");
	}
	#endif	/*(#ifdef HAVE_PORT_MIRROR_CMD)*/

	#ifdef HAVE_GWD_PON_LASER_CONTROL
	{
		struct cli_command *laser = NULL;

		laser = cli_register_command(cmd_root, NULL, "laser",  NULL,     PRIVILEGE_ADMIN, MODE_DEBUG, "laser");
		cli_register_command(cmd_root, laser, "control",  cmd_laser_control,     PRIVILEGE_ADMIN, MODE_DEBUG, "laser control");
	}
	#endif	/*(#ifdef HAVE_GWD_PON_LASER_CONTROL)*/

	#ifdef HAVE_GPIO_DEBUG_CMD
	{
		struct cli_command *gpio = NULL;
		
		gpio = cli_register_command(cmd_root, NULL, "gpio", NULL, PRIVILEGE_ADMIN, MODE_DEBUG, "gpio test");
		cli_register_command(cmd_root, gpio, "direction", cmd_onu_gpio_direction, PRIVILEGE_ADMIN, MODE_DEBUG, "Gpio direction test");
		cli_register_command(cmd_root, gpio, "status", cmd_onu_gpio_status, PRIVILEGE_ADMIN, MODE_DEBUG, "Gpio status test");
	}
	#endif	/*(#ifdef HAVE_GPIO_DEBUG_CMD)*/
	#ifdef HAVE_PLATFORM_UNIFY_CMD
	{
		cli_register_command(cmd_root, port, "link_show", cmd_port_link_show, PRIVILEGE_ADMIN, MODE_CONFIG, "Show port link status");
		cli_register_command(cmd_root, port, "en", cmd_port_admin, PRIVILEGE_ADMIN, MODE_CONFIG, "Show or config onu port admin information");
	}
	#endif

	#ifdef HAVE_EVENT_SHOW_CMD
	{
		struct cli_command *event = NULL;

		event = cli_register_command(cmd_root, NULL, "event",  NULL, PRIVILEGE_ADMIN, MODE_CONFIG, "event");
		cli_register_command(cmd_root, event, "show ", cmd_event_show, PRIVILEGE_ADMIN, MODE_CONFIG, "event show");
	}
	#endif

	#ifdef HAVE_DEVICE_NAME_CMD
	{
		struct cli_command *device = NULL;
		
		device = cli_register_command(cmd_root,  show, "device", NULL,PRIVILEGE_ADMIN, MODE_CONFIG, "Show device information");
		cli_register_command(cmd_root,  device, "name", cmd_show_device_name,PRIVILEGE_ADMIN, MODE_CONFIG, "Show device name");
	
		device = cli_register_command(cmd_root,  NULL, "device", NULL,PRIVILEGE_ADMIN, MODE_CONFIG, "Show or config device information");
		cli_register_command(cmd_root,  device, "name", cmd_device_name,PRIVILEGE_ADMIN, MODE_CONFIG, "Show or config device name");
	}
	#endif

	#ifdef HAVE_PLATFORM_UNIFY_CMD
	{
		struct cli_command *ip = NULL;
		struct cli_command *address = NULL;
		ip = cli_register_command(cmd_root,  NULL, "ip", NULL,PRIVILEGE_ADMIN, MODE_CONFIG, "IP information");
		address = cli_register_command(cmd_root,  ip, "address", cmd_ip_address,PRIVILEGE_ADMIN, MODE_CONFIG, "IP address information");
		cli_register_command(cmd_root,  address, "clear", cmd_ip_address_clear,PRIVILEGE_ADMIN, MODE_CONFIG, "set ip address to default config");
	}
	#endif
	
}
#endif
