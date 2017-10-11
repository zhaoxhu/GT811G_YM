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

#include "oam_common.h"
#include "oam_sm.h"
#include "sdl_api.h"
#include "app_gpio.h"

/*****************************************************************************/
/* $rtn_hdr_start  oam_sdl_send_pkt                                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_status oam_sdl_send_pkt(
        cs_port_id_t port,
        cs_uint8 * buf,
        cs_uint32 len)
/*                                                                           */
/* INPUTS  : o port - send packet from this port                             */
/*           o buf - oam packet                                           */
/*           o len - oam packet len                                          */
/* OUTPUTS : SDL status                                                      */
/* DESCRIPTION:                                                              */
/* send the packet to the olt                                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_callback_context_t     context;
    cs_status ret = CS_E_OK;

    OAM_ASSERT_RET(buf != NULL,CS_E_PARAM);

    ret = epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, buf, len, port, ONU_CPU_PKT_PRIO);

    if(ret != CS_E_OK){
        OAM_LOG("Send packet failed,ret = %d  \n",ret);
    }
    return ret;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_sdl_get_llid_mac                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_status oam_sdl_get_llid_mac(
        cs_llid_t llid,
        cs_mac_t * mac)
/*                                                                           */
/* INPUTS  : o llid - onu llid                                               */
/*           o mac - llid mac address                                        */
/* OUTPUTS : SDL status                                                      */
/* DESCRIPTION:                                                              */
/* get the mac address for the llid                                          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_callback_context_t     context;
    cs_status ret = CS_E_OK;

    OAM_ASSERT_RET(mac != NULL,CS_E_PARAM);

    ret = epon_request_onu_pon_mac_addr_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
                                                               mac);
    
    if(ret != CS_E_OK){
        OAM_LOG("Get Llid %d mac address failed \n",llid);
    }

    return ret;

}

/*****************************************************************************/
/* $rtn_hdr_start  oam_sdl_port_lpbk_set                                     */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_status oam_sdl_port_lpbk_set(
        cs_port_id_t port,
        cs_uint8 mux_action,
        cs_uint8 par_action)
/*                                                                           */
/* INPUTS  : o port - onu llid                                               */
/*           o mux_action - mux action                                       */
/*           o par_action - par action                                       */
/* OUTPUTS : SDL status                                                      */
/* DESCRIPTION:                                                              */
/* set the loopback to the sdl                                               */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    static cs_boolean g_oam_loopback = FALSE;
    cs_uint8 depth = 0;
    cs_callback_context_t     context;
    cs_sdl_port_loopback_t status = SDL_PORT_LOOPBACK_NONE; 
    cs_port_id_t  port_id, i;
	cs_uint8 port_num = 0 ;
    cs_status ret = CS_E_OK;

    if ((par_action == OAM_PAR_ACTION_LB) &&
            (mux_action == OAM_MUX_ACTION_DISCARD)) {
        status = SDL_PORT_LOOPBACK_TX2RX;
        if(g_oam_loopback == TRUE){
            return CS_E_OK;
        }
        else{ 
            g_oam_loopback = TRUE;
        }
    }

    if ((par_action == OAM_PAR_ACTION_FWD) &&
            (mux_action == OAM_MUX_ACTION_FWD)) {
        status= SDL_PORT_LOOPBACK_NONE;
        if(g_oam_loopback == FALSE){
            return CS_E_OK;
        }
        else{ 
            g_oam_loopback = FALSE;
        }
    }

    ret = startup_config_read(CFG_ID_LOOPBACK_DEPTH, 1, &depth);
    if(ret != CS_E_OK){
        OAM_LOG("Get CFG_ID_LOOPBACK_DEPTH from start config failed, ret = %d \n",
                ret);
        return CS_E_ERROR;
    }

	ret = startup_config_read(CFG_ID_SWITCH_PORT_NUM, sizeof(port_num), &port_num);
    if(ret != CS_E_OK){
        OAM_LOG("Get CFG_ID_SWITCH_PORT_NUM from start config failed, ret = %d \n",
                ret);
        return CS_E_ERROR;
    }

	if(depth == 1){
		if(4 == port_num){
			/*bug39891*/
			for(i=0; i<4; i++){
			    ret = epon_request_onu_port_lpbk_set(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, i+1, status);
			    if(CS_E_OK != ret)
			    {
			        cs_printf("%s() --- set port %d loopback fail!!! ret=%d\n", __FUNCTION__, port, ret);
			    }
		        OAM_DEBUG("oam loopback set port: %d, status %d\n", i+1, status);					
			}		
	    }
		else{
			port_id = CS_UNI_PORT_ID1;
		    OAM_DEBUG("oam loopback set port: %d, status %d\n", port_id, status);
		    ret = epon_request_onu_port_lpbk_set(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
		            port_id, status);			
		}
	}
    else{
        port_id = CS_PON_PORT_ID;
	    OAM_DEBUG("oam loopback set port: %d, status %d\n", port_id, status);
	    ret = epon_request_onu_port_lpbk_set(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
	            port_id, status);
	}

    if(ret != CS_E_OK){
        OAM_LOG("epon_request_onu_port_lpbk_set with port %d , status %d failed, ret = %d \n",
                port_id,status, ret);
        return CS_E_ERROR;
    }

    if(status != SDL_PORT_LOOPBACK_NONE) {
        /* pon led -> ora on */
        cs_led_proc(LED_PON, LED_YELLOW, LED_ON);
        cs_led_los_handle();
    }
    else {
        cs_led_proc(LED_PON, LED_GREEN, LED_ON);
        cs_led_los_handle();
    }
    return CS_E_OK;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_sdl_get_buf_int_status                                */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_status oam_sdl_get_buf_int_status(cs_boolean *status)
