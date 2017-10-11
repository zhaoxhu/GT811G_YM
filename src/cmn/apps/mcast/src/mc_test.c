#include "mc_core.h"
#include "mc_fdb.h"
#include "mc_grp.h"
#include "mc_src.h"
#include "mc_mgmt.h"
#include "mc_adapter.h"
#include "mc_test.h"

cs_uint8 g_mc_dump = 1;

static cs_uint8 mc_out_buf[MC_RECORD_BUF_LEN];

/* 
 * port stats 
 */
 
void mc_node_port_stats_show(mc_node_t *node, cs_port_id_t port_id)
{
    mc_port_t *port = NULL;
    port = mc_port_find(node, port_id);

    if(port) {
        cs_printf("port %d stats:\n", port_id);
        
        cs_printf("rx igmp query: \t\t%d\n", port->in_igmp_query);
        cs_printf("rx igmp v1 report: \t%d\n", port->in_igmpv1_report);
        cs_printf("rx igmp v2 report: \t%d\n", port->in_igmpv2_report);
        cs_printf("rx igmp v2 leave: \t%d\n", port->in_igmpv2_leave);
        cs_printf("rx igmp v3 report: \t%d\n", port->in_igmpv3_report);
        cs_printf("rx mld query: \t\t%d\n", port->in_mld_query);
        cs_printf("rx mld v1 report: \t%d\n", port->in_mldv1_report);
        cs_printf("rx mld v2 report: \t%d\n", port->in_mldv2_report);
        cs_printf("rx mld v1 done: \t%d\n", port->in_mldv1_done);

        cs_printf("tx igmp query: \t\t%d\n", port->out_igmp_query);
        cs_printf("tx igmp v1 report: \t%d\n", port->out_igmpv1_report);
        cs_printf("tx igmp v2 report: \t%d\n", port->out_igmpv2_report);
        cs_printf("tx igmp v2 leave: \t%d\n", port->out_igmpv2_leave);
        cs_printf("tx igmp v3 report: \t%d\n", port->out_igmpv3_report);
        cs_printf("tx mld query: \t\t%d\n", port->out_mld_query);
        cs_printf("tx mld v1 report: \t%d\n", port->out_mldv1_report);
        cs_printf("tx mld v2 report: \t%d\n", port->out_mldv2_report);
        cs_printf("tx mld v1 done: \t%d\n", port->out_mldv1_done);
    }
    
}

void mc_node_port_stats_clr(mc_node_t *node, cs_port_id_t port_id)
{
    mc_port_t *port = NULL;
    
    port = mc_port_find(node, port_id);

    if(port) {
        port->in_igmp_query = 0;
        port->in_igmpv1_report = 0;
        port->in_igmpv2_report = 0;
        port->in_igmpv2_leave = 0;
        port->in_igmpv3_report = 0;
        port->in_mld_query = 0;
        port->in_mldv1_report = 0;
        port->in_mldv2_report = 0;
        port->in_mldv1_done = 0;

        port->out_igmp_query = 0;
        port->out_igmpv1_report = 0;
        port->out_igmpv2_report = 0;
        port->out_igmpv2_leave = 0;
        port->out_igmpv3_report = 0;
        port->out_mld_query = 0;
        port->out_mldv1_report = 0;
        port->out_mldv2_report = 0;
        port->out_mldv1_done = 0;
    }
    
}

/*
 * print utils
 */
 
cs_uint32 mc_print_ipv4_addr(cs_uint32 addr, cs_uint8 *out_buffer)
{
    return sprintf (out_buffer,  "%d.%d.%d.%d",
                                    (addr & 0xff000000) >> 24,
                                    (addr & 0xff0000) >> 16,
                                    (addr & 0xff00) >> 8,
                                    (addr & 0xff) );
}

cs_uint32 mc_print_ipv6_addr(ipv6_addr_t *addr, cs_uint8 *out_buf)
{
    cs_uint16 addr16[8];    
    cs_uint32 i = 0;

    /* split ipv6 fields */
    for (i = 0; i <8; i += 2) {
        addr16[i] = (addr->mc_addr32[i/2] >>16) & 0xffff;
        addr16[i+1] = addr->mc_addr32[i/2] & 0xffff;
    }

    return sprintf(out_buf, "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x", 
                    addr16[0], addr16[1], addr16[2], addr16[3],
                    addr16[4], addr16[5], addr16[6], addr16[7]);
        
}

