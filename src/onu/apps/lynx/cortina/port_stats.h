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

Copyright (c) 2011 by Cortina Systems Incorporated
****************************************************************************/
#ifndef __PORT_STATS_H__
#define __PORT_STATS_H__
#include "oam_std_pdu.h"

#define PORT_STATS_LOG(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_MAJ,IROS_MID_APP,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define PORT_STATS_LOG_INFO(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_INF,IROS_MID_APP,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define PORT_STATS_LOG_MIN(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_MIN,IROS_MID_APP,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define PORT_STATS_LOG_MAJ(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_MAJ,IROS_MID_APP,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define PORT_STATS_CRI(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_CRI,IROS_MID_APP,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)


#define PORT_STATS_DEBUG(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_DBG0,IROS_MID_APP,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define  PORT_STATS_ASSERT_RET(x,ret)    \
    do{ \
        if (!(x))  { \
            cs_printf("%s() _%d_: assert fail\n", __FUNCTION__, __LINE__); \
            return (ret); \
        } \
    }while(0)

#define  PORT_STATS_ASSERT(x)    \
    do{ \
        if (!(x))  { \
            cs_printf("%s() _%d_: assert fail\n", __FUNCTION__, __LINE__); \
            return ; \
        } \
    }while(0)

#define PORT_STATS_MALLOC(size)  iros_malloc(IROS_MID_APP,size)

typedef struct {
    cs_uint64 pon_byte_cnt           ;
    cs_uint64 pon_tx_byte_cnt        ;
    cs_uint64 pon_frame_cnt          ;
    cs_uint64 pon_txframe_cnt        ;
    cs_uint64 pon_crcerr_cnt         ;
    cs_uint64 pon_mcframe_cnt        ;
    cs_uint64 pon_bcframe_cnt        ;
    cs_uint64 pon_txmcframe_cnt      ;
    cs_uint64 pon_txbcframe_cnt      ;
    cs_uint64 pon_ctrlframe_cnt      ;
    cs_uint64 pon_txctrlframe_cnt    ;
    cs_uint64 pon_pauseframe_cnt     ;
    cs_uint64 pon_unknownop_cnt      ;
    cs_uint64 pon_runt_cnt           ;
    cs_uint64 pon_oversize_cnt       ;
    cs_uint64 pon_rmon64_cnt         ;
    cs_uint64 pon_rmon65_127_cnt     ;
    cs_uint64 pon_rmon128_255_cnt    ;
    cs_uint64 pon_rmon256_511_cnt    ;
    cs_uint64 pon_rmon512_1023_cnt   ;
    cs_uint64 pon_rmon1024_1518_cnt  ;
    cs_uint64 pon_txrmon64_cnt       ;
    cs_uint64 pon_txrmon65_127_cnt   ;
    cs_uint64 pon_txrmon128_255_cnt  ;
    cs_uint64 pon_txrmon256_511_cnt  ;
    cs_uint64 pon_txrmon512_1023_cnt ;
    cs_uint64 pon_txrmon1024_1518_cnt;
    cs_uint64 pon_mpcp_uc_rev_cnt     ;
    cs_uint64 pon_mpcp_uc_reg_ack     ;
    cs_uint64 pon_mpcp_uc_reg_nack    ;
    cs_uint64 pon_mpcp_uc_reg_dereg   ;
    cs_uint64 pon_mpcp_uc_reg_rereg   ;
    cs_uint64 pon_mpcp_uc_reg_reg_rev ;
    cs_uint64 pon_mpcp_uc_gat_rev     ;
    cs_uint64 pon_mpcp_uc_gat_norm    ;
    cs_uint64 pon_mpcp_uc_gat_frpt    ;
    cs_uint64 pon_mpcp_uc_gat_udis    ;
    cs_uint64 pon_mpcp_uc_gat_bdis    ;
    cs_uint64 pon_mpcp_mc_rev_cnt     ;
    cs_uint64 pon_mpcp_mc_reg_ack     ;
    cs_uint64 pon_mpcp_mc_reg_nack    ;
    cs_uint64 pon_mpcp_mc_reg_dereg   ;
    cs_uint64 pon_mpcp_mc_reg_rereg   ;
    cs_uint64 pon_mpcp_mc_reg_reg_rev ;
    cs_uint64 pon_mpcp_mc_gat_rev     ;
    cs_uint64 pon_mpcp_mc_gat_norm    ;
    cs_uint64 pon_mpcp_mc_gat_frpt    ;
    cs_uint64 pon_mpcp_mc_gat_udis    ;
    cs_uint64 pon_mpcp_mc_gat_bdis    ;
    cs_uint64 pon_mpcp_bc_rev_cnt     ;
    cs_uint64 pon_mpcp_bc_reg_ack     ;
    cs_uint64 pon_mpcp_bc_reg_nack    ;
    cs_uint64 pon_mpcp_bc_reg_dereg   ;
    cs_uint64 pon_mpcp_bc_reg_rereg   ;
    cs_uint64 pon_mpcp_bc_reg_reg_rev ;
    cs_uint64 pon_mpcp_bc_gat_rev     ;
    cs_uint64 pon_mpcp_bc_gat_norm    ;
    cs_uint64 pon_mpcp_bc_gat_frpt    ;
    cs_uint64 pon_mpcp_bc_gat_udis    ;
    cs_uint64 pon_mpcp_bc_gat_bdis    ;
    cs_uint64 pon_mpcp_tx_req_dereg   ;
    cs_uint64 pon_mpcp_tx_req_reg     ;
    cs_uint64 pon_mpcp_tx_ack_ack     ;
    cs_uint64 pon_mpcp_tx_ack_nack    ;
    cs_uint64 pon_mpcp_tx_rpt    ;
    cs_uint64 pon_mpcp_tx_oam    ;
    cs_uint64 pon_mpcp_da_mc_nslf;
    cs_uint64 pon_mpcp_da_mc_slf ;
    cs_uint64 pon_mpcp_da_uc     ;
    cs_uint64 pon_framecnt       ;
    cs_uint64 pon_txoversizecnt  ;
    cs_uint64 pon_rx1519_maxcnt  ;
    cs_uint64 pon_tx1519_maxcnt  ;
    cs_uint64 pon_txframecnt     ;
    cs_uint64 pon_txpausecnt     ;
    cs_uint64 pon_undersizecnt   ;
    cs_uint64 pon_jabbercnt      ;
    cs_uint64 pon_extensioncnt   ;
    cs_uint64 pon_txextensioncnt ;
    cs_uint64 pon_err_symbol     ;
    cs_uint64 pon_outofsync      ;
    cs_uint64 pon_sld_err        ;
    cs_uint64 pon_crc8_err       ;
    cs_uint64 pon_mac_drop       ;
    cs_uint64 pon_ipg_drop       ;
    cs_uint64 pon_drain          ;
    cs_uint64 pon_tx_crc_err     ;
}oam_pon_stats_t;



