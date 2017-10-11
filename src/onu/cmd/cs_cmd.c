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

DEFINITIONS:  "DEVICE" means the Cortina Systems?LynxD SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems?SDK software.

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

Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/
/*
 * $Id: cs_cmd.c,v 1.1.8.3.8.1.36.1 2011/10/09 07:19:58 zsun Exp $
 */
#include "plat_common.h"
#include "cs_cmd.h"

#if 1//def HAVE_SDL_CMD

extern cs_status pon_cmd_self_reg(void);
extern cs_status show_cmd_self_reg(void);
extern cs_status ma_cmd_self_reg(void);
extern cs_status clear_cmd_self_reg(void);
extern cs_status vlan_cmd_self_reg(void);

const char *sal_cmd_result_str[SAL_CMD_PROMPT] = 
{
    "exec ok",
    "exec fail",
    "invalid cmd",
    "param error",
    "Not found",
    "no resource"
};
#if 0
const char *fe_id_str[] = 
{
    "GE",
    "PON",
    "MA"
};

const char *l4port_type_str[] = 
{
    "tcp_src",
    "udp_src",
    "tcp_dst",
    "udp_dst"
};
#endif
sal_cmd_cfg_list_t cmd_cfg_head;

sal_cmd_cfg_t *ma_cmd;

cs_status sal_cmd_reg(sal_cmd_cfg_t *node)
{
    sal_cmd_cfg_list_t *p = &cmd_cfg_head;
    sal_cmd_cfg_list_t *temp = NULL;

    while(p->next != NULL)
    {
        if(p->next->cfg == node)
            return CS_E_OK;
        p = p->next;
    }

    temp = (sal_cmd_cfg_list_t*)iros_malloc(IROS_MID_APP, sizeof(sal_cmd_cfg_list_t));
    if(NULL==temp) 
    {
        return CS_E_ERROR;
    }

    temp->cfg = node;
    temp->next = NULL;
    p->next = temp;

    return CS_E_OK;
}

cs_status sal_cmd_unreg(sal_cmd_cfg_t *node)
{
    sal_cmd_cfg_list_t *p = &cmd_cfg_head;
    sal_cmd_cfg_list_t *temp = NULL;

    while((p->next!=NULL) && (p->next->cfg!=node))
        p = p->next;

    if(NULL==p->next) return CS_E_ERROR;

    temp = p->next;
    p->next = p->next->next;

    iros_free(temp);

    return CS_E_OK;
}

void __print_cmd(void)
{
    sal_cmd_cfg_list_t *p;
    sal_cmd_show("ONU commands are:\n");
    for(p = &cmd_cfg_head; p->next!=NULL; p=p->next)
    {
        sal_cmd_cfg_t *cfg = p->next->cfg;
        sal_cmd_show("  %-10s - %s\n", cfg->module, cfg->desc);
    }    
}

void __print_help(sal_cmd_cfg_t *cfg)
{
    int cmd_id;
    for(cmd_id=0; cmd_id<cfg->cmd_num; cmd_id++)
    {
        sal_cmd_show("%s %-10s -- %s\n", cfg->module, cfg->cmd_cfg[cmd_id].cmd_str, cfg->cmd_cfg[cmd_id].help_str);
    }
}

void __print_usage(sal_cmd_cfg_t *cfg, char* cmd, cs_uint8 cmd_id)
{
    cs_uint16 line;
    if(cfg->cmd_cfg[cmd_id].usage_line == 0)
        return;
    sal_cmd_show("\n%s %s: %s\n", cfg->module, cmd, cfg->cmd_cfg[cmd_id].help_str);
    sal_cmd_show("usage: \n");
    for(line = 0; line < cfg->cmd_cfg[cmd_id].usage_line; line++) {
        sal_cmd_show("%s\n", cfg->cmd_cfg[cmd_id].usage[line]);
    }
}

