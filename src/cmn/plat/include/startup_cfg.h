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

#ifndef _STARTUP_CFG_H_
#define _STARTUP_CFG_H_
#include "cs_types.h"

#define STARTUP_CFG_LEN     1024 
#define HAVE_SCFG_PROTECTION
typedef enum {
    /*MACRO_NAME                  MACRO_ID      Length             DATA_TYPE     LEVEL          */
    CFG_ID_MAC_ADDRESS            = 0,          /*6 Bytes*/        /*BUFFER*/    /*System Level*/
    CFG_ID_SDRAM_BANK_CNT         = 1,          /*1 Bytes*/        /*CHAR*/      /*System Level*/
    CFG_ID_SDRAM_ROW_CNT          = 2,          /*1 Bytes*/        /*CHAR*/      /*System Level*/
    CFG_ID_SDRAM_PAGE_SIZE        = 3,          /*1 Bytes*/        /*CHAR*/      /*System Level*/
    CFG_ID_BRINGUP_MODE           = 4,          /*1 Bytes*/        /*CHAR*/      /*System Level*/
    CFG_ID_ONU_UNI_CFG            = 5,          /*1 Bytes*/        /*CHAR*/      /*System Level*/
    CFG_ID_MDIO_DEVID             = 6,          /*2 Bytes*/        /*BUFFER*/    /*System Level*/
    CFG_ID_GE_CLK_SEL             = 7,          /*1 Bytes*/        /*CHAR*/      /*System Level*/
    CFG_ID_GPIO_PIN0              = 8,          /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN1              = 9,          /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN2              = 10,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN3              = 11,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN4              = 12,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN5              = 13,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN6              = 14,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN7              = 15,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN8              = 16,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN9              = 17,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN10             = 18,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN11             = 19,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN12             = 20,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN13             = 21,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN14             = 22,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_GPIO_PIN15             = 23,         /*32Bytes*/        /*BUFFER*/    /*APP Level      */

    CFG_ID_MPCP_TIMEOUT           = 101,        /*1 Bytes*/        /*CHAR*/      /*APP Level      */
    CFG_ID_PON_LASER_POLARITY     = 102,        /*1 Bytes*/        /*CHAR*/      /*APP Level      */
    CFG_ID_CUSTOM_MODE_SEL        = 103,        /*1 Bytes*/        /*CHAR*/      /*APP Level      */
    CFG_ID_DSP_BUFF_MODE          = 104,        /*1 Bytes*/        /*CHAR*/      /*APP Level      */
    CFG_ID_SWITCH_PORT_NUM        = 105,        /*1 Bytes*/        /*CHAR*/      /*APP Level      */
    CFG_ID_8021x_USERNAME         = 106,        /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_8021x_PWD              = 107,        /*32Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_OAM_VERSION            = 108,        /*1 Bytes*/        /*CHAR*/      /*APP Level      */
    CFG_ID_VENDOR_ID              = 109,        /*4 Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_ONU_MOD_ID             = 110,        /*16Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_HW_VERSION             = 111,        /*2 Bytes*/        /*SHORT*/     /*APP Level      */
    CFG_ID_YEAR                   = 112,        /*2 Bytes*/        /*SHORT*/     /*APP Level      */
    CFG_ID_WEEK                   = 113,        /*2 Bytes*/        /*SHORT*/     /*APP Level      */
    CFG_ID_SERIAL_NUM             = 114,        /*6 Bytes*/        /*BUFFER*/    /*APP Level      */
    CFG_ID_RESOURECE_MODE         = 115,        /*1 Bytes*/        /*CHAR*/      /*APP Level      */
    CFG_ID_WATCHDOG_TIMER         = 116,        /*2 Bytes*/        /*SHORT*/     /*APP Level      */
    CFG_ID_PARTCODE               = 119,        /*1 Bytes*/        /*CHAR*/      /*APP Level      */
    CFG_ID_MONTH                  = 120,        /*2 Bytes*/        /*SHORT*/     /*APP Level      */
    CFG_ID_DAY                    = 121,        /*2 Bytes*/        /*SHORT*/     /*APP Level      */
    CFG_ID_SWITCH_TYPE            = 122,        /*1 Bytes*/        /*CHAR*/      /*APP Level      */
    CFG_ID_PON_TX_TYPE            = 123,        /*1 Bytes*/        /*CHAR*/      /*APP Level      */

    
    CFG_ID_LASER_ALWAYS_ON        = 201,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_PON_ADMIN_STATUS       = 202,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_KEEP_SILENCE_TIME      = 203,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_WINDOW_FIT             = 204,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_ZQT_MODE               = 205,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_LASER_ON_TIME          = 206,        /*2 Bytes*/        /*SHORT*/     /*USER DATA      */
    CFG_ID_LASER_OFF_TIME         = 207,        /*2 Bytes*/        /*SHORT*/     /*USER DATA      */
    CFG_ID_UNI_NEG_MODE           = 208,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_UNI_ADMIN_STATUS       = 209,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_OAM_BYPASS_MODE        = 210,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_CMODE_UPGRADE_MODE     = 211,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_8021X_TUNL_MODE        = 212,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_ONU_CTRL_VLAN          = 213,        /*2 Bytes*/        /*SHORT*/     /*USER DATA      */
    CFG_ID_IGMP_VLAN_LRN_MODE     = 214,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_DYINGGASP_TRIG_MODE    = 215,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_DYING_GASP_NUM         = 216,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_AES_MODE               = 217,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_FDB_LRU_BEHAVIOR       = 218,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_LOOPBACK_DEPTH         = 219,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_8021X_TIMEOUT          = 220,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_EAPOL_RETRY_TIMES      = 221,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_8021X_HELD_TIMER       = 222,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_DATA_RESTORE           = 223,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_ONU_MAX_OAM_PDU        = 224,        /*2 Bytes*/        /*SHORT*/     /*USER DATE      */
    CFG_ID_ONU_POWERSAVING_MODE   = 225,        /*1 Bytes*/        /*CHAR*/      /*USER DATE      */
    CFG_ID_PHY_EEE_MODE           = 226,        /*1 Bytes*/        /*CHAR*/      /*USER DATE      */
    CFG_ID_SERDES_SWCTRL_TX       = 227,        /*1 Bytes*/        /*CHAR*/      /*USER DATE      */
    CFG_ID_SERDES_EMP_EN_TX       = 228,        /*1 Bytes*/        /*CHAR*/      /*USER DATE      */
    CFG_ID_ONU_PWS_TRX_POWERON_TIME = 229,      /*2 Bytes*/        /*SHORT*/     /*USER DATE      */
    CFG_ID_OUI                    = 230,        /*3 Bytes*/        /*BUFFER*/    /*USER DATA      */
    CFG_ID_LASER_DELAY            = 231,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_UPGRADE_MODE           = 232,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_BAK_BATTERY_MODE       = 233,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */
    CFG_ID_CUC_OAM_MODE           = 234,        /*1 Bytes*/        /*CHAR*/      /*USER DATA      */

} CFG_ID_TYPE;

