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
#ifndef _APP_OAM_CTC_H_
#define _APP_OAM_CTC_H_

#ifdef HAVE_CTC_OAM
#include "oam_ctc_pdu.h"
#include "oam_supp.h"


#define MAX_LASER_CONTROL_ACTION_NUM  65535
#define MAX_LASER_CONTROL_TRANSCEIVER_ID  2

#define OAM_CTC_AUTO_NEG_CTRL_DEACTIVATE  1
#define OAM_CTC_AUTO_NEG_CTRL_ACTIVATE       2

#define OAM_CTC_LOOP_DETECT_DEACTIVATE  1
#define OAM_CTC_LOOP_DETECT_ACTIVATE       2

#define MAX_CTC_ONU_IMAGE_FRAME_BUFF_SIZE      412000


/*ctc oam code clean*/
#define  CTC_OAM_BUILD_RESP \
    xx(01, OAM_INTF_COUNTERS_MIB_BRANCH,OAM_CTC_LEAF_CODE_ONU_ETH_ADM_STATE, OAM_INTF_LEAF_ETH_ADM_STATE/*0x25*/,\
          ctc_oam_eth_admin_state_get, NULL) \
    xx(02, OAM_INTF_COUNTERS_MIB_BRANCH,OAM_CTC_LEAF_CODE_ONU_ETH_AUTONEG_ADM_STATE, OAM_INTF_LEAF_ETH_AUTONEG_ADM_STATE/*0x4f*/, \
          ctc_oam_eth_auto_admin_get, NULL) \
    xx(03, OAM_INTF_COUNTERS_MIB_BRANCH,OAM_CTC_LEAF_CODE_ONU_ETH_AUTONEG_LOCAL_CAP, OAM_INTF_LEAF_ETH_AUTONEG_LOCAL_CAP/*0x52*/,\
          ctc_oam_eth_auto_local_cap_get, NULL) \
    xx(04, OAM_INTF_COUNTERS_MIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_ETH_AUTONEG_ADVTS_CAP,OAM_INTF_LEAF_ETH_AUTONEG_ADVTS_CAP/*0x53*/,\
          ctc_oam_eth_auto_advts_cap_get, NULL) \
    xx(05, OAM_INTF_COUNTERS_MIB_BRANCH,OAM_CTC_LEAF_CODE_ONU_ETH_FEC_ABITITY, OAM_INTF_LEAF_ETH_FEC_ABILITY/*0x139*/,\
          ctc_oam_eth_fec_ability_get, NULL) \
    xx(06, OAM_INTF_COUNTERS_MIB_BRANCH,OAM_CTC_LEAF_CODE_ONU_ETH_FEC_MODE, OAM_INTF_LEAF_ETH_FEC_MODE/*0x13a*/,\
          ctc_oam_eth_fec_mode_get, ctc_oam_eth_fec_mode_set) \
    \
    \
    xx(07, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_SN, 0x0001,\
          ctc_oam_sn_get, NULL) \
    xx(08, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_FRM_VER, 0x0002,\
          ctc_oam_fw_get, NULL) \
    xx(09, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_CHIP_ID, 0x0003,\
            ctc_oam_chipid_get, NULL) \
    xx(10, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_CAP, 0x0004,\
            ctc_oam_onucap_get, NULL) \
    xx(11, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_OPM_DIAG, 0x0005,\
            ctc_oam_onu_opmdiag_get, NULL) \
    xx(12, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_SLA, 0x0006,\
            ctc_oam_sla_get, ctc_oam_onu_sla_set) \
    xx(13, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_CAP_2, 0x0007, \
            ctc_oam_onucap2_get, NULL)\
    xx(14, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_HOLDOVER, 0x0008,\
            ctc_oam_holdover_get, ctc_oam_holdover_set) \
    xx(15, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_MGMT_CONFIG, 0x0009,\
            ctc_oam_onu_mgmt_config_get, ctc_oam_onu_mgmt_config_set) \
    /*xx(16, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_MDU_SNMP, 0x000a,\
            ctc_oam_onu_mdu_snmp_get, ctc_oam_onu_mdu_snmp_set) */ \
    xx(17, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_ACTIVE_PON, 0x000b, \
            ctc_oam_onu_active_pon_get, ctc_oam_onu_active_pon_set) \
    xx(18, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_CHIP_CAP3, 0x000c, \
            ctc_oam_onucap3_get, NULL) \
    xx(19, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_POWER_SAVE_CAP, 0x000D, \
            ctc_oam_onu_power_saving_cap_get, NULL) /* CTC V3.0 */ \
    xx(20, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_POWER_SAVE_CONFIG, 0x000E, \
            ctc_oam_onu_0x000e_config_get, ctc_oam_onu_0x000e_config_set) /*CTC V3.0*/\
    xx(21, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_PON_PROTECT_PARAM, 0x000F, \
            ctc_oam_onu_0x000f_config_get, ctc_oam_onu_0x000f_config_set)/* CTC V3.0 */\
    xx(22, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_LINK_STATE, 0x0011, \
            ctc_oam_eth_port_link_get, NULL) \
    xx(23, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_ETH_PAUSE, 0x0012, \
            ctc_oam_eth_port_pause_get, ctc_oam_eth_port_pause_set) \
    xx(24, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_ETH_POLIC, 0x0013, \
            ctc_oam_eth_port_policing_get, ctc_oam_eth_port_policing_set) \
    xx(025, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_DS_RATE_LIMIT, 0x0016,\
            ctc_oam_eth_ds_rate_limit_get, ctc_oam_eth_ds_rate_limit_set) \
    xx(026, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_LOOPDETECT, 0x17, \
            ctc_oam_loop_detect_status_get, ctc_oam_loop_detect_status_set)  \
    xx(027, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_DISABLE_LOOPED, 0x18, \
            NULL, ctc_oam_disable_looped_status_set)/*CTC V3.0*/ \
    xx(027, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_MAC_LIMIT, 0x1C, \
            ctc_oam_onu_mac_limit_get, ctc_oam_onu_mac_limit_set)/*CUC OAM*/ \
    xx(027, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_PORT_MODE, 0x1D, \
            ctc_oam_onu_port_mode_get, ctc_oam_onu_port_mode_set)/*CUC OAM*/ \
    xx(027, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_PORT_MODE_STATUS, 0x1E, \
            ctc_oam_onu_port_work_mode_get, NULL)/*CUC OAM*/ \
    xx(027, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_PORT_STATS, 0x1F, \
            ctc_oam_onu_port_stats_get, NULL)/*CUC OAM*/ \
    xx(019, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_PON_PORT_STAT1, 0x5D, \
            ctc_oam_pon_port_stat_get, 0) /*CUC OAM*/ \
    xx(020, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_PON_PORT_STAT2, 0x5E,  \
            ctc_oam_pon_port_stat_get, 0) /*CUC OAM */ \
    xx(028, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_MAC_AGING_TIME, 0x00A4,\
            ctc_oam_eth_mac_aging_time_get, ctc_oam_eth_mac_aging_time_set) \
    xx(029, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_VLAN, 0x0021, \
            ctc_oam_onu_vlan_get, ctc_oam_onu_vlan_set) \
    xx(030, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_CLASSIF, 0x0031, \
            ctc_oam_onu_classification_get, ctc_oam_onu_classification_set) \
    xx(031, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_MC_VLAN, 0x0041,\
            ctc_oam_onu_mcvlan_get, ctc_oam_onu_mcvlan_set) \
    xx(032, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_MC_TAG_STRIP, 0x0042,\
            ctc_oam_onu_mctagstrip_get, ctc_oam_onu_mctagstrip_set) \
    xx(033, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_MC_SWITCH, 0x0043, \
            ctc_oam_onu_mc_switch_get, ctc_oam_onu_mc_switch_set) \
    xx(034, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_MC_CONTRL, 0x0044, \
            ctc_oam_onu_mc_control_get, ctc_oam_onu_mc_control_set) \
    xx(035, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_GRP_NUMMAX, 0x0045, \
            ctc_oam_onu_grp_nmb_max_get, ctc_oam_onu_grp_nmb_max_set) \
    xx(036, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_FAST_LEAVE_ABILITY, 0x0046, \
            ctc_oam_onu_fast_leave_ability_get, NULL) \
    xx(037, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_FAST_LEAVE_STATE, 0x0047, \
            ctc_oam_onu_fast_leave_state_get, NULL) \
    xx(038, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_LLID_QUEUE_CONFIG, 0x0051, \
            ctc_oam_onu_llid_queue_config_get, ctc_oam_onu_llid_queue_config_set) \
    xx(039, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_ALARM, 0x0081,\
            ctc_oam_onu_alarm_get, ctc_oam_onu_alarm_set) \
    xx(040, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_ALARM_THRESHOLD, 0x0082,\
            ctc_oam_onu_alarm_threshold_get, ctc_oam_onu_alarm_threshold_set) \
    xx(041, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_LASER_CONTROL, 0x00A1, \
            NULL, ctc_oam_onu_laser_control_set) \
    xx(041, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_MONITOR_STATUS, 0x00B1, \
            ctc_oam_onu_stats_monitor_status_get, ctc_oam_onu_stats_monitor_status_set) /*CTC V3.0*/ \
    xx(041, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_MONITOR_CURR_DATA, 0x00B2, \
            ctc_oam_onu_curr_stats_get, ctc_oam_onu_curr_stats_set) \
     xx(041, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_MONITOR_HISTORY_DATA, 0x00B3, \
            ctc_oam_onu_histroy_stats_get, NULL) \
    xx(041, OAM_CTC_ATTRIB_BRANCH, OAM_CTC_LEAF_CODE_ONU_LASER_CONTROL_EVENT, 0x8001, \
            NULL, ctc_oam_event_set) \
    \
    \
    xx(046, OAM_INTF_CONTROL_MIB_BRANCH, ONU_CTC_LEAF_CODE_ONU_ETH_ADM_STATE_SET, OAM_INTF_LEAF_ETH_ADM_STATE_SET/*0x05*/,\
            NULL, ctc_oam_eth_admin_state_set) \
    xx(047, OAM_INTF_CONTROL_MIB_BRANCH,ONU_CTC_LEAF_CODE_ONU_ETH_AUTO_NEG_RESTART, OAM_INTF_LEAF_ETH_AUTO_NEG_RESTART/*0x0b*/, \
            NULL, ctc_oam_auto_neg_restart_set) \
    xx(048, OAM_INTF_CONTROL_MIB_BRANCH,ONU_CTC_LEAF_CODE_ONU_ETH_AUTO_NEG_CONTRL, OAM_INTF_LEAF_ETH_AUTO_NEG_CONTRL/*0x0c*/, \
            NULL, ctc_oam_auto_neg_ctrl_set) \
    xx(049, OAM_CTC_ACTION_BRANCH, OAM_CTC_LEAF_CODE_ONU_RESET, 0x0001,\
            NULL, ctc_oam_onu_reset_set) \
    xx(049, OAM_CTC_ACTION_BRANCH, OAM_CTC_LEAF_CODE_ONU_SLEEP_CTRL, 0x0002,\
            NULL, ctc_oam_onu_0xc9_0x0002_config_set) /*ctc V3.0*/ \
    xx(050, OAM_CTC_ACTION_BRANCH, OAM_CTC_LEAF_CODE_ONU_FAST_LEAVE_CTRL, 0x0048,\
            NULL, ctc_oam_fast_leave_ctrl_set) \



