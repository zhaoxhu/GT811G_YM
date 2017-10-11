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

#ifdef HAVE_SDL_CMD


cs_callback_context_t     context;

#define SDL_MAX_VLANID    4095


#ifdef HAVE_SDL_CMD_NTT

#include "sdl_vlan.h"
#include "sdl_vlan_util.h"
#include "aal_l2.h"
#include "aal.h"


#ifdef HAVE_SDL_CMD_HELP
char *vlanmode[] = {
    "TRANSPARENT",
    "TAG",
    "TAG_IPV4_TOS",
    "TAG_IPV6_TOS",
    "TAG_BOTH_TOS"
};

char *filtermode[] = {
    "FILTER DISABLE",
    "FILTER ENABLE",
    "FILTER ENABLE WITH TPID"
};

static const char *vlan_cmd_usage_pvid[] = 
{
    "vlan pvid <port> <vlan_id:0-4095> ",
    "      port: UNI port,now is 1 ",
    "   vlan_id: default vlan id ",
};

static const char *vlan_cmd_usage_mode[] = 
{
    "vlan mode <port> <mode:0-4> ",
    "     port:UNI port,now is 1 ",
    "     mode:0--TRANSPARENT ",
    "          1--TAG ",
    "          2--TAG_IPV4_TOS ",
    "          3--TAG_IPV6_TOS ",
    "          4--TAG_BOTH_TOS ",
};

static const char *vlan_cmd_usage_filter[] = 
{
    "vlan fmode <mode:0-2> <TPID> ",
    "     mode:0--DISABLE ",
    "          1--ENABLE ",
    "          2--ENABLE_APPEND_TPID ",
    "     TPID:VLAN Tag Identification,valid when mode 2 else is 0 ",
};

static const char *vlan_cmd_usage_filter_add[] = 
{
    "vlan fadd <vlan_id:0-4095> ",
    "     vlan_id:0--4095,member of filter table ",
};

static const char *vlan_cmd_usage_filter_del[] = 
{
    "vlan fdel <vlan_id:0-4095> ",
    "     vlan_id:0--4095,member of filter table ",
};

static const char *vlan_cmd_usage_filter_clr[] = 
{
    "vlan fclr <cr> ",
    "     clear filter table ",
};

static const char *vlan_cmd_usage_tpid_sel[] = 
{
    "vlan tpidsel <sel:0-7> ",
    "     TPID selection ",
};

static const char *vlan_cmd_usage_tpid_set[] = 
{
    "vlan settpid <TPID:U16> ",
    "     TPID set ",
};

static const char *vlan_cmd_usage_show[] = 
{
    "vlan show <cr>, show all vlan configuration ",
    "          pvid, show UNI default vlanid ",
    "          mode, show vlan mode ",
    "         fmode, show vlan filter mode ",
    "          ftbl, show vlan filter table ",
};
#endif
void __show_vlan_pvid(void)
{   
    cs_status  ret    = CS_E_OK;
    cs_uint16  vlanid = 0;


    ret = epon_request_onu_pvid_get(context, 0, 0, CS_UNI_PORT_ID1, &vlanid);
    if(ret)
    {
        sal_cmd_show("failed to get port default vlan id \n");
        return;
    }
    
    sal_cmd_show( "\t    UNI port 0    default vlan_id    : %d  \n\r",vlanid);
    
    return;
}

void __show_vlan_mode(void)
{   
    cs_status           ret  = CS_E_OK;
    cs_sdl_vlan_mode_t  mode = 0;

    ret = epon_request_onu_vlan_mode_get(context, 0, 0, CS_UNI_PORT_ID1, &mode);
    if(ret)
    {
        sal_cmd_show("failed to get vlan mode \n");
        return;
    }
    #ifdef HAVE_SDL_CMD_HELP
    sal_cmd_show("\t    %-20s   :  %s  \n\r","vlan mode", vlanmode[mode]);
    #endif
    return;
}

