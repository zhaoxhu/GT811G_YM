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
 * dma.c
 *
 * $Id: dma.c,v 1.1.4.5.2.2.10.1 2011/04/18 02:26:36 zsun Exp $
 */

/****************************************************************

OVERVIEW

This file contains the DMA driver code

****************************************************************/
#ifdef HAVE_DMA

#include "iros_config.h"
#include "registers.h"
#include "plat_common.h"
#include "dma.h"
#include "cs_fxs.h"


#ifdef _SI_NAV_HELPER_
extern L_DMA_t *g_dma_base;
extern MA_t *g_ma_base;
#else
#define g_dma_base  ((L_DMA_t *)&(g_lynxd_reg_ptr->l_dma))
#define g_ma_base   ((MA_t *)&(g_lynxd_reg_ptr->ma))
#endif

cyg_sem_t dma_tx_sem_chan0;
cs_uint32 dma_send_count = 0;
cs_uint32 dma_done = 1;

static void dma_chan_init(cs_uint8 _chan)
{
    if(DMA_CHAN_0 == _chan)
    {
        g_dma_base->PER0.bf.poll_per = DMA_CHAN_CFG_POLL_PERIOD; // HW polling period, not matter for SW

        g_dma_base->IE0.bf.tce = DMA_CHAN_INTR_TRANSACTION_DISABLE; //  individual transaction complete interrupt
        g_dma_base->IE0.bf.bce = DMA_CHAN_INTR_BUF_CHAIN_DISABLE;
        g_dma_base->IE0.bf.ahb_erre = 0;

        g_dma_base->CTL0.bf.autoi = DMA_CHAN_CTL_MOD_AUTO; // Auto-initialization mode
        g_dma_base->CTL0.bf.instr = DMA_CHAN_CTL_OP_COPY; // Copy operation
        g_dma_base->CTL0.bf.sai = DMA_CHAN_CTL_OP_ADDR_SRC; // Increment
        g_dma_base->CTL0.bf.dai = DMA_CHAN_CTL_OP_ADDR_DST; // Increment
    }
    else
    {
        // TBD: other chan init
        IROS_LOG_CRI(IROS_MID_DMA, "DMA only support chan 0\n");
    }
}

static void dma_intr_clear()
{
    MA_INT_t  int_stat, int_stat_new;
    MA_INT_EN_t int_en;

    int_stat.wrd = g_ma_base->INT.wrd;
    int_en.wrd   = g_ma_base->INT_EN.wrd;

    // init to zero, not to clear other bit
    int_stat_new.wrd = 0;

    // write 1 to clear existing intr
    if(int_en.bf.dma0_tx_done_i && int_stat.bf.dma0_tx_done_i)
    {
        int_stat_new.bf.dma0_tx_done_i = 1;
    }

    if(int_en.bf.dma1_tx_done_i && int_stat.bf.dma1_tx_done_i)
    {
        int_stat_new.bf.dma1_tx_done_i = 1;
    }

    g_ma_base->INT.wrd = int_stat_new.wrd;

}

static void dma_intr_enable()
{
    MA_INT_EN_t int_en;

    int_en.wrd = g_ma_base->INT_EN.wrd;
    int_en.bf.dma0_tx_done_i = 1;
    g_ma_base->INT_EN.wrd = int_en.wrd;
}

void dma_intr_hanlder()
{
    MA_INT_t int_stat;

    // Check if DMA interrupt raised
    int_stat.wrd = g_ma_base->INT.wrd;

    // Handle the DMA interrupt
    if(int_stat.bf.dma0_tx_done_i)
    {
        onu_int_dma0_tx++;
        cyg_semaphore_post(&dma_tx_sem_chan0);
    }

    // clear the INRT status
    dma_intr_clear();
}


void dma_engine_init()
{
    MA_DMA0_BUF_CTRL_t buf_ctrl;

    cyg_semaphore_init(&dma_tx_sem_chan0, 1);

    dma_intr_enable();
    dma_intr_clear();

    buf_ctrl.wrd = g_ma_base->DMA0_BUF_CTRL.wrd;
    // pkt in buffer does not contain header
    buf_ctrl.bf.hd_sel = DMA_BUF_CTRL_HEADER_BY_SIDE_BAND;
    // byte swap always enable
    buf_ctrl.bf.byte_swap_en = DMA_BYTE_SWAP_ENABLE;
    buf_ctrl.bf.eop_byte_swap_en = DMA_BYTE_SWAP_DISABLE; // this bit will be removed in ASIC
    g_ma_base->DMA0_BUF_CTRL.wrd = buf_ctrl.wrd;

    dma_chan_init(DMA_CHAN_DEFAULT);
}


void dma_chan_cfg(cs_uint32 s_addr, cs_uint32 d_addr, cs_uint32 byte_num)
{
    g_dma_base->SA0.bf.sa = s_addr;
    g_dma_base->DA0.bf.da = d_addr;
    g_dma_base->NB0.bf.nb = byte_num;
}

