#include "cli_common.h"
#include "plat_common.h"
#include "db_instance.h"
#include "cli.h"
#ifdef HAVE_ONU_RSTP
#include "stp_api.h"
#endif
#ifdef HAVE_IP_STACK
#include "app_ip_api.h"
#endif
#ifdef HAVE_MC_CTRL
#include "mc_api.h"
#include "mc_ctcctrl.h"
#endif
#ifdef HAVE_CTC_OAM
#include "app_oam_ctc_adapt.h"
#endif
#ifdef HAVE_PPPOE
#include "pppoe_api.h"
#endif
#include "sys_cfg.h"
#include "port_stats.h"
#include "sdl_api.h"
#include "packet.h"
//#include <math.h>

#define cli_ntohs(x)        ((((x) >> 8) & 0xff) | (((x) << 8) & 0xff00))
cs_uint8  uni_max_port_num=0;


int cmd_mgmt_mode_int(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    cli_set_configmode(cli, MODE_CONFIG_MGMT, "mgmt");
    return CLI_OK;
}

#ifdef HAVE_IP_STACK

int ip_address_check_valid( struct cli_def *cli, char * ip_address )
{
   int   value[4];
   char  buffer[16];
   int   i=0;
   int  mini_str_len=0;
   int  value_count=0;
   int  str_len=0;

    memset(value, 0, sizeof(value));
    
    str_len=strlen(ip_address) ;

    if ( str_len>16 ) {
    cli_print(cli, "%% Invalid input");
    return CLI_ERROR;
    }

   strcpy(buffer, ip_address);

   for (i=0; i <str_len;i++) {
        if ( (buffer[i] !='.')
             && (( buffer[i] < '0' ) ||(buffer[i] >'9') )) {
                cli_print(cli, "%% Invalid input");
                return CLI_ERROR;
        }
   }

    /*10.1.1.10*/
       for ( i=0; i < str_len ; i++) {
            if (value_count>=3) {
                value[value_count]=atoi(buffer+i);
                value_count++;
                break;
            }else {
                if ( buffer[i]=='.' ) {
                     buffer[i]='\0';
                     value[value_count]=atoi(buffer+i-mini_str_len);
                     value_count++;
                     mini_str_len=0;
                }else {
                    mini_str_len++;
                }
            }
       }


      if ( ((value[0]==0 )&& (value[1]==0 )&& (value[2]==0 )&& (value[3]==0 ))
            || ((value[0]==255 )&& (value[1]==255 )&& (value[2]==255 )&& (value[3]==255 ))) {
            cli_print(cli, "%% Invalid input ");
            return CLI_ERROR;
      }

       for (i=0; i < 4; i++) {

            if (value[i] > 255) {
                cli_print(cli, "%% value[i]=%d in  Invalid input",  value[i]);
                return CLI_ERROR;
            }
       }

    if (value_count !=4) {
        cli_print(cli, "%% Invalid input");
        return CLI_ERROR;
    }


    return CLI_OK;

}

int ip_config_check(cs_uint32 ip, cs_uint32 mask, cs_uint32 gw)
{
    int lan_len = 0;
    int non_zero_num = 0;
    int i;
    int mask_1 = 0;

    /* check ip address validity */
    if( ip == 0 || ip == (cs_uint32)-1)
    {
        return 1;
    }

    /* check multicast address */
    if( (ip >> 28) == 0xe)
    {
        return 1;
    }

    /* check loopback address(127.0.0.1) */
    if(ip == 0x7f000001)
    {
        return 1;
    }

    /* check netmask validity */
    if( mask == 0 || mask == 0xffffffff )
    {
        return 1;
    }
    for(i=0; i<32; i++)
    {
        if(mask & (1<<i))
        {
            mask_1 = i;
            break;
        }
    }
    for(i=mask_1; i<32; i++)
    {
        if(mask & (1<<i))
        {
            continue;
        }
        return 1;
    }

    /* check subnet address */
    if(0 == (ip & (~mask)) )
    {
        return 1;
    }

    /* check broadcast address */
    for(i=0; i<32; i++)
    {
        if(mask & (1<<i))
        {
            continue;
        }

        lan_len++;
        if(ip & (1<<i))
        {
            non_zero_num++;
        }
    }

    if(lan_len == non_zero_num)
    {
        return 1;
    }

    /* all 0 is subnet address */
    for(i=0; i<lan_len; i++)
    {
        if( (ip>>i) & 1)
        {
            non_zero_num++;
        }
    }

    if(non_zero_num == 0)
    {
        return 1;
    }

    /* input gateway, need to check gateway validity */
    if(gw)
    {
        if( (gw&mask) != (ip&mask) )
        {
            return 1;
        }

        non_zero_num = 0;
        for(i=0; i<lan_len; i++)
        {
            if( (gw>>i) & 1)
            {
                non_zero_num++;
            }
        }

        // gw ip should not be brocast or all zero
        if(non_zero_num == lan_len || non_zero_num == 0)
        {
            return 1;
        }
    }

    return 0;
}

