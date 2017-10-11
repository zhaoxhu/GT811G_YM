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

Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/
/*
 * sdL_vlan_util.h
 *
 *
 */

#ifndef __SDL_VLAN_UTIL_H__
#define __SDL_VLAN_UTIL_H__


#include "cs_types.h"

#define SDL_TPID_8100       0x8100
#define SDL_TPID_88a8       0x88a8
#define SDL_TPID_9100       0x9100
#define SDL_TPID_8808       0x8808
#define SDL_TPID_8809       0x8809
#define SDL_TPID_888e       0x888e

#define SDL_VLAN_MAX_ID      4095

#define SDL_VLAN_ENGINE_MAX  32

// vlan ID check table
#define SDL_VLAN_TABLE_MAX_ENTRY    8
#define SDL_VLAN_TABLE_START_ENTRY  9  
#define SDL_VLAN_KEY_NUM_ENTRY      8


/*  vlan rule entry resource manager */
// pri: 0 lowest

/* Normal VLAN, VLAN domain  */
/* 0 reserve;                */
/* 1 DEF(vlan control);      */
/* 2 VLAN TABLE rule start;  */
/*****************************/
#define SDL_VLAN_TBL_START              2  // both up&ds,

#define SDL_VLAN_DEF_RULE               1  // both up&ds
#ifdef HAVE_SDL_NTT
#define SDL_VLAN_SPEC_RULE_0            0  // for arp
#define SDL_VLAN_SPEC_RULE_1            2
#define SDL_VLAN_SPEC_DA_RULE_START     3 // 3,4,5,6
#define SDL_VLAN_SPEC_RULE_2            7
#define SDL_PVLAN_FILTER_RULE           8 // NTT filter(Downstream), PVID VLAN rule
//#define SDL_VLAN_SPEC_RULE_3           17
#define SDL_VLAN_SPEC_RULE_4           18
#define SDL_VLAN_SPEC_UP_RULE           5
#define SDL_VLAN_SPEC_UP_DA_RULE_START  6 //6,7,8,9
#endif
/*==============================================================*/
/* Default rule in entry SDL_VLAN_DEF_RULE */
/* key_type is vlankey,8 keys in one entry */
/* include: unknow vlan control,p_tag(VLAN 0) vlan control,     */
/*          pvlan(PVID) control,untag packet control            */
/* Priority order is : un_tag > pvlan>p_tag>UKN_VLAN, Rsvd: key 0-1 */
#define VLAN_DEF_UKN_VLAN_KEY   2
#define VLAN_DEF_PTAG_VLAN_KEY  3
#define VLAN_DEF_PVLAN_KEY      4
#define VLAN_DEF_UNTAG_KEY_0    5
#define VLAN_DEF_UNTAG_KEY_1    6
#define VLAN_DEF_UNTAG_KEY_2    7

/*==============================================================*/

/* cs_port_id_t map to cs_aal_port_id_t, only map GE(AAL_PORT_ID_GE:0), PON(AAL_PORT_ID_PON:1) */
#define AAL_PORT(port)      ((port) == CS_PON_PORT_ID ? 1 : 0)


/* P_TAG(zero vlan) packets process selection */
typedef enum {
    SDL_P_TAG_VLAN  = 0,
    SDL_P_TAG_UNTAG = 1
}cs_sdl_p_tag_type_t;

/* P_TAG(zero vlan) packets control selection */
typedef enum {
    SDL_PTAG_DROP         = 0,
    SDL_PTAG_FWD          = 1,
    SDL_PTAG_FWD_UNTAG    = 2, // POP tag 0
    SDL_PTAG_FWD_TAG      = 3, // == swap 
    SDL_PTAG_CTRL_MAX     = SDL_PTAG_FWD_TAG
}cs_sdl_p_tag_ctrl_t;

/* Untag packets control selection */
typedef enum {
    SDL_UNTAG_DROP         = 0,
    SDL_UNTAG_FWD          = 1,
    SDL_UNTAG_FWD_TAG      = 2,
    SDL_UNTAG_FWD_TAG_IPV4 = 3,
    SDL_UNTAG_FWD_TAG_IPV6 = 4,
    SDL_UNTAG_FWD_TAG_IP   = 5,
    SDL_UNTAG_CTRL_MAX     = SDL_UNTAG_FWD_TAG_IP
}cs_sdl_untag_ctrl_t;


/* Unknow vlan packets control selection */
typedef enum {
    SDL_UNK_VLAN_DROP      = 0,
    SDL_UNK_VLAN_FWD       = 1,
    SDL_UNK_VLAN_FWD_UNTAG = 2,
    SDL_UNK_VLAN_FWD_TAG   = 3,
    SDL_UNK_VLAN_MAX       = SDL_UNK_VLAN_FWD_TAG
}cs_sdl_unk_vlan_ctrl_t;


