#ifndef __CGI_CS_SYS_CONFIG_H__
#define __CGI_CS_SYS_CONFIG_H__
#include "cgi_cmn.h"
cgi_status cgi_cs_get_wlb_image_ext(cgi_handler_param_t * p);
cgi_status cgi_cs_image_verify(cgi_handler_param_t * p);
cgi_status cgi_cs_config_save(cgi_handler_param_t * p);
cgi_status cgi_cs_config_download(cgi_handler_param_t * p);
cgi_status cgi_cs_config_reset(cgi_handler_param_t * p);
#endif
