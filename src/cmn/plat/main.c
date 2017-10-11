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
 * main.c
 *
 * $Id: main.c,v 1.22.2.55.2.1.2.25.2.28.2.2.4.41.4.7.6.4.2.8.4.1.2.2.2.5.2.4.2.13.2.20.4.1.2.32.2.1.4.2.4.4 2013/07/29 08:16:19 yding Exp $
 */

/****************************************************************

OVERVIEW

This file contains main process entry point

****************************************************************/
#include "iros_config.h"
#include <stdio.h>
#include <cyg/kernel/kapi.h>

#include <cyg/hal/hal_arch.h>
#include <pkgconf/hal.h>
#include <cyg/hal/hal_if.h>
#include <cyg/hal/hal_io.h>

#include <pkgconf/system.h>
#include <pkgconf/memalloc.h>
#include <pkgconf/isoinfra.h>
#include <stdlib.h>

#include "registers.h"
#include "plat_common.h"
#include "cli.h"
#include "mempool.h"
#include "interrupt.h"
#include "gpio.h"
#include "mdio.h"
#include "eth_init.h"
#include "i2c.h"
#ifdef HAVE_DMA
#include "dma.h"
#endif
#include "uart.h"
#include "boot_seq.h"
#include "cpu_load.h"
#include "app_gpio.h"
#ifdef HAVE_TERMINAL_SERVER
#include "terminal_server.h"
#endif
#ifdef HAVE_GWD_OAM
#include "app_oam_gwd_intf.h"
#endif
cs_uint32 sdl_thread_id = 0;
extern void cs_intr_thread(cyg_addrword_t data);
cs_uint32 appl_thread_id = 0;
extern void app_thread(cyg_addrword_t data);

cs_uint32 daemon_thread_id = 0;
void app_daemon_thread(cyg_addrword_t data);

#ifdef HAVE_VOIP
cs_uint32 pcm_thread_id = 0;
extern void onu_pcm_thread(cyg_addrword_t data);
cs_uint32 voip_thread_id = 0;
extern void app_voip_thread(cyg_addrword_t p);
#endif

extern void cyg_httpd_start(void);
extern void shell_init();
extern void telnetd_init();
extern void onu_interrupt_enable();
extern void onu_interrupt_init();
extern void app_global_init();
extern void app_task_init();
extern void onu_aal_global_init(void);

extern cs_status sdl_init(void);
extern cs_status sdl_service_up();

extern void plat_cmd_init();
extern void db_restore();

#if defined(HAVE_FLASH_FS)
extern int fs_mount(void);
#endif

extern void cs_gpio_init(void);

#ifdef HAVE_ONU_REGISTER_LAST
void oam_init_step2(void);
#endif
void plat_init(void)
{
    /* osal init */
    cs_osal_core_init();
    cs_mempool_init(CS_OSAL_MAX_MEMPOOL);
    cs_timer_init();
    
    /* flash driver init */
    ssp_init();
    flash_writer_setup();

#ifdef HAVE_EXCEPTION_HANDLER
    /* install customized exception handlers */
    install_exception_handlers();
#endif

    /* invalidate standby crc for roll-back */
    standby_blob_crc_invalidate();

#if defined(HAVE_FLASH_FS)
    fs_mount();
#endif

    /* log depends on file system */
    cs_log_init(CS_DFLT_LOG_NUM);

    /* read startup cfg from flash */
    startup_config_init();
    
    /* init ARM peripheral */
#ifdef HAVE_CTC_OAM    
    cs_polling_init();
#endif

    /* install interrupt handler */
    onu_interrupt_init();
    cs_gpio_init();
    mdio_init(4); //mdio speed 4Mhz
    i2c_init();
    
    /* register debug command */
    plat_cmd_init();
}

// user application start
#ifdef HAVE_POSIX
int main(int argc, char *argv[])
#else
void cyg_user_start(void)
#endif
{
    /* use second serial port */
    CYGACC_CALL_IF_SET_CONSOLE_COMM(0);

    cs_printf("Start to init system..., time %ld\n", cs_current_time());
    cs_init_seq_init();
    cs_reg_init_seq(INIT_STEP_SDL, INIT_STEP_GLB);
    cs_reg_init_seq(INIT_STEP_SERVICE,INIT_STEP_SDL);

    cs_init_seq_wait(INIT_STEP_GLB);

    plat_init();
    
    app_global_init();

    /* SDL init */
    sdl_init();

#ifdef HAVE_DMA
    /* DMA/ESDMA init */
    dma_engine_init();
    esdma_init();
#endif

    app_task_init();

    cs_init_seq_done(INIT_STEP_GLB);


    /* thread initialization */
    cs_thread_create(&sdl_thread_id, AAL_THREAD_NAME, cs_intr_thread, NULL, AAL_THREAD_STACKSIZE, AAL_THREAD_PRIORITY, 0);
    cs_thread_create(&appl_thread_id, APPL_THREAD_NAME, app_thread, NULL, APPL_THREAD_STACKSIZE, APPL_THREAD_PRIORITY, 0);
    cs_thread_create(&daemon_thread_id, DAEMON_THREAD_NAME, app_daemon_thread, NULL, DAEMON_THREAD_STACKSIZE, DAEMON_THREAD_PRIORITY, 0);
    
#ifdef HAVE_VOIP
    cs_thread_create(&pcm_thread_id, PCM_THREAD_NAME, onu_pcm_thread, NULL, PCM_THREAD_STACKSIZE, PCM_THREAD_PRIORITY, 0);
    cs_thread_create(&voip_thread_id, VOIP_THREAD_NAME, app_voip_thread, NULL, VOIP_THREAD_STACKSIZE, VOIP_THREAD_PRIORITY, 0);
#endif

#ifdef HAVE_UART_TEST
     uart_rx_thread_create();
#endif

    shell_init();

#ifdef HAVE_IP_STACK
    init_ip_service();
#endif

#ifdef HAVE_TERMINAL_SERVER
    /*  After IP stack */
    ts_init();
#endif

#ifdef HAVE_TELNET_CLI
    telnetd_init();
#endif

#ifdef HAVE_WEB_SERVER
    cyg_httpd_start();
#endif

    cs_init_seq_wait(INIT_STEP_SERVICE);

    /* enable all top interrupt */
    onu_interrupt_enable();

    sdl_service_up();
#ifdef HAVE_DB_MANAGMENT
    /* DB restore */
    db_restore();
#endif

    cs_init_seq_done(INIT_STEP_SERVICE);

	#ifdef HAVE_GWD_OAM
	gwd_onu_init();
	#endif

#ifdef HAVE_ONU_REGISTER_LAST
	/*oam_init_step2 should be execused at the end of init*/
	oam_init_step2();
#endif

    cs_printf("Init system done, time %ld\n",cs_current_time());

    cs_circle_timer_add(1000 , cs_cpuload_warning , NULL);
    cs_gpio_app_hook();
#ifdef HAVE_POSIX
    return 0;
#endif

}


