/****************************************************************************
            Software License for Customer Use of Cortina Software
                          Grant Terms and Conditions

IMPORTANT NOTICE - READ CAREFULLY: This Software License for Customer Use
of Cortina Software ("LICENSE") is the agreement which governs use of
software of Cortina Systems, Inc. and its subsidiaries ("CORTINA"),
including computer software (source code and object code) and associated
printed materials ("SOFTWARE").  The SOFTWARE is protected by copyright laws
and international copyright treaties, as well as other intellectual property
laws and treaties.  The SOFTWARE is not sold, and instead is only licensed
for use, strictly in accordance with this document.  Any hardware sold by
CORTINA is protected by various patents, and is sold but this LICENSE does
not cover that sale, since it may not necessarily be sold as a package with
the SOFTWARE.  This LICENSE sets forth the terms and conditions of the
SOFTWARE LICENSE only.  By downloading, installing, copying, or otherwise
using the SOFTWARE, you agree to be bound by the terms of this LICENSE.
If you do not agree to the terms of this LICENSE, then do not download the
SOFTWARE.

DEFINITIONS:  "DEVICE" means the Cortina Systems?LynxD SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems?SDK software.

GRANT OF LICENSE:  Subject to the restrictions below, CORTINA hereby grants
CUSTOMER a non-exclusive, non-assignable, non-transferable, royalty-free,
perpetual copyright license to (1) install and use the SOFTWARE for
reference only with the DEVICE; and (2) copy the SOFTWARE for your internal
use only for use with the DEVICE.

RESTRICTIONS:  The SOFTWARE must be used solely in conjunction with the
DEVICE and solely with Your own products that incorporate the DEVICE.  You
may not distribute the SOFTWARE to any third party.  You may not modify
the SOFTWARE or make derivatives of the SOFTWARE without assigning any and
all rights in such modifications and derivatives to CORTINA.  You shall not
through incorporation, modification or distribution of the SOFTWARE cause
it to become subject to any open source licenses.  You may not
reverse-assemble, reverse-compile, or otherwise reverse-engineer any
SOFTWARE provided in binary or machine readable form.  You may not
distribute the SOFTWARE to your customers without written permission
from CORTINA.

OWNERSHIP OF SOFTWARE AND COPYRIGHTS. All title and copyrights in and the
SOFTWARE and any accompanying printed materials, and copies of the SOFTWARE,
are owned by CORTINA. The SOFTWARE protected by the copyright laws of the
United States and other countries, and international treaty provisions.
You may not remove any copyright notices from the SOFTWARE.  Except as
otherwise expressly provided, CORTINA grants no express or implied right
under CORTINA patents, copyrights, trademarks, or other intellectual
property rights.

DISCLAIMER OF WARRANTIES. THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY
EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING ANY IMPLIED WARRANTIES
OF MERCHANTABILITY, NONINFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE,
TITLE, AND NON-INFRINGEMENT.  CORTINA does not warrant or assume
responsibility for the accuracy or completeness of any information, text,
graphics, links or other items contained within the SOFTWARE.  Without
limiting the foregoing, you are solely responsible for determining and
verifying that the SOFTWARE that you obtain and install is the appropriate
version for your purpose.

LIMITATION OF LIABILITY. IN NO EVENT SHALL CORTINA OR ITS SUPPLIERS BE
LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, LOST
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING OUT
OF THE USE OF OR INABILITY TO USE OF OR INABILITY TO USE THE SOFTWARE, EVEN
IF CORTINA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
TERMINATION OF THIS LICENSE. This LICENSE will automatically terminate if
You fail to comply with any of the terms and conditions hereof. Upon
termination, You will immediately cease use of the SOFTWARE and destroy all
copies of the SOFTWARE or return all copies of the SOFTWARE in your control
to CORTINA.  IF you commence or participate in any legal proceeding against
CORTINA, then CORTINA may, in its sole discretion, suspend or terminate all
license grants and any other rights provided under this LICENSE during the
pendency of such legal proceedings.
APPLICABLE LAWS. Claims arising under this LICENSE shall be governed by the
laws of the State of California, excluding its principles of conflict of
laws.  The United Nations Convention on Contracts for the International Sale
of Goods is specifically disclaimed.  You shall not export the SOFTWARE
without first obtaining any required export license or other approval from
the applicable governmental entity, if required.  This is the entire
agreement and understanding between You and CORTINA relating to this subject
matter.
GOVERNMENT RESTRICTED RIGHTS. The SOFTWARE is provided with "RESTRICTED
RIGHTS." Use, duplication, or disclosure by the Government is subject to
restrictions as set forth in FAR52.227-14 and DFAR252.227-7013 et seq. or
its successor. Use of the SOFTWARE by the Government constitutes
acknowledgment of CORTINA's proprietary rights therein. Contractor or
Manufacturer is CORTINA.

Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/
#ifndef _APP_IP_TYPES_H_
#define _APP_IP_TYPES_H_
#include "cs_types.h"
#include "util_list.h"
#include "log.h"
#define DHCP_OPTION_PPPOE_PLUS_SUPPORT 1
#define ETH_HTTP_PORT  0x50
#define ETH_ARP_PKT_LEN 60
#define IPV4_ADDR_LEN 4
#define ETHER_ADDR_LEN		6


#define NUM_PINGS   5
#define MAX_PACKET  128
#define MIN_PACKET  64

#define UNIQUEID 0x1234
#define APP_IPINTF_MAX_ARP_ENTRY_LEN 1000
#define IPINTF_PING_INTERVAL_MIN 1/* ms */




