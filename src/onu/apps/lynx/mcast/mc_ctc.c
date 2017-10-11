#ifdef HAVE_CTC_OAM
#include "cs_cmn.h"
#include "plat_common.h"
#include "mc_api.h"
#include "app_vlan.h"
#include "packet.h"

extern cs_status mc_vlan_ingress_snoop_filter(cs_pkt_t *pkt, cs_uint8 type, cs_uint8 *changed);
extern cs_status mc_vlan_ingress_mcctrl_filter(cs_pkt_t *pkt, cs_uint8 type, cs_uint8 *changed);
extern cs_int32 mc_vlan_egress_filter(
                                                    cs_port_id_t dport,
                                                    cs_uint32 *len,
                                                    cs_uint8 *buf);

mc_fwd_action_t mc_snoop_ingress_filter(cs_pkt_t *pkt, cs_uint8 type, cs_uint8 *chg)
{
    mc_fwd_action_t action;

    if(pkt->port == 0) {
        return MC_PKT_FWD;
    }

    cs_log_msg(IROS_LOG_LEVEL_INF, 15, "enter [mc_snoop_ingress_filter]\n");
    
    if(mc_vlan_ingress_snoop_filter(pkt, type, chg)) {
        action = MC_PKT_DROP;
    }
    else {
        action = MC_PKT_FWD;
    }

    return action;
}

mc_fwd_action_t mc_ctrl_ingress_filter(cs_pkt_t *pkt, cs_uint8 type, cs_uint8 *chg)
{
    mc_fwd_action_t action;

    if(pkt->port == 0) {
        return MC_PKT_FWD;
    }

    cs_log_msg(IROS_LOG_LEVEL_INF, 15, "enter [mc_ctrl_ingress_filter]\n");
    
    if(mc_vlan_ingress_mcctrl_filter(pkt, type, chg)) {
        action = MC_PKT_DROP;
    }
    else {
        action = MC_PKT_FWD;
    }

    return action;
}

cs_status mc_egress_filter(cs_port_id_t port, cs_uint8 type, cs_uint8 *frame, cs_uint16 *len)
{
    cs_uint32 pkt_len = *len;

    if(port == 0) {
        return CS_E_OK;
    }
    
    if(CS_E_ERROR != mc_vlan_egress_filter(port, &pkt_len, frame)) {
        cs_log_msg(IROS_LOG_LEVEL_INF, 15, "enter [mc_egress_filter]\n");
        *len = (cs_uint16)pkt_len;
        return CS_E_OK;
    }

    return CS_E_ERROR;
}

#endif  /* #ifdef HAVE_CTC_OAM */

