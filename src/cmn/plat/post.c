/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

/* OLT: power-on system test */

#include "iros_config.h"
#include "plat_common.h"
#include <cyg/hal/hal_immenstar.h>        // Hardware definitions
#include <stdio.h>

#include <cyg/hal/hal_arch.h>           // for CYGNUM_HAL_STACK_SIZE_TYPICAL
#include <pkgconf/hal.h>
#include <cyg/hal/hal_if.h>
#include <cyg/hal/hal_io.h>

void sram_test(void);
void ddr_test(void);

#define IO_READ(p)          ((*(volatile unsigned int *)(p)))
#define IO_WRITE(p, c)      (*(volatile unsigned int *)(p) = (c))
#define GET_STATUS(p)       (IO_READ((p) + IMST_UARTFR))
#define GET_CHAR(p)         (IO_READ((p) + IMST_UARTDR))
#define PUT_CHAR(p, c)      (IO_WRITE(((p) + IMST_UARTDW), (c)))
#define RX_DATA(s)          (((s) & IMST_UARTFR_DA) != 0)
#define TX_READY(s)         (((s) & IMST_UARTFR_TXFF) == 0)

#define WDT_PRESCALE_VAL       0x7fff
//#define OLT_GLB_SW_RESET_CTRL_REG 0x2c306008

#define POST_MAX_ERRO_NUM      0x800




void post_serial_putc(char c)
{
    cyg_uint32 status;
    int i;

    i = 10000;
    do {
        status = GET_STATUS(IMST_UART0_BASE);
        i--;
        if (i < 0) break;
    } while (!TX_READY(status));    // wait until ready

    PUT_CHAR(IMST_UART0_BASE, c);

    if (c == '\n') {
        i = 10000;
        do {
            status = GET_STATUS(IMST_UART0_BASE);
            i--;
            if (i < 0) break;
        } while (!TX_READY(status));    // wait until ready

        PUT_CHAR(IMST_UART0_BASE, '\r');
    }

}

void post_output_setup(void) {

#if 0
#ifdef CYG_HAL_ARM_IMST_OLT
        // reset asic, this will not reset arm
        diag_printf("RESET ASIC, not reset ARM\n");
        HAL_WRITE_UINT32(OLT_GLB_SW_RESET_CTRL_REG, 0xFF000000); // 0x6008
        HAL_WRITE_UINT32(OLT_GLB_SW_RESET_CTRL_REG, 0xFFFFFFFF);
        diag_printf("RESET ASIC Done\n");
#endif
#endif

#if 0
        // clear IMAGE_DOWNLOAD_EN bit
        HAL_WRITE_UINT32(0x2c302000, 0);
        HAL_WRITE_UINT32(0x2c301080, 0);
        HAL_WRITE_UINT32(0x2c301090, 0);
#endif


#if 0
    HAL_WRITE_UINT32(0x2c205000, 0x865a01c3);
    HAL_WRITE_UINT32(0x2c20500c, 0x21030000);

    //HAL_WRITE_UINT32(0x2c205004, '1');
#endif
}

void post_output(char *p) {

    //HAL_WRITE_UINT32(0x2c205004, '4');

    int i = 0;
    while(p[i] != 0) {
        post_serial_putc(p[i]);
        if (i >= 128) {
            break;
        }
        i++;
    }
    return;
}

void post_output_hex(unsigned int val) {
    int i;
    unsigned int v1, v2;
    post_serial_putc('0');
    post_serial_putc('x');
    v1 = val;
    for (i = 0; i < 8; i++) {
        v2 = (v1 & 0xf0000000)>>28;
        v1 = (v1 << 4);
        if (v2 <= 9) {
                post_serial_putc('0'+v2);
        } else {
                post_serial_putc('a' + v2 - 10);
        }
    }
    //post_serial_putc('\n');
};

void post_output_binary(char val) {
    int i;
    char v1, v2;
    v1 = val;
    for (i = 0; i < 8; i++) {
        v2 = (v1 & 0x80)>>7;
        v1 = (v1 << 1);
        if (v2 == 0 ) {
            diag_printf("0");
        } else {
            diag_printf("1");
        }
    }
};

/* stage1 should have been done by the loader */

/* stage2: tests are executed after the loader but before the application starts */
void post_stage2(void) {

    post_output_setup();
    //HAL_WRITE_UINT32(0x2c205004, '2');
    post_serial_putc('p');
    post_serial_putc('o');
    post_serial_putc('s');
    post_serial_putc('t');
    post_serial_putc('2');
    post_serial_putc('\n');


#if 0
    // speed-up sram/flash access, requested by John Namkung 20061128
    HAL_WRITE_UINT32(0x2c100004, 0x00f10b01);

    // reset watchdog timer for longer timeout value
    HAL_WRITE_UINT32(IROS_WDT_CTRL, 0);
    HAL_WRITE_UINT32(IROS_WDT_STATUS, 0);

    // 20 seconds
    HAL_WRITE_UINT32(IROS_WDT_PRESCALE, 0); // clear
    HAL_WRITE_UINT32(IMST_WDT_COUNTUP, WDT_PRESCALE_VAL);
    HAL_WRITE_UINT32(IROS_WDT_LOAD, (60 * (CYG_HAL_ARM_IMST_DEV_FREQ / WDT_PRESCALE_VAL)));
    HAL_WRITE_UINT32(IROS_WDT_CTRL, (IMST_WDT_RSTEN | IMST_WDT_EN | IMST_WDT_INTEN));

    post_output_setup();
    //HAL_WRITE_UINT32(0x2c205004, '2');

    //HAL_WRITE_UINT32(IMST_GPIO_DIR, 0x0001); // all GPIO execpt bit 0 are OUTPUT
    /* other tests */
#ifdef CYG_HAL_ARM_IMST_ONU
    sram_test_with_gpio(false);
#else
    sram_test();
#endif

#endif
}

#ifdef CYG_HAL_ARM_IMST_ONU
#if 0
bool chip_test_ok(void) {
    // TBD
    return true;
}

