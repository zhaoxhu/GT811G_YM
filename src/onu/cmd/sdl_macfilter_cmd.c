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
 * sdl_macfilter_cmd.c
 */
#include "cs_cmd.h"
#include "sdl.h"

#ifdef HAVE_SDL_CMD
#ifdef HAVE_SDL_CMD_NTT

#include "aal.h"
#include "sdl_macfilter.h"
#include "sdl_macfilter_util.h"
#include "sdl_init.h"

/* This table saves the new entry list. 
 * It is the same as OAM.
 * Thru CLI, add/del/modify one by one.
 * Then thru 'set entry/filter' cmd to update.
 * __mac_filter_new_entry_num tells you how many entries.
 * Make sure the table is continuous !!!
 */
cs_uint32 __mac_filter_new_entry_num = 0;
cs_sdl_macfilter_entry_t __mac_filter_new_entry[MAX_MAC_FILTER_ENTRY];
extern sdl_init_cfg_t sdl_int_cfg;
extern cs_sdl_macfilter_oper_mode_t __mac_oper_mode;
cs_uint32  __mfilter_new_timer = 0;
cs_uint16  __mfilter_new_tpid = 0;
extern cs_uint8 __mac_filter_check_mac(
    cs_sdl_macfilter_oper_mode_t    filter_state,
    cs_mac_t                        *mac);

#ifdef HAVE_SDL_CMD_HELP
static const char *macfilter_cmd_usage_mode[] = 
{
    "mfilter mode <0 - 2>",
    "Attention: Only expert can use this cmd",
    "   0: disable mode",
    "   1: static mode",
    "   2: dynamic mode",
    "   3: static mode with TPID",
    "   4: dynamic mode with TPID"
};

static const char *macfilter_cmd_usage_set[] = 
{
    "mfilter set <'entry', 'filter', 'mix' <0-2> >",
    "Attention: Only expert can use this cmd",
    "   entry: set entry",
    "   filter: set filter",
    "   mix: set mode (0-disable, 1-static, 2-dynamic) + entry"
};

static const char *macfilter_cmd_usage_add[] = 
{
    "mfilter add mac2 mac3 mac4 mac5 f/d aging_en ",
    "Attention: Only expert can use this cmd",
    "   mac0: macaddr byte 0, fixed 0x33",
    "   mac1: macaddr byte 1, fixed 0x33",
    "   mac2: macaddr byte 2, 0x0 - 0xff",
    "   mac3: macaddr byte 3, 0x0 - 0xff",
    "   mac4: macaddr byte 4, 0x0 - 0xff",
    "   mac5: macaddr byte 5, 0x0 - 0xff",
    "   f/d: <'f' - forward, 'd' - discard>",
    "   aging_en: <0-disable, 1-enable>"
};

static const char *macfilter_cmd_usage_del[] = 
{
    "mfilter del <0-63>",
    "Attention: Only expert can use this cmd"
};

static const char *macfilter_cmd_usage_clr[] = 
{
    "mfilter clr <'sw', 'hw>",
    "Attention: Only expert can use this cmd",
    "   sw: clr entries in SW table __mac_filter_new_entry",
    "   hw: clr entries in HW table mac_oper_table"
};

static const char *macfilter_cmd_usage_timer[] = 
{
    "mfilter timer <0-1000000>",
    "Attention: Only expert can use this cmd"
};

static const char *macfilter_cmd_usage_tpid[] = 
{
    "mfilter tpid <0-0xFFFF>",
    "Attention: Only expert can use this cmd"
};

static const char *macfilter_cmd_usage_show[] = 
{
    "mfilter show <cr>"
};
#endif

sal_cmd_result_t macfilter_cmd_timer(int argc, char **argv)
{
    cs_uint32 timer = 0;

    if(argc != 3) return SAL_CMD_INVALID_PARAM;
    timer = iros_strtol(argv[2]);
    if(timer > 10000000) return SAL_CMD_INVALID_PARAM;

    __mfilter_new_timer = timer;
    
    return 0;
}

sal_cmd_result_t macfilter_cmd_tpid(int argc, char **argv)
{
    cs_uint16 tpid = 0;

    if(argc != 3) return SAL_CMD_INVALID_PARAM;
    tpid = iros_strtol(argv[2]);

    __mfilter_new_tpid = tpid;
    
    return 0;
}

