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
 * $Id: sdl_vlan.c,v 1.1.2.54.2.3.4.1.2.8 2013/12/26 08:37:33 jsheng Exp $
 */

#include "plat_common.h" 
#include "sdl_vlan.h"
#include "sdl_fdb.h"
#include "sdl_util.h"
#include "sdl.h"
#include "rtk_api.h"
#include "rtk_api_ext.h"
#include "rtk_error.h"
#include "rtl8367b_asicdrv.h"
#include "rtl8367b_asicdrv_vlan.h"
#include "rtl8367b_asicdrv_svlan.h"
#include "sdl_l2.h"


#define __MC_VLAN_SWAP_PER_PORT_MAX     8


#if 0
#define __VLAN_MAX                      /*(8*UNI_PORT_MAX+4)*/ 64
#else
//modified by zhuxh
#define __VLAN_MAX                      (9*UNI_PORT_MAX)	/*һ���˿���8��tag vlan����һ��untag vlan(default vlan)*/
#endif

#define __SVID_RSVD_TRANSPARENT         0

#define __INVALID_INDEX                 0xffff

#define __DEFAULT_FID                   0

#define __DS_TRANSPARENT_SVID_INDEX     0
#define __US_TRANSPARENT_EVID_INDEX     31
#define __US_TRANSPARENT_EVID           4096
#define __US_TRANSPARENT_ACL_ID         RTK_ACL_VLAN_START

#define __EXT_QINQ_ENTRY_NUM_MAX          8


#define __NULL_CHECK(pointer)                                       \
    do{                                                             \
    if((pointer) == NULL)                                           \
        return CS_E_PARAM;                                          \
    }while(0)

#define __VLAN_MODE_CHECK(mode)                                     \
    do{                                                             \
    if((mode) >= SDL_VLAN_MODE_END)                                 \
        return CS_E_PARAM;                                          \
    }while(0)


typedef struct
{
    cs_boolean          valid;      /* rule entry valid indication */
    cs_sdl_vlan_cfg_t   rule;       /* rule entry info */
}__vlan_rule_t;

typedef struct
{
    cs_boolean  valid;      /* MC VLAN entry valid indication */
    cs_uint16   vid;        /* MC VLAN ID */
}__mc_vlan_t;


typedef struct
{
    cs_boolean      valid;      /* VLAN entry valid indication */
    cs_sdl_vlan_t   vlan;       /* VLAN info */
}__vlan_t;

typedef struct
{
    cs_sdl_vlan_tag_t       def_vlan;                           /* port vlan id */
    cs_sdl_vlan_mode_t      vlan_mode;                          /* port vlan mode */
    __vlan_rule_t           vlan_rule[__VLAN_RULE_PER_PORT_MAX];         /* vlan rule table */
    cs_boolean              mc_vlan_en;                         /* mc vlan configured */
    __mc_vlan_t             mc_vlan[__MC_VLAN_PER_PORT_MAX];    /* mc vlan table */
    cs_sdl_mc_vlan_act_t    mc_act;                             /* mc tag action */
    __vlan_rule_t           mc_swap[__MC_VLAN_SWAP_PER_PORT_MAX];        /* mc vlan swap table */
}__port_vlan_info_t;

typedef struct
{
    cs_boolean      	    valid;      /* entry valid indication */
    cs_sdl_ext_qinq_cfg_t   cfg;       /* qinq entry info */
}__ext_qinq_t;

typedef struct
{
    __vlan_t                s_vlan_table[__VLAN_MAX];
    __vlan_t                c_vlan_table[__VLAN_MAX];
    __port_vlan_info_t      port_vlan_table[UNI_PORT_MAX + 1];
}__vlan_cfg_info_t;



typedef enum
{
    __RULE_LOOKUP_ALL   = 0,
    __RULE_LOOKUP_CVLAN = 1,
    __RULE_LOOKUP_SVLAN = 2
}__rule_mode_t;


static __vlan_t __s_vlan_table[__VLAN_MAX];
static __vlan_t __c_vlan_table[__VLAN_MAX];

static __port_vlan_info_t __port_vlan_table[UNI_PORT_MAX + 1];

static __ext_qinq_t __ext_qinq_table[__EXT_QINQ_ENTRY_NUM_MAX];




static cs_boolean __vlan_table_lookup
(
    __vlan_t            *vlan_table, 
    cs_uint16           vid, 
    cs_uint16           *index          /* if false, index==first empty entry; if true, index==vid entry */
)
{
    int i;
    cs_boolean empty_found = EPON_FALSE;

    for(i = 0; i < __VLAN_MAX; ++i)
    {
        if( (!vlan_table[i].valid) && (!empty_found) )
        {
            *index = i;
            empty_found = EPON_TRUE;
            continue;
        }
    
        if( (vlan_table[i].valid) && (vid == vlan_table[i].vlan.vid) )
        {   
            *index = i;
            return EPON_TRUE;
        }
    }

    if(!empty_found) /* table is full */
    {
        *index = __INVALID_INDEX;
    }

    return EPON_FALSE;
}


static cs_status __vlan_table_add_port(__vlan_t *vlan_table, cs_uint16 vid, cs_port_id_t port, cs_boolean utg_en)
{
    cs_uint16 index;

    __vlan_table_lookup(vlan_table, vid, &index);

    if(__INVALID_INDEX == index)
    {
        return CS_E_RESOURCE;
    }

    vlan_table[index].vlan.vid = vid;
    vlan_table[index].vlan.mbr |= (1<<L2P_PORT(port));

    if(utg_en)
    {
        vlan_table[index].vlan.utg |= (1<<L2P_PORT(port));
    }
    else
    {
        vlan_table[index].vlan.utg &= ~(1<<L2P_PORT(port));
    }
    
    vlan_table[index].valid = EPON_TRUE;

    return CS_E_OK;
}

static void __vlan_table_del_port(__vlan_t *vlan_table, cs_port_id_t port)
{
    int i;
    rtk_port_t rtk_port = L2P_PORT(port);

    for(i = 0; i < __VLAN_MAX; ++i)
    {
        if(vlan_table[i].valid)
        {
            vlan_table[i].vlan.mbr &= ~(1<<rtk_port);
            vlan_table[i].vlan.utg &= ~(1<<rtk_port);
            if(0 == (vlan_table[i].vlan.mbr & 0xf))  /* no UNI port in this vlan */
            {
                vlan_table[i].valid = EPON_FALSE;
            }
        }
    }
}

static cs_boolean __rule_table_lookup(__vlan_rule_t *rule_table, __rule_mode_t mode, cs_sdl_vlan_cfg_t *rule, cs_uint16 *index)
{
    int i = 0;
    cs_boolean empty_found = EPON_FALSE;

    for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
    {
        /* search the first empty entry */
        if( (!rule_table[i].valid) && (!empty_found) )
        {
            *index = i;
            empty_found = EPON_TRUE;
            continue;
        }
    
        if(rule_table[i].valid)
        {
            if( (__RULE_LOOKUP_ALL == mode) &&
                (rule_table[i].rule.c_vlan.vid == rule->c_vlan.vid) &&
                (rule_table[i].rule.s_vlan.vid == rule->s_vlan.vid) ) 
            {
                *index = i;
                return EPON_TRUE;
            }
            
            if( (__RULE_LOOKUP_CVLAN == mode) &&
                (rule_table[i].rule.c_vlan.vid == rule->c_vlan.vid) )
            {
                *index = i;
                return EPON_TRUE;
            }

            if( (__RULE_LOOKUP_SVLAN == mode) &&
                (rule_table[i].rule.s_vlan.vid == rule->s_vlan.vid) )
            {
                *index = i;
                return EPON_TRUE;
            }
        }
    }

    if(!empty_found) /* table is full */
    {
        *index = __INVALID_INDEX;
    }

    return EPON_FALSE;
}

static cs_status __rule_table_add(__vlan_rule_t *rule_table, __rule_mode_t mode, cs_sdl_vlan_cfg_t *rule)
{
    cs_uint16 index = 0;
    cs_boolean rule_found;

    rule_found = __rule_table_lookup(rule_table, mode, rule, &index);

    if(__INVALID_INDEX == index)
    {
        return CS_E_RESOURCE;
    }

    if(!rule_found)
    {
        rule_table[index].rule.s_vlan.vid = rule->s_vlan.vid;
        rule_table[index].rule.c_vlan.vid = rule->c_vlan.vid;
        rule_table[index].valid = EPON_TRUE;
    }
    
    return CS_E_OK;
}


static void __rule_table_clr(__vlan_rule_t *rule_table)
{
    int i;

	memset(rule_table,0,sizeof(__vlan_rule_t));
    for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)    
    {
        rule_table[i].valid = EPON_FALSE;
    }
}


static cs_boolean __mc_table_lookup(__mc_vlan_t *mc_vlan_table, cs_uint16 vid, cs_uint16 *index)
{
    int i;
    cs_boolean empty_found = EPON_FALSE;

    for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
    {
        if( (!mc_vlan_table[i].valid) && (!empty_found) )
        {
            *index = i;
            empty_found = EPON_TRUE;
            continue;
        }
    
        if( (mc_vlan_table[i].valid) && (vid == mc_vlan_table[i].vid) )
        {   
            *index = i;
            return EPON_TRUE;
        }
    }

    if(!empty_found) /* table is full */
    {
        *index = __INVALID_INDEX;
    }

    return EPON_FALSE;
}


static cs_status __mc_table_add(__mc_vlan_t *mc_vlan_table, cs_uint16 vid)
{
    cs_uint16 index = 0;

    if(!__mc_table_lookup(mc_vlan_table, vid, &index))
    {
        if(__INVALID_INDEX == index)
        {
            return CS_E_RESOURCE;
        }

        mc_vlan_table[index].vid = vid; 
        mc_vlan_table[index].valid = EPON_TRUE; 
    }

    return CS_E_OK;
}

static cs_status __mc_table_del(__mc_vlan_t *mc_vlan_table, cs_uint16 vid)
{
    cs_uint16 index = 0;

    if(__mc_table_lookup(mc_vlan_table, vid, &index))
    {
        mc_vlan_table[index].valid = EPON_FALSE; 
    }

    return CS_E_OK;
}

/* Transparent port should add all others port's vlan */
static cs_uint16 __get_transparent_mbp(void)
{
    int i;
    cs_uint16 mbp = 0;

    for(i = CS_UNI_PORT_ID1; i <= UNI_PORT_MAX; ++i)
    {
        if(SDL_VLAN_MODE_TRANSPARENT == __port_vlan_table[i].vlan_mode)
        {
            mbp |= (1<<(i-1));
        }
    }

    return mbp;
}

/* get the bitmap of ports that still use current MC VLAN */
static cs_uint16 __get_mc_vlan_port_mbp(cs_uint16 s_vid)
{
    cs_port_id_t port_id;
    cs_sdl_vlan_cfg_t vlan_rule;
    cs_uint16 index;
    cs_uint16 mbp = 0;
    
    for(port_id = CS_UNI_PORT_ID1; port_id <= UNI_PORT_MAX; port_id++)
    {
        memset(&vlan_rule, 0, sizeof(cs_sdl_vlan_cfg_t));
        vlan_rule.s_vlan.vid = s_vid;
        
        if(__mc_table_lookup(__port_vlan_table[port_id].mc_vlan, s_vid, &index) || 
          __rule_table_lookup(__port_vlan_table[port_id].mc_swap, __RULE_LOOKUP_SVLAN, &vlan_rule, &index))
        {
            mbp |= (1 << L2P_PORT(port_id));
        }
    }
    
    return mbp;
}

static void __transparent_us_set(rtk_port_t rtk_port, cs_boolean en)
{
    rtk_filter_field_t            filter_field;
    rtk_filter_cfg_t              cfg;
    rtk_filter_action_t           act;
    rtk_filter_number_t           ruleNum;
    static cs_uint32              __us_transparent_mbp = 0;

    if(en)
    {
        if(__us_transparent_mbp & (1<<rtk_port))
            return;
        __us_transparent_mbp |= (1<<rtk_port);
    }
    else
    {
        if(!(__us_transparent_mbp & (1<<rtk_port)))
            return;
        __us_transparent_mbp &= ~(1<<rtk_port);
    }

    /*Search all MAC (data & mask are all "0") to P0 to EVID 4096 for transparent*/  
    memset(&filter_field, 0, sizeof(rtk_filter_field_t));
    memset(&cfg, 0, sizeof(rtk_filter_cfg_t));
    memset(&act, 0, sizeof(rtk_filter_action_t));            
    filter_field.fieldType = FILTER_FIELD_SMAC;                    
    filter_field.filter_pattern_union.smac.dataType = FILTER_FIELD_DATA_MASK;
    filter_field.filter_pattern_union.smac.value.octet[0] = 0x00;
    filter_field.filter_pattern_union.smac.value.octet[1] = 0x00;
    filter_field.filter_pattern_union.smac.value.octet[2] = 0x00;
    filter_field.filter_pattern_union.smac.value.octet[3] = 0x00;
    filter_field.filter_pattern_union.smac.value.octet[4] = 0x00;
    filter_field.filter_pattern_union.smac.value.octet[5] = 0x00;            
    filter_field.filter_pattern_union.smac.mask.octet[0]  = 0x00;
    filter_field.filter_pattern_union.smac.mask.octet[1]  = 0x00;
    filter_field.filter_pattern_union.smac.mask.octet[2]  = 0x00;
    filter_field.filter_pattern_union.smac.mask.octet[3]  = 0x00;
    filter_field.filter_pattern_union.smac.mask.octet[4]  = 0x00;
    filter_field.filter_pattern_union.smac.mask.octet[5]  = 0x00;
    filter_field.next = NULL;        
    rtk_filter_igrAcl_field_add(&cfg, &filter_field);

    /* Set active ports */
    cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
    cfg.activeport.value = __us_transparent_mbp;
    cfg.activeport.mask = 0xFF; 
    cfg.invert = FALSE;
         
    /* Set Action to CVLAN Index 31 */
    act.actEnable[FILTER_ENACT_INGRESS_CVLAN_INDEX] = TRUE;
    act.filterIngressCvlanIdx = __US_TRANSPARENT_EVID_INDEX;
    rtk_filter_igrAcl_cfg_del(__US_TRANSPARENT_ACL_ID);
    rtk_filter_igrAcl_cfg_add(__US_TRANSPARENT_ACL_ID, &cfg, &act, &ruleNum);
}

