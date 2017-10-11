#ifndef __LOOP_DETECT_H__
#define __LOOP_DETECT_H__

#include "app_timer.h"
#include "packet.h"

#define   LOOP_DETECT_PORT_MAX    (g_ldCb.max_port)

#define LOOP_DETECT_DISABLE 0
#define LOOP_DETECT_ENABLE  1

#define LOOP_DETECT_BLOCK_DISABLE 0
#define LOOP_DETECT_BLOCK_ENABLE  1


#define LOOP_DETECT_SEND_INTERVAL_DEFAULT   2000
#define LOOP_DETECT_BLOCK_DEFAULT   30000
#define LOOP_DETECT_CLEAR_CHECK_DEFAULT   1000

#define LOOP_DETECT_PORT_CHECK(port)  if((port < CS_UNI_PORT_ID1) || (port > LOOP_DETECT_PORT_MAX  \
                                                                   && port != CS_ALL_UNI_PORT_ID)) \
                                                                   return CS_E_PARAM;

#define EPON_ETHERTYPE_LOOP_DETECT               0xFFFE

#define DUMPLOOPPKT(a, b, c)   if(g_ldTrace) cs_buffer_dump(a, b, c)

#define LOOP_DETECT_CALL_BACK(port, event)    if(g_ldCb.special_handler)   \
                                                                             g_ldCb.special_handler(port, event);

typedef enum {
    LOOP_DETECTED_EVENT = 0,
    UNBLOCK_TIMER_EXPIRED_EVENT ,
    CLEAR_CHECK_TIMER_EXPIRED_EVENT,
    LOOPDETECT_TIMER_EXPIRED_EVENT ,
    LOOPDETECT_ENABLE_EVENT ,
    LOOPDETECT_DISABLE_EVENT ,
    LOOP_CLEARED_EVENT ,
    LOOPDETECT_ENABLE_PREPARE_EVENT ,
    LOOPDETECT_ENABLE_AFTER_EVENT ,
    LOOPDETECT_DISABLE_PREPARE_EVENT ,
    LOOPDETECT_DISABLE_AFTER_EVENT ,
    LOOP_DETECTED_PREPARE_EVENT ,
    LOOP_DETECTED_AFTER_EVENT ,
    LOOP_CLEARED_PREPARE_EVENT ,
    LOOP_CLEARED_AFTER_EVENT ,
    LOOP_DETECT_PORT_BLOCK_PREPARE_EVENT ,
    LOOP_DETECT_PORT_BLOCK_AFTER_EVENT ,
    LOOP_DETECT_PORT_UNBLOCK_PREPARE_EVENT ,
    LOOP_DETECT_PORT_UNBLOCK_AFTER_EVENT ,
    LOOP_DETECT_MAX_EVENT
}loop_detect_event_t;


typedef cs_status (*loop_detect_event_handler_t)(cs_port_id_t port, loop_detect_event_t  type);

typedef enum {
    NO_LOOP = 0,
    LOOP_DETECTED = 1,
    LOOP_CHECK = 2,
}loop_detect_status_t;

typedef enum {
    LOOP_DETECT_PERIOD_TIMER = 0,
    LOOP_DETECT_BLOCK_TIMER = 1,
    LOOP_DETECT_CLEAR_CHECK_TIMER = 2,
}loop_detect_timers_t;

typedef enum{
    LOOP_DETECT_NOT_BLOCK  = 0,
    LOOP_DETECT_BLOCKED  = 1,
}loop_detect_block_status_t;

typedef enum{
    LOOP_DETECT_BLOCK_GLOBAL = 0,
    LOOP_DETECT_BLOCK_PORT_BASE
}loop_detect_block_object_t;

typedef enum{
    LOOP_DETECT_EXPIRE_UNBLOCK = 0,
    LOOP_DETECT_TX_UNBLOCK 
}loop_detect_unblock_policy_t;