int cmd_arp_show(struct cli_def *cli, char *command, char *argv[], int argc)
{
    // deal with help
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(0 == argc) {
        epon_request_onu_arp_show(0);
    } else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}

int cmd_arp_clear(struct cli_def *cli, char *command, char *argv[], int argc)
{
    // deal with help
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,
                "<host>", "IP address with X.X.X.X format",
                 NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if (ip_address_check_valid(cli, argv[0]) !=CLI_OK) {
      cli_print(cli, "%% Invalid input");
      return CLI_ERROR;
    }

    if(1 == argc) {
        epon_request_onu_arp_del(argv[0]);
    } else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}

int cmd_ping_host(struct cli_def *cli, char *command, char *argv[], int argc)
{

    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
        case 1:
            return cli_arg_help(cli, 0,
                "<IP address>", "X.X.X.X",
                 NULL);
        default:
            return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(1 == argc)
    {

      if (ip_address_check_valid(cli, argv[0]) !=CLI_OK) {
        cli_print(cli, "%% Invalid input, .");
        return CLI_ERROR;
      }
      
      extern cs_status app_ipintf_ping(char *host);
        app_ipintf_ping(argv[0]);

    } else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}


int cmd_ip_set(struct cli_def *cli, char *command, char *argv[], int argc)
{

    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
            case 1:
                return cli_arg_help(cli, 0,
                    "<IP address>", "X.X.X.X",NULL);
            case 2:
                return cli_arg_help(cli, 0,
                    "<Submask>", "X.X.X.X",NULL);
            default:
                return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(2 == argc)
    {
        cs_uint32 ip, mask;
        db_mgmt_t  mgmt_info;
             
        if(cs_str2ip(argv[0],&ip)||cs_str2ip(argv[1],&mask))
        {
            cli_print(cli, "%% Invalid IP or Mask string ");
            return CLI_ERROR;
        }
        
        if (epon_request_onu_check_ip_address(ip, mask,0)) 
        {
            cli_print(cli, "%% Invalid IP or Mask ");
            return CLI_ERROR;
        }

        memset(&mgmt_info,0x00,sizeof(mgmt_info));
        db_read(DB_MGMT_INFO_ID,(cs_uint8*) &mgmt_info,sizeof(mgmt_info));

        epon_request_onu_cur_ip_save(ip,mask);

        mgmt_info.ip = ip;
        mgmt_info.mask = mask;
        db_write(DB_MGMT_INFO_ID,(cs_uint8*) &mgmt_info,sizeof(mgmt_info));    

        cli_print(cli, "%% Save and Reset to valid.");
    } else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}

int cmd_ip_get(struct cli_def *cli, char *command, char *argv[], int argc)
{

    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(0 == argc)
    {
        cs_int8 cur_ip[16],cur_mask[16];
        cs_int8 new_ip[16],new_mask[16];
        cs_uint32 ip, mask ;
        db_mgmt_t  mgmt_info;

        memset(cur_ip,0x00,sizeof(cur_ip));
        memset(cur_mask,0x00,sizeof(cur_mask));
        memset(new_ip,0x00,sizeof(new_ip));
        memset(new_mask,0x00,sizeof(new_mask));

        memset(&mgmt_info,0x00,sizeof(mgmt_info));
        db_read(DB_MGMT_INFO_ID,(cs_uint8*) &mgmt_info,sizeof(mgmt_info));

        if(epon_request_onu_cur_ip_get(&ip,&mask))
        {
            ip = mgmt_info.ip;
            mask = mgmt_info.mask;
        }

        cs_ip2str(cur_ip,ip);
        cs_ip2str(cur_mask,mask);

        cs_ip2str(new_ip,mgmt_info.ip);
        cs_ip2str(new_mask,mgmt_info.mask);

        cli_print(cli, "%% Current IP/Mask:%s/%s, New IP/Mask: %s/%s.",
                    cur_ip,cur_mask,new_ip,new_mask);   
    } 
    else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}

int cmd_inband_ip_set(struct cli_def *cli, char *command, char *argv[], int argc)
{

    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
            case 1:
                return cli_arg_help(cli, 0,
                    "<IP address>", "X.X.X.X",NULL);
            case 2:
                return cli_arg_help(cli, 0,
                    "<Submask>", "X.X.X.X",NULL);
            case 3:
                return cli_arg_help(cli, 0,
                    "<Gateway>", "X.X.X.X",NULL);
            default:
                return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(3 == argc)
    {
        cs_uint32 ip, mask, gw;
        db_mgmt_t  mgmt_info;
             
        if(cs_str2ip(argv[0],&ip)||cs_str2ip(argv[1],&mask)||cs_str2ip(argv[2],&gw))
        {
            cli_print(cli, "%% Invalid IP, Mask or gw string ");
            return CLI_ERROR;
        }
        
        if (epon_request_onu_check_ip_address(ip, mask, gw)) 
        {
            cli_print(cli, "%% Invalid IP, Mask or gw ");
            return CLI_ERROR;
        }

        memset(&mgmt_info,0x00,sizeof(mgmt_info));
        db_read(DB_MGMT_INFO_ID,(cs_uint8*) &mgmt_info,sizeof(mgmt_info));

        mgmt_info.inband_ip = ip;
        mgmt_info.inband_mask = mask;
        mgmt_info.gw = gw;
        
        db_write(DB_MGMT_INFO_ID,(cs_uint8*) &mgmt_info,sizeof(mgmt_info));    

        cli_print(cli, "%% Save and Reset to valid.");
    } else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}

