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
 * sdl_pon_api.h
 *
 * $Id: sdl_pon.h,v 1.1.2.1.14.8.6.1 2012/06/01 02:45:04 jsheng Exp $
 */

#ifndef __SDL_PON_H__
#define __SDL_PON_H__

#include "cs_types.h"
#include "plat_common.h"

/* PON stats */
typedef struct {
    cs_uint64 pon_byte_cnt           ;
    cs_uint64 pon_tx_byte_cnt        ;
    cs_uint32 pon_frame_cnt          ;
    cs_uint32 pon_txframe_cnt        ;
    cs_uint32 pon_crcerr_cnt         ;
    cs_uint32 pon_mcframe_cnt        ;
    cs_uint32 pon_bcframe_cnt        ;
    cs_uint32 pon_txmcframe_cnt      ;
    cs_uint32 pon_txbcframe_cnt      ;
    cs_uint32 pon_ctrlframe_cnt      ;
    cs_uint32 pon_txctrlframe_cnt    ;
    cs_uint32 pon_pauseframe_cnt     ;
    cs_uint32 pon_unknownop_cnt      ;
    cs_uint32 pon_runt_cnt           ;
    cs_uint32 pon_oversize_cnt       ;
    cs_uint32 pon_rmon64_cnt         ;
    cs_uint32 pon_rmon65_127_cnt     ;
    cs_uint32 pon_rmon128_255_cnt    ;
    cs_uint32 pon_rmon256_511_cnt    ;
    cs_uint32 pon_rmon512_1023_cnt   ;
    cs_uint32 pon_rmon1024_1518_cnt  ;
    cs_uint32 pon_txrmon64_cnt       ;
    cs_uint32 pon_txrmon65_127_cnt   ;
    cs_uint32 pon_txrmon128_255_cnt  ;
    cs_uint32 pon_txrmon256_511_cnt  ;
    cs_uint32 pon_txrmon512_1023_cnt ;
    cs_uint32 pon_txrmon1024_1518_cnt;
    cs_uint32 pon_mpcp_uc_rev_cnt     ;
    cs_uint32 pon_mpcp_uc_reg_ack     ;
    cs_uint32 pon_mpcp_uc_reg_nack    ;
    cs_uint32 pon_mpcp_uc_reg_dereg   ;
    cs_uint32 pon_mpcp_uc_reg_rereg   ;
    cs_uint32 pon_mpcp_uc_reg_reg_rev ;
    cs_uint32 pon_mpcp_uc_gat_rev     ;
    cs_uint32 pon_mpcp_uc_gat_norm    ;
    cs_uint32 pon_mpcp_uc_gat_frpt    ;
    cs_uint32 pon_mpcp_uc_gat_udis    ;
    cs_uint32 pon_mpcp_uc_gat_bdis    ;
    cs_uint32 pon_mpcp_mc_rev_cnt     ;
    cs_uint32 pon_mpcp_mc_reg_ack     ;
    cs_uint32 pon_mpcp_mc_reg_nack    ;
    cs_uint32 pon_mpcp_mc_reg_dereg   ;
    cs_uint32 pon_mpcp_mc_reg_rereg   ;
    cs_uint32 pon_mpcp_mc_reg_reg_rev ;
    cs_uint32 pon_mpcp_mc_gat_rev     ;
    cs_uint32 pon_mpcp_mc_gat_norm    ;
    cs_uint32 pon_mpcp_mc_gat_frpt    ;
    cs_uint32 pon_mpcp_mc_gat_udis    ;
    cs_uint32 pon_mpcp_mc_gat_bdis    ;
    cs_uint32 pon_mpcp_bc_rev_cnt     ;
    cs_uint32 pon_mpcp_bc_reg_ack     ;
    cs_uint32 pon_mpcp_bc_reg_nack    ;
    cs_uint32 pon_mpcp_bc_reg_dereg   ;
    cs_uint32 pon_mpcp_bc_reg_rereg   ;
    cs_uint32 pon_mpcp_bc_reg_reg_rev ;
    cs_uint32 pon_mpcp_bc_gat_rev     ;
    cs_uint32 pon_mpcp_bc_gat_norm    ;
    cs_uint32 pon_mpcp_bc_gat_frpt    ;
    cs_uint32 pon_mpcp_bc_gat_udis    ;
    cs_uint32 pon_mpcp_bc_gat_bdis    ;
    cs_uint32 pon_mpcp_tx_req_dereg   ;
    cs_uint32 pon_mpcp_tx_req_reg     ;
    cs_uint32 pon_mpcp_tx_ack_ack     ;
    cs_uint32 pon_mpcp_tx_ack_nack    ;
    cs_uint32 pon_mpcp_tx_rpt    ;
    cs_uint32 pon_mpcp_tx_oam    ;
    cs_uint32 pon_mpcp_da_mc_nslf;
    cs_uint32 pon_mpcp_da_mc_slf ;
    cs_uint32 pon_mpcp_da_uc     ;
    cs_uint32 pon_framecnt       ;
    cs_uint32 pon_txoversizecnt  ;
    cs_uint32 pon_rx1519_maxcnt  ;
    cs_uint32 pon_tx1519_maxcnt  ;
    cs_uint32 pon_txframecnt     ;
    cs_uint32 pon_txpausecnt     ;
    cs_uint32 pon_undersizecnt   ;
    cs_uint32 pon_jabbercnt      ;
    cs_uint32 pon_extensioncnt   ;
    cs_uint32 pon_txextensioncnt ;
    cs_uint32 pon_err_symbol     ;
    cs_uint32 pon_outofsync      ;
    cs_uint32 pon_sld_err        ;
    cs_uint32 pon_crc8_err       ;
    cs_uint32 pon_mac_drop       ;
    cs_uint32 pon_ipg_drop       ;
    cs_uint32 pon_drain          ;
    cs_uint32 pon_tx_crc_err     ;
}cs_pon_stats_t;

