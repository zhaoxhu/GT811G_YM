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
 * interrupt.h
 *
 * $Id: interrupt.h,v 1.1.4.2.6.1.6.5.2.3.4.1.2.6 2011/05/18 09:13:38 zsun Exp $
 */
/****************************************************************

OVERVIEW

This file contains interrupt var/handle/vectors

****************************************************************/


#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <cyg/hal/hal_platform_ints.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_immenstar.h>
#include <cyg/kernel/kapi.h>


/* interrupt priority */
#define ONU_INT_PRIORITY1                  1
#define ONU_INT_PRIORITY0                  0

/* bit definition of ONU interrupt source register */
#define ONU_INT_FLAGBIT_SOFT            CYGNUM_HAL_INTERRUPT_SOFT
#define ONU_INT_FLAGBIT_TIMER1          CYGNUM_HAL_INTERRUPT_TIMER1
#define ONU_INT_FLAGBIT_TIMER2          CYGNUM_HAL_INTERRUPT_TIMER2
#define ONU_INT_FLAGBIT_TIMER3          CYGNUM_HAL_INTERRUPT_TIMER3
#define ONU_INT_FLAGBIT_TIMER4          CYGNUM_HAL_INTERRUPT_TIMER4
#define ONU_INT_FLAGBIT_TIMER5          CYGNUM_HAL_INTERRUPT_TIMER5
#define ONU_INT_FLAGBIT_UART0           CYGNUM_HAL_INTERRUPT_UART0
#define ONU_INT_FLAGBIT_UART1           CYGNUM_HAL_INTERRUPT_UART1
#define ONU_INT_FLAGBIT_SSP             CYGNUM_HAL_INTERRUPT_SSP
#define ONU_INT_FLAGBIT_GPIO            CYGNUM_HAL_INTERRUPT_GPIO
#define ONU_INT_FLAGBIT_I2C             CYGNUM_HAL_INTERRUPT_I2C
#define ONU_INT_FLAGBIT_WD              CYGNUM_HAL_INTERRUPT_WDT
#define ONU_INT_FLAGBIT_MDIO            CYGNUM_HAL_INTERRUPT_MDIO
#define ONU_INT_FLAGBIT_AHB             CYGNUM_HAL_INTERRUPT_AHB
#define ONU_INT_FLAGBIT_PCM             CYGNUM_HAL_INTERRUPT_PCM
#define ONU_INT_FLAGBIT_DMA1            CYGNUM_HAL_INTERRUPT_DMA1
#define ONU_INT_FLAGBIT_DMA0            CYGNUM_HAL_INTERRUPT_DMA0
#define ONU_INT_FLAGBIT_RESERVED17      CYGNUM_HAL_INTERRUPT_RESERVED17
#define ONU_INT_FLAGBIT_RESERVED18      CYGNUM_HAL_INTERRUPT_RESERVED18
#define ONU_INT_FLAGBIT_RESERVED19      CYGNUM_HAL_INTERRUPT_RESERVED19
#define ONU_INT_FLAGBIT_GLB             CYGNUM_HAL_INTERRUPT_GLB
#define ONU_INT_FLAGBIT_MA              CYGNUM_HAL_INTERRUPT_MA
#define ONU_INT_FLAGBIT_RESERVED22      CYGNUM_HAL_INTERRUPT_RESERVED22
#define ONU_INT_FLAGBIT_RESERVED23      CYGNUM_HAL_INTERRUPT_RESERVED23
#define ONU_INT_FLAGBIT_BM              CYGNUM_HAL_INTERRUPT_BM
#define ONU_INT_FLAGBIT_FE              CYGNUM_HAL_INTERRUPT_FE
#define ONU_INT_FLAGBIT_RESERVED26      CYGNUM_HAL_INTERRUPT_RESERVED26
#define ONU_INT_FLAGBIT_NI              CYGNUM_HAL_INTERRUPT_NI
#define ONU_INT_FLAGBIT_EPON            CYGNUM_HAL_INTERRUPT_EPON
#define ONU_INT_FLAGBIT_UART2           CYGNUM_HAL_INTERRUPT_UART2
#define ONU_INT_FLAGBIT_UART3           CYGNUM_HAL_INTERRUPT_UART3
#define ONU_INT_FLAGBIT_UART4           CYGNUM_HAL_INTERRUPT_UART4

