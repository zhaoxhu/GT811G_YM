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

DEFINITIONS:  "DEVICE" means the Cortina Systems?LynxD SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems?SDK software.

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
#ifdef HAVE_CTC_OAM
#include "plat_common.h"
#include "aal_util.h"
#include "aal_mpcp.h"
#include "aal_bm.h"
#include "sdl_api.h"
#include "power_saving.h"
#include "app_alarm_ctc.h"
#include "oam_timer.h"
#include "app_gpio.h"

static pwrs_pkgc_cfg_t pkgc_cfg;
static void (*pkgc_state_condition_table[STATE_PKGC_STATE_MAX][CT_PKGC_MAX])(void);
cs_uint32 pwrs_thread_count = 0;
cs_uint32 pwrs_msg_q;
cs_uint8* g_state_string[STATE_PKGC_STATE_MAX] = {
    (cs_uint8*)"Wait state",
    (cs_uint8*)"Confirm SC state",
    (cs_uint8*)"Parse SC state",
    (cs_uint8*)"Start watchdog state",
    (cs_uint8*)"Power down state",
    (cs_uint8*)"Power up state",
    (cs_uint8*)"Early power up state",
    (cs_uint8*)"Confirm SC type state",
    (cs_uint8*)"Stop watchdog state",
    (cs_uint8*)"Wakeup  state",
    (cs_uint8*)"Wakeup timer state"
};

cs_boolean pwrs_sleep_state_get()
{
    if(pkgc_cfg.onu_state == STATE_PKGC_WAIT)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
cs_status pwrs_event(cs_int32 type, cs_uint8 *data, cs_uint16 len)
{
    pwrs_msg_t *pMsg = NULL;
    cs_int32 ret;

    pMsg = (pwrs_msg_t *)iros_malloc(IROS_MID_EVENT, sizeof(pwrs_msg_t)+len);
    if (!pMsg) {
        diag_printf("pwrs_event iros_malloc failed.\n");
        return CS_E_ERROR;
    }

    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "event: %d\n", type);
    
    pMsg->type = type;
    if(data)
        memcpy(pMsg->data, data, len);

    ret = cs_pri_queue_put(pwrs_msg_q , (void *) & pMsg, sizeof(cs_uint32), CS_OSAL_NO_WAIT, APP_QUEUE_PRI_3);
    if (ret) {
        cs_log_msg(IROS_LOG_LEVEL_MAJ, IROS_MID_PWRS, "pwrs_event cs_queue_put failed %d\n", ret);
        iros_free(pMsg);
        return CS_E_ERROR;
    }
    return CS_E_OK;
}


static void pwrs_timer_reset()
{
    if(pkgc_cfg.slp_duration_max_timer)
    {
        cs_timer_del(pkgc_cfg.slp_duration_max_timer);
        pkgc_cfg.slp_duration_max_timer = CS_INVALID_TIMER;
    }
    
    if(pkgc_cfg.slp_int_timer)
    {
        cs_timer_del(pkgc_cfg.slp_int_timer);
        pkgc_cfg.slp_int_timer = CS_INVALID_TIMER;
    }
}

void sleep_duration_max_timer()
{
    pwrs_event(PWRS_MSG_SLP_MAX_TIMEOUT, NULL, 0);
}

static void pwrs_max_durations_timer_restart()
{
    if(pkgc_cfg.slp_duration_max_timer)
    {
        cs_timer_del(pkgc_cfg.slp_duration_max_timer);
        pkgc_cfg.slp_duration_max_timer = CS_INVALID_TIMER;
    }
    pkgc_cfg.slp_duration_max_timer = cs_timer_add(pkgc_cfg.slp_duration_max, sleep_duration_max_timer, NULL);
}

static void pwrs_oam_event_up(cs_uint32 alarm_info)
{
    ctc_oam_build_alarm_event(CTC_ONU_ALARM, 0, CTC_ALARM_ONU_SLP_STATUS_UPDATE, CTC_ALARM_RAISE, alarm_info);
    ctc_oam_clear_alarm_event(CTC_ONU_ALARM, 0, CTC_ALARM_ONU_SLP_STATUS_UPDATE);
}

void __mpcp_timedrift_set(cs_boolean flag)
{
    cs_aal_mpcp_timer_msk_t msk ;
    cs_aal_mpcp_timer_cfg_t cfg;

    memset(&msk, 0, sizeof(msk));
    memset(&cfg, 0, sizeof(cfg));
    msk.s.timedrift_dereg_en = 1;
    cfg.timedrift_dereg_en =  flag;
    aal_mpcp_timer_cfg_set(msk, &cfg);
}

static void pwrs_pkgc_state_set(pwrs_pkgc_onu_state state)
{
    cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_PWRS, 
        "State [%d] change to [%d] : %s\n", pkgc_cfg.onu_state, state, g_state_string[state]);
    pkgc_cfg.onu_state = state;
    if(state == STATE_PKGC_WAIT)
    {
        pwrs_timer_reset();
        pkgc_cfg.slp_flag = 0;

        if(pkgc_cfg.slp_mode == PWRS_TRX || pkgc_cfg.slp_mode_bak == PWRS_TRX)
            oam_hold_link_lost(FALSE);

        if(!pkgc_cfg.dereg)
        {
            /*restore pon led*/
            cs_led_proc(LED_PON, LED_GREEN,LED_ON);
        }
        pkgc_cfg.dereg = FALSE;
        //cs_timer_stop(pkgc_cfg.traffic_timer);
    }
}

