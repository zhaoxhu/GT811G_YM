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
#ifndef __APP_OAM_CTC_ADAPT_H__
#define __APP_OAM_CTC_ADAPT_H__
#include "app_oam_ctc.h"


#define CTC_DEFAULT_TPID 0x8100
#define CTC_MCVLAN_OPER_DEL 0
#define CTC_MCVLAN_OPER_ADD 1
#define CTC_MCVLAN_OPER_CLR 2
#define CTC_MCVLAN_OPER_GET 3
#define CTC_MCVLAN_TAG_OPER_SWAP 2

typedef struct{
    cs_ip_t     mgmt_ip;
    cs_uint32   mask;
    cs_ip_t     mgmt_gateway;
    cs_uint16   mgmt_svlan;
    cs_uint16   mgmt_cvlan;
    cs_uint8    mgmt_data_pri;
}ctc_oam_mgmt_parm_t;

void ctc_oam_adapt_init();

cs_status oam_port_event_register_adapt (
         void                        *call_back);

cs_status ctc_oam_eth_admin_state_get_adapt(
        cs_port_id_t port,
        cs_uint8*  admin);

cs_status ctc_oam_eth_auto_admin_get_adapt(
        cs_port_id_t port,
        cs_uint8*  admin);

cs_status ctc_oam_eth_auto_local_cap_get_adapt(
        cs_port_id_t port,
        cs_uint32 * value,
        cs_uint8 * getNum);

cs_status ctc_oam_eth_auto_advts_cap_get_adapt(
        cs_port_id_t port,
        cs_uint32 * value,
        cs_uint8 * getNum);

cs_status ctc_oam_eth_fec_ability_get_adapt(
        cs_uint8 *ability);


cs_status ctc_oam_eth_fec_mode_get_adapt(
        cs_uint32* fecMode);

cs_status ctc_oam_eth_fec_mode_set_adapt(
        cs_uint32 fecMode);

cs_status ctc_oam_onu_opmdiag_get_adapt(
        cs_int16 * temp,
        cs_uint16 * volt,
        cs_uint16 * tx_bias,
        cs_uint16 * tx_power,
        cs_uint16 * rx_power);

cs_status ctc_oam_sla_get_adapt(
        cs_uint8  * oper,
        cs_uint8  * scheduling_scheme,
        cs_uint8  * high_pri_boundary,
        cs_uint32 * cycle_len,
        cs_uint8  * num_of_service,
        oam_sla_queue_t * sla);

cs_status ctc_oam_sla_set_adapt(
        cs_uint8   oper,
        cs_uint8   scheduling_scheme,
        cs_uint8   high_pri_boundary,
        cs_uint32  cycle_len,
        cs_uint8   num_of_service,
        oam_sla_queue_t * sla);


cs_status ctc_oam_holdover_get_adapt(
        cs_uint32 *holdover_state,
        cs_uint32 *holdover_time);

cs_status ctc_oam_holdover_set_adapt(
        cs_uint32 holdover_state,
        cs_uint32 holdover_time);


cs_status ctc_oam_onu_mgmt_config_get_adapt(
        ctc_oam_mgmt_parm_t * mgmt);

cs_status ctc_oam_onu_mgmt_config_set_adapt(
        ctc_oam_mgmt_parm_t * parm);

cs_status ctc_oam_onu_active_pon_get_adapt(
        cs_uint8* pon);

cs_status ctc_oam_onu_active_pon_set_adapt(
        cs_uint8 pon);

cs_status ctc_oam_eth_port_link_get_adapt(
        cs_port_id_t port,
        cs_uint8*  linkState);

cs_status ctc_oam_eth_port_pause_get_adapt(
        cs_port_id_t port,
        cs_uint8*  flowControl); 

cs_status ctc_oam_eth_port_pause_set_adapt(
        cs_port_id_t port,
        cs_uint8 flowControl);


cs_status ctc_oam_eth_port_policing_get_adapt(
        cs_port_id_t port,
        cs_uint8 * oper,
        cs_uint32 * cir,
        cs_uint32 * cbs,
        cs_uint32 *ebs);

cs_status ctc_oam_eth_port_policing_set_adapt(
        cs_port_id_t port,
        cs_uint8   oper,
        cs_uint32  cir,
        cs_uint32  cbs,
        cs_uint32  ebs);


cs_status ctc_oam_eth_ds_rate_limit_get_adapt(
        cs_port_id_t port,
        cs_uint8 * oper,
        cs_uint32 * cir,
        cs_uint32 * pir);

cs_status ctc_oam_eth_ds_rate_limit_set_adapt(
        cs_port_id_t port,
        cs_uint8   oper,
        cs_uint32  cir,
        cs_uint32  pir);

