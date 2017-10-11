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

Copyright (c) 2011 by Cortina Systems Incorporated
****************************************************************************/
#include "plat_common.h"
#include "sdl_api.h"
#include "app_timer.h"
#include "port_stats.h"
#include "oam_event.h"

/* for pon stats, hold 64 bit value  */
static oam_pon_stats_t g_pon_stats;
/* the last time get from sdl, need compare it with next time get
 * to make sure the next get has run over 32 bit*/
static cs_pon_stats_t  g_pon_stats_last;

/* index 0 to store the inter Ge port
 * for one port, the index 0 will not used */
static oam_port_uni_stats_t *g_uni_stats = NULL;
/* the last time get from sdl, need compare it with next time get
 * to make sure the next get has run over 32 bit*/
static cs_sdl_port_uni_stats_t *g_uni_stats_last = NULL;

/*record uni status change times*/
static cs_uint32 *g_uni_status_change_times = NULL;

static cs_uint32 g_app_pon_stats_mutex = 0;
static cs_uint32 g_app_uni_stats_mutex = 0;
static cs_uint32 g_app_uni_status_change_mutex = 0;

/* Enable or disable the polling */
static  cs_boolean g_app_stats_polling_enable = TRUE;

/* Timer data */
static app_timer_msg_t g_app_stats_timer;

/* max uni port number */
static cs_uint8 g_app_max_uni_port = 1;

#ifdef HAVE_STATISTICS_NNI
static oam_port_uni_stats_t	g_nni_stats;
static oam_port_uni_stats_t	g_nni_stats_last;
static cs_uint32 g_app_nni_stats_mutex = 0;
#endif

/* use the current value and last value to make sure the value not over 32 bit */
#define COUNT_STATS(cur, last) \
        (cur) >= (last)? ( (cur) - (last) ) : (0xffffffff - (last) + (cur) + 1)

extern cs_int32 onu_evt_reg(cs_int32 evt_type , void *func , void *cookie);

