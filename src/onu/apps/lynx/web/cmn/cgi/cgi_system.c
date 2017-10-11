#include "cgi_cmn.h" 
#include "plat_common.h"


cgi_status cgi_sys_reset(cgi_handler_param_t * p)
{
    CGI_DEBUG("enter \n");

    cgi_send_int(p,0);

    iros_system_reset(RESET_TYPE_FORCE);

    return CS_E_OK;
}


cgi_status cgi_switch_type_get(cgi_handler_param_t * p)
{
    cs_uint32 is_switch = 1;
	
    CGI_DEBUG("enter \n");
#ifdef HAVE_MPORTS
	cs_uint8 chip_id;
	startup_config_read(CFG_ID_SWITCH_TYPE, 1, &chip_id);
	is_switch = chip_id + 1;
#else
    is_switch = 0;
#endif
    cgi_send_int(p,is_switch);

    return CS_E_OK;
}