void __show_vlan_filter_mode(void)
{   
    cs_status                  ret  = CS_E_OK;
    cs_sdl_vlan_filter_type_t  mode = 0;
    cs_uint16                  tpid = 0;
     
    ret = epon_request_onu_vlan_filter_mode_get(context, 0, 0, &mode, &tpid);
    if(ret)
    {
        sal_cmd_show("failed to get vlan filter mode \n");
        return;
    }
    
#ifdef HAVE_SDL_CMD_HELP
    sal_cmd_show("\t    %-20s   :  %s    TPID  : 0x%x  \n\r","vlan filter mode", filtermode[mode], tpid);
#endif
    return;
}

void __show_vlan_filter_tbl(void)
{   
    cs_status  ret  = CS_E_OK;
    cs_uint8   num  = 0;
    cs_uint16  i    = 0;
    cs_uint16  vlan_list[64];
      
    ret = epon_request_onu_vlan_filter_get(context, 0, 0, &num, vlan_list);
    if(ret)
    {
        sal_cmd_show("failed to get vlan filter table \n");
        return;
    }

    sal_cmd_show("\t    %-20s   :  ( number: %d )  \n\r","vlan filter table", num);
    
    for (i = 0; i < num; i++) {
        if ((i % 4) == 0){
            sal_cmd_show("\n");
            sal_cmd_show("\t            ");
        }
        sal_cmd_show("%4d      ", vlan_list[i]);
    }
    sal_cmd_show("\n");
                        
    return;
}

sal_cmd_result_t vlan_cmd_show(int argc, char **argv)
{
    if((argc<2)||(argc>3)) return SAL_CMD_INVALID_PARAM;

    if(2==argc)
    {
          sal_cmd_show("\n===============================================================\n\r");
        __show_vlan_pvid();
        __show_vlan_mode();
        __show_vlan_filter_mode();
        __show_vlan_filter_tbl();
        return SAL_CMD_OK; 
    }

    if(cmd_cmp(argv[2], "pvid"))
    {   
          sal_cmd_show("\n===============================================================\n\r");
        __show_vlan_pvid();
    }
    else if(cmd_cmp(argv[2], "mode"))
    {
          sal_cmd_show("\n===============================================\n\r");
        __show_vlan_mode();
    }
    else if(cmd_cmp(argv[2], "fmode"))
    {
          sal_cmd_show("\n===============================================================\n\r");
        __show_vlan_filter_mode();
    }
    else if(cmd_cmp(argv[2], "ftbl"))
    {
          sal_cmd_show("\n===============================================================\n\r");
        __show_vlan_filter_tbl();
    }

    return SAL_CMD_OK;       
}

sal_cmd_result_t vlan_cmd_pvid(int argc, char **argv)
{
   
    cs_status  ret    = CS_E_OK;
    cs_uint16  vlanid = 0;

    if(argc!=4) return SAL_CMD_INVALID_PARAM;

    vlanid = iros_strtol(argv[3]);
    
    if(vlanid >SDL_MAX_VLANID) return SAL_CMD_INVALID_PARAM;
    
    ret = epon_request_onu_pvid_set(context, 0, 0, CS_UNI_PORT_ID1, vlanid);
    
    return cs_status_2_cmd_rc_map(ret);       
}


sal_cmd_result_t vlan_cmd_mode(int argc, char **argv)
{
   
    cs_status  ret  = CS_E_OK;
    cs_uint16  mode = 0;

    if(argc!=4) return SAL_CMD_INVALID_PARAM;

    mode = iros_strtol(argv[3]);
    
    if(mode>4)return SAL_CMD_INVALID_PARAM;
    
    ret = epon_request_onu_vlan_mode_set(context, 0, 0, CS_UNI_PORT_ID1, mode);
    
    return cs_status_2_cmd_rc_map(ret);       
}

