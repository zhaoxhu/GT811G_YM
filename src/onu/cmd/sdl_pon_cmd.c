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
/*
 * sdl_pon_cmd.c
 *
 * $Id: sdl_pon_cmd.c,v 1.1.2.3.2.33.2.3.2.13.2.1 2011/10/18 05:06:59 chuang Exp $
 */

#include "cs_cmd.h"

#ifdef HAVE_SDL_CMD

#include "aal_mpcp.h"
#include "aal_pon.h"
#include "aal_serdes.h"
#include "sdl_pon.h"
#include "sdl_mpcp.h"
#include "sdl_fec_cmn.h"
#include "sdl_enc.h"
#include "aal_enc.h"
#include "sdl_port.h"
#include "sdl_init.h"
#include "aal_fec.h"
//#include "ssp.h"

#ifdef HAVE_SDL_CMD_NTT
extern mpcp_zqt_cfg_t zqt_cfg;
extern mpcp_zqt_mon_t zqt_mon[];
extern mpcp_zqt_mode_t zqt_mode;
#endif

#if HAVE_SDL_CMD_NTT
extern cs_uint32   g_enc_intr_cnt;
extern cs_boolean  g_enc_encryped_packet_received;
extern cs_sdl_enc_mode_t g_enc_mode;
extern cs_sdl_enc_mode_t  g_enc_cur_mode;
#endif


cs_status sdl_enc_cfg_show(void)
{
    cs_aal_enc_cfg_t cfg;

    aal_enc_cfg_get(&cfg);

    cs_printf("Decryption Mode           : %d\n",  cfg.mode);
    cs_printf("Decryption Enable         : %s\n\n", cfg.en ? "Enable" : "Disable");
    cs_printf("OAM OUI                   : 0x%x\n",  cfg.oam_oui);
    cs_printf("OAM flag                  : 0x%x\n",  cfg.oam_flag);
    cs_printf("OAM Rpt Length            : 0x%x\n",  cfg.extra_oam_rpt_len);
    cs_printf("Single Churning Enable    : %s\n\n",  cfg.churn_mode ? "Enable" : "Disable");
    cs_printf("key generated Mode        : %s\n\n",  cfg.key_gen_mode ? "SW" : "HW");
    cs_printf("key Exchange  Mode        : %s\n\n",  cfg.key_exchange_mode ? "HW" : "SW");

    return CS_E_OK;
}

cs_status sdl_enc_tx_cfg_show(void)
{
    cs_aal_enc_tx_cfg_t tx_cfg;

    aal_enc_tx_cfg_get(&tx_cfg);
    cs_printf("Encryption Enable         : %s\n",  tx_cfg.en ? "Enable" : "Disable");
    cs_printf("Encryption Trigger cnt    : %d\n",  tx_cfg.threshold);
    cs_printf("Encryption Index Select   : %s\n",  tx_cfg.key_idx_sel_mode ? "From CSR" : "From Rx Decrypted");
    cs_printf("Encryption Index          : %d\n",  tx_cfg.key_idx);
    cs_printf("Key index notify          : %s\n",  tx_cfg.key_idx_notify_en ? "Enable" : "Disable");
    cs_printf("CRC8 Regeneration Enable  : %s\n",  tx_cfg.crc8_regen_en ? "Enable" : "Disable");
    cs_printf("CRC8 ReverseIn            : %s\n",  tx_cfg.crc8_in_reverse_en ? "Enable" : "Disable");
    cs_printf("CRC8 ReverseOut           : %s\n\n",tx_cfg.crc8_out_reverse_en ? "Enable" : "Disable");
    cs_printf("Encryption RCC B0 Select  : %d\n",  tx_cfg.rcc_b0_sel_mode);
    cs_printf("Encryption key Select     : %d\n",  tx_cfg.key_sel_mode);

    return CS_E_OK;
}

cs_status sdl_enc_timer_show(void)
{
    cs_aal_enc_timer_cfg_t timer;
    
    aal_enc_timer_cfg_get(&timer);
    cs_printf("\n******Encryption Timer Configuration******\n");
    cs_printf("Rx Timer Adjust Direction : %s\n",  timer.rx_dir ? "Add" : "Subtract");
    cs_printf("Rx Timer Adjust Value     : %d\n",  timer.rx_delta);
    cs_printf("Tx Timer Adjust Direction : %s\n",  timer.tx_dir ? "Add" : "Subtract");
    cs_printf("Timer Adjust Value        : %d\n",  timer.tx_delta);
    cs_printf("AES Timer2                : 0x%x\n", timer.timer_hi16);
    cs_printf("JAES Timer                : 0x%x\n\n", timer.jaes_timer);

    return CS_E_OK;
    
}

cs_status  sdl_enc_key_show(void)
{
    cs_aal_enc_key_t uc_key0, uc_key1, hw_key1, hw_key0;
    cs_uint8 *key = NULL;
    cs_uint32 i;
    
    aal_enc_key_get(0, &uc_key0);
    aal_enc_key_get(1, &uc_key1);
    aal_enc_key_hw_get(0, &hw_key0);
    aal_enc_key_hw_get(1, &hw_key1);

    cs_printf("\n******UC Key Status******\n");

    key = (cs_uint8 *)uc_key0;
    for(i=0; i<4; i++)
        cs_printf("UC Key Index 0 Key %d: 0x%02x%02x%02x%02x\n",  i, key[i*4+3], key[i*4+2], key[i*4+1], key[i*4]);
    key = (cs_uint8 *)uc_key1;
    for(i=0; i<4; i++)
        cs_printf("UC Key Index 1 Key %d: 0x%02x%02x%02x%02x\n",  i, key[i*4+3], key[i*4+2], key[i*4+1], key[i*4]);
    cs_printf("\n");

    cs_printf("*HW Generated Key Status*\n");
    key = (cs_uint8 *)hw_key0;
    for(i=0; i<4; i++)
        cs_printf("UC Key Index 0 Key %d: 0x%02x%02x%02x%02x\n",  i, key[i*4+3], key[i*4+2], key[i*4+1], key[i*4]);
    key = (cs_uint8 *)hw_key1;
    for(i=0; i<4; i++)
        cs_printf("UC Key Index 0 Key %d: 0x%02x%02x%02x%02x\n",  i, key[i*4+3], key[i*4+2], key[i*4+1], key[i*4]);
    cs_printf("\n");
    
    return CS_E_OK;

}

cs_status sdl_enc_cnt_show(void)
{
    cs_aal_enc_cnt_t stats;
    
    aal_enc_cnt_get(&stats);
    
    cs_printf("\n********ENC statistics********\n");
    cs_printf("AES Timer                 : 0x%x\n", stats.aes_timer);
    cs_printf("Local Timer               : 0x%x\n", stats.local_timer);
    cs_printf("Key Request OAM           : %d\n", stats.request_cnt);
    cs_printf("UC Debug Counter          : 0x%x\n", stats.local_cnt);
    cs_printf("Key toggle counter        : %d\n", stats.key_idx_tog_cnt);
    cs_printf("Packet header             : 0x%x\n", stats.rx_aes_flag);
    cs_printf("Enc last packet rcc_b0    : %d\n", stats.rcc_b0);
    cs_printf("crc32 err cnt (even key)  : %d\n", stats.crc_err_cnt_even_key);
    cs_printf("crc32 good cnt (even key) : %d\n", stats.crc_good_cnt_even_key);
    cs_printf("crc32 err cnt (odd key)   : %d\n", stats.crc_err_cnt_odd_key);
    cs_printf("crc32 good cnt (odd key)  : %d\n", stats.crc_good_cnt_odd_key);
    cs_printf("Rx Key Index              : %d\n", stats.rx_index);
    cs_printf("Tx Key Index              : %d\n", stats.tx_index);

#if HAVE_SDL_CMD_NTT
    cs_printf("Current mode              : %d\n", g_enc_cur_mode);
    cs_printf("Current mode(g_enc_mode)  : %d\n", g_enc_mode);
    cs_printf("Enc status                : %d\n", g_enc_encryped_packet_received);
    cs_printf("Enc interrupt cnt         : %d\n", g_enc_intr_cnt);
#endif

    return CS_E_OK;

}

cs_status sdl_fec_cfg_show(void)
{
    cs_aal_fec_cfg_t cfg;

    aal_fec_cfg_get(&cfg);
    cs_printf("TFEC Standard             : %s\n",   cfg.std_mode ? "2005 Std" : "2003 Std");
    cs_printf("Rx Replace Mode           : %d\n",   cfg.rx_rep_mode);
    cs_printf("Rx Preamble Mode          : %d\n",   cfg.rx_preamble_mode);
    cs_printf("Rx Bypass                 : %s\n",   cfg.rx_bypass ? "Enable" : "Disable");
    cs_printf("Rx Enable                 : %s\n",   cfg.rx_en ? "Enable" : "Disable");
    cs_printf("Tx Replace Mode           : %d\n",   cfg.tx_rep_mode);
    cs_printf("Tx Preamble Mode          : %d\n",   cfg.tx_preamble_mode);
    cs_printf("Tx Enable                 : %s\n\n", cfg.tx_en ? "Enable" : "Disable");

    return CS_E_OK;
}

