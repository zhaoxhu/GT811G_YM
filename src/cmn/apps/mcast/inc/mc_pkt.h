#ifndef _MC_PKT_H
#define _MC_PKT_H
#include "mc_type.h"

#include "cs_cmn.h"

/* ip version */
#define MC_TYPE_IPV4    0x0800
#define MC_TYPE_IPv6    0x86dd

/* PDU type */
#define IGMP_V1_REPORT  0x12
#define IGMP_V2_REPORT  0x16
#define IGMP_V3_REPORT  0x22
#define IGMP_V2_LEAVE     0x17
#define IGMP_QUERY          0x11

#define MLD_QUERY           130
#define MLD_V1_REPORT   131
#define MLD_V1_DONE       132
#define MLD_V2_REPORT   143

#define   IGMP_V1V2_QUERY_PDU_LEN                    8
#define   MLD_V1_QUERY_PDU_LEN                         24

typedef enum {
    IGMP_V1_QUERY = 1,    
    IGMP_V2_QUERY = 2,
    IGMP_V3_QUERY = 3,   
    MLD_V1_QUERY = 4,
    MLD_V2_QUERY = 5,
}mc_query_type_t;

typedef enum {
    MODE_IS_INCLUDE = 1,    
    MODE_IS_EXCLUDE = 2,
    CHANGE_TO_INCLUDE_MODE = 3,   
    CHANGE_TO_EXCLUDE_MODE = 4,
    ALLOW_NEW_SOURCES = 5,
    BLOCK_OLD_SOURCES = 6
} mc_record_type_t;

/* 
 * ethernet header defines
 */

typedef struct {
    cs_mac_t    dst;
    cs_mac_t    src;
    cs_uint16   ethertype;
} __attribute__((packed)) mc_eth_hdr_t;

typedef struct {
    cs_uint16 tpid;
    cs_uint16 tag;
} __attribute__((packed)) mc_vlan_hdr_t;

typedef struct {
    cs_uint8        dest[6];
    cs_uint8        src[6];
    cs_uint16      svlan_tpid;
    cs_uint16      svlan_vlanid;
    cs_uint16      cvlan_tpid;
    cs_uint16      cvlan_vlanid;
    cs_uint16      ethertype;
} __attribute__((packed)) mc_ether_header_t;


/*
 * ip headers defines 
 */

typedef struct {
    cs_uint8    version_ihl;  
    cs_uint8    tos;
    cs_uint16  tot_len;  
    cs_uint16  id;
    cs_uint16  frag_off;
    cs_uint8    ttl;
    cs_uint8    protocol_type;
    cs_uint16  check;
    cs_uint32  src_ip;
    cs_uint32  dst_ip;
    cs_uint8   data_option[4];
} __attribute__((packed)) igmp_ip_header_t;

typedef struct {
#if defined(__LITTLE_ENDIAN__)
    cs_uint8	priority:4,
                    version:4;
#elif defined(__BIG_ENDIAN__)
    cs_uint8	version:4,
                    priority:4;
#else
#error	"Please fix mc_type.h"
#endif
    cs_uint8	flow_lbl[3];

    cs_uint16   payload_len;
    cs_uint8	nexthdr;
    cs_uint8	hop_limit;

    ipv6_addr_t      saddr;
    ipv6_addr_t      daddr;
} __attribute__((packed)) mld_ip_header_t;


/*
 * IGMP/MLD PDU
 */
  
typedef struct {
    cs_uint8   type; 
    union {
        cs_uint8   max_resp_time;         /*IGMPv1, IGMPv2,IGMPv3 query segment */
        cs_uint8   reserved;                    /*IGMPv3 report , MLDv2 report */   
        /*for IPV6*/
        cs_uint8   code ;                          /*MLDv1 report , MLDv1 and MLDv2 query*/
    }__attribute__((packed))  time_key;

    cs_uint16  checksum;
     union {
        cs_uint32    ipv4_grp;                           /*v1, v2, v3 query */
        struct {
            cs_uint16  continue_reserved;               /*IGMPv3 report , MLDv2 report*/
            cs_uint16  num_grp_records;
            /*cs_uint8    data[1];*/
        }__attribute__((packed))  reserved;
      
        struct {
            cs_uint16  max_resp_time;                        /*MLDv1 report , MLDv1 and MLDv2 query*/
            cs_uint16  Reserved;
        } __attribute__((packed)) mld_key;   

    }__attribute__((packed)) grp_key;
} __attribute__((packed)) mc_pdu_t;


typedef struct {
    mc_pdu_t    original_pdu;
    cs_uint8        qrv:3,
                        s:1,
                        resv:4;
    cs_uint8        qqic;
    cs_uint16   num_sources;
} __attribute__((packed)) igmp_v3_query_pdu_t;

/*MLDv1 query */
typedef struct {
    mc_pdu_t     original_pdu;
    ip_addr_t     mc_grp;
} __attribute__((packed)) mld_v1_pdu_t;

/*MLDv2 query */
typedef struct {
    mld_v1_pdu_t    mld_v1_query;
    cs_uint8        qrv:3,
                        s:1,
                    resv:4;
    cs_uint8    qqic;
    cs_uint16   num_sources;
} __attribute__((packed)) mld_v2_query_pdu_t;


typedef struct {
        cs_uint8    record_type;
        cs_uint8    aux_data_len;
        cs_uint16  num_sources;
        ip_addr_t   mc_grp;
        /*ip_type_t  src_ip[1];*/
        /*cs_uint8    auxiliary_data[];*/
} __attribute__((packed)) mc_group_record_pdu_t;



#define MC_IS_QUERY_PKT(pdu_type)  ((pdu_type == IGMP_QUERY) || (pdu_type == MLD_QUERY))
#define MC_IS_LEAVE_PKT(pdu_type)  ((pdu_type == IGMP_V2_LEAVE) || (pdu_type == MLD_V1_DONE))


/*
* Group record format:
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
|record type    |aux data len            | number of source                                          |
|                                               multicast address                                                  |
|                                               source    address                                                  |
|                                               source    address                                                  |
|                                               source    address                                                  |
|                                               auxiliary       data                                                 |
*/

#define MC_IP_ADDR_LEN(ip_type) ((ip_type) == MC_IPV4 ?(MC_IPV4_IP_LEN):(MC_IPV6_IP_LEN))
#define MC_SIZEOF_SRC_LIST(ip_type,num) (MC_IP_ADDR_LEN((ip_type))*(num))

/* 
* The start address of mc_grp +  the size of mc_grp .
* This Macro used for IGMP V3/MLD V2 REPORT ONLY.
*/
#define MC_SRC_OFFSET_PER_GRP(ip_type) \
                                               (cs_uint32)((cs_uint32)&(((mc_group_record_pdu_t*)0)->mc_grp) \
                                               + MC_IP_ADDR_LEN((ip_type)))

/*
* This MACRO is ONLY used to caculate the size of group record in IGMP V3/MLD V2 REPORT packets. 
*/
#define MC_SIZEOF_GROUP_RECORD(ip_type,grp_record) \
                                                         (MC_SRC_OFFSET_PER_GRP(ip_type) \
	                                                  + MC_SIZEOF_SRC_LIST((ip_type),ntohs((grp_record).num_sources))\
	                                                  + (grp_record).aux_data_len)
#endif /* #ifndef _MC_PKT_H */