typedef struct {
        epon_macaddr_t          dst;
        epon_macaddr_t          src;
} __attribute__((packed)) epon_macaddr_header_t;

typedef enum {
    IPINTF_DROP,
    IPINTF_PASS    
}ipintf_filter_retcode;

typedef enum {
    IPOE_IN_PROGRESS          = 0,  /* in progress               */
    IPOE_TEST_END             = 1,  /* test end                  */
    IPOE_TEST_MAX
}ipoe_emulation_state_t;

typedef enum {
    IPOE_PASS                 = 0,  /* PASS               */
    IPOE_FAIL                 = 1,  /* FAIL               */
}ipoe_conclusion_t;

typedef enum {
    IPOE_DISCOVERY_TIMEOUT    = 0, 
    IPOE_REQUEST_TIMEOUT      = 1, 
    IPOE_NACK                 = 2, 
    IPOE_GET_IP_FAIL          = 3, 
    IPOE_TEST_TIMEOUT         = 4,
    IPOE_OTHER                = 5,
    IPOE_VLAN_UNCFG           = 6,
    IPOE_PVC_NOT_EXIST        = 7,
}ipoe_fail_reason_t;

typedef struct {
    cs_uint8   state;         
    cs_uint8   conclusion;      
    cs_uint8   reason; 
    cs_uint32  ip;
    cs_uint32  gw;
    cs_uint32  mask;
    cs_uint16  ping_times;
    cs_uint16  ping_ack_timers;
    cs_uint16  min_delay;
    cs_uint16  max_delay;
    cs_uint16  avg_delay;
    cs_uint16 port;//only ONE port supported
    cs_uint16 vlan;
    cs_uint32 ipaddr_backup;
}ipoe_state_t;
extern ipoe_state_t g_ipoe_state;
#define IPOE_INPROGESS()  (g_ipoe_state.state == IPOE_IN_PROGRESS)
#define APP_IPINTF_LOG_PKT_BCMP 0x1
#define APP_IPINTF_LOG_PKT_PONEOC 0x2
#define APP_IPINTF_LOG_CFG 0x4
#define APP_IPINTF_LOG_INFO 0x8
#define APP_IPINTF_LOG_ERR 0x10
#define APP_IPINTF_LOG_DBG 0x20


#define APP_IPINTF_LOG(level, args...)  \
    do{\
        if(ipintf_info.debug & level) \
            cs_printf(args);\
    }while(0)

#ifdef DHCP_OPTION_PPPOE_PLUS_SUPPORT
typedef struct _arp_hdr_t_ {
    cs_uint16 ar_hrd;       /* format of hardware address */
    cs_uint16 ar_pro;       /* format of protocol address */
    cs_uint8 ar_hln;        /* length of hardware address */
    cs_uint8 ar_pln;        /* length of protocol address */
    cs_uint16 ar_op;        /* one of: */
    cs_uint8    arp_sha[6]; /* sender hardware address */
    cs_uint32   arp_spa;    /* sender protocol address */
    cs_uint8    arp_tha[6]; /* target hardware address */
    cs_uint32   arp_tpa;    /* target protocol address */
}__attribute__ ((aligned(1), packed))arp_hdr_t;

