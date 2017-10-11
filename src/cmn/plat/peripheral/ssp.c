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
 * ssp.c
 *
 * $Id: ssp.c,v 1.1.4.6.2.7.2.3.2.11.2.1 2011/12/13 02:42:36 ljin Exp $
 *
 */

/****************************************************************

OVERVIEW

This file contains the SSP driver code

****************************************************************/
#include "iros_config.h"
#include "plat_common.h"
#include "registers.h"
#include "ssp.h"


#ifdef _SI_NAV_HELPER_
extern AHB_t *g_ahb_base;
#else
#define g_ahb_base   ((AHB_t *)&(g_lynxd_reg_ptr->ahb))
#endif

#define EEPROM_SIZE  64
static cs_uint16 eeprom_buf[EEPROM_SIZE];

#ifndef IROSBOOT
static cs_uint32 ssp_access_sem;
static cs_uint32 ssp_tran_lock;
extern cs_uint32  iros_flash_mutex;

#ifdef HAVE_SERIAL_FLASH
/* Definitions for sflash test */
static  cs_uint32 sflash_test_len[]={0x1000,0x2000,0x10000,0x20000,0x631a9,0x80000,0x100000,0};      
static cs_uint32 test_base = 0x40300020; /* select  a 256 bytes unaligned address */
static cs_uint32 start_block = 0x30;
bool sflash_detail_info= FALSE;

#define MEM_BUF           0x10700000
#define SFLASH_DBG  if(sflash_detail_info) cs_printf 

extern bool  do_memcmp(const void *m1, const void *m2, unsigned int n);
#endif
#endif


#define LYNXD_ASIC_HCLK_KHZ       125000 /* 125 MHz */
static cs_uint16 ssp_eeprom_clk = 0x20;
static cs_uint16 ssp_slic_clk   = 0x6;
static cs_uint16 ssp_serdes_clk = 0x6;
static cs_uint16 ssp_switch_clk = 0x18;

/* Chip select 3 is used for SSP RXD mux, deselect is requred after use*/
static void cs3_deselect(void)
{
    AHB_SSP_CFG_t ssp_cfg;
    
      /* deselect chip select 3 */
    CS_REG_READ(IROS_AHB_SSP_CFG, ssp_cfg.wrd);
    ssp_cfg.bf.sel_ssp_cs &= 0x7;  // READ
    CS_REG_WRITE(IROS_AHB_SSP_CFG, ssp_cfg.wrd);
}

cs_int32 ssp_set_config(ssp_intf_cfg_t *in_cfg)
{
    AHB_SSP_CFG_t cfg;
    AHB_SSP_CLK_t clk;

    if (in_cfg == NULL) return -1;
    if (0 == in_cfg->speed_kHz) return -1;

#if 0
    /*
     * The equation to compute the data to be programmed is
     * ssp_freq = 1/ (2* (Speed_Khz + 1)* per_clk ))
     * or
     * Assumed units as Mhz, But Peripheral clk units as Time period.
     * Speed_Khz = (per_clk/ (2* ssp_freq) ) - 1
     */
    cs_uint16 per_clk_KHz, prer;
    per_clk_KHz = SSP_CLOCK;
    prer = ((per_clk_KHz * 1000)/(2 * in_cfg->speed_kHz) - 1);
#endif

    CS_REG_READ(IROS_AHB_SSP_CLK, clk.wrd);
    clk.bf.counter_load = in_cfg->speed_kHz;
    CS_REG_WRITE(IROS_AHB_SSP_CLK, clk.wrd);
    // diag_printf("SSP_CLK: 0x%08x\n", clk.wrd);

    CS_REG_READ(IROS_AHB_SSP_CFG, cfg.wrd);
    cfg.bf.sel_ssp_cs           = 0xF  & in_cfg->ssp_select;
    cfg.bf.micro_wire_cs_sel    = 0xF  & in_cfg->mwr_ssp_select;
    cfg.bf.pre_ssp_dat_cnt      = 0x1F & in_cfg->data_len;
    cfg.bf.ssp_cmd_cnt          = 0x7F & in_cfg->command_len;
    cfg.bf.command_cyc          = 0x1  & in_cfg->command_only;
    cfg.bf.edge_align           = 0x1  & in_cfg->edge_align;
    cfg.bf.datin_cmds           = 0x1  & in_cfg->datin_cmds;
    cfg.bf.idat_mode            = 0x1  & in_cfg->idat_mode;
    cfg.bf.tdat_cpha            = 0x1  & in_cfg->tdat_cpha;
    cfg.bf.read_write           = 0x1  & in_cfg->read_write;
    CS_REG_WRITE(IROS_AHB_SSP_CFG, cfg.wrd);
    //diag_printf("SSP_CFG: 0x%08x\n", cfg.wrd);

    return 0;
}

cs_int32 ssp_get_config(ssp_intf_cfg_t *out_cfg)
{
    AHB_SSP_CFG_t cfg;
    AHB_SSP_CLK_t clk;
    cs_uint16 per_clk_KHz, prer;

    if (out_cfg == NULL) return -1;

    per_clk_KHz = SSP_CLOCK;
    /*
     * The equation to compute the data to be programmed is
     * Speed_Khz = (prer + 1) * 2) /(per_clk_Khz * 1000) ;
     */
    CS_REG_READ(IROS_AHB_SSP_CLK, clk.wrd);
    prer = (((clk.bf.counter_load + 1) * 2) /(per_clk_KHz * 1000));
    out_cfg->speed_kHz = prer;

    CS_REG_READ(IROS_AHB_SSP_CFG, cfg.wrd);
    out_cfg->ssp_select = cfg.bf.sel_ssp_cs;
    out_cfg->mwr_ssp_select = cfg.bf.micro_wire_cs_sel;
    out_cfg->data_len = cfg.bf.pre_ssp_dat_cnt;
    out_cfg->command_len = cfg.bf.ssp_cmd_cnt;
    out_cfg->command_only = cfg.bf.command_cyc;

    return 0;
}


cs_int32 ssp_read(ssp_intf_data_t *_intf_data)
{
    AHB_SSP_CTRL_t ctrl;
    AHB_SSP_CFG_t cfg;
    AHB_SSP_CA0_t cmd0;
    AHB_SSP_RDAT_t data0;
    unsigned int cnt=0;

    if (_intf_data == NULL) return -1;

    /* Write the cmds */
    cmd0.bf.ssp_cmd_addr0 = _intf_data->cmd[0];
    CS_REG_WRITE(IROS_AHB_SSP_CA0, cmd0.wrd);

    /* set it as read access */
    CS_REG_READ(IROS_AHB_SSP_CFG, cfg.wrd);
    cfg.bf.read_write = 1;  // READ
    CS_REG_WRITE(IROS_AHB_SSP_CFG, cfg.wrd);

    /* start the control to retrieve data */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspdone = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspstart = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

    /* Non block check SSP done */
    if (_intf_data->mode == SSP_MODE_NON_BLOCK) {
        CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
        if (ctrl.bf.sspdone) {
            CS_REG_READ(IROS_AHB_SSP_RDAT, data0.wrd);
        } else {
            diag_printf("fail in non block mode!\n");
            return -1;
        }
    }
    else if (_intf_data->mode == SSP_MODE_BLOCK)
    {
        cnt = SSP_POLL_TIME;
        do {
            CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
            if (ctrl.bf.sspdone) {
                // diag_printf("still got %d count left\n", cnt);
                CS_REG_READ(IROS_AHB_SSP_RDAT, data0.wrd);
                // diag_printf("0x%08x\n", data0.wrd);
                break;
            }
        } while (cnt--);
        if (!cnt) {
            diag_printf("fail in block mode!\n");
            return -1;
        }
    }

    /* Write 1 to SSPDONE to clear the SSP access */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspdone = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

    /* copy the data read to the _intf_data structure */
    _intf_data->data = data0.bf.ssp_rdata;

    return 0;
}

