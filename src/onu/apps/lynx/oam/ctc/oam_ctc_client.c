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
#ifdef HAVE_CTC_OAM

#include "sdl_ma.h"
#include "oam_common.h"
#include "oam_oob_mgmt.h"
#include "oam_supp.h"
#include "oam_core.h"
#include "oam_sdl_if.h"
#include "oam_control.h"
#include "oam_client.h"
#include "oam_mux_par.h"
#include "oam_ctc_client.h"
#include "oam_ctc_pdu.h"
#include "oam_plat_if.h"
#include "app_oam_ctc_intf.h"
#include "app_gpio.h"
#include "app_stats_ctc.h"
#include "app_alarm_ctc.h"
#include "img_upgrade.h"
#include "power_saving.h"

oam_oui_t  oam_oui_ctc = {0x11, 0x11, 0x11};
cs_boolean g_ctc_oam_enable = TRUE;

cs_uint8 g_ctc_ver_list[8] = {0};
cs_uint8 ver_flag[8] = {0}; 
cs_uint32  CTC_MAX_VERSION   = 0;

cs_uint8 CTC_MAX_ONU_UNI_PORTS = 4;

cs_boolean g_supp_cuc_oam = FALSE;

extern cs_boolean cgi_get_web_upgrade_status();
extern void oam_timer_restart();

void oam_ctc_get_oam_ext_status(cs_uint8 *status)
{
    oam_if_t *oamif = oam_intf_find(oam_sdl_get_llid());

    *status = OAM_CTC_EXT_ONU_SEND_WAIT;
    
    if(!oamif) {
        return ;
    }
    if(oamif->valid==OAM_TRUE)
        *status = oamif->ctc_OAM_Ext_Status;
    return;

}

static void oam_ctc_set_oam_ext_status(
        oam_if_t *intf,
        cs_uint8 status)
{
    if (intf->ctc_OAM_Ext_Status != status) {
        OAM_DEBUG("CTC: OAM_Ext_Status changes from %d to %d\n",
                intf->ctc_OAM_Ext_Status, status);
        intf->ctc_OAM_Ext_Status = status;

        if(status == OAM_CTC_EXT_ONU_COMPLETED)
        {
            oam_oob_send_ctc_event(OAM_OOB_EVENT_CTC_OAM_SUCCESS);
        }
        else if(status == OAM_CTC_EXT_ONU_NOTSUPPORTED 
                || status == OAM_CTC_EXT_ONU_SEND_INFO_NACK)
        {
            oam_oob_send_ctc_event(OAM_OOB_EVENT_CTC_OAM_FAILED);
        }
    }
}



