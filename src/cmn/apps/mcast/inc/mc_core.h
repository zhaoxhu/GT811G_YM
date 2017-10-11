#ifndef _MC_CORE_H
#define _MC_CORE_H

#include "mc_pkt.h"

#define MC_MAX_PORT    8
#define MC_MAX_VLAN     4095

#define         MC_PORT_DEFAULT_GROUP           64

/*
 *  port ctrl block 
 */
typedef struct {
    void            *mc_node;

    cs_port_id_t  portid;                     

    mc_leave_mode_t   leave_mode;
        
    /* port IGMPv1 or IGMPv2, IGMPv3 state */
    mc_version_t          igmp_version_level;     /* IGMPv1, IGMPv2 or IGMPv3 */
    mc_version_t          mld_version_level;       /* MLDv1, MLDv2 */
      
    cs_uint16     max_grp;   
    cs_uint16     grp_count;

     /*port packet stats*/

     /* rx */
    cs_uint64     in_igmp_query;
    cs_uint64     in_igmpv1_report;
    cs_uint64     in_igmpv2_report;
    cs_uint64     in_igmpv2_leave;
    cs_uint64     in_igmpv3_report;
    
    cs_uint64     in_mld_query;
    cs_uint64     in_mldv1_report;
    cs_uint64     in_mldv2_report;
    cs_uint64     in_mldv1_done;

    /* tx */
    cs_uint64     out_igmp_query;
    cs_uint64     out_igmpv1_report;
    cs_uint64     out_igmpv2_report;
    cs_uint64     out_igmpv2_leave;
    cs_uint64     out_igmpv3_report;
    
    cs_uint64     out_mld_query;
    cs_uint64     out_mldv1_report;
    cs_uint64     out_mldv2_report;
    cs_uint64     out_mldv1_done;        

} mc_port_t;

/*
 *  IGMP/MLD PDU object
 */
typedef struct  {
    mc_port_t *port;     
    
    cs_llid_t     llid;    
    cs_uint16   len;
    cs_uint8     *frame;

    /* l2 info */
    cs_uint16    ether_type;
    cs_uint16    vlanid;    
    cs_uint16   ether_head_len;
    cs_uint16   rsvd;

    /* l3 info */
    cs_uint16   ip_type;
    cs_uint16   ip_head_len;         
    cs_uint8     *ip_hdr;

    /* pdu info */       
    cs_uint16   pdu_len;
    cs_uint8     pdu_type;
    mc_pdu_t   *pdu;
    
    cs_list        record_list;         
} mc_object_t;

/*
 *  node ctrl block
 */
typedef struct  {       
    cs_dev_id_t    device;   

    /* run mode */
    mc_mode_t     mode;        
    mc_fwd_engine_t  fwd_engine;
    mc_vlan_learn_t    vlan_lrn_mode;
    cs_boolean      ipv6_aware;      

    /* capabilities */
    cs_uint16       grp_cap;
    cs_uint16       port_grp_cap;
    /* group num */
    cs_uint16       max_grp_num;
    cs_uint16       cur_grp_num;

    /* protocol parameters */
    cs_uint16   member_interval;
    cs_uint16   query_rsp_interval;
    cs_uint16   query_interval;
    cs_uint16   last_member_query_count;
    cs_uint16   last_member_query_interval;
    cs_uint16   robustness;

    cs_boolean     up_port_learn;
    cs_uint32       port_num;   
    
    mc_port_t  *up_port;  
    mc_port_t  *port[MC_MAX_PORT];   

    mc_port_ingress_filter_routine_t  ing_filter;
    mc_port_egress_filter_routine_t   eg_filter;
    
    cs_hash_tbl    *grp_tbl[MC_MAX_VLAN+1];    /* group table */ 
           
    mc_object_t   message; 
} mc_node_t;

/* 
 * group record
 */

typedef struct  {        
    cs_uint8    flag;   /* static/dynamic */
    cs_uint8    pdu_type;  /* IGMP V1/V2/V3 report, leave, query */       
    cs_uint16  vlanid;   

    ip_type_t  mc_grp;   

    mc_record_type_t    record_type;  
    cs_list                   src_list;            
 } mc_group_record_t;

typedef struct {        
    cs_node                node;
    mc_group_record_t   entry;
 } mc_group_record_list_t;

void mc_pkt_proc(cs_pkt_t *pkt, mc_node_t *node);
cs_status mc_fwd_msg(mc_port_t *port, cs_uint8 pdu_type, cs_uint8 *frame, cs_uint16 len);
cs_status mc_flood_msg(mc_node_t *mc_node, mc_object_t *pObj);
void mc_report_proc(mc_node_t *node, mc_object_t *msg);
void mc_query_proc(mc_node_t *node, mc_object_t *pObj);
cs_status mc_grp_record_handle(
    mc_node_t *mc_node, 
    mc_port_t  *port, 
    mc_group_record_list_t *record);

#endif  /* #ifndef _MC_CORE_H */

