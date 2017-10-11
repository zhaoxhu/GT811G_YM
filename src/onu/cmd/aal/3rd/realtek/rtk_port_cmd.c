#include "rtk_api_ext.h"
#include "cs_utils.h"
#include "plat_common.h"
#include "sdl_l2.h"

extern int diag_printf(const char *fmt, ...);

void __sw_cmd_rtk_port_test_mode_set(int argc, char **argv)
{
    rtk_api_ret_t rt = 0; 
    rtk_port_t    port_id = 0;
    rtk_port_phy_test_mode_t mode = 0;
	
    port_id = (rtk_port_t)iros_strtol(argv[0]);
    mode = (rtk_port_phy_test_mode_t)iros_strtol(argv[1]);
	
    rt = rtk_port_phyTestMode_set(port_id, mode);

    diag_printf("configure return %d\n", rt);  
}

void __sw_cmd_rtk_port_test_mode_get(int argc, char **argv)
{
    rtk_api_ret_t rt = 0; 
    rtk_port_t port_id = 0;
    rtk_port_phy_test_mode_t mode = 0;
    port_id = (rtk_port_t)iros_strtol(argv[0]);
	
    rt = rtk_port_phyTestMode_get(port_id, &mode);

    diag_printf("configure return %d\n", rt); 
	diag_printf("Port_id  : %d\n", port_id);    
    diag_printf("test mode: %d\n", mode);    
}

