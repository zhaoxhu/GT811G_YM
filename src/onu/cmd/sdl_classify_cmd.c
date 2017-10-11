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

#include "cs_cmd.h"
#include "cs_utils.h"
#include "sdl.h"

#ifdef HAVE_SDL_CMD



#ifdef HAVE_SDL_CMD_CTC

#include "sdl_classify.h"

static cs_sdl_classification_t  g_rule_entry;

extern cs_status aal_cmd_mac_match(char *str);
extern cs_int32 cmd_ipv4_match (char *str);

char * field_name[] = {
    "DA",
    "SA",
    "802.1p",
    "vlanid",      
    "eth-type",
    "ipv4 da",
    "ipv4 sa",
    "ip protocol",
    "DSCP",
    "ipv6 TC",
    "L4 source port",
    "L4 dest port",
    "IP Version",
    "ipv6 flow label",
    "ipv6 da",
    "ipv6 sa",
    "ipv6 da prefix",
    "ipv6 sa prefix",
    "ipv6 Next header"
};

#ifdef HAVE_SDL_CMD_HELP

static const char *cls_cmd_usage_action_set[] = 
{
    "cls act <queue:0-7> <priMark: 0-7>",
    "   CTC classify action set "
    "      priMark : 0-7, 0xff - no mark. "
};

static const char *cls_cmd_usage_field_set[] = {
    "cls field <entry-id:0-2> <field: 0x0-0x12> <match value>", 
    "   entry-id: NO.field(0-2), a rule up to 3 fields",            
    "      field: 0x0-dmac;0x1-smac;0x2-dot1p;0x3-vlanid;0x4-etype;",
    "             0x5-dipv4;0x6-sipv4;0x7-ipproto;0x8-dscp;0x9-tc;",
    "             0xa-l4sport;0xb-l4dport;0xc-ipver;0xd-flowlabel;0xe-dipv6;",
    "             0xf-sipv6;0x10-dipv6-prefix;0x11-sipv6-prefix;0x12-Next-header;",
    "match value: max length is 16*8 bits.",
    "             mac  -aa:bb:cc:dd:ee:ff.",
    "             IPV4 -A.B.C.D.",
    "             IPV6 -[U32-0][U32-1][U32-2][U32-3]",
    "             IPV6 prefix-[U8-prefix][U8-0][U8-1][U8-2]...[U8-14]"
};

static const char *cls_cmd_usage_rule_add[] = {
    "cls add <port-id:1-4> <precedence: 1-255> <number of entry:1-3>",
    "   port-id : UNI port(1~4)",
    " precedence: precedence of rule(1~255)",
    "    entry  : number of rule field"
};

static const char *cls_cmd_usage_rule_del[] = {
    "cls del <port-id:1-4> <number: 1-8> <precedence 0> <precedence 1> ...", 
    "       port-id : UNI port(1~4)",
    "        number : number of rule",
    "    precedence : precedence of a deleted rule (1~255)"
};

static const char *cls_cmd_usage_temp_get[] = {
    "cls get <cr> ", 
    "    get rule config from local variable"
};

static const char *cls_cmd_usage_temp_clr[] = {
    "cls tempclr <cr> ", 
    "    clear rule config in local variable"
};

static const char *cls_cmd_usage_rule_dump[] = {
    "cls show <port-id:1-4> ", 
    "       port-id : UNI port(1~4)"
};


#endif

