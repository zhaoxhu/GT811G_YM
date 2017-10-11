#include "cgi_system.h"
#include "cgi_upgrade.h"

static char upload_html[] =
    "<HTML>\
        <FORM action=/cgi-bin/get_mif_image.cgi method=post encType=multipart/form-data >\
            <BODY>Upgrade MIF firmware(.mif format): <INPUT type=file name=userfile><INPUT type=submit value=Upload></BODY></FORM>"

        "<FORM action=/cgi-bin/get_wlb_image.cgi method=post  encType=multipart/form-data >\
            <BODY>Upgrade app firmware(.wlb format): <INPUT type=file name=userfile><INPUT type=submit value=Upload></BODY></FORM>"

        "<FORM action=/cgi-bin/get_jffs2_image.cgi method=post  encType=multipart/form-data >\
            <BODY>Upgrade JFFS2 image(.wfs format): <INPUT type=file name=userfile><INPUT type=submit value=Upload></BODY></FORM>\
    </HTML>"
    ;

CYG_HTTPD_IRES_TABLE_ENTRY(cyg_ires_upload, "/upload.html", upload_html, sizeof(upload_html));

CGI_HTTPD_HANDLER_REG("/cgi-bin/reset_onu.cgi", cgi_sys_reset);
CGI_HTTPD_HANDLER_REG("/cgi-bin/get_switch_type.cgi", cgi_switch_type_get);
CGI_HTTPD_HANDLER_REG("/cgi-bin/get_wlb_image.cgi", cgi_get_wlb_image);
CGI_HTTPD_HANDLER_REG("/cgi-bin/get_mif_image.cgi", cgi_get_mif_image);
#ifdef HAVE_FLASH_FS
CGI_HTTPD_HANDLER_REG("/cgi-bin/get_jffs2_image.cgi", cgi_get_jffs2_image);
#endif
