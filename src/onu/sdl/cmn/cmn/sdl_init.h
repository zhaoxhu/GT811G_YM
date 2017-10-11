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
* $Id: sdl_init.h,v 1.1.2.1.2.9 2011/08/02 02:36:12 panfeng Exp $
*/

#ifndef __SDL_INIT_H__
#define __SDL_INIT_H__

#include "cs_types.h"
#include "aal_mpcp.h"

#include "sdl_fec_cmn.h"
#include "aal_cls.h"

typedef enum
{
    MPCP_WINDOW_FIT_MODE_SP             = 0,
    MPCP_WINDOW_FIT_MODE_FIT            = 1,
    MPCP_WINDOW_FIT_MODE_RPT_SP_SCH_FIT = 2,
}mpcp_window_fit_mode_t;

typedef enum
{
    MPCP_ZQT_MODE_NORMAL = 0,
    MPCP_ZQT_MODE_TIMER  = 1,
}mpcp_zqt_mode_t;


typedef struct
{
    /* PON */
    cs_boolean                      laser_polarity;
    cs_uint16                       laser_on_time;
    cs_uint16                       laser_off_time;
    cs_boolean                      laser_always_on;

    /* MPCP */
    cs_uint16                       gate_timeout;
    mpcp_window_fit_mode_t          window_fit;
    mpcp_zqt_mode_t                 zqt_mode;
    cs_aal_mpcp_pending_grant_t     pending_grant;
    cs_uint8                        keep_silence_time;

    /* ENC */
    cs_uint8                        enc_mode;

    /* FEC */
    cs_boolean                      fec_en;
    cs_uint8                        fec_ctc_mode;

    /*VLAN*/
    cs_uint8                        max_vlan_num;

    /* MAC FILTER */
    cs_uint8                        max_mac_oper_entry;
    cs_uint32                       max_mac_oper_aging_time;
    cs_uint8                        fe_cls_default_timer;

    /* FDB */
    cs_uint32                       max_fdb_entry;
    cs_uint32                       fdb_aging_cycle_time; /* 125M core clock. So cycle time is 1/125M = 8ns */
    cs_uint32                       fdb_aging_max_time;   /* 0xfff */
    cs_uint32                       max_static_mac_entry;
    cs_uint32                       max_vlan_mac_limit_entry;
    cs_boolean                      fdb_limit_include_static; /* FDB limit includes the num of static entries? */

    /* UNI */
    cs_boolean                      mtu_tag_exclude; /* when calculate MTU, exclude the tag or not */
    cs_boolean                      phy_support_1000M; /* whether the PHY support 1000M speed */
    cs_uint32                       uni_tx_fifo_threshould;
    cs_uint32                       uni_tx_fifo_ready_high;
    cs_uint32                       uni_tx_fifo_ready_low;
    cs_uint32                       uni_phy_mod;
    cs_uint32                       mtu_max_value;
    cs_uint8                        max_uni_num;
    cs_boolean                      pon_lpbk_turnoff_uni_led;

    /*Domain table*/
    cs_aal_cl_domain_res_t          *domain_table_up;
    cs_aal_cl_domain_res_t          *domain_table_dn;

    /* QoS */
    cs_uint8                        max_queue_num;
    cs_uint32                       max_uni_rate;/*****the maximum rate for each uni port*******/
    
}sdl_init_cfg_t;


#endif

