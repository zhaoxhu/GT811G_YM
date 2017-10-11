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

DEFINITIONS:  "DEVICE" means the Cortina Systems?Daytona SDK product.
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

Copyright (c) 2011 by Cortina Systems Incorporated
****************************************************************************/
#include "oam_common.h"
#include "oam_core.h"
#include "oam_supp.h"
#include "oam_sdl_if.h"
#include "oam_plat_if.h"
#include "oam_std_pdu.h"
#include "oam_control.h"
#include "oam_oob_mgmt.h"

#include "oam_client.h"
#include "port_stats.h"

static cs_boolean oam_len_check = OAM_FALSE;
/*
 * slow protocol address, refer 802.3ah
 */
static cs_uint8 oam_slow_protocols_multicast[OAM_MACADDR_LEN] 
                    = {0x01, 0x80, 0xc2, 0x00, 0x00, 0x02};

/*
 * Cortina OUI, the 3 bytes of cortina Mac address 
 */
static cs_uint8 oam_oui_imst[OAM_OUI_LEN] = {0x00, 0x13, 0x25};

/*
 * Passive OUI
 */
static cs_uint8 oam_oui_pas[OAM_OUI_LEN] = {0x00, 0x0c, 0xd5};


/*****************************************************************************/
/* $rtn_hdr_start  oam_macaddr_higher                                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static cs_boolean oam_macaddr_higher(
        cs_uint8 *mac1, 
        cs_uint8 *mac2)
/*                                                                           */
/* INPUTS  : o mac1 - mac to compare                                         */
/*           o mac2 - mac to compare                                         */
/* OUTPUTS : TRUE - when mac1 > mac2                                         */
/*           FALSE - when mac1 <= mac2                                       */
/* DESCRIPTION:                                                              */
/* Compare the two mac address                                               */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 i;

    OAM_ASSERT_RET(mac1 != NULL 
            && mac2 != NULL,OAM_FALSE);

    for (i=0;i<OAM_MACADDR_LEN;i++) {
        if (mac1[i] > mac2[i]) {
            return OAM_TRUE;
        }
    }
    return OAM_FALSE;
}


/*****************************************************************************/
/* $rtn_hdr_start  oam_proc_tlv                                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static cs_int32 oam_proc_tlv(
        cs_uint8 *curr, 
        cs_int32 *max, 
        cs_uint8 **next)
/*                                                                           */
/* INPUTS  : o curr - current tlv address                                    */
/*           o max - max len                                                 */
/*           o next - next tlv address                                       */
/* OUTPUTS : 0 - find the next tlv                                           */
/*           -1 - find the next tlv failed                                   */
/* DESCRIPTION:                                                              */
/* process the TLV struct like below:                                        */
/*  {                                                                        */
/*      cs_uint8 type;                                                       */
/*      cs_uint8 length;                                                     */
/*      cs_uint8 data[];                                                     */
/*  }                                                                        */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 len;

    if (curr[0] == 0) {
        *next = NULL;
        return 0;
    }

    if (*max < 2) {
        return -1;
    }

    len = curr[1];
    if ((len < 2) || (len > *max)) {
        return -1;
    }

    *next = (len == *max) ? NULL : (curr + len);
    *max -= len;

    return 0;
}


/*****************************************************************************/
/* $rtn_hdr_start  oam_proc_var_desc                                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_int32 oam_proc_var_desc(
        oam_var_desc_t *curr, 
        cs_int32 *max, 
        oam_var_desc_t **next)
/*                                                                           */
/* INPUTS  : o curr - current tlv address                                    */
/*           o max - max len                                                 */
/*           o next - next tlv address                                       */
/* OUTPUTS : 0 - find the next tlv                                           */
/*           -1 - find the next tlv failed                                   */
/* DESCRIPTION:                                                              */
/* process the TLV struct like below:                                        */
/*  {                                                                        */
/*      cs_uint8 type;                                                       */
/*      cs_int16 leaf;                                                 */
/*      cs_uint8 length;                                                     */
/*      cs_uint8 data[];                                                     */
/*  }                                                                        */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 len;
    oam_var_cont_t *curr_container = NULL;

    if (curr->branch == 0) {
        *next = NULL;
        return 0;
    }

    len = sizeof(oam_var_desc_t);

    if (len > *max) {
        *next = NULL;
        return -1;
    }

    curr_container = (oam_var_cont_t *)curr;
    /* if var set, include data len */
    if (curr->branch == OAM_INTF_CONTROL_MIB_BRANCH) {
        /* Auto NEG Restart ONLY has branch and leaf, no width*/
        if (curr_container->leaf != htons(OAM_INTF_LEAF_ETH_AUTO_NEG_RESTART)) {
            len = curr_container->width + sizeof(oam_var_cont_t);
        }
    }
    else if (curr->branch == OAM_INTF_COUNTERS_MIB_BRANCH) {
        if (curr_container->leaf == htons(OAM_INTF_LEAF_ETH_FEC_MODE)) {
            /* aFECmode has set and get, the set has width and get has not*/
            if (4 == curr_container->width) {
                len = curr_container->width + sizeof(oam_var_cont_t);
            }
        }
    }
    /* any other branch, we assume there has width*/
    else {
        len = curr_container->width + sizeof(oam_var_cont_t);
    }

    *next = (oam_var_desc_t *)((len == *max) ? NULL :
            ((cs_uint8 *)curr + len));
    *max -= len;

    return 0;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_proc_var_cont                                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static cs_int32 oam_proc_var_cont(
        oam_var_cont_t *curr, 
        cs_int32 *max, 
        oam_var_cont_t **next)
/*                                                                           */
/* INPUTS  : o curr - current tlv address                                    */
/*           o max - max len                                                 */
/*           o next - next tlv address                                       */
/* OUTPUTS : 0 - find the next tlv                                           */
/*           -1 - find the next tlv failed                                   */
/* DESCRIPTION:                                                              */
/* process the TLV struct like below:                                        */
/*  {                                                                        */
/*      cs_uint8 branch;                                                     */
/*      cs_uint16 leaf;                                                      */
/*      cs_uint8 width;                                                      */
/*  }                                                                        */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 len;

    if (curr->branch == 0) {
        *next = NULL;
        return 0;
    }

    if (*max < 4) {
        return -1;
    }

    len = (curr->width & OAM_VAR_WIDTH_IND_PRESENT) ? 4 : curr->width;

    if (len == 0) {
        len = 128;
    }
    len += 4;
    if (len > *max) {
        return -1;
    }
    *next = (oam_var_cont_t *)((len == *max) ? NULL :
            ((cs_uint8 *)curr + len));
    *max -= len;

    return 0;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_d2_proc_var_desc                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static cs_int32 oam_d2_proc_var_desc(
        oam_d2_var_desc_t *curr, 
        cs_int32 *max, 
        oam_d2_var_desc_t **next)
/*                                                                           */
/* INPUTS  : o curr - current tlv address                                    */
/*           o max - max len                                                 */
/*           o next - next tlv address                                       */
/* OUTPUTS : 0 - find the next tlv                                           */
/*           -1 - find the next tlv failed                                   */
/* DESCRIPTION:                                                              */
/* process the TLV struct like below:                                        */
/*  {                                                                        */
/*      cs_uint8 branch;                                                     */
/*      cs_uint8 leaf;                                                       */
/*  }                                                                        */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint8 len;

    if (curr->branch == 0) {
        *next = NULL;
        return 0;
    }

    len = sizeof(oam_d2_var_desc_t);
    if (len > *max) {
        return -1;
    }
    *next = (oam_d2_var_desc_t *)((len == *max) ? NULL :
            ((cs_uint8 *)curr + len));
    *max -= len;

    return 0;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_d2_set_mib_variable                                   */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_uint32 oam_d2_set_mib_variable(
        oam_if_t *intf, 
        cs_uint8 branch, 
        cs_uint8 leaf, 
        oam_d2_var_cont_t *pD2Cont, 
        cs_int32 value)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o branch - branch of the mib                                    */