/* VLAN commands */
typedef enum {
    SDL_VLAN_CMD_NOP  = 0,
    SDL_VLAN_CMD_PUSH = 1,
    SDL_VLAN_CMD_POP  = 2,
    SDL_VLAN_CMD_SWAP = 3,
    SDL_VLAN_CMD_L2   = 4,
    SDL_VLAN_CMD_MAX  = SDL_VLAN_CMD_L2
}cs_sdl_vlan_cmd_t;


typedef struct {    
    cs_uint16                type; //unicase, multicast
    cs_uint16             vlan_id;
    cs_sdl_vlan_cmd_t    vlan_cmd;    
    cs_uint16              op_vid;
}cs_sdl_vlan_oper_t;


typedef struct
{
    cs_uint8                  count;
    cs_sdl_vlan_oper_t        vlan_entry[SDL_VLAN_ENGINE_MAX];
    
}cs_sdl_vlan_member_port_t;

/*******************************************************************************/
/* $rtn_hdr_start  vlan_ptag_type_set                                          */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_ptag_type_set(
    CS_IN cs_port_id_t        port_id, 
    CS_IN cs_sdl_p_tag_type_t type
);
/* INPUTS     :                                                                */
/*              o port_id -- port index.(GE:upstream,PON:downstream)           */
/*              o type                                                         */
/* OUTPUTS    : o NA                                                           */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to configured priority tag whether be as VLAN ID.          */
/*typedef enum {                                                               */
/*    SDL_P_TAG_VLAN = 0,                                                      */
/*    SDL_P_TAG_UNTAG = 1,                                                     */
/*}cs_sdl_p_tag_type_t;                                                        */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  vlan_ptag_type_get                                          */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_ptag_type_get(
    CS_IN  cs_port_id_t        port_id, 
    CS_OUT cs_sdl_p_tag_type_t *type
);
/* INPUTS     :                                                                */
/*              o port_id -- port index.(GE:upstream,PON:downstream)           */
/* OUTPUTS    : o type                                                         */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to Retrieve priority tag whether be as VLAN ID.            */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  vlan_def_vid_set                                            */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_def_vid_set(
    CS_IN cs_port_id_t  port_id, 
    CS_IN cs_uint16     pvid  
);
/* INPUTS     :                                                                */
/*              o port_id -- UNI port index.(use GE port)                      */
/*              o pvid ---(default=0) range: 0-0xffe(4094)                     */
/* OUTPUTS    : o NA                                                           */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to set port default vlanid and relating rule.              */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  vlan_def_vid_get                                            */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_def_vid_get(
    CS_IN  cs_port_id_t  port_id, 
    CS_OUT cs_uint16     *pvid    
);
/* INPUTS     :                                                                */
/*              o port_id -- UNI port index.(use GE port)                      */
/* OUTPUTS    : o pvid --- range: 0-0xffe(4094)                                */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to Retrieve PVID.                                          */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  vlan_def_vlan_ctrl_set                                      */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_def_vlan_ctrl_set(
    CS_IN cs_port_id_t  port_id, 
    CS_IN cs_boolean    drop_en,
    CS_IN cs_boolean    in_untag_en    
);
/* INPUTS     :                                                                */
/*              o port_id -- port index.(GE:upstream,PON:downstream)           */
/*              o drop_en --- enable/disable to drop the packets with VID=PVID.*/
/*              o in_untag_en ---enable/disable pop the tag of ingress packets */
/*              o                 with VID=PVID                                */
/* OUTPUTS    : o NA                                                           */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to:                                                        */
/*            enable/disable to drop the packets with VID=PVID.                */
/*            enable/disable pop the tag of ingress packets with VID=PVID      */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  vlan_def_vlan_ctrl_get                                      */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_def_vlan_ctrl_get(
    CS_IN  cs_port_id_t  port_id, 
    CS_OUT cs_boolean    *drop_en,    
    CS_OUT cs_boolean    *in_untag_en    
);
/* INPUTS     :                                                                */
/*              o port_id -- port index.(GE:upstream,PON:downstream)           */
/* OUTPUTS    : o drop_en ---1:drop,0:FWD                                      */
/*              o in_untag_en ---1:pop,0:NOP                                   */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to Retrieve the control status for packets with VID=PVID.  */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  vlan_unk_vlan_ctrl_set                                      */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_unk_vlan_ctrl_set(
    CS_IN cs_port_id_t           port_id, 
    CS_IN cs_sdl_unk_vlan_ctrl_t unk_vlan_status    
);
/* INPUTS     :                                                                */
/*              o port_id -- port index.(GE:upstream,PON:downstream)           */
/*              o unk_vlan_status                                              */
/* OUTPUTS    : o NA                                                           */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to configured unkonw vlan behavior                         */
/*typedef enum {                                                               */
/*    SDL_UNK_VLAN_DROP = 0,                                                   */
/*    SDL_UNK_VLAN_FWD = 1,                                                    */
/*    SDL_UNK_VLAN_FWD_UNTAG = 2,                                              */
/*}cs_sdl_unk_vlan_ctrl_t;                                                     */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  vlan_unk_vlan_ctrl_get                                      */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_unk_vlan_ctrl_get(
    CS_IN  cs_port_id_t           port_id, 
    CS_OUT cs_sdl_unk_vlan_ctrl_t *unk_vlan_status    
);
/* INPUTS     :                                                                */
/*              o port_id -- port index.                                       */
/* OUTPUTS    : o unk_vlan_status                                              */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to retrieve the behavior of the packets with unknow vlanid.*/
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  vlan_untag_ctrl_set                                         */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_untag_ctrl_set(
    CS_IN cs_port_id_t        port_id, 
    CS_IN cs_sdl_untag_ctrl_t untag_status    
);
/* INPUTS     :                                                                */
/*              o port_id -- port index.(GE:upstream,PON:downstream)           */
/*              o untag_status                                                 */
/* OUTPUTS    : o NA                                                           */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to configured untag packets behavior                       */
/*typedef enum {                                                               */
/*    SDL_UNTAG_DROP = 0,                                                      */
/*    SDL_UNTAG_FWD = 1,                                                       */
/*    SDL_UNTAG_FWD_TAG = 2,                                                   */
/*    SDL_UNTAG_FWD_TAG_IPV4 = 3,                                              */
/*    SDL_UNTAG_FWD_TAG_IPV6 = 4,                                              */
/*    SDL_UNTAG_FWD_TAG_IP = 5,                                                */
/*}cs_sdl_untag_ctrl_t;                                                        */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  vlan_untag_ctrl_get                                         */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_untag_ctrl_get(
    CS_IN  cs_port_id_t        port_id, 
    CS_OUT cs_sdl_untag_ctrl_t *untag_status    
);
/* INPUTS     :                                                                */
/*              o port_id -- port index.(GE:upstream,PON:downstream)           */
/* OUTPUTS    : o untag_status                                                 */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to retrieve the behavior of the untag packets.             */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/