void dma_chan_enable()
{
    L_DMA_IE0_t ie_cfg;
    L_DMA_CTL0_t ctl_cfg;

    // individual transaction complete interrupt
    ie_cfg.wrd = g_dma_base->IE0.wrd;
    ie_cfg.bf.tce = DMA_CHAN_INTR_TRANSACTION_ENABLE;
    g_dma_base->IE0.wrd = ie_cfg.wrd;

    // enable DMA engine
    ctl_cfg.wrd = g_dma_base->CTL0.wrd;
    ctl_cfg.bf.en = 1;
    g_dma_base->CTL0.wrd = ctl_cfg.wrd;
}

cs_status dma_pkt_send(cs_uint8 *p_pkt, cs_uint32 len)
{

    // Should guard only one dma send at a time
    if(cyg_semaphore_timed_wait(&dma_tx_sem_chan0, cyg_current_time()+ DMA_CHAN_TX_TIMEOUT))
    {
        dma_done = 1;
    }
    else if(g_ma_base->DMA0_BUF_CTRL.bf.pkt_rdy == 0)
    {
        dma_done = 1;
    }

    if(dma_done)
    {
        cache_flush_data((cs_uint32)p_pkt, len);

        dma_intr_clear();
        dma_chan_cfg(DMA_ADDR_PHYSICAL((cs_uint32)p_pkt), DMA_CHAN_BUF_BASE(0), len);
        dma_chan_enable();
        dma_done = 0;
        dma_send_count++;
        IROS_LOG_INF(IROS_MID_DMA, "DMA tx %d\n", dma_send_count);
    }
    else
    {
        IROS_LOG_CRI(IROS_MID_DMA, "DMA tx timeout\n");
    }

    return CS_E_OK;;
}

void dma_pkt_ctrl(cs_uint8 bypass, cs_uint8 dpid, cs_uint8 cos)
{
    MA_CPU_CTRL_t ma_cpu_ctrl;

    ma_cpu_ctrl.wrd = g_ma_base->CPU_CTRL.wrd;
    ma_cpu_ctrl.bf.bypass = ma_cpu_ctrl.bf.bypass | (bypass << 1); // DMA to FE look-up bypass enable
    ma_cpu_ctrl.bf.dma_dpid = dpid;
    ma_cpu_ctrl.bf.dma_cos = cos;
    g_ma_base->CPU_CTRL.wrd = ma_cpu_ctrl.wrd;
}


/* DESCRIPTION:                                                                    */
/* Here is how ESDMA works: SW setup ESDMA with two CPU buffer pointers and two    */
/* token value equal to the size of CPU buffers. DMA driver install one of the     */
/* buffers into ESDMA HW. Classification rule need to be installed to catch all    */
/* inbound packets with matching fields and send them to MII FIFO (2KB), the packet*/
/* will then be DMAed to CPU buffer as long as there are tokens avaliable in the   */
/* installed buffer. Application will need to poll the CPU buffer periodically and */
/* process received packets. When ESDMA run out of tokens, an interrupt will be    */
/* generated to CPU, CPU will then need to setup ESDMA with the second CPU buffer  */
/* pointer (CPU implements double buffering such that two packet buffers are used  */
/* by ESDMA alternatively). If Out-of-token interrupt come before CPU application  */
/* finish reading the buffer, an critical error needs to be logged (in that case,  */
/* we need in decrease the CPU polling interval or increase CPU DMA buffer size to */
/* alleviate the problem                                                           */

cs_uint8 esdma_buf_write = DMA_BUFFER_0;
cs_uint8 esdma_buf_read  = DMA_BUFFER_0;

esdma_buf_desc_t esdma_buf_desc[ESDMA_BUF_COUNT];

static cs_uint32 esdma_get_available_token()
{
    cs_uint32 token;
    token = g_ma_base->ESDMA_CTRL.bf.axfer_toc_bucket;
    return token;
}

static void esdma_set_token(cs_uint32 token)
{
    MA_ESDMA_ADRC_t esdma_adrc;

    // enable clear dest buffer, when new token loaded
    MA_ESDMA_CTRL_t esdma_ctrl;
    esdma_ctrl.wrd = g_ma_base->ESDMA_CTRL.wrd;
    esdma_ctrl.bf.clear_buf_en = 1;
    g_ma_base->ESDMA_CTRL.wrd = esdma_ctrl.wrd;

    // load token
    esdma_adrc.wrd = g_ma_base->ESDMA_ADRC.wrd;
    esdma_adrc.bf.axfer_tok = token;
    esdma_adrc.bf.axfer_load_tok = 1;
    g_ma_base->ESDMA_ADRC.wrd = esdma_adrc.wrd;

    // token load finished
    esdma_adrc.wrd = g_ma_base->ESDMA_ADRC.wrd;
    esdma_adrc.bf.axfer_load_tok = 0;
    g_ma_base->ESDMA_ADRC.wrd = esdma_adrc.wrd;
}

static void esdma_set_dest_addr(cs_uint32 addr)
{
    MA_ESDMA_BADDR_t base_addr;

    base_addr.wrd = g_ma_base->ESDMA_BADDR.wrd;
    base_addr.bf.baddr = addr;
    g_ma_base->ESDMA_BADDR.wrd = base_addr.wrd;
}

