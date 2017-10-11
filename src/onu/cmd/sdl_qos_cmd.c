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

DEFINITIONS:  "DEVICE" means the Cortina Systems? Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems? SDK software.

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

Copyright (c) 2010 by Cortina Systems Incorporated
****************************************************************************/

#include "cs_cmd.h"


#ifdef HAVE_SDL_CMD

#include "aal.h"
#include "sdl_qos.h"
#include "cs_types.h"
#include "aal_flow.h"
#include "aal_uni.h"
#include "aal_pon.h"
#include "aal_bm.h"
#include "sdl_port.h"

cs_callback_context_t qos_context;
#define LLID_PORTID 0x40000000
#define QOS_DEVICE_ID 0

typedef struct {
    cs_uint32 offset;
    cs_uint8 *name;
} __sdl_counter_entry_t;


static cs_uint64 bm_tx[3] = {0};
static cs_uint64 bm_rx[3] = {0};
cs_uint64 fe_drp[3] = {0};
cs_uint64 bm_drp[3] = {0};
static cs_uint64 voq_drp[3] = {0};
static cs_uint64 mac_drp[3] = {0};
static cs_uint64 te_fwd[7] = {0};
static cs_uint64 te_drp[7] = {0};
static cs_uint64 mpcp_drp = 0;
extern cs_status aal_uni_mib_get(cs_aal_uni_mib_t  mib_id, cs_boolean  rc, cs_uint32 *hi, cs_uint32 *lo);
extern cs_status aal_pon_mib_reset(void);

