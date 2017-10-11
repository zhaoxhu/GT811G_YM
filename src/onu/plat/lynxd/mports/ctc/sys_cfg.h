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


#ifndef _SYS_CFG_H_
#define _SYS_CFG_H_
#include "cs_types.h"
#include "startup_cfg.h"
#include "gpio.h"
#include "app_gpio.h"
#include "packet.h"

#ifdef SYS_SCFG_PARAM
unsigned int gScfgVer = SCFG_CTC_VERSION;
cfg_field_t glb_field[] =
{
    /*FIELD-TYPE                  VALUE-TYPE           DESC                                   LEN      MODE      INT-VAL     BUF-VAL*/
    {CFG_ID_MAC_ADDRESS         , FIELD_TYPE_BUFF    , "mac address"                        ,  6     , SCFG_R   , 0 ,         {0x00,0x13,0x25,0x00,0x00,0x01},
            /*HELP STRING*/ "mac - system mac(eg. 00:13:25:00:00:01)",-1,-1},
    {CFG_ID_SDRAM_BANK_CNT      , FIELD_TYPE_CHAR    , "SDRAM bank count"                   ,  1     , SCFG_R   , 0 ,         {0},
            /*HELP STRING*/ "0 - 4 bank memory\n1 - 8 bank memory",0,1},
    {CFG_ID_SDRAM_ROW_CNT       , FIELD_TYPE_CHAR    , "SDRAM row count"                    ,  1     , SCFG_R   , 1 ,         {0},
            /*HELP STRING*/ "0 - 2K\n1 - 4K\n2 - 8K\n3 - 16K",0,3},
    {CFG_ID_SDRAM_PAGE_SIZE     , FIELD_TYPE_CHAR    , "SDRAM page size"                    ,  1     , SCFG_R   , 3 ,         {0},
            /*HELP STRING*/ "0 - 512 page size\n1 - 1K page size\n2 - 2K page size\n3 - 256 page size",0,3},
    {CFG_ID_BRINGUP_MODE        , FIELD_TYPE_CHAR    , "Bring up mode"                      ,  1     , SCFG_R   , 1 ,         {0},
            /*HELP STRING*/ "0- Normal bringup\n1- Fast bringup",0,1},
    {CFG_ID_ONU_UNI_CFG         , FIELD_TYPE_CHAR    , "External device"                    ,  1     , SCFG_R   , 0 ,         {1},
            /*HELP STRING*/ "0 - Connected with PHY\n1 - Connected with Switch\n2 - No UNI port",0,2},
    {CFG_ID_MDIO_DEVID          , FIELD_TYPE_BUFF    , "MDIO device ID"                     ,  2     , SCFG_R   , 0 ,         {0x03,0x01},
            /*HELP STRING*/ "MDIO device ID(eg. 01:02)",-1,-1},
    {CFG_ID_GE_CLK_SEL          , FIELD_TYPE_CHAR    , "GE clock selection"                 ,  1     , SCFG_R   , 0 ,         {0},
            /*HELP STRING*/ "0 - Data/Txclk inline\n1 - Internal delay of TxClk with respect to TxData",0,1},
          
     {CFG_ID_GPIO_PIN0           , FIELD_TYPE_STRING  , "GPIO pin-0"                         ,  32    , SCFG_R   , 0 ,         {"TRIGGER.DYING_GASP.FALLING\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN1           , FIELD_TYPE_STRING  , "GPIO pin-1"                         ,  32    , SCFG_R   , 0 ,         {"INPUT\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN2           , FIELD_TYPE_STRING  , "GPIO pin-2"                         ,  32    , SCFG_R   , 0 ,         {"OUTPUT.1\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN3           , FIELD_TYPE_STRING  , "GPIO pin-3"                         ,  32    , SCFG_R   , 0 ,         {"INPUT\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN4           , FIELD_TYPE_STRING  , "GPIO pin-4"                         ,  32    , SCFG_R   , 0 ,         {"INPUT\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN5           , FIELD_TYPE_STRING  , "GPIO pin-5"                         ,  32    , SCFG_R   , 0 ,         {"INPUT\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN6           , FIELD_TYPE_STRING  , "GPIO pin-6"                         ,  32    , SCFG_R   , 0 ,         {"OUTPUT.1\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN7           , FIELD_TYPE_STRING  , "GPIO pin-7"                         ,  32    , SCFG_R   , 0 ,         {"INPUT\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN8           , FIELD_TYPE_STRING  , "GPIO pin-8"                         ,  32    , SCFG_R   , 0 ,         {"INPUT\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN9           , FIELD_TYPE_STRING  , "GPIO pin-9"                         ,  32    , SCFG_R   , 0 ,         {"INPUT\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN10          , FIELD_TYPE_STRING  , "GPIO pin-10"                        ,  32    , SCFG_R   , 0 ,         {"INPUT\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN11          , FIELD_TYPE_STRING  , "GPIO pin-11"                        ,  32    , SCFG_R   , 0 ,         {"INPUT\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN12          , FIELD_TYPE_STRING  , "GPIO pin-12"                        ,  32    , SCFG_R   , 0 ,         {"INPUT\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN13          , FIELD_TYPE_STRING  , "GPIO pin-13"                        ,  32    , SCFG_R   , 0 ,         {"OUTPUT.1\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN14          , FIELD_TYPE_STRING  , "GPIO pin-14"                        ,  32    , SCFG_R   , 0 ,         {"LED.LOS.RED.ON.0\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
    {CFG_ID_GPIO_PIN15          , FIELD_TYPE_STRING  , "GPIO pin-15"                        ,  32    , SCFG_R   , 0 ,         {"LED.PON.GREEN.OFF.0\0"},
            /*HELP STRING*/ "GPIO pin description",-1,-1},
               
    {CFG_ID_MPCP_TIMEOUT        , FIELD_TYPE_CHAR    , "MPCP timeout"                       ,  1     , SCFG_R   , 20 ,        {0},
            /*HELP STRING*/ "1-255, measured in 10ms",1,255},
    {CFG_ID_PON_LASER_POLARITY  , FIELD_TYPE_CHAR    , "PON port laser polarity"            ,  1     , SCFG_R   , 0 ,         {0},
            /*HELP STRING*/ "(tx_burst_polarity:a sig_loss_polarity:b)\n0 - a=0,b=0\n1 - a=1,b=0\n2 - a=0,b=0\n3 - a=1,b=1",0,3},
    {CFG_ID_SWITCH_PORT_NUM     , FIELD_TYPE_CHAR    , "Swith port number"                  ,  1     , SCFG_R   , 4 ,         {0},
            /*HELP STRING*/ "Indicates the external switch port number",1,8},
    {CFG_ID_OAM_VERSION         , FIELD_TYPE_CHAR    , "OAM version"                        ,  1     , SCFG_R   , 0 ,         {0},
            /*HELP STRING*/ "0x00 - Standard\n0x01 - 802.3ah Draft 2.0",0,1},
    {CFG_ID_VENDOR_ID           , FIELD_TYPE_STRING  , "Vendor ID"                          ,  4     , SCFG_R   , 0 ,         {0},
            /*HELP STRING*/ "Vendor ID should be a string, Eg. 'CTC'",-1,-1},
    {CFG_ID_ONU_MOD_ID          , FIELD_TYPE_STRING  , "ONU system model ID"                ,  4     , SCFG_W   , 0 ,         {0},
            /*HELP STRING*/ "The module ID, it is a string",-1,-1},
    {CFG_ID_HW_VERSION          , FIELD_TYPE_STRING  , "Hardware Version"                   ,  8     , SCFG_W   , 0 ,         {0},
            /*HELP STRING*/ "Hardware version, it is a string",-1,-1},
    {CFG_ID_YEAR                , FIELD_TYPE_SHORT   , "Year"                               ,  2     , SCFG_W   , 0 ,         {0},
            /*HELP STRING*/ "The low two digits of the production year (00 ¨C 99)",0,99},
    {CFG_ID_WEEK                , FIELD_TYPE_SHORT   , "Week"                               ,  2     , SCFG_W   , 0 ,         {0},
            /*HELP STRING*/ "The production week (01 ¨C 52)",1,52},
    {CFG_ID_SERIAL_NUM          , FIELD_TYPE_STRING  , "Serial Number"                      ,  6     , SCFG_W   , 0 ,         {0},
            /*HELP STRING*/ "The ONU system serial number, it shall be a string.",-1,-1},
    {CFG_ID_RESOURECE_MODE      , FIELD_TYPE_CHAR    , "Resource Mode"                      ,  1     , SCFG_W   , 0 ,         {0},
            /*HELP STRING*/ "Resource reservation mode.",0,255},
    {CFG_ID_WATCHDOG_TIMER      , FIELD_TYPE_SHORT   , "Watch Dog Timer"                    ,  2     , SCFG_R   , 300 ,       {0},
            /*HELP STRING*/ "The ONU system serial number, it shall be a string.",1,65535},        

    {CFG_ID_SWITCH_TYPE      , FIELD_TYPE_CHAR    , "Switch type"                           ,  1     , SCFG_W   , 0 ,         {0},
            /*HELP STRING*/ "Switch type: 0-RTL8305, 1-RTL8365, default is 0. others reserved",0,255},
	{CFG_ID_LASER_ALWAYS_ON     , FIELD_TYPE_CHAR    , "Laser always on/off"                ,  1     , SCFG_R   , 0 ,         {0},
            /*HELP STRING*/ "0--Disable\n1--Enable ",0,1},
    {CFG_ID_PON_ADMIN_STATUS    , FIELD_TYPE_CHAR    , "Pon port admin status"              ,  1     , SCFG_R   , 1 ,         {0},
            /*HELP STRING*/ "0 - DOWN\n1 - UP",0,1},
    {CFG_ID_KEEP_SILENCE_TIME   , FIELD_TYPE_CHAR    , "Keep silence time"                  ,  1     , SCFG_W   , 60 ,        {0},
            /*HELP STRING*/ "0-255 seconds, default is 60s.",0,255},
    {CFG_ID_LASER_ON_TIME       , FIELD_TYPE_SHORT   , "Laser on time"                      ,  2     , SCFG_R   , 32 ,        {0},
            /*HELP STRING*/ "The laser on time in 1TQs,default is 0x20",-1,-1},
    {CFG_ID_LASER_OFF_TIME      , FIELD_TYPE_SHORT   , "Laser off time"                     ,  2     , SCFG_R   , 32 ,        {0},
            /*HELP STRING*/ "The laser off time in 1TQs,default is 0x20",-1,-1},
    {CFG_ID_UNI_NEG_MODE        , FIELD_TYPE_CHAR    , "UNI port negotiation mode"          ,  1     , SCFG_R   , 0 ,         {0},
            /*HELP STRING*/ "0x00 - Auto neg\n0x01 - 10-half\n0x02 - 10-full\n0x03 - 100-half\n0x04 - 100-full\n0x05 - 1000-full",0,5},
    {CFG_ID_UNI_ADMIN_STATUS    , FIELD_TYPE_CHAR    , "UNI port admin status"              ,  1     , SCFG_R   , 1 ,         {0},
            /*HELP STRING*/ "0 - DOWN\n1 - UP",0,1},
    {CFG_ID_IGMP_VLAN_LRN_MODE  , FIELD_TYPE_CHAR    , "IGMP vlan learning mode"            ,  1     , SCFG_W   , 1 ,         {0},
            /*HELP STRING*/ "0 - Disable\n1- Enable .",0,1},
    {CFG_ID_DYINGGASP_TRIG_MODE , FIELD_TYPE_CHAR    , "Dying gasp trigger mode"            ,  1     , SCFG_R   , 1 ,         {0},
            /*HELP STRING*/ "0 -  Rise edge trigger\n1 -  Falling edge trigger",0,1},
    {CFG_ID_DYING_GASP_NUM      , FIELD_TYPE_CHAR    , "Dying gasp number"                  ,  1     , SCFG_W   , 3 ,         {0},
            /*HELP STRING*/ "Dying gasp number, default is 1",1,16},
    {CFG_ID_FDB_LRU_BEHAVIOR    , FIELD_TYPE_CHAR    , "FDB LRU behavior"                   ,  1     , SCFG_R   , 2 ,         {0},
            /*HELP STRING*/ "0 - drop\n1 - pass\n2 - pass and learn\n3 - not valid",0,3},
    {CFG_ID_LOOPBACK_DEPTH      , FIELD_TYPE_CHAR    , "Loopback depth"                     ,  1     , SCFG_W   , 0 ,         {0},
            /*HELP STRING*/ "0 -- PON port\n1 -- UNI PHY",0,1},
    {CFG_ID_DATA_RESTORE        , FIELD_TYPE_CHAR    , "user data restore flag"             ,  1     , SCFG_R   , 0 ,         {0},
            /*HELP STRING*/ "The flag control whether enable user date restore",0,1},
    {CFG_ID_OUI                 , FIELD_TYPE_BUFF    , "OUI used by CTC OAM messages"       ,  3     , SCFG_R   , 0 ,         {0x11,0x11,0x11},
            /*HELP STRING*/ "OUI used by CTC OAM messages(Eg. 11:22:33)",-1,-1},
    {CFG_ID_UPGRADE_MODE        , FIELD_TYPE_CHAR    , "CTC upgrade mode"                   ,  1     , SCFG_R   , 1 ,         {0},
            /*HELP STRING*/ "0 - Enable\n1 - Disable",0,1},    
    {CFG_ID_BAK_BATTERY_MODE    , FIELD_TYPE_CHAR    , "Bak battery mode"                   ,  1     , SCFG_W   , 1 ,         {0},
            /*HELP STRING*/ "0 - Don't support\n1 - Support",0,1},
};
#else
extern unsigned int gScfgVer;
extern cfg_field_t glb_field[];
#endif

#if 0
#ifdef SYS_GPIO_PARAM

cs_gpio_function_t g_gpio_profile[] = {
    /*USED-PIN             POLARITY&INIT         MODE&DESC       DEF-STATUS*/
    {GPIO_PIN0            ,0                    ,GPIO_TRIGGER   ,0,
                           onu_dying_gasp_init  ,"dy_gasp"},
    {GPIO_PIN1            ,0                    ,GPIO_OUTPUT    ,0,
                           onu_peri_reset_init  ,"bd_rst"},
    {GPIO_PIN5|GPIO_PIN6  ,0                    ,GPIO_OUTPUT    ,GPIO_PIN5|GPIO_PIN6,
                           NULL                 ,"laser_pwr"},
    {GPIO_PIN10|GPIO_PIN11,GPIO_PIN10|GPIO_PIN11,GPIO_OUTPUT    ,GPIO_PIN11,
                           onu_pwr_led_init     ,"pwr_led"},
    {GPIO_PIN12           ,GPIO_PIN12           ,GPIO_OUTPUT    ,GPIO_PIN12,
                           onu_auth_led_init    ,"ath_led"},
    {GPIO_PIN14|GPIO_PIN15,GPIO_PIN14|GPIO_PIN15,GPIO_OUTPUT    ,GPIO_PIN14|GPIO_PIN15,  
                           onu_pon_led_init     ,"pon_led"}
};
#else
extern cs_gpio_function_t g_gpio_profile[];
#endif
#endif

#ifdef SYS_SPCL_PKT_CFG
cs_uint8 g_spcl_pkt_def_state[CS_PKT_TYPE_NUM][2] =
{
    { DST_FE, DST_FE }, /* CS_PKT_BPDU */
    { DST_FE, DST_FE }, /* CS_PKT_8021X */
    { DST_FE, DST_FE }, /* CS_PKT_GMP */
    { DST_CPU, DST_CPU }, /* CS_PKT_ARP */
    { DST_CPU, DST_CPU }, /* CS_PKT_OAM */
    { DST_FE, DST_FE }, /* CS_PKT_MPCP */
    { DST_FE, DST_FE }, /* CS_PKT_DHCP */
    { DST_CPU, DST_CPU }, /* CS_PKT_IROS */
    { DST_FE, DST_FE }, /* CS_PKT_PPPOE_DIS */
    { DST_FE, DST_FE }, /* CS_PKT_PPPOE_SES */
    { DST_CPU, DST_CPU }, /* CS_PKT_IP */
    { DST_FE, DST_FE }, /* CS_PKT_NDP */
    { DST_CPU, DST_FE }, /* CS_PKT_LOOPDETECT */
    { DST_CPU, DST_CPU }  /* CS_PKT_MYMAC */
};
#else
extern cs_uint8 g_spcl_pkt_def_state[CS_PKT_TYPE_NUM][2];
#endif

/* ONU basic information */

#define ONU_CLIENT_PRODUCT_MODEL    "CORTINA LYNXD"
#define ONU_SW_VERSION              "V1.0.0"
#define ONU_CHIPSET_VENDOR_ID       "F2"
#define ONU_CHIPSET_MODE            "8030"
#define ONU_CHIPSET_VER             "A0"
#define ONU_CHIPSET_MADE_DATE       "11/01/29"


#endif

