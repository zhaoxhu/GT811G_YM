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

#include "sdl_fec_cmn.h"
#include "aal_fec.h"
#include "sdl_init.h"
#include "sdl.h"

cs_status sdl_fec_init(
    CS_IN  sdl_init_cfg_t    *cfg
)
{
    cs_aal_fec_cfg_t fec_cfg; 
    cs_aal_fec_msk_t cfg_msk;

    if(NULL==cfg){
        SDL_MIN_LOG("sdl_fec_init():err param!\n");
        return CS_E_PARAM;
    }
    
    fec_cfg.rx_bypass = 1;
    fec_cfg.rx_en = 0;    
    fec_cfg.tx_en = 0;

    cfg_msk.u32 = 0;
    cfg_msk.s.rx_bypass = 1;
    cfg_msk.s.rx_en = 1;    
    cfg_msk.s.tx_en = 1;
    
    aal_fec_cfg_set(cfg_msk, &fec_cfg);
    
#ifdef HAVE_SDL_CTC
    cs_aal_fec_comp_cfg_t comp_cfg;
    cs_aal_fec_comp_msk_t comp_msk;
    
    /*For CTC, FEC compensation mode is set to 2, so that FEC enable/disable 
    switching will not cause de-register by default*/
    comp_cfg.mode = AAL_FEC_COMP_MODE_2;
    comp_cfg.churn_rx_path = 0x1b; /* bug 35981 */
    comp_msk.u32 = 0;
    comp_msk.s.mode = 1;
    comp_msk.s.churn_rx_path = 1;
    aal_fec_comp_cfg_set(comp_msk, &comp_cfg);
    
    /*For CTC, FEC RX data bypass is disabled by default*/
    cfg_msk.u32 = 0;
    cfg_msk.s.rx_bypass = 1;
    fec_cfg.rx_bypass = 0;
    
    aal_fec_cfg_set(cfg_msk, &fec_cfg);
#endif

    return CS_E_OK;

}

cs_status epon_request_onu_fec_mode_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_uint32                 device,
    CS_IN cs_uint32                 llidport,
    CS_IN cs_sdl_fec_mode_t         mode
)
{

    cs_aal_fec_cfg_t cfg;
    cs_aal_fec_msk_t mask;

    if(mode>SDL_FEC_MODE_3){
        SDL_MIN_LOG("epon_request_onu_fec_mode_set():err param!\n");
        return CS_E_PARAM;
    }

    mask.u32 = 0;
    
    switch (mode){
        case SDL_FEC_MODE_0:{
            cfg.rx_preamble_mode = 1;
            cfg.tx_preamble_mode = 1;
            break;
        }
        case SDL_FEC_MODE_1 :{
            cfg.rx_preamble_mode = 0;
            cfg.tx_preamble_mode = 1;
            break;
        }
        case SDL_FEC_MODE_2 :{
            cfg.rx_preamble_mode = 1;
            cfg.tx_preamble_mode = 0;
            break;
        }       
        case SDL_FEC_MODE_3 :{
            cfg.rx_preamble_mode = 0;
            cfg.tx_preamble_mode = 0;
            break;
        }
        default:
            return CS_E_PARAM;
    }
    
    mask.s.rx_preamble_mode = 1;
    mask.s.tx_preamble_mode = 1;
    
    aal_fec_cfg_set(mask, &cfg);

    return CS_E_OK;
    
}


cs_status epon_request_onu_fec_mode_get(
    CS_IN cs_callback_context_t    context,
    CS_IN cs_uint32                device,
    CS_IN cs_uint32                llidport,
    CS_IN cs_sdl_fec_mode_t        *mode
)
{
    cs_aal_fec_cfg_t cfg;

    if(NULL == mode){
        SDL_MIN_LOG("epon_request_onu_fec_mode_get():err param!\n");
        return CS_E_PARAM;
    }

    aal_fec_cfg_get(&cfg);

    if((cfg.rx_preamble_mode == 1) && (cfg.tx_preamble_mode == 1)){
        *mode = SDL_FEC_MODE_0;
        return CS_E_OK;            
    }
    else if((cfg.rx_preamble_mode == 0) && (cfg.tx_preamble_mode == 1)){
        *mode = SDL_FEC_MODE_1;
        return CS_E_OK; 
    }
    else if((cfg.rx_preamble_mode == 1) && (cfg.tx_preamble_mode == 0)){
        *mode = SDL_FEC_MODE_2;
        return CS_E_OK; 
    }
    else if((cfg.rx_preamble_mode == 0) && (cfg.tx_preamble_mode == 0)){
        *mode = SDL_FEC_MODE_3;
        return CS_E_OK; 
    }
    else 
        return CS_E_ERROR;            
       
}

cs_status epon_request_onu_fec_cnt_get(
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_int32                  device,
    CS_IN  cs_int32                  llidport,
    CS_OUT cs_sdl_fec_cnt_t          *counter
)
{
    cs_aal_fec_cnt_t cnt;

    if(NULL==counter){
        SDL_MIN_LOG("epon_request_onu_fec_get():err param!\n");
        return CS_E_PARAM;
    }
    
    aal_fec_cnt_get(&cnt);
    
    counter->rx_blks = cnt.rx_blks;
    counter->rx_correct_blks = cnt.rx_correct_blks;
    counter->rx_correct_bytes = cnt.rx_correct_bytes;
    counter->rx_good_blks = cnt.rx_good_blks;
    counter->rx_uncorrect_blks = cnt.rx_uncorrect_blks;    
    
    return CS_E_OK;
    
}


cs_status epon_request_onu_fec_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                enable
)
{
    cs_aal_fec_cfg_t cfg;
    cs_aal_fec_msk_t mask;

    if(enable>1){
        SDL_MIN_LOG("epon_request_onu_fec_set():err params!\n");
        return CS_E_PARAM;
    }
  
    mask.u32 = 0;
    mask.s.rx_en = 1;
    mask.s.tx_en = 1;
    
#ifndef HAVE_SDL_CTC    
    mask.s.rx_bypass = 1;
    cfg.rx_bypass = enable?0:1;
#endif
    
    cfg.rx_en = enable;
    cfg.tx_en = enable;   
    
    aal_fec_cfg_set(mask, &cfg);
        
    return CS_E_OK;

}


cs_status epon_request_onu_fec_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                *enable
)
{
    cs_aal_fec_cfg_t cfg;

    if(NULL==enable){
        SDL_MIN_LOG("epon_request_onu_fec_get():err param!\n");
        return CS_E_PARAM;
    }

    aal_fec_cfg_get(&cfg);
    
    if((cfg.rx_en) && (cfg.tx_en))
        *enable = 1;
    else 
        *enable = 0;
        
    return CS_E_OK;
}

cs_status epon_request_onu_fec_ability_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                *ability
)
{
    //cs_aal_fec_cfg_t cfg;

    if(NULL==ability){
        SDL_MIN_LOG("epon_request_onu_fec_ability_get():err param!\n");
        return CS_E_PARAM;
    }
    
#if 0
    aal_fec_cfg_get(&cfg);
    
    if((cfg.rx_en) && (!cfg.rx_bypass))
        *ability = 1;
    else 
        *ability = 0;
#endif

    *ability = 1;

        
    return CS_E_OK;
}

