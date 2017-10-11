/****************************************************************************
            Software License for Customer Use of Cortina Software
                          Grant Terms and Conditions
 
IMPORTANT NOTICE - READ CAREFULLY: This Software License for Customer Use
of Cortina Software ("LICENSE") is the agreement which governs use of
software of Cortina Systems, Inc. and its subsidiaries ("CORTINA"),
including computer software (source code and object code) and associated
printed materials ("SOFTWARE").  The SOFTWARE is protected by copyright laws
and international copyright treaties, as well as other intellectual property
laws and treaties.  The SOFTWARE is not sold, and instead is only licensed
for use, strictly in accordance with this document.  Any hardware sold by
CORTINA is protected by various patents, and is sold but this LICENSE does
not cover that sale, since it may not necessarily be sold as a package with
the SOFTWARE.  This LICENSE sets forth the terms and conditions of the
SOFTWARE LICENSE only.  By downloading, installing, copying, or otherwise
using the SOFTWARE, you agree to be bound by the terms of this LICENSE.
If you do not agree to the terms of this LICENSE, then do not download the
SOFTWARE.
 
DEFINITIONS:  "DEVICE" means the Cortina Systems LynxD product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems software.
 
GRANT OF LICENSE:  Subject to the restrictions below, CORTINA hereby grants
CUSTOMER a non-exclusive, non-assignable, non-transferable, royalty-free,
perpetual copyright license to (1) install and use the SOFTWARE for
reference only with the DEVICE; and (2) copy the SOFTWARE for your internal
use only for use with the DEVICE.
 
RESTRICTIONS:  The SOFTWARE must be used solely in conjunction with the
DEVICE and solely with Your own products that incorporate the DEVICE.  You
may not distribute the SOFTWARE to any third party.  You may not modify
the SOFTWARE or make derivatives of the SOFTWARE without assigning any and
all rights in such modifications and derivatives to CORTINA.  You shall not
through incorporation, modification or distribution of the SOFTWARE cause
it to become subject to any open source licenses.  You may not
reverse-assemble, reverse-compile, or otherwise reverse-engineer any
SOFTWARE provided in binary or machine readable form.  You may not
distribute the SOFTWARE to your customers without written permission
from CORTINA.
 
OWNERSHIP OF SOFTWARE AND COPYRIGHTS. All title and copyrights in and the
SOFTWARE and any accompanying printed materials, and copies of the SOFTWARE,
are owned by CORTINA. The SOFTWARE protected by the copyright laws of the
United States and other countries, and international treaty provisions.
You may not remove any copyright notices from the SOFTWARE.  Except as
otherwise expressly provided, CORTINA grants no express or implied right
under CORTINA patents, copyrights, trademarks, or other intellectual
property rights.
 
DISCLAIMER OF WARRANTIES. THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY
EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING ANY IMPLIED WARRANTIES
OF MERCHANTABILITY, NONINFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE,
TITLE, AND NON-INFRINGEMENT.  CORTINA does not warrant or assume
responsibility for the accuracy or completeness of any information, text,
graphics, links or other items contained within the SOFTWARE.  Without
limiting the foregoing, you are solely responsible for determining and
verifying that the SOFTWARE that you obtain and install is the appropriate
version for your purpose.
 
LIMITATION OF LIABILITY. IN NO EVENT SHALL CORTINA OR ITS SUPPLIERS BE
LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, LOST
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING CS_OUT
OF THE USE OF OR INABILITY TO USE OF OR INABILITY TO USE THE SOFTWARE, EVEN
IF CORTINA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
TERMINATION OF THIS LICENSE. This LICENSE will automatically terminate if
You fail to comply with any of the terms and conditions hereof. Upon
termination, You will immediately cease use of the SOFTWARE and destroy all
copies of the SOFTWARE or return all copies of the SOFTWARE in your control
to CORTINA.  IF you commence or participate in any legal proceeding against
CORTINA, then CORTINA may, in its sole discretion, suspend or terminate all
license grants and any other rights provided under this LICENSE during the
pendency of such legal proceedings.
APPLICABLE LAWS. Claims arising under this LICENSE shall be governed by the
laws of the State of California, excluding its principles of conflict of
laws.  The United Nations Convention on Contracts for the International Sale
of Goods is specifically disclaimed.  You shall not export the SOFTWARE
without first obtaining any required export license or other approval from
the applicable governmental entity, if required.  This is the entire
agreement and understanding between You and CORTINA relating to this subject
matter.
GOVERNMENT RESTRICTED RIGHTS. The SOFTWARE is provided with "RESTRICTED
RIGHTS." Use, duplication, or disclosure by the Government is subject to
restrictions as set forth in FAR52.227-14 and DFAR252.227-7013 et seq. or
its successor. Use of the SOFTWARE by the Government constitutes
acknowledgment of CORTINA's proprietary rights therein. Contractor or
Manufacturer is CORTINA.
 
Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/

/*

	This file is used for classification code
	
*/

#ifndef __AAL_CLS_H__
#define __AAL_CLS_H__

#include "cs_types.h"
#include "aal.h"

//reserve cls 
#define AAL_CL_RSV_ENTRY_UP_A       47
#define AAL_CL_RSV_SPE_ENTRY_UP_A    7

/* CL DPID Resolution */
typedef enum {
     AAL_CL_DST_PORT           = 0,  /* cs_aal_port_id_t port;     */
     AAL_CL_DST_DROP           = 1,  /* Blackhole port (dropped);  */  
     AAL_CL_DST_L2             = 2   /* Reserved port for CE, indicating the DPID should be further */
                                     /* retrieved from L2 engine;  */
}cs_aal_cl_dst_op_t;

 
typedef struct {   
    cs_aal_cl_dst_op_t     dst_op;
    cs_aal_port_id_t       dpid;       // valid when op = AAL_SPEC_DST_PORT
    
} cs_aal_cl_dpid_t;


/* op, port from cs_aal_cl_dpid_t, dpid is cs_aal_dpid_t */
#define CL_PORT_TO_DPID(op, port, dpid)                do{          \
    if(op == 1)                                                     \
    {                                                               \
        dpid = 3;                                                   \
    }                                                               \
    else if (op == 2)                                               \
    {                                                               \
        dpid = 2;                                                   \
    }                                                               \
    else                                                            \
    {                                                               \
        if ( port >1 )                                              \
        {                                                           \
            dpid = (port +2);                                       \
        }                                                           \
        else                                                        \
        {                                                           \
            dpid = port;                                            \
        }                                                           \
    }                                                               \
}while(0)


#define CL_DPID_TO_PORT(dpid, op, port)                do{          \
    if(dpid == 3)                                                   \
    {                                                               \
        op = 1;                                                     \
    }                                                               \
    else if (dpid == 2)                                             \
    {                                                               \
        op = 2;                                                     \
    }                                                               \
    else                                                            \
    {                                                               \
        op = 0;                                                     \
        if ( dpid < 2 )                                             \
        {                                                           \
            port = dpid;                                            \
        }                                                           \
        else                                                        \
        {                                                           \
            port = (dpid-2);                                        \
        }                                                           \
    }                                                               \
}while(0)


 /* VLAN commands */
 typedef enum {
     AAL_CL_VLAN_CMD_NOP  = 0,
     AAL_CL_VLAN_CMD_PUSH = 1,
     AAL_CL_VLAN_CMD_POP  = 2,
     AAL_CL_VLAN_CMD_SWAP = 3
 
 }cs_aal_cl_vlan_cmd_t;

 
 /* VLAN ID selection */
 typedef enum {
     AAL_CL_VLANID_SEL_L2 = 0,
     AAL_CL_VLANID_SEL_CL = 1
   
 }cs_aal_cl_vlanid_sel_t;

 
 /* TPID Selection */
 typedef enum{
     AAL_CL_TPID_SEL_NOP           = 0,         /* Means no modification */
     AAL_CL_TPID_SEL_8100          = 1,         /* Means 0x8100 */
     AAL_CL_TPID_SEL_88A8          = 2,         /* Means 0x88a8 */
     AAL_CL_TPID_SEL_USER_DEF1     = 3,         /* Inner vlan menas ingress_inner_tpid, outer means ingress_outer_tpid1 */
     AAL_CL_TPID_SEL_USER_DEF2     = 4,         /* Inner vlan menas ingress_inner_tpid, outer means ingress_outer_tpid2 */
     AAL_CL_TPID_SEL_DST_PORT_DEF  = 5, 
     AAL_CL_TPID_SEL_RX_INNER_TPID = 6,
     AAL_CL_TPID_SEL_RX_OUTER_TPID = 7
     
 }cs_aal_cl_tpid_sel_t;

 
 /* DSCP Selection */
 typedef enum {
     AAL_CL_DSCP_SEL_ORGINAL_RX = 0,    /* Use receiving original value  */
     AAL_CL_DSCP_SEL_CL         = 1,    /* Use classification value      */
     AAL_CL_DSCP_SEL_MAP_RX     = 2,    /* Use QoS defined mapping value */
     AAL_CL_DSCP_SEL_MAP_DOT1P  = 3     /* Use dot1p mapped value defined in QoS */
   
 }cs_aal_cl_dscp_sel_t;

 
 /* DOT1P Selection */
 typedef enum {
   
     AAL_CL_DOT1P_SEL_ORGINAL_RX       = 0,    /* Use receiving original value  */
     AAL_CL_DOT1P_SEL_CL               = 1,    /* Use classification value      */
     AAL_CL_DOT1P_SEL_MAP_RX_DOT1P     = 2,    /* Use QoS defined mapping value */
     AAL_CL_DOT1P_SEL_MAP_RX_DSCP      = 3,    /* Use dscp mapped value defined in QoS */
     AAL_CL_DOT1P_SEL_MAP_DST_PORT_DEF = 4
   
 }cs_aal_cl_dot1p_sel_t;

 
 /* COS Selection */
 typedef enum {  
     AAL_CL_COS_SEL_MAP_PORT_DEF = 0,    /* Use port default */
     AAL_CL_COS_SEL_CL           = 1,    /* Use classification value */
     AAL_CL_COS_SEL_MAP_RX_DOT1P = 2, 
     AAL_CL_COS_SEL_MAP_RX_DSCP  = 3
   
 }cs_aal_cl_cos_sel_t;

 
 /* FLOWID Selection */
 typedef enum {
     AAL_CL_FLOWID_SEL_NO       = 0,       /* No rate limit */
     AAL_CL_FLOWID_SEL_RX_DOT1P = 1,       /* Use ingress dot1p */
     AAL_CL_FLOWID_SEL_CL_RSLT  = 2,       /* Use classification result */
     AAL_CL_FLOWID_SEL_RX_DSCP  = 3,       /* Use ingress dscp */
     AAL_CL_FLOWID_SEL_TX_COS   = 4        /* Use tx tos */
    
 }cs_aal_cl_flowid_sel_t;

 
 /* Egress VLAN checking selection */
 typedef enum {
     AAL_CL_EG_VLAN_CHECK_OUT_VLAN    = 0,
     AAL_CL_EG_VLAN_CHECK_IN_VLAN     = 1,
     AAL_CL_EG_VLAN_CHECK_CL_TOP_VLAN = 2, 
     AAL_CL_EG_VLAN_CHECK_NO          = 3
     
 }cs_aal_cl_eg_vlan_check_t;

 
 /* Mirror source selection */
 typedef enum{  
     AAL_CL_MIRR_SEL_BEFORE_PE = 0, /* Do packet mirroring before packet editing */
     AAL_CL_MIRR_SEL_AFTER_PE  = 1  /* Do packet mirroring after packet editing */
     
 }cs_aal_cl_mirr_sel_t;

 
