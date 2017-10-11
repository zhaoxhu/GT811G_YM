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

Copyright (c) 2011 by Cortina Systems Incorporated
****************************************************************************/

#include <string.h>
#include "oam_common.h"
#include "oam_api.h"
#include "oam_core.h"
#include "oam_mux_par.h"
#include "oam_sdl_if.h"
#include "oam_supp.h"

/*
 * OAM handler for discovery and extend org OAM 
 */
static oam_vendor_register_handler_t  oam_oui_handlers[OAM_MAX_OUI];

/*
 * OAM handler for standard oam for special code process
 */
static oam_std_oam_handler_t     g_oam_handlers[OAM_MAX_STD_HDL_NUM];

/*
 * Dump all org oam packet switch
 */
static oam_dump_pkt_type_e g_oam_dump_pkt_type = OAM_DUMP_TYPE_NONE;

/*****************************************************************************/
/* $rtn_hdr_start  oam_dump_pkt                                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_dump_pkt(
        char *comment, 
        cs_port_id_t port, 
        cs_uint8 *buffer, 
        cs_uint32 len)
/*                                                                           */
/* INPUTS  : o comment - debug info tag                                      */
/*           o port - llid port                                              */
/*           o buffer - dump packet                                          */
/*           o len - dump packet len                                         */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* Dump packet to the screen                                                 */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_pdu_hdr_t *pdu = (oam_pdu_hdr_t*) buffer;
    cs_boolean dump = FALSE;
    OAM_ASSERT(comment != NULL 
            && buffer != NULL);
    
    if(g_oam_dump_pkt_type == OAM_DUMP_TYPE_NONE){
        return;
    }
    
    /*always dump too short pkt*/
    if(len < sizeof(oam_pdu_hdr_t)){
        oam_printf("\n\n%s:pkt len = %d, port = %d ",comment, len,port);
        cs_buf_print(buffer,len);
        return;
    }

    if((pdu->code == OAM_PDU_CODE_ORG_SPEC 
            && g_oam_dump_pkt_type == OAM_DUMP_TYPE_ORG)
             || (pdu->code == OAM_PDU_CODE_EVENT
            && g_oam_dump_pkt_type == OAM_DUMP_TYPE_EVT)
            || g_oam_dump_pkt_type ==OAM_DUMP_TYPE_ALL ){
        dump = TRUE;
    }

    if(dump){
        oam_printf("\n\n%s:pkt len = %d, port = %d ",comment, len,port);
        cs_buf_print(buffer,len);
        oam_printf("\n");
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_set_pkt_dump                                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_set_pkt_dump(oam_dump_pkt_type_e type)
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* set the onu oam pkt dump                                                  */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    if(g_oam_dump_pkt_type == type){
        g_oam_dump_pkt_type = OAM_DUMP_TYPE_NONE;
        oam_printf("disable oam packet dump \n");
    }
    else{
        g_oam_dump_pkt_type = type;
        oam_printf("enable %s oam packet dump \n",
                type == OAM_DUMP_TYPE_ALL?"all":"org");
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_is_oui_equal                                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_boolean oam_is_oui_equal(oam_oui_t oui1, oam_oui_t oui2)
/*                                                                           */
/* INPUTS  : o oui1 - first oui                                              */
/*           o oui2 - second oui                                             */
/* OUTPUTS : TRUE/FALSE                                                      */
/* DESCRIPTION:                                                              */
/* Api to compare the two ouis are equal or not                              */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{   
    OAM_ASSERT_RET(oui1 != NULL,FALSE);
    OAM_ASSERT_RET(oui2 != NULL,FALSE);

    if(memcmp(oui1, oui2, OAM_OUI_LEN))
        return FALSE;
    else
        return TRUE;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_std_handler_register                                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_status oam_std_handler_register(
        cs_uint8 code,
        oam_vendor_handler_t handler)
/*                                                                           */
/* INPUTS  : o code - register callback code                                 */
/*           o handlers - register hanlders for the vendor                   */
/* OUTPUTS : reigister status                                                */
/* DESCRIPTION:                                                              */
/* register the std handlers to the system                                   */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint32 i = 0;
    OAM_ASSERT_RET(code != 0,FALSE);
    
    for(i = 0; i < OAM_MAX_STD_HDL_NUM; i++){
        if(g_oam_handlers[i].code  == code){
            OAM_LOG("The code is already register \n"); 
            return CS_E_PARAM;
        }
    }
    /*oui doesn't exist in table, register the handler */
    for(i = 0; i < OAM_MAX_STD_HDL_NUM; i++){
        if(0 == g_oam_handlers[i].code){
            g_oam_handlers[i].code = code;
            g_oam_handlers[i].code_handler = handler;
            return CS_E_OK;
        }
    } 

    return CS_E_RESOURCE;
}


/*****************************************************************************/
/* $rtn_hdr_start  oam_std_handler                                           */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_boolean  oam_std_handler( 
        cs_uint8 code,
        cs_port_id_t port,
        cs_uint8 * frame,
        cs_uint32 len)
/*                                                                           */
/* INPUTS  : o code - register code                                          */
/*           o port - port id                                                */
/*           o frame - the oam frame to process                              */
/*           o len - the length of frame                                     */
/* OUTPUTS : TRUE - call the handle success                                  */
/*           FALSE - No handle register                                      */
/* DESCRIPTION:                                                              */
/* call the std oam hanlder according to the code                            */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 i = 0;
    oam_vendor_handler_t handle = NULL;
    
    OAM_ASSERT_RET(frame != NULL,FALSE);
    OAM_ASSERT_RET(code != 0 , FALSE);

    for(i = 0; i < OAM_MAX_STD_HDL_NUM; i++){
        if(g_oam_handlers[i].code  == code ){
            handle = g_oam_handlers[i].code_handler;
        }
    }

    if(handle == NULL){
        return FALSE;
    }

    handle(port,frame,len);

    return TRUE;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_vendor_handlers_init                                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_vendor_handlers_init(oam_vendor_handlers_t * handlers)
/*                                                                           */
/* INPUTS  : o handlers - vendor oam handles                                 */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* Init the vendor oam handles to NULL                                       */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    OAM_ASSERT(handlers != NULL);

    memset(handlers, 0x00,sizeof(oam_vendor_handlers_t));

}

/*****************************************************************************/
/* $rtn_hdr_start  oam_vendor_register_handler_init                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_vendor_register_handler_init()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* Init all vendors oam handles to NULL                                      */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 i = 0 ; 

    memset(oam_oui_handlers,0x00, 
            sizeof(oam_vendor_register_handler_t) * OAM_MAX_OUI);
    memset(g_oam_handlers,0x00,sizeof(oam_std_oam_handler_t)*OAM_MAX_STD_HDL_NUM);

    for(i = 0; i < OAM_MAX_OUI; i++){
        oam_oui_handlers[i].valid = OAM_FALSE;
        oam_vendor_handlers_init(&oam_oui_handlers[i].handlers);
    }

}

