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
/*
 * $Id: sdl_show_cmd.c,v 1.1.2.2.2.22.8.14.2.1.2.1 2012/06/01 03:12:23 jsheng Exp $
 */

#include "cs_cmd.h"

#ifdef HAVE_SDL_CMD

#include "cs_types.h"
#include "sdl_pon.h"
#include "sdl_ma.h"
#include "sdl_port.h"
//#include "string.h"
#include "registers.h"

extern void sdl_mpcp_status_show(void);
extern void sdl_mpcp_diagnose(void);
extern void sdl_enc_cnt_show(void);
extern void sdl_enc_timer_show(void);
extern void sdl_enc_key_show(void);
extern void sdl_fec_status_show(void);
extern void sdl_dump_us_fwd_stats(void);
extern void sdl_dump_ds_fwd_stats(void);
extern void sdl_dump_us_drp_stats(void);
extern void sdl_dump_ds_drp_stats(void);
extern void sdl_dump_all_interrupt(void);
extern void sdl_dump_bm_counter(void);
extern void sdl_dump_all_interrupt(void);
extern void do_dump_bm_counter(void);
extern void sdl_fec_cnt_show(void);
extern cs_status sdl_ma_stats_get(cs_boolean read_clear, cs_ma_stats_t *stats);
sal_cmd_result_t show_cmd_mpcp(int argc, char **argv);
sal_cmd_result_t show_cmd_pon(int argc, char **argv);
sal_cmd_result_t show_cmd_uni(int argc, char **argv);
sal_cmd_result_t show_cmd_enc(int argc, char **argv);
sal_cmd_result_t show_cmd_fec(int argc, char **argv);
sal_cmd_result_t show_cmd_bm(int argc, char **argv);
sal_cmd_result_t show_cmd_ma(int argc, char **argv);
sal_cmd_result_t show_cmd_path(int argc, char **argv);
sal_cmd_result_t show_cmd_upass(int argc, char **argv);
sal_cmd_result_t show_cmd_udrop(int argc, char **argv);
sal_cmd_result_t show_cmd_dpass(int argc, char **argv);
sal_cmd_result_t show_cmd_ddrop(int argc, char **argv);
sal_cmd_result_t show_cmd_intrs(int argc, char **argv);
sal_cmd_result_t show_cmd_thread(int argc, char **argv);
sal_cmd_result_t show_cmd_system(int argc, char **argv);
sal_cmd_result_t show_cmd_msgq(int argc, char **argv);
sal_cmd_result_t show_cmd_pri_msgq(int argc, char **argv);

#ifdef HAVE_SDL_CMD_HELP
static const char *show_cmd_usage_enc[] = 
{
    "show enc"
};

static const char *show_cmd_usage_fec[] = 
{
    "show fec"
};

static const char *show_cmd_usage_mpcp[] = 
{
    "show mpcp"
};

static const char *show_cmd_usage_pon[] = 
{
    "show pon"
};

static const char *show_cmd_usage_ma[] = 
{
    "show ma"
};

static const char *show_cmd_usage_uni[] = 
{
    "show uni"
};

static const char *show_cmd_usage_path[] = 
{
    "show path"
};

static const char *show_cmd_usage_upass[] = 
{
    "show upass"
};

static const char *show_cmd_usage_udrop[] = 
{
    "show udrop"
};

static const char *show_cmd_usage_dpass[] = 
{
    "show dpass"
};

static const char *show_cmd_usage_ddrop[] = 
{
    "show ddrop"
};

static const char *show_cmd_usage_intrs[] = 
{
    "show intrs"
};

static const char *show_cmd_usage_bm[] = 
{
    "show bm"
};
#endif
static sal_cmd_reg_struct_t show_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    {show_cmd_mpcp, "mpcp", "show mpcp stats", show_cmd_usage_mpcp, sizeof(show_cmd_usage_mpcp)/4},
    {show_cmd_pon, "pon", "show pon stats", show_cmd_usage_pon, sizeof(show_cmd_usage_pon)/4},
    {show_cmd_uni, "uni", "show uni stats", show_cmd_usage_uni, sizeof(show_cmd_usage_uni)/4},
    {show_cmd_enc, "enc", "show enc stats", show_cmd_usage_enc, sizeof(show_cmd_usage_enc)/4},
    {show_cmd_fec, "fec", "show fec stats", show_cmd_usage_fec, sizeof(show_cmd_usage_fec)/4},
    {show_cmd_path, "path", "Display all the statistics, including upstream forwarding / discarding, downstream forwarding / discarding", show_cmd_usage_path, sizeof(show_cmd_usage_path)/4},
    {show_cmd_upass, "upass", "Display upstream forwarding counters", show_cmd_usage_upass, sizeof(show_cmd_usage_upass)/4},
    {show_cmd_udrop, "udrop", "Display upstream discarding counters", show_cmd_usage_udrop, sizeof(show_cmd_usage_udrop)/4},
    {show_cmd_dpass, "dpass", "Display downstream forwarding counters", show_cmd_usage_dpass, sizeof(show_cmd_usage_dpass)/4},
    {show_cmd_ddrop, "ddrop", "Display downstream discarding counters", show_cmd_usage_ddrop, sizeof(show_cmd_usage_ddrop)/4},    
    {show_cmd_intrs, "intrs", "Display interrupt stauts", show_cmd_usage_intrs, sizeof(show_cmd_usage_intrs)/4},    
    {show_cmd_bm, "bm", "show BM stats", show_cmd_usage_bm, sizeof(show_cmd_usage_bm)/4},
    {show_cmd_ma, "ma", "show MA stats", show_cmd_usage_ma, sizeof(show_cmd_usage_ma)/4}, 
    {show_cmd_thread, "thread", "show thread stats", NULL, 0}, 
    {show_cmd_system, "system", "show system stats", NULL, 0}, 
    {show_cmd_msgq, "msgq", "show message queue", NULL, 0}, 
    {show_cmd_pri_msgq, "priq", "show priority queue", NULL, 0}, 
