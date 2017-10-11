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
#include "plat_common.h" 
#include "aal_l2.h"
#include "aal.h"
#include "aal_flow.h"
#include "sdl.h"
#include "aal_cls.h"
#include "sdl_ma.h"
#include "sdl_l2.h"
#include <rtk_api.h>
#include <rtk_api_ext.h>

extern cs_status aal_pon_mac_addr_get( CS_IN cs_mac_t *mac);


#define __LOOPDETECT_ETHTYPE 0xfffe
#define __IROS_OAM_ETHTYPE   0xffff
#define __PPPOE_DIS_ETHTYPE  0x8863
#define __PPPOE_SES_ETHTYPE  0x8864
#define __BCMP_ETHTYPE       0x0877
#define __BCMP_PON_EOC       0x8891
#define __DHCPV6_SPORT       0x0222

#define __DEFAULT_MSTI       0

/********************************************
*     RTL ACL resource       0--highest pri *
*     special packets        10 (0-9)       *
*     CTC classfication      24 (10-33)     *
*    / QOS remarking         24 (10-33)     *
*     MAC filter/bind        24 (34-57)     *
*     egress queue ratelimit 4  (58-61)     *
*     vlan                   1  (63)        *
*     reserved               1  (62)        *
*********************************************/
#define __PKT_STP_TRAP_RULE         0
#define __PKT_STP_BLOCK_RULE        1
#define __PKT_ARP_RULE              2
#define __PKT_DHCP_RULE             3
#define __PKT_PPPOE_SES_RULE        4
#define __PKT_PPPOE_DIS_RULE        5
#define __PKT_LOOPDETECT_RULE       6
#define __PKT_IROS_RULE             7
#define __PKT_IGMP_RULE             8
#define __PKT_MYMAC_RULE            9
#ifdef HAVE_GWD_LOOP_DETECT
#define __PKT_LOOP_RULE				10
#endif
#ifdef HAVE_DHCPV6_PKT_DEST_PORT_SUPPORT
#define __PKT_DHCPV6_RULE           12 
#endif

#define __PKT_RATE_MAX       2000

#define __FLOW_ID_PRI(queue) (AAL_RATE_LIMIT_FLOW_28 +(queue))

#define __PKT_CPU_QUEUE_NUM         4

static cs_sdl_queue_t     g_ds_pkt_queue[CS_PKT_TYPE_NUM];

static cs_uint32          g_queue_pkt_rate[__PKT_CPU_QUEUE_NUM];

static cs_sdl_pkt_state_t g_pkt_state[S_BOTH][CS_PKT_TYPE_NUM];

static cs_uint32  g_stp_blockport_status;


static cs_uint8 g_sdl_pkt_map[] = 
{
    AAL_PKT_BPDU,           /* CS_PKT_BPDU*/
    AAL_PKT_8021X,          /* CS_PKT_8021X */
    AAL_PKT_IGMP,           /* CS_PKT_GMP */
    AAL_PKT_ARP,            /* CS_PKT_ARP */
    AAL_PKT_OAM,            /* CS_PKT_OAM */
    AAL_PKT_MPCP,           /* CS_PKT_MPCP */
    AAL_PKT_DHCP,           /* CS_PKT_DHCP */
    AAL_PKT_SWT,            /* CS_PKT_IROS */
    AAL_PKT_PPPoE_DIS,      /* CS_PKT_PPPOE_DIS */
    AAL_PKT_PPPoE_Session,  /* CS_PKT_PPPOE_SES */
    AAL_PKT_SWT,            /* CS_PKT_IP */
    AAL_PKT_IPV6NDP,        /* CS_PKT_NDP */
    AAL_PKT_SWT,            /* CS_PKT_LOOPDETECT */
    AAL_PKT_MYMAC,          /* CS_PKT_MYMAC */
    AAL_PKT_SWT,            /* CS_PKT_TYPE_NUM */
  
};



static cs_status __l2_pon_iros_state_set( CS_IN cs_sdl_pkt_state_t state )
{
    cs_aal_pkt_spec_udf_msk_t spe_udf_msk;
    cs_aal_pkt_spec_udf_t     spe_udf_cfg;
    cs_aal_spec_pkt_ctrl_msk_t spe_pkt_msk;
    cs_aal_spec_pkt_ctrl_t     spe_pkt_cfg;

    /* set udf2 ethtype 0xffff */
    spe_udf_cfg.udf2_etype = __IROS_OAM_ETHTYPE;
    spe_udf_msk.u32 = 0;
    spe_udf_msk.s.udf2_etype = 1;
    (void)aal_pkt_special_udf_set(AAL_PORT_ID_PON, spe_udf_msk, &spe_udf_cfg);

    memset(&spe_pkt_cfg, 0, sizeof(cs_aal_spec_pkt_ctrl_t));
    
    spe_pkt_msk.u32 = 0;
    spe_pkt_msk.s.dpid = 1;
 
    spe_pkt_cfg.dpid.dst_op = (state==DST_FE)?AAL_SPEC_DST_FE:((state==DST_CPU)?AAL_SPEC_DST_PORT: AAL_SPEC_DST_DROP);
    spe_pkt_cfg.dpid.dpid = AAL_PORT_ID_CPU;
    
    return aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_UDF2, spe_pkt_msk, &spe_pkt_cfg);
}

#ifdef HAVE_GWD_LOOP_DETECT
static cs_status __l2_pon_lpd_state_set(
		CS_IN cs_sdl_pkt_state_t state
)
{
	cs_aal_pkt_spec_udf_msk_t spe_udf_msk;
	cs_aal_pkt_spec_udf_t     spe_udf_cfg;
	cs_aal_spec_pkt_ctrl_msk_t spe_pkt_msk;
	cs_aal_spec_pkt_ctrl_t     spe_pkt_cfg;

	/* set udf2 ethtype 0xffff */
	spe_udf_cfg.udf0_etype = 0x0800;
	spe_udf_cfg.udf0_da.addr[0]=0x00;
	spe_udf_cfg.udf0_da.addr[1]=0x0f;
	spe_udf_cfg.udf0_da.addr[2]=0xe9;
	spe_udf_cfg.udf0_da.addr[3]=0x04;
	spe_udf_cfg.udf0_da.addr[4]=0x8e;
	spe_udf_cfg.udf0_da.addr[5]=0xdf;
	spe_udf_cfg.udf0_chk_da = 1;
	spe_udf_msk.u32 = 0;
	spe_udf_msk.s.udf0_etype = 1;
	spe_udf_msk.s.udf0_da = 1;
	spe_udf_msk.s.udf0_chk_da = 1;
	(void)aal_pkt_special_udf_set(AAL_PORT_ID_GE, spe_udf_msk, &spe_udf_cfg);

	//  memset(&spe_pkt_cfg, 0, sizeof(cs_aal_spec_pkt_ctrl_t));

	spe_pkt_msk.u32 = 0;
	spe_pkt_msk.s.dpid = 1;
	spe_pkt_cfg.dpid.dst_op = AAL_SPEC_DST_PORT;
	spe_pkt_cfg.dpid.dpid = AAL_PORT_ID_CPU;

	return aal_special_pkt_behavior_set(AAL_PORT_ID_GE, AAL_PKT_UDF0, spe_pkt_msk, &spe_pkt_cfg);
}
#endif

