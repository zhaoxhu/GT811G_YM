#include <stdio.h>
#include "cgi_cmn.h"
#include "cgi_auth.h"
cs_uint32 web_timer = 0;
cs_uint32 web_save = 0;
cs_boolean web_req = 1;
cs_uint32 web_timeout_int = 300;

cs_status cgi_get_int32_by_key(
        cgi_handler_param_t * p, 
        cs_int8 * key, 
        cs_uint32 * value)
{
    cs_uint8 * pRet = NULL;

    CGI_ASSERT_RET(p != NULL 
            && key != NULL 
            && value != NULL,CS_E_PARAM);

    pRet = cyg_httpd_find_form_variable(key );

    if(pRet != NULL){
        sscanf( pRet, "%d", value);
        CGI_DEBUG("value = %d \n",*value);
        return CS_E_OK;
    }
    return CS_E_ERROR;
}

cs_status cgi_get_int16_by_key(
        cgi_handler_param_t * p, 
        cs_int8 * key, 
        cs_uint16 * value)
{
    cs_uint8 * pRet = NULL;
    cs_uint32  val;
    CGI_ASSERT_RET(p != NULL 
            && key != NULL 
            && value != NULL,CS_E_PARAM);

    pRet = cyg_httpd_find_form_variable(key );

    if(pRet != NULL){
        sscanf(pRet, "%d", &val);
        CGI_DEBUG("get val = %d \n",val);
        *value = val;
        CGI_DEBUG("value = %d \n",*value);
        return CS_E_OK;
    }
    return CS_E_ERROR;
}

cs_status cgi_get_int8_by_key(
        cgi_handler_param_t * p,
        cs_int8 * key, 
        cs_uint8 * value)
{
    cs_uint8 * pRet = NULL;
    cs_uint32  val;

    CGI_ASSERT_RET(p != NULL 
            && key != NULL 
            && value != NULL,CS_E_PARAM);

    pRet = cyg_httpd_find_form_variable(key );

    if(pRet != NULL){
        sscanf(pRet, "%d", &val);
        CGI_DEBUG("get val = %d \n",val);
        *value = val;
        CGI_DEBUG("value = %d \n",*value);
        return CS_E_OK;
    }
    return CS_E_ERROR;
}

cs_status cgi_get_string_by_key(
        cgi_handler_param_t * p, 
        cs_int8 * key, 
        cs_int8 * value,
        cs_uint32 max_len) 
{
    cs_uint8 * pRet = NULL;

    CGI_ASSERT_RET(p != NULL 
            && key != NULL 
            && value != NULL,CS_E_PARAM);

    pRet = cyg_httpd_find_form_variable(key );

    if(pRet != NULL){
        cs_uint32 len = strlen(pRet) > max_len ? 
            max_len:strlen(pRet);
        strncpy(value,pRet,len);

        CGI_DEBUG("get string = %s \n",value);
        return CS_E_OK;
    }
    return CS_E_ERROR;
}


void cgi_send_str(
        cgi_handler_param_t *p,
        cs_int8 * buff,
        cs_uint32 buff_len)
{
    cs_uint32 len = buff_len > CYG_HTTPD_MAXOUTBUFFER ?
            CYG_HTTPD_MAXOUTBUFFER:buff_len;

    CGI_ASSERT(p != NULL 
            && buff != NULL);

    cyg_httpd_start_chunked("txt"); 
    memset(p->outbuffer, 0, CYG_HTTPD_MAXOUTBUFFER); 
    strncpy(p->outbuffer, buff,len);
    cyg_httpd_write_chunked(p->outbuffer, len); 
    cyg_httpd_end_chunked(); 
}

void cgi_send_int(
        cgi_handler_param_t *p,
        cs_uint32 code)
{
    CGI_ASSERT(p != NULL);
    cyg_httpd_start_chunked("txt"); 
    memset(p->outbuffer, 0, CYG_HTTPD_MAXOUTBUFFER); 
    sprintf(p->outbuffer,"%d", code); 
    cyg_httpd_write_chunked(p->outbuffer, strlen(p->outbuffer)); 
    cyg_httpd_end_chunked(); 
}


void cgi_send_html(
        cgi_handler_param_t *p,
        cs_int8 * buff,
        cs_uint32 buff_len)
{
    cs_uint32 len = buff_len > CYG_HTTPD_MAXOUTBUFFER ?
            CYG_HTTPD_MAXOUTBUFFER:buff_len;

    CGI_ASSERT(p != NULL 
            && buff != NULL);

    cyg_httpd_start_chunked("html"); 
    memset(p->outbuffer, 0, CYG_HTTPD_MAXOUTBUFFER); 
    strncpy(p->outbuffer, buff,len);
    cyg_httpd_write_chunked(p->outbuffer, len); 
    cyg_httpd_end_chunked(); 
}

void cgi_send_bin(
        cgi_handler_param_t *p,
        cs_uint8 * buff,
        cs_uint32 buff_len)
{
    cs_uint32 len = buff_len > CYG_HTTPD_MAXOUTBUFFER ?
            CYG_HTTPD_MAXOUTBUFFER:buff_len;

    CGI_ASSERT(p != NULL 
            && buff != NULL);

    cyg_httpd_start_chunked("bin"); 
    memset(p->outbuffer, 0, CYG_HTTPD_MAXOUTBUFFER); 
    memcpy(p->outbuffer, buff,len);
    cyg_httpd_write_chunked(p->outbuffer, len); 
    cyg_httpd_end_chunked(); 
}



void cgi_init()
{
    cgi_auth_init();
}

void cgi_clean_socket_data(cgi_handler_param_t *p)
{
    cs_int32 socket = p->sockets[p->client_index].descriptor;
    cs_uint8 buff[CGI_HTTP_BUF_SIZE];
    fd_set rfds;
    cs_uint32 len = 0;
    struct timeval tv = {2, 0};
    cs_int32 rc = 0;

    FD_ZERO(&rfds);
    FD_SET(socket, &rfds);

    do
    {
        rc = select(socket+1, &rfds, NULL, NULL, &tv);
        if(rc > 0)
        {
            len = recv(socket, buff, CGI_HTTP_BUF_SIZE, MSG_DONTWAIT);
        }
        else
        {
            break;
        }
    }while(len != 0 && len != -1);
    
}

cs_int32 cgi_get_current_data(cs_uint8 *buf, cs_uint32 len, cgi_handler_param_t * p)
{
    cs_int32 ret_len = 0;

    ret_len = p->content_len < p->inbuffer_len ? p->content_len : p->inbuffer_len;
    CGI_LOG_INFO("content_len=%d, inbuffer_len=%d\n", p->content_len, p->inbuffer_len);
    if(ret_len > len)
    {
        ret_len = len;
    }
    memcpy(buf, p->inbuffer, ret_len);

    return ret_len;
}

cs_int32 cgi_read_socket_data(cs_uint8 *buf, cs_uint32 len, cgi_handler_param_t * p)
{
    cs_int32 socket = p->sockets[p->client_index].descriptor;
    cs_int32 ret_len = 0;
    struct timeval tv = {6, 0};
    cs_int32 rc = 0;
    fd_set rfds;

    FD_ZERO(&rfds);
    FD_SET(socket, &rfds);

    rc = select(socket+1, &rfds, NULL, NULL, &tv);
    if(rc <= 0)
    {
        return -1;
    }

    ret_len =  read(socket, buf, len);
    CGI_LOG_INFO("read socket len %d\n", ret_len);

    if(ret_len <= 0)
    {
        return -1;
    }

    return ret_len;
    
}

