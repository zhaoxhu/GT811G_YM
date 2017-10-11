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
#ifdef HAVE_IP_STACK
#include <network.h>
#include "app_vlan.h"
#include "cs_module.h"
#include "cs_cmn.h"
#include "cs_module.h"
#include "mempool.h"
#include "osal_api_core.h"
#include "sdl_api.h"

#include "app_ip_types.h"
#include "app_ip_api.h"
#include "app_ip_init.h"
#include "app_ip_util.h"

#include "app_ip_mgnt.h"


#include "db_instance.h"
#ifdef HAVE_ZTE_SIJIE
#define EOC_PON_VLAN_ID 4088
#endif /*END_HAVE_ZTE_SIJIE*/

void app_ipintf_pkt_dir_default()
{
    cs_callback_context_t     context;
    epon_request_onu_spec_pkt_state_set(context, 0, 0, S_DOWNSTREAM, CS_PKT_ARP, DST_FE);
    epon_request_onu_spec_pkt_state_set(context, 0, 0, S_UPSTREAM, CS_PKT_ARP, DST_CPU);
    epon_request_onu_spec_pkt_state_set(context, 0, 0, S_DOWNSTREAM, CS_PKT_MYMAC, DST_FE);
    epon_request_onu_spec_pkt_state_set(context, 0, 0, S_UPSTREAM, CS_PKT_MYMAC, DST_CPU);
}
cs_status app_ipintf_outband_enable()
{
    cs_status ret = CS_E_OK;
    db_mgmt_t  mgmt_info;
    
    memset(&mgmt_info,0x00,sizeof(mgmt_info));
    db_read(DB_MGMT_INFO_ID,(cs_uint8*) &mgmt_info,sizeof(mgmt_info));

    if(APP_IPINTF_OUTBNAD_MGNT == mgmt_info.inband_enable) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"No need set to OUTBAND \n");
        return CS_E_OK;
    }
    ret = app_ipintf_ip_config_set(
                mgmt_info.ip,
                mgmt_info.mask,
                0);
    
    if(ret != CS_E_OK) {
        return ret;
    }
    ret = app_ipintf_set_wan_cfg(
        APP_IPINTF_OUTBNAD_MGNT,
        mgmt_info.inband_pri,
        mgmt_info.inband_vlan,
        mgmt_info.mgmt_net,
        mgmt_info.mgmt_mask,
        mgmt_info.gw);
    
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"Set to OUTBAND \n");
    return ret;
}

cs_status app_ipintf_inband_enable()
{
    cs_status ret = CS_E_OK;
    db_mgmt_t  mgmt_info;
    
    memset(&mgmt_info,0x00,sizeof(mgmt_info));
    db_read(DB_MGMT_INFO_ID,(cs_uint8*) &mgmt_info,sizeof(mgmt_info));

    if(APP_IPINTF_OUTBNAD_MGNT == mgmt_info.inband_enable) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"No need set to INBAND \n");
        return CS_E_OK;
    }
    ret = app_ipintf_ip_config_set(
                mgmt_info.inband_ip,
                mgmt_info.inband_mask,
                mgmt_info.gw);

    if(ret != CS_E_OK) {
        return ret;
    }
    ret = app_ipintf_set_wan_cfg(
        APP_IPINTF_INBNAD_MGNT,
        mgmt_info.inband_pri,
        mgmt_info.inband_vlan,
        mgmt_info.mgmt_net,
        mgmt_info.mgmt_mask,
        mgmt_info.gw);
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG,"Set to INBAND \n");
    
    return ret;
    
}

#if 1
//modified by zhuxh, all arp should be passed
ipintf_filter_retcode app_ipintf_ctc_rx_filter(cs_pkt_t *pkt)
{
	return IPINTF_PASS;
}
#else
/*
*   PROTOTYPE    cs_status app_ipintf_ctc_ingress_filter(cs_pkt_t *pkt)
*   INPUT            pkt
*   OUTPUT         None.
*   Description     
*   Pre-condition  
*   Post-condition
*   Note             inband management mode :
*                            All arp packets from UNI ports should be discarded.
*                            All ip packets from UNI ports should be discarded.
*                            All packets from PON port should be checked with vlan.
*/
ipintf_filter_retcode app_ipintf_ctc_rx_filter(cs_pkt_t *pkt)
{
    if((pkt->port == CS_PON_PORT_ID && !(ipintf_info.inband)) ||
        (pkt->port != CS_PON_PORT_ID && ipintf_info.inband)) {
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "ARP/IP packet from invalid port %d, %s mode \n", 
            pkt->port, ipintf_info.inband?"inband":"outband");
        return IPINTF_DROP;
    }
    if(pkt->port == CS_PON_PORT_ID) {
        if((pkt->svlan&0xfff) != ipintf_info.vlanid) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "ARP/IP packet from PON port, invalid vlan %d\n", pkt->svlan);
            return IPINTF_DROP;
        }
    }
    return IPINTF_PASS;
}
#endif