cs_status ctc_oam_loop_detect_status_set_adapt(
        cs_port_id_t port,
        cs_uint32 status) ;

cs_status ctc_oam_loop_detect_status_get_adapt(
        cs_port_id_t port,
        cs_uint32 *status) ;

cs_status ctc_oam_disable_looped_status_set_adapt(
        cs_port_id_t port,
        cs_uint32 status) ;


cs_status ctc_oam_eth_mac_aging_time_get_adapt(
        cs_uint32 *time) ;

cs_status ctc_oam_eth_mac_aging_time_set_adapt(
        cs_uint32 time) ;

cs_status ctc_oam_onu_mac_limit_get_adapt(
    cs_port_id_t port,
    cs_uint16*  num);

cs_status ctc_oam_onu_mac_limit_set_adapt(
    cs_port_id_t port,
    cs_uint16  num);


cs_status ctc_oam_onu_port_mode_set_adapt(
    cs_port_id_t port,
    cs_uint8  mode);

cs_status ctc_oam_onu_port_mode_get_adapt(
    cs_port_id_t port,
    cs_uint8*  mode);
cs_status ctc_oam_onu_port_work_mode_get_adapt(
    cs_port_id_t port,
    cs_uint8*  mode);

cs_status ctc_oam_onu_port_stats_get_adapt(
        cs_port_id_t port,
        oam_ctc_eth_port_stats_t*stats);

cs_status ctc_oam_pon_port_stat_get_adapt(
        oam_ctc_onu_llid_stats1 *stats1,
        oam_ctc_onu_llid_stats2 *stats2);

cs_status ctc_oam_onu_cfg_counter_get_adapt(cs_uint32 *counter);
cs_status ctc_oam_onu_cfg_counter_set_adapt(cs_uint32 counter);
cs_status ctc_oam_onu_cfg_mgmt_set_adapt(cs_uint8);

typedef struct{
    cs_uint16 agg_vlan_num;
    oam_ctc_onu_vlan_tag_t agg_dst_vlan; 
    oam_ctc_onu_vlan_tag_t agg_vlan[CTC_ONU_AGGR_VLAN_NUM];
}ctc_oam_agg_vlan_t;

cs_status ctc_oam_onu_default_vlan_tag_get_adapt(
        cs_port_id_t port,
        cs_uint16 * tpid,
        cs_uint16 * tag);

cs_status ctc_oam_onu_vlan_mode_get_adapt(
        cs_port_id_t port,
        cs_uint8 * mode);

cs_status ctc_oam_onu_translate_vlan_get_adapt(
        cs_port_id_t port,
        cs_uint16 * translate_num,
        oam_ctc_onu_vlan_xlate_entry * vlan);

cs_status ctc_oam_onu_trunk_vlan_get_adapt(
        cs_port_id_t port,
        cs_uint16 * trunk_num,
        oam_ctc_onu_vlan_tag_t * vlan);

cs_status ctc_oam_onu_agg_vlan_get_adapt(
        cs_port_id_t port,
        cs_uint16 * agg_num,
        ctc_oam_agg_vlan_t * vlan);


cs_status ctc_oam_onu_transparent_vlan_set_adapt(
        cs_port_id_t port);

cs_status ctc_oam_onu_tag_vlan_set_adapt(
        cs_port_id_t port,
        oam_ctc_onu_vlan_tag_t vlan);

cs_status ctc_oam_onu_translate_vlan_set_adapt(
        cs_port_id_t port,
        oam_ctc_onu_vlan_tag_t defvlan,
        cs_uint16 num,
        oam_ctc_onu_vlan_xlate_entry * vlan);

cs_status ctc_oam_onu_trunk_vlan_set_adapt(
        cs_port_id_t port,
        oam_ctc_onu_vlan_tag_t defvlan,
        cs_uint16 num,
        oam_ctc_onu_vlan_tag_t * vlan);

cs_status ctc_oam_onu_agg_vlan_set_adapt(
        cs_port_id_t port,
        oam_ctc_onu_vlan_tag_t defvlan,
        cs_uint16 num,
        ctc_oam_agg_vlan_t * vlan);


cs_status ctc_oam_onu_classification_get_adapt(
        cs_port_id_t port,
        cs_uint32 *num,
        cs_uint8 * data);

cs_status ctc_oam_onu_classification_set_adapt(
        cs_uint8 action,
        cs_port_id_t port,
        cs_uint32 num,
        cs_uint8 * data);

cs_status ctc_oam_onu_mcvlan_get_adapt(
        cs_port_id_t port,
        cs_uint16 *num,
        cs_uint16 * vlanIds);

