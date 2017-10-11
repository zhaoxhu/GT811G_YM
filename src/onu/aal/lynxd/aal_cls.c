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
 
DEFINITIONS:  "DEVICE" means the Cortina Systems LynxD product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems software.
 
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
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING CS_OUT
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
#include "aal_cls.h"
#include "aal_cls_util.h"
#include "aal_util.h"


cs_aal_cl_domain_res_t  g_ce_domain_res[CL_PORT_TYPE_NUM][AAL_CL_DOMAIN_ID_NUM];
cs_uint8                g_ce_pri_res[CL_PORT_TYPE_NUM][48];                      //max pri 47


/*****************************************************************************/
/*                    Local Functions                                        */
/*****************************************************************************/

static cs_status __cl_set_ctrl_pkt_key_entry(
 CS_IN   cs_aal_cl_rule_cfg_t *cfg, 
 CS_OUT  cl_key_entry_t       *pdata)
{  
    cs_uint32 i                    = 0;
    cl_ctrl_pkt_key_t        *pkey = NULL;
    cs_aal_cl_ctrl_pkt_key_t *pcfg = NULL;
  
           
    if((pdata==NULL)||(cfg==NULL))
    {    
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
     
    for (i = 0; i<4; i++)
    {  
        pcfg = (cs_aal_cl_ctrl_pkt_key_t *)(&cfg->cl_key.ctrl_pkt_key[i]);
        pkey = (cl_ctrl_pkt_key_t *)(&pdata->u.cl_ctrl_pkt_key[i]);

        pkey->spl_pkt_en = pcfg->spl_pkt_en;
        
        if(pcfg->mask.s.mac)
        {  
            memcpy(&pkey->mac_da[0], &pcfg->mac_da[0], 6*sizeof(cs_uint8));
            pkey->macmask = pcfg->macmask;
            pkey->mask |= CL_KEY_MASK_MACDA;
        }
        if(pcfg->spl_pkt_en)
        {                 
            pkey->special_packet_vec = pcfg->special_packet_vec;          
        }
        else
        {          
            if(pcfg->mask.s.ev2pt)
            {                 
                pkey->special_packet_vec = pcfg->special_packet_vec;    
                pkey->mask |= CL_KEY_MASK_EV2PT;
            }         
        }         
        if(pcfg->mask.s.mask_all_msk)
        {         
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        }
    }
    
    return CS_E_OK;
}

static cs_status __cl_get_ctrl_pkt_key_entry(
 CS_IN  cl_key_entry_t       *pdata, 
 CS_OUT cs_aal_cl_rule_cfg_t *cfg)
{   
    cs_uint32 i;
    cl_ctrl_pkt_key_t        *pkey = NULL;
    cs_aal_cl_ctrl_pkt_key_t *pcfg = NULL;
     
    if((pdata==NULL)||(cfg==NULL))
    {  
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
  
    for (i = 0; i<4; i++)
    {    
        pkey = (cl_ctrl_pkt_key_t *)(&pdata->u.cl_ctrl_pkt_key[i]);
        pcfg = (cs_aal_cl_ctrl_pkt_key_t *)(&cfg->cl_key.ctrl_pkt_key[i]);

        pcfg->spl_pkt_en = pkey->spl_pkt_en;
        
        if(pkey->mask&CL_KEY_MASK_MACDA)
        {  
            memcpy(&pcfg->mac_da[0], &pkey->mac_da[0],6*sizeof(cs_uint8));
            pcfg->macmask = pkey->macmask;
            pcfg->mask.s.mac = 1;
        }
        if(pkey->spl_pkt_en)
        {                 
            pcfg->special_packet_vec = pkey->special_packet_vec;          
        }
        else
        {          
            if(pkey->mask&CL_KEY_MASK_EV2PT)
            {                 
                pcfg->special_packet_vec = pkey->special_packet_vec;    
                pcfg->mask.s.ev2pt = 1;
            }         
        }         
        if (pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
        {
            pcfg->mask.s.mask_all_msk = 1;
        } 
    }
     
    return CS_E_OK;
}

static cs_status __cl_set_ipv4_long_key_entry(
 CS_IN  cs_aal_cl_rule_cfg_t *cfg, 
 CS_OUT cl_key_entry_t       *pdata)
{
    cl_ipv4_long_key_t        *pkey = NULL;
    cs_aal_cl_ipv4_long_key_t *pcfg = NULL;
      
    if((pdata==NULL)||(cfg==NULL))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    pcfg = (cs_aal_cl_ipv4_long_key_t *)(&cfg->cl_key.ipv4_long_key);
    pkey = (cl_ipv4_long_key_t *)(&pdata->u.cl_ipv4_long_key);

    if(pcfg->mask.u32 ==0)
        return CS_E_OK; 
      
    if(pcfg->mask.s.spid)
    {
        pkey->spid = pcfg->spid;
        pkey->mask |= CL_KEY_MASK_SPID;
    }
    if(pcfg->mask.s.vlan_count)
    {
        pkey->vlan_count = pcfg->vlan_count;
        pkey->mask |= CL_KEY_MASK_VLAN_COUNT;
    }
    if(pcfg->mask.s.p_tag_flag)
    {
        pkey->p_tag_flag = pcfg->p_tag_flag;
        pkey->mask |= CL_KEY_MASK_P_TAG;
    }
    if(pcfg->mask.s.tpid_enc)
    {
        pkey->tpid_enc = pcfg->tpid_enc;
        pkey->mask |= CL_KEY_MASK_TPID;
    }
    if(pcfg->mask.s.igmp_type)
    {
        pkey->igmp_type = pcfg->igmp_type;
        pkey->mask |= CL_KEY_MASK_IGMP_TYPE;
    }
    if(pcfg->mask.s.icmp_type)
    {
        pkey->icmp_type = pcfg->icmp_type;
        pkey->mask |= CL_KEY_MASK_ICMP_TYPE;
    }
    if(pcfg->mask.s.tcp_flags)
    {
        pkey->tcp_flags = pcfg->tcp_flags;
        pkey->mask |= CL_KEY_MASK_TCP_FLAGS;
    }
    if(pcfg->mask.s.l4_port)
    {
        pkey->l4_port_type = pcfg->l4_port_type;
        pkey->l4_port = pcfg->l4_port;
        pkey->mask |= CL_KEY_MASK_L4PORT;
    }
    if(pcfg->mask.s.ip_fragment)
    {
        pkey->ip_fragment_state = pcfg->ip_fragment_state;
        pkey->mask |= CL_KEY_MASK_IPFRAGMENT;
    }
    if(pcfg->mask.s.ip_option)
    {
        pkey->ip_option_flag = pcfg->ip_option_flag;
        pkey->mask |= CL_KEY_MASK_IPOPTION;
    }
    if(pcfg->mask.s.ipdscp)
    {
        pkey->ipdscp = pcfg->ipdscp;
        pkey->mask |= CL_KEY_MASK_DSCP;
    }
    if(pcfg->mask.s.ipprotocol)
    {
        pkey->ipprotocol = pcfg->ipprotocol;
        pkey->mask |= CL_KEY_MASK_IPPROTO;
    }
    if(pcfg->mask.s.srcip)
    {
        pkey->srcip = pcfg->srcip;
        pkey->srcipmask = pcfg->srcipmask;
        pkey->mask |= CL_KEY_MASK_IPSA;
    }
    if(pcfg->mask.s.dstip)
    {
        pkey->dstip = pcfg->dstip;
        pkey->dstipmask = pcfg->dstipmask;
        pkey->mask |= CL_KEY_MASK_IPDA;
    }
    if(pcfg->mask.s.ev2pt)
    {
        pkey->ev2pt = pcfg->ev2pt;
        pkey->mask |= CL_KEY_MASK_EV2PT;
    }
    if(pcfg->mask.s.mac_sa)
    {
        memcpy(&pkey->srcmac[0], &pcfg->srcmac[0], 6*sizeof(cs_uint8));
        pkey->mask |= CL_KEY_MASK_MACSA;
    }
    if(pcfg->mask.s.mac_da)
    {
        memcpy(&pkey->dstmac[0], &pcfg->dstmac[0], 6*sizeof(cs_uint8));
        pkey->mask |= CL_KEY_MASK_MACDA;
    }
    if(pcfg->mask.s.prsr_len_excds)
    {
        pkey->prsr_len_excds = pcfg->prsr_len_excds;
        pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
    }
    if(pcfg->mask.s.dot1p)
    {
        pkey->dot1p = pcfg->dot1p;
        pkey->mask |= CL_KEY_MASK_DOT1P;
    }
    if(pcfg->mask.s.vlanid)
    {
        pkey->vlanid = pcfg->vlanid;
        pkey->mask |= CL_KEY_MASK_VLANID;
    }
    if(pcfg->mask.s.spcl_hit_map)
    {
        pkey->spcl_hit_map = pcfg->spcl_hit_map;
        pkey->mask |= CL_KEY_MASK_SPECPKT;
    }
    if(pcfg->mask.s.l4_src_hit_map)
    {
        pkey->l4_src_hit_map = pcfg->l4_src_hit_map;
        pkey->mask |= CL_KEY_MASK_L4SRC_RANGE;
    }
    if(pcfg->mask.s.l4_dest_hit_map)
    {
        pkey->l4_dest_hit_map = pcfg->l4_dest_hit_map;
        pkey->mask |= CL_KEY_MASK_L4DST_RANGE;
    }
    if(pcfg->mask.s.mask_all_msk)
    {         
        pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
    }

    return CS_E_OK;
}

static cs_status __cl_get_ipv4_long_key_entry(
 CS_IN  cl_key_entry_t       *pdata, 
 CS_OUT cs_aal_cl_rule_cfg_t *cfg)
{
    cl_ipv4_long_key_t        *pkey = NULL;
    cs_aal_cl_ipv4_long_key_t *pcfg = NULL;
     
    if((pdata==NULL)||(cfg==NULL))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }    
        
    pkey = (cl_ipv4_long_key_t *)(&pdata->u.cl_ipv4_long_key);
    
    if(pkey->mask ==0)
        return CS_E_OK; 
    
    pcfg = (cs_aal_cl_ipv4_long_key_t *)(&cfg->cl_key.ipv4_long_key);

    if(pkey->mask&CL_KEY_MASK_SPID)
    {
        pcfg->spid = pkey->spid;
        pcfg->mask.s.spid = 1;
    }
    if(pkey->mask&CL_KEY_MASK_VLAN_COUNT)
    {
        pcfg->vlan_count = pkey->vlan_count;
        pcfg->mask.s.vlan_count = 1;
    }
    if(pkey->mask&CL_KEY_MASK_P_TAG)
    {
        pcfg->p_tag_flag = pkey->p_tag_flag;
        pcfg->mask.s.p_tag_flag = 1;
    }
    if(pkey->mask&CL_KEY_MASK_TPID)
    {
        pcfg->tpid_enc = pkey->tpid_enc;
        pcfg->mask.s.tpid_enc = 1;
    }
    if(pkey->mask&CL_KEY_MASK_IGMP_TYPE)
    {
        pcfg->igmp_type = pkey->igmp_type;
        pcfg->mask.s.igmp_type = 1;
    }
    if(pkey->mask&CL_KEY_MASK_ICMP_TYPE)
    {
        pcfg->icmp_type = pkey->icmp_type;
        pcfg->mask.s.icmp_type = 1;
    }
    if(pkey->mask&CL_KEY_MASK_TCP_FLAGS)
    {
        pcfg->tcp_flags = pkey->tcp_flags;
        pcfg->mask.s.tcp_flags = 1;
    }
    if(pkey->mask&CL_KEY_MASK_L4PORT)
    {
        pcfg->l4_port_type = pkey->l4_port_type;
        pcfg->l4_port = pkey->l4_port;
        pcfg->mask.s.l4_port = 1;
    }
    if(pkey->mask&CL_KEY_MASK_IPFRAGMENT)
    {
        pcfg->ip_fragment_state = pkey->ip_fragment_state;
        pcfg->mask.s.ip_fragment = 1;
    }
    if(pkey->mask&CL_KEY_MASK_IPOPTION)
    {
        pcfg->ip_option_flag = pkey->ip_option_flag;
        pcfg->mask.s.ip_option = 1;
    }
    if(pkey->mask&CL_KEY_MASK_DSCP)
    {
        pcfg->ipdscp = pkey->ipdscp;
        pcfg->mask.s.ipdscp = 1;
    }
    if(pkey->mask&CL_KEY_MASK_IPPROTO)
    {
        pcfg->ipprotocol = pkey->ipprotocol;
        pcfg->mask.s.ipprotocol = 1;
    }
    if(pkey->mask&CL_KEY_MASK_IPSA)
    {
        pcfg->srcip = pkey->srcip;
        pcfg->srcipmask = pkey->srcipmask;
        pcfg->mask.s.srcip = 1;
    }
    if(pkey->mask&CL_KEY_MASK_IPDA)
    {
        pcfg->dstip = pkey->dstip;
        pcfg->dstipmask = pkey->dstipmask;
        pcfg->mask.s.dstip = 1;
    }
    if(pkey->mask&CL_KEY_MASK_EV2PT)
    {
        pcfg->ev2pt = pkey->ev2pt;
        pcfg->mask.s.ev2pt = 1;
    }
    if(pkey->mask&CL_KEY_MASK_MACSA)
    {
        memcpy(&pcfg->srcmac[0], &pkey->srcmac[0], 6*sizeof(cs_uint8));
        pcfg->mask.s.mac_sa = 1;
    }
    if(pkey->mask&CL_KEY_MASK_MACDA)
    {
        memcpy(&pcfg->dstmac[0], &pkey->dstmac[0], 6*sizeof(cs_uint8));
        pcfg->mask.s.mac_da = 1;
    }
    if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
    {
        pcfg->prsr_len_excds = pkey->prsr_len_excds;
        pcfg->mask.s.prsr_len_excds = 1;
    }
    if(pkey->mask&CL_KEY_MASK_DOT1P)
    {
        pcfg->dot1p = pkey->dot1p;
        pcfg->mask.s.dot1p = 1;
    }
    if(pkey->mask&CL_KEY_MASK_VLANID)
    {
        pcfg->vlanid = pkey->vlanid;
        pcfg->mask.s.vlanid = 1;
    }
    if(pkey->mask&CL_KEY_MASK_SPECPKT)
    {
        pcfg->spcl_hit_map = pkey->spcl_hit_map;
        pcfg->mask.s.spcl_hit_map = 1;
    }
    if(pkey->mask&CL_KEY_MASK_L4SRC_RANGE)
    {
        pcfg->l4_src_hit_map = pkey->l4_src_hit_map;
        pcfg->mask.s.l4_src_hit_map = 1;
    }
    if(pkey->mask&CL_KEY_MASK_L4DST_RANGE)
    {
        pcfg->l4_dest_hit_map = pkey->l4_dest_hit_map;
        pcfg->mask.s.l4_dest_hit_map = 1;
    }
    if (pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
    {
        pcfg->mask.s.mask_all_msk = 1;
    } 
      
    return CS_E_OK;
}

static cs_status __cl_set_ipv4_short_key_entry(
 CS_IN  cs_aal_cl_rule_cfg_t *cfg, 
 CS_OUT cl_key_entry_t       *pdata)
{  
    cs_uint32 i;
    cl_ipv4_short_key_t        *pkey = NULL;
    cs_aal_cl_ipv4_short_key_t *pcfg = NULL;
  
           
    if((pdata==NULL)||(cfg==NULL))
    {  
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
     
    for (i = 0; i<4; i++)
    {  
        pcfg = (cs_aal_cl_ipv4_short_key_t *)(&cfg->cl_key.ipv4_short_key[i]);
        pkey = (cl_ipv4_short_key_t *)(&pdata->u.cl_ipv4_short_key[i]);

        if(pcfg->mask.u32 ==0)
            continue;
            
        if(pcfg->mask.s.ip)
        {
            pkey->ip = pcfg->ip;
            pkey->ip_type = pcfg->ip_type;
            pkey->ipmask = pcfg->ipmask;
            pkey->mask |= CL_KEY_MASK_IPADDR;
        }
        if(pcfg->mask.s.dscp_tos)
        {
            pkey->dscp_tos = pcfg->dscp_tos;         
            pkey->mask |= CL_KEY_MASK_DSCP;
        }
        if(pcfg->mask.s.ipprotocol)
        {
            pkey->ipprotocol = pcfg->ipprotocol;         
            pkey->mask |= CL_KEY_MASK_IPPROTO;
        }
        if(pcfg->mask.s.l4_port)
        {
            pkey->l4_port = pcfg->l4_port;      
            pkey->l4_port_type = pcfg->l4_port_type;     
            pkey->mask |= CL_KEY_MASK_L4PORT;
        } 
        if(pcfg->mask.s.prsr_len_excds)
        {
            pkey->prsr_len_excds = pcfg->prsr_len_excds;         
            pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
        }
        if(pcfg->mask.s.mask_all_msk)
        {         
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        }
    }
    
    return CS_E_OK;
}

static cs_status __cl_get_ipv4_short_key_entry(
 CS_IN  cl_key_entry_t       *pdata, 
 CS_OUT cs_aal_cl_rule_cfg_t *cfg)
{   
    cs_uint32 i;
    cl_ipv4_short_key_t        *pkey = NULL;
    cs_aal_cl_ipv4_short_key_t *pcfg = NULL;
     
    if((pdata==NULL)||(cfg==NULL))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
  
    for (i = 0; i<4; i++)
    {    
        pkey = (cl_ipv4_short_key_t *)(&pdata->u.cl_ipv4_short_key[i]);
        pcfg = (cs_aal_cl_ipv4_short_key_t *)(&cfg->cl_key.ipv4_short_key[i]);

        if(pkey->mask ==0)
            continue;
        if(pkey->mask&CL_KEY_MASK_IPADDR)
        {                 
            pcfg->ip = pkey->ip;
            pcfg->ip_type = pkey->ip_type;
            pcfg->ipmask = pkey->ipmask;
            pcfg->mask.s.ip = 1;
        }        
        if(pkey->mask&CL_KEY_MASK_DSCP)
        {                 
            pcfg->dscp_tos = pkey->dscp_tos;
            pcfg->mask.s.dscp_tos = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_IPPROTO)
        {                 
            pcfg->ipprotocol = pkey->ipprotocol;
            pcfg->mask.s.ipprotocol = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_L4PORT)
        {                 
            pcfg->l4_port = pkey->l4_port;
            pcfg->l4_port_type = pkey->l4_port_type;
            pcfg->mask.s.l4_port = 1;
        }  
        if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
        {                 
            pcfg->prsr_len_excds = pkey->prsr_len_excds;
            pcfg->mask.s.prsr_len_excds = 1;
        }       
        if (pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
        {
            pcfg->mask.s.mask_all_msk = 1;
        } 
    }
     
    return CS_E_OK;
}

static cs_status __cl_set_ipv6_long_key_entry(
 CS_IN  cs_aal_cl_rule_cfg_t *cfg, 
 CS_OUT cl_key_entry_t       *pdata)
{
    cl_ipv6_long_key_t        *pkey = NULL;
    cs_aal_cl_ipv6_long_key_t *pcfg = NULL;
      
    if((pdata==NULL)||(cfg==NULL))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    pcfg = (cs_aal_cl_ipv6_long_key_t *)(&cfg->cl_key.ipv6_long_key);
    pkey = (cl_ipv6_long_key_t *)(&pdata->u.cl_ipv6_long_key);

    if(pcfg->mask.u32 ==0)
        return CS_E_OK; 

    if(pcfg->mask.s.ipv6_flow_label)
    {
        pkey->flow_label = pcfg->ipv6_flow_label;
        pkey->mask |= CL_KEY_MASK_FLOW_LABEL;
    }
    if(pcfg->mask.s.ipv6_sa)
    {
        pkey->ipv6_sa[0] = pcfg->ipv6_sa[0];
        pkey->ipv6_sa[1] = pcfg->ipv6_sa[1];
        pkey->ipv6_sa[2] = pcfg->ipv6_sa[2];
        pkey->ipv6_sa[3] = pcfg->ipv6_sa[3];
        pkey->ipv6_sa[4] = pcfg->ipv6_sa[4];
        pkey->ipv6_sa[5] = pcfg->ipv6_sa[5];
        pkey->ipv6_sa[6] = pcfg->ipv6_sa[6];
        pkey->ipv6_sa[7] = pcfg->ipv6_sa[7];
        pkey->srcipmask = pcfg->srcipmask;
        pkey->mask |= CL_KEY_MASK_IPSA;
    }
    if(pcfg->mask.s.ipv6_da)
    {
        pkey->ipv6_da[0] = pcfg->ipv6_da[0];
        pkey->ipv6_da[1] = pcfg->ipv6_da[1];
        pkey->ipv6_da[2] = pcfg->ipv6_da[2];
        pkey->ipv6_da[3] = pcfg->ipv6_da[3];
        pkey->ipv6_da[4] = pcfg->ipv6_da[4];
        pkey->ipv6_da[5] = pcfg->ipv6_da[5];
        pkey->ipv6_da[6] = pcfg->ipv6_da[6];
        pkey->ipv6_da[7] = pcfg->ipv6_da[7];
        pkey->dstipmask = pcfg->dstipmask;
        pkey->mask |= CL_KEY_MASK_IPDA;
    }
    if(pcfg->mask.s.spid)
    {
        pkey->spid = pcfg->spid;
        pkey->mask |= CL_KEY_MASK_SPID;
    }
    if(pcfg->mask.s.vlan_count)
    {
        pkey->vlan_count = pcfg->vlan_count;
        pkey->mask |= CL_KEY_MASK_VLAN_COUNT;
    }
    if(pcfg->mask.s.p_tag_flag)
    {
        pkey->p_tag_flag = pcfg->p_tag_flag;
        pkey->mask |= CL_KEY_MASK_P_TAG;
    }
    if(pcfg->mask.s.tpid_enc)
    {
        pkey->tpid_enc = pcfg->tpid_enc;
        pkey->mask |= CL_KEY_MASK_TPID;
    }
    if(pcfg->mask.s.igmp_type)
    {
        pkey->igmp_type = pcfg->igmp_type;
        pkey->mask |= CL_KEY_MASK_IGMP_TYPE;
    }
    if(pcfg->mask.s.icmp_type)
    {
        pkey->icmp_type = pcfg->icmp_type;
        pkey->mask |= CL_KEY_MASK_ICMP_TYPE;
    }
    if(pcfg->mask.s.tcp_flags)
    {
        pkey->tcp_flags = pcfg->tcp_flags;
        pkey->mask |= CL_KEY_MASK_TCP_FLAGS;
    }
    if(pcfg->mask.s.l4_port)
    {
        pkey->l4_port_type = pcfg->l4_port_type;
        pkey->l4_port = pcfg->l4_port;
        pkey->mask |= CL_KEY_MASK_L4PORT;
    }
    if(pcfg->mask.s.ip_fragment)
    {
        pkey->ip_fragment_state = pcfg->ip_fragment_state;
        pkey->mask |= CL_KEY_MASK_IPFRAGMENT;
    }
    if(pcfg->mask.s.ip_option)
    {
        pkey->ip_option_flag = pcfg->ip_option_flag;
        pkey->mask |= CL_KEY_MASK_IPOPTION;
    }
    if(pcfg->mask.s.ipv6_tc)
    {
        pkey->ipdscp = pcfg->ipv6_tc;
        pkey->mask |= CL_KEY_MASK_DSCP;
    }
    if(pcfg->mask.s.ipprotocol)
    {
        pkey->ipprotocol = pcfg->ipprotocol;
        pkey->mask |= CL_KEY_MASK_IPPROTO;
    } 
    if(pcfg->mask.s.ev2pt)
    {
        pkey->ev2pt = pcfg->ev2pt;
        pkey->mask |= CL_KEY_MASK_EV2PT;
    }
    if(pcfg->mask.s.mac_sa)
    {
        memcpy(&pkey->srcmac[0], &pcfg->srcmac[0], 6*sizeof(cs_uint8));
        pkey->mask |= CL_KEY_MASK_MACSA;
    }
    if(pcfg->mask.s.mac_da)
    {
        memcpy(&pkey->dstmac[0], &pcfg->dstmac[0], 6*sizeof(cs_uint8));
        pkey->mask |= CL_KEY_MASK_MACDA;
    }
    if(pcfg->mask.s.prsr_len_excds)
    {
        pkey->prsr_len_excds = pcfg->prsr_len_excds;
        pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
    }
    if(pcfg->mask.s.dot1p)
    {
        pkey->dot1p = pcfg->dot1p;
        pkey->mask |= CL_KEY_MASK_DOT1P;
    }
    if(pcfg->mask.s.vlanid)
    {
        pkey->vlanid = pcfg->vlanid;
        pkey->mask |= CL_KEY_MASK_VLANID;
    }
    if(pcfg->mask.s.spcl_hit_map)
    {
        pkey->spcl_hit_map = pcfg->spcl_hit_map;
        pkey->mask |= CL_KEY_MASK_SPECPKT;
    }
    if(pcfg->mask.s.l4_src_hit_map)
    {
        pkey->l4_src_hit_map = pcfg->l4_src_hit_map;
        pkey->mask |= CL_KEY_MASK_L4SRC_RANGE;
    }
    if(pcfg->mask.s.l4_dest_hit_map)
    {
        pkey->l4_dest_hit_map = pcfg->l4_dest_hit_map;
        pkey->mask |= CL_KEY_MASK_L4DST_RANGE;
    }
    
    if(pcfg->mask.s.mask_all_msk)
    {         
        pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
    }
     
    return CS_E_OK;
}

static cs_status __cl_get_ipv6_long_key_entry(
 CS_IN  cl_key_entry_t       *pdata, 
 CS_OUT cs_aal_cl_rule_cfg_t *cfg)
{
    cl_ipv6_long_key_t        *pkey = NULL;
    cs_aal_cl_ipv6_long_key_t *pcfg = NULL;
     
    if((pdata==NULL)||(cfg==NULL))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }    
    
    pkey = (cl_ipv6_long_key_t *)(&pdata->u.cl_ipv6_long_key);
    pcfg = (cs_aal_cl_ipv6_long_key_t *)(&cfg->cl_key.ipv6_long_key);
    
    if(pkey->mask ==0)
        return CS_E_OK; 
    
    if(pkey->mask&CL_KEY_MASK_FLOW_LABEL)
    {
        pcfg->ipv6_flow_label = pkey->flow_label;
        pcfg->mask.s.ipv6_flow_label = 1;
    }
    if(pkey->mask&CL_KEY_MASK_IPSA)
    {
        pcfg->ipv6_sa[0] = pkey->ipv6_sa[0];
        pcfg->ipv6_sa[1] = pkey->ipv6_sa[1];
        pcfg->ipv6_sa[2] = pkey->ipv6_sa[2];
        pcfg->ipv6_sa[3] = pkey->ipv6_sa[3];
        pcfg->ipv6_sa[4] = pkey->ipv6_sa[4];
        pcfg->ipv6_sa[5] = pkey->ipv6_sa[5];
        pcfg->ipv6_sa[6] = pkey->ipv6_sa[6];
        pcfg->ipv6_sa[7] = pkey->ipv6_sa[7];
        pcfg->srcipmask = pkey->srcipmask;
        pcfg->mask.s.ipv6_sa = 1;
    }
    if(pkey->mask&CL_KEY_MASK_IPDA)
    {
        pcfg->ipv6_da[0]= pkey->ipv6_da[0];
        pcfg->ipv6_da[1]= pkey->ipv6_da[1];
        pcfg->ipv6_da[2]= pkey->ipv6_da[2];
        pcfg->ipv6_da[3]= pkey->ipv6_da[3];
        pcfg->ipv6_da[4]= pkey->ipv6_da[4];
        pcfg->ipv6_da[5]= pkey->ipv6_da[5];
        pcfg->ipv6_da[6]= pkey->ipv6_da[6];
        pcfg->ipv6_da[7]= pkey->ipv6_da[7];
        pcfg->dstipmask = pkey->dstipmask;
        pcfg->mask.s.ipv6_da = 1;
    }    
    if(pkey->mask&CL_KEY_MASK_SPID)
    {
        pcfg->spid = pkey->spid;
        pcfg->mask.s.spid = 1;
    }
    if(pkey->mask&CL_KEY_MASK_VLAN_COUNT)
    {
        pcfg->vlan_count = pkey->vlan_count;
        pcfg->mask.s.vlan_count = 1;
    }
    if(pkey->mask&CL_KEY_MASK_P_TAG)
    {
        pcfg->p_tag_flag = pkey->p_tag_flag;
        pcfg->mask.s.p_tag_flag = 1;
    }
    if(pkey->mask&CL_KEY_MASK_TPID)
    {
        pcfg->tpid_enc = pkey->tpid_enc;
        pcfg->mask.s.tpid_enc = 1;
    }
    if(pkey->mask&CL_KEY_MASK_IGMP_TYPE)
    {
        pcfg->igmp_type = pkey->igmp_type;
        pcfg->mask.s.igmp_type = 1;
    }
    if(pkey->mask&CL_KEY_MASK_ICMP_TYPE)
    {
        pcfg->icmp_type = pkey->icmp_type;
        pcfg->mask.s.icmp_type = 1;
    }
    if(pkey->mask&CL_KEY_MASK_TCP_FLAGS)
    {
        pcfg->tcp_flags = pkey->tcp_flags;
        pcfg->mask.s.tcp_flags = 1;
    }
    if(pkey->mask&CL_KEY_MASK_L4PORT)
    {
        pcfg->l4_port_type = pkey->l4_port_type;
        pcfg->l4_port = pkey->l4_port;
        pcfg->mask.s.l4_port = 1;
    }
    if(pkey->mask&CL_KEY_MASK_IPFRAGMENT)
    {
        pcfg->ip_fragment_state = pkey->ip_fragment_state;
        pcfg->mask.s.ip_fragment = 1;
    }
    if(pkey->mask&CL_KEY_MASK_IPOPTION)
    {
        pcfg->ip_option_flag = pkey->ip_option_flag;
        pcfg->mask.s.ip_option = 1;
    }
    if(pkey->mask&CL_KEY_MASK_DSCP)
    {
        pcfg->ipv6_tc = pkey->ipdscp;
        pcfg->mask.s.ipv6_tc = 1;
    }
    if(pkey->mask&CL_KEY_MASK_IPPROTO)
    {
        pcfg->ipprotocol = pkey->ipprotocol;
        pcfg->mask.s.ipprotocol = 1;
    }
    if(pkey->mask&CL_KEY_MASK_EV2PT)
    {
        pcfg->ev2pt = pkey->ev2pt;
        pcfg->mask.s.ev2pt = 1;
    }
    if(pkey->mask&CL_KEY_MASK_MACSA)
    {
        memcpy(&pcfg->srcmac[0], &pkey->srcmac[0], 6*sizeof(cs_uint8));
        pcfg->mask.s.mac_sa = 1;
    }
    if(pkey->mask&CL_KEY_MASK_MACDA)
    {
        memcpy(&pcfg->dstmac[0], &pkey->dstmac[0], 6*sizeof(cs_uint8));
        pcfg->mask.s.mac_da = 1;
    }
    if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
    {
        pcfg->prsr_len_excds = pkey->prsr_len_excds;
        pcfg->mask.s.prsr_len_excds = 1;
    }
    if(pkey->mask&CL_KEY_MASK_DOT1P)
    {
        pcfg->dot1p = pkey->dot1p;
        pcfg->mask.s.dot1p = 1;
    }
    if(pkey->mask&CL_KEY_MASK_VLANID)
    {
        pcfg->vlanid = pkey->vlanid;
        pcfg->mask.s.vlanid = 1;
    }
    if(pkey->mask&CL_KEY_MASK_SPECPKT)
    {
        pcfg->spcl_hit_map = pkey->spcl_hit_map;
        pcfg->mask.s.spcl_hit_map = 1;
    }
    if(pkey->mask&CL_KEY_MASK_L4SRC_RANGE)
    {
        pcfg->l4_src_hit_map = pkey->l4_src_hit_map;
        pcfg->mask.s.l4_src_hit_map = 1;
    }
    if(pkey->mask&CL_KEY_MASK_L4DST_RANGE)
    {
        pcfg->l4_dest_hit_map = pkey->l4_dest_hit_map;
        pcfg->mask.s.l4_dest_hit_map = 1;
    }
    if (pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
    {
        pcfg->mask.s.mask_all_msk = 1;
    } 
      
    return CS_E_OK;
}

static cs_status __cl_set_ipv6_short_key_entry(
 CS_IN  cs_aal_cl_rule_cfg_t *cfg, 
 CS_OUT cl_key_entry_t       *pdata)
{  
    cl_ipv6_short_key_t        *pkey = NULL;
    cs_aal_cl_ipv6_short_key_t *pcfg = NULL;
             
    if((pdata==NULL)||(cfg==NULL))
    {    
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
     
    pcfg = (cs_aal_cl_ipv6_short_key_t *)(&cfg->cl_key.ipv6_short_key);
    pkey = (cl_ipv6_short_key_t *)(&pdata->u.cl_ipv6_short_key);

    if(pcfg->mask.u32 ==0)
        return CS_E_OK; 
    
    if(pcfg->mask.s.ipv6_addr)
    {
        pkey->ipv6_addr[0] = pcfg->ipv6_addr[0];
        pkey->ipv6_addr[1] = pcfg->ipv6_addr[1];
        pkey->ipv6_addr[2] = pcfg->ipv6_addr[2];
        pkey->ipv6_addr[3] = pcfg->ipv6_addr[3];
        pkey->ipv6_addr[4] = pcfg->ipv6_addr[4];
        pkey->ipv6_addr[5] = pcfg->ipv6_addr[5];
        pkey->ipv6_addr[6] = pcfg->ipv6_addr[6];
        pkey->ipv6_addr[7] = pcfg->ipv6_addr[7];
        pkey->ip_type = pcfg->ip_type;
        pkey->ipmask = pcfg->ipmask;
        pkey->mask |= CL_KEY_MASK_IPADDR;
    }
    if(pcfg->mask.s.dscp_tos)
    {
        pkey->dscp_tos = pcfg->dscp_tos;         
        pkey->mask |= CL_KEY_MASK_DSCP;
    }
    if(pcfg->mask.s.ipprotocol)
    {
        pkey->ipprotocol = pcfg->ipprotocol;         
        pkey->mask |= CL_KEY_MASK_IPPROTO;
    }
    if(pcfg->mask.s.l4_port)
    {
        pkey->l4_port = pcfg->l4_port;      
        pkey->l4_port_type = pcfg->l4_port_type;     
        pkey->mask |= CL_KEY_MASK_L4PORT;
    } 
    if(pcfg->mask.s.prsr_len_excds)
    {
        pkey->prsr_len_excds = pcfg->prsr_len_excds;         
        pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
    }
    if(pcfg->mask.s.ipv6_flow_label)
    {
        pkey->flow_label = pcfg->ipv6_flow_label;         
        pkey->mask |= CL_KEY_MASK_FLOW_LABEL;
    }
    if(pcfg->mask.s.mask_all_msk)
    {         
        pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
    }
    
    return CS_E_OK;
}

static cs_status __cl_get_ipv6_short_key_entry(
 CS_IN  cl_key_entry_t       *pdata, 
 CS_OUT cs_aal_cl_rule_cfg_t *cfg)
{  
    cl_ipv6_short_key_t        *pkey = NULL;
    cs_aal_cl_ipv6_short_key_t *pcfg = NULL;
     
    if((pdata==NULL)||(cfg==NULL))
    {  
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    pkey = (cl_ipv6_short_key_t *)(&pdata->u.cl_ipv6_short_key);
    pcfg = (cs_aal_cl_ipv6_short_key_t *)(&cfg->cl_key.ipv6_short_key);
    
    if(pkey->mask ==0)
        return CS_E_OK; 
    
    if(pkey->mask&CL_KEY_MASK_IPADDR)
    {                 
        pcfg->ipv6_addr[0] = pkey->ipv6_addr[0];
        pcfg->ipv6_addr[1] = pkey->ipv6_addr[1];
        pcfg->ipv6_addr[2] = pkey->ipv6_addr[2];
        pcfg->ipv6_addr[3] = pkey->ipv6_addr[3];
        pcfg->ipv6_addr[4] = pkey->ipv6_addr[4];
        pcfg->ipv6_addr[5] = pkey->ipv6_addr[5];
        pcfg->ipv6_addr[6] = pkey->ipv6_addr[6];
        pcfg->ipv6_addr[7] = pkey->ipv6_addr[7];
        pcfg->ip_type = pkey->ip_type;
        pcfg->ipmask = pkey->ipmask;
        pcfg->mask.s.ipv6_addr = 1;
    }        
    if(pkey->mask&CL_KEY_MASK_DSCP)
    {                 
        pcfg->dscp_tos = pkey->dscp_tos;
        pcfg->mask.s.dscp_tos = 1;
    }       
    if(pkey->mask&CL_KEY_MASK_IPPROTO)
    {                 
        pcfg->ipprotocol = pkey->ipprotocol;
        pcfg->mask.s.ipprotocol = 1;
    }           
    if(pkey->mask&CL_KEY_MASK_L4PORT)
    {                 
        pcfg->l4_port = pkey->l4_port;
        pcfg->l4_port_type = pkey->l4_port_type;
        pcfg->mask.s.l4_port = 1;
    }  
    if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
    {                 
        pcfg->prsr_len_excds = pkey->prsr_len_excds;
        pcfg->mask.s.prsr_len_excds = 1;
    }       
    if(pkey->mask&CL_KEY_MASK_FLOW_LABEL)
    {                 
        pcfg->ipv6_flow_label = pkey->flow_label;
        pcfg->mask.s.ipv6_flow_label = 1;
    }       
    if (pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
    {
        pcfg->mask.s.mask_all_msk = 1;
    } 
      
    return CS_E_OK;
}

static cs_status __cl_set_mirror_key_entry(
 CS_IN  cs_aal_cl_rule_cfg_t *cfg, 
 CS_OUT cl_key_entry_t       *pdata)
{  
    cs_uint32 i;
    cl_mirror_key_t        *pkey = NULL;
    cs_aal_cl_mirror_key_t *pcfg = NULL;
  
           
    if((pdata==NULL)||(cfg==NULL))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
     
    for (i = 0; i<8; i++)
    {  
        pcfg = (cs_aal_cl_mirror_key_t *)(&cfg->cl_key.mirroe_key[i]);
        pkey = (cl_mirror_key_t *)(&pdata->u.cl_mirror_key[i]);
        
        pkey->mirror_pkt_flag = pcfg->mirror_pkt_flag;
        
        if(pcfg->mask.u32 ==0)
            continue;
        
        if(pcfg->mask.s.spid)
        {
            pkey->spid = pcfg->spid;         
            pkey->mask |= CL_KEY_MASK_SPID;
        }
        if(pcfg->mask.s.vlanid)
        {
            pkey->vlanid = pcfg->vlanid;         
            pkey->mask |= CL_KEY_MASK_VLANID;
        }     
        if(pcfg->mask.s.mirror_group_id)
        {
            pkey->mirror_group_id = pcfg->mirror_group_id;         
            pkey->mask |= CL_KEY_MASK_MIRROR_GROUP;
        } 
        if(pcfg->mask.s.mask_all_msk)
        {         
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        }
    }
    
    return CS_E_OK;
}

static cs_status __cl_get_mirror_key_entry(
 CS_IN  cl_key_entry_t       *pdata, 
 CS_OUT cs_aal_cl_rule_cfg_t *cfg)
{  
    cs_uint32 i;
    cl_mirror_key_t        *pkey = NULL;
    cs_aal_cl_mirror_key_t *pcfg = NULL;
  
           
    if((pdata==NULL)||(cfg==NULL))
    {  
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
     
    for (i = 0; i<8; i++)
    {        
        pkey = (cl_mirror_key_t *)(&pdata->u.cl_mirror_key[i]);
        pcfg = (cs_aal_cl_mirror_key_t *)(&cfg->cl_key.mirroe_key[i]);

        pcfg->mirror_pkt_flag = pkey->mirror_pkt_flag;
        if(pkey->mask ==0)
            continue;
        if(pkey->mask&CL_KEY_MASK_SPID)
        {                 
            pcfg->spid = pkey->spid;
            pcfg->mask.s.spid = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_VLANID)
        {                 
            pcfg->vlanid = pkey->vlanid;
            pcfg->mask.s.vlanid = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_MIRROR_GROUP)
        {                 
            pcfg->mirror_group_id = pkey->mirror_group_id;
            pcfg->mask.s.mirror_group_id = 1;
        }
        if (pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
        {
            pcfg->mask.s.mask_all_msk = 1;
        } 
    }
    
    return CS_E_OK;
}

static cs_status __cl_set_mac_vlan_key_entry(
 CS_IN  cs_aal_cl_rule_cfg_t *cfg, 
 CS_OUT cl_key_entry_t       *pdata)
{  
    cs_uint32 i;
    cl_mac_vlan_key_t        *pkey = NULL;
    cs_aal_cl_mac_vlan_key_t *pcfg = NULL;
  
           
    if((pdata==NULL)||(cfg==NULL))
    {  
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
     
    for (i = 0; i<4; i++)
    {  
        pcfg = (cs_aal_cl_mac_vlan_key_t *)(&cfg->cl_key.mac_vlan_key[i]);
        pkey = (cl_mac_vlan_key_t *)(&pdata->u.cl_mac_vlan_key[i]);

        pkey->age_timer_offset = pcfg->age_timer_offset;
        
        if(pcfg->mask.u32 ==0)
            continue;         
        
        if(pcfg->mask.s.mac)
        {
            memcpy(&pkey->mac[0], &pcfg->mac[0], 6*sizeof(cs_uint8));
            pkey->mac_type = pcfg->mac_type;
            pkey->mask |= CL_KEY_MASK_MACADDR;
        }
        if(pcfg->mask.s.dot1p)
        {
            pkey->dot1p = pcfg->dot1p;         
            pkey->mask |= CL_KEY_MASK_DOT1P;
        }
        if(pcfg->mask.s.vlanid)
        {
            pkey->vlanid = pcfg->vlanid;         
            pkey->mask |= CL_KEY_MASK_VLANID;
        }
        if(pcfg->mask.s.p_tag_flag)
        {
            pkey->p_tag_flag = pcfg->p_tag_flag;                 
            pkey->mask |= CL_KEY_MASK_P_TAG;
        } 
        if(pcfg->mask.s.vlan_count)
        {
            pkey->vlan_count = pcfg->vlan_count;         
            pkey->mask |= CL_KEY_MASK_VLAN_COUNT;
        }
        if(pcfg->mask.s.mask_all_msk)
        {         
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        }
    }
    
    return CS_E_OK;
}

static cs_status __cl_get_mac_vlan_key_entry(
 CS_IN  cl_key_entry_t       *pdata, 
 CS_OUT cs_aal_cl_rule_cfg_t *cfg)
{   
    cs_uint32 i;
    cl_mac_vlan_key_t        *pkey = NULL;
    cs_aal_cl_mac_vlan_key_t *pcfg = NULL;
     
    if((pdata==NULL)||(cfg==NULL))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
  
    for (i = 0; i<4; i++)
    {    
        pkey = (cl_mac_vlan_key_t *)(&pdata->u.cl_mac_vlan_key[i]);
        pcfg = (cs_aal_cl_mac_vlan_key_t *)(&cfg->cl_key.mac_vlan_key[i]);

        pcfg->age_timer_offset = pkey->age_timer_offset;
        
        if(pkey->mask ==0)
            continue;
        
        if(pkey->mask&CL_KEY_MASK_MACADDR)
        {                 
            memcpy(&pcfg->mac[0], &pkey->mac[0], 6*sizeof(cs_uint8));
            pcfg->mac_type = pkey->mac_type;
            pcfg->mask.s.mac = 1;
        }
        if(pkey->mask&CL_KEY_MASK_DOT1P)
        {                 
            pcfg->dot1p = pkey->dot1p;
            pcfg->mask.s.dot1p = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_VLANID)
        {                 
            pcfg->vlanid = pkey->vlanid;
            pcfg->mask.s.vlanid = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_P_TAG)
        {                 
            pcfg->p_tag_flag = pkey->p_tag_flag;
            pcfg->mask.s.p_tag_flag = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_VLAN_COUNT)
        {                 
            pcfg->vlan_count = pkey->vlan_count;
            pcfg->mask.s.vlan_count = 1;
        }      
        if (pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
        {
            pcfg->mask.s.mask_all_msk = 1;
        } 
    }
     
    return CS_E_OK;
}

static cs_status __cl_set_mac_etype_key_entry(
 CS_IN  cs_aal_cl_rule_cfg_t *cfg, 
 CS_OUT cl_key_entry_t       *pdata)
{   
    cs_uint32 i;
    cl_mac_etype_key_t        *pkey = NULL;
    cs_aal_cl_mac_etype_key_t *pcfg = NULL;
     
    if((pdata==NULL)||(cfg==NULL))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
  
    for (i = 0; i<4; i++)
    {  
        pcfg = (cs_aal_cl_mac_etype_key_t *)(&cfg->cl_key.mac_etype_key[i]);
        pkey = (cl_mac_etype_key_t *)(&pdata->u.cl_mac_etype_key[i]);

        pkey->age_timer_offset = pcfg->age_timer_offset;
        
        if(pcfg->mask.u32 ==0)
            continue;
                
        if(pcfg->mask.s.mac)
        {
            memcpy(&pkey->mac[0], &pcfg->mac[0], 6*sizeof(cs_uint8));
            pkey->mac_type = pcfg->mac_type;
            pkey->mask |= CL_KEY_MASK_MACADDR;
        }
        if(pcfg->mask.s.tpid_enc)
        {
            pkey->tpid_enc = pcfg->tpid_enc;         
            pkey->mask |= CL_KEY_MASK_TPID;
        }
        if(pcfg->mask.s.ev2pt)
        {
            pkey->ev2pt = pcfg->ev2pt;         
            pkey->mask |= CL_KEY_MASK_EV2PT;
        }
        if(pcfg->mask.s.mac_da_range_flag)
        {
            pkey->mac_da_range_flag = pcfg->mac_da_range_flag;                 
            pkey->mask |= CL_KEY_MASK_MACDA_RANGE_FLAG;
        }  
        if(pcfg->mask.s.mask_all_msk)
        {         
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        }
    }
     
    return CS_E_OK;
}

static cs_status __cl_get_mac_etype_key_entry(
 CS_IN  cl_key_entry_t       *pdata, 
 CS_OUT cs_aal_cl_rule_cfg_t *cfg)
{   
    cs_uint32 i;
    cl_mac_etype_key_t        *pkey = NULL;
    cs_aal_cl_mac_etype_key_t *pcfg = NULL;
     
    if((pdata==NULL)||(cfg==NULL))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
  
    for (i = 0; i<4; i++)
    {    
        pkey = (cl_mac_etype_key_t *)(&pdata->u.cl_mac_etype_key[i]);
        pcfg = (cs_aal_cl_mac_etype_key_t *)(&cfg->cl_key.mac_etype_key[i]);

        pcfg->age_timer_offset = pkey->age_timer_offset;
        
        if(pkey->mask ==0)
            continue;
        
        if(pkey->mask&CL_KEY_MASK_MACADDR)
        {                 
            memcpy(&pcfg->mac[0], &pkey->mac[0], 6*sizeof(cs_uint8));
            pcfg->mac_type = pkey->mac_type;
            pcfg->mask.s.mac = 1;
        }
        if(pkey->mask&CL_KEY_MASK_TPID)
        {                 
            pcfg->tpid_enc = pkey->tpid_enc;
            pcfg->mask.s.tpid_enc = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_EV2PT)
        {                 
            pcfg->ev2pt = pkey->ev2pt;
            pcfg->mask.s.ev2pt = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_MACDA_RANGE_FLAG)
        {                 
            pcfg->mac_da_range_flag = pkey->mac_da_range_flag;
            pcfg->mask.s.mac_da_range_flag = 1;
        }
        if (pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
        {
            pcfg->mask.s.mask_all_msk = 1;
        } 
    
    }
     
    return CS_E_OK;
}

static cs_status __cl_set_vlan_key_entry(
 CS_IN  cs_aal_cl_rule_cfg_t *cfg, 
 CS_OUT cl_key_entry_t       *pdata)
{  
    cs_uint32 i;
    cl_vlan_key_t        *pkey = NULL;
    cs_aal_cl_vlan_key_t *pcfg = NULL;
  
           
    if((pdata==NULL)||(cfg==NULL))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
     
    for (i = 0; i<8; i++)
    {  
        pcfg = (cs_aal_cl_vlan_key_t *)(&cfg->cl_key.vlan_key[i]);
        pkey = (cl_vlan_key_t *)(&pdata->u.cl_vlan_key[i]);

        if(pcfg->mask.u32 ==0)
            continue;
        
        if(pcfg->mask.s.vlanid)
        {
            pkey->vlanid = pcfg->vlanid;         
            pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
        }
        if(pcfg->mask.s.tpid_enc)
        {
            pkey->tpid_enc = pcfg->tpid_enc;         
            pkey->mask |= CL_VLAN_KEY_MASK_TPID_ENC;
        }
        if(pcfg->mask.s.ev2pt_enc)
        {
            pkey->ev2pt_enc = pcfg->ev2pt_enc;         
            pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
        }
        if(pcfg->mask.s.rsvd_mac_da_flag)
        {
            pkey->rsvd_mac_da_flag = pcfg->rsvd_mac_da_flag;         
            pkey->mask |= CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG;
        }
        if(pcfg->mask.s.bc_flag)
        {
            pkey->bc_flag = pcfg->bc_flag;         
            pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
        }
        if(pcfg->mask.s.mc_flag)
        {
            pkey->mc_flag = pcfg->mc_flag;         
            pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
        }
        if(pcfg->mask.s.p_tag_flag)
        {
            pkey->p_tag_flag = pcfg->p_tag_flag;         
            pkey->mask |= CL_VLAN_KEY_MASK_P_TAG_FLAG;
        }
        if(pcfg->mask.s.vlan_count)
        {
            pkey->vlan_count = pcfg->vlan_count;         
            pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
        }
        if(pcfg->mask.s.bc_llid)
        {
            pkey->bc_llid = pcfg->bc_llid;         
            pkey->mask |= CL_VLAN_KEY_MASK_BC_LLID;
        }        
        if(pcfg->mask.s.mask_all_msk)
        {         
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        }
    }
    
    return CS_E_OK;
}

static cs_status __cl_get_vlan_key_entry(
 CS_IN  cl_key_entry_t       *pdata, 
 CS_OUT cs_aal_cl_rule_cfg_t *cfg)
{  
    cs_uint32 i;
    cl_vlan_key_t        *pkey = NULL;
    cs_aal_cl_vlan_key_t *pcfg = NULL;
  
           
    if((pdata==NULL)||(cfg==NULL))
    {  
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
     
    for (i = 0; i<8; i++)
    {        
        pkey = (cl_vlan_key_t *)(&pdata->u.cl_vlan_key[i]);
        pcfg = (cs_aal_cl_vlan_key_t *)(&cfg->cl_key.vlan_key[i]);

        if(pkey->mask ==0)
            continue;
        if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
        {                 
            pcfg->vlanid = pkey->vlanid;
            pcfg->mask.s.vlanid = 1;
        }       
        if(pkey->mask&CL_VLAN_KEY_MASK_TPID_ENC)
        {                 
            pcfg->tpid_enc = pkey->tpid_enc;
            pcfg->mask.s.tpid_enc = 1;
        }       
        if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
        {                 
            pcfg->ev2pt_enc = pkey->ev2pt_enc;
            pcfg->mask.s.ev2pt_enc = 1;
        }       
        if(pkey->mask&CL_VLAN_KEY_MASK_RSVD_MACDA_FLAG)
        {                 
            pcfg->rsvd_mac_da_flag = pkey->rsvd_mac_da_flag;
            pcfg->mask.s.rsvd_mac_da_flag = 1;
        }       
        if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
        {                 
            pcfg->bc_flag = pkey->bc_flag;
            pcfg->mask.s.bc_flag = 1;
        }       
        if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
        {                 
            pcfg->mc_flag = pkey->mc_flag;
            pcfg->mask.s.mc_flag = 1;
        }       
        if(pkey->mask&CL_VLAN_KEY_MASK_P_TAG_FLAG)
        {                 
            pcfg->p_tag_flag = pkey->p_tag_flag;
            pcfg->mask.s.p_tag_flag = 1;
        }       
        if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
        {                 
            pcfg->vlan_count = pkey->vlan_count;
            pcfg->mask.s.vlan_count = 1;
        }       
        if(pkey->mask&CL_VLAN_KEY_MASK_BC_LLID)
        {                 
            pcfg->bc_llid = pkey->bc_llid;
            pcfg->mask.s.bc_llid = 1;
        }
        if (pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
        {
            pcfg->mask.s.mask_all_msk = 1;
        } 
    }
    
    return CS_E_OK;
}

static cs_status __cl_set_vlan_range_key_entry(
 CS_IN  cs_aal_cl_rule_cfg_t *cfg, 
 CS_OUT cl_key_entry_t       *pdata)
{  
    cs_uint32 i;
    cl_vlan_range_key_t        *pkey = NULL;
    cs_aal_cl_vlan_range_key_t *pcfg = NULL;
  
           
    if((pdata==NULL)||(cfg==NULL))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
     
    for (i = 0; i<8; i++)
    {  
        pcfg = (cs_aal_cl_vlan_range_key_t *)(&cfg->cl_key.vlan_range_key[i]);
        pkey = (cl_vlan_range_key_t *)(&pdata->u.cl_vlan_range_key[i]);

        if(pcfg->mask.u32 ==0)
            continue;
        
        if(pcfg->mask.s.ev2pt_enc)
        {
            pkey->ev2pt_enc = pcfg->ev2pt_enc;         
            pkey->mask |= CL_VLAN_KEY_MASK_EV2PT_ENC;
        }
        if(pcfg->mask.s.vlan)
        {
            pkey->vlanid_hi = pcfg->vlanid_hi;         
            pkey->vlanid_lo = pcfg->vlanid_lo;         
            pkey->inner_vlan_en = pcfg->inner_vlan_en;         
            pkey->mask |= CL_VLAN_KEY_MASK_VLANID;
        }  
        if(pcfg->mask.s.bc_flag)
        {
            pkey->bc_flag = pcfg->bc_flag;         
            pkey->mask |= CL_VLAN_KEY_MASK_BC_FLAG;
        }
        if(pcfg->mask.s.mc_flag)
        {
            pkey->mc_flag = pcfg->mc_flag;         
            pkey->mask |= CL_VLAN_KEY_MASK_MC_FLAG;
        }
        if(pcfg->mask.s.vlan_count)
        {
            pkey->vlan_count = pcfg->vlan_count;         
            pkey->mask |= CL_VLAN_KEY_MASK_VLAN_COUNT;
        }     
        if(pcfg->mask.s.mask_all_msk)
        {         
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        }
    }
    
    return CS_E_OK;
}

static cs_status __cl_get_vlan_range_key_entry(
 CS_IN  cl_key_entry_t       *pdata, 
 CS_OUT cs_aal_cl_rule_cfg_t *cfg)
{  
    cs_uint32 i;
    cl_vlan_range_key_t        *pkey = NULL;
    cs_aal_cl_vlan_range_key_t *pcfg = NULL;
  
           
    if((pdata==NULL)||(cfg==NULL))
    {  
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
     
    for (i = 0; i<8; i++)
    {        
        pkey = (cl_vlan_range_key_t *)(&pdata->u.cl_vlan_range_key[i]);
        pcfg = (cs_aal_cl_vlan_range_key_t *)(&cfg->cl_key.vlan_range_key[i]);

        if(pkey->mask ==0)
            continue;
        
        if(pkey->mask&CL_VLAN_KEY_MASK_VLANID)
        {                 
            pcfg->vlanid_hi = pkey->vlanid_hi;         
            pcfg->vlanid_lo = pkey->vlanid_lo;         
            pcfg->inner_vlan_en = pkey->inner_vlan_en;         
            pcfg->mask.s.vlan = 1;
        }            
        if(pkey->mask&CL_VLAN_KEY_MASK_EV2PT_ENC)
        {                 
            pcfg->ev2pt_enc = pkey->ev2pt_enc;
            pcfg->mask.s.ev2pt_enc = 1;
        }          
        if(pkey->mask&CL_VLAN_KEY_MASK_BC_FLAG)
        {                 
            pcfg->bc_flag = pkey->bc_flag;
            pcfg->mask.s.bc_flag = 1;
        }       
        if(pkey->mask&CL_VLAN_KEY_MASK_MC_FLAG)
        {                 
            pcfg->mc_flag = pkey->mc_flag;
            pcfg->mask.s.mc_flag = 1;
        }       
        if(pkey->mask&CL_VLAN_KEY_MASK_VLAN_COUNT)
        {                 
            pcfg->vlan_count = pkey->vlan_count;
            pcfg->mask.s.vlan_count = 1;
        } 
        if (pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
        {
            pcfg->mask.s.mask_all_msk = 1;
        } 
    }
    
    return CS_E_OK;
}

static cs_status __cl_set_proto_key_entry(
 CS_IN  cs_aal_cl_rule_cfg_t *cfg, 
 CS_OUT cl_key_entry_t       *pdata)
{
    cs_uint32 i;
    cl_proto_key_t        *pkey = NULL;
    cs_aal_cl_proto_key_t *pcfg = NULL;
            
    if((pdata==NULL)||(cfg==NULL))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    for (i = 0; i<4; i++)
    {  
        pcfg = (cs_aal_cl_proto_key_t *)(&cfg->cl_key.proco_key[i]);
        pkey = (cl_proto_key_t *)(&pdata->u.cl_proto_key[i]);
            
        pkey->cfi_flag = pcfg->cfi_flag;  
        
        if(pcfg->mask.u32 ==0)
            continue;
        
        if(pcfg->mask.s.tpid_enc)
        {
            pkey->tpid_enc = pcfg->tpid_enc;         
            pkey->mask |= CL_KEY_MASK_TPID;
        }
        if(pcfg->mask.s.ev2pt)
        {
            pkey->ev2pt = pcfg->ev2pt;         
            pkey->mask |= CL_KEY_MASK_EV2PT;
        }
        if(pcfg->mask.s.ipprotocol)
        {
            pkey->ipprotocol = pcfg->ipprotocol;         
            pkey->mask |= CL_KEY_MASK_IPPROTO;
        } 
        if(pcfg->mask.s.sp_mac_flag)
        {
            pkey->myMAC_flag = pcfg->myMAC_flag; 
            pkey->rsvd_mac_da_flag = pcfg->rsvd_mac_da_flag; 
            pkey->mcst_ip_flag = pcfg->mcst_ip_flag; 
            pkey->mask |= CL_KEY_MASK_DA_FLAGS;
        }
        if(pcfg->mask.s.mac_da_low)
        {
            pkey->mac_da_low = pcfg->mac_da_low;         
            pkey->mask |= CL_KEY_MASK_MAC_DA_LOW;
        } 
        if(pcfg->mask.s.icmp_type)
        {
            pkey->icmp_type = pcfg->icmp_type;         
            pkey->mask |= CL_KEY_MASK_ICMP_TYPE;
        } 
        if(pcfg->mask.s.prsr_len_excds)
        {
            pkey->prsr_len_excds = pcfg->prsr_len_excds;         
            pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
        } 
        if(pcfg->mask.s.l4_src_hit_map)
        {
            pkey->l4_src_hit_map = pcfg->l4_src_hit_map;         
            pkey->mask |= CL_KEY_MASK_L4SRC_RANGE;
        } 
        if(pcfg->mask.s.l4_dest_hit_map)
        {
            pkey->l4_dest_hit_map = pcfg->l4_dest_hit_map;         
            pkey->mask |= CL_KEY_MASK_L4DST_RANGE;
        } 
        if(pcfg->mask.s.spcl_hit_map)
        {
            pkey->spcl_hit_map = pcfg->spcl_hit_map;         
            pkey->mask |= CL_KEY_MASK_SPECPKT;
        }        
        if(pcfg->mask.s.mask_all_msk)
        {         
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        }
    }
    return CS_E_OK;
}

static cs_status __cl_get_proto_key_entry(
 CS_IN  cl_key_entry_t       *pdata, 
 CS_OUT cs_aal_cl_rule_cfg_t *cfg)
{  
    cs_uint32 i;
    cl_proto_key_t        *pkey = NULL;
    cs_aal_cl_proto_key_t *pcfg = NULL;
         
    if((pdata==NULL)||(cfg==NULL))
    {  
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    for (i = 0; i<4; i++)
    {    
        pkey = (cl_proto_key_t *)(&pdata->u.cl_proto_key[i]);
        pcfg = (cs_aal_cl_proto_key_t *)(&cfg->cl_key.proco_key[i]);

        pcfg->cfi_flag = pkey->cfi_flag;  
        
        if(pkey->mask ==0)
            continue;
        
        if(pkey->mask&CL_KEY_MASK_TPID)
        {                 
            pcfg->tpid_enc = pkey->tpid_enc;
            pcfg->mask.s.tpid_enc = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_EV2PT)
        {                 
            pcfg->ev2pt = pkey->ev2pt;
            pcfg->mask.s.ev2pt = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_IPPROTO)
        {                 
            pcfg->ipprotocol = pkey->ipprotocol;
            pcfg->mask.s.ipprotocol = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_DA_FLAGS)
        {                 
            pcfg->myMAC_flag = pkey->myMAC_flag; 
            pcfg->rsvd_mac_da_flag = pkey->rsvd_mac_da_flag; 
            pcfg->mcst_ip_flag = pkey->mcst_ip_flag; 
            pcfg->mask.s.sp_mac_flag = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_MAC_DA_LOW)
        {                 
            pcfg->mac_da_low = pkey->mac_da_low;
            pcfg->mask.s.mac_da_low = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_ICMP_TYPE)
        {                 
            pcfg->icmp_type = pkey->icmp_type;
            pcfg->mask.s.icmp_type = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
        {                 
            pcfg->prsr_len_excds = pkey->prsr_len_excds;
            pcfg->mask.s.prsr_len_excds = 1;
        }               
        if(pkey->mask&CL_KEY_MASK_L4SRC_RANGE)
        {                 
            pcfg->l4_src_hit_map = pkey->l4_src_hit_map;
            pcfg->mask.s.l4_src_hit_map = 1;
        }   
        if(pkey->mask&CL_KEY_MASK_L4DST_RANGE)
        {                 
            pcfg->l4_dest_hit_map = pkey->l4_dest_hit_map;
            pcfg->mask.s.l4_dest_hit_map = 1;
        }   
        if(pkey->mask&CL_KEY_MASK_SPECPKT)
        {                 
            pcfg->spcl_hit_map = pkey->spcl_hit_map;
            pcfg->mask.s.spcl_hit_map = 1;
        }
        if (pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
        {
            pcfg->mask.s.mask_all_msk = 1;
        } 
    }
    return CS_E_OK;
}

static cs_status __cl_set_mcst_key_entry(
 CS_IN  cs_aal_cl_rule_cfg_t *cfg, 
 CS_OUT cl_key_entry_t       *pdata)
{  
    cs_uint32 i;
    cl_mcst_key_t         *pkey = NULL;
    cs_aal_cl_mcast_key_t *pcfg = NULL;
  
           
    if((pdata==NULL)||(cfg==NULL))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    for (i = 0; i<2; i++)
    {  
        pcfg = (cs_aal_cl_mcast_key_t *)(&cfg->cl_key.mcast_key[i]);
        pkey = (cl_mcst_key_t *)(&pdata->u.cl_mcst_key[i]);

        pkey->group_id = pcfg->group_id;
        pkey->group_id_vld = pcfg->group_id_vld;
        pkey->source_addr_vld = pcfg->source_addr_vld;
        
        if(pcfg->mask.u32 ==0)
            continue;
        
        if(pcfg->mask.s.addr)
        {
            memcpy(&pkey->addr[0], &pcfg->addr[0], 16*sizeof(cs_uint8));   
            pkey->addr_type = pcfg->addr_type;
            pkey->mask |= CL_KEY_MASK_IPADDR;
        }
        if(pcfg->mask.s.vlanId)
        {
            pkey->vlanId = pcfg->vlanId;         
            pkey->mask |= CL_KEY_MASK_VLANID;
        }     
        if(pcfg->mask.s.prsr_len_excds)
        {
            pkey->prsr_len_excds = pcfg->prsr_len_excds;         
            pkey->mask |= CL_KEY_MASK_PRSR_LEN_EXCD;
        }           
        if(pcfg->mask.s.mask_all_msk)
        {         
            pkey->mask |= CL_KEY_MASK_ALWAYS_MATCH;
        }
    }  
    return CS_E_OK;
}

static cs_status __cl_get_mcst_key_entry(
 CS_IN  cl_key_entry_t       *pdata, 
 CS_OUT cs_aal_cl_rule_cfg_t *cfg)
{   
    cs_uint32 i;
    cl_mcst_key_t         *pkey = NULL;
    cs_aal_cl_mcast_key_t *pcfg = NULL;
     
    if((pdata==NULL)||(cfg==NULL))
    {
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
  
    for (i = 0; i<2; i++)
    {    
        pkey = (cl_mcst_key_t *)(&pdata->u.cl_mcst_key[i]);
        pcfg = (cs_aal_cl_mcast_key_t *)(&cfg->cl_key.mcast_key[i]);

        pcfg->group_id = pkey->group_id;
        pcfg->group_id_vld = pkey->group_id_vld;
        pcfg->source_addr_vld = pkey->source_addr_vld;
        
        if(pkey->mask ==0)
            continue;
        
        if(pkey->mask&CL_KEY_MASK_IPADDR)
        {                 
            memcpy(&pcfg->addr[0], &pkey->addr[0], 16*sizeof(cs_uint8));
            pcfg->addr_type = pkey->addr_type;
            pcfg->mask.s.addr = 1;
        }
        if(pkey->mask&CL_KEY_MASK_VLANID)
        {                 
            pcfg->vlanId = pkey->vlanId;
            pcfg->mask.s.vlanId = 1;
        }       
        if(pkey->mask&CL_KEY_MASK_PRSR_LEN_EXCD)
        {                 
            pcfg->prsr_len_excds = pkey->prsr_len_excds;
            pcfg->mask.s.prsr_len_excds = 1;
        }       
        if (pkey->mask&CL_KEY_MASK_ALWAYS_MATCH)
        {
            pcfg->mask.s.mask_all_msk = 1;
        }         
    }
     
    return CS_E_OK;
}


//public
/*=============================================================*/

cs_status aal_cl_domain_set(
    CS_IN cs_aal_port_id_t       port,
    CS_IN cs_uint8               domain_num,
    CS_IN cs_aal_cl_domain_res_t *cfg
)
{   
    cs_uint32 i;
    cs_aal_cl_domain_res_t   *prescfg = NULL;
    cs_aal_fe_instance_t        fe_id = 0;
 
    if((cfg==NULL)||(port>AAL_PORT_ID_MII1)||(domain_num>16))
    {    
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    fe_id = FE_PORT(port);
     
    memset( &g_ce_domain_res[fe_id][0], 0, AAL_CL_DOMAIN_ID_NUM*sizeof(cs_aal_cl_domain_res_t));
     
    for (i = 0; i < domain_num; i++) {    
        prescfg = (cs_aal_cl_domain_res_t *) ((cs_uint8*)cfg + i*sizeof(cs_aal_cl_domain_res_t));
        if(fe_id == AAL_FE_MA_ID)
        {
            if((prescfg->cl_entry_start>15)||((prescfg->cl_entry_start+prescfg->length)>16))
            {
                AAL_INF_LOG("cl_entry_start: %d, length: %d. \n", prescfg->cl_entry_start, prescfg->length);
                return CS_E_PARAM;
            }
        }
        else if(fe_id == AAL_FE_GE_ID)
        {
            if((prescfg->cl_entry_start>46)||((prescfg->cl_entry_start+prescfg->length)>47))
            {
                AAL_INF_LOG("cl_entry_start: %d, length: %d. \n", prescfg->cl_entry_start, prescfg->length);
                return CS_E_PARAM;
            }
        }
        else
        {
            if((prescfg->cl_entry_start>47)||((prescfg->cl_entry_start+prescfg->length)>48))
            {
                AAL_INF_LOG("cl_entry_start: %d, length: %d. \n", prescfg->cl_entry_start, prescfg->length);
                return CS_E_PARAM;
            }
        }   
        g_ce_domain_res[fe_id][i].cl_entry_start = prescfg->cl_entry_start;
        g_ce_domain_res[fe_id][i].length         = prescfg->length;
    }
    
    return CS_E_OK;
}


cs_status aal_cl_domain_get(
    CS_IN  cs_aal_port_id_t       port,
    CS_IN  cs_uint8               domain_id,
    CS_OUT cs_aal_cl_domain_res_t *cfg
)
{    
    cs_aal_fe_instance_t        fe_id = 0;

    if((cfg==NULL)||(port>AAL_PORT_ID_MII1)||(domain_id>15))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    } 
    
    fe_id = FE_PORT(port);
      
    cfg->cl_entry_start = g_ce_domain_res[fe_id][domain_id].cl_entry_start;
    cfg->length         = g_ce_domain_res[fe_id][domain_id].length;

    return CS_E_OK;    
}


cs_status aal_cl_ctrl_set(
    CS_IN cs_aal_port_id_t          port,
    CS_IN cs_aal_cl_ctrl_msk_t      cfg_msk,
    CS_IN cs_aal_cl_ctrl_t          *cfg
)
{  
    FE_LE_ENGINE_CTRL_t lectrl_mask;
    FE_LE_ENGINE_CTRL_t lectrl_data;
    cs_uint32             temp0 = 0;
    cs_uint16             temp1 = 0;
    cs_aal_fe_instance_t  fe_id = 0;

    if((cfg==NULL)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  
    
    if (cfg_msk.u32 == 0)
        return CS_E_OK;
    
    fe_id = FE_PORT(port);
    
    lectrl_mask.wrd = 0;
    lectrl_data.wrd = 0;
    
    if (cfg_msk.s.tos_ecn_msk_en)
    {   
        if (cfg->tos_ecn_msk_en > 1)
            return CS_E_PARAM;
        lectrl_mask.bf.ce_tos_ecn_msk_en = FILL_MSK;
        lectrl_data.bf.ce_tos_ecn_msk_en = cfg->tos_ecn_msk_en;
    }
    if (cfg_msk.s.ce_msk_cfi_flg)
    {   
        if (cfg->ce_msk_cfi_flg > 1)
            return CS_E_PARAM;
        lectrl_mask.bf.ce_msk_cfi_flg = FILL_MSK;
        lectrl_data.bf.ce_msk_cfi_flg = cfg->ce_msk_cfi_flg;
    }
    
    if (lectrl_mask.wrd)
        REG_MASK_WRITE_PORT(FE_LE_ENGINE_CTRL, fe_id, lectrl_mask.wrd, lectrl_data.wrd); 
    
    if (cfg_msk.s.mac_da_range)
    {   
        temp1 = (cfg->mac_da_high.addr[0] << 8) + cfg->mac_da_high.addr[1];
        temp0 = (cfg->mac_da_high.addr[2] << 24) + (cfg->mac_da_high.addr[3] << 16) + (cfg->mac_da_high.addr[4] << 8) + cfg->mac_da_high.addr[5];
        REG_FIELD_WRITE_PORT(FE_LE_PP_MAC_DA_RANGE_HIGH1, fe_id, mac_addr, temp1);
        REG_WRITE_PORT(FE_LE_PP_MAC_DA_RANGE_HIGH0, fe_id, temp0);
        temp1 = (cfg->mac_da_low.addr[0] << 8) + cfg->mac_da_low.addr[1];
        temp0 = (cfg->mac_da_low.addr[2] << 24) + (cfg->mac_da_low.addr[3] << 16) + (cfg->mac_da_low.addr[4] << 8) + cfg->mac_da_low.addr[5];
        REG_FIELD_WRITE_PORT(FE_LE_PP_MAC_DA_RANGE_LOW1, fe_id, mac_addr, temp1);
        REG_WRITE_PORT(FE_LE_PP_MAC_DA_RANGE_LOW0, fe_id, temp0);     
    }
    
    if (cfg_msk.s.mac_filter_aging_en)
    {  
        REG_FIELD_WRITE_PORT(FE_LE_CE_CTRL, fe_id, mac_filter_aging_en, cfg->mac_filter_aging_en);
    }
        
    return CS_E_OK;
}


cs_status aal_cl_ctrl_get(
    CS_IN  cs_aal_port_id_t       port,
    CS_OUT cs_aal_cl_ctrl_t       *cfg
)
{
    FE_LE_ENGINE_CTRL_t   lectrl_data;
    cs_uint32             temp0 = 0;
    cs_uint16             temp1 = 0;
    cs_aal_fe_instance_t  fe_id = 0;

    if((cfg==NULL)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  

    fe_id = FE_PORT(port);
     
    REG_READ_PORT(FE_LE_ENGINE_CTRL, fe_id, lectrl_data.wrd); 
    
    cfg->tos_ecn_msk_en = lectrl_data.bf.ce_tos_ecn_msk_en;
    cfg->ce_msk_cfi_flg = lectrl_data.bf.ce_msk_cfi_flg;

    REG_FIELD_READ_PORT(FE_LE_CE_CTRL, fe_id, mac_filter_aging_en, cfg->mac_filter_aging_en);
    
    REG_FIELD_READ_PORT(FE_LE_PP_MAC_DA_RANGE_HIGH1, fe_id, mac_addr, temp1);
    REG_READ_PORT(FE_LE_PP_MAC_DA_RANGE_HIGH0, fe_id, temp0);
    cfg->mac_da_high.addr[0] = temp1 >> 8;
    cfg->mac_da_high.addr[1] = temp1 & 0xff;
    cfg->mac_da_high.addr[2] = (temp0 >> 24)& 0xff;
    cfg->mac_da_high.addr[3] = (temp0 >> 16)& 0xff;
    cfg->mac_da_high.addr[4] = (temp0 >> 8) & 0xff;
    cfg->mac_da_high.addr[5] = temp0 & 0xff;
    
    REG_FIELD_READ_PORT(FE_LE_PP_MAC_DA_RANGE_LOW1, fe_id, mac_addr, temp1);
    REG_READ_PORT(FE_LE_PP_MAC_DA_RANGE_LOW0, fe_id, temp0);
    cfg->mac_da_low.addr[0] = temp1 >> 8;
    cfg->mac_da_low.addr[1] = temp1 & 0xff;
    cfg->mac_da_low.addr[2] = (temp0 >> 24)& 0xff;
    cfg->mac_da_low.addr[3] = (temp0 >> 16)& 0xff;
    cfg->mac_da_low.addr[4] = (temp0 >> 8) & 0xff;
    cfg->mac_da_low.addr[5] = temp0 & 0xff;
      
    return CS_E_OK;
}


cs_status aal_cl_l4_port_range_set(
    CS_IN cs_aal_port_id_t            port,
    CS_IN cs_aal_cl_l4_port_type_t    port_type,
    CS_IN cs_uint8                    id,
    CS_IN cs_aal_cl_l4_port_range_t   *cfg
)
{
    cl_l4key_cfg_t          l4key_cfg;
    cs_status                ret = 0;
    cs_uint8                type = 0;
    cs_aal_fe_instance_t   fe_id = 0;
    
    if((cfg==NULL)||(port_type>AAL_CL_L4_DST_UDP)||(port>AAL_PORT_ID_MII1)||(id>7))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  
    
    if(cfg->low_port > cfg->high_port )
    {
        AAL_INF_LOG("low_port: %d,  high_port: %d.\n", cfg->low_port, cfg->high_port);
        return CS_E_PARAM;
    }
    
    fe_id = FE_PORT(port);
    
    l4key_cfg.L4PortHigh = cfg->high_port;
    l4key_cfg.L4PortLow  = cfg->low_port;
    l4key_cfg.valid      = cfg->cmp_en;
    
    if(port_type == AAL_CL_L4_SRC_TCP)
    {  
        type = CL_L4_SRC_TCP;
    }
    else if(port_type == AAL_CL_L4_SRC_UDP)
    {  
        type = CL_L4_SRC_UDP;
    }
    else if(port_type == AAL_CL_L4_DST_TCP)
    {  
        type = CL_L4_DST_TCP;
    }
    else if(port_type == AAL_CL_L4_DST_UDP)
    {  
        type = CL_L4_DST_UDP;
    }
    ret = cl_l4_key_set( fe_id, id, type, &l4key_cfg);  
    if(ret)
    {       
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
}


cs_status aal_cl_l4_port_range_get(
    CS_IN  cs_aal_port_id_t               port,
    CS_IN  cs_aal_cl_l4_port_type_t       port_type,
    CS_IN  cs_uint8                       id,
    CS_OUT cs_aal_cl_l4_port_range_t      *cfg
)
{
    cl_l4key_cfg_t        l4key_cfg;
    cs_status               ret = 0;
    cs_uint8               type = 0;
    cs_aal_fe_instance_t  fe_id = 0;
    
    if((cfg==NULL)||(port_type>AAL_CL_L4_DST_UDP)||(port>AAL_PORT_ID_MII1)||(id>7))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  

    fe_id = FE_PORT(port);
     
    if(port_type == AAL_CL_L4_SRC_TCP)
    {  
        type = CL_L4_SRC_TCP;
    }
    else if(port_type == AAL_CL_L4_SRC_UDP)
    {  
        type = CL_L4_SRC_UDP;
    }
    else if(port_type == AAL_CL_L4_DST_TCP)
    {  
        type = CL_L4_DST_TCP;
    }
    else if(port_type == AAL_CL_L4_DST_UDP)
    {  
        type = CL_L4_DST_UDP;
    }
    ret = cl_l4_key_get( fe_id, id, type, &l4key_cfg);  
    if(ret)
    {    
        AAL_INF_LOG("cl_l4_key_get error. \n");
        return CS_E_ERROR;
    }
    cfg->high_port = l4key_cfg.L4PortHigh;
    cfg->low_port  = l4key_cfg.L4PortLow;
    cfg->cmp_en    = l4key_cfg.valid;
   
    return CS_E_OK;
}


cs_status aal_cl_spcl_ptn_ctrl_set(
    CS_IN  cs_aal_port_id_t                   port,
    CS_IN  cs_aal_cl_spcl_ptn_control_msk_t   cfg_msk,
    CS_IN  cs_aal_cl_spcl_ptn_control_t       *cfg
)
{    
    FE_LE_PP_PARSER_CTRL_t msk;
    FE_LE_PP_PARSER_CTRL_t data;
    cs_aal_fe_instance_t   fe_id = 0;

    if((cfg==NULL)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  
    
    if (cfg_msk.u32 == 0)
        return CS_E_OK;

    data.wrd = 0;
    msk.wrd  = 0;
    
    fe_id = FE_PORT(port);
    
    if(cfg_msk.s.mode_sel)
    {
        msk.bf.spcl_hdr_mode_sel  = FILL_MSK;
        data.bf.spcl_hdr_mode_sel = cfg->spcl_hdr_mode_sel;
    }
    if(cfg_msk.s.hdr_sel)
    {
        msk.bf.spcl_hdr_after_l2hdr_en   = FILL_MSK;
        msk.bf.spcl_hdr_after_iphdr_en   = FILL_MSK;
        msk.bf.spcl_hdr_after_tcphdr_en  = FILL_MSK;
        data.bf.spcl_hdr_after_l2hdr_en  = cfg->spcl_hdr_after_l2hdr_en;
        data.bf.spcl_hdr_after_iphdr_en  = cfg->spcl_hdr_after_iphdr_en;
        data.bf.spcl_hdr_after_tcphdr_en = cfg->spcl_hdr_after_l4hdr_en;
    }
    if(cfg_msk.s.parse_ctrl)
    {
        msk.bf.spcl_hdr_parsing_en  = FILL_MSK;
        data.bf.spcl_hdr_parsing_en = cfg->spcl_hdr_parsing_en;
    }
    if(cfg_msk.s.glb_strt)
    {
        msk.bf.spcl_glb_hdr_strt_idx  = FILL_MSK;
        data.bf.spcl_glb_hdr_strt_idx = cfg->spcl_glb_hdr_strt_idx;
    }
    
    REG_MASK_WRITE_PORT(FE_LE_PP_PARSER_CTRL, fe_id, msk.wrd, data.wrd); 
    
    return CS_E_OK;
}


cs_status aal_cl_spcl_ptn_ctrl_get(
    CS_IN  cs_aal_port_id_t                 port,
    CS_OUT cs_aal_cl_spcl_ptn_control_t     *cfg
)
{
    FE_LE_PP_PARSER_CTRL_t data;
    cs_aal_fe_instance_t   fe_id = 0;

    if((cfg==NULL)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  

    fe_id = FE_PORT(port);
 
    REG_READ_PORT(FE_LE_PP_PARSER_CTRL, fe_id, data.wrd); 
        
    cfg->spcl_hdr_mode_sel       = data.bf.spcl_hdr_mode_sel;
    cfg->spcl_hdr_after_l2hdr_en = data.bf.spcl_hdr_after_l2hdr_en;
    cfg->spcl_hdr_after_iphdr_en = data.bf.spcl_hdr_after_iphdr_en;
    cfg->spcl_hdr_after_l4hdr_en = data.bf.spcl_hdr_after_tcphdr_en;
    cfg->spcl_glb_hdr_strt_idx   = data.bf.spcl_glb_hdr_strt_idx;
    cfg->spcl_hdr_parsing_en     = data.bf.spcl_hdr_parsing_en;
        
    return CS_E_OK;
}


cs_status aal_cl_spcl_ptn_entry_set(
    CS_IN  cs_aal_port_id_t           port,
    CS_IN  cs_uint8                   id,
    CS_IN  cs_aal_cl_spcl_ptn_t       *cfg
)
{
    cl_spec_key_cfg_t    speckey_cfg;
    cs_status                ret = 0;
    cs_aal_fe_instance_t   fe_id = 0;
    
    if((cfg==NULL)||(port>AAL_PORT_ID_MII1)||(id>7))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    fe_id = FE_PORT(port);
    
    speckey_cfg.spec_ms_vld   = cfg->spcl_ptn_cmp;
    speckey_cfg.spec_hdr_type = cfg->spcl_ptn_type;
    speckey_cfg.spec_key      = cfg->spcl_ptn_key;
    speckey_cfg.spec_msk      = cfg->spcl_ptn_mask;

    ret = cl_spec_key_set( fe_id, id, &speckey_cfg);  
    
    return ret;
}


cs_status aal_cl_spcl_ptn_entry_get(
    CS_IN  cs_aal_port_id_t           port,
    CS_IN  cs_uint8                   id,
    CS_OUT cs_aal_cl_spcl_ptn_t       *cfg
)
{  
    cl_spec_key_cfg_t  speckey_cfg;
    cs_status ret =0;
    cs_aal_fe_instance_t   fe_id = 0;
    
    if((cfg==NULL)||(port>AAL_PORT_ID_MII1)||(id>7))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  

    fe_id = FE_PORT(port);
    
    ret = cl_spec_key_get( fe_id, id, &speckey_cfg);  
    if(ret)
    {      
        AAL_INF_LOG("cl_spec_key_get error. \n");
        return CS_E_ERROR;
    }
    cfg->spcl_ptn_cmp  = speckey_cfg.spec_ms_vld;
    cfg->spcl_ptn_type = speckey_cfg.spec_hdr_type;
    cfg->spcl_ptn_key  = speckey_cfg.spec_key;
    cfg->spcl_ptn_mask = speckey_cfg.spec_msk;
    
    return CS_E_OK;
}


cs_status aal_cl_aging_timer_set(
    CS_IN cs_aal_port_id_t           port,
    CS_IN cs_uint8                   id,
    CS_IN cs_aal_cl_mac_aging_t      *cfg
)
{
    cl_mac_age_cfg_t      macage_cfg;
    cs_status                ret = 0;
    cs_aal_fe_instance_t   fe_id = 0;

    if((cfg==NULL)||(port>AAL_PORT_ID_MII1)||(id>CL_MAC_AGING_MAXNUM))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  

    fe_id = FE_PORT(port);
    
    macage_cfg.valid = cfg->valid;
    macage_cfg.time  = cfg->aging_time;
    
    ret = cl_mac_filter_aging_time_set( fe_id, id, &macage_cfg);  

    return ret;
}


cs_status aal_cl_aging_timer_get(
    CS_IN  cs_aal_port_id_t           port,
    CS_IN  cs_uint8                   id,
    CS_OUT cs_aal_cl_mac_aging_t      *cfg
)
{
    cl_mac_age_cfg_t      macage_cfg;
    cs_status                ret = 0;
    cs_aal_fe_instance_t   fe_id = 0;

    if((cfg==NULL)||(port>AAL_PORT_ID_MII1)||(id>CL_MAC_AGING_MAXNUM))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }  

    fe_id = FE_PORT(port);
     
    ret = cl_mac_filter_aging_time_get( fe_id, id, &macage_cfg);  
    if(ret)
    {       
        AAL_INF_LOG("cl_mac_filter_aging_time_get error. \n");
        return CS_E_ERROR;
    }
    cfg->valid = macage_cfg.valid;
    cfg->aging_time = macage_cfg.time;
    
    return CS_E_OK;
}


cs_status aal_cl_rule_set(
    CS_IN cs_aal_port_id_t      port,
    CS_IN cs_uint8              domain_id,
    CS_IN cs_uint8              cfg_id,
    CS_IN cs_aal_cl_rule_cfg_t  *cfg
)
{
    cl_key_entry_t         ce_entry;
    cs_aal_cl_domain_res_t *prescfg    = NULL;
    cs_uint16              entry_index = 0;
    cs_status              ret         = 0;
    cs_aal_fe_instance_t   fe_id       = 0;
     
    if((cfg==NULL)||(domain_id>15)||(port>AAL_PORT_ID_MII1))
    {  
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    fe_id = FE_PORT(port);
       
    prescfg = &g_ce_domain_res[fe_id][domain_id];
    if(prescfg->length)
    {
        if(cfg_id>(prescfg->length-1))
        {
            AAL_MIN_LOG("cfg_id: %d, length: %d.\n", cfg_id, prescfg->length);
            return CS_E_PARAM;
        }
        entry_index = (cfg_id+prescfg->cl_entry_start);
    }
    else
    {    
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(cfg->key_type >=AAL_CL_MAX_KEY)
    {     
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    memset( &ce_entry, 0, sizeof(cl_key_entry_t));
    ce_entry.keytype    = cfg->key_type;
    ce_entry.entryvalid = cfg->valid;

    switch (cfg->key_type)
    {
        case AAL_CL_IPV4_LONG_KEY:
        {  
            ret = __cl_set_ipv4_long_key_entry(cfg, &ce_entry);
            if(cfg->cl_key.ipv4_long_key.mask.s.rule_mode)
            {
                ret += cl_rule_mode_set( fe_id, entry_index, cfg->cl_key.ipv4_long_key.rule_mode);
            }
            break;
        }
        case AAL_CL_IPV4_SHORT_KEY:
        {  
            ret = __cl_set_ipv4_short_key_entry(cfg, &ce_entry);
            break;
        }
        case AAL_CL_IPV6_LONG_KEY:
        {  
            if((cfg_id+1)>(prescfg->length-1))
                    return CS_E_PARAM;
            ret = __cl_set_ipv6_long_key_entry(cfg, &ce_entry);
            if(cfg->cl_key.ipv6_long_key.mask.s.rule_mode)
            {
                ret += cl_rule_mode_set( fe_id, entry_index, cfg->cl_key.ipv6_long_key.rule_mode);
                ret += cl_rule_mode_set( fe_id, (entry_index+1), cfg->cl_key.ipv6_long_key.rule_mode);
            }
            break;
        }
        case AAL_CL_IPV6_SHORT_KEY:
        {  
            ret = __cl_set_ipv6_short_key_entry(cfg, &ce_entry);
            break;
        }
        case AAL_CL_MIRROR_KEY:
        {  
            ret = __cl_set_mirror_key_entry(cfg, &ce_entry);
            break;
        }
        case AAL_CL_MAC_VLAN_KEY:
        case AAL_CL_MAC_VLAN_KEY_C:
        {  
            ret = __cl_set_mac_vlan_key_entry(cfg, &ce_entry);
            break;
        }
        case AAL_CL_MAC_ETYPE_KEY:
        case AAL_CL_MAC_ETYPE_KEY_D:   
        {  
            ret = __cl_set_mac_etype_key_entry(cfg, &ce_entry);
            break;
        }
        case AAL_CL_CTRL_PKT_KEY:
        {  
            ret = __cl_set_ctrl_pkt_key_entry(cfg, &ce_entry);
            break;
        }
        case AAL_CL_VLAN_KEY:
        case AAL_CL_VLAN_KEY_E:   
        {  
            ret = __cl_set_vlan_key_entry(cfg, &ce_entry);
            break;
        }
        case AAL_CL_VLAN_RANGE_KEY:
        {  
            ret = __cl_set_vlan_range_key_entry(cfg, &ce_entry);
            break;
        }
        case AAL_CL_PROTO_KEY:
        {  
            ret = __cl_set_proto_key_entry(cfg, &ce_entry);
            break;
        }
        case AAL_CL_MCST_KEY:
        {  
            ret = __cl_set_mcst_key_entry(cfg, &ce_entry);
            break;
        }
        default:
            break;
    }
    if (ret)
    {   
        AAL_MIN_LOG("cl set key entry error.\n");
        return CS_E_ERROR;
    }    
    
    ret = cl_key_entry_set( fe_id, entry_index, &ce_entry);
   
    return ret;
}


cs_status aal_cl_rule_get(
    CS_IN  cs_aal_port_id_t       port,
    CS_IN  cs_uint8               domain_id,
    CS_IN  cs_uint32              cfg_id,
    CS_OUT cs_aal_cl_rule_cfg_t   *cfg
)
{
    cl_key_entry_t           ce_entry;
    cs_aal_cl_domain_res_t * prescfg = NULL;
    cs_uint16            entry_index = 0;
    cs_status                    ret = 0;
    cs_boolean                status = 0;
    cs_aal_fe_instance_t      fe_id  = 0;
     
    if((cfg==NULL)||(domain_id>15)||(port>AAL_PORT_ID_MII1))
    {  
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    fe_id = FE_PORT(port);
    
    prescfg = &g_ce_domain_res[fe_id][domain_id];
    if(prescfg->length)
    {
        if(cfg_id>(prescfg->length-1))
            return CS_E_PARAM;
        entry_index = (cfg_id+prescfg->cl_entry_start);
    }
    else
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
 
    memset( &ce_entry, 0, sizeof(cl_key_entry_t));
    
    ret = cl_key_entry_get(fe_id, entry_index, &ce_entry);
    ret += cl_rule_mode_get(fe_id, entry_index, &status);
    if(ret)
    {   
        AAL_INF_LOG("cl key rule get error.\n");
        return CS_E_ERROR;
    } 
    
    cfg->key_type = ce_entry.keytype;
    cfg->valid = ce_entry.entryvalid;
   
    switch (ce_entry.keytype)
    {
        case AAL_CL_IPV4_LONG_KEY:
        {  
            ret = __cl_get_ipv4_long_key_entry(&ce_entry, cfg);
            cfg->cl_key.ipv4_long_key.rule_mode = status;
            break;
        }
        case AAL_CL_IPV4_SHORT_KEY:
        {  
            ret = __cl_get_ipv4_short_key_entry(&ce_entry, cfg);
            break;
        }
        case AAL_CL_IPV6_LONG_KEY:
        {  
            ret = __cl_get_ipv6_long_key_entry(&ce_entry, cfg);
            cfg->cl_key.ipv6_long_key.rule_mode = status;
            break;
        }
        case AAL_CL_IPV6_SHORT_KEY:
        {  
            ret = __cl_get_ipv6_short_key_entry(&ce_entry, cfg);
            break;
        }
        case AAL_CL_MIRROR_KEY:
        {  
            ret = __cl_get_mirror_key_entry(&ce_entry, cfg);
            break;
        }
        case AAL_CL_MAC_VLAN_KEY:
        case AAL_CL_MAC_VLAN_KEY_C:
        {  
            ret = __cl_get_mac_vlan_key_entry(&ce_entry, cfg);
            break;
        }
        case AAL_CL_MAC_ETYPE_KEY:
        case AAL_CL_MAC_ETYPE_KEY_D:   
        {  
            ret = __cl_get_mac_etype_key_entry(&ce_entry, cfg);
            break;
        }
        case AAL_CL_CTRL_PKT_KEY:
        {  
            ret = __cl_get_ctrl_pkt_key_entry(&ce_entry, cfg);
            break;
        }
        case AAL_CL_VLAN_KEY:
        case AAL_CL_VLAN_KEY_E:   
        {  
            ret = __cl_get_vlan_key_entry(&ce_entry, cfg);
            break;
        }
        case AAL_CL_VLAN_RANGE_KEY:
        {  
            ret = __cl_get_vlan_range_key_entry(&ce_entry, cfg);
            break;
        }
        case AAL_CL_PROTO_KEY:
        {  
            ret = __cl_get_proto_key_entry(&ce_entry, cfg);
            break;
        }
        case AAL_CL_MCST_KEY:
        {  
            ret = __cl_get_mcst_key_entry(&ce_entry, cfg);
            break;
        }
        default:
            break;
    }
   
    return ret;
}


cs_status aal_cl_fib_set(
    CS_IN cs_aal_port_id_t      port,
    CS_IN cs_uint8              domain_id,
    CS_IN cs_uint16             fib_id,
    CS_IN cs_aal_cl_fib_data_t  *cfg
)
{
    cl_fib_data_t           fibdata;
    cs_aal_cl_domain_res_t  *prescfg = NULL;
    cs_uint16               fibindex = 0;
    cs_status                    ret = 0;
    cs_aal_fe_instance_t      fe_id  = 0;

    if((cfg==NULL)||(domain_id>15)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    fe_id = FE_PORT(port);
     
    prescfg = &g_ce_domain_res[fe_id][domain_id];
    if(prescfg->length)
    {
        if(fib_id>(CL_SUB_KEY_NUM*prescfg->length-1))
        {
            AAL_MIN_LOG("fib_id: %d, length: %d.\n", fib_id, prescfg->length);
            return CS_E_PARAM;
        }
        fibindex = (fib_id+CL_SUB_KEY_NUM*prescfg->cl_entry_start);
    }
    else
    {    
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    memset( &fibdata, 0, sizeof(cl_fib_data_t));
 
    fibdata.valid             = cfg->valid;
    fibdata.permit_filter_dis = cfg->permit_filter_dis;
    fibdata.permit            = cfg->permit;
    fibdata.permit_pri        = cfg->permit_pri;

    if(cfg->mask.s.dpid)
    {
        CL_PORT_TO_DPID(cfg->dpid.dst_op, cfg->dpid.dpid, fibdata.dpid);
        fibdata.cl_mirror_en      = cfg->cl_mirror_en;
        fibdata.cl_mirror_src_sel = cfg->cl_mirror_src_sel;
        fibdata.mask |= CL_RSLT_MASK_DPID;
    }
    if(cfg->mask.s.dscp)
    {
        if(cfg->dscp_sel > AAL_CL_DSCP_SEL_MAP_DOT1P )
            return CS_E_ERROR;
        fibdata.dscp_sel = cfg->dscp_sel;
        if(cfg->dscp > 63 )
            return CS_E_ERROR;
        fibdata.dscp = cfg->dscp;
        fibdata.mask |= CL_RSLT_MASK_DSCP;
    }
    if(cfg->mask.s.vlan_op)
    {
        if(cfg->inner_vlan_cmd > AAL_CL_VLAN_CMD_SWAP )
            return CS_E_ERROR;
        fibdata.inner_vlan_cmd = cfg->inner_vlan_cmd;
        if(cfg->inner_tpid_sel > AAL_CL_TPID_SEL_RX_OUTER_TPID )
            return CS_E_ERROR;
        fibdata.inner_tpid_sel = cfg->inner_tpid_sel;
        if(cfg->inner_vlanid > 4095 )
            return CS_E_ERROR;
        fibdata.inner_vlanid = cfg->inner_vlanid;
        if(cfg->top_vlanid_sel > AAL_CL_VLANID_SEL_CL)
            return CS_E_ERROR;
        fibdata.top_vlanid_sel = cfg->top_vlanid_sel;
        if(cfg->top_vlan_cmd > AAL_CL_VLAN_CMD_SWAP)
            return CS_E_ERROR;
        fibdata.top_vlan_cmd = cfg->top_vlan_cmd;
        if(cfg->top_tpid_sel > AAL_CL_TPID_SEL_RX_OUTER_TPID)
            return CS_E_ERROR;
        fibdata.top_tpid_sel = cfg->top_tpid_sel;
        if(cfg->top_vlanid > 4095 )
            return CS_E_ERROR;
        fibdata.top_vlanid = cfg->top_vlanid;       
        if(cfg->eg_chk_vlan_sel > AAL_CL_EG_VLAN_CHECK_NO )
            return CS_E_ERROR;
        fibdata.eg_chk_vlan_sel = cfg->eg_chk_vlan_sel;
        fibdata.mask |= CL_RSLT_MASK_VLAN;
    }
    if(cfg->mask.s.dot1p)
    {
        if(cfg->inner_8021p_sel > AAL_CL_DOT1P_SEL_MAP_DST_PORT_DEF )
            return CS_E_ERROR;
        fibdata.inner_8021p_sel = cfg->inner_8021p_sel;
        if(cfg->inner_8021p > 7 )
            return CS_E_ERROR;
        fibdata.inner_8021p = cfg->inner_8021p;
        if(cfg->top_8021p_sel > AAL_CL_DOT1P_SEL_MAP_DST_PORT_DEF )
            return CS_E_ERROR;
        fibdata.top_8021p_sel = cfg->top_8021p_sel;
        if(cfg->top_8021p > 7 )
            return CS_E_ERROR;
        fibdata.top_8021p = cfg->top_8021p;
        fibdata.mask |= CL_RSLT_MASK_8021P;
    }
    if(cfg->mask.s.cos)
    {
       if(cfg->cos_sel > AAL_CL_COS_SEL_MAP_RX_DSCP )
            return CS_E_ERROR;
        fibdata.cos_sel = cfg->cos_sel;      
        if(cfg->cos > 7 )
            return CS_E_ERROR;
        fibdata.cos = cfg->cos;
        fibdata.mask |= CL_RSLT_MASK_COS;
    }
    if(cfg->mask.s.l2_lrn)
    {
        fibdata.learn_dis       = cfg->learn_dis;
        fibdata.l2_limit_permit = cfg->l2_limit_permit;
        fibdata.mask |= CL_RSLT_MASK_L2_LEARN;
    }
    if(cfg->mask.s.rate_limit)
    {
        if(cfg->flowId_sel > AAL_CL_FLOWID_SEL_TX_COS )
            return CS_E_ERROR;
        fibdata.flowId_sel          = cfg->flowId_sel;
        fibdata.flowID              = cfg->flowid;
        fibdata.rate_limiter_bypass = cfg->cl_rate_limiter_bypass;
        fibdata.mask |= CL_RSLT_MASK_RATE_LIMITER;
    }
    ret = cl_fib_table_set(fe_id, fibindex, &fibdata);

    return ret;
}


cs_status aal_cl_fib_get(
    CS_IN  cs_aal_port_id_t            port,
    CS_IN  cs_uint8                    domain_id,
    CS_IN  cs_uint16                   fib_id,
    CS_OUT cs_aal_cl_fib_data_t        *cfg
)
{
    cl_fib_data_t            fibdata;
    cs_aal_cl_domain_res_t   *prescfg = NULL;
    cs_uint16                fibindex = 0;
    cs_status                     ret = 0;
    cs_aal_fe_instance_t       fe_id  = 0;

    if((cfg==NULL)||(domain_id>15)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    fe_id = FE_PORT(port);
     
    prescfg = &g_ce_domain_res[fe_id][domain_id];
    if(prescfg->length)
    {
        if(fib_id>(CL_SUB_KEY_NUM*prescfg->length-1))
            return CS_E_PARAM;
        fibindex = (fib_id+CL_SUB_KEY_NUM*prescfg->cl_entry_start);
    }
    else
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    memset( &fibdata, 0, sizeof(cl_fib_data_t));
     
    ret = cl_fib_table_get(fe_id, fibindex, &fibdata);
    if (ret)
    {    
        AAL_INF_LOG("cl_fib_table_get error. \n");
        return CS_E_ERROR;
    }
    
    cfg->valid             = fibdata.valid;
    cfg->permit_filter_dis = fibdata.permit_filter_dis;
    cfg->permit            = fibdata.permit;
    cfg->permit_pri        = fibdata.permit_pri;
    
    if(fibdata.mask&CL_RSLT_MASK_DPID)
    {   
        CL_DPID_TO_PORT(fibdata.dpid, cfg->dpid.dst_op, cfg->dpid.dpid);
        cfg->cl_mirror_en      = fibdata.cl_mirror_en;
        cfg->cl_mirror_src_sel = fibdata.cl_mirror_src_sel;
        cfg->mask.s.dpid = 1;
    }
    if(fibdata.mask&CL_RSLT_MASK_DSCP)
    {
        cfg->dscp_sel = fibdata.dscp_sel;      
        cfg->dscp     = fibdata.dscp;
        cfg->mask.s.dscp = 1;
    }
    if(fibdata.mask&CL_RSLT_MASK_VLAN)
    {
        cfg->inner_vlan_cmd  = fibdata.inner_vlan_cmd;
        cfg->inner_tpid_sel  = fibdata.inner_tpid_sel;
        cfg->inner_vlanid    = fibdata.inner_vlanid;
        cfg->top_vlanid_sel  = fibdata.top_vlanid_sel;
        cfg->top_vlan_cmd    = fibdata.top_vlan_cmd;
        cfg->top_tpid_sel    = fibdata.top_tpid_sel;
        cfg->top_vlanid      = fibdata.top_vlanid;       
        cfg->eg_chk_vlan_sel = fibdata.eg_chk_vlan_sel;
        cfg->mask.s.vlan_op = 1;
    }
    if(fibdata.mask&CL_RSLT_MASK_8021P)
    {     
        cfg->inner_8021p_sel = fibdata.inner_8021p_sel;   
        cfg->inner_8021p     = fibdata.inner_8021p;
        cfg->top_8021p_sel   = fibdata.top_8021p_sel;
        cfg->top_8021p       = fibdata.top_8021p;
        cfg->mask.s.dot1p = 1;
    }
    if(fibdata.mask&CL_RSLT_MASK_COS)
    {   
        cfg->cos_sel = fibdata.cos_sel;        
        cfg->cos     = fibdata.cos;
        cfg->mask.s.cos = 1;
    }
    if(fibdata.mask&CL_RSLT_MASK_L2_LEARN)
    {
        cfg->learn_dis       = fibdata.learn_dis;
        cfg->l2_limit_permit = fibdata.l2_limit_permit;
        cfg->mask.s.l2_lrn = 1;
    }
    if(fibdata.mask&CL_RSLT_MASK_RATE_LIMITER)
    {  
        cfg->flowId_sel = fibdata.flowId_sel;
        cfg->flowid     = fibdata.flowID;
        cfg->cl_rate_limiter_bypass = fibdata.rate_limiter_bypass;
        cfg->mask.s.rate_limit = 1;
    }
    return CS_E_OK;
}


cs_status aal_cl_domain_rule_clear(
    CS_IN cs_aal_port_id_t    port,
    CS_IN cs_uint8            domain_id
)
{
    cl_key_entry_t  ce_entry;
    cl_fib_data_t   fibdata;
    cs_aal_cl_domain_res_t * prescfg = NULL;
    int i;
    cs_status                    ret = 0;
    cs_aal_fe_instance_t       fe_id = 0;

         
    if((domain_id>15)||(port>AAL_PORT_ID_MII1))
    {    
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
    
    prescfg = &g_ce_domain_res[fe_id][domain_id];
    if(0 == prescfg->length)
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    memset( &ce_entry, 0, sizeof(cl_key_entry_t));
    memset( &fibdata, 0, sizeof(cl_fib_data_t));

    for (i = prescfg->cl_entry_start; i < (prescfg->cl_entry_start+prescfg->length); i++) 
    {  
        ret = cl_key_entry_set( fe_id, i, &ce_entry);
        if(ret)
        {   
            AAL_INF_LOG("cl_key_entry_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        } 
    }
    for (i = CL_SUB_KEY_NUM*prescfg->cl_entry_start; i < CL_SUB_KEY_NUM*(prescfg->cl_entry_start+prescfg->length); i++) 
    {  
        ret = cl_fib_table_set(fe_id, i, &fibdata);
        if (ret)
        {   
            AAL_INF_LOG("cl_fib_table_set error. [%s %d]\n",  __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
    }
     
    return CS_E_OK;
}


cs_status aal_cl_default_fib_set(
    CS_IN cs_aal_port_id_t          port,
    CS_IN cs_aal_cl_def_fib_data_t  *cfg
)
{
    cl_fib_data_t            fibdata;
    cs_status                ret = 0;
    cs_aal_fe_instance_t   fe_id = 0;

    if((cfg==NULL)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_MAJ_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    if (cfg->mask.u32 == 0)
        return CS_E_OK;
    
    fe_id = FE_PORT(port);
     
    memset(&fibdata, 0, sizeof(cl_fib_data_t));

    if(cfg->mask.s.permit)
    {
        fibdata.permit = cfg->permit;
        fibdata.mask |= CL_RSLT_MASK_PERMIT;
    }
    if(cfg->mask.s.dpid)
    {
        CL_PORT_TO_DPID(cfg->dpid.dst_op, cfg->dpid.dpid, fibdata.dpid);
        fibdata.cl_mirror_en      = cfg->cl_mirror_en;
        fibdata.cl_mirror_src_sel = cfg->cl_mirror_src_sel;
        fibdata.mask |= CL_RSLT_MASK_DPID;
    }
    if(cfg->mask.s.dscp)
    {
        if(cfg->dscp_sel > AAL_CL_DSCP_SEL_MAP_DOT1P )
            return CS_E_PARAM;
        fibdata.dscp_sel = cfg->dscp_sel;
        if(cfg->dscp > 63 )
            return CS_E_PARAM;
        fibdata.dscp = cfg->dscp;
        fibdata.mask |= CL_RSLT_MASK_DSCP;
    }
    if(cfg->mask.s.vlan_op)
    {
        if(cfg->inner_vlan_cmd > AAL_CL_VLAN_CMD_SWAP )
            return CS_E_PARAM;
        fibdata.inner_vlan_cmd = cfg->inner_vlan_cmd;
        if(cfg->inner_tpid_sel > AAL_CL_TPID_SEL_RX_OUTER_TPID )
            return CS_E_PARAM;
        fibdata.inner_tpid_sel = cfg->inner_tpid_sel;
        if(cfg->inner_vlanid > 4095 )
            return CS_E_PARAM;
        fibdata.inner_vlanid = cfg->inner_vlanid;
        if(cfg->top_vlanid_sel > AAL_CL_VLANID_SEL_CL)
            return CS_E_PARAM;
        fibdata.top_vlanid_sel = cfg->top_vlanid_sel;
        if(cfg->top_vlan_cmd > AAL_CL_VLAN_CMD_SWAP)
            return CS_E_PARAM;
        fibdata.top_vlan_cmd = cfg->top_vlan_cmd;
        if(cfg->top_tpid_sel > AAL_CL_TPID_SEL_RX_OUTER_TPID)
            return CS_E_PARAM;
        fibdata.top_tpid_sel = cfg->top_tpid_sel;
        if(cfg->top_vlanid > 4095 )
            return CS_E_PARAM;
        fibdata.top_vlanid = cfg->top_vlanid;       
        if(cfg->eg_chk_vlan_sel > AAL_CL_EG_VLAN_CHECK_NO )
            return CS_E_PARAM;
        fibdata.eg_chk_vlan_sel = cfg->eg_chk_vlan_sel;
        fibdata.mask |= CL_RSLT_MASK_VLAN;
    }
    if(cfg->mask.s.dot1p)
    {
        if(cfg->inner_8021p_sel > AAL_CL_DOT1P_SEL_MAP_DST_PORT_DEF )
            return CS_E_PARAM;
        fibdata.inner_8021p_sel = cfg->inner_8021p_sel;
        if(cfg->inner_8021p > 7 )
            return CS_E_PARAM;
        fibdata.inner_8021p = cfg->inner_8021p;
        if(cfg->top_8021p_sel > AAL_CL_DOT1P_SEL_MAP_DST_PORT_DEF )
            return CS_E_PARAM;
        fibdata.top_8021p_sel = cfg->top_8021p_sel;
        if(cfg->top_8021p > 7 )
            return CS_E_PARAM;
        fibdata.top_8021p = cfg->top_8021p;
        fibdata.mask |= CL_RSLT_MASK_8021P;
    }
    if(cfg->mask.s.cos)
    {
       if(cfg->cos_sel > AAL_CL_COS_SEL_MAP_RX_DSCP )
            return CS_E_PARAM;
        fibdata.cos_sel = cfg->cos_sel;      
        if(cfg->cos > 7 )
            return CS_E_PARAM;
        fibdata.cos = cfg->cos;
        fibdata.mask |= CL_RSLT_MASK_COS;
    }
    if(cfg->mask.s.l2_lrn)
    {
        fibdata.learn_dis       = cfg->learn_dis;
        fibdata.l2_limit_permit = cfg->l2_limit_permit;
        fibdata.mask |= CL_RSLT_MASK_L2_LEARN;
    }
    if(cfg->mask.s.rate_limit)
    {
        if(cfg->flowId_sel > AAL_CL_FLOWID_SEL_TX_COS )
            return CS_E_PARAM;
        fibdata.flowId_sel     = cfg->flowId_sel;
        fibdata.flowID         = cfg->flowid;
        fibdata.rate_limiter_bypass = cfg->cl_rate_limiter_bypass;
        fibdata.mask |= CL_RSLT_MASK_RATE_LIMITER;
    }
    ret = cl_default_rule_set(fe_id, &fibdata);
    
    return ret;
}


cs_status aal_cl_default_fib_get(
    CS_IN  cs_aal_port_id_t           port,
    CS_OUT cs_aal_cl_def_fib_data_t   *cfg
)
{
    cl_fib_data_t            fibdata;
    cs_status                ret = 0;
    cs_aal_fe_instance_t   fe_id = 0;

    if((cfg==NULL)||(port>AAL_PORT_ID_MII1))
    {  
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
     
    memset( &fibdata, 0, sizeof(cl_fib_data_t));
     
    ret = cl_default_rule_get(fe_id, &fibdata);
    if (ret)
    {  
        AAL_INF_LOG("cl_default_rule_get error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_ERROR;
    }
    
    cfg->permit = fibdata.permit;
    
    CL_DPID_TO_PORT(fibdata.dpid, cfg->dpid.dst_op, cfg->dpid.dpid);
    cfg->cl_mirror_en           = fibdata.cl_mirror_en;
    cfg->cl_mirror_src_sel      = fibdata.cl_mirror_src_sel;
    cfg->dscp_sel               = fibdata.dscp_sel; 
    cfg->dscp                   = fibdata.dscp;
    cfg->inner_vlan_cmd         = fibdata.inner_vlan_cmd;
    cfg->inner_tpid_sel         = fibdata.inner_tpid_sel;
    cfg->inner_vlanid           = fibdata.inner_vlanid;
    cfg->top_vlanid_sel         = fibdata.top_vlanid_sel;
    cfg->top_vlan_cmd           = fibdata.top_vlan_cmd;
    cfg->top_tpid_sel           = fibdata.top_tpid_sel;
    cfg->top_vlanid             = fibdata.top_vlanid;
    cfg->eg_chk_vlan_sel        = fibdata.eg_chk_vlan_sel;  
    cfg->inner_8021p_sel        = fibdata.inner_8021p_sel;
    cfg->inner_8021p            = fibdata.inner_8021p;
    cfg->top_8021p_sel          = fibdata.top_8021p_sel;
    cfg->top_8021p              = fibdata.top_8021p;
    cfg->cos_sel                = fibdata.cos_sel;
    cfg->cos                    = fibdata.cos; 
    cfg->learn_dis              = fibdata.learn_dis;
    cfg->l2_limit_permit        = fibdata.l2_limit_permit ;
    cfg->cl_rate_limiter_bypass = fibdata.rate_limiter_bypass;
    cfg->flowId_sel             = fibdata.flowId_sel;
    cfg->flowid                 = fibdata.flowID;
    
    return CS_E_OK;
}


cs_status aal_cl_type_hit_set(
    CS_IN cs_aal_port_id_t             port,
    CS_IN cs_aal_cl_type_no_hit_msk_t  *type_msk     
)
{ 
    FE_LE_CE_CTRL_t            msk;
    FE_LE_CE_CTRL_t           data;
    cs_aal_fe_instance_t fe_id = 0;
      
    if((type_msk==NULL)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
     
    msk.wrd = 0;
    data.wrd = 0;
    
    msk.bf.deny_no_type0_match =1;
    msk.bf.deny_no_type1_match =1;
    msk.bf.deny_no_type2_match =1;
    msk.bf.deny_no_type3_match =1;
    msk.bf.deny_no_type4_match =1;
    msk.bf.deny_no_type5_match =1;
    msk.bf.deny_no_type6_match =1;
    msk.bf.deny_no_type7_match =1;
    msk.bf.deny_no_type8_match =1;
    msk.bf.deny_no_type9_match =1;
    msk.bf.deny_no_type10_match =1;
    msk.bf.deny_no_type11_match =1;
    msk.bf.deny_no_type12_match =1;
    msk.bf.deny_no_type13_match =1;
    msk.bf.deny_no_type14_match =1;

    data.bf.deny_no_type0_match = type_msk->s.type_ipv4_long_key;
    data.bf.deny_no_type1_match = type_msk->s.type_ipv4_short_key;
    data.bf.deny_no_type2_match = type_msk->s.type_ipv6_long_key;
    data.bf.deny_no_type3_match = type_msk->s.type_ipv6_short_key;
    data.bf.deny_no_type4_match = type_msk->s.type_mirror_key;
    data.bf.deny_no_type5_match = type_msk->s.type_mac_vlan_key;
    data.bf.deny_no_type6_match = type_msk->s.type_mac_etype_key;
    data.bf.deny_no_type7_match = type_msk->s.type_ctrl_pkt_key;
    data.bf.deny_no_type8_match = type_msk->s.type_vlan_key;
    data.bf.deny_no_type9_match = type_msk->s.type_vlan_range_key;
    data.bf.deny_no_type10_match = type_msk->s.type_proto_key;
    data.bf.deny_no_type11_match = type_msk->s.type_mcast_key;
    data.bf.deny_no_type12_match = type_msk->s.type_mac_vlan_key_c;
    data.bf.deny_no_type13_match = type_msk->s.type_mac_etype_key_d;
    data.bf.deny_no_type14_match = type_msk->s.type_vlan_key_e;
        
    REG_MASK_WRITE_PORT(FE_LE_CE_CTRL , fe_id, msk.wrd, data.wrd); 
    
    return CS_E_OK;
}


cs_status aal_cl_type_hit_get(
    CS_IN  cs_aal_port_id_t               port,
    CS_OUT cs_aal_cl_type_no_hit_msk_t    *type_msk     
)
{
    FE_LE_CE_CTRL_t           data;
    cs_aal_fe_instance_t fe_id = 0;
      
    if((type_msk==NULL)||(port>AAL_PORT_ID_MII1))
    {  
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
     
    REG_READ_PORT(FE_LE_CE_CTRL , fe_id, data.wrd); 
    
    type_msk->s.type_ipv4_long_key   = data.bf.deny_no_type0_match;
    type_msk->s.type_ipv4_short_key  = data.bf.deny_no_type1_match;
    type_msk->s.type_ipv6_long_key   = data.bf.deny_no_type2_match;
    type_msk->s.type_ipv6_short_key  = data.bf.deny_no_type3_match ;
    type_msk->s.type_mirror_key      = data.bf.deny_no_type4_match;
    type_msk->s.type_mac_vlan_key    = data.bf.deny_no_type5_match;
    type_msk->s.type_mac_etype_key   = data.bf.deny_no_type6_match;
    type_msk->s.type_ctrl_pkt_key    = data.bf.deny_no_type7_match;
    type_msk->s.type_vlan_key        = data.bf.deny_no_type8_match;
    type_msk->s.type_vlan_range_key  = data.bf.deny_no_type9_match;
    type_msk->s.type_proto_key       = data.bf.deny_no_type10_match;
    type_msk->s.type_mcast_key       = data.bf.deny_no_type11_match;
    type_msk->s.type_mac_vlan_key_c  = data.bf.deny_no_type12_match;
    type_msk->s.type_mac_etype_key_d = data.bf.deny_no_type13_match;
    type_msk->s.type_vlan_key_e      = data.bf.deny_no_type14_match;
    
    return CS_E_OK;
}


cs_status aal_cl_rule_pri_get(
    CS_IN  cs_aal_port_id_t   port,
    CS_IN  cs_uint8           domain_id,
    CS_IN  cs_uint8           entry_id,
    CS_OUT cs_uint8           *pri
)
{   
    int                            i = 0;
    cs_aal_cl_domain_res_t  *prescfg = NULL;
    cs_uint16            entry_index = 0;
    cs_aal_fe_instance_t       fe_id = 0;
    
    if((domain_id>15)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
    
    prescfg = &g_ce_domain_res[fe_id][domain_id];
    if(prescfg->length)
    {
        if(entry_id>(prescfg->length-1))
            return CS_E_PARAM;
        entry_index = (entry_id+prescfg->cl_entry_start);
    }
    else
    {     
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    // search entry_index by pri
    for (i = prescfg->cl_entry_start; i < (prescfg->cl_entry_start+prescfg->length); i++) 
    { 
        if(g_ce_pri_res[fe_id][i] == entry_index)
        {
            *pri = (i-prescfg->cl_entry_start);
            return CS_E_OK;
        }       
    }         
    
    return CS_E_NOT_FOUND;
}


cs_status aal_cl_rule_pri_set(
    CS_IN cs_aal_port_id_t  port,
    CS_IN cs_uint8          domain_id,
    CS_IN cs_uint8          entry_id,
    CS_IN cs_uint8          pri
)
{  
    cs_aal_cl_domain_res_t * prescfg = NULL;
    cs_status                    ret = 0;
    int                            i = 0;
    cs_uint16            entry_index = 0;
    cs_uint8                  dst_pri= 0;
    cs_uint8                  cur_pri= 0;
    cs_aal_fe_instance_t       fe_id = 0;
    
    if((domain_id>15)||(port>AAL_PORT_ID_MII1))
    {  
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
    
    prescfg = &g_ce_domain_res[fe_id][domain_id];
    if(prescfg->length)
    {
        if((entry_id>(prescfg->length-1))||(pri>(prescfg->length-1)))
        {
            AAL_MIN_LOG("entry_id: %d, length: %d.\n", entry_id, prescfg->length);
            return CS_E_PARAM;
        }
        entry_index = (entry_id+prescfg->cl_entry_start);
        dst_pri = (pri+prescfg->cl_entry_start);
    }
    else
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    ret = aal_cl_rule_pri_get(fe_id, domain_id, entry_id, &cur_pri);
    if (ret)
    {   
        AAL_INF_LOG("ret : %d. [%s %d]\n", ret, __FUNCTION__, __LINE__);
        return ret;
    }
    
    cur_pri = cur_pri+prescfg->cl_entry_start;
    if(cur_pri == dst_pri) 
    { 
        return CS_E_OK;
    }
    
    if((dst_pri>=48)||(cur_pri>=48))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(cur_pri<dst_pri)
    {
        //dst_pri is more higher, then up insert node, move down the link list 
        for (i = 1; i < (dst_pri -cur_pri+1); i++) 
        {
            SWAP_INT_DATA(g_ce_pri_res[fe_id][dst_pri],g_ce_pri_res[fe_id][dst_pri-i]);
        }
        
         // write to hw entry
        if(cur_pri ==0)
        {
            ret = cl_entry_link_list_set( fe_id, g_ce_pri_res[fe_id][cur_pri], 63);        
        }
        else
        {
            ret = cl_entry_link_list_set( fe_id, g_ce_pri_res[fe_id][cur_pri], g_ce_pri_res[fe_id][cur_pri-1]);        
        }
        if (ret)
        {    
            AAL_INF_LOG("ret : %d. [%s %d]\n", ret, __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
        
        if(dst_pri ==47)
        {
            REG_FIELD_WRITE_PORT(FE_LE_CE_CTRL, fe_id, cl_start_entry, g_ce_pri_res[fe_id][dst_pri]);
            ret = cl_entry_link_list_set( fe_id, g_ce_pri_res[fe_id][dst_pri], g_ce_pri_res[fe_id][dst_pri-1]);  
        }
        else
        {
            ret = cl_entry_link_list_set( fe_id, g_ce_pri_res[fe_id][dst_pri+1], g_ce_pri_res[fe_id][dst_pri]);
            ret += cl_entry_link_list_set( fe_id, g_ce_pri_res[fe_id][dst_pri], g_ce_pri_res[fe_id][dst_pri-1]);  
        }
        if (ret)
        {   
            AAL_INF_LOG("ret : %d. [%s %d]\n", ret, __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
    }
    else
    {
        //cur_pri is more higher, then down insert node, move up the link list 
        for (i = (cur_pri -dst_pri); i >0; i--) 
        {
            SWAP_INT_DATA(g_ce_pri_res[fe_id][cur_pri],g_ce_pri_res[fe_id][cur_pri-i]);
        }
        
        // write to hw entry
        if(cur_pri ==47)
        {
            REG_FIELD_WRITE_PORT(FE_LE_CE_CTRL, fe_id, cl_start_entry, g_ce_pri_res[fe_id][cur_pri]);       
        }
        else
        {
            ret = cl_entry_link_list_set( fe_id, g_ce_pri_res[fe_id][cur_pri+1], g_ce_pri_res[fe_id][cur_pri]); 
            if (ret)
            {  
                AAL_INF_LOG("ret : %d. [%s %d]\n", ret, __FUNCTION__, __LINE__);
                return CS_E_ERROR;
            }
        }
        
        if(dst_pri ==0)
        {
            ret = cl_entry_link_list_set( fe_id, g_ce_pri_res[fe_id][dst_pri+1], g_ce_pri_res[fe_id][dst_pri]);
            ret += cl_entry_link_list_set( fe_id, g_ce_pri_res[fe_id][dst_pri], 63);  
        }
        else
        {
            ret = cl_entry_link_list_set( fe_id, g_ce_pri_res[fe_id][dst_pri+1], g_ce_pri_res[fe_id][dst_pri]);
            ret += cl_entry_link_list_set( fe_id, g_ce_pri_res[fe_id][dst_pri], g_ce_pri_res[fe_id][dst_pri-1]);  
        }
        if (ret)
        {  
            AAL_INF_LOG("ret : %d. [%s %d]\n", ret, __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
    }
    
    return CS_E_OK;
}


cs_status aal_cl_mac_filter_aged_latest_entry_get(
    CS_IN  cs_aal_port_id_t  port,
    CS_OUT cs_uint8          *entry_index
)
{   
    cs_aal_fe_instance_t       fe_id = 0;

    if((entry_index==NULL)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
     
    REG_FIELD_READ_PORT(FE_LE_CE_MAC_FILTER_AGING_STS, fe_id, aging_index, *entry_index);
    return CS_E_OK;
}


cs_status aal_cl_mac_filter_aging_vld_status_get(
    CS_IN  cs_aal_port_id_t  port,
    CS_OUT cs_uint32         *bitmap_high, 
    CS_OUT cs_uint32         *bitmap_low
)
{   
    cs_aal_fe_instance_t       fe_id = 0;

    if((bitmap_high==NULL)||(bitmap_low==NULL)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
     
    REG_READ_PORT(FE_LE_CE_MAC_FLTR_AGNG_VLD_STS1, fe_id, *bitmap_high);
    REG_READ_PORT(FE_LE_CE_MAC_FLTR_AGNG_VLD_STS0, fe_id, *bitmap_low);
    return CS_E_OK;
}

cs_status aal_cl_debug_status_get(
    CS_IN  cs_aal_port_id_t         port,
    CS_OUT cs_aal_cl_debug_status_t *cfg
)
{  
    FE_LE_CE_DEBUG_STS_t                        ce_sts;
    FE_LE_CE_DEBUG_PKT_STS_0_t             ce_pkt_sts0;
    FE_LE_CE_DEBUG_PKT_STS_1_t             ce_pkt_sts1;
    FE_LE_CE_DEBUG_PKT_STS_2_t             ce_pkt_sts2;
    FE_LE_CE_DEBUG_ENT_3_0_HIT_CNT_t   ce_ent_hit_cnt0;
    FE_LE_CE_DEBUG_ENT_7_4_HIT_CNT_t   ce_ent_hit_cnt1;
    FE_LE_CE_DEBUG_ENT_11_8_HIT_CNT_t  ce_ent_hit_cnt2;
    FE_LE_CE_DEBUG_ENT_15_12_HIT_CNT_t ce_ent_hit_cnt3;
    cs_aal_fe_instance_t                     fe_id = 0;
    

    if((cfg==NULL)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
    
    REG_READ_PORT(FE_LE_CE_DEBUG_STS, fe_id, ce_sts.wrd);
    REG_READ_PORT(FE_LE_CE_DEBUG_PKT_STS_0, fe_id, ce_pkt_sts0.wrd);
    REG_READ_PORT(FE_LE_CE_DEBUG_PKT_STS_1, fe_id, ce_pkt_sts1.wrd);
    REG_READ_PORT(FE_LE_CE_DEBUG_PKT_STS_2, fe_id, ce_pkt_sts2.wrd);
    REG_READ_PORT(FE_LE_CE_DEBUG_ENT_3_0_HIT_CNT, fe_id, ce_ent_hit_cnt0.wrd);
    REG_READ_PORT(FE_LE_CE_DEBUG_ENT_7_4_HIT_CNT, fe_id, ce_ent_hit_cnt1.wrd);
    REG_READ_PORT(FE_LE_CE_DEBUG_ENT_11_8_HIT_CNT, fe_id, ce_ent_hit_cnt2.wrd);
    REG_READ_PORT(FE_LE_CE_DEBUG_ENT_15_12_HIT_CNT, fe_id, ce_ent_hit_cnt3.wrd);
    
    cfg->hit_fibindex_recent      = ce_sts.bf.hit_index_0;
    cfg->hit_fibindex_previous    = ce_sts.bf.hit_index_1;
    cfg->hit_fibindex_earlier     = ce_sts.bf.hit_index_2;
    cfg->fibindex_vlan_rslt       = ce_pkt_sts0.bf.vlan_rslt_addr;
    cfg->fibindex_dscp_rslt       = ce_pkt_sts0.bf.dscp_rslt_addr;
    cfg->fibindex_dpid_rslt       = ce_pkt_sts0.bf.dpid_rslt_addr;
    cfg->fibindex_cos_rslt        = ce_pkt_sts1.bf.cos_rslt_addr;
    cfg->fibindex_l2_lrn_rslt     = ce_pkt_sts1.bf.l2_lrn_rslt_addr;
    cfg->fibindex_dot1p_rslt      = ce_pkt_sts1.bf.dot1p_rslt_addr;
    cfg->fibindex_permit_pri_rslt = ce_pkt_sts2.bf.permit_priority_rslt_addr;
    cfg->fibindex_rate_lmt_rslt   = ce_pkt_sts2.bf.rate_lmt_rslt_addr;
    cfg->entry0_hit_count         = ce_ent_hit_cnt0.bf.hit_cnt_0;
    cfg->entry1_hit_count         = ce_ent_hit_cnt0.bf.hit_cnt_1;
    cfg->entry2_hit_count         = ce_ent_hit_cnt0.bf.hit_cnt_2;
    cfg->entry3_hit_count         = ce_ent_hit_cnt0.bf.hit_cnt_3;
    cfg->entry4_hit_count         = ce_ent_hit_cnt1.bf.hit_cnt_0;
    cfg->entry5_hit_count         = ce_ent_hit_cnt1.bf.hit_cnt_1;
    cfg->entry6_hit_count         = ce_ent_hit_cnt1.bf.hit_cnt_2;
    cfg->entry7_hit_count         = ce_ent_hit_cnt1.bf.hit_cnt_3;
    cfg->entry8_hit_count         = ce_ent_hit_cnt2.bf.hit_cnt_0;
    cfg->entry9_hit_count         = ce_ent_hit_cnt2.bf.hit_cnt_1;
    cfg->entry10_hit_count        = ce_ent_hit_cnt2.bf.hit_cnt_2;
    cfg->entry11_hit_count        = ce_ent_hit_cnt2.bf.hit_cnt_3;
    cfg->entry12_hit_count        = ce_ent_hit_cnt3.bf.hit_cnt_0;
    cfg->entry13_hit_count        = ce_ent_hit_cnt3.bf.hit_cnt_1;
    cfg->entry14_hit_count        = ce_ent_hit_cnt3.bf.hit_cnt_2;
    cfg->entry15_hit_count        = ce_ent_hit_cnt3.bf.hit_cnt_3;
        
    return CS_E_OK;
}


cs_status aal_cl_debug_prog_count_set(
    CS_IN cs_aal_port_id_t               port,
    CS_IN cs_aal_cl_debug_prog_ent_cnt_t *cfg
)
{   
    cs_aal_fe_instance_t     fe_id = 0;
 
    if((cfg==NULL)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_MIN_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);
    
    if(cfg->entry_addr > CL_ENTRY_MAXNUM)
    {    
        AAL_INF_LOG("entry_addr: %d. [%s %d]\n", cfg->entry_addr, __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    REG_FIELD_WRITE_PORT(FE_LE_CE_DEBUG_ENT_HIT_CNT_ADR_CFG, fe_id, key_addr, cfg->entry_addr);

    return CS_E_OK;
}


cs_status aal_cl_debug_prog_count_get(
    CS_IN  cs_aal_port_id_t                port,
    CS_OUT cs_aal_cl_debug_prog_ent_cnt_t  *cfg
)
{   
    cs_aal_fe_instance_t     fe_id = 0;

    if((cfg==NULL)||(port>AAL_PORT_ID_MII1))
    {   
        AAL_INF_LOG("para error. [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    fe_id = FE_PORT(port);

    REG_FIELD_READ_PORT(FE_LE_CE_DEBUG_ENT_HIT_CNT_ADR_CFG, fe_id, key_addr, cfg->entry_addr);
    REG_READ_PORT(FE_LE_CE_DEBUG_ENT_HIT_CNT, fe_id, cfg->hit_count);

    return CS_E_OK;
}


cs_status cls_init(void)
{
    int                                     i;
    cs_aal_fe_instance_t                fe_id;
    FE_LE_PP_PARSER_CTRL_t                msk;
    FE_LE_PP_PARSER_CTRL_t               data;
    cs_aal_cl_def_fib_data_t          def_fib;
    cs_aal_cl_spcl_ptn_control_msk_t   ctrmsk;
    cs_aal_cl_spcl_ptn_control_t speckey_ctrl;
    cs_aal_cl_spcl_ptn_t          speckey_cfg;
    cl_key_entry_t                   ce_entry;
    cl_fib_data_t                     fibdata;
    cs_status                         ret = 0;

    
    cls_util_init();
     
// init  Priority Mapping resource
//Highest priority is 47, it corresponds to the entry 47.
/*          PRI		  CE entryindex          */
/* start    47               47	             */
/*          46	             46	             */
/* 	        45               45	             */
/*          44               44              */
/* 	        43               43	             */
/*          42               42              */
/*          ...              ...             */
/*          2                2               */
/*          1                1               */
/*          0                0               */
/*  end                      63              */
/*********************************************/
    for ( fe_id= 0; fe_id <= AAL_FE_MA_ID; fe_id++) 
    {  
        for (i = 0; i < 48; i++) 
        { 
            g_ce_pri_res[fe_id][i] = i;
        }         
    }
    
    //enable aging
    //Since user request minimum 0xfffef sec, max_value is 0xffffff (24bits),
    //so Minimum interval should be 0x1DCD4.
    //Set it to 0x20000
    REG_WRITE_PORT(FE_LE_CE_MAC_FLTR_AGNG_TMR_CFG, AAL_FE_PON_ID, 0x20000);
    REG_FIELD_WRITE_PORT(FE_LE_CE_CTRL, AAL_FE_PON_ID, mac_filter_aging_en, 1);
    
    //special packet parser control
    data.wrd = 0;
    msk.wrd  = 0;
    
    msk.bf.spcl_hdr_after_l2hdr_en   = FILL_MSK;
    msk.bf.spcl_hdr_after_iphdr_en   = FILL_MSK;
    msk.bf.spcl_hdr_after_tcphdr_en  = FILL_MSK;
    data.bf.spcl_hdr_after_l2hdr_en  = 1;
    data.bf.spcl_hdr_after_iphdr_en  = 1;
    data.bf.spcl_hdr_after_tcphdr_en = 1;

    msk.bf.spcl_hdr_parsing_en  = FILL_MSK;
    data.bf.spcl_hdr_parsing_en = 1;

    for ( fe_id= 0; fe_id <= AAL_FE_MA_ID; fe_id++) 
    {
        REG_MASK_WRITE_PORT(FE_LE_PP_PARSER_CTRL, fe_id, msk.wrd, data.wrd); 
    }
    // cls default fib 
    memset(&def_fib, 0, sizeof(cs_aal_cl_def_fib_data_t));
    def_fib.mask.s.permit = 1;
    def_fib.mask.s.dpid = 1;
    def_fib.mask.s.cos = 1;
    
    def_fib.permit = 1;
    def_fib.cos_sel = AAL_CL_COS_SEL_MAP_RX_DOT1P;
    def_fib.cl_mirror_en = 0;
    def_fib.cl_mirror_src_sel = 0;
    def_fib.dpid.dst_op = AAL_CL_DST_PORT;
   
    for ( fe_id= 0; fe_id <= AAL_FE_MA_ID; fe_id++) 
    {
        def_fib.dpid.dpid = (fe_id == AAL_FE_PON_ID) ? 0 : 1;       
        ret += aal_cl_default_fib_set((cs_aal_port_id_t)fe_id, &def_fib);
    }
    if (ret)
    {    
        cs_halt("cls default fib init FAILED\n");
    }
    // upstream drop SA=mc/bc traffic
    ctrmsk.u32 = 0;
    ctrmsk.s.mode_sel   = 1;
    ctrmsk.s.parse_ctrl = 1;
    ctrmsk.s.glb_strt   = 1;
    speckey_ctrl.spcl_hdr_parsing_en   = 1;
    speckey_ctrl.spcl_hdr_mode_sel     = AAL_CL_SPCL_MODE_STA;
    speckey_ctrl.spcl_glb_hdr_strt_idx = 6;
    
    ret = aal_cl_spcl_ptn_ctrl_set( AAL_PORT_ID_GE, ctrmsk, &speckey_ctrl);
    
    speckey_cfg.spcl_ptn_key  = 0x0100;
    speckey_cfg.spcl_ptn_mask = 0xfeff;
    speckey_cfg.spcl_ptn_cmp  = AAL_CL_SPCL_PTN_CMP_HI;
    speckey_cfg.spcl_ptn_type = AAL_CL_SPCL_PTN_TYPE_STA;
    
    ret += aal_cl_spcl_ptn_entry_set( AAL_PORT_ID_GE, AAL_CL_RSV_SPE_ENTRY_UP_A, &speckey_cfg);

    if (ret)
    {     
        cs_halt("cls upstream speckey_cfg for drop SA=mc/bc init FAILED\n");
    }
     
    memset( &ce_entry, 0, sizeof(cl_key_entry_t));
    memset( &fibdata, 0, sizeof(cl_fib_data_t));

    ce_entry.entryvalid    = 1;
    ce_entry.keytype = AAL_CL_IPV4_LONG_KEY;
 
    ce_entry.u.cl_ipv4_long_key.spcl_hit_map   = 0x80;
    ce_entry.u.cl_ipv4_long_key.mask           = CL_KEY_MASK_SPECPKT;    

    ret = cl_key_entry_set( AAL_FE_GE_ID, AAL_CL_RSV_ENTRY_UP_A, &ce_entry);
      
    fibdata.valid             = 1;
    fibdata.permit_filter_dis = 1;
    fibdata.permit            = 0;
    fibdata.permit_pri        = 1;

    ret += cl_fib_table_set(AAL_FE_GE_ID, CL_SUB_KEY_NUM*AAL_CL_RSV_ENTRY_UP_A+0, &fibdata);
   
    if (ret)
    {    
        cs_halt("cls upstream default drop SA=mc/bc init FAILED\n");
    }
#if 0
    // downstream drop SA=000000 traffic
    memset( &ce_entry, 0, sizeof(cl_key_entry_t));
    memset( &fibdata, 0, sizeof(cl_fib_data_t));

    ce_entry.entryvalid    = 1;
    ce_entry.keytype = AAL_CL_IPV4_LONG_KEY;
 
    ce_entry.u.cl_ipv4_long_key.mask       = CL_KEY_MASK_MACSA;    

    ret = cl_key_entry_set( AAL_FE_PON_ID, AAL_CL_RSV_ENTRY_UP_A, &ce_entry);
      
    fibdata.valid             = 1;
    fibdata.permit_filter_dis = 1;
    fibdata.permit            = 0;
    fibdata.permit_pri        = 1;

    ret += cl_fib_table_set(AAL_FE_PON_ID, CL_SUB_KEY_NUM*AAL_CL_RSV_ENTRY_UP_A+0, &fibdata);
   
    if (ret)
    {    
        cs_halt("cls downstream default drop SA=000000 init FAILED\n");
    }
#endif    
    return CS_E_OK;
}


/*  end of file */