sal_cmd_result_t macfilter_cmd_clr(int argc, char **argv)
{
    cs_uint8 ii = 0;
    cs_status  rc = 0;
    cs_port_id_t  port = CS_PON_PORT_ID;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    if(cmd_cmp(argv[2], "sw")) {
        for (ii = 0; ii < 64; ii++) {
            memset(&__mac_filter_new_entry[ii], 0, sizeof(cs_sdl_macfilter_entry_t));
        }
        __mac_filter_new_entry_num = 0;
    } else if (cmd_cmp(argv[2], "hw")) {
        rc = macfilter_clear_table(port,
            sdl_int_cfg.max_mac_oper_entry, mac_oper_table);
        if (rc) {
            cs_printf("macfilter_clear_table failed\n");
        }
    } else {
        return SAL_CMD_INVALID_PARAM;
    }

    return sal_api_2_cmd_rc_map(rc);       
}



sal_cmd_result_t macfilter_cmd_del(int argc, char **argv)
{
    cs_uint8 index = 0, ii = 0;
    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    index = iros_strtol(argv[2]);

    if (index >= __mac_filter_new_entry_num) {
        cs_printf("ERROR: there are only %d entries\n", __mac_filter_new_entry_num);
        return SAL_CMD_INVALID_PARAM;
    }

    for (ii = index; ii < (__mac_filter_new_entry_num - 1); ii++) {
        memcpy(&__mac_filter_new_entry[ii], &__mac_filter_new_entry[ii+1], sizeof(cs_sdl_macfilter_entry_t));
    }
    memset(&__mac_filter_new_entry[__mac_filter_new_entry_num - 1], 0, sizeof(cs_sdl_macfilter_entry_t));

    __mac_filter_new_entry_num--;

    return 0;       
}



sal_cmd_result_t macfilter_cmd_add(int argc, char **argv)
{
    if(argc!=8) return SAL_CMD_INVALID_PARAM;

    if (__mac_filter_new_entry_num >= 64) {
        cs_printf("ERROR: entry table is full now\n");
        return SAL_CMD_FAIL;
    }

    __mac_filter_new_entry[__mac_filter_new_entry_num].mac.addr[0] = 0x33;
    __mac_filter_new_entry[__mac_filter_new_entry_num].mac.addr[1] = 0x33;
    __mac_filter_new_entry[__mac_filter_new_entry_num].mac.addr[2] = iros_strtol(argv[2]);
    __mac_filter_new_entry[__mac_filter_new_entry_num].mac.addr[3] = iros_strtol(argv[3]);
    __mac_filter_new_entry[__mac_filter_new_entry_num].mac.addr[4] = iros_strtol(argv[4]);
    __mac_filter_new_entry[__mac_filter_new_entry_num].mac.addr[5] = iros_strtol(argv[5]);

    if(cmd_cmp(argv[6], "f")) {
        __mac_filter_new_entry[__mac_filter_new_entry_num].oper = 1;
    } else {
        __mac_filter_new_entry[__mac_filter_new_entry_num].oper = 0;
    }
    
    __mac_filter_new_entry[__mac_filter_new_entry_num].aging_en = iros_strtol(argv[7]);
    
    __mac_filter_new_entry_num++;
    
    return 0;       
}


sal_cmd_result_t macfilter_cmd_mode(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  rc = 0;
    cs_port_id_t  port = CS_PON_PORT_ID;
    cs_sdl_macfilter_oper_mode_t mode = 0;
    cs_sdl_macfilter_err_code_t err_code;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    mode = iros_strtol(argv[2]);
    rc = epon_request_onu_macfilter_mode_set(context, 
        0, 0, port, mode, __mfilter_new_timer, __mfilter_new_tpid, &err_code);
    if (rc) {
        cs_printf("epon_request_onu_macfilter_mode_set failed\n");
        cs_printf("err_code %d \n", err_code);
    }
    
    return sal_api_2_cmd_rc_map(rc);       
}

