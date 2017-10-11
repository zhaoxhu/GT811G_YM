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
 
Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/

#include "plat_common.h"
#include "sdl_util.h"
#include "aal.h"
#include "aal_cls.h"
#include "aal_serdes.h"
#include "aal_sys.h"
#include "sdl_init.h"
#include "sdl.h"

extern cs_status sdl_service_down(void);
extern cs_status sdl_domain_table_init(sdl_init_cfg_t *sdl_init_cfg);
extern cs_status sdl_enc_init(const sdl_init_cfg_t*);
extern cs_status sdl_fec_init(const sdl_init_cfg_t*);
extern cs_status sdl_port_init(const sdl_init_cfg_t*);
extern cs_status sdl_fdb_init(const sdl_init_cfg_t*);
extern cs_status sdl_qos_init(sdl_init_cfg_t *cfg);
extern cs_status sdl_vlan_init(void);
extern cs_status sdl_mpcp_init(sdl_init_cfg_t *cfg);
extern cs_status sdl_pon_init(sdl_init_cfg_t *cfg);
extern cs_status sdl_ma_init(const sdl_init_cfg_t *cfg);

extern cs_status sdl_cmd_init(void);
extern cs_status pon_cmd_self_reg(void);
extern cs_status show_cmd_self_reg(void);
extern cs_status ma_cmd_self_reg(void);
extern cs_status clear_cmd_self_reg(void);
extern cs_status vlan_cmd_self_reg(void);
extern cs_status uni_cmd_self_reg(void);
extern cs_status macfilter_cmd_self_reg(void);
extern cs_status fdb_cmd_self_reg(void);
extern cs_status qos_cmd_self_reg(void);

extern cs_status sdl_switch_init(void);
extern cs_status sdl_l2_init(void);
extern cs_status sdl_mc_init(void);
extern cs_status sdl_vlan_init(void);
extern cs_status sdl_macfilter_init(void);
extern cs_status sdl_cls_init(void);
extern cs_status cls_cmd_self_reg(void);

cs_aal_cl_domain_res_t          domain_table_up[AAL_CL_DOMAIN_ID_NUM];
cs_aal_cl_domain_res_t          domain_table_dn[AAL_CL_DOMAIN_ID_NUM];
    
sdl_init_cfg_t sdl_int_cfg = 
{
    /* PON */
    1,                              /* laser_polarity */
    32,                             /* laser_on_time */
    32,                             /* laser_off_time */
    0,                              /* laser always on */

    /* MPCP */
    50,                             /* gate_timeout */
    0,                              /* N/A (window_fit) */
    0,                              /* N/A (zqt_mode) */
    AAL_MPCP_PENDING_GRANT_4,       /* pending_grant*/
    60,                             /* keep silence time */

    0,
    0,
    0,
    /* MAX_VLAN_NUM */
    0,
    /* MAC FILTER */
    64,                             /* max_mac_oper_entry */
    0xfffef,                        /* max_mac_oper_aging_time */
    63,                             /* fe_cls_default_timer */

    /* FDB */
    2112,                           /* max_fdb_entry */
    8,                              /* fdb_aging_cycle_time; */
    458,                            /* fdb_aging_max_time */
    64,                             /* max_static_mac_entry */
    64,                             /* max_vlan_mac_limit_entry */
    FALSE,                          /* fdb_limit_include_static */

    /* UNI */
    0,                              /* mtu_tag_exclude */
    1,                              /* phy_support_1000M */
    0xa,                            /* uni_tx_fifo_threshould; */
    0x18,                           /* uni_tx_fifo_ready_high; */
    0x10,                           /* uni_tx_fifo_ready_low; */
    1,
    1900,                           /* mtu_max_value*/
    1,                              /* max_uni_num */
    FALSE,                          /* pon_lpbk_turnoff_uni_led */

    /* Domain table */
    domain_table_up,
    domain_table_dn,

    /* QoS */
    8,                              /* max_queue_num */
    100000,
};


cs_status sdl_domain_table_init(sdl_init_cfg_t *sdl_init_cfg)
{   
    //Downstream domain init
    memset( &domain_table_dn[0], 0, AAL_CL_DOMAIN_ID_NUM*sizeof(cs_aal_cl_domain_res_t));

    //Upstream domain init
    memset( &domain_table_up[0], 0, AAL_CL_DOMAIN_ID_NUM*sizeof(cs_aal_cl_domain_res_t));
	
    domain_table_dn[DOMAIN_ID_EXT_QINQ].cl_entry_start = 0;
    domain_table_dn[DOMAIN_ID_EXT_QINQ].length         = 8;


	  
    domain_table_dn[DOMAIN_ID_MC].cl_entry_start = 8;
    domain_table_dn[DOMAIN_ID_MC].length         = 17;

    domain_table_dn[DOMAIN_ID_VLAN].cl_entry_start      = 26;
    domain_table_dn[DOMAIN_ID_VLAN].length              = 12;

	
	domain_table_up[DOMAIN_ID_EXT_QINQ].cl_entry_start		= 9;
	domain_table_up[DOMAIN_ID_EXT_QINQ].length				= 8;	

    domain_table_up[DOMAIN_ID_VLAN].cl_entry_start      = 18;
    domain_table_up[DOMAIN_ID_VLAN].length              = 12;
    
    domain_table_up[DOMAIN_ID_CLASSIFICATION].cl_entry_start      = 31;
    domain_table_up[DOMAIN_ID_CLASSIFICATION].length              = 10;
#if 0
    domain_table_dn[DOMAIN_ID_CLASSIFICATION].cl_entry_start      = 31;
    domain_table_dn[DOMAIN_ID_CLASSIFICATION].length              = 10;
#endif
    domain_table_up[DOMAIN_ID_SWITCH].cl_entry_start              = 46;
    domain_table_up[DOMAIN_ID_SWITCH].length                      = 1;

    (void)aal_cl_domain_set( AAL_PORT_ID_PON, AAL_CL_DOMAIN_ID_NUM, &domain_table_dn[0]);
    
    (void)aal_cl_domain_set( AAL_PORT_ID_GE, AAL_CL_DOMAIN_ID_NUM, &domain_table_up[0]);

    
    return CS_E_OK;
}

cs_status sdl_cmd_init(void)
{
#ifdef HAVE_SDL_CMD
    show_cmd_self_reg();
    pon_cmd_self_reg();
    clear_cmd_self_reg();
    ma_cmd_self_reg();
    vlan_cmd_self_reg();
    uni_cmd_self_reg();
    macfilter_cmd_self_reg();
    fdb_cmd_self_reg();
    qos_cmd_self_reg();
    cls_cmd_self_reg();

#endif
    return CS_E_OK;
}

cs_status sdl_internel_init(void)
{    
    /******temp for reset workaround*******/
    hal_delay_us(2000000);
    aal_init();
    sdl_cmd_init();
    sdl_domain_table_init(&sdl_int_cfg);
    sdl_mpcp_init(&sdl_int_cfg);
    sdl_pon_init(&sdl_int_cfg);
    sdl_ma_init(&sdl_int_cfg);
    sdl_enc_init(&sdl_int_cfg);
    sdl_fec_init(&sdl_int_cfg);

    sdl_switch_init();
    sdl_fdb_init(&sdl_int_cfg);
    sdl_qos_init(&sdl_int_cfg);
    sdl_l2_init();
    sdl_vlan_init();
    sdl_mc_init();
    sdl_macfilter_init();
    sdl_cls_init();
    sdl_port_init(&sdl_int_cfg);
    return CS_E_OK;
}


