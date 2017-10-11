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

#include "cs_types.h"
#include "plat_common.h"
#include "aal.h"
#include "sdl.h"
#include "sdl_init.h"
#include "sdl_int.h"
#include "aal_mpcp.h"
#include "aal_pon.h"
#include "aal_l2.h"
#include "aal_ma.h"
#include "cs_module.h"
#include "aal_sys.h"
#include "aal_uni.h"
#include "aal_pon.h"
#include "aal_ma.h"
#include "aal_ptp.h"
#include "aal_bm.h"
#include "sdl_port.h"
#include "sdl_pon.h"
#include "sdl_util.h"
#include "startup_cfg.h"
#include "sdl_pon.h"
#include "aal_serdes.h"
#ifndef HAVE_MPORTS
#include "aal_phy.h"
#endif
#ifdef HAVE_VOIP
#include "aal_pcm.h"
#endif
#include "aal_util.h"

cs_sdl_int_handler_list_t sdl_int_handler_head[SDL_INT_MAX];

cs_status sdl_service_up(void)
{
    cs_status ret = CS_E_OK;
    cs_uint8 tmp_val = 0;
    cs_callback_context_t   context;
    cs_aal_pon_mac_msk_t    pon_mac_mask;
    cs_aal_pon_mac_cfg_t    pon_mac_cfg;
    cs_aal_pon_int_msk_t    pon_int;
    cs_aal_ma_mii_msk_t     ma_mask;
    cs_aal_ma_mii_cfg_t     ma_cfg;
    cs_aal_ma_int_msk_t     ma_int_mask;
    cs_aal_fe_int_msk_t     fe_int_mask;
    cs_aal_uni_cfg_mask_t   uni_mac_mask;
    cs_aal_uni_cfg_t        uni_mac;
    cs_aal_module_reset_mask_t  reset_mask;
	cs_aal_uni_int_msk_t    uni_int_mask;

#ifndef HAVE_MPORTS
    cs_aal_phy_int_msk_t    phy_int_mask;
#endif
    cs_port_id_t port;

    /* 1. Enable interrupt */
    pon_int.u32 = 0;
    //pon_int.s.discovery_frm = 1;
    pon_int.s.registration_status_change = 1;
    pon_int.s.sig_loss = 1;
    pon_int.s.rx_register_frame = 1;
    //pon_int.s.enc_en_trig = 1;
    aal_pon_int_en_set(pon_int);

    ma_int_mask.u32 = 0;
    ma_int_mask.s.cpu_rx_buff0_rdy = 1;
    ma_int_mask.s.cpu_rx_buff1_rdy = 1;
    aal_ma_int_en_set(ma_int_mask);

    fe_int_mask.u32[0] = 0;
    fe_int_mask.u32[1] = 0;
    fe_int_mask.s.evt_ce_mac_filter_aged_out_pon = 1;
    aal_fe_int_enable(fe_int_mask);

	uni_int_mask.u32 = 0;
	uni_int_mask.s.phy_int = 1;
	aal_uni_int_enable(uni_int_mask);

#ifndef HAVE_MPORTS
    cs_phy_type_e type;
    aal_phy_type_get(AAL_PHY_ID_GE, &type);
    if (PHY_RTK_8211 == type)
    {
        // Realtek PHY will use polling
        uni_int_mask.u32 = 0;
        uni_int_mask.s.phy_int = 1;
        aal_uni_int_disable(uni_int_mask);
    }
    
    /* As suggested, use PHY int instead of MAC int for link-status */
    /* Attention: Must clear phy interrupt before enable phy_int */
    aal_phy_int_status_clr(AAL_PHY_ID_GE);

    phy_int_mask.s.auto_neg_fail = 1;
    phy_int_mask.s.auto_neg_success = 1;
    phy_int_mask.s.link_change  = 1;
    aal_phy_int_en_set(AAL_PHY_ID_GE, phy_int_mask);

    extern void __phy_int_process_handler();
    __phy_int_process_handler();
#endif

    /*2. Enable Tx/Rx on PON, UNI and MA.    */
#ifdef HAVE_VOIP
    onu_aal_pcm_enable();
#endif
    // PON SerDes init
    ma_cfg.rx_en = 1;
    ma_cfg.tx_en = 1;
    ma_mask.u32 = 0;
    ma_mask.s.rx_en = 1;
    ma_mask.s.tx_en = 1;
    aal_ma_mii_cfg_set(ma_mask, &ma_cfg);

    pon_mac_cfg.rx_en = 1;
    pon_mac_cfg.tx_en = 1;
    pon_mac_mask.u32 = 0;
    pon_mac_mask.s.rx_en = 1;
    pon_mac_mask.s.tx_en = 1;
    aal_pon_mac_cfg_set(pon_mac_mask, &pon_mac_cfg);

    /* Reset ge_rx_rst_n    in reg GLOBAL_RST_CTRL 0xc014,
     * Workaround for KT pkt corrupt issue (RX UNI add 2 bytes)
     * Bug 27045
     */
    reset_mask.u32 = 0;
    reset_mask.s.reset_ge_rx = 1;
    aal_sys_reset_ctrl_set(reset_mask,0);
    cs_thread_delay(1);
    aal_sys_reset_ctrl_set(reset_mask,1);

    uni_mac.rx_en = 1;
    uni_mac.tx_en = 1;
    uni_mac_mask.u32 = 0;
    uni_mac_mask.s.rx_en = 1;
    uni_mac_mask.s.tx_en = 1;
    aal_uni_mac_cfg_set(uni_mac_mask, uni_mac);

    /* UNI admin up/down */
    ret = startup_config_read(CFG_ID_UNI_ADMIN_STATUS, 1, &tmp_val);
    if(ret != CS_E_OK)
    {
        cs_halt("startup_config_read reutn %u. %s, line: %d\n", ret, __FILE__, __LINE__);
    }
    
    port = CS_UNI_PORT_ID1;
    
#ifdef HAVE_MPORTS
    for(port=CS_UNI_PORT_ID1; port<=UNI_PORT_MAX; port++){
#endif
        epon_request_onu_port_admin_set(context, 0, 0, port, (cs_sdl_port_admin_t)tmp_val);
    

        /* 3. protocol stack would send packets before init done unexpectedly */
        epon_request_onu_port_stats_clr(context, 0, 0, port);
    
#ifdef HAVE_MPORTS
    }
#endif
    
    
    epon_request_onu_pon_stats_clr(context, 0, 0);

    // power on transceiver
    epon_request_onu_pon_trans_rx_pwr_set(context, 0, 0, 1);

    epon_request_onu_pon_trans_tx_pwr_set(context, 0, 0, 1);

    return ret;
}