#ifdef HAVE_GWD_VLAN_MODE
#define __SVID_RSVD_UNTAG	1
static void __transparent_hw_set(rtk_port_t rtk_port, cs_boolean tran_en, rtk_vlan_acceptFrameType_t accept_frame_type)
{
	rtl8367b_svlan_memconf_t unmatch_svlan_memconf;
	rtl8367b_svlan_memconf_t untag_svlan_memconf;
	
	memset(&unmatch_svlan_memconf, 0, sizeof(rtl8367b_svlan_memconf_t));
	memset(&untag_svlan_memconf, 0, sizeof(rtl8367b_svlan_memconf_t));

	/*vlan ��ǩ0������͸��*/
	rtk_svlan_unmatch_action_set(UNMATCH_ASSIGN, __SVID_RSVD_TRANSPARENT);	//��vlan ��ƥ��İ�������vlan ��ǩ0
	
	/*vlan ��ǩ1������untag ��*/
	rtk_svlan_untag_action_set(UNTAG_ASSIGN, __SVID_RSVD_UNTAG);	//��untag ������vlan ��ǩ1

	/*vlan 0 ������*/
	rtl8367b_getAsicSvlanMemberConfiguration(__DS_TRANSPARENT_SVID_INDEX, &unmatch_svlan_memconf);
	unmatch_svlan_memconf.vs_svid = __SVID_RSVD_TRANSPARENT;
	unmatch_svlan_memconf.vs_member |= (1<<SWITCH_UPLINK_PORT);
	unmatch_svlan_memconf.vs_untag  &= ~(1<<rtk_port);
	unmatch_svlan_memconf.vs_untag  |= (1<<SWITCH_UPLINK_PORT);

	/*vlan 1 ������*/
	rtl8367b_getAsicSvlanMemberConfiguration(__SVID_RSVD_UNTAG, &untag_svlan_memconf);	//untag vlan 1 ������
	untag_svlan_memconf.vs_svid = __SVID_RSVD_UNTAG;
	untag_svlan_memconf.vs_member |= (1<<SWITCH_UPLINK_PORT);
	untag_svlan_memconf.vs_untag  &= ~(1<<rtk_port);
	untag_svlan_memconf.vs_untag  |= (1<<SWITCH_UPLINK_PORT);
	if(tran_en)		//͸���˿ڵ�����
	{
		
		rtk_vlan_portIgrFilterEnable_set(rtk_port, EPON_FALSE);				//�رս���˿ڵİ�����vlan ���Ĺ���
		rtk_vlan_portAcceptFrameType_set(rtk_port, ACCEPT_FRAME_TYPE_ALL);	//�˿ڽ������еİ�
		__transparent_us_set(rtk_port, EPON_TRUE);							//�����ǽ�ָ���Ķ˿ڼ��뵽͸���˿��б�

		/*vlan 0 ������*/
		unmatch_svlan_memconf.vs_member |= (1<<rtk_port);
	
		/*vlan 1 ������*/
		untag_svlan_memconf.vs_member |= (1<<rtk_port);
	}
	else			//��͸���˿ڵ�����
	{
		rtk_vlan_portIgrFilterEnable_set(rtk_port, EPON_TRUE);			//�򿪽���˿ڵİ�����vlan ���Ĺ���
		rtk_vlan_portAcceptFrameType_set(rtk_port, accept_frame_type);	//�˿ڽ���ָ���İ�
		__transparent_us_set(rtk_port, EPON_FALSE);						//�����ǽ�ָ���Ķ˿ڴ�͸���˿��б����Ƴ�

		/*vlan 0 ������*/	
		unmatch_svlan_memconf.vs_member &= ~(1<<rtk_port);

		/*vlan 1 ������*/
		rtk_port_t new_port = P2L_PORT(rtk_port);
		__port_vlan_info_t *port_vlan = &__port_vlan_table[new_port];
		if(1 == port_vlan->def_vlan.vid)
		{
			untag_svlan_memconf.vs_member |= (1<<rtk_port);
		}
		else
		{
			untag_svlan_memconf.vs_member &= ~(1<<rtk_port);
		}	
	}	
	rtl8367b_setAsicSvlanMemberConfiguration(__DS_TRANSPARENT_SVID_INDEX, &unmatch_svlan_memconf);
	rtl8367b_setAsicSvlanMemberConfiguration(__SVID_RSVD_UNTAG, &untag_svlan_memconf);
	#if 0
	cs_printf("tran_en :0x%x\n", tran_en);
	cs_printf("rtk_port :0x%x\n", rtk_port);
	cs_printf("unmatch_svlan_memconf.vs_svid :0x%x\n", unmatch_svlan_memconf.vs_svid);
	cs_printf("unmatch_svlan_memconf.vs_member :0x%x\n", unmatch_svlan_memconf.vs_member);
	cs_printf("unmatch_svlan_memconf.vs_untag :0x%x\n", unmatch_svlan_memconf.vs_untag);
	cs_printf("untag_svlan_memconf.vs_svid :0x%x\n", untag_svlan_memconf.vs_svid);
	cs_printf("untag_svlan_memconf.vs_member :0x%x\n", untag_svlan_memconf.vs_member);
	cs_printf("untag_svlan_memconf.vs_untag :0x%x\n", untag_svlan_memconf.vs_untag);
	#endif	
}
#else
static void __transparent_hw_set(rtk_port_t rtk_port, cs_boolean tran_en, rtk_vlan_acceptFrameType_t accept_frame_type)
{
    rtl8367b_svlan_memconf_t svlan_memconf;

    memset(&svlan_memconf, 0, sizeof(rtl8367b_svlan_memconf_t));

    if(tran_en)
    {
        rtk_vlan_portIgrFilterEnable_set(rtk_port, EPON_FALSE);
        rtk_vlan_portAcceptFrameType_set(rtk_port, ACCEPT_FRAME_TYPE_ALL);
        __transparent_us_set(rtk_port, EPON_TRUE);

        /* Reserve entry 0 for downstream transparent */
        rtl8367b_getAsicSvlanMemberConfiguration(__DS_TRANSPARENT_SVID_INDEX, &svlan_memconf);
        svlan_memconf.vs_svid = __SVID_RSVD_TRANSPARENT;
        svlan_memconf.vs_member |= (1<<rtk_port);
        svlan_memconf.vs_member |= (1<<SWITCH_UPLINK_PORT);
        svlan_memconf.vs_untag  &= ~(1<<rtk_port);
        svlan_memconf.vs_untag  |= (1<<SWITCH_UPLINK_PORT);
        rtl8367b_setAsicSvlanMemberConfiguration(0, &svlan_memconf);
        
        rtk_svlan_untag_action_set(UNTAG_ASSIGN, __SVID_RSVD_TRANSPARENT);
        rtk_svlan_unmatch_action_set(UNMATCH_ASSIGN, __SVID_RSVD_TRANSPARENT);
    }
    else
    {
        /* non-transparent configuration */
        rtk_vlan_portIgrFilterEnable_set(rtk_port, EPON_TRUE);
        rtk_vlan_portAcceptFrameType_set(rtk_port, accept_frame_type);

        __transparent_us_set(rtk_port, EPON_FALSE);

        rtl8367b_getAsicSvlanMemberConfiguration(__DS_TRANSPARENT_SVID_INDEX, &svlan_memconf);
        svlan_memconf.vs_svid = __SVID_RSVD_TRANSPARENT;
        svlan_memconf.vs_member &= ~(1<<rtk_port);
        if(0 == (svlan_memconf.vs_member & 0xf)) /* remove uplink port when no UNI port in the map*/
        {
            svlan_memconf.vs_member = 0;
            svlan_memconf.vs_untag  = 0;
            rtk_svlan_untag_action_set(UNTAG_DROP, __SVID_RSVD_TRANSPARENT);
            rtk_svlan_unmatch_action_set(UNTAG_DROP, __SVID_RSVD_TRANSPARENT);
        }
        rtl8367b_setAsicSvlanMemberConfiguration(0, &svlan_memconf);
    }
}
#endif

static void __remove_port_from_svlan(cs_uint16 s_vid, rtk_port_t rtk_port)
{
    rtk_svlan_memberCfg_t rtk_svlan_cfg;
    rtl8367b_svlan_memconf_t svlan_memconf;
    cs_uint16 transparent_mbp = __get_transparent_mbp();
    cs_uint16 mcvlan_mbp = __get_mc_vlan_port_mbp(s_vid);

    rtk_svlan_memberPortEntry_get(s_vid, &rtk_svlan_cfg);
    
    rtk_svlan_cfg.memberport &= ~(1<<rtk_port);
    rtk_svlan_cfg.untagport &= ~(1<<rtk_port);
    rtk_svlan_cfg.svid = s_vid;

	#ifdef HAVE_GWD_VLAN_MODE
	//added by zhuxh, svlan 1 is used for down untag stream transmit, should not be deleted
	if(1 == s_vid)
	{
		goto end;
	}
	#endif
	
    /* bug 30963: only clear this SVLAN entry if both UC and MC don't use it */
    if((0 == (0xf & (rtk_svlan_cfg.memberport & (~transparent_mbp))))  /* Clean entry, if no non-transparent port in this vlan */
       && (0 == (rtk_svlan_cfg.memberport & mcvlan_mbp)))
    {
        memset(&svlan_memconf, 0, sizeof(rtl8367b_svlan_memconf_t));
        rtl8367b_setAsicSvlanMemberConfiguration(rtk_svlan_cfg.index, &svlan_memconf);
        return;
    }
#ifdef HAVE_GWD_VLAN_MODE
//added by zhuxh
end:
#endif
    rtk_svlan_memberPortEntry_set(s_vid, &rtk_svlan_cfg);
}

static void __remove_port_from_cvlan(cs_uint16 c_vid, rtk_port_t rtk_port)
{    
    rtk_portmask_t rtk_mbr,rtk_utg;
    rtk_fid_t rtk_fid;

    rtk_vlan_get(c_vid, &rtk_mbr, &rtk_utg, &rtk_fid);
    rtk_mbr.bits[0] &= ~(1<<rtk_port);
    rtk_utg.bits[0] &= ~(1<<rtk_port);
    if(0 == (rtk_mbr.bits[0] & 0xf))  /* no UNI port in this vlan */
    {
        rtk_mbr.bits[0] = 0;
        rtk_utg.bits[0] = 0;
    }
    rtk_vlan_set(c_vid, rtk_mbr, rtk_utg, rtk_fid);
}

static void __clear_hw_mc_swap(cs_port_id_t port_id)
{
    int i;
    rtk_port_t rtk_port = L2P_PORT(port_id);

    __port_vlan_info_t *port_vlan = &__port_vlan_table[port_id];

    for(i = 0; i < __MC_VLAN_SWAP_PER_PORT_MAX; ++i)
    {
        if(port_vlan->mc_swap[i].valid)
        {
            //rtk_svlan_c2s_del(port_vlan->mc_swap[i].rule.c_vlan.vid, rtk_port);
            rtk_svlan_sp2c_del(port_vlan->mc_swap[i].rule.s_vlan.vid, rtk_port);
            __remove_port_from_svlan(port_vlan->mc_swap[i].rule.s_vlan.vid, rtk_port);
            __remove_port_from_cvlan(port_vlan->mc_swap[i].rule.c_vlan.vid, rtk_port);
        }
    }
}

static void __clear_hw_mc_vlan(cs_port_id_t port_id)
{
    int i;
    rtk_port_t rtk_port = L2P_PORT(port_id);

    __port_vlan_info_t *port_vlan = &__port_vlan_table[port_id];

    for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
    {
        if(port_vlan->mc_vlan[i].valid)
        {
            __remove_port_from_svlan(port_vlan->mc_vlan[i].vid, rtk_port);	
			// added by wujm,delete cvlan when delete mc vlan
			__remove_port_from_cvlan(port_vlan->mc_vlan[i].vid, rtk_port);
			rtk_svlan_c2s_del(port_vlan->mc_vlan[i].vid, L2P_PORT(port_id));
        }
    }
}

static void __clear_uc(cs_port_id_t port_id)
{
    int i;
    rtk_port_t rtk_port = L2P_PORT(port_id);

    __port_vlan_info_t *port_vlan = &__port_vlan_table[port_id];

    /* clear HW rule table */
    /* rule entry MUST be delete BEFORE vlan entry */
    for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
    {
        if(port_vlan->vlan_rule[i].valid)
        {
            rtk_svlan_c2s_del(port_vlan->vlan_rule[i].rule.c_vlan.vid, rtk_port);
            rtk_svlan_sp2c_del(port_vlan->vlan_rule[i].rule.s_vlan.vid, rtk_port);
        }
    }

	#if 1
	//added by zhuxh
	rtk_svlan_c2s_del(port_vlan->def_vlan.vid, rtk_port);
	#endif
	
    /* clear vlan table for this port */
    for(i = 0; i < __VLAN_MAX; ++i)
    {
        if(__s_vlan_table[i].valid)
        {
            __remove_port_from_svlan(__s_vlan_table[i].vlan.vid, rtk_port);
        }

        if(__c_vlan_table[i].valid)
        {
            __remove_port_from_cvlan(__c_vlan_table[i].vlan.vid, rtk_port);
        }
    }

    /* clear vlan table and rule table for this port */
    __vlan_table_del_port(__s_vlan_table, port_id);
    __vlan_table_del_port(__c_vlan_table, port_id);
    __rule_table_clr(__port_vlan_table[port_id].vlan_rule);
}

