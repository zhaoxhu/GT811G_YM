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
#include "sdl_classify_util.h"
#include "plat_common.h"
#include "sdl.h"


#define  UNI_PORT_RANGE_CHECK(port)  ((port)>UNI_PORT_MAX)||((port)<CS_UNI_PORT_ID1)  

/********************************************************************
 *************************** Global variables ***********************
 ********************************************************************/
static classify_unified_rules_port_t g_port_ctc_rule_tbl[UNI_PORT_MAX];


#define CTC_PORT_KEY_ENTRY(port, x)      (g_port_ctc_rule_tbl[port-1].rule_entry[x])
#define CTC_PORT_KEY_ENTRY_KEYNUMS(port) (g_port_ctc_rule_tbl[port-1].rule_nums)

#define CTC_PORT_KEY_ENTRY_SAVE(port, x, conf) do {                            \
    CTC_PORT_KEY_ENTRY(port, x).precedence  = conf->precedence;                \
    CTC_PORT_KEY_ENTRY(port, x).queueMapped = conf->queueMapped;               \
    CTC_PORT_KEY_ENTRY(port, x).priMark     = conf->priMark;                   \
    CTC_PORT_KEY_ENTRY(port, x).entries     = conf->entries;                   \
    memcpy(CTC_PORT_KEY_ENTRY(port, x).fselect, conf->fselect, CLASS_FIELD_SELECT_MAX*sizeof(cs_sdl_cls_rule_t)); \
    }while(0)
#define CTC_PORT_KEY_ENTRY_MOVE(port, dst, src) do {                                        \
    CTC_PORT_KEY_ENTRY(port, dst).precedence  = CTC_PORT_KEY_ENTRY(port, src).precedence;   \
    CTC_PORT_KEY_ENTRY(port, dst).queueMapped = CTC_PORT_KEY_ENTRY(port, src).queueMapped;  \
    CTC_PORT_KEY_ENTRY(port, dst).priMark     = CTC_PORT_KEY_ENTRY(port, src).priMark;      \
    CTC_PORT_KEY_ENTRY(port, dst).entries     = CTC_PORT_KEY_ENTRY(port, src).entries;      \
    memcpy(CTC_PORT_KEY_ENTRY(port, dst).fselect, CTC_PORT_KEY_ENTRY(port, src).fselect, CLASS_FIELD_SELECT_MAX*sizeof(cs_sdl_cls_rule_t)); \
    }while(0)
#define CTC_PORT_KEY_ENTRY_CLR(port, x) do {           \
    CTC_PORT_KEY_ENTRY(port, x).precedence     = 0;    \
    CTC_PORT_KEY_ENTRY(port, x).queueMapped    = 0;    \
    CTC_PORT_KEY_ENTRY(port, x).priMark        = 0;    \
    CTC_PORT_KEY_ENTRY(port, x).entries        = 0;    \
    memset(CTC_PORT_KEY_ENTRY(port, x).fselect, 0, CLASS_FIELD_SELECT_MAX*sizeof(cs_sdl_cls_rule_t)); \
    }while(0)

static cs_boolean __class_find_rule_record ( CS_IN cs_port_id_t port_id, CS_IN cs_uint8 precedence)
{
    cs_int32 i;

    if(UNI_PORT_RANGE_CHECK(port_id))
    {  
        SDL_MIN_LOG("para error. port_id: %d. [%s %d]\n", port_id,  __FUNCTION__, __LINE__);
        return FALSE;
    }
          
    for(i=0; i<CTC_PORT_KEY_ENTRY_KEYNUMS(port_id); i++)
    {
        if(CTC_PORT_KEY_ENTRY(port_id, i).precedence == precedence)
        {        
            return TRUE;
        }
    }
     
    return FALSE;
}

cs_status class_rule_para_check(CS_IN cs_uint8 num, CS_IN cs_sdl_classification_t  *cfg )
{   
    cs_int32 i;
    cs_status ret = CS_E_OK;
    cs_sdl_classification_t  *ptemp =NULL;
        
    if((num>CLASS_RULES_MAX)||(num<1)||(cfg == NULL))
    {
        ret = CS_E_PARAM;
        goto end;
    }

    ptemp = cfg;
    
    for(i=0; i<num; i++)
    {
        if(ptemp->entries< 1|| ptemp->entries > CLASS_FIELD_SELECT_MAX){
            ret = CS_E_PARAM;
            goto end;
        }
        if(ptemp->queueMapped > CLASS_MAX_QUEUE_ID){
            ret = CS_E_PARAM;
            goto end;
        }
        if((ptemp->priMark > CLASS_PRIMARKING_MAX)&&(ptemp->priMark != 0xff)){
            ret = CS_E_PARAM;
            goto end;
        }       
        ptemp++;
    }

end:
    return ret;       
}

