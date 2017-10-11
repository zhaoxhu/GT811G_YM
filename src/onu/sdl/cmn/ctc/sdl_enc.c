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
#include "plat_common.h"
#include "sdl_enc.h"
#include "aal_enc.h"
#include "sdl_event_cmn.h"
#include "sdl_int.h"
#include "sdl_pon.h"
#include "aal_pon.h"
#include "sdl_init.h"
#include "sdl.h"


void sdl_enc_int_handler(void)
{
    sdl_event_enc_encrypted_t enc_event;
    cs_aal_pon_int_msk_t int_msk;
    
    //cs_printf("ENC Interrupt triggerred!!!\n");

    /*Disable interrupt after triggered*/
    int_msk.u32 = 0;
    int_msk.s.enc_en_trig = 1;   
    aal_pon_int_disable(int_msk);

    enc_event.encrypted = 1;
    sdl_event_send(EPON_EVENT_ENC_ENCRYPTED, sizeof(sdl_event_enc_encrypted_t), &enc_event);

}

cs_status sdl_enc_init(
    CS_IN const sdl_init_cfg_t *cfg
)
{    
    cs_aal_enc_cfg_t enc_cfg;
    cs_aal_enc_msk_t cfg_msk;

    /*Decryption is enable*/
    enc_cfg.en = 1; 
    enc_cfg.mode = AAL_ENC_ALG_CHURN;
    enc_cfg.oam_oui = 0x111111;
    enc_cfg.key_gen_mode = AAL_ENC_KEY_GEN_MODE_HW;
    enc_cfg.key_exchange_mode = AAL_ENC_KEY_EXCHANGE_SW;
    
    cfg_msk.u32 = 0;   
    cfg_msk.s.en = 1;
    cfg_msk.s.mode = 1;
    cfg_msk.s.oam_oui = 1;
    cfg_msk.s.key_gen_mode = 1;
    cfg_msk.s.key_exchange_mode = 1;
    aal_enc_cfg_set(cfg_msk, &enc_cfg);
       
    /*Register Interrupt handler */
    sdl_int_handler_reg(SDL_INT_ENC_ENCRYPTED_RECEIVED, sdl_enc_int_handler);

    return CS_E_OK;
}


/* Random number generator. Ripped out of the C library.*/
static cs_int32 __key_rand(cs_int32 *seed)
{    
    #define KEY_RAND_MAX 0x7fffffff
    #define KEY_MM 2147483647    /* a Mersenne prime*/
    #define KEY_AA 48271         /* this does well in the spectral test*/
    #define KEY_QQ 44488         /* (long)(MM/AA)*/
    #define KEY_RR 3399          /* MM % AA; it is important that KEY_RR<KEY_QQ*/

    *seed = KEY_AA * (*seed % KEY_QQ) - KEY_RR * (cs_int32)(*seed / KEY_QQ);
    if (*seed < 0)
        *seed += KEY_MM;

    return (cs_int32)(*seed & KEY_RAND_MAX);
}

cs_status epon_request_onu_enc_mode_set(
    CS_IN cs_callback_context_t      context,
    CS_IN cs_uint32                  device,
    CS_IN cs_uint32                  llidport,
    CS_IN cs_uint8                   mode,
    CS_IN cs_boolean                 enable
)
{
    cs_aal_enc_cfg_t cfg;
    cs_aal_enc_msk_t cfg_msk;

    if((mode>AAL_ENC_ALG_AES_SPECIAL) || (enable>1)){
        SDL_MIN_LOG("epon_request_onu_enc_mode_set():err params!\n");
        return CS_E_PARAM;
    }
    
    cfg.mode = mode;
    cfg.en = enable;

    if(mode==AAL_ENC_ALG_CHURN)    
        cfg.key_gen_mode = AAL_ENC_KEY_GEN_MODE_HW;
    else 
        cfg.key_gen_mode = AAL_ENC_KEY_GEN_MODE_SW;
    
    cfg_msk.u32 = 0;   
    cfg_msk.s.mode = 1;
    cfg_msk.s.en = 1;
    cfg_msk.s.key_gen_mode = 1;
    aal_enc_cfg_set(cfg_msk, &cfg);

    //SDL_INF_LOG("ENC mode set to %d: %s\n", cfg.mode, cfg.en ? "Enable" : "Disable");

    return CS_E_OK;
}