static void __rule_field_dump( cs_sdl_cls_rule_t *pdata)
{   
    cs_int32     i;
    cs_uint32    ip =0;
    char buf[20]; 

    
    if(pdata==NULL)
    {       
        sal_cmd_show("%% parameter error!\n\r");
        return;
    }
    
    sal_cmd_show("                  field   : %s   \n\r", field_name[pdata->fieldSelect]);
    sal_cmd_show("                operate   : 0x%x \n\r", pdata->validationOper);
    sal_cmd_show("               matchValue :");
    switch(pdata->fieldSelect)
    {
        case CLASS_RULES_FSELECT_DA_MAC:  
        case CLASS_RULES_FSELECT_SA_MAC:   
        {    
         
            sal_cmd_show(" %02x:%02x:%02x:%02x:%02x:%02x    \n\r", pdata->matchValue[CLASS_MATCH_VAL_LEN-6],
                                                                       pdata->matchValue[CLASS_MATCH_VAL_LEN-5],
                                                                       pdata->matchValue[CLASS_MATCH_VAL_LEN-4],
                                                                       pdata->matchValue[CLASS_MATCH_VAL_LEN-3],
                                                                       pdata->matchValue[CLASS_MATCH_VAL_LEN-2],
                                                                       pdata->matchValue[CLASS_MATCH_VAL_LEN-1]);    
         
            break;
        }
        case CLASS_RULES_FSELECT_802_1P:
        case CLASS_RULES_FSELECT_IPPRTO:
        case CLASS_RULES_FSELECT_TOS_DSCP:
        case CLASS_RULES_FSELECT_IPV6_TC:
        case CLASS_RULES_FSELECT_IPVER:
        case CLASS_RULES_FSELECT_IPV6_NEXT_HEADER:  
        {                   
            sal_cmd_show(" %d    \n\r", pdata->matchValue[CLASS_MATCH_VAL_LEN-1]);                    
            break;
        }
        case CLASS_RULES_FSELECT_VLAN_ID:    
        {                   
            sal_cmd_show(" %d    \n\r", (((pdata->matchValue[CLASS_MATCH_VAL_LEN-2]&0xf)<<8)|
                                          pdata->matchValue[CLASS_MATCH_VAL_LEN-1]));                    
            break;
        }
        case CLASS_RULES_FSELECT_ETH_TYPE:                           
        case CLASS_RULES_FSELECT_L4_SRC_PORT:
        case CLASS_RULES_FSELECT_L4_DST_PORT:  
        {                   
            sal_cmd_show(" 0x%x    \n\r", ((pdata->matchValue[CLASS_MATCH_VAL_LEN-2]<<8)|
                                             pdata->matchValue[CLASS_MATCH_VAL_LEN-1]));                    
            break;
        }
        case CLASS_RULES_FSELECT_DA_IPV4:
        case CLASS_RULES_FSELECT_SA_IPV4:
        {                   
            ip= ((pdata->matchValue[CLASS_MATCH_VAL_LEN - 4]<<24)|(pdata->matchValue[CLASS_MATCH_VAL_LEN - 3]<<16)|
                    (pdata->matchValue[CLASS_MATCH_VAL_LEN - 2]<<8)|pdata->matchValue[CLASS_MATCH_VAL_LEN - 1]);
            
            cs_ip2str(buf, ip);
            sal_cmd_show(" %s    \n\r", buf);           
            break;
        }

        case CLASS_RULES_FSELECT_IPV6_FLOWLABEL:  
        {                   
            sal_cmd_show(" 0x%x    \n\r", (((pdata->matchValue[CLASS_MATCH_VAL_LEN-3]&0xf)<<16)|
                                             (pdata->matchValue[CLASS_MATCH_VAL_LEN-2]<<8)|
                                              pdata->matchValue[CLASS_MATCH_VAL_LEN-1]));                    
            break;
        }   
        case CLASS_RULES_FSELECT_DA_IPV6:  
        case CLASS_RULES_FSELECT_SA_IPV6:  
        {   
            sal_cmd_show(" 0x"); 
            for(i = 0; i < CLASS_MATCH_VAL_LEN; i++){
                sal_cmd_show("%x ", pdata->matchValue[i]);  
            }
            sal_cmd_show("   \n\r"); 
            break;        
        }
        case CLASS_RULES_FSELECT_DA_IPV6_PREFIX:  
        case CLASS_RULES_FSELECT_SA_IPV6_PREFIX: 
        {   
            sal_cmd_show(" 0x"); 
            for(i = 0; i < 15; i++){
                sal_cmd_show("%x ", pdata->matchValue[i]);  
            }
            sal_cmd_show("  %d   \n\r", pdata->matchValue[CLASS_MATCH_VAL_LEN-1]); 
            break;        
        }
        default: 
             sal_cmd_show("   \n\r"); 
            break;
    }
     
    return;
}

