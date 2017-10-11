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

	This file is used for aal_cls_util.c
	
*/

#ifndef __AAL_CLS_UTIL_H__
#define __AAL_CLS_UTIL_H__


#include "cs_types.h"


/*---------------------------------------------------------------------------------------
*CL_IPV4_LONG_KEY       1 key/entry  
       The key contains combination of all fields supported for IPV4 packets and matches only if
        ethertype is 0x0800 or IPV4.
CL_IPV4_SHORT_KEY      4 keys/entry
       The key contains combination of IPV4 source or destination address with net mask; IP protocol;
        and TCP/UDP source or Destination ports. The key matches only if ethertype is 0x0800 or IPV4
CL_IPV6_LONG_KEY      1 key in 2 entries
       One key occupies two consecutive entries in the table and contains combination of all fields
       supported for with IPV6 addresses. The key matches only if ethertype is 0x86dd or IPV6.
 CL_IPV6_SHORT_KEY    1 key/entry   
        The key contains combination of IPV6 source or destination address with net mask; IP protocol;
        and TCP/UDP source or Destination ports. The key matches only if ethertype is 0x86dd or IPV6.
CL_MAC_TPID_KEY        4 keys/entry
        The key contains MAC address, TPID, and 802.1P fields
CL_MAC_VLAN_KEY       4 keys/entry
        The key contains MAC address, VLAN, and 802.1p fields.
CL_MAC_ETYPE_KEY      4 keys/entry
       The key contains MAC address and Ethertype fields.
CL_CTRL_PKT_KEY       4 keys/entry
        The key contains source or destination MAC address and full 16-bit Ethertype in every key to
         identify Layer2 control packets.
CL_VLAN_KEY            8 keys/entry
        The key contains a VLAN field, encoded special TPID, encoded special Ethernet type, reserved
        MAC address flag, etc. It can be used for applications like VLAN filter (white list as well as
        black list) and ingress VLAN membership check.
CL_VLAN_RANGE_KEY       8 keys/entry
         The key contains a high and a low VLAN, indicating a VLAN range. The key is similar to a
         the VLAN range fields in the Key  of the VLAN Engine
CL_PROTO_KEY               4 keys/entry
          The key contains Layer2, Layer3, and Layer4 fields to identify various types of protocols.
----------------------------------------------------------------------------------------*/

#define CL_IPV4_LONG_KEY          0x0
#define CL_IPV4_SHORT_KEY         0x1
#define CL_IPV6_LONG_KEY          0x2
#define CL_IPV6_SHORT_KEY         0x3
#define CL_MIRROR_KEY             0x4
#define CL_MAC_VLAN_KEY           0x5
#define CL_MAC_ETYPE_KEY          0x6
#define CL_CTRL_PKT_KEY           0x7
#define CL_VLAN_KEY               0x8
#define CL_VLAN_RANGE_KEY         0x9
#define CL_PROTO_KEY              0xa
#define CL_MCST_KEY               0xb
#define CL_MAC_VLAN_KEY_C         0xc
#define CL_MAC_ETYPE_KEY_D        0xd
#define CL_VLAN_KEY_E             0xe
#define CL_RSVD_KEY               0xf


#define CL_PORT_TYPE_NUM               3
#define CL_KEY_TYPE_NUM                16
#define CL_ENTRY_MAXNUM                47
#define CL_FIB_ENTRY_MAXNUM            384 
#define CL_SUB_KEY_NUM                 8
#define CL_MAC_AGING_MAXNUM            63 


