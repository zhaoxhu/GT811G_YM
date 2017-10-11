#ifdef  HAVE_MC_CTRL
#include "plat_common.h"
#include "cs_cmd.h"

#include "mc_api.h"

void mc_cmd_help()
{
    cs_printf("mc mode [mode] - set mc mode(disable, snooping or ctc ctrl)\n");
    cs_printf("mc max_grp [num] - set max group\n");
    cs_printf("mc vl [vl_mode] - set vlan learn mode(IVL - 0, SVL -1)\n");
    cs_printf("mc state - dump mc config and status\n");
    cs_printf("mc prot dump - dump protocol parameters\n");
    cs_printf("mc prot robust [robustness] - set robustness\n");
    cs_printf("mc prot qi [qi] - set query interval\n");
    cs_printf("mc prot qri [qri] - set query rsp interval\n");
    cs_printf("mc prot lmqi [lmqi] - set last_member_query_interval \n");
    cs_printf("mc prot lmqc [lmqc] - set last_member_query_count \n");
    cs_printf("mc fdb port [port] - show port fdb\n");
    cs_printf("mc fdb vlan [vid] - show vlan fdb\n");
    cs_printf("mc grp - show group list\n");
    cs_printf("mc fe [engine] - set mc forward engine, l2 or l3\n");
    cs_printf("mc port dump [port] - dump port config\n");
    cs_printf("mc port leave [port] [mode] - set port fast leave mode\n");
    cs_printf("mc port max_grp [port] [max] - set port max group\n");
    cs_printf("mc port stats [port] - show port stats\n");
    cs_printf("mc port clr_stats [port] - clear port stats\n");
    cs_printf("mc dump [en] - dump on/off\n");
}

