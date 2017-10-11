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

    This file is used for global configuration and aal initiation entry driver code
*/

#ifndef __AAL_SYS_H__
#define __AAL_SYS_H__
#include "cs_types.h"

typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd                    :  14 ; 
    cs_uint32 reset_pcm                :  1 ; /* 1 -- Enable PCM module reset */
    cs_uint32 reset_arm                :  1 ; /* 1 -- Enable ARM module reset */
    cs_uint32 reset_arm_peri           :  1 ; /* 1 -- Enable ARM Peripheral module reset*/  
    cs_uint32 reset_sdram              :  1 ; /* 1 -- Enable SDRAM controller module reset */
    cs_uint32 reset_pll                :  1 ; /* 1 -- Enable PLL module reset */
    cs_uint32 reset_te                 :  1 ; /* 1 -- Enable TE module reset */
    cs_uint32 rsrvd1                   :  1 ; 
    cs_uint32 reset_mii0               :  1 ; /* 1 -- Enable MII0 port reset */
    cs_uint32 reset_ma                 :  1 ; /* 1 -- Enable Management Access module reset */
    cs_uint32 reset_fe                 :  1 ; /* 1 -- Enable Forwarding Engine reset */
    cs_uint32 reset_bm                 :  1 ; /* 1 -- Enable Buffer Manager reset */
    cs_uint32 reset_ge_tx              :  1 ; /* 1 -- Enable GE port Transmit path only reset*/
    cs_uint32 reset_ge_rx              :  1 ; /* 1 -- Enable GE port Receive path only reset*/
    cs_uint32 reset_ge                 :  1 ; /* 1 -- Enable GE port reset*/
    cs_uint32 reset_serdes             :  1 ; /* 1 -- Enable SerDes reset*/
    cs_uint32 reset_pon_tx             :  1 ; /* 1 -- Enable PON port Transmit path only reset*/
    cs_uint32 reset_pon_rx             :  1 ; /* 1 -- Enable PON port Receive path only reset*/
    cs_uint32 reset_pon                :  1 ; /* 1 -- Enable PON port reset*/
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 reset_pon                :  1 ; /* 1 -- Enable PON port reset*/
    cs_uint32 reset_pon_rx             :  1 ; /* 1 -- Enable PON port Receive path only reset*/
    cs_uint32 reset_pon_tx             :  1 ; /* 1 -- Enable PON port Transmit path only reset*/
    cs_uint32 reset_serdes             :  1 ; /* 1 -- Enable SerDes reset*/
    cs_uint32 reset_ge                 :  1 ; /* 1 -- Enable GE port reset*/
    cs_uint32 reset_ge_rx              :  1 ; /* 1 -- Enable GE port Receive path only reset*/
    cs_uint32 reset_ge_tx              :  1 ; /* 1 -- Enable GE port Transmit path only reset*/
    cs_uint32 reset_bm                 :  1 ; /* 1 -- Enable Buffer Manager reset */
    cs_uint32 reset_fe                 :  1 ; /* 1 -- Enable Forwarding Engine reset */
    cs_uint32 reset_ma                 :  1 ; /* 1 -- Enable Management Access module reset */
    cs_uint32 reset_mii0               :  1 ; /* 1 -- Enable MII0 port reset */
    cs_uint32 rsrvd1                   :  1 ; 
    cs_uint32 reset_te                 :  1 ; /* 1 -- Enable TE module reset */
    cs_uint32 reset_pll                :  1 ; /* 1 -- Enable PLL module reset */
    cs_uint32 reset_sdram              :  1 ; /* 1 -- Enable SDRAM controller module reset */
    cs_uint32 reset_arm_peri           :  1 ; /* 1 -- Enable ARM Peripheral module reset*/  
    cs_uint32 reset_arm                :  1 ; /* 1 -- Enable ARM module reset */
    cs_uint32 reset_pcm                :  1 ; /* 1 -- Enable PCM module reset */
    cs_uint32 rsrvd                    :  14 ; 
#   endif
  } s ;
  cs_uint32     u32 ;
} cs_aal_module_reset_mask_t;

