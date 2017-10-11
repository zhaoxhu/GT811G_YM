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
#include "plat_common.h"
#include "pppoe.h"
#include "packet.h"
#include "app_vlan.h"
#include "pppoe_adapt.h"
#include "pppoe_session.h"
#include "ppp_api.h"
#ifdef HAVE_LOOP_DETECT
#include "loop_detect.h"
#endif

pppoe_cb_t  g_pppoe_info;
static cs_boolean g_pppoe_print = FALSE;
cs_uint32 g_pppoe_msgQ = 0;

#define PPPOE_IS_UNI_PORT(port) ((port)>=1 && (port)<=g_pppoe_info.max_port)

cs_status pppoe_packet_config(
        cs_uint8 direction,
        cs_uint8 action,
        cs_boolean plus)
{
    
    return pppoe_packet_config_adapt(direction, action, plus);
}

void pppoe_timer_start(
        cs_uint8 type,
        pppoe_port_info_t *pInfo)
{
    if(type == PPPOE_TIMER_PADI || type == PPPOE_TIMER_PADR)
    {
        pInfo->msg.msg_type = IROS_MSG_TIME_OUT;
        pInfo->msg.timer_type = APP_TIMER_PPPOE;
        pInfo->timer_handler = cs_msg_timer_add(app_msg_q, PPPOE_TIMEOUT_INTERVAL, (void *)(&pInfo->msg), NULL);
    }
    else
    {
        return;
    }
}

void pppoe_timer_stop(pppoe_port_info_t *pInfo)
{
    cs_timer_del(pInfo->timer_handler);
}

void pppoe_simul_timer_handler(void *data)
{   
    PPPOE_LOG_INFO("PPPoE simulation timer out.\n");

    pppoe_state_change(g_pppoe_info.port, PPPOE_STATE_INIT, TRUE, TRUE, PPPOE_TEST_RET_OK);
}

void pppoe_simul_timer_start()
{
    g_pppoe_info.timer_handler = cs_timer_add(PPPOE_RESTORE_INTERVAL, pppoe_simul_timer_handler, NULL);
}

void pppoe_simul_timer_stop()
{
    cs_timer_del(g_pppoe_info.timer_handler);
    g_pppoe_info.timer_handler = CS_INVALID_TIMER;
}

void pppoe_buf_dump(cs_uint8* buf, cs_uint32 buf_len)
{
    cs_uint32 i = 0;

    if(!buf || !g_pppoe_print)
    {
        return;
    }

    for(i =0; i < buf_len; i++)
    {
        if(i%16 == 0)
            cs_printf("\n");
        cs_printf("%02x ", buf[i]);
        
    }
    cs_printf("\n");
}

cs_status pppoe_tag_parse(
        pppoe_pkt_t *pkt,
        pppoe_parse_func_t func,
        void *extra)
{
    pppoe_tag_t *pTag= NULL;
    cs_uint32 len = 0;
    cs_uint8 * cursor = pkt->payload;
    
    if(pkt->type != PPPOE_TYPE || pkt->ver != PPPOE_VER)
    {
        return CS_E_ERROR;
    }

    pTag = (pppoe_tag_t*)pkt->payload;
    len = ntohs(pkt->length);

    while(cursor-pkt->payload < len)
    {
        
        pTag = (pppoe_tag_t*)cursor;
        if(ntohs(pTag->type) == TAG_END_OF_LIST)
        {
            return CS_E_OK;
        }
        if(func)
        {
            func(pTag, extra);
        }
        cursor += ntohs(pTag->len) + TAG_HDR_SIZE;
        
    }
    
    return CS_E_OK;
}

cs_status pppoe_plus_tag_info_gen(
        cs_port_id_t port, 
        cs_uint16 svlan,
        cs_uint16 cvlan,
        pppoe_plus_tag_t *ptag)
{
    pppoe_plus_subopt_t  *p_opt = NULL;
    cs_uint16         opt_len = 4; 

    p_opt = &ptag->pad[0];
    p_opt->opt = 0x1; // sub option 1

    snprintf(p_opt->pad, PPPOE_PLUS_SUB_MAX_PAD-1, 
            "0 0/0/0:0.0 0/0/0/0/0/0/%02x%02x.%02x%02x.%02x%02x/0/0/%d:eth/%d.%d EP",
            g_pppoe_info.my_mac.addr[0], g_pppoe_info.my_mac.addr[1],g_pppoe_info.my_mac.addr[2],
            g_pppoe_info.my_mac.addr[3], g_pppoe_info.my_mac.addr[4],g_pppoe_info.my_mac.addr[5],
            port, svlan ? svlan : 4096, cvlan ? cvlan : 4096);

    p_opt->pad_len = strlen((char *)p_opt->pad);
    opt_len += 2 + p_opt->pad_len;
    
    // Sub option 2
    p_opt = (pppoe_plus_subopt_t *)((cs_uint8 *)ptag->pad + p_opt->pad_len + 2);
    p_opt->opt = 0x2; 
    snprintf(p_opt->pad, strlen(PPPOE_ONU_NAME)+1, PPPOE_ONU_NAME);
    p_opt->pad_len = strlen(PPPOE_ONU_NAME);
    opt_len += 2 + p_opt->pad_len;

    // Create tag
    ptag->type   = htons(PPPOE_PLUS_TAG_VENDOR_ID);
    ptag->vendor = htonl(PPPOE_PLUS_VENDOR_ID);
    ptag->len = htons(opt_len);
    
    return CS_E_OK;
}

static cs_status pppoe_pkt_parser(cs_pkt_t *pPkt)
{
    if(pPkt->eth_type == EPON_ETHERTYPE_PPPOE_DISC)
    {
        pPkt->pkt_type = CS_PKT_PPPOE_DIS;
        return CS_E_OK;
    }

    if(pPkt->eth_type == EPON_ETHERTYPE_PPPOE_SESSION)
    {
        pPkt->pkt_type = CS_PKT_PPPOE_SES;
        return CS_E_OK;
    }
    
    return CS_E_NOT_SUPPORT;
}

