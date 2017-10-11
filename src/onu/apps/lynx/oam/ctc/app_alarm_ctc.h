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
#ifndef _APP_ALARM_CTC_H_
#define _APP_ALARM_CTC_H_


#ifdef HAVE_CTC_OAM
#include "app_timer.h"

#define ALARM_POLLING_INTERVAL  4000 /*ms*/

/*ONU alarm*/
#define CTC_ALARM_EQUIPMENT                 0x0001
#define CTC_ALARM_POWER                       0x0002
#define CTC_ALARM_BATT_MISSING           0x0003
#define CTC_ALARM_BATT_FAILURE           0x0004
#define CTC_ALARM_BATT_VOLTLOW          0x0005
#define CTC_ALARM_PHY_INTRUSION         0x0006
#define CTC_ALARM_SELF_TEST                  0x0007
#define CTC_ALARM_ONU_TEMP_HIGH        0x0009
#define CTC_ALARM_ONU_TEMP_LOW         0x000a
#define CTC_ALARM_ONU_IAD_CONN          0x000b
#define CTC_ALARM_ONU_PONIF_SWITCH    0x000c
#define CTC_ALARM_ONU_SLP_STATUS_UPDATE    0x000d
#define CTC_ALARM_ONU_DRY_CONTACTA   0x8001
#define CTC_ALARM_ONU_DRY_CONTACTB   0x8002

/*PON alarm */
#define CTC_ALARM_RX_POW_HIGH           0x0101
#define CTC_ALARM_RX_POW_LOW            0x0102
#define CTC_ALARM_TX_POW_HIGH           0x0103
#define CTC_ALARM_TX_POW_LOW            0x0104
#define CTC_ALARM_TX_BIAS_HIGH          0x0105
#define CTC_ALARM_TX_BIAS_LOW           0x0106
#define CTC_ALARM_VCC_HIGH                0x0107
#define CTC_ALARM_VCC_LOW                 0x0108
#define CTC_ALARM_PON_TEMP_HIGH       0x0109
#define CTC_ALARM_PON_TEMP_LOW        0x010a
#define CTC_WARN_RX_POW_HIGH           0x010b
#define CTC_WARN_RX_POW_LOW            0x010c
#define CTC_WARN_TX_POW_HIGH          0x010d
#define CTC_WARN_TX_POW_LOW           0x010e
#define CTC_WARN_TX_BIAS_HIGH          0x010f
#define CTC_WARN_TX_BIAS_LOW           0x0110
#define CTC_WARN_VCC_HIGH                 0x0111
#define CTC_WARN_VCC_LOW                 0x0112
#define CTC_WARN_PON_TEMP_HIGH      0x0113
#define CTC_WARN_PON_TEMP_LOW       0x0114

#define CTC_ALARM_PON_RX_DROP         0x0115
#define CTC_ALARM_PON_TX_DROP         0x0116
#define CTC_ALARM_PON_RX_CRC           0x0117
#define CTC_ALARM_PON_TX_CRC           0x0118
#define CTC_ALARM_PON_RX_UNDER        0x0119
#define CTC_ALARM_PON_TX_UNDER    0x011a
#define CTC_ALARM_PON_RX_OVER      0x011b
#define CTC_ALARM_PON_TX_OVER       0x011c
#define CTC_ALARM_PON_RX_FRAG       0x011d
#define CTC_ALARM_PON_TX_FRAG       0x011e
#define CTC_ALARM_PON_RX_JABBER       0x011f
#define CTC_ALARM_PON_TX_JABBER       0x0120
#define CTC_ALARM_PON_RX_DISCA       0x0121
#define CTC_ALARM_PON_TX_DISCA       0x0122
#define CTC_ALARM_PON_RX_ERROR       0x0123
#define CTC_ALARM_PON_TX_ERROR       0x0124
/*#define CTC_ALARM_PON_RX_COLLIS      0x011e*/

