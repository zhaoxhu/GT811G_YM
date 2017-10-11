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
#include "plat_common.h"
#include "sdl_event_cmn.h"
#include "cs_timer.h"
#include "app_timer.h"
#include "packet.h"

#include "oam_common.h"
#include "oam_core.h"
#include "oam_supp.h"
#include "oam_event.h"
#include "oam_control.h"
#include "oam_timer.h"
#include "oam_plat_if.h"
#include "oam_sdl_if.h"
#include "oam_std_pdu.h"
#include "oam_client.h"
#ifdef HAVE_CTC_OAM
#include "oam_ctc_pdu.h"
#include "oam_ctc_client.h"
#include "app_oam_ctc_adapt.h"
#endif
#ifdef HAVE_NTT_OAM
#include "oam_ntt_client.h"
#endif

#ifdef HAVE_KT_OAM
#include "oam_kt_client.h"
#endif

//TODO:Find a solution for this global
cs_port_id_t g_oam_port;

static app_timer_msg_t g_oam_timer;
static app_timer_msg_t g_oam_pon_event_timer;
static cs_int32 g_oam_timer_handler = 0;


cs_uint8 oam_pkt_proc(cs_pkt_t *pPkt);
cs_status oam_pkt_parser(cs_pkt_t *pPkt);
cs_uint32 oam_special_tx_laser_ctrl_set(
	cs_uint8 * pRecv);
/*****************************************************************************/
/* $rtn_hdr_start  oam_port_reg_change_event_handler                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static void oam_port_reg_change_event_handler(
        cs_uint16 dev, 
        cs_int32 type, 
        void* msg)
/*                                                                           */
/* INPUTS  : o dev - device id                                               */
/*           o type - event type                                             */
/*           o msg - event message                                           */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process the llid register event                                           */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    sdl_event_reg_t *reg = NULL;

    OAM_ASSERT(msg != NULL);
    
    reg = (sdl_event_reg_t*)msg;

    if(reg->reg)
    {
        OAM_LOG_INFO("onu register, llid = %d  \n",reg->llid);
        oam_llid_registration(reg->llid); 

#ifdef HAVE_NTT_OAM
        oam_ntt_llid_registration(reg->llid);
#endif

#ifdef HAVE_KT_OAM
        oam_kt_llid_registration(reg->llid);
#endif
#ifdef HAVE_CTC_OAM
        oam_ctc_llid_registration(reg->llid);
#endif
    }
    else
    {
        OAM_LOG_INFO("onu deregister, llid = %d  \n",reg->llid);
        oam_llid_deregistration(reg->llid);

#ifdef HAVE_NTT_OAM
        oam_ntt_llid_deregistration(reg->llid);
#endif

#ifdef HAVE_KT_OAM
        oam_kt_llid_deregistration(reg->llid);
#endif
#ifdef HAVE_CTC_OAM
        oam_ctc_llid_deregistration(reg->llid);
#endif

    }
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_timer_init                                            */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static void oam_timer_init()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* oam keep alive timer init                                                 */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    g_oam_timer.msg_type = IROS_MSG_TIME_OUT;
    g_oam_timer.timer_type= APP_TIMER_OAM;
    
    g_oam_timer_handler = cs_msg_circle_timer_add(app_msg_q,
            ONU_OAM_INTERVAL, (void *)&g_oam_timer);
    if(g_oam_timer_handler == CS_INVALID_TIMER)
    {
        OAM_LOG("cs_msg_circle_timer_add failed.\n");
    }
}

