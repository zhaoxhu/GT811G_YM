#include "cgi_cmn.h"

#include "cgi_cs_sys_status.h"
#include "cgi_cs_port_config.h"
#include "cgi_cs_user_config.h"
#include "cgi_cs_sys_config.h"

#include "db_instance.h"


CGI_HTTPD_FVAR_REG(enable, CGI_FORM_VAR_INT8_LEN);
CGI_HTTPD_FVAR_REG(fec_mode,   CGI_FORM_VAR_INT32_LEN);
/*CGI_HTTPD_FVAR_REG(sn,   CGI_CTC_SN_LEN+1);*/
CGI_HTTPD_FVAR_REG(loid,   DB_LOID_MAX_LEN+1);
CGI_HTTPD_FVAR_REG(password,   DB_PWD_MAX_LEN+1);
CGI_HTTPD_FVAR_REG(port, CGI_FORM_VAR_INT8_LEN);
CGI_HTTPD_FVAR_REG(phy, CGI_FORM_VAR_INT8_LEN);
CGI_HTTPD_FVAR_REG(auto, CGI_FORM_VAR_INT8_LEN);
CGI_HTTPD_FVAR_REG(mode, CGI_FORM_VAR_INT8_LEN);
CGI_HTTPD_FVAR_REG(pause, CGI_FORM_VAR_INT8_LEN);
CGI_HTTPD_FVAR_REG(action, CGI_FORM_VAR_INT8_LEN);

CGI_HTTPD_FVAR_REG(name, DB_MAX_USR_LEN+1);
CGI_HTTPD_FVAR_REG(pwdold, DB_MAX_PWD_LEN+1);
CGI_HTTPD_FVAR_REG(pwdnew, DB_MAX_PWD_LEN+1);

CGI_HTTPD_FVAR_REG(alarm_sig, CGI_FORM_VAR_INT8_LEN);
CGI_HTTPD_FVAR_REG(alarm_status, CGI_FORM_VAR_INT8_LEN);
CGI_HTTPD_FVAR_REG(log_state, CGI_FORM_VAR_INT8_LEN);
CGI_HTTPD_FVAR_REG(alarm_info, CGI_FORM_VAR_INT8_LEN);

CGI_HTTPD_HANDLER_REG("/cgi-bin/sys_info_get.cgi", cgi_cs_sys_info_get);

CGI_HTTPD_HANDLER_REG("/cgi-bin/ctc_info_get.cgi", cgi_cs_ctc_info_get);

CGI_HTTPD_HANDLER_REG("/cgi-bin/port_config_get.cgi", cgi_cs_port_config_get);
CGI_HTTPD_HANDLER_REG("/cgi-bin/port_config_set.cgi", cgi_cs_port_config_set);

CGI_HTTPD_HANDLER_REG("/cgi-bin/set_admin_password.cgi", cgi_cs_admin_password_set);
CGI_HTTPD_HANDLER_REG("/cgi-bin/set_user_password.cgi", cgi_cs_user_password_set);

CGI_HTTPD_HANDLER_REG("/cgi-bin/get_wlb_image_ext.cgi", cgi_cs_get_wlb_image_ext);
CGI_HTTPD_HANDLER_REG("/cgi-bin/img_verify.cgi", cgi_cs_image_verify);

CGI_HTTPD_HANDLER_REG("/cgi-bin/config_save.cgi", cgi_cs_config_save);
CGI_HTTPD_HANDLER_REG("/cgi-bin/config_download.cgi", cgi_cs_config_download);
CGI_HTTPD_HANDLER_REG("/cgi-bin/config_reset.cgi", cgi_cs_config_reset);