typedef enum 
{
    SDL_PON_LASER_MODE_OFF                          = 0, /* laser is always off*/
    SDL_PON_LASER_MODE_BURST                        = 1, /* normal burst mode */
    SDL_PON_LASER_MODE_ON                           = 3, /* laser is always on */
} cs_sdl_pon_laser_mode_t;


/********************************************************************************/
/* $rtn_hdr_start  epon_request_onu_pon_laser_on_time_get                       */
/* CATEGORY   : Device                                                          */
/* ACCESS     : Public                                                          */
/* BLOCK      : SDL                                                             */
/* CHIP       : 8030                                                            */
cs_status epon_request_onu_pon_laser_on_time_get(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32            device,
    CS_IN cs_int32            llidport,
    CS_OUT cs_uint16          *time
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                     */
/*              synchronous invoking, not NULL for iROS invoking                */
/*              o device -- refer to section 3.3.                               */
/*              o llidport -- llid to identify which                            */
/*              is intended for, -1 for synchronous invoking                    */
/* OUTPUTS    : o time -- laser on time, unit of TQ                             */
/* RETURNS    : o result code, 0 - success, 1 - fail,                           */
/*              6 - invalid parameters                                          */
/* DESCRIPTION:                                                                 */
/* This function is used to get ONU laser on time                               */
/* $rtn_hdr_end                                                                 */
/********************************************************************************/

/********************************************************************************/
/* $rtn_hdr_start  epon_request_onu_pon_laser_off_time_get                      */
/* CATEGORY   : Device                                                          */
/* ACCESS     : Public                                                          */
/* BLOCK      : SDL                                                             */
/* CHIP       : 8030                                                            */
cs_status epon_request_onu_pon_laser_off_time_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_uint16              *time
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                     */
/*              synchronous invoking, not NULL for iROS invoking                */
/*              o device -- refer to section 3.3.                               */
/*              o llidport -- llid to identify which                            */
/*              is intended for, -1 for synchronous invoking                    */
/* OUTPUTS    : o time -- laser off time, unit of TQ                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,                           */
/*              6 - invalid parameters                                          */
/* DESCRIPTION:                                                                 */
/* This function is used to get ONU laer off time                               */
/* $rtn_hdr_end                                                                 */
/********************************************************************************/

/********************************************************************************/
/* $rtn_hdr_start  epon_request_onu_pon_stats_clr                               */
/* CATEGORY   : Device                                                          */
/* ACCESS     : Public                                                          */
/* BLOCK      : SDL                                                             */
/* CHIP       : 8030                                                            */
cs_status epon_request_onu_pon_stats_clr(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                     */
/*              synchronous invoking, not NULL for iROS invoking                */
/*              o device -- refer to section 3.3.                               */
/*              o llidport -- llid to identify which                            */
/*              is intended for, -1 for synchronous invoking                    */
/* OUTPUTS    : o NA                                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,                           */
/*              6 - invalid parameters                                          */
/* DESCRIPTION:                                                                 */
/* This function is used to reset ONU PON all MIB counters                      */
/* $rtn_hdr_end                                                                 */
/********************************************************************************/

/********************************************************************************/
/* $rtn_hdr_start  epon_request_onu_pon_stats_get                               */
/* CATEGORY   : Device                                                          */
/* ACCESS     : Public                                                          */
/* BLOCK      : SDL                                                             */
/* CHIP       : 8030                                                            */
cs_status epon_request_onu_pon_stats_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              read_clear,
    CS_OUT cs_pon_stats_t             *stats
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                     */
/*              synchronous invoking, not NULL for iROS invoking                */
/*              o device -- refer to section 3.3.                               */
/*              o llidport -- llid to identify which                            */
/*              is intended for, -1 for synchronous invoking                    */
/*              o read_clear -- read clear or not, 1: clear, 0: not clear       */
/* OUTPUTS    : o stats -- ONU PON statistics                                   */
/* RETURNS    : o result code, 0 - success, 1 - fail,                           */
/*              6 - invalid parameters                                          */
/* DESCRIPTION:                                                                 */
/* This function is used to get ONU PON statistics                              */
/* $rtn_hdr_end                                                                 */
/********************************************************************************/

/********************************************************************************/
/* $rtn_hdr_start  epon_request_onu_pon_stats_get_pwrsv                         */
/* CATEGORY   : Device                                                          */
/* ACCESS     : Public                                                          */
/* BLOCK      : SDL                                                             */
/* CHIP       : 8030                                                            */

cs_status epon_request_onu_pon_stats_get_pwrsv(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              read_clear,
    CS_OUT cs_pon_stats_t             *stats
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                     */
/*              synchronous invoking, not NULL for iROS invoking                */
/*              o device -- refer to section 3.3.                               */
/*              o llidport -- llid to identify which                            */
/*              is intended for, -1 for synchronous invoking                    */
/*              o read_clear -- read clear or not, 1: clear, 0: not clear       */
/* OUTPUTS    : o stats -- ONU PON statistics                                   */
/* RETURNS    : o result code, 0 - success, 1 - fail,                           */
/*              6 - invalid parameters                                          */
/* DESCRIPTION:                                                                 */
/* This function is used to get ONU PON statistics                              */
/* $rtn_hdr_end                                                                 */
/********************************************************************************/


/********************************************************************************/
/* $rtn_hdr_start  epon_request_onu_pon_mac_addr_set                            */
/* CATEGORY   : Device                                                          */
/* ACCESS     : Public                                                          */
/* BLOCK      : SAL                                                             */
/* CHIP       : 8030                                                            */
cs_status epon_request_onu_pon_mac_addr_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_mac_t                    mac
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                     */
/*              synchronous invoking, not NULL for iROS invoking                */
/*              o device -- refer to section 3.3.                               */
/*              o llidport -- llid to identify which                            */
/*              is intended for, -1 for synchronous invoking                    */
/*              o mac -- ONU PON MAC address                                    */
/* OUTPUTS    : o NA                                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,                           */
/*              6 - invalid parameters                                          */
/* DESCRIPTION:                                                                 */
/* This function is used to set ONU PON MAC address                             */
/* $rtn_hdr_end                                                                 */
/********************************************************************************/

/********************************************************************************/
/* $rtn_hdr_start  epon_request_onu_pon_mac_addr_get                            */
/* CATEGORY   : Device                                                          */
/* ACCESS     : Public                                                          */
/* BLOCK      : SAL                                                             */
/* CHIP       : 8030                                                            */
cs_status epon_request_onu_pon_mac_addr_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_mac_t                    *mac
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                     */
/*              synchronous invoking, not NULL for iROS invoking                */
/*              o device -- refer to section 3.3.                               */
/*              o llidport -- llid to identify which                            */
/*              is intended for, -1 for synchronous invoking                    */
/* OUTPUTS    : o mac -- ONU PON MAC address                                    */
/* RETURNS    : o result code, 0 - success, 1 - fail,                           */
/*              6 - invalid parameters                                          */
/* DESCRIPTION:                                                                 */
/* This function is used to get ONU PON MAC address                             */
/* $rtn_hdr_end                                                                 */
/********************************************************************************/

cs_status epon_request_onu_pon_trans_rx_pwr_set(
    CS_IN cs_callback_context_t   context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              en);

cs_status epon_request_onu_pon_trans_rx_pwr_get(
    CS_IN cs_callback_context_t   context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *en);

cs_status epon_request_onu_pon_trans_tx_pwr_set(
    CS_IN cs_callback_context_t   context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              en);

cs_status epon_request_onu_pon_trans_tx_pwr_get(
    CS_IN cs_callback_context_t   context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *en);

cs_status epon_request_onu_pon_rx_enable_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_boolean              enable);

cs_status epon_request_onu_pon_rx_enable_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *enable);

cs_status epon_request_onu_pon_laser_mode_set(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32            device,
    CS_IN cs_int32            llidport,
    CS_IN cs_sdl_pon_laser_mode_t mode);

cs_status epon_request_onu_pon_laser_mode_get(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32            device,
    CS_IN cs_int32            llidport,
    CS_OUT cs_sdl_pon_laser_mode_t *mode);

cs_status epon_request_onu_pon_link_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *link);

cs_status epon_request_onu_pon_tx_enable_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *enable
);
cs_status epon_request_onu_pon_mtu_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_uint16               *mtu
);

cs_status epon_request_onu_pon_preamble_mode_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_boolean             *preamble
);

#endif

