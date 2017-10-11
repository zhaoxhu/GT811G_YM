
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
#ifndef __SDL_FDB_H__
#define __SDL_FDB_H__

#include "cs_types.h"
#include "plat_common.h"

#define __FDB_NO_MAC_LIMIT                                        (0xffff)
#define __FDB_AGING_TIME_DEFAULT                            (300)

typedef enum {
    SDL_FDB_ENTRY_DYNAMIC       = 0,  
    SDL_FDB_ENTRY_STATIC        = 1 ,
    SDL_FDB_ENTRY_MC_TYPE       =2
} cs_sdl_fdb_entry_type_t;

typedef enum {
    SDL_FDB_ENTRY_GET_MODE_DYNAMIC       = 0,  
    SDL_FDB_ENTRY_GET_MODE_STATIC        = 1, 
    SDL_FDB_ENTRY_GET_MODE_ALL           = 2 
} cs_sdl_fdb_entry_get_mode_t;

typedef enum {
    SDL_FDB_FULL_PASS_NOLEARN   = 0,    /* pass and do not learn  */
    SDL_FDB_FULL_DROP_NOLEARN   = 1,    /* drop and do not learn  */
    SDL_FDB_FULL_PASS_LRU       = 2,    /* Least recent usage algorithm */
} cs_sdl_fdb_full_behavior_t;

typedef enum {
    SDL_FDB_MAC_LEARN_DISABLE   = 0,    /* disable MAC learn  */
    SDL_FDB_MAC_LEARN_ENABLE    = 1,    /* enable MAC learn  */
} cs_sdl_fdb_mac_learn_t;

typedef enum {
    SDL_FDB_CLR_BOTH    = 0,       /* Clear all FDB entry  */
    SDL_FDB_CLR_DYNAMIC = 1,       /* Clear dynamic FDB entry  */
    SDL_FDB_CLR_STATIC  = 2        /* Clear static FDB entry  */
} cs_sdl_fdb_clr_mode_t;

typedef enum {
    SDL_FDB_SHARED_VLAN_LEARNING        = 0,    /*Don't learn VLAN when learing MAC*/
    SDL_FDB_INDEPENDENT_VLAN_LEARNING   = 1     /*Learn VLAN when learing MAC*/
} cs_sdl_fdb_learn_mode_t;

typedef struct
{
    cs_port_id_t               port;
    cs_mac_t                   mac;
    cs_uint16                  vlan_id;        
    cs_sdl_fdb_entry_type_t    type;      
} cs_sdl_fdb_entry_t;

typedef struct {
    cs_uint8  mac_limit;
    cs_uint16 vlan_id;              /* vlan id */
} cs_sdl_fdb_vlan_limit_entry_t;


typedef struct{
    cs_uint32 mac_limit;
    cs_uint32 static_mac_num;
    cs_sdl_fdb_mac_learn_t  lrn_en;
    cs_sdl_fdb_learn_mode_t lrn_mod;
    cs_sdl_fdb_full_behavior_t full_mod;
}sdl_fdb_port_cfg_t;

