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
 * osal_common.h
 *
 * $Id: osal_common.h,v 1.1.2.1.2.4.2.1.12.4 2011/08/11 08:23:40 zsun Exp $
 */

#ifndef _OSAL_COMMON_
#define _OSAL_COMMON_

/*
** Common include files
*/
#include "iros_config.h"
#ifdef printf
#undef printf
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include "stdlib.h"
#include "string.h"

#include "cs_types.h"
#include "os_core.h"
#include "osal_config.h"
#include "osal_api_core.h"
#include "util_list.h"
#include "cs_module.h"
#include "mempool.h"



/* Utility for return codes */
#define CS_ERR_DEFINE(mod, sub_mod, code)   ( ((mod)<<24) | ((sub_mod)<<16) | (code) )
#define CS_ERR_DEFINE_OSAL(code)            (  CS_ERR_DEFINE(IROS_MID_OSAL, 0, code) )


#define CS_E_OSAL_OK                    CS_E_OK
#define CS_E_OSAL_ERR                   CS_E_ERROR
#define CS_E_OSAL_INVALID_POINTER       CS_ERR_DEFINE_OSAL(0x1)
#define CS_E_OSAL_ERR_ADDR_MISALIGNED   CS_ERR_DEFINE_OSAL(0x2)
#define CS_E_OSAL_ERR_TIMEOUT           CS_ERR_DEFINE_OSAL(0x3)
#define CS_E_OSAL_INVALID_INT_NUM       CS_ERR_DEFINE_OSAL(0x4)
#define CS_E_OSAL_SEM_FAILURE           CS_ERR_DEFINE_OSAL(0x5)
#define CS_E_OSAL_SEM_TIMEOUT           CS_ERR_DEFINE_OSAL(0x6)
#define CS_E_OSAL_QUEUE_EMPTY           CS_ERR_DEFINE_OSAL(0x7)
#define CS_E_OSAL_QUEUE_FULL            CS_ERR_DEFINE_OSAL(0x8)
#define CS_E_OSAL_QUEUE_TIMEOUT         CS_ERR_DEFINE_OSAL(0x9)
#define CS_E_OSAL_QUEUE_INVALID_SIZE    CS_ERR_DEFINE_OSAL(0xA)
#define CS_E_OSAL_QUEUE_ID_ERROR        CS_ERR_DEFINE_OSAL(0xB)
#define CS_E_OSAL_ERR_NAME_TOO_LONG     CS_ERR_DEFINE_OSAL(0xC)
#define CS_E_OSAL_ERR_NO_FREE_IDS       CS_ERR_DEFINE_OSAL(0xD)
#define CS_E_OSAL_ERR_NAME_TAKEN        CS_ERR_DEFINE_OSAL(0xE)
#define CS_E_OSAL_ERR_INVALID_ID        CS_ERR_DEFINE_OSAL(0xF)
#define CS_E_OSAL_ERR_NAME_NOT_FOUND    CS_ERR_DEFINE_OSAL(0x10)
#define CS_E_OSAL_ERR_SEM_NOT_FULL      CS_ERR_DEFINE_OSAL(0x11)
#define CS_E_OSAL_ERR_INVALID_PRIORITY  CS_ERR_DEFINE_OSAL(0x12)

typedef void (*cs_io_hdl_t)(cs_uint8*, cs_uint32);

extern void cs_sys_capability_show();
extern void cs_bit_set(cs_int8 *pbuf , cs_int32 bit_pos);
extern void cs_bit_clr(cs_int8 *pbuf , cs_int32 bit_pos);
extern cs_int32 cs_bit_tst(cs_int8 *pbuf , cs_int32 bit_pos);
extern cs_int32 cs_bit_get_0(cs_int8 *pbuf , cs_int32 total_bit);
extern cs_int32 cs_bit_get_1(cs_int8 *pbuf , cs_int32 total_bit);
extern cs_int32 cs_bit_tst_all(cs_int8 *pbuf , cs_int32 total_bit);


#endif

