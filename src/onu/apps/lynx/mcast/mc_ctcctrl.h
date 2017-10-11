#ifndef _MC_CTCCTRL_H
#define _MC_CTCCTRL_H

#include "mc_type.h"

typedef enum {
    MC_CTRL_GDA_MAC = 0,
    MC_CTRL_GDA_MAC_VLAN = 1,
    MC_CTRL_GDA_MAC_SA_IP = 2,    
    MC_CTRL_GDA_IPV4_VLAN = 3,
    MC_CTRL_GDA_IPV6_VLAN = 4,
    MC_CTRL_TYPE_UNDEF
} mc_ctc_ctrl_type_t;

void mc_ctrl_init();
cs_uint32 mc_get_fast_leave_state();
cs_status mc_set_fast_leave_state(cs_uint32 state);
cs_status mc_set_mc_fwd_mode(mc_ctc_ctrl_type_t mode);
mc_ctc_ctrl_type_t mc_get_mc_fwd_mode();
cs_status mc_ctc_ctrl_add_mac_vlan_entry(cs_port_id_t port_id, cs_uint8 *da, cs_uint16 vid);
cs_status mc_ctc_ctrl_del_mac_vlan_entry(cs_port_id_t port_id, cs_uint8 *da, cs_uint16 vid);
cs_status mc_ctc_ctrl_add_da_sip_entry(
    cs_port_id_t port_id, 
    cs_uint8 *da, 
    mc_ip_type_t ip_type, 
    cs_uint8 *sip);
cs_status mc_ctc_ctrl_del_da_sip_entry(
    cs_port_id_t port_id, 
    cs_uint8 *da, 
    mc_ip_type_t ip_type, 
    cs_uint8 *sip);
cs_status mc_ctc_ctrl_add_ip_vlan_entry(
    cs_port_id_t port_id, 
    cs_uint8 *dip, 
    mc_ip_type_t ip_type,
    cs_uint16 vid);
cs_status mc_ctc_ctrl_del_ip_vlan_entry(
    cs_port_id_t port_id, 
    cs_uint8 *dip, 
    mc_ip_type_t ip_type,
    cs_uint16 vid);
cs_status mc_ctc_ctrl_clr_mfdb();
cs_status mc_ctc_ctrl_get_entries(cs_uint8 *output, cs_uint16 *count);

#endif /* #ifndef _MC_CTCCTRL_H */

