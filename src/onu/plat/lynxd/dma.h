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
 * dma.h
 *
 * $Id: dma.h,v 1.1.4.3.8.1.10.1 2011/08/18 10:29:34 zsun Exp $
 */

#ifndef _DMA_H_
#define _DMA_H_

#include "cs_types.h"
#include "cache.h"



// External SDRAM can be accessed by CPU via 0x0000_0000 and 0x1000_0000, they are both physical address.
// But DMA/ESDMA has the logic, only allow BADDR in the region started from 0x1000_0000/0x2c00_0000/0x2c50_0000.
// Following MACRO to convert address in SDRAM to a DMA allowed address.
// When SW heap is mapped to other memory (eg. ASYNC SRAM, 0x2000_0000), DMA_ADDR_PHYSICAL need to be updated.
#define DMA_ADDR_PHYSICAL(_addr)    (((cs_uint32)(_addr)) < EXTERNAL_SDRAM_START ? ((_addr)|EXTERNAL_SDRAM_START) : (_addr))

#define DMA_CHAN_BASE_PHYS_ADDR     0x2c600000
#define DMA_CHAN_SPACING            0x800  // 2K
#define DMA_CHAN_BUF_BASE(chan) \
            (DMA_CHAN_BASE_PHYS_ADDR + \
            (DMA_CHAN_SPACING*(chan)) )


#define DMA_CHAN_DEFAULT    0
#define DMA_CHAN_0          0
#define DMA_CHAN_1          1
#define DMA_CHAN_2          2

#define DMA_CHAN_CFG_POLL_PERIOD    0x100
#define DMA_CHAN_CTL_MOD_AUTO       1
#define DMA_CHAN_CTL_OP_COPY        0x2
#define DMA_CHAN_CTL_OP_ADDR_SRC    1
#define DMA_CHAN_CTL_OP_ADDR_DST    1

#define DMA_CHAN_INTR_BUF_CHAIN_ENABLE      1
#define DMA_CHAN_INTR_BUF_CHAIN_DISABLE     0
#define DMA_CHAN_INTR_TRANSACTION_ENABLE      1
#define DMA_CHAN_INTR_TRANSACTION_DISABLE     0

#define DMA_CHAN_TX_TIMEOUT     10 // tick

#define DMA_BUF_CTRL_HEADER_WITHIN_PKT      0
#define DMA_BUF_CTRL_HEADER_BY_SIDE_BAND    1

#define DMA_BYTE_SWAP_ENABLE    0x1
#define DMA_BYTE_SWAP_DISABLE   0x0

#define DMA_DPID_GE     0
#define DMA_DPID_EPON   1
#define DMA_DPID_CPU    4
#define DMA_DPID_DMA    6

#define ESDMA_BUF_COUNT         2
#define ESDMA_BUF_ID_0          0
#define ESDMA_BUF_ID_1          1

#define ESDMA_TOKEN_IN_BYTES    4

#define ESDMA_XFER_MODE_PKT     0
#define ESDMA_XFER_MODE_TOKEN   1


typedef struct
{
    cs_uint8* address;
    cs_uint32 token_num;
    cs_uint32 used_token;   // after read all, clear to zero
    cs_uint32 wrt_done;
} esdma_buf_desc_t;

extern void dma_engine_init();
extern void dma_chan_cfg(cs_uint32 s_addr, cs_uint32 d_addr, cs_uint32 byte_num);
extern void dma_chan_enable();
extern void dma_intr_hanlder();
extern void dma_pkt_ctrl(cs_uint8 bypass, cs_uint8 dpid, cs_uint8 cos);
extern cs_status dma_pkt_send(cs_uint8 *p_pkt, cs_uint32 len);

extern cs_status esdma_init();
extern void esdma_intr_hanlder();
extern cs_status esdma_auto_xfer_done();
extern cs_status esdma_setup(cs_uint8* buf_addr_0, cs_uint32 token_num_0, cs_uint8* buf_addr_1, cs_uint32 token_num_1);
extern cs_status esdma_get_buf_token(cs_uint8* _buf_idx, cs_uint32* _token_cnt);

#ifdef HAVE_DMA_TEST
extern void esdma_dump_buf(cs_uint8 buf_idx);
extern void dma_loop_init(cs_uint32 interval, cs_uint32 dir);
extern void dma_loop_setup_buffer();
extern void dma_loop_check(cs_uint32 num_check, cs_uint32 seq_check, cs_uint32 content_check, cs_uint32 content_dump, cs_uint32 dump_all);
extern void dma_loop_data_pattern(cs_uint32 pattern);
extern void dma_loop_len(cs_uint32 pkt_len, cs_uint32 seq_len);
extern void dma_loop_stat_dump();
extern void dma_loop_stat_clear();
extern esdma_buf_desc_t esdma_buf_desc[ESDMA_BUF_COUNT];
#endif


#endif

