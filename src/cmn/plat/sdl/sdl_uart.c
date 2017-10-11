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
 * uart.c
 *
 */

/****************************************************************

OVERVIEW

This file contains the UART driver code

****************************************************************/

#include "plat_common.h"
#include "uart.h"
#include "sdl_uart.h"

/*----------------------------------------------
**
**                   UART SDL APIs
**
**---------------------------------------------*/
/* clear interrupt status */
cs_status cs_plat_uart_int_status_clear(
    CS_IN       cs_callback_context_t    context,
    CS_IN       cs_dev_id_t              device,
    CS_IN       cs_llid_t                llidport,
    CS_IN       cs_uint8                 uart_id
    )
{
    uart_int_status_clear(uart_id);

    return CS_E_OK;
}

/* Get the UART configurations */
cs_status cs_plat_uart_cfg_get(
    CS_IN   cs_callback_context_t    context,
    CS_IN   cs_dev_id_t              device,
    CS_IN   cs_llid_t                llidport,
    CS_IN   cs_uint8                 uart_id,
    CS_OUT  cs_sdl_uart_mode_t       *mode,
    CS_OUT  cs_uint8                 *data_bits,
    CS_OUT  cs_uint8                 *parity,
    CS_OUT  cs_uint8                 *duplex,
    CS_OUT  cs_uint32                *baud_rate
    )
{
    uart_config_t config;
    
    uart_cfg_get(uart_id,&config);
    
    *mode       = config.mode;
    *data_bits  = config.data_bits;
    *parity     = config.parity;
    *duplex     = config.duplex;
    *baud_rate  = config.baud_rate;

    return CS_E_OK;
}

/* Init the UART with the configuration parameters */
cs_status cs_plat_uart_cfg_set(
    CS_IN   cs_callback_context_t    context,
    CS_IN   cs_dev_id_t              device,
    CS_IN   cs_llid_t                llidport,
	CS_IN   cs_uint8                 uart_id,
	CS_IN   cs_sdl_uart_mode_t       mode,
	CS_IN   cs_uint8                 data_bits,
	CS_IN   cs_uint8                 parity,
	CS_IN   cs_uint8                 duplex,
	CS_IN   cs_uint32                baud_rate
    )
{
    uart_config_t config;
    
    if(uart_id >= MAX_UART_NUM)
        return CS_E_PARAM;

    config.uart_id      = uart_id;
    config.mode         = mode;
    config.data_bits    = data_bits;
    config.parity       = parity;
    config.duplex       = duplex;
    config.baud_rate    = baud_rate;
    
    uart_init(&config);
    
    return CS_E_OK;
}

/* Installed interrupt handler */
cs_status cs_plat_uart_int_handler_set(
    CS_IN   cs_callback_context_t       context,
    CS_IN   cs_dev_id_t                 device,
    CS_IN   cs_llid_t                   llidport,
    CS_IN   cs_uint8                    uart_id,
    CS_IN   cs_uart_intr_handler_t      handler
)
{
    if((uart_id >= MAX_UART_NUM) || (NULL == handler))
        return CS_E_PARAM;
    
    switch(uart_id) {
        case 0:
        {
            cs_intr_reg(ONU_INT_FLAGBIT_UART0,"UART0_intr",handler,NULL,INTR_THREAD_CB);
            break;
        }
        case 1:
        {
            cs_intr_reg(ONU_INT_FLAGBIT_UART1,"UART1_intr",handler,NULL,INTR_THREAD_CB);
            break;
        }
        case 2:
        {
            cs_intr_reg(ONU_INT_FLAGBIT_UART2,"UART2_intr",handler,NULL,INTR_THREAD_CB);
            break;
        }
        case 3:
        {
            cs_intr_reg(ONU_INT_FLAGBIT_UART3,"UART3_intr",handler,NULL,INTR_THREAD_CB);
            break;
        }
        case 4:
        {
            cs_intr_reg(ONU_INT_FLAGBIT_UART4,"UART4_intr",handler,NULL,INTR_THREAD_CB);
            break;
        }
        default:
            break;
    }
    
    return CS_E_OK;
    
}

/* Get the UART enable/ disable status */
cs_status cs_plat_uart_ctrl_get(
    CS_IN   cs_callback_context_t       context,
    CS_IN   cs_dev_id_t                 device,
    CS_IN   cs_llid_t                   llidport,
    CS_IN   cs_uint8                    uart_id,
    CS_OUT  cs_sdl_uart_ctrl_t          *ctrl
)
{
    uart_ctrl_get(uart_id,ctrl);

    return CS_E_OK;
    
}

/* Enable/ disable UART */
cs_status cs_plat_uart_ctrl_set(
    CS_IN   cs_callback_context_t       context,
    CS_IN   cs_dev_id_t                 device,
    CS_IN   cs_llid_t                   llidport,
    CS_IN   cs_uint8                    uart_id,
    CS_IN   cs_sdl_uart_ctrl_t          ctrl
)
{
    uart_ctrl_set(uart_id,ctrl);

    return CS_E_OK;
    
}