cs_uint32 mc_ip_sprintf(ip_type_t * ip, cs_uint8 *out_buffer)
{
    cs_uint32 len = 0;
    
    if(ip->type == MC_IPV4) {
        len = mc_print_ipv4_addr(ip->addr.ipv4, out_buffer);
    }
    else {
        len = mc_print_ipv6_addr(&ip->addr.ipv6, out_buffer);
    }

    return len;
}

void mc_ip_print(ip_type_t *ip)
{
    cs_uint8  buffer[MC_IP_BUF_LEN];

    memset(buffer, 0, MC_IP_BUF_LEN);
    mc_ip_sprintf(ip, buffer);
    cs_printf("%s", buffer);

}

void  mc_src_list_print(cs_list * src_list)
{
    mc_source_record_list_t *src = NULL;
    
    cs_lst_scan(src_list, src, mc_source_record_list_t *) {
        mc_ip_print(&src->entry.src_ip);
        if(src->entry.timer.active_flag) {
            cs_printf ("(%ds)",  src->entry.timer.age);
        }
        cs_printf("\n");
    }

}

/* 
 * record dump
 */
 
cs_uint32 mc_grp_record_show(mc_group_record_t *record, cs_uint8 *output)
{    
    cs_uint32 cnt = 0;
        
    cnt = sprintf(output, "type = %d, src_num = %d, grp = ", 
        record->record_type, cs_lst_count(&record->src_list));    
    cnt += mc_ip_sprintf(&record->mc_grp, &output[cnt]); 
    
    if(cs_lst_count(&record->src_list)) {
        mc_source_record_list_t *src = NULL;
        cs_list *src_list = &record->src_list;
        cs_uint8 src_num = 0;

        cnt += sprintf(&output[cnt], "\nsrc list:");
        
        cs_lst_scan(src_list, src, mc_source_record_list_t *) {
            cnt += sprintf(&output[cnt], "\n%d: ", src_num++);
            cnt += mc_ip_sprintf(&src->entry.src_ip, &output[cnt]);
        }
    }

    cnt += sprintf(&output[cnt], "\n\n\n");

    return cnt;
}

void mc_handle_record_show(mc_group_record_t *record)
{
    cs_uint32 cnt = 0;
    
    memset(mc_out_buf, 0, MC_RECORD_BUF_LEN);
    
    cnt = sprintf(mc_out_buf, "\nhandle record: vid = %d, ", record->vlanid);
    
    mc_grp_record_show(record, &mc_out_buf[cnt]);

    MC_LOG(IROS_LOG_LEVEL_INF, "%s", mc_out_buf);
}

void mc_report_msg_show(mc_object_t *msg)
{
    cs_list * record_list = &msg->record_list;
    mc_group_record_list_t *record = NULL;
    cs_uint8 record_num = 0;

    cs_uint32 cnt = 0;

    memset(mc_out_buf, 0, MC_RECORD_BUF_LEN);
    
    cnt += sprintf(&mc_out_buf[cnt], "recv report from port %d:\n", msg->port->portid);
    cnt += sprintf(&mc_out_buf[cnt], "vlan = %d, pdu type: %d, len = %d, record_num = %d\n", 
        msg->vlanid, msg->pdu_type, msg->len, cs_lst_count(&msg->record_list));

    MC_LOG(IROS_LOG_LEVEL_INF, "%s", mc_out_buf);    
     
    cs_lst_scan(record_list , record , mc_group_record_list_t *) {
        cnt = 0;
        memset(mc_out_buf, 0, MC_RECORD_BUF_LEN);
    
        cnt += sprintf(&mc_out_buf[cnt], "%d: ", record_num++);
        cnt += mc_grp_record_show(&record->entry, &mc_out_buf[cnt]);

        cnt += sprintf(&mc_out_buf[cnt], "\n\n");

        MC_LOG(IROS_LOG_LEVEL_INF, "%s", mc_out_buf);
    }
    
}

