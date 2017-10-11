#ifndef __CS_MSG_H__
#define __CS_MSG_H__

#define CS_OAM_OPCODE_HELLO          0
#define CS_OAM_OPCODE_VAR_REQ      0x1
#define CS_OAM_OPCODE_VAR_RESP     0x2
#define CS_OAM_OPCODE_SET_REQ       0x3
#define CS_OAM_OPCODE_SET_RESP      0x4
#define CS_OAM_OPCODE_L2FTP            0x5
#define CS_OAM_OPCODE_EVENT           0x6

typedef struct {
    cs_mac_t           dst_mac;
    cs_mac_t           src_mac;
    cs_uint16          eth_type;
    cs_uint8           sub_type;
    cs_uint16          flags;
    cs_uint8           code;
    cs_uint8            oui[3];
    cs_uint8           opcode;
    cs_uint8           data[0];
} __attribute__((packed)) cs_oam_pdu_hdr_t;

#ifdef CS_GLOBAL_PARAM
cs_mac_t CS_MAC_ADDR_SLOW = {{0x01,0x80,0xc2,0x00,0x00,0x02}};
cs_mac_t CS_MAC_ADDR_ALL_SC = {{0x01, 0x13, 0x25, 0x00, 0x00, 0x02}};
cs_mac_t CS_MAC_ADDR_ALL_ONU = {{0x01, 0x13, 0x25, 0x00, 0x00, 0x03}};
cs_uint8 CS_OUI[3] = {0x00,0x13,0x25};
#else
extern cs_mac_t CS_MAC_ADDR_SLOW;
extern cs_mac_t CS_MAC_ADDR_ALL_SC;
extern cs_mac_t CS_MAC_ADDR_ALL_ONU;
extern cs_uint8 CS_OUI[3];
#endif
#endif