void __sw_cmd_rtk_port_autoneg_set(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_port_t port_id;
    rtk_port_phy_ability_t phy_abi;

    port_id = (rtk_port_t)iros_strtol(argv[0]);
    phy_abi.AutoNegotiation = iros_strtol(argv[1]);
    phy_abi.Half_10 = iros_strtol(argv[2]);
    phy_abi.Full_10 = iros_strtol(argv[3]);
    phy_abi.Half_100 = iros_strtol(argv[4]);
    phy_abi.Full_100 = iros_strtol(argv[5]);
    phy_abi.Full_1000 = iros_strtol(argv[6]);
    phy_abi.FC = iros_strtol(argv[7]);
    phy_abi.AsyFC = iros_strtol(argv[8]);
    
    rt = rtk_port_phyAutoNegoAbility_set(port_id,&phy_abi);
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_port_autoneg_get(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_port_t port_id;
    rtk_port_phy_ability_t phy_abi;

    port_id = (rtk_port_t)iros_strtol(argv[0]);
    rt = rtk_port_phyAutoNegoAbility_get(port_id,&phy_abi);

    diag_printf("configure return %x\n", rt);
    diag_printf("autoneg: %s\n", phy_abi.AutoNegotiation ? "enable":"disable");    
    diag_printf("half 10M: %s\n", phy_abi.Half_10 ? "enable":"disable");    
    diag_printf("full 10M: %s\n", phy_abi.Full_10 ? "enable":"disable");    
    diag_printf("half 100M: %s\n", phy_abi.Half_100 ? "enable":"disable");    
    diag_printf("full 100M: %s\n", phy_abi.Full_100 ? "enable":"disable");    
    diag_printf("full 1000M: %s\n", phy_abi.Full_1000 ? "enable":"disable");    
    diag_printf("flow control: %s\n", phy_abi.FC ? "enable":"disable");    
    diag_printf("ASY fc: %s\n", phy_abi.AsyFC ? "enable":"disable");    
}

void __sw_cmd_rtk_port_force_set(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_port_t port_id;
    rtk_port_phy_ability_t phy_abi;

    port_id = (rtk_port_t)iros_strtol(argv[0]);
    phy_abi.AutoNegotiation = iros_strtol(argv[1]);
    phy_abi.Half_10 = iros_strtol(argv[2]);
    phy_abi.Full_10 = iros_strtol(argv[3]);
    phy_abi.Half_100 = iros_strtol(argv[4]);
    phy_abi.Full_100 = iros_strtol(argv[5]);
    phy_abi.Full_1000 = iros_strtol(argv[6]);
    phy_abi.FC = iros_strtol(argv[7]);
    phy_abi.AsyFC = iros_strtol(argv[8]);
    
    rt = rtk_port_phyForceModeAbility_set(port_id,&phy_abi);
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_port_force_get(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_port_t port_id;
    rtk_port_phy_ability_t phy_abi;

    port_id = (rtk_port_t)iros_strtol(argv[0]);
    rt = rtk_port_phyForceModeAbility_get(port_id,&phy_abi);

    diag_printf("configure return %x\n", rt);
    diag_printf("autoneg: %s\n", phy_abi.AutoNegotiation ? "enable":"disable");    
    diag_printf("half 10M: %s\n", phy_abi.Half_10 ? "enable":"disable");    
    diag_printf("full 10M: %s\n", phy_abi.Full_10 ? "enable":"disable");    
    diag_printf("half 100M: %s\n", phy_abi.Half_100 ? "enable":"disable");    
    diag_printf("full 100M: %s\n", phy_abi.Full_100 ? "enable":"disable");    
    diag_printf("full 1000M: %s\n", phy_abi.Full_1000 ? "enable":"disable");    
    diag_printf("flow control: %s\n", phy_abi.FC ? "enable":"disable");    
    diag_printf("ASY fc: %s\n", phy_abi.AsyFC ? "enable":"disable");    
}
void __sw_cmd_rtk_port_mdx_set(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_port_t port_id;
    rtk_port_phy_mdix_mode_t phy_mdix;

    port_id = (rtk_port_t)iros_strtol(argv[0]);
    phy_mdix = (rtk_port_phy_mdix_mode_t)iros_strtol(argv[1]);
 
    rt = rtk_port_phyMdx_set(port_id,phy_mdix);
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_port_mdx_get(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_port_t port_id;
    rtk_data_t phy_mdix;

    port_id = (rtk_port_t)iros_strtol(argv[0]);
    rt = rtk_port_phyMdx_get(port_id,&phy_mdix);

    diag_printf("configure return %x\n", rt);
    diag_printf("phy_mdix: %d\n", phy_mdix);    
}

void __sw_cmd_rtk_port_phy_status_get(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_port_t port_id;
    rtk_port_linkStatus_t phy_link;
    rtk_port_speed_t speed;
    rtk_port_duplex_t duplex;
    rtk_data_t nego;

    port_id = (rtk_port_t)iros_strtol(argv[0]);
    rt = rtk_port_phyStatus_get(port_id,&phy_link,&speed,&duplex,&nego);

    diag_printf("configure return %x\n", rt);
    diag_printf("phy_link: %s\n", phy_link ? "link up":"link down");
    switch(speed){
        case PORT_SPEED_10M:
            diag_printf("speed: %s\n",  "10M");
        break;
        case PORT_SPEED_100M:
            diag_printf("speed: %s\n",  "100M");
        break;
        case PORT_SPEED_1000M:
            diag_printf("speed: %s\n",  "1000M");
        break;
        default:
            break;
    }
    diag_printf("duplex: %s\n", duplex ? "full duplex":"half duplex");     
    diag_printf("auto neg: %s\n", nego ? "enable":"disable");     
}

#if 1
void __sw_cmd_rtk_port_mac_force_set(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_port_t port_id;
    rtk_port_mac_ability_t mac_abi;

    port_id = (rtk_port_t)iros_strtol(argv[0]);
    mac_abi.forcemode = iros_strtol(argv[1]);
    mac_abi.speed = iros_strtol(argv[2]);
    mac_abi.duplex = iros_strtol(argv[3]);
    mac_abi.link = iros_strtol(argv[4]);
    mac_abi.nway = iros_strtol(argv[5]);
    mac_abi.txpause = iros_strtol(argv[6]);
    mac_abi.rxpause = iros_strtol(argv[7]);    
    rt = rtk_port_macForceLink_set(port_id,&mac_abi);
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_port_mac_force_get(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_port_t port_id;
    rtk_port_mac_ability_t mac_abi;

    port_id = (rtk_port_t)iros_strtol(argv[0]);
    rt = rtk_port_macForceLink_get(port_id,&mac_abi);

    diag_printf("configure return %x\n", rt);
    diag_printf("forcemode: %s\n", mac_abi.forcemode ? "enable":"disable");    
    diag_printf("speed: %s\n", mac_abi.speed ? "enable":"disable");    
    diag_printf("duplex: %s\n", mac_abi.duplex ? "enable":"disable");    
    diag_printf("link: %s\n", mac_abi.link ? "enable":"disable");    
    diag_printf("nway: %s\n", mac_abi.nway ? "enable":"disable");    
    diag_printf("txpause: %s\n", mac_abi.txpause ? "enable":"disable");    
    diag_printf("rxpause: %s\n", mac_abi.rxpause ? "enable":"disable");    
}

void __sw_cmd_rtk_port_mac_force_ext0_set(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_mode_ext_t mod;
    rtk_port_mac_ability_t mac_abi;

    mod = (rtk_mode_ext_t)iros_strtol(argv[0]);
    mac_abi.forcemode = iros_strtol(argv[1]);
    mac_abi.speed = iros_strtol(argv[2]);
    mac_abi.duplex = iros_strtol(argv[3]);
    mac_abi.link = iros_strtol(argv[4]);
    mac_abi.nway = iros_strtol(argv[5]);
    mac_abi.txpause = iros_strtol(argv[6]);
    mac_abi.rxpause = iros_strtol(argv[7]);    
    rt = rtk_port_macForceLinkExt0_set(mod,&mac_abi);
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_port_mac_force_ext0_get(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_mode_ext_t mod;
    rtk_port_mac_ability_t mac_abi;

    rt = rtk_port_macForceLinkExt0_get(&mod,&mac_abi);

    diag_printf("configure return %x\n", rt);
    switch(mod){
        case MODE_EXT_DISABLE:
            diag_printf("ext port1: %s\n","disable");    
            break;
            
        case MODE_EXT_RGMII:
            diag_printf("ext port1: %s\n","rgmii");    
            break;
                
        case MODE_EXT_MII_MAC:
            diag_printf("ext port1: %s\n","mii mac");    
            break;
            
        case MODE_EXT_MII_PHY:
            diag_printf("ext port1: %s\n","mii phy");    
            break;
                
        case MODE_EXT_TMII_MAC:
            diag_printf("ext port1: %s\n","tmii mac");    
            break;
            
        case MODE_EXT_TMII_PHY:
            diag_printf("ext port1: %s\n","tmii phy");    
            break;
        case MODE_EXT_RMII_MAC:
            diag_printf("ext port1: %s\n","rmii mac");    
            break;
            
        case MODE_EXT_RMII_PHY:
            diag_printf("ext port1: %s\n","rmii phy");    
            break;
        case MODE_EXT_GMII:
            diag_printf("ext port1: %s\n","gmii");    
            break;
            
        case MODE_EXT_RGMII_33V:
            diag_printf("ext port1: %s\n","rgmii 33v");    
            break;
        default:
            break;
    }
    
    diag_printf("forcemode: %s\n", mac_abi.forcemode ? "enable":"disable");    
    diag_printf("speed: %s\n", mac_abi.speed ? "enable":"disable");    
    diag_printf("duplex: %s\n", mac_abi.duplex ? "enable":"disable");    
    diag_printf("link: %s\n", mac_abi.link ? "enable":"disable");    
    diag_printf("nway: %s\n", mac_abi.nway ? "enable":"disable");    
    diag_printf("txpause: %s\n", mac_abi.txpause ? "enable":"disable");    
    diag_printf("rxpause: %s\n", mac_abi.rxpause ? "enable":"disable");    
}

void __sw_cmd_rtk_port_mac_force_ext1_set(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_mode_ext_t mod;
    rtk_port_mac_ability_t mac_abi;

    mod = (rtk_mode_ext_t)iros_strtol(argv[0]);
    mac_abi.forcemode = iros_strtol(argv[1]);
    mac_abi.speed = iros_strtol(argv[2]);
    mac_abi.duplex = iros_strtol(argv[3]);
    mac_abi.link = iros_strtol(argv[4]);
    mac_abi.nway = iros_strtol(argv[5]);
    mac_abi.txpause = iros_strtol(argv[6]);
    mac_abi.rxpause = iros_strtol(argv[7]);    
    rt = rtk_port_macForceLinkExt1_set(mod,&mac_abi);
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_port_mac_force_ext1_get(int argc, char **argv)
{
    rtk_api_ret_t rt; 
    rtk_mode_ext_t mod;
    rtk_port_mac_ability_t mac_abi;

    rt = rtk_port_macForceLinkExt1_get(&mod,&mac_abi);

    diag_printf("configure return %x\n", rt);
    switch(mod){
        case MODE_EXT_DISABLE:
            diag_printf("ext port0: %s\n","disable");    
            break;
            
        case MODE_EXT_RGMII:
            diag_printf("ext port0: %s\n","rgmii");    
            break;
                
        case MODE_EXT_MII_MAC:
            diag_printf("ext port0: %s\n","mii mac");    
            break;
            
        case MODE_EXT_MII_PHY:
            diag_printf("ext port0: %s\n","mii phy");    
            break;
                
        case MODE_EXT_TMII_MAC:
            diag_printf("ext port0: %s\n","tmii mac");    
            break;
            
        case MODE_EXT_TMII_PHY:
            diag_printf("ext port0: %s\n","tmii phy");    
            break;
        case MODE_EXT_RMII_MAC:
            diag_printf("ext port0: %s\n","rmii mac");    
            break;
            
        case MODE_EXT_RMII_PHY:
            diag_printf("ext port0: %s\n","rmii phy");    
            break;
        case MODE_EXT_GMII:
            diag_printf("ext port0: %s\n","gmii");    
            break;
            
        case MODE_EXT_RGMII_33V:
            diag_printf("ext port0: %s\n","rgmii 33v");    
            break;
        default:
            break;
    }
    
    diag_printf("forcemode: %s\n", mac_abi.forcemode ? "enable":"disable");    
    diag_printf("speed: %s\n", mac_abi.speed ? "enable":"disable");    
    diag_printf("duplex: %s\n", mac_abi.duplex ? "enable":"disable");    
    diag_printf("link: %s\n", mac_abi.link ? "enable":"disable");    
    diag_printf("nway: %s\n", mac_abi.nway ? "enable":"disable");    
    diag_printf("txpause: %s\n", mac_abi.txpause ? "enable":"disable");    
    diag_printf("rxpause: %s\n", mac_abi.rxpause ? "enable":"disable");    
}

#endif

void __sw_cmd_rtk_port_phy_reg_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_port_phy_reg_t phy_reg;
    rtk_port_phy_data_t data;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    phy_reg = (rtk_port_phy_reg_t)iros_strtol(argv[1]); 
    data = (rtk_port_phy_data_t)iros_strtol(argv[2]);

    rt = rtk_port_phyReg_set(port_id,phy_reg,data);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_port_phy_reg_get(int argc, char **argv)
{
    rtk_api_ret_t rt = 0;
    rtk_port_t port_id;
    rtk_port_phy_reg_t phy_reg;
    rtk_port_phy_data_t data;
        
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    phy_reg = (rtk_port_phy_reg_t)iros_strtol(argv[1]);
    rt |= rtk_port_phyReg_get(port_id,phy_reg,&data);       
    diag_printf("configure return %x\n", rt);    
    diag_printf("phy_reg %d, data 0x%x\n", phy_reg,data);
}

void __sw_cmd_rtk_port_back_press_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_enable_t enable;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    enable = (rtk_enable_t)iros_strtol(argv[1]); 

    rt = rtk_port_backpressureEnable_set(port_id,enable);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_port_back_press_get(int argc, char **argv)
{
       rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_enable_t enable;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    rt = rtk_port_backpressureEnable_get(port_id,&enable);    
    diag_printf("configure return %x\n", rt);
    diag_printf("back press is %s\n", enable ?"enable":"disable");
}

void __sw_cmd_rtk_port_admin_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_enable_t enable;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    enable = (rtk_enable_t)iros_strtol(argv[1]); 

    rt = rtk_port_adminEnable_set(port_id,enable);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_port_admin_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_enable_t enable;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    rt = rtk_port_adminEnable_get(port_id,&enable);    
    diag_printf("configure return %x\n", rt);
    diag_printf("admin is %s\n", enable ?"enable":"disable");
}

void __sw_cmd_rtk_port_isolation_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_portmask_t iso;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    iso.bits[0] = iros_strtol(argv[1]); 

    rt = rtk_port_isolation_set(port_id,iso);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_port_isolation_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_portmask_t iso;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    rt = rtk_port_isolation_get(port_id,&iso);    
    diag_printf("configure return %x\n", rt);
    diag_printf("isolation bits is 0x%x\n", iso.bits[0]);
}


void __sw_cmd_rtk_port_all_phy_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_enable_t enable;
    enable = (rtk_enable_t)iros_strtol(argv[0]); 

    rt = rtk_port_phyEnableAll_set(enable);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_port_all_phy_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_enable_t enable;
    
    rt = rtk_port_phyEnableAll_get(&enable);    
    diag_printf("configure return %x\n", rt);
    diag_printf("all phy is %s\n", enable ?"enable":"disable");
}

void __sw_cmd_rtk_port_stats_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_stat_port_cntr_t port_cnt;

    
    memset((cs_uint8*)&port_cnt, 0, sizeof(port_cnt)); 
    port_id = (rtk_port_t)iros_strtol(argv[0]); 
    rt = rtk_stat_port_getAll(port_id,&port_cnt);
    
    diag_printf("configure return %x\n", rt);
    if(rt != CS_E_OK){
        return;
    }
    diag_printf("ifInOctets                         %lld\n",port_cnt.ifInOctets);
    diag_printf("dot3StatsFCSErrors                 %d\n",port_cnt.dot3StatsFCSErrors);
    diag_printf("dot3StatsSymbolErrors              %d\n",port_cnt.dot3StatsSymbolErrors);
    diag_printf("dot3InPauseFrames                  %d\n",port_cnt.dot3InPauseFrames);
    diag_printf("dot3ControlInUnknownOpcodes        %d\n",port_cnt.dot3ControlInUnknownOpcodes);
    diag_printf("etherStatsFragments                %d\n",port_cnt.etherStatsFragments);
    diag_printf("etherStatsJabbers                  %d\n",port_cnt.etherStatsJabbers);
    diag_printf("ifInUcastPkts                      %d\n",port_cnt.ifInUcastPkts);
    diag_printf("etherStatsDropEvents               %d\n",port_cnt.etherStatsDropEvents);
    diag_printf("etherStatsOctets                   %lld\n",port_cnt.etherStatsOctets);
    diag_printf("etherStatsUndersizePkts            %d\n",port_cnt.etherStatsUndersizePkts);
    diag_printf("etherStatsOversizePkts             %d\n",port_cnt.etherStatsOversizePkts);
    diag_printf("etherStatsPkts64Octets             %d\n",port_cnt.etherStatsPkts64Octets);
    diag_printf("etherStatsPkts65to127Octets        %d\n",port_cnt.etherStatsPkts65to127Octets);
    diag_printf("etherStatsPkts128to255Octets       %d\n",port_cnt.etherStatsPkts128to255Octets);
    diag_printf("etherStatsPkts256to511Octets       %d\n",port_cnt.etherStatsPkts256to511Octets);
    diag_printf("etherStatsPkts512to1023Octets      %d\n",port_cnt.etherStatsPkts512to1023Octets);
    diag_printf("etherStatsPkts1024toMaxOctets      %d\n",port_cnt.etherStatsPkts1024toMaxOctets);

    diag_printf("etherStatsMcastPkts                %d\n",port_cnt.etherStatsMcastPkts);
    diag_printf("etherStatsBcastPkts                %d\n",port_cnt.etherStatsBcastPkts);
    diag_printf("ifOutOctets                        %lld\n",port_cnt.ifOutOctets);
    diag_printf("dot3StatsSingleCollisionFrames     %d\n",port_cnt.dot3StatsSingleCollisionFrames);
    diag_printf("dot3StatsMultipleCollisionFrames   %d\n",port_cnt.dot3StatsMultipleCollisionFrames);
    diag_printf("dot3StatsDeferredTransmissions     %d\n",port_cnt.dot3StatsDeferredTransmissions);
    diag_printf("dot3StatsLateCollisions            %d\n",port_cnt.dot3StatsLateCollisions);
    diag_printf("etherStatsCollisions               %d\n",port_cnt.etherStatsCollisions);
    diag_printf("dot3StatsExcessiveCollisions       %d\n",port_cnt.dot3StatsExcessiveCollisions);
    diag_printf("dot3OutPauseFrames                 %d\n",port_cnt.dot3OutPauseFrames);
    diag_printf("dot1dBasePortDelayExceededDiscards %d\n",port_cnt.dot1dBasePortDelayExceededDiscards);
    diag_printf("dot1dTpPortInDiscards              %d\n",port_cnt.dot1dTpPortInDiscards);
    diag_printf("ifOutUcastPkts                     %d\n",port_cnt.ifOutUcastPkts);
    diag_printf("ifOutMulticastPkts                 %d\n",port_cnt.ifOutMulticastPkts);
    diag_printf("ifOutBrocastPkts                   %d\n",port_cnt.ifOutBrocastPkts);
    diag_printf("outOampduPkts                      %d\n",port_cnt.outOampduPkts);
    diag_printf("inOampduPkts                       %d\n",port_cnt.inOampduPkts);
    
    diag_printf("pktgenPkts                         %d\n",port_cnt.pktgenPkts);
    diag_printf("inMldChecksumError                 %d\n",port_cnt.inMldChecksumError);
    diag_printf("inIgmpChecksumError                %d\n",port_cnt.inIgmpChecksumError);
    diag_printf("inMldSpecificQuery                 %d\n",port_cnt.inMldSpecificQuery);
    diag_printf("inMldGeneralQuery                  %d\n",port_cnt.inMldGeneralQuery);
    diag_printf("inIgmpSpecificQuery                %d\n",port_cnt.inIgmpSpecificQuery);
    diag_printf("inIgmpGeneralQuery                 %d\n",port_cnt.inIgmpGeneralQuery);
    diag_printf("inMldLeaves                        %d\n",port_cnt.inMldLeaves);
    diag_printf("inIgmpLeaves                       %d\n",port_cnt.inIgmpLeaves);
    diag_printf("inIgmpJoinsSuccess                 %d\n",port_cnt.inIgmpJoinsSuccess);
    diag_printf("inIgmpJoinsFail                    %d\n",port_cnt.inIgmpJoinsFail);
    diag_printf("inMldJoinsSuccess                  %d\n",port_cnt.inMldJoinsSuccess);
    diag_printf("inMldJoinsFail                     %d\n",port_cnt.inMldJoinsFail);
    diag_printf("inReportSuppressionDrop            %d\n",port_cnt.inReportSuppressionDrop);
    diag_printf("inLeaveSuppressionDrop             %d\n",port_cnt.inLeaveSuppressionDrop);
    diag_printf("outIgmpReports                     %d\n",port_cnt.outIgmpReports);
    diag_printf("outIgmpLeaves                      %d\n",port_cnt.outIgmpLeaves);

    diag_printf("outIgmpGeneralQuery                %d\n",port_cnt.outIgmpGeneralQuery);
    diag_printf("outIgmpSpecificQuery               %d\n",port_cnt.outIgmpSpecificQuery);
    diag_printf("outMldReports                      %d\n",port_cnt.outMldReports);
    diag_printf("outMldLeaves                       %d\n",port_cnt.outMldLeaves);
    diag_printf("outMldGeneralQuery                 %d\n",port_cnt.outMldGeneralQuery);
    diag_printf("outMldSpecificQuery                %d\n",port_cnt.outMldSpecificQuery);
    diag_printf("inKnownMulticastPkts               %d\n",port_cnt.inKnownMulticastPkts);
}

void __sw_cmd_rtk_port_stats_clear(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;    
    port_id = (rtk_port_t)iros_strtol(argv[0]); 
    rt = rtk_stat_port_reset(port_id);
    diag_printf("configure return %x\n", rt);    
}

void __sw_cmd_rtk_port_ext0_delay_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_data_t rx_delay, tx_delay;
    tx_delay = (rtk_data_t)iros_strtol(argv[0]); 
    rx_delay = (rtk_data_t)iros_strtol(argv[1]); 
    
    rt = rtk_port_rgmiiDelayExt0_set(tx_delay,rx_delay);
    diag_printf("configure return %x\n", rt);    
}

void __sw_cmd_rtk_port_ext0_delay_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_data_t rx_delay, tx_delay;    
    rt = rtk_port_rgmiiDelayExt0_get(&tx_delay,&rx_delay);   
    diag_printf("configure return %x\n", rt);
    diag_printf("tx_delay = %d, rx_delay = %d\n", tx_delay,rx_delay);    
}