pwrs_pkgc_onu_state pwrs_pkgc_state_get()
{
    return pkgc_cfg.onu_state ;
}

void __wait_grt_empty()
{
    cs_boolean empty=0;  
    cs_uint32  retry=0;
    while(!empty && retry< 50000){
        REG_FIELD_READ(ONU_MAC_MPCP_STATUS, empty_grt, empty);
        retry++;
    }

    if(50000 == retry )
        cs_printf("__wait_grt_empty timeout\n");
}


void __start_mpcp_grand_permit()
{
    cs_callback_context_t   context;
    epon_request_onu_mpcp_report_status_set(context, 0, 0, TRUE);
    aal_mpcp_reg_toggle();
}

void __stop_mpcp_grand_permit()
{
    cs_callback_context_t   context;
    epon_request_onu_mpcp_report_status_set(context, 0, 0, FALSE);
    __wait_grt_empty();
}

void __start_bm_tx_scheduler()
{
    __bm_port_tx_ctrl_t     port_tx_ctrl;    
    __bm_port_tx_ctrl_msk_t port_tx_ctrl_msk;

    port_tx_ctrl_msk.u32 = 0;
    port_tx_ctrl_msk.s.pon_tx_en = 0xFF;
    port_tx_ctrl.pon_tx_en = 0xFF;
    if (__bm_port_tx_ctrl_set(port_tx_ctrl_msk, &port_tx_ctrl)) 
    {
        cs_printf("err: pwrs en pon tx bm fail...\n"); 
    }
}

void __stop_bm_tx_scheduler()
{
    __bm_port_tx_ctrl_t     port_tx_ctrl;    
    __bm_port_tx_ctrl_msk_t port_tx_ctrl_msk;

    port_tx_ctrl_msk.u32 = 0;
    port_tx_ctrl_msk.s.pon_tx_en = 0xFF;
    port_tx_ctrl.pon_tx_en = 0x00;
    if (__bm_port_tx_ctrl_set(port_tx_ctrl_msk, &port_tx_ctrl)) 
    {
        cs_printf("err: pwrs en pon tx bm fail...\n"); 
    }
}

static void pwrs_powerOnu(pwrs_power_op param)
{
    cs_callback_context_t   context;

    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS,"%-35s %s\n", __func__,(param == PWRS_POWER_DOWN) ?  "Down" : 
                               (param == PWRS_POWER_UP)   ?  "Up"   : "Unknown");

    if(param == PWRS_POWER_DOWN)
        pkgc_cfg.slp_mode_bak = pkgc_cfg.slp_mode;
    
     /*update Stat.*/  
    switch(param)
    {
        case PWRS_POWER_UP:
            pkgc_cfg.powerup_count++;
            break;
        case PWRS_POWER_DOWN:
        {
            pkgc_cfg.sleep_count++;
            /*stop grand and tx scheduler*/
            __stop_mpcp_grand_permit();
            __stop_bm_tx_scheduler();
            cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS,"%-35s pon %s disable\n",
                               __func__, (pkgc_cfg.slp_mode == PWRS_TX)?"tx" : "trx");
            break;
        }
        default: 
            break;
    }

    epon_request_onu_pon_trans_tx_pwr_set(context, 0, 0, param);
    if(PWRS_TRX == pkgc_cfg.slp_mode || PWRS_TRX == pkgc_cfg.slp_mode_bak) 
    {
        #if 0
        if(param == PWRS_POWER_DOWN)
            oam_hold_link_lost(TRUE);
        else
            oam_hold_link_lost(FALSE);
        #endif
        epon_request_onu_pon_trans_rx_pwr_set(context, 0, 0, param);
    }
    else
    {
        if(pkgc_cfg.slp_mode != PWRS_TX)
        {
            cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "not expected mode %d\n", pkgc_cfg.slp_mode);
        }
    }
    {
        cs_uint32 val32=0;
        HAL_READ_UINT32(ONU_GPIO_DATA, val32);
        val32 &= 0x18 ;
        cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS,"GPIO val 0x%04x\n",val32);
    }
    
}
static void pwrs_pkgc_sm_run(pwrs_pkgc_condition ct)
{
    pwrs_pkgc_onu_state current_state = pwrs_pkgc_state_get();
    
    if(pkgc_state_condition_table[current_state][ct])
    {
        pkgc_state_condition_table[current_state][ct]();
    }
    else
    {
        cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS,"pkgc sm check current_state %d condition %d\n",current_state,ct);
    }
        
}

void pwrs_pkgc_wait_to_confirm_sc()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    cs_log_msg(IROS_LOG_LEVEL_DBG2, IROS_MID_PWRS, "sleep_flag = %d\n", pkgc_cfg.slp_flag);
    
    pwrs_pkgc_state_set(STATE_PKGC_CONFIRM_SC);
    if(pkgc_cfg.slp_flag == 1)
        pwrs_pkgc_sm_run(CT_PKGC_DATAFLAG_IF);
    else
        pwrs_pkgc_sm_run(CT_PKGC_DATAFLAG_ELSE);
}

void pwrs_pkgc_confirm_sc_to_parse_sc()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    cs_log_msg(IROS_LOG_LEVEL_DBG2, IROS_MID_PWRS, "sleep_duration = %d  wake_duration = %d  sleep_mode = %d\n", 
        pkgc_cfg.slp_duration, pkgc_cfg.wake_duration, pkgc_cfg.slp_mode);
    pwrs_pkgc_state_set(STATE_PKGC_PARSE_SC);
    //cs_timer_restart(pkgc_cfg.traffic_timer);
    pwrs_pkgc_sm_run(CT_PKGC_UCT);
}