typedef enum{
    AAL_CL_L4_SRC_TCP = 0,
    AAL_CL_L4_SRC_UDP = 1,
    AAL_CL_L4_DST_TCP = 2,
    AAL_CL_L4_DST_UDP = 3
 
}cs_aal_cl_l4_port_type_t;


typedef enum{
    AAL_CL_SPCL_MODE_STA = 0,    /* Static mode, always use the static packet offset */
    AAL_CL_SPCL_MODE_DYN = 1     /* Dynamic mode, refer to the L2/IP/L4 header enable control */
   
}cs_aal_cl_spcl_mode_t;


typedef enum{
    AAL_CL_SPCL_PTN_CMP_LO = 0,  /* Compare the lower 16 bits from the 32 bit packet pattern */
    AAL_CL_SPCL_PTN_CMP_HI = 1   /* Compare the most significant 16 bits from the 32 bit packet pattern */
   
}cs_aal_cl_spcl_ptn_cmp_t;


typedef enum{
    AAL_CL_SPCL_PTN_TYPE_L2  = 0, /* Match the pattern if it's extracted from a L2 packet header */
    AAL_CL_SPCL_PTN_TYPE_IP  = 1, /* Match the pattern if it's extracted from a L3 IP header */
    AAL_CL_SPCL_PTN_TYPE_L4  = 2, /* Match the pattern if it's extracted from a L4 TCP/UDP header */
    AAL_CL_SPCL_PTN_TYPE_STA = 3  /* Match the pattern if it's extracted by the static mode */
   
}cs_aal_cl_spcl_ptn_type_t;


/* classification key type */
typedef enum {
    AAL_CL_IPV4_LONG_KEY          = 0,
    AAL_CL_IPV4_SHORT_KEY         = 1,
    AAL_CL_IPV6_LONG_KEY          = 2,
    AAL_CL_IPV6_SHORT_KEY         = 3,
    AAL_CL_MIRROR_KEY             = 4,
    AAL_CL_MAC_VLAN_KEY           = 5,
    AAL_CL_MAC_ETYPE_KEY          = 6,
    AAL_CL_CTRL_PKT_KEY           = 7,
    AAL_CL_VLAN_KEY               = 8,
    AAL_CL_VLAN_RANGE_KEY         = 9,
    AAL_CL_PROTO_KEY              = 10,
    AAL_CL_MCST_KEY               = 11,
    AAL_CL_MAC_VLAN_KEY_C         = 12,
    AAL_CL_MAC_ETYPE_KEY_D        = 13,
    AAL_CL_VLAN_KEY_E             = 14,
    AAL_CL_MAX_KEY                = 15
}cs_aal_cl_key_t;


// Domain for manage resouece 
#define AAL_CL_DOMAIN_ID_NUM        16

typedef struct {
    cs_uint8          cl_entry_start; // res strart 0-47 for PON and UNI FE, 0-15 for MA FE
    cs_uint8          length;         // res length 1-48 for PON and UNI FE, 1-16 for MA FE 
    
}cs_aal_cl_domain_res_t;


/* L4 port range table, each meaning:
   o  cmp_en: means this entry is used to compare ot not.
       0 means this entry is not used.
       1 means used.
   o  low_port: Low L4 port value;
   o  high_port: High L4 port value.
*/
typedef struct {
    cs_boolean         cmp_en;
    cs_uint16          low_port;
    cs_uint16          high_port;
    
}cs_aal_cl_l4_port_range_t;


/* Aging timer table, each meaning:
   o  valid: means the validity of this entry
       0 means valid.
       1 means invalid.
   o  aging_timer: aging timer value; */
typedef struct {
    cs_boolean          valid;
    cs_uint32           aging_time; /* Unit is second */
}cs_aal_cl_mac_aging_t;


/* CS8030 can be configured to extract 32 bits from:
   immediately after the Layer2/Layer3/Layer4 packet header or
   from a configured offset from the first byte of the packet.
   In addition, three separate control bits can be configured
   to enable packet header extraction for:
   non-IP packets
   IP packets with protocol type that is neither TCP nor UDP
   TCP/UDP packets
   One or more of those three bits can be set by software to control
   o  spcl_hdr_mode_sel - Special header extraction mode
                    1 -Dynamic header extraction mode, Extract special
                         header received based on the received packet
                         Non-IP, extract 4 bytes after EV2pt
                         For IP but Non-TCP/UDP, extract 4 bytes after IP header
                         For TCP/UDP packet, extract 4 bytes after TCP/UDP header
                    0 -Global header extraction mode: Four consecutive bytes in the
                         receive packet will be treated as special header based on the
                         programmed start index spcl_glb_hdr_strt_idx.
   o  spcl_hdr_after_l2hdr_en - Special Header for Non-IP packet  control.
                   1 - Enable special header extraction
                   0 - Disable special header extraction
   o  spcl_hdr_after_l2hdr_en - Special Header for Non-IP packet  control.
                   1 - Enable special header extraction
                   0 - Disable special header extraction
   o  spcl_hdr_after_iphdr_en - Special Header for IP but non-tcp/udp packet  control.
                   1 - Enable special header extraction
                   0 - Disable special header extraction
   o  spcl_hdr_after_tcphdr_en - Special Header for tcp/udp packet  control.
                   1 - Enable special header extraction
                   0 - Disable special header extraction
   o  spcl_glb_hdr_strt_idx: Special global header extraction start index. */

/* Special Pattern parsing control structure */
typedef union {
    struct {
    cs_uint32  mode_sel      : 1;        
    cs_uint32  hdr_sel       : 1;
    cs_uint32  parse_ctrl    : 1;
    cs_uint32  glb_strt      : 1; 
    cs_uint32  rsvd          : 28;
    } s;
    cs_uint32  u32;
    
}cs_aal_cl_spcl_ptn_control_msk_t;

typedef struct {
    cs_aal_cl_spcl_mode_t     spcl_hdr_mode_sel;       // (default=AAL_CL_SPCL_MODE_STA), use static/dynamic mode
    cs_boolean                spcl_hdr_after_l2hdr_en; // (defalt=true), check non-IP packet after L2 header
    cs_boolean                spcl_hdr_after_iphdr_en; // (defalt=true), check IP but non-TCP/UDP packet after L3 header
    cs_boolean                spcl_hdr_after_l4hdr_en; // (defalt=true), check TCP/UDP packet after L4 header
    cs_boolean                spcl_hdr_parsing_en;     // (defalt=true), Special header extraction control at the time of packet parsing
    cs_uint8                  spcl_glb_hdr_strt_idx;   // 0~127(default=0): under static mode, specify the fixed offset for parsing

}cs_aal_cl_spcl_ptn_control_t;

/* Special Pattern structure 
Note: There is a limitation. The extracted 32 bits are matched against all eight signatures 
stored in the Special Packet Header table. the result will be compared to spcl_hit_map.
Led to the following results:
1. MS and LS will be a combination for 32 bits in Special Header table.
2.Two bits in the spcl_hit_map must be set if Special Header table has MS and LS entries.
3.One bit in the spcl_hit_map should be set if Special Header table only has MS or LS entries.
The rule will never hit when more bits in the spcl_hit_map be set. */
typedef struct {
    cs_aal_cl_spcl_ptn_type_t  spcl_ptn_type; // (default=AAL_CL_SPCL_PTN_TYPE_L2) How to match the packet 
    cs_aal_cl_spcl_ptn_cmp_t   spcl_ptn_cmp;  // (default=AAL_CL_SPCL_PTN_CMP_LO) Compare with lo/hi 16bits from the packets pattern with below key/mask
    cs_uint16                  spcl_ptn_key;  // (default=0) 16bit key value for speical pattern key
    cs_uint16                  spcl_ptn_mask; // (default=0) 16bit key mask for special pattern mask
}cs_aal_cl_spcl_ptn_t;


// CE control
typedef struct {
    cs_mac_t          mac_da_high;         // MAC for mac_da_range_flag,default :0x33:33:ff:ff:ff:ff----0x33:33:00:01:00:00
    cs_mac_t          mac_da_low;          //
    cs_boolean        mac_filter_aging_en; // (default= true), enable/disable mac filter aging function      
    cs_boolean        tos_ecn_msk_en;      // (default= true), Mask(1) /Enable(0) the IP-TOS ECN bits in the classifier
    cs_boolean        ce_msk_cfi_flg;      // (default= true), Ignore(1)/Compare(0) the cfi_flag in the Classification Key
    cs_uint8          rsvd;
    
}cs_aal_cl_ctrl_t;

typedef union {
    struct {
    cs_uint32  mac_da_range          : 1;
    cs_uint32  mac_filter_aging_en   : 1;
    cs_uint32  tos_ecn_msk_en        : 1;
    cs_uint32  ce_msk_cfi_flg        : 1;
    cs_uint32  rsvd                  : 28;
    } s;
    cs_uint32  u32;
    
}cs_aal_cl_ctrl_msk_t;


