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
extern cs_status aal_cmd_mac_match(char *str);

char *le_str[] = {"GE", "PON", "MA"};
  
/****************************************************************************/
#define __AAL_L2_PRINT_CMD_HELP(func,cmd,help)  diag_printf("%s %s %-20s --- %s\n","aal",(func),(cmd),(help))

typedef void (*aal_l2_cmd_func_t)(int, char **);

/****************************************************************************/
/*                     L2(fdb/port/vlan) SDK API  debug commands            */
/****************************************************************************/
static int __aal_l2_cmd_cmp(char *str, const char *mode)
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


static void __aal_fdb_cmd_entry_query(int  argc, char **argv)
{
    cs_uint32 i = 0;
    cs_uint8  zeromac[6] = {0, 0, 0, 0, 0, 0};
    cs_aal_fdb_entry_t  entry;

        
    for (i=0; i<64; i++)
    {
        if (aal_fdb_entry_get(i, &entry) != CS_E_OK)
        {
            diag_printf("failed to get fdb entry\n");
            return;
        }
        else
        {
            if (memcmp(entry.mac.addr, zeromac, 6))
                diag_printf("index(%02d) mac(%02x:%02x:%02x:%02x:%02x:%02x vld(%d) port_id(%d) static(%d) aging(%d)\n", i, entry.mac.addr[0], entry.mac.addr[1],
                    entry.mac.addr[2], entry.mac.addr[3], entry.mac.addr[4], entry.mac.addr[5], entry.vld, entry.port_id, entry.static_flag, entry.aging_status);
        }
    }
        
    return;
}

static void __aal_fdb_cmd_entry_get(int  argc, char **argv)
{
    cs_aal_fdb_entry_t  entry;
    int para1;

    memset(&entry, 0, sizeof( cs_aal_fdb_entry_t));
        
    para1 = iros_strtol(argv[0]);
    if (para1 > 63) {
        diag_printf("para errorl.(0~63)\n");
        return;
    }
    
    if (aal_fdb_entry_get(para1, &entry) != CS_E_OK)
    {
        diag_printf("failed to get fdb entry\n");
        return;
    }

    diag_printf("index          : %d\n",  para1);
    diag_printf("vld            : %d\n",  entry.vld);
    diag_printf("mac            : %02x:%02x:%02x:%02x:%02x:%02x\n",  entry.mac.addr[0], entry.mac.addr[1], entry.mac.addr[2], entry.mac.addr[3], entry.mac.addr[4], entry.mac.addr[5]);
    diag_printf("vid            : %d\n",  entry.vid);
    diag_printf("dot1p          : %d\n",  entry.dot1p);
    diag_printf("mac_vld        : %d\n",  entry.mac_vld);
    diag_printf("vid_vld        : %d\n",  entry.vid_vld);
    diag_printf("dot1p_vld      : %d\n",  entry.dot1p_vld);
    diag_printf("static_flag    : %d\n",  entry.static_flag);
    diag_printf("port_id        : %d\n",  entry.port_id);
    diag_printf("permit_da_ge   : %d\n",  entry.permit_da_ge);
    diag_printf("permit_sa_ge   : %d\n",  entry.permit_sa_ge);
    diag_printf("permit_da_pon  : %d\n",  entry.permit_da_pon);
    diag_printf("permit_sa_pon  : %d\n",  entry.permit_sa_pon);
    diag_printf("permit_da_cpu  : %d\n",  entry.permit_da_cpu);
    diag_printf("permit_sa_cpu  : %d\n",  entry.permit_sa_cpu);
    diag_printf("permit_da_mii0 : %d\n", entry.permit_da_mii0);
    diag_printf("permit_sa_mii0 : %d\n", entry.permit_sa_mii0);
    diag_printf("permit_da_mii1 : %d\n", entry.permit_da_mii1);
    diag_printf("permit_sa_mii1 : %d\n", entry.permit_sa_mii1);
    diag_printf("aging_status   : %d\n", entry.aging_status);
    
    return;
}

static void __aal_fdb_cmd_status_get(int  argc, char **argv)
{
    cs_aal_fdb_status_t  fdb_status;

    memset(&fdb_status, 0, sizeof( cs_aal_fdb_status_t));
        
    if (CS_E_OK != aal_fdb_status_get(&fdb_status)){
        diag_printf("get fdb debug status fail.\n");
        return;
    }

    diag_printf("ge_vln0_lrn_cnt  : %d\n", fdb_status.ge_vln0_lrn_cnt);
    diag_printf("ge_vln1_lrn_cnt  : %d\n", fdb_status.ge_vln1_lrn_cnt);
    diag_printf("ge_vln2_lrn_cnt  : %d\n", fdb_status.ge_vln2_lrn_cnt);
    diag_printf("ge_vln3_lrn_cnt  : %d\n", fdb_status.ge_vln3_lrn_cnt);    
    diag_printf("lrn_cnt_ge   : %d\n", fdb_status.lrn_cnt_ge);
    diag_printf("lrn_cnt_pon  : %d\n", fdb_status.lrn_cnt_pon);
    diag_printf("lrn_cnt_cpu  : %d\n", fdb_status.lrn_cnt_cpu);
    diag_printf("lrn_cnt_mii0 : %d\n", fdb_status.lrn_cnt_mii0);
    diag_printf("lrn_cnt_mii1 : %d\n", fdb_status.lrn_cnt_mii1);
    diag_printf("aging_idx    : %d\n", fdb_status.aging_idx);
    diag_printf("station_move_idx_ge   : %d\n", fdb_status.station_move_idx_ge);
    diag_printf("station_move_idx_pon  : %d\n", fdb_status.station_move_idx_pon);
    diag_printf("station_move_idx_cpu  : %d\n", fdb_status.station_move_idx_cpu);
    diag_printf("station_move_idx_mii0 : %d\n", fdb_status.station_move_idx_mii0);
    diag_printf("station_move_idx_mii1 : %d\n", fdb_status.station_move_idx_mii1);
    
    return;
}

static void __aal_fdb_cmd_cfg_get(int  argc, char **argv)
{
    cs_aal_fdb_cfg_t  cfg;

    memset(&cfg, 0, sizeof( cs_aal_fdb_cfg_t));
        
    if (CS_E_OK != aal_fdb_cfg_get(&cfg)){
        diag_printf("get fdb cfg fail.\n");
        return;
    }

    diag_printf("%-20s        : %s(%d)\n", "learn_mode", ((cfg.learn_mode == 0) ? "h/w learning" : "s/w learning"), cfg.learn_mode);
    diag_printf("%-20s        : %s(%d)\n", "aging_en", ((cfg.aging_en == 1) ? "enabled" : "disabled"), cfg.aging_en);
    diag_printf("%-20s        : %d\n", "aging_cntr", cfg.aging_cntr);
    diag_printf("%-20s        : %d\n", "aging_intvl", cfg.aging_intvl);
    diag_printf("%-20s        : %s(%d)\n", "station_move_enable", ((cfg.station_move_en == 1) ? "enabled" : "disabled"), cfg.station_move_en);
    diag_printf("%-20s        : %s(%d)\n", "station_move_drop", ((cfg.station_move_drop == 1) ? "true" : "false"), cfg.station_move_drop);
    diag_printf("%-20s        : %d\n", "st_loop_thrshld", cfg.st_loop_thrshld);
    diag_printf("%-20s        : %s(%d)\n", "st_loop_detect_drop", ((cfg.st_loop_detect_drop == 1) ? "true" : "false"), cfg.st_loop_detect_drop);
    
    return;
}

static void __aal_fdb_cmd_vlan_lmt_get(int  argc, char **argv)
{
    cs_uint32 i = 0;
    cs_aal_vlan_mac_entry_t  entry;

       
    for (i=0; i<4; i++)
    {
        if (aal_ge_vlan_mac_limit_entry_get(i, &entry) != CS_E_OK)
        {
            diag_printf("failed to get vlan mac limit entry\n");
            return;
        }
        else
        {
            if (entry.vld)
                diag_printf("idx: %d    vlan : %-4d     mac_limit : %d\n", i, entry.mac_limited_vlan, entry.mac_limit);
        }
    }
        
    return;
}

