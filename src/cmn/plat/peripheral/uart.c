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

#include "iros_config.h"
#include "plat_common.h"
#include "registers.h"
#include "uart.h"

#define IO_READ(p)              ((*(volatile unsigned int *)(p)))
#define IO_WRITE(p, c)          (*(volatile unsigned int *)(p) = (c))
#define GET_STATUS(p)       (IO_READ((p) + IMST_UARTFR))
#define PUT_CHAR(p, c)      (IO_WRITE(((p) + IMST_UARTDW), (c)))
#define TX_READY(s)         (((s) & IMST_UARTFR_TXFF) == 0)

#undef GET_CHAR
#undef RX_DATA
#define GET_CHAR(p)             ((IO_READ((p) + IMST_UARTDR)) & 0xff)
#define RX_DATA(s)              (((s) & IMST_UARTFR_DA) == 0)

/*default use uart0*/
#define MAX_RX_LEN    0x100
static unsigned char uart_rx_buf[MAX_RX_LEN];
static unsigned int rx_in = 0;
static const uart_channel_t uart_channels[MAX_UART_NUM] = {
        {IMST_UART0_BASE,CYGNUM_HAL_INTERRUPT_UART0},
        {IMST_UART1_BASE,CYGNUM_HAL_INTERRUPT_UART1},
        {IMST_UART2_BASE,CYGNUM_HAL_INTERRUPT_UART2},
        {IMST_UART3_BASE,CYGNUM_HAL_INTERRUPT_UART3},
        {IMST_UART4_BASE,CYGNUM_HAL_INTERRUPT_UART4}
  };
   
void uart_intr_hanlder(cs_uint8 uart_id)
{
        PER_UART0_INT_t int_stat;

        CS_REG_READ(uart_channels[uart_id].base_address + IMST_UARTINT,int_stat.wrd);
//      if(int_stat.bf.rx_fifo_nempty)
        {
                /* clear interrupt status */
                CS_REG_WRITE(uart_channels[uart_id].base_address + IMST_UARTINT,int_stat.wrd);

                /* read data */
                CS_REG_READ(uart_channels[uart_id].base_address + IMST_UARTDR,uart_rx_buf[rx_in]);

                rx_in ++;
                if(rx_in >= MAX_RX_LEN)
                    rx_in = 0;
          }

}

cs_status uart_int_status_clear(cs_uint8 uart_id)
{
    PER_UART0_INT_t int_stat;
    
    if(uart_id >= MAX_UART_NUM)
        return CS_E_PARAM;
    
    CS_REG_READ(uart_channels[uart_id].base_address + IMST_UARTINT,int_stat.wrd);
    /* clear interrupt status */
    CS_REG_WRITE(uart_channels[uart_id].base_address + IMST_UARTINT,int_stat.wrd);

    return CS_E_OK;
}