sal_cmd_result_t cls_cmd_class_rule_get(int argc, char **argv)
{
    cs_status    ret = CS_E_OK;
    cs_port_id_t portid;
    cs_uint8     num=0;
    cs_int32     i,j;
    cs_callback_context_t   context;
    cs_sdl_classification_t cfg[12]; 
     
    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    portid = iros_strtol(argv[2]);
      
    if((portid>UNI_PORT_MAX)||(portid<CS_UNI_PORT_ID1))
        return SAL_CMD_INVALID_PARAM;

    memset(&cfg, 0, 8*sizeof(cs_sdl_classification_t));
     
    ret = epon_request_onu_port_classification_get(context, 0, 0, portid,&num,cfg);
    if(ret)
        return cs_status_2_cmd_rc_map(ret); 

    sal_cmd_show("\n===========================================================\n\r");
    sal_cmd_show("        UNI port :%d         rule num:%d  \n\r",portid,num); 
    for(i = 0; i < num; i++){
        sal_cmd_show("    rule[%d]    precedence   :%d   \n\r", i,cfg[i].precedence);
        sal_cmd_show("    rule[%d]    queuemap     :%d   \n\r", i,cfg[i].queueMapped);
        sal_cmd_show("    rule[%d]    priority mark:%d   \n\r", i,cfg[i].priMark);
        sal_cmd_show("    rule[%d]    entry number :%d   \n\r", i,cfg[i].entries);
                     
        for(j = 0; j< cfg[i].entries; j++){
            __rule_field_dump(&cfg[i].fselect[j]);                  
        }
        
        if (i % 5 == 0) {
            //HAL_DELAY_US(1000);
            cs_thread_delay(10);
        }         
    }
          
    return SAL_CMD_OK;       
}

sal_cmd_result_t cls_cmd_class_rule_get_temp(int argc, char **argv)
{   
    cs_int32     i;

     if(argc!=2) return SAL_CMD_INVALID_PARAM;
     
    sal_cmd_show("\n===========================================================\n\r");
    sal_cmd_show("              precedence   :%d   \n\r", g_rule_entry.precedence);
    sal_cmd_show("              queuemap     :%d   \n\r", g_rule_entry.queueMapped);
    sal_cmd_show("              priority mark:%d   \n\r", g_rule_entry.priMark);
    sal_cmd_show("              entry number :%d   \n\r", g_rule_entry.entries);
                    
    for(i = 0; i< CLASS_FIELD_SELECT_MAX; i++){
        __rule_field_dump(&g_rule_entry.fselect[i]);                  
    }
        
    return SAL_CMD_OK; 
}

sal_cmd_result_t cls_cmd_class_rule_clr_temp(int argc, char **argv)
{  
    if(argc!=2) return SAL_CMD_INVALID_PARAM;

    memset(&g_rule_entry, 0, sizeof(cs_sdl_classification_t));

    return SAL_CMD_OK; 
}

sal_cmd_result_t cls_cmd_class_action(int argc, char **argv)
{
    if(argc!=4) return SAL_CMD_INVALID_PARAM;
    
    g_rule_entry.queueMapped = iros_strtol(argv[2]);
    g_rule_entry.priMark = iros_strtol(argv[3]);
    
    return SAL_CMD_OK;       
}

