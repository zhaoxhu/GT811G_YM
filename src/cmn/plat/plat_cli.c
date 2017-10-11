/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#include "iros_config.h"
#include <stdio.h>
#include <stdlib.h>
#include "plat_common.h"
#include "registers.h"
#include "gpio.h"
#include "dma.h"
#include "cli.h"
#include "i2c.h"
#include "uart.h"
#include "mdio.h"
#include "cs_cmd.h"
#include "oam_api.h"
#include "cs_utils.h"
#include "aal.h"
#include "app_gpio.h"
#ifdef HAVE_TERMINAL_SERVER
#include "terminal_server.h"
#include "terminal_server_api.h"
#endif

extern bool  do_memcmp(const void *m1, const void *m2, unsigned int n);
extern cs_uint32 app_thread_count;
extern cs_uint32 intr_thread_count;
extern int tolower( int );


typedef struct
{
    cs_node node;
    cyg_thread_info info;
}thread_display_t;


sal_cmd_result_t core_dump_cmd_proc(int argc, char **argv)
{
    if (!cmd_cmp(argv[0], "core"))
        return SAL_CMD_FAIL;

    if (argc == 1)
    {
        cs_printf("core show  - show core dump in flash\n");
        cs_printf("core clear - clear core dump in flash\n");
    }
    else
    {
        if(cmd_cmp(argv[1], "show"))
        {
            show_core_in_flash();
        }
        else if(cmd_cmp(argv[1], "clear"))
        {
            clear_core_in_flash();
        }
        else
        {
            cs_printf("\nInvalid parameter!\n");
        }
    }

    return SAL_CMD_OK;
}

#define CS_TIMER_DEBUG
sal_cmd_result_t timer_cmd_proc(int argc, char **argv)
{
    if (!cmd_cmp(argv[0], "timer"))
        return SAL_CMD_FAIL;

    if (argc == 1)
    {
        cs_printf("timer show  - show timer info\n");
        cs_printf("timer usdelay [usec] [loop] - test usdelay\n");
    }
    else
    {
        if(cmd_cmp(argv[1], "show"))
        {
            cs_timer_show();
            return SAL_CMD_OK;
        }
        if(cmd_cmp(argv[1], "usdelay"))
        {
            cs_uint32 usdelay = (cs_uint32)iros_strtol(argv[2]);
            cs_uint32 loop = (cs_uint32)iros_strtol(argv[3]);
            cs_uint64 ticks_prev, ticks_now = 0;
            cs_uint32 idx;
            cs_printf("delay %d us %d times\n", usdelay, loop);

            ticks_prev = cyg_current_time();
            if(usdelay == 0)
                return SAL_CMD_FAIL;
            for(idx = 0; idx < loop; idx++)
            {
                HAL_DELAY_US(usdelay);
            }
            ticks_now = cyg_current_time();

            cs_printf("tick delta %lld\n", ticks_now - ticks_prev);
        }
        else if(cmd_cmp(argv[1], "test"))
        {
#ifdef CS_TIMER_DEBUG
            extern void timer_test();
            timer_test();
#endif
        }
        else
        {
            cs_printf("\nInvalid parameter!\n");
        }
    }

    return SAL_CMD_OK;
}

sal_cmd_result_t flash_cmd_proc(int argc, char **argv)
{
    if (!cmd_cmp(argv[0], "flash"))
        return SAL_CMD_FAIL;

    if (argc == 1)
    {
        cs_printf("flash part  - show flash partition info\n");
    }
    else
    {
        if(cmd_cmp(argv[1], "part"))
        {
            flash_partition_table_show();
        }
        else
        {
            cs_printf("\nInvalid parameter!\n");
        }
    }

    return SAL_CMD_OK;
}

extern void cs_gpio_cfg_dump(void);
int gpio_cmd_proc(int argc, char **argv)
{
    if (!cmd_cmp(argv[0], "gpio"))
        return 0;

    if (argc == 1)
    {
        cs_printf("gpio show - show GPIO cfg\n");
    }
    else
    {
        if(cmd_cmp(argv[1], "show"))
        {
            cs_gpio_cfg_dump();
        }
        else
        {
            unsigned char pin=0;
            cs_callback_context_t context;
            pin=(cs_uint8)iros_strtol(argv[1]);
            cs_plat_gpio_write(context, 0, 0, pin, 0);
            hal_delay_us(50000);
            cs_plat_gpio_write(context, 0, 0, pin, 1);

            cs_printf("\nReset Pin %d !\n",pin);
        }
    }

    return 1;
}

extern void cs_led_proc(cs_uint8 led, cs_uint8 color, cs_uint8 action);
extern void cs_led_all_set(cs_uint8 led_action);
int led_cmd_proc(int argc, char **argv)
{
    if (!cmd_cmp(argv[0], "led"))
        return 0;

    if (argc == 1)
    {
        cs_printf("led [led] [action] - set led\n");
        cs_printf("led all on/off - test all led\n");
    }
    else
    {
        if(cmd_cmp(argv[1], "all"))
        {
            if(argc < 3)
            {
                cs_printf("led all on/off - test all led\n");
                return 1;
            }
            if(cmd_cmp(argv[2], "on"))
                cs_led_all_set(LED_ON);
            else if (cmd_cmp(argv[2], "off"))
                cs_led_all_set(LED_OFF);
            else
                return 0;
        }
        else
        {
            cs_uint8 led;
            cs_uint8 action;

            led = iros_strtol(argv[1]);
            action = iros_strtol(argv[2]);

            cs_led_proc(led, LED_GREEN,action);
        }
    }

    return 1;
}