cs_status epon_request_onu_enc_mode_get(
    CS_IN  cs_callback_context_t      context,
    CS_IN  cs_uint32                  device,
    CS_IN  cs_uint32                  llidport,
    CS_OUT cs_uint8                   *mode,
    CS_OUT cs_boolean                 *enable
)
{
    cs_aal_enc_cfg_t cfg;

    if((NULL==mode)||(NULL==enable)){
        SDL_MIN_LOG("epon_request_onu_enc_mode_get():err param!\n");
        return CS_E_PARAM;
    }

    aal_enc_cfg_get(&cfg);
    
    *mode = cfg.mode;
    *enable = cfg.en;
    
    //SDL_INF_LOG("Get ENC mode  is %d: %s\n", cfg.mode, cfg.en ? "Enable" : "Disable");

    return CS_E_OK;
}

cs_status epon_request_onu_enc_cnt_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_uint32                 device,
    CS_IN cs_uint32                 llidport,
    CS_IN cs_uint16                 counter
)
{
    cs_aal_enc_timer_cfg_t timer;
    cs_aal_enc_timer_msk_t timer_msk;

    timer_msk.u32 = 0;    
    timer_msk.s.timer_hi16 = 1;
    
    timer.timer_hi16 = counter; 
 
    aal_enc_timer_cfg_set(timer_msk, &timer);
        
    //SDL_INF_LOG("Set ENC Counter  is %d: \n", timer.timer_hi16);

    return CS_E_OK;
}

cs_status epon_request_onu_enc_cnt_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_uint32                 device,
    CS_IN cs_uint32                 llidport,
    CS_OUT cs_uint16                 *counter
)
{
    cs_aal_enc_timer_cfg_t timer;

    if(NULL==counter){
        SDL_MIN_LOG("epon_request_onu_enc_cnt_get():err param!\n");
        return CS_E_PARAM;
    }
    
    aal_enc_timer_cfg_get(&timer);
    *counter = timer.timer_hi16;

    return CS_E_OK;
}


cs_status epon_request_onu_enc_key_get(
    CS_IN  cs_callback_context_t      context,
    CS_IN  cs_uint32                  device,
    CS_IN  cs_uint32                  llidport,
    CS_IN  cs_uint8                   key_index,
    CS_OUT cs_uint32                  *key
)
{
    cs_uint8 i;
    cs_uint32 seed;
    cs_aal_enc_cfg_t cfg;
    cs_aal_enc_key_t onu_key;

    if((NULL==key)||(key_index>AAL_ENC_KEY_INDEX_1)){
        SDL_MIN_LOG("epon_request_onu_enc_key_get():err param!\n");
        return CS_E_PARAM;
    }

    memset(&onu_key, 0, sizeof(onu_key));

    aal_enc_cfg_get(&cfg);

    /*when mode is churning,key is generated by HW.otherwise key is genereated by SW */
    if(cfg.mode){
        for(i=0; i<4;i++){
            srand( (unsigned)time( NULL ) );
            seed = (i+1) * rand();
            key[i] = __key_rand(&seed);
        }
        memcpy((cs_uint8 *)onu_key, (cs_uint8 *)key, 16);
    }
    else{
        aal_enc_key_hw_get(key_index, &onu_key);
    }
    
    memcpy((cs_uint8 *)key, (cs_uint8 *)onu_key, 16);
    aal_enc_key_set(key_index, &onu_key);

    return CS_E_OK;
}




