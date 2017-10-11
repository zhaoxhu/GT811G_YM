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
 * ssp.h
 *
 * $Id: ssp.h,v 1.1.4.6.2.3.2.2.2.3 2011/08/02 09:13:55 ljin Exp $
 */

#ifndef _SSP_H_
#define _SSP_H_

#include "cs_types.h"

#define SFLASH_BLOCK_SIZE 0x10000  //64K

#define SSP_MAX_CMD_LEN     96
#define SSP_POLL_TIME       0x10000
#define SSP_CLOCK           100

#define SSP_MODE_NON_BLOCK  1
#define SSP_MODE_BLOCK      0

#define SSP_POLLING_MODE 1
#define SFLASH_MAX_READ_PAGE_SIZE   0x2000
#define SFLASH_MAX_WRITE_PAGE_SIZE  0x100
#define SBST_ACC_RDONE        0x0
#define SBST_ACC_MASK        0x7
#define SSP_SFLASH_OP_RDID  0x9F
#define SSP_SFLASH_OP_WREN  0x06
#define SSP_SFLASH_OP_PP    0x02
#define SSP_SFLASH_OP_SE    0xD8
#define SSP_SFLASH_OP_RDSR  0x05
#define SSP_SFLASH_SR_WIP  (0x1<<24)

#define SSP_EEPROM_OP_WRITE 0x5 //A5-A0 D7-D0 D15-D0 Write Address AN¨C A0
#define SSP_EEPROM_OP_READ  0x6 //A5-A0 Read Address AN¨C A0
#define SSP_EEPROM_OP_EWEN  0x98  //11XXXX Write Enable

#define SSP_CMD_WRITE   0
#define SSP_CMD_READ    1

#define SSP_RTL_OP_READ           3
#define SSP_RTL_OP_WRITE         2

#define SSP_PRE_DATA_CNT_8BIT   0x7
#define SSP_PRE_DATA_CNT_32BIT  0x1f


typedef struct {
    /* SSP Clock settings */
    cs_uint16 speed_kHz;
    /* Each of this bits enables one of the chip select lines of the SSP;
     * It supports only 3 bits */
    cs_uint8 ssp_select;
    /* Each of this bits configures the micro wire  chip select line
     * either SPI or micro wire.
     * value 0 - for SPI
     * value 1 - for Microwire
     */
    cs_uint8 mwr_ssp_select;
    cs_uint8 data_len;
    cs_uint8 command_len;
    cs_uint8 command_only;
    cs_uint8 edge_align;
    cs_uint8 datin_cmds;
    cs_uint8 idat_mode;
    cs_uint8 tdat_cpha;
    cs_uint8 read_write;
} ssp_intf_cfg_t;

typedef struct {
    cs_uint32 cmd[3];
    cs_uint32 data;
    cs_uint8 data_len;
    cs_uint8 command_len;
    cs_uint8 mode;  /*  0 - blocking or 1 - non-blocking */
    cs_uint8 pad;
} ssp_intf_data_t;

extern cs_int32 ssp_set_config(ssp_intf_cfg_t *cfg);
extern cs_int32 ssp_get_config(ssp_intf_cfg_t *cfg);
extern cs_int32 ssp_write(ssp_intf_data_t *data);
extern cs_int32 ssp_read(ssp_intf_data_t *data);
extern cs_int32 ssp_write_byte(cs_uint8 data);
extern cs_int32 ssp_read_byte(cs_uint8 *data);
extern void ssp_intr_hanlder();
extern void ssp_init(void);

extern cs_status ssp_slic_clock_set(cs_uint32 clk_khz);
extern cs_status ssp_slic_clock_get(cs_uint32 *clk_khz);
extern cs_int32 ssp_slic_write_byte(cs_uint8 data);
extern cs_int32 ssp_slic_read_byte(cs_uint8 *data);
extern void ssp_slic_reset(void);
extern cs_int32 ssp_test_all(void);

extern cs_status serdes_clock_set(cs_uint32 clk_khz);
extern cs_status serdes_clock_get(cs_uint32 *clk_khz);
extern cs_uint32 serdes_read(cs_uint8 raddr, cs_uint8 * rdata);
extern cs_uint32 serdes_write(cs_uint8 waddr, cs_uint8 wdata);

extern cs_status switch_clock_set(cs_uint32 clk_khz);
extern cs_status switch_clock_get(cs_uint32 *clk_khz);
extern cs_uint32 switch_write(cs_uint32 waddr, cs_uint32 wdata);
extern cs_uint32 switch_read(cs_uint32 raddr, cs_uint32 * rdata);

extern cs_status ssp_eeprom_clock_set(cs_uint32 clk_khz);
extern cs_status ssp_eeprom_clock_get(cs_uint32 *clk_khz);
extern void ssp_eeprom_dump();
extern void ssp_eeprom_read(cs_uint8 e_addr, char *dest, cs_uint8 len);
extern void ssp_eeprom_write(cs_uint8 e_addr, char *src, cs_uint8 len);

extern void sflash_test();
extern void sflash_read(cs_uint32 dest, cs_uint32 src, cs_uint32 len);
extern void sflash_program_buf_word(cs_uint32 dest, cs_uint32 src, cs_uint32 len);
extern int flash_program_buf_word(void* dest, cs_uint32 data);
extern void sflash_block_read(cs_uint32 dest, cs_uint32 src, cs_uint32 len);
extern int flash_program_buf(cs_uint32 dest, cs_uint32 src,cs_uint32 len,unsigned long block_mask, int buffer_size);
extern int flash_program_buf_force(cs_uint32 dest, cs_uint32 src,cs_uint32 len,unsigned long block_mask, int buffer_size);
extern int flash_erase_block(cs_uint32 s_addr,unsigned int block_size) ;
extern int flash_force_erase_block(cs_uint32 s_addr,unsigned int block_size);
extern void flash_query(void * data);
#endif
