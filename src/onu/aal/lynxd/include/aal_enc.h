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

/*

    This file is used for ENC driver code
    
*/



#ifndef __AAL_ENC_H__
#define __AAL_ENC_H__

#include "cs_types.h"

#define AAL_ENC_TIMER_ADJUST_RX_DEFAULT        (0xe)
#define AAL_ENC_TIMER_ADJUST_TX_DEFAULT        (0x0)


typedef enum {
    AAL_ENC_TIMER_ADJUST_DIR_SUB    = 0, /*timer is adjust by adding the delta */
    AAL_ENC_TIMER_ADJUST_DIR_ADD    = 1  /*timer is adjust by subtracting the delta */
} cs_aal_enc_timer_dir_t;

typedef enum {
    AAL_ENC_KEY_INDEX_0             = 0, /*key index 0 : even key */
    AAL_ENC_KEY_INDEX_1             = 1  /*key index 1 : odd key */
} cs_aal_enc_key_idx_t;

typedef enum {
    AAL_ENC_KEY_IDX_SEL_AUTO        = 0, /*key index is selected from the decrypted packet indication */
    AAL_ENC_KEY_IDX_SEL_MANUAL      = 1  /*key index is selected from the specified key index */
} cs_aal_enc_key_idx_sel_mode_t;

typedef enum {
    AAL_ENC_KEY_GEN_MODE_HW         = 0, /*key is genereated by HW */
    AAL_ENC_KEY_GEN_MODE_SW         = 1  /*key is genereated by SW */
} cs_aal_enc_key_gen_mode_t;

typedef enum {
    AAL_ENC_KEY_EXCHANGE_SW         = 0, /*key is exchanged by HW */
    AAL_ENC_KEY_EXCHANGE_HW         = 1  /*key is exchanged by HW */
} cs_aal_enc_key_exchange_mode_t;

typedef enum {
    AAL_ENC_KEY_SEL_AUTO            = 0, /*tx key is selected from downstream key set for sync*/
    AAL_ENC_KEY_SEL_MANUAL          = 1  /*tx key is selected from config*/
} cs_aal_enc_key_sel_mode_t;

typedef enum {
    AAL_ENC_RCC_SEL_DS              = 0, /*US RCC counter select from local RCC counter with downstream RCC_b0 compensation*/
    AAL_ENC_RCC_SEL_LOCAL           = 1  /*US RCC counter select from local MPCP RCC counter*/
} cs_aal_enc_rcc_sel_mode_t;

typedef enum {
    AAL_ENC_ALG_CHURN               = 0, /*encrypt/decrypt algorithms is churning */
    AAL_ENC_ALG_AES_32              = 1, /*encrypt/decrypt algorithms is AES 32 */
    AAL_ENC_ALG_AES_48              = 2, /*encrypt/decrypt algorithms is AES 48 */
    AAL_ENC_ALG_AES_SPECIAL         = 3  /*encrypt/decrypt algorithms is AES Special */
} cs_aal_enc_alg_t;

typedef enum {
    AAL_ENC_CHURN_MODE_TRIPLE       = 0, /*encrypt/decrypt algorithms is triple churning */
    AAL_ENC_CHURN_MODE_SINGLE       = 1  /*encrypt/decrypt algorithms is single churning */
} cs_aal_enc_churn_mode_t;


typedef cs_uint8 cs_aal_enc_key_t[16];


typedef struct {
    cs_boolean                       en;                 /*(default=0)en/disable decryption*/
    cs_aal_enc_alg_t                 mode;               /*(default=EPON_ENC_AES_NTT)*/      
    cs_aal_enc_churn_mode_t          churn_mode;         /*(default=ENC_CHURN_MODE_TRIPLE):valid when mode is churning*/                                          
    cs_uint16                        oam_flag;           /*(default=0x50):churning oam frame flag field*/
    cs_uint32                        oam_oui;            /*(default=0x111111):oam oui(only lowest 24bits used)*/
    cs_aal_enc_key_gen_mode_t        key_gen_mode;       /*(default= 1)*/
    cs_aal_enc_key_exchange_mode_t   key_exchange_mode;  /*(default= 0)*/
    cs_uint8                         extra_oam_rpt_len;  /*(default=128):Extra byte length for oam reports. */
}cs_aal_enc_cfg_t;