sal_cmd_result_t vlan_cmd_filter(int argc, char **argv)
{
   
    cs_status  ret  = CS_E_OK;
    cs_uint16  mode = 0;
    cs_uint16  tpid = 0;

    if(argc!=4) return SAL_CMD_INVALID_PARAM;

    mode = iros_strtol(argv[2]);
    tpid = iros_strtol(argv[3]);
    
    if(mode>2)return SAL_CMD_INVALID_PARAM;
    
    ret = epon_request_onu_vlan_filter_mode_set(context, 0, 0, mode, tpid);
    
    return cs_status_2_cmd_rc_map(ret);       
}

sal_cmd_result_t vlan_cmd_filter_tbl_add(int argc, char **argv)
{
   
    cs_status  ret    = CS_E_OK;
    cs_uint16  vlanid = 0;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    vlanid = iros_strtol(argv[2]);

    
    if(vlanid >SDL_MAX_VLANID)
        return SAL_CMD_INVALID_PARAM;
    
    ret = epon_request_onu_vlan_filter_add(context, 0, 0, 1, &vlanid);
    
    return cs_status_2_cmd_rc_map(ret);       
}

sal_cmd_result_t vlan_cmd_filter_tbl_del(int argc, char **argv)
{
   
    cs_status  ret  = CS_E_OK;
    cs_uint16  vlanid = 0;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    vlanid = iros_strtol(argv[2]);

    
    if(vlanid >SDL_MAX_VLANID)
        return SAL_CMD_INVALID_PARAM;
    
    ret = epon_request_onu_vlan_filter_del(context, 0, 0, 1, &vlanid);
    
    return cs_status_2_cmd_rc_map(ret);       
}

sal_cmd_result_t vlan_cmd_filter_tbl_clr(int argc, char **argv)
{
   
    cs_status  ret  = CS_E_OK;

    if(argc!=2) return SAL_CMD_INVALID_PARAM;
    
    ret = epon_request_onu_vlan_filter_clr(context, 0, 0 );
    
    return cs_status_2_cmd_rc_map(ret);       
}

sal_cmd_result_t vlan_cmd_tpid_sel(int argc, char **argv)
{
    extern cs_uint8             g_tpid_sel;
    extern cs_sdl_untag_ctrl_t  g_untag_status[2];
      
    cs_status  ret  = CS_E_OK;
    cs_uint8   tpid_sel = 0;
    
    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    tpid_sel = iros_strtol(argv[2]);
    
    if(tpid_sel>7)
        return SAL_CMD_INVALID_PARAM;
        
    g_tpid_sel = tpid_sel;
     
    if(g_untag_status[CS_UNI_PORT_ID1] > SDL_UNTAG_FWD)
    {
        ret = vlan_untag_ctrl_set(CS_UNI_PORT_ID1,g_untag_status[CS_UNI_PORT_ID1]);
    }
    
    return cs_status_2_cmd_rc_map(ret);       
}

sal_cmd_result_t vlan_cmd_tpid_set(int argc, char **argv)
{
 
    cs_status  ret  = CS_E_OK;
    cs_uint16  tpid = 0;
    cs_aal_vlan_cfg_msk_t msk;
    cs_aal_vlan_cfg_t     cfg;
    
    if(argc!=3) return SAL_CMD_INVALID_PARAM;
  
    tpid = iros_strtol(argv[2]);

    msk.u32 =0;
    msk.s.parse_outer_tpid2 =1;
    msk.s.ingress_outer_tpid2 =1;

    cfg.parse_outer_tpid2 = 1;
    cfg.ingress_outer_tpid2 = tpid;
        
    ret = aal_vlan_cfg_set(AAL_PORT_ID_GE, msk,&cfg);    
     
    return cs_status_2_cmd_rc_map(ret);       
}


