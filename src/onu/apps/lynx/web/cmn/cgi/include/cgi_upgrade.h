#ifndef __CGI_UPGRADE_H__
#define __CGI_UPGRADE_H__
#define CGI_UPLOAD_JFFS2_TYPE   1
#define CGI_UPLOAD_MIF_TYPE      2
#define CGI_UPLOAD_WLB_TYPE     3


typedef struct{
    cs_uint32 part_index;
    cs_uint32 img_len;
    cs_uint32 curr_len;
}cgi_upload_cb_t;

typedef enum{
    CGI_UPLOAD_SUCCESS = 0,
    CGI_UPLOAD_FAIL 
}cgi_upload_result_e;

#ifdef HAVE_FLASH_FS
cgi_status cgi_get_jffs2_image(cgi_handler_param_t * p);
#endif
cgi_status cgi_get_mif_image(cgi_handler_param_t * p);
cgi_status cgi_get_wlb_image(cgi_handler_param_t * p);
cgi_status cgi_upload_image_process(cgi_handler_param_t * p, cs_uint8 type);
cs_uint8 cgi_get_img_verify_flag();

#endif