cs_status sdl_fec_ext_cfg_show(void)
{
    cs_aal_fec_ext_cfg_t ext_cfg;

    aal_fec_ext_cfg_get(&ext_cfg);
    cs_printf("\n*****FEC Extension Configuration*****\n\n");
    cs_printf("Rx Path Clear             : %s\n", ext_cfg.rx_path_clr ? "Enable" : "Disable");
    cs_printf("Tx Path Clear             : %s\n", ext_cfg.tx_path_clr ? "Enable" : "Disable");
    cs_printf("Correct Counter Clear     : %s\n", ext_cfg.correct_cnt_clr ? "Enable" : "Disable");
    cs_printf("Uncorrect Counter Clear   : %s\n", ext_cfg.uncorrect_cnt_clr ? "Enable" : "Disable");
    cs_printf("Stop Correct Counter      : %s\n", ext_cfg.stop_correct_cnt ? "Enable" : "Disable");
    cs_printf("Stop Uncorrect Counter    : %s\n", ext_cfg.stop_uncorrect_cnt ? "Enable" : "Disable");
    cs_printf("Rx Counter Read Clear Mode: %d\n", ext_cfg.cnt_rd_clr_mode);
    cs_printf("Rx Status Clear           : %s\n", ext_cfg.rx_status_clr ? "Enable" : "Disable");
    cs_printf("Rx Window Protection      : %s\n", ext_cfg.rx_protection_window ? "Enable" : "Disable");
    cs_printf("Error Threshold           : %d\n", ext_cfg.err_threshold);
    cs_printf("Tx Encode Mode            : %s\n", ext_cfg.encode_mode ? "Cortina" : "Standard");
    cs_printf("Tx IPG Adjust Mode        : %d\n", ext_cfg.tx_ipg_adjust);
    cs_printf("Tx Error Send Mode        : %d\n\n", ext_cfg.err_send_mode);

    return CS_E_OK;
}

cs_status sdl_fec_errinj_cfg_show(void)
{
    cs_aal_fec_err_inj_cfg_t err_inj;

    aal_fec_err_inj_cfg_get(&err_inj);
    cs_printf("\n****FEC TX Error inject Config*****\n");
    cs_printf("Inject Enable             : %s\n", err_inj.en ? "Enable" : "Disable");
    cs_printf("Inject Burst Mode         : %s\n", err_inj.burst_en ? "Enable" : "Disable");
    cs_printf("Inject Packet Counter     : %d\n", err_inj.packet_cnt);
    cs_printf("Inject Byte Counter       : %d\n", err_inj.byte_cnt);
    cs_printf("Inject Bit Counter        : %d\n", err_inj.bit_cnt);
    cs_printf("Inject TFEC1  Enable      : %s\n", err_inj.tfec1_en ? "Enable" : "Disable");
    cs_printf("Inject TFEC2 Enable       : %s\n", err_inj.tfec2_en ? "Enable" : "Disable");
    cs_printf("Inject Data Enable        : %s\n", err_inj.data_en ? "Enable" : "Disable");
    cs_printf("Inject SFEC Enable        : %s\n", err_inj.sfec_en ? "Enable" : "Disable");
    cs_printf("Inject Parity Enable      : %s\n\n", err_inj.parity_en ? "Enable" : "Disable");

    return CS_E_OK;
}

cs_status sdl_fec_pktsize_cfg_show(void)
{
    cs_aal_fec_pktsize_cfg_t pkt_size;

    aal_fec_pktsize_cfg_get(&pkt_size);
    
    cs_printf("\n******FEC Packet Size Config******\n");
    cs_printf("Min Packet size           : %d\n", pkt_size.min);
    cs_printf("Max Packet size           : %d\n\n", pkt_size.max);

    return CS_E_OK;
}

cs_status sdl_fec_latency_cfg_show(void)
{
    cs_aal_fec_latency_cfg_t latency;

    aal_fec_latency_cfg_get(&latency);

    cs_printf("\n********FEC Latency Config********\n");
    cs_printf("Decode latency            : %d\n", latency.decode);
    cs_printf("Max Packet latency        : %d\n\n", latency.max_pkt);

    return CS_E_OK;
}

cs_status sdl_fec_comp_cfg_show(void)
{
    cs_aal_fec_comp_cfg_t comp;

    aal_fec_comp_cfg_get(&comp);

    cs_printf("\n********FEC Compensation Config********\n\n");
    cs_printf("Compensation Enable       : %s\n", comp.en ? "Enable" : "Disable");
    cs_printf("Compensation mode         : %d\n", comp.mode);
    cs_printf("Compensation Direction    : %s\n", comp.direction ? "Add" : "Subtract");
    cs_printf("Compensation counter      : %d\n", comp.counter);
    cs_printf("Egress compensation       : %d\n", comp.egress);
    cs_printf("Ingress compensation      : %d\n", comp.ingress);
    cs_printf("Timestamp compensation    : %d\n", comp.timestamp);
    cs_printf("AES RX PATH               : %d\n", comp.aes_rx_path);
    cs_printf("Churnning  RX PATH        : %d\n", comp.churn_rx_path);
    cs_printf("AES NON RX PATH           : %d\n", comp.aes_non_rx_path);
    cs_printf("Churning NON RX PATH      : %d\n\n", comp.churn_non_rx_path);

    return CS_E_OK;
}

cs_status sdl_fec_cnt_show(void)
{
    cs_aal_fec_cnt_t cnt;

    aal_fec_cnt_get(&cnt);

    cs_printf("\n******FEC Counter Show******\n");
    cs_printf("Rx Blocks          : 0x%x\n", cnt.rx_blks);
    cs_printf("Rx Correct Blocks  : 0x%x\n", cnt.rx_correct_blks);
    cs_printf("Rx Uncorrect Blocks: 0x%x\n", cnt.rx_uncorrect_blks);
    cs_printf("Rx Good Blocks     : 0x%x\n", cnt.rx_good_blks);
    cs_printf("Rx Correct Bytes   : 0x%x\n", cnt.rx_correct_bytes);
    cs_printf("SFEC Counter       : 0x%x\n", cnt.rx_sfec);
    cs_printf("TFEC Counter       : 0x%x\n", cnt.rx_tfec);
    cs_printf("SPKT Counter       : 0x%x\n", cnt.rx_spkt);
    cs_printf("EPKT Counter       : 0x%x\n", cnt.rx_epkt);

    return CS_E_OK;

}

cs_status sdl_fec_status_show(void)
{
    cs_aal_fec_status_t stats;

    aal_fec_status_get(&stats);

    cs_printf("\n******FEC Status Show******\n");
    cs_printf("Rx Data Correct Ack        : %s\n", stats.rx_data_corr_ack ? "Yes" : "No");
    cs_printf("Parity Buffer Full         : %s\n", stats.parity_buffer_full ? "Yes" : "No");
    cs_printf("Packet Buffer Full         : %s\n", stats.pkt_buffer_full ? "Yes" : "No");
    cs_printf("CMD buffer Full            : %s\n", stats.cmd_buffer_full ? "Yes" : "No");
    cs_printf("Code Word Buffer Full      : %s\n", stats.code_buffer_full ? "Yes" : "No");
    cs_printf("Pipe is Stall              : %s\n", stats.pipe_stuck ? "Yes" : "No");
    cs_printf("LOS TFEC1 Boundary         : %s\n", stats.los_tfec1_boundary ? "Yes" : "No");
    cs_printf("LOS TFEC2 Boundary         : %s\n", stats.los_tfec2_boundary ? "Yes" : "No");
    cs_printf("FEC Uncorrected detected   : %s\n", stats.uncorrected ? "Yes" : "No");
    cs_printf("FEC errors                 : %d\n", stats.errors);
    cs_printf("FEC Packet Status          : %d\n", stats.pkt_state);

    return CS_E_OK;

}

extern cs_status sdl_pon_trans_tx_pwr_set(cs_uint8 en);
#if 0
extern cs_status sdl_pon_trans_rx_pwr_set(cs_uint8 en);
extern cs_status sdl_pon_trans_rx_pwr_get(cs_uint8 *en);
extern cs_status sdl_pon_trans_tx_pwr_get(cs_uint8 *en);
#endif

#ifdef HAVE_SDL_CMD_NTT

extern cs_status epon_request_onu_mpcp_window_fit_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN mpcp_window_fit_mode_t      mode
);

extern cs_status epon_request_onu_mpcp_window_fit_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT mpcp_window_fit_mode_t     *mode
);