void pppoe_state_change(
        cs_port_id_t port,
        cs_uint8 state,
        cs_boolean disc_over,
        cs_boolean simul_over,
        cs_uint8 code)
{
    pppoe_port_info_t *pInfo = &(g_pppoe_info.ports_info[port-1]);
    cs_uint8 old_state = 0;

    old_state = pInfo->state;

    /*if(state == old_state)
        return;*/
    
    pInfo->state = state;
    pInfo->disc_over = disc_over;
    pInfo->simul_over = simul_over;
    pInfo->code = code;

    PPPOE_LOG_INFO("State(%d) switch to state(%d)\n", old_state, state);

    if((pInfo->disc_over == TRUE) && (state == PPPOE_STATE_PADS)){
        pInfo->disc_rslt = PPPOE_TEST_OK;
    }

    if((pInfo->simul_over == TRUE) && (state == PPPOE_STATE_SESSION)){
        pInfo->simul_rslt = PPPOE_TEST_OK;
    }

    if(state == PPPOE_STATE_INIT)
    {
        //memset(&pInfo->conn, 0, sizeof(pppoe_connection_t));
        /*restore port loobback*/
        pppoe_port_lpbk_set_adapt(port, FALSE);

        if(g_pppoe_info.loop_status){
            /*Enable loopdetect*/
            loop_detect_enable(port);
        }
        
        /*set packet config*/
        pppoe_packet_config(PPPOE_PACKET_DOWN, PPPOE_PACKET_NOT_TO_CPU, FALSE);

        /*stop pppoe simulation timer*/
        pppoe_simul_timer_stop();
    }
    
}

cs_status pppoe_packet_send(
        cs_uint8 *frame,
        cs_uint32 len,
        cs_port_id_t port,
        cs_uint8 prio)
{
#if 0
/*Bug 36277*/
    pppoe_pkt_t pkt;

    if(g_pppoe_print)
    {
        cs_printf("send pppoe packet to port %d:\n", port);
        pppoe_buf_dump(frame, len);
    }
    
    memset(&pkt, 0, sizeof(pppoe_pkt_t));
    vlan_egress_handle( 0, port, frame,(cs_uint8*)&pkt,&len);

    if(g_pppoe_print)
    {
        cs_printf("send pppoe packet to port %d:\n", port);
        pppoe_buf_dump((cs_uint8*)&pkt, len);
    }
    return pppoe_packet_send_adapt((cs_uint8*)&pkt, len, port, ONU_CPU_PKT_PRIO_6);
/*Bug 36277*/
#endif
    if(g_pppoe_print)
    {
        cs_printf("send pppoe packet to port %d:\n", port);
        pppoe_buf_dump(frame, len);
    }
    return pppoe_packet_send_adapt(frame, len, port, ONU_CPU_PKT_PRIO_6);
}


void pppoe_discovery_client_pkt_handle(cs_pkt_t* hdr)
{
    pppoe_pkt_t *pPkt = NULL;
    pppoe_plus_tag_t plus_tag;
    cs_status ret = CS_E_OK;
    cs_uint8 *new_pkt = NULL;
    cs_pkt_t* new_hdr = NULL; 
    cs_uint32 pad_len = 0;
    cs_uint32 copy_len = 0;
   

    pPkt = (pppoe_pkt_t*)(hdr->data+hdr->offset + sizeof(app_vlan_header_t)*hdr->tag_num);

    if(pPkt->code != PPPOE_CODE_PADI && pPkt->code != PPPOE_CODE_PADR)
    {
        PPPOE_LOG_INFO("Not discovery packet\n");
        goto FORWARD_PACKET;
    }

    memset(&plus_tag, 0 , sizeof(pppoe_plus_tag_t));
    
    ret = pppoe_plus_tag_info_gen(hdr->port, 0, hdr->svlan, &plus_tag);
    if(CS_E_OK != ret)
    {
        PPPOE_LOG("PPPoE plus tag info fail\n");
        goto FORWARD_PACKET;
    }

    /*check the new packet length*/
    if(ntohs(pPkt->length) + TAG_HDR_SIZE + ntohs(plus_tag.len) > PPPOE_MAX_PAYLOAD)
    {
        /*the packet is over flow*/
        PPPOE_LOG("Too long packet\n");
        /* Format: PPPoE header | AC_NAME_tag | HOST_UINQ_tag | Generic Error Tag */
        goto FORWARD_PACKET;;
    }

    pad_len = TAG_HDR_SIZE + ntohs(plus_tag.len);
    new_pkt = iros_malloc(IROS_MID_PPPOE, hdr->len+pad_len+hdr->offset+sizeof(cs_pkt_t));
    if(new_pkt == NULL)
    {
        PPPOE_LOG("malloc fail\n");
        goto FORWARD_PACKET;
    }
    new_hdr= (cs_pkt_t*)new_pkt;

    copy_len = sizeof(pppoe_pkt_t)-PPPOE_MAX_PAYLOAD+ ntohs(pPkt->length) + hdr->tag_num*sizeof(app_vlan_header_t);
    pPkt->length = htons(ntohs(pPkt->length)+pad_len);
    memcpy(new_hdr, hdr, sizeof(cs_pkt_t));
    memcpy(new_hdr->data+new_hdr->offset, hdr->data+hdr->offset, copy_len);
    memcpy(new_hdr->data+new_hdr->offset+copy_len, &plus_tag, pad_len);
    memcpy(new_hdr->data+new_hdr->offset+copy_len+pad_len, hdr->data+hdr->offset+copy_len, hdr->len-copy_len);
    new_hdr->len += pad_len;
    if(CS_E_OK == vlan_ingress_filter(new_hdr))
        pppoe_packet_send(new_hdr->data+new_hdr->offset, new_hdr->len, CS_PON_PORT_ID, 7);
    iros_free(new_pkt);
    return;
 
FORWARD_PACKET:
    if(CS_E_OK == vlan_ingress_filter(hdr))
        pppoe_packet_send(hdr->data+hdr->offset, hdr->len, CS_PON_PORT_ID, 7);
    return;
}

