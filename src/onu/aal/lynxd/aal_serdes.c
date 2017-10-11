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
 * $Id: aal_serdes.c,v 1.1.2.7.4.6.2.1 2011/09/26 09:36:54 xyu Exp $
 */

#include "aal_serdes.h"
#include "cs_types.h"
#include "aal_util.h"
#include "ssp.h"
#include "plat_common.h"

#if 0
#define __SPI_TIMEOUT 1000
#define BIT_REV(v, r) \
do{\
    int s = 8 * sizeof(v) - 1;\
    for (v >>= 1; v; v >>= 1)\
    {\
      r <<= 1;\
      r |= v & 1;\
      s--;\
    }\
    r <<= s;\
}while(0)

/* serdes access mutex */
cs_uint32 __serdes_mutex;


cs_status __serdes_write(cs_uint8 addr, cs_uint8 data)
{
    cs_uint8 addr_rev, data_rev;
    cs_uint32 ca0 = 0;
    cs_uint32 wdat = 0;
    cs_uint32 timeout = __SPI_TIMEOUT;
    cs_uint8 done;
    AHB_SSP_CFG_t ssp_cfg;
    cs_status rt = CS_E_OK;

    // reverse address and set AHB_SSP_CA0
    BIT_REV(addr, addr_rev);
    ca0 |= addr_rev<<22;
    REG_WRITE(AHB_SSP_CA0, ca0);

    // reverse data and set AHB_SSP_WDAT
    BIT_REV(data, data_rev);
    wdat |= addr_rev;
    REG_WRITE(AHB_SSP_WDAT, wdat);

    cs_mutex_lock(__serdes_mutex);

    // config AHB_SSP_CFG 
    ssp_cfg.wrd = 0;
    ssp_cfg.bf.command_cyc = 0;
    ssp_cfg.bf.ssp_cmd_cnt = 9;
    ssp_cfg.bf.pre_ssp_dat_cnt = 7;
    ssp_cfg.bf.datin_cmds = 1;  // TODO: need confirm
    ssp_cfg.bf.sel_ssp_cs = 0x8; //  chip select: serdes spi device
    ssp_cfg.bf.tdat_cpha = 0;
    ssp_cfg.bf.edge_align = 0;
    ssp_cfg.bf.read_write = 0; // write
    REG_WRITE(AHB_SSP_CFG, ssp_cfg.wrd);

    // trigger access
    REG_FIELD_WRITE(AHB_SSP_CTRL, sspstart, 1);

    // wait for done
    do
    {
        REG_FIELD_READ(AHB_SSP_CTRL, sspdone, done);
    }while(!done && (--timeout));

    if(!done)
    {
        rt = CS_E_TIMEOUT;
        goto end;
    }
    
    REG_FIELD_WRITE(AHB_SSP_CTRL, sspdone, 1); // write 1 clear
end:
    cs_mutex_unlock(__serdes_mutex);
    return rt;
}


cs_status __serdes_read(cs_uint8 addr, cs_uint8 *data)
{
    if(NULL==data)
        return CS_E_PARAM;

    cs_uint8 addr_rev, data_rev, data_temp;
    cs_uint32 ca0 = 0;
    cs_uint32 rdata = 0;
    cs_uint32 timeout = __SPI_TIMEOUT;
    cs_uint8 done;
    AHB_SSP_CFG_t ssp_cfg;
    cs_status rt = CS_E_OK;

    // reverse address and set AHB_SSP_CA0
    BIT_REV(addr, addr_rev);
    ca0 |= ((0x1<<30)|(addr_rev<<22));
    REG_WRITE(AHB_SSP_CA0, ca0);

    cs_mutex_lock(__serdes_mutex);

    // config AHB_SSP_CFG 
    ssp_cfg.wrd = 0;
    ssp_cfg.bf.command_cyc = 0;
    ssp_cfg.bf.ssp_cmd_cnt = 9;
    ssp_cfg.bf.pre_ssp_dat_cnt = 7;
    ssp_cfg.bf.datin_cmds = 1;  // TODO: need confirm
    ssp_cfg.bf.sel_ssp_cs = 0x8; // chip select: serdes spi device
    ssp_cfg.bf.tdat_cpha = 0;
    ssp_cfg.bf.edge_align = 0;
    ssp_cfg.bf.read_write = 1; // read
    REG_WRITE(AHB_SSP_CFG, ssp_cfg.wrd);

    // trigger access
    REG_FIELD_WRITE(AHB_SSP_CTRL, sspstart, 1);

    // wait for done
    do
    {
        REG_FIELD_READ(AHB_SSP_CTRL, sspdone, done);
    }while(!done && (--timeout));

    if(!done)
    {
        rt = CS_E_TIMEOUT;
        goto end;
    }
    
    REG_FIELD_WRITE(AHB_SSP_CTRL, sspdone, 1); // write 1 clear

    // get data and reverse it
    REG_READ(AHB_SSP_RDAT, rdata);
    data_temp = rdata & 0xff; // get [7:0]
    BIT_REV(data_temp, data_rev);
    *data = data_rev;

end:
    cs_mutex_unlock(__serdes_mutex);
    return rt;
}
#endif


