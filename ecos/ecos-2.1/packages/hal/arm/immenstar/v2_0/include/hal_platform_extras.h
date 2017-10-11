#ifndef CYGONCE_HAL_PLATFORM_EXTRAS_H
#define CYGONCE_HAL_PLATFORM_EXTRAS_H

/*=============================================================================
//
//      hal_platform_extras.h
//
//      Platform specific support for HAL (assembly code)
//
//=============================================================================
*/

#include <cyg/hal/hal_immenstar.h>

// immenstar onu_e loader
#if defined(IROSBOOT)
        .text
        .code   32
	.global ram_check_preamble
	.type   ram_check_preamble, function
ram_check_preamble:

        mov     r0,#(CPSR_IRQ_DISABLE|CPSR_FIQ_DISABLE|CPSR_SUPERVISOR_MODE)
        msr     cpsr,r0
/*
        ldr     r2, .imst_image_download_ctrl
ice_loop:
        ldr     r0, [r2]
        mov     r4, #IMAGE_DOWNLOAD_BITS
        mov     r5, #IMAGE_FROM_ANY
        and     r3, r0, r4
        cmp     r3, r5
        bne     1000f
        b       ice_loop
.imst_image_download_ctrl:
        .word IMST_IMAGE_DOWNLOAD
1000:
*/

        //GPIO 0-3 set to high
        ldr     r1, .imst_gpio_out_reg
        mov     r0, #0xf
        str     r0, [r1]


        // configure uart (tun and rx sample)
        ldr     r1, .imst_uart_tun_reg
        ldr     r0, .imst_uart_tun_val
        str     r0, [r1]
        ldr     r1, .imst_uart_rx_sample
        ldr     r0, .imst_uart_rx_sample_val
        str     r0, [r1]

        // set UART
        ldr     r1, .imst_uart_config_reg
        ldr     r0, .imst_uart_config_val
        str     r0, [r1]

        // output "boot"
        ldr     r1, .imst_uart_out_reg
        ldr     r0, .imst_uart_out_valb
        str     r0, [r1]
        ldr     r0, .imst_uart_out_valo
        str     r0, [r1]
        ldr     r0, .imst_uart_out_valo
        str     r0, [r1]
        ldr     r0, .imst_uart_out_valt
        str     r0, [r1]


        //GPIO 0-3 set to high
        ldr     r1, .imst_gpio_out_reg
        mov     r0, #0xf
        str     r0, [r1]


// IMAGE_DOWNLOAD is not 0b10

//#define MIN_MEM_CHECK_BY_C
#if defined(MIN_MEM_CHECK_BY_C)
        .extern min_mem_check
        bl    min_mem_check
        cmp   r0, #0
        beq   10f
        // flash LED to report ram check error
        b     abort_preamble                            /* no return */
10:
        // passed ram check
        b     reset_vector                              /* no return */
#else

        // REMAP on OLT and ONU
        ldr     r1,.imst_remap_crtl
        str     r0,[r1]                                 /* write any value do REMAP */

        b       reset_vector        /* passed min ram check */


#endif

.imst_remap_crtl:
        .word IMST_REMAP_CRTL_REG



//For FPGA ddr2
.imst_sdram_config_reg:
        .word 0x2c308068
.imst_sdram_config_val1:
        .word 0x003f0960
.imst_sdram_config_val2:
        .word 0x003f0160
.imst_sdram_config_val3:
        .word 0x003f72a0


//PER
.imst_uart_config_reg: //UART BASE
        .word IMST_UART0_BASE
.imst_uart_tun_reg:
        .word IROS_PER_UART0_TUN
.imst_uart_tun_val:
        //.word 0x01210030
        .word IMST_UART_TUNE_VAL
.imst_uart_rx_sample:
        .word IROS_PER_UART0_RX_SAMPLE
.imst_uart_rx_sample_val:
        //.word 0x0000032d
        .word IMST_UART_SAMPLE_VAL


.imst_uart_config_val:
        //.word 0x865a0063
         .word IMST_UART_CFG_VAL
.imst_uart_out_reg:
        .word (IMST_UART0_BASE+IMST_UARTDW)
.imst_uart_out_valb: //69 6E 69 74
        .word 0x42 //'s'
.imst_uart_out_valo:
        .word 0x6f
.imst_uart_out_valt:
        .word 0x74
.imst_uart_out_val_dot:
        .word 0x2e
.imst_uart_out_val_st:
        .word 0x2a
.imst_gpio_out_reg:
        .word IROS_PER_GPIO_OUT



abort_preamble:
	.global abort_preamble
        .type abort_preamble, function

         // flash LED to report ram check error
#if defined(IMST_USE_CM_MISC_LED)
        bl    led_flash
#endif

        b     platform_preamble_start  // no return



#if defined(IMST_USE_CM_MISC_LED)
#if !defined(LED_FLASH_BY_C)
	.global led_flash
        .type led_flash, function
led_flash:
        ldr     r3, .reg_led
        ldr     r0, .led_on
        ldrb    r2, [r3]
        and     r2, r2, #255
        orr     r1, r2, #CM_HDR_DBG_MISC_LED
        strb    r1, [r3]
led_10: subs    r0, r0, #1
        bne     led_10
        ldr     ip, .reg_led
        bic     r3, r2, #CM_HDR_DBG_MISC_LED
        ldr     r0, .led_off
        strb    r3, [ip]
led_20: subs    r0, r0, #1
        bne     led_20
        bx      lr

.reg_led:
        .word   CM_HDR_CTRL_REG
.led_on:
        .word   IMST_USE_CM_MISC_LED_ON_LEN
.led_off:
        .word   IMST_USE_CM_MISC_LED_OFF_LEN

#endif /* LED_FLASH_BY_C */
#endif /* IMST_USE_CM_MISC_LED */


#endif /* IROSBOOT */

#endif /* CYGONCE_HAL_PLATFORM_EXTRAS_H */