void pwrs_pkgc_confirm_sc_to_wait()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    pwrs_pkgc_state_set(STATE_PKGC_WAIT);
}

static void resync_timeout()
{
    pkgc_cfg.slp_duration_max_timer = CS_INVALID_TIMER;
    
    if(pkgc_cfg.slp_int_timer)
    {
        cs_timer_del(pkgc_cfg.slp_int_timer);
        pkgc_cfg.slp_int_timer = CS_INVALID_TIMER;
    }

    //pwrs_pkgc_state_set(STATE_PKGC_WAKE_UP_TIMER);
    pwrs_pkgc_sm_run(CT_PKGC_UCT);
    pwrs_oam_event_up(CTC_PWRS_WAKEUP_BY_TIMEROUT);
}

void pwrs_pkgc_parse_sc_to_start_wtdog()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    cs_log_msg(IROS_LOG_LEVEL_DBG2, IROS_MID_PWRS, "sleep_duration_max = %d\n", pkgc_cfg.slp_duration_max);
    pwrs_pkgc_state_set(STATE_PKGC_START_WTDOG);

    pkgc_cfg.slp_duration_max_timer = cs_timer_add(pkgc_cfg.slp_duration_max, sleep_duration_max_timer, NULL);
    if(CS_INVALID_TIMER == pkgc_cfg.slp_duration_max_timer)
    {
        cs_printf("start sleep duration timer error\n");
        pwrs_pkgc_state_set(STATE_PKGC_WAIT);
    }
    else
    {
        /*before go to sleep circle, disable oam link lost check*/
        if(pkgc_cfg.slp_mode == PWRS_TRX)
            oam_hold_link_lost(TRUE);

        /*disalbe timestamp drift*/
        __mpcp_timedrift_set(FALSE);

        pwrs_pkgc_sm_run(CT_PKGC_UCT);
    }
}

void sleep_timer()
{
    pwrs_event(PWRS_MSG_INT_TIMER, NULL, 0);
}

void pwrs_pkgc_start_wtdog_or_power_up_to_power_dn()
{
    cs_uint16 slp_time  =0;
    cs_uint16 delay_time =0;
    
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    
    switch( pkgc_cfg.slp_mode)
    {
        case PWRS_TX:
            delay_time = pkgc_cfg.tx_poweron_delay + pkgc_cfg.process_delay;
            break;
        case PWRS_TRX:
            delay_time = pkgc_cfg.trx_poweron_delay + pkgc_cfg.process_delay;
            break;
        default:
            break;
    }

    if(pkgc_cfg.slp_duration > delay_time)
        slp_time = pkgc_cfg.slp_duration - delay_time;
    
    pwrs_pkgc_state_set(STATE_PKGC_POWER_DN);      
    pwrs_powerOnu(PWRS_POWER_DOWN);
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "real sleep time: %d\n", slp_time);
    pkgc_cfg.slp_int_timer = cs_timer_add(slp_time, sleep_timer, NULL);
    if(CS_INVALID_TIMER == pkgc_cfg.slp_int_timer)
    {
        cs_printf("Create timer failed %s %d\n", __func__, __LINE__);
    }
}

void power_on_timer()
{
    pwrs_event(PWRS_MSG_POWERON_TIMER, NULL, 0);
}

static void pwrs_start_power_on_timer()
{
    cs_uint16 poweron_time  =0;
    
    switch( pkgc_cfg.slp_mode_bak)
    {
        case PWRS_TX:
            poweron_time = pkgc_cfg.tx_poweron_delay;
            break;
        case PWRS_TRX:
            poweron_time = pkgc_cfg.trx_poweron_delay;
            break;
        default:
            break;
    }
    pwrs_powerOnu(PWRS_POWER_UP);
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "real power on time: %d\n", poweron_time);
    if(poweron_time)
    {
        pkgc_cfg.poweron_timer = cs_timer_add(poweron_time, power_on_timer, NULL);
        if(CS_INVALID_TIMER == pkgc_cfg.poweron_timer)
        {
            cs_printf("Create timer failed %s %d\n", __func__, __LINE__);
        }
    }
    else
        power_on_timer();
}

void pwrs_pkgc_power_dn_to_power_up()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    pwrs_pkgc_state_set(STATE_PKGC_POWER_UP); 
    pwrs_start_power_on_timer();
}

void pwrs_pkgc_power_dn_to_early_power_up()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    pwrs_pkgc_state_set(STATE_PKGC_EARLY_POWER_UP); 
    pwrs_start_power_on_timer();
}

void pwrs_pkgc_power_dn_to_wake_up_timer()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    pwrs_pkgc_state_set(STATE_PKGC_WAKE_UP_TIMER); 
    pwrs_start_power_on_timer();
}

void pwrs_pkgc_power_up_to_confirm_sc_type()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    cs_log_msg(IROS_LOG_LEVEL_DBG2, IROS_MID_PWRS, "sleep_flag = %d\n", pkgc_cfg.slp_flag);
    
    
    if(!pkgc_cfg.slp_flag)
    {
        pwrs_pkgc_state_set(STATE_PKGC_CONFIRM_SC_TYPE);
        pwrs_pkgc_sm_run(CT_PKGC_DATAFLAG_IF);
    }
    else
    {
        pwrs_max_durations_timer_restart();
        //pwrs_pkgc_sm_run(CT_PKGC_DATAFLAG_ELSE);
    }
}