/*bit mask for Classifier KEY:-------------------------
*CL_IPV4_LONG_KEY or , CL_IPV6_LONG_KEY 
* CL_IPV4_SHORT_KEY or IPV6_SHORT_KEY
* CL_MIRROR_KEY,CL_MAC_VLAN_KEY
*CL_MAC_ETYPE_KEY,CL_CTRL_PKT_KEY,CL_PROTO_KEY
-----------------------------------------------*/
#define CL_KEY_MASK_SPID                  0x00000001
#define CL_KEY_MASK_MACSA                 0x00000002
#define CL_KEY_MASK_MACDA                 0x00000004
#define CL_KEY_MASK_MACADDR               0x00000008
#define CL_KEY_MASK_EV2PT                 0x00000010
#define CL_KEY_MASK_VLANID                0x00000020
#define CL_KEY_MASK_TPID                  0x00000040
#define CL_KEY_MASK_DOT1P                 0x00000080
#define CL_KEY_MASK_IPSA                  0x00000100
#define CL_KEY_MASK_IPDA                  0x00000200
#define CL_KEY_MASK_IPADDR                0x00000400
#define CL_KEY_MASK_DSCP                  0x00000800
#define CL_KEY_MASK_IPPROTO               0x00001000
#define CL_KEY_MASK_IPFRAGMENT            0x00002000
#define CL_KEY_MASK_IPOPTION              0x00004000
#define CL_KEY_MASK_L4PORT                0x00008000
#define CL_KEY_MASK_L4SRC_RANGE           0x00010000
#define CL_KEY_MASK_L4DST_RANGE           0x00020000
#define CL_KEY_MASK_TCP_FLAGS             0x00040000
#define CL_KEY_MASK_SPECPKT               0x00080000
#define CL_KEY_MASK_ICMP_TYPE             0x00100000
#define CL_KEY_MASK_PRSR_LEN_EXCD         0x00200000
#define CL_KEY_MASK_P_TAG                 0x00400000
#define CL_KEY_MASK_VLAN_COUNT            0x00800000
#define CL_KEY_MASK_IGMP_TYPE             0x01000000
#define CL_KEY_MASK_MACDA_RANGE_FLAG      0x02000000
#define CL_KEY_MASK_AGE_TIMER             0x04000000
#define CL_KEY_MASK_MAC_DA_LOW            0x08000000
#define CL_KEY_MASK_DA_FLAGS              0x10000000
#define CL_KEY_MASK_FLOW_LABEL            0x20000000
#define CL_KEY_MASK_MIRROR_GROUP          0x40000000
#define CL_KEY_MASK_ALWAYS_MATCH          0x80000000

/*bit mask for Classifier KEY:-------------------------
*CL_VLAN_KEY ,CL_VLAN_RANGE_KEY
--------------------------------------------*/
#define CL_VLAN_KEY_MASK_TPID_ENC              0x00000001
#define CL_VLAN_KEY_MASK_EV2PT_ENC             0x00000002
#define CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG       0x00000004
#define CL_VLAN_KEY_MASK_BC_FLAG               0x00000008
#define CL_VLAN_KEY_MASK_MC_FLAG               0x00000010
#define CL_VLAN_KEY_MASK_P_TAG_FLAG            0x00000020
#define CL_VLAN_KEY_MASK_VLAN_COUNT            0x00000040
#define CL_VLAN_KEY_MASK_VLANID                0x00000080
#define CL_VLAN_KEY_MASK_BC_LLID               0x00000100

/* Fib fields mask*/
#define CL_RSLT_MASK_PERMIT                           (1)
#define CL_RSLT_MASK_DPID                             (1 << 1)
#define CL_RSLT_MASK_VLAN                             (1 << 2)
#define CL_RSLT_MASK_DSCP                             (1 << 3)
#define CL_RSLT_MASK_8021P                            (1 << 4)
#define CL_RSLT_MASK_COS                              (1 << 5)
#define CL_RSLT_MASK_L2_LEARN                         (1 << 6)
#define CL_RSLT_MASK_RATE_LIMITER                     (1 << 7)

/*  classification special header structure */

typedef struct {

    cs_uint16      spec_key;
    cs_uint16      spec_msk; 
    cs_boolean     spec_ms_vld;
    cs_uint8       spec_hdr_type;
    cs_uint16      rsvd;
    
} cl_spec_key_cfg_t;