static cs_status __update_hw_mc(cs_port_id_t port_id)
{
    int i;
    __port_vlan_info_t *port_vlan = &__port_vlan_table[port_id];
    rtk_port_t rtk_port = L2P_PORT(port_id);
    rtk_svlan_memberCfg_t rtk_svlan_cfg;
    rtk_portmask_t rtk_mbr, rtk_utg;
    rtk_fid_t rtk_fid;
    cs_sdl_vlan_cfg_t vlan_rule;
    cs_uint16 index;
    cs_uint16 transparent_mbp = __get_transparent_mbp();
    cs_status   ret=0;

    /* delete overlap vlan rule */
    if(port_vlan->mc_vlan_en)
    {
        if(SDL_VLAN_MODE_TRANSLATION == port_vlan->vlan_mode)
        {
            /* delete overlaped vlan from vlan rule table*/
            for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
            {
                if(port_vlan->mc_vlan[i].valid)
                {
                    vlan_rule.s_vlan.vid = port_vlan->mc_vlan[i].vid; /* key */
                    if(__rule_table_lookup(port_vlan->vlan_rule, __RULE_LOOKUP_SVLAN, &vlan_rule, &index))
                    {
                        rtk_svlan_sp2c_del(port_vlan->mc_vlan[i].vid, L2P_PORT(port_id));
                    }
                }
            }
        }

        if(SDL_VLAN_MODE_AGGREGATION == port_vlan->vlan_mode)
        {
            for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
            {
                if(port_vlan->mc_vlan[i].valid)
                {
                    vlan_rule.s_vlan.vid = port_vlan->mc_vlan[i].vid; /* key */
                    if(__rule_table_lookup(port_vlan->vlan_rule, __RULE_LOOKUP_SVLAN, &vlan_rule, &index))
                    {
                        rtk_svlan_dmac_vidsel_set(rtk_port, EPON_FALSE);  /* disable d/s 1:N */
                        break;
                    }
                }
            }

            for(i = 0; i < __MC_VLAN_SWAP_PER_PORT_MAX; ++i)
            {
                if(port_vlan->mc_swap[i].valid)
                {
                    vlan_rule.s_vlan.vid = port_vlan->mc_swap[i].rule.s_vlan.vid; /* key */
                    if(__rule_table_lookup(port_vlan->vlan_rule, __RULE_LOOKUP_SVLAN, &vlan_rule, &index))
                    {
                        rtk_svlan_dmac_vidsel_set(rtk_port, EPON_FALSE);  /* disable d/s 1:N */
                        break;
                    }
                }
            }
        }
    }
    /* END: delete overlap vlan rule */

    if(SDL_MC_VLAN_TAG_KEEP == port_vlan->mc_act)           
    {
        for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
        {
            if(port_vlan->mc_vlan[i].valid)
            {
                /* svlan member */
                memset(&rtk_svlan_cfg, 0, sizeof(rtk_svlan_memberCfg_t));

                rtk_svlan_memberPortEntry_get(port_vlan->mc_vlan[i].vid, &rtk_svlan_cfg);
                rtk_svlan_cfg.memberport |= (1<<rtk_port);
                rtk_svlan_cfg.memberport |= (1<<SWITCH_UPLINK_PORT);
                rtk_svlan_cfg.memberport |= transparent_mbp;
                rtk_svlan_cfg.untagport &= ~(1<<rtk_port);
                rtk_svlan_cfg.svid = port_vlan->mc_vlan[i].vid;
                ret=rtk_svlan_memberPortEntry_set(port_vlan->mc_vlan[i].vid, &rtk_svlan_cfg);
                if (ret !=CS_E_OK ){
                    goto END;
                }
				// added by wujm,create c_vlan when create mc_vlan
				#if 1
				{
					cs_uint16 vid = port_vlan->mc_vlan[i].vid;
					cs_uint16 index = 0;
					rtk_portmask_t rtk_mbr, rtk_utg;
					
					if(__vlan_table_lookup(__c_vlan_table, vid, &index))
					{
						rtk_mbr.bits[0] = __c_vlan_table[i].vlan.mbr;
                		rtk_utg.bits[0] = __c_vlan_table[i].vlan.utg;
						rtk_utg.bits[0] &= ~(1<<rtk_port);
						rtk_mbr.bits[0] |= rtk_svlan_cfg.memberport;
                		rtk_utg.bits[0] |= rtk_svlan_cfg.untagport;						
					}
					else
					{
						rtk_mbr.bits[0] = rtk_svlan_cfg.memberport;
                		rtk_utg.bits[0] = rtk_svlan_cfg.untagport;
						
					}

					ret=rtk_vlan_set(vid, rtk_mbr, rtk_utg, 0);
	                if (ret !=CS_E_OK ){
	                    goto END;
	                }

					ret=rtk_svlan_c2s_add(vid, rtk_port, vid);
					if (ret !=CS_E_OK ){
	                    goto END;
	                }
				}
				#endif
            }
        }
    }
    else if(SDL_MC_VLAN_TAG_STRIP == port_vlan->mc_act)     
    {
        for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
        {
            if(port_vlan->mc_vlan[i].valid)
            {
                /* svlan member */
                memset(&rtk_svlan_cfg, 0, sizeof(rtk_svlan_memberCfg_t));

                rtk_svlan_memberPortEntry_get(port_vlan->mc_vlan[i].vid, &rtk_svlan_cfg);
                rtk_svlan_cfg.memberport |= (1<<rtk_port);
                rtk_svlan_cfg.memberport |= (1<<SWITCH_UPLINK_PORT);
                rtk_svlan_cfg.memberport |= transparent_mbp;
                rtk_svlan_cfg.untagport |= (1<<rtk_port);
                rtk_svlan_cfg.svid = port_vlan->mc_vlan[i].vid;
                ret=rtk_svlan_memberPortEntry_set(port_vlan->mc_vlan[i].vid, &rtk_svlan_cfg);
                if (ret !=CS_E_OK ){
                    goto END;
                }
				// added by wujm,create c_vlan when create mc_vlan
				#if 1
				{
					cs_uint16 vid = port_vlan->mc_vlan[i].vid;
					cs_uint16 index = 0;
					rtk_portmask_t rtk_mbr, rtk_utg;
					
					if(__vlan_table_lookup(__c_vlan_table, vid, &index))
					{
						rtk_mbr.bits[0] = __c_vlan_table[i].vlan.mbr;
                		rtk_utg.bits[0] = __c_vlan_table[i].vlan.utg;
						rtk_mbr.bits[0] |= rtk_svlan_cfg.memberport;
                		rtk_utg.bits[0] |= rtk_svlan_cfg.untagport;
					}
					else
					{
						rtk_mbr.bits[0] = rtk_svlan_cfg.memberport;
                		rtk_utg.bits[0] = rtk_svlan_cfg.untagport;
					}

					ret=rtk_vlan_set(vid, rtk_mbr, rtk_utg, 0);
	                if (ret !=CS_E_OK ){
	                    goto END;
	                }

					ret=rtk_svlan_c2s_add(vid, rtk_port, vid);
					if (ret !=CS_E_OK ){
	                    goto END;
	                }

				}
				#endif
            }
        }
    }
    else if(SDL_MC_VLAN_TAG_SWAP == port_vlan->mc_act)      
    {
        /* change previous mc vlan to KEEP mode */
        for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
        {
            if(port_vlan->mc_vlan[i].valid)
            {
                /* svlan member */
                memset(&rtk_svlan_cfg, 0, sizeof(rtk_svlan_memberCfg_t));

                rtk_svlan_memberPortEntry_get(port_vlan->mc_vlan[i].vid, &rtk_svlan_cfg);
                rtk_svlan_cfg.memberport |= (1<<rtk_port);
                rtk_svlan_cfg.memberport |= (1<<SWITCH_UPLINK_PORT);
                rtk_svlan_cfg.memberport |= transparent_mbp;
                rtk_svlan_cfg.untagport &= ~(1<<rtk_port);
                rtk_svlan_cfg.svid = port_vlan->mc_vlan[i].vid;
                ret=rtk_svlan_memberPortEntry_set(port_vlan->mc_vlan[i].vid, &rtk_svlan_cfg);
                if (ret !=CS_E_OK ){
                    goto END;
                }
            }
        }

        for(i = 0; i < __MC_VLAN_SWAP_PER_PORT_MAX; ++i)
        {
            if(port_vlan->mc_swap[i].valid)
            {
                /* svlan member */
                memset(&rtk_svlan_cfg, 0, sizeof(rtk_svlan_memberCfg_t));

                rtk_svlan_memberPortEntry_get(port_vlan->mc_swap[i].rule.s_vlan.vid, &rtk_svlan_cfg);
                rtk_svlan_cfg.memberport |= (1<<rtk_port);
                rtk_svlan_cfg.memberport |= (1<<SWITCH_UPLINK_PORT);
                rtk_svlan_cfg.memberport |= transparent_mbp;
                rtk_svlan_cfg.untagport |= (1<<rtk_port);
                rtk_svlan_cfg.svid = port_vlan->mc_swap[i].rule.s_vlan.vid;
                ret=rtk_svlan_memberPortEntry_set(port_vlan->mc_swap[i].rule.s_vlan.vid, &rtk_svlan_cfg);
                if (ret !=CS_E_OK ){
                    goto END;
                }
    
                /* cvlan member */
                rtk_mbr.bits[0] = 0;
                rtk_utg.bits[0] = 0;

                rtk_vlan_get(port_vlan->mc_swap[i].rule.c_vlan.vid, &rtk_mbr, &rtk_utg, &rtk_fid);
                rtk_mbr.bits[0] |= (1<<rtk_port);
                rtk_mbr.bits[0] |= (1<<SWITCH_UPLINK_PORT);
                rtk_utg.bits[0] |= (1<<SWITCH_UPLINK_PORT);
                ret=rtk_vlan_set(port_vlan->mc_swap[i].rule.c_vlan.vid, rtk_mbr, rtk_utg, 0);
                if (ret !=CS_E_OK ){
                    goto END;
                }
                
                /* s2c */
                ret=rtk_svlan_sp2c_add(port_vlan->mc_swap[i].rule.s_vlan.vid, rtk_port, port_vlan->mc_swap[i].rule.c_vlan.vid);
                if (ret !=CS_E_OK ){
                    goto END;
                }

                

                #if 0
                /* for u/s: c-vlan to vlan==4095, then drop, if there is no translation rule matched */
                if(!__rule_table_lookup(port_vlan->vlan_rule, __RULE_LOOKUP_CVLAN, &port_vlan->mc_swap[i].rule, &index))
                {
                    if(rtk_svlan_memberPortEntry_get(4095, &rtk_svlan_cfg))
                    {
                        memset(&rtk_svlan_cfg, 0, sizeof(rtk_svlan_memberCfg_t));
                    }
                    rtk_svlan_cfg.memberport |= (1<<rtk_port);
                    rtk_svlan_cfg.svid = 4095;
                    rtk_svlan_memberPortEntry_set(4095, &rtk_svlan_cfg);
                    rtk_svlan_c2s_add(port_vlan->mc_swap[i].rule.c_vlan.vid, rtk_port, 4095);
                }
                #endif
            }
        }
    }
    else
    {
        SDL_MIN_LOG("\nNot supported MC VLAN ACTION: %d\n", port_vlan->mc_act);
    }
END:
    return ret;

    
}