cs_status class_field_para_check(CS_IN cs_uint8 entry_num, CS_IN cs_sdl_cls_rule_t *cfg, CS_OUT cs_uint32 *masktemp)
{  
    cs_int32 i;
    cs_uint8 zeromac[] = {0, 0, 0, 0, 0, 0};
    cs_uint16 vlanid =0;
    cs_uint32 mask =0; // for Multiple fields
    cs_uint32 ipv4temp =0;
    cs_uint32 ipv6temp =0;
    cs_uint32 ip =0;
    cs_uint32 flowlable=0;
    cs_status ret = CS_E_OK;
    cs_sdl_cls_rule_t  *ptemp =NULL;

    
    if((entry_num>CLASS_FIELD_SELECT_MAX)||(entry_num<1)||(cfg == NULL)||(masktemp == NULL))
    {
        ret = CS_E_PARAM;
        goto end;
    }

    ptemp = cfg;

    /* Multiple fields check */
    for(i=0; i<entry_num; i++)
    {        
        /* we only support 0x1 (field euqal to value) */
        if (ptemp->validationOper != CLASS_VALIDATION_OPER_EQL)   
        {
            ret = CS_E_PARAM;
            goto end;
        }
        
        switch(ptemp->fieldSelect)
        {
            case CLASS_RULES_FSELECT_DA_MAC:  
            {
                if (memcmp(&(ptemp->matchValue[CLASS_MATCH_VAL_LEN - 6]), zeromac, sizeof(zeromac)) == 0)
                {
                    SDL_MIN_LOG("illegal da mac, L%d\n", __LINE__);
                    ret = CS_E_PARAM;
                    goto end;
                }            
                break;
            }
            case CLASS_RULES_FSELECT_SA_MAC:   
            {
                if (memcmp(&(ptemp->matchValue[CLASS_MATCH_VAL_LEN - 6]), zeromac, sizeof(zeromac)) == 0)
                {
                    SDL_MIN_LOG("illegal sa mac, L%d\n", __LINE__);
                    ret = CS_E_PARAM;
                    goto end;
                }  
                break;
            }
            case CLASS_RULES_FSELECT_802_1P:     
            {
                if (ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1] > 7)
                {
                    SDL_MIN_LOG("illegal 802_1P value, L%d\n", __LINE__);
                    ret = CS_E_PARAM;
                    goto end;
                }
                break;
            }
            case CLASS_RULES_FSELECT_VLAN_ID:    
            {    
                vlanid = (((ptemp->matchValue[CLASS_MATCH_VAL_LEN - 2]&0xf)<<8)|ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]);    
                if (vlanid < 1 || vlanid > 4095)
                {
                    SDL_MIN_LOG("illegal vlan id, L%d\n", __LINE__);
                    ret = CS_E_PARAM;
                    goto end;
                }
                break;
            }    
            case CLASS_RULES_FSELECT_DA_IPV4:
            {       
                memcpy(&ip, &(ptemp->matchValue[CLASS_MATCH_VAL_LEN - 4]), sizeof(cs_uint32));               
                if (ip == 0)
                {
                    SDL_MIN_LOG("illegal ipda, L%d\n", __LINE__);
                    ret = CS_E_PARAM;
                    goto end;
                }
                break;
            }
            case CLASS_RULES_FSELECT_SA_IPV4:
            {       
                memcpy(&ip, &(ptemp->matchValue[CLASS_MATCH_VAL_LEN - 4]), sizeof(cs_uint32));             
                if (ip == 0)
                {
                    SDL_MIN_LOG("illegal ipsa, L%d\n", __LINE__);
                    ret = CS_E_PARAM;
                    goto end;
                }
                break;
            }
            case CLASS_RULES_FSELECT_TOS_DSCP:
            {               
                if (ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1] > 63)
                {
                    SDL_MIN_LOG("illegal dscp, L%d\n", __LINE__);
                    ret = CS_E_PARAM;
                    goto end;
                }
                break;        
            }
            case CLASS_RULES_FSELECT_IPVER:  
            {   
                if ((ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1] != 4)&&
                    (ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1] != 6))
                {
                    SDL_MIN_LOG("illegal IP version, L%d\n", __LINE__);
                    ret = CS_E_PARAM;
                    goto end;
                }                
                break;        
            }
            case CLASS_RULES_FSELECT_IPV6_FLOWLABEL:  
            {   
                flowlable = ((ptemp->matchValue[CLASS_MATCH_VAL_LEN - 3]<<16)|(ptemp->matchValue[CLASS_MATCH_VAL_LEN - 2]<<8)|
                              ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1]);
                if (flowlable > 0xfffff)
                {
                    SDL_MIN_LOG("illegal IPV6 flow label, L%d\n", __LINE__);
                    ret = CS_E_PARAM;
                    goto end;
                }                
                break;        
            }   
            case CLASS_RULES_FSELECT_DA_IPV6_PREFIX:  
            {    
                if (ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1] > 128)
                {
                    SDL_MIN_LOG("illegal DA_IPV6 PREFIX, L%d\n", __LINE__);
                    ret = CS_E_PARAM;
                    goto end;
                } 
                break;        
            }
            case CLASS_RULES_FSELECT_SA_IPV6_PREFIX: 
            {   
                if (ptemp->matchValue[CLASS_MATCH_VAL_LEN - 1] > 128)
                {
                    SDL_MIN_LOG("illegal SA_IPV6 PREFIX, L%d\n", __LINE__);
                    ret = CS_E_PARAM;
                    goto end;
                }
                break;        
            }
            case CLASS_RULES_FSELECT_ETH_TYPE:
            case CLASS_RULES_FSELECT_IPPRTO:
            case CLASS_RULES_FSELECT_IPV6_TC:     
            case CLASS_RULES_FSELECT_L4_SRC_PORT: 
            case CLASS_RULES_FSELECT_L4_DST_PORT:  
            case CLASS_RULES_FSELECT_DA_IPV6:  
            case CLASS_RULES_FSELECT_SA_IPV6:  
            case CLASS_RULES_FSELECT_IPV6_NEXT_HEADER:  
            {                                           
                break;        
            }    
            default:
            {
                SDL_MIN_LOG("error, unexpected fiedSelect:%d, L%d\n", ptemp->fieldSelect, __LINE__);
                ret = CS_E_PARAM;
                goto end;
            }
        }  

        /* a field can't be configured more than once in a rule*/
        if(mask&(1<< (ptemp->fieldSelect)))
        {
            ret = CS_E_PARAM;
            goto end;
        }
        mask |= (1<< (ptemp->fieldSelect)); 
        ptemp++;
    }

    /* check Multiple fields Configuration conflicts */
    if(entry_num>1)
    {
        ipv4temp = ((1<<CLASS_RULES_FSELECT_DA_IPV4)|(1<<CLASS_RULES_FSELECT_SA_IPV4)|
                    (1<<CLASS_RULES_FSELECT_IPPRTO)|(1<<CLASS_RULES_FSELECT_TOS_DSCP));
        ipv6temp = ((1<<CLASS_RULES_FSELECT_IPV6_TC)|(1<<CLASS_RULES_FSELECT_IPV6_FLOWLABEL)|(1<<CLASS_RULES_FSELECT_DA_IPV6)|
                    (1<<CLASS_RULES_FSELECT_SA_IPV6)|(1<<CLASS_RULES_FSELECT_DA_IPV6_PREFIX)|
                    (1<<CLASS_RULES_FSELECT_SA_IPV6_PREFIX)|(1<<CLASS_RULES_FSELECT_IPV6_NEXT_HEADER));
        
        if(((mask&ipv4temp) != 0)&&((mask&ipv6temp) != 0))
        {
            SDL_MIN_LOG("conflict,IPV4 and IPV6 field can't be in a rule! L%d\n",__LINE__);
            ret = CS_E_CONFLICT;
            goto end;
        }
        
        if(((mask&(1<<CLASS_RULES_FSELECT_SA_IPV6))&&(mask&(1<<CLASS_RULES_FSELECT_SA_IPV6_PREFIX)))||
            ((mask&(1<<CLASS_RULES_FSELECT_DA_IPV6))&&(mask&(1<<CLASS_RULES_FSELECT_DA_IPV6_PREFIX))))
        {
            SDL_MIN_LOG("conflict,IPV6 addr and IPV6 addr prefix can't be in a rule! L%d\n",__LINE__);
            ret = CS_E_CONFLICT;
            goto end;
        }
    }
    
    *masktemp = mask;
