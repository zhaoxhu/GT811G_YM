#include "cs_types.h"
#include "cs_cmd.h"

extern sal_cmd_result_t startup_cfg_cmd_show(int argc, char **argv);
extern sal_cmd_result_t startup_cfg_cmd_reset(int argc, char **argv);
extern sal_cmd_result_t startup_cfg_cmd_set(int argc, char **argv);
extern sal_cmd_result_t startup_cfg_cmd_clear(int argc, char **argv);
extern sal_cmd_result_t startup_cfg_cmd_enc(int argc, char **argv);
extern sal_cmd_result_t startup_cfg_cmd_dump(int argc, char **argv);
static sal_cmd_reg_struct_t startup_cfg_cmd_list[] = 
{
    {startup_cfg_cmd_show,  "show",     "show startup configuration",         NULL, 0},
    {startup_cfg_cmd_reset, "reset",    "reset startup coniguration",         NULL, 0},
    {startup_cfg_cmd_set,   "set",      "set a specific field",               NULL, 0},
    {startup_cfg_cmd_clear, "clear",    "clear a specific field",             NULL, 0},
    {startup_cfg_cmd_enc,   "enc",      "encrypt/decrypt startup config",     NULL, 0},
    {startup_cfg_cmd_dump,  "load",     "Use xmodem to startup bin file from Flash",     NULL, 0},

};
sal_cmd_cfg_t scfg_cmd_cfg = {"scfg", "startup configuration", startup_cfg_cmd_list, sizeof(startup_cfg_cmd_list)/sizeof(sal_cmd_reg_struct_t)};


extern sal_cmd_result_t core_dump_cmd_proc(int argc, char **argv);
static sal_cmd_reg_struct_t core_dump_cmd_list[] = 
{
    {core_dump_cmd_proc,  "show",     "show core dump in flash",         NULL, 0},
    {core_dump_cmd_proc,  "clear",    "clear core dump in flash",        NULL, 0},
};
sal_cmd_cfg_t core_dump_cmd_cfg = {"core", "core dump", core_dump_cmd_list, sizeof(core_dump_cmd_list)/sizeof(sal_cmd_reg_struct_t)};


extern sal_cmd_result_t timer_cmd_proc(int argc, char **argv);
static sal_cmd_reg_struct_t timer_cmd_list[] = 
{
    {timer_cmd_proc,  "show",     "show timer info",     NULL, 0},
    {timer_cmd_proc,  "usdelay",  "test usdelay",        NULL, 0},
};
sal_cmd_cfg_t timer_cmd_cfg = {"timer", "timer cmd", timer_cmd_list, sizeof(timer_cmd_list)/sizeof(sal_cmd_reg_struct_t)};


extern sal_cmd_result_t flash_cmd_proc(int argc, char **argv);
static sal_cmd_reg_struct_t flash_cmd_list[] = 
{
    {flash_cmd_proc,  "part",     "show flash partition info",     NULL, 0},
};
sal_cmd_cfg_t flash_cmd_cfg = {"flash", "flash cmd", flash_cmd_list, sizeof(flash_cmd_list)/sizeof(sal_cmd_reg_struct_t)};


extern sal_cmd_result_t app_cmd_proc(int argc , char **argv);
extern sal_cmd_result_t mc_cmd_proc(int argc , char **argv);