void __sw_cmd_rtk_port_loopdetect_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    cs_boolean enable;
    cs_callback_context_t     context;
    enable = (cs_boolean)iros_strtol(argv[0]); 

    rt = epon_request_onu_loopdetect_set(context, 0, 0, enable); 
   
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_port_stp_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_stp_state_t state;
    rtk_port_t port_id = (rtk_port_t)iros_strtol(argv[0]); 
    state = (rtk_stp_state_t)iros_strtol(argv[1]); 

    rt = rtk_stp_mstpState_set(0, port_id, state); 
   
    diag_printf("configure return %x\n", rt);  
}
void __sw_cmd_rtk_port_stp_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_stp_state_t state;
    rtk_port_t port_id = (rtk_port_t)iros_strtol(argv[0]); 

    rt = rtk_stp_mstpState_get(0, port_id, &state); 
   
    diag_printf("configure return %x\n", rt);  
    diag_printf("port %d, stp_state %d\n", port_id, state);  

}
void __sw_cmd_rtk_port_stp_en(int argc, char **argv)
{
   
}

typedef void (*rtk_port_cmd_func_t)(int, char **);
#define __RTK_PORT_PRINT_CMD_HELP(func,cmd,help)  diag_printf("%s %s %-20s --- %s\n","sw",(func),(cmd),(help))

