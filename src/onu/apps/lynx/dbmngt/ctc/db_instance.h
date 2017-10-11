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

DEFINITIONS:  "DEVICE" means the Cortina Systems? Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems? SDK software.

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
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING OUT
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

#ifndef __DB_INSTANCE_H__
#define __DB_INSTANCE_H__
#ifdef HAVE_DB_MANAGMENT
#include "db_api.h"
#include "oam_ctc_pdu.h"

#define DB_CUR_VERSION   0x0

#define DB_CLASS_FIELD_SELECT_MAX  3
#define DB_CLASS_MATCH_VAL_LEN     16
#define DB_MC_VLAN_PER_PORT_MAX  8
#define DB_MAX_ALARM_ENTRY  128
#ifdef HAVE_MPORTS
#define DB_MAX_MTU_LEN        1632
#define DB_CLS_RULE_TBL_MAX   12
#define DB_MAX_PORT       4
#else
#define DB_MAX_MTU_LEN        1900
#define DB_CLS_RULE_TBL_MAX   8
#define DB_MAX_PORT       1
#endif
#define DB_MIN_MTU_LEN    1518
enum{
    DB_LOID_PASSWD_ID = 1,
    /* admin user information */
    DB_ADMIN_INFO_ID ,
    /* power user infomation */
    DB_USER_INFO_ID,
    /* onu ip,mask and mgmt vlan, subnet */
    DB_MGMT_INFO_ID,
    /* serial number */
    DB_SN_ID,
    /* Backup battery   */
    DB_BACKUP_BATTERY_ID,
    /* fec,port iso,mac aging time */ 
    DB_ONU_SVC_INFO_ID,
    /* Port mirror */
    DB_PORT_MIRROR_ID,
    /* Voq info  */
    DB_QOS_VOQ_ID,
    /* Qos remark */
    DB_QOS_REMARK_ID,
    /* port admin, auto, bc ctrl */
    DB_PORT_INFO_ID,   
    /* mac bind */
    DB_MAC_BIND_ID,
    /* onu vlan  */
    DB_VLAN_ID,    
    /* onu profile  */
    DB_PROFILE_ID,
    /* Guest info */
    DB_GUEST_INFO_ID,
    /* Telnet admin info */
    DB_TEL_ADMIN_INFO_ID,
    /* Telnet user info*/
    DB_TEL_USER_INFO_ID,
    /* Telnet guest info*/
    DB_TEL_GUEST_INFO_ID,
    
    DB_MAC_LIMIT_ID,
    DB_MAC_FILTER_ID,
    DB_STATIC_MAC_ID,
    DB_CLS_ID,
    DB_MC_VLAN_ID,
    DB_MC_TAGSTRIP_ID,
    DB_MC_MAX_GRP_ID,
    DB_FAST_LEAVE_ID,
    DB_ONU_ALARM_ID,
    DB_PORT_ALARM_ID,
    DB_TIMEOUT_ID,
	#ifdef HAVE_TERMINAL_SERVER
	//added by zhuxh
	DB_CTC_TRM_SVR0_CFG_ID,
    DB_CTC_TRM_SVR1_CFG_ID,
    DB_CTC_TRM_SVR2_CFG_ID,
    DB_CTC_TRM_SVR3_CFG_ID,
	#endif
	
};

#define DB_LOID_MAX_LEN  24
#define DB_PWD_MAX_LEN   12

typedef struct{
    cs_uint8 loid[DB_LOID_MAX_LEN + 1];
    cs_uint8 passwd[DB_PWD_MAX_LEN + 1];
    cs_uint16 rsvd;
}db_loid_pwd_t;
/*************** User infomation define *******************/
#define DB_MAX_USR_LEN  64
#define DB_MAX_PWD_LEN  64
#define DB_DEFAULT_ADMIN_NAME  "admin" 
#define DB_DEFAULT_ADMIN_PWD  "admin"
#define DB_DEFAULT_USR_NAME  "user" 
#define DB_DEFAULT_USR_PWD  "user" 
#define DB_DEFAULT_GUEST_NAME  "guest" 
#define DB_DEFAULT_GUEST_PWD   "guest" 