sal_cmd_result_t macfilter_cmd_set(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  rc = 0;
    cs_port_id_t  port = CS_PON_PORT_ID;
    cs_sdl_macfilter_oper_mode_t mode = 0;
    cs_sdl_macfilter_err_code_t err_code;

    if(argc < 3) return SAL_CMD_INVALID_PARAM;

    if(cmd_cmp(argv[2], "entry")) {
        rc = epon_request_onu_macfilter_entry_set(context, 0, 0, port, 
            __mac_filter_new_entry_num, __mac_filter_new_entry, &err_code);
        if (rc) {
            cs_printf("epon_request_onu_macfilter_entry_set failed\n");
            cs_printf("err_code %d \n", err_code);
        }
    
    } else if(cmd_cmp(argv[2], "filter")) {
        rc = epon_request_onu_macfilter_filter_set(context, 0, 0, port, 
            __mac_filter_new_entry_num, __mac_filter_new_entry, &err_code);
        if (rc) {
            cs_printf("epon_request_onu_macfilter_filter_set failed\n");
            cs_printf("err_code %d \n", err_code);
        }
    
    } else if (cmd_cmp(argv[2], "mix")) {
        if(argc != 4) return SAL_CMD_INVALID_PARAM;
        mode = iros_strtol(argv[3]);
        
        rc = epon_request_onu_macfilter_set_mode_entry(context, 0, 0, port, 
            mode, __mac_filter_new_entry_num, __mac_filter_new_entry, 
            __mfilter_new_timer, __mfilter_new_tpid, &err_code);
        if (rc) {
            cs_printf("epon_request_onu_macfilter_set_mode_entry failed\n");
            cs_printf("err_code %d \n", err_code);
        }
    
    }
    
    return sal_api_2_cmd_rc_map(rc);       
}

void __show_macfilter_mode(void)
{
    cs_sdl_macfilter_oper_mode_t mode;
    cs_callback_context_t        context;

    epon_request_onu_macfilter_mode_get(context, 
        0, 0, CS_PON_PORT_ID, &mode, &__mfilter_new_timer, &__mfilter_new_tpid);

    cs_printf("Current mac filter mode is %d - ", mode);
    switch (mode) {
    case SDL_MACFILTER_OPER_MODE_DISABLE:
        cs_printf("DISABLE\n");
        break;
    case SDL_MACFILTER_OPER_MODE_STATIC:
        cs_printf("STATIC without TPID\n");
        break;
    case SDL_MACFILTER_OPER_MODE_DYNAMIC:
        cs_printf("DYNAMIC without TPID\n");
        break;
    case SDL_MACFILTER_OPER_MODE_STATIC_TPID:
        cs_printf("STATIC with TPID\n");
        break;
    case SDL_MACFILTER_OPER_MODE_DYNAMIC_TPID:
        cs_printf("DYNAMIC with TPID\n");
        break;
    default:
        cs_printf("INVALID!!!\n");
    }
    
    cs_printf("aging_timer             is %d\n", __mfilter_new_timer);
    cs_printf("tpid                    is %d\n", __mfilter_new_tpid);

}

void __show_macfilter_sw(void)
{
    
    cs_uint8 ii = 0;
    cs_printf("======OAM mac filter entry is shown:=======\n");
    cs_printf(" index        MAC Addr      Forward Aging\n");

    for (ii = 0; ii < __mac_filter_new_entry_num; ii++) {
        if (__mac_filter_new_entry[ii].aging_en) {
            cs_printf("  %2d     %2x_%2x_%2x_%2x_%2x_%2x     %1d %7d\n", ii,
                __mac_filter_new_entry[ii].mac.addr[0], __mac_filter_new_entry[ii].mac.addr[1], 
                __mac_filter_new_entry[ii].mac.addr[2], __mac_filter_new_entry[ii].mac.addr[3], 
                __mac_filter_new_entry[ii].mac.addr[4], __mac_filter_new_entry[ii].mac.addr[5], 
                __mac_filter_new_entry[ii].oper, 
                __mfilter_new_timer);
        } else {
            cs_printf("  %2d     %2x_%2x_%2x_%2x_%2x_%2x     %1d     N/A\n", ii,
                __mac_filter_new_entry[ii].mac.addr[0], __mac_filter_new_entry[ii].mac.addr[1], 
                __mac_filter_new_entry[ii].mac.addr[2], __mac_filter_new_entry[ii].mac.addr[3], 
                __mac_filter_new_entry[ii].mac.addr[4], __mac_filter_new_entry[ii].mac.addr[5], 
                __mac_filter_new_entry[ii].oper);
        }
    }
    cs_printf("======Totally %2d valid entries==============\n", __mac_filter_new_entry_num);

}

