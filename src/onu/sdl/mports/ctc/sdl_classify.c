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
#include "sdl_classify.h"
#include "rtk_api_ext.h"
#include "rtk_error.h"
#include "sdl_classify_util.h"
#include "sdl_util.h"
#include "sdl.h"



// port(1-4),index(0-11)
#define  __RTK_ACL_IDX(port,index)    (((port-1)*RTK_ACL_CLS_MAC_PORT_LENGTH)+RTK_ACL_CLS_MAC_START+index)


sw_cls_mac_res_port_t g_sw_tbl[UNI_PORT_MAX];
static cs_boolean     g_def_rule_en;

static cs_status __sw_acl_add (
    CS_IN cs_port_id_t                 port_id,
    CS_IN cs_uint8                     index,
    CS_IN cs_sdl_classification_t      *prule
)
{
    cs_int32            i;
    cs_sdl_cls_rule_t   *ptemp =NULL;
    cs_uint32           ruleNum;
    rtk_filter_cfg_t    cfg;
    rtk_filter_action_t act;
    rtk_filter_field_t  field;
    rtk_api_ret_t       rt;


    memset(&cfg, 0, sizeof(rtk_filter_cfg_t));
    memset(&act, 0, sizeof(rtk_filter_action_t));
    memset(&field, 0, sizeof(rtk_filter_field_t));
    
    ptemp = &prule->fselect[0];
    
    switch(ptemp->fieldSelect)
    {
        case CLASS_RULES_FSELECT_DA_MAC:  
        {
            field.fieldType = FILTER_FIELD_DMAC;
            field.filter_pattern_union.mac.dataType = FILTER_FIELD_DATA_MASK;
            for(i = 0; i < CS_MACADDR_LEN; i++){
                field.filter_pattern_union.mac.value.octet[i] = ptemp->matchValue[CLASS_MATCH_VAL_LEN-6+i];
                field.filter_pattern_union.mac.mask.octet[i]  = 0xff;
            }
            break;
        }
        case CLASS_RULES_FSELECT_SA_MAC:   
        {
            field.fieldType = FILTER_FIELD_SMAC;
            field.filter_pattern_union.mac.dataType = FILTER_FIELD_DATA_MASK;
            for(i = 0; i < CS_MACADDR_LEN; i++){
                field.filter_pattern_union.mac.value.octet[i] = ptemp->matchValue[CLASS_MATCH_VAL_LEN-6+i];
                field.filter_pattern_union.mac.mask.octet[i]  = 0xff;
            }
            break;
        }
        case CLASS_RULES_FSELECT_802_1P:     
        {
            field.fieldType = FILTER_FIELD_CTAG;
            field.filter_pattern_union.ctag.pri.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.ctag.pri.value = ptemp->matchValue[CLASS_MATCH_VAL_LEN-1];
            field.filter_pattern_union.ctag.pri.mask = 0x7;
              
            break;
        }
        case CLASS_RULES_FSELECT_VLAN_ID:    
        {
            field.fieldType = FILTER_FIELD_CTAG;
            field.filter_pattern_union.ctag.vid.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.ctag.vid.value = (((ptemp->matchValue[CLASS_MATCH_VAL_LEN - 2]&0xf)<<8)|ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]);
            field.filter_pattern_union.ctag.vid.mask = 0xfff;
              
            break;
        }
        case CLASS_RULES_FSELECT_ETH_TYPE:
        {
            field.fieldType = FILTER_FIELD_ETHERTYPE;
            field.filter_pattern_union.etherType.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.etherType.value = ((ptemp->matchValue[CLASS_MATCH_VAL_LEN - 2]<<8)|ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]);
            field.filter_pattern_union.etherType.mask = 0xffff;
              
            break;
        }
        case CLASS_RULES_FSELECT_DA_IPV4:
        {     
            field.fieldType = FILTER_FIELD_IPV4_DIP;
            field.filter_pattern_union.dip.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.dip.value = ((ptemp->matchValue[CLASS_MATCH_VAL_LEN - 4]<<24)|(ptemp->matchValue[CLASS_MATCH_VAL_LEN - 3]<<16)|
                                                    (ptemp->matchValue[CLASS_MATCH_VAL_LEN - 2]<<8)|ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]);
            field.filter_pattern_union.dip.mask = 0xffffffff;
               
            break;
        }
        case CLASS_RULES_FSELECT_SA_IPV4:
        {     
            field.fieldType = FILTER_FIELD_IPV4_SIP;
            field.filter_pattern_union.sip.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.sip.value = ((ptemp->matchValue[CLASS_MATCH_VAL_LEN - 4]<<24)|(ptemp->matchValue[CLASS_MATCH_VAL_LEN - 3]<<16)|
                                                    (ptemp->matchValue[CLASS_MATCH_VAL_LEN - 2]<<8)|ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]);
            field.filter_pattern_union.sip.mask = 0xffffffff;
               
            break;
        }
        case CLASS_RULES_FSELECT_IPPRTO:
        {     
            field.fieldType = FILTER_FIELD_IPV4_PROTOCOL;
            field.filter_pattern_union.protocol.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.protocol.value = ptemp->matchValue[CLASS_MATCH_VAL_LEN-1];
            field.filter_pattern_union.protocol.mask  = 0xff;
               
            break;
        }
        case CLASS_RULES_FSELECT_TOS_DSCP:
        {     
            field.fieldType = FILTER_FIELD_IPV4_TOS;
            field.filter_pattern_union.ipTos.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.ipTos.value = ptemp->matchValue[CLASS_MATCH_VAL_LEN-1]<<2;
            field.filter_pattern_union.ipTos.mask  = 0xfc;
               
            break;
        }
        case CLASS_RULES_FSELECT_IPV6_TC:
        {     
            field.fieldType = FILTER_FIELD_IPV6_TRAFFIC_CLASS;
            field.filter_pattern_union.ipv6TrafficClass.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.ipv6TrafficClass.value = ptemp->matchValue[CLASS_MATCH_VAL_LEN-1];
            field.filter_pattern_union.ipv6TrafficClass.mask  = 0xff;
               
            break;
        }
        case CLASS_RULES_FSELECT_L4_SRC_PORT:
        {     
            field.fieldType = FILTER_FIELD_TCP_SPORT;
            field.filter_pattern_union.tcpSrcPort.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.tcpSrcPort.value = ((ptemp->matchValue[CLASS_MATCH_VAL_LEN - 2]<<8)|ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]);
            field.filter_pattern_union.tcpSrcPort.mask  = 0xffff;
               
            break;
        }
        case CLASS_RULES_FSELECT_L4_DST_PORT:  
        {     
            field.fieldType = FILTER_FIELD_TCP_DPORT;
            field.filter_pattern_union.tcpDstPort.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.tcpDstPort.value = ((ptemp->matchValue[CLASS_MATCH_VAL_LEN - 2]<<8)|ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]);
            field.filter_pattern_union.tcpDstPort.mask  = 0xffff;
               
            break;
        }   
        case CLASS_RULES_FSELECT_IPVER:  
        {   
            field.fieldType = FILTER_FIELD_DMAC;
            field.filter_pattern_union.dmac.dataType = FILTER_FIELD_DATA_MASK;
            if(ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1] == 4)
            {
                cfg.careTag.tagType[CARE_TAG_IPV4].value = TRUE;
                cfg.careTag.tagType[CARE_TAG_IPV4].mask  = TRUE;           
            }
            else
            {
                cfg.careTag.tagType[CARE_TAG_IPV6].value = TRUE;
                cfg.careTag.tagType[CARE_TAG_IPV6].mask  = TRUE;           
            }            
            break;        
        }
        case CLASS_RULES_FSELECT_IPV6_FLOWLABEL:  
        {   
            field.fieldType = FILTER_FIELD_IPV6_FLOW_LABEL;
            field.filter_pattern_union.flowLabel.dataType = FILTER_FIELD_DATA_MASK;
            
            field.filter_pattern_union.flowLabel.value= (((ptemp->matchValue[CLASS_MATCH_VAL_LEN - 3]&0xf)<<16)|
                                                          (ptemp->matchValue[CLASS_MATCH_VAL_LEN - 2]<<8)|
                                                           ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]);
            field.filter_pattern_union.flowLabel.mask = 0xfffff;
         
            break;        
        }   
        case CLASS_RULES_FSELECT_DA_IPV6:  
        {   
            field.fieldType = FILTER_FIELD_IPV6_DIPV6;
            field.filter_pattern_union.dipv6.dataType = FILTER_FIELD_DATA_MASK;
            for(i = 0; i < RTK_IPV6_ADDR_WORD_LENGTH; i++){
                field.filter_pattern_union.dipv6.value.addr[i]= ((ptemp->matchValue[4*i]<<24)|(ptemp->matchValue[4*i+1]<<16)|
                                                                 (ptemp->matchValue[4*i+2]<<8)|ptemp->matchValue[4*i+3]);             
                field.filter_pattern_union.dipv6.mask.addr[i] = 0xffffffff;
            }
            field.filter_pattern_union.dipv6.mask.addr[2] = 0; // not care this 32bits
            break;        
        }
        case CLASS_RULES_FSELECT_SA_IPV6:  
        {   
            field.fieldType = FILTER_FIELD_IPV6_SIPV6;
            field.filter_pattern_union.dipv6.dataType = FILTER_FIELD_DATA_MASK;
            for(i = 0; i < RTK_IPV6_ADDR_WORD_LENGTH; i++){
                field.filter_pattern_union.sipv6.value.addr[i]= ((ptemp->matchValue[4*i]<<24)|(ptemp->matchValue[4*i+1]<<16)|
                                                                 (ptemp->matchValue[4*i+2]<<8)|ptemp->matchValue[4*i+3]);             
                field.filter_pattern_union.sipv6.mask.addr[i] = 0xffffffff;
            }
            field.filter_pattern_union.sipv6.mask.addr[2] = 0; // not care this 32bits
            break;        
        }
        case CLASS_RULES_FSELECT_DA_IPV6_PREFIX:  
        {   
            field.fieldType = FILTER_FIELD_IPV6_DIPV6;
            field.filter_pattern_union.dipv6.dataType = FILTER_FIELD_DATA_MASK;
            
            for(i = 0; i < 2; i++){
                field.filter_pattern_union.dipv6.value.addr[i]= ((ptemp->matchValue[4*i]<<24)|(ptemp->matchValue[4*i+1]<<16)|
                                                                 (ptemp->matchValue[4*i+2]<<8)|ptemp->matchValue[4*i+3]);             
            }
            if(ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]>32)
            {
                field.filter_pattern_union.dipv6.mask.addr[0] = 0xffffffff;
                field.filter_pattern_union.dipv6.mask.addr[1] = 0xffffffff<<(64-ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]);
            }
            else
                field.filter_pattern_union.dipv6.mask.addr[0] = 0xffffffff<<(32-ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]);                 
              
            break;        
        }
        case CLASS_RULES_FSELECT_SA_IPV6_PREFIX: 
        {   
            field.fieldType = FILTER_FIELD_IPV6_SIPV6;
            field.filter_pattern_union.sipv6.dataType = FILTER_FIELD_DATA_MASK;
            
            for(i = 0; i < 2; i++){
                field.filter_pattern_union.sipv6.value.addr[i]= ((ptemp->matchValue[4*i]<<24)|(ptemp->matchValue[4*i+1]<<16)|
                                                                 (ptemp->matchValue[4*i+2]<<8)|ptemp->matchValue[4*i+3]);             
            }
            if(ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]>32)
            {
                field.filter_pattern_union.sipv6.mask.addr[0] = 0xffffffff;
                field.filter_pattern_union.sipv6.mask.addr[1] = 0xffffffff<<(64-ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]);
            }
            else
                field.filter_pattern_union.sipv6.mask.addr[0] = 0xffffffff<<(32-ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]);                 
              
            break;        
        }
        case CLASS_RULES_FSELECT_IPV6_NEXT_HEADER:  
        {     
            field.fieldType = FILTER_FIELD_IPV6_NEXT_HEADER;
            field.filter_pattern_union.ipv6NextHeader.dataType = FILTER_FIELD_DATA_MASK;
            field.filter_pattern_union.ipv6NextHeader.value = ptemp->matchValue[CLASS_MATCH_VAL_LEN-1];
            field.filter_pattern_union.ipv6NextHeader.mask  = 0xff;
               
            break;
        }    
        default:       
            return CS_E_PARAM;
    }

    field.next = NULL; 
    rt = rtk_filter_igrAcl_field_add(&cfg, &field);       
    if(rt){
      return CS_E_ERROR;
    }
    cfg.invert = FALSE;    
    cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
    cfg.activeport.value = 0x1<<(port_id-1);
    cfg.activeport.mask = 0xFF;  
    
    /*Set QOS Action */
    if(prule->priMark== 0xff) /* 0xff means that don't perform priority marking */
    {
        // act.filterPriority = prule->queueMapped;
        /* 
           NO action, because the resources of classification is share with MAC filter/bind,
           Must ensure that Forward action is from MAC filter/bind,
        */
    }
    else
    {
       // act.actEnable[FILTER_ENACT_1P_REMARK] = TRUE;
        act.actEnable[FILTER_ENACT_PRIORITY] = TRUE;
        act.filterPriority = prule->priMark;
    }
      
    rt = rtk_filter_igrAcl_cfg_del(__RTK_ACL_IDX(port_id,index));
        
    rt += rtk_filter_igrAcl_cfg_add(__RTK_ACL_IDX(port_id,index), &cfg, &act, &ruleNum); 
    if(rt){
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
}