static struct {
    cs_uint8 *name;
    rtk_port_cmd_func_t      exe_func;
    int                     argc;
    cs_uint8                *para;
    cs_uint8                *help;
    }rtk_port_cmds[] = {
    {"autoneg-set" , __sw_cmd_rtk_port_autoneg_set, 9, "[portid][autoneg][H10][F10][H100][F100][F1000][FC][AsyFC]","Phy autoneg set"},
    {"autoneg-get" , __sw_cmd_rtk_port_autoneg_get, 1 , "portid","get phy autoneg ability"},
    {"force-set" , __sw_cmd_rtk_port_force_set, 9, "[port][autoneg][H10][F10][H100][F100][F1000][FC][AsyFC]","Phy force set"},
    {"force-get" , __sw_cmd_rtk_port_force_get, 1 , "portid","get phy force ability"},
    {"phystatus-get" , __sw_cmd_rtk_port_phy_status_get, 1 , "portid","get phy ability"},
#if 1
    {"macforce-set" , __sw_cmd_rtk_port_mac_force_set, 8, "[port][forcemode][speed][duplex][link][nway][txpause][rxpause]","mac force set"},
    {"macforce-get" , __sw_cmd_rtk_port_mac_force_get, 1, "[port]","mac force get"},
    {"macfrcext0-set" , __sw_cmd_rtk_port_mac_force_ext0_set, 8, "[mod][forcemode][speed][duplex][link][nway][txpause][rxpause]","ext0 mac force set"},
    {"macfrcext0-get" , __sw_cmd_rtk_port_mac_force_ext0_get, 0, "cr","ext0 mac force get"},
    {"macfrcext1-set" , __sw_cmd_rtk_port_mac_force_ext1_set, 8, "[mod][forcemode][speed][duplex][link][nway][txpause][rxpause]","ext1 mac force set"},
    {"macfrcext1-get" , __sw_cmd_rtk_port_mac_force_ext1_get, 0, "cr","ext1 mac force get"},     
#endif
    {"phyreg-set" , __sw_cmd_rtk_port_phy_reg_set, 3 , "[portid][reg][value]","set phy reg value"},
    {"phyreg-get" , __sw_cmd_rtk_port_phy_reg_get, 2 , "[portid][reg]","get phy reg value"},
    {"backpress-set" , __sw_cmd_rtk_port_back_press_set, 2 , "[portid][enable]","set port back press"},
    {"backpress-get" , __sw_cmd_rtk_port_back_press_get, 1 , "[portid]","set port back press"},
    {"admin-set" , __sw_cmd_rtk_port_admin_set, 2 , "[portid][admin]","set port admin"},
    {"admin-get" , __sw_cmd_rtk_port_admin_get, 1 , "[portid]","get port admin"},
    {"isolation-set" , __sw_cmd_rtk_port_isolation_set, 2 , "[portid][en]","set port isolation"},
    {"isolation-get" , __sw_cmd_rtk_port_isolation_get, 1 , "[portid]","get port isolation"},
    {"allphy-set" , __sw_cmd_rtk_port_all_phy_set, 1 , "[en]","set phy enable or disable"},
    {"allphy-get" , __sw_cmd_rtk_port_all_phy_get, 0 , "cr","get all phy enable"},    
    {"stats-get" , __sw_cmd_rtk_port_stats_get, 1 , "[port]","get all statistic"},
    {"stats-clr" , __sw_cmd_rtk_port_stats_clear, 1 , "[port]","clear all statistic"},
    {"ext0txdelay-set" , __sw_cmd_rtk_port_ext0_delay_set, 2 , "[tx_delay][rx_delay]","set ext0 port tx and rx delay"},
    {"ext0txdelay-get" , __sw_cmd_rtk_port_ext0_delay_get, 0 , "","get ext0 port tx and rx delay"},
    {"loopdetect-set" , __sw_cmd_rtk_port_loopdetect_set, 1 , "","set loopdetect enable/disable"},
    {"stp-set" , __sw_cmd_rtk_port_stp_set, 2 , "[port][mod]","set stp state"},    
    {"stp-get" , __sw_cmd_rtk_port_stp_get, 1 , "[port]","set stp state"},
    {"stp-en" , __sw_cmd_rtk_port_stp_en, 1 , "[port]","set stp enable/disable"},
    {"mdx-set" , __sw_cmd_rtk_port_mdx_set, 2 , "[port][mode]","set port mdix mode "},
    {"mdx-get" , __sw_cmd_rtk_port_mdx_get, 1 , "[port]","get port mdix mode "},    
    {"test-mode-set" , __sw_cmd_rtk_port_test_mode_set, 2 , "[port][mode]","set port test mode "},
    {"test-mode-get" , __sw_cmd_rtk_port_test_mode_get, 1 , "[port]","get port test mode "},
    }; 
    

void __rtk_port_cmd_help(void)
{
    int i;
    for (i = 0; i < sizeof(rtk_port_cmds) / sizeof(rtk_port_cmds[0]); i++) {  
        __RTK_PORT_PRINT_CMD_HELP("port", rtk_port_cmds[i].name, rtk_port_cmds[i].help);    
    }
}

static rtk_port_cmd_func_t  __rtk_port_find_cmd(
    const char *cmd_str,
    int   argc
)
{
    int i = 0;

    if (NULL == cmd_str) {
        return NULL;
    }

    for (i = 0;
        i < (sizeof(rtk_port_cmds)/ sizeof(rtk_port_cmds[0]));
        ++i) {
        if (cmd_cmp(rtk_port_cmds[i].name, cmd_str)){

            if ((argc - 3) == rtk_port_cmds[i].argc) {
                return rtk_port_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", rtk_port_cmds[i].argc);
                diag_printf("parameters:  %s\n", rtk_port_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __rtk_port_cmd_help();

    return (NULL);

}
    
 void rtk_port_cmd(int argc, char **argv)
 {
    rtk_port_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal fdb  ..."*/

        p_cmd_func = __rtk_port_find_cmd(argv[2], argc);

    }
    else {
        __rtk_port_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}