static int ssp_cmd_proc(int argc, char **argv)
{
    cs_callback_context_t    context;

    if (!(cmd_cmp(argv[0], "eeprom") || cmd_cmp(argv[0], "sflash")
          || cmd_cmp(argv[0], "serdes") || cmd_cmp(argv[0], "switch")))
        return 0;

    if (argc == 1)
    {
        cs_printf("per eeprom [read addr |write addr data | show] - EEPROM access\n");
        cs_printf("per serdes [read addr |write addr data] - serdes access\n");
        cs_printf("per switch [read addr |write addr data ] - switch access\n");
        cs_printf("per sflash [erase addr | write src dest len | read src dest len] - SFLASH access\n");
        cs_printf("           [brd src dest len | bwr src dest len]  - SFLASH access\n");
        cs_printf("           [test 0|1]  - SFLASH r/w perfermance test\n");
    }
    else
    {
        if(cmd_cmp(argv[0], "eeprom"))
        {
            if(cmd_cmp(argv[1], "write"))
            {
                cs_uint16 data_write=0;
                data_write = (cs_uint16)iros_strtol(argv[3]);
                cs_plat_ssp_eeprom_write(context,0,0,(cs_uint8)iros_strtol(argv[2]), (cs_uint8)sizeof(data_write),(char*)&data_write);
                cs_printf("EEPROM write: addr 0x%x -> 0x%x\n", iros_strtol(argv[2]), data_write);
            }
            else if(cmd_cmp(argv[1], "read"))
            {
                cs_uint16 data_read;
                cs_plat_ssp_eeprom_read(context,0,0,(cs_uint8)iros_strtol(argv[2]), (cs_uint8)sizeof(data_read),(char*)&data_read);
                cs_printf("EEPROM read: addr 0x%x = 0x%x\n", iros_strtol(argv[2]), data_read);
            }
            else if(cmd_cmp(argv[1], "show"))
            {
                ssp_eeprom_dump();
            }
            else
            {
                cs_printf("\nInvalid parameter!\n");
            }
        }
#ifdef HAVE_SERIAL_FLASH
        else if(cmd_cmp(argv[0], "sflash"))
        {
            if(cmd_cmp(argv[1], "write"))
            {
                cs_uint64 ticks = cs_current_time();
                sflash_program_buf_word(iros_strtol(argv[3]), iros_strtol(argv[2]),iros_strtol(argv[4]));
                ticks = cs_current_time() - ticks;
                if(do_memcmp((const void *) iros_strtol(argv[2]), (const void *) iros_strtol(argv[3]), iros_strtol(argv[4]))){
                        cs_printf("Normal  write verify passed\n");
                        cs_printf("Normal  write  dest 0x%x  src 0x%x len 0x%x ticks: %lld\n",
                            iros_strtol(argv[3]), iros_strtol(argv[2]), iros_strtol(argv[4]),ticks);
                }
                else{
                    cs_printf("Normal write verify failed\n");
                }
                cs_printf("src 0x%x  dest 0x%x len 0x%x ticks: %lld\n",
                                iros_strtol(argv[3]), iros_strtol(argv[2]), iros_strtol(argv[4]),ticks);

            }
            else if(cmd_cmp(argv[1], "erase"))
            {
                flash_erase_block(iros_strtol(argv[2]),0);
            }
            else if(cmd_cmp(argv[1], "read"))
            {
                cs_uint64 ticks = cs_current_time();
                sflash_read(iros_strtol(argv[3]), iros_strtol(argv[2]),iros_strtol(argv[4]));
                ticks = cs_current_time() - ticks;
                if(do_memcmp((const void *) iros_strtol(argv[2]), (const void *) iros_strtol(argv[3]), iros_strtol(argv[4]))){
                        cs_printf("Normal  Read verify passed\n");
                        cs_printf("Normal  Read dest 0x%x  src 0x%x len 0x%x ticks: %lld\n",
                            iros_strtol(argv[3]), iros_strtol(argv[2]), iros_strtol(argv[4]),ticks);
                    }
                    else{
                        cs_printf("Normal Read verify failed\n");
                    }
                cs_printf("src 0x%x  dest 0x%x len 0x%x ticks: %lld\n",
                                iros_strtol(argv[2]), iros_strtol(argv[3]), iros_strtol(argv[4]),ticks);
            }
            else if(cmd_cmp(argv[1], "brd") && 5 == argc)
            {
                    cs_uint64 ticks = cs_current_time();
                    sflash_block_read(iros_strtol(argv[3]), iros_strtol(argv[2]),iros_strtol(argv[4]));
                    ticks = cs_current_time() - ticks;
                    if(do_memcmp((const void *) iros_strtol(argv[2]), (const void *) iros_strtol(argv[3]), iros_strtol(argv[4]))){
                        cs_printf("Block Read verify passed\n");
                        cs_printf("Block  Read  test %4d  KB Data dest 0x%x  src 0x%x len 0x%x ticks: %lld\n",
                            iros_strtol(argv[4])/0x400,iros_strtol(argv[3]), iros_strtol(argv[2]), iros_strtol(argv[4]),ticks);
                    }
                    else{
                        cs_printf("Block Read verify failed\n");
                    }
                    cs_printf("src 0x%x  dest 0x%x len 0x%x ticks: %lld\n",
                    iros_strtol(argv[2]), iros_strtol(argv[3]), iros_strtol(argv[4]),ticks);

            }
             else if(cmd_cmp(argv[1], "bwr") && 5 == argc)
            {
                    cs_uint64 ticks = cs_current_time();
                    flash_program_buf(iros_strtol(argv[3]), iros_strtol(argv[2]),iros_strtol(argv[4]),0,0);
                    ticks = cs_current_time() - ticks;
                    if(do_memcmp((const void *) iros_strtol(argv[2]), (const void *) iros_strtol(argv[3]), iros_strtol(argv[4]))){
                        cs_printf("Block write verify passed\n");
                        cs_printf("Block write  test %4d  KB Data dest 0x%x  src 0x%x len 0x%x ticks: %lld\n",
                            iros_strtol(argv[4])/0x400,iros_strtol(argv[3]), iros_strtol(argv[2]), iros_strtol(argv[4]),ticks);
                    }
                    else{
                        cs_printf("Block write verify failed\n");
                    }
                    cs_printf("src 0x%x  dest 0x%x len 0x%x ticks: %lld\n",
                    iros_strtol(argv[2]), iros_strtol(argv[3]), iros_strtol(argv[4]),ticks);

            }
            else if(cmd_cmp(argv[1], "test"))
            {
                extern bool sflash_detail_info;
                sflash_detail_info=iros_strtol(argv[2]);
                sflash_test();
            }
            else
            {
               cs_printf("per sflash [clear addr | write src dest len | read src dest len] - SFLASH access\n");
               cs_printf("           [brd src dest len | bwr src dest len]  - SFLASH access\n");
               cs_printf("           [test 0|1]  - SFLASH r/w perfermance test\n");
            }
        }
#endif /* #ifdef HAVE_SERIAL_FLASH */
        else if(cmd_cmp(argv[0], "serdes"))
        {
            if(cmd_cmp(argv[1], "write"))
            {
                cs_printf("Serdes write addr: 0x%02x data 0x%x\n", iros_strtol(argv[2]), iros_strtol(argv[3]));
                cs_plat_ssp_serdes_write(context,0,0,iros_strtol(argv[2]),iros_strtol(argv[3]));
            }
            else if(cmd_cmp(argv[1], "read"))
            {
                cs_uint8 rdata = 0;

                cs_plat_ssp_serdes_read(context,0,0,iros_strtol(argv[2]),(cs_uint8 *)&rdata);
                cs_printf("Serdes read from 0x%02x data:0x%02x\n", iros_strtol(argv[2]),rdata);

            }
            else
            {
                cs_printf("\nInvalid parameter!\n");
            }
        }
        else if(cmd_cmp(argv[0], "switch"))
        {
            if(cmd_cmp(argv[1], "write"))
            {
                cs_printf("switch write addr: 0x%02x data 0x%x\n", iros_strtol(argv[2]), iros_strtol(argv[3]));
                cs_plat_ssp_switch_write(context,0,0,iros_strtol(argv[2]),iros_strtol(argv[3]));
            }
            else if(cmd_cmp(argv[1], "read"))
            {
                cs_uint32 rdata = 0;

                cs_plat_ssp_switch_read(context,0,0,iros_strtol(argv[2]),(cs_uint32 *)&rdata);
                cs_printf("switch read from 0x%02x data:0x%02x\n", iros_strtol(argv[2]),rdata);

            }
            else
            {
                cs_printf("\nInvalid parameter!\n");
                cs_printf("per switch [read addr |write addr data ] - switch access\n");
            }
        }
#if 0
        else if(cmd_cmp(argv[1], "slic"))
        {
            if(cmd_cmp(argv[2], "write"))
            {
                cs_printf("SLIC write: 0x%02x -> 0x%x\n", iros_strtol(argv[3]), iros_strtol(argv[4]));
                cs_plat_ssp_slic_write(context,0,0,iros_strtol(argv[3]));
                cs_plat_ssp_slic_write(context,0,0,iros_strtol(argv[4]));
                cs_printf("SLIC write done\n");
            }
            else if(cmd_cmp(argv[2], "read"))
            {
                cs_uint8 rdata = 0;
                cs_uint8 loop;
                cs_uint32 raddr;
                cs_uint32 data_cnt;

                raddr = iros_strtol(argv[3]);
                data_cnt = iros_strtol(argv[4]);
                cs_printf("SLIC read from 0x%02x\n", raddr);

                cs_plat_ssp_slic_write(context,0,0,raddr);

                cs_printf("Data read: ");
                for(loop = 0; loop < data_cnt; loop++)
                {
                    cs_plat_ssp_slic_read(context,0,0,&rdata);
                    cs_printf("0x%02x ", rdata);
                }
                cs_printf("\n");
            }
            else if(cmd_cmp(argv[2], "reset"))
            {
                ssp_slic_reset();
                cs_printf("reset SLIC chip\n");
            }
            else
            {
                cs_printf("\nInvalid parameter!\n");
            }
        }
#endif
        else
        {
           cs_printf("per eeprom [read addr |write addr data | show] - EEPROM access\n");
           cs_printf("per serdes [read addr |write addr data] - serdes access\n");
           cs_printf("per switch [read addr |write addr data ] - switch access\n");
           cs_printf("per sflash [clear addr | write src dest len | read src dest len] - SFLASH access\n");
           cs_printf("           [brd src dest len | bwr src dest len]  - SFLASH access\n");
           cs_printf("           [test 0|1]  - SFLASH r/w perfermance test\n");
        }
    }

    return 1;
}


