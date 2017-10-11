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

#include "iros_config.h"
#include "plat_common.h"
#include "osal_api_packet.h"
#include "packet.h"
#include "cs_event.h"
#include "event.h"
#include "cs_msg.h"
#include "cs_hello.h"
#include "onu_hello.h"
#include "sdl_api.h"
#include "app_oam_cs.h"

#if 0
/*global variable*/
event_node_t event_node;
cs_callback_context_t     event_context;

/*definiation*/

/*local routine declaration*/
static void tmfunc_event_tx(void *);
static void event_tx_del(frame_record_t **frame_queue);


void event_node_clean()
{
    IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ, IROS_MID_EVENT,
               "EVENT: clear up tx queue and event node\n");

    while (event_node.event_record.frame_queue) {
        event_tx_del(&event_node.event_record.frame_queue);
    }
    event_init();
}

void event_node_requeue()
{
    frame_record_t *frame_queue;


    IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ, IROS_MID_EVENT,
               "EVENT: requeue pending events\n");

    frame_queue = event_node.event_record.frame_queue;

    event_init();

    while (frame_queue) {
        event_tx_add(frame_queue->frame,
                     frame_queue->size);
        frame_queue->frame = NULL;
        event_tx_del(&frame_queue);
    }
}

static void event_tx_del(frame_record_t **frame_queue)
{
    frame_record_t *first_frame, *last_frame;

    first_frame = *frame_queue;
    last_frame = first_frame->prev;

    IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3, IROS_MID_EVENT,
               "EVENT: tx del record 0x%x frame 0x%x\n",
               first_frame, first_frame->frame);

    if (first_frame == last_frame) {
        *frame_queue = NULL;
    } else {
        *frame_queue = first_frame->next;
        last_frame->next = first_frame->next;
        first_frame->next->prev = first_frame->prev;
    }

    if (first_frame->frame) {
        iros_free(first_frame->frame);
    }
    iros_free(first_frame);
}

void event_tx_start()
{
    frame_record_t *first_frame = event_node.event_record.frame_queue;
    cs_sdk_event_header_t *msg;

    cs_port_id_t  dest_port = active_pon_port;

    if (!first_frame) {
        return;
    }

    IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3, IROS_MID_EVENT,
               "EVENT: tx start record 0x%x frame 0x%x port 0x%08x\n",
               first_frame, first_frame->frame,
               dest_port);

    msg = (cs_sdk_event_header_t *)((cs_int8 *)first_frame->frame +
                                          sizeof(cs_oam_pdu_hdr_t));

    event_node.event_record.retry_num = EVENT_MAX_RETRY;
    if(event_node.event_record.seq_next == 0)
    {
        msg->seq_num = htonl(1);
        event_node.event_record.seq_next = 0;
    }
    else if(event_node.event_record.seq_next == 1)
    {
        msg->seq_num = 0;
        event_node.event_record.seq_next = 1;
    }
    else
    {
        event_node_clean();
        return;
    }

    event_node.event_record.timer_id = cs_timer_add(EVENT_RETRY_INTERVAL,tmfunc_event_tx,NULL);

    IROS_LOG_MSG(IROS_LOG_LEVEL_DBG1, IROS_MID_OOBCMD, "=>SC EVENT port 0x%x msg type 0x%x len %d\n", active_mgmt_port, ntohl(msg->msg_type), first_frame->size);
#ifdef HAVE_ONU_HELLO  
    dest_port = get_dest_port();
#else
    dest_port = active_pon_port;
#endif

    epon_request_onu_frame_send(event_context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, (cs_uint8 *)first_frame->frame,
                                first_frame->size, dest_port, ONU_CPU_PKT_PRIO_6);
}

void event_tx_stop()
{
    IROS_LOG_MSG(IROS_LOG_LEVEL_CRI, IROS_MID_EVENT,
               "EVENT: stop event tx\n");

    if (event_node.event_record.frame_queue) {
        cs_timer_del(event_node.event_record.timer_id);
    }

}