void oam_ctc_proc_pdu_info(
        cs_port_id_t port,
        cs_uint8 * frame,
        cs_uint32 len)
{
    oam_pdu_info_t * pdu = (oam_pdu_info_t*) frame;
    oam_tlv_org_spec_t *ctc_tlv;
    oam_ctc_tlv_val_t *ctc_val;
    oam_ctc_tlv_ext_t *ctc_ext;
    oam_ctc_tlv_ext_t *ctc_ext_temp = NULL;
    cs_int32 count, i;
    cs_uint8 tlv_type;
    cs_uint8 tlv_len;
    cs_uint8 ext_len = 0;
    cs_uint8 ctc_oui_spported = 0;
    cs_uint8 *pos;
    oam_if_t *intf = NULL;

    OAM_ASSERT(frame != NULL);

    intf = oam_intf_find(port);
    if(intf == NULL){
        OAM_LOG("intf is NULL, llid id = %d \n",port);
        return;
    }

    if(!g_ctc_oam_enable){
        return;
    }

    pos = (cs_uint8 *)&pdu->local_info;
    tlv_type = *pos;
    tlv_len = *(pos + 1);

    while (tlv_type != 0) {

        if ((tlv_type != OAM_INFO_TLV_ORG_SPEC) 
                ||(!oam_is_oui_equal(pos + 2, oam_oui_ctc))) {
            pos += tlv_len;
            tlv_type = *pos;
            tlv_len = *(pos + 1);
            continue;
        }

        ctc_tlv = (oam_tlv_org_spec_t *)pos;
        ctc_val = (oam_ctc_tlv_val_t *)(ctc_tlv + 1);
        ctc_ext = (oam_ctc_tlv_ext_t *)(ctc_val + 1);
        count = ctc_tlv->length - sizeof(oam_tlv_org_spec_t) -
            sizeof(oam_ctc_tlv_val_t);


        if (ctc_tlv->length == (sizeof(oam_tlv_org_spec_t) +
                    sizeof(oam_ctc_tlv_val_t))) {

            if (intf->ctc_info_tlv.valid) {

                if(ctc_val->ext_support) {
                    if (intf->ctc_OAM_Ext_Status == OAM_CTC_EXT_ONU_SEND_WAIT ||
                            intf->ctc_OAM_Ext_Status == OAM_CTC_EXT_ONU_COMPLETED) {
                        oam_ctc_set_oam_ext_status(intf, OAM_CTC_EXT_ONU_SEND_INFO_RCV);
                    }
                    else if (intf->ctc_OAM_Ext_Status == OAM_CTC_EXT_ONU_SEND_INFO_SNT) {
                        oam_ctc_set_oam_ext_status(intf, OAM_CTC_EXT_ONU_SEND_INFO_ACK_RCV);
                    }
                }
                else {
                    oam_ctc_set_oam_ext_status(intf, OAM_CTC_EXT_ONU_NOTSUPPORTED);
                    OAM_DEBUG("%d - STATE %d ONU CTC: ext NOT supported\n",
                            __LINE__, intf->ctc_OAM_Ext_Status);
                }
                intf->ctc_info_tlv.ext_support = ctc_val->ext_support;
                intf->ctc_info_tlv.version = ctc_val->version;
                intf->ctc_OAM_Ext_version = ctc_val->version;
                intf->ctc_info_tlv.num_ext = 0;
            }
            else {
                OAM_DEBUG("%d - STATE %d ONU CTC: unexpected TLV format\n",
                        __LINE__, intf->ctc_OAM_Ext_Status);
            }
        }
        else {

            intf->ctc_info_tlv.valid = OAM_TRUE;
            intf->ctc_info_tlv.version = 0;
            intf->ctc_info_tlv.num_ext = CTC_MAX_VERSION;

            for(i=0;i<CTC_MAX_VERSION;i++)
            {
                memcpy(intf->ctc_info_tlv.ext[i].oui, oam_oui_ctc,
                        OAM_OUI_LEN);
                intf->ctc_info_tlv.ext[i].ver = g_ctc_ver_list[CTC_MAX_VERSION-1-i];
            }

            ctc_oui_spported = FALSE;
            ctc_ext_temp = ctc_ext;
            ext_len = 0;
            while (ext_len < count) {

                OAM_DEBUG("%d - CTC OUI 0x%x 0x%x 0x%x version %d\n",
                        __LINE__, ctc_ext_temp->oui[0], ctc_ext_temp->oui[1], ctc_ext_temp->oui[2], ctc_ext_temp->ver);

                /* CTC 2.0 */
                if (!memcmp(ctc_ext_temp->oui, oam_oui_ctc, OAM_OUI_LEN)) {
                    for(i=0; i<CTC_MAX_VERSION; i++) {
                        if(ctc_ext_temp->ver == g_ctc_ver_list[i]) {
                            ver_flag[i] = 1;
                            ctc_oui_spported = TRUE;
                        }
                    }
                }
                ctc_ext_temp += 1;
                ext_len += sizeof(oam_ctc_tlv_ext_t);
            }

            if (ctc_oui_spported) {
                OAM_DEBUG("%d - STATE %d ONU CTC: extension matched ver %d\n",
                        __LINE__, intf->ctc_OAM_Ext_Status, ctc_ext_temp->ver);
                intf->ctc_info_tlv.valid = OAM_TRUE;
                intf->ctc_info_tlv.ext_support = 1;
                /* CTC 2.0 */
                //intf->ctc_info_tlv.num_ext = 1;
                //intf->ctc_info_tlv.version = ctc_ext_temp->ver;
                memcpy(intf->ctc_OAM_Ext_OUI, oam_oui_ctc, OAM_OUI_LEN);
                //intf->ctc_OAM_Ext_version = ctc_ext_temp->ver;
                for(i=0; i<CTC_MAX_VERSION; i++) {
                    if(ver_flag[i]) {
                        intf->ctc_info_tlv.version = g_ctc_ver_list[i];
                        intf->ctc_OAM_Ext_version = g_ctc_ver_list[i];
                        break;
                    }
                }
                memset(ver_flag, 0, sizeof(cs_uint8)*CTC_MAX_VERSION);

                if (intf->ctc_OAM_Ext_Status == OAM_CTC_EXT_ONU_SEND_WAIT ||
                        (intf->ctc_OAM_Ext_Status == OAM_CTC_EXT_ONU_COMPLETED))
                    oam_ctc_set_oam_ext_status(intf, OAM_CTC_EXT_ONU_SEND_INFO_RCV);
                else if(intf->ctc_OAM_Ext_Status == OAM_CTC_EXT_ONU_SEND_INFO_SNT)
                    oam_ctc_set_oam_ext_status(intf, OAM_CTC_EXT_ONU_SEND_INFO_ACK_RCV);

                OAM_DEBUG("%d - STATE %d ONU CTC: state changed\n",
                        __LINE__, intf->ctc_OAM_Ext_Status);
            }
            else {
                OAM_LOG("%d - STATE %d ONU CTC: extension not matched\n",
                        __LINE__, intf->ctc_OAM_Ext_Status);
                intf->ctc_info_tlv.ext_support = 0;
                oam_ctc_set_oam_ext_status(intf, OAM_CTC_EXT_ONU_SEND_INFO_NACK);
            }
        }

 
        oam_discovery_send(intf, (oam_build_oampdu_t)oam_build_pdu_info, NULL);

        return;
        break;
    }
    
    //oam_discovery_send(intf, (oam_build_oampdu_t)oam_build_pdu_info, NULL);

}