int cmd_inband_ip_get(struct cli_def *cli, char *command, char *argv[], int argc)
{

    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(0 == argc)
    {
        cs_int8 inband_ip[16],inband_mask[16],gateway[16];
        cs_uint32 ip, mask, gw ;
        db_mgmt_t  mgmt_info;

        memset(inband_ip,0x00,sizeof(inband_ip));
        memset(inband_mask,0x00,sizeof(inband_mask));
        memset(gateway,0x00,sizeof(gateway));

        memset(&mgmt_info,0x00,sizeof(mgmt_info));
        db_read(DB_MGMT_INFO_ID,(cs_uint8*) &mgmt_info,sizeof(mgmt_info));

        ip = mgmt_info.inband_ip;
        mask = mgmt_info.inband_mask;
        gw = mgmt_info.gw;

        cs_ip2str(inband_ip,ip);
        cs_ip2str(inband_mask,mask);
        cs_ip2str(gateway,gw);

        cli_print(cli, "%% Inband IP/Mask:%s/%s/%s, Inband Mode: %d.",
                    inband_ip,inband_mask,gateway,mgmt_info.inband_enable);   
    } 
    else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}


int cmd_ip_mode(struct cli_def *cli, char *command, char *argv[], int argc)
{

    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
            case 1:
                return cli_arg_help(cli, 0,
                    "<Mode>", "1:Inband;2:Outband",NULL);
            default:
                return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(1 == argc)
    {
        cs_uint32 mode;
        db_mgmt_t  mgmt_info;
             
        mode = cs_atoi(argv[0]);
        
        if((!mode)||(mode>2))
        {
            cli_print(cli, "%% Invalid mode");
            return CLI_ERROR;
        }
        
        memset(&mgmt_info,0x00,sizeof(mgmt_info));
        db_read(DB_MGMT_INFO_ID,(cs_uint8*) &mgmt_info,sizeof(mgmt_info));
        
        if((!mgmt_info.inband_ip)&&(mode==1))
        {
            cli_print(cli, "%% Inband IP can not be Null.");
            return CLI_ERROR;
        }
        
        mgmt_info.inband_enable = mode;
        
        db_write(DB_MGMT_INFO_ID,(cs_uint8*) &mgmt_info,sizeof(mgmt_info));    

        cli_print(cli, "%% Save and Reset to valid.");
    } else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}
#endif

#ifdef HAVE_PPPOE
int cmd_pppoe_plus_get(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
            case 1:
                return cli_arg_help(cli, 0,"<Enable>", "1-Enable;0-Disable",NULL);
            default:
                return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(0 == argc)
    {
        cs_boolean enable = 0;
        cs_status ret = CS_E_OK;
        cs_callback_context_t     context;
        
        ret = epon_request_onu_pppoe_plus_status_get(context, 0, 0, &enable);

        if(ret != CS_E_OK)
        {
            cli_print(cli, "%% PPPOE plus get fail.");
            return CLI_ERROR;
        } 
        
        cli_print(cli, "%% PPPOE plus %s.",enable?"Enable":"Disable"); 
    } 
    else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}

int cmd_pppoe_plus_set(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
            case 1:
                return cli_arg_help(cli, 0,"<Enable>", "1-Enable;0-Disable",NULL);
            default:
                return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(1 == argc)
    {
        cs_uint8 enable = (cs_uint8)iros_strtol(argv[0]);
        cs_status ret = CS_E_OK;

        cs_callback_context_t     context;
        
        ret = epon_request_onu_pppoe_plus_enable(context, 0, 0, enable);

        if(ret != CS_E_OK)
        {
            cli_print(cli, "%% PPPOE plus set fail.");
            return CLI_ERROR;
        }  
        {
            db_onu_svc_t   svc;
            memset(&svc,0x00,sizeof(svc));
            db_read(DB_ONU_SVC_INFO_ID,(cs_uint8 *) &svc, sizeof(svc));
            svc.ppoe_plus_en = enable;
            db_write(DB_ONU_SVC_INFO_ID,(cs_uint8 *) &svc, sizeof(svc));
        }
        cli_print(cli, "%% PPPOE plus set to %d.",enable); 

    } 
    else
    {
        cli_print(cli, "%% Invalid input.");
    }

    return CLI_OK;
}
#endif


#ifdef HAVE_LOOP_DETECT

