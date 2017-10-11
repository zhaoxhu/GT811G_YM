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
#include "plat_common.h" 
#include "sdl_vlan_util.h"
#include "sdl_util.h"
#include "sdl.h"
#include "aal_cls.h"
#include "aal_pon.h"
#include "aal_l2.h"
#include "aal_flow.h"
#include "sdl_vlan.h"


// for single port onu, so only two port : UNI port 1 and PON port 

cs_sdl_p_tag_type_t     g_ptag_type[2]={SDL_P_TAG_VLAN,SDL_P_TAG_VLAN};
cs_uint16               g_def_vlanid[2]= {0,0};
cs_boolean              g_pvid_drop_en[2];
cs_boolean              g_pvid_in_untag_en[2];
cs_sdl_untag_ctrl_t     g_untag_status[2];
cs_sdl_p_tag_ctrl_t     g_ptag_status[2];
cs_sdl_unk_vlan_ctrl_t  g_unk_vlan_status[2]; 
cs_uint8                g_tpid_sel = 1;
cs_uint8                g_tbl_maxsize = SDL_VLAN_TABLE_MAX_ENTRY;
cs_uint8                g_tbl_start = SDL_VLAN_TABLE_START_ENTRY;
cs_uint8                g_tbl_keytype = AAL_CL_VLAN_KEY; // or AAL_CL_VLAN_KEY_E
cs_boolean              g_cos_sel_en = 0; // temp handle

cs_sdl_vlan_member_port_t  g_vlan_member[2];


static cs_boolean __vlan_find_record(
    CS_IN   cs_port_id_t               port_id, 
    CS_IN   cs_sdl_vlan_oper_t         *entry,
    CS_OUT  cs_uint8                   *index
)
{
    cs_int32 i;

    if((port_id>CS_UNI_PORT_ID1)||(entry==NULL)||(index==NULL))
    {    
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
          
    for(i=0; i<g_vlan_member[port_id].count; i++)
    {
        if(g_vlan_member[port_id].vlan_entry[i].vlan_id == entry->vlan_id)
        {     
            *index = (cs_uint8)i;
            return TRUE;
        }
    }
     
    return FALSE;
}

static cs_status __vlan_pvid_rule_update 
(
    CS_IN cs_port_id_t                port,
    CS_IN cs_uint16                   pvid  
)
{   
    cs_aal_port_id_t     port_idx;
    cs_aal_cl_rule_cfg_t rule;
    cs_aal_cl_fib_data_t fib;
    cs_status ret = CS_E_OK;
    
    if((port>CS_UNI_PORT_ID1)||(pvid>SDL_VLAN_MAX_ID))
    {    
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    port_idx = AAL_PORT(port); 
      
     //default vlan rule 
    memset(&rule, 0, sizeof(cs_aal_cl_rule_cfg_t));
    memset(&fib, 0, sizeof(cs_aal_cl_fib_data_t));

    ret = aal_cl_rule_get(port_idx,DOMAIN_ID_VLAN,SDL_VLAN_DEF_RULE,&rule);
    if(ret)
    {    
        SDL_MIN_LOG("get default rule error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    rule.valid = 1;
    rule.key_type = AAL_CL_VLAN_KEY;

    // entry key of vlan default rule:4 for PVID
    memset(&rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY], 0, sizeof(cs_aal_cl_vlan_key_t));

    rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY].mask.s.vlanid= 1;
    rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY].vlanid = pvid;
    if(pvid == 0)
    {
        rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY].mask.s.p_tag_flag= 1;
        rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY].p_tag_flag = 1;
    }
    
    fib.valid = 1;
    if(g_pvid_drop_en[port])
    {
        fib.permit = 0;
        fib.permit_filter_dis = 0;
        fib.permit_pri = 1;
    }
    else
    {
        fib.permit = 1;
        fib.permit_filter_dis = 0;
        fib.permit_pri = 0;
    }
    fib.mask.s.vlan_op = 1;
    if(g_pvid_in_untag_en[port])
    {
        fib.inner_vlan_cmd  = AAL_CL_VLAN_CMD_NOP;
        fib.top_vlan_cmd    = AAL_CL_VLAN_CMD_POP;
        fib.eg_chk_vlan_sel = AAL_CL_EG_VLAN_CHECK_NO;       
    }
    else
    {
        fib.inner_vlan_cmd  = AAL_CL_VLAN_CMD_NOP;
        fib.top_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
        fib.eg_chk_vlan_sel = AAL_CL_EG_VLAN_CHECK_NO;       
    }
    fib.mask.s.cos = (g_cos_sel_en ==TRUE)?1:0;
    fib.cos_sel    = AAL_CL_COS_SEL_MAP_RX_DOT1P;
    
  
    ret = aal_cl_fib_set(port_idx,DOMAIN_ID_VLAN, SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_DEF_RULE+VLAN_DEF_PVLAN_KEY,&fib);
    ret += aal_cl_rule_set(port_idx,DOMAIN_ID_VLAN,SDL_VLAN_DEF_RULE,&rule);
    if (ret)
    {  
        return CS_E_ERROR;
    }

    return CS_E_OK;
}


////////////////////////public////////////////////////////////////////////

cs_status vlan_ptag_type_set(
    CS_IN cs_port_id_t         port_id, 
    CS_IN cs_sdl_p_tag_type_t  type
)
{
    if((port_id>CS_UNI_PORT_ID1)||(type>SDL_P_TAG_UNTAG))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  
    
    g_ptag_type[port_id] = type;
    
    return CS_E_OK;
}