static cs_status __l2_pon_spec_pkt_state_set(
     CS_IN cs_pkt_type_t             pkt_type,
     CS_IN cs_sdl_pkt_state_t        state
)
{  
    cs_aal_spec_pkt_ctrl_msk_t  pkt_msk;
    cs_aal_spec_pkt_ctrl_t      pkt_cfg;
    cs_aal_pkt_type_t           pkt;
    cs_status                   ret;

         
    if(pkt_type >= CS_PKT_TYPE_NUM)
    {
        SDL_MIN_LOG("pkt type is not be supported.(%d) FILE: %s, LINE: %d", pkt_type, __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    memset(&pkt_msk, 0, sizeof(cs_aal_spec_pkt_ctrl_msk_t));
    memset(&pkt_cfg, 0, sizeof(cs_aal_spec_pkt_ctrl_t));

    pkt_msk.u32 = 0;
    pkt_msk.s.dpid = 1;
   
    pkt_cfg.dpid.dst_op = (state==DST_FE)?AAL_SPEC_DST_FE:((state==DST_CPU)?AAL_SPEC_DST_PORT: AAL_SPEC_DST_DROP);
    pkt_cfg.dpid.dpid   = AAL_PORT_ID_CPU;
        
    
    pkt = g_sdl_pkt_map[pkt_type];

    if(pkt == AAL_PKT_SWT)
    {
#if 0    
        if(pkt_type == CS_PKT_PPPOE)
        {
            ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_PPPoE_DIS, pkt_msk, &pkt_cfg);
            ret += aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_PPPoE_Session, pkt_msk, &pkt_cfg);
        }
        else 
#endif            
        if (pkt_type == CS_PKT_IROS)
        {
            ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_HELLO, pkt_msk, &pkt_cfg);
            ret += __l2_pon_iros_state_set(state );
        }
		#ifdef HAVE_GWD_LOOP_DETECT
		else if(pkt_type == CS_PKT_GWD_LOOPDETECT)
		{
			  ret = aal_special_pkt_behavior_set(AAL_PORT_ID_GE, AAL_PKT_UDF0, pkt_msk, &pkt_cfg);
          	  ret += __l2_pon_lpd_state_set(state );
		}
		#endif
        else 
        {
            SDL_MIN_LOG("pkt type is not be supported.(%d) FILE: %s, LINE: %d", pkt_type, __FILE__, __LINE__);
            ret = CS_E_ERROR;
        }
    }
    else
    {   
        ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, pkt, pkt_msk, &pkt_cfg);          
    }
        
    if(ret)
    {
        SDL_MIN_LOG("pkt type set failed.(%d) FILE: %s, LINE: %d", pkt_type, __FILE__, __LINE__);
        return CS_E_ERROR;
    }     
     
    return CS_E_OK;
}

