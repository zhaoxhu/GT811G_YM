/************************************************************************ 
 * RSTP library - Rapid Spanning Tree (802.1t, 802.1w) 
 * Copyright (C) 2001-2003 Optical Access 
 * Author: Alex Rozin 
 * 
 * This file is part of RSTP library. 
 * 
 * RSTP library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by the 
 * Free Software Foundation; version 2.1 
 * 
 * RSTP library is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser 
 * General Public License for more details. 
 * 
 * You should have received a copy of the GNU Lesser General Public License 
 * along with RSTP library; see the file COPYING.  If not, write to the Free 
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 
 * 02111-1307, USA. 
 **********************************************************************/

/* This file contains system dependent API
   from the RStp to a operation system (see stp_to.h) */

/* stp_to API for Linux */

#include "base.h"
#include "stpm.h"
#include "stp_in.h"
#include "stp_to.h"
#include "packet.h"
#include "sdl_api.h"
#include "stp_sdl_adapt.h"

cs_uint8 pktRstpTrace = 0;
extern cs_mac_t           stp_bridge_macaddr;

#ifdef STRONGLY_SPEC_802_1W
int
STP_OUT_set_learning (int port_index, int vlan_id, int enable)
{
  return STP_OK;
}

int
STP_OUT_set_forwarding (int port_index, int vlan_id, int enable)
{
  return STP_OK;
}
#else
/* 
 * In many kinds of hardware the state of ports may
 * be changed with another method
 */
int
STP_OUT_set_port_state (IN int port_index, IN int vlan_id,
            IN rstp_port_state_e state)
{
        stp_set_port_stp_state(port_index,  state);
        return STP_OK;
  //return AR_INT_STP_set_port_state (port_index, vlan_id, state);
}
#endif


void
STP_OUT_get_port_mac (int port_index, cs_uint8 *mac)
{
    /*get mac address*/
    memcpy((cs_int8 *)mac, &stp_bridge_macaddr , CS_MACADDR_LEN);

}

#ifdef STP_REMOVE
int             /* 1- Up, 0- Down */
STP_OUT_get_port_link_status (int port_index)
{
  if (BitmapGetBit (&enabled_ports, (port_index - 1))) return 1;
  return 0;
}
#endif

int
STP_OUT_flush_lt (IN int port_index, IN int vlan_id, stp_lt_flash_type_e type, cs_int8* reason)
{
      cs_callback_context_t context;
      
      epon_request_onu_fdb_entry_clr (context, ONU_DEVICEID_FOR_API, 
      ONU_LLIDPORT_FOR_API, SDL_FDB_CLR_DYNAMIC);
      return STP_OK;
}

int
STP_OUT_set_hardware_mode (int vlan_id, UID_STP_MODE_T mode)
{
  return STP_OK;
  //return AR_INT_STP_set_mode (vlan_id, mode);
}


int
STP_OUT_tx_bpdu (int port_index, int vlan_id,
         cs_uint8 *bpdu, cs_uint32 bpdu_len)
{
    app_pkt_tx(port_index, CS_PKT_BPDU, bpdu, bpdu_len);

    return 1;
    
}

const cs_int8 *
STP_OUT_get_port_name (IN int port_index)
{
  static cs_int8 tmp[10];
  sprintf (tmp, "p%02d", (int) port_index);
  return tmp;
  //return port2str (port_index, &sys_config);
}

cs_uint64
STP_OUT_get_deafult_port_path_cost (IN cs_uint32 portNo)
{
  return 20000;
}

cs_uint64 STP_OUT_get_port_oper_speed (cs_uint32 portNo)
{
        cs_uint64 port_speed=100L;
        cs_sdl_port_ether_speed_t speed;

        if (portNo == CS_PON_PORT_ID) {
                port_speed=1000L;
        }else {
#ifdef   HAVE_MPORTS  
            cs_callback_context_t context;
            
            epon_request_onu_port_speed_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, portNo, 
            &speed);

            STP_LOG("port %d, speed %d\n", portNo, speed);
            
            if(speed == 0)
                port_speed = 10L;
            else if(speed == 1)
                port_speed = 100L;
            else if(speed == 2)
                port_speed = 1000L;
#endif
        }
        
        return port_speed;
}

int             /* 1- Full, 0- Half */
STP_OUT_get_duplex (IN int port_index)
{
  return 1;
}

int
STP_OUT_get_init_stpm_cfg (IN int vlan_id,
                           INOUT UID_STP_CFG_T* cfg)
{
  cfg->bridge_priority =        DEF_BR_PRIO;
  cfg->max_age =                DEF_BR_MAXAGE;
  cfg->hello_time =             DEF_BR_HELLOT;
  cfg->forward_delay =          DEF_BR_FWDELAY;
  cfg->force_version =          NORMAL_RSTP;

  return STP_OK;
}
  

int
STP_OUT_get_init_port_cfg (IN int vlan_id,
                           IN int port_index,
                           INOUT UID_STP_PORT_CFG_T* cfg)
{
  cfg->port_priority =                  DEF_PORT_PRIO;
  cfg->admin_non_stp =                  DEF_ADMIN_NON_STP;
  cfg->admin_edge =                     DEF_ADMIN_EDGE;
  cfg->admin_port_path_cost =           ADMIN_PORT_PATH_COST_AUTO;
  cfg->admin_point2point =              DEF_P2P;

  return STP_OK;
}