typedef enum{
    #undef xx
    #define xx(SEQ,BRANCH,LEAF,LEAF_VAL,GET_FUNC,SET_FUNC) LEAF=LEAF_VAL,
    CTC_OAM_BUILD_RESP
}ctc_oam_leaf_e;


/* if any set oam without length, list here*/
#define CTC_OAM_ODD_SET_OAM_LISTS \
    { OAM_CTC_ACTION_BRANCH, OAM_CTC_LEAF_CODE_ONU_RESET }, \
    { OAM_INTF_CONTROL_MIB_BRANCH, OAM_INTF_LEAF_ETH_AUTO_NEG_RESTART }

typedef struct{
    cs_boolean valid;
    cs_uint8  idxBranch;
    cs_uint16 idxLeaf;
    cs_uint32 idxLen;
    cs_uint32 idxValue;
    cs_uint8  idxType; /* eth, e1, voip */
}ctc_oam_mgmt_obj_idx_t; 

typedef cs_uint32 (*ctc_oam_get_handler_t)(
        ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut);

typedef cs_uint32 (* ctc_oam_set_handler_t)(
        ctc_oam_mgmt_obj_idx_t index,
        cs_uint8   * pRecv,
        cs_uint32    Recvlen,
        cs_uint8   * pOut);


#define OAM_CTC_FUNCTION_ENTER(index)  \
    OAM_ORG_DEBUG("index is %s, index = %d \n",index.valid? "Valid":"Invalid",index.idxValue);