static cs_status __update_hw_uc(cs_port_id_t port_id)
{
    int i;
    rtk_portmask_t rtk_mbr, rtk_utg;
    rtk_svlan_memberCfg_t rtk_svlan_cfg;
    rtk_port_t rtk_port = L2P_PORT(port_id);
    __port_vlan_info_t *port_vlan = &__port_vlan_table[port_id];
    cs_uint16 index;
    cs_status   ret=0;
#if 0
    rtk_l2_ucastAddr_t l2_data;
    rtk_uint32 address = 0;
#endif
    cs_uint16 transparent_mbp = __get_transparent_mbp();
    cs_callback_context_t   context;

    memset(&rtk_svlan_cfg, 0, sizeof(rtk_svlan_memberCfg_t));
    ret=rtk_svlan_dmac_vidsel_set(rtk_port, EPON_FALSE);  /* disable d/s 1:N */
    if (ret !=CS_E_OK ){
        goto END;
    }

    /* delete mc-swap rule if it overlap with uc-svlan */
    if(!port_vlan->mc_vlan_en)
    {
        for(i = 0; i < __MC_VLAN_SWAP_PER_PORT_MAX; ++i)
        {
            if(port_vlan->mc_swap[i].valid)
            {
                if(__vlan_table_lookup(__s_vlan_table, port_vlan->mc_swap[i].rule.s_vlan.vid, &index))
                {
                    if(__s_vlan_table[index].vlan.mbr & (1<<rtk_port))
                    {
                        rtk_svlan_sp2c_del(port_vlan->mc_swap[i].rule.s_vlan.vid, rtk_port);
                    }
                }
            }
        }
    }
    /* END: delete mc-swap rule if it overlap with uc-svlan */

    /* sync sw S-vlan table to HW (including default vlan) */
    for(i = 0; i < __VLAN_MAX; ++i)
    {
        if(__s_vlan_table[i].valid)
        {
            memset(&rtk_svlan_cfg, 0, sizeof(rtk_svlan_memberCfg_t));
            
            /* bug #30714: only add current port into SVLAN */
            rtk_svlan_memberPortEntry_get(__s_vlan_table[i].vlan.vid, &rtk_svlan_cfg);
            rtk_svlan_cfg.svid = __s_vlan_table[i].vlan.vid;
            rtk_svlan_cfg.memberport |= ((1 << rtk_port) & __s_vlan_table[i].vlan.mbr);
            /* don't forget to add Uplink port */
            rtk_svlan_cfg.memberport |= ((1 << SWITCH_UPLINK_PORT) & __s_vlan_table[i].vlan.mbr);
            rtk_svlan_cfg.memberport |= transparent_mbp;
            if((1 << rtk_port) & __s_vlan_table[i].vlan.utg)
            {
                rtk_svlan_cfg.untagport |= (1 << rtk_port);
            }
            else
            {
                rtk_svlan_cfg.untagport &= ~(1 << rtk_port);
            }
            ret=rtk_svlan_memberPortEntry_set(__s_vlan_table[i].vlan.vid, &rtk_svlan_cfg);
            if (ret !=CS_E_OK ){
                goto END;
            }

        }
    }

    if(SDL_VLAN_MODE_TRANSPARENT != port_vlan->vlan_mode)
    {
        /* sync sw C-vlan table to HW (including default vlan) */
        for(i = 0; i < __VLAN_MAX; ++i)
        {
            rtk_mbr.bits[0] = 0;
            rtk_utg.bits[0] = 0;
            if( __c_vlan_table[i].valid )
            {
                rtk_mbr.bits[0] = __c_vlan_table[i].vlan.mbr;
                rtk_utg.bits[0] = __c_vlan_table[i].vlan.utg;
                ret=rtk_vlan_set(__c_vlan_table[i].vlan.vid, rtk_mbr, rtk_utg, 0);
                if (ret !=CS_E_OK ){
                    goto END;
                }

            }
        }
    }

    switch(port_vlan->vlan_mode)
    {
        case SDL_VLAN_MODE_TRANSPARENT:
            __transparent_hw_set(rtk_port, EPON_TRUE, ACCEPT_FRAME_TYPE_ALL);
            break;

        case SDL_VLAN_MODE_TAG:
            __transparent_hw_set(rtk_port, EPON_FALSE, ACCEPT_FRAME_TYPE_UNTAG_ONLY);   /* ingress accept type */
			// for up stream untag packet 
			#if 1
			{
				rtl8367b_user_vlan4kentry     vlan4K;
				memset(&vlan4K, 0, sizeof(rtl8367b_user_vlan4kentry));
				vlan4K.vid = port_vlan->def_vlan.vid;
			    vlan4K.fid_msti = 0;
				cs_uint16 index = 0;
				#if 0
				if(__vlan_table_lookup(__c_vlan_table, vlan4K.vid, &index))
				{
					vlan4K.mbr = __c_vlan_table[index].vlan.mbr;
					vlan4K.untag = __c_vlan_table[index].vlan.utg;
				}
				else
				{
					vlan4K.mbr = 0x40 | (0x1 << (L2P_PORT(port_id)));
					vlan4K.untag = vlan4K.mbr;
				}
				#else
				// modified by wujm@2016-04-13,merge new and original config
				vlan4K.mbr = 0x40 | (0x1 << (L2P_PORT(port_id)));
				vlan4K.untag = vlan4K.mbr;

				if(__vlan_table_lookup(__c_vlan_table, vlan4K.vid, &index))
				{
					vlan4K.mbr   |= __c_vlan_table[index].vlan.mbr;
					vlan4K.untag |= __c_vlan_table[index].vlan.utg;
				}
				#endif
			    rtl8367b_setAsicVlan4kEntry(&vlan4K);
			}
			
			#ifdef HAVE_GWD_VLAN_MODE
			//added by zhuxh,for gwd vlan tag mode, up stream packet with vlan 1 switch to vlan 0
			if(1 == port_vlan->def_vlan.vid)
			{
				rtk_svlan_c2s_add(port_vlan->def_vlan.vid, rtk_port, 0);
			}
			else
			{
				rtk_svlan_c2s_add(port_vlan->def_vlan.vid, rtk_port, port_vlan->def_vlan.vid);
			}
			#else
			//added by zhuxh,for ctc tag mode, up stream untag packet add default vlan tag
			rtk_svlan_c2s_add(port_vlan->def_vlan.vid, rtk_port, port_vlan->def_vlan.vid);
			#endif

			#endif
			
            break;

        case SDL_VLAN_MODE_TRANSLATION:
            __transparent_hw_set(rtk_port, EPON_FALSE, ACCEPT_FRAME_TYPE_ALL);               
			// for up stream untag packet 
			#if 1
			//added by zhuxh
			{
				rtl8367b_user_vlan4kentry     vlan4K;
				memset(&vlan4K, 0, sizeof(rtl8367b_user_vlan4kentry));
				vlan4K.vid = port_vlan->def_vlan.vid;
			    vlan4K.fid_msti = 0;
				cs_uint16 index = 0;
				#if 0
				if(__vlan_table_lookup(__c_vlan_table, vlan4K.vid, &index))
				{
					vlan4K.mbr = __c_vlan_table[index].vlan.mbr;
					vlan4K.untag = __c_vlan_table[index].vlan.utg;
				}
				else
				{
					vlan4K.mbr = 0x40 | (0x1 << (L2P_PORT(port_id)));
					vlan4K.untag = vlan4K.mbr;
				}
				#else
				// modified by wujm@2016-04-13,merge new and original config
				vlan4K.mbr = 0x40 | (0x1 << (L2P_PORT(port_id)));
				vlan4K.untag = vlan4K.mbr;
				
				if(__vlan_table_lookup(__c_vlan_table, vlan4K.vid, &index))
				{
					vlan4K.mbr   |= __c_vlan_table[index].vlan.mbr;
					vlan4K.untag |= __c_vlan_table[index].vlan.utg;
				}
				#endif
			    rtl8367b_setAsicVlan4kEntry(&vlan4K);
			}
			#endif
			
			for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
            {
                if(port_vlan->vlan_rule[i].valid)
                {
                    /* s2c */
					// added by wujm, if pvid is in translation table,do not translate from svlan to cvlan,
					// otherwise,vlan tag won't be removed from down stream tagged packet whose vlan tag is equal to pvid. 
					if(port_vlan->def_vlan.vid != port_vlan->vlan_rule[i].rule.c_vlan.vid)
                    {
                    	ret=rtk_svlan_sp2c_add(port_vlan->vlan_rule[i].rule.s_vlan.vid, rtk_port, port_vlan->vlan_rule[i].rule.c_vlan.vid);
					}
					else
					{
						ret = CS_E_OK;
					}
					
                    if (ret !=CS_E_OK ){
                        goto END;
                    }
                    /* c2s */
                    ret=rtk_svlan_c2s_add(port_vlan->vlan_rule[i].rule.c_vlan.vid, rtk_port, port_vlan->vlan_rule[i].rule.s_vlan.vid);
                    if (ret !=CS_E_OK ){
                        goto END;
                    }
                }
            }

			#ifdef HAVE_GWD_VLAN_MODE
			//added by zhuxh,for gwd vlan translation mode, up stream packet with vlan 1 switch to vlan 0
			if(1 == port_vlan->def_vlan.vid)
			{
				rtk_svlan_c2s_add(port_vlan->def_vlan.vid, rtk_port, 0);
			}
			else
			{
				rtk_svlan_c2s_add(port_vlan->def_vlan.vid, rtk_port, port_vlan->def_vlan.vid);
			}
			#endif
            break;

        case SDL_VLAN_MODE_AGGREGATION:
            __transparent_hw_set(rtk_port, EPON_FALSE, ACCEPT_FRAME_TYPE_ALL);   
            rtk_svlan_dmac_vidsel_set(rtk_port, EPON_TRUE); /* enable d/s 1:N */

			#if 1
			//added by zhuxh
			{
				rtl8367b_user_vlan4kentry     vlan4K;
				memset(&vlan4K, 0, sizeof(rtl8367b_user_vlan4kentry));
				vlan4K.vid = port_vlan->def_vlan.vid;
			    vlan4K.fid_msti = 0;
				cs_uint16 index = 0;
				#if 0
				if(__vlan_table_lookup(__c_vlan_table, vlan4K.vid, &index))
				{
					vlan4K.mbr = __c_vlan_table[index].vlan.mbr;
					vlan4K.untag = __c_vlan_table[index].vlan.utg;
				}
				else
				{
					vlan4K.mbr = 0x40 | (0x1 << (L2P_PORT(port_id)));
					vlan4K.untag = vlan4K.mbr;
				}
				#else
				// modified by wujm@2016-04-13,merge new and original config
				vlan4K.mbr = 0x40 | (0x1 << (L2P_PORT(port_id)));
				vlan4K.untag = vlan4K.mbr;

				if(__vlan_table_lookup(__c_vlan_table, vlan4K.vid, &index))
				{
					vlan4K.mbr   |= __c_vlan_table[index].vlan.mbr;
					vlan4K.untag |= __c_vlan_table[index].vlan.utg;
				}

				#endif
			    rtl8367b_setAsicVlan4kEntry(&vlan4K);
			}
			#endif

            for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
            {
                if(port_vlan->vlan_rule[i].valid)
                {
                    /* c2s */
                    ret=rtk_svlan_c2s_add(port_vlan->vlan_rule[i].rule.c_vlan.vid, rtk_port, port_vlan->vlan_rule[i].rule.s_vlan.vid);
                    if (ret !=CS_E_OK ){
                        goto END;
                    }
                }
            }
            /* clear dynamic entry from FDB */
            epon_request_onu_fdb_entry_clr_per_port(context,0,0,SDL_FDB_CLR_DYNAMIC,port_id);
			
			#ifdef HAVE_GWD_VLAN_MODE
			//added by zhuxh,for gwd vlan agg mode, up stream packet with vlan 1 switch to vlan 0
			if(1 == port_vlan->def_vlan.vid)
			{
				rtk_svlan_c2s_add(port_vlan->def_vlan.vid, rtk_port, 0);
			}
			else
			{
				rtk_svlan_c2s_add(port_vlan->def_vlan.vid, rtk_port, port_vlan->def_vlan.vid);
			}
			#else
			//added by zhuxh,for ctc trunk mode, up stream untag packet add default vlan tag
			rtk_svlan_c2s_add(port_vlan->def_vlan.vid, rtk_port, port_vlan->def_vlan.vid);
			#endif

            break;

        case SDL_VLAN_MODE_TRUNK:
            __transparent_hw_set(rtk_port, EPON_FALSE, ACCEPT_FRAME_TYPE_ALL);           
            for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
            {
                if(port_vlan->vlan_rule[i].valid)
                {
                    /* c2s */
                    ret=rtk_svlan_c2s_add(port_vlan->vlan_rule[i].rule.c_vlan.vid, rtk_port, port_vlan->vlan_rule[i].rule.c_vlan.vid);
                    if (ret !=CS_E_OK ){
                        goto END;
                    }
                }
            }

			#if 1
			//added by zhuxh
			{
				rtl8367b_user_vlan4kentry     vlan4K;
				memset(&vlan4K, 0, sizeof(rtl8367b_user_vlan4kentry));
				vlan4K.vid = port_vlan->def_vlan.vid;
			    vlan4K.fid_msti = 0;
				cs_uint16 index = 0;
				#if 0
				if(__vlan_table_lookup(__c_vlan_table, vlan4K.vid, &index))
				{
					vlan4K.mbr = __c_vlan_table[index].vlan.mbr;
					vlan4K.untag = __c_vlan_table[index].vlan.utg;
				}
				else
				{
					vlan4K.mbr = 0x4f;
					vlan4K.untag = 0x4f;
				}
				#else
				// modified by wujm@2016-04-13,merge new and original config
				vlan4K.mbr = 0x40 | (0x1 << (L2P_PORT(port_id)));
				vlan4K.untag = vlan4K.mbr;

				if(__vlan_table_lookup(__c_vlan_table, vlan4K.vid, &index))
				{
					vlan4K.mbr   |= __c_vlan_table[index].vlan.mbr;
					vlan4K.untag |= __c_vlan_table[index].vlan.utg;
				}

				#endif
			    rtl8367b_setAsicVlan4kEntry(&vlan4K);
			}

			#ifdef HAVE_GWD_VLAN_MODE
			//added by zhuxh,for gwd vlan trunk mode, up stream packet with vlan 1 switch to vlan 0
			if(1 == port_vlan->def_vlan.vid)
			{
				rtk_svlan_c2s_add(port_vlan->def_vlan.vid, rtk_port, 0);
			}
			else
			{
				rtk_svlan_c2s_add(port_vlan->def_vlan.vid, rtk_port, port_vlan->def_vlan.vid);
			}
			#else
			//added by zhuxh,for ctc trunk mode, up stream untag packet add default vlan tag
			rtk_svlan_c2s_add(port_vlan->def_vlan.vid, rtk_port, port_vlan->def_vlan.vid);
			#endif
			#endif
            break;

        case SDL_VLAN_MODE_STACKING:
            __transparent_hw_set(rtk_port, EPON_FALSE, ACCEPT_FRAME_TYPE_ALL);   /* ingress accept type */
            /* no vlan check and always add s-tag for upstream */
            rtk_vlan_portIgrFilterEnable_set(rtk_port, EPON_FALSE);
            __transparent_us_set(rtk_port, EPON_TRUE);
            break;

        default: break;
    }

    ret=rtk_svlan_defaultSvlan_set(rtk_port, port_vlan->def_vlan.vid); /* default vlan should be set after member set */
    if (ret !=CS_E_OK ){
        goto END;
    }
	#if 0
    ret=rtk_vlan_portPvid_set(rtk_port, 0, port_vlan->def_vlan.pri);
	#else
	//modified by zhuxh, The untagged frame received from the port will be classified to default VLAN and assigned to the default vlan priority.
	ret=rtk_vlan_portPvid_set(rtk_port, port_vlan->def_vlan.vid, port_vlan->def_vlan.pri);
	#endif

END:
    return ret;
    
}

static cs_status __update_sw
(
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_sdl_vlan_tag_t             def_vlan,   /* ignored for TRANSPARENT mode */
    CS_IN cs_sdl_vlan_mode_t            vlan_mode,
    CS_IN cs_sdl_vlan_cfg_t             *vlan_cfg,  /* ignored for TRANSPARENT mode */
    CS_IN cs_uint16                     cfg_nums    /* can be zero for modes */
)
{
    int i;
    cs_status ret=CS_E_OK;

    /* common for all modes */
    __port_vlan_table[port_id].vlan_mode = vlan_mode;
    __port_vlan_table[port_id].def_vlan.vid = def_vlan.vid;
    __port_vlan_table[port_id].def_vlan.pri = def_vlan.pri;
    __port_vlan_table[port_id].mc_vlan_en = EPON_FALSE;

    if(SDL_VLAN_MODE_TRANSPARENT != vlan_mode)
    {
        ret=__vlan_table_add_port(__s_vlan_table, def_vlan.vid, port_id, EPON_TRUE);    /* default vlan */
        if (ret !=CS_E_OK ){
            goto END;
        }
        ret=__vlan_table_add_port(__s_vlan_table, def_vlan.vid, CS_UPLINK_PORT, EPON_FALSE);
        if (ret !=CS_E_OK ){
            goto END;
        }   
    }

    if( (SDL_VLAN_MODE_TRANSLATION == vlan_mode) || (SDL_VLAN_MODE_AGGREGATION == vlan_mode) ) 
    {
        for(i=0; i<cfg_nums; ++i)
        {
            ret=__rule_table_add(__port_vlan_table[port_id].vlan_rule, __RULE_LOOKUP_ALL, &vlan_cfg[i]);
            if (ret !=CS_E_OK ){
               goto END;
            }
            
             ret=__vlan_table_add_port(__s_vlan_table, vlan_cfg[i].s_vlan.vid, port_id, EPON_TRUE);
             if (ret !=CS_E_OK ){
                goto END;
             }
            
             
             ret=__vlan_table_add_port(__s_vlan_table, vlan_cfg[i].s_vlan.vid, CS_UPLINK_PORT, EPON_FALSE);
              if (ret !=CS_E_OK ){
                 goto END;
              }
             ret=__vlan_table_add_port(__c_vlan_table, vlan_cfg[i].c_vlan.vid, port_id, EPON_FALSE);
             if (ret !=CS_E_OK ){
                 goto END;
             }
             
             ret=__vlan_table_add_port(__c_vlan_table, vlan_cfg[i].c_vlan.vid, CS_UPLINK_PORT, EPON_TRUE);
            
             if (ret !=CS_E_OK ){
                 goto END;
             }

        }
    }
    else if( SDL_VLAN_MODE_TRUNK == vlan_mode )
    {
		#if 1
		//added by zhuxh,solved bug:up stream untag packet cann't transmit to olt
		__vlan_table_add_port(__c_vlan_table,def_vlan.vid, port_id, EPON_TRUE);
		__vlan_table_add_port(__c_vlan_table,def_vlan.vid, CS_UPLINK_PORT, EPON_TRUE);
		#endif
        for(i=0; i<cfg_nums; ++i)
        {
            ret=__rule_table_add(__port_vlan_table[port_id].vlan_rule, __RULE_LOOKUP_CVLAN, &vlan_cfg[i]);
            if (ret !=CS_E_OK ){
                goto END;
            }

            ret=__vlan_table_add_port(__s_vlan_table, vlan_cfg[i].c_vlan.vid, port_id, EPON_FALSE);

            if (ret !=CS_E_OK ){
                goto END;
            }

            
            ret=__vlan_table_add_port(__s_vlan_table, vlan_cfg[i].c_vlan.vid, CS_UPLINK_PORT, EPON_FALSE);
            if (ret !=CS_E_OK ){
                goto END;
            }
            ret=__vlan_table_add_port(__c_vlan_table, vlan_cfg[i].c_vlan.vid, port_id, EPON_FALSE);
            if (ret !=CS_E_OK ){
                goto END;
            }

            
            ret=__vlan_table_add_port(__c_vlan_table, vlan_cfg[i].c_vlan.vid, CS_UPLINK_PORT, EPON_TRUE);
            if (ret !=CS_E_OK ){
                 goto END;
            }

        }
    }
    /* added by wujm@2016-07-29,solved bug:up stream untag packet cann't transmit to olt */
    else if( SDL_VLAN_MODE_TAG == vlan_mode )
    {
        __vlan_table_add_port(__c_vlan_table,def_vlan.vid, port_id, EPON_TRUE);
        __vlan_table_add_port(__c_vlan_table,def_vlan.vid, CS_UPLINK_PORT, EPON_TRUE);
    }
    
    else{/* no need update sw table in transparent/stacking/tag mode */}

END:
    return ret;

    
}