#define CL_L4_SRC_TCP     0x0
#define CL_L4_SRC_UDP     0x1
#define CL_L4_DST_TCP     0x2
#define CL_L4_DST_UDP     0x3
  

typedef struct {
    cs_uint16   L4PortHigh;
    cs_uint16   L4PortLow;   
    cs_uint16   valid; /*L4key_chk_vld*/
    
} cl_l4key_cfg_t;


/***************************************************
* 1: corresponding MAC entry  is dynamic and is
*     valid only if the age timer has not expired
* 0:corresponding MAC entry  is static and the age
*     timer is ignored
*****************************************************/
typedef struct {

    cs_boolean     valid;    /*1 dynamic/0 static */
    cs_uint32      time;  

} cl_mac_age_cfg_t;


typedef struct{
    
        cs_uint32                       mask;            // 1:care;0:no care.
        cs_uint8                        dstmac[6];
        cs_uint8                        srcmac[6];
        cs_uint16                       vlanid;
        cs_uint8                        spid;          
        cs_uint8                        dot1p;
        cs_uint16                       ev2pt;      
        cs_uint8                        tpid_enc; 
        cs_uint8                        vlan_count;
        cs_uint32                       dstip;
        cs_uint32                       srcip;
        cs_uint8                        dstipmask;
        cs_uint8                        srcipmask;
        cs_uint8                        ipprotocol;
        cs_uint8                        ipdscp;
        cs_uint8                        ip_fragment_state; 
        cs_uint8                        ip_option_flag; 
        cs_uint8                        prsr_len_excds;
        cs_uint8                        p_tag_flag;
        cs_uint16                       l4_port;
        cs_uint8                        l4_port_type;    // 0,Destination port/1,Source port
        cs_uint8                        tcp_flags;       // low 6bits
        cs_uint8                        l4_src_hit_map;
        cs_uint8                        l4_dest_hit_map;
        cs_uint8                        spcl_hit_map;
        cs_uint8                        icmp_type;
        cs_uint8                        igmp_type;
        cs_uint8                        rsvd[3];
     
}cl_ipv4_long_key_t;


typedef struct{

        cs_uint32                       mask;   // 1:care;0:no care.
        cs_uint16                       vlanid;
        cs_uint8                        tpid_enc;
        cs_uint8                        ev2pt_enc;      
        cs_boolean                      rsvd_mac_da_flag;
        cs_boolean                      bc_flag;
        cs_boolean                      mc_flag;
        cs_boolean                      p_tag_flag;   
        cs_uint8                        vlan_count;
        cs_uint8                        bc_llid;
        cs_uint16                       rsvd;    
        
}cl_vlan_key_t;


typedef struct{
    
        cs_uint32                       mask;            // 1:care;0:no care.
        cs_uint32                       flow_label;
        cs_uint8                        dstmac[6];
        cs_uint8                        srcmac[6];
        cs_uint16                       vlanid;
        cs_uint8                        spid;          
        cs_uint8                        dot1p;
        cs_uint16                       ev2pt;      
        cs_uint8                        tpid_enc; 
        cs_uint8                        vlan_count;
        cs_uint16                       ipv6_da[8];
        cs_uint16                       ipv6_sa[8];
        cs_uint8                        dstipmask;
        cs_uint8                        srcipmask;
        cs_uint8                        ipprotocol;
        cs_uint8                        ipdscp;
        cs_uint8                        ip_fragment_state; 
        cs_uint8                        ip_option_flag; 
        cs_uint8                        prsr_len_excds;
        cs_uint8                        p_tag_flag;
        cs_uint16                       l4_port;
        cs_uint8                        l4_port_type;     // 0,Destination port/1,Source port
        cs_uint8                        tcp_flags;        // low 6bits
        cs_uint8                        l4_src_hit_map;
        cs_uint8                        l4_dest_hit_map;
        cs_uint8                        spcl_hit_map;
        cs_uint8                        icmp_type;
        cs_uint8                        igmp_type;
        cs_uint8                        rsvd[3];    
       
}cl_ipv6_long_key_t;


