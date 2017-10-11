#include "mc_type.h"
#include "mc_fdb.h"
#include "mc_grp.h"
#include "mc_util.h"
#include "mc_src.h"
#include "mc_timer.h"
#include "mc_test.h"
#include "mc_adapter.h"


cs_status  mc_port_fdb_entry_get(
    mc_node_t *mc_node,
    cs_port_id_t  portid,     
    cs_uint32      offset,
    cs_uint8        *mac,
    cs_uint16     *vlanid);

/*
 * fdb utilities
 */
 
cs_status mc_group_state_to_fdb(
    mc_port_group_state_list_t *port_state, 
    mc_fdb_entry_t *out_port_fdb)
{
    mc_node_t   *mc_node = NULL;
    mc_port_t    *port = NULL;
    
    cs_list   *src_list = NULL;
        
    mc_group_node_t   *grp_state = NULL;
    cs_uint32 src_num;

    if (!port_state || !out_port_fdb) {
        return CS_E_ERROR;
    }

    memset(out_port_fdb, 0, sizeof(mc_fdb_entry_t ));

    port = port_state->entry.port;
    mc_node = (mc_node_t *)port->mc_node;
    grp_state = port_state->entry.grp_father;
    
    out_port_fdb->device = mc_node->device;
    out_port_fdb->portid = port->portid;
    out_port_fdb->vlanid = grp_state->entry.vlanid;
    memcpy(&out_port_fdb->mc_grp, &grp_state->entry.mc_grp, sizeof(out_port_fdb->mc_grp));   
    
    /* out_port_fdb->version = port_state->entry.ver_state.version; */
    out_port_fdb->filter_mode = port_state->entry.filter_mode;
    if(port_state->entry.filter_mode == SRC_INCLUDE) {
        src_list = &port_state->entry.src_list;
    }
    else {
        src_list = &port_state->entry.aged_src_list;
    }
    mc_src_num_get(src_list, &src_num);

    if(src_num) {
        mc_source_record_list_t  *src_entry = NULL;
        cs_uint32 src_id = 0;    

        out_port_fdb->src_num = src_num;
        
        cs_lst_scan(src_list, src_entry, mc_source_record_list_t  *) {
            memcpy(&out_port_fdb->src_ip_tbl[src_id], &src_entry->entry.src_ip,
                          sizeof(out_port_fdb->src_ip_tbl[src_id]));
            src_id++;

            if(src_id >= MC_MAX_SRC_IP_NUM) {
                break;
            }
        }
  
    }

    return CS_E_OK;

}


/*
 * fdb aging
 */
 
cs_boolean mc_port_grp_age(mc_port_group_state_list_t *port_state)
{
    if(port_state->entry.filter_mode == SRC_EXCLUDE ) {
        mc_timer_age(&port_state->entry.grp_timer);

        if(mc_timer_is_aged(&port_state->entry.grp_timer)) {
            /*
             *  action on group timer aged out:
             *  1. clear src list without timer
             *  2. switch to include mode
             */
             
            mc_src_list_clear(&port_state->entry.aged_src_list);
            port_state->entry.filter_mode = SRC_INCLUDE;

            return TRUE;
        }

    }

    return FALSE;
}

cs_boolean mc_port_src_age(mc_port_group_state_list_t *port_state)
{
    mc_source_record_list_t *src = NULL;
    mc_source_record_list_t  *prev = NULL;
    cs_list *src_list = NULL;
    cs_list *aged_src_list = NULL;

    cs_boolean src_update = FALSE;

    src_list = &port_state->entry.src_list;
    aged_src_list = &port_state->entry.aged_src_list;

    for(src = (mc_source_record_list_t * )cs_lst_first(src_list ); src;) {
        mc_timer_age(&src->entry.timer);

        if(mc_timer_is_aged(&src->entry.timer)) {
            /* 
             * action on src aged out:
             * 1. for exclude mode, migrate src entry to aged src list
             * 2. for include mode, delete src
             */
             
            prev = (mc_source_record_list_t *)cs_lst_prev((cs_node *)src);
            mc_src_rm(src_list, src);
            if (port_state->entry.filter_mode == SRC_EXCLUDE) {
                /*migrate src ip node from timer table to no-timer table*/
                mc_src_add(aged_src_list,  src);
             }
            else {
                mc_free(src);
            }
            
            src_update = TRUE;
            if(prev != NULL) {
                src = (mc_source_record_list_t * ) cs_lst_next(&prev->node);
            }
            else {
                src = (mc_source_record_list_t * )cs_lst_first(src_list );                
            }
        }
        else {
            src= (mc_source_record_list_t *)cs_lst_next((cs_node *)src); 
        }        
                         
    }
    
    return src_update;
    
}

