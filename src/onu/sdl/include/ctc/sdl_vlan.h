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
 * $Id: sdl_vlan.h,v 1.1.2.11.6.1.12.4 2013/12/02 08:31:41 jsheng Exp $
 */


#ifndef __SDL_VLAN_H__
#define __SDL_VLAN_H__

#include "plat_common.h"

#ifdef HAVE_MPORTS
#define __VLAN_RULE_PER_PORT_MAX        32
#else
#define __VLAN_RULE_PER_PORT_MAX        8
#endif

#define __MC_VLAN_PER_PORT_MAX          8

typedef enum
{
    SDL_MC_VLAN_TAG_KEEP = 0,		    /* do not strip off VLAN TAG  */
    SDL_MC_VLAN_TAG_STRIP,			    /* strip off VLAN TAG  */
    SDL_MC_VLAN_TAG_SWAP,			    /* swap the VLAN , VLAN A <-> VLAN B */
    SDL_MC_VLAN_TAG_END
}cs_sdl_mc_vlan_act_t;


typedef enum
{
    SDL_VLAN_MODE_TRANSPARENT = 0,		/* transparent mode */
    SDL_VLAN_MODE_TAG,			        /* tag mode */
    SDL_VLAN_MODE_TRANSLATION,			/* translation mode, VLAN A <-> VLAN B */
    SDL_VLAN_MODE_AGGREGATION,			/* 1:N aggregation mode */
    SDL_VLAN_MODE_TRUNK,			    /* trunk mode */
    SDL_VLAN_MODE_STACKING,             /* zte stacking mode */
    SDL_VLAN_MODE_END
}cs_sdl_vlan_mode_t;

typedef enum
{
    SDL_EXT_QINQ_MATCH_ETHER_TYPE 		=0x00,		
    SDL_EXT_QINQ_MATCH_CVLAN_TYPE 		=0x01,			        
    SDL_EXT_QINQ_MATCH_IP_PRO_TYPE      =0x02,			
    SDL_EXT_QINQ_MATCH_IP_TOS_TYPE		=0x03,
    SDL_EXT_QINQ_MATCH_SRC_IP_TYPE	    =0x04,			  
    SDL_EXT_QINQ_MATCH_DST_IP_TYPE		=0x05,     
    /*below is option match type , now oam not support it*/
    SDL_EXT_QINQ_MATCH_SRC_MAC_TYPE		=0x10, 
    SDL_EXT_QINQ_MATCH_DST_MAC_TYPE		=0x11, 
    SDL_EXT_QINQ_MATCH_COS_TYPE			=0x12, 
    SDL_EXT_QINQ_MATCH_L4_SRC_PORT_TYPE =0x13,
    SDL_EXT_QINQ_MATCH_L4_DST_PORT_TYPE =0x14,
    SDL_EXT_QINQ_MATCH_END_TYPE	
}cs_sdl_ext_qinq_match_type_t;


typedef enum
{
    SDL_EXT_QINQ_TPID_8100_TYPE 	    =0x00,		
    SDL_EXT_QINQ_TPID_9100_TYPE 		=0x01,			        
    SDL_EXT_QINQ_TPID_88A8_TYPE         =0x02	
}cs_sdl_ext_qinq_tpid_type_t;



typedef struct
{
    cs_uint16 tpid;
    cs_uint16 vid:12;
    cs_uint16 cfi:1;
    cs_uint16 pri:3;
}cs_sdl_vlan_tag_t;

typedef struct
{
	cs_sdl_vlan_tag_t c_vlan;	        /* customer vlan */
	cs_sdl_vlan_tag_t s_vlan;	        /* service vlan, for 1:N mode, s_vlan should be all the same */
}cs_sdl_vlan_cfg_t;

typedef struct
{
    cs_uint16 vid;                      /* vlan id */
    cs_uint16 mbr;                      /* member port bitmap */
    cs_uint16 utg;                      /* untagged port bitmap */
}cs_sdl_vlan_t;


