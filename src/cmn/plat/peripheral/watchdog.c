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
 * watchdog.c
 *
 * $Id: watchdog.c,v 1.1.4.3.8.1.6.1 2011/08/01 06:12:11 ljin Exp $
 */

#include "iros_config.h"
#include "plat_common.h"
#include "watchdog.h"

//default 5 min
static cs_uint32 wdt_sec = 300;
static cs_wdt_mode_t wdt_mode = CS_WDT_MODE_RESET;

void watchdog_enable()
{
    HAL_WRITE_UINT32(IROS_WDT_DIVIDER, WDT_DIVIDER_VAL);
    HAL_WRITE_UINT32(IROS_WDT_PRESCALE, WDT_PRESCALE_VAL);
    HAL_WRITE_UINT32(IROS_WDT_LOAD, wdt_sec * (CYG_HAL_ARM_IMST_DEV_FREQ / WDT_PRESCALE_VAL));

    HAL_WRITE_UINT32(IROS_WDT_LOAD_ENABLE, (IROS_WDT_LOAD_ENABLE_PS | IROS_WDT_LOAD_ENABLE_WDT));
    if(CS_WDT_MODE_RESET == wdt_mode){
        HAL_WRITE_UINT32(IROS_WDT_CTRL, (IROS_WDT_CTRL_CLKSEL_PS | IROS_WDT_CTRL_RSTEN | IROS_WDT_CTRL_WDTEN));
    }
    else if(CS_WDT_MODE_INTR == wdt_mode){
        HAL_WRITE_UINT32(IROS_WDT_INT, 0x1); /* clear interrupt */
        HAL_WRITE_UINT32(IROS_WDT_INTEN, 0x1);
        HAL_WRITE_UINT32(IROS_WDT_CTRL, (IROS_WDT_CTRL_CLKSEL_PS | IROS_WDT_CTRL_WDTEN));
    }
}

void watchdog_disable()
{
    HAL_WRITE_UINT32(IROS_WDT_CTRL, 0);
}

void watchdog_set_time(cs_uint32 _sec)
{
    wdt_sec = _sec;
}

void watchdog_get_time(cs_uint32 *_sec)
{
    *_sec = wdt_sec;
}

void watchdog_set_mode(cs_wdt_mode_t mode)
{
    if((CS_WDT_MODE_RESET == wdt_mode) || (CS_WDT_MODE_INTR == wdt_mode)){
        wdt_mode = mode;
    }
}

void watchdog_get_mode(cs_wdt_mode_t *mode)
{
    *mode = wdt_mode;
}

void watchdog_status()
{
    cs_uint32 val32;

    HAL_READ_UINT32(IROS_WDT_CTRL, val32);
    diag_printf("WDT_EN: %d, RESET_EN: %d, CLK_SEL: %d\n", BITON(val32, 0), BITON(val32, 1), BITON(val32, 2));
    diag_printf("Seconds: %d\n", wdt_sec);

    HAL_READ_UINT32(IROS_WDT_LOAD, val32);
    diag_printf("LOAD: 0x%x\n", val32);
    HAL_READ_UINT32(IROS_WDT_PRESCALE, val32);
    diag_printf("PS: 0x%x\n", val32);
    HAL_READ_UINT32(IROS_WDT_DIVIDER, val32);
    diag_printf("DIV: 0x%x\n", val32);

    HAL_READ_UINT32(IROS_WDT_COUNT, val32);
    diag_printf("COUNT: 0x%x\n", val32);
}

void watchdog_debug(cs_uint32 _sec, cs_uint32 _div, cs_uint32 _ps)
{
    diag_printf("set: load %d, div %d, ps %d\n", _sec, _div, _ps);
    HAL_WRITE_UINT32(IROS_WDT_DIVIDER, _div);
    HAL_WRITE_UINT32(IROS_WDT_PRESCALE, _ps);
    HAL_WRITE_UINT32(IROS_WDT_LOAD, _sec);
    HAL_WRITE_UINT32(IROS_WDT_LOAD_ENABLE, (IROS_WDT_LOAD_ENABLE_PS | IROS_WDT_LOAD_ENABLE_WDT));
}

