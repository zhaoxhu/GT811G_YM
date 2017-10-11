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
#ifdef HAVE_CTC_OAM
#include "plat_common.h"
#include "oam_core.h"
#include "oam_client.h"
#include "oam_sdl_if.h"
#include "oam_ctc_pdu.h"
#include "oam_ctc_client.h"
#include "app_alarm_ctc.h"
#include "app_timer.h"
#include "event.h"
#include "sdl_api.h"
#include "db_instance.h"
#ifdef HAVE_LOOP_DETECT
#include "loop_detect.h"
#endif /* END HAVE_LOOP_DETECT */
#ifdef HAVE_EVENT_SHOW_CMD
#include "gwd_log.h"
#endif

/*Global variables*/
ctc_oam_alarm_info_t g_onu_alarm_cfg[] = 
{
    /* alarm_code                           threshold_type                      info_need,    polling_need */ 
    {CTC_ALARM_EQUIPMENT,       CTC_THRESH_UNSUPPORT ,          TRUE,             FALSE  },
    {CTC_ALARM_POWER,             CTC_THRESH_UNSUPPORT ,          TRUE,             FALSE  },
    {CTC_ALARM_BATT_MISSING,  CTC_THRESH_UNSUPPORT,            FALSE,           FALSE  },
    {CTC_ALARM_BATT_FAILURE,   CTC_THRESH_UNSUPPORT ,           FALSE,          FALSE  },
    {CTC_ALARM_BATT_VOLTLOW, CTC_THRESH_SIGNED_LOW ,         TRUE,            FALSE  },
    {CTC_ALARM_PHY_INTRUSION, CTC_THRESH_UNSUPPORT,            FALSE,          FALSE  }, 
    {CTC_ALARM_SELF_TEST,        CTC_THRESH_UNSUPPORT ,          TRUE,            FALSE  },
    {CTC_ALARM_ONU_TEMP_HIGH, CTC_THRESH_SIGNED_HIGH,         TRUE,           FALSE   }, 
    {CTC_ALARM_ONU_TEMP_LOW, CTC_THRESH_SIGNED_LOW,          TRUE,           FALSE   },
    {CTC_ALARM_ONU_IAD_CONN,  CTC_THRESH_UNSUPPORT,            TRUE,           FALSE   },
    {CTC_ALARM_ONU_PONIF_SWITCH, CTC_THRESH_UNSUPPORT,      TRUE,           FALSE   },
    {CTC_ALARM_ONU_SLP_STATUS_UPDATE, CTC_THRESH_UNSUPPORT,      TRUE,           FALSE   },
    {CTC_ALARM_ONU_DRY_CONTACTA, CTC_THRESH_UNSUPPORT,     TRUE,            FALSE   }, 
    {CTC_ALARM_ONU_DRY_CONTACTB, CTC_THRESH_UNSUPPORT,     TRUE,            FALSE   }
};

ctc_oam_alarm_info_t g_pon_alarm_cfg[] = 
{
    /* alarm_code                          threshold_type                    info_need,  polling_need */   
    {CTC_ALARM_RX_POW_HIGH,   CTC_THRESH_UNSIGNED_HIGH,   TRUE,         TRUE}, 
    {CTC_ALARM_RX_POW_LOW,   CTC_THRESH_UNSIGNED_LOW,   TRUE,          TRUE},
    {CTC_ALARM_TX_POW_HIGH,   CTC_THRESH_UNSIGNED_HIGH,  TRUE,          TRUE},
    {CTC_ALARM_TX_POW_LOW,   CTC_THRESH_UNSIGNED_LOW,   TRUE,          TRUE},
    {CTC_ALARM_TX_BIAS_HIGH,   CTC_THRESH_UNSIGNED_HIGH,  TRUE,          TRUE},
    {CTC_ALARM_TX_BIAS_LOW,   CTC_THRESH_UNSIGNED_LOW,   TRUE,          TRUE},
    {CTC_ALARM_VCC_HIGH,         CTC_THRESH_UNSIGNED_HIGH,  TRUE,          TRUE}, 
    {CTC_ALARM_VCC_LOW,          CTC_THRESH_UNSIGNED_LOW,  TRUE,          TRUE},
    {CTC_ALARM_PON_TEMP_HIGH, CTC_THRESH_SIGNED_HIGH,     TRUE,          TRUE},  
    {CTC_ALARM_PON_TEMP_LOW, CTC_THRESH_SIGNED_LOW,      TRUE,          TRUE},
    {CTC_WARN_RX_POW_HIGH,     CTC_THRESH_UNSIGNED_HIGH,  TRUE,          TRUE},
    {CTC_WARN_RX_POW_LOW,     CTC_THRESH_UNSIGNED_LOW,   TRUE,          TRUE}, 
    {CTC_WARN_TX_POW_HIGH,    CTC_THRESH_UNSIGNED_HIGH,   TRUE,          TRUE},
    {CTC_WARN_TX_POW_LOW,    CTC_THRESH_UNSIGNED_LOW,    TRUE,          TRUE},
    {CTC_WARN_TX_BIAS_HIGH,    CTC_THRESH_UNSIGNED_HIGH,   TRUE,          TRUE},
    {CTC_WARN_TX_BIAS_LOW,     CTC_THRESH_UNSIGNED_LOW,   TRUE,          TRUE},
    {CTC_WARN_VCC_HIGH,           CTC_THRESH_UNSIGNED_HIGH,  TRUE,          TRUE}, 
    {CTC_WARN_VCC_LOW,           CTC_THRESH_UNSIGNED_LOW,   TRUE,          TRUE},
    {CTC_WARN_PON_TEMP_HIGH, CTC_THRESH_SIGNED_HIGH,       TRUE,          TRUE},
    {CTC_WARN_PON_TEMP_LOW, CTC_THRESH_SIGNED_LOW,       TRUE,           TRUE},
    {CTC_ALARM_PON_RX_DROP,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_ALARM_PON_TX_DROP,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_ALARM_PON_RX_CRC,  CTC_THRESH_UNSIGNED_HIGH,      TRUE,           FALSE},
    {CTC_ALARM_PON_TX_CRC,  CTC_THRESH_UNSIGNED_HIGH,      TRUE,           FALSE},
    {CTC_ALARM_PON_RX_UNDER, CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_ALARM_PON_TX_UNDER, CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_ALARM_PON_RX_OVER,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_ALARM_PON_TX_OVER,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_ALARM_PON_RX_FRAG,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_ALARM_PON_TX_FRAG,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_ALARM_PON_RX_JABBER, CTC_THRESH_UNSIGNED_HIGH,   TRUE,           FALSE},
    {CTC_ALARM_PON_TX_JABBER, CTC_THRESH_UNSIGNED_HIGH,   TRUE,           FALSE},
    {CTC_ALARM_PON_RX_DISCA, CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_ALARM_PON_TX_DISCA, CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_ALARM_PON_RX_ERROR,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_ALARM_PON_TX_ERROR,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_WARN_PON_RX_DROP,  CTC_THRESH_UNSIGNED_HIGH,     TRUE,           FALSE},
    {CTC_WARN_PON_TX_DROP,  CTC_THRESH_UNSIGNED_HIGH,     TRUE,           FALSE},
    {CTC_WARN_PON_RX_CRC,  CTC_THRESH_UNSIGNED_HIGH,       TRUE,           FALSE},
    {CTC_WARN_PON_TX_CRC,  CTC_THRESH_UNSIGNED_HIGH,       TRUE,           FALSE},
    {CTC_WARN_PON_RX_UNDER, CTC_THRESH_UNSIGNED_HIGH,     TRUE,           FALSE},
    {CTC_WARN_PON_TX_UNDER, CTC_THRESH_UNSIGNED_HIGH,     TRUE,           FALSE},
    {CTC_WARN_PON_RX_OVER,  CTC_THRESH_UNSIGNED_HIGH,     TRUE,           FALSE},
    {CTC_WARN_PON_TX_OVER,  CTC_THRESH_UNSIGNED_HIGH,     TRUE,           FALSE},
    {CTC_WARN_PON_RX_FRAG,  CTC_THRESH_UNSIGNED_HIGH,     TRUE,           FALSE},
    {CTC_WARN_PON_TX_FRAG,  CTC_THRESH_UNSIGNED_HIGH,     TRUE,           FALSE},
    {CTC_WARN_PON_RX_JABBER, CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_WARN_PON_TX_JABBER, CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE},
    {CTC_WARN_PON_RX_DISCA, CTC_THRESH_UNSIGNED_HIGH,     TRUE,           FALSE},
    {CTC_WARN_PON_TX_DISCA, CTC_THRESH_UNSIGNED_HIGH,     TRUE,           FALSE},
    {CTC_WARN_PON_RX_ERROR,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE}, 
    {CTC_WARN_PON_TX_ERROR,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,           FALSE} 
};