typedef struct 
{
    cs_sdl_ext_qinq_match_type_t           match_type;     /* match type */

	cs_uint64           			       match_value;    /* match value */

    cs_uint16           				   attach_vid;     /* vlan id */
    cs_sdl_ext_qinq_tpid_type_t            tpid_type;      /* tpid */
    cs_uint32           				   pri;       	   /* priority */
}  cs_sdl_ext_qinq_cfg_t;

typedef struct
{
    cs_uint8    valid;
    cs_uint32 src_port;
    cs_uint32 dst_port;
    cs_uint16 src_vid;
    cs_uint16 dst_vid;
}cs_sdl_vlan_mapping_t;

cs_status epon_request_onu_vlan_set
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_sdl_vlan_tag_t             def_vlan,   /* ignored for TRANSPARENT mode*/
    CS_IN cs_sdl_vlan_mode_t            vlan_mode,
    CS_IN cs_sdl_vlan_cfg_t             *vlan_cfg,  /* ignored for TRANSPARENT mode */
    CS_IN cs_uint16                     cfg_nums    /* can be zero for modes */
);

cs_status epon_request_onu_vlan_get
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_vlan_tag_t            *def_vlan,  /* ignored for TRANSPARENT mode */
    CS_OUT cs_sdl_vlan_mode_t           *vlan_mode,
    CS_OUT cs_sdl_vlan_cfg_t            *vlan_cfg,   /* ignored for TRANSPARENT mode */
    CS_OUT cs_uint16                    *cfg_nums    /* zero for TRANSPARENT/TAGGED mode */
);

#if 1
//added by zhuxh
cs_status epon_onu_sw_get_port_pvid
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_uint16            		*vid
);
#endif

cs_status epon_request_onu_mc_vlan_add
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_uint16                     *vlan_id,
    CS_IN cs_uint16                     cfg_nums
);

cs_status epon_request_onu_mc_vlan_del
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_uint16                     *vlan_id,
    CS_IN cs_uint16                     cfg_nums
);

cs_status epon_request_onu_mc_vlan_get
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_uint16                    *vlan_id,
    CS_OUT cs_uint16                    *cfg_nums
);

cs_status epon_request_onu_mc_vlan_clr
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id
);

cs_status epon_request_onu_mc_vlan_action_set
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_sdl_mc_vlan_act_t          vlan_act,
    CS_IN cs_sdl_vlan_cfg_t             *swap_rule,
    CS_IN cs_uint16                     rule_nums
);

cs_status epon_request_onu_mc_vlan_action_get
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_mc_vlan_act_t          *vlan_act,
    CS_OUT cs_sdl_vlan_cfg_t             *swap_rule,
    CS_OUT cs_uint16                     *rule_nums
);


cs_status epon_request_onu_ext_qinq_add
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_sdl_ext_qinq_cfg_t       * qinq_cfg
);

cs_status epon_request_onu_ext_qinq_del
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_sdl_ext_qinq_cfg_t	        *qinq_cfg
);

cs_status epon_request_onu_ext_qinq_get
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_OUT cs_sdl_ext_qinq_cfg_t         *qinq_cfg_table,/*it is array*/
    CS_OUT cs_int32                     *table_len
);

#ifdef HAVE_ZTE_SIJIE
cs_status epon_request_onu_manage_vlan_set
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_uint16                     manage_vlanid
);

cs_status epon_request_onu_manage_vlan_del
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id
);


cs_status epon_request_onu_manage_vlan_get
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id, 
    CS_OUT cs_uint16                   * manage_vlanid
);

#endif /*END_HAVE_ZTE_SIJIE*/

cs_status epon_request_vlan_add_tag_port (
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_port_id_t                  port,
    CS_IN  cs_uint16                     vid
    
);

cs_status epon_request_vlan_add_untag_port (
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_port_id_t                  port,
    CS_IN  cs_uint16                     vid
    
);

cs_status epon_request_vlan_del(cs_uint16 vid);

cs_status epon_request_vlan_del_from_port (
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_port_id_t                  port,
    CS_IN  cs_uint16                     vid
);

cs_status epon_request_vlan_mapping_get (
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_sdl_vlan_mapping_t *mapping
);

cs_status epon_request_vlan_mapping_set (
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_sdl_vlan_mapping_t *mapping
);

#endif /* __SDL_VLAN_H__ */

