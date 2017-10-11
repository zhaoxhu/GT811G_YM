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
#include "cs_utils.h"
#include "cs_types.h"
#include "cli.h"
#include "aal.h"
#include "aal_util.h"
#include "aal_l2.h"


/*
 * externs
 */
extern void sw_vlan_cmd(int argc, char **argv);
extern void sw_fdb_cmd(int argc, char **argv);
extern void sw_l2_cmd(int argc, char **argv);
extern void sw_igmp_cmd(int argc, char **argv);
extern void sw_trunk_cmd(int argc, char **argv);
extern void rtk_port_cmd(int argc, char **argv);
extern void rtk_led_cmd(int argc, char **argv);
extern void rtk_int_cmd(int argc, char **argv);
extern void rtk_qos_cmd(int argc, char **argv);
extern void rtk_glb_cmd(int argc, char **argv);
extern void rtk_acl_cmd(int argc, char **argv);
cs_status aal_cmd_mac_match(char *str);


///////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_DOT_NUM       5
#define SEPERATE_CHAR_1 ':'
#define SEPERATE_CHAR_2 '.'


typedef void (*aal_cmd_routine_t)(int, char **);


typedef struct {
    aal_cmd_routine_t cmd_routine;
    const char *cmd_str;
    const char *help_str;
} aal_cmd_reg_struct_t;

static aal_cmd_reg_struct_t sw_cmd_list[] = { 
    { sw_vlan_cmd,   "vlan",  "vlan   operations"},
    { sw_l2_cmd,     "l2",    "layer2 operations"},
    { sw_fdb_cmd,    "fdb",   "fdb    operations"},
    { sw_igmp_cmd,   "igmp",  "igmp   operations"},
    { sw_trunk_cmd,  "trunk", "trunk  operations"},
    { rtk_port_cmd,  "port",  "port  operations"},
    { rtk_led_cmd,  "led",  "led  operations"},    
    { rtk_int_cmd,  "int",  "interrupt  operations"},
    { rtk_qos_cmd, "qos", "qos operations"},
    { rtk_glb_cmd, "glb", "global operations"},
    { rtk_acl_cmd, "acl", "acl operations"},
};

/*
 * cmd proc
 *
 */
void sw_debug_help()
{
    int i;

    for (i = 0; i < sizeof(sw_cmd_list) / sizeof(aal_cmd_reg_struct_t); i++) {
        diag_printf("\t%-16s --- %s\n", sw_cmd_list[i].cmd_str, sw_cmd_list[i].help_str);
    }
}

void sw_debug_cmd(int argc, char **argv)
{
    int i;

    for (i = 0; i < sizeof(sw_cmd_list) / sizeof(aal_cmd_reg_struct_t); i++) {
        if (cmd_cmp(argv[1], sw_cmd_list[i].cmd_str)) {
            sw_cmd_list[i].cmd_routine(argc, argv);
            return;
        }
    }
}

int sw_debug_cmd_proc(int argc, char **argv)
{

    if (!cmd_cmp(argv[0], "sw"))
        return 0;

    if (argc == 1)
        sw_debug_help();
    else if (argc == 2)
    {
        if (argv[1][0] == '?')
            sw_debug_help();
        else
            sw_debug_cmd(argc, argv);
    }
    else
        sw_debug_cmd(argc, argv);

    return 1;
}


