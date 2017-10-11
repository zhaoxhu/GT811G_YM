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

#include "sdl_macfilter.h"
#include "sdl_util.h"
#include "sdl_int.h"
#include "aal_cls.h"
#include "aal_mpcp.h"
#include "sdl_init.h"
#include "aal_util.h"
#include "sdl.h"
#include "sdl_mac_filterbind_util.h"
#include "rtk_api_ext.h"
#include "rtk_error.h"

// for mac filter/bind

extern sw_cls_mac_res_port_t g_sw_tbl[UNI_PORT_MAX]; 

#define MAC_PORT_TBL_SIZE(port) (RTK_ACL_CLS_MAC_PORT_LENGTH -(g_sw_tbl[(port)-1].g_cls_cnt))

static cs_uint32  g_macbind_portmask;


static cs_status __mac_acl_add(
    CS_IN cs_uint8                     ruleindex,
    CS_IN cs_port_id_t                 port_id,
    CS_IN cs_sdl_mac_tbl_entry_t       *pmac,
    CS_IN cs_sdl_mac_filterbind_flag_t flag
)     
{
    rtk_filter_cfg_t   cfg;
    rtk_filter_action_t act;
    rtk_filter_field_t field;
    rtk_api_ret_t  retVal;
    cs_uint8  index;
    cs_uint32 ruleNum =0;
    
      
    if(pmac == NULL)
    {    
        SDL_MIN_LOG("pmac is NULL.[%s %d]\n", __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }
      
    memset(&cfg, 0, sizeof(rtk_filter_cfg_t));
    memset(&act, 0, sizeof(rtk_filter_action_t));
    memset(&field, 0, sizeof(rtk_filter_field_t));
         
    field.fieldType = (pmac->mac_type == SDL_SRC_MAC)? FILTER_FIELD_SMAC: FILTER_FIELD_DMAC;   
    field.filter_pattern_union.mac.dataType = FILTER_FIELD_DATA_MASK;
       
    for(index = 0; index < CS_MACADDR_LEN; index++){
        field.filter_pattern_union.mac.value.octet[index] = pmac->mac.addr[index];
        field.filter_pattern_union.mac.mask.octet[index]  = 0xff;
    }
    
    field.next = NULL;   
              
    retVal = rtk_filter_igrAcl_field_add(&cfg, &field);
    if(retVal){
      return CS_E_ERROR;
    }
                        
    cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
    cfg.activeport.value = (1 << (port_id-1));
    cfg.activeport.mask  = 0xFF;   
    cfg.invert = FALSE; 
    
    if(flag == SDL_PORT_MAC_FILTER)
    {
        act.actEnable[FILTER_ENACT_DROP] = TRUE;
    }
    else
    {
        act.actEnable[FILTER_ENACT_ADD_DSTPORT] = TRUE;
        act.filterAddDstPortmask = 0;
    }

    retVal  = rtk_filter_igrAcl_cfg_del(ruleindex); 
    retVal += rtk_filter_igrAcl_cfg_add(ruleindex, &cfg, &act, &ruleNum);
    if(retVal){
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

static cs_status __mac_acl_del( CS_IN cs_uint8  ruleindex )     
{   
    rtk_api_ret_t  retVal;
 
      
    retVal = rtk_filter_igrAcl_cfg_del(ruleindex); 
    if(retVal){
        return CS_E_ERROR;
    }
    
    return CS_E_OK;
}

/* Beacust VLAN transparent mode use ALWAYS MATH rule to achieve function,
 so API"rtk_filter_igrAcl_unmatchAction_set" is invaild for MAC BINDing.
 Now we use ALWAYS MATH rule to discard those packets which is unmatch 
 MAC BINDing table on enabling MAC BINDING UNI port. 
 The rule's priority must be higher than VLAN'rule(63).
*/
static cs_status __mac_bind_acl_deny(
     CS_IN cs_port_id_t              portid,
     CS_IN cs_boolean                enable
)     
{
    rtk_api_ret_t rt;
    rtk_filter_field_t  field;
    rtk_filter_cfg_t     cfg;
    rtk_filter_action_t  act; 
    rtk_filter_number_t  ruleNum;
    

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
            
    if(enable)    
    {
        if(g_macbind_portmask &(1 << (portid-1)))
            return CS_E_OK;

        g_macbind_portmask |= (1 << (portid-1));
            
        rt = rtk_filter_igrAcl_field_add(&cfg, &field);       
        if(rt)
            return CS_E_ERROR;
    
        act.actEnable[FILTER_ENACT_DROP] = TRUE;
        
        cfg.invert = FALSE;    
        cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
        cfg.activeport.value = g_macbind_portmask;
        cfg.activeport.mask = 0xFF;  
        
        rt = rtk_filter_igrAcl_cfg_del(RTK_ACL_MAC_DENY);
        rt += rtk_filter_igrAcl_cfg_add(RTK_ACL_MAC_DENY, &cfg, &act, &ruleNum); 
        if(rt)
            return CS_E_ERROR;
    }
    else
    {
        if(0==(g_macbind_portmask &(1 << (portid-1))))
            return CS_E_OK;

        g_macbind_portmask &= ~(1<<(portid-1));

        if(0 == g_macbind_portmask)
        {    
            rt = rtk_filter_igrAcl_cfg_del(RTK_ACL_MAC_DENY);
            if(rt)
                return CS_E_ERROR;            
        }
        else
        {
            rt = rtk_filter_igrAcl_field_add(&cfg, &field);       
            if(rt)
                return CS_E_ERROR;
            
            act.actEnable[FILTER_ENACT_DROP] = TRUE;
            
            cfg.invert = FALSE;    
            cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
            cfg.activeport.value = g_macbind_portmask;
            cfg.activeport.mask = 0xFF;  
            
            rt = rtk_filter_igrAcl_cfg_del(RTK_ACL_MAC_DENY);
            rt += rtk_filter_igrAcl_cfg_add(RTK_ACL_MAC_DENY, &cfg, &act, &ruleNum); 
            if(rt)
                return CS_E_ERROR;
        }
    }
   
    return CS_E_OK;
}


cs_status epon_request_onu_mac_filter_add_entry(
    CS_IN cs_callback_context_t    context,
    CS_IN cs_int32              device_id,
    CS_IN cs_int32                llidport,
    CS_IN cs_port_id_t             port_id,
    CS_IN cs_uint8                 num,
    CS_IN cs_sdl_mac_tbl_entry_t   *mac_list
)
{  
    cs_int32                i           =0;
    cs_uint8                new_cnt     =0;
    cs_uint8                aclindex    =0;
    cs_uint8                entry_count =0;  
    cs_status               ret = CS_E_OK;
    cs_sdl_mac_tbl_entry_t  *pmac = NULL;
    cs_sdl_mac_filterbind_flag_t  flag = SDL_PORT_MAC_INVLAD;
    cs_sdl_mac_filterbind_entry_t filterbind_entry;

    
    UNI_PORT_CHECK(port_id);
   
    if((num > MAC_PORT_TBL_SIZE(port_id))||(mac_list == NULL))
    {    
        SDL_MIN_LOG("para error. num : %d. [%s %d]\n", num,  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    mac_filterbind_status_get(port_id, &flag, &entry_count);
    
    if( SDL_PORT_MAC_BIND == flag)  
        return CS_E_RESOURCE;
    else if((SDL_PORT_MAC_INVLAD == flag)&&(0 != entry_count))
        return CS_E_CONFLICT;
    
    memset(&filterbind_entry, 0, sizeof(cs_sdl_mac_filterbind_entry_t));
     
    pmac = mac_list;

    if(0 == entry_count)
        new_cnt = num; 
    else
    {
        for(i=0; i<num; i++)
        {    
            // only search mac, VLANID is useless.
            if( pmac->mac_type > SDL_SRC_MAC)
            {   
                SDL_MIN_LOG("para error. mac_type : %d. [%s %d]\n", pmac->mac_type,  __FUNCTION__, __LINE__);
                return CS_E_PARAM;
            }
            if(pmac->mac.addr[0] & 0x1)
            {
                SDL_MIN_LOG("invalid unicast mac address. FILE: %s, LINE: %d", __FILE__, __LINE__);
                return CS_E_PARAM;
            }
            filterbind_entry.mac_type = pmac->mac_type;
            memcpy(&(filterbind_entry.mac), &(pmac->mac), sizeof(cs_mac_t));
            
            if(!mac_find_filterbindrecord(port_id,&filterbind_entry,&aclindex))
            {
                new_cnt++;
            }
            pmac++;
        }
    }
    
    // all MAC entries are in table.
    if(new_cnt ==0)
        return CS_E_OK;

    //no enough resource in mac privilege table 
    if(new_cnt>(MAC_PORT_TBL_SIZE(port_id)-entry_count))
    {   
        SDL_MIN_LOG("new_cnt: %d, MAC entry_count : %d. [%s %d]\n", new_cnt, entry_count,  __FUNCTION__, __LINE__);
        return CS_E_RESOURCE;
    }

    // first add, enable MAC filter on UNI port
    if((SDL_PORT_MAC_INVLAD == flag)&&(0 == entry_count))
    {
        mac_filterbind_status_set(port_id, SDL_PORT_MAC_FILTER);
        if(rtk_filter_igrAcl_unmatchAction_set(port_id-1, FILTER_UNMATCH_PERMIT)!= RT_ERR_OK)
        {   
            SDL_MIN_LOG("RTK ACL SET unmatchAction error. [%s %d]\n", __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
    }

    memset(&filterbind_entry, 0, sizeof(cs_sdl_mac_filterbind_entry_t));
    aclindex = 0;
    new_cnt  = 0;
    pmac = mac_list;
        
    for(i=0; i<num; i++)
    {     
        filterbind_entry.mac_type = pmac->mac_type;
        memcpy(&(filterbind_entry.mac), &(pmac->mac), sizeof(cs_mac_t));
        if(!mac_find_filterbindrecord(port_id,&filterbind_entry,&aclindex))
        {
            new_cnt++;
            /* acl index : 11,10,9,8-->0 */
            aclindex = (cs_uint8)((port_id*RTK_ACL_CLS_MAC_PORT_LENGTH)+RTK_ACL_CLS_MAC_START-entry_count-new_cnt);
            
            ret = __mac_acl_add( aclindex, port_id, pmac, SDL_PORT_MAC_FILTER);   
            if( ret)
            {   
                SDL_MIN_LOG("__mac_acl_add error. i:%d. [%s %d]\n", i, __FUNCTION__, __LINE__);
                return ret;
            }

            filterbind_entry.vlanid   = pmac->vlanid;
            filterbind_entry.hw_index = aclindex;

            ret = mac_add_filterbindrecord(port_id, &filterbind_entry);
            if( ret)
            {   
                SDL_MIN_LOG("mac add record. i:%d.[%s %d]\n", i,__FUNCTION__, __LINE__);
                return ret;
            }
        }
        pmac++;
        
    }

    mac_filterbind_status_get(port_id, &flag, &entry_count);
    
    g_sw_tbl[port_id-1].g_mac_cnt = entry_count;
       
    return CS_E_OK;       
}

cs_status epon_request_onu_mac_filter_del_entry(
    CS_IN cs_callback_context_t    context,
    CS_IN cs_int32              device_id,
    CS_IN cs_int32                llidport,
    CS_IN cs_port_id_t             port_id,
    CS_IN cs_uint8                 num,
    CS_IN cs_sdl_mac_tbl_entry_t   *mac_list
)
{
    cs_uint16               i  =0;
    cs_uint8                aclindex    =0;
    cs_uint8                entry_count =0;  
    cs_status               ret = CS_E_OK;
    cs_sdl_mac_tbl_entry_t  *pmac = NULL;
    cs_sdl_mac_filterbind_flag_t  flag = SDL_PORT_MAC_INVLAD;
    cs_sdl_mac_filterbind_entry_t filterbind_entry;
    cs_sdl_mac_filterbind_entry_t tail_entry;
    cs_sdl_mac_tbl_entry_t        entry;         

     
    UNI_PORT_CHECK(port_id);
 
    if((num > MAC_PORT_TBL_SIZE(port_id))||(mac_list == NULL))
    {    
        SDL_MIN_LOG("para error. num : %d. [%s %d]\n", num,  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    mac_filterbind_status_get(port_id, &flag, &entry_count);
    
    if( SDL_PORT_MAC_BIND == flag)  
        return CS_E_RESOURCE;
    else if((SDL_PORT_MAC_INVLAD == flag)&&(0 != entry_count))
        return CS_E_CONFLICT;
    else if(0 == entry_count)
        return CS_E_OK;
     
    pmac = mac_list;

    for(i=0; i<num; i++)
    {    
        // only search mac, VLANID is useless.
        if( pmac->mac_type > SDL_SRC_MAC)
        {   
            SDL_MIN_LOG("para error. mac_type : %d. [%s %d]\n", pmac->mac_type,  __FUNCTION__, __LINE__);
            return CS_E_PARAM;
        }
        if(pmac->mac.addr[0] & 0x1)
        {
            SDL_MIN_LOG("invalid unicast mac address. FILE: %s, LINE: %d", __FILE__, __LINE__);
            return CS_E_PARAM;
        }
 
        pmac++;
    }
 
    memset(&filterbind_entry, 0, sizeof(cs_sdl_mac_filterbind_entry_t));
    memset(&tail_entry, 0, sizeof(cs_sdl_mac_filterbind_entry_t));
    memset(&entry, 0, sizeof(cs_sdl_mac_tbl_entry_t));
    aclindex = 0;
    pmac = mac_list;
    
    for(i=0; i<num; i++)
    {   
        filterbind_entry.mac_type = pmac->mac_type;
        memcpy(&(filterbind_entry.mac), &(pmac->mac), sizeof(cs_mac_t));
            
        if(mac_find_filterbindrecord(port_id,&filterbind_entry,&aclindex))
        {   
            if(!mac_find_tail_filterbindrecord(port_id,&tail_entry))
            {   
                SDL_MIN_LOG("get tail_entry error. i:%d. [%s %d]\n", i, __FUNCTION__, __LINE__);
                return CS_E_ERROR;
            }
             
            /* move tail entry to the deleted entry in HW*/
            if(tail_entry.hw_index == aclindex)
            {
                ret = __mac_acl_del( aclindex);  
            }
            else
            {   
                entry.mac_type = tail_entry.mac_type; 
                memcpy(&(entry.mac), &(tail_entry.mac), sizeof(cs_mac_t));             
                ret =  __mac_acl_add(aclindex, port_id, &entry, SDL_PORT_MAC_FILTER);
                ret += __mac_acl_del(tail_entry.hw_index);
                tail_entry.hw_index = aclindex;
                ret += mac_save_tail_filterbindrecord(port_id,&tail_entry);
            }          
            if( ret)
            {   
                SDL_MIN_LOG("__mac_acl_del error. i:%d. [%s %d]\n", i, __FUNCTION__, __LINE__);
                return CS_E_ERROR;
            }
                
            ret = mac_del_filterbindrecord(port_id, &filterbind_entry);
            if( ret)
            {   
                SDL_MIN_LOG("mac del record. i:%d.[%s %d]\n", i,__FUNCTION__, __LINE__);
                return ret;
            }
        }
         
        pmac++;
    }

    mac_filterbind_status_get(port_id, &flag, &entry_count);
    g_sw_tbl[port_id-1].g_mac_cnt = entry_count;
    if(0 == entry_count)
    {
        mac_filterbind_status_set(port_id, SDL_PORT_MAC_INVLAD);
    }
   
    return CS_E_OK;
}

cs_status epon_request_onu_mac_filter_clr_entry(
    CS_IN cs_callback_context_t    context,
    CS_IN cs_int32              device_id,
    CS_IN cs_int32                llidport,
    CS_IN cs_port_id_t             port_id
)
{   
    cs_uint16               i =0;
    cs_uint8                entry_count =0;  
    cs_status               ret = CS_E_OK;
    cs_sdl_mac_filterbind_flag_t  flag = SDL_PORT_MAC_INVLAD;
    cs_sdl_mac_filterbind_entry_t filterbind_entry[RTK_ACL_CLS_MAC_PORT_LENGTH];
    

    UNI_PORT_CHECK(port_id);

    mac_filterbind_status_get(port_id, &flag, &entry_count);

    if( SDL_PORT_MAC_BIND == flag)  
        return CS_E_RESOURCE;
    else if((SDL_PORT_MAC_INVLAD == flag)&&(0 != entry_count))
        return CS_E_CONFLICT;
    else if(0 == entry_count)
        return CS_E_OK;

    memset(&filterbind_entry[0], 0, RTK_ACL_CLS_MAC_PORT_LENGTH*sizeof(cs_sdl_mac_filterbind_entry_t));


    ret = mac_get_filterbindrecord(port_id, &entry_count, filterbind_entry);
    if( ret)
    {   
        SDL_MIN_LOG("mac get record error.[%s %d]\n",__FUNCTION__, __LINE__);
        return ret;
    }
        
    for(i=0; i<entry_count; i++)
    {         
        ret = __mac_acl_del( filterbind_entry[i].hw_index);   
        if( ret)
        {   
            SDL_MIN_LOG("__mac_acl_del error. i:%d. [%s %d]\n", i, __FUNCTION__, __LINE__);
            return ret;
        }
                
        ret = mac_del_filterbindrecord(port_id, &filterbind_entry[i]);
        if( ret)
        {   
            SDL_MIN_LOG("mac del record. i:%d.[%s %d]\n", i,__FUNCTION__, __LINE__);
            return ret;
        }
    }
     
    mac_filterbind_status_set(port_id, SDL_PORT_MAC_INVLAD);
    g_sw_tbl[port_id-1].g_mac_cnt = 0;

    return CS_E_OK;
}

cs_status epon_request_onu_mac_filter_get_entry(
    CS_IN  cs_callback_context_t      context,
    CS_IN  cs_int32                device_id,
    CS_IN  cs_int32                  llidport,
    CS_IN  cs_port_id_t               port_id,
    CS_OUT cs_uint8                   *number,
    CS_OUT cs_sdl_mac_tbl_entry_t     *mac_list
)
{    
    cs_uint16               i =0;
    cs_uint8                entry_count =0;  
    cs_status               ret = CS_E_OK;
    cs_sdl_mac_filterbind_flag_t  flag = SDL_PORT_MAC_INVLAD;
    cs_sdl_mac_filterbind_entry_t filterbind_entry[RTK_ACL_CLS_MAC_PORT_LENGTH];
    
    UNI_PORT_CHECK(port_id);

    if((number == NULL)||(mac_list == NULL))
    {    
        SDL_MIN_LOG("para error. NULL point. [%s %d]\n", __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    mac_filterbind_status_get(port_id, &flag, &entry_count);

    if( SDL_PORT_MAC_BIND == flag)  
        return CS_E_RESOURCE;
    else if((SDL_PORT_MAC_INVLAD == flag)&&(0 != entry_count))
        return CS_E_CONFLICT;
    else if(0 == entry_count)
    {
        *number = 0;
        return CS_E_OK;
    }
    
    memset(&filterbind_entry[0], 0, RTK_ACL_CLS_MAC_PORT_LENGTH*sizeof(cs_sdl_mac_filterbind_entry_t));
    
    ret = mac_get_filterbindrecord(port_id, &entry_count, filterbind_entry);
    if( ret)
    {   
        SDL_MIN_LOG("mac get record error.[%s %d]\n",__FUNCTION__, __LINE__);
        return ret;
    }

    for(i=0; i<entry_count; i++)
    {         
        mac_list->mac_type = filterbind_entry[i].mac_type;
        mac_list->vlanid   = filterbind_entry[i].vlanid;
        memcpy(&(mac_list->mac), &(filterbind_entry[i].mac), sizeof(cs_mac_t));
        
        mac_list++;
    }
    *number = entry_count;
    
    return CS_E_OK;
}

cs_status epon_request_onu_mac_bind_add_entry(
    CS_IN cs_callback_context_t    context,
    CS_IN cs_int32              device_id,
    CS_IN cs_int32                llidport,
    CS_IN cs_port_id_t             port_id,
    CS_IN cs_uint8                 num,
    CS_IN cs_sdl_mac_tbl_entry_t   *mac_list
)
{  
    cs_int32                i           =0;
    cs_uint8                new_cnt     =0;
    cs_uint8                aclindex    =0;
    cs_uint8                entry_count =0;  
    cs_status               ret = CS_E_OK;
    cs_sdl_mac_tbl_entry_t  *pmac = NULL;
    cs_sdl_mac_filterbind_flag_t  flag = SDL_PORT_MAC_INVLAD;
    cs_sdl_mac_filterbind_entry_t filterbind_entry;

    
    UNI_PORT_CHECK(port_id);

    if((num > MAC_PORT_TBL_SIZE(port_id))||(mac_list == NULL))
    {    
        SDL_MIN_LOG("para error. num : %d. [%s %d]\n", num,  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    mac_filterbind_status_get(port_id, &flag, &entry_count);
    
    if( SDL_PORT_MAC_FILTER == flag)  
        return CS_E_RESOURCE;
    else if((SDL_PORT_MAC_INVLAD == flag)&&(0 != entry_count))
        return CS_E_CONFLICT;
    
    memset(&filterbind_entry, 0, sizeof(cs_sdl_mac_filterbind_entry_t));
     
    pmac = mac_list;

    if(0 == entry_count)
        new_cnt = num; 
    else
    {
        for(i=0; i<num; i++)
        {    
            // only search mac, VLANID is useless.
            if( pmac->mac_type != SDL_SRC_MAC)
            {   
                SDL_MIN_LOG("para error. mac_type : %d. [%s %d]\n", pmac->mac_type,  __FUNCTION__, __LINE__);
                return CS_E_PARAM;
            }
            if(pmac->mac.addr[0] & 0x1)
            {
                SDL_MIN_LOG("invalid unicast mac address. FILE: %s, LINE: %d", __FILE__, __LINE__);
                return CS_E_PARAM;
            }
            filterbind_entry.mac_type = pmac->mac_type;
            memcpy(&(filterbind_entry.mac), &(pmac->mac), sizeof(cs_mac_t));
            
            if(!mac_find_filterbindrecord(port_id,&filterbind_entry,&aclindex))
            {
                new_cnt++;
            }
            pmac++;
        }
    }

    //no enough resource in mac privilege table 
    if(new_cnt>(MAC_PORT_TBL_SIZE(port_id)-entry_count))
    {   
        SDL_MIN_LOG("new_cnt: %d, MAC entry_count : %d. [%s %d]\n", new_cnt, entry_count,  __FUNCTION__, __LINE__);
        return CS_E_RESOURCE;
    }

    /* first add, enable MAC bind on UNI port */
    if((SDL_PORT_MAC_INVLAD == flag)&&(0 == entry_count))
    {
        mac_filterbind_status_set(port_id, SDL_PORT_MAC_BIND);
        if(rtk_filter_igrAcl_unmatchAction_set(port_id-1, FILTER_UNMATCH_DROP)!= RT_ERR_OK)
        {   
            SDL_MIN_LOG("RTK ACL SET unmatchAction error. [%s %d]\n", __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
        /*workaround for Bug 29555  */
        ret =__mac_bind_acl_deny(port_id, TRUE);
        if(ret)
        {   
            SDL_MIN_LOG("__mac_bind_acl_deny error.[%s %d]\n", __FUNCTION__, __LINE__);
            return ret;
        }
    }

    // all MAC entries are in table.
    if(new_cnt ==0){
        return CS_E_OK;
    }


    memset(&filterbind_entry, 0, sizeof(cs_sdl_mac_filterbind_entry_t));
    aclindex = 0;
    new_cnt  = 0;
    pmac = mac_list;
        
    for(i=0; i<num; i++)
    {       
        filterbind_entry.mac_type = pmac->mac_type;
        memcpy(&(filterbind_entry.mac), &(pmac->mac), sizeof(cs_mac_t));
        if(!mac_find_filterbindrecord(port_id,&filterbind_entry,&aclindex))
        {   
            new_cnt++;
            /* acl index : 11,10,9,8-->0 */
            aclindex = (cs_uint8)((port_id*RTK_ACL_CLS_MAC_PORT_LENGTH)+RTK_ACL_CLS_MAC_START-entry_count-new_cnt);
            
            ret = __mac_acl_add( aclindex, port_id, pmac, SDL_PORT_MAC_BIND);   
            if( ret)
            {   
                SDL_MIN_LOG("__mac_acl_add error. i:%d. [%s %d]\n", i, __FUNCTION__, __LINE__);
                return ret;
            }
 
            filterbind_entry.vlanid   = pmac->vlanid;
            filterbind_entry.hw_index = aclindex;
              
            ret = mac_add_filterbindrecord(port_id, &filterbind_entry);
            if( ret)
            {   
                SDL_MIN_LOG("mac add record. i:%d.[%s %d]\n", i,__FUNCTION__, __LINE__);
                return ret;
            }
        }
        pmac++;
        
    }

    mac_filterbind_status_get(port_id, &flag, &entry_count);
    
    g_sw_tbl[port_id-1].g_mac_cnt = entry_count;
    
    return CS_E_OK;       
}

cs_status epon_request_onu_mac_bind_del_entry(
    CS_IN cs_callback_context_t    context,
    CS_IN cs_int32              device_id,
    CS_IN cs_int32                llidport,
    CS_IN cs_port_id_t             port_id,
    CS_IN cs_uint8                 num,
    CS_IN cs_sdl_mac_tbl_entry_t   *mac_list
)
{
    cs_uint16               i  =0;
    cs_uint8                aclindex    =0;
    cs_uint8                entry_count =0;  
    cs_status               ret = CS_E_OK;
    cs_sdl_mac_tbl_entry_t  *pmac = NULL;
    cs_sdl_mac_filterbind_flag_t  flag = SDL_PORT_MAC_INVLAD;
    cs_sdl_mac_filterbind_entry_t filterbind_entry;
    cs_sdl_mac_filterbind_entry_t tail_entry;
    cs_sdl_mac_tbl_entry_t        entry;      

     
    UNI_PORT_CHECK(port_id);

    if((num > MAC_PORT_TBL_SIZE(port_id))||(mac_list == NULL))
    {    
        SDL_MIN_LOG("para error. num : %d. [%s %d]\n", num,  __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    mac_filterbind_status_get(port_id, &flag, &entry_count);
    
    if( SDL_PORT_MAC_FILTER == flag)  
        return CS_E_RESOURCE;
    else if((SDL_PORT_MAC_INVLAD == flag)&&(0 != entry_count))
        return CS_E_CONFLICT;
    else if(0 == entry_count)
        return CS_E_OK;
    
    pmac = mac_list;

    for(i=0; i<num; i++)
    {    
        // only search mac, VLANID is useless.
        if( pmac->mac_type != SDL_SRC_MAC)
        {   
            SDL_MIN_LOG("para error. mac_type : %d. [%s %d]\n", pmac->mac_type,  __FUNCTION__, __LINE__);
            return CS_E_PARAM;
        }
        if(pmac->mac.addr[0] & 0x1)
        {
            SDL_MIN_LOG("invalid unicast mac address. FILE: %s, LINE: %d", __FILE__, __LINE__);
            return CS_E_PARAM;
        }
 
        pmac++;
    }
   
    memset(&filterbind_entry, 0, sizeof(cs_sdl_mac_filterbind_entry_t));
    memset(&tail_entry, 0, sizeof(cs_sdl_mac_filterbind_entry_t));
    memset(&entry, 0, sizeof(cs_sdl_mac_tbl_entry_t));
    aclindex = 0;
    pmac = mac_list;
    
    for(i=0; i<num; i++)
    {   
        filterbind_entry.mac_type = pmac->mac_type;
        memcpy(&(filterbind_entry.mac), &(pmac->mac), sizeof(cs_mac_t));
            
        if(mac_find_filterbindrecord(port_id,&filterbind_entry,&aclindex))
        {   
            if(!mac_find_tail_filterbindrecord(port_id,&tail_entry))
            {   
                SDL_MIN_LOG("get tail_entry error. i:%d. [%s %d]\n", i, __FUNCTION__, __LINE__);
                return CS_E_ERROR;
            }
            /* move tail entry to the deleted entry in HW*/
            if(tail_entry.hw_index == aclindex)
            {
                 ret = __mac_acl_del( aclindex);  
            }
            else
            {   
                entry.mac_type = tail_entry.mac_type; 
                memcpy(&(entry.mac), &(tail_entry.mac), sizeof(cs_mac_t));             
                ret =  __mac_acl_add(aclindex, port_id, &entry, SDL_PORT_MAC_BIND);
                ret += __mac_acl_del(tail_entry.hw_index);
                tail_entry.hw_index = aclindex;
                ret += mac_save_tail_filterbindrecord(port_id,&tail_entry);
            }
            if( ret)
            {   
                SDL_MIN_LOG("__mac_acl_del error. i:%d. [%s %d]\n", i, __FUNCTION__, __LINE__);
                return CS_E_ERROR;
            }
                    
            ret = mac_del_filterbindrecord(port_id, &filterbind_entry);
            if( ret)
            {   
                SDL_MIN_LOG("mac del record. i:%d.[%s %d]\n", i,__FUNCTION__, __LINE__);
                return ret;
            }
        }
             
        pmac++;
    }

    mac_filterbind_status_get(port_id, &flag, &entry_count);
    g_sw_tbl[port_id-1].g_mac_cnt = entry_count;
    
    if(0 == entry_count)
    {
        mac_filterbind_status_set(port_id, SDL_PORT_MAC_INVLAD);
        if(rtk_filter_igrAcl_unmatchAction_set(port_id-1, FILTER_UNMATCH_PERMIT)!= RT_ERR_OK)
        {   
            SDL_MIN_LOG("RTK ACL SET unmatchAction error. [%s %d]\n", __FUNCTION__, __LINE__);
            return CS_E_ERROR;
        }
          /*workaround for Bug 29555  */
        ret =__mac_bind_acl_deny(port_id, FALSE);
        if( ret)
        {   
            SDL_MIN_LOG("__mac_bind_acl_deny error.[%s %d]\n", __FUNCTION__, __LINE__);
            return ret;
        }
    }
   
    return CS_E_OK;
}

cs_status epon_request_onu_mac_bind_clr_entry(
    CS_IN cs_callback_context_t    context,
    CS_IN cs_int32              device_id,
    CS_IN cs_int32                llidport,
    CS_IN cs_port_id_t             port_id
)
{   
    cs_uint16               i =0;
    cs_uint8                entry_count =0;  
    cs_status               ret = CS_E_OK;
    cs_sdl_mac_filterbind_flag_t  flag = SDL_PORT_MAC_INVLAD;
    cs_sdl_mac_filterbind_entry_t filterbind_entry[RTK_ACL_CLS_MAC_PORT_LENGTH];
    

    UNI_PORT_CHECK(port_id);

    mac_filterbind_status_get(port_id, &flag, &entry_count);

    if( SDL_PORT_MAC_FILTER == flag)  
        return CS_E_RESOURCE;
    else if((SDL_PORT_MAC_INVLAD == flag)&&(0 != entry_count))
        return CS_E_CONFLICT;


     /*disable mac filter*/
     mac_filterbind_status_set(port_id, SDL_PORT_MAC_INVLAD);
     if(rtk_filter_igrAcl_unmatchAction_set(port_id-1, FILTER_UNMATCH_PERMIT)!= RT_ERR_OK)
     {   
         SDL_MIN_LOG("RTK ACL SET unmatchAction error. [%s %d]\n", __FUNCTION__, __LINE__);
         return CS_E_ERROR;
     }
    
    /*workaround for Bug 29555  */
     ret =__mac_bind_acl_deny(port_id, FALSE);
     if( ret)
     {   
         SDL_MIN_LOG("__mac_bind_acl_deny error.[%s %d]\n", __FUNCTION__, __LINE__);
         return ret;
     }
     

    memset(&filterbind_entry[0], 0, RTK_ACL_CLS_MAC_PORT_LENGTH*sizeof(cs_sdl_mac_filterbind_entry_t));

    ret = mac_get_filterbindrecord(port_id, &entry_count, filterbind_entry);
    if( ret)
    {   
        SDL_MIN_LOG("mac get record error.[%s %d]\n",__FUNCTION__, __LINE__);
        return ret;
    }
        
    for(i=0; i<entry_count; i++)
    {         
        ret = __mac_acl_del( filterbind_entry[i].hw_index);   
        if( ret)
        {   
            SDL_MIN_LOG("__mac_acl_del error. i:%d. rule:%d. [%s %d]\n",i, filterbind_entry[i].hw_index, __FUNCTION__, __LINE__);
            return ret;
        }
                
        ret = mac_del_filterbindrecord(port_id, &filterbind_entry[i]);
        if( ret)
        {   
            SDL_MIN_LOG("mac del record. i:%d.[%s %d]\n", i,__FUNCTION__, __LINE__);
            return ret;
        }
    }

    g_sw_tbl[port_id-1].g_mac_cnt = 0;
 

    return CS_E_OK;
}

cs_status epon_request_onu_mac_bind_get_entry(
    CS_IN  cs_callback_context_t      context,
    CS_IN  cs_int32                device_id,
    CS_IN  cs_int32                  llidport,
    CS_IN  cs_port_id_t                port_id,
    CS_OUT cs_uint8                   *number,
    CS_OUT cs_sdl_mac_tbl_entry_t     *mac_list
)
{    
    cs_uint16               i =0;
    cs_uint8                entry_count =0;  
    cs_status               ret = CS_E_OK;
    cs_sdl_mac_filterbind_flag_t  flag = SDL_PORT_MAC_INVLAD;
    cs_sdl_mac_filterbind_entry_t filterbind_entry[RTK_ACL_CLS_MAC_PORT_LENGTH];

    
    UNI_PORT_CHECK(port_id);

    if((number == NULL)||(mac_list == NULL))
    {    
        SDL_MIN_LOG("para error. NULL point. [%s %d]\n", __FUNCTION__, __LINE__);
        return CS_E_PARAM;
    }

    mac_filterbind_status_get(port_id, &flag, &entry_count);

    if( SDL_PORT_MAC_FILTER == flag)  
        return CS_E_RESOURCE;
    else if((SDL_PORT_MAC_INVLAD == flag)&&(0 != entry_count)){
        return CS_E_CONFLICT;
    }
    else if(0 == entry_count)
    {
        *number = 0;
        return CS_E_OK;
    }
    
    memset(&filterbind_entry[0], 0, RTK_ACL_CLS_MAC_PORT_LENGTH*sizeof(cs_sdl_mac_filterbind_entry_t));
    
    ret = mac_get_filterbindrecord(port_id, &entry_count, filterbind_entry);
    if( ret)
    {   
        SDL_MIN_LOG("mac get record error.[%s %d]\n",__FUNCTION__, __LINE__);
        return ret;
    }

    for(i=0; i<entry_count; i++)
    {         
        mac_list->mac_type = filterbind_entry[i].mac_type;
        mac_list->vlanid   = filterbind_entry[i].vlanid;
        memcpy(&(mac_list->mac), &(filterbind_entry[i].mac), sizeof(cs_mac_t));

        mac_list++;
    }
    *number = entry_count;
    
    return CS_E_OK;
}


cs_status sdl_macfilter_init(void)
{
    g_macbind_portmask = 0;
    
    sdl_mac_tbl_init();

    return CS_E_OK;
}


cs_status epon_request_onu_mac_filter_state_get ( cs_port_id_t port_id, cs_sdl_mac_filter_type_t * filter_type)
{
    cs_uint8   entry_count=0;
    cs_sdl_mac_filterbind_flag_t  flag;
    
    mac_filterbind_status_get( port_id, &flag, &entry_count);

    * filter_type =flag;

    return CS_E_OK;
}