cs_uint32 oam_count_stats_offset(cs_uint32 cur, cs_uint32 last)
{
    if(cur >= last)
    {
        return cur - last;
    }
    else
    {
        if((last - cur) < 10000)
        {
            /*consider it's not a normal around*/
            return 0;
        }
        else
        {
            return (0xffffffff - last + cur + 1);
        }
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  app_pon_stats_count                                       */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static void app_pon_stats_count()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* Polling the pon port stats                                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_callback_context_t     context;
    cs_pon_stats_t         pon_stats;
    cs_status ret = CS_E_OK;
    
    ret = epon_request_onu_pon_stats_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
                                                        FALSE, &pon_stats);
    if(ret != CS_E_OK){
        PORT_STATS_LOG("epon_request_onu_pon_stats_get return failed, ret = %d \n",ret);
        return;
    }
    
    cs_mutex_lock(g_app_pon_stats_mutex);
    g_pon_stats.pon_byte_cnt            +=  COUNT_STATS( pon_stats.pon_byte_cnt            ,g_pon_stats_last.pon_byte_cnt           );         
    g_pon_stats.pon_tx_byte_cnt         +=  COUNT_STATS( pon_stats.pon_tx_byte_cnt         ,g_pon_stats_last.pon_tx_byte_cnt        );
    g_pon_stats.pon_frame_cnt           +=  COUNT_STATS( pon_stats.pon_frame_cnt           ,g_pon_stats_last.pon_frame_cnt          );
    g_pon_stats.pon_txframe_cnt         +=  COUNT_STATS( pon_stats.pon_txframe_cnt         ,g_pon_stats_last.pon_txframe_cnt        );
    g_pon_stats.pon_crcerr_cnt          +=  COUNT_STATS( pon_stats.pon_crcerr_cnt          ,g_pon_stats_last.pon_crcerr_cnt         );
    g_pon_stats.pon_mcframe_cnt         +=  COUNT_STATS( pon_stats.pon_mcframe_cnt         ,g_pon_stats_last.pon_mcframe_cnt        );
    g_pon_stats.pon_bcframe_cnt         +=  COUNT_STATS( pon_stats.pon_bcframe_cnt         ,g_pon_stats_last.pon_bcframe_cnt        );
    g_pon_stats.pon_txmcframe_cnt       +=  COUNT_STATS( pon_stats.pon_txmcframe_cnt       ,g_pon_stats_last.pon_txmcframe_cnt      );
    g_pon_stats.pon_txbcframe_cnt       +=  COUNT_STATS( pon_stats.pon_txbcframe_cnt       ,g_pon_stats_last.pon_txbcframe_cnt      );
    g_pon_stats.pon_ctrlframe_cnt       +=  COUNT_STATS( pon_stats.pon_ctrlframe_cnt       ,g_pon_stats_last.pon_ctrlframe_cnt      );
    g_pon_stats.pon_txctrlframe_cnt     +=  COUNT_STATS( pon_stats.pon_txctrlframe_cnt     ,g_pon_stats_last.pon_txctrlframe_cnt    );
    g_pon_stats.pon_pauseframe_cnt      +=  COUNT_STATS( pon_stats.pon_pauseframe_cnt      ,g_pon_stats_last.pon_pauseframe_cnt     );
    g_pon_stats.pon_unknownop_cnt       +=  COUNT_STATS( pon_stats.pon_unknownop_cnt       ,g_pon_stats_last.pon_unknownop_cnt      );
    g_pon_stats.pon_runt_cnt            +=  COUNT_STATS( pon_stats.pon_runt_cnt            ,g_pon_stats_last.pon_runt_cnt           );
    g_pon_stats.pon_oversize_cnt        +=  COUNT_STATS( pon_stats.pon_oversize_cnt        ,g_pon_stats_last.pon_oversize_cnt       );
    g_pon_stats.pon_rmon64_cnt          +=  COUNT_STATS( pon_stats.pon_rmon64_cnt          ,g_pon_stats_last.pon_rmon64_cnt         );
    g_pon_stats.pon_rmon65_127_cnt      +=  COUNT_STATS( pon_stats.pon_rmon65_127_cnt      ,g_pon_stats_last.pon_rmon65_127_cnt     );
    g_pon_stats.pon_rmon128_255_cnt     +=  COUNT_STATS( pon_stats.pon_rmon128_255_cnt     ,g_pon_stats_last.pon_rmon128_255_cnt    );
    g_pon_stats.pon_rmon256_511_cnt     +=  COUNT_STATS( pon_stats.pon_rmon256_511_cnt     ,g_pon_stats_last.pon_rmon256_511_cnt    );
    g_pon_stats.pon_rmon512_1023_cnt    +=  COUNT_STATS( pon_stats.pon_rmon512_1023_cnt    ,g_pon_stats_last.pon_rmon512_1023_cnt   );
    g_pon_stats.pon_rmon1024_1518_cnt   +=  COUNT_STATS( pon_stats.pon_rmon1024_1518_cnt   ,g_pon_stats_last.pon_rmon1024_1518_cnt  );
    g_pon_stats.pon_txrmon64_cnt        +=  COUNT_STATS( pon_stats.pon_txrmon64_cnt        ,g_pon_stats_last.pon_txrmon64_cnt       );
    g_pon_stats.pon_txrmon65_127_cnt    +=  COUNT_STATS( pon_stats.pon_txrmon65_127_cnt    ,g_pon_stats_last.pon_txrmon65_127_cnt   );
    g_pon_stats.pon_txrmon128_255_cnt   +=  COUNT_STATS( pon_stats.pon_txrmon128_255_cnt   ,g_pon_stats_last.pon_txrmon128_255_cnt  );
    g_pon_stats.pon_txrmon256_511_cnt   +=  COUNT_STATS( pon_stats.pon_txrmon256_511_cnt   ,g_pon_stats_last.pon_txrmon256_511_cnt  );
    g_pon_stats.pon_txrmon512_1023_cnt  +=  COUNT_STATS( pon_stats.pon_txrmon512_1023_cnt  ,g_pon_stats_last.pon_txrmon512_1023_cnt );
    g_pon_stats.pon_txrmon1024_1518_cnt +=  COUNT_STATS( pon_stats.pon_txrmon1024_1518_cnt ,g_pon_stats_last.pon_txrmon1024_1518_cnt);
    g_pon_stats.pon_mpcp_uc_rev_cnt     +=  COUNT_STATS( pon_stats.pon_mpcp_uc_rev_cnt     ,g_pon_stats_last.pon_mpcp_uc_rev_cnt    );
    g_pon_stats.pon_mpcp_uc_reg_ack     +=  COUNT_STATS( pon_stats.pon_mpcp_uc_reg_ack     ,g_pon_stats_last.pon_mpcp_uc_reg_ack    );
    g_pon_stats.pon_mpcp_uc_reg_nack    +=  COUNT_STATS( pon_stats.pon_mpcp_uc_reg_nack    ,g_pon_stats_last.pon_mpcp_uc_reg_nack   );
    g_pon_stats.pon_mpcp_uc_reg_dereg   +=  COUNT_STATS( pon_stats.pon_mpcp_uc_reg_dereg   ,g_pon_stats_last.pon_mpcp_uc_reg_dereg  );
    g_pon_stats.pon_mpcp_uc_reg_rereg   +=  COUNT_STATS( pon_stats.pon_mpcp_uc_reg_rereg   ,g_pon_stats_last.pon_mpcp_uc_reg_rereg  );
    g_pon_stats.pon_mpcp_uc_reg_reg_rev +=  COUNT_STATS( pon_stats.pon_mpcp_uc_reg_reg_rev ,g_pon_stats_last.pon_mpcp_uc_reg_reg_rev);
    g_pon_stats.pon_mpcp_uc_gat_rev     +=  COUNT_STATS( pon_stats.pon_mpcp_uc_gat_rev     ,g_pon_stats_last.pon_mpcp_uc_gat_rev    );
    g_pon_stats.pon_mpcp_uc_gat_norm    +=  COUNT_STATS( pon_stats.pon_mpcp_uc_gat_norm    ,g_pon_stats_last.pon_mpcp_uc_gat_norm   );
    g_pon_stats.pon_mpcp_uc_gat_frpt    +=  COUNT_STATS( pon_stats.pon_mpcp_uc_gat_frpt    ,g_pon_stats_last.pon_mpcp_uc_gat_frpt   );
    g_pon_stats.pon_mpcp_uc_gat_udis    +=  COUNT_STATS( pon_stats.pon_mpcp_uc_gat_udis    ,g_pon_stats_last.pon_mpcp_uc_gat_udis   );
    g_pon_stats.pon_mpcp_uc_gat_bdis    +=  COUNT_STATS( pon_stats.pon_mpcp_uc_gat_bdis    ,g_pon_stats_last.pon_mpcp_uc_gat_bdis   );
    g_pon_stats.pon_mpcp_mc_rev_cnt     +=  COUNT_STATS( pon_stats.pon_mpcp_mc_rev_cnt     ,g_pon_stats_last.pon_mpcp_mc_rev_cnt    );
    g_pon_stats.pon_mpcp_mc_reg_ack     +=  COUNT_STATS( pon_stats.pon_mpcp_mc_reg_ack     ,g_pon_stats_last.pon_mpcp_mc_reg_ack    );
    g_pon_stats.pon_mpcp_mc_reg_nack    +=  COUNT_STATS( pon_stats.pon_mpcp_mc_reg_nack    ,g_pon_stats_last.pon_mpcp_mc_reg_nack   );
    g_pon_stats.pon_mpcp_mc_reg_dereg   +=  COUNT_STATS( pon_stats.pon_mpcp_mc_reg_dereg   ,g_pon_stats_last.pon_mpcp_mc_reg_dereg  );
    g_pon_stats.pon_mpcp_mc_reg_rereg   +=  COUNT_STATS( pon_stats.pon_mpcp_mc_reg_rereg   ,g_pon_stats_last.pon_mpcp_mc_reg_rereg  );
    g_pon_stats.pon_mpcp_mc_reg_reg_rev +=  COUNT_STATS( pon_stats.pon_mpcp_mc_reg_reg_rev ,g_pon_stats_last.pon_mpcp_mc_reg_reg_rev);
    g_pon_stats.pon_mpcp_mc_gat_rev     +=  COUNT_STATS( pon_stats.pon_mpcp_mc_gat_rev     ,g_pon_stats_last.pon_mpcp_mc_gat_rev    );
    g_pon_stats.pon_mpcp_mc_gat_norm    +=  COUNT_STATS( pon_stats.pon_mpcp_mc_gat_norm    ,g_pon_stats_last.pon_mpcp_mc_gat_norm   );
    g_pon_stats.pon_mpcp_mc_gat_frpt    +=  COUNT_STATS( pon_stats.pon_mpcp_mc_gat_frpt    ,g_pon_stats_last.pon_mpcp_mc_gat_frpt   );
    g_pon_stats.pon_mpcp_mc_gat_udis    +=  COUNT_STATS( pon_stats.pon_mpcp_mc_gat_udis    ,g_pon_stats_last.pon_mpcp_mc_gat_udis   );
    g_pon_stats.pon_mpcp_mc_gat_bdis    +=  COUNT_STATS( pon_stats.pon_mpcp_mc_gat_bdis    ,g_pon_stats_last.pon_mpcp_mc_gat_bdis   );
    g_pon_stats.pon_mpcp_bc_rev_cnt     +=  COUNT_STATS( pon_stats.pon_mpcp_bc_rev_cnt     ,g_pon_stats_last.pon_mpcp_bc_rev_cnt    );
    g_pon_stats.pon_mpcp_bc_reg_ack     +=  COUNT_STATS( pon_stats.pon_mpcp_bc_reg_ack     ,g_pon_stats_last.pon_mpcp_bc_reg_ack    );
    g_pon_stats.pon_mpcp_bc_reg_nack    +=  COUNT_STATS( pon_stats.pon_mpcp_bc_reg_nack    ,g_pon_stats_last.pon_mpcp_bc_reg_nack   );
    g_pon_stats.pon_mpcp_bc_reg_dereg   +=  COUNT_STATS( pon_stats.pon_mpcp_bc_reg_dereg   ,g_pon_stats_last.pon_mpcp_bc_reg_dereg  );
    g_pon_stats.pon_mpcp_bc_reg_rereg   +=  COUNT_STATS( pon_stats.pon_mpcp_bc_reg_rereg   ,g_pon_stats_last.pon_mpcp_bc_reg_rereg  );
    g_pon_stats.pon_mpcp_bc_reg_reg_rev +=  COUNT_STATS( pon_stats.pon_mpcp_bc_reg_reg_rev ,g_pon_stats_last.pon_mpcp_bc_reg_reg_rev);
    g_pon_stats.pon_mpcp_bc_gat_rev     +=  COUNT_STATS( pon_stats.pon_mpcp_bc_gat_rev     ,g_pon_stats_last.pon_mpcp_bc_gat_rev    );
    g_pon_stats.pon_mpcp_bc_gat_norm    +=  COUNT_STATS( pon_stats.pon_mpcp_bc_gat_norm    ,g_pon_stats_last.pon_mpcp_bc_gat_norm   );
    g_pon_stats.pon_mpcp_bc_gat_frpt    +=  COUNT_STATS( pon_stats.pon_mpcp_bc_gat_frpt    ,g_pon_stats_last.pon_mpcp_bc_gat_frpt   );
    g_pon_stats.pon_mpcp_bc_gat_udis    +=  COUNT_STATS( pon_stats.pon_mpcp_bc_gat_udis    ,g_pon_stats_last.pon_mpcp_bc_gat_udis   );
    g_pon_stats.pon_mpcp_bc_gat_bdis    +=  COUNT_STATS( pon_stats.pon_mpcp_bc_gat_bdis    ,g_pon_stats_last.pon_mpcp_bc_gat_bdis   );
    g_pon_stats.pon_mpcp_tx_req_dereg   +=  COUNT_STATS( pon_stats.pon_mpcp_tx_req_dereg   ,g_pon_stats_last.pon_mpcp_tx_req_dereg  );
    g_pon_stats.pon_mpcp_tx_req_reg     +=  COUNT_STATS( pon_stats.pon_mpcp_tx_req_reg     ,g_pon_stats_last.pon_mpcp_tx_req_reg    );
    g_pon_stats.pon_mpcp_tx_ack_ack     +=  COUNT_STATS( pon_stats.pon_mpcp_tx_ack_ack     ,g_pon_stats_last.pon_mpcp_tx_ack_ack    );
    g_pon_stats.pon_mpcp_tx_ack_nack    +=  COUNT_STATS( pon_stats.pon_mpcp_tx_ack_nack    ,g_pon_stats_last.pon_mpcp_tx_ack_nack   );
    g_pon_stats.pon_mpcp_tx_rpt         +=  COUNT_STATS( pon_stats.pon_mpcp_tx_rpt         ,g_pon_stats_last.pon_mpcp_tx_rpt        );
    g_pon_stats.pon_mpcp_tx_oam         +=  COUNT_STATS( pon_stats.pon_mpcp_tx_oam         ,g_pon_stats_last.pon_mpcp_tx_oam        );
    g_pon_stats.pon_mpcp_da_mc_nslf     +=  COUNT_STATS( pon_stats.pon_mpcp_da_mc_nslf     ,g_pon_stats_last.pon_mpcp_da_mc_nslf    );
    g_pon_stats.pon_mpcp_da_mc_slf      +=  COUNT_STATS( pon_stats.pon_mpcp_da_mc_slf      ,g_pon_stats_last.pon_mpcp_da_mc_slf     );
    g_pon_stats.pon_mpcp_da_uc          +=  COUNT_STATS( pon_stats.pon_mpcp_da_uc          ,g_pon_stats_last.pon_mpcp_da_uc         );
    g_pon_stats.pon_framecnt            +=  COUNT_STATS( pon_stats.pon_framecnt            ,g_pon_stats_last.pon_framecnt           );
    g_pon_stats.pon_txoversizecnt       +=  COUNT_STATS( pon_stats.pon_txoversizecnt       ,g_pon_stats_last.pon_txoversizecnt      );
    g_pon_stats.pon_rx1519_maxcnt       +=  COUNT_STATS( pon_stats.pon_rx1519_maxcnt       ,g_pon_stats_last.pon_rx1519_maxcnt      );
    g_pon_stats.pon_tx1519_maxcnt       +=  COUNT_STATS( pon_stats.pon_tx1519_maxcnt       ,g_pon_stats_last.pon_tx1519_maxcnt      );
    g_pon_stats.pon_txframecnt          +=  COUNT_STATS( pon_stats.pon_txframecnt          ,g_pon_stats_last.pon_txframecnt         );
    g_pon_stats.pon_txpausecnt          +=  COUNT_STATS( pon_stats.pon_txpausecnt          ,g_pon_stats_last.pon_txpausecnt         );
    g_pon_stats.pon_undersizecnt        +=  COUNT_STATS( pon_stats.pon_undersizecnt        ,g_pon_stats_last.pon_undersizecnt       );
    g_pon_stats.pon_jabbercnt           +=  COUNT_STATS( pon_stats.pon_jabbercnt           ,g_pon_stats_last.pon_jabbercnt          );
    g_pon_stats.pon_extensioncnt        +=  COUNT_STATS( pon_stats.pon_extensioncnt        ,g_pon_stats_last.pon_extensioncnt       );
    g_pon_stats.pon_txextensioncnt      +=  COUNT_STATS( pon_stats.pon_txextensioncnt      ,g_pon_stats_last.pon_txextensioncnt     );
    g_pon_stats.pon_err_symbol          +=  COUNT_STATS( pon_stats.pon_err_symbol          ,g_pon_stats_last.pon_err_symbol         );
    g_pon_stats.pon_outofsync           +=  COUNT_STATS( pon_stats.pon_outofsync           ,g_pon_stats_last.pon_outofsync          );
    g_pon_stats.pon_sld_err             +=  COUNT_STATS( pon_stats.pon_sld_err             ,g_pon_stats_last.pon_sld_err            );
    g_pon_stats.pon_crc8_err            +=  COUNT_STATS( pon_stats.pon_crc8_err            ,g_pon_stats_last.pon_crc8_err           );
    g_pon_stats.pon_mac_drop            +=  COUNT_STATS( pon_stats.pon_mac_drop            ,g_pon_stats_last.pon_mac_drop           );
    g_pon_stats.pon_ipg_drop            +=  COUNT_STATS( pon_stats.pon_ipg_drop            ,g_pon_stats_last.pon_ipg_drop           );
    g_pon_stats.pon_drain               +=  COUNT_STATS( pon_stats.pon_drain               ,g_pon_stats_last.pon_drain              );
    g_pon_stats.pon_tx_crc_err          +=  COUNT_STATS( pon_stats.pon_tx_crc_err          ,g_pon_stats_last.pon_tx_crc_err         );

    memcpy(&g_pon_stats_last,&pon_stats,sizeof(pon_stats));

    cs_mutex_unlock(g_app_pon_stats_mutex);

}