sal_cmd_result_t cls_cmd_class_field(int argc, char **argv)
{
    cs_uint32 ip =0;
    cs_sdl_cls_rule_t  *prule=NULL;
    cs_int32            i;
    cs_uint32 fs_idx;
    cs_sdl_cls_fselect_t cf;
     
    if((fs_idx = iros_strtol(argv[2])) > 2)
        return SAL_CMD_INVALID_PARAM;

    if((cf = iros_strtol(argv[3])) > 0x12)
        return SAL_CMD_INVALID_PARAM;

    prule = &g_rule_entry.fselect[fs_idx];
    prule->fieldSelect = cf;
    prule->validationOper = CLASS_VALIDATION_OPER_EQL;
      
    switch(prule->fieldSelect)
    {
        case CLASS_RULES_FSELECT_DA_MAC:  
        case CLASS_RULES_FSELECT_SA_MAC:   
        {
            if(cs_str2mac(argv[4], &prule->matchValue[CLASS_MATCH_VAL_LEN-6])!= CS_E_OK)
                return SAL_CMD_INVALID_PARAM;
            break;
        }
        case CLASS_RULES_FSELECT_802_1P:     
        {
            if(iros_strtol(argv[4])>7)
                return SAL_CMD_INVALID_PARAM;
            
            prule->matchValue[CLASS_MATCH_VAL_LEN-1] = iros_strtol(argv[4]);  
            break;
        }
        case CLASS_RULES_FSELECT_VLAN_ID:    
        {
            if(iros_strtol(argv[4])>4095)
                return SAL_CMD_INVALID_PARAM;
            
            prule->matchValue[CLASS_MATCH_VAL_LEN-2] = (iros_strtol(argv[4])>>8)&0xf;  
            prule->matchValue[CLASS_MATCH_VAL_LEN-1] = iros_strtol(argv[4])&0xff;  
            break;
        }
        case CLASS_RULES_FSELECT_ETH_TYPE:
        {       
            prule->matchValue[CLASS_MATCH_VAL_LEN-2] = (iros_strtol(argv[4])>>8)&0xff;  
            prule->matchValue[CLASS_MATCH_VAL_LEN-1] = iros_strtol(argv[4])&0xff;  
            break;
        }
        case CLASS_RULES_FSELECT_DA_IPV4:
        case CLASS_RULES_FSELECT_SA_IPV4:
        {                   
            if(cmd_ipv4_match(argv[4]) != CS_E_OK)
            {
                sal_cmd_show("%% invalid ipv4 address format!must be: A.B.C.D\n");
                return SAL_CMD_INVALID_PARAM;
            }
            cs_str2ip(argv[4], &ip);
            prule->matchValue[CLASS_MATCH_VAL_LEN-4]= (ip>>24)&0xff;
            prule->matchValue[CLASS_MATCH_VAL_LEN-3]= (ip>>16)&0xff;
            prule->matchValue[CLASS_MATCH_VAL_LEN-2]= (ip>>8)&0xff;
            prule->matchValue[CLASS_MATCH_VAL_LEN-1]= ip&0xff;
               
            break;
        }
        case CLASS_RULES_FSELECT_IPPRTO:
        {     
            prule->matchValue[CLASS_MATCH_VAL_LEN-1] = iros_strtol(argv[4]);                
            break;
        }
        case CLASS_RULES_FSELECT_TOS_DSCP:
        {
            if(iros_strtol(argv[4])>63)
                return SAL_CMD_INVALID_PARAM;
            
            prule->matchValue[CLASS_MATCH_VAL_LEN-1] = iros_strtol(argv[4]);  
            break;
        }
        case CLASS_RULES_FSELECT_IPV6_TC:
        {     
            prule->matchValue[CLASS_MATCH_VAL_LEN-1] = iros_strtol(argv[4]);                
            break;
        }
        case CLASS_RULES_FSELECT_L4_SRC_PORT:
        case CLASS_RULES_FSELECT_L4_DST_PORT:  
        {           
            prule->matchValue[CLASS_MATCH_VAL_LEN-2] = (iros_strtol(argv[4])>>8)&0xff;  
            prule->matchValue[CLASS_MATCH_VAL_LEN-1] = iros_strtol(argv[4])&0xff;  
            break;
        }   
        case CLASS_RULES_FSELECT_IPVER:  
        {   
             if ((iros_strtol(argv[4]) != 4)&&
                 (iros_strtol(argv[4]) != 6))
                {
                    sal_cmd_show("illegal IP version.\n");
                    return SAL_CMD_INVALID_PARAM;
                }         
            prule->matchValue[CLASS_MATCH_VAL_LEN-1] = iros_strtol(argv[4]);           
            break;        
        }
        case CLASS_RULES_FSELECT_IPV6_FLOWLABEL:  
        {
            if(iros_strtol(argv[4])>0XFFFFF)
                return SAL_CMD_INVALID_PARAM;

            prule->matchValue[CLASS_MATCH_VAL_LEN-3] = (iros_strtol(argv[4])>>16)&0xf;  
            prule->matchValue[CLASS_MATCH_VAL_LEN-2] = (iros_strtol(argv[4])>>8)&0xff;  
            prule->matchValue[CLASS_MATCH_VAL_LEN-1] = iros_strtol(argv[4])&0xff;  
            break;
        }   
        case CLASS_RULES_FSELECT_DA_IPV6:  
        case CLASS_RULES_FSELECT_SA_IPV6:  
        {   
            for(i = 0; i < 4; i++){
                prule->matchValue[4*i] = (iros_strtol(argv[4+i])>>24)&0xff;  
                prule->matchValue[4*i+1] = (iros_strtol(argv[4+i])>>16)&0xff;
                prule->matchValue[4*i+2] = (iros_strtol(argv[4+i])>>8)&0xff;
                prule->matchValue[4*i+3] = iros_strtol(argv[4+i])&0xff;
            }

            break;        
        }
        case CLASS_RULES_FSELECT_DA_IPV6_PREFIX:  
        case CLASS_RULES_FSELECT_SA_IPV6_PREFIX: 
        {   
            if(iros_strtol(argv[4])>120)
                return SAL_CMD_INVALID_PARAM;                          

            prule->matchValue[CLASS_MATCH_VAL_LEN-1] = iros_strtol(argv[4]);  
           
            for(i = 0; i < (iros_strtol(argv[4])/8); i++){
                prule->matchValue[i] = iros_strtol(argv[5+i])&0xff;
            }
            if(iros_strtol(argv[4])%8!=0)
            {
               prule->matchValue[i]= (iros_strtol(argv[5+i])&0xff)&(0xff<<(8-iros_strtol(argv[4])%8));
            }
            break;        
        }
        case CLASS_RULES_FSELECT_IPV6_NEXT_HEADER:  
        {     
            prule->matchValue[CLASS_MATCH_VAL_LEN-1] = iros_strtol(argv[4]);                
            break;
        }    
        default:       
            return SAL_CMD_INVALID_PARAM;
    }
            
    return SAL_CMD_OK;       
}