static sal_cmd_reg_struct_t vlan_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    {vlan_cmd_show,           "show",   "show vlan configuration",         vlan_cmd_usage_show,       sizeof(vlan_cmd_usage_show)/sizeof(char *)},
    {vlan_cmd_pvid,           "pvid",   "set port default vlanid",         vlan_cmd_usage_pvid,       sizeof(vlan_cmd_usage_pvid)/sizeof(char *)},
    {vlan_cmd_mode,           "mode",   "set vlan mode",                   vlan_cmd_usage_mode,       sizeof(vlan_cmd_usage_mode)/sizeof(char *)},
    {vlan_cmd_filter,         "fmode",  "set vlan filter mode",            vlan_cmd_usage_filter,     sizeof(vlan_cmd_usage_filter)/sizeof(char *)},
    {vlan_cmd_filter_tbl_add, "fadd",   "add one entry to filter table",   vlan_cmd_usage_filter_add, sizeof(vlan_cmd_usage_filter_add)/sizeof(char *)},
    {vlan_cmd_filter_tbl_del, "fdel",   "del one entry from filter table", vlan_cmd_usage_filter_del, sizeof(vlan_cmd_usage_filter_del)/sizeof(char *)},
    {vlan_cmd_filter_tbl_clr, "fclr",   "clear filter table",              vlan_cmd_usage_filter_clr, sizeof(vlan_cmd_usage_filter_clr)/sizeof(char *)},
    {vlan_cmd_tpid_sel,       "tpidsel","TPID selection for Tag",          vlan_cmd_usage_tpid_sel,   sizeof(vlan_cmd_usage_tpid_sel)/sizeof(char *)},
    {vlan_cmd_tpid_set,       "settpid","TPID set for Tag",                vlan_cmd_usage_tpid_set,   sizeof(vlan_cmd_usage_tpid_set)/sizeof(char *)},
#else
    {vlan_cmd_show,           "show"},//   "show vlan configuration"},//         vlan_cmd_usage_show,       sizeof(vlan_cmd_usage_show)/sizeof(char *)},
    {vlan_cmd_pvid,           "pvid"},//   "set port default vlanid"},//         vlan_cmd_usage_pvid,       sizeof(vlan_cmd_usage_pvid)/sizeof(char *)},
    {vlan_cmd_mode,           "mode"},//   "set vlan mode"},//                   vlan_cmd_usage_mode,       sizeof(vlan_cmd_usage_mode)/sizeof(char *)},
    {vlan_cmd_filter,         "fmode"},//  "set vlan filter mode"},//            vlan_cmd_usage_filter,     sizeof(vlan_cmd_usage_filter)/sizeof(char *)},
    {vlan_cmd_filter_tbl_add, "fadd"},//   "add one entry to filter table"},//   vlan_cmd_usage_filter_add, sizeof(vlan_cmd_usage_filter_add)/sizeof(char *)},
    {vlan_cmd_filter_tbl_del, "fdel"},//   "del one entry from filter table"},// vlan_cmd_usage_filter_del, sizeof(vlan_cmd_usage_filter_del)/sizeof(char *)},
    {vlan_cmd_filter_tbl_clr, "fclr"},//   "clear filter table"},//              vlan_cmd_usage_filter_clr, sizeof(vlan_cmd_usage_filter_clr)/sizeof(char *)},
    {vlan_cmd_tpid_sel,       "tpidsel"},//"TPID selection for Tag"},//          vlan_cmd_usage_tpid_sel,   sizeof(vlan_cmd_usage_tpid_sel)/sizeof(char *)},
    {vlan_cmd_tpid_set,       "settpid"},//"TPID set for Tag"},//                vlan_cmd_usage_tpid_set,   sizeof(vlan_cmd_usage_tpid_set)/sizeof(char *)},
#endif
};

sal_cmd_cfg_t vlan_cmd_cfg = {"vlan", "vlan configuration", vlan_cmd_list, sizeof(vlan_cmd_list)/sizeof(sal_cmd_reg_struct_t)};

#endif

#ifdef HAVE_SDL_CMD_KT

#include "sdl_vlan.h"
#include "sdl_vlan_util.h"
#include "aal_l2.h"
#include "aal.h"


#ifdef HAVE_SDL_CMD_HELP

static const char *kt_vlan_cmd_usage_show[] = 
{
    "vlan show <cr>, show all vlan configuration ",
};