static void tmfunc_event_tx(void * data)
{
    frame_record_t *first_frame = event_node.event_record.frame_queue;
    cs_port_id_t dest_port = active_pon_port;

    if (!first_frame) {
        return;
    }

#ifdef HAVE_ONU_HELLO
    dest_port = get_dest_port();
#else
    dest_port = active_pon_port;
#endif
    if (!dest_port) {
        event_node_clean();
        return;
    }

    event_node.event_record.retry_num--;
    if (event_node.event_record.retry_num) {
        event_node.event_record.timer_id = cs_timer_add(EVENT_RETRY_INTERVAL,tmfunc_event_tx,NULL);


        /* re-send the frame */
        epon_request_onu_frame_send(event_context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
                               (cs_uint8 *)first_frame->frame, first_frame->size, dest_port, ONU_CPU_PKT_PRIO_6);
        IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ, IROS_MID_EVENT,
                   "EVENT: timed out record 0x%x frame 0x%x port 0x%08x\n",
                   (cs_uint32) first_frame,
                   (cs_uint32) first_frame->frame, dest_port);
    } else {
        IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ, IROS_MID_EVENT,
                   "EVENT: disabled due to max retries 0x%x\n", active_mgmt_port);

        event_node_clean();
    }
}

void cmn_msg_proc_event_ack(cs_sdk_event_ack_t *msg)
{
    frame_record_t *first_frame = event_node.event_record.frame_queue;
    cs_sdk_event_header_t *event;

    if (first_frame) {
        event = (cs_sdk_event_header_t *)(first_frame->frame +
                                                sizeof(cs_sdk_event_msg_t));
        if (msg->seq_num == event->seq_num) {
            IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3, IROS_MID_EVENT,
                       "EVENT: matched ack seq %d\n", msg->seq_num);
            cs_timer_del(event_node.event_record.timer_id);
            event_tx_del(&event_node.event_record.frame_queue);
            event_tx_start();
        } 
        else {
            IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_EVENT,
                       "EVENT: unexpected ack seq %d\n",
                       msg->seq_num);
        }
    } else {
        IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_EVENT,
                   "EVENT: no frame for ack %d\n",
                   msg->seq_num);
    }
}

void onu_event_proc(cs_uint8 *frame,int len,cs_port_id_t port)
{
    cs_sdk_event_header_t *p = NULL;

    p = (cs_sdk_event_header_t *)frame;
    switch(ntohl(p->msg_type))
    {
        case CS_ESS_SDK_EVENT_ACK:
            cmn_msg_proc_event_ack((cs_sdk_event_ack_t *)p);
            break;
        default:
            break;
    }

    return;
}


cs_int32 event_tx_add(cs_uint8 *frame, cs_uint32 size)
{
    frame_record_t  *this_frame;
    frame_record_t  *last_frame;

#ifdef HAVE_OOB_MGMT
    if (!active_mgmt_port || !hello_is_active()) {
        IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3, IROS_MID_EVENT,
                   "EVENT: event not sent on port 0x%x\n",
                   active_mgmt_port);

        iros_free(frame);
        return 0;
    }
#endif

    this_frame = event_malloc(sizeof(frame_record_t));
    if (!this_frame) {
        IROS_LOG_MSG(IROS_LOG_LEVEL_CRI, IROS_MID_EVENT,
                   "EVENT: fail to alloc record\n");
        iros_free(frame);
        return -1;
    }

    this_frame->frame = (cs_int8 *)frame;
    this_frame->size = size;

    if (event_node.event_record.frame_queue) {
        last_frame = event_node.event_record.frame_queue->prev;
        last_frame->next = this_frame;
        this_frame->next = event_node.event_record.frame_queue;
        this_frame->prev = last_frame;
        event_node.event_record.frame_queue->prev = this_frame;
    } else {
        event_node.event_record.frame_queue = this_frame;
        this_frame->prev = this_frame;
        this_frame->next = this_frame;
        event_tx_start();
    }

    return 0;
}
#endif

#ifdef HAVE_ONU_EVENT
event_record_t evt_info;
void cs_event_tx();

void cs_event_init()
{
    memset(&evt_info, 0, sizeof(event_record_t));
    cs_lst_init(&evt_info.evt_list, NULL);
    cs_mutex_init(&evt_info.mutex, "evtListMut", 0);
    evt_info.retry_num = EVENT_MAX_RETRY;
}