typedef enum{
    LOOP_CLEAR_NEXT_ROUND_CHECK = 0,
    LOOP_CLEAR_IMMEDIATE_CHECK 
}loop_clear_policy_t;

typedef enum{
    LOOP_REASON_NULL = 0,
    LOOP_REASON_INNER,
    LOOP_REASON_OUT
}loop_reason_t;

typedef struct {
    app_timer_msg_t msg;
    loop_detect_timers_t name;
}loop_detect_timer_type_t;

typedef struct {
    loop_detect_timer_type_t type;
    cs_int32    timer_handler;
    cs_uint32  timer_value;
    cs_port_id_t port;
} loop_detect_timer_t;


typedef struct {
    cs_boolean enable;
    loop_detect_status_t status;
    cs_uint32 loop_counter;
    loop_detect_timer_t block_timer;
    loop_detect_timer_t clear_timer;
    cs_boolean need_block;
    loop_detect_block_status_t block_state;
    cs_port_id_t port;
    loop_reason_t reason;
} loop_detect_port_check_t;

typedef struct {
    cs_boolean enable;
    cs_boolean need_block;
    loop_detect_block_object_t block_obj;
    loop_detect_block_status_t block_state; /*valid for global block obj*/
    loop_detect_unblock_policy_t unblock_policy;
    loop_clear_policy_t   clear_policy;
    cs_uint8 max_port;
    loop_detect_timer_t  interval_timer;
    loop_detect_timer_t  block_timer;
    loop_detect_event_handler_t special_handler;
    loop_detect_port_check_t *detect_ports;
}  loop_detect_cb_t;

/* ethernet frame header */
typedef struct {
        cs_uint8                  dst[6];
        cs_uint8                  src[6];
        cs_uint16                ethertype;
        cs_uint32                lb_port;
        cs_uint8                 reserved[60];
} __attribute__((packed))  loop_detect_ether_header_t;

/* ethernet frame header with vlan */
typedef struct {
        cs_uint8                  dst[6];
        cs_uint8                  src[6];
        cs_uint16                tpid;
        cs_uint16                vlan;
        cs_uint16                ethertype;
        cs_uint32                lb_port;
        cs_uint8                  flag;
        cs_uint8                 reserved[59];
} __attribute__((packed))  loop_detect_ether_header_vlan_t;

cs_status loop_detect_enable(cs_port_id_t port);
cs_status loop_detect_disable(cs_port_id_t port);
cs_uint8 loop_detect_frame_handle(cs_pkt_t *pPkt);
cs_status loop_detect_special_handler_reg(loop_detect_event_handler_t special);
void loop_detect_init();
cs_status loop_detect_block_time_set(cs_port_id_t port, cs_uint32 time);
void loop_detect_interval_set(cs_uint32 interval);
cs_status loop_detect_event_handler(cs_port_id_t port, loop_detect_event_t  type);
cs_boolean loop_detect_block_state_get(cs_port_id_t port);
cs_status loop_detect_unblock(cs_port_id_t port);
void loop_detect_block_enable(cs_boolean enable);
cs_status loop_detect_oper_get(cs_port_id_t port, cs_boolean *oper);
cs_uint32 loop_detect_block_time_get(cs_port_id_t port);
cs_uint32 loop_detect_interval_get();
void loop_detect_unblock_policy_set(loop_detect_unblock_policy_t policy);
loop_detect_unblock_policy_t loop_detect_unblock_policy_get();
void loop_detect_block_object_set(loop_detect_block_object_t obj);
loop_detect_block_object_t loop_detect_block_object_get();
void loop_clear_policy_set(loop_clear_policy_t policy);
loop_clear_policy_t loop_clear_policy_get();
cs_status loop_detect_port_block_enable(cs_port_id_t port, cs_boolean enable);
void loop_detect_port_oper_down_handler(cs_port_id_t port);
cs_status loop_detect_status_get(cs_port_id_t port, cs_uint8 *status);
cs_status loop_detect_loop_reason_get(cs_port_id_t port, loop_reason_t *reason);

#endif