void pppoe_host_uniq_parse(
        pppoe_tag_t *pTag,
        void *extra)
{
    cs_port_id_t port = 0;
    if(ntohs(pTag->type) == TAG_HOST_UNIQ)
    {
        port = pTag->payload[6];
        memcpy(extra, &port, sizeof(cs_port_id_t));
    }
}

    void pppoe_tag_type_parse(
        pppoe_tag_t *pTag,
        void *extra)
{
    pppoe_connection_t *pConn = (pppoe_connection_t*)extra;
    cs_uint16 type = ntohs(pTag->type);
    pppoe_tag_t *pOut;
    PPPOE_TAG_TYPE_E index = 0;
    cs_uint16 copy_len = 0;

    switch(type)
    {
        case TAG_AC_NAME:
            index = PPPOE_TAG_AC_NAME;
            break;
            
        case TAG_SERVICE_NAME:
            index = PPPOE_TAG_SERV_NAME;
            break;
            
        case TAG_AC_COOKIE:
            index = PPPOE_TAG_COOKIE;
            break;
            
        case TAG_RELAY_SESSION_ID:
            index = PPPOE_TAG_RELAY_ID;
            break;

        case TAG_GENERIC_ERROR:
        case TAG_AC_SYSTEM_ERROR:
        case TAG_SERVICE_NAME_ERROR:
            pConn->pads_error = 1;
            return;
            
        default:
            return;
    }

    pOut = &pConn->tags[index];
    copy_len = ntohs(pTag->len) > PPPOE_TAG_MAX_LEN ? PPPOE_TAG_MAX_LEN : ntohs(pTag->len);
    pOut->type = ntohs(pTag->type);
    pOut->len = copy_len;   
    memcpy(pOut->payload, pTag->payload, copy_len);
    return;
}

void pppoe_next_host_uniq_gen(
        cs_port_id_t port,
        cs_uint8* data,
        cs_uint16 *out_len)
{
    g_pppoe_info.ports_info[port-1].test_id = (g_pppoe_info.ports_info[port-1].test_id+1) % 255;

    memcpy(data, g_pppoe_info.my_mac.addr, CS_MACADDR_LEN);
    data[6] = port;
    data[7] = g_pppoe_info.ports_info[port-1].test_id;
    *out_len = PPPOE_HOSTID_LEN;
}