typedef struct {
    cs_uint32 override_en                 :  1 ; /* 1 -- Configuration of correponding mode control bits in this structure overrides configuration mode through bootstrap option */
    cs_uint32 share_pin_sel4              :  1 ; /* UART4, GPIO0, GPIO1 share pin select.*/
                                                 /* 1 -- UART4 port select for GPIO0 - RX_RE, GPIO1 - UART1_DE; */
                                                 /* 0 -- Use as GPIO0, 1 */
    cs_uint32 share_pin_sel3              :  1 ; /* UART3, GPIO2, GPIO3, PCM_TXD, PCM_RXD share pin select.*/
                                                 /* 1 -- UART3 port select for GPIO2 - RX_RE, GPIO3 - UART1_DE, PCM_RXD - RXD, GPIO9 -PCM_TXD */
                                                 /* 0 -- Use as GPIO2, 3, PCM_TXD, PCM_RXD */
    cs_uint32 share_pin_sel2              :  1 ; /* UART2, GPIO4, GPIO5, PCM_CLK, PCM_FSYNC share pin select.*/
                                                 /* 1 -- UART1 port select for GPIO4 - RX_RE, GPIO5 - UART1_DE, PCM_FSYNC - RXD, GPIO9 - PCM_CLK; */
                                                 /* 0 -- Use as GPIO4, 5, PCM_CLK, PCM_FSYNC */
    cs_uint32 share_pin_sel1              :  1 ; /* UART1, GPIO6, GPIO7, GPIO8, GPIO9 share pin select.*/
                                                 /* 1 -- UART1 port select for GPIO0 - RX_RE, GPIO1 - UART1_DE; */
                                                 /* 0 -- Use as GPIO6, 7, 8, 9 */
    cs_uint32 ptp_1pps_sel                :  1 ; /* 1 -- UART4 RXD select; 0 - PTP 1PPS pulse output*/
    cs_uint32 sdm_f_share                 :  1 ; /* Control whether SDRAM shares Flash/SRAM interface*/ 
                                                 /* 0 -- SDRAM uses dedicated pins*/
                                                 /* 1 -- SDRAM shares with Flash interface*/
    cs_uint32 mii0_if_mode                :  1 ; /* Control interface mode for MII0 port */
                                                 /* 0 -- MII mode */
                                                 /* 1-- RMII mode */
    cs_uint32 int_clk_sel                 :  2 ; /* Control ARM/AHB/core clock selection from internal PLL divider */
                                                  /* 0 -- 4:1 ratio. ARM runs at 500Mhz, core/AHB runs at 125Mhz.*/
                                                  /* 1 -- 8:1 ratio. ARM runs at 500Mhz, core/AHB runs at 62.5Mhz.*/
                                                  /* 2 -- 1:1 ratio. ARM/AHB/core clock run at 125Mhz.*/
                                                  /* 3 -- Reserved. */
     cs_uint32 xcpu_sel                    :  1 ; /* External CPU mode select ??*/
     cs_uint32 mem_addr_sel                :  2 ; /* Memory address selecti for system */
                                                  /* 0 -- 0x1000_0000 (SDRAM)*/
                                                  /* 1 -- 0x2000_0000 (ASYNC SRAM)*/
                                                  /* 2 -- 0x2c00_0000 (Internal SRAM)*/
                                                  /* 3 -- Reserved.   */
     cs_uint32 image_download_mode         :  2 ; /* Image_download_mode option*/
                                                  /* 0 -- UART download*/
                                                  /* 1 -- Flash download*/
                                                  /* 2 -- Serial Flash download*/
                                                  /* 3 -- MII download*/
     cs_uint32 ge_if_mode                  :  1 ; /* 0 -- UNI ports is RGII mode; 1 -- UNI port is MII mode*/
     cs_uint32 pon_if_mode                 :  1 ; /* 0 -- SGMII mode; 1 -- SerDes mod*/
     cs_uint32 boot_mode                   :  2 ; /* Boot_mde option*/
                                                  /*0 -- Boot fram parallel Flash*/
                                                  /*1 -- Boot from internal MROM*/
                                                  /*2 -- Boot from serial Flash*/
                                                  /*3 -- Reserved*/
    cs_uint32 rsrvd                       : 13 ; 
} cs_aal_pin_ctrl_cfg_t;

