#ifndef _ONU_HELLO_H
#define _ONU_HELLO_H

#include "util_list.h"

#define ONU_L2FTP_INTERVAL 3

typedef struct {
        cs_uint8    state;
        cs_uint8    llid;
        cs_uint8    uni_state;
        cs_port_id_t llid_port_id;
        cs_uint8    ext_phy_device;
        cs_uint8    max_mc_fdb_entry;
} onu_record_t;//need to remove this define

typedef struct {
        cs_mac_t macaddr;
} olt_record_t;

typedef struct {
        cs_mac_t macaddr;
} sc_record_t;

typedef struct {
        cs_uint32 timer;
        cs_uint16 state;
        cs_uint16 expire;
}onu_hello_record_t;

typedef struct {
        cs_uint32 timer;
} l2ftp_record_t;

typedef struct {
        cs_mac_t    macaddr;
        cs_mac_t    device;
        onu_hello_record_t  hello_record;
        cs_uint32   state;
        olt_record_t    olt;
        sc_record_t sc;

        cs_uint32   hello_interval;
        cs_uint32   dead_interval;
        cs_uint32   version;
        cs_boolean  isOobChip;
        cs_list handler_list;
} onu_node_t;

#define HELLO_LINK_CONNECTED    1
#define HELLO_LINK_DISCONNECTED 0

typedef struct {
    cs_node node;
    void (*handler)();
    cs_int32 link_status;
} hello_link_hdl_t;

extern onu_node_t onu_node;
extern cs_port_id_t active_hello_port;
extern cs_port_id_t get_dest_port();
extern void cs_hello_link_change_reg
    (
    void (*handler)(),
    cs_int32 link_status
    );

extern void app_hello_init();
extern cs_int32 hello_is_active();
extern void onu_hello_proc(cs_uint8 *frame,int len,cs_port_id_t port);
#endif