cs_status pppoe_discovery_packet_send(
        cs_uint8 type,
        pppoe_connection_t *conn)
{
    pppoe_pkt_t pkt;
    pppoe_pkt_t *pPkt = &pkt;
    cs_uint8 bc_mac[CS_MACADDR_LEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    cs_uint32 payload_len = 0;
    app_vlan_header_t vlan_header;
    pppoe_tag_t *p_srvtag, *p_hostid;
    cs_uint8 *cursor;
    cs_uint32 len = 0;
    cs_status ret = CS_E_OK;

    memset(&pkt, 0, sizeof(pppoe_pkt_t));
    memcpy(pkt.eth_hdr.dst.addr, bc_mac, CS_MACADDR_LEN);
    memcpy(pkt.eth_hdr.src.addr, g_pppoe_info.my_mac.addr, CS_MACADDR_LEN);
    if(conn->vlan)
    {
        vlan_header.tpid = htons(0x8100);
        vlan_header.vlan = htons(conn->vlan);
        memcpy(&pkt.eth_hdr.ethertype, &vlan_header, sizeof(app_vlan_header_t));
        pPkt = (pppoe_pkt_t*)((cs_uint8*)pPkt + sizeof(app_vlan_header_t));
    }
    pPkt->eth_hdr.ethertype = htons(EPON_ETHERTYPE_PPPOE_DISC);
    pPkt->type = PPPOE_TYPE;
    pPkt->ver = PPPOE_VER;
    pPkt->session = 0;
    cursor = pPkt->payload;
    if(type == PPPOE_DISCOVERY_PADI)
    {
        pPkt->code = PPPOE_CODE_PADI;
        p_srvtag = (pppoe_tag_t*)pPkt->payload;
        p_srvtag->type  = htons(TAG_SERVICE_NAME);
        p_srvtag->len = 0;
        payload_len = 2*sizeof(cs_uint16);
        cursor = pPkt->payload + payload_len;
        
    }
    else if(type == PPPOE_DISCOVERY_PADR)
    {
        memcpy(pkt.eth_hdr.dst.addr, conn->peer_mac.addr, CS_MACADDR_LEN);
        pPkt->code = PPPOE_CODE_PADR;
        if (conn->tags[PPPOE_TAG_SERV_NAME].type == TAG_SERVICE_NAME ) 
        {
        
            p_srvtag = (pppoe_tag_t*)pPkt->payload;
            p_srvtag->type  = htons(TAG_SERVICE_NAME);
            p_srvtag->len = htons((conn->tags[PPPOE_TAG_SERV_NAME].len));
            memcpy(p_srvtag->payload, conn->tags[PPPOE_TAG_SERV_NAME].payload, ntohs(p_srvtag->len));
            payload_len = TAG_HDR_SIZE+conn->tags[PPPOE_TAG_SERV_NAME].len;
            cursor = pPkt->payload + payload_len;
        }
    }
    else if(type == PPPOE_DISCOVERY_PADT)
    {
        memcpy(pkt.eth_hdr.dst.addr, conn->peer_mac.addr, CS_MACADDR_LEN);
        pPkt->code = PPPOE_CODE_PADT;
        pPkt->length = 0;
        pPkt->session = htons(conn->session);
        goto SEND_PACKET;
    }
    else
    {
        return CS_E_ERROR;
    }

    if(conn->host_uniq_tag)
    {
        cs_uint16 hostid_len = 0;
        p_hostid = (pppoe_tag_t*)cursor;
        p_hostid->type   = htons(TAG_HOST_UNIQ);       
        pppoe_next_host_uniq_gen(conn->port, p_hostid->payload, &hostid_len);
        p_hostid->len = htons(hostid_len);
        payload_len += TAG_HDR_SIZE + hostid_len;
        cursor +=  TAG_HDR_SIZE + hostid_len;
    }
    if(conn->tags[PPPOE_TAG_COOKIE].type == TAG_AC_COOKIE)
    {
        pppoe_tag_t *p_cookie;
        p_cookie = (pppoe_tag_t*)cursor;
        p_cookie->type   = htons(TAG_AC_COOKIE);
        p_cookie->len = htons(conn->tags[PPPOE_TAG_COOKIE].len);
        memcpy(p_cookie->payload, conn->tags[PPPOE_TAG_COOKIE].payload, ntohs(p_cookie->len));
        payload_len += TAG_HDR_SIZE + ntohs(p_cookie->len);
        cursor += TAG_HDR_SIZE + ntohs(p_cookie->len);
    }
    if(conn->tags[PPPOE_TAG_RELAY_ID].type == TAG_RELAY_SESSION_ID)
    {
        pppoe_tag_t *p_relay;
        p_relay = (pppoe_tag_t*)cursor;
        p_relay->type   = htons(TAG_RELAY_SESSION_ID);
        p_relay->len = htons(conn->tags[PPPOE_TAG_RELAY_ID].len);
        memcpy(p_relay->payload, conn->tags[PPPOE_TAG_RELAY_ID].payload, ntohs(p_relay->len));
        payload_len += TAG_HDR_SIZE + ntohs(p_relay->len);
        cursor += TAG_HDR_SIZE + ntohs(p_relay->len);
    }
    pPkt->length = htons(payload_len);

SEND_PACKET:
    if(payload_len > PPPOE_MAX_PAYLOAD)
    {
        PPPOE_LOG("wrong packet len %d\n", payload_len);
        return CS_E_ERROR;
    }
    len = sizeof(pppoe_pkt_t) - PPPOE_MAX_PAYLOAD + ntohs(pPkt->length);
    if(conn->vlan)
    {
        len += sizeof(app_vlan_header_t);
    }

    ret = pppoe_packet_send((cs_uint8*)&pkt, len, conn->port, 7);
    if(ret != CS_E_OK)
    {
        PPPOE_LOG("send packet fail\n");
    }
                            
    return ret;
}


void pppoe_discovery_AC_pkt_handle(cs_pkt_t* hdr)
{
    pppoe_pkt_t *pPkt;
    cs_port_id_t port = 0;
    pppoe_port_info_t *pInfo = NULL;
    
    pPkt = (pppoe_pkt_t*)(hdr->data+hdr->offset + (hdr->tag_num)*sizeof(app_vlan_header_t));

    if((pPkt->code != PPPOE_CODE_PADO) && (pPkt->code != PPPOE_CODE_PADS) && (pPkt->code != PPPOE_CODE_PADT))
    {
        PPPOE_LOG("Should not receive this type:%d\n", pPkt->code);
        return;
    }

    pInfo = &g_pppoe_info.ports_info[g_pppoe_info.port-1];

    if(pPkt->code == PPPOE_CODE_PADT){
        PPPOE_LOG_INFO("Receive PADT.\n");
        pInfo->padt_recv = TRUE;
        pppoe_discovery_packet_send(PPPOE_DISCOVERY_PADT, &pInfo->conn);
        return;
    }
    
    pppoe_tag_parse(pPkt, pppoe_host_uniq_parse, &port);
    
    if(!PPPOE_IS_UNI_PORT(port))
    {
        PPPOE_LOG("parse packet fail\n");
        return;
    }

    if(pPkt->code == PPPOE_CODE_PADO)
    {
        if(pInfo->state != PPPOE_STATE_PADI)
        {
            PPPOE_LOG("Receive PADO in sate %d\n", pInfo->state);
            return;
        }
        pppoe_tag_parse(pPkt, pppoe_tag_type_parse, &pInfo->conn);

        /*Check parse result*/
        if(pInfo->conn.tags[PPPOE_TAG_AC_NAME].type != TAG_AC_NAME)
        {
            PPPOE_LOG("Without acname\n");
            return;
        }
        if(pInfo->conn.tags[PPPOE_TAG_SERV_NAME].type != TAG_SERVICE_NAME)
        {
            PPPOE_LOG("Without service name\n");
            return;
        }

        /*delete the timer and goto next state*/
        pppoe_timer_stop(pInfo);
        pppoe_state_change(pInfo->port, PPPOE_STATE_PADO, FALSE, FALSE, PPPOE_TEST_RET_OK);

        memcpy(pInfo->conn.peer_mac.addr, hdr->data+hdr->offset+CS_MACADDR_LEN, CS_MACADDR_LEN);

        if(CS_E_OK == pppoe_discovery_packet_send(PPPOE_DISCOVERY_PADR, &pInfo->conn))
        {
            pppoe_state_change(pInfo->port, PPPOE_STATE_PADR, FALSE, FALSE, PPPOE_TEST_RET_OK);
            pppoe_timer_start(PPPOE_TIMER_PADR, pInfo);
        }
        else
        {
            pppoe_state_change(pInfo->port, PPPOE_STATE_INIT, TRUE, FALSE, PPPOE_TEST_RET_SDL_ERR);
            pppoe_test_result_set(pInfo->port, PPPOE_TEST_FAIL);
        }
    }
    else if(pPkt->code == PPPOE_CODE_PADS)
    {
        if(pInfo->state != PPPOE_STATE_PADR)
        {
            PPPOE_LOG("Receive PADS in sate %d\n", pInfo->state);
            return;
        }
        pppoe_tag_parse(pPkt, pppoe_tag_type_parse, &pInfo->conn);
        if(pInfo->conn.pads_error)
        {
            PPPOE_LOG("PADS has error tag\n");
        }
        else
        {
            pInfo->conn.session = ntohs(pPkt->session);
            pppoe_state_change(pInfo->port, PPPOE_STATE_PADS, TRUE, FALSE, PPPOE_TEST_RET_OK);
            pppoe_session_start();
        }
        pppoe_timer_stop(pInfo);
    }
    
}

cs_boolean pppoe_discovery_completed(
    cs_port_id_t *port)
{
    cs_uint8 i = 0;
    
    for(i=0; i<g_pppoe_info.max_port; i++)
    {
        if(g_pppoe_info.ports_info[i].disc_rslt == PPPOE_TEST_OK)
        {
            *port = i+1;
            return TRUE;
        }
    }

    return FALSE;
}

cs_uint8 pppoe_pkt_recv(cs_pkt_t* hdr)
{

    if(g_pppoe_print)
    {
        cs_printf("recv pppoe packet from port %d:\n", hdr->port);
        pppoe_buf_dump(hdr->data+hdr->offset, hdr->len);
    }

    /*handle pppoe packet*/
    if(PPPOE_IS_UNI_PORT(hdr->port))
    {
        if(hdr->eth_type == EPON_ETHERTYPE_PPPOE_DISC){
            pppoe_discovery_client_pkt_handle(hdr);
        }
        else{
            /*just forward pppoe session packets from UNI ports*/
            pppoe_packet_send(hdr->data+hdr->offset, hdr->len, CS_PON_PORT_ID, 7);
            return 1;
        }
    }
    else if(hdr->port == CS_PON_PORT_ID)
    {
        if(memcmp(&g_pppoe_info.my_mac, hdr->data+hdr->offset, CS_MACADDR_LEN))
        {
            /*not our address, forward*/
            pppoe_packet_send(hdr->data+hdr->offset, hdr->len, CS_ALL_UNI_PORT_ID, 7);
        }
        else
        {
            if(hdr->eth_type == EPON_ETHERTYPE_PPPOE_DISC){
                /*handle discovery packet from PON port*/
                pppoe_discovery_AC_pkt_handle(hdr);
            }
            else if(hdr->eth_type == EPON_ETHERTYPE_PPPOE_SESSION){
                /*handle session packet from PON port*/
                pppoe_session_pkt_recv(hdr);
            }
        }
    }
    return 1;
}

void pppoe_timer_handle(app_timer_msg_t *msg)
{
    pppoe_port_info_t *pInfo = (pppoe_port_info_t*)msg;
    cs_uint8 code = 0;


    PPPOE_LOG("pppoe Time out %d\n", pInfo->state);

    if(pInfo->state == PPPOE_STATE_PADI) 
    {
        code = PPPOE_TEST_RET_PADI_TIMEOUT;
    }
    else if(pInfo->state == PPPOE_STATE_PADR)
    {
        code = PPPOE_TEST_RET_PADR_TIMEOUT;
    }
    else
    {
        return;
    }

    pppoe_state_change(pInfo->port, PPPOE_STATE_INIT, TRUE, FALSE, code);
    pppoe_test_result_set(pInfo->port, PPPOE_TEST_FAIL);
    
}

void app_pppoe_queue_create()
{
    cs_int32 ret = 0;
    
    ret = cs_queue_create(&g_pppoe_msgQ, PPPOE_QUEUE_NAME, PPPOE_QUEUE_DEPTH, PPPOE_QUEUE_SIZE, 0);
    if(ret != CS_E_OSAL_OK) {
        IROS_LOG_MSG(IROS_LOG_LEVEL_CRI, IROS_MID_APP, 
                               "PPPoE MsgQ create failed, 0x%x\n", ret);
    }
}

void pppoe_init()
{
    cs_uint32 len = 0;
    
    memset(&g_pppoe_info, 0, sizeof(g_pppoe_info));
    
    if(CS_E_OK != startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &(g_pppoe_info.max_port)))
    {
        g_pppoe_info.max_port = 1;
        PPPOE_LOG("read switch num fail\n");
    }
    
    len = sizeof(pppoe_port_info_t)*g_pppoe_info.max_port;

    g_pppoe_info.ports_info = iros_malloc(IROS_MID_PPPOE, len);
    if(!(g_pppoe_info.ports_info))
    {
        PPPOE_LOG("pppoe_init malloc fail\n");
        return;
    }
    memset(g_pppoe_info.ports_info, 0, len);
    pppoe_pon_mac_addr_get_adapt(&g_pppoe_info.my_mac);

    /*reg packet handler*/
    app_pkt_reg_parser(CS_PKT_PPPOE_DIS, pppoe_pkt_parser);
    app_pkt_reg_handler(CS_PKT_PPPOE_DIS, pppoe_pkt_recv);
    
    app_pkt_reg_parser(CS_PKT_PPPOE_SES, pppoe_pkt_parser);
    app_pkt_reg_handler(CS_PKT_PPPOE_SES, pppoe_pkt_recv);
    
#ifdef HAVE_PPP
    ppp_start();
#endif
}