void pwrs_pkgc_power_dn_to_confirm_sc_type()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    cs_log_msg(IROS_LOG_LEVEL_DBG2, IROS_MID_PWRS, "sleep_flag = %d\n", pkgc_cfg.slp_flag);
    
    
    if(!pkgc_cfg.slp_flag)
    {
        pwrs_pkgc_state_set(STATE_PKGC_CONFIRM_SC_TYPE);
        pwrs_start_power_on_timer();
    }
    else
    {
        pwrs_max_durations_timer_restart();
        //pwrs_pkgc_sm_run(CT_PKGC_DATAFLAG_ELSE);
    }
}

void pwrs_pkgc_early_power_up_to_wake_up()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    
    pwrs_timer_reset();
    pwrs_pkgc_state_set(STATE_PKGC_WAKE_UP);
    pwrs_oam_event_up(CTC_PWRS_WAKEUP_BY_TRAFFIC);
    cs_thread_delay(1);
    pwrs_pkgc_sm_run(CT_PKGC_UCT);
}

void pwrs_pkgc_power_up_to_wake_up()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    if(pkgc_cfg.poweron_timer)
        pwrs_pkgc_state_set(STATE_PKGC_EARLY_POWER_UP); 
    else
    {
        cs_timer_del(pkgc_cfg.slp_int_timer);
        pwrs_pkgc_early_power_up_to_wake_up();
    }
}

void pwrs_pkgc_power_up_to_wake_up_timer()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    pwrs_pkgc_state_set(STATE_PKGC_WAKE_UP_TIMER);
    if(!pkgc_cfg.poweron_timer)
        resync_timeout();
    else
    {
        cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "wait for poweron timer\n");
    }
}

void pwrs_pkgc_confirm_sc_type_to_stop_wtdog()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    
    pwrs_timer_reset();
    pwrs_pkgc_state_set(STATE_PKGC_STOP_WTDOG);
    if(!pkgc_cfg.poweron_timer)
    {
        pwrs_oam_event_up(CTC_PWRS_WAKEUP_BY_OLT);
        pwrs_pkgc_sm_run(CT_PKGC_UCT);
    }
    else
    {
        cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "wait for poweron timer\n");
    }
}

void pwrs_pkgc_confirm_sc_type_to_wait()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    pwrs_pkgc_state_set(STATE_PKGC_WAIT);
}

void pwrs_pkgc_stop_wtdog_to_wait()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    pwrs_pkgc_state_set(STATE_PKGC_WAIT);
}

void pwrs_pkgc_wake_up_to_wait()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    pwrs_pkgc_state_set(STATE_PKGC_WAIT);
}

void pwrs_pkgc_wake_up_timer_to_wait()
{
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    pwrs_pkgc_state_set(STATE_PKGC_WAIT);
}

void power_on_timerout()
{
    pwrs_pkgc_onu_state state = pwrs_pkgc_state_get();

    pkgc_cfg.poweron_timer = CS_INVALID_TIMER;
    
    __start_mpcp_grand_permit();
    __start_bm_tx_scheduler();

    switch (state) {
        case STATE_PKGC_POWER_UP:
            pkgc_cfg.slp_int_timer = cs_timer_add(pkgc_cfg.wake_duration+pkgc_cfg.process_delay, sleep_timer, NULL);
            if(CS_INVALID_TIMER == pkgc_cfg.slp_int_timer)
            {
                cs_printf("Create timer failed %s %d\n", __func__, __LINE__);
            }
            break;
        case STATE_PKGC_EARLY_POWER_UP:
            pwrs_pkgc_sm_run(CT_PKGC_UCT);
            break;
        case STATE_PKGC_STOP_WTDOG:
            pwrs_pkgc_sm_run(CT_PKGC_UCT);
            pwrs_oam_event_up(CTC_PWRS_WAKEUP_BY_OLT);
            break;
        case STATE_PKGC_WAKE_UP_TIMER:
            resync_timeout();
            break;

        case STATE_PKGC_CONFIRM_SC_TYPE:
            pwrs_pkgc_sm_run(CT_PKGC_DATAFLAG_IF);
            break;
        default:
            break;
    }
}

void pwrs_recv_control(cs_uint8 *data)
{
    oam_pwrs_ctrl_t *ctrl = (oam_pwrs_ctrl_t *)data;

    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "slp_dur %d wake_dur %d slp_flag %d slp_mode %d\n", 
        ctrl->sleep_duration, ctrl->wait_duration, ctrl->sleep_flag, ctrl->sleep_mode);

    pkgc_cfg.slp_flag = ctrl->sleep_flag;
    if(ctrl->sleep_flag != 0)
    {
        pkgc_cfg.slp_mode = ctrl->sleep_mode;
        pkgc_cfg.slp_duration_tq = ctrl->sleep_duration;
        pkgc_cfg.wake_duration_tq = ctrl->wait_duration;

        pkgc_cfg.slp_duration = pkgc_cfg.slp_duration_tq * 16 / 1000000;
        pkgc_cfg.wake_duration = pkgc_cfg.wake_duration_tq * 16 / 1000000;
    }
    
    pwrs_pkgc_sm_run(CT_PKGC_RX_EOAM);
}

void pwrs_recv_cfg(cs_uint8 *data)
{
    oam_pwrs_config_t *pwrs_cfg = (oam_pwrs_config_t *)data;
    
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "%-35s\n", __func__);
    cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS, "early_wakeup %d max_slp_duration 0x%llx\n",
        pwrs_cfg->early_wakeup, pwrs_cfg->max_sleep_duration);

    pkgc_cfg.early_wakeup = pwrs_cfg->early_wakeup;
    pkgc_cfg.slp_duration_max_tq = pwrs_cfg->max_sleep_duration;
    pkgc_cfg.slp_duration_max = pkgc_cfg.slp_duration_max_tq * 16 / 1000000;
    
    pwrs_pkgc_sm_run(CT_PKGC_RX_EOAM);
}

