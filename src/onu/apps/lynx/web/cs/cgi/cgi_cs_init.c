#include "cgi_cmn.h" 
#include "cgi_cs_user_config.h"


cs_uint8 CGI_MAX_UNI_PORT = 1;
void cgi_org_init()
{
    startup_config_read(
        CFG_ID_SWITCH_PORT_NUM, sizeof(CGI_MAX_UNI_PORT), &CGI_MAX_UNI_PORT);

    cgi_cs_account_init();
}