/*           o leaf - leaf of the mib                                        */
/*           o pD2Cont - the struct of the mib                               */
/*           o value - the value need set to sdl                             */
/* OUTPUTS : length of the whole mib                                         */
/* DESCRIPTION:                                                              */
/* According the mib information, we set/get the value to/from sdl           */
/* this is only the oam draft2                                               */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint32 size = 0;
    unsigned long var = 0;
    oam_pon_stats_t stats;

    if(!branch || !pD2Cont || !intf){
        return 0;
    }

    memset(&stats, 0, sizeof(stats));

    app_onu_pon_stats_get(&stats);
    

    switch(branch)
    {
        case OAM_PASSAVE_BRANCH:
            switch(leaf)
            {
                case OAM_PASSAVE_PON_BYT_TX:
                    MIBVARSET(stats.pon_tx_byte_cnt);
                    break;
                case OAM_PASSAVE_PON_BYT_RX:
                    MIBVARSET(stats.pon_byte_cnt);
                    break;
                case OAM_PASSAVE_REG_REQ_TX:
                    MIBVARSET(stats.pon_mpcp_tx_req_reg);
                    break;
                case OAM_PASSAVE_REG_ACK_TX:
                    MIBVARSET(stats.pon_mpcp_tx_ack_ack);
                    break;
                case OAM_PASSAVE_REG_RX:
                    MIBVARSET(stats.pon_mpcp_uc_reg_ack);
                    break;
                case OAM_PASSAVE_GATE_RX:
                    MIBVARSET(stats.pon_mpcp_uc_gat_norm+stats.pon_mpcp_uc_gat_frpt);
                    break;
                case OAM_PASSAVE_RPT_RX:
                    MIBVARSET(0);
                    break;
#if 0
                case OAM_PASSAVE_NOTSUP_RX:
                    MIBVARSET(epon_stats.rx_not_supported);
                    break;
#endif
                case OAM_PASSAVE_OAMINF_RX:
                    IFMIBVARSET(intf->info_rx);
                    break;
                case OAM_PASSAVE_OAMINF_TX:
                    IFMIBVARSET(intf->info_tx);
                    break;
                case OAM_PASSAVE_OAMEVT_RX:
                case OAM_PASSAVE_OAMUEVT_RX:
                    IFMIBVARSET(intf->unique_event_tx);
                    break;
                case OAM_PASSAVE_OAMDEVT_RX:
                    IFMIBVARSET(intf->dup_event_tx);
                    break;
                case OAM_PASSAVE_OAMLBC_TX:
                    IFMIBVARSET(intf->lb_control_tx);
                    break;
                case OAM_PASSAVE_OAMLBC_RX:
                    IFMIBVARSET(intf->lb_control_rx);
                    break;
                case OAM_PASSAVE_OAMVR_TX:
                    IFMIBVARSET(intf->var_req_tx);
                    break;
                case OAM_PASSAVE_OAMVR_RX:
                    IFMIBVARSET(intf->var_req_rx);
                    break;
                case OAM_PASSAVE_OAMVRSP_TX:
                    IFMIBVARSET(intf->var_rsp_tx);
                    break;
                case OAM_PASSAVE_OAMVRSP_RX:
                    IFMIBVARSET(intf->var_rsp_rx);
                    break;
                case OAM_PASSAVE_OAMOS_TX:
                    IFMIBVARSET(intf->org_specific_tx);
                    break;
                case OAM_PASSAVE_OAMOS_RX:
                    IFMIBVARSET(intf->org_specific_rx);
                    break;
                case OAM_PASSAVE_OAMLST:
                    IFMIBVARSET(intf->frame_lost);
                    break;
                case OAM_PASSAVE_PON_ERR:
                    MIBVARSET(stats.pon_crc8_err + stats.pon_err_symbol);
                    break;
                case OAM_PASSAVE_PON_REG_REQ_TX:
                    MIBVARSET(stats.pon_mpcp_tx_req_reg);
                    break;
                case OAM_PASSAVE_PON_REG_ACK_TX:
                    MIBVARSET(stats.pon_mpcp_tx_ack_ack);
                    break;
                case OAM_PASSAVE_PON_REG_RX:
                    MIBVARSET(stats.pon_mpcp_uc_reg_ack);
                    break;
                case OAM_PASSAVE_OAM_TX:
                    IFMIBVARSET(intf->info_tx + intf->unique_event_tx + intf->dup_event_tx +
                            intf->lb_control_tx + intf->var_req_tx + intf->var_rsp_tx + intf->org_specific_tx + intf->unsupported_tx);
                    break;
                case OAM_PASSAVE_OAM_RX:
                    IFMIBVARSET(intf->info_rx + intf->unique_event_rx + intf->dup_event_rx +
                            intf->lb_control_rx + intf->var_req_rx + intf->var_rsp_rx + intf->org_specific_rx + intf->unsupported_rx);
                    break;
                case OAM_PASSAVE_REPORT_TX:
                    MIBVARSET(stats.pon_mpcp_tx_req_reg);
                    break;
                default:
                    pD2Cont->width = OAM_MIB_VARIABLE_INDICATION_MASK | OAM_MIB_VARIABLE_NOT_SUPORT;
                    break;
            }
            if(pD2Cont->width & OAM_MIB_VARIABLE_INDICATION_MASK)
                size = sizeof(oam_d2_var_cont_t) - 1;
            else
                size = sizeof(oam_d2_var_cont_t) - 1 + pD2Cont->width;
            OAM_DEBUG( "branch 0x%x leaf 0x%x, len %d\n",
                    pD2Cont->branch, pD2Cont->leaf, size);
            break;
        case OAM_INTF_COUNTERS_MIB_BRANCH:
            switch(leaf)
            {
                case OAM_INTF_LEAF_PON_OCT_TX:
                    MIBVARSET(stats.pon_tx_byte_cnt);
                    break;
                case OAM_INTF_LEAF_PON_SING_COLL:
                    MIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_MULTI_COLL:
                    MIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_UNIC_RX:
                    MIBVARSET(stats.pon_frame_cnt);
                    break;
                case OAM_INTF_LEAF_PON_SEQ_ERR:
                    MIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_ALG_ERR:
                    MIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_UNIC_TX:
                    MIBVARSET(stats.pon_txframe_cnt);
                    break;
                case OAM_INTF_LEAF_PON_DEFR:
                    MIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_LATE_COLL:
                    MIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_EXC_COLL:
                    MIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_MAXTX_ERR:
                    MIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_SEN_ERR:
                    MIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_OCT_RX:
                    MIBVARSET(stats.pon_byte_cnt);
                    break;
                case OAM_INTF_LEAF_PON_RX_ERR:
                    MIBVARSET(stats.pon_crcerr_cnt+stats.pon_crc8_err);
                    break;
                case OAM_INTF_LEAF_PON_TOO_LNG:
                    MIBVARSET(stats.pon_oversize_cnt);
                    break;
#if 0
                case OAM_INTF_LEAF_ETH_ADM_STATE:
                    var = onu_aal_get_admin_state(intf->port);
                    IFMIBVARSET(var);
                    break;
                case OAM_INTF_LEAF_ETH_AUTONEG_ADM_STATE:
                    var = onu_aal_get_auto_neg_admin_state(intf->port);
                    IFMIBVARSET(var);
                    break;
                case OAM_INTF_LEAF_ETH_PORT_CAP:
                    var = onu_aal_get_port_cap(intf->port);
                    IFMIBVARSET(var);
                    break;
                case OAM_INTF_LEAF_ETH_AUTONEG_CAP:
                    var = onu_aal_get_port_auto_neg_cap(intf->port);
                    IFMIBVARSET(var);
                    break;
                case OAM_INTF_LEAF_ETH_FEC_MODE:
                    var = onu_aal_get_port_fec_mode(intf->port);
                    MIBVARSET(var);
                    break;
#endif
                default:
                    pD2Cont->width = OAM_MIB_VARIABLE_INDICATION_MASK | OAM_MIB_VARIABLE_NOT_SUPORT;
                    break;
            }
            if(pD2Cont->width & OAM_MIB_VARIABLE_INDICATION_MASK)
                size = sizeof(oam_d2_var_cont_t) - 1;
            else
                size = sizeof(oam_d2_var_cont_t) - 1 + pD2Cont->width;
            OAM_DEBUG( "branch 0x%x leaf 0x%x, len %d\n",
                    pD2Cont->branch, pD2Cont->leaf, size);
            break;
        case OAM_INTF_CONTROL_MIB_BRANCH:
            switch(leaf)
            {
                case OAM_INTF_LEAF_ETH_FEC_MODE:
                    /*onu_aal_FEC_Mode_set(value)*/
                    break;
                case OAM_INTF_LEAF_ETH_ADM_STATE_SET:
                    /*onu_aal_admin_state_set(value)*/
                    break;
                case OAM_INTF_LEAF_ETH_AUTO_NEG_RESTART:
                    /*onu_aal_auto_neg_reset(value)*/
                    break;
                case OAM_INTF_LEAF_ETH_AUTO_NEG_CONTRL:
                    /*onu_aal_auto_neg_set(value)*/
                    break;
                default:
                    break;
            }
            pD2Cont->width = OAM_MIB_VARIABLE_INDICATION_MASK | OAM_MIB_VARIABLE_NOT_SUPORT;
            size = sizeof(oam_d2_var_cont_t) - 1;
            break;
        default:
            /* unknow branch, assume data length is 2 bytes */
            pD2Cont->width = OAM_MIB_VARIABLE_INDICATION_MASK | OAM_MIB_VARIABLE_NOT_SUPORT;
            size = sizeof(oam_d2_var_cont_t) - 1;
            OAM_DEBUG( "branch 0x%x leaf 0x%x, len %d\n",
                    pD2Cont->branch, pD2Cont->leaf, size);
            break;
    }
    return size;
}