static int dma_cmd_proc(int argc, char **argv)
{
    if (!cmd_cmp(argv[0], "dma") && !cmd_cmp(argv[0], "esdma"))
        return 0;
#ifdef HAVE_DMA_TEST
    if (argc == 1)
    {
        cs_printf("dma ctrl BYPASS DPID COS - DMA set bypass(0-lookup,1-bypass), DPID(0-GE,1-EPON), cos\n");
        cs_printf("dma pkt PATTERN LEN - DMA define packet with pattern\n");
        cs_printf("dma copy ADDR_SRC ADDR_DEST LEN - DMA copy mem from SRC to DEST\n");
        cs_printf("dma data [fixed | dynamic] - set pkt data pattern\n");
        cs_printf("dma loop [INTERVAL] [DIR] - loopback on PON side, interval in ms, dir 0-tx, 1-rx, 2-bi\n");
        cs_printf("dma check [NUM] [SEQ] [CONTENT] [DUMP_ERR_PKT] [DUMP_ALL] - enable/disable pkt check, 0-disable, 1-enable\n");
        cs_printf("dma len PKT_LEN SEQ_LEN - set pkt length and seq. id length, to check byte swap\n");
        cs_printf("dma stat [dump | clear] - show or clear tx/rx counter\n");
        cs_printf("esdma setup [int | ext] [LEN]- ESDMA setup buffer, internal or external SDRAM, len - buffer len in bytes\n");
        cs_printf("esdma clear - ESDMA clear buffer\n");
        cs_printf("esdma read BUF TOKEN - ESDMA read pkt\n");
        cs_printf("esdma buf IDX - ESDMA dump all pkt in buffer idx 0/1\n");
    }
    else
    {
        if(cmd_cmp(argv[0], "esdma"))
        {
            if(cmd_cmp(argv[1], "setup"))
            {
                if(cmd_cmp(argv[2], "ext"))
                {
                    cs_uint32 buf_len = iros_strtol(argv[4]);
                    dma_loop_setup_buffer(buf_len);
                }
                else if(cmd_cmp(argv[2], "int"))
                {
                    cs_uint8 *buf_addr_0;
                    cs_uint8 *buf_addr_1;
                    cs_uint32 buf_token, buf_len;
                    buf_len = iros_strtol(argv[3]);
                    buf_token = buf_len / ESDMA_TOKEN_IN_BYTES;

                    buf_addr_0 = (void *)INTERNAL_SRAM_START;
                    buf_addr_1 = (void *)(INTERNAL_SRAM_START + buf_len);

                    esdma_setup(buf_addr_0, buf_token, buf_addr_1, buf_token);
                    cs_printf("ESDMA setup internal mem: 0: %p T 0x%x, 1: %p, T 0x%x\n", buf_addr_0, buf_token, buf_addr_1, buf_token);
                }
                else
                {
                    cs_printf("\nInvalid parameter!\n");
                }
            }
            else if(cmd_cmp(argv[1], "clear"))
            {
                cs_uint8 *buf_addr_0;
                cs_uint8 *buf_addr_1;
                cs_uint32 buf_token, buf_len;

                buf_token = esdma_buf_desc[ESDMA_BUF_ID_0].token_num;
                buf_len = buf_token * ESDMA_TOKEN_IN_BYTES;
                buf_addr_0 = (void*)esdma_buf_desc[ESDMA_BUF_ID_0].address;
                buf_addr_1 = (void*)esdma_buf_desc[ESDMA_BUF_ID_1].address;

                memset(buf_addr_0, 0, buf_len);
                memset(buf_addr_1, 0, buf_len);
                cs_printf("ESDMA clear buf: 0: %p Bytes 0x%x, 1: %p, Bytes 0x%x\n", buf_addr_0, buf_len, buf_addr_1, buf_len);
            }
            else if(cmd_cmp(argv[1], "read"))
            {
                cs_uint8 buf_idx;
                cs_uint32 token_cnt;
                buf_idx = (cs_uint8)iros_strtol(argv[2]);
                token_cnt = iros_strtol(argv[3]);
                cs_printf("ESDMA read pkt: buf_idx 0x%x, tok_cnt 0x%x\n", buf_idx, token_cnt);
                esdma_get_buf_token(&buf_idx, &token_cnt);
                cs_printf("after read pkt: buf_idx 0x%x, tok_cnt 0x%x\n", buf_idx, token_cnt);
            }
            else if(cmd_cmp(argv[1], "buf"))
            {
                cs_uint8 buf_idx;
                buf_idx = (cs_uint8)iros_strtol(argv[2]);
                cs_printf("ESDMA dump buf 0x%x:\n", buf_idx);
                esdma_dump_buf(buf_idx);
            }
            else
            {
                cs_printf("\nInvalid parameter!\n");
            }
            return 1;
        }

        if(cmd_cmp(argv[0], "dma"))
        {
            if(cmd_cmp(argv[1], "ctrl"))
            {
                cs_printf("DMA ctrl: bypass %d, dpid %d, cos %d\n",
                    (cs_uint8)iros_strtol(argv[2]),
                    (cs_uint8)iros_strtol(argv[3]),
                    (cs_uint8)iros_strtol(argv[4]));
                dma_pkt_ctrl((cs_uint8)iros_strtol(argv[2]), (cs_uint8)iros_strtol(argv[3]), (cs_uint8)iros_strtol(argv[4]));
            }
            else if(cmd_cmp(argv[1], "loop"))
            {
                cs_printf("DMA loopback test: interval %d, dir %d\n",
                    iros_strtol(argv[2]),
                    iros_strtol(argv[3]));
                dma_loop_init(iros_strtol(argv[2]), iros_strtol(argv[3]));
            }
            else if(cmd_cmp(argv[1], "stat"))
            {
                if(cmd_cmp(argv[2], "dump"))
                {
                    dma_loop_stat_dump();
                }
                else if(cmd_cmp(argv[2], "clear"))
                {
                    dma_loop_stat_clear();
                }
                else
                {
                    cs_printf("\nInvalid parameter!\n");
                }
            }
            else if(cmd_cmp(argv[1], "check"))
            {
                cs_printf("DMA loopback check: num in one loop %d, seq check %d, content check %d, content dump %d, dump all %d\n",
                    (cs_uint8)iros_strtol(argv[2]),
                    (cs_uint8)iros_strtol(argv[3]),
                    (cs_uint8)iros_strtol(argv[4]),
                    (cs_uint8)iros_strtol(argv[5]),
                    (cs_uint8)iros_strtol(argv[6]));
                dma_loop_check(iros_strtol(argv[2]), iros_strtol(argv[3]), iros_strtol(argv[4]), iros_strtol(argv[5]), iros_strtol(argv[6]));
            }
            else if(cmd_cmp(argv[1], "len"))
            {
                cs_printf("DMA loopback len: pkt %d seq %d\n", iros_strtol(argv[2]), iros_strtol(argv[3]));
                dma_loop_len(iros_strtol(argv[2]), iros_strtol(argv[3]));
            }
            else if(cmd_cmp(argv[1], "data"))
            {
                if(cmd_cmp(argv[2], "fixed"))
                {
                    dma_loop_data_pattern(0);
                }
                else if(cmd_cmp(argv[2], "dynamic"))
                {
                    dma_loop_data_pattern(1);
                }
                else
                {
                    cs_printf("\nInvalid parameter!\n");
                }
            }
            else
            {
                cs_printf("\nInvalid parameter!\n");
            }
                return 1;
        }
    }
#endif
    return 1;
}