typedef union {
    struct {
        cs_uint32  en                : 1;
        cs_uint32  mode              : 1;
        cs_uint32  churn_mode        : 1;
        cs_uint32  oam_flag          : 1;
        cs_uint32  oam_oui           : 1;
        cs_uint32  key_gen_mode      : 1;        
        cs_uint32  key_exchange_mode : 1;        
        cs_uint32  extra_oam_rpt_len : 1;
        cs_uint32  rsvd              : 24;
    } s;
    cs_uint32 u32;
}cs_aal_enc_msk_t;

typedef struct {
    cs_boolean                       en;                 /*(default=0):en/disable encryption*/
    cs_uint8                         threshold;          /*(default=2):threshold to trigger US to encrypt.range:0~7*/
    cs_aal_enc_key_idx_sel_mode_t    key_idx_sel_mode;   /*(default=ENC_INDEX_FROM_CFG)*/
    cs_aal_enc_key_idx_t             key_idx;            /*(default=ENC_EVEN_KEY_TYPE)*/
    cs_boolean                       key_idx_notify_en;  /*(default=0): en/disable notify OLT key index */
    cs_boolean                       crc8_regen_en;      /*(default=enable),En/disable the crc8 generate*/
    cs_boolean                       crc8_in_reverse_en; /*(default=disable)En/disable the CRC8 input reverse*/
    cs_boolean                       crc8_out_reverse_en;/*(default=disable)en/disable the inverse of CRC8 results*/
    cs_aal_enc_rcc_sel_mode_t        rcc_b0_sel_mode;    /*debug only*/
    cs_aal_enc_key_sel_mode_t        key_sel_mode;       /*debug only*/
}cs_aal_enc_tx_cfg_t;

typedef union {
    struct {
        cs_uint32   en                  : 1;                    
        cs_uint32   threshold           : 1;              
        cs_uint32   key_idx_sel_mode    : 1;       
        cs_uint32   key_idx             : 1;                
        cs_uint32   key_idx_notify_en   : 1;      
        cs_uint32   crc8_regen_en       : 1;          
        cs_uint32   crc8_in_reverse_en  : 1;    
        cs_uint32   crc8_out_reverse_en : 1;    
        cs_uint32   rcc_b0_sel_mode     : 1;        
        cs_uint32   key_sel_mode        : 1;
        cs_uint32   rsvd                : 22;
    }s;
    cs_uint32 u32;
}cs_aal_enc_tx_msk_t;



typedef struct {
    cs_aal_enc_timer_dir_t  rx_dir;     /*(default=AAL_ENC_TIMER_ADJUST_DIR_SUB)*/ 
    cs_uint8                rx_delta;   /*(default=0): RX timer adjust value;range:0x0~0xffff*/
    cs_aal_enc_timer_dir_t  tx_dir;     /*(default=AAL_ENC_TIMER_ADJUST_DIR_SUB)*/
    cs_uint8                tx_delta;   /*(default=0): TX timer adjust value;range:0x0~0xff*/
    cs_uint16               timer_hi16; /*(default=0): high 16bit of aes timer; range should be 0x0~0xffff */ 
    cs_uint32               jaes_timer; /*(default=0):JAES timer, only used when the alg is SPECIAL AES.*/
}cs_aal_enc_timer_cfg_t;


typedef union {
    struct {
        cs_uint32  rx_dir     : 1;
        cs_uint32  rx_delta   : 1;
        cs_uint32  tx_dir     : 1;
        cs_uint32  tx_delta   : 1;
        cs_uint32  timer_hi16 : 1;
        cs_uint32  jaes_timer : 1;
        cs_uint32  rsvd       : 26;
    } s;
    cs_uint32 u32;
}cs_aal_enc_timer_msk_t;