void chip_test(void) {
    // output high GPIO13
    //HAL_WRITE_UINT32(IMST_GPIO_DIR, 0xdfff); // GPIO13
    HAL_WRITE_UINT32(IMST_GPIO_DATA, 0x2000); // GPIO13 high

    // do the chip test ???
    hal_delay_us(10);
    if (!chip_test_ok()) {
        // output GPIO12 pulse
        //HAL_WRITE_UINT32(IMST_GPIO_DIR, 0xefff); // GPIO12
        HAL_WRITE_UINT32(IMST_GPIO_DATA, 0x1000 | 0x2000 ); // GPIO12 and GPIO13 high
        hal_delay_us(1);
        //HAL_WRITE_UINT32(IMST_GPIO_DIR, 0xefff); // GPIO12
        HAL_WRITE_UINT32(IMST_GPIO_DATA, 0x0000); // GPIO12 and GPIO13 low
        post_output("CHIP Test Failed.\n");
    } else {
        post_output("CHIP Test Passed.\n");
    }
    // output high GPIO13
    //HAL_WRITE_UINT32(IMST_GPIO_DIR, 0xdfff); // GPIO13
    HAL_WRITE_UINT32(IMST_GPIO_DATA, 0x0000); // GPIO13 and all low
}

extern int eeprom_read(char *p, cyg_uint32 e_addr, int len);
extern int eeprom_write(cyg_uint32 e_addr, char *p, int len);
bool eeprom_test_ok(void) {
    char eeprom_data[128];
    eeprom_read(eeprom_data, 0, 64 /* 2Bytes words*/);
    if (eeprom_data[0] == 0x55) {
        post_output("EEPROM Test Passed.\n");
        return true;
    } else {
        // trying to write
        memset(eeprom_data, 0x55, 128);
        eeprom_write(0, eeprom_data, 64);
        memset(eeprom_data, 0, 128);
        eeprom_read(eeprom_data, 0, 64 /* 2Bytes words*/);
        if (eeprom_data[0] == 0x55) {
            memset(eeprom_data, 0, 128);
            eeprom_write(0, eeprom_data, 64);
            post_output("EEPROM Test Passed.\n");
            return true;
        }
    }
    post_output("EEPROM Test Failed.\n");
    return false;
}

void eeprom_test(void) {
    // output high GPIO14
    // HAL_WRITE_UINT32(IMST_GPIO_DIR, 0xbfff); // GPIO14
    HAL_WRITE_UINT32(IMST_GPIO_DATA, 0x4000); // GPIO14 high

    // do the eeprom test ???
    hal_delay_us(10);
    if (!eeprom_test_ok()) {
        // output GPIO12 pulse
        //HAL_WRITE_UINT32(IMST_GPIO_DIR, 0xefff); // GPIO12
        HAL_WRITE_UINT32(IMST_GPIO_DATA, 0x1000 | 0x4000); // GPIO12 high
        hal_delay_us(1);
        //HAL_WRITE_UINT32(IMST_GPIO_DIR, 0xefff); // GPIO12
        HAL_WRITE_UINT32(IMST_GPIO_DATA, 0x0000); // GPIO12 low
    }
    // output high GPIO13
    //HAL_WRITE_UINT32(IMST_GPIO_DIR, 0xbfff); // GPIO14
    HAL_WRITE_UINT32(IMST_GPIO_DATA, 0x0000); // GPIO14 low
}

#endif
#endif


/* stage3: executed after normal application setup completed */
void post_stage3(void) {

#ifdef CYG_HAL_ARM_IMST_OLT
    ddr_test();
#else
// ONU
    //chip_test_with_gpio(false);
    //eeprom_test_with_gpio(false);
#endif
    /* other tests */
}

#ifdef CYG_HAL_ARM_IMST_OLT
int sram_test_general(void * test_start, unsigned int test_size) {
    // make sure the wrap-around not happen in the range tested
    unsigned int * p = (unsigned int *) test_start;
    unsigned int len = test_size >> 2;
    unsigned int i, j, k;
    // write loop, write a pattern of every 256KB of 16 words
    // write mroe words may cause the test program itself fails to run.
    // ideally no sram should be used to test sram.

    //post_output_hex((unsigned int)test_start);
    //post_output_hex((unsigned int)test_size);

    i = len - 16 - 1;
    j = 1;
    while (i > (64<<10)) {
        for (k = 0; k < 16; k++) {
            p[i + k] = j;
            j++;
        }
        i -= (64<<10);
    }
    // verifying loop
    i = len - 16 - 1;
    j = 1;
    while (i > (64<<10)) {
        for (k = 0; k < 16; k++) {
            if (p[i + k] != j) {
                post_output("SRAM Test Failed: Size Problem!\n");
                return 1;
            }
            j++;
        }
        i -= (64<<10);
    }
    post_output("SRAM Test Size Check Passed.\n");
    return 0;
}