/* IPv4 long key type */
typedef union {
    struct {
    cs_uint32  spid              : 1;
    cs_uint32  mac_sa            : 1;
    cs_uint32  mac_da            : 1;
    cs_uint32  ev2pt             : 1;
    cs_uint32  vlanid            : 1;
    cs_uint32  tpid_enc          : 1;
    cs_uint32  dot1p             : 1;
    cs_uint32  srcip             : 1;
    cs_uint32  dstip             : 1;
    cs_uint32  ipdscp            : 1;
    cs_uint32  ipprotocol        : 1;
    cs_uint32  ip_fragment       : 1;
    cs_uint32  ip_option         : 1;
    cs_uint32  l4_port           : 1;
    cs_uint32  l4_src_hit_map    : 1;
    cs_uint32  l4_dest_hit_map   : 1;
    cs_uint32  tcp_flags         : 1;
    cs_uint32  spcl_hit_map      : 1;
    cs_uint32  icmp_type         : 1;
    cs_uint32  prsr_len_excds    : 1;
    cs_uint32  p_tag_flag        : 1; 
    cs_uint32  vlan_count        : 1;      
    cs_uint32  igmp_type         : 1;
    cs_uint32  mask_all_msk      : 1;   // for always match (mask all field if the field can be masked. will ignore above other msk..)
    cs_uint32  rule_mode         : 1;   
    cs_uint32  rsvd              : 7;
    } s;
    cs_uint32  u32;
}cs_aal_cl_ipv4_long_key_msk_t;

typedef struct {
    cs_aal_cl_ipv4_long_key_msk_t           mask;              /* 1:care;0:no care.*/
    cs_uint8                                dstmac[6];
    cs_uint8                                srcmac[6];
    cs_uint16                               vlanid;
    cs_uint8                                spid;
    cs_uint8                                dot1p;
    cs_uint16                               ev2pt;
    cs_uint8                                tpid_enc;
    cs_uint8                                vlan_count;
    cs_uint32                               dstip;
    cs_uint32                               srcip;
    cs_uint8                                dstipmask;
    cs_uint8                                srcipmask;
    cs_uint8                                ipprotocol;
    cs_uint8                                ipdscp;
    cs_uint8                                ip_fragment_state;
    cs_uint8                                ip_option_flag;
    cs_uint8                                prsr_len_excds;
    cs_uint8                                p_tag_flag;
    cs_uint16                               l4_port;
    cs_uint8                                l4_port_type;       /* 0,Destination port/1,Source port */
    cs_uint8                                tcp_flags;          /* low 6bits */
    cs_uint8                                l4_src_hit_map;
    cs_uint8                                l4_dest_hit_map;
    cs_uint8                                spcl_hit_map;
    cs_uint8                                icmp_type;
    cs_uint8                                igmp_type;
    cs_boolean                              rule_mode;          //default(0,AND), enable rule "AND/OR" function,1,"OR" function, 
                                                                //any hit in the layer2, layer 3, layer 4 or special header hit sub classification rules is taken as a final hit. 
       
}cs_aal_cl_ipv4_long_key_t;


/* IPv4 short key type */
typedef union {
    struct {
    cs_uint32  ip              : 1;
    cs_uint32  dscp_tos        : 1;
    cs_uint32  ipprotocol      : 1;
    cs_uint32  ip_fragment     : 1;
    cs_uint32  ip_option       : 1;
    cs_uint32  l4_port         : 1;
    cs_uint32  prsr_len_excds  : 1;
    cs_uint32  mask_all_msk    : 1;   // for always match (mask all field,if the field can be masked. will ignore other msk.)
    cs_uint32  rsvd            : 24;
    } s;
    cs_uint32  u32;
    
}cs_aal_cl_ipv4_short_key_msk_t;

typedef struct {
    cs_aal_cl_ipv4_short_key_msk_t      mask;               /* 1:care;0:no care. */
    cs_uint32                           ip;
    cs_uint8                            ipmask;
    cs_uint8                            ip_type;            /* 0,dst ip;1 src ip */
    cs_uint8                            ipprotocol;
    cs_uint8                            dscp_tos;
    cs_uint16                           l4_port;
    cs_uint8                            l4_port_type;       /* 0,Destination port/1,Source port */
    cs_uint8                            prsr_len_excds;
    
}cs_aal_cl_ipv4_short_key_t;


/* IPv6 long key */
typedef union {
    struct {
cs_uint32  spid              : 1;
cs_uint32  mac_sa            : 1;
cs_uint32  mac_da            : 1;
cs_uint32  ev2pt             : 1;
cs_uint32  vlanid            : 1;
cs_uint32  tpid_enc          : 1;
cs_uint32  dot1p             : 1;
cs_uint32  ipv6_sa           : 1;
cs_uint32  ipv6_da           : 1;
cs_uint32  ipv6_tc           : 1;
cs_uint32  ipprotocol        : 1;
cs_uint32  ip_fragment       : 1;
cs_uint32  ip_option         : 1;
cs_uint32  l4_port           : 1;
cs_uint32  l4_src_hit_map    : 1;
cs_uint32  l4_dest_hit_map   : 1;
cs_uint32  tcp_flags         : 1;
cs_uint32  spcl_hit_map      : 1;
cs_uint32  icmp_type         : 1;
cs_uint32  prsr_len_excds    : 1;
cs_uint32  p_tag_flag        : 1; 
cs_uint32  vlan_count        : 1;      
cs_uint32  igmp_type         : 1;      
cs_uint32  ipv6_flow_label   : 1;
cs_uint32  mask_all_msk      : 1;   // for always match (mask all field if the field can be masked. will ignore above other msk.)
cs_uint32  rule_mode         : 1;        
cs_uint32  rsvd              : 6;
    } s;
    cs_uint32 u32;
    
}cs_aal_cl_ipv6_long_key_msk_t;

typedef struct {
    cs_aal_cl_ipv6_long_key_msk_t      mask;              /* 1:care;0:no care. */
    cs_uint8                           dstmac[6];
    cs_uint8                           srcmac[6];
    cs_uint16                          vlanid;
    cs_uint8                           spid;
    cs_uint8                           dot1p;
    cs_uint16                          ev2pt;
    cs_uint8                           tpid_enc;
    cs_uint8                           vlan_count;
    cs_uint16                          ipv6_da[8];
    cs_uint16                          ipv6_sa[8];
    cs_uint8                           dstipmask;
    cs_uint8                           srcipmask;
    cs_uint8                           ipprotocol;
    cs_uint8                           ipv6_tc;
    cs_uint8                           ip_fragment_state;
    cs_uint8                           ip_option_flag;
    cs_uint8                           prsr_len_excds;
    cs_uint8                           p_tag_flag;
    cs_uint16                          l4_port;
    cs_uint8                           l4_port_type;      /* 0,Destination port/1,Source port */
    cs_uint8                           tcp_flags;         /* low 6bits */
    cs_uint8                           l4_src_hit_map;
    cs_uint8                           l4_dest_hit_map;
    cs_uint8                           spcl_hit_map;
    cs_uint8                           icmp_type;
    cs_uint8                           igmp_type;
    cs_boolean                         rule_mode;         //default(0,AND), enable rule "AND/OR" function,1,"OR" function, 
                                                          //any hit in the layer2, layer 3, layer 4 or special header hit sub classification rules is taken as a final hit. 
    cs_uint8                           rsvd[2];
    cs_uint32                          ipv6_flow_label;
    
}cs_aal_cl_ipv6_long_key_t;


/* IPv6 short key type */
typedef union {
    struct {
    cs_uint32  ipv6_addr       : 1;
    cs_uint32  dscp_tos        : 1;
    cs_uint32  ipprotocol      : 1;
    cs_uint32  ipv6_flow_label : 1;
    cs_uint32  l4_port         : 1;
    cs_uint32  prsr_len_excds  : 1;
    cs_uint32  mask_all_msk    : 1;                     // for always match (mask all field if the field can be masked. will ignore other msk.)
    cs_uint32  rsvd            : 25;
    } s;
    cs_uint32  u32;
    
}cs_aal_cl_ipv6_short_key_msk_t;

typedef struct {
    cs_aal_cl_ipv6_short_key_msk_t       mask;           // 1:care;0:no care.
    cs_uint16                            ipv6_addr[8];
    cs_uint8                             ipmask;
    cs_uint8                             ip_type;        // 0,dst ip;1 src ip
    cs_uint8                             ipprotocol;
    cs_uint8                             dscp_tos;
    cs_uint16                            l4_port;
    cs_uint8                             l4_port_type;   // 0,Destination port/1,Source port
    cs_uint8                             prsr_len_excds;
    cs_uint32                            ipv6_flow_label;
    
}cs_aal_cl_ipv6_short_key_t;


/* Mirror key type */
typedef union {
    struct {
    cs_uint32  spid            : 1;
    cs_uint32  vlanid          : 1;
    cs_uint32  mirror_group_id : 1;
    cs_uint32  mask_all_msk    : 1;                    // for always match (mask all field,if the field can be masked. will ignore other msk.)
    cs_uint32  rsvd            : 28;
    } s;
    cs_uint32 u32;
    
}cs_aal_cl_mirror_key_msk_t;

typedef struct {
    cs_aal_cl_mirror_key_msk_t     mask;               /* 1:care;0:no care.  */
    cs_uint16                      vlanid;
    cs_uint8                       spid;
    cs_uint8                       mirror_group_id; 
    cs_uint8                       mirror_pkt_flag;   
    cs_uint8                       rsvd[3];  
    
}cs_aal_cl_mirror_key_t;


/* MAC VLAN Key */
typedef union {
    struct {
    cs_uint32  mac             : 1;
    cs_uint32  vlanid          : 1;
    cs_uint32  dot1p           : 1;
    cs_uint32  p_tag_flag      : 1;
    cs_uint32  vlan_count      : 1;
    cs_uint32  mask_all_msk    : 1;                     // for always match (mask all field,if the field can be masked. will ignore other msk.)
    cs_uint32  rsvd            : 26;
    } s;
    cs_uint32  u32;
    
}cs_aal_cl_mac_vlan_key_msk_t;

typedef struct {
    cs_aal_cl_mac_vlan_key_msk_t       mask;            // 1:care;0:no care.
    cs_uint8                           mac[6];
    cs_uint8                           mac_type;
    cs_uint8                           dot1p;
    cs_uint16                          vlanid;
    cs_uint8                           p_tag_flag;
    cs_uint8                           vlan_count;
    cs_uint8                           age_timer_offset;
    cs_uint8                           rsvd [3];
    
}cs_aal_cl_mac_vlan_key_t;