/*****************************************************************************/
/* $rtn_hdr_start  oam_set_mib_variable                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_uint32 oam_set_mib_variable(
        oam_if_t *intf, 
        cs_uint8 branch,
        cs_int16 leaf, 
        oam_var_cont_t *pCont, 
        cs_int32 value)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o branch - branch of the mib                                    */
/*           o leaf - leaf of the mib                                        */
/*           o pCont - the struct of the mib                                 */
/*           o value - the value need set to sdl                             */
/* OUTPUTS : length of the whole mib                                         */
/* DESCRIPTION:                                                              */
/* According the mib information, we set/get the value  to/from sdl          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_uint32 size = 0;
    unsigned long var = 0;
    oam_pon_stats_t stats;
    
    if(!branch || !pCont || !intf)
        return 0;

    memset(&stats, 0, sizeof(stats));
    app_onu_pon_stats_get(&stats);

    switch(branch)
    {
        case OAM_INTF_COUNTERS_MIB_BRANCH:
            switch(leaf)
            {
                case OAM_INTF_LEAF_PON_OCT_TX:
                    STDMIBVARSET(stats.pon_tx_byte_cnt);
                    break;
                case OAM_INTF_LEAF_PON_SING_COLL:
                    STDMIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_MULTI_COLL:
                    STDMIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_UNIC_RX:
                    STDMIBVARSET(stats.pon_frame_cnt);
                    break;
                case OAM_INTF_LEAF_PON_SEQ_ERR:
                    STDMIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_ALG_ERR:
                    STDMIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_UNIC_TX:
                    STDMIBVARSET(stats.pon_txframe_cnt);
                    break;
                case OAM_INTF_LEAF_PON_DEFR:
                    STDMIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_LATE_COLL:
                    STDMIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_EXC_COLL:
                    STDMIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_MAXTX_ERR:
                    STDMIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_SEN_ERR:
                    STDMIBVARSET(0);
                    break;
                case OAM_INTF_LEAF_PON_OCT_RX:
                    STDMIBVARSET(stats.pon_byte_cnt);
                    break;
                case OAM_INTF_LEAF_PON_RX_ERR:
                    STDMIBVARSET(stats.pon_crcerr_cnt+stats.pon_crc8_err+stats.pon_err_symbol+stats.pon_sld_err);
                    break;
                case OAM_INTF_LEAF_TX_MC_FRAME_OK:
                    STDMIBVARSET(stats.pon_txmcframe_cnt);
                    break;
                case OAM_INTF_LEAF_TX_BC_FRAME_OK:
                    STDMIBVARSET(stats.pon_txbcframe_cnt);
                    break;
                case OAM_INTF_LEAF_RX_MC_FRAME_OK:
                    STDMIBVARSET(stats.pon_mcframe_cnt);
                    break;
                case OAM_INTF_LEAF_RX_BC_FRAME_OK:
                    STDMIBVARSET(stats.pon_bcframe_cnt);
                    break;
                case OAM_INTF_LEAF_PON_TOO_LNG:
                    STDMIBVARSET(stats.pon_oversize_cnt);
                    break;
                default:
                    pCont->width = OAM_MIB_VARIABLE_INDICATION_MASK | OAM_MIB_VARIABLE_NOT_SUPORT;
                    break;
            }
            if(pCont->width & OAM_MIB_VARIABLE_INDICATION_MASK)
                size = sizeof(oam_var_cont_t);
            else
                size = sizeof(oam_var_cont_t)  + pCont->width;
            OAM_DEBUG( "branch 0x%x leaf 0x%x, len %d\n",
                    pCont->branch, pCont->leaf, size);
            break;
        case OAM_INTF_CONTROL_MIB_BRANCH:
            switch(leaf)
            {
                case OAM_INTF_LEAF_ETH_FEC_MODE:
                    /*onu_aal_FEC_Mode_set(value)*/
                    break;
                case OAM_INTF_LEAF_ETH_ADM_STATE_SET:
                    /*onu_aal_admin_state_set(value)*/
                    break;
                case OAM_INTF_LEAF_ETH_AUTO_NEG_RESTART:
                    /*onu_aal_auto_neg_reset(value)*/
                    break;
                case OAM_INTF_LEAF_ETH_AUTO_NEG_CONTRL:
                    /*onu_aal_auto_neg_set(value)*/
                    break;
                default:
                    break;
            }
            pCont->width = OAM_MIB_VARIABLE_INDICATION_MASK | OAM_MIB_VARIABLE_NOT_SUPORT;
            size = sizeof(oam_var_cont_t);
            break;
        default:
            /* unknow branch, assume data length is 2 bytes */
            pCont->width = OAM_MIB_VARIABLE_INDICATION_MASK | OAM_MIB_VARIABLE_NOT_SUPORT;
            size = sizeof(oam_var_cont_t) ;
            OAM_DEBUG( "branch 0x%x leaf 0x%x, len %d\n",
                    pCont->branch, pCont->leaf, size);
            break;
    }
    return size;

}

