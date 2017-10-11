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
 * interrupt.c
 *
 * $Id: interrupt.c,v 1.1.2.5.6.1.4.1.2.1.12.7.2.7.2.1.2.16.2.1 2011/09/21 06:09:49 zsun Exp $
 */
/****************************************************************
OVERVIEW

This file use to handle interrupt relate operator

****************************************************************/

#include "iros_config.h"
#include "plat_common.h"
#include "interrupt.h"
#include "dma.h"
#include "boot_seq.h"
#ifdef HAVE_TERMINAL_SERVER
#include "terminal_server.h"
#endif

cyg_uint32 onu_int_dma0_tx      = 0;
cyg_uint32 onu_int_esdma_auto_xfer  = 0;
cyg_uint32 onu_int_esdma_xfer2mem   = 0;
cyg_uint32 onu_int_esdma_pkt_xfer   = 0;

cyg_flag_t onu_intr_flag;
#ifdef HAVE_VOIP
cyg_flag_t onu_pcm_flag;
#endif

cs_hw_timer_t hw_timer[CS_MAX_HW_TIMER];

void onu_hw_timer_init()
{
    memset(&hw_timer[0] , 0 , sizeof(cs_hw_timer_t)*CS_MAX_HW_TIMER);
    hw_timer[0].free = 0;

    hw_timer[1].ctrl = IROS_PER_TMR_CTRL2;
    hw_timer[1].ld   = IROS_PER_TMR_LD2;
    hw_timer[1].ie   = IROS_PER_TMR_IE2;
    hw_timer[1].vector = ONU_INT_FLAGBIT_TIMER2;
    hw_timer[1].free = 1;

    hw_timer[2].ctrl = IROS_PER_TMR_CTRL3;
    hw_timer[2].ld   = IROS_PER_TMR_LD3;
    hw_timer[2].ie   = IROS_PER_TMR_IE3;
    hw_timer[2].vector = ONU_INT_FLAGBIT_TIMER3;
    hw_timer[2].free = 1;

    hw_timer[3].ctrl = IROS_PER_TMR_CTRL4;
    hw_timer[3].ld   = IROS_PER_TMR_LD4;
    hw_timer[3].ie   = IROS_PER_TMR_IE4;
    hw_timer[3].vector = ONU_INT_FLAGBIT_TIMER4;
    hw_timer[3].free = 1;

    hw_timer[4].ctrl = IROS_PER_TMR_CTRL5;
    hw_timer[4].ld   = IROS_PER_TMR_LD5;
    hw_timer[4].ie   = IROS_PER_TMR_IE5;
    hw_timer[4].vector = ONU_INT_FLAGBIT_TIMER5;
    hw_timer[4].free = 1;
}

cs_int32 cs_hw_timer_add(
    cs_int32 interval ,
    void (*callback)(void *data),
    void *data
)
{
    int i;

    if (callback == NULL)
        return CS_E_ERROR;

    for (i = 0 ; i < CS_MAX_HW_TIMER ; i++) {
        if (hw_timer[i].free == 1)
            break;
    }

    if (i >= CS_MAX_HW_TIMER)
        return CS_E_ERROR;

    hw_timer[i].free = 0;
    hw_timer[i].callback = callback;
    hw_timer[i].data = data;
    hw_timer[i].type = 0;
    interval *= 10;
    HAL_WRITE_UINT32(hw_timer[i].ctrl , 0x0);
    HAL_WRITE_UINT32(hw_timer[i].ld, interval);
    HAL_WRITE_UINT32(hw_timer[i].ctrl, 0xc4);
    HAL_WRITE_UINT32(hw_timer[i].ie, 0x1);
    cs_intr_unlock(hw_timer[i].vector);
    return i;
}


cs_int32 cs_hw_circle_timer_add(
    cs_int32 interval ,
    void (*callback)(void *data),
    void *data
)
{
    int i;

    if (callback == NULL)
        return CS_E_ERROR;

    for (i = 0 ; i < CS_MAX_HW_TIMER ; i++) {
        if (hw_timer[i].free == 1)
            break;
    }

    if (i >= CS_MAX_HW_TIMER)
        return CS_E_ERROR;

    hw_timer[i].free = 0;
    hw_timer[i].callback = callback;
    hw_timer[i].data = data;
    hw_timer[i].type = 1;
    interval *= 10;
    HAL_WRITE_UINT32(hw_timer[i].ctrl , 0x0);
    HAL_WRITE_UINT32(hw_timer[i].ld, interval);
    HAL_WRITE_UINT32(hw_timer[i].ctrl, 0xc4);
    HAL_WRITE_UINT32(hw_timer[i].ie, 0x1);
    cs_intr_unlock(hw_timer[i].vector);
    return i;
}