static int i2c_cmd_proc(int argc, char **argv)
{
    if (!cmd_cmp(argv[0], "i2c"))
        return 0;

    if (argc == 1)
    {
        cs_printf("i2c read slave_address offset length\n");
        cs_printf("i2c write slave_address offset data\n");
    }
    else
    {
        if(cmd_cmp(argv[1], "read"))
        {
              if(argc == 5)
              {
                    unsigned int i;
                    char *p_buf = NULL;
                    cs_uint16 length = iros_strtol(argv[4]);

                    p_buf = iros_malloc(IROS_MID_SYS,length);
                    if(p_buf == NULL)
                        return 1;
                    memset((void*)p_buf, 0, length);
                    if(p_buf == NULL)
                    {
                            cs_printf("i2c test malloc failed\n");
                            return 1;
                    }
                    i2c_read(iros_strtol(argv[2]),iros_strtol(argv[3]), iros_strtol(argv[4]),p_buf);
                    cs_printf("I2C read data:");
                    for(i = 0; i < length; i++)
                    {
                            if(i%16 == 0)
                                cs_printf("\n");
                            cs_printf("0x%02x  ",p_buf[i]);
                    }
                    cs_printf("\n");
                    iros_free(p_buf);
              }
            else
            {
                    cs_printf("Invalid Command\n");
                    cs_printf("i2c read slave_address offset length\n");
            }
        }
        else if(cmd_cmp(argv[1], "write"))
        {
              if(argc == 5)
              {
                    cyg_uint8 data;
                    data = iros_strtol(argv[4]);
                    i2c_write(iros_strtol(argv[2]),iros_strtol(argv[3]), 1,(unsigned char *)&data);

                    i2c_read(iros_strtol(argv[2]),iros_strtol(argv[3]),1,&data);
                    if(data == iros_strtol(argv[4]))
                    {
                        cs_printf("I2C write data 0x%02x success.\n",iros_strtol(argv[4]));
                    }
                    else
                    {
                        cs_printf("I2C write data 0x%02x failed.\n",iros_strtol(argv[4]));
                        cs_printf("I2C read back data 0x%02x.\n",data);
                    }
              }
            else
            {
                    cs_printf("Invalid Command\n");
                    cs_printf("i2c write slave_address offset data\n");
            }
        }
        else
        {
                cs_printf("Invalid Command\n");
                cs_printf("i2c read slave_address offset length\n");
                cs_printf("i2c write slave_address offset data\n");
        }
    }

    return 1;
}
static void do_uart_help(void)
{
        cs_printf("uart cfg <id> <baudrate> <databits> <parity> <mode> <duplex>\n");
        cs_printf("            --<id>: 0~4,0 for uart0, 4 for uart4\n");
        cs_printf("            --<baudrate>: 1~312500\n");
        cs_printf("            --<databits>: 5/6/7/8\n");
        cs_printf("            --<parity>: 0-None 1-Odd 2-Even\n");
        cs_printf("            --<mode>: 1-polling 2-interrupt\n");
        cs_printf("            --<duplex>: 0-full-duplex 1-half-duplex\n");
}
static int uart_cmd_proc(int argc, char **argv)
{
    if (!cmd_cmp(argv[0], "uart"))
        return 0;

    if (argc == 1)
    {
        do_uart_help();
    }
    else
    {
        if(cmd_cmp(argv[1], "cfg"))
        {
              if(argc == 8)
              {
                    uart_config_t uart_cfg;

                    uart_cfg.uart_id = iros_strtol(argv[2]);
                    uart_cfg.baud_rate = iros_strtol(argv[3]);
                    uart_cfg.data_bits = iros_strtol(argv[4]);
                    uart_cfg.parity = iros_strtol(argv[5]);
                    uart_cfg.mode = iros_strtol(argv[6]);
                    uart_cfg.duplex = iros_strtol(argv[7]);
                    uart_init((uart_config_t *)&uart_cfg);
              }
            else
            {
                    cs_printf("Invalid Command\n");
                    do_uart_help();
            }
        }
        else if(cmd_cmp(argv[1], "test"))
        {
                if(argc == 3)
                {
                    unsigned char   uart_id;

                    uart_id = iros_strtol(argv[2]);
                    uart_electricity_meter_test(uart_id);
                }
                else
                {
                    cs_printf("Invalid Command\n");
                    do_uart_help();
                }
        }
        else
        {
                cs_printf("Invalid Command\n");
                do_uart_help();
        }
    }

    return 1;
}

