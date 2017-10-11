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
 * $Id: sdl_ma_cmd.c,v 1.1.2.12.8.10.2.1.2.1.4.1 2013/09/05 09:16:42 jsheng Exp $
 */

#include "cs_cmd.h"

#ifdef HAVE_SDL_CMD

#include "cs_types.h"
#include "cs_module.h"
#include "aal_ma.h"
#include "sdl_ma.h"
#include "sdl_l2.h"
#include "sdl.h"

extern cs_boolean ma_debug;
extern __cpu_rx_stats_t __cpu_rx_stats;

sal_cmd_result_t ma_cmd_dbg(int argc, char **argv);
sal_cmd_result_t ma_cmd_pkt(int argc, char **argv);
sal_cmd_result_t ma_cmd_stat(int argc, char **argv);

static char *pkt_str[] = {  "BPDU", 
                            "802.1X",
                            "IGMP/MLD",
                            "ARP",
                            "OAM",
                            "MPCP",
                            "DHCP",
                            "IROS",
                            "PPPOE_DIS",
                            "PPPOE_SES",
                            "IP",
                            "NDP",
                            "LOOPDETECT",
                            "MYMAC",
                            "TYPE_NUM"};

static char *state_str[] = {  "TO FE",
                              "DROP",
                              "TO CPU"};

#ifdef HAVE_SDL_CMD_HELP
static const char *ma_cmd_usage_debug[] = 
{
    "ma dbg <on/off>",
    "   on: debug on",
    "   off: debug off."
};

static const char *ma_cmd_usage_pkt[] = 
{
    "ma pkt <len/mod>",
    "   port <value>: set destination port, <value:> ge/pon/mii0",
    "   len <value>: set pkt length (exclude FCS), <value>: 32~1500",
    "   mod <offset> <len> <data>: modify packet content",
    "       <offset>: 0~(len-1)",
    "       <len>: <len>+<offset> <= frame length",
    "       <data>: number in hex",
    "   send <priority>: trigger packet send with the priority",
    "   dump: show packet would be sent"
};

static const char *ma_cmd_usage_stat[] = 
{
    "ma stat",
    "   show CPU rx statistics based on frame type"
};

static const char *pkt_cmd_usage_set[] = 
{
    "pkt set <direction> <pkt_type:0~12> <state>",
    "   direction:0-upstream;1-downstream;2-both.",
    "    pkt_type:0-BPDU;1-802.1X;2-GMP;3-ARP;4-OAM;6-DHCP;",
    "             7-IROS;8-PPPOE;11-LOOPDETECT;12-MYMAC.",
    "      state: 0-TO FE;1-DROP;2-TO CPU."
};

static const char *pkt_cmd_usage_get[] = 
{
    "pkt get <direction> <pkt_type:0~12>",
    "   direction:0-upstream;1-downstream.",
    "    pkt_type:0-BPDU;1-802.1X;2-GMP;3-ARP;4-OAM;6-DHCP;",
    "             7-IROS;8-PPPOE;11-LOOPDETECT;12-MYMAC."
};

#endif

static sal_cmd_reg_struct_t ma_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    {ma_cmd_dbg, "dbg", "debug on/off", ma_cmd_usage_debug, sizeof(ma_cmd_usage_debug)/4},
    {ma_cmd_pkt, "pkt", "packet send", ma_cmd_usage_pkt, sizeof(ma_cmd_usage_pkt)/4},
    {ma_cmd_stat, "stat", "cpu rx statistics", ma_cmd_usage_stat, sizeof(ma_cmd_usage_stat)/4},
#else
    {ma_cmd_dbg, "dbg"},// "debug on/off"},// ma_cmd_usage_debug, sizeof(ma_cmd_usage_debug)/4},
    {ma_cmd_pkt, "pkt"},// "packet send"},// ma_cmd_usage_pkt, sizeof(ma_cmd_usage_pkt)/4},
    {ma_cmd_stat, "stat"},// "cpu rx statistics", ma_cmd_usage_stat, sizeof(ma_cmd_usage_stat)/4},
#endif
};

sal_cmd_cfg_t ma_cmd_cfg = {"ma", "ma configuration", ma_cmd_list, sizeof(ma_cmd_list)/sizeof(sal_cmd_reg_struct_t)};