void mc_query_msg_show(mc_object_t *msg)
{
    cs_list * record_list = &msg->record_list;
    cs_uint32 cnt = 0;

    memset(mc_out_buf, 0, MC_RECORD_BUF_LEN);
    
    cnt += sprintf(&mc_out_buf[cnt], "recv query from port %d:\n", msg->port->portid);
    cnt += sprintf(&mc_out_buf[cnt], "vlan = %d, pdu type: %d, len = %d\n", 
        msg->vlanid, msg->pdu_type, msg->len);

    if(cs_lst_count(record_list) == 1) {
        mc_group_record_list_t *record = (mc_group_record_list_t *)cs_lst_first(record_list);

        cnt += sprintf(&mc_out_buf[cnt], "query_type = %d\n", record->entry.pdu_type);
        cnt += mc_grp_record_show(&record->entry, &mc_out_buf[cnt]);
    }

    cnt += sprintf(&mc_out_buf[cnt], "\n\n\n");

    MC_LOG(IROS_LOG_LEVEL_INF, "%s", mc_out_buf);
}

cs_uint32 mc_port_grp_show(mc_port_group_state_list_t *port_grp, cs_uint8 *output)
{
    mc_port_t *port = port_grp->entry.port;

    cs_uint32 cnt = 0;
    
    cnt += sprintf(&output[cnt], "port %d:", port->portid);
    cnt += sprintf(&output[cnt], "vlan = %d, grp = ", port_grp->entry.grp_father->entry.vlanid);
    cnt += mc_ip_sprintf(&port_grp->entry.grp_father->entry.mc_grp, &output[cnt]);
    cnt += sprintf(&output[cnt], "\nfilter mode = %d, grp timer = %d, %s\n", port_grp->entry.filter_mode, 
                                                        port_grp->entry.grp_timer.age, 
                                                        port_grp->entry.grp_timer.static_flag ? "static" : "dynamic");

    return cnt;
}

void mc_new_grp_dump(mc_group_record_t *record)
{
    cs_uint32 cnt = 0;

    memset(mc_out_buf, 0, MC_RECORD_BUF_LEN);
    
    cnt += sprintf(&mc_out_buf[cnt], "create new group: ");
    cnt += mc_grp_record_show(record, &mc_out_buf[cnt]);

    MC_LOG(IROS_LOG_LEVEL_INF, "%s", mc_out_buf);
}

void mc_add_port_grp_dump(mc_port_t *port, mc_group_record_t *record)
{
    cs_uint32 cnt = 0;

    memset(mc_out_buf, 0, MC_RECORD_BUF_LEN);
    
    cnt += sprintf(&mc_out_buf[cnt], "add group to port %d: ", port->portid);
    cnt += mc_grp_record_show(record, &mc_out_buf[cnt]);

    MC_LOG(IROS_LOG_LEVEL_INF, "%s", mc_out_buf);
}

void mc_update_port_grp_dump(
    mc_port_group_state_list_t *port_grp, 
    mc_group_record_list_t *record)
{
    cs_uint32 cnt = 0;

    memset(mc_out_buf, 0, MC_RECORD_BUF_LEN);
    
    cnt += sprintf(&mc_out_buf[cnt], "update port grp: \n");

    cnt += sprintf(&mc_out_buf[cnt], "old port group info:\n");
    cnt += mc_port_grp_show(port_grp, &mc_out_buf[cnt]);

    MC_LOG(IROS_LOG_LEVEL_INF, "%s", mc_out_buf);

    cnt = 0;
    memset(mc_out_buf, 0, MC_RECORD_BUF_LEN);
    
    cnt += sprintf(&mc_out_buf[cnt], "record info: ");
    
    mc_grp_record_show(&record->entry, &mc_out_buf[cnt]);

    MC_LOG(IROS_LOG_LEVEL_INF, "%s", mc_out_buf);
}

void mc_del_port_grp_dump(mc_port_group_state_list_t *port_grp)
{
    cs_uint32 cnt = 0;

    memset(mc_out_buf, 0, MC_RECORD_BUF_LEN);
    
    cnt += sprintf(&mc_out_buf[cnt], "del port group:\n");
    cnt += mc_port_grp_show(port_grp, &mc_out_buf[cnt]);

    MC_LOG(IROS_LOG_LEVEL_INF, "%s", mc_out_buf);
}


/*
 * MFDB
 */


