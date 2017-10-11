#ifndef _MC_GRP_H
#define _MC_GRP_H

#define  MC_GROUP_HASH_LEN      64

/*
 * port grp entry
 */

mc_port_group_state_list_t * mc_port_grp_create();
void  mc_port_grp_destroy(mc_port_group_state_list_t *port_state);
cs_status mc_port_grp_add(mc_group_node_t  *grp_state, mc_port_group_state_list_t *port_state);
cs_status mc_port_grp_rm(mc_port_group_state_list_t * port_state);
cs_boolean  mc_port_grp_exist(mc_port_group_state_list_t *port_state);
mc_port_group_state_list_t *mc_port_grp_find(mc_port_t *port, mc_group_node_t *grp_state);
mc_port_group_state_list_t *mc_port_grp_search(
    mc_node_t *node, 
    mc_port_t *port, 
    cs_uint16 vlan, 
    ip_type_t *mc_grp);
void mc_port_grp_copy(
    mc_port_group_state_list_t  *dst_port_state, 
    mc_port_group_state_list_t  *src_port_state);
mc_port_group_state_list_t *mc_port_grp_find_globally(
    mc_node_t *mc_node, 
    mc_port_t  *port,
    ip_type_t *mc_grp);

/*
 * grp entry
 */
 
mc_group_node_t *mc_grp_create();
void  mc_grp_destroy(mc_group_node_t *grp_state);
cs_status  mc_grp_add(cs_hash_tbl *  grp_table, mc_group_node_t *grp_state);
cs_status  mc_grp_rm(cs_hash_tbl *  grp_table,  mc_group_node_t * grp_state);
cs_boolean  mc_grp_exist(mc_group_node_t  * grp_state);
mc_group_node_t *mc_grp_find(mc_node_t *mc_node, cs_uint16 vlanid, ip_type_t *mc_grp);
mc_group_node_t *mc_grp_find_globally(mc_node_t *mc_node, ip_type_t *mc_grp);

/*
 * grp table 
 */

cs_hash_tbl  *mc_grp_table_create();
cs_status  mc_grp_table_destroy(cs_hash_tbl   *grp_table);
cs_boolean mc_grp_table_exist(cs_hash_tbl *grp_table);

#endif /* #ifndef _MC_GRP_H */

