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
#include "cs_types.h"
#include "aal.h"
#include "aal_util.h"
#include "aal_l2.h"


/*
 * externs
 */
extern int cs_atoi(char *str);
extern void aal_enc_cmd(int argc, char **argv);
extern void aal_fec_cmd(int argc, char **argv);
extern void aal_fdb_cmd(int argc, char **argv);
extern void aal_port_cmd(int argc, char **argv);
extern void aal_vlan_cmd(int argc, char **argv);
extern void aal_sp_cmd(int argc, char **argv);
extern void aal_flow_cmd(int argc, char **argv);
extern void aal_bm_cmd(int argc, char **argv);
extern void aal_pon_cmd(int argc, char **argv);
extern void aal_mpcp_cmd(int argc, char **argv);


extern void aal_cls_cmd(int argc, char **argv);
#ifndef HAVE_MPORTS
extern void aal_ptp_cmd(int argc, char **argv);
#endif

#if 0
extern void do_dump_bm_register();
extern void do_dump_bm_counter();
extern void do_dump_us_fwd_stats(void);
extern void do_dump_ds_fwd_stats(void);
extern void do_dump_us_drp_stats(void);
extern void do_dump_ds_drp_stats(void);
extern void do_dump_all_interrupt(void);
extern void do_clear_all_counter(void);

void do_aal_test(cs_uint32 test_num,
                 cs_uint32 para1,
                 cs_uint32 para2,
                 cs_uint32 para3,
                 cs_uint32 para4,
                 cs_uint32 para5,
                 cs_uint32 para6
                )
{

    switch(test_num){
        case 23:
            do_dump_bm_register();
            break;
        case 24:
            do_dump_bm_counter();      
            break;
        case 100:
            do_dump_us_fwd_stats();
            //HAL_DELAY_US(1000);
            cs_thread_delay(10);
            do_dump_ds_fwd_stats();
            //HAL_DELAY_US(1000);
            cs_thread_delay(10);
            do_dump_us_drp_stats();
            //HAL_DELAY_US(1000);
            cs_thread_delay(10);
            do_dump_ds_drp_stats();
            //HAL_DELAY_US(1000);
            cs_thread_delay(10);
            do_dump_all_interrupt();
            break;
        case 101:
            switch(para1){
                case 1:
                    do_dump_us_fwd_stats();
                    break;
                case 2:
                    do_dump_ds_fwd_stats();
                    break;
                case 3:
                    do_dump_us_drp_stats();
                    break;
                case 4:
                    do_dump_ds_drp_stats();
                    break;       
                case 5:
                    do_dump_ds_drp_stats();
                    break;
                default:
                    break;
            }
            break;
        case 102:
            do_clear_all_counter();         
            break;
        default:
            break;
    }
    return;

}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IPV6_ADDR_STR           "0123456789abcdefABCDEF:.%"
#define STATE_START               1
#define STATE_COLON               2
#define STATE_DOUBLE              3
#define STATE_ADDR                4
#define STATE_DOT                 5



extern int isdigit(int __c);

cs_int32 cmd_ipv4_match (char *str)
{
    char *sp = NULL;
    int dots = 0, nums = 0;
    char buf[4];

    memset(buf, 0, sizeof(buf));
    
    if (NULL == str)
        return CS_ERROR;

    for (;;)
    {
        memset (buf, 0, sizeof (buf));
        sp = str;
        
        while ('\0' != *str)
        {
            if ('.' == *str)
            {
                if (dots >= 3)
                    return CS_ERROR;

                if ('.' == *(str + 1))
                    return CS_ERROR;

                if ('\0' == *(str + 1))
                    return CS_ERROR;

                dots++;
                break;
            }
            
            if (!isdigit ((int) *str))
                return CS_ERROR;

            str++;
        }

        if (str - sp > 3)
            return CS_ERROR;

        strncpy (buf, sp, str - sp);
        
        if (cs_atoi (buf) > 255)
            return CS_ERROR;

        nums++;

        if ('\0' == *str)
            break;

        str++;
    }

    if (nums < 4)
        return CS_ERROR;

    return CS_OK;
}