static void esdma_set_xfer_mode(cs_uint8 mode)
{
    MA_ESDMA_ADRC_t adrc;

    adrc.wrd = g_ma_base->ESDMA_ADRC.wrd;
    adrc.bf.pkt_axfer_en = 0x1 & mode;
    // byte swap always enable
    adrc.bf.byte_swap_en = DMA_BYTE_SWAP_ENABLE;
    g_ma_base->ESDMA_ADRC.wrd = adrc.wrd;
}

static void esdma_intr_clear()
{
    MA_INT_t int_stat, int_stat_new;
    MA_INT_EN_t int_en;

    int_stat.wrd = g_ma_base->INT.wrd;
    int_en.wrd   = g_ma_base->INT_EN.wrd;

    // init to zero, not to clear other bit
    int_stat_new.wrd = 0;

    // write 1 to clear existing intr
    if(int_en.bf.auto_xfer_done && int_stat.bf.auto_xfer_done)
    {
        int_stat_new.bf.auto_xfer_done = 1;
    }

    if(int_en.bf.pkt_xfer2mem_done && int_stat.bf.pkt_xfer2mem_done)
    {
        int_stat_new.bf.pkt_xfer2mem_done = 1;
    }

    if(int_en.bf.pkt_xfer_done && int_stat.bf.pkt_xfer_done)
    {
        int_stat_new.bf.pkt_xfer_done = 1;
    }

    g_ma_base->INT.wrd = int_stat_new.wrd;

}


static void esdma_intr_enable()
{
    MA_INT_EN_t int_en;

    int_en.wrd = g_ma_base->INT_EN.wrd;
    int_en.bf.auto_xfer_done = 1;
    int_en.bf.pkt_xfer2mem_done = 1;
    int_en.bf.pkt_xfer_done = 1;
    g_ma_base->INT_EN.wrd = int_en.wrd;
}


void esdma_intr_hanlder()
{
    MA_INT_t int_stat;

    // Check if DMA interrupt raised
    int_stat.wrd = g_ma_base->INT.wrd;

    if(int_stat.bf.pkt_xfer2mem_done)
    {
        onu_int_esdma_xfer2mem++;
    }

    if(int_stat.bf.pkt_xfer_done)
    {
        onu_int_esdma_pkt_xfer++;
    }

    if(int_stat.bf.auto_xfer_done)
    {
        onu_int_esdma_auto_xfer++;
        esdma_auto_xfer_done();
    }

    // clear the INRT status
    esdma_intr_clear();
}


cs_status esdma_init()
{
    esdma_intr_enable();

    /* clear existing interrupt */
    esdma_intr_clear();

    /* setup buffer descriptor */
    memset(esdma_buf_desc, 0, sizeof(esdma_buf_desc_t) * DMA_BUFFER_COUNT);
    esdma_buf_write = DMA_BUFFER_0;
    esdma_buf_read  = DMA_BUFFER_0;

    return CS_E_OK;
}

cs_status esdma_setup(cs_uint8* buf_addr_0, cs_uint32 token_num_0, cs_uint8* buf_addr_1, cs_uint32 token_num_1)
{

    if(!buf_addr_0 || !token_num_0 || !buf_addr_1 || !token_num_1)
        return CS_E_PARAM;

    esdma_buf_desc[DMA_BUFFER_0].address   = buf_addr_0;
    esdma_buf_desc[DMA_BUFFER_0].token_num = token_num_0;
    esdma_buf_desc[DMA_BUFFER_0].used_token= 0;
    esdma_buf_desc[DMA_BUFFER_0].wrt_done  = 0;

    esdma_buf_desc[DMA_BUFFER_1].address   = buf_addr_1;
    esdma_buf_desc[DMA_BUFFER_1].token_num = token_num_1;
    esdma_buf_desc[DMA_BUFFER_1].used_token= 0;
    esdma_buf_desc[DMA_BUFFER_1].wrt_done  = 0;

    esdma_buf_write = DMA_BUFFER_0;
    esdma_buf_read  = DMA_BUFFER_0;

    // setup ESDMA token with buf 0
    esdma_set_dest_addr(DMA_ADDR_PHYSICAL((cs_uint32)esdma_buf_desc[DMA_BUFFER_0].address));
    esdma_set_xfer_mode(ESDMA_XFER_MODE_TOKEN);
    esdma_set_token(esdma_buf_desc[DMA_BUFFER_0].token_num);

    return CS_E_OK;
}