static cs_status __l2_sw_mac_trap_set(
     CS_IN cs_pkt_type_t             pkt_type,
     CS_IN cs_sdl_pkt_state_t        state
)
{   
    rtk_mac_t mac;
    rtk_api_ret_t rtk_ret;
    rtk_trap_rma_action_t rma_action;
    

    rma_action = (state==DST_FE)?RMA_ACTION_FORWARD:((state==DST_CPU)?RMA_ACTION_TRAP2CPU: RMA_ACTION_DROP);

    if(pkt_type == CS_PKT_BPDU)
    {  
        mac.octet[0] = 0x01;
        mac.octet[1] = 0x80;
        mac.octet[2] = 0xc2;
        mac.octet[3] = 0x00;
        mac.octet[4] = 0x00;
        mac.octet[5] = 0x00;    
    }
    else if(pkt_type == CS_PKT_OAM)
    {  
        mac.octet[0] = 0x01;
        mac.octet[1] = 0x80;
        mac.octet[2] = 0xc2;
        mac.octet[3] = 0x00;
        mac.octet[4] = 0x00;
        mac.octet[5] = 0x02;    
    }
    else if(pkt_type == CS_PKT_8021X)
    {  
        mac.octet[0] = 0x01;
        mac.octet[1] = 0x80;
        mac.octet[2] = 0xc2;
        mac.octet[3] = 0x00;
        mac.octet[4] = 0x00;
        mac.octet[5] = 0x03;    
    }
    else
    {
        SDL_MIN_LOG("pkt type is not be supported.(%d) FILE: %s, LINE: %d", pkt_type, __FILE__, __LINE__);
        return CS_E_PARAM;
    }  

    rtk_ret = rtk_trap_rmaAction_set(&mac, rma_action);
    if(rtk_ret!=RT_ERR_OK)
    {
        SDL_MIN_LOG("rtk_trap_rmaAction_set return %d. FILE: %s, LINE: %d", rtk_ret, __FILE__, __LINE__);
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

static cs_status __l2_sw_acl_trap_set(
     CS_IN cs_pkt_type_t             pkt_type,
     CS_IN cs_sdl_pkt_state_t        state
)
{  
    rtk_filter_cfg_t   cfg;
    rtk_filter_action_t act;
    rtk_api_ret_t rt = 0;
    cs_uint32 ruleNum;
    rtk_filter_id_t  rule_idx;
    rtk_filter_care_tag_index_t tag_idx;
    rtk_filter_field_t field;
   

    if(pkt_type == CS_PKT_ARP)
    {  
        tag_idx  = CARE_TAG_ARP;
        rule_idx = __PKT_ARP_RULE;
    }
    else if(pkt_type == CS_PKT_DHCP)
    {  
        tag_idx  = CARE_TAG_DHCP;
        rule_idx = __PKT_DHCP_RULE;
    }
    else if(pkt_type == CS_PKT_GMP)
    {  
        tag_idx  = CARE_TAG_IGMP;
        rule_idx = __PKT_IGMP_RULE;
    }
    else if(pkt_type == CS_PKT_PPPOE_DIS)
    {  
        rule_idx = __PKT_PPPOE_DIS_RULE;
    }
    else if(pkt_type == CS_PKT_PPPOE_SES)
    {  
        rule_idx = __PKT_PPPOE_SES_RULE;
    }
    else if(pkt_type == CS_PKT_MYMAC)
    {  
        rule_idx = __PKT_MYMAC_RULE;
    }
    else if(pkt_type == CS_PKT_LOOPDETECT)
    {  
        rule_idx = __PKT_LOOPDETECT_RULE;
    }
    else if(pkt_type == CS_PKT_IROS)
    {  
        rule_idx = __PKT_IROS_RULE;
    }
	#ifdef HAVE_GWD_LOOP_DETECT
	else if(pkt_type == CS_PKT_GWD_LOOPDETECT)
	{
		rule_idx = __PKT_LOOP_RULE;
	}
	#endif
	#ifdef HAVE_DHCPV6_PKT_DEST_PORT_SUPPORT
	else if(pkt_type == CS_PKT_DHCPV6)
	{
		rule_idx = __PKT_DHCPV6_RULE;
	}
	#endif
    else
    {
        SDL_MIN_LOG("pkt type is not be supported.(%d) FILE: %s, LINE: %d", pkt_type, __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    memset(&cfg, 0, sizeof(rtk_filter_cfg_t));
    memset(&act, 0, sizeof(rtk_filter_action_t));
    memset(&field, 0, sizeof(rtk_filter_field_t));
      
    if(state == DST_FE )
    {
        rt = rtk_filter_igrAcl_cfg_del(rule_idx);

        /*BUG31377 RTP Session packets are trapped to uplink */
        if(pkt_type == CS_PKT_PPPOE_SES)
        {
            field.fieldType = FILTER_FIELD_ETHERTYPE;
            field.filter_pattern_union.etherType.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.etherType.value = __PPPOE_SES_ETHTYPE;
            field.filter_pattern_union.etherType.mask = 0xFFFF;

            field.next = NULL;
        
            rt = rtk_filter_igrAcl_field_add(&cfg, &field);       
      
            cfg.invert = FALSE;    
            cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
            cfg.activeport.value = 0xF;
            cfg.activeport.mask = 0xFF;             
            act.actEnable[FILTER_ENACT_ADD_DSTPORT] = TRUE;       
            act.filterAddDstPortmask = 0x40;
            
            rt = rtk_filter_igrAcl_cfg_add(rule_idx, &cfg, &act, &ruleNum);
        }
    }
    else
    {            
        if(pkt_type == CS_PKT_LOOPDETECT)
        {
            field.fieldType = FILTER_FIELD_ETHERTYPE;
            field.filter_pattern_union.etherType.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.etherType.value = __LOOPDETECT_ETHTYPE;
            field.filter_pattern_union.etherType.mask = 0xFFFF;
        }
        else if(pkt_type == CS_PKT_IROS)
        {
            field.fieldType = FILTER_FIELD_ETHERTYPE;
            field.filter_pattern_union.etherType.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.etherType.value = __IROS_OAM_ETHTYPE;
            field.filter_pattern_union.etherType.mask = 0xFFFF;
        }
        else if(pkt_type == CS_PKT_PPPOE_DIS)
        {
            field.fieldType = FILTER_FIELD_ETHERTYPE;
            field.filter_pattern_union.etherType.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.etherType.value = __PPPOE_DIS_ETHTYPE;
            field.filter_pattern_union.etherType.mask = 0xFFFF;
        }
        else if(pkt_type == CS_PKT_MYMAC)
        {   
             cs_mac_t     mac;

            //MYmac is configured in Startcofig,so it is fixed.
            //But must pay attention to the order of initialization

            memset(&mac, 0, sizeof(cs_mac_t));

            (void)aal_pon_mac_addr_get(&mac);

            field.fieldType = FILTER_FIELD_DMAC;
            field.filter_pattern_union.dmac.dataType = FILTER_FIELD_DATA_MASK;

            field.filter_pattern_union.dmac.value.octet[0] = mac.addr[0];
            field.filter_pattern_union.dmac.value.octet[1] = mac.addr[1];
            field.filter_pattern_union.dmac.value.octet[2] = mac.addr[2];
            field.filter_pattern_union.dmac.value.octet[3] = mac.addr[3];
            field.filter_pattern_union.dmac.value.octet[4] = mac.addr[4];
            field.filter_pattern_union.dmac.value.octet[5] = mac.addr[5];            
            field.filter_pattern_union.dmac.mask.octet[0]  = 0xFF;
            field.filter_pattern_union.dmac.mask.octet[1]  = 0xFF;
            field.filter_pattern_union.dmac.mask.octet[2]  = 0xFF;
            field.filter_pattern_union.dmac.mask.octet[3]  = 0xFF;
            field.filter_pattern_union.dmac.mask.octet[4]  = 0xFF;
            field.filter_pattern_union.dmac.mask.octet[5]  = 0xFF;
            
        }
		#ifdef HAVE_GWD_LOOP_DETECT
	    else if(pkt_type == CS_PKT_GWD_LOOPDETECT)
        {
        	#if 0
            field.fieldType = FILTER_FIELD_ETHERTYPE;
            field.filter_pattern_union.etherType.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.etherType.value = 0x0800;
            field.filter_pattern_union.etherType.mask = 0xFFFF;
			#else

            field.fieldType = FILTER_FIELD_DMAC;
            field.filter_pattern_union.dmac.dataType = FILTER_FIELD_DATA_MASK;

            field.filter_pattern_union.dmac.value.octet[0] = 0x00;
            field.filter_pattern_union.dmac.value.octet[1] = 0x0f;
            field.filter_pattern_union.dmac.value.octet[2] = 0xe9;
            field.filter_pattern_union.dmac.value.octet[3] = 0x04;
            field.filter_pattern_union.dmac.value.octet[4] = 0x8e;
            field.filter_pattern_union.dmac.value.octet[5] = 0xdf;            
            field.filter_pattern_union.dmac.mask.octet[0]  = 0xFF;
            field.filter_pattern_union.dmac.mask.octet[1]  = 0xFF;
            field.filter_pattern_union.dmac.mask.octet[2]  = 0xFF;
            field.filter_pattern_union.dmac.mask.octet[3]  = 0xFF;
            field.filter_pattern_union.dmac.mask.octet[4]  = 0xFF;
            field.filter_pattern_union.dmac.mask.octet[5]  = 0xFF;
			#endif
        }
		#endif
		#ifdef HAVE_DHCPV6_PKT_DEST_PORT_SUPPORT
		else if(pkt_type == CS_PKT_DHCPV6)
		{
			field.fieldType = FILTER_FIELD_UDP_SPORT;
            field.filter_pattern_union.udpSrcPort.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.udpSrcPort.value = __DHCPV6_SPORT;
            field.filter_pattern_union.udpSrcPort.mask = 0xFFFF;
		}
		#endif
        else
        {
            field.fieldType = FILTER_FIELD_DMAC;
            field.filter_pattern_union.dmac.dataType = FILTER_FIELD_DATA_MASK;

            cfg.careTag.tagType[tag_idx].value = TRUE;
            cfg.careTag.tagType[tag_idx].mask  = TRUE;    
        }
        
        field.next = NULL;
        
        rt = rtk_filter_igrAcl_field_add(&cfg, &field);       
      
        cfg.invert = FALSE;    
        cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
        cfg.activeport.value = 0xF;
        cfg.activeport.mask = 0xFF;  
            
        if(state == DST_DROP )
        {
            act.actEnable[FILTER_ENACT_DROP] = TRUE;
        }
        else
        {            
            act.actEnable[FILTER_ENACT_TRAP_CPU] = TRUE;       
            act.actEnable[FILTER_ENACT_PRIORITY] = TRUE;           
            act.filterPriority = 7;
        }
        
        rt = rtk_filter_igrAcl_cfg_del(rule_idx);
        rt += rtk_filter_igrAcl_cfg_add(rule_idx, &cfg, &act, &ruleNum); 
    }

    if(rt)
    {
        SDL_MIN_LOG("RTK ACL rule set failed.(%d) FILE: %s, LINE: %d", rule_idx,  __FILE__, __LINE__);
        return CS_E_ERROR;
    }
     
    return CS_E_OK;
}

static cs_status __l2_sw_uni_set(
     CS_IN cs_pkt_type_t             pkt_type,
     CS_IN cs_sdl_pkt_state_t        state
)
{   
    cs_status    ret;
    
    if((pkt_type == CS_PKT_BPDU)||
        (pkt_type == CS_PKT_OAM)||
        (pkt_type == CS_PKT_8021X))
    {  
        ret = __l2_sw_mac_trap_set(pkt_type, state);
    }
    else 
    {  
        ret = __l2_sw_acl_trap_set(pkt_type, state);
    }
    
    return ret;
}

static cs_status __l2_sw_uni_stp_block(
     CS_IN cs_port_id_t              portid,
     CS_IN cs_boolean                block
)
{
    rtk_api_ret_t rt;
    rtk_filter_field_t  field[2];
    rtk_filter_cfg_t     cfg;
    rtk_filter_action_t  act; 
    rtk_filter_number_t  ruleNum;
    cs_status ret = CS_E_OK;


    memset(&cfg, 0, sizeof(rtk_filter_cfg_t));
    memset(&act, 0, sizeof(rtk_filter_action_t));
    memset(&field, 0, 2*sizeof(rtk_filter_field_t));

    field[0].fieldType = FILTER_FIELD_DMAC;       
    field[0].filter_pattern_union.dmac.dataType = FILTER_FIELD_DATA_MASK;
    field[0].filter_pattern_union.dmac.value.octet[0] = 0x01;
    field[0].filter_pattern_union.dmac.value.octet[1] = 0x80;
    field[0].filter_pattern_union.dmac.value.octet[2] = 0xc2;
    field[0].filter_pattern_union.dmac.value.octet[3] = 0x00;
    field[0].filter_pattern_union.dmac.value.octet[4] = 0x00;
    field[0].filter_pattern_union.dmac.value.octet[5] = 0x00;
    field[0].filter_pattern_union.dmac.mask.octet[0]  = 0xFF;
    field[0].filter_pattern_union.dmac.mask.octet[1]  = 0xFF;
    field[0].filter_pattern_union.dmac.mask.octet[2]  = 0xFF;
    field[0].filter_pattern_union.dmac.mask.octet[3]  = 0xFF;
    field[0].filter_pattern_union.dmac.mask.octet[4]  = 0xFF;
    field[0].filter_pattern_union.dmac.mask.octet[5]  = 0xFF;
    field[0].next = NULL;

    field[1].fieldType = FILTER_FIELD_DMAC;       
    field[1].filter_pattern_union.dmac.dataType = FILTER_FIELD_DATA_MASK;
    field[1].filter_pattern_union.dmac.value.octet[0] = 0x00;
    field[1].filter_pattern_union.dmac.value.octet[1] = 0x00;
    field[1].filter_pattern_union.dmac.value.octet[2] = 0x00;
    field[1].filter_pattern_union.dmac.value.octet[3] = 0x00;
    field[1].filter_pattern_union.dmac.value.octet[4] = 0x00;
    field[1].filter_pattern_union.dmac.value.octet[5] = 0x00;
    field[1].filter_pattern_union.dmac.mask.octet[0]  = 0x00;
    field[1].filter_pattern_union.dmac.mask.octet[1]  = 0x00;
    field[1].filter_pattern_union.dmac.mask.octet[2]  = 0x00;
    field[1].filter_pattern_union.dmac.mask.octet[3]  = 0x00;
    field[1].filter_pattern_union.dmac.mask.octet[4]  = 0x00;
    field[1].filter_pattern_union.dmac.mask.octet[5]  = 0x00;
    field[1].next = NULL;
            
    if(block)    
    {
        if(g_stp_blockport_status &(1 << (portid-1)))
            goto end;

        g_stp_blockport_status |= (1 << (portid-1));
            
        rt = rtk_filter_igrAcl_field_add(&cfg, &field[1]);       
        if(rt)
        {
            ret = CS_E_ERROR; 
            goto end;
        }
        act.actEnable[FILTER_ENACT_DROP] = TRUE;
        
        cfg.invert = FALSE;    
        cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
        cfg.activeport.value = g_stp_blockport_status;
        cfg.activeport.mask = 0xFF;  
        
        rt = rtk_filter_igrAcl_cfg_del(__PKT_STP_BLOCK_RULE);
        rt += rtk_filter_igrAcl_cfg_add(__PKT_STP_BLOCK_RULE, &cfg, &act, &ruleNum); 
        if(rt)
        {
            ret = CS_E_ERROR; 
            goto end;
        }
        memset(&cfg, 0, sizeof(rtk_filter_cfg_t));
        memset(&act, 0, sizeof(rtk_filter_action_t));
        
        rt = rtk_filter_igrAcl_field_add(&cfg, &field[0]);       
        if(rt)
        {
            ret = CS_E_ERROR; 
            goto end;
        }
        cfg.invert = FALSE;    
        cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
        cfg.activeport.value = g_stp_blockport_status;
        cfg.activeport.mask = 0xFF;  

        act.actEnable[FILTER_ENACT_TRAP_CPU] = TRUE;       
        act.actEnable[FILTER_ENACT_PRIORITY] = TRUE;           
        act.filterPriority = 7;
        
        rt = rtk_filter_igrAcl_cfg_del(__PKT_STP_TRAP_RULE);
        rt += rtk_filter_igrAcl_cfg_add(__PKT_STP_TRAP_RULE, &cfg, &act, &ruleNum); 
        if(rt)
        {
            ret = CS_E_ERROR; 
            goto end;
        }           
    }
    else
    {
        if(0==(g_stp_blockport_status &(1 << (portid-1))))
            goto end;

        g_stp_blockport_status &= ~(1<<(portid-1));

        if(0 == g_stp_blockport_status)
        {    
            rt = rtk_filter_igrAcl_cfg_del(__PKT_STP_BLOCK_RULE);
            rt += rtk_filter_igrAcl_cfg_del(__PKT_STP_TRAP_RULE);
            if(rt)
            {
                ret = CS_E_ERROR; 
                goto end;
            }            
        }
        else
        {
            rt = rtk_filter_igrAcl_field_add(&cfg, &field[1]);       
            if(rt)
            {
                ret = CS_E_ERROR; 
                goto end;
            }
            act.actEnable[FILTER_ENACT_DROP] = TRUE;
            
            cfg.invert = FALSE;    
            cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
            cfg.activeport.value = g_stp_blockport_status;
            cfg.activeport.mask = 0xFF;  
            
            rt = rtk_filter_igrAcl_cfg_del(__PKT_STP_BLOCK_RULE);
            rt += rtk_filter_igrAcl_cfg_add(__PKT_STP_BLOCK_RULE, &cfg, &act, &ruleNum); 
            if(rt)
            {
                ret = CS_E_ERROR; 
                goto end;
            }
            memset(&cfg, 0, sizeof(rtk_filter_cfg_t));
            memset(&act, 0, sizeof(rtk_filter_action_t));
            
            rt = rtk_filter_igrAcl_field_add(&cfg, &field[0]);       
            if(rt)
            {
                ret = CS_E_ERROR; 
                goto end;
            }
            cfg.invert = FALSE;    
            cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
            cfg.activeport.value = g_stp_blockport_status;
            cfg.activeport.mask = 0xFF;  

            act.actEnable[FILTER_ENACT_TRAP_CPU] = TRUE;       
            act.actEnable[FILTER_ENACT_PRIORITY] = TRUE;           
            act.filterPriority = 7;
            
            rt = rtk_filter_igrAcl_cfg_del(__PKT_STP_TRAP_RULE);
            rt += rtk_filter_igrAcl_cfg_add(__PKT_STP_TRAP_RULE, &cfg, &act, &ruleNum); 
            if(rt)
            {
                ret = CS_E_ERROR; 
                goto end;
            }           
        }
    }
   
end:
    return ret;
}

static cs_status __l2_mii_port_set(
     CS_IN cs_pkt_type_t             pkt_type,
     CS_IN cs_sdl_pkt_state_t        state
)
{   
    //Not support now
    return CS_E_OK;
}

/******Special packet MA status ********************/ 
static cs_status __l2_pkt_rate_set(    
    CS_IN cs_sdl_queue_t            queue,
    CS_IN cs_uint32                 pkt_s
)
{   
    cs_status               ret = CS_E_OK;
    cs_aal_rate_limit_msk_t rl_msk;
    cs_aal_rate_limit_t     rate_limit;
    
    memset(&rate_limit, 0, sizeof(cs_aal_rate_limit_t));

    rl_msk.u = 0;
     
    rl_msk.s.enable = 1;
    rl_msk.s.rate   = 1;
    rl_msk.s.cbs    = 1;
    rl_msk.s.rlmode = 1;
    rate_limit.rate = pkt_s;
    rate_limit.ctrl.s.enable = (pkt_s == 0)? 0: 1;
    rate_limit.ctrl.s.rlmode = AAL_RATE_LIMIT_MODE_PPS;
    rate_limit.cbs = 100;
    
    ret = aal_flow_rate_limit_set(__FLOW_ID_PRI(queue), &rl_msk, &rate_limit);
      
    return ret;
}

static cs_status __l2_ds_pkt_pri_set(    
    CS_IN cs_pkt_type_t             pkt_type,
    CS_IN cs_sdl_queue_t            queue
)
{
    cs_aal_spec_pkt_ctrl_msk_t  pkt_msk;
    cs_aal_spec_pkt_ctrl_t      pkt_cfg;
    cs_aal_pkt_type_t           pkt;
    cs_status                   ret = CS_E_OK;


    memset(&pkt_cfg, 0, sizeof(cs_aal_spec_pkt_ctrl_t));

    pkt_msk.u32          = 0;
    pkt_msk.s.cos        = 1;
    pkt_msk.s.rate_limit = 1;
    pkt_msk.s.bypass_plc = 1;

    /* cos: 0,1~que 0, */
    /*      2,3~que 1, */
    /*      4,5~que 2, */
    /*      6,7~que 3, */
    pkt_cfg.cos        = 2*queue;      
    pkt_cfg.flow_en    = TRUE; 
    pkt_cfg.flow_id    = (__FLOW_ID_PRI(queue)-AAL_RATE_LIMIT_FLOW_0);
    pkt_cfg.bypass_plc = FALSE;
    
    pkt = g_sdl_pkt_map[pkt_type];
      
    if(pkt == AAL_PKT_SWT)
    {   
#if 0    
        if(pkt_type == CS_PKT_PPPOE)
        {
            ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_PPPoE_DIS, pkt_msk, &pkt_cfg);
            if(ret)
                return ret;
            ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_PPPoE_Session, pkt_msk, &pkt_cfg);
        }
        else
#endif            
        if (pkt_type == CS_PKT_IROS)
        {
            ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_HELLO, pkt_msk, &pkt_cfg);
            if(ret)
                return ret;
            ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_UDF2, pkt_msk, &pkt_cfg);
        }
    }
    else
    {   
        ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, pkt, pkt_msk, &pkt_cfg);          
    }   
    
    return ret;
}

