/***********************************************************************/
/* This file contains unpublished documentation and software                                            */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,                              */
/* in whole or in part, of the information in this file without a                                              */
/* written consent of an officer of Cortina Systems Incorporated is                                     */
/* strictly prohibited.                                                                                                             */
/* Copyright (c) 2002-2010 by Cortina Systems Incorporated.                                            */
/***********************************************************************/

#ifndef _CS_TIMER_H_
#define _CS_TIMER_H_

#include "cs_types.h"
#include "util_list.h"


#define CS_TIMERS_EX_MSG


typedef struct {
    cs_uint32   timer_thread_id;
    cs_uint32   timer_mutex_id;
    cs_uint32   delta;
    cs_list     timer_list;
    cs_list     expire_timer_list;
    cs_uint32   msg_timer_sendQ_error;
    cs_uint32   total_timeout_timer;
    cs_uint32   total_timer_delta;
    cs_uint32   timer_peek_value;
    cs_uint32   callback_peek_time;
    cs_uint32   callback_peek_ptr;
} cs_timer_control_t;

typedef struct iros_timer {
    cs_node         node;
    cs_int32        interval;
    cs_int32        remain_interval;
    cs_int32        (*callback)(struct iros_timer *);
    void            *data;
    void            (*app_callback)(void *data);
    cs_int16         timer_type;
    cs_int16         stop_flag; /*1: stopped  0: not stopped*/
    cs_uint32       queue_id; /* indentify timer is via msg or callback */
#ifdef CS_TIMERS_EX_MSG
    void            (*data_free_func)(void *data);
#endif
    cs_int16        del_apply;
    cs_int16        stop_apply;
} cs_timer_t;

#define CS_TIMER_DESTROYED  2
#define CS_TIMER_STOPPED    1
#define CS_TIMER_NO_STOPPED 0

#define CS_TIMER_OK         0
#define CS_TIMER_ERROR      -1

#define CS_INVALID_TIMER    0
#define CS_TIMER_NO_QUEUE  (-1)

#ifdef CS_OSAL_MAX_TIMER
#define CS_MAX_TIMER_NUMBER  CS_OSAL_MAX_TIMER
#else
#define CS_MAX_TIMER_NUMBER  32
#endif

typedef enum
{
    CS_ONCE_TIMER = 0,
    CS_CIRCLE_TIMER = 1,
}TIMER_TYPE;

#define iros_timer_log if(cs_timers_debug) cs_printf

extern cs_uint32 timer_thread_count;

extern cs_int32 cs_timer_init();
extern cs_int32 cs_timer_add(cs_uint32 timeout, void (*callback)(void *), void *data);
extern cs_int32 cs_circle_timer_add(cs_uint32 timeout, void (*callback)(void *), void *data);
extern cs_int32 cs_timer_del(cs_uint32 timer_handle);
extern cs_int32 cs_timer_stop(cs_uint32 timer_handle);
extern cs_int32 cs_timer_start(cs_uint32 timer_handle);
extern cs_int32 cs_timer_restart(cs_uint32 timer_handle);
extern cs_uint32 cs_count_timer();
extern void cs_timer_show();
extern cs_int32 cs_timer_retiming(cs_uint32 timer_handle , cs_uint32 timeout);
extern cs_int32 cs_timer_remain_interval(cs_int32 timer);

#ifdef CS_TIMERS_EX_MSG

cs_int32 cs_msg_circle_timer_add(
                        cs_uint32 queue_id ,
                        cs_int32 timeout ,
                        void *data
                        );

cs_int32 cs_msg_timer_add(
                        cs_uint32 queue_id ,
                        cs_int32 timeout ,
                        void *data,
                        void (*data_free_func)(void *)
                        );
#endif

#endif