typedef struct {
    cs_uint8 branch;
    cs_uint16 leaf;
    ctc_oam_get_handler_t get_func;
    ctc_oam_set_handler_t set_func;
}ctc_oam_handle_t;

#define OAM_CTC_MAX_STD_HANDLER  100
typedef struct {
    oam_oui_t             oui;
    cs_uint32   num;
    ctc_oam_handle_t handlers[OAM_CTC_MAX_STD_HANDLER];
}g_oam_handle_t;


#define CTC_IMAGE_UPGRADE_NULL            0
#define CTC_IMAGE_UPGRADE_DATA            1
#define CTC_IMAGE_UPGRADE_END             2
#define CTC_IMAGE_UPGRADE_ACTIVE          3
#define CTC_IMAGE_UPGRADE_WRITING      4
#define CTC_IMAGE_UPGRADE_WRITE_FINISHED      5

#define CTC_IMAGE_WRITE          1
#define CTC_IMAGE_UPGRADE_WRITE_FINISHED      5

#define CTC_IMAGE_WRITE_SUCCESS    0
#define CTC_IMAGE_CRC_CHECK_FAILE  1
#define CTC_IMAGE_WRITE_FAIL    2

typedef struct{
    cs_uint32 status;
    cs_uint32 result;
}ctc_oam_img_upgrade_status;