/*****************************************************************************/
/* $rtn_hdr_start  app_uni_stats_count                                       */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static void app_uni_stats_count()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* Polling the Uni ports stats                                               */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_callback_context_t     context;
    cs_sdl_port_uni_stats_t uni_stats;
    cs_status ret = CS_E_OK;
    cs_uint8 i = 0; 
    cs_port_id_t port = 0;
    cs_uint32 offset = 0;

    for(i = 0; i < (g_app_max_uni_port + 1); i++){

        memset(&uni_stats,0x00,sizeof(uni_stats));

        if(i == 0){
            if(g_app_max_uni_port == 1)
            {
                /* for one port onu, the no internal ge port, skip it*/
                continue;
            }
            else
            {
                /* internal PON chip GE port */
                port = CS_DOWNLINK_PORT;
            }
        }else{
            port = i;
        }

        ret = epon_request_onu_port_stats_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
                port,FALSE, &uni_stats);
        if(ret != CS_E_OK){
            PORT_STATS_LOG_INFO("epon_request_onu_port_stats_get return failed, port = %d ret = %d \n",port,ret);
            return ;
        }

        cs_mutex_lock(g_app_uni_stats_mutex);
        g_uni_stats[i].rxucfrm_cnt               += COUNT_STATS( uni_stats.rxucfrm_cnt             ,g_uni_stats_last[i].rxucfrm_cnt            )  ;
     
        /*special handle for rx mc counter*/
        offset = oam_count_stats_offset( uni_stats.rxmcfrm_cnt , g_uni_stats_last[i].rxmcfrm_cnt);
        g_uni_stats[i].rxmcfrm_cnt               +=  offset;
        if(offset == 0)
        {
            uni_stats.rxmcfrm_cnt = g_uni_stats_last[i].rxmcfrm_cnt;
        }

        /*special handle for rx bc counter*/
        offset = oam_count_stats_offset( uni_stats.rxbcfrm_cnt , g_uni_stats_last[i].rxbcfrm_cnt);
        g_uni_stats[i].rxbcfrm_cnt               +=  offset;
        if(offset == 0)
        {
            uni_stats.rxbcfrm_cnt = g_uni_stats_last[i].rxbcfrm_cnt;
        }
        g_uni_stats[i].rxoamfrm_cnt              += COUNT_STATS( uni_stats.rxoamfrm_cnt            ,g_uni_stats_last[i].rxoamfrm_cnt           )  ;
        g_uni_stats[i].rxpausefrm_cnt            += COUNT_STATS( uni_stats.rxpausefrm_cnt          ,g_uni_stats_last[i].rxpausefrm_cnt         )  ;
        g_uni_stats[i].rxunknownopfrm_cnt        += COUNT_STATS( uni_stats.rxunknownopfrm_cnt      ,g_uni_stats_last[i].rxunknownopfrm_cnt     )  ;
        g_uni_stats[i].rxcrcerrfrm_cnt           += COUNT_STATS( uni_stats.rxcrcerrfrm_cnt         ,g_uni_stats_last[i].rxcrcerrfrm_cnt        )  ;
        g_uni_stats[i].rxundersizefrm_cnt        += COUNT_STATS( uni_stats.rxundersizefrm_cnt      ,g_uni_stats_last[i].rxundersizefrm_cnt     )  ;
        g_uni_stats[i].rxruntfrm_cnt             += COUNT_STATS( uni_stats.rxruntfrm_cnt           ,g_uni_stats_last[i].rxruntfrm_cnt          )  ;
        g_uni_stats[i].rxoversizefrm_cnt         += COUNT_STATS( uni_stats.rxoversizefrm_cnt       ,g_uni_stats_last[i].rxoversizefrm_cnt      )  ;
        g_uni_stats[i].rxjabberfrm_cnt           += COUNT_STATS( uni_stats.rxjabberfrm_cnt         ,g_uni_stats_last[i].rxjabberfrm_cnt        )  ;
        g_uni_stats[i].rxstatsfrm64_cnt          += COUNT_STATS( uni_stats.rxstatsfrm64_cnt        ,g_uni_stats_last[i].rxstatsfrm64_cnt       )  ;
        g_uni_stats[i].rxstatsfrm65_127_cnt      += COUNT_STATS( uni_stats.rxstatsfrm65_127_cnt    ,g_uni_stats_last[i].rxstatsfrm65_127_cnt   )  ;
        g_uni_stats[i].rxstatsfrm128_255_cnt     += COUNT_STATS( uni_stats.rxstatsfrm128_255_cnt   ,g_uni_stats_last[i].rxstatsfrm128_255_cnt  )  ;
        g_uni_stats[i].rxstatsfrm256_511_cnt     += COUNT_STATS( uni_stats.rxstatsfrm256_511_cnt   ,g_uni_stats_last[i].rxstatsfrm256_511_cnt  )  ;
        g_uni_stats[i].rxstatsfrm512_1023_cnt    += COUNT_STATS( uni_stats.rxstatsfrm512_1023_cnt  ,g_uni_stats_last[i].rxstatsfrm512_1023_cnt )  ;
        g_uni_stats[i].rxstatsfrm1024_1518_cnt   += COUNT_STATS( uni_stats.rxstatsfrm1024_1518_cnt ,g_uni_stats_last[i].rxstatsfrm1024_1518_cnt)  ;
        g_uni_stats[i].rxstatsfrm1519_Max_cnt    += COUNT_STATS( uni_stats.rxstatsfrm1519_Max_cnt  ,g_uni_stats_last[i].rxstatsfrm1519_Max_cnt )  ;
        g_uni_stats[i].rxbyte_cnt                += COUNT_STATS( uni_stats.rxbyte_cnt              ,g_uni_stats_last[i].rxbyte_cnt             )  ;
        g_uni_stats[i].rxfrm_cnt                 += COUNT_STATS( uni_stats.rxfrm_cnt               ,g_uni_stats_last[i].rxfrm_cnt              )  ;
        g_uni_stats[i].txucfrm_cnt               += COUNT_STATS( uni_stats.txucfrm_cnt             ,g_uni_stats_last[i].txucfrm_cnt            )  ;
        g_uni_stats[i].txmcfrm_cnt               += COUNT_STATS( uni_stats.txmcfrm_cnt             ,g_uni_stats_last[i].txmcfrm_cnt            )  ;
        g_uni_stats[i].txbcfrm_cnt               += COUNT_STATS( uni_stats.txbcfrm_cnt             ,g_uni_stats_last[i].txbcfrm_cnt            )  ;
        g_uni_stats[i].txoamfrm_cnt              += COUNT_STATS( uni_stats.txoamfrm_cnt            ,g_uni_stats_last[i].txoamfrm_cnt           )  ;
        g_uni_stats[i].txpausefrm_cnt            += COUNT_STATS( uni_stats.txpausefrm_cnt          ,g_uni_stats_last[i].txpausefrm_cnt         )  ;
        g_uni_stats[i].txcrcerrfrm_cnt           += COUNT_STATS( uni_stats.txcrcerrfrm_cnt         ,g_uni_stats_last[i].txcrcerrfrm_cnt        )  ;
        g_uni_stats[i].txoversizefrm_cnt         += COUNT_STATS( uni_stats.txoversizefrm_cnt       ,g_uni_stats_last[i].txoversizefrm_cnt      )  ;
        g_uni_stats[i].txsinglecolfrm_cnt        += COUNT_STATS( uni_stats.txsinglecolfrm_cnt      ,g_uni_stats_last[i].txsinglecolfrm_cnt     )  ;
        g_uni_stats[i].txmulticolfrm_cnt         += COUNT_STATS( uni_stats.txmulticolfrm_cnt       ,g_uni_stats_last[i].txmulticolfrm_cnt      )  ;
        g_uni_stats[i].txlatecolfrm_cnt          += COUNT_STATS( uni_stats.txlatecolfrm_cnt        ,g_uni_stats_last[i].txlatecolfrm_cnt       )  ;
        g_uni_stats[i].txexesscolfrm_cnt         += COUNT_STATS( uni_stats.txexesscolfrm_cnt       ,g_uni_stats_last[i].txexesscolfrm_cnt      )  ;
        g_uni_stats[i].txstatsfrm64_cnt          += COUNT_STATS( uni_stats.txstatsfrm64_cnt        ,g_uni_stats_last[i].txstatsfrm64_cnt       )  ;
        g_uni_stats[i].txstatsfrm65_127_cnt      += COUNT_STATS( uni_stats.txstatsfrm65_127_cnt    ,g_uni_stats_last[i].txstatsfrm65_127_cnt   )  ;
        g_uni_stats[i].txstatsfrm128_255_cnt     += COUNT_STATS( uni_stats.txstatsfrm128_255_cnt   ,g_uni_stats_last[i].txstatsfrm128_255_cnt  )  ;
        g_uni_stats[i].txstatsfrm256_511_cnt     += COUNT_STATS( uni_stats.txstatsfrm256_511_cnt   ,g_uni_stats_last[i].txstatsfrm256_511_cnt  )  ;
        g_uni_stats[i].txstatsfrm512_1023_cnt    += COUNT_STATS( uni_stats.txstatsfrm512_1023_cnt  ,g_uni_stats_last[i].txstatsfrm512_1023_cnt )  ;
        g_uni_stats[i].txstatsfrm1024_1518_cnt   += COUNT_STATS( uni_stats.txstatsfrm1024_1518_cnt ,g_uni_stats_last[i].txstatsfrm1024_1518_cnt)  ;
        g_uni_stats[i].txstatsfrm1519_Max_cnt    += COUNT_STATS( uni_stats.txstatsfrm1519_Max_cnt  ,g_uni_stats_last[i].txstatsfrm1519_Max_cnt )  ;
        g_uni_stats[i].txbyte_cnt                += COUNT_STATS( uni_stats.txbyte_cnt              ,g_uni_stats_last[i].txbyte_cnt             )  ;
        g_uni_stats[i].txfrm_cnt                 += COUNT_STATS( uni_stats.txfrm_cnt               ,g_uni_stats_last[i].txfrm_cnt              )  ;
        g_uni_stats[i].single_collision           += COUNT_STATS( uni_stats.single_collision        ,g_uni_stats_last[i].single_collision      )  ;
        g_uni_stats[i].multiple_collision        += COUNT_STATS( uni_stats.multiple_collision       ,g_uni_stats_last[i].multiple_collision      )  ;
        g_uni_stats[i].fcs_errors                 += COUNT_STATS( uni_stats.fcs_errors              ,g_uni_stats_last[i].fcs_errors      )  ;
        g_uni_stats[i].late_collisions            += COUNT_STATS( uni_stats.late_collisions          ,g_uni_stats_last[i].late_collisions      )  ;
        g_uni_stats[i].excessive_collisions     += COUNT_STATS( uni_stats.excessive_collisions   ,g_uni_stats_last[i].excessive_collisions    )  ;

        memcpy(&g_uni_stats_last[i],&uni_stats,sizeof(uni_stats));
        cs_mutex_unlock(g_app_uni_stats_mutex);


    }

}