static void __aal_fdb_cmd_port_get(int  argc, char **argv)
{
    cs_aal_port_id_t para1;
    cs_aal_fdb_port_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);

    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }
    memset(&cfg, 0, sizeof( cs_aal_fdb_port_cfg_t));
        
    if (CS_E_OK != aal_fdb_port_cfg_get(para1, &cfg)){
        diag_printf("get fdb port cfg fail.\n");
        return;
    }

    diag_printf("%-20s          : %d\n", "learn_dis",cfg.learn_dis);
    diag_printf("%-20s          : %d\n", "learn_lmt_ovrwrt",cfg.learn_lmt_ovrwrt);
    diag_printf("%-20s          : %d\n", "learn_dpid_cpu_dis",cfg.learn_dpid_cpu_dis);
    diag_printf("%-20s          : %d\n", "learn_dpid_drop_dis",cfg.learn_dpid_drop_dis);
    diag_printf("%-20s          : %d\n", "pkt_fwd_dis",cfg.pkt_fwd_dis);
    diag_printf("%-20s          : %d\n", "learn_vlan_sel",cfg.learn_vlan_sel);
    diag_printf("%-20s          : %d\n", "lookup_vlan_sel",cfg.lookup_vlan_sel);
    diag_printf("%-20s          : %d\n", "sa_drop_en",cfg.sa_drop_en);
    diag_printf("%-20s          : %d\n", "learn_mac_vld",cfg.learn_mac_vld);
    diag_printf("%-20s          : %d\n", "learn_vlan_vld",cfg.learn_vlan_vld);
    diag_printf("%-20s          : %d\n", "learn_dot1p_vld",cfg.learn_dot1p_vld);
    diag_printf("%-20s          : %d\n", "lookup_mac_vld",cfg.lookup_mac_vld);
    diag_printf("%-20s          : %d\n", "lookup_vlan_vld",cfg.lookup_vlan_vld);
    diag_printf("%-20s          : %d\n", "lookup_dot1p_vld",cfg.lookup_dot1p_vld);
    diag_printf("%-20s          : %d\n", "mac_match_ctrl",cfg.mac_match_ctrl);
    diag_printf("%-20s          : %d\n", "sa_max_limit",cfg.sa_max_limit);
    diag_printf("%-20s          : %d\n", "sa_lmt_drop_en",cfg.sa_lmt_drop_en);
    
    return;
}