cs_int32 cmd_ipv6_match (char *str)
{
    int state = STATE_START;
    int colons = 0, nums = 0, double_colon = 0;
    char *sp = NULL;

    if (NULL == str)
        return CS_ERROR;

    if (strspn (str, IPV6_ADDR_STR) != strlen (str))
        return CS_ERROR;

    while ('\0' != *str)
    {
        switch (state)
        {
            case STATE_START:
                if (':' == *str)
                {
                    if (':' != *(str + 1) && '\0' != *(str + 1))
                        return CS_ERROR;
                    
                    colons--;
                    state = STATE_COLON;
                }
                else
                {
                    sp = str;
                    state = STATE_ADDR;
                }

                continue;
            case STATE_COLON:
                colons++;
                
                if (':' == *(str + 1))
                    state = STATE_DOUBLE;
                else
                {
                    sp = str + 1;
                    state = STATE_ADDR;
                }
                break;
            case STATE_DOUBLE:
                if (double_colon)
                    return CS_ERROR;

                if (':' == *(str + 1))
                    return CS_ERROR;
                else
                {
                    if ('\0' != *(str + 1))
                        colons++;
                    
                    sp = str + 1;
                    state = STATE_ADDR;
                }

                double_colon++;
                nums++;
                break;
            case STATE_ADDR:
                if (':' == *(str + 1) || '\0' == *(str + 1))
                {
                    if (str - sp > 3)
                        return CS_ERROR;

                    nums++;
                    state = STATE_COLON;
                }
                
                if ('.' == *(str + 1))
                    state = STATE_DOT;
                
                break;
            case STATE_DOT:
                state = STATE_ADDR;
                break;
            default:
                break;
        }

        if (nums > 8)
            return CS_ERROR;

        if (colons > 7)
            return CS_ERROR;

        str++;
    }

    return CS_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_DOT_NUM       5
#define SEPERATE_CHAR_1 ':'
#define SEPERATE_CHAR_2 '.'

cs_status aal_cmd_mac_match(char *str)
{
    int token = 0;
    int mac = 0;
    char *sp;

    if (NULL == str)
        return CS_E_RESOURCE;

    sp = str;

    while (*sp != '\0') {
        if (((*sp <= 'F') && (*sp >= 'A')) || ((*sp <= 'f') && (*sp >= 'a')) || ((*sp <= '9') && (*sp >= '0'))) {
            mac ++;
            sp ++;

            if (mac > 2)
                return CS_E_PARAM;
        } else if ((SEPERATE_CHAR_1 == *sp) || (SEPERATE_CHAR_2 == *sp)) {
            sp ++;

            if (mac != 2)
                return CS_E_PARAM;

            mac = 0;
            token++;

            if (token > MAX_DOT_NUM)
                return CS_E_PARAM;
        } else
            return CS_E_PARAM;
    }

    if (token < MAX_DOT_NUM)
        return CS_E_PARAM;

    if (token == MAX_DOT_NUM && mac < 2)
        return CS_E_PARAM;

    return CS_E_OK;
}


typedef void (*aal_cmd_routine_t)(int, char **);


typedef struct {
    aal_cmd_routine_t cmd_routine;
    const char *cmd_str;
    const char *help_str;
} aal_cmd_reg_struct_t;

static aal_cmd_reg_struct_t aal_cmd_list[] = { 
    { aal_sp_cmd,     "sp", "Special packet operations"},
    { aal_fdb_cmd,   "fdb", "fdb sdk operations"},
    { aal_enc_cmd,   "enc", "enc  operations"},
    { aal_fec_cmd,   "fec", "fec  operations"},        
    { aal_port_cmd, "port", "port sdk operations"},
    { aal_vlan_cmd, "vlan", "vlan engine sdk operations"},
    { aal_bm_cmd,   "bm", "bm operations"},
    { aal_flow_cmd,   "flow", "flow operations"},
    { aal_pon_cmd,   "pon", "pon operations"},
    { aal_mpcp_cmd,   "mpcp", "mpcp operations"},
#ifdef HAVE_MPORTS
#ifdef HAVE_DEBUG
    { aal_cls_cmd,   "cls", "classifier operations"},  
#endif
#endif

#ifndef HAVE_MPORTS
    { aal_cls_cmd,   "cls", "classifier operations"},  
#ifdef HAVE_PTP
    { aal_ptp_cmd,   "ptp", "ptp  operations"},
#endif
#endif

};

/*
 * cmd proc
 *
 */
void aal_debug_help(void)
{
    int i;

    for (i = 0; i < sizeof(aal_cmd_list) / sizeof(aal_cmd_reg_struct_t); i++) {
        diag_printf("\t%-16s --- %s\n", aal_cmd_list[i].cmd_str, aal_cmd_list[i].help_str);
    }
}

void aal_debug_cmd(int argc, char **argv)
{
    int i;

    for (i = 0; i < sizeof(aal_cmd_list) / sizeof(aal_cmd_reg_struct_t); i++) {
        if (cmd_cmp(argv[1], aal_cmd_list[i].cmd_str)) {
            aal_cmd_list[i].cmd_routine(argc, argv);
            return;
        }
    }
}

int aal_debug_cmd_proc(int argc, char **argv)
{

    if (!cmd_cmp(argv[0], "aal"))
        return 0;

    if (argc == 1)
        aal_debug_help();
    else
        aal_debug_cmd(argc, argv);

    return 1;
}