cs_int32 ssp_write(ssp_intf_data_t *data)
{
    AHB_SSP_CTRL_t ctrl;
    AHB_SSP_CFG_t cfg;
    AHB_SSP_CA0_t cmd0;
    AHB_SSP_WDAT_t wdata;
    cs_uint32 cnt = 0;

    if (data == NULL) return -1;

    cmd0.bf.ssp_cmd_addr0 = data->cmd[0];
    CS_REG_WRITE(IROS_AHB_SSP_CA0, cmd0.wrd);

    wdata.bf.ssp_wdata = data->data;
    CS_REG_WRITE(IROS_AHB_SSP_WDAT, wdata.wrd);

    /* prepare cfg value */
    CS_REG_READ(IROS_AHB_SSP_CFG, cfg.wrd);

#if 0
    /* Set the Cmd sync only if < 96 data len */
    if (data->command_len < SSP_MAX_CMD_LEN)
        cfg.bf.command_cyc = 1;
    /* update data len and command len */
    cfg.bf.ssp_cmd_cnt = data->command_len;
    cfg.bf.pre_ssp_dat_cnt = data->data_len;
#endif

    /* Set it as Write access */
    cfg.bf.read_write = 0;
    CS_REG_WRITE(IROS_AHB_SSP_CFG, cfg.wrd);

    /* set the control before invoking write command */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspstart = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

    if (data->mode == SSP_MODE_NON_BLOCK) {
        CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
        if (!ctrl.bf.sspdone) {
            diag_printf("fail to perform SSP write in Non block mode!\n");
            return -1;
        }
    } else if (data->mode == SSP_MODE_BLOCK) {
        cnt = SSP_POLL_TIME;
        do {
            CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
            if (ctrl.bf.sspdone) break;
        } while (cnt--);
        if (!cnt) {
            diag_printf("block mode cannot poll the result\n");
            return -1;
        }
    }

    /* Write 1 to SSPDONE to clear the SSP access */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspdone = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

    return 0;
}

/* for some cases that slave SPI only supports 8-bit width */
cs_int32 ssp_read_byte(cs_uint8 *data)
{
    AHB_SSP_CTRL_t ctrl;
    AHB_SSP_CFG_t cfg;
    cs_uint32 dataRead;

    /* prepare cfg value */
    CS_REG_READ(IROS_AHB_SSP_CFG, cfg.wrd);
    cfg.bf.read_write = 1;      /* set it as read */
    CS_REG_WRITE(IROS_AHB_SSP_CFG, cfg.wrd);


    /* start the control to receive the data.
     * poll until it finishes */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspstart = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

#ifdef SSP_POLLING_MODE
    cs_uint32 cnt = 0;
    cnt = SSP_POLL_TIME;
    do {
        CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
        if (ctrl.bf.sspdone) {
            break;
        }
    } while (cnt--);

    if (cnt <= 0) {
        /* unable to receive completely before count runs out */
        diag_printf("couldn't complete receiving data\n");
        return -1;
    }
#else
    if(CS_E_OSAL_OK != cs_semaphore_wait(ssp_access_sem,100))
    {
        diag_printf("ssp cs_semaphore_wait failed\n",);
        return -1;
    }
#endif

    /* write sspdone back to register to clear off */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspdone = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

    /* read the data */
    CS_REG_READ(IROS_AHB_SSP_RDAT, dataRead);
    data[0] = dataRead & 0x000000ff;

    return 0;
}

cs_int32 ssp_write_byte(cs_uint8 _data)
{
    AHB_SSP_CTRL_t ctrl;
    AHB_SSP_CFG_t cfg;

    /* prepare cfg value */
    CS_REG_READ(IROS_AHB_SSP_CFG, cfg.wrd);
    cfg.bf.read_write = 0;      /* set it as write */
    CS_REG_WRITE(IROS_AHB_SSP_CFG, cfg.wrd);

    /* write the _data to cmd0 register */
    CS_REG_WRITE(IROS_AHB_SSP_CA0, _data<<24);
    CS_REG_WRITE(IROS_AHB_SSP_CA1, 0);
    CS_REG_WRITE(IROS_AHB_SSP_CA2, 0);

    /* set the control before invoking write command */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspstart = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

#ifdef SSP_POLLING_MODE
    cs_int32 cnt = 0;

    /* polling until cmd done */
    cnt = SSP_POLL_TIME;
    do {
        CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
        if (ctrl.bf.sspdone) break;
    } while (cnt--);

    if (cnt <= 0) {
        diag_printf("SSP write timeout\n");
        return -1;
    }
#else
    if(CS_E_OSAL_OK != cs_semaphore_wait(ssp_access_sem,100))
    {
        diag_printf("ssp cs_semaphore_wait failed\n",);
        return -1;
    }
#endif

    /* write sspdone back to register to clear off */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspdone = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

    return 0;
}


void ssp_intr_clear()
{
    AHB_SSP_INT_t int_stat;

    int_stat.wrd = g_ahb_base->SSP_INT.wrd;
    int_stat.bf.sspi = 1;
    g_ahb_base->SSP_INT.wrd = int_stat.wrd;
}

void ssp_intr_enable()
{
    AHB_SSP_IE_t int_en;
    int_en.wrd = g_ahb_base->SSP_IE.wrd;
    int_en.bf.sspe = 1;
    g_ahb_base->SSP_IE.wrd = int_en.wrd;
}

void ssp_intr_hanlder()
{
    AHB_SSP_INT_t int_stat;
    int_stat.wrd = g_ahb_base->SSP_INT.wrd;

    // Check if SSP done interrupt raised
    if(int_stat.bf.sspi)
    {
        // clear the INTR status
        ssp_intr_clear();
#ifndef IROSBOOT
        cs_semaphore_post(ssp_access_sem);
#endif
    }
    else
    {
        diag_printf("unknow SSP intr\n");
    }

}

void ssp_init(void)
{
    cs_status   ret = CS_E_OK;
    
    diag_printf("Initializing SSP\n");

#ifndef IROSBOOT
    cs_semaphore_init((cs_uint32 *)&ssp_access_sem,"SSP internal bus sem", 1,0);
    ret = cs_mutex_init((cs_uint32 *)&ssp_tran_lock, "SSP interface mutex",0);
    if (ret != CS_E_OK) {
        diag_printf("cs_mutex_init FAILED ret=0x%x\n",ret);
    }
#endif

    cs_mutex_init(&iros_flash_mutex, "Flashmutex", 0);
    ssp_intr_clear();
    ssp_intr_enable();
}

/* The clock unit is KHz */
cs_status ssp_slic_clock_set(cs_uint32 clk_khz)
{
    if((2*clk_khz) >= LYNXD_ASIC_HCLK_KHZ)
        return CS_E_PARAM;

    #ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
    #endif 
    ssp_slic_clk = LYNXD_ASIC_HCLK_KHZ/(2*clk_khz) -1;
    #ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
    #endif
    
    return CS_E_OK;
}

cs_status ssp_slic_clock_get(cs_uint32 *clk_khz)
{
    *clk_khz = (LYNXD_ASIC_HCLK_KHZ/(2*(ssp_slic_clk+1)));
     return CS_E_OK;
}

static void ssp_slic_cfg_write()
{
    ssp_intf_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    cfg.speed_kHz = ssp_slic_clk;
    cfg.ssp_select = 0x4;
    cfg.mwr_ssp_select = 0x2;   //each bit, 0: spi 1:MWR

    cfg.data_len = 0x0;
    cfg.command_len = 0x7;      // to read 8 bit, include 1 additional bit
    cfg.command_only = 1;       // 1: command-only, 0: Normal
    cfg.edge_align = 1;
    cfg.datin_cmds = 0;         // 0 for write
    cfg.idat_mode = 0;
    cfg.tdat_cpha = 1;

    ssp_set_config(&cfg);
}


