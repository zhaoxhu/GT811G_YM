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
#include "aal.h"
#include "aal_util.h"
#include "aal_l2.h"
#include <rtk_api.h>
#include <rtk_api_ext.h>
#include <rtl8367b_asicdrv_lut.h>
#include "sdl_vlan.h"
#include "sdl_ma.h"
#include "sdl_mc.h"
#include "sdl_fdb.h"
#include "sdl_l2.h"

extern cs_status aal_cmd_mac_match(char *str);
extern cs_int32 cmd_ipv4_match (char *str);

//char *le_str[] = {"GE", "PON", "MA"};
  
/****************************************************************************/
#define __SW_L2_PRINT_CMD_HELP(func,cmd,help)  diag_printf("%s %s %-20s --- %s\n", "sw",(func),(cmd),(help))

typedef void (*aal_l2_cmd_func_t)(int, char **);

/****************************************************************************/
/*                     L2(fdb/port/vlan) SDK API  debug commands            */
/****************************************************************************/
static int __sw_l2_cmd_cmp(char *str, const char *mode)
{   
    while(mode && *mode)
    {
        if( (*str=='\0') || (*mode != *str) )
            return 0;

        mode++;
        str++;
    }

    return 1;
}

static void __sw_vlan_cmd_init(int  argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;

    rtn = rtk_vlan_init();

    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");
    
    return;
}

static void __sw_vlan_cmd_set(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    int para1;
    int para2;
    int para3;
    int para4;

    if (argc != 4)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    para4 = iros_strtol(argv[3]);

    if (para1 > 0xfff)
    {
        diag_printf("vlan id error\n");
        return;
    }

    rtn = rtk_vlan_set(para1, *(rtk_portmask_t *)&para2, *(rtk_portmask_t *)&para3, para4);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_vlan_cmd_get(int argc, char **argv)
{
    rtk_portmask_t member, untagged;
    rtk_fid_t fid = 0;
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    int para1;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > 0xfff)
    {
        diag_printf("vlan id error\n");
        return;
    }

    rtn = rtk_vlan_get(para1, &member, &untagged, &fid);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("member bitmap(%08x), untagged bitmap(%08x), fid(%d)\n", member.bits[0], untagged.bits[0], fid);

    return;
}


static void __sw_vlan_cmd_pvid_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    int para3;

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 > 0xfff)
    {
        diag_printf("vlan id error\n");
        return;
    }

    if (para3 > 0x7)
    {
        diag_printf("priority error\n");
        return;
    }

    rtn = rtk_vlan_portPvid_set(para1, para2, para3);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}


static void __sw_vlan_cmd_pvid_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_vlan_t vlanid;
    rtk_pri_t  priority;
    int para1;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_vlan_portPvid_get(para1, &vlanid, &priority);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("vlanid(%d), priority(%d)\n", vlanid, priority);

    return;
}

static void __sw_vlan_cmd_port_transparent_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_port_t egress_port = 0;
    rtk_enable_t enable = 0;
    rtk_portmask_t ingress_bmp;

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    egress_port = iros_strtol(argv[0]);
    ingress_bmp.bits[0] = iros_strtol(argv[1]);
    enable = iros_strtol(argv[2]);

    if (egress_port > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (ingress_bmp.bits[0] > 0xff)
    {
        diag_printf("vlan id error\n");
        return;
    }

    rtn = rtk_vlan_transparent_set(egress_port, ingress_bmp, enable);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}


static void __sw_vlan_cmd_port_transparent_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_portmask_t ingress_bmp;
    rtk_port_t egress_port = 0;
    rtk_enable_t enable = 0;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    egress_port = iros_strtol(argv[0]);
    if (egress_port > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_vlan_transparent_get(egress_port, &ingress_bmp, &enable);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("egress-port(%d), ingress-bitmap(%02xh), enabled(%d)\n", egress_port, ingress_bmp.bits[0], enable);

    return;
}


static void __sw_vlan_cmd_port_fid_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    int para3;

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 > 1)
    {
        diag_printf("enable error\n");
        return;
    }

    rtn = rtk_vlan_portFid_set(para1, para2, 0);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_vlan_cmd_port_fid_get(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_enable_t  enable = 0;
    rtk_fid_t fid = 0;
    int para1;
    
    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_vlan_portFid_get(para1, &enable, &fid);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("port-based-filtering(%s), fid(%d)\n", enable ? "enabled" : "disabled", fid);

    return;
}