static const char *kt_vlan_cmd_usage_pvid[] = 
{
    "vlan pvid <port> <vlan_id:1-4095> ",
    "      port: 0-PON port, 1-UNI port ",
    "   vlan_id: default vlan id ",
};

static const char *kt_vlan_cmd_usage_add_tbl[] = 
{
    "vlan add <port> <vlan_id:1-4095> <egressmode> ",
    "        port: 0-PON port, 1-UNI port ",
    "     vlan_id: 1-4095 ",
    "  egressmode: 0-tag,1-untag ",
};

static const char *kt_vlan_cmd_usage_del_tbl[] = 
{
    "vlan del <port> <vlan_id:1-4095> ",
    "        port: 0-PON port, 1-UNI port ",
    "     vlan_id: 1-4095 ",
};

#endif

sal_cmd_result_t kt_vlan_cmd_show(int argc, char **argv)
{    
   cs_status         ret = CS_E_OK;
   cs_uint16         u_vlanid,p_vlanid;
   cs_sdl_vlan_cfg_t vlan_cfg[SDL_VLAN_ENGINE_MAX];
   cs_sdl_vlan_info_t vlan_info[SDL_VLAN_ENGINE_MAX];
   cs_uint8   accept_frame_type[] = {'A', 'T'};
   cs_uint8   uni_type =0,pon_type =0;
   cs_uint16  uni_vlan_nums =0, pon_vlan_nums =0;
   cs_uint8   i =0, j=0, ex=0;
   cs_boolean  find  = FALSE;
    
    if(argc!=2) return SAL_CMD_INVALID_PARAM;

    memset(&vlan_cfg[0], 0, SDL_VLAN_ENGINE_MAX*sizeof(cs_sdl_vlan_cfg_t));
    memset(&vlan_info[0], 0, SDL_VLAN_ENGINE_MAX*sizeof(cs_sdl_vlan_info_t));

    ret = epon_request_onu_pvid_get(context, 0, 0, CS_PON_PORT_ID, &p_vlanid);
    ret += epon_request_onu_pvid_get(context, 0, 0, CS_UNI_PORT_ID1, &u_vlanid);
    if(ret)
    {
        sal_cmd_show("failed to get port default vlan id \n");
        return SAL_CMD_FAIL;
    }
    
    ret = epon_request_onu_vlan_get(context, 0, 0, CS_PON_PORT_ID, &vlan_cfg[0], &pon_vlan_nums);  
    if((ret)||(pon_vlan_nums > SDL_VLAN_ENGINE_MAX))
    {
        sal_cmd_show("failed to dump pon port vlan table \n");
        return SAL_CMD_FAIL;
    }
    
    if(pon_vlan_nums >0)
        pon_type = 1;
    
    for (i = 0; i < pon_vlan_nums; i++) 
    {
        vlan_info[i].c_vlan = vlan_cfg[i].c_vlan;
        vlan_info[i].mbr_bmp |= 0x8000;
        if(vlan_cfg[i].eg_mode == CS_SDL_VLAN_EG_MODE_UNTAG)
        {
          vlan_info[i].utg_bmp |= 0x8000;
          pon_type =0;
        }
    }
    
    memset(&vlan_cfg[0], 0, SDL_VLAN_ENGINE_MAX*sizeof(cs_sdl_vlan_cfg_t));
    
    ret = epon_request_onu_vlan_get(context, 0, 0, CS_UNI_PORT_ID1, &vlan_cfg[0], &uni_vlan_nums);  
    if((ret)||(uni_vlan_nums > SDL_VLAN_ENGINE_MAX))
    {
        sal_cmd_show("failed to dump UNI port vlan table \n");
        return SAL_CMD_FAIL;
    }

    if(uni_vlan_nums >0)
        uni_type = 1;
     
    for (i = 0; i < uni_vlan_nums; i++) 
    {   
        find  = FALSE ;          
        for (j = 0; j < pon_vlan_nums; j++) 
        {
            if(vlan_info[j].c_vlan == vlan_cfg[i].c_vlan)
            {   
                vlan_info[j].mbr_bmp |= 0x1;
                if(vlan_cfg[i].eg_mode == CS_SDL_VLAN_EG_MODE_UNTAG)
                {
                  vlan_info[j].utg_bmp |= 0x1;
                  uni_type =0;
                }
                find = TRUE;
                break;
            }
        }
        if(!find)// not found
        {
            ex++;
            
            if((pon_vlan_nums+ex)>SDL_VLAN_ENGINE_MAX)//total VLAN nums
            {
                sal_cmd_show("Exceed MAX vlan numbers!\n");
                return SAL_CMD_FAIL;
            }
                
            vlan_info[pon_vlan_nums+ex-1].c_vlan = vlan_cfg[i].c_vlan;
            vlan_info[pon_vlan_nums+ex-1].mbr_bmp |= 0x1;
            if(vlan_cfg[i].eg_mode == CS_SDL_VLAN_EG_MODE_UNTAG)
            {
                vlan_info[pon_vlan_nums+ex-1].utg_bmp |= 0x1;
                uni_type =0;
            }
        }         
    }
    
    
    sal_cmd_show("+----------------+-------------+---------+\n");
    sal_cmd_show("|   VLAN TABLE   |  UNI-PORTS  | PON PORT|\n");
    sal_cmd_show("|                |      1      |    0    |\n");
    sal_cmd_show("+----------------+-------------+---------+\n");

    sal_cmd_show("|     PVID       |     %-4d    |   %-4d  |\n",  u_vlanid, p_vlanid);

    sal_cmd_show("| ACCEPT. FRAMES |      %c      |    %c    |\n",accept_frame_type[uni_type],accept_frame_type[pon_type]);

    sal_cmd_show("+----------------+-------------+---------+\n");

    for (i = 0; i < (pon_vlan_nums+ex); i++) 
    {      
        sal_cmd_show("| VLAN %04d(S/W) |      %c      |    %c    |\n",
          vlan_info[i].c_vlan,
        ( vlan_info[i].mbr_bmp & 0x1) ? ((vlan_info[i].utg_bmp & 0x1) ? 'U' : 'T') : '.', 
        ( vlan_info[i].mbr_bmp & 0x8000) ? ((vlan_info[i].utg_bmp & 0x8000) ? 'U' : 'T') : '.');
    }


    sal_cmd_show("+----------------+-------------+---------+\n");
    sal_cmd_show("| VLAN NUMBERS   |     %-4d    |   %-4d  |\n", uni_vlan_nums,pon_vlan_nums);
    sal_cmd_show("+----------------+-------------+---------+\n");
    sal_cmd_show("| LEGENDS                                |\n");
    sal_cmd_show("| -------                                |\n");
    sal_cmd_show("| ACCEPT. FRAME                          |\n");
    sal_cmd_show("|     A - accept all frames              |\n");
    sal_cmd_show("|     T - accept tagged frames only      |\n");
    sal_cmd_show("|     U - accept untagged frames only    |\n");
    sal_cmd_show("| VLAN                                   |\n");    
    sal_cmd_show("|     T - tagged on egress direction     |\n");    
    sal_cmd_show("|     U - untagged on egress direction   |\n");    
    sal_cmd_show("|     . - unconfigured                   |\n");    
    sal_cmd_show("+----------------------------------------+\n");
    
    return SAL_CMD_OK;       
}