void onu_timer_int_mask(cs_int32 timer_id)
{
    if (timer_id < 0 || timer_id >= CS_MAX_HW_TIMER)
        return;

    cs_intr_lock(hw_timer[timer_id].vector);
}

void onu_timer_int_unmask(cs_int32 timer_id)
{
    if (timer_id < 0 || timer_id >= CS_MAX_HW_TIMER)
        return;

    cs_intr_unlock(hw_timer[timer_id].vector);
}


cs_intr_t intr_list[CS_MAX_INTR_NUM];
cs_uint32 cs_intr_isr_handler(cyg_vector_t vector, cyg_addrword_t data)
{
    if (vector >= CS_MAX_INTR_NUM)
        return CYG_ISR_HANDLED;

    intr_list[vector].count++;
    cyg_interrupt_mask(vector);
    cyg_interrupt_acknowledge(vector);

    return(CYG_ISR_HANDLED | CYG_ISR_CALL_DSR);
}

void cs_intr_dsr_handler(cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data)
{
    int i;
    cs_intr_cb_t *pIntrCb = NULL;

    if (vector >= CS_MAX_INTR_NUM)
        return;

    for (i = 0 ; i < CS_MAX_INTR_CB_NUM ; i++) {
        pIntrCb = (cs_intr_cb_t *) & intr_list[vector].intr_cb[i];
        if(!pIntrCb->used)
        {
            break;
        }
        
        if (pIntrCb->depth == INTR_DSR_CB && pIntrCb->callback)
            pIntrCb->callback(vector, pIntrCb->data);
    }

    if (!intr_list[vector].thread_cb_flag) {
        cs_intr_unlock(vector);
        return;
    }

    for (i = 0 ; i < CS_MAX_INTR_CB_NUM ; i++) {
        pIntrCb = (cs_intr_cb_t *) & intr_list[vector].intr_cb[i];
        if (pIntrCb->depth == INTR_THREAD_CB && pIntrCb->callback) {
            cs_uint32 flag = 0;

            flag = 1 << vector;
            cyg_flag_setbits(&onu_intr_flag, flag);
            return;
        }
    }
}

cs_int32 cs_intr_reg
(
    cs_int32 vector ,
    const cs_int8 *irq_name,
    cs_int32(*handle)(cs_int32 , void *),
    void *data,
    cs_int32 depth /*INTR_DSR_CB:0   INTR_THREAD_CB:1*/
)
{
    cs_int32 i;
    cs_intr_cb_t *pIntrCb = NULL;

    if (vector >= CS_MAX_INTR_NUM)
        return CS_E_ERROR;

    if (!intr_list[vector].attached) {
        memset(&intr_list[vector], 0 , sizeof(cs_intr_t));
        cyg_interrupt_create((cyg_vector_t)vector,
                             ONU_INT_PRIORITY1,
                             0,
                             &cs_intr_isr_handler,
                             &cs_intr_dsr_handler,
                             &intr_list[vector].handle,
                             &intr_list[vector].intr);
        cyg_interrupt_attach(intr_list[vector].handle);
    }
    intr_list[vector].attached = 1;

    if (depth != INTR_DSR_CB && depth != INTR_THREAD_CB)
        depth = INTR_THREAD_CB;

    for (i = 0 ; i < CS_MAX_INTR_CB_NUM ; i++) {
        pIntrCb = (cs_intr_cb_t *) & intr_list[vector].intr_cb[i];
        if (!pIntrCb->used)
            break;

        if (pIntrCb->callback == handle && pIntrCb->depth == depth)
            return CS_E_OK;

        pIntrCb = NULL;
    }

    if (pIntrCb == NULL)
        return CS_E_ERROR;

    pIntrCb->callback = handle;
    pIntrCb->data = data;
    pIntrCb->depth = depth;
    pIntrCb->used = 1;
    if (depth == INTR_THREAD_CB)
        intr_list[vector].thread_cb_flag = 1;

    if (irq_name) {
        int len = 0;

        len = (strlen(irq_name) >= CS_MAX_INTR_NAME_LEN - 1) ? CS_MAX_INTR_NAME_LEN - 1 : strlen(irq_name);
        memcpy(pIntrCb->name , irq_name , strlen(irq_name));
    }
    return CS_E_OK;
}

void cs_intr_lock(cs_int32 vector)
{
    cyg_interrupt_mask(vector);
    return;
}

void cs_intr_unlock(cs_int32 vector)
{
    cyg_interrupt_unmask(vector);
    return;
}