/*
*   PROTOTYPE    cs_status app_ipintf_ctc_ip_ingress_filter(cs_pkt_t *pkt)
*   INPUT            pkt
*   OUTPUT         None.
*   Description     
*   Pre-condition  
*   Post-condition
*   Note              inband management mode :
*                            All arp packets from UNI ports should be discarded.
*                            All ip packets from UNI ports should be discarded.
*                            All packets from PON port should be checked with vlan.
*/
ipintf_filter_retcode app_ipintf_ctc_ip_rx_filter(cs_pkt_t *pkt)
{
    if(IPOE_INPROGESS()) {
        if(/*(g_ipoe_state.port == pkt->port) && */app_dhcp_packet_parse(pkt)) {
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: receive dhcp packet port %d, %d\n", 
                __func__, g_ipoe_state.port, __LINE__);
            return IPINTF_PASS;
        }
    }
    return app_ipintf_ctc_rx_filter(pkt);
}

/*
*   PROTOTYPE    cs_status app_ipintf_ctc_arp_ingress_filter(cs_pkt_t *pkt)
*   INPUT            pkt
*   OUTPUT         None.
*   Description     
*   Pre-condition  
*   Post-condition
*   Note             CTC inband management mode :
*                            All arp packets from UNI ports should be discarded.
*                            All ip packets from UNI ports should be discarded.
*                            All packets from PON port should be checked with vlan.
*/
ipintf_filter_retcode app_ipintf_ctc_arp_rx_filter(cs_pkt_t *pkt)
{
    APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: %d\n", __func__,  __LINE__);
    if(IPOE_INPROGESS()) {
        /*if(g_ipoe_state.port == pkt->port) */{
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s: receive arp packet on port %d, IPOE port %d, %d\n", 
                __func__, pkt->port, g_ipoe_state.port, __LINE__);
            return IPINTF_PASS;
        }
    }
    return app_ipintf_ctc_rx_filter(pkt);
}

/*
*   PROTOTYPE    cs_status app_ipintf_ctc_arp_tx_special_process(cs_pkt_t *pkt)
*   INPUT            pkt
*   OUTPUT         None.
*   Description     
*   Pre-condition  
*   Post-condition
*   Note             the return code 1 means the whole process is finished.
*/
ipintf_filter_retcode app_ipintf_ctc_arp_tx_special_process(cs_pkt_t *pkt)
{
    cs_uint32 i = 0;
    if(ipintf_info.inband) {
        /* send PON port only */
        push_vlan_ingress(ipintf_info.vlanid, ipintf_info.pri, pkt);
        app_ipintf_send_pkt(CS_PON_PORT_ID, pkt);
        APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "BC send to PON (inband), len %d\n", pkt->len);
    }
    else {
        /* send UNI port */
#ifdef HAVE_ZTE_SIJIE
        push_vlan_ingress(EOC_PON_VLAN_ID, 0, pkt);
#endif /*END_HAVE_ZTE_SIJIE*/
        for(i=0;i<ipintf_info.maxport;i++) {
            if(PTABLE[i].port == CS_PON_PORT_ID)
                continue;
            
            /* TODO : how to push vlan ? there are should be no vlan ?*/
            app_ipintf_send_pkt(PTABLE[i].port,pkt);
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "BC flooding(outband) to %d, len %d\n",
                                PTABLE[i].port, pkt->len);
        }
    }
    return IPINTF_DROP;
}

/*
*   PROTOTYPE    void app_ipintf_pkt_special_process_reg(void)
*   INPUT            pkt
*   OUTPUT         None.
*   Description     
*   Pre-condition  
*   Post-condition
*   Note             
*/
void app_ipintf_pkt_special_process_reg(void)
{
    app_ipintf_ip_rx_filter = app_ipintf_ctc_ip_rx_filter;
    app_ipintf_arp_rx_filter = app_ipintf_ctc_arp_rx_filter;
    app_ipintf_arp_tx_filter = app_ipintf_ctc_arp_tx_special_process;
    app_ipintf_enable_inband = app_ipintf_inband_enable;
    app_ipintf_enable_outband = app_ipintf_outband_enable;
}



#endif