// flag definition corresponding to interrupt vector
#define ONU_INT_FLAG_SOFT               (1<<ONU_INT_FLAGBIT_SOFT)
#define ONU_INT_FLAG_TIMER1             (1<<ONU_INT_FLAGBIT_TIMER1)
#define ONU_INT_FLAG_TIMER2             (1<<ONU_INT_FLAGBIT_TIMER2)
#define ONU_INT_FLAG_TIMER3             (1<<ONU_INT_FLAGBIT_TIMER3)
#define ONU_INT_FLAG_TIMER4             (1<<ONU_INT_FLAGBIT_TIMER4)
#define ONU_INT_FLAG_TIMER5             (1<<ONU_INT_FLAGBIT_TIMER5)
#define ONU_INT_FLAG_UART0              (1<<ONU_INT_FLAGBIT_UART0)
#define ONU_INT_FLAG_UART1              (1<<ONU_INT_FLAGBIT_UART1)
#define ONU_INT_FLAG_SSP                (1<<ONU_INT_FLAGBIT_SSP)
#define ONU_INT_FLAG_GPIO               (1<<ONU_INT_FLAGBIT_GPIO)
#define ONU_INT_FLAG_I2C                (1<<ONU_INT_FLAGBIT_I2C)
#define ONU_INT_FLAG_WD                 (1<<ONU_INT_FLAGBIT_WD)
#define ONU_INT_FLAG_MDIO               (1<<ONU_INT_FLAGBIT_MDIO)
#define ONU_INT_FLAG_AHB                (1<<ONU_INT_FLAGBIT_AHB)
#define ONU_INT_FLAG_PCM                (1<<ONU_INT_FLAGBIT_PCM)
#define ONU_INT_FLAG_DMA1               (1<<ONU_INT_FLAGBIT_DMA1)
#define ONU_INT_FLAG_DMA0               (1<<ONU_INT_FLAGBIT_DMA0)
#define ONU_INT_FLAG_RESERVED17         (1<<ONU_INT_FLAGBIT_RESERVED17)
#define ONU_INT_FLAG_RESERVED18         (1<<ONU_INT_FLAGBIT_RESERVED18)
#define ONU_INT_FLAG_RESERVED19         (1<<ONU_INT_FLAGBIT_RESERVED19)
#define ONU_INT_FLAG_GLB                (1<<ONU_INT_FLAGBIT_GLB)
#define ONU_INT_FLAG_MA                 (1<<ONU_INT_FLAGBIT_MA)
#define ONU_INT_FLAG_RESERVED22         (1<<ONU_INT_FLAGBIT_RESERVED22)
#define ONU_INT_FLAG_RESERVED23         (1<<ONU_INT_FLAGBIT_RESERVED23)
#define ONU_INT_FLAG_BM                 (1<<ONU_INT_FLAGBIT_BM)
#define ONU_INT_FLAG_FE                 (1<<ONU_INT_FLAGBIT_FE)
#define ONU_INT_FLAG_RESERVED26         (1<<ONU_INT_FLAGBIT_RESERVED26)
#define ONU_INT_FLAG_NI                 (1<<ONU_INT_FLAGBIT_NI)
#define ONU_INT_FLAG_EPON               (1<<ONU_INT_FLAGBIT_EPON)
#define ONU_INT_FLAG_UART2              (1<<ONU_INT_FLAGBIT_UART2)
#define ONU_INT_FLAG_UART3              (1<<ONU_INT_FLAGBIT_UART3)
#define ONU_INT_FLAG_UART4              (1<<ONU_INT_FLAGBIT_UART4)

/* Data Present Mask */
#define PRESENT_MASK(b)               (0x00000001<<(b))
#define BITON(m, b)                   (((m) & PRESENT_MASK(b)) != 0)
#define SETBIT(m, b)                  ((m) |= PRESENT_MASK(b))
#define CLRBIT(m, b)                  ((m) &= ~PRESENT_MASK(b))

#define INTR_DSR_CB     0
#define INTR_THREAD_CB  1

extern cyg_flag_t onu_intr_flag;
#ifdef HAVE_VOIP
extern cyg_flag_t onu_pcm_flag;
#endif
extern cyg_uint32 onu_int_dma0_tx;
extern cyg_uint32 onu_int_esdma_auto_xfer;
extern cyg_uint32 onu_int_esdma_xfer2mem;
extern cyg_uint32 onu_int_esdma_pkt_xfer;

extern cs_int32 hw_cpuload_timer_id;

typedef struct
{
    cs_int16 free;
    cs_int16 type; /*CIRCLE:1  ONCE:0*/
    void (*callback)(void *data);
    void *data;
    cs_uint32 ctrl;
    cs_uint32 ld;
    cs_uint32 ie;
    cs_uint32 vector;
}cs_hw_timer_t;

#define CS_MAX_HW_TIMER 5

extern void onu_timer_int_mask(cs_int32 timer_id);
extern void onu_timer_int_unmask(cs_int32 timer_id);
extern cs_int32 cs_hw_timer_add(
    cs_int32 interval ,
    void (*callback)(void *data),
    void *data
    );
extern cs_int32 cs_hw_circle_timer_add(
    cs_int32 interval ,
    void (*callback)(void *data),
    void *data
    );
extern cs_int32 cs_hw_circle_timer_del(cs_int32 id);

#define CS_MAX_INTR_NAME_LEN 32
#define CS_MAX_INTR_CB_NUM   4
#define CS_MAX_INTR_NUM      CYGNUM_HAL_ISR_COUNT

typedef struct {
    cs_int8  name[CS_MAX_INTR_NAME_LEN];
    cs_int32 (*callback)(cs_int32 , void *);
    void     *data;
    cs_int32 depth;
    cs_int32 used;
}cs_intr_cb_t;

typedef struct {
    cs_int32        attached;
    cyg_handle_t    handle;
    cyg_interrupt   intr;
    cs_intr_cb_t    intr_cb[CS_MAX_INTR_CB_NUM];
    cs_uint32       count;
    cs_int32        thread_cb_flag; /*At least one callback need to be executed in thread context*/
}cs_intr_t;

extern cs_int32 cs_intr_reg
    (
    cs_int32 vector ,
    const cs_int8 *irq_name,
    cs_int32 (*handle)(cs_int32 , void *),
    void *data,
    cs_int32 depth
    );
extern void cs_intr_lock(cs_int32 vector);
extern void cs_intr_unlock(cs_int32 vector);
extern cs_uint32 cs_intr_count_get(cs_int32 vector);
extern void onu_interrupt_init();
extern void onu_interrupt_enable();

#endif