cs_status esdma_auto_xfer_done()
{
    IROS_LOG_INF(IROS_MID_ESDMA, "ES: wrt done %d, read: %d\n", esdma_buf_write, esdma_buf_read);
    if(esdma_buf_write != esdma_buf_read)
    {
        // HW write already consume the pair buffer
        // while SW is reading current buffer, not finished
        // esdma_buf_full = esdma_buf_read;
        esdma_buf_desc[esdma_buf_write].wrt_done = 1;
        IROS_LOG_INF(IROS_MID_ESDMA, "ES: No buf to wrt, cur read %d\n", esdma_buf_read);
    }
    else
    {
        // xfer done, so the buffer is avaliable for read
        // record token count used
        esdma_buf_desc[esdma_buf_write].used_token = esdma_buf_desc[esdma_buf_write].token_num - esdma_get_available_token();
        esdma_buf_desc[esdma_buf_write].wrt_done = 1;
        IROS_LOG_INF(IROS_MID_ESDMA, "ES: wrt done on buf %d token %d\n", esdma_buf_write, esdma_buf_desc[esdma_buf_write].used_token);

        // prepare the pair buffer for write
        esdma_buf_write = (esdma_buf_read == DMA_BUFFER_0) ? DMA_BUFFER_1 : DMA_BUFFER_0;
        esdma_buf_desc[esdma_buf_write].used_token = 0;
        esdma_buf_desc[esdma_buf_write].wrt_done = 0;

        // don't need to clear buffer
        // HW pipe line may delay this command, and caused buffer is cleared after recevied data
        // memset(esdma_buf_desc[esdma_buf_write].address, 0, esdma_buf_desc[esdma_buf_write].token_num * ESDMA_TOKEN_IN_BYTES);

        // configure HW to use new buffer
        esdma_set_dest_addr(DMA_ADDR_PHYSICAL((cs_uint32)esdma_buf_desc[esdma_buf_write].address));
        esdma_set_xfer_mode(ESDMA_XFER_MODE_TOKEN);
        esdma_set_token(esdma_buf_desc[esdma_buf_write].token_num);
        IROS_LOG_INF(IROS_MID_ESDMA, "ES: buf %d ready to wrt\n", esdma_buf_write);
    }

    return CS_E_OK;
}


// read buffer index is changed by the caller, unless all data in the buffer was read. Then switch to HW ESDMA buffer
// write buffer index is changed by HW via INTR
// caller need to pass current read buffer idx and current read token count
cs_status esdma_get_buf_token(cs_uint8* _buf_idx, cs_uint32* _token_cnt)
{
    cs_uint8 cur_read;

    if(!_buf_idx || !_token_cnt)
        return CS_E_PARAM;

    IROS_LOG_INF(IROS_MID_ESDMA, "ES: buf %d token %d\n", *_buf_idx, *_token_cnt);

    //esdma_buf_read = *_buf_idx;

    if(esdma_buf_read != esdma_buf_write)
    {
        // read buffer and write buffer are different
        if(esdma_buf_desc[esdma_buf_read].used_token &&
            *_token_cnt == esdma_buf_desc[esdma_buf_read].used_token)
        {
            // the token count passed in indicate last time buffer read terminated at,
            // if it readched the total token, no more data to read from current read buffer,
            // need to switch buffer

            cur_read = esdma_buf_read;

            // continue to read from old write buf
            esdma_buf_read = esdma_buf_write;
            esdma_buf_desc[esdma_buf_read].used_token = esdma_buf_desc[esdma_buf_read].token_num - esdma_get_available_token();

            *_buf_idx = esdma_buf_read;
            *_token_cnt = esdma_buf_desc[*_buf_idx].used_token;
            IROS_LOG_INF(IROS_MID_ESDMA, "ES: read toggle to buf %d token %d\n", *_buf_idx, *_token_cnt);

            if(esdma_buf_desc[esdma_buf_write].wrt_done)
            {
                // old read buf can be written now
                esdma_buf_write = cur_read;
                esdma_buf_desc[esdma_buf_write].used_token = 0;
                esdma_buf_desc[esdma_buf_write].wrt_done = 0;
                // memset(esdma_buf_desc[esdma_buf_write].address, 0, esdma_buf_desc[esdma_buf_write].token_num * ESDMA_TOKEN_IN_BYTES);

                esdma_set_dest_addr(DMA_ADDR_PHYSICAL((cs_uint32)esdma_buf_desc[esdma_buf_write].address));
                esdma_set_xfer_mode(ESDMA_XFER_MODE_TOKEN);
                esdma_set_token(esdma_buf_desc[esdma_buf_write].token_num);

                IROS_LOG_INF(IROS_MID_ESDMA, "ES: buf %d ready to write\n", esdma_buf_write);
            }
        }
        else
        {
            // read not finished, return current read buf
            *_buf_idx = esdma_buf_read;
            *_token_cnt = esdma_buf_desc[*_buf_idx].used_token;
            IROS_LOG_INF(IROS_MID_ESDMA, "ES: read buf %d, token %d\n", *_buf_idx, *_token_cnt);
        }
    }
    else
    {
        *_token_cnt = esdma_buf_desc[esdma_buf_read].token_num - esdma_get_available_token();
        IROS_LOG_INF(IROS_MID_ESDMA, "ES: same buf, token %d\n", *_token_cnt);
    }

    return CS_E_OK;
}


#ifdef HAVE_DMA_TEST

#include "aal.h"