int sram_test32(void * test_start, unsigned int test_size) {
    unsigned int * p = (unsigned int *) test_start;
    unsigned int i;
    int erro_count = 0;
#if 0
    for (i = 0; i < (test_size >> 2); i++) {
        HAL_WRITE_UINT32(&p[i], 0xaa55aa55);
        if (p[i] != 0xaa55aa55) {
            diag_printf("32Bit Word Error 1 addr 0x%08x val 0x%08x exp 0x%08x\n",
                        &p[i], p[i], 0xaa55aa55);
            erro_count++;
            if (erro_count > POST_MAX_ERRO_NUM) break;
            break;
        }
        HAL_WRITE_UINT32(&p[i], 0x55aa55aa);
        if (p[i] != 0x55aa55aa) {
            diag_printf("32Bit Word Error 2 addr 0x%08x val 0x%08x exp 0x%08x\n",
                        &p[i], p[i], 0x55aa55aa);
            erro_count++;
            if (erro_count > POST_MAX_ERRO_NUM) break;
            break;
        }
    }
    if (erro_count > POST_MAX_ERRO_NUM) goto too_many_errors;
#endif

    for (i = 0; i < (test_size >> 2); i++) {
        HAL_WRITE_UINT32(&p[i], 0xaa55aa55);
    }
    for (i = 0; i < (test_size >> 2); i++) {
        if (p[i] != 0xaa55aa55) {
            diag_printf("32Bit Word Error 1 addr 0x%08x val 0x%08x exp 0x%08x\n",
                        &p[i], p[i], 0xaa55aa55);
            erro_count++;
            if (erro_count > POST_MAX_ERRO_NUM) break;
        }
    }
    if (erro_count > POST_MAX_ERRO_NUM) goto too_many_errors;

    for (i = 0; i < (test_size >> 2); i++) {
        HAL_WRITE_UINT32(&p[i], 0x55aa55aa);
    }
    for (i = 0; i < (test_size >> 2); i++) {
        if (p[i] != 0x55aa55aa) {
            diag_printf("32Bit Word Error 2 addr 0x%08x val 0x%08x exp 0x%08x\n",
                        &p[i], p[i], 0x55aa55aa);
            erro_count++;
            if (erro_count > POST_MAX_ERRO_NUM) break;
        }
    }

    if (erro_count > POST_MAX_ERRO_NUM) goto too_many_errors;

    if (erro_count == 0) {
        // do more tests
        for (i = 0; i < (test_size >> 2); i++) {
            HAL_WRITE_UINT32(&p[i], (unsigned int)(&p[i]));
        }
        for (i = 0; i < (test_size >> 2); i++) {
            if (p[i] != (unsigned int) (&p[i])) {
                diag_printf("32Bit Word Error 3 addr 0x%08x val 0x%08x exp 0x%08x\n",
                            &p[i], p[i], (unsigned int) (&p[i]));
                erro_count++;
                if (erro_count > POST_MAX_ERRO_NUM) break;
            }
        }
        if (erro_count > POST_MAX_ERRO_NUM) goto too_many_errors;
    }

    goto finished;

 too_many_errors:
    diag_printf("Too many errors. Test aborted. \n");

 finished:

    if (erro_count >= 1) {
        post_output("SRAM Test Failed: 32 Bit Word R/W Problem!\n");
    } else {
        post_output("SRAM Test 32 Bit Word R/W Passed.\n");
    }

    return erro_count;

}

int sram_test16(void * test_start, unsigned int test_size) {
    unsigned short int * p = (unsigned short int *) test_start;
    int i;
    int erro_count = 0;
#if 0
    for (i = 0; i < (test_size >> 1); i++) {
        HAL_WRITE_UINT16(&p[i], 0xaa55);
        if (p[i] != 0xaa55) {
            diag_printf("16Bit Halfword Error 1 addr 0x%08x val 0x%04x exp 0x%04x\n",
                        &p[i], p[i], 0xaa55);
            erro_count++;
            if (erro_count > POST_MAX_ERRO_NUM) break;
        }
        HAL_WRITE_UINT16(&p[i], 0x55aa);
        if (p[i] != 0x55aa) {
            diag_printf("16Bit Halfword Error 2 addr 0x%08x val 0x%04x exp 0x%04x\n",
                        &p[i], p[i], 0x55aa);
            erro_count++;
            if (erro_count > POST_MAX_ERRO_NUM) break;
        }
    }

    if (erro_count > POST_MAX_ERRO_NUM) goto too_many_errors;
#endif

    for (i = 0; i < (test_size >> 1); i++) {
        HAL_WRITE_UINT16(&p[i], 0xaa55);
    }

    for (i = 0; i < (test_size >> 1); i++) {
        if (p[i] != 0xaa55) {
            diag_printf("16Bit Halfword Error 1 addr 0x%08x val 0x%04x exp 0x%04x\n",
                        &p[i], p[i], 0xaa55);
            erro_count++;
            if (erro_count > POST_MAX_ERRO_NUM) break;
        }
    }

    if (erro_count > POST_MAX_ERRO_NUM) goto too_many_errors;

    for (i = 0; i < (test_size >> 1); i++) {
        HAL_WRITE_UINT16(&p[i], 0x55aa);
    }

    for (i = 0; i < (test_size >> 1); i++) {
        if (p[i] != 0x55aa) {
            diag_printf("16Bit Halfword Error 2 addr 0x%08x val 0x%04x exp 0x%04x\n",
                        &p[i], p[i], 0x55aa);
            erro_count++;
            if (erro_count > POST_MAX_ERRO_NUM) break;
        }
    }


    if (erro_count > POST_MAX_ERRO_NUM) goto too_many_errors;

    if (erro_count == 0) {
        // do more test
        for (i = 0; i < (test_size >> 2); i++) {
            HAL_WRITE_UINT16(&p[i<<1], ((unsigned int)(&p[i<<1])) & 0xffff);
            HAL_WRITE_UINT16(&p[(i<<1) + 1], (((unsigned int)(&p[i<<1]))>> 16) & 0xffff);
        }
        for (i = 0; i < (test_size >> 2); i++) {
            if (p[i<<1] != (((unsigned int) (&p[i<<1])) & 0xffff)) {
                diag_printf("16Bit Halfword Error 3 addr 0x%08x val 0x%04x exp 0x%04x\n",
                            &p[i<<1], p[i<<1], (((unsigned int) (&p[i<<1])) & 0xffff));
                erro_count++;
                if (erro_count > POST_MAX_ERRO_NUM) break;
            }
            if (p[(i<<1) + 1] != (((unsigned int) (&p[i<<1]) >> 16) & 0xffff)) {
                diag_printf("16Bit Halfword Error 4 addr 0x%08x val 0x%04x exp 0x%04x\n",
                            &p[(i<<1) + 1], p[(i<<1) + 1], (((unsigned int) (&p[i<<1]) >> 16) & 0xffff));
                erro_count++;
                if (erro_count > POST_MAX_ERRO_NUM) break;
            }
        }
        if (erro_count > POST_MAX_ERRO_NUM) goto too_many_errors;
    }
    goto finished;

 too_many_errors:
    diag_printf("Too many errors. Test aborted. \n");

 finished:
    if (erro_count >= 1) {
        post_output("SRAM Test Failed: 16 Bit Half-Word R/W Problem!\n");
    } else {
        post_output("SRAM Test 16 Bit Half-Word R/W Passed.\n");
    }

    return erro_count;
}