ctc_oam_alarm_info_t g_cuc_pon_alarm_cfg[] = 
{
    /* alarm_code                          threshold_type                    info_need,  polling_need */   
    {CTC_ALARM_RX_POW_HIGH,   CTC_THRESH_UNSIGNED_HIGH,   TRUE,         TRUE}, 
    {CTC_ALARM_RX_POW_LOW,   CTC_THRESH_UNSIGNED_LOW,   TRUE,          TRUE},
    {CTC_ALARM_TX_POW_HIGH,   CTC_THRESH_UNSIGNED_HIGH,  TRUE,          TRUE},
    {CTC_ALARM_TX_POW_LOW,   CTC_THRESH_UNSIGNED_LOW,   TRUE,          TRUE},
    {CTC_ALARM_TX_BIAS_HIGH,   CTC_THRESH_UNSIGNED_HIGH,  TRUE,          TRUE},
    {CTC_ALARM_TX_BIAS_LOW,   CTC_THRESH_UNSIGNED_LOW,   TRUE,          TRUE},
    {CTC_ALARM_VCC_HIGH,         CTC_THRESH_UNSIGNED_HIGH,  TRUE,          TRUE}, 
    {CTC_ALARM_VCC_LOW,          CTC_THRESH_UNSIGNED_LOW,  TRUE,          TRUE},
    {CTC_ALARM_PON_TEMP_HIGH, CTC_THRESH_SIGNED_HIGH,     TRUE,          TRUE},  
    {CTC_ALARM_PON_TEMP_LOW, CTC_THRESH_SIGNED_LOW,      TRUE,          TRUE},
    {CTC_WARN_RX_POW_HIGH,     CTC_THRESH_UNSIGNED_HIGH,  TRUE,          TRUE},
    {CTC_WARN_RX_POW_LOW,     CTC_THRESH_UNSIGNED_LOW,   TRUE,          TRUE}, 
    {CTC_WARN_TX_POW_HIGH,    CTC_THRESH_UNSIGNED_HIGH,   TRUE,          TRUE},
    {CTC_WARN_TX_POW_LOW,    CTC_THRESH_UNSIGNED_LOW,    TRUE,          TRUE},
    {CTC_WARN_TX_BIAS_HIGH,    CTC_THRESH_UNSIGNED_HIGH,   TRUE,          TRUE},
    {CTC_WARN_TX_BIAS_LOW,     CTC_THRESH_UNSIGNED_LOW,   TRUE,          TRUE},
    {CTC_WARN_VCC_HIGH,           CTC_THRESH_UNSIGNED_HIGH,  TRUE,          TRUE}, 
    {CTC_WARN_VCC_LOW,           CTC_THRESH_UNSIGNED_LOW,   TRUE,          TRUE},
    {CTC_WARN_PON_TEMP_HIGH, CTC_THRESH_SIGNED_HIGH,       TRUE,          TRUE},
    {CTC_WARN_PON_TEMP_LOW, CTC_THRESH_SIGNED_LOW,       TRUE,           TRUE},
    {CUC_OUTOFTIME_TX_SLOT,   CTC_THRESH_UNSUPPORT,       TRUE,            TRUE},
};

/*port alarm cfg table*/
/*ctc*/
ctc_oam_alarm_info_t **g_port_alarm_cfg = NULL;
cs_uint32 g_port_alarm_num = 0;
/*cuc*/
ctc_oam_alarm_info_t **g_cuc_port_alarm_cfg = NULL;
cs_uint32 g_cuc_port_alarm_num = 0;

/*polling timer*/
ctc_oam_alarm_polling_cb_t g_alarm_polling_cb;


extern void ctc_oam_send(
        cs_uint32 port,
        cs_uint8 *buf,
        cs_int32 len
        );

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_init_port_alarm_info                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
void ctc_oam_init_alarm_item_info(ctc_oam_alarm_info_t* item)
 /*                                                                           */
 /* INPUTS  :                                                               */