void __show_macfilter_entry(void)
{
    cs_int32 ii = 0, num = 0;
    cs_boolean                  aging_en;
    cs_int32                    aging_time;
    cs_port_id_t                port_id = CS_PON_PORT_ID;
    
    if (__mac_oper_mode != SDL_MACFILTER_OPER_MODE_DISABLE) {
        cs_printf("======Valid mac filter entry is shown:=====\n");
        cs_printf(" index        MAC Addr      Forward Aging\n");
        for (ii = 0; ii < sdl_int_cfg.max_mac_oper_entry; ii++) {
            if (mac_oper_table[ii].valid) {
                macfilter_get_aging(port_id, ii, 
                    &aging_en, &aging_time);

                cs_printf("  %2d     %2x_%2x_%2x_%2x_%2x_%2x     %1d   %s\n", 
                    num,
                    mac_oper_table[ii].mac.addr[0], mac_oper_table[ii].mac.addr[1], 
                    mac_oper_table[ii].mac.addr[2], mac_oper_table[ii].mac.addr[3], 
                    mac_oper_table[ii].mac.addr[4], mac_oper_table[ii].mac.addr[5], 
                    mac_oper_table[ii].forward, 
                    aging_en ? "Enabled" : "Disabled");
                num++;
            }
        }
        cs_printf("======Totally %2d valid entries==============\n", num);
    } else {
        num = 0;
        cs_printf("======INVALID mac filter entry is shown:=====\n");
        cs_printf(" index        MAC Addr\n");

        for (ii = 0; ii < sdl_int_cfg.max_mac_oper_entry; ii++) {
            if (!(__mac_filter_check_mac(__mac_oper_mode, &(mac_oper_table[ii].mac)))) {
                cs_printf("  %2d     %2x_%2x_%2x_%2x_%2x_%2x\n", 
                    num,
                    mac_oper_table[ii].mac.addr[0], mac_oper_table[ii].mac.addr[1], 
                    mac_oper_table[ii].mac.addr[2], mac_oper_table[ii].mac.addr[3], 
                    mac_oper_table[ii].mac.addr[4], mac_oper_table[ii].mac.addr[5]);
                num++;
            }
        }
        cs_printf("======Totally %2d INVALID entries==============\n", num);
    }

    return;
}

sal_cmd_result_t macfilter_cmd_show(int argc, char **argv)
{
    if((argc<2)||(argc>3)) return SAL_CMD_INVALID_PARAM;

    if(2==argc)
    {
        __show_macfilter_mode();
        __show_macfilter_entry();
        return SAL_CMD_OK; 
    }

    if(cmd_cmp(argv[2], "mode"))
    {
        __show_macfilter_mode();
    }
    else if(cmd_cmp(argv[2], "entry"))
    {
        __show_macfilter_entry();
    }
    else if(cmd_cmp(argv[2], "sw"))
    {
        __show_macfilter_sw();
    }

    return SAL_CMD_OK;       
}

static sal_cmd_reg_struct_t macfilter_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    {macfilter_cmd_show, "show", "show macfilter config/entry", macfilter_cmd_usage_show, sizeof(macfilter_cmd_usage_show)/4},
    {macfilter_cmd_mode, "mode", "set macfilter mode", macfilter_cmd_usage_mode, sizeof(macfilter_cmd_usage_mode)/4},
    {macfilter_cmd_set, "set", "set macfilter entry/filter/(mode+entry)", macfilter_cmd_usage_set, sizeof(macfilter_cmd_usage_set)/4},
    {macfilter_cmd_add, "add", "add one entry in macfilter SW table", macfilter_cmd_usage_add, sizeof(macfilter_cmd_usage_add)/4},
    {macfilter_cmd_del, "del", "del one entry in macfilter SW table", macfilter_cmd_usage_del, sizeof(macfilter_cmd_usage_del)/4},
    {macfilter_cmd_clr, "clr", "clr macfilter entires", macfilter_cmd_usage_clr , sizeof(macfilter_cmd_usage_clr)/4},
    {macfilter_cmd_timer, "timer", "set aging timer", macfilter_cmd_usage_timer, sizeof(macfilter_cmd_usage_timer)/4},
    {macfilter_cmd_tpid, "tpid", "set tpid", macfilter_cmd_usage_tpid, sizeof(macfilter_cmd_usage_tpid)/4},
