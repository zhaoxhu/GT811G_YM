
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
#ifndef __SDL_PORT_H__
#define __SDL_PORT_H__

#include "cs_types.h"
#include "plat_common.h"

typedef enum {
    SDL_PORT_LOOPBACK_NONE  = 0,   /* No loopback */
    SDL_PORT_LOOPBACK_TX2RX = 1,   /* tx2rx */
    SDL_PORT_LOOPBACK_RX2TX = 2,   /* rx2tx  */
} cs_sdl_port_loopback_t;

typedef enum {
    SDL_UNI_MAC_LOOPBACK = 0,   /* MAC loopback */
    SDL_UNI_PHY_LOOPBACK = 1,   /* PHY loopback */
} cs_sdl_uni_loopback_depth_t;

/* UNI port configuration structure      */
typedef enum {
    SDL_PORT_SPEED_CFG_RSV    = 0, /**< Reserved              */
    SDL_PORT_AUTO_10_100      = 1, /**< Auto(10/100Mbps)      */
    SDL_PORT_AUTO_10_100_1000 = 2, /**< Auto(10/100/1000Mbps) */
    SDL_PORT_10_FULL          = 3, /**< 10Mbps(full)          */
    SDL_PORT_10_HALF          = 4, /**< 10Mbps(half)          */
    SDL_PORT_100_FULL         = 5, /**< 100Mbps(full)         */
    SDL_PORT_100_HALF         = 6, /**< 100Mbps(half)         */
    SDL_PORT_1000_FULL        = 7, /**< 1000Mbps(full)        */
    SDL_PORT_1000_HALF        = 8, /**< 1000Mbps(half)        */
} cs_sdl_port_speed_cfg_t;

typedef enum {
    SDL_PORT_SPEED_10 = 0,
    SDL_PORT_SPEED_100,
    SDL_PORT_SPEED_1000,
} cs_sdl_port_ether_speed_t;

typedef enum {
    SDL_PORT_DUPLEX_FULL = 0,
    SDL_PORT_DUPLEX_HALF,
} cs_sdl_port_ether_duplex_t;

/* port auto neg status */
typedef enum {
    SDL_PORT_AUTONEG_DOWN = 0,
    SDL_PORT_AUTONEG_UP,
} cs_sdl_port_autoneg_status_t;

typedef enum {
    SDL_PORT_MDI_AUTO = 0, /**< Auto MDI/MDIX   */
    SDL_PORT_MDI_MDI  = 1, /**< Fixed MDI  */
    SDL_PORT_MDI_MDIX = 2, /**< Fixed MDIX */
} cs_sdl_port_mdi_t;

typedef enum {
    SDL_PORT_ADMIN_DOWN = 0,  /* Admin down */
    SDL_PORT_ADMIN_UP   = 1,  /* Admin up   */
} cs_sdl_port_admin_t;

typedef enum {
    SDL_PORT_LINK_STATUS_DOWN = 0,  /* link down */
    SDL_PORT_LINK_STATUS_UP  = 1,   /* link up  */
} cs_sdl_port_link_status_t;

typedef enum {
    SDL_PORT_NO_DROP = 0,
    SDL_PORT_DROP_DATA = 1,
    SDL_PORT_DROP_CPU = 2,
    SDL_PORT_DROP_ALL = 3
}cs_sdl_port_drop_cfg_t;