static void ssp_slic_cfg_read()
{
    ssp_intf_cfg_t cfg;

    cfg.speed_kHz = ssp_slic_clk;
    cfg.ssp_select = 0x4;
    cfg.mwr_ssp_select = 0x2;   //each bit, 0: spi 1:MWR

    cfg.data_len = 0x6;
    cfg.command_len = 0;        // to read 8 bit, include 1 additional bit
    cfg.command_only = 0;       // 1: command-only, 0: Normal
    cfg.edge_align = 1;
    cfg.datin_cmds = 1;         // 1 for read
    cfg.idat_mode = 0;
    cfg.tdat_cpha = 1;

    ssp_set_config(&cfg);
}

/* specific API for slic chip as SPI slave */
cs_int32 ssp_slic_read_byte(cs_uint8 *data)
{
    AHB_SSP_CTRL_t ctrl;
    AHB_SSP_CFG_t cfg;
    cs_uint32 dataRead;

#ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
#endif

    ssp_slic_cfg_read();

    /* prepare cfg value */
    CS_REG_READ(IROS_AHB_SSP_CFG, cfg.wrd);
    cfg.bf.read_write = 1;      /* set it as read */
    CS_REG_WRITE(IROS_AHB_SSP_CFG, cfg.wrd);


    /* start the control to receive the data.
     * poll until it finishes */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspstart = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

#ifdef SSP_POLLING_MODE
    cs_uint32 cnt = 0;
    cnt = SSP_POLL_TIME;
    do {
        CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
        if (ctrl.bf.sspdone) {
            break;
        }
    } while (cnt--);

    if (cnt <= 0) {
        /* unable to receive completely before count runs out */
        diag_printf("couldn't complete receiving data\n");
        #ifndef IROSBOOT
        cs_mutex_unlock(ssp_tran_lock);
        #endif
        return -1;
    }
#else
    if(CS_E_OSAL_OK != cs_semaphore_wait(ssp_access_sem,100))
    {
        diag_printf("ssp cs_semaphore_wait failed\n");
#ifndef IROSBOOT
        cs_mutex_unlock(ssp_tran_lock);
#endif
        return -1;
    }
#endif

    /* write sspdone back to register to clear off */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspdone = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

    /* read the data */
    CS_REG_READ(IROS_AHB_SSP_RDAT, dataRead);
    data[0] = dataRead & 0x000000ff;

#ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
#endif
    return 0;
}

cs_int32 ssp_slic_write_byte(cs_uint8 _data)
{
    AHB_SSP_CTRL_t ctrl;
    AHB_SSP_CFG_t cfg;

#ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
#endif

    ssp_slic_cfg_write(SSP_CMD_WRITE);

    /* prepare cfg value */
    CS_REG_READ(IROS_AHB_SSP_CFG, cfg.wrd);
    cfg.bf.read_write = 0;      /* set it as write */
    CS_REG_WRITE(IROS_AHB_SSP_CFG, cfg.wrd);

    /* write the _data to cmd0 register */
    CS_REG_WRITE(IROS_AHB_SSP_CA0, _data<<24);
    CS_REG_WRITE(IROS_AHB_SSP_CA1, 0);
    CS_REG_WRITE(IROS_AHB_SSP_CA2, 0);

    /* set the control before invoking write command */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspstart = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

#ifdef SSP_POLLING_MODE
    cs_int32 cnt = 0;

    /* polling until cmd done */
    cnt = SSP_POLL_TIME;
    do {
        CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
        if (ctrl.bf.sspdone) break;
    } while (cnt--);

    if (cnt <= 0) {
        diag_printf("SSP write timeout\n");
        #ifndef IROSBOOT
        cs_mutex_unlock(ssp_tran_lock);
        #endif
        return -1;
    }
#else
    if(CS_E_OSAL_OK != cs_semaphore_wait(ssp_access_sem,100))
    {
        diag_printf("ssp cs_semaphore_wait failed\n");
#ifndef IROSBOOT
        cs_mutex_unlock(ssp_tran_lock);
#endif
        return -1;
    }
#endif

    /* write sspdone back to register to clear off */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspdone = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

#ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
#endif
    return 0;
}

void ssp_slic_reset(void)
{
    cs_uint32 rdata;

    CS_REG_READ(IROS_PER_GPIO_CFG, rdata);
    rdata &= 0xfffe;
    CS_REG_WRITE(IROS_PER_GPIO_CFG, rdata);

    CS_REG_READ(IROS_PER_GPIO_OUT, rdata);
    rdata &= 0xfffe;
    CS_REG_WRITE(IROS_PER_GPIO_OUT, rdata);
    rdata |= 0x1;
    CS_REG_WRITE(IROS_PER_GPIO_OUT, rdata);
 }

/* SerDes APIs */ 
static unsigned char reverse8( unsigned char c )
{
    c = ( c & 0x55 ) << 1 | ( c & 0xAA ) >> 1;
    c = ( c & 0x33 ) << 2 | ( c & 0xCC ) >> 2;
    c = ( c & 0x0F ) << 4 | ( c & 0xF0 ) >> 4;
    return c;
}

/* The clock unit is KHz */
cs_status serdes_clock_set(cs_uint32 clk_khz)
{
    if((2*clk_khz) >= LYNXD_ASIC_HCLK_KHZ)
        return CS_E_PARAM;
    
    #ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
    #endif 
    ssp_serdes_clk = LYNXD_ASIC_HCLK_KHZ/(2*clk_khz) -1;
    #ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
    #endif 

    return CS_E_OK;
}

cs_status serdes_clock_get(cs_uint32 *clk_khz)
{
    *clk_khz = (LYNXD_ASIC_HCLK_KHZ/(2*(ssp_serdes_clk+1)));
     return CS_E_OK;
}

cs_uint32 serdes_write(cs_uint8 waddr, cs_uint8 wdata) 
{
  ssp_intf_cfg_t cfg;
  cs_uint8 waddr_rev;
  cs_uint8 wdata_rev;
  AHB_SSP_CTRL_t ctrl;
  cs_uint32 cnt = SSP_POLL_TIME;

#ifndef IROSBOOT
  cs_mutex_lock(ssp_tran_lock);
#endif 
  waddr_rev = reverse8(waddr);
  wdata_rev = reverse8(wdata);
  
  memset(&cfg, 0, sizeof(cfg));
  cfg.speed_kHz = ssp_serdes_clk;
  cfg.ssp_select = 0x8;
  cfg.data_len = 7;
  cfg.command_len = 0x9;      // to read 8 bit, include 1 additional bit
  cfg.command_only = 0;       // 1: command-only, 0: Normal
  cfg.edge_align = 0;
  cfg.datin_cmds = 0;         // 0 for write
  cfg.tdat_cpha = 0;
  cfg.read_write = 0;
    
  // write Commad/Address
  CS_REG_WRITE(IROS_AHB_SSP_CA0, waddr_rev<<22);
  CS_REG_WRITE(IROS_AHB_SSP_WDAT, wdata_rev);

  ssp_set_config(&cfg);
  
  /* start the control to receive the data.
    * poll until it finishes */
  CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
  ctrl.bf.sspstart = 1;
  CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

#ifdef SSP_POLLING_MODE
    do {
        CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
        if (ctrl.bf.sspdone) {
            break;
        }
    } while (cnt--);

    if (cnt <= 0) {
        /* unable to receive completely before count runs out */
        diag_printf("couldn't complete receiving data\n");
        #ifndef IROSBOOT
        cs_mutex_unlock(ssp_tran_lock);
        #endif
        return -1;
    }
#else
    if(CS_E_OSAL_OK != cs_semaphore_wait(ssp_access_sem,100))
    {
        diag_printf("ssp cs_semaphore_wait failed\n");
#ifndef IROSBOOT
        cs_mutex_unlock(ssp_tran_lock);
#endif
        return -1;
    }
#endif

    /* write sspdone back to register to clear off */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspdone = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

    cs3_deselect();
#ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
#endif

    return 0;

}