static void __sw_vlan_cmd_port_ingress_filter_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 > 1)
    {
        diag_printf("vlan id error\n");
        return;
    }

    rtn = rtk_vlan_portIgrFilterEnable_set(para1, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_vlan_cmd_port_ingress_filter_get(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_enable_t  enable = 0;
    int para1;
    
    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_vlan_portIgrFilterEnable_get(para1, &enable);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("port-ingress-filter-state(%s)\n", enable ? "enabled" : "disabled");

    return;
}

static void __sw_vlan_cmd_port_accept_frame_set(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 >= ACCEPT_FRAME_TYPE_END)
    {
        diag_printf("vlan id error\n");
        return;
    }

    rtn = rtk_vlan_portAcceptFrameType_set(para1, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_vlan_cmd_port_accept_frame_get(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_vlan_acceptFrameType_t  frame_type = ACCEPT_FRAME_TYPE_ALL;
    int para1;
    
    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_vlan_portAcceptFrameType_get(para1, &frame_type);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("acceptted-frame-type(%d : %s)\n", frame_type, (frame_type == 0) ? "all" : ((frame_type == 1) ? "tag only" : "untagged only"));

    return;
}

static void __sw_vlan_cmd_port_tag_mode_set(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 >= VLAN_TAG_MODE_END)
    {
        diag_printf("tag mode error\n");
        return;
    }
    

    rtn = rtk_vlan_tagMode_set(para1, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_vlan_cmd_port_tag_mode_get(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_vlan_tagMode_t mode = VLAN_TAG_MODE_ORIGINAL;
    int para1;
    char *eg_tag_mode[] = {"original", "keep format", "pri", "real keep format"};
    
    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_vlan_tagMode_get(para1, &mode);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("tag-mode(%d:%s)\n", mode, eg_tag_mode[mode]);

    return;
}

static void __sw_vlan_cmd_svlan_init(int  argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;

    rtn = rtk_svlan_init();

    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");
    
    return;
}

static void __sw_vlan_cmd_svlan_serv_port_add(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    int para1;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_svlan_servicePort_add(para1);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_vlan_cmd_svlan_serv_port_del(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    int para1;
    
    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_svlan_servicePort_del(para1);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_vlan_cmd_svlan_serv_port_get(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_portmask_t serv_port;

    rtn = rtk_svlan_servicePort_get(&serv_port);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("service-port(%02xh)\n", serv_port.bits[0]);

    return;
}

static void __sw_vlan_cmd_svlan_tpid_set(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    int para1;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }
    
    para1 = iros_strtol(argv[0]);

    rtn = rtk_svlan_tpidEntry_set(para1);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}


static void __sw_vlan_cmd_svlan_tpid_get(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_svlan_tpid_t tpid = 0;

    rtn = rtk_svlan_tpidEntry_get(&tpid);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("svlan-tpid(%04xh)\n", tpid);

    return;
}

static void __sw_vlan_cmd_svlan_member_set(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_svlan_memberCfg_t member;
    int para1;

    if (argc != 6)
    {
        diag_printf("parameter number error\n");
        return;
    }

    memset(&member, 0, sizeof(member));
    
    para1       = iros_strtol(argv[0]);
    member.svid = iros_strtol(argv[1]);
    member.memberport = iros_strtol(argv[2]);
	member.untagport = iros_strtol(argv[3]);
    member.fid        = iros_strtol(argv[4]);
    member.priority   = iros_strtol(argv[5]);

    rtn = rtk_svlan_memberPortEntry_set(para1, &member);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}


static void __sw_vlan_cmd_svlan_member_get(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_svlan_memberCfg_t member;
    int para1;
    
    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    memset(&member, 0, sizeof(member));
    rtn = rtk_svlan_memberPortEntry_get(para1, &member);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("s-vlanid(%d), member(%04xh), untagged port(%08x), fid(%d), priority(%d)\n", 
            member.svid, member.memberport ,member.untagport, member.fid, member.priority);

    return;
}

static void __sw_vlan_cmd_dft_egress_svlan_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 > 0xfff)
    {
        diag_printf("vlan id error\n");
        return;
    }

    rtn = rtk_svlan_defaultSvlan_set(para1, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}


static void __sw_vlan_cmd_dft_egress_svlan_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_vlan_t vlanid;
    int para1;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_svlan_defaultSvlan_get(para1, &vlanid);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("default-svlan-id(%d)\n", vlanid);

    return;
}


static void __sw_vlan_cmd_c2s_add(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    int para3;

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);

    if (para1 > 0xfff)
    {
        diag_printf("c-vlan error\n");
        return;
    }

    if (para2 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para3 > 0xfff)
    {
        diag_printf("s-vlan error\n");
        return;
    }

    rtn = rtk_svlan_c2s_add(para1, para2, para3);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}


static void __sw_vlan_cmd_c2s_del(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 > 0xfff)
    {
        diag_printf("c-vlan error\n");
        return;
    }

    if (para2 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_svlan_c2s_del(para1, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_vlan_cmd_c2s_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_vlan_t svid = 0;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 > 0xfff)
    {
        diag_printf("c-vlan error\n");
        return;
    }

    if (para2 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_svlan_c2s_get(para1, para2, &svid);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("c-vlan(%d) + s-port(%d) -> s-vlan(%d)\n", para1, para2, svid);

    return;
}


static void __sw_vlan_cmd_sp2c_add(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    int para3;

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);

    if (para1 > 0xfff)
    {
        diag_printf("s-vlan error\n");
        return;
    }

    if (para2 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para3 > 0xfff)
    {
        diag_printf("c-vlan error\n");
        return;
    }

    rtn = rtk_svlan_sp2c_add(para1, para2, para3);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}


static void __sw_vlan_cmd_sp2c_del(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 > 0xfff)
    {
        diag_printf("c-vlan error\n");
        return;
    }

    if (para2 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_svlan_sp2c_del(para1, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_vlan_cmd_sp2c_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_vlan_t cvid = 0;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 > 0xfff)
    {
        diag_printf("s-vlan error\n");
        return;
    }

    if (para2 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_svlan_sp2c_get(para1, para2, &cvid);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("s-vlan(%d) + d-port(%d) -> c-vlan(%d)\n", para1, para2, cvid);

    return;
}

static void __sw_vlan_cmd_svlan_untag_action_set(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1       = iros_strtol(argv[0]);
    if (para1 >= UNTAG_END)
    {
        diag_printf("action error\n");
        return;
    }

    para2       = iros_strtol(argv[1]);
    if (para2 > 0xfff)
    {
        diag_printf("s-vlan error\n");
        return;
    }

    rtn = rtk_svlan_untag_action_set(para1, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}


static void __sw_vlan_cmd_svlan_untag_action_get(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_svlan_untag_action_t action = UNTAG_DROP;
    rtk_vlan_t svid = 0;
    char *untag_action_str[] = {"drop", "trap", "assign"};

    rtn = rtk_svlan_untag_action_get(&action, &svid);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("action(%d:%s), s-vlanid(%d)\n", 
            action, untag_action_str[action], svid);

    return;
}

static void __sw_vlan_cmd_svlan_unmatch_action_set(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1       = iros_strtol(argv[0]);
    if (para1 >= UNMATCH_END)
    {
        diag_printf("action error\n");
        return;
    }

    para2       = iros_strtol(argv[1]);
    if (para2 > 0xfff)
    {
        diag_printf("s-vlan error\n");
        return;
    }

    rtn = rtk_svlan_unmatch_action_set(para1, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}


static void __sw_vlan_cmd_svlan_unmatch_action_get(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_svlan_unmatch_action_t action = UNMATCH_DROP;
    rtk_vlan_t svid = 0;
    char *unmatch_action_str[] = {"drop", "trap", "assign"};

    rtn = rtk_svlan_unmatch_action_get(&action, &svid);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("action(%d:%s), s-vlanid(%d)\n", 
            action, unmatch_action_str[action], svid);

    return;
}

static void __sw_vlan_cmd_svlan_dmac_vidsel_set(int argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 >= RTK_ENABLE_END)
    {
        diag_printf("enable error\n");
        return;
    }

    rtn = rtk_svlan_dmac_vidsel_set(para1, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_vlan_cmd_svlan_dmac_vidsel_get(int argc, char **argv)
{
    rtk_api_ret_t rtn   = RT_ERR_FAILED;
    rtk_enable_t enable = DISABLED;
    int para1;
    
    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_svlan_dmac_vidsel_get(para1, &enable);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("enable(%d:%s)\n", enable, enable ? "enabled" : "disabled");

    return;
}

static void __sw_vlan_cmd_svlan_pri_ref_set(int argc, char **argv)
{
    rtk_api_ret_t rtn   = RT_ERR_FAILED;
    int para1;
    
    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    if (para1 >= REF_PRI_END)
    {
        diag_printf("pri ref error\n");
        return;
    }

    rtn = rtk_svlan_priorityRef_set(para1);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_vlan_cmd_svlan_pri_ref_get(int argc, char **argv)
{
    rtk_api_ret_t rtn   = RT_ERR_FAILED;
    rtk_svlan_pri_ref_t ref;
    char *pri_ref_str[] = {"internal", "ctag", "svlan"};

    rtn = rtk_svlan_priorityRef_get(&ref);

    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("svlan-pri-ref(%s)\n", pri_ref_str[ref]);

    return;
}
#if 0
static void __sw_vlan_cmd_dasan_vlan_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    cs_sdl_vlan_cfg_t vlan_cfg[3];
    cs_callback_context_t context;

    memset(&context, 0, sizeof(context));

    if (argc != 8)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 > 3)
    {
        diag_printf("max 3 group vlan info supported\n");
        return;
    }

    if (para2 == 0)
    {
        diag_printf("no vlan configured");
        return;
    }

    vlan_cfg[0].c_vlan   = iros_strtol(argv[2]);
    vlan_cfg[0].eg_mode  = iros_strtol(argv[3]) ? 0 : 1;
    vlan_cfg[1].c_vlan   = iros_strtol(argv[4]);
    vlan_cfg[1].eg_mode  = iros_strtol(argv[5]) ? 0 : 1;
    vlan_cfg[2].c_vlan   = iros_strtol(argv[6]);
    vlan_cfg[2].eg_mode  = iros_strtol(argv[7]) ? 0 : 1;


    //rtn = epon_request_onu_vlan_set(context, 0, 0, para1, &vlan_cfg[0], para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}


static void __sw_vlan_cmd_dasan_vlan_del(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    cs_sdl_vlan_cfg_t vlan_cfg[3];
    cs_callback_context_t context;

    memset(&context, 0, sizeof(context));

    if (argc != 8)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 > 3)
    {
        diag_printf("max 3 group vlan info supported\n");
        return;
    }

    vlan_cfg[0].c_vlan   = iros_strtol(argv[2]);
    vlan_cfg[0].eg_mode  = iros_strtol(argv[3]);
    vlan_cfg[1].c_vlan   = iros_strtol(argv[4]);
    vlan_cfg[1].eg_mode  = iros_strtol(argv[5]);
    vlan_cfg[2].c_vlan   = iros_strtol(argv[6]);
    vlan_cfg[2].eg_mode  = iros_strtol(argv[7]);

    //rtn = epon_request_onu_vlan_del(context, 0, 0, para1, &vlan_cfg[0], para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}


static void __sw_vlan_cmd_ctc_vlan_set(int argc, char **argv)
{
    cs_port_id_t                  port_id;
    cs_sdl_vlan_tag_t             def_vlan;
    cs_sdl_vlan_mode_t            vlan_mode;
    cs_sdl_vlan_cfg_t             vlan_cfg[8];
    cs_uint16                     cfg_nums;

    port_id = 

}
#endif

static void __sw_vlan_cmd_vlan_dump(int argc, char **argv)
{
    //(void)epon_onu_8021q_vlan_dump();
    extern void sdl_sw_vlan_dump(void);
    sdl_sw_vlan_dump();
}




static struct {
    char                   *name;
    aal_l2_cmd_func_t  exe_func;
    int                     argc;
    char                   *para;
    char                   *help;
}sw_vlan_cmds[] = {
        { "init",                   __sw_vlan_cmd_init           , 0,  "<cr>",                       "initialize vlan engine"},
        { "set",                    __sw_vlan_cmd_set            , 4,  "[vlanid] [member port] [untagged port] [fid]", "set vlan configuration"},
        { "get",                    __sw_vlan_cmd_get            , 1,  "[vlanid]",                   "get vlan configuration"},
        { "pvid-set",               __sw_vlan_cmd_pvid_set       , 3,  "[port] [vlanid] [priority]", "set port's default vlan id"},
        { "pvid-get",               __sw_vlan_cmd_pvid_get       , 1,  "[port]",                     "get port's default vlan id"},
        { "port-transparent-set",   __sw_vlan_cmd_port_transparent_set , 3,  "[eg_port] [ig_mask] [enable]",  "set port's transparent mode"},
        { "port-transparent-get",   __sw_vlan_cmd_port_transparent_get , 1,  "[port]",                        "get port's transparent mode"},
        { "port-fid-set",           __sw_vlan_cmd_port_fid_set   , 3,   "[port] [enable] [fid]",     "set port fid configuration"},
        { "port-fid-get",           __sw_vlan_cmd_port_fid_get   , 1,   "[port]",                    "get port fid configuration"},
        { "port-ig-filter-set",     __sw_vlan_cmd_port_ingress_filter_set , 2, "[port] [enable]",    "set port ingress filter configuration"},
        { "port-ig-filter-get",     __sw_vlan_cmd_port_ingress_filter_get , 1, "[port]",             "get port ingress filter configuration"},
        { "port-accept-frame-set",  __sw_vlan_cmd_port_accept_frame_set       , 2, "[port] [frame-type]", "set acceptted frame type for specified port"},
        { "port-accept-frame-get",  __sw_vlan_cmd_port_accept_frame_get       , 1, "[port]",              "get acceptted frame type for specified port"},        
        { "port-eg-tag-mode-set",   __sw_vlan_cmd_port_tag_mode_set           , 2, "[port] [mode]",       "set port's egress tag mode"},
        { "port-eg-tag-mode-get",   __sw_vlan_cmd_port_tag_mode_get           , 1, "[port]",              "get port's egress tag mode"},
        { "svlan-init",             __sw_vlan_cmd_svlan_init             , 0,  "<cr>",               "initialize s-vlan engine"},
        { "svlan-serv-port-add",    __sw_vlan_cmd_svlan_serv_port_add    , 1, "[port]",              "add service port"},
        { "svlan-serv-port-del",    __sw_vlan_cmd_svlan_serv_port_del    , 1, "[port]",              "del service port"},
        { "svlan-serv-port-get",    __sw_vlan_cmd_svlan_serv_port_get    , 0, "<cr>",                "get service port"},
        { "svlan-tpid-set",         __sw_vlan_cmd_svlan_tpid_set         , 1, "[tpid]",              "set TPID for s-vlan tag"},
        { "svlan-tpid-get",         __sw_vlan_cmd_svlan_tpid_get         , 0, "<cr>",                "get TPID for s-vlan tag"},
        { "svlan-member-set",       __sw_vlan_cmd_svlan_member_set       , 6, "[s-vlanid] [s-vlanid] [member] [untagged member][fid] [priority]", "set s-vlan member ports"},
        { "svlan-member-get",       __sw_vlan_cmd_svlan_member_get       , 1, "[s-vlanid]",          "get s-vlan member ports"},
        { "svlan-eg-dft-svid-set",  __sw_vlan_cmd_dft_egress_svlan_set   , 2,  "[port] [s-vlanid]",      "set port's default s-vlan id"},
        { "svlan-eg-dft-svid-get",  __sw_vlan_cmd_dft_egress_svlan_get   , 1,  "[port]",                 "get port's default s-vlan id"},
        { "svlan-c2s-add",          __sw_vlan_cmd_c2s_add       , 3, "[c-vlanid] [s-port] [s-vlanid]", "set c-vlan to s-vlan translation pair"},
        { "svlan-c2s-del",          __sw_vlan_cmd_c2s_del       , 2, "[c-vlanid] [s-port]",            "del c-vlan to s-vlan translation pair"},        
        { "svlan-c2s-get",          __sw_vlan_cmd_c2s_get       , 2, "[c-vlanid] [s-port]",            "get c-vlan to s-vlan translation pair"},
        { "svlan-sp2c-add",         __sw_vlan_cmd_sp2c_add      , 3, "[s-vlanid] [d-port] [c-vlanid]", "set s-vlan to c-vlan translation pair"},
        { "svlan-sp2c-del",         __sw_vlan_cmd_sp2c_del      , 2, "[s-vlanid] [d-port]",            "del s-vlan to c-vlan translation pair"},        
        { "svlan-sp2c-get",         __sw_vlan_cmd_sp2c_get      , 2, "[s-vlanid] [d-port]",            "get s-vlan to c-vlan translation pair"},
        { "svlan-untag-action-set", __sw_vlan_cmd_svlan_untag_action_set       , 2, "[action] [s-vlanid]", "set action of downstream un-stag packet"},
        { "svlan-untag-action-get", __sw_vlan_cmd_svlan_untag_action_get       , 0, "<cr>",                "get action of downstream un-stag packet"},
        { "svlan-unmatch-action-set", __sw_vlan_cmd_svlan_unmatch_action_set       , 2, "[action] [s-vlanid]", "set action of downstream un-match packet"},
        { "svlan-unmatch-action-get", __sw_vlan_cmd_svlan_unmatch_action_get       , 0, "<cr>",                "get action of downstream un-match packet"},
        { "svlan-dmac-vidsel-set",    __sw_vlan_cmd_svlan_dmac_vidsel_set          , 2, "[port] [enable]",       "set d-mac c-vlanid selection state"},
        { "svlan-dmac-vidsel-get",    __sw_vlan_cmd_svlan_dmac_vidsel_get          , 1, "[port]",                "get d-mac c-vlanid selection state"},
        { "svlan-pri-ref-set",        __sw_vlan_cmd_svlan_pri_ref_set      , 1, "[pri-ref]", "set SVLAN tag priority reference source"},
        { "svlan-pri-ref-get",        __sw_vlan_cmd_svlan_pri_ref_get      , 0, "<cr>", "get SVLAN tag priority reference source"},
#if 0
        { "dasan-vlan-set",           __sw_vlan_cmd_dasan_vlan_set         , 8,    "[port] [nums] [vlanid-0] [tag-mode-0] [vlanid-1] [tag-mode-1] ...",    "sdl vlan add for dasan"},
        { "dasan-vlan-del",           __sw_vlan_cmd_dasan_vlan_del         , 8,    "[port] [nums] [vlanid-0] [tag-mode-0] [vlanid-1] [tag-mode-1] ...",    "sdl vlan del for kt"},
        { "ctc-vlan-set",             __sw_vlan_cmd_ctc_vlan_set         , 8,    "[port] [nums] [vlanid-0] [tag-mode-0] [vlanid-1] [tag-mode-1] ...",    "sdl vlan add for dasan"},
#endif

        { "dump",                     __sw_vlan_cmd_vlan_dump           , 0,    "<cr>",                                         "sdl vlan query for kt"},

        
};

void __sw_vlan_cmd_help(void)
{
    int i;

    for (i = 0; i < sizeof(sw_vlan_cmds) / sizeof(sw_vlan_cmds[0]); i++) {  
        __SW_L2_PRINT_CMD_HELP("vlan", sw_vlan_cmds[i].name, sw_vlan_cmds[i].help);    
    }
}

static aal_l2_cmd_func_t  __sw_vlan_find_cmd
(
    const char *cmd_str,
    int   argc
)
{
    int i = 0;

    if (NULL == cmd_str) {
        return NULL;
    }

    for (i = 0;
        i < (sizeof(sw_vlan_cmds)/ sizeof(sw_vlan_cmds[0]));
        ++i) {
        if (__sw_l2_cmd_cmp(sw_vlan_cmds[i].name, cmd_str)){

            if ((argc - 3) == sw_vlan_cmds[i].argc) {
                return sw_vlan_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", sw_vlan_cmds[i].argc);
                diag_printf("parameters:  %s\n", sw_vlan_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __sw_vlan_cmd_help();

    return (NULL);

}

void sw_vlan_cmd(int argc, char **argv)
{
    aal_l2_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal vlan  ..."*/

        p_cmd_func = __sw_vlan_find_cmd(argv[2], argc);

    }
    else {
        __sw_vlan_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
}

static void __sw_l2_cmd_l2_init(int  argc, char **argv)
{
#if 1

    rtk_api_ret_t rtn = RT_ERR_FAILED;

    rtn = rtk_switch_init();
    if (rtn)
        diag_printf("switch init errcode(%d)\n", rtn);
    else
        diag_printf("switch is being initialized ... done\n");
    

    rtn = rtk_l2_init();
    if (rtn)
        diag_printf("l2 init errcode(%d)\n", rtn);
    else
        diag_printf("l2 is being initialized     ... done\n");

    rtn = rtk_vlan_init();
    if (rtn)
        diag_printf("vlan init errcode(%d)\n", rtn);
    else
        diag_printf("vlan is being initialized   ... done\n");


    rtn = rtk_svlan_init();
    if (rtn)
        diag_printf("svlan init errcode(%d)\n", rtn);
    else
        diag_printf("svlan is being initialized  ... done\n");

    rtn = rtk_igmp_init();
    if (rtn)
        diag_printf("igmp init errcode(%d)\n", rtn);
    else
        diag_printf("igmp is being initialized   ... done\n");

#if 0
    rtn = rtk_qos_init(8);
    if (rtn)
        diag_printf("qos init errcode(%d)\n", rtn);
    else
        diag_printf("qos init done\n");
#endif
    #endif
    return;
}


static void __sw_l2_cmd_mac_learned_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    rtk_mac_cnt_t learned_number = 0;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_l2_learningCnt_get(para1, &learned_number);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("mac-learned-number(%d)\n", learned_number);

    return;
}


static void __sw_l2_cmd_mac_aging_enable_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 > RTK_MAX_NUM_OF_LEARN_LIMIT)
    {
        diag_printf("bitmap error\n");
        return;
    }

    rtn = rtk_l2_agingEnable_set(para1, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_l2_cmd_mac_aging_enable_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    rtk_enable_t enable;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_l2_agingEnable_get(para1, &enable);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("aging-enabled(%d:%s)\n", enable, enable ? "enabled" : "disabled");

    return;
}


static void __sw_l2_cmd_mac_aging_time_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > 800)    /* max 800s */
    {
        diag_printf("aging time error\n");
        return;
    }

    rtn = rtk_l2_aging_set(para1);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_l2_cmd_mac_aging_time_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_l2_age_time_t time;

    rtn = rtk_l2_aging_get(&time);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("aging-time(%ds)\n", time);

    return;
}


static void __sw_l2_cmd_isolation_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    rtk_portmask_t port_bmp;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 > 0xffff)
    {
        diag_printf("bitmap error\n");
        return;
    }

    port_bmp.bits[0] = para2;

    rtn = rtk_port_isolation_set(para1, port_bmp);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_l2_cmd_isolation_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    rtk_portmask_t port_bmp;


    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_port_isolation_get(para1, &port_bmp);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("isolation-bitmap(%04xh)\n", port_bmp.bits[0]);

    return;
}

static void __sw_l2_cmd_flood_portbmp_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    rtk_portmask_t port_bmp;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 >= FLOOD_END)
    {
        diag_printf("flood type error\n");
        return;
    }

    if (para2 > RTK_MAX_PORT_MASK)
    {
        diag_printf("port bitmap error\n");
        return;
    }

    port_bmp.bits[0] = para2;

    rtn = rtk_l2_floodPortMask_set(para1, port_bmp);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_l2_cmd_flood_portbmp_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    rtk_portmask_t port_bmp;
    char *flood_str[] = {"unknown uc", "unkown mc", "broadcast", "invalid"};

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 >= FLOOD_END)
    {
        diag_printf("flood type error\n");
        return;
    }

    rtn = rtk_l2_floodPortMask_get(para1, &port_bmp);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("flood-type(%d:%s), flood-port-bitmap(%04xh)\n", para1, flood_str[para1], port_bmp.bits[0]);

    return;
}