sal_cmd_result_t kt_vlan_cmd_pvid(int argc, char **argv)
{
   
    cs_status  ret    = CS_E_OK;
    cs_port_id_t    port_id;
    cs_uint16    vlanid = 0;

    if(argc!=4) return SAL_CMD_INVALID_PARAM;

    port_id = iros_strtol(argv[2]);
    vlanid  = iros_strtol(argv[3]);
    
    if((port_id >CS_UNI_PORT_ID1)||(vlanid >SDL_MAX_VLANID)||(vlanid < 1)) return SAL_CMD_INVALID_PARAM;
    
    ret = epon_request_onu_pvid_set(context, 0, 0, port_id, vlanid);
    
    return cs_status_2_cmd_rc_map(ret);       
}

sal_cmd_result_t kt_vlan_cmd_add_tbl(int argc, char **argv)
{
   
    cs_status         ret    = CS_E_OK;
    cs_port_id_t       port_id;
    cs_sdl_vlan_cfg_t  vlan_cfg;

    if(argc!=5) return SAL_CMD_INVALID_PARAM;

    port_id         = iros_strtol(argv[2]);
    vlan_cfg.c_vlan  = iros_strtol(argv[3]);
    vlan_cfg.eg_mode = iros_strtol(argv[4]);
      
    if((vlan_cfg.eg_mode >CS_SDL_VLAN_EG_MODE_UNTAG)||(port_id >CS_UNI_PORT_ID1)||(vlan_cfg.c_vlan >SDL_MAX_VLANID)) return SAL_CMD_INVALID_PARAM;
    
    ret = epon_request_onu_vlan_set(context, 0, 0, port_id, &vlan_cfg, 1);
    
    return cs_status_2_cmd_rc_map(ret);       
}