/*     none                                                                     */
/* OUTPUTS : ----                                                            */
/* DESCRIPTION:                                                              */
/* API to init alarm info                                         */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    item->alarm_state = CTC_ALARM_CLEAR;
    item->enable = CTC_ALARM_OFF;

    if(item->threshold_type == CTC_THRESH_SIGNED_HIGH)
    {
        item->raise_alarm_threshold.signValue = SIGNED_RAISED_DEF_THRESH;
        item->clear_alarm_threshold.signValue = SIGNED_CLEARED_DEF_THRESH;
    }
    else if(item->threshold_type == CTC_THRESH_SIGNED_LOW)
    {
        item->raise_alarm_threshold.signValue = SIGNED_CLEARED_DEF_THRESH;
        item->clear_alarm_threshold.signValue = SIGNED_RAISED_DEF_THRESH;
    }
    else if(item->threshold_type == CTC_THRESH_UNSIGNED_HIGH)
    {
        item->raise_alarm_threshold.unsignValue = UNSIGNED_RAISED_DEF_THRESH;
        item->clear_alarm_threshold.unsignValue = UNSIGNED_CLEARED_DEF_THRESH;
    }
    else if(item->threshold_type == CTC_THRESH_UNSIGNED_LOW)
    {
        item->raise_alarm_threshold.unsignValue = UNSIGNED_CLEARED_DEF_THRESH;
        item->clear_alarm_threshold.unsignValue = UNSIGNED_RAISED_DEF_THRESH;
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_init_pon_alarm_info                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
void ctc_oam_init_pon_alarm_info()
 /*                                                                           */
 /* INPUTS  :                                                               */
/*     none                                                                     */
/* OUTPUTS : ----                                                            */
/* DESCRIPTION:                                                              */
/* API to init alarm info                                         */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 i = 0;
    cs_uint8 num = 0;
    ctc_oam_alarm_info_t *pInfo = NULL;
#if 0
    if(!oam_is_cuc_oam())
    {
        num = sizeof(g_pon_alarm_cfg)/sizeof(ctc_oam_alarm_info_t);
        pInfo = &g_pon_alarm_cfg[0];
    }
    else
    {
        num = sizeof(g_cuc_pon_alarm_cfg)/sizeof(ctc_oam_alarm_info_t);
        pInfo = &g_cuc_pon_alarm_cfg[0];
    }
#endif

    /*init both table*/
    /*init ctc table*/
    num = sizeof(g_pon_alarm_cfg)/sizeof(ctc_oam_alarm_info_t);
    pInfo = &g_pon_alarm_cfg[0];
    for(i = 0; i < num; i++)
    {
        ctc_oam_init_alarm_item_info(pInfo);
        pInfo++;
    }

    /*init cuc table*/
    num = sizeof(g_cuc_pon_alarm_cfg)/sizeof(ctc_oam_alarm_info_t);
    pInfo = &g_cuc_pon_alarm_cfg[0];
    for(i = 0; i < num; i++)
    {
        ctc_oam_init_alarm_item_info(pInfo);
        pInfo++;
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_init_onu_alarm_info                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
void ctc_oam_init_onu_alarm_info()
 /*                                                                           */
 /* INPUTS  :                                                               */
/*     none                                                                     */
/* OUTPUTS : ----                                                            */
/* DESCRIPTION:                                                              */
/* API to init alarm info                                         */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 i = 0;
    cs_uint8 num = sizeof(g_onu_alarm_cfg)/sizeof(ctc_oam_alarm_info_t);

    for(i = 0; i < num; i++)
    {
        ctc_oam_init_alarm_item_info(&g_onu_alarm_cfg[i]);
    }
}


/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_init_port_alarm_info                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
void ctc_oam_init_port_alarm_info()
 /*                                                                           */
 /* INPUTS  :                                                               */
/*     none                                                                     */
/* OUTPUTS : ----                                                            */
/* DESCRIPTION:                                                              */
/* API to init alarm info                                         */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 i = 0;
    cs_uint8 port = 0;
    ctc_oam_alarm_info_t item_cfg[] = 
    {
        /* alarm_code                            threshold_type                  info_need,  polling_need  */   
        {CTC_ALARM_AUTO_NEG_FAIL,  CTC_THRESH_UNSUPPORT ,       FALSE,       FALSE  }, 
        {CTC_ALARM_PORT_LOS,         CTC_THRESH_UNSUPPORT ,        FALSE,       FALSE },
        {CTC_ALARM_PORT_FAIL,        CTC_THRESH_UNSUPPORT,         FALSE,       FALSE  },
        {CTC_ALARM_PORT_LB,           CTC_THRESH_UNSUPPORT ,        FALSE,       FALSE},
        {CTC_ALARM_PORT_CONGEST,  CTC_THRESH_UNSUPPORT,         FALSE,       FALSE  },
        {CTC_ALARM_PORT_RX_DROP,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_ALARM_PORT_TX_DROP,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_ALARM_PORT_RX_CRC,    CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_ALARM_PORT_TX_CRC,    CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_ALARM_PORT_RX_UNDER,  CTC_THRESH_UNSIGNED_HIGH,   TRUE,        FALSE  },
        {CTC_ALARM_PORT_TX_UNDER,  CTC_THRESH_UNSIGNED_HIGH,   TRUE,        FALSE  },
        {CTC_ALARM_PORT_RX_OVER,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_ALARM_PORT_TX_OVER,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_ALARM_PORT_RX_FRAG,  CTC_THRESH_UNSIGNED_HIGH,     TRUE,        FALSE  },
        {CTC_ALARM_PORT_TX_FRAG,  CTC_THRESH_UNSIGNED_HIGH,     TRUE,        FALSE  },
        
        {CTC_ALARM_PORT_RX_JABBER,  CTC_THRESH_UNSIGNED_HIGH,   TRUE,        FALSE  },
        {CTC_ALARM_PORT_TX_JABBER,  CTC_THRESH_UNSIGNED_HIGH,   TRUE,        FALSE  },
        {CTC_ALARM_PORT_RX_DISCA,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_ALARM_PORT_TX_DISCA,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_ALARM_PORT_RX_ERROR,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_ALARM_PORT_TX_ERROR,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        
        {CTC_ALARM_PORT_STATUS,     CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_WARN_PORT_RX_DROP,  CTC_THRESH_UNSIGNED_HIGH,      TRUE,        FALSE  },
        {CTC_WARN_PORT_TX_DROP,  CTC_THRESH_UNSIGNED_HIGH,     TRUE,        FALSE  },
        {CTC_WARN_PORT_RX_CRC,    CTC_THRESH_UNSIGNED_HIGH,     TRUE,        FALSE  },
        {CTC_WARN_PORT_TX_CRC,    CTC_THRESH_UNSIGNED_HIGH,     TRUE,        FALSE  },
        {CTC_WARN_PORT_RX_UNDER,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_WARN_PORT_TX_UNDER,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_WARN_PORT_RX_OVER,  CTC_THRESH_UNSIGNED_HIGH,      TRUE,        FALSE  },
        {CTC_WARN_PORT_TX_OVER,  CTC_THRESH_UNSIGNED_HIGH,      TRUE,        FALSE  },
        {CTC_WARN_PORT_RX_FRAG,  CTC_THRESH_UNSIGNED_HIGH,      TRUE,        FALSE  },
        {CTC_WARN_PORT_TX_FRAG,  CTC_THRESH_UNSIGNED_HIGH,      TRUE,        FALSE  },
        {CTC_WARN_PORT_RX_JABBER,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_WARN_PORT_TX_JABBER,  CTC_THRESH_UNSIGNED_HIGH,    TRUE,        FALSE  },
        {CTC_WARN_PORT_RX_DISCA,  CTC_THRESH_UNSIGNED_HIGH,     TRUE,        FALSE  },
        {CTC_WARN_PORT_TX_DISCA,  CTC_THRESH_UNSIGNED_HIGH,     TRUE,        FALSE  },
        {CTC_WARN_PORT_RX_ERROR,  CTC_THRESH_UNSIGNED_HIGH,     TRUE,        FALSE  },
        {CTC_WARN_PORT_TX_ERROR,  CTC_THRESH_UNSIGNED_HIGH,     TRUE,        FALSE  },
        {CTC_WARN_PORT_STATUS,     CTC_THRESH_UNSIGNED_HIGH,     TRUE,        FALSE  },
        {CTC_ALARM_PORT_LOOPDETECT,CTC_THRESH_UNSUPPORT,        FALSE,       FALSE  }
    };
    ctc_oam_alarm_info_t cuc_item_cfg[] = 
    {
        /* alarm_code                            threshold_type                  info_need,  polling_need  */   
        {CTC_ALARM_AUTO_NEG_FAIL,  CTC_THRESH_UNSUPPORT ,       FALSE,       FALSE  }, 
        {CTC_ALARM_PORT_LOS,         CTC_THRESH_UNSUPPORT ,        FALSE,       FALSE  },
        {CTC_ALARM_PORT_FAIL,        CTC_THRESH_UNSUPPORT,         FALSE,       FALSE  },
        {CTC_ALARM_PORT_LB,           CTC_THRESH_UNSUPPORT ,        TRUE,       FALSE  },
        {CTC_ALARM_PORT_CONGEST,  CTC_THRESH_UNSUPPORT,         FALSE,       FALSE  }
    };

    g_port_alarm_num = sizeof(item_cfg)/sizeof(ctc_oam_alarm_info_t);
    g_cuc_port_alarm_num = sizeof(cuc_item_cfg)/sizeof(ctc_oam_alarm_info_t);

    g_port_alarm_cfg = iros_malloc(IROS_MID_OAM, sizeof(ctc_oam_alarm_info_t *)*CTC_MAX_ONU_UNI_PORTS);
    g_cuc_port_alarm_cfg = iros_malloc(IROS_MID_OAM, sizeof(ctc_oam_alarm_info_t *)*CTC_MAX_ONU_UNI_PORTS);
    if(g_port_alarm_cfg == NULL || g_cuc_port_alarm_cfg == NULL)
    {
        cs_printf("alarm malloc fail: fatal error\n");
        return;
    }
    memset(g_port_alarm_cfg, 0, sizeof(ctc_oam_alarm_info_t *)*CTC_MAX_ONU_UNI_PORTS);
    memset(g_cuc_port_alarm_cfg, 0, sizeof(ctc_oam_alarm_info_t *)*CTC_MAX_ONU_UNI_PORTS);

    for(port = 0; port < CTC_MAX_ONU_UNI_PORTS; port++)
    {
        g_port_alarm_cfg[port] = iros_malloc(IROS_MID_OAM, sizeof(item_cfg));
        g_cuc_port_alarm_cfg[port] = iros_malloc(IROS_MID_OAM, sizeof(cuc_item_cfg));
        if(g_port_alarm_cfg[port] == NULL || g_cuc_port_alarm_cfg[port] == NULL)
        {
            cs_printf("alarm malloc fail: fatal error\n");
            return;
        }
        memcpy(g_port_alarm_cfg[port],item_cfg, sizeof(item_cfg));
        memcpy(g_cuc_port_alarm_cfg[port],cuc_item_cfg, sizeof(cuc_item_cfg));
        for(i = 0; i < g_port_alarm_num; i++)
        {
            ctc_oam_init_alarm_item_info(&g_port_alarm_cfg[port][i]);
        }
        for(i = 0; i < g_cuc_port_alarm_num; i++)
        {
            ctc_oam_init_alarm_item_info(&g_cuc_port_alarm_cfg[port][i]);
        }
    }

#if 0
    if(!oam_is_cuc_oam())
    {
        g_port_alarm_num = sizeof(item_cfg)/sizeof(ctc_oam_alarm_info_t);
    }
    else
    {
        g_port_alarm_num = sizeof(cuc_item_cfg)/sizeof(ctc_oam_alarm_info_t);
    }

    g_port_alarm_cfg = iros_malloc(IROS_MID_OAM, sizeof(ctc_oam_alarm_info_t *)*CTC_MAX_ONU_UNI_PORTS);
    if(!g_port_alarm_cfg)
        return;
    memset(g_port_alarm_cfg, 0, sizeof(ctc_oam_alarm_info_t *)*CTC_MAX_ONU_UNI_PORTS);

    for(port = 0; port < CTC_MAX_ONU_UNI_PORTS; port++)
    {
        if(!oam_is_cuc_oam())
        {
            g_port_alarm_cfg[port] = iros_malloc(IROS_MID_OAM, sizeof(item_cfg));
        }
        else
        {
            g_port_alarm_cfg[port] = iros_malloc(IROS_MID_OAM, sizeof(cuc_item_cfg));
        }
        if(!g_port_alarm_cfg[port])
            return;

        if(!oam_is_cuc_oam())
        {
            memcpy(g_port_alarm_cfg[port],item_cfg, sizeof(item_cfg));
        }
        else
        {
            memcpy(g_port_alarm_cfg[port],cuc_item_cfg, sizeof(cuc_item_cfg));
        }
        for(i = 0; i < g_port_alarm_num; i++)
        {
            ctc_oam_init_alarm_item_info(&g_port_alarm_cfg[port][i]);
        }
    } 
#endif  
}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_alarm_start_polling_timer                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
void ctc_alarm_stop_polling_timer()
 /*                                                                           */
 /* INPUTS  :                                                               */
/*     none                                                                     */
/* OUTPUTS : ----                                                            */
/* DESCRIPTION:                                                              */
/* API to init alarm info                                         */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    if(g_alarm_polling_cb.timer_handler != 0)
    {
        cs_timer_del(g_alarm_polling_cb.timer_handler );
        g_alarm_polling_cb.timer_handler = 0;
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_alarm_start_polling_timer                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
void ctc_alarm_start_polling_timer()
 /*                                                                           */
 /* INPUTS  :                                                               */
/*     none                                                                     */
/* OUTPUTS : ----                                                            */
/* DESCRIPTION:                                                              */
/* API to init alarm info                                         */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    g_alarm_polling_cb.timer_handler = cs_msg_circle_timer_add(app_msg_q, g_alarm_polling_cb.timer_value, 
                                                                    (void*)&g_alarm_polling_cb);
}
extern int flag_port_up;

void
ctc_oam_alarm_port_link_status_callback(
        cs_uint16   evt_dev,
        cs_uint32   evt_type,
        void        *data)
{
        sdl_event_port_link_t *msg = (sdl_event_port_link_t*)data;
        cs_port_id_t port = 0;
        cs_sdl_port_link_status_t opr = SDL_PORT_LINK_STATUS_DOWN;
        
        if(msg == NULL)
            return;

        port = msg->port;
        opr = msg->link;

        cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM, "port %d, link %d\n", port, opr);
        
        if(port > CTC_MAX_ONU_UNI_PORTS) {
            cs_log_msg(IROS_LOG_LEVEL_MIN, IROS_MID_ORG_OAM,"oam_handler_port_link_status port %d error!\n", port);
            return;
        }

        if(port == CS_PON_PORT_ID)
            return;

        if(opr == SDL_PORT_LINK_STATUS_UP)
        {
            if(flag_port_up == 0)
            ctc_oam_build_alarm_event(CTC_PORT_ALARM, port, CTC_ALARM_PORT_LOS,
                                                       CTC_ALARM_CLEAR, 0);
        }
        else if(opr == SDL_PORT_LINK_STATUS_DOWN)
        {
            if(flag_port_up == 0)
            ctc_oam_build_alarm_event(CTC_PORT_ALARM, port, CTC_ALARM_PORT_LOS,
                                                       CTC_ALARM_RAISE, 0);
        }
        else
        {
            cs_log_msg(IROS_LOG_LEVEL_MIN, IROS_MID_ORG_OAM,"oam_handler_port_link_status unknown opr %d!\n", opr);
        }
}