/*****************************************************************************/
/* $rtn_hdr_start  oam_proc_pdu_sanity                                       */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static cs_int32 oam_proc_pdu_sanity(
        oam_if_t *intf, 
        oam_pdu_hdr_t *pdu, 
        cs_int32 len)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o pdu - the oam packet                                          */
/*           o len - length of the packet                                    */
/* OUTPUTS : 0 - packet pass the check                                       */
/*           -1 - packet not pass the check                                  */
/* DESCRIPTION:                                                              */
/* check the oam packet is success or not                                    */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_int32 max;
    cs_uint8 *curr, *next;
    oam_var_desc_t *next_var_desc;
    oam_var_cont_t *next_var_cont;
    cs_int16 flags = ntohs(pdu->flags);

    if (memcmp(pdu->dst_mac, oam_slow_protocols_multicast,
                OAM_MACADDR_LEN) ||
            (pdu->eth_type != htons(OAM_SLOW_PROTOCOLS_TYPE)) ||
            (pdu->sub_type != OAM_SUBTYPE) ||
            (len < sizeof(oam_pdu_hdr_t))) {
        OAM_DEBUG("Invalid header\n");
        return -1;
    }

    max = len - sizeof(oam_pdu_hdr_t);
    if ((max == 0) && ((pdu->code != OAM_PDU_CODE_INFO) ||
                !(flags & OAM_FLAG_LINK_FAULT))) {
        OAM_DEBUG("PDU too short\n");
        return -1;
    }

    if (!intf->d2_compatible && (flags & OAM_FLAG_LOCAL_EVALUATING) &&
            (flags & OAM_FLAG_LOCAL_STABLE)) {
        OAM_DEBUG("Wrong flags\n");
        return -1;
    }

    curr = (cs_uint8 *)(pdu + 1);
    switch (pdu->code) {
        case OAM_PDU_CODE_INFO:
            if (!(flags & OAM_FLAG_LINK_FAULT)) {
                if (oam_proc_tlv(curr, &max, &next)) {
                    OAM_DEBUG("Invalid local tlv\n");
                    return -1;
                }
                if (curr[0] != OAM_INFO_TLV_LOCAL_INFO) {
                    OAM_DEBUG("Local info missing\n");
                    return -1;
                }
#if 0
                /* ??? please verify ??? */
                if (!(((oam_tlv_info_t *)curr)->oam_config &
                            OAM_INFO_CONFIG_OAM_MODE) &&
                        (intf->local_pdu != OAM_LOCAL_PDU_INFO) &&
                        (intf->local_pdu != OAM_LOCAL_PDU_ANY)) {
                    OAM_DEBUG(
                            "unexpected info oampdu\n");
                    return -1;
                }
#endif
                curr = next;

                if ((!intf->d2_compatible &&
                            !(flags & OAM_FLAG_LOCAL_EVALUATING)) ||
                        (intf->d2_compatible &&
                         (flags & OAM_2DOT0_FLAG_LOCAL_STABLE))) {
                    if (!curr) {
                        OAM_DEBUG("Remote info TLV expected\n");
                        return -1;
                    }
                    if (oam_proc_tlv(curr, &max, &next)) {
                        OAM_DEBUG("Invalid remote info TLV\n");
                        return -1;
                    }
                    if (curr[0] != OAM_INFO_TLV_REMOTE_INFO) {
                        OAM_DEBUG("Remote info TLV missing\n");
                        return -1;
                    }
                    curr = next;
                }

                while (curr) {
                    if (oam_proc_tlv(curr, &max, &next)) {
                        OAM_DEBUG("Invalid info TLV\n");
                        return -1;
                    }
                    if (curr[0] && (curr[0] != OAM_INFO_TLV_ORG_SPEC)) {
                        OAM_DEBUG("Unexpected TLV type\n");
                    }
                    curr = next;
                }
            }
            break;

        case OAM_PDU_CODE_EVENT:
            if (max < 3) {
                OAM_DEBUG("PDU too short\n");
                return -1;
            }
            if (((oam_pdu_event_t *)pdu)->seq_num == intf->seq_num_last_rx){
                OAM_DEBUG("Duplicated event\n");
                return -1;
            }
            curr += 2;
            max -= 2;
            while (curr) {
                if (oam_proc_tlv(curr, &max, &next)) {
                    OAM_DEBUG("Invalid TLV\n");
                    return -1;
                }
                if ((curr[0] != OAM_EVENT_TLV_ERR_SYM_PERIOD) &&
                        (curr[0] != OAM_EVENT_TLV_ERR_FRAME) &&
                        (curr[0] != OAM_EVENT_TLV_ERR_FRAME_PERIOD) &&
                        (curr[0] != OAM_EVENT_TLV_ERR_FRAME_SUMM) &&
                        curr[0]) {
                    OAM_DEBUG("Unknown TLV type\n");
                }
                curr = next;
            }
            break;

        case OAM_PDU_CODE_VAR_REQ:
            if (!(intf->remote_oam_config & OAM_INFO_CONFIG_OAM_MODE)) {
                OAM_DEBUG("Unexpected VarReq\n");
                return -1;
            }

            while (curr) {
                if (oam_proc_var_desc((oam_var_desc_t *)curr, &max, &next_var_desc)) {
                    OAM_DEBUG("Invalid variable descriptor\n");
                    return -1;
                }
                curr = (cs_uint8 *)next_var_desc;
            }
            break;

        case OAM_PDU_CODE_VAR_RSP:
            if (intf->local_oam_mode == OAM_MODE_PASSIVE) {
                OAM_DEBUG("Unexpected VarRsp\n");
                return -1;
            }

            while (curr) {
                if (oam_proc_var_cont((oam_var_cont_t *)curr, &max, &next_var_cont)) {
                    OAM_DEBUG("Invalid variable container\n");
                    return -1;
                }
                curr = (cs_uint8 *)next_var_cont;
            }
            break;

        case OAM_PDU_CODE_LOOPBACK:
            if (!(intf->remote_oam_config & OAM_INFO_CONFIG_OAM_MODE)) {
                OAM_DEBUG("Unexpected loopback control\n");
                return -1;
            }

            if ((curr[0] != OAM_LOOPBACK_COMMAND_ENABLE) &&
                    (curr[0] != OAM_LOOPBACK_COMMAND_DISABLE)) {
                OAM_DEBUG("Unknown remote loopback command\n");
            }
            break;

        case OAM_PDU_CODE_ORG_SPEC:
        case OAM_PDU_PASSAVE_PING:
            break;

        default:
            OAM_DEBUG("Unknown OAMPDU code\n");
            return -1;
    }

    return 0;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_build_pdu_hdr                                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_build_pdu_hdr(
        oam_if_t *intf, 
        oam_pdu_hdr_t *pdu, 
        cs_uint8 code)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o pdu - the oam packet                                          */
