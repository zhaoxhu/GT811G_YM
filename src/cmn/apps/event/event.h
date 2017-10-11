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
#include "cs_types.h"
#include "cs_event.h"
#include "osal_api_packet.h"

#if 0
#define _EVENT_H_

#include "cs_types.h"
#include "cs_event.h"
#include "osal_api_packet.h"
#define EVENT_MAX_RETRY         16
#define EVENT_RETRY_INTERVAL    1000       /* 1 second */

#define event_malloc(size) iros_malloc(IROS_MID_EVENT, size)

struct frame_record_t {
    struct frame_record_t   *prev;
    struct frame_record_t   *next;
    cs_int8                 *frame;
    cs_uint32               size;
};

typedef struct frame_record_t   frame_record_t;

/* event record */
typedef struct {
    cs_uint32       timer_id;
    cs_uint32  seq_next;
    cs_uint32   retry_num;
    frame_record_t  *frame_queue;
    cs_boolean  enabled;
} event_record_t;

typedef struct {
    event_record_t  event_record;
} event_node_t;

typedef void (*onu_event_handler_t)(cs_uint16 dev, cs_int32 evt, void *msg);
typedef void (*onu_event_hton_t)(cs_uint16 dev, cs_int32 evt, void *msg);

#define MAX_HANDLER_NUM 16
typedef struct {
    onu_event_handler_t    handler[MAX_HANDLER_NUM];
    onu_event_hton_t    hton_func;
} onu_events_t;

typedef struct {
    cs_uint16		  type;
    cs_uint16		  relay;
    cs_uint32		  msg_len;
    cs_uint8		  data[1];
} onu_event_hdr_t;

typedef struct {
    cs_uint8		type;
    cs_uint8		rsvd[3];
    onu_event_hdr_t	evt;
} onu_event_msg_t;

typedef struct {
    cs_node node;
    cs_int32 evt_type;
    void (*handler)(cs_uint16 dev, cs_int32 evt, void *msg, void *cookie);
    void (*hton_func)(cs_uint16 dev, cs_int32 evt, void *msg, void *cookie);
    void *cookie;
} cs_sdk_event_cfg_t;


extern event_node_t event_node;
extern cs_callback_context_t     event_context;

extern void event_node_clean();
extern void event_node_requeue();
extern void event_tx_start();
extern void event_tx_stop();
extern void cmn_msg_proc_event_ack(cs_sdk_event_ack_t *msg);
extern cs_int32 event_tx_add(cs_uint8 *frame, cs_uint32 size);
extern void event_init();
extern void event_detect(cs_int32 type, cs_uint16 msg_len, void* msg);
extern cs_int32 event_hton_reg(cs_int32 evt_type , void *func , void *cookie);
extern cs_int32 onu_evt_reg(cs_int32 evt_type , void *func , void *cookie);
extern void event_main_proc(cs_int32 type, cs_uint32 msg_len, void* msg, cs_boolean relay);

extern void onu_evt_changed_led_proc(cs_uint16 dev, cs_int32 evt, void *msg);
extern void onu_hello_proc(cs_uint8 *frame,int len,cs_port_id_t port);
extern void onu_event_proc(cs_uint8 *frame,int len,cs_port_id_t port);
#endif

#ifndef _EVENT_H_
#define _EVENT_H_

#define EVENT_MAX_RETRY         16
#define EVENT_RETRY_INTERVAL    1000

#define CS_MAX_EVT_NUM 128

typedef struct 
{
    cs_node node;
    void *evt_frame;
    cs_int32 len;
}evt_msg_t;

typedef struct {
    cs_uint32   mutex;
    cs_uint32   timer_id;
    cs_uint32   seq_next;
    cs_uint32   retry_num;
    cs_list     evt_list;
} event_record_t;

#define CS_EVT_LIST_LOCK() cs_mutex_lock(evt_info.mutex)
#define CS_EVT_LIST_UNLOCK() cs_mutex_unlock(evt_info.mutex)

extern cs_int32 onu_evt_reg(cs_int32 evt_type , void *func , void *cookie);
extern void onu_event_proc(cs_uint8 *frame,int len,cs_port_id_t port);
extern void cs_event_init();
extern void onu_evt_changed_led_proc(cs_uint16 dev, cs_int32 evt, void *msg);
extern void onu_link_changed_led_proc(cs_uint16 dev, cs_int32 evt, void *msg);
extern void cs_event_add(evt_msg_t *evt_msg, cs_int32 len);
extern evt_msg_t *evt_msg_alloc(cs_int32 len);
extern void evt_msg_free(evt_msg_t *p);
extern void event_init();

typedef void (*onu_event_handler_t)(cs_uint16 dev, cs_int32 evt, void *msg);
typedef void (*onu_event_hton_t)(cs_uint16 dev, cs_int32 evt, void *msg);

#define MAX_HANDLER_NUM 16
typedef struct {
    onu_event_handler_t    handler[MAX_HANDLER_NUM];
    onu_event_hton_t    hton_func;
} onu_events_t;

typedef struct {
    cs_uint16		  type;
    cs_uint16		  relay;
    cs_uint32		  msg_len;
    cs_uint8		  data[1];
} onu_event_hdr_t;

typedef struct {
    cs_uint8		type;
    cs_uint8		rsvd[3];
    onu_event_hdr_t	evt;
} onu_event_msg_t;


#endif