int sram_test8(void * test_start, unsigned int test_size) {

    char * p = (char *) test_start;
    int i;
    int erro_count = 0;

#if 0
    for (i = 0; i < test_size; i++) {
        HAL_WRITE_UINT8(&p[i], 0xaa);
        if (p[i] != 0xaa) {
                diag_printf("Byte Error 1 addr 0x%08x val 0x%02x exp 0x%02x\n",
                            &p[i], p[i], 0xaa);
                erro_count++;
                if (erro_count > POST_MAX_ERRO_NUM) break;
        }
        HAL_WRITE_UINT8(&p[i], 0x55);
        if ( p[i] != 0x55) {
            diag_printf("Byte Error 2 addr 0x%08x val 0x%02x exp 0x%02x\n",
                        &p[i], p[i], 0x55);
        }
    }
    if (erro_count > POST_MAX_ERRO_NUM) goto too_many_errors;
#endif

    for (i = 0; i < test_size; i++) {
        HAL_WRITE_UINT8(&p[i], 0xaa);
    }
    for (i = 0; i < test_size; i++) {
        if (p[i] != 0xaa) {
                diag_printf("Byte Error 1 addr 0x%08x val 0x%02x exp 0x%02x\n",
                            &p[i], p[i], 0xaa);
                erro_count++;
                if (erro_count > POST_MAX_ERRO_NUM) break;
        }
    }

    if (erro_count > POST_MAX_ERRO_NUM) goto too_many_errors;

    for (i = 0; i < test_size; i++) {
        HAL_WRITE_UINT8(&p[i], 0x55);
    }

    for (i = 0; i < test_size; i++) {
        if ( p[i] != 0x55) {
            diag_printf("Byte Error 2 addr 0x%08x val  0x%02x exp 0x%02x\n",
                            &p[i], p[i], 0x55);
            erro_count++;
            if (erro_count > POST_MAX_ERRO_NUM) break;
        }
    }

    if (erro_count > POST_MAX_ERRO_NUM) goto too_many_errors;

    if (erro_count == 0) {
        // do more test
        for (i = 0; i < (test_size >> 2); i++) {
            HAL_WRITE_UINT8(&p[i<<2], ((unsigned int)(&p[i<<2])) & 0xff);
            HAL_WRITE_UINT8(&p[(i<<2) + 1], (((unsigned int)(&p[i<<2]))>> 8) & 0xff);
            HAL_WRITE_UINT8(&p[(i<<2) + 2], (((unsigned int)(&p[i<<2]))>> 16) & 0xff);
            HAL_WRITE_UINT8(&p[(i<<2) + 3], (((unsigned int)(&p[i<<2]))>> 24) & 0xff);
        }

        for (i = 0; i < (test_size >> 2); i++) {
            if (p[i<<2] != (((unsigned int) (&p[i<<2])) & 0xff)) {
                diag_printf("Byte Error 3 addr 0x%08x val 0x%02x exp 0x%02x\n",
                            &p[i<<2], p[i<<2], (((unsigned int) (&p[i<<2])) & 0xff));
                erro_count++;
                if (erro_count > POST_MAX_ERRO_NUM) break;
            }
            if (p[(i<<2) + 1] != (((unsigned int) (&p[i<<2]) >> 8) & 0xff)) {
                diag_printf("Byte Error 4 addr 0x%08x val 0x%02x exp 0x%02x\n",
                            &p[(i<<2)+1], p[(i<<2) + 1], (((unsigned int) (&p[i<<2]) >> 8) & 0xff));
                erro_count++;
                if (erro_count > POST_MAX_ERRO_NUM) break;
            }
            if (p[(i<<2) + 2] != (((unsigned int) (&p[i<<2]) >> 16) & 0xff)) {
                    diag_printf("Byte Error 5 addr 0x%08x val 0x%02x exp 0x%02x\n",
                                &p[(i<<2)+2], p[(i<<2) + 2], (((unsigned int) (&p[i<<2]) >> 16) & 0xff));
                    erro_count++;
                    if (erro_count > POST_MAX_ERRO_NUM) break;
            }
            if (p[(i<<2) + 3] != (((unsigned int) (&p[i<<2]) >> 24) & 0xff)) {
                    diag_printf("Byte Error 6 addr 0x%08x val 0x%02x exp 0x%02x\n",
                                &p[(i<<2)+3], p[(i<<2) + 3], (((unsigned int) (&p[i<<2]) >> 24) & 0xff));
                    erro_count++;
                    if (erro_count > POST_MAX_ERRO_NUM) break;
            }
        }
        if (erro_count > POST_MAX_ERRO_NUM) goto too_many_errors;

    }

    goto finished;

 too_many_errors:
    diag_printf("Too many errors. Test aborted. \n");

 finished:
    if (erro_count >= 1) {
        post_output("SRAM Test Failed: Byte R/W Problem!\n");
    } else {
        post_output("SRAM Test 8 Bit Byte R/W Passed.\n");
    }

    return erro_count;

}

#ifdef CYG_HAL_ARM_IMST_OLT
#include "startup_cfg.h"
extern epon_olt_startup_config_t app_boot_parameters;
#endif

extern void * __rom_data_end;


void sram_test(void) {
    void *test_start;
    unsigned int test_size; /* number of bytes */
    unsigned int sram_size;


    //HAL_WRITE_UINT32(0x2c205004, '3');

    //unsigned int image_size;

#ifdef CYG_HAL_ARM_IMST_OLT

    if(0)/*(app_eeprom_parameters->eeprom_control_flags == 0x55) {*//*XXXX_DELETE*/
       /* sram_size = app_boot_parameters.sram_size << 20;*//*XXXX_DELETE*/
    } else {
        sram_size = IMST_OLT_RAM_SIZE;
    }

    //post_output_hex(sram_size);
    if (sram_size == 0) {
        //sram_size = 1 << 20; /* 1 MB*/
        // default is IMST_OLT_RAM_SIZE
        sram_size = IMST_OLT_RAM_SIZE;
    }

    //sram_size = 1 << 20; /* 1 MB*/
    //image_size = (unsigned int)__rom_data_end - IMST_RAM_APP_START;
    test_start = (void *) &__rom_data_end;
    test_size = sram_size - (unsigned int) test_start;
