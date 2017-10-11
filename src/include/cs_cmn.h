#ifndef _CS_CMN_H_
#define _CS_CMN_H_

#include "cs_types.h"

#define CS_PKT_OFFSET   48

typedef enum {
    CS_PKT_BPDU,
    CS_PKT_8021X,
    CS_PKT_GMP,    /* IGMP and MLD */
    CS_PKT_ARP,
    CS_PKT_OAM,
    CS_PKT_MPCP,
    CS_PKT_DHCP,
    CS_PKT_IROS,
    CS_PKT_PPPOE_DIS, /* PPPOE Discovery */
    CS_PKT_PPPOE_SES, /* PPPOE Session */
    CS_PKT_IP,
    CS_PKT_NDP,
    CS_PKT_LOOPDETECT,
    CS_PKT_MYMAC,
    CS_PKT_BCMP,
    CS_PKT_PON_EOC, /* 0x8891 */
    CS_PKT_GWD_LOOPDETECT,
    CS_PKT_DHCPV6,
    CS_PKT_TYPE_NUM
} cs_pkt_type_t;

typedef struct _cs_pkt_t
{
    cs_uint8    msg_type;
    cs_uint8    version;
    cs_uint16   len;
    cs_uint16   port;
    cs_uint16   offset;
    cs_uint16   svlan;
    cs_uint16   cvlan;
    cs_uint16   stpid;
    cs_uint16   ctpid;
    cs_uint8     frame_type; 
    cs_uint8     pkt_type;
    cs_uint16   eth_type;
    cs_uint16   tag_num;
    cs_uint16   u2Reserved;
    cs_uint8    data[1];
} cs_pkt_t;

typedef struct {
    /* invocation identifier */
    cs_uint32           invoke_id;
    /* the opaque application data */
    void *                  apps_data;
    void*                   apps_handler;
} cs_callback_context_t;


#endif