extern cs_status epon_request_onu_mpcp_zqt_timer_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint32                   interval,
    CS_IN cs_uint32                   count
);
extern cs_status epon_request_onu_mpcp_zqt_timer_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_uint32                  *interval,
    CS_OUT cs_uint32                  *count
);
extern cs_status epon_request_onu_mpcp_zqt_mode_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN mpcp_zqt_mode_t             mode
);
extern cs_status epon_request_onu_mpcp_zqt_mode_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT mpcp_zqt_mode_t            *mode
);
extern cs_status epon_request_onu_mpcp_zqt_factor_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint16                   factor_lo,
    CS_IN cs_uint16                   factor_hi
);
extern cs_status epon_request_onu_mpcp_zqt_factor_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_uint16                  *factor_lo,
    CS_OUT cs_uint16                  *factor_hi
);
#endif

cs_status epon_request_onu_mpcp_report_status_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                enable
);

extern cs_status epon_request_onu_mpcp_enable_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean                  enable
);

extern cs_status epon_request_onu_mpcp_enable_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean                 *enable
);

extern cs_status epon_request_onu_mpcp_offline_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              offline
);

extern cs_status epon_request_onu_mpcp_offline_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *offline
);

extern cs_status epon_request_onu_mpcp_gate_timer_threshold_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint16                   threshold
);

extern cs_status epon_request_onu_mpcp_gate_timer_threshold_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_uint16                  *threshold
);

extern cs_status epon_request_onu_mpcp_timer_threshold_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint16                   threshold
);
extern cs_status epon_request_onu_mpcp_timer_threshold_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_uint16                  *threshold
);

extern cs_status epon_request_onu_pon_laser_mode_set(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32            device,
    CS_IN cs_int32            llidport,
    CS_IN cs_sdl_pon_laser_mode_t mode
);
extern cs_status epon_request_onu_pon_laser_mode_get(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32            device,
    CS_IN cs_int32            llidport,
    CS_OUT cs_sdl_pon_laser_mode_t *mode
);
extern cs_status epon_request_onu_pon_link_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *link
);
extern cs_status epon_request_onu_pon_rx_enable_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              enable
);
extern cs_status epon_request_onu_pon_rx_enable_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *enable
);
extern cs_status epon_request_onu_pon_tx_enable_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              enable
);
extern cs_status epon_request_onu_pon_tx_enable_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *enable
);
extern cs_status epon_request_onu_pon_mtu_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint16               mtu
);
extern cs_status epon_request_onu_pon_mtu_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint16               *mtu
);
extern cs_status epon_request_onu_pon_preamble_mode_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              preamble
);
extern cs_status epon_request_onu_pon_preamble_mode_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *preamble
);

cs_status epon_request_onu_mpcp_report_status_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_OUT cs_boolean               *enable
);

cs_status epon_request_onu_port_lpbk_set(
    CS_IN cs_callback_context_t             context,
    CS_IN cs_int32                          device_id,
    CS_IN cs_int32                          llidport,
    CS_IN cs_port_id_t                      port_id,
    CS_IN cs_sdl_port_loopback_t            loopback
);

sal_cmd_result_t pon_cmd_rx(int argc, char **argv);
sal_cmd_result_t pon_cmd_tx(int argc, char **argv);
sal_cmd_result_t pon_cmd_mtu(int argc, char **argv);
sal_cmd_result_t pon_cmd_pm(int argc, char **argv);
sal_cmd_result_t pon_cmd_t(int argc, char **argv);
sal_cmd_result_t pon_cmd_zqt(int argc, char **argv);
sal_cmd_result_t pon_cmd_zq(int argc, char **argv);
sal_cmd_result_t pon_cmd_wf(int argc, char **argv);
sal_cmd_result_t pon_cmd_agg(int argc, char **argv);
sal_cmd_result_t pon_cmd_mpcp(int argc, char **argv);
sal_cmd_result_t pon_cmd_offline(int argc, char **argv);
sal_cmd_result_t pon_cmd_rpt(int argc, char **argv);
sal_cmd_result_t pon_cmd_gt(int argc, char **argv);
sal_cmd_result_t pon_cmd_mt(int argc, char **argv);
sal_cmd_result_t pon_cmd_lm(int argc, char **argv);
sal_cmd_result_t pon_cmd_ld(int argc, char **argv);
sal_cmd_result_t pon_cmd_zm(int argc, char **argv);
sal_cmd_result_t pon_cmd_zt(int argc, char **argv);
sal_cmd_result_t pon_cmd_zf(int argc, char **argv);
sal_cmd_result_t pon_cmd_show(int argc, char **argv);
sal_cmd_result_t pon_cmd_fec(int argc, char **argv);
#ifdef HAVE_SDL_CMD_CTC
sal_cmd_result_t pon_cmd_fec_mode(int argc, char **argv);
sal_cmd_result_t pon_cmd_enc_mode(int argc, char **argv);
sal_cmd_result_t pon_cmd_enc_cnt(int argc, char **argv);
sal_cmd_result_t pon_cmd_holdover(int argc, char **argv);
#endif
sal_cmd_result_t pon_cmd_trans_tx_pwr(int argc, char **argv);
#if 0
sal_cmd_result_t pon_cmd_trans_rx_pwr(int argc, char **argv);
sal_cmd_result_t pon_cmd_ss(int argc, char **argv);
sal_cmd_result_t pon_cmd_sg(int argc, char **argv);
sal_cmd_result_t pon_cmd_si(int argc, char **argv);
sal_cmd_result_t pon_cmd_lpbk(int argc, char **argv);
#endif
void __show_mac(void);
void __show_mpcp(void);
void __show_laser(void);
void __show_enc(void);
void __show_fec(void);
void __show_zqt(void);

#ifdef HAVE_SDL_CMD_HELP
static const char *pon_cmd_usage_fec[] = 
{
    "pon fec <0~1>",
    "   0: disable fec;",
    "   1: enable fec."
};

static const char *pon_cmd_usage_mpcp[] = 
{
    "pon mpcp <0~1>",
    "   0: disable mpcp engine;",
    "   1: enable mpcp engine."
};

static const char *pon_cmd_usage_offline[] = 
{
    "pon offline <0~1>",
    "   0: disable onu local offline;",
    "   1: enable onu local offline."
};

static const char *pon_cmd_usage_rpt[] = 
{
    "pon report <0~1>",
    "   0: disable onu report;",
    "   1: enable onu report."
};

static const char *pon_cmd_usage_gt[] = 
{
    "pon gt <0~4095>"
};

static const char *pon_cmd_usage_mt[] = 
{
    "pon gt <0~4095>"
};

static const char *pon_cmd_usage_lm[] = 
{
    "pon lm <0,1,3>",
    "   0: always off;",
    "   1: burst;",
    "   3: always on."
};

static const char *pon_cmd_usage_ld[] = 
{
    "pon ld <0~254>"
};

static const char *pon_cmd_usage_rx[] = 
{
    "pon rx <0~1>",
    "   0: disable pon mac rx",
    "   1: enable pon mac rx."
};

static const char *pon_cmd_usage_tx[] = 
{
    "pon tx <0~1>",
    "   0: disable pon mac tx;",
    "   1: enable pon mac tx."
};

static const char *pon_cmd_usage_mtu[] = 
{
    "pon mtu <0~1900>"
};

static const char *pon_cmd_usage_pm[] = 
{
    "pon pm <0~1>",
    "   0: 7B/8B mode",
    "   1: fixed 8B mode"
};

#ifdef HAVE_SDL_CMD_NTT
static const char *pon_cmd_usage_t[] = 
{
    "pon tt <0~65535>"
};

static const char *pon_cmd_usage_zqt[] = 
{
    "pon zqt <0~65535>"
};

static const char *pon_cmd_usage_zq[] = 
{
    "pon zqm <0~0x1f>",
    "   0: none of queue is set to zeroed queue;",
    "   1: queue 0 is set to zeroed queue;",
    "   ...",
    "   0x1f: all queues(queue 0~5) are set to zeroed queue."
};

static const char *pon_cmd_usage_wf[] = 
{
    "pon wf <0~1>",
    "   0: report sp, schedule sp;",
    "   1: report fit, schedule fit;",
    "   2: report sp, schedule fit;"
};

static const char *pon_cmd_usage_agg[] = 
{
    "pon aggr <0~1>",
    "   0: disable aggregation report;",
    "   1: enable aggregation report;"
};


static const char *pon_cmd_usage_zm[] = 
{
    "pon zrm <0,1>",
    "   0: normal mode (one threshold);",
    "   1: timer mode (two threshold);"
};

static const char *pon_cmd_usage_zt[] = 
{
    "pon zrt <interval:10~65535> <count:10~65535>",
    "   interval: polling interval, in unit of millisecond",
    "   count: polling times"
};

