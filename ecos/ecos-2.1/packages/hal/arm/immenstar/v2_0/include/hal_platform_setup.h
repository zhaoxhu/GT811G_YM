#ifndef CYGONCE_HAL_PLATFORM_SETUP_H
#define CYGONCE_HAL_PLATFORM_SETUP_H

/*=============================================================================
//
//      hal_platform_setup.h
//
//      Platform specific support for HAL (assembly code)
//
//===========================================================================*/

#include <cyg/hal/hal_immenstar.h>


#if defined(IROSBOOT)

#define PLATFORM_PREAMBLE platform_preamble
    .macro platform_preamble
	.global   platform_preamble_start
	.type     platform_preamble_start, function
platform_preamble_start:
	  // vector table for ram checking
      ldr     pc,.ram_check_preamble
	  ldr     pc,.abort_preamble                  // 0x04
	  ldr     pc,.abort_preamble                  // 0x08 start && software int
	  ldr     pc,.abort_preamble                  // 0x0C
	  ldr     pc,.abort_preamble                  // 0x10
      ldr     pc,.abort_preamble                  // unused
	  ldr     pc,.abort_preamble                  // 0x18
	  ldr     pc,.abort_preamble                  // 0x1C
.ram_check_preamble:
        .word ram_check_preamble
.abort_preamble:
        .word abort_preamble

//UNMAPPED_PTR(ram_check_preamble)
//PTR(abort_preamble)
        .endm

#define PLATFORM_EXTRAS <cyg/hal/hal_platform_extras.h>

#endif /* IROSBOOT */


#ifdef CYG_HAL_STARTUP_ROMRAM
#define CYGSEM_HAL_ROM_RESET_USES_JUMP
#endif

// Define macro used to diddle the LEDs during early initialization.
// Can use r0+r1. Argument in \x.
// Control the LEDs PP0-PP3. This requires the jumpers on pins 9-16 to
// be set on LK11 in order to be visible. Otherwise the parallel port
// data pins are diddled instead.


#ifdef CYGHWR_HAL_ARM_IMST_DIAG_LEDS
#ifdef __thumb__
// vectors.S is compiled with -mthumb flag here LED is arm. it calls thumb version of
// hal_immenstar_led_show_num
#define CYGHWR_LED_MACRO                                   \
        .extern hal_immenstar_led_show_num                 ;\
        mov r0, #\x                                        ;\
        bl hal_immenstar_led_show_num                      ;
#else
#define CYGHWR_LED_MACRO				   \
        .extern hal_immenstar_led_show_num                 ;\
        mov r0, #\x                                        ;\
        bl hal_immenstar_led_show_num                      ;
#endif
#endif





#define PLATFORM_SETUP1 platform_setup1

// This macro represents the initial startup code for the platform
        .macro  platform_setup1


#if defined(CYG_HAL_ARM_IMST_OLT) && !defined(IROSBOOT) && defined(CYG_HAL_ARM_IMST_ASIC)
// POST for OLT
#define STACK_START_TEMP IMST_RAM_APP_START
        ldr sp, .stack_start_temp
        mov r11,#0
        mov r12,#0
        .extern post_stage2
        bl  post_stage2
        b   post_done
.stack_start_temp: .word STACK_START_TEMP
post_done:
#endif
#if 0
#if defined(CYG_HAL_ARM_IMST_ONU) && !defined(IROSBOOT) && defined(CYG_HAL_ARM_IMST_ASIC)
// POST for ONU
#define STACK_START_TEMP 0x10000  /* use internal sram for the temp stack */
        ldr sp, .stack_start_temp
        mov r11,#0
        mov r12,#0
        .extern post_stage2
        bl  post_stage2
        b   post_done
.stack_start_temp: .word STACK_START_TEMP
post_done:
#endif
#endif



	.endm

/*---------------------------------------------------------------------------*/
/* end of hal_platform_setup.h                                               */
#endif /* CYGONCE_HAL_PLATFORM_SETUP_H */

