#ifndef __CGI_CS_SYS_STATUS_H__
#define __CGI_CS_SYS_STATUS_H__

#define CGI_MAX_START_CFG_LEN  100
#define CGI_IP_ADDRESS_LEN          15

cgi_status cgi_cs_sys_info_get(cgi_handler_param_t * p);
cgi_status cgi_cs_ctc_info_get(cgi_handler_param_t * p);

#endif