static const char *pon_cmd_usage_zf[] = 
{
    "pon ztf <low factor:10~65530> <high factor:10~65530>",
    "   low factor: in unit of percentage, in step of 10",
    "   high factor: in unit of percentage, in step of 10",
    "   NOTE: high factor should grater than low's"
};

#endif


#ifdef HAVE_SDL_CMD_CTC
static const char *pon_cmd_usage_fec_mode[] = 
{
    "pon fec_mode <0~3>",
    "   0:  rx_preamble_mode = 1; tx_preamble_mode = 1; rx_rep_mode = 0; tx_rep_mode = 1;",
    "   1:  rx_preamble_mode = 0; tx_preamble_mode = 1; rx_rep_mode = 0; tx_rep_mode = 1;",
    "   2:  rx_preamble_mode = 1; tx_preamble_mode = 0; rx_rep_mode = 0; tx_rep_mode = 1;",
    "   3:  rx_preamble_mode = 0; tx_preamble_mode = 0; rx_rep_mode = 1; tx_rep_mode = 1;"
};


static const char *pon_cmd_usage_encrypt_mode[] = 
{
    "pon enc <0~3>",
    "   0: Triple Churning Mode;",
    "   1: AES 32 mode;",
    "   2: AES 48 mode;",
    "   3: JAES mode;"
};

static const char *pon_cmd_usage_encrypt_cnt[] = 
{
    "pon enc_cnt <0~0xffffffff>",
};

static const char *pon_cmd_usage_holdover[] = 
{
    "pon holdover <state: 0(deactive)~1(active)>, <time: 50~4095(ms)>",
};

#endif 

static const char *pon_cmd_usage_show[] = 
{
    "pon show <'mac', 'mpcp', 'fec', 'enc', 'laser', ' '>",
    "   mac: show mac configuration;",
    "   mpcp: show mpcp configuration;",
    "   fec: show fec configuration;",
    "   enc: show enc configuration;",
    "   laser: show laser configuration;",
    "   NONE: show all configuration;"
};

static const char *pon_cmd_usage_trans_tx_pwr[] = 
{
    "pon tx_pwr <0~1>",
    "   0: disable transceiver tx power",
    "   1: enable transceiver tx power"
};

#if 0
static const char *pon_cmd_usage_lpbk[] = 
{
    "pon lpbk [none/tx2rx]"
};

static const char *pon_cmd_usage_trans_rx_pwr[] = 
{
    "pon rx_pwr <0~1>",
    "   0: disable transceiver rx power",
    "   1: enable transceiver rx power"
};

static const char *pon_cmd_usage_ss[] = 
{
    "pon ss <address> <data>",
    "   <address>: reg address",
    "   <data>: reg data to be set",
};

static const char *pon_cmd_usage_sg[] = 
{
    "pon sg <address>",
    "   <address>: reg address"
};
#define   TXD_SEL_MASK          0x00000001
#define   SWCTRL_TX_MASK        0x00000002
#define   SPEED_TX_MASK         0x00000004
#define   HSCLKSEL_TX_MASK      0x00000008
#define   MODE_RX_MASK          0x00000010
#define   SPEED_RX_MASK         0x00000020
#define   HSCLKSEL_RX_MASK      0x00000040
#define   REFCLK_UOSEL_PLL_MASK 0x00000080
#define   REFCLK_LOSEL_PLL_MASK 0x00000100
#define   REFCLKIN_SEL_PLL_MASK 0x00000200
#define   HSCLKSEL_PLL_MASK     0x00000400
#define   DIVSEL_PLL_MASK       0x00000800
#define   CLKOSEL_PLL_MASK      0x00001000
#define   PDZ_MISC_MASK         0x00002000
#define   PDZ_PMC_MASK          0x00004000
#define   SRSTZ_PMC_MASK        0x00008000
#define   __WAIT_10US_0_MASK    0x00010000
#define   PMC_START_MASK        0x00020000
#define   __WAIT_150US_MASK     0x00040000
#define   PDZ_PLL_MASK          0x00080000
#define   SRSTZ_PLL_MASK        0x00100000
#define   __WAIT_100US_MASK     0x00200000
#define   PLL_CONTROL_0_MASK    0x00400000
#define   __WAIT_10MS_MASK      0x00800000
#define   PDZ_RX_MASK           0x01000000
#define   PDZ_TX_MASK           0x02000000
#define   __WAIT_10US_1_MASK    0x04000000
#define   SRST_TX_MASK          0x08000000
#define   SRST_RX_MASK          0x10000000
#define   __WAIT_4T_MASK        0x20000000
#define   SRSTZ_DIGITAL_MASK    0x40000000
#define   SERDES_INIT_MASK      0x80000000


static const char *pon_cmd_usage_si[] = 
{
    "pon si <mask> <value>",
    "   <mask>: 0x00000001 - TXD_SEL",
    "           0x00000002 - SWCTRL_TX",
    "           0x00000004 - SPEED_TX",
    "           0x00000008 - HSCLKSEL_TX",
    "           0x00000010 - MODE_RX",
    "           0x00000020 - SPEED_RX",
    "           0x00000040 - HSCLKSEL_RX",
    "           0x00000080 - REFCLK_UOSEL_PLL",
    "           0x00000100 - REFCLK_LOSEL_PLL",
    "           0x00000200 - REFCLKIN_SEL_PLL",
    "           0x00000400 - HSCLKSEL_PLL",
    "           0x00000800 - DIVSEL_PLL",
    "           0x00001000 - CLKOSEL_PLL",
    "           0x00002000 - PDZ_MISC",
    "           0x00004000 - PDZ_PMC",
    "           0x00008000 - SRSTZ_PMC",
    "           0x00010000 - __WAIT_10US_0",
    "           0x00020000 - PMC_START",
    "           0x00040000 - __WAIT_150US",
    "           0x00080000 - PDZ_PLL",
    "           0x00100000 - SRSTZ_PLL",
    "           0x00200000 - __WAIT_100US",
    "           0x00400000 - PLL_CONTROL_0",
    "           0x00800000 - __WAIT_10MS",
    "           0x01000000 - PDZ_RX",
    "           0x02000000 - PDZ_TX",
    "           0x04000000 - __WAIT_10US_1",
    "           0x08000000 - SRST_TX",
    "           0x10000000 - SRST_RX",
    "           0x20000000 - __WAIT_4T",
    "           0x40000000 - SRSTZ_DIGITAL",
    "           0x80000000 - trigger init",
    "",
    "   <value>: data to be set"
};

static const char *pon_cmd_usage_slr[] = 
{
    "pon slr <0~255>"
};

static const char *pon_cmd_usage_slt[] = 
{
    "pon slt <0~255>"
};
#endif
#endif

static sal_cmd_reg_struct_t pon_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    //{pon_cmd_trans_rx_pwr, "rx_pwr", "configure transceiver rx power", pon_cmd_usage_trans_rx_pwr, sizeof(pon_cmd_usage_trans_rx_pwr)/4},
    {pon_cmd_trans_tx_pwr, "tx_pwr", "configure transceiver tx power", pon_cmd_usage_trans_tx_pwr, sizeof(pon_cmd_usage_trans_tx_pwr)/4},
#ifdef HAVE_SDL_CMD_CTC
    {pon_cmd_fec_mode, "fec_mode", "configure fec mode", pon_cmd_usage_fec_mode, sizeof(pon_cmd_usage_fec_mode)/4},
    {pon_cmd_enc_cnt,  "enc_cnt", "configure encrypt high 16bit counter(for aes 48 mode only)", pon_cmd_usage_encrypt_cnt, sizeof(pon_cmd_usage_encrypt_cnt)/4},
    {pon_cmd_enc_mode, "enc", "configure encryption mode", pon_cmd_usage_encrypt_mode, sizeof(pon_cmd_usage_encrypt_mode)/4},
    {pon_cmd_holdover, "holdover", "configure holdover state", pon_cmd_usage_holdover, sizeof(pon_cmd_usage_holdover)/4},
#endif
    {pon_cmd_fec, "fec", "configure fec enable(1) or not(0)", pon_cmd_usage_fec, sizeof(pon_cmd_usage_fec)/4},
    {pon_cmd_rx, "rx", "configure pon rx enable or not", pon_cmd_usage_rx, sizeof(pon_cmd_usage_rx)/4},
    {pon_cmd_tx, "tx", "configure pon tx enable or not", pon_cmd_usage_tx, sizeof(pon_cmd_usage_tx)/4},
    {pon_cmd_mtu, "mtu", "configure pon mtu", pon_cmd_usage_mtu, sizeof(pon_cmd_usage_mtu)/4},
    {pon_cmd_pm, "pm", "configure pon preamble mode", pon_cmd_usage_pm, sizeof(pon_cmd_usage_pm)/4},