typedef struct {
    cs_uint32 rxucfrm_cnt                 ;
    cs_uint32 rxmcfrm_cnt                 ;
    cs_uint32 rxbcfrm_cnt                 ;
    cs_uint32 rxoamfrm_cnt                ;
    cs_uint32 rxpausefrm_cnt              ;
    cs_uint32 rxunknownopfrm_cnt          ;
    cs_uint32 rxcrcerrfrm_cnt             ;
    cs_uint32 rxundersizefrm_cnt          ;
    cs_uint32 rxruntfrm_cnt               ;
    cs_uint32 rxoversizefrm_cnt           ;
    cs_uint32 rxjabberfrm_cnt             ;
    cs_uint32 rxstatsfrm64_cnt            ;
    cs_uint32 rxstatsfrm65_127_cnt        ;
    cs_uint32 rxstatsfrm128_255_cnt       ;
    cs_uint32 rxstatsfrm256_511_cnt       ;
    cs_uint32 rxstatsfrm512_1023_cnt      ;
    cs_uint32 rxstatsfrm1024_1518_cnt     ;
    cs_uint32 rxstatsfrm1519_Max_cnt      ;
    cs_uint64 rxbyte_cnt                  ;
    cs_uint32 rxfrm_cnt                   ;
    cs_uint32 txucfrm_cnt                 ;
    cs_uint32 txmcfrm_cnt                 ;
    cs_uint32 txbcfrm_cnt                 ;
    cs_uint32 txoamfrm_cnt                ;
    cs_uint32 txpausefrm_cnt              ;
    cs_uint32 txcrcerrfrm_cnt             ;
    cs_uint32 txoversizefrm_cnt           ;
    cs_uint32 txsinglecolfrm_cnt          ;
    cs_uint32 txmulticolfrm_cnt           ;
    cs_uint32 txlatecolfrm_cnt            ;
    cs_uint32 txexesscolfrm_cnt           ;
    cs_uint32 txstatsfrm64_cnt            ;
    cs_uint32 txstatsfrm65_127_cnt        ;
    cs_uint32 txstatsfrm128_255_cnt       ;
    cs_uint32 txstatsfrm256_511_cnt       ;
    cs_uint32 txstatsfrm512_1023_cnt      ;
    cs_uint32 txstatsfrm1024_1518_cnt     ;
    cs_uint32 txstatsfrm1519_Max_cnt      ;
    cs_uint64 txbyte_cnt                  ;
    cs_uint32 txfrm_cnt                   ;
    cs_uint32 single_collision            ;
    cs_uint32 multiple_collision          ;
    cs_uint32 fcs_errors                  ;
    cs_uint32 late_collisions             ;
    cs_uint32 excessive_collisions        ;    
}cs_sdl_port_uni_stats_t;


typedef struct{
    cs_boolean enable;
    cs_uint32 rate;
    cs_uint32 cbs; 
    cs_uint32 ebs;  
}cs_sdl_policy_t;

typedef struct{
    cs_boolean en;
    cs_uint8 priority[8];
}cs_pri_remark_t;

typedef enum {
    SDL_RL_MOD_BPS = 0,
    SDL_RL_MOD_PPS = 1,
}cs_sdl_rl_mod_e;


typedef enum {
    SDL_STORM_TYPE_BC          = 0,
    SDL_STORM_TYPE_BC_UMC      = 1,
    SDL_STORM_TYPE_BC_UMC_UUC  = 2,
    SDL_STORM_TYPE_DISABLE     = 3,
}cs_sdl_storm_ctrl_e;