#define PER_LEVEL_MAX_ACCOUNT  3

typedef struct{
    cs_uint8    usr[DB_MAX_USR_LEN + 1];
    cs_uint8    pwd[DB_MAX_PWD_LEN + 1];
    cs_uint16   valid;
}db_user_t;

/*********** Onu manangement information  ***********************/
#define DB_DEFAULT_IP  0xc0a80101   //192.168.1.1
#define DB_DEFAULT_MASK  0xffffff00   //255.255.255.0
#define DB_DEFAULT_GW  0xc0a80101   //192.168.1.1
#define DB_DEFAULT_MGMT_MTU  1500
#define DB_DEFAULT_MGMT_NET     0xc0a80100  /*192.168.1.1*/
#define DB_DEFAULT_MGMT_MASK    0xffffff00  /*255.255.255.0 */
#define DB_DEFAULT_INBAND_VLAN  100 
#define DB_DEFAULT_INBAND_PRI   1 
#define DB_DEFAULT_INBAND_MODE  2  /* disable */
typedef struct{
    cs_uint32   ip;
    cs_uint32   mask;
    cs_uint32   inband_ip;
    cs_uint32   inband_mask;
    cs_uint32   gw; 
    cs_uint32   mgmt_mtu;
    cs_uint32   mgmt_net;
    cs_uint32   mgmt_mask;
    cs_uint16   inband_vlan; 
    cs_uint8    inband_pri; 
    cs_uint8    inband_enable;
}db_mgmt_t;


/************* ONU serial number ******************************/
#define DB_SN_LEN              128 
typedef struct{
    cs_uint8 sn[DB_SN_LEN + 1];
    cs_uint8 rsvd[3];
}db_sn_t;

/***************ONU backup battery ***************************/
typedef struct{
    cs_uint8 alarm_sig; 
    cs_uint8 alarm_status;
    cs_uint8 log_state;
    cs_uint8 alarm_info;
}db_backup_battery_entry_t;

#define DB_BATTERY_CFG_NUM  5
typedef struct{
    db_backup_battery_entry_t  battery[DB_BATTERY_CFG_NUM];
}db_backup_battery_t; 



/********************* ONU service information ******************/
#define  DB_QUEUE_PRIO_LEN  8

#define  DB_DEFAULT_FEC_MODE  0                      
#if 1
//modefied by zhuxh, default port isolation should be enabled
#define  DB_DEFAULT_PORT_ISO  1
#else
#define  DB_DEFAULT_PORT_ISO  0
#endif
#define  DB_DEFAULT_UPLINK_FLOW_CTRL  0

#define  DB_DEFAULT_MAX_MTU    1536 

#define  DB_DEFAULT_QOS_MODE  0
#define  DB_DEFAULT_MAC_AGING_TIME   300

#define  DB_DEFAULT_MC_AGING_TIME   260
#define  DB_DEFAULT_MC_MODE     0 /*snooping */

#define  DB_MAX_CBS 511744   

#define   DB_MAC_FILTER_FLAG   0
#define   DB_MAC_BIND_FLAG      1

typedef struct{
    cs_uint8    fec_mode;
    cs_uint8    port_iso; 
    cs_uint8    uplink_flow_ctrl;
    cs_uint8    qos_sched_mode;

    cs_uint8    pri_cos_map[DB_QUEUE_PRIO_LEN];

    cs_uint16   max_mtu;
    cs_uint8    rsvd;
    cs_uint8    ds_shape_en;
    cs_uint32   ds_shape_rate;
    cs_uint8    ppoe_plus_en;
    cs_uint8    rstp;
    cs_uint8    tcp;
    cs_uint8    mc_mode;
    cs_uint32   mc_time;
    cs_uint32   mac_aging_time;
    cs_uint32   cfg_counter;
    cs_uint32   ds_cbs;
}db_onu_svc_t;


