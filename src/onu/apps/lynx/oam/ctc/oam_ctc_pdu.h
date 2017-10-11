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

Copyright (c) 2011 by Cortina Systems Incorporated
****************************************************************************/

#ifndef __OAM_CTC_PDU_H__
#define __OAM_CTC_PDU_H__
#ifdef HAVE_CTC_OAM
#include "oam_std_pdu.h"

#define CTC_PORT_ID_PON                     0
#define CTC_PORT_ID_UNI                     1
#define CTC_PORT_ID_MGMT                    2
#define CTC_PORT_ID_ALL_UNI                 0xff
#define CTC_2_1_PORT_ID_ALL_UNI             0xffff

/*57-17 value 0x21 */
#define OAM_CTC_ATTRIB_NOT_SUPPORTED        OAM_ATTRIB_NOT_SUPPORTED 
#define OAM_CTC_VAR_SET_OK                  OAM_VAR_SET_OK
#define OAM_CTC_VAR_SET_BAD_PARAM           OAM_VAR_SET_BAD_PARAM 
#define OAM_CTC_VAR_SET_NO_RES              OAM_VAR_SET_NO_RES 



/* CTC ext opcode definitions */
#define OAM_PDU_CTC_VAR_REQ                 0x01
#define OAM_PDU_CTC_VAR_RESP                0x02
#define OAM_PDU_CTC_SET_REQ                 0x03
#define OAM_PDU_CTC_SET_RESP                0x04
#define OAM_PDU_CTC_ONU_AUTH                0x05
#define OAM_PDU_CTC_FILE_IMG                0x06
#define OAM_PDU_CTC_CHURNING                0x09
#define OAM_PDU_CTC_DBA                     0x0a
#define OAM_PDU_CTC_EVENT                   0xff


/* CTC set response codes */
#define OAM_CTC_SET_OK                      0x80
#define OAM_CTC_SET_BAD_PARAM               0x86
#define OAM_CTC_SET_NO_RES                  0x87
#define OAM_CTC_SET_RESP_BIT(a)             (a | 0x80)

/* CTC Extended Object Class */
#define OAM_CTC_INDEX_BRANCH                0x36
#define OAM_CTC_INDEX_BRANCH_E              0x37
#define OAM_CTC_ATTRIB_BRANCH               0xc7
#define OAM_CTC_ACTION_BRANCH               0xc9


#define CTC_INDEX_DEFAULT                   0

#define CTC_MAX_DATA_LEN                    128


/* CTC Extended MO Instance */
#define OAM_CTC_LEAF_CODE_ONU_PORT          0x0001
#define OAM_CTC_LEAF_CODE_ONU_CARD          0x0002
#define OAM_CTC_LEAF_CODE_ONU_LLID          0x0003
#define OAM_CTC_LEAF_CODE_ONU_PON           0x0004
#define OAM_CTC_LEAF_CODE_ONU_PON_PORT      0x0004






/* Dry Contact Alarm Info */
#define OAM_CTC_ALARM_INFO_UNKNOW_TYPE      0x0000
#define OAM_CTC_ALARM_INFO_TEMPERATURE      0x0001
#define OAM_CTC_ALARM_INFO_HUMIDITY         0x0002
#define OAM_CTC_ALARM_INFO_SMOKE            0x0003
#define OAM_CTC_ALARM_INFO_DOOR_ACCESS      0x0004


#define OAM_CTC_ALARM_STATE_RAISED          OAM_CTC_ALARM_OP_CODE_RAISE
#define OAM_CTC_ALARM_STATE_CLEARED         OAM_CTC_ALARM_OP_CODE_CLEAR




#define OAM_CTC_ALARM_TYPE_ONU              0xffff
#define OAM_CTC_ALARM_TYPE_PON              0x0004
#define OAM_CTC_ALARM_TYPE_UNI              0x0001

typedef struct {
        cs_uint8           oui[OAM_OUI_LEN];
        cs_uint8           ver;
} __attribute__((packed)) oam_ctc_tlv_ext_t;

typedef struct {
        cs_uint8           ext_support;
        cs_uint8           version;
        oam_ctc_tlv_ext_t  extension[0];
} __attribute__((packed)) oam_ctc_tlv_val_t;


typedef struct {
        oam_pdu_hdr_t      hdr;
        cs_uint8           oui[OAM_OUI_LEN];
        cs_uint8           opcode;
        cs_uint8           data[1];
} __attribute__((packed)) oam_pdu_ctc_t;


typedef struct {
        cs_uint16          object_type;
        cs_uint32          instance_num;
        cs_uint16          alarm_id;
        cs_uint16          time_stamp;
        cs_uint8           alarm_state; 
        cs_uint8           alarm_info[1];       /*4 byte or 8 byte*/
} __attribute__((packed)) oam_org_spec_value_t;

typedef struct {
        cs_uint8           type;
        cs_uint8           length;
        cs_uint8           oui[OAM_OUI_LEN];
        oam_org_spec_value_t    org_spec_value;
} __attribute__((packed)) oam_ctc_tlv_org_spec_t;



typedef struct {
        cs_uint8   branch;
        cs_uint16  leaf;
        cs_uint8   width;
        cs_uint8   data[1];
} __attribute__((packed)) oam_ctc_onu_var_set_hdr_t;


/* CTC index of instance container */
typedef struct {
        oam_var_cont_t hdr;
        cs_uint8    value;
} __attribute__((packed)) oam_pdu_ctc_index_t;

typedef enum
{
    OAM_CTC_ETH_PORT =  1,
    OAM_CTC_VOIP_PORT ,
    OAM_CTC_ADSL2_PORT,
    OAM_CTC_VDSL2_PORT,
    OAM_CTC_E1_PORT,
    OAM_CTC_RF_PORT,
    OAM_CTC_PON_PORT = 0x10 
} ctc_oam_port_mode_e;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8    portType;
        cs_uint8    slotInfo;
        cs_uint16   portIndex;
} __attribute__((packed)) oam_pdu_ctc_port_index_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint32 cardIndex;
} __attribute__((packed)) oam_pdu_ctc_card_index_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint32 ponIndex;
} __attribute__((packed)) oam_pdu_ctc_pon_index_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint32 llidIndex;
} __attribute__((packed)) oam_pdu_ctc_llid_index_t;

