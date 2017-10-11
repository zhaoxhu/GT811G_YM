#ifndef _CS_HELLO_H_
#define _CS_HELLO_H_

#define DEFAULT_HELLO_INTERVAL 1 /*1 second*/
#define DEFAULT_HELLO_EXPIRE   3 /*3 times*/

typedef enum
{
    HELLO_INIT_STATE = 0,
    HELLO_ACTIVE     = 1,
    HELLO_DISCONNECT = 2,
    HELLO_UPGRADE    = 3, /*Used for l2ftp RRQ*/
}HELLO_STATE_ENUM;

typedef enum
{
    CS_ONU_DEV_TYPE = 0,
    CS_OLT_DEV_TYPE = 1,
    CS_SC_DEV_TYPE  = 2,
} DEV_TYPE_ENUM;

#define CS_LAYER3_TYPE_HELLO          1
#define CS_LAYER3_TYPE_FTP            2
#define CS_LAYER3_TYPE_MSG            3
#define CS_LAYER3_TYPE_ONU_HELLO      4

typedef struct {
    cs_uint32    hello_interval;
    cs_uint32    hello_expire;
    cs_uint8     hello_state;
    cs_uint8     payload_type;   /*0: SC payload  1: OLT payload 2: ONU payload*/
    cs_uint16    payload_len;
    cs_mac_t     src_mac;
    cs_mac_t     dst_mac;
    cs_uint8     reserved[20];   /*Reserved for future*/    
    cs_uint8     payload[1];
} __attribute__((packed))cs_hello_pdu_t;
#define HELLO_PDU_HDR_LEN   (sizeof(cs_hello_pdu_t)-1)


#endif

