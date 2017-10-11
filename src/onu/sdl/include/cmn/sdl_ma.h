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
 * $Id: sdl_ma.h,v 1.1.2.3.14.7.6.1 2013/05/08 07:15:35 panfeng Exp $
 */

#ifndef __SDL_MA_H__
#define __SDL_MA_H__

#include "cs_types.h"
#include "plat_common.h"
typedef enum {
    SDL_PKT_NORMAL            = 0,
    SDL_PKT_BPDU              = 1,
    SDL_PKT_8021X             = 2,
    SDL_PKT_IEEE_RSRVD1       = 3,
    SDL_PKT_MYMAC             = 4,
    SDL_PKT_MC_A              = 5,
    SDL_PKT_MC_B              = 6,
    SDL_PKT_MC_C              = 7,
    SDL_PKT_IGMP              = 8,
    SDL_PKT_MLD               = SDL_PKT_IGMP,
    SDL_PKT_ARP               = 9,
    SDL_PKT_OAM               = 10,
    SDL_PKT_MPCP              = 11,
    SDL_PKT_DHCP              = 12,
    SDL_PKT_PPPoE_DIS         = 13,
    SDL_PKT_PPPoE_Session     = 14,
    SDL_PKT_HELLO             = 15,
    SDL_PKT_IPV6NDP           = 16,
    SDL_PKT_UDF0              = 17,
    SDL_PKT_UDF1              = 18,
    SDL_PKT_UDF2              = 19,
    SDL_PKT_UDF3              = 20,  
    SDL_PKT_SWT               = 21,  
}cs_sdl_pkt_type_t;          /*check if need to put into sdl*/

typedef struct {
    cs_uint64 ma_rxbytecount             ;
    cs_uint64 ma_txbytecount             ;
    cs_uint32 ma_rxucpktcnt              ;
    cs_uint32 ma_rxmcfrmcnt              ;
    cs_uint32 ma_rxbcfrmcnt              ;
    cs_uint32 ma_rxoamfrmcnt             ;
    cs_uint32 ma_rxpausefrmcnt           ;
    cs_uint32 ma_rxunknownocfrmcnt       ;
    cs_uint32 ma_rxcrcerrfrmcnt          ;
    cs_uint32 ma_rxundersizefrmcnt       ;
    cs_uint32 ma_rxruntfrmcnt            ;
    cs_uint32 ma_rxovsizefrmcnt          ;
    cs_uint32 ma_rxjabberfrmcnt          ;
    cs_uint32 ma_rxstatsfrm64oct         ;
    cs_uint32 ma_rxstatsfrm65to127oct    ;
    cs_uint32 ma_rxstatsfrm128to255oct   ;
    cs_uint32 ma_rxstatsfrm256to511oct   ;
    cs_uint32 ma_rxstatsfrm512to1023oct  ;
    cs_uint32 ma_rxstatsfrm1024to1518oct ;
    cs_uint32 ma_rxstatsfrm1519tomaxoct  ;
    cs_uint32 ma_rxfrmcount              ;
    cs_uint32 ma_txucpktcnt              ;
    cs_uint32 ma_txmcfrmcnt              ;
    cs_uint32 ma_txbcfrmcnt              ;
    cs_uint32 ma_txoamfrmcnt             ;
    cs_uint32 ma_txpausefrmcnt           ;
    cs_uint32 ma_txcrcerrfrmcnt          ;
    cs_uint32 ma_txovsizefrmcnt          ;
    cs_uint32 ma_txestatsfrm64oct        ;
    cs_uint32 ma_txestatsfrm65to127oct   ;
    cs_uint32 ma_txestatsfrm128to255oct  ;
    cs_uint32 ma_txestatsfrm256to511oct  ;
    cs_uint32 ma_txestatsfrm512to1023oct ;
    cs_uint32 ma_txestatsfrm1024to1518oct;
    cs_uint32 ma_txestatsfrm1519tomaxoct ;
    cs_uint32 ma_txfrmcount              ;
}cs_ma_stats_t;