/***************************************************************/

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8   pon_port;
} __attribute__((packed)) oam_ctc_onu_active_pon_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint32   ip_addr;
        cs_uint32   ip_mask;
        cs_uint32   gateway;
        cs_uint16   cvlan;
        cs_uint16   svlan;
        cs_uint8    priority;
} __attribute__((packed)) oam_ctc_onu_mgmt_config_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8   ip_addr[16];
        cs_uint8   ip_mask[4];
        cs_uint8   gateway[16];
        cs_uint16  cvlan;
        cs_uint16  svlan;
        cs_uint8   priority;
} __attribute__((packed)) oam_ctc_onu_mgmt_config_v6_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8    version;
        cs_uint32   trap_ip;
        cs_uint16   trap_port;
        cs_uint16   snmp_port;
        cs_uint8    name[32];
        cs_uint8    com_rd[32];
        cs_uint8    com_wr[32];
} __attribute__((packed)) oam_ctc_onu_mdu_snmp_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8    version;
        cs_uint8    trap_ip[16];
        cs_uint16   trap_port;
        cs_uint16   snmp_port;
        cs_uint8    name[32];
        cs_uint8    com_rd[32];
        cs_uint8    com_wr[32];
} __attribute__((packed)) oam_ctc_onu_mdu_snmp_v6_t;

/*
typedef struct {
        oam_var_cont_t hdr;
        cs_uint8    sipDigBlockNum;
        cs_uint8    digMapId;
        cs_uint8    digitBlock[1024];
} __attribute__((packed)) oam_ctc_onu_sip_digit_map_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint32   iad_oper;
} __attribute__((packed)) oam_ctc_onu_iad_oper_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint32   port_status;
        cs_uint32   port_service_state;
        cs_uint32   port_codec_mode;
} __attribute__((packed)) oam_ctc_onu_pots_status_t;


typedef struct {
        oam_var_cont_t hdr;
        cs_uint32   oper_status;
} __attribute__((packed)) oam_ctc_onu_h248_iad_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8   voice_t38_enable;
        cs_uint8   voice_control;
} __attribute__((packed)) oam_ctc_onu_fax_modem_config_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8   user_account[16];
        cs_uint8   user_name[32];
        cs_uint8   user_passwd[16];
} __attribute__((packed)) oam_ctc_onu_sip_user_config_t;


typedef struct {
        oam_var_cont_t hdr;
        cs_uint16       mg_port_no;
        cs_uint32       proxy_srv_ip;
        cs_uint16       proxy_srv_port;
        cs_uint32       backup_proxy_srv_ip;
        cs_uint16       backup_proxy_srv_port;
        cs_uint32       active_proxy_srv;
        cs_uint32       reg_srv_ip;
        cs_uint16       reg_srv_port;
        cs_uint32       backup_reg_srv_ip;
        cs_uint16       backup_reg_srv_port;
        cs_uint32       outbound_srv_ip;
        cs_uint16       outbound_srv_port;
        cs_uint32       reg_interval;
        cs_uint8        heartbeat_switch;
        cs_uint16       heartbeat_cycle;
        cs_uint16       heartbeat_count;
} __attribute__((packed)) oam_ctc_onu_sip_param_config_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8    number_of_tid;
        cs_uint8    first_tid_name[32];
} __attribute__((packed)) oam_ctc_onu_h248_rtp_tid_info_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8    number_of_tid;
        cs_uint8    tid_prefix[16];
        cs_uint8    tid_digit_begin[8];
        cs_uint8    tid_mode;
        cs_uint8    tid_digit_len;
} __attribute__((packed)) oam_ctc_onu_h248_rtp_tid_info_config_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8    user_tid_name[32];
} __attribute__((packed)) oam_ctc_onu_h248_user_tid_info_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint16       mg_port_no;
        cs_uint32       mgc_ip;
        cs_uint16       mgc_port;
        cs_uint32       backup_mgc_ip;
        cs_uint16       backup_mgc_port;
        cs_uint8        active_mgc;
        cs_uint8        reg_mode;
        cs_uint8        mid[64];
        cs_uint8        heartbeat_mode;
        cs_uint16       heartbeat_cycle;
        cs_uint8        heartbeat_count;
} __attribute__((packed)) oam_ctc_onu_h248_param_config_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8        voip_mode;
        cs_uint32       iad_ip;
        cs_uint32       iad_ip_mask;
        cs_uint32       iad_default_gw;
        cs_uint8        ppoe_mode;
        cs_uint8        ppoe_username[32];
        cs_uint8        ppoe_passwd[32];
        cs_uint8        tagged_flag;
        cs_uint16       voice_cvlan;
        cs_uint16       voice_svlan;
        cs_uint8        voice_priority;
} __attribute__((packed)) oam_ctc_onu_voip_global_param_config_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8     mac[6];
        cs_uint8     protocol_support;
        cs_uint8     software_ver[32];
        cs_uint8     software_time[32];
        cs_uint8     voip_user_count;
} __attribute__((packed)) oam_ctc_onu_voip_iad_info_t;
*/

typedef struct {
    cs_uint8    queue_num;
    cs_uint16   fixed_packet_size;
    cs_uint16   fixed_bandwidth;
    cs_uint16   guaranteed_bandwidth;
    cs_uint16   best_effort_bandwith;
    cs_uint8    wrr_weigth;
} __attribute__((packed)) oam_sla_queue_t;

#define OAM_CTC_MAX_SLA_NUM   8
typedef struct {
        oam_var_cont_t hdr;
        cs_uint8    oper;
        cs_uint8    scheduling_scheme;
        cs_uint8    high_pri_boundary;
        cs_uint32   cycle_len;
        cs_uint8    num_of_service;
        oam_sla_queue_t sla_info[OAM_CTC_MAX_SLA_NUM];
} __attribute__((packed)) oam_ctc_onu_sla_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8   pon_port;
} __attribute__((packed)) oam_ctc_onu_chip_cap3_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint16   action;
        cs_uint8    mac_address[6];
        cs_uint32   transceiver_id;
} __attribute__((packed)) oam_ctc_onu_laser_control_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint32   action;
        cs_uint8    mac_address[6];
        cs_uint32   transceiver_id;
} __attribute__((packed)) oam_ctc_onu_laser_control_old_t;


/* CTC ONU SN variable container */
#define OAM_CTC_VENDER_ID_LEN               4
#define OAM_CTC_MODEL_ID_LEN                4
#define OAM_CTC_ONU_ID_LEN                  6
#define OAM_CTC_ONU_SN_LEN                  14
#define OAM_CTC_ONU_HW_LEN                  8
#define OAM_CTC_ONU_SW_LEN                  16
#define OAM_CTC_EXTEND_MODEL_ID        16

#define HW_APP_VERSION   "V2R1C01B001T30" 	
#define HW_OAM_APP_VERSION "V2R1C01"

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 venderId[OAM_CTC_VENDER_ID_LEN];
        cs_uint8 model[OAM_CTC_MODEL_ID_LEN];
        cs_uint8 ONUId[OAM_CTC_ONU_ID_LEN];
        cs_uint8 hwVer[OAM_CTC_ONU_HW_LEN];
        cs_uint8 swVer[OAM_CTC_ONU_SW_LEN];
        cs_uint8 extend_model[OAM_CTC_EXTEND_MODEL_ID];
} __attribute__((packed)) oam_ctc_onu_sn_t;