extern cs_status loop_detect_enable(cs_port_id_t port);
extern cs_status loop_detect_disable(cs_port_id_t port);
extern cs_status loop_detect_unblock(cs_port_id_t port);
int cmd_loopdetect_set(struct cli_def *cli, char *command, char *argv[], int argc)
{

    if(CLI_HELP_REQUESTED)
    {
        switch(argc)
        {
            case 1:
                return cli_arg_help(cli, 0,
                    "<Port ID>", "1-4",NULL);
            case 2:
                return cli_arg_help(cli, 0,
                    "<Enable>", "1-Enable;0-Disable",NULL);
            default:
                return cli_arg_help(cli, argc > 1, NULL);
        }
    }

    if(2 == argc)
    {
        cs_uint8 port = (cs_uint8)iros_strtol(argv[0]);
        cs_uint8 enable = (cs_uint8)iros_strtol(argv[1]);
        cs_status ret = CS_E_OK;

        if((!port)||(port>uni_max_port_num))
        {
            cli_print(cli,"%% Port Id error\n");
            return CLI_ERROR;
        }
        
        if(enable)
        {
            ret = loop_detect_enable(port);
        }
        else
        {
            ret = loop_detect_disable(port);
            if(ret == CS_E_OK)
            {
                ret = loop_detect_unblock(port);
            }
        }

        if(ret != CS_E_OK)
        {
            cli_print(cli,"%% Set loop detect fail, ret=%d\n", ret);
            return CLI_ERROR;
        }

        cli_print(cli, "%% Port %d Loopdetect %d.",port,enable);
        
    } 
    else
    {
        cli_print(cli, "%% Invalid input.");
    }
    return CLI_OK;
}
#endif


int cmd_printf_system_infor(struct cli_def *cli)
{
    show_system_status();
    return 0;
}

#ifdef HAVE_ONU_RSTP
int cmd_printf_stp_infor(struct cli_def *cli)
{
    cs_onu_stp_state_config_t             bridge_state;
    cs_onu_port_stp_state_config_t     port_state;
    cs_int32                                    i=0;
    char * state_table[]={"unkown state","Disable", "Blocking", "Listening","Learning", "Forward", "Broken" };
    char * role_table[]={"DisabledPort","AlternatePort", "BackupPort", "RootPort" , "DesignatedPort","NonStpPort" };


    app_stp_bridge_state_get(&bridge_state);
    /*show Bridge infor*/
    cli_print(cli, "!RSTP");
    cli_print(cli, "%-30s: %s", "bridge RSTP:", bridge_state.stp_enable==1 ? "enabled" :"disabled" );
    cli_print(cli, "%-30s: %d.-%02x%02x.%02x%02x.%02x%02x"
                , "The bridge ID (Pri.-MAC)"
                , bridge_state.bridge_prio,bridge_state.bridge_addr[0], bridge_state.bridge_addr[1]
                , bridge_state.bridge_addr[2], bridge_state.bridge_addr[3]
                , bridge_state.bridge_addr[4], bridge_state.bridge_addr[5]);

    cli_print(cli, "%-30s: Hello Time %d sec, Max Age %d sec, Forward Delay %d sec"
                , "The bridge times"
                , bridge_state.hello_time, bridge_state.max_age, bridge_state.forward_delay  );
    cli_print(cli, "%-30s: %d.-%02x%02x.%02x%02x.%02x%02x"
                ,"Root bridge ID(Pri.-MAC)"
                , bridge_state.root_bridge_prio,bridge_state.root_bridge_addr[0], bridge_state.root_bridge_addr[1]
                , bridge_state.root_bridge_addr[2], bridge_state.root_bridge_addr[3]
                , bridge_state.root_bridge_addr[4], bridge_state.root_bridge_addr[5]);

    cli_print(cli, "%-30s: %lld"
        , "Root path cost"
        , bridge_state.root_path_cost );

    cli_print(cli, "%-30s: %d"
        , "Root port"
        , bridge_state.root_port);

    cli_print(cli, "\r\n");
    /*show port infor*/
    for (i=1; i <uni_max_port_num+1  ;i++) {
        cli_print(cli, "!");
        app_stp_port_state_get(i , &port_state);
        cli_print(cli, "Port %d  of bridge is %s", i, state_table[(int)port_state.state] );
        cli_print(cli, "%-30s: %s"
                            , "Port spanning tree protocol", port_state.stp_enable==1 ?"enabled":"disable");
        cli_print(cli, "%-30s: %s", "Port role",  role_table[port_state.role] );
        cli_print(cli, "%-30s: %lld","Port path cost", port_state.path_cost );
        cli_print(cli, "%-30s: %d", "Port priority", port_state.port_prio );

        cli_print(cli, "%-30s:%d.-%02x%02x.%02x%02x.%02x%02x"
                , "Designated bridge ID(Pri.-MAC)"
                , port_state.designated_bridge_prio,port_state.designated_bridge_addr[0], port_state.designated_bridge_addr[1]
                , port_state.designated_bridge_addr[2], port_state.designated_bridge_addr[3]
                , port_state.designated_bridge_addr[4], port_state.designated_bridge_addr[5]);

        cli_print(cli, "The Port is a %s port", port_state.oper_edge==1 ?"edged":"non-edged");

        cli_print(cli, "Connected to a %s LAN segment", port_state.oper_point2point==1 ?"point-to-point ":"non-point-to-point ");

        cli_print(cli, "BPDU sent:" );
        cli_print(cli, "RST: %lld, Config BPDU: %lld, TCN: %lld "
                , port_state.tx_rstp_bpdu_cnt, port_state.tx_cfg_bpdu_cnt, port_state.tx_tcn_bpdu_cnt);

        cli_print(cli, "BPDU received:" );
        cli_print(cli, "RST: %lld, Config BPDU: %lld, TCN: %lld "
                , port_state.rx_rstp_bpdu_cnt, port_state.rx_cfg_bpdu_cnt, port_state.rx_tcn_bpdu_cnt);
        cli_print(cli, "\r\n");

    }


    return 0;

}
#endif