cs_status mc_port_fdb_age(mc_port_group_state_list_t *port_state)
{
    cs_boolean    update_grp = FALSE;
    cs_boolean    update_src = FALSE;
    
    mc_port_group_state_list_t  *old_port_state = mc_port_grp_create();    

    if(old_port_state == NULL){
        return CS_E_ERROR;
    }
    /* store original port state */
    mc_port_grp_copy(old_port_state,  port_state);


    update_grp = mc_port_grp_age(port_state);
    update_src = mc_port_src_age(port_state);     

    if(update_grp || update_src) {
        mc_fdb_entry_t      old_port_fdb;        

        mc_group_state_to_fdb(old_port_state , &old_port_fdb);
        if(mc_port_grp_exist(port_state)) {            
            mc_fdb_entry_t      port_fdb;
            
            mc_group_state_to_fdb(port_state , &port_fdb);
            
            mc_port_fdb_update(&old_port_fdb,  &port_fdb);
        } 
        else {
            mc_port_fdb_del(&old_port_fdb);
        }
    }

    /* destroy old_grp_state */
    mc_port_grp_destroy(old_port_state);

    return CS_E_OK;
    
}

cs_status mc_fdb_age(mc_group_node_t * grp_state)
{
    cs_list    *port_state_list = NULL;
    mc_port_group_state_list_t   *port_state = NULL;
    mc_port_group_state_list_t   *prev = NULL;
    
    port_state_list = &grp_state->entry.port_state_list;

     for(port_state = (mc_port_group_state_list_t * )cs_lst_first(port_state_list); port_state;) {
        prev = (mc_port_group_state_list_t *)cs_lst_prev((cs_node *)port_state);

        mc_port_fdb_age(port_state);
        if (!mc_port_grp_exist(port_state)) {   /* port fdb is aged out and delete it */
            mc_port_grp_rm(port_state);
            mc_port_grp_destroy(port_state);
            if(prev != NULL) {
                port_state = (mc_port_group_state_list_t * )cs_lst_next(&prev->node);
            }
            else {
                port_state = (mc_port_group_state_list_t * )cs_lst_first(port_state_list );
            }
        } 
        else {
            port_state=(mc_port_group_state_list_t * )cs_lst_next((cs_node *)port_state);
        }

    }

    return CS_E_OK;

}


cs_status mc_fdb_tbl_age(cs_hash_tbl *grp_table)
{    
    mc_group_node_t  *grp_state = NULL;
    mc_group_node_t  *prev = NULL;
    cs_list     *grp_list = NULL;

    cs_int32   hash_index = 0;

    if(grp_table == NULL) {
        return CS_E_ERROR;
    }

    cs_scan_hash_bucket(grp_table , hash_index) {
        grp_list=&grp_table->hash_list[hash_index];
        
        for(grp_state = (mc_group_node_t *)cs_lst_first(grp_list); grp_state; ) {      
            
            mc_fdb_age(grp_state);           

            if (!mc_grp_exist(grp_state)) {           
                prev = (mc_group_node_t *)cs_lst_prev((cs_node *)grp_state);

                mc_grp_rm(grp_table, grp_state);
                mc_grp_destroy(grp_state);
                if(prev) {
                    grp_state = (mc_group_node_t * )cs_lst_next(&prev->node);
                }
                else {
                    grp_state = (mc_group_node_t * )cs_lst_first(grp_list );
                }
            } 
            else {
                grp_state = (mc_group_node_t *)cs_lst_next((cs_node *)grp_state);
            }
            
        }
    }

    return CS_E_OK;

}