/**************************************************************/
/* CTC ONU firmware version variable container */
#define OAM_CTC_ONU_FW_VERSION_LEN          32

typedef struct {
        cs_uint16         major;
        cs_uint16         minor;
        cs_uint16         revision;
        cs_uint16         build_id;
} oam_image_version_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 fwVersion[OAM_CTC_ONU_FW_VERSION_LEN];
} __attribute__((packed)) oam_ctc_onu_fw_ver_t;

/**************************************************************/
/* CTC ONU chip id variable container */
#define OAM_CTC_ONU_CHIPID_VID_LEN          2
#define OAM_CTC_ONU_CHIPID_MODEL_LEN        2
#define OAM_CTC_ONU_CHIPID_DATE_LEN         3

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 venderId[OAM_CTC_ONU_CHIPID_VID_LEN];
        cs_uint8 chipModel[OAM_CTC_ONU_CHIPID_MODEL_LEN];
        cs_uint8 Revision;
        cs_uint8 Date[OAM_CTC_ONU_CHIPID_DATE_LEN];
} __attribute__((packed)) oam_ctc_onu_chip_id_t;

/**************************************************************/
/* CTC ONU chip capabilities variable container */
#define OAM_CTC_CHIP_CAP_GE_SUPP            0x1
#define OAM_CTC_CHIP_CAP_FE_SUPP            0x2
#define OAM_CTC_CHIP_CAP_VOIP_SUPP          0x4
#define OAM_CTC_CHIP_CAP_TDM_SUPP           0x8
#define OAM_CTC_CHIP_CAP_BITS               OAM_CTC_CHIP_CAP_GE_SUPP
#define OAM_CTC_CHIP_CAP_PORT_BITMAP        0x8

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8    serviceSupported;
        cs_uint8    numGEPorts;
        cs_uint64   geBitMap;
        cs_uint8    numFEPorts;
        cs_uint64   feBitMap;
        cs_uint8    numPOTSPorts;
        cs_uint8    numE1Ports;
        cs_uint8    numUSQueues;
        cs_uint8    maxQueueUSPort;
        cs_uint8    numDSQueues;
        cs_uint8    maxQueueDSPort;
        cs_uint8    BatteryBackup;
} __attribute__((packed)) oam_ctc_onu_capabilities_t;

#define MAX_MDU_PORTS_TYPES         2
#define MDU_ONU_TYPE                0x00000003
#define MDU_IF_TYPE_GE              0x00000000
#define MDU_IF_TYPE_FE              0x00000001


typedef struct {
        oam_var_cont_t hdr;
        cs_uint16 alarm_id;
        cs_uint32 alarm_config;
} __attribute__((packed)) oam_ctc_onu_alarm_t;


typedef struct {
        oam_var_cont_t hdr;
        cs_uint16 alarm_id;
        cs_uint32 alarm_threshold;
        cs_uint32 clear_threshold;
} __attribute__((packed)) oam_ctc_onu_alarm_threshold_t;



#define OAM_CTC_ONU_LOOPDETECT_DEACTIVED      0x00000001
#define OAM_CTC_ONU_LOOPDETECT_ACTIVED        0x00000002

typedef struct {
        oam_var_cont_t hdr;
        cs_uint32 status;
} __attribute__((packed)) oam_ctc_onu_loopdetect_t;

#define OAM_CTC_ONU_DISABLE_LOOPED_DEACTIVED      0x00000000
#define OAM_CTC_ONU_DISABLE_LOOPED_ACTIVED        0x00000001

typedef struct {
        oam_var_cont_t hdr;
        cs_uint32 status;
} __attribute__((packed)) oam_ctc_onu_disable_loopded_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint32 time;
} __attribute__((packed)) oam_ctc_onu_mac_aging_time_t;


typedef struct {
        oam_var_cont_t hdr;
        cs_uint16  mac_limit;
} __attribute__((packed)) oam_ctc_onu_mac_limit_t;

#define     CTC_OAM_PORT_MODE_AUTO              0
#define     CTC_OAM_PORT_MODE_10M_HALF          1 
#define     CTC_OAM_PORT_MODE_10M_FULL          2
#define     CTC_OAM_PORT_MODE_100M_HALF         3
#define     CTC_OAM_PORT_MODE_100M_FULL         4
#define     CTC_OAM_PORT_MODE_1000M_FULL        5

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8  port_mode;
} __attribute__((packed)) oam_ctc_onu_port_mode_t;




typedef struct {
        cs_uint64 in_octets; 
        cs_uint64 in_ucast_pkt;
        cs_uint64 in_nucast_pkt;
        cs_uint64 in_discard;
        cs_uint64 in_error;
        cs_uint64 out_octets;
        cs_uint64 out_ucast_pkt;
        cs_uint64 out_nucast_pkt;
        cs_uint64 out_discard;
        cs_uint64 out_error;
        cs_uint64 resverved1;
        cs_uint64 resverved2;
        cs_uint64 resverved3;
        cs_uint64 resverved4;
} __attribute__((packed)) oam_ctc_eth_port_stats_t;

typedef struct{
    oam_var_cont_t hdr;
    oam_ctc_eth_port_stats_t  stats;
} __attribute__((packed)) oam_ctc_onu_eth_port_stats_t;


typedef struct {
    oam_var_cont_t hdr;
    cs_uint64 out_pkts;
    cs_uint64 out_octets;
    cs_uint64 out_mc_pkts;
    cs_uint64 out_bc_pkts;
    cs_uint64 in_pkts;
    cs_uint64 in_octets;
    cs_uint64 in_mc_pkts;
    cs_uint64 in_bc_pkts;
} __attribute__((packed)) oam_ctc_onu_llid_stats1;

typedef struct {
    oam_var_cont_t hdr;
    cs_uint64 in_crc8_err_pkts;
    cs_uint64 in_fec_crct_blks;
    cs_uint64 in_fec_uncrct_blks;
    cs_uint64 out_mpcp_pkts;
    cs_uint64 in_mpcp_pkts;
    cs_uint64 out_mpcp_reg_pkts;
    cs_uint64 out_mpcp_rreq_pkts;
    cs_uint64 out_mpcp_rpt_pkts;
    cs_uint64 in_mpcp_gate_pkts;
    cs_uint64 in_mpcp_reg_pkts;
} __attribute__((packed)) oam_ctc_onu_llid_stats2;

typedef struct {
    oam_var_cont_t hdr;
    cs_uint32  cfg_counter;
} __attribute__((packed)) oam_ctc_onu_cfg_counter_t;