/********************ONU QOS remark ********************/
typedef struct{
    cs_boolean remark_en;
    cs_uint8   remark[DB_QUEUE_PRIO_LEN];
    cs_uint8   rsvd[3];
}db_remark_entry_t;

typedef struct{
    db_remark_entry_t remark[DB_MAX_PORT];
}db_remark_t;


/******************* ONU Voq information ******************/
#define DB_MAX_VOQ_SIZE 8
#define DB_DEFAULT_DN_VOQ_THRESH  230 
#define DB_DEFAULT_UP_VOQ_THRESH  384

#define DB_MAX_RATE                             1000000

#define DB_NON_FAST_LEAVE                 0x1
#define DB_FAST_LEAVE                          0x2

typedef struct{
    /* 0 for downstream
     * 1 for upstream */
    cs_uint32 voq_cfg[2][DB_MAX_VOQ_SIZE];
}db_voq_t;
    
/******************** Port base infomation ********************/
typedef struct{
    cs_uint8             admin;
    cs_uint8             auto_neg;
    cs_uint8             work_mode;
    cs_uint8             pause;

    cs_uint8             loop_detect;
    cs_uint8             rsvd[2];
    cs_uint8             policy_enable;
    cs_uint32            policy_rate;
    cs_uint32            policy_cbs;
    cs_uint32            policy_ebs;
    cs_uint8             rsvd2[3];
    cs_uint8             bc_ctrl_mode;
    cs_uint32            bc_ctrl_rate;
    cs_uint8             ctrl_enable;
    cs_uint8             rsvd3[2];
    cs_uint8             ds_rate_limit_enable;
    cs_uint32            ds_rate_limit;
    cs_uint32            ds_cbs;
} db_port_entry_t;

typedef struct{
    db_port_entry_t port[DB_MAX_PORT];
} db_port_t;


/******************* Mac bind information *************************/
#define DB_MAX_MAC_BIND_NUM      16
typedef struct{
    cs_uint16 num;
    cs_uint16  rsvd;
    cs_mac_t mac[DB_MAX_MAC_BIND_NUM];
} db_mac_bind_entry_t;

typedef struct{
    db_mac_bind_entry_t  bind[DB_MAX_PORT];
}db_mac_bind_t;


/********************Port mirror ****************************/
typedef struct{
    cs_uint8 enable;
    cs_uint8 rsvd[3];
    cs_uint32 rx_mask;
    cs_uint32 tx_mask;
}db_mirror_t;


/******************** Vlan information  *********************/
#define DB_VLAN_MAX  16
typedef struct {
        cs_uint16 tpid;
        cs_uint16 tag;
}db_vlan_tag_t;

typedef struct{
    cs_uint8   vlan_mode;
    cs_uint8   vlan_num;
    cs_uint16  agg_vlan_num;
    db_vlan_tag_t  def_vlan;
    db_vlan_tag_t vlan[DB_VLAN_MAX *2];
}db_vlan_entry_t;

typedef struct{
    db_vlan_entry_t vlan[DB_MAX_PORT];
}db_vlan_t;


typedef struct{
    cs_uint32  num;
}db_mac_limit_entry_t;

typedef struct{
    db_mac_limit_entry_t  mac[DB_MAX_PORT];
}db_mac_limit_t;

typedef struct{
    cs_uint32  time;
}db_mac_age_t;


typedef struct{
    cs_mac_t mac[DB_MAX_MAC_BIND_NUM];
    cs_uint16 vlan;
    cs_uint16 num;
}db_mac_entry_t;  

        
typedef struct{
    db_mac_entry_t  entry[DB_MAX_PORT];    
}db_static_mac_t;        


