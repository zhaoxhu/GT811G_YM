#ifndef _MC_TYPE_H
#define _MC_TYPE_H

#include "cs_types.h"
#include "cs_cmn.h"

#include "util_list.h"
#include "util_hash.h"

#define MC_IPV4_IP_LEN      4
#define MC_IPV6_IP_LEN      16

#define MC_MAX_SRC_IP_NUM     4

#define MC_STATIC_GROUP_FLAG        0x1
#define MC_DYN_GROUP_FLAG             0x0

/*IGMP parameter*/
#define MC_MAX_GROUP_NUM                                       256
#define ROBUSTNESS_VARIABLE                                    2
#define QUERY_INTERVAL                                              125      
#define QUERY_RESPONSE_INTERVAL                            10
#define GRP_MEMBERSHIP_INTERVAL                            \
    (ROBUSTNESS_VARIABLE*QUERY_INTERVAL+QUERY_RESPONSE_INTERVAL )
#define OTHER_QUERIER_PRESENT_INTERVAL              \
    (ROBUSTNESS_VARIABLE*QUERY_INTERVAL+QUERY_RESPONSE_INTERVAL/2 )
#define STARTUP_QUERY_INTERVAL                               (QUERY_INTERVAL/4)
#define STARTUP_QUERY_COUNT                                   ROBUSTNESS_VARIABLE
#define LAST_MEMBER_QUERY_INTERVAL                      1
#define LAST_MEMBER_QUERY_COUNT                            ROBUSTNESS_VARIABLE
#define LAST_MEMBER_QUERY_TIME                             \
    (LAST_MEMBER_QUERY_INTERVAL*LAST_MEMBER_QUERY_COUNT)
#define UNSOLICITED_REPORT_INTERVAL                      1   
#define OLDER_VERSION_QUERIER_PRESENT_TIMEOUT   \
    (ROBUSTNESS_VARIABLE*QUERY_INTERVAL+QUERY_RESPONSE_INTERVAL )
#define OLDER_HOST_PRESENT_INTERVAL                     \
    (ROBUSTNESS_VARIABLE*QUERY_INTERVAL+QUERY_RESPONSE_INTERVAL )

typedef enum {
    MC_VER_UNDEF = 0,  
    IGMP_V1 = 1,    
    IGMP_V2 = 2,
    IGMP_V3 = 3,
    MLD_V1 = 4,
    MLD_V2 = 5
} mc_version_t;

typedef enum {    
    MC_SNOOPING,
    MC_MANUAL,
    MC_PROXY,
    MC_DISABLE
} mc_mode_t;

typedef enum {
    MC_ENGINE_L2,
    MC_ENGINE_GDA_MAC_SIP,
    MC_ENGINE_IP
} mc_fwd_engine_t;

typedef enum {
    MC_IVL = 0x0,
    MC_SVL = 0x1,
} mc_vlan_learn_t;

typedef enum {
    SRC_INCLUDE = 0,    
    SRC_EXCLUDE = 1
} mc_filter_mode_t;

typedef enum {
    PORT_FAST_LEAVE = 0,    
    PORT_NON_FAST_LEAVE = 1,
    PORT_UNDEFINED_LEAVE_MODE
} mc_leave_mode_t;

typedef enum {
    MC_PKT_DROP,
    MC_PKT_FWD,
    MC_PKT_BYPASS
} mc_fwd_action_t;

typedef enum {
    MC_IPV4 = 0,    
    MC_IPV6 = 1
} mc_ip_type_t;

typedef struct {
    union 
    {
        cs_uint8     addr8[16];
        cs_uint16   addr16[8];
        cs_uint32   addr32[4];
    } ip6_u;
   
#define mc_addr8			ip6_u.addr8
#define mc_addr16		ip6_u.addr16
#define mc_addr32		ip6_u.addr32
} ipv6_addr_t;

typedef  union {        
    cs_uint32       ipv4;
    ipv6_addr_t    ipv6;
 } ip_addr_t ;

typedef struct   {        
    ip_addr_t    addr;
    cs_uint32    type;
} ip_type_t;


typedef struct {
    cs_dev_id_t     device;
    cs_uint16        vlanid;
    cs_port_id_t    portid;

    /* mc_version_t   version;         igmp v1, v2, v3 and mld v1, v2 */ 
    ip_type_t        mc_grp;         /* group */
    
    cs_uint8        flag;              /* reserved */    
    cs_uint8        filter_mode;   /* include or exclude */
    cs_uint8        src_num;          
    ip_type_t       src_ip_tbl[MC_MAX_SRC_IP_NUM];        
} mc_fdb_entry_t;

typedef struct {
    cs_boolean up_port_lrn;
    cs_port_id_t up_port;
}mc_up_port_cfg_t;

typedef struct {
    cs_uint32 grp;
    cs_uint32 port_map;
} mc_ipv4_grp_t;

typedef struct {
    cs_ipv6_t grp;
    cs_uint32 port_map;
} mc_ipv6_grp_t;

typedef mc_fwd_action_t (*mc_port_ingress_filter_routine_t)(cs_pkt_t *, cs_uint8, cs_uint8 *);
typedef cs_status (*mc_port_egress_filter_routine_t)(cs_port_id_t, cs_uint8, cs_uint8 *, cs_uint16 *);

typedef void (*mc_tmr_handler_t)();

#endif  /* #ifndef _MC_TYPE_H */