/*           o code - oam packet std code                                    */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* Buid the OAM packet header                                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_int16 flags;

    memcpy(pdu->dst_mac, oam_slow_protocols_multicast, OAM_MACADDR_LEN);
    memcpy(pdu->src_mac, intf->macaddr, OAM_MACADDR_LEN);
    pdu->eth_type = htons(OAM_SLOW_PROTOCOLS_TYPE);
    pdu->sub_type = OAM_SUBTYPE;
    flags = 0;
    if (intf->local_critical_event) {
        flags |= OAM_FLAG_CRITICAL_EVENT;
    }
    if (intf->local_dying_gasp) {
        flags |= OAM_FLAG_DYING_GASP;
    }
    if (intf->local_link_status == OAM_LINK_STATUS_FAIL) {
        flags |= OAM_FLAG_LINK_FAULT;
    }
    if (intf->remote_state_valid) {
        flags |= intf->remote_flags;
        if (intf->local_satisfied) {
            flags |= (intf->d2_compatible) ?
                OAM_2DOT0_FLAG_LOCAL_STABLE :
                OAM_FLAG_LOCAL_STABLE;
        }
    }
    else if (!intf->d2_compatible) {
        flags |= OAM_FLAG_LOCAL_EVALUATING;
    }

    pdu->flags = htons(flags);
    pdu->code = code;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_build_pdu_info                                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_int32 oam_build_pdu_info(
        oam_if_t *intf, 
        void *parm,
        oam_pdu_info_t *pdu)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o parm - parameters for build the pdu info                      */
/*           o pdu - oam packet                                              */
/* OUTPUTS : oam packet length                                               */
/* DESCRIPTION:                                                              */
/* Buid the OAM info packet                                                  */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_int32 len;
    cs_int16 tmp;
    cs_uint8 *pos;
    cs_uint32 vendor_len;

    len = sizeof(oam_pdu_hdr_t);
    pos = (cs_uint8 *)pdu + len;

    oam_build_pdu_hdr(intf, (oam_pdu_hdr_t *)pdu, OAM_PDU_CODE_INFO);

    if (intf->local_pdu != OAM_LOCAL_PDU_LF_INFO) {
        pdu->local_info.type = OAM_INFO_TLV_LOCAL_INFO;
        pdu->local_info.length = sizeof(oam_tlv_info_t);
        pdu->local_info.version = OAM_VERSION;
        if (intf->info_tlv_changed) {
            intf->revision_next_tx++;
            intf->info_tlv_changed = 0;
        }
        pdu->local_info.revision = htons(intf->revision_next_tx);
        if (intf->local_mux_action == OAM_MUX_ACTION_DISCARD) {
            if (intf->d2_compatible) {
                pdu->local_info.state |=
                    OAM_2DOT0_INFO_STATE_MUX_DISCARD;
            }
            else {
                pdu->local_info.state |=
                    OAM_INFO_STATE_MUX_DISCARD;
            }
        }
        if (intf->local_par_action == OAM_PAR_ACTION_LB) {
            if (intf->d2_compatible) {
                pdu->local_info.state |=
                    OAM_2DOT0_INFO_STATE_PAR_LB;
            }
            else {
                pdu->local_info.state |=
                    OAM_INFO_STATE_PAR_LB;
            }
        }
        else if (intf->local_par_action == OAM_PAR_ACTION_DISCARD) {
            if (intf->d2_compatible) {
                pdu->local_info.state |=
                    OAM_2DOT0_INFO_STATE_PAR_DISCARD;
            }
            else {
                pdu->local_info.state |=
                    OAM_INFO_STATE_PAR_DISCARD;
            }
        }
        if (intf->local_oam_mode == OAM_MODE_ACTIVE) {
            pdu->local_info.oam_config |= OAM_INFO_CONFIG_OAM_MODE;
        }
        if (intf->local_unidir) {
            pdu->local_info.oam_config |= OAM_INFO_CONFIG_UNIDIR;
        }

        /* interop */
        pdu->local_info.oam_config |= OAM_INFO_CONFIG_REMOTE_LB |
            OAM_INFO_CONFIG_LINK_EVENT;
        if (intf->d2_compatible) {
            pdu->local_info.oam_config |=
                OAM_2DOT0_INFO_CONFIG_ORG_SPEC_OAMPDU;
        }

        pdu->local_info.oampdu_config = ntohs(intf->remote_tlv_info.oampdu_config);
        if((pdu->local_info.oampdu_config != intf->local_oampdu_config) && (pdu->local_info.oampdu_config <= OAM_MAX_OAMPDU_SIZE))
        {
            intf->local_oampdu_config = pdu->local_info.oampdu_config;
        }

        pdu->local_info.oampdu_config = htons(intf->local_oampdu_config);

        if (intf->d2_compatible) {
            /* interop */
            tmp = 128;
            pdu->local_info.oampdu_config = htons(tmp);
        }

        if (intf->d2_compatible) {
            memcpy(pdu->local_info.oui, oam_oui_pas, OAM_OUI_LEN);
        }
        else {
            memcpy(pdu->local_info.oui, oam_oui_imst, OAM_OUI_LEN);
        }

        pdu->local_info.vendor[1] = 0x00;
        if (intf->d2_compatible) {
            /* interop */
            pdu->local_info.vendor[0] = 0x00;
            //pdu->local_info.vendor[1] = 0x00;
            pdu->local_info.vendor[2] = 0x10;
            pdu->local_info.vendor[3] = 0x00;
            //pdu->local_info.vendor[4] = 0x02;
            //pdu->local_info.vendor[5] = 0x10;
        }
        else
        {
            /* interop */
            pdu->local_info.vendor[0] = 0x00;
            //pdu->local_info.vendor[1] = 0x00;
            pdu->local_info.vendor[2] = 0x00;
            pdu->local_info.vendor[3] = 0x00;
        }

        /* Local_info Vendor build */
        oam_vendor_build_handler(OAM_VENDOR_HANDLE_INFO_BUILD,
                intf->port, (cs_uint8*) &pdu->local_info,sizeof(pdu->local_info));


        pos += sizeof(oam_tlv_info_t);
        len += sizeof(oam_tlv_info_t);

        if (intf->remote_state_valid) {
            memcpy(&pdu->remote_info, &intf->remote_tlv_info,
                    sizeof(oam_tlv_info_t));
            pos += sizeof(oam_tlv_info_t);
            len += sizeof(oam_tlv_info_t);
        }

        /* Extend discovery information */
        vendor_len = oam_vendor_build_handler(OAM_VENDOR_HANDLE_EXT_INFO_BUILD,
                intf->port, pos,OAM_MAX_OAMPDU_SIZE - len);

        if(vendor_len != -1){
            len += vendor_len;
        }

    }

    intf->info_tx++;
    return len;
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_build_pdu_var_rsp                                     */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static cs_int32 oam_build_pdu_var_rsp(
        oam_if_t *intf, 
        oam_parm_oampdu_var_rsp_t *parm,
        oam_pdu_var_rsp_t *pdu)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o parm - parameters for build the var response packet           */