void sdl_dump_us_fwd_stats(void)
{
    //onu_aal_ge_mib_t mib_id;
    cs_uint32  cnt_lo = 0;
    cs_uint32  cnt_hi = 0;
    cs_uint64  total_cnt = 0;
    cs_aal_rate_limit_msk_t rl_msk;
    cs_aal_rate_limit_t stats;

    diag_printf("\n######### upstream blocks forward statistic #########\n\n");
    aal_uni_mib_get(AAL_MIB_RxUCPktCnt, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    //if(total_cnt !=0){
    diag_printf("GE Rx UC pkt                       = %lld\n", total_cnt);
    //}

    aal_uni_mib_get(AAL_MIB_RxMCFrmCnt, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    //if(total_cnt !=0){
    diag_printf("GE Rx MC pkt                       = %lld\n", total_cnt);
    //}

    aal_uni_mib_get(AAL_MIB_RxBCFrmCnt, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    //  if(total_cnt !=0){
    diag_printf("GE Rx BC pkt                       = %lld\n", total_cnt);
    //}
    aal_uni_mib_get(AAL_MIB_RxOAMFrmCnt, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    //  if(total_cnt !=0){
    diag_printf("GE Rx to CPU pkt                   = %lld\n", total_cnt);
    //}
    aal_uni_mib_get(AAL_MIB_RxFrmCount, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    //    if(total_cnt !=0){
    diag_printf("GE Rx total pkt                    = %lld\n", total_cnt);
    //}

    rl_msk.u = 0;
    rl_msk.s.fwdcnt = 1;


    aal_flow_rate_limit_get(AAL_RATE_LIMIT_FLOW_0, &rl_msk, &stats);
    te_fwd[0] += stats.fwdcnt;
    diag_printf("GE->PON TE queue 0 pkt             = %lld\n", te_fwd[0]);

    
    aal_flow_rate_limit_get(AAL_RATE_LIMIT_FLOW_1, &rl_msk, &stats);
    te_fwd[1] += stats.fwdcnt;
    diag_printf("GE->PON TE queue 1 pkt             = %lld\n", te_fwd[1]);

    aal_flow_rate_limit_get(AAL_RATE_LIMIT_FLOW_2, &rl_msk, &stats);
    te_fwd[2] += stats.fwdcnt;
    diag_printf("GE->PON TE queue 2 pkt             = %lld\n", te_fwd[2]);

    
    aal_flow_rate_limit_get(AAL_RATE_LIMIT_FLOW_3, &rl_msk, &stats);
    te_fwd[3] += stats.fwdcnt;
    diag_printf("GE->PON TE queue 3 pkt             = %lld\n", te_fwd[3]);


    aal_flow_rate_limit_get(AAL_RATE_LIMIT_CPU_DEST, &rl_msk, &stats);
    te_fwd[5] += stats.fwdcnt;
    diag_printf("PON+GE->CPU TE forward pkt         = %lld\n", te_fwd[5]);

    cnt_hi = aal_get_bm_cnt(BM_PON_TX_PKT_CNTR);
    //if(cnt_hi != 0){
    bm_tx[1] += cnt_hi;
    diag_printf("GE+MA->PON_MAC pkt(include OAM)    = %lld\n", bm_tx[1]);
    //}


    cnt_hi = aal_get_bm_cnt(BM_CPU_TX_PKT_CNTR);
    bm_tx[2] += cnt_hi;
    //if(cnt_hi != 0){
    diag_printf("GE+PON->CPU pkt                    = %lld\n", bm_tx[2]);
    // }

    cnt_hi = aal_get_bm_cnt(BM_CPU_RX_PKT_CNTR);

    bm_rx[2] += cnt_hi;
    //if(cnt_hi != 0){
    diag_printf("CPU->GE+PON pkt                    = %lld\n", bm_rx[2]);
    //}

    total_cnt = 0;
    aal_pon_mib_get(FALSE, MIB_ID_TXUCFRAMECNT, &cnt_hi, &cnt_lo);
    total_cnt += cnt_hi;
    //if(cnt_hi != 0){
    diag_printf("PON Tx UC pkt                      = %d\n", cnt_hi);
    // }

    aal_pon_mib_get(FALSE, MIB_ID_PON_TXMCFRAMECNT, &cnt_hi, &cnt_lo);
    total_cnt += cnt_hi;
    //if(cnt_hi != 0){
    diag_printf("PON Tx MC pkt                      = %d\n", cnt_hi);
    //}

    aal_pon_mib_get(FALSE,MIB_ID_PON_TXBCFRAMECNT, &cnt_hi, &cnt_lo);
    total_cnt += cnt_hi;
    //if(cnt_hi != 0){
    diag_printf("PON Tx BC pkt                      = %d\n", cnt_hi);
    //}

    aal_pon_mib_get(FALSE, MIB_ID_PON_TXCTRLFRAMECNT, &cnt_hi, &cnt_lo);
    total_cnt += cnt_hi;
    //if(cnt_hi != 0){
    diag_printf("PON Tx OAM pkt                     = %d\n", cnt_hi);
    //}

    //if(total_cnt !=0){
    diag_printf("PON Tx total pkt                   = %lld\n", total_cnt);
    //}

}

void sdl_dump_ds_fwd_stats(void)
{
    //onu_aal_ge_mib_t mib_id;
    cs_uint32  cnt_lo = 0;
    cs_uint32  cnt_hi = 0;
    cs_uint64  total_cnt = 0;
    cs_uint64  total_cnt1 = 0;
    cs_aal_rate_limit_msk_t rl_msk;
    cs_aal_rate_limit_t stats;

    diag_printf("\n######### downstream blocks forward statistic #########\n\n");

    total_cnt = 0;
    aal_pon_mib_get(FALSE, MIB_ID_UCFRAMECNT, &cnt_hi, &cnt_lo);
    total_cnt += cnt_hi;
    // if(cnt_hi != 0){
    diag_printf("PON Rx UC pkt                      = %d\n", cnt_hi);
    //}

    aal_pon_mib_get(FALSE, MIB_ID_PON_MCFRAMECNT, &cnt_hi, &cnt_lo);
    total_cnt += cnt_hi;
    //if(cnt_hi != 0){
    diag_printf("PON Rx MC pkt                      = %d\n", cnt_hi);
    //}

    aal_pon_mib_get(FALSE, MIB_ID_PON_BCFRAMECNT, &cnt_hi, &cnt_lo);
    total_cnt += cnt_hi;
    //if(cnt_hi != 0){
    diag_printf("PON Rx BC pkt                      = %d\n", cnt_hi);
    //}

    aal_pon_mib_get(FALSE, MIB_ID_PON_CTRLFRAMECNT, &cnt_hi, &cnt_lo);
    total_cnt += cnt_hi;
    //if(cnt_hi != 0){
    diag_printf("PON Rx OAM pkt                     = %d\n", cnt_hi);

    //if(total_cnt !=0){
    diag_printf("PON Rx total pkt                   = %lld\n", total_cnt);
    //}

    //}

    rl_msk.u = 0;
    rl_msk.s.fwdcnt = 1;
    
    aal_flow_rate_limit_get(AAL_RATE_LIMIT_PON_SP, &rl_msk, &stats);
    te_fwd[4] += stats.fwdcnt;

    diag_printf("PON->GE TE forward pkt             = %lld\n", te_fwd[4]);

    aal_flow_rate_limit_get(AAL_RATE_LIMIT_CPU_DEST, &rl_msk, &stats);
    te_fwd[6] += stats.fwdcnt;
    diag_printf("GE+PON -> CPU TE forward pkt       = %lld\n", te_fwd[6]);

    cnt_hi = aal_get_bm_cnt(BM_GE_TX_PKT_CNTR);

    bm_tx[0] += cnt_hi;
    //if(cnt_hi != 0){
    diag_printf("MA+PON->GE_MAC Tx pkt              = %lld\n", bm_tx[0]);
    //}

    cnt_hi = aal_get_bm_cnt(BM_CPU_TX_PKT_CNTR);
    bm_tx[2] += cnt_hi;
    //if(cnt_hi != 0){
    diag_printf("GE+PON->MA Tx pkt                  = %lld\n", bm_tx[2]);
    //}

    cnt_hi = aal_get_bm_cnt(BM_CPU_RX_PKT_CNTR);
    bm_rx[2] += cnt_hi;
    //if(cnt_hi != 0){
    diag_printf("CPU->GE+PON pkt                    = %lld\n", bm_rx[2]);
    //}
#if 0
    onu_aal_ma_mib_get(AAL_MIB_RxFrmCount, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    //if(total_cnt !=0){
    diag_printf("MA Rx pkt                      = %lld\n", total_cnt);
    //}

    onu_aal_ma_mib_get(AAL_MIB_TxFrmCount, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    //if(total_cnt !=0){
    diag_printf("MA Tx pkt                      = %lld\n", total_cnt);
    //}
#endif
    // onu_aal_ge_mib_get(AAL_MIB_TxFrmCount,FALSE, &cnt_hi, &cnt_lo);
    //total_cnt = ((cs_uint64)cnt_hi <<32)|cnt_lo;
    //if(total_cnt !=0){
    //diag_printf("GE Tx pkt                  = %lld\n", total_cnt);
    //}
    total_cnt1 = 0;
    aal_uni_mib_get(AAL_MIB_TxUCPktCnt, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    total_cnt1 += total_cnt;
    //if(total_cnt !=0){
    diag_printf("GE Tx UC pkt                       = %lld\n", total_cnt);
    //}

    aal_uni_mib_get(AAL_MIB_TxMCFrmCnt, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    total_cnt1 += total_cnt;
    //if(total_cnt !=0){
    diag_printf("GE Tx MC pkt                       = %lld\n", total_cnt);
    //}

    aal_uni_mib_get(AAL_MIB_TxBCFrmCnt, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    total_cnt1 += total_cnt;
    //if(total_cnt !=0){
    diag_printf("GE Tx BC pkt                       = %lld\n", total_cnt);
    //}

    aal_uni_mib_get(AAL_MIB_TxOAMFrmCnt, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    total_cnt1 += total_cnt;
    //if(total_cnt !=0){
    diag_printf("GE Tx from CPU pkt                 = %lld\n", total_cnt);
    //}
    diag_printf("GE total Tx                        = %lld\n", total_cnt1);

}

void sdl_dump_us_drp_stats(void)
{
    //onu_aal_ge_mib_t mib_id;
    cs_uint32  cnt_lo = 0;
    cs_uint32  cnt_hi = 0;
    cs_uint64  total_cnt = 0;
#ifdef HAVE_SDL_CMD_NTT        
    cs_uint32 us_store_cnt = 0;
    cs_uint32 us_store_oam  = 0;
    cs_uint32 ge_cpu_pkt  = 0;
#endif    
    cs_aal_rate_limit_msk_t rl_msk;
    cs_aal_rate_limit_t stats;

    diag_printf("\n######### upstream blocks drop statistic #########\n\n");

    aal_uni_mib_get(AAL_MIB_RxCrcErrFrmCnt, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    // if (total_cnt != 0) {
    diag_printf("GE Rx CRC pkt                                  = %lld\n", total_cnt);
    //}
    aal_uni_mib_get(AAL_MIB_RxUndersizeFrmCnt, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    //if (total_cnt != 0) {
    diag_printf("GE Rx under size pkt                           = %lld\n", total_cnt);
    //}

    aal_uni_mib_get(AAL_MIB_RxRuntFrmCnt, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    //if (total_cnt != 0) {
    diag_printf("GE Rx under size with bad CRC                  = %lld\n", total_cnt);
    //}

    aal_uni_mib_get(AAL_MIB_RxOvSizeFrmCnt, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    //if (total_cnt != 0) {
    diag_printf("GE Rx over size pkt                            = %lld\n", total_cnt);
    //}

    aal_uni_mib_get(AAL_MIB_RxJabberFrmCnt, FALSE, &cnt_hi, &cnt_lo);
    total_cnt = ((cs_uint64)cnt_hi << 32) | cnt_lo;
    //if (total_cnt != 0) {
    diag_printf("GE Rx over size pkt with bad CRC               = %lld\n", total_cnt);
    //}

    cnt_hi = aal_get_bm_cnt(BM_GE_FE_DROP_PKT_CNTR);
    mac_drp[0] += cnt_hi;
    // if (mac_drp[0] != 0) {
    diag_printf("GE->FE drp cnt                                 = %lld\n", mac_drp[0]);
    //}

    cnt_hi = aal_get_bm_cnt(BM_CPU_FE_DROP_PKT_CNTR);
    mac_drp[2] += cnt_hi;
    //if (mac_drp[2] != 0) {
    diag_printf("CPU->FE drp cnt                                = %lld\n", mac_drp[2]);
    //}
    
    rl_msk.u = 0;
    rl_msk.s.pircnt = 1;


    aal_flow_rate_limit_get(AAL_RATE_LIMIT_FLOW_0, &rl_msk, &stats);
    te_drp[0] += stats.pircnt;
    //if (te_drp[0] != 0) {
    diag_printf("GE->PON TE queue 0 drop pkt                    = %lld\n", te_drp[0]);
    //}

    aal_flow_rate_limit_get(AAL_RATE_LIMIT_FLOW_1, &rl_msk, &stats);
    te_drp[1] += stats.pircnt;
      //if (te_drp[0] != 0) {
    diag_printf("GE->PON TE queue 1 drop pkt                    = %lld\n", te_drp[1]);
      
    aal_flow_rate_limit_get(AAL_RATE_LIMIT_FLOW_2, &rl_msk, &stats);
    te_drp[2] += stats.pircnt;
        //if (te_drp[0] != 0) {
    diag_printf("GE->PON TE queue 2 drop pkt                    = %lld\n", te_drp[2]);
        
    aal_flow_rate_limit_get(AAL_RATE_LIMIT_FLOW_3, &rl_msk, &stats);
    te_drp[3] += stats.pircnt;
          //if (te_drp[0] != 0) {
    diag_printf("GE->PON TE queue 3 drop pkt                    = %lld\n", te_drp[3]);

        
    aal_flow_rate_limit_get(AAL_RATE_LIMIT_CPU_DEST, &rl_msk, &stats);
    te_drp[4] += stats.pircnt;
    //if (te_drp[2] != 0) {
    diag_printf("GE+PON->CPU TE drop pkt                        = %lld\n", te_drp[4]);
    //}
#ifdef HAVE_SDL_CMD_NTT
    cnt_hi = aal_get_bm_cnt(BM_RMU_PT0_DROP_CNT1);
    bm_drp[0] += cnt_hi;
    //if (bm_drp[0] != 0) {
    diag_printf("GE rx good packet but BM drop                  = %lld\n", bm_drp[0]);
    //}

    cnt_hi = aal_get_bm_cnt(BM_RMU_PT0_DROP_CNT2);
    fe_drp[0] += cnt_hi;
    //if (fe_drp[0] != 0) {
    diag_printf("GE rx good packet but FE drop                  = %lld\n", fe_drp[0]);
    //}

    cnt_hi = aal_get_bm_cnt(BM_RMU_PT0_DROP_CNT3);
    voq_drp[0] += cnt_hi;
    //if (voq_drp[0] != 0) {
    diag_printf("GE rx good packet but VOQ collision drp cnt    = %lld\n", voq_drp[0]);
    //}

    cnt_hi = aal_get_bm_cnt(BM_RMU_PT2_DROP_CNT1);
    bm_drp[2] += cnt_hi;
    //if (bm_drp[2] != 0) {
    diag_printf("CPU tx good packet but BM drop cnt             = %lld\n", bm_drp[2]);
    //}

    cnt_hi = aal_get_bm_cnt(BM_RMU_PT2_DROP_CNT2);
    fe_drp[2] += cnt_hi;
    //if (fe_drp[2] != 0) {
    diag_printf("CPU tx good packet but FE drop cnt             = %lld\n", fe_drp[2]);
    //}

    cnt_hi = aal_get_bm_cnt(BM_RMU_PT1_DROP_CNT3);
    voq_drp[2] += cnt_hi;
    //if (voq_drp[2] != 0) {
    diag_printf("CPU tx good packet but VOQ  collision drp pkt  = %lld\n", voq_drp[2]);
    //}
    us_store_cnt = aal_get_bm_cnt(BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS0) +
                   aal_get_bm_cnt(BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS1) +
                   aal_get_bm_cnt(BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS2) +
                   aal_get_bm_cnt(BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS3);
    //if (us_store_cnt != 0) {
    diag_printf("GE->PON store in buffer pkt                    = %d\n", us_store_cnt);
    //}
    us_store_oam = aal_get_bm_cnt(BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS4);

    //if (us_store_oam != 0) {
    diag_printf("CPU->PON  store in buffer pkt                  = %d\n", us_store_oam);
    //}
    ge_cpu_pkt = aal_get_bm_cnt(BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS6);

    // if (ge_cpu_pkt != 0) {
    diag_printf("GE->CPU store in buffer pkt                    = %d\n", ge_cpu_pkt);
    //}

#else
    cnt_hi = aal_get_bm_cnt(BM_GE_DROP_PKT_CNTR) + aal_get_bm_cnt(BM_PON_ABR_DROP_PKT_CNTR);
    bm_drp[0] += cnt_hi;
    diag_printf("GE rx good packet but BM drop                  = %lld\n", bm_drp[0]);
#endif

}

void sdl_dump_ds_drp_stats(void)
{
    //onu_aal_ge_mib_t mib_id;
    // cs_uint32  cnt_lo = 0;
    cs_uint32  cnt_hi = 0;
    cs_uint32  cnt_lo = 0;
#ifdef HAVE_SDL_CMD_NTT    
    cs_uint32 ds_store_cnt = 0;
    cs_uint32 ds_store_oam  = 0;
    cs_uint32 pon_cpu_pkt  = 0;
 #endif   
    cs_aal_rate_limit_msk_t rl_msk;
    cs_aal_rate_limit_t stats;

    diag_printf("\n######### downstream blocks drop statistic #########\n\n");

    aal_pon_mib_get(FALSE, MIB_ID_PON_CRCERRCNT, &cnt_hi, &cnt_lo);
    //if (cnt_hi != 0) {
    diag_printf("PON Rx CRC error pkt                           = %d\n", cnt_hi);
    //}
    aal_pon_mib_get(FALSE, MIB_ID_PON_RUNTCNT, &cnt_hi, &cnt_lo);
    //if (cnt_hi != 0) {
    diag_printf("PON Rx under size pkt                          = %d\n", cnt_hi);
    //}
    aal_pon_mib_get(FALSE, MIB_ID_PON_OVERSIZECNT, &cnt_hi, &cnt_lo);
    //if (cnt_hi != 0) {
    diag_printf("PON Rx over size pkt                           = %d\n", cnt_hi);
    //}

    cnt_hi = aal_get_bm_cnt(BM_MPCP_DROP_PKT_CNTR);
    mpcp_drp += cnt_hi;
    diag_printf("PON MPCP drp cnt                               = %lld\n", mpcp_drp);

    cnt_hi = aal_get_bm_cnt(BM_PON_FE_DROP_PKT_CNTR);
    mac_drp[1] += cnt_hi;
    //if (mac_drp[1] != 0) {
    diag_printf("PON->FE drp cnt                                = %lld\n", mac_drp[1]-mpcp_drp);
    //}

    cnt_hi = aal_get_bm_cnt(FE_TOP_MIR_2_DROP_CNT);
    mac_drp[2] += cnt_hi;
    //if (mac_drp[2] != 0) {
    diag_printf("MA->FE drp cnt                                 = %lld\n", mac_drp[2]);
    //}
    rl_msk.u = 0;
    rl_msk.s.pircnt = 1;

    aal_flow_rate_limit_get(AAL_RATE_LIMIT_PON_SP, &rl_msk, &stats);

    te_drp[1] += stats.pircnt;
    //if (te_drp[1] != 0) {
    diag_printf("PON->GE TE drop pkt                            = %lld\n", te_drp[1]);
    //}

    te_drp[5] += stats.pircnt;
    //if (te_drp[2] != 0) {
    diag_printf("GE+PON->CPU TE drp pkt                         = %lld\n", te_drp[5]);
    //}
#ifdef HAVE_SDL_CMD_NTT
    cnt_hi = aal_get_bm_cnt(BM_RMU_PT1_DROP_CNT1);
    bm_drp[1] += cnt_hi;
    //if (bm_drp[1] != 0) {
    diag_printf("PON rx good packet but BM drop  pkt            = %lld\n", bm_drp[1]);
    //}

    cnt_hi = aal_get_bm_cnt(BM_RMU_PT1_DROP_CNT2);
    fe_drp[1] += cnt_hi;
    //if (fe_drp[1] != 0) {
    diag_printf("PON rx good packet but FE drop  pkt            = %lld\n", fe_drp[1]);
    //}

    cnt_hi = aal_get_bm_cnt(BM_RMU_PT1_DROP_CNT3);
    voq_drp[1] += cnt_hi;
    //if (voq_drp[1] != 0) {
    diag_printf("PON rx good packet but VOQ  collision drp pkt  = %lld\n", voq_drp[1]);
    //}

    cnt_hi = aal_get_bm_cnt(BM_RMU_PT2_DROP_CNT1);
    bm_drp[2] += cnt_hi;
    //if (bm_drp[2] != 0) {
    diag_printf("CPU tx good packet but BM drop cnt             = %lld\n", bm_drp[2]);
    //}

    cnt_hi = aal_get_bm_cnt(BM_RMU_PT2_DROP_CNT2);
    fe_drp[2] += cnt_hi;
    //if (fe_drp[2] != 0) {
    diag_printf("CPU tx good packet but FE drop cnt             = %lld\n", fe_drp[2]);
    //}

    cnt_hi = aal_get_bm_cnt(BM_RMU_PT1_DROP_CNT3);
    voq_drp[2] += cnt_hi;
    //if (voq_drp[2] != 0) {
    diag_printf("CPU tx good packet but VOQ  collision drp pkt  = %lld\n", voq_drp[2]);
    //}
    ds_store_cnt = aal_get_bm_cnt(BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS0) +
                   aal_get_bm_cnt(BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS1) +
                   aal_get_bm_cnt(BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS2) +
                   aal_get_bm_cnt(BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS3);
    //if (ds_store_cnt != 0) {
    diag_printf("PON->GE store in buffer pkt                    = %d\n", ds_store_cnt);
    //}
    ds_store_oam = aal_get_bm_cnt(BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS4);

    //if (ds_store_oam != 0) {
    diag_printf("CPU->GE store in buffer pkt                    = %d\n", ds_store_oam);
    //}
    pon_cpu_pkt = aal_get_bm_cnt(BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS7);

    //if (pon_cpu_pkt != 0) {
    diag_printf("PON->CPU  store in buffer pkt                  = %d\n", pon_cpu_pkt);
    //}
#else
    cnt_hi = aal_get_bm_cnt(BM_PON_DROP_PKT_CNTR) + aal_get_bm_cnt(BM_GE_ABR_DROP_PKT_CNTR);
    bm_drp[1] += cnt_hi;
    diag_printf("GE rx good packet but BM drop                  = %lld\n", bm_drp[1]);
#endif
}

void sdl_dump_all_interrupt(void)
{

    __sdl_counter_entry_t int_sta[] = {
        {GLOBAL_PTP_INTERRUPT, "GLOBAL_PTP_INTERRUPT"},
        {FE_TOP_STS_INTERRUPT, "FE_TOP_STS_INTERRUPT"},
        {FE_TOP_STS_INTERRUPT_1, "FE_TOP_STS_INTERRUPT_1"},
        {GE_INTERRUPT, "GE_INTERRUPT"},
        {ONU_MAC_INTERRUPT_SOURCE, "ONU_MAC_INTERRUPT_SOURCE"},
        {ONU_MAC_INTERRUPT_ISAS, "ONU_MAC_INTERRUPT_ISAS"},
        {ONU_MAC_INTERRUPT_SOURCE2, "ONU_MAC_INTERRUPT_SOURCE2"},
        {ONU_MAC_INTERRUPT_ISAS2, "ONU_MAC_INTERRUPT_ISAS2"},
        {BM_INTERRUPT, "BM_INTERRUPT"}
    };
    static cs_uint32 size = sizeof(int_sta) / sizeof(__sdl_counter_entry_t);
    cs_uint8 i;
    cs_uint32 value;

    diag_printf("\n######### dump interrupt #########\n\n");

    for (i = 0; i < size; i++) {
        value = aal_get_bm_cnt(int_sta[i].offset);
        diag_printf("%32s:  =  0x%08x\n", int_sta[i].name, value);
    }
}

void sdl_clear_all_counter(void)
{
    cs_uint8 index;
    cs_aal_rate_limit_msk_t rl_msk;
    cs_aal_rate_limit_t stats;
    cs_aal_rate_limit_type_t rl_type;
    
    cs_aal_uni_cfg_t uni_cfg;
    cs_aal_uni_cfg_mask_t uni_msk;
    
    uni_msk.u32 = 0;
    uni_msk.s.mib_clr_all= 1;
    uni_cfg.mib_clr_all = 1;
    

   // diag_printf("######### clear all statistic #########\n");
    if (aal_uni_mac_cfg_set(uni_msk, uni_cfg) == CS_E_OK) {
        ;//diag_printf("UNI MIB reset done.\n");
    }
    
    if (!aal_pon_mib_reset()) {
        aal_get_bm_cnt(FE_LE_ENGINE_INGRESS_PKT_CNT + FE_LE_STRIDE);
        //diag_printf("PMAC MIB reset done.\n");
    }

    rl_msk.u = 0;
    rl_msk.s.fwdcnt = 1;
    rl_msk.s.pircnt = 1;
    for (index = 0; index < 3; index++) {
        aal_get_bm_cnt(FE_LE_ENGINE_INGRESS_PKT_CNT + index*FE_LE_STRIDE);
        aal_get_bm_cnt(BM_GE_TX_PKT_CNTR + index);
        aal_get_bm_cnt(BM_GE_RX_PKT_CNTR + index);
        aal_get_bm_cnt(BM_GE_FE_DROP_PKT_CNTR + index);
        aal_get_bm_cnt(BM_GE_DROP_PKT_CNTR + index);
        aal_get_bm_cnt(BM_GE_ABR_DROP_PKT_CNTR + index);        
        aal_get_bm_cnt(BM_RMU_PT0_DROP_CNT1 + index);
        aal_get_bm_cnt(BM_RMU_PT1_DROP_CNT1 + index);
        aal_get_bm_cnt(BM_RMU_PT2_DROP_CNT1 + index);
        aal_get_bm_cnt(FE_TOP_MIR_0_DROP_CNT + index);
        fe_drp[index] = 0;
        bm_drp[index] = 0;
        voq_drp[index] = 0;
        bm_rx[index] = 0;
        bm_tx[index] = 0;
        mac_drp[index] = 0;
        mpcp_drp = 0;
    }
    
    aal_get_bm_cnt(BM_MPCP_DROP_PKT_CNTR);
    
    for (index = 0; index < 7; index++) {
        te_fwd[index] = 0;
        te_drp[index] = 0;
    }    
    for(rl_type = AAL_RATE_LIMIT_GE_SP;rl_type<= AAL_RATE_LIMIT_CPU_DEST; rl_type++){ 
        aal_flow_rate_limit_get(rl_type,&rl_msk, &stats);
    }
    index = 1;
#ifdef HAVE_RTK_CMD
    for (; index < 5; index++)
#endif
    {
        epon_request_onu_port_stats_clr(qos_context, QOS_DEVICE_ID, LLID_PORTID,index);
    }
#ifdef HAVE_RTK_CMD
    epon_request_onu_port_stats_clr(qos_context, QOS_DEVICE_ID, LLID_PORTID,CS_UPLINK_PORT);
#endif
}

#if !(HAVE_SDL_CMD_KT)  
sal_cmd_result_t qos_cmd_dot1p_que_map_set(int argc, char **argv);
sal_cmd_result_t qos_cmd_dot1p_que_map_get(void);
sal_cmd_result_t qos_cmd_tos_pri_map_set(int argc, char **argv);
sal_cmd_result_t qos_cmd_tos_pri_map_get(void);

#ifdef HAVE_SDL_CMD_HELP
static const char *qos_cmd_dot1p_map_que[] = {
    "priority 0 mapped queue <0~7>",
    "priority 1 mapped queue <0~7>",
    "priority 2 mapped queue <0~7>",
    "priority 3 mapped queue <0~7>",
    "priority 4 mapped queue <0~7>",
    "priority 5 mapped queue <0~7>",
    "priority 6 mapped queue <0~7>",
    "priority 7 mapped queue <0~7>"
};


static const char *qos_cmd_tos_map_dot1p[] = {
    "tos 0 mapped priority <0~7>",
    "tos 1 mapped priority <0~7>",
    "tos 2 mapped priority <0~7>",
    "tos 3 mapped priority <0~7>",
    "tos 4 mapped priority <0~7>",
    "tos 5 mapped priority <0~7>",
    "tos 6 mapped priority <0~7>",
    "tos 7 mapped priority <0~7>"
};
#endif

sal_cmd_result_t qos_cmd_dot1p_que_map_set(int argc, char **argv)
{
    cs_status  sdl_rt;
    cs_pri_cos_map_t cos;
    cs_uint8 index;
   
    if(argc != 10){
        return SAL_CMD_INVALID_PARAM;
    }
    
    for(index=0; index<8; index++){
        cos.cos[index] = iros_strtol(argv[2+index]);
    }
    
    sdl_rt = epon_request_onu_dot1p_map_set(qos_context, QOS_DEVICE_ID, LLID_PORTID, &cos);

    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_dot1p_que_map_get(void)
{
    cs_status  sdl_rt;
    cs_pri_cos_map_t cos;
    cs_uint8 index;
      
    sdl_rt = epon_request_onu_dot1p_map_get(qos_context, QOS_DEVICE_ID, LLID_PORTID, &cos);
    for(index = 0; index<8; index++){
        sal_cmd_show("\n priority %d  queue = %d\n", index, cos.cos[index]);
    }   
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_tos_pri_map_set(int argc, char **argv)
{
    cs_status  sdl_rt;
    cs_tos_pri_map_t pri;
    cs_uint8 index;
   
    if(argc != 10){
        return SAL_CMD_INVALID_PARAM;
    }
    
    for(index=0; index<8; index++){
        pri.pri[index] = iros_strtol(argv[2+index]);
    }
    
    sdl_rt = epon_request_onu_tos_map_set(qos_context, QOS_DEVICE_ID, LLID_PORTID, &pri);

    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_tos_pri_map_get(void)
{
    cs_status  sdl_rt;
    cs_tos_pri_map_t pri;
    cs_uint8 index;
      
    sdl_rt = epon_request_onu_tos_map_get(qos_context, QOS_DEVICE_ID, LLID_PORTID, &pri);
    for(index = 0; index<8; index++){
        sal_cmd_show("\n priority %d  queue = %d\n", index, pri.pri[index]);
    }   
    return cs_status_2_cmd_rc_map(sdl_rt);       
}
#endif

#ifdef HAVE_SDL_CMD_NTT

sal_cmd_result_t qos_cmd_que_peak_rate_set(int argc, char **argv);
sal_cmd_result_t qos_cmd_que_peak_rate_get(void);
sal_cmd_result_t qos_cmd_que_burst_size_set(int argc, char **argv);
sal_cmd_result_t qos_cmd_que_burst_size_get(void);
sal_cmd_result_t qos_cmd_que_flow_ctrl_set(int argc, char **argv);
sal_cmd_result_t qos_cmd_que_flow_ctrl_get(void);
sal_cmd_result_t qos_cmd_que_buffer_size_set(int argc, char **argv);
sal_cmd_result_t qos_cmd_que_buffer_size_get(void);
sal_cmd_result_t qos_cmd_sdl_setting_dump(int argc, char **argv);
sal_cmd_result_t qos_cmd_dest_cpu_rate_set(int argc, char **argv);


#ifdef HAVE_SDL_CMD_HELP
static const char *qos_cmd_que_peak_rate[] = {
    "queue 0 peak rate <0~10000>",
    "queue 1 peak rate <0~10000>",
    "queue 2 peak rate <0~10000>",
    "queue 3 peak rate <0~10000>"
};

static const char *qos_cmd_que_burst_size[] = {
    "queue 0 burst size <0~255>",
    "queue 1 burst size <0~255>",
    "queue 2 burst size <0~255>",
    "queue 3 burst size <0~255>"
};

static const char *qos_cmd_que_buffer_size[] = {
    "upstream queue 0 buffer size <0~512>",
    "upstream queue 1 buffer size <0~512>",
    "upstream queue 2 buffer size <0~512>",
    "upstream queue 3 buffer size <0~512>",
    "downstream queue 0 buffer size <0~512>",
    "downstream queue 1 buffer size <0~512>",
    "downstream queue 2 buffer size <0~512>",
    "downstream queue 3 buffer size <0~512>"
};

static const char *qos_cmd_flow_ctrl_thre[] = {
    "queue 0 threshold  <0~512>",
    "queue 1 threshold <0~512>",
    "queue 2 threshold <0~512>",
    "queue 3 threshold <0~512>"
};

static const char *qos_cmd_show_sdl_setting[] = {
    "rt, show rate limiter setting",
    "burst, show burst size setting",
    "fc, show flow control setting",
    "qsize, show buffer size setting",
    "1pcos, show 8021p to que map",
    "tos1p, show tos to 8021p map",
    "all, show all 8021p setting",
};

static const char *qos_cmd_dst_cpu_rate[] = {
    "rate limit mode, 1-pps, 0-bps",
    "to cpu rate <0~1000000>",
};
#endif

static sal_cmd_reg_struct_t qos_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    {qos_cmd_que_peak_rate_set, "rt", "set upstream queue based rate limiter", qos_cmd_que_peak_rate, sizeof(qos_cmd_que_peak_rate)/4},
    {qos_cmd_que_burst_size_set, "burst", "set burst size of rate limiter", qos_cmd_que_burst_size, sizeof(qos_cmd_que_burst_size)/4},
    {qos_cmd_que_flow_ctrl_set, "fc", "set flow control threshold", qos_cmd_flow_ctrl_thre, sizeof(qos_cmd_flow_ctrl_thre)/4},
    {qos_cmd_que_buffer_size_set, "qsize", "set us/ds queue buffer size", qos_cmd_que_buffer_size, sizeof(qos_cmd_que_buffer_size)/4},
    {qos_cmd_dot1p_que_map_set, "1pcos", "set 8021p to queue mapping", qos_cmd_dot1p_map_que, sizeof(qos_cmd_dot1p_map_que)/4},
    {qos_cmd_tos_pri_map_set, "tos1p", "set tos to 8021p mapping", qos_cmd_tos_map_dot1p, sizeof(qos_cmd_tos_map_dot1p)/4},
    {qos_cmd_dest_cpu_rate_set, "cpurt", "set to cpu packet rate limit", qos_cmd_dst_cpu_rate, sizeof(qos_cmd_dst_cpu_rate)/4},
    {qos_cmd_sdl_setting_dump, "show", "get qos sdl setting ", qos_cmd_show_sdl_setting, sizeof(qos_cmd_show_sdl_setting)/4},
#else
    {qos_cmd_que_peak_rate_set, "rt"},// "set upstream queue based rate limiter"},// qos_cmd_que_peak_rate, sizeof(qos_cmd_que_peak_rate)/4},
    {qos_cmd_que_burst_size_set, "burst"},// "set burst size of rate limiter"},// qos_cmd_que_burst_size, sizeof(qos_cmd_que_burst_size)/4},
    {qos_cmd_que_flow_ctrl_set, "fc"},// "set flow control threshold"},// qos_cmd_flow_ctrl_thre, sizeof(qos_cmd_flow_ctrl_thre)/4},
    {qos_cmd_que_buffer_size_set, "qsize"},// "set us/ds queue buffer size"},// qos_cmd_que_buffer_size, sizeof(qos_cmd_que_buffer_size)/4},
    {qos_cmd_dot1p_que_map_set, "1pcos"},// "set 8021p to queue mapping"},// qos_cmd_dot1p_map_que, sizeof(qos_cmd_dot1p_map_que)/4},
    {qos_cmd_tos_pri_map_set, "tos1p"},// "set tos to 8021p mapping"},// qos_cmd_tos_map_dot1p, sizeof(qos_cmd_tos_map_dot1p)/4},
    {qos_cmd_dest_cpu_rate_set, "cpurt"},// "set to cpu packet rate limit"},// qos_cmd_dst_cpu_rate, sizeof(qos_cmd_dst_cpu_rate)/4},
    {qos_cmd_sdl_setting_dump, "show"},// "get qos sdl setting "},// qos_cmd_show_sdl_setting, sizeof(qos_cmd_show_sdl_setting)/4},
#endif
};

sal_cmd_cfg_t qos_cmd_cfg = {"qos", "qos configuration", qos_cmd_list, sizeof(qos_cmd_list)/sizeof(sal_cmd_reg_struct_t)};
sal_cmd_result_t qos_cmd_dest_cpu_rate_set(int argc, char **argv)
{
    cs_status  sdl_rt;
    cs_aal_rate_limit_t  cpu_rate;
    cs_aal_rate_limit_msk_t msk;

    
    if(argc != 4){
        return SAL_CMD_INVALID_PARAM;
    }

    memset(&cpu_rate, 0, sizeof(cpu_rate));
    msk.u = 0;
    msk.s.rlmode = 1;
    msk.s.rate = 1;
    
    cpu_rate.ctrl.s.rlmode = iros_strtol(argv[2]);
    cpu_rate.rate = iros_strtol(argv[3]);
    
    sdl_rt = aal_flow_rate_limit_set(AAL_RATE_LIMIT_CPU_DEST, &msk, &cpu_rate);

    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_dest_cpu_rate_get(void)
{
    cs_status  sdl_rt;
    cs_aal_rate_limit_t  cpu_rate;
    cs_aal_rate_limit_msk_t msk;

    memset(&cpu_rate, 0, sizeof(cpu_rate));
    msk.u = 0;
    msk.s.rlmode = 1;
    msk.s.rate = 1;
        
    sdl_rt = aal_flow_rate_limit_get(AAL_RATE_LIMIT_CPU_DEST, &msk, &cpu_rate);
    sal_cmd_show("\n rl mode is %s.\n", cpu_rate.ctrl.s.rlmode? "PPS":"BPS");
    sal_cmd_show("\n to CPU port rate %d.\n", cpu_rate.rate);

    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_que_peak_rate_set(int argc, char **argv)
{
    cs_status  sdl_rt;
    cs_uint8 index;
    cs_peak_rate_t  peak_rate;
    
    if(argc != 6){
        return SAL_CMD_INVALID_PARAM;
    }
    
    for(index=0; index<4; index++){
        peak_rate.queue_rate[index] = iros_strtol(argv[2+index]);
    }
    
    sdl_rt = epon_request_onu_peak_rate_set(qos_context, QOS_DEVICE_ID, LLID_PORTID, peak_rate);

    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_que_peak_rate_get(void)
{
    cs_status  sdl_rt;
    cs_peak_rate_t  peak_rate;
    cs_uint8 index;
         
    sdl_rt = epon_request_onu_peak_rate_get(qos_context, QOS_DEVICE_ID, LLID_PORTID, &peak_rate);
    for(index = 0; index<4; index++){
        sal_cmd_show("\n queue %d peak rate = %d\n", index, peak_rate.queue_rate[index]);
    }
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_que_burst_size_set(int argc, char **argv)
{
    cs_status      sdl_rt;
    cs_uint8            index;
    cs_bust_size_t          burst_size;
    
    if(argc != 6){
        return SAL_CMD_INVALID_PARAM;
    }
    
    for(index=0; index<4; index++){
        burst_size.queue_burst_size[index] = iros_strtol(argv[2+index]);
    }
    
    sdl_rt = epon_request_onu_burst_size_set(qos_context, QOS_DEVICE_ID, LLID_PORTID, burst_size);

    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_que_burst_size_get(void)
{
    cs_status      sdl_rt;
    cs_uint8            index;
    cs_bust_size_t          burst_size;
      
    sdl_rt = epon_request_onu_burst_size_get(qos_context, QOS_DEVICE_ID, LLID_PORTID, &burst_size);
    for(index = 0; index<4; index++){
        sal_cmd_show("\n queue %d burst size = %d\n", index, burst_size.queue_burst_size[index]);
    }
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_que_flow_ctrl_set(int argc, char **argv)
{
    cs_status          sdl_rt;
    cs_flowctrl_threshold_t     threshold;
    cs_uint8                index;
   
    if(argc != 6){
        return SAL_CMD_INVALID_PARAM;
    }
    
    for(index=0; index<4; index++){
        threshold.queue_threshold[index] = iros_strtol(argv[2+index]);
    }
    
    sdl_rt = epon_request_onu_flowctrl_threshold_set(qos_context, QOS_DEVICE_ID, LLID_PORTID, threshold);

    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_que_flow_ctrl_get(void)
{
    cs_status          sdl_rt;
    cs_flowctrl_threshold_t     threshold;
    cs_uint8                index;
   
    sdl_rt = epon_request_onu_flowctrl_threshold_get(qos_context, QOS_DEVICE_ID, LLID_PORTID, &threshold);
    for(index = 0; index<4; index++){
        sal_cmd_show("\n queue %d flow control thre = %d\n", index, threshold.queue_threshold[index]);
    }
    return cs_status_2_cmd_rc_map(sdl_rt);       
}


sal_cmd_result_t qos_cmd_que_buffer_size_set(int argc, char **argv)
{
    cs_status  sdl_rt;
    cs_buf_size_t buf_size;
    cs_uint8 index;
   
    if(argc != 10){
        return SAL_CMD_INVALID_PARAM;
    }
    
    for(index=0; index<4; index++){
        buf_size.up[index] = iros_strtol(argv[2+index]);
    }
    
    for(index=0; index<4; index++){
        buf_size.down[index] = iros_strtol(argv[6+index]);
    }
    
    sdl_rt = epon_request_onu_queue_size_set(qos_context, QOS_DEVICE_ID, LLID_PORTID, &buf_size);

    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_que_buffer_size_get(void)
{
    cs_status  sdl_rt;
    cs_buf_size_t buf_size;
    cs_uint8 index;
      
    sdl_rt = epon_request_onu_queue_size_get(qos_context, QOS_DEVICE_ID, LLID_PORTID, &buf_size);
    for(index = 0; index<4; index++){
        sal_cmd_show("\n upstream queue %d buffer size = %d\n", index, buf_size.up[index]);
    }
    
    for(index = 0; index<4; index++){
        sal_cmd_show("\n downstream queue %d buffer size = %d\n", index, buf_size.down[index]);
    }
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_que_num_get(void)
{
    cs_status  sdl_rt;
    cs_uint8 que_num;
      
    sdl_rt = epon_request_onu_queue_num_get(qos_context, QOS_DEVICE_ID, LLID_PORTID, &que_num);
    sal_cmd_show("queue number is %d", que_num);

    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_bm_stuck_workaround_get(void)
{
    cs_boolean              work_around_en = 0;
    CS_OUT cs_uint32        detected_counter = 0;
    cs_status sdl_rt;
    
    
   sdl_rt =  epon_request_stuck_pkt_detected_times_get(
        qos_context,
        QOS_DEVICE_ID,
        &work_around_en,
        &detected_counter
        );
    sal_cmd_show("\n BM Stuck workaround enable = %d \n", work_around_en);
    sal_cmd_show("\n BM Stuck detected count    = %d \n", detected_counter);

    return cs_status_2_cmd_rc_map(sdl_rt);       

}
sal_cmd_result_t qos_cmd_sdl_setting_dump(int argc, char **argv)
{
    sal_cmd_result_t  sdl_rt = SAL_CMD_OK;
    if(argc != 3){
        return SAL_CMD_INVALID_PARAM;
    }
    if(cmd_cmp(argv[2], "rt")) {
       return qos_cmd_que_peak_rate_get();
    }
    if(cmd_cmp(argv[2], "burst")) {
       return qos_cmd_que_burst_size_get();
    }
    if(cmd_cmp(argv[2], "fc")) {
       return qos_cmd_que_flow_ctrl_get();
    }
    if(cmd_cmp(argv[2], "qsize")) {
       return qos_cmd_que_buffer_size_get();
    }
    if(cmd_cmp(argv[2], "1pcos")) {
       return qos_cmd_dot1p_que_map_get();
    }
    if(cmd_cmp(argv[2], "tos1p")) {
       return qos_cmd_tos_pri_map_get();
    }
    
    if(cmd_cmp(argv[2], "cpurt")) {
       return qos_cmd_dest_cpu_rate_get();
    }

    
    if(cmd_cmp(argv[2], "qmod")) {
       return qos_cmd_que_num_get();
    }
    
    if(cmd_cmp(argv[2], "all")) {
        sdl_rt |= qos_cmd_que_peak_rate_get();
        sdl_rt |= qos_cmd_que_burst_size_get();
        sdl_rt |= qos_cmd_que_flow_ctrl_get();
        sdl_rt |= qos_cmd_que_buffer_size_get();        
        sdl_rt |= qos_cmd_dot1p_que_map_get();
        sdl_rt |= qos_cmd_tos_pri_map_get();
        sdl_rt |= qos_cmd_bm_stuck_workaround_get();
    }
    return sdl_rt;
}

#endif
#ifdef HAVE_SDL_CMD_CTC



sal_cmd_result_t qos_cmd_legacy_policy_set(int argc, char **argv);
sal_cmd_result_t qos_cmd_legacy_policy_get(int argc, char **argv);
sal_cmd_result_t qos_cmd_sdl_setting_dump(int argc, char **argv);


 

#ifdef HAVE_SDL_CMD_HELP



static const char *qos_get_cmd_list[] = {
    "1pcos, show 8021p to que map",
    "tos1p, show tos to 8021p map",
};
#endif

static sal_cmd_reg_struct_t qos_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    {qos_cmd_dot1p_que_map_set, "1pcos", "set 8021p to queue mapping", qos_cmd_dot1p_map_que, sizeof(qos_cmd_dot1p_map_que)/4},
    {qos_cmd_tos_pri_map_set, "tos1p", "set tos to 8021p mapping", qos_cmd_tos_map_dot1p, sizeof(qos_cmd_tos_map_dot1p)/4},
    {qos_cmd_sdl_setting_dump, "show", "show qos setting", qos_get_cmd_list, sizeof(qos_get_cmd_list)/4},
#else
    {qos_cmd_legacy_policy_set, "lgcy-pl"},// "set upstream port based rate limiter"},// qos_cmd_uni_policy, sizeof(qos_cmd_uni_policy)/4},
    {qos_cmd_dot1p_que_map_set, "1pcos"},// "set 8021p to queue mapping"},// qos_cmd_dot1p_map_que, sizeof(qos_cmd_dot1p_map_que)/4},
    {qos_cmd_tos_pri_map_set, "tos1p"},// "set tos to 8021p mapping"},// qos_cmd_tos_map_dot1p, sizeof(qos_cmd_tos_map_dot1p)/4},
    {qos_cmd_sdl_setting_dump, "show"},// "show qos setting"},// qos_get_cmd_list, sizeof(qos_get_cmd_list)/4},
#endif
};


sal_cmd_result_t qos_cmd_legacy_policy_set(int argc, char **argv)
{
    cs_status  sdl_rt = CS_E_OK;
    cs_boolean enable;
    
    if(argc != 3){
        return SAL_CMD_INVALID_PARAM;
    }
    enable = iros_strtol(argv[2]);
    if(enable){        
        sdl_rt = (cs_status)cs_plat_ssp_switch_write(qos_context,0,0,0x1203, 0xed); 
    }else{
       sdl_rt = (cs_status)cs_plat_ssp_switch_write(qos_context,0,0,0x1203, 0x12); 
    }
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t qos_cmd_legacy_policy_get(int argc, char **argv)
{
    cs_status  sdl_rt = CS_E_OK;
    cs_uint32  value;
    if(argc != 3){
        return SAL_CMD_INVALID_PARAM;
    }
    sdl_rt = (cs_status)cs_plat_ssp_switch_read(qos_context,0,0,0x1203,&value);
    sal_cmd_show("\n legacy policy: %s", value == 0xed ? "enable":"disable");
  
    return cs_status_2_cmd_rc_map(sdl_rt);       
}



sal_cmd_result_t qos_cmd_sdl_setting_dump(int argc, char **argv)
{    
    if(argc != 3){
        return SAL_CMD_INVALID_PARAM;
    }
    
    if(cmd_cmp(argv[2], "lgcy-pl")) {
       return qos_cmd_legacy_policy_get(argc, argv);
    }
    
    if(cmd_cmp(argv[2], "1pcos")) {
       return qos_cmd_dot1p_que_map_get();
    }
    
    if(cmd_cmp(argv[2], "tos1p")) {
       return qos_cmd_tos_pri_map_get();
    }

    return SAL_CMD_INVALID_PARAM;
    
}

sal_cmd_cfg_t qos_cmd_cfg = {"qos", "qos configuration", qos_cmd_list, sizeof(qos_cmd_list)/sizeof(sal_cmd_reg_struct_t)};

#endif //#ifdef HAVE_SDL_CMD
#endif

cs_status qos_cmd_self_reg(void)
{
    cs_status ret = CS_E_OK;
#if !(HAVE_SDL_CMD_KT)  
    ret = sal_cmd_reg(&qos_cmd_cfg);
#endif
    return ret;
}

