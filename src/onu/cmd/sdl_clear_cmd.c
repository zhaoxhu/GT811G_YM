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

DEFINITIONS:  "DEVICE" means the Cortina Systems?Daytona SDK product.
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
 * $Id: sdl_clear_cmd.c,v 1.1.2.12.8.5 2011/08/11 07:20:08 duchen Exp $
 */

#include "cs_cmd.h"
#ifdef HAVE_SDL_CMD
#include "sdl_ma.h"
#include "sdl_pon.h"
#include "sdl_port.h"
#include "aal_fec.h"
#include "aal_ma.h"

extern __cpu_rx_stats_t __cpu_rx_stats;

extern void sdl_clear_all_counter(void);

sal_cmd_result_t clear_cmd_pon(int argc, char **argv);
sal_cmd_result_t clear_cmd_ma(int argc, char **argv);
sal_cmd_result_t clear_cmd_uni(int argc, char **argv);
sal_cmd_result_t clear_cmd_fec(int argc, char **argv);
sal_cmd_result_t clear_cmd_all(int argc, char **argv);


#ifdef HAVE_SDL_CMD_HELP
static const char *clear_cmd_usage_fec[] = 
{
    "clear fec",
};

static const char *clear_cmd_usage_pon[] = 
{
    "clear pon",
};

static const char *clear_cmd_usage_ma[] = 
{
    "clear ma",
};

static const char *clear_cmd_usage_uni[] = 
{
    "clear uni",
};
#ifdef HAVE_SDL_CMD

static const char *clear_cmd_usage_all[] = 
{
    "clear all",
};
#endif
#endif

static sal_cmd_reg_struct_t clear_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    {clear_cmd_pon, "pon", "clear pon port stats", clear_cmd_usage_pon, sizeof(clear_cmd_usage_pon)/4},
    {clear_cmd_uni, "uni", "clear uni port stats", clear_cmd_usage_uni, sizeof(clear_cmd_usage_uni)/4},
    {clear_cmd_fec, "fec", "clear fec stats", clear_cmd_usage_fec, sizeof(clear_cmd_usage_fec)/4},
    {clear_cmd_ma,  "ma", "clear ma stats", clear_cmd_usage_ma, sizeof(clear_cmd_usage_ma)/4},    
#ifdef HAVE_SDL_CMD

    {clear_cmd_all, "all", "clear all stats", clear_cmd_usage_all, sizeof(clear_cmd_usage_all)/4},
#endif
#else
    {clear_cmd_pon, "pon"},// "clear pon port stats"},// clear_cmd_usage_pon, sizeof(clear_cmd_usage_pon)/4},
    {clear_cmd_uni, "uni"},// "clear uni port stats"},// clear_cmd_usage_uni, sizeof(clear_cmd_usage_uni)/4},
    {clear_cmd_fec, "fec"},// "clear fec stats"},// clear_cmd_usage_fec, sizeof(clear_cmd_usage_fec)/4},
    {clear_cmd_ma,  "ma"},// "clear ma stats"},// clear_cmd_usage_ma, sizeof(clear_cmd_usage_ma)/4},    
#ifdef HAVE_SDL_CMD

    {clear_cmd_all, "all"},// "clear all stats"},// clear_cmd_usage_all, sizeof(clear_cmd_usage_all)/4},
#endif
#endif
};

sal_cmd_cfg_t clear_cmd_cfg = {"clear", "clear various statistics", clear_cmd_list, sizeof(clear_cmd_list)/sizeof(sal_cmd_reg_struct_t)};

sal_cmd_result_t clear_cmd_fec(int argc, char **argv)
{
    cs_status rt = CS_E_OK;
    cs_aal_fec_ext_cfg_t cfg;
    cs_aal_fec_ext_msk_t msk;

    msk.u32 = 0;
    msk.s.cnt_rd_clr_mode = 1;
    cfg.cnt_rd_clr_mode = 1;

    if(argc!=2) return SAL_CMD_INVALID_PARAM;

    rt = aal_fec_ext_cfg_set(msk, &cfg);

    return cs_status_2_cmd_rc_map(rt);
}

sal_cmd_result_t clear_cmd_pon(int argc, char **argv)
{
    cs_status rt = CS_E_OK;
    cs_callback_context_t context;

    if(argc!=2) return SAL_CMD_INVALID_PARAM;

    rt = epon_request_onu_pon_stats_clr(context, 0, CS_PON_PORT_ID);

    return cs_status_2_cmd_rc_map(rt);
}

sal_cmd_result_t clear_cmd_ma(int argc, char **argv)
{
    cs_status rt = CS_E_OK;

    if(argc!=2) return SAL_CMD_INVALID_PARAM;

    rt = aal_ma_mib_reset();
    memset(&__cpu_rx_stats, 0, sizeof(__cpu_rx_stats_t));

    return cs_status_2_cmd_rc_map(rt);
}

sal_cmd_result_t clear_cmd_uni(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  status = 0;
    cs_port_id_t  portid;
    portid = (cs_port_id_t)iros_strtol(argv[2]);

    if(argc != 3) {
        return SAL_CMD_INVALID_PARAM;
    }

    status = epon_request_onu_port_stats_clr(context, 0, 0, portid);
    if(status != CS_E_OK){
        diag_printf("epon_request_onu_port_stats_clr failed \n");    
        return cs_status_2_cmd_rc_map(status);       
    }

    return cs_status_2_cmd_rc_map(status);
}

sal_cmd_result_t clear_cmd_all(int argc, char **argv)
{
    cs_status  status = 0;

    if(argc != 2) {
        return SAL_CMD_INVALID_PARAM;
    }

    sdl_clear_all_counter();

    return cs_status_2_cmd_rc_map(status);
}

#endif //#ifdef HAVE_SDL_CMD

cs_status clear_cmd_self_reg(void)
{
        cs_status ret = CS_E_OK;
#ifdef HAVE_SDL_CMD
    
        ret = sal_cmd_reg(&clear_cmd_cfg);
#endif
        return ret;
}


