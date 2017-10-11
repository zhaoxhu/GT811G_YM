#ifndef __CGI_CS_USER_CONFIG_H__
#define __CGI_CS_USER_CONFIG_H__

#include "cgi_cmn.h"

typedef enum{ 
    CGI_CTC_ADMIN = 0,
    CGI_CTC_USER
}cgi_cs_account_type_e;

cgi_status cgi_cs_admin_password_set(cgi_handler_param_t *p);
cgi_status cgi_cs_user_password_set(cgi_handler_param_t *p);
void cgi_cs_account_init();

#endif