cs_status vlan_ptag_type_get(
    CS_IN  cs_port_id_t         port_id, 
    CS_OUT cs_sdl_p_tag_type_t  *type
)
{
    if((port_id>CS_UNI_PORT_ID1)||(type ==NULL))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  
    
    *type = g_ptag_type[port_id];
    
    return CS_E_OK;
}
// for zero vlan p_tag
cs_status vlan_ptag_ctrl_set(
    CS_IN cs_port_id_t         port_id, 
    CS_IN cs_sdl_p_tag_ctrl_t  ptag_status    
)
{
    cs_aal_cl_rule_cfg_t rule;
    cs_aal_cl_fib_data_t  fib;
    cs_aal_port_id_t     port;
    cs_uint16        pvid = 0;  
    cs_status   ret = CS_E_OK;
    
    if((port_id>CS_UNI_PORT_ID1)||(ptag_status>SDL_PTAG_CTRL_MAX))
    {
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    port = AAL_PORT(port_id); 
    
    pvid = g_def_vlanid[port_id];
    
    memset(&rule, 0, sizeof(cs_aal_cl_rule_cfg_t));
    memset(&fib, 0, sizeof(cs_aal_cl_fib_data_t));
   
    ret = aal_cl_rule_get(port,DOMAIN_ID_VLAN,SDL_VLAN_DEF_RULE,&rule);
    if(ret)
    {   
        SDL_MIN_LOG("aal_cl_rule_get error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    rule.valid = 1;
    rule.key_type = AAL_CL_VLAN_KEY;
    
    // entry key of vlan default rule: 3 for p_tag vlan ctrl
    memset(&rule.cl_key.vlan_key[VLAN_DEF_PTAG_VLAN_KEY], 0, sizeof(cs_aal_cl_vlan_key_t));
   
    rule.cl_key.vlan_key[VLAN_DEF_PTAG_VLAN_KEY].mask.s.p_tag_flag= 1;
    rule.cl_key.vlan_key[VLAN_DEF_PTAG_VLAN_KEY].p_tag_flag       = 1;
    fib.mask.u32 = 0;
    fib.valid    = 1;
    
    switch(ptag_status)
    {
        case SDL_PTAG_DROP:
        {    
            fib.permit            = 0;
            fib.permit_filter_dis = 0;
            fib.permit_pri        = 1;
            break;
        }
        case SDL_PTAG_FWD:
        {    
            fib.permit            = 1;
            fib.permit_filter_dis = 0;
            fib.permit_pri        = 0;
            fib.mask.s.cos        = (g_cos_sel_en ==TRUE)?1:0;
            fib.cos_sel           = AAL_CL_COS_SEL_MAP_RX_DOT1P;          
            break;
        }
        case SDL_PTAG_FWD_UNTAG:
        {    
            fib.permit            = 1;
            fib.permit_filter_dis = 0;
            fib.permit_pri        = 0;
            fib.mask.s.vlan_op    = 1;
            fib.inner_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
            fib.top_vlan_cmd      = AAL_CL_VLAN_CMD_POP;    
            fib.eg_chk_vlan_sel   = AAL_CL_EG_VLAN_CHECK_NO;  
            fib.mask.s.cos        = (g_cos_sel_en ==TRUE)?1:0;
            fib.cos_sel           = AAL_CL_COS_SEL_MAP_RX_DOT1P;
            break;
        }
        case SDL_PTAG_FWD_TAG:
        {    
            fib.permit            = 1;
            fib.permit_filter_dis = 0;
            fib.permit_pri        = 0;
            fib.mask.s.vlan_op    = 1;
            fib.inner_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
            fib.top_vlan_cmd      = AAL_CL_VLAN_CMD_PUSH;
            fib.top_vlanid_sel    = AAL_CL_VLANID_SEL_CL;
            fib.top_vlanid        = pvid;
            if(port == AAL_PORT_ID_PON)
            {
                fib.top_tpid_sel  = AAL_CL_TPID_SEL_8100;
            }
            else
            {
                fib.top_tpid_sel  = g_tpid_sel;
            }
            fib.eg_chk_vlan_sel   = AAL_CL_EG_VLAN_CHECK_NO;  
            fib.mask.s.dot1p      = 1;
            fib.top_8021p_sel     = AAL_CL_DOT1P_SEL_CL;  
            fib.top_8021p         = 0; 
            fib.mask.s.cos        = (g_cos_sel_en ==TRUE)?1:0;
            fib.cos_sel           = AAL_CL_COS_SEL_MAP_RX_DOT1P;      
            break;
        }
        default:
            return CS_E_PARAM;
    }
    
 
    ret = aal_cl_fib_set(port,DOMAIN_ID_VLAN, SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_DEF_RULE+VLAN_DEF_PTAG_VLAN_KEY,&fib);
    ret += aal_cl_rule_set(port,DOMAIN_ID_VLAN,SDL_VLAN_DEF_RULE,&rule);
    if(ret)
    {   
        SDL_MIN_LOG(" cl rule set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    } 
    
    g_ptag_status[port_id] = ptag_status;
        
    return CS_E_OK;
}

cs_status vlan_ptag_ctrl_get(
    CS_IN  cs_port_id_t         port_id, 
    CS_OUT cs_sdl_p_tag_ctrl_t  *ptag_status    
)
{

    if((port_id>CS_UNI_PORT_ID1)||(ptag_status ==NULL))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  
    
    *ptag_status = g_ptag_status[port_id];
    
    return CS_E_OK;
}

cs_status vlan_untag_ctrl_set(
    CS_IN cs_port_id_t         port_id, 
    CS_IN cs_sdl_untag_ctrl_t  untag_status    
)
{
    cs_aal_cl_rule_cfg_t rule;
    cs_aal_cl_fib_data_t fib[3];
    cs_aal_flow_mapping_t qos_cfg;
    cs_aal_port_id_t      port;
    cs_uint16     pvid = 0;  
    cs_uint16 fibindex = 0;
    cs_uint32        i = 0;
    cs_status ret = CS_E_OK;
    
    if((port_id>CS_UNI_PORT_ID1)||(untag_status>SDL_UNTAG_CTRL_MAX))
    {
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    port = AAL_PORT(port_id); 
    
    pvid = g_def_vlanid[port_id];
    
    memset(&rule, 0, sizeof(cs_aal_cl_rule_cfg_t));
    memset(&fib[0], 0, sizeof(fib));
    memset(&qos_cfg, 0, sizeof(cs_aal_flow_mapping_t));

    if(untag_status >= SDL_UNTAG_FWD_TAG)
    {   
        // workaround for TX dot1p map to cos
        ret = aal_flow_dot1p_mapping_get(port,&qos_cfg);
        if(ret)
        {   
            SDL_MIN_LOG("aal_flow_dot1p_mapping_get error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_PARAM;
        }
    }
    
    ret = aal_cl_rule_get(port,DOMAIN_ID_VLAN,SDL_VLAN_DEF_RULE,&rule);
    if(ret)
    {   
        SDL_MIN_LOG("aal_cl_rule_get error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    rule.valid = 1;
    rule.key_type = AAL_CL_VLAN_KEY;
    
    // entry key of vlan default rule: 5,6,7 for untag ctrl
    memset(&rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_0], 0, 3*sizeof(cs_aal_cl_vlan_key_t));
   
    rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_0].mask.s.tpid_enc= 1;
    rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_0].tpid_enc       = 0;
    fib[0].mask.u32 = 0;
    fib[0].valid    = 1;
    
    switch(untag_status)
    {
        case SDL_UNTAG_DROP:
        {    
            fib[0].permit            = 0;
            fib[0].permit_filter_dis = 0;
            fib[0].permit_pri        = 1;
            break;
        }
        case SDL_UNTAG_FWD:
        {    
            fib[0].permit            = 1;
            fib[0].permit_filter_dis = 0;
            fib[0].permit_pri        = 0;
            fib[0].mask.s.cos        = (g_cos_sel_en ==TRUE)?1:0;
            fib[0].cos_sel           = AAL_CL_COS_SEL_MAP_RX_DOT1P;           
            break;
        }
        case SDL_UNTAG_FWD_TAG:
        {    
            fib[0].permit            = 1;
            fib[0].permit_filter_dis = 0;
            fib[0].permit_pri        = 0;
            fib[0].mask.s.vlan_op    = 1;
            fib[0].inner_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
            fib[0].top_vlan_cmd      = AAL_CL_VLAN_CMD_PUSH;
            fib[0].top_vlanid_sel    = AAL_CL_VLANID_SEL_CL;
            fib[0].top_vlanid        = pvid;
            if(port == AAL_PORT_ID_PON)
            {
                fib[0].top_tpid_sel  = AAL_CL_TPID_SEL_8100;
            }
            else
            {
                fib[0].top_tpid_sel  = g_tpid_sel;
            }
            fib[0].eg_chk_vlan_sel   = AAL_CL_EG_VLAN_CHECK_NO;  
            fib[0].mask.s.dot1p      = 1;
            fib[0].top_8021p_sel     = AAL_CL_DOT1P_SEL_CL;  
            fib[0].top_8021p         = 0; 
            fib[0].mask.s.cos        = (g_cos_sel_en ==TRUE)?1:0;
            fib[0].cos_sel           = AAL_CL_COS_SEL_CL;
            fib[0].cos               = qos_cfg.cos.s.map0;// cos= TXdot1p pri0--cos     
            break;
        }
        case SDL_UNTAG_FWD_TAG_IPV4:
        {    
            fib[0].permit            = 1;
            fib[0].permit_filter_dis = 0;
            fib[0].permit_pri        = 0;
            fib[0].mask.s.vlan_op    = 1;
            fib[0].inner_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
            fib[0].top_vlan_cmd      = AAL_CL_VLAN_CMD_PUSH;
            fib[0].top_vlanid_sel    = AAL_CL_VLANID_SEL_CL;
            fib[0].top_vlanid        = pvid;
            if(port == AAL_PORT_ID_PON)
            {
                fib[0].top_tpid_sel  = AAL_CL_TPID_SEL_8100;
            }
            else
            {
                fib[0].top_tpid_sel  = g_tpid_sel;
            }
            fib[0].eg_chk_vlan_sel   = AAL_CL_EG_VLAN_CHECK_NO;  
            fib[0].mask.s.dot1p      = 1;
            fib[0].top_8021p_sel     = AAL_CL_DOT1P_SEL_CL;  
            fib[0].top_8021p         = 0; 
            fib[0].mask.s.cos        = 1;                  // follow is only for NTT
            fib[0].cos_sel           = AAL_CL_COS_SEL_CL;
            fib[0].cos               = qos_cfg.cos.s.map0; // cos= TXdot1p pri0--cos
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_1].mask.s.tpid_enc = 1;
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_1].tpid_enc        = 0;
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_1].mask.s.ev2pt_enc= 1;
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_1].ev2pt_enc       = 0;
            fib[1].mask.u32          = 0;
            fib[1].valid             = 1;
            fib[1].permit            = 1;
            fib[1].permit_filter_dis = 0;
            fib[1].permit_pri        = 0;
            fib[1].mask.s.vlan_op    = 1;
            fib[1].inner_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
            fib[1].top_vlan_cmd      = AAL_CL_VLAN_CMD_PUSH;
            fib[1].top_vlanid_sel    = AAL_CL_VLANID_SEL_CL;
            fib[1].top_vlanid        = pvid;
            if(port == AAL_PORT_ID_PON)
            {
                fib[1].top_tpid_sel  = AAL_CL_TPID_SEL_8100;
            }
            else
            {
                fib[1].top_tpid_sel  = g_tpid_sel;
            }
            fib[1].eg_chk_vlan_sel   = AAL_CL_EG_VLAN_CHECK_NO;  
            fib[1].mask.s.dot1p      = 1;
            fib[1].top_8021p_sel     = AAL_CL_DOT1P_SEL_MAP_RX_DSCP;  
            fib[1].mask.s.cos        = 1;
            fib[1].cos_sel           = AAL_CL_COS_SEL_MAP_RX_DSCP;
            break;
        }
        case SDL_UNTAG_FWD_TAG_IPV6:
        {    
            fib[0].permit            = 1;
            fib[0].permit_filter_dis = 0;
            fib[0].permit_pri        = 0;
            fib[0].mask.s.vlan_op    = 1;
            fib[0].inner_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
            fib[0].top_vlan_cmd      = AAL_CL_VLAN_CMD_PUSH;
            fib[0].top_vlanid_sel    = AAL_CL_VLANID_SEL_CL;
            fib[0].top_vlanid        = pvid;
            if(port == AAL_PORT_ID_PON)
            {
                fib[0].top_tpid_sel  = AAL_CL_TPID_SEL_8100;
            }
            else
            {
                fib[0].top_tpid_sel  = g_tpid_sel;
            }
            fib[0].eg_chk_vlan_sel   = AAL_CL_EG_VLAN_CHECK_NO;  
            fib[0].mask.s.dot1p      = 1;
            fib[0].top_8021p_sel     = AAL_CL_DOT1P_SEL_CL;  
            fib[0].top_8021p         = 0; 
            fib[0].mask.s.cos        = 1;
            fib[0].cos_sel           = AAL_CL_COS_SEL_CL;
            fib[0].cos               = qos_cfg.cos.s.map0; // cos= TXdot1p pri0--cos
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_1].mask.s.tpid_enc = 1;
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_1].tpid_enc        = 0;
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_1].mask.s.ev2pt_enc= 1;
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_1].ev2pt_enc       = 1;
            fib[1].mask.u32          = 0;
            fib[1].valid             = 1;
            fib[1].permit            = 1;
            fib[1].permit_filter_dis = 0;
            fib[1].permit_pri        = 0;
            fib[1].mask.s.vlan_op    = 1;
            fib[1].inner_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
            fib[1].top_vlan_cmd      = AAL_CL_VLAN_CMD_PUSH;
            fib[1].top_vlanid_sel    = AAL_CL_VLANID_SEL_CL;
            fib[1].top_vlanid        = pvid;
            if(port == AAL_PORT_ID_PON)
            {
                fib[1].top_tpid_sel  = AAL_CL_TPID_SEL_8100;
            }
            else
            {
                fib[1].top_tpid_sel  = g_tpid_sel;
            }
            fib[1].eg_chk_vlan_sel   = AAL_CL_EG_VLAN_CHECK_NO;  
            fib[1].mask.s.dot1p      = 1;
            fib[1].top_8021p_sel     = AAL_CL_DOT1P_SEL_MAP_RX_DSCP;  
            fib[1].mask.s.cos        = 1;
            fib[1].cos_sel           = AAL_CL_COS_SEL_MAP_RX_DSCP;
            break;
        }
        case SDL_UNTAG_FWD_TAG_IP:
        {    
            fib[0].permit            = 1;
            fib[0].permit_filter_dis = 0;
            fib[0].permit_pri        = 0;
            fib[0].mask.s.vlan_op    = 1;
            fib[0].inner_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
            fib[0].top_vlan_cmd      = AAL_CL_VLAN_CMD_PUSH;
            fib[0].top_vlanid_sel    = AAL_CL_VLANID_SEL_CL;
            fib[0].top_vlanid        = pvid;
            if(port == AAL_PORT_ID_PON)
            {
                fib[0].top_tpid_sel  = AAL_CL_TPID_SEL_8100;
            }
            else
            {
                fib[0].top_tpid_sel  = g_tpid_sel;
            }
            fib[0].eg_chk_vlan_sel   = AAL_CL_EG_VLAN_CHECK_NO;  
            fib[0].mask.s.dot1p      = 1;
            fib[0].top_8021p_sel     = AAL_CL_DOT1P_SEL_CL;  
            fib[0].top_8021p         = 0; 
            fib[0].mask.s.cos        = 1;
            fib[0].cos_sel           = AAL_CL_COS_SEL_CL;
            fib[0].cos               = qos_cfg.cos.s.map0;// cos= TXdot1p pri0--cos
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_1].mask.s.tpid_enc = 1;
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_1].tpid_enc        = 0;
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_1].mask.s.ev2pt_enc= 1;
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_1].ev2pt_enc       = 0;
            fib[1].mask.u32          = 0;
            fib[1].valid             = 1;
            fib[1].permit            = 1;
            fib[1].permit_filter_dis = 0;
            fib[1].permit_pri        = 0;
            fib[1].mask.s.vlan_op    = 1;
            fib[1].inner_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
            fib[1].top_vlan_cmd      = AAL_CL_VLAN_CMD_PUSH;
            fib[1].top_vlanid_sel    = AAL_CL_VLANID_SEL_CL;
            fib[1].top_vlanid        = pvid;
            if(port == AAL_PORT_ID_PON)
            {
                fib[1].top_tpid_sel  = AAL_CL_TPID_SEL_8100;
            }
            else
            {
                fib[1].top_tpid_sel  = g_tpid_sel;
            }
            fib[1].eg_chk_vlan_sel   = AAL_CL_EG_VLAN_CHECK_NO;  
            fib[1].mask.s.dot1p      = 1;
            fib[1].top_8021p_sel     = AAL_CL_DOT1P_SEL_MAP_RX_DSCP;  
            fib[1].mask.s.cos        = 1;
            fib[1].cos_sel           = AAL_CL_COS_SEL_MAP_RX_DSCP;
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_2].mask.s.tpid_enc = 1;
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_2].tpid_enc        = 0;
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_2].mask.s.ev2pt_enc= 1;
            rule.cl_key.vlan_key[VLAN_DEF_UNTAG_KEY_2].ev2pt_enc       = 1;
            fib[2].mask.u32          = 0;
            fib[2].valid             = 1;
            fib[2].permit            = 1;
            fib[2].permit_filter_dis = 0;
            fib[2].permit_pri        = 0;
            fib[2].mask.s.vlan_op    = 1;
            fib[2].inner_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
            fib[2].top_vlan_cmd      = AAL_CL_VLAN_CMD_PUSH;
            fib[2].top_vlanid_sel    = AAL_CL_VLANID_SEL_CL;
            fib[2].top_vlanid        = pvid;
            if(port == AAL_PORT_ID_PON)
            {
                fib[2].top_tpid_sel  = AAL_CL_TPID_SEL_8100;
            }
            else
            {
                fib[2].top_tpid_sel  = g_tpid_sel;
            }
            fib[2].eg_chk_vlan_sel   = AAL_CL_EG_VLAN_CHECK_NO;  
            fib[2].mask.s.dot1p      = 1;
            fib[2].top_8021p_sel     = AAL_CL_DOT1P_SEL_MAP_RX_DSCP;  
            fib[2].mask.s.cos        = 1;
            fib[2].cos_sel           = AAL_CL_COS_SEL_MAP_RX_DSCP;
            break;
        }
        default:
            return CS_E_PARAM;
    }
    
 
    for (i = 0; i < 3; i++) 
    {
        fibindex = SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_DEF_RULE+VLAN_DEF_UNTAG_KEY_0+i;
        ret += aal_cl_fib_set(port,DOMAIN_ID_VLAN,fibindex,&fib[i]);
    }
    ret += aal_cl_rule_set(port,DOMAIN_ID_VLAN,SDL_VLAN_DEF_RULE,&rule);
    if(ret)
    {   
        SDL_MIN_LOG("vlan_untag_ctrl_set set rule error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    } 
    
    if(g_ptag_type[port_id]==SDL_P_TAG_UNTAG)
    {
        // TODO: p tag process
    }
    
    g_untag_status[port_id] = untag_status;
        
    return CS_E_OK;
}

cs_status vlan_untag_ctrl_get(
    CS_IN  cs_port_id_t         port_id, 
    CS_OUT cs_sdl_untag_ctrl_t  *untag_status    
)
{

    if((port_id>CS_UNI_PORT_ID1)||(untag_status ==NULL))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  
    
    *untag_status = g_untag_status[port_id];
    
    return CS_E_OK;
}

cs_status cosmap_vlan_untag_ctrl_update( CS_IN cs_port_id_t  port_id )
{
    cs_status ret = CS_E_OK;
      
    // workaround for TX dot1p to cos, update vlan_untag_ctrl after setting RX-dot1p map to cos 
    
    if(port_id>CS_UNI_PORT_ID1)
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  
    
    if(g_untag_status[port_id] >= SDL_UNTAG_FWD_TAG)
    {
        ret = vlan_untag_ctrl_set(port_id,g_untag_status[port_id]);
    }
    
    return ret;
}

cs_status vlan_def_vid_set(
    CS_IN cs_port_id_t port_id, 
    CS_IN cs_uint16    pvid  
)
{   
    cs_status ret    = CS_E_OK;
    cs_aal_port_id_t      port;

    if((port_id > CS_UNI_PORT_ID1)||(pvid>SDL_VLAN_MAX_ID))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    port = AAL_PORT(port_id); 
    
    if(g_def_vlanid[port_id] == pvid)
    {
        return CS_E_OK;
    }
    else
    {   
        /*
         * In Translation/Aggregation/Trunk mode, if configured VID equals to 
         * Default VID, upstream Untagged frames will be forwarded Untagged.
         * The reason is that there are two matches: Default rule(1) and VLAN 
         * member rule(2-5). The result from VLAN member rule takes precedence.
         * So, the final 'top_vlan_cmd' is 'swap/nop' and Untagged frames will 
         * be forwarded without tags.
         * According to Feng, to fix this problem, we should make sure that PVID 
         * keeps unchanged(ASIC default value: 0).
         */
#if 0    
        msk.s.vlanid = 1;
        cfg.vlanid   = pvid;

        (void)aal_port_dft_cfg_set(port, msk, &cfg);
#endif            
        g_def_vlanid[port_id] = pvid;
    }
    
    //update PVID rule
    ret = __vlan_pvid_rule_update(port_id, pvid);
    if(ret)
    {   
        SDL_MIN_LOG("update PVID rule error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
       
    //update untag_ctrl, beacuse add tag with PVID in tag mode
    if(g_untag_status[port_id]>SDL_UNTAG_FWD)
    {
        ret = vlan_untag_ctrl_set( port_id, g_untag_status[port_id]);
    }
    if(ret)
    {   
        SDL_MIN_LOG("vlan_untag_ctrl_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
}

cs_status vlan_def_vid_get(
    CS_IN  cs_port_id_t  port_id, 
    CS_OUT cs_uint16     *pvid    
)
{
    if((port_id > CS_UNI_PORT_ID1)||(pvid ==NULL))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  
    
    *pvid = g_def_vlanid[port_id];
    
    return CS_E_OK;
}

cs_status vlan_def_vlan_ctrl_set(
    CS_IN cs_port_id_t  port_id, 
    CS_IN cs_boolean    drop_en,
    CS_IN cs_boolean    in_untag_en    
)
{   
    cs_aal_port_id_t     port;
    cs_aal_cl_rule_cfg_t rule;
    cs_aal_cl_fib_data_t fib;
    cs_uint16   pvid = 0;
    cs_status   ret  = CS_E_OK;

    if((port_id>CS_UNI_PORT_ID1)||(drop_en>1)||(in_untag_en>1))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    port = AAL_PORT(port_id); 
    
    memset(&rule, 0, sizeof(cs_aal_cl_rule_cfg_t));
    memset(&fib, 0, sizeof(cs_aal_cl_fib_data_t));
   
    pvid = g_def_vlanid[port_id]; 
    
    ret = aal_cl_rule_get(port,DOMAIN_ID_VLAN,SDL_VLAN_DEF_RULE,&rule);
    if(ret)
    {   
        SDL_MIN_LOG("aal_cl_rule_get error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    rule.valid    = 1;
    rule.key_type = AAL_CL_VLAN_KEY;

    // entry key of vlan default rule:4 for PVID
    memset(&rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY], 0, sizeof(cs_aal_cl_vlan_key_t));
    
    rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY].mask.s.vlanid= 1;
    rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY].vlanid = pvid;
    if(pvid == 0)
    {
        rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY].mask.s.p_tag_flag= 1;
        rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY].p_tag_flag = 1;
    }

    fib.valid = 1;
    if(drop_en)
    {
        fib.permit = 0;
        fib.permit_filter_dis = 0;
        fib.permit_pri = 1;
    }
    else
    {
        fib.permit = 1;
        fib.permit_filter_dis = 0;
        fib.permit_pri = 0;
    }
    fib.mask.s.vlan_op = 1;
    if(in_untag_en)
    {
        fib.inner_vlan_cmd  = AAL_CL_VLAN_CMD_NOP;
        fib.top_vlan_cmd    = AAL_CL_VLAN_CMD_POP;
        fib.eg_chk_vlan_sel = AAL_CL_EG_VLAN_CHECK_NO;       
    }
    else
    {
        fib.inner_vlan_cmd  = AAL_CL_VLAN_CMD_NOP;
        fib.top_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
        fib.eg_chk_vlan_sel = AAL_CL_EG_VLAN_CHECK_NO;       
    }
    fib.mask.s.cos = (g_cos_sel_en ==TRUE)?1:0;
    fib.cos_sel    = AAL_CL_COS_SEL_MAP_RX_DOT1P;     
   
    ret = aal_cl_fib_set(port,DOMAIN_ID_VLAN, SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_DEF_RULE+VLAN_DEF_PVLAN_KEY,&fib);
    ret += aal_cl_rule_set(port,DOMAIN_ID_VLAN,SDL_VLAN_DEF_RULE,&rule);
    if(ret)
    {   
        SDL_MIN_LOG(" cl rule set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    } 

    g_pvid_drop_en[port_id]     = drop_en;
    g_pvid_in_untag_en[port_id] = in_untag_en;
    
    return CS_E_OK;
}

cs_status vlan_def_vlan_ctrl_get(
    CS_IN  cs_port_id_t  port_id, 
    CS_OUT cs_boolean    *drop_en,    
    CS_OUT cs_boolean    *in_untag_en    
)
{
    if((port_id>CS_UNI_PORT_ID1)||(drop_en ==NULL)||(in_untag_en ==NULL))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  
    *drop_en     = g_pvid_drop_en[port_id];
    *in_untag_en = g_pvid_in_untag_en[port_id];
    
    return CS_E_OK;
}

cs_status vlan_def_vlan_push_set(
    CS_IN cs_port_id_t  port_id
)
{   
    cs_aal_port_id_t     port;
    cs_aal_cl_rule_cfg_t rule;
    cs_aal_cl_fib_data_t fib;
    cs_uint16   pvid = 0;
    cs_status   ret  = CS_E_OK;



    if(port_id>CS_UNI_PORT_ID1)
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    port = AAL_PORT(port_id); 
    
    memset(&rule, 0, sizeof(cs_aal_cl_rule_cfg_t));
    memset(&fib, 0, sizeof(cs_aal_cl_fib_data_t));
   
    pvid = g_def_vlanid[port_id]; 
    
    ret = aal_cl_rule_get(port,DOMAIN_ID_VLAN,SDL_VLAN_DEF_RULE,&rule);
    if(ret)
    {   
        SDL_MIN_LOG("aal_cl_rule_get error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    rule.valid    = 1;
    rule.key_type = AAL_CL_VLAN_KEY;

    // entry key of vlan default rule:4 for PVID
    memset(&rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY], 0, sizeof(cs_aal_cl_vlan_key_t));
    
    rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY].mask.s.vlanid= 1;
    rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY].vlanid = pvid;
    if(pvid == 0)
    {
        rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY].mask.s.p_tag_flag= 1;
        rule.cl_key.vlan_key[VLAN_DEF_PVLAN_KEY].p_tag_flag = 1;
    }

    fib.valid = 1;
    fib.permit = 1;
    fib.permit_filter_dis = 0;
    fib.permit_pri = 0;

    fib.mask.s.vlan_op = 1;
   
    fib.inner_vlan_cmd  = AAL_CL_VLAN_CMD_NOP;
    fib.top_vlan_cmd    = AAL_CL_VLAN_CMD_PUSH;
    fib.top_vlanid_sel    = AAL_CL_VLANID_SEL_CL;
    fib.top_vlanid=pvid;
    fib.top_tpid_sel  = g_tpid_sel;
    
    fib.eg_chk_vlan_sel = AAL_CL_EG_VLAN_CHECK_NO;   

    fib.mask.s.dot1p      = 1;
    fib.top_8021p_sel     = AAL_CL_DOT1P_SEL_CL;  


   
    fib.mask.s.cos = (g_cos_sel_en ==TRUE)?1:0;
    fib.cos_sel    = AAL_CL_COS_SEL_MAP_RX_DOT1P;     
   
    ret = aal_cl_fib_set(port,DOMAIN_ID_VLAN, SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_DEF_RULE+VLAN_DEF_PVLAN_KEY,&fib);
    ret += aal_cl_rule_set(port,DOMAIN_ID_VLAN,SDL_VLAN_DEF_RULE,&rule);
    if(ret)
    {   
        SDL_MIN_LOG(" cl rule set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    } 

    g_pvid_drop_en[port_id]     = FALSE;
    g_pvid_in_untag_en[port_id] = FALSE;


    return CS_E_OK;
}


cs_status vlan_unk_vlan_ctrl_set(
    CS_IN cs_port_id_t            port_id, 
    CS_IN cs_sdl_unk_vlan_ctrl_t  unk_vlan_status    
)
{    
    cs_aal_cl_type_no_hit_msk_t type_msk;
    cs_aal_port_id_t     port;
    cs_aal_cl_rule_cfg_t rule;
    cs_aal_cl_fib_data_t fib;
    cs_status   ret = CS_E_OK;

    if((port_id>CS_UNI_PORT_ID1)||(unk_vlan_status>SDL_UNK_VLAN_MAX))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    port = AAL_PORT(port_id); 
    
    memset(&rule, 0, sizeof(cs_aal_cl_rule_cfg_t));
    memset(&fib, 0, sizeof(cs_aal_cl_fib_data_t));
   
    ret = aal_cl_rule_get(port,DOMAIN_ID_VLAN,SDL_VLAN_DEF_RULE,&rule); 
    if(ret)
    {   
        SDL_MIN_LOG("aal_cl_rule_get error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    rule.valid    = 1;
    rule.key_type = AAL_CL_VLAN_KEY;
    
    // entry key of vlan default rule:2 for unknow vlan
    memset(&rule.cl_key.vlan_key[VLAN_DEF_UKN_VLAN_KEY], 0, sizeof(cs_aal_cl_vlan_key_t));
    
    ret = aal_cl_type_hit_get(port, &type_msk);
    if(ret)
    {
        return CS_E_ERROR;
    } 
    
    if(unk_vlan_status ==SDL_UNK_VLAN_DROP)
    {
        type_msk.s.type_vlan_key = 1;
        //del key 3 
    }
    else
    {
        type_msk.s.type_vlan_key = 0;
        
        //add or update key 3
        rule.cl_key.vlan_key[VLAN_DEF_UKN_VLAN_KEY].mask.s.mask_all_msk= 1;

        fib.valid = 1;
        fib.permit = 1;
        fib.permit_filter_dis = 0;
        fib.permit_pri = 0;
      
        fib.mask.s.vlan_op = 1;
        if(unk_vlan_status ==SDL_UNK_VLAN_FWD_UNTAG)
        {
            fib.inner_vlan_cmd  = AAL_CL_VLAN_CMD_NOP;
            fib.top_vlan_cmd    = AAL_CL_VLAN_CMD_POP;
            fib.eg_chk_vlan_sel = AAL_CL_EG_VLAN_CHECK_NO;       
        }
        else if(unk_vlan_status ==SDL_UNK_VLAN_FWD_TAG)
        {

            fib.inner_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
            fib.top_vlan_cmd      = AAL_CL_VLAN_CMD_PUSH;
            fib.top_vlanid_sel    = AAL_CL_VLANID_SEL_CL;
            fib.top_vlanid        = g_def_vlanid[port_id];;
            if(port == AAL_PORT_ID_PON)
            {
                fib.top_tpid_sel  = AAL_CL_TPID_SEL_8100;
            }
            else
            {
                fib.top_tpid_sel  = g_tpid_sel;
            }
            fib.eg_chk_vlan_sel   = AAL_CL_EG_VLAN_CHECK_NO;  
            fib.mask.s.dot1p      = 1;
            fib.top_8021p_sel     = AAL_CL_DOT1P_SEL_CL;  
            fib.top_8021p         = 0; 
        
        }
        else
        {
            fib.inner_vlan_cmd  = AAL_CL_VLAN_CMD_NOP;
            fib.top_vlan_cmd    = AAL_CL_VLAN_CMD_NOP;
            fib.eg_chk_vlan_sel = AAL_CL_EG_VLAN_CHECK_NO;       
        }
        fib.mask.s.cos = (g_cos_sel_en ==TRUE)?1:0;
        fib.cos_sel    = AAL_CL_COS_SEL_MAP_RX_DOT1P;     
    }
    
    ret = aal_cl_type_hit_set(port, &type_msk);

    ret += aal_cl_fib_set(port,DOMAIN_ID_VLAN, SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_DEF_RULE+VLAN_DEF_UKN_VLAN_KEY,&fib);
    ret += aal_cl_rule_set(port,DOMAIN_ID_VLAN,SDL_VLAN_DEF_RULE,&rule);
    if(ret)
    {   
        SDL_MIN_LOG("cl rule set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    } 

    g_unk_vlan_status[port_id] = unk_vlan_status;
       
    return CS_E_OK;
}

cs_status vlan_unk_vlan_ctrl_get(
    CS_IN  cs_port_id_t            port_id, 
    CS_OUT cs_sdl_unk_vlan_ctrl_t  *unk_vlan_status    
)
{
    if((port_id>CS_UNI_PORT_ID1)||(unk_vlan_status ==NULL))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  
    
    *unk_vlan_status = g_unk_vlan_status[port_id];

    return CS_E_OK;
}

cs_status vlan_member_add(
    CS_IN cs_port_id_t         port_id, 
    CS_IN cs_uint16            vlan_num,
    CS_IN cs_sdl_vlan_oper_t   *vlanid_list
)
{  
    cs_status ret     = CS_E_OK;
    cs_int32  new_cnt = 0;
    cs_int32  i,j;
    cs_uint8  index    = 0;
    cs_uint16 fibindex = 0;
    cs_sdl_vlan_oper_t *pvlan = NULL;
    cs_aal_port_id_t     a_port;
    cs_aal_cl_rule_cfg_t rule;
    cs_aal_cl_fib_data_t fib[8];
    

    if((port_id > CS_UNI_PORT_ID1)||(vlan_num>SDL_VLAN_ENGINE_MAX)||(vlanid_list == NULL))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    pvlan = vlanid_list;
     
    if(0 == g_vlan_member[port_id].count)
    {
        new_cnt = vlan_num; 
    }
    else
    {
        for(i=0; i<vlan_num; i++)
        {    
            if(( pvlan->vlan_id > SDL_VLAN_MAX_ID)||( pvlan->vlan_cmd > SDL_VLAN_CMD_MAX)||
                ( pvlan->op_vid > SDL_VLAN_MAX_ID))
            {   
                SDL_MIN_LOG("para error. vlan_id : %d. [%s %d]\n", pvlan->vlan_id,  __FUNCTION__, __LINE__);
                return CS_E_PARAM;
            }      
            if(!__vlan_find_record(port_id,pvlan,&index))
            {
                new_cnt++;
            }
            pvlan++;
        }
    }

    //no enough resource in vlan table 
    if(new_cnt>(SDL_VLAN_ENGINE_MAX-g_vlan_member[port_id].count))
    {   
        SDL_MIN_LOG("new_cnt: %d, vlan entry_count : %d. [%s %d]\n", new_cnt, g_vlan_member[port_id].count,  __FUNCTION__, __LINE__);
        return CS_E_RESOURCE;
    }

    pvlan = vlanid_list;
    
    for(i=0; i<vlan_num; i++)
    { 
        if(__vlan_find_record(port_id,pvlan,&index))
        {
            g_vlan_member[port_id].vlan_entry[index].vlan_id  = pvlan->vlan_id;
            g_vlan_member[port_id].vlan_entry[index].vlan_cmd = pvlan->vlan_cmd;
            g_vlan_member[port_id].vlan_entry[index].op_vid   = pvlan->op_vid;
        }
        else
        {
            g_vlan_member[port_id].vlan_entry[g_vlan_member[port_id].count].vlan_id  = pvlan->vlan_id;
            g_vlan_member[port_id].vlan_entry[g_vlan_member[port_id].count].vlan_cmd = pvlan->vlan_cmd;
            g_vlan_member[port_id].vlan_entry[g_vlan_member[port_id].count].op_vid   = pvlan->op_vid;
            g_vlan_member[port_id].count++;
        }
        pvlan++;            
    }
         
    a_port = AAL_PORT(port_id); 
    pvlan  = g_vlan_member[port_id].vlan_entry;
    new_cnt= 0;
    for(i = 0; i < (SDL_VLAN_ENGINE_MAX/SDL_VLAN_KEY_NUM_ENTRY); i++) 
    {
        memset(&rule, 0, sizeof(cs_aal_cl_rule_cfg_t));
        memset(&fib[0], 0, sizeof(fib));
        
        // 8 keys per entry
        for(j = 0; j < SDL_VLAN_KEY_NUM_ENTRY; j++) 
        {   
            // if count =0, only clear table
            if(new_cnt<g_vlan_member[port_id].count)
            {
                rule.key_type = AAL_CL_VLAN_KEY;
                rule.valid    = 1;
                rule.cl_key.vlan_key[j].mask.s.vlanid= 1;
                rule.cl_key.vlan_key[j].vlanid = pvlan->vlan_id;
                if(pvlan->vlan_id == 0)
                {
                    rule.cl_key.vlan_key[j].mask.s.p_tag_flag= 1;
                    rule.cl_key.vlan_key[j].p_tag_flag = 1;
                }
                fib[j].valid  = 1;
                fib[j].permit = 1;
                fib[j].permit_filter_dis = 0;
                fib[j].permit_pri     = 0;
                fib[j].mask.s.vlan_op = 1;
                fib[j].inner_vlan_cmd = AAL_CL_VLAN_CMD_NOP;
                if(pvlan->vlan_cmd == SDL_VLAN_CMD_L2)
                {
                    fib[j].top_vlan_cmd   = SDL_VLAN_CMD_SWAP; // same define
                    fib[j].top_vlanid_sel = AAL_CL_VLANID_SEL_L2;
                }
                else
                {
                    fib[j].top_vlan_cmd   = pvlan->vlan_cmd; // same define
                    fib[j].top_vlanid_sel = AAL_CL_VLANID_SEL_CL;
                }
                fib[j].top_vlanid     = pvlan->op_vid;
                fib[j].top_tpid_sel   = AAL_CL_TPID_SEL_8100;
                fib[j].eg_chk_vlan_sel = AAL_CL_EG_VLAN_CHECK_NO;
            }
            else
            {
                break;// to entry cyc
            }
            
            pvlan++;
            new_cnt++;
         
        }
        
        for (j = 0; j < SDL_VLAN_KEY_NUM_ENTRY; j++) 
        {
            fibindex = SDL_VLAN_KEY_NUM_ENTRY*(i+SDL_VLAN_TBL_START)+j;
            ret += aal_cl_fib_set(a_port,DOMAIN_ID_VLAN,fibindex,&fib[j]);
        }
        ret += aal_cl_rule_set(a_port,DOMAIN_ID_VLAN,(i+SDL_VLAN_TBL_START),&rule);
        if(ret)
        {
            SDL_MIN_LOG("update table rule error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        } 
        
    }
   
    return CS_E_OK;
}

cs_status vlan_member_del(
    CS_IN cs_port_id_t         port_id, 
    CS_IN cs_uint16            vlan_num,
    CS_IN cs_uint16            *vlanid_list
)
{  
    cs_status ret = CS_E_OK;
    cs_int32  i,j;
    cs_int32  cfg_cnt  = 0;
    cs_uint8  index    = 0;
    cs_uint16 fibindex = 0;
    cs_sdl_vlan_oper_t   vlan;
    cs_sdl_vlan_oper_t   *pvlan = NULL;
    cs_aal_port_id_t     a_port;
    cs_aal_cl_rule_cfg_t rule;
    cs_aal_cl_fib_data_t fib[8];
    
    if((port_id > CS_UNI_PORT_ID1)||(vlan_num>SDL_VLAN_ENGINE_MAX)||(vlanid_list == NULL))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    memset(&vlan, 0, sizeof(cs_sdl_vlan_oper_t));
    
    for(i=0; i<vlan_num; i++)
    {    
        if( vlanid_list[i] > SDL_VLAN_MAX_ID)
        {   
            SDL_MIN_LOG("para error. vlan_id : %d. [%s %d]\n", vlanid_list[i],  __FUNCTION__, __LINE__);
            return CS_E_PARAM;
        }      
    }
       
    for(i=0; i<vlan_num; i++)
    {    
        vlan.vlan_id = vlanid_list[i];
        if(__vlan_find_record(port_id,&vlan,&index))
        {     
            memcpy(&(g_vlan_member[port_id].vlan_entry[index]), 
                    &(g_vlan_member[port_id].vlan_entry[g_vlan_member[port_id].count-1]), sizeof(cs_sdl_vlan_oper_t));
            memset(&(g_vlan_member[port_id].vlan_entry[g_vlan_member[port_id].count-1]), 0, sizeof(cs_sdl_vlan_oper_t));
            g_vlan_member[port_id].count--;     
        }
    }

    a_port = AAL_PORT(port_id); 
    pvlan  = g_vlan_member[port_id].vlan_entry;

    for(i = 0; i < (SDL_VLAN_ENGINE_MAX/SDL_VLAN_KEY_NUM_ENTRY); i++) 
    {
        memset(&rule, 0, sizeof(cs_aal_cl_rule_cfg_t));
        memset(&fib[0], 0, sizeof(fib));
        
        // 8 keys per entry
        for(j = 0; j < SDL_VLAN_KEY_NUM_ENTRY; j++) 
        {   
            // if count =0, only clear table
            if(cfg_cnt<g_vlan_member[port_id].count)
            {
                rule.key_type = AAL_CL_VLAN_KEY;
                rule.valid    = 1;
                rule.cl_key.vlan_key[j].mask.s.vlanid= 1;
                rule.cl_key.vlan_key[j].vlanid = pvlan->vlan_id;
                if(pvlan->vlan_id == 0)
                {
                    rule.cl_key.vlan_key[j].mask.s.p_tag_flag= 1;
                    rule.cl_key.vlan_key[j].p_tag_flag = 1;
                }
                fib[j].valid  = 1;
                fib[j].permit = 1;
                fib[j].permit_filter_dis = 0;
                fib[j].permit_pri     = 0;
                fib[j].mask.s.vlan_op = 1;
                fib[j].inner_vlan_cmd = AAL_CL_VLAN_CMD_NOP;
                fib[j].top_vlan_cmd   = pvlan->vlan_cmd; // same define
                fib[j].top_vlanid_sel = AAL_CL_VLANID_SEL_CL;
                fib[j].top_vlanid     = pvlan->op_vid;
                fib[j].top_tpid_sel   = AAL_CL_TPID_SEL_8100;
                fib[j].eg_chk_vlan_sel = AAL_CL_EG_VLAN_CHECK_NO;
            }
            else
            {
                break;// to entry cyc
            }
            
            pvlan++;
            cfg_cnt++;
         
        }
        
        for (j = 0; j < SDL_VLAN_KEY_NUM_ENTRY; j++) 
        {
            fibindex = SDL_VLAN_KEY_NUM_ENTRY*(i+SDL_VLAN_TBL_START)+j;
            ret += aal_cl_fib_set(a_port,DOMAIN_ID_VLAN,fibindex,&fib[j]);
        }
        ret += aal_cl_rule_set(a_port,DOMAIN_ID_VLAN,(i+SDL_VLAN_TBL_START),&rule);
        if(ret)
        {
            SDL_MIN_LOG("update table rule error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        } 
        
    }
        
    return CS_E_OK;
}

cs_status vlan_member_get(
    CS_IN  cs_port_id_t         port_id, 
    CS_OUT cs_uint16            *vlan_num,
    CS_OUT cs_sdl_vlan_oper_t   *vlanid_list
)
{  
    cs_int32  i;
     
    if((port_id > CS_UNI_PORT_ID1)||(vlan_num == NULL)||(vlanid_list == NULL))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    *vlan_num = g_vlan_member[port_id].count;

    for(i=0; i< g_vlan_member[port_id].count; i++)
    {
        memcpy(vlanid_list, &(g_vlan_member[port_id].vlan_entry[i]), sizeof(cs_sdl_vlan_oper_t));
        vlanid_list++;
    }
    
    return CS_E_OK;
}

cs_status vlan_def_rule_init (cs_boolean cos_sel_en)
{
    cs_aal_cl_rule_cfg_t    ce_entry;
    cs_aal_cl_fib_data_t    fibdata;
    cs_uint16                  i =0;
    cs_status         ret = CS_E_OK;


    if (cos_sel_en > 1)
        return CS_E_PARAM;
    
    g_cos_sel_en = cos_sel_en; // default vlan rule control cos selecting or not
        
    memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));

    // default key_type is IPV4_LONG_KEY, so init to vlan_key
    ce_entry.key_type = AAL_CL_VLAN_KEY;
       
    ret = aal_cl_rule_set( AAL_PORT_ID_GE, DOMAIN_ID_VLAN, SDL_VLAN_DEF_RULE, &ce_entry);
    if(ret)
    {   
        SDL_MIN_LOG("aal_cl_rule_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    
    for (i = 0; i<8; i++)
    {
        ret = aal_cl_fib_set(AAL_PORT_ID_GE, DOMAIN_ID_VLAN, SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_DEF_RULE+i, &fibdata);
        if (ret)
        {   
            SDL_MIN_LOG("aal_cl_fib_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
    }

    ret = aal_cl_rule_set( AAL_PORT_ID_PON, DOMAIN_ID_VLAN, SDL_VLAN_DEF_RULE, &ce_entry);
    if(ret)
    {    
        SDL_MIN_LOG("aal_cl_rule_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    
    for (i = 0; i<8; i++)
    {
        ret = aal_cl_fib_set(AAL_PORT_ID_PON, DOMAIN_ID_VLAN, SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_DEF_RULE+i, &fibdata);
        if (ret)
        {  
            SDL_MIN_LOG("aal_cl_fib_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
    }
#ifdef HAVE_SDL_CTC      
    cs_aal_vlan_cfg_msk_t msk;
    cs_aal_vlan_cfg_t     cfg;
    
    cfg.top_vlan_l2_en = 1;
    msk.u32 = 0;  
    msk.s.top_vlan_l2_en =1;
    aal_vlan_cfg_set(AAL_PORT_ID_PON, msk, &cfg);
#endif

#ifdef HAVE_SDL_NTT    
    // after VLAN filter , send arp pkt to cpu
    // pri: arp < default VLAN rule < VLAN filter(check)
    memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));

    ce_entry.key_type = AAL_CL_IPV4_LONG_KEY; 
    ce_entry.valid = 1;
    ce_entry.cl_key.ipv4_long_key.mask.s.ev2pt = 1;
    ce_entry.cl_key.ipv4_long_key.ev2pt = 0x0806;

    fibdata.mask.s.dpid = 1;
    
    fibdata.valid = 1;
    fibdata.permit = 1;
    fibdata.permit_pri = 0;
    fibdata.permit_filter_dis = 0;
    fibdata.dpid.dst_op = AAL_SPEC_DST_PORT;
    fibdata.dpid.dpid = AAL_PORT_ID_CPU;
    
    ret = aal_cl_rule_set( AAL_PORT_ID_GE, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_0, &ce_entry);
    if(ret)
    {   
        SDL_MIN_LOG("aal_cl_rule_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    ret = aal_cl_fib_set(AAL_PORT_ID_GE, DOMAIN_ID_VLAN, SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_RULE_0, &fibdata);
    if (ret)
    {   
        SDL_MIN_LOG("aal_cl_fib_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }

    ret = aal_cl_rule_set( AAL_PORT_ID_PON, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_0, &ce_entry);
    if(ret)
    {   
        SDL_MIN_LOG("aal_cl_rule_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    ret = aal_cl_fib_set(AAL_PORT_ID_PON, DOMAIN_ID_VLAN, SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_RULE_0, &fibdata);
    if (ret)
    {   
        SDL_MIN_LOG("aal_cl_fib_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
#endif

    return CS_E_OK;
}

#ifdef HAVE_SDL_NTT

cs_uint16               g_vlan_filter_spec_tpid[2]={0,0};

/*****************************************************************************/
/*                    Local Functions                                        */
/*****************************************************************************/
static cs_status __spec_tpid_8808_filter_set 
(
    CS_IN cs_port_id_t     port
)
{
    cs_status               ret = CS_E_OK;
    cs_uint16               i = 0;
    cs_uint16               fib_index = 0;
    cs_aal_port_id_t        port_idx;
    cs_aal_cl_rule_cfg_t    ce_entry;
    cs_aal_cl_fib_data_t    fibdata;
    cs_aal_pon_drop_t       drop_policy;
    cs_aal_pon_drop_t       drop_msk;

      
    if(port != CS_PON_PORT_ID) 
    {
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    port_idx = AAL_PORT(port); 
    
    memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));  
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));
    drop_msk.u32 = 0;
    drop_policy.u32 =0;

    ce_entry.valid= 1;
    ce_entry.key_type = AAL_CL_VLAN_KEY_E;
    
    for ( i= 0; i< 6; i++)
    {   
        ce_entry.cl_key.vlan_key_e[i].mask.s.tpid_enc = 1;
        ce_entry.cl_key.vlan_key_e[i].mask.s.vlanid = 1;
        ce_entry.cl_key.vlan_key_e[i].tpid_enc = 4;
        ce_entry.cl_key.vlan_key_e[i].vlanid = i+1;
    }
    
    fibdata.mask.u32 = 0;
    fibdata.valid  = 1;
    fibdata.permit = 0;
    fibdata.permit_filter_dis = 0;
    fibdata.permit_pri = 1;

    ret = aal_cl_rule_set( port_idx, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_2, &ce_entry);
    if(ret)
    {    
        SDL_MIN_LOG("aal_cl_rule_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
   
    for (i = 0; i<6; i++)
    {
        fib_index = SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_RULE_2+i;
        ret = aal_cl_fib_set(port_idx, DOMAIN_ID_VLAN, fib_index, &fibdata);
        if (ret)
        {  
            SDL_MIN_LOG("aal_cl_fib_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
    }
    
    //MAC drop cortrol
    drop_msk.s.err_maccontrol_permit =1;
    drop_msk.s.maccontrol_permit =1;
    drop_policy.s.err_maccontrol_permit =1;
    drop_policy.s.maccontrol_permit =1;
       
    ret = aal_pon_drop_policy_set( drop_msk, &drop_policy);   
    
    return ret;
}

static cs_status __spec_tpid_8808_filter_del
(
    CS_IN cs_port_id_t      port
)
{
    cs_status               ret = CS_E_OK;
    cs_uint16               i = 0;
    cs_aal_port_id_t        port_idx;
    cs_aal_cl_rule_cfg_t    ce_entry;
    cs_aal_cl_fib_data_t    fibdata;
    cs_uint16               fib_index = 0;
    cs_aal_pon_drop_t       drop_policy;
    cs_aal_pon_drop_t       drop_msk;

      
    if(port != CS_PON_PORT_ID) 
    {
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    port_idx = AAL_PORT(port); 
      
    memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));  
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));
    
    drop_msk.u32 = 0;
    drop_policy.u32 =0; 

    drop_msk.s.err_maccontrol_permit =1;
    drop_msk.s.maccontrol_permit =1;
    drop_policy.s.err_maccontrol_permit =0;
    drop_policy.s.maccontrol_permit =0;
    
    ret = aal_pon_drop_policy_set( drop_msk, &drop_policy);   
    if(ret)
    {    
        SDL_MIN_LOG("aal_pon_drop_policy_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
     
    ret = aal_cl_rule_set( port_idx, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_2, &ce_entry);
    if(ret)
    {     
        SDL_MIN_LOG("aal_cl_rule_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    
    for (i = 0; i<6; i++)
    {
        fib_index = SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_RULE_2+i;
        ret = aal_cl_fib_set(port_idx, DOMAIN_ID_VLAN, fib_index, &fibdata);
        if (ret)
        {  
            SDL_MIN_LOG("aal_cl_fib_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
    }
    
    return CS_E_OK;
}

#if 0
static cs_status __spec_tpid_8809_filter_set
(
    CS_IN cs_aal_port_id_t       port
)
{
    cs_status              ret = CS_E_OK;
    cs_uint16                  i =0;
    cs_aal_cl_rule_cfg_t   ce_entry;
    cs_aal_cl_fib_data_t   fibdata;
    cs_aal_cl_spcl_ptn_t   speckey_cfg;
    cs_uint16              fib_index =0;

    if(port != AAL_PORT_ID_PON) return CS_E_PARAM;
    
    memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));  
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));
    memset(&speckey_cfg, 0, sizeof(cs_aal_cl_spcl_ptn_t));

    for ( i= 0; i< 3; i++)
    {
        speckey_cfg.spcl_ptn_key  = (i+1)<<8;
        speckey_cfg.spcl_ptn_mask = 0xff;
        speckey_cfg.spcl_ptn_cmp  = AAL_CL_SPCL_PTN_CMP_LO;
        speckey_cfg.spcl_ptn_type = AAL_CL_SPCL_PTN_TYPE_STA;
        ret = aal_cl_spcl_ptn_entry_set( port, i, &speckey_cfg);
        if (ret)
        {  
            return CS_E_ERROR;
        }
    }
    ce_entry.valid= 1;
    ce_entry.key_type = AAL_CL_PROTO_KEY;

    for (i = 0; i<3; i++)
    {
        ce_entry.cl_key.proco_key[i].mask.s.spcl_hit_map =1;        
        SETBIT(ce_entry.cl_key.proco_key[i].spcl_hit_map, i); 
        SETBIT(ce_entry.cl_key.proco_key[i].spcl_hit_map, 3);
    }

    fibdata.mask.u32 = 0;
    fibdata.valid  = 1;
    fibdata.permit = 0;
    fibdata.permit_filter_dis = 1;
    fibdata.permit_pri = 1;

    ret = aal_cl_rule_set( port, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_3, &ce_entry);
    if(ret)
    {    
        return CS_E_ERROR;
    }
   
    for (i = 0; i<2; i++)
    {
        fib_index = SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_RULE_3+i;
        ret = aal_cl_fib_set(port, DOMAIN_ID_VLAN, fib_index, &fibdata);
        if (ret)
        {  
            return CS_E_ERROR;
        }
    }

    fibdata.mask.s.dpid       = 1;
    fibdata.mask.s.cos        = 1;
    fibdata.valid             = 1;
    fibdata.permit            = 1;
    fibdata.permit_filter_dis = 1;
    fibdata.permit_pri        = 1;
    fibdata.dpid.dst_op       = AAL_CL_DST_PORT;
    fibdata.dpid.dpid         = AAL_PORT_ID_CPU;
    fibdata.cl_mirror_en      = 0;
    fibdata.cl_mirror_src_sel = 0;
    fibdata.cos_sel           = AAL_CL_COS_SEL_CL;
    fibdata.cos               = 7; 

    fib_index = SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_RULE_3+2;
    ret = aal_cl_fib_set(port, DOMAIN_ID_VLAN, fib_index, &fibdata);
    if (ret)
    {  
        return CS_E_ERROR;
    }
    
   //petmit  DA 01:80:c2:00:00:02  type == 0x8809
    memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));  
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));
    
    ret = aal_cl_rule_get(port, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_2, &ce_entry);
    if ((ret)||(ce_entry.key_type != AAL_CL_MAC_ETYPE_KEY))
    {  
        return CS_E_ERROR;
    }

    ce_entry.cl_key.mac_etype_key[3].mac[0]= 0x01;
    ce_entry.cl_key.mac_etype_key[3].mac[1]= 0x80;
    ce_entry.cl_key.mac_etype_key[3].mac[2]= 0xc2;
    ce_entry.cl_key.mac_etype_key[3].mac[3]= 0x00;
    ce_entry.cl_key.mac_etype_key[3].mac[4]= 0x00;
    ce_entry.cl_key.mac_etype_key[3].mac[5]= 0x02;
    ce_entry.cl_key.mac_etype_key[3].mac_type = 0;
    ce_entry.cl_key.mac_etype_key[3].tpid_enc = 4;
    ce_entry.cl_key.mac_etype_key[3].age_timer_offset = 63;
    ce_entry.cl_key.mac_etype_key[3].mask.u32 = 0; //clear mask
    ce_entry.cl_key.mac_etype_key[3].mask.s.tpid_enc =1;
    ce_entry.cl_key.mac_etype_key[3].mask.s.mac =1;
 
    fibdata.mask.u32          = 0;
    fibdata.valid             = 1;
    fibdata.permit            = 1;
    fibdata.permit_filter_dis = 0;
    fibdata.permit_pri        = 1;

    ret = aal_cl_rule_set( port, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_2, &ce_entry);
    
    fib_index = SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_RULE_2+3;    
    ret += aal_cl_fib_set(port, DOMAIN_ID_VLAN, fib_index, &fibdata);   
    if(ret)
    {    
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

static cs_status __spec_tpid_8809_filter_del
(
    CS_IN cs_aal_port_id_t       port
)
{
    cs_status             ret = CS_E_OK;
    cs_uint16                  i=0;
    cs_aal_cl_rule_cfg_t  ce_entry;
    cs_aal_cl_fib_data_t  fibdata;
    cs_aal_cl_spcl_ptn_t  speckey_cfg;
    cs_uint16             fib_index =0;

    if(port != AAL_PORT_ID_PON) return CS_E_PARAM;
    
    memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));  
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));
    memset(&speckey_cfg, 0, sizeof(cs_aal_cl_spcl_ptn_t));

    for ( i= 0; i< 3; i++)
    {
        ret = aal_cl_spcl_ptn_entry_set( port, i, &speckey_cfg);
        if (ret)
        {  
            return CS_E_ERROR;
        }
    }

    ret = aal_cl_rule_set( port, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_3, &ce_entry);
    if(ret)
    {    
        return CS_E_ERROR;
    }
    
    for (i = 0; i<3; i++)
    {
        fib_index = SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_RULE_3+i;
        ret = aal_cl_fib_set(port, DOMAIN_ID_VLAN, fib_index, &fibdata);
        if (ret)
        {  
            return CS_E_ERROR;
        }
    }
    
    //clear petmit  DA 01:80:c2:00:00:01  type == 0x8809
    ret = aal_cl_rule_get(port, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_2, &ce_entry);
    if ((ret)||(ce_entry.key_type != AAL_CL_MAC_ETYPE_KEY))
    {  
        return CS_E_ERROR;
    }
    
    memset(&ce_entry.cl_key.mac_etype_key[3], 0, sizeof(cs_aal_cl_mac_etype_key_t));
    
    ret  = aal_cl_rule_set( port, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_2, &ce_entry);
    
    fib_index = SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_RULE_2+3;    
    ret += aal_cl_fib_set(port, DOMAIN_ID_VLAN, fib_index, &fibdata);
    if(ret)
    {    
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

static cs_status __spec_tpid_888e_filter_set
(
    CS_IN cs_aal_port_id_t       port
)
{
    cs_status             ret = CS_E_OK;
    cs_aal_cl_rule_cfg_t  ce_entry;
    cs_aal_cl_fib_data_t  fibdata;
    cs_aal_cl_spcl_ptn_t  speckey_cfg;
    cs_uint16             fib_index =0;

    if(port != AAL_PORT_ID_PON) return CS_E_PARAM;
    
    memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));  
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));
    memset(&speckey_cfg, 0, sizeof(cs_aal_cl_spcl_ptn_t));

    speckey_cfg.spcl_ptn_key  = 0x100;
    speckey_cfg.spcl_ptn_mask = 0xff;
    speckey_cfg.spcl_ptn_cmp  = AAL_CL_SPCL_PTN_CMP_LO;
    speckey_cfg.spcl_ptn_type = AAL_CL_SPCL_PTN_TYPE_STA;
    
    ret = aal_cl_spcl_ptn_entry_set( port, 0, &speckey_cfg);
    if (ret)
    {  
        return CS_E_ERROR;
    }

    ce_entry.valid    = 1;
    ce_entry.key_type = AAL_CL_PROTO_KEY;

    ce_entry.cl_key.proco_key[0].mask.s.tpid_enc     = 1;
    ce_entry.cl_key.proco_key[0].mask.s.spcl_hit_map = 1;
    ce_entry.cl_key.proco_key[0].tpid_enc            = 4;
    ce_entry.cl_key.proco_key[0].spcl_hit_map        = 0x1;      

    fibdata.mask.s.dpid       = 1;
    fibdata.mask.s.cos        = 1;
    fibdata.valid             = 1;
    fibdata.permit            = 1;
    fibdata.permit_filter_dis = 1;
    fibdata.permit_pri        = 1;
    fibdata.dpid.dst_op       = AAL_CL_DST_PORT;
    fibdata.dpid.dpid         = AAL_PORT_ID_CPU;
    fibdata.cl_mirror_en      = 0;
    fibdata.cl_mirror_src_sel = 0;
    fibdata.cos_sel           = AAL_CL_COS_SEL_CL;
    fibdata.cos               = 7;

    ret = aal_cl_rule_set( port, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_3, &ce_entry);
    if(ret)
    {    
        return CS_E_ERROR;
    }
    
    fib_index = SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_RULE_3+0;
    ret = aal_cl_fib_set(port, DOMAIN_ID_VLAN, fib_index, &fibdata);
    if (ret)
    {  
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
}

static cs_status __spec_tpid_888e_filter_del
(
    CS_IN cs_aal_port_id_t       port
)
{
    cs_status              ret = CS_E_OK;
    cs_aal_cl_rule_cfg_t   ce_entry;
    cs_aal_cl_fib_data_t   fibdata;
    cs_aal_cl_spcl_ptn_t   speckey_cfg;
    cs_uint16              fib_index =0;

    if(port != AAL_PORT_ID_PON) return CS_E_PARAM;
    
    memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));  
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));
    memset(&speckey_cfg, 0, sizeof(cs_aal_cl_spcl_ptn_t));
 
    ret = aal_cl_spcl_ptn_entry_set( port, 0, &speckey_cfg);
    if (ret)
    {  
        return CS_E_ERROR;
    }

    ret = aal_cl_rule_set( port, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_3, &ce_entry);
    if(ret)
    {    
        return CS_E_ERROR;
    }
    
    fib_index = SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_RULE_3+0;
    ret = aal_cl_fib_set(port, DOMAIN_ID_VLAN, fib_index, &fibdata);
    if (ret)
    {  
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

#endif

static cs_status __spec_pkt_behavior_ge_set (void)
{
    cs_aal_spec_pkt_ctrl_msk_t         sp_msk;
    cs_aal_spec_pkt_ctrl_t            sp_ctrl;
    cs_status                    ret = CS_E_OK;


    /* purpose special pkt behavior(Base L2_init) :                                                                       */
    /* Note: when DPID == 7({2, 0}), Learning,lmt_fwd,cos,bypass_plc,flow,mirror, permit will be ignored.   */
    /*       they will be from CL_RSLT or others(L2,PORT).                                                  */
    /*        PKTtype                PRI     DPID    Learning    LimitForward  COS   rlimBypass             */
    /* AAL_PKT_NORMAL                 1       7         1            0          0           0               */
    /* AAL_PKT_BPDU                  27       7         0            0          0           0               */
    /* AAL_PKT_8021X                 30       7         0            0          0           1               */
    /* AAL_PKT_IEEE_RSVD1            22       7         0            0          0           0               */
    /* AAL_PKT_MYMAC                 29       4         0            0          3           0               */
    /* AAL_PKT_MC_A                  21       7         1            0          0           0               */
    /* AAL_PKT_MC_B                  20       7         1            0          0           0               */
    /* AAL_PKT_MC_C                  19       7         1            0          0           0               */
    /* AAL_PKT_IGMP                  26       7         1            0          0           0               */
    /* AAL_PKT_ARP                   24       7         1            0          0           0               */
    /* AAL_PKT_OAM                   31       3         0            0          3           1               */
    /* AAL_PKT_MPCP                  28       3         0            0          0           0               */
    /* AAL_PKT_DHCP                  18       7         1            0          0           0               */
    /* AAL_PKT_PPPOE_DIS             25       7         1            0          0           0               */
    /* AAL_PKT_PPPOE_SESSION         24       7         1            0          0           0               */
    /* AAL_PKT_HELLO                 23       7         0            0          3           0               */
    /* AAL_PKT_IPV6NDP               22       7         1            0          0           0               */
    /* AAL_PKT_UDF0                   2       7         0            0          0           0               */
    /* AAL_PKT_UDF1                   3       7         0            0          0           0               */
    /* AAL_PKT_UDF2                   4       7         0            0          0           0               */
    /* AAL_PKT_UDF3                   5       7         0            0          0           0               */

     // MYMAC to CPU
    memset(&sp_ctrl, 0, sizeof(cs_aal_spec_pkt_ctrl_t));
    sp_msk.u32     = 0;
    sp_msk.s.dpid  = 1;
    sp_msk.s.cos   = 1;
    sp_ctrl.dpid.dst_op = AAL_SPEC_DST_PORT;
    sp_ctrl.dpid.dpid   = AAL_PORT_ID_CPU;
    sp_ctrl.cos    = 3;
    
    // upstream to cpu/MII0.MII1 cos = 3, downstream to cpu/MII0.MII1 cos =7.
    // BM will mapping cos if work in NTT mode.
    
    ret = aal_special_pkt_behavior_set(AAL_PORT_ID_GE, AAL_PKT_MYMAC, sp_msk, &sp_ctrl);
    
    // drop OAM on UNI port
    memset(&sp_ctrl, 0, sizeof(cs_aal_spec_pkt_ctrl_t));
    sp_msk.u32 = 0;
    sp_msk.s.permit = 1;
    sp_msk.s.dpid   = 1;
    
    sp_ctrl.permit      = 0;
    sp_ctrl.dpid.dst_op = AAL_SPEC_DST_DROP;
       
    ret += aal_special_pkt_behavior_set(AAL_PORT_ID_GE, AAL_PKT_OAM, sp_msk, &sp_ctrl);
    if (ret)
    {  
        SDL_MIN_LOG("aal_special_pkt_behavior_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    // 8021x PACKET to FE engine on UNI port
    memset(&sp_ctrl, 0, sizeof(cs_aal_spec_pkt_ctrl_t));
    sp_msk.u32     = 0;
    sp_msk.s.dpid  = 1;
    sp_msk.s.cos   = 1;
    sp_ctrl.dpid.dst_op = AAL_SPEC_DST_FE;
    sp_ctrl.cos    = 0;
    
    ret = aal_special_pkt_behavior_set(AAL_PORT_ID_GE, AAL_PKT_8021X, sp_msk, &sp_ctrl);
    ret += aal_special_pkt_behavior_set(AAL_PORT_ID_GE, AAL_PKT_ARP, sp_msk, &sp_ctrl);
    if (ret)
    {  
        SDL_MIN_LOG("aal_special_pkt_behavior_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

static cs_status __spec_pkt_behavior_pon_set (void)
{
    cs_aal_spec_pkt_ctrl_msk_t         sp_msk;
    cs_aal_spec_pkt_ctrl_t            sp_ctrl;
    cs_status                    ret = CS_E_OK;


    /* purpose special pkt behavior (Base L2_init):                                                                       */
    /* Note: when DPID == 7({2, 0}), Learning,lmt_fwd,cos,bypass_plc,flow,mirror, permit will be ignored.   */
    /*       they will be from CL_RSLT or others(L2,PORT).                                                  */
    /*        PKTtype                PRI     DPID    Learning    LimitForward  COS   rlimBypass             */
    /* AAL_PKT_NORMAL                 1       7         0            0          0           0               */
    /* AAL_PKT_BPDU                  27       7         0            0          0           0               */
    /* AAL_PKT_8021X                 30       7         0            0          0           1               */
    /* AAL_PKT_IEEE_RSVD1            22       7         0            0          0           0               */
    /* AAL_PKT_MYMAC                 29       4         0            0          7           0               */
    /* AAL_PKT_MC_A                  21       7         0            0          0           0               */
    /* AAL_PKT_MC_B                  20       7         0            0          0           0               */
    /* AAL_PKT_MC_C                  19       7         0            0          0           0               */
    /* AAL_PKT_IGMP                  26       7         0            0          0           0               */
    /* AAL_PKT_ARP                   24       7         0            0          0           0               */
    /* AAL_PKT_OAM                   31       7         0            0          0           1               */
    /* AAL_PKT_MPCP                  28       3         0            0          0           0               */
    /* AAL_PKT_DHCP                  18       7         0            0          0           0               */
    /* AAL_PKT_PPPOE_DIS             25       7         0            0          0           0               */
    /* AAL_PKT_PPPOE_SESSION         24       7         0            0          0           0               */
    /* AAL_PKT_HELLO                 23       7         0            0          7           0               */
    /* AAL_PKT_IPV6NDP               22       7         0            0          0           0               */
    /* AAL_PKT_UDF0                   2       7         0            0          0           0               */
    /* AAL_PKT_UDF1                   3       7         0            0          0           0               */
    /* AAL_PKT_UDF2                   4       7         0            0          0           0               */
    /* AAL_PKT_UDF3                   5       7         0            0          0           0               */

    // MYMAC to CPU
    memset(&sp_ctrl, 0, sizeof(cs_aal_spec_pkt_ctrl_t));
    sp_msk.u32     = 0;
    sp_msk.s.dpid  = 1;
    sp_msk.s.cos   = 1;
    sp_ctrl.dpid.dst_op = AAL_SPEC_DST_PORT;
    sp_ctrl.dpid.dpid   = AAL_PORT_ID_CPU;
    sp_ctrl.cos    = 7;
    ret = aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_MYMAC, sp_msk, &sp_ctrl);
    // OAM, 8021X to CE engine on pon port
    memset(&sp_ctrl, 0, sizeof(cs_aal_spec_pkt_ctrl_t));
    sp_msk.u32     = 0;
    sp_msk.s.dpid  = 1;
    sp_msk.s.cos   = 1;
    sp_ctrl.dpid.dst_op = AAL_SPEC_DST_FE;
    sp_ctrl.cos    = 0;
       
    ret += aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_8021X, sp_msk, &sp_ctrl);
    ret += aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_ARP, sp_msk, &sp_ctrl);
    ret += aal_special_pkt_behavior_set(AAL_PORT_ID_PON, AAL_PKT_OAM, sp_msk, &sp_ctrl);
    if (ret)
    {  
        SDL_MIN_LOG("aal_special_pkt_behavior_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

static cs_status __spec_ev2pt_filter_ge_set (void)
{
    cs_aal_cl_rule_cfg_t             ce_entry;
    cs_aal_cl_fib_data_t              fibdata;
    cs_uint16                    fib_index =0;
    cs_status                    ret = CS_E_OK;

    
    // A: DA MYmac, type( which be forwarded to FE, now type only is 0x888e) send to CPU. not bypass_plc.
    // B: Drop DA any,  type == 0x8809 / 0x8808
    // PRI: A > B , so A:key 2.B:key 1, key 0

    memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));  
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t)); 

    ce_entry.valid = 1;
    ce_entry.key_type = AAL_CL_PROTO_KEY;  

    ce_entry.cl_key.proco_key[0].mask.s.ev2pt  = 1;
    ce_entry.cl_key.proco_key[0].ev2pt         = SDL_TPID_8808;
    
    ce_entry.cl_key.proco_key[1].mask.s.ev2pt  = 1;
    ce_entry.cl_key.proco_key[1].ev2pt         = SDL_TPID_8809;
    
    ce_entry.cl_key.proco_key[2].mask.s.sp_mac_flag  = 1;
    ce_entry.cl_key.proco_key[2].myMAC_flag          = 1;   //DA MYmac, Type: NOW only 0x888e 

     
    fibdata.mask.u32 = 0;
    fibdata.valid  = 1;
    fibdata.permit = 0;
    fibdata.permit_filter_dis = 0;
    fibdata.permit_pri = 1; 

    ret = aal_cl_rule_set( AAL_PORT_ID_GE, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_UP_RULE, &ce_entry);
  
    for (fib_index= (SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_UP_RULE+0); fib_index< (SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_UP_RULE+2); fib_index++)
    {
        ret += aal_cl_fib_set(AAL_PORT_ID_GE, DOMAIN_ID_VLAN, fib_index, &fibdata);
    }
  
    if (ret)
    {  
        SDL_MIN_LOG("cl_rule set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));

    fibdata.mask.u32 = 0;
    fibdata.mask.s.dpid       = 1;
    fibdata.mask.s.cos        = 1;
    fibdata.valid             = 1;
    fibdata.permit            = 1;
    fibdata.permit_filter_dis = 1;
    fibdata.permit_pri        = 1;
    fibdata.dpid.dst_op       = AAL_CL_DST_PORT;
    fibdata.dpid.dpid         = AAL_PORT_ID_CPU;
    fibdata.cl_mirror_en      = 0;
    fibdata.cl_mirror_src_sel = 0;
    fibdata.cos_sel           = AAL_CL_COS_SEL_CL;
    fibdata.cos               = 3; 
    
    fib_index= SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_UP_RULE+2 ;
    
    ret = aal_cl_fib_set(AAL_PORT_ID_GE, DOMAIN_ID_VLAN, fib_index, &fibdata);
    if (ret)
    {  
        SDL_MIN_LOG("aal_cl_fib_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

static cs_status __spec_ev2pt_filter_pon_set (void)
{
    cs_aal_cl_rule_cfg_t             ce_entry;
    cs_aal_cl_fib_data_t              fibdata;
    cs_aal_cl_spcl_ptn_control_msk_t      msk;
    cs_aal_cl_spcl_ptn_control_t speckey_ctrl;
    cs_aal_cl_spcl_ptn_t          speckey_cfg;
    cs_uint16                    fib_index =0;
    cs_uint16                            i =0;
    cs_status                    ret = CS_E_OK;


    memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));  
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));
    memset(&speckey_ctrl, 0, sizeof(cs_aal_cl_spcl_ptn_control_t));
    memset(&speckey_cfg, 0, sizeof(cs_aal_cl_spcl_ptn_t));
 
    //====================================================================================================
    //A: DA 01:80:c2:00:00:02           type = 0x8809,Next octet = 0x3      TO CPU on pon port, bypass_plc=1
    //B: DA 01:80:c2:00:00:03 or MYmac  type = 0x888e,Next octet = 0x1/0x2  TO CPU on pon port, bypass_plc=1
    //C: DA any                         type = 0x8809,Next octet = any      Drop   on pon port
    //Pri : C < B=A
    //====================================================================================================

    // use global offset of speckey 
    // speckey entry 0 : 0x1;entry 1: 0x2; entry 2: 0x3; 
    // speckey entry 3 : 0x8809; entry 4: 0x888e;  
    msk.u32 = 0;
    msk.s.mode_sel   = 1;
    msk.s.parse_ctrl = 1;
    msk.s.glb_strt   = 1;
    speckey_ctrl.spcl_hdr_parsing_en   = 1;
    speckey_ctrl.spcl_hdr_mode_sel     = AAL_CL_SPCL_MODE_STA;
    speckey_ctrl.spcl_glb_hdr_strt_idx = 12;  //offset 12 bytes (after MAC address of pkt header)
    
    ret = aal_cl_spcl_ptn_ctrl_set( AAL_PORT_ID_PON, msk, &speckey_ctrl);
    if (ret)
    {  
        SDL_MIN_LOG("aal_cl_spcl_ptn_ctrl_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
       
    for ( i= 0; i< 3; i++)
    {
        speckey_cfg.spcl_ptn_key  = (i+1)<<8;
        speckey_cfg.spcl_ptn_mask = 0xff;
        speckey_cfg.spcl_ptn_cmp  = AAL_CL_SPCL_PTN_CMP_LO;
        speckey_cfg.spcl_ptn_type = AAL_CL_SPCL_PTN_TYPE_STA;
        ret += aal_cl_spcl_ptn_entry_set( AAL_PORT_ID_PON, i, &speckey_cfg);
        if (ret)
        {  
            SDL_MIN_LOG("aal_cl_spcl_ptn_entry_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
    }
    
    speckey_cfg.spcl_ptn_key  = SDL_TPID_8809;
    speckey_cfg.spcl_ptn_mask = 0;
    speckey_cfg.spcl_ptn_cmp  = AAL_CL_SPCL_PTN_CMP_HI;
    speckey_cfg.spcl_ptn_type = AAL_CL_SPCL_PTN_TYPE_STA;
    
    ret = aal_cl_spcl_ptn_entry_set( AAL_PORT_ID_PON, 3, &speckey_cfg);    
    
    speckey_cfg.spcl_ptn_key  = SDL_TPID_888e;
    speckey_cfg.spcl_ptn_mask = 0;
    speckey_cfg.spcl_ptn_cmp  = AAL_CL_SPCL_PTN_CMP_HI;
    speckey_cfg.spcl_ptn_type = AAL_CL_SPCL_PTN_TYPE_STA;
    
    ret += aal_cl_spcl_ptn_entry_set( AAL_PORT_ID_PON, 4, &speckey_cfg);
    if (ret)
    {  
        SDL_MIN_LOG("aal_cl_spcl_ptn_entry_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    
    ce_entry.valid    = 1;
    ce_entry.key_type = AAL_CL_PROTO_KEY;

    ce_entry.cl_key.proco_key[0].mask.s.spcl_hit_map = 1;
    ce_entry.cl_key.proco_key[0].mask.s.sp_mac_flag  = 1;
    ce_entry.cl_key.proco_key[0].mask.s.mac_da_low   = 1;

    ce_entry.cl_key.proco_key[0].spcl_hit_map        = 0x11;  //type:0x888e,Next octet:0x1
    ce_entry.cl_key.proco_key[0].rsvd_mac_da_flag    = 1;  
    ce_entry.cl_key.proco_key[0].mac_da_low          = 0x03;  //DA 01:80:c2:00:00:03   
    

    ce_entry.cl_key.proco_key[1].mask.s.spcl_hit_map = 1;
    ce_entry.cl_key.proco_key[1].mask.s.sp_mac_flag  = 1;
    ce_entry.cl_key.proco_key[1].mask.s.mac_da_low   = 1;

    ce_entry.cl_key.proco_key[1].spcl_hit_map        = 0x12;  //type:0x888e,Next octet:0x2
    ce_entry.cl_key.proco_key[1].rsvd_mac_da_flag    = 1;  
    ce_entry.cl_key.proco_key[1].mac_da_low          = 0x03;  //DA 01:80:c2:00:00:03   

    ce_entry.cl_key.proco_key[2].mask.s.spcl_hit_map = 1;
    ce_entry.cl_key.proco_key[2].mask.s.sp_mac_flag  = 1;
    ce_entry.cl_key.proco_key[2].mask.s.mac_da_low   = 1;

    ce_entry.cl_key.proco_key[2].spcl_hit_map        = 0xc;  //type:0x8809,Next octet:0x3
    ce_entry.cl_key.proco_key[2].rsvd_mac_da_flag    = 1;  
    ce_entry.cl_key.proco_key[2].mac_da_low          = 0x02; //DA 01:80:c2:00:00:02  
    
    ce_entry.cl_key.proco_key[3].mask.s.sp_mac_flag  = 1;
    ce_entry.cl_key.proco_key[3].myMAC_flag          = 1;   //DA MYmac, Type: NOW only 0x888e 
    

    ret = aal_cl_rule_set( AAL_PORT_ID_PON, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_4, &ce_entry);
    if(ret)
    {    
        SDL_MIN_LOG("aal_cl_rule_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
      
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
    fibdata.cl_mirror_en      = 0;
    fibdata.cl_mirror_src_sel = 0;
    fibdata.cos_sel           = AAL_CL_COS_SEL_CL;
    fibdata.cos               = 7; 
    fibdata.cl_rate_limiter_bypass = 1; 
      
    for (i = 0; i<4; i++)
    {
        fib_index = SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_RULE_4+i;
        ret = aal_cl_fib_set(AAL_PORT_ID_PON, DOMAIN_ID_VLAN, fib_index, &fibdata);
        if (ret)
        {  
            SDL_MIN_LOG("aal_cl_fib_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
    }

    //drop DA any,  type == 0x8809  on pon port
 
    memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));  
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));

    ce_entry.valid = 1;
    ce_entry.key_type = AAL_CL_CTRL_PKT_KEY;  

    ce_entry.cl_key.ctrl_pkt_key[0].spl_pkt_en = 0;
    ce_entry.cl_key.ctrl_pkt_key[0].special_packet_vec = SDL_TPID_8809;
    ce_entry.cl_key.ctrl_pkt_key[0].mask.s.ev2pt = 1;
   
    ret  = aal_cl_rule_set( AAL_PORT_ID_PON, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_RULE_1, &ce_entry);
    
    fibdata.mask.u32 = 0;
    fibdata.valid = 1;
    fibdata.permit = 0;
    fibdata.permit_filter_dis = 0;
    fibdata.permit_pri = 1;
    
    fib_index = SDL_VLAN_KEY_NUM_ENTRY*SDL_VLAN_SPEC_RULE_1+0;    
    ret += aal_cl_fib_set(AAL_PORT_ID_PON, DOMAIN_ID_VLAN, fib_index, &fibdata);
     
    if (ret)
    {  
        SDL_MIN_LOG("cl rule set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
}

static cs_status __spec_da_range_drop_ge_set (void)
{
    cs_aal_cl_rule_cfg_t             ce_entry;
    cs_aal_cl_fib_data_t              fibdata;
    cs_uint16                    fib_index =0;
    cs_uint16                       i =0, j=0;
    cs_uint8                      da_mac   =0;
    cs_status                    ret = CS_E_OK;


    //drop DA 01:80:C2:00:00:00 ~~~ 01:80:C2:00:00:0f , type: any
    //Need four entry:8*4; DA_RULE entry : 6,7,8,9
    
    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));
    fibdata.mask.u32 = 0;
    fibdata.valid  = 1;
    fibdata.permit = 0;
    fibdata.permit_filter_dis = 0;
    fibdata.permit_pri = 1; 
    
    da_mac = 0x00;
    for (j = 0; j<4; j++)
    {
        memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));  
        
        ce_entry.valid = 1;
        ce_entry.key_type = AAL_CL_CTRL_PKT_KEY;  

        for (i = 0; i<4; i++)
        {   
            ce_entry.cl_key.ctrl_pkt_key[i].mac_da[0] = 0x01;
            ce_entry.cl_key.ctrl_pkt_key[i].mac_da[1] = 0x80;
            ce_entry.cl_key.ctrl_pkt_key[i].mac_da[2] = 0xc2;
            ce_entry.cl_key.ctrl_pkt_key[i].mac_da[3] = 0x00;
            ce_entry.cl_key.ctrl_pkt_key[i].mac_da[4] = 0x00;
            ce_entry.cl_key.ctrl_pkt_key[i].mac_da[5] = da_mac;
            ce_entry.cl_key.ctrl_pkt_key[i].macmask = 0; 
            ce_entry.cl_key.ctrl_pkt_key[i].spl_pkt_en = 0;
            ce_entry.cl_key.ctrl_pkt_key[i].mask.s.mac = 1;
            
            da_mac++;
            fib_index = (SDL_VLAN_KEY_NUM_ENTRY*(SDL_VLAN_SPEC_UP_DA_RULE_START+j)+i);
            ret += aal_cl_fib_set(AAL_PORT_ID_GE, DOMAIN_ID_VLAN, fib_index, &fibdata);
        }
         
        ret += aal_cl_rule_set( AAL_PORT_ID_GE, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_UP_DA_RULE_START+j, &ce_entry);
    }

    if (ret)
    {  
        SDL_MIN_LOG("cl rule set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }   
    
    return CS_E_OK;
}

static cs_status __spec_da_range_drop_pon_set (void)
{
    cs_aal_cl_rule_cfg_t             ce_entry;
    cs_aal_cl_fib_data_t              fibdata;
    cs_uint16                        i =0,j=0;
    cs_uint16                    fib_index =0;
    cs_uint8                      da_mac   =0;
    cs_status                    ret = CS_E_OK;

  
    //drop DA 01:80:C2:00:00:00 ~~~ 01:80:C2:00:00:0F , type: any  on pon port
    //Need four entry:8*4; DA_RULE entry : 3,4,5,6

    memset(&fibdata, 0, sizeof(cs_aal_cl_fib_data_t));
    fibdata.mask.u32 = 0;
    fibdata.valid  = 1;
    fibdata.permit = 0;
    fibdata.permit_filter_dis = 0;
    fibdata.permit_pri = 1; 
    
    da_mac = 0x00;
    for (j = 0; j<4; j++)
    {
        memset(&ce_entry, 0, sizeof(cs_aal_cl_rule_cfg_t));  
        
        ce_entry.valid = 1;
        ce_entry.key_type = AAL_CL_CTRL_PKT_KEY;  

        for (i = 0; i<4; i++)
        {   
            ce_entry.cl_key.ctrl_pkt_key[i].mac_da[0] = 0x01;
            ce_entry.cl_key.ctrl_pkt_key[i].mac_da[1] = 0x80;
            ce_entry.cl_key.ctrl_pkt_key[i].mac_da[2] = 0xc2;
            ce_entry.cl_key.ctrl_pkt_key[i].mac_da[3] = 0x00;
            ce_entry.cl_key.ctrl_pkt_key[i].mac_da[4] = 0x00;
            ce_entry.cl_key.ctrl_pkt_key[i].mac_da[5] = da_mac;
            ce_entry.cl_key.ctrl_pkt_key[i].macmask = 0; 
            ce_entry.cl_key.ctrl_pkt_key[i].spl_pkt_en = 0;
            ce_entry.cl_key.ctrl_pkt_key[i].mask.s.mac = 1;
            
            da_mac++;
            fib_index = (SDL_VLAN_KEY_NUM_ENTRY*(SDL_VLAN_SPEC_DA_RULE_START+j)+i);
            ret += aal_cl_fib_set(AAL_PORT_ID_PON, DOMAIN_ID_VLAN, fib_index, &fibdata);
        }
         
        ret += aal_cl_rule_set( AAL_PORT_ID_PON, DOMAIN_ID_VLAN, SDL_VLAN_SPEC_DA_RULE_START+j, &ce_entry);
    }

    if (ret)
    {  
        SDL_MIN_LOG("cl rule set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }   

    return CS_E_OK;
}

static cs_status __spec_tpid_filter_ge_init (void)
{
    cs_status                    ret = CS_E_OK;

    
   //////////////upstream(GE)////////////////////////////////////////////////////////////////////////////////////// 
   // pri         Dst address           type      Next octet      Action
   // 1(Highest)   01:80:C2:00:00:02     0x8809    any             drop       (OAM)
   // 2            MYmac                 0x888e    any             TO CPU     (802.1X)   
   // 3            MYmac                 any       any             TO CPU
   // 4            01:80:C2:00:00:01     0x8808    any             drop     
   //[5            Any                   0X0806    0x1/0x2         TO CPU (if HAVE_IP_STACK)  (ARP)]                   
   // 5            Hello_mac             0XFFFF    any             TO CPU
   // 6            01:80:C2:00:00:00 -   any       any             drop
   //              -01:80:C2:00:00:0f
   // 7            Any                   0x8809    any             drop  (exclude OAM)
   // 8            Any                   0x8808    any             drop  (exclude MPCP)
   // 9            Any                   0x888e    any             Forward as normal packet (exclude 802.1X)
   //[9            Any                   0X0806    0x1/0x2         Forward as normal packet (if NO_IP_STACK)  (ARP)]    
   // 9            Others                Others    any             Forward as normal packet  
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ret = __spec_pkt_behavior_ge_set();
    if(ret)
    {
        SDL_MIN_LOG("__spec_pkt_behavior_ge_set error.ret: %d. [%s %d]\n", ret,  __FUNCTION__, __LINE__);
        return ret;
    }

    ret = __spec_ev2pt_filter_ge_set();
    if(ret)
    {   
        SDL_MIN_LOG("__spec_ev2pt_filter_ge_set error.ret: %d. [%s %d]\n", ret,  __FUNCTION__, __LINE__);
        return ret;
    }

    ret = __spec_da_range_drop_ge_set();
    if(ret)
    {  
        SDL_MIN_LOG("__spec_da_range_drop_ge_set error.ret: %d. [%s %d]\n", ret,  __FUNCTION__, __LINE__);
        return ret;
    }

    return CS_E_OK;
}

static cs_status __spec_tpid_filter_pon_init (void)
{

    cs_status                    ret = CS_E_OK;


   //////////////downstream(pon)////////////////////////////////////////////////////////////////////////////////////// 
   // pri         Dst address           type      Next octet      Action
   // 1(Highest)   MYmac                 any       any             TO CPU
   // 2            01:80:C2:00:00:02     0x8809    0x3             TO CPU     (OAM) 
   // 3            01:80:C2:00:00:03     0x888e    0x1/0x2         TO CPU     (802.1X)  
   // 4            Any                   0x8808    0x0001-0006     drop       (MPCP) 
   //[5            Any                   0X0806    0x1/0x2         TO CPU (if HAVE_IP_STACK)  (ARP)]                   
   // 5            Hello_mac             0XFFFF    any             TO CPU
   // 6            01:80:C2:00:00:00 -   any       any             drop
   //              -01:80:C2:00:00:0f
   // 7            Any                   0x8808    any             Forward as normal packet to vlan filter with (exclude MPCP)  
   //                                                              a configured TPID=0x8808, otherwise drop.
   // 8            Any                   0x8809    any             Forward as normal packet to vlan filter with (exclude OAM) 
   //                                                              a configured TPID=0x8809, otherwise drop. 
   // 9            Any                   0x888e    any             Forward as normal packet to vlan filter (exclude 802.1X)
   //[9            Any                   0X0806    0x1/0x2         Forward as normal packet to vlan filter (if NO_IP_STACK)  (ARP)]    
   // 9            Others                Others    any             Forward as normal packet to vlan filter
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ret = __spec_pkt_behavior_pon_set();
    if(ret)
    {  
        SDL_MIN_LOG("__spec_pkt_behavior_pon_set error.ret: %d. [%s %d]\n", ret,  __FUNCTION__, __LINE__);
        return ret;
    }

    ret = __spec_ev2pt_filter_pon_set();
    if(ret)
    {  
        SDL_MIN_LOG("__spec_ev2pt_filter_pon_set error.ret: %d. [%s %d]\n", ret,  __FUNCTION__, __LINE__);
        return ret;
    }

    ret = __spec_da_range_drop_pon_set();
    if(ret)
    {  
        SDL_MIN_LOG("__spec_da_range_drop_pon_set error.ret: %d. [%s %d]\n", ret,  __FUNCTION__, __LINE__);
        return ret;
    }

    return CS_E_OK;
}

// VLAN filtering INIT VLAN with PVID
cs_status vlan_filter_def_vlan_set(
    CS_IN cs_uint16            pvid, 
    CS_IN cs_boolean         drop_en
)
{
    cs_status   ret = CS_E_OK;
    cs_aal_cl_rule_cfg_t rule;
    cs_aal_cl_fib_data_t  fib;
    
    if((pvid>SDL_VLAN_MAX_ID)||(drop_en>1))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  
     
    memset(&rule, 0, sizeof(cs_aal_cl_rule_cfg_t));
    memset(&fib, 0, sizeof(cs_aal_cl_fib_data_t));
       
    rule.key_type = AAL_CL_VLAN_KEY_E;
    rule.valid    = 1;
    rule.cl_key.vlan_key_e[0].mask.s.vlanid= 1;
    rule.cl_key.vlan_key_e[0].vlanid = pvid;
    if(pvid == 0)
    {
        rule.cl_key.vlan_key_e[0].mask.s.p_tag_flag= 1;
        rule.cl_key.vlan_key_e[0].p_tag_flag = 1;
    }
    
    fib.valid                 = 1;
    
    if(drop_en)   
    {
        fib.permit            = 0;
        fib.permit_filter_dis = 0;
        fib.permit_pri        = 1;
    }
    else
    {
        fib.permit            = 1;
        fib.permit_filter_dis = 0;
        fib.permit_pri        = 0;
    }

    ret = aal_cl_fib_set(AAL_PORT_ID_PON,DOMAIN_ID_VLAN,SDL_VLAN_KEY_NUM_ENTRY*SDL_PVLAN_FILTER_RULE,&fib);

    ret += aal_cl_rule_set(AAL_PORT_ID_PON,DOMAIN_ID_VLAN,SDL_PVLAN_FILTER_RULE,&rule);
    if(ret)
    {   
        SDL_MIN_LOG(" cl rule set error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    } 

    return CS_E_OK;
}

cs_status vlan_filter_ctrl_set(
    CS_IN cs_port_id_t     port_id, 
    CS_IN cs_boolean       filter_en
)
{
    cs_aal_cl_type_no_hit_msk_t type_msk;
    cs_aal_port_id_t     port;
    cs_status   ret = CS_E_OK;
    
    if((port_id != CS_PON_PORT_ID)||(filter_en>1))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  

    port = AAL_PORT(port_id); 
     
    ret = aal_cl_type_hit_get(port, &type_msk);
    if(ret)
    {
        return CS_E_ERROR;
    } 
    
    if(filter_en)
    {
        type_msk.s.type_vlan_key_e = 1;
    }
    else
    {
        type_msk.s.type_vlan_key_e = 0;
    }
    
    ret = aal_cl_type_hit_set(port, &type_msk);

    return ret;
}

cs_status vlan_filter_table_update(
    CS_IN cs_port_id_t     port_id, 
    CS_IN cs_uint16        vlan_num,
    CS_IN cs_uint16        *vlanid_list
)
{   
    cs_aal_cl_rule_cfg_t rule;
    cs_aal_cl_fib_data_t fib[8];
    cs_aal_port_id_t     port;
    cs_uint16 fibindex =0;
    cs_uint16 cfg_cnt =0;
    int i,j;
    cs_status ret = CS_E_OK;
    
    if((port_id != CS_PON_PORT_ID)||(vlan_num > SDL_VLAN_TABLE_MAX_ENTRY*SDL_VLAN_KEY_NUM_ENTRY)||(vlanid_list == NULL))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  

    port = AAL_PORT(port_id); 
      
    for(i = 0; i < SDL_VLAN_TABLE_MAX_ENTRY; i++) 
    {
        memset(&rule, 0, sizeof(cs_aal_cl_rule_cfg_t));
        memset(&fib[0], 0, sizeof(fib));

         // if vlan_num =0, only clear table
        if(vlan_num !=0)
        {
            // 8 keys per entry
            for(j = 0; j < SDL_VLAN_KEY_NUM_ENTRY; j++) 
            {   
                if(cfg_cnt<vlan_num)
                {
                    rule.key_type = AAL_CL_VLAN_KEY_E;
                    rule.valid    = 1;
                    rule.cl_key.vlan_key_e[j].mask.s.vlanid= 1;
                    rule.cl_key.vlan_key_e[j].vlanid = *vlanid_list;
                    if(*vlanid_list == 0)
                    {
                        rule.cl_key.vlan_key_e[j].mask.s.p_tag_flag= 1;
                        rule.cl_key.vlan_key_e[j].p_tag_flag = 1;
                    }
                    fib[j].valid             = 1;
                    fib[j].permit            = 1;
                    fib[j].permit_filter_dis = 0;
                    fib[j].permit_pri        = 0;
                }
                else
                {
                    break;// to entry cyc
                }
                
                vlanid_list++;
                cfg_cnt++;         
            }
        }
        
        for (j = 0; j < SDL_VLAN_KEY_NUM_ENTRY; j++) 
        {
            fibindex = SDL_VLAN_KEY_NUM_ENTRY*(i+SDL_VLAN_TABLE_START_ENTRY)+j;
            ret += aal_cl_fib_set(port,DOMAIN_ID_VLAN,fibindex,&fib[j]);
        }
        ret += aal_cl_rule_set(port,DOMAIN_ID_VLAN,(i+SDL_VLAN_TABLE_START_ENTRY),&rule);
        if(ret)
        {    
            SDL_MIN_LOG("update rule error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        } 
    }
    
    return CS_E_OK;
}

cs_status vlan_spec_tpid_filter_clr(
    CS_IN cs_port_id_t  port_id
)
{
    cs_uint16 old_filter_tpid;
    cs_status ret = CS_E_OK;
 
    if(port_id != CS_PON_PORT_ID) 
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    old_filter_tpid = g_vlan_filter_spec_tpid[port_id];
    
    if(old_filter_tpid == SDL_TPID_8808)
    {
        ret =  __spec_tpid_8808_filter_del(port_id);
    }
#if 0    
    else if(old_filter_tpid == SDL_TPID_8809)
    {
      //  ret =  __spec_tpid_8809_filter_del(port_id);
    }
    else if(old_filter_tpid == SDL_TPID_888e)
    {
      //  ret =  __spec_tpid_888e_filter_del(port_id);
    }
#endif
    if (ret)
    {    
        return CS_E_ERROR;
    }
  
    g_vlan_filter_spec_tpid[port_id] = 0;
    
    return CS_E_OK;
}

cs_status vlan_spec_tpid_filter_set(
    CS_IN cs_port_id_t   port_id, 
    CS_IN cs_uint16      spec_tpid
)
{   
    cs_status ret = CS_E_OK;
 
    if(port_id != CS_PON_PORT_ID) 
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(spec_tpid == g_vlan_filter_spec_tpid[port_id])
        return CS_E_OK;

    ret =  vlan_spec_tpid_filter_clr(port_id);  
    if (ret)
    {    
        SDL_MIN_LOG("vlan_spec_tpid_filter_clr error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }

    if(spec_tpid == SDL_TPID_8808)
    {
        ret =  __spec_tpid_8808_filter_set(port_id);
    }
#if 0    
    else if(spec_tpid == SDL_TPID_8809)
    {
       // ret =  __spec_tpid_8809_filter_set(port_id);
    }
    else if(spec_tpid == SDL_TPID_888e)
    {
       // ret =  __spec_tpid_888e_filter_set(port_id);
    }
#endif 
    if (ret)
    {  
        return CS_E_ERROR;
    }
          
    g_vlan_filter_spec_tpid[port_id] = spec_tpid;
    
    return CS_E_OK;
}

cs_status vlan_spec_tpid_filter_init (void)
{
 
    cs_status         ret = CS_E_OK;

    ret = __spec_tpid_filter_ge_init();
    if(ret)
    {    
        SDL_MIN_LOG("__spec_tpid_filter_ge_init error.[%s %d]\n",  __FUNCTION__, __LINE__);
        return ret;
    }
      
    ret = __spec_tpid_filter_pon_init();
    if(ret)
    {    
        SDL_MIN_LOG("__spec_tpid_filter_pon_init error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return ret;
    }
       
    return CS_E_OK;
}

#endif


#ifdef HAVE_SDL_KT

cs_uint8                g_vlan_tbl_cnt[2]  = {0,0};
cs_sdl_vlan_mem_t       g_vlan_table[2][SDL_VLAN_ENGINE_MAX]; // GE,PON 
cs_uint8                g_vlan_untag_cnt[2]  = {0,0};          // vlan, egress untag mode
cs_uint16               g_vlan_untag_table[2][SDL_VLAN_ENGINE_MAX]; // GE,PON 

static cs_boolean __vlan_tbl_record_find
(   
      CS_IN  cs_port_id_t                  port,
      CS_IN  cs_uint16                   vlanid,
      CS_OUT cs_uint16                   *index
)
{
    cs_uint16 i;

    if(port>CS_UNI_PORT_ID1)
         return FALSE;
    
    for(i=0; i<g_vlan_tbl_cnt[port]; i++)
    {    
        if(i== SDL_VLAN_ENGINE_MAX) 
            break;
            
        if(vlanid == g_vlan_table[port][i].vlan_id)
        {
            *index = i;
             return TRUE;
        }
    }
    return FALSE;
}

static void __vlan_untag_tbl_record_del
(   
    CS_IN  cs_port_id_t              port,
    CS_IN  cs_uint16                vlanid
)
{
    cs_uint16 i;

    if(port>CS_UNI_PORT_ID1)
        return;
        
    for(i=0; i<g_vlan_untag_cnt[port]; i++)
    {    
        if(i== SDL_VLAN_ENGINE_MAX) 
            break;
            
        if(vlanid == g_vlan_untag_table[port][i])
        {
            g_vlan_untag_cnt[port]--;
            g_vlan_untag_table[port][i] = g_vlan_untag_table[port][g_vlan_untag_cnt[port]];     
            g_vlan_untag_table[port][g_vlan_untag_cnt[port]]= 0;   
        }
    }

    return;
}

static cs_status __vlan_untag_table_update(
    CS_IN cs_port_id_t         port_id, 
    CS_IN cs_uint16           vlan_num,
    CS_IN cs_uint16        *vlanid_list
)
{  
    cs_status ret = CS_E_OK;
    cs_aal_port_id_t     port;
    cs_aal_cl_rule_cfg_t rule;
    cs_aal_cl_fib_data_t fib[8];
    cs_sdl_vlan_oper_t * pcfg =NULL;
    cs_uint16 fibindex =0;
    cs_uint16 cfg_cnt  =0;
    int i,j;
    cs_sdl_vlan_oper_t cfg[SDL_VLAN_ENGINE_MAX];
    
    if((port_id > CS_UNI_PORT_ID1)||(vlan_num > SDL_VLAN_ENGINE_MAX)||(vlanid_list == NULL))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  

    memset(&cfg[0], 0, sizeof(cfg));

    for(i=0; i<vlan_num; i++)
    {
        cfg[i].vlan_id = *vlanid_list;
        cfg[i].vlan_cmd = SDL_VLAN_CMD_POP;
        
        vlanid_list++;
    }

    port = AAL_PORT(port_id); 
    
    pcfg = cfg;

    for(i = 0; i < SDL_VLAN_EGRESS_TABLE_MAX_ENTRY; i++) 
    {
        memset(&rule, 0, sizeof(cs_aal_cl_rule_cfg_t));
        memset(&fib[0], 0, sizeof(fib));
        
        // 8 keys per entry
        for(j = 0; j < SDL_VLAN_KEY_NUM_ENTRY; j++) 
        {   
            // if vlan_num =0, only clear table
            if(cfg_cnt<vlan_num)
            {
                rule.key_type = AAL_CL_VLAN_KEY_E;
                rule.valid    = 1;
                rule.cl_key.vlan_key[j].mask.s.vlanid= 1;
                rule.cl_key.vlan_key[j].vlanid = pcfg->vlan_id;
                if(pcfg->vlan_id == 0)
                {
                    rule.cl_key.vlan_key[j].mask.s.p_tag_flag= 1;
                    rule.cl_key.vlan_key[j].p_tag_flag = 1;
                }
                fib[j].valid  = 1;
                fib[j].permit = 1;
                fib[j].permit_filter_dis = 0;
                fib[j].permit_pri     = 0;
                fib[j].mask.s.vlan_op = 1;
                fib[j].inner_vlan_cmd = AAL_CL_VLAN_CMD_NOP;
                fib[j].top_vlan_cmd   = pcfg->vlan_cmd; // same define
                fib[j].top_vlanid_sel = AAL_CL_VLANID_SEL_CL;
                fib[j].top_vlanid     = pcfg->op_vid;
                fib[j].top_tpid_sel   = AAL_CL_TPID_SEL_8100;
                fib[j].eg_chk_vlan_sel = AAL_CL_EG_VLAN_CHECK_NO;
            }
            else
            {
                break;// to entry cyc
            }
            
            pcfg++;
            cfg_cnt++;
         
        }
        
        for (j = 0; j < SDL_VLAN_KEY_NUM_ENTRY; j++) 
        {
            fibindex = SDL_VLAN_KEY_NUM_ENTRY*(i+SDL_VLAN_EGRESS_TABLE_START_ENTRY)+j;
            ret += aal_cl_fib_set(port,DOMAIN_ID_VLAN,fibindex,&fib[j]);
        }
        ret += aal_cl_rule_set(port,DOMAIN_ID_VLAN,(i+SDL_VLAN_EGRESS_TABLE_START_ENTRY),&rule);
        if(ret)
        {
            SDL_MIN_LOG("update rule error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        } 
        
    }
    
    return CS_E_OK;
}

cs_status vlan_add_vlan_member(
       CS_IN cs_port_id_t                port_id, 
       CS_IN cs_uint16                    vlanid, 
       CS_IN cs_sdl_vlan_egressmode_t egress_mode)
{
    cs_status            ret       = CS_E_OK;
    cs_uint16            tbl_index = 0; 
    cs_aal_vlan_entry_t  vlan_cfg;
 
 
    if((port_id > CS_UNI_PORT_ID1)||(vlanid>SDL_VLAN_MAX_ID)||(egress_mode>SDL_VLAN_EMODE_UNTAG))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    // found, modify tagmode action
    if(__vlan_tbl_record_find(port_id,vlanid,&tbl_index))
    {
       if(egress_mode == g_vlan_table[port_id][tbl_index].eg_mode)
           return CS_E_OK;

        g_vlan_table[port_id][tbl_index].eg_mode = egress_mode;
         
        if(egress_mode == SDL_VLAN_EMODE_UNTAG)
        {
            g_vlan_untag_table[port_id][g_vlan_untag_cnt[port_id]] = vlanid;
            g_vlan_untag_cnt[port_id]++;          
        }
        else if(egress_mode == SDL_VLAN_EMODE_TAG)
        {
            __vlan_untag_tbl_record_del(port_id, vlanid);
        }
        
        ret = __vlan_untag_table_update(POP_PORT(port_id), g_vlan_untag_cnt[port_id],&g_vlan_untag_table[port_id][0]);
        
        return ret;  
    }

    // no found,it is new
    
    if(g_vlan_tbl_cnt[port_id] == SDL_VLAN_ENGINE_MAX)
    {   
        SDL_MIN_LOG(" vlan table full! [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }

    // first add vlan member, configure default ingress vlan entry to drop  
    if(g_vlan_tbl_cnt[port_id]==0)
    {
        vlan_cfg.vid_hi         = 4095; // defatule entry, range is invalid
        vlan_cfg.vid_lo         = 0;
        vlan_cfg.permit_ge      = 0;
        vlan_cfg.permit_pon     = 0;
        vlan_cfg.permit_mii0    = 0;
        vlan_cfg.permit_mii1    = 0;
        vlan_cfg.ingress_filter = 0;
        vlan_cfg.egress_filter  = 0;
        vlan_cfg.mapping_flag   = 0;
       
        ret = aal_vlan_entry_set(AAL_PORT(port_id), AAL_VLAN_IDX_DEF_INGRESS, &vlan_cfg);
        if(ret)
        {   
            SDL_MIN_LOG(" vlan engine ingress default error. [%s %d]\n", __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        } 
    }
    
    vlan_cfg.vid_hi         = vlanid;
    vlan_cfg.vid_lo         = vlanid;
    vlan_cfg.permit_ge      = 1;
    vlan_cfg.permit_pon     = 1;
    vlan_cfg.permit_mii0    = 0;
    vlan_cfg.permit_mii1    = 0;
    vlan_cfg.ingress_filter = 1;
    vlan_cfg.egress_filter  = 0;
    vlan_cfg.mapping_flag   = 0;
       
    ret = aal_vlan_entry_set(AAL_PORT(port_id), g_vlan_tbl_cnt[port_id], &vlan_cfg);
    if(ret)
    {   
        SDL_MIN_LOG(" vlan engine set error. [%s %d]\n", __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    } 

    g_vlan_table[port_id][g_vlan_tbl_cnt[port_id]].vlan_id = vlanid;
    g_vlan_table[port_id][g_vlan_tbl_cnt[port_id]].eg_mode = egress_mode;        
    g_vlan_tbl_cnt[port_id]++;
        
    if(egress_mode == SDL_VLAN_EMODE_UNTAG)
    {
        g_vlan_untag_table[port_id][g_vlan_untag_cnt[port_id]] = vlanid;
        g_vlan_untag_cnt[port_id]++;      

        ret = __vlan_untag_table_update(POP_PORT(port_id), g_vlan_untag_cnt[port_id],&g_vlan_untag_table[port_id][0]);
        if(ret)
        {   
            SDL_MIN_LOG("vlan untag table update error. [%s %d]\n", __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        } 
    }
      
    return ret;
}

cs_status vlan_del_vlan_member(
       CS_IN cs_port_id_t                port_id, 
       CS_IN cs_uint16                    vlanid)
{
    cs_status            ret = CS_E_OK;   
    cs_aal_vlan_entry_t  vlan_cfg;
    cs_uint16            tbl_index = 0; 
     
    if((port_id > CS_UNI_PORT_ID1)||(vlanid>SDL_VLAN_MAX_ID))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(!__vlan_tbl_record_find(port_id,vlanid,&tbl_index))
    {   
        SDL_MIN_LOG("no this vlan ID. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
         
    if(g_vlan_table[port_id][tbl_index].eg_mode == SDL_VLAN_EMODE_UNTAG)
    {
        __vlan_untag_tbl_record_del(port_id, vlanid);
        ret = __vlan_untag_table_update(POP_PORT(port_id), g_vlan_untag_cnt[port_id],&g_vlan_untag_table[port_id][0]);
        if(ret)
        {   
            SDL_MIN_LOG("vlan untag table update error. [%s %d]\n", __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        } 
    }

    g_vlan_tbl_cnt[port_id]--;

      // del last vlan member, condfiure default ingress vlan entry to forward  
    if(g_vlan_tbl_cnt[port_id]==0)
    {
        vlan_cfg.vid_hi         = 4095;
        vlan_cfg.vid_lo         = 0;
        vlan_cfg.permit_ge      = 1;
        vlan_cfg.permit_pon     = 1;
        vlan_cfg.permit_mii0    = 1;
        vlan_cfg.permit_mii1    = 1;
        vlan_cfg.ingress_filter = 0;
        vlan_cfg.egress_filter  = 0;
        vlan_cfg.mapping_flag   = 0;
       
        ret = aal_vlan_entry_set(AAL_PORT(port_id), AAL_VLAN_IDX_DEF_INGRESS, &vlan_cfg);
        if(ret)
        {   
            SDL_MIN_LOG(" vlan engine ingress default error. [%s %d]\n", __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        } 
        
    }
    else
    {
        vlan_cfg.vid_hi         = g_vlan_table[port_id][g_vlan_tbl_cnt[port_id]].vlan_id;
        vlan_cfg.vid_lo         = g_vlan_table[port_id][g_vlan_tbl_cnt[port_id]].vlan_id;
        vlan_cfg.permit_ge      = 1;
        vlan_cfg.permit_pon     = 1;
        vlan_cfg.permit_mii0    = 0;
        vlan_cfg.permit_mii1    = 0;
        vlan_cfg.ingress_filter = 1;
        vlan_cfg.egress_filter  = 0;
        vlan_cfg.mapping_flag   = 0;
           
        ret = aal_vlan_entry_set(AAL_PORT(port_id), tbl_index, &vlan_cfg);
        if(ret)
        {   
            SDL_MIN_LOG(" vlan engine set error. [%s %d]\n", __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
    }
    if(ret)
    {   
        SDL_MIN_LOG("config untag status error. [%s %d]\n", __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    } 
       
    vlan_cfg.vid_hi         = 4095;
    vlan_cfg.vid_lo         = 0;
    vlan_cfg.permit_ge      = 1;
    vlan_cfg.permit_pon     = 1;
    vlan_cfg.permit_mii0    = 1;
    vlan_cfg.permit_mii1    = 1;
    vlan_cfg.ingress_filter = 0;
    vlan_cfg.egress_filter  = 0;
    vlan_cfg.mapping_flag   = 0;

    ret = aal_vlan_entry_set(AAL_PORT(port_id), g_vlan_tbl_cnt[port_id], &vlan_cfg);
    if(ret)
    {   
        SDL_MIN_LOG(" vlan engine ingress default error. [%s %d]\n", __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    } 
        
    g_vlan_table[port_id][tbl_index].vlan_id = g_vlan_table[port_id][g_vlan_tbl_cnt[port_id]].vlan_id; 
    g_vlan_table[port_id][tbl_index].eg_mode = g_vlan_table[port_id][g_vlan_tbl_cnt[port_id]].eg_mode;
    g_vlan_table[port_id][g_vlan_tbl_cnt[port_id]].vlan_id = 0;
    g_vlan_table[port_id][g_vlan_tbl_cnt[port_id]].eg_mode = 0;   

    return CS_E_OK;
}

cs_status vlan_clr_vlan_member( CS_IN cs_port_id_t port_id )
{
    cs_uint16               i=0; 
    cs_aal_vlan_entry_t  vlan_cfg;
    cs_status            ret = CS_E_OK;   
     
    if(port_id > CS_UNI_PORT_ID1)
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    if(g_vlan_untag_cnt[port_id])
    {    
        memset(&g_vlan_untag_table[port_id][0], 0, SDL_VLAN_ENGINE_MAX*sizeof(cs_uint16));
        g_vlan_untag_cnt[port_id] = 0;
        ret = __vlan_untag_table_update(POP_PORT(port_id), 0,&g_vlan_untag_table[port_id][0]);
        if(ret)
        {   
            SDL_MIN_LOG("vlan untag table update error. [%s %d]\n", __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }      
    }

    for(i=0; i<g_vlan_tbl_cnt[port_id]; i++)
    {   
        vlan_cfg.vid_hi         = 4095;
        vlan_cfg.vid_lo         = 0;
        vlan_cfg.permit_ge      = 1;
        vlan_cfg.permit_pon     = 1;
        vlan_cfg.permit_mii0    = 1;
        vlan_cfg.permit_mii1    = 1;
        vlan_cfg.ingress_filter = 0;
        vlan_cfg.egress_filter  = 0;
        vlan_cfg.mapping_flag   = 0;

        ret += aal_vlan_entry_set(AAL_PORT(port_id), i, &vlan_cfg);
    
    }
    
    ret += aal_vlan_entry_set(AAL_PORT(port_id), AAL_VLAN_IDX_DEF_INGRESS, &vlan_cfg);    
    if(ret)
    {   
        SDL_MIN_LOG("vlan entry table clr error. [%s %d]\n", __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    } 
    
    memset(&g_vlan_table[port_id][0], 0, SDL_VLAN_ENGINE_MAX*sizeof(cs_sdl_vlan_mem_t));     
    g_vlan_tbl_cnt[port_id] = 0 ;

    return CS_E_OK;
}

cs_status vlan_dump_vlan_member(
    CS_IN  cs_port_id_t               port_id, 
    CS_OUT cs_uint8                   *tag_number,
    CS_OUT cs_uint16                  *tag_vlanid_list,
    CS_OUT cs_uint8                   *untag_number,
    CS_OUT cs_uint16                  *untag_vlanid_list
)
{    
    cs_uint16 index =0;
    cs_uint16 tag_index =0;
 
    if((port_id > CS_UNI_PORT_ID1)||(tag_number ==NULL)||(tag_vlanid_list ==NULL)
        ||(untag_number ==NULL)||(untag_vlanid_list ==NULL))
    {   
        SDL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    *tag_number = g_vlan_tbl_cnt[port_id]-g_vlan_untag_cnt[port_id];

    for (index = 0; index < g_vlan_tbl_cnt[port_id]; index++) 
    {   
        if(g_vlan_table[port_id][index].eg_mode == SDL_VLAN_EMODE_TAG)
        {
            tag_vlanid_list[tag_index] = g_vlan_table[port_id][index].vlan_id;
            tag_index++;
        }
    }

    *untag_number = g_vlan_untag_cnt[port_id];
    memcpy(untag_vlanid_list, &g_vlan_untag_table[port_id][0], g_vlan_untag_cnt[port_id]*sizeof(cs_uint16));
      
    return CS_E_OK;
}

#endif

#if 1
//added by zhuxh
typedef struct addr
{
	unsigned char DA[6];
	unsigned char SA[6];
}eth_head_t;

typedef struct
{
	cs_uint32 TPID		:16;
	cs_uint32 vlan_id_h	:4;
	cs_uint32 Priority	:3;
	cs_uint32 CFI		:1;
	cs_uint32 vlan_id_l	:4;
	cs_uint32 vlan_id_m	:4;
}tag_t;

extern void push_vlan(cs_uint16 vlan, cs_uint8 *in, cs_uint8 *out,cs_uint32* len);

/**********************************************************************************************************************************************
*函数名：pkt_vlan_tag_check
*函数功能描述：判断包是有vlan 标签
*函数参数：pkt - 包起始地址; len - 包长度
*函数返回值：有vlan 标签(tag )返回1; 无vlan 标签(untag )返回0
*作者：朱晓辉
*函数创建日期：
*函数修改日期：尚未修改
*修改人：尚未修改
*修改原因：尚未修改
*版本：1.0
*历史版本：无
**********************************************************************************************************************************************/
extern int pkt_vlan_tag_check(char *eth_pkt, cs_uint32 len)
{
	int found = 0;
	unsigned char * posation = NULL;
	tag_t *tag = NULL;
	//入口规则检查
	if(NULL == eth_pkt)
	{
		cs_printf("in %s, line :%d pointer is NULL!\n");
		return -1;
	}

	posation = (unsigned char *)eth_pkt;
	//跳过以太网头
	posation += sizeof(eth_head_t);
	
	//跳过vlan tag
	tag = (tag_t *)posation;
	if(0x8100 == ntohs(tag->TPID))
	{	
		found = 1;
	}
	else
	{
		found = 0;
	}

	return found;
}

extern int pkt_ip_check(char *eth_pkt)
{
	int found = 0;
	unsigned char * posation = NULL;
	tag_t *tag = NULL;
	//入口规则检查
	if(NULL == eth_pkt)
	{
		cs_printf("in %s, line :%d pointer is NULL!\n");
		return -1;
	}

	posation = (unsigned char *)eth_pkt;
	//跳过以太网头
	posation += sizeof(eth_head_t);
	
	//跳过vlan tag
	tag = (tag_t *)posation;
	while(0x8100 == ntohs(tag->TPID))
	{
		posation = posation + sizeof(tag_t);
		tag = (tag_t *)posation;
	}

	if(0x0800 == ntohs(tag->TPID))
	{	
		found = 1;
	}
	else
	{
		found = 0;
	}

	return found;
}

//tag 报文不做处理，untag 报文打上端口的默认vlan 标签
extern cs_status ip_pkt_to_pkt_with_tag( cs_uint8 *eth_pkt, cs_uint32 *len, cs_port_id_t port_id )
{
	//入口规则检查
	if(NULL == eth_pkt)
	{
		cs_printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return CS_E_ERROR;
	}
	
	if(NULL == len)
	{
		cs_printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return CS_E_ERROR;
	}

	if((port_id < CS_UNI_PORT_ID1) || (port_id > UNI_PORT_MAX))
	{
		return CS_E_ERROR;
	}
	//报文如果不是ip 报文则返回错误
	if(1 != pkt_ip_check(eth_pkt))
	{
		return CS_E_ERROR;
	}

	//报文是否有vlan tag
	if(0 == pkt_vlan_tag_check(eth_pkt, *len))
	{
		//untag 报文的处理
		
		cs_uint16 def_vlan = 0;
		//没有，则获取端口的默认vlan
		def_vlan = port_def_vlan_id_get(port_id);
		if(1 != def_vlan)
		{
			push_vlan(def_vlan, eth_pkt, eth_pkt, len);
		}
		else
		{
			//vlan id : 1 的untag 包不打上vlan 1 的标签,继续以untag 形式处理
		}
	}
	else
	{
		//非untag 报文不做处理
	}
	return CS_E_OK;
}


extern cs_uint16 port_def_vlan_id_get(cs_port_id_t port_id)
{
	cs_callback_context_t context;
    cs_sdl_vlan_tag_t    def_vlan;
    cs_sdl_vlan_mode_t  vlan_mode;
    cs_sdl_vlan_cfg_t    vlan_cfg[__VLAN_RULE_PER_PORT_MAX];
    cs_uint16 nums;
	epon_request_onu_vlan_get(context, 0, 0, port_id, &def_vlan, &vlan_mode, vlan_cfg, &nums);
	return def_vlan.vid;
}

extern cs_status vlan_add_all_port(int vlan)
{
	int port_id;
	cs_callback_context_t context;
    cs_sdl_vlan_tag_t    def_vlan;
    cs_sdl_vlan_mode_t  vlan_mode;
    cs_sdl_vlan_cfg_t    vlan_cfg[__VLAN_RULE_PER_PORT_MAX];
    cs_uint16 nums;
	int vlan_exist = 0;
	int count = 0;
	
	for(count=0; count<UNI_PORT_MAX; count++)
	{
		port_id = count + CS_UNI_PORT_ID1;
		vlan_exist = 0;
		nums = 0;
		memset(vlan_cfg, 0, sizeof(vlan_cfg));
		
		//查看端口是否在vlan 中
		if(CS_E_OK != epon_request_onu_vlan_get(context, 0, 0, port_id, &def_vlan, &vlan_mode, vlan_cfg, &nums))
		{
			cs_printf("epon_request_onu_vlan_get failed!\n");
			return CS_E_ERROR;
		}
		//端口以untag 形式在vlan 中
		if(vlan == def_vlan.vid)
		{
			vlan_exist = 1;
			continue;
		}
		
		int i = 0;
		for(i=0; i<nums; i++)
		{
			//端口以tag 形式在vlan 中
			if(vlan == vlan_cfg[i].c_vlan.vid)
			{
				vlan_exist = 1;
				break;
			}
		}

		//端口不在vlan 中
		if(1 != vlan_exist)
		{
			if(nums < __VLAN_RULE_PER_PORT_MAX)
			{
				vlan_cfg[nums].c_vlan.vid = vlan;
				vlan_cfg[nums].s_vlan.vid = vlan;
				nums++;
				if(CS_E_OK != epon_request_onu_vlan_set(context, 0, 0, port_id, def_vlan, vlan_mode, vlan_cfg, nums))
				{
					cs_printf("epon_request_onu_vlan_set failed!\n");
					return CS_E_ERROR;
				}
			}
		}
	}
	
	return CS_E_OK;
}
#endif


/*  end of file */