typedef struct {
    oam_var_cont_t hdr;
    cs_uint8  action;
} __attribute__((packed)) oam_ctc_onu_cfg_mgmt_t;
/**************************************************************/
/* CTC ONU chip capabilities_2 variable container */

typedef struct {
        cs_uint32 interfaceType;
        cs_uint16 numPorts;
} __attribute__((packed)) oam_ctc_onu_interface_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint32   onuType;
        cs_uint8    multillid;
        cs_uint8    protectionType;
        cs_uint8    numPONIf;
        cs_uint8    numSlot;
        cs_uint8    numInterface;
        oam_ctc_onu_interface_t intf[1];
        cs_uint8    BatteryBackup;
} __attribute__((packed)) oam_ctc_onu_capabilities2_t;

#define CTC_IPV6_CAPABILTY_NOT_SUPPORT  0x00 
#define CTC_IPV6_CAPABILTY_SUPPORT      0x01


#define CTC_POWER_CONTROL_NOT_SUPPORT   0x00
#define CTC_POWER_CONTROL_SUPPORT_TX    0x01
#define CTC_POWER_CONTROL_SUPPORT_TX_RX 0x02

#define CTC_SERVICE_SLA_NOT_SUPPORT     0x01

typedef struct {
	oam_var_cont_t hdr;
	cs_uint8 ipv6Support;
	cs_uint8 powerControl;
    cs_uint8 serviceSla;
} __attribute__((packed)) oam_ctc_onu_capabilities3_t;

#define  CTC_OAM_SLEEP_MODE_NOT_SUPP      0 
#define  CTC_OAM_SLEEP_MODE_TX_ONLY       1 
#define  CTC_OAM_SLEEP_MODE_RX_ONLY       2 
#define  CTC_OAM_SLEEP_MODE_TX_RX         3


#define CTC_OAM_EARLY_WAKEUP_SUPP       0
#define CTC_OAM_EARLY_WAKEUP_NOT_SUPP   1

typedef struct {
	oam_var_cont_t hdr;
    cs_uint8 sleep_mode_cap;
    cs_uint8 early_wakeup_cap;
} __attribute__((packed)) oam_ctc_onu_power_saving_cap_t;

#define CTC_OAM_EARLY_WAKEUP_ENABLE   0 
#define CTC_OAM_EARLY_WAKEUP_DISABLE  1 

#define CTC_OAM_DURATION_MAX_LEN    6
typedef struct {
    oam_var_cont_t hdr;
    cs_uint8  early_wakeup;
    cs_uint8  max_sleep_duration[CTC_OAM_DURATION_MAX_LEN];
} __attribute__((packed)) oam_ctc_onu_power_saving_config_t;

typedef struct {
	oam_var_cont_t hdr;
    cs_uint16  los_optical_time;
    cs_uint16  los_mpcp_time;
} __attribute__((packed)) oam_ctc_onu_pon_protect_parm_t;



typedef struct {
        oam_var_cont_t hdr;
        cs_uint32   onuType;
        cs_uint8    multillid;
        cs_uint8    protectionType;
        cs_uint8    numPONIf;
        cs_uint8    numSlot;
        cs_uint8    num_if_type;
        oam_ctc_onu_interface_t intf[MAX_MDU_PORTS_TYPES];
        cs_uint8    BatteryBackup;
} __attribute__((packed)) oam_ctc_onu_capabilities2_config_t;

#define OAM_CTC_ONU_HOLDOVER_DEACTIVED      0x00000001
#define OAM_CTC_ONU_HOLDOVER_ACTIVED        0x00000002

#define OAM_CTC_ONU_HOLDOVER_TIME_MIN 50
#define OAM_CTC_ONU_HOLDOVER_TIME_MAX 1000

typedef struct {
        oam_var_cont_t hdr;
        cs_uint32 holdover_state;
        cs_uint32 holdover_time;
} __attribute__((packed)) oam_ctc_onu_holdover_t;

/* CTC ONU OPM diagnosis variable container */
typedef struct {
        oam_var_cont_t hdr;
        cs_int16  temp;
        cs_uint16 volt;
        cs_uint16 tx_bias;
        cs_uint16 tx_power;
        cs_uint16 rx_power;
} __attribute__((packed)) oam_ctc_onu_opm_diagnosis_t;


/* CTC ONU eth port link state variable container */
#define OAM_CTC_ONU_ETH_DOWN                0x0
#define OAM_CTC_ONU_ETH_UP                  0x1

#define OAM_INTF_PHY_ADMIN_DIS                  1
#define OAM_INTF_PHY_ADMIN_EN                   2

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 linkState;
} __attribute__((packed)) oam_ctc_onu_link_state_t;

/* CTC ONU eth port pause variable container */
#define CTC_ONU_PORT_PAUSE_DISABLE          0
#define CTC_ONU_PORT_PAUSE_ENABLE           1


typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 portOper;
} __attribute__((packed)) oam_ctc_onu_eth_pause_t;



/* CTC ONU eth port policing variable container */
#define CTC_PORT_CIR_LEN                    3
#define CTC_PORT_CBS_LEN                    3
#define CTC_PORT_EBS_LEN                    3

#define CTC_PORT_POLICY_OPER_DISABLE        0
#define CTC_PORT_POLICY_OPER_ENBALE         1

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 portOper;
        cs_uint8 portCIR[CTC_PORT_CIR_LEN];
        cs_uint8 portCBS[CTC_PORT_CBS_LEN];
        cs_uint8 portEBS[CTC_PORT_EBS_LEN];
} __attribute__((packed)) oam_ctc_onu_port_policing_t;


#define CTC_PORT_DS_RATE_LIMIT_DISABLE        0
#define CTC_PORT_DS_RATE_LIMIT_ENABLE         1

#define CTC_PORT_PIR_LEN                      3

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 portOper;
        cs_uint8 portCIR[CTC_PORT_CIR_LEN];
        cs_uint8 portPIR[CTC_PORT_PIR_LEN];
} __attribute__((packed)) oam_ctc_onu_ds_rate_limit_t;


/* CTC ONU VoIP Port variable container */
#define CTC_VOIP_PORT_MIN                   0x50
#define CTC_VOIP_PORT_MAX                   0x8f
#define CTC_VOIP_PORT_LOCK                  0
#define CTC_VOIP_PORT_UNLOCK                1
#define CTC_MAX_ONU_VOIP_PORTS              1

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 portOper;
} __attribute__((packed)) oam_ctc_onu_port_voip_t;


/* CTC ONU E1 Port variable container */
#define CTC_E1_PORT_MIN                     0x90
#define CTC_E1_PORT_MAX                     0x9f
#define CTC_E1_PORT_LOCK                    0
#define CTC_E1_PORT_UNLOCK                  1


typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 portOper;
} __attribute__((packed)) oam_ctc_onu_port_E1_t;

/* CTC ONU vlan variable container */
#define CTC_VLAN_MODE_TRANSPARENT           0x0
#define CTC_VLAN_MODE_TAG                   0x1
#define CTC_VLAN_MODE_TRANSLATE             0x2
#define CTC_VLAN_MODE_AGG                   0x3
#define CTC_VLAN_MODE_TRUNK                 0x4


#define CTC_VLAN_MODE_TRANSLATE_MAX         16
#define CTC_VLAN_MODE_TRUNK_MAX             16
#define CTC_ONU_AGGR_VLAN_MAX               16

#define CTC_ONU_AGGR_VLAN_NUM               32

typedef struct {
        cs_uint16 tpid;
        cs_uint16 tag;
} __attribute__((packed)) oam_ctc_onu_vlan_tag_t;

typedef struct {
        oam_ctc_onu_vlan_tag_t tag1;
        oam_ctc_onu_vlan_tag_t tag2;
} __attribute__((packed)) oam_ctc_onu_vlan_xlate_entry;

typedef struct {
        oam_ctc_onu_vlan_tag_t  defaultVlan;
        oam_ctc_onu_vlan_xlate_entry entries[CTC_VLAN_MODE_TRANSLATE_MAX];
} __attribute__((packed)) oam_ctc_onu_vlan_xlate_t;

typedef struct oam_ctc_onu_port_vlan_agg_table
{
    cs_uint16 num_of_agg_vlan;
    oam_ctc_onu_vlan_tag_t    agg_dst_vlan;
    oam_ctc_onu_vlan_tag_t    agg_vlan[1];
}__attribute__((packed)) oam_ctc_onu_port_vlan_agg_table_t;

typedef struct {
        cs_uint16   num;
        oam_ctc_onu_port_vlan_agg_table_t item[1];
} __attribute__((packed)) oam_ctc_onu_vlan_aggre_table_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 vlanMode;
        cs_uint8 data[1];
} __attribute__((packed)) oam_ctc_onu_vlan_t;


/* CTC ONU classification variable container */
#define CTC_CLASS_FIELD_SELECT_MAX              4
#define CTC_CLASS_RULES_MAX                     8
#define CTC_CLASS_MATCH_VAL_LEN                 6

#define CTC_CLASS_RULES_MORE(a) if(a & 0x80) return EPON_TRUE; return EPON_FALSE
#define CTC_CLASS_RULES_COUNT(a)    return (a & 0xf)

#define CTC_CLASS_RULES_ACTION_DEL              0x00
#define CTC_CLASS_RULES_ACTION_ADD              0x01
#define CTC_CLASS_RULES_ACTION_CLR              0x02
#define CTC_CLASS_RULES_ACTION_LST              0x03

#define CTC_CLASS_RULES_FSELECT_BASE            0x0
#define CTC_CLASS_RULES_FSELECT_DA_MAC          CTC_CLASS_RULES_FSELECT_BASE
#define CTC_CLASS_RULES_FSELECT_SA_MAC          CTC_CLASS_RULES_FSELECT_BASE + 0x1
#define CTC_CLASS_RULES_FSELECT_VLAN_PRI        CTC_CLASS_RULES_FSELECT_BASE + 0x2
#define CTC_CLASS_RULES_FSELECT_VLAN_ID         CTC_CLASS_RULES_FSELECT_BASE + 0x3
#define CTC_CLASS_RULES_FSELECT_ETH_TYPE        CTC_CLASS_RULES_FSELECT_BASE + 0x4
#define CTC_CLASS_RULES_FSELECT_DEST_IP         CTC_CLASS_RULES_FSELECT_BASE + 0x5
#define CTC_CLASS_RULES_FSELECT_SRC_IP          CTC_CLASS_RULES_FSELECT_BASE + 0x6
#define CTC_CLASS_RULES_FSELECT_IP              CTC_CLASS_RULES_FSELECT_BASE + 0x7
#define CTC_CLASS_RULES_FSELECT_IPV4_DSCP       CTC_CLASS_RULES_FSELECT_BASE + 0x8
#define CTC_CLASS_RULES_FSELECT_IPV6_DSCP       CTC_CLASS_RULES_FSELECT_BASE + 0x9
#define CTC_CLASS_RULES_FSELECT_L4_SRC_PORT     CTC_CLASS_RULES_FSELECT_BASE + 0xA
#define CTC_CLASS_RULES_FSELECT_L4_DST_PORT     CTC_CLASS_RULES_FSELECT_BASE + 0xB
#define CTC_CLASS_RULES_FSELECT_IPV             CTC_CLASS_RULES_FSELECT_BASE + 0xC
#define CTC_CLASS_RULES_FSELECT_IPV6_FLOW_LABEL CTC_CLASS_RULES_FSELECT_BASE + 0xD
#define CTC_CLASS_RULES_FSELECT_DST_IPV6        CTC_CLASS_RULES_FSELECT_BASE + 0xE
#define CTC_CLASS_RULES_FSELECT_SRC_IPV6        CTC_CLASS_RULES_FSELECT_BASE + 0xF
#define CTC_CLASS_RULES_FSELECT_DST_IPV6_PREFIX CTC_CLASS_RULES_FSELECT_BASE + 0x10
#define CTC_CLASS_RULES_FSELECT_SRC_IPV6_PREFIX CTC_CLASS_RULES_FSELECT_BASE + 0x11
#define CTC_CLASS_RULES_FSELECT_NEXT_HEADER     CTC_CLASS_RULES_FSELECT_BASE + 0x12


#define CTC_CLASS_VALIDATION_OPER_NOT           0
#define CTC_CLASS_VALIDATION_OPER_EQL           1
#define CTC_CLASS_VALIDATION_OPER_NOT_EQL       2
#define CTC_CLASS_VALIDATION_OPER_LESS_EQL      3
#define CTC_CLASS_VALIDATION_OPER_GRT_EQL       4
#define CTC_CLASS_VALIDATION_OPER_EXIST         5
#define CTC_CLASS_VALIDATION_OPER_NOT_EXIST     6
#define CTC_CLASS_VALIDATION_OPER_ALWAYS        7

#define CTC_CLASS_PREC_MIN                      1

#define CTC_CLASS_ACTION_MIN                    0

#define CTC_CLASS_ACTION_MAX                    3

#define CTC_CLASS_PRIMARKING_MIN                0
#define CTC_CLASS_PRIMARKING_MAX                7

#define CTC_CLASS_FSELECT_MIN                   0
#define CTC_CLASS_FSELECT_MAX                   CTC_CLASS_RULES_FSELECT_L4_DST_PORT

