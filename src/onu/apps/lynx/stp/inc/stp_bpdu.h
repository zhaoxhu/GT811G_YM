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

/* BPDU formats: 9.1 - 9.3, 17.28 */
 
#ifndef _STP_BPDU_H__
#define _STP_BPDU_H__

#define MIN_BPDU                7
#define BPDU_L_SAP              0x42
#define LLC_UI                  0x03
#define BPDU_PROTOCOL_ID        0x0000
#define BPDU_VERSION_ID         0x00
#define BPDU_VERSION_RAPID_ID   0x02

#define BPDU_TOPO_CHANGE_TYPE   0x80
#define BPDU_CONFIG_TYPE        0x00
#define BPDU_RSTP               0x02

#define TOLPLOGY_CHANGE_BIT     0x01
#define PROPOSAL_BIT            0x02
#define PORT_ROLE_OFFS          2   /* 0x04 & 0x08 */
#define PORT_ROLE_MASK          (0x03 << PORT_ROLE_OFFS)
#define LEARN_BIT               0x10
#define FORWARD_BIT             0x20
#define AGREEMENT_BIT           0x40
#define TOLPLOGY_CHANGE_ACK_BIT 0x80

#define RSTP_PORT_ROLE_UNKN     0x00
#define RSTP_PORT_ROLE_ALTBACK  0x01
#define RSTP_PORT_ROLE_ROOT     0x02
#define RSTP_PORT_ROLE_DESGN    0x03

typedef struct mac_header_t {
  cs_uint8 dst_mac[6];
  cs_uint8 src_mac[6];
} __attribute__((packed))  stp_mac_hdr_t;

typedef struct eth_header_t {
  cs_uint8 len8023[2];
  cs_uint8 dsap;
  cs_uint8 ssap;
  cs_uint8 llc;
} __attribute__((packed)) stp_eth_hdr_t;

typedef struct bpdu_header_t {
  cs_uint8 protocol[2];
  cs_uint8 version;
  cs_uint8 bpdu_type;
} __attribute__((packed)) BPDU_HEADER_T;

typedef struct bpdu_body_t {
  cs_uint8 flags;
  cs_uint8 root_id[8];
  cs_uint8 root_path_cost[4];
  cs_uint8 bridge_id[8];
  cs_uint8 port_id[2];
  cs_uint8 message_age[2];
  cs_uint8 max_age[2];
  cs_uint8 hello_time[2];
  cs_uint8 forward_delay[2];
} __attribute__((packed)) BPDU_BODY_T;

typedef struct BPDU_T {
  stp_eth_hdr_t  eth;
  BPDU_HEADER_T hdr;
  BPDU_BODY_T   body;
  cs_uint8 ver_1_len[2];
} __attribute__((packed)) BPDU_T;
#endif /* _STP_BPDU_H__ */