#ifdef HAVE_STATISTICS_NNI
static void app_nni_stats_count()
{
    cs_callback_context_t     context;
    cs_sdl_port_uni_stats_t uni_stats;
    cs_status ret = CS_E_OK;
    cs_port_id_t port = 0;
    cs_uint32 offset = 0;

	port = CS_UPLINK_PORT;
    ret = epon_request_onu_port_stats_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
            port,FALSE, &uni_stats);
    if(ret != CS_E_OK){
        PORT_STATS_LOG_INFO("epon_request_onu_port_stats_get return failed, port = %d ret = %d \n",port,ret);
        return ;
    }

    cs_mutex_lock(g_app_nni_stats_mutex);
    g_nni_stats.rxucfrm_cnt               += COUNT_STATS( uni_stats.rxucfrm_cnt             ,g_nni_stats_last.rxucfrm_cnt            )  ;
 
    /*special handle for rx mc counter*/
    offset = oam_count_stats_offset( uni_stats.rxmcfrm_cnt , g_nni_stats_last.rxmcfrm_cnt);
    g_nni_stats.rxmcfrm_cnt               +=  offset;
    if(offset == 0)
    {
        uni_stats.rxmcfrm_cnt = g_nni_stats_last.rxmcfrm_cnt;
    }

    /*special handle for rx bc counter*/
    offset = oam_count_stats_offset( uni_stats.rxbcfrm_cnt , g_nni_stats_last.rxbcfrm_cnt);
    g_nni_stats.rxbcfrm_cnt               +=  offset;
    if(offset == 0)
    {
        uni_stats.rxbcfrm_cnt = g_nni_stats_last.rxbcfrm_cnt;
    }
    g_nni_stats.rxoamfrm_cnt              += COUNT_STATS( uni_stats.rxoamfrm_cnt            ,g_nni_stats_last.rxoamfrm_cnt           )  ;
    g_nni_stats.rxpausefrm_cnt            += COUNT_STATS( uni_stats.rxpausefrm_cnt          ,g_nni_stats_last.rxpausefrm_cnt         )  ;
    g_nni_stats.rxunknownopfrm_cnt        += COUNT_STATS( uni_stats.rxunknownopfrm_cnt      ,g_nni_stats_last.rxunknownopfrm_cnt     )  ;
    g_nni_stats.rxcrcerrfrm_cnt           += COUNT_STATS( uni_stats.rxcrcerrfrm_cnt         ,g_nni_stats_last.rxcrcerrfrm_cnt        )  ;
    g_nni_stats.rxundersizefrm_cnt        += COUNT_STATS( uni_stats.rxundersizefrm_cnt      ,g_nni_stats_last.rxundersizefrm_cnt     )  ;
    g_nni_stats.rxruntfrm_cnt             += COUNT_STATS( uni_stats.rxruntfrm_cnt           ,g_nni_stats_last.rxruntfrm_cnt          )  ;
    g_nni_stats.rxoversizefrm_cnt         += COUNT_STATS( uni_stats.rxoversizefrm_cnt       ,g_nni_stats_last.rxoversizefrm_cnt      )  ;
    g_nni_stats.rxjabberfrm_cnt           += COUNT_STATS( uni_stats.rxjabberfrm_cnt         ,g_nni_stats_last.rxjabberfrm_cnt        )  ;
    g_nni_stats.rxstatsfrm64_cnt          += COUNT_STATS( uni_stats.rxstatsfrm64_cnt        ,g_nni_stats_last.rxstatsfrm64_cnt       )  ;
    g_nni_stats.rxstatsfrm65_127_cnt      += COUNT_STATS( uni_stats.rxstatsfrm65_127_cnt    ,g_nni_stats_last.rxstatsfrm65_127_cnt   )  ;
    g_nni_stats.rxstatsfrm128_255_cnt     += COUNT_STATS( uni_stats.rxstatsfrm128_255_cnt   ,g_nni_stats_last.rxstatsfrm128_255_cnt  )  ;
    g_nni_stats.rxstatsfrm256_511_cnt     += COUNT_STATS( uni_stats.rxstatsfrm256_511_cnt   ,g_nni_stats_last.rxstatsfrm256_511_cnt  )  ;
    g_nni_stats.rxstatsfrm512_1023_cnt    += COUNT_STATS( uni_stats.rxstatsfrm512_1023_cnt  ,g_nni_stats_last.rxstatsfrm512_1023_cnt )  ;
    g_nni_stats.rxstatsfrm1024_1518_cnt   += COUNT_STATS( uni_stats.rxstatsfrm1024_1518_cnt ,g_nni_stats_last.rxstatsfrm1024_1518_cnt)  ;
    g_nni_stats.rxstatsfrm1519_Max_cnt    += COUNT_STATS( uni_stats.rxstatsfrm1519_Max_cnt  ,g_nni_stats_last.rxstatsfrm1519_Max_cnt )  ;
    g_nni_stats.rxbyte_cnt                += COUNT_STATS( uni_stats.rxbyte_cnt              ,g_nni_stats_last.rxbyte_cnt             )  ;
    g_nni_stats.rxfrm_cnt                 += COUNT_STATS( uni_stats.rxfrm_cnt               ,g_nni_stats_last.rxfrm_cnt              )  ;
    g_nni_stats.txucfrm_cnt               += COUNT_STATS( uni_stats.txucfrm_cnt             ,g_nni_stats_last.txucfrm_cnt            )  ;
    g_nni_stats.txmcfrm_cnt               += COUNT_STATS( uni_stats.txmcfrm_cnt             ,g_nni_stats_last.txmcfrm_cnt            )  ;
    g_nni_stats.txbcfrm_cnt               += COUNT_STATS( uni_stats.txbcfrm_cnt             ,g_nni_stats_last.txbcfrm_cnt            )  ;
    g_nni_stats.txoamfrm_cnt              += COUNT_STATS( uni_stats.txoamfrm_cnt            ,g_nni_stats_last.txoamfrm_cnt           )  ;
    g_nni_stats.txpausefrm_cnt            += COUNT_STATS( uni_stats.txpausefrm_cnt          ,g_nni_stats_last.txpausefrm_cnt         )  ;
    g_nni_stats.txcrcerrfrm_cnt           += COUNT_STATS( uni_stats.txcrcerrfrm_cnt         ,g_nni_stats_last.txcrcerrfrm_cnt        )  ;
    g_nni_stats.txoversizefrm_cnt         += COUNT_STATS( uni_stats.txoversizefrm_cnt       ,g_nni_stats_last.txoversizefrm_cnt      )  ;
    g_nni_stats.txsinglecolfrm_cnt        += COUNT_STATS( uni_stats.txsinglecolfrm_cnt      ,g_nni_stats_last.txsinglecolfrm_cnt     )  ;
    g_nni_stats.txmulticolfrm_cnt         += COUNT_STATS( uni_stats.txmulticolfrm_cnt       ,g_nni_stats_last.txmulticolfrm_cnt      )  ;
    g_nni_stats.txlatecolfrm_cnt          += COUNT_STATS( uni_stats.txlatecolfrm_cnt        ,g_nni_stats_last.txlatecolfrm_cnt       )  ;
    g_nni_stats.txexesscolfrm_cnt         += COUNT_STATS( uni_stats.txexesscolfrm_cnt       ,g_nni_stats_last.txexesscolfrm_cnt      )  ;
    g_nni_stats.txstatsfrm64_cnt          += COUNT_STATS( uni_stats.txstatsfrm64_cnt        ,g_nni_stats_last.txstatsfrm64_cnt       )  ;
    g_nni_stats.txstatsfrm65_127_cnt      += COUNT_STATS( uni_stats.txstatsfrm65_127_cnt    ,g_nni_stats_last.txstatsfrm65_127_cnt   )  ;
    g_nni_stats.txstatsfrm128_255_cnt     += COUNT_STATS( uni_stats.txstatsfrm128_255_cnt   ,g_nni_stats_last.txstatsfrm128_255_cnt  )  ;
    g_nni_stats.txstatsfrm256_511_cnt     += COUNT_STATS( uni_stats.txstatsfrm256_511_cnt   ,g_nni_stats_last.txstatsfrm256_511_cnt  )  ;
    g_nni_stats.txstatsfrm512_1023_cnt    += COUNT_STATS( uni_stats.txstatsfrm512_1023_cnt  ,g_nni_stats_last.txstatsfrm512_1023_cnt )  ;
    g_nni_stats.txstatsfrm1024_1518_cnt   += COUNT_STATS( uni_stats.txstatsfrm1024_1518_cnt ,g_nni_stats_last.txstatsfrm1024_1518_cnt)  ;
    g_nni_stats.txstatsfrm1519_Max_cnt    += COUNT_STATS( uni_stats.txstatsfrm1519_Max_cnt  ,g_nni_stats_last.txstatsfrm1519_Max_cnt )  ;
    g_nni_stats.txbyte_cnt                += COUNT_STATS( uni_stats.txbyte_cnt              ,g_nni_stats_last.txbyte_cnt             )  ;
    g_nni_stats.txfrm_cnt                 += COUNT_STATS( uni_stats.txfrm_cnt               ,g_nni_stats_last.txfrm_cnt              )  ;
    g_nni_stats.single_collision          += COUNT_STATS( uni_stats.single_collision        ,g_nni_stats_last.single_collision       )  ;
    g_nni_stats.multiple_collision        += COUNT_STATS( uni_stats.multiple_collision      ,g_nni_stats_last.multiple_collision     )  ;
    g_nni_stats.fcs_errors                += COUNT_STATS( uni_stats.fcs_errors              ,g_nni_stats_last.fcs_errors             )  ;
    g_nni_stats.late_collisions           += COUNT_STATS( uni_stats.late_collisions         ,g_nni_stats_last.late_collisions        )  ;
    g_nni_stats.excessive_collisions	  += COUNT_STATS( uni_stats.excessive_collisions    ,g_nni_stats_last.excessive_collisions   )  ;

    memcpy(&g_nni_stats_last,&uni_stats,sizeof(uni_stats));
    cs_mutex_unlock(g_app_nni_stats_mutex);

}