typedef struct{

        cs_uint32                       mask;          // 1:care;0:no care.
        cs_uint32                       ip;
        cs_uint8                        ipmask;
        cs_uint8                        ip_type;       // 0,dst ip;1 src ip
        cs_uint8                        ipprotocol;
        cs_uint8                        dscp_tos;
        cs_uint16                       l4_port;
        cs_uint8                        l4_port_type;  // 0,Destination port/1,Source port
        cs_uint8                        prsr_len_excds;   
        
}cl_ipv4_short_key_t;


typedef struct{

        cs_uint32                       mask;          // 1:care;0:no care.
        cs_uint16                       ipv6_addr[8];
        cs_uint8                        ipmask;
        cs_uint8                        ip_type;       //0,dst ip;1 src ip
        cs_uint8                        ipprotocol;
        cs_uint8                        dscp_tos;
        cs_uint16                       l4_port;
        cs_uint8                        l4_port_type;  //0,Destination port/1,Source port
        cs_uint8                        prsr_len_excds;
        cs_uint32                       flow_label;    //LOW 20 BITS
          
}cl_ipv6_short_key_t;


typedef struct{

        cs_uint32                       mask;           // 1:care;0:no care.
        cs_uint16                       vlanid;
        cs_uint8                        spid;        
        cs_uint8                        mirror_group_id;//Low 5 bits
        cs_boolean                      mirror_pkt_flag;
        cs_uint8                        rsvd;  
           
}cl_mirror_key_t;


typedef struct{

        cs_uint32                       mask;            // 1:care;0:no care.
        cs_uint8                        mac[6];
        cs_uint8                        mac_type;
        cs_uint8                        dot1p;
        cs_uint16                       vlanid;
        cs_uint8                        p_tag_flag;
        cs_uint8                        vlan_count;
        cs_uint8                        age_timer_offset;   
        cs_uint8                        rsvd[3];    
             
}cl_mac_vlan_key_t;


typedef struct{

        cs_uint32                       mask;             // 1:care;0:no care.
        cs_uint8                        mac[6];
        cs_uint8                        mac_type;
        cs_uint8                        tpid_enc; 
        cs_uint16                       ev2pt;
        cs_boolean                      mac_da_range_flag;
        cs_uint8                        age_timer_offset;   
        
}cl_mac_etype_key_t;


typedef struct{

        cs_uint32                       mask;              // 1:care;0:no care.
        cs_uint8                        mac_da[6];
        cs_uint8                        macmask;
        cs_boolean                      spl_pkt_en;
        cs_uint32                       special_packet_vec;
           
}cl_ctrl_pkt_key_t;


typedef struct{

        cs_uint32                       mask;              // 1:care;0:no care.
        cs_uint16                       vlanid_hi;
        cs_uint16                       vlanid_lo;
        cs_boolean                      inner_vlan_en;
        cs_uint8                        ev2pt_enc;     
        cs_boolean                      bc_flag;
        cs_boolean                      mc_flag;
        cs_uint8                        vlan_count;
        cs_uint8                        rsvd[3];        
        
}cl_vlan_range_key_t;


typedef struct{

        cs_uint32                       mask;             // 1:care;0:no care.
        cs_uint8                        tpid_enc;
        cs_uint8                        ipprotocol;
        cs_uint16                       ev2pt;        
        cs_uint8                        spcl_hit_map; 
        cs_uint8                        l4_src_hit_map;
        cs_uint8                        l4_dest_hit_map;       
        cs_boolean                      myMAC_flag;            
        cs_boolean                      rsvd_mac_da_flag;    
        cs_boolean                      mcst_ip_flag;    
        cs_uint8                        mac_da_low;
        cs_uint8                        icmp_type;
        cs_uint8                        prsr_len_excds;
        cs_boolean                      cfi_flag;
        cs_uint8                        rsvd[2];       
          
}cl_proto_key_t;