static void __sw_l2_cmd_mac_limit_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 > RTK_MAX_NUM_OF_LEARN_LIMIT)
    {
        diag_printf("bitmap error\n");
        return;
    }

    rtn = rtk_l2_limitLearningCnt_set(para1, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_l2_cmd_mac_limit_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    rtk_mac_cnt_t mac_limit;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_l2_limitLearningCnt_get(para1, &mac_limit);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("mac-limit-number(%d)\n", mac_limit);

    return;
}

static void __sw_l2_cmd_mac_limit_action_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 >= LIMIT_LEARN_CNT_ACTION_END)
    {
        diag_printf("action error\n");
        return;
    }

    rtn = rtk_l2_limitLearningCntAction_set(RTK_WHOLE_SYSTEM, para1);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_l2_cmd_mac_limit_action_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_l2_limitLearnCntAction_t action;
    char *action_str[] = {"drop", "forward", "cpu", "invalid"};

    rtn = rtk_l2_limitLearningCntAction_get(RTK_WHOLE_SYSTEM, &action);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("mac-limit-action(%d:%s)\n", action, action_str[action]);

    return;
}

static void __sw_l2_cmd_stp_state_set(int argc, char **argv)
{
    cs_status rtn;
    int para1;
    int para2;
    cs_callback_context_t context;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para2 > STP_STATE_END)
    {
        diag_printf("stp state error\n");
        return;
    }

    rtn = epon_request_onu_stp_set(context,0 , 0, para1, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_l2_cmd_stp_state_get(int argc, char **argv)
{
    cs_status rtn;
    int para1;
    cs_sdl_stp_state_t stp_state = STP_STATE_DISABLED;
    cs_callback_context_t context;
    char *stp_state_str[] = {"disabled", "blocking", "learning", "forwarding", "invalid"};

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
   

    rtn = epon_request_onu_stp_get(context, 0,0, para1, &stp_state);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("mac-limit-action(%d:%s)\n", stp_state, stp_state_str[stp_state]);

    return;
}


static void __sw_l2_cmd_cpu_port_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 >= CPU_INSERT_END)
    {
        diag_printf("msti error\n");
        return;
    }

    rtn = rtk_cpu_enable_set(ENABLED);
    if (rtn)
        diag_printf("errcode1(%d)\n", rtn);

    rtn = rtk_cpu_tagPort_set(para1, para2);
    if (rtn)
        diag_printf("errcode2(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_l2_cmd_cpu_port_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_port_t cpu_port = 0;
    rtk_cpu_insert_t tag_mode = CPU_INSERT_TO_ALL;
    char *tag_mode_str[] = {"all", "trapping", "none", "invalid"};

    rtn = rtk_cpu_tagPort_get(&cpu_port, &tag_mode);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("cpu-port(%d), tag-insert-mode(%d:%s)\n", cpu_port, tag_mode, tag_mode_str[tag_mode]);

    return;
}