cs_uint32 oam_ctc_build_pdu_info(
        cs_port_id_t port,
        cs_uint8 * frame,
        cs_uint32 length)
{
    cs_uint8 *pos = NULL;
    cs_int32 len = 0;
    oam_tlv_org_spec_t *ctc_tlv;
    oam_ctc_tlv_val_t *ctc_val;
    oam_ctc_tlv_ext_t *ctc_ext;
    oam_if_t *intf = NULL;

    OAM_ASSERT_RET(frame != NULL,-1);

    intf = oam_intf_find(port);
    if(intf == NULL){
        OAM_LOG("intf is NULL, llid id = %d \n",port);
        return -1;
    }

    pos = frame;

    if(!g_ctc_oam_enable
            || !intf->ctc_info_tlv.valid 
            ||(intf->ctc_OAM_Ext_Status != OAM_CTC_EXT_ONU_SEND_INFO_RCV 
                && intf->ctc_OAM_Ext_Status != OAM_CTC_EXT_ONU_SEND_INFO_ACK_RCV
                && intf->ctc_OAM_Ext_Status != OAM_CTC_EXT_ONU_SEND_INFO_NACK)
      ){
        return len;
    }

    ctc_tlv = (oam_tlv_org_spec_t *)pos;
    pos += sizeof(oam_tlv_org_spec_t);
    len += sizeof(oam_tlv_org_spec_t);
    ctc_tlv->type = OAM_INFO_TLV_ORG_SPEC;
    ctc_tlv->length = 7 + intf->ctc_info_tlv.num_ext * 4;
    memcpy(ctc_tlv->oui, oam_oui_ctc, OAM_OUI_LEN);

    ctc_val = (oam_ctc_tlv_val_t *)pos;
    ctc_val->ext_support = intf->ctc_info_tlv.ext_support;
    pos += sizeof(oam_ctc_tlv_val_t);
    len += sizeof(oam_ctc_tlv_val_t);


    if(intf->ctc_OAM_Ext_Status == OAM_CTC_EXT_ONU_SEND_INFO_RCV 
            || intf->ctc_OAM_Ext_Status == OAM_CTC_EXT_ONU_SEND_INFO_ACK_RCV){
        cs_int32 i;
        for (i=0;i<intf->ctc_info_tlv.num_ext;i++) {
            ctc_ext = (oam_ctc_tlv_ext_t *)pos;
            memcpy(ctc_ext->oui,
                    intf->ctc_info_tlv.ext[i].oui,
                    OAM_OUI_LEN);
            ctc_ext->ver = intf->ctc_info_tlv.ext[i].ver;
            pos += sizeof(oam_ctc_tlv_ext_t);
            len += sizeof(oam_ctc_tlv_ext_t);
        }

        if(intf->ctc_OAM_Ext_Status == OAM_CTC_EXT_ONU_SEND_INFO_RCV){
            ctc_val->version = 0;
            OAM_DEBUG("%d - STATE %d ONU CTC: extension OAM sent\n",
                    __LINE__, intf->ctc_OAM_Ext_Status);

            oam_ctc_set_oam_ext_status(intf,OAM_CTC_EXT_ONU_SEND_INFO_SNT);

        }else{
            ctc_val->version = intf->ctc_info_tlv.version;
            OAM_DEBUG("%d - STATE %d ONU CTC: extension OAM ACK sent\n",
                    __LINE__, intf->ctc_OAM_Ext_Status);
            oam_ctc_set_oam_ext_status(intf,OAM_CTC_EXT_ONU_COMPLETED);
            cs_led_proc(LED_PON, LED_GREEN,LED_ON);
            oam_timer_restart();
            
        }
    }else if(intf->ctc_OAM_Ext_Status == OAM_CTC_EXT_ONU_SEND_INFO_NACK){
        ctc_val->version = 0;
        intf->ctc_info_tlv.ext_support = 0;

        OAM_DEBUG("%d - STATE %d ONU CTC: extension OAM NACK sent\n",
                __LINE__, intf->ctc_OAM_Ext_Status);
        intf->ctc_OAM_Ext_Status = OAM_CTC_EXT_ONU_NOTSUPPORTED;
    }


    return len;
}