cs_uint32 serdes_read(cs_uint8 raddr, cs_uint8 * rdata) 
{
  ssp_intf_cfg_t cfg;
  cs_uint8 raddr_rev;
  cs_uint32 read_data;
  AHB_SSP_CTRL_t ctrl;
  cs_uint32 cnt = SSP_POLL_TIME;
  
#ifndef IROSBOOT
  cs_mutex_lock(ssp_tran_lock);
#endif   
  raddr_rev = reverse8(raddr);

  // write Commad/Address
  CS_REG_WRITE(IROS_AHB_SSP_CA0, (0x01<<30 | raddr_rev<<22));
//  CS_REG_WRITE(IROS_AHB_SSP_WDAT, 0);

  memset(&cfg, 0, sizeof(cfg));
  cfg.speed_kHz = ssp_serdes_clk;
  cfg.ssp_select = 0x8;
  cfg.data_len = 0x7;
  cfg.command_len = 0x9;      // to read 8 bit, include 1 additional bit
  cfg.command_only = 0;       // 1: command-only, 0: Normal
  cfg.edge_align = 0;
  cfg.datin_cmds = 1;         // 1 for read
  cfg.tdat_cpha = 0;
  cfg.read_write = 1;
  ssp_set_config(&cfg);
  
  /* start the control to receive the data.
    * poll until it finishes */
  CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
  ctrl.bf.sspstart = 1;
  CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

#ifdef SSP_POLLING_MODE
  do {
        CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
        if (ctrl.bf.sspdone) {
            break;
        }
   } while (cnt--);

  if (cnt <= 0) {
        /* unable to receive completely before count runs out */
        diag_printf("couldn't complete receiving data\n");
        #ifndef IROSBOOT
        cs_mutex_unlock(ssp_tran_lock);
        #endif
        return -1;
  }
#else
    if(CS_E_OSAL_OK != cs_semaphore_wait(ssp_access_sem,100))
    {
        diag_printf("ssp cs_semaphore_wait failed\n");
#ifndef IROSBOOT
        cs_mutex_unlock(ssp_tran_lock);
#endif
        return -1;
    }
#endif

  /* write sspdone back to register to clear off */
  CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
  ctrl.bf.sspdone = 1;
  CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

  /* read the data */
  CS_REG_READ(IROS_AHB_SSP_RDAT, read_data);
  *rdata = reverse8(read_data & 0x000000ff);
  
  cs3_deselect();
#ifndef IROSBOOT
  cs_mutex_unlock(ssp_tran_lock);
#endif

  return 0;
}

/* The clock unit is KHz */
cs_status switch_clock_set(cs_uint32 clk_khz)
{
    if((2*clk_khz) >= LYNXD_ASIC_HCLK_KHZ)
        return CS_E_PARAM;

    #ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
    #endif
    ssp_switch_clk = LYNXD_ASIC_HCLK_KHZ/(2*clk_khz) -1;
    #ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
    #endif
    
    return CS_E_OK;
}

cs_status switch_clock_get(cs_uint32 *clk_khz)
{
    *clk_khz = (LYNXD_ASIC_HCLK_KHZ/(2*(ssp_switch_clk+1)));
     return CS_E_OK;
}

cs_uint32 switch_write(cs_uint32 waddr, cs_uint32 wdata) 
{
  ssp_intf_cfg_t cfg;
  AHB_SSP_CTRL_t ctrl;
  cs_uint32 cnt = SSP_POLL_TIME;

#ifndef IROSBOOT
  cs_mutex_lock(ssp_tran_lock);
#endif 
  
  memset(&cfg, 0, sizeof(cfg));
  cfg.speed_kHz = ssp_switch_clk;
  cfg.ssp_select = 0x2;
  cfg.data_len = 0xF;
  cfg.command_len = 0x17;      // to read 8 bit, include 1 additional bit
  cfg.command_only = 0;       // 1: command-only, 0: Normal
  cfg.edge_align = 0;
  cfg.datin_cmds = 0;         // 0 for write
  cfg.tdat_cpha = 0;
  cfg.read_write = 0;
    
  // write Commad/Address
  CS_REG_WRITE(IROS_AHB_SSP_CA0, (SSP_RTL_OP_WRITE<<24 | waddr<<8));
  CS_REG_WRITE(IROS_AHB_SSP_WDAT, wdata);

  ssp_set_config(&cfg);
  
  /* start the control to receive the data.
    * poll until it finishes */
  CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
  ctrl.bf.sspstart = 1;
  CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

 #ifdef SSP_POLLING_MODE
    do {
        CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
        if (ctrl.bf.sspdone) {
            break;
        }
    } while (cnt--);

    if (cnt <= 0) {
        /* unable to receive completely before count runs out */
        diag_printf("couldn't complete receiving data\n");
        #ifndef IROSBOOT
        cs_mutex_unlock(ssp_tran_lock);
        #endif
        return -1;
    }
#else
    if(CS_E_OSAL_OK != cs_semaphore_wait(ssp_access_sem,100))
    {
        diag_printf("ssp cs_semaphore_wait failed\n");
#ifndef IROSBOOT
        cs_mutex_unlock(ssp_tran_lock);
#endif
        return -1;
    }
#endif

    /* write sspdone back to register to clear off */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspdone = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

#ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
#endif

    return 0;

}

cs_uint32 switch_read(cs_uint32 raddr, cs_uint32 * rdata) 
{
  ssp_intf_cfg_t cfg;
  cs_uint32 read_data;
  AHB_SSP_CTRL_t ctrl;
  cs_uint32 cnt = SSP_POLL_TIME;
  
#ifndef IROSBOOT
  cs_mutex_lock(ssp_tran_lock);
#endif   

  // write Commad/Address
  CS_REG_WRITE(IROS_AHB_SSP_CA0, (SSP_RTL_OP_READ<<24 | raddr<<8));
//  CS_REG_WRITE(IROS_AHB_SSP_WDAT, 0);

  memset(&cfg, 0, sizeof(cfg));
  cfg.speed_kHz = ssp_switch_clk;
  cfg.ssp_select = 0x2;
  cfg.data_len = 0xf;
  cfg.command_len = 0x17;      // to read 8 bit, include 1 additional bit
  cfg.command_only = 0;       // 1: command-only, 0: Normal
  cfg.edge_align = 0;
  cfg.datin_cmds = 1;         // 1 for read
  cfg.tdat_cpha = 0;
  cfg.read_write = 1;
  ssp_set_config(&cfg);
  
  /* start the control to receive the data.
    * poll until it finishes */
  CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
  ctrl.bf.sspstart = 1;
  CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

#ifdef SSP_POLLING_MODE
  do {
        CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
        if (ctrl.bf.sspdone) {
            break;
        }
   } while (cnt--);

  if (cnt <= 0) {
        /* unable to receive completely before count runs out */
        diag_printf("couldn't complete receiving data\n");
        #ifndef IROSBOOT
        cs_mutex_unlock(ssp_tran_lock);
        #endif
        return -1;
  }
#else
    if(CS_E_OSAL_OK != cs_semaphore_wait(ssp_access_sem,100))
    {
        diag_printf("ssp cs_semaphore_wait failed\n");
#ifndef IROSBOOT
        cs_mutex_unlock(ssp_tran_lock);
#endif
        return -1;
    }
#endif

  /* write sspdone back to register to clear off */
  CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
  ctrl.bf.sspdone = 1;
  CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

  /* read the data */
  CS_REG_READ(IROS_AHB_SSP_RDAT, read_data);
  *rdata = read_data & 0xffff;
  
#ifndef IROSBOOT
  cs_mutex_unlock(ssp_tran_lock);
#endif

  return 0;
}
/* EEPROM APIs*/

/* The clock unit is KHz */
cs_status ssp_eeprom_clock_set(cs_uint32 clk_khz)
{
    if((2*clk_khz) >= LYNXD_ASIC_HCLK_KHZ)
        return CS_E_PARAM;

    #ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
    #endif
    ssp_eeprom_clk = LYNXD_ASIC_HCLK_KHZ/(2*clk_khz) -1;
    #ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
    #endif
    
    return CS_E_OK;
}