static cs_uint8  *frame_buf     = NULL;
static cs_uint16 frame_len      = 0;
static cs_uint8  frame_port     = 0;

cs_status sdl_ma_stats_get(cs_boolean read_clear, cs_ma_stats_t *stats)
{

    cs_status rt = CS_E_OK;
    cs_uint32 value_h, value_l;
    cs_uint64 temp;

    if(NULL == stats)
    {
        SDL_MIN_LOG("null pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        return CS_E_PARAM;
    }
    
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXBYTECOUNT           , &value_l, &value_h);
    temp = value_h;
    stats->ma_rxbytecount =  (temp<<32)|value_l;
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXBYTECOUNT         , &value_l, &value_h);
    temp = value_h;
    stats->ma_txbytecount =  (temp<<32)|value_l;
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXUCPKTCNT              , &stats->ma_rxucpktcnt              , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXMCFRMCNT              , &stats->ma_rxmcfrmcnt              , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXBCFRMCNT              , &stats->ma_rxbcfrmcnt              , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXOAMFRMCNT             , &stats->ma_rxoamfrmcnt             , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXPAUSEFRMCNT           , &stats->ma_rxpausefrmcnt           , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXUNKNOWNOCFRMCNT       , &stats->ma_rxunknownocfrmcnt       , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXCRCERRFRMCNT          , &stats->ma_rxcrcerrfrmcnt          , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXUNDERSIZEFRMCNT       , &stats->ma_rxundersizefrmcnt       , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXRUNTFRMCNT            , &stats->ma_rxruntfrmcnt            , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXOVSIZEFRMCNT          , &stats->ma_rxovsizefrmcnt          , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXJABBERFRMCNT          , &stats->ma_rxjabberfrmcnt          , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXSTATSFRM64OCT         , &stats->ma_rxstatsfrm64oct         , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXSTATSFRM65TO127OCT    , &stats->ma_rxstatsfrm65to127oct    , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXSTATSFRM128TO255OCT   , &stats->ma_rxstatsfrm128to255oct   , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXSTATSFRM256TO511OCT   , &stats->ma_rxstatsfrm256to511oct   , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXSTATSFRM512TO1023OCT  , &stats->ma_rxstatsfrm512to1023oct  , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXSTATSFRM1024TO1518OCT , &stats->ma_rxstatsfrm1024to1518oct , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXSTATSFRM1519TOMAXOCT  , &stats->ma_rxstatsfrm1519tomaxoct  , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_RXFRMCOUNT              , &stats->ma_rxfrmcount              , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXUCPKTCNT              , &stats->ma_txucpktcnt              , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXMCFRMCNT              , &stats->ma_txmcfrmcnt              , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXBCFRMCNT              , &stats->ma_txbcfrmcnt              , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXOAMFRMCNT             , &stats->ma_txoamfrmcnt             , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXPAUSEFRMCNT           , &stats->ma_txpausefrmcnt           , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXCRCERRFRMCNT          , &stats->ma_txcrcerrfrmcnt          , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXOVSIZEFRMCNT          , &stats->ma_txovsizefrmcnt          , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXESTATSFRM64OCT        , &stats->ma_txestatsfrm64oct        , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXESTATSFRM65TO127OCT   , &stats->ma_txestatsfrm65to127oct   , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXESTATSFRM128TO255OCT  , &stats->ma_txestatsfrm128to255oct  , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXESTATSFRM256TO511OCT  , &stats->ma_txestatsfrm256to511oct  , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXESTATSFRM512TO1023OCT , &stats->ma_txestatsfrm512to1023oct , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXESTATSFRM1024TO1518OCT, &stats->ma_txestatsfrm1024to1518oct, &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXESTATSFRM1519TOMAXOCT , &stats->ma_txestatsfrm1519tomaxoct , &value_h);
    rt |= aal_ma_mib_get(read_clear, MIB_ID_MA_TXFRMCOUNT              , &stats->ma_txfrmcount              , &value_h);

    return rt;
}