/*           o pdu - oam packet                                              */
/* OUTPUTS : oam packet length                                               */
/* DESCRIPTION:                                                              */
/* Buid the OAM var response packet                                          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_int32 totalSize = 0;
    cs_int32 size = 0;
    cs_uint8 *pData = (cs_uint8 *)&pdu->container[0];
    oam_parm_oampdu_var_rsp_t *pParm;
    oam_var_cont_t *pCont = NULL;

    oam_build_pdu_hdr(intf, (oam_pdu_hdr_t *)pdu, OAM_PDU_CODE_VAR_RSP);
    if(intf->d2_compatible)
    {
        oam_d2_parm_oampdu_var_rsp_t *pParm_d2;
        oam_d2_var_cont_t *pD2Cont = NULL;

        pParm_d2 = (oam_d2_parm_oampdu_var_rsp_t *)parm;
        pD2Cont = (oam_d2_var_cont_t *)pData;

        totalSize = sizeof(oam_pdu_hdr_t);

        while(pParm_d2->branch)
        {
            unsigned long tmpl = 0;
            memset(pD2Cont, 0, sizeof(oam_d2_var_cont_t));
            pD2Cont->leaf = pParm_d2->leaf;
            pD2Cont->branch = pParm_d2->branch;

            memcpy(&tmpl, parm->value, sizeof(tmpl));

            size = oam_d2_set_mib_variable(intf, pParm_d2->branch, pParm_d2->leaf, pD2Cont, tmpl);
            totalSize += size;
            pData += size;
            pD2Cont = (oam_d2_var_cont_t *)pData;
            pParm_d2++;
        }

        *pData++ = 0;
        *pData = 0;
        totalSize += 2;

        OAM_DEBUG( "sending D2 oam len %d\n", totalSize);
        intf->var_rsp_tx++;
        return totalSize;
    }
    pParm = (oam_parm_oampdu_var_rsp_t *)parm;
    pCont = (oam_var_cont_t *)pData;
    totalSize = sizeof(oam_pdu_hdr_t);
    while(pParm->branch)
    {
        unsigned long tmpl = 0;
        cs_int16 tmps = 0;
        memset(pCont, 0, sizeof(oam_var_cont_t));
        pCont->branch = pParm->branch;
        pCont->leaf = pParm->leaf;
        pCont->width = pParm->width;


        if(pParm->width == sizeof(unsigned long))
        {
            memcpy(&tmpl, pParm->value, pParm->width);
            tmpl = ntohl(tmpl);
            size = oam_set_mib_variable(intf, pParm->branch, ntohs(pParm->leaf), pCont, tmpl);
        }
        else if(pParm->width == sizeof(cs_int16))
        {
            memcpy(&tmps, pParm->value, pParm->width);
            tmps = ntohs(tmps);
            size = oam_set_mib_variable(intf, pParm->branch, ntohs(pParm->leaf), pCont, tmps);
        }
        else {
            size = oam_set_mib_variable(intf, pParm->branch, ntohs(pParm->leaf), pCont, pParm->value[0]);
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

/*****************************************************************************/
/* $rtn_hdr_start  oam_proc_pdu_info                                         */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static void oam_proc_pdu_info(
        oam_if_t *intf, 
        oam_pdu_info_t *pdu, 
        cs_int32 len)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o pdu - oam packet                                              */
/*           o len - oam packet length                                       */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process the OAM info packet                                               */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_parm_req_oam_ctl_t parm;
    cs_uint8 reply = OAM_FALSE;
    cs_int16 flags = ntohs(pdu->hdr.flags);

    intf->info_rx++;
    intf->remote_pdu_flags = flags;
    if (flags & OAM_FLAG_LINK_FAULT || flags & OAM_FLAG_DYING_GASP) {
        parm.mask = OAM_CTL_REQ_MASK_REMOTE_STATE_VALID |
            OAM_CTL_REQ_MASK_LOCAL_SATISFIED;
        parm.remote_state_valid = OAM_FALSE;
        parm.local_satisfied = OAM_FALSE;
        oam_req_oam_ctl(intf, &parm);
        OAM_DEBUG( "Link fault/Dying gasp event\n");
        return;
    }

    if (pdu->local_info.revision == intf->revision_last_rx) {
        OAM_DEBUG(
                "Skip local info TLV with no revision change\n");
        memcpy(&intf->remote_tlv_info, &pdu->local_info,
                sizeof(oam_tlv_info_t));
        intf->remote_tlv_info.type = OAM_INFO_TLV_REMOTE_INFO;

        if(oam_vendor_proc_handler(OAM_VENDOR_HANDLE_EXT_INFO_PROC,
                    intf->port,(cs_uint8*)pdu,len)){
            return;
        }else{ /* if no vendor define, use std instead*/
            if(intf->local_pdu == OAM_LOCAL_PDU_ANY)
            {
                if(intf->reply)
                {
                    oam_req_oampdu(intf, (oam_build_oampdu_t)oam_build_pdu_info,NULL);
                    intf->reply = OAM_FALSE;
                }
                return;
            }
        }
    }
    if ((intf->local_oam_mode == OAM_MODE_PASSIVE) &&
            (intf->local_pdu == OAM_LOCAL_PDU_RX_INFO)) {
        reply = OAM_TRUE;
    }

    if((intf->local_oam_mode == OAM_MODE_PASSIVE) &&
            (intf->remote_stable) && (intf->local_pdu != OAM_LOCAL_PDU_ANY))
    {
        reply = OAM_TRUE;
    }

    intf->revision_last_rx = pdu->local_info.revision;

    if (intf->d2_compatible) {
        intf->remote_flags = (flags & OAM_2DOT0_FLAG_LOCAL_STABLE) >> 1;
        intf->remote_oam_config = pdu->local_info.oam_config &
            OAM_2DOT0_INFO_CONFIG_MASK;
    }
    else {
        intf->remote_flags = (flags & (OAM_FLAG_LOCAL_EVALUATING |
                    OAM_FLAG_LOCAL_STABLE)) << 2;
        intf->remote_oam_config = pdu->local_info.oam_config &
            OAM_INFO_CONFIG_MASK;
    }

    intf->remote_oampdu_config = ntohs(pdu->local_info.oampdu_config) &
        OAM_INFO_PDU_CONFIG_MASK;
    memcpy(&intf->remote_tlv_info, &pdu->local_info,
            sizeof(oam_tlv_info_t));
    intf->remote_tlv_info.type = OAM_INFO_TLV_REMOTE_INFO;

    /*Vendor infor process*/
    oam_vendor_proc_handler(OAM_VENDOR_HANDLE_INFO_PROC,
            intf->port,(cs_uint8*)pdu,len);


    parm.mask = OAM_CTL_REQ_MASK_REMOTE_STATE_VALID;
    parm.remote_state_valid = OAM_TRUE;

    parm.mask |= OAM_CTL_REQ_MASK_LOCAL_SATISFIED;
    if ((pdu->local_info.version != OAM_VERSION) ||
            (intf->remote_oampdu_config < OAM_MIN_OAMPDU_SIZE) ||
            (intf->remote_oampdu_config > OAM_MAX_OAMPDU_SIZE)) {
        parm.local_satisfied = OAM_FALSE;
    }
    else {
        parm.local_satisfied = OAM_TRUE;
    }

    if (intf->lb_state_peer == OAM_LB_STATE_PEER_INIT) {
        if ((!intf->d2_compatible &&
                    (pdu->local_info.state & OAM_INFO_STATE_MUX_DISCARD) &&
                    ((pdu->local_info.state & OAM_INFO_STATE_PAR_MASK) ==
                     OAM_INFO_STATE_PAR_LB)) ||
                (intf->d2_compatible &&
                 (pdu->local_info.state & OAM_2DOT0_INFO_STATE_MUX_DISCARD) &&
                 ((pdu->local_info.state & OAM_2DOT0_INFO_STATE_PAR_MASK) ==
                  OAM_2DOT0_INFO_STATE_PAR_LB))) {
            parm.mask |= OAM_CTL_REQ_MASK_LOCAL_MUX_ACTION;
            parm.local_mux_action = OAM_MUX_ACTION_FWD;

            intf->lb_state_peer = OAM_LB_STATE_PEER_LB;
            /*oam_env_loopback_ind(intf->port, OAM_TRUE);*/
        }
    }
    else if (intf->lb_state_peer == OAM_LB_STATE_PEER_EXIT) {
        if ((!intf->d2_compatible &&
                    !(pdu->local_info.state & OAM_INFO_STATE_MUX_DISCARD) &&
                    ((pdu->local_info.state & OAM_INFO_STATE_PAR_MASK) ==
                     OAM_INFO_STATE_PAR_FWD)) ||
                (intf->d2_compatible &&
                 !(pdu->local_info.state & OAM_2DOT0_INFO_STATE_MUX_DISCARD) &&
                 ((pdu->local_info.state & OAM_2DOT0_INFO_STATE_PAR_MASK) ==
                  OAM_2DOT0_INFO_STATE_PAR_FWD))) {
            parm.mask |= OAM_CTL_REQ_MASK_LOCAL_MUX_ACTION |
                OAM_CTL_REQ_MASK_LOCAL_PAR_ACTION;
            parm.local_mux_action = OAM_MUX_ACTION_FWD;
            parm.local_par_action = OAM_PAR_ACTION_FWD;

            intf->lb_state_peer = OAM_LB_STATE_PEER_NORM;
            /* oam_env_loopback_ind(intf->port, OAM_FALSE);*/
        }
    }

    oam_req_oam_ctl(intf, &parm);

    if (reply) {
        oam_req_oampdu(intf, (oam_build_oampdu_t)oam_build_pdu_info,
                NULL);
    }

    oam_vendor_proc_handler(OAM_VENDOR_HANDLE_EXT_INFO_PROC,
                    intf->port,(cs_uint8*)pdu,len);
}