static int mdio_cmd_proc(int argc, char **argv)
{
    cyg_uint32 rc;
    cyg_uint16 value;
    cs_callback_context_t    context;

    if (!cmd_cmp(argv[0], "mdio"))
        return 0;

    if (argc == 1)
    {
        cs_printf("mdio read device register\n");
        cs_printf("mdio write device register data\n");
    }
    else
    {
        if(cmd_cmp(argv[1], "read"))
        {
             if(argc == 4)
            {
                    rc = cs_plat_mdio_data_get(context,0,0,iros_strtol(argv[2]),
                                iros_strtol(argv[3]), &value);
                    cs_printf("MDIO read dev%d reg%d=0x%x rc=%x\n", iros_strtol(argv[2]),
                            iros_strtol(argv[3]), value, rc);
            }

        }
        else if(cmd_cmp(argv[1], "write"))
        {
            if(argc == 5)
            {
                    rc = cs_plat_mdio_data_set(context,0,0,iros_strtol(argv[2]), iros_strtol(argv[3]), iros_strtol(argv[4]));
                    cs_printf("MDIO write dev%d reg%d=0x%x rc=%x\n",
                        iros_strtol(argv[2]), iros_strtol(argv[3]), iros_strtol(argv[4]), rc);
            }
        }
        else
        {
            cs_printf("Invalid Command\n");
            cs_printf("mdio read device register\n");
            cs_printf("mdio write device register data\n");
        }
    }

    return 1;
}

sal_cmd_result_t log_cmd_proc(int argc , char **argv)
{
    if(argc < 1)
        return SAL_CMD_OK;

    if (!cmd_cmp(argv[0], "log"))
        return SAL_CMD_OK;

    if(argc < 2)
    {
        cs_printf("log show all             - dump all logs\n");
#ifdef HAVE_LOG_FILE
        cs_printf("log show sys             - dump all system logs in flash\n");
#endif
        cs_printf("log show new <num>       - dump the latest 'num' logs\n");
        cs_printf("log show old <num>       - dump the oldest 'num' logs\n");
        cs_printf("log show mod <modid>     - dump all logs of the specific module\n");
        cs_printf("log show level <modid>   - dump all logs of the specific level\n");
        cs_printf("log prtl <level> [modid] - set print level\n");
        cs_printf("log recl <level> [modid] - set record level\n");
        cs_printf("log clr                  - clear all logs\n");
        cs_printf("log remote <level> [modid] - set remote log level\n");
        cs_printf("log remoteip <ip>          - Set remote log server ip\n");
        return SAL_CMD_OK;
    }

    if(cmd_cmp(argv[1] , "show"))
    {
        if(argc == 2 || cmd_cmp(argv[2] , "all"))
        {
            cs_log_dump_all();
            return SAL_CMD_OK;
        }

#ifdef HAVE_LOG_FILE
        if(cmd_cmp(argv[2] , "sys") && argc == 3)
        {
            cs_sys_log_dump();
            return SAL_CMD_OK;
        }
#endif

        if(cmd_cmp(argv[2] , "new") && argc == 4)
        {
            cs_log_dump_latest((cs_uint32)atoi(argv[3]));
            return SAL_CMD_OK;
        }

        if(cmd_cmp(argv[2] , "old") && argc == 4)
        {
            cs_log_dump_oldest((cs_uint32)atoi(argv[3]));
            return SAL_CMD_OK;
        }

        if(cmd_cmp(argv[2] , "mod") && argc == 4)
        {
            cs_log_dump_by_modid((cs_uint32)atoi(argv[3]));
            return SAL_CMD_OK;
        }

        if(cmd_cmp(argv[2] , "level") && argc == 4)
        {
            cs_log_dump_by_level((cs_uint32)atoi(argv[3]));
            return SAL_CMD_OK;
        }
    }

    if(cmd_cmp(argv[1] , "prtl"))
    {
        if(argc != 3 && argc != 4)
        {
            cs_printf("Syntax : log prtl <level> [modid]\n");
            return SAL_CMD_OK;
        }

        if(argc == 3)
            cs_log_print_level_set_all((cs_uint32)atoi(argv[2]));
        else
            cs_log_print_level_set((cs_uint32)atoi(argv[3]) , (cs_uint32)atoi(argv[2]));
        return SAL_CMD_OK;
    }

    if(cmd_cmp(argv[1] , "recl"))
    {
        if(argc != 3 && argc != 4)
        {
            cs_printf("Syntax : log recl <level> [modid]\n");
            return SAL_CMD_OK;
        }

        if(argc == 3)
            cs_log_record_level_set_all((cs_uint32)atoi(argv[2]));
        else
            cs_log_record_level_set((cs_uint32)atoi(argv[3]) , (cs_uint32)atoi(argv[2]));
        return SAL_CMD_OK;
    }

    if(cmd_cmp(argv[1] , "clr") && argc == 2)
    {
        cs_log_clear_all();
        return SAL_CMD_OK;
    }

#ifdef HAVE_LOG_FILE
    if(argc == 3 && cmd_cmp(argv[1] , "clr") && cmd_cmp(argv[2] , "sys"))
    {
        cs_sys_log_clr();
        return SAL_CMD_OK;
    }
#endif
    if(cmd_cmp(argv[1] , "remote"))
    {
        if(argc != 3 && argc != 4)
        {
            cs_printf("Syntax : log remote <level> [modid]\n");
            return SAL_CMD_OK;
        }

        if(argc == 3)
            cs_log_remote_level_set_all((cs_uint32)atoi(argv[2]));
        else
            cs_log_remote_level_set((cs_uint32)atoi(argv[3]) , (cs_uint32)atoi(argv[2]));
        return SAL_CMD_OK;
    }

    if(cmd_cmp(argv[1] , "remoteip") && argc == 3)
    {
        cs_uint32 ip=DEF_REMOTE_SYSLOG_IP;
        cs_str2ip(argv[2],&ip);
        cs_log_remoteip_set(ip);
        return SAL_CMD_OK;
    }

    return SAL_CMD_OK;
}

sal_cmd_result_t memory_cmd_proc(int argc, char **argv)
{
    if (!cmd_cmp(argv[0], "mem"))
        return SAL_CMD_OK;

    if(argc <= 1 || argc > 3 || !cmd_cmp(argv[1],"show"))
    {
        cs_printf("mem show         - dump all memory allocation information\n");
        cs_printf("mem show <modid> - dump specific module memory allocation information\n");
        cs_printf("mem show pool    - dump all memory pool information\n");
        return SAL_CMD_OK;
    }
    else
    {
        if(argc == 2)
            cs_sys_module_show(0xFFFF);
        else
        {
            if(cmd_cmp(argv[2] , "pool"))
                cs_pool_show(CS_INVALID_POOL_ID);
            else
                cs_sys_module_show(atoi(argv[2]));
        }
        return SAL_CMD_OK;
    }

    return SAL_CMD_OK;
}