end:
    return ret; 
}


/*add/del record must be after rule checking */
cs_status class_rule_tbl_add_record(
    CS_IN   cs_port_id_t                 port_id,
    CS_IN   cs_uint8                     tbl_size,
    CS_IN   cs_uint8                     num, 
    CS_IN   cs_sdl_classification_t      *cfg,
    CS_OUT  cs_uint8                     *move_flag
)
{   
    cs_int32 i,j;
    cs_uint8 pre_max  =0; //lowest precedence is max value
    cs_uint8 rule_cnt =0;
    cs_uint8 flag ;   //affect highest priority index,for the start index of update HW 
    cs_int32 k;
    cs_status ret = CS_E_OK;
    classify_unified_rules_port_t backup;
    cs_sdl_classification_t  *ptemp =NULL;


    if(UNI_PORT_RANGE_CHECK(port_id)) 
    {
        ret = CS_E_PARAM;
        goto end;
    }
    
    if(tbl_size>CLASS_RULES_MAX)
    {
        ret = CS_E_PARAM;
        goto end;
    }
                
    if((num>tbl_size)||(num<1)||(cfg == NULL)||(move_flag == NULL))
    {
        ret = CS_E_PARAM;
        goto end;
    }
    
    /* backup data for restore */
    memset(&backup, 0, sizeof(classify_unified_rules_port_t));
    memcpy(&backup, &g_port_ctc_rule_tbl[port_id-1], sizeof(classify_unified_rules_port_t));
    
    ptemp = cfg;
    
    rule_cnt = CTC_PORT_KEY_ENTRY_KEYNUMS(port_id);
    
    if(rule_cnt >0)
    {
        pre_max = CTC_PORT_KEY_ENTRY(port_id,(rule_cnt-1)).precedence;//value order: pre[0]<pre[1]<--pre[n]
        flag = rule_cnt; //start new index
    }
    else
    {
        pre_max = 0;
        flag = 0;   // empty table
    }
        
    for(i=0; i<num; i++)
    {          
        // class rule table full
        if(rule_cnt == tbl_size)
        {
            if (ptemp->precedence > pre_max)  
            {   
               /* restore data*/
                memcpy(&g_port_ctc_rule_tbl[port_id-1], &backup, sizeof(classify_unified_rules_port_t));
                ret = CS_E_RESOURCE;
                goto end;
            }
            // remove lowest precedence
            CTC_PORT_KEY_ENTRY_CLR(port_id, (rule_cnt -1));
            CTC_PORT_KEY_ENTRY_KEYNUMS(port_id)--;
            rule_cnt = CTC_PORT_KEY_ENTRY_KEYNUMS(port_id);
            pre_max  = CTC_PORT_KEY_ENTRY(port_id,(rule_cnt-1)).precedence;
            
            /* flag is the last member index*/
            if(flag > rule_cnt)
                flag = rule_cnt; 
        }
        
        if(ptemp->precedence > pre_max)
        {    
            CTC_PORT_KEY_ENTRY_SAVE(port_id, rule_cnt, ptemp);// now index rule_cnt is new member
            CTC_PORT_KEY_ENTRY_KEYNUMS(port_id)++;   
            /* now flag is the new start index*/
        } 
        else
        {    
            // class rule table is not full,add member,
            for(j=0; j<rule_cnt; j++)
            {
                if (ptemp->precedence <=CTC_PORT_KEY_ENTRY(port_id,j).precedence)
                    break;
            }
            for(k=rule_cnt-1; k>=j; k--)
            {             
                /* move local config from previous location to next location */
               CTC_PORT_KEY_ENTRY_MOVE(port_id, k + 1, k);              
            }
            CTC_PORT_KEY_ENTRY_CLR(port_id, j);
            CTC_PORT_KEY_ENTRY_SAVE(port_id, j, ptemp);// now j is new member
            CTC_PORT_KEY_ENTRY_KEYNUMS(port_id)++;
           /****************************************************************
            * if the precedence of new rule is equal to one exsiting rule  * 
            * the precedence of all subsequent rules will be increased by 1*
            ***************************************************************/   
            for(k=j; k<(CTC_PORT_KEY_ENTRY_KEYNUMS(port_id)-1); k++)
            {
                // Order table,only pre[k] <= pre[k+1]
                if(CTC_PORT_KEY_ENTRY(port_id, k).precedence == CTC_PORT_KEY_ENTRY(port_id, k+1).precedence)     
                    CTC_PORT_KEY_ENTRY(port_id, k+1).precedence++;
                else 
                    break;
            }         
            if(j<flag)
                flag = j; 
        } 
        rule_cnt = CTC_PORT_KEY_ENTRY_KEYNUMS(port_id);
        pre_max  = CTC_PORT_KEY_ENTRY(port_id,(rule_cnt-1)).precedence;
       
        ptemp++;
    }

    *move_flag = flag;
        
end:
    return ret;
}