/*                                                                           */
/* INPUTS  : o status - onu buf int status                                   */
/* OUTPUTS : SDL status                                                      */
/* DESCRIPTION:                                                              */
/* get the bm status from the sdl                                            */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    //TODO:Geesun remove the MACRO after the SDL complete
#ifdef HAVE_NTT_OAM
    cs_callback_context_t     context;
    cs_status ret = CS_E_OK;

    cs_boolean ge = FALSE, pon = FALSE, ma = FALSE;

    OAM_ASSERT_RET(status != NULL,CS_E_PARAM);

    ret = epon_request_onu_buf_int_status_get(context, 
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
            &ge, &pon, &ma);

    if(ret != CS_E_OK) {
        OAM_LOG("epon_request_onu_buf_int_status_get get failed,ret = %d \n",ret);
        *status = FALSE;
        return ret;
    }

    if(ge || pon || ma)
        *status = TRUE;
    else
        *status = FALSE;

    return CS_E_OK;
#else
    return CS_E_OK;
#endif
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_sdl_get_enc_mode                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_status oam_sdl_get_enc_mode(cs_uint8 *mode)
/*                                                                           */
/* INPUTS  : o mode - onu encrypt mode                                       */
/* OUTPUTS : SDL status                                                      */
/* DESCRIPTION:                                                              */
/* get the onu encrypt mode from the sdl                                     */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
#ifdef HAVE_NTT_OAM
    cs_sdl_enc_mode_t enc;
    cs_callback_context_t     context;
    
    epon_request_onu_enc_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
            &enc);

    *mode = enc;
#endif
    return CS_E_OK;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_sdl_set_enc_mode                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_status oam_sdl_set_enc_mode(cs_uint8 mode)
/*                                                                           */
/* INPUTS  : o mode - onu encrypt mode                                       */
/* OUTPUTS : SDL status                                                      */
/* DESCRIPTION:                                                              */
/* set the onu encrypt mode from the sdl                                     */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
#ifdef HAVE_NTT_OAM
    cs_callback_context_t     context;
    
    epon_request_onu_enc_set(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,
            (cs_sdl_enc_mode_t)mode);
#endif
    return CS_E_OK;
}


/*****************************************************************************/
/* $rtn_hdr_start  oam_sdl_get_llid                                          */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_uint16 oam_sdl_get_llid()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : llid number                                                     */
/* DESCRIPTION:                                                              */
/* get the onu llid from the sdl                                             */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint16 llid =0;
    cs_callback_context_t     context;

    epon_request_onu_mpcp_llid_get(context,
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,&llid);

    return llid;
}