/* UART get a char */
cs_status cs_plat_uart_data_get(
    CS_IN       cs_callback_context_t    context,
    CS_IN       cs_dev_id_t              device,
    CS_IN       cs_llid_t                llidport,
    CS_IN       cs_uint8                 uart_id,
    CS_IN_OUT   cs_uint8                 *data
)
{
    if(uart_id >= MAX_UART_NUM)
        return CS_E_PARAM;
    
    if(uart_getc_nonblock(uart_id,data))
        return CS_E_OK;
    else
        return CS_E_ERROR;
    
}

/* UART put a char */
cs_status cs_plat_uart_data_set(
    CS_IN   cs_callback_context_t    context,
    CS_IN   cs_dev_id_t              device,
    CS_IN   cs_llid_t                llidport,
    CS_IN   cs_uint8                 uart_id,
    CS_IN   cs_uint8                 data
)
{
    if(uart_id >= MAX_UART_NUM)
        return CS_E_PARAM;
    
    if(uart_putc(uart_id,data))
        return CS_E_OK;
    else
        return CS_E_ERROR;
    
}



#ifdef HAVE_TERMINAL_SERVER
//added by zhuxh
cs_status cs_plat_uart_mode_get(
	CS_IN	cs_callback_context_t	context,
	CS_IN	cs_dev_id_t				device,
	CS_IN	cs_llid_t				llidport,
	CS_IN	cs_uint8				uart_id,
	CS_OUT	cs_uart_mode_t			*mode
)
{
	uart_config_t config;
	cs_status ret = CS_E_OK;

	ret = uart_cfg_get(uart_id, &config);

	*mode = config.mode;

	return ret;
}

cs_status cs_plat_uart_mode_set(
	CS_IN	cs_callback_context_t	context,
	CS_IN	cs_dev_id_t				device,
	CS_IN	cs_llid_t				llidport,
	CS_IN	cs_uint8				uart_id,
	CS_IN	cs_uart_mode_t			mode
)
{
	uart_config_t config;
	uart_cfg_mask_t mask;
	cs_status ret = CS_E_OK;

	if(uart_id >= MAX_UART_NUM)
		return CS_E_PARAM;

	mask.u32 = 0;
	mask.s.mode = 1;

	config.mode = mode;
	ret = uart_cfg_set(uart_id, mask, &config);

	return ret;
}

cs_status cs_plat_uart_databits_get(
	CS_IN	cs_callback_context_t	context,
	CS_IN	cs_dev_id_t				device,
	CS_IN	cs_llid_t				llidport,
	CS_IN	cs_uint8				uart_id,
	CS_OUT	cs_uint8				*data_bits
)
{
	uart_config_t config;
	cs_status ret = CS_E_OK;

	ret = uart_cfg_get(uart_id, &config);

	*data_bits = config.data_bits;

	return ret;
}

cs_status cs_plat_uart_databits_set(
	CS_IN	cs_callback_context_t	context,
	CS_IN	cs_dev_id_t				device,
	CS_IN	cs_llid_t				llidport,
	CS_IN	cs_uint8				uart_id,
	CS_IN	cs_uint8				data_bits
)
{
	uart_config_t config;
	uart_cfg_mask_t mask;
	cs_status ret = CS_E_OK;

	if(uart_id >= MAX_UART_NUM)
	return CS_E_PARAM;

	mask.u32 = 0;
	mask.s.data_bits = 1;

	config.data_bits = data_bits;

	ret = uart_cfg_set(uart_id, mask, &config);

	return ret;
}

cs_status cs_plat_uart_parity_get(
	CS_IN	cs_callback_context_t	context,
	CS_IN	cs_dev_id_t				device,
	CS_IN	cs_llid_t				llidport,
	CS_IN	cs_uint8				uart_id,
	CS_OUT	cs_uart_parity_t		*parity
)
{
	uart_config_t config;
	cs_status ret = CS_E_OK;

	ret = uart_cfg_get(uart_id, &config);

	*parity = config.parity;

	return ret;
}

cs_status cs_plat_uart_parity_set(
    CS_IN   cs_callback_context_t   context,
    CS_IN   cs_dev_id_t             device,
    CS_IN   cs_llid_t               llidport,
    CS_IN   cs_uint8                uart_id,
    CS_IN   cs_uart_parity_t        parity
)
{
    uart_config_t config;
    uart_cfg_mask_t mask;
    cs_status ret = CS_E_OK;

    if(uart_id >= MAX_UART_NUM)
        return CS_E_PARAM;

    mask.u32 = 0;
    mask.s.parity = 1;

    config.parity = parity;

    ret = uart_cfg_set(uart_id, mask, &config);

    return ret;
}