int cmd_printf_age_time_infor(struct cli_def *cli)
{
    cs_uint32  time = 0;
    cs_callback_context_t     context;

    epon_request_onu_fdb_age_get(context, 0, 0, &time);

    cli_print(cli, "!");
    cli_print(cli, "%-30s: %d", "FDB age time", time);
    cli_print(cli, "\r\n");

    return 0;
}

#ifdef HAVE_MPORTS
int cmd_printf_isolation_infor(struct cli_def *cli)
{
    cs_boolean enable=0;
    cs_callback_context_t context;

    cli_print(cli, "!isolation");
    epon_request_onu_port_isolation_get(context, 0, 0, &enable);
    cli_print(cli, "%s isolation\n", enable==1 ? "enable": "disable");
    
    cli_print(cli, "\r\n");

    return 0;
}
#endif


int cmd_show_run_system(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;




    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }


    /*system inforamtion*/
    cmd_printf_system_infor(cli);
    return 0;


}

#ifdef  HAVE_MC_CTRL

int cmd_printf_igmp_infor(struct cli_def *cli)
{
    cs_sdl_mc_vlan_act_t   opr=0;
    mc_mode_t igmp_mode;
    cs_int32    i=0,j=0;
    cs_int32    portid=0;
    cs_uint16  max_group=0;
    cs_uint8    buffer[1280];
    cs_uint16 num=0;
    mc_ipv4_grp_t v4_tbl[64];
    cs_uint32 v4_tbl_len = 64;
    mc_ipv6_grp_t v6_tbl[64];
    cs_uint32 v6_tbl_len = 64;
    cs_uint32 id;
    unsigned int   out_group=0, port_map=0;
    cs_sdl_vlan_cfg_t             swap_rule[20];
    unsigned short  * pVlanId=(unsigned short  *) buffer;
    char                temp_buffer[128];
    int                 str_len=0;
    oam_ctc_onu_mc_control_vlan_mac_t *pDaVid= (oam_ctc_onu_mc_control_vlan_mac_t *)buffer;
    
    cs_status    ret = CS_E_OK;
    cs_callback_context_t context;
    
    /*show multicast mode*/
    ret = mc_mode_get(0,&igmp_mode);
    cli_print(cli, "!IGMP");
    cli_print(cli, "%-30s:%s\n" ,"IGMP mode",  igmp_mode == MC_SNOOPING ? "IGMP snooping": "CTC IGMP");

    if (igmp_mode == MC_SNOOPING) {
        /*IGMP group entry*/
        str_len=0;

        cli_print(cli, "%-30s %-8s", "IPv4 multicast group", "port id");
        
        mc_get_grp_tbl(0, v4_tbl, &v4_tbl_len, v6_tbl, &v6_tbl_len);

        for(id = 0; id < v4_tbl_len; id++) {
            out_group = v4_tbl[id].grp;
            port_map = v4_tbl[id].port_map;
            
            sprintf(buffer, "%d.%d.%d.%d "
            , ((out_group&0xff000000) >>24), ((out_group&0xff0000) >>16), (( out_group&0xff00) >>8),  (out_group &0xff) );
            str_len=0;
            for (i=1; i <uni_max_port_num+1; i++) {
                if (port_map & (0x1 << i)){
                    str_len+=sprintf(temp_buffer+str_len, "%d ", i );
                }
            }
            cli_print(cli, "%-30s %-8s", buffer, temp_buffer);
        }

        cli_print(cli, "%-30s %-8s", "IPv6 multicast group", "port id");
        for(id = 0; id < v6_tbl_len; id++) {
            port_map = v6_tbl[id].port_map;
            
            sprintf(buffer, "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x ", 
                    v6_tbl[id].grp[1], v6_tbl[id].grp[0],
                    v6_tbl[id].grp[3], v6_tbl[id].grp[2],
                    v6_tbl[id].grp[5], v6_tbl[id].grp[4],
                    v6_tbl[id].grp[7], v6_tbl[id].grp[6]);
            
            str_len=0;
            for (i=1; i <uni_max_port_num+1; i++) {
                if (port_map & (0x1 << i)){
                    str_len+=sprintf(temp_buffer+str_len, "%d ", i );
                }
            }
            cli_print(cli, "%-30s %-8s", buffer, temp_buffer);
            
        }

        cli_print(cli, "\r\n");

    }else     {
        cli_print(cli, "IGMP control list :");

        mc_ctc_ctrl_get_entries(buffer, &num);
        cli_print(cli, "%-30s %-6s %-10s", "MAC", "portid", "VLAN ID");

        str_len=0;
        for (i=0; i < num; i++) {
           sprintf(temp_buffer,"%02x-%02x-%02x-%02x-%02x-%02x", 
            pDaVid[i].mc_da[0], pDaVid[i].mc_da[1], 
            pDaVid[i].mc_da[2], pDaVid[i].mc_da[3],
            pDaVid[i].mc_da[4], pDaVid[i].mc_da[5] );
            cli_print(cli, "%-30s %-6d %-10d", temp_buffer, cli_ntohs(pDaVid[i].userId),cli_ntohs(pDaVid[i].vlanId));

        }
        cli_print(cli, "\r\n");
    }

    for (portid = 1 ; portid < uni_max_port_num+1; portid++) {
        cli_print(cli , "!ethernet port %d", portid );
        if ( igmp_mode == MC_SNOOPING) {
            
             /*max group*/
            mc_port_max_grp_get(0, portid, &max_group);
            cli_print(cli, "%-30s:%d","max multicast group" , max_group);

              /*multicast vlan*/
              ret = epon_request_onu_mc_vlan_get(context,0,0, portid, pVlanId, &num);
            if (ret == 0) {
                str_len = 0;
                for (j=0; j < num; j++) {
                    str_len += sprintf(temp_buffer+str_len, "%d%s",cli_ntohs(pVlanId[j]) ,((j+1)%8)==0?"\r\n":" ");
                }
                if ( num >0 ) {
                    cli_print(cli, "%-30s: %s",  "CTC multicast VLAN", temp_buffer);
                }
            }
        }

        /*Multicast Tagstrip*/
        str_len = 0;

        epon_request_onu_mc_vlan_action_get(context,0,0, portid, &opr, swap_rule,&num);

        if (opr == SDL_MC_VLAN_TAG_KEEP) {
            str_len += sprintf(buffer+str_len, "port no tag strip" );
        }
        if (opr == SDL_MC_VLAN_TAG_STRIP) {
            str_len+=sprintf(buffer+str_len, "port tag strip" );
        }

        if (opr == SDL_MC_VLAN_TAG_SWAP) {
            
            str_len+=sprintf (buffer+str_len, "%s " , "port tag replace multicast vlan by iptv vlan:");
            for (j=0; j < num;  ++j) {
                str_len+=sprintf(buffer+str_len, " (%d, %d)", swap_rule[j].c_vlan.vid, swap_rule[j].s_vlan.vid);
            }

        }
        
        cli_print (cli, "%-30s"  , buffer);


        cli_print(cli, "\r\n");
    }


    cli_print(cli, "\r\n");

    return CLI_OK;

}