#define RTP_PKT_SIZE            (RTP_HEADER_LEN + RTP_PAYLOAD_LEN + RTP_CRC_LEN) //220
#define RTP_HEADER_LEN          42
#define RTP_CRC_LEN             4
#define RTP_PAYLOAD_LEN         174
#define RTP_PAYLOAD_DATA_LEN    170
#define RTP_PAYLOAD_SEQ_LEN     4

typedef enum {
    DMA_LOOP_DIR_TX = 0,
    DMA_LOOP_DIR_RX = 1,
    DMA_LOOP_DIR_BI = 2,
    DMA_LOOP_DIR_MAX
} DMA_LOOP_DIR_E;

#define DMA_PKT_HEADER_LEN  4

typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN
    cs_uint32 reserved             :  7 ;
    cs_uint32 spid                 :  3 ;
    cs_uint32 pkt_type             :  5 ;
    cs_uint32 dpid                 :  3 ;
    cs_uint32 cos                  :  2 ;
    cs_uint32 pkt_len              : 12 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_len              : 12 ; /* bits 11:0 */
    cs_uint32 cos                  :  2 ; /* bits 13:12 */
    cs_uint32 dpid                 :  3 ; /* bits 16:14 */
    cs_uint32 pkt_type             :  5 ; /* bits 21:17 */
    cs_uint32 spid                 :  3 ; /* bits 24:22 */
    cs_uint32 reserved             :  7 ; /* bits 31:25 */
#   endif
  } bf ;
  cs_uint32   wrd;
} DMA_PKT_HEADER_t;

//default seq start from 212, 4 bytes
cs_uint32 rtp_seq_len = 4;
cs_uint32 rtp_seq_start = 212;

// The source/destination buffer for DMA Engine should be word aligned
// there are two ways below can achieve

// 1. GNU compiler extension on ARM
//#define __align__ __attribute__ ((aligned (4)))
// 2. buffer definition with cs_uint32
//cs_uint32 dma_pkt_buf_align[DMA_PKT_SIZE/4] = {0, };

// packet in network order
static cs_uint8 rtp_pkt_buf[(RTP_PKT_SIZE + DCACHE_LINE_SIZE - 1) & ~(DCACHE_LINE_SIZE - 1)] CACHE_ALIGNED =
{
    0x00,0x13,0x25,0x00,0x00,0x11,0x00,0x13,0x25,0x00,0x00,0x22,0x08,0x00,0x45,0x00,
    0x00,0xc8,0x00,0x00,0x00,0x00,0x40,0x11,0xda,0x9a,0xc8,0x39,0x07,0xcc,0xc8,0x39,
    0x07,0x4c,0x1f,0x40,0x12,0x34,0x00,0xb4,0xbb,0x81,0x00,0x01,0x02,0x03,0x04,0x05,
    0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,
    0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,
    0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,
    0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,0x44,0x45,
    0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,
    0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,
    0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,
    0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,0x80,0x81,0x82,0x83,0x84,0x85,
    0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,0x95,
    0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,
    0xa6,0xa7,0xa8,0xa9,0x00,0x00,0x00,0x00,0x94,0x8f,0xae,0x4b
};
// last 4 bytes will be filled by HW with CRC
// except CRC, last two/four bytes will hold sequence id
// when len = 218, 2bytes for seq; len = 220, 4bytes for seq.

// data buffer in the packet
static cs_uint8 rtp_pkt_data[RTP_PAYLOAD_DATA_LEN] =
{
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9
};

// buffer in external SDRAM for receive from ESDMA
#define RECV_BUF_LEN        4096
static cs_uint8 recv_buffer[ESDMA_BUF_COUNT][RECV_BUF_LEN] CACHE_ALIGNED;


void esdma_dump_buf(cs_uint8 buf_idx)
{
    cs_uint8 *read_cur;
    cs_uint8 *read_end;
    cs_uint32 pkt_count = 0;
    cs_uint32 pkt_len;
    DMA_PKT_HEADER_t pkt_header;

    // packet always locate at the beginning of buffer
    read_cur = esdma_buf_desc[buf_idx].address;
    read_end = esdma_buf_desc[buf_idx].address + esdma_buf_desc[buf_idx].token_num * ESDMA_TOKEN_IN_BYTES;

    while(read_cur < read_end)
    {
        pkt_count++;

        memcpy((cs_uint8*)&pkt_header.wrd, read_cur, DMA_PKT_HEADER_LEN);
        pkt_len = pkt_header.bf.pkt_len;
        diag_printf("pkt %d header 0x%08x -> len %d, DPID %d, SPID %d\n",
            pkt_count, pkt_header.wrd, pkt_len, pkt_header.bf.dpid, pkt_header.bf.spid);

        if(0 == pkt_len)
        {
            pkt_count--;
            diag_printf("END of buf at %p\n", read_cur);
            break;
        }

        // skip the header
        read_cur += DMA_PKT_HEADER_LEN;

        // guard pkt header and body not in the same buffer
        if(read_cur >= read_end || read_cur + pkt_len >= read_end)
        {
            pkt_count--;
            diag_printf("Pkt body exceed max buf len\n");
            break;
        }

        data_dump(read_cur, pkt_len, read_cur);

        // skip the pkt body
        // pkt buffer always be 4-bytes align, skip the padding added by HW
        if(pkt_len % ESDMA_TOKEN_IN_BYTES)
        {
            read_cur += (pkt_len/ESDMA_TOKEN_IN_BYTES + 1) * ESDMA_TOKEN_IN_BYTES;
        }
        else
        {
            read_cur += pkt_len;
        }
    }

    diag_printf("Total pkt: %d\n", pkt_count);
}