static void __sw_l2_cmd_cpu_dump_set(int argc, char **argv)
{
    extern rtk_uint8 g_rtk_cpu_dump_flag;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    g_rtk_cpu_dump_flag = iros_strtol(argv[0]);

    diag_printf("done\n");

    return;
}

static void __sw_l2_cmd_cpu_tx_test(int argc, char **argv)
{
    cs_callback_context_t context;
    rtk_uint8 dport = 0;
    cs_uint8 pkt[64] = 
    {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x81, 0x00, 0x00, 0x0a, 
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }
    

    memset(&context, 0, sizeof(context));
    dport = iros_strtol(argv[0]);
    if (dport < 1 || (dport > 4 && dport != 0xff))
    {
        diag_printf("d-port error\n");
        return;
    }
    if (epon_request_onu_frame_send (context, 0, 0, pkt, 64, dport, 0) != CS_OK)
    {
        diag_printf("tx error\n");
        return;
    }

    diag_printf("%d bytes was sent out\n", (cs_int32)sizeof(pkt));

    return;
}



static void __sw_l2_cmd_rma_action_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_api_ret_t temp_mac[6];
    rtk_mac_t mgmt_mac;
    int para2, i = 0;
    void *ptr = NULL;
    
    
    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }
    
    if(aal_cmd_mac_match(argv[0]) != CS_E_OK)
    {
        diag_printf("mac address error\n");
        return;
    }
    
    ptr = argv[0];
    sscanf(ptr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp_mac[0], &temp_mac[1], &temp_mac[2], &temp_mac[3], &temp_mac[4], &temp_mac[5]);

    for (i=0; i<6; i++)
    {
        mgmt_mac.octet[i] = temp_mac[i];
    }

    para2 = iros_strtol(argv[1]);
    if (para2 >= RMA_ACTION_END)
    {
        diag_printf("action error\n");
        return;
    }

    rtn = rtk_trap_rmaAction_set(&mgmt_mac, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_l2_cmd_rma_action_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_mac_t mgmt_mac;
    rtk_uint8 bpdu_mac[] = {0x1, 0x80, 0xc2, 0, 0, 0};
    int i = 0;
    rtk_trap_rma_action_t action = RMA_ACTION_END;
    char *rma_action_str[] = {"forward", "cpu", "drop", "forward except for cpu", "invalid"};
   
    memcpy(&mgmt_mac, bpdu_mac, sizeof(bpdu_mac));

    for (i=0; i<0x10; i++)
    {
        rtn = rtk_trap_rmaAction_get(&mgmt_mac, &action);
        if (rtn)
            diag_printf("errcode(%d)\n", rtn);
        else
            diag_printf("reserved-mc-addr(%02x:%02x:%02x:%02x:%02x:%02x), action(%d:%s)\n",
               mgmt_mac.octet[0], mgmt_mac.octet[1], mgmt_mac.octet[2],
               mgmt_mac.octet[3], mgmt_mac.octet[4], mgmt_mac.octet[5],
               action, rma_action_str[action]);
        mgmt_mac.octet[5]++;
    }
    
    return;
}


#ifdef __RTK_MIRROR
static void __sw_l2_cmd_mirror_port_based_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    int para3;

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("mirror port error\n");
        return;
    }

    if (para2 > RTK_MAX_PORT_MASK)
    {
        diag_printf("mirrored rx port error\n");
        return;
    }

    if (para3 > RTK_MAX_PORT_MASK)
    {
        diag_printf("mirrored tx port error\n");
        return;
    }

    rtn = rtk_mirror_portBased_set(para1, (rtk_portmask_t *)&para2, (rtk_portmask_t *)&para3);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_l2_cmd_mirror_port_based_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_port_t mirror_dst_port;
    rtk_portmask_t rx, tx;

    rtn = rtk_mirror_portBased_get(&mirror_dst_port, &rx, &tx);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("mirroring-port(%d), mirrored-rx-portmask(%04xh), mirrored-tx-portmask(%04xh)\n", 
            mirror_dst_port, rx.bits[0], tx.bits[0]);

    return;
}

static void __sw_l2_cmd_mirror_isolation_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 > 1)
    {
        diag_printf("enable error\n");
        return;
    }

    rtn = rtk_mirror_portIso_set(para1);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_l2_cmd_mirror_isolation_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_enable_t enable;

    rtn = rtk_mirror_portIso_get(&enable);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("mirror-isolation(%d:%s)\n", enable, enable ? "enabled" : "disabled");

    return;
}
#endif

static void __sw_l2_cmd_unkwn_uc_action_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);

    if (para1 >= UCAST_END)
    {
        diag_printf("ucast type error\n");
        return;
    }

    if (para2 >= UCAST_ACTION_END)
    {
        diag_printf("action error\n");
        return;
    }

    rtn = rtk_trap_unknownUnicastPktAction_set(para1, para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_l2_cmd_unkwn_uc_action_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    rtk_trap_ucast_action_t action;
    char *action_str[] = {"forward", "drop", "trap-to-cpu", "invalid"};
    char *uc_type_str[] = {"unknown-da", "unknown-sa", "unmatch-sa", "invalid"};

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);

    if (para1 >= UCAST_END)
    {
        diag_printf("ucast type error\n");
        return;
    }

    rtn = rtk_trap_unknownUnicastPktAction_get(para1, &action);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("uc-type(%d:%s), action(%d:%s)\n", para1, uc_type_str[para1], action, action_str[action]);

    return;
}
#if 0
static void __sw_l2_cmd_unkwn_mc_action_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    int para3;

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);

    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 >= MCAST_END)
    {
        diag_printf("mcast type error\n");
        return;
    }

    if (para3 >= MCAST_ACTION_END)
    {
        diag_printf("action error\n");
        return;
    }

    rtn = rtk_trap_unknownMcastPktAction_set(para1, para2, para3);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}
#else
static void __sw_l2_cmd_unkwn_mc_action_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    cs_callback_context_t context;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);


    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = epon_request_onu_unknown_mc_forward_set(context, 0, 0, para1, (para2!=0));
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}
#endif

static void __sw_l2_cmd_unkwn_mc_action_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    rtk_trap_mcast_action_t action;
    char *action_str[] = {"forward", "drop", "trap-to-cpu", "invalid"};
    char *mc_type_str[] = {"mc-l2", "mc-ipv4", "mc-ipv6", "invalid"};

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    
    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (para2 >= MCAST_END)
    {
        diag_printf("ucast type error\n");
        return;
    }

    rtn = rtk_trap_unknownMcastPktAction_get(para1, para2, &action);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("mc-type(%d:%s), action(%d:%s)\n", para2, mc_type_str[para2], action, action_str[action]);

    return;
}


/*******************************************************************************/
static struct {
    char                   *name;
    aal_l2_cmd_func_t  exe_func;
    int                     argc;
    char                   *para;
    char                   *help;
}sw_l2_cmds[] = {
        { "l2-init",                __sw_l2_cmd_l2_init               , 0, "<cr>", "initialize l2 engine"},
        { "mac-learned-get",        __sw_l2_cmd_mac_learned_get       , 1, "[port]",             "get port's learned mac numbers"},
        { "mac-aging-enable-set",   __sw_l2_cmd_mac_aging_enable_set  , 2, "[port] [enable]",    "set port's aging enable state"},
        { "mac-aging-enable-get",   __sw_l2_cmd_mac_aging_enable_get  , 1, "[port]",             "get port's aging enable state"},
        { "mac-aging-time-set",     __sw_l2_cmd_mac_aging_time_set    , 1, "[aging-time]",       "set default aging time"},
        { "mac-aging-time-get",     __sw_l2_cmd_mac_aging_time_get    , 0, "<cr>",               "get default aging time"},
        { "mac-limit-set",          __sw_l2_cmd_mac_limit_set         , 2, "[port] [limit]",     "set port's max learnable mac address number"},
        { "mac-limit-get",          __sw_l2_cmd_mac_limit_get         , 1, "[port]",             "get port's max learnable mac address number"},
        { "mac-limit-action-set",   __sw_l2_cmd_mac_limit_action_set  , 1, "[action]",           "set port's unknwon sa packet action when limit number is over"},
        { "mac-limit-action-get",   __sw_l2_cmd_mac_limit_action_get  , 0, "<cr>",               "get port's unknown sa packet action when limit number is over"},
        { "isolation-set",          __sw_l2_cmd_isolation_set         , 2, "[port] [portbmp]",   "set port's isolation mask"},
        { "isolation-get",          __sw_l2_cmd_isolation_get         , 1, "[port]",             "get port's isolation mask"},
        { "flood-portbmp-set",      __sw_l2_cmd_flood_portbmp_set     , 2, "[type] [portbmp]",   "set flood port bitmap for unknown uc, mc or bc"},
        { "flood-portbmp-get",      __sw_l2_cmd_flood_portbmp_get     , 1, "[type]",             "get flood port bitmap for unknown uc, mc or bc"},
        { "stp-state-set",          __sw_l2_cmd_stp_state_set         , 2, "[port] [stp-state]", "set port's stp state"},
        { "stp-state-get",          __sw_l2_cmd_stp_state_get         , 1, "[port]",             "get port's stp state"},
        { "cpu-port-set",           __sw_l2_cmd_cpu_port_set          , 2, "[port] [tag-mode]",  "set cpu port and tag mode"},
        { "cpu-port-get",           __sw_l2_cmd_cpu_port_get          , 0, "<cr>",               "get cpu port and tag mode"},
        { "cpu-dump-set",           __sw_l2_cmd_cpu_dump_set          , 1, "[debug-flag]",       "set cpu packet debug mode"},        
        { "cpu-tx-test",            __sw_l2_cmd_cpu_tx_test           , 1, "[d-port]",           "cpu tx simulation"},
        { "rma-action-set",         __sw_l2_cmd_rma_action_set        , 2, "[mc-addr] [action]",  "set reserved multicast action"},
        { "rma-action-get",         __sw_l2_cmd_rma_action_get        , 0, "<cr>",                "get reserved multicast action"},
#ifdef __RTK_MIRROR        
        { "mirror-portbased-set",   __sw_l2_cmd_mirror_port_based_set , 3, "[mirroring-port] [mirrored-rx-portmask] [mirrored-tx-portmask]", "set mirror port mask"},
        { "mirror-portbased-get",   __sw_l2_cmd_mirror_port_based_get , 0, "<cr>",                             "get mirror port mask"},
        { "mirror-isolation-set",   __sw_l2_cmd_mirror_isolation_set  , 1, "[enable]",            "set mirror isolation"},
        { "mirror-isolation-get",   __sw_l2_cmd_mirror_isolation_get  , 0, "<cr>",                "get mirror isolation"},
#endif
        { "unkwn-uc-action-set",    __sw_l2_cmd_unkwn_uc_action_set   , 2, "[uc-type] [action]",  "set forwarding behaviour for unknown uc"},
        { "unkwn-uc-action-get",    __sw_l2_cmd_unkwn_uc_action_get   , 1, "[uc-type]",                  "get forwarding behaviour for unknown uc"},
        { "unkwn-mc-action-set",    __sw_l2_cmd_unkwn_mc_action_set   , 2, "[port] [enable]",  "set forwarding behaviour for unknown mc"},
        { "unkwn-mc-action-get",    __sw_l2_cmd_unkwn_mc_action_get   , 2, "[port] [mc-type]", "get forwarding behaviour for unknown mc"},
};