static int watchdog_cmd_proc(int argc, char **argv)
{
    cs_uint32 value;

    if (!cmd_cmp(argv[0], "wdt"))
        return 0;

    if (argc == 1)
    {
        cs_printf("per wdt enable - enabel watchdog\n");
        cs_printf("per wdt disable - disable watchdog\n");
        cs_printf("per wdt set value- set watchdog timeout value(second)\n");
        cs_printf("per wdt get - get current watchdog timeout value(second)\n");
    }
    else
    {
        if(cmd_cmp(argv[1], "enable"))
        {
            watchdog_enable();
        }
        else if(cmd_cmp(argv[1], "disable"))
        {
            watchdog_disable();
        }
        else if(cmd_cmp(argv[1], "get"))
        {
            watchdog_get_time((cs_uint32 *)&value);
            cs_printf("watchdog timeout value is %d second\n",value);
        }
        else if(cmd_cmp(argv[1], "set"))
        {
           if(argc == 3)
               watchdog_set_time(iros_strtol(argv[2]));
           else
               cs_printf("per wdt set value- set watchdog timeout value(second)\n");
        }
        else
        {
            cs_printf("Invalid Command\n");
            cs_printf("per wdt enable - enabel watchdog\n");
            cs_printf("per wdt disable - disable watchdog\n");
            cs_printf("per wdt set value- set watchdog timeout value(second)\n");
            cs_printf("per wdt get - get current watchdog timeout value(second)\n");
        }
    }

    return 1;
}


int per_cmd_proc(int argc, char **argv)
{
    if(ssp_cmd_proc(argc, argv))
    {
        return 0;
    }

    if(watchdog_cmd_proc(argc, argv))
    {
        return 0;
    }

    if(dma_cmd_proc(argc, argv))
    {
        return 0;
    }

    if(i2c_cmd_proc(argc, argv))
    {
        return 0;
    }

    if(uart_cmd_proc(argc, argv))
    {
        return 0;
    }

    if(mdio_cmd_proc(argc, argv))
    {
        return 0;
    }

    if(gpio_cmd_proc(argc, argv))
    {
        return 0;
    }

    if(led_cmd_proc(argc, argv))
    {
        return 0;
    }

    return 1;
}
extern void do_show_version();
extern void do_set_memory(char **argv);
extern void do_dump_memory(char **argv, int argc);
sal_cmd_result_t peri_cmd_proc(int argc , char **argv)
{
    argc --;
    argv = (char **)&argv[1];
    per_cmd_proc(argc,argv);
    return SAL_CMD_OK;
}

sal_cmd_result_t show_cmd_thread(int argc, char **argv)
{
    if(argc == 3)
    {
        if (cmd_cmp(argv[2] , "cpuload"))
        {
#ifdef HAVE_CPULOAD_PER_THREAD
            cs_thread_cpu_load_show();
#endif
            return SAL_CMD_OK;
        }

        if (cmd_cmp(argv[2] , "stack"))
        {
            show_threads_info(NULL);
            return SAL_CMD_OK;
        }
    }

    if(argc == 2)
    {
        cs_thread_show();
        return SAL_CMD_OK;
    }

    return SAL_CMD_OK;
}

sal_cmd_result_t show_cmd_system(int argc, char **argv)
{
    if(argc == 3 && cmd_cmp(argv[2] , "cpuload"))
    {
        dump_cpuload_info(NULL);
        return SAL_CMD_OK;
    }

    if(argc == 2)
        show_system_status();
    return SAL_CMD_OK;
}

sal_cmd_result_t show_cmd_msgq(int argc, char **argv)
{
    cs_queue_show(-1);
    return SAL_CMD_OK;
}

sal_cmd_result_t show_cmd_pri_msgq(int argc, char **argv)
{
    if(argc == 3)
    {
        cs_pri_queue_show(iros_strtol(argv[2]));
    }
    else
    {
        cs_pri_queue_show(-1);
    }
    return SAL_CMD_OK;
}


#ifdef HAVE_KT_OAM
extern cs_uint8 g_opm_flag;
extern cs_uint8 g_opm_gpio_polarity;
extern cs_uint8 g_opm_poll_period;
static void opm_cmd_proc(int argc, char **argv)
{
    switch(tolower(argv[2][0]))
    {
          case 't':
          {
                 extern void oam_build_opm_event(cs_uint8 status, cs_uint8 flag);
                 cs_uint8 action = iros_strtol(argv[3]);

                 if(action) {
                         action = 1;
                  }
                  oam_build_opm_event(2 - action, 0);
           }
              break;

          case 'd':
          {
                  cs_uint8 num = iros_strtol(argv[3]);
                   if(num == 0) {
                           num = 5;
                   }

                   g_opm_poll_period = num;
           }
              break;

           case 'f':
           {
                    cs_uint8 flag = iros_strtol(argv[3]);
                    if(flag) {
                          flag = 1;
                     }

                    g_opm_flag = flag;
           }
                break;

           case 'p':
           {
                    cs_uint8 polarity = iros_strtol(argv[3]);
                    if(polarity) {
                         polarity = 1;
                      }

                    g_opm_gpio_polarity = polarity;
            }
                 break;

            case 's':
            {
                    cs_uint8 status;
                    cs_uint8 unit, dec;
                    cs_callback_context_t  context;

                    unit = g_opm_poll_period/10;
                    dec = g_opm_poll_period - unit * 10;

                     cs_plat_gpio_read(context,0,0,13, &status);
                     cs_printf("\nFlag = %d(%s)\n", g_opm_flag, g_opm_flag ? "Normal" : "Failure");
                     cs_printf("Read gpio = %d\n", status);
                     cs_printf("Delay = %d.%d seconds\n", unit, dec);
              }
                   break;

             default:
                   break;
     }
}
#endif

#ifdef HAVE_TERMINAL_SERVER
static void do_term_server_help(void)
{
    cs_printf("ts enable <uart> <baudrate> <databits> <parity>");
    cs_printf(" <proto> <port> <timeout> <min> <max> <resp>\n");
    cs_printf("            --<uart>: 1 for uart1, 2 for uart2...4 for uart4\n");
    cs_printf("            --<baudrate>: 300~115200\n");
    cs_printf("            --<databits>: 7/8\n");
    cs_printf("            --<parity>: 0-None 1-Odd 2-Even\n");
    cs_printf("            --<proto>: 17 for UDP, 6 for TCP\n");
    cs_printf("            --<port>: 1024~65535\n");
    cs_printf("            --<timeout>: 0~65535s for remote timeout, 0-never timeout\n");
    cs_printf("            --<min>: 40~<max> for minimal payload length\n");
    cs_printf("            --<max>: 512~1400 for maximum playload length\n");
    cs_printf("            --<resp>: 25~1000ms for maximum reponse time\n");
    cs_printf("            ---Used to create a terminal server\n");
    cs_printf("ts disable <uart>  --disable terminal server\n");
    cs_printf("ts dbg             --enable/disable terminal server debug info\n");
    cs_printf("ts reset <uart>    --reset terminal server wait for new connect \n");
    cs_printf("ts save <uart>     --save configuration into flash \n");
    cs_printf("ts show <uart>     --show terminal server status\n");
}