void pppoe_simulation_init(cs_port_id_t port)
{
    pppoe_port_info_t *pInfo = NULL;
    
    pInfo = &g_pppoe_info.ports_info[port-1];
    memset(g_pppoe_info.ports_info, 0, sizeof(pppoe_port_info_t)*g_pppoe_info.max_port);
    pInfo->start_time = cs_current_time();

    pppoe_simul_timer_start();
}

cs_status pppoe_discovery_test_result_get(
        cs_port_id_t port,
        cs_uint8* status,
        cs_uint8* code)
{
    pppoe_port_info_t *pInfo = NULL;
    
    if(!PPPOE_IS_UNI_PORT(port) || !status || !code)
    {
        return CS_E_PARAM;
    }

    pInfo = &g_pppoe_info.ports_info[port-1];
    if(pInfo->disc_over)
    {
        *status = (pInfo->code==PPPOE_TEST_RET_OK?PPPOE_TEST_OK:PPPOE_TEST_FAIL);
    }
    else
    {
        if(pInfo->state == PPPOE_STATE_INIT)
        {
            *status = PPPOE_TEST_NOT_RUN;
        }
        else
        {
            *status = PPPOE_TEST_RUNNING;
        }
    }

    *code = pInfo->code;
    return CS_E_OK;
}

