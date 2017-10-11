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
 * plat_common.h
 *
 * $Id: plat_common.h,v 1.1.4.6.4.1.6.2.2.3.2.6.2.8.8.12.2.1.20.1 2013/07/10 03:12:18 yshen Exp $
 */

#ifndef _PLAT_COMMON_H_
#define _PLAT_COMMON_H_

#include "iros_config.h"

#include "cs_types.h"
#include "plat_config.h"
#include "osal_common.h"
#include "util_list.h"
#include "tlv_parser.h"
#include "interrupt.h"
#include "cs_timer.h"
#include "startup_cfg.h"
#include "mempool.h"
#include "cache.h"
#include "image.h"
#include "iros_flash_oper.h"
#include "watchdog.h"
#include "cpu_load.h"
#include "cs_module.h"
#include "log.h"
#include "exception.h"
#include "gpio.h"
#include "uart.h"
#include "led.h"
#include "ssp.h"
#include "glb_reg_api.h"
#include "crc32.h"
#include "cs_cmn.h"
#include "cs_utils.h"
//#include "stdio.h"
#include "sdl_plat.h"
#include "iros_version.h"

/**********************************************************
 *         Register Access Methods                        *
 **********************************************************/
#ifndef __CS_REG_ACCESS_DEFINED__
#define __CS_REG_ACCESS_DEFINED__
/* Register Access macros - customize as needed */
#define CS_REG_READ(REG_ADDR, REG_DATA)     \
        HAL_READ_UINT32(REG_ADDR, REG_DATA)

#define CS_REG_WRITE(REG_ADDR, REG_DATA)    \
        HAL_WRITE_UINT32(REG_ADDR, REG_DATA)
#endif /* __CS_REG_ACCESS_DEFINED__ */


#define CS_OFFSET(_type, _member) (size_t)( &(((_type *)0)->_member) )


typedef enum
{
    RESET_TYPE_RESERVED = 0
    ,RESET_TYPE_FORCE
    ,RESET_TYPE_AUTO_ENABLE
    ,RESET_TYPE_AUTO_CANCEL
    ,RESET_TYPE_MAX
} RESET_TYPE_E;

extern void iros_system_reset(RESET_TYPE_E reset_type);
extern void cs_polling_init();

#if 1 /*Remove immenstar.h*/
extern void data_dump(char *ptext, int len, char *show_addr);
extern void data_dump_w(char *ptext, int len, char *show_addr);
extern int cmd_cmp(char *str, const char *mode);

#define IROS_MSG_UNDEF        0
#define IROS_MSG_PKT    1 
#define IROS_MSG_AAL_ALARM    2
#define IROS_MSG_IFM          3
#define IROS_MSG_GPIO         4
#define IROS_MSG_TIME_OUT     5
#define IROS_MSG_MAC_FILTER_AGED    6
#define IROS_MSG_EVENT          7
#define IROS_MSG_TERM_SERVER          8

extern cs_uint32  app_msg_q;
#define DUMPPKT(a, b, c)        if(pktTrace) cs_buffer_dump(a, b, c)
#define DUMPDOT1XPKT(a, b, c)        if(pktDot1xTrace) cs_buffer_dump(a, b, c)

typedef struct {
    cs_uint8 type;
    cs_uint8 u1Reserved;
    cs_uint16 u2Reserved;
} iros_msg_t;

extern unsigned char pktTrace;
extern unsigned char pktDot1xTrace;
extern unsigned char pktRstpTrace;

#endif

#endif

