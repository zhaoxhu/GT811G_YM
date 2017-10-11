#include "cgi_cmn.h"
#include "plat_common.h"
#include "cgi_upgrade.h"
#include "db_api.h"
#include "db_instance.h"
cgi_status cgi_cs_image_verify(cgi_handler_param_t * p)
{
    cs_uint32 verify = 0;
    CGI_DEBUG("cgi_zte_image_verify enter \n");
    
    verify = (cs_uint32)cgi_get_img_verify_flag();
    cgi_send_int(p, verify);

    CGI_DEBUG("verify is %d \n", verify);

    return CGI_OK;
}

cgi_status cgi_cs_get_wlb_image_ext(cgi_handler_param_t * p)
{
    CGI_DEBUG("cgi_get_wlb_image_ext enter \n");
    if(CGI_OK == cgi_upload_image_process(p, CGI_UPLOAD_WLB_TYPE))
    {
        cgi_send_int(p, CGI_OK);
        return CGI_OK;
    }
    else
    {
        cgi_send_int(p, CGI_INNER_ERROR);
        CGI_DEBUG("cgi_get_wlb_image_ext proess fail \n");
        return CGI_INNER_ERROR;
    }
}

cgi_status cgi_cs_config_save(cgi_handler_param_t * p)
{
    if(CS_E_OK == db_save_to_flash()){
        cgi_send_int(p, CGI_OK);
    }else{
        cgi_send_int(p, CGI_INNER_ERROR);
    }
    return CGI_OK;
}

cgi_status cgi_cs_config_download(cgi_handler_param_t * p)
{
    cs_uint8 filename[] = "/config.bin";
    if(CS_E_OK == db_save_to_file(filename)){
        cgi_send_int(p, CGI_OK);
    }else{
        cgi_send_int(p, CGI_INNER_ERROR);
    }

    return CGI_OK;
}

cgi_status cgi_cs_config_reset(cgi_handler_param_t * p)
{
    db_set_default();
    db_save_to_flash();
    iros_system_reset(RESET_TYPE_FORCE);

    return CGI_OK;
}