void mc_node_fdb_age(mc_node_t *node)
{
    cs_uint16 vid;
    cs_uint16 max_vid = 1;

    if(node->vlan_lrn_mode == MC_IVL) {
        max_vid = MC_MAX_VLAN;
    }

    for(vid = 0; vid < max_vid; vid++) {
        cs_hash_tbl * grp_table = node->grp_tbl[vid];
        if(grp_table) {
            if(mc_grp_table_exist(grp_table)) {
                mc_fdb_tbl_age(grp_table);
            }
            else {
                mc_grp_table_destroy(grp_table);
                MC_LOG(IROS_LOG_LEVEL_INF ,"\r\n group table  %d destroy  \r\n", vid);
                node->grp_tbl[vid] = NULL;
            }
        }
        
    }
}


/*
 * fdb operations
 */

cs_status mc_port_fdb_entry_add(mc_port_group_state_list_t  *port_state)
{
    mc_fdb_entry_t  port_fdb;

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_port_fdb_entry_add]\n");
    
    if(mc_group_state_to_fdb(port_state , &port_fdb)) {
        return CS_E_RESOURCE;
    }
    else {        
        return mc_port_fdb_add(&port_fdb);
    }
}

cs_status mc_port_fdb_entry_update(
    mc_port_group_state_list_t *olb_port_grp,
    mc_port_group_state_list_t *new_port_grp)
{
    mc_fdb_entry_t old_port_fdb;
    mc_fdb_entry_t new_port_fdb;

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_port_fdb_entry_update]\n");

    /* check src num */
    if(cs_lst_count(&new_port_grp->entry.src_list) +
        cs_lst_count(&new_port_grp->entry.aged_src_list) > MC_MAX_SRC_IP_NUM) {
        MC_LOG(IROS_LOG_LEVEL_INF, "update fdb fail - src num exceed the limit\n");
        return CS_E_RESOURCE;
    }

    mc_group_state_to_fdb(olb_port_grp, &old_port_fdb);
    mc_group_state_to_fdb(new_port_grp, &new_port_fdb);
    
    return mc_port_fdb_update(&old_port_fdb, &new_port_fdb);
}

cs_status mc_port_fdb_entry_del(mc_port_group_state_list_t  *port_state)
{
    mc_fdb_entry_t  port_fdb;    

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_port_fdb_entry_del]\n");
    
    if(mc_group_state_to_fdb(port_state, &port_fdb)) {
        return CS_E_RESOURCE;
    }
    
    mc_port_fdb_del(&port_fdb);
    mc_port_grp_rm(port_state);
    mc_port_grp_destroy(port_state);

    return CS_E_OK;
}


void mc_fdb_entry_del(mc_group_node_t * grp_state)
{
    mc_port_group_state_list_t  *port_state = NULL;
    cs_list  *port_state_list = NULL;

    if(NULL == grp_state) {
        return ;
    }
    
    port_state_list = &grp_state->entry.port_state_list; 
    for(port_state = (mc_port_group_state_list_t * )cs_lst_first( port_state_list ); port_state; ) {

        mc_port_fdb_entry_del(port_state);  
        
        port_state = (mc_port_group_state_list_t * )cs_lst_first( port_state_list);
    }

    return ;
}


void mc_fdb_tbl_clear(cs_hash_tbl *grp_table)
{    
    mc_group_node_t   *grp_state = NULL;
    cs_list     *grp_list = NULL;
    cs_int32   hash_index = 0;
  
    if(grp_table == NULL) {
        return;
    }

    cs_scan_hash_bucket(grp_table, hash_index) {
        
        grp_list = &grp_table->hash_list[hash_index];
        for(grp_state = (mc_group_node_t *)cs_lst_first(grp_list); grp_state; ) {
            mc_fdb_entry_del(grp_state); 
            mc_grp_rm(grp_table, grp_state);
            mc_grp_destroy(grp_state);
            grp_state = (mc_group_node_t *)cs_lst_first(grp_list);
        }
    }

    return ;
}