/* MAC Ether Key */
typedef union {
    struct {
    cs_uint32  mac               : 1;
    cs_uint32  tpid_enc          : 1;
    cs_uint32  ev2pt             : 1;
    cs_uint32  mac_da_range_flag : 1;
    cs_uint32  mask_all_msk      : 1;                 // for always match (mask all field,if the field can be masked. will ignore other msk.)
    cs_uint32  rsvd              : 27;
    } s;
    cs_uint32  u32;
    
}cs_aal_cl_mac_etype_key_msk_t;

typedef struct {
    cs_aal_cl_mac_etype_key_msk_t   mask;              // 1:care;0:no care.
    cs_uint8                        mac[6];
    cs_uint8                        mac_type;
    cs_uint8                        tpid_enc;
    cs_uint16                       ev2pt;
    cs_boolean                      mac_da_range_flag;
    cs_uint8                        age_timer_offset;

}cs_aal_cl_mac_etype_key_t;


/* control-plane packet key */
typedef union {
    struct {
    cs_uint32  mac               : 1;
    cs_uint32  ev2pt             : 1;
    cs_uint32  mask_all_msk      : 1;                   // for always match (mask all field,if the field can be masked. will ignore other msk.)
    cs_uint32  rsvd              : 29;
    } s;
    cs_uint32  u32;
    
}cs_aal_cl_ctrl_pkt_key_msk_t;

typedef struct {
    cs_aal_cl_ctrl_pkt_key_msk_t    mask;               // 1:care;0:no care.
    cs_uint8                        mac_da[6];
    cs_uint8                        macmask;
    cs_uint8                        spl_pkt_en;
    cs_uint32                       special_packet_vec;
    
}cs_aal_cl_ctrl_pkt_key_t;


/* VLAN KEY */
typedef union {
    struct {
    cs_uint32  vlanid            : 1;
    cs_uint32  tpid_enc          : 1;
    cs_uint32  ev2pt_enc         : 1;
    cs_uint32  rsvd_mac_da_flag  : 1;   
    cs_uint32  bc_flag           : 1; 
    cs_uint32  mc_flag           : 1; 
    cs_uint32  p_tag_flag        : 1; 
    cs_uint32  vlan_count        : 1; 
    cs_uint32  bc_llid           : 1; 
    cs_uint32  mask_all_msk      : 1;                   // for always match (mask all field,if the field can be masked. will ignore other msk.)
    cs_uint32  rsvd              : 22;
    } s;
    cs_uint32  u32;
    
}cs_aal_cl_vlan_key_msk_t;

typedef struct {
    cs_aal_cl_vlan_key_msk_t     mask;                  // 1:care;0:no care.
    cs_uint16                    vlanid;
    cs_uint8                     tpid_enc;
    cs_uint8                     ev2pt_enc;
    cs_boolean                   rsvd_mac_da_flag;
    cs_boolean                   bc_flag;
    cs_boolean                   mc_flag;
    cs_boolean                   p_tag_flag;
    cs_uint8                     vlan_count;
    cs_uint8                     bc_llid;
    cs_uint8                     rsvd [2];
    
}cs_aal_cl_vlan_key_t;


/* VLAN RANGE KEY */
typedef union {
    struct {
    cs_uint32  vlan         : 1;
    cs_uint32  ev2pt_enc    : 1;
    cs_uint32  bc_flag      : 1; 
    cs_uint32  mc_flag      : 1; 
    cs_uint32  vlan_count   : 1; 
    cs_uint32  mask_all_msk : 1;                        // for always match (mask all field,if the field can be masked. will ignore other msk.)
    cs_uint32  rsvd         : 26;
    } s;
    cs_uint32  u32;
    
}cs_aal_cl_vlan_range_key_msk_t;

typedef struct {

    cs_aal_cl_vlan_range_key_msk_t    mask;             // 1:care;0:no care.
    cs_uint16                         vlanid_hi;
    cs_uint16                         vlanid_lo;
    cs_boolean                        inner_vlan_en;
    cs_uint8                          ev2pt_enc;   
    cs_boolean                        bc_flag;
    cs_boolean                        mc_flag;
    cs_uint8                          vlan_count;
    cs_uint8                          rsvd[3];

}cs_aal_cl_vlan_range_key_t;


/* _PROTO_KEY */
typedef union {
    struct {
    cs_uint32  tpid_enc        : 1;
    cs_uint32  ipprotocol      : 1;
    cs_uint32  ev2pt           : 1; 
    cs_uint32  sp_mac_flag     : 1;                      /* Used to mask myMAC_flag, rsvd_mac_da_flag,mcst_ip_flag */ 
    cs_uint32  mac_da_low      : 1; 
    cs_uint32  icmp_type       : 1; 
    cs_uint32  prsr_len_excds  : 1; 
    cs_uint32  l4_src_hit_map  : 1; 
    cs_uint32  l4_dest_hit_map : 1; 
    cs_uint32  spcl_hit_map    : 1; 
    cs_uint32  mask_all_msk    : 1;                      // for always match (mask all field,if the field can be masked. will ignore other msk.)
    cs_uint32  rsvd            : 21;
    } s;
    cs_uint32  u32;
    
}cs_aal_cl_proto_key_msk_t;

typedef struct {

    cs_aal_cl_proto_key_msk_t        mask;               // 1:care;0:no care.
    cs_uint8                         tpid_enc;
    cs_uint8                         ipprotocol;
    cs_uint16                        ev2pt;
    cs_boolean                       myMAC_flag;
    cs_boolean                       rsvd_mac_da_flag;
    cs_boolean                       mcst_ip_flag;
    cs_uint8                         l4_src_hit_map;
    cs_uint8                         l4_dest_hit_map;
    cs_uint8                         spcl_hit_map;
    cs_uint8                         mac_da_low;
    cs_uint8                         icmp_type;
    cs_uint8                         prsr_len_excds;
    cs_boolean                       cfi_flag;
    cs_uint8                         rsvd[2];    
    
}cs_aal_cl_proto_key_t;


/* Multicast KEY */
typedef union {
    struct {
    cs_uint32  addr           : 1;
    cs_uint32  vlanId         : 1;
    cs_uint32  prsr_len_excds : 1; 
    cs_uint32  mask_all_msk   : 1;     // for always match (mask all field,if the field can be masked. will ignore other msk.)
    cs_uint32  rsvd           : 28;
    } s;
    cs_uint32  u32;
    
}cs_aal_cl_mcast_key_msk_t;

typedef struct {
    cs_aal_cl_mcast_key_msk_t        mask;            // 1:care;0:no care.
    cs_uint8                         addr[16];
    cs_uint8                         addr_type;
    cs_boolean                       source_addr_vld;
    cs_uint16                        vlanId;
    cs_boolean                       group_id_vld;
    cs_uint8                         group_id;
    cs_uint8                         prsr_len_excds;
    cs_uint8                         rsvd;
}cs_aal_cl_mcast_key_t;


/* FIB */
typedef union {
    struct {
    cs_uint32  dpid        : 1;
    cs_uint32  vlan_op     : 1;
    cs_uint32  dscp        : 1; 
    cs_uint32  dot1p       : 1; 
    cs_uint32  cos         : 1; 
    cs_uint32  l2_lrn      : 1; 
    cs_uint32  rate_limit  : 1; 
    cs_uint32  rsvd        : 25;
    } s;
    cs_uint32  u32;
    
}cs_aal_cl_fib_msk_t;

typedef struct {
    cs_aal_cl_fib_msk_t                mask;                 // 1:care;0:no care.
    cs_boolean                         valid;
    
    cs_boolean                         permit;
    cs_boolean                         permit_filter_dis;
    cs_boolean                         permit_pri;

    /* dpid */
    cs_aal_cl_dpid_t                   dpid;                /* AAL_CL_FIB_MASK_DPID */
    cs_boolean                         cl_mirror_en;        /* AAL_CL_FIB_MASK_DPID */
    cs_aal_cl_mirr_sel_t               cl_mirror_src_sel;   /* AAL_CL_FIB_MASK_DPID */

    /* vlan */
    cs_aal_cl_vlan_cmd_t               inner_vlan_cmd;      /* AAL_CL_FIB_MASK_VLAN_OP */
    cs_aal_cl_tpid_sel_t               inner_tpid_sel;      /* AAL_CL_FIB_MASK_VLAN_OP */
    cs_uint16                          inner_vlanid;        /* AAL_CL_FIB_MASK_VLAN_OP */
    cs_aal_cl_vlanid_sel_t             top_vlanid_sel;      /* AAL_CL_FIB_MASK_VLAN_OP */
    cs_aal_cl_vlan_cmd_t               top_vlan_cmd;        /* AAL_CL_FIB_MASK_VLAN_OP */
    cs_uint16                          top_vlanid;          /* AAL_CL_FIB_MASK_VLAN_OP */
    cs_aal_cl_tpid_sel_t               top_tpid_sel;        /* AAL_CL_FIB_MASK_VLAN_OP */
    cs_aal_cl_eg_vlan_check_t          eg_chk_vlan_sel;     /* AAL_CL_FIB_MASK_VLAN_OP */

    /* dscp */
    cs_aal_cl_dscp_sel_t               dscp_sel;            /* AAL_CL_FIB_MASK_DSCP */
    cs_uint8                           dscp;                /* AAL_CL_FIB_MASK_DSCP */

    /* 802.1p */
    cs_aal_cl_dot1p_sel_t              inner_8021p_sel;     /* AAL_CL_FIB_MASK_DOT1P */
    cs_uint8                           inner_8021p;         /* AAL_CL_FIB_MASK_DOT1P */
    cs_aal_cl_dot1p_sel_t              top_8021p_sel;       /* AAL_CL_FIB_MASK_DOT1P */
    cs_uint8                           top_8021p;           /* AAL_CL_FIB_MASK_DOT1P */

    /* cos */
    cs_aal_cl_cos_sel_t                cos_sel;             /* AAL_CL_FIB_MASK_COS */
    cs_uint8                           cos;                 /* AAL_CL_FIB_MASK_COS */  

    /* l2 lrn */
    cs_boolean                         learn_dis;           /* AAL_CL_FIB_MASK_L2_LRN */
    cs_boolean                         l2_limit_permit;     /* AAL_CL_FIB_MASK_L2_LRN */

    /* rate limit */
    cs_aal_cl_flowid_sel_t             flowId_sel;          /* AAL_CL_FIB_MASK_RATE_LMT */
    cs_uint8                           flowid;              /* AAL_CL_FIB_MASK_RATE_LMT */
    cs_boolean                         cl_rate_limiter_bypass; /* AAL_CL_FIB_MASK_RATE_LMT */
    
}cs_aal_cl_fib_data_t;