typedef struct {
        cs_uint8 fieldSelect;
        cs_uint8 matchValue[CTC_CLASS_MATCH_VAL_LEN];
        cs_uint8 validationOper;
} __attribute__((packed)) oam_ctc_onu_class_fselect_t;

typedef struct {
        cs_uint8 fieldSelect;
        cs_uint8 matchValue[OAM_IPV6_ADDR_LEN];
        cs_uint8 validationOper;
} __attribute__((packed)) oam_ctc_onu_class_fselect_v6_t;

typedef struct {
        cs_uint8 precedence;
        cs_uint8 len;
        cs_uint8 queueMapped;
        cs_uint8 priMark;
        cs_uint8 entries;
        cs_uint8 data[1];
} __attribute__((packed)) oam_ctc_onu_classification_struct;


typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 action;
        cs_uint8 numRules;
        cs_uint8 data[1];
} __attribute__((packed)) oam_ctc_onu_classification_t;

/* CTC ONU multicast vlan variable container */
#define CTC_MC_VLAN_MAX                         64

#define CTC_MC_VLAN_OPR_BASE                    0
#define CTC_MC_VLAN_OPR_DEL                     CTC_MC_VLAN_OPR_BASE
#define CTC_MC_VLAN_OPR_ADD                     CTC_MC_VLAN_OPR_BASE + 1
#define CTC_MC_VLAN_OPR_CLR                     CTC_MC_VLAN_OPR_BASE + 2
#define CTC_MC_VLAN_OPR_LST                     CTC_MC_VLAN_OPR_BASE + 3

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8  vlanOper;
        cs_uint16 vlanIds[CTC_MC_VLAN_MAX];
} __attribute__((packed)) oam_ctc_onu_mc_vlan_t;

/* CTC ONU multicast Tag Strip variable container */
#define CTC_MC_TAGSTRIP_NOT_STRIP               0x0
#define CTC_MC_TAGSTRIP_STRIP                   0x1
#define CTC_MC_TAGSTRIP_IPTV                    0x2

#define CTC_ONU_TAGSTRIP_IPTV_VLAN_NUM          4
#define CTC_ONU_MAX_IPTV_VLAN                   16 

typedef struct {
        cs_uint16 vid ;
        cs_uint16 iptv_vid ;
} __attribute__((packed)) oam_ctc_iptv_vid_t ;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint8    opr;
        cs_uint8    count ; 
        cs_uint8    data[1] ;
} __attribute__((packed)) oam_ctc_onu_mc_tagstrip_t;



/* CTC ONU multicast Tag Strip variable container */
#define CTC_MC_SWITCH_START_IMGP_SNOOPING       0x0
#define CTC_MC_SWITCH_START_CTC_IGMP                0x1
#define CTC_MC_SWITCH_TRANSPARENT_ZTE_EXT       0xa0
#define UT_MC_SWITCH_IVL_IGMP_SNOOPING_MODE     0xa0
#define SNOOP_STRICT_VLAN_MODE                  0xb0


typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 opr;
} __attribute__((packed)) oam_ctc_onu_mc_switch_t;

#define CTC_MC_CONTROL_MAX_ENTRY                128

#define CTC_MC_TYPE_CONTROL_DA_MAC              0
#define CTC_MC_TYPE_CONTROL_DA_VLAN             1
#define CTC_MC_TYPE_CONTROL_MAC_SA_IP           2
#define CTC_MC_TYPE_CONTROL_DA_IPV4_VLAN        3
#define CTC_MC_TYPE_CONTROL_DA_IPV6_VLAN        4

#define CTC_MC_CONTROL_ACTION_DEL               0
#define CTC_MC_CONTROL_ACTION_ADD               1
#define CTC_MC_CONTROL_ACTION_CLR               2
#define CTC_MC_CONTROL_ACTION_LST               3

typedef struct {
        cs_uint16 userId;
        cs_uint16 vlanId;
        cs_uint8 mc_da[CS_MACADDR_LEN];
} __attribute__((packed)) oam_ctc_onu_mc_control_vlan_mac_t;


typedef struct {
        cs_uint16 userId;
        cs_uint8 mc_da[CS_MACADDR_LEN];
        cs_uint8 ip_addr[OAM_IPV6_ADDR_LEN];
} __attribute__((packed)) oam_ctc_onu_mc_control_mac_ip_t;


typedef struct {
        cs_uint16 userId;
        cs_uint16 vlanId;
        cs_uint8 ip_addr[OAM_IPV6_ADDR_LEN];
} __attribute__((packed)) oam_ctc_onu_mc_control_vlan_ip_t;


typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 action;
        cs_uint8 mcType;
        cs_uint8 entries;
        cs_uint8 data[1];
} __attribute__((packed)) oam_ctc_onu_mc_control_t;


/* CTC ONU group number max variable container */
#define CTC_ONU_MAX_MC_GRP                      32
typedef struct {
        oam_var_cont_t hdr;
        cs_uint8 maxGrp;
} __attribute__((packed)) oam_ctc_onu_mc_max_grp_t;
#define CTC_ONU_FAST_LEAVE_MODE_MAX   6
typedef struct {
        oam_var_cont_t hdr;
        cs_uint32 mode_num;
        cs_uint32 mode[CTC_ONU_FAST_LEAVE_MODE_MAX];
} __attribute__((packed)) oam_ctc_onu_fast_leave_ability_t;

#define CTC_ONU_FAST_LEAVE_DEACTIVATE                    1
#define CTC_ONU_FAST_LEAVE_ACTIVATE                      2
typedef struct {
        oam_var_cont_t hdr;
        cs_uint32 enable;
} __attribute__((packed)) oam_ctc_onu_fast_leave_t;

#define CTC_ONU_AUTO_NEG_TECH_MAX   12

typedef struct {
        oam_var_cont_t          hdr;
        cs_uint32               value1;
        cs_uint32               value2[CTC_ONU_AUTO_NEG_TECH_MAX];
} __attribute__((packed)) oam_ctc_onu_auto_neg_tech_t;

typedef struct {
        oam_var_cont_t hdr;
        cs_uint32 num_llid;
} __attribute__((packed)) oam_ctc_onu_ac_multi_llid_admin_control_t;

#define MDU_MAX_QUEUES_PER_LLID                  8

typedef struct {
    cs_uint16 qid;
    cs_uint16 wrr; /*0-sp, 1-100 wrr*/
} __attribute__((packed)) oam_ctc_onu_llid_config_t;


typedef struct {
    oam_var_cont_t hdr;
    cs_uint8 queue_num;
    oam_ctc_onu_llid_config_t config[MDU_MAX_QUEUES_PER_LLID]; 
} __attribute__((packed)) oam_ctc_onu_llid_queue_config_t;