void cs_event_ack_proc(cs_sdk_event_ack_t *msg)
{
    cs_uint32 seq = 0;
    evt_msg_t *pMsg = NULL;

    CS_EVT_LIST_LOCK();
    seq = htonl(msg->seq_num);
    if(seq == evt_info.seq_next)
    {
        cs_timer_del(evt_info.timer_id);
        pMsg = (evt_msg_t *)cs_lst_get(&evt_info.evt_list);
        if(pMsg)
            evt_msg_free(pMsg);

        if(evt_info.seq_next == CS_EVENT_SEQ_RESET)
            evt_info.seq_next = 0;
        else
            evt_info.seq_next++;

        cs_event_tx();
    }
    else
    {
        IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_EVENT,
                       "EVENT: unexpected ack seq %d\n",seq);
    }

    CS_EVT_LIST_UNLOCK();
    return;
}

void cs_evt_list_clr()
{
    cs_node *pNode = NULL;

    CS_EVT_LIST_LOCK();
    while((pNode = cs_lst_get(&evt_info.evt_list)) != NULL)
    {
        evt_msg_free((evt_msg_t *)pNode);
    }

    evt_info.retry_num = EVENT_MAX_RETRY;
    CS_EVT_LIST_UNLOCK();
}

void cs_event_tmfunc(void *data)
{
    CS_EVT_LIST_LOCK();
    if(evt_info.retry_num == 0)
    {
        cs_evt_list_clr();
        CS_EVT_LIST_UNLOCK();
        return;
    }

    evt_info.retry_num--;
    cs_event_tx();
    CS_EVT_LIST_UNLOCK();
    return;
}

void cs_event_tx()
{
    cs_callback_context_t   event_context;
    evt_msg_t *evt_msg = NULL;
    cs_sdk_event_msg_t *pdu = NULL;
    cs_port_id_t dst_port;

    evt_msg = (evt_msg_t *)cs_lst_first(&evt_info.evt_list);
    if(evt_msg == NULL)
        return;

    pdu = (cs_sdk_event_msg_t *)((cs_uint8 *)evt_msg->evt_frame + sizeof(cs_oam_pdu_hdr_t));
    pdu->seq_num = htonl(evt_info.seq_next);
    dst_port = get_dest_port();
    epon_request_onu_frame_send(event_context, 0, 0, 
         (cs_uint8 *)evt_msg->evt_frame, evt_msg->len, dst_port, ONU_CPU_PKT_PRIO_6);
    evt_info.timer_id = cs_timer_add(EVENT_RETRY_INTERVAL,cs_event_tmfunc,NULL);
}

void cs_event_add(evt_msg_t *evt_msg, cs_int32 len)
{
    if(evt_msg == NULL)
        return;

    evt_msg->len = len;
    CS_EVT_LIST_LOCK();
    cs_lst_add(&evt_info.evt_list, (cs_node *)evt_msg);

    if(cs_lst_count(&evt_info.evt_list) == 1)
    {
        evt_info.retry_num = EVENT_MAX_RETRY;
        cs_event_tx();
    }
    CS_EVT_LIST_UNLOCK();
    return;
}

void onu_event_proc(cs_uint8 *frame,int len,cs_port_id_t port)
{
    cs_sdk_event_header_t *p = NULL;

    p = (cs_sdk_event_header_t *)frame;
    switch(ntohl(p->msg_type))
    {
        case CS_ESS_SDK_EVENT_ACK:
            cs_event_ack_proc((cs_sdk_event_ack_t *)p);
            break;
        default:
            break;
    }

    return;
}

evt_msg_t *evt_msg_alloc(cs_int32 len)
{
    evt_msg_t *p = NULL;

    p = (evt_msg_t *)iros_malloc(IROS_MID_EVENT, sizeof(evt_msg_t));
    if(p == NULL)
        return NULL;

    memset(p, 0, sizeof(evt_msg_t));
    p->evt_frame = (void *)iros_malloc(IROS_MID_EVENT, len);
    if(p->evt_frame == NULL)
    {
        iros_free(p);
        return NULL;
    }
    memset(p->evt_frame, 0, len);

    return (evt_msg_t *)p;
}

void evt_msg_free(evt_msg_t *p)
{
    iros_free(p->evt_frame);
    iros_free(p);
    return;
}
    
#endif