typedef struct _ip_hdr_t_ {
#ifndef CS_BIG_ENDIAN
//#error "CS_BIG_ENDIAN"
    cs_uint8    ihl:4,
                   version:4;
#else
    cs_uint8    version:4,
                   ihl:4;
#endif
    cs_uint8    tos;
    cs_uint16   tot_len;
    cs_uint16   id;
    cs_uint16   frag_off;
    cs_uint8    ttl;
    cs_uint8    protocol;
    cs_uint16   check;
    cs_uint32   saddr;
    cs_uint32   daddr;
    /*The options start here. */
}__attribute__ ((aligned(1), packed))ip_hdr_t;

typedef struct _udp_hdr_t_{
    cs_uint16 sport;          /* Source port */
    cs_uint16 dport;          /* Destination port */
    cs_uint16 len;             /* Datagram length */
    cs_uint16 crc;             /* Checksum */
}udp_hdr_t;
#define IP_UDP_PROTOCOL_ID (17)
#define IP_TCP_PROTOCOL_ID (6)

#define IS_IP_PKT(type) ((type)==0x0800) 

#define DHCP_UDP_SRC_PORT (0x43)
#define DHCP_UDP_DST_PORT (0x44)
#define DHCPV6_UDP_SRC_PORT (0x0222) //host mode
#define DHCPV6_UDP_DST_PORT (0x0223) //host mode
//typedef struct _arp_hdr_t_ arp_hdr_t;
//typedef struct _ip_hdr_t_ ip_hdr_t;
//typedef struct _udp_hdr_t_ udp_hdr_t;



//DHCP options start

#define IPV4_DHCP 0x81
#define IPV6_DHCP 0x82
#define IPVX_DHCP_MASK 0x80

//IPV4 related
#define DHCP_CHADDR_LEN 16U
#define DHCP_SNAME_LEN 64U
#define DHCP_FILE_LEN 128U

typedef struct  {
  cs_uint8 op;
  cs_uint8 htype;
  cs_uint8 hlen;
  cs_uint8 hops;
  cs_uint32 xid;
  cs_uint16 secs;
  cs_uint16 flags;
  cs_uint32 ciaddr;
  cs_uint32 yiaddr;
  cs_uint32 siaddr;
  cs_uint32 giaddr;
  cs_uint8 chaddr[DHCP_CHADDR_LEN];
  cs_uint8 sname[DHCP_SNAME_LEN];
  cs_uint8 file[DHCP_FILE_LEN];
  cs_uint32 cookie;
  cs_uint8 options[0];
}__attribute__ ((aligned(1), packed)) dhcpv4_hdr_t;

typedef struct {
    cs_uint8 code;
    cs_uint8 len;
}__attribute__ ((aligned(1), packed)) dhcpv4_option_hdr_t;


struct dhcpv4_option_entry_s_;
typedef cs_int32 (*dhcpv4_proc) (cs_pkt_t *pkt, struct dhcpv4_option_entry_s_ *entry);
typedef struct dhcpv4_option_entry_s_
{
    //The order is crucial IMPORTANT.
    cs_uint8 code;
    cs_uint8 len;
    #define DHCP_OPTION_PAYLOAD_MAX 128
    //payload without OPTION CODE and LEN
    cs_uint8 payload[DHCP_OPTION_PAYLOAD_MAX];
    cs_uint8 enable;
    dhcpv4_proc init;
    dhcpv4_proc proc;
}dhcpv4_option_entry_t;

//IPV6 related

//DHCP V6
typedef struct {
#if defined(__LITTLE_ENDIAN__)
    cs_uint8 pri:4,
                    ver:4;
#else
    cs_uint8 ver:4,
                    pri:4;
#endif
    cs_uint8 flow[3];
    cs_uint16 len;
    cs_uint8 next;
    cs_uint8 hop;
    cs_uint8 saddr[16];
    cs_uint8 daddr[16];
} __attribute__ ((aligned(1), packed))  ipv6_hdr_t;


typedef struct {
    cs_uint8 type;
    cs_uint8 id[3];
}__attribute__ ((aligned(1), packed)) dhcpv6_hdr_t;

typedef struct dhcpv6_option_hdr_s{
    cs_uint16 code;
    cs_uint16 len;
}__attribute__ ((aligned(1), packed)) dhcpv6_option_hdr_t;




struct dhcpv6_option_entry_s_;
typedef cs_int32 (*dhcpv6_proc) (cs_pkt_t *pkt, struct dhcpv6_option_entry_s_ *entry);

typedef struct dhcpv6_option_entry_s_
{
    //The order is crucial IMPORTANT.
    cs_uint16 code;
    cs_uint16 len;
    #define DHCP_OPTION_PAYLOAD_MAX 128
    cs_uint8 payload[DHCP_OPTION_PAYLOAD_MAX];
    cs_uint8 enable;
    dhcpv6_proc init;
    dhcpv6_proc proc;
}dhcpv6_option_entry_t;