#if 0


cs_uint32 oam_ctc_set_mib_variable(
        oam_if_t *intf, 
        cs_uint8 branch,
        cs_uint16 leaf,
        oam_var_cont_t *pCont,
        cs_int32 value,
        cs_uint8 portId,
        cs_boolean incomingPortIndex,
        cs_boolean *outgoingPortIndex)
{
    cs_uint32 size = 0;
    unsigned long var = 0x10; /*test*/
    cs_uint8 *pData = (cs_uint8 *)pCont;
    oam_var_cont_val_t *pVar = NULL;

    if(!branch || !pCont || !intf)
        return 0;

    switch(branch) {
        case OAM_INTF_COUNTERS_MIB_BRANCH:
            switch(leaf) {
                case OAM_INTF_LEAF_ETH_ADM_STATE:
                    /*lynxd clean  size = oam_build_std_get_port_base_func(intf,(cs_uint8 *)pCont, portId, branch,
                      leaf, incomingPortIndex, outgoingPortIndex, (std_eth_func_t)onu_aal_get_phy_admin_state);
                      */
                    break;

                case OAM_INTF_LEAF_ETH_AUTONEG_ADM_STATE:
                    /* lynxd clean
                       size = oam_build_std_get_port_base_func(intf,(cs_uint8 *)pCont, portId, branch,
                       leaf, incomingPortIndex, outgoingPortIndex, (std_eth_func_t)onu_aal_get_auto_neg_admin_state);
                       */
                    break;
                case OAM_INTF_LEAF_ETH_AUTONEG_LOCAL_CAP:
                case OAM_INTF_LEAF_ETH_AUTONEG_ADVTS_CAP:
                    /* lynxd clean
                       size = oam_build_std_get_port_base_func(intf,(cs_uint8 *)pCont, portId, branch,
                       leaf, incomingPortIndex, outgoingPortIndex, NULL);
                       */
                    break;

                case OAM_INTF_LEAF_ETH_FEC_ABILITY:
                    pCont->width = sizeof(unsigned long);
                    pVar = (oam_var_cont_val_t *)pCont;
                    pVar->value1 = htonl(OAM_INTF_FEC_SUPPORDTED);
                    size = sizeof(oam_var_cont_t) + sizeof(unsigned long);
                    //var = onu_aal_get_port_fec_ability(intf->port);
                    break;

                case OAM_INTF_LEAF_ETH_FEC_MODE:
                    //diag_printf("fec mode set len = %d, value = %d\n", pCont->width, value);
                    if (pCont->width == 4) {
                        if (value != OAM_INTF_FEC_MODE_UNKOWN &&
                                value != OAM_INTF_FEC_MODE_EN &&
                                value != OAM_INTF_FEC_MODE_DIS)
                            var = -1;
                        else
                            //lynxd clean var = onu_aal_FEC_Mode_set(value);

                            if (CS_OK == var) {
                                pCont->width = OAM_CTC_VAR_SET_OK;
                            }
                            else {
                                pCont->width = OAM_CTC_VAR_SET_BAD_PARAM;
                            }
                        size = sizeof(oam_var_cont_t);
                    }
                    else if (pCont->width == 0) {
                        pCont->width = sizeof(unsigned long);
                        pVar = (oam_var_cont_val_t *)pCont;
                        //lynxd clean pVar->value1 = htonl(onu_aal_get_port_fec_mode(intf->port));
                        size = sizeof(oam_var_cont_t) + sizeof(unsigned long);
                    }
                    break;

                default:
                    pCont->width = OAM_CTC_ATTRIB_NOT_SUPPORTED;
                    size = sizeof(oam_var_cont_t);
                    break;
            }
            break;

        case OAM_INTF_CONTROL_MIB_BRANCH:
            switch(leaf) {
                case OAM_INTF_LEAF_ETH_FEC_MODE:
                    if (value != OAM_INTF_FEC_MODE_UNKOWN &&
                            value != OAM_INTF_FEC_MODE_EN &&
                            value != OAM_INTF_FEC_MODE_DIS)
                        var = -1;
                    else
                        //lynxd clean var = onu_aal_FEC_Mode_set(value);
                        size = sizeof(oam_var_cont_t);
                    if (CS_OK == var) {
                        pCont->width = OAM_CTC_VAR_SET_OK;
                    }
                    else {
                        pCont->width = OAM_CTC_VAR_SET_BAD_PARAM;
                    }
                    break;

                case OAM_INTF_LEAF_ETH_ADM_STATE_SET:
                    if(value != OAM_INTF_PHY_ADMIN_SET_DIS &&
                            value != OAM_INTF_PHY_ADMIN_SET_EN)
                        var = -1;
                    else {
                        /* lynxd clean
                           size = oam_build_std_set_port_base_func(intf,(cs_uint8 *)pCont, portId, branch,
                           leaf, value, incomingPortIndex, outgoingPortIndex,
                           (std_eth_set_func_t)onu_aal_phy_admin_state_set);
                           */
                    }
                    break;

                case OAM_INTF_LEAF_ETH_AUTO_NEG_RESTART:
                    /*lynxd clean
                      size = oam_build_std_set_port_base_func(intf,(cs_uint8 *)pCont, portId, branch,
                      leaf, value, incomingPortIndex, outgoingPortIndex,
                      (std_eth_set_func_t)onu_aal_auto_neg_reset);
                      */
                    break;

                case OAM_INTF_LEAF_ETH_AUTO_NEG_CONTRL:
                    if (value != OAM_INTF_AUTO_NEG_ADMIN_ST_EN &&
                            value != OAM_INTF_AUTO_NEG_ADMIN_ST_DIS)
                        var = -1;
                    else
                        /*lynxd clean
                          size = oam_build_std_set_port_base_func(intf,(cs_uint8 *)pCont, portId, branch,
                          leaf, value, incomingPortIndex, outgoingPortIndex,
                          (std_eth_set_func_t)onu_aal_auto_neg_set);
                          */
                        break;

                default:
                    var = -1;
                    size = sizeof(oam_var_cont_t);
                    pCont->width = OAM_CTC_VAR_SET_BAD_PARAM;
                    break;
            }
            break;

        default:
            size = sizeof(oam_var_cont_t) + sizeof(var);
            OAM_DEBUG("unknown branch 0x%x leaf 0x%x len %d\n",
                    branch, leaf, size);
            pData = (cs_uint8 *)pCont;
            pData += sizeof(oam_var_cont_t);
            memset(pData, 0, sizeof(var));
            size = sizeof(oam_var_cont_t) + sizeof(var);
            break;
    }

    return size;
}



