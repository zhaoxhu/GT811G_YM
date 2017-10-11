/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#ifndef _HELLO_H_
#define _HELLO_H_
#include "cs_types.h"
#include "frame.h"

#define EPON_HELLO_INTERVAL             2000
#define EPON_HELLO_EXPIRE               3

#define EPON_HELLO_STATE_OLT_STABLE     0x0001
#define EPON_HELLO_STATE_OLT_IMAGE      0x0002
#define EPON_HELLO_STATE_OLT_CONFIG     0x0004

typedef cs_uint16 epon_hello_state_t;

typedef struct {
        /* olt device id */
        epon_olt_device_id_t            device;
        /* hello state */
        epon_hello_state_t              state;
} __attribute__((packed)) epon_hello_olt_info_t;

typedef struct {
        /* SC-OLT hello interval */
        cs_uint32           hello_interval;
        /* SC-OLT dead interval */
        cs_uint32           dead_interval;
        /* number of olts in the hello */
        cs_uint16           olt_number;
        /* list of olt info */
        epon_hello_olt_info_t   olt_info[1];
} __attribute__((packed)) epon_sc_hello_pdu_t;

typedef struct {
        /* olt info */
        epon_hello_olt_info_t   olt_info;
        /* sc mac address */
        epon_macaddr_t          sc_macaddr;
        /* olt version */
        epon_olt_version_t      version;
} __attribute__((packed)) epon_olt_hello_pdu_t;

/* process received hello pdu frame */
void epon_hello_proc(epon_ether_frame_t *frame);

/* ONU hello defines */
#define EPON_HELLO_STATE_ONU_STABLE     0x0001
#define EPON_HELLO_STATE_ONU_IMAGE      0x0002
#define EPON_HELLO_STATE_ONU_CONFIG     0x0004

/* sc hellol status */
typedef enum {
        EPON_HELLO_PEER_STATE_DISC,
        EPON_HELLO_PEER_STATE_RECV,
        EPON_HELLO_PEER_STATE_ONUCONN,
        EPON_HELLO_PEER_STATE_DOWNLOADING
} epon_sc_hello_status_t;

/* process received hello pdu frame */
void epon_onu_hello_proc(epon_ether_frame_t *frame);

extern cs_port_id_t active_hello_port;
#endif