void pwrs_reset()
{
    pwrs_pkgc_onu_state state = pwrs_pkgc_state_get();

    if(state > STATE_PKGC_PARSE_SC)
    {
        pwrs_powerOnu(PWRS_POWER_UP);
        if((PWRS_TX == pkgc_cfg.slp_mode || PWRS_TX == pkgc_cfg.slp_mode_bak) && pkgc_cfg.tx_poweron_delay)
        {
            cs_thread_delay(pkgc_cfg.tx_poweron_delay);
        }
        else if((PWRS_TRX == pkgc_cfg.slp_mode || PWRS_TRX == pkgc_cfg.slp_mode_bak) && pkgc_cfg.trx_poweron_delay)
        {
            cs_thread_delay(pkgc_cfg.trx_poweron_delay);
        }
        __start_mpcp_grand_permit();
        __start_bm_tx_scheduler();
    }
    pkgc_cfg.dereg = TRUE;
    pwrs_pkgc_state_set(STATE_PKGC_WAIT);
}

void pwrs_thread(cyg_addrword_t p)
{
    cs_int32 ret = CS_E_OSAL_OK;
    cs_uint32 real_len = 0;
    pwrs_msg_t *msg = NULL;
    cs_uint8  free_msg = 1;

    IROS_LOG_INF(IROS_MID_PWRS, "============Start of PWRS Thread===========\n");

    while (1) {
        ret = cs_pri_queue_get(pwrs_msg_q, (void *)&msg, sizeof(msg), &real_len, CS_OSAL_WAIT_FOREVER);
        IROS_LOG_INF(IROS_MID_PWRS, "pwrs thread recv code %d msg %d\n", ret, msg->type);

        if(ret != CS_E_OSAL_OK || msg == NULL)
            continue;

        pwrs_thread_count++;

        switch (msg->type) {

            case PWRS_MSG_INT_TIMER:
                pwrs_pkgc_sm_run(CT_PKGC_TIMER_INT_DONE);
                break;

            case PWRS_MSG_LWI:
                if(STATE_PKGC_POWER_UP == pwrs_pkgc_state_get() ||
                      STATE_PKGC_POWER_DN == pwrs_pkgc_state_get())
                {
                    pwrs_pkgc_sm_run(CT_PKGC_LWD);
                } 
                free_msg = 1;
                break;

            case PWRS_MSG_EOAM_CFG:
                pwrs_recv_cfg(msg->data);
                free_msg = 1;
                break;
                
            case PWRS_MSG_EOAM_CONTROL:
                cs_thread_delay(1);
                pwrs_recv_control(msg->data);
                free_msg = 1;
                break;
                
            case PWRS_MSG_SLP_MAX_TIMEOUT:
                pwrs_pkgc_sm_run(CT_PKGC_TIMER_RESYNC_DONE);
                break;
            case PWRS_MSG_POWERON_TIMER:
                power_on_timerout();
                break;
            case PWRS_MSG_RESET:
                pwrs_reset();
                break;
                
            default:
                IROS_LOG_INF(IROS_MID_PWRS, "unknown msg type 0x%x\n", msg->type);
                break;

        }

        if(free_msg) {
            iros_free(msg);
        }
    }

}