void mc_fdb_tbl_clear_port(cs_hash_tbl *grp_table, mc_port_t *port)
{
    cs_int32  hash_index = 0;
    cs_list     *grp_list = NULL;
    mc_group_node_t   *grp_state = NULL;
    mc_group_node_t   *prev = NULL;
    mc_port_group_state_list_t *port_state = NULL;
    
    cs_scan_hash_bucket(grp_table , hash_index) {
        grp_list = &grp_table->hash_list[hash_index];

        for(grp_state = (mc_group_node_t * )cs_lst_first(grp_list); grp_state; ){
            prev = (mc_group_node_t *)cs_lst_prev((cs_node *)grp_state);

            port_state = mc_port_grp_find(port, grp_state);
            if (!port_state) {
                grp_state = (mc_group_node_t * )cs_lst_next((cs_node *)grp_state);
                continue;
            }

            mc_port_fdb_entry_del(port_state);

            if(mc_grp_exist(grp_state)) {
                grp_state = (mc_group_node_t * )cs_lst_next((cs_node *)grp_state);
            }
            else {
                mc_grp_rm(grp_table,  grp_state);
                mc_grp_destroy(grp_state);

                if(prev != NULL) {
                    grp_state = (mc_group_node_t * )cs_lst_next(&prev->node);
                } 
                else {
                    grp_state = (mc_group_node_t * )cs_lst_first(grp_list);
                }
            }
        }
    }
}

void mc_port_fdb_flush(mc_port_t *port)
{
    mc_node_t *mc_node = port->mc_node;
    cs_uint16 vid;

    for(vid = 0; vid < MC_MAX_VLAN; vid++) {
        if(mc_node->grp_tbl[vid]) {
            mc_fdb_tbl_clear_port(mc_node->grp_tbl[vid], port);
        }

    }
}

void mc_fdb_flush(mc_node_t  *mc_node)
{
    cs_uint16  vid;

    for(vid =0; vid < MC_MAX_VLAN; vid++) {
        mc_fdb_tbl_clear(mc_node->grp_tbl[vid]);
    }

    mc_node->cur_grp_num = 0;    
}


void mc_fdb_get_grp_tbl(
    mc_node_t *mc_node,
    mc_ipv4_grp_t *v4_tbl, 
    cs_uint32 *v4_tbl_len,
    mc_ipv6_grp_t *v6_tbl,
    cs_uint32 *v6_tbl_len)
{
    cs_uint16 vid;
    cs_uint16 max_vid = 1;

    cs_int32      hash_index = 0;
    cs_hash_tbl  *grp_tbl = NULL;
    cs_list          *grp_list = NULL;
    mc_group_node_t   *grp_state = NULL;

    cs_uint32 v4_cnt = 0;
    cs_uint32 v6_cnt = 0;

    cs_list                          *port_state_list = NULL;
    mc_port_group_state_list_t  *port_state = NULL;

    if(v4_tbl && v4_tbl_len) {
        memset(v4_tbl, 0, *v4_tbl_len * sizeof(mc_ipv4_grp_t));
    }

    if(v6_tbl && v6_tbl_len) {
        memset(v6_tbl, 0, *v6_tbl_len * sizeof(mc_ipv6_grp_t));
    }

    if(MC_IVL == mc_node->vlan_lrn_mode) {
        max_vid = MC_MAX_VLAN;
    }

    for(vid = 0; vid < max_vid; vid++) {
        grp_tbl = mc_node->grp_tbl[vid];        

        if(!grp_tbl) {
            continue ;
        }

        cs_scan_hash_bucket(grp_tbl , hash_index) {
            grp_list = &grp_tbl->hash_list[hash_index];
             cs_scan_hash_record(grp_list , grp_state , mc_group_node_t *) { 
                
                if(MC_IPV4 == grp_state->entry.mc_grp.type) {
                    if(v4_cnt < *v4_tbl_len && v4_tbl && v4_tbl_len) {
                        v4_tbl[v4_cnt].grp = grp_state->entry.mc_grp.addr.ipv4;
                        v4_tbl[v4_cnt].port_map = 0;

                        port_state_list = &grp_state->entry.port_state_list;
                        
                        cs_lst_scan(port_state_list, port_state, mc_port_group_state_list_t * ) {
                            v4_tbl[v4_cnt].port_map |= 
                                1 << (((mc_port_t *)port_state->entry.port)->portid - 1);
                        }
                        
                        v4_cnt++;
                    }
                }
                else {
                    if(v6_cnt < *v6_tbl_len && v6_tbl && v6_tbl_len) {
                        memcpy((cs_uint8 *)v6_tbl[v6_cnt].grp, 
                            grp_state->entry.mc_grp.addr.ipv6.mc_addr8, sizeof(cs_ipv6_t));

                        v6_tbl[v6_cnt].port_map = 0;

                        port_state_list = &grp_state->entry.port_state_list;
                        
                        cs_lst_scan(port_state_list, port_state, mc_port_group_state_list_t * ) {
                            v6_tbl[v6_cnt].port_map |= 
                                1 << ( ((mc_port_t *)port_state->entry.port)->portid - 1);
                        }
                        
                        v6_cnt++;
                    }
                }
                
            }
        }
        
    }

    *v4_tbl_len = v4_cnt;
    *v6_tbl_len = v6_cnt;
    
}