cs_status ctc_oam_onu_mcvlan_set_adapt(
        cs_port_id_t port,
        cs_uint8 oper,
        cs_uint16 num,
        cs_uint16 * vlanIds);


cs_status ctc_oam_onu_mctagstrip_get_adapt(
        cs_port_id_t port,
        cs_uint8 * tagStriped,
        cs_uint8 * num,
        oam_ctc_iptv_vid_t * vlans);

cs_status ctc_oam_onu_mctagstrip_set_adapt(
        cs_port_id_t port,
        cs_uint8  tagStriped,
        cs_uint8  num,
        oam_ctc_iptv_vid_t * vlans);

cs_status ctc_oam_onu_mc_switch_get_adapt(
        cs_uint8 *mode);

cs_status ctc_oam_onu_mc_switch_set_adapt(
        cs_uint8 mode);

cs_status ctc_oam_onu_mc_control_type_get_adapt(
        cs_uint8 * type);

cs_status ctc_oam_onu_mc_control_type_set_adapt(
        cs_uint8  type);

cs_status ctc_oam_onu_mc_control_vlan_mac_get_adapt(
        cs_uint16 *num,
        oam_ctc_onu_mc_control_vlan_mac_t * vlanMac);

cs_status ctc_oam_onu_mc_control_mac_ip_get_adapt(
        cs_uint16 *num,
        oam_ctc_onu_mc_control_mac_ip_t * macIp);

cs_status ctc_oam_onu_mc_control_vlan_ip_get_adapt(
        cs_uint16 *num,
        oam_ctc_onu_mc_control_vlan_ip_t* vlanIp);

cs_status ctc_oam_onu_mc_control_clr_adapt();

cs_status ctc_oam_onu_mc_control_vlan_mac_set_adapt(
        cs_uint8 action,
        cs_uint16 num,
        oam_ctc_onu_mc_control_vlan_mac_t * vlanMac);

cs_status ctc_oam_onu_mc_control_mac_ip_set_adapt(
        cs_uint8 action,
        cs_uint16 num,
        oam_ctc_onu_mc_control_mac_ip_t * macIp);

cs_status ctc_oam_onu_mc_control_vlan_ip_set_adapt(
        cs_uint8 action,
        cs_uint16 num,
        oam_ctc_onu_mc_control_vlan_ip_t* vlanIp);

cs_status ctc_oam_onu_grp_nmb_max_get_adapt(
        cs_port_id_t port,
        cs_uint8 *  maxGrp);

cs_status ctc_oam_onu_grp_nmb_max_set_adapt(
        cs_port_id_t port,
        cs_uint8   maxGrp);

cs_status ctc_oam_onu_fast_leave_ability_get_adapt(
        cs_uint32 * num,
        cs_uint32 * mode);

cs_status ctc_oam_onu_fast_leave_state_get_adapt(
        cs_uint32 * state);


cs_status ctc_oam_onu_llid_queue_config_get_adapt (
        cs_llid_t llid,
        cs_uint8 * num,
        oam_ctc_onu_llid_config_t * config);

cs_status ctc_oam_onu_llid_queue_config_set_adapt (
        cs_llid_t llid,
        cs_uint8  num,
        oam_ctc_onu_llid_config_t * config);

cs_status ctc_oam_onu_alarm_get_adapt(
        ctc_oam_mgmt_obj_idx_t index,
        cs_uint16 alarm_id,
        cs_uint32 * alarm_config);

cs_status ctc_oam_onu_alarm_set_adapt(
        ctc_oam_mgmt_obj_idx_t index,
        cs_uint16 alarm_id,
        cs_uint32  alarm_config);

cs_status ctc_oam_onu_alarm_threshold_get_adapt(
        ctc_oam_mgmt_obj_idx_t index,
        cs_uint16 alarm_id,
        cs_uint32  *alarm_threshold,
        cs_uint32  *clear_threshold);

cs_status ctc_oam_onu_alarm_threshold_set_adapt(
        ctc_oam_mgmt_obj_idx_t index,
        cs_uint16 alarm_id,
        cs_uint32  raise_threshold,
        cs_uint32  clear_threshold);
    
cs_status ctc_oam_onu_laser_control_set_adapt(
        cs_uint16 action,
        cs_uint8 * onuId,
        cs_uint32  transId);


cs_status ctc_oam_eth_admin_state_set_adapt(
        cs_port_id_t port,
        cs_uint32 phyAdmin);

cs_status ctc_oam_auto_neg_restart_set_adapt(
        cs_port_id_t port);

cs_status ctc_oam_auto_neg_ctrl_set_adapt(
        cs_port_id_t port,
        cs_uint32 autoNegAdmin);

cs_status ctc_oam_onu_reset_set_adapt();