static cs_status __update_hw(cs_port_id_t port_id)
{
    cs_status   ret=CS_E_OK;
    
    __port_vlan_info_t *port_vlan = &__port_vlan_table[port_id];

    if(port_vlan->mc_vlan_en)
    {
        ret =__update_hw_uc(port_id);
        if ( ret != CS_E_OK ){
            goto END;

        }
        ret=__update_hw_mc(port_id);
        if ( ret != CS_E_OK ){
            goto END;
        }
    }
    else
    {
        ret=__update_hw_mc(port_id);
        if ( ret != CS_E_OK ){
            goto END;
        }
        
        ret=__update_hw_uc(port_id);
        if ( ret != CS_E_OK ){
            goto END;
        }        
    }

END:
    return ret;

}

static cs_status __ext_qinq_cfg_check 
(
    cs_sdl_ext_qinq_cfg_t * qinq_cfg
)
{
    cs_status ret=CS_E_PARAM;

    if (qinq_cfg==NULL)
    {
        goto END;
    }

    if ( ((qinq_cfg->match_type > SDL_EXT_QINQ_MATCH_DST_IP_TYPE)
    && (qinq_cfg->match_type < SDL_EXT_QINQ_MATCH_SRC_MAC_TYPE))
    || ( qinq_cfg->match_type < SDL_EXT_QINQ_MATCH_ETHER_TYPE)
    || ( qinq_cfg->match_type >= SDL_EXT_QINQ_MATCH_END_TYPE))
    {
        goto END;
    }

    if (qinq_cfg->attach_vid > 4096)
    {
        goto END;
    }

    if ( ! ((qinq_cfg->tpid_type ==  SDL_EXT_QINQ_TPID_8100_TYPE)
    || (qinq_cfg->tpid_type ==  SDL_EXT_QINQ_TPID_9100_TYPE)
    || (qinq_cfg->tpid_type ==  SDL_EXT_QINQ_TPID_88A8_TYPE)))
    {
        goto END;
    }

    if (qinq_cfg->pri > 7)
    {
        goto END;
    }

    ret=CS_E_OK;

END:

    return ret;

}

static cs_status __ext_qinq_cfg_find 
(
    cs_sdl_ext_qinq_cfg_t  * qinq_cfg, 
    cs_uint32              * out_index 
)
{
    cs_int32        i=0;
    __ext_qinq_t  * entry=NULL;
    cs_status       ret=CS_E_OK;

    *out_index=0;

    for ( i=0; i< __EXT_QINQ_ENTRY_NUM_MAX ; i++ )
    {
        entry=&__ext_qinq_table[i];
        if ( entry->valid ==FALSE )
        {
            continue;
        }

        ret=memcmp( qinq_cfg, &entry->cfg, sizeof (cs_sdl_ext_qinq_cfg_t));
        if (ret ==0)
        {
            * out_index=i;
            ret=CS_E_OK;
            goto END;
        }
    }

    ret=CS_E_NOT_FOUND;

    END:
        
    return ret;

}

static cs_status __ext_qinq_update_entry_find 
(
    cs_sdl_ext_qinq_match_type_t    match_type,
    cs_uint64                       match_value,
    cs_int32                       * out_index
)
    {
    cs_int32                    i=0;
    __ext_qinq_t            *  entry=NULL;
    cs_sdl_ext_qinq_cfg_t   *  cfg=NULL;
    cs_status                  ret=CS_E_OK;

    *out_index=0;

    for ( i=0; i< __EXT_QINQ_ENTRY_NUM_MAX ; i++ )
    {
        entry=&__ext_qinq_table[i];
        cfg = &entry->cfg;
        if ( entry->valid ==FALSE )
        {
            continue;
        }

        if ( (match_type == cfg->match_type)
          &&( match_value ==cfg->match_value ) )
        {
            * out_index=i;
            ret=CS_E_OK;
            goto END;
        }
    }

    ret=CS_E_NOT_FOUND;

    END:
        
    return ret;

}


static cs_status __ext_qinq_cfg_add 
( 
    cs_sdl_ext_qinq_cfg_t  * qinq_cfg, 
    cs_int32               * out_index
)
{

    cs_int32                    i=0;
    __ext_qinq_t              * entry=NULL;
    cs_sdl_ext_qinq_cfg_t     * cfg=NULL;
    cs_status                   ret=CS_E_OK;

    *out_index=0;

    ret=CS_E_RESOURCE;

    for ( i=0; i< __EXT_QINQ_ENTRY_NUM_MAX ; i++ )
    {
        entry=&__ext_qinq_table[i];
        cfg = &entry->cfg;
        if ( entry->valid ==TRUE )
        {
            continue;
        }
        memcpy (cfg, qinq_cfg, sizeof (cs_sdl_ext_qinq_cfg_t ));
        entry->valid=TRUE;
        * out_index=i;
        ret=CS_E_OK;
        break;
    }

    return ret;

}

static cs_status __ext_qinq_cfg_update 
( 
    cs_sdl_ext_qinq_cfg_t  * qinq_cfg, 
    cs_int32                 index
    )
    {
    __ext_qinq_t          * entry=NULL;
    cs_status               ret=CS_E_OK;

    entry=&__ext_qinq_table[index];
    memcpy (&entry->cfg, qinq_cfg, sizeof (cs_sdl_ext_qinq_cfg_t) );

    return ret;
}


static cs_status __ext_qinq_cfg_del 
( 
    cs_int32  index 
)
{
    memset (&__ext_qinq_table[index], 0, sizeof (__ext_qinq_t));

    return CS_E_OK;
}

static cs_status __ext_up_rule_set 
(
    __ext_qinq_t             * entry, 
    cs_aal_cl_rule_cfg_t     * rule

)
{
    cs_sdl_ext_qinq_cfg_t    *  cfg=NULL;
    cs_uint32                   match_value=0;
    cs_uint8                 *  mac=NULL;
    cs_uint8                    l4_port_type=0;
    cs_aal_cl_ipv4_long_key_t * ip_key=NULL;
    cs_aal_cl_ipv4_long_key_msk_t * mask=NULL;

    ip_key= &rule->cl_key.ipv4_long_key;

    if (entry->valid==FALSE) {
        rule->valid=FALSE;
        memset (ip_key , 0, sizeof (cs_aal_cl_ipv4_long_key_t ));
        goto END;
    }

    cfg=&entry->cfg;

    mask=&ip_key->mask;
    match_value =(cs_uint32) cfg->match_value;

    switch (cfg->match_type)
    {
        case   SDL_EXT_QINQ_MATCH_ETHER_TYPE:
        mask->s.ev2pt=1;
        ip_key->ev2pt=match_value;
        break;

        case   SDL_EXT_QINQ_MATCH_CVLAN_TYPE:
        mask->s.vlanid=1;
        ip_key->vlanid=match_value;
        break;

        case   SDL_EXT_QINQ_MATCH_IP_PRO_TYPE:
        mask->s.ipprotocol=1;
        ip_key->ipprotocol=match_value;
        break;

        case   SDL_EXT_QINQ_MATCH_IP_TOS_TYPE:
        mask->s.ipdscp=1;
        ip_key->ipdscp=match_value;
        break;

        case   SDL_EXT_QINQ_MATCH_SRC_IP_TYPE:
        mask->s.srcip=1;
        ip_key->srcip=match_value;
        ip_key->srcipmask=32;
        break;

        case   SDL_EXT_QINQ_MATCH_DST_IP_TYPE:

        mask->s.dstip=1;
        ip_key->dstip=match_value;
        ip_key->dstipmask=32;
        break;

        /*below is option match type , now oam not support it*/
        case   SDL_EXT_QINQ_MATCH_SRC_MAC_TYPE:
        mask->s.mac_sa=1;
        mac=(cs_uint8 * )&cfg->match_value + 2;
        memcpy (ip_key->srcmac, mac, 6);
        break;


        case   SDL_EXT_QINQ_MATCH_DST_MAC_TYPE:
        mask->s.mac_da=1;
        mac=(cs_uint8 * )&cfg->match_value + 2 ;
        memcpy (ip_key->dstmac, mac, 6);
        break;

        case   SDL_EXT_QINQ_MATCH_COS_TYPE:
        mask->s.dot1p=1;
        ip_key->dot1p=match_value;

        break;

        case   SDL_EXT_QINQ_MATCH_L4_SRC_PORT_TYPE:
        case   SDL_EXT_QINQ_MATCH_L4_DST_PORT_TYPE:
        mask->s.l4_port=1;

        if (cfg->match_type ==SDL_EXT_QINQ_MATCH_L4_SRC_PORT_TYPE)
        {
        l4_port_type=1;

        }else 
        {
        l4_port_type=0;
        }

        ip_key->l4_port=match_value;
        ip_key->l4_port_type=l4_port_type;
        break;
        
        default:
        break;

    }

    rule->valid=TRUE;
    rule->key_type=AAL_CL_IPV4_LONG_KEY;

    END:

    return CS_E_OK;

}


static cs_status __ext_up_fib_set 
(
    __ext_qinq_t               *     entry,
    cs_aal_cl_fib_data_t       *     fib
)
{
    cs_aal_cl_tpid_sel_t          top_tpid_sel=0;
    cs_sdl_ext_qinq_cfg_t       * cfg=NULL;

    if (entry->valid ==FALSE ) 
    {
        memset (fib, 0, sizeof (cs_aal_cl_fib_data_t));
        goto END;
    }

    cfg= &entry->cfg;

    switch (cfg->tpid_type)
    {
        case   SDL_EXT_QINQ_TPID_8100_TYPE:
        top_tpid_sel= AAL_CL_TPID_SEL_8100;
        break;

        case   SDL_EXT_QINQ_TPID_9100_TYPE:
        top_tpid_sel=AAL_CL_TPID_SEL_USER_DEF2;
        break;

        case   SDL_EXT_QINQ_TPID_88A8_TYPE:
        top_tpid_sel=AAL_CL_TPID_SEL_88A8;
        break;

        default:
        break;
    }

    fib->mask.s.vlan_op	  = 1;
    fib->mask.s.dot1p	  = 1;

    fib->valid=TRUE;
    fib->permit=1;
    fib->permit_pri=0;
    fib->permit_filter_dis=0;

    fib->inner_vlan_cmd=AAL_CL_VLAN_CMD_NOP;
    fib->top_vlan_cmd=AAL_CL_VLAN_CMD_PUSH;
    fib->top_vlanid_sel=AAL_CL_VLANID_SEL_CL;
    fib->top_vlanid=cfg->attach_vid;
    fib->top_8021p_sel=AAL_CL_DOT1P_SEL_CL;
    fib->top_8021p=cfg->pri;
    fib->top_tpid_sel=top_tpid_sel;
    fib->eg_chk_vlan_sel=AAL_CL_EG_VLAN_CHECK_NO;

    END:

    return CS_E_OK;

}