#else
    {show_cmd_mpcp, "mpcp"},// "show mpcp stats"},// show_cmd_usage_mpcp, sizeof(show_cmd_usage_mpcp)/4},
    {show_cmd_pon, "pon"},// "show pon stats"},// show_cmd_usage_pon, sizeof(show_cmd_usage_pon)/4},
    {show_cmd_uni, "uni"},// "show uni stats"},// show_cmd_usage_uni, sizeof(show_cmd_usage_uni)/4},
    {show_cmd_enc, "enc"},// "show enc stats"},// show_cmd_usage_enc, sizeof(show_cmd_usage_enc)/4},
    {show_cmd_fec, "fec"},// "show fec stats"},// show_cmd_usage_fec, sizeof(show_cmd_usage_fec)/4},
    {show_cmd_path, "path"},// "Display all the statistics, including upstream forwarding / discarding, downstream forwarding / discarding"},// show_cmd_usage_path, sizeof(show_cmd_usage_path)/4},
    {show_cmd_upass, "upass"},// "Display upstream forwarding counters"},// show_cmd_usage_upass, sizeof(show_cmd_usage_upass)/4},
    {show_cmd_udrop, "udrop"},// "Display upstream discarding counters"},// show_cmd_usage_udrop, sizeof(show_cmd_usage_udrop)/4},
    {show_cmd_dpass, "dpass"},// "Display downstream forwarding counters"},// show_cmd_usage_dpass, sizeof(show_cmd_usage_dpass)/4},
    {show_cmd_ddrop, "ddrop"},// "Display downstream discarding counters"},// show_cmd_usage_ddrop, sizeof(show_cmd_usage_ddrop)/4},    
    {show_cmd_intrs, "intrs"},// "Display interrupt stauts"},// show_cmd_usage_intrs, sizeof(show_cmd_usage_intrs)/4},    
    {show_cmd_bm, "bm"},// "show BM stats"},// show_cmd_usage_bm, sizeof(show_cmd_usage_bm)/4},
    {show_cmd_ma, "ma"},// "show MA stats"},// show_cmd_usage_ma, sizeof(show_cmd_usage_ma)/4}, 
    {show_cmd_thread, "thread"},// "show thread stats"},// NULL, 0}, 
    {show_cmd_system, "system"},// "show thread stats"},// NULL, 0}, 
    {show_cmd_msgq, "msgq"}, 
    {show_cmd_pri_msgq, "priq"}, 
        
#endif
};

sal_cmd_cfg_t show_cmd_cfg = {"show", "show various statistics", show_cmd_list, sizeof(show_cmd_list)/sizeof(sal_cmd_reg_struct_t)};

sal_cmd_result_t show_cmd_enc(int argc, char **argv)
{

    sdl_enc_cnt_show();
    sdl_enc_timer_show();
    sdl_enc_key_show();

    return SAL_CMD_OK;
}

sal_cmd_result_t show_cmd_fec(int argc, char **argv)
{
    sdl_fec_cnt_show();
    //sdl_fec_status_show();
    
    return SAL_CMD_OK;
}

void __mpcp_status_show(void)
{
    volatile EPON_t *g_epon_base = (volatile EPON_t *)&g_lynxd_reg_ptr->epon;
    volatile GLB_t *g_glb_base = ( volatile GLB_t *)&g_lynxd_reg_ptr->glb;

    ONU_MAC_MPCP_MONITOR_CNT_t monitor_cnt = g_epon_base->MPCP_MONITOR_CNT;
    //ONU_MAC_Debug_MPCP_Gate_Counter1_t gate_cnt_1 = g_epon_base->Debug_MPCP_Gate_Counter1;
    //ONU_MAC_Debug_MPCP_Gate_Counter2_t gate_cnt_2 = g_epon_base->Debug_MPCP_Gate_Counter2;
    //ONU_MAC_Debug_MPCP_Register_Counter_t register_cnt = g_epon_base->Debug_MPCP_Register_Counter;
    ONU_MAC_PCS_Status_Reg_t pcs_status = g_epon_base->PCS_Status_Reg;
    ONU_MAC_MPCP_STATUS_t mpcp_status = g_epon_base->MPCP_STATUS;
    ONU_MAC_MPCP_OLTMAC1_t olt_mac1 = g_epon_base->MPCP_OLTMAC1;
    ONU_MAC_MPCP_OLTMAC0_t olt_mac0 = g_epon_base->MPCP_OLTMAC0;
    ONU_MAC_MPCP_LLID_SyncTime_t llid_synctime = g_epon_base->MPCP_LLID_SyncTime;
    ONU_MAC_MPCP_REGISTER_PARAMETERS1_t reg_param1 = g_epon_base->MPCP_REGISTER_PARAMETERS1;
    //ONU_MAC_MPCP_REGISTER_PARAMETERS2_t reg_param2 = g_epon_base->MPCP_REGISTER_PARAMETERS2;
    GLOBAL_ONU_MAC_ADDR1_t onu_mac1 = g_glb_base->ONU_MAC_ADDR1;
    GLOBAL_ONU_MAC_ADDR0_t onu_mac0 = g_glb_base->ONU_MAC_ADDR0;

    cs_printf("\nMPCP STATUS\n");
    cs_printf("========================================\n");
    cs_printf("%-20s: %s\n", "ONU", (mpcp_status.bf.onureg ? "REGISTERED" : "UNREGISTERED"));
    cs_printf("%-20s: %u\n", "LLID", llid_synctime.bf.LLID);
    cs_printf("%-20s: %u\n", "REG TIMES", monitor_cnt.bf.reged_cnt);    
    cs_printf("%-20s: %s\n", "PCS", (pcs_status.bf.PCS_Rx_Sync ? "UP" : "DOWN"));
    cs_printf("%-20s: %s\n", "LINK", (pcs_status.bf.Link_status ? "UP" : "DOWN"));
    cs_printf("%-20s: %u\n", "GATE TIMER EXP", monitor_cnt.bf.gate_timer_exp_cnt);
    cs_printf("%-20s: %u\n", "MPCP TIMER EXP", monitor_cnt.bf.mp_timer_exp_cnt);
    cs_printf("%-20s: %u\n", "TIMESTAMP DRIFT", monitor_cnt.bf.timestamp_drift_cnt);
    cs_printf("%-20s: %u\n", "REG FLAG", reg_param1.bf.flag);
    cs_printf("%-20s: %u\n", "PENDING_GRANT", reg_param1.bf.pending_grant);
    cs_printf("%-20s: %u\n", "SYNCTIME", reg_param1.bf.synctime);
    cs_printf("%-20s: %02x-%02x-%02x-%02x-%02x-%02x\n", "ONU MAC",
        (onu_mac1.bf.sa >> 8) & 0xff,
        (onu_mac1.bf.sa)      & 0xff,
        (onu_mac0.bf.sa >>24) & 0xff,
        (onu_mac0.bf.sa >>16) & 0xff,
        (onu_mac0.bf.sa >>8)  & 0xff,
        (onu_mac0.bf.sa)      & 0xff);

    cs_printf("%-20s: %02x-%02x-%02x-%02x-%02x-%02x\n", "OLT MAC",
        (olt_mac1.bf.olt_mac >> 8) & 0xff,
        (olt_mac1.bf.olt_mac)      & 0xff,
        (olt_mac0.bf.olt_mac >>24) & 0xff,
        (olt_mac0.bf.olt_mac >>16) & 0xff,
        (olt_mac0.bf.olt_mac >>8)  & 0xff,
        (olt_mac0.bf.olt_mac)      & 0xff);
    cs_printf("========================================\n\n");
#if 0
    cs_printf("%-20s: %s\n", "Good gate:                       %u\n", gate_cnt_1.bf.Good_gate);
    cs_printf("%-20s: %s\n", "Discovery gate:                  %u\n", gate_cnt_1.bf.Dis_gate);
    cs_printf("%-20s: %s\n", "Force report gate:               %u\n", gate_cnt_2.bf.Force_report_gate);
    cs_printf("%-20s: %s\n", "Normal gate:                     %u\n", gate_cnt_2.bf.Normal_gate);
    cs_printf("%-20s: %s\n", "Register frame with ack:         %u\n", register_cnt.bf.Reg_ack);
    cs_printf("%-20s: %s\n", "Register frame with nack:        %u\n", register_cnt.bf.Reg_nack);
    cs_printf("%-20s: %s\n", "Register frame with dereg:       %u\n", register_cnt.bf.Reg_dereg);
    cs_printf("%-20s: %s\n", "Register frame with rereg:       %u\n", register_cnt.bf.Reg_rereg);
#endif
}