void ctc_oam_vendor_std_handler_reg(
        oam_oui_t oui,
        ctc_oam_handle_t * handlers,
        cs_uint32 num);

cs_status ctc_oam_onu_default_vlan_tag_get(
        cs_port_id_t port,
        cs_uint8 * data
        );

cs_status ctc_oam_onu_translate_vlan_get(
        cs_port_id_t port,
        cs_uint32 * num,
        cs_uint8 * data
        );

cs_status ctc_oam_onu_translate_vlan_set(
        cs_port_id_t port,
        oam_ctc_onu_vlan_tag_t defVlan,
        cs_uint8 * data,
        cs_uint32 len);

cs_status ctc_oam_onu_trunk_vlan_get(
        cs_port_id_t port,
        cs_uint32 * num,
        cs_uint8 * data
        );

cs_status ctc_oam_onu_trunk_vlan_set(
        cs_port_id_t port,
        oam_ctc_onu_vlan_tag_t defVlan,
        cs_uint8 * data,
        cs_uint32 len);

cs_status ctc_oam_onu_agg_vlan_get(
        cs_port_id_t port,
        cs_uint32 * len,
        cs_uint8 * data
        );

cs_status ctc_oam_onu_agg_vlan_set(
        cs_port_id_t port,
        oam_ctc_onu_vlan_tag_t defVlan,
        cs_uint8 * data,
        cs_uint32 len);
void ctc_oam_llid_deregister_handle();
#endif

#endif /* _APP_OAM_CTC_H_ */