void oam_timer_restart()
{
    if(g_oam_timer_handler != CS_INVALID_TIMER)
    {
        cs_timer_del(g_oam_timer_handler);
        g_oam_timer_handler = CS_INVALID_TIMER;
    }

    g_oam_timer_handler = cs_msg_circle_timer_add(app_msg_q,
            ONU_OAM_INTERVAL_NEW, (void *)&g_oam_timer);
    if(g_oam_timer_handler == CS_INVALID_TIMER)
    {
        OAM_LOG("cs_msg_circle_timer_add failed.\n");
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_event_init                                            */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static void oam_event_init()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* oam event init                                                            */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_err_event_init();

    cs_uint32 timerHandle = 0;
    g_oam_pon_event_timer.msg_type = IROS_MSG_TIME_OUT;
    g_oam_pon_event_timer.timer_type = APP_TIMER_OAM_POLLING;
    
    timerHandle = cs_msg_circle_timer_add(app_msg_q,
            OAM_LINK_EVENT_POLL_ACCURACY,
            (void *)&g_oam_pon_event_timer);
    if(timerHandle == CS_INVALID_TIMER)
    {
        OAM_LOG("cs_msg_circle_timer_add failed.\n");
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_link_complete                                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_link_complete()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* call back for oam link discovery completed                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    //oam_link_event_reset();
    //onu_pon_event_process(EPON_ONU_OAM_DISCOVERIED);
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_link_lost                                             */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_link_lost()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* call back for oam link lost                                               */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    //onu_pon_event_process(EPON_ONU_OAM_RESTART);
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_init                                                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_init()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* oam init process                                                          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_core_init();

    oam_control_handler_init(oam_link_complete,oam_link_lost,
            (oam_build_oampdu_t)oam_build_pdu_info);
    
    oam_vendor_register_handler_init();

    oam_timer_init();
    
    oam_event_init();

    oam_plat_onu_evt_reg(EPON_EVENT_REG_CHANGE,
            (void*)oam_port_reg_change_event_handler, NULL);

	#ifdef HAVE_ONU_REGISTER_LAST
	
	#else
    app_pkt_reg_parser(CS_PKT_OAM, oam_pkt_parser);
    app_pkt_reg_handler(CS_PKT_OAM, oam_pkt_proc);
	#endif
    oam_printf("OAM init done \n");
}

#ifdef HAVE_ONU_REGISTER_LAST
void oam_init_step2(void)
{
	app_pkt_reg_parser(CS_PKT_OAM, oam_pkt_parser);
    app_pkt_reg_handler(CS_PKT_OAM, oam_pkt_proc);
}
#endif

/*****************************************************************************/
/* $rtn_hdr_start  oam_orgspec_handle                                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_orgspec_handle(cs_uint8 *frame, cs_uint32 len)
/*                                                                           */
/* INPUTS  : o frame - oam packet                                            */
/*           o len - oam length                                              */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process the org special oam                                               */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_pdu_orgspec_t *pdu = (oam_pdu_orgspec_t *) frame;
    OAM_ASSERT(frame != NULL);

    if(!oam_vendor_handler(pdu->oui,
                OAM_VENDOR_HANDLE_ORG,
                0,(cs_uint8*)pdu,len)){
        OAM_LOG("received unknown OUI 0x%x 0x%x 0x%x\n", pdu->oui[0],
            pdu->oui[1], pdu->oui[2]);
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_pkt_parser                                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_status oam_pkt_parser(cs_pkt_t *pPkt)
/*                                                                           */
/* INPUTS  : o frame_ptr - oam packet                                        */
/*           o len - oam length                                              */
/*           o port - received the oam source port                           */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process any oam packet                                                    */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_pdu_hdr_t *hdr = NULL;

    hdr = (oam_pdu_hdr_t*)(pPkt->data + pPkt->offset + pPkt->tag_num * sizeof(cs_vlan_hdr_t ));

    if(ntohs(hdr->eth_type) == EPON_ETHERTYPE_SLOW)
    {
        pPkt->pkt_type = CS_PKT_OAM; 
        return CS_E_OK;
    }

    return CS_E_NOT_SUPPORT;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_pkt_proc                                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_uint8 oam_pkt_proc(cs_pkt_t *pPkt)
/*                                                                           */
/* INPUTS  : o frame_ptr - oam packet                                        */
/*           o len - oam length                                              */
/*           o port - received the oam source port                           */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process any oam packet                                                    */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 *frame_ptr;
    cs_uint32 len;
    cs_port_id_t port;
    
    cs_llid_t llid = oam_sdl_get_llid();

    frame_ptr = pPkt->data + pPkt->offset;
    len = pPkt->len;
    port = pPkt->port;

    oam_dump_pkt("Rcv OAM",port,frame_ptr,len);

    /* This is for reply the oam*/
    g_oam_port = port;
    
    /* if the frame was not came from pon port
     * it maybe oob message, all oob message must be 
     * follow the org oam format*/
    if(port == CS_PON_PORT_ID){
		if(llid == BROADCAST_LLID){
			oam_special_tx_laser_ctrl_set(frame_ptr);
		}
		else{
	        oam_ind_oami(llid, (oam_pdu_hdr_t *)frame_ptr, len);
		}
    }
    else {
        oam_orgspec_handle(frame_ptr, len);
    }

    return 1;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_time_msg_proc                                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_time_msg_proc()
/*                                                                           */
/* INPUTS  : N/A                                                             */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* oam keep alive timer process                                              */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_keep_alive();
}

cs_uint32 oam_special_tx_laser_ctrl_set(
	cs_uint8 * pRecv)
{
#ifdef HAVE_CTC_OAM 
    oam_pdu_ctc_t *pdu = (oam_pdu_ctc_t *)pRecv;
    oam_ctc_onu_laser_control_t * pInData = (oam_ctc_onu_laser_control_t *) pdu->data;

    oam_ctc_onu_laser_control_old_t *p21Data = (oam_ctc_onu_laser_control_old_t *)pdu->data;


    if((pdu->hdr.code == OAM_PDU_CODE_ORG_SPEC) 
            && oam_is_oui_equal(oam_oui_ctc, pdu->oui)
            &&(pdu->opcode == OAM_PDU_CTC_SET_REQ)
      ){
            if((pInData->hdr.branch == OAM_CTC_ATTRIB_BRANCH)
                    && (ntohs(pInData->hdr.leaf) == 0xa1)
                    ){
                /*CTC 2.1 oam format */
                if(pInData->hdr.width == 
                        (sizeof(oam_ctc_onu_laser_control_old_t) - sizeof(oam_var_cont_t))
                        ){
                    ctc_oam_onu_laser_control_set_adapt(ntohl(p21Data->action),p21Data->mac_address,ntohl(p21Data->transceiver_id));
                /* CTC 3.0 format */
                }else if(pInData->hdr.width == 
                        (sizeof(oam_ctc_onu_laser_control_t) - sizeof(oam_var_cont_t))
                        ){
                    ctc_oam_onu_laser_control_set_adapt(ntohs(pInData->action),pInData->mac_address,ntohl(pInData->transceiver_id));
                }
				else{ /*if OLT send OAM  pInData->hdr.width != 0x0C/0x0E,  just follow CTC2.1  */

						ctc_oam_onu_laser_control_set_adapt(ntohl(p21Data->action),p21Data->mac_address,ntohl(p21Data->transceiver_id));
					}
            }
    }
#endif

    return 1;
}