static cs_status __sw_acl_del (
    CS_IN cs_port_id_t                 port_id,
    CS_IN cs_uint8                     index
)
{   
    rtk_api_ret_t  retVal;
          
    retVal = rtk_filter_igrAcl_cfg_del(__RTK_ACL_IDX(port_id,index)); 
    if(retVal){
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
}

static cs_status __sw_acl_def ( CS_IN cs_boolean  enable)
{    
    cs_uint32      i;
    rtk_api_ret_t rt;
#if 0    
    rtk_filter_field_t  field;
    rtk_filter_cfg_t     cfg;
    rtk_filter_action_t  act; 
    rtk_filter_number_t  ruleNum;
#endif

    if(enable == g_def_rule_en)
        return CS_E_OK;
   
    if(enable)
    {
        rt = rtk_qos_1pRemarkEnable_set(SWITCH_UPLINK_PORT, EPON_TRUE);  
        if(rt)
            return CS_E_ERROR;
 #if 0       
        memset(&cfg, 0, sizeof(rtk_filter_cfg_t));
        memset(&act, 0, sizeof(rtk_filter_action_t));
        memset(&field, 0, sizeof(rtk_filter_field_t));

        field.fieldType = FILTER_FIELD_DMAC;       
        field.filter_pattern_union.dmac.dataType = FILTER_FIELD_DATA_MASK;
        field.filter_pattern_union.dmac.value.octet[0] = 0x00;
        field.filter_pattern_union.dmac.value.octet[1] = 0x00;
        field.filter_pattern_union.dmac.value.octet[2] = 0x00;
        field.filter_pattern_union.dmac.value.octet[3] = 0x00;
        field.filter_pattern_union.dmac.value.octet[4] = 0x00;
        field.filter_pattern_union.dmac.value.octet[5] = 0x00;
        field.filter_pattern_union.dmac.mask.octet[0]  = 0x00;
        field.filter_pattern_union.dmac.mask.octet[1]  = 0x00;
        field.filter_pattern_union.dmac.mask.octet[2]  = 0x00;
        field.filter_pattern_union.dmac.mask.octet[3]  = 0x00;
        field.filter_pattern_union.dmac.mask.octet[4]  = 0x00;
        field.filter_pattern_union.dmac.mask.octet[5]  = 0x00;
        field.next = NULL;

        rt = rtk_filter_igrAcl_field_add(&cfg, &field);       
        if(rt)
            return CS_E_ERROR;
        
        cfg.invert = FALSE;    
        cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
        cfg.activeport.value = 0xf;
        cfg.activeport.mask = 0xFF;  

        act.actEnable[FILTER_ENACT_PRIORITY] = TRUE;
        act.filterPriority = 0;

        rt = rtk_filter_igrAcl_cfg_add(RTK_ACL_CLS_DEF, &cfg, &act, &ruleNum); 
        if(rt)
            return CS_E_ERROR;        
#endif       
    }
    else
    {
        for(i=0; i<4; i++)
        {   
            if(g_sw_tbl[i].g_cls_cnt)
                return CS_E_OK;
        }

        rt = rtk_qos_1pRemarkEnable_set(SWITCH_UPLINK_PORT, EPON_FALSE);  
     //   rt += rtk_filter_igrAcl_cfg_del(RTK_ACL_CLS_DEF); 
        if(rt)
            return CS_E_ERROR;      
    }
   
    g_def_rule_en = enable;
      
    return CS_E_OK;
}

cs_status epon_request_onu_port_classification_add(
        CS_IN cs_callback_context_t         context,
        CS_IN cs_int32                      device,
        CS_IN cs_int32                      llidport,
        CS_IN cs_port_id_t                  port_id,
        CS_IN cs_uint8                      num,
        CS_IN cs_sdl_classification_t       *cfg
)
{    
    cs_int32  i;
    cs_status ret = CS_E_OK;
    cs_uint8  start_idx=0; //affect highest index,for the start index of update HW 
    cs_uint8  tbl_num =0;
    cs_uint8  tbl_size =0;
    cs_uint32 masktemp;
    cs_sdl_classification_t  *ptemp =NULL;
    cs_sdl_classification_t  tbl[RTK_ACL_CLS_MAC_PORT_LENGTH];

        
    UNI_PORT_CHECK(port_id);

    if(cfg == NULL)
    {  
        SDL_MIN_LOG(" NULL pointer.\n");
        return CS_E_PARAM;
    }
     
    //tbl_size = (g_sw_tbl[port_id-1].g_mac_cnt >4)?(RTK_ACL_CLS_MAC_PORT_LENGTH -g_sw_tbl[port_id-1].g_mac_cnt): RTK_ACL_CLS_PORT_LENGTH;
    tbl_size = RTK_ACL_CLS_MAC_PORT_LENGTH -g_sw_tbl[port_id-1].g_mac_cnt;
    if(tbl_size == 0)
    {  
        SDL_MIN_LOG("CLS resouce is 0.\n");
        return CS_E_RESOURCE;
    }
       
    if((num >tbl_size)||(num <1))
    {  
        SDL_MIN_LOG("para error. rule num: %d. [%s %d]\n", num,  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    ret = class_rule_para_check(num, cfg);
    if (ret) 
        return ret;

    ptemp = cfg;
    
     /* only support one filter key for multi-ports onu */
    for(i=0; i<num; i++)
    {   
        if(ptemp->entries!= 1){
            return CS_E_PARAM;
        }
        /* not support 0xff*/
        if(ptemp->priMark == 0xff){
            return CS_E_NOT_SUPPORT;
        }
        masktemp = 0;
        /* rule field para check */
        ret = class_field_para_check(ptemp->entries, ptemp->fselect, &masktemp);
        if (ret) 
            return ret;
        if((masktemp&(1<<CLASS_RULES_FSELECT_DA_IPV6_PREFIX))||
            (masktemp&(1<<CLASS_RULES_FSELECT_SA_IPV6_PREFIX)))
        {
            if (ptemp->fselect[0].matchValue[CLASS_MATCH_VAL_LEN - 1] > 64)
            {
                SDL_MIN_LOG("illegal IPV6 PREFIX(1-64),%d. L%d\n",ptemp->fselect[0].matchValue[CLASS_MATCH_VAL_LEN - 1], __LINE__);
                return CS_E_PARAM;
            }
        }
            
        ptemp++;
    }
    
     /* add record */
    ret = class_rule_tbl_add_record(port_id, tbl_size, num, cfg, &start_idx);
    if (ret) 
        return ret;

    memset(tbl, 0, RTK_ACL_CLS_MAC_PORT_LENGTH*sizeof(cs_sdl_classification_t));
    /* get record to update HW */ 
    ret = class_rule_tbl_get_record(port_id, &tbl_num, tbl);
    if (ret) 
        return ret;

    for(i=tbl_num-1; i>=start_idx; i--)
    { 
        ret =__sw_acl_add(port_id, i, &tbl[i]);
        if (ret) 
            return ret;
    }

    g_sw_tbl[port_id-1].g_cls_cnt = tbl_num;

    ret =__sw_acl_def(TRUE);
       
    return ret;    
}

cs_status epon_request_onu_port_classification_del(
        CS_IN cs_callback_context_t         context,
        CS_IN cs_int32                      device,
        CS_IN cs_int32                      llidport,
        CS_IN cs_port_id_t                  port_id,
        CS_IN cs_uint8                      prenum,
        CS_IN cs_uint8                      *precedence
)
{   
    cs_int32  i;
    cs_status ret = CS_E_OK;
    cs_uint8  tbl_num =0;
    cs_uint8  tbl_size =0;
    cs_uint8  start_idx; //Effects highest index,for the start index of update HW 
    cs_sdl_classification_t  tbl[RTK_ACL_CLS_MAC_PORT_LENGTH];

    
    UNI_PORT_CHECK(port_id);

    // tbl_size = (g_sw_tbl[port_id-1].g_mac_cnt >4)?(RTK_ACL_CLS_MAC_PORT_LENGTH -g_sw_tbl[port_id-1].g_mac_cnt): RTK_ACL_CLS_PORT_LENGTH;
    tbl_size = RTK_ACL_CLS_MAC_PORT_LENGTH -g_sw_tbl[port_id-1].g_mac_cnt; 
    if((prenum >tbl_size)||(prenum <1))
    {  
        SDL_MIN_LOG("para error. rule num: %d. [%s %d]\n", prenum,  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
    
    if(precedence == NULL)
    {  
        SDL_MIN_LOG(" NULL pointer.\n");
        return CS_E_PARAM;
    }
    /* get current data number before del operation */
    ret = class_rule_tbl_get_record(port_id, &tbl_num, tbl);
    if (ret) 
        return ret;
    
    if(tbl_num==0)
         return CS_E_OK;
        
    ret = class_rule_tbl_del_record(port_id, prenum, precedence, &start_idx);
    if (ret) 
        return ret;
    /* can't deleted any entry*/
    if(start_idx == tbl_num)
        return CS_E_OK;

    memset(tbl, 0, RTK_ACL_CLS_MAC_PORT_LENGTH*sizeof(cs_sdl_classification_t));
    tbl_num =0;
     /* get finally record to update HW */ 
    ret = class_rule_tbl_get_record(port_id, &tbl_num, tbl);
    if (ret) 
        return ret;

    for(i=start_idx; i< tbl_size; i++)
    {   
        if(i>=tbl_num)
        {
            ret =__sw_acl_del(port_id, i);
        }
        else
        {
            ret =__sw_acl_add(port_id, i, &tbl[i]);
        }
      
        if (ret) 
            return ret;
    }

    g_sw_tbl[port_id-1].g_cls_cnt = tbl_num;
   
    ret =__sw_acl_def(FALSE);
       
    return ret;       
}

cs_status epon_request_onu_port_classification_get(
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_int32                  device,
    CS_IN  cs_int32                  llidport,
    CS_IN  cs_port_id_t              port_id,                
    CS_OUT cs_uint8                  *rule_num,
    CS_OUT cs_sdl_classification_t   *cfg
)
{ 
   
    UNI_PORT_CHECK(port_id);
   
    if((rule_num == NULL)||(cfg == NULL))
    {  
        SDL_MIN_LOG("para error.  [%s %d]\n",  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    return class_rule_tbl_get_record(port_id, rule_num, cfg);        
}

cs_status epon_request_onu_port_classification_clr(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id
)
{
    cs_int32  i;
    cs_uint8  tbl_size =0;
    cs_status ret = CS_E_OK;

  
    UNI_PORT_CHECK(port_id);

   // tbl_size = (g_sw_tbl[port_id-1].g_mac_cnt >4)?(RTK_ACL_CLS_MAC_PORT_LENGTH -g_sw_tbl[port_id-1].g_mac_cnt): RTK_ACL_CLS_PORT_LENGTH;
    tbl_size = RTK_ACL_CLS_MAC_PORT_LENGTH -g_sw_tbl[port_id-1].g_mac_cnt;
    ret = class_rule_tbl_clr_record(port_id);
    if (ret) 
        return ret;
    
    for(i=0; i< tbl_size; i++)
    {       
        ret =__sw_acl_del(port_id, i);
        if (ret) 
            return ret;
    }

    g_sw_tbl[port_id-1].g_cls_cnt = 0;

    ret =__sw_acl_def(FALSE);
       
    return ret;      
}


cs_status sdl_cls_init(void)
{

    memset(g_sw_tbl, 0, UNI_PORT_MAX*sizeof(sw_cls_mac_res_port_t));
    g_def_rule_en = 0;
    
    return CS_E_OK;    
}