////////////////////// public/////////////////////////////////////////////
cs_status vlan_member_add(
    CS_IN cs_port_id_t         port_id, 
    CS_IN cs_uint16            vlan_num,
    CS_IN cs_sdl_vlan_oper_t   *vlanid_list
);

cs_status vlan_member_del(
    CS_IN cs_port_id_t         port_id, 
    CS_IN cs_uint16            vlan_num,
    CS_IN cs_uint16            *vlanid_list
);

cs_status vlan_member_get(
    CS_IN  cs_port_id_t         port_id, 
    CS_OUT cs_uint16            *vlan_num,
    CS_OUT cs_sdl_vlan_oper_t   *vlanid_list
);

cs_status vlan_ptag_ctrl_set(
    CS_IN cs_port_id_t         port_id, 
    CS_IN cs_sdl_p_tag_ctrl_t  ptag_status    
);

cs_status vlan_ptag_ctrl_get(
    CS_IN  cs_port_id_t         port_id, 
    CS_OUT cs_sdl_p_tag_ctrl_t  *ptag_status    
);

cs_status cosmap_vlan_untag_ctrl_update( CS_IN  cs_port_id_t   port_id );

cs_status vlan_def_vlan_push_set(
    CS_IN cs_port_id_t  port_id
);


#ifdef HAVE_SDL_NTT
/*******************************************************************************/
/* Follow APIs are only for NTT                                                */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
/*******************************************************************************/

cs_status vlan_filter_def_vlan_set(
    CS_IN cs_uint16            pvid, 
    CS_IN cs_boolean         drop_en
);