cs_status ssp_eeprom_clock_get(cs_uint32 *clk_khz)
{
    *clk_khz = (LYNXD_ASIC_HCLK_KHZ/(2*(ssp_eeprom_clk+1)));
     return CS_E_OK;
}

void ssp_eeprom_dump()
{
    char *p_buf = NULL;
    p_buf = iros_malloc(IROS_MID_SYS, EEPROM_SIZE*2);
    if(p_buf == NULL)
    {
        diag_printf("dump_eeprom malloc failed\n");
        return;
    }
    ssp_eeprom_read(0, p_buf, EEPROM_SIZE*2);
    data_dump(p_buf, EEPROM_SIZE*2, (char *)0);
    iros_free(p_buf);
}

static void ssp_eeprom_cfg_cmd()
{
    ssp_intf_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    cfg.speed_kHz = ssp_eeprom_clk;
    cfg.ssp_select = 0x2;
    cfg.mwr_ssp_select = 0x02;   //each bit, 0: spi 1:MWR
    cfg.data_len = 0x0;
    cfg.command_len = 0x1F;      // to read 8 bit, include 1 additional bit
    cfg.command_only = 1;        // 1: command-only, 0: Normal
    ssp_set_config(&cfg);
}


static void ssp_eeprom_cfg_data()
{
    ssp_intf_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    cfg.speed_kHz = ssp_eeprom_clk;
    cfg.ssp_select = 0x2;
    cfg.mwr_ssp_select = 0x02;   // each bit, 0: spi 1:MWR
    cfg.data_len = 0xF;
    cfg.command_len = 0x08;      // to read 8 bit, include 1 additional bit
    cfg.command_only = 0;        // 1: command-only, 0: Normal
    ssp_set_config(&cfg);
}

void ssp_eeprom_read(cs_uint8 e_addr, char *dest, cs_uint8 len)
{
    ssp_intf_data_t intf_data;
    cs_uint32 i;

    if( (e_addr + len) > EEPROM_SIZE*2 )
    {
        diag_printf("overflow of eeprom address,please check the addr/len \n");
        return;
    }

    diag_printf("eeprom_read 0x%x 0x%x\n", e_addr, len);

    #ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
    #endif
    for(i = 0; i < EEPROM_SIZE; i++)
    {
        ssp_eeprom_cfg_data();

        intf_data.cmd[0] = ((SSP_EEPROM_OP_READ<<29) | (i<<23));
        intf_data.mode = SSP_MODE_BLOCK;
        ssp_read(&intf_data);
        eeprom_buf[i]= 0xffff & intf_data.data;
    }
    #ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
    #endif
    
    memcpy((void *)dest, (void *)((char *)eeprom_buf + e_addr), len);
}

void ssp_eeprom_write(cs_uint8 e_addr, char *src, cs_uint8 len)
{
    ssp_intf_data_t intf_data;
    cs_uint32 i;

    if((e_addr + len) > EEPROM_SIZE*2)
    {
        diag_printf("overflow of eeprom address,please check the addr/len \n");
        return;
    }

    ssp_eeprom_read(0, (char *)eeprom_buf, EEPROM_SIZE*2);
    memcpy((void *)((char *)eeprom_buf + e_addr), (const void *)src, len);

    #ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
    #endif
    for(i=0; i<EEPROM_SIZE; i++)
    {
        ssp_eeprom_cfg_cmd();
        ssp_write_byte(SSP_EEPROM_OP_EWEN);

        ssp_eeprom_cfg_data();
        intf_data.cmd[0] = ((SSP_EEPROM_OP_WRITE<<29) | (i<<23));
        intf_data.mode = SSP_MODE_BLOCK;
        intf_data.data = 0xffff & eeprom_buf[i];
        ssp_write(&intf_data);
    }
    #ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
    #endif
}

#ifdef HAVE_SERIAL_FLASH
/* Serial Flash Drivers */
static void ssp_sflash_triggle_cmd()
{
    AHB_SSP_CTRL_t ctrl;
    cs_uint32 cnt=0;
    /* set the control before invoking write command */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspstart = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);

    /* polling until cmd done */
    cnt = SSP_POLL_TIME;

    while(cnt){
        cnt--;
        CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
        if (ctrl.bf.sspdone) break;
    }

    if (cnt <= 0) {
    //    cs_printf("SSP write timeout\n");
        return ;
    }
    /* write sspdone back to register to clear off */
    CS_REG_READ(IROS_AHB_SSP_CTRL, ctrl.wrd);
    ctrl.bf.sspdone = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CTRL, ctrl.wrd);
}

static void ssp_sflash_write_done()
{
    ssp_intf_cfg_t cfg;
    unsigned int val32=0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.speed_kHz = 0x10;
    cfg.ssp_select = 0x1;
    cfg.mwr_ssp_select = 0x0;  // each bit, 0: spi 1:MWR
    cfg.data_len = 0x1f;
    cfg.command_len = 0x7;      // to read 8 bit, include 1 additional bit
    cfg.command_only = 0;       // 1: command-only, 0: Normal
    cfg.read_write = SSP_CMD_READ; 
    ssp_set_config(&cfg);  
    CS_REG_WRITE(IROS_AHB_SSP_CA0, (SSP_SFLASH_OP_RDSR<<24) ); 
    
    do{
         /*Check WIP done*/         
         ssp_sflash_triggle_cmd();
         CS_REG_READ(IROS_AHB_SSP_RDAT,val32);
    } while( val32 & SSP_SFLASH_SR_WIP );
}


static void ssp_sflash_cfg_cmd(cs_uint32 cmd)
{
    ssp_intf_cfg_t cfg;

    memset(&cfg, 0, sizeof(cfg));
    cfg.speed_kHz = 0x10;
    cfg.ssp_select = 0x1;
    cfg.mwr_ssp_select = 0x0;  // each bit, 0: spi 1:MWR
    cfg.data_len = 0x0;
    cfg.command_len = 0x7;      // to read 8 bit, include 1 additional bit
    cfg.command_only = 1;       // 1: command-only, 0: Normal
    cfg.read_write=0;           // 1: read;   0: write
    ssp_set_config(&cfg);

    /* write the _data to cmd0 register */
    CS_REG_WRITE(IROS_AHB_SSP_CA0, cmd<<24);
    CS_REG_WRITE(IROS_AHB_SSP_CA1, 0);
    CS_REG_WRITE(IROS_AHB_SSP_CA2, 0);

    ssp_sflash_triggle_cmd();
}


static void ssp_sflash_query(cs_uint32 * data)
{    
    ssp_intf_cfg_t cfg;
    cs_uint32 val32 = 0xFFFFFFFF;
    cs_uint8 id[4] = {0};
    cs_uint16* flash_id = (cs_uint16*)data;

    #ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
    #endif
    memset(&cfg, 0, sizeof(cfg));
    cfg.speed_kHz = 0x10;
    cfg.ssp_select = 0x1;
    cfg.mwr_ssp_select = 0x0;  // each bit, 0: spi 1:MWR
    cfg.data_len = 0x1F;
    cfg.command_len = 0x7;      // to read 8 bit, include 1 additional bit
    cfg.command_only = 0;       // 1: command-only, 0: Normal
    cfg.read_write = SSP_CMD_READ; 
    ssp_set_config(&cfg);  
    CS_REG_WRITE(IROS_AHB_SSP_CA0, (SSP_SFLASH_OP_RDID<<24) ); 
    
    /*Check WIP done*/         
    ssp_sflash_triggle_cmd();
    CS_REG_READ(IROS_AHB_SSP_RDAT,val32);
    *data = val32;
        /* Manufacture ID */
    id[0] = (val32 & 0xFF000000) >> 24;
    /* Device ID0 */
    id[1] = (val32 & 0xFF0000) >> 16;
    /* Device ID1 */
    id[2] = (val32 & 0xFF00) >> 8;
    /* Extention Device ID0 */
    id[3] = (val32 & 0xFF);

    flash_id[0] = id[0] & 0xFF;
    flash_id[1] = (id[1] << 8) | id[2];
    #ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
    #endif
    
}

