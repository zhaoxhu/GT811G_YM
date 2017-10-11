#ifndef _MC_FDB_H
#define _MC_FDB_H

#include "mc_type.h"
#include "mc_timer.h"
#include "mc_core.h"

typedef struct {        
    void          *mc_node;    /* iterator to mc_node_t */

    ip_type_t   mc_grp;       
    cs_uint16   vlanid;
    cs_uint16   flag;        /* resv */

    cs_list       port_state_list;       /* mc_port_group_state_list_t */                
 } mc_group_state_t;

typedef struct  {
    cs_hash_node    node;
    mc_group_state_t    entry;
} mc_group_node_t;


typedef struct  {
    void            *port;                    /*mc_port_t*/
    cs_uint32     flag;

    mc_group_node_t    *grp_father;

    /* version_state_t       ver_state;  temp disable version manage */           
    cs_uint64       update_time;     /* record upate time to suppress report */

    cs_uint32       filter_mode;     /* INCLUDE or EXCLUDE */
    mc_age_t       grp_timer;       /* for EXCLUDE mode */
    cs_list           src_list;       
    cs_list           aged_src_list;      
} mc_port_group_state_t;

typedef struct  {
    cs_node                  node;
    mc_port_group_state_t   entry;
} mc_port_group_state_list_t;

/* source record and source recoard list */
typedef struct {
    ip_type_t   src_ip;
    mc_age_t   timer;   
} mc_source_record_t;

typedef struct {        
    cs_node                  node;
    mc_source_record_t   entry;
} mc_source_record_list_t;

void mc_node_fdb_age(mc_node_t *node);
cs_status mc_group_timer_update( 
    mc_port_group_state_list_t *port_state , 
    mc_port_group_state_list_t *  new_port_state);
cs_status mc_port_fdb_entry_update(
    mc_port_group_state_list_t *olb_port_grp,
    mc_port_group_state_list_t *new_port_grp);
cs_status mc_port_fdb_entry_add(mc_port_group_state_list_t  *port_state);
cs_status mc_port_fdb_entry_del(mc_port_group_state_list_t  *port_state);

void mc_fdb_entry_del(mc_group_node_t * grp_state);
void mc_fdb_tbl_clear(cs_hash_tbl *grp_table);
void mc_fdb_flush(mc_node_t  *mc_node);
void mc_port_fdb_flush(mc_port_t *port);
void mc_fdb_get_grp_tbl(
    mc_node_t *mc_node,
    mc_ipv4_grp_t *v4_tbl, 
    cs_uint32 *v4_tbl_len,
    mc_ipv6_grp_t *v6_tbl,
    cs_uint32 *v6_tbl_len);

#endif /* #ifndef _MC_FDB_H */

