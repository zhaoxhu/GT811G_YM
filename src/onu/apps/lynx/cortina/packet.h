#ifndef _PACKET_H
#define _PACKET_H

#include "cs_cmn.h"
#include "osal_api_packet.h"

#define EPON_ETHERTYPE_MPCP	    0x8808
#define EPON_ETHERTYPE_PAE      0x888e
#define EPON_ETHERTYPE_SLOW     0x8809
#define EPON_ETHERTYPE_DOT1Q    0x8100
#define EPON_ETHERTYPE_IP       0x0800
#define EPON_ETHERTYPE_IPV6       0x86dd
#define EPON_ETHERTYPE_ARP      0x0806
#define EPON_ETHERTYPE_TPID_SVLAN      0x88a8
#define EPON_ETHERTYPE_TPID_SVLAN_OLD  0x9100
#define EPON_ETHERTYPE_STP		0x0026	/*STP_PROTOCOL*/
#define EPON_ETHERTYPE_RSTP	0x0027	
#define EPON_ETHERTYPE_TCN	0x0007	
#define EPON_ETHERTYPE_PPPOE_DISC	0x8863
#define EPON_ETHERTYPE_PPPOE_SESSION	0x8864
#define EPON_ETHERTYPE_DEFAULT	0x0001	
#define EPON_ETHERTYPE_BCMP	0x0877
#define EPON_ETHERTYPE_PON_EOC	0x8891

#define IP_PROTOCOL_IGMP             2
#define IP_PROTOCOL_MLD               58

#define ONU_PKT_FILTER_NUM  16

typedef cs_status (*cs_pkt_parser_t)(cs_pkt_t *);
typedef cs_uint8 (*cs_pkt_handler_t)(cs_pkt_t *);

typedef struct {
    cs_pkt_parser_t  parser;
    cs_pkt_handler_t handler;
    cs_uint32    counter;
    cs_uint8      trace;    
} cs_pkt_profile_t;

typedef cs_status (*cs_pkt_filter_entry_t)(cs_pkt_t *);

typedef struct {
    cs_port_id_t              port;
    cs_pkt_filter_entry_t entry;
} cs_pkt_filter_t;


extern cs_status app_pkt_add_filter_rule(cs_port_id_t port, cs_pkt_filter_entry_t entry);
extern cs_status app_pkt_reg_handler(cs_uint8 pkt_type, cs_pkt_handler_t hdl);
extern cs_status app_pkt_reg_parser(cs_uint8 pkt_type, cs_pkt_parser_t parser);
extern void app_pkt_toggle_trace(cs_pkt_type_t type);

extern void app_pkt_init();
extern cs_uint8 app_pkt_msg_proc(cs_pkt_t *pPkt);
extern cs_uint32 app_pkt_get_counter(cs_pkt_type_t pkt_type);
extern void app_pkt_clr_counter(cs_pkt_type_t pkt_type);
extern cs_status app_pkt_tx(cs_port_id_t port_id, cs_pkt_type_t type, cs_uint8 *frame, cs_uint16 len);

#endif