cs_status class_rule_tbl_del_record(
    CS_IN   cs_port_id_t                 port_id,
    CS_IN   cs_uint8                     prenum,
    CS_IN   cs_uint8                     *precedence,
    CS_OUT  cs_uint8                     *move_flag
)
{
    cs_int32 i,j,k;
    cs_uint8 flag;   //affect highest priority index,for the start index of updating HW 
    cs_status ret = CS_E_OK;
      
    if(UNI_PORT_RANGE_CHECK(port_id)) 
    {
        ret = CS_E_PARAM;
        goto end;
    }
             
    if((prenum >CLASS_RULES_MAX)||(prenum<1)||(precedence == NULL)||(move_flag == NULL))
    {
        ret = CS_E_PARAM;
        goto end;
    }
    
    flag = CTC_PORT_KEY_ENTRY_KEYNUMS(port_id); // default value:(last member +1)
    // prenum<=CTC_PORT_KEY_ENTRY_KEYNUMS
    for(i=0; i<prenum; i++)
    {   
        if(!__class_find_rule_record (port_id, precedence[i]))
            continue;
        
        for(j=0; j<CTC_PORT_KEY_ENTRY_KEYNUMS(port_id); j++)
        {
            if(CTC_PORT_KEY_ENTRY(port_id, j).precedence == precedence[i])
                break;    
        }
        CTC_PORT_KEY_ENTRY_CLR(port_id, j);
        for(k=j; k<(CTC_PORT_KEY_ENTRY_KEYNUMS(port_id)-1); k++)
        {             
            /* move local config from next location to previous location */
           CTC_PORT_KEY_ENTRY_MOVE(port_id, k, k+1);              
        }
        CTC_PORT_KEY_ENTRY_CLR(port_id, (CTC_PORT_KEY_ENTRY_KEYNUMS(port_id)-1));
        CTC_PORT_KEY_ENTRY_KEYNUMS(port_id)--;
        
        if(j<flag)
            flag = j;   
    }

    *move_flag = flag;
end:
    return ret;   
}