void
ctc_oam_alarm_port_auto_neg_callback(
        cs_uint16   evt_dev,
        cs_uint32   evt_type,
        void        *data)
{
        sdl_event_port_auto_nego_t *msg = (sdl_event_port_auto_nego_t*)data;
        cs_port_id_t port = 0;
        cs_boolean status = FALSE;
        
        if(msg == NULL)
            return;

        port = msg->port;
        status = msg->status;

        cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM, "port %d, status %d\n", port, status);
        
        if(port > CTC_MAX_ONU_UNI_PORTS) {
            cs_log_msg(IROS_LOG_LEVEL_MIN, IROS_MID_ORG_OAM,"oam_handler_port_link_status port %d error!\n", port);
            return;
        }

        if(status)
        {
            ctc_oam_build_alarm_event(CTC_PORT_ALARM, port, CTC_ALARM_AUTO_NEG_FAIL,
                                                       CTC_ALARM_CLEAR, 0);
			#ifdef HAVE_EVENT_SHOW_CMD
			gw_syslog(GW_LOG_LEVEL_MAJOR, "port :%d auto  negotiation success\n", port);
			#endif
        }
        else
        {
            ctc_oam_build_alarm_event(CTC_PORT_ALARM, port, CTC_ALARM_AUTO_NEG_FAIL,
                                                       CTC_ALARM_RAISE, 0);
			#ifdef HAVE_EVENT_SHOW_CMD
			gw_syslog(GW_LOG_LEVEL_MAJOR, "port :%d auto  negotiation failed\n", port);
			#endif
        }
        
}

void
ctc_oam_alarm_port_up_congest_callback(
        cs_uint16   evt_dev,
        cs_uint32   evt_type,
        void        *data)
{
        sdl_event_congest_t *msg = (sdl_event_congest_t*)data;
        cs_port_id_t port = 0;
        cs_boolean status = FALSE;
        
        if(msg == NULL)
            return;

        port = msg->port;
        status = msg->congest;

        cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM, "port %d, status %d\n", port, status);
        
        if(port > CTC_MAX_ONU_UNI_PORTS) {
            cs_log_msg(IROS_LOG_LEVEL_MIN, IROS_MID_ORG_OAM,"oam_handler_port_up_congest port %d error!\n", port);
            return;
        }

        if(status)
        {
            ctc_oam_build_alarm_event(CTC_PORT_ALARM, port, CTC_ALARM_PORT_CONGEST,
                                                       CTC_ALARM_RAISE, 0);
        }
        else
        {
            ctc_oam_build_alarm_event(CTC_PORT_ALARM, port, CTC_ALARM_PORT_CONGEST,
                                                       CTC_ALARM_CLEAR, 0);
        }
        
}