typedef struct {
    cs_uint32 aes_timer;
    cs_uint32 request_cnt;
    cs_uint32 local_cnt;
    cs_uint32 local_timer;
    cs_uint32 crc_err_cnt_even_key;
    cs_uint32 crc_good_cnt_even_key;
    cs_uint32 crc_err_cnt_odd_key;
    cs_uint32 crc_good_cnt_odd_key;
    cs_uint8  key_idx_tog_cnt;
    cs_uint8  rx_index;
    cs_uint8  tx_index;
    cs_uint8  rx_aes_flag;
    cs_uint8  rcc_b0;   
}cs_aal_enc_cnt_t;


/**************************************************************************/
/* $rtn_hdr_start onu_aal_enc_cfg_set                                     */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_enc_cfg_set(
    CS_IN cs_aal_enc_msk_t msk,
    CS_IN cs_aal_enc_cfg_t *cfg
);
/* INPUTS     :   enc_cfg and mask defined in aal_enc.h                   */
/*                                                                        */
/* OUTPUTS    :   None                                                    */
/*    o---                                                                */ 
/* RETURNS    :   CS_E_OK or CS_E_PARAM defined in cs_types.h             */
/* DESCRIPTION:                                                           */
/*    Config the encryption                                               */
/* $rtn_hdr_end                                                           */
/**************************************************************************/ 


/**************************************************************************/
/* $rtn_hdr_start onu_aal_enc_cfg_get                                     */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */

cs_status aal_enc_cfg_get(
    CS_OUT cs_aal_enc_cfg_t *cfg
); 
/* INPUTS     :   enc_cfg and mask defined in aal_enc.h                   */
/*                                                                        */
/* OUTPUTS    :   None                                                    */
/*    o---                                                                */ 
/* RETURNS    :   CS_E_OK or CS_E_PARAM defined in cs_types.h             */
/* DESCRIPTION:                                                           */
/*    Get the encryption configuration;                                   */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start onu_aal_enc_tx_cfg_set                                  */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_enc_tx_cfg_set(
    CS_IN cs_aal_enc_tx_msk_t msk,
    CS_OUT cs_aal_enc_tx_cfg_t *cfg
);
/* INPUTS     :   cfg:enc_tx_cfg and mask defined in aal_enc.h            */
/*                                                                        */
/* OUTPUTS    :   None                                                    */
/*    o---                                                                */ 
/* RETURNS    :   CS_E_OK or CS_E_PARAM defined in cs_types.h             */
/* DESCRIPTION:                                                           */
/*    Config the tx encryption;                                           */
/* $rtn_hdr_end                                                           */
/**************************************************************************/



/**************************************************************************/
/* $rtn_hdr_start onu_aal_enc_tx_cfg_get                                  */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */


cs_status aal_enc_tx_cfg_get(
    CS_OUT cs_aal_enc_tx_cfg_t *cfg
);
/* INPUTS     :   None                                                    */
/*                                                                        */
/* OUTPUTS    :   cfg:enc_tx_cfg and mask defined in aal_enc.h            */
/*    o---                                                                */ 
/* RETURNS    :   CS_E_OK or CS_E_PARAM defined in cs_types.h             */
/* DESCRIPTION:                                                           */
/*    Get the tx encryption configuration;                                */
/* $rtn_hdr_end                                                           */
/**************************************************************************/



/**************************************************************************/
/* $rtn_hdr_start onu_aal_enc_timer_cfg_set                               */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_enc_timer_cfg_set(
    CS_IN cs_aal_enc_timer_msk_t msk,
    CS_IN cs_aal_enc_timer_cfg_t *cfg
);
/* INPUTS     :   enc_timer_cfg and mask defined in aal_enc.h             */
/*                                                                        */
/* OUTPUTS    :   None                                                    */
/*    o---                                                                */ 
/* RETURNS    :   CS_E_OK or CS_E_PARAM defined in cs_types.h             */
/* DESCRIPTION:                                                           */
/*    Config the encryption timer;                                        */
/* $rtn_hdr_end                                                           */
/**************************************************************************/