void __mpcp_diagnose(void)
{
    volatile EPON_t *g_epon_base = (volatile EPON_t *)&g_lynxd_reg_ptr->epon;
    ONU_MAC_SIM_CTRL_t sim_ctrl;
    ONU_MAC_REVISION_t revision;
    ONU_MAC_OPTICAL_XCR_CTRL_t xcr_ctrl;
    ONU_MAC_MPCP_CONTROL_t mpcp_control;
    ONU_MAC_MPCP_ENABLE_CONTROL_t mpcp_enable;
    ONU_MAC_RXCTRL_t rxctrl;
    ONU_MAC_RXCTRL_t txctrl;
    ONU_MAC_PCS_Status_Reg_t pcs_status;
    ONU_MAC_INPCS_CNT_t inpcs_cnt;
    ONU_MAC_INPCS_CNT_t inpcs_cnt2;
    ONU_MAC_OUTPCS_CNT_t outpcs_cnt2;
    ONU_MAC_OUTPCS_CNT_t outpcs_cnt;
    ONU_MAC_RXMAC_DEBUG_t rxmac_debug;
    ONU_MAC_RXMAC_DEBUG_t rxmac_debug2;
    ONU_MAC_OUTLSR_CNT_t outlsr_cnt;
    ONU_MAC_OUTLSR_CNT_t outlsr_cnt2;
    ONU_MAC_OUTMAC_CNT_t outmac_cnt;
    ONU_MAC_OUTMAC_CNT_t outmac_cnt2;
    ONU_MAC_Debug_MPCP_Gate_Counter1_t gate_counter1;
    ONU_MAC_Debug_MPCP_Gate_Counter1_t gate_counter1_2;
    ONU_MAC_MPCP_STATUS_t mpcp_status;
    //volatile GLB_t *g_glb_base = ( volatile GLB_t *)&g_lynxd_reg_ptr->glb;
    cs_uint8 in_rxdv;
    cs_uint8 in_rxdv2;
    cs_uint8 sop_cnt;
    cs_uint8 sop_cnt2;
    cs_uint8 llid_match;
    cs_uint8 llid_match2;

    cs_printf("\nSW: %s  %s\n", __DATE__,__TIME__);
    // fpga/chip revision
    revision = g_epon_base->REVISION;
    cs_printf("HW: %02x/%02x/%02x(mm/dd/yy)\n\n",
                revision.bf.month,
                revision.bf.day,
                revision.bf.year);

    // sim control disable
    sim_ctrl = g_epon_base->SIM_CTRL;
    cs_printf("Check SIM_CTRL disable..............................");
    if(!sim_ctrl.wrd) cs_printf("PASS\n");
    else cs_printf("\n");

    // optical transceiver setting
    xcr_ctrl = g_epon_base->OPTICAL_XCR_CTRL;
    cs_printf("Check optical transceiver setting...................");
    if(0xd9 == xcr_ctrl.wrd) cs_printf("PASS\n");
    else cs_printf("\n");

    // mpcp_mode
    mpcp_control = g_epon_base->MPCP_CONTROL;
    cs_printf("Check MPCP mode.....................................");
    if (mpcp_control.wrd&0x4) cs_printf("PASS\n");
    else cs_printf("\n");

    // onu local offline
    mpcp_enable = g_epon_base->MPCP_ENABLE_CONTROL;
    cs_printf("Check ONU local offline.............................");
    if (!(mpcp_enable.wrd&0x2)) cs_printf("PASS\n");
    else cs_printf("\n");

    // rx/tx mac enable
    rxctrl = g_epon_base->RXCTRL;
    txctrl = g_epon_base->RXCTRL;
    cs_printf("Check rx/tx MAC enable..............................");
    if( (rxctrl.wrd & 0x1) && (txctrl.wrd & 0x1) ) cs_printf("PASS\n");
    else cs_printf("\n");

    // mpcp initiation
    cs_printf("Check MPCP initiation...............................");
    if (mpcp_enable.wrd&0x1) cs_printf("PASS\n");
    else cs_printf("\n");

    // PCS status: signal_detect, polarity
    pcs_status = g_epon_base->PCS_Status_Reg;
    cs_printf("Check PCS status....................................");
    if(0x81 == pcs_status.wrd) cs_printf("PASS\n");
    else cs_printf("\n");

    // pcs tbi code counter
    inpcs_cnt = g_epon_base->INPCS_CNT;
    cs_thread_delay(100);
    inpcs_cnt2 = g_epon_base->INPCS_CNT;
    outpcs_cnt = g_epon_base->OUTPCS_CNT;
    cs_thread_delay(100);
    outpcs_cnt2 = g_epon_base->OUTPCS_CNT;
    cs_printf("Check tbi in/out packet counter.....................");
    if((inpcs_cnt.wrd!=inpcs_cnt2.wrd) && (outpcs_cnt.wrd!=outpcs_cnt2.wrd)) cs_printf("PASS\n");
    else cs_printf("\n");

    // rxmac in_rxdv counter
    rxmac_debug = g_epon_base->RXMAC_DEBUG;
    cs_thread_delay(100);
    rxmac_debug2 = g_epon_base->RXMAC_DEBUG;
    in_rxdv = (cs_uint8)(rxmac_debug.wrd&0xff);
    in_rxdv2 = (cs_uint8)(rxmac_debug2.wrd&0xff);
    sop_cnt = (cs_uint8)((rxmac_debug.wrd>>8)&0xff);
    sop_cnt2 = (cs_uint8)((rxmac_debug2.wrd>>8)&0xff);
    llid_match = (cs_uint8)((rxmac_debug.wrd>>16)&0xff);
    llid_match2 = (cs_uint8)((rxmac_debug2.wrd>>16)&0xff);

    cs_printf("Check in_rxdv counter...............................");
    if( in_rxdv!=in_rxdv2 ) cs_printf("PASS\n");
    else cs_printf("\n");
    cs_printf("Check frame sop counter.............................");
    if( sop_cnt!=sop_cnt2 ) cs_printf("PASS\n");
    else cs_printf("\n");
    cs_printf("Check llid match counter............................");
    if( llid_match!=llid_match2 ) cs_printf("PASS\n");
    else cs_printf("\n");

    // output txmac counter
    outlsr_cnt = g_epon_base->OUTLSR_CNT;
    cs_thread_delay(100);
    outlsr_cnt2 = g_epon_base->OUTLSR_CNT;
    outmac_cnt = g_epon_base->OUTMAC_CNT;
    cs_thread_delay(100);
    outmac_cnt2 = g_epon_base->OUTMAC_CNT;

    cs_printf("Check out laser counter.............................");
    if( outlsr_cnt.wrd != outlsr_cnt2.wrd ) cs_printf("PASS\n");
    else cs_printf("\n");

    cs_printf("Check out mac counter...............................");
    if( outmac_cnt.wrd != outmac_cnt2.wrd ) cs_printf("PASS\n");    
    else cs_printf("\n");

    // comdet setting

    // discovery/gate frame count
    gate_counter1 = g_epon_base->Debug_MPCP_Gate_Counter1;
    cs_thread_delay(100);
    gate_counter1_2 = g_epon_base->Debug_MPCP_Gate_Counter1;
    cs_printf("Check Discovery/Gate counter........................");
    if( gate_counter1.wrd != gate_counter1_2.wrd ) cs_printf("PASS\n");
    else cs_printf("\n");
#if 0
    // clock switch
    GLOBAL_ONU_CTRL_t onu_ctrl = g_glb_base->ONU_CTRL;
    cs_printf("Check txclk.........................................");
    if( 0x1 == onu_ctrl.bf.tck_sel ) cs_printf("PASS\n");
    else cs_printf("\n");
#endif
    // mpcp status
    mpcp_status = g_epon_base->MPCP_STATUS;
    cs_printf("Check MPCP Status...................................");
    if (mpcp_status.wrd&0x80000000) cs_printf("PASS\n");
    else cs_printf("\n");
}