void uart_init(uart_config_t *uart_cfg)
{
        PER_UART0_CFG_t  cfg;
        PER_UART0_RX_SAMPLE_t  sample;
        PER_UART0_TUN_t  fine_tun;
        GLOBAL_PIN_CTRL_t pin_ctrl;
        unsigned int baudrate_cnt = 0;
        PER_UART0_IE_t int_enable;
        cs_uint32 uart_base;


        if(uart_cfg->uart_id >= MAX_UART_NUM)
        {
            diag_printf("uart ID exceed max number %d\n",(MAX_UART_NUM-1));
            return;
        }
        cfg.wrd = 0;
        sample.wrd = 0;
        fine_tun.wrd = 0;
        uart_base = uart_channels[uart_cfg->uart_id].base_address;

        /* Uart4 shared with TOD */
        if(uart_cfg->uart_id == 4)
        {
            /* Configure global pin control bit to enable uart4 */
            CS_REG_READ(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
            pin_ctrl.bf.glb_ptp_1pps_seln = 1;
            pin_ctrl.bf.glb_share_pin_gp4 = 1;
            CS_REG_WRITE(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
        }
        else if(uart_cfg->uart_id == 3)
        {
            /* Configure global pin control bit to enable uart4 */
            CS_REG_READ(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
            pin_ctrl.bf.glb_share_pin_gp3 = 1;
            CS_REG_WRITE(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
        }
        else if(uart_cfg->uart_id == 2)
        {
            /* Configure global pin control bit to enable uart4 */
            CS_REG_READ(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
            pin_ctrl.bf.glb_share_pin_gp2 = 1;
            CS_REG_WRITE(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
        }
        else if(uart_cfg->uart_id == 1)
        {
            /* Configure global pin control bit to enable uart4 */
            CS_REG_READ(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
            pin_ctrl.bf.glb_share_pin_gp1 = 1;
            CS_REG_WRITE(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
        }
        // first, disable everything
        CS_REG_WRITE(uart_base + IMST_UARTCR, 0);

        // set baud rate
        // set the UART to be 8 bits, 1 stop bit, no parity, fifo enabled
        // finally, enable the uart
        baudrate_cnt = CYG_HAL_ARM_IMST_DEV_FREQ/uart_cfg->baud_rate;
        /* baud_count only contain 16bits */
        if((baudrate_cnt -1) > BAUD_COUNT_MASK)
        {
            cfg.bf.baud_count = (baudrate_cnt -1) & BAUD_COUNT_MASK;
            cfg.bf.baud_count_hi = (baudrate_cnt -1) >> 16;
        }
        else
        {
            cfg.bf.baud_count = baudrate_cnt -1;
        }
        if(baudrate_cnt/48 > FINE_TUNE_MASK)
        {
                /* fine tune register has 12 bits,baudrate_cnt has 13bits for baudrate 300*/
                if(uart_cfg->baud_rate == 300)
                {
                    fine_tun.bf.tx_fine_tune_ctl = 0xFF;
                    fine_tun.bf.tx_fine_tune_ctl_hi = 0xF;
                    fine_tun.bf.rx_fine_tune_ctl = 0xFF;
                    fine_tun.bf.rx_fine_tune_ctl_hi = 0xF;
                }
                else
                {
                    fine_tun.bf.tx_fine_tune_ctl = (baudrate_cnt/48) & FINE_TUNE_MASK;
                    fine_tun.bf.tx_fine_tune_ctl_hi = (baudrate_cnt/48) >> 8;
                    fine_tun.bf.rx_fine_tune_ctl = (baudrate_cnt/48) & FINE_TUNE_MASK;
                    fine_tun.bf.rx_fine_tune_ctl_hi = (baudrate_cnt/48) >> 8;
                }
        }
        else
        {
                fine_tun.bf.tx_fine_tune_ctl = (baudrate_cnt)/48;
                fine_tun.bf.rx_fine_tune_ctl = (baudrate_cnt)/48;
        }

        if(SDL_UART_HALF_DUPLEX == uart_cfg->duplex )
        {
                /* RS485 must work under half-duplex */
                fine_tun.bf.uart_rxd_re_pol = 1;
                fine_tun.bf.uart_txd_de_pol = 0;
                fine_tun.bf.uart_half_duplex = 1;
        }
       /* Ideally this value should beset to half the valu of baud_count */
        sample.bf.rx_center = (baudrate_cnt -1)/2;

        /* Uart data bits setting */
        if(uart_cfg->data_bits == 5)
        {
            cfg.bf.char_cnt = IMST_UARTCR_WLEN_5;
        }
        else if(uart_cfg->data_bits == 6)
        {
            cfg.bf.char_cnt = IMST_UARTCR_WLEN_6;
        }
        else if(uart_cfg->data_bits == 7)
        {
            cfg.bf.char_cnt = IMST_UARTCR_WLEN_7;
        }
        else /* default is 8 bits */
        {
            cfg.bf.char_cnt = IMST_UARTCR_WLEN_8;
        }

        /* Uart parity setting 0-None 1-Odd 2-Even */
        if(uart_cfg->parity == SDL_UART_PARITY_ODD)
        {
            cfg.bf.even_parity_sel = 0;
            cfg.bf.uart_parity_en   = 1;
        }
        else if(uart_cfg->parity == SDL_UART_PARITY_EVEN)
        {
            cfg.bf.even_parity_sel = 1;
            cfg.bf.uart_parity_en   = 1;
        }
        else /* default is None */
        {
            cfg.bf.even_parity_sel = 0;
            cfg.bf.uart_parity_en   = 0;
        }

        /* enable tx, rx and uart core*/
        cfg.bf.rx_sm_enable = 1;
        cfg.bf.tx_sm_enable = 1;
        cfg.bf.uart_en          = 1;

        /* fine tuning config */
        CS_REG_WRITE(uart_base + IMST_UART_TUN, fine_tun.wrd);

        /* sample config */
        CS_REG_WRITE(uart_base + IMST_UART_RX_SAMPLE, sample.wrd);

        /* control config */
        CS_REG_WRITE(uart_base + IMST_UARTCR, cfg.wrd);

      /* enable interrupt if need */
        if(SDL_UART_MODE_INTR == uart_cfg->mode)
        {
            int_enable.wrd = 0;
            int_enable.bf.rx_fifo_nempty = 1;
            CS_REG_WRITE(uart_base + IMST_UARTIE, int_enable.wrd);
        }
}

#ifdef HAVE_TERMINAL_SERVER
//added by zhuxh
cs_status uart_cfg_set(cs_uint8 uart_id, uart_cfg_mask_t mask, uart_config_t *uart_cfg)
{
	PER_UART0_CFG_t cfg;
	PER_UART0_RX_SAMPLE_t sample;
	PER_UART0_TUN_t fine_tun;
	PER_UART0_IE_t int_enable;
	GLOBAL_PIN_CTRL_t pin_ctrl;
	cs_uint32 baudrate_cnt = 0;
	cs_uint32 uart_base;

	if(uart_id >= MAX_UART_NUM)
	{
		cs_printf("uart ID exceed max number %d\n",(MAX_UART_NUM-1));
		return CS_E_PARAM;
	}


	uart_base = uart_channels[uart_id].base_address;

	CS_REG_READ(uart_base + IMST_UARTCR, cfg.wrd);
	CS_REG_READ(uart_base + IMST_UART_RX_SAMPLE, sample.wrd);
	CS_REG_READ(uart_base + IMST_UART_TUN, fine_tun.wrd);
	CS_REG_READ(uart_base + IMST_UARTIE, int_enable.wrd);


	/* Uart4 shared with TOD */
	if(uart_id == 4)
	{
		/* Configure global pin control bit to enable uart4 */
		CS_REG_READ(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
		pin_ctrl.bf.glb_ptp_1pps_seln = 1;
		pin_ctrl.bf.glb_share_pin_gp4 = 1;
		CS_REG_WRITE(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
	}
	else if(uart_id == 3)
	{
		/* Configure global pin control bit to enable uart4 */
		CS_REG_READ(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
		pin_ctrl.bf.glb_share_pin_gp3 = 1;
		CS_REG_WRITE(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
	}
	else if(uart_id == 2)
	{
		/* Configure global pin control bit to enable uart4 */
		CS_REG_READ(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
		pin_ctrl.bf.glb_share_pin_gp2 = 1;
		CS_REG_WRITE(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
	}
	else if(uart_id == 1)
	{
		/* Configure global pin control bit to enable uart4 */
		CS_REG_READ(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
		pin_ctrl.bf.glb_share_pin_gp1 = 1;
		CS_REG_WRITE(IROS_GLB_GLOBAL_PIN_CTRL, pin_ctrl.wrd);
	}


#if 0
	// TBD: test if support dynamic cfg
	// first, disable everything
	CS_REG_WRITE(uart_base + IMST_UARTCR, 0);
#endif


	if(mask.s.data_bits)
	{
		/* Uart data bits setting */
		if(uart_cfg->data_bits == 5)
		{
			cfg.bf.char_cnt = IMST_UARTCR_WLEN_5;
		}
		else if(uart_cfg->data_bits == 6)
		{
			cfg.bf.char_cnt = IMST_UARTCR_WLEN_6;
		}
		else if(uart_cfg->data_bits == 7)
		{
			cfg.bf.char_cnt = IMST_UARTCR_WLEN_7;
		}
		else /* default is 8 bits */
		{
			cfg.bf.char_cnt = IMST_UARTCR_WLEN_8;
		}
	}

	if(mask.s.stop_bits)
	{

	}

	if(mask.s.flow_control)
	{

	}

	if(mask.s.parity)
	{
		/* Uart parity setting 0-None 1-Odd 2-Even */
		if(uart_cfg->parity == UART_ODD_PARITY)
		{
			cfg.bf.even_parity_sel = 0;
			cfg.bf.uart_parity_en   = 1;
		}
		else if(uart_cfg->parity == UART_EVEN_PARITY)
		{
			cfg.bf.even_parity_sel = 1;
			cfg.bf.uart_parity_en   = 1;
		}
		else /* default is None */
		{
			cfg.bf.even_parity_sel = 0;
			cfg.bf.uart_parity_en   = 0;
		}
	}

	if(mask.s.duplex)
	{
		if(UART_HALF_DUPLEX == uart_cfg->duplex)
		{
			/* RS485 must work under half-duplex */
			fine_tun.bf.uart_rxd_re_pol = 1;
			fine_tun.bf.uart_txd_de_pol = 0;
			fine_tun.bf.uart_half_duplex = 1;
		}
	}

	if(mask.s.baud_rate)
	{
		/* Set baud rate to default setting */
		cfg.bf.baud_count = 0;
		cfg.bf.baud_count_hi = 0;
		fine_tun.bf.tx_fine_tune_ctl = 0;
		fine_tun.bf.tx_fine_tune_ctl_hi = 0;
		fine_tun.bf.rx_fine_tune_ctl = 0;
		fine_tun.bf.rx_fine_tune_ctl_hi = 0;
		/* set the UART to be 8 bits, 1 stop bit, no parity, fifo enabled */
		/* finally, enable the uart */
		baudrate_cnt = CYG_HAL_ARM_IMST_DEV_FREQ/uart_cfg->baud_rate;
		/* baud_count only contain 16bits */
		if((baudrate_cnt -1) > BAUD_COUNT_MASK)
		{
			cfg.bf.baud_count = (baudrate_cnt -1) & BAUD_COUNT_MASK;
			cfg.bf.baud_count_hi = (baudrate_cnt -1) >> 16;
		}
		else
		{
			cfg.bf.baud_count = baudrate_cnt -1;
		}

		if(baudrate_cnt/48 > FINE_TUNE_MASK)
		{
			/* fine tune register has 12 bits,baudrate_cnt has 13bits for baudrate 300*/
			if(uart_cfg->baud_rate == 300)
			{
				fine_tun.bf.tx_fine_tune_ctl = 0xFF;
				fine_tun.bf.tx_fine_tune_ctl_hi = 0xF;
				fine_tun.bf.rx_fine_tune_ctl = 0xFF;
				fine_tun.bf.rx_fine_tune_ctl_hi = 0xF;
			}
			else
			{
				fine_tun.bf.tx_fine_tune_ctl = (baudrate_cnt/48) & FINE_TUNE_MASK;
				fine_tun.bf.tx_fine_tune_ctl_hi = (baudrate_cnt/48) >> 8;
				fine_tun.bf.rx_fine_tune_ctl = (baudrate_cnt/48) & FINE_TUNE_MASK;
				fine_tun.bf.rx_fine_tune_ctl_hi = (baudrate_cnt/48) >> 8;
			}
		}
		else
		{
		fine_tun.bf.tx_fine_tune_ctl = (baudrate_cnt)/48;
		fine_tun.bf.rx_fine_tune_ctl = (baudrate_cnt)/48;
		}

		/* slower down the Tx. usally, TX path need slower than RX path */
		fine_tun.bf.tx_tune_add = 1;

		/* Ideally this value should beset to half the valu of baud_count */
		sample.bf.rx_center = (baudrate_cnt -1)/2;
	}

	if(mask.s.mode)
	{
		/* enable interrupt if need */
		if(UART_INTR_MODE == uart_cfg->mode)
		{
			int_enable.bf.rx_fifo_nempty = 1;
		}
	}

	if(mask.s.enable)
	{
		/* enable tx, rx and uart core*/
		cfg.bf.rx_sm_enable = 1;
		cfg.bf.tx_sm_enable = 1;
		cfg.bf.uart_en      = 1;
	}


	/* fine tuning config */
	CS_REG_WRITE(uart_base + IMST_UART_TUN, fine_tun.wrd);

	/* sample config */
	CS_REG_WRITE(uart_base + IMST_UART_RX_SAMPLE, sample.wrd);

	/* control config */
	CS_REG_WRITE(uart_base + IMST_UARTCR, cfg.wrd);

	/* intr */
	CS_REG_WRITE(uart_base + IMST_UARTIE, int_enable.wrd);

	return CS_E_OK;
}
#endif
/* Get the UART configurations */
cs_status uart_cfg_get(cs_uint8 uart_id, uart_config_t *config)
{
    PER_UART0_CFG_t  cfg;
    PER_UART0_IE_t  int_e;
    PER_UART0_TUN_t  fine_tun;
    unsigned int baudrate_cnt = 0;
    
    if(uart_id >= MAX_UART_NUM)
        return CS_E_PARAM;

    config->uart_id = uart_id;
    
    CS_REG_READ(uart_channels[uart_id].base_address + IMST_UARTCR, cfg.wrd);
    /* Uart data bits setting */
    if(cfg.bf.char_cnt == IMST_UARTCR_WLEN_5)
    {
        config->data_bits = 5;
    }
    else if(cfg.bf.char_cnt == IMST_UARTCR_WLEN_6)
    {
        config->data_bits = 6;
    }
    if(cfg.bf.char_cnt == IMST_UARTCR_WLEN_7)
    {
        config->data_bits = 7;
    }
    else if(cfg.bf.char_cnt == IMST_UARTCR_WLEN_8)
    {
        config->data_bits = 8;
    }

    if((0 == cfg.bf.even_parity_sel) && (1 == cfg.bf.uart_parity_en))
        config->parity = SDL_UART_PARITY_ODD;
    else if((1 == cfg.bf.even_parity_sel) && (1 == cfg.bf.uart_parity_en))
        config->parity = SDL_UART_PARITY_EVEN;
    else if((0 == cfg.bf.even_parity_sel) && (0 == cfg.bf.uart_parity_en))
        config->parity = SDL_UART_PARITY_NONE;
    
    baudrate_cnt = ((cfg.bf.baud_count_hi << 16) | cfg.bf.baud_count) + 1;
    config->baud_rate = CYG_HAL_ARM_IMST_DEV_FREQ/baudrate_cnt;

    if(cfg.bf.uart_stop_2bit)
        config->stop_bits = 2;
    else
        config->stop_bits = 1;

    CS_REG_READ(uart_channels[uart_id].base_address + IMST_UART_TUN, fine_tun.wrd);
    if(fine_tun.bf.uart_half_duplex)
        config->duplex = SDL_UART_HALF_DUPLEX;
    else
        config->duplex = SDL_UART_FULL_DUPLEX;

    CS_REG_READ(uart_channels[uart_id].base_address + IMST_UARTIE, int_e.wrd);

    if(int_e.bf.tx_fifo_emptye | int_e.bf.rx_fifo_nempty)
        config->mode =  SDL_UART_MODE_INTR;
    else
        config->mode =  SDL_UART_MODE_POLL;

    return CS_E_OK;

}

/* Enable/ disable UART */
cs_status uart_ctrl_set(cs_uint8 uart_id,cs_sdl_uart_ctrl_t ctrl)
{
    PER_UART0_IE_t  int_e;

    if((uart_id >= MAX_UART_NUM) || (ctrl > SDL_UART_ENABLE))
        return CS_E_PARAM;

    CS_REG_READ(uart_channels[uart_id].base_address + IMST_UARTIE, int_e.wrd);
    if(SDL_UART_DISABLE == ctrl)
    {
        int_e.bf.rx_fifo_nempty = 0;
    }
    else
    {
        int_e.bf.rx_fifo_nempty = 1;
    }

    CS_REG_WRITE(uart_channels[uart_id].base_address + IMST_UARTIE, int_e.wrd);

    return CS_E_OK;
    
}


cs_status uart_ctrl_get(cs_uint8 uart_id,cs_sdl_uart_ctrl_t *ctrl)
{
    PER_UART0_IE_t  int_e;

    if(uart_id >= MAX_UART_NUM)
        return CS_E_PARAM;
        
    CS_REG_READ(uart_channels[uart_id].base_address + IMST_UARTIE, int_e.wrd);
    if(int_e.bf.rx_fifo_nempty)
    {
        *ctrl = SDL_UART_ENABLE;
    }
    else
    {
        *ctrl = SDL_UART_DISABLE;
    }

    return CS_E_OK;
}

cs_boolean uart_getc_nonblock(cs_uint8 uart_id,cs_uint8* ch)
{
    cs_uint32 status ;
    int timeout = 2;  // ... just try twice

    if(uart_id >= MAX_UART_NUM)
    {
        return false;
    }
        
    do {
        status = GET_STATUS(uart_channels[uart_id].base_address);
        if (timeout-- == 0) return false ;
    } while (!RX_DATA(status)); // wait until ready

    *ch = GET_CHAR(uart_channels[uart_id].base_address);

    return true;
}

cs_boolean uart_putc(cs_uint8 uart_id,char ch)
{
        cs_uint32 status;
        int i;
        cs_boolean  res = true;

        if(uart_id >= MAX_UART_NUM)
        {
            return false;
        }
        
        i = 10000;
        do {
                status = GET_STATUS(uart_channels[uart_id].base_address);
                i--;
                if (i < 0)
                {
                    res = false;
                    break;
                }
        } while (!TX_READY(status)); // wait until ready

        PUT_CHAR(uart_channels[uart_id].base_address, ch);

        return res;
}


cs_boolean  uart_receive(cs_uint8 uart_id,cs_uint8* ch)
{
    int delay_count;
    cs_boolean res;

    delay_count = 1;

    for(;;) {
        res = uart_getc_nonblock(uart_id,ch);
        if (res || 0 == delay_count--)
            break;
//        cs_thread_delay(5); // delay 20 ticks, that is, 0.2 seconds
    }

    return res;
}

void  uart_send(cs_uint8 uart_id,char * buffer, unsigned int len)
{
    int count = 0;

    for(count = len; count > 0; count--)
        uart_putc(uart_id,*buffer++);
}

void uart_electricity_meter_test(cs_uint8 uart_id)
{
   static char command[100] = {0xfe,0xfe,0x68,0x99,0x99,0x99,0x99,
              0x99,0x99,0x68,0x11,0x04,0x33,0x33,0x34,0x33,0x48,0x16};
   
   uart_send(uart_id,(char *)command,strlen(command));
}