cs_status app_onu_nni_stats_get(
        oam_port_uni_stats_t *uni_stats)
{
    PORT_STATS_ASSERT_RET(uni_stats != NULL,CS_E_PARAM);

	app_nni_stats_count();
	
    memcpy(uni_stats,&g_nni_stats,sizeof(g_nni_stats));
	
    return CS_E_OK;
}

void app_stats_reset_nni()
{
    cs_callback_context_t     context;
    cs_port_id_t port = 0;

    cs_mutex_lock(g_app_nni_stats_mutex);

	port = CS_UPLINK_PORT;
	/*clr from sdl*/
	epon_request_onu_port_stats_clr(context, 
		ONU_DEVICEID_FOR_API,
		ONU_LLIDPORT_FOR_API, 
		port);
	
    /*clear from oam*/
    memset(&g_nni_stats, 0x00,sizeof(g_nni_stats));
    memset(&g_nni_stats_last, 0x00,sizeof(g_nni_stats_last));

    cs_mutex_unlock(g_app_nni_stats_mutex);

}
#endif


/*****************************************************************************/
/* $rtn_hdr_start  app_stats_polling_enable_set                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void app_stats_polling_enable_set(cs_boolean enable)
/*                                                                           */
/* INPUTS  : o enable - enable or disable the polling                        */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* enable or disable the polling process                                     */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    g_app_stats_polling_enable = enable;
}


