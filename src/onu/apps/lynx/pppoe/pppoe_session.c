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
#ifdef HAVE_PPPOE
#include <stdio.h>
#include "plat_common.h"
#include "pppoe.h"
#include "pppoe_session.h"
#include "packet.h"
#include "app_vlan.h"
#include "pppoe_adapt.h"
#include "ppp_api.h"

cs_status pppoe_auth_mode_set(
    cs_uint8        mode,
    cs_int8        *name,
    cs_int8        *pwd)
{
    
    if(mode == AUTH_AUTO || mode == AUTH_PAP || mode == AUTH_CHAP){
        if(name && (strlen(name)<= PPP_USER_MAX_LEN)) {
            strncpy(g_pppoe_info.auth.name, name, strlen(name));
            g_pppoe_info.auth.name[strlen(name)] = '\0';
        } else
            g_pppoe_info.auth.name[0] = '\0';
        
        if(pwd && (strlen(pwd)<= PPP_PWD_MAX_LEN)) {
            strncpy(g_pppoe_info.auth.pwd, pwd, strlen(pwd));
            g_pppoe_info.auth.pwd[strlen(pwd)] = '\0';
        } else
            g_pppoe_info.auth.pwd[0] = '\0';
        
        g_pppoe_info.auth.mode = mode+1;
    }
    else{
        return CS_E_PARAM;
    }

    return CS_E_OK;
}

cs_boolean pppoe_header_check(
    cs_port_id_t port,
    pppoe_pkt_t *pPkt)
{
    pppoe_port_info_t *pInfo = NULL;
    cs_boolean ret = FALSE;
    
    if(port<1 || port>g_pppoe_info.max_port) 
    {
        return CS_E_PARAM;
    }
    
    pInfo = &g_pppoe_info.ports_info[port-1];

    if(pPkt->type != 1 || pPkt->ver != 1 || pPkt->code != 0){
        PPPOE_LOG_INFO("Invalid type or ver or code.\n");
        return ret;
    }

    if(ntohs(pPkt->length) > PPP_MAX_PAYLOAD)
    {
        PPPOE_LOG_INFO("Invalid length: %d\n", ntohs(pPkt->length));
        return ret;
    }

    if(pInfo->conn.session == ntohs(pPkt->session)){
        return TRUE;
    }

    return ret;
}

void pppoe_session_pkt_recv(cs_pkt_t* hdr)
{
    pppoe_pkt_t *pInPkt = NULL;
    ppp_pkt_t *pOutPkt = NULL;
    cs_port_id_t port = 0;
    cs_int32 ret = 0;

    if(!pppoe_discovery_completed(&port)){
        PPPOE_LOG_INFO("PPPoE discovery not completed.\n");
        return;
    }

    pInPkt = (pppoe_pkt_t *)(hdr->data+hdr->offset + sizeof(app_vlan_header_t)*hdr->tag_num);

    if(!pppoe_header_check(port, pInPkt)){
        PPPOE_LOG_INFO("PPPoE header check fail.\n");
        return;
    }

    pInPkt->length = ntohs(pInPkt->length);
    pOutPkt = (ppp_pkt_t *)iros_malloc(IROS_MID_PPPOE, sizeof(ppp_pkt_t));
    if (!pOutPkt) {
        PPPOE_LOG("pppoe_session_pkt_recv malloc failed.\n");
        return;
    }

    pOutPkt->len = pInPkt->length;
    memcpy(pOutPkt->data, pInPkt->payload, pInPkt->length);

    ret = cs_queue_put(g_pppoe_msgQ , (void *)&pOutPkt, sizeof(pOutPkt), CS_OSAL_NO_WAIT, 0);
    if(ret) {
        PPPOE_LOG("pkt send put to queue failed, 0x%x\n", ret);
        iros_free(pOutPkt);
    }

    return;
}