cs_boolean pppoe_discovery_test_started(
    cs_port_id_t *port)
{
    cs_uint8 i = 0;

    for(i=0; i<g_pppoe_info.max_port; i++)
    {
        if(g_pppoe_info.ports_info[i].state != PPPOE_STATE_INIT)
        {
            *port = i + 1;
            return TRUE;
        }
    }

    return FALSE;
}

void pppoe_test_result_set(
    cs_port_id_t port,
    cs_uint8 result)
{
    pppoe_port_info_t *pInfo = NULL;
    pInfo = &g_pppoe_info.ports_info[port-1];
    
    pInfo->disc_rslt = result;
    pInfo->simul_rslt = result;
}

cs_status pppoe_discovery_test_start_handle(
        cs_port_id_t port,
        cs_uint16 vlan)
{
    cs_status ret = CS_E_OK;
    pppoe_port_info_t *pInfo = NULL;
    pppoe_connection_t *pConn = NULL;
    cs_port_id_t test_port = 0;

    /*Get loopdetect status*/
    ret = loop_detect_oper_get(port, &g_pppoe_info.loop_status);
    if(CS_E_OK != ret)
    {
        PPPOE_LOG("loop_detect get status fail\n");
        pppoe_state_change(port, PPPOE_STATE_INIT, TRUE, FALSE, PPPOE_TEST_RET_SDL_ERR);
        return CS_E_ERROR;
    }
        
    /*Disable loopdetect*/
    ret = loop_detect_disable(port);
    if(CS_E_OK != ret)
    {
        PPPOE_LOG("loop_detect_disable fail\n");
        pppoe_state_change(port, PPPOE_STATE_INIT, TRUE, FALSE, PPPOE_TEST_RET_SDL_ERR);
        return CS_E_ERROR;
    }
    /*unblock ports*/
    ret = loop_detect_unblock(port);
    if(CS_E_OK != ret)
    {
        PPPOE_LOG("loop_detect_unblock fail\n");
        pppoe_state_change(port, PPPOE_STATE_INIT, TRUE, FALSE, PPPOE_TEST_RET_SDL_ERR);
        return CS_E_ERROR;
    }
    
    /*set loopback*/
    ret = pppoe_port_lpbk_set_adapt(port, TRUE);
    if(CS_E_OK != ret)
    {
        PPPOE_LOG("epon_request_onu_port_lpbk_set fail\n");
        pppoe_state_change(port, PPPOE_STATE_INIT, TRUE, FALSE, PPPOE_TEST_RET_SDL_ERR);
        return CS_E_ERROR;
    }

    pInfo = &(g_pppoe_info.ports_info[port-1]);
    pInfo->port = port;
    pInfo->state = PPPOE_STATE_INIT;
    
    pConn = (pppoe_connection_t*)(&pInfo->conn);
    pConn->vlan = vlan&0xfff;
    pConn->port = port;
    pConn->host_uniq_tag = TRUE;
    
    /*check pppoe test is started*/
    if(!pppoe_discovery_test_started(&test_port))
    {
        ret = pppoe_packet_config(PPPOE_PACKET_DOWN, PPPOE_PACKET_TO_CPU, FALSE);
        if(CS_E_OK != ret)
        {
            PPPOE_LOG("pppoe_packet_config fail\n");
            pppoe_state_change(port, PPPOE_STATE_INIT, TRUE, FALSE, PPPOE_TEST_RET_SDL_ERR);
            return CS_E_ERROR;
        }
    }

    /*send padi*/
    /*delay 100ms for the port is operating*/
    cs_thread_delay(100);
    ret = pppoe_discovery_packet_send(PPPOE_DISCOVERY_PADI, pConn);
    if(CS_E_OK == ret)
    {
        pppoe_test_result_set(pInfo->port, PPPOE_TEST_RUNNING);
        pppoe_state_change(pInfo->port, PPPOE_STATE_PADI, FALSE, FALSE, PPPOE_TEST_RET_OK);
        /*add timer to monitor*/
        pppoe_timer_start(PPPOE_TIMER_PADI, pInfo);
    }
    else
    {
        pppoe_state_change(pInfo->port, PPPOE_STATE_INIT, TRUE, FALSE, PPPOE_TEST_RET_SDL_ERR);
    }
    
    return CS_E_OK;
}

cs_status pppoe_discovery_test_stop_handle(
        cs_port_id_t port)
{
    pppoe_port_info_t *pInfo = NULL;

    pInfo = &g_pppoe_info.ports_info[port-1];

    pppoe_timer_stop(pInfo);
    pppoe_state_change(port, PPPOE_STATE_INIT, FALSE, FALSE, pInfo->code);
    
    return CS_E_OK;
}