static cs_status __ext_down_rule_set 
(
    __ext_qinq_t                * entry,
    cs_aal_cl_rule_cfg_t        * rule
)
    {
    cs_uint32                             match_value=0;
    cs_aal_cl_ipv4_long_key_msk_t       * mask=NULL;
    cs_aal_cl_ipv4_long_key_t           * ip_key=NULL;
    cs_aal_cl_vlan_range_key_t         * vlan_key=NULL;
    cs_aal_cl_vlan_range_key_msk_t     * vlan_mask=NULL;
    cs_uint8                           * mac=NULL;
    cs_uint8                            l4_port_type=0;
    cs_sdl_ext_qinq_cfg_t              *   cfg=NULL;

    ip_key=(cs_aal_cl_ipv4_long_key_t  *)&rule->cl_key.ipv4_long_key;
    vlan_key=(cs_aal_cl_vlan_range_key_t * )&rule->cl_key.vlan_range_key[0];

    if (entry->valid ==FALSE ) 
    {
        rule->valid=FALSE;
        memset (&rule->cl_key, 0, sizeof (rule->cl_key));
        goto END;
    }

    cfg= &entry->cfg;
    mask=&ip_key->mask;
    vlan_mask=&vlan_key->mask;
    match_value= (cs_uint32 )cfg->match_value;

    switch (cfg->match_type)
    {
    case    SDL_EXT_QINQ_MATCH_ETHER_TYPE:
    mask->s.ev2pt=1;
    ip_key->ev2pt=match_value;
    break;

    case    SDL_EXT_QINQ_MATCH_CVLAN_TYPE:
    vlan_mask->s.vlan=1;
    vlan_mask->s.vlan_count=1;
    vlan_key->vlanid_hi=cfg->attach_vid;
    vlan_key->vlanid_lo=match_value;
    vlan_key->inner_vlan_en=TRUE;
    vlan_key->vlan_count=2;
    break;

    case    SDL_EXT_QINQ_MATCH_IP_PRO_TYPE:
    mask->s.ipprotocol=1;
    ip_key->ipprotocol=match_value;
    break;

    case    SDL_EXT_QINQ_MATCH_IP_TOS_TYPE:
    mask->s.ipdscp=1;
    ip_key->ipdscp=match_value;
    break;

    case    SDL_EXT_QINQ_MATCH_SRC_IP_TYPE:
    /*inverse src and dst ip*/
    mask->s.dstip=1;
    ip_key->dstip=match_value;
    ip_key->dstipmask=32;
    break;

    case    SDL_EXT_QINQ_MATCH_DST_IP_TYPE:
    /*inverse src and dst ip*/
    mask->s.srcip=1;
    ip_key->srcip=match_value;
    ip_key->srcipmask=32;
    break;

    /*below is option match type , now oam not support it*/
    case    SDL_EXT_QINQ_MATCH_SRC_MAC_TYPE:
    /*inverse src and dst mac */
    mask->s.mac_da=1;
    mac=(cs_uint8 * )&cfg->match_value + 2 ;
    memcpy (ip_key->dstmac, mac, 6);
    break;


    case    SDL_EXT_QINQ_MATCH_DST_MAC_TYPE:
    /*inverse src and dst mac */
    mask->s.mac_sa=1;
    mac=(cs_uint8 * )&cfg->match_value + 2;
    memcpy (ip_key->srcmac, mac, 6);
    break;

    case   SDL_EXT_QINQ_MATCH_COS_TYPE:
    mask->s.dot1p=1;
    ip_key->dot1p=match_value;

    /*it is workround, now no key will match  802.1p in inner vlan*/
    mask->s.vlan_count=1;
    ip_key->vlan_count=2;

    mask->s.vlanid=1;
    ip_key->vlanid=cfg->attach_vid;
    break;

    case     SDL_EXT_QINQ_MATCH_L4_SRC_PORT_TYPE:
    case     SDL_EXT_QINQ_MATCH_L4_DST_PORT_TYPE:
    mask->s.l4_port=1;

    if (cfg->match_type ==SDL_EXT_QINQ_MATCH_L4_DST_PORT_TYPE)
    {
    l4_port_type=1;

    }else 
    {
    l4_port_type=0;
    }

    ip_key->l4_port=match_value;
    ip_key->l4_port_type=l4_port_type;
    break;

    default:
    break;

    }

    if ( cfg->match_type == SDL_EXT_QINQ_MATCH_CVLAN_TYPE )
    {
        rule->key_type= AAL_CL_VLAN_RANGE_KEY;
    }else 
    {
        rule->key_type= AAL_CL_IPV4_LONG_KEY;
        mask->s.vlanid=1;
        ip_key->vlanid=cfg->attach_vid;
    }
    rule->valid=TRUE;

    END:
    return CS_E_OK;

}


static cs_status __ext_down_fib_set 
(
    __ext_qinq_t                *     entry,
    cs_aal_cl_fib_data_t        *     fib
)
{
    cs_aal_cl_tpid_sel_t          top_tpid_sel=0;
    cs_sdl_ext_qinq_cfg_t   *     cfg=NULL;

    if (entry->valid ==FALSE ) 
    {
        memset (fib, 0, sizeof (cs_aal_cl_fib_data_t));
        goto END;
    }

    cfg= &entry->cfg;

    switch (cfg->tpid_type)
    {
        case    SDL_EXT_QINQ_TPID_8100_TYPE:
                top_tpid_sel= AAL_CL_TPID_SEL_8100;
                break;

        case    SDL_EXT_QINQ_TPID_9100_TYPE:
                top_tpid_sel=AAL_CL_TPID_SEL_USER_DEF2;
                break;

        case    SDL_EXT_QINQ_TPID_88A8_TYPE:
                top_tpid_sel=AAL_CL_TPID_SEL_88A8;
                break;

                default:
                break;
    }

    fib->mask.s.vlan_op	  = 1;
    fib->mask.s.dot1p	  = 1;

    fib->valid=TRUE;
    fib->permit=1;
    fib->permit_pri=0;
    fib->permit_filter_dis=0;

    fib->inner_vlan_cmd=AAL_CL_VLAN_CMD_NOP;
    fib->top_vlan_cmd=AAL_CL_VLAN_CMD_POP;
    fib->eg_chk_vlan_sel=AAL_CL_EG_VLAN_CHECK_NO;

END:
    return CS_E_OK;

}



static cs_status __ext_qinq_entry_hw_set
( 
    cs_uint32 index 
)
{
    cs_aal_cl_rule_cfg_t            rule;
    cs_aal_cl_fib_data_t            fib;
    __ext_qinq_t                 *  entry=NULL;
    cs_sdl_ext_qinq_cfg_t        *  cfg=NULL;
    cs_uint32                       offset=0;
    cs_status                       ret=CS_E_OK;

    memset (&rule, 0,sizeof (cs_aal_cl_rule_cfg_t ));
    memset (&fib, 0,sizeof (cs_aal_cl_fib_data_t ));

    entry=&__ext_qinq_table[index];
    cfg=&entry->cfg;

    offset=__EXT_QINQ_ENTRY_NUM_MAX-1 - index;

    /*upstream key set*/
    ret=__ext_up_rule_set (entry, &rule);
    ret=__ext_up_fib_set (entry, &fib);


    aal_cl_fib_set( AAL_PORT_ID_GE,   DOMAIN_ID_EXT_QINQ, offset*8 + 0, &fib);
    aal_cl_rule_set(AAL_PORT_ID_GE,  DOMAIN_ID_EXT_QINQ, offset, & rule);

    memset (&rule, 0,sizeof (cs_aal_cl_rule_cfg_t ));
    memset (&fib, 0,sizeof (cs_aal_cl_fib_data_t ));

    ret=__ext_down_rule_set ( entry, &rule);
    ret=__ext_down_fib_set (entry, &fib);

    ret=aal_cl_fib_set(  AAL_PORT_ID_PON,	DOMAIN_ID_EXT_QINQ, offset*8 + 0, & fib);

    ret +=aal_cl_rule_set(	AAL_PORT_ID_PON,  DOMAIN_ID_EXT_QINQ, offset, & rule);

    return ret;

}

static cs_status __ext_qinq_table_hw_update 
( 
    cs_uint32 del_index
)
{
    cs_int32        i=0;
    __ext_qinq_t  * entry=NULL;
    cs_status       ret=CS_E_OK;

    /*move entry*/
    for ( i=del_index; i< __EXT_QINQ_ENTRY_NUM_MAX  ; i++ )
    {
        entry=&__ext_qinq_table[i];
        if ( i <  __EXT_QINQ_ENTRY_NUM_MAX-1 )
        {
            memcpy (entry, entry+1, sizeof (__ext_qinq_t)); 
            __ext_qinq_cfg_del ( i+1);
        } else {
            __ext_qinq_cfg_del ( i);
        }

        ret=__ext_qinq_entry_hw_set (i);
        if (ret !=CS_E_OK )
        {
            goto END;
        }

        if ( entry->valid ==FALSE )
        {
            /*reach to the last entry*/
            break;
        }
    }

    END:

    return ret;

}



void sdl_sw_vlan_dump(void)
{
    int i,j;
    rtl8367b_svlan_memconf_t svlanMemConf;
    rtl8367b_user_vlan4kentry vlan4K;
    rtl8367b_svlan_s2c_t svlanSP2CConf;
    rtk_uint32 evid, portmask, svidx;

    cs_printf("\nsw c-vlan table: \n");
    for(i = 0; i < __VLAN_MAX; ++i)
    {
        if(__c_vlan_table[i].valid)
        {
            cs_printf("vid = %d, mbr = %x, utg = %x\n", __c_vlan_table[i].vlan.vid, __c_vlan_table[i].vlan.mbr, __c_vlan_table[i].vlan.utg);
        }
    }
    
    cs_printf("\nsw s-vlan table: \n");
    for(i = 0; i < __VLAN_MAX; ++i)
    {
        if(__s_vlan_table[i].valid)
        {
            cs_printf("vid = %d, mbr = %x, utg = %x\n", __s_vlan_table[i].vlan.vid, __s_vlan_table[i].vlan.mbr, __s_vlan_table[i].vlan.utg);
        }
    }
    
    cs_printf("\nsw port vlan table: \n");
    for(i = 1; i <= UNI_PORT_MAX; ++i)
    {
        cs_printf("\nport %d:\n", i);
        cs_printf("def_vlan: %d, pri: %d\n", __port_vlan_table[i].def_vlan.vid, __port_vlan_table[i].def_vlan.pri);
        cs_printf("vlan_mode: %d\n", __port_vlan_table[i].vlan_mode);
        cs_printf("mc_vlan_en: %d\n", __port_vlan_table[i].mc_vlan_en);
        cs_printf("mc_act: %d\n", __port_vlan_table[i].mc_act);
        cs_printf("vlan_rule: \n");
        for(j = 0; j < __VLAN_RULE_PER_PORT_MAX; ++j)
        {
            if(__port_vlan_table[i].vlan_rule[j].valid)
            {
                cs_printf("s-vlan(%d)<==>(%d)c-vlan\n", __port_vlan_table[i].vlan_rule[j].rule.s_vlan.vid, __port_vlan_table[i].vlan_rule[j].rule.c_vlan.vid);
            }
        }
        cs_printf("mc_vlan: ");
        for(j = 0; j < __MC_VLAN_PER_PORT_MAX; ++j)
        {
            if(__port_vlan_table[i].mc_vlan[j].valid)
            {
                cs_printf("%d, ", __port_vlan_table[i].mc_vlan[j].vid);
            }
        }
        cs_printf("\n");
        cs_printf("mc_swap: \n");
        for(j = 0; j < __VLAN_RULE_PER_PORT_MAX; ++j)
        {
            if(__port_vlan_table[i].mc_swap[j].valid)
            {
                cs_printf("s-vlan(%d)<==>(%d)c-vlan\n", __port_vlan_table[i].mc_swap[j].rule.s_vlan.vid, __port_vlan_table[i].mc_swap[j].rule.c_vlan.vid);
            }
        }
    }

    cs_printf("\nhw svlan table:\n");
    for (i = 0; i<= RTL8367B_SVIDXMAX; i++) 
    {
        rtl8367b_getAsicSvlanMemberConfiguration(i, &svlanMemConf);
        if((svlanMemConf.vs_member!=0) || (svlanMemConf.vs_svid!=0))
            cs_printf("svid = %4d, mbp = 0x%02x, utg = 0x%02x\n", svlanMemConf.vs_svid, svlanMemConf.vs_member, svlanMemConf.vs_untag);
    }

    cs_printf("\nhw cvlan table:\n");
    for (i = 0; i<= 4095; i++) 
    {
        vlan4K.vid = i;
        rtl8367b_getAsicVlan4kEntry(&vlan4K);
        if(vlan4K.mbr!=0)
            cs_printf("cvid = %4d, mbp = 0x%02x, utg = 0x%02x\n", vlan4K.vid, vlan4K.mbr, vlan4K.untag);
    }

    cs_printf("\nhw s2c rule:\n");
    for (i = 0; i<= RTL8367B_SP2CMAX; i++) 
    {
        rtl8367b_getAsicSvlanSP2CConf(i, &svlanSP2CConf);
        rtl8367b_getAsicSvlanMemberConfiguration(svlanSP2CConf.svidx, &svlanMemConf);
        if(svlanSP2CConf.valid)
            cs_printf("svid(%4d) => port(%d) => (%4d)cvid\n", svlanMemConf.vs_svid, svlanSP2CConf.dstport, svlanSP2CConf.vid);
    }

    cs_printf("\nhw c2s rule:\n");
    for (i = 0; i<= RTL8367B_C2SIDXMAX; i++) 
    {
        rtl8367b_getAsicSvlanC2SConf(i, &evid, &portmask, &svidx);
        rtl8367b_getAsicSvlanMemberConfiguration(svidx, &svlanMemConf);
        if(portmask)
            cs_printf("cvid(%4d) => portmask(0x%02x) => (%4d)svid\n", evid, portmask, svlanMemConf.vs_svid);
    }
}

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
)
{
    int                   i=0;
    cs_status             ret=CS_E_OK;
    __vlan_cfg_info_t   * temp_vlan_cfg=NULL;
    cs_uint32             port_max_vlan_num=0;



    UNI_PORT_CHECK(port_id);
    __VLAN_MODE_CHECK(vlan_mode);

    if( (SDL_VLAN_MODE_TRANSLATION == vlan_mode)
    || (SDL_VLAN_MODE_AGGREGATION == vlan_mode) 
    || (SDL_VLAN_MODE_TRUNK == vlan_mode) )
    {
        __NULL_CHECK(vlan_cfg);

		//added by zhuxh, todo:make sure port_max_vlan_num
        if ( vlan_mode ==SDL_VLAN_MODE_TRUNK) {
            port_max_vlan_num=__VLAN_RULE_PER_PORT_MAX;
        }else {
            port_max_vlan_num=8;
        }

        if(cfg_nums > port_max_vlan_num)
        {
            ret= CS_E_PARAM;
            goto END;

        }


        for(i = 0; i < cfg_nums; ++i)
        {
        	cs_uint16 index = 0;
			
            VID_CHECK(vlan_cfg[i].s_vlan.vid);
            VID_CHECK(vlan_cfg[i].c_vlan.vid);

			// added by wujm@2016-04-13,check vid if already used by mc_vlan
			if(__mc_table_lookup(__port_vlan_table[port_id].mc_vlan, vlan_cfg[i].c_vlan.vid, &index))
        	{
				cs_printf("vid %d is already use in mc vlan,this may cause unknown error!\n",vlan_cfg[i].c_vlan.vid);
			}

			// added by wujm@2016-04-13,according to CTC protocol,pvid shouldn't in vlan rules
        	if(vlan_cfg[i].c_vlan.vid == def_vlan.vid
				|| vlan_cfg[i].s_vlan.vid == def_vlan.vid)
			{
				ret= CS_E_PARAM;
            	goto END;
			}
        }
    }

    
    temp_vlan_cfg=(__vlan_cfg_info_t * )iros_malloc(IROS_MID_SAL, sizeof (__vlan_cfg_info_t));
    if (temp_vlan_cfg ==NULL )
    {
        ret=CS_E_RESOURCE;
        goto END;

    }

    memset (temp_vlan_cfg, 0, sizeof (__vlan_cfg_info_t));

    memcpy (&(temp_vlan_cfg->port_vlan_table[0]), &__port_vlan_table[0], sizeof (__port_vlan_table));
    memcpy (&(temp_vlan_cfg->s_vlan_table[0]), &__s_vlan_table[0], sizeof (__s_vlan_table));
    memcpy (&(temp_vlan_cfg->c_vlan_table[0]), &__c_vlan_table[0], sizeof (__c_vlan_table));

    __clear_uc(port_id);


    ret=__update_sw(port_id, def_vlan, vlan_mode, vlan_cfg, cfg_nums );
    if (ret !=CS_E_OK ) {
        /*rollback sw cfg*/
        memcpy ( &__port_vlan_table[0], &(temp_vlan_cfg->port_vlan_table[0]), sizeof (__port_vlan_table) );
        memcpy ( &__s_vlan_table[0], &(temp_vlan_cfg->s_vlan_table[0]), sizeof (__s_vlan_table));
        memcpy ( &__c_vlan_table[0], &(temp_vlan_cfg->c_vlan_table[0]),sizeof (__c_vlan_table));
        __update_hw(port_id);
        goto END;
    }
    
    ret=__update_hw(port_id);
    if (ret !=CS_E_OK ) {        
        __clear_uc(port_id);
        memcpy ( &__port_vlan_table[0], &(temp_vlan_cfg->port_vlan_table[0]), sizeof (__port_vlan_table) );
        memcpy ( &__s_vlan_table[0], &(temp_vlan_cfg->s_vlan_table[0]), sizeof (__s_vlan_table));
        memcpy ( &__c_vlan_table[0], &(temp_vlan_cfg->c_vlan_table[0]),sizeof (__c_vlan_table));
        __update_hw(port_id);
        goto END;
    }


 END:
    if (temp_vlan_cfg  !=NULL )
    {
        iros_free (temp_vlan_cfg );
    }

    return ret;

    
}


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
)
{    
    int i;
    cs_uint16 vlan_cnt = 0;

    UNI_PORT_CHECK(port_id);
    __NULL_CHECK(def_vlan);
    __NULL_CHECK(vlan_mode);
    __NULL_CHECK(vlan_cfg);
    __NULL_CHECK(cfg_nums);
    
    *cfg_nums = 0;

    if( (SDL_VLAN_MODE_TRANSLATION == __port_vlan_table[port_id].vlan_mode) 
        || (SDL_VLAN_MODE_AGGREGATION == __port_vlan_table[port_id].vlan_mode) ) 
    {
        for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
        {
            if(__port_vlan_table[port_id].vlan_rule[i].valid)
            {
                vlan_cfg[vlan_cnt].s_vlan.vid = __port_vlan_table[port_id].vlan_rule[i].rule.s_vlan.vid;
                vlan_cfg[vlan_cnt].c_vlan.vid = __port_vlan_table[port_id].vlan_rule[i].rule.c_vlan.vid;
                ++vlan_cnt;
            }
        }
        *cfg_nums = vlan_cnt;
    }
    else if(SDL_VLAN_MODE_TRUNK == __port_vlan_table[port_id].vlan_mode)
    {
        for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
        {
            if(__port_vlan_table[port_id].vlan_rule[i].valid)
            {
                vlan_cfg[vlan_cnt++].c_vlan.vid = __port_vlan_table[port_id].vlan_rule[i].rule.c_vlan.vid;
            }
        }
        *cfg_nums = vlan_cnt;
    }
    else{ /* do nothing */ }
    
    *def_vlan = __port_vlan_table[port_id].def_vlan;
    *vlan_mode = __port_vlan_table[port_id].vlan_mode;

    return CS_E_OK;
}


