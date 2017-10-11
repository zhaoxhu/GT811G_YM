#ifndef _APP_OAM_GWD_INTF_H_
#define _APP_OAM_GWD_INTF_H_

#ifdef HAVE_GWD_OAM
void handle_gwd_oams(
        cs_port_id_t port,
        cs_uint8 *frame,
        cs_uint32 length);

void gwd_onu_init(void);

#endif

#endif	/*end of the file*/