static cs_uint32 oam_ctc_build_pdu_var_rsp(
        oam_if_t *intf, 
        oam_parm_oampdu_var_rsp_t *parm,
        oam_pdu_var_rsp_t *pdu)
{
    cs_int32 totalSize = 0;
    cs_int32 size = 0;
    cs_uint8 *pData = (cs_uint8 *)&pdu->container[0];
    oam_parm_oampdu_var_rsp_t *pParm;
    oam_var_cont_t *pCont = NULL;
    cs_uint8 portId = 0;
    cs_boolean incomingPortIndex = false, outgoingPortIndex = false;

    oam_build_pdu_hdr(intf, (oam_pdu_hdr_t *)pdu, OAM_PDU_CODE_VAR_RSP);
    pParm = (oam_parm_oampdu_var_rsp_t *)parm;
    pCont = (oam_var_cont_t *)pData;
    totalSize = sizeof(oam_pdu_hdr_t);
    while(pParm->branch)
    {
        unsigned long tmpl = 0;
        cs_uint16 tmps = 0;
        memset(pCont, 0, sizeof(oam_var_cont_t));
        pCont->branch = pParm->branch;
        pCont->leaf = pParm->leaf;
        pCont->width = pParm->width;

        if (OAM_CTC_INDEX_BRANCH == pParm->branch
                ||OAM_CTC_INDEX_BRANCH_E == pParm->branch) {
            if (OAM_CTC_LEAF_CODE_ONU_PORT == ntohs(pParm->leaf)) {
                portId = pParm->value[0];
                incomingPortIndex = true;
                outgoingPortIndex = false;
            }
            else {
                //CTCOAMTRC("CTC extended standard OAM port instance index leaf error %d\n",
                //    ntohs(pParm->leaf));
                return 0;
            }
            pParm++;
            continue;
        }

        if(pParm->width == sizeof(unsigned long))
        {
            memcpy(&tmpl, pParm->value, pParm->width);
            tmpl = ntohl(tmpl);
            size = oam_ctc_set_mib_variable(intf, pParm->branch, ntohs(pParm->leaf), pCont, tmpl,
                    portId, incomingPortIndex, &outgoingPortIndex);
        }
        else if(pParm->width == sizeof(cs_uint16))
        {
            memcpy(&tmps, pParm->value, pParm->width);
            tmps = ntohs(tmps);
            size = oam_ctc_set_mib_variable(intf, pParm->branch, ntohs(pParm->leaf), pCont, tmps,
                    portId, incomingPortIndex, &outgoingPortIndex);
        }
        else {
            size = oam_ctc_set_mib_variable(intf, pParm->branch, ntohs(pParm->leaf), pCont, pParm->value[0],
                    portId, incomingPortIndex, &outgoingPortIndex);
        }

        totalSize += size;
        pData += size;
        pCont = (oam_var_cont_t *)pData;
        pParm++;
    }

    //pack two 0 bytes to mark the end
    *pData++ = 0;
    *pData = 0;
    totalSize += 2;

    intf->var_rsp_tx++;
    return totalSize;
}


