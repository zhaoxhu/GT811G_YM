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

#ifdef HAVE_ONU_HELLO
#define CS_GLOBAL_PARAM
#include "iros_config.h"
#include "cs_types.h"
#include "plat_common.h"
#include "osal_api_packet.h"
#include "cs_timer.h"
#include "app_timer.h"
#include "sdl_api.h"
#include "cs_msg.h"
#include "cs_hello.h"
#include "onu_hello.h"
#include "app_oam_cs.h"

onu_node_t onu_node;

cs_port_id_t active_hello_port = INVALID_PORT_ID;
app_timer_msg_t g_hello_timer;
extern cs_uint32 app_msg_q;
extern cs_port_id_t active_mgmt_port;

void onu_hello_init()
{
    cs_mac_t hello_mac;
    cs_callback_context_t context;

    memcpy(hello_mac.addr, CS_MAC_ADDR_ALL_ONU.addr, CS_MACADDR_LEN);
    epon_request_onu_port_hello_mac_set(context, ONU_DEVICEID_FOR_API, 
    ONU_LLIDPORT_FOR_API, CS_UNI_PORT_ID1, &hello_mac );
}

static void
onu_hello_reset()
{
    hello_link_hdl_t *pNode = NULL;
    
    onu_node.hello_record.state = HELLO_INIT_STATE;
    active_mgmt_port = INVALID_PORT_ID;
    active_hello_port = INVALID_PORT_ID;
    onu_node.hello_record.expire = onu_node.dead_interval;
    cs_lst_scan(&onu_node.handler_list, pNode, hello_link_hdl_t *)
    {
        if(pNode->link_status == HELLO_LINK_DISCONNECTED &&
            pNode->handler != NULL)
        {
            pNode->handler();
        }
    }
}

void onu_hello_proc(cs_uint8 *frame,int len,cs_port_id_t port)
{
    cs_hello_pdu_t *pdu = NULL;
    hello_link_hdl_t *pNode = NULL;

    pdu = (cs_hello_pdu_t *)frame;

    active_mgmt_port = port;
    active_hello_port = port;

    memcpy(onu_node.sc.macaddr.addr, pdu->src_mac.addr, CS_MACADDR_LEN);
    onu_node.hello_interval = ntohl(pdu->hello_interval);
    onu_node.dead_interval = ntohl(pdu->hello_expire);
    onu_node.hello_record.expire= onu_node.dead_interval;
    if(onu_node.hello_record.state == HELLO_INIT_STATE ||
        onu_node.hello_record.state == HELLO_DISCONNECT)
    {
        cs_lst_scan(&onu_node.handler_list, pNode, hello_link_hdl_t *)
        {
            if(pNode->link_status == HELLO_LINK_CONNECTED &&
                pNode->handler != NULL)
            {
                pNode->handler();
            }
        }
    }
    
    onu_node.hello_record.state = HELLO_ACTIVE;

    return;
}

void onu_tmfunc_hello()
{
    cs_int32 rc = -1;
    cs_int8 pkt_buf[1518];
    cs_hello_pdu_t *pdu = NULL;
    cs_int32 pkt_len = 0;
    cs_int32 reset_flag = 0;

    IROS_LOG_MSG(IROS_LOG_LEVEL_DBG0, IROS_MID_HELLO, "ONU hello state 0x%x,expire=%d\n", onu_node.hello_record.state,onu_node.hello_record.expire);
    if (onu_node.hello_record.expire > 0)
        onu_node.hello_record.expire--;

    if (onu_node.hello_record.expire == 0
        && onu_node.hello_record.state == HELLO_ACTIVE) 
    {
        onu_hello_reset();
        reset_flag = 1;
    }

    memset(pkt_buf, 0, sizeof(pkt_buf));
    pkt_len = cs_oam_hdr_build((cs_uint8 *)pkt_buf, &onu_node.sc.macaddr, CS_OAM_OPCODE_HELLO, CS_OUI);
    pdu = (cs_hello_pdu_t *)(&pkt_buf[pkt_len]);
    if(reset_flag)
        pdu->hello_state = HELLO_DISCONNECT;
    else
        pdu->hello_state = onu_node.hello_record.state;
    pdu->payload_len = 0;
    pdu->hello_interval = htonl(onu_node.hello_interval);
    pdu->hello_expire   = htonl(onu_node.dead_interval);
    pdu->payload_type   = CS_ONU_DEV_TYPE;
    memcpy(pdu->src_mac.addr, onu_node.macaddr.addr, CS_MACADDR_LEN);
    memcpy(pdu->dst_mac.addr, onu_node.sc.macaddr.addr, CS_MACADDR_LEN);
    pkt_len += HELLO_PDU_HDR_LEN;
    if (INVALID_PORT_ID != active_hello_port) 
    {
        cs_callback_context_t     context;
        
        rc = epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
                  (cs_uint8 *)pkt_buf,
                  pkt_len,
                  active_hello_port,
                  ONU_CPU_PKT_PRIO_6);
    }
}