cs_status sdl_int_handler_reg(cs_sdl_int_t type, cs_sdl_int_handler_f handler)
{
    cs_sdl_int_handler_list_t *p = &sdl_int_handler_head[type];
    cs_sdl_int_handler_list_t *temp = NULL;

    if(NULL == handler) return CS_E_PARAM;
    if(type >= SDL_INT_MAX) return CS_E_PARAM;

    while(p->next != NULL) p = p->next;

    temp = (cs_sdl_int_handler_list_t*)iros_malloc(IROS_MID_SAL, sizeof(cs_sdl_int_handler_list_t));

    if(NULL==temp)
    {
        return CS_E_ERROR;
    }

    temp->func = handler;
    temp->next = NULL;
    p->next = temp;

    return CS_E_OK;
}

void sdl_int_handler(cs_sdl_int_t type)
{
    cs_sdl_int_handler_list_t *handler_list;
    handler_list = &sdl_int_handler_head[type];

    if(type >= SDL_INT_MAX) return;

    while( NULL != handler_list->next )
    {
        handler_list = handler_list->next;
        if( NULL!= handler_list->func )
        {
            handler_list->func();
        }
    }
}

void sdl_pon_int_process(void)
{
    cs_aal_pon_int_msk_t src,en;

    aal_pon_int_en_get(&en);
    aal_pon_int_src_get(&src);
    aal_pon_int_clr(src);

    if(src.s.sig_loss && en.s.sig_loss)
    {
        sdl_int_handler(SDL_INT_PON_LINK_CHANGE);
    }

    if(src.s.registration_status_change && en.s.registration_status_change)
    {
        sdl_int_handler(SDL_INT_REG_CHANGE);
    }

    if(src.s.enc_en_trig && en.s.enc_en_trig)
    {
        sdl_int_handler(SDL_INT_ENC_ENCRYPTED_RECEIVED);
    }

    if(src.s.rx_register_frame && en.s.rx_register_frame)
    {
        sdl_int_handler(SDL_INT_REG_FRAME_RECEIVED);
    }
}

void sdl_uni_int_process(void)
{
    cs_aal_uni_int_msk_t src,en;

    aal_uni_int_en_get(&en);
    aal_uni_int_status_get(&src);

    if (en.s.phy_int && src.s.phy_int)
    {
        sdl_int_handler(SDL_INT_UNI_LINK_CHANGE);
    }

    /* For UNI, since it is sourced from PHY,
     * Need to clear it after PHY int clear */
    aal_uni_int_status_clr(src);
}


void sdl_fe_int_process(void)
{
    cs_aal_fe_int_msk_t src,en;

    aal_fe_int_en_get(&en);
    aal_fe_int_status_get(&src);
    aal_fe_int_status_clr(src);

    if (src.s.evt_ce_mac_filter_aged_out_pon &&
        en.s.evt_ce_mac_filter_aged_out_pon)
    {
        sdl_int_handler(SDL_INT_MACFILTER_AGING_OUT);
    }
}

void sdl_bm_int_process(void)
{
    cs_aal_bm_int_msk_t src,en;

    aal_bm_int_en_get(&en);
    aal_bm_int_status_get(&src);
    aal_bm_int_status_clr(src);
    if((en.s.ge_tx_crc_error && src.s.ge_tx_crc_error)||
        (en.s.pon_tx_crc_error && src.s.pon_tx_crc_error)||
        (en.s.cpu_tx_crc_error && src.s.cpu_tx_crc_error)){
        sdl_int_handler(SDL_INT_BM_CHECK_CEC_ERR);
    }
}

void sdl_ma_int_process(void)
{
    cs_aal_ma_int_msk_t src,en;

    aal_ma_int_en_get(&en);
    aal_ma_int_src_get(&src);
    aal_ma_int_clr(src);

    if((src.s.cpu_rx_buff1_rdy && en.s.cpu_rx_buff1_rdy) ||
        (src.s.cpu_rx_buff0_rdy && en.s.cpu_rx_buff0_rdy))
    {
        sdl_int_handler(SDL_INT_CPU_RX_BUFF_RDY);
    }
}

cs_status sdl_init(void)
{    

    sdl_internel_init();

    return CS_E_OK;
}

#if 1
//added by zhuxh
cs_status uni_port_check(cs_port_id_t port)
{
	UNI_PORT_CHECK(port);
	return CS_E_OK;
}

cs_status uni_port_num_get(int *num)
{
	*num = UNI_PORT_MAX;
	return CS_E_OK;
}
#endif