#else
    {macfilter_cmd_show, "show"},// "show macfilter config/entry"},// macfilter_cmd_usage_show, sizeof(macfilter_cmd_usage_show)/4},
    {macfilter_cmd_mode, "mode"},// "set macfilter mode"},// macfilter_cmd_usage_mode, sizeof(macfilter_cmd_usage_mode)/4},
    {macfilter_cmd_set, "set"},// "set macfilter entry/filter/(mode+entry)"},// macfilter_cmd_usage_set, sizeof(macfilter_cmd_usage_set)/4},
    {macfilter_cmd_add, "add"},// "add one entry in macfilter SW table"},// macfilter_cmd_usage_add, sizeof(macfilter_cmd_usage_add)/4},
    {macfilter_cmd_del, "del"},// "del one entry in macfilter SW table"},// macfilter_cmd_usage_del, sizeof(macfilter_cmd_usage_del)/4},
    {macfilter_cmd_clr, "clr"},// "clr macfilter entires"},// macfilter_cmd_usage_clr , sizeof(macfilter_cmd_usage_clr)/4},
    {macfilter_cmd_timer, "timer"},// "set aging timer"},// macfilter_cmd_usage_timer, sizeof(macfilter_cmd_usage_timer)/4},
    {macfilter_cmd_tpid, "tpid"}, // "set tpid", macfilter_cmd_usage_tpid, sizeof(macfilter_cmd_usage_tpid)/4},
#endif
};

sal_cmd_cfg_t macfilter_cmd_cfg = {"mfilter", "mac-filter configuration", macfilter_cmd_list, sizeof(macfilter_cmd_list)/sizeof(sal_cmd_reg_struct_t)};

#endif
#ifdef HAVE_SDL_CMD_CTC

#include "sdl_macfilter.h"

extern cs_status aal_cmd_mac_match(char *str);

#ifdef HAVE_SDL_CMD_HELP

static const char *mfilter_cmd_usage_add[] = 
{
    "mfilter add [port] [mac] [mac_type] ",
    "   port: UNI port-(1-4)",    
    "   mac: aa:bb:cc:dd:ee:ff",
    "   mac_type: 1-src mac, 0-dst mac",
};

static const char *mfilter_cmd_usage_del[] = 
{
    "mfilter del [port] [mac] [mac_type]",
    "   port: UNI port-(1-4)", 
    "   mac: aa:bb:cc:dd:ee:ff",
    "   mac_type: 1-src mac, 0-dst mac",
};

static const char *mfilter_cmd_usage_clr[] = 
{
    "mfilter clr [port]",
    "   port: UNI port-(1-4)",
};

static const char *mfilter_cmd_usage_show[] = 
{
    "mfilter show <cr>"
};

static const char *mbind_cmd_usage_add[] = 
{
    "mbind add [port] [mac]",
    "   port: UNI port-(1-4)",    
    "   mac: aa:bb:cc:dd:ee:ff"
};

static const char *mbind_cmd_usage_del[] = 
{
    "mbind del [port] [mac]",
    "   port: UNI port-(1-4)", 
    "   mac: aa:bb:cc:dd:ee:ff"
};

static const char *mbind_cmd_usage_clr[] = 
{
    "mbind clr [port]",
    "   port: UNI port-(1-4)",
};

static const char *mbind_cmd_usage_show[] = 
{
    "mbind show <cr>"
};
#endif

sal_cmd_result_t mfilter_cmd_show(int argc, char **argv)
{    
    cs_callback_context_t context;
    cs_status  rc = 0;
    cs_port_id_t  port = CS_PON_PORT_ID;
    int        i;
    cs_uint8                   number =0;
    cs_sdl_mac_tbl_entry_t     mac_list[16];

       
    if(argc!=2) return SAL_CMD_INVALID_PARAM;
              
    cs_printf("+-------------------MAC filter table--------------------+\n");
       
    for(port =CS_UNI_PORT_ID1; port <= UNI_PORT_MAX; port++)
    {  
        rc = epon_request_onu_mac_filter_get_entry( context,0, 0, port, &number, mac_list);
        if(rc!=CS_E_OK)
        {
            cs_printf("|   UNI-PORTS : %d         no-resource                   |\n", port);
            cs_printf("+-------------------------------------------------------+\n");
            continue;
        }        
        
        cs_printf("|   UNI-PORTS : %d         number: %2d                    |\n", port, number);
        for(i =0; i < number; i++)
        {
            cs_printf("|              %-8s       %02x:%02x:%02x:%02x:%02x:%02x         |\n",
           (mac_list[i].mac_type==SDL_SRC_MAC)?"SRC-MAC":"DST-MAC",mac_list[i].mac.addr[0],mac_list[i].mac.addr[1],
            mac_list[i].mac.addr[2],mac_list[i].mac.addr[3],mac_list[i].mac.addr[4],mac_list[i].mac.addr[5]);
        }
        cs_printf("+-------------------------------------------------------+\n");
            
    }

    return SAL_CMD_OK;       
}