typedef struct
{
    cs_uint32 cnt_normal            ;
    cs_uint32 cnt_bpdu              ;
    cs_uint32 cnt_8021x             ;
    cs_uint32 cnt_ieee_rsrvd1       ;
    cs_uint32 cnt_igmp              ;
    cs_uint32 cnt_arp               ;
    cs_uint32 cnt_oam               ;
    cs_uint32 cnt_mpcp              ;
    cs_uint32 cnt_mymac            ;
    cs_uint32 cnt_ipv4              ;
    cs_uint32 cnt_ipv6              ;
    cs_uint32 cnt_hello             ;
    cs_uint32 cnt_udf0              ;
    cs_uint32 cnt_udf1              ;
    cs_uint32 cnt_udf2              ;
    cs_uint32 cnt_udf3              ;
}__cpu_rx_stats_t;

/* Packet RX callback functions */
typedef cs_status (*cs_sdl_pkt_rx_f)(cs_pkt_t *pkt);

#define MA_RX_BUF_HEAD_LEN 4
/* RX callback for packet with CPU-tag inside - from external switch */
typedef cs_int32 (*cs_sdl_sw_pkt_rx_f)(cs_uint8 *pkt, cs_uint16 in_len, cs_uint16 *out_len, cs_uint8 *s_port);
cs_uint32 __ma_rx_parser_hook_reg(cs_sdl_sw_pkt_rx_f rx_hook);
/* TX callback for packet - from cpu to switch */
typedef cs_int32 (*cs_sdl_sw_pkt_tx_f)(cs_uint8 *pkt_in, cs_uint16 in_len, cs_uint8 *pkt_out, cs_uint16 *out_len, cs_uint8 d_port);
cs_uint32 __ma_tx_parser_hook_reg(cs_sdl_sw_pkt_tx_f tx_hook);

cs_status epon_request_onu_frame_send_to_pon_no_mutex (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_uint8                 *frame_buf,
    CS_IN cs_uint16                 frame_len,
    CS_IN cs_uint8                  priority);


/********************************************************************************/
/* $rtn_hdr_start  epon_request_onu_frame_send                                  */
/* CATEGORY   : Device                                                          */
/* ACCESS     : Public                                                          */
/* BLOCK      : SAL                                                             */
/* CHIP       : 8030                                                            */
cs_status epon_request_onu_frame_send (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_uint8                 *frame_buf,
    CS_IN cs_uint16                 frame_len,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_uint8                  priority);
/* INPUTS     : o context -- refer to section 3.1, NULL for                     */
/*              synchronous invoking, not NULL for iROS invoking                */
/*              o device -- refer to section 3.3.                               */
/*              o llidport -- llid to identify which                            */
/*              is intended for, -1 for synchronous invoking                    */
/*              o frame_buf -- packet buffer                                    */
/*              o frame_len -- packet length                                    */
/*              o port_id -- destination port id                                */
/*              o priority -- which priority queue packet send from             */
/* OUTPUTS    : o NA                                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,                           */
/*              6 - invalid parameters                                          */
/* DESCRIPTION:                                                                 */
/* ONU packet sending routine                                                   */
/* $rtn_hdr_end                                                                 */
/********************************************************************************/

/********************************************************************************/
/* $rtn_hdr_start  epon_request_onu_pkt_rcv_handler_reg                         */
/* CATEGORY   : Device                                                          */
/* ACCESS     : Public                                                          */
/* BLOCK      : SAL                                                             */
/* CHIP       : 8030                                                            */
cs_status epon_request_onu_pkt_rcv_handler_reg (
    CS_IN   cs_callback_context_t   context,
    CS_IN   cs_int32                device,
    CS_IN   cs_int32                llidport,
    CS_IN   cs_sdl_pkt_rx_f         rx_func);
/* INPUTS     : o context -- refer to section 3.1, NULL for                     */
/*              synchronous invoking, not NULL for iROS invoking                */
/*              o device -- refer to section 3.3.                               */
/*              o llidport -- llid to identify which                            */
/*              is intended for, -1 for synchronous invoking                    */
/*              o rx_func -- handler pointer                                    */
/* OUTPUTS    : o NA                                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,                           */
/*              6 - invalid parameters                                          */
/* DESCRIPTION:                                                                 */
/* Register ONU packet receiving handler                                        */
/* $rtn_hdr_end                                                                 */
/********************************************************************************/

#endif



