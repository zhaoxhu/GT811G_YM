#include "mc_type.h"
#include "mc_timer.h"
#include "mc_fdb.h"
#include "mc_core.h"
#include "mc_util.h"
#include "mc_src.h"
#include "mc_grp.h"
#include "mc_test.h"
#include "mc_adapter.h"

/* 
 * port grp entry
 */

mc_port_group_state_list_t * mc_port_grp_create()
{
    mc_port_group_state_list_t  *port_state = NULL;

    port_state = (mc_port_group_state_list_t  *)mc_malloc(sizeof(mc_port_group_state_list_t ));
    if (port_state != NULL) {
        memset(port_state, 0, sizeof(mc_port_group_state_list_t));
        cs_lst_init(&port_state->entry.src_list, NULL);
        cs_lst_init(&port_state->entry.aged_src_list, NULL);
    }

    return port_state;    

}

void  mc_port_grp_destroy(mc_port_group_state_list_t *port_state)
{    
    if (port_state == NULL) {
        return ;
    }

    mc_src_list_clear(&port_state->entry.src_list);
    mc_src_list_clear(&port_state->entry.aged_src_list );
        
    mc_free(port_state);

    return ;
}


cs_status mc_port_grp_add(mc_group_node_t  *grp_state, mc_port_group_state_list_t *port_state)
{
    mc_port_t  *port = NULL;
    cs_list       *port_state_list = NULL;

    if (( grp_state == NULL) 
        || (port_state == NULL ) ) {
        return CS_E_ERROR;
    }

    port = port_state->entry.port;
    port->grp_count++;        

    port_state_list = &grp_state->entry.port_state_list;
    cs_lst_add(port_state_list, &port_state->node);      

     return CS_E_OK;
}

cs_status mc_port_grp_rm(mc_port_group_state_list_t * port_state)
{
    mc_group_node_t  *grp_state = NULL;
    mc_port_t            *port = NULL;

    if(port_state == NULL) {
        return CS_E_ERROR;
    }
    
    grp_state = port_state->entry.grp_father;
    if (grp_state == NULL) {
        MC_LOG (IROS_LOG_LEVEL_INF ,"\r\n warning: grp_state==NULL \r\n");
        return CS_E_ERROR;;
    }

    MC_DEL_PORT_GRP_DUMP(port_state);

    port = port_state->entry.port;
    if(port->grp_count > 0) {
        port->grp_count--;
    }    
    
    cs_lst_delete(&grp_state->entry.port_state_list , &port_state->node);    

    return CS_E_OK;
     
}

mc_port_group_state_list_t *mc_port_grp_find(mc_port_t *port, mc_group_node_t *grp_state)
{
    cs_list     *port_state_list = NULL;
    mc_port_group_state_list_t    *port_state = NULL;

    if (port == NULL || grp_state == NULL) {
        return NULL;
    }
    
    port_state_list = &grp_state->entry.port_state_list ;

    cs_lst_scan(port_state_list, port_state, mc_port_group_state_list_t *) {
        if(port_state->entry.port == port) {
            return port_state;
        }
    }
    
    return NULL;    
}

cs_boolean  mc_port_grp_exist(mc_port_group_state_list_t *port_state)
{
    if((port_state->entry.filter_mode == SRC_INCLUDE)
        && (port_state->entry.src_list.count == 0)) {
        return FALSE;
    }
    else {
        return TRUE;
    }
     
}


void mc_port_grp_copy(
    mc_port_group_state_list_t  *dst_port_state, 
    mc_port_group_state_list_t  *src_port_state)
{
    mc_src_list_clear(&dst_port_state->entry.src_list);
    mc_src_list_clear(&dst_port_state->entry.aged_src_list);

    dst_port_state->entry.port = src_port_state->entry.port;
    dst_port_state->entry.flag = src_port_state->entry.flag;
    dst_port_state->entry.grp_father = src_port_state->entry.grp_father;
    /* dst_port_state->entry.ver_state = src_port_state->entry.ver_state; */
    dst_port_state->entry.filter_mode = src_port_state->entry.filter_mode;
    dst_port_state->entry.grp_timer = src_port_state->entry.grp_timer;
    
    mc_src_list_copy(&dst_port_state->entry.src_list, &src_port_state->entry.src_list);
    mc_src_list_copy(&dst_port_state->entry.aged_src_list, &src_port_state->entry.aged_src_list);    
    
}

