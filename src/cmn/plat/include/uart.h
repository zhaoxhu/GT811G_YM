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
 * startup_config.h
 *
 * $Id: uart.h,v 1.1.2.4.2.1.6.3 2011/06/18 09:26:02 ljin Exp $
 */

#ifndef _UART_H_
#define _UART_H_

#define MAX_UART_NUM            5
#define BAUD_COUNT_MASK         0xFFFF  /* baud_count only contain 16bits  */
#define FINE_TUNE_MASK          0xFF    /* fine_tune only contain 8bits  */

typedef enum {
    SDL_UART_FULL_DUPLEX   = 0,
    SDL_UART_HALF_DUPLEX   = 1
} cs_sdl_uart_duplex_t;

typedef enum {
    SDL_UART_PARITY_NONE   = 0,
    SDL_UART_PARITY_ODD   = 1,
    SDL_UART_PARITY_EVEN   = 2
} cs_sdl_uart_parity_t;

typedef enum {
    SDL_UART_MODE_INTR   = 0,
    SDL_UART_MODE_POLL   = 1
} cs_sdl_uart_mode_t;

typedef enum {
    SDL_UART_DISABLE   = 0,
    SDL_UART_ENABLE    = 1
} cs_sdl_uart_ctrl_t;

typedef struct{      
    unsigned char           uart_id;      /* UART number 0,1,2,3,4*/
    unsigned char           data_bits;    /* 7/8  */
    unsigned char           stop_bits;    /* 1/2 */
    unsigned char           flow_control; /* None/XONXOFF/HardWare */
    cs_sdl_uart_parity_t    parity;       /* None/Odd/Even */
    cs_sdl_uart_duplex_t    duplex;       /* full-duplex or half-duplex*/
    cs_sdl_uart_mode_t      mode;         /* Polling or Interrupt */
    int                     baud_rate;    /*300/600/1200/2400/4800/9600/19200/38400/57600/115200*/
	#ifdef HAVE_TERMINAL_SERVER
	//added by zhuxh
	cs_uint8            	enable;       /* enable/disable */
	#endif
} uart_config_t;

typedef struct {
    cs_uint32 base_address;
    int isr_vector;
} uart_channel_t;

#ifdef HAVE_TERMINAL_SERVER
//added by zhuxh
typedef enum {
    UART_DISABLE        = 0,
    UART_ENABLE         = 1
} uart_ctrl_t;

typedef enum {
    UART_POLLING_MODE   = 0,
    UART_INTR_MODE      = 1
} uart_mode_t;

typedef enum {
    UART_FULL_DUPLEX    = 0,
    UART_HALF_DUPLEX    = 1
} uart_duplex_t;

typedef enum {
    UART_NONE_PARITY    = 0,
    UART_ODD_PARITY     = 1,
    UART_EVEN_PARITY    = 2
} uart_parity_t;

typedef enum {
    UART_BAUD_RATE_300 = 300,
    UART_BAUD_RATE_600 = 600,
    UART_BAUD_RATE_1200 = 1200,
    UART_BAUD_RATE_2400 = 2400,
    UART_BAUD_RATE_4800 = 4800,
    UART_BAUD_RATE_9600 = 9600,
    UART_BAUD_RATE_19200 = 19200,
    UART_BAUD_RATE_38400 = 38400,
    UART_BAUD_RATE_57600 = 57600,
    UART_BAUD_RATE_115200 = 115200,
    UART_BAUD_RATE_MAX = 230400
} uart_baud_rate_t;


typedef union {
    struct {
    cs_uint32  mode             : 1;
    cs_uint32  data_bits        : 1;
    cs_uint32  stop_bits        : 1;
    cs_uint32  flow_control     : 1;
    cs_uint32  parity           : 1;
    cs_uint32  duplex           : 1;
    cs_uint32  baud_rate        : 1;
    cs_uint32  enable           : 1;
    } s;
    cs_uint32 u32;
} uart_cfg_mask_t;

typedef cs_int32 (*uart_intr_handler_t)(cs_uint8,void *);

cs_status uart_cfg_set(cs_uint8 uart_id, uart_cfg_mask_t mask, uart_config_t *uart_cfg);
#endif

extern void uart_intr_hanlder(cs_uint8 uart_num);
extern void uart_init(uart_config_t *uart_cfg);
extern void uart_electricity_meter_test(cs_uint8 uart_id);
extern void uart_rx_thread_create(void);
extern cs_status uart_int_status_clear(cs_uint8 uart_id);
extern cs_status uart_cfg_get(cs_uint8 uart_id, uart_config_t *config);
extern cs_status uart_ctrl_set(cs_uint8 uart_id,cs_sdl_uart_ctrl_t ctrl);
extern cs_status uart_ctrl_get(cs_uint8 uart_id,cs_sdl_uart_ctrl_t *ctrl);
extern cs_boolean uart_getc_nonblock(cs_uint8 uart_id,cs_uint8* ch);
extern cs_boolean uart_putc(cs_uint8 uart_id,char ch);

#endif