sal_cmd_result_t mc_cmd_proc(int argc , char **argv)
{
    if(argc == 2)
    {
        mc_cmd_help();
    }
    else {
        argc--;
        argv++;
        
        if(!strcmp(argv[1], "mode") && argc == 3) { /* set mode */
            if(CS_E_OK != mc_mode_set(0, atoi(argv[2]))) {
                cs_printf("exec fail!\n");
            }
        }
        else if(!strcmp(argv[1], "vl") && argc == 3) { 
            mc_vlan_lrn_set(0, atoi(argv[2]));            
        }
        else if(!strcmp(argv[1], "dump") && argc == 3) { 
            mc_dump_set(atoi(argv[2]));
        }
        else if(!strcmp(argv[1], "state") && argc == 2) {
            mc_mode_t mode;       
            mc_vlan_learn_t lrn_mode;
            mc_fwd_engine_t engine;
            cs_uint16 max_grp, grp_num;
            mc_up_port_cfg_t up_cfg;

            mc_mode_get(0, &mode);
            cs_printf("mc mode: ");
            switch(mode) {
                case MC_DISABLE:
                    cs_printf("Disable\n");
                    break;

                case MC_MANUAL:
                    cs_printf("MC Ctrl\n");
                    break;

                case MC_SNOOPING:
                    cs_printf("Snooping\n");
                    break;

                default:
                    cs_printf("Unsupport mode\n");
                    break;
            }

            mc_vlan_lrn_get(0, &lrn_mode);
            cs_printf("vlan lrn mode: %s\n", lrn_mode ? "SVL" : "IVL");

            mc_fwd_engine_get(0, &engine);
            cs_printf("mc forward engine: ");
            switch(engine) {
                case MC_ENGINE_L2:
                    cs_printf("L2\n");
                    break;

                case MC_ENGINE_GDA_MAC_SIP:
                    cs_printf("GDA_MAC+SIP\n");
                    break;

                case MC_ENGINE_IP:
                    cs_printf("IP\n");
                    break;
            }

            mc_max_grp_get(0, &max_grp);
            cs_printf("max group: %d\n", max_grp);

            mc_grp_num_get(0, &grp_num);
            cs_printf("cur group num: %d\n", grp_num);

            mc_up_port_get(0, &up_cfg);
            cs_printf("up_portid: %d\n", up_cfg.up_port);        
            
        }
        else if(!strcmp(argv[1], "max_grp") && argc == 3) {
            mc_max_grp_set(0, atoi(argv[2]));
        }
        else if(!strcmp(argv[1], "prot")) {
            
            if(!strcmp(argv[2], "dump")) {
                cs_uint32 robustness;
                cs_uint32 query_interval;
                cs_uint32 query_rsp_interval;
                cs_uint32 last_member_query_interval;
                cs_uint32 last_member_cnt;
                                 
                cs_printf("MC protocol parameters: \n");
                
                mc_robustness_get(0, &robustness);
                cs_printf("robustness: %d\n", robustness);

                mc_query_interval_get(0, &query_interval);
                cs_printf("query_interval: %d\n", query_interval);

                mc_query_rsp_interval_get(0, &query_rsp_interval);
                cs_printf("query_rsp_interval: %d\n", query_rsp_interval);

                mc_last_member_query_get(0, &last_member_query_interval);
                cs_printf("last_member_query_interval: %d\n", last_member_query_interval);

                mc_last_member_count_get(0, &last_member_cnt);
                cs_printf("last_member_query_count: %d\n", last_member_cnt);
            }
            else {
               
                if(!strcmp(argv[2], "robust")) {
                    mc_robustness_set(0, atoi(argv[3]));
                }
                else if(!strcmp(argv[2], "qi")) {
                    mc_query_interval_set(0, atoi(argv[3]));;
                }
                else if(!strcmp(argv[2], "qri")) {
                    mc_query_rsp_interval_set(0, atoi(argv[3]));
                }
                else if(!strcmp(argv[2], "lmqi")) {
                    mc_last_member_query_set(0, atoi(argv[3]));
                }
                else if(!strcmp(argv[2], "lmqc")) {
                    mc_last_member_count_set(0, atoi(argv[3]));
                }
                else {
                    return SAL_CMD_INVALID_CMD;
                }

            }
        }
        else if(!strcmp(argv[1], "fdb")) {
            if(!strcmp(argv[2], "port")) {
                mc_port_fdb_list(0, atoi(argv[3]));
            }
            else if(!strcmp(argv[2], "vlan")) {
                mc_vlan_fdb_list(0, atoi(argv[3]));
            }
            else if(!strcmp(argv[2], "list")) {
                mc_fdb_list(0);
            }
        }
        else if(!strcmp(argv[1], "grp")) {
            mc_ipv4_grp_t v4_tbl[64];
            cs_uint32 v4_tbl_len = 64;
            mc_ipv6_grp_t v6_tbl[64];
            cs_uint32 v6_tbl_len = 64;

            cs_uint32 id;
            
            mc_get_grp_tbl(0, v4_tbl, &v4_tbl_len, v6_tbl, &v6_tbl_len);

            cs_printf("ipv4 group - %d\n", v4_tbl_len);
            for(id = 0; id < v4_tbl_len; id++) {
                cs_printf("%02d(0x%02x): %d.%d.%d.%d\n", id+1, v4_tbl[id].port_map,
                    (v4_tbl[id].grp>>24) & 0xff,
                    (v4_tbl[id].grp>>16) & 0xff,
                    (v4_tbl[id].grp>>8) & 0xff,
                    v4_tbl[id].grp & 0xff);
            }

            cs_printf("ipv6 group - %d\n", v6_tbl_len);
            for(id = 0; id < v6_tbl_len; id++) {
                cs_printf("%02d(0x%02x): %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n", 
                    id+1, v6_tbl[id].port_map,
                    v6_tbl[id].grp[1], v6_tbl[id].grp[0],
                    v6_tbl[id].grp[3], v6_tbl[id].grp[2],
                    v6_tbl[id].grp[5], v6_tbl[id].grp[4],
                    v6_tbl[id].grp[7], v6_tbl[id].grp[6]);
            }
        }
        else if(!strcmp(argv[1], "fe") && argc == 3) {
            mc_fwd_engine_set(0, atoi(argv[2]));
        }
        else if(!strcmp(argv[1], "port") && argc >= 4) {            
                        
            if(!strcmp(argv[2], "dump") && argc == 4) {                
                mc_leave_mode_t mode;
                cs_uint16 max_grp, grp_num;

                cs_port_id_t port = atoi(argv[3]);

                if(port > 4) {
                    port = 4;
                }
                
                mc_leave_mode_get(0, port, &mode);
                mc_port_max_grp_get(0, port, &max_grp);
                mc_port_grp_num_get(0, port, &grp_num);

                cs_printf("port %d config: \n", port);
                cs_printf("leave_mode: %s\n", mode ? "Non Fast Leave" : "Fast Leave");
                cs_printf("max group: %d\n", max_grp);
                cs_printf("cur group num: %d\n", grp_num);
                
            }
            else if(!strcmp(argv[2], "max_grp") && argc == 5) {
                cs_uint32 max_grp = atoi(argv[4]);
                cs_port_id_t port = atoi(argv[3]);

                mc_port_max_grp_set(0, port, max_grp);
            }
            else if(!strcmp(argv[2], "leave") && argc == 5) {
                mc_leave_mode_t leave = atoi(argv[4]);
                cs_port_id_t port = atoi(argv[3]);

                mc_leave_mode_set(0, port, leave);
            }
            else if(!strcmp(argv[2], "stats") && argc == 4) {
                mc_port_stats_show(0, atoi(argv[3]));
            }
            else if(!strcmp(argv[2], "clr_stats") && argc == 4) {
                cs_port_id_t port = atoi(argv[3]);
                if(0xff != port) {
                    mc_port_stats_clr(0, port);
                }
                else {
                    for(port = 0; port < 5; port++) {
                        mc_port_stats_clr(0, port);
                    }
                }
            }
        }

    }

    return SAL_CMD_OK;
}

#endif