#define SCFG_MAX_CNT 0xFE

typedef enum {
    FIELD_TYPE_CHAR,
    FIELD_TYPE_SHORT,
    FIELD_TYPE_INT,
    FIELD_TYPE_BUFF,
    FIELD_TYPE_STRING,
} FIELD_TYPE_ENUM;

typedef enum {
    SCFG_INVALID_VERSION = 0,
    SCFG_KT_VERSION      = 1,
    SCFG_NTT_VERSION     = 2,
    SCFG_CTC_VERSION     = 3,
    SCFG_MDU_VERSION     = 4,
} SCFG_VERSION_ENUM;

#define SCFG_R  0 /*Read-only mode, Need to reboot when field value is changed*/
#define SCFG_W  1 /*Read-Write mode, It is no need to reboot when fied value is changed*/

typedef struct {
    unsigned short  type;
    unsigned short  value_type;
    unsigned char   desc[32];
    unsigned int    value_max_len;
    unsigned int    mode;
    unsigned int    dflt_val;
    unsigned char   dflt_buf[32];
    unsigned char   help_str[128];
    short           range_low;
    short           range_high;
} cfg_field_t;

void startup_config_init();
cs_status startup_config_write(cs_uint32 type, cs_uint32 len, cs_uint8 *buf);
cs_status startup_config_read(cs_uint32 type, cs_uint32 len, cs_uint8 *buf);
int startup_cfg_cmd_proc(int argc, char **argv);
void startup_cfg_enc_enable(int flag);
void startup_cfg_autosave_enable(int flag);
cs_uint32 startup_config_backup_index(cs_uint8 update);
cs_status startup_config_update();
#endif