int cmd_show_run_igmp(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }


    cmd_printf_igmp_infor( cli);
     return 0;

}
#endif

int cmd_show_run_qos(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

     return 0;
}

#ifdef HAVE_ONU_RSTP
int cmd_show_run_stp(struct cli_def *cli, char *command, char *argv[], int argc)
{
    if (CLI_HELP_REQUESTED)
        return CLI_HELP_NO_ARGS;

    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }


    cmd_printf_stp_infor( cli);

    return 0;

}
#endif

int cmd_sys_info_get(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_uint32 i ; 
    cs_uint8  pVer[32] ={0};
    cs_uint32 ip, mask ,gw;
    cs_int8 strIp[16];
    cs_callback_context_t     context;
    cs_uint16 llid; 
    cs_uint8 reg_status;
    cs_uint64 time_end, lSecs, lMins;
    cs_uint32 nDays, nHours;
    cs_mac_t mac;
    cs_uint8 vendorId[32 ] = {0};
    cs_uint8 model_name[32] = {0};
    cs_uint16  chip_version = 0;


    cli_print(cli,"ONU System info:");

    cli_print(cli,"ONU Type: %s",ONU_CLIENT_PRODUCT_MODEL);

    startup_config_read(CFG_ID_VENDOR_ID, sizeof(vendorId), vendorId);
    startup_config_read(CFG_ID_ONU_MOD_ID, sizeof(model_name), model_name);



    strcpy(pVer,ONU_SW_VERSION);
    i = 0; 
    while(i < strlen(pVer)){
        if(pVer[i] == '_'){
            pVer[i] = '\0';
            break;
        }
        i++;
    }
    cli_print(cli,"ONU SW Ver: %s",pVer);
    
    cli_print(cli,"ONU Firmware Ver: %d.%d.%d.%d",
            atoi(IROS_ONU_APP_VER_MAJOR),
            atoi(IROS_ONU_APP_VER_MINOR),
            atoi(IROS_ONU_APP_VER_REVISION),
            atoi(IROS_ONU_APP_VER_BUILD));
    


    epon_request_onu_mpcp_llid_get( context,
            ONU_DEVICEID_FOR_API,
            ONU_LLIDPORT_FOR_API,
            &llid);
    
    cli_print(cli,"LLID: 0x%04x",(llid&0xFFFF));
    cli_print(cli,"Chip Vendor ID: %s",ONU_CHIPSET_VENDOR_ID);
    
    onu_get_chip_version(&chip_version);
    cli_print(cli,"Chip Model ID: %x",chip_version);
    cli_print(cli,"Chipset Revision: %s",ONU_CHIPSET_VER);
    cli_print(cli,"Chipset IC Ver: %s",ONU_CHIPSET_MADE_DATE);
    startup_config_read(CFG_ID_MAC_ADDRESS,CS_MACADDR_LEN, &mac.addr[0]);
    cli_print(cli,"Mac/ID: %02X:%02X:%02X:%02X:%02X:%02X",
                            mac.addr[0], 
                            mac.addr[1], 
                            mac.addr[2], 
                            mac.addr[3], 
                            mac.addr[4], 
                            mac.addr[5]);
    
    epon_request_onu_ip_config_get(&ip,&mask,&gw);
    memset(strIp,0,sizeof(strIp));
    cs_ip2str(strIp,ip);
    cli_print(cli,"IP Address: %s",strIp);
    memset(strIp,0,sizeof(strIp));
    cs_ip2str(strIp,mask);
    cli_print(cli,"Netmask: %s",strIp);

    time_end = cs_current_time();
    lSecs = time_end/1000;
    lMins = lSecs/60;
    nHours = lMins/60;
    nDays = nHours/24;
    cli_print(cli,"Uptime: %d Day,%d Hour,%d Min,%d Seconds",nDays, nHours % 24, (int)(lMins % 60), (int)(lSecs % 60));

    epon_request_onu_mpcp_reg_status_get(context,ONU_DEVICEID_FOR_API,ONU_LLIDPORT_FOR_API,&reg_status);
    cli_print(cli,"Reg Status: %d",reg_status);

    return CLI_OK;
}

