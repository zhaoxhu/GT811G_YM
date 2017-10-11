/***********************************************************************/
/* This file contains unpublished documentation and software                                            */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,                              */
/* in whole or in part, of the information in this file without a                                              */
/* written consent of an officer of Cortina Systems Incorporated is                                     */
/* strictly prohibited.                                                                                                             */
/* Copyright (c) 2002-2010 by Cortina Systems Incorporated.                                            */
/***********************************************************************/


#ifndef _osal_api_core_
#define _osal_api_core_

#include "cs_types.h"
#include "osal_config.h"
#include "stdarg.h"   /* for va_list */


/* Define constants for semaphore */
#define CS_OSAL_SEM_FULL               (1)
#define CS_OSAL_SEM_EMPTY              (0)


/* Defines for Semaphore, Queue Timeout parameters */
#define CS_OSAL_NO_WAIT          (0)
#define CS_OSAL_WAIT_FOREVER     (-1)

#define CS_OSAL_INVALID_QUEUE  (-1)
#define CS_OSAL_INVALID_THREAD  (-1)

#define osal_printf if(osal_debug) cs_printf
/*  tables for the properties of objects */

/* threads */
typedef struct
{
    cs_int8 name [CS_OSAL_MAX_API_NAME];
    cs_uint32 creator;
    cs_uint32 stack_size;
    cs_uint32 priority;
    cs_uint32 os_thread_id;
}osal_thread_prop_t;

/* Counting Semaphores */
typedef struct
{
    cs_int8 name [CS_OSAL_MAX_API_NAME];
    cs_uint32 creator;
}osal_count_sem_prop_t;

/* Mutexes */
typedef struct
{
    cs_int8 name [CS_OSAL_MAX_API_NAME];
    cs_uint32 creator;
}osal_mut_prop_t;


/* struct for cs_local_time_get() */
typedef struct
{
    cs_uint32 seconds;
    cs_uint32 microsecs;
}osal_time_t;


/* This typedef is for the cs_err_name_get function, to ensure
 * everyone is making an array of the same length */
typedef cs_int8 osal_err_name_t[35];


/**************************************************************************
                                                        Initialization of OSAL API
**************************************************************************/
void cs_osal_core_init(
                    void
                    );

/**************************************************************************
                                                        Thread API
**************************************************************************/
cs_int32 cs_thread_create(
                    cs_uint32        *thread_id,
                    const cs_int8   *thread_name,      /* Thread name's length must be less than 20-bytes*/
                    const void       *function_pointer, /*Entry of the thread you create*/
                    void                *param,                  /*parameter required by function_pointer*/
                    cs_uint32       stack_size,              /* Stack size */
                    cs_uint32       priority,                   /* Range 0~255 , 0 is the highest priority , 255 is the lowest priority*/
                    cs_uint32       flags                        /* Don't support , you can ignore it */
                    );

#if 1
//added by zhuxh
cs_int32 cs_thread_delete(cs_int32 thread_id);	
#endif

cs_int32 cs_thread_delay(
                    cs_uint32 milli_second                 /* in millisecond , For Ecos and Linux , the minimum value should be 10,*/
                    );                                                   /* for Vxworks , the minimum value should be 17    */

cs_uint32 cs_thread_number();

void cs_thread_show();


/**************************************************************************
                                                    Semaphore API
**************************************************************************/
cs_int32 cs_semaphore_init(
                    cs_uint32       *sem_id,                /* pointer of semaphore */
                    const cs_int8 *sem_name,          /* semaphore's name */
                    cs_uint32       sem_initial_value,/* counter semaphore's count */
                    cs_uint32       options                  /* Don't support , you can ignore it now */
                    );
cs_int32 cs_semaphore_destroy(
                    cs_uint32 sem_id
                    );
cs_int32 cs_semaphore_post(
                    cs_uint32 sem_id
                    );
cs_int32 cs_semaphore_wait(
                    cs_uint32 sem_id,
                    cs_int32 timeout                   /* Three types value : CS_OSAL_NO_WAIT  , CS_OSAL_WAIT_FOREVER or milliseconds */
                    );
cs_uint32 cs_semaphore_number();


/**************************************************************************
                                                    Mutex API
**************************************************************************/
cs_int32 cs_mutex_init(
                    cs_uint32 *mut_id,              /* Pointer of mutex */
                    const cs_int8 *mut_name,  /* Mutex's name */
                    cs_uint32 options                /* Don't support , you can ignore it now */
                    );