typedef struct {
    cs_uint64 rxucfrm_cnt                 ;
    cs_uint64 rxmcfrm_cnt                 ;
    cs_uint64 rxbcfrm_cnt                 ;
    cs_uint64 rxoamfrm_cnt                ;
    cs_uint64 rxpausefrm_cnt              ;
    cs_uint64 rxunknownopfrm_cnt          ;
    cs_uint64 rxcrcerrfrm_cnt             ;
    cs_uint64 rxundersizefrm_cnt          ;
    cs_uint64 rxruntfrm_cnt               ;
    cs_uint64 rxoversizefrm_cnt           ;
    cs_uint64 rxjabberfrm_cnt             ;
    cs_uint64 rxstatsfrm64_cnt            ;
    cs_uint64 rxstatsfrm65_127_cnt        ;
    cs_uint64 rxstatsfrm128_255_cnt       ;
    cs_uint64 rxstatsfrm256_511_cnt       ;
    cs_uint64 rxstatsfrm512_1023_cnt      ;
    cs_uint64 rxstatsfrm1024_1518_cnt     ;
    cs_uint64 rxstatsfrm1519_Max_cnt      ;
    cs_uint64 rxbyte_cnt                  ;
    cs_uint64 rxfrm_cnt                   ;
    cs_uint64 txucfrm_cnt                 ;
    cs_uint64 txmcfrm_cnt                 ;
    cs_uint64 txbcfrm_cnt                 ;
    cs_uint64 txoamfrm_cnt                ;
    cs_uint64 txpausefrm_cnt              ;
    cs_uint64 txcrcerrfrm_cnt             ;
    cs_uint64 txoversizefrm_cnt           ;
    cs_uint64 txsinglecolfrm_cnt          ;
    cs_uint64 txmulticolfrm_cnt           ;
    cs_uint64 txlatecolfrm_cnt            ;
    cs_uint64 txexesscolfrm_cnt           ;
    cs_uint64 txstatsfrm64_cnt            ;
    cs_uint64 txstatsfrm65_127_cnt        ;
    cs_uint64 txstatsfrm128_255_cnt       ;
    cs_uint64 txstatsfrm256_511_cnt       ;
    cs_uint64 txstatsfrm512_1023_cnt      ;
    cs_uint64 txstatsfrm1024_1518_cnt     ;
    cs_uint64 txstatsfrm1519_Max_cnt      ;
    cs_uint64 txbyte_cnt                  ;
    cs_uint64 txfrm_cnt                   ;
    cs_uint32 single_collision            ;
    cs_uint32 multiple_collision          ;
    cs_uint32 fcs_errors                  ;
    cs_uint32 late_collisions             ;
    cs_uint32 excessive_collisions        ;    
}oam_port_uni_stats_t;


#define APP_STATS_POLL_ACCURACY 60000 /* 60s */

void app_port_stats_reset();
void app_port_stats_init();
void app_stats_polling_enable_set(cs_boolean  enable);

cs_status app_onu_port_stats_get(
        cs_port_id_t port_id,
        oam_port_uni_stats_t *uni_stats);

void app_onu_pon_stats_summary_get(
    cs_uint64 *in_frame, 
    cs_uint64 *in_errors, 
    cs_uint64 *in_err_symb);

cs_status app_onu_pon_stats_get(oam_pon_stats_t *pon_stats);
cs_status app_onu_std_pon_stats_get(epon_if_stats_t *if_stats);
void app_port_stats_polling(void * data);
void app_stats_reset_pon();

void stats_reset_cpu();

void app_stats_reset_uni(cs_port_id_t port_id);

cs_status app_show_pon_stats_cmd();
void app_show_uni_counter( cs_port_id_t   portid);
cs_status app_onu_port_status_change_get(cs_port_id_t port_id, cs_uint32 *count);

#ifdef HAVE_STATISTICS_NNI
cs_status app_onu_nni_stats_get(oam_port_uni_stats_t *uni_stats);
void app_stats_reset_nni();
#endif

#endif