cs_status pppoe_discovery_test_start(
        cs_port_id_t port,
        cs_uint16 vlan)
{
    cs_boolean valid = FALSE;
    cs_port_id_t test_port = 0;

    if(!PPPOE_IS_UNI_PORT(port))
    {
        PPPOE_LOG("port(%d) not valid\n",port);
        return CS_E_PARAM;
    }

    if(pppoe_discovery_test_started(&test_port))
    {
        PPPOE_LOG("Test on port[%d] is already running\n", test_port);
        return CS_E_PARAM;
    }

    /*pppoe_port_state_get_adapt(port, &state);
    if(state == STP_S_BLOCKING)
    {
        PPPOE_LOG("Port[%d] state is Blocking.\n", port);
        return CS_E_PARAM;
    }*/

    pppoe_simulation_init(port);
    g_pppoe_info.port = port;

    /*check vlan*/
    if(vlan != 0)
    {
        valid = vlan_vid_port_check(port, vlan);
        if(!valid)
        {   
            PPPOE_LOG("Vlan(%d) on port(%d) is invalid.\n", vlan, port);
            pppoe_state_change(port, PPPOE_STATE_INIT, TRUE, FALSE, PPPOE_TEST_RET_VLAN_NOT_EXIST);
            return CS_E_ERROR;
        }
    }

    /*start test for the port*/
    return pppoe_discovery_test_start_handle(port, vlan);
}

cs_status pppoe_discovery_test_stop(
        cs_port_id_t port)
{
    pppoe_port_info_t *pInfo = NULL;
    if(!PPPOE_IS_UNI_PORT(port))
    {
        PPPOE_LOG_INFO("port(%d) not valid\n",port);
        return CS_E_PARAM;
    }

    pInfo = &(g_pppoe_info.ports_info[port-1]);

    if(pInfo->state == PPPOE_STATE_INIT)
    {
        PPPOE_LOG_INFO("Port(%d) test is already stopped\n", port);
        return CS_E_OK;
    }

    /*stop test for the port*/
    return pppoe_discovery_test_stop_handle(port);
}

cs_status pppoe_plus_enable(cs_boolean enable)
{
    cs_uint8 action = 0;
    
    if(enable != g_pppoe_info.plus_enable)
    {
        g_pppoe_info.plus_enable = enable;
        if(enable)
        {
            action = PPPOE_PACKET_TO_CPU;
        }
        else
        {
            action = PPPOE_PACKET_NOT_TO_CPU;
        }
        pppoe_packet_config(PPPOE_PACKET_UP, action, TRUE);
    }
    else
    {
        PPPOE_LOG_DEBUG("PPPoE plus is already %s", enable ? "enable":"disable");
    }
    return CS_E_OK;
}

cs_boolean pppoe_plus_status_get()
{
    return g_pppoe_info.plus_enable;
}

cs_uint8 pppoe_state_get(cs_port_id_t port)
{
    if(!PPPOE_IS_UNI_PORT(port))
    {
        PPPOE_LOG("port(%d) not valid\n",port);
        return PPPOE_STATE_INVALID;
    }

    return (g_pppoe_info.ports_info[port-1].state);
}