static void __aal_fdb_cmd_lrn_mode_set(int  argc, char **argv)
{
    int para1;
    cs_aal_fdb_msk_t  msk;
    cs_aal_fdb_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    
    if (para1 > 1) {
        diag_printf("para0 errorl.\n");
        return;
    }

    msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_fdb_cfg_t));
    msk.s.lrn_mode = 1;
    cfg.learn_mode = para1;

    diag_printf("%s\n", (aal_fdb_cfg_set(msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_fdb_cmd_age_ctrl_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    int para4;
    int para5;
    int para6;
    cs_aal_fdb_msk_t msk;
    cs_aal_fdb_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    para4 = iros_strtol(argv[3]);
    para5 = iros_strtol(argv[4]);
    para6 = iros_strtol(argv[5]);
        
    if (para1 > 1) {
        diag_printf("para0 errorl.\n");
        return;
    }
    if (para3 > 0xfff) {
        diag_printf("para2 errorl.\n");
        return;
    }

    msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_fdb_cfg_t));
    if(para2)
    {
        msk.s.aging_en = 1;
        cfg.aging_en = para1;
    }
    if(para4)
    {
        msk.s.aging_cntr = 1;
        cfg.aging_cntr = para3;
    }
    if(para6)
    {
        msk.s.aging_intvl = 1;
        cfg.aging_intvl = para5;
    }


    diag_printf("%s\n", (aal_fdb_cfg_set(msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_fdb_cmd_staton_ctrl_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    int para4;
    cs_aal_fdb_msk_t msk;
    cs_aal_fdb_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    para4 = iros_strtol(argv[3]);
        
    if (para1 > 1) {
        diag_printf("para0 errorl.\n");
        return;
    }
    if (para3 > 1) {
        diag_printf("para2 errorl.\n");
        return;
    }

    msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_fdb_cfg_t));
    if(para2)
    {
        msk.s.station_move_enable = 1;
        cfg.station_move_en = para1;
    }
    if(para4)
    {
        msk.s.station_move_drop = 1;
        cfg.station_move_drop = para3;
    }

    diag_printf("%s\n", (aal_fdb_cfg_set(msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_fdb_cmd_staton_loop_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    int para4;
    cs_aal_fdb_msk_t  msk;
    cs_aal_fdb_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    para4 = iros_strtol(argv[3]);
        
    if (para1 > 0xfff) {
        diag_printf("para0 errorl.\n");
        return;
    }
    if (para3 > 1) {
        diag_printf("para2 errorl.\n");
        return;
    }

    msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_fdb_cfg_t));
    if(para2)
    {
        msk.s.st_loop_thrshld = 1;
        cfg.st_loop_thrshld = para1;
    }
    if(para4)
    {
        msk.s.st_loop_detect_drop = 1;
        cfg.st_loop_detect_drop = para3;
    }

    diag_printf("%s\n", (aal_fdb_cfg_set(msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_fdb_cmd_entry_set(int  argc, char **argv)
{  
    cs_aal_fdb_entry_t  entry;
    cs_uint8 fdb_idx = 0;
    void *ptr = NULL;
    cs_uint32 mac[6], i = 0;
    

    fdb_idx   = iros_strtol(argv[0]);
    entry.vld = iros_strtol(argv[1]);

    if(aal_cmd_mac_match(argv[2]) != CS_E_OK)
    {
        diag_printf("invalid mac format\n");
        return;
    }
    ptr = argv[2];
    sscanf(ptr, "%02x:%02x:%02x:%02x:%02x:%02x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);

    for (i=0; i<6; i++)
        entry.mac.addr[i] = mac[i];

    entry.vid            = iros_strtol(argv[3]);
    entry.dot1p          = iros_strtol(argv[4]);
    entry.mac_vld        = iros_strtol(argv[5]);
    entry.vid_vld        = iros_strtol(argv[6]);
    entry.dot1p_vld      = iros_strtol(argv[7]);
    entry.static_flag    = iros_strtol(argv[8]);
    entry.port_id        = iros_strtol(argv[9]);
    entry.permit_da_ge   = iros_strtol(argv[10]);
    entry.permit_sa_ge   = iros_strtol(argv[11]);
    entry.permit_da_pon  = iros_strtol(argv[12]);
    entry.permit_sa_pon  = iros_strtol(argv[13]);
    entry.permit_da_cpu  = iros_strtol(argv[14]);
    entry.permit_sa_cpu  = iros_strtol(argv[15]);
    entry.permit_da_mii0 = iros_strtol(argv[16]);
    entry.permit_sa_mii0 = iros_strtol(argv[17]);
    entry.permit_da_mii1 = iros_strtol(argv[18]);
    entry.permit_sa_mii1 = iros_strtol(argv[19]);
    entry.aging_status   = iros_strtol(argv[20]);
 
    diag_printf("%s\n", (aal_fdb_entry_set( fdb_idx, &entry) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_fdb_cmd_vlan_lmt_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    int para4;
    cs_aal_vlan_mac_entry_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    para4 = iros_strtol(argv[3]);
        
    if (para1 > 3) {
        diag_printf("para0 errorl.\n");
        return;
    }
    if (para2 > 1) {
        diag_printf("para1 errorl.\n");
        return;
    }
    if (para3 > 64) {
        diag_printf("para2 errorl.\n");
        return;
    }
    memset(&cfg, 0, sizeof( cs_aal_vlan_mac_entry_t));
    cfg.vld = para2;
    cfg.mac_limit = para3;
    cfg.mac_limited_vlan = para4;
   
    diag_printf("%s\n", (aal_ge_vlan_mac_limit_entry_set(para1, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_fdb_cmd_learn_ctrl0_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    int para4;
    int para5;
    cs_aal_fdb_port_msk_t  cfg_msk;
    cs_aal_fdb_port_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    para4 = iros_strtol(argv[3]);
    para5 = iros_strtol(argv[4]);

    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }
    if (para2 > 1) {
        diag_printf("para1 errorl.\n");
        return;
    }
    if (para3 > 1) {
        diag_printf("para2 errorl.\n");
        return;
    }
    if (para4 > 1) {
        diag_printf("para3 errorl.\n");
        return;
    }
    if (para5 > 1) {
        diag_printf("para4 errorl.\n");
        return;
    }
    cfg_msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_fdb_port_cfg_t));
    cfg_msk.s.learn_dis = 1;
    cfg_msk.s.learn_lmt_ovrwrt = 1;
    cfg_msk.s.learn_dpid_cpu_dis = 1;
    cfg_msk.s.learn_dpid_drop_dis = 1;
    
    cfg.learn_dis = para2;
    cfg.learn_lmt_ovrwrt = para5;
    cfg.learn_dpid_cpu_dis = para3;
    cfg.learn_dpid_drop_dis = para4;
   
    diag_printf("%s\n", (aal_fdb_port_cfg_set( para1, cfg_msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_fdb_cmd_learn_ctrl1_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    int para4;
    int para5;
    cs_aal_fdb_port_msk_t cfg_msk;
    cs_aal_fdb_port_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    para4 = iros_strtol(argv[3]);
    para5 = iros_strtol(argv[4]);

    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }
    if (para2 > 3) {
        diag_printf("learn_vlan_sel errorl.\n");
        return;
    }
    if (para3 > 1) {
        diag_printf("para2 errorl.\n");
        return;
    }
    if (para4 > 1) {
        diag_printf("para3 errorl.\n");
        return;
    }
    if (para5 > 1) {
        diag_printf("para4 errorl.\n");
        return;
    }
    cfg_msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_fdb_port_cfg_t));
    cfg_msk.s.learn_vlan_sel = 1;
    cfg_msk.s.learn_mac_vld  = 1;
    cfg_msk.s.learn_vlan_vld = 1;
    cfg_msk.s.learn_dot1p_vld = 1;
    
    cfg.learn_vlan_sel  = para2;
    cfg.learn_mac_vld   = para3;
    cfg.learn_vlan_vld  = para4;
    cfg.learn_dot1p_vld = para5;
   
    diag_printf("%s\n", (aal_fdb_port_cfg_set( para1, cfg_msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_fdb_cmd_lookup_ctrl_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    int para4;
    int para5;
    cs_aal_fdb_port_msk_t cfg_msk;
    cs_aal_fdb_port_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    para4 = iros_strtol(argv[3]);
    para5 = iros_strtol(argv[4]);

    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }
    if (para2 > 1) {
        diag_printf("para1 errorl.\n");
        return;
    }
    if (para3 > 1) {
        diag_printf("para2 errorl.\n");
        return;
    }
    if (para4 > 1) {
        diag_printf("para3 errorl.\n");
        return;
    }
    if (para5 > 1) {
        diag_printf("para4 errorl.\n");
        return;
    }
    cfg_msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_fdb_port_cfg_t));
    cfg_msk.s.lookup_vlan_sel = 1;
    cfg_msk.s.lookup_mac_vld  = 1;
    cfg_msk.s.lookup_vlan_vld = 1;
    cfg_msk.s.lookup_dot1p_vld = 1;
    
    cfg.lookup_vlan_sel = para2;
    cfg.lookup_mac_vld = para3;
    cfg.lookup_vlan_vld = para4;
    cfg.lookup_dot1p_vld = para5;
   
    diag_printf("%s\n", (aal_fdb_port_cfg_set( para1, cfg_msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_fdb_cmd_fwd_ctrl_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    int para4;
    int para5;
    int para6;
    int para7;
    cs_aal_fdb_port_msk_t  cfg_msk;
    cs_aal_fdb_port_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    para4 = iros_strtol(argv[3]);
    para5 = iros_strtol(argv[4]);
    para6 = iros_strtol(argv[5]);
    para7 = iros_strtol(argv[6]);

    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }  
    if (para3 > 1) {
        diag_printf("para2 errorl.\n");
        return;
    }
    if (para5 > 1) {
        diag_printf("para4 errorl.\n");
        return;
    } 
    if (para7 > 1) {
        diag_printf("para6 errorl.\n");
        return;
    }
    cfg_msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_fdb_port_cfg_t));

    if(para3)
    {   
          if (para2 > 1) {
            diag_printf("para1 errorl.\n");
            return;
        }
        cfg_msk.s.pkt_fwd_dis = 1;
        cfg.pkt_fwd_dis = para2;
    } 
    if(para5)
    {   
           if (para4 > 1) {
            diag_printf("para3 errorl.\n");
            return;
        }
        cfg_msk.s.sa_drop_en = 1;
        cfg.sa_drop_en = para4;
    }
    if(para7)
    {   
            if (para6 > 3) {
            diag_printf("para5 errorl.\n");
            return;
        }
        cfg_msk.s.mac_match_ctrl = 1;
        cfg.mac_match_ctrl = para6;
    }
   
    diag_printf("%s\n", (aal_fdb_port_cfg_set( para1, cfg_msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_fdb_cmd_port_lmt_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    int para4;
    int para5;
    cs_aal_fdb_port_msk_t  cfg_msk;
    cs_aal_fdb_port_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    para4 = iros_strtol(argv[3]);
    para5 = iros_strtol(argv[4]);

    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }
    if (para3 > 1) {
        diag_printf("para2 errorl.\n");
        return;
    }
    if (para5 > 1) {
        diag_printf("para4 errorl.\n");
        return;
    }
    cfg_msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_fdb_port_cfg_t));
    if(para3)
    {   
             if (para2 > 64) {
            diag_printf("para1 errorl.\n");
            return;
        }
        cfg_msk.s.sa_max_limit = 1;
        cfg.sa_max_limit = para2;
    } 
    if(para5)
    {   
                if (para4 > 1) {
            diag_printf("para3 errorl.\n");
            return;
        }
        cfg_msk.s.sa_lmt_drop_en = 1;
        cfg.sa_lmt_drop_en = para4;
    }
   
    diag_printf("%s\n", (aal_fdb_port_cfg_set( para1, cfg_msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_port_cmd_def_get(int  argc, char **argv)
{
    int para1;
    cs_aal_port_dft_cfg_t  dft_cfg;

    para1 = iros_strtol(argv[0]);
    
    if(para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }
    memset(&dft_cfg, 0, sizeof( cs_aal_port_dft_cfg_t));
    if (aal_port_dft_cfg_get(para1, &dft_cfg) != CS_E_OK)
    {
        diag_printf("failed to get port default config \n");
        return;
    }
    diag_printf("vlanid  : %d\n", dft_cfg.vlanid);
    diag_printf("cos     : %d\n", dft_cfg.cos);
    diag_printf("dot1p   : %d\n", dft_cfg.dot1p);
    diag_printf("dscp    : %d\n", dft_cfg.dscp);
    diag_printf("tpid    : 0x%x\n", dft_cfg.tpid);

    return;
}

static void __aal_port_cmd_def_set(int  argc, char **argv)
{
    int para1;
    cs_aal_port_dft_msk_t  msk;
    cs_aal_port_dft_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    
    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }

    msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_port_dft_cfg_t));
    msk.s.vlanid = 1;
    msk.s.cos    = 1;
    msk.s.dot1p  = 1;
    msk.s.dscp   = 1;
    msk.s.tpid   = 1;
    cfg.vlanid   = iros_strtol(argv[1]);
    cfg.cos      = iros_strtol(argv[2]);
    cfg.dot1p    = iros_strtol(argv[3]);
    cfg.dscp     = iros_strtol(argv[4]);
    cfg.tpid     = iros_strtol(argv[5]);

    diag_printf("%s\n", (aal_port_dft_cfg_set(para1, msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_port_cmd_cfg_get(int  argc, char **argv)
{
    int para1;
    cs_aal_port_cfg_t  port_cfg;
    cs_aal_dpid_t      dpid;

    para1 = iros_strtol(argv[0]);
    
    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }
    memset(&port_cfg, 0, sizeof( cs_aal_port_cfg_t));
    if (aal_port_cfg_get(para1, &port_cfg) != CS_E_OK)
    {
        diag_printf("failed to get port config \n");
        return;
    }
    diag_printf("%-20s          : %d\n", "cpu_header_pkt_type",port_cfg.cpu_header_pkt_type);
    diag_printf("%-20s          : %d\n", "oam_lpbk_en",port_cfg.oam_lpbk_en);
    diag_printf("%-20s          : %d\n", "dbg_da_sa_swap_en",port_cfg.dbg_da_sa_swap_en);
    diag_printf("%-20s          : 0x%x\n", "ipv4_dscp_mask",port_cfg.ipv4_dscp_mask);
    diag_printf("%-20s          : 0x%x\n", "ipv6_tc_mask",port_cfg.ipv6_tc_mask);
    diag_printf("%-20s          : %d\n", "rx_def_vid_swap",port_cfg.rx_def_vid_swap);
    diag_printf("%-20s          : %d\n", "tx_def_vid_pop",port_cfg.tx_def_vid_pop);
    SPEC_PORT_TO_DPID(port_cfg.uuc_dpid.dst_op, port_cfg.uuc_dpid.dpid, dpid);
    diag_printf("%-20s          : %d\n", "uuc_dpid",dpid);
    diag_printf("%-20s          : %d\n", "uuc_fwd_cos",port_cfg.uuc_fwd_cos);
    SPEC_PORT_TO_DPID(port_cfg.bc_dpid.dst_op, port_cfg.bc_dpid.dpid, dpid);
    diag_printf("%-20s          : %d\n", "bc_dpid",dpid);   
    diag_printf("%-20s          : %d\n", "bc_fwd_cos",port_cfg.bc_fwd_cos);
    SPEC_PORT_TO_DPID(port_cfg.mc_dpid.dst_op, port_cfg.mc_dpid.dpid, dpid);
    diag_printf("%-20s          : %d\n", "mc_dpid",dpid);
    diag_printf("%-20s          : %d\n", "mc_fwd_cos",port_cfg.mc_fwd_cos);
 
    return;
}

static void __aal_port_cmd_msk_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    cs_aal_port_msk_t  msk;
    cs_aal_port_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    
    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }
    if (para2 > 63) {
        diag_printf("para1 errorl.\n");
        return;
    }
    if (para3 > 63) {
        diag_printf("para2 errorl.\n");
        return;
    }
    msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_port_cfg_t));
    msk.s.ipv4_dscp_mask = 1;
    msk.s.ipv6_tc_mask = 1;
    cfg.ipv4_dscp_mask = para2;
    cfg.ipv6_tc_mask = para3;

    diag_printf("%s\n", (aal_port_cfg_set(para1, msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_port_cmd_vlnop_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    cs_aal_port_msk_t  msk;
    cs_aal_port_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    
    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }
    if (para2 > 1) {
        diag_printf("para1 errorl.\n");
        return;
    }
    if (para3 > 1) {
        diag_printf("para2 errorl.\n");
        return;
    }
    msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_port_cfg_t));
    msk.s.tx_def_vid_pop  = 1;
    msk.s.rx_def_vid_swap = 1;
    cfg.tx_def_vid_pop  = para2;
    cfg.rx_def_vid_swap = para3;

    diag_printf("%s\n", (aal_port_cfg_set(para1, msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_port_cmd_uuc_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    cs_aal_port_msk_t  msk;
    cs_aal_port_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    
    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }
    if (para2>AAL_DPID_FE) {
        diag_printf("DPID errorl.\n");
        return;
    }
    if (para3 > 7) {
        diag_printf("para2 errorl.\n");
        return;
    }
    msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_port_cfg_t));
    msk.s.uuc_dpid = 1;
    msk.s.uuc_fwd_cos = 1;
    SPEC_DPID_TO_PORT(para2, cfg.uuc_dpid.dst_op, cfg.uuc_dpid.dpid);
    cfg.uuc_fwd_cos = para3;

    diag_printf("%s\n", (aal_port_cfg_set(para1, msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_port_cmd_bc_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    cs_aal_port_msk_t  msk;
    cs_aal_port_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    
    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }
    if (para2>AAL_DPID_FE) {
        diag_printf("DPID errorl.\n");
        return;
    }
    if (para3 > 7) {
        diag_printf("para2 errorl.\n");
        return;
    }
    msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_port_cfg_t));
    msk.s.bc_dpid    = 1;
    msk.s.bc_fwd_cos = 1;
    SPEC_DPID_TO_PORT(para2, cfg.bc_dpid.dst_op, cfg.bc_dpid.dpid);
    cfg.bc_fwd_cos = para3;

    diag_printf("%s\n", (aal_port_cfg_set(para1, msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_port_cmd_mc_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    cs_aal_port_msk_t  msk;
    cs_aal_port_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    
    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }
    if (para2>AAL_DPID_FE) {
        diag_printf("DPID errorl.\n");
        return;
    }
    if (para3 > 7) {
        diag_printf("para2 errorl.\n");
        return;
    }
    msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_port_cfg_t));
    msk.s.mc_dpid    = 1;
    msk.s.mc_fwd_cos = 1;
    SPEC_DPID_TO_PORT(para2, cfg.mc_dpid.dst_op, cfg.mc_dpid.dpid);
    cfg.mc_fwd_cos = para3;

    diag_printf("%s\n", (aal_port_cfg_set(para1, msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_port_cmd_other_set(int  argc, char **argv)
{
    int para1;
    cs_aal_port_msk_t  msk;
    cs_aal_port_cfg_t  cfg;

    para1 = iros_strtol(argv[0]);
    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port errorl.\n");
        return;
    }
    msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_port_cfg_t));
    msk.s.cpu_header_pkt_type = 1;
    msk.s.oam_lpbk_en         = 1;
    msk.s.dbg_da_sa_swap_en   = 1;
    cfg.cpu_header_pkt_type   = iros_strtol(argv[1]);
    cfg.oam_lpbk_en           = iros_strtol(argv[2]);
    cfg.dbg_da_sa_swap_en     = iros_strtol(argv[3]);

    diag_printf("%s\n", (aal_port_cfg_set(para1, msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_vlan_cmd_entry_get(int  argc, char **argv)
{
    cs_aal_vlan_entry_t  vlan_cfg;
    cs_aal_port_id_t      port_id = 0;
    cs_uint16               index = 0;

    port_id = iros_strtol(argv[0]);
    index   = iros_strtol(argv[1]);
    
    if((port_id>AAL_PORT_ID_MII1)||(index>33))
    {
        diag_printf("%% invalid input index.\n");
        return;
    }
 
    memset( &vlan_cfg, 0, sizeof( cs_aal_vlan_entry_t));
    if (aal_vlan_entry_get(port_id, index, &vlan_cfg) != CS_E_OK)
    {
        diag_printf("failed to get vlan entry config \n");
        return;
    }

    diag_printf("\n Port id:%d,   vlan entry index: %d\n", port_id, index);
    diag_printf("\t       %-20s   %d\n\r","vid_hi", vlan_cfg.vid_hi);
    diag_printf("\t       %-20s   %d\n\r","vid_lo", vlan_cfg.vid_lo);
    diag_printf("\t       %-20s   %d\n\r","permit_ge", vlan_cfg.permit_ge);
    diag_printf("\t       %-20s   %d\n\r","permit_pon", vlan_cfg.permit_pon);
    diag_printf("\t       %-20s   %d\n\r","permit_mii0", vlan_cfg.permit_mii0);
    diag_printf("\t       %-20s   %d\n\r","permit_mii1", vlan_cfg.permit_mii1);
    diag_printf("\t       %-20s   %d\n\r","ingress_filter", vlan_cfg.ingress_filter);
    diag_printf("\t       %-20s   %d\n\r","egress_filter", vlan_cfg.egress_filter);
    diag_printf("\t       %-20s   %d\n\r","mapping_flag", vlan_cfg.mapping_flag);
                
    return;
}

static void __aal_vlan_cmd_entry_set(int  argc, char **argv)
{
    cs_aal_vlan_entry_t  vlan_cfg;
    cs_aal_port_id_t      port_id = 0;
    cs_uint16               index = 0;

    port_id = iros_strtol(argv[0]);
    index   = iros_strtol(argv[1]);
    
   if((port_id>AAL_PORT_ID_MII1)||(index>33))
    {
        diag_printf("%% invalid input index.\n");
        return;
    }
    vlan_cfg.vid_hi         = iros_strtol(argv[2]);
    vlan_cfg.vid_lo         = iros_strtol(argv[3]);
    vlan_cfg.permit_ge      = iros_strtol(argv[4]);
    vlan_cfg.permit_pon     = iros_strtol(argv[5]);
    vlan_cfg.permit_mii0    = iros_strtol(argv[6]);
    vlan_cfg.permit_mii1    = iros_strtol(argv[7]);
    vlan_cfg.ingress_filter = iros_strtol(argv[8]);
    vlan_cfg.egress_filter  = iros_strtol(argv[9]);
    vlan_cfg.mapping_flag   = iros_strtol(argv[10]);
    if(vlan_cfg.vid_lo > vlan_cfg.vid_hi)
    {
        diag_printf("%% invalid input vlan id.\n");
        return;
    }

    diag_printf("%s\n", (aal_vlan_entry_set(port_id, index, &vlan_cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_vlan_cmd_cfg_get(int  argc, char **argv)
{
    cs_aal_vlan_cfg_t  vlan_cfg;
    cs_aal_port_id_t    port_id = 0;

    port_id = iros_strtol(argv[0]);    
    if(port_id>AAL_PORT_ID_MII1)
    {
        diag_printf("%% invalid input instance.\n");
        return;
    }
 
    memset( &vlan_cfg, 0, sizeof( cs_aal_vlan_cfg_t));
    if (aal_vlan_cfg_get(port_id, &vlan_cfg) != CS_E_OK)
    {
        diag_printf("failed to get vlan control config \n");
        return;
    }

    diag_printf("\n Port id:%d      \n", port_id);
    diag_printf("\t       %-20s   %d\n\r","mapping_en", vlan_cfg.ingress_mapping_flag);
    diag_printf("\t       %-20s   %d\n\r","top_vlan_l2_en", vlan_cfg.top_vlan_l2_en);
    diag_printf("\t       %-20s   %d\n\r","parse_inner_8100", vlan_cfg.parse_inner_8100);
    diag_printf("\t       %-20s   %d\n\r","parse_outer_88a8", vlan_cfg.parse_outer_88a8);
    diag_printf("\t       %-20s   %d\n\r","parse_inner_tpid", vlan_cfg.parse_inner_tpid);
    diag_printf("\t       %-20s   %d\n\r","parse_outer_tpid1", vlan_cfg.parse_outer_tpid1);
    diag_printf("\t       %-20s   %d\n\r","parse_outer_tpid2", vlan_cfg.parse_outer_tpid2);
    diag_printf("\t       %-20s   %d\n\r","deny_inner_8100", vlan_cfg.deny_inner_8100);
    diag_printf("\t       %-20s   %d\n\r","deny_outer_88a8", vlan_cfg.deny_outer_88a8);
    diag_printf("\t       %-20s   %d\n\r","deny_inner_tpid", vlan_cfg.deny_inner_tpid);
    diag_printf("\t       %-20s   %d\n\r","deny_outer_tpid1", vlan_cfg.deny_outer_tpid1);
    diag_printf("\t       %-20s   %d\n\r","deny_outer_tpid2", vlan_cfg.deny_outer_tpid2);
    diag_printf("\t       %-20s   0x%x \n\r","ingress_inner_tpid", vlan_cfg.ingress_inner_tpid);
    diag_printf("\t       %-20s   0x%x \n\r","ingress_outer_tpid1", vlan_cfg.ingress_outer_tpid1);
    diag_printf("\t       %-20s   0x%x \n\r","ingress_outer_tpid2", vlan_cfg.ingress_outer_tpid2);
                
    return;
}

static void __aal_vlan_cmd_map_set(int  argc, char **argv)
{
    cs_aal_vlan_cfg_msk_t cfg_msk;
    cs_aal_vlan_cfg_t     cfg;
    cs_aal_port_id_t      port_id = 0;
    int para2;

    port_id = iros_strtol(argv[0]);
    para2   = iros_strtol(argv[1]);
    
   if(port_id>AAL_PORT_ID_MII1)
    {
        diag_printf("%% invalid input instance.\n");
        return;
    }
    if (para2 > 1) {
        diag_printf("para1 error.\n");
        return;
    }
    cfg_msk.u32 = 0;
    memset(&cfg, 0, sizeof( cs_aal_vlan_cfg_t));
    cfg_msk.s.ingress_mapping_flag = 1;   
    cfg.ingress_mapping_flag = para2;
   
    diag_printf("%s\n", (aal_vlan_cfg_set(port_id, cfg_msk, &cfg) == CS_E_OK) ? "done" : "failed");

    return;
}

static void __aal_sp_cmd_hello_get(int  argc, char **argv)
{
    cs_uint32 i = 0;
    cs_aal_pkt_spec_udf_t   cfg;
       
    for (i=0; i<3; i++)
    {    
        memset(&cfg, 0, sizeof(cs_aal_pkt_spec_udf_t));
        if (aal_pkt_special_udf_get(i, &cfg) != CS_E_OK)
        {
            diag_printf("failed to get special user define!\n");
            return;
        }
        else
        {
            diag_printf("\t%10s : %02x-%02x-%02x-%02x-%02x-%02x\n",
                le_str[i], cfg.hello_da.addr[0], cfg.hello_da.addr[1], 
                cfg.hello_da.addr[2], cfg.hello_da.addr[3], cfg.hello_da.addr[4], cfg.hello_da.addr[5]);
        }
    }
        
    return;
}

static void __aal_sp_cmd_hello_set(int  argc, char **argv)
{
    int para1;
    cs_uint32 tmp[6] = {0};
    cs_aal_pkt_spec_udf_msk_t msk;
    cs_aal_pkt_spec_udf_t     cfg;
     
    para1 = iros_strtol(argv[0]);

    if (para1>2)
    {
        diag_printf("port error.\n");
        return;
    }

    if(aal_cmd_mac_match (argv[1]) != CS_E_OK)
    {
        diag_printf("mac format must be: AA:BB:CC:DD:EE:FF\n");
        return;
    }

    sscanf(argv[1], "%02x:%02x:%02x:%02x:%02x:%02x",
            &tmp[0], &tmp[1], &tmp[2],
            &tmp[3], &tmp[4], &tmp[5]);
    memset(&cfg, 0, sizeof(cs_aal_pkt_spec_udf_t));
    msk.u32 = 0;
    msk.s.hello_da = 1;  
    cfg.hello_da.addr[0] = tmp[0];
    cfg.hello_da.addr[1] = tmp[1];
    cfg.hello_da.addr[2] = tmp[2];
    cfg.hello_da.addr[3] = tmp[3];
    cfg.hello_da.addr[4] = tmp[4];
    cfg.hello_da.addr[5] = tmp[5];

    diag_printf("%s\n", (aal_pkt_special_udf_set(para1, msk, &cfg) == CS_E_OK) ? "done" : "failed");
         
    return;
}

static void __aal_sp_cmd_udf_get(int  argc, char **argv)
{
    
    int para1;
    cs_aal_pkt_spec_udf_t   cfg;
    const char *le_str[] = {"GE", "PON", "MA"};

    para1 = iros_strtol(argv[0]);    
    if (para1>2)
    {
        diag_printf("port error.\n");
        return;
    }
    
    memset(&cfg, 0, sizeof(cs_aal_pkt_spec_udf_t));
    if (aal_pkt_special_udf_get(para1, &cfg) != CS_E_OK)
    {
        diag_printf("failed to get special user define!\n");
        return;
    }
    else
    {     
        diag_printf("\n Port id:  %10s  \n", le_str[para1]);
        diag_printf("\t %s     \n\r","UDF 0");
        diag_printf("\t       %-10s   : 0x%x\n\r","ethtype", cfg.udf0_etype);
        diag_printf("\t       %-10s   : %d\n\r","mac_en", cfg.udf0_chk_da);
        diag_printf("\t       %-10s   : %02x:%02x:%02x:%02x:%02x:%02x\n\r","mac", 
                          cfg.udf0_da.addr[0], cfg.udf0_da.addr[1], cfg.udf0_da.addr[2],
                          cfg.udf0_da.addr[3], cfg.udf0_da.addr[4], cfg.udf0_da.addr[5]);
        diag_printf("\t %s     \n\r","UDF 1");
        diag_printf("\t       %-10s   : 0x%x\n\r","ethtype", cfg.udf1_etype);
        diag_printf("\t       %-10s   : %d\n\r","mac_en", cfg.udf1_chk_da);
        diag_printf("\t       %-10s   : %02x:%02x:%02x:%02x:%02x:%02x\n\r","mac", 
                          cfg.udf1_da.addr[0], cfg.udf1_da.addr[1], cfg.udf1_da.addr[2],
                          cfg.udf1_da.addr[3], cfg.udf1_da.addr[4], cfg.udf1_da.addr[5]);
        diag_printf("\t %s     \n\r","UDF 2");
        diag_printf("\t       %-10s   : 0x%x\n\r","ethtype", cfg.udf2_etype);
        diag_printf("\t %s     \n\r","UDF 3");
        diag_printf("\t       %-10s   : 0x%x\n\r","ethtype", cfg.udf3_etype);
        diag_printf("\t %s     \n\r","UDF 4");
        diag_printf("\t       %-10s   : 0x%x\n\r","ethtype", cfg.udf4_etype);
       
    }
        
    return;
}

static void __aal_sp_cmd_udf_set(int  argc, char **argv)
{
    int para1;
    int para2;
    int para3;
    int para4;
    cs_uint32 tmp[6] = {0};
    cs_aal_pkt_spec_udf_msk_t msk;
    cs_aal_pkt_spec_udf_t     cfg;
 
    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    para4 = iros_strtol(argv[3]);
 
    if (para1>2)
    {
        diag_printf("port error.\n");
        return;
    }
    if (para2>4)
    {
        diag_printf("udf index error.\n");
        return;
    }
    msk.u32 = 0;
    memset(&cfg, 0, sizeof(cs_aal_pkt_spec_udf_t));
    switch (para2) 
    {
        case 0:
        {
            if(para4)
            {
                if(aal_cmd_mac_match (argv[4]) != CS_E_OK)
                {
                    diag_printf("mac format must be: AA:BB:CC:DD:EE:FF\n");
                    return;
                }
                sscanf(argv[4], "%02x:%02x:%02x:%02x:%02x:%02x",
                            &tmp[0], &tmp[1], &tmp[2],
                            &tmp[3], &tmp[4], &tmp[5]);
                cfg.udf0_da.addr[0] = tmp[0];
                cfg.udf0_da.addr[1] = tmp[1];
                cfg.udf0_da.addr[2] = tmp[2];
                cfg.udf0_da.addr[3] = tmp[3];
                cfg.udf0_da.addr[4] = tmp[4];
                cfg.udf0_da.addr[5] = tmp[5];
            
                cfg.udf0_chk_da = 1; 
            }
            msk.s.udf0_da   = 1;
            msk.s.udf0_chk_da = 1;
            msk.s.udf0_etype  = 1;
            cfg.udf0_etype    = para3;   
            break;
        }
        case 1:
        {
            if(para4)
            {
                if(aal_cmd_mac_match (argv[4]) != CS_E_OK)
                {
                    diag_printf("mac format must be: AA:BB:CC:DD:EE:FF\n");
                    return;
                }
                sscanf(argv[4], "%02x:%02x:%02x:%02x:%02x:%02x",
                            &tmp[0], &tmp[1], &tmp[2],
                            &tmp[3], &tmp[4], &tmp[5]);
                cfg.udf1_da.addr[0] = tmp[0];
                cfg.udf1_da.addr[1] = tmp[1];
                cfg.udf1_da.addr[2] = tmp[2];
                cfg.udf1_da.addr[3] = tmp[3];
                cfg.udf1_da.addr[4] = tmp[4];
                cfg.udf1_da.addr[5] = tmp[5];
             
                cfg.udf1_chk_da = 1; 
            }
            msk.s.udf1_da   = 1;
            msk.s.udf1_chk_da = 1;
            msk.s.udf1_etype  = 1;
            cfg.udf1_etype    = para3;   
            break;
        }
        case 2:
        {  
            msk.s.udf2_etype = 1;
            cfg.udf2_etype   = para3;   
            break;
        }
        case 3:
        {  
            msk.s.udf3_etype = 1;
            cfg.udf3_etype   = para3;   
            break;
        }
        case 4:
        {  
            msk.s.udf4_etype = 1;
            cfg.udf4_etype   = para3;   
            break;
        }
         default:
                return;
    }
        
    diag_printf("%s\n", (aal_pkt_special_udf_set(para1, msk, &cfg) == CS_E_OK) ? "done" : "failed");    
             
    return;
}

static void __aal_sp_cmd_cfg_get(int  argc, char **argv)
{   
    int para1;
    int para2;
    cs_aal_dpid_t            dpid;
    cs_aal_spec_pkt_ctrl_t    cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    
    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port error.\n");
        return;
    }
    if (para2>AAL_PKT_TYPE_MAX) {
        diag_printf("pkt type error.\n");
        return;
    }

    memset(&cfg, 0, sizeof( cs_aal_spec_pkt_ctrl_t));

    if (aal_special_pkt_behavior_get(para1, para2, &cfg) != CS_E_OK)
    {
        diag_printf("failed to get special pkt behavior config \n");
        return;
    }
    SPEC_PORT_TO_DPID(cfg.dpid.dst_op, cfg.dpid.dpid, dpid);
    diag_printf("\n Port %d pkt_type %d's behavior\n", para1, para2);
    diag_printf("----------------------------------------\n");
    cs_printf("\t %-22s = %d\n\t %-22s = %d\n\t %-22s = %d\n\t %-22s = %d\n"
                "\t %-22s = %d\n\t %-22s = %d\n\t %-22s = %d\n",
                "permit", cfg.permit, "cos", cfg.cos, "dpid",dpid,
                "learn_en", cfg.learning, "ctrl_pri", cfg.pri,
                "police_bypass", cfg.bypass_plc, "sa_lmt_fwd", cfg.lmt_fwd);  
    cs_printf("\t %-22s = %d\n\t %-22s = %d\n"
                "\t %-22s = %d\n\t %-22s = %d\n",
                "sp_flow_en", cfg.flow_en, "sp_flow_id", cfg.flow_id,
                "sp_mirror_en", cfg.mirror_en, "sp_mirror_pkt_src_sel", cfg.mirror_src_sel);  
      
    return;
}

static void __aal_sp_cmd_cfg_set(int  argc, char **argv)
{   
    int para1;
    int para2;
    cs_aal_spec_pkt_ctrl_msk_t  cfg_msk;
    cs_aal_spec_pkt_ctrl_t          cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    
    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port error.\n");
        return;
    }
    if (para2>AAL_PKT_TYPE_MAX) {
        diag_printf("pkt type error.\n");
        return;
    }
    if(iros_strtol(argv[2])>7){
        diag_printf("pkt DPID error.\n");
        return;
    }
        
    cfg_msk.u32      = 0xffffffff;
    cfg_msk.s.rsvd   = 0;
    cfg_msk.s.permit = 0;
    cfg_msk.s.pri    = 0;
    memset(&cfg, 0, sizeof( cs_aal_spec_pkt_ctrl_t));
    SPEC_DPID_TO_PORT(iros_strtol(argv[2]), cfg.dpid.dst_op, cfg.dpid.dpid);
    cfg.bypass_plc     = iros_strtol(argv[3]);
    cfg.learning       = iros_strtol(argv[4]);
    cfg.lmt_fwd        = iros_strtol(argv[5]);
    cfg.cos            = iros_strtol(argv[6]);
    cfg.flow_en        = iros_strtol(argv[7]);
    cfg.flow_id        = iros_strtol(argv[8]);
    cfg.mirror_en      = iros_strtol(argv[9]);
    cfg.mirror_src_sel = iros_strtol(argv[10]);

    diag_printf("%s\n", (aal_special_pkt_behavior_set(para1, para2, cfg_msk, &cfg) == CS_E_OK) ? "done" : "failed"); 
   
    return;
}

static void __aal_sp_cmd_cfg_1_set(int  argc, char **argv)
{   
    int para1;
    int para2;
    int para3;
    int para4;
    cs_aal_spec_pkt_ctrl_msk_t  cfg_msk;
    cs_aal_spec_pkt_ctrl_t          cfg;

    para1 = iros_strtol(argv[0]);
    para2 = iros_strtol(argv[1]);
    para3 = iros_strtol(argv[2]);
    para4 = iros_strtol(argv[3]);
    
    if (para1>AAL_PORT_ID_MII1)
    {
        diag_printf("port error.\n");
        return;
    }
    if (para2>AAL_PKT_TYPE_MAX) {
        diag_printf("pkt type error.\n");
        return;
    }
    if (para3>31) {
        diag_printf("pkt PRI error.\n");
        return;
    }
    if (para4>1) {
        diag_printf("pkt Permit error.\n");
        return;
    }
     
    cfg_msk.u32      = 0;
    cfg_msk.s.permit = 1;
    cfg_msk.s.pri    = 1;
    memset(&cfg, 0, sizeof( cs_aal_spec_pkt_ctrl_t));
  
    cfg.pri     = para3;
    cfg.permit  = para4;

    diag_printf("%s\n", (aal_special_pkt_behavior_set(para1, para2, cfg_msk, &cfg) == CS_E_OK) ? "done" : "failed"); 
   
    return;
}
/*******************************************************************************/
static struct {
    char                   *name;
    aal_l2_cmd_func_t  exe_func;
    int                     argc;
    char                   *para;
    char                   *help;
}aal_fdb_cmds[] = {
        { "status",   __aal_fdb_cmd_status_get       , 0, "<cr>", "Display fdb debug statistics"},  
        { "show_cfg", __aal_fdb_cmd_cfg_get          , 0, "<cr>", "Display fdb config status"},  
        { "lrn_mode", __aal_fdb_cmd_lrn_mode_set     , 1, "<0(HW)/1(SW)>", "Set fdb learning mode"},  
        { "age_ctrl", __aal_fdb_cmd_age_ctrl_set     , 6, "<age_en:1-enable/0><en_msk:1-valid/0-no care><aging_cntr:1~0xfff><cntr_msk><aging_intvl:U32><intvl_msk>", "Set fdb aging controls "}, 
        { "st_move",  __aal_fdb_cmd_staton_ctrl_set  , 4, "<st_move_en:1-enable/0><en_msk:1-valid/0-no care><st_move_drop_en:1-drop/0><drop_msk>", "Set fdb station move controls "}, 
        { "st_loop",  __aal_fdb_cmd_staton_loop_set  , 4, "<st_loop_thrshld: 0-4095><trld_msk:1-valid/0-no care><st_loop_detect_drop:1-drop/0><drop_msk>", "Set fdb station loop controls "}, 
        { "query",    __aal_fdb_cmd_entry_query      , 0, "<cr>", "Query fdb table entry "}, 
        { "get",      __aal_fdb_cmd_entry_get        , 1, "<idx:0-63>", "Get fdb table entry "}, 
        { "entry",    __aal_fdb_cmd_entry_set       , 21, "<idx> <valid> <---19 paras>", "Set fdb table entry "}, 
        { "show_vlan",__aal_fdb_cmd_vlan_lmt_get     , 0, "<cr>", "Display VLAN based mac limit statistics"},  
        { "vlan_lmt", __aal_fdb_cmd_vlan_lmt_set     , 4, "<idx:0-3> <valid:1-valid/0-ignored> <mac_limit:0-64><vlanID>", "Set VLAN based mac learn limit "},
        { "show_port",__aal_fdb_cmd_port_get         , 1, "<port:0,1,2,3,4>", "Display fdb config status per port "},  
        { "learn_0",  __aal_fdb_cmd_learn_ctrl0_set  , 5, "<port:0,1,2,3,4> <learn_dis:1-disallow/0-allow> <learn_dpid_cpu_dis:1-disallow/0><learn_dpid_drop_dis:1-disallow/0><learn_lmt_ovrwrt:1-enable/0>", "Set FDB learning ctrl per port "},
        { "learn_1",  __aal_fdb_cmd_learn_ctrl1_set  , 5, "<port:0,1,2,3,4> <learn_vlan_sel:0-3> <learn_mac_vld:1-valid/0><learn_vlan_vld:1-valid/0><learn_dot1p_vld:1-valid/0>", "Set FDB learning ctrl per port "},
        { "lookup",   __aal_fdb_cmd_lookup_ctrl_set  , 5, "<port:0,1,2,3,4> <lookup_vlan_sel:0-1> <lookup_mac_vld:1-valid/0><lookup_vlan_vld:1-valid/0><lookup_dot1p_vld:1-valid/0>", "Set FDB lookup ctrl per port "},
        { "fwd",      __aal_fdb_cmd_fwd_ctrl_set     , 7, "<port:0,1,2,3,4> <pkt_fwd_dis:1-drop/0> <fwd_msk:1-valid/0-no care><sa_drop_en:1-drop/0><drop_msk:1/0><mac_match_ctrl:0-3><match_msk:1/0>", "Set FDB forward ctrl per port "},
        { "port_lmt", __aal_fdb_cmd_port_lmt_set     , 5, "<port:0,1,2,3,4> <sa_max_limit:0-64> <lmt_msk:1-valid/0-no care><sa_lmt_drop_en:1-drop/0><drop_msk:1/0>", "Set FDB port based mac learn limit  "},
};

static struct {
    char                   *name;
    aal_l2_cmd_func_t  exe_func;
    int                     argc;
    char                   *para;
    char                   *help;
}aal_port_cmds[] = {
        { "show_def",  __aal_port_cmd_def_get    , 1, "<port:0,1,2,3,4>", "Display port default config "},  
        { "def",       __aal_port_cmd_def_set    , 6, "<port:0,1,2,3,4><vlanid><cos><dot1p><dscp><tpid>", "Set port default config "},  
        { "show_cfg",  __aal_port_cmd_cfg_get    , 1, "<port:0,1,2,3,4>", "Display port config "}, 
        { "cfg_msk",   __aal_port_cmd_msk_set    , 3, "<port:0,1,2,3,4><ipv4_dscp_mask:0-63><ipv6_tc_mask:0-63>", "Set Qos mask per port "},  
        { "cfg_vlnop", __aal_port_cmd_vlnop_set  , 3, "<port:0,1,2,3,4><tx_def_vid_pop:1-enable/0><rx_def_vid_swap:1-enable/0>", "Set default vlan operaton per port "}, 
        { "cfg_uuc",   __aal_port_cmd_uuc_set    , 3, "<port:0,1,2,3,4><uuc_dpid:0~7><uuc_fwd_cos:0~7>", "Set forward config for unknown unicast per port "},  
        { "cfg_bc",    __aal_port_cmd_bc_set     , 3, "<port:0,1,2,3,4><bc_dpid:0~7><bc_fwd_cos:0~7>", "Set forward config for broadcast per port "},  
        { "cfg_mc",    __aal_port_cmd_mc_set     , 3, "<port:0,1,2,3,4><mc_dpid:0~7><mc_fwd_cos:0~7>", "Set forward config for multicast per port "}, 
        { "cfg_other", __aal_port_cmd_other_set  , 4, "<port:0,1,2,3,4><cpu_header_pkt_type:0-1><oam_lpbk_en:1-enable/0><dbg_da_sa_swap_en:1-enable/0>", "Set oam loopback per port "}, 
};

static struct {
    char                   *name;
    aal_l2_cmd_func_t  exe_func;
    int                     argc;
    char                   *para;
    char                   *help;
}aal_vlan_cmds[] = {
        { "get_entry", __aal_vlan_cmd_entry_get    , 2, "<port:0,1,2>[index(0-33)]", "Get vlan entry per FE instance"},  
        { "entry",     __aal_vlan_cmd_entry_set   , 11, "<port:0,1,2>[index(0-33)][vid_hi(mapped)][vid_lo][permit_ge][permit_pon][permit_mii0][permit_mii1][ingress_filter][egress_filter][map_valid]", "Set vlan entry per FE instance"},  
        { "show_cfg",  __aal_vlan_cmd_cfg_get      , 1, "<port:0,1,2>", "Get vlan control config per FE instance"},  
        { "map",       __aal_vlan_cmd_map_set      , 2, "<port:0,1,2>[map_en:1-enable/0]", "Set vlan ingress vlan mapping function per FE instance"},  
        
};

static struct {
    char                   *name;
    aal_l2_cmd_func_t  exe_func;
    int                     argc;
    char                   *para;
    char                   *help;
}aal_sp_cmds[] = {
        { "show_hello",    __aal_sp_cmd_hello_get    , 0, "<cr>", "Dump the hello packet MAC"},  
        { "show_udf",      __aal_sp_cmd_udf_get      , 1, "<port:0-GE,1-PON,2-MA>", "Dump the user defined special packet"},  
        { "show_cfg",      __aal_sp_cmd_cfg_get      , 2, "<port:0-GE,1-PON,2-CPU,3-MII0,4-VoIP> "
                                                          "\n\t     <pkt_type:0-normal,1-BPDU,2-EAP,3-IEEE_rsv,4-MyMAC,5-MC_A,6-MC_B,"
                                                          "\n\t\t\t 7-MC_C,8-IGMP,9-ARP,10-OAM,11-MPCP,12-DHCP,13-PPPoE_dis,14-PPPoE-sess,"
                                                          "\n\t\t\t 15-hello,16-IPv6NDP, 17-UDF0, 18-UDF1, 19-UDF2, 20-UDF3>", 
                                                          "Dump the special packet behavior"},  
        { "hello",         __aal_sp_cmd_hello_set    , 2, "<port:0-GE,1-PON,2-MA> <mac: AA:BB:CC:DD:EE:FF>", "Configure the hello packet MAC"},  
        { "udf",           __aal_sp_cmd_udf_set      , 5, "<port:0-GE,1-PON,2-MA> <index:0~4> <ev2pt> <mac check:0/1> < mac: AA:BB:CC:DD:EE:FF>", "Configure the user defined special packet"},  
        { "pkt",           __aal_sp_cmd_cfg_set     , 11, "<port:0-GE,1-PON,2-CPU,3-MII0,4-VoIP> <pkt_type:0~20>"
                                                          "\n\t     <dpid:0-7> <plc_bypass:0/1> <learn:0/1> <lmt_fwd:0/1> <cos:0~7>"
                                                          "\n\t     <flow_en:0/1> <flow_id:0~31> <mir_en:0/1> <mir_sel:0/1>", 
                                                          "Configure the special packet behavior"},  
        { "pri",         __aal_sp_cmd_cfg_1_set      , 4, "<port:0-GE,1-PON,2-CPU,3-MII0,4-VoIP> <pkt_type:0~20> <pkt_pri:0~31> <permit:0/1>", "Configure the special packet behavior--PRI,PERMIT"},  
};

void __aal_fdb_cmd_help(void)
{
    int i;

    for (i = 0; i < sizeof(aal_fdb_cmds) / sizeof(aal_fdb_cmds[0]); i++) {  
        __AAL_L2_PRINT_CMD_HELP("fdb", aal_fdb_cmds[i].name, aal_fdb_cmds[i].help);    
    }
}

void __aal_port_cmd_help(void)
{
    int i;

    for (i = 0; i < sizeof(aal_port_cmds) / sizeof(aal_port_cmds[0]); i++) {  
        __AAL_L2_PRINT_CMD_HELP("port", aal_port_cmds[i].name, aal_port_cmds[i].help);    
    }
}

void __aal_vlan_cmd_help(void)
{
    int i;

    for (i = 0; i < sizeof(aal_vlan_cmds) / sizeof(aal_vlan_cmds[0]); i++) {  
        __AAL_L2_PRINT_CMD_HELP("vlan", aal_vlan_cmds[i].name, aal_vlan_cmds[i].help);    
    }
}

void __aal_sp_cmd_help(void)
{
    int i;

    for (i = 0; i < sizeof(aal_sp_cmds) / sizeof(aal_sp_cmds[0]); i++) {  
        __AAL_L2_PRINT_CMD_HELP("sp", aal_sp_cmds[i].name, aal_sp_cmds[i].help);    
    }
}

static aal_l2_cmd_func_t  __aal_fdb_find_cmd
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
        i < (sizeof(aal_fdb_cmds)/ sizeof(aal_fdb_cmds[0]));
        ++i) {
        if (__aal_l2_cmd_cmp(aal_fdb_cmds[i].name, cmd_str)){

            if ((argc - 3) == aal_fdb_cmds[i].argc) {
                return aal_fdb_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", aal_fdb_cmds[i].argc);
                diag_printf("parameters:  %s\n", aal_fdb_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __aal_fdb_cmd_help();

    return (NULL);

}

static aal_l2_cmd_func_t  __aal_port_find_cmd
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
        i < (sizeof(aal_port_cmds)/ sizeof(aal_port_cmds[0]));
        ++i) {
        if (__aal_l2_cmd_cmp(aal_port_cmds[i].name, cmd_str)){

            if ((argc - 3) == aal_port_cmds[i].argc) {
                return aal_port_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", aal_port_cmds[i].argc);
                diag_printf("parameters:  %s\n", aal_port_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __aal_port_cmd_help();

    return (NULL);

}

static aal_l2_cmd_func_t  __aal_vlan_find_cmd
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
        i < (sizeof(aal_vlan_cmds)/ sizeof(aal_vlan_cmds[0]));
        ++i) {
        if (__aal_l2_cmd_cmp(aal_vlan_cmds[i].name, cmd_str)){

            if ((argc - 3) == aal_vlan_cmds[i].argc) {
                return aal_vlan_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", aal_vlan_cmds[i].argc);
                diag_printf("parameters:  %s\n", aal_vlan_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __aal_vlan_cmd_help();

    return (NULL);

}

static aal_l2_cmd_func_t  __aal_sp_find_cmd
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
        i < (sizeof(aal_sp_cmds)/ sizeof(aal_sp_cmds[0]));
        ++i) {
        if (__aal_l2_cmd_cmp(aal_sp_cmds[i].name, cmd_str)){

            if ((argc - 3) == aal_sp_cmds[i].argc) {
                return aal_sp_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", aal_sp_cmds[i].argc);
                diag_printf("parameters:  %s\n", aal_sp_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __aal_sp_cmd_help();

    return (NULL);

}

void aal_fdb_cmd(int argc, char **argv)
{
    aal_l2_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal fdb  ..."*/

        p_cmd_func = __aal_fdb_find_cmd(argv[2], argc);

    }
    else {
        __aal_fdb_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}

void aal_port_cmd(int argc, char **argv)
{
    aal_l2_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal port  ..."*/

        p_cmd_func = __aal_port_find_cmd(argv[2], argc);

    }
    else {
        __aal_port_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}

void aal_vlan_cmd(int argc, char **argv)
{
    aal_l2_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal vlan  ..."*/

        p_cmd_func = __aal_vlan_find_cmd(argv[2], argc);

    }
    else {
        __aal_vlan_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}

void aal_sp_cmd(int argc, char **argv)
{
    aal_l2_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal sp  ..."*/

        p_cmd_func = __aal_sp_find_cmd(argv[2], argc);

    }
    else {
        __aal_sp_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}

/*  end of file */