static void ssp_sflash_cfg_data(cs_uint32 data_len)
{
    ssp_intf_cfg_t cfg;

    memset(&cfg, 0, sizeof(cfg));
    cfg.speed_kHz = 0x10;
    cfg.ssp_select = 0x1;
    cfg.mwr_ssp_select = 0x0;   // each bit, 0: spi 1:MWR
    cfg.data_len = data_len;     // real len= data_len+1;
    cfg.command_len = 0x1F;      // to read 8 bit, include 1 additional bit
    cfg.command_only = 0;        // 1: command-only, 0: Normal
    cfg.read_write =  SSP_CMD_WRITE;      /* set it as write */
    ssp_set_config(&cfg);
}

static void sflash_write_byte(cs_uint32 _addr, cs_uint8 _data)
{
    ssp_sflash_cfg_cmd(SSP_SFLASH_OP_WREN); 
    
    ssp_sflash_cfg_data(SSP_PRE_DATA_CNT_8BIT);
    /* write the _data to cmd0 register */
    CS_REG_WRITE(IROS_AHB_SSP_CA0, (SSP_SFLASH_OP_PP<<24) | _addr);
    CS_REG_WRITE(IROS_AHB_SSP_WDAT, _data);
    
    ssp_sflash_triggle_cmd();
    ssp_sflash_write_done();

}


static void sflash_sector_erase(cs_uint32 s_addr) {
    
    AHB_SSP_CFG_t cfg;
    cs_uint32 sflash_addr;

    #ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
    #endif
    sflash_addr = s_addr - SFLASH_BASE;
    ssp_sflash_cfg_cmd(SSP_SFLASH_OP_WREN);

    ssp_sflash_cfg_data(SSP_PRE_DATA_CNT_8BIT);
    /* prepare cfg value */
    CS_REG_READ(IROS_AHB_SSP_CFG, cfg.wrd);
    cfg.bf.command_cyc = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CFG, cfg.wrd);    

    /* write the _data to cmd0 register */
    CS_REG_WRITE(IROS_AHB_SSP_CA0, (SSP_SFLASH_OP_SE<<24) | sflash_addr);   
    ssp_sflash_triggle_cmd();
    ssp_sflash_write_done();
    #ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
    #endif
}

static void sflash_block_write_force(cs_uint32 src, cs_uint32 dest, cs_uint32 len)
{
    
    cs_uint32 val32=0,write_len=0,first_write=TRUE;
    /* external SDRAM */
   if(src< 0x10000000)
    {
        src |= 0x10000000;        
    }
    dest -= SFLASH_BASE;
    cache_flush_data(src,len);
    CS_REG_WRITE(IROS_AHB_SSP_CLK,    0x10);     /* Set clock speed  */   
    CS_REG_WRITE(IROS_AHB_SSP_BST1,   dest);     /* write to sflash address*/
    CS_REG_WRITE(IROS_AHB_SSP_BST2,   src );     /* src address */ 
    
     while(len>0){
             cs_uint32 cnt = SSP_POLL_TIME; 
             write_len = (len > SFLASH_MAX_WRITE_PAGE_SIZE) ? SFLASH_MAX_WRITE_PAGE_SIZE : len;
             len -=write_len;
             
             /*WREN*/
             CS_REG_WRITE(IROS_AHB_SSP_CFG,  0x10000781);   // Set brust enable   
             CS_REG_WRITE(IROS_AHB_SSP_CA0, SSP_SFLASH_OP_WREN<<24);
             ssp_sflash_triggle_cmd();
     
             CS_REG_WRITE(IROS_AHB_SSP_BST0,  ((0x4e01<<16) | (write_len*8-1)) );  // Read from 8MB sflash page size is 8KB
    
             
             if(first_write){
                 CS_REG_WRITE(IROS_AHB_SSP_INT , 0x2);          // Clear up the done status
                 CS_REG_WRITE(IROS_AHB_SSP_CTRL,  0x40      );   // Trigger to start first burst operation
                 first_write=FALSE;
                 }
             else{
                 CS_REG_WRITE(IROS_AHB_SSP_INT , 0x2);               // Clear up the done status
                 CS_REG_WRITE(IROS_AHB_SSP_CTRL,  0x20      );  // Trigger to continue burst operation
             }

             while(cnt)
                {
                    cnt--;
                    CS_REG_READ(IROS_AHB_SSP_INT ,  val32);        // check bit1 set block access done  
                    if(BITON(val32, 1)) break;
                }
             if (cnt <= 0) {
                 return;
             }
             ssp_sflash_write_done();
             CS_REG_WRITE(IROS_AHB_SSP_INT,  0x2);              // Clear up the done status
    }

    return;
}

static void sflash_block_write(cs_uint32 src, cs_uint32 dest, cs_uint32 len)
{
    
     cs_uint32 val32=0,write_len=0,first_write=TRUE;
     cs_uint8* allign_ptr = NULL;

    if( (dest< SFLASH_BASE) || (dest > (SFLASH_BASE + SFLASH_SIZE)))
    {
     //   cs_printf("Overflow sflash address space, check dest address");
        return;
    }
    
    if(len > SFLASH_SIZE )
    {
      //  cs_printf("len Overflow, check len");
      
        return;
    }
    /* src should be 4-bytes alligned */
    if(src & 0x3)
    {
        if((allign_ptr = iros_malloc(IROS_MID_SYS, len+4)) != NULL)
        {
            cs_uint8 *tmp = NULL;
            tmp = (cs_uint8*)(((cs_uint32)allign_ptr+4) & 0xfffffffc);
            memcpy(tmp, (void*)src, len);
            src = (cs_uint32)tmp;
        }
        else
        {
            cs_printf("block write:malloc fail\n");
            return;
        }
    }
    
   /* Block write use DMA, flush data cache*/ 
   cache_flush_data(src,len);
    /* external SDRAM */
   if(src< 0x10000000)
    {
        src |= 0x10000000;        
    }
    dest -= SFLASH_BASE;
    
    #ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
    #endif     
     CS_REG_WRITE(IROS_AHB_SSP_CLK,    0x10);     /* Set clock speed  */   
     CS_REG_WRITE(IROS_AHB_SSP_BST1,   dest);     /* write to sflash address*/
     CS_REG_WRITE(IROS_AHB_SSP_BST2,   src );     /* src address */ 
    
     while(len>0){
             cs_uint32 cnt = SSP_POLL_TIME; 
             write_len = (len > SFLASH_MAX_WRITE_PAGE_SIZE) ? SFLASH_MAX_WRITE_PAGE_SIZE : len;
             len -=write_len;
             
             /*WREN*/
             CS_REG_WRITE(IROS_AHB_SSP_CFG,  0x10000781);   // Set brust enable   
             CS_REG_WRITE(IROS_AHB_SSP_CA0, SSP_SFLASH_OP_WREN<<24);
             ssp_sflash_triggle_cmd();
     
             CS_REG_WRITE(IROS_AHB_SSP_BST0,  ((0x4e01<<16) | (write_len*8-1)) );  // Read from 8MB sflash page size is 8KB
    
             
             if(first_write){
                 CS_REG_WRITE(IROS_AHB_SSP_INT , 0x2);          // Clear up the done status
                 CS_REG_WRITE(IROS_AHB_SSP_CTRL,  0x40      );   // Trigger to start first burst operation
                 first_write=FALSE;
                 }
             else{
                 CS_REG_WRITE(IROS_AHB_SSP_INT , 0x2);               // Clear up the done status
                 CS_REG_WRITE(IROS_AHB_SSP_CTRL,  0x20      );  // Trigger to continue burst operation
             }

             while(cnt)
                {
                    cnt--;
                    CS_REG_READ(IROS_AHB_SSP_INT ,  val32);        // check bit1 set block access done  
                    if(BITON(val32, 1)) break;
                }
             if (cnt <= 0) {
                // cs_printf("block access tiemout[0x%08x]\n",val32);
                 #ifndef IROSBOOT
                  cs_mutex_unlock(ssp_tran_lock);
                 #endif
                 if(allign_ptr != NULL)
                 {
                    iros_free(allign_ptr);
                 }
                 return;
             }
            // hal_delay_us (5000);
             ssp_sflash_write_done();
             CS_REG_WRITE(IROS_AHB_SSP_INT,  0x2);              // Clear up the done status
    }
    #ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
   #endif
   if(allign_ptr != NULL)
   {
        iros_free(allign_ptr);
   }
}