static int term_server_cmd_proc(int argc, char **argv)
{
    cs_callback_context_t context;

    if(argc == 1)
    {
          do_term_server_help();
    }
    else if(tolower(argv[1][0]) == 's' && tolower(argv[1][1]) == 'h') {
            if(argc == 3)
            {
                if(ts_uart_id_check(iros_strtol(argv[2])))
                   cs_printf("Invalid UART ID\n\r");
                else
                   ts_show(iros_strtol(argv[2]));
            }
            else
            {
                do_term_server_help();
            }
    }
    else if(tolower(argv[1][0]) == 'e' && tolower(argv[1][1]) == 'n') {
            if(argc == 12)
            {
                term_server_config_t Term_serv_cfg;

                memset(&Term_serv_cfg,0,sizeof(term_server_config_t));
                Term_serv_cfg.init_flag = VALID_CONFIG_FLAG;
                Term_serv_cfg.client_closed_flag = 1;
                Term_serv_cfg.uart_cfg.uart_id = iros_strtol(argv[2]);
                Term_serv_cfg.uart_cfg.baud_rate = iros_strtol(argv[3]);
                Term_serv_cfg.uart_cfg.data_bits = iros_strtol(argv[4]);
                Term_serv_cfg.uart_cfg.parity = iros_strtol(argv[5]);
                Term_serv_cfg.uart_cfg.mode = SDL_UART_MODE_INTR;
                Term_serv_cfg.uart_cfg.duplex = SDL_UART_HALF_DUPLEX;
                Term_serv_cfg.uart_cfg.stop_bits = 1;
                Term_serv_cfg.proto_type = iros_strtol(argv[6]);
                Term_serv_cfg.proto_port = iros_strtol(argv[7]);
                Term_serv_cfg.client_timeout = iros_strtol(argv[8]);
                Term_serv_cfg.min_payload = iros_strtol(argv[9]);
                Term_serv_cfg.max_payload = iros_strtol(argv[10]);
                Term_serv_cfg.max_res_time = iros_strtol(argv[11]);
                if(CS_E_OK == ts_uart_id_check(iros_strtol(argv[2])))
                {
                    if(epon_request_onu_ts_enable(context,0,0,
                        Term_serv_cfg.uart_cfg.uart_id,
                        (term_server_config_t *)&Term_serv_cfg))
                        diag_printf("Terminal Server %d create failed\n\r",
                                            UART_TO_TERM_SERV(iros_strtol(argv[2])));
                    else
                        diag_printf("Terminal Server %d create sucess\n\r",
                                            UART_TO_TERM_SERV(iros_strtol(argv[2])));
                }
                else
                    diag_printf("Invalid UART ID [1~4]\n\r");
            }
            else
            {
                do_term_server_help();
            }
    }
    else if(tolower(argv[1][0]) == 'd' && tolower(argv[1][1]) == 'i') {
            if(argc == 3)
            {
                if(CS_E_OK == ts_uart_id_check(iros_strtol(argv[2])))
                    epon_request_onu_ts_disable(context,0,0,iros_strtol(argv[2]));
                else
                    diag_printf("Invalid UART ID [1~4]\n\r");
            }
            else
            {
                    do_term_server_help();
            }
    }
    else if(tolower(argv[1][0]) == 't' && tolower(argv[1][1]) == 'e') {
                ts_electricity_meter_test(iros_strtol(argv[2]));
    }
    else if(tolower(argv[1][0]) == 'd' && tolower(argv[1][1]) == 'b') {
            if(term_serv_dbg)
           {
                term_serv_dbg = 0;
            }
            else
            {
                term_serv_dbg = 1;
            }
    }
    else if(tolower(argv[1][0]) == 'r' && tolower(argv[1][1]) == 'e') {
            if(argc == 3)
            {
                if(CS_E_OK == ts_uart_id_check(iros_strtol(argv[2])))
                    ts_reset(iros_strtol(argv[2]));
                else
                    diag_printf("Invalid UART ID\n\r");
            }
            else
            {
                    do_term_server_help();
            }
    }
    else if(tolower(argv[1][0]) == 'c' && tolower(argv[1][1]) == 'l') {
                if(argc == 3)
                {
                    if(CS_E_OK == ts_uart_id_check(iros_strtol(argv[2])))
                        epon_request_onu_ts_cfg_clear(context,0,0,iros_strtol(argv[2]));
                    else
                        diag_printf("Invalid UART ID [1~4]\n\r");
                }
                else
                {
                        do_term_server_help();
                }
    }
    else if(tolower(argv[1][0]) == 's' && tolower(argv[1][1]) == 'a') {
                if(argc == 3)
                {
                    if(CS_E_OK == ts_uart_id_check(iros_strtol(argv[2])))
                        epon_request_onu_ts_cfg_save(context,0,0,iros_strtol(argv[2]));
                    else
                        diag_printf("Invalid UART ID [1~4]\n\r");
                }
                else
                {
                        do_term_server_help();
                }
    }
    else
    {
        do_term_server_help();
    }

    return 1;
}
#endif