#define CTC_WARN_PON_RX_DROP         0x0125
#define CTC_WARN_PON_TX_DROP         0x0126
#define CTC_WARN_PON_RX_CRC           0x0127
#define CTC_WARN_PON_TX_CRC           0x0128
#define CTC_WARN_PON_RX_UNDER        0x0129
#define CTC_WARN_PON_TX_UNDER    0x012a
#define CTC_WARN_PON_RX_OVER      0x012b
#define CTC_WARN_PON_TX_OVER       0x012c
#define CTC_WARN_PON_RX_FRAG       0x012d
#define CTC_WARN_PON_TX_FRAG       0x012e
#define CTC_WARN_PON_RX_JABBER       0x012f
#define CTC_WARN_PON_TX_JABBER       0x0130
#define CTC_WARN_PON_RX_DISCA       0x0131
#define CTC_WARN_PON_TX_DISCA       0x0132
#define CTC_WARN_PON_RX_ERROR       0x0133
#define CTC_WARN_PON_TX_ERROR       0x0134

/*#define CTC_WARN_PON_RX_COLLIS      0x012b*/


#define CUC_OUTOFTIME_TX_SLOT  0x0115


/*Port alarm */
#define CTC_ALARM_AUTO_NEG_FAIL       0x0301
#define CTC_ALARM_PORT_LOS                0x0302



#define CTC_ALARM_PORT_FAIL               0x0303
#define CTC_ALARM_PORT_LB                  0x0304
#define CTC_ALARM_PORT_CONGEST        0x0305
#define CTC_ALARM_PORT_RX_DROP         0x0306
#define CTC_ALARM_PORT_TX_DROP         0x0307
#define CTC_ALARM_PORT_RX_CRC           0x0308
#define CTC_ALARM_PORT_TX_CRC             0x0309
#define CTC_ALARM_PORT_RX_UNDER        0x030a
#define CTC_ALARM_PORT_TX_UNDER    0x030b
#define CTC_ALARM_PORT_RX_OVER      0x030c
#define CTC_ALARM_PORT_TX_OVER       0x030d
#define CTC_ALARM_PORT_RX_FRAG       0x030e
#define CTC_ALARM_PORT_TX_FRAG       0x030f
#define CTC_ALARM_PORT_RX_JABBER       0x0310
#define CTC_ALARM_PORT_TX_JABBER       0x0311
#define CTC_ALARM_PORT_RX_DISCA       0x0312
#define CTC_ALARM_PORT_TX_DISCA       0x0313
#define CTC_ALARM_PORT_RX_ERROR       0x0314
#define CTC_ALARM_PORT_TX_ERROR       0x0315
#define CTC_ALARM_PORT_STATUS          0x0316

/*#define CTC_ALARM_PORT_RX_COLLIS      0x030f*/

#define CTC_WARN_PORT_RX_DROP         0x0317
#define CTC_WARN_PORT_TX_DROP         0x0318
#define CTC_WARN_PORT_RX_CRC           0x0319
#define CTC_WARN_PORT_TX_CRC           0x031a
#define CTC_WARN_PORT_RX_UNDER        0x031b
#define CTC_WARN_PORT_TX_UNDER    0x031c
#define CTC_WARN_PORT_RX_OVER      0x031d
#define CTC_WARN_PORT_TX_OVER       0x031e
#define CTC_WARN_PORT_RX_FRAG       0x031f
#define CTC_WARN_PORT_TX_FRAG       0x0320
#define CTC_WARN_PORT_RX_JABBER     0x0321
#define CTC_WARN_PORT_TX_JABBER     0x0322
#define CTC_WARN_PORT_RX_DISCA       0x0323
#define CTC_WARN_PORT_TX_DISCA       0x0324
#define CTC_WARN_PORT_RX_ERROR       0x0325
#define CTC_WARN_PORT_TX_ERROR       0x0326

#define CTC_WARN_PORT_STATUS          0x0327

/*#define CTC_WARN_PORT_RX_COLLIS      0x031d*/