/* Default FIB structure */
typedef union {
    struct {
    cs_uint32  permit     : 1;        
    cs_uint32  dpid       : 1;
    cs_uint32  vlan_op    : 1;
    cs_uint32  dscp       : 1; 
    cs_uint32  dot1p      : 1; 
    cs_uint32  cos        : 1; 
    cs_uint32  l2_lrn     : 1; 
    cs_uint32  rate_limit : 1; 
    cs_uint32  rsvd       : 24;
    } s;
    cs_uint32  u32;
    
}cs_aal_cl_def_fib_msk_t;

typedef struct {
    cs_aal_cl_def_fib_msk_t               mask;                 // (default=0) FIB mask control
    cs_boolean                            permit;               // (default=true) Permit/deny packets
    cs_aal_cl_dpid_t                      dpid;                 // (default=AAL_CL_DST_L2) DPID or further handling by L2
    cs_boolean                            cl_mirror_en;         // (default=false) Mirror enable/disable
    cs_aal_cl_mirr_sel_t                  cl_mirror_src_sel;    // (default=AAL_CL_MIRR_SEL_BEFORE_PE) Mirroring selection after/before PE
    cs_aal_cl_vlan_cmd_t                  inner_vlan_cmd;       // (default=AAL_CL_VLAN_CMD_NOP) Inner VLAN command
    cs_aal_cl_tpid_sel_t                  inner_tpid_sel;       // (default=AAL_CL_TPID_SEL_NOP) Inner TPID selection
    cs_uint16                             inner_vlanid;         // (default=0) new Inner VLAN ID
    cs_aal_cl_vlanid_sel_t                top_vlanid_sel;       // (default=AAL_CL_VLANID_SEL_L2) Top VLAN ID selection
    cs_aal_cl_vlan_cmd_t                  top_vlan_cmd;         // (default=AAL_CL_VLAN_CMD_NOP) Top VLAN command
    cs_uint16                             top_vlanid;           // (default=0) new Top VLAN ID
    cs_aal_cl_tpid_sel_t                  top_tpid_sel;         // (default=AAL_CL_TPID_SEL_NOP) Top TPID selection
    cs_aal_cl_eg_vlan_check_t             eg_chk_vlan_sel;      // (default=AAL_CL_EG_VLAN_CHECK_OUT_VLAN) Egress VLAN check selection
    cs_aal_cl_dscp_sel_t                  dscp_sel;             // (default=AAL_CL_DSCP_SEL_ORGINAL_RX) DSCP selection 
    cs_uint8                              dscp;                 // (default=0) New DSCP code
    cs_aal_cl_dot1p_sel_t                 inner_8021p_sel;      // (default=AAL_CL_DOT1P_SEL_ORGINAL_RX) Inner 802.1P selection
    cs_uint8                              inner_8021p;          // (default=0) New Inner 802.1P 
    cs_aal_cl_dot1p_sel_t                 top_8021p_sel;        // (default=AAL_CL_DOT1P_SEL_ORGINAL_RX) Top 802.1P selection
    cs_uint8                              top_8021p;            // (default=0) New Top 802.1P 
    cs_aal_cl_cos_sel_t                   cos_sel;              // (default=AAL_CL_COS_SEL_MAP_PORT_DEF) COS selection
    cs_uint8                              cos;                  // (default=0) CE COS
    cs_boolean                            learn_dis;            // (default=false) disable/enable to force L2 not-learning
    cs_boolean                            l2_limit_permit;      // (default=false) enable/disable to forard traffic if MAC limit reached
    cs_aal_cl_flowid_sel_t                flowId_sel;           // (default=AAL_CL_FLOWID_SEL_NO) Flow-ID selection
    cs_uint8                              flowid;               // 0~31 (default=0) CE flow id
    cs_boolean                            cl_rate_limiter_bypass; // (default=false) enable/disable to bypass the TE rate limit
    cs_uint8                              rsvd;
    
}cs_aal_cl_def_fib_data_t;


typedef struct {
    cs_aal_cl_key_t key_type;
    cs_boolean      valid;
    union {
        cs_aal_cl_ipv4_long_key_t      ipv4_long_key;
        cs_aal_cl_ipv6_long_key_t      ipv6_long_key;
        cs_aal_cl_ipv4_short_key_t     ipv4_short_key[4];
        cs_aal_cl_ipv6_short_key_t     ipv6_short_key;
        cs_aal_cl_mirror_key_t         mirroe_key[8];
        cs_aal_cl_mac_vlan_key_t       mac_vlan_key[4];
        cs_aal_cl_mac_etype_key_t      mac_etype_key[4];
        cs_aal_cl_ctrl_pkt_key_t       ctrl_pkt_key [4];
        cs_aal_cl_vlan_key_t           vlan_key[8];
        cs_aal_cl_vlan_range_key_t     vlan_range_key[8];
        cs_aal_cl_proto_key_t          proco_key[4];
        cs_aal_cl_mcast_key_t          mcast_key[2];
        cs_aal_cl_mac_vlan_key_t       mac_vlan_key_c[4];
        cs_aal_cl_mac_etype_key_t      mac_etype_key_d[4];
        cs_aal_cl_vlan_key_t           vlan_key_e[8];
    }cl_key;
}cs_aal_cl_rule_cfg_t;


typedef union {
    struct {
    cs_uint32   type_ipv4_long_key          : 1;
    cs_uint32   type_ipv4_short_key         : 1;
    cs_uint32   type_ipv6_long_key          : 1;
    cs_uint32   type_ipv6_short_key         : 1;
    cs_uint32   type_mirror_key             : 1;
    cs_uint32   type_mac_vlan_key           : 1;
    cs_uint32   type_mac_etype_key          : 1;
    cs_uint32   type_ctrl_pkt_key           : 1;
    cs_uint32   type_vlan_key               : 1;
    cs_uint32   type_vlan_range_key         : 1;
    cs_uint32   type_proto_key              : 1;
    cs_uint32   type_mcast_key              : 1;
    cs_uint32   type_mac_vlan_key_c         : 1;
    cs_uint32   type_mac_etype_key_d        : 1;
    cs_uint32   type_vlan_key_e             : 1;
    cs_uint32   rscd                        : 17;
     
    }s;
    cs_uint32   wrd;
} cs_aal_cl_type_no_hit_msk_t;


//CE debug 
//Programmable classifier key address for counting entry hit counter 
typedef struct {
    cs_uint8           entry_addr; //(0-47), key entry addresses
    cs_uint32          hit_count;  // Rollover counter 

}cs_aal_cl_debug_prog_ent_cnt_t;


//Debug status register for classifier
typedef struct {
    cs_uint16             hit_fibindex_recent;     // Most recent packet classification first hit index
    cs_uint16             hit_fibindex_previous;   // Previous packet classification hit index 
    cs_uint16             hit_fibindex_earlier;    // hit index for a packet which went through CE two packets earlier.
                                                   // If there is no hit the default entry index will be used. default(0x180(384));
    cs_uint16             fibindex_vlan_rslt;      // Default: 0x1FF,The fib address where the last packets vlan results were taken
    cs_uint16             fibindex_dscp_rslt;      // Default: 0x1FF,The fib address where the last packet's dscp results were taken
    cs_uint16             fibindex_dpid_rslt;      // Default: 0x1FF,The fib address where the last packet's dpid results were taken.
    cs_uint16             fibindex_cos_rslt;       // Default: 0x1FF,The fib address where the last packets cos results were taken.
    cs_uint16             fibindex_l2_lrn_rslt;    // Default: 0x1FF,The fib address where the last packet's l2_lrn results were taken.
    cs_uint16             fibindex_dot1p_rslt;     // Default: 0x1FF,The fib address where the last packet's dot1p results were taken.
    cs_uint16             fibindex_permit_pri_rslt;// Default: 0x1FF,The fib address where the last packet's permit priority results were taken.
    cs_uint16             fibindex_rate_lmt_rslt;  // Default: 0x1FF,The fib address where the last packet's rate limit results were taken
    cs_uint8              entry0_hit_count;        // default:0,Rollover counter which counts the entry hit for key entry address 0
    cs_uint8              entry1_hit_count;        // default:0,Rollover counter which counts the entry hit for key entry address 1
    cs_uint8              entry2_hit_count;        // default:0,Rollover counter which counts the entry hit for key entry address 2
    cs_uint8              entry3_hit_count;        // default:0,Rollover counter which counts the entry hit for key entry address 3
    cs_uint8              entry4_hit_count;        // default:0,Rollover counter which counts the entry hit for key entry address 4
    cs_uint8              entry5_hit_count;        // default:0,Rollover counter which counts the entry hit for key entry address 5
    cs_uint8              entry6_hit_count;        // default:0,Rollover counter which counts the entry hit for key entry address 6
    cs_uint8              entry7_hit_count;        // default:0,Rollover counter which counts the entry hit for key entry address 7
    cs_uint8              entry8_hit_count;        // default:0,Rollover counter which counts the entry hit for key entry address 8
    cs_uint8              entry9_hit_count;        // default:0,Rollover counter which counts the entry hit for key entry address 9
    cs_uint8              entry10_hit_count;       // default:0,Rollover counter which counts the entry hit for key entry address 10
    cs_uint8              entry11_hit_count;       // default:0,Rollover counter which counts the entry hit for key entry address 11
    cs_uint8              entry12_hit_count;       // default:0,Rollover counter which counts the entry hit for key entry address 12
    cs_uint8              entry13_hit_count;       // default:0,Rollover counter which counts the entry hit for key entry address 13
    cs_uint8              entry14_hit_count;       // default:0,Rollover counter which counts the entry hit for key entry address 14
    cs_uint8              entry15_hit_count;       // default:0,Rollover counter which counts the entry hit for key entry address 15
    cs_uint8              rsvd[2];
    
}cs_aal_cl_debug_status_t;