sal_cmd_result_t show_cmd_mpcp(int argc, char **argv)
{
    if(2==argc) 
        __mpcp_status_show();
    else if((cmd_cmp(argv[2], "diag")) & (3==argc))
        __mpcp_diagnose();
    else
        return SAL_CMD_INVALID_PARAM;
    
    return SAL_CMD_OK;
    
    return SAL_CMD_OK;
}


sal_cmd_result_t show_cmd_pon(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_port_id_t  port = CS_PON_PORT_ID ;
    cs_pon_stats_t stats;

    epon_request_onu_pon_stats_get(context, 0, port, 0, &stats);

    cs_printf("\nPON STATISTICS\n");
    cs_printf("========================================\n");
    cs_printf("%-30s: %llu\n", "pon_byte_cnt         ", stats.pon_byte_cnt           );
    cs_printf("%-30s: %llu\n", "pon_tx_byte_cnt      ", stats.pon_tx_byte_cnt        );
    cs_printf("%-30s: %u\n", "pon_frame_cnt          ", stats.pon_frame_cnt          );
    cs_printf("%-30s: %u\n", "pon_txframe_cnt        ", stats.pon_txframe_cnt        );
    cs_printf("%-30s: %u\n", "pon_crcerr_cnt         ", stats.pon_crcerr_cnt         );
    cs_printf("%-30s: %u\n", "pon_mcframe_cnt        ", stats.pon_mcframe_cnt        );
    cs_printf("%-30s: %u\n", "pon_bcframe_cnt        ", stats.pon_bcframe_cnt        );
    cs_printf("%-30s: %u\n", "pon_txmcframe_cnt      ", stats.pon_txmcframe_cnt      );
    cs_printf("%-30s: %u\n", "pon_txbcframe_cnt      ", stats.pon_txbcframe_cnt      );
    cs_printf("%-30s: %u\n", "pon_ctrlframe_cnt      ", stats.pon_ctrlframe_cnt      );
    cs_printf("%-30s: %u\n", "pon_txctrlframe_cnt    ", stats.pon_txctrlframe_cnt    );
    cs_printf("%-30s: %u\n", "pon_pauseframe_cnt     ", stats.pon_pauseframe_cnt     );
    cs_printf("%-30s: %u\n", "pon_unknownop_cnt      ", stats.pon_unknownop_cnt      );
    cs_printf("%-30s: %u\n", "pon_runt_cnt           ", stats.pon_runt_cnt           );
    cs_printf("%-30s: %u\n", "pon_oversize_cnt       ", stats.pon_oversize_cnt       );
    cs_printf("%-30s: %u\n", "pon_rmon64_cnt         ", stats.pon_rmon64_cnt         );
    cs_printf("%-30s: %u\n", "pon_rmon65_127_cnt     ", stats.pon_rmon65_127_cnt     );
    cs_printf("%-30s: %u\n", "pon_rmon128_255_cnt    ", stats.pon_rmon128_255_cnt    );
    cs_printf("%-30s: %u\n", "pon_rmon256_511_cnt    ", stats.pon_rmon256_511_cnt    );
    cs_printf("%-30s: %u\n", "pon_rmon512_1023_cnt   ", stats.pon_rmon512_1023_cnt   );
    cs_printf("%-30s: %u\n", "pon_rmon1024_1518_cnt  ", stats.pon_rmon1024_1518_cnt  );
    cs_printf("%-30s: %u\n", "pon_txrmon64_cnt       ", stats.pon_txrmon64_cnt       );
    cs_printf("%-30s: %u\n", "pon_txrmon65_127_cnt   ", stats.pon_txrmon65_127_cnt   );
    cs_printf("%-30s: %u\n", "pon_txrmon128_255_cnt  ", stats.pon_txrmon128_255_cnt  );
    cs_printf("%-30s: %u\n", "pon_txrmon256_511_cnt  ", stats.pon_txrmon256_511_cnt  );
    cs_printf("%-30s: %u\n", "pon_txrmon512_1023_cnt ", stats.pon_txrmon512_1023_cnt );
    cs_printf("%-30s: %u\n", "pon_txrmon1024_1518_cnt", stats.pon_txrmon1024_1518_cnt);
    cs_printf("%-30s: %u\n", "pon_mpcp_uc_rev_cnt     ", stats.pon_mpcp_uc_rev_cnt     );
    cs_printf("%-30s: %u\n", "pon_mpcp_uc_reg_ack     ", stats.pon_mpcp_uc_reg_ack     );
    cs_printf("%-30s: %u\n", "pon_mpcp_uc_reg_nack    ", stats.pon_mpcp_uc_reg_nack    );
    cs_printf("%-30s: %u\n", "pon_mpcp_uc_reg_dereg   ", stats.pon_mpcp_uc_reg_dereg   );
    cs_printf("%-30s: %u\n", "pon_mpcp_uc_reg_rereg   ", stats.pon_mpcp_uc_reg_rereg   );
    cs_printf("%-30s: %u\n", "pon_mpcp_uc_reg_reg_rev ", stats.pon_mpcp_uc_reg_reg_rev );
    cs_printf("%-30s: %u\n", "pon_mpcp_uc_gat_rev     ", stats.pon_mpcp_uc_gat_rev     );
    cs_printf("%-30s: %u\n", "pon_mpcp_uc_gat_norm    ", stats.pon_mpcp_uc_gat_norm    );
    cs_printf("%-30s: %u\n", "pon_mpcp_uc_gat_frpt    ", stats.pon_mpcp_uc_gat_frpt    );
    cs_printf("%-30s: %u\n", "pon_mpcp_uc_gat_udis    ", stats.pon_mpcp_uc_gat_udis    );
    cs_printf("%-30s: %u\n", "pon_mpcp_uc_gat_bdis    ", stats.pon_mpcp_uc_gat_bdis    );
    cs_printf("%-30s: %u\n", "pon_mpcp_mc_rev_cnt     ", stats.pon_mpcp_mc_rev_cnt     );
    cs_printf("%-30s: %u\n", "pon_mpcp_mc_reg_ack     ", stats.pon_mpcp_mc_reg_ack     );
    cs_printf("%-30s: %u\n", "pon_mpcp_mc_reg_nack    ", stats.pon_mpcp_mc_reg_nack    );
    cs_printf("%-30s: %u\n", "pon_mpcp_mc_reg_dereg   ", stats.pon_mpcp_mc_reg_dereg   );
    cs_printf("%-30s: %u\n", "pon_mpcp_mc_reg_rereg   ", stats.pon_mpcp_mc_reg_rereg   );
    cs_printf("%-30s: %u\n", "pon_mpcp_mc_reg_reg_rev ", stats.pon_mpcp_mc_reg_reg_rev );
    cs_printf("%-30s: %u\n", "pon_mpcp_mc_gat_rev     ", stats.pon_mpcp_mc_gat_rev     );
    cs_printf("%-30s: %u\n", "pon_mpcp_mc_gat_norm    ", stats.pon_mpcp_mc_gat_norm    );
    cs_printf("%-30s: %u\n", "pon_mpcp_mc_gat_frpt    ", stats.pon_mpcp_mc_gat_frpt    );
    cs_printf("%-30s: %u\n", "pon_mpcp_mc_gat_udis    ", stats.pon_mpcp_mc_gat_udis    );
    cs_printf("%-30s: %u\n", "pon_mpcp_mc_gat_bdis    ", stats.pon_mpcp_mc_gat_bdis    );
    cs_printf("%-30s: %u\n", "pon_mpcp_bc_rev_cnt     ", stats.pon_mpcp_bc_rev_cnt     );
    cs_printf("%-30s: %u\n", "pon_mpcp_bc_reg_ack     ", stats.pon_mpcp_bc_reg_ack     );
    cs_printf("%-30s: %u\n", "pon_mpcp_bc_reg_nack    ", stats.pon_mpcp_bc_reg_nack    );
    cs_printf("%-30s: %u\n", "pon_mpcp_bc_reg_dereg   ", stats.pon_mpcp_bc_reg_dereg   );
    cs_printf("%-30s: %u\n", "pon_mpcp_bc_reg_rereg   ", stats.pon_mpcp_bc_reg_rereg   );
    cs_printf("%-30s: %u\n", "pon_mpcp_bc_reg_reg_rev ", stats.pon_mpcp_bc_reg_reg_rev );
    cs_printf("%-30s: %u\n", "pon_mpcp_bc_gat_rev     ", stats.pon_mpcp_bc_gat_rev     );
    cs_printf("%-30s: %u\n", "pon_mpcp_bc_gat_norm    ", stats.pon_mpcp_bc_gat_norm    );
    cs_printf("%-30s: %u\n", "pon_mpcp_bc_gat_frpt    ", stats.pon_mpcp_bc_gat_frpt    );
    cs_printf("%-30s: %u\n", "pon_mpcp_bc_gat_udis    ", stats.pon_mpcp_bc_gat_udis    );
    cs_printf("%-30s: %u\n", "pon_mpcp_bc_gat_bdis    ", stats.pon_mpcp_bc_gat_bdis    );
    cs_printf("%-30s: %u\n", "pon_mpcp_tx_req_dereg   ", stats.pon_mpcp_tx_req_dereg   );
    cs_printf("%-30s: %u\n", "pon_mpcp_tx_req_reg     ", stats.pon_mpcp_tx_req_reg     );
    cs_printf("%-30s: %u\n", "pon_mpcp_tx_ack_ack     ", stats.pon_mpcp_tx_ack_ack     );
    cs_printf("%-30s: %u\n", "pon_mpcp_tx_ack_nack    ", stats.pon_mpcp_tx_ack_nack    );
    cs_printf("%-30s: %u\n", "pon_mpcp_tx_rpt    ", stats.pon_mpcp_tx_rpt    );
    cs_printf("%-30s: %u\n", "pon_mpcp_tx_oam    ", stats.pon_mpcp_tx_oam    );
    cs_printf("%-30s: %u\n", "pon_mpcp_da_mc_nslf", stats.pon_mpcp_da_mc_nslf);
    cs_printf("%-30s: %u\n", "pon_mpcp_da_mc_slf ", stats.pon_mpcp_da_mc_slf );
    cs_printf("%-30s: %u\n", "pon_mpcp_da_uc     ", stats.pon_mpcp_da_uc     );
    cs_printf("%-30s: %u\n", "pon_framecnt       ", stats.pon_framecnt       );
    cs_printf("%-30s: %u\n", "pon_txoversizecnt  ", stats.pon_txoversizecnt  );
    cs_printf("%-30s: %u\n", "pon_rx1519_maxcnt  ", stats.pon_rx1519_maxcnt  );
    cs_printf("%-30s: %u\n", "pon_tx1519_maxcnt  ", stats.pon_tx1519_maxcnt  );
    cs_printf("%-30s: %u\n", "pon_txframecnt     ", stats.pon_txframecnt     );
    cs_printf("%-30s: %u\n", "pon_txpausecnt     ", stats.pon_txpausecnt     );
    cs_printf("%-30s: %u\n", "pon_undersizecnt   ", stats.pon_undersizecnt   );
    cs_printf("%-30s: %u\n", "pon_jabbercnt      ", stats.pon_jabbercnt      );
    cs_printf("%-30s: %u\n", "pon_extensioncnt   ", stats.pon_extensioncnt   );
    cs_printf("%-30s: %u\n", "pon_txextensioncnt ", stats.pon_txextensioncnt );
    cs_printf("%-30s: %u\n", "pon_err_symbol     ", stats.pon_err_symbol     );
    cs_printf("%-30s: %u\n", "pon_outofsync      ", stats.pon_outofsync      );
    cs_printf("%-30s: %u\n", "pon_sld_err        ", stats.pon_sld_err        );
    cs_printf("%-30s: %u\n", "pon_crc8_err       ", stats.pon_crc8_err       );
    cs_printf("%-30s: %u\n", "pon_mac_drop       ", stats.pon_mac_drop       );
    cs_printf("%-30s: %u\n", "pon_ipg_drop       ", stats.pon_ipg_drop       );
    cs_printf("%-30s: %u\n", "pon_drain          ", stats.pon_drain          );
    cs_printf("%-30s: %u\n", "pon_tx_crc_err     ", stats.pon_tx_crc_err     );
    cs_printf("========================================\n");

    return SAL_CMD_OK;
}