sal_cmd_result_t mfilter_cmd_add(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  rc = 0;
    int        i;
    cs_port_id_t  port = CS_PON_PORT_ID;
    cs_uint32     mac[6];
    cs_sdl_mac_tbl_entry_t entry;

    
    if(argc!=5) return SAL_CMD_INVALID_PARAM;

    port = iros_strtol(argv[2]);
    if((port>UNI_PORT_MAX)||(port<CS_UNI_PORT_ID1))
    {
        cs_printf("port error.\n");
        return EPON_RETURN_INV_PARAM;
    }
       
    if(aal_cmd_mac_match (argv[3]) != CS_E_OK)
    {
        cs_printf("Invalid mac format!must be: aa:bb:cc:dd:ee:ff\n");
        return EPON_RETURN_INV_PARAM;
    }
    sscanf(argv[3], "%02x:%02x:%02x:%02x:%02x:%02x", &mac[0], &mac[1], &mac[2], 
                            &mac[3], &mac[4], &mac[5]);
    for(i=0; i<6; i++){
        entry.mac.addr[i] = (cs_uint8)mac[i];
    }
   
    entry.vlanid = 0;
    entry.mac_type = (iros_strtol(argv[4])==0)?SDL_DST_MAC: SDL_SRC_MAC;
     
    rc = epon_request_onu_mac_filter_add_entry( context,0, 0, port, 1, &entry);
    
    return sal_api_2_cmd_rc_map(rc);       
}

sal_cmd_result_t mfilter_cmd_del(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  rc = 0;
    int        i;
    cs_port_id_t  port = CS_PON_PORT_ID;
    cs_uint32     mac[6];
    cs_sdl_mac_tbl_entry_t entry;

    
    if(argc!=5) return SAL_CMD_INVALID_PARAM;

    port = iros_strtol(argv[2]);
    if((port>UNI_PORT_MAX)||(port<CS_UNI_PORT_ID1))
    {
        cs_printf("port error.\n");
        return EPON_RETURN_INV_PARAM;
    }
       
    if(aal_cmd_mac_match (argv[3]) != CS_E_OK)
    {
        cs_printf("Invalid mac format!must be: aa:bb:cc:dd:ee:ff\n");
        return EPON_RETURN_INV_PARAM;
    }
    sscanf(argv[3], "%02x:%02x:%02x:%02x:%02x:%02x", &mac[0], &mac[1], &mac[2], 
                            &mac[3], &mac[4], &mac[5]);
    for(i=0; i<6; i++){
        entry.mac.addr[i] = (cs_uint8)mac[i];
    }
    entry.vlanid = 0;
    entry.mac_type = (iros_strtol(argv[4])==0)?SDL_DST_MAC: SDL_SRC_MAC;

    rc = epon_request_onu_mac_filter_del_entry( context,0, 0, port, 1, &entry);
    
    return sal_api_2_cmd_rc_map(rc);       
}

sal_cmd_result_t mfilter_cmd_clr(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  rc = 0;
    cs_port_id_t  port = CS_PON_PORT_ID;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    port = iros_strtol(argv[2]);
    if((port>UNI_PORT_MAX)||(port<CS_UNI_PORT_ID1))
    {
        cs_printf("port error.\n");
        return EPON_RETURN_INV_PARAM;
    }

    rc = epon_request_onu_mac_filter_clr_entry( context,0, 0, port);
   
    return sal_api_2_cmd_rc_map(rc);       
}

