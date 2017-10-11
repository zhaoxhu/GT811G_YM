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

DEFINITIONS:  "DEVICE" means the Cortina Systems?Daytona SDK product.
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
 * app_init.c
 *
 * $Id: app_init.c,v 1.1.4.3.2.5.2.4.2.2.2.2.2.3.2.4.2.31.2.58.2.1.2.50.2.6.4.1.12.2 2013/11/06 09:13:05 hyuan Exp $
 */

/****************************************************************

OVERVIEW

This file contains the application task entry point

****************************************************************/

#ifdef HAVE_WEB_SERVER
#include "cgi_cmn.h"
#endif
#include "plat_common.h"
#include "cs_cmn.h"
#include "event.h"
#include "app_gpio.h"
#include "app_daemon.h"
#include "app_timer.h"
#include "img_upgrade.h"
#include "oam_api.h"
#include "port_stats.h"
#include "packet.h"
#include "iros_pkt.h"

#ifdef HAVE_ONU_HELLO
#include "onu_hello.h"
#endif


#ifdef HAVE_DB_MANAGMENT
#include "db_instance.h"
#include "db_api.h"
#endif

#ifdef HAVE_LOOP_DETECT
#include "loop_detect.h"
#endif

#ifdef HAVE_DHCP
#include "dhcp.h"
#endif

#ifdef HAVE_PPPOE
#include "pppoe.h"
#endif

#ifdef HAVE_OOB_MGMT
#include "app_oam_cs.h"
#endif

#ifdef HAVE_ONU_RSTP
#include "stp_api.h"
#endif

#ifdef HAVE_ONU_8021X
#include "port_dot1x.h"
#endif

#ifdef HAVE_MC_CTRL
#include "mc_api.h"
#endif

#ifdef HAVE_CTC_OAM
#include "app_oam_ctc_adapt.h"
#endif

/*
 * externs
 */

/*
 * globals
 */

cs_uint32 app_msg_q;
cs_uint32 app_thread_count = 0;

void app_queue_create()
{
    cs_int32 ret = 0;

    ret = cs_pri_queue_create(&app_msg_q, "AppMsgQ", 128, 8, APP_QUEUE_PRI_NUM);
    if(ret != CS_E_OSAL_OK) {
        IROS_LOG_CRI(IROS_MID_APP, "APP MsgQ create failed, 0x%x\n", ret);
    }
}

void app_global_init()
{

    IROS_LOG_INF(IROS_MID_APP, "============Start of APP Global Init===========\n");

    app_queue_create();

    app_daemon_queue_create();
    
#ifdef HAVE_PPPOE
    app_pppoe_queue_create();
#endif
}

void app_ref_init()
{

    IROS_LOG_INF(IROS_MID_APP, "============Start of APP REF Init===========\n");
    oam_org_init();

#ifdef HAVE_GWD_OAM
	oam_gwd_init();
#endif

#ifdef HAVE_WEB_SERVER
    //cgi_init();
    cgi_org_init();
#endif

#ifdef HAVE_OOB_MGMT
    oam_cs_init();
#endif

#ifdef HAVE_CTC_OAM
    mc_mode_set ( 0, MC_SNOOPING);
#endif

}


void app_task_init()
{
    app_pkt_init();

    event_init();

    app_daemon_init();

    img_upgrade_init();

#ifdef HAVE_LOOP_DETECT
    loop_detect_init();
#endif

#ifdef HAVE_DHCP
    dhcp_init();
#endif
    
    oam_init();

    app_port_stats_init();

#ifdef HAVE_ONU_HELLO
    app_hello_init();
#endif

#ifdef HAVE_ONU_8021X
    dot1x_port_init();
#endif

#ifdef HAVE_MC_CTRL
    if(mc_init()) {
        return;
    }
    else {
        mc_up_port_cfg_t up_cfg;
        
        up_cfg.up_port = CS_PON_PORT_ID;
        up_cfg.up_port_lrn = 0;
        mc_up_port_set(0, &up_cfg);
    }
#endif

#ifdef HAVE_ONU_RSTP
    stp_init();
#endif

#ifdef HAVE_DB_MANAGMENT
    db_init();
#endif

#ifdef HAVE_PPPOE
    pppoe_init();
#endif

    iros_pkt_init();

    app_ref_init();

}

void app_thread(cyg_addrword_t p)
{
    cs_int32 ret = CS_E_OSAL_OK;
    cs_uint32 real_len = 0;
    iros_msg_t *msg = NULL;
    cs_uint8  free_msg = 1;

    IROS_LOG_INF(IROS_MID_APP, "============Start of APP Thread===========\n");

    while (1) {
        ret = cs_pri_queue_get(app_msg_q, (void *)&msg, sizeof(msg), &real_len, CS_OSAL_WAIT_FOREVER);
        IROS_LOG_INF(IROS_MID_APP, "appsentry recv code %d msg %d\n", ret, msg->type);

        if(ret != CS_E_OSAL_OK || msg == NULL)
            continue;

        app_thread_count++;

        switch (msg->type) {

            case IROS_MSG_GPIO:
                /*free_msg = onu_gpio_event_proc((cs_gpio_msg_t *) msg);*/
                break;

            case IROS_MSG_TIME_OUT:
                free_msg = onu_timerout_proc((app_timer_msg_t *)msg);
                break;

             case IROS_MSG_PKT:
                free_msg = app_pkt_msg_proc((cs_pkt_t *)msg);
                break;

            case IROS_MSG_EVENT:
                free_msg = onu_evt_msg_proc(msg);
                break;

            default:
                IROS_LOG_INF(IROS_MID_APP, "unknown msg type 0x%x\n", msg->type);
                break;

        }

        if(free_msg) {
            iros_free(msg);
        }
    }

}


