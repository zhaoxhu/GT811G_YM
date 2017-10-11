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

    This file is used for l2 function code except classification. (vlan, fdb, special packet and port forwarding)
    
*/



#ifndef __AAL_L2_H__
#define __AAL_L2_H__


#include "cs_types.h"
#include "aal.h"



/* Special Packet DPID Resolution */
typedef enum {
     AAL_SPEC_DST_PORT           = 0,  /* cs_aal_port_id_t port;     */
     AAL_SPEC_DST_DROP           = 1,  /* Blackhole port (dropped);  */  
     AAL_SPEC_DST_FE             = 2   /* indicating the DPID should be further decided from FE DPID resolution.          */
                                       /* dpid shall be used instead of the Classification result or the DPID from the L2 table */
}cs_aal_spec_dst_op_t;

 
typedef struct {   
    cs_aal_spec_dst_op_t   dst_op;
    cs_aal_port_id_t       dpid;       // valid when op = AAL_SPEC_DST_PORT
    
} cs_aal_spec_pkt_dpid_t;


/* op, port from cs_aal_spec_pkt_dpid_t, dpid is cs_aal_dpid_t */
#define SPEC_PORT_TO_DPID(op, port, dpid)                do{        \
    if(op == 1)                                                     \
    {                                                               \
        dpid = 3;                                                   \
    }                                                               \
    else if (op == 2)                                               \
    {                                                               \
        dpid = 7;                                                   \
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


#define SPEC_DPID_TO_PORT(dpid, op, port)                do{        \
    if(dpid == 3)                                                   \
    {                                                               \
        op = 1;                                                     \
    }                                                               \
    else if (dpid == 7)                                             \
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


 /* Mirror source selection */
typedef enum{
     AAL_MIRR_SEL_BEFORE_PE = 0,  /* Do packet mirroring before packet editing */
     AAL_MIRR_SEL_AFTER_PE  = 1,  /* Do packet mirroring after packet editing  */
    
}cs_aal_mirr_sel_t;

        
/* Special Packet Behavior structure */
typedef union {
    struct {
    cs_uint32  pri           : 1;
    cs_uint32  cos           : 1;
    cs_uint32  dpid          : 1;
    cs_uint32  learning      : 1;
    cs_uint32  lmt_fwd       : 1;
    cs_uint32  bypass_plc    : 1;
    cs_uint32  rate_limit    : 1;
    cs_uint32  mirror        : 1;
    cs_uint32  permit        : 1;
    cs_uint32  rsvd          : 23;
    } s;
    cs_uint32  u32;
    
}cs_aal_spec_pkt_ctrl_msk_t;


typedef struct {
    cs_uint8                     pri;           // 0~0x1f(refer to below default values): Packet identification priority (0x1f takes the highest precedence);        
                                                // If more than 1 have same priority, the behavior
                                                // could not be a definite behavior  
                                                  
    cs_boolean                   bypass_plc;    // (default=false) whether bypass the policing or not 
    cs_boolean                   lmt_fwd;       // (default=false) Whether or not to forward this packet if learning is enabled but address 
                                                //  pool is full or limited when new special packet arrives           
    cs_boolean                   learning;      // (refer to below default values) whether learning the source MAC                                  
    cs_uint8                     cos;           // 0~7(default=0): Internal COS 

    cs_aal_spec_pkt_dpid_t       dpid;          // (refer to below default values) the detination port ID                         
              
    cs_boolean                   flow_en;       // (default=false) enable to disbale to use the flow id to do rate limit 
    cs_uint8                     flow_id;       // (default=0) 0~63: Flow id to do rate limit for this special packet 
    cs_boolean                   mirror_en;     // (default=false): whether or not to enable the mirror for this special packet 
    cs_aal_mirr_sel_t            mirror_src_sel;// (default=AAL_MIRR_SEL_BEFORE_PE): do mirroing after/before packet editing
    cs_boolean                   permit;        // (default=true),Permit/Drop this special packet type,ignored if sp_dpid is 3'h7
  
} cs_aal_spec_pkt_ctrl_t;
/* default behavior regarding each type is as following:                                                */
/* Note: when DPID == 7({2, 0}), Learning,lmt_fwd,cos,bypass_plc,flow,mirror, permit will be ignored.   */
/*       they will be from CL_RSLT or others(L2,PORT).                                                  */
/*        PKTtype                PRI     DPID    Learning    LimitForward  COS   rlimBypass             */
/* AAL_PKT_NORMAL                 1       7         1            0          0           0               */
/* AAL_PKT_BPDU                  27       7         0            0          0           0               */
/* AAL_PKT_8021X                 30       7         0            0          0           1               */
/* AAL_PKT_IEEE_RSVD1            22       7         0            0          0           0               */
/* AAL_PKT_MYMAC                 29       7         0            0          6           0               */
/* AAL_PKT_MC_A                  21       7         1            0          0           0               */
/* AAL_PKT_MC_B                  20       7         1            0          0           0               */
/* AAL_PKT_MC_C                  19       7         1            0          0           0               */
/* AAL_PKT_IGMP                  26       7         1            0          0           0               */
/* AAL_PKT_ARP                   24       7         1            0          0           0               */
/* AAL_PKT_OAM                   31       4         0            0          7           1               */
/* AAL_PKT_MPCP                  28       3         0            0          0           0               */
/* AAL_PKT_DHCP                  18       7         1            0          0           0               */
/* AAL_PKT_PPPOE_DIS             25       7         1            0          0           0               */
/* AAL_PKT_PPPOE_SESSION         24       7         1            0          0           0               */
/* AAL_PKT_HELLO                 23       7         0            0          6           0               */
/* AAL_PKT_IPV6NDP               22       7         1            0          0           0               */
/* AAL_PKT_UDF0                   2       7         0            0          0           0               */
/* AAL_PKT_UDF1                   3       7         0            0          0           0               */
/* AAL_PKT_UDF2                   4       7         0            0          0           0               */
/* AAL_PKT_UDF3                   5       7         0            0          0           0               */


/* Special Packet UDF pattern structure */
typedef union {
    struct {
    cs_uint32  udf0_etype   : 1;
    cs_uint32  udf0_chk_da  : 1;
    cs_uint32  udf0_da      : 1;
    cs_uint32  udf1_etype   : 1;
    cs_uint32  udf1_chk_da  : 1;
    cs_uint32  udf1_da      : 1;
    cs_uint32  udf2_etype   : 1;
    cs_uint32  udf3_etype   : 1;
    cs_uint32  udf4_etype   : 1;
    cs_uint32  hello_da     : 1;
    cs_uint32  rsvd         : 22;
    } s;
    cs_uint32  u32;
    
}cs_aal_pkt_spec_udf_msk_t;


typedef struct {
    cs_uint16               udf0_etype; // (default=0x0802) UDF0 ethertype (also the ev2pt encoding value 0xA for classificaiton) 
    cs_mac_t                udf0_da;    // (default=00:00:00:00:00:00) UDF0 DA MAC 
    cs_boolean              udf0_chk_da;// (default=false) enable or disable to check DA MAC to decare a UDF0 
                                        //  enable:  UDF0 is detected when etype and DA matched 
                                        //  disable: UDF0 is detected when only etype matched 
    cs_uint16               udf1_etype; // (default=0x0801) UDF1 ethertype (also the ev2pt encoding value 0xB for classificaiton) 
    cs_boolean              udf1_chk_da;// (default=false) enable or disable to check DA MAC to decare a UDF1 
                                        //  enable:  UDF1 is detected when etype and DA matched 
                                        //  disable: UDF1 is detected when only etype matched 
    cs_mac_t                udf1_da;    // (default=00:00:00:00:00:00) UDF1 DA MAC 

    cs_uint16               udf2_etype; // (default=0x0804) UDF2 ethertype (also the ev2pt encoding value 0xC for classificaiton) 
    cs_uint16               udf3_etype; // (default=0x0803) UDF3 ethertype (also the ev2pt encoding value 0xD for classificaiton) 
    cs_uint16               udf4_etype; // (default=0x0805) UDF4 ethertype (also the ev2pt encoding value 0xE for classificaiton) 

    cs_mac_t                hello_da;   // (default=00:00:00:00:00:00) Hello DA MAC, A packet is detected as HELLO when etype=0xffff and DA matched 

    
} cs_aal_pkt_spec_udf_t;


typedef enum{
    AAL_TO_TE_ACTUAL = 0,  /* giving the actual packet length after VLAN commands to TE for updating token */
    AAL_TO_TE_INCOME = 1,  /* giving the incoming packet length to TE for updating token */
  
}cs_aal_te_pkt_len_t;


// the FCS calculation's configuration
typedef enum{
    AAL_FCS_CALC_NOT_CRC32 = 0, /* using NOT(CRC-32) as FCS */
    AAL_FCS_CALC_CRC32     = 1, /* using CRC-32 as FCS; */
  
}cs_aal_fcs_mode_t;


/* Packet FE ctrl structure */
typedef union {
    struct {
    cs_uint32  pe_ge_en         : 1;
    cs_uint32  pe_pon_en        : 1;
    cs_uint32  pe_cpu_en        : 1;
    cs_uint32  pe_mii0_en       : 1;
    cs_uint32  pe_mii1_en       : 1;
    cs_uint32  ingress_pkt_len  : 1;
    cs_uint32  inner_def_dot1p  : 1;
    cs_uint32  top_def_8021p    : 1;
    cs_uint32  txpad_en         : 1;
    cs_uint32  stomp_crc        : 1;
    cs_uint32  keep_fcs         : 1;
    cs_uint32  loop_dpid_deny   : 1;
    cs_uint32  loop_mac_chk     : 1;
    cs_uint32  loop_vlan_chk    : 1;
    cs_uint32  loop_deny        : 1;
    cs_uint32  rsvd             : 17;
    } s;
    cs_uint32  u32;
}cs_aal_pkt_fe_ctrl_msk_t;


typedef struct {
    cs_boolean          pe_ge_en;       // (default=true),Enable/disable all packet editing functions when the resolved dpid is GE 
    cs_boolean          pe_pon_en;      // (default=true),Enable/disable all packet editing functions when the resolved dpid is pon 
    cs_boolean          pe_cpu_en;      // (default=false),Enable/disable all packet editing functions when the resolved dpid is cpu
    cs_boolean          pe_mii0_en;     // (default=true),Enable/disable all packet editing functions when the resolved dpid is mii0 
    cs_boolean          pe_mii1_en;     // (default=true),Enable/disable all packet editing functions when the resolved dpid is mii1 
    cs_aal_te_pkt_len_t ingress_pkt_len;// (default=AAL_TO_TE_ACTUAL),packet length to TE for updating token
    cs_uint8            inner_def_dot1p;// (default=0),the default 8021p for INNER_PUSH_CMD
    cs_uint8            top_def_8021p;  // (default=0),the default 8021p for TOP_PUSH_CMD
    cs_boolean          txpad_en;       // (default=false),pad or no change outgoing packet if less than 64B
    cs_aal_fcs_mode_t   stomp_crc;      // (default=AAL_FCS_CALC_NOT_CRC32),the FCS calculation's configuration
    cs_boolean          keep_fcs;       // (default=false), keep or replace the incoming FCS
    cs_boolean          loop_dpid_deny; // (default=true),enable or disable to drop packet if SPID==DPID
    cs_boolean          loop_vlan_chk;  // (default=false).enable or disable to check if ingress vlan equal to egress vlan for loop
    cs_boolean          loop_mac_chk;   // (default=false).enable or disable to check if mac DA equal to mac SA for loop
                                        // for enable: perform real checking; for disable: alwasy treat VID or MAC as equal (only check SPID and DPID)
    cs_boolean          loop_deny;      // (default=false),enable or disable to drop packet if the spid == dpid and ingrss vlan == egress vlan       
    cs_uint8            rsvd;
    
} cs_aal_pkt_fe_ctrl_t;


/* cpu info type selection */
typedef enum{
    AAL_CPU_HEADINFO_FLOW_ID   = 0, /*5-bit information in the packet header of packets destined to the CPU is the flow ID */
    AAL_CPU_HEADINFO_SPEK_TYPE = 1, /*5-bit information in the packet header of packets destined to the CPU is the special packet type*/
    
}cs_aal_cpu_info_type_t;


/* Port Default parameters*/
typedef union {
    struct {
    cs_uint32    vlanid       : 1 ;
    cs_uint32    cos          : 1 ;
    cs_uint32    dot1p        : 1 ;
    cs_uint32    dscp         : 1 ;
    cs_uint32    tpid         : 1 ;
    cs_uint32    rsvd         : 27;
    } s;
    cs_uint32    u32;
    
} cs_aal_port_dft_msk_t;


typedef struct {
    cs_uint16    vlanid;   // (default=0): configurable default port vlan id
    cs_uint8     cos;      // (default=0): configurable default port cos
    cs_uint8     dot1p;    // (default=0): configurable default port 802.1p value
    cs_uint8     dscp;     // (default=0): configurable default port dscp value
    cs_uint16    tpid;     // (default=0x8100): configurable default port tpid
    
} cs_aal_port_dft_cfg_t;


/* Port configuration structure */
typedef union {
    struct {
    cs_uint32    ipv4_dscp_mask           : 1;
    cs_uint32    ipv6_tc_mask             : 1;
    cs_uint32    tx_def_vid_pop           : 1;
    cs_uint32    rx_def_vid_swap          : 1;
    cs_uint32    cpu_header_pkt_type      : 1;
    cs_uint32    oam_lpbk_en              : 1;
    cs_uint32    dbg_da_sa_swap_en        : 1;
    cs_uint32    uuc_dpid                 : 1;
    cs_uint32    uuc_fwd_cos              : 1;
    cs_uint32    bc_dpid                  : 1;
    cs_uint32    bc_fwd_cos               : 1;
    cs_uint32    mc_dpid                  : 1;
    cs_uint32    mc_fwd_cos               : 1;
    cs_uint32    rsvd                     : 19;
    } s;
    cs_uint32    u32;
    
}cs_aal_port_msk_t;


typedef struct {
    cs_uint16               ipv4_dscp_mask;       //  0~0x3f(default=0): DSCP protect bitmap (for each bit: 0 for permit packet editing, 1 for keep the packet bit value)
    cs_uint16               ipv6_tc_mask;         //  0~0x3f(default=0): TC protect bitmap (for each bit: 0 for permit packet editing, 1 for keep the packet bit value)
    cs_boolean              tx_def_vid_pop;       //  (default=false): enable/disable to pop top egress vlan tag if the final TX top vlanid is default VID
    cs_boolean              rx_def_vid_swap;      //  (default=false):enable/disable this function:*/
                                                  //  When the top vlanid of the incoming packet matches with the port default vlanid,
                                                  //  and if the final vlan command for this packet is push then the packet
                                                  //  edit engine pops the incoming top vlanid and pushes the new vlanid back as the top outgoing vlan. 
                                                  
    cs_aal_cpu_info_type_t  cpu_header_pkt_type;  //  (default=special packet type), the type of 5-bit information in the packet header of packets destined to the CPU  
    cs_boolean              oam_lpbk_en;          //  (default=false): enable the RX2TX loopback for packets coming into this port. OAM is always not looped back
    cs_boolean              dbg_da_sa_swap_en;    //  (default=false): enable/disable the swap of DA and SA MAC for packets coming into this port, debug fot loopback function
    cs_aal_spec_pkt_dpid_t  uuc_dpid;             //  (default=AAL_DPID_PON for GE port, AAL_DPID_GE for PON port, AAL_DPID_CPU for other ports) unknown unicast forward control
    cs_uint8                uuc_fwd_cos;          //  0~7(default=7): cos for unknown unicast
    cs_aal_spec_pkt_dpid_t  bc_dpid;              //  (default=AAL_DPID_PON for GE port, AAL_DPID_GE for PON port, AAL_DPID_CPU for other ports) broadcast forward control
    cs_uint8                bc_fwd_cos;           //  0~7(default=7): cos for broadcast
    cs_aal_spec_pkt_dpid_t  mc_dpid;              //  (default=AAL_DPID_PON for GE port, AAL_DPID_GE for PON port, AAL_DPID_CPU for other ports) multicast forward control
    cs_uint8                mc_fwd_cos;           //  0~7(default=7): cos for multicast
    cs_uint8                rsvd; 

}cs_aal_port_cfg_t;


/**************************************** fdb module ******************************/
typedef enum {
    AAL_FDB_LEARN_HW       = 0, /* hardware learning mode   */
    AAL_FDB_LEARN_SW       = 1, /* software learning mode   */

} cs_aal_fdb_learn_t;


typedef enum {
    AAL_FDB_PERMIT_NONE    = 0, /* deny DA match, deny SA match     */
    AAL_FDB_PERMIT_DA      = 1, /* permit DA match, deny SA match   */
    AAL_FDB_PERMIT_SA      = 2, /* deny DA match, permit SA match   */
    AAL_FDB_PERMIT_DA_SA   = 3, /* permit DA match, permit SA match */
    
} cs_aal_fdb_permit_t;


typedef enum { 
    AAL_FDB_VLAN_LRN_RX_TOP      = 0, /* select receive packet's top vlanid for tagged packet or
                                         source port's default vlanid for un-tagged and priority tagged packets as the vlanid for layer 2 source address
                                         learning, received packet 802.1P field in the top VLAN header or
                                         source port's default 802.1P for untagged packets*/
    AAL_FDB_VLAN_LRN_TX_TOP      = 1, /* select egress top vlanid/802.1p (outgoing top vlanid) after vlan
                                         resolution as the vlanid field for l2 learning, for un-tagged outgoing packet, use destination port's 
                                         default vlanid (PVID) and default 802.1P,for priority tagged outgoing packet use destination port's
                                         default vlanid (PVID) and 802.1P from the VLAN header  */
    AAL_FDB_VLAN_LRN_TX_INNER    = 2, /* select egress inner vlanid/802.1p (outgoing inner vlanid) after vlan
                                         resolution as the vlanid field for l2 learning, for single tagged or untagged same as AAL_FDB_VLAN_LRN_TX_TOP */
    AAL_FDB_VLAN_LRN_RX_INNER    = 3, /* select receive packet's inner vlanid/802.1p for 2 or more vlan tagged packet 
                                         for single tagged or untagged same as AAL_FDB_VLAN_LRN_RX_TOP  */
    
} cs_aal_fdb_vlan_lrn_t;


typedef enum {
    AAL_FDB_VLAN_LKUP_RX_TOP     = 0, /* select receiving packet's top vlanid/802.1p for tagged packet or
                                         source port's default vlanid for un-tagged / priority
                                         tagged packets as the vlanid  */
    AAL_FDB_VLAN_LKUP_RX_INNER   = 1, /* select receiving inner vlanid/802.1p, 
                                         for single tagged or untagged same as AAL_FDB_VLAN_LKUP_RX_TOP */
    
} cs_aal_fdb_vlan_lkup_t;


typedef enum {
    AAL_FDB_GE_VLN_CFG0      = 0, /* GE port vlan 0  */
    AAL_FDB_GE_VLN_CFG1      = 1, /* GE port vlan 1  */
    AAL_FDB_GE_VLN_CFG2      = 2, /* GE port vlan 2  */
    AAL_FDB_GE_VLN_CFG3      = 3, /* GE port vlan 3  */
    
} cs_aal_fdb_vlan_mac_t;


/* Global fdb configurations */
typedef union {
    struct {
    cs_uint32  lrn_mode                 : 1;
    cs_uint32  aging_en                 : 1;
    cs_uint32  aging_cntr               : 1;
    cs_uint32  aging_intvl              : 1;
    cs_uint32  station_move_enable      : 1;
    cs_uint32  station_move_drop        : 1;
    cs_uint32  st_loop_thrshld          : 1;
    cs_uint32  st_loop_detect_drop      : 1;
    cs_uint32  rsvd                     : 24;
    } s;
    cs_uint32  u32;
    
}cs_aal_fdb_msk_t;


typedef struct {
    cs_aal_fdb_learn_t  learn_mode;          //  (default=AAL_L2_LEARN_HW): Hardware or software learning mode
    cs_boolean          aging_en;            //  (default=true) enable or disable aging
    cs_uint16           aging_cntr;          //  1~0xfff(default=4095): initial aging counter for each entry. It refers to how many aging intervals.
    cs_uint32           aging_intvl;         //  1~0xffffffff(default=143080): aging interval
                                             //  Final aging time = aging_cntr * aging_intvl * 64 * 8ns
    cs_boolean          station_move_en;     //  (default=true): enable or disable station move operation in l2 mac table
    cs_boolean          station_move_drop;   //  (default=false): enable or disable the drop when station move is disabled but station move happens
                                             //  true: drop, false: continue forwarding but invalidate the L2 entry
    cs_uint16           st_loop_thrshld;     //  0..4095(default=4082): station loop detection threshold in unit of 1/4095
                                             //  When station move happens and the FDB entry lives for smaller than
                                             //  (aging_cntr -station_loop_thrshld), it will be detected as
                                             //  station loop. If this value is bigger than aging_cntr, loop detect doesn't work.
    cs_boolean          st_loop_detect_drop; //  (default=false) enable or disable to drop of looping packets
    cs_uint8            rsvd[3];
      
}cs_aal_fdb_cfg_t;


/* FDB  table entry */
typedef struct {
    cs_boolean         vld;                /* false: invalid fdb entry, true:valid fdb entry */
    cs_mac_t           mac;                /* mac address learned             */
    cs_uint8           dot1p;              /* dot1p leanred                   */
    cs_uint16          vid;                /* vlan id leanred                 */
    cs_boolean         mac_vld;            /* valid indicator for mac address */
    cs_boolean         vid_vld;            /* valid indicator for vlan id     */
    cs_boolean         dot1p_vld;          /* valid indicator for dot1p       */
    cs_boolean         static_flag;        /* false: dynamic entry, true: dynamic entry */
    cs_aal_port_id_t   port_id;            /* 0 - 4, source port id */
    cs_boolean         permit_da_ge;       /* false: destination address deny, true: destination address permit */
    cs_boolean         permit_sa_ge;       /* false: source address deny, true: source address permit   */
    cs_boolean         permit_da_pon;      /* false: destination address deny, true: destination address permit */
    cs_boolean         permit_sa_pon;      /* false: source address deny, true: source address permit   */
    cs_boolean         permit_da_cpu;      /* false: destination address deny, true: destination address permit */
    cs_boolean         permit_sa_cpu;      /* false: source address deny, true: source address permit   */
    cs_boolean         permit_da_mii0;     /* false: destination address deny, true: destination address permit */
    cs_boolean         permit_sa_mii0;     /* false: source address deny, true: source address permit   */
    cs_boolean         permit_da_mii1;     /* false: destination address deny, true: destination address permit */
    cs_boolean         permit_sa_mii1;     /* false: source address deny, true: source address permit   */
    cs_uint16          aging_status;       /* 0 - 0xfff, 0 means aged out, <aging_cntr> means entry newly learned or entry hit refresh */
   
      
}cs_aal_fdb_entry_t;


/* FDB  GE vlan based  mac limit */
typedef struct {  
    cs_boolean                      vld;              // false: invalid vlan mac limit entry, true:valid vlan mac limit entry 
    cs_uint8                        mac_limit;        // 0-0x40(default=16): max entries can be learned
    cs_uint16                       mac_limited_vlan; // compared with the outermost VLAN of packets. If there is a match, the VLAN based MAC address limit is checked.
    
}cs_aal_vlan_mac_entry_t;


/* FDB Port configuration structure */
typedef union {
    struct {
    cs_uint32  learn_dis            : 1;
    cs_uint32  learn_lmt_ovrwrt     : 1;
    cs_uint32  learn_dpid_cpu_dis   : 1;
    cs_uint32  learn_dpid_drop_dis  : 1;
    cs_uint32  pkt_fwd_dis          : 1;
    cs_uint32  learn_vlan_sel       : 1;
    cs_uint32  lookup_vlan_sel      : 1;
    cs_uint32  sa_drop_en           : 1;
    cs_uint32  learn_mac_vld        : 1;
    cs_uint32  learn_vlan_vld       : 1;
    cs_uint32  learn_dot1p_vld      : 1;
    cs_uint32  lookup_mac_vld       : 1;
    cs_uint32  lookup_vlan_vld      : 1;
    cs_uint32  lookup_dot1p_vld     : 1;
    cs_uint32  mac_match_ctrl       : 1;
    cs_uint32  sa_max_limit         : 1;
    cs_uint32  sa_lmt_drop_en       : 1;
    cs_uint32  rsvd                 : 15;
    } s;
    cs_uint32  u32;
    
}cs_aal_fdb_port_msk_t;


typedef struct {
    cs_boolean             learn_dis;          // (default=false for GE, true for other ports): disallow(true) or allow(false) learning on this port 
    cs_boolean             learn_lmt_ovrwrt;   // (default=false): enable/disable to overwrite the eldest entry if MAC limit reached
    cs_boolean             learn_dpid_cpu_dis; // (default=true): disallow(true) or allow(false) learning for TO CPU packets coming into this port
    cs_boolean             learn_dpid_drop_dis;// (default=true): disallow(true) or allow(false) learning for DROPPING packets coming into this port
    cs_boolean             pkt_fwd_dis;        // (default=false): drop(true) or forward(false) all packets destined TO this port except for those CPU sending
    cs_aal_fdb_vlan_lrn_t  learn_vlan_sel;     // (default=AAL_L2_VLAN_LRN_RX_TOP): vlan id selection for l2 mac table learning
    cs_aal_fdb_vlan_lkup_t lookup_vlan_sel;    // (default=AAL_L2_VLAN_LKUP_RX_TOP): vlan id selection for l2 mac table lkup
    cs_boolean             sa_drop_en;         // (default=true),Enable(true) or disable dropping of packets when L2 Engine decides to drop a packet because of
                                               // MAC SA checks or Station Move.For oam_lpbk. 
    cs_boolean             learn_mac_vld;      // (default=true): enable/disable to learn MAC
    cs_boolean             learn_vlan_vld;     // (default=false): enable/disable to learn VLAN
    cs_boolean             learn_dot1p_vld;    // (default=false): enable/disable to learn dot1p
    cs_boolean             lookup_mac_vld;     // (default=true): enable/disable to lookup MAC
    cs_boolean             lookup_vlan_vld;    // (default=false): enable/disable to lookup VLAN
    cs_boolean             lookup_dot1p_vld;   // (default=false): enable/disable to lookup dot1p    
    cs_aal_fdb_permit_t    mac_match_ctrl;     // (default=AAL_FDB_PERMIT_DA_SA), Source address, destination address pemit/deny control bits for port.    
    cs_uint8               sa_max_limit;       // 0-0x40(default=64): max address entries can be learned
    cs_boolean             sa_lmt_drop_en;     // (default=false): enable or disable to drop packets if SA limit reached
    
}cs_aal_fdb_port_cfg_t;


/* Forwarding engine interrupt enable*/
typedef union {
    struct {
#ifdef CS_BIG_ENDIAN 
    cs_uint32  evt_pkt_len_mismatch_err_ge      : 1;
    cs_uint32  evt_pkt_len_mismatch_err_pon     : 1;
    cs_uint32  evt_pkt_len_mismatch_err_ma      : 1;
    cs_uint32  evt_pkt_len_runt_err_ge          : 1;
    cs_uint32  evt_pkt_len_runt_err_pon         : 1;
    cs_uint32  evt_pkt_len_runt_err_ma          : 1;
    cs_uint32  evt_ce_mac_filter_aged_out_ge    : 1;
    cs_uint32  evt_ce_mac_filter_aged_out_pon   : 1;
    cs_uint32  evt_ce_mac_filter_aged_out_ma    : 1;
    cs_uint32  evt_pkt_len_err_ge               : 1;
    cs_uint32  evt_pkt_len_err_pon              : 1;
    cs_uint32  evt_pkt_len_err_ma               : 1;
    cs_uint32  evt_lat_fifo_overflow_ge         : 1;
    cs_uint32  evt_lat_fifo_overflow_pon        : 1;
    cs_uint32  evt_lat_fifo_overflow_ma         : 1;
    cs_uint32  evt_fdb_entry_aged_out           : 1;
    cs_uint32  evt_fdb_no_avail_dyn_entry_learn : 1;
    cs_uint32  evt_fdb_ovr_max_limit_ge         : 1;
    cs_uint32  evt_fdb_ovr_max_limit_pon        : 1;
    cs_uint32  evt_fdb_ovr_max_limit_cpu        : 1;
    cs_uint32  evt_fdb_ovr_max_limit_mii0       : 1;
    cs_uint32  evt_fdb_ovr_max_limit_mii1       : 1;
    cs_uint32  evt_fdb_stn_mov_ge               : 1;
    cs_uint32  evt_fdb_stn_mov_pon              : 1;
    cs_uint32  evt_fdb_stn_mov_cpu              : 1;
    cs_uint32  evt_fdb_stn_mov_mii0             : 1;
    cs_uint32  evt_fdb_stn_mov_mii1             : 1;
    cs_uint32  evt_fdb_learn_ge                 : 1;
    cs_uint32  evt_fdb_learn_pon                : 1;
    cs_uint32  evt_fdb_learn_cpu                : 1;
    cs_uint32  evt_fdb_learn_mii0               : 1;
    cs_uint32  evt_fdb_learn_mii1               : 1;
#else
    cs_uint32  evt_fdb_learn_mii1               : 1;
    cs_uint32  evt_fdb_learn_mii0               : 1;
    cs_uint32  evt_fdb_learn_cpu                : 1;
    cs_uint32  evt_fdb_learn_pon                : 1;
    cs_uint32  evt_fdb_learn_ge                 : 1;
    cs_uint32  evt_fdb_stn_mov_mii1             : 1;
    cs_uint32  evt_fdb_stn_mov_mii0             : 1;
    cs_uint32  evt_fdb_stn_mov_cpu              : 1;
    cs_uint32  evt_fdb_stn_mov_pon              : 1;
    cs_uint32  evt_fdb_stn_mov_ge               : 1;
    cs_uint32  evt_fdb_ovr_max_limit_mii1       : 1;
    cs_uint32  evt_fdb_ovr_max_limit_mii0       : 1;
    cs_uint32  evt_fdb_ovr_max_limit_cpu        : 1;
    cs_uint32  evt_fdb_ovr_max_limit_pon        : 1;
    cs_uint32  evt_fdb_ovr_max_limit_ge         : 1;
    cs_uint32  evt_fdb_no_avail_dyn_entry_learn : 1;
    cs_uint32  evt_fdb_entry_aged_out           : 1;
    cs_uint32  evt_lat_fifo_overflow_ma         : 1;
    cs_uint32  evt_lat_fifo_overflow_pon        : 1;
    cs_uint32  evt_lat_fifo_overflow_ge         : 1;
    cs_uint32  evt_pkt_len_err_ma               : 1;
    cs_uint32  evt_pkt_len_err_pon              : 1;
    cs_uint32  evt_pkt_len_err_ge               : 1;
    cs_uint32  evt_ce_mac_filter_aged_out_ma    : 1;
    cs_uint32  evt_ce_mac_filter_aged_out_pon   : 1;
    cs_uint32  evt_ce_mac_filter_aged_out_ge    : 1;
    cs_uint32  evt_pkt_len_runt_err_ma          : 1;
    cs_uint32  evt_pkt_len_runt_err_pon         : 1;
    cs_uint32  evt_pkt_len_runt_err_ge          : 1;
    cs_uint32  evt_pkt_len_mismatch_err_ma      : 1;
    cs_uint32  evt_pkt_len_mismatch_err_pon     : 1;
    cs_uint32  evt_pkt_len_mismatch_err_ge      : 1;
#endif
#ifdef CS_BIG_ENDIAN 
    cs_uint32  evt_fdb_stn_mov_loop_det_drop    : 1;
    cs_uint32  rsvd                             : 31;
#else
    cs_uint32  rsvd                             : 31;
    cs_uint32  evt_fdb_stn_mov_loop_det_drop    : 1;
#endif
    } s;
    cs_uint32  u32[2];
    
}cs_aal_fe_int_msk_t;

#if 0
typedef struct {
    cs_boolean   evt_pkt_len_mismatch_err_ge_en;      // (default=true),Control for enable/disable of GE port packet length error interrupt
    cs_boolean   evt_pkt_len_mismatch_err_pon_en;     // (default=true),Control for enable/disable of PON port packet length error interrupt
    cs_boolean   evt_pkt_len_mismatch_err_ma_en;      // (default=true),Control for enable/disable of MA port packet length error interrupt
                                                      // while the packet length in header-a doesn't match the actual packet length, this interrupt is asserted;
    cs_boolean   evt_pkt_len_runt_err_ge_en;          // (default=true),Control for enable/disable of GE port packet length error interrupt
    cs_boolean   evt_pkt_len_runt_err_pon_en;         // (default=true),Control for enable/disable of PON port packet length error interrupt
    cs_boolean   evt_pkt_len_runt_err_ma_en;          // (default=true),Control for enable/disable of MA port packet length error interrupt;
                                                      // while a runt packet is delivered to FE module, this interrupt is asserted;
    cs_boolean   evt_ce_mac_filter_aged_out_ge_en;    // (default=true),Control for enable/disable of GE mac filter aging out interrupt
    cs_boolean   evt_ce_mac_filter_aged_out_pon_en;   // (default=true),Control for enable/disable of PON mac filter aging out interrupt
    cs_boolean   evt_ce_mac_filter_aged_out_ma_en;    // (default=true),Control for enable/disable of MA mac filter aging out interrupt
    cs_boolean   evt_pkt_len_err_ge_en;               // (default=true),Control for enable/disable of GE port packet length error interrupt
    cs_boolean   evt_pkt_len_err_pon_en;              // (default=true),Control for enable/disable of PON port packet length error interrupt
    cs_boolean   evt_pkt_len_err_ma_en;               // (default=true),Control for enable/disable of MA port packet length error interrupt
    cs_boolean   evt_lat_fifo_overflow_ge_en;         // (default=true),Control for enable/disable of GE latency FIFO's overflow interrupt
    cs_boolean   evt_lat_fifo_overflow_pon_en;        // (default=true),Control for enable/disable of PON latency FIFO's overflow interrupt
    cs_boolean   evt_lat_fifo_overflow_ma_en;         // (default=true),Control for enable/disable of MA latency FIFO's overflow interrupt
    cs_boolean   evt_fdb_entry_aged_out_en;           // (default=false),Control for enable/disable of layer 2 dynamic entry aging out event by hardware.
    cs_boolean   evt_fdb_no_avail_dyn_entry_learn_en; // (default=false),Control for enable/disable of unavailability of dynamic entries for hardware learning event.
    cs_boolean   evt_fdb_ovr_max_limit_ge_en;         // (default=false),Control for enable/disable learning event over the maximum allowed source address learning limit interrupt from ge port
    cs_boolean   evt_fdb_ovr_max_limit_pon_en;        // (default=false),Control for enable/disable learning event over the maximum allowed source address learning limit interrupt from pon port
    cs_boolean   evt_fdb_ovr_max_limit_cpu_en;        // (default=false),Control for enable/disable learning event over the maximum allowed source address learning limit interrupt from cpu port
    cs_boolean   evt_fdb_ovr_max_limit_mii0_en;       // (default=false),Control for enable/disable learning event over the maximum allowed source address learning limit interrupt from MII0 port
    cs_boolean   evt_fdb_ovr_max_limit_mii1_en;       // (default=false),Control for enable/disable learning event over the maximum allowed source address learning limit interrupt from MII1 port
    cs_boolean   evt_fdb_stn_mov_ge_en;               // (default=false),Control for enable/disable of layer 2 station move event interrupt from GE port
    cs_boolean   evt_fdb_stn_mov_pon_en;              // (default=false),Control for enable/disable of layer 2 station move event interrupt from pon port
    cs_boolean   evt_fdb_stn_mov_cpu_en;              // (default=false),Control for enable/disable of layer 2 station move event interrupt from cpu port
    cs_boolean   evt_fdb_stn_mov_mii0_en;             // (default=false),Control for enable/disable of layer 2 station move event interrupt from mii0 port
    cs_boolean   evt_fdb_stn_mov_mii1_en;             // (default=false),Control for enable/disable of layer 2 station move event interrupt from mii1 port
    cs_boolean   evt_fdb_learn_ge_en;                 // (default=false),Control for enable/disable of layer 2 learning event interrupt from GE port
    cs_boolean   evt_fdb_learn_pon_en;                // (default=false),Control for enable/disable of layer 2 learning event interrupt from PON port
    cs_boolean   evt_fdb_learn_cpu_en;                // (default=false),Control for enable/disable of layer 2 learning event interrupt from CPU port
    cs_boolean   evt_fdb_learn_mii0_en;               // (default=false),Control for enable/disable of layer 2 learning event interrupt from MII0 port
    cs_boolean   evt_fdb_learn_mii1_en;               // (default=false),Control for enable/disable of layer 2 learning event interrupt from MII1 port
    cs_boolean   evt_fdb_stn_mov_loop_det_drop_en;    // (default=true),Control for enable/disable of layer 2 engine station move loop detect packet drop interrupt;
    cs_uint8     rsvd[3]; 
    
}cs_aal_fe_int_cfg_t;


/* Debug status */

typedef struct {
    cs_boolean   evt_pkt_len_mismatch_err_ge_sts;     // Status of GE port packet length error interrupt
    cs_boolean   evt_pkt_len_mismatch_err_pon_sts;    // Status of PON port packet length error interrupt
    cs_boolean   evt_pkt_len_mismatch_err_ma_sts;     // Status of MA port packet length error interrupt
                                                      // while the packet length in header-a doesn't match the actual packet length, this interrupt is asserted;
    cs_boolean   evt_pkt_len_runt_err_ge_sts;         // Status of GE port packet length error interrupt
    cs_boolean   evt_pkt_len_runt_err_pon_sts;        // Status of PON port packet length error interrupt
    cs_boolean   evt_pkt_len_runt_err_ma_sts;         // Status of MA port packet length error interrupt
                                                      // while a runt packet is delivered to FE module, this interrupt is asserted;
    cs_boolean   evt_ce_mac_filter_aged_out_ge_sts;   // Status of GE classification engine mac filter entry aging out interrupt.
    cs_boolean   evt_ce_mac_filter_aged_out_pon_sts;  // Status of PON classification engine mac filter entry aging out interrupt.
    cs_boolean   evt_ce_mac_filter_aged_out_ma_sts;   // Status of MA classification engine mac filter entry aging out interrupt.
    cs_boolean   evt_pkt_len_err_ge_sts;              // Status of GE port packet length error interrupt
    cs_boolean   evt_pkt_len_err_pon_sts;             // Status of PON port packet length error interrupt
    cs_boolean   evt_pkt_len_err_ma_sts;              // Status of MA port packet length error interrupt
    cs_boolean   evt_lat_fifo_overflow_ge_sts;        // Status of GE latency FIFO's overflow interrupt
    cs_boolean   evt_lat_fifo_overflow_pon_sts;       // Status of PON latency FIFO's overflow interrupt
    cs_boolean   evt_lat_fifo_overflow_ma_sts;        // Status of MA latency FIFO's overflow interrupt
    cs_boolean   evt_fdb_entry_aged_out_sts;          // Status of layer 2 layer 2 dynamic entry aging out event.
    cs_boolean   evt_fdb_no_avail_dyn_entry_learn_sts;// Status of layer 2 unavailability of dynamic entries for hardware learning event.
    cs_boolean   evt_fdb_ovr_max_limit_ge_sts;        // Status of learning event over the maximum allowed source address learning limit interrupt from ge port
    cs_boolean   evt_fdb_ovr_max_limit_pon_sts;       // Status of learning event over the maximum allowed source address learning limit interrupt from pon port
    cs_boolean   evt_fdb_ovr_max_limit_cpu_sts;       // Status of learning event over the maximum allowed source address learning limit interrupt from cpu port
    cs_boolean   evt_fdb_ovr_max_limit_mii0_sts;      // Status of learning event over the maximum allowed source address learning limit interrupt from MII0 port
    cs_boolean   evt_fdb_ovr_max_limit_mii1_sts;      // Status of learning event over the maximum allowed source address learning limit interrupt from MII1 port
    cs_boolean   evt_fdb_stn_mov_ge_sts;              // Status of station move event interrupt from GE port
    cs_boolean   evt_fdb_stn_mov_pon_sts;             // Status of station move event interrupt from pon port
    cs_boolean   evt_fdb_stn_mov_cpu_sts;             // Status of station move event interrupt from cpu port
    cs_boolean   evt_fdb_stn_mov_mii0_sts;            // Status of station move event interrupt from mii0 port
    cs_boolean   evt_fdb_stn_mov_mii1_sts;            // Status of station move event interrupt from mii1 port
    cs_boolean   evt_fdb_learn_ge_sts;                // Status of learning event interrupt from GE port
    cs_boolean   evt_fdb_learn_pon_sts;               // Status of learning event interrupt from PON port
    cs_boolean   evt_fdb_learn_cpu_sts;               // Status of learning event interrupt from CPU port
    cs_boolean   evt_fdb_learn_mii0_sts;              // Status of learning event interrupt from MII0 port
    cs_boolean   evt_fdb_learn_mii1_sts;              // Status of learning event interrupt from MII1 port
    cs_boolean   evt_fdb_stn_mov_loop_det_drop_sts;   // Status of station move loop detect packet drop interrupt;
    cs_uint8     rsvd[3]; 
    
}cs_aal_fe_int_status_t;
#endif

typedef struct {
    cs_uint16  lrn_cnt_ge;            // 0 - 0x3f, learning counter for ge port   
    cs_uint16  lrn_cnt_pon;           // 0 - 0x3f, learning counter for pon port 
    cs_uint16  lrn_cnt_cpu;           // 0 - 0x3f, learning counter for cpu port  
    cs_uint16  lrn_cnt_mii0;          // 0 - 0x3f, learning counter for mii0 port 
    cs_uint16  lrn_cnt_mii1;          // 0 - 0x3f, learning counter for mii1 port 
    
    cs_uint16  ge_vln0_lrn_cnt;       // 0 - 0x3f, learning counter for GE port vlan 0 based 
    cs_uint16  ge_vln1_lrn_cnt;       // 0 - 0x3f, learning counter for GE port vlan 1 based 
    cs_uint16  ge_vln2_lrn_cnt;       // 0 - 0x3f, learning counter for GE port vlan 2 based 
    cs_uint16  ge_vln3_lrn_cnt;       // 0 - 0x3f, learning counter for GE port vlan 3 based 
    
    cs_uint16  station_move_idx_ge;   // 0 - 0x3f, l2 table hit index for station move events 
    cs_uint16  station_move_idx_pon;  // 0 - 0x3f, l2 table hit index for station move events 
    cs_uint16  station_move_idx_cpu;  // 0 - 0x3f, l2 table hit index for station move events 
    cs_uint16  station_move_idx_mii0; // 0 - 0x3f, l2 table hit index for station move events 
    cs_uint16  station_move_idx_mii1; // 0 - 0x3f, l2 table hit index for station move events 
      
    cs_uint16  aging_idx;             // 0 - 0x3f, indicates lastest aged out entry index due to l2 table aging function 
    cs_uint8   rsvd[2]; 
    
} cs_aal_fdb_status_t;


/**************************************** vlan module ******************************/

typedef enum {
    AAL_VLAN_IDX_MIN          = 0,
    AAL_VLAN_IDX_FIRST        = 0,  /* The first VLAN member index   */
    AAL_VLAN_IDX_LAST         = 31, /* The last VLAN member index   */
    AAL_VLAN_IDX_DEF_INGRESS  = 32, /* The default ingress member pseudo index */
    AAL_VLAN_IDX_DEF_EGRESS   = 33, /* The default egress member pseudoindex   */
  
} cs_aal_vlan_idx_t;


/* VLAN Configuration structure */
typedef struct {
 
    cs_boolean  parse_inner_8100;    // (default=true):  enable or disable to parse/check packet with inner TPID=0x8100
    cs_boolean  parse_outer_88a8;    // (default=false): enable or disable to parse/check packet with outer TPID=0x88a8      
    cs_boolean  parse_inner_tpid;    // (default=false): enable or disable to parse/check packet with inner TPID=<inner_tpid>       
    cs_boolean  parse_outer_tpid1;   // (default=false): enable or disable to parse/check packet with outer TPID=<outer_tpid1>
    cs_boolean  parse_outer_tpid2;   // (default=false): enable or disable to parse/check packet with outer TPID=<outer_tpid2>
    cs_boolean  deny_inner_8100;     // (default=false): enable or disable to drop packet with inner TPID is parsed and equal to 0x8100
    cs_boolean  deny_outer_88a8;     // (default=false): enable or disable to drop packet with outer TPID is parsed and equal to 0x88a8
    cs_boolean  deny_inner_tpid;     // (default=false): enable or disable to drop packet with inner TPID is parsed and equal to <inner_tpid>
    cs_boolean  deny_outer_tpid1;    // (default=false): enable or disable to drop packet with outer TPID is parsed and equal to <outer_tpid1>
    cs_boolean  deny_outer_tpid2;    // (default=false): enable or disable to drop packet with outer TPID is parsed and equal to <outer_tpid2>
    cs_uint16   ingress_inner_tpid;  // (default=0x8100): ingress inner TPID
    cs_uint16   ingress_outer_tpid1; // (default=0x8100): ingress outer TPID #1
    cs_uint16   ingress_outer_tpid2; // (default=0x9100): ingress outer TPID #2 
    cs_boolean  ingress_mapping_flag;// (default = 0) Control to turn on(1) /off (0) ingress vlan mapping function in the vlan table
    cs_boolean  top_vlan_l2_en;      // (default = 0),Control to replace top VLAN with the VLAN ID from VLAN based MAC limit
    cs_uint8    rsvd[2]; 
    
} cs_aal_vlan_cfg_t;


typedef union {
    struct {
    cs_uint32  parse_inner_8100       : 1;
    cs_uint32  parse_outer_88a8       : 1;
    cs_uint32  parse_inner_tpid       : 1;
    cs_uint32  parse_outer_tpid1      : 1;
    cs_uint32  parse_outer_tpid2      : 1;
    cs_uint32  deny_inner_8100        : 1;
    cs_uint32  deny_outer_88a8        : 1;
    cs_uint32  deny_inner_tpid        : 1;
    cs_uint32  deny_outer_tpid1       : 1;
    cs_uint32  deny_outer_tpid2       : 1;
    cs_uint32  ingress_inner_tpid     : 1;
    cs_uint32  ingress_outer_tpid1    : 1;
    cs_uint32  ingress_outer_tpid2    : 1;
    cs_uint32  ingress_mapping_flag   : 1;
    cs_uint32  top_vlan_l2_en         : 1;
    cs_uint32  rsvd                   : 17;
    } s;
    cs_uint32  u32;
    
}cs_aal_vlan_cfg_msk_t;


typedef struct {
    cs_uint16   vid_lo;         /* vlan id range low for vlan filtering;  for vlan mpping, used for mapped vlan id */
    cs_uint16   vid_hi;         /* vlan id range high for vlan filtering;  for vlan mapping, used for original vlan id */
    cs_boolean  permit_ge;      /* permit control for giga port     */
    cs_boolean  permit_pon;     /* permit control for pon port  */
    cs_boolean  permit_mii0;    /* permit control for mii0 port */
    cs_boolean  permit_mii1;    /* permit control for mii1 port */
    cs_boolean  ingress_filter; /* enable or disable ingress filter, ignored for default VLAN member */
    cs_boolean  egress_filter;  /* enable or disable egress filter, ignored for default VLAN member */
    cs_boolean  mapping_flag;   /* Used for vlan mapping (translation), 1 mapping, 0 no mapping  */
    cs_uint8    rsvd;
    
} cs_aal_vlan_entry_t;


/**************************************** other module ******************************/

typedef enum{
    AAL_MIRR_WEIGHT_BASE_32B  = 0, //32Bytes weight base
    AAL_MIRR_WEIGHT_BASE_64B  = 1, //64Bytes weight base
    AAL_MIRR_WEIGHT_BASE_128B = 2, //128Bytes weight base
    AAL_MIRR_WEIGHT_BASE_256B = 3, //256Bytes weight base
 
}cs_aal_mirr_weight_base_t;


/* Global Mirror configurations */
typedef struct {
    cs_aal_mirr_weight_base_t drr_weight_base;  // (default=AAL_L2_MIRR_WEI_32B): mirroring base
    cs_uint8                  drr_ge_weight;    // 0~0xff(default=64): the DRR weight for mirroring packets from GE port
    cs_uint8                  drr_pon_weight;   // 0~0xff(default=64): the DRR weight for mirroring packets from PON port
    cs_uint8                  drr_ma_weight;    // 0~0xff(default=64): the DRR weight for mirroring packets from MA port   
    cs_boolean                enhance_drr;      // (default=disable) enable/disable the enhacned DRR scheduling
    cs_boolean                l2_mirror_en;     // (default=enable) enable/disable the L2 mirroring function for software learning mode.
                                                // This bit is for debug purposes only and must not be changed.
    cs_uint8                  l2_mirror_groupid;// 0~0x1f(default=0)
    cs_boolean                gl_mirror_en;     // (default=disable), enables/disable mirror port function.
                                                //  Packet memory has a dedicated slice for mirror port FIFO
}cs_aal_mirr_cfg_t;


typedef union {
    struct {
    cs_uint32  drr_ge_weight       : 1;
    cs_uint32  drr_pon_weight      : 1;
    cs_uint32  drr_ma_weight       : 1;
    cs_uint32  drr_weight_base     : 1;
    cs_uint32  enhance_drr         : 1;
    cs_uint32  l2_mirror_groupid   : 1;
    cs_uint32  gl_mirror_en        : 1;
    cs_uint32  rsvd                : 25;
    } s;
    cs_uint32  u32;
    
}cs_aal_mirr_msk_t;


/* Port mirror parameters*/
typedef struct {
    cs_boolean              umc_mirror_en;     // (default=disable) enable/disable the unknown multicast packets mirroring function
    cs_aal_mirr_sel_t       umc_mirror_src_sel;// (default=0),mirror_src_sel flag,the packet may be copied before or after packet modification
    cs_uint8                umc_mirror_groupid;// 0~0x1f(default=0)
    cs_boolean              uuc_mirror_en;     // (default=disable) enable/disable the unknown unicast packets mirroring function
    cs_aal_mirr_sel_t       uuc_mirror_src_sel;// (default=0),mirror_src_sel flag,the packet may be copied before or after packet modification
    cs_uint8                uuc_mirror_groupid;// 0~0x1f(default=0)
    cs_boolean              bc_mirror_en;      // (default=disable) enable/disable the broadcast packets mirroring function
    cs_aal_mirr_sel_t       bc_mirror_src_sel; // (default=0),mirror_src_sel flag,the packet may be copied before or after packet modification
    cs_uint8                bc_mirror_groupid; //  0~0x1f(default=0)
    
} cs_aal_mirror_port_cfg_t;


typedef union {
    struct {
    cs_uint32  umc_mirror_en       : 1;
    cs_uint32  umc_mirror_src_sel  : 1;
    cs_uint32  umc_mirror_groupid  : 1;
    cs_uint32  uuc_mirror_en       : 1;
    cs_uint32  uuc_mirror_src_sel  : 1;
    cs_uint32  uuc_mirror_groupid  : 1;
    cs_uint32  bc_mirror_en        : 1;
    cs_uint32  bc_mirror_src_sel   : 1;
    cs_uint32  bc_mirror_groupid   : 1;
    cs_uint32  rsvd                : 23;      
    } s;
    cs_uint32 u32;
    
} cs_aal_mirror_port_cfg_msk_t;

/* dpid redirect configurations */
typedef struct {
    cs_boolean                   dpid_redirect1_en;       // (default=disable),enable/disable redirect1 forwarding function
    cs_aal_spec_pkt_dpid_t       redirect1_fwd_dpid;      // redirect 1 forwarding destination portid
    cs_aal_spec_pkt_dpid_t       redirect1_original_dpid; // redirect 1 compare destinatin port
    cs_boolean                   dpid_redirect0_en;       // (default=disable),enable/disable redirect0 forwarding function
    cs_aal_spec_pkt_dpid_t       redirect0_fwd_dpid;      // redirect 0 forwarding destination portid
    cs_aal_spec_pkt_dpid_t       redirect0_original_dpid; // redirect 0 compare destinatin port
    
} cs_aal_redirect_cfg_t;


typedef union {
    struct {
    cs_uint32  dpid_redirect1_en        : 1;
    cs_uint32  redirect1_fwd_dpid       : 1;
    cs_uint32  redirect1_original_dpid  : 1;
    cs_uint32  dpid_redirect0_en        : 1;
    cs_uint32  redirect0_fwd_dpid       : 1;
    cs_uint32  redirect0_original_dpid  : 1;
    cs_uint32  rsvd                     : 26;   
    } s;
    cs_uint32  u32;
    
} cs_aal_redirect_cfg_msk_t;


/* debug control configurations */
typedef struct {   
    cs_boolean   ge_mask_rxmac_drop_en;  // (default=true), enable don't capture the rxmac drop source
    cs_boolean   pon_mask_rxmac_drop_en; // (default=true),enable don't capture the rxmac drop source
    cs_boolean   ma_mask_rxmac_drop_en;  // (default=true),enable don't capture the rxmac drop source
    cs_boolean   dbg_cntr_clr_on_read;   // (default=true)Control for debug packet counters clear on read(1:Clear on read;0:Sample mode).
    
} cs_aal_debug_ctrl_t;


typedef union {
    struct {
    cs_uint32  ge_mask_rxmac_drop_en  : 1;
    cs_uint32  pon_mask_rxmac_drop_en : 1;
    cs_uint32  ma_mask_rxmac_drop_en  : 1;
    cs_uint32  dbg_cntr_clr_on_read   : 1;
    cs_uint32  rsvd                   : 28;   
      
    } s;
    cs_uint32  u32;
    
} cs_aal_debug_ctrl_msk_t;


typedef struct {       
    cs_uint32  pkt_drop_cnt_ge_fe;  // the counter of dropped packets for GE_FE
    cs_uint32  pkt_drop_cnt_pon_fe; // the counter of dropped packets for PON_FE
    cs_uint32  pkt_drop_cnt_ma_fe;  // the counter of dropped packets for MA_FE
    cs_uint32  pkt_cnt_ingress_ge;  // rollover counter for valid end of packet received from the port Mac or MA module
    cs_uint32  pkt_cnt_ingress_pon; // rollover counter for valid end of packet received from the port Mac or MA module
    cs_uint32  pkt_cnt_ingress_ma;  // rollover counter for valid end of packet received from the port Mac or MA module
    cs_uint32  pkt_cnt_egress_ge;   // rollover counter for valid end of packet be sent out of FE module
    cs_uint32  pkt_cnt_egress_pon;  // rollover counter for valid end of packet be sent out of FE module
    cs_uint32  pkt_cnt_egress_ma;   // rollover counter for valid end of packet be sent out of FE module
    
} cs_aal_pkt_fe_cnt_sts_t;


typedef struct {
   cs_uint16    vlan_rsltn_st;  // Destination vlan resolution state. 
   cs_uint8     dscp_rsltn_st;  // Destination dscps resolution state.
   cs_uint8     cos_rsltn_st;   // Destination cos resolution state
   cs_uint16    dot1p_rsltn_st; // Destination 802.1p resolution state
   cs_uint8     dpid_rsltn_st;  // Destination port resolution state. 
   cs_uint8     rsvd;
   
} cs_aal_pkt_rsltn_state_t;


typedef struct {
    cs_uint8         Pop_no_top_vlan_cnt;   // The top VLAN command is POP, but the incoming packet without VLAN tag
    cs_uint8         Swap_np_top_vlan_cnt;  // The top VLAN command is SWAP, but the incoming packet without VLAN tag
    cs_uint8         Pop_no_inner_vlan_cnt; // The inner VLAN command is POP, but the incoming packet without inner VLAN tag
    cs_uint8         swap_no_inner_vlan_cnt;// The inner VLAN command is SWAP, but the incoming packet without inner VLAN tag
    cs_uint8         Push_no_inner_vlan_cnt;// The inner VLAN command is PUSH, but the incoming packet without VLAN tag
    cs_uint8         Runt_pkt_cnt;          // Count the runt packet after VLAN command
    cs_uint8         rsvd[2];
    
}cs_aal_pe_vlan_cmd_err_cnt_t;


typedef struct {
    cs_uint32    pp_ni_header_capture_0; // [`HDRA_WIDTH-1:0] header,capture the header at the PP-NI interface while SOP is asserted
    cs_uint32    pp_ni_header_capture_1; // [`HDRA_WIDTH-1:0] header,capture the header at the PP-NI interface while EOP is asserted
                                         // capture the information at the PP-PR interface 
    cs_uint32    pp_pr_info_capture_2;   // [31] "1" ping-pong fifo in Parser is overflow,[19:0] special packet flags
    cs_uint32    pp_pr_info_capture_3;   //  info[31:0]      
                                         //  [31] ptp_packet flag
                                         //  [30] mirrored packet indication
                                         //  [29:25] group_id for mirrored packet
                                         //  [24:22] spid[21] tpid_drop_flag
                                         //  [20:9] pp_pr_hdr_sts
                                         //  [11] HDRA_PKT_BCLLID_FLD
                                         //  [10] HDRA_PKT_BYPASS_FLD
                                         //  [9] 0,
                                         //  while the incoming packet is bypass-packet, pp_pr_hdr_sts[5:0] is valid;otherwise, pp_pr_hdr_sts[8:6] is valid.
                                         //  [8:6] HDRA_MPCP_NRM_GATE_FRM_FLD, HDRA_MPDP_DSCVR_FRM_FLD,HDRA_MPCP_RGSTR_FRM_FLD
                                         //  [5:3] HDRA_DPID_FLD
                                         //  [2:0] HDRA_COS_FLD
                                         //  [8] parsing exceeded flag
                                         //  [7:6] ingress vlan cnt
                                         //  [5] ingress top priority vlan flag
                                         //  [4:3] L3 type: 2'b00 ipv4; 2'b01 ipv6; 2'b10 others
                                         //  [2:0] l4 type: 3'b000 tcp; 3'b001 udp; 3'b010 icmp; 3'b011 igmp; 3'b100 others 
    
} cs_aal_pp_info_capture_t;


/* capture the information at the PR-PE interface */ 
typedef struct {  
    cs_mac_t          da;
    cs_mac_t          sa;
    cs_boolean        da_sa_swap;
    cs_uint8          ptype;
    cs_uint8          top_tpid_encode;
    cs_uint8          inner_tpid_encode;  
    cs_uint16         inner_ingress_tpid;
    cs_uint16         top_ingress_tpid; 
    cs_boolean        top_pri_vlan_flag;
    cs_uint8          vlan_num;
    cs_uint8          top_cmd;
    cs_uint8          inner_cmd;
    cs_uint16         inner_vid;
    cs_uint16         top_vid;
    cs_boolean        top_802_1p_update;
    cs_uint8          top_802_1p;
    cs_boolean        inner_802_1p_update;
    cs_uint8          inner_802_1p;
    cs_uint8          ip_type;
    cs_boolean        dscp_update;
    cs_uint8          dscp;
    cs_boolean        rlim_byp;        // rate limit bypass flag 
    cs_boolean        flow_en;
    cs_uint8          flow_id;
    cs_aal_mirr_sel_t mirror_src_sel;
    cs_uint8          mirror_pkt_flag;
    cs_boolean        lat_fifo_pe_drop;
    cs_uint32         pr_pr_header_b;  // excluding pr_pe_mirror_pkt_flag
                                       // capture the header at the FE-BM interface
    cs_uint32         pe_bm_capture;   // [`HDRB_WIDTH-1:0] header
        
}cs_aal_pr_pe_info_capture_t;


/*capture the drop source of dropped packets */
typedef struct {
   cs_boolean          st_flag;    // 1: captured a dropped packet;0:doesn't capture any dropped packet
   cs_uint8            drop_state; // the drop source of dropped packet
   cs_uint8            rsvd[2];
   
}cs_aal_fe_drop_src_capture_t;



/*****************************************************************************/
/* $rtn_hdr_start   Set packet Packet FE Configurations                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : LE_PE (L2)                                                   */
/* CHIP       : CS8030                                                       */
cs_status aal_pkt_fe_ctrl_set(
    CS_IN cs_aal_port_id_t         port,
    CS_IN cs_aal_pkt_fe_ctrl_msk_t cfg_msk,
    CS_IN cs_aal_pkt_fe_ctrl_t     *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  cfg_msk - Parameter mask                                  */
/*              o  cfg -   pe_ctrl parameters                                */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: set successfully                                 */
/* DESCRIPTION:                                                              */
/* This API is used to configure packet FE engine configurations, configuration fields including: */
/* typedef struct { */                                                                                                                         
/*    cs_boolean     pe_ge_en; //(default=true),Enable/disable all packet editing functions when the resolved dpid is GE  */
/*    cs_boolean     pe_pon_en; //(default=true),Enable/disable all packet editing functions when the resolved dpid is pon  */
/*    cs_boolean     pe_cpu_en; //(default=false),Enable/disable all packet editing functions when the resolved dpid is cpu  */
/*    cs_boolean     pe_mii0_en; //(default=true),Enable/disable all packet editing functions when the resolved dpid is mii0  */
/*    cs_boolean     pe_mii1_en; //(default=true),Enable/disable all packet editing functions when the resolved dpid is mii1  */
/*    cs_aal_te_pkt_len_t  ingress_pkt_len;// (default=AAL_TO_TE_INCOME),packet length to TE for updating token  */
/*    cs_uint8          inner_def_dot1p;//(default=0),the default 8021p for INNER_PUSH_CMD */
/*    cs_uint8          top_def_8021p;//(default=0),the default 8021p for TOP_PUSH_CMD */
/*    cs_boolean      txpad_en;//(default=false),pad or no change outgoing packet if less than 64B */
/*    cs_aal_fcs_mode_t stomp_crc;//(default=AAL_FCS_CALC_NOT_CRC32),the FCS calculation's configuration  */
/*    cs_boolean      keep_fcs;//(default=false), keep or replace the incoming FCS  */
/*    cs_boolean      loop_dpid_deny;//(default=true),enable or disable to drop packet if SPID==DPID*/
/*    cs_boolean      loop_mac_chk; //(default=false).enable or disable to check if ingress vlan equal to egress vlan for loop */
/*    cs_boolean      loop_vlan_chk;//(default=false).enable or disable to check if mac DA equal to mac SA for loop */
/*      // for enable: perform real checking; for disable: alwasy treat VID or MAC as equal (only check SPID and DPID) */
/*    cs_boolean      loop_deny;  //(default=false),enable or disable to drop packet if the spid == dpid and ingrss vlan == egress vlan */   
/*    cs_uint8          rsvd;                                                */
/*} cs_aal_pkt_pe_ctrl_t;                                                    */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get Packet Edit engine configurations                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : LE_PE (L2)                                                   */
/* CHIP       : CS8030                                                       */
cs_status aal_pkt_fe_ctrl_get(
    CS_IN  cs_aal_port_id_t      port,
    CS_OUT cs_aal_pkt_fe_ctrl_t  *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/* OUTPUTS    : o  cfg - pe_ctrl parameters                                  */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: Get successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to return Packet FE engine configurations    */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set User Defined Special Packet Pattern                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : Packet                                                       */
/* CHIP       : CS8030                                                       */
cs_status aal_pkt_special_udf_set(
    CS_IN cs_aal_port_id_t           port,
    CS_IN cs_aal_pkt_spec_udf_msk_t  msk,
    CS_IN cs_aal_pkt_spec_udf_t      *cfg
);
/* INPUTS     : o  port -   port index( GE,PON,MA)                           */
/*              o  msk    -    special packet pattern mask                   */
/*              o  cfg    -    special packet pattern                        */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: rx_func is not a valid one                    */
/*              o  CS_E_OK: set successfully                                 */
/* DESCRIPTION:                                                              */
/*  This API is used to configure user defined special packet pattern in     */
/*  different FE instance.  Related structure is defined as following:       */
/* typedef struct { */
/*     cs_uint16                udf0_etype;// (default=0x0802) UDF0 ethertype (also the ev2pt encoding value 0xA for classificaiton) */
/*     cs_boolean                   udf0_chk_da; // (default=false) enable or disable to check DA MAC to decare a UDF0 */
/*     // enable: UDF0 is detected when etype and DA matched */
/*     // disable: UDF0 is detected when only etype matched */
/*     cs_mac_t                     udf0_da; // (default=00:00:00:00:00:00) UDF0 DA MAC */
/*     cs_uint16                udf1_etype;// (default=0x0801) UDF1 ethertype (also the ev2pt encoding value 0xB for classificaiton) */
/*     cs_boolean                   udf1_chk_da; // (default=false) enable or disable to check DA MAC to decare a UDF1 */
/*     // enable: UDF1 is detected when etype and DA matched */
/*     // disable: UDF1 is detected when only etype matched */
/*     cs_mac_t                     udf1_da; // (default=00:00:00:00:00:00) UDF1 DA MAC */
/*     cs_uint16                udf2_etype;// (default=0x0804) UDF2 ethertype (also the ev2pt encoding value 0xC for classificaiton) */
/*     cs_uint16                udf3_etype;// (default=0x0803) UDF3 ethertype (also the ev2pt encoding value 0xD for classificaiton) */
/*     cs_uint16                udf4_etype;// (default=0x0805) UDF4 ethertype (also the ev2pt encoding value 0xE for classificaiton) */
/*     cs_mac_t                     hello_da; // (default=00:00:00:00:00:00) Hello DA MAC, A packet is detected as HELLO when etype=0xffff and DA matched */
/* } cs_aal_pkt_spec_udf_t; */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get User Defined Special Packet Pattern                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : Packet                                                       */
/* CHIP       : CS8030                                                       */
cs_status aal_pkt_special_udf_get(
    CS_IN  cs_aal_port_id_t           port,
    CS_OUT cs_aal_pkt_spec_udf_t      *cfg
);
/* INPUTS     : o  port -   port index( GE,PON,MA)                           */
/* OUTPUTS    : o  cfg    -    special packet pattern -                      */
/* RETURNS    : o  CS_E_ERROR: rx_func is not a valid one                    */
/*              o  CS_E_OK: set successfully                                 */
/* DESCRIPTION:                                                              */
/* This API is used to get user defined special packet pattern in            */
/*              different FE instance.                                       */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set Special Packet Behavior                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : SPEC Packet                                                  */
/* CHIP       : CS8030                                                       */
cs_status aal_special_pkt_behavior_set(
    CS_IN cs_aal_port_id_t            port,
    CS_IN cs_aal_pkt_type_t           pkt_type,
    CS_IN cs_aal_spec_pkt_ctrl_msk_t  cfg_msk,
    CS_IN cs_aal_spec_pkt_ctrl_t      *cfg
);
/* INPUTS     : o  port -      port index                                    */
/*              o  pkt_type -  packet type want to be controlled             */
/*              o  cfg_msk -   Parameter mask                                */
/*              o  cfg    -    special packet configuraiton                  */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: rx_func is not a valid one                    */
/*              o  CS_E_OK: set successfully                                 */
/* DESCRIPTION:                                                              */
/*  This API is used to configure special packet behavior in                 */
/*  different physical port.   Related structure and default value is following */
/* default behavior regarding each type is as following:                                     */
/*        PKTtype                PRI     DPID     Learning    LimitForward                   */
/* AAL_PKT_NORMAL                 0        7         1            0                          */
/* AAL_PKT_BPDU                  27        7         1            0                          */
/* OLA_PKT_8021X                 29        7         1            0                          */
/* AAL_PKT_IEEE_RSVD1            22        7         1            0                          */
/* AAL_PKT_MYMAC                 28        4         0            0                          */
/* AAL_PKT_MC_A                  21        7         1            0                          */
/* AAL_PKT_MC_B                  20        7         1            0                          */
/* AAL_PKT_MC_C                  19        7         1            0                          */
/* AAL_PKT_IGMP                  26        7         1            0                          */
/* AAL_PKT_ARP                   24        7         1            0                          */
/* AAL_PKT_OAM                   31        4         0            0                          */
/* AAL_PKT_MPCP                  30        3         0            0                          */
/* AAL_PKT_DHCP                  18        7         1            0                          */
/* AAL_PKT_PPPOE_DIS             25        7         1            0                          */
/* AAL_PKT_PPPOE_SESSION         24        7         1            0                          */
/* AAL_PKT_HELLO                 23        4         0            0                          */
/* AAL_PKT_IPV6NDP               22        7         1            0                          */
/* AAL_PKT_UDF0                   1        7         1            0                          */
/* AAL_PKT_UDF1                   2        7         1            0                          */
/* AAL_PKT_UDF2                   3        7         1            0                          */
/* AAL_PKT_UDF3                   4        7         1            0                          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get Special Packet Status                                */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : Packet                                                       */
/* CHIP       : CS8030                                                       */
cs_status aal_special_pkt_behavior_get(
    CS_IN  cs_aal_port_id_t         port,
    CS_IN  cs_aal_pkt_type_t        pkt_type,
    CS_OUT cs_aal_spec_pkt_ctrl_t   *cfg
);
/* INPUTS     :o  port      -    Physical port id;                           */
/*             o  pkt_type    -    packet type want to be controlled         */
/* OUTPUTS   : o  cfg        -    special packet configuraiton               */
/* RETURNS   : o  CS_E_ERROR: Get failed                                     */
/*             o  CS_E_OK: Get successfully                                  */
/* DESCRIPTION:                                                              */
/*            This API is used to retrieve special packet behavior in        */
/*            different physical port.                                       */
/*                                                                           */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set per port L2 FDB default configuration                */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : L2                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_port_dft_cfg_set(
    CS_IN cs_aal_port_id_t        port,
    CS_IN cs_aal_port_dft_msk_t   cfg_msk,
    CS_IN cs_aal_port_dft_cfg_t   *cfg
);
/* INPUTS     : o port - Port index                                          */
/*              o cfg_msk - Parameter mask                                   */
/*              o cfg -  L2 port default parameters                          */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o CS_E_ERROR: other error occurs                             */
/*              o CS_E_PARAM: one of parameter is not correct                */
/*              o CS_E_OK: set successfully                                  */
/* DESCRIPTION:                                                              */
/*             This API is used to configure per port default configuration  */
/*             Configured structure is defined as following:                 */
/* typedef struct {                                                          */
/*     cs_uint16 vlanid;   // (default=0): configurable default port vlan id */
/*     cs_uint8  cos;      // (default=0): configurable default port cos     */
/*     cs_uint8  dot1p;    // (default=0): configurable default port 802.1p value */
/*     cs_uint8  dscp;     // (default=0): configurable default port dscp value */
/*     cs_uint16 tpid;     // (default=0x8100): configurable default port tpid */
/* } cs_aal_port_dft_cfg_t;                                                  */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get per port default configuration                       */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : L2                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_port_dft_cfg_get(
    CS_IN  cs_aal_port_id_t       port,
    CS_OUT cs_aal_port_dft_cfg_t  *cfg
);
/* INPUTS     : o port - Port index                                          */
/* OUTPUTS    : o cfg - L2 port default parameters                           */
/* RETURNS    : o CS_E_ERROR: other error occurs                             */
/*              o CS_E_PARAM: one of parameter is not correct                */
/*              o CS_E_OK: Get successfully                                  */
/* DESCRIPTION:                                                              */
/*             This API is used to return per port default configuration     */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set Per Port Configuration                               */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : L2                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_port_cfg_set(
    CS_IN cs_aal_port_id_t      port,
    CS_IN cs_aal_port_msk_t     cfg_msk,
    CS_IN cs_aal_port_cfg_t     *cfg
);
/* INPUTS     : o port - Port index                                          */
/*              o cfg_msk - Parameter mask                                   */
/*              o cfg -  L2 port parameters                                  */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o CS_E_ERROR: other error occurs                             */
/*              o CS_E_PARAM: one of parameter is not correct                */
/*              o CS_E_OK: set successfully                                  */
/* DESCRIPTION:                                                              */
/*             This API is used to configure per port L2 configuration,      */
/*             including following:                                          */
/* typedef struct {                                                          */
/*   cs_uint16 ipv4_dscp_mask;       // 0~0x3f(default=0): DSCP protect bitmap (for each bit: 0 for permit packet editing, 1 for keep the packet bit value) */
/*    cs_uint16 ipv6_tc_mask;         // 0~0x3f(default=0): TC protect bitmap (for each bit: 0 for permit packet editing, 1 for keep the packet bit value) */
/*  cs_boolean  tx_def_vid_pop;//(default=false): enable/disable to pop top egress vlan tag if the final TX top vlanid is default VID*/
/*    cs_boolean  rx_def_vid_swap; //(default=false):enable/disable this function:*/
/*                                 //When the top vlanid of the incoming packet matches with the port default vlanid,*/
/*                                   and if the final vlan command for this packet is push then the packet*/
/*                                   edit engine pops the incoming top vlanid and pushes the new vlanid back as the top outgoing vlan. */
/*    cs_aal_cpu_info_type_t cpu_header_pkt_type;//(default=special packet type), the type of 5-bit information in the packet header of packets destined to the CPU  */
/*    cs_boolean  oam_lpbk_en;        // (default=false): enable the RX2TX loopback for packets coming into this port. OAM is always not looped back*/
/*    cs_boolean dbg_da_sa_swap_en; // (default=false): enable/disable the swap of DA and SA MAC for packets coming into this port, debug fot loopback function*/
/*    cs_aal_dpid_t uuc_dpid;     // (default=AAL_PORT_ID_PON for GE port, AAL_PORT_ID_GE for PON port, AAL_PORT_ID_CPU for other ports) unknown unicast forward control*/
/*    cs_uint8 uuc_fwd_cos;           // 0~7(default=7): cos for unknown unicast*/
/*    cs_aal_dpid_t bc_dpid;       // (default=AAL_PORT_ID_PON for GE port, AAL_PORT_ID_GE for PON port, AAL_PORT_ID_CPU for other ports) broadcast forward control*/
/*    cs_uint8 bc_fwd_cos;           // 0~7(default=7): cos for broadcast*/
/*    cs_aal_dpid_t mc_dpid;      // (default=AAL_PORT_ID_PON for GE port, AAL_PORT_ID_GE for PON port, AAL_PORT_ID_CPU for other ports) multicast forward control*/
/*    cs_uint8 mc_fwd_cos;         // 0~7(default=7): cos for multicast */
/*    cs_uint8       rsvd;                                                   */
/* }cs_aal_port_cfg_t;                                                       */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get Per Port L2 Configuration                            */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : L2                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_port_cfg_get(
    CS_IN  cs_aal_port_id_t    port,
    CS_OUT cs_aal_port_cfg_t   *cfg
);
/* INPUTS     : o port - Port index                                          */
/* OUTPUTS    : o cfg - L2 port parameters                                   */
/* RETURNS    : o CS_E_ERROR: other error occurs                             */
/*              o CS_E_PARAM: one of parameter is not correct                */
/*              o CS_E_OK: Get successfully                                  */
/* DESCRIPTION:                                                              */
/*             This API is used to return per port L2 configuration          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set FDB Configurations                                   */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : L2                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_fdb_cfg_set(
    CS_IN cs_aal_fdb_msk_t  cfg_msk,
    CS_IN cs_aal_fdb_cfg_t  *cfg
);
/* INPUTS     : o  cfg_msk - Parameter mask                                  */
/*              o  cfg -   FDB parameters                                    */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: set successfully                                 */
/* DESCRIPTION:                                                              */
/* This API is used to configure FDB configurations, configuration fields including: */
/* typedef struct {                                                          */                                                                                                                         
/*    cs_aal_fdb_learn_t learn_mode; // (default=aal_L2_LEARN_HW): Hardware or software learning mode   */                                     
/*    cs_boolean aging_en;     // (default=true) enable or disable aging   */                                                                
/*    cs_uint16 aging_cntr; // 1~0xfff(default=4095): initial aging counter for each entry. It refers to how many aging intervals.*/         
/*    cs_uint32 aging_intvl;  // 1~0xffffffff(default=143080): aging interval */                                                             
/*                                            // Final aging time = aging_cntr * aging_intvl * 64 * 8ns           */                             
/*    cs_boolean station_move_en;   // (default=true): enable or disable station move operation in l2 mac table          */                  
/*    cs_boolean station_move_drop;   // (default=false): enable or disable the drop when station move is disabled but station move happens*/
/*                                        // true: drop, false: continue forwarding but invalidate the L2 entry */                               
/*    cs_uint16 st_loop_thrshld; // 0..4095(default=4082): station loop detection threshold in unit of 1/4095 */                             
/*        // When station move happens and the FDB entry lives for smaller than */                                                               
/*        // (aging_cntr -station_loop_thrshld), it will be detected as    */                                                                    
/*        // station loop. If this value is bigger than aging_cntr, loop detect doesn't work.       */                                           
/*    cs_boolean st_loop_detect_drop; // (default=false) enable or disable to drop of looping pack */                                       
/*    cs_boolean st_loop_detect_drop; // (default=false) enable or disable to drop of looping packets */                                                                     
/* }cs_aal_fdb_cfg_t;                                                        */  
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get FDB configurations                                   */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : L2                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_fdb_cfg_get(
    CS_OUT cs_aal_fdb_cfg_t *cfg
);
/* INPUTS     : ----                                                         */
/* OUTPUTS    : o  cfg - L2 FDB parameters                                   */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: Get successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to return L2 FDB configurations              */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set L2 FDB entry by given index                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : L2                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_fdb_entry_set(
    CS_IN cs_uint8           idx,
    CS_IN cs_aal_fdb_entry_t *fdb
);
/* INPUTS     : o  idx - Index of fdb entry, range is from 0 to 63           */
/*              o  fdb -  L2 FDB entry                                       */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: Other error occurs                            */
/*              o  CS_E_PARAM: One of parameter is not correct               */
/*              o  CS_E_OK: Set successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to set a L2 FDB entry                        */
/* typedef struct {                                                          */
/*    cs_boolean  vld;                // false: invalid fdb entry, true:valid fdb entry */
/*    cs_mac_t    mac;                // mac address learned                 */
/*    cs_uint16   vid;                // vlan id leanred                     */
/*    cs_uint8    dot1p;              // dot1p leanred                       */
/*    cs_boolean  mac_vld;            // valid indicator for mac address     */
/*    cs_boolean  vid_vld;            // valid indicator for vlan id         */
/*    cs_boolean  dot1p_vld;          // valid indicator for dot1p           */
/*    cs_boolean  static_flag;        // false: dynamic entry, true: dynamic entry */
/*    cs_uint8    port_id;            // 0 - 7, source port id               */
/*    cs_boolean  permit_da_ge;       // false: destination address deny, true: destination address permit */
/*    cs_boolean  permit_sa_ge;       // false: source address deny, true: source address permit   */
/*    cs_boolean  permit_da_pon;      // false: destination address deny, true: destination address permit */
/*    cs_boolean  permit_sa_pon;      // false: source address deny, true: source address permit   */
/*    cs_boolean  permit_da_cpu;      // false: destination address deny, true: destination address permit */
/*    cs_boolean  permit_sa_cpu;      // false: source address deny, true: source address permit   */
/*    cs_boolean  permit_da_mii0;     // false: destination address deny, true: destination address permit */
/*    cs_boolean  permit_sa_mii0;     // false: source address deny, true: source address permit   */
/*    cs_boolean  permit_da_mii1;     // false: destination address deny, true: destination address permit */
/*    cs_boolean  permit_sa_mii1;     // false: source address deny, true: source address permit   */
/*    cs_uint16   aging_status;       // 0 - 0xfff, 0 means aged out, 0xfff means entry newly learned or entry hit refresh */
/* }cs_aal_fdb_entry_t; */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get L2 FDB entry by given index                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : L2                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_fdb_entry_get(
    CS_IN  cs_uint8           idx,
    CS_OUT cs_aal_fdb_entry_t *fdb
);
/* INPUTS     : o  idx - Index of fdb entry, range is from 0 to 63           */
/* OUTPUTS    : o  fdb - L2 FDB status                                       */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: Get successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to get the overal L2 FDB status              */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set ge vlan mac limit entry by given index               */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : L2                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_ge_vlan_mac_limit_entry_set(
    CS_IN cs_aal_fdb_vlan_mac_t   idx,
    CS_IN cs_aal_vlan_mac_entry_t * vlan_mac_entry
);
/* INPUTS     : o  idx - Index of vlan mac limit entry, range is from 0 to 3 */
/* o  vlan_mac_entry -  vlan mac limit entry                                 */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: Other error occurs                            */
/*              o  CS_E_PARAM: One of parameter is not correct               */
/*              o  CS_E_OK: Set successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to set the vlan based mac address learning limit function for GE port */
/* typedef struct {                                                          */
/*    cs_boolean     vld;      // false: invalid vlan mac limit entry, true:valid vlan mac limit entry */
/*    cs_uint8        mac_limit;      // 0-0x40(default=16): max entries can be learned */
/*    cs_uint16       mac_limited_vlan;//compared with the outermost VLAN of packets.  */ 
/*                                     If there is a match, the VLAN based MAC address limit is checked. */ 
/*}cs_aal_vlan_mac_entry_t;                                                  */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get ge vlan mac limit entry by given index               */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : L2                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_ge_vlan_mac_limit_entry_get(
    CS_IN  cs_aal_fdb_vlan_mac_t   idx,
    CS_OUT cs_aal_vlan_mac_entry_t *vlan_mac_entry
);
/* INPUTS     : o  idx - Index of vlan mac limit entry, range is from 0 to 3 */
/* OUTPUTS    : o  vlan_mac_entry - vlan mac limit status                    */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: Get successfully                                 */
/* DESCRIPTION:                                                              */
/* This API is used to get the the vlan based mac address learning limit status */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set Per Port FDB Configuration                           */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : L2                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_fdb_port_cfg_set(
    CS_IN cs_aal_port_id_t      port,
    CS_IN cs_aal_fdb_port_msk_t cfg_msk,
    CS_IN cs_aal_fdb_port_cfg_t *cfg
);
/* INPUTS     : o  port - Port index                                         */
/*              o  cfg_msk - Parameter mask                                  */
/*              o  cfg -  L2 fdb port parameters                             */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: set successfully                                 */
/* DESCRIPTION:                                                              */
/* This API is used to configure per port L2 FDB configuration, including following: */
/* typedef struct { */
/*    cs_boolean learn_dis;        // (default=false for GE, true for other ports): disallow(true) or allow(false) learning on this port  */
/*    cs_boolean learn_lmt_ovrwrt;       // (default=false): enable/disable to overwrite the eldest entry if MAC limit reached */
/*    cs_boolean learn_dpid_cpu_dis;  // (default=true): disallow(true) or allow(false) learning for TO CPU packets coming into this port */
/*    cs_boolean learn_dpid_drop_dis; // (default=true): disallow(true) or allow(false) learning for DROPPING packets coming into this port */
/*    cs_boolean pkt_fwd_dis;       // (default=false): drop(true) or forward(false) all packets destined TO this port except for those CPU sending */
/*    cs_aal_fdb_vlan_lrn_t   learn_vlan_sel;           // (default=aal_L2_VLAN_LRN_RX_TOP): vlan id selection for l2 mac table learning  */
/*    cs_aal_fdb_vlan_lkup_t lookup_vlan_sel;       // (default=aal_L2_VLAN_LKUP_RX_TOP): vlan id selection for l2 mac table lkup  */
/*    cs_boolean  sa_drop_en;  //(default=true),Enable(true) or disable dropping of packets when L2 Engine decides to drop a packet because of MAC SA checks or Station Move  */   
/*    cs_boolean learn_mac_vld;   // (default=true): enable/disable to learn MAC  */
/*    cs_boolean learn_vlan_vld;       // (default=false): enable/disable to learn VLAN  */
/*    cs_boolean learn_dot1p_vld;  // (default=false): enable/disable to learn dot1p  */
/*    cs_boolean lookup_mac_vld;     // (default=true): enable/disable to lookup MAC  */
/*    cs_boolean lookup_vlan_vld;     // (default=false): enable/disable to lookup VLAN  */
/*    cs_boolean lookup_dot1p_vld;    // (default=false): enable/disable to lookup dot1p  */    
/*    cs_aal_fdb_permit_t  mac_match_ctrl; //(default=aal_FDB_PERMIT_DA_SA), Source address, destination address pemit/deny control bits for port.  */   
/*    cs_uint8       sa_max_limit;    // 0-0x40(default=64): max address entries can be learned  */
/*    cs_boolean   sa_lmt_drop_en;    // (default=false): enable or disable to drop packets if SA limit reached  */
/*    cs_uint8       rsvd[3];                                                */     
/*}cs_aal_fdb_port_cfg_t;                                                    */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get Per Port FDB Configuration                           */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : L2                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_fdb_port_cfg_get(
    CS_IN  cs_aal_port_id_t       port,
    CS_OUT cs_aal_fdb_port_cfg_t  *cfg
);
/* INPUTS     : o  port - Port index                                         */
/* OUTPUTS    : o  cfg - L2 fdb port parameters                              */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: Get successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to return per port L2 fdb configuration      */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get L2 FDB  debug status                                 */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : L2                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_fdb_status_get(
    CS_OUT cs_aal_fdb_status_t *cfg
);
/* INPUTS     : ----                                                         */
/* OUTPUTS    : o  cfg - L2 FDB debug parameters                             */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: Get successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to get L2 FDB debug info                     */
/* typedef struct {                                                          */
/*   cs_uint16 lrn_cnt_ge;   // 0 - 0x3f, learning counter for ge port       */
/*    cs_uint16 lrn_cnt_pon;  // 0 - 0x3f, learning counter for pon port     */
/*    cs_uint16 lrn_cnt_cpu;   // 0 - 0x3f, learning counter for cpu port    */
/*    cs_uint16 lrn_cnt_mii0;  // 0 - 0x3f, learning counter for mii0 port   */
/*    cs_uint16 lrn_cnt_mii1;  // 0 - 0x3f, learning counter for mii1 port   */  
/*    cs_uint16 ge_vln0_lrn_cnt; // 0 - 0x3f, learning counter for GE port vlan 0 based */
/*    cs_uint16 ge_vln1_lrn_cnt; // 0 - 0x3f, learning counter for GE port vlan 1 based */
/*    cs_uint16 ge_vln2_lrn_cnt; // 0 - 0x3f, learning counter for GE port vlan 2 based */
/*    cs_uint16 ge_vln3_lrn_cnt; // 0 - 0x3f, learning counter for GE port vlan 3 based   */
/*    cs_uint16 station_move_idx_ge;   // 0 - 0x3f, l2 table hit index for station move events */
/*    cs_uint16 station_move_idx_pon;  // 0 - 0x3f, l2 table hit index for station move events */
/*    cs_uint16 station_move_idx_cpu;  // 0 - 0x3f, l2 table hit index for station move events */
/*    cs_uint16 station_move_idx_mii0; // 0 - 0x3f, l2 table hit index for station move events */
/*    cs_uint16 station_move_idx_mii1; // 0 - 0x3f, l2 table hit index for station move events  */ 
/*    cs_uint32  ge_fe_pkt_drop_cnt;//the counter of dropped packets for GE_FE*/
/*    cs_uint32  pon_fe_pkt_drop_cnt;//the counter of dropped packets for PON_FE*/
/*    cs_uint32  ma_fe_pkt_drop_cnt;//the counter of dropped packets for MA_FE */
/*    cs_uint16 aging_idx;    // 0 - 0x3f, indicates lastest aged out entry index due to l2 table aging function */
/*    cs_uint8   rsvd[2];                                                    */
/*} cs_aal_fdb_status_t;                                                     */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/

#if 0
/*****************************************************************************/
/* $rtn_hdr_start   Set FE Interrupt Configuration                           */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : Interrupt                                                    */
/* CHIP       : CS8030                                                       */
cs_status aal_fe_int_cfg_set(
    CS_IN cs_aal_fe_int_msk_t evt_msk,
    CS_IN cs_aal_fe_int_cfg_t *event
);
/* INPUTS     : o  evt_msk - Parameter mask                                  */
/*              o  event -  FE Interrupt cfg parameters                      */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: set successfully                                 */
/* DESCRIPTION:                                                              */
/*   This API is used to configure FE Interrupt enable, including following: */
/* typedef struct {                                                          */
/*   cs_boolean   evt_pkt_len_mismatch_err_ge_en;//(default=true),Control for enable/disable of GE port packet length error interrupt */
/*   cs_boolean   evt_pkt_len_mismatch_err_pon_en;//(default=true),Control for enable/disable of PON port packet length error interrupt */
/*   cs_boolean   evt_pkt_len_mismatch_err_ma_en;//(default=true),Control for enable/disable of MA port packet length error interrupt */
/*    //while the packet length in header-a doesn't match the actual packet length, this interrupt is asserted; */
/*    cs_boolean   evt_pkt_len_runt_err_ge_en;//(default=true),Control for enable/disable of GE port packet length error interrupt*/
/*    cs_boolean   evt_pkt_len_runt_err_pon_en;//(default=true),Control for enable/disable of PON port packet length error interrupt */
/*    cs_boolean   evt_pkt_len_runt_err_ma_en;//(default=true),Control for enable/disable of MA port packet length error interrupt; */
/*    //while a runt packet is delivered to FE module, this interrupt is asserted; */
/*    cs_boolean   evt_ce_mac_filter_aged_out_ge_en;//(default=true),Control for enable/disable of GE mac filter aging out interrupt */
/*    cs_boolean   evt_ce_mac_filter_aged_out_pon_en;//(default=true),Control for enable/disable of PON mac filter aging out interrupt */
/*    cs_boolean   evt_ce_mac_filter_aged_out_ma_en;//(default=true),Control for enable/disable of MA mac filter aging out interrupt */
/*    cs_boolean   evt_pkt_len_err_ge_en;//(default=true),Control for enable/disable of GE port packet length error interrupt */
/*    cs_boolean   evt_pkt_len_err_pon_en;//(default=true),Control for enable/disable of PON port packet length error interrupt */
/*    cs_boolean   evt_pkt_len_err_ma_en;//(default=true),Control for enable/disable of MA port packet length error interrupt */
/*    cs_boolean   evt_lat_fifo_overflow_ge_en;//(default=true),Control for enable/disable of GE latency FIFO's overflow interrupt */
/*    cs_boolean   evt_lat_fifo_overflow_pon_en;//(default=true),Control for enable/disable of PON latency FIFO's overflow interrupt */
/*    cs_boolean   evt_lat_fifo_overflow_ma_en;//(default=true),Control for enable/disable of MA latency FIFO's overflow interrupt */
/*    cs_boolean   evt_fdb_entry_aged_out_en;//(default=false),Control for enable/disable of layer 2 dynamic entry aging out event by hardware. */
/*    cs_boolean   evt_fdb_no_avail_dyn_entry_learn_en;//(default=false),Control for enable/disable of unavailability of dynamic entries for hardware learning event. */
/*    cs_boolean   evt_fdb_ovr_max_limit_ge_en;//(default=false),Control for enable/disable learning event over the maximum allowed source address learning limit interrupt from ge port */
/*    cs_boolean   evt_fdb_ovr_max_limit_pon_en;//(default=false),Control for enable/disable learning event over the maximum allowed source address learning limit interrupt from pon port */
/*    cs_boolean   evt_fdb_ovr_max_limit_cpu_en;//(default=false),Control for enable/disable learning event over the maximum allowed source address learning limit interrupt from cpu port */
/*    cs_boolean   evt_fdb_ovr_max_limit_mii0_en;//(default=false),Control for enable/disable learning event over the maximum allowed source address learning limit interrupt from MII0 port */
/*    cs_boolean   evt_fdb_ovr_max_limit_mii1_en;//(default=false),Control for enable/disable learning event over the maximum allowed source address learning limit interrupt from MII1 port */
/*    cs_boolean   evt_fdb_stn_mov_ge_en;//(default=false),Control for enable/disable of layer 2 station move event interrupt from GE port */
/*    cs_boolean   evt_fdb_stn_mov_pon_en;//(default=false),Control for enable/disable of layer 2 station move event interrupt from pon port */
/*    cs_boolean   evt_fdb_stn_mov_cpu_en;//(default=false),Control for enable/disable of layer 2 station move event interrupt from cpu port */ 
/*    cs_boolean   evt_fdb_stn_mov_mii0_en;//(default=false),Control for enable/disable of layer 2 station move event interrupt from mii0 port */
/*    cs_boolean   evt_fdb_stn_mov_mii1_en;//(default=false),Control for enable/disable of layer 2 station move event interrupt from mii1 port */
/*    cs_boolean   evt_fdb_learn_ge_en;//(default=false),Control for enable/disable of layer 2 learning event interrupt from GE port */
/*    cs_boolean   evt_fdb_learn_pon_en;//(default=false),Control for enable/disable of layer 2 learning event interrupt from PON port */
/*    cs_boolean   evt_fdb_learn_cpu_en;//(default=false),Control for enable/disable of layer 2 learning event interrupt from CPU port */
/*    cs_boolean   evt_fdb_learn_mii0_en;//(default=false),Control for enable/disable of layer 2 learning event interrupt from MII0 port */
/*    cs_boolean   evt_fdb_learn_mii1_en;//(default=false),Control for enable/disable of layer 2 learning event interrupt from MII1 port */
/*    cs_boolean   evt_fdb_stn_mov_loop_det_drop_en;//(default=true),Control for enable/disable of layer 2 engine station move loop detect packet drop interrupt; */
/*    cs_uint8       rsvd[3];                                                */
/* }cs_aal_fe_interrupt_cfg_t;                                               */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get FE Interrupt Configuration                           */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : Interrupt                                                    */
/* CHIP       : CS8030                                                       */
cs_status aal_fe_int_cfg_get(
    CS_OUT cs_aal_fe_int_cfg_t *event
);
/* INPUTS     :  ----                                                        */
/* OUTPUTS    : o  event - FE Interrupt parameters                           */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: Get successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to return FE Interrupt configuration         */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get FE Interrupt status                                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : Interrupt                                                    */
/* CHIP       : CS8030                                                       */
cs_status aal_fe_int_status_get(
    CS_OUT cs_aal_fe_int_status_t *event_staus
);
/* INPUTS     :  ----                                                        */
/* OUTPUTS    : o  event_staus - FE Interrupt status parameters              */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: Get successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to return FE Interrupt status.               */
/*             indicates what interrupt sources (should the                  */
/* corresponding enable bit be set) are causing an interrupt to be generated.*/
/* $rtn_hdr_end                                                              */
/*****************************************************************************/

/*****************************************************************************/
/* $rtn_hdr_start   Clear FE Interrupt status                                */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : Interrupt                                                    */
/* CHIP       : CS8030                                                       */
cs_status aal_fe_int_status_clr(
    void
);
/* INPUTS     :  ----                                                        */
/* OUTPUTS    :  ----                                                        */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: Get successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to clear FE Interrupt status.                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
#endif

/*****************************************************************************/
/* $rtn_hdr_start   Set VLAN General Configuration                           */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : VLAN                                                         */
/* CHIP       : CS8030                                                       */
cs_status aal_vlan_cfg_set(
    CS_IN cs_aal_port_id_t       port,
    CS_IN cs_aal_vlan_cfg_msk_t  msk,
    CS_IN cs_aal_vlan_cfg_t      *cfg
);
/* INPUTS     : o port - port index( GE,PON,MA)                              */
/*              o msk - VLAN configuration mask                              */
/*              o  cfg -  VLAN configuration                                 */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: Other error occurs                            */
/*              o  CS_E_PARAM: One of parameter is not correct               */
/*              o  CS_E_OK: Set successfully                                 */
/* DESCRIPTION:                                                              */
/*This API is used to set a VLAN general configuraiton for a certain instance*/
/*             Related structure is defined as following:                    */
/* typedef struct {                                                          */
/*     cs_boolean  loop_deny;    // (default=true): enable or disable to drop packet if SPID=DPID and RX VID= TX VID */
/*     cs_boolean loop_vid_chk;    // (default=false): enable or disable to check the above RX VID and TX VID. */
/*         // for enable: perform real checking; for disable: alwasy treat VID as equal (only check SPID and DPID) */
/*     cs_boolean parse_inner_8100;   // (default=true): enable or disable to parse/check packet with inner TPID=0x8100 */
/*     cs_boolean parse_outer_88a8;   // (default=false): enable or disable to parse/check packet with outer TPID=0x88a8       */
/*     cs_boolean parse_inner_tpid;   // (default=false): enable or disable to parse/check packet with inner TPID=<inner_tpid>     */   
/*     cs_boolean parse_outer_tpid1;   // (default=false): enable or disable to parse/check packet with outer TPID=<outer_tpid1> */
/*     cs_boolean parse_outer_tpid2;   // (default=false): enable or disable to parse/check packet with outer TPID=<outer_tpid2> */
/*     cs_boolean deny_inner_8100;   // (default=false): enable or disable to drop packet with inner TPID is parsed and equal to 0x8100 */
/*     cs_boolean deny_outer_88a8;   // (default=false): enable or disable to drop packet with outer TPID is parsed and equal to 0x88a8 */
/*     cs_boolean deny_inner_tpid;   // (default=false): enable or disable to drop packet with inner TPID is parsed and equal to <inner_tpid> */
/*     cs_boolean deny_outer_tpid1;   // (default=false): enable or disable to drop packet with outer TPID is parsed and equal to <outer_tpid1> */
/*     cs_boolean deny_outer_tpid2;   // (default=false): enable or disable to drop packet with outer TPID is parsed and equal to <outer_tpid2> */
/*     cs_uint16 ingress_inner_tpid;   // (default=0x8100): ingress inner TPID */
/*     cs_uint16 ingress_outer_tpid1;   // (default=0x8100): ingress outer TPID #1 */
/*     cs_uint16 ingress_outer_tpid2;   // (default=0x9100): ingress outer TPID #2  */
/*    cs_boolean ingress_mapping_flag; //(default = 0) Control to turn on(1) /off (0) ingress vlan mapping function in the vlan table  */
/*    cs_boolean top_vlan_l2_en;   // (default = 0),Control to replace top VLAN with the VLAN ID from VLAN based MAC limit */
/* } cs_aal_vlan_cfg_t; */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get VLAN General Configuration                           */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : VLAN                                                         */
/* CHIP       : CS8030                                                       */
cs_status aal_vlan_cfg_get(
    CS_IN cs_aal_port_id_t    port,
    CS_IN cs_aal_vlan_cfg_t   *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/* OUTPUTS    : o  cfg -  VLAN configuration                                 */
/* RETURNS    : o  CS_E_ERROR: Other error occurs                            */
/*              o  CS_E_PARAM: One of parameter is not correct               */
/*              o  CS_E_OK: Set successfully                                 */
/* DESCRIPTION:                                                              */
/* This API is used to get a VLAN general configuraiton for a certain        */
/* fWD engine instance. Same structure as defined in "aal_vlan_cfg_set".     */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set VLAN Entry of One FE                                 */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : VLAN                                                         */
/* CHIP       : CS8030                                                       */
cs_status aal_vlan_entry_set(
    CS_IN cs_aal_port_id_t    port,
    CS_IN cs_aal_vlan_idx_t   idx,
    CS_IN cs_aal_vlan_entry_t *cfg
);
/* INPUTS     : o port- port index( GE,PON,MA)                               */
/*              o idx - Index of VLAN member entry, range is from 0 to 33    */
/*                      including ingress and egress default member          */
/*              o  cfg -  VLAN member entry                                  */
/*                (default=ALL ZERO for non-default, VLAN0~4095 permit any   */
/*                 port for default ingress/egress)                          */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: Other error occurs                            */
/*              o  CS_E_PARAM: One of parameter is not correct               */
/*              o  CS_E_OK: Set successfully                                 */
/* DESCRIPTION:                                                              */
/* This API is used to set a VLAN entry, each VLAN entry could be used as    */
/* memebership checking, or be used as vlan translation pair.Translation and */
/* membership checking could be configured at ingress and /or egress         */
/* Each VLAN table entry is defined as following structure                   */
/*typedef struct {                                                           */
/*    cs_uint16 vid_lo; // vlan id range low for vlan filtering;  for vlan mpping, used for mapped vlan id */
/*    cs_uint16 vid_hi; // vlan id range high for vlan filtering;  for vlan mapping, used for original vlan id */
/*    cs_boolean  permit_ge;    // permit control for giga port              */
/*    cs_boolean permit_pon;   // permit control for pon port                */
/*    cs_boolean permit_mii0;  // permit control for mii0 port               */
/*    cs_boolean permit_mii1;  // permit control for mii1 port               */
/*    cs_boolean ingress_filter;  // enable or disable ingress filter, ignored for default VLAN member */
/*    cs_boolean egress_filter;   // enable or disable egress filter, ignored for default VLAN member */
/*    cs_boolean mapping_flag; // Used for vlan mapping (translation), 1 mapping, 0 no mapping  */
/*} cs_aal_vlan_entry_t;                                                     */
/* For example, if we set ingress tranlation only, then following member could be set as */
/*     ingress_filter =1, egress_filter =0, mapping_flag =1                  */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get VLAN Membership Entry                                */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : VLAN                                                         */
/* CHIP       : CS8030                                                       */
cs_status aal_vlan_entry_get(
    CS_IN  cs_aal_port_id_t     port,
    CS_IN  cs_aal_vlan_idx_t    idx,
    CS_OUT cs_aal_vlan_entry_t  *cfg
);
/* INPUTS     : o port- port index( GE,PON,MA)                               */
/*              o idx - Index of VLAN member entry, range is from 0 to 33    */
/*                      including ingress and egress default member          */
/* OUTPUTS    : o  cfg -  VLAN member entry                                  */
/* RETURNS    : o  CS_E_ERROR: Other error occurs                            */
/*              o  CS_E_PARAM: One of parameter is not correct               */
/*              o  CS_E_OK: Set successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to get a VLAN entry, each VLAN entry could be used as memebership */
/*             checking, or be used as vlan translation pair.                */
/*             Translation and membership checking could be configured at ingress and /or egress */
/*             Each VLAN table entry is refer to "aal_vlan_entry_set"        */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set mirror Configurations                                */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_mirror_cfg_set(
    CS_IN cs_aal_mirr_msk_t  cfg_msk,
    CS_IN cs_aal_mirr_cfg_t  *cfg
);
/* INPUTS     : o  cfg_msk - Parameter mask                                  */
/*              o  cfg -   mirror cfg parameters                             */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: set successfully                                 */
/* DESCRIPTION:                                                              */
/* This API is used to configure Mirror configurations, configuration fields including: */
/* typedef struct { */                                                                                                                         
/*   cs_uint8 drr_ge_weight;  // 0~0xff(default=64): the DRR weight for mirroring packets from GE port */
/*   cs_uint8 drr_pon_weight;  // 0~0xff(default=64): the DRR weight for mirroring packets from PON port */
/*   cs_uint8 drr_ma_weight;  // 0~0xff(default=64): the DRR weight for mirroring packets from MA port */
/*   cs_aal_mirr_weight_base_t drr_weight_base; // (default=ONU_L2_MIRR_WEI_32B): mirroring base */
/*   cs_boolean enhance_drr;     // (default=disable) enable/disable the enhacned DRR scheduling */
/*   cs_boolean l2_mirror_en;     // (default=enable) enable/disable the L2 mirroring function */
/*   cs_uint8     l2_mirror_groupid;//// 0~0x1f(default=0)   */
/*   cs_boolean  gl_mirror_en; //(default=disable), enables/disable mirror port function.  */
/*                                            // Packet memory has a dedicated slice for mirror port FIFO  */
/*}cs_aal_mirr_cfg_t;                                                        */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get mirror configurations                                */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_mirror_cfg_get(
    CS_OUT cs_aal_mirr_cfg_t *cfg
);
/* INPUTS     : ----                                                         */
/* OUTPUTS    : o  cfg - mirror parameters                                   */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: Get successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to return mirror configurations              */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set Per Port mirror Configuration                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_mirror_port_cfg_set(
    CS_IN cs_aal_port_id_t             port,
    CS_IN cs_aal_mirror_port_cfg_msk_t cfg_msk,
    CS_IN cs_aal_mirror_port_cfg_t     *cfg
);
/* INPUTS     : o  port - Port index                                         */
/*              o  cfg_msk - Parameter mask                                  */
/*              o  cfg -  mirror port parameters                             */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: set successfully                                 */
/* DESCRIPTION:                                                              */
/* This API is used to configure per port mirror configuration, including following: */
/* typedef struct {                                                          */
/*   cs_boolean              umc_mirror_en; //(default=disable) enable/disable the unknown multicast packets mirroring function    */
/*     cs_onu_mirr_sel_e   umc_mirror_src_sel;//(default=0),mirror_src_sel flag,the packet may be copied before or after packet modification    */
/*     cs_uint8                  umc_mirror_groupid;// 0~0x1f(default=0)    */
/*     cs_boolean              uuc_mirror_en; //(default=disable) enable/disable the unknown unicast packets mirroring function    */
/*     cs_onu_mirr_sel_e   uuc_mirror_src_sel;//(default=0),mirror_src_sel flag,the packet may be copied before or after packet modification    */
/*     cs_uint8                  uuc_mirror_groupid;// 0~0x1f(default=0)     */
/*     cs_boolean              bc_mirror_en;//(default=disable) enable/disable the broadcast packets mirroring function     */
/*     cs_onu_mirr_sel_e   bc_mirror_src_sel;//(default=0),mirror_src_sel flag,the packet may be copied before or after packet modification */
/*     cs_uint8                  bc_mirror_groupid;// 0~0x1f(default=0)      */
/* } cs_aal_mirror_port_cfg_t;                                               */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get Per Port mirror Configuration                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_mirror_port_cfg_get(
    CS_IN  cs_aal_port_id_t         port,
    CS_OUT cs_aal_mirror_port_cfg_t *cfg
);
/* INPUTS     : o  port - Port index                                         */
/* OUTPUTS    : o  cfg - mirrorfdb port parameters                           */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: Get successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to return per port mirror configuration      */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set redirect Configurations                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_redirect_cfg_set(
    CS_IN cs_aal_port_id_t            port,
    CS_IN cs_aal_redirect_cfg_msk_t   cfg_msk,
    CS_IN cs_aal_redirect_cfg_t       *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/*              o  cfg_msk - Parameter mask                                  */
/*              o  cfg -   redirect cfg parameters                           */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: set successfully                                 */
/* DESCRIPTION:                                                              */
/* This API is used to configure redirect configurations, configuration fields including: */
/* typedef struct {                                                          */                                                                                                                         
/*     cs_boolean                   dpid_redirect1_en;//(default=disable),enable/disable redirect1 forwarding function */
/*     cs_aal_spec_pkt_dpid_t       redirect1_fwd_dpid; // redirect 1 forwarding destination portid  */
/*     cs_aal_spec_pkt_dpid_t       redirect1_original_dpid;//redirect 1 compare destinatin port */
/*     cs_boolean                   dpid_redirect0_en;//(default=disable),enable/disable redirect0 forwarding function */
/*     cs_aal_spec_pkt_dpid_t       redirect0_fwd_dpid;//redirect 0 forwarding destination portid  */
/*     cs_aal_spec_pkt_dpid_t       redirect0_original_dpid;//redirect 0 compare destinatin port */
/* } cs_aal_redirect_cfg_t;                                                  */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get redirect configurations                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_redirect_cfg_get(
    CS_IN  cs_aal_port_id_t       port,
    CS_OUT cs_aal_redirect_cfg_t  *cfg
);
/* INPUTS     : o  port - port index( GE,PON,MA)                             */
/* OUTPUTS    : o  cfg - redirect parameters                                 */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: Get successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to return redirect configurations            */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Set debug control Configurations                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_debug_ctrl_set(
    CS_IN cs_aal_debug_ctrl_msk_t  cfg_msk,
    CS_IN cs_aal_debug_ctrl_t      *cfg
);
/* INPUTS     : o  cfg_msk - Parameter mask                                  */
/*              o  cfg -   debug control cfg parameters                      */
/* OUTPUTS    : ----                                                         */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: set successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to configure debug control configurations, configuration fields including: */
/* typedef struct {                                                          */                                                                                                                         
/*    cs_boolean   ge_mask_rxmac_drop_en; // (default=true), enable don't capture the rxmac drop source  */
/*    cs_boolean   pon_mask_rxmac_drop_en; // (default=true),enable don't capture the rxmac drop source  */
/*    cs_boolean   ma_mask_rxmac_drop_en; // (default=true),enable don't capture the rxmac drop source  */
/*    cs_boolean   dbg_cntr_clr_on_read; // (default=true)Control for debug packet counters clear on read(1:Clear on read;0:Sample mode).  */
/* } cs_aal_debug_ctrl_t;                                                    */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get debug control configurations                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_debug_ctrl_get(
    CS_OUT cs_aal_debug_ctrl_t *cfg
);
/* INPUTS     : ----                                                         */
/* OUTPUTS    : o  cfg - debug control parameters                            */
/* RETURNS    : o  CS_E_ERROR: other error occurs                            */
/*              o  CS_E_PARAM: one of parameter is not correct               */
/*              o  CS_E_OK: Get successfully                                 */
/* DESCRIPTION:                                                              */
/*             This API is used to return debug control configurations       */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get counter which counts packets for FE                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_debug_fe_pkt_count_get(
    CS_OUT cs_aal_pkt_fe_cnt_sts_t  *cfg
);
/* INPUTS   : ----                                                           */
/* OUTPUTS: o  cfg - FE debug counter parameters                             */
/* RETURNS: o  CS_E_OK   : get successfully                                  */
/*          o  CS_E_ERROR: get failed                                        */
/* DESCRIPTION:                                                              */
/*            This API is used to retrieve counters of FE                    */
/* typedef struct {                                                          */
/*    cs_uint32  pkt_drop_cnt_ge_fe;//the counter of dropped packets for GE_FE */
/*    cs_uint32  pkt_drop_cnt_pon_fe;//the counter of dropped packets for PON_FE */
/*    cs_uint32  pkt_drop_cnt_ma_fe;//the counter of dropped packets for MA_FE */
/*    cs_uint32  pkt_cnt_ingress_ge;//rollover counter for valid end of packet received from the port Mac or MA module */
/*    cs_uint32  pkt_cnt_ingress_pon;//rollover counter for valid end of packet received from the port Mac or MA module */
/*    cs_uint32  pkt_cnt_ingress_ma;//rollover counter for valid end of packet received from the port Mac or MA module */
/*    cs_uint32  pkt_cnt_egress_ge; //rollover counter for valid end of packet be sent out of FE module  */
/*    cs_uint32  pkt_cnt_egress_pon; //rollover counter for valid end of packet be sent out of FE module */
/*    cs_uint32  pkt_cnt_egress_ma; //rollover counter for valid end of packet be sent out of FE module   */
/*} cs_aal_pkt_fe_cnt_sts_t;                                                 */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   Get status for packet resoluton                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_debug_pkt_rsltn_state_get(
    CS_IN  cs_aal_port_id_t          port,
    CS_OUT cs_aal_pkt_rsltn_state_t  *cfg
);
/* INPUTS   : o  port - port index( GE,PON,MA)                               */
/* OUTPUTS  : o  cfg - state for packet resoluton                            */
/* RETURNS  : o  CS_E_OK: get successfully                                   */
/*            o  CS_E_ERROR: get failed                                      */
/* DESCRIPTION:                                                              */
/*  This API is used to retrieve the state for packet resoluton of FE        */
/* typedef struct {                                                          */
/*   cs_uint16    vlan_rsltn_st; //Destination vlan resolution state.        */
/*   cs_uint8      dscp_rsltn_st; //Destination dscps resolution state.      */
/*   cs_uint8      cos_rsltn_st; //Destination cos resolution state          */
/*   cs_uint16    8021p_rsltn_st; //Destination 802.1p resolution state      */
/*   cs_uint8      dpid_rsltn_st; //Destination port resolution state.       */
/*} cs_aal_pkt_rsltn_state_t;                                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start Get counter which counts the error resulted by VLAN command*/
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_debug_vlan_cmd_err_count_get(
    CS_IN  cs_aal_port_id_t              port,
    CS_OUT cs_aal_pe_vlan_cmd_err_cnt_t  *cfg
);
/* INPUTS   : o  port - port index( GE,PON,MA)                               */
/* OUTPUTS  : o  cfg - FE debug counter parameters                           */
/* RETURNS  : o  CS_E_OK: get successfully                                   */
/*            o  CS_E_ERROR: get failed                                      */
/* DESCRIPTION:                                                              */
/* This API is used to retrieve error counters of top/inner VLAN command     */
/* typedef struct {                                                          */
/*     cs_uint8    Pop_no_top_vlan_cnt; //The top VLAN command is POP, but the incoming packet without VLAN tag */
/*     cs_uint8    Swap_np_top_vlan_cnt;//The top VLAN command is SWAP, but the incoming packet without VLAN tag */
/*     cs_uint8    Pop_no_inner_vlan_cnt;//The inner VLAN command is POP, but the incoming packet without inner VLAN tag */
/*     cs_uint8    swap_no_inner_vlan_cnt;//The inner VLAN command is SWAP, but the incoming packet without inner VLAN tag */
/*     cs_uint8    Push_no_inner_vlan_cnt;//The inner VLAN command is PUSH, but the incoming packet without VLAN tag */
/*     cs_uint8    Runt_pkt_cnt;//Count the runt packet after VLAN command   */
/* }cs_aal_pe_vlan_cmd_err_cnt_t;                                            */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   capture the information at the PP-PR interface           */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_debug_pp_info_capture_get(
    CS_IN  cs_aal_port_id_t           port,
    CS_OUT cs_aal_pp_info_capture_t   *cfg
);
/* INPUTS   :  o  port - port index( GE,PON,MA)                              */
/* OUTPUTS  :  o  cfg - info for PP-PR or PP-NI                              */
/* RETURNS  :  o  CS_E_OK: get successfully                                  */
/*             o  CS_E_ERROR: get failed                                     */
/* DESCRIPTION:                                                              */
/*            This API is used to retrieve the  information at the PP-PR interface */
/* typedef struct { */
/*    cs_uint32    pp_ni_header_capture_0; //[`HDRA_WIDTH-1:0] header,capture the header at the PP-NI interface while SOP is asserted */
/*    cs_uint32    pp_ni_header_capture_1;//[`HDRA_WIDTH-1:0] header,capture the header at the PP-NI interface while EOP is asserted */
/*   // capture the information at the PP-PR interface   */
/*    cs_uint32    pp_pr_info_capture_2;//[31] "1" ping-pong fifo in Parser is overflow,[19:0] special packet flags */
/*    cs_uint32    pp_pr_info_capture_3;//info[31:0]       */
/* //  [31] ptp_packet flag   */
/* //  [30] mirrored packet indication  */
/* //  [29:25] group_id for mirrored packet   */
/* //  [24:22] spid[21] tpid_drop_flag   */ 
/* //  [20:9] pp_pr_hdr_sts   */
/* //  [11] HDRA_PKT_BCLLID_FLD  */
/* //  [10] HDRA_PKT_BYPASS_FLD  */
/* //  [9] 0,  */
/* //  while the incoming packet is bypass-packet, pp_pr_hdr_sts[5:0] is valid;otherwise, pp_pr_hdr_sts[8:6] is valid. */
/* //  [8:6] HDRA_MPCP_NRM_GATE_FRM_FLD, HDRA_MPDP_DSCVR_FRM_FLD,HDRA_MPCP_RGSTR_FRM_FLD */
/* //  [5:3] HDRA_DPID_FLD  */
/* //  [2:0] HDRA_COS_FLD  */
/* //  [8] parsing exceeded flag */
/* //  [7:6] ingress vlan cnt */
/* //  [5] ingress top priority vlan flag */
/* //  [4:3] L3 type: 2'b00 ipv4; 2'b01 ipv6; 2'b10 others */
/* //  [2:0] l4 type: 3'b000 tcp; 3'b001 udp; 3'b010 icmp; 3'b011 igmp; 3'b100 others   */
/* } cs_aal_pp_info_capture_t;                                               */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start   capture the information at the PR-PE interface           */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_debug_pr_info_capture_get(
    CS_IN  cs_aal_port_id_t             port,
    CS_OUT cs_aal_pr_pe_info_capture_t  *cfg
);
/* INPUTS   : o  port - port index( GE,PON,MA)                               */
/* OUTPUTS  : o  cfg - info for PR-PE or PE-BM                               */
/* RETURNS  : o  CS_E_OK: get successfully                                   */
/*            o  CS_E_ERROR: get failed                                      */
/* DESCRIPTION:                                                              */
/* This API is used to retrieve the  information at the PR-PE interface      */
/* typedef struct {                                                          */ 
/*  // capture the information at the PR-PE interface                        */
/*    cs_mac_t         da;                                                   */
/*    cs_mac_t         sa;                              */
/*    cs_boolean       da_sa_swap;                */
/*    cs_uint8           ptype;                        */
/*    cs_uint8           top_tpid_encode;       */
/*    cs_uint8           inner_tpid_encode;    */
/*    cs_uint16         inner_ingress_tpid;   */
/*    cs_uint16         top_ingress_tpid;     */
/*    cs_boolean       top_pri_vlan_flag;    */
/*    cs_uint8           vlan_num;       */
/*    cs_uint8           top_cmd;       */
/*    cs_uint8           inner_cmd;    */
/*    cs_uint16         inner_vid;    */
/*    cs_uint16         top_vid;   */ 
/*    cs_boolean       top_802_1p_update;   */
/*    cs_uint8           top_802_1p;   */
/*    cs_boolean       inner_802_1p_update;   */
/*    cs_uint8           inner_802_1p;   */
/*    cs_uint8           ip_type;  */
/*    cs_boolean       dscp_update;   */
/*    cs_uint8           dscp;  */
/*    cs_boolean       rlim_byp;//rate limit bypass flag   */
/*    cs_boolean       flow_en;  */
/*    cs_uint8           flow_id;  */
/*    cs_aal_mirr_sel_e  mirror_src_sel;  */
/*    cs_uint8           mirror_pkt_flag;   */
/*    cs_boolean       lat_fifo_pe_drop;     */
/*    cs_uint32         pr_pr_header_b; // excluding pr_pe_mirror_pkt_flag  */
/*    //capture the header at the FE-BM interface   */
/*    cs_uint32         pe_bm_capture;//[`HDRB_WIDTH-1:0] header             */ 
/*}cs_aal_pr_pe_info_capture_t;                                              */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/


/*****************************************************************************/
/* $rtn_hdr_start    capture the drop source of dropped packets              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : FE                                                           */
/* CHIP       : CS8030                                                       */
cs_status aal_debug_drop_src_capture_get(
    CS_IN  cs_aal_port_id_t             port,
    CS_OUT cs_aal_fe_drop_src_capture_t  *cfg
);
/* INPUTS   : o  port - port index( GE,PON,MA)                               */
/* OUTPUTS  : o  cfg - info for drop source                                  */
/* RETURNS  : o  CS_E_OK: get successfully                                   */
/*            o  CS_E_ERROR: get failed                                      */
/* DESCRIPTION:                                                              */
/*  This API is used to retrieve the drop source of dropped packe            */
/* typedef struct { */
/*   cs_boolean        st_flag;// 1: captured a dropped packet;0:doesn't capture any dropped packet  */
/*   cs_uint8            drop_state; //the drop source of dropped packet     */
/*}cs_aal_fe_drop_src_capture_t;                                             */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/

cs_status aal_fe_int_enable(cs_aal_fe_int_msk_t mask);
cs_status aal_fe_int_disable(cs_aal_fe_int_msk_t mask);
cs_status aal_fe_int_en_get(cs_aal_fe_int_msk_t *mask);
cs_status aal_fe_int_status_get(cs_aal_fe_int_msk_t *mask);
cs_status aal_fe_int_status_clr(cs_aal_fe_int_msk_t mask);




#endif /* __AAL_L2_H__ */