typedef struct{

        cs_uint32                       mask;              // 1:care;0:no care.     
        cs_uint8                        addr[16];
        cs_uint8                        addr_type;
        cs_boolean                      source_addr_vld;
        cs_uint16                       vlanId;
        cs_boolean                      group_id_vld;
        cs_uint8                        group_id;
        cs_uint8                        prsr_len_excds;
        cs_uint8                        rsvd;
        
}cl_mcst_key_t;


/* Classifcation rule entry table*/
typedef struct {

    cs_uint8      keytype;
    cs_boolean    entryvalid;
      
    union {
        
        cl_ipv6_long_key_t       cl_ipv6_long_key;
        cl_ipv4_long_key_t       cl_ipv4_long_key; 
        cl_ipv6_short_key_t      cl_ipv6_short_key;
        cl_ipv4_short_key_t      cl_ipv4_short_key[4];
        cl_mirror_key_t          cl_mirror_key[8];
        cl_mac_vlan_key_t        cl_mac_vlan_key[4];
        cl_mac_etype_key_t       cl_mac_etype_key[4];
        cl_ctrl_pkt_key_t        cl_ctrl_pkt_key[4];
        cl_vlan_key_t            cl_vlan_key[8];
        cl_vlan_range_key_t      cl_vlan_range_key[8];
        cl_proto_key_t           cl_proto_key[4];
        cl_mcst_key_t            cl_mcst_key[2];
        
    } u;
  
} cl_key_entry_t;


typedef struct{
        cs_uint32            mask;            
        cs_boolean           valid;
        cs_boolean           permit;
        cs_boolean           permit_filter_dis;
        cs_boolean           permit_pri;                      
        cs_uint8             dpid;  
        cs_boolean           cl_mirror_en;      
        cs_boolean           cl_mirror_src_sel;      
        cs_uint8             inner_vlan_cmd;        
        cs_uint8             inner_tpid_sel;
        cs_uint16            inner_vlanid;
        cs_uint8             top_vlanid_sel;
        cs_uint8             top_vlan_cmd;
        cs_uint16            top_vlanid;            
        cs_uint8             top_tpid_sel;        
        cs_uint8             eg_chk_vlan_sel;        
        cs_uint8             dscp_sel;        
        cs_uint8             dscp;
        cs_uint8             inner_8021p_sel;         
        cs_uint8             inner_8021p;   
        cs_uint8             top_8021p_sel; 
        cs_uint8             top_8021p;       
        cs_uint8             cos_sel;         
        cs_uint8             cos;         
        cs_boolean           learn_dis;
        cs_boolean           l2_limit_permit;
        cs_uint8             flowId_sel;         
        cs_uint8             flowID;
        cs_boolean           rate_limiter_bypass; 
        cs_uint8             rsvd[2];
     

}cl_fib_data_t;


/*CE ctrl mask*/
#define CL_CFG_MASK_AGING_EN                (1)
#define CL_CFG_MASK_CL_START                (1 << 1)
#define CL_CFG_MASK_DENY_MATCH              (1<<2)
#define CL_CFG_MASK_TOS_ECN_EN              (1<<3)
#define CL_CFG_MASK_CE_CFI_FLAG             (1<<4)
#define CL_CFG_MASK_CE_MAX_NUM              (1<<5)