typedef struct 
{
    //The order is crucial IMPORTANT.
    cs_uint8 enable;
}pppoe_plus_entry_t;

#define OPT_HOP_BY_HOP 0
#define OPT_DST_OPT_HDR 60
#define OPT_ROUTING_HDR 43
#define OPT_FRAGMENT_HDR 44
#define OPT_AUTHENTICATION_HDR 51
#define OPT_ESP_HDR 50

#define IP_UDP 0x11

/*
* RFC2132
* 0       ~          15 |
* 60      | len        |
* enterprise number |
* sub_type|sub_len |
* sub_data            |
*/
typedef struct {
    cs_uint16 enterp_num;
    cs_uint8 data[0]; //dhcpv4_op125_sub_data_t
}__attribute__ ((aligned(1), packed)) dhcpv4_op60_data_t;
typedef struct {
    cs_uint8 field_type;
    cs_uint8 field_len;
    cs_uint8 field_data[0];
}__attribute__ ((aligned(1), packed)) dhcpv4_op60_sub_data_t;
#define YOTC_OP60_ENTERP_NUM 0


/*
* RFC3925
* 0       ~          15 |
* 125      | len        |
* enterprise number |
* op_len  | op_data |
*/
typedef struct {
    cs_uint32 enterp_num;
    cs_uint8 len;
    cs_uint8 data[0]; //dhcpv4_op125_sub_data_t
}__attribute__ ((aligned(1), packed)) dhcpv4_op125_data_t;
typedef struct {
    cs_uint8 sub_code;
    cs_uint8 sub_len;
    cs_uint8 data[0]; 
}__attribute__ ((aligned(1), packed)) dhcpv4_op125_sub_data_t;
#define YOTC_OP125_ENTERP_NUM 0

/*
* RFC3046
* 0       ~          15 |
* 82      | len        |
* sub1   | sub1_len|
* sub1_data          |  
* sub2   | sub2_len|
* sub2_data         |  
*/
typedef struct {
    cs_uint8 sub_op;
    cs_uint8 sub_len;
}__attribute__ ((aligned(1), packed)) dhcpv4_op82_data_t;



typedef struct _bcmp_l2_hdr_s {
//major.minor XXXX.YYYY, default 1.0
#define BCMP_VERSION 0x10
    cs_uint8 ver;
//Type for BCMP client registering
#define BCMP_REG 0x01 
//Type for BCMP keepalive
#define BCMP_TICK 0x02 
//Type for BCMP server response
#define BCMP_RES 0x03 
//Type for parameter changed(from BCMP server)
#define BCMP_PARACHG 0x04 
//Type for BCMP server response
#define BCMP_EXTENDED 0x05 //~0x0F 
#ifndef CS_BIG_ENDIAN
    cs_uint8    rev:4,
                   type:4;
#else
    cs_uint8    type:4,
                   rev:4;
#endif
    cs_uint16 seq;
    cs_uint16 crc;
    cs_uint16 len;
    cs_uint16 cluster;
    cs_uint8 rev1[6];
}__attribute__ ((aligned(1), packed)) bcmp_l2_hdr_t;
typedef struct _bcmp_ip_hdr_s {
//major.minor XXXX.YYYY, default 1.0
#define BCMP_VERION 0x10
    cs_uint8 ver;
//Type for BCMP client registering
#define IP_BCMP_REG 0x01 
//Type for BCMP keepalive
#define IP_BCMP_TICK 0x02 
//Type for BCMP server response
#define IP_BCMP_RES 0x03 
//Type for parameter changed(from BCMP server)
#define IP_BCMP_PARACHG 0x04 
//Type for BCMP server response
#define IP_BCMP_EXTENDED 0x05 //~0x0F 
#ifndef CS_BIG_ENDIAN
    cs_uint8    rev:4,
                   type:4;
#else
    cs_uint8    type:4,
                   rev:4;
#endif
    cs_uint16 seq;
    cs_uint16 crc;
    cs_uint16 len;
    cs_uint8 bcmp_mac[6];
    cs_uint8 rev1[6];
}__attribute__ ((aligned(1), packed)) bcmp_ip_hdr_t;

typedef struct {
    cs_uint8 len;
    cs_uint8 type;
}__attribute__ ((aligned(1), packed)) bcmp_cluster_hdr_t;
typedef struct {
    bcmp_cluster_hdr_t hdr;
    cs_uint8 mac[6];//ONU mac address
}__attribute__ ((aligned(1), packed)) bcmp_cluster_cbat_data_t;

