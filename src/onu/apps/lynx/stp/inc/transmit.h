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

/* Port Transmit state machine : 17.27 */
  
#ifndef _STP_TRANSMIT_H__
#define _STP_TRANSMIT_H__

#define MIN_FRAME_LENGTH    64
#define BPDU_LEN8023_OFF    12

typedef struct tx_tcn_bpdu_t {
  stp_mac_hdr_t  mac;
  stp_eth_hdr_t  eth;
  BPDU_HEADER_T hdr;
}  stp_tcn_bpdu_t;

typedef struct tx_BPDU_T {
  stp_mac_hdr_t  mac;
  stp_eth_hdr_t  eth;
  BPDU_HEADER_T hdr;
  BPDU_BODY_T   body;
}  stp_config_bpdu_t;

typedef struct tx_rBPDU_T {
  stp_mac_hdr_t  mac;

  stp_eth_hdr_t  eth;
  BPDU_HEADER_T hdr;
  BPDU_BODY_T   body;
 // cs_uint8 ver_1_length[2];
  cs_uint8 ver_1_length[1];
} __attribute__((packed))  stp_tx_bpdu_t;


void
STP_transmit_enter_state (STATE_MACH_T* s);
  
Bool
STP_transmit_check_conditions (STATE_MACH_T* s);

cs_int8*
STP_transmit_get_state_name (int state);

#endif /* _STP_TRANSMIT_H__ */