typedef enum {
    SA_BLOCK = 1 << 0,
    DA_BLOCK = 1 << 1,
    SA_DA_BOTH_BLOCK = SA_BLOCK | DA_BLOCK,
    SDL_MAC_BLOCK_TYPE_END = SA_DA_BOTH_BLOCK+1
} sdl_mac_block_type_t;

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_fdb_full_behavior_set       */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_fdb_full_behavior_set (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_sdl_fdb_full_behavior_t    full_behavior
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id - port id                             */
/*              o full_behavior -- the behavior configuration.  */
/*             0:  Drop and not learn;                          */
/*             1:  Pass and not learn                           */
/*             2:pass and learn(replace one of the oldest one.) */
/* OUTPUTS    : o NA                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to set packet learn and traffic        */
/* behavior when the FDB table is full.                         */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_fdb_full_behavior_get       */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_fdb_full_behavior_get (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_fdb_full_behavior_t   *full_behavior
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which ONU        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id - port id                             */
/* OUTPUTS    : o full_behavior -- the behavior configuration.  */
/*             0:Drop and not learn;                            */
/*             1:Pass and not learn                             */
/*             2:pass and learn(replace one of the oldest one.) */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*                             6 - invalid parameters           */
/* DESCRIPTION:                                                 */
/* This function is used to get ONU MAC table full behavior     */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_fdb_learn_mode_set          */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status
epon_request_onu_fdb_learn_mode_set (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_sdl_fdb_learn_mode_t       lrn_mode
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which onu        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o lrn_mode -- MAC learning mode                 */
/*              0: Not learn VLAN when learning MAC;            */
/*              1: Learn VLAN when learning MAC                 */
/* OUTPUTS    : o                                               */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to set UNI port learning mode          */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_fdb_learn_mode_get          */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status
epon_request_onu_fdb_learn_mode_get (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_fdb_learn_mode_t      *lrn_mode
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which onu        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/* OUTPUTS    :                                                 */
/*              o lrn_mode -- MAC learning mode                 */
/*              0: Don't learn VLAN when learning MAC;          */
/*              1: Learn VLAN when learning MAC                 */
/* OUTPUTS    : o                                               */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get UNI port learning mode          */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_fdb_mac_limit_set           */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status
epon_request_onu_fdb_mac_limit_set (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_uint32                     mac_num
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which onu        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o mac_num -- the max learn number [0,64]        */
/* OUTPUTS    : o                                               */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to set the max MAC  number             */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_fdb_mac_limit_get           */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status
epon_request_onu_fdb_mac_limit_get (
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_uint32                    *mac_num
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which onu        */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o mac_num -- the max learn number [0,64]        */
/* OUTPUTS    : o                                               */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get the max MAC number              */
/* $rtn_hdr_end                                                 */
/****************************************************************/


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_fdb_age_set                 */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status
epon_request_onu_fdb_age_set (
    CS_IN cs_callback_context_t          context,
    CS_IN cs_int32                       device_id,
    CS_IN cs_int32                       llidport,
    CS_IN cs_uint32                      aging_time
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which onu        */
/*              is intended for, -1 for synchronous invoking    */
/*              o aging_time -- aging time, unit is second      */
/*                  0: Disable aging                            */
/*                  1s-10000000s: The aging time value          */
/* OUTPUTS    : o                                               */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to set aging time                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_fdb_age_get                 */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status
epon_request_onu_fdb_age_get (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_OUT cs_uint32                *aging_time
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which onu        */
/*              is intended for, -1 for synchronous invoking    */
/* OUTPUTS    :                                                 */
/*              o aging_time -- aging time, unit is second      */
/*                  0: Disable aging                            */
/*                  1-10000000: The aging time value            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get aging time                      */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_mac_learn_set               */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_mac_learn_set(
    CS_IN cs_callback_context_t   context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_port_id_t            port_id,
    CS_IN cs_sdl_fdb_mac_learn_t  status
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which            */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o status -- enable/disable MAC learning         */
/*              0x0  disabled                                   */
/*              0x1  enabled                                    */
/* OUTPUTS    : o NA                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to enable/disable MAC learning         */
/* on UNI port.                                                 */
/* $rtn_hdr_end                                                 */
/****************************************************************/

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_mac_learn_get               */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SDL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_mac_learn_get(
    CS_IN cs_callback_context_t   context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_port_id_t            port_id,
    CS_OUT cs_sdl_fdb_mac_learn_t *status
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which            */
/*              is intended for, -1 for synchronous invoking    */
/*              o port_id -- port id                            */
/*              o status -- enable/disable MAC learning.        */
/*              0x0  disabled                                   */
/*              0x1  enabled                                    */
/* OUTPUTS    : o NA                                            */
/* RETURNS    : o result code, 0 - success, 1 - fail,           */
/*              6 - invalid parameters                          */
/* DESCRIPTION:                                                 */
/* This function is used to get UNI port MAC learning status    */
/* $rtn_hdr_end                                                 */
/****************************************************************/


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_fdb_entry_clr               */
/* CATEGORY   :   Device                                        */
/* ACCESS     :   Public                                        */
/* BLOCK      :   SDL                                           */
/* CHIP       :   CS8030                                        */
cs_status epon_request_onu_fdb_entry_clr (
    CS_IN   cs_callback_context_t context,
    CS_IN   cs_int32              device_id,
    CS_IN   cs_int32              llidport,
    CS_IN   cs_sdl_fdb_clr_mode_t clr_mode
);
/* INPUTS     : o context -- refer to section 3.1, NULL for     */
/*              synchronous invoking, not NULL for iROS invoking*/
/*              o device -- refer to section 3.3.               */
/*              o llidport -- llid to identify which onu        */
/*              o clr_mode - Clear mode                         */
/*                  0,   all FDB entry                          */
/*                  1,   dynamic FDB entry                      */
/*                  2,   static FDB entry                       */
/* OUTPUTS  : -------                                           */
/* RETURNS  : CS_E_OK: Set successfully;                        */
/*                   CS_E_ERROR: Set failed                     */
/* DESCRIPTION:                                                 */
/* This function is used to clear FDB table                     */
/* $rtn_hdr_end                                                 */
/****************************************************************/


cs_status epon_request_onu_fdb_entry_add(
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_sdl_fdb_entry_t            *entry
);

cs_status epon_request_onu_fdb_entry_del(
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_mac_t                      *mac,
    CS_IN  cs_uint16                     vlan
); 

cs_status epon_request_onu_fdb_entry_get(
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_mac_t                      *mac,                  
    CS_IN  cs_uint16                     vlan,
    CS_OUT cs_sdl_fdb_entry_t            *entry
);

cs_status epon_request_onu_fdb_entry_getnext( 
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_sdl_fdb_entry_get_mode_t   mode,
    CS_IN  cs_uint16                     offset,
    CS_OUT cs_uint16                     *next,
    CS_OUT cs_sdl_fdb_entry_t            *entry
);

cs_status epon_request_onu_fdb_entry_clr_per_port (
    CS_IN   cs_callback_context_t   context,
    CS_IN   cs_int32                device_id,
    CS_IN   cs_int32                llidport,
    CS_IN   cs_sdl_fdb_clr_mode_t   clr_mode,
    CS_IN   cs_port_id_t            portid
);

cs_status epon_request_onu_fdb_entry_getnext_per_port( 
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_port_id_t                  port_id,
    CS_IN  cs_sdl_fdb_entry_get_mode_t   mode,
    CS_IN  cs_uint16                     offset,
    CS_OUT cs_uint16                     *next,
    CS_OUT cs_sdl_fdb_entry_t            *entry
);

#if 1
//addec by zhuxh
cs_status epon_request_onu_fdb_entry_get_byindex( 
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_sdl_fdb_entry_get_mode_t   mode,
    CS_IN  cs_uint16                     offset,
    CS_OUT cs_sdl_fdb_entry_t            *entry,
    CS_OUT cs_uint16                     *next
);
int fdb_static_list_add(char *mac, int port, int vlan_id);
int fdb_static_list_del(char *mac, int vlan_id);
#endif

#ifdef HAVE_USER_MAC_TRACE
cs_status epon_request_onu_fdb_port_get(
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_mac_t                      *mac,                  
    CS_OUT cs_port_id_t					 *port
);

cs_status 
onu_mac_locate(
		CS_IN		cs_uint8 	*mac, 
		CS_OUT		cs_uint8	*onuport
		);

cs_status
locateUserMac(
		CS_IN	cs_uint8	*mac, 
		CS_OUT	cs_uint8	*onuslot, 
		CS_OUT	cs_uint8	*onuport, 
		CS_OUT	cs_uint8	*subsw, 
		CS_OUT	cs_uint8	*sw_mac,
		CS_OUT	cs_uint32	*sw_port
		);
#endif	/*(#ifdef HAVE_USER_MAC_TRACE)*/

#ifdef HAVE_ONU_PORT_MAC_COUNT
cs_status mac_num_hw_to_sw_init(void);
void mac_num_update_hw_to_sw(void);
cs_status port_mac_count_get(cs_port_id_t port_id, cs_uint32 *mac_count);
#endif	/*(#ifdef HAVE_ONU_PORT_MAC_COUNT)*/

cs_status epon_request_onu_fdb_block_entry_add(
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_int32                  device_id,
    CS_IN  cs_int32                  llidport,
    CS_IN  cs_sdl_fdb_entry_t *entry,
    CS_IN  sdl_mac_block_type_t      type);

cs_status epon_request_onu_fdb_block_entry_del(
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_mac_t                      *mac,
    CS_IN  cs_uint16                     vlan
);

cs_status epon_request_onu_fdb_block_entry_get(
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_mac_t                      *mac,                  
    CS_IN  cs_uint16                     vlan,
    CS_OUT cs_sdl_fdb_entry_t            *entry,
    CS_OUT sdl_mac_block_type_t      *type
);

cs_status epon_request_onu_fdb_block_entry_getnext( 
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_uint16                     offset,
    CS_OUT cs_uint16                     *next,
    CS_OUT cs_sdl_fdb_entry_t            *entry,
    CS_OUT sdl_mac_block_type_t *type
);

#endif