/*****************************************************************************/
/* $rtn_hdr_start  app_port_stats_polling                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void app_port_stats_polling(void * data)
/*                                                                           */
/* INPUTS  : o data - no use,just for compatiable                            */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* enable or disable the polling process                                     */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    if(g_app_stats_polling_enable){
        app_pon_stats_count();
        app_uni_stats_count();
		#ifdef HAVE_STATISTICS_NNI
		app_nni_stats_count();
		#endif
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  app_stats_reset_pon                                       */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void app_stats_reset_pon()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* reset the pon stats                                                       */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_callback_context_t     context;

    cs_mutex_lock(g_app_pon_stats_mutex);
    /*clr from sdl*/
    epon_request_onu_pon_stats_clr(context,
            ONU_DEVICEID_FOR_API, 
            ONU_LLIDPORT_FOR_API);

    /*clear from oam*/    
    memset(&g_pon_stats, 0, sizeof(g_pon_stats));
    memset(&g_pon_stats_last, 0, sizeof(g_pon_stats_last));
    cs_mutex_unlock(g_app_pon_stats_mutex);
}


/*****************************************************************************/
/* $rtn_hdr_start  app_stats_reset_uni                                       */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void app_stats_reset_uni(cs_port_id_t port_id)
/*                                                                           */
/* INPUTS  : o port_id - uni port id                                         */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* reset the uni stats                                                       */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_callback_context_t     context;
    cs_port_id_t port = 0;

    PORT_STATS_ASSERT(!(port_id < CS_UNI_PORT_ID1 
                || (port_id > g_app_max_uni_port 
                    &&  port_id != CS_DOWNLINK_PORT))
            );
   
    cs_mutex_lock(g_app_uni_stats_mutex);
    if(port_id == CS_DOWNLINK_PORT){
        if(g_app_max_uni_port == 1)
        {
            PORT_STATS_LOG("invalid port id = %d \n",port_id);
            cs_mutex_unlock(g_app_uni_stats_mutex);
            return;
        }
        else
        {
            cs_sdl_port_uni_stats_t  clear_stats;
            /*index 0 for internal Ge port*/
            port = 0;
             /*clr from sdl*/
            epon_request_onu_port_stats_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
                    CS_DOWNLINK_PORT,TRUE, &clear_stats);
        }
    }
    else{
        port = port_id  ;
        /*clr from sdl*/
        epon_request_onu_port_stats_clr(context, 
                ONU_DEVICEID_FOR_API,
                ONU_LLIDPORT_FOR_API, 
                port);
    }
    
    /*clear from oam*/
    memset(&g_uni_stats[port], 0x00,sizeof(g_uni_stats[port]));
    memset(&g_uni_stats_last[port], 0x00,sizeof(g_uni_stats_last[port]));

    cs_mutex_unlock(g_app_uni_stats_mutex);

}