int cmd_capability_info_get(struct cli_def *cli, char *command, char *argv[], int argc)
{

    cs_uint8 fec_mode;
    cs_uint8 fec_ability;
    cs_uint8 portNum;
    cs_uint8 haveBattery;
    cs_callback_context_t     context;

    
    startup_config_read(CFG_ID_SWITCH_PORT_NUM, sizeof(portNum), &portNum);

    
#ifdef HAVE_MPORTS
    cli_print(cli,"Cap Service: FE");
    cli_print(cli,"Cap GE Num: 0");
    cli_print(cli,"Cap FE Num: %d",portNum);
    
#else
    cli_print(cli,"Cap Service: GE");
    cli_print(cli,"Cap GE Num: %d",portNum);
    cli_print(cli,"Cap FE Num: 0");
#endif

    cli_print(cli,"Cap Number of POTS ports: 0");
    cli_print(cli,"Cap Number of E1 port: 0");
    cli_print(cli,"Cap Number of US Queues: 8");
    cli_print(cli,"Cap QueueMax per US Port: 8");
    cli_print(cli,"Cap Number of DS Queues: 8");
    cli_print(cli,"Cap QueueMax per DS Port: 8");            


    startup_config_read(CFG_ID_BAK_BATTERY_MODE,sizeof(haveBattery), &haveBattery);
    cli_print(cli,"Battery Backup: %d",haveBattery);

    epon_request_onu_fec_ability_get(context,ONU_DEVICEID_FOR_API,ONU_LLIDPORT_FOR_API,&fec_ability);
    cli_print(cli,"FEC Ability: %d",fec_ability);


    epon_request_onu_fec_get(context,ONU_DEVICEID_FOR_API,ONU_LLIDPORT_FOR_API,&fec_mode);
    cli_print(cli,"FEC Mode: %d",fec_mode);

    return CLI_OK;
}
extern  double log10(double x) ;
int cmd_opm_info_get(struct cli_def *cli, char *command, char *argv[], int argc)
{
    cs_int16 temperature;
    cs_uint16 svcc;
    cs_uint16 tx_bias;
    cs_uint16 tx_power;
    cs_uint16 rx_power;
    cs_uint16 dev_id=0;
    cs_uint8  vendor_name[32]={0};
    cs_uint8  vendor_sn[32]={0};
    cs_callback_context_t   context;
    
    if(!cs_plat_opm_info_get(context,0,0,&dev_id,vendor_name,vendor_sn))
    {
        cli_print(cli,"ONU OPM Status: ");
            
        cli_print(cli,"Dev ID: %x",dev_id);
        cli_print(cli,"Vendor Name: %s",vendor_name);
        cli_print(cli,"Vendor SN: %s",vendor_sn);

    }
    else
    {
        cli_print(cli,"Get ONU OPM Info fail");
    }
    
    if(!cs_plat_opm_status_get(context,0,0,&temperature,&svcc, &tx_bias, &tx_power,&rx_power))
    {
        double txdbm = 0.0, rxdbm = 0.0;
        txdbm = tx_power;
        rxdbm = rx_power;

        txdbm = 10*log10(txdbm*0.0001);
        rxdbm = 10*log10(rxdbm*0.0001);
        
        cli_print(cli,"ONU OPM Status: ");

        cli_print(cli,"Temp: %3.0f",((double)temperature)/256);
        cli_print(cli,"Voltage: %1.2f",((double)svcc)/10000);
        cli_print(cli,"Bias: %3.0f",((double)tx_bias)/500);
        cli_print(cli,"Tx_power: %1.4f mW",((double)tx_power)/10000);
        cli_print(cli,"Rx_power: %1.4f mW",((double)rx_power)/10000);
        cli_print(cli,"Tx_power: %1.4f dbm",txdbm);
        cli_print(cli,"Rx_power: %1.4f dbm",rxdbm);

    }
    else
    {
        cli_print(cli,"Get ONU OPM Status fail");
    }


    return CLI_OK;
}