cs_status  mc_port_fdb_entry_get(
    mc_node_t *mc_node,
    cs_port_id_t  portid,     
    cs_uint32      offset,
    cs_uint8        *mac,
    cs_uint16     *vlanid)
{
    cs_uint16 vid;
    cs_uint16 max_vid = 1;
    cs_int32      hash_index = 0;
    cs_hash_tbl  *grp_tbl = NULL;
    cs_list          *grp_list = NULL;
    mc_group_node_t   *grp_state = NULL;
    cs_list                          *port_state_list = NULL;
    mc_port_group_state_list_t  *port_state = NULL;
    cs_uint32       grp_count=0;
    cs_status       ret=CS_E_ERROR;


    if(MC_IVL == mc_node->vlan_lrn_mode) {
        max_vid = MC_MAX_VLAN;
    }

    for(vid = 0; vid < max_vid; vid++) {
        grp_tbl = mc_node->grp_tbl[vid];        

        if(!grp_tbl) {
            continue ;
        }

        cs_scan_hash_bucket(grp_tbl , hash_index) {
            grp_list = &grp_tbl->hash_list[hash_index];
             cs_scan_hash_record(grp_list , grp_state , mc_group_node_t *) { 

                port_state_list = &grp_state->entry.port_state_list;
                cs_lst_scan(port_state_list, port_state, mc_port_group_state_list_t * ) {
                    if (((mc_port_t *)port_state->entry.port)->portid ==portid ) {
                        grp_count++;
                        break;
                    }
                }
               
                if ( grp_count==offset+1 ) {

                    if(grp_state->entry.mc_grp.type == MC_IPV4) {
                        mac[0] = 0x01;
                        mac[1] = 0x00;
                        mac[2] = 0x5e;
                        
                        mac[3] = (grp_state->entry.mc_grp.addr.ipv4 >> 16) & 0x7f;
                        mac[4] = (grp_state->entry.mc_grp.addr.ipv4 >> 8) & 0xff;
                        mac[5] = grp_state->entry.mc_grp.addr.ipv4 & 0xff;            
                    }
                    else {
                        mac[0] = 0x33;
                        mac[1] = 0x33;

                        mac[2] = grp_state->entry.mc_grp.addr.ipv6.mc_addr8[15];
                        mac[3] = grp_state->entry.mc_grp.addr.ipv6.mc_addr8[14];
                        mac[4] = grp_state->entry.mc_grp.addr.ipv6.mc_addr8[13] ;
                        mac[5] = grp_state->entry.mc_grp.addr.ipv6.mc_addr8[12];
                    }

                    *vlanid=grp_state->entry.vlanid;

                    ret=CS_E_OK;
                    
                    goto END;

                }
                
            }
        }
        
    }


END:
    return ret;
    
}