static void ssp_sflash_block_read(cs_uint32 src, cs_uint32 dest, cs_uint32 len)
{
    cs_uint32 val32=0,read_len=0,first_read=TRUE;
    if( (src< SFLASH_BASE) || (src > (SFLASH_BASE + SFLASH_SIZE)))
    {
       // cs_printf("Overflow sflash address space, check src address");
        return;
    }
    src -= SFLASH_BASE;
    if(len > SFLASH_SIZE )
    {
      //  cs_printf("len Overflow, check len");
        return;
    }
    
   /* Block read use DMA, flush data cache*/ 
   cache_flush_data(dest,len);
        /* external SDRAM */
   if(dest < 0x10000000)
    {
        dest |= 0x10000000;        
    }
    #ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
    #endif
    CS_REG_WRITE(IROS_AHB_SSP_CLK,  0x10 );        // Set clock speed
    CS_REG_WRITE(IROS_AHB_SSP_CFG,  0x10000781);   // Set brust enable    
    
    CS_REG_WRITE(IROS_AHB_SSP_BST1,   src        );   // read from sflash address 0
    CS_REG_WRITE(IROS_AHB_SSP_BST2,  (cs_uint32)dest);   // dest address 

    while(len>0){
            cs_uint32 cnt = SSP_POLL_TIME;            
            read_len = (len > SFLASH_MAX_READ_PAGE_SIZE) ? SFLASH_MAX_READ_PAGE_SIZE : len;
            len -=read_len;
            
            CS_REG_WRITE(IROS_AHB_SSP_BST0,  ((0x4f01<<16) | (read_len*8-1)) );  // Read from 8MB sflash page size 
            
            if(first_read){
                CS_REG_WRITE(IROS_AHB_SSP_INT,   0x2);               // Clear up the done status
                CS_REG_WRITE(IROS_AHB_SSP_CTRL,  0x40);       // Trigger to start first burst operation
                first_read=FALSE;
                }
            else{
                /* Wait read done */
                while(cnt)
                {
                    cnt--;
                    CS_REG_READ(IROS_AHB_SSP_BST_STAT ,val32);        // check bit1 set block access done  
                    if(SBST_ACC_RDONE == (val32 & SBST_ACC_MASK))
                        break;
                }
                if (cnt <= 0) {
                    //  cs_printf("block access timeout val[0x%08x]\n",val32);
                    #ifndef IROSBOOT
                    cs_mutex_unlock(ssp_tran_lock);
                    #endif
                    return;
                }
                CS_REG_WRITE(IROS_AHB_SSP_INT ,  0x2);               // Clear up the done status                   
                CS_REG_WRITE(IROS_AHB_SSP_CTRL,  0x20); 
            }
            cnt = SSP_POLL_TIME;
            while(cnt)
            {
                cnt--;
                CS_REG_READ(IROS_AHB_SSP_INT ,  val32);        // check bit1 set block access done  
                if(BITON(val32, 1)) break;
            }
            if (cnt <= 0) {
              //  cs_printf("block access timeout val[0x%08x]\n",val32);
                #ifndef IROSBOOT
                cs_mutex_unlock(ssp_tran_lock);
                #endif
                return;
            }
            CS_REG_WRITE(IROS_AHB_SSP_INT , 0x2);               // Clear up the done status            
   }  

   #ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
   #endif
}


/*****************************************************************************/
/* $rtn_hdr_start  flash_query                                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
void flash_query(void * data)
/*                                                                           */
/* INPUTS  : o block       Block address                                     */
/* OUTPUTS : ----                                                            */
/* RETURNS : o ---                                                           */
/* DESCRIPTION:                                                              */
/*     This function is used to erase a block                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    ssp_sflash_query((cs_uint32 * )data);
}

/*****************************************************************************/
/* $rtn_hdr_start  FLASH ERASE BLOCK                                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
int flash_erase_block(cs_uint32 s_addr,unsigned int block_size)
/*                                                                           */
/* INPUTS  : o block       Block address                                     */
/* OUTPUTS : ----                                                            */
/* RETURNS : o ---                                                           */
/* DESCRIPTION:                                                              */
/*     This function is used to erase a block                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{

    sflash_sector_erase(s_addr);

    return 0;
}
int flash_force_erase_block(cs_uint32 s_addr,unsigned int block_size)
{
    
    AHB_SSP_CFG_t cfg;
    cs_uint32 sflash_addr;

    sflash_addr = s_addr - SFLASH_BASE;
    ssp_sflash_cfg_cmd(SSP_SFLASH_OP_WREN);

    ssp_sflash_cfg_data(SSP_PRE_DATA_CNT_8BIT);
    /* prepare cfg value */
    CS_REG_READ(IROS_AHB_SSP_CFG, cfg.wrd);
    cfg.bf.command_cyc = 1;
    CS_REG_WRITE(IROS_AHB_SSP_CFG, cfg.wrd);    

    /* write the _data to cmd0 register */
    CS_REG_WRITE(IROS_AHB_SSP_CA0, (SSP_SFLASH_OP_SE<<24) | sflash_addr);   
    ssp_sflash_triggle_cmd();
    ssp_sflash_write_done();
    return 0;
}

void sflash_program_buf_word(cs_uint32 dest,cs_uint32 src,cs_uint32 len) 
{
    cs_uint32 writed_len=0; 

    if( (dest< SFLASH_BASE) || (dest > (SFLASH_BASE + SFLASH_SIZE)))
    {
        cs_printf("Overflow sflash address space, check src address");
        return;
    }
    if((len > SFLASH_SIZE ) || (0 == len))
    {
        cs_printf("check length");
        return;
    }
    dest -= SFLASH_BASE;
    
    #ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
    #endif

    while(writed_len<len)
    {
        sflash_write_byte((dest+writed_len), *(cs_uint8 *)src);
        src +=1;
        writed_len++;                
    }

    #ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
    #endif
}
int flash_program_buf_word(void* dest, cs_uint32 data)
{
    sflash_program_buf_word((cs_uint32)dest,(cs_uint32)&data,4);
    return 0;
}

int flash_program_buf_force(cs_uint32 dest, cs_uint32 src,cs_uint32 len,unsigned long block_mask, int buffer_size)
{
    sflash_block_write_force(src,dest,len);
    return 0;
}
/*****************************************************************************/
/* $rtn_hdr_start  FLASH ERASE BLOCK                                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
int flash_program_buf(cs_uint32 dest, cs_uint32 src,cs_uint32 len,unsigned long block_mask, int buffer_size)
/*                                                                           */
/* INPUTS  : o block       Block address                                     */
/* OUTPUTS : ----                                                            */
/* RETURNS : o ---                                                           */
/* DESCRIPTION:                                                              */
/*     This function is used to erase a block                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint32 first_write_len = 0,block_write_len = 0;
    cs_uint32 last_write_len = 0; /* process 16bytes unaligned at tail */

     if(0 == len){
        return 0;
     }
     
    /* block write length should be 16bytes aligned */
    if(len < 16)
    {
        sflash_program_buf_word(dest,src,len);
        return 0;
    }
    else
    {
        /* Source address should be 256 bytes aligned */
        first_write_len = (0x100 - (dest & 0xFF));
        /* process unaligned data */
        if(first_write_len >= len)
        {
            sflash_program_buf_word(dest,src,len);
            return 0;
         }
        else if(0x100 != first_write_len)
        {
            sflash_program_buf_word(dest,src,first_write_len);
        }
        else /* The dest is already 256 aligend */
        {
            first_write_len = 0;
        }
        /* now dest is 256 aligned */ 
        /* next check block write length should be 256bytes aligned*/
        block_write_len = len -first_write_len;
        last_write_len = block_write_len & 0xFF;
        block_write_len = len - first_write_len - last_write_len;
        sflash_block_write((src+first_write_len),(dest+first_write_len),block_write_len);

        /* write unaligned data */
        if(last_write_len > 0)
            sflash_program_buf_word((dest+first_write_len+block_write_len),
                                                    (src+first_write_len+block_write_len),last_write_len);
    }

    return 0;
}