#if 1
//added by zhuxh
cs_status epon_onu_sw_get_port_pvid
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_uint16            		*vid
)
{
     cs_sdl_vlan_tag_t			def_vlan;  /* ignored for TRANSPARENT mode */
     cs_sdl_vlan_mode_t			vlan_mode;
     cs_sdl_vlan_cfg_t			vlan_cfg[__VLAN_RULE_PER_PORT_MAX];   /* ignored for TRANSPARENT mode */
     cs_uint16					cfg_nums;    /* zero for TRANSPARENT/TAGGED mode */
     cs_status 					ret = CS_E_ERROR;

     if(epon_request_onu_vlan_get(context, device, llidport, port_id, &def_vlan, &vlan_mode, vlan_cfg, &cfg_nums) == CS_OK)
     {
    	 *vid = def_vlan.vid;
    	 ret = CS_E_OK;
     }

     return ret;
}
#endif

cs_status epon_request_onu_mc_vlan_add
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_uint16                     *vlan_id,
    CS_IN cs_uint16                     cfg_nums
)
{
    int i;
    cs_status rt = CS_E_OK;    
    cs_uint16 index;
    cs_uint16 used_num = 0;
    cs_uint16 new_num = 0;

    UNI_PORT_CHECK(port_id);

    if( (cfg_nums > __MC_VLAN_PER_PORT_MAX) || (0 == cfg_nums) )
    {
        return CS_E_PARAM;
    }

    __NULL_CHECK(vlan_id);

    for(i = 0; i < cfg_nums; ++i)
    {
        VID_CHECK(vlan_id[i]);
    }

    for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
    {
        if(__port_vlan_table[port_id].mc_vlan[i].valid)
        {
            ++used_num;
        }
    }

    for(i = 0; i < cfg_nums; ++i)
    {
        if(!__mc_table_lookup(__port_vlan_table[port_id].mc_vlan, vlan_id[i], &index))
        {
            ++new_num;
        }
    }

    if( (used_num + new_num) > __MC_VLAN_PER_PORT_MAX )
    {
        return CS_E_RESOURCE;
    }

    for(i = 0; i < cfg_nums; ++i)
    {
        // added by wujm,check vid if already used by c_vlan
		if(__vlan_table_lookup(__c_vlan_table, vlan_id[i], &index))
		{
			cs_printf("vid %d is already use in uc vlan,this may cause unknown error!\n",vlan_id[i]);
		}
		__mc_table_add(__port_vlan_table[port_id].mc_vlan, vlan_id[i]);
    }

    __port_vlan_table[port_id].mc_vlan_en = EPON_TRUE;

    rt=__update_hw(port_id);


    return rt;
}

cs_status epon_request_onu_mc_vlan_del
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_uint16                     *vlan_id,
    CS_IN cs_uint16                     cfg_nums
)
{
    int i;
    cs_status rt = CS_E_OK;    
    cs_uint16 index;

    UNI_PORT_CHECK(port_id);

    if( (cfg_nums > __MC_VLAN_PER_PORT_MAX) || (0==cfg_nums) )
    {
        return CS_E_PARAM;
    }

    __NULL_CHECK(vlan_id);

    for(i = 0; i < cfg_nums; ++i)
    {
        VID_CHECK(vlan_id[i]);
    }

    for(i = 0; i < cfg_nums; ++i)
    {
        __mc_table_del(__port_vlan_table[port_id].mc_vlan, vlan_id[i]);

        if(!__vlan_table_lookup(__s_vlan_table, vlan_id[i], &index))
        {
            __remove_port_from_svlan(vlan_id[i], L2P_PORT(port_id));
        }
		// added by wujm,delete cvlan when delete mc vlan
		if(!__vlan_table_lookup(__c_vlan_table, vlan_id[i], &index))
        {
			__remove_port_from_cvlan(vlan_id[i], L2P_PORT(port_id));
        }

		rtk_svlan_c2s_del(vlan_id[i], L2P_PORT(port_id));
    }

    __update_hw(port_id);

    return rt;
}

cs_status epon_request_onu_mc_vlan_get
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_uint16                    *vlan_id,
    CS_OUT cs_uint16                    *cfg_nums
)
{
    int i;
    cs_uint16 vlan_cnt = 0;

    UNI_PORT_CHECK(port_id);
    __NULL_CHECK(vlan_id);
    __NULL_CHECK(cfg_nums);

    for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
    {
        if(__port_vlan_table[port_id].mc_vlan[i].valid)
        {
            vlan_id[vlan_cnt++] = __port_vlan_table[port_id].mc_vlan[i].vid;
        }
    }

    *cfg_nums = vlan_cnt;

    return CS_E_OK;
}

cs_status epon_request_onu_mc_vlan_clr
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id
)
{
    int i; 

    UNI_PORT_CHECK(port_id);

    __clear_hw_mc_vlan(port_id);

    for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
    {
        __port_vlan_table[port_id].mc_vlan[i].valid = 0;
    }

	__port_vlan_table[port_id].mc_vlan_en = EPON_FALSE;
	
    __update_hw(port_id);

    return CS_E_OK;
}

cs_status epon_request_onu_mc_vlan_action_set
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_sdl_mc_vlan_act_t          vlan_act,
    CS_IN cs_sdl_vlan_cfg_t             *swap_rule,
    CS_IN cs_uint16                     rule_nums
)
{
    int i;

    UNI_PORT_CHECK(port_id);

    if(vlan_act >= SDL_MC_VLAN_TAG_END)
    {
        return CS_E_PARAM;
    }

    if(SDL_MC_VLAN_TAG_SWAP == __port_vlan_table[port_id].mc_act) /* old mode is swap, need to clear old swap rule */
    {
        __clear_hw_mc_swap(port_id);
        __rule_table_clr(__port_vlan_table[port_id].mc_swap);
    }

    if(SDL_MC_VLAN_TAG_SWAP == vlan_act)
    {
        __NULL_CHECK(swap_rule);
        if(rule_nums > __MC_VLAN_SWAP_PER_PORT_MAX)
        {
            return CS_E_PARAM;
        }

        for(i = 0; i < rule_nums; ++i)
        {
            VID_CHECK(swap_rule[i].s_vlan.vid);
            VID_CHECK(swap_rule[i].c_vlan.vid);
        }

        for(i = 0; i < rule_nums; ++i)
        {
            __rule_table_add(__port_vlan_table[port_id].mc_swap, __RULE_LOOKUP_ALL, &swap_rule[i]);
        }
    }

    __port_vlan_table[port_id].mc_act = vlan_act;
    __port_vlan_table[port_id].mc_vlan_en = EPON_TRUE;

    __update_hw(port_id);

    return CS_E_OK;
}

cs_status epon_request_onu_mc_vlan_action_get
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_mc_vlan_act_t          *vlan_act,
    CS_OUT cs_sdl_vlan_cfg_t             *swap_rule,
    CS_OUT cs_uint16                     *rule_nums
)
{
    int i;
    cs_uint16 rule_cnt = 0;

    UNI_PORT_CHECK(port_id);
    __NULL_CHECK(vlan_act);
    
    if(SDL_MC_VLAN_TAG_SWAP == __port_vlan_table[port_id].mc_act)
    {
        __NULL_CHECK(swap_rule);
        __NULL_CHECK(rule_nums);
        
        for(i = 0; i < __MC_VLAN_SWAP_PER_PORT_MAX; ++i)
        {
            if(__port_vlan_table[port_id].mc_swap[i].valid)
            {
                swap_rule[rule_cnt].s_vlan.vid = __port_vlan_table[port_id].mc_swap[i].rule.s_vlan.vid;
                swap_rule[rule_cnt].c_vlan.vid = __port_vlan_table[port_id].mc_swap[i].rule.c_vlan.vid;
                ++rule_cnt;
            }
        }

        *rule_nums = rule_cnt;
    }

    *vlan_act = __port_vlan_table[port_id].mc_act;

    return CS_E_OK;
}




cs_status epon_request_onu_ext_qinq_add
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_sdl_ext_qinq_cfg_t        *qinq_cfg
)
{
    cs_int32        index=0;
    cs_status       ret=CS_E_OK;

    ret=__ext_qinq_cfg_check ( qinq_cfg);
    if (ret !=CS_E_OK)
    {
        goto END;
    }

    /*check this same oam cfg */
    ret=__ext_qinq_cfg_find( qinq_cfg, &index);
    if (ret ==CS_E_OK )
    {
        /* this entry exist, so do nothing*/
        goto END;
    }

    /*check whether the same math type and match_value
    if existed, update this entry*/
    ret=__ext_qinq_update_entry_find (qinq_cfg->match_type,qinq_cfg->match_value ,&index);
    if (ret ==CS_E_OK )
    {
        /*update the qinq entry*/
        __ext_qinq_cfg_update (qinq_cfg, index);
    } else {
        /*new cfg, so add it into qinq software table*/
        ret=__ext_qinq_cfg_add ( qinq_cfg, &index);
        if (ret !=CS_E_OK )
        {
            /*qinq table is full*/
            goto END;
        }
    }

    /*update the hw*/
    ret=__ext_qinq_entry_hw_set ( index);
    if ( ret !=CS_E_OK)
    {
        __ext_qinq_cfg_del (index);
    }

    END:
        
    return ret;

}

cs_status epon_request_onu_ext_qinq_del
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_IN cs_sdl_ext_qinq_cfg_t	        *qinq_cfg
)
{
    cs_uint32       index=0;
    cs_status       ret=CS_E_OK;

    ret=__ext_qinq_cfg_check ( qinq_cfg);
    if (ret !=CS_E_OK)
    {
        goto END;
    }

    /*whether this cfg  exist*/
    ret=__ext_qinq_update_entry_find(qinq_cfg->match_type,qinq_cfg->match_value ,&index);
    if (ret != CS_OK )
    {
        /* this entry not exist, so do nothing*/
        goto END;
    }

    /*del this entry from the table  */
    //__ext_qinq_cfg_del ( index);

    /*update the entire sw and hw table*/
    ret=__ext_qinq_table_hw_update (index);

    END:

    return ret;


}


cs_status epon_request_onu_ext_qinq_get
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_dev_id_t                   device,
    CS_IN cs_llid_t                     llidport,
    CS_OUT cs_sdl_ext_qinq_cfg_t	    *qinq_cfg_table,/*it is array*/
    CS_OUT cs_int32                     *table_len
)
{

    cs_int32        i=0;
    __ext_qinq_t  * entry=NULL;
    cs_status       ret=CS_E_OK;

    *table_len=0;

    for ( i=0; i< __EXT_QINQ_ENTRY_NUM_MAX ; i++ )
    {
        entry=&__ext_qinq_table[i];
        if ( entry->valid ==FALSE )
        {
            continue;
        }

        memcpy (qinq_cfg_table+i, &entry->cfg, sizeof (cs_sdl_ext_qinq_cfg_t));
        (*table_len)++;
    }

    return ret;

}