typedef union 
{
    struct 
    {
        cs_uint32 override_en                 :  1 ; 
        cs_uint32 share_pin_sel4              :  1 ; 
        cs_uint32 share_pin_sel3              :  1 ; 
        cs_uint32 share_pin_sel2              :  1 ; 
        cs_uint32 share_pin_sel1              :  1 ; 
        cs_uint32 ptp_1pps_sel                :  1 ; 
        cs_uint32 sdm_f_share                 :  1 ;
        cs_uint32 mii0_if_mode                :  1 ; 
        cs_uint32 int_clk_sel                 :  1 ; 
        cs_uint32 xcpu_sel                    :  1 ;
        cs_uint32 mem_addr_sel                :  1 ; 
        cs_uint32 image_download_mode         :  1 ; 
        cs_uint32 ge_if_mode                  :  1 ; 
        cs_uint32 pon_if_mode                 :  1 ; 
        cs_uint32 boot_mode                   :  1 ; 
        cs_uint32 rsrvd                       : 17 ; 
    } s;
    cs_uint32 u32;
}cs_aal_pin_ctrl_cfg_mask_t;

typedef struct {
    cs_uint32 ext_clk_sel                :  1 ; /* Control ARM/AHB/core clock source selection if the clock source comes from externalclock.*/
                                                /* 0 -- ARM/AHB/core all run from a clock source provided by internal PLL. */
                                                /*      The clock speed is determined by over_int_clk_sel(cs_aal_pin_ctrl_cfg_t) */
                                                /* 1 -- ARM runs on an externally provided 250Mhz clock, core/AHB runs at 125Mhz */
    cs_uint32 sdm_f_share                :  1 ; /* Control whether SDRAM shares Flash/SRAM interface*/ 
                                                /* 0 -- SDRAM uses dedicated pins*/
                                                /* 1 -- SDRAM shares with Flash interface*/
    cs_uint32 mii0_if_mode               :  1 ; /* Control interface mode for MII0 port */
                                                /* 0 -- MII mode */
                                                /* 1-- RMII mode */
    cs_uint32 int_clk_sel                :  2 ; /* Control ARM/AHB/core clock selection from internal PLL divider */
                                                 /* 0 -- 4:1 ratio. ARM runs at 500Mhz, core/AHB runs at 125Mhz.*/
                                                 /* 1 -- 8:1 ratio. ARM runs at 500Mhz, core/AHB runs at 62.5Mhz.*/
                                                 /* 2 -- 1:1 ratio. ARM/AHB/core clock run at 125Mhz.*/
                                                 /* 3 -- Reserved. */
                                                 /* Note: When ext_clk_sel is set, ARM will run at 250Mhz with core/AHB running at 125Mhz only*/
    cs_uint32 xcpu_sel                    :  1 ; /* External CPU mode select ??*/
    cs_uint32 mem_addr_sel                :  2 ; /* Memory address selecti for system */
                                                 /* 0 -- 0x1000_0000 (SDRAM)*/
                                                 /* 1 -- 0x2000_0000 (ASYNC SRAM)*/
                                                 /* 2 -- 0x2c00_0000 (Internal SRAM)*/
                                                 /* 3 -- Reserved.   */
    cs_uint32 image_download_mode         :  2 ; /* Image_download_mode option*/
                                                 /* 0 -- UART download*/
                                                 /* 1 -- Flash download*/
                                                 /* 2 -- Serial Flash download*/
                                                 /* 3 -- MII download*/
    cs_uint32 ge_if_mode                  :  1 ; /* 0 -- UNI ports is RGII mode; 1 -- UNI port is MII mode*/
    cs_uint32 pon_if_mode                 :  1 ; /* 0 -- SGMII mode; 1 -- SerDes mod*/
    cs_uint32 boot_mode                   :  2 ; /* Boot_mde option*/
                                                 /*0 -- Boot fram parallel Flash*/
                                                 /*1 -- Boot from internal MROM*/
                                                 /*2 -- Boot from serial Flash*/
                                                 /*3 -- Reserved*/
    cs_uint32 rsrvd                       : 18 ; 
} cs_aal_pin_status_t;



/*This function do the task as below( Only called by init APIs):         */
/*   1.Disable Tx/Rx on PON, MA and UNI                                  */
/*   2.Mask interrupt                                                    */
/*   2.Reset all module after step 1 and 2                               */
cs_status aal_sys_reset_ctrl_set(
    CS_IN  cs_aal_module_reset_mask_t mask,
    CS_IN  cs_boolean en
);

cs_status aal_sys_pin_ctrl_set(
    CS_IN  cs_aal_pin_ctrl_cfg_mask_t mask, 
    CS_IN  cs_aal_pin_ctrl_cfg_t cfg
);

cs_status aal_sys_pin_status_get(
    CS_OUT cs_aal_pin_status_t *status
);

/*Set default value to register*/
cs_status aal_init(void);

#endif /* __AAL_SYS_H__ */