/*****************************************************************************/
/* $rtn_hdr_start  oam_vendor_handler_register                               */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_status oam_vendor_handler_register(
        oam_oui_t   oui,
        oam_vendor_handlers_t   handlers)
/*                                                                           */
/* INPUTS  : o oui - register vendor oui                                     */
/*           o handlers - register hanlders for the vendor                   */
/* OUTPUTS : reigister status                                                */
/* DESCRIPTION:                                                              */
/* register the vendor handlers to the system                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 i = 0;

    if(oui[0] == 0 && oui[1] == 0 && oui[2] == 0)
        return CS_E_PARAM;

    /*if oui already registered, just overwrite the callback function */
    for(i = 0; i < OAM_MAX_OUI; i++){
        if(!oam_oui_handlers[i].valid)
            continue;

        if(oam_is_oui_equal(oui, oam_oui_handlers[i].oui)){
            oam_oui_handlers[i].handlers = handlers;
            return CS_E_OK;
        }
    }

    /*oui doesn't exist in table, register the handler */
    for(i = 0; i < OAM_MAX_OUI; i++){
        if(!oam_oui_handlers[i].valid){
            memcpy(oam_oui_handlers[i].oui, oui, OAM_OUI_LEN);
            oam_oui_handlers[i].handlers = handlers;
            oam_oui_handlers[i].valid = OAM_TRUE;
            return CS_E_OK;
        }
    }

    return CS_E_RESOURCE;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_vendor_handler                                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_boolean  oam_vendor_handler( 
        oam_oui_t oui, 
        oam_vendor_handler_type_e type,
        cs_port_id_t port,
        cs_uint8 * frame,
        cs_uint32 len)