#else
//ONU
    if ((IMST_RAM_APP_START & 0xff000000) == 0x2f000000) {
        // ROM app
        sram_size = 0x40000; // 256KB
        test_start = (void *) 0x22000000;
        test_size = sram_size;
        return; // no sram test for rom app
    } else {
        // RAM app
       sram_size = (IMST_RAM_APP_LEN + IMST_DEBUG_BUF_LEN);
       test_start = (void *) &__rom_data_end;
       test_size = IMST_RAM_APP_START + sram_size - (unsigned int) test_start;
    }

#endif


    //post_output_hex((unsigned int)test_start);
    //post_output_hex(test_size);

    //post_output_hex(&test_start);
    //post_output_hex(&test_size);

    post_output("SRAM Test Size ");
    post_output_hex(sram_size);
    post_output(" Start ");
    post_output_hex((unsigned int)test_start);
    post_output("\n");

#ifdef CYG_HAL_ARM_IMST_ONU
    // output high GPIO15
    //HAL_WRITE_UINT32(IMST_GPIO_DIR, 0x7fff); // GPIO15
    //HAL_WRITE_UINT32(IMST_GPIO_DATA, 0x8000); // GPIO15 high
#endif
    int result = sram_test_general(test_start, test_size);

    result += sram_test8(test_start, test_size);
    if (result > POST_MAX_ERRO_NUM) goto too_many_errors;

    result += sram_test16(test_start, test_size);
    if (result > POST_MAX_ERRO_NUM) goto too_many_errors;

    result += sram_test32(test_start, test_size);
    if (result > POST_MAX_ERRO_NUM) goto too_many_errors;


    goto finished;

 too_many_errors:
    diag_printf("Too many errors. Test aborted. \n");

 finished:
    if (result >= 1) {
        post_output("SRAM Test Failed!\n");
#ifdef CYG_HAL_ARM_IMST_ONU
        if ((IMST_RAM_APP_START & 0xff000000) == 0x2f000000) {
            return;
        }
#endif

        post_output("reboot\n");
        hal_delay_us(10000);
        HAL_PLATFORM_RESET();
    } else {
        post_output("SRAM Test Passed.\n");

    }

}

#endif /* CYG_HAL_ARM_IMST_OLT */

#ifdef CYG_HAL_ARM_IMST_OLT

#define DRAM_DLL0_CONF_REG        (0x2c300000 + 0x5044)
#define DRAM_WRITE_READ_DATA0     (0x2c300000 + 0x5014)
#define DRAM_ACCESS_COMMAND_REG   (0x2c300000 + 0x500c)

bool is_dll_ok(unsigned int test_count) {
    unsigned int row, bank, col;
    //unsigned int seq_num;
    unsigned int i;
    unsigned int write_data;
    unsigned int read_data;
    unsigned int data_pattern[8];

    // data is <test_count><seq_num>
    // seq_num is 1 to to 8 and stored in bit0 to bit3
    // seq_num is i+1
    for (i = 0; i < 8; i++) {
        data_pattern[i] = (test_count << 4) + (i+1);
        HAL_WRITE_UINT32((DRAM_WRITE_READ_DATA0 + 4 * i),data_pattern[i]);
    }

    // write
    for (row = 0; row < 12; row++) {
        for (bank = 0; bank < 4; bank++) {
            for (col = 0; col < 9; col++) {
                write_data = ((1<<row)<<19) +   /* row */
                    (bank<<17) +            /* bank */
                    ((1<<col)<<7) +         /* col */
                    (1<<5);                 /* write command */
                HAL_WRITE_UINT32(DRAM_ACCESS_COMMAND_REG, write_data);
                i = 0;
                read_data = 0;
                while ((read_data & 1) == 1) {
                    if (i > 500) {
                        return false;
                    }
                    i++;
                    HAL_READ_UINT32(DRAM_ACCESS_COMMAND_REG, read_data);
                }
            }
        }
    }

    // read-back and verify
    for (row = 0; row < 12; row++) {
        for (bank = 0; bank < 4; bank++) {
            for (col = 0; col < 9; col++) {
                write_data = ((1<<row)<<19) +   /* row */
                    (bank<<17) +            /* bank */
                    ((1<<col)<<7) +         /* col */
                    (3<<5);                 /* read command */
                HAL_WRITE_UINT32(DRAM_ACCESS_COMMAND_REG, write_data);
                i = 0;
                read_data = 0;
                while ((read_data & 1) == 1) {
                    if (i > 500) {
                        return false;
                    }
                    i++;
                    HAL_READ_UINT32(DRAM_ACCESS_COMMAND_REG, read_data);
                }
                for (i = 0; i < 8; i++) {
                    HAL_READ_UINT32((DRAM_WRITE_READ_DATA0 + 4 * i), read_data);
                    if (read_data != data_pattern[i]) {
                        return false;
                    }
                }

            }
        }
    }

    return true;

}

/* diag_printf available at post stage 3 */
void ddr_test(void) {
    int image_download_direction;
    //post_output("post_output ddr_test\n");
    diag_printf("DDR Test Started.\n");
    if (is_dll_ok(1)) {
        diag_printf("DDR Test Passed.\n");
    } else {
        diag_printf("DDR Test Failed!\n");
        // if UART load, no reboot

        HAL_READ_UINT32(IMST_IMAGE_DOWNLOAD, image_download_direction);
        image_download_direction &= IMAGE_DOWNLOAD_BITS;
        if (image_download_direction == IMAGE_FROM_ETH) {
            diag_printf("reboot\n");
            hal_delay_us(10000);
            HAL_PLATFORM_RESET();
        }
    }
}