/**************************************************************************/
/* $rtn_hdr_start onu_aal_enc_timer_cfg_get                               */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_enc_timer_cfg_get(
    CS_OUT cs_aal_enc_timer_cfg_t *cfg
);
/* INPUTS     :   None                                                    */
/*                                                                        */
/* OUTPUTS    :   cfg: cs_aal_enc_timer_cfg_t defined in aal_enc.h;       */
/*    o---                                                                */ 
/* RETURNS    :   CS_E_OK or CS_E_PARAM defined in cs_types.h             */
/* DESCRIPTION:                                                           */
/*    Get the encryption timer configration;                              */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start onu_aal_enc_key_set                                     */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_enc_key_set(
    CS_IN cs_aal_enc_key_idx_t index,
    CS_IN cs_aal_enc_key_t *key
);
/* INPUTS     :   id: cs_aal_enc_key_idx_t defined in aal_enc.h;        */
/*                key: cs_aal_enc_key_t defined in aal_enc.h;             */
/* OUTPUTS    :   None                                                    */
/*    o---                                                                */ 
/* RETURNS    :   CS_E_OK or CS_E_PARAM defined in cs_types.h             */
/* DESCRIPTION:                                                           */
/*    Set the encryption key;                                             */
/* $rtn_hdr_end                                                           */
/**************************************************************************/



/**************************************************************************/
/* $rtn_hdr_start onu_aal_enc_key_get                                     */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_enc_key_get(
    CS_IN cs_aal_enc_key_idx_t index,
    CS_OUT cs_aal_enc_key_t *key
);
/* INPUTS     :   id: cs_aal_enc_key_idx_t defined in aal_enc.h;        */
/*                                                                        */
/* OUTPUTS    :   key: cs_aal_enc_key_t defined in aal_enc.h;             */
/*    o---                                                                */ 
/* RETURNS    :   CS_E_OK or CS_E_PARAM defined in cs_types.h             */
/* DESCRIPTION:                                                           */
/*    Get ket from key mem address(set by SW);                            */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start onu_aal_enc_key_hw_get                                  */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_enc_key_hw_get(
    CS_IN cs_aal_enc_key_idx_t index,
    CS_OUT cs_aal_enc_key_t *key
);
/* INPUTS     :   id:cs_aal_enc_key_idx_t defined in aal_enc.h          */
/*                                                                        */
/* OUTPUTS    :   key:cs_aal_enc_key_t defined in aal_enc.h               */
/*    o---                                                                */ 
/* RETURNS    :   CS_E_OK or CS_E_PARAM defined in cs_types.h             */
/* DESCRIPTION:                                                           */
/*    Get key which is generated by HW;                                   */
/* $rtn_hdr_end                                                           */
/**************************************************************************/


/**************************************************************************/
/* $rtn_hdr_start onu_aal_enc_cnt_get                                     */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */

cs_status aal_enc_cnt_get(
    CS_OUT cs_aal_enc_cnt_t *cnt
);
/* INPUTS     :   NA                                                      */
/*                                                                        */
/* OUTPUTS    :   cs_aal_enc_cnt_t defined in aal_enc.h                   */
/*    o---                                                                */ 
/* RETURNS    :   CS_E_OK or CS_E_PARAM defined in cs_types.h             */
/* DESCRIPTION:                                                           */
/*    Get the encryption counter;                                         */
/* $rtn_hdr_end                                                           */
/**************************************************************************/

/**************************************************************************/
/* $rtn_hdr_start onu_aal_enc_cnt_clr                                     */
/* CATEGORY   :   Device                                                  */
/* ACCESS     :   Public                                                  */
/* BLOCK      :   AAL                                                     */
/* CHIP       :   LynxD                                                   */
cs_status aal_enc_cnt_clr( void );
/* INPUTS     :   NA                                                      */
/*                                                                        */
/* OUTPUTS    :   None                                                    */
/*    o---                                                                */ 
/* RETURNS    :   CS_E_OK or CS_E_PARAM defined in cs_types.h             */
/* DESCRIPTION:                                                           */
/*    Clear the encryption counter of received encrypted packets;         */
/* $rtn_hdr_end                                                           */
/**************************************************************************/

cs_status enc_init(void);

inline cs_uint32 aal_local_timer_get(void);
inline cs_boolean aal_enc_jaes_clear_cond(void);
inline void aal_enc_jaes_timer_clear(void);

#endif /* __AAL_ENC_H__ */


