#include "mc_ctcctrl.h"
#include "mc_core.h"
#include "mc_mgmt.h"
#include "mc_devm.h"
#include "mc_fdb.h"
#include "mc_grp.h"
#include "mc_src.h"
#include "mc_util.h"
#include "mc_test.h"
#include "mc_adapter.h"

typedef struct {
    cs_uint16 userId;
    cs_uint16 vlanId;
    cs_uint8 mc_da[6];
} __attribute__((packed)) oam_ctc_onu_mc_control_vlan_mac_t;


typedef struct {
    cs_uint16 userId;
    cs_uint8 mc_da[6];
    cs_uint8 ip_addr[16];
} __attribute__((packed)) oam_ctc_onu_mc_control_mac_ip_t;


typedef struct {
    cs_uint16 userId;
    cs_uint16 vlanId;
    cs_uint8 ip_addr[16];
} __attribute__((packed)) oam_ctc_onu_mc_control_vlan_ip_t;

cs_uint8 g_mc_ctrl_type = MC_CTRL_GDA_MAC;

cs_uint32 g_mc_fast_leave_state = 1;

void mc_ctrl_init()
{
    g_mc_ctrl_type = MC_CTRL_GDA_MAC;
}

cs_uint32 mc_get_fast_leave_state()
{
    return g_mc_fast_leave_state;
}

cs_status mc_set_fast_leave_state(cs_uint32 state)
{
    mc_node_t *node = MC_GET_NODE(0);
    cs_uint8 port_num = mc_device_port_num_get(0);
    mc_leave_mode_t leave_mode;

    cs_uint8 port_id;
    if(state == 1) {    /* non-fast-leave */
        leave_mode = PORT_NON_FAST_LEAVE;
    }
    else {
        leave_mode = PORT_FAST_LEAVE;
    }

    for(port_id = 1; port_id < port_num; port_id++) {
        mc_node_set_leave_mode(node, port_id, leave_mode);
    }

    g_mc_fast_leave_state = state;

    return CS_E_OK;
}

cs_status mc_set_mc_fwd_mode(mc_ctc_ctrl_type_t mode)
{
    mc_node_t *node = MC_GET_NODE(0);

    MC_LOG(IROS_LOG_LEVEL_INF, "set mc_fwd_mode(%d) to %d\n", g_mc_ctrl_type, mode);

    if(mode >= MC_CTRL_TYPE_UNDEF) {
        return CS_E_PARAM;
    }

    if(g_mc_ctrl_type == mode) {
        return CS_E_OK;
    }  

    if(node) {
        mc_vlan_learn_t  vlan_lrn_mode = MC_IVL;
        mc_fwd_engine_t fwd_engine = MC_ENGINE_L2;                     
        
        g_mc_ctrl_type = mode;

        switch(mode) {
            case MC_CTRL_GDA_MAC:
                vlan_lrn_mode = MC_SVL;
                fwd_engine = MC_ENGINE_L2;
                break;

            case MC_CTRL_GDA_MAC_VLAN:
                vlan_lrn_mode = MC_IVL;
                fwd_engine = MC_ENGINE_L2;
                break;

#if 0
            case MC_CTRL_GDA_MAC_SA_IP:
                vlan_lrn_mode = MC_SVL;
                fwd_engine = MC_ENGINE_GDA_MAC_SIP;
                break;

            case MC_CTRL_GDA_IPV4_VLAN:
                vlan_lrn_mode = MC_IVL;
                fwd_engine = MC_ENGINE_IP;
                break;

            case MC_CTRL_GDA_IPV6_VLAN:
                vlan_lrn_mode = MC_SVL;
                fwd_engine = MC_ENGINE_IP;
                break;
#endif
            default:
                return CS_E_NOT_SUPPORT;
        }            

        return mc_node_set_vlan_lrn(node, vlan_lrn_mode) ||
            mc_node_set_fwd_engine(node, fwd_engine);
    }

    return CS_E_ERROR;
}

mc_ctc_ctrl_type_t mc_get_mc_fwd_mode()
{
   return g_mc_ctrl_type;
}

