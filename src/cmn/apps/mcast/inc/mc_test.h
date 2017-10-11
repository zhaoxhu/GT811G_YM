#ifndef _MC_TEST_H
#define _MC_TEST_H

#define  MC_IP_BUF_LEN      64
#define  MC_FDB_BUF_LEN    128

#define MC_RECORD_BUF_LEN   512

extern cs_uint8 g_mc_dump;

void  mc_src_list_print(cs_list * src_list);

void mc_node_port_stats_show(mc_node_t *node, cs_port_id_t port_id);
void mc_node_port_stats_clr(mc_node_t *node, cs_port_id_t port_id);

void mc_report_msg_show(mc_object_t *msg);
void mc_query_msg_show(mc_object_t *msg);
void mc_new_grp_dump(mc_group_record_t *record);
void mc_add_port_grp_dump(mc_port_t *port, mc_group_record_t *record);
void mc_update_port_grp_dump(
    mc_port_group_state_list_t *port_grp, 
    mc_group_record_list_t *record);
void mc_del_port_grp_dump(mc_port_group_state_list_t *port_grp);
void mc_handle_record_show(mc_group_record_t *record);

void mc_port_fdb_show(mc_node_t  *mc_node, mc_port_t *port);
void mc_vlan_fdb_show(mc_node_t *mc_node, cs_uint16 vid);
void mc_fdb_show(mc_node_t *mc_node);

#define MC_HANDLE_RECORD_DUMP(record)    \
    do {    \
        if(g_mc_dump)  \
            mc_handle_record_show(record);   \
    }   \
    while(0)
        
#define MC_REPORT_RECORD_DUMP(pObj)    \
    do {    \
        if(g_mc_dump)  \
            mc_report_msg_show(pObj);   \
    }   \
    while(0)

#define MC_QUERY_RECORD_DUMP(pObj)    \
    do {    \
        if(g_mc_dump)  \
            mc_query_msg_show(pObj);    \
    }   \
    while(0)

#define MC_NEW_GRP_DUMP(record)    \
    do {    \
        if(g_mc_dump)  \
            mc_new_grp_dump(record);    \
    }   \
    while(0)    

#define MC_ADD_PORT_GRP_DUMP(port, record)    \
    do {    \
        if(g_mc_dump)  \
            mc_add_port_grp_dump(port, record);    \
    }   \
    while(0)     

#define MC_UPDATE_PORT_GRP_DUMP(port_grp, record)    \
    do {    \
        if(g_mc_dump)  \
            mc_update_port_grp_dump(port_grp, record);    \
    }   \
    while(0)     
        
#define MC_DEL_PORT_GRP_DUMP(port_grp)    \
    do {    \
        if(g_mc_dump) { \
            mc_del_port_grp_dump(port_grp);    \
        }   \
    }   \
    while(0)       
    

#endif /* #ifndef _MC_TEST_H */