#ifdef HAVE_LOOP_DETECT
cs_status ctc_oam_loop_detect_handle(cs_port_id_t port, loop_detect_event_t type)
{
    loop_reason_t reason = 0;
    cs_uint32 info = 0;
    switch(type)
    {
        case LOOPDETECT_ENABLE_PREPARE_EVENT:
            /*set block object*/
            loop_detect_block_object_set(LOOP_DETECT_BLOCK_PORT_BASE);
            /*set unblock policy*/
            loop_detect_unblock_policy_set(LOOP_DETECT_EXPIRE_UNBLOCK);
            /*set clear check policy*/
            loop_clear_policy_set(LOOP_CLEAR_NEXT_ROUND_CHECK);
            /*set interval time*/
            loop_detect_interval_set(3*1000);
            /*set block time*/
            loop_detect_block_time_set(port, 60*1000);
            break;
            

        case LOOP_CLEARED_AFTER_EVENT:
            /*build CTC event*/
            if(oam_is_cuc_oam())
            {
                loop_detect_loop_reason_get(port, &reason);
                if(reason == LOOP_REASON_INNER)
                {
                    info = CUC_INNER_LOOP_VALUE;
                }
                else if(reason == LOOP_REASON_OUT)
                {
                    info = CUC_OUT_LOOP_VALUE;
                }
            }

            ctc_oam_build_alarm_event(CTC_PORT_ALARM, port, CTC_ALARM_PORT_LB, 
                                                       CTC_ALARM_CLEAR, (cs_uint64)info);
            break;
            
        case LOOP_DETECTED_AFTER_EVENT:
            if(oam_is_cuc_oam())
            {
                loop_detect_loop_reason_get(port, &reason);
                if(reason == LOOP_REASON_INNER)
                {
                    info = CUC_INNER_LOOP_VALUE;
                }
                else if(reason == LOOP_REASON_OUT)
                {
                    info = CUC_OUT_LOOP_VALUE;
                }
            }

            /*build CTC event*/
            ctc_oam_build_alarm_event(CTC_PORT_ALARM, port, CTC_ALARM_PORT_LB,
                                                       CTC_ALARM_RAISE,(cs_uint64) info);
            break;
            
        default:
            break;
    }

    return CS_E_OK;
}
#endif /* END HAVE_LOOP_DETECT */

cs_boolean ctc_oam_discovery_result_get()
{
    oam_if_t *oamif = oam_intf_find(oam_sdl_get_llid());

    if(oamif == NULL)
    {
        return FALSE;
    }

    if(oamif->ctc_OAM_Ext_Status == OAM_CTC_EXT_ONU_COMPLETED)
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_alarm_init                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
void ctc_oam_alarm_init()
 /*                                                                           */
 /* INPUTS  :                                                               */
/*     none                                                                     */
/* OUTPUTS : ----                                                            */
/* DESCRIPTION:                                                              */
/* API to init alarm info                                         */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    memset(&g_alarm_polling_cb, 0, sizeof(g_alarm_polling_cb));
    g_alarm_polling_cb.timer_value = ALARM_POLLING_INTERVAL;
    g_alarm_polling_cb.msg_info.msg_type = IROS_MSG_TIME_OUT;
    g_alarm_polling_cb.msg_info.timer_type = APP_TIMER_ALARM_POLLING;

    /*init alarm info table*/
    ctc_oam_init_pon_alarm_info();
    ctc_oam_init_onu_alarm_info();
    ctc_oam_init_port_alarm_info();

    onu_evt_reg(EPON_EVENT_PORT_LINK_CHANGE, (void*)ctc_oam_alarm_port_link_status_callback, NULL);
    onu_evt_reg(EPON_EVENT_PORT_AUTO_NEGO_STATUS, (void*)ctc_oam_alarm_port_auto_neg_callback, NULL);
    onu_evt_reg(EPON_EVENT_UPSTREAM_CONGEST, (void*)ctc_oam_alarm_port_up_congest_callback, NULL);

    #ifdef HAVE_LOOP_DETECT
    loop_detect_special_handler_reg(ctc_oam_loop_detect_handle);
   #endif 

   {
    cs_uint8 port = CS_UNI_PORT_ID1;

    for(port = CS_UNI_PORT_ID1;port <= CTC_MAX_ONU_UNI_PORTS;port ++)
    {
        ctc_oam_set_alarm_config(CTC_PORT_ALARM, port, CTC_ALARM_PORT_LOS, CTC_ALARM_ON);
        ctc_oam_build_alarm_event(CTC_PORT_ALARM, port, CTC_ALARM_PORT_LOS,CTC_ALARM_RAISE, 0);
    }
   }
   
   
}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_get_alarm_info                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
static cs_status ctc_oam_get_alarm_info_table(
        ctc_oam_alarm_class_e type,
        cs_port_id_t port,
        cs_uint32 *num,
        ctc_oam_alarm_info_t **info)
/*                                                                           */
/* INPUTS  : o alarm_code---alarm id                                           */
/*               o enable---                                          */
/*               o portId---port id                                          */
/* OUTPUTS : -----                                                            */
/* RETURNS : o EPON_RETURN_XXX(see section 3.7 for return code definitions)*/
/* DESCRIPTION:                                                              */
/* API to set alarm on/off                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    if(!ctc_oam_discovery_result_get())
    {
        cs_log_msg(IROS_LOG_LEVEL_MIN, IROS_MID_ORG_OAM,
                           "ctc oam discovery not finish, you may get wrong info\n");
    }
    if(type == CTC_PON_ALARM)
    {
        if(!oam_is_cuc_oam())
        {
            *info = g_pon_alarm_cfg;
            *num = sizeof(g_pon_alarm_cfg)/sizeof(ctc_oam_alarm_info_t);
        }
        else
        {
            *info = g_cuc_pon_alarm_cfg;
            *num = sizeof(g_cuc_pon_alarm_cfg)/sizeof(ctc_oam_alarm_info_t);
        }
    }
    else if(type == CTC_ONU_ALARM)
    {
        *info = g_onu_alarm_cfg;
        *num = sizeof(g_onu_alarm_cfg)/sizeof(ctc_oam_alarm_info_t);
    }
    else if(type == CTC_PORT_ALARM)
    {
        if(port > CTC_MAX_ONU_UNI_PORTS || port < CS_UNI_PORT_ID1)
        {
            return CS_E_PARAM;
        }
        if(!oam_is_cuc_oam())
        {
            *info = g_port_alarm_cfg[port-1];
            *num = g_port_alarm_num;
        }
        else 
        {
            *info = g_cuc_port_alarm_cfg[port-1];
            *num = g_cuc_port_alarm_num;
        }
    }
    else
    {
        return CS_E_PARAM;
    }

    return CS_E_OK;
}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_get_alarm_info                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
static ctc_oam_alarm_info_t* ctc_oam_get_alarm_info(
        ctc_oam_alarm_class_e type,
        cs_port_id_t port,
        cs_uint16  alarm_code)
/*                                                                           */
/* INPUTS  : o alarm_code---alarm id                                           */
/*               o enable---                                          */
/*               o portId---port id                                          */
/* OUTPUTS : -----                                                            */
/* RETURNS : o EPON_RETURN_XXX(see section 3.7 for return code definitions)*/
/* DESCRIPTION:                                                              */
/* API to set alarm on/off                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    ctc_oam_alarm_info_t *p_info_table = NULL;
    cs_uint32 num = 0;
    cs_uint32 i = 0;

    if(CS_E_OK != ctc_oam_get_alarm_info_table(type, port, &num, &p_info_table))
    {
        cs_printf("can not search alarm table, type %d, port %d\n", type, port);
        return NULL;
    }

    if(!p_info_table)
    {
        cs_printf("alarm table is null\n");
        return NULL;
    }
    for(i = 0; i < num; i++)
    {
        if(p_info_table[i].alarm_code == alarm_code)
        {
            return &p_info_table[i];
        }
    }

    /*Not search it*/
    cs_printf("can not search table item, type %d, port %d\n", type, port);
    return NULL;
    
}