void hello_tm_free(void * ptr)
{
    cs_uint32 timerHandle = 0;

    IROS_LOG_MSG(IROS_LOG_LEVEL_DBG1, IROS_MID_HELLO, "hello_tm_free.\n");
    timerHandle = cs_msg_circle_timer_add(app_msg_q, onu_node.hello_interval*1000, (void *) & g_hello_timer);
    if (timerHandle == CS_INVALID_TIMER) {
        IROS_LOG_MSG(IROS_LOG_LEVEL_CRI, IROS_MID_HELLO, "hello_tm_free cs_msg_circle_timer_add failed.\n");
    }
}

static void onu_hello_connected()
{
    cs_printf("ONU hello connected!\n");
}

static void onu_hello_disconnected()
{
    cs_printf("ONU hello disconnected!\n");
}

void app_hello_init()
{    
    
    cs_uint8 mac[CS_MACADDR_LEN];
    cs_uint32 timerHandle = 0;

    IROS_LOG_MSG(IROS_LOG_LEVEL_DBG0, IROS_MID_HELLO, "============Start of APP HELLO Init===========\n");
    onu_hello_init();

    memset(&onu_node, 0, sizeof(onu_node_t));
    cs_lst_init(&onu_node.handler_list , NULL);
    cs_hello_link_change_reg(onu_hello_connected, HELLO_LINK_CONNECTED);
    cs_hello_link_change_reg(onu_hello_disconnected, HELLO_LINK_DISCONNECTED);
    memcpy(onu_node.sc.macaddr.addr, CS_MAC_ADDR_ALL_SC.addr, CS_MACADDR_LEN);
    onu_node.hello_interval = DEFAULT_HELLO_INTERVAL;
    onu_node.dead_interval = DEFAULT_HELLO_EXPIRE;
    onu_node.version = 0x100000;

    startup_config_read(CFG_ID_MAC_ADDRESS, CS_MACADDR_LEN, mac);
    memcpy(onu_node.macaddr.addr, mac, CS_MACADDR_LEN);
    memcpy(onu_node.device.addr, onu_node.macaddr.addr, CS_MACADDR_LEN);

    active_mgmt_port = INVALID_PORT_ID;
    active_hello_port = INVALID_PORT_ID;

    g_hello_timer.msg_type = IROS_MSG_TIME_OUT;
    g_hello_timer.timer_type = APP_TIMER_HELLO;
    timerHandle = cs_msg_circle_timer_add(app_msg_q, 
        onu_node.hello_interval*1000, (void *) & g_hello_timer);
    if (timerHandle == CS_INVALID_TIMER) {
        IROS_LOG_MSG(IROS_LOG_LEVEL_CRI, IROS_MID_HELLO, "app_hello_init cs_msg_circle_timer_add failed.\n");
    }
}

cs_port_id_t get_dest_port()
{
    return active_hello_port;
}

void cs_hello_link_change_reg
    (
    void (*handler)(),
    cs_int32 link_status
    )
{
    hello_link_hdl_t *pNode = NULL;
    
    if(handler == NULL)
        return;

    if(link_status != HELLO_LINK_CONNECTED && 
        link_status != HELLO_LINK_DISCONNECTED)
    cs_lst_scan(&onu_node.handler_list, pNode, hello_link_hdl_t *)
    {
        if(pNode->handler == handler)
            break;
    }

    if(pNode == NULL)
    {
        pNode = (hello_link_hdl_t *)iros_malloc(IROS_MID_HELLO,sizeof(hello_link_hdl_t));
        if(pNode == NULL)
        {
            cs_printf("alloc hello handler failed.\n");
            return;
        }
        pNode->handler = handler;
        pNode->link_status = link_status;
        cs_lst_add(&onu_node.handler_list, (cs_node *)pNode);
        return;
    }

    return;
}

cs_int32 hello_is_active()
{
    if(onu_node.hello_record.state == HELLO_ACTIVE)
        return 1;
    else
        return 0;
}

#endif 