cs_status cs_plat_uart_duplex_get(
    CS_IN   cs_callback_context_t   context,
    CS_IN   cs_dev_id_t             device,
    CS_IN   cs_llid_t               llidport,
    CS_IN   cs_uint8                uart_id,
    CS_OUT  cs_uart_duplex_t        *duplex
)
{
    uart_config_t config;
    cs_status ret = CS_E_OK;

    ret = uart_cfg_get(uart_id, &config);

    *duplex = config.duplex;

    return ret;
}

cs_status cs_plat_uart_duplex_set(
    CS_IN   cs_callback_context_t   context,
    CS_IN   cs_dev_id_t             device,
    CS_IN   cs_llid_t               llidport,
    CS_IN   cs_uint8                uart_id,
    CS_IN   cs_uart_duplex_t        duplex
)
{
    uart_config_t config;
    uart_cfg_mask_t mask;
    cs_status ret = CS_E_OK;

    if(uart_id >= MAX_UART_NUM)
        return CS_E_PARAM;

    mask.u32 = 0;
    mask.s.duplex = 1;

    config.duplex = duplex;

    ret = uart_cfg_set(uart_id, mask, &config);

    return ret;
}


cs_status cs_plat_uart_baudrate_get(
    CS_IN   cs_callback_context_t   context,
    CS_IN   cs_dev_id_t             device,
    CS_IN   cs_llid_t               llidport,
    CS_IN   cs_uint8                uart_id,
    CS_OUT  cs_uint32               *baudrate
)
{
    uart_config_t config;
    cs_status ret = CS_E_OK;

    ret = uart_cfg_get(uart_id, &config);

    *baudrate = config.baud_rate;

    return ret;
}

cs_status cs_plat_uart_baudrate_set(
    CS_IN   cs_callback_context_t   context,
    CS_IN   cs_dev_id_t             device,
    CS_IN   cs_llid_t               llidport,
    CS_IN   cs_uint8                uart_id,
    CS_IN   cs_uint32               baudrate
)
{
    uart_config_t config;
    uart_cfg_mask_t mask;
    cs_status ret = CS_E_OK;

    if(uart_id >= MAX_UART_NUM)
        return CS_E_PARAM;

    mask.u32 = 0;
    mask.s.baud_rate = 1;

    config.baud_rate = baudrate;

    ret = uart_cfg_set(uart_id, mask, &config);

    return ret;
}

cs_status cs_plat_uart_enable(
    CS_IN   cs_callback_context_t   context,
    CS_IN   cs_dev_id_t             device,
    CS_IN   cs_llid_t               llidport,
    CS_IN   cs_uint8                uart_id
)
{
    uart_config_t config;
    uart_cfg_mask_t mask;
    cs_status ret = CS_E_OK;

    if(uart_id >= MAX_UART_NUM)
        return CS_E_PARAM;

    mask.u32 = 0;
    mask.s.enable = 1;

    config.enable = UART_ENABLE;

    uart_cfg_set(uart_id, mask, &config);
    
    ret = uart_ctrl_set(uart_id, UART_ENABLE);

    return ret;

}

cs_status cs_plat_uart_disable(
    CS_IN   cs_callback_context_t   context,
    CS_IN   cs_dev_id_t             device,
    CS_IN   cs_llid_t               llidport,
    CS_IN   cs_uint8                uart_id
)
{
    cs_status ret = CS_E_OK;

    ret = uart_ctrl_set(uart_id, UART_DISABLE);

    return ret;
}


cs_status cs_plat_uart_read(
    CS_IN       cs_callback_context_t    context,
    CS_IN       cs_dev_id_t              device,
    CS_IN       cs_llid_t                llidport,
    CS_IN       cs_uint8                 uart_id,
    CS_OUT      cs_uint8                 *data
)
{
    if(uart_id >= MAX_UART_NUM)
    return CS_E_PARAM;
	    if(uart_getc_nonblock(uart_id, data))
	        return CS_E_OK;
	    else
	        return CS_E_ERROR;
}


cs_status cs_plat_uart_write(
    CS_IN   cs_callback_context_t    context,
    CS_IN   cs_dev_id_t              device,
    CS_IN   cs_llid_t                llidport,
    CS_IN   cs_uint8                 uart_id,
    CS_IN   cs_uint32                len,
    CS_IN   cs_uint8                 *data
)
{
    cs_uint32 loop;
    cs_status ret = CS_E_OK;

    if(uart_id >= MAX_UART_NUM || 0 == len)
        return CS_E_PARAM;

    for(loop = 0; loop < len; loop++)
    {
        ret = uart_putc(uart_id, *(data + loop));
        if(ret != CS_E_OK)
        {
            ret = CS_E_ERROR;
            break;
        }
    }
    return ret;
}
#endif