static cs_status __l2_ds_pkt_def_set(    
    CS_IN cs_pkt_type_t             pkt_type
)
{
    cs_aal_spec_pkt_ctrl_msk_t  pkt_msk;
    cs_aal_spec_pkt_ctrl_t      pkt_cfg;
    cs_aal_pkt_type_t           pkt;
    cs_status                   ret = CS_E_OK;

    
    memset(&pkt_cfg, 0, sizeof(cs_aal_spec_pkt_ctrl_t));

    pkt_msk.u32          = 0;
    pkt_msk.s.cos        = 1;
    pkt_msk.s.rate_limit = 1;
    pkt_msk.s.bypass_plc = 1;

    pkt_cfg.cos          = 0;
    pkt_cfg.flow_en      = FALSE; 
    pkt_cfg.flow_id      = 0;
    pkt_cfg.bypass_plc   = FALSE;
    
    pkt = g_sdl_pkt_map[pkt_type];
      
    if(pkt == AAL_PKT_SWT)
    {
#if 0    
        if(pkt_type == CS_PKT_PPPOE)
        {
            ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_PPPoE_DIS, pkt_msk, &pkt_cfg);
            if(ret)
                return ret;
            ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_PPPoE_Session, pkt_msk, &pkt_cfg);
        }
        else
#endif            
        if (pkt_type == CS_PKT_IROS)
        {
            ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_HELLO, pkt_msk, &pkt_cfg);
            if(ret)
                return ret;
            ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_UDF2, pkt_msk, &pkt_cfg);
        }
    }
    else
    {   
        ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, pkt, pkt_msk, &pkt_cfg);          
    }   
    
    return ret;
}


