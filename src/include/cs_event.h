#ifndef _CS_EVENT_H_
#define _CS_EVENT_H_

typedef struct {
    cs_uint32   msg_type;
    cs_uint32   seq_num;
    cs_mac_t    device;
    cs_int32    event;
    cs_uint32   len;
    cs_uint8    data[1];
} __attribute__((packed)) cs_sdk_event_msg_t;

typedef struct {
    cs_uint32   msg_type;
    cs_uint32   seq_num;
} __attribute__((packed)) cs_sdk_event_ack_t;

typedef struct {
    cs_uint32   msg_type;
    cs_uint32   seq_num;
} __attribute__((packed)) cs_sdk_event_header_t;


#define CS_EVENT_SEQ_RESET 0xFFFFFFFF

#define CS_ESS_SDK_EVENT        0x04000003
#define CS_ESS_SDK_EVENT_ACK    0x04000004


#endif