/*                                                                           */
/* INPUTS  : o oui - register vendor oui                                     */
/*           o type - hanlder type                                           */
/*           o port - port id                                                */
/*           o frame - the oam frame to process                              */
/*           o len - the length of frame                                     */
/* OUTPUTS : TRUE - call the handle success                                  */
/*           FALSE - No handle register                                      */
/* DESCRIPTION:                                                              */
/* call the vendor org oam hanlder                                           */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 i = 0;
    oam_vendor_handler_t handle = NULL;
    
    OAM_ASSERT_RET(frame != NULL,FALSE);

    switch(type){
        case OAM_VENDOR_HANDLE_ORG:
           for(i = 0; i < OAM_MAX_OUI; i++){
               if(oam_oui_handlers[i].valid
                       && oam_is_oui_equal(oui, oam_oui_handlers[i].oui)){
                   handle = oam_oui_handlers[i].handlers.org_handler;
                   break;
               }
           }
           break;

        default:
            handle = NULL;
    }

    if(handle == NULL){
        OAM_LOG_INFO("No handler found ,oui = %#x:%#x:%#x, handle type = %d\n",
                oui[0],oui[1],oui[2], type);
        return FALSE;
    }

    handle(port,frame,len);

    return TRUE;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_vendor_build_handler                                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_uint32  oam_vendor_build_handler( 
        oam_vendor_handler_type_e type,
        cs_port_id_t port,
        cs_uint8 * frame,
        cs_uint32 len)
/*                                                                           */
/* INPUTS  : o type - hanlder type                                           */
/*           o port - port id                                                */
/*           o frame - the oam frame to process                              */
/*           o len - the length of frame                                     */
/* OUTPUTS : TRUE - call the handle success                                  */
/*           FALSE - No handle register                                      */
/* DESCRIPTION:                                                              */
/* call the vendor oam info build hanlder                                    */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 i = 0;
    oam_vendor_build_handler_t handle = NULL;
    
    OAM_ASSERT_RET(frame != NULL,-1);

    switch(type){
         case OAM_VENDOR_HANDLE_EXT_INFO_BUILD:
           for(i = 0; i < OAM_MAX_OUI; i++){
               if(oam_oui_handlers[i].valid){
                   handle = oam_oui_handlers[i].handlers.ext_info_build_handler;
                   if(handle != NULL){
                       break;
                   }
               }
           }
           break;

        case OAM_VENDOR_HANDLE_INFO_BUILD:
            for(i = 0; i < OAM_MAX_OUI; i++){
                if(oam_oui_handlers[i].valid){
                    handle = oam_oui_handlers[i].handlers.info_build_handler;
                    if(handle != NULL){
                        break;
                    }
                }
            }
           break;
        default:
            handle = NULL;
    }

    if(handle == NULL){
        return -1;
    }

    return handle(port,frame,len);
}


/*****************************************************************************/
/* $rtn_hdr_start  oam_vendor_proc_handler                                   */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_boolean  oam_vendor_proc_handler( 
        oam_vendor_handler_type_e type,
        cs_port_id_t port,
        cs_uint8 * frame,
        cs_uint32 len)
/*                                                                           */
/* INPUTS  : o type - hanlder type                                           */
/*           o port - port id                                                */
/*           o frame - the oam frame to process                              */
/*           o len - the length of frame                                     */
/* OUTPUTS : TRUE - call the handle success                                  */
/*           FALSE - No handle register                                      */
/* DESCRIPTION:                                                              */
/* call the vendor oam info process hanlder                                  */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 i = 0;
    oam_vendor_handler_t handle = NULL;
    
    OAM_ASSERT_RET(frame != NULL,FALSE);

    switch(type){
        case OAM_VENDOR_HANDLE_EXT_INFO_PROC:
            for(i = 0; i < OAM_MAX_OUI; i++){
                if(oam_oui_handlers[i].valid){
                    handle = oam_oui_handlers[i].handlers.ext_info_proc_handler;
                    if(handle != NULL){
                        break;
                    }
                }
            }
           break;
        case OAM_VENDOR_HANDLE_INFO_PROC:
            for(i = 0; i < OAM_MAX_OUI; i++){
                if(oam_oui_handlers[i].valid){
                    handle = oam_oui_handlers[i].handlers.info_proc_handler;
                    if(handle != NULL){
                        break;
                    }
                }
            }
           break;
        default:
            handle = NULL;
    }

    if(handle == NULL){
        return FALSE;
    }

    handle(port,frame,len);

    return TRUE;
}