cs_status epon_request_onu_loopdetect_set (
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_uint32                 device,
    CS_IN  cs_uint32                 llidport,
    CS_IN  cs_boolean                enable
)
{
    cs_aal_pkt_spec_udf_msk_t spe_udf_msk;
    cs_aal_pkt_spec_udf_t     spe_udf_cfg;
    cs_aal_spec_pkt_ctrl_msk_t spe_pkt_msk;
    cs_aal_spec_pkt_ctrl_t     spe_pkt_cfg;

    /* set udf3 ethtype 0xfffe */
    spe_udf_cfg.udf3_etype = __LOOPDETECT_ETHTYPE;
    spe_udf_msk.u32 = 0;
    spe_udf_msk.s.udf3_etype = 1;
    aal_pkt_special_udf_set(AAL_PORT_ID_GE, spe_udf_msk, &spe_udf_cfg);

    /* set drop or forward */
    spe_pkt_cfg.dpid.dst_op = enable ? AAL_SPEC_DST_PORT : AAL_SPEC_DST_FE;
    spe_pkt_cfg.dpid.dpid = AAL_PORT_ID_CPU;
    spe_pkt_msk.u32 = 0;
    spe_pkt_msk.s.dpid = 1;
    spe_pkt_msk.s.cos = 1;
    spe_pkt_msk.s.rate_limit = 1;
   
    /* cos: 0,1~que 0, */
    /*      2,3~que 1, */
    /*      4,5~que 2, */
    /*      6,7~que 3, */
    spe_pkt_cfg.cos        = enable ?4 : 0;      
    spe_pkt_cfg.flow_en    = enable ?TRUE: FALSE; 
    spe_pkt_cfg.flow_id    = enable ?(__FLOW_ID_PRI(2)-AAL_RATE_LIMIT_FLOW_0): 0;
  
    aal_special_pkt_behavior_set(AAL_PORT_ID_GE, SDL_PKT_UDF3, spe_pkt_msk, &spe_pkt_cfg);

    return CS_E_OK;
}

