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

#ifdef HAVE_TERMINAL_SERVER
#include "plat_common.h"
#include "terminal_server.h"
#ifdef HAVE_DB_MANAGMENT
#include "db_instance.h"
#endif

cs_status ts_uart_init(uart_config_t *uart_cfg)
{
    cs_callback_context_t    context;
    cs_status  res = CS_E_OK;

    cs_plat_uart_disable(context,0,0,uart_cfg->uart_id);
    cs_plat_uart_baudrate_set(context,0,0,uart_cfg->uart_id,uart_cfg->baud_rate);
    cs_plat_uart_databits_set(context,0,0,uart_cfg->uart_id,uart_cfg->data_bits);
    cs_plat_uart_parity_set(context,0,0,uart_cfg->uart_id,uart_cfg->parity);
    cs_plat_uart_mode_set(context,0,0,uart_cfg->uart_id,uart_cfg->mode);
    cs_plat_uart_duplex_set(context,0,0,uart_cfg->uart_id,uart_cfg->duplex);
    cs_plat_uart_enable(context,0,0,uart_cfg->uart_id);

    return res;
}

cs_status ts_putc(cs_uint8 uart,char ch)
{
    cs_callback_context_t    context;
    cs_status  res = CS_E_OK;

    res = cs_plat_uart_write(context,0,0,uart,1,&ch);
    
    return res;
}

cs_status ts_getc(cs_uint8 uart,cs_uint8 *ch)
{
    cs_callback_context_t    context;
    cs_status  res = CS_E_OK;

    res = cs_plat_uart_read(context,0,0,uart,ch);
    
    return res;
}

void  ts_uart_send(cs_uint8 uart,char * buffer, unsigned int len)
{
    int count = 0;

    for(count = len; count > 0; count--){
        ts_putc(uart,*buffer++);
    }
}

cs_status ts_onu_uart_disable(cs_uint8 uart_id)
{
    cs_callback_context_t    context;
    cs_plat_uart_disable(context,0,0,uart_id);
    return CS_E_OK;
}

cs_status ts_onu_uart_enable(cs_uint8 uart_id)
{
    cs_callback_context_t    context;
    cs_plat_uart_enable(context,0,0,uart_id);
    return CS_E_OK;
}

void ts_get_config(cs_uint8 uart_id,cs_uint8 *buff, cs_int32 len)
{
#ifdef HAVE_DB_MANAGMENT
    cs_uint32 id;

    switch (uart_id) {
        case 1:
            id = DB_CTC_TRM_SVR0_CFG_ID;
            break;
        case 2:
            id = DB_CTC_TRM_SVR1_CFG_ID;
            break;
        case 3:
            id = DB_CTC_TRM_SVR2_CFG_ID;
            break;
        case 4:
            id = DB_CTC_TRM_SVR3_CFG_ID;
            break;
        default:
            return;
    } 
    
    db_read(id,buff, len);
#endif
    return;
}

void ts_save_config(cs_uint8 uart_id,cs_uint8 *buff, cs_int32 len)
{
#ifdef HAVE_DB_MANAGMENT
    cs_uint32 id;

    switch (uart_id) {
        case 1:
            id = DB_CTC_TRM_SVR0_CFG_ID;
            break;
        case 2:
            id = DB_CTC_TRM_SVR1_CFG_ID;
            break;
        case 3:
            id = DB_CTC_TRM_SVR2_CFG_ID;
            break;
        case 4:
            id = DB_CTC_TRM_SVR3_CFG_ID;
            break;
        default:
            return;
    } 
    
    db_write(id,buff, len);
    db_save_to_flash();
#endif
    return;
}


#endif

