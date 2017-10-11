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
 * cs_module.h
 *
 * $Id: cs_module.h,v 1.1.2.1.2.2.8.4.8.6.2.1.4.3 2012/06/11 07:09:54 lwang Exp $
 */
#ifndef _CS_MODULE_H_
#define _CS_MODULE_H_

#include "cs_types.h"

typedef enum
{
    IROS_MID_OSAL             = 0,
    IROS_MID_TIMER            = 1,
    IROS_MID_INTR             = 2,
    IROS_MID_CPULOAD          = 3,
    IROS_MID_MALLOC           = 4,
    IROS_MID_IFETH            = 5,
    IROS_MID_EXCEPT           = 6,
    IROS_MID_LOGGER           = 7,
    IROS_MID_AAL              = 8,
    IROS_MID_APP              = 9,
    IROS_MID_CLI              = 10,
    IROS_MID_SC               = 11,
    IROS_MID_OAM              = 12,
    IROS_MID_1X               = 13,
    IROS_MID_HELLO            = 14,
    IROS_MID_IGMP             = 15,
    IROS_MID_OOBCMD           = 16,
    IROS_MID_L2FTP            = 17,
    IROS_MID_EVENT            = 18,
    IROS_MID_ETHDRV           = 19,
    IROS_MID_STP              = 20,
    IROS_MID_SNMP             = 21,
    IROS_MID_UPGRADE          = 22,
    IROS_MID_PKT_HDR          = 23,
    IROS_MID_PACKET           = 24,
    IROS_MID_SYS              = 25,
    IROS_MID_SAL              = 26,
    IROS_MID_WEB              = 27,
    IROS_MID_SLIC             = 28,
    IROS_MID_DMA              = 29,
    IROS_MID_ESDMA            = 30,
    IROS_MID_VOIP             = 31,
    IROS_MID_ORG_OAM          = 32,
    IROS_MID_DHCP             = 33,
    IROS_MID_STACK_RSTP       = 34,
    IROS_MID_PPPOE            = 35,
    IROS_MID_VLAN             = 36,
    IROS_MID_PPP              = 37,
    IROS_MID_PWRS             = 38,
    IROS_MID_CAPTURE          = 39,
    IROS_MID_MAX
} IROS_MID_E;


typedef struct
{
    cs_int32 mod_id;
    cs_int8  mod_name[8];
}sys_mod_desc_t;

extern sys_mod_desc_t sys_mod_desc[];
extern cs_int32 sys_mod_number;

extern cs_int32 cs_sys_mod_init();
extern cs_int8 *cs_sys_mod_desc_get(cs_int32 mod_id);

#endif