cs_status mc_ctc_ctrl_add_entry(cs_port_id_t port_id, mc_group_record_list_t *record)
{
    mc_node_t *node = NULL;
    mc_port_t  *port = NULL;

    node = MC_GET_NODE(0);

    MC_HANDLE_RECORD_DUMP(&record->entry);

    if(MC_MANUAL != node->mode) {
        return CS_E_NOT_SUPPORT;
    }
    
    port = mc_port_find(node, port_id);
    if (NULL == port) {
        return CS_E_NOT_FOUND;
    }
    
    return mc_grp_record_handle(node, port, record);
}

cs_status mc_ctc_ctrl_del_entry(cs_port_id_t port_id, ip_type_t *mc_grp, cs_uint16 vid)
{
    mc_node_t *node = NULL;
    mc_port_t  *port = NULL;

    mc_group_node_t * grp = NULL;
    mc_port_group_state_list_t *port_grp = NULL;

    cs_status ret;

    node = MC_GET_NODE(0);

    if(MC_MANUAL != node->mode) {
        return CS_E_NOT_SUPPORT;
    }
    
    port = mc_port_find(node, port_id);
    if (NULL == port) {
        return CS_E_NOT_FOUND;
    }

    grp = mc_grp_find(node, vid, mc_grp);
    if(NULL == grp) {
        MC_LOG(IROS_LOG_LEVEL_INF, "cannot find the grp!\n");
        return CS_E_NOT_FOUND;
    }

    port_grp = mc_port_grp_find(port, grp);

    ret = mc_port_fdb_entry_del(port_grp);

    if(CS_E_OK == ret && !mc_grp_exist(grp)) {
        cs_hash_tbl *grp_table = node->grp_tbl[vid];
        
        mc_grp_rm(grp_table, grp);
        mc_grp_destroy(grp);
    }

    return ret;
}

cs_status mc_ctc_ctrl_add_mac_vlan_entry(cs_port_id_t port_id, cs_uint8 *da, cs_uint16 vid)
{        
    mc_group_record_list_t *record = NULL;

    cs_status ret;

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_ctc_ctrl_add_mac_vlan_entry], port = %d\n", port_id);
    
    record = mc_record_create();
    if(NULL == record) {
        MC_LOG(IROS_LOG_LEVEL_CRI,"creat record fail!\n");
        return CS_E_RESOURCE;
    }

    record->entry.flag = MC_STATIC_GROUP_FLAG;
    record->entry.pdu_type = IGMP_V2_REPORT;
    record->entry.record_type = MODE_IS_EXCLUDE;
    record->entry.vlanid = vid;

    mc_mac_to_ip(da, &record->entry.mc_grp);
    
    ret = mc_ctc_ctrl_add_entry(port_id, record);

    mc_record_destroy(record);

    return ret;
}

cs_status mc_ctc_ctrl_del_mac_vlan_entry(cs_port_id_t port_id, cs_uint8 *da, cs_uint16 vid)
{      
    ip_type_t  mc_grp;    

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_ctc_ctrl_del_mac_vlan_entry], port = %d\n", port_id);
  
    mc_mac_to_ip(da, &mc_grp);
    
    mc_ctc_ctrl_del_entry(port_id, &mc_grp, vid);

    return CS_E_OK;
}