sal_cmd_result_t cls_cmd_class_rule_add(int argc, char **argv)
{   
    cs_port_id_t portid;
    cs_uint8     precedence;
    cs_uint8     entry;
    cs_status    ret  = CS_E_OK;
    cs_callback_context_t   context;

    
    if(argc!=5) return SAL_CMD_INVALID_PARAM;

    portid     = iros_strtol(argv[2]);
    precedence = iros_strtol(argv[3]);
    entry      = iros_strtol(argv[4]);
    if((portid>UNI_PORT_MAX)||(portid<CS_UNI_PORT_ID1))
        return SAL_CMD_INVALID_PARAM;

    if(precedence < 1)
        return SAL_CMD_INVALID_PARAM;
    
    if((entry>3)||(entry<1))
        return SAL_CMD_INVALID_PARAM;
            
    g_rule_entry.precedence = precedence;
    g_rule_entry.entries    = entry;

    ret = epon_request_onu_port_classification_add(context, 0, 0, portid, 1,&g_rule_entry);
    
    return cs_status_2_cmd_rc_map(ret);           
}

sal_cmd_result_t cls_cmd_class_rule_del(int argc, char **argv)
{    
    cs_port_id_t portid;
    cs_uint8     num;
    cs_int32       i;
    cs_uint8     precedence[8];
    cs_status    ret  = CS_E_OK;
    cs_callback_context_t   context;
      
    if(argc >12) return SAL_CMD_INVALID_PARAM;

    portid     = iros_strtol(argv[2]);
    num        = iros_strtol(argv[3]); 

    if((portid>UNI_PORT_MAX)||(portid<CS_UNI_PORT_ID1))
        return SAL_CMD_INVALID_PARAM;

    if((num>8)||(num<1))
        return SAL_CMD_INVALID_PARAM;
    if(num==8)
    {
        ret = epon_request_onu_port_classification_clr(context, 0, 0, portid);
    }
    else
    {
        for(i = 0; i < num; i++){
            if((iros_strtol(argv[4+i])>255)||(iros_strtol(argv[4+i])<1))
                return SAL_CMD_INVALID_PARAM;
            precedence[i]= iros_strtol(argv[4+i]); 
        }

        ret = epon_request_onu_port_classification_del(context, 0, 0, portid, num,precedence);
    }
        
    return cs_status_2_cmd_rc_map(ret);              
}