void pppoe_cmd(
        cs_uint32 para1,
        cs_uint32 para2,
        cs_uint32 para3,
        cs_uint32 para4)
{
    const cs_int8 *state_str[] = {
        "INIT", "SEND_PADI", "RCVD_PADO", "SEND_PADR", "RCVD_PADS"
    };  

    const cs_int8 *result_str[] = {
        "OK ",
        "UNK_ERR ",
        "NOT_SUPPORT",
        "VLAN_NOT_EXIST",
        "PADI_TIMEOUT",
        "PADR_TIMEOUT",
        "LCP_NEG_FAIL",
        "AUTH_FAIL",
        "IP_NEG_FAIL"
    };

    const cs_int8 *status_str[]= {
        "NOT_RUN", "RUNNING", "TEST_OK","TEST_FAIL"
    };

    cs_uint8 i = 0;
    pppoe_port_info_t *pInfo = NULL;
    pppoe_connection_t *pConn = NULL;
    
    if(para1 == 1)
    {
        /*show the info*/
        cs_printf("addr is      :%p\n", &g_pppoe_info);
        cs_printf("plus enable:%d\n", g_pppoe_info.plus_enable);
        cs_printf("print enable:%d\n", g_pppoe_print);
        cs_printf("max port   :%d\n", g_pppoe_info.max_port);
        cs_printf("loop detect status   :%d\n", g_pppoe_info.loop_status);
        cs_printf("my mac:%x-%x-%x-%x-%x-%x\n", 
            g_pppoe_info.my_mac.addr[0],
            g_pppoe_info.my_mac.addr[1],
            g_pppoe_info.my_mac.addr[2],
            g_pppoe_info.my_mac.addr[3],
            g_pppoe_info.my_mac.addr[4],
            g_pppoe_info.my_mac.addr[5]);

            for(i = 0; i< g_pppoe_info.max_port; i++){
                cs_printf("**********port(%d) info **********\n" , i+1);
                pInfo = &g_pppoe_info.ports_info[i];
                pConn = &pInfo->conn;
                cs_printf("peer mac:%x-%x-%x-%x-%x-%x\n", 
                    pConn->peer_mac.addr[0],
                    pConn->peer_mac.addr[1],
                    pConn->peer_mac.addr[2],
                    pConn->peer_mac.addr[3],
                    pConn->peer_mac.addr[4],
                    pConn->peer_mac.addr[5]);
                cs_printf("state         :%s\n", (pInfo->state)>4 ? "Unkown" : state_str[pInfo->state]);
                cs_printf("timer         :%d\n", pInfo->timer_handler);
                cs_printf("discovery    :%d\n", pInfo->disc_over);
                cs_printf("id              :%d\n", pInfo->test_id);
                cs_printf("code          :%s\n", (pInfo->code)>8 ? "Unkown" : result_str[pInfo->code]);
                cs_printf("session       :%d\n", pConn->session);
                cs_printf("pads err      :%d\n", pConn->pads_error);
                cs_printf("ac name      :%s\n", pConn->tags[PPPOE_TAG_AC_NAME].payload);
                cs_printf("serv name   :%s\n", pConn->tags[PPPOE_TAG_SERV_NAME].payload);
        }
    }
    else if(para1 == 2)
    {
        /*start test*/
        pppoe_discovery_test_start(para2, para3);
    }
    else if(para1 == 3)
    {
        /*stop test*/
        pppoe_discovery_test_stop(para2);
    }
    else if(para1 == 4)
    {
        /*show discovery test result*/
        cs_uint8 status=0, code = 0;
        pppoe_discovery_test_result_get(para2, &status, &code);
        cs_printf("status:%s", status>3 ? "Unk"  : status_str[status]);
        cs_printf("code:%s", code>8 ? "Unk"  : result_str[code]);   
    }
    else if(para1 == 5)
    {
        /*enable plus*/
        pppoe_plus_enable(para2);
    }
    else if(para1 == 6)
    {
        if(g_pppoe_print)
        {
            g_pppoe_print = FALSE;
        }
        else
        {
            g_pppoe_print = TRUE;
        }
    }
    else if(para1 == 8)
    {
        /*stop pppoe simulation test*/
        pppoe_simulation_stop(para2);
    }
    else if(para1 == 9)
    {
        /*send lcp conf req to ppp*/
		cs_pkt_t cs_pkt;
		cs_uint8 frame[] = {0x00, 0x13, 0x25, 0x00, 0x00, 0x01,
		   0x00, 0x19, 0x25, 0x00, 0x00, 0x01, 0x88, 0x64, 0x11,
		   0x00, 0x00, 0x1f, 0x00, 0x10, 0xc0, 0x21, 0x01, 0x01, 
		   0x00, 0x0e, 0x01, 0x04, 0x05, 0xba, 0x05, 0x06, 0x14,
		   0x49, 0x1f, 0x9e};
		cs_pkt_t *pPkt = &cs_pkt;

		memset(&cs_pkt, 0, sizeof(cs_pkt_t));
		pPkt->port = 0;
		pPkt->offset = 0;
		pPkt->len = 64;
		pPkt->eth_type = 0x8864;
		
		memcpy(pPkt->data, frame, sizeof(frame));
		pppoe_pkt_recv(pPkt);
    }
    else if(para1 == 10)
    {
        /*send lcp conf ack to ppp*/
		cs_pkt_t cs_pkt;
		cs_uint8 frame[] = {0x00, 0x13, 0x25, 0x00, 0x00, 0x01,
		   0x00, 0x19, 0x25, 0x00, 0x00, 0x01, 0x88, 0x64, 0x11,
		   0x00, 0x00, 0x1f, 0x00, 0x16, 0xc0, 0x21, 0x02, 0x01, 
		   0x00, 0x14, 0x02, 0x06, 0x00, 0x00, 0x00, 0x00, 0x05,
		   0x06, 0x28, 0x5a, 0x40, 0x69, 0x07, 0x02, 0x08, 0x02};
		cs_pkt_t *pPkt = &cs_pkt;

		memset(&cs_pkt, 0, sizeof(cs_pkt_t));
		pPkt->port = 0;
		pPkt->offset = 0;
		pPkt->len = 64;
		pPkt->eth_type = 0x8864;

		memcpy(pPkt->data, frame, sizeof(frame));

		pppoe_pkt_recv(pPkt);
    }
    else if(para1 == 11)
    {
        /*send ipcp conf req to ppp*/
		cs_pkt_t cs_pkt;
		cs_uint8 frame[] = {0x00, 0x13, 0x25, 0x00, 0x00, 0x01,
		   0x00, 0x19, 0x25, 0x00, 0x00, 0x01, 0x88, 0x64, 0x11,
		   0x00, 0x00, 0x1f, 0x00, 0x0c, 0x80, 0x21, 0x01, 0x01, 
		   0x00, 0x0a, 0x03, 0x06, 0x0a, 0x00, 0x00, 0x01};
		cs_pkt_t *pPkt = &cs_pkt;

		memset(&cs_pkt, 0, sizeof(cs_pkt_t));
		pPkt->port = 0;
		pPkt->offset = 0;
		pPkt->len = 64;
		pPkt->eth_type = 0x8864;
		
		memcpy(pPkt->data, frame, sizeof(frame));
		pppoe_pkt_recv(pPkt);
    }
    else if(para1 == 12)
    {
        /*send ipcp conf ack to ppp*/
		cs_pkt_t cs_pkt;
		cs_uint8 frame[] = {0x00, 0x13, 0x25, 0x00, 0x00, 0x01,
		   0x00, 0x19, 0x25, 0x00, 0x00, 0x01, 0x88, 0x64, 0x11,
		   0x00, 0x00, 0x1f, 0x00, 0x18, 0x80, 0x21, 0x02, 0x01, 
		   0x00, 0x16, 0x03, 0x06, 0x0a, 0x00, 0x00, 0x05, 0x81,
		   0x06, 0x00, 0x00, 0x00, 0x00, 0x83, 0x06, 0x00, 0x00,
		   0x00, 0x00};
		cs_pkt_t *pPkt = &cs_pkt;

		memset(&cs_pkt, 0, sizeof(cs_pkt_t));
		pPkt->port = 0;
		pPkt->offset = 0;
		pPkt->len = 64;
		pPkt->eth_type = 0x8864;
		
		memcpy(pPkt->data, frame, sizeof(frame));
		pppoe_pkt_recv(pPkt);
    }
    else if(para1 == 14)
    {
        /*send pppoe discovery packets to server*/
		cs_uint8 frame[] = {0x00, 0x10, 0x25, 0x00, 0x02, 0x01,
		   0x00, 0x13, 0x25, 0x00, 0x00, 0x88, 0x88, 0x63, 0x11,
		   0x07, 0x00, 0x1f, 0x00, 0x18, 0x80, 0x21, 0x02, 0x01, 
		   0x00, 0x16, 0x03, 0x06, 0x0a, 0x00, 0x00, 0x05, 0x81,
		   0x06, 0x00, 0x00, 0x00, 0x00, 0x83, 0x06, 0x00, 0x00,
		   0x00, 0x00};

        pppoe_packet_send(frame, sizeof(frame), 1, 7);
    }
    
}


#endif