cs_status epon_request_onu_loopdetect_block (
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_uint32                 device,
    CS_IN  cs_uint32                 llidport,
    CS_IN  cs_port_id_t              portid,
    CS_IN  cs_boolean                block
)
{
    rtk_api_ret_t rtk_ret = 0;
    rtk_stp_state_t stp_state;
    cs_status ret = CS_E_OK;
    int i;

    if((portid < CS_UNI_PORT_ID1) || ((portid > UNI_PORT_MAX) && (portid!=CS_ALL_UNI_PORT_ID)))
    {
        SDL_MIN_LOG("port id invalid.(%d) FILE: %s, LINE: %d", portid, __FILE__, __LINE__);
        ret = CS_E_PARAM;
        goto end;
    }

    stp_state = block ? STP_STATE_BLOCKING : STP_STATE_FORWARDING;

    if(portid!=CS_ALL_UNI_PORT_ID)
    {
        rtk_ret = rtk_stp_mstpState_set(__DEFAULT_MSTI, portid-1, stp_state);
        if(rtk_ret!=RT_ERR_OK)
        {
            SDL_MIN_LOG("rtk_stp_mstpState_set return %d. FILE: %s, LINE: %d", rtk_ret, __FILE__, __LINE__);
            ret = CS_E_ERROR;
            goto end;
        }
        ret = __l2_sw_uni_stp_block( portid, block);
    }
    else
    {
        for(i=0; i<4; ++i) 
        {
            rtk_ret = rtk_stp_mstpState_set(__DEFAULT_MSTI, i, stp_state);
            {
                SDL_MIN_LOG("rtk_stp_mstpState_set return %d. FILE: %s, LINE: %d", rtk_ret, __FILE__, __LINE__);
                ret = CS_E_ERROR;
                goto end;
            }
            ret = __l2_sw_uni_stp_block( i+1, block);
            if(ret)
                goto end;
        } 
    }    
end:
    return ret;
}

cs_status epon_request_onu_stp_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              portid,
    CS_IN cs_sdl_stp_state_t        state
)
{
    rtk_port_t rtk_port;
    rtk_stp_state_t stp_state;
    rtk_api_ret_t rtk_ret;
    cs_status ret = CS_E_OK;

    if((portid < CS_UNI_PORT_ID1) || (portid > UNI_PORT_MAX))
    {
        SDL_MIN_LOG("port id invalid.(%d) FILE: %s, LINE: %d", portid, __FILE__, __LINE__);

        ret = CS_E_PARAM; 
        goto end;
    }

    if(state > STP_S_FORWARDING)
    {
        SDL_MIN_LOG("stp state invalid.(%d) FILE: %s, LINE: %d", state, __FILE__, __LINE__);
        ret = CS_E_PARAM; 
        goto end;
    }

    rtk_port = portid - CS_UNI_PORT_ID1;
    stp_state = state;
    rtk_ret = rtk_stp_mstpState_set(__DEFAULT_MSTI, rtk_port, stp_state);

    if(rtk_ret!=RT_ERR_OK)
    {
        SDL_MIN_LOG("rtk_stp_mstpState_set return %d. FILE: %s, LINE: %d", rtk_ret, __FILE__, __LINE__);
        ret = CS_E_ERROR; 
        goto end;
    }

    if((stp_state == STP_S_BLOCKING)||
        (stp_state == STP_S_LEARNING))
    {
        // blocking port
        ret = __l2_sw_uni_stp_block( portid, TRUE);
    }
    else
    {
        // Free port
        ret=  __l2_sw_uni_stp_block( portid, FALSE);
    } 
        
end:
    return ret;
}