cs_status mc_ctc_ctrl_add_da_sip_entry(
    cs_port_id_t port_id, 
    cs_uint8 *da, 
    mc_ip_type_t ip_type, 
    cs_uint8 *sip)
{
    mc_group_record_list_t *record = NULL;
    mc_source_record_list_t *src_record = NULL;

    cs_status ret;

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_ctc_ctrl_add_da_sip_entry], port = %d\n", port_id);
    
    record = mc_record_create();
    if(NULL == record) {
        MC_LOG(IROS_LOG_LEVEL_CRI,"creat record fail!\n");
        return CS_E_RESOURCE;
    }

    record->entry.flag = MC_STATIC_GROUP_FLAG;
    record->entry.pdu_type = IGMP_V3_REPORT;
    record->entry.record_type = MODE_IS_INCLUDE;
    record->entry.vlanid = 0;

    mc_mac_to_ip(da, &record->entry.mc_grp);

    src_record = (mc_source_record_list_t *)mc_malloc(sizeof(mc_source_record_list_t));
    if(NULL == record) {
        MC_LOG(IROS_LOG_LEVEL_CRI,"creat src_record fail!\n");
        mc_record_destroy(record);
        return CS_E_RESOURCE;
    }

    mc_data_to_ip(ip_type, sip, &src_record->entry.src_ip);
    mc_src_add(&record->entry.src_list, src_record);

    ret = mc_ctc_ctrl_add_entry(port_id, record);

    mc_record_destroy(record);

    return ret;
}

cs_status mc_ctc_ctrl_del_da_sip_entry(
    cs_port_id_t port_id, 
    cs_uint8 *da, 
    mc_ip_type_t ip_type, 
    cs_uint8 *sip)
{      
    ip_type_t  mc_grp;    

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_ctc_ctrl_del_da_sip_entry], port = %d\n", port_id);
  
    mc_mac_to_ip(da, &mc_grp);
    
    return mc_ctc_ctrl_del_entry(port_id, &mc_grp, 0);
}

cs_status mc_ctc_ctrl_add_ip_vlan_entry(
    cs_port_id_t port_id, 
    cs_uint8 *dip, 
    mc_ip_type_t ip_type,
    cs_uint16 vid)
{        
    mc_group_record_list_t *record = NULL;

    cs_status ret;

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_ctc_ctrl_add_ip_vlan_entry], port = %d\n", port_id);
    
    record = mc_record_create();
    if(NULL == record) {
        MC_LOG(IROS_LOG_LEVEL_CRI,"creat record fail!\n");
        return CS_E_RESOURCE;
    }

    record->entry.flag = MC_STATIC_GROUP_FLAG;    
    record->entry.pdu_type = ip_type ? MLD_V1_REPORT : IGMP_V2_REPORT;
    record->entry.record_type = MODE_IS_EXCLUDE;
    record->entry.vlanid = vid;

    mc_data_to_ip(ip_type, dip, &record->entry.mc_grp);
    
    ret = mc_ctc_ctrl_add_entry(port_id, record);

    mc_record_destroy(record);

    return ret;
}

cs_status mc_ctc_ctrl_del_ip_vlan_entry(
    cs_port_id_t port_id, 
    cs_uint8 *dip, 
    mc_ip_type_t ip_type,
    cs_uint16 vid)
{      
    ip_type_t  mc_grp;    

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_ctc_ctrl_del_ip_vlan_entry], port = %d\n", port_id);
  
    mc_data_to_ip(ip_type, dip, &mc_grp);
    
    return mc_ctc_ctrl_del_entry(port_id, &mc_grp, vid);
}

cs_status mc_ctc_ctrl_clr_mfdb()
{
    mc_node_t *node = MC_GET_NODE(0);

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_ctc_ctrl_clr_mfdb]\n");

    if(node) {
        mc_fdb_flush(node);
        return CS_E_OK;
    }

    return CS_E_ERROR;
}