void ctc_oam_clear_alarm_event(
        ctc_oam_alarm_class_e type,
        cs_port_id_t port,
        cs_uint16  alarm_code)
{
    ctc_oam_alarm_info_t *p_info = NULL;
    p_info = ctc_oam_get_alarm_info(type, port, alarm_code);

    if(p_info == NULL)
    {
        return;
    }

    p_info->alarm_state = CTC_ALARM_CLEAR;
}
extern int ctc_version_set;

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_build_alarm_event                                   */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
void ctc_oam_build_alarm_event(
        ctc_oam_alarm_class_e type,
        cs_port_id_t port,
        cs_uint16  alarm_code,
        cs_uint8 alarm_op,
        cs_uint64 alarm_info)
/*                                                                           */
/* INPUTS  : o  type - alarm type                                           */
/*     o  port - ONU uni or pon port, onu self             */
/*     o  alarm_code  - alarm code           */
/*     o  alarm_op  - 0/raise 1/clear          */
/*     o  alarm_info  - info     */
/* OUTPUTS : ----                                                            */
/* RETURNS :                                    */
/* DESCRIPTION:                                                              */
/* API to compose CTC alram event OAM frame                             */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
        cs_int32 len = 0;
        oam_ctc_tlv_org_spec_t *p_tlv_org_event_oam = NULL ;
        cs_uint8  * p_seq = NULL;
        cs_uint16 seq = 0;
        cs_uint8 pdu[OAM_MAX_OAMPDU_SIZE];
        oam_if_t *oamif = oam_intf_find(oam_sdl_get_llid());
        ctc_oam_alarm_info_t *p_info = NULL;
        cs_uint32   alarm_info_len=0;
        cs_uint32   alarm_infor_uint32=0;
        cs_uint8    alarm_infor_buffer[8];

        /*0x0115---DownstreamDropEventsAlarm
           0x0134--- UpstreamErrorsWarning
           0x0306----DownstreamDropEventsAlarm
           0x0327---StatusChangeTimesWarning
        */
        if ( ( (alarm_code >=0x0115) && (alarm_code <=0x0134) )
                 || (( alarm_code>=0x0306)  && ( alarm_code <=0x0327)) ) {
            alarm_info_len= 8;
        }else {
            alarm_info_len=4;
        }
cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM,"oamif %p\n",oamif);
        if(oamif == NULL)
        {
            return;
        }

        p_info = ctc_oam_get_alarm_info(type, port, alarm_code);
cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM,"p_info %p,type %d,port %d,code 0x%x\n",
    p_info,type,port,alarm_code);

        if(p_info == NULL)
        {cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM,"%s. %d\n",__func__,__LINE__);
            return;
        }
        cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM, "p_info->enable %d, p_info->alarm_state  %d,alarm op %d\n", 
            p_info->enable, p_info->alarm_state,alarm_op);
        
        /*check alarm is opened*/
        if(p_info->enable == CTC_ALARM_OFF) {
            cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM,"%s. %d\n",__func__,__LINE__);
            return;
        }

        /*check whether redundant alarm*/
        if(p_info->alarm_state == alarm_op) {
            cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM,"%s. %d\n",__func__,__LINE__);
            return; 
         }
        p_info->alarm_state = alarm_op;

        memset(pdu,0x00,OAM_MAX_OAMPDU_SIZE);

        oam_build_pdu_hdr(oamif, (oam_pdu_hdr_t *)pdu, OAM_PDU_CODE_EVENT);
cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM,"%s. %d\n",__func__,__LINE__);
        p_seq = pdu + sizeof(oam_pdu_hdr_t);
        seq = htons(++oamif->seq_num_next_tx);
        memcpy(p_seq, &seq, sizeof(seq));
        len = sizeof(oam_pdu_hdr_t) + sizeof(cs_uint16);

        p_tlv_org_event_oam = (oam_ctc_tlv_org_spec_t *)(pdu + len);

        len +=  sizeof(oam_ctc_tlv_org_spec_t) - 1; /*remove seq_num field*/
        p_tlv_org_event_oam ->type = OAM_INFO_TLV_ORG_SPEC;
        p_tlv_org_event_oam ->length = sizeof(oam_ctc_tlv_org_spec_t) - 1;/*remove seq_num field*/
        memcpy(p_tlv_org_event_oam->oui,oam_oui_ctc,OAM_OUI_LEN);

        p_tlv_org_event_oam ->org_spec_value.alarm_id = htons(alarm_code);
        p_tlv_org_event_oam ->org_spec_value.time_stamp = 0;
        //p_tlv_org_event_oam ->org_spec_value.alarm_state = alarm_op;
        cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM,"%s. %d\n",__func__,__LINE__);
        if(oam_is_cuc_oam())
        {
            if(alarm_op == CTC_ALARM_RAISE)
            {
                p_tlv_org_event_oam ->org_spec_value.alarm_state = 1;
            }
            else if(alarm_op == CTC_ALARM_CLEAR)
            {
                p_tlv_org_event_oam ->org_spec_value.alarm_state = 0;
            }
        }
		else if(oam_ctc_version_is_30() || ctc_version_set == 0)
		{
			if(alarm_op == CTC_ALARM_RAISE)
			{
				p_tlv_org_event_oam ->org_spec_value.alarm_state = 0;
			}
			else if(alarm_op == CTC_ALARM_CLEAR)
			{
				p_tlv_org_event_oam ->org_spec_value.alarm_state = 1;
			}
		}
		else if(oam_ctc_version_is_21() || ctc_version_set == 1)
		{
			if(alarm_op == CTC_ALARM_RAISE)
			{
				p_tlv_org_event_oam ->org_spec_value.alarm_state = 1;
			}
			else if(alarm_op == CTC_ALARM_CLEAR)
			{
				p_tlv_org_event_oam ->org_spec_value.alarm_state = 0;
			}
		}
		else
		{
			if(alarm_op == CTC_ALARM_RAISE)
			{
				p_tlv_org_event_oam ->org_spec_value.alarm_state = 0;
			}
			else if(alarm_op == CTC_ALARM_CLEAR)
			{
				p_tlv_org_event_oam ->org_spec_value.alarm_state = 1;
			}
		}
		
cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM,"%s. %d\n",__func__,__LINE__);
        if(type == CTC_ONU_ALARM)
        {
            p_tlv_org_event_oam ->org_spec_value.object_type = htons(OAM_CTC_ALARM_TYPE_ONU);
            p_tlv_org_event_oam ->org_spec_value.instance_num = htonl(0xffffffff);
        }
        else if(type == CTC_PON_ALARM)
        {
            p_tlv_org_event_oam ->org_spec_value.object_type = htons(OAM_CTC_ALARM_TYPE_PON);
            p_tlv_org_event_oam ->org_spec_value.instance_num = htonl(0x00000001);
        }
        else if(type == CTC_PORT_ALARM)
        {
            p_tlv_org_event_oam ->org_spec_value.object_type = htons(OAM_CTC_ALARM_TYPE_UNI);
            p_tlv_org_event_oam ->org_spec_value.instance_num = htonl(0x01000000|port);
        }
cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM,"%s. %d\n",__func__,__LINE__);
        if(p_info->info_need)
        {
            if ( alarm_info_len==4) {
                alarm_infor_uint32 = alarm_info;
                alarm_infor_uint32=htonl (alarm_infor_uint32);
                memcpy( alarm_infor_buffer, &alarm_infor_uint32, 4 );
            } else {
                alarm_info = htonll(alarm_info);
                memcpy( alarm_infor_buffer, &alarm_info, 8 );
            }

            memcpy(p_tlv_org_event_oam->org_spec_value.alarm_info, alarm_infor_buffer, alarm_info_len);
            len += alarm_info_len;
            p_tlv_org_event_oam ->length += alarm_info_len;
        }

cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_ORG_OAM,"before ctc_oam_send\n");
        ctc_oam_send(oamif->port, pdu, len);

}


/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_set_alarm_config        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
cs_status ctc_oam_set_alarm_config(
        ctc_oam_alarm_class_e type,
        cs_uint32 portId,
        cs_uint16  alarm_code,
        cs_uint8 enable)