cs_int32 cs_mutex_destroy(
                    cs_uint32 mut_id
                    );
cs_int32 cs_mutex_unlock(
                    cs_uint32 mut_id
                    );
cs_int32 cs_mutex_lock(
                    cs_uint32 mut_id
                    );
cs_int32 cs_mutex_trylock(
                    cs_uint32 mut_id
                    );
cs_uint32 cs_mutex_number();

/**************************************************************************
                                                    Queue API
**************************************************************************/
cs_int32 cs_queue_create(
                        cs_uint32 *queue_id,                /* Pointer of queue_id */
                        const cs_int8 *queue_name,    /* Queue's name */
                        cs_uint32 queue_depth,           /* Max message can be put into the queue */
                        cs_uint32 data_size,                /* The max length of each message */
                        cs_uint32 flags);                       /* Don't support , you can ignore it now */
cs_int32 cs_queue_delete(
                        cs_uint32 queue_id
                        );
cs_int32 cs_queue_get(
                        cs_uint32 queue_id,
                        void *data,                               /* Message in the queue will be copied to the buffer which pointer is 'data',
                                                                         so you must insure for the buffer's length , the buffer's lenght must equal or
                                                                         greater than the length of the message which has been put in the queue. */
                        cs_uint32 size,                         /* data's size , It must equal or greater than '*size_copied' */
                        cs_uint32 *size_copied,          /* the actual lenght  which have been copied to the buffer 'data' */
                        cs_int32 timeout                      /* The maximum interval you can wait , if there is no message in the queue. */
                                                                        /* Three types value : CS_OSAL_NO_WAIT  , CS_OSAL_WAIT_FOREVER or milliseconds */
                        );
cs_int32 cs_queue_put(
                        cs_uint32 queue_id,
                        void *data,                             /* data which will be copied to the queue */
                        cs_uint32 size,                       /* size of data , it must equal or less than the queue's size*/
                        cs_int32 timeout,                   /* Maximum interval you can wait , if the queue is full */
                        cs_uint32 flags                      /* Don't support , you can ignore it */
                        );
cs_int32 cs_queue_count(
                        cs_uint32 queue_id               /* Get the total message in the queue */
                        );
void cs_queue_show(cs_uint32 queue_id);
cs_uint32 cs_queue_number();
cs_int32 cs_queue_type_get(cs_uint32 queue_id);

cs_int32 cs_pri_queue_create (
                        cs_uint32 *queue_id,
                        const cs_int8 *queue_name,
                        cs_uint32 queue_depth,
                        cs_uint32 data_size,
                        cs_int32 pri_num
                        );
cs_int32 cs_pri_queue_put (
                        cs_uint32 queue_id,
                        void *data,
                        cs_uint32 size,
                        cs_int32 timeout,
                        cs_int32 priority
                        );
cs_int32 cs_pri_queue_get (
                        cs_uint32 queue_id,
                        void *data,
                        cs_uint32 size,
                        cs_uint32 *size_copied,
                        cs_int32 timeout
                        );
cs_uint32 cs_pri_queue_number();
cs_int32 cs_pri_queue_count (cs_uint32 queue_id);
cs_int32 cs_pri_queue_max_priority(cs_uint32 queue_id);
void cs_pri_queue_show(cs_uint32 queue_id);


/**************************************************************************
                                                   cs_current_time
**************************************************************************/
cs_uint64 cs_current_time(void);
void cs_timestamp_print();


/**************************************************************************
                                                   OS Time/Tick related API
**************************************************************************/
cs_int32 cs_milli_to_ticks(
                        cs_uint32 milli_seconds
                        );
cs_int32 cs_tick_to_micros(
                        void
                        );
cs_int32 cs_local_time_get(
                        osal_time_t *time_struct
                        );
cs_int32 cs_local_time_set(
                        osal_time_t *time_struct
                        );


/**************************************************************************
                                                  API for a useful debugging function
**************************************************************************/
#if 0
cs_int32 cs_err_name_get(
                        cs_int32 error_num,
                        osal_err_name_t* err_name
                        );

#endif
/**************************************************************************
                                             Abstraction for printf statements
**************************************************************************/
void cs_printf(const cs_int8 *string, ...);

cs_uint32 cs_memory_usage();

cs_int32 cs_halt(cs_int8 *String,...);

#endif