static void __traffic_detect(cs_boolean *us,cs_boolean *ds,traffic_detect_mask_t *mask)
{
    cs_callback_context_t   context={0};
    cs_sdl_port_uni_stats_t mib={0};    
    cs_pon_stats_t          pon_stats={0};
    cs_uint32               pon_rx_mc_delta=0;
    cs_uint32               uni_tx_mc_delta=0;
    cs_uint32               uni_drop = 0;
    cs_boolean              uni_rx_mib_changed = FALSE;

    if((NULL==mask)|| (mask->us_en && (NULL == us)) || (mask->ds_en && (NULL == ds)))
    {
        cs_printf("%s %d, par err\n",__FILE__,__LINE__);
        return;
    }
    memset(&pon_stats, 0, sizeof(cs_pon_stats_t));
    memset(&mib, 0, sizeof(cs_sdl_port_uni_stats_t));
    
    epon_request_onu_port_stats_get_pwrsv(context, 0, 0, CS_UNI_PORT_ID1, 0, &mib);
    epon_request_onu_pon_stats_get_pwrsv(context, 0, CS_PON_PORT_ID, 0, &pon_stats);
    #if 0
    /*The UNI mib and PON mib have unsync case, will cause detect fail. So roll back to only check UNI mib*/
    /*US detect*/
    /* Detect the real upsteam traffic : pon_tx_mib_changed & uni_rx_mib_changed
    ------------------------------------------------------------------------------------------------
    Pon Tx mib            | Changed       | Changed            | Unchanged  | Unchanged
    ------------------------------------------------------------------------------------------------
    UNI Rx mib            | Changed       | Unchanged          | Unchanged  | Changed
    ------------------------------------------------------------------------------------------------
    Real upstream exist?  |   Yes         |  No                | No         | No
    ------------------------------------------------------------------------------------------------
    scenario              |Real traffic   | CPU send pkt to PON| No traffic | Packet drp or to CPU.
    ------------------------------------------------------------------------------------------------
    */
    #endif
    uni_drop = aal_get_bm_cnt(BM_RMU_PT0_DROP_CNT2);
    if(mask->us_en)
    {
        if((mib.rxucfrm_cnt != pkgc_cfg.my_rx_uc_cnt) || (mib.rxmcfrm_cnt != pkgc_cfg.my_rx_mc_cnt) || (mib.rxbcfrm_cnt != pkgc_cfg.my_rx_bc_cnt))
        {
            uni_rx_mib_changed = TRUE;
            cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS,"uni mib n u/m/b %u/%u/%u\n",mib.rxucfrm_cnt,mib.rxmcfrm_cnt,mib.rxbcfrm_cnt);
            cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_PWRS,"uni mib l u/m/b %u/%u/%u\n",pkgc_cfg.my_rx_uc_cnt,pkgc_cfg.my_rx_mc_cnt,pkgc_cfg.my_rx_bc_cnt);
        }
        if(uni_rx_mib_changed)
        {   /*Real traffic detected*/
            *us = TRUE;
        }
        else
        {   /*No upstream traffic*/
            *us = FALSE;
            if(mib.rxoversizefrm_cnt >= pkgc_cfg.my_rx_os_cnt && uni_drop >= pkgc_cfg.my_rx_drop_cnt)
            {
                if((mib.rxoversizefrm_cnt - pkgc_cfg.my_rx_os_cnt) > (uni_drop - pkgc_cfg.my_rx_drop_cnt))
                    *us = TRUE;
            }
        }
    }

    /*Decide DS traffic exist*/
    if(mask->ds_en)
    {
        if(pon_stats.pon_mcframe_cnt < pkgc_cfg.my_pon_rx_mc_cnt)
            pon_rx_mc_delta =  0xFFFFFFFF - pkgc_cfg.my_pon_rx_mc_cnt + pon_stats.pon_mcframe_cnt;
        else
            pon_rx_mc_delta =   pon_stats.pon_mcframe_cnt - pkgc_cfg.my_pon_rx_mc_cnt;
        
        if(1 == pon_rx_mc_delta)
        {/*1 mc PDU entry PON port*/
            if(mib.txmcfrm_cnt < pkgc_cfg.my_tx_mc_cnt)
                uni_tx_mc_delta =  0xFFFFFFFF - pkgc_cfg.my_tx_mc_cnt + mib.txmcfrm_cnt;
            else
                uni_tx_mc_delta =   mib.txmcfrm_cnt - pkgc_cfg.my_tx_mc_cnt;
        }
        if((pon_stats.pon_frame_cnt == pkgc_cfg.my_pon_rx_uc_cnt) && (pon_stats.pon_bcframe_cnt == pkgc_cfg.my_pon_rx_bc_cnt))
        {/*No uc/bc DS*/
            if((pon_stats.pon_mcframe_cnt == pkgc_cfg.my_pon_rx_mc_cnt))
            {/*NO mc DS*/
                *ds = FALSE;
            }
            else if((pon_rx_mc_delta == 1) && (uni_tx_mc_delta == 0))
            {/*No uc/bc, mc changed. But only get 1 mc packet ,consider as control packet, decide as no DS*/
                *ds = FALSE;
            }
            else
            {/* mc changed and get more than 1 mc packet or pon rx and uni tx both changed, decide as has DS*/
                *ds = TRUE;
            }
        }
        else
        {/*have uc or bc DS, decide as has DS*/
            *ds = TRUE;
        }
    }
    pkgc_cfg.my_rx_uc_cnt = mib.rxucfrm_cnt;
    pkgc_cfg.my_rx_bc_cnt = mib.rxbcfrm_cnt;
    pkgc_cfg.my_rx_mc_cnt = mib.rxmcfrm_cnt;
    pkgc_cfg.my_tx_mc_cnt = mib.txmcfrm_cnt;
    pkgc_cfg.my_rx_os_cnt = mib.rxoversizefrm_cnt;
    pkgc_cfg.my_rx_drop_cnt = aal_get_bm_cnt(BM_RMU_PT0_DROP_CNT2);
     
    pkgc_cfg.my_pon_rx_uc_cnt = pon_stats.pon_frame_cnt;
    pkgc_cfg.my_pon_rx_mc_cnt = pon_stats.pon_mcframe_cnt;
    pkgc_cfg.my_pon_rx_bc_cnt = pon_stats.pon_bcframe_cnt;
}

void traffic_detect_timer()
{
    cs_boolean us=FALSE, ds=FALSE;
    traffic_detect_mask_t mask;
    mask.us_en = 1;
    mask.ds_en = 0;

    __traffic_detect(&us, &ds, &mask);
     
    if(us && pkgc_cfg.early_wakeup)
    {
        pwrs_event(PWRS_MSG_LWI, NULL, 0);
    } 

}

cs_status pwrs_pkgc_cap_get(cs_uint8 *slp_mode, cs_uint8 *early_wakeup)
{
    if(!slp_mode || !early_wakeup)
        return CS_E_PARAM;
    
    *slp_mode = pkgc_cfg.slp_mode_cap;
    *early_wakeup = pkgc_cfg.early_wakeup_cap;

    return CS_E_OK;
}

cs_status pwrs_pkgc_cfg_get(cs_uint8 *early_wakeup, cs_uint64 *max_duration)
{
    if(!max_duration || !early_wakeup)
        return CS_E_PARAM;

    *early_wakeup = !pkgc_cfg.early_wakeup;
    *max_duration = pkgc_cfg.slp_duration_max_tq;

    return CS_E_OK;
}