sal_cmd_result_t mbind_cmd_show(int argc, char **argv)
{    
    cs_callback_context_t context;
    cs_status  rc = 0;
    cs_port_id_t  port = CS_PON_PORT_ID;
    int        i;
    cs_uint8                   number =0;
    cs_sdl_mac_tbl_entry_t     mac_list[16];

       
    if(argc!=2) return SAL_CMD_INVALID_PARAM;
              
    cs_printf("+-------------------MAC bind table----------------------+\n");
       
    for(port =CS_UNI_PORT_ID1; port <= UNI_PORT_MAX; port++)
    {  
        rc = epon_request_onu_mac_bind_get_entry( context,0, 0, port, &number, mac_list);
        if(rc!=CS_E_OK)
        {
            cs_printf("|   UNI-PORTS : %d         no-resource                   |\n", port);
            cs_printf("+-------------------------------------------------------+\n");
            continue;
        }        
        
        cs_printf("|   UNI-PORTS : %d         number: %2d                    |\n", port, number);
        for(i =0; i < number; i++)
        {
            cs_printf("|              %-8s       %02x:%02x:%02x:%02x:%02x:%02x         |\n",
                                               "SRC-MAC",mac_list[i].mac.addr[0],mac_list[i].mac.addr[1],
            mac_list[i].mac.addr[2],mac_list[i].mac.addr[3],mac_list[i].mac.addr[4],mac_list[i].mac.addr[5]);
        }
        cs_printf("+-------------------------------------------------------+\n");
            
    }

    return SAL_CMD_OK;       
}

sal_cmd_result_t mbind_cmd_add(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  rc = 0;
    int        i;
    cs_port_id_t  port = CS_PON_PORT_ID;
    cs_uint32     mac[6];
    cs_sdl_mac_tbl_entry_t entry;

    
    if(argc!=4) return SAL_CMD_INVALID_PARAM;

    port = iros_strtol(argv[2]);
    if((port>UNI_PORT_MAX)||(port<CS_UNI_PORT_ID1))
    {
        cs_printf("port error.\n");
        return EPON_RETURN_INV_PARAM;
    }
       
    if(aal_cmd_mac_match (argv[3]) != CS_E_OK)
    {
        cs_printf("Invalid mac format!must be: aa:bb:cc:dd:ee:ff\n");
        return EPON_RETURN_INV_PARAM;
    }
    sscanf(argv[3], "%02x:%02x:%02x:%02x:%02x:%02x", &mac[0], &mac[1], &mac[2], 
                            &mac[3], &mac[4], &mac[5]);
    for(i=0; i<6; i++){
        entry.mac.addr[i] = (cs_uint8)mac[i];
    }
    entry.vlanid = 0;
    entry.mac_type = SDL_SRC_MAC;
  
    rc = epon_request_onu_mac_bind_add_entry( context,0, 0, port, 1, &entry);
    
    return sal_api_2_cmd_rc_map(rc);       
}

sal_cmd_result_t mbind_cmd_del(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  rc = 0;
    int        i;
    cs_port_id_t  port = CS_PON_PORT_ID;
    cs_uint32     mac[6];
    cs_sdl_mac_tbl_entry_t entry;

    
    if(argc!=4) return SAL_CMD_INVALID_PARAM;

    port = iros_strtol(argv[2]);
    if((port>UNI_PORT_MAX)||(port<CS_UNI_PORT_ID1))
    {
        cs_printf("port error.\n");
        return EPON_RETURN_INV_PARAM;
    }
       
    if(aal_cmd_mac_match (argv[3]) != CS_E_OK)
    {
        cs_printf("Invalid mac format!must be: aa:bb:cc:dd:ee:ff\n");
        return EPON_RETURN_INV_PARAM;
    }
    sscanf(argv[3], "%02x:%02x:%02x:%02x:%02x:%02x", &mac[0], &mac[1], &mac[2], 
                            &mac[3], &mac[4], &mac[5]);
    for(i=0; i<6; i++){
        entry.mac.addr[i] = (cs_uint8)mac[i];
    }
    entry.vlanid = 0;
    entry.mac_type = SDL_SRC_MAC;

    rc = epon_request_onu_mac_bind_del_entry( context,0, 0, port, 1, &entry);
    
    return sal_api_2_cmd_rc_map(rc);       
}