cs_uint32 cs_intr_count_get(cs_int32 vector)
{
    if (vector >= CS_MAX_INTR_NUM)
        return 0;

    return intr_list[vector].count;
}

extern void sdl_bm_int_process();
extern void sdl_fe_int_process();
extern void sdl_uni_int_process();
extern void sdl_pon_int_process();
extern void ssp_intr_hanlder();
extern void sdl_ma_int_process();
static cs_int32 cs_intr_handler_bm(cs_int32 vector , void *data)
{
    sdl_bm_int_process();
    return CS_E_OK;
}

static cs_int32 cs_intr_handler_fe(cs_int32 vector , void *data)
{
    sdl_fe_int_process();
    return CS_E_OK;
}

static cs_int32 cs_intr_handler_ni(cs_int32 vector , void *data)
{
    sdl_uni_int_process();
    return CS_E_OK;
}

static cs_int32 cs_intr_handler_pon(cs_int32 vector , void *data)
{
    sdl_pon_int_process();
    return CS_E_OK;
}

#if 0
static cs_int32 cs_ssp_intr_handler(cs_int32 vector , void *data)
{
    ssp_intr_hanlder();
    return CS_E_OK;
}
#endif

#if 0
static cs_int32 cs_gpio_intr_handler(cs_int32 vector, void *data)
{
    cs_uint32 int_status;
    cs_uint32 isr;
    cs_uint32 evt;
    cs_uint32 gpio_id = 0;

    CS_REG_READ(ONU_GPIO_INT_STATUS, int_status);

    isr = g_gpio_dev.isr_map & int_status;
    while (isr && (gpio_id < ONU_GPIO_MAX_NUM)) {
        if (isr & (1 << gpio_id)) {
            g_gpio_dev.cfg[gpio_id].info.trigger.isr(gpio_id);
            isr &= ~(1 << gpio_id);
        }

        gpio_id++;
    }

    evt = g_gpio_dev.evt_map & int_status;
#if 0
    if (evt) {
        cyg_flag_setbits(&onu_intr_flag, 1 << vector);
    } else {
        CS_REG_WRITE(ONU_GPIO_INT_STATUS, int_status);
        cs_intr_unlock(vector);
    }
#else
    if (evt == 0)
        CS_REG_WRITE(ONU_GPIO_INT_STATUS, int_status);
#endif

    return CS_E_OK;
}
#endif

#ifdef HAVE_VOIP
static cs_int32 cs_pcm_intr_handler(cs_int32 vector, void *data)
{
    cs_uint32 flag = 0;

    flag = 1 << vector;
    cyg_flag_setbits(&onu_pcm_flag, flag);
    return CS_E_OK;
}
#endif

static cs_int32 cs_ma_intr_handler_dsr(cs_int32 vector, void *data)
{
#ifdef HAVE_DMA
    esdma_intr_hanlder();
    dma_intr_hanlder();
#endif
    return CS_E_OK;
}

static cs_int32 cs_ma_intr_handler_thread(cs_int32 vector, void *data)
{
    sdl_ma_int_process();
    return CS_E_OK;
}

static cs_int32 cs_hwtimer_intr_handler(cs_int32 vector , void *data)
{
    int i;

    for (i = 0 ; i < CS_MAX_HW_TIMER ; i++) {
        if (hw_timer[i].free == 0 && hw_timer[i].vector == vector)
            break;
    }

    if (i >= CS_MAX_HW_TIMER) {
        cs_intr_lock(vector);
        return CS_E_OK;
    }

    if (hw_timer[i].type == 0) {
        HAL_WRITE_UINT32(hw_timer[i].ctrl , 0x0);
    }

    if (hw_timer[i].callback == NULL) {
        cs_intr_lock(vector);
        return CS_E_OK;
    }

    if (hw_timer[i].type == 1) {
        hw_timer[i].callback(hw_timer[i].data);
        cs_intr_unlock(vector);
    } else {
        void (*callback)(void *data) = NULL;
        void *data = NULL;

        callback = hw_timer[i].callback;
        data = hw_timer[i].data;
        hw_timer[i].callback = NULL;
        hw_timer[i].data = NULL;
        hw_timer[i].free = 1;
        hw_timer[i].type = 0;
        cs_intr_lock(vector);
        callback(data);
    }
    return CS_E_OK;
}