cs_status pwrs_pkgc_cfg_set(cs_uint8 early_wakeup, cs_uint64 max_duration)
{
    oam_pwrs_config_t pwrs_cfg;
    
    if(early_wakeup > PWRS_EARLY_WAKEUP_DISABLE)
        return CS_E_PARAM;

    if(max_duration < PWRS_PKGC_DURATION_MIN)
        return CS_E_PARAM;
    
    pwrs_cfg.early_wakeup = !early_wakeup;
    pwrs_cfg.max_sleep_duration = max_duration;

    return pwrs_event(PWRS_MSG_EOAM_CFG, (cs_uint8 *)&pwrs_cfg, sizeof(pwrs_cfg));
}

cs_status pwrs_pkgc_control_set(oam_pwrs_ctrl_t *ctrl)
{
    if(ctrl->sleep_mode > PWRS_TRX || ctrl->sleep_flag >= PWRS_SLP_FLAG_MAX)
        return CS_E_PARAM;

    if((ctrl->sleep_flag == 0) && (ctrl->sleep_duration!=0 || ctrl->wait_duration!=0 || ctrl->sleep_mode!=0))
    {
        return CS_E_PARAM;
    }

    if(ctrl->sleep_flag != 0)
    {
        if(ctrl->sleep_duration > PWRS_PKGC_DURATION_MAX || ctrl->sleep_duration < PWRS_PKGC_DURATION_MIN)
            return CS_E_PARAM;

        if(ctrl->wait_duration > PWRS_PKGC_DURATION_MAX || ctrl->wait_duration < PWRS_PKGC_DURATION_MIN)
            return CS_E_PARAM;
    }

    return pwrs_event(PWRS_MSG_EOAM_CONTROL, (cs_uint8 *)ctrl, sizeof(oam_pwrs_ctrl_t));
}

void pwrs_show()
{
    cs_printf("*************** Cap **************************************\n");
    cs_printf("early wake up cap      [%d]\n",pkgc_cfg.early_wakeup_cap);
    cs_printf("sleep mode cap         [%d]\n",pkgc_cfg.slp_mode_cap);
    cs_printf("*************** Mib counter ******************************\n");
    cs_printf("Power saving package C:\n");
    cs_printf("current onu_state      [%d][%s]\n",pkgc_cfg.onu_state,
    STATE_PKGC_WAIT              == pkgc_cfg.onu_state ? "STATE_PKGC_WAIT"          :           
    STATE_PKGC_CONFIRM_SC        == pkgc_cfg.onu_state ? "STATE_PKGC_CONFIRM_SC":
    STATE_PKGC_PARSE_SC          == pkgc_cfg.onu_state ? "STATE_PKGC_PARSE_SC":
    STATE_PKGC_START_WTDOG       == pkgc_cfg.onu_state ? "STATE_PKGC_START_WTDOG"      :
    STATE_PKGC_POWER_DN          == pkgc_cfg.onu_state ? "STATE_PKGC_POWER_DN"         :
    STATE_PKGC_POWER_UP          == pkgc_cfg.onu_state ? "STATE_PKGC_POWER_UP"      :
    STATE_PKGC_EARLY_POWER_UP    == pkgc_cfg.onu_state ? "STATE_PKGC_EARLY_POWER_UP"   :
    STATE_PKGC_CONFIRM_SC_TYPE   == pkgc_cfg.onu_state ? "STATE_PKGC_CONFIRM_SC_TYPE"      :            
    STATE_PKGC_STOP_WTDOG        == pkgc_cfg.onu_state ? "STATE_PKGC_STOP_WTDOG":
    STATE_PKGC_WAKE_UP           == pkgc_cfg.onu_state ? "STATE_PKGC_WAKE_UP":
    STATE_PKGC_WAKE_UP_TIMER     == pkgc_cfg.onu_state ? "STATE_PKGC_WAKE_UP_TIMER":
    "UNKNOWN");
    cs_printf("sleep flag             [%d]\n",pkgc_cfg.slp_flag);
    cs_printf("sleep mode             [%d]\n",pkgc_cfg.slp_mode);
    cs_printf("sleep mode  bak        [%d]\n",pkgc_cfg.slp_mode_bak);
    cs_printf("early wake up          [%d]\n",pkgc_cfg.early_wakeup);
    cs_printf("max sleep duration     [%d]\n",pkgc_cfg.slp_duration_max);
    cs_printf("sleep duration         [%d]\n",pkgc_cfg.slp_duration);
    cs_printf("wake duration          [%d]\n",pkgc_cfg.wake_duration);
    cs_printf("max sleep duration_tq  [%lld]\n",pkgc_cfg.slp_duration_max_tq);
    cs_printf("sleep duration_tq      [%d]\n",pkgc_cfg.slp_duration_tq);
    cs_printf("wake duration_tq       [%d]\n",pkgc_cfg.wake_duration_tq);
    cs_printf("process delay          [%d]\n",pkgc_cfg.process_delay);
    cs_printf("tx poweron delay       [%d]\n",pkgc_cfg.tx_poweron_delay);
    cs_printf("trx poweron delay      [%d]\n",pkgc_cfg.trx_poweron_delay);
    cs_printf("power up count         [%d]\n",pkgc_cfg.powerup_count);
    cs_printf("sleep count            [%d]\n",pkgc_cfg.sleep_count);
    cs_printf("max duration timer     [%d]\n",pkgc_cfg.slp_duration_max_timer);
    cs_printf("sleep int timer        [%d]\n",pkgc_cfg.slp_int_timer);
    cs_printf("power on timer         [%d]\n",pkgc_cfg.poweron_timer);
    cs_printf("*************** Mib counter ******************************\n");
    cs_printf("my_rx_uc_cnt           [%d]\n",pkgc_cfg.my_rx_uc_cnt);
    cs_printf("my_rx_mc_cnt           [%d]\n",pkgc_cfg.my_rx_mc_cnt);
    cs_printf("my_rx_bc_cnt           [%d]\n",pkgc_cfg.my_rx_bc_cnt);
    cs_printf("my_tx_mc_cnt           [%d]\n",pkgc_cfg.my_tx_mc_cnt);
    cs_printf("my_rx_os_cnt           [%d]\n",pkgc_cfg.my_rx_os_cnt);
    cs_printf("my_rx_drop_cnt         [%d]\n",pkgc_cfg.my_rx_drop_cnt);
}
cs_uint32 pwrs_thread_id = 0;