sal_cmd_result_t cs_status_2_cmd_rc_map(cs_status rt)
{
    switch(rt)
    {
    case CS_E_OK:
        return SAL_CMD_OK;
    case CS_E_RESOURCE:
        return SAL_CMD_NO_RESOURCE;
    case CS_E_PARAM:
        return SAL_CMD_INVALID_PARAM;
    case CS_E_NOT_FOUND:
        return SAL_CMD_NOT_FOUND;
    default:
        break;
    }

    return SAL_CMD_FAIL;
}

sal_cmd_result_t sal_api_2_cmd_rc_map(epon_return_code_t rt)
{
    switch(rt)
    {
    case EPON_RETURN_SUCCESS:
        return SAL_CMD_OK;
    case EPON_RETURN_INV_PARAM:
        return SAL_CMD_INVALID_PARAM;
    case EPON_RETURN_NOT_FOUND:
        return SAL_CMD_NOT_FOUND;
    case EPON_RETURN_NO_RESOURCE:
        return SAL_CMD_NO_RESOURCE;
    default:
        break;
    }

    return SAL_CMD_FAIL;
}

#endif //#ifdef HAVE_SDL_CMD

int sal_cmd_proc(int argc, char **argv)
{
#ifdef HAVE_SDL_CMD
    sal_cmd_cfg_list_t *p;
    int cmd_id;
#if 0
    if(argc == 1 &&
        (cmd_cmp(argv[0], "?") ||
         cmd_cmp(argv[0], "help") || 
         cmd_cmp(argv[0], "h")) ) {
        for(p = &cmd_cfg_head; p->next!=NULL; p=p->next)
        {
            sal_cmd_cfg_t *cfg = p->next->cfg;
            sal_cmd_show("----%10s\n", cfg->module);
        }
        return 1;
    }
#endif
    for(p = &cmd_cfg_head; p->next!=NULL; p=p->next)
    {
        sal_cmd_cfg_t *cfg = p->next->cfg;

        if(!cmd_cmp(argv[0], cfg->module)) {
            continue;
        }
    
        if(argc == 1) {
             sal_cmd_show("\n");
            __print_help(cfg);
            return 1;
        }
        else {
            sal_cmd_result_t rc = SAL_CMD_INVALID_CMD;
            
            for(cmd_id=0; cmd_id<cfg->cmd_num; cmd_id++)
            {
                if(cmd_cmp(argv[1], cfg->cmd_cfg[cmd_id].cmd_str))
                {
#if 0
                    if((argc == 3) && cmd_cmp(argv[2], "?")) { /* usage */
                        cs_uint16 line;
                        sal_cmd_show("%s %s: %s\n", cfg->module, argv[1], cfg->cmd_cfg[cmd_id].help_str);
                        sal_cmd_show("usage: \n");
                        for(line = 0; line < cfg->cmd_cfg[cmd_id].usage_line; line++) {
                            sal_cmd_show("%s\n", cfg->cmd_cfg[cmd_id].usage[line]);
                        }
                        rc = SAL_CMD_PROMPT;
                    }
                    else {
                        rc = cfg->cmd_cfg[cmd_id].cmd_routine(argc, argv);
                    }
#endif
                    rc = cfg->cmd_cfg[cmd_id].cmd_routine(argc, argv);
                    if(rc != SAL_CMD_OK)
                        sal_cmd_show(sal_cmd_result_str[rc]);
                    if(SAL_CMD_OK != rc) __print_usage(cfg, argv[1], cmd_id);
                    return 1;
                }
            }
            __print_help(cfg);
            return 1;
        }

    }
#endif //#ifdef HAVE_SDL_CMD
    //__print_cmd();
    return CS_E_OK;
}

#if 0
cs_status sdl_cmd_init(void)
{
    cs_status rt = CS_E_OK;

    rt |= show_cmd_self_reg();
    rt |= pon_cmd_self_reg();
    rt |= clear_cmd_self_reg();
    rt |= ma_cmd_self_reg();
#ifdef HAVE_SDL_CMD
    rt |= vlan_cmd_self_reg();
#endif

    return rt;
}
#endif