int cmd_show_run(struct cli_def *cli, char *command, char *argv[], int argc)
{
    
    if(CLI_HELP_REQUESTED)
    {
        return cli_arg_help(cli, 0,
            "show system -- runing config system",NULL);
    }
    if(argc > 0)
    {
        cli_print(cli, "%% Invalid input.");
        return CLI_OK;
    }

    /*system inforamtion*/
    cmd_printf_system_infor(cli);


#ifdef HAVE_MC_CTRL
    /*IGMP*/
    cmd_printf_igmp_infor(cli);
#endif

#ifdef  HAVE_ONU_RSTP
    /*STP*/
    cmd_printf_stp_infor(cli);
#endif

    /*FDB age time*/
    cmd_printf_age_time_infor(cli);

#ifdef HAVE_MPORTS
    /*port isolation mode*/
    cmd_printf_isolation_infor(cli);
#endif

    return CLI_OK;
}

void user_register_command_entry(struct cli_command **cmd_root)
{
    struct cli_command *c;
#ifdef HAVE_IP_STACK
    struct cli_command *arp, *arp_show, *arp_clear;
    struct cli_command *ping;
#endif

    if(CS_E_OK != startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &uni_max_port_num))
    {
        uni_max_port_num = 1;
    }

    c = cli_register_command(cmd_root, NULL, "show",NULL, PRIVILEGE_GUEST, MODE_EXEC,"Show ONU information");
    cli_register_command(cmd_root, c, "system",cmd_sys_info_get, PRIVILEGE_GUEST, MODE_EXEC,"Show ONU system");
    cli_register_command(cmd_root, c, "capability",cmd_capability_info_get, PRIVILEGE_GUEST, MODE_EXEC,"Show ONU capability");
    cli_register_command(cmd_root, c, "optical",cmd_opm_info_get, PRIVILEGE_GUEST, MODE_EXEC,"Show optical status");

    cli_register_command(cmd_root, NULL, "mgmt",cmd_mgmt_mode_int, PRIVILEGE_ADMIN, MODE_CONFIG,"Enter mgmt mode");

#ifdef HAVE_IP_STACK
    /*ARP*/
    arp = cli_register_command(cmd_root, NULL, "arp",  NULL,     PRIVILEGE_ADMIN, MODE_CONFIG_MGMT, "ARP configuration");
    arp_show = cli_register_command(cmd_root, arp, "show",  cmd_arp_show, PRIVILEGE_ADMIN, MODE_CONFIG_MGMT, "Show ARP table");
    arp_clear = cli_register_command(cmd_root, arp, "clear",  cmd_arp_clear,     PRIVILEGE_ADMIN, MODE_CONFIG_MGMT, "Clear ARP table");
    /*Ping*/
    ping = cli_register_command(cmd_root, NULL, "ping",  cmd_ping_host,     PRIVILEGE_ADMIN, MODE_CONFIG_MGMT, "Ping the specified host");
#endif
#ifdef HAVE_LOOP_DETECT
    cli_register_command(cmd_root, NULL, "loopdetect",cmd_loopdetect_set, PRIVILEGE_ADMIN, MODE_CONFIG_MGMT,"Set UNI loopdetect");
#endif

#ifdef HAVE_PPPOE
    c = cli_register_command(cmd_root, NULL, "pppoe",NULL, PRIVILEGE_ADMIN, MODE_CONFIG_MGMT,"PPPOE plus state Get/Set");
    cli_register_command(cmd_root, c, "get",cmd_pppoe_plus_get, PRIVILEGE_ADMIN, MODE_CONFIG_MGMT,"PPPOE plus state Get");
    cli_register_command(cmd_root, c, "set",cmd_pppoe_plus_set, PRIVILEGE_ADMIN, MODE_CONFIG_MGMT,"PPPOE plus state Set");
#endif
    return;
}