/*****************************************************************************/
/* $rtn_hdr_start  oam_proc_hdr                                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static void oam_proc_hdr(
        oam_if_t *intf, 
        oam_pdu_hdr_t *pdu)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o pdu - oam packet                                              */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process the OAM packet header                                             */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_int16 events;
    oam_parm_req_oam_ctl_t parm;
    cs_uint32 flags;
    /* check for remote stable */
    if (intf->d2_compatible) {
        flags = ntohs(pdu->flags) & OAM_2DOT0_FLAG_LOCAL_STABLE;
        intf->remote_flags = flags >> 1;
    }
    else {
        flags = ntohs(pdu->flags) & (OAM_FLAG_LOCAL_EVALUATING | OAM_FLAG_LOCAL_STABLE);
        intf->remote_flags = flags << 2;
    }

    parm.mask= 0;
    if ((intf->d2_compatible &&
                (flags == OAM_2DOT0_FLAG_LOCAL_STABLE)) ||
            (!intf->d2_compatible &&
             (flags == OAM_FLAG_LOCAL_STABLE))) {
        parm.mask |= OAM_CTL_REQ_MASK_REMOTE_STABLE;
        parm.remote_stable = OAM_TRUE;
    }
    else {
        parm.mask |= OAM_CTL_REQ_MASK_REMOTE_STABLE;
        parm.remote_stable = OAM_FALSE;
    }

    if (parm.mask) {
        oam_req_oam_ctl(intf, &parm);
    }

    /* signalling critical link events */
    events = ntohs(pdu->flags) & OAM_FLAG_MASK_EVENTS;
    if (events) {
        /* oam_env_critical_event_ind(intf->port, events); */
    }
}


/*****************************************************************************/
/* $rtn_hdr_start  oam_proc_pdu_event                                        */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static void oam_proc_pdu_event(
        oam_if_t *intf, 
        oam_pdu_event_t *pdu, 
        cs_int32 len)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o pdu - oam packet                                              */
/*           o len - oam packet length                                       */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process the OAM event packet                                              */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_parm_req_oam_ctl_t parm;
    cs_int16 flags = ntohs(pdu->hdr.flags);
    /*
       cs_int32 num, max;
       oam_tlv_link_event_t *curr;
       cs_uint8 *next;
       */

    if (intf->seq_num_last_rx == pdu->seq_num) {
        intf->dup_event_rx++;
    }
    else {
        intf->unique_event_rx++;
    }

    intf->seq_num_last_rx = pdu->seq_num;

    if (flags & OAM_FLAG_LINK_FAULT || flags & OAM_FLAG_DYING_GASP) {
        parm.mask = OAM_CTL_REQ_MASK_REMOTE_STATE_VALID |
            OAM_CTL_REQ_MASK_LOCAL_SATISFIED;
        parm.remote_state_valid = OAM_FALSE;
        parm.local_satisfied = OAM_FALSE;
        oam_req_oam_ctl(intf, &parm);
        OAM_DEBUG( "Link fault/Dying gasp event\n");
        return;
    }

    /* parse the pdu*/
    /*
       num = 0;
       curr = &pdu->event[0];

       max = len - sizeof(oam_pdu_hdr_t) - sizeof(pdu->seq_num);
       while (curr) {
       oam_proc_tlv((cs_uint8 *)curr, &max, &next);
       if (curr->type != 0) {
       num++;
       }
       curr = (oam_tlv_link_event_t *)next;
       }

       oam_env_event_ind(intf->port, num, &pdu->event[0]);
       */
}
/*****************************************************************************/
/* $rtn_hdr_start  oam_proc_pdu_var_req                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static void oam_proc_pdu_var_req(
        oam_if_t *intf, 
        oam_pdu_var_req_t *pdu, 
        cs_int32 len)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o pdu - oam packet                                              */
/*           o len - oam packet length                                       */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process the OAM variable request packet                                   */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    cs_int32 num, max;
    oam_var_desc_t *curr, *next;
    oam_parm_oampdu_var_rsp_t parm[OAM_MAX_VAR_REQ_ENTRIES];
    cs_int32 count = 0;
    oam_var_cont_t *curr_container = NULL;
    cs_uint8 *pSetData = NULL;

    intf->var_req_rx++;

    num = 0;
    if(intf->d2_compatible) {
        oam_d2_pdu_var_req_t *pdu_d2 = (oam_d2_pdu_var_req_t *)pdu;
        oam_d2_var_desc_t *curr_d2, *next_d2;
        oam_d2_parm_oampdu_var_rsp_t parm_d2[OAM_2DOT0_MAX_VAR_REQ_ENTRIES];

        memset(parm_d2, 0, sizeof(parm_d2));
        curr_d2 = &pdu_d2->descriptor[0];
        max = len - sizeof(oam_pdu_hdr_t);
        while (curr_d2) {
            oam_d2_proc_var_desc(curr_d2, &max, &next_d2);
            if (curr_d2->branch != 0) {
                OAM_DEBUG(
                        "%d - rcv d2 var req branch 0x%x leaf 0x%x\n", count, curr_d2->branch, curr_d2->leaf);
                parm_d2[count].branch = curr_d2->branch;
                parm_d2[count].leaf = curr_d2->leaf;
                count++;
                if(count >= OAM_2DOT0_MAX_VAR_REQ_ENTRIES)
                    break;
            }
            curr_d2 = next_d2;
        }

        oam_req_oampdu(intf, (oam_build_oampdu_t)
                oam_build_pdu_var_rsp, &parm_d2[0]);
        return;
    }

    curr = &pdu->descriptor[0];
    max = len - sizeof(oam_pdu_hdr_t);
    while (curr) {
        oam_proc_var_desc(curr, &max, &next);
        if (curr->branch != 0) {
            OAM_DEBUG(
                    "rcv var req branch 0x%x leaf 0x%x\n", curr->branch, ntohs(curr->leaf));
            parm[count].branch = curr->branch;
            parm[count].leaf = curr->leaf;
            parm[count].width = 0;

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

    oam_req_oampdu(intf, (oam_build_oampdu_t)oam_build_pdu_var_rsp, &parm);
}
/*****************************************************************************/
/* $rtn_hdr_start  oam_proc_pdu_loopback                                     */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static void oam_proc_pdu_loopback(
        oam_if_t *intf, 
        oam_pdu_loopback_t *pdu, 
        cs_int32 len)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o pdu - oam packet                                              */
/*           o len - oam packet length                                       */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process the OAM loopback packet                                           */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_parm_req_oam_ctl_t parm;

    intf->lb_control_rx++;

    if ((intf->lb_state_peer == OAM_LB_STATE_PEER_INIT) &&
            oam_macaddr_higher(pdu->hdr.src_mac, intf->macaddr)) {
        return;
    }

    if ((pdu->command == OAM_LOOPBACK_COMMAND_ENABLE) &&
            (intf->lb_state_self == OAM_LB_STATE_SELF_NORM)) {
        intf->lb_state_self = OAM_LB_STATE_SELF_LB;
        parm.mask = OAM_CTL_REQ_MASK_LOCAL_MUX_ACTION |
            OAM_CTL_REQ_MASK_LOCAL_PAR_ACTION;
        parm.local_mux_action = OAM_MUX_ACTION_DISCARD;
        parm.local_par_action = OAM_PAR_ACTION_LB;
        oam_oob_send_loopback_event(pdu->command);
    }
    else if ((pdu->command == OAM_LOOPBACK_COMMAND_DISABLE) &&
            (intf->lb_state_self == OAM_LB_STATE_SELF_LB)) {
        intf->lb_state_self = OAM_LB_STATE_SELF_NORM;
        parm.mask = OAM_CTL_REQ_MASK_LOCAL_MUX_ACTION |
            OAM_CTL_REQ_MASK_LOCAL_PAR_ACTION;
        parm.local_mux_action = OAM_MUX_ACTION_FWD;
        parm.local_par_action = OAM_PAR_ACTION_FWD;
        oam_oob_send_loopback_event(pdu->command);
    }
    else {
        OAM_DEBUG(
                "loopback cmd inconsistent with loopback state\n");
        return;
    }

    oam_req_oam_ctl(intf, &parm);
    oam_req_oampdu(intf, (oam_build_oampdu_t)oam_build_pdu_info, NULL);
}