sal_cmd_result_t kt_vlan_cmd_del_tbl(int argc, char **argv)
{
   
    cs_status          ret = CS_E_OK;
    cs_port_id_t       port_id;
    cs_sdl_vlan_cfg_t  vlan_cfg;
    
    if(argc!=4) return SAL_CMD_INVALID_PARAM;

            port_id  = iros_strtol(argv[2]);
    vlan_cfg.c_vlan  = iros_strtol(argv[3]);
      
    if((port_id >CS_UNI_PORT_ID1)||(vlan_cfg.c_vlan >SDL_MAX_VLANID)) return SAL_CMD_INVALID_PARAM;
    
    ret = epon_request_onu_vlan_del(context, 0, 0, port_id, &vlan_cfg, 1);
    
    return cs_status_2_cmd_rc_map(ret);       
}

static sal_cmd_reg_struct_t kt_vlan_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    {kt_vlan_cmd_show,           "show",    "show vlan configuration",         kt_vlan_cmd_usage_show,       sizeof(kt_vlan_cmd_usage_show)/sizeof(char *)},
    {kt_vlan_cmd_pvid,           "pvid",    "set port default vlanid",          kt_vlan_cmd_usage_pvid,    sizeof(kt_vlan_cmd_usage_pvid)/sizeof(char *)},
    {kt_vlan_cmd_add_tbl,        "add",     "add vlan member to port",       kt_vlan_cmd_usage_add_tbl,    sizeof(kt_vlan_cmd_usage_add_tbl)/sizeof(char *)},
    {kt_vlan_cmd_del_tbl,        "del",     "del vlan member from port",     kt_vlan_cmd_usage_del_tbl,    sizeof(kt_vlan_cmd_usage_del_tbl)/sizeof(char *)},
  
#else
    {kt_vlan_cmd_show,           "show"},
    {kt_vlan_cmd_pvid,           "pvid"},
    {kt_vlan_cmd_add_tbl,        "add" },
    {kt_vlan_cmd_del_tbl,        "del" },
  
#endif
};


sal_cmd_cfg_t kt_vlan_cmd_cfg = {"vlan", "vlan configuration", kt_vlan_cmd_list, sizeof(kt_vlan_cmd_list)/sizeof(sal_cmd_reg_struct_t)};
#endif




#endif //#ifdef HAVE_SDL_CMD
cs_status vlan_cmd_self_reg(void)
{
    cs_status ret = CS_E_OK;
#ifdef HAVE_SDL_CMD



#ifdef HAVE_SDL_CMD_NTT

    ret = sal_cmd_reg(&vlan_cmd_cfg);
#endif
#ifdef HAVE_SDL_CMD_KT

    ret = sal_cmd_reg(&kt_vlan_cmd_cfg);
#endif

#endif
    return ret;
}

//end file


