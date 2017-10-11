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
/*
 *
 * power_saving.h
 *
 */
#ifdef HAVE_CTC_OAM

#define PWRS_MSG_LWI                1 
#define PWRS_MSG_INT_TIMER          2
#define PWRS_MSG_EOAM_CFG           3
#define PWRS_MSG_EOAM_CONTROL       4
#define PWRS_MSG_SLP_MAX_TIMEOUT    5
#define PWRS_MSG_POWERON_TIMER      6
#define PWRS_MSG_RESET              7

#define PWRS_PKGC_DURATION_MAX  62500000
#define PWRS_PKGC_DURATION_MIN  62500

typedef enum{
     PWRS_POWER_DOWN       = 0,
     PWRS_POWER_UP         = 1,
}pwrs_power_op;

typedef enum{    
    PWRS_NOT_SUPP  = 0,   
    PWRS_TX        = 1,   
    PWRS_TRX       = 2,  
    PWRS_TX_TRX    = 3, 
}pwrs_slpmode;

typedef enum{    
    PWRS_WAKEUP          = 0,   
    PWRS_SLEEP           = 1,   
    PWRS_UPDAET          = 2,  
    PWRS_SLP_FLAG_MAX    = 3, 
}pwrs_slpflag;

typedef enum{    
    PWRS_EARLY_WAKEUP_ENABLE  = 0,   
    PWRS_EARLY_WAKEUP_DISABLE = 1,   
}pwrs_wakeup_mode;

typedef enum{   
    STATE_PKGC_WAIT                      = 0, 
    STATE_PKGC_CONFIRM_SC                = 1, 
    STATE_PKGC_PARSE_SC                  = 2, 
    STATE_PKGC_START_WTDOG               = 3, 
    STATE_PKGC_POWER_DN                  = 4, 
    STATE_PKGC_POWER_UP                  = 5,
    STATE_PKGC_EARLY_POWER_UP            = 6, 
    STATE_PKGC_CONFIRM_SC_TYPE           = 7, 
    STATE_PKGC_STOP_WTDOG                = 8, 
    STATE_PKGC_WAKE_UP                   = 9, 
    STATE_PKGC_WAKE_UP_TIMER             = 10, 
    STATE_PKGC_STATE_MAX,
}pwrs_pkgc_onu_state;

typedef enum{   
    CT_PKGC_UCT                                  = 0,   
    CT_PKGC_DATAFLAG_IF                          = 1,   
    CT_PKGC_DATAFLAG_ELSE                        = 2,   
    CT_PKGC_ACKCAP_DIS_AND_SLEEP_MODE_TX_OR_TRX  = 3,
    CT_PKGC_LWD                                  = 4, 
    CT_PKGC_RX_EOAM                              = 5,   
    CT_PKGC_TIMER_RESYNC_DONE                    = 6, 
    CT_PKGC_TIMER_INT_DONE                       = 7, 
    CT_PKGC_MAX ,  
}pwrs_pkgc_condition;

 typedef struct {
    pwrs_pkgc_onu_state onu_state;
    cs_uint8  slp_flag;
    cs_uint8  slp_mode;
    cs_uint8  slp_mode_bak;
    cs_uint8  early_wakeup;
    cs_uint8  early_wakeup_cap;
    cs_uint8  slp_mode_cap;
    cs_boolean  dereg;
    cs_uint32 slp_duration_max;
    cs_uint64 slp_duration_max_tq;
    cs_uint32 slp_duration;
    cs_uint32 slp_duration_tq;
    cs_uint32 wake_duration;
    cs_uint32 wake_duration_tq;
    cs_uint32 slp_duration_max_timer;
    cs_uint32 slp_int_timer;  
    cs_uint32 poweron_timer;  // need clean
    cs_uint32 traffic_timer;  
    cs_uint32 powerup_count;
    cs_uint32 sleep_count;
    cs_uint32 process_delay;
    cs_uint32 tx_poweron_delay;
    cs_uint32 trx_poweron_delay;
    cs_uint32 my_rx_uc_cnt;
    cs_uint32 my_rx_mc_cnt;
    cs_uint32 my_rx_bc_cnt; 
    cs_uint32 my_tx_mc_cnt;
    cs_uint32 my_rx_os_cnt;
    cs_uint32 my_rx_drop_cnt;
    cs_uint32 my_pon_rx_uc_cnt;
    cs_uint32 my_pon_rx_mc_cnt;
    cs_uint32 my_pon_rx_bc_cnt; 
} pwrs_pkgc_cfg_t;

 /* Traffic detect mask stats */
typedef struct {
    cs_uint32 us_en        ;
    cs_uint32 ds_en        ;
}traffic_detect_mask_t;

typedef struct {
    cs_uint32 type;
    cs_uint8  data[0];
} pwrs_msg_t;

typedef struct {
    cs_uint32  sleep_duration;
    cs_uint32  wait_duration;
    cs_uint8   sleep_flag;
    cs_uint8   sleep_mode;
} oam_pwrs_ctrl_t;

typedef struct {
    cs_uint8  early_wakeup;
    cs_uint64 max_sleep_duration;
} oam_pwrs_config_t;

extern void pwrs_pkgc_init();
extern void pwrs_show();
extern cs_status pwrs_event(cs_int32 type, cs_uint8 *data, cs_uint16 len);
extern pwrs_pkgc_onu_state pwrs_pkgc_state_get();
extern cs_status pwrs_pkgc_cap_get(cs_uint8 *slp_mode, cs_uint8 *early_wakeup);
extern cs_status pwrs_pkgc_cfg_get(cs_uint8 *early_wakeup, cs_uint64 *max_duration);
extern cs_status pwrs_pkgc_cfg_set(cs_uint8 early_wakeup, cs_uint64 max_duration);
extern cs_status pwrs_pkgc_control_set(oam_pwrs_ctrl_t *ctrl);
cs_boolean pwrs_sleep_state_get();
#endif