cs_uint8 *mc_ctc_ctrl_get_entry(
    ip_type_t *mc_grp,
    mc_port_group_state_t *port_grp, 
    cs_uint16 vid, 
    cs_uint8 *output)
{
    mc_port_t  *port = (mc_port_t *)port_grp->port;
    
    cs_port_id_t port_id = port->portid;
    
    switch(g_mc_ctrl_type) {
        case MC_CTRL_GDA_MAC:
        case MC_CTRL_GDA_MAC_VLAN:
            {
                oam_ctc_onu_mc_control_vlan_mac_t *vlan_mac_ptr = 
                    (oam_ctc_onu_mc_control_vlan_mac_t *)output;

                 vlan_mac_ptr->userId = port_id;
                 vlan_mac_ptr->vlanId = vid;
                 mc_ip_to_mac(mc_grp, vlan_mac_ptr->mc_da);

                 output += sizeof(oam_ctc_onu_mc_control_vlan_mac_t);
            }
            break;

        case MC_CTRL_GDA_MAC_SA_IP:
            {
                oam_ctc_onu_mc_control_mac_ip_t *mac_ip_ptr = 
                    (oam_ctc_onu_mc_control_mac_ip_t *)output;
                cs_list *src_list;
                
                memset(mac_ip_ptr, 0, sizeof(oam_ctc_onu_mc_control_vlan_ip_t));

                mac_ip_ptr->userId = port_id;

                mc_ip_to_mac(mc_grp, mac_ip_ptr->mc_da);

                /* src ip */
                src_list = &port_grp->src_list;
                if(cs_lst_count(src_list) == 1) {
                    mc_source_record_list_t *src = (mc_source_record_list_t *)cs_lst_first(src_list);
                    if(MC_IPV4 == src->entry.src_ip.type) {
                        cs_uint32 ipv4 = htonl(src->entry.src_ip.addr.ipv4);   
                        memcpy(&mac_ip_ptr->ip_addr[12], &ipv4, 4);
                    }
                    else {
                        memcpy(&mac_ip_ptr->ip_addr, src->entry.src_ip.addr.ipv6.mc_addr8, 16);
                    }
                }
                

                output += sizeof(oam_ctc_onu_mc_control_mac_ip_t);
            }
            break;

        case MC_CTRL_GDA_IPV4_VLAN:
        case MC_CTRL_GDA_IPV6_VLAN:
            {
                oam_ctc_onu_mc_control_vlan_ip_t *vlan_ip_ptr = 
                    (oam_ctc_onu_mc_control_vlan_ip_t *)output;

                memset(vlan_ip_ptr, 0, sizeof(oam_ctc_onu_mc_control_vlan_ip_t));
                
                vlan_ip_ptr->userId = port_id;
                vlan_ip_ptr->vlanId = vid;

                if(MC_IPV4 == mc_grp->type) {
                    cs_uint32 ipv4 = htonl(mc_grp->addr.ipv4);   
                    memcpy(&vlan_ip_ptr->ip_addr[12], &ipv4, 4);
                }
                else {
                    memcpy(&vlan_ip_ptr->ip_addr, mc_grp->addr.ipv6.mc_addr8, 16);
                }

                output += sizeof(oam_ctc_onu_mc_control_vlan_ip_t);
            }
            break;

        default:
            break;
    }

    return output;
}

cs_status mc_ctc_ctrl_get_entries(cs_uint8 *output, cs_uint16 *count)
{
    mc_node_t  *mc_node = NULL;
    cs_int32      hash_index = 0;
    cs_hash_tbl  *grp_tbl = NULL;
    cs_list          *grp_list = NULL;
    mc_group_node_t   *grp_state = NULL;

    cs_list                          *port_state_list = NULL;
    mc_port_group_state_list_t  *port_state = NULL;
    
    cs_uint16 vid;
    cs_uint16 max_vid = 1;

    *count = 0;        

    mc_node = MC_GET_NODE(0);

    if(MC_IVL == mc_node->vlan_lrn_mode) {
        max_vid = MC_MAX_VLAN;
    }
    
    for(vid = 0; vid < max_vid; vid++) {
        grp_tbl = mc_node->grp_tbl[vid];        

        if(!grp_tbl) {
            continue ;
        }

        /* go thru grp table */
        cs_scan_hash_bucket(grp_tbl , hash_index) {
            grp_list = &grp_tbl->hash_list[hash_index];

            cs_scan_hash_record(grp_list, grp_state, mc_group_node_t *) {                 

                port_state_list = &grp_state->entry.port_state_list;

                /* go thru port list */
                cs_lst_scan(port_state_list, port_state, mc_port_group_state_list_t *) {
                    output = mc_ctc_ctrl_get_entry(&grp_state->entry.mc_grp, 
                        &port_state->entry, vid, output);
                    (*count)++;
                }

            }
        }
                
    }

    return CS_E_OK;
}