/*                                                                           */
/* INPUTS  : o type---alarm type                                          */
/*               o portid--- port id                                         */
/*               o alarm_code---code                                          */
/*               o enable---CTC_ALARM_ON/CTC_ALARM_OFF   */
/* OUTPUTS : -----                                                            */
/* RETURNS : o EPON_RETURN_XXX(see section 3.7 for return code definitions)*/
/* DESCRIPTION:                                                              */
/* API to set alarm on/off                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    ctc_oam_alarm_info_t *p_info = NULL;


    if(enable != CTC_ALARM_ON && enable != CTC_ALARM_OFF) {
        return CS_E_PARAM;
    }

    p_info = ctc_oam_get_alarm_info(type, portId, alarm_code);
    if(!p_info)
    {
        return CS_E_PARAM;
    }

    if(enable == p_info->enable)
    {
        /*Do nothing*/
        return CS_E_OK;
    }
        
    p_info->enable = enable;
    /*check the alarm status when the first set it to on or last set it off*/
    if(enable == CTC_ALARM_ON)
    {
        if(p_info->polling_need) 
        {
            g_alarm_polling_cb.polling_enable_counter++;
            if(g_alarm_polling_cb.polling_enable_counter == 1)
            {
                /*Start the polling time to check the alarm status period*/
                ctc_alarm_start_polling_timer();
            }
        }                       
    }
    else 
    {
        if(p_info->polling_need)
        {
            if(g_alarm_polling_cb.polling_enable_counter > 0)
            {
                g_alarm_polling_cb.polling_enable_counter--;
                if(g_alarm_polling_cb.polling_enable_counter == 0)
                {
                    ctc_alarm_stop_polling_timer();
                }
            }
        }
    }
    return CS_E_OK;        
        
}

#if 1
//added by zhuxh
cs_status set_loop_alarm_config(
        cs_port_id_t port_id,
        cs_boolean enable)
{
	cs_uint8 enable_alarm = 0;
	if(enable)
	{
		enable_alarm = CTC_ALARM_ON;
	}
	else
	{
		enable_alarm = CTC_ALARM_OFF;
	}

	return ctc_oam_set_alarm_config(CTC_PORT_ALARM, port_id, CTC_ALARM_PORT_LB, enable_alarm);
}


#endif

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_get_alarm_config                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
cs_status ctc_oam_get_alarm_config(
        ctc_oam_alarm_class_e type,
        cs_uint32 portId,
        cs_uint16  alarm_code,
        cs_uint32* alarm_config)
/*                                                                           */
/* INPUTS  : o alarm_code---alarm id                                           */
/*               o portId---port id                                          */
/* OUTPUTS : o raiseValue                                                            */
/* OUTPUTS : o clearValue                                                            */
/* RETURNS : o CS_E_XXX(see section 3.7 for return code definitions)*/
/* DESCRIPTION:                                                              */
/* API to get alarm threshold                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    ctc_oam_alarm_info_t *p_info = NULL;

    p_info = ctc_oam_get_alarm_info(type, portId, alarm_code);
    if(!p_info)
    {
        return CS_E_PARAM;
    }
       
    *alarm_config = p_info->enable;
    return CS_E_OK;

}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_set_alarm_threshold                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
cs_status ctc_oam_set_alarm_threshold(
        ctc_oam_alarm_class_e type,
        cs_uint32 portId,
        cs_uint16  alarm_code,
        cs_uint32 raiseValue,
        cs_uint32 clearValue)
/*                                                                           */
/* INPUTS  : o alarm_code---alarm id                                           */
/*               o raiseValue---raise value                                          */
/*               o clearValue---clear value                                         */
/*               o portId---port id                                          */
/* OUTPUTS : o ------                                                           */
/* RETURNS : o EPON_RETURN_XXX(see section 3.7 for return code definitions)*/
/* DESCRIPTION:                                                              */
/* API to set alarm threshold                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    ctc_oam_alarm_info_t *p_info = NULL;

    p_info = ctc_oam_get_alarm_info(type, portId, alarm_code);
    if(!p_info)
    {
        return CS_E_PARAM;
    }


    if(p_info->threshold_type == CTC_THRESH_SIGNED_LOW ||
       p_info->threshold_type == CTC_THRESH_SIGNED_HIGH)
    {
        p_info->raise_alarm_threshold.signValue = raiseValue;
        p_info->clear_alarm_threshold.signValue = clearValue;
        return CS_E_OK;
    }
    else if(p_info->threshold_type == CTC_THRESH_UNSIGNED_HIGH ||
              p_info->threshold_type == CTC_THRESH_UNSIGNED_LOW)
    {
        p_info->raise_alarm_threshold.unsignValue = raiseValue;
        p_info->clear_alarm_threshold.unsignValue = clearValue;
        return CS_E_OK;
    }
    else
    {
        return CS_E_ERROR;
    }  
}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_get_alarm_threshold                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
cs_status ctc_oam_get_alarm_threshold(
        ctc_oam_alarm_class_e type,
        cs_uint32 portId,
        cs_uint16  alarm_code,
        cs_uint32* raiseValue,
        cs_uint32 *clearValue)
/*                                                                           */
/* INPUTS  : o alarm_code---alarm id                                           */
/*               o portId---port id                                          */
/* OUTPUTS : o raiseValue                                                            */
/* OUTPUTS : o clearValue                                                            */
/* RETURNS : o EPON_RETURN_XXX(see section 3.7 for return code definitions)*/
/* DESCRIPTION:                                                              */
/* API to get alarm threshold                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    ctc_oam_alarm_info_t *p_info = NULL;

    p_info = ctc_oam_get_alarm_info(type, portId, alarm_code);
    if(!p_info)
    {
        return CS_E_PARAM;
    }


    if(p_info->threshold_type == CTC_THRESH_SIGNED_LOW ||
       p_info->threshold_type == CTC_THRESH_SIGNED_HIGH)
    {
        * raiseValue = p_info->raise_alarm_threshold.signValue ;
        *clearValue = p_info->clear_alarm_threshold.signValue;
         return CS_E_OK;

    }
    else if(p_info->threshold_type == CTC_THRESH_UNSIGNED_HIGH ||
              p_info->threshold_type == CTC_THRESH_UNSIGNED_LOW)
    {
         *raiseValue = p_info->raise_alarm_threshold.unsignValue;
         *clearValue = p_info->clear_alarm_threshold.unsignValue;
          return CS_E_OK;
    }
    else
    {
        return CS_E_ERROR;
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_alarm_threshold_get                 */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
cs_status ctc_oam_alarm_status_get(
        ctc_oam_alarm_class_e type,
        cs_uint32 portId,
        cs_uint16  alarm_code,
        cs_uint8 *status,
        cs_uint32 *info)
 /*                                                                           */
 /* INPUTS  :                                                               */
/*     none                                                                     */
/* OUTPUTS : ----                                                            */
/* DESCRIPTION:                                                              */
/* API to init alarm info                                         */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_status ret = CS_E_OK;
    cs_callback_context_t context;
    cs_sdl_pon_laser_mode_t mode;
    /*add code here for alarm status get*/
    /* if alarm raise, return status=1, else return status=0 */
    switch(alarm_code)
    {
        case CUC_OUTOFTIME_TX_SLOT:
            epon_request_onu_pon_laser_mode_get(context, 0, 0, &mode);
            if(mode == 3)
            {
                *status = 1;
            }
            else
            {
                *status = 0;
            }
            *info = 1;
            break;
        default:
            ret = CS_E_ERROR;
    }
    
    return ret;
}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_alarm_current_value_get                 */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
cs_status ctc_oam_alarm_current_value_get(
        ctc_oam_alarm_class_e type,
        cs_uint32 portId,
        cs_uint16  alarm_code,
        cs_uint32 *value)
 /*                                                                           */
 /* INPUTS  :                                                               */