sal_cmd_result_t ma_cmd_dbg(int argc, char **argv)
{
    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    if(cmd_cmp(argv[2], "on")) ma_debug = 1;
    else if (cmd_cmp(argv[2], "off")) ma_debug = 0;
    else return SAL_CMD_INVALID_PARAM;

    return SAL_CMD_OK;
}
sal_cmd_result_t ma_cmd_pkt(int argc, char **argv)
{
    sal_cmd_result_t rt = SAL_CMD_OK;
    cs_uint16 offset,len,f_len;
    cs_uint8 *data;
    cs_uint8 priority;
    int i;

    if(argc<3) return SAL_CMD_INVALID_PARAM;

    if(cmd_cmp(argv[2], "port"))
    {
        if(argc<4) return SAL_CMD_INVALID_PARAM;
        frame_port = iros_strtol(argv[3]);
    }

    if(cmd_cmp(argv[2], "len"))
    {
        if(argc<4) return SAL_CMD_INVALID_PARAM;

        f_len = iros_strtol(argv[3]);
        if((f_len>1500) || (f_len<32))
        {
            return SAL_CMD_INVALID_PARAM;
        }

        if(iros_strtol(argv[3])!=frame_len)
        {
            frame_len = iros_strtol(argv[3]);
            if(frame_buf!=NULL) iros_free(frame_buf);
            frame_buf = (cs_uint8*)iros_malloc(IROS_MID_SAL, frame_len);
        }
    }

    if(cmd_cmp(argv[2], "mod"))
    {
        if(argc<6) return SAL_CMD_INVALID_PARAM;

        offset = iros_strtol(argv[3]);
        len = iros_strtol(argv[4]);
        if(0==len) return SAL_CMD_INVALID_PARAM;
        if(offset>=frame_len) return SAL_CMD_INVALID_PARAM; // offset
        if((len+offset)>frame_len) return SAL_CMD_INVALID_PARAM; // len
        data = (cs_uint8*)iros_malloc(IROS_MID_SAL, len);
        if(data == NULL) return SAL_CMD_NO_RESOURCE;
        cs_str2hex((cs_int8*)argv[5], (cs_int8*)data, len);
        if(frame_buf!=NULL){
            for(i = 0; i < len; ++i)
            {
                frame_buf[i + offset] = data[i];
            }
        }
        iros_free(data);
    }

    if(cmd_cmp(argv[2], "send"))
    {
        cs_callback_context_t     context;
        if(argc<4) return SAL_CMD_INVALID_PARAM;
        if(NULL==frame_buf) return SAL_CMD_NO_RESOURCE;
		 
       priority = iros_strtol(argv[3]);

        rt = epon_request_onu_frame_send(context,0,0, frame_buf, frame_len, frame_port, priority);
        return cs_status_2_cmd_rc_map(rt);
    }

    if(cmd_cmp(argv[2], "dump"))
    {
        if(NULL==frame_buf) return SAL_CMD_NO_RESOURCE;

        for(i = 0; i<frame_len; ++i)
        {
            if(0 == (i&0xf)) cs_printf("\n%04x: ", i);
            if(0 == (i&0x7)) cs_printf(" ");
            cs_printf("%02x ", frame_buf[i]);
        }
        cs_printf(".\n");
    }

    return SAL_CMD_OK;
}

sal_cmd_result_t ma_cmd_stat(int argc, char **argv)
{
    cs_printf("\nCPU RX STATISTICS\n");
    cs_printf("========================================\n");
    cs_printf("%-30s: %u\n", "normal                    ", __cpu_rx_stats.cnt_normal                    );
    cs_printf("%-30s: %u\n", "bpdu                      ", __cpu_rx_stats.cnt_bpdu                      );
    cs_printf("%-30s: %u\n", "8021x                     ", __cpu_rx_stats.cnt_8021x                     );
    cs_printf("%-30s: %u\n", "ieee_rsrvd1               ", __cpu_rx_stats.cnt_ieee_rsrvd1               );
    cs_printf("%-30s: %u\n", "igmp                      ", __cpu_rx_stats.cnt_igmp                      );
    cs_printf("%-30s: %u\n", "arp                       ", __cpu_rx_stats.cnt_arp                       );
    cs_printf("%-30s: %u\n", "oam                       ", __cpu_rx_stats.cnt_oam                       );
    cs_printf("%-30s: %u\n", "mpcp                      ", __cpu_rx_stats.cnt_mpcp                      );
    cs_printf("%-30s: %u\n", "mymac                     ", __cpu_rx_stats.cnt_mymac                     );
    cs_printf("%-30s: %u\n", "ipv4                      ", __cpu_rx_stats.cnt_ipv4                      );
    cs_printf("%-30s: %u\n", "ipv6                      ", __cpu_rx_stats.cnt_ipv6                      );
    cs_printf("%-30s: %u\n", "hello                     ", __cpu_rx_stats.cnt_hello                     );
    cs_printf("%-30s: %u\n", "ipv6                      ", __cpu_rx_stats.cnt_ipv6                      );
    cs_printf("%-30s: %u\n", "udf0                      ", __cpu_rx_stats.cnt_udf0                      );
    cs_printf("%-30s: %u\n", "udf1                      ", __cpu_rx_stats.cnt_udf1                      );
    cs_printf("%-30s: %u\n", "udf2                      ", __cpu_rx_stats.cnt_udf2                      );
    cs_printf("%-30s: %u\n", "udf3                      ", __cpu_rx_stats.cnt_udf3                      );

    return SAL_CMD_OK;
}