/*******************************************************************************/
/* $rtn_hdr_start  vlan_filter_ctrl_set                                        */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_filter_ctrl_set(
    CS_IN cs_port_id_t     port_id, 
    CS_IN cs_boolean       filter_en
);
/* INPUTS     :                                                                */
/*              o port_id -- port index.(Only for PON:downstream)              */
/*              o filter_en --- enable/disable vlan filter                     */
/* OUTPUTS    : o NA                                                           */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to enable/disable vlan filter FUNC                         */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  vlan_filter_table_update                                    */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_filter_table_update(
    CS_IN cs_port_id_t     port_id, 
    CS_IN cs_uint16        vlan_num,
    CS_IN cs_uint16        *vlanid_list
);
/* INPUTS     :                                                                */
/*              o port_id -- port index.(Only for PON:downstream)              */
/*              o vlan_num ---0---SDL_VLAN_TABLE_MAX_ENTRY*8 (0 clear table)   */
/*              o vlanid_list ---list of vlan ID                               */
/* OUTPUTS    : o NA                                                           */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to update vlan table for vlan filter FUNC                  */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  vlan_spec_tpid_filter_set                                   */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_spec_tpid_filter_set(
    CS_IN cs_port_id_t   port_id, 
    CS_IN cs_uint16      spec_tpid
);
/* INPUTS     :                                                                */
/*              o port_id -- port index.(GE:upstream,PON:downstream)           */
/*              o spec_tpid ---TPID_8808,TPID_8809,TPID_888e,etc               */
/* OUTPUTS    : o NA                                                           */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to set behavior of specify TPID filter                     */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/


/*******************************************************************************/
/* $rtn_hdr_start  vlan_spec_tpid_filter_clr                                   */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
cs_status vlan_spec_tpid_filter_clr(
    CS_IN cs_port_id_t  port_id
);
/* INPUTS     :                                                                */
/*              o port_id -- port index.(GE:upstream,PON:downstream)           */
/* OUTPUTS    : o NA                                                           */
/* RETURNS    : o CS_ERROR: other error occurs                                 */
/*              o CS_E_PARAM: one of parameter is not correct                  */
/*              o CS_OK: set successfully                                      */
/* DESCRIPTION:                                                                */
/* This API is used to clear the config for behavior of specify TPID filter    */
/* $rtn_hdr_end                                                                */
/*******************************************************************************/
#endif

#ifdef HAVE_SDL_KT

// for vlan egress mode behavior at port. CLS is Ingress,so 
// pop tag for upstream(UNI), configure at pon port,
// pop tag for downstream(PON),configure at UNI port.
#define POP_PORT(port)      ((port) == CS_PON_PORT_ID ? 1 : 0)

#define SDL_VLAN_EGRESS_TABLE_MAX_ENTRY    4
#define SDL_VLAN_EGRESS_TABLE_START_ENTRY  2 // 2,3,4,5   


// fo fwd packet with ether type 0x8899 from reateltek switch
#define SDL_VLAN_SW_RTK                    10 // total 12 entries

/* The behavior of vlan for egress packet at port */
typedef enum 
{
    SDL_VLAN_EMODE_TAG   =0,         //unmodify egress packet when packet's VID is equal to vlan
    SDL_VLAN_EMODE_UNTAG =1          //pop egress packet tag when packet's VID is equal to vlan
} cs_sdl_vlan_egressmode_t;

typedef struct {    
    cs_uint16                   vlan_id;
    cs_sdl_vlan_egressmode_t    eg_mode;    

}cs_sdl_vlan_mem_t;


/*******************************************************************************/
/* Follow APIs are for KT                                                      */
/* CATEGORY   : Device                                                         */
/* ACCESS     : SDL                                                            */
/* BLOCK      : SDL                                                            */
/* CHIP       : 8030                                                           */
/*******************************************************************************/
cs_status vlan_add_vlan_member(
       CS_IN cs_port_id_t                port_id, 
       CS_IN cs_uint16                    vlanid, 
       CS_IN cs_sdl_vlan_egressmode_t egress_mode
);

cs_status vlan_del_vlan_member(
       CS_IN cs_port_id_t                port_id, 
       CS_IN cs_uint16                    vlanid
);

cs_status vlan_clr_vlan_member( CS_IN cs_port_id_t port_id );

cs_status vlan_dump_vlan_member(
    CS_IN  cs_port_id_t               port_id, 
    CS_OUT cs_uint8                   *tag_number,
    CS_OUT cs_uint16                  *tag_vlanid_list,
    CS_OUT cs_uint8                   *untag_number,
    CS_OUT cs_uint16                  *untag_vlanid_list
);
#endif

#if 1
//added by zhuxh
extern int pkt_vlan_tag_check(char *eth_pkt, cs_uint32 len);
extern int pkt_ip_check(char *eth_pkt);
extern cs_status ip_pkt_to_pkt_with_tag( cs_uint8 *eth_pkt, cs_uint32 *len, cs_port_id_t port_id );
extern cs_uint16 port_def_vlan_id_get(cs_port_id_t port_id);
extern cs_status vlan_add_all_port(int vlan);
#endif

#endif /* __SDL_VLAN_UTIL_H__ */