cs_status serdes_init(void)
{
    cs_status rt = CS_E_OK;
    cs_status pll_lock_timeout = 100000;
    cs_boolean pll_locked;
    cs_uint8 pre_emphasis=0;
    cs_uint8 swctrl_tx=0;
    cs_callback_context_t  context;

    serdes_init_t serdes_init_cfg =
    {
        0x00,    // txd_sel;
        0x01,    // swctrl_tx;
        0x00,    // speed_tx;
        0x03,    // hsclksel_tx;
        0x04,    // mode_rx;
        0x00,    // speed_rx;
        0x03,    // hsclksel_rx;
        0x02,    // refclk_uosel_pll;
        0x02,    // refclk_losel_pll;
        0x00,    // refclkin_sel_pll;
        0x01,    // hsclksel_pll;
        0x05,    // divsel_pll;
        0x02,    // clkosel_pll;
        0x04,    // test_pll;
        0x01,    // srstz_pmc;
        0x01,    // pdz_pmc;
        0x07,    // pdz_misc;
        0x01,    // r_cal_en;
        15  ,    // __wait_10us_0;
        0x01,    // pmc_start;
        200 ,    // __wait_150us;
        0x1f,    // pdz_pll;
        0x04,    // srstz_pll;
        150 ,    // __wait_100us;
        0x01,    // pll_control_0;
        20000,    // __wait_10ms;
        0x1f,    // pdz_rx;
        0x1f,    // pdz_tx;
        15  ,    // __wait_10us_1;
        0x03,    // srst_tx;
        0x01,    // srst_rx;
        10 ,    // __wait_4t;
        0x07     // srstz_digital;
    };

    // enable SSP interrupt
    //REG_FIELD_WRITE(AHB_SSP_IE, sspe, 1);

    // lower the clock to K-micro serdes, 1Mhz
    //REG_FIELD_WRITE(AHB_SSP_CLK, counter_load, 6);

    // enable SPI_IG
    cs_plat_ssp_serdes_write(context,0,0,0xe0, 0x01);
    
    rt = startup_config_read(CFG_ID_SERDES_SWCTRL_TX, 1, &swctrl_tx);
    if (rt == CS_E_NOT_FOUND){
        AAL_MIN_LOG("Read CFG_ID_SERDES_SWCTRL_TX from startup-cfg FAILED\n");
    } else if (rt == CS_E_OK) {
        if(swctrl_tx<4) /*valid var in startup config,Normal var range from 0-3*/
            serdes_init_cfg.swctrl_tx=swctrl_tx;
    } else {
        cs_halt("Read CFG_ID_SERDES_SWCTRL_TX from startup-cfg FAILED\n");
    }

    rt = startup_config_read(CFG_ID_SERDES_EMP_EN_TX, 1, &pre_emphasis);
    if (rt == CS_E_NOT_FOUND){
        AAL_MIN_LOG("Read CFG_ID_SERDES_EMP_EN_TX from startup-cfg FAILED\n");
    } else if (rt == CS_E_OK) {
        if(pre_emphasis<0x10) /*valid var in startup config,Normal var range from 0-0xF*/
            cs_plat_ssp_serdes_write(context,0,0,0x72,pre_emphasis);
    } else {
        cs_halt("Read CFG_ID_SERDES_EMP_EN_TX from startup-cfg FAILED\n");
    }

    cs_plat_ssp_serdes_write(context,0,0,0x10, serdes_init_cfg.txd_sel);
    cs_plat_ssp_serdes_write(context,0,0,0x71, serdes_init_cfg.swctrl_tx);
    cs_plat_ssp_serdes_write(context,0,0,0x7c, serdes_init_cfg.speed_tx);
    cs_plat_ssp_serdes_write(context,0,0,0x7d, serdes_init_cfg.hsclksel_tx);
    cs_plat_ssp_serdes_write(context,0,0,0x95, serdes_init_cfg.mode_rx);
    cs_plat_ssp_serdes_write(context,0,0,0x98, serdes_init_cfg.speed_rx);
    cs_plat_ssp_serdes_write(context,0,0,0x9d, serdes_init_cfg.hsclksel_rx);
    cs_plat_ssp_serdes_write(context,0,0,0xc0, serdes_init_cfg.refclk_uosel_pll);
    cs_plat_ssp_serdes_write(context,0,0,0xc1, serdes_init_cfg.refclk_losel_pll);
    cs_plat_ssp_serdes_write(context,0,0,0xc2, serdes_init_cfg.refclkin_sel_pll);
    cs_plat_ssp_serdes_write(context,0,0,0xc3, serdes_init_cfg.hsclksel_pll);
    cs_plat_ssp_serdes_write(context,0,0,0xc6, serdes_init_cfg.divsel_pll);
    cs_plat_ssp_serdes_write(context,0,0,0xc7, serdes_init_cfg.clkosel_pll);
    cs_plat_ssp_serdes_write(context,0,0,0xcf, serdes_init_cfg.test_pll);

    // enable SPI_COM
    cs_plat_ssp_serdes_write(context,0,0,0xe0, 0x02);
    cs_plat_ssp_serdes_write(context,0,0,0x00, serdes_init_cfg.srstz_pmc);
    cs_plat_ssp_serdes_write(context,0,0,0x08, serdes_init_cfg.pdz_pmc);
    cs_plat_ssp_serdes_write(context,0,0,0x09, serdes_init_cfg.pdz_misc);
    cs_plat_ssp_serdes_write(context,0,0,0x21, serdes_init_cfg.r_cal_en);
    hal_delay_us(serdes_init_cfg.__wait_10us_0);
    cs_plat_ssp_serdes_write(context,0,0,0x20, serdes_init_cfg.pmc_start);
    hal_delay_us(serdes_init_cfg.__wait_150us);

    // enable SPI_IG
    cs_plat_ssp_serdes_write(context,0,0,0xe0, 0x01);
    cs_plat_ssp_serdes_write(context,0,0,0x0a, serdes_init_cfg.pdz_pll);
    cs_plat_ssp_serdes_write(context,0,0,0x00, serdes_init_cfg.srstz_pll);

    // wait for PLL lock
    do
    {
        REG_FIELD_READ(GLOBAL_KM_SDS_STS, pll_lock, pll_locked);
    }while(!pll_locked && (--pll_lock_timeout));

    if(!pll_locked)
    {
        AAL_MAJ_LOG("PLL lock failed\n");
        return CS_E_ERROR;
    }
    else
        AAL_MAJ_LOG("PLL is locked\n");

    hal_delay_us(serdes_init_cfg.__wait_100us);
    cs_plat_ssp_serdes_write(context,0,0,0x60, serdes_init_cfg.pll_control_0);
    hal_delay_us(serdes_init_cfg.__wait_10ms);
    cs_plat_ssp_serdes_write(context,0,0,0x09, serdes_init_cfg.pdz_rx);
    cs_plat_ssp_serdes_write(context,0,0,0x08, serdes_init_cfg.pdz_tx);
    hal_delay_us(serdes_init_cfg.__wait_10us_1);
    cs_plat_ssp_serdes_write(context,0,0,0x01, serdes_init_cfg.srst_tx);
    cs_plat_ssp_serdes_write(context,0,0,0x02, serdes_init_cfg.srst_rx);
    hal_delay_us(serdes_init_cfg.__wait_4t);
    cs_plat_ssp_serdes_write(context,0,0,0x00, serdes_init_cfg.srstz_digital);

    // disble SSP interrupt
    //REG_FIELD_WRITE(AHB_SSP_IE, sspe, 0);

    // reset RSTZ_FIFO to serdes
    REG_FIELD_WRITE(GLOBAL_KM_SDS_CTRL, rstz_fifo, 0);
    hal_delay_us(10);
    REG_FIELD_WRITE(GLOBAL_KM_SDS_CTRL, rstz_fifo, 1);

    // delay 1ms ensure SerDes's stable
    hal_delay_us(1000);

    return rt;
}