void sflash_read(cs_uint32 dest, cs_uint32 src, cs_uint32 len)
{

    if( (src< SFLASH_BASE) || (src > (SFLASH_BASE + SFLASH_SIZE)))
    {
        cs_printf("Overflow sflash address space, check src address");
        return;
    }
    if(len > SFLASH_SIZE )
    {
        cs_printf("len Overflow, check len");
        return;
    }
#ifndef IROSBOOT
    cs_mutex_lock(ssp_tran_lock);
#endif
    memcpy((void *)dest, (const void *)src, len);   
#ifndef IROSBOOT
    cs_mutex_unlock(ssp_tran_lock);
#endif
}

void sflash_block_read(cs_uint32 dest, cs_uint32 src, cs_uint32 len)
{
    cs_uint32 byte_read_len = 0,block_read_len = 0;

    /*block write length must be 4bytes aligned*/
    byte_read_len = len % 4;
    block_read_len = len -byte_read_len;
    ssp_sflash_block_read(src,dest,block_read_len);

     /* write unaligned data */
     if(byte_read_len > 0)
        sflash_read((dest+block_read_len),(src+block_read_len),byte_read_len);
}


#ifndef IROSBOOT
static void sflash_write_test(cs_uint32 len)
{
    cs_uint32 block_num=0,loop=0;
    cs_uint64 ticks = 0;
    
    block_num = len/SFLASH_BLOCK_SIZE+1;
    /*Normal write*/
    SFLASH_DBG("*****************Normal write test%4d    KB Data start****************\n",len/0x400);
    SFLASH_DBG("1. Format memory buf\n");
    for(loop=0;loop < len; loop +=4)
        {
           *(cs_uint32 *)(MEM_BUF+loop)= rand();
        }
    SFLASH_DBG("2. Erase sflash\n");
    for(loop=start_block;loop < (start_block+block_num);loop++){
        ticks = cs_current_time();
        flash_erase_block((cs_uint32)(SFLASH_BASE + loop*SFLASH_BLOCK_SIZE),0);
        ticks = cs_current_time() - ticks;
        cs_printf("erase  ticks: %lld\n",ticks);
        SFLASH_DBG("Erase 0x%08x\n",loop*SFLASH_BLOCK_SIZE);
    } 
    SFLASH_DBG("3. Writing sflash\n");
    ticks = cs_current_time();
    sflash_program_buf_word(test_base,MEM_BUF, len);
    ticks = cs_current_time() - ticks;      
    SFLASH_DBG("4. Verifing writed data\n");
    if(do_memcmp((const void *) MEM_BUF, (const void *) test_base, len)){
        SFLASH_DBG("write verify passed\n");
        cs_printf("Normal write test %4d  KB Data src 0x%x  dest 0x%x len 0x%x ticks: %lld\n", len/0x400,MEM_BUF, test_base, len,ticks);
        }
    else
        cs_printf("Write verify failed\n");

    /*Block write*/
    SFLASH_DBG("Block write test %4d  KB Data Start\n",len/0x400);
    SFLASH_DBG("1. Format memory buf\n");
    for(loop=0;loop < len; loop +=4)
        {
           *(cs_uint32 *)(MEM_BUF+loop)= rand();
        }
    SFLASH_DBG("2. Erase sflash\n");
    for(loop=start_block;loop < (start_block+block_num);loop++){
        flash_erase_block((cs_uint32)(SFLASH_BASE + loop*SFLASH_BLOCK_SIZE),0);
        SFLASH_DBG("Erase 0x%08x\n",loop*SFLASH_BLOCK_SIZE);
    } 
    SFLASH_DBG("3. Writing sflash\n");
    ticks = cs_current_time();
    flash_program_buf(test_base,MEM_BUF, len,0,0);
    ticks = cs_current_time() - ticks;
    SFLASH_DBG("Block write src 0x%x  dest 0x%x len 0x%x ticks: %lld\n", MEM_BUF, test_base, len,ticks);
    SFLASH_DBG("4. Verifing writed data\n");
    if(do_memcmp((const void *) MEM_BUF, (const void *) test_base, len)){
        SFLASH_DBG("write verify passed\n");
        cs_printf("Block  write test %4d  KB Data src 0x%x  dest 0x%x len 0x%x ticks: %lld\n", len/0x400,MEM_BUF, test_base, len,ticks);        
        }
    else
        cs_printf("Block write verify failed\n");
}


static void sflash_read_test(cs_uint32 len)
{
    cs_uint64 ticks = 0;
    
    /*Normal READ*/
    SFLASH_DBG("Normal Read test %4d    KB Data Start\n",len/0x400);
    SFLASH_DBG("1. Format memory buf\n");
    memset((void *)MEM_BUF, 0, len); 

    SFLASH_DBG("2. Read from sflash\n");
    ticks = cs_current_time();
    sflash_read(MEM_BUF, test_base, len);
    ticks = cs_current_time() - ticks;
    SFLASH_DBG("Read src 0x%x  dest 0x%x len 0x%x ticks: %lld\n", test_base, MEM_BUF, len,ticks); 
    SFLASH_DBG("3. Verifing read data\n");
    if(do_memcmp((const void *) MEM_BUF, (const void *) test_base, len)){
        SFLASH_DBG("Read verify passed\n");
        cs_printf("Normal Read  test %4d  KB Data src 0x%x  dest 0x%x len 0x%x ticks: %lld\n", len/0x400,MEM_BUF, test_base, len,ticks);
        }
    else
        cs_printf("Read verify failed\n"); 
    
    /*Block READ*/
        SFLASH_DBG("Block Read test %4d  KB Data Start\n",len/0x400);
        SFLASH_DBG("1. Format memory buf\n");
        memset((void *)MEM_BUF, 0, len); 
        SFLASH_DBG("2. Read from sflash\n");
        ticks = cs_current_time();
        sflash_block_read(MEM_BUF, test_base, len);
        ticks = cs_current_time() - ticks;
        SFLASH_DBG("Block  Read src 0x%x  dest 0x%x len 0x%x ticks: %lld\n", test_base, MEM_BUF, len,ticks);  
        SFLASH_DBG("3. Verifing read data\n");
        if(do_memcmp((const void *) MEM_BUF, (const void *) test_base, len)){
            SFLASH_DBG("Block Read verify passed\n");
            cs_printf("Block  Read  test %4d  KB Data src 0x%x  dest 0x%x len 0x%x ticks: %lld\n", len/0x400,MEM_BUF, test_base, len,ticks);
        }
        else
            cs_printf("Block Read verify failed\n");
    
}


void sflash_test()
{
        cs_uint32 loop=0;
        
    while(sflash_test_len[loop] != 0 ){
        cs_printf("\nTest sflash %4d    KB R/W\n",sflash_test_len[loop]/0x400);
        sflash_read_test(sflash_test_len[loop]);
        sflash_write_test(sflash_test_len[loop]);
        loop++;
    }

}
#endif /* HAVE_SERIAL_FLASH */

#endif