/*
 *  grp entry
 */
 
mc_group_node_t *mc_grp_create()
{
    mc_group_node_t  *grp_state = NULL;
    
    grp_state = (mc_group_node_t  *)mc_malloc(sizeof (mc_group_node_t));
    if(grp_state) {
        memset(grp_state, 0, sizeof(mc_group_node_t));
        cs_lst_init(&grp_state->entry.port_state_list, NULL);
    }
    
    return grp_state;    

}

void  mc_grp_destroy(mc_group_node_t *grp_state)
{    
    cs_list                           * port_state_list = NULL;
    mc_port_group_state_list_t   * port_state = NULL;

    if(grp_state == NULL) {
        return;
    }

    port_state_list = &grp_state->entry.port_state_list;

    for(port_state = (mc_port_group_state_list_t * )cs_lst_first(port_state_list);port_state; )  {
        mc_port_grp_rm (port_state);
        mc_port_grp_destroy ( port_state);
        port_state = (mc_port_group_state_list_t * )cs_lst_first(port_state_list);
    }

    mc_free(grp_state);
    
    return;
}

cs_status  mc_grp_add(cs_hash_tbl *  grp_table, mc_group_node_t *grp_state)
{  
    mc_node_t *mc_node;
    
    if (!grp_table) {
        return CS_E_ERROR;
    }

    cs_hash_add(grp_table , (void *)grp_state);    
    mc_node = grp_state->entry.mc_node;
    mc_node->cur_grp_num++;

    return CS_E_OK;
}

cs_status  mc_grp_rm(cs_hash_tbl *  grp_table,  mc_group_node_t * grp_state)
{   
    mc_node_t *mc_node;

    if (!grp_table) {
        return CS_E_ERROR;
    }
 
    cs_hash_del(grp_table, (void *)grp_state);  
    mc_node = grp_state->entry.mc_node;
    if(mc_node->cur_grp_num) {
        mc_node->cur_grp_num--;
    }
    
    return CS_E_OK;

}

cs_boolean  mc_grp_exist(mc_group_node_t  *grp_state)
{

   if(grp_state && grp_state->entry.port_state_list.count) {
        return TRUE;
   }
 
    return FALSE;
     
}

mc_port_group_state_list_t *mc_port_grp_find_globally(
    mc_node_t *mc_node, 
    mc_port_t  *port,
    ip_type_t *mc_grp)
{
    cs_uint32   key = 0;
    mc_group_state_t   grp_entry;
    cs_hash_tbl      *grp_table = NULL;
    cs_uint16 vid;

    mc_group_node_t *node = NULL;
    mc_port_group_state_list_t *port_grp = NULL;

    if (mc_grp == NULL) {
        return NULL;
    }

    for(vid = 0; vid < MC_MAX_VLAN; vid++) {
        grp_table = mc_node->grp_tbl[vid];
        if(NULL == grp_table) {
            continue;
        }

        memset(&grp_entry, 0, sizeof( mc_group_state_t));
        memcpy(&grp_entry.mc_grp, mc_grp, sizeof(grp_entry.mc_grp));
        grp_entry.vlanid = vid;
        key = (cs_uint32)&grp_entry;
        node = (mc_group_node_t *)cs_hash_find(grp_table, key);

        if(node) {
            port_grp = mc_port_grp_find(port, node);
            if(port_grp) {
                return port_grp;
            }
        }
        
    }   

    return NULL;
    
}