cs_int32 hw_cpuload_timer_id = -1;
void onu_interrupt_init()
{
    int i;

    onu_hw_timer_init();
    for (i = 0 ; i < CS_MAX_INTR_NUM ; i++) {
        memset(&intr_list[i], 0, sizeof(cs_intr_t));
    }

    /*cs_intr_reg(ONU_INT_FLAGBIT_SSP   , "ssp_intr" , cs_ssp_intr_handler    , NULL , INTR_DSR_CB);*/
    /*cs_intr_reg(ONU_INT_FLAGBIT_GPIO  , "gpio_intr", cs_gpio_intr_handler   , NULL , INTR_DSR_CB);*/
#ifdef HAVE_VOIP
    cs_intr_reg(ONU_INT_FLAGBIT_PCM   , "pcm_intr" , cs_pcm_intr_handler    , NULL , INTR_DSR_CB);
#endif
    cs_intr_reg(ONU_INT_FLAGBIT_MA    , "ma_intr"  , cs_ma_intr_handler_dsr , NULL , INTR_DSR_CB);
    cs_intr_reg(ONU_INT_FLAGBIT_MA    , "ma_intr"  , cs_ma_intr_handler_thread, NULL, INTR_THREAD_CB);

    cs_intr_reg(ONU_INT_FLAGBIT_BM    , "bm_intr"  , cs_intr_handler_bm     , NULL, INTR_THREAD_CB);
    cs_intr_reg(ONU_INT_FLAGBIT_FE    , "fe_intr"  , cs_intr_handler_fe     , NULL, INTR_THREAD_CB);
    cs_intr_reg(ONU_INT_FLAGBIT_NI    , "ni_intr"  , cs_intr_handler_ni     , NULL, INTR_THREAD_CB);
    cs_intr_reg(ONU_INT_FLAGBIT_EPON  , "epon_intr", cs_intr_handler_pon    , NULL, INTR_THREAD_CB);
    cs_intr_reg(ONU_INT_FLAGBIT_TIMER2, "TMR2_intr", cs_hwtimer_intr_handler, NULL, INTR_DSR_CB);
    cs_intr_reg(ONU_INT_FLAGBIT_TIMER3, "TMR3_intr", cs_hwtimer_intr_handler, NULL, INTR_DSR_CB);
    cs_intr_reg(ONU_INT_FLAGBIT_TIMER4, "TMR4_intr", cs_hwtimer_intr_handler, NULL, INTR_DSR_CB);
    cs_intr_reg(ONU_INT_FLAGBIT_TIMER5, "TMR5_intr", cs_hwtimer_intr_handler, NULL, INTR_DSR_CB);
#ifdef HAVE_TERMINAL_SERVER
    cs_intr_reg(ONU_INT_FLAGBIT_UART1, "UART1_intr", ts_uart_intr_handler, NULL, INTR_DSR_CB);
    cs_intr_reg(ONU_INT_FLAGBIT_UART2, "UART2_intr", ts_uart_intr_handler, NULL, INTR_DSR_CB);
    cs_intr_reg(ONU_INT_FLAGBIT_UART3, "UART3_intr", ts_uart_intr_handler, NULL, INTR_DSR_CB);
    cs_intr_reg(ONU_INT_FLAGBIT_UART4, "UART4_intr", ts_uart_intr_handler, NULL, INTR_DSR_CB);
#endif
#ifdef HAVE_CPULOAD_PER_THREAD
    hw_cpuload_timer_id = cs_hw_circle_timer_add(1000, cs_cpuload_sampling, NULL);
#endif
}

void onu_interrupt_enable()
{
    int i;

    for (i = 0 ; i < CS_MAX_INTR_NUM ; i++) {
        if (intr_list[i].attached)
            cs_intr_unlock(i);
    }
}

cs_uint32 intr_thread_count = 0;
void cs_intr_thread(cs_uint32 data)
{
    cs_uint32 i, j;
    cyg_flag_value_t flag;

    cs_init_seq_wait(INIT_STEP_SDL);
    cs_init_seq_done(INIT_STEP_SDL);

    while (1) {
        flag = cyg_flag_wait(&onu_intr_flag, 0xffffffff, CYG_FLAG_WAITMODE_OR | CYG_FLAG_WAITMODE_CLR);

        if (flag == 0)
            continue;

        for (i = 0; i < 32; i++) {

            if (!BITON(flag, i))
                continue;

            intr_thread_count++;

            if (!intr_list[i].thread_cb_flag) {
                cs_intr_unlock(i);
                continue;
            }

            for (j = 0 ; j < CS_MAX_INTR_CB_NUM ; j++) {
                if (intr_list[i].intr_cb[j].depth == INTR_THREAD_CB &&
                        intr_list[i].intr_cb[j].callback) {
                    intr_list[i].intr_cb[j].callback(i , intr_list[i].intr_cb[j].data);
                }
            }
            cs_intr_unlock(i);
        } // for ()
    } // while(1)
}