/*#include "ifm.h"*/
extern ifm_tabs_t ifm_tabs;

bool do_cmd_dll_once(
    unsigned int,
    unsigned int bank,
    unsigned int row,
    unsigned int count);

void do_cmd_dll_test(
    unsigned int repeat_count,
    unsigned int limit,
    unsigned int bank,
    unsigned int row,
    unsigned int count)
{

    int i;
    unsigned long __state;
    ifm_port_entry_t *p_port;
    cs_port_id_t port_id;

    // turn off all ports: pon, nni, and management ports
    for (i = 0; i < EPON_MAX_PON_PORTS; i++) {
        p_port = (ifm_port_entry_t *)&ifm_tabs.pon_port_tab[i];
        port_id = p_port->id;
        if (port_id != 0) {
            olt_aal_set_port_status(port_id, EPON_PORT_ADMIN_DOWN);
        }
    }
    for (i = 0; i < EPON_MAX_NNI_PORTS; i++) {
        p_port = (ifm_port_entry_t *)&ifm_tabs.nni_port_tab[i];
        port_id = p_port->id;
        if (port_id != 0) {
            olt_aal_set_port_status(port_id, EPON_PORT_ADMIN_DOWN);
        }
    }
#if 0 // AAL not allow to turn off Management Ports
    for (i = 0; i < EPON_MAX_MGMT_PORTS; i++) {
        p_port = (ifm_port_entry_t *)&ifm_tabs.mgmt_port_tab[i];
        port_id = p_port->id;
        if (port_id != 0) {
            olt_aal_set_port_status(port_id, EPON_PORT_ADMIN_DOWN);
        }
    }
#endif
    // turn off management ports as Allen Fan told 01/10/2007
    HAL_WRITE_UINT32(0x2c301080, 0x24);
    HAL_WRITE_UINT32(0x2c301090, 0x24);

    hal_delay_us(5000); // wait to clean up

    HAL_DISABLE_INTERRUPTS(__state);
    diag_printf("Start destructive DDR test ...\n");
    for (i = 0; i < repeat_count; i++) {
        diag_printf("DDR test %2d\n", i+1);
        if (do_cmd_dll_once(limit, bank, row, count)) {
            diag_printf("DDR test PASSED.\n");
        } else {
            diag_printf("DDR test FAILED.\n");
        }
    }
    diag_printf("DDR test completed. Please reboot the system! (use cmd: v r)\n");
    HAL_RESTORE_INTERRUPTS(__state);

}

