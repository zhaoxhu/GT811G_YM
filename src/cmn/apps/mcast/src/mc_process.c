#include "mc_core.h"
#include "mc_parser.h"
#include "mc_adapter.h"
#include "mc_mgmt.h"
#ifdef HAVE_GWD_MC_TVM
#include "gwd_mc_process.h"
#endif
#ifdef HAVE_QINQ
#include "gwd_qinq.h"
#endif

void mc_pkt_proc(cs_pkt_t *pkt, mc_node_t *node)
{    
    cs_status ret = CS_E_OK;
    cs_uint8   pkt_chg = 0;

	#ifdef HAVE_QINQ
	pkt_qinq_vlan_translate(pkt);
	#endif
	
	#ifdef HAVE_GWD_MC_TVM
	gwd_mc_tvm_process(pkt, node);
	#endif
	
    ret = mc_pkt_object_build(node, pkt, &node->message);
    if(ret) {
        return ;
    }

    if(node->ing_filter) {
        mc_fwd_action_t fwd_action = node->ing_filter(pkt, node->message.pdu_type, &pkt_chg);
        if(MC_PKT_DROP == fwd_action) {
            return;
        }

        if(pkt_chg) {
            mc_pkt_obj_update(pkt, &node->message);
        }

         if(MC_PKT_BYPASS == fwd_action) {
            mc_port_t *port = node->message.port;
            
            if(mc_is_uplink_port(node, port->portid)) {
                mc_flood_msg(node, &node->message);
            }
            else {
                mc_fwd_msg(node->up_port, node->message.pdu_type, 
                    node->message.frame, node->message.len);
            }
            
            return;
        }
    }
    
    switch(node->message.pdu_type) { 
        case IGMP_V1_REPORT:
        case IGMP_V2_REPORT:
        case IGMP_V3_REPORT:
        case IGMP_V2_LEAVE:
        case MLD_V1_REPORT:
        case MLD_V2_REPORT:
        case MLD_V1_DONE: 
            mc_report_proc(node, &node->message);
            break;

        case IGMP_QUERY:
        case MLD_QUERY:
            mc_query_proc(node, &node->message);
            break;

        default:
            break;
    }

    return ;

}

cs_status mc_fwd_msg(mc_port_t *port, cs_uint8 pdu_type, cs_uint8 *frame, cs_uint16 len)
{
    mc_node_t *node = port->mc_node;
    
    if(node->eg_filter) {
        if(node->eg_filter(port->portid, pdu_type, frame, &len)) {
            return CS_E_ERROR;
        }
    }
    
    /* tx port stats */
    switch (pdu_type) { 
        case    IGMP_V1_REPORT:
            port->out_igmpv1_report ++;
            break;
            
        case    IGMP_V2_REPORT:
            port->out_igmpv2_report ++;
            break;
            
        case    IGMP_V3_REPORT:
            port->out_igmpv3_report ++;
            break;
            
        
        case    IGMP_V2_LEAVE:
            port->out_igmpv2_leave ++;
            break;
            
        case    MLD_V1_REPORT:
            port->out_mldv1_report ++;
            break;

        case    MLD_V2_REPORT:
            port->out_mldv2_report++;
            break;
            
        case    MLD_V1_DONE: 
            port->out_mldv1_done ++;
            break;
            
        case    IGMP_QUERY:
            port->out_igmp_query++;
            break;
            
        case    MLD_QUERY:
            port->out_mld_query++;
            break;

        default:
            break;
    }
   
    return mc_frame_send(node->device, port->portid, frame, len);
}

cs_status mc_flood_msg(mc_node_t *mc_node, mc_object_t *pObj)
{
    cs_status   ret = CS_E_OK;
    cs_port_id_t port_id;
    mc_port_t *port = NULL;

    cs_uint8 *frame = NULL;

    if(mc_node->port_num > 2) {
        frame = mc_malloc(pObj->len);
    }
    else {
        frame = pObj->frame;
    }
        
    for(port_id = 0; port_id < mc_node->port_num; port_id++) {            

        if(!mc_is_uplink_port(mc_node, port_id)) {
            /* not host port */
            port = mc_port_find(mc_node, port_id);
            if (NULL == port) {
                return CS_E_NOT_FOUND;
            }
            else{
                if(frame != pObj->frame) {
                    memcpy(frame, pObj->frame, pObj->len);
                }
                
                ret = mc_fwd_msg(port, pObj->pdu_type, frame, pObj->len);
                MC_LOG(IROS_LOG_LEVEL_DBG3, "%s,forward query to port %d,ret %d\n",__func__,port_id,ret);
            }
        }
    }

    if(frame != pObj->frame) {
        mc_free(frame);
    }

    return ret;
}