#ifdef HAVE_SDL_CMD_NTT
    {pon_cmd_t, "tt", "configure t threshold", pon_cmd_usage_t, sizeof(pon_cmd_usage_t)/4},
    {pon_cmd_zqt, "zqt", "configure zeroed queue threshold", pon_cmd_usage_zqt, sizeof(pon_cmd_usage_zqt)/4},
    {pon_cmd_zq, "zqm", "configure zeroed queue map", pon_cmd_usage_zq, sizeof(pon_cmd_usage_zq)/4},
    {pon_cmd_wf, "wf", "configure window fit mode", pon_cmd_usage_wf, sizeof(pon_cmd_usage_wf)/4},
    {pon_cmd_agg, "aggr", "configure aggregation report enable or not", pon_cmd_usage_agg, sizeof(pon_cmd_usage_agg)/4},
    {pon_cmd_zm, "zrm", "configure zeroed queue threshold mode", pon_cmd_usage_zm, sizeof(pon_cmd_usage_zm)/4},
    {pon_cmd_zt, "zrt", "configure zeroed queue timer interval and count", pon_cmd_usage_zt, sizeof(pon_cmd_usage_zt)/4},
    {pon_cmd_zf, "ztf", "configure zeroed queue threshold factors", pon_cmd_usage_zf, sizeof(pon_cmd_usage_zf)/4},
#endif
    {pon_cmd_mpcp, "mpcp", "configure mpcp enable or not", pon_cmd_usage_mpcp, sizeof(pon_cmd_usage_mpcp)/4},
    {pon_cmd_offline, "offline", "configure onu local offline or not", pon_cmd_usage_offline, sizeof(pon_cmd_usage_offline)/4},
    {pon_cmd_rpt, "rpt", "configure onu report enable or not", pon_cmd_usage_rpt, sizeof(pon_cmd_usage_rpt)/4},
    {pon_cmd_gt, "gt", "configure gate timer threshold", pon_cmd_usage_gt, sizeof(pon_cmd_usage_gt)/4},
    {pon_cmd_mt, "mt", "configure mpcp timer threshold", pon_cmd_usage_mt, sizeof(pon_cmd_usage_mt)/4},
    {pon_cmd_lm, "lm", "configure onu laser mode", pon_cmd_usage_lm, sizeof(pon_cmd_usage_lm)/4},
    {pon_cmd_ld, "ld", "configure onu laser delay", pon_cmd_usage_ld, sizeof(pon_cmd_usage_ld)/4},
    {pon_cmd_show, "show", "show pon configuration", pon_cmd_usage_show, sizeof(pon_cmd_usage_show)/4},
#if 0
    {pon_cmd_lpbk, "lpbk", "configure zeroed queue threshold factors", pon_cmd_usage_lpbk, sizeof(pon_cmd_usage_lpbk)/4},
    {pon_cmd_ss, "ss", "pon serdes register set", pon_cmd_usage_ss, sizeof(pon_cmd_usage_ss)/4},
    {pon_cmd_sg, "sg", "pon serdes register get", pon_cmd_usage_sg, sizeof(pon_cmd_usage_sg)/4},
    {pon_cmd_si, "si", "pon serdes init parameter set", pon_cmd_usage_si, sizeof(pon_cmd_usage_si)/4},
    {pon_cmd_slr, "slr", "pon serdes rx latency set", pon_cmd_usage_slr, sizeof(pon_cmd_usage_slr)/4},
    {pon_cmd_slt, "slt", "pon serdes tx latency set", pon_cmd_usage_slt, sizeof(pon_cmd_usage_slt)/4},
#endif
#else
#ifdef HAVE_SDL_CMD_CTC
    {pon_cmd_fec_mode, "fec_mode"},// "configure fec mode"},// pon_cmd_usage_fec_mode, sizeof(pon_cmd_usage_fec_mode)/4},
    {pon_cmd_enc_cnt,  "enc_cnt"},// "configure encrypt high 16bit counter(for aes 48 mode only)"},// pon_cmd_usage_encrypt_cnt, sizeof(pon_cmd_usage_encrypt_cnt)/4},
    {pon_cmd_enc_mode, "enc"},// "configure encryption mode"},// pon_cmd_usage_encrypt_mode, sizeof(pon_cmd_usage_encrypt_mode)/4},
    {pon_cmd_holdover, "holdover"},// "configure encryption mode"},// pon_cmd_usage_encrypt_mode, sizeof(pon_cmd_usage_encrypt_mode)/4},
#endif
    {pon_cmd_fec, "fec"},// "configure fec enable(1) or not(0)"},// pon_cmd_usage_fec, sizeof(pon_cmd_usage_fec)/4},
    {pon_cmd_rx, "rx"},// "configure pon rx enable or not"},// pon_cmd_usage_rx, sizeof(pon_cmd_usage_rx)/4},
    {pon_cmd_tx, "tx"},// "configure pon tx enable or not"},// pon_cmd_usage_tx, sizeof(pon_cmd_usage_tx)/4},
    {pon_cmd_mtu, "mtu"},// "configure pon mtu"},// pon_cmd_usage_mtu, sizeof(pon_cmd_usage_mtu)/4},
    {pon_cmd_pm, "pm"},// "configure pon preamble mode"},// pon_cmd_usage_pm, sizeof(pon_cmd_usage_pm)/4},
#ifdef HAVE_SDL_CMD_NTT
    {pon_cmd_t, "tt"},// "configure t threshold"},// pon_cmd_usage_t, sizeof(pon_cmd_usage_t)/4},
    {pon_cmd_zqt, "zqt"},// "configure zeroed queue threshold"},// pon_cmd_usage_zqt, sizeof(pon_cmd_usage_zqt)/4},
    {pon_cmd_zq, "zqm"},// "configure zeroed queue map"},// pon_cmd_usage_zq, sizeof(pon_cmd_usage_zq)/4},
    {pon_cmd_wf, "wf"},// "configure window fit mode"},// pon_cmd_usage_wf, sizeof(pon_cmd_usage_wf)/4},
    {pon_cmd_agg, "aggr"},// "configure aggregation report enable or not"},// pon_cmd_usage_agg, sizeof(pon_cmd_usage_agg)/4},
    {pon_cmd_zm, "zrm"},// "configure zeroed queue threshold mode"},// pon_cmd_usage_zm, sizeof(pon_cmd_usage_zm)/4},
    {pon_cmd_zt, "zrt"},// "configure zeroed queue timer interval and count"},// pon_cmd_usage_zt, sizeof(pon_cmd_usage_zt)/4},
    {pon_cmd_zf, "ztf"},// "configure zeroed queue threshold factors"},// pon_cmd_usage_zf, sizeof(pon_cmd_usage_zf)/4},
#endif
    {pon_cmd_mpcp, "mpcp"},// "configure mpcp enable or not"},// pon_cmd_usage_mpcp, sizeof(pon_cmd_usage_mpcp)/4},
    {pon_cmd_offline, "offline"},// "configure onu local offline or not"},// pon_cmd_usage_offline, sizeof(pon_cmd_usage_offline)/4},
    {pon_cmd_rpt, "rpt"},// "configure onu report enable or not"},// pon_cmd_usage_rpt, sizeof(pon_cmd_usage_rpt)/4},
    {pon_cmd_gt, "gt"},// "configure gate timer threshold"},// pon_cmd_usage_gt, sizeof(pon_cmd_usage_gt)/4},
    {pon_cmd_mt, "mt"},// "configure mpcp timer threshold"},// pon_cmd_usage_mt, sizeof(pon_cmd_usage_mt)/4},
    {pon_cmd_lm, "lm"},// "configure onu laser mode"},// pon_cmd_usage_lm, sizeof(pon_cmd_usage_lm)/4},
    {pon_cmd_show, "show"},// "show pon configuration"},// pon_cmd_usage_show, sizeof(pon_cmd_usage_show)/4},
#endif
};

sal_cmd_cfg_t pon_cmd_cfg = {"pon", "pon configuration", pon_cmd_list, sizeof(pon_cmd_list)/sizeof(sal_cmd_reg_struct_t)};