/*****************************************************************************/
/* $rtn_hdr_start  app_port_stats_reset                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void app_port_stats_reset()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* reset the all ports stats, include pon and uni                            */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 i;
    app_stats_reset_pon();

    for(i = 0; i < (g_app_max_uni_port); i++){
        app_stats_reset_uni(i + 1);
    }
    if(g_app_max_uni_port != 1)
    {
        /* Reset from Ge port*/
        app_stats_reset_uni(CS_DOWNLINK_PORT);
    }

	#ifdef HAVE_STATISTICS_NNI
	app_stats_reset_nni();
	#endif
}

/*****************************************************************************/
/* $rtn_hdr_start  app_port_stats_port_reg_change_event_handler                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static void app_port_stats_reg_change_event_handler(
        cs_uint16 dev, 
        cs_int32 type, 
        void* msg)
/*                                                                           */
/* INPUTS  : o dev - device id                                               */
/*           o type - event type                                             */
/*           o msg - event message                                           */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process the llid register event                                           */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    sdl_event_reg_t *reg = NULL;

    PORT_STATS_ASSERT(msg != NULL);
    
    reg = (sdl_event_reg_t*)msg;

    if(reg->reg)
    {
        /* reset the port stats and start polling */
        //app_port_stats_reset();
        app_stats_polling_enable_set(TRUE);
        oam_link_event_reset();
    }
    else
    {
        /* disable the port stats polling */
        app_stats_polling_enable_set(FALSE);
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  app_onu_port_status_change_get                                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_status app_onu_port_status_change_get(
        cs_port_id_t port_id,
        cs_uint32 *count)
/*                                                                           */
/* INPUTS  : o port_id - uni port id                                         */
/*           o uni_stats - the uni port stats                                */
/* OUTPUTS : get status                                                      */
/* DESCRIPTION:                                                              */
/* get the uni port stats                                                    */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    /*only for uni port*/
    if(port_id < CS_UNI_PORT_ID1 || port_id > g_app_max_uni_port)
    {
        return CS_E_PARAM;
    }
    cs_mutex_lock(g_app_uni_status_change_mutex);
    *count = g_uni_status_change_times[port_id-1];
    cs_mutex_unlock(g_app_uni_status_change_mutex);

    return CS_E_OK;
}

/*****************************************************************************/
/* $rtn_hdr_start  app_port_status_change_event_handler                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static void app_port_status_change_event_handler(
        cs_uint16 dev, 
        cs_int32 type, 
        void* data)
/*                                                                           */
/* INPUTS  : o dev - device id                                               */
/*           o type - event type                                             */
/*           o msg - event message                                           */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process the llid register event                                           */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    sdl_event_port_link_t *msg = NULL;
    cs_port_id_t port;

    PORT_STATS_ASSERT(data != NULL);
    
    msg = (sdl_event_port_link_t*)data;
    port = msg->port;

    if(port >= CS_UNI_PORT_ID1 && port <= g_app_max_uni_port)
    {
        cs_mutex_lock(g_app_uni_status_change_mutex);
        /* add counter */
        g_uni_status_change_times[port-1]++;
        cs_mutex_unlock(g_app_uni_status_change_mutex);
    }
   
}


