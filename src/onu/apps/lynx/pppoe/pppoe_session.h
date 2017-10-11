#ifndef _APP_PPPOE_H
#define _APP_PPPOE_H
#include "pppoe.h"

void pppoe_session_pkt_recv(cs_pkt_t* hdr);cs_status pppoe_session_start();
cs_status pppoe_session_start();
cs_status pppoe_simulation_stop(
        cs_port_id_t port);

cs_status pppoe_simulation_result_get(
        cs_port_id_t port,
        pppoe_test_result_t* result);

cs_status pppoe_auth_mode_set(
    cs_uint8        mode,
    cs_int8        *name,
    cs_int8        *pwd);

void pppoe_cmd_result_get(
    cs_port_id_t port);
#endif