void pwrs_pkgc_init()
{
    cs_uint16 poweron = 240;
    memset(&pkgc_cfg, 0, sizeof(pkgc_cfg));

    pkgc_cfg.early_wakeup_cap = PWRS_EARLY_WAKEUP_ENABLE;
    pkgc_cfg.early_wakeup = !PWRS_EARLY_WAKEUP_ENABLE;
    pkgc_cfg.slp_mode_cap = PWRS_TX_TRX;
    startup_config_read(CFG_ID_ONU_PWS_TRX_POWERON_TIME, 2, 
            (cs_uint8 *)&poweron);
    pkgc_cfg.trx_poweron_delay = poweron;

    pkgc_state_condition_table[STATE_PKGC_WAIT][CT_PKGC_RX_EOAM] = pwrs_pkgc_wait_to_confirm_sc;
    pkgc_state_condition_table[STATE_PKGC_CONFIRM_SC][CT_PKGC_DATAFLAG_IF] = pwrs_pkgc_confirm_sc_to_parse_sc;
    pkgc_state_condition_table[STATE_PKGC_CONFIRM_SC][CT_PKGC_DATAFLAG_ELSE] = pwrs_pkgc_confirm_sc_to_wait;
    pkgc_state_condition_table[STATE_PKGC_PARSE_SC][CT_PKGC_UCT] = pwrs_pkgc_parse_sc_to_start_wtdog;
    pkgc_state_condition_table[STATE_PKGC_START_WTDOG][CT_PKGC_UCT] = pwrs_pkgc_start_wtdog_or_power_up_to_power_dn;
    pkgc_state_condition_table[STATE_PKGC_POWER_DN][CT_PKGC_TIMER_INT_DONE] = pwrs_pkgc_power_dn_to_power_up;
    pkgc_state_condition_table[STATE_PKGC_POWER_DN][CT_PKGC_LWD] = pwrs_pkgc_power_dn_to_early_power_up;
    pkgc_state_condition_table[STATE_PKGC_POWER_DN][CT_PKGC_TIMER_RESYNC_DONE] = pwrs_pkgc_power_dn_to_wake_up_timer;
    pkgc_state_condition_table[STATE_PKGC_POWER_DN][CT_PKGC_RX_EOAM] = pwrs_pkgc_power_dn_to_confirm_sc_type;
    pkgc_state_condition_table[STATE_PKGC_POWER_UP][CT_PKGC_TIMER_INT_DONE] = pwrs_pkgc_start_wtdog_or_power_up_to_power_dn;
    pkgc_state_condition_table[STATE_PKGC_POWER_UP][CT_PKGC_RX_EOAM] = pwrs_pkgc_power_up_to_confirm_sc_type;
    pkgc_state_condition_table[STATE_PKGC_POWER_UP][CT_PKGC_LWD] = pwrs_pkgc_power_up_to_wake_up;
    pkgc_state_condition_table[STATE_PKGC_POWER_UP][CT_PKGC_TIMER_RESYNC_DONE] = pwrs_pkgc_power_up_to_wake_up_timer;
    pkgc_state_condition_table[STATE_PKGC_CONFIRM_SC_TYPE][CT_PKGC_DATAFLAG_IF] = pwrs_pkgc_confirm_sc_type_to_stop_wtdog;
    pkgc_state_condition_table[STATE_PKGC_CONFIRM_SC_TYPE][CT_PKGC_DATAFLAG_ELSE] = pwrs_pkgc_confirm_sc_type_to_wait;
    pkgc_state_condition_table[STATE_PKGC_STOP_WTDOG][CT_PKGC_UCT] = pwrs_pkgc_stop_wtdog_to_wait;
    pkgc_state_condition_table[STATE_PKGC_EARLY_POWER_UP][CT_PKGC_UCT] = pwrs_pkgc_early_power_up_to_wake_up;
    pkgc_state_condition_table[STATE_PKGC_WAKE_UP][CT_PKGC_UCT] = pwrs_pkgc_wake_up_to_wait;
    pkgc_state_condition_table[STATE_PKGC_WAKE_UP_TIMER][CT_PKGC_UCT] = pwrs_pkgc_wake_up_timer_to_wait;

    cs_pri_queue_create(&pwrs_msg_q, "PwrsMsgQ", 128, 8, APP_QUEUE_PRI_NUM);

    pkgc_cfg.traffic_timer = cs_circle_timer_add(10, traffic_detect_timer, NULL);
    
    cs_thread_create(&pwrs_thread_id, PWRS_THREAD_NAME, pwrs_thread, NULL, PWRS_THREAD_STACKSIZE, PWRS_THREAD_PRIORITY, 0);

    /*disalbe timestamp drift*/
    __mpcp_timedrift_set(FALSE);

    //cs_timer_stop(pkgc_cfg.traffic_timer);
}
#endif