sal_cmd_result_t pon_cmd_rx(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_boolean rx;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    rx = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_pon_rx_enable_set(context, 0, port, rx);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_tx(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_boolean rx;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    rx = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_pon_tx_enable_set(context, 0, port, rx);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_mtu(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_uint16 mtu;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    mtu = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_pon_mtu_set(context, 0, port, mtu);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_pm(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_boolean pm;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    pm = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_pon_preamble_mode_set(context, 0, port, pm);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

#ifdef HAVE_SDL_CMD_NTT
sal_cmd_result_t pon_cmd_t(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_uint16 t_threshold;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    t_threshold = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_mpcp_t_threshold_set(context, 0, port, t_threshold);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_zqt(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_uint16 zqt;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    zqt = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_mpcp_zeroed_threshold_set(context, 0, port, zqt);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_zq(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_uint8 zq;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    zq = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_mpcp_zeroed_queue_set(context, 0, port, zq);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_wf(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    mpcp_window_fit_mode_t wf;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    wf = (mpcp_window_fit_mode_t)iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_mpcp_window_fit_set(context, 0, port, wf);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_agg(int argc, char **argv)
{
    cs_aal_mpcp_report_cfg_t cfg;
    cs_aal_mpcp_report_msk_t mask;
    cs_status sdk_rt;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    mask.u32 = 0;
    mask.s.aggregation_mode_en = 1;
    cfg.aggregation_mode_en = iros_strtol(argv[2]);

    sdk_rt = aal_mpcp_report_cfg_set(mask, &cfg);
    return cs_status_2_cmd_rc_map(sdk_rt);       
}

sal_cmd_result_t pon_cmd_zm(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    mpcp_zqt_mode_t zm;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    zm = (mpcp_zqt_mode_t)iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_mpcp_zqt_mode_set(context, 0, port, zm);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_zt(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_uint32 zt_interval;
    cs_uint32 zt_count;

    if(argc!=4) return SAL_CMD_INVALID_PARAM;

    zt_interval = iros_strtol(argv[2]);
    zt_count = iros_strtol(argv[3]);
    sdl_rt = epon_request_onu_mpcp_zqt_timer_set(context, 0, port, zt_interval, zt_count);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_zf(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_uint32 zf_low;
    cs_uint32 zf_high;

    if(argc!=4) return SAL_CMD_INVALID_PARAM;

    zf_low = iros_strtol(argv[2]);
    zf_high = iros_strtol(argv[3]);
    sdl_rt = epon_request_onu_mpcp_zqt_factor_set(context, 0, port, zf_low, zf_high);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

#endif
sal_cmd_result_t pon_cmd_mpcp(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_boolean mpcp;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    mpcp = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_mpcp_enable_set(context, 0, port, mpcp);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_offline(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_boolean offline;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    offline = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_mpcp_offline_set(context, 0, port, offline);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_rpt(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_boolean rpt;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    rpt = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_mpcp_report_status_set(context, 0, port, rpt);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_gt(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_uint16 gt;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    gt = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_mpcp_gate_timer_threshold_set(context, 0, port, gt);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_mt(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_uint16 mt;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    mt = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_mpcp_timer_threshold_set(context, 0, port, mt);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_lm(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_sdl_pon_laser_mode_t lm;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    lm = (cs_sdl_pon_laser_mode_t)iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_pon_laser_mode_set(context, 0, port, lm);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_ld(int argc, char **argv)
{
    cs_status  sdl_rt;
    cs_aal_pon_laser_msk_t msk;
    cs_aal_pon_laser_cfg_t cfg;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;
    cfg.tx_delay = (cs_uint8)iros_strtol(argv[2]);
    msk.u32 = 0;
    msk.s.tx_delay = 1;

    sdl_rt = aal_pon_laser_cfg_set(msk, &cfg);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_fec(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  portid = CS_PON_PORT_ID ;
    cs_boolean enable;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    enable = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_fec_set(context, 0, portid, enable);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}
#if 0
sal_cmd_result_t pon_cmd_trans_rx_pwr(int argc, char **argv)
{
    //cs_callback_context_t context;
    cs_status  sdl_rt;
    //cs_port_id_t  portid = CS_PON_PORT_ID ;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    //cs_uint8 mode = iros_strtol(argv[2]);
    //sdl_rt = epon_request_onu_fec_mode_set(context, 0, portid, mode);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}
#endif
sal_cmd_result_t pon_cmd_trans_tx_pwr(int argc, char **argv)
{
    cs_status  sdl_rt;
    cs_callback_context_t   context;
    cs_uint8 enable;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    enable = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_pon_trans_tx_pwr_set(context, 0, 0, enable);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

#ifdef HAVE_SDL_CMD_CTC
sal_cmd_result_t pon_cmd_fec_mode(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  portid = CS_PON_PORT_ID ;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    cs_uint16 mode = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_fec_mode_set(context, 0, portid, mode);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}


sal_cmd_result_t pon_cmd_enc_mode(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  portid = CS_PON_PORT_ID ;
    cs_uint8 mode;
    cs_boolean enable;

    if(argc!=4) return SAL_CMD_INVALID_PARAM;

    mode = iros_strtol(argv[2]);
    enable = iros_strtol(argv[3]);
    sdl_rt = epon_request_onu_enc_mode_set(context, 0, portid, mode, enable);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_enc_cnt(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  portid = CS_PON_PORT_ID ;
    cs_uint16 mode;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    mode = iros_strtol(argv[2]);
    sdl_rt = epon_request_onu_enc_cnt_set(context, 0, portid, mode);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_holdover(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  sdl_rt;
    cs_port_id_t  portid = CS_PON_PORT_ID ;
    cs_uint32 state;
    cs_uint32 time;

    if(argc!=4) return SAL_CMD_INVALID_PARAM;

    state = iros_strtol(argv[2]);
    time = iros_strtol(argv[3]);
    sdl_rt = epon_request_onu_mpcp_timer_set(context, 0, portid, !state, time);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}
#endif

sal_cmd_result_t pon_cmd_show(int argc, char **argv)
{
    if((argc<2)||(argc>3)) return SAL_CMD_INVALID_PARAM;

    if(2==argc)
    {
        __show_mac();
        __show_mpcp();
        __show_laser();
        __show_enc();
        __show_fec();
        return SAL_CMD_OK; 
    }

    if(cmd_cmp(argv[2], "mac"))
    {
        __show_mac();
    }
    else if(cmd_cmp(argv[2], "mpcp"))
    {
        __show_mpcp();
    }
    else if(cmd_cmp(argv[2], "laser"))
    {
        __show_laser();
    }
    else if(cmd_cmp(argv[2], "fec"))
    {
        __show_fec();
    }
    else if(cmd_cmp(argv[2], "enc"))
    {
        __show_enc();
    }
    else if(cmd_cmp(argv[2], "zqt"))
    {
#ifdef HAVE_SDL_CMD_NTT
        __show_zqt();
#endif
    }
    else return SAL_CMD_INVALID_PARAM;

    return SAL_CMD_OK;       
}


void __show_mac(void)
{
    cs_callback_context_t context;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_boolean rx,tx,link,pm;
    cs_uint16 mtu;
    cs_aal_pon_mac_cfg_t cfg;
    cs_sdl_port_loopback_t loopback = SDL_PORT_LOOPBACK_NONE;

    epon_request_onu_pon_rx_enable_get(context, 0, port, &rx);
    epon_request_onu_pon_tx_enable_get(context, 0, port, &tx);
    epon_request_onu_pon_link_get(context, 0, port, &link);
    epon_request_onu_pon_mtu_get(context, 0, port, &mtu);
    epon_request_onu_pon_preamble_mode_get(context, 0, port, &pm);
    epon_request_onu_port_lpbk_get(context, 0, 0, port, &loopback);
    aal_pon_mac_cfg_get(&cfg);

    diag_printf("\nMAC CONFIGURATION\n");
    diag_printf("========================================\n");
    diag_printf("%-26s: %s\n", "rx", (rx ? "enable" : "disable"));
    diag_printf("%-26s: %s\n", "tx", (tx ? "enable" : "disable"));
    diag_printf("%-26s: %u\n", "mtu", mtu);
    diag_printf("%-26s: %s\n", "preamble mode", (pm ? "fixed 8B" : "7B/8B auto-adjust"));
    diag_printf("%-26s: %u(rx), %u(tx)\n", "ipg", cfg.rx_ipg, cfg.tx_ipg);
    diag_printf("%-26s: %s\n", "link", (link ? "up" : "down"));
    diag_printf("%-26s: %s\n", "loopback", (loopback ? "TX2RX" : "NONE"));
    diag_printf("========================================\n");
}

void __show_mpcp(void)
{
    cs_callback_context_t context;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_uint16 gt,mt;
#ifdef HAVE_SDL_CMD_NTT
    cs_uint16 t,zqt,f_lo, f_hi;
    cs_uint8 zq;
    mpcp_zqt_mode_t z_mode;
    mpcp_window_fit_mode_t wf;
    cs_uint32 invterval, count;
#endif
    cs_boolean mpcp,offline,rpt;
    cs_aal_mpcp_report_cfg_t cfg;

#ifdef HAVE_SDL_CMD_NTT
    epon_request_onu_mpcp_t_threshold_get(context, 0, port, &t);
    epon_request_onu_mpcp_zeroed_threshold_get(context, 0, port, &zqt);
    epon_request_onu_mpcp_zeroed_queue_get(context, 0, port, &zq);
    epon_request_onu_mpcp_window_fit_get(context, 0, port, &wf);
    epon_request_onu_mpcp_zqt_mode_get(context, 0, port, &z_mode);
    epon_request_onu_mpcp_zqt_timer_get(context, 0, port, &invterval, &count);
    epon_request_onu_mpcp_zqt_factor_get(context, 0, port, &f_lo, &f_hi);
#endif
    epon_request_onu_mpcp_enable_get(context, 0, port, &mpcp);
    epon_request_onu_mpcp_offline_get(context, 0, port, &offline);
    epon_request_onu_mpcp_report_status_get(context, 0, port, &rpt);
    epon_request_onu_mpcp_gate_timer_threshold_get(context, 0, port, &gt);
    epon_request_onu_mpcp_timer_threshold_get(context, 0, port, &mt);

    aal_mpcp_report_cfg_get(&cfg);

    diag_printf("\nMPCP CONFIGURATION\n");
    diag_printf("========================================\n");
#ifdef HAVE_SDL_CMD_NTT
    diag_printf("%-26s: %u\n", "t threshold", t);
    diag_printf("%-26s: %u\n", "zeroed threshold", zqt);
    diag_printf("%-26s: 0x%02x\n", "zeroed queue map", zq);
    diag_printf("%-26s: %u\n", "window fit mode", wf);
    diag_printf("%-26s: %s\n", "aggregation", (cfg.aggregation_mode_en ? "enable" : "disable"));
    diag_printf("%-26s: %s\n", "zqt mode", (z_mode ? "Timer mode (two threshold)" : "Normal mode (one threshold)"));
    diag_printf("%-26s: interval:%u, count:%u\n", "zqt timer", invterval, count);
    diag_printf("%-26s: low:%u, high:%u\n", "zqt factor", f_lo, f_hi);
#endif
    diag_printf("%-26s: %s\n", "mpcp", (mpcp ? "enable" : "disable"));
    diag_printf("%-26s: %s\n", "offline", (offline ? "enable" : "disable"));
    diag_printf("%-26s: %s\n", "report", (rpt ? "enable" : "enable"));
    diag_printf("%-26s: %u\n", "gate timer threshold", gt);
    diag_printf("%-26s: %u\n", "mpcp timer threshold", mt);
    diag_printf("========================================\n");
}

void __show_laser(void)
{
    cs_callback_context_t context;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_sdl_pon_laser_mode_t lm;
    cs_uint16 nt,ft;
    cs_aal_pon_laser_cfg_t cfg;

    epon_request_onu_pon_laser_mode_get(context, 0, port, &lm);
    epon_request_onu_pon_laser_on_time_get(context, 0, port, &nt);
    epon_request_onu_pon_laser_off_time_get(context, 0, port, &ft);

    aal_pon_laser_cfg_get(&cfg);

    diag_printf("\nLASER CONFIGURATION\n");
    diag_printf("========================================\n");
    diag_printf("%-26s: %s\n", "laser mode", (lm==1 ? "burst" : (lm ? "always on" : "always off")));
    diag_printf("%-26s: %u\n", "laser on time", nt);
    diag_printf("%-26s: %u\n", "laser off time", ft);
    diag_printf("%-26s: %s\n", "laser power", (cfg.tx_power_en ? "on" : "off"));
    diag_printf("%-26s: %u\n", "laser polarity", cfg.sig_loss_polarity);
    diag_printf("%-26s: %u\n", "laser delay", cfg.tx_delay);
    diag_printf("========================================\n");
}

void __show_fec(void)
{
    cs_callback_context_t context;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_boolean fec;
    cs_sdl_fec_mode_t fec_mode;

    epon_request_onu_fec_get(context, 0, port, &fec);
    epon_request_onu_fec_mode_get(context, 0, port, &fec_mode);

    diag_printf("\nFEC CONFIGURATION\n");
    diag_printf("========================================\n");
    diag_printf("%-26s: %s\n", "FEC", (fec ? "Enable" : "Disable"));   
    diag_printf("%-26s: %d\n", "FEC Mode", fec_mode);   
    
    sdl_fec_cfg_show();
    diag_printf("========================================\n");

}

void __show_enc(void)
{
    cs_callback_context_t context;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    

#ifdef HAVE_SDL_CMD_NTT
    cs_sdl_enc_mode_t enc;
#else
    cs_uint8 mode;
    cs_boolean enable;
    cs_uint16 counter;
#endif



#ifdef HAVE_SDL_CMD_NTT
    epon_request_onu_enc_get(context, 0, port, &enc);
#else
    epon_request_onu_enc_mode_get(context, 0, port, &mode, &enable);
    epon_request_onu_enc_cnt_get(context, 0, port, &counter);
#endif


    diag_printf("\nENC CONFIGURATION\n");
    diag_printf("========================================\n");
    


#ifdef HAVE_SDL_CMD_NTT
    diag_printf("%-26s: %u\n\n", "ENC mode", enc);
#else
    diag_printf("%-26s: %u\n", "ENC mode", mode);
    diag_printf("%-26s: %u\n", "ENC enable", enable);
    diag_printf("%-26s: %u\n\n", "High 16bit Counter", counter);    
#endif

    sdl_enc_cfg_show();
    
#ifdef HAVE_SDL_CMD_NTT
    sdl_enc_tx_cfg_show();
#endif

    diag_printf("========================================\n");

    
}
#ifdef HAVE_SDL_CMD_NTT
void __show_zqt(void)
{
    int i;
    cs_aal_mpcp_report_cfg_t cfg;

    aal_mpcp_report_cfg_get(&cfg);

    diag_printf("\nZQT CONFIGURATION\n");
    diag_printf("========================================\n");
    for(i=0; i<5; ++i)
    {
        diag_printf("%-26s[%d]: %u\n","zqt_high_counter", i, zqt_mon[i].zqt_high_counter);
    }
    for(i=0; i<5; ++i)
    {
        diag_printf("%-26s[%d]: %u\n","zqt_low_counter", i, zqt_mon[i].zqt_low_counter);
    }
    for(i=0; i<5; ++i)
    {
        diag_printf("%-26s[%d]: %u\n","zqt_state", i, zqt_mon[i].zqt_state);
    }

    diag_printf("%-26s: %u\n","zqt_high", zqt_cfg.zqt_high);
    diag_printf("%-26s: %u\n","zqt_low", zqt_cfg.zqt_low);
    diag_printf("%-26s: %u\n","zqt", zqt_cfg.zqt);
    diag_printf("%-26s: %u\n","zqt_map", zqt_cfg.zqt_map);
    diag_printf("%-26s: %u\n","zqt_interval", zqt_cfg.zqt_interval);
    diag_printf("%-26s: %u\n","zqt_count", zqt_cfg.zqt_count);
    diag_printf("%-26s: %u\n","zqt_mode", zqt_mode);
    diag_printf("%-26s: %u\n","zqt_map_hw", cfg.zeroq_map);
    diag_printf("%-26s: %u\n","zqt_hw", cfg.zeroq_threshold);
    diag_printf("========================================\n");
}
#endif

#if 0
void __show_trans_pwr(void)
{
    cs_uint8 rx_en;
    cs_uint8 tx_en;
    
    sdl_pon_trans_rx_pwr_get(&rx_en);
    sdl_pon_trans_tx_pwr_get(&tx_en);    


    diag_printf("\nTRANSCEIVER POWER\n");
    diag_printf("========================================\n");
    diag_printf("%-26s: %s\n", "rx power", (rx_en ? "ON" : "OFF"));
    diag_printf("%-26s: %s\n", "tx power", (tx_en ? "ON" : "OFF"));
    diag_printf("========================================\n");

}

sal_cmd_result_t pon_cmd_lpbk(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  status = 0;
    cs_port_id_t  portid = CS_PON_PORT_ID;
    cs_sdl_port_loopback_t loopback = 0;

    if(argc != 3) return SAL_CMD_INVALID_PARAM;

    if (cmd_cmp(argv[2], "none")) {
        loopback = SDL_PORT_LOOPBACK_NONE;
    } else if (cmd_cmp(argv[2], "tx2rx")) {
        loopback = SDL_PORT_LOOPBACK_TX2RX;
    } else {
        return SAL_CMD_INVALID_PARAM;
    }

    status = epon_request_onu_port_lpbk_set(context, 0, 0, portid, loopback);
    if(status != CS_E_OK){
        diag_printf("epon_request_onu_port_lpbk_set failed \n");    
        return cs_status_2_cmd_rc_map(status);       
    }

    return cs_status_2_cmd_rc_map(status);
}

sal_cmd_result_t pon_cmd_ss(int argc, char **argv)
{
    sal_cmd_result_t rt = 0;
    cs_uint8 addr = iros_strtol(argv[2]);
    cs_uint8 data = iros_strtol(argv[3]);
    cs_callback_context_t    context;

    if(argc!=4) return SAL_CMD_INVALID_PARAM;

    rt = cs_plat_ssp_serdes_write(context,0,0,addr, data);

    return cs_status_2_cmd_rc_map(rt);
}

sal_cmd_result_t pon_cmd_sg(int argc, char **argv)
{
    sal_cmd_result_t rt = 0;
    cs_uint8 addr = iros_strtol(argv[2]);
    cs_uint8 data;
    cs_callback_context_t    context;
    
    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    rt = cs_plat_ssp_serdes_read(context,0,0,addr, &data);
    cs_printf("0x%-2x\n", data);

    return cs_status_2_cmd_rc_map(rt);
}

void __dump_serdes_init_cfg(void)
{
    cs_printf("TXD_SEL          = %u\n", serdes_init_cfg.txd_sel         );
    cs_printf("SWCTRL_TX        = %u\n", serdes_init_cfg.swctrl_tx       );
    cs_printf("SPEED_TX         = %u\n", serdes_init_cfg.speed_tx        );
    cs_printf("HSCLKSEL_TX      = %u\n", serdes_init_cfg.hsclksel_tx     );
    cs_printf("MODE_RX          = %u\n", serdes_init_cfg.mode_rx         );
    cs_printf("SPEED_RX         = %u\n", serdes_init_cfg.speed_rx        );
    cs_printf("HSCLKSEL_RX      = %u\n", serdes_init_cfg.hsclksel_rx     );
    cs_printf("REFCLK_UOSEL_PLL = %u\n", serdes_init_cfg.refclk_uosel_pll);
    cs_printf("REFCLK_LOSEL_PLL = %u\n", serdes_init_cfg.refclk_losel_pll);
    cs_printf("REFCLKIN_SEL_PLL = %u\n", serdes_init_cfg.refclkin_sel_pll);
    cs_printf("HSCLKSEL_PLL     = %u\n", serdes_init_cfg.hsclksel_pll    );
    cs_printf("DIVSEL_PLL       = %u\n", serdes_init_cfg.divsel_pll      );
    cs_printf("CLKOSEL_PLL      = %u\n", serdes_init_cfg.clkosel_pll     );
    cs_printf("PDZ_MISC         = %u\n", serdes_init_cfg.pdz_misc        );
    cs_printf("PDZ_PMC          = %u\n", serdes_init_cfg.pdz_pmc         );
    cs_printf("SRSTZ_PMC        = %u\n", serdes_init_cfg.srstz_pmc       );
    cs_printf("__WAIT_10US_0    = %u\n", serdes_init_cfg.__wait_10us_0   );
    cs_printf("PMC_START        = %u\n", serdes_init_cfg.pmc_start       );
    cs_printf("__WAIT_150US     = %u\n", serdes_init_cfg.__wait_150us    );
    cs_printf("PDZ_PLL          = %u\n", serdes_init_cfg.pdz_pll         );
    cs_printf("SRSTZ_PLL        = %u\n", serdes_init_cfg.srstz_pll       );
    cs_printf("__WAIT_100US     = %u\n", serdes_init_cfg.__wait_100us    );
    cs_printf("PLL_CONTROL_0    = %u\n", serdes_init_cfg.pll_control_0   );
    cs_printf("__WAIT_10MS      = %u\n", serdes_init_cfg.__wait_10ms     );
    cs_printf("PDZ_RX           = %u\n", serdes_init_cfg.pdz_rx          );
    cs_printf("PDZ_TX           = %u\n", serdes_init_cfg.pdz_tx          );
    cs_printf("__WAIT_10US_1    = %u\n", serdes_init_cfg.__wait_10us_1   );
    cs_printf("SRST_TX          = %u\n", serdes_init_cfg.srst_tx         );
    cs_printf("SRST_RX          = %u\n", serdes_init_cfg.srst_rx         );
    cs_printf("__WAIT_4T        = %u\n", serdes_init_cfg.__wait_4t       );
    cs_printf("SRSTZ_DIGITAL    = %u\n", serdes_init_cfg.srstz_digital   );
}


sal_cmd_result_t pon_cmd_si(int argc, char **argv)
{
    sal_cmd_result_t rt = 0;
    cs_uint32 mask;
    cs_uint32 data;

    if(argc < 3) return SAL_CMD_INVALID_PARAM;

    mask = iros_strtol(argv[2]);
    data = iros_strtol(argv[3]);

    if(mask & TXD_SEL_MASK         ) serdes_init_cfg.txd_sel = data;
    if(mask & SWCTRL_TX_MASK       ) serdes_init_cfg.swctrl_tx = data;
    if(mask & SPEED_TX_MASK        ) serdes_init_cfg.speed_tx = data;
    if(mask & HSCLKSEL_TX_MASK     ) serdes_init_cfg.hsclksel_tx = data;
    if(mask & MODE_RX_MASK         ) serdes_init_cfg.mode_rx = data;
    if(mask & SPEED_RX_MASK        ) serdes_init_cfg.speed_rx = data;
    if(mask & HSCLKSEL_RX_MASK     ) serdes_init_cfg.hsclksel_rx = data;
    if(mask & REFCLK_UOSEL_PLL_MASK) serdes_init_cfg.refclk_uosel_pll = data;
    if(mask & REFCLK_LOSEL_PLL_MASK) serdes_init_cfg.refclk_losel_pll = data;
    if(mask & REFCLKIN_SEL_PLL_MASK) serdes_init_cfg.refclkin_sel_pll = data;
    if(mask & HSCLKSEL_PLL_MASK    ) serdes_init_cfg.hsclksel_pll = data;
    if(mask & DIVSEL_PLL_MASK      ) serdes_init_cfg.divsel_pll = data;
    if(mask & CLKOSEL_PLL_MASK     ) serdes_init_cfg.clkosel_pll = data;
    if(mask & PDZ_MISC_MASK        ) serdes_init_cfg.pdz_misc = data;
    if(mask & PDZ_PMC_MASK         ) serdes_init_cfg.pdz_pmc = data;
    if(mask & SRSTZ_PMC_MASK       ) serdes_init_cfg.srstz_pmc = data;
    if(mask & __WAIT_10US_0_MASK   ) serdes_init_cfg.__wait_10us_0 = data;
    if(mask & PMC_START_MASK       ) serdes_init_cfg.pmc_start = data;
    if(mask & __WAIT_150US_MASK    ) serdes_init_cfg.__wait_150us = data;
    if(mask & PDZ_PLL_MASK         ) serdes_init_cfg.pdz_pll = data;
    if(mask & SRSTZ_PLL_MASK       ) serdes_init_cfg.srstz_pll = data;
    if(mask & __WAIT_100US_MASK    ) serdes_init_cfg.__wait_100us = data;
    if(mask & PLL_CONTROL_0_MASK   ) serdes_init_cfg.pll_control_0 = data;
    if(mask & __WAIT_10MS_MASK     ) serdes_init_cfg.__wait_10ms = data;
    if(mask & PDZ_RX_MASK          ) serdes_init_cfg.pdz_rx = data;
    if(mask & PDZ_TX_MASK          ) serdes_init_cfg.pdz_tx = data;
    if(mask & __WAIT_10US_1_MASK   ) serdes_init_cfg.__wait_10us_1 = data;
    if(mask & SRST_TX_MASK         ) serdes_init_cfg.srst_tx = data;
    if(mask & SRST_RX_MASK         ) serdes_init_cfg.srst_rx = data;
    if(mask & __WAIT_4T_MASK       ) serdes_init_cfg.__wait_4t = data;
    if(mask & SRSTZ_DIGITAL_MASK   ) serdes_init_cfg.srstz_digital = data;
    if(mask & SERDES_INIT_MASK     ) rt = serdes_init(&serdes_init_cfg);
    if(mask == 0) __dump_serdes_init_cfg();

    return cs_status_2_cmd_rc_map(rt);
}

sal_cmd_result_t pon_cmd_slr(int argc, char **argv)
{
    cs_status  sdl_rt;
    cs_aal_pon_serdes_msk_t msk;
    cs_aal_pon_serdes_cfg_t cfg;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    cfg.rx_latency= (cs_uint8)iros_strtol(argv[2]);
    msk.u32 = 0;
    msk.s.rx_latency = 1;

    sdl_rt = aal_pon_serdes_cfg_set(msk, &cfg);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}

sal_cmd_result_t pon_cmd_slt(int argc, char **argv)
{
    cs_status  sdl_rt;
    cs_aal_pon_serdes_msk_t msk;
    cs_aal_pon_serdes_cfg_t cfg;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    cfg.tx_latency= (cs_uint8)iros_strtol(argv[2]);
    msk.u32 = 0;
    msk.s.tx_latency = 1;

    sdl_rt = aal_pon_serdes_cfg_set(msk, &cfg);
    return cs_status_2_cmd_rc_map(sdl_rt);       
}
#endif
#endif //#ifdef HAVE_SDL_CMD

cs_status pon_cmd_self_reg(void)
{
        cs_status ret = CS_E_OK;
#ifdef HAVE_SDL_CMD
    
        ret = sal_cmd_reg(&pon_cmd_cfg);
#endif
        return ret;
}