void mc_port_mc_fdb_print (mc_port_group_state_list_t *port_state)
{
    mc_group_node_t   *grp_state = NULL;
    mc_port_t  *port = NULL;

    cs_list *src_list = NULL;
    cs_list *aged_src_list = NULL;
    
    cs_uint8    fdb_buf[MC_FDB_BUF_LEN];
    cs_uint32  count = 0;

    cs_uint8    ip_buf[MC_IP_BUF_LEN];
    cs_uint32  ip_len = MC_IP_BUF_LEN;    

    memset(fdb_buf, 0, MC_FDB_BUF_LEN);
    memset(ip_buf, 0, ip_len);
   
    grp_state = port_state->entry.grp_father;    
    port = (mc_port_t *)port_state->entry.port;

    /* port */
    count += sprintf(fdb_buf, "port = %02d, ", port->portid);
    count += sprintf(&fdb_buf[count], "vlan = %04d, ", grp_state->entry.vlanid);

    /* group */
    ip_len = mc_ip_sprintf(&grp_state->entry.mc_grp, ip_buf);
    sprintf(ip_buf + ip_len, "(%ds)", port_state->entry.grp_timer.age);
    count += sprintf(&fdb_buf[count], "%45s", ip_buf);
    count += sprintf(&fdb_buf[count], ", %2s", port_state->entry.filter_mode ? "EX" : "IN");

    cs_printf("%s\n", fdb_buf);

    /* src list */
    src_list = &port_state->entry.src_list;
    if(cs_lst_count(src_list)) {
        cs_printf("src list: \n");
        mc_src_list_print(src_list);
    }

    aged_src_list = &port_state->entry.aged_src_list;
    if(cs_lst_count(aged_src_list)) {
        cs_printf("aged src list: \n");
        mc_src_list_print(aged_src_list);
    }

    cs_printf("\n");
}


 
void mc_port_fdb_show(mc_node_t  *mc_node, mc_port_t *port)
{
    cs_hash_tbl  *grp_table = NULL;
    cs_int32       hash_index = 0;

    cs_list         *grp_list = NULL;
    mc_group_node_t   *grp_state = NULL;
    mc_port_group_state_list_t *port_state = NULL;

    cs_uint16 vid;
    cs_uint16 max_vid = 1; 

    if(MC_DISABLE == mc_node->mode) {
        return ;
    } 

    if(mc_node->vlan_lrn_mode == MC_IVL) {
        max_vid = MC_MAX_VLAN;
    }
    
    for(vid = 0; vid < max_vid; vid++) {        
        grp_table = mc_node->grp_tbl[vid];
        if (grp_table == NULL ) {
            continue;
        }
        
        cs_scan_hash_bucket(grp_table, hash_index) {
            grp_list = &grp_table->hash_list[hash_index];
            cs_scan_hash_record(grp_list, grp_state, mc_group_node_t *) {
                port_state = mc_port_grp_find(port, grp_state);
                if(port_state) {
                    mc_port_mc_fdb_print(port_state);
                }                    
                                
            }
        }
        
    }
  
}

void mc_vlan_fdb_show(mc_node_t *mc_node, cs_uint16 vid)
{
    cs_hash_tbl  *grp_table = NULL;
    cs_int32       hash_index = 0;

    cs_list         *grp_list = NULL;
    mc_group_node_t   *grp_state = NULL;

    cs_list *port_state_list = NULL;
    mc_port_group_state_list_t *port_state = NULL;
    
    if(MC_DISABLE == mc_node->mode) {
        return ;
    } 

    if(vid && MC_SVL == mc_node->vlan_lrn_mode) {
        return ;
    }

    if(NULL == mc_node->grp_tbl[vid]) {
        return ;
    }
       
    grp_table = mc_node->grp_tbl[vid];
    if (grp_table == NULL ) {
        return;
    }
        
    cs_scan_hash_bucket(grp_table, hash_index) {
        grp_list = &grp_table->hash_list[hash_index];
        cs_scan_hash_record(grp_list, grp_state, mc_group_node_t *) {
            port_state_list = &grp_state->entry.port_state_list;
            cs_lst_scan(port_state_list, port_state, mc_port_group_state_list_t *) {
                mc_port_mc_fdb_print(port_state);
            }                    
                            
        }
    }
       
  
}

void mc_fdb_show(mc_node_t *mc_node)
{
    cs_uint16 vid;
    cs_uint16 max_vid = 1;

    if(MC_DISABLE == mc_node->mode) {
        return ;
    } 

    if(mc_node->vlan_lrn_mode == MC_IVL) {
        max_vid = MC_MAX_VLAN;
    }

    cs_printf("device %d grp table: \n", mc_node->device);
    
    for(vid = 0; vid < max_vid; vid++) {    
        if(mc_node->grp_tbl[vid]) {
            mc_vlan_fdb_show(mc_node, vid);        
        }
    }
  
}