static sal_cmd_reg_struct_t cls_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
 
    {cls_cmd_class_action, "act", "set action of a CTC classification rule", cls_cmd_usage_action_set, sizeof(cls_cmd_usage_action_set)/4},
    {cls_cmd_class_field, "field","set fields of a CTC classification rule", cls_cmd_usage_field_set, sizeof(cls_cmd_usage_field_set)/4},
    {cls_cmd_class_rule_add,"add", "add CTC classification rule", cls_cmd_usage_rule_add, sizeof(cls_cmd_usage_rule_add)/4},
    {cls_cmd_class_rule_del, "del", "del CTC classification rules", cls_cmd_usage_rule_del, sizeof(cls_cmd_usage_rule_del)/4},
    {cls_cmd_class_rule_get_temp, "get", "get local temporary data", cls_cmd_usage_temp_get, sizeof(cls_cmd_usage_temp_get)/4},
    {cls_cmd_class_rule_clr_temp, "tempclr", "clear local temporary data", cls_cmd_usage_temp_clr, sizeof(cls_cmd_usage_temp_clr)/4},
    {cls_cmd_class_rule_get, "show", "show CTC classification rules", cls_cmd_usage_rule_dump, sizeof(cls_cmd_usage_rule_dump)/4},
#else
    {cls_cmd_class_action, "act"},
    {cls_cmd_class_field, "field"},
    {cls_cmd_class_rule_add,"add"},
    {cls_cmd_class_rule_del, "del"},
    {cls_cmd_class_rule_get_temp, "get"},
    {cls_cmd_class_rule_clr_temp, "tempclr"},
    {cls_cmd_class_rule_get, "show"},
#endif
};

sal_cmd_cfg_t cls_cmd_cfg = {"cls", "CTC classification configuration", cls_cmd_list, sizeof(cls_cmd_list)/sizeof(sal_cmd_reg_struct_t)};

#endif
#endif //#ifdef HAVE_SDL_CMD

cs_status cls_cmd_self_reg(void)
{
    cs_status ret = CS_E_OK;
#ifdef HAVE_SDL_CMD
#ifdef HAVE_SDL_CMD_CTC

    ret = sal_cmd_reg(&cls_cmd_cfg);
#endif
#endif
    return ret;
}

//end file