typedef struct {
    bcmp_cluster_hdr_t hdr;
    cs_uint8 cbat_mac[6];//CBAT mac address
    cs_uint32 port_id;
    cs_uint8 onu_mac[6];//CBAT mac address
    cs_uint32 onu_id;
    cs_uint32 olt_id;
    cs_uint32 slot_id;
    cs_uint16 padding;
}__attribute__ ((aligned(1), packed)) bcmp_ip_cluster_onu_data_t;


typedef struct
{
    cs_uint8 used;
    cs_uint8 enable;
    cs_uint32 port_id;//ani_port
    cs_uint32 onu_id;
    cs_uint32 olt_id;
    cs_uint32 slot_id;
    //TODO
    cs_uint8 sub_slot_id; // not used
    cs_uint8 ani_rack;
    cs_uint8 ani_frame;
    cs_uint8 id[50];
}bcmp_cb_t;

#define BCMP_DFT_VID 4041
#define BCMP_DFT_PRI 5

#define BCMP_TYPE 0x0877
#define ETHER_BCMP_TYPE 0x0877
#define ETHER_PON_EOC_TYPE 0x8891

typedef enum {
    BCMP_DEV_TYPE_CBAT = 2,
    BCMP_DEV_TYPE_ONU = 3,
    BCMP_DEV_TYPE_OLT = 4
}BCMP_DEV_TYPE_T;
typedef struct
{
    dhcpv4_option_entry_t op60;
    dhcpv4_option_entry_t op82;
    dhcpv4_option_entry_t op125;
    dhcpv6_option_entry_t op18;
    dhcpv6_option_entry_t op37;
    pppoe_plus_entry_t pppoe;
    bcmp_cb_t bcmp_cb;
    cs_uint8 mac[6];
}dhcp_option_cb_t;

typedef struct{
#ifndef CS_BIG_ENDIAN
    cs_uint8 type:4,        /* PPPoE Type (must be 1) */
                ver:4;         /* PPPoE Version (must be 1) */
#else
    cs_uint8 ver:4,      /* PPPoE Type (must be 1) */
                type:4;         /* PPPoE Version (must be 1) */
#endif
    cs_uint8  code;        /* PPPoE code */
    cs_uint16 session;    /* PPPoE session */
    cs_uint16 length;     /* Payload length */
    cs_uint8  payload[0]; /* A bit of room to spare */
}__attribute__ ((aligned(1), packed)) pppoe_hdr_t;


#define pppoe_payload_offset() ((cs_uint32)&(((pppoe_hdr_t*)0)->payload) )
                                              

typedef struct _pppoe_plus_hdr_s
{
    cs_uint8 tag;//vendor stpcific 01
    cs_uint8 tag1;//vendor stpcific 10
    cs_uint8 rev;
    cs_uint8 len;
}_pppoe_plus_hdr_t;


typedef struct {
#define DHCP_V4_OPTION60_MASK_BIT (1<<0)
#define DHCP_V4_OPTION82_MASK_BIT (1<<1)
#define DHCP_V4_OPTION125_MASK_BIT (1<<2)
#define DHCP_V6_OPTION18_MASK_BIT (1<<3)
#define DHCP_V6_OPTION37_MASK_BIT (1<<4)
#define PPPOE_PLUS_MASK_BIT (1<<5)
#define BCMP_MASK_BIT (1<<6)
    cs_uint8 v4_option60;
    cs_uint8 v4_option82;
    cs_uint8 v4_option125;
    cs_uint8 v6_option18;
    cs_uint8 v6_option37;
    cs_uint8 pppoe_plus;
    cs_uint8 bcmp;
    cs_uint32 mask;
}dhcp_option_enable_t;

typedef struct dhcp_access_node_id_s_{
    cs_uint8 id[50]; //access node identifier
    cs_uint8 ani_rack;
    cs_uint8 ani_frame;
    cs_uint8 ani_slot;
    cs_uint8 ani_subslot;
    cs_uint8 ani_port;
    cs_uint8 onu_id;
    cs_uint8 rev[8];
}__attribute__ ((aligned(1), packed)) dhcp_access_node_id_t;


extern cs_int32 dhcpv4_add(cs_pkt_t *pkt);
extern cs_int32 dhcpv6_add(cs_pkt_t *pkt);
extern void dhcp_option_init(void);
#endif



#endif/* _APP_IP_TYPES_H_ */