cs_status class_rule_tbl_clr_record(CS_IN cs_port_id_t port_id)
{
    if(UNI_PORT_RANGE_CHECK(port_id)) 
        return CS_E_PARAM;
    
    memset(&g_port_ctc_rule_tbl[port_id-1], 0, sizeof(classify_unified_rules_port_t));
    
    return CS_E_OK; 
}

cs_status class_rule_tbl_get_record(
    CS_IN    cs_port_id_t                 port_id,
    CS_OUT   cs_uint8                     *num, 
    CS_OUT   cs_sdl_classification_t      *cfg
)
{    
    cs_status ret = CS_E_OK;
    cs_int32 i;

    if(UNI_PORT_RANGE_CHECK(port_id)) 
    {
        ret = CS_E_PARAM;
        goto end;
    }
                
    if((cfg == NULL)||(num == NULL))
    {
        ret = CS_E_PARAM;
        goto end;
    }
    
    *num = CTC_PORT_KEY_ENTRY_KEYNUMS(port_id);
    
    for(i=0; i<CTC_PORT_KEY_ENTRY_KEYNUMS(port_id); i++)
    {
        cfg[i].precedence  = CTC_PORT_KEY_ENTRY(port_id, i).precedence; 
        cfg[i].queueMapped = CTC_PORT_KEY_ENTRY(port_id, i).queueMapped; 
        cfg[i].priMark     = CTC_PORT_KEY_ENTRY(port_id, i).priMark; 
        cfg[i].entries     = CTC_PORT_KEY_ENTRY(port_id, i).entries; 
        memcpy(cfg[i].fselect, CTC_PORT_KEY_ENTRY(port_id, i).fselect, CLASS_FIELD_SELECT_MAX*sizeof(cs_sdl_cls_rule_t));
    }
    
end:
    return ret;    
}



