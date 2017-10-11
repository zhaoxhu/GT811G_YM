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

/* This file contains prototypes for system dependent API
   from the RSTP to an operation system */
  
#ifndef _STP_OUT_H__
#define _STP_OUT_H__

/* In the best case: clean all Learning entries with
 the vlan_id and the port (if 'exclude'=0) or for all ports,
 exclude the port (if ''exclude'=1). If 'port'=0, delete all
 entries with the vlan_id, don't care to 'exclude'  */
typedef enum {
  LT_FLASH_ALL_PORTS_EXCLUDE_THIS,
  LT_FLASH_ONLY_THE_PORT
} stp_lt_flash_type_e;

int
STP_OUT_flush_lt (IN int port_index, IN int vlan_id,
                  IN stp_lt_flash_type_e type, IN cs_int8* reason);

void /* for bridge id calculation */
STP_OUT_get_port_mac (IN int port_index, OUT cs_uint8* mac);

cs_uint64
STP_OUT_get_port_oper_speed (IN cs_uint32 portNo);

int /* 1- Up, 0- Down */
STP_OUT_get_port_link_status (IN int port_index);

int /* 1- Full, 0- Half */
STP_OUT_get_duplex (IN int port_index);

#ifdef STRONGLY_SPEC_802_1W
int
STP_OUT_set_learning (IN int port_index, IN int vlan_id, IN int enable);

int
STP_OUT_set_forwarding (IN int port_index, IN int vlan_id, IN int enable);
#else
/*
 * In many kinds of hardware the state of ports may
 * be changed with another method
 */
int
STP_OUT_set_port_state (IN int port_index, IN int vlan_id, IN rstp_port_state_e state);
#endif

int
STP_OUT_set_hardware_mode (int vlan_id, UID_STP_MODE_T mode);

int
STP_OUT_tx_bpdu (IN int port_index, IN int vlan_id,
                 IN cs_uint8* bpdu,
                 IN cs_uint32 bpdu_len);

const cs_int8 *
STP_OUT_get_port_name (IN int port_index);

int
STP_OUT_get_init_stpm_cfg (IN int vlan_id,
                           INOUT UID_STP_CFG_T* cfg);


int
STP_OUT_get_init_port_cfg (IN int vlan_id,
                           IN int port_index,
                           INOUT UID_STP_PORT_CFG_T* cfg);

#endif /* _STP_OUT_H__ */

