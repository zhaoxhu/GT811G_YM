#ifndef __CGI_CMN_H__
#define __CGI_CMN_H__
#include <pkgconf/system.h>
#include <cyg/fileio/fileio.h>
#include <network.h>
#include <cyg/kernel/kapi.h>
#include <cyg/athttpd/http.h>
#include <cyg/athttpd/auth.h>
#include <cyg/athttpd/socket.h>
#include <cyg/athttpd/handler.h>
#include <cyg/athttpd/forms.h>

#include "iros_config.h"
#include "osal_api_core.h"
#include "plat_common.h"
#include "cs_module.h"
#include "cs_types.h"


#define CGI_MAX_BUFF_LEN  1024

#define CGI_FORM_VAR_INT32_LEN  16
#define CGI_FORM_VAR_INT16_LEN  8
#define CGI_FORM_VAR_INT8_LEN   4
#define CGI_FORM_VAR_VLAN_LEN   128
#define CGI_FORM_VAR_MAC_LEN    20 

#define CGI_OK                0 
#define CGI_ERROR_PARAM       1
#define CGI_SDL_FAILED        2
#define CGI_INNER_ERROR        3


#define CGI_HTTP_BUF_SIZE    4096


#define  CGI_ASSERT_RET(x,ret)    \
    do{ \
        if (!(x))  { \
            cs_printf("%s() _%d_: assert fail\n", __FUNCTION__, __LINE__); \
            return (ret); \
        } \
    }while(0)

#define  CGI_ASSERT(x)    \
    do{ \
        if (!(x))  { \
            cs_printf("%s() _%d_: assert fail\n", __FUNCTION__, __LINE__); \
            return ; \
        } \
    }while(0)


#define CGI_LOG(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_MAJ,IROS_MID_WEB,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define CGI_LOG_INFO(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_INF,IROS_MID_WEB,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define CGI_LOG_MIN(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_MIN,IROS_MID_WEB,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define CGI_LOG_MAJ(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_MAJ,IROS_MID_WEB,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define CGI_LOG_CRI(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_CRI,IROS_MID_WEB,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define CGI_DEBUG(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_DBG0,IROS_MID_WEB,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define CGI_HTTPD_HANDLER_REG(handler_name,handler_func) \
            CYG_HTTPD_HANDLER_TABLE_ENTRY(g_##handler_func##_hdl_id,handler_name,handler_func) 

/* Maybe Only for ahttp, in Linux, please empty instead */
#define CGI_HTTPD_FVAR_REG(var_key, var_len) \
            static cs_uint8 g_##var_key##_buf[var_len] ; \
            CYG_HTTPD_FVAR_TABLE_ENTRY(g_##var_key##_id, #var_key,g_##var_key##_buf, var_len) 


typedef  CYG_HTTPD_STATE cgi_handler_param_t ; 

typedef  cs_int32   cgi_status ;


cs_status cgi_get_int32_by_key(
        cgi_handler_param_t * p, 
        cs_int8 * key, 
        cs_uint32 * value);

cs_status cgi_get_int16_by_key(
        cgi_handler_param_t * p, 
        cs_int8 * key, 
        cs_uint16 * value); 

cs_status cgi_get_int8_by_key(
        cgi_handler_param_t * p,
        cs_int8 * key, 
        cs_uint8 * value);

cs_status cgi_get_string_by_key(
        cgi_handler_param_t * p, 
        cs_int8 * key, 
        cs_int8 * value,
        cs_uint32 max_len); 

void cgi_send_str(
        cgi_handler_param_t *p,
        cs_int8 * buff,
        cs_uint32 buff_len);

void cgi_send_int(
        cgi_handler_param_t *p,
        cs_uint32 code);

void cgi_send_html(
        cgi_handler_param_t *p,
        cs_int8 * buff,
        cs_uint32 buff_len);

void cgi_send_bin(
        cgi_handler_param_t *p,
        cs_uint8 * buff,
        cs_uint32 buff_len);

void cgi_org_init();

void cgi_init();

void cgi_clean_socket_data(cgi_handler_param_t *p);

cs_int32 cgi_get_current_data(
        cs_uint8 *buf, 
        cs_uint32 len, 
        cgi_handler_param_t * p);

cs_int32 cgi_read_socket_data(
        cs_uint8 *buf, 
        cs_uint32 len, 
        cgi_handler_param_t * p);

extern cs_uint8 CGI_MAX_UNI_PORT ;
#endif