typedef enum {
    CS_DOWN_STREAM     = 0x00,
    CS_UP_STREAM       = 0x01,
} cs_direction_t;


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_mtu_get                */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_mtu_get (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_uint16                *mtu
);
/* INPUTS     : o context -- refer to sectionion 3.3.           */
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/* OUTPUTS    : o mtu -- Maximum Transmission Unit              */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*                             6 - invalid parameters           */
/* DESCRIPTION:                                                 */
/* This function is used to get ONU UNI/PON port mtu            */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_mtu_set                */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_mtu_set (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_uint16                 mtu
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/* OUTPUTS    : o mtu -- Maximum Transmission Unit              */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*                             6 - invalid parameters           */
/* DESCRIPTION:                                                 */
/* This function is used to set ONU UNI/PON port mtu            */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_status_set             */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_status_set (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_sdl_port_speed_cfg_t   speed_cfg
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o speed_cfg -- UNI port speed/duplex/auto-neg   */
/*              0 -- Reserved                                   */
/*              1 -- Auto(10/100Mbps)                           */
/*              2 -- Auto(10/100/1000Mbps)                      */
/*              3 -- 10Mbps(full)                               */
/*              4 -- 10Mbps(half)                               */
/*              5 -- 100Mbps(full)                              */
/*              6 -- 100Mbps(half)                              */
/*              7 -- 1000Mbps(full)                             */
/* OUTPUTS    : o NA                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to set UNI port speed/duplex/auto-neg  */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_status_get             */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_status_get (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_sdl_port_speed_cfg_t  *speed_cfg
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/* OUTPUTS    : o speed_cfg -- UNI port speed/duplex/auto-neg   */
/*              0 -- Reserved                                   */
/*              1 -- Auto(10/100Mbps)                           */
/*              2 -- Auto(10/100/1000Mbps)                      */
/*              3 -- 10Mbps(full)                               */
/*              4 -- 10Mbps(half)                               */
/*              5 -- 100Mbps(full)                              */
/*              6 -- 100Mbps(half)                              */
/*              7 -- 1000Mbps(full)                             */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get ONU UNI port status             */
/* $rtn_hdr_end                                                 */
/****************************************************************/
/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_speed_mdi_set             */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */

cs_status epon_request_onu_port_speed_mdi_set (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_sdl_port_speed_cfg_t   speed_cfg,
    CS_IN cs_sdl_port_mdi_t         mdi
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o speed_cfg -- UNI port speed/duplex/auto-neg   */
/*              0 -- Reserved                                   */
/*              1 -- Auto(10/100Mbps)                           */
/*              2 -- Auto(10/100/1000Mbps)                      */
/*              3 -- 10Mbps(full)                               */
/*              4 -- 10Mbps(half)                               */
/*              5 -- 100Mbps(full)                              */
/*              6 -- 100Mbps(half)                              */
/*              7 -- 1000Mbps(full)                             */
/*              o mdi-- UNI MDI mode, the value please see the  */
/*              description. The valid value is 0~2.            */
/*              0 -- Auto                                       */
/*              1 -- MDI                                        */
/*              2 -- MDI-X                                      */
/* OUTPUTS    : o NA                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to set UNI port speed/duplex/auto-neg  */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_autoneg_restart        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_autoneg_restart (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to restart ONU auto-neg process        */
/* $rtn_hdr_end                                                 */
/****************************************************************/


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_speed_get              */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_speed_get (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_port_ether_speed_t    *speed
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/* OUTPUTS    :                                                 */
/*              o speed -- UNI port speed                       */
/*              0 -- Auto                                       */
/*              1 -- 10Mbps                                     */
/*              2 -- 100Mbps                                    */
/*              3 -- 1000Mbps                                   */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get UNI port speed                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_duplex_get             */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_duplex_get (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_port_ether_duplex_t   *duplex
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/* OUTPUTS    :                                                 */
/*              o duplex -- UNI port duplex                     */
/*              0 -- Auto                                       */
/*              1 -- FULL duplex                                */
/*              2 -- HALF duplex                                */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get UNI port duplex                 */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_auto_neg_get           */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_auto_neg_get (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_port_autoneg_status_t *auto_neg
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/* OUTPUTS    :                                                 */
/*              o auto_neg -- UNI port auto-neg                 */
/*              0 -- Disable auto-negotiation                   */
/*              1 -- Enable auto-negotiation                    */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get UNI port auto-neg               */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_mdi_set                */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_mdi_set (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_sdl_port_mdi_t         mdi
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which            */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o mdi-- UNI MDI mode, the value please see the  */
/*              description. The valid value is 0~2.            */
/*              0 -- Auto                                       */
/*              1 -- MDI                                        */
/*              2 -- MDI-X                                      */
/* OUTPUTS    : o NA                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/*  This function is used to set UNI mdi mode.                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_mdi_get                */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_mdi_get (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_sdl_port_mdi_t        *mdi
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which            */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o mdi-- MDI mode, the value please see the      */
/*              description. The valid value is 0~2.            */
/*              0 -- Auto                                       */
/*              1 -- MDI                                        */
/*              2 -- MDI-X                                      */
/* OUTPUTS    : o NA                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/*  This function is used to get UNI mdi mode.                  */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_admin_set              */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_admin_set (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_sdl_port_admin_t       admin
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o admin -- admin status                         */
/*              0 -- Down                                       */
/*              1 -- Up                                         */
/* OUTPUTS    : o NA                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to set UNI port admin state            */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_admin_get              */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_admin_get (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_sdl_port_admin_t      *admin
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/* OUTPUTS    : o admin -- admin status                         */
/*              0 -- Down                                       */
/*              1 -- Up                                         */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*                             6 - invalid parameters           */
/* DESCRIPTION:                                                 */
/* This function is used to get UNI port admin state            */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_stats_get              */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_stats_get (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_boolean                read_clear,
    CS_OUT cs_sdl_port_uni_stats_t  *uni_stats
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o read_clear -- read clear or not               */
/*                  0: Not clear                                */
/*                  1: Clear                                    */
/* OUTPUTS    : o uni_stats -- UNI interface standard statistics*/
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get UNI port statistics             */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_stats_get_pwrsv        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_stats_get_pwrsv (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_boolean                read_clear,
    CS_OUT cs_sdl_port_uni_stats_t  *uni_stats
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o read_clear -- read clear or not               */
/*                  0: Not clear                                */
/*                  1: Clear                                    */
/* OUTPUTS    : o uni_stats -- UNI interface standard statistics*/
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get UNI port statistics             */
/* $rtn_hdr_end                                                 */
/****************************************************************/


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_stats_clr              */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_stats_clr (
    CS_IN cs_callback_context_t             context,
    CS_IN cs_int32                          device_id,
    CS_IN cs_int32                          llidport,
    CS_IN cs_port_id_t                      port_id
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/* OUTPUTS    : o  --                         */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to clear UNI port statistics           */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_lpbk_set               */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_lpbk_set(
    CS_IN cs_callback_context_t             context,
    CS_IN cs_int32                          device_id,
    CS_IN cs_int32                          llidport,
    CS_IN cs_port_id_t                      port_id,
    CS_IN cs_sdl_port_loopback_t            loopback
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o loopback - loopback status,                   */
/*              Includes below value:                           */
/*              0 -- No loopback                                */
/*              1 -- UNI tx2rx (local) loopback                 */
/*              2 -- UNI rx2tx (remote)  loopback               */
/* OUTPUTS    : o NA                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to set UNI port loopback               */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_lpbk_get               */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_lpbk_get(
    CS_IN cs_callback_context_t             context,
    CS_IN cs_int32                          device_id,
    CS_IN cs_int32                          llidport,
    CS_IN cs_port_id_t                      port_id,
    CS_OUT cs_sdl_port_loopback_t           *loopback
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o loopback - loopback status,                   */
/*              Includes below value:                           */
/*              0 -- No loopback                                */
/*              1 -- UNI tx2rx (local) loopback                 */
/*              2 -- UNI rx2tx (remote)  loopback               */
/* OUTPUTS    : o NA                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get UNI port loopback               */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_link_status_get        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_link_status_get (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_port_link_status_t    *link_status
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device_id -- refer to section 3.3.            */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o link_status - Link status                     */
/*                  0 -- Link down                              */
/*                  1 -- Link up                                */
/* OUTPUTS    : o NA                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get UNI port link status            */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_flow_ctrl_set          */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_flow_ctrl_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_boolean                enable
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which            */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o enable -- 0-disable, 1-enable                 */
/* OUTPUTS    : o NA                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to set flow control status             */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_flow_ctrl_get          */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_flow_ctrl_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_boolean               *enable
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which            */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o enable -- 0-disable, 1-enable                 */
/* OUTPUTS    : o NA                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get flow control status             */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_traffic_set                        */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SDL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_traffic_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,        
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_sdl_port_drop_cfg_t    is_drop
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o port_id --  port id, the range is 1 for UNI port          */
/*                                     and 0 for PON port                   */
/*              o is_drop -- 1- drop the packets to PON/UNI from this port  */
/*                          exclude to CPU, 0- forward all pkts             */
/* OUTPUTS    : o NA                                                        */
/* DESCRIPTION:                                                             */
/* This function is used to block the ingress traffic                       */
/* which its destination port is PON/UNI  on the port.                      */
/*                                                                          */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_traffic_set                        */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SDL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_traffic_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,        
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_sdl_port_drop_cfg_t    *is_drop
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o port_id --  port id, the range is 1 for UNI port          */
/*                                     and 0 for PON port                   */
/* OUTPUTS      o is_drop -- 1- drop the packets to PON/UNI from this port  */
/*                          exclude to CPU, 0- forward all pkts             */
/* DESCRIPTION:                                                             */
/* This function is used to block the ingress traffic                       */
/* which its destination port is PON/UNI  on the port.                      */
/*                                                                          */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_hello_mac_set                      */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SDL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_hello_mac_set(
    CS_IN cs_callback_context_t      context,
    CS_IN cs_int32                   device_id,
    CS_IN cs_int32                   llidport,        
    CS_IN cs_port_id_t               port_id,
    CS_IN cs_mac_t                  *mac
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o port_id --  port id, the range is 1 for UNI port          */
/*                                     and 0 for PON port                   */
/*              o mac --hello mac address                                   */
/* OUTPUTS    : o NA                                                        */
/* DESCRIPTION:                                                             */
/* This function is used to set hello mac on port                           */
/*                                                                          */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_hello_mac_get          */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_hello_mac_get(
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_int32                   device_id,
    CS_IN  cs_int32                 llidport,
    CS_IN  cs_port_id_t              port_id,
    CS_OUT cs_mac_t                   *mac
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which            */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/* OUTPUTS    : o mac  hello mac address                        */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get hello mac from port             */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_ds_rate_limit_set                  */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SAL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_ds_rate_limit_set(
        CS_IN cs_callback_context_t     context,
        CS_IN cs_int32                device_id,
        CS_IN cs_int32                llidport,
        CS_IN cs_port_id_t              port_id,
        CS_IN cs_sdl_policy_t           *rate
    );
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o port_id --  port id, the range is 1 for single port onu   */
/*                                     and is 1~4 for 4 port onu            */
/*              o rate->enable -- 1- enable policy, 0- disable policy       */
/*              o rate->rate -- egress rate for this port.the rang is       */
/*                                   0~1000000, Unit of 1kbps               */
/*              o rate->cbs -- commit burst size, the rang is 0~1000,       */
/*                                uint of 1  byte                           */
/* OUTPUTS    : o NA                                                        */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* Configure egress rate of each port. Unit of 1kbps,                       */ 
/* This configuration is used to implement CTC port downstream rate limit   */
/* for each port                                                            */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_ds_rate_limit_get                  */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SAL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_ds_rate_limit_get(
        CS_IN cs_callback_context_t     context,
        CS_IN cs_int32                device_id,
        CS_IN cs_int32                llidport,
        CS_IN cs_port_id_t              port_id,
        CS_OUT cs_sdl_policy_t           *rate
    );
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o port_id --  port id, the range is 1 for single port onu   */
/*                                     and is 1~4 for 4 port onu            */
/* OUTPUTS    : o rate->enable -- 1- enable policy, 0- disable policy       */
/*              o rate->rate -- egress rate for this port.the rang is       */
/*                                   0~1000000, Unit of 1kbps               */
/*              o rate->cbs -- commit burst size, the rang is 0~1000,       */
/*                                uint of 1k byte                           */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* Configure egress rate of each port. Unit of 1kbps,                       */ 
/* This configuration is used to implement CTC port downstream rate limit   */
/* for each port                                                            */
/* $rtn_hdr_end                                                             */
/****************************************************************************/



/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_policy_set                         */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SAL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_policy_set(
        CS_IN cs_callback_context_t     context,
        CS_IN cs_int32                device_id,
        CS_IN cs_int32                llidport,
        CS_IN cs_port_id_t              port_id,
        CS_IN cs_sdl_policy_t           *policy
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o port_id --  port id, the range is 1 for single port onu   */
/*                                     and is 1~4 for 4 port onu            */
/*              o policy->enable -- 1- enable policy, 0- disable policy     */
/*              o policy->rate -- ingress rate for this port.the rang is    */
/*                                   0~1000000, Unit of 1kbps               */
/*              o policy->cbs -- commit burst size, the rang is 0~1000,     */
/*                                uint of 1k byte                           */
/* OUTPUTS    : o NA                                                        */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* Configure ingress rate of each port. Unit of 1kbps,                      */ 
/* This configuration is used to implement CTC port policy function         */
/* for each port                                                            */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_policy_get                         */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SAL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_policy_get(
        CS_IN cs_callback_context_t     context,
        CS_IN cs_int32                device_id,
        CS_IN cs_int32                llidport,
        CS_IN cs_port_id_t              port_id,
        CS_OUT cs_sdl_policy_t           *policy
    );
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o port_id --  port id, the range is 1 for single port onu   */
/*                                     and is 1~4 for 4 port onu            */
/* OUTPUTS    : o policy->enable -- 1- enable policy, 0- disable policy     */
/*              o policy->rate -- ingress rate for this port.the rang is    */
/*                                   0~1000000, Unit of 1kbps               */
/*              o policy->cbs -- commit burst size, the rang is 0~1000,     */
/*                                unit of 1k byte                           */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* Configure ingress rate of each port. Unit of 1kbps,                      */ 
/* This configuration is used to implement CTC port policy function         */
/* for each port                                                            */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_storm_ctrl_set                  */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SAL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_storm_ctrl_set(
        CS_IN cs_callback_context_t     context,
        CS_IN cs_int32                device_id,
        CS_IN cs_int32                llidport,
        CS_IN cs_port_id_t              port_id,                
        CS_IN cs_sdl_rl_mod_e           storm_mod,        
        CS_IN cs_sdl_storm_ctrl_e       storm_type,
        CS_IN cs_sdl_policy_t           *rate
    );
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o port_id --  port id, the range is 1 for single port onu   */
/*                                     and is 1~4 for 4 port onu            */
/*              o storm_mod -- storm control mode                           */
/*                              0 - bit per second(bps)                     */
/*                              1 - packet per second(pps)                  */
/*              o storm_type -- packet type for storm control               */
/*                              0 - storm control for broadcast packet      */
/*                              1 - storm control for BC+UMC packet         */
/*                              2 - storm control for BC+UMC+UUC packet     */
/*              o rate->enable -- 1- enable policy, 0- disable policy       */
/*              o rate->rate -- egress rate for this port.the rang is       */
/*                                   0~1000000, Unit of 1kbps               */
/*              o rate->cbs -- commit burst size, the rang is 0~1000,       */
/*                                uint of 1k byte                           */
/* OUTPUTS    : o NA                                                        */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* Configure flooding stream rate of each port. Unit of 1kbps,              */ 
/* This configuration is used to implement storm control                    */
/* for each port                                                            */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_storm_ctrl_get                  */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SAL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_storm_ctrl_get(
        CS_IN cs_callback_context_t     context,
        CS_IN cs_int32                device_id,
        CS_IN cs_int32                llidport,
        CS_IN cs_port_id_t              port_id,                
        CS_OUT cs_sdl_rl_mod_e           *storm_mod,        
        CS_OUT cs_sdl_storm_ctrl_e       *storm_type,
        CS_OUT cs_sdl_policy_t           *rate
    );
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o port_id --  port id, the range is 1 for single port onu   */
/*                                     and is 1~4 for 4 port onu            */
/* OUTPUTS    : o storm_mod -- storm control mode                           */
/*                              0 - bit per second(bps)                     */
/*                              1 - packet per second(pps)                  */
/*              o storm_type -- packet type for storm control               */
/*                              0 - storm control for broadcast packet      */
/*                              1 - storm control for BC+UMC packet         */
/*                              2 - storm control for BC+UMC+UUC packet     */
/*              o rate->enable -- 1- enable policy, 0- disable policy       */
/*              o rate->rate -- egress rate for this port.the rang is       */
/*                                   0~1000000, Unit of 1kbps               */
/*              o rate->cbs -- commit burst size, the rang is 0~1000,       */
/*                                uint of 1k byte                           */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* Configure flooding traffic rate of each port. Unit of 1kbps,             */ 
/* This configuration is used to implement storm control                    */
/* for each port                                                            */
/* $rtn_hdr_end                                                             */
/****************************************************************************/


/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_eee_set                            */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SAL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_eee_set(
    CS_IN cs_callback_context_t      context,
    CS_IN cs_int32                   device_id,
    CS_IN cs_int32                   llidport,        
    CS_IN cs_port_id_t               port_id,
    CS_IN cs_boolean                 enable
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o port_id --  port id, the range is 1 for single port onu   */
/*                                     and is 1~4 for 4 port onu            */
/*              o enable -- 1- enable                                       */
/*                          0- disable                                      */
/* OUTPUTS    : o NA                                                        */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* Configure IEEE802.3az EEE function at UNI                                */ 
/* $rtn_hdr_end                                                             */
/****************************************************************************/


/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_eee_get                            */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SAL                                                         */
/* CHIP       : 8030                                                        */

cs_status epon_request_onu_port_eee_get(
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_int32                  device_id,
    CS_IN  cs_int32                  llidport,
    CS_IN  cs_port_id_t              port_id,
    CS_OUT cs_boolean               *enable
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o port_id --  port id, the range is 1 for single port onu   */
/*                                     and is 1~4 for 4 port onu            */
/*              o enable -- 1- enable                                       */
/*                          0- disable                                      */
/* OUTPUTS    : o enable -- retive EEE enable configuration                                                       */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* Retrive the configuration of IEEE802.3az EEE function at UNI                                */ 
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_isolation_set                      */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SAL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_isolation_set(
        CS_IN cs_callback_context_t     context,
        CS_IN cs_int32                  device_id,
        CS_IN cs_int32                  llidport,
        CS_IN cs_boolean                enable                
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o enable -- 1- enable isolation for UNI ports               */
/*                          0- disable isolation                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* Configure isolation for UNI ports.                                       */
/* If enabled, traffic from UNI port to UNI port shall not be passed        */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_isolation_get                      */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SAL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_isolation_get(
        CS_IN cs_callback_context_t     context,
        CS_IN cs_int32                  device_id,
        CS_IN cs_int32                  llidport,
        CS_OUT cs_boolean               *enable                
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/* OUTPUTS    : o                                                           */
/*              o enable -- 1- enable isolation for UNI ports               */
/*                          0- disable isolation                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* Get isolation config for UNI ports.                                      */
/* If enabled, traffic from UNI port to UNI port shall not be passed        */
/* $rtn_hdr_end                                                             */
/****************************************************************************/


cs_status epon_request_onu_port_mirror_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              mirror_port, 
    CS_IN cs_uint32                 rx_port_msk, 
    CS_IN cs_uint32                 tx_port_msk
);

cs_status epon_request_onu_port_mirror_get(
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_int32                  device_id,
    CS_IN  cs_int32                  llidport,
    CS_OUT cs_port_id_t              *mirror_port, 
    CS_OUT cs_uint32                 *rx_port_msk, 
    CS_OUT cs_uint32                 *tx_port_msk
);


/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_big_burst_size_set                      */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SDL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_big_burst_size_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                enable
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o enable -- 1- enable big burst size(default setting),      */
/*                          0- disable big burst size                       */
/* OUTPUTS    : o NA                                                        */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* for TCP throughput under ingress port rate limit work-around.            */ 
/* by default, this function is turned on, and it can be turned on by WEB   */
/* for global ONU                                                           */
/* $rtn_hdr_end                                                             */
/****************************************************************************/



/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_big_burst_size_get                      */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SDL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_big_burst_size_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                *enable
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/* OUTPUTS    : o enable -- 1-big burst size, 0-normal setting              */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* for TCP throughput under ingress port rate limit work-around.            */ 
/* by default, this function is turned on, and it can be turned on by WEB   */
/* for global ONU                                                           */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

cs_status epon_request_onu_pkt_remote_capture_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean              enable
);

cs_status epon_request_onu_pkt_remote_capture_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean              *enable
);

cs_status epon_request_onu_traffic_block(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_direction_t            dirction,
    CS_IN cs_boolean                is_block
);

cs_status epon_request_onu_phy_loopback_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                enable
);


#endif


