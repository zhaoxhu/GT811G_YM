#include "cgi_cmn.h"
#include "cgi_auth.h"

/* Only exist in athttpd*/
CYG_HTTPD_AUTH_TABLE_ENTRY(hal_pon2_entry,"/","epon","user","user",CYG_HTTPD_AUTH_BASIC);

void cgi_get_cookie_str(char *cokieStr)
{
     strcpy(cokieStr, CGI_DEFAULT_COOKIE);
}

cs_boolean cgi_do_authorization(char *user, char *passwd)
{
    cs_boolean is_authorized = FALSE;

    if ((strcmp(CGI_DEFAULT_USER_NAME,user) == 0)
            && (strcmp(CGI_DEFAULT_PASSWD, passwd) == 0)) {
        is_authorized = TRUE;
    }
    return is_authorized;
}


void cgi_auth_init()
{
    cyg_auth_handlers_reg(cgi_do_authorization,
            cgi_get_cookie_str,
            NULL);
}