void oam_ctc_proc_pdu_var_req(
        cs_port_id_t port,
        cs_uint8 * frame,
        cs_uint32 len)
{
    cs_int32 num, max;
    oam_var_desc_t *curr, *next;
    oam_parm_oampdu_var_rsp_t parm[OAM_MAX_VAR_REQ_ENTRIES];
    cs_int32 count = 0;
    oam_var_cont_t *curr_container = NULL;
    cs_uint8 *pSetData = NULL;
    oam_if_t *intf = NULL;
    oam_pdu_var_req_t* pdu = (oam_pdu_var_req_t*) frame;

    OAM_ASSERT(frame != NULL);

    intf = oam_intf_find(port);
    if(intf == NULL){
        OAM_LOG("intf is NULL, llid id = %d \n",port);
        return ;
    }

    intf->var_req_rx++;

    num = 0;
    curr = &pdu->descriptor[0];
    max = len - sizeof(oam_pdu_hdr_t);
    while (curr) {
        oam_proc_var_desc(curr, &max, &next);
        if (curr->branch != 0) {
            OAM_DEBUG("rcv var req branch 0x%x leaf 0x%x\n", curr->branch, ntohs(curr->leaf));
            parm[count].branch = curr->branch;
            parm[count].leaf = curr->leaf;
            parm[count].width = 0;

            if (curr->branch == OAM_CTC_INDEX_BRANCH
                    ||curr->branch == OAM_CTC_INDEX_BRANCH_E) {
                curr_container = (oam_var_cont_t *)curr;
                parm[count].width = curr_container->width;
                pSetData = (cs_uint8 *)curr_container;
                pSetData += sizeof(oam_var_cont_t);
                if(parm[count].width < OAM_VAR_REQ_LEN) {
                    memcpy(parm[count].value, pSetData, parm[count].width);
                }
            }
            else if (curr->branch == OAM_INTF_COUNTERS_MIB_BRANCH) {
                if (curr->leaf == htons(OAM_INTF_LEAF_ETH_FEC_MODE)) {
                    curr_container = (oam_var_cont_t *)curr;
                    parm[count].width = curr_container->width;
                    if (parm[count].width == 4) {
                        pSetData = (cs_uint8 *)curr_container;
                        pSetData += sizeof(oam_var_cont_t);
                        memcpy(parm[count].value, pSetData, parm[count].width);
                    }
                }
            }

            if (curr->branch == OAM_INTF_CONTROL_MIB_BRANCH) {
                curr_container = (oam_var_cont_t *)curr;
                parm[count].width = curr_container->width;
                pSetData = (cs_uint8 *)curr_container;
                pSetData += sizeof(oam_var_cont_t);
                if((parm[count].width < OAM_VAR_REQ_LEN) && (parm[count].width > 0)) {
                    memcpy(parm[count].value, pSetData, parm[count].width);
                }
            }

            count++;
            if(count >= OAM_MAX_VAR_REQ_ENTRIES)
                break;
        }
        curr = next;
    }

    oam_req_oampdu(intf, (oam_build_oampdu_t)oam_ctc_build_pdu_var_rsp, &parm);
}
#endif