typedef union {
    struct {
    cs_uint32   type_ipv4_long_key    : 1;
    cs_uint32   type_ipv4_short_key   : 1;
    cs_uint32   type_ipv6_long_key    : 1;
    cs_uint32   type_ipv6_short_key   : 1;
    cs_uint32   type_mirror_key       : 1;
    cs_uint32   type_mac_vlan_key     : 1;
    cs_uint32   type_mac_etype_key    : 1;
    cs_uint32   type_ctrl_pkt_key     : 1;
    cs_uint32   type_vlan_key         : 1;
    cs_uint32   type_vlan_range_key   : 1;
    cs_uint32   type_proto_key        : 1;
    cs_uint32   type_mcast_key        : 1;
    cs_uint32   type_mac_vlan_key_c   : 1;
    cs_uint32   type_mac_etype_key_d  : 1;
    cs_uint32   type_vlan_key_e       : 1;
    cs_uint32   type_rsv_key_f        : 1;
    cs_uint32   rsvd                  : 16;
            
    }s;
    cs_uint32   wrd;
} deny_match_t;

/*CLS ctrl */
typedef struct {
        cs_uint32         mask;      
        cs_boolean        mac_filter_aging_en; 
        cs_uint8          cl_start_entry;
        cs_boolean        tos_ecn_msk_en;
        cs_boolean        ce_msk_cfi_flg;
        deny_match_t      deny_no_type_match;
        cs_uint8          dbg_ce_max_entry;
          
}cl_ctrl_cfg_t;



cs_status cl_entry_valid_set(cs_uint8 port_id, cs_uint8 entryindex, cs_boolean  status);
cs_status cl_entry_valid_get(cs_uint8 port_id, cs_uint8 entryindex, cs_boolean *status);
cs_status cl_rule_mode_set(cs_uint8 port_id, cs_uint8 entryindex, cs_boolean  status);
cs_status cl_rule_mode_get(cs_uint8 port_id, cs_uint8 entryindex, cs_boolean *status);
cs_status cl_spec_key_set(cs_uint8 port_id, cs_uint8 addr, cl_spec_key_cfg_t * pSpecKeyCfg);
cs_status cl_spec_key_get(cs_uint8 port_id, cs_uint8 addr, cl_spec_key_cfg_t * pSpecKeyCfg);
cs_status cl_l4_key_set(cs_uint8 port_id, cs_uint8 index, cs_uint8 port_type, cl_l4key_cfg_t * pdata);
cs_status cl_l4_key_get(cs_uint8 port_id, cs_uint8 index, cs_uint8 port_type, cl_l4key_cfg_t * pdata);
cs_status cl_mac_filter_aging_time_set(cs_uint8 port_id, cs_uint8 addr, cl_mac_age_cfg_t * pmacageCfg);
cs_status cl_mac_filter_aging_time_get(cs_uint8 port_id, cs_uint8 addr, cl_mac_age_cfg_t * pdata);
cs_status cl_ctrl_set(cs_uint8 port_id, cl_ctrl_cfg_t *pdata);
cs_status cl_ctrl_get(cs_uint8 port_id, cl_ctrl_cfg_t *pdata);
cs_status cl_default_rule_set(cs_uint8 port_id, cl_fib_data_t *pdata);
cs_status cl_default_rule_get(cs_uint8 port_id, cl_fib_data_t *pdata);
cs_status cl_fib_table_set(cs_uint8 port_id, cs_uint16 addr, cl_fib_data_t *pdata);
cs_status cl_fib_table_get(cs_uint8 port_id, cs_uint16 addr, cl_fib_data_t *pdata);
cs_status cl_key_entry_set(cs_uint8 port_id, cs_uint16 addr, cl_key_entry_t *pdata);
cs_status cl_key_entry_get(cs_uint8 port_id, cs_uint16 addr, cl_key_entry_t *pdata);
cs_status cl_entry_link_list_set(cs_uint8 port_id, cs_uint8 addr, cs_uint8 cl_next_entry);
cs_status cl_entry_link_list_get(cs_uint8 port_id, cs_uint8 addr, cs_uint8 *cl_next_entry);
void cls_util_init(void);



#endif /* __AAL_CLS_UTIL_H__ */