cs_status ctc_oam_fast_leave_ctrl_set_adapt(
        cs_uint32 fastLeaveAdmin);

cs_status ctc_oam_onu_sleep_control_set_adapt(
        cs_uint32 sleep_duration,
        cs_uint32 wait_duration,
        cs_uint8 sleep_flag,
        cs_uint8 sleep_mode);

cs_status ctc_oam_onu_stats_monitor_status_get_adapt(
        cs_port_id_t port,
        cs_uint16 *monitor_status,
        cs_uint32 * monitor_period);

cs_status ctc_oam_onu_stats_monitor_status_set_adapt(
        cs_port_id_t port,
        cs_uint16 monitor_status,
        cs_uint32  monitor_period);

cs_status ctc_oam_onu_curr_stats_get_adapt(
        cs_port_id_t port,
        oam_ctc_onu_port_stats_t * stats);

cs_status ctc_oam_onu_curr_stats_set_adapt(
        cs_port_id_t port,
        oam_ctc_onu_port_stats_t * stats);

cs_status ctc_oam_onu_histroy_stats_get_adapt(
        cs_port_id_t port,
        oam_ctc_onu_port_stats_t * stats);

cs_status ctc_oam_onu_power_saving_cap_get_adapt(
        cs_uint8 * sleep_mode,
        cs_uint8 * early_wakeup);

cs_status ctc_oam_onu_power_saving_config_get_adapt(
        cs_uint8 * early_wakeup,
        cs_uint64 *max_sleep_duration);

cs_status ctc_oam_onu_power_saving_config_set_adapt(
        cs_uint8 early_wakeup,
        cs_uint64 max_sleep_duration);

cs_status ctc_oam_onu_pon_protect_parm_get_adapt(
        cs_uint16 * opt_time,
        cs_uint16 * mpcp_time);

cs_status ctc_oam_onu_pon_protect_parm_set_adapt(
        cs_uint16 opt_time,
        cs_uint16 mpcp_time);

cs_status ctc_oam_onucap_get_adapt(
        cs_uint8 * numUSQueues,
        cs_uint8 * maxQueueUSPort,
        cs_uint8 * numDSQueues,
        cs_uint8 * maxQueueDSPort);

cs_status ctc_oam_event_status_get_adapt(
        oam_ctc_event_entry_t entry,
        cs_uint32 *status);
cs_status ctc_oam_event_thresh_set_adapt(
        oam_ctc_event_thresh_t config);
cs_status ctc_oam_event_status_set_adapt(
        oam_ctc_event_status_t config);
cs_status ctc_oam_event_thresh_get_adapt(
        oam_ctc_event_entry_t entry,
        cs_uint32 *raise_thresh,
        cs_uint32 *clear_thresh);
cs_status ctc_oam_auth_keep_silence_set_adapt(cs_boolean enable);
cs_status ctc_mc_mode_set_adapt(
    cs_uint32 mode
    );

cs_status ctc_mc_gmi_set_adapt(
    cs_uint32 gmi
    );       


cs_status ctc_onu_1pcos_set_adapt (
    cs_uint32                       pri,
    cs_uint32                       cos
    );

cs_status ctc_port_queue_size_set_adapt (
    cs_port_id_t                   port,
    cs_uint32                       queue_id,
    cs_uint32                       queue_size
    );

cs_status ctc_onu_mtu_set_adapt (
    cs_uint16           mtu
    );

typedef struct {
        cs_uint8  oper;
        cs_uint32 cir;
        cs_uint32 cbs;
        cs_uint32 ebs;
}ctc_port_us_policy_t;
typedef struct {
        cs_uint8  oper;
        cs_uint32 cir;
        cs_uint32 pir;
}ctc_port_ds_rate_limit_t;

typedef struct{
    cs_uint8 admin_status;
    cs_uint8 link_status;
    cs_uint8 work_mode;
    cs_uint8 auto_neg_mode;
    cs_uint8 pause;
    cs_uint8 loopdected;
    cs_uint8 mc_tag_strip;
    cs_uint16 mc_max_group;
    ctc_port_us_policy_t     policy; 
    ctc_port_ds_rate_limit_t ds_rate;

    cs_uint8  vlan_mode; 
    oam_ctc_onu_vlan_tag_t def_vlan_tag;
    cs_uint16 translate_num;
    oam_ctc_onu_vlan_xlate_entry translate_vlan[CTC_VLAN_MODE_TRANSLATE_MAX];
    cs_uint16 trunk_num ;
    oam_ctc_onu_vlan_tag_t trunk_vlan[CTC_VLAN_MODE_TRUNK_MAX];
}ctc_port_info_t;

cs_status ctc_get_port_info(cs_port_id_t port, ctc_port_info_t * info);


#endif