/*****************************************************************************/
/* $rtn_hdr_start  oam_proc_pdu_orgspec                                      */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
static void oam_proc_pdu_orgspec(
        oam_if_t *intf, 
        oam_pdu_orgspec_t *pdu, 
        cs_int32 len)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o pdu - oam packet                                              */
/*           o len - oam packet length                                       */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process the OAM org special packet                                        */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    OAM_ASSERT(intf != NULL);
    /*
     * vendor specific ONU OAM handling
     */
    if(oam_vendor_handler(pdu->oui,
                OAM_VENDOR_HANDLE_ORG,
                intf->port,(cs_uint8*)pdu,len)){
        return;
    }
    else
    {
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_ind_oampdu                                            */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_ind_oampdu(
        oam_if_t *intf, 
        oam_pdu_hdr_t *pdu, 
        cs_int32 len)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o pdu - oam packet                                              */
/*           o len - oam packet length                                       */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process all the OAM packet                                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    if (oam_proc_pdu_sanity(intf, pdu, len)) {
        OAM_LOG_MAJ( "OAMPDU sanity check fails\n");
        return;
    }
    if (pdu->code != OAM_PDU_CODE_ORG_SPEC
            && pdu->code != OAM_PDU_PASSAVE_PING) {
        oam_proc_hdr(intf, pdu);
    }

    oam_start_local_lost_link_timer(intf);
    intf->frame_rx++;

    if(pdu->code != OAM_PDU_CODE_ORG_SPEC && oam_len_check)
    {
        if(len-4 > intf->local_oampdu_config) { /* remove crc */
            intf->frame_lost++;
            return;
        }
    }
    switch (pdu->code) {
        case OAM_PDU_CODE_INFO:
            OAM_DEBUG( "OAMPDU info rx\n");
            oam_proc_pdu_info(intf, (oam_pdu_info_t *)pdu, len);
            break;
        case OAM_PDU_CODE_EVENT:
            OAM_DEBUG( "OAMPDU event rx\n");
            oam_proc_pdu_event(intf, (oam_pdu_event_t *)pdu, len);
            break;

        case OAM_PDU_CODE_VAR_REQ:
            OAM_DEBUG( "OAMPDU varreq rx\n");
            if(!oam_std_handler(OAM_PDU_CODE_VAR_REQ,
                        intf->port,
                        (cs_uint8*)pdu,len)){
                oam_proc_pdu_var_req(intf, (oam_pdu_var_req_t *)pdu, len);
            }
            break;

        case OAM_PDU_CODE_VAR_RSP:
            OAM_DEBUG( "OAMPDU varrsp rx\n");
            /* oam_proc_pdu_var_rsp(intf, (oam_pdu_var_rsp_t *)pdu, len); */
            break;

        case OAM_PDU_CODE_LOOPBACK:
            OAM_DEBUG( "OAMPDU loopback rx\n");
            oam_proc_pdu_loopback(intf, (oam_pdu_loopback_t *)pdu, len);
            break;

        case OAM_PDU_CODE_ORG_SPEC:
            OAM_DEBUG( "OAMPDU orgspec rx\n");
            intf->org_specific_rx++;
            oam_proc_pdu_orgspec(intf, (oam_pdu_orgspec_t *)pdu, len);
            break;

        case OAM_PDU_PASSAVE_PING:
            intf->org_specific_rx++;
            oam_std_handler(OAM_PDU_PASSAVE_PING,
                    intf->port,
                    (cs_uint8*)pdu,len);
            break;

        default:
            intf->unsupported_rx++;
            oam_printf("Unknown OAMPDU code %d\n", pdu->code);
            break;
    }
}
/*****************************************************************************/
/* $rtn_hdr_start  oam_ind_oami                                              */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void  oam_ind_oami(
        cs_llid_t llid, 
        oam_pdu_hdr_t *pdu, 
        cs_int32 len)
/*                                                                           */
/* INPUTS  : o intf - oam core interface                                     */
/*           o pdu - oam packet                                              */
/*           o len - oam packet length                                       */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* process all the OAM packet                                                */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_if_t * intf = oam_intf_find(llid);

    if (intf == NULL) {
        OAM_LOG_MAJ( "Interface not found\n");
        return;
    }

    if (oam_rx_rule_passed(intf, pdu)) {
        oam_ind_oampdu(intf, pdu, len);
    }
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_llid_registration                                     */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_llid_registration(cs_llid_t llid)
/*                                                                           */
/* INPUTS  : o llid - onu llid                                               */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* oam llid register process                                                 */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_if_t *oam_intf = NULL; 

    oam_intf = oam_intf_find(llid);
    if (oam_intf) {
        oam_intf_free(oam_intf);
    }

    oam_intf = oam_intf_alloc();
    if (!oam_intf) {
        OAM_LOG("oam_intf_alloc: failed (1)\n");
        return;
    }

    oam_intf_init(oam_intf, llid, OAM_MODE_PASSIVE);

    oam_sdl_get_llid_mac(llid,(cs_mac_t *)oam_intf->macaddr);

    oam_enable(oam_intf);
}

/*****************************************************************************/
/* $rtn_hdr_start  oam_llid_deregistration                                     */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
void oam_llid_deregistration(cs_llid_t llid)
/*                                                                           */
/* INPUTS  : o llid - onu llid                                               */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* oam llid deregister process                                                 */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_if_t *oam_intf = NULL; 
    oam_intf = oam_intf_find(llid);

    if (oam_intf != NULL) {
        oam_intf_free(oam_intf);
    }
    else {
        OAM_LOG("oam_intf_find: failed\n");
    }
}