/* DMA/ESDMA loop back test */

// debug statistic
cs_uint32 cfg_pkt_len = RTP_PKT_SIZE;
cs_uint32 counter_tx = 0;
cs_uint32 counter_rx = 0;
cs_uint32 pkt_num_handled = 0;
cs_uint32 pkt_num_corrupt = 0;
cs_uint32 pkt_num_misorder = 0;
cs_uint32 pkt_num_seq_err = 0;
cs_uint32 cur_seq = 0;
cs_uint32 seq_max = 0;

// debug switch
cs_uint32 check_pkt_all = 0;
cs_uint32 check_pkt_num_hanlded = 0;
cs_uint32 check_pkt_seq = 0;
cs_uint32 check_pkt_content = 0;
cs_uint32 check_pkt_content_dump = 0;
cs_uint32 check_pkt_data_dynamic = 0;

// app read ptr
cs_uint8 rx_buf_idx = 0;
cs_uint32 rx_token_cnt = 0;
cs_uint32 rx_token_offset = 0;


void dma_loop_rx()
{
    cs_uint8 cur_buf_idx = rx_buf_idx;
    DMA_PKT_HEADER_t pkt_header;
    cs_uint32 pkt_len;
    cs_uint32 token_of_pkt = 0;
    cs_uint8* cur_read = 0;
    cs_uint8 *seq_ptr = NULL;
    cs_uint32 cmp_len = 0;
    cs_uint32 seq_rev = 0;

    esdma_get_buf_token(&rx_buf_idx, &rx_token_cnt);

    if(rx_token_cnt > esdma_buf_desc[ESDMA_BUF_ID_0].token_num)
    {
        IROS_LOG_CRI(IROS_MID_ESDMA, "APP: Token num larger than max\n");
        return;
    }

    if(cur_buf_idx != rx_buf_idx)
    {
        rx_token_offset = 0;
        IROS_LOG_INF(IROS_MID_ESDMA, "APP: switch, start from 0\n");
    }

    pkt_num_handled = 0;

    while(rx_token_offset < rx_token_cnt)
    {
        counter_rx++;

#if 0
        // if MMU did not support to map address to uncached region, then invalidate the buffer
        cache_invalidate_data((cs_uint32)esdma_buf_desc[rx_buf_idx].address,
            esdma_buf_desc[rx_buf_idx].token_num*ESDMA_TOKEN_IN_BYTES);
#endif

        cur_read = (cs_uint8 *)UNCACHED_ADDRESS(esdma_buf_desc[rx_buf_idx].address + rx_token_offset*4);

        memcpy((cs_uint8*)&pkt_header.wrd, cur_read, DMA_PKT_HEADER_LEN);
        rx_token_offset += 1;

        if(0 == cfg_pkt_len)
        {
            pkt_len = pkt_header.bf.pkt_len;
            if(pkt_len != RTP_PKT_SIZE)
                IROS_LOG_CRI(IROS_MID_ESDMA, "APP: wrong pkt_len = %d\n", pkt_len);
        }
        else
        {
            // ignore the pkt len in header
            pkt_len = cfg_pkt_len;
        }

        IROS_LOG_INF(IROS_MID_ESDMA, "pkt %d header 0x%08x -> len %d, DPID %d, SPID %d\n",
            counter_rx, pkt_header.wrd, pkt_len, pkt_header.bf.dpid, pkt_header.bf.spid);

        if(pkt_len % ESDMA_TOKEN_IN_BYTES)
        {
            token_of_pkt = pkt_len/ESDMA_TOKEN_IN_BYTES + 1;
        }
        else
        {
            token_of_pkt = pkt_len/ESDMA_TOKEN_IN_BYTES;
        }

        cur_read = (cs_uint8 *)UNCACHED_ADDRESS(esdma_buf_desc[rx_buf_idx].address + rx_token_offset*4);

        cur_seq++;

        if(check_pkt_all)
        {
            diag_printf("Read pkt %d: %p, id %d, off %d\n", cur_seq, cur_read, rx_buf_idx, rx_token_offset*4);
            data_dump(cur_read, pkt_len, cur_read);
        }

        // check pkt content
        if(check_pkt_content || check_pkt_seq)
        {
            if(check_pkt_seq)
            {
                // check sequence id
                seq_ptr = cur_read + rtp_seq_start;
                memcpy((cs_uint8*)&seq_rev, seq_ptr, sizeof(seq_rev));
                seq_rev = ntohl(seq_rev);
                //diag_printf("seq: 0x%x\n", seq_rev);

                if(seq_rev != cur_seq)
                {
                    IROS_LOG_CRI(IROS_MID_ESDMA, "seq cur: 0x%x, recv: 0x%x\n", cur_seq, seq_rev);
                    pkt_num_seq_err++;

                    // if seq mismatch then back up one entry
                    diag_printf("==> rx tok_offset=%d   tok_cnt=%d    seq_cur=%d  recv=%d   (%d)\n",
                                rx_token_offset, rx_token_cnt, cur_seq, seq_rev, cur_seq-seq_rev);
                }

                // when 4 bytes hold seq. id, record the mis-rorder pkt
                if(rtp_seq_len == 4)
                {
                    if(seq_rev <= seq_max)
                    {
                        pkt_num_misorder++;
                    }
                    else
                    {
                        seq_max = seq_rev;
                    }
                }
            }

            if(check_pkt_content)
            {
                // check pattern
                cmp_len = cfg_pkt_len - RTP_CRC_LEN - RTP_HEADER_LEN - rtp_seq_len; // last 1/2/4 byte is seq. id

                if(check_pkt_data_dynamic)
                {
                    // fill the data buffer with INC seq # before compare
                    memset(rtp_pkt_data, (cur_seq % 0xff), RTP_PAYLOAD_DATA_LEN);
                    memcpy(&rtp_pkt_buf[RTP_HEADER_LEN], rtp_pkt_data, RTP_PAYLOAD_DATA_LEN);
                }

                if(0 != memcmp(rtp_pkt_buf + RTP_HEADER_LEN, cur_read + RTP_HEADER_LEN, cmp_len))
                {
                    pkt_num_corrupt++;

                    IROS_LOG_CRI(IROS_MID_ESDMA, "APP: pkt corrupt\n");
                    if(check_pkt_content_dump)
                    {
                        diag_printf("RX pkt: \n");
                        data_dump(cur_read, RTP_PKT_SIZE, cur_read);
                        diag_printf("Expect pkt: \n");
                        data_dump(rtp_pkt_buf, RTP_PKT_SIZE, rtp_pkt_buf);

                        // only dump the first one, wait for manually enable it again
                        check_pkt_content_dump = 0;

                        diag_printf("==> rx tok_offset=%d   tok_cnt=%d\n", rx_token_offset, rx_token_cnt);
                    }
                }
            }
        }

        rx_token_offset += token_of_pkt;
        IROS_LOG_INF(IROS_MID_ESDMA, "APP: token offset %d\n", rx_token_offset);

        // record the pkt process in one loop, detecting burst
        pkt_num_handled++;
    }

    if(check_pkt_num_hanlded && pkt_num_handled >= 2)
    {
        IROS_LOG_CRI(IROS_MID_ESDMA, "RX: %d pkt in one loop\n", pkt_num_handled);
    }
}