void oam_send_dying_gasp(cs_llid_t llid)
{
    oam_pdu_info_t pdu;
    cs_uint32 len = 0;
    cs_uint8 num = OAM_DYING_GASP_NUM, i;
    cs_callback_context_t     context;

    /*TODO: if need support mLlid, we need change the llid */
    oam_if_t *oamif = oam_intf_find(oam_sdl_get_llid());

    if(oamif == NULL)
        return;

    oamif->local_dying_gasp = TRUE;
    oamif->local_critical_event = FALSE;

    len = oam_build_pdu_info(oamif, NULL, &pdu);

    num = oam_plat_get_dying_gasp_num();

    OAM_DEBUG("Send Dying Gasp %d times..\n", num);

    for(i=0; i<num; i++){
        epon_request_onu_frame_send_to_pon_no_mutex(
                context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, (cs_uint8*)&pdu, len,  ONU_CPU_PKT_PRIO);
    }

    oamif->local_dying_gasp = FALSE;
    oamif->local_critical_event = EPON_TRUE;
}

cs_boolean oam_is_cuc_oam()
{
    oam_if_t *oamif = oam_intf_find(oam_sdl_get_llid());
    
    if(oamif == NULL){
        return FALSE;
    }

    if(oamif->ctc_OAM_Ext_version == 0xc1){
        return TRUE;
    }
    return FALSE;
}