sal_cmd_result_t show_cmd_ma(int argc, char **argv)
{
    cs_ma_stats_t stats;

    sdl_ma_stats_get(0, &stats);

    cs_printf("\nMA STATISTICS\n");
    cs_printf("========================================\n");
    cs_printf("%-30s: %llu\n", "ma_rxbytecount             ", stats.ma_rxbytecount             );
    cs_printf("%-30s: %llu\n", "ma_txbytecount             ", stats.ma_txbytecount             );
    cs_printf("%-30s: %u\n", "ma_rxucpktcnt              ", stats.ma_rxucpktcnt              );
    cs_printf("%-30s: %u\n", "ma_rxmcfrmcnt              ", stats.ma_rxmcfrmcnt              );
    cs_printf("%-30s: %u\n", "ma_rxbcfrmcnt              ", stats.ma_rxbcfrmcnt              );
    cs_printf("%-30s: %u\n", "ma_rxoamfrmcnt             ", stats.ma_rxoamfrmcnt             );
    cs_printf("%-30s: %u\n", "ma_rxpausefrmcnt           ", stats.ma_rxpausefrmcnt           );
    cs_printf("%-30s: %u\n", "ma_rxunknownocfrmcnt       ", stats.ma_rxunknownocfrmcnt       );
    cs_printf("%-30s: %u\n", "ma_rxcrcerrfrmcnt          ", stats.ma_rxcrcerrfrmcnt          );
    cs_printf("%-30s: %u\n", "ma_rxundersizefrmcnt       ", stats.ma_rxundersizefrmcnt       );
    cs_printf("%-30s: %u\n", "ma_rxruntfrmcnt            ", stats.ma_rxruntfrmcnt            );
    cs_printf("%-30s: %u\n", "ma_rxovsizefrmcnt          ", stats.ma_rxovsizefrmcnt          );
    cs_printf("%-30s: %u\n", "ma_rxjabberfrmcnt          ", stats.ma_rxjabberfrmcnt          );
    cs_printf("%-30s: %u\n", "ma_rxstatsfrm64oct         ", stats.ma_rxstatsfrm64oct         );
    cs_printf("%-30s: %u\n", "ma_rxstatsfrm65to127oct    ", stats.ma_rxstatsfrm65to127oct    );
    cs_printf("%-30s: %u\n", "ma_rxstatsfrm128to255oct   ", stats.ma_rxstatsfrm128to255oct   );
    cs_printf("%-30s: %u\n", "ma_rxstatsfrm256to511oct   ", stats.ma_rxstatsfrm256to511oct   );
    cs_printf("%-30s: %u\n", "ma_rxstatsfrm512to1023oct  ", stats.ma_rxstatsfrm512to1023oct  );
    cs_printf("%-30s: %u\n", "ma_rxstatsfrm1024to1518oct ", stats.ma_rxstatsfrm1024to1518oct );
    cs_printf("%-30s: %u\n", "ma_rxstatsfrm1519tomaxoct  ", stats.ma_rxstatsfrm1519tomaxoct  );
    cs_printf("%-30s: %u\n", "ma_rxfrmcount              ", stats.ma_rxfrmcount              );
    cs_printf("%-30s: %u\n", "ma_txucpktcnt              ", stats.ma_txucpktcnt              );
    cs_printf("%-30s: %u\n", "ma_txmcfrmcnt              ", stats.ma_txmcfrmcnt              );
    cs_printf("%-30s: %u\n", "ma_txbcfrmcnt              ", stats.ma_txbcfrmcnt              );
    cs_printf("%-30s: %u\n", "ma_txoamfrmcnt             ", stats.ma_txoamfrmcnt             );
    cs_printf("%-30s: %u\n", "ma_txpausefrmcnt           ", stats.ma_txpausefrmcnt           );
    cs_printf("%-30s: %u\n", "ma_txcrcerrfrmcnt          ", stats.ma_txcrcerrfrmcnt          );
    cs_printf("%-30s: %u\n", "ma_txovsizefrmcnt          ", stats.ma_txovsizefrmcnt          );
    cs_printf("%-30s: %u\n", "ma_txestatsfrm64oct        ", stats.ma_txestatsfrm64oct        );
    cs_printf("%-30s: %u\n", "ma_txestatsfrm65to127oct   ", stats.ma_txestatsfrm65to127oct   );
    cs_printf("%-30s: %u\n", "ma_txestatsfrm128to255oct  ", stats.ma_txestatsfrm128to255oct  );
    cs_printf("%-30s: %u\n", "ma_txestatsfrm256to511oct  ", stats.ma_txestatsfrm256to511oct  );
    cs_printf("%-30s: %u\n", "ma_txestatsfrm512to1023oct ", stats.ma_txestatsfrm512to1023oct );
    cs_printf("%-30s: %u\n", "ma_txestatsfrm1024to1518oct", stats.ma_txestatsfrm1024to1518oct);
    cs_printf("%-30s: %u\n", "ma_txestatsfrm1519tomaxoct ", stats.ma_txestatsfrm1519tomaxoct );
    cs_printf("%-30s: %u\n", "ma_txfrmcount              ", stats.ma_txfrmcount              );
    cs_printf("========================================\n");

    return SAL_CMD_OK;
}



 const char *actualspeed2string[] = 
{
    "10M",
    "100M",
    "1000M"
};

 const char *actualduplex2string[] = 
{
    "FULL",
    "HALF"
};

 const char *autoneg2string[] = 
{
    "Autoneg DOWN",
    "Autoneg UP"
};

