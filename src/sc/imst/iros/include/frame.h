/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#ifndef _FRAME_H_
#define _FRAME_H_


#define EPON_ETHER_MTU                  1536
#define EPON_ETHER_MIN_MTU              60
#define EPON_ETHERTYPE_IROS             0xFFFF

#define EPON_MGMTCH_VLAN             0xFFF
#define EPON_OUI_LEN                    3
#define EPON_MACADDR_LEN                6
#define EPON_LAYER2_HEADER_LEN          14
#define EPON_LAYER3_HEADER_LEN          14
#define EPON_FRAME_HEADER_LEN           (EPON_LAYER2_HEADER_LEN + \
                                         EPON_LAYER3_HEADER_LEN)
#define EPON_FRAME_PAYLOAD_LEN          (EPON_ETHER_MTU - \
                                         EPON_LAYER3_HEADER_LEN)


#define KT_LB_DEFAULT_RULE_ID           1


/* frame carrying iros hello protocol */
#define EPON_LAYER3_TYPE_HELLO          1
/* frame carrying iros file transfer protocol */
#define EPON_LAYER3_TYPE_FTP            2
/* frame carrying an iros message */
#define EPON_LAYER3_TYPE_MSG            3
/* frame carrying iros onu hello protocol */
#define EPON_LAYER3_TYPE_ONU_HELLO      4

/* ethernet frame header */
typedef struct {
        epon_macaddr_t          dst;
        epon_macaddr_t          src;
        cs_uint16           ethertype;
} __attribute__((packed)) epon_ether_header_t;

/* layer three header */
typedef struct {
        epon_macaddr_t          dst;
        epon_macaddr_t          src;
        cs_uint16           layer3type;
} __attribute__((packed)) epon_layer3_header_t;

/* ethernet frame encapsulation */
typedef struct {
        /* ether header */
        epon_ether_header_t     ether_header;
        /* layer three header */
        epon_layer3_header_t    layer3_header;
        /* the payload */
        cs_uint8           payload[EPON_FRAME_PAYLOAD_LEN];
} __attribute__((packed)) epon_ether_frame_t;

/* tagged ethernet frame encapsulation */
typedef struct {
        /* ether header */
        epon_macaddr_t          l2dst;
        epon_macaddr_t          l2src;
        cs_uint16           tpid;
        cs_uint16           tag_id;
        cs_uint16           ethertype;
       
        /* layer three header */
        epon_macaddr_t          l3dst;
        epon_macaddr_t          l3src;
        cs_uint16           layer3type;
        /* the payload */
        cs_uint8           payload[EPON_FRAME_PAYLOAD_LEN - sizeof(cs_uint16) * 2];
} __attribute__((packed)) epon_tagged_ether_frame_t;


extern cs_uint8     epon_macaddr_all_olt[];
extern cs_uint8     epon_macaddr_all_sc[];
extern cs_uint8     epon_macaddr_all_onu[];
extern cs_uint8     epon_oui_immenstar[];
extern cs_uint8     epon_macaddr_slow[];

typedef struct {
    cs_uint32    timer;
    cs_uint32   lb_port;
}loop_detect_block_timer_t;


epon_ether_frame_t *epon_frame_alloc(epon_macaddr_pt, epon_macaddr_pt,
                                     cs_uint16);

void epon_frame_proc(epon_ether_frame_t* frame, cs_uint32 len);

#endif