typedef struct{
    oam_var_cont_t hdr;
    cs_uint32  sleep_duration;
    cs_uint32  wait_duration;
    cs_uint8   sleep_flag;
    cs_uint8   sleep_mode;
} __attribute__((packed)) oam_ctc_onu_sleep_ctrl_t;

typedef struct{
    oam_var_cont_t hdr;
    cs_uint16 monitor_status;
    cs_uint32 monitor_period;
} __attribute__((packed)) oam_ctc_onu_stats_monitor_t;

typedef struct{
    cs_uint64 dn_drop_events;
    cs_uint64 up_drop_events;
    cs_uint64 dn_octets;
    cs_uint64 up_octets;
    cs_uint64 dn_frames;
    cs_uint64 up_frames;
    cs_uint64 dn_bc_frames;
    cs_uint64 up_bc_frames;
    cs_uint64 dn_mc_frames;
    cs_uint64 up_mc_frames;
    cs_uint64 dn_crc_err_frames;
    cs_uint64 up_crc_err_frames;
    cs_uint64 dn_undersize_frames;
    cs_uint64 up_undersize_frames;
    cs_uint64 dn_oversize_frames;
    cs_uint64 up_oversize_frames;
} __attribute__((packed)) oam_ctc_onu_port_stats_frag1_t;

typedef struct{
    cs_uint64 dn_fragments;
    cs_uint64 up_fragments;
    cs_uint64 dn_jabbers;
    cs_uint64 up_jabbers;

    cs_uint64 dn_64_octets_frames;
    cs_uint64 dn_65_127_octets_frames;
    cs_uint64 dn_128_255_octets_frames;
    cs_uint64 dn_256_511_octets_frames;
    cs_uint64 dn_512_1023_octets_frames;
    cs_uint64 dn_1024_1518_octets_frames;
    cs_uint64 up_64_octets_frames;
    cs_uint64 up_65_127_octets_frames;
    cs_uint64 up_128_255_octets_frames;
    cs_uint64 up_256_511_octets_frames;
    cs_uint64 up_512_1023_octets_frames;
    cs_uint64 up_1024_1518_octets_frames;
} __attribute__((packed)) oam_ctc_onu_port_stats_frag2_t;


typedef struct{
    cs_uint64 dn_discard_frames;
    cs_uint64 up_discard_frames;
    cs_uint64 dn_err_frames;
    cs_uint64 up_err_frames;
    cs_uint64 status_change_times;
} __attribute__((packed)) oam_ctc_onu_port_stats_frag3_t;


typedef struct{
    cs_uint64 dn_drop_events;
    cs_uint64 up_drop_events;
    cs_uint64 dn_octets;
    cs_uint64 up_octets;
    cs_uint64 dn_frames;
    cs_uint64 up_frames;
    cs_uint64 dn_bc_frames;
    cs_uint64 up_bc_frames;
    cs_uint64 dn_mc_frames;
    cs_uint64 up_mc_frames;
    cs_uint64 dn_crc_err_frames;
    cs_uint64 up_crc_err_frames;
    cs_uint64 dn_undersize_frames;
    cs_uint64 up_undersize_frames;
    cs_uint64 dn_oversize_frames;
    cs_uint64 up_oversize_frames;
    cs_uint64 dn_fragments;
    cs_uint64 up_fragments;
    cs_uint64 dn_jabbers;
    cs_uint64 up_jabbers;
    /*cs_uint64 dn_collisions;*/
    cs_uint64 dn_64_octets_frames;
    cs_uint64 dn_65_127_octets_frames;
    cs_uint64 dn_128_255_octets_frames;
    cs_uint64 dn_256_511_octets_frames;
    cs_uint64 dn_512_1023_octets_frames;
    cs_uint64 dn_1024_1518_octets_frames;
    cs_uint64 up_64_octets_frames;
    cs_uint64 up_65_127_octets_frames;
    cs_uint64 up_128_255_octets_frames;
    cs_uint64 up_256_511_octets_frames;
    cs_uint64 up_512_1023_octets_frames;
    cs_uint64 up_1024_1518_octets_frames;
    cs_uint64 dn_discard_frames;
    cs_uint64 up_discard_frames;
    cs_uint64 dn_err_frames;
    cs_uint64 up_err_frames;
    cs_uint64 status_change_times;
} __attribute__((packed)) oam_ctc_onu_port_stats_t;

typedef struct{
    oam_var_cont_t hdr;
    oam_ctc_onu_port_stats_frag1_t  frag1;
    oam_var_cont_t                           hdr2;    
    oam_ctc_onu_port_stats_frag2_t  frag2;
    oam_var_cont_t                           hdr3;  
    oam_ctc_onu_port_stats_frag3_t  frag3;
    
} __attribute__((packed)) oam_ctc_onu_stats_data_t;

#define OAM_PDU_CTC_DBA_CODE_GET_REQ            0x0
#define OAM_PDU_CTC_DBA_CODE_GET_RSP            0x1
#define OAM_PDU_CTC_DBA_CODE_SET_REQ            0x2
#define OAM_PDU_CTC_DBA_CODE_SET_RSP            0x3
#define CTC_ONU_MAX_QUEUE                       8
#define CTC_ONU_MAX_QUEUE_SET                   4

typedef struct {
        cs_uint8  reportBitmap;
        cs_uint16 queueThreshold[CTC_ONU_MAX_QUEUE];
} __attribute__((packed)) oam_ctc_onu_dba_queue_set_t;

typedef struct {
        cs_uint8 numQueueSet;
        oam_ctc_onu_dba_queue_set_t queueSet[CTC_ONU_MAX_QUEUE_SET];
        cs_uint8 setack;
} __attribute__((packed)) oam_ctc_onu_dba_struct;

typedef struct {
        cs_uint8 dbaCode;
        cs_uint8 numQueueSet;
        cs_uint8 queueset_start;
} __attribute__((packed)) oam_ctc_onu_dba_t;

#define KEY_INDEX_MAX                           1
#define OAM_PDU_CTC_CODE_KEY_REQ                0x0
#define OAM_PDU_CTC_CODE_NEW_KEY                0x1

typedef struct {
        cs_uint8 churningCode;
        cs_uint8 inUserKeyIdx;
} __attribute__((packed)) oam_ctc_onu_new_key_req_t;


#define CHURNING_KEY_LEN                        3

typedef struct {
        cs_uint8 churningCode;
        cs_uint8 inUserKeyIdx;
        cs_uint8 churningKey[CHURNING_KEY_LEN];
} __attribute__((packed)) oam_ctc_onu_new_key_t;


typedef struct {
        cs_uint8 ctc_oui[3];
        cs_uint8 spare[5];
} __attribute__((packed)) ctc_vspecific_t;