/*
extern sal_cmd_result_t oam_cmd_proc(int argc , char **argv);
*/
static sal_cmd_reg_struct_t app_cmd_list[] = 
{
    {app_cmd_proc,  "errevt",     "Set errored event parameters",     NULL, 0},
    {app_cmd_proc,  "eesim",     "Simulated OAM alarm for event generation",     NULL, 0},
    {app_cmd_proc,  "loop",     "loop detect configure",     NULL, 0},
    {app_cmd_proc,  "dhcp",     "dhcp configure",     NULL, 0},
    {app_cmd_proc,  "pppoe",     "pppoe configure",     NULL, 0},
    {app_cmd_proc,  "dg",     "Set to force ONU to send dying gasp",     NULL, 0},
    {app_cmd_proc,  "crtcevt",     "Set to force ONU to send critical event",     NULL, 0},
    {app_cmd_proc,  "ps",         "Set power saving",     NULL, 0},
    {app_cmd_proc,  "sysmib",     "Set system",     NULL, 0},
    {app_cmd_proc,  "lpbk",     "Set loopback",     NULL, 0},
    {app_cmd_proc,  "pktprt",     "app pktprt set",     NULL, 0},
    {app_cmd_proc,  "show",     "Show application configurations",     NULL, 0},
    {app_cmd_proc,  "1xst",     "app 1xst set",     NULL, 0},
    {app_cmd_proc,  "1xht",     "app 1xht set",     NULL, 0},
    {app_cmd_proc,  "1xrsd",     "app 1xrsd set",     NULL, 0},
    {app_cmd_proc,  "uid",     "app uid set",     NULL, 0},
    {app_cmd_proc,  "pwd",     "app pwd set",     NULL, 0},
    /*{oam_cmd_proc,  "oam",     "oam config/show cmd",     NULL, 0 }, */
#ifdef HAVE_VOIP
    {app_cmd_proc,  "slic",     "slic cmd",     NULL, 0},
#endif

#ifdef HAVE_MC_CTRL
    { mc_cmd_proc,  "mc",     "mc config/status cmd",     NULL, 0 },
#endif


#ifdef HAVE_ONU_RSTP
    { app_cmd_proc,  "rstp",     "rstp cmd",     NULL, 0},
#endif

    {app_cmd_proc,  "vlan",     "app vlan operate",     NULL, 0},
#ifdef HAVE_ZTE_SIJIE
    {app_cmd_proc,  "manage_vlan",     "app manage_vlan operate",     NULL, 0},
#endif /*END_HAVE_ZTE_SIJIE*/
    {app_cmd_proc,  "capture",     "app packet capture",     NULL, 0},
#ifdef HAVE_DB_MANAGMENT
	//added by zhuxh
	{app_cmd_proc,  "db",     "db configuration ",     NULL, 0}
#endif
};
sal_cmd_cfg_t app_cmd_cfg = {"app", "app cmd", app_cmd_list, sizeof(app_cmd_list)/sizeof(sal_cmd_reg_struct_t)};


extern sal_cmd_result_t log_cmd_proc(int argc , char **argv);
static sal_cmd_reg_struct_t log_cmd_list[] = 
{
    {log_cmd_proc,  "show",     "Show system logs",     NULL, 0},
    {log_cmd_proc,  "prtl",     "Set print level",     NULL, 0},
    {log_cmd_proc,  "recl",     "Set record level",     NULL, 0},
    {log_cmd_proc,  "clr",      "Clear all logs",     NULL, 0},
    {log_cmd_proc,  "remote",   "Set remote log level",     NULL, 0},
    {log_cmd_proc,  "remoteip", "Set remote log server ip",     NULL, 0},
};
sal_cmd_cfg_t log_cmd_cfg = {"log", "log cmd", log_cmd_list, sizeof(log_cmd_list)/sizeof(sal_cmd_reg_struct_t)};


extern sal_cmd_result_t peri_cmd_proc(int argc , char **argv);
static sal_cmd_reg_struct_t per_cmd_list[] = 
{
    {peri_cmd_proc,  "eeprom",     "Eeprom commands",     NULL, 0},
    {peri_cmd_proc,  "sflash",     "Sflash commands",     NULL, 0},
    {peri_cmd_proc,  "switch",     "switch ssp commands",     NULL, 0},
    {peri_cmd_proc,  "serdes",     "Serdes commands",     NULL, 0},
    {peri_cmd_proc,  "dma",        "Dma commands",     NULL, 0},
    {peri_cmd_proc,  "esdma",      "Esdma commands",     NULL, 0},
    {peri_cmd_proc,  "i2c",      "I2c commands",     NULL, 0},
    {peri_cmd_proc,  "uart",      "Uart commands",     NULL, 0},
    {peri_cmd_proc,  "mdio",      "Mdio commands",     NULL, 0},
    {peri_cmd_proc,  "gpio",      "Gpio commands",     NULL, 0},
    {peri_cmd_proc,  "led",      "Led commands",     NULL, 0},
    {peri_cmd_proc,  "wdt",      "Watchdog commands",     NULL, 0},
};
sal_cmd_cfg_t per_cmd_cfg = {"per", "per cmd", per_cmd_list, sizeof(per_cmd_list)/sizeof(sal_cmd_reg_struct_t)};


extern sal_cmd_result_t memory_cmd_proc(int argc , char **argv);
static sal_cmd_reg_struct_t memory_cmd_list[] = 
{
    {memory_cmd_proc,  "show",     "Show memory information",     NULL, 0},
};
sal_cmd_cfg_t memory_cmd_cfg = {"mem", "memory cmd", memory_cmd_list, sizeof(memory_cmd_list)/sizeof(sal_cmd_reg_struct_t)};

void plat_cmd_init()
{
    sal_cmd_reg(&scfg_cmd_cfg);
    sal_cmd_reg(&core_dump_cmd_cfg);
    sal_cmd_reg(&timer_cmd_cfg);
    sal_cmd_reg(&flash_cmd_cfg);
    sal_cmd_reg(&app_cmd_cfg);
    sal_cmd_reg(&log_cmd_cfg);
    sal_cmd_reg(&per_cmd_cfg);
    sal_cmd_reg(&memory_cmd_cfg);
}