mc_group_node_t *mc_grp_find_globally(mc_node_t *mc_node, ip_type_t *mc_grp)
{
    cs_uint32   key = 0;
    mc_group_state_t   grp_entry;
    cs_hash_tbl      *grp_table = NULL;
    cs_uint16 vid;

    mc_group_node_t *node = NULL;

    if (mc_grp == NULL) {
        return NULL;
    }

    for(vid = 0; vid < MC_MAX_VLAN; vid++) {
        grp_table = mc_node->grp_tbl[vid];
        if(NULL == grp_table) {
            continue;
        }

        memset(&grp_entry, 0, sizeof( mc_group_state_t));
        memcpy(&grp_entry.mc_grp, mc_grp, sizeof(grp_entry.mc_grp));
        grp_entry.vlanid = vid;
        key = (cs_uint32)&grp_entry;
        node = (mc_group_node_t *)cs_hash_find(grp_table, key);

        if(node) {
            break;
        }
    }   

    return node;
    
}


mc_group_node_t *mc_grp_find(mc_node_t *mc_node, cs_uint16 vlanid, ip_type_t *mc_grp)
{
    cs_uint32   key = 0;
    mc_group_state_t   grp_entry;
    cs_hash_tbl      *grp_table = NULL;

    if (mc_grp == NULL) {
        return NULL;
    }

    grp_table = mc_node->grp_tbl[vlanid];
    if(NULL == grp_table) {
        return NULL;
    }
    
    memset(&grp_entry, 0, sizeof( mc_group_state_t));
    memcpy(&grp_entry.mc_grp, mc_grp, sizeof(grp_entry.mc_grp));
    grp_entry.vlanid = vlanid;
    key = (cs_uint32)&grp_entry;

    return (mc_group_node_t *)cs_hash_find(grp_table, key);
    
}


/*
 * grp table 
 */

cs_uint32 mc_grp_key_gen (void *node)
{
    mc_group_node_t  *grp_state = NULL;
    cs_uint32   value = 0; 
    ip_type_t   *mc_grp = NULL;    

    if (node == NULL) {
        return 0;
    }

    grp_state = (mc_group_node_t *)node;
    mc_grp = &grp_state->entry.mc_grp;    

    if (grp_state->entry.mc_grp.type == MC_IPV4) {
        value = mc_grp->addr.ipv4 % MC_GROUP_HASH_LEN;
    }
    else {
        cs_uint8 *p_data =(cs_uint8 *)&mc_grp->addr;
        cs_uint32    i=0;
        
        for (i=0; i < MC_IPV6_IP_LEN; i++) {
            value += *(p_data+i);
        }
        
        value %= MC_GROUP_HASH_LEN ;
    }
   
    return   value;
}

cs_int32 mc_grp_compare_func(void *pNode , cs_uint32 key)
{
    mc_group_node_t  *grp_state = (mc_group_node_t *)pNode;
    mc_group_state_t       *grp_entry = (mc_group_state_t  *)key;

#if 0
    if(grp_state->entry.vlanid != grp_entry->vlanid) {
        return 1;
    }
#endif

    if(mc_ip_cmp(&grp_state->entry.mc_grp, &grp_entry->mc_grp)) {
        return 1;
    }
    
    return 0;
    
}

cs_hash_tbl  *mc_grp_table_create()
{   
    cs_hash_tbl *grp_table = NULL;
    grp_table = cs_hash_create(MC_GROUP_HASH_LEN, mc_grp_key_gen , mc_grp_compare_func);
    
    return grp_table;
    
}

cs_status  mc_grp_table_destroy(cs_hash_tbl   *grp_table)
{ 
    if (grp_table == NULL) {
        return CS_E_ERROR;
    }
    
    cs_hash_destroy(grp_table);    
    
    return CS_E_OK;
    
}

cs_boolean mc_grp_table_exist(cs_hash_tbl *grp_table)
{
    if(cs_hash_count(grp_table)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}