bool do_ddr_data_once(
    unsigned int pattern,
    unsigned int bankid,
    unsigned int rowid,
    unsigned int count)
{
    unsigned int row, bank, col;
    unsigned int row1, row2, row3;
    //unsigned int seq_num;
    unsigned int i;
    unsigned int write_data;
    unsigned int read_data;
    unsigned int data_pattern[8];
    unsigned int data_read[8];
    unsigned int data_error_bits[8];
    bool result;
    result = true;

    // data is <test_count><seq_num>
    // seq_num is 1 to to 8 and stored in bit0 to bit3
    // seq_num is i+1
    for (i = 0; i < 8; i++) {
        data_pattern[i] = pattern;
        HAL_WRITE_UINT32((DRAM_WRITE_READ_DATA0 + 4 * i),data_pattern[i]);
    }

    if (count != 0) {
        // do test for the given bankid and rowid only for count in col
        // write
        row = rowid;
        bank = bankid;
        diag_printf("Test bank 0x%1x row 0x%04x col 0x%03x - 0x%03x\n",
                   bank, row, 0, (count <<2));
        diag_printf("Write Pattern 0x%08x\n", pattern);

        for ( col = 0; col < (count << 2); col += 4) {
            write_data = (row<<19) +    /* row */
                (bank<<17) +            /* bank */
                (col<<7) +              /* col */
                (1<<5);                 /* write command */
            HAL_WRITE_UINT32(DRAM_ACCESS_COMMAND_REG, write_data);
            i = 0;
            read_data = 0;
            while ((read_data & 1) == 1) {
                if (i > 500) {
                }
                i++;
                HAL_READ_UINT32(DRAM_ACCESS_COMMAND_REG, read_data);
            }
        }

        // read-back and verify
        diag_printf("Verify ");
        for (col = 0; col < (count << 2); col += 4) {
            write_data = (row<<19) +    /* row */
                (bank<<17) +            /* bank */
                (col<<7) +              /* col */
                (3<<5);                 /* read command */
            HAL_WRITE_UINT32(DRAM_ACCESS_COMMAND_REG, write_data);
            i = 0;
            read_data = 0;
            while ((read_data & 1) == 1) {
                if (i > 500) {
                    return false;
                }
                i++;
                HAL_READ_UINT32(DRAM_ACCESS_COMMAND_REG, read_data);
            }
            for (i = 0; i < 8; i++) {
                HAL_READ_UINT32((DRAM_WRITE_READ_DATA0 + 4 * i), read_data);
                //read_data=0x55aa55ab; // test failure
                data_read[i] = read_data;
                if (read_data != data_pattern[i]) {
                    result = false;
                }
            }
        }

    } else {
        // write
        diag_printf("Write Pattern 0x%08x\n", pattern);
        row = 0;
        for (row1 = 0; row1 < 1; row1++) {
            for (row2 = 0; row2 < 1; row2++) {
                for (row3 = 0 ; row3 < 1; row3++) {
                    row++;
                    for (bank = 0; bank < 1; bank++) {
                        //for (col = 0; col < 1; col++) {
                        for (col = 0; col < 1; col++) {
                            write_data = (row<<19) +    /* row */
                                (bank<<17) +            /* bank */
                                (col<<7) +              /* col */
                                (1<<5);                 /* write command */
                            HAL_WRITE_UINT32(DRAM_ACCESS_COMMAND_REG, write_data);
                            i = 0;
                            read_data = 0;
                            while ((read_data & 1) == 1) {
                                if (i > 500) {

                                }
                                i++;
                                HAL_READ_UINT32(DRAM_ACCESS_COMMAND_REG, read_data);
                            }
                        }
                    }
                }
            }
        }

        // read-back and verify
        diag_printf("Verify ");
        //for (row = 0; row < 1; row++) {
        row = 0;
        for (row1 = 0; row1 < 1; row1++) {
            for (row2 = 0; row2 < 1; row2++) {
                for (row3 = 0 ; row3 < 1; row3++) {
                    row++;
                    for (bank = 0; bank < 1; bank++) {
                        //for (col = 0; col < 1; col++) {
                        for (col = 0; col < 1; col++) {
                            write_data = (row<<19) +    /* row */
                                (bank<<17) +            /* bank */
                                (col<<7) +              /* col */
                                (3<<5);                 /* read command */
                            HAL_WRITE_UINT32(DRAM_ACCESS_COMMAND_REG, write_data);
                            i = 0;
                            read_data = 0;
                            while ((read_data & 1) == 1) {
                                if (i > 500) {
                                    return false;
                                }
                                i++;
                                HAL_READ_UINT32(DRAM_ACCESS_COMMAND_REG, read_data);
                            }
                            for (i = 0; i < 8; i++) {
                                HAL_READ_UINT32((DRAM_WRITE_READ_DATA0 + 4 * i), read_data);
                                //read_data=0x55aa55ab; // test failure
                                data_read[i] = read_data;
                                if (read_data != data_pattern[i]) {
                                    result = false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (!result) {
        //report error
        diag_printf("Failed\n");
        for (i = 0; i < 8; i++) {
            data_error_bits[i] = data_pattern[i] ^ data_read[i];
        }
        unsigned char *p;
        //int j;
        p = (unsigned char *)data_error_bits;
        for (i = 0; i < 8; i++) {
            diag_printf("Bytes %2d to %2d Write 0x%08x Read 0x%08x\n", 4*i, 4*i+3,
                        data_pattern[i], data_read[i]);
            diag_printf("Error Bits: ");
            post_output_binary((char)((data_error_bits[i]>>24)&0xff));
            diag_printf(" ");
            post_output_binary((char)((data_error_bits[i]>>16)&0xff));
            diag_printf(" ");
            post_output_binary((char)((data_error_bits[i]>>8)&0xff));
            diag_printf(" ");
            post_output_binary((char)((data_error_bits[i])&0xff));
            diag_printf("\n");
        }
    } else {
        diag_printf("Passed\n");
    }

    return result;
}

bool do_ddr_addr_once( unsigned int error_count_limit )
{
    unsigned int row, bank, col;
    unsigned int row1, row2, row3;
    //unsigned int seq_num;
    unsigned int i;
    unsigned int write_data;
    unsigned int read_data;
    //unsigned int data_pattern[8];
    unsigned int start_seq;
    unsigned error_count;

    error_count = 0;

    start_seq = 1;



    // write
    diag_printf("Write\n");
    //for (row = 0; row < 12; row++) {
    row = 0;
    for (row1 = 0; row1 < 1; row1++) {
        for (row2 = 0; row2 < 64; row2++) {
            for (row3 = 0 ; row3 < 16; row3++) {
                row++;
                for (bank = 0; bank < 4; bank++) {
                    //for (col = 0; col < 9; col++) {
                    for (col = 0; col < 512; col++) {
                        for (i = 0; i < 8; i++) {
                            HAL_WRITE_UINT32((DRAM_WRITE_READ_DATA0 + 4 * i), start_seq);
                            start_seq++;
                            //if (start_seq > 0x100000) start_seq++; // test failure case
                        }
                        write_data = (row<<19) +    /* row */
                            (bank<<17) +            /* bank */
                            (col<<7) +              /* col */
                            (1<<5);                 /* write command */
                        HAL_WRITE_UINT32(DRAM_ACCESS_COMMAND_REG, write_data);
                        i = 0;
                        read_data = 0;
                        while ((read_data & 1) == 1) {
                            if (i > 500) {

                            }
                            i++;
                            HAL_READ_UINT32(DRAM_ACCESS_COMMAND_REG, read_data);
                        }
                    }
                }
            }
            diag_printf(".");
        }
        diag_printf("\n");
    }


    // read-back and verify
    diag_printf("Verify\n");
    //for (row = 0; row < 12; row++) {
    start_seq = 1;
    row = 0;
    for (row1 = 0; row1 < 1; row1++) {
        for (row2 = 0; row2 < 64; row2++) {
            for (row3 = 0 ; row3 < 16; row3++) {
                row++;
                for (bank = 0; bank < 4; bank++) {
                    //for (col = 0; col < 9; col++) {
                    for (col = 0; col < 512; col++) {
                        write_data = (row<<19) +    /* row */
                            (bank<<17) +            /* bank */
                            (col<<7) +              /* col */
                            (3<<5);                 /* read command */
                        HAL_WRITE_UINT32(DRAM_ACCESS_COMMAND_REG, write_data);
                        i = 0;
                        read_data = 0;
                        while ((read_data & 1) == 1) {
                            if (i > 500) {
                                return false;
                            }
                            i++;
                            HAL_READ_UINT32(DRAM_ACCESS_COMMAND_REG, read_data);
                        }
                        for (i = 0; i < 8; i++) {
                            HAL_READ_UINT32((DRAM_WRITE_READ_DATA0 + 4 * i), read_data);
                            //diag_printf("data 0x%08x start_seq 0x%08x\n", read_data, start_seq);
                            start_seq++;
                            if (read_data != (start_seq - 1)) {
                                if (error_count == 0) diag_printf("\n");
                                error_count++;
                                diag_printf("Error %3d Failed at row 0x%x bank 0x%x col 0x%x\n", error_count, row, bank, col);
                                if (error_count >= error_count_limit) {
                                    diag_printf("Test aborted due to max number of errors reached!\n");
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
            diag_printf(".");
        }
        if (error_count == 0) {
            diag_printf("\n");
        }
    }
    if (error_count == 0)  return true;
    return false;
}

bool do_cmd_dll_once(
    unsigned int limit,
    unsigned int bank,
    unsigned int row,
    unsigned int count)
{
    bool result;
    result = true;
    if (!do_ddr_data_once(0x55aa55aa, bank, row, count)) result = false;
    if (!do_ddr_data_once(0xaa55aa55, bank, row, count)) result = false;
    if (count == 0) {
        if (!do_ddr_addr_once(limit)) result = false;
    }
    return result;
}


#define DDR_ROW         8096    // 13 bits
#define DDR_BANK        4       // 2 bits
#define DDR_COL         512     // 9 bits
#define MAX_PATTERN     3

unsigned int pre_pattern[MAX_PATTERN][8] =
{
    {0x00000011, 0x00000012, 0x00000013, 0x00000014, 0x00000015, 0x00000016, 0x00000017, 0x00000018},
    {0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00},
    {0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff, 0x00ff00ff},
};

extern STATUS olt_aal_packet_to_ddr_enable(unsigned char enable);

bool test_ddr_once(unsigned int pattern_idx, unsigned int usr_pattern)
{
    unsigned int row, bank, col;
    unsigned int i;
    unsigned int write_data;
    unsigned int read_data;
    unsigned int data_pattern[8];
    unsigned int repeat_time;


    if(MAX_PATTERN == pattern_idx)
    {
        for (i = 0; i < 8; i++) {
            data_pattern[i] = usr_pattern;
            HAL_WRITE_UINT32((DRAM_WRITE_READ_DATA0 + 4 * i), data_pattern[i]);
        }
    }
    else if(pattern_idx < MAX_PATTERN)
    {
        for (i = 0; i < 8; i++) {
            data_pattern[i] = pre_pattern[pattern_idx][i];
            HAL_WRITE_UINT32((DRAM_WRITE_READ_DATA0 + 4 * i), data_pattern[i]);
        }
    }
    else
    {
        diag_printf("invalid pattern\n");
        return false;
    }


    if(MAX_PATTERN == pattern_idx)
    {
        diag_printf("User defined pattern: 0x%08x\n", usr_pattern);
    }
    else
    {
        diag_printf("Predefined pattern #%d\n", pattern_idx+1);
    }

    for (i = 0; i < 8; i++) {
        diag_printf("0x%08x ", data_pattern[i]);
    }
    diag_printf("\n");

    // write
    for (row = 0; row < DDR_ROW; row++) {
        for (bank = 0; bank < DDR_BANK; bank++) {
            for (col = 0; col < DDR_COL; col++) {
                write_data = ((1<<row)<<19) +   /* row */
                    (bank<<17) +            /* bank */
                    ((1<<col)<<7) +         /* col */
                    (1<<5);                 /* write command */
                HAL_WRITE_UINT32(DRAM_ACCESS_COMMAND_REG, write_data);
                i = 0;
                read_data = 0;
                while ((read_data & 1) == 1) {
                    if (i > 500) {
                        return false;
                    }
                    i++;
                    HAL_READ_UINT32(DRAM_ACCESS_COMMAND_REG, read_data);
                }
            }
        }
    }

    // read-back and verify
    for (row = 0; row < DDR_ROW; row++) {
        for (bank = 0; bank < DDR_BANK; bank++) {
            for (col = 0; col < DDR_COL; col++) {
                write_data = ((1<<row)<<19) +   /* row */
                    (bank<<17) +            /* bank */
                    ((1<<col)<<7) +         /* col */
                    (3<<5);                 /* read command */
                HAL_WRITE_UINT32(DRAM_ACCESS_COMMAND_REG, write_data);
                i = 0;
                read_data = 0;
                while ((read_data & 1) == 1) {
                    if (i > 500) {
                        return false;
                    }
                    i++;
                    HAL_READ_UINT32(DRAM_ACCESS_COMMAND_REG, read_data);
                }

                for (i = 0; i < 8; i++) {
                    HAL_READ_UINT32((DRAM_WRITE_READ_DATA0 + 4 * i), read_data);
                    if (read_data != data_pattern[i]) {
                        diag_printf("\n");
                        diag_printf("Verification failed at row 0x%x bank 0x%x col 0x%x\n", row, bank, col);
                        diag_printf("Test aborted due to DDR error occured!\n");

                        diag_printf("Expected value is 0x%08X, while read value as 0x%08X.\n", data_pattern[i], read_data);
                        diag_printf("Perform another 10 times of reading:\n");
                        for(repeat_time = 0; repeat_time < 10; repeat_time++) {
                            HAL_READ_UINT32((DRAM_WRITE_READ_DATA0 + 4 * i), read_data);
                            diag_printf("[%2d] Read value as 0x%08X\n", repeat_time+1, read_data);
                        }

                        return false;
                    }
                }
            }
        }
    }

    return true;
}


void do_cmd_ddr_test(unsigned int repeat_count, unsigned int pattern_idx, unsigned int usr_pattern)
{
    int i;
    unsigned long __state;

    if(pattern_idx > MAX_PATTERN)
    {
        diag_printf("Invalid predefined pattern, DDR test QUITED.\n");
        return;
    }

    if(0 == repeat_count)
    {
        diag_printf("Count 0, DDR test not performed.\n");
        return;
    }

    // disable BM
    if(ERROR == olt_aal_packet_to_ddr_enable(0))
    {
        diag_printf("disable service failed\n");
        return;
    }

    hal_delay_us(5000); // wait to clean up

    HAL_DISABLE_INTERRUPTS(__state);
    diag_printf("Start destructive DDR test ...\n");

    for (i = 0; i < repeat_count; i++)
    {
        diag_printf("DDR test round %d\n", i+1);

        if (test_ddr_once(pattern_idx, usr_pattern))
        {
            diag_printf("DDR test PASSED.\n");
        }
        else
        {
            diag_printf("DDR test FAILED.\n");
        }
    }

    diag_printf("DDR test completed. \n");
    HAL_RESTORE_INTERRUPTS(__state);

    // enable BM
    if(ERROR == olt_aal_packet_to_ddr_enable(1))
    {
        diag_printf("recover service failed\n");
    }
}



#endif