cs_boolean oam_ctc_version_is_21(void)
{
	oam_if_t *oamif = oam_intf_find(oam_sdl_get_llid());

	if(oamif == NULL){
		return FALSE;
	}

	if(oamif->ctc_OAM_Ext_version == 0x21){
		return TRUE;
	}
	
	return FALSE;
}

cs_boolean oam_ctc_version_is_30(void)
{
	oam_if_t *oamif = oam_intf_find(oam_sdl_get_llid());

	if(oamif == NULL){
		return FALSE;
	}

	if(oamif->ctc_OAM_Ext_version == 0x30){
		return TRUE;
	}

	return FALSE;
}

cs_status  oam_ctc_version_get(cs_llid_t llid, cs_uint8 *version)
{
    oam_if_t *oamif = oam_intf_find(llid);
    
    if(!version || !oamif)
    {
        return CS_E_PARAM;
    }

    *version = oamif->ctc_OAM_Ext_version;
    return CS_E_OK;
}

void oam_ctc_llid_registration(cs_llid_t llid)
{
}
void oam_ctc_llid_deregistration(cs_llid_t llid)
{
    cs_led_proc(LED_PON, LED_GREEN,LED_OFF);

    if(!cgi_get_web_upgrade_status())
    {
        img_upgrade_clean();
    }
    ctc_oam_llid_deregister_handle();
    pwrs_event(PWRS_MSG_RESET, NULL, 0);
}
void oam_org_init()
{
	#if 0
	cs_uint8 ctc_ver_list[] = {0xc1,0x30,0x21,0x20,0x13,0x01};
	#else
	//modified by zhuxh,0xc1 is for Unicom
	cs_uint8 ctc_ver_list[] = {0x30,0x21,0x20,0x13,0x01};
	#endif

    oam_vendor_handlers_t ctc_handle;

    oam_vendor_handlers_init(&ctc_handle);
#if 0
    oam_std_handler_register(OAM_PDU_CODE_VAR_REQ,
            oam_ctc_proc_pdu_var_req);
#endif

    CTC_MAX_ONU_UNI_PORTS = oam_plat_get_switch_port_num();

    startup_config_read(
		CFG_ID_OUI, sizeof(oam_oui_ctc), oam_oui_ctc);
    
    memset(ver_flag,0x00,sizeof(ver_flag));
    memset(g_ctc_ver_list,0x00,sizeof(g_ctc_ver_list));

    memcpy(g_ctc_ver_list,ctc_ver_list,sizeof(ctc_ver_list));
    CTC_MAX_VERSION = sizeof(ctc_ver_list)/sizeof(ctc_ver_list[0]);

    ctc_handle.ext_info_proc_handler = oam_ctc_proc_pdu_info;
    ctc_handle.ext_info_build_handler = oam_ctc_build_pdu_info;

    ctc_handle.org_handler = handle_ctc_oams;

    oam_vendor_handler_register(oam_oui_ctc,ctc_handle);

    ctc_oam_alarm_init();
    ctc_onu_stats_monitor_init();


    cs_printf("oam_org_init\r\n");

    pwrs_pkgc_init();
}
#endif /*END_HAVE_CTC_OAM*/

