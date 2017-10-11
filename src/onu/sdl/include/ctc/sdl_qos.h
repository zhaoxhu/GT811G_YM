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
#ifndef _SDL_QOS_CTC_H_
#define _SDL_QOS_CTC_H_
#include "cs_types.h"
#include "plat_common.h"


#define MAX_PRI_NUM 8

#define US_MAX_QUE_BUFFER_SIZE 384
#define DS_MAX_QUE_BUFFER_SIZE 230


typedef struct{
    cs_uint8 cos[MAX_PRI_NUM];
}cs_pri_cos_map_t;


typedef struct{
    cs_uint8 pri[MAX_PRI_NUM];
}cs_tos_pri_map_t;

typedef struct{
    cs_uint32 max_buf_abi;
    cs_uint32 min_buf_abi;    
}cs_buf_abi_t;


typedef enum{
    SDL_SCHD_SP_MODE,
    SDL_SCHD_WRR_MODE,
}cs_sdl_sched_type_t;

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_dot1p_map_set                           */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SDL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_dot1p_map_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_pri_cos_map_t          *queue
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o que->cos -- mapping relationship with priority.           */
/*              Each number shall be in the range 0~7.                      */
/* OUTPUTS    : o NA                                                        */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* This function is used to set the ONU priority mapping with queues.       */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_dot1p_map_get                           */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SDL                                                         */
/* CHIP       : 8030                                                        */

cs_status epon_request_onu_dot1p_map_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_pri_cos_map_t         *queue
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/* OUTPUTS      o que->cos -- mapping relationship with priority.           */
/*              Each number shall be in the range 0~7.                      */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* This function is used to get the ONU priority mapping with queues.       */
/* value.                                                                   */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_tos_map_set                             */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SDL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_tos_map_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_tos_pri_map_t          *tos_map
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o tos_map->pri -  mapping relationship with priority.       */
/*              Each mapping value shall be in range 0~7                    */
/* OUTPUTS    : o NA                                                        */
/* DESCRIPTION:                                                             */
/* This function is used to set the ONU priority mapping to ToS value.      */
/*                                                                          */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_tos_map_get                             */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SDL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_tos_map_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_tos_pri_map_t         *tos_map
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/* OUTPUTS    : o tos_map->pri -  mapping relationship with priority.       */
/*              Each mapping value shall be in range 0~7                    */
/* DESCRIPTION:                                                             */
/* This function is used to get the ONU priority mapping to ToS value.      */
/*                                                                          */
/* $rtn_hdr_end                                                             */
/****************************************************************************/





/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_que_size_set        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_que_size_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_uint8                  que,
    CS_IN cs_uint16                 size
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o port_id -- 0 for downstream, 1-for upstream               */
/*              o que -- que id.                                            */
/*              o size -- queue size, unit 1kB = 1024B                      */
/*              Each number shall be in the range 0~3.                      */
/* OUTPUTS    : o NA                                                        */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* This function is used to set the ONU  queue's size.                      */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_que_size_get        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_que_size_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_uint8                  que,
    CS_OUT cs_uint16                 *size
);

/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o port_id -- 0 for downstream, 1-for upstream               */
/*              o que -- que id.                                            */
/* OUTPUTS      o size -- queue size, unit 1kB = 1024B                      */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* This function is used to get the ONU  queue's size.                      */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_schedule_mode_set           */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_schedule_mode_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_sdl_sched_type_t       sche_mode
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/*              o sche_mode -- 0 sp mode, 1-wrr mode, weight = 2            */
/* OUTPUTS      NA                                                          */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* This function is used to set the ONU scheduling mode.                    */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_schedule_mode_get           */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_schedule_mode_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_OUT cs_sdl_sched_type_t      *sche_mode
);
/* INPUTS     : o context -- refer to section 3.1, NULL for                 */
/*              synchronous invoking, not NULL for iROS invoking            */
/*              o device -- refer to section 3.3.                           */
/*              o llidport -- llid to identify which                        */
/*              is intended for, -1 for synchronous invoking                */
/* OUTPUTS      o sche_mode -- 0 sp mode, 1-wrr mode, weight = 2            */
/* RETURNS    : o result code, 0 - success, 1 - fail,                       */
/*              6 - invalid parameters                                      */
/* DESCRIPTION:                                                             */
/* This function is used to get the ONU scheduling mode.                    */
/* $rtn_hdr_end                                                             */
/****************************************************************************/

cs_status epon_request_onu_port_buf_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_uint32                 size
);

cs_status epon_request_onu_port_buf_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_uint32                 *size
);

cs_status epon_request_onu_port_buf_ability_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_buf_abi_t                 *abi
);

#endif