/*     none                                                                     */
/* OUTPUTS : ----                                                            */
/* DESCRIPTION:                                                              */
/* API to init alarm info                                         */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_status ret = CS_E_OK;
    cs_int16 tem= 0;
    cs_uint16 svcc = 0, tx_bias = 0, tx_power = 0, rx_power = 0;
    cs_uint32 tmp = 0;
    cs_callback_context_t   context;

    ret = cs_plat_opm_status_get(context,0,0,&tem, &svcc, &tx_bias, &tx_power, &rx_power);
    cs_log_msg(IROS_LOG_LEVEL_DBG0, IROS_MID_APP, "temp=%d,svcc=%d,bias=%d,tx_pow=%d, rx_pow=%d\n",
                                 tem, svcc, tx_bias, tx_power, rx_power);
    
    switch(alarm_code)
    {
        case CTC_ALARM_PON_TEMP_HIGH:
        case CTC_ALARM_PON_TEMP_LOW:
        case CTC_WARN_PON_TEMP_HIGH:
        case CTC_WARN_PON_TEMP_LOW:
            tmp = (cs_uint32)tem;
            break;

        case CTC_ALARM_RX_POW_HIGH:
        case CTC_ALARM_RX_POW_LOW:
        case CTC_WARN_RX_POW_HIGH:
        case CTC_WARN_RX_POW_LOW:
            tmp = (cs_uint32)rx_power;
            break;

        case CTC_ALARM_TX_POW_HIGH:
        case CTC_ALARM_TX_POW_LOW:
        case CTC_WARN_TX_POW_HIGH:
        case CTC_WARN_TX_POW_LOW:
            tmp = (cs_uint32)tx_power;
            break;

        case CTC_ALARM_TX_BIAS_HIGH:
        case CTC_ALARM_TX_BIAS_LOW:
        case CTC_WARN_TX_BIAS_HIGH:
        case CTC_WARN_TX_BIAS_LOW:
            tmp = (cs_uint32)tx_bias;
            break;

        case CTC_ALARM_VCC_HIGH:
        case CTC_ALARM_VCC_LOW:
        case CTC_WARN_VCC_HIGH:
        case CTC_WARN_VCC_LOW:
            tmp = (cs_uint32)svcc;
            break;
            
        default:
            ret = CS_E_ERROR;
            break;
    }

    *value = tmp;
    return ret;
}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_alarm_polling_handle                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
void ctc_oam_alarm_polling_handle(
            ctc_oam_alarm_class_e type,
            cs_port_id_t port,
            cs_uint32 num,
            ctc_oam_alarm_info_t *p_info)
 /*                                                                           */
 /* INPUTS  :                                                               */
/*     none                                                                     */
/* OUTPUTS : ----                                                            */
/* DESCRIPTION:                                                              */
/* API to init alarm info                                         */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint32 i = 0;
    cs_uint32 unsign_value = 0;
    cs_int32 sign_value = 0;
    cs_uint8 status = 0;

    for(i = 0; i < num; i++)
    {
        if(p_info[i].enable == CTC_ALARM_ON && p_info[i].polling_need)
        {
            /*need check*/
            if(p_info[i].threshold_type != CTC_THRESH_UNSUPPORT)
            {
                /*get current value to compare with threshold*/
                if(CS_E_OK != ctc_oam_alarm_current_value_get(type, port, p_info[i].alarm_code, &unsign_value))
                {
                    continue;
                }
                sign_value = unsign_value;
                switch(p_info[i].threshold_type)
                {
                    case CTC_THRESH_SIGNED_HIGH:
                        if(sign_value >= p_info[i].raise_alarm_threshold.signValue)
                        {
                            ctc_oam_build_alarm_event(type, port, p_info[i].alarm_code, CTC_ALARM_RAISE, (cs_uint64)sign_value);
                        }
                        else if(sign_value <  p_info[i].clear_alarm_threshold.signValue)
                        {
                            ctc_oam_build_alarm_event(type, port, p_info[i].alarm_code, CTC_ALARM_CLEAR, (cs_uint64)sign_value);
                        }
                        break;
                    case CTC_THRESH_UNSIGNED_HIGH:
                        if(unsign_value >= p_info[i].raise_alarm_threshold.unsignValue)
                        {
                            ctc_oam_build_alarm_event(type, port, p_info[i].alarm_code, CTC_ALARM_RAISE, (cs_uint64)unsign_value);
                        }
                        else if(unsign_value <  p_info[i].clear_alarm_threshold.unsignValue)
                        {
                            ctc_oam_build_alarm_event(type, port, p_info[i].alarm_code, CTC_ALARM_CLEAR, (cs_uint64)unsign_value);
                        }
                        break;
                    case CTC_THRESH_SIGNED_LOW:
                        if(sign_value <= p_info[i].raise_alarm_threshold.signValue)
                        {
                            ctc_oam_build_alarm_event(type, port, p_info[i].alarm_code, CTC_ALARM_RAISE, (cs_uint64)sign_value);
                        }
                        else if(sign_value > p_info[i].clear_alarm_threshold.signValue)
                        {
                            ctc_oam_build_alarm_event(type, port, p_info[i].alarm_code, CTC_ALARM_CLEAR, (cs_uint64)sign_value);
                        }
                        break;
                    case CTC_THRESH_UNSIGNED_LOW:
                        if(unsign_value <= p_info[i].raise_alarm_threshold.unsignValue)
                        {
                            ctc_oam_build_alarm_event(type, port, p_info[i].alarm_code, CTC_ALARM_RAISE, (cs_uint64)unsign_value);
                        }
                        else if(unsign_value > p_info[i].clear_alarm_threshold.unsignValue)
                        {
                            ctc_oam_build_alarm_event(type, port, p_info[i].alarm_code, CTC_ALARM_CLEAR, (cs_uint64) unsign_value);
                        }
                        break;
                    default:
                        break;
                }
            }
            else
            {
                /*not threshold type, then get status and info*/
                if(CS_E_OK == ctc_oam_alarm_status_get(type, port, p_info[i].alarm_code, &status, &unsign_value))
                {
                    if(status == 1)
                    {
                        ctc_oam_build_alarm_event(type, port, p_info[i].alarm_code, CTC_ALARM_RAISE, (cs_uint64)unsign_value);
                    }
                    else
                    {
                        ctc_oam_build_alarm_event(type, port, p_info[i].alarm_code, CTC_ALARM_CLEAR, (cs_uint64)unsign_value);
                    }
                }
                else
                    continue;
            }
        }
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_alarm_polling                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8016                                                       */
/*                                                                           */
void ctc_oam_alarm_polling()
 /*                                                                           */
 /* INPUTS  :                                                               */
/*     none                                                                     */
/* OUTPUTS : ----                                                            */
/* DESCRIPTION:                                                              */
/* API to init alarm info                                         */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint32 num = 0;
    cs_uint8 i = 0;
    ctc_oam_alarm_info_t *p_info = NULL;
    
    if(!ctc_oam_discovery_result_get())
    {
        cs_log_msg(IROS_LOG_LEVEL_DBG0, IROS_MID_APP, "alarm polling wait for discovery finished\n");
        return;
    }
    /*polling pon alarm*/
    ctc_oam_get_alarm_info_table(CTC_PON_ALARM, 0, &num, &p_info);
    ctc_oam_alarm_polling_handle(CTC_PON_ALARM, 0, num, p_info);

    /*polling onu alarm*/
    ctc_oam_get_alarm_info_table(CTC_ONU_ALARM, 0, &num, &p_info);
    ctc_oam_alarm_polling_handle(CTC_ONU_ALARM, 0, num, p_info);

    /*Polling port alarm*/
    for(i = 0; i < CTC_MAX_ONU_UNI_PORTS; i++)
    {
        ctc_oam_get_alarm_info_table(CTC_PORT_ALARM, i+1, &num, &p_info);
        ctc_oam_alarm_polling_handle(CTC_PORT_ALARM, i+1, num, p_info);
    }
    
}


#endif