sal_cmd_result_t mbind_cmd_clr(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  rc = 0;
    cs_port_id_t  port = CS_PON_PORT_ID;

    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    port = iros_strtol(argv[2]);
    if((port>UNI_PORT_MAX)||(port<CS_UNI_PORT_ID1))
    {
        cs_printf("port error.\n");
        return EPON_RETURN_INV_PARAM;
    }

    rc = epon_request_onu_mac_bind_clr_entry( context,0, 0, port);
   
    return sal_api_2_cmd_rc_map(rc);       
}

static sal_cmd_reg_struct_t mfilter_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    {mfilter_cmd_show, "show", "show macfilter table", mfilter_cmd_usage_show, sizeof(mfilter_cmd_usage_show)/4},
    {mfilter_cmd_add, "add", "add one entry in macfilter table", mfilter_cmd_usage_add, sizeof(mfilter_cmd_usage_add)/4},
    {mfilter_cmd_del, "del", "del one entry in macfilter table", mfilter_cmd_usage_del, sizeof(mfilter_cmd_usage_del)/4},
    {mfilter_cmd_clr, "clr", "clr macfilter entires", mfilter_cmd_usage_clr , sizeof(mfilter_cmd_usage_clr)/4},   
#else
    {mfilter_cmd_show, "show"},// "show macfilter config/entry"},// macfilter_cmd_usage_show, sizeof(macfilter_cmd_usage_show)/4},  
    {mfilter_cmd_add, "add"},// "add one entry in macfilter SW table"},// macfilter_cmd_usage_add, sizeof(macfilter_cmd_usage_add)/4},
    {mfilter_cmd_del, "del"},// "del one entry in macfilter SW table"},// macfilter_cmd_usage_del, sizeof(macfilter_cmd_usage_del)/4},
    {mfilter_cmd_clr, "clr"},// "clr macfilter entires"},// macfilter_cmd_usage_clr , sizeof(macfilter_cmd_usage_clr)/4},
#endif
};

sal_cmd_cfg_t mac_filter_cmd_cfg = {"mfilter", "mac-filter configuration", mfilter_cmd_list, sizeof(mfilter_cmd_list)/sizeof(sal_cmd_reg_struct_t)};


static sal_cmd_reg_struct_t macbind_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    {mbind_cmd_show, "show", "show mac bind table", mbind_cmd_usage_show, sizeof(mbind_cmd_usage_show)/4},
    {mbind_cmd_add, "add", "add one entry in mac bind table", mbind_cmd_usage_add, sizeof(mbind_cmd_usage_add)/4},
    {mbind_cmd_del, "del", "del one entry in mac bind table", mbind_cmd_usage_del, sizeof(mbind_cmd_usage_del)/4},
    {mbind_cmd_clr, "clr", "clr mac bind entires", mbind_cmd_usage_clr , sizeof(mbind_cmd_usage_clr)/4},   
#else
    {mbind_cmd_show, "show"},// "show macfilter config/entry"},// macfilter_cmd_usage_show, sizeof(macfilter_cmd_usage_show)/4},  
    {mbind_cmd_add, "add"},// "add one entry in macfilter SW table"},// macfilter_cmd_usage_add, sizeof(macfilter_cmd_usage_add)/4},
    {mbind_cmd_del, "del"},// "del one entry in macfilter SW table"},// macfilter_cmd_usage_del, sizeof(macfilter_cmd_usage_del)/4},
    {mbind_cmd_clr, "clr"},// "clr macfilter entires"},// macfilter_cmd_usage_clr , sizeof(macfilter_cmd_usage_clr)/4},
#endif
};

sal_cmd_cfg_t mac_bind_cmd_cfg = {"mbind", "mac-bind configuration", macbind_cmd_list, sizeof(macbind_cmd_list)/sizeof(sal_cmd_reg_struct_t)};

#endif
#endif //#ifdef HAVE_SDL_CMD


cs_status macfilter_cmd_self_reg(void)
{
    cs_status ret = CS_E_OK;
#ifdef HAVE_SDL_CMD
#ifdef HAVE_SDL_CMD_NTT
    ret = sal_cmd_reg(&macfilter_cmd_cfg);
#endif

#ifdef HAVE_SDL_CMD_CTC

ret = sal_cmd_reg(&mac_filter_cmd_cfg);
ret = sal_cmd_reg(&mac_bind_cmd_cfg);
#endif
#endif
    return ret;
}