void __show_uni_status(cs_port_id_t            portid)
{
    cs_callback_context_t   context;
    cs_status               status = 0;
    cs_sdl_port_link_status_t    link_status;
    cs_sdl_port_ether_speed_t    speed;
    cs_sdl_port_ether_duplex_t   duplex;
    cs_sdl_port_autoneg_status_t auto_neg;

    //portid = CS_UNI_PORT_ID1;
    if(portid == CS_UPLINK_PORT || portid == CS_DOWNLINK_PORT){
        return;
    }
    status = epon_request_onu_port_link_status_get(context, 0, 0, portid, &link_status);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_link_status_get failed \n");    
        return;       
    }

    status = epon_request_onu_port_speed_get(context, 0, 0, portid, &speed);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_speed_get failed \n");    
        return;       
    }

    status = epon_request_onu_port_duplex_get(context, 0, 0, portid, &duplex);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_duplex_get failed \n");    
        return;       
    }

    status = epon_request_onu_port_auto_neg_get(context, 0, 0, portid, &auto_neg);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_auto_neg_get failed \n");    
        return;       
    }

    cs_printf("----------------------------------\n");
    cs_printf("Link           is %s!\n", (link_status == SDL_PORT_ADMIN_UP) ? "UP" : "DOWN");
    cs_printf("Actual speed   is %s!\n", actualspeed2string[speed]);
    cs_printf("Actual duplex  is %s\n", actualduplex2string[duplex]);
    cs_printf("Auto neg       is %s\n", autoneg2string[auto_neg]);
    cs_printf("----------------------------------\n");
    
}