cs_status sdl_vlan_init(void)
{
    cs_port_id_t                  port;
    cs_callback_context_t         context;
    cs_sdl_vlan_tag_t             def_vlan;
    rtl8367b_user_vlan4kentry     vlan4K;
    rtl8367b_vlanconfiguser       vlanMC;
    rtk_portmask_t                rtk_mbp;
    int i;

    memset (__ext_qinq_table, 0, sizeof (__ext_qinq_table));

    rtk_svlan_servicePort_add(SWITCH_UPLINK_PORT);
    rtk_svlan_tpidEntry_set(0x8100);
    rtk_svlan_priorityRef_set(REF_INTERNAL_PRI);

    vlanMC.evid = __US_TRANSPARENT_EVID;
    vlanMC.vbpen = 0;
    vlanMC.vbpri = 0;
    vlanMC.mbr= 0x4f;
    vlanMC.envlanpol = 0;
    vlanMC.meteridx = 0;
    vlanMC.fid_msti = 0;
    rtl8367b_setAsicVlanMemberConfig(__US_TRANSPARENT_EVID_INDEX, &vlanMC);

    /* Set a default VLAN with vid 0 to 4K table for all ports */
    memset(&vlan4K, 0, sizeof(rtl8367b_user_vlan4kentry));
    vlan4K.vid = 0;
    vlan4K.mbr = RTK_MAX_PORT_MASK;
    vlan4K.untag = RTK_MAX_PORT_MASK;
    vlan4K.fid_msti = 0;
    rtl8367b_setAsicVlan4kEntry(&vlan4K);

    /* Enable downstream transparent for double-tagged frames */
    rtk_mbp.bits[0] = (1<<SWITCH_UPLINK_PORT);
    for(i = 0; i < 4; ++i)
    {
        rtk_vlan_transparent_set(i, rtk_mbp, EPON_TRUE);
    }

#ifdef HAVE_GWD_VLAN_MODE
	//modefied by zhuxh
	/* trunk for all ports */
	cs_sdl_vlan_cfg_t cfg;
    def_vlan.vid = 1;
	bzero(&cfg, sizeof(cfg));
    for(port = CS_UNI_PORT_ID1; port <=UNI_PORT_MAX; ++port)
    {
        epon_request_onu_vlan_set(context,0,0, port, def_vlan,SDL_VLAN_MODE_TRUNK,&cfg, 0);	 //�˿�VLANģʽ:TRUNK,Ĭ��vlan :1
    }
#else
	/* transparent for all ports */
	def_vlan.vid = 0;
	for(port = CS_UNI_PORT_ID1; port <=UNI_PORT_MAX; ++port)
	{
		epon_request_onu_vlan_set(context,0,0, port, def_vlan, SDL_VLAN_MODE_TRANSPARENT, NULL, 0);
	}
#endif

    SDL_MAJ_LOG("sdl vlan initialized\n");
    
    return CS_E_OK;

}

// added by wujm
cs_status epon_request_vlan_add_tag_port (
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_port_id_t                  port,
    CS_IN  cs_uint16                     vid
    
)
{
	cs_status                     ret = 0;
	cs_sdl_vlan_tag_t  			  def_vlan;
	cs_sdl_vlan_mode_t 			  vlan_mode = -1;
	cs_sdl_vlan_cfg_t  	          vlan_cfg[__VLAN_RULE_PER_PORT_MAX];
	cs_uint16		   			  cfg_nums = __VLAN_RULE_PER_PORT_MAX;

	memset(&def_vlan,0,sizeof(cs_sdl_vlan_tag_t));
	memset(&vlan_cfg,0,sizeof(cs_sdl_vlan_cfg_t)*__VLAN_RULE_PER_PORT_MAX);
	
	UNI_PORT_CHECK(port);

	if(CS_E_OK == (ret = epon_request_onu_vlan_get(context, device_id, llidport,port,&def_vlan,&vlan_mode,vlan_cfg,&cfg_nums)))
	{
		int i = 0;
		// ���˿��Ƿ���tagged�ڸ�VLAN
		for(i = 0;i < cfg_nums;i++)
		{
			if(vlan_cfg[i].c_vlan.vid == vid)
			{
				return CS_E_OK;
			}		
		}

		// ����ö˿�ԭ��untagged�ڸ�VLAN��untagged��VLAN 1
		if(vid == def_vlan.vid)
		{
			def_vlan.vid = 1;
		}
		
		// ���˿�tagged����VLAN
		if(cfg_nums < __VLAN_RULE_PER_PORT_MAX)
		{
			vlan_cfg[cfg_nums].c_vlan.vid = vid;
			vlan_cfg[cfg_nums].s_vlan.vid = vid;
			cfg_nums++;	
			ret = epon_request_onu_vlan_set(context, device_id, llidport,port,def_vlan,vlan_mode,vlan_cfg,cfg_nums);
		}
		else
		{
			ret = CS_E_ERROR;
		}
	}

	return ret;
}

// added by wujm
cs_status epon_request_vlan_add_untag_port (
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_port_id_t                  port,
    CS_IN  cs_uint16                     vid
    
)
{
	cs_status                     ret = 0;
	cs_sdl_vlan_tag_t  			  def_vlan;
	cs_sdl_vlan_mode_t 			  vlan_mode = -1;
	cs_sdl_vlan_cfg_t  	          vlan_cfg[__VLAN_RULE_PER_PORT_MAX];
	cs_uint16		   			  cfg_nums = __VLAN_RULE_PER_PORT_MAX;
	cs_uint16					  new_cfg_nums = 0;
	
	memset(&def_vlan,0,sizeof(cs_sdl_vlan_tag_t));
	memset(&vlan_cfg,0,sizeof(cs_sdl_vlan_cfg_t)*__VLAN_RULE_PER_PORT_MAX);
	
	UNI_PORT_CHECK(port);

	if(CS_E_OK == (ret = epon_request_onu_vlan_get(context, device_id, llidport,port,&def_vlan,&vlan_mode,vlan_cfg,&cfg_nums)))
	{
		int i = 0;
		
		new_cfg_nums = cfg_nums;

		// ���˿��Ƿ���untagged�ڸ�VLAN
		if(vid == def_vlan.vid)
		{
			return CS_E_OK;
		}

		// ����ö˿�ԭ��tagged�ڸ�VLAN��ȥ��
		for(i = 0;i < cfg_nums;i++)
		{
			if(vlan_cfg[i].c_vlan.vid == vid)
			{
				memmove(&vlan_cfg[i],&vlan_cfg[i+1],sizeof(cs_sdl_vlan_cfg_t)*(cfg_nums-1));

				new_cfg_nums = cfg_nums - 1;
				break;
			}		
		}

		def_vlan.vid = vid;

		ret = epon_request_onu_vlan_set(context, device_id, llidport,port,def_vlan,vlan_mode,vlan_cfg,new_cfg_nums);

	}

	return ret;
}

cs_status epon_request_vlan_get (
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_uint16                     vid,
    CS_OUT cs_sdl_vlan_t                 *vlan
    
)
{
	int i = 0;

	if(NULL == vlan || 0 == vid) {
        return CS_E_PARAM;
    }
	
	for(i = 0; i < __VLAN_MAX; ++i)
    {
        if(__c_vlan_table[i].valid && __c_vlan_table[i].vlan.vid == vid)
        {
        	memcpy(vlan,&(__c_vlan_table[i].vlan),sizeof(cs_sdl_vlan_t));
			return CS_E_OK;
        }
		
	}

    return CS_E_NOT_FOUND;
}

cs_status epon_request_vlan_del_from_port (
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_port_id_t                  port,
    CS_IN  cs_uint16                     vid
    
)
{
	cs_status                     ret = 0;
	cs_sdl_vlan_tag_t  			  def_vlan;
	cs_sdl_vlan_mode_t 			  vlan_mode = -1;
	cs_sdl_vlan_cfg_t  	          vlan_cfg[__VLAN_RULE_PER_PORT_MAX];
	cs_uint16		   			  cfg_nums = __VLAN_RULE_PER_PORT_MAX;

	memset(&def_vlan,0,sizeof(cs_sdl_vlan_tag_t));
	memset(&vlan_cfg,0,sizeof(cs_sdl_vlan_cfg_t)*__VLAN_RULE_PER_PORT_MAX);
	
	UNI_PORT_CHECK(port);

	if(CS_E_OK == (ret = epon_request_onu_vlan_get(context, device_id, llidport,port,&def_vlan,&vlan_mode,vlan_cfg,&cfg_nums)))
	{
		int i = 0;
		
		// ��vlan_cfg��ɾ��
		for(i = 0;i < cfg_nums;i++)
		{
			if(vlan_cfg[i].c_vlan.vid == vid)
			{
				memmove(&vlan_cfg[i],&vlan_cfg[i+1],sizeof(cs_sdl_vlan_cfg_t)*(cfg_nums-1));

				cfg_nums--;	
				break;
			}		
		}

		// ��def_vlan��ɾ��
		if(def_vlan.vid == vid)
		{
			def_vlan.vid = 1;
		}
		
		ret = epon_request_onu_vlan_set(context, device_id, llidport,port,def_vlan,vlan_mode,vlan_cfg,cfg_nums);
	}

	return ret;
}


cs_status epon_request_vlan_del(cs_uint16 vid)
{
	cs_callback_context_t         context;
	cs_int32                      device_id = 0;
	cs_int32                      llidport = 0;
	cs_uint8                      i = 0;
	cs_uint8                      port_id = 0;
	cs_sdl_vlan_t                 vlan_entry;

	memset(&vlan_entry,0,sizeof(vlan_entry));

	if(CS_E_OK == epon_request_vlan_get(context,device_id,llidport,vid,&vlan_entry))
	{
		// �Ӷ˿���ɾ��VLAN
		for(i = 0;i < UNI_PORT_MAX;i++)
		{
			if(((vlan_entry.mbr >> i) & 0x01 ) == 1)
			{
				port_id = i+1;
				if(CS_E_OK != epon_request_vlan_del_from_port(context,device_id,llidport,port_id,vid))
				{
					return CS_E_ERROR;
				}
			}
		}
	}
	
	return CS_E_OK;
}

static cs_sdl_vlan_mapping_t g_mapping;
cs_status epon_request_vlan_mapping_get (
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_sdl_vlan_mapping_t *mapping
)
{
    if(NULL == mapping)
    {
        cs_printf("%s.%d:null pointer!!!\n",__func__,__LINE__);
        return CS_E_PARAM;
    }

    memcpy(mapping,&g_mapping,sizeof(cs_sdl_vlan_mapping_t));
    return CS_E_OK;    
}
cs_status epon_request_vlan_mapping_set (
    CS_IN  cs_callback_context_t         context,
    CS_IN  cs_int32                      device_id,
    CS_IN  cs_int32                      llidport,
    CS_IN  cs_sdl_vlan_mapping_t *mapping
)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t filter_field;
    rtk_filter_action_t filter_action;
    rtk_filter_cfg_t filter_cfg;
    rtk_filter_id_t filter_id;    
    rtk_filter_number_t rule_num;
    cs_status ret = CS_E_ERROR;

    if(NULL == mapping)
    {
        cs_printf("%s.%d:null pointer!!!\n",__func__,__LINE__);
        return CS_E_PARAM;
    }

    if(!mapping->valid)
    {
        rt   = rtk_filter_igrAcl_cfg_del(__PKT_VLAN_MAPPING_0_RULE);
        rt |= rtk_filter_igrAcl_cfg_del(__PKT_VLAN_MAPPING_1_RULE);

        if(0 == rt)
        {
           memset(&g_mapping,0,sizeof(cs_sdl_vlan_mapping_t));
           ret = CS_E_OK;
        }
    }
    else
    {
        memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
        memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
        memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
        
        filter_field.fieldType = FILTER_FIELD_CTAG;
        filter_field.filter_pattern_union.stag.vid.dataType = FILTER_FIELD_DATA_MASK;
        filter_field.filter_pattern_union.stag.vid.value = mapping->src_vid;
        filter_field.filter_pattern_union.stag.vid.mask = 0xfff;
        filter_field.next = NULL;
        rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);

        filter_id = __PKT_VLAN_MAPPING_0_RULE;
        filter_cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
        filter_cfg.activeport.value = 0x1 << L2P_PORT(mapping->src_port);
        filter_cfg.activeport.mask = 0xFF;
        filter_cfg.invert = 0;

        filter_action.actEnable[FILTER_ENACT_ADD_DSTPORT] = TRUE;   
        filter_action.filterAddDstPortmask = 0x1 << L2P_PORT(mapping->dst_port);;
        filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
        filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
        filter_action.actEnable[FILTER_ENACT_INGRESS_CVLAN_VID] = TRUE;  
        filter_action.filterIngressCvlanVid = mapping->dst_vid;
        filter_action.filterPriority = 0;
       
        rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);


        memset((cs_uint8 *)&filter_field, 0 , sizeof(filter_field));
        memset((cs_uint8 *)&filter_action, 0 , sizeof(filter_action));
        memset((cs_uint8 *)&filter_cfg, 0 , sizeof(filter_cfg));
        
        filter_field.fieldType = FILTER_FIELD_CTAG;
        filter_field.filter_pattern_union.stag.vid.dataType = FILTER_FIELD_DATA_MASK;
        filter_field.filter_pattern_union.stag.vid.value = mapping->dst_vid;
        filter_field.filter_pattern_union.stag.vid.mask = 0xfff;
        filter_field.next = NULL;
        rt = rtk_filter_igrAcl_field_add(&filter_cfg,&filter_field);

        filter_id = __PKT_VLAN_MAPPING_1_RULE;
        filter_cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
        filter_cfg.activeport.value = 0x1 << L2P_PORT(mapping->dst_port);
        filter_cfg.activeport.mask = 0xFF;
        filter_cfg.invert = 0;
        filter_action.actEnable[FILTER_ENACT_ADD_DSTPORT] = TRUE;   
        filter_action.filterAddDstPortmask = 0x1 << L2P_PORT(mapping->src_port);;
        filter_action.actEnable[FILTER_ENACT_PRIORITY] = TRUE;     
        filter_action.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;   
        filter_action.actEnable[FILTER_ENACT_INGRESS_CVLAN_VID] = TRUE;  
        filter_action.filterIngressCvlanVid = mapping->src_vid;
        filter_action.filterPriority = 0;
       
        rt = rtk_filter_igrAcl_cfg_add(filter_id,&filter_cfg,&filter_action,&rule_num);

        
        if(rt)
        {
            diag_printf("configure return %x\n", rt);      
        }
        else
        {
            memcpy(&g_mapping,mapping,sizeof(cs_sdl_vlan_mapping_t));
            ret = CS_E_OK;
        }
    }
    return ret;
}