void pppoe_session_pkt_send(
        cs_uint8 *frame,
        cs_uint32 len)
{
    pppoe_pkt_t pkt;
    pppoe_pkt_t *pPkt = &pkt;
    cs_uint32 outlen = 0;
    cs_port_id_t port = 0;
    pppoe_port_info_t *pInfo = NULL;
    app_vlan_header_t vlan_header;

    if(!pppoe_discovery_completed(&port)){
        PPPOE_LOG_INFO("pppoe discovery not completed yet.\n");
        return;
    }

    pInfo = &g_pppoe_info.ports_info[port - 1];
    
    memset(&pkt, 0, sizeof(pppoe_pkt_t));
    memcpy(pkt.eth_hdr.dst.addr, pInfo->conn.peer_mac.addr, CS_MACADDR_LEN);
    memcpy(pkt.eth_hdr.src.addr, g_pppoe_info.my_mac.addr, CS_MACADDR_LEN);

    outlen = sizeof(pppoe_pkt_t) - PPPOE_MAX_PAYLOAD;
    if(pInfo->conn.vlan)
    {
        vlan_header.tpid = htons(0x8100);
        vlan_header.vlan = htons(pInfo->conn.vlan);
        memcpy(&pkt.eth_hdr.ethertype, &vlan_header, sizeof(app_vlan_header_t));
        pPkt = (pppoe_pkt_t*)((cs_uint8*)pPkt + sizeof(app_vlan_header_t));
    }
    
    pPkt->eth_hdr.ethertype = htons(EPON_ETHERTYPE_PPPOE_SESSION);
    pPkt->type = PPPOE_TYPE;
    pPkt->ver = PPPOE_VER;
    pPkt->code = PPPOE_CODE;
    pPkt->session = htons(pInfo->conn.session);
    pPkt->length = htons(len-2);

    memcpy(pPkt->payload, (frame+2), (len-2));
    outlen += len -2;
    
    if(pInfo->conn.vlan){
        outlen += sizeof(app_vlan_header_t);
    }
    pppoe_packet_send((cs_uint8 *)&pkt, outlen, g_pppoe_info.port, 7);

}

void ppp_nego_status_handler(
    cs_uint8 code,
    cs_uint32 ip_addr)
{
    pppoe_port_info_t *pInfo = NULL;

    pInfo = &g_pppoe_info.ports_info[g_pppoe_info.port-1];

    if(code == PPP_LCP_DOWN){
        pInfo->code = PPPOE_TEST_RET_LCP_NEG_FAIL;
        PPPOE_LOG_INFO("Lcp negotiation fail.\n");
    }
    else if(code == PPP_AUTH_FAIL){
        pInfo->code = PPPOE_TEST_RET_AUTH_FAIL;
        PPPOE_LOG_INFO("Authentication fail.\n");
    }
    else if(code == PPP_IPCP_NEG_FAIL){
        pInfo->code = PPPOE_TEST_RET_IPCP_NEG_FAIL;
        PPPOE_LOG_INFO("Ipcp negotiation fail.\n");
    }
    else if(code == PPP_IPCP_OK){
        pInfo->use_time = cs_current_time() - pInfo->start_time;
        pInfo->ip_addr = ip_addr;
        pInfo->code = PPPOE_TEST_RET_OK;

        PPPOE_LOG_INFO("Ipcp negotiation successful.\n");
        pppoe_state_change(g_pppoe_info.port, PPPOE_STATE_SESSION, TRUE, TRUE, PPPOE_TEST_RET_OK);
    }
    
    if((code != PPP_IPCP_OK)||(pInfo->padt_recv == TRUE)){
        PPPOE_LOG_INFO("Receive PADT and stop test.\n");
        pInfo->simul_rslt = PPPOE_TEST_FAIL;
        pppoe_simulation_stop(g_pppoe_info.port);
    }

#ifdef HAVE_PPP
    ppp_handler_dereg();
#endif
}

cs_status pppoe_session_start()
{
#ifdef HAVE_PPP
    ppp_pkt_tx_handler_reg(pppoe_session_pkt_send);
    ppp_nego_status_handler_reg(ppp_nego_status_handler);
    ppp_active();
    pppSetAuth(g_pppoe_info.auth.mode, g_pppoe_info.auth.name, g_pppoe_info.auth.pwd);
#endif

    return CS_E_OK;
}

cs_status pppoe_simulation_stop(
    cs_port_id_t port)
{
    pppoe_port_info_t *pInfo = NULL;
    
    if(port<1 || port>g_pppoe_info.max_port || port!= g_pppoe_info.port) 
    {
        PPPOE_LOG_INFO("Invalid port number :%d", port);
        return CS_E_PARAM;
    }

    pInfo = &g_pppoe_info.ports_info[port-1];

    if(pInfo->disc_rslt == PPPOE_TEST_RUNNING)
        pInfo->disc_rslt = PPPOE_TEST_FAIL;
    
    if(pInfo->simul_rslt == PPPOE_TEST_RUNNING)
        pInfo->simul_rslt = PPPOE_TEST_FAIL;
    
    pppoe_discovery_test_stop(port);
    
    return CS_E_OK;
}