void dma_loop_tx()
{
    cs_uint8 *seq = NULL;
    cs_uint32 pkt_len;
    cs_uint8 *tx_buffer;

    tx_buffer = (cs_uint8 *)UNCACHED_ADDRESS(rtp_pkt_buf);

    counter_tx++;

    pkt_len = cfg_pkt_len;
    seq = rtp_pkt_buf + rtp_seq_start;

    if(rtp_seq_len == 4)
        *((cs_uint32*)seq) = htonl(counter_tx);
    else if (rtp_seq_len == 2)
        *((cs_uint16*)seq) = htons(counter_tx % 0xffff);
    else
        *((cs_uint8*)seq) = counter_tx % 0xff;

    if(check_pkt_data_dynamic)
    {
        // fill the seq # to the data
        memset(rtp_pkt_data, (counter_tx % 0xff), RTP_PAYLOAD_DATA_LEN);
        memcpy(tx_buffer + RTP_HEADER_LEN, rtp_pkt_data, RTP_PAYLOAD_DATA_LEN);
    }

    if(check_pkt_content_dump) {
        diag_printf("TX pkt: \n");
        data_dump(tx_buffer, pkt_len, tx_buffer);
    }

    dma_pkt_send(rtp_pkt_buf, pkt_len);
}



void dma_loop_init(cs_uint32 interval, cs_uint32 dir)
{
    static cs_uint32 timer_id_tx = 0;
    static cs_uint32 timer_id_rx = 0;

    if(dir > DMA_LOOP_DIR_MAX)
    {
        diag_printf("dir wrong\n");
        return;
    }

    // FIXME, after AAL merge is done
    //onu_aal_rtp_data_ctrl_set(0, 0x1234, 1);
    //diag_printf("CLS rule applied\n");

    switch(dir)
    {
        case DMA_LOOP_DIR_TX:
            // interval at ms
            if(interval)
            {
                timer_id_tx = cs_circle_timer_add(interval, dma_loop_tx, NULL);
                diag_printf("TX timer 0x%x at interval: %d ms, pkt len %d\n", timer_id_tx, interval, cfg_pkt_len);
            }
            else
            {
                if(timer_id_tx != 0)
                {
                    cs_timer_del(timer_id_tx);
                    diag_printf("TX timer 0x%x deleted\n", timer_id_tx);
                }
            }
            break;

        case DMA_LOOP_DIR_RX:
            if(interval)
            {
                timer_id_rx = cs_circle_timer_add(interval, dma_loop_rx, NULL);
                diag_printf("RX timer 0x%x at interval: %d ms, pkt len %d\n", timer_id_tx, interval, cfg_pkt_len);
            }
            else
            {
                if(timer_id_rx != 0)
                {
                    cs_timer_del(timer_id_rx);
                    diag_printf("RX timer 0x%x deleted\n", timer_id_rx);
                }
            }
            break;

        case DMA_LOOP_DIR_BI:
            if(interval)
            {
                timer_id_tx = cs_circle_timer_add(interval, dma_loop_tx, NULL);
                diag_printf("TX timer 0x%x at interval: %d ms\n", timer_id_tx, interval);
                timer_id_rx = cs_circle_timer_add(interval, dma_loop_rx, NULL);
                diag_printf("RX timer 0x%x at interval: %d ms\n", timer_id_rx, interval);
            }
            else
            {
                if(timer_id_tx != 0)
                {
                    cs_timer_del(timer_id_tx);
                    diag_printf("TX timer 0x%x deleted\n", timer_id_tx);
                }
                if(timer_id_rx != 0)
                {
                    cs_timer_del(timer_id_rx);
                    diag_printf("RX timer 0x%x deleted\n", timer_id_rx);
                }
            }
            break;

        default:
            diag_printf("invalid dir\n");
            break;
    }
}