#define CTC_ALARM_PORT_LOOPDETECT        0x0504

#define CTC_ALARM_ON                     0x02
#define CTC_ALARM_OFF                   0x01

#if 0
//for ctc 3.0
typedef enum{
    CTC_ALARM_RAISE = 0,
    CTC_ALARM_CLEAR    
}ctc_oam_alarm_op_code_e;
#else
//for ctc 2.1
typedef enum{
    CTC_ALARM_CLEAR = 0,
    CTC_ALARM_RAISE    
}ctc_oam_alarm_op_code_e;
#endif

typedef enum{
    CTC_PON_ALARM = 0,
    CTC_ONU_ALARM ,
    CTC_PORT_ALARM
}ctc_oam_alarm_class_e;

#define CUC_OUT_LOOP_VALUE       1
#define CUC_INNER_LOOP_VALUE    2

/*alarm threshold type*/
#define CTC_THRESH_UNSUPPORT       0
#define CTC_THRESH_SIGNED_HIGH       1
#define CTC_THRESH_UNSIGNED_HIGH     2
#define CTC_THRESH_SIGNED_LOW        3
#define CTC_THRESH_UNSIGNED_LOW      4

/*threshold default value*/
#define UNSIGNED_RAISED_DEF_THRESH     0xffffffff
#define UNSIGNED_CLEARED_DEF_THRESH    0x00000000
#define SIGNED_RAISED_DEF_THRESH       0x7fffffff
#define SIGNED_CLEARED_DEF_THRESH      0x80000000

#define CTC_PWRS_WAKEUP_BY_TRAFFIC          1
#define CTC_PWRS_WAKEUP_BY_TIMEROUT         2
#define CTC_PWRS_WAKEUP_BY_OLT              3

typedef struct {
    cs_uint16    alarm_code;
    cs_uint8     threshold_type;
    cs_boolean  info_need;
    cs_boolean  polling_need;
    cs_uint8     alarm_state;   /*raise or clear*/
    cs_uint8     enable;           /*alarm on or alarm off*/
    union{
        cs_uint32  unsignValue;
        cs_int32   signValue;
    }raise_alarm_threshold;  
    union{
        cs_uint32  unsignValue;
        cs_int32   signValue;
    }clear_alarm_threshold;   
} ctc_oam_alarm_info_t;

typedef struct{
    app_timer_msg_t msg_info;
    cs_uint32 polling_enable_counter;
    cs_int32  timer_handler;
    cs_uint32 timer_value;
}ctc_oam_alarm_polling_cb_t;


cs_status ctc_oam_get_alarm_config(
        ctc_oam_alarm_class_e type,
        cs_uint32 portId,
        cs_uint16  alarm_code,
        cs_uint32* alarm_config);
cs_status ctc_oam_set_alarm_config(
        ctc_oam_alarm_class_e type,
        cs_uint32 portId,
        cs_uint16  alarm_code,
        cs_uint8 enable);
cs_status ctc_oam_set_alarm_threshold(
        ctc_oam_alarm_class_e type,
        cs_uint32 portId,
        cs_uint16  alarm_code,
        cs_uint32 raiseValue,
        cs_uint32 clearValue);
cs_status ctc_oam_get_alarm_threshold(
        ctc_oam_alarm_class_e type,
        cs_uint32 portId,
        cs_uint16  alarm_code,
        cs_uint32* raiseValue,
        cs_uint32 *clearValue);
void ctc_oam_build_alarm_event(
        ctc_oam_alarm_class_e type,
        cs_port_id_t port,
        cs_uint16  alarm_code,
        cs_uint8 alarm_op,
        cs_uint64 alarm_info);
void ctc_oam_alarm_polling();
void ctc_oam_alarm_init();


void ctc_oam_clear_alarm_event(
        ctc_oam_alarm_class_e type,
        cs_port_id_t port,
        cs_uint16  alarm_code);

#endif

#endif /* _APP_ALARM_CTC_H_ */

