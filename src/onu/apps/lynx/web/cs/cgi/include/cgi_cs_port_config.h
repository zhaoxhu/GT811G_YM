#ifndef __CGI_CS_PORT_CONFIG_H__
#define __CGI_CS_PORT_CONFIG_H__

typedef enum
{
    CGI_CTC_PORT_MODE_AUTO =  0,
    CGI_CTC_PORT_MODE_UNKOWN =  0,
    CGI_CTC_PORT_MODE_10M_HALF,
    CGI_CTC_PORT_MODE_10M_FULL,
    CGI_CTC_PORT_MODE_100M_HALF,
    CGI_CTC_PORT_MODE_100M_FULL,
    CGI_CTC_PORT_MODE_1000M_FULL
} cgi_cs_port_work_mode_e;


cgi_status cgi_cs_port_config_get(cgi_handler_param_t * p) ;
cgi_status cgi_cs_port_config_set(cgi_handler_param_t * p);

#endif