void dma_loop_setup_buffer(cs_uint32 len)
{
    cs_uint8 *buf_addr_0;
    cs_uint8 *buf_addr_1;
    cs_uint32 buf_token, buf_len;

    if(len > RECV_BUF_LEN)
        buf_len = RECV_BUF_LEN;
    else
        buf_len = len;

    buf_token = buf_len / ESDMA_TOKEN_IN_BYTES;

    buf_addr_0 = &recv_buffer[DMA_BUFFER_0][0];
    buf_addr_1 = &recv_buffer[DMA_BUFFER_1][0];
    if(buf_addr_0 && buf_addr_0)
    {
        esdma_setup(buf_addr_0, buf_token, buf_addr_1, buf_token);
        diag_printf("ESDMA setup external mem: 0: %p T 0x%x, 1: %p, T 0x%x\n", buf_addr_0, buf_token, buf_addr_1, buf_token);
    }
    else
    {
        diag_printf("mem alloc failed\n");
    }
}

void dma_loop_check(cs_uint32 num_check, cs_uint32 seq_check, cs_uint32 content_check, cs_uint32 content_dump, cs_uint32 dump_all)
{
    if(num_check)
        check_pkt_num_hanlded = 1;
    else
        check_pkt_num_hanlded = 0;

    if(seq_check)
        check_pkt_seq = 1;
    else
        check_pkt_seq = 0;

    if(content_check)
        check_pkt_content = 1;
    else
        check_pkt_content = 0;

    if(content_dump)
        check_pkt_content_dump = 1;
    else
        check_pkt_content_dump = 0;

    if(dump_all)
        check_pkt_all = 1;
    else
        check_pkt_all = 0;
}

void dma_loop_data_pattern(cs_uint32 pattern)
{
    check_pkt_data_dynamic = pattern;
}

void dma_loop_len(cs_uint32 pkt_len, cs_uint32 seq_len)
{
    if(pkt_len == 0 || pkt_len > RTP_PKT_SIZE)
    {
        diag_printf("Invalid pkt len, 1-220\n");
        return;
    }
    cfg_pkt_len = pkt_len;

    if(seq_len != 1 && seq_len != 2 && seq_len != 4)
    {
        diag_printf("Wrong seq_len, valid is 1/2/4\n");
        return;
    }
    rtp_seq_len   = seq_len;


    rtp_seq_start = cfg_pkt_len - RTP_CRC_LEN - rtp_seq_len;
    diag_printf("seq. start from %d, seq_len %d\n", rtp_seq_start, rtp_seq_len);
}

void dma_loop_stat_dump()
{
    diag_printf("RX: %d\n", counter_rx);
    diag_printf("RX: %d pkt in one loop\n", pkt_num_handled);
    diag_printf("RX: %d pkt content error\n", pkt_num_corrupt);
    diag_printf("RX: %d pkt mis-order\n", pkt_num_misorder);
    diag_printf("RX: %d pkt seq error\n", pkt_num_seq_err);
    diag_printf("TX: %d\n", counter_tx);
}

void dma_loop_stat_clear()
{
    diag_printf("clear counters\n");
    counter_rx = 0;
    counter_tx = 0;
    pkt_num_handled = 0;
    pkt_num_corrupt = 0;
    pkt_num_misorder = 0;
    pkt_num_seq_err = 0;

    cur_seq = 0;
    seq_max = 0;

    dma_loop_stat_dump();
}

#endif

#endif