typedef struct{
    cs_uint8 enable; /*0 disbale, 1 enable*/
    cs_uint8  reserv[3];
    cs_uint32 cir;
    cs_uint32 cbs;
}db_dn_shape_rate_t;    


typedef struct {
        cs_uint32    fieldSelect;
        cs_uint8     matchValue[DB_CLASS_MATCH_VAL_LEN];
        cs_uint32   validationOper;
} db_cls_rule_t;


typedef struct {
        cs_uint8  precedence;
        cs_uint8  queueMapped;
        cs_uint8  priMark;
        cs_uint8  entries;
        db_cls_rule_t fselect[DB_CLASS_FIELD_SELECT_MAX];
} db_classification_t;

typedef struct{
    db_classification_t   rule[DB_CLS_RULE_TBL_MAX];
    cs_uint32                     num;
}db_cls_entry_t;   

typedef struct{
    db_cls_entry_t   cls[DB_MAX_PORT];
}db_cls_t; 

typedef struct{
    cs_uint16    vlan[DB_MC_VLAN_PER_PORT_MAX];
    cs_uint32    num;
}db_mc_vlan_entry_t;   

typedef struct{
    db_mc_vlan_entry_t  entry[DB_MAX_PORT];
}db_mc_vlan_t; 



typedef struct{
    cs_uint32    tagStriped;
    cs_uint16    vlan[DB_MC_VLAN_PER_PORT_MAX*2];
    cs_uint32    num;
}db_mc_tagstrip_entry_t;   
        
typedef struct{
    db_mc_tagstrip_entry_t  entry[DB_MAX_PORT];
}db_mc_tagstrip_t;   
        
typedef struct{
    cs_uint32        mc_grp[DB_MAX_PORT];
}db_mc_grp_t;   


typedef struct {
    cs_uint32       fast_leave;
}db_fast_leave_t;   


typedef struct {
    cs_uint16      alarm_code;
    cs_uint8        alarm_type;
    cs_uint8        enable;           /*alarm on or alarm off*/

    cs_uint8        reserve[3];
    cs_uint8         threshold_type;
    cs_uint32       raise_threshold;
    cs_uint32       clear_threshold;
    
}db_alarm_entry_t; 

typedef struct {
    db_alarm_entry_t    entry[DB_MAX_ALARM_ENTRY];
    cs_uint32                      num;
}db_onu_alarm_t; 

typedef struct {
    db_onu_alarm_t    onu_entry[DB_MAX_PORT];
}db_port_alarm_t; 


#define CTC_PROFILE_NAME_LEN                20
#define CTC_VLAN_MODE_TRANSLATE_MAX         16
#define CTC_VLAN_MODE_TRUNK_MAX             16

typedef struct {
    cs_int32 used;
    cs_int8  profile_name[CTC_PROFILE_NAME_LEN];
    cs_int8  phyadmin;
    cs_int8  mode;
    cs_int8  flow_ctrl;
    cs_int8  us_limited;
    cs_int32 us_rate;    
    cs_int32 ds_rate;
    cs_int16 dvid;
    cs_int16 igmp_aging_time; 
    cs_uint16 translate_num;
    cs_uint16 trunk_num ;
    oam_ctc_onu_vlan_xlate_entry translate_vlan[CTC_VLAN_MODE_TRANSLATE_MAX];
    oam_ctc_onu_vlan_tag_t trunk_vlan[CTC_VLAN_MODE_TRUNK_MAX];  
    cs_int8  ds_limited;
    cs_int8  vlan_mode;
    cs_int8  igmp_mode;  
}db_profile_struct_t;

#define DB_TIMEOUT_DEFAULT   300
typedef struct{
    cs_uint32   web_timeout;
    cs_uint32   telnet_timeout;
}db_timeout_t;

void db_init();
void db_restore();

cs_status  db_set_default();
#endif
#endif