void __show_uni_counter( cs_port_id_t            portid)
{
    cs_callback_context_t context;
    cs_status               status = 0;
    //cs_port_id_t            portid;
    cs_sdl_port_uni_stats_t mib;

    status = epon_request_onu_port_stats_get(context, 0, 0, portid, 0, &mib);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_port_stats_get failed \n");    
        return;       
    }
    cs_printf("----------------------------------\n");
    cs_printf("RxUCPktCnt                = %u\n", mib.rxucfrm_cnt);
    cs_printf("RxMCFrmCnt                = %u\n", mib.rxmcfrm_cnt);
    cs_printf("RxBCFrmCnt                = %u\n", mib.rxbcfrm_cnt);
    cs_printf("RxOAMFrmCnt               = %u\n", mib.rxoamfrm_cnt);
    cs_printf("RxPauseFrmCnt             = %u\n", mib.rxpausefrm_cnt);
    cs_printf("RxUnKnownOCFrmCnt         = %u\n", mib.rxunknownopfrm_cnt);
    cs_printf("RxCrcErrFrmCnt            = %u\n", mib.rxcrcerrfrm_cnt);
    cs_printf("RxUndersizeFrmCnt         = %u\n", mib.rxundersizefrm_cnt);
    cs_printf("RxRuntFrmCnt              = %u\n", mib.rxruntfrm_cnt);
    cs_printf("RxOvSizeFrmCnt            = %u\n", mib.rxoversizefrm_cnt);
    cs_printf("RxJabberFrmCnt            = %u\n", mib.rxjabberfrm_cnt);
    cs_printf("RxStatsFrm64              = %u\n", mib.rxstatsfrm64_cnt);
    cs_printf("RxStatsFrm65to127Oct      = %u\n", mib.rxstatsfrm65_127_cnt);
    cs_printf("RxStatsFrm128to255Oct     = %u\n", mib.rxstatsfrm128_255_cnt);
    cs_printf("RxStatsFrm256to511Oct     = %u\n", mib.rxstatsfrm256_511_cnt);
    cs_printf("RxStatsFrm512to1023Oct    = %u\n", mib.rxstatsfrm512_1023_cnt);
    cs_printf("RxStatsFrm1024to1518Oct   = %u\n", mib.rxstatsfrm1024_1518_cnt);
    cs_printf("RxStatsFrm1519toMaxOct    = %u\n", mib.rxstatsfrm1519_Max_cnt);
    cs_printf("RxByteCount               = %llu\n", mib.rxbyte_cnt);
    cs_printf("RxPktCount                = %u\n", mib.rxfrm_cnt);
    cs_printf("TxUCPktCnt                = %u\n", mib.txucfrm_cnt);
    cs_printf("TxMCFrmCnt                = %u\n", mib.txmcfrm_cnt);
    cs_printf("TxBCFrmCnt                = %u\n", mib.txbcfrm_cnt);
    cs_printf("TxOAMFrmCnt               = %u\n", mib.txoamfrm_cnt);
    cs_printf("TxPauseFrmCnt             = %u\n", mib.txpausefrm_cnt);
    cs_printf("TxCrcErrFrmCnt            = %u\n", mib.txcrcerrfrm_cnt);
    cs_printf("TxOvSizeFrmCnt            = %u\n", mib.txoversizefrm_cnt);
    cs_printf("TxSingleColFrm            = %u\n", mib.txsinglecolfrm_cnt);
    cs_printf("TxMultiColFrm             = %u\n", mib.txmulticolfrm_cnt);
    cs_printf("TxLateColFrm              = %u\n", mib.txlatecolfrm_cnt);
    cs_printf("TxExessColFrm             = %u\n", mib.txexesscolfrm_cnt);
    cs_printf("TxEStatsFrm64Oct          = %u\n", mib.txstatsfrm64_cnt);
    cs_printf("TxEStatsFrm65to127Oct     = %u\n", mib.txstatsfrm65_127_cnt);
    cs_printf("TxEStatsFrm128to255Oct    = %u\n", mib.txstatsfrm128_255_cnt);
    cs_printf("TxEStatsFrm256to511Oct    = %u\n", mib.txstatsfrm256_511_cnt);
    cs_printf("TxEStatsFrm512to1023Oct   = %u\n", mib.txstatsfrm512_1023_cnt);
    cs_printf("TxEStatsFrm1024to1518Oc   = %u\n", mib.txstatsfrm1024_1518_cnt);
    cs_printf("TxEStatsFrm1519toMaxOct   = %u\n", mib.txstatsfrm1519_Max_cnt);
    cs_printf("TxByteCount               = %llu\n", mib.txbyte_cnt);
    cs_printf("TxPktCount                = %u\n", mib.txfrm_cnt);
    cs_printf("----------------------------------\n");
}