/*****************************************************************************/
/* $rtn_hdr_start  app_port_stats_init                                       */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void app_port_stats_init()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* port status init                                                          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    startup_config_read(CFG_ID_SWITCH_PORT_NUM, sizeof(g_app_max_uni_port), &g_app_max_uni_port);

    if(CS_E_OSAL_OK != cs_mutex_init(&g_app_pon_stats_mutex, "app_pon_stat_mux", 0)) {
        PORT_STATS_LOG("initialize g_app_pon_stats_mutex failed ");
        return;
    }
    if(CS_E_OSAL_OK != cs_mutex_init(&g_app_uni_stats_mutex, "app_uni_stat_mux", 0)) {
        PORT_STATS_LOG("initialize g_app_uni_stats_mutex failed ");
        return;
    }
    if(CS_E_OSAL_OK != cs_mutex_init(&g_app_uni_status_change_mutex, "app_uni_change_mux", 0)) {
        PORT_STATS_LOG("initialize g_app_uni_status_change_mutex failed ");
        return;
    }
	#ifdef HAVE_STATISTICS_NNI
	if(CS_E_OSAL_OK != cs_mutex_init(&g_app_nni_stats_mutex, "app_nni_stat_mux", 0)) {
        PORT_STATS_LOG("initialize g_app_nni_stats_mutex failed ");
        return;
    }
	#endif

    /*this memory no need free*/
    g_uni_stats = PORT_STATS_MALLOC(sizeof(oam_port_uni_stats_t) * (g_app_max_uni_port + 1));
    g_uni_stats_last = PORT_STATS_MALLOC(sizeof(cs_sdl_port_uni_stats_t) * (g_app_max_uni_port + 1));
    g_uni_status_change_times = PORT_STATS_MALLOC(sizeof(cs_uint32) * g_app_max_uni_port);

    app_port_stats_reset();

    g_app_stats_timer.msg_type = IROS_MSG_TIME_OUT;
    g_app_stats_timer.timer_type = APP_TIMER_OAM_STATS;
    cs_msg_circle_timer_add(app_msg_q,APP_STATS_POLL_ACCURACY,
            (void *)&g_app_stats_timer);

    onu_evt_reg(EPON_EVENT_REG_CHANGE, (void*)app_port_stats_reg_change_event_handler, NULL);
    onu_evt_reg(EPON_EVENT_PORT_LINK_CHANGE, (void*)app_port_status_change_event_handler, NULL);

}

/*****************************************************************************/
/* $rtn_hdr_start  app_onu_pon_stats_get                                     */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_status app_onu_pon_stats_get(oam_pon_stats_t *pon_stats)
/*                                                                           */
/* INPUTS  : o pon_stats - store pon port stats                              */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* get the pon port stats                                                    */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    PORT_STATS_ASSERT_RET(pon_stats != NULL,CS_E_PARAM);

    app_pon_stats_count();

    memcpy(pon_stats,&g_pon_stats,sizeof(g_pon_stats));

    return CS_E_OK;
}


/*****************************************************************************/
/* $rtn_hdr_start  app_onu_std_pon_stats_get                                 */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_status app_onu_std_pon_stats_get(epon_if_stats_t *if_stats)
/*                                                                           */
/* INPUTS  : o if_stats - store std pon port stats                           */
/* OUTPUTS : get status                                                      */
/* DESCRIPTION:                                                              */
/* get the std pon port stats                                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_pon_stats_t pon_stats; 

    PORT_STATS_ASSERT_RET(if_stats != NULL,CS_E_PARAM);

    memset(&pon_stats, 0, sizeof(pon_stats));

    app_onu_pon_stats_get(&pon_stats);

    if_stats->in_octets = pon_stats.pon_byte_cnt;
    if_stats->in_unicast = pon_stats.pon_frame_cnt;
    if_stats->in_multicast = pon_stats.pon_mcframe_cnt;
    if_stats->in_broadcast = pon_stats.pon_bcframe_cnt;
    if_stats->in_discards = 0;
    if_stats->in_errors = pon_stats.pon_crcerr_cnt +  pon_stats.pon_sld_err + pon_stats.pon_crc8_err;

    if_stats->in_unknown_protos = 0;
    if_stats->in_err_symbol = pon_stats.pon_err_symbol;
    if_stats->out_octets = pon_stats.pon_tx_byte_cnt;
    if_stats->out_unicast = pon_stats.pon_txframe_cnt;
    if_stats->out_multicast = pon_stats.pon_txmcframe_cnt;
    if_stats->out_broadcast = pon_stats.pon_txbcframe_cnt;
    if_stats->out_discards = 0;
    if_stats->out_errors = pon_stats.pon_tx_crc_err;

    return CS_E_OK;
}

/*****************************************************************************/
/* $rtn_hdr_start  app_onu_pon_stats_summary_get                             */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void app_onu_pon_stats_summary_get(
        cs_uint64 *in_frame, 
        cs_uint64 *in_errors, 
        cs_uint64 *in_err_symb)
/*                                                                           */
/* INPUTS  : o in_frame - in frame counter                                   */
/*           o in_error - in error counter                                   */
/*           o in_err_symb - in error symbol counter                         */
/*           o in_octets - in octets counter                                 */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* get the summary of the pon port stats                                     */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    epon_if_stats_t pon_stats; 

    PORT_STATS_ASSERT(in_frame != NULL 
            && in_errors != NULL
            && in_err_symb != NULL);

    memset(&pon_stats,0x00,sizeof(pon_stats));

    app_onu_std_pon_stats_get(&pon_stats);

    *in_frame = pon_stats.in_unicast + pon_stats.in_multicast 
        + pon_stats.in_broadcast + pon_stats.in_errors;
    *in_errors = pon_stats.in_errors; 
    *in_err_symb = pon_stats.in_err_symbol;
}


/*****************************************************************************/
/* $rtn_hdr_start  app_onu_port_stats_get                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_status app_onu_port_stats_get(
        cs_port_id_t port_id,
        oam_port_uni_stats_t *uni_stats)
/*                                                                           */
/* INPUTS  : o port_id - uni port id                                         */
/*           o uni_stats - the uni port stats                                */
/* OUTPUTS : get status                                                      */
/* DESCRIPTION:                                                              */
/* get the uni port stats                                                    */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 port = 0;

    PORT_STATS_ASSERT_RET(uni_stats != NULL,CS_E_PARAM);

    PORT_STATS_ASSERT_RET(!(port_id < CS_UNI_PORT_ID1 
                || (port_id > g_app_max_uni_port 
                    &&  port_id != CS_DOWNLINK_PORT))
            ,CS_E_PARAM);

    app_uni_stats_count();

    if(port_id == CS_DOWNLINK_PORT){
        if(g_app_max_uni_port == 1)
        {
            PORT_STATS_LOG("invalid port id = %d \n",port_id);
            return CS_E_PARAM;
        }
        else
        {
            port = 0;
        }
    }
    else{
        port = port_id;
    }

    memcpy(uni_stats,&g_uni_stats[port],sizeof(g_uni_stats[port]));
    return CS_E_OK;

}

cs_status app_show_pon_stats_cmd()
{
    oam_pon_stats_t stats;

    app_onu_pon_stats_get(&stats);

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

    return CS_E_OK;
}

void app_show_uni_counter( cs_port_id_t   portid)
{
    cs_status               status = 0;
    oam_port_uni_stats_t mib;

    status = app_onu_port_stats_get(portid, &mib);
    if(status != CS_E_OK){
        cs_printf("app_onu_port_stats_get failed \n");    
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