cs_status pppoe_simulation_result_get(
        cs_port_id_t port,
        pppoe_test_result_t* result)
{
    pppoe_port_info_t *pInfo = NULL;

    if(port<1 || port>g_pppoe_info.max_port || !result) 
    {
        return CS_E_PARAM;
    }
    
    pInfo = &g_pppoe_info.ports_info[port-1];
    result->port = port;
    
    if(port != g_pppoe_info.port)
        return CS_E_OK;

    result->disc_rslt = pInfo->disc_rslt;
    result->simul_rslt = pInfo->simul_rslt;
    result->time = pInfo->use_time;
    result->auth_mode = g_pppoe_info.auth.mode;
    result->reason = pInfo->code;
    memcpy(&result->serv_mac, &(pInfo->conn.peer_mac), sizeof(cs_mac_t));
    result->ipv4 = pInfo->ip_addr;

    strncpy(result->name, g_pppoe_info.auth.name, strlen(g_pppoe_info.auth.name));
    result->name[strlen(g_pppoe_info.auth.name)] = '\0';

    strncpy(result->pwd, g_pppoe_info.auth.pwd, strlen(g_pppoe_info.auth.pwd));
    result->pwd[strlen(g_pppoe_info.auth.pwd)] = '\0';

    if(result->simul_rslt == PPPOE_TEST_OK)
        pppoe_discovery_test_stop(port);
    
    return CS_E_OK;
}

void pppoe_cmd_result_get(
    cs_port_id_t port)
{
    pppoe_test_result_t result;
    cs_uint8 temp1[20], temp2[20];
    
    memset(&result, 0, sizeof(pppoe_test_result_t));
    
    pppoe_simulation_result_get(port, &result);

    if(result.disc_rslt == PPPOE_TEST_NOT_RUN){
        strncpy(temp1, "Not start", strlen("Not start"));
        temp1[strlen("Not start")] = '\0';
    }
    else if(result.disc_rslt == PPPOE_TEST_RUNNING){
        strncpy(temp1, "Running", strlen("Running"));
        temp1[strlen("Running")] = '\0';
    }
    else if(result.disc_rslt == PPPOE_TEST_OK){
        strncpy(temp1, "Success", strlen("Success"));
        temp1[strlen("Success")] = '\0';
    }
    else if(result.disc_rslt == PPPOE_TEST_FAIL){
        strncpy(temp1, "Fail", strlen("Fail"));
        temp1[strlen("Fail")] = '\0';
    }
    else
        temp1[0] = '\0';

    if(result.simul_rslt == PPPOE_TEST_NOT_RUN){
        strncpy(temp2, "Not start", strlen("Not start"));
        temp2[strlen("Not start")] = '\0';
    }
    else if(result.simul_rslt == PPPOE_TEST_RUNNING){
        strncpy(temp2, "Running", strlen("Running"));
        temp2[strlen("Running")] = '\0';
    }
    else if(result.simul_rslt == PPPOE_TEST_OK){
        strncpy(temp2, "Success", strlen("Success"));
        temp2[strlen("Success")] = '\0';
    }
    else if(result.simul_rslt == PPPOE_TEST_FAIL){
        strncpy(temp2, "Fail", strlen("Fail"));
        temp2[strlen("Fail")] = '\0';
    }
    else
        temp2[0] = '\0';

    
    
    cs_printf("port         : %d\n", result.port);
    cs_printf("time         : %d\n", result.time);
    cs_printf("auth_mode    : %d\n", (result.auth_mode > 0)? (result.auth_mode-1) : 0);
    cs_printf("name         : %s\n", result.name);
    cs_printf("password     : %s\n", result.pwd);
    cs_printf("discovery result   : %s\n", temp1);
    cs_printf("simulation result  : %s\n", temp2);
    cs_printf("reason code  : %d\n", result.reason);
    cs_printf("server mac   : %x-%x-%x-%x-%x-%x\n", 
        result.serv_mac.addr[0],
        result.serv_mac.addr[1],
        result.serv_mac.addr[2],
        result.serv_mac.addr[3],
        result.serv_mac.addr[4],
        result.serv_mac.addr[5]);
#ifdef HAVE_PPP    
    cs_printf("ip address   : %s\n", _inet_ntoa(result.ipv4));
#endif
}

#endif