void __sw_l2_cmd_help(void)
{
    int i;

    for (i = 0; i < sizeof(sw_l2_cmds) / sizeof(sw_l2_cmds[0]); i++) {  
        __SW_L2_PRINT_CMD_HELP("l2", sw_l2_cmds[i].name, sw_l2_cmds[i].help);    
    }
}

static aal_l2_cmd_func_t  __sw_l2_find_cmd
(
    const char *cmd_str,
    int   argc
)
{
    int i = 0;

    if (NULL == cmd_str) {
        return NULL;
    }

    for (i = 0;
        i < (sizeof(sw_l2_cmds)/ sizeof(sw_l2_cmds[0]));
        ++i) {
        if (__sw_l2_cmd_cmp(sw_l2_cmds[i].name, cmd_str)){

            if ((argc - 3) == sw_l2_cmds[i].argc) {
                return sw_l2_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", sw_l2_cmds[i].argc);
                diag_printf("parameters:  %s\n", sw_l2_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __sw_l2_cmd_help();

    return (NULL);

}


void sw_l2_cmd(int argc, char **argv)
{
    aal_l2_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal fdb  ..."*/

        p_cmd_func = __sw_l2_find_cmd(argv[2], argc);

    }
    else {
        __sw_l2_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}

static void __sw_igmp_cmd_init(int  argc, char **argv)
{
    rtk_api_ret_t rtn = RT_ERR_FAILED;

    rtn = rtk_igmp_init();

    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");
    
    return;
}


static void __sw_igmp_cmd_state_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    if (para1 > 1)
    {
        diag_printf("enable error\n");
        return;
    }

    rtn = rtk_igmp_state_set(para1);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_igmp_cmd_state_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_enable_t enable;

    rtn = rtk_igmp_state_get(&enable);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("igmp-state(%d:%s)\n", enable, enable ? "enabled" : "disabled");

    return;
}

static void __sw_igmp_cmd_router_port_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    rtk_portmask_t port_bmp;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    if (para1 > RTK_MAX_PORT_MASK)
    {
        diag_printf("port bitmap error\n");
        return;
    }

    port_bmp.bits[0] = para1;

    rtn = rtk_igmp_static_router_port_set(port_bmp);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_igmp_cmd_router_port_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_portmask_t port_bmp;

    rtn = rtk_igmp_static_router_port_get(&port_bmp);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("router-port-bitmap(%04x)\n", port_bmp.bits[0]);

    return;
}

static void __sw_igmp_cmd_protocol_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    int para3;

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    para2 = iros_strtol(argv[1]);
    if (para2 >= PROTOCOL_END)
    {
        diag_printf("protocol error\n");
        return;
    }

    para3 = iros_strtol(argv[2]);
    if (para3 >= IGMP_ACTION_END)
    {
        diag_printf("action error\n");
        return;
    }

    rtn = rtk_igmp_protocol_set(para1, para2, para3);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_igmp_cmd_protocol_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_trap_igmp_action_t action;
    int para1;
    int para2;
    char *proto_str[]  = {"igmp-v1", "igmp-v2", "igmp-v3", "mld-v1", "mld-v2", "invalid"};
    char *action_str[] =
        {"forward", "cpu", "drop", "forward-exclude-cpu", "asic"};
    
    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    para2 = iros_strtol(argv[1]);
    if (para2 >= PROTOCOL_END)
    {
        diag_printf("protocol error\n");
        return;
    }

    rtn = rtk_igmp_protocol_get(para1, para2, &action);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("protocol(%d:%s), igmp-action(%d:%s)\n", para2, proto_str[para2], action, action_str[action]);

    return;
}


static void __sw_igmp_cmd_set(int argc, char **argv)
{
#if 0
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    cs_callback_context_t     context;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    if (para1 > UNI_PORT_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    para2 = iros_strtol(argv[1]);

    rtn = epon_request_onu_igmp_state_set(context, 0, 0, para1, (para2!=0));
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");
#endif
    return;
}


/*******************************************************************************/
static struct {
    char                   *name;
    aal_l2_cmd_func_t  exe_func;
    int                     argc;
    char                   *para;
    char                   *help;
}sw_igmp_cmds[] = {
        { "init",              __sw_igmp_cmd_init       ,     0, "<cr>",       "initialize igmp/mld engine"},
        { "state-set",         __sw_igmp_cmd_state_set  ,     1, "[enable]",   "set igmp state"},
        { "state-get",         __sw_igmp_cmd_state_get  ,     0, "<cr>",       "get igmp state"},
        { "router-port-set",   __sw_igmp_cmd_router_port_set, 1, "[portmask]", "set igmp router port mask"},
        { "router-port-get",   __sw_igmp_cmd_router_port_get, 0, "<cr>",       "get igmp router port mask"},        
        { "protocol-set",      __sw_igmp_cmd_protocol_set,    3, "[port] [proto] [action]", "set igmp protocol action"},
        { "protocol-get",      __sw_igmp_cmd_protocol_get,    2, "[port] [proto]",          "get igmp protocol action"},
        { "set",               __sw_igmp_cmd_set,             2, "[port] [enable]",          "set igmp enable/disable"},

};

void __sw_igmp_cmd_help(void)
{
    int i;

    for (i = 0; i < sizeof(sw_igmp_cmds) / sizeof(sw_igmp_cmds[0]); i++) {  
        __SW_L2_PRINT_CMD_HELP("igmp", sw_igmp_cmds[i].name, sw_igmp_cmds[i].help);    
    }
}

static aal_l2_cmd_func_t  __sw_igmp_find_cmd
(
    const char *cmd_str,
    int   argc
)
{
    int i = 0;

    if (NULL == cmd_str) {
        return NULL;
    }

    for (i = 0;
        i < (sizeof(sw_igmp_cmds)/ sizeof(sw_igmp_cmds[0]));
        ++i) {
        if (__sw_l2_cmd_cmp(sw_igmp_cmds[i].name, cmd_str)){

            if ((argc - 3) == sw_igmp_cmds[i].argc) {
                return sw_igmp_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", sw_igmp_cmds[i].argc);
                diag_printf("parameters:  %s\n", sw_igmp_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __sw_igmp_cmd_help();

    return (NULL);

}


void sw_igmp_cmd(int argc, char **argv)
{
    aal_l2_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal fdb  ..."*/

        p_cmd_func = __sw_igmp_find_cmd(argv[2], argc);

    }
    else {
        __sw_igmp_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}


static void __sw_trunk_cmd_port_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    if (para1 >= TRUNK_GROUP2)
    {
        diag_printf("trunk group error\n");
        return;
    }

    para2 = iros_strtol(argv[1]);
    if (para2 > RTK_MAX_PORT_MASK);
    {
        diag_printf("port mask error\n");
        return;
    }

    rtn = rtk_trunk_port_set(para1, *(rtk_portmask_t *)&para2);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_trunk_cmd_port_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_portmask_t port_bmp;
    int para1;
    
    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    if (para1 >= TRUNK_GROUP2)
    {
        diag_printf("trunk group error\n");
        return;
    }

    rtn = rtk_trunk_port_get(para1, &port_bmp);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("trunk-member-bitmap(%04xh)\n", port_bmp.bits[0]);

    return;
}


static void __sw_trunk_cmd_hash_algorithm_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    rtk_trunk_hashVal2Port_t hash_algo;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    if (para1 >= TRUNK_GROUP2)
    {
        diag_printf("trunk group error\n");
        return;
    }

    hash_algo.value[0] = iros_strtol(argv[1]);
    if (hash_algo.value[0]  >= 0x80);
    {
        diag_printf("hash algorithm bitmap error\n");
        return;
    }

    rtn = rtk_trunk_distributionAlgorithm_set(para1, hash_algo);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_trunk_cmd_hash_algorithm_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_trunk_hashVal2Port_t hash_algo;
    int para1;
    
    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    if (para1 >= TRUNK_GROUP2)
    {
        diag_printf("trunk group error\n");
        return;
    }

    rtn = rtk_trunk_distributionAlgorithm_get(para1, &hash_algo);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("hash-algorithm--bitmap(%02xh)\n", hash_algo.value[0]);

    return;
}


/*******************************************************************************/
static struct {
    char                   *name;
    aal_l2_cmd_func_t  exe_func;
    int                     argc;
    char                   *para;
    char                   *help;
}sw_trunk_cmds[] = {
        { "port-set",         __sw_trunk_cmd_port_set,               2, "[group-id] [member-bitmap]",  "trunk group set"},
        { "port-get",         __sw_trunk_cmd_port_get,               1, "[group-id]",                  "trunk group get"},
        { "hash-ag-set",      __sw_trunk_cmd_hash_algorithm_set,     2, "[group-id] [algo-bitmap]",    "trunk group hash algorithm set"},
        { "hash-ag-get",      __sw_trunk_cmd_hash_algorithm_get,     1, "[group-id]",                  "trunk group hash algorithm get"},
};

void __sw_trunk_cmd_help(void)
{
    int i;

    for (i = 0; i < sizeof(sw_trunk_cmds) / sizeof(sw_trunk_cmds[0]); i++) {  
        __SW_L2_PRINT_CMD_HELP("trunk", sw_trunk_cmds[i].name, sw_trunk_cmds[i].help);    
    }
}

static aal_l2_cmd_func_t  __sw_trunk_find_cmd
(
    const char *cmd_str,
    int   argc
)
{
    int i = 0;

    if (NULL == cmd_str) {
        return NULL;
    }

    for (i = 0;
        i < (sizeof(sw_trunk_cmds)/ sizeof(sw_trunk_cmds[0]));
        ++i) {
        if (__sw_l2_cmd_cmp(sw_trunk_cmds[i].name, cmd_str)){

            if ((argc - 3) == sw_trunk_cmds[i].argc) {
                return sw_trunk_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", sw_trunk_cmds[i].argc);
                diag_printf("parameters:  %s\n", sw_trunk_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __sw_trunk_cmd_help();

    return (NULL);

}


void sw_trunk_cmd(int argc, char **argv)
{
    aal_l2_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal fdb  ..."*/

        p_cmd_func = __sw_trunk_find_cmd(argv[2], argc);

    }
    else {
        __sw_trunk_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}


extern cs_status aal_cmd_mac_match(char *str);


static void __sw_fdb_cmd_ucast_add(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_l2_ucastAddr_t l2_addr_entry;
    rtk_mac_t mac;
    unsigned int temp_mac[6], i = 0;
    void *ptr = NULL;


    memset(&l2_addr_entry, 0, sizeof(l2_addr_entry));

    if (argc != 8)
    {
        diag_printf("parameter number error\n");
        return;
    }

    if(aal_cmd_mac_match(argv[0]) != CS_E_OK)
    {
        diag_printf("mac address error\n");
        return;
    }
    
    ptr = argv[0];
    sscanf(ptr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp_mac[0], &temp_mac[1], &temp_mac[2], &temp_mac[3], &temp_mac[4], &temp_mac[5]);

    for (i=0; i<6; i++)
    {
        mac.octet[i] = temp_mac[i];
    }

    l2_addr_entry.fid  = iros_strtol(argv[1]);
    l2_addr_entry.efid = iros_strtol(argv[2]);
    l2_addr_entry.port = iros_strtol(argv[3]);
    l2_addr_entry.sa_block  = iros_strtol(argv[4]);
    l2_addr_entry.da_block  = iros_strtol(argv[5]);
    l2_addr_entry.auth      = iros_strtol(argv[6]);
    l2_addr_entry.is_static = iros_strtol(argv[7]);

    if (l2_addr_entry.port > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    if (l2_addr_entry.auth > 1)
    {
        diag_printf("auth indicator error\n");
        return;
    }

    if (l2_addr_entry.sa_block > 1)
    {
        diag_printf("sa block indicator error\n");
        return;
    }

    if (l2_addr_entry.is_static > 1)
    {
        diag_printf("static indicator error\n");
        return;
    }
    
    memcpy(&l2_addr_entry.mac, &mac, sizeof(mac));
        
    rtn = rtk_l2_addr_add(&mac, &l2_addr_entry);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_fdb_cmd_ucast_get(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_l2_ucastAddr_t l2_addr_entry;
    rtk_mac_t mac;
    unsigned int temp_mac[6], i = 0;
    void *ptr = NULL;

    memset(&l2_addr_entry, 0, sizeof(l2_addr_entry));

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    if(aal_cmd_mac_match(argv[0]) != CS_E_OK)
    {
        diag_printf("mac address error\n");
        return;
    }
    
    ptr = argv[0];
    sscanf(ptr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp_mac[0], &temp_mac[1], &temp_mac[2], &temp_mac[3], &temp_mac[4], &temp_mac[5]);

    for (i=0; i<6; i++)
    {
        mac.octet[i] = temp_mac[i];
    }

    l2_addr_entry.fid  = iros_strtol(argv[1]);
    l2_addr_entry.efid = iros_strtol(argv[2]);

    rtn = rtk_l2_addr_get(&mac, &l2_addr_entry);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
    {
        diag_printf("mac(%02x:%02x:%02x:%02x:%02x:%02x) fid(%d) efid(%d) port(%d) sa_block(%d) auth(%d) sa_block(%d)",
            l2_addr_entry.mac.octet[0],
            l2_addr_entry.mac.octet[1],
            l2_addr_entry.mac.octet[2],
            l2_addr_entry.mac.octet[3],
            l2_addr_entry.mac.octet[4],
            l2_addr_entry.mac.octet[5],
            l2_addr_entry.fid,
            l2_addr_entry.efid,
            l2_addr_entry.port,
            l2_addr_entry.sa_block,
            l2_addr_entry.auth,
            l2_addr_entry.is_static);
    }

    return;
}

static void __sw_fdb_cmd_ucast_del(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_l2_ucastAddr_t l2_addr_entry;
    rtk_mac_t mac;
    unsigned int temp_mac[6], i = 0;
    void *ptr = NULL;

    memset(&l2_addr_entry, 0, sizeof(l2_addr_entry));

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    if(aal_cmd_mac_match(argv[0]) != CS_E_OK)
    {
        diag_printf("mac address error\n");
        return;
    }
    
    ptr = argv[0];
    sscanf(ptr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp_mac[0], &temp_mac[1], &temp_mac[2], &temp_mac[3], &temp_mac[4], &temp_mac[5]);

    for (i=0; i<6; i++)
    {
        mac.octet[i] = temp_mac[i];
    }

    l2_addr_entry.fid  = iros_strtol(argv[1]);
    l2_addr_entry.efid = iros_strtol(argv[2]);

    rtn = rtk_l2_addr_del(&mac, &l2_addr_entry);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_fdb_cmd_ucast_get_next(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_l2_ucastAddr_t l2_addr_entry;
    rtk_l2_read_method_t read_method;
    rtk_uint32 port = 0;
    rtk_uint32 next_offset = 0;;

    memset(&l2_addr_entry, 0, sizeof(l2_addr_entry));

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    read_method = iros_strtol(argv[0]);
    if (read_method != READMETHOD_NEXT_L2UC && read_method != READMETHOD_NEXT_L2UCSPA)
    {
        diag_printf("read method error\n");
        return;
    }

    port = iros_strtol(argv[1]);
    if (port > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    next_offset = iros_strtol(argv[2]);
    
    rtn = rtk_l2_addr_next_get(read_method, port, &next_offset, &l2_addr_entry);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
    {
        diag_printf("offset(%d), mac(%02x:%02x:%02x:%02x:%02x:%02x), fid(%d), efid(%d), port(%d), sa_block(%d), auth(%d), sa_block(%d)",
            next_offset,
            l2_addr_entry.mac.octet[0],
            l2_addr_entry.mac.octet[1],
            l2_addr_entry.mac.octet[2],
            l2_addr_entry.mac.octet[3],
            l2_addr_entry.mac.octet[4],
            l2_addr_entry.mac.octet[5],
            l2_addr_entry.fid,
            l2_addr_entry.efid,
            l2_addr_entry.port,
            l2_addr_entry.sa_block,
            l2_addr_entry.auth,
            l2_addr_entry.is_static);
    }

    return;
}
#if 0
static void __sw_fdb_cmd_mcast_add(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_mac_t mac;
    unsigned int temp_mac[6], i = 0;
    void *ptr = NULL;
    rtk_fid_t fid = 0;
    rtk_portmask_t port_mask;

    memset(&port_mask, 0, sizeof(port_mask));

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    if(aal_cmd_mac_match(argv[0]) != CS_E_OK)
    {
        diag_printf("mac address error\n");
        return;
    }
    
    ptr = argv[0];
    sscanf(ptr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp_mac[0], &temp_mac[1], &temp_mac[2], &temp_mac[3], &temp_mac[4], &temp_mac[5]);

    for (i=0; i<6; i++)
    {
        mac.octet[i] = temp_mac[i];
    }

    fid               = iros_strtol(argv[1]);
    port_mask.bits[0] = iros_strtol(argv[2]);
    
    rtn = rtk_l2_mcastAddr_add(&mac, fid, port_mask);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}
#else
static void __sw_fdb_cmd_mcast_add(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    unsigned int temp_mac[6], i = 0;
    void *ptr = NULL;
    cs_port_id_t port;
    cs_callback_context_t context;
    cs_sdl_mc_l2_entry_t  entry;


    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    if(aal_cmd_mac_match(argv[0]) != CS_E_OK)
    {
        diag_printf("mac address error\n");
        return;
    }
    
    ptr = argv[0];
    sscanf(ptr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp_mac[0], &temp_mac[1], &temp_mac[2], &temp_mac[3], &temp_mac[4], &temp_mac[5]);

    for (i=0; i<6; i++)
    {
        entry.mac.addr[i] = temp_mac[i];
    }

    entry.vlan = iros_strtol(argv[1]);
    port = iros_strtol(argv[2]);
    
    rtn = epon_request_onu_mc_l2_entry_add(context, 0, 0, port , &entry);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}
#endif

#if 0
static void __sw_fdb_cmd_mcast_del(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_mac_t mac;
    unsigned int temp_mac[6], i = 0;
    void *ptr = NULL;
    rtk_fid_t fid = 0;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    if(aal_cmd_mac_match(argv[0]) != CS_E_OK)
    {
        diag_printf("mac address error\n");
        return;
    }
    
    ptr = argv[0];
    sscanf(ptr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp_mac[0], &temp_mac[1], &temp_mac[2], &temp_mac[3], &temp_mac[4], &temp_mac[5]);

    for (i=0; i<6; i++)
    {
        mac.octet[i] = temp_mac[i];
    }

    fid = iros_strtol(argv[1]);
    
    rtn = rtk_l2_mcastAddr_del(&mac, fid);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}
#else
static void __sw_fdb_cmd_mcast_del(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    unsigned int temp_mac[6], i = 0;
    void *ptr = NULL;
    cs_callback_context_t context;
    cs_sdl_mc_l2_entry_t  entry;


    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    if(aal_cmd_mac_match(argv[0]) != CS_E_OK)
    {
        diag_printf("mac address error\n");
        return;
    }
    
    ptr = argv[0];
    sscanf(ptr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp_mac[0], &temp_mac[1], &temp_mac[2], &temp_mac[3], &temp_mac[4], &temp_mac[5]);

    for (i=0; i<6; i++)
    {
        entry.mac.addr[i] = temp_mac[i];
    }
    
    entry.vlan = iros_strtol(argv[1]);

    rtn = epon_request_onu_mc_l2_entry_del(context,0,0,CS_UNI_PORT_ID1, &entry);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}
#endif


#if 0
static void __sw_fdb_cmd_mcast_get(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_mac_t mac;
    unsigned int temp_mac[6], i = 0;
    void *ptr = NULL;
    rtk_fid_t fid = 0;
    rtk_portmask_t port_mask;

    memset(&port_mask, 0, sizeof(port_mask));

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    if(aal_cmd_mac_match(argv[0]) != CS_E_OK)
    {
        diag_printf("mac address error\n");
        return;
    }
    
    ptr = argv[0];
    sscanf(ptr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp_mac[0], &temp_mac[1], &temp_mac[2], &temp_mac[3], &temp_mac[4], &temp_mac[5]);

    for (i=0; i<6; i++)
    {
        mac.octet[i] = temp_mac[i];
    }

    fid = iros_strtol(argv[1]);
    
    rtn = rtk_l2_mcastAddr_get(&mac, fid, &port_mask);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("port-mask(%04xh)\n", port_mask.bits[0]);

    return;
}
#else
static void __sw_fdb_cmd_mcast_get(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    unsigned int temp_mac[6], i = 0;
    void *ptr = NULL;
    cs_sdl_portmask_t port_mask;
    cs_callback_context_t context;
    cs_sdl_mc_l2_entry_t  entry;

    memset(&port_mask, 0, sizeof(port_mask));

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    if(aal_cmd_mac_match(argv[0]) != CS_E_OK)
    {
        diag_printf("mac address error\n");
        return;
    }
    
    ptr = argv[0];
    sscanf(ptr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp_mac[0], &temp_mac[1], &temp_mac[2], &temp_mac[3], &temp_mac[4], &temp_mac[5]);

    for (i=0; i<6; i++)
    {
        entry.mac.addr[i] = temp_mac[i];
    }
    
    rtn = epon_request_onu_mc_l2_port_get(context,0,0,&entry,&port_mask);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("port-mask(%04xh)\n", port_mask.bits[0]);

    return;
}
#endif

static void __sw_fdb_cmd_l2_entry_get(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_l2_addr_table_t l2_entry;

    memset(&l2_entry, 0, sizeof(l2_entry));

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }

    l2_entry.index = iros_strtol(argv[0]);
    if (l2_entry.index < 1 || l2_entry.index > 2112)
    {
        diag_printf("index error\n");
        return;
    }
    
    rtn = rtk_l2_entry_get(&l2_entry);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
    {
        diag_printf("index(%04d) mac(%02x:%02x:%02x:%02x:%02x:%02x) port-mask(%04xh)\nsa-block(%d) auth(%d) aging(%04d)    fid(%d) static(%d)\n",
            l2_entry.index, l2_entry.mac.octet[0], l2_entry.mac.octet[1], l2_entry.mac.octet[2],
            l2_entry.mac.octet[3], l2_entry.mac.octet[4], l2_entry.mac.octet[5], l2_entry.portmask,
            l2_entry.sa_block, l2_entry.auth, l2_entry.age, l2_entry.fid, l2_entry.is_static);
    }

    return;
}

static void __sw_fdb_cmd_ipmcast_add(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    ipaddr_t sip =0,dip =0;
    rtk_portmask_t port_mask;

    memset(&port_mask, 0, sizeof(port_mask));

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }
    
    if((cmd_ipv4_match(argv[0]) != CS_E_OK)||
        (cmd_ipv4_match(argv[1]) != CS_E_OK))
    {
        diag_printf("IP address error\n");
        return;
    }
    
    cs_str2ip(argv[0], &sip);
    cs_str2ip(argv[1], &dip);
   
    port_mask.bits[0] = iros_strtol(argv[2]);
      
    rtn = rtk_l2_ipMcastAddr_add( htonl(sip), htonl(dip), port_mask);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_fdb_cmd_ipmcast_del(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    ipaddr_t sip =0,dip =0;

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    if((cmd_ipv4_match(argv[0]) != CS_E_OK)||
        (cmd_ipv4_match(argv[1]) != CS_E_OK))
    {
        diag_printf("IP address error\n");
        return;
    }
    
    cs_str2ip(argv[0], &sip);
    cs_str2ip(argv[1], &dip);
    
    rtn = rtk_l2_ipMcastAddr_del(htonl(sip), htonl(dip));
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_fdb_cmd_unkmc_action_set(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    int para1;
    int para2;
    int para3;

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    para2 = iros_strtol(argv[1]);
    if (para2 >= MCAST_END)
    {
        diag_printf("mc type error\n");
        return;
    }

    para3 = iros_strtol(argv[2]);
    if (para3 >= MCAST_ACTION_END)
    {
        diag_printf("action error\n");
        return;
    }

    rtn = rtk_trap_unknownMcastPktAction_set(para1, para2, para3);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_fdb_cmd_unkmc_action_get(int argc, char **argv)
{
    rtk_api_ret_t rtn;
    rtk_trap_mcast_action_t action;
    int para1;
    int para2;
    char *type_str[]  = {"L2", "IPV4", "IPV6"};
    char *action_str[] = {"forward", "drop", "to cpu"};
    
    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    para1 = iros_strtol(argv[0]);
    if (para1 > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    para2 = iros_strtol(argv[1]);
    if (para2 >= MCAST_END)
    {
        diag_printf("mc type error\n");
        return;
    }

    rtn = rtk_trap_unknownMcastPktAction_get(para1, para2, &action);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("MC(%d:%s), action(%d:%s)\n", para2, type_str[para2], action, action_str[action]);

    return;
}

static void __sw_fdb_cmd_mc_lookup_set(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED; 
    rtk_l2_lookup_type_t type;

    if (argc != 1)
    {
        diag_printf("parameter number error\n");
        return;
    }
       
    if ( iros_strtol(argv[0]) > 2)
    {
        diag_printf(" type error\n");
        return;
    }
    
    type = iros_strtol(argv[0]);
    
    rtn = rtk_l2_ipMcastAddrLookup_set(type);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_fdb_cmd_mc_lookup_get(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED; 
    rtk_l2_lookup_type_t type;
    
    rtn = rtk_l2_ipMcastAddrLookup_get(&type);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("MC_lookup_type(%s)\n",  (type) ? ((type==LOOKUP_DIP) ? "DIP" : "SIP+DIP") : "MAC");

    return;
}

static void __sw_fdb_cmd_ipmcgrp_add(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    ipaddr_t ipgr =0;
  

    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }
    if(iros_strtol(argv[0])>63)
    {
        diag_printf("index must be (0-63)\n");
        return;
    }    
    if(cmd_ipv4_match(argv[1]) != CS_E_OK)
    {
        diag_printf("IP address error\n");
        return;
    }
    
    cs_str2ip(argv[1], &ipgr);
    
    rtn = rtl8367b_setAsicLutIPMCGroup(iros_strtol(argv[0]), ipgr);
     
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_fdb_cmd_ipmcgrp_get(int  argc, char **argv)
{
    rtk_api_ret_t rtn  = RT_ERR_FAILED;
    ipaddr_t ipgrp =0;
    int i;
    char d_buf[20];

    for(i =0; i<64; i++)
    {
        rtn = rtl8367b_getAsicLutIPMCGroup( i, &ipgrp);
        if((rtn!= RT_ERR_OK)||(ipgrp == 0xE0000000))
            continue;
        
        cs_ip2str(d_buf, ipgrp);
        
        diag_printf("| index(%04d) |    IP MC group IP %-15s   |\n", i, d_buf);
    }

    return;
}
static void __sw_fdb_cmd_flush(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    rtk_l2_flushType_t flush_type = FLUSH_TYPE_BY_PORT;
    rtk_vlan_t         vlanid = 0;
    rtk_l2_flushItem_t port = 0;

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    flush_type = iros_strtol(argv[0]);
    if (flush_type >= FLUSH_TYPE_END)
    {
        diag_printf("flush type error, 0 - by port, 1 - by port+vid\n");
        return;
    }
    
    vlanid     = iros_strtol(argv[1]);
    if (vlanid > 0xfff)
    {
        diag_printf("vlan-id error\n");
        return;
    }
    
    port = iros_strtol(argv[2]);
    if (port > RTK_PORT_ID_MAX)
    {
        diag_printf("port error\n");
        return;
    }

    rtn = rtk_l2_flushType_set(flush_type, vlanid, port);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_fdb_cmd_query(int  argc, char **argv)
{
    rtk_api_ret_t rtn  = RT_ERR_FAILED;
    rtk_l2_ucastAddr_t l2_data;
    rtk_uint32 address = 0, total = 0;;

    while (1)
    {
        if ((rtn = rtk_l2_addr_next_get(READMETHOD_NEXT_L2UC, 0, &address, &l2_data)) != RT_ERR_OK)
            break;

        diag_printf("+-------------+------------------------------------------------------------------------+\n");
        diag_printf("| index(%04d) | mac(%02x:%02x:%02x:%02x:%02x:%02x) port(%d) vlan(%d) fid(%d) efid(%d) sa-block(%d) da-block(%d) is-static(%s) |\n",
            address, 
            l2_data.mac.octet[0], l2_data.mac.octet[1], l2_data.mac.octet[2],
            l2_data.mac.octet[3], l2_data.mac.octet[4], l2_data.mac.octet[5],
            l2_data.port, l2_data.cvid, l2_data.fid, l2_data.efid, l2_data.sa_block, l2_data.da_block, (l2_data.is_static ? "y" : "n"));

        address++;
        total++;

        if(total % 10 == 0) {
            cs_thread_delay(10);
        }  
    }
    
    address = 0;
    while (1)
    {
        if ((rtn = rtk_l2_addr_next_get(READMETHOD_NEXT_L2MC, 0, &address, &l2_data)) != RT_ERR_OK)
            break;

        diag_printf("+-------------+------------------------------------------------------------------------+\n");
        diag_printf("| index(%04d) | mac(%02x:%02x:%02x:%02x:%02x:%02x) port(%d) vlan(%d) fid(%d) efid(%d) sa-block(%d) da-block(%d) is-static(%s) |\n",
            address, 
            l2_data.mac.octet[0], l2_data.mac.octet[1], l2_data.mac.octet[2],
            l2_data.mac.octet[3], l2_data.mac.octet[4], l2_data.mac.octet[5],
            l2_data.port, l2_data.cvid, l2_data.fid, l2_data.efid, l2_data.sa_block, l2_data.da_block, (l2_data.is_static ? "y" : "n"));

        address++;
        total++;

        if(total % 10 == 0) {
            cs_thread_delay(10);
        }          
    }

    if (address)
    {
        diag_printf("+-------------+------------------------------------------------------------------------+\n");
        diag_printf("total fdb entries : %d\n", total);
    }

    return;
}

static void __sw_fdb_cmd_mc_query(int  argc, char **argv)
{
    rtk_api_ret_t rtn  = RT_ERR_FAILED;
    rtk_l2_ucastAddr_t l2_data;
    rtk_uint32 address = 0, total = 0;;

    while (1)
    {
        if ((rtn = rtk_l2_addr_next_get(READMETHOD_NEXT_L2MC, 0, &address, &l2_data)) != RT_ERR_OK)
            break;

        diag_printf("+-------------+------------------------------------------------------------------------+\n");
        diag_printf("| index(%04d) | mac(%02x:%02x:%02x:%02x:%02x:%02x) port(%d) fid(%d) efid(%d) sa-block(%d) is-static(%s) |\n",
            address, 
            l2_data.mac.octet[0], l2_data.mac.octet[1], l2_data.mac.octet[2],
            l2_data.mac.octet[3], l2_data.mac.octet[4], l2_data.mac.octet[5],
            l2_data.port, l2_data.fid, l2_data.efid, l2_data.sa_block, (l2_data.is_static ? "y" : "n"));

        address++;
        total++;
    }

    if (address)
    {
        diag_printf("+-------------+------------------------------------------------------------------------+\n");
        diag_printf("total fdb entries : %d\n", total);
    }

    extern cs_status sdl_mc_l2entry_sw_show(cs_uint8 index, cs_uint8 num);
    sdl_mc_l2entry_sw_show(0, 100);

    return;
}

static void __sw_fdb_cmd_ipmc_query(int  argc, char **argv)
{
    rtk_api_ret_t rtn  = RT_ERR_FAILED;
    ipaddr_t sip =0,dip =0;
    rtk_portmask_t port_msk;
    rtk_uint32 address = 0, total = 0;;
    char s_buf[20];
    char d_buf[20];
    
    memset(&port_msk, 0, sizeof(port_msk));
     
    while (1)
    {
        if ((rtn = rtk_l2_ipMcastAddr_next_get(&address, &sip, &dip, &port_msk)) != RT_ERR_OK)
            break;
        cs_ip2str(s_buf, ntohl(sip));
        cs_ip2str(d_buf, ntohl(dip));
         
        diag_printf("+-------------+------------------------------------------------------------------------+\n");
        diag_printf("| index(%04d) |    SIP %-15s     DIP %-15s     port-mask(%04xh)    |\n",
            address, s_buf, d_buf, port_msk.bits[0]);
        
        address++;
        total++;
    }

    if (address)
    {
        diag_printf("+-------------+------------------------------------------------------------------------+\n");
        diag_printf("total fdb entries : %d\n", total);
    }

    return;
}

static void __sw_fdb_cmd_mcast_del_port(int  argc, char **argv)
{   
    rtk_api_ret_t rtn = RT_ERR_FAILED;
    unsigned int temp_mac[6], i = 0;
    void *ptr = NULL;
    cs_callback_context_t context;
    cs_sdl_mc_l2_entry_t  entry;
    cs_port_id_t port;

    if (argc != 3)
    {
        diag_printf("parameter number error\n");
        return;
    }

    if(aal_cmd_mac_match(argv[0]) != CS_E_OK)
    {
        diag_printf("mac address error\n");
        return;
    }
    
    ptr = argv[0];
    sscanf(ptr, "%02x:%02x:%02x:%02x:%02x:%02x", &temp_mac[0], &temp_mac[1], &temp_mac[2], &temp_mac[3], &temp_mac[4], &temp_mac[5]);

    for (i=0; i<6; i++)
    {
        entry.mac.addr[i] = temp_mac[i];
    }
    
    entry.vlan = iros_strtol(argv[1]);
    port = iros_strtol(argv[2]);

    rtn = epon_request_onu_mc_l2_entry_del_port(context,0,0,port ,&entry);
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");

    return;
}

static void __sw_fdb_cmd_mcast_clr(int  argc, char **argv)
{
    cs_status rtn = CS_E_OK;
    cs_callback_context_t context;
    rtn = epon_request_onu_mc_l2_entry_clr(context,0,0); 
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");
}

static void __sw_fdb_cmd_ucast_clr(int  argc, char **argv)
{
#if 0   
    cs_callback_context_t context;
    cs_sdl_fdb_clr_mode_t clr_mode;
    cs_port_id_t port;
    cs_status rtn = CS_E_OK;
    
    if (argc != 2)
    {
        diag_printf("parameter number error\n");
        return;
    }

    clr_mode = iros_strtol(argv[0]);
    port = iros_strtol(argv[1]);

    rtn = epon_request_onu_uc_l2_entry_clr(context,0,0,clr_mode, port); 
    if (rtn)
        diag_printf("errcode(%d)\n", rtn);
    else
        diag_printf("done\n");
#endif
}

/*******************************************************************************/
static struct {
    char                   *name;
    aal_l2_cmd_func_t  exe_func;
    int                     argc;
    char                   *para;
    char                   *help;
}sw_fdb_cmds[] = {
        { "add-uc-entry",      __sw_fdb_cmd_ucast_add       , 8, "[mac] [fid] [efid] [port] [sa-block][da-block] [auth] [is-static]", "add a unicast fdb entry"},
        { "del-uc-entry",      __sw_fdb_cmd_ucast_del       , 3, "[mac] [fid] [efid]", "del a unicast fdb entry"},
        { "get-uc-entry",      __sw_fdb_cmd_ucast_get       , 3, "[mac] [fid] [efid]", "get a unicast fdb entry"},
        { "clr-uc-entry",      __sw_fdb_cmd_ucast_clr       , 2, "[mode][port]", "clear all ucast fdb entry, static or dynamic"},
        { "get-next-uc-entry", __sw_fdb_cmd_ucast_get_next  , 3, "[read-method] [port] [next]", "next unicast fdb entry"},
        { "add-mc-entry",      __sw_fdb_cmd_mcast_add       , 3, "[mac] [vlan] [port]", "add a mcast fdb entry"},
        { "del-mc-entry",      __sw_fdb_cmd_mcast_del       , 2, "[mac] [vlan]","del a mcast fdb entry"},
        { "del-mc-port",       __sw_fdb_cmd_mcast_del_port  , 3, "[mac] [vlan] [port]","del a port from mcast fdb entry"},
        { "get-mc-entry",      __sw_fdb_cmd_mcast_get       , 1, "[mac]","get a mcast fdb entry"},
        { "clr-mc-entry",      __sw_fdb_cmd_mcast_clr       , 0, "<cr>", "clear all mcast fdb entry"},
        { "get-l2-entry",      __sw_fdb_cmd_l2_entry_get    , 1, "[index : 1 - 2112]",      "get a l2 entry by index"},
        { "flush",             __sw_fdb_cmd_flush           , 3, "[flush-type] [vlan-id] [port]", "flush l2 entry by vlan and port"},
        { "query",             __sw_fdb_cmd_query           , 0, "<cr>", "query all fdb entries"},
        { "mc-query",          __sw_fdb_cmd_mc_query        , 0, "<cr>", "query all mc-fdb entries"},
        { "ipmc-query",        __sw_fdb_cmd_ipmc_query      , 0, "<cr>", "query all IP mc-fdb entries"},
        { "add-ipmc-entry",    __sw_fdb_cmd_ipmcast_add     , 3, "[sip] [dip] [port_msk]", "add a ip mcast fdb entry"},
        { "del-ipmc-entry",    __sw_fdb_cmd_ipmcast_del     , 2, "[sip] [dip]", "del an ip mcast fdb entry"},
        { "set-mc-lookup",     __sw_fdb_cmd_mc_lookup_set   , 1, "[0:mac/1:sip+dip/2:dip]", " Set Lookup type for MC packet"},
        { "get-mc-lookup",     __sw_fdb_cmd_mc_lookup_get   , 0, "<cr>", " Get Lookup type for MC packet"},
        { "set-unk-mc-action", __sw_fdb_cmd_unkmc_action_set, 3, "[port] [type:0 L2/1 IPV4/2 IPV6] [action:0 fwd/1 drop/2 to cpu]", "set unknown multicast action"},
        { "get-unk-mc-action", __sw_fdb_cmd_unkmc_action_get, 2, "[port] [type:0 L2/1 IPV4/2 IPV6]",          "get unknown multicast action"},
        { "add-ipmc-group",    __sw_fdb_cmd_ipmcgrp_add     , 2, "[index] [group ip] ", "add a ip mcast group entry"},
        { "get-ipmc-group",    __sw_fdb_cmd_ipmcgrp_get     , 0, "<cr>", "get all IP mc group entries"},
};

void __sw_fdb_cmd_help(void)
{
    int i;

    for (i = 0; i < sizeof(sw_fdb_cmds) / sizeof(sw_fdb_cmds[0]); i++) {  
        __SW_L2_PRINT_CMD_HELP("fdb", sw_fdb_cmds[i].name, sw_fdb_cmds[i].help);    
    }
}

static aal_l2_cmd_func_t  __sw_fdb_find_cmd
(
    const char *cmd_str,
    int   argc
)
{
    int i = 0;

    if (NULL == cmd_str) {
        return NULL;
    }

    for (i = 0;
        i < (sizeof(sw_fdb_cmds)/ sizeof(sw_fdb_cmds[0]));
        ++i) {
        if (__sw_l2_cmd_cmp(sw_fdb_cmds[i].name, cmd_str)){

            if ((argc - 3) == sw_fdb_cmds[i].argc) {
                return sw_fdb_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", sw_fdb_cmds[i].argc);
                diag_printf("parameters:  %s\n", sw_fdb_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __sw_fdb_cmd_help();

    return (NULL);

}


void sw_fdb_cmd(int argc, char **argv)
{
    aal_l2_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal fdb  ..."*/

        p_cmd_func = __sw_fdb_find_cmd(argv[2], argc);

    }
    else {
        __sw_fdb_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}



/*  end of file */