sal_cmd_result_t pkt_cmd_set(int argc, char **argv)
{   
    sal_cmd_result_t rc = SAL_CMD_INVALID_CMD;
    cs_callback_context_t context;
    int para1;
    int para2;
    int para3;
       
    if(argc!=5) return SAL_CMD_INVALID_PARAM;

    para1 = iros_strtol(argv[2]);
    para2 = iros_strtol(argv[3]);
    para3 = iros_strtol(argv[4]);

    if (para1 > S_BOTH) {
        cs_printf("dir error!\n");
        return SAL_CMD_INVALID_PARAM;
    }

    if (para2 >= CS_PKT_TYPE_NUM) {
        cs_printf("pkt type error!\n");
        return SAL_CMD_INVALID_PARAM;
    }
    if (para3 > DST_CPU) {
        cs_printf("state error!\n");
        return SAL_CMD_INVALID_PARAM;
    }
 
    rc = cs_status_2_cmd_rc_map(epon_request_onu_spec_pkt_state_set(context,0,0,para1,para2,para3));
     
    return rc;
}

sal_cmd_result_t pkt_cmd_get(int argc, char **argv)
{   
    sal_cmd_result_t rc = SAL_CMD_INVALID_CMD;
    cs_callback_context_t context;
    int para1;
    int para2;
    cs_sdl_pkt_state_t state;
 
    if(argc!=4) return SAL_CMD_INVALID_PARAM;

    para1 = iros_strtol(argv[2]);
    para2 = iros_strtol(argv[3]);

    if (para1 > S_BOTH) {
        cs_printf("dir error!\n");
        return SAL_CMD_INVALID_PARAM;
    }

    if (para2 >= CS_PKT_TYPE_NUM) {
        cs_printf("pkt type error!\n");
        return SAL_CMD_INVALID_PARAM;
    }

    rc = cs_status_2_cmd_rc_map(epon_request_onu_spec_pkt_state_get(context,0,0,para1,para2,&state));
    if(rc == SAL_CMD_OK)
    {
        cs_printf("----------------------------------------\n");
        cs_printf("    Direction  : %d  \n", para1);
        cs_printf("    packet type: %-12s  \n", pkt_str[para2]);
        cs_printf("    state      : %-12s  \n", state_str[state]);
    }
    return rc;
}

static sal_cmd_reg_struct_t pkt_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    {pkt_cmd_set, "set", "protocol packet state set", pkt_cmd_usage_set, sizeof(pkt_cmd_usage_set)/4},
    {pkt_cmd_get, "get", "protocol packet state get", pkt_cmd_usage_get, sizeof(pkt_cmd_usage_get)/4},
#else
    {pkt_cmd_set, "set"},
    {pkt_cmd_get, "get"},
#endif
};

sal_cmd_cfg_t pkt_cmd_cfg = {"pkt", "protocol packet control", pkt_cmd_list, sizeof(pkt_cmd_list)/sizeof(sal_cmd_reg_struct_t)};

#endif //#ifdef HAVE_SDL_CMD

cs_status ma_cmd_self_reg(void)
{
        cs_status ret = CS_E_OK;
#ifdef HAVE_SDL_CMD
    
        ret = sal_cmd_reg(&ma_cmd_cfg);
        ret = sal_cmd_reg(&pkt_cmd_cfg);
#endif
        return ret;
}