sal_cmd_result_t show_cmd_uni(int argc, char **argv)
{
    cs_port_id_t port;
    
    if((argc != 3) && (argc != 4)){
        return SAL_CMD_INVALID_PARAM;
    }
    
    port = (cs_port_id_t)iros_strtol(argv[2]);
    
    if (argc == 3) {
        __show_uni_status(port);
        __show_uni_counter(port);
    } 
    else{
        if (cmd_cmp(argv[3], "status")) {
            __show_uni_status(port);
        } 
        else if (cmd_cmp(argv[3], "stats")) {
            __show_uni_counter(port);
        }
        else{
            return SAL_CMD_INVALID_PARAM;
        }
    }

    return SAL_CMD_OK;
}

#ifdef HAVE_SDL_CMD

sal_cmd_result_t show_cmd_path(int argc, char **argv)
{
    sdl_dump_us_fwd_stats();
    //HAL_DELAY_US(1000);
    cs_thread_delay(10);
    sdl_dump_ds_fwd_stats();
    //HAL_DELAY_US(1000);
    cs_thread_delay(10);
    sdl_dump_us_drp_stats();
    //HAL_DELAY_US(1000);
    cs_thread_delay(10);
    sdl_dump_ds_drp_stats();
    //HAL_DELAY_US(1000);
    cs_thread_delay(10);
    sdl_dump_all_interrupt();  
    return SAL_CMD_OK;
}

sal_cmd_result_t show_cmd_upass(int argc, char **argv)
{
    sdl_dump_us_fwd_stats();
    return SAL_CMD_OK;
}

sal_cmd_result_t show_cmd_udrop(int argc, char **argv)
{
    sdl_dump_us_drp_stats();
    return SAL_CMD_OK;
}

sal_cmd_result_t show_cmd_dpass(int argc, char **argv)
{
    sdl_dump_ds_fwd_stats();
    return SAL_CMD_OK;
}

sal_cmd_result_t show_cmd_ddrop(int argc, char **argv)
{
    sdl_dump_ds_drp_stats();
    return SAL_CMD_OK;
}

sal_cmd_result_t show_cmd_intrs(int argc, char **argv)
{
    sdl_dump_all_interrupt();
    return SAL_CMD_OK;
}

sal_cmd_result_t show_cmd_bm(int argc, char **argv)
{
    do_dump_bm_counter();
    return SAL_CMD_OK;
}
#endif
#endif //#ifdef HAVE_SDL_CMD

cs_status show_cmd_self_reg(void)
{
    cs_status ret = CS_E_OK;
#ifdef HAVE_SDL_CMD
    ret = sal_cmd_reg(&show_cmd_cfg);
#endif
    return ret;
}