/*****************************************************************************/
/* $rtn_hdr_start   Set domain Configurations for CE services                */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_domain_set(
    CS_IN cs_aal_port_id_t       port,
    CS_IN cs_uint8               domain_num,
    CS_IN cs_aal_cl_domain_res_t *cfg
);
/* INPUTS:      o  port - port index( GE,PON,MA)                             */
/*              o  domain_num  - 1-16                                        */
/*              o  cfg -  domain config parameters                           */
/*                        cs_aal_cl_domain_res_t  ce_domain_res[domain_num]  */
/* OUTPUTS: ----                                                             */
/* typedef struct {                                                          */
/*     cs_uint8          cl_entry_start; //res strart 0-47 for PON and UNI FE, 0-15 for MA FE */
/*     cs_uint8          length; //res length 1-48 for PON and UNI FE, 1-16 for MA FE       */
/* }cs_aal_cl_domain_res_t;                                                  */
/* RETURNS: o  CS_OK: get successfully                                       */
/*          o  CS_ERROR: get failed                                          */
/* DESCRIPTION:                                                              */
/*  This API is used to set per fe_id CE domain configuration                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get domain Configurations for CE services                */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_domain_get(
    CS_IN  cs_aal_port_id_t        port,
    CS_IN  cs_uint8                domain_id,
    CS_OUT cs_aal_cl_domain_res_t  *cfg
);
/* INPUTS:     o  port - port index( GE,PON,MA)                              */
/*             o  domain_id  - 0-15                                          */
/* OUTPUTS:    o  cfg -  domain parameters                                   */
/* typedef struct {                                                          */
/*     cs_uint8          cl_entry_start; //res strart 0-47 for PON and UNI FE, 0-15 for MA FE */
/*     cs_uint8          length; //res length 1-48 for PON and UNI FE, 1-16 for MA FE       */
/* }cs_aal_cl_domain_res_t;                                                  */
/* RETURNS: o  CS_OK: get successfully                                       */
/*          o  CS_ERROR: get failed                                          */
/* DESCRIPTION:                                                              */
/*     This API is used to return domain configuration  per domain_id        */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set Control Configurations for CE                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_ctrl_set(
    CS_IN cs_aal_port_id_t      port,
    CS_IN cs_aal_cl_ctrl_msk_t  cfg_msk,
    CS_IN cs_aal_cl_ctrl_t      *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  cfg_msk - Parameter mask                                  */
/*              o  cfg -   CE  Control parameters                            */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_ERROR: other error occurs                              */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_OK: set successfully                                   */
/* DESCRIPTION:                                                              */
/*             This API is used to configure CE control configurations, configuration fields including: */
/* typedef struct {    */
/*    cs_mac_t          mac_da_high; //MAC for mac_da_range_flag,default :0x33:33:ff:ff:ff:ff----0x33:33:00:01:00:00   */
/*    cs_mac_t          mac_da_low; //  */
/*    cs_boolean        mac_filter_aging_en; //(default= true), enable/disable mac filter aging function       */
/*    cs_boolean        tos_ecn_msk_en; //(default= true), Mask(1) /Enable(0) the IP-TOS ECN bits in the classifier  */
/*    cs_boolean        ce_msk_cfi_flg;//(default= true), Ignore(1)/Compare(0) the cfi_flag in the Classification Key   */
/*}cs_aal_cl_ctrl_t;                                                         */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get Control Configurations for CE                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_ctrl_get(
    CS_IN  cs_aal_port_id_t      port,
    CS_OUT cs_aal_cl_ctrl_t      *cfg
);
/* INPUTS:  o  port - port index( GE,PON,MA)                                 */
/* OUTPUTS: o  cfg - CE control parameters                                   */
/* RETURNS: o  CS_OK: get successfully                                       */
/*          o  CS_ERROR: get failed                                          */
/* DESCRIPTION:                                                              */
/*  This API is used to return per fe_id CE control configuration            */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set L4 port range                                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_l4_port_range_set(
    CS_IN cs_aal_port_id_t               port,
    CS_IN cs_aal_cl_l4_port_type_t       port_type,
    CS_IN cs_uint8                       id,
    CS_IN cs_aal_cl_l4_port_range_t      *cfg
);
/* INPUTS:      o  port - port index( GE,PON,MA)                             */
/*              o  port_type - Source/Destination TCP/UDP port               */
/*              o  id - L4 port range table, 0-7                             */
/*              o  cfg - L4 port range set                                   */
/*                  (default=DISABLE,0~0)                                    */
/* OUTPUTS: ----                                                             */
/* RETURNS:     o  CS_E_PARAM: id value error                                */
/*              o  CS_OK: set successfully                                   */
/*              o  CS_ERROR: Set failed                                      */
/* Default value                                                             */
/*            All value shall be 0                                           */
/* DESCRIPTION:                                                              */
/*            This API is used to configure L4 port range table used for     */
/*            classification.                                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   get L4 port range                                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_l4_port_range_get(
    CS_IN  cs_aal_port_id_t              port,
    CS_IN  cs_aal_cl_l4_port_type_t      port_type,
    CS_IN  cs_uint8                      id,
    CS_OUT cs_aal_cl_l4_port_range_t     *cfg
);
/* INPUTS:      o  port - port index( GE,PON,MA)                             */
/*              o  port_type - Source/Destination TCP/UDP port               */
/*              o  id - L4 port range table, 0-7                             */
/* OUTPUTS:     o  cfg - L4 port range set                                   */
/* RETURNS:     o  CS_OK: get successfully                                   */
/*              o  CS_ERROR: get failed                                      */
/* Default value                                                             */
/*            All value shall be 0                                           */
/* DESCRIPTION:                                                              */
/*            This API is used to retrieve L4 port range table used for      */
/*            classification                                                 */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/*$rtn_hdr_start   Set special packet control field setting                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_spcl_ptn_ctrl_set(
    CS_IN  cs_aal_port_id_t                  port,
    CS_IN  cs_aal_cl_spcl_ptn_control_msk_t  cfg_msk,
    CS_IN  cs_aal_cl_spcl_ptn_control_t      *cfg
);
/* INPUTS   : o  port - port index( GE,PON,MA)                               */
/*            o  cfg_msk - Parameter mask                                    */
/*            o  cfg - spec packet control set                               */
/* OUTPUTS  : -----                                                          */
/* RETURNS  : o  CS_OK: set successfully                                     */
/*            o  CS_ERROR: Set failed                                        */
/* Default value                                                             */
/*            All value shall be 0                                           */
/* DESCRIPTION:                                                              */
/*            This API is used to configure L4 port range table used for     */
/*            classification.  Related structure is defined as following:    */
/* typedef struct {                                                          */
/*     cs_aal_cl_spcl_mode_e         spcl_hdr_mode_sel; // (default=AAL_CL_SPCL_MODE_STA), use static/dynamic mode */
/*     cs_boolean         spcl_hdr_after_l2hdr_en; // (defalt=true), check non-IP packet after L2 header */
/*     cs_boolean         spcl_hdr_after_iphdr_en; // (defalt=true), check IP but non-TCP/UDP packet after L3 header */
/*     cs_boolean         spcl_hdr_after_l4hdr_en; // (defalt=true), check TCP/UDP packet after L4 header */
/*     cs_uint8             spcl_glb_hdr_strt_idx; // 0~127(default=0): under static mode, specify the fixed offset for parsing */
/* }cs_aal_cl_spcl_ptn_control_t;                                            */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set special packet control field setting                 */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_spcl_ptn_ctrl_get(
    CS_IN    cs_aal_port_id_t              port,
    CS_OUT   cs_aal_cl_spcl_ptn_control_t  *cfg
);
/* INPUTS      : o  port - port index( GE,PON,MA)                            */
/* OUTPUTS     : o  cfg - spec packet control set                            */
/* RETURNS     : o  CS_OK: get successfully                                  */
/*               o  CS_ERROR: get failed                                     */
/* DESCRIPTION:                                                              */
/*            This API is used to configure L4 port range table used for     */
/*            classification.                                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set special packet                                       */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_spcl_ptn_entry_set(
    CS_IN   cs_aal_port_id_t              port,
    CS_IN   cs_uint8                      id,
    CS_IN   cs_aal_cl_spcl_ptn_t          *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  id - special packet id, 0-7                               */
/*              o  cfg - special packet identifier configuration             */
/* OUTPUTS  : -----                                                          */
/* RETURNS    : o  CS_OK: set successfully                                   */
/*              o  CS_ERROR: det failed                                      */
/* Default value                                                             */
/*            All value shall be 0                                           */
/* DESCRIPTION:                                                              */
/*            This API is used to configure special packet                   */
/*            classification.                                                */
/* typedef struct { */
/*     cs_aal_cl_spcl_ptn_type_t spcl_ptn_type; // (default=AAL_CL_SPCL_PTN_TYPE_L2) How to match the packet  */
/*     cs_aal_cl_spcl_ptn_cmp_t spcl_ptn_cmp; // (default=AAL_CL_SPCL_PTN_CMP_HI) Compare with lo/hi 16bits from the packets pattern with below key/mask */
/*     cs_uint16             spcl_ptn_key; // (default=0) 16bit key value for speical pattern key */
/*     cs_uint16             spcl_ptn_mask; // (default=0) 16bit key mask for special pattern mask */
/* }cs_aal_cl_spcl_ptn_t;                                                    */ 
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/*$rtn_hdr_start   get special packet                                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_spcl_ptn_entry_get(
    CS_IN   cs_aal_port_id_t           port,
    CS_IN   cs_uint8                   id,
    CS_OUT  cs_aal_cl_spcl_ptn_t       *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  id - special packet id, 0-7                               */
/* OUTPUTS    : o  cfg - special packet identifier configuration             */
/* RETURNS    : o  CS_OK: get successfully                                   */
/*              o  CS_ERROR: get failed                                      */
/* DESCRIPTION:                                                              */
/*            This API is used to retrieve special packet                    */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set aging timer                                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_aging_timer_set(
    CS_IN cs_aal_port_id_t          port,
    CS_IN cs_uint8                  id,
    CS_IN cs_aal_cl_mac_aging_t     *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  id - aging timer id, 0 - 63                               */
/*              o  cfg - aging timer entry                                   */
/*                      (default=INVALID, ZERO TIMER)                        */
/* OUTPUTS : -----                                                           */
/* RETURNS    : o  CS_OK: set successfully                                   */
/*              o  CS_ERROR: set failed                                      */
/* DESCRIPTION:                                                              */
/*            This API is used to configure timer                            */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   get aging timer                                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_aging_timer_get(
    CS_IN  cs_aal_port_id_t          port,
    CS_IN  cs_uint8                  id,
    CS_OUT cs_aal_cl_mac_aging_t     *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  id - aging timer id, 0 - 63                               */
/* OUTPUTS    : o  cfg - aging timer entry                                   */
/* RETURNS    : o  CS_OK: get successfully                                   */
/*              o  CS_ERROR: get failed                                      */
/* DESCRIPTION:                                                              */
/*            This API is used to retrieve timer                             */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set Classification Rule                                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP        : CS8030                                                      */
cs_status aal_cl_rule_set(
    CS_IN cs_aal_port_id_t      port,
    CS_IN cs_uint8              domain_id,
    CS_IN cs_uint8              entry_id,
    CS_IN cs_aal_cl_rule_cfg_t  *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  domain_id  - 0-15                                         */
/*              o  entry_id - classification id     0 base                   */
/*                            id range: 0--(length-1)                        */
/*              o  cfg - classification configuration                        */
/*                       for different key type, each configuration has diff */
/*                       classification rules                                */
/*                       AAL_CL_IPV4_LONG_KEY, 1 conf maps to 1 cls rule     */
/*                       AAL_CL_IPV4_SHORT_KEY,1 conf maps to 4 cls rule     */
/*                       AAL_CL_IPV6_LONG_KEY, 2 conf maps to 1 cls rule     */
/*                       AAL_CL_IPV6_SHORT_KEY,1 conf maps to 1 cls rule     */
/*                       AAL_CL_MIRROR_KEY,    1 conf maps to 8 cls rule     */
/*                       AAL_CL_MAC_VLAN_KEY,  1 conf maps to 4 cls rule     */
/*                       AAL_CL_MAC_ETYPE_KEY, 1 conf maps to 4 cls rule     */
/*                       AAL_CL_CTRL_PKT_KEY,  1 conf maps to 4 cls rule     */
/*                       AAL_CL_VLAN_KEY,      1 conf maps to 8 cls rule     */
/*                       AAL_CL_VLAN_RANGE_KEY,1 conf maps to 8 cls rule     */
/*                       AAL_CL_PROTO_KEY,     1 conf maps to 4 cls rule     */
/*                       AAL_CL_MCST_KEY,      1 conf maps to 2 cls rule     */
/*                       AAL_CL_MAC_VLAN_KEY_C,1 conf maps to 4 cls rule     */
/*                       AAL_CL_MAC_ETYPE_KEY_D,1 conf maps to 4 cls rule    */
/*                       AAL_CL_VLAN_KEY_E,    1 conf maps to 8 cls rule     */
/*                      (default=ALL ZERO)                                   */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_PARAM: fe-id, rule_i, or fe-id rule-id mapping is not*/
/*                             correct.                                      */
/*              o  CS_OK: set successfully                                   */
/*              o  CS_ERROR: Set failed                                      */
/* Default value                                                             */
/*            All fields, valid and mask is each configuration is 0.         */
/* DESCRIPTION:                                                              */
/*            This API is used to configure classification                   */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get Classification Rule                                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_rule_get(
    CS_IN  cs_aal_port_id_t       port,
    CS_IN  cs_uint8               domain_id,
    CS_IN  cs_uint32              entry_id,
    CS_OUT cs_aal_cl_rule_cfg_t   *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  domain_id  - 0-15                                         */
/*              o  entry_id - classification id     0 base                   */
/*                           id range: 0--(length-1)                         */
/* OUTPUTS    : o  cfg - classification configuration                        */
/*                       for different key type, each configuration has diff */
/*                       classification rules, same as set API               */
/*                       AAL_CL_IPV4_LONG_KEY, 1 conf maps to 1 cls rule     */
/*                       AAL_CL_IPV4_SHORT_KEY,1 conf maps to 4 cls rule     */
/*                       AAL_CL_IPV6_LONG_KEY, 2 conf maps to 1 cls rule     */
/*                       AAL_CL_IPV6_SHORT_KEY,1 conf maps to 1 cls rule     */
/*                       AAL_CL_MIRROR_KEY,    1 conf maps to 8 cls rule     */
/*                       AAL_CL_MAC_VLAN_KEY,  1 conf maps to 4 cls rule     */
/*                       AAL_CL_MAC_ETYPE_KEY, 1 conf maps to 4 cls rule     */
/*                       AAL_CL_CTRL_PKT_KEY,  1 conf maps to 4 cls rule     */
/*                       AAL_CL_VLAN_KEY,      1 conf maps to 8 cls rule     */
/*                       AAL_CL_VLAN_RANGE_KEY,1 conf maps to 8 cls rule     */
/*                       AAL_CL_PROTO_KEY,     1 conf maps to 4 cls rule     */
/*                       AAL_CL_MCST_KEY,      1 conf maps to 2 cls rule     */
/*                       AAL_CL_MAC_VLAN_KEY_C,1 conf maps to 4 cls rule     */
/*                       AAL_CL_MAC_ETYPE_KEY_D,1 conf maps to 4 cls rule    */
/*                       AAL_CL_VLAN_KEY_E,    1 conf maps to 8 cls rule     */
/* RETURNS    : o  CS_E_PARAM: fe-id, rule_i, or fe-id rule-id mapping is not*/
/*                             correct.                                      */
/*              o  CS_OK: get successfully                                   */
/*              o  CS_ERROR: Get failed                                      */
/* Default value                                                             */
/*            All fields, valid and mask is each configuration is 0.         */
/* DESCRIPTION:                                                              */
/*            This API is used to configure classification                   */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set Classification FIB                                   */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_fib_set(
    CS_IN cs_aal_port_id_t       port,
    CS_IN cs_uint8               domain_id,
    CS_IN cs_uint16              fib_id,
    CS_IN cs_aal_cl_fib_data_t  *cfg
);
/* INPUTS   :    o  port - port index( GE,PON,MA)                            */
/*               o  domain_id  - 0-15                                        */
/*               o  fib_id - 0~~8*(length)-1                                 */
/*               o  cfg - FIB data                                           */
/*                      (default=INVALID, ALL ZERO)                          */
/* OUTPUTS  : -----                                                          */
/* RETURNS    : o  CS_OK: set successfully                                   */
/*              o  CS_ERROR: set failed                                      */
/* DESCRIPTION:                                                              */
/*            This API is used to configure classification FIB               */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get Classification FIB                                   */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_fib_get(
    CS_IN  cs_aal_port_id_t       port,
    CS_IN  cs_uint8               domain_id,
    CS_IN  cs_uint16              fib_id,
    CS_OUT cs_aal_cl_fib_data_t   *cfg
);
/* INPUTS : o  port - port index( GE,PON,MA)                                 */
/*          o  domain_id  - 0-15                                             */
/*          o  fib_id - 0~~8*(length)-1                                      */
/* OUTPUTS: o  cfg - default FIB                                             */
/* RETURNS: o  CS_OK: get successfully                                       */
/*          o  CS_ERROR: get failed                                          */
/* DESCRIPTION:                                                              */
/*            This API is used to retrieve classification FIB                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Clear Classification rule and FIB  per domain ID         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_domain_rule_clear(
    CS_IN cs_aal_port_id_t       port,
    CS_IN cs_uint8               domain_id
);
/* INPUTS   :  o  port - port index( GE,PON,MA)                              */
/*             o  domain_id  - 0-15                                          */
/* OUTPUTS  : -----                                                          */
/* RETURNS  :  o  CS_OK: clear successfully                                  */
/*             o  CS_ERROR: clear failed                                     */
/* DESCRIPTION:                                                              */
/*  This API is used to clear all classification rule and FIB  per domain ID */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set Classification Default FIB                           */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_default_fib_set(
    CS_IN cs_aal_port_id_t          port,
    CS_IN cs_aal_cl_def_fib_data_t  *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  cfg - FIB data                                            */
/* OUTPUTS    : -----                                                        */
/* RETURNS    : o  CS_OK: set successfully                                   */
/*              o  CS_ERROR: set failed                                      */
/* DESCRIPTION:                                                              */
/*            This API is used to configure classification FIB, which is defined in following structure: */
/* typedef struct { */
/*     cs_uint32                                  mask;                    // (default=0) FIB mask control */
/*     cs_boolean                               permit;                  // (default=true) Permit/deny packets */
/*     cs_aal_cl_portid_t                       dpid;                        // (default=ONU_PORT_ID_CE_L2) DPID or further handling by L2 */
/*     cs_boolean                               cl_mirror_en;         // (default=false) Mirror enable/disable */
/*     cs_aal_cl_mirr_sel_t                    cl_mirror_src_sel;// (default=ONU_MIRR_SEL_BEFORE_PE) Mirroring selection after/before PE */
/*     cs_aal_cl_vlan_cmd_t             inner_vlan_cmd;    // (default=AAL_CL_VLAN_CMD_NOP) Inner VLAN command */
/*     cs_aal_cl_tpid_sel_t                inner_tpid_sel;      // (default=AAL_CL_TPID_SEL_NOP) Inner TPID selection */
/*     cs_uint16                                 inner_vlanid;         // (default=0) new Inner VLAN ID */
/*     cs_aal_cl_vlanid_sel_t             top_vlanid_sel;     // (default=AAL_CL_VLANID_SEL_L2) Top VLAN ID selection */
/*     cs_aal_cl_vlan_cmd_t             top_vlan_cmd;      // (default=AAL_CL_VLAN_CMD_NOP) Top VLAN command */
/*     cs_uint16                                 top_vlanid;           // (default=0) new Top VLAN ID */
/*     cs_aal_cl_tpid_sel_t                top_tpid_sel;       // (default=AAL_CL_TPID_SEL_NOP) Top TPID selection */
/*     cs_aal_cl_tg_vlan_check_t          eg_chk_vlan_sel; // (default=AAL_CL_EG_VLAN_CHECK_OUT_VLAN) Egress VLAN check selection */
/*     cs_aal_cl_dscp_sel_t                   dscp_sel;             // (default=AAL_CL_DSCP_SEL_ORGINAL_RX) DSCP selection  */
/*     cs_uint8                                   dscp;                    // (default=0) New DSCP code */
/*     cs_aal_cl_dot1p_sel_t                  inner_8021p_sel; // (default=AAL_CL_DOT1P_SEL_ORGINAL_RX) Inner 802.1P selection */
/*     cs_uint8                                   inner_8021p;       // (default=0) New Inner 802.1P  */
/*     cs_aal_cl_dot1p_sel_t                  top_8021p_sel;    // (default=AAL_CL_DOT1P_SEL_ORGINAL_RX) Top 802.1P selection */
/*     cs_uint8                                   top_8021p;           // (default=0) New Top 802.1P  */
/*     cs_aal_cl_cos_sel_t                     cos_sel;                // (default=AAL_CL_COS_SEL_MAP_PORT_DEF) COS selection */
/*     cs_uint8                                   cos;                       // (default=0) CE COS */
/*     cs_boolean                               learn_dis;             // (default=false) disable/enable to force L2 not-learning */
/*     cs_boolean                               l2_limit_permit;    // (default=false) enable/disable to forard traffic if MAC limit reached */
/*     cs_aal_cl_flowid_sel_t                  flowId_sel;           // (default=AAL_CL_FLOWID_SEL_NO) Flow-ID selection */
/*     cs_uint8                                   flowid;                  // 0~31 (default=0) CE flow id */
/*     cs_boolean                               cl_rate_limiter_bypass; // (default=false) enable/disable to bypass the TE rate limit */
/*     cs_uint8                                   rsvd; */
/* }cs_aal_cl_def_fib_data_t;                                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get Classification DefaultFIB                            */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_default_fib_get(
    CS_IN  cs_aal_port_id_t          port,
    CS_OUT cs_aal_cl_def_fib_data_t  *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  fib_id - 0~383, the 384th is used for default rule        */
/* OUTPUTS    : o  cfg - default FIB                                         */
/* RETURNS    : o  CS_OK: get successfully                                   */
/*              o  CS_ERROR: get failed                                      */
/* DESCRIPTION:                                                              */
/*            This API is used to retrieve classification FIB                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set Type Non-hit Behavior                                */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_type_hit_set(
    CS_IN cs_aal_port_id_t             port,
    CS_IN cs_aal_cl_type_no_hit_msk_t  *type_msk     
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  type_msk - bit map for each type                          */
/*                      (default=ALL ZERO)                                   */
/* OUTPUTS    : -----                                                        */
/* RETURNS    : o  CS_OK: set successfully                                   */
/*              o  CS_ERROR: set failed                                      */
/* DESCRIPTION:                                                              */
/* This API is used to configure if one type of type is not matched,         */
/*   the packet behavior: drop or not drop. 1 means drop, 0 means not drop   */
/* DEFAULT VALUE: 0                                                          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set Type Non-hit Behavior                                */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_type_hit_get(
    CS_IN  cs_aal_port_id_t             port,
    CS_OUT cs_aal_cl_type_no_hit_msk_t  *type_msk     
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/* OUTPUTS    : o  ctrl_bits - Control bits,                                 */
/* RETURNS    : o  CS_OK: set successfully                                   */
/*              o  CS_ERROR: set failed                                      */
/* DESCRIPTION:                                                              */
/* This API is used to configure if one type of type is not matched,         */
/* the packet behavior: drop or not drop. 1 means drop, 0 means not drop     */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start  Get priority of CE  Entry                                 */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_rule_pri_get(
    CS_IN  cs_aal_port_id_t      port,
    CS_IN  cs_uint8              domain_id,
    CS_IN  cs_uint8              entry_id,
    CS_OUT cs_uint8              *pri
);
/* INPUTS:   o   port - port index( GE,PON,MA)                               */
/*           o   domain_id  - 0-15                                           */
/*           o   entry_id - classification id     0 base                     */
/*                            id range: 0--(length-1)                        */
/* OUTPUTS:  o   pri -     0--(length-1)                                     */
/* RETURNS:  o   CS_OK: set successfully                                     */
/*           o   CS_ERROR: set failed                                        */
/* DESCRIPTION:                                                              */
/*            This API is used to retrieve the pri of CE entry               */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set priority of CE  Entry                                */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_rule_pri_set(
    CS_IN cs_aal_port_id_t      port,
    CS_IN cs_uint8              domain_id,
    CS_IN cs_uint8              entry_id,
    CS_IN cs_uint8              pri
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  domain_id  - 0-15                                         */
/*              o  entry_id - classification id     0 base                   */
/*                            id range: 0--(length-1)                        */
/*              o    pri -     0--(length-1)                                 */
/* OUTPUTS    : -----                                                        */
/* RETURNS    : o  CS_OK: set successfully                                   */
/*              o  CS_ERROR: set failed                                      */
/* DESCRIPTION:                                                              */
/*            This API is used to configure the pri of CE entry              */
/* DEFAULT VALUE: 0                                                          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  aal_cl_mac_filter_aged_latest_entry_get                     */
/* CATEGORY   : Device                                                         */
/* ACCESS     : Public                                                         */
/* BLOCK      : CE                                                             */
/* CHIP       : CS8030                                                         */
cs_status aal_cl_mac_filter_aged_latest_entry_get(
    CS_IN  cs_aal_port_id_t   port,
    CS_OUT cs_uint8           *aging_index
);
/* INPUTS     : o  port - port index( GE,PON,MA)                               */
/* OUTPUTS    : o  aging_index -  the latest aged out entry index              */
/* RETURNS    : o  CS_ERROR: other error occurs                                */
/*              o  CS_E_PARAM: one of parameter is not correct                 */
/*              o  CS_OK: Get successfully                                     */
/* DESCRIPTION:                                                                */
/* Retrieve  the latest aged out entry index due to classifier mac filter      */
/* table aging function.                                                       */
/* The last aged out entry index is latched when aging counter decrements      */
/* from 001 to 000. An interrupt is generated for this entry aged out event    */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  aal_cl_mac_filter_aging_vld_status_get                      */
/* CATEGORY   : Device                                                         */
/* ACCESS     : Public                                                         */
/* BLOCK      : CE                                                             */
/* CHIP       : CS8030                                                         */
cs_status aal_cl_mac_filter_aging_vld_status_get(
    CS_IN  cs_aal_port_id_t   port,
    CS_OUT cs_uint32          *bitmap_high, 
    CS_OUT cs_uint32          *bitmap_low
);
/* INPUTS     : o  port - port index( GE,PON,MA)                               */
/* OUTPUTS    : o  bitmap_high -the latest status of valid bit for index(32--63)*/
/*              o  bitmap_low - the latest status of valid bit for index(0--31)*/
/* RETURNS    : o  CS_ERROR: other error occurs                                */
/*              o  CS_E_PARAM: one of parameter is not correct                 */
/*              o  CS_OK: Get successfully                                     */
/* DESCRIPTION:                                                                */
/* Retrieve  the current status of valid bit in the mac filter aging memory    */
/* for all entries                                                             */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get CE debug status                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_debug_status_get(
    CS_IN  cs_aal_port_id_t         port,
    CS_OUT cs_aal_cl_debug_status_t *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/* OUTPUTS    : o  cfg - CE debug parameters                                 */
/* RETURNS    : o  CS_ERROR: other error occurs                              */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_OK: Get successfully                                   */
/* DESCRIPTION:                                                              */
/*             This API is used to get CE debug info                         */
/* typedef struct { */
/*    cs_uint16            hit_fibindex_recent; // Most recent packet classification first hit index*/
/*    cs_uint16            hit_fibindex_previous;// Previous packet classification hit index */
/*    cs_uint16            hit_fibindex_earlier; //hit index for a packet which went through CE two packets earlier.*/
/*    // If there is no hit the default entry index will be used. default(0x180(384));*/
/*    cs_uint16            fibindex_vlan_rslt; //Default: 0x1FF,The fib address where the last packets vlan results were taken*/
/*    cs_uint16            fibindex_dscp_rslt; //Default: 0x1FF,The fib address where the last packet's dscp results were taken*/
/*    cs_uint16            fibindex_dpid_rslt;//Default: 0x1FF,The fib address where the last packet's dpid results were taken.*/
/*    cs_uint16            fibindex_cos_rslt; //Default: 0x1FF,The fib address where the last packets cos results were taken.*/
/*    cs_uint16            fibindex_l2_lrn_rslt;//Default: 0x1FF,The fib address where the last packet's l2_lrn results were taken.*/
/*    cs_uint16            fibindex_dot1p_rslt; //Default: 0x1FF,The fib address where the last packet's dot1p results were taken.*/
/*    cs_uint16            fibindex_permit_pri_rslt;//Default: 0x1FF,The fib address where the last packet's permit priority results were taken.*/
/*    cs_uint16            fibindex_rate_lmt_rslt;//Default: 0x1FF,The fib address where the last packet's rate limit results were taken*/
/*    cs_uint8              entry0_hit_count; //default:0,Rollover counter which counts the entry hit for key entry address 0*/
/*    cs_uint8              entry1_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 1*/
/*    cs_uint8              entry2_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 2*/
/*    cs_uint8              entry3_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 3*/
/*    cs_uint8              entry4_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 4*/
/*    cs_uint8              entry5_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 5*/
/*    cs_uint8              entry6_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 6*/
/*    cs_uint8              entry7_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 7*/
/*    cs_uint8              entry8_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 8*/
/*    cs_uint8              entry9_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 9*/
/*    cs_uint8              entry10_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 10*/
/*    cs_uint8              entry11_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 11*/
/*    cs_uint8              entry12_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 12*/
/*    cs_uint8              entry13_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 13 */
/*    cs_uint8              entry14_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 14*/
/*    cs_uint8              entry15_hit_count;//default:0,Rollover counter which counts the entry hit for key entry address 15  */
/*}cs_aal_cl_debug_status_t;                                                 */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start Set Programmable classifier key address                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_debug_prog_count_set(
    CS_IN cs_aal_port_id_t               port,
    CS_IN cs_aal_cl_debug_prog_ent_cnt_t *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  cfg - CE debug Programmable parameters                    */
/* OUTPUTS    : -----                                                        */
/* RETURNS    : o  CS_OK: set successfully                                   */
/*              o  CS_ERROR: set failed                                      */
/* DESCRIPTION:                                                              */
/*       This API is used to configure classifier key address                */
/*       for counting entry hit counter                                      */
/* typedef struct {                                                          */
/*  cs_uint8            entry_addr; //(0-47), key entry addresses            */
/*}cs_aal_cl_debug_prog_ent_cnt_t;                                           */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get counter which counts hit for a programmable entry    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : CE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_cl_debug_prog_count_get(
    CS_IN  cs_aal_port_id_t                port,
    CS_OUT cs_aal_cl_debug_prog_ent_cnt_t  *cfg
);
/* INPUTS   :  o  port - port index( GE,PON,MA)                              */
/* OUTPUTS  :  o  cfg - CE debug Programmable parameters                     */
/* RETURNS  :  o  CS_OK: get successfully                                    */
/*             o  CS_ERROR: get failed                                       */
/* DESCRIPTION:                                                              */
/* This API is used to retrieve counter which counts the entry hit           */
/* for a programmable entry address                                          */
/* typedef struct {                                                          */
/*  cs_uint8            entry_addr; //(0-47), key entry addresses            */
/*  cs_uint32          hit_count; // Rollover counter                        */  
/*}cs_aal_cl_debug_prog_ent_cnt_t;                                           */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/




#endif /* __AAL_CLS_H__ */