#if 1
//add by zhuxh
extern void db_restore_to_factory();
#endif
#ifdef HAVE_ZTE_SIJIE
extern cs_status epon_request_onu_reset_set(void);
#endif  /*END_HAVE_ZTE_SIJIE*/
int plat_cmd_proc(int argc , char **argv)
{
    if(argc == 1 && cmd_cmp(argv[0] , "reset"))
    {
#if defined(HAVE_FLASH_FS)
        extern int fs_umount(void);
        fs_umount();
#endif
        cs_printf("reboot\n");
#ifdef HAVE_ZTE_SIJIE
        epon_request_onu_reset_set();
#endif  /*END_HAVE_ZTE_SIJIE*/
        hal_delay_us(1000);
        HAL_PLATFORM_RESET();
        return 1;
    }

    if(argc == 1 && cmd_cmp(argv[0] , "v"))
    {
        do_show_version();
        return 1;
    }

#ifdef HAVE_TERMINAL_SERVER
    if(cmd_cmp(argv[0] , "ts"))
    {
        term_server_cmd_proc(argc , argv);
        return 1;
    }
#endif

#ifdef HAVE_KT_OAM
    if(cmd_cmp(argv[0] , "x") && cmd_cmp(argv[1], "opm"))
    {
        opm_cmd_proc(argc , argv);
        return 1;
    }
#endif


    if(argc >= 2)
    {
		#if 1
		//add by zhuxh
		if(argc == 2 && cmd_cmp(argv[0] , "db") && cmd_cmp(argv[1] , "restore"))
		{
			db_restore_to_factory();
			cs_printf("db restore to factory\n");
			return 1;
		}
		#endif

        if(argc == 4 && cmd_cmp(argv[0] , "s") && cmd_cmp(argv[1] , "m"))
        {
            do_set_memory(argv);
            return 1;
        }

        if(argc >= 3 && cmd_cmp(argv[0] , "d") && cmd_cmp(argv[1] , "m"))
        {
            do_dump_memory(argv , argc);
            cs_thread_delay(25);
            return 1;
        }

        if(argc == 2 && cmd_cmp(argv[0] , "p") && cmd_cmp(argv[1] , "p"))
        {
            oam_set_pkt_dump(OAM_DUMP_TYPE_ALL);
            return 1;
        }

        if(argc == 2 && cmd_cmp(argv[0] , "p") && cmd_cmp(argv[1] , "o"))
        {
            oam_set_pkt_dump(OAM_DUMP_TYPE_ORG);
            return 1;
        }

        if(argc == 2 && cmd_cmp(argv[0] , "p") && cmd_cmp(argv[1] , "e"))
        {
            oam_set_pkt_dump(OAM_DUMP_TYPE_EVT);
            return 1;
        }

        if(argc >= 2 && cmd_cmp(argv[0] , "d") && cmd_cmp(argv[1] , "c"))
        {
            if (argc == 3 && cmd_cmp(argv[2] , "t"))
            {
#ifdef HAVE_CPULOAD_PER_THREAD
                cs_thread_cpu_load_show();
#endif
            }
            else
            {
                dump_cpuload_info(NULL);
            }

            return 1;
        }

        if(argc == 3 && cmd_cmp(argv[0] , "p") && cmd_cmp(argv[1] , "t") && cmd_cmp(argv[2] , "s"))
        {
            show_threads_info(NULL);
            return 1;
        }

        if(argc == 3 && cmd_cmp(argv[0] , "p") && cmd_cmp(argv[1] , "t") && cmd_cmp(argv[2] , "i"))
        {
            cs_thread_show();
            return 1;
        }

        if(argc == 2 && cmd_cmp(argv[0] , "p") && cmd_cmp(argv[1] , "s"))
        {
            show_system_status();
            return 1;
        }

        if(argc == 2 && cmd_cmp(argv[0] , "d") && cmd_cmp(argv[1] , "p"))
        {
            extern struct mallinfo mallinfo( void );
            struct mallinfo minfo;
            minfo = mallinfo();
            diag_printf("malloc info:\n mallinfo.arean %d\n", minfo.arena);
            diag_printf(" mallinfo.ordblks %d\n", minfo.ordblks);
            diag_printf(" mallinfo.smblks %d\n", minfo.smblks);
            diag_printf(" mallinfo.smblks %d\n", minfo.smblks);
            diag_printf(" mallinfo.hblks %d\n", minfo.hblks);
            diag_printf(" mallinfo.hblkhd %d\n", minfo.hblkhd);
            diag_printf(" mallinfo.usmblks %d\n", minfo.usmblks);
            diag_printf(" mallinfo.fsmblks %d\n", minfo.fsmblks);
            diag_printf(" mallinfo.uordblks %d\n", minfo.uordblks);
            diag_printf(" mallinfo.keepcost %d\n", minfo.keepcost);
            diag_printf(" mallinfo.maxfree %d\n\n", minfo.maxfree);
            return 1;
        }
#ifdef HAVE_IP_STACK
        if(argc == 2 && cmd_cmp(argv[0] , "ping"))
        {
            extern cs_status app_ipintf_ping(char *host);
            app_ipintf_ping(argv[1]);
            return 1;
        }

        if(argc == 2 && cmd_cmp(argv[0] , "dhcp"))
        {
            if(cmd_cmp(argv[1] , "start"))
            {
                extern void app_dhcp_start();
                app_dhcp_start();
            }
            else if(cmd_cmp(argv[1] , "stop"))
            {
                extern void app_dhcp_stop();
                app_dhcp_stop();
            }
            else if(cmd_cmp(argv[1] , "show"))
            {
                extern void app_dhcp_show();
                app_dhcp_show();
            }
            return 1;
        }

#endif
    }

    return 0;
}


void show_system_status(void)
{
    cyg_uint32 seconds, minutes, hours, days;
    cyg_tick_count_t sys_up_time_ticks;
    cyg_uint32 val32, val32a;

    // sys up time
    sys_up_time_ticks = cyg_current_time(); // 1 tick = 10ms
    seconds = sys_up_time_ticks / 1000;
    hours = seconds / 3600;
    seconds -= hours * 3600;
    minutes = seconds / 60;
    seconds -= minutes * 60;

    days = hours / 24;
    hours -= days * 24;
    cs_printf("%-30s: %02d:%02d:%02d:%02d\n", "System Up Time(DD:HH:MM:SS)",
                days, hours, minutes, seconds);

    // top interrupt
    HAL_READ_UINT32(IROS_PER_IRQ, val32);
    HAL_READ_UINT32(IROS_PER_IRQ_ENABLE, val32a);
    cs_printf("%-30s: 0x%08x/0x%08x\n", "top irq status/enable",
                val32, val32a);

    cs_printf("%-30s: %d\n", "onu_int_timer2_count", cs_intr_count_get(ONU_INT_FLAGBIT_TIMER2));
    cs_printf("%-30s: %d\n", "onu_int_timer3_count", cs_intr_count_get(ONU_INT_FLAGBIT_TIMER3));
    cs_printf("%-30s: %d\n", "onu_int_timer4_count", cs_intr_count_get(ONU_INT_FLAGBIT_TIMER4));
    cs_printf("%-30s: %d\n", "onu_int_timer5_count", cs_intr_count_get(ONU_INT_FLAGBIT_TIMER5));
    cs_printf("%-30s: %d\n", "onu_int_uart0_count", cs_intr_count_get(ONU_INT_FLAGBIT_UART0));
    cs_printf("%-30s: %d\n", "onu_int_ssp_count", cs_intr_count_get(ONU_INT_FLAGBIT_SSP));
    cs_printf("%-30s: %d\n", "onu_int_gpio_count", cs_intr_count_get(ONU_INT_FLAGBIT_GPIO));
#ifdef HAVE_VOIP
    cs_printf("%-30s: %d\n", "onu_int_pcm_count", cs_intr_count_get(ONU_INT_FLAGBIT_PCM));
#endif
    cs_printf("%-30s: %d\n", "onu_int_dma0_tx", onu_int_dma0_tx);
    cs_printf("%-30s: %d\n", "onu_int_esdma_auto_xfer", onu_int_esdma_auto_xfer);
    cs_printf("%-30s: %d\n", "onu_int_esdma_xfer2mem", onu_int_esdma_xfer2mem);
    cs_printf("%-30s: %d\n", "onu_int_esdma_pkt_xfer", onu_int_esdma_pkt_xfer);
    cs_printf("%-30s: %d\n", "onu_int_ma_count", cs_intr_count_get(ONU_INT_FLAGBIT_MA));
    cs_printf("%-30s: %d\n", "onu_int_bm_count", cs_intr_count_get(ONU_INT_FLAGBIT_BM));
    cs_printf("%-30s: %d\n", "onu_int_fe_count", cs_intr_count_get(ONU_INT_FLAGBIT_FE));
    cs_printf("%-30s: %d\n", "onu_int_ni_count", cs_intr_count_get(ONU_INT_FLAGBIT_NI));
    cs_printf("%-30s: %d\n", "onu_int_epon_count", cs_intr_count_get(ONU_INT_FLAGBIT_EPON));

    cs_printf("%-30s: %d\n", "sdl thread count", intr_thread_count);
    cs_printf("%-30s: %d\n", "timer thread count", timer_thread_count);
    cs_printf("%-30s: %d\n", "app thread count", app_thread_count);
    cs_printf("%-30s: %d\n", "shell thread count", shell_thread_count);
}