#define TFTP_DATA       1
#define CRC_CHECK       2
#define IMAGE_DATA      3
#define IMAGE_COMMIT    4

#define CTC_OAM_ONU_UPGRADE_FILE_WRITE_REQ          2
#define CTC_OAM_ONU_UPGRADE_FILE_TRANSFER_DATA      3
#define CTC_OAM_ONU_UPGRADE_FILE_TRANSFER_ACK       4
#define CTC_OAM_ONU_UPGRADE_ERROR                   5
#define CTC_OAM_ONU_UPGRADE_END_DOWNLOAD_REQ        6
#define CTC_OAM_ONU_UPGRADE_END_DOWNLOAD_RSP        7
#define CTC_OAM_ONU_UPGRADE_ACTIVATE_IMG_REQ        8
#define CTC_OAM_ONU_UPGRADE_ACTIVATE_IMG_RSP        9
#define CTC_OAM_ONU_UPGRADE_COMMIT_IMG_REQ          10
#define CTC_OAM_ONU_UPGRADE_COMMIT_IMG_RSP          11

typedef struct {
        cs_uint8    type;
        cs_uint16   length;
        cs_uint16   tid;
        cs_uint8    msg[1];
} __attribute__((packed)) ctc_onu_upgrade_header;

typedef struct {
        cs_uint16 opcode;
        cs_uint16 block;
        cs_uint8 data[1];
} __attribute__((packed)) ctc_onu_upgrade_transfer_container;

#define TFTP_ERROR_NOT_DEFINED              0
#define TFTP_ERROR_FILE_NOT_FOUND           1
#define TFTP_ERROR_ACCESS_VIOLATION         2
#define TFTP_ERROR_DISKFULL_ALLOC_EXCEED    3
#define TFTP_ERROR_ILLEGAL_OPERTION         4
#define TFTP_ERROR_UNKNOWN_TRANS_ID         5
#define TFTP_ERROR_FILE_EXIST               6
#define TFTP_ERROR_NO_SUCH_USER             7

typedef struct {
        cs_uint16 opcode;
        cs_uint16 error;
        cs_uint8 errmsg[1];
} __attribute__((packed)) ctc_onu_upgrade_error_container;

typedef struct {
        cs_uint16 opcode;
        cs_uint32 size;
} __attribute__((packed)) ctc_onu_upgrade_end_req_container;

#define END_RSP_ACK_OK          0
#define END_RSP_ACK_WRITE       1
#define END_RSP_ACK_CRC_ERROR   2
#define END_RSP_ACK_PARA_ERROR  3
#define END_RSP_ACK_NOT_SUPP    4

#define ACTIVE_RSP_ACK_OK          0
#define ACTIVE_RSP_ACK_PARA_ERROR  1
#define ACTIVE_RSP_ACK_NOT_SUPP    2
#define ACTIVE_RSP_ACK_ERROR       3

typedef struct {
        cs_uint16 opcode;
        cs_uint8 data;
} __attribute__((packed)) ctc_onu_upgrade_common_container;

#define MAX_ONU_IMAGE_FRAME_SIZE      1400

#define CTC_ONU_UPGRADE_NULL            0
#define CTC_ONU_UPGRADE_DATA            1
#define CTC_ONU_UPGRADE_END             2
#define CTC_ONU_UPGRADE_ACTIVE          3
#define CTC_ONU_UPGRADE_WRITING         4
#define CTC_ONU_UPGRADE_WRITE_FINISHED  5

#define CTC_ONU_UPGRADE_WRITE_ACK       0
#define CTC_ONU_UPGRADE_DATA_ACK        1
#define CTC_ONU_UPGRADE_END_RSP         2
#define CTC_ONU_UPGRADE_ACTIVE_RSP      3
#define CTC_ONU_UPGRADE_COMMIT_REQ      4
#define CTC_ONU_UPGRADE_COMMIT_RSP      5



/* CTC 2.1 Auth code */
#define OAM_CTC_AUTH_REQUEST            0x01
#define OAM_CTC_AUTH_RESPONSE           0x02
#define OAM_CTC_AUTH_SUCCESS            0x03
#define OAM_CTC_AUTH_FAILED             0x04

/* CTC 2.1 Auth type */
#define OAM_CTC_AUTH_TYPE_LOID_PASSWD   0x01
#define OAM_CTC_AUTH_TYPE_UNSUPPORT     0x02

/*LOID Auth faided type */
#define OAM_CTC_LOID_AUTH_LOID_NOT_EXIST    0x01
#define OAM_CTC_LOID_AUTH_PASSWD_ERROR      0x02
#define OAM_CTC_LOID_AUTH_LOID_CONFLICT     0x03


/* CTC 2.1 Auth pdu*/
typedef struct {
        oam_pdu_hdr_t      hdr;
        cs_uint8           oui[OAM_OUI_LEN];
        cs_uint8           ext_opcode;
        cs_uint8           auth_code;
        cs_uint16          len;
        cs_uint8           auth_data[1];
} __attribute__((packed)) oam_ctc_pdu_auth_t;

/* CTC 2.1 Auth Data*/
typedef struct {
    cs_uint8 auth_type;
    cs_uint8 auth_data[36];
} __attribute__((packed)) oam_ctc_auth_data_t;

/*******************************************/
/*CTC 3.0 Event*/
#define OAM_CTC_EVENT_STATUS_GET           0x0001
#define OAM_CTC_EVENT_STATUS_SET           0x0002
#define OAM_CTC_EVENT_STATUS_RESP          0x0003
#define OAM_CTC_EVENT_THRESH_GET           0x0004
#define OAM_CTC_EVENT_THRESH_SET           0x0005
#define OAM_CTC_EVENT_THRESH_RESP          0x0006

#define OAM_CTC_EVENT_STATUS_DISABLE  0x00000000
#define OAM_CTC_EVENT_STATUS_ENABLE   0x00000001
#define OAM_CTC_EVENT_STATUS_NOT_FOUND   0xffffffff

#define OAM_CTC_EVENT_THRESH_NOT_FOUND   0xffffffff

typedef struct{
    cs_uint8 subtype;
    cs_uint16 entry_count;
    cs_uint8 event_data[1];
} __attribute__((packed)) oam_ctc_event_hdr_t;

typedef struct{
    cs_uint16 object_type;
    cs_uint32 object_inst;
    cs_uint16 id;
} __attribute__((packed)) oam_ctc_event_entry_t;

typedef struct{
    oam_ctc_event_entry_t entry;
    cs_uint32 status;
} __attribute__((packed)) oam_ctc_event_status_t;

typedef struct{
    oam_ctc_event_entry_t entry;
    cs_uint32 raise_thresh;
    cs_uint32 clear_thresh;
} __attribute__((packed)) oam_ctc_event_thresh_t;


#endif
#endif