cs_status epon_request_onu_stp_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              portid,
    CS_OUT cs_sdl_stp_state_t        *state
)
{
    rtk_port_t rtk_port;
    rtk_stp_state_t stp_state;
    rtk_api_ret_t rtk_ret;

    if((portid < CS_UNI_PORT_ID1) || (portid > UNI_PORT_MAX))
    {
        SDL_MIN_LOG("port id invalid.(%d) FILE: %s, LINE: %d", portid, __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(NULL == state)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    rtk_port = portid - CS_UNI_PORT_ID1;
    rtk_ret = rtk_stp_mstpState_get(__DEFAULT_MSTI, rtk_port, &stp_state);
    if(rtk_ret!=RT_ERR_OK)
    {
        SDL_MIN_LOG("rtk_stp_mstpState_set return %d. FILE: %s, LINE: %d", rtk_ret, __FILE__, __LINE__);
        return CS_E_ERROR;
    }

    *state = stp_state;

    return CS_E_OK;
}

cs_status epon_request_onu_stp_ctrl_set (
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_uint32                 device,
    CS_IN  cs_uint32                 llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN  cs_boolean                enable
)
{
    rtk_mac_t mac;
    rtk_api_ret_t rtk_ret;
    rtk_trap_rma_action_t rma_act;

    /* let BPDU to CPU */
    mac.octet[0] = 0x01;
    mac.octet[1] = 0x80;
    mac.octet[2] = 0xc2;
    mac.octet[3] = 0x00;
    mac.octet[4] = 0x00;
    mac.octet[5] = 0x00;

    rma_act = enable ? RMA_ACTION_TRAP2CPU : RMA_ACTION_FORWARD;

    rtk_ret = rtk_trap_rmaAction_set(&mac, rma_act);
    if(rtk_ret!=RT_ERR_OK)
    {
        SDL_MIN_LOG("rtk_trap_rmaAction_set return %d. FILE: %s, LINE: %d", rtk_ret, __FILE__, __LINE__);
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

cs_status epon_request_onu_stp_ctrl_get (
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_uint32                 device,
    CS_IN  cs_uint32                 llidport,
     CS_IN cs_port_id_t              port_id,
    CS_OUT  cs_boolean                *enable
)
{
    rtk_mac_t mac;
    rtk_api_ret_t rtk_ret;
    rtk_trap_rma_action_t rma_act;

    /* BPDU */
    mac.octet[0] = 0x01;
    mac.octet[1] = 0x80;
    mac.octet[2] = 0xc2;
    mac.octet[3] = 0x00;
    mac.octet[4] = 0x00;
    mac.octet[5] = 0x00;

    rtk_ret = rtk_trap_rmaAction_get(&mac, &rma_act);
    if(rtk_ret!=RT_ERR_OK)
    {
        SDL_MIN_LOG("rtk_trap_rmaAction_get return %d. FILE: %s, LINE: %d", rtk_ret, __FILE__, __LINE__);
        return CS_E_ERROR;
    }

    *enable = (RMA_ACTION_TRAP2CPU==rma_act);

    return CS_E_OK;
}

cs_status epon_request_onu_spec_pkt_state_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_sdl_pkt_dir_t          direction,
    CS_IN cs_pkt_type_t             pkt_type,
    CS_IN cs_sdl_pkt_state_t        state
)
{
    cs_status                   ret = CS_E_OK;
   
    if(direction > S_BOTH)
    {
        SDL_MIN_LOG("Direction invalid.(%d) FILE: %s, LINE: %d", direction, __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(state > DST_CPU)
    {
        SDL_MIN_LOG("pkt state invalid.(%d) FILE: %s, LINE: %d", state, __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    if(pkt_type >= CS_PKT_TYPE_NUM)
    {
        SDL_MIN_LOG("pkt type invalid.(%d) FILE: %s, LINE: %d", pkt_type, __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(S_DOWNSTREAM==direction)
    {
        /* set PON(lynxd) chip */              
        ret = __l2_pon_spec_pkt_state_set(pkt_type, state);          
    }
    else if(S_UPSTREAM==direction) 
    {
        /* set Realtek switch chip */        
        ret = __l2_sw_uni_set(pkt_type, state);
        // set management port
        ret +=__l2_mii_port_set(pkt_type, state);
    }
    else// S_BOTH
    {
       if((pkt_type == CS_PKT_MPCP)||
          (pkt_type == CS_PKT_IP)||
          (pkt_type == CS_PKT_NDP)||
          (pkt_type == CS_PKT_LOOPDETECT))
        {
            SDL_MIN_LOG("The pkt type don't support both direction.(%d) FILE: %s, LINE: %d", pkt_type, __FILE__, __LINE__);
            return CS_E_PARAM;
        }

        ret = __l2_pon_spec_pkt_state_set(pkt_type, state);  
        /* set Realtek switch chip */        
        ret += __l2_sw_uni_set(pkt_type, state);
        // set management port
        ret +=__l2_mii_port_set(pkt_type, state);             
    }
    
    if(ret != CS_E_OK)
    {
        SDL_MIN_LOG("pkt type(%d) set error. FILE: %s, LINE: %d", pkt_type, __FILE__, __LINE__);
        return CS_E_ERROR;
    }

    // record
    if (S_BOTH == direction)
    {
        g_pkt_state[S_UPSTREAM][pkt_type]   = state;
        g_pkt_state[S_DOWNSTREAM][pkt_type] = state;
    }
    else
       g_pkt_state[direction][pkt_type] = state;     

    if((S_BOTH == direction)||(S_DOWNSTREAM == direction))
    {
        if(DST_CPU == state)
        {
            ret = __l2_ds_pkt_pri_set( pkt_type, g_ds_pkt_queue[pkt_type]);    
        }
        else
        {
            ret = __l2_ds_pkt_def_set( pkt_type);
        }
    }
        
    return ret;
}

cs_status epon_request_onu_spec_pkt_state_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_sdl_pkt_dir_t          direction,
    CS_IN cs_pkt_type_t             pkt_type,
    CS_OUT cs_sdl_pkt_state_t       *state
)
{
    if(direction >= S_BOTH)
    {
        SDL_MIN_LOG("Direction invalid.(%d) FILE: %s, LINE: %d", direction, __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(pkt_type >= CS_PKT_TYPE_NUM)
    {
        SDL_MIN_LOG("pkt type invalid.(%d) FILE: %s, LINE: %d", pkt_type, __FILE__, __LINE__);
        return CS_E_PARAM;
    }

    *state = g_pkt_state[direction][pkt_type];
   
    return CS_E_OK;
}

cs_status epon_request_onu_pkt_cpu_queue_rate_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_sdl_queue_t            queue,
    CS_IN cs_uint32                 rate
)
{   
    cs_status     ret     = CS_E_OK;
  
    if(queue > QUEUE_3)
        return CS_E_PARAM;

    if(rate > __PKT_RATE_MAX)
        return CS_E_PARAM;

    ret =  __l2_pkt_rate_set( queue, rate);
    if(ret)
        return ret;
    
    g_queue_pkt_rate [queue] = rate;
        
    return CS_E_OK;
}

cs_status epon_request_onu_pkt_cpu_queue_rate_get(
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_int32                  device_id,
    CS_IN  cs_int32                  llidport,
    CS_IN  cs_sdl_queue_t            queue,
    CS_OUT cs_uint32                 *rate    
)
{   
    
    if(queue > QUEUE_3)
        return CS_E_PARAM;
        
    if(rate == NULL)
        return CS_E_PARAM;

    *rate = g_queue_pkt_rate [queue];

    return CS_E_OK;
}

cs_status epon_request_onu_pkt_cpu_queue_map_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_sdl_pkt_dir_t          direction,
    CS_IN cs_pkt_type_t             pkt_type,
    CS_IN cs_sdl_queue_t            queue
)
{       
    cs_status     ret     = CS_E_OK;
   
     /* only support downstream */
    if(direction != S_DOWNSTREAM)
        return CS_E_NOT_SUPPORT;

    if(pkt_type >= CS_PKT_TYPE_NUM)
        return CS_E_PARAM;
      
    if(queue > QUEUE_3)
        return CS_E_PARAM;

    if((pkt_type == CS_PKT_IP)||
        (pkt_type == CS_PKT_LOOPDETECT))
        return CS_E_NOT_SUPPORT;
        
    if(queue == g_ds_pkt_queue[pkt_type])
        return CS_E_OK;

    if(g_pkt_state[direction][pkt_type] == DST_CPU)
    {
        ret = __l2_ds_pkt_pri_set( pkt_type, queue);
        if(ret)
            return ret;
    }

    g_ds_pkt_queue[pkt_type] = queue;
   
    return CS_E_OK;
}

cs_status epon_request_onu_pkt_cpu_queue_map_get(
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_int32                  device_id,
    CS_IN  cs_int32                  llidport,
    CS_IN  cs_sdl_pkt_dir_t          direction,
    CS_IN  cs_pkt_type_t             pkt_type,
    CS_OUT cs_sdl_queue_t            *queue
)
{   
    if (NULL == queue)    
        return CS_E_PARAM;

     /* only support downstream */
    if(direction != S_DOWNSTREAM)
        return CS_E_NOT_SUPPORT;

    if(pkt_type >= CS_PKT_TYPE_NUM)
        return CS_E_PARAM;
    
    *queue = g_ds_pkt_queue[pkt_type];
 
    return CS_E_OK;
}

// fo fwd packet with ether type 0x8899 from reateltek switch
//#define SDL_VLAN_SW_RTK                    10 // total 12 entries
cs_status sdl_switch_init(void)
{
    rtk_api_ret_t rtn = RT_ERR_OK;
    rtk_priority_select_t pridec;
    cs_aal_cl_rule_cfg_t ce_entry;
    cs_aal_cl_fib_data_t fibdata;
    cs_uint16 fib_index = 0;
    cs_uint16 rule_offset = 0; //DOMAIN_ID_SWITCH has onlu one rule
    int i;
    cs_callback_context_t  context;

    diag_printf("sdl swich chip is being initialized   ... ");
    
    if ((rtn = rtk_switch_init())!= RT_ERR_OK)
        goto end;

    if ((rtn = rtk_l2_init()) != RT_ERR_OK)
        goto end;
        
    if ((rtn = rtk_vlan_init()) != RT_ERR_OK)
        goto end;
    
    if ((rtn = rtk_svlan_init()) != RT_ERR_OK)
        goto end;

    rtn = rtk_cpu_enable_set(ENABLED);
    if(rtn){
        goto end;
    }
      
    rtn = rtk_cpu_tagPort_set(6, CPU_INSERT_TO_TRAPPING);
    if(rtn){
        goto end;
    }
       
    rtn = rtk_qos_init(8);
    if(rtn){
        goto end;
    }
    memset(&pridec, 0, sizeof(rtk_priority_select_t));
       //ACL>802.1p>>DSCP>Port>SVLAN>CVLAN>DMAC>SMAC
    pridec.port_pri = 4;
    pridec.acl_pri = 7;
    pridec.dscp_pri = 3;
    pridec.dot1q_pri = 5;
    pridec.svlan_pri = 6;
    pridec.cvlan_pri = 2;
    pridec.dmac_pri = 1;
    pridec.smac_pri = 0;
    rtn =rtk_qos_priSel_set(&pridec);
    if(rtn){
       goto end;
    }
    
    rtn = rtk_filter_igrAcl_init();
    if(rtn){
       goto end;
    }

     /* attach cpu tx and rx parser */
    __ma_rx_parser_hook_reg(rtk_cpu_pkt_rx);
    __ma_tx_parser_hook_reg(rtk_cpu_pkt_tx);

    /*
    ** create a rule for capturing packet with ether type is 0x8899 to CPU
    ** this kind of packet comes from reateltek switch
    */
    memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));  
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t)); 

    ce_entry.valid = 1;
    ce_entry.key_type = AAL_CL_PROTO_KEY;  

    ce_entry.cl_key.proco_key[0].mask.s.ev2pt  = 1;
    ce_entry.cl_key.proco_key[0].ev2pt         = 0x8899;
     
    fibdata.mask.u32 = 0;
    fibdata.mask.s.dpid       = 1;
    fibdata.mask.s.cos        = 1;
    fibdata.mask.s.rate_limit = 1;
    fibdata.valid             = 1;
    fibdata.permit            = 1;
    fibdata.permit_filter_dis = 1;
    fibdata.permit_pri        = 1;
    fibdata.dpid.dst_op       = AAL_CL_DST_PORT;
    fibdata.dpid.dpid         = AAL_PORT_ID_CPU;
    fibdata.cos_sel           = AAL_CL_COS_SEL_CL;
    fibdata.cos               = 4; /*4,5 mapp to que 2 of CPU*/
    fibdata.flowId_sel        = AAL_CL_FLOWID_SEL_CL_RSLT; 
    fibdata.flowid            = (__FLOW_ID_PRI(0)-AAL_RATE_LIMIT_FLOW_0); 
    fibdata.cl_rate_limiter_bypass = 0; 

    if (aal_cl_rule_set(AAL_PORT_ID_GE, DOMAIN_ID_SWITCH, rule_offset, &ce_entry) != CS_E_OK)
        goto end;

    
    //fib_index = SDL_VLAN_KEY_NUM_ENTRY * rule_offset + 0;
    fib_index = 8 * rule_offset + 0;
    if (aal_cl_fib_set(AAL_PORT_ID_GE, DOMAIN_ID_SWITCH, fib_index, &fibdata) != CS_E_OK)
        goto end;
    
    /*For SP2C priority, eable 802.1p remarking*/	
    for(i = 0; i < 4; ++i)
    {
        rtk_qos_1pRemarkEnable_set(i, EPON_TRUE);
    }
   
    for(i = 0; i < 8; ++i)
    {
        rtk_qos_1pRemark_set(i, i);
    }
    /*for the throughput and scheduling issue, suggested by realtek engineers*/
    for(i = 0; i<8; i++)
    {
        cs_plat_ssp_switch_write(context,0,0,0x200+i, 0x24); 
    }
    
    cs_plat_ssp_switch_write(context,0,0,0x219, 0x18); 

   /*for TCP throughput work around, default value is 0xed*/ 
    cs_plat_ssp_switch_write(context,0,0,0x1203, 0x12); 

    diag_printf("[done]\n");
    return CS_E_OK;

end:
    diag_printf("[fail]\n");
    return CS_E_ERROR;      
}

cs_status sdl_l2_init(void)
{
    cs_aal_fdb_port_cfg_t port_cfg;
    cs_aal_fdb_port_msk_t port_msk;
    cs_callback_context_t context;
    cs_aal_port_id_t      port;
    cs_aal_spec_pkt_ctrl_msk_t  sp_msk;
    cs_aal_spec_pkt_ctrl_t      sp_ctrl;
    cs_int32              i   = 0;
    cs_pkt_type_t         pkt_type;
    cs_status             ret = CS_E_OK;
    cs_uint32 queue_rate[4] = {
                            300,   /* queue 0 */
                            500,   /* queue 1 */
                            600,   /* queue 2 */
                            100    /* queue 3 */
    };
    /******Downstream OAM,802.1X packet will be mapped to queue3 ****/
    cs_sdl_queue_t downstream_pkt[CS_PKT_TYPE_NUM] = {
                            QUEUE_0,   /** CS_PKT_BPDU       */     
                            QUEUE_3,   /** CS_PKT_8021X      */    
                            QUEUE_2,   /** CS_PKT_GMP        */    
                            QUEUE_0,   /** CS_PKT_ARP        */    
                            QUEUE_3,   /** CS_PKT_OAM        */    
                            QUEUE_2,   /** CS_PKT_MPCP       */    
                            QUEUE_1,   /** CS_PKT_DHCP       */    
                            QUEUE_0,   /** CS_PKT_IROS       */    
                            QUEUE_1,   /** CS_PKT_PPPOE      */  
                            QUEUE_1,   /** CS_PKT_PPPOE      */ 
                            QUEUE_0,   /** CS_PKT_IP         */    
                            QUEUE_0,   /** CS_PKT_NDP        */    
                            QUEUE_0,   /** CS_PKT_LOOPDETECT */    
                            QUEUE_0    /** CS_PKT_MYMAC      */    
    };
    diag_printf("sdl l2 is being initialized   ... ");

    /* Reduce the priority of the MYMAC packet*/
    /*   27 to 2 */
    memset(&sp_ctrl, 0, sizeof(cs_aal_spec_pkt_ctrl_t));
    sp_msk.u32     = 0;
    sp_msk.s.pri   = 1;

    sp_ctrl.pri    = 2;
    for (port = AAL_PORT_ID_GE; port <= AAL_PORT_ID_MII1; ++port) {
        ret = aal_special_pkt_behavior_set(port, AAL_PKT_MYMAC, sp_msk, &sp_ctrl);
        if(ret)
            return ret;
    }
    
    memset(&g_ds_pkt_queue, 0, sizeof(g_ds_pkt_queue));
    memset(&g_queue_pkt_rate, 0, sizeof(g_queue_pkt_rate));

    for (i = 0; i <= QUEUE_3; i++) 
    {
        ret = epon_request_onu_pkt_cpu_queue_rate_set( context, 0, 0, i, queue_rate[i]);
        if(ret)
            return ret;
    }

    for (pkt_type = CS_PKT_BPDU; pkt_type < CS_PKT_TYPE_NUM; pkt_type++) 
    {    
        if((pkt_type == CS_PKT_IP)||(pkt_type == CS_PKT_LOOPDETECT))
            continue;
        
        ret = epon_request_onu_pkt_cpu_queue_map_set( context, 0, 0, S_DOWNSTREAM, 
                                                      pkt_type, 
                                                      downstream_pkt[pkt_type]);
        if(ret)
            return ret;   
    }

    
    /* enable learning function for frames destinated to CPU */
    memset(&port_cfg, 0, sizeof(port_cfg));
    memset(&port_msk, 0, sizeof(port_msk));

    port_msk.s.learn_dpid_cpu_dis = 1;
    port_cfg.learn_dpid_cpu_dis   = 0;
    (void)aal_fdb_port_cfg_set(AAL_PORT_ID_GE, port_msk, &port_cfg);
    
    g_stp_blockport_status = 0;
     
    diag_printf("[done]\n");
    return CS_E_OK;
}

