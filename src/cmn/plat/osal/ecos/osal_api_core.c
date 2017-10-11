/***********************************************************************/
/* This file contains unpublished documentation and software                                            */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,                              */
/* in whole or in part, of the information in this file without a                                              */
/* written consent of an officer of Cortina Systems Incorporated is                                     */
/* strictly prohibited.                                                                                                             */
/* Copyright (c) 2002-2010 by Cortina Systems Incorporated.                                            */
/***********************************************************************/


#include <stdio.h>
#include <cyg/kernel/kapi.h>
#include <cyg/hal/hal_arch.h>
#include <pkgconf/hal.h>
#include <cyg/hal/hal_if.h>
#include <cyg/hal/hal_io.h>
#include <pkgconf/system.h>
#include <pkgconf/memalloc.h>
#include <pkgconf/isoinfra.h>
#include <stdlib.h>
#include <sys/time.h>
#include "fcntl.h"
#include "pthread.h"
#include "semaphore.h"
#include "mqueue.h"
#include "osal_common.h"
#include "osal_api_core.h"

#define CS_OSAL_MAX_PRI         31
#define CS_OSAL_UNINIT          0
#define OK	                    0

cs_uint32 cs_creator_find(void);

void usleep(unsigned int usecs)
{
	unsigned int msecs = 0;
	unsigned int ticks = 0;

	msecs = usecs/1000;
	ticks = msecs/10;
	if(ticks == 0)
		ticks = 1;

	cyg_thread_delay(ticks);
	return;
}

/*  tables for the properties of objects */

/* threads */
#if 1
typedef struct {
    cs_int32 free;
    cyg_handle_t id;
    cs_int8 name [CS_OSAL_MAX_API_NAME];
    cyg_handle_t creator;
    cs_uint32 stack_size;
    cs_uint32 priority;
    cyg_thread thread_ctrl;
    cs_uint8 *stack_buf;
}osal_thread_record_t;
#else
typedef struct {
    int 			free;
    pthread_t 		id;
    char 			name[CS_OSAL_MAX_API_NAME];
    int 			creator;
    unsigned int 	stack_size;
    unsigned int 	priority;
}osal_thread_record_t;
#endif

/* Counting Semaphores */
#if 0
typedef struct {
    cs_int32 free;
    cyg_sem_t  id;                       /* a pointer to the id */
    cs_int8 name [CS_OSAL_MAX_API_NAME];
    cyg_handle_t creator;
}osal_count_sem_record_t;
#else
typedef struct {
    int 	free;
    sem_t 	id;
    char 	name[CS_OSAL_MAX_API_NAME];
    int 	creator;
}osal_count_sem_record_t;
#endif

/* Mutexes */
#if 0
typedef struct {
    cs_int32 free;
    cyg_mutex_t id;
    cs_int8 name [CS_OSAL_MAX_API_NAME];
    cyg_handle_t creator;
}osal_mut_record_t;
#else
typedef struct {
    int 			free;
    pthread_mutex_t id;
    char 			name[CS_OSAL_MAX_API_NAME];
    int 			creator;
}osal_mut_record_t;
#endif

/* queues */
#if 0
typedef struct {
    cs_int32 free;
    cyg_handle_t id;
    cs_int8 name [CS_OSAL_MAX_API_NAME];
    cyg_handle_t creator;
    cyg_mbox mbox;
    cs_uint32 mempool_id;
    cs_int32 depth;
    cs_int32 queue_size;
    cs_int32 queue_type;
    cs_int32    pri_num;
    cs_uint32   cnt_sem_id;
    cs_list         queue_list[CS_OSAL_MAX_QUEUE_PRI];
    cs_uint32 queue_mutex[CS_OSAL_MAX_QUEUE_PRI];
    cs_uint32   pri_queue_peek[CS_OSAL_MAX_QUEUE_PRI];

    cs_uint32 peek_value;
    cs_uint32 put_error;
    cs_uint32 put_full_error;
    cs_uint32 put_timeout_error;
    cs_uint32 put_data_error;
    cs_uint32 put_data_long;
    cs_uint32 get_error;
    cs_uint32 get_timeout_error;
    cs_uint32 get_data_error;
    cs_uint32 get_data_long;
}osal_queue_record_t;
#else
typedef struct {
    int 			free;
    mqd_t 			id;
    char 			name [CS_OSAL_MAX_API_NAME];
    int 			creator;
    unsigned int 	mempool_id;
    int				queue_size;
    int				depth;
    int				queue_type;
    int    			pri_num;
    unsigned int   	cnt_sem_id;
    cs_list     	queue_list[CS_OSAL_MAX_QUEUE_PRI];
    unsigned int 	queue_mutex[CS_OSAL_MAX_QUEUE_PRI];
    unsigned int	pri_queue_peek[CS_OSAL_MAX_QUEUE_PRI];
    unsigned int 	cur_num;
    unsigned int 	peek_value;
    unsigned int 	put_error;
    unsigned int 	put_full_error;
    unsigned int 	put_timeout_error;
    unsigned int 	put_data_error;
    unsigned int 	put_data_long;
    unsigned int 	get_error;
    unsigned int 	get_timeout_error;
    unsigned int 	get_data_error;
    unsigned int 	get_data_long;
}osal_queue_record_t;

#endif

/* Tables where the OS object information is stored */
osal_thread_record_t    osal_thread_table      [CS_OSAL_MAX_THREAD];
osal_count_sem_record_t osal_count_sem_table   [CS_OSAL_MAX_COUNT_SEM];
osal_mut_record_t       osal_mut_table         [CS_OSAL_MAX_MUTEX];
osal_queue_record_t     osal_queue_table       [CS_OSAL_MAX_QUEUE];

#if 1
cyg_mutex_t osal_task_table_mutex;
#else
pthread_mutex_t osal_thread_table_mut;
#endif
#if 0
cyg_mutex_t osal_count_sem_table_mutex;
#else
pthread_mutex_t osal_count_sem_table_mut;
#endif
#if 0
cyg_mutex_t osal_mut_table_mutex;
#else
pthread_mutex_t osal_mut_table_mut;
#endif
#if 0
cyg_mutex_t osal_queue_table_mutex;
#else
pthread_mutex_t osal_queue_table_mut;
#endif
cs_int32 osal_debug=0;


/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: cs_osal_core_init

   Purpose: Initialize the tables that the OSAL API uses to keep track of information
            about objects

   returns: nothing
---------------------------------------------------------------------------------------*/
void cs_osal_core_init(void)
{
    cs_int32 i;

    /* Initialize Task Table */
	#if 0
    for (i = 0; i < CS_OSAL_MAX_THREAD; i++) {
        memset(&osal_thread_table[i] , 0 , sizeof(osal_thread_record_t));
        osal_thread_table[i].free        = TRUE;
        osal_thread_table[i].id          = CS_OSAL_UNINIT;
        osal_thread_table[i].creator     = CS_OSAL_UNINIT;
        strcpy(osal_thread_table[i].name, "");
    }
	#else
	for (i = 0; i < CS_OSAL_MAX_THREAD; i++) {
        memset(&osal_thread_table[i] , 0 , sizeof(osal_thread_record_t));
        osal_thread_table[i].free = TRUE;
    }
	#endif

    /* Initialize Counting Semaphore Table */
	#if 0
    for (i = 0; i < CS_OSAL_MAX_COUNT_SEM; i++) {
        osal_count_sem_table[i].free        = TRUE;
        memset(&osal_count_sem_table[i].id , 0 , sizeof(cyg_sem_t));
        osal_count_sem_table[i].creator     = CS_OSAL_UNINIT;
        strcpy(osal_count_sem_table[i].name, "");
    }
	#else
	for (i = 0; i < CS_OSAL_MAX_COUNT_SEM; i++) {
        memset(&osal_count_sem_table[i] , 0 , sizeof(osal_count_sem_record_t));
        osal_count_sem_table[i].free = TRUE;
    }
	#endif


    /* Initialize Mutex Semaphore Table */
	#if 0
    for (i = 0; i < CS_OSAL_MAX_MUTEX; i++) {
        osal_mut_table[i].free        = TRUE;
        memset(&osal_mut_table[i].id , 0 , sizeof(cyg_mutex_t));
        osal_mut_table[i].creator     = CS_OSAL_UNINIT;
        strcpy(osal_mut_table[i].name, "");
    }
	#else
	for (i = 0; i < CS_OSAL_MAX_MUTEX; i++) {
        memset(&osal_mut_table[i] , 0 , sizeof(osal_mut_record_t));
        osal_mut_table[i].free = TRUE;
    }
	#endif

    /* Initialize message queue Table */
	#if 0
    for (i = 0; i < CS_OSAL_MAX_QUEUE ; i++) {
        memset(&osal_queue_table[i] , 0 , sizeof(osal_queue_record_t));
        osal_queue_table[i].free        = TRUE;
        osal_queue_table[i].id          = 0;
        osal_queue_table[i].creator     = CS_OSAL_UNINIT;
        strcpy(osal_queue_table[i].name, "");
    }
	#else
	for (i = 0; i < CS_OSAL_MAX_QUEUE; i++) {
        memset(&osal_queue_table[i] , 0 , sizeof(osal_queue_record_t));
        osal_queue_table[i].free = TRUE;
    }
	#endif

	#if 1
    cyg_mutex_init(&osal_task_table_mutex);
	#else
	pthread_mutex_init((pthread_mutex_t *)&osal_thread_table_mut, NULL);
	#endif
	#if 0
    cyg_mutex_init(&osal_count_sem_table_mutex);
	#else
	pthread_mutex_init((pthread_mutex_t *)&osal_count_sem_table_mut, NULL);
	#endif
	#if 0
    cyg_mutex_init(&osal_mut_table_mutex);
	#else
	pthread_mutex_init((pthread_mutex_t *)&osal_mut_table_mut, NULL);
	#endif
	#if 0
    cyg_mutex_init(&osal_queue_table_mutex);
	#else
	pthread_mutex_init((pthread_mutex_t *)&osal_queue_table_mut, NULL);
	#endif

    cs_sys_mod_init();
    return;

}


/****************************************************************************************
                                    TASK API
****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: cs_thread_create

   Purpose: Creates a task and starts running it.

   returns: CS_E_OSAL_INVALID_POINTER if any of the necessary pointers are NULL
            CS_E_OSAL_ERR_NAME_TOO_LONG if the name of the task is too long to be copied
            CS_E_OSAL_ERR_INVALID_PRIORITY if the priority is bad
            CS_E_OSAL_ERR_NO_FREE_IDS if there can be no more tasks created
            CS_E_OSAL_ERR_NAME_TAKEN if the name specified is already used by a task
            CS_E_OSAL_ERR if the operating system calls fail
            CS_E_OSAL_OK if success

    NOTES: task_id is passed back to the user as the ID. Flags are unused at this point.


---------------------------------------------------------------------------------------*/
#if 1
cs_int32 cs_thread_create(cs_uint32 *thread_id,  const cs_int8 *thread_name,
                              const void *function_pointer, void *param , cs_uint32 stack_size,
                              cs_uint32 priority, cs_uint32 flags)
{
    cs_uint32 possible_taskid;
    cs_uint8 *stack_buf = NULL;

    /* we don't want to allow names too long*/
    /* if truncated, two names might be the same */

    /* Check for NULL pointers */

    if ((thread_name == NULL) || (function_pointer == NULL) || (thread_id == NULL)) {
        osal_printf("\r\n thread create failed , cause some parameter is NULL");
        return CS_E_OSAL_INVALID_POINTER;
    }

    if (strlen(thread_name) >= CS_OSAL_MAX_API_NAME) {
        osal_printf("\r\n thread name is too long");
        return CS_E_OSAL_ERR_NAME_TOO_LONG;
    }

    /* Check for bad priority */

    if (priority > CS_OSAL_MAX_PRI) {
        osal_printf("\r\n thread priority is out of range");
        return CS_E_OSAL_ERR_INVALID_PRIORITY;
    }

    /* Check Parameters */
    stack_buf = (cs_uint8 *)iros_malloc(IROS_MID_OSAL , stack_size);
    if (stack_buf == NULL) {
        osal_printf("\r\n Allocate thread's stack space failed");
        return CS_E_OSAL_ERR;
    }

    cyg_mutex_lock(&osal_task_table_mutex);
    for (possible_taskid = 0; possible_taskid < CS_OSAL_MAX_THREAD; possible_taskid++) {
        if (osal_thread_table[possible_taskid].free  == TRUE) {
            break;
        }
    }

    /* Check to see if the id is out of bounds */
    if (possible_taskid >= CS_OSAL_MAX_THREAD || osal_thread_table[possible_taskid].free != TRUE) {
        cyg_mutex_unlock(&osal_task_table_mutex);
        iros_free(stack_buf);
        osal_printf("\r\n no free thread can be allocate");
        return CS_E_OSAL_ERR_NO_FREE_IDS;
    }

#if 0 /*Remove it for new requirement 2009-12-28*/
    /* Check to see if the name is already taken */
    for (i = 0; i < CS_OSAL_MAX_THREAD; i++) {
        if ((osal_thread_table[i].free == FALSE) &&
                (strcmp(thread_name, osal_thread_table[i].name) == 0)) {
            cyg_mutex_unlock(&osal_task_table_mutex);
            iros_free(stack_buf);
            return CS_E_OSAL_ERR_NAME_TAKEN;
        }
    }
#endif

    osal_thread_table[possible_taskid].free = FALSE;

    cyg_mutex_unlock(&osal_task_table_mutex);
    /* Create VxWorks Task */

    cyg_thread_create(priority,
                      function_pointer,
                      (cyg_addrword_t)param,
                      (cs_int8*)thread_name,
                      stack_buf,
                      stack_size,
                      &osal_thread_table[possible_taskid].id,
                      &osal_thread_table[possible_taskid].thread_ctrl);
    cyg_thread_resume(osal_thread_table[possible_taskid].id);

    *thread_id = possible_taskid;

    strcpy(osal_thread_table[*thread_id].name, thread_name);

    /* this Id no longer free */
    cyg_mutex_lock(&osal_task_table_mutex);
    osal_thread_table[*thread_id].free = FALSE;
    osal_thread_table[*thread_id].creator = (cyg_handle_t)cs_creator_find();
    osal_thread_table[*thread_id].stack_size = stack_size;
    osal_thread_table[*thread_id].priority = priority;
    osal_thread_table[*thread_id].stack_buf = stack_buf;
    cyg_mutex_unlock(&osal_task_table_mutex);
    return CS_E_OSAL_OK;
}

#if 1
/*
 *added by zhuxh
 */
cs_int32 cs_thread_delete(cs_int32 thread_id)
{
	//进入资源保护
	cyg_mutex_lock(&osal_task_table_mutex);
	
	//入口规则检查
	if((CS_OSAL_MAX_THREAD <= thread_id) || (osal_thread_table[thread_id].free != FALSE))
	{
		//退出线程保护
		cyg_mutex_unlock(&osal_task_table_mutex);
		return CS_E_OSAL_ERR_INVALID_ID;
	}
	
	/* Delete VxWorks Task */
	cyg_thread_kill(osal_thread_table[thread_id].id);

	//释放内存
	iros_free(osal_thread_table[thread_id].stack_buf);

	//删除线程的操作
	memset(osal_thread_table[thread_id].name, 0, CS_OSAL_MAX_API_NAME);
	osal_thread_table[thread_id].free  = TRUE;
	osal_thread_table[thread_id].creator = 0;
	osal_thread_table[thread_id].stack_size = 0;
	osal_thread_table[thread_id].priority = 0;
	osal_thread_table[thread_id].stack_buf = NULL;
	
	//退出线程保护
	cyg_mutex_unlock(&osal_task_table_mutex);
	//退出
	return CS_E_OSAL_OK;
}
#endif

/*---------------------------------------------------------------------------------------
   Name: cs_thread_delay

   Purpose: Delay a task for specified amount of clock ticks have occurred

   returns: CS_E_OSAL_ERR if sleep fails
            CS_E_OSAL_OK if success

   Notes: VxWorks uses the system clock to handle task delays.  The system clock usually
            runs at 60Hz. This means that the resolution of the delay will be course.
            It rounds up.
---------------------------------------------------------------------------------------*/

cs_int32 cs_thread_delay(cs_uint32 milli_second)
{
    /* if successful, the execution of task will pend here until delay finishes */
    cs_uint32 sys_ticks;

    sys_ticks = cs_milli_to_ticks(milli_second);
    if(sys_ticks == 0)
        sys_ticks = 1;
    cyg_thread_delay(sys_ticks);

    return CS_E_OSAL_OK;

}

cs_uint32 cs_thread_number()
{
    cs_int32 i;
    cs_uint32 count=0;

    cyg_mutex_lock(&osal_task_table_mutex);
    for(i = 0; i < CS_OSAL_MAX_THREAD; i++)
    {
        if(osal_thread_table[i].free == FALSE)
            count++;
    }
    cyg_mutex_unlock(&osal_task_table_mutex);

    return count;
}

typedef struct
{
    cs_node node;
    cyg_thread_info info;
}thread_display_t;

void cs_thread_show()
{
    cyg_handle_t thread=0;
    cyg_uint16 id=0;
    thread_display_t info[32];
    cs_uint32 thread_num = 0;
    cs_list display_list;
    thread_display_t *pNode = NULL;
    int addflag = 0;

    cs_lst_init(&display_list , NULL);
    memset(&info[0] , 0 , 32*sizeof(cyg_thread_info));
    while( cyg_thread_get_next(&thread, &id))
    {
        if(!cyg_thread_get_info(thread, id, &info[thread_num].info))
            continue;

        addflag = 0;
        cs_lst_scan(&display_list , pNode , thread_display_t *)
        {
            if(pNode->info.set_pri <= info[thread_num].info.set_pri)
                continue;
            cs_lst_insert(&display_list, (cs_node *)cs_lst_prev((cs_node *)pNode), (cs_node *)&info[thread_num]);
            addflag = 1;
            break;
        }

        if(!addflag)
            cs_lst_add(&display_list, (cs_node *)&info[thread_num]);
        thread_num++;
    }

    //cs_printf("\r\n==============================================================================");
    cs_printf("\r\n%-3s %-25s %-7s %-7s %-6s %-7s %-7s %-8s","ID","NAME","CUR_PRI","SET_PRI","SIZE",
                                                                            "CUR","MARGIN","STATE");
    //cs_printf("\r\n-----------------------------------------------------------------------------");
    cs_lst_scan(&display_list , pNode , thread_display_t *)
    {
        cs_printf("\r\n%-3d %-25s %-7d %-7d %-6d %-7d %-7d %s",pNode->info.id,
            pNode->info.name,pNode->info.cur_pri,pNode->info.set_pri,
            pNode->info.stack_size,pNode->info.stack_used,
            pNode->info.stack_size-pNode->info.stack_used,(pNode->info.state == 0)?"RUNNING":
                    (pNode->info.state == 1)?"SLEEPING":
                    (pNode->info.state == 2)?"COUNTSLEEP":
                    (pNode->info.state == 4)?"SUSPENDED":
                    (pNode->info.state == 8)?"CREATING":
                    (pNode->info.state == 16)?"EXITED":
                    (pNode->info.state == 3)?"SLEEPSET":"N/A");
        cs_thread_delay(1);
    }
    //cs_printf("\r\n==============================================================================\r\n");

    return;
}
#else
int cs_thread_create(unsigned int *thread_id,  const char *thread_name,
                     const void *function_pointer, void *param , unsigned int stack_size,
                     unsigned int priority, unsigned int flags)
{
    int					return_code = 0;
    pthread_attr_t		custom_attr;
    struct sched_param	priority_holder;
    int					possible_taskid;
    unsigned int		local_stack_size;
    int					i;

    *thread_id = CS_OSAL_MAX_THREAD;
    if ((thread_name == NULL) || (function_pointer == NULL) || (thread_id == NULL)) {
        cs_printf("\r\n thread create failed , cause some parameter is NULL");
        return CS_E_OSAL_INVALID_POINTER;
    }

    if (strlen(thread_name) > CS_OSAL_MAX_API_NAME) {
        cs_printf("\r\n thread name is too long");
        return CS_E_OSAL_ERR_NAME_TOO_LONG;
    }

    if (priority > CS_OSAL_MAX_PRI) {
        cs_printf("\r\n thread priority is out of range");
        return CS_E_OSAL_ERR_INVALID_PRIORITY;
    }

    pthread_mutex_lock(&osal_thread_table_mut);
    for (possible_taskid = 0; possible_taskid < CS_OSAL_MAX_THREAD; possible_taskid++) {
        if (osal_thread_table[possible_taskid].free == TRUE) {
            break;
        }
    }

    if (possible_taskid >= CS_OSAL_MAX_THREAD) {
        cs_printf("\r\n no free thread can be allocate");
        pthread_mutex_unlock(&osal_thread_table_mut);
        return CS_E_OSAL_ERR_NO_FREE_IDS;
    }

    for (i = 0; i < CS_OSAL_MAX_THREAD; i++) {
        if ((osal_thread_table[i].free == FALSE) &&
                (strcmp((char*) thread_name, osal_thread_table[i].name) == 0)) {
            pthread_mutex_unlock(&osal_thread_table_mut);
            return CS_E_OSAL_ERR_NAME_TAKEN;
        }
    }

    osal_thread_table[possible_taskid].free = FALSE;
    pthread_mutex_unlock(&osal_thread_table_mut);

    if (stack_size < PTHREAD_STACK_MIN)
        local_stack_size = PTHREAD_STACK_MIN;
    else
        local_stack_size = stack_size;

    pthread_attr_init(&custom_attr);
    pthread_attr_setinheritsched(&custom_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setstacksize(&custom_attr, (size_t)local_stack_size);
    priority_holder.sched_priority = CS_OSAL_MAX_PRI - priority;
    pthread_attr_setschedparam(&custom_attr, &priority_holder);
    return_code = pthread_create(&(osal_thread_table[possible_taskid].id),
                                 &custom_attr,
                                 function_pointer,
                                 (void*)param);
    if (return_code != 0) {
        pthread_mutex_lock(&osal_thread_table_mut);
        memset(&osal_thread_table[possible_taskid] , 0 , sizeof(osal_thread_record_t));
        osal_thread_table[possible_taskid].free = TRUE;
        pthread_mutex_unlock(&osal_thread_table_mut);
		cs_printf("\r\n Create thread %s failed.",thread_name);
        return(CS_E_OSAL_ERR);
    }

    *thread_id = possible_taskid;
    pthread_mutex_lock(&osal_thread_table_mut);
    strcpy(osal_thread_table[*thread_id].name, (char*) thread_name);
    osal_thread_table[possible_taskid].stack_size = stack_size;
    osal_thread_table[possible_taskid].priority = CS_OSAL_MAX_PRI - priority;
    pthread_mutex_unlock(&osal_thread_table_mut);

    return CS_E_OSAL_OK;
}

int cs_thread_delay(unsigned int milli_second)
{
    if (milli_second < 10)
        milli_second = 10;
	usleep(milli_second * 1000);
	return CS_E_OSAL_OK;
}

cs_uint32 cs_thread_number()
{
    cs_int32 i;
    cs_uint32 count=0;

    pthread_mutex_lock(&osal_thread_table_mut);
    for(i = 0; i < CS_OSAL_MAX_THREAD; i++)
    {
        if(osal_thread_table[i].free == FALSE)
            count++;
    }
    pthread_mutex_unlock(&osal_thread_table_mut);

    return count;
}

typedef struct
{
    cs_node node;
    cyg_thread_info info;
}thread_display_t;

void cs_thread_show()
{
    cyg_handle_t thread=0;
    cyg_uint16 id=0;
    thread_display_t info[32];
    cs_uint32 thread_num = 0;
    cs_list display_list;
    thread_display_t *pNode = NULL;
    int addflag = 0;

    cs_lst_init(&display_list , NULL);
    memset(&info[0] , 0 , 32*sizeof(cyg_thread_info));
    while( cyg_thread_get_next(&thread, &id))
    {
        if(!cyg_thread_get_info(thread, id, &info[thread_num].info))
            continue;

        addflag = 0;
        cs_lst_scan(&display_list , pNode , thread_display_t *)
        {
            if(pNode->info.set_pri <= info[thread_num].info.set_pri)
                continue;
            cs_lst_insert(&display_list, (cs_node *)cs_lst_prev((cs_node *)pNode), (cs_node *)&info[thread_num]);
            addflag = 1;
            break;
        }

        if(!addflag)
            cs_lst_add(&display_list, (cs_node *)&info[thread_num]);
        thread_num++;
    }

    //cs_printf("\r\n==============================================================================");
    cs_printf("\r\n%-3s %-25s %-7s %-7s %-6s %-7s %-7s %-8s","ID","NAME","CUR_PRI","SET_PRI","SIZE",
                                                                            "CUR","MARGIN","STATE");
    //cs_printf("\r\n-----------------------------------------------------------------------------");
    cs_lst_scan(&display_list , pNode , thread_display_t *)
    {
        cs_printf("\r\n%-3d %-25s %-7d %-7d %-6d %-7d %-7d %s",pNode->info.id,
            pNode->info.name,pNode->info.cur_pri,pNode->info.set_pri,
            pNode->info.stack_size,pNode->info.stack_used,
            pNode->info.stack_size-pNode->info.stack_used,(pNode->info.state == 0)?"RUNNING":
                    (pNode->info.state == 1)?"SLEEPING":
                    (pNode->info.state == 2)?"COUNTSLEEP":
                    (pNode->info.state == 4)?"SUSPENDED":
                    (pNode->info.state == 8)?"CREATING":
                    (pNode->info.state == 16)?"EXITED":
                    (pNode->info.state == 3)?"SLEEPSET":"N/A");
        cs_thread_delay(1);
    }
    //cs_printf("\r\n==============================================================================\r\n");

    return;
}

#endif

/*---------------------------------------------------------------------------------------
   Name: cs_semaphore_init

   Purpose: Creates a counting semaphore with initial value specified by
            sem_initial_value and name specified by sem_name. sem_id will be
            returned to the caller

   Returns: CS_E_OSAL_INVALID_POINTER if sen name or sem_id are NULL
            CS_E_OSAL_ERR_NAME_TOO_LONG if the name given is too long
            CS_E_OSAL_ERR_NO_FREE_IDS if all of the semaphore ids are taken
            CS_E_OSAL_ERR_NAME_TAKEN if this is already the name of a counting semaphore
            CS_E_OSAL_SEM_FAILURE if the OS call failed
            CS_E_OSAL_OK if success


   Notes: options is an unused parameter
---------------------------------------------------------------------------------------*/
#if 0
cs_int32 cs_semaphore_init(cs_uint32 *sem_id, const cs_int8 *sem_name, cs_uint32 sem_initial_value, cs_uint32 options)
{
    /* the current candidate for the new sem id */
    cs_uint32 possible_semid;

    if (sem_id == NULL || sem_name == NULL) {
        osal_printf("\r\n semaphore create failed cause some NULL parameter");
        return CS_E_OSAL_INVALID_POINTER;
    }

    /* we don't want to allow names too long*/
    /* if truncated, two names might be the same */

    if (strlen(sem_name) >= CS_OSAL_MAX_API_NAME) {
        osal_printf("\r\n semaphore name is too long");
        return CS_E_OSAL_ERR_NAME_TOO_LONG;
    }

    /* Check Parameters */
    cyg_mutex_lock(&osal_count_sem_table_mutex);

    for (possible_semid = 0; possible_semid < CS_OSAL_MAX_COUNT_SEM; possible_semid++) {
        if (osal_count_sem_table[possible_semid].free == TRUE)
            break;
    }

    if ((possible_semid >= CS_OSAL_MAX_COUNT_SEM) ||
            (osal_count_sem_table[possible_semid].free != TRUE)) {
        cyg_mutex_unlock(&osal_count_sem_table_mutex);
        osal_printf("\r\n no free semaphore slot");
        return CS_E_OSAL_ERR_NO_FREE_IDS;
    }

#if 0 /*Remove it for new requirement 2009-12-28*/
    /* Check to see if the name is already taken */
    for (i = 0; i < CS_OSAL_MAX_COUNT_SEM; i++) {
        if ((osal_count_sem_table[i].free == FALSE) &&
                strcmp((cs_int8*)sem_name, osal_count_sem_table[i].name) == 0) {
            cyg_mutex_unlock(&osal_count_sem_table_mutex);
            return CS_E_OSAL_ERR_NAME_TAKEN;
        }
    }
#endif

    osal_count_sem_table[possible_semid].free = FALSE;
    cyg_mutex_unlock(&osal_count_sem_table_mutex);

    /* Create VxWorks Semaphore */
    cyg_semaphore_init(&osal_count_sem_table[possible_semid].id,
                       sem_initial_value);

    /* Set the sem_id to the one that we found available */
    /* Set the name of the semaphore,creator and free as well */

    *sem_id = possible_semid;

    cyg_mutex_lock(&osal_count_sem_table_mutex);
    osal_count_sem_table[*sem_id].free = FALSE;
    strcpy(osal_count_sem_table[*sem_id].name , (cs_int8*) sem_name);
    osal_count_sem_table[*sem_id].creator = cs_creator_find();
    cyg_mutex_unlock(&osal_count_sem_table_mutex);


    return CS_E_OSAL_OK;

}


/*--------------------------------------------------------------------------------------
     Name: cs_semaphore_destroy

    Purpose: Deletes the specified Counting Semaphore.

    Returns: CS_E_OSAL_ERR_INVALID_ID if the id passed in is not a valid counting semaphore
             CS_E_OSAL_ERR_SEM_NOT_FULL if the semahore is taken and cannot be deleted
             CS_E_OSAL_SEM_FAILURE the OS call failed
             CS_E_OSAL_OK if success

    Notes: Since we can't delete a semaphore which is currently locked by some task
           (as it may ber crucial to completing the task), the semaphore must be full to
           allow deletion.
---------------------------------------------------------------------------------------*/

cs_int32 cs_semaphore_destroy(cs_uint32 sem_id)
{
    /* Check to see if this sem_id is valid */
    if (sem_id >= CS_OSAL_MAX_COUNT_SEM || osal_count_sem_table[sem_id].free == TRUE) {
        osal_printf("\r\n can not destroy an invalid semaphore");
        return CS_E_OSAL_ERR_INVALID_ID;
    }


    cyg_semaphore_destroy(&osal_count_sem_table[sem_id].id);

    /* Remove the Id from the table, and its name, so that it cannot be found again */
    cyg_mutex_lock(&osal_count_sem_table_mutex);
    osal_count_sem_table[sem_id].free = TRUE;
    strcpy(osal_count_sem_table[sem_id].name , "");
    osal_count_sem_table[sem_id].creator = CS_OSAL_UNINIT;
    memset(&osal_count_sem_table[sem_id].id , 0 , sizeof(cyg_sem_t));
    cyg_mutex_unlock(&osal_count_sem_table_mutex);

    return CS_E_OSAL_OK;

}


/*---------------------------------------------------------------------------------------
    Name: cs_semaphore_post

    Purpose: The function  unlocks the semaphore referenced by sem_id by performing
             a semaphore unlock operation on that semaphore.If the semaphore value
             resulting from this operation is positive, then no threads were blocked
             waiting for the semaphore to become unlocked; the semaphore value is
             simply incremented for this semaphore.


    Returns: CS_E_OSAL_SEM_FAILURE the semaphore was not previously  initialized or is not
             in the array of semaphores defined by the system
             CS_E_OSAL_ERR_INVALID_ID if the id passed in is not a counting semaphore
             CS_E_OSAL_OK if success

---------------------------------------------------------------------------------------*/
cs_int32 cs_semaphore_post(cs_uint32 sem_id)
{
    /* Check Parameters */

    if (sem_id >= CS_OSAL_MAX_COUNT_SEM || osal_count_sem_table[sem_id].free == TRUE) {
        osal_printf("\r\n can not post an invalid semaphore");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    /* Give VxWorks Semaphore */
    cyg_semaphore_post(&osal_count_sem_table[sem_id].id);
    return CS_E_OSAL_OK;

}

/*---------------------------------------------------------------------------------------
    Name:    cs_semaphore_wait

    Purpose: The locks the semaphore referenced by sem_id by performing a
             semaphore lock operation on that semaphore.If the semaphore value
             is currently zero, then the calling thread may have 3 chocies:
             1) shall not return from the call until it either locks the semaphore or the call is
             interrupted by a signal.
             2) return CS_E_OSAL_SEM_FAILURE immediately
             3) wait until timeout

    Return:  CS_E_OSAL_SEM_FAILURE : the semaphore was not previously initialized
             or is not in the array of semaphores defined by the system
             CS_E_OSAL_ERR_INVALID_ID the Id passed in is not a valid countar semaphore
             CS_E_OSAL_SEM_FAILURE if the OS call failed
             CS_E_OSAL_OK if success

    Notes:   A timeout in ticks should be specified.
             Timeouts of WAIT_FOREVER (-1) and NO_WAIT (0) indicate to wait indefinitely or not to wait at all.

----------------------------------------------------------------------------------------*/

cs_int32 cs_semaphore_wait(cs_uint32 sem_id, cs_int32 timeout)
{
    /* msecs rounded to the closest system tick count */
    cs_int32 sys_ticks;

    /* Check Parameters */
    if (sem_id >= CS_OSAL_MAX_COUNT_SEM  || osal_count_sem_table[sem_id].free == TRUE) {
        osal_printf("\r\n wait an invalid semaphore");
        return CS_E_OSAL_ERR_INVALID_ID;
    }
    /* Give VxWorks Semaphore */
    if (CS_OSAL_NO_WAIT == timeout) {
        if (!cyg_semaphore_trywait(&osal_count_sem_table[sem_id].id)) {
            return CS_E_OSAL_ERR;
        }
    } else if (CS_OSAL_WAIT_FOREVER == timeout) {
        cyg_semaphore_wait(&osal_count_sem_table[sem_id].id);
    } else {
        sys_ticks = cs_milli_to_ticks(timeout);

        if (!cyg_semaphore_timed_wait(&osal_count_sem_table[sem_id].id , sys_ticks)) {
            return CS_E_OSAL_ERR;
        }
    }

    return CS_E_OSAL_OK;
}

cs_uint32 cs_semaphore_number()
{
    cs_int32 i;
    cs_uint32 count=0;

    cyg_mutex_lock(&osal_count_sem_table_mutex);
    for(i = 0; i < CS_OSAL_MAX_COUNT_SEM; i++)
    {
        if(osal_count_sem_table[i].free == FALSE)
            count++;
    }
    cyg_mutex_unlock(&osal_count_sem_table_mutex);

    return count;
}
#else
int cs_semaphore_init
	(
    unsigned int 	*sem_id,
    const char 		*sem_name,
    unsigned int 	sem_initial_value,
    unsigned int 	options
	)
{
    unsigned int possible_semid;
    int Status;
    int i;

    if (sem_id == NULL || sem_name == NULL) {
        cs_printf("\r\n semaphore create failed cause some NULL parameter");
        return CS_E_OSAL_INVALID_POINTER;
    }

    if (strlen(sem_name) >= CS_OSAL_MAX_API_NAME) {
        cs_printf("\r\n semaphore name is too long");
        return CS_E_OSAL_ERR_NAME_TOO_LONG;
    }

    pthread_mutex_lock(&osal_count_sem_table_mut);
    for (possible_semid = 0; possible_semid < CS_OSAL_MAX_COUNT_SEM; possible_semid++) {
        if (osal_count_sem_table[possible_semid].free == TRUE)
            break;
    }

    if (possible_semid >= CS_OSAL_MAX_COUNT_SEM) {
        cs_printf("\r\n no free semaphore slot");
        pthread_mutex_unlock(&osal_count_sem_table_mut);
        return CS_E_OSAL_ERR_NO_FREE_IDS;
    }

    for (i = 0; i < CS_OSAL_MAX_COUNT_SEM; i++) {
        if ((osal_count_sem_table[i].free == FALSE) &&
                strcmp((char*) sem_name, osal_count_sem_table[i].name) == 0) {
            pthread_mutex_unlock(&osal_count_sem_table_mut);
            return CS_E_OSAL_ERR_NAME_TAKEN;
        }
    }

    osal_count_sem_table[possible_semid].free = FALSE;
    pthread_mutex_unlock(&osal_count_sem_table_mut);

    Status = sem_init(&(osal_count_sem_table[possible_semid].id) , 0 , sem_initial_value);
    if (Status == -1) {
        pthread_mutex_lock(&osal_count_sem_table_mut);
        memset(&osal_count_sem_table[possible_semid] , 0 , sizeof(osal_count_sem_record_t));
        osal_count_sem_table[possible_semid].free = TRUE;
        pthread_mutex_unlock(&osal_count_sem_table_mut);
        return CS_E_OSAL_ERR;
    }

    *sem_id = possible_semid;
    pthread_mutex_lock(&osal_count_sem_table_mut);
    osal_count_sem_table[*sem_id].free = FALSE;
    strcpy(osal_count_sem_table[*sem_id].name , (char*)sem_name);
    pthread_mutex_unlock(&osal_count_sem_table_mut);

    return CS_E_OSAL_OK;
}

int cs_semaphore_destroy
	(
    unsigned int sem_id
	)
{
    if (sem_id >= CS_OSAL_MAX_COUNT_SEM || osal_count_sem_table[sem_id].free == TRUE) {
        cs_printf("\r\n can not destroy an invalid semaphore");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    if (sem_destroy(&(osal_count_sem_table[sem_id].id)) != 0) {
        cs_printf("\r\n Destroy semaphore(%d) failed", osal_count_sem_table[sem_id].id);
        return CS_E_OSAL_SEM_FAILURE;
    }

    pthread_mutex_lock(&osal_count_sem_table_mut);
    memset(&osal_count_sem_table[sem_id] , 0 , sizeof(osal_count_sem_record_t));
    osal_count_sem_table[sem_id].free = TRUE;
    pthread_mutex_unlock(&osal_count_sem_table_mut);

    return CS_E_OSAL_OK;
}

int cs_semaphore_post
	(
    unsigned int sem_id
	)
{
    int ret_val ;
    int    ret;

    if (sem_id >= CS_OSAL_MAX_COUNT_SEM || osal_count_sem_table[sem_id].free == TRUE) {
        cs_printf("\r\n can not post an invalid semaphore");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    ret = sem_post(&(osal_count_sem_table[sem_id].id));
    if (ret != 0) {
        ret_val = CS_E_OSAL_SEM_FAILURE;
    } else {
        ret_val = CS_E_OSAL_OK;
    }

    return ret_val;
}

int cs_semaphore_wait(unsigned int sem_id, int timeout)
{
    int ret_val ;
    int    ret;
    int timeloop;

    if (sem_id >= CS_OSAL_MAX_COUNT_SEM  || osal_count_sem_table[sem_id].free == TRUE) {
        cs_printf("\r\n wait an invalid semaphore");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    if (CS_OSAL_NO_WAIT == timeout) {
        ret = sem_trywait(&(osal_count_sem_table[sem_id].id));
    } else if (CS_OSAL_WAIT_FOREVER == timeout) {
        ret = sem_wait(&(osal_count_sem_table[sem_id].id));
    } else {
        for (timeloop = timeout; timeloop > 0; timeloop -= 100) {
            if (timeloop >= 100) {
                if (sem_trywait(&(osal_count_sem_table[sem_id].id)) == -1)
                    usleep(100*1000);
                else
                    return CS_E_OSAL_OK;
            } else {
                if (sem_trywait(&(osal_count_sem_table[sem_id].id)) == -1)
                    return CS_E_OSAL_SEM_TIMEOUT;
                else
                    return CS_E_OSAL_OK;
            }
        }
        return CS_E_OSAL_SEM_TIMEOUT;
    }

    if (ret == 0) {
        ret_val = CS_E_OSAL_OK;
    } else {
        ret_val = CS_E_OSAL_ERR;
    }

    return ret_val;
}

unsigned int cs_semaphore_number()
{
    int i;
    unsigned int count = 0;

    pthread_mutex_lock(&osal_count_sem_table_mut);
    for (i = 0; i < CS_OSAL_MAX_COUNT_SEM; i++) {
        if (osal_count_sem_table[i].free == FALSE)
            count++;
    }
    pthread_mutex_unlock(&osal_count_sem_table_mut);

    return count;
}

#endif


/****************************************************************************************
                                  MUTEX API
****************************************************************************************/

/*---------------------------------------------------------------------------------------
    Name: cs_mutex_init

    Purpose: Creates a mutex semaphore initially full.

    Returns: CS_E_OSAL_INVALID_POINTER if sem_id or sem_name are NULL
             CS_E_OSAL_ERR_NAME_TOO_LONG if the sem_name is too long to be stored
             CS_E_OSAL_ERR_NO_FREE_IDS if there are no more free mutex Ids
             CS_E_OSAL_ERR_NAME_TAKEN if there is already a mutex with the same name
             CS_E_OSAL_SEM_FAILURE if the OS call failed
             CS_E_OSAL_OK if success

    Notes: the options parameter is not used in this implementation

---------------------------------------------------------------------------------------*/
#if 0
cs_int32 cs_mutex_init(cs_uint32 *mut_id, const cs_int8 *mut_name, cs_uint32 options)
{
    cs_uint32 possible_semid;

    /* Check Parameters */

    if (mut_id == NULL || mut_name == NULL) {
        osal_printf("\r\n Create mutex failed cause some parameter is NULL");
        return CS_E_OSAL_INVALID_POINTER;
    }

    /* we don't want to allow names too long*/
    /* if truncated, two names might be the same */

    if (strlen(mut_name) >= CS_OSAL_MAX_API_NAME) {
        osal_printf("\r\n Mutex name is too long");
        return CS_E_OSAL_ERR_NAME_TOO_LONG;
    }

    cyg_mutex_lock(&osal_mut_table_mutex);
    for (possible_semid = 0; possible_semid < CS_OSAL_MAX_MUTEX; possible_semid++) {
        if (osal_mut_table[possible_semid].free == TRUE)
            break;
    }

    if ((possible_semid >= CS_OSAL_MAX_MUTEX) ||
            (osal_mut_table[possible_semid].free != TRUE)) {
        osal_printf("\r\n no free mutex slot");
        cyg_mutex_unlock(&osal_mut_table_mutex);
        return CS_E_OSAL_ERR_NO_FREE_IDS;
    }

    osal_mut_table[possible_semid].free = FALSE;
    cyg_mutex_unlock(&osal_mut_table_mutex);

    /* Create ECOS mutex */
    cyg_mutex_init(&osal_mut_table[possible_semid].id);

    *mut_id = possible_semid;
    cyg_mutex_lock(&osal_mut_table_mutex);
    strcpy(osal_mut_table[*mut_id].name, (cs_int8*)mut_name);
    osal_mut_table[*mut_id].free = FALSE;
    osal_mut_table[*mut_id].creator = cs_creator_find();
    cyg_mutex_unlock(&osal_mut_table_mutex);

    return CS_E_OSAL_OK;
}


/*--------------------------------------------------------------------------------------
     Name: cs_mutex_destroy

    Purpose: Deletes the specified Mutex Semaphore.

    Returns: CS_E_OSAL_ERR_INVALID_ID if the id passed in is not a valid mutex
             CS_E_OSAL_ERR_SEM_NOT_FULL if the mutex is empty
             CS_E_OSAL_SEM_FAILURE if the OS call failed
             CS_E_OSAL_OK if success

    Notes: The mutex must be full to take it, so we have to check for fullness

---------------------------------------------------------------------------------------*/

cs_int32 cs_mutex_destroy(cs_uint32 mut_id)
{
    /* Check to see if this mut_id is valid   */
    if (mut_id >= CS_OSAL_MAX_MUTEX || osal_mut_table[mut_id].free == TRUE) {
        osal_printf("\r\n can not destroy an invalid mutex");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    cyg_mutex_destroy(&osal_mut_table[mut_id].id);
    /* Delete its presence in the table */

    cyg_mutex_lock(&osal_mut_table_mutex);
    osal_mut_table[mut_id].free = TRUE;
    memset(&osal_mut_table[mut_id].id , 0 , sizeof(cyg_mutex_t));
    strcpy(osal_mut_table[mut_id].name , "");
    osal_mut_table[mut_id].creator = CS_OSAL_UNINIT;
    cyg_mutex_unlock(&osal_mut_table_mutex);


    return CS_E_OSAL_OK;

}


/*---------------------------------------------------------------------------------------
    Name: cs_mutex_unlock

    Purpose: The function releases the mutex object referenced by sem_id.The
             manner in which a mutex is released is dependent upon the mutex's type
             attribute.  If there are threads blocked on the mutex object referenced by
             mutex when this function is called, resulting in the mutex becoming
             available, the scheduling policy shall determine which thread shall
             acquire the mutex.

    Returns: CS_E_OSAL_OK if success
             CS_E_OSAL_SEM_FAILURE if the semaphore was not previously  initialized
             CS_E_OSAL_ERR_INVALID_ID if the id passed in is not a valid mutex

---------------------------------------------------------------------------------------*/

cs_int32 cs_mutex_unlock(cs_uint32 mut_id)
{
    /* Check Parameters */

    if (mut_id >= CS_OSAL_MAX_MUTEX || osal_mut_table[mut_id].free == TRUE) {
        osal_printf("\r\n unlock an invalid mutex");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    /* Give VxWorks Semaphore */
    cyg_mutex_unlock(&osal_mut_table[mut_id].id);
    return CS_E_OSAL_OK;

}

/*---------------------------------------------------------------------------------------
    Name: cs_mutex_lock

    Purpose: The mutex object referenced by mut_id shall be locked by calling this
             function. If the mutex is already locked, the calling thread shall
             block until the mutex becomes available. This operation shall return
             with the mutex object referenced by mutex in the locked state with the
             calling thread as its owner.

    Returns: CS_E_OSAL_OK if success
             CS_E_OSAL_SEM_FAILURE if the semaphore was not previously initialized or is
             not in the array of semaphores defined by the system
             CS_E_OSAL_ERR_INVALID_ID the id passed in is not a valid mutex
---------------------------------------------------------------------------------------*/
cs_int32 cs_mutex_lock(cs_uint32 mut_id)
{
    /* Check Parameters */

    if (mut_id >= CS_OSAL_MAX_MUTEX || osal_mut_table[mut_id].free == TRUE) {
        osal_printf("\r\n lock an invalid mutex");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    /* Take VxWorks Semaphore */
    cyg_mutex_lock(&osal_mut_table[mut_id].id);

    return CS_E_OSAL_OK;

}


/*---------------------------------------------------------------------------------------
    Name: cs_mutex_trylock

    Purpose: The mutex object referenced by mut_id shall be locked by calling this
             function. If the mutex is already locked, the calling thread will
             always return immediately rather than block, again returning success
             or failure.

    Returns: CS_E_OSAL_OK if success
             CS_E_OSAL_SEM_FAILURE if the semaphore was not previously initialized or is
             not in the array of semaphores defined by the system
             CS_E_OSAL_ERR_INVALID_ID the id passed in is not a valid mutex
---------------------------------------------------------------------------------------*/
cs_int32 cs_mutex_trylock(cs_uint32 mut_id)
{
    /* Check Parameters */

    if (mut_id >= CS_OSAL_MAX_MUTEX || osal_mut_table[mut_id].free == TRUE) {
        osal_printf("\r\n trylock an invalid mutex");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    /* Take VxWorks Semaphore */
    if (!cyg_mutex_trylock(&osal_mut_table[mut_id].id))
        return CS_E_OSAL_ERR;

    return CS_E_OSAL_OK;
}

cs_uint32 cs_mutex_number()
{
    cs_int32 i;
    cs_uint32 count=0;

    cyg_mutex_lock(&osal_mut_table_mutex);
    for(i = 0; i < CS_OSAL_MAX_MUTEX; i++)
    {
        if(osal_mut_table[i].free == FALSE)
            count++;
    }
    cyg_mutex_unlock(&osal_mut_table_mutex);

    return count;
}
#else
int cs_mutex_init(unsigned int *mut_id, const char *mut_name, unsigned int options)
{
    int                 return_code;
    pthread_mutexattr_t mutex_attr ;
    unsigned int        possible_semid;
    int 				i;

    if (mut_id == NULL || mut_name == NULL) {
        cs_printf("\r\n Create mutex failed cause some parameter is NULL");
        return CS_E_OSAL_INVALID_POINTER;
    }

    if (strlen(mut_name) >= CS_OSAL_MAX_API_NAME) {
        cs_printf("\r\n Mutex name is too long");
        return CS_E_OSAL_ERR_NAME_TOO_LONG;
    }

    pthread_mutex_lock(&osal_mut_table_mut);
    for (possible_semid = 0; possible_semid < CS_OSAL_MAX_MUTEX; possible_semid++) {
        if (osal_mut_table[possible_semid].free == TRUE)
            break;
    }

    if (possible_semid >= CS_OSAL_MAX_MUTEX) {
        cs_printf("\r\n no free mutex slot");
        pthread_mutex_unlock(&osal_mut_table_mut);
        return CS_E_OSAL_ERR_NO_FREE_IDS;
    }

    for (i = 0; i < CS_OSAL_MAX_MUTEX; i++) {
        if ((osal_mut_table[i].free == FALSE) &&
                strcmp((char*) mut_name, osal_mut_table[i].name) == 0) {
            pthread_mutex_unlock(&osal_mut_table_mut);
            return CS_E_OSAL_ERR_NAME_TAKEN;
        }
    }

    osal_mut_table[possible_semid].free = FALSE;
    pthread_mutex_unlock(&osal_mut_table_mut);

    pthread_mutexattr_init(&mutex_attr);
    return_code = pthread_mutex_init((pthread_mutex_t *) & osal_mut_table[possible_semid].id, &mutex_attr);
    if (return_code != 0) {
        pthread_mutex_lock(&osal_mut_table_mut);
        memset(&osal_mut_table[possible_semid] , 0 , sizeof(osal_mut_record_t));
        osal_mut_table[possible_semid].free = TRUE;
        pthread_mutex_unlock(&osal_mut_table_mut);
        return CS_E_OSAL_ERR;
    } else {
        *mut_id = possible_semid;
        pthread_mutex_lock(&osal_mut_table_mut);
        strcpy(osal_mut_table[*mut_id].name, (char*) mut_name);
        osal_mut_table[*mut_id].free = FALSE;
        pthread_mutex_unlock(&osal_mut_table_mut);
        return CS_E_OSAL_OK;
    }
}

int cs_mutex_destroy(unsigned int mut_id)
{
    int status = -1;

    if (mut_id >= CS_OSAL_MAX_MUTEX || osal_mut_table[mut_id].free == TRUE) {
        cs_printf("\r\n can not destroy an invalid mutex");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    status = pthread_mutex_destroy(&(osal_mut_table[mut_id].id));
    if (status != 0) {
        cs_printf("\r\n delete mutex failed");
        return CS_E_OSAL_SEM_FAILURE;
    }

    pthread_mutex_lock(&osal_mut_table_mut);
    memset(&osal_mut_table[mut_id] , 0 , sizeof(osal_mut_record_t));
    osal_mut_table[mut_id].free = TRUE;
    pthread_mutex_unlock(&osal_mut_table_mut);

    return CS_E_OSAL_OK;
}

int cs_mutex_unlock(unsigned int mut_id)
{
    int ret_val ;

    if (mut_id >= CS_OSAL_MAX_MUTEX || osal_mut_table[mut_id].free == TRUE) {
        cs_printf("\r\n unlock an invalid mutex");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    if (pthread_mutex_unlock(&(osal_mut_table[mut_id].id))) {
        ret_val = CS_E_OSAL_SEM_FAILURE ;
    } else {
        ret_val = CS_E_OSAL_OK;
    }

    return ret_val;
}

int cs_mutex_lock(unsigned int mut_id)
{
    int ret_val;

    if (mut_id >= CS_OSAL_MAX_MUTEX || osal_mut_table[mut_id].free == TRUE) {
        cs_printf("\r\n lock an invalid mutex");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    if (pthread_mutex_lock(&(osal_mut_table[mut_id].id))) {
        ret_val = CS_E_OSAL_SEM_FAILURE;
    } else {
        ret_val = CS_E_OSAL_OK;
    }

    return ret_val;
}

int cs_mutex_trylock(unsigned int mut_id)
{
    int ret_val;

    if (mut_id >= CS_OSAL_MAX_MUTEX || osal_mut_table[mut_id].free == TRUE) {
        cs_printf("\r\n trylock an invalid mutex");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    if (pthread_mutex_trylock(&(osal_mut_table[mut_id].id))) {
        ret_val = CS_E_OSAL_SEM_FAILURE;
    } else {
        ret_val = CS_E_OSAL_OK;
    }

    return ret_val;
}

cs_uint32 cs_mutex_number()
{
    cs_int32 i;
    cs_uint32 count=0;

    pthread_mutex_lock(&osal_mut_table_mut);
    for(i = 0; i < CS_OSAL_MAX_MUTEX; i++)
    {
        if(osal_mut_table[i].free == FALSE)
            count++;
    }
    pthread_mutex_unlock(&osal_mut_table_mut);

    return count;
}
#endif

/****************************************************************************************
                                MESSAGE QUEUE API
****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: cs_queue_create

   Purpose: Create a message queue which can be refered to by name or ID

   Returns: CS_E_OSAL_INVALID_POINTER if a pointer passed in is NULL
            CS_E_OSAL_ERR_NAME_TOO_LONG if the name passed in is too long
            CS_E_OSAL_ERR_NO_FREE_IDS if there are already the max queues created
            CS_E_OSAL_ERR_NAME_TAKEN if the name is already being used on another queue
            CS_E_OSAL_ERR if the OS create call fails
            CS_E_OSAL_OK if success

   Notes: the flahs parameter is unused.
---------------------------------------------------------------------------------------*/
#if 0
cs_int32 cs_queue_create(cs_uint32 *queue_id, const cs_int8 *queue_name, cs_uint32 queue_depth, cs_uint32 data_size, cs_uint32 flags)
{
    cs_uint32 possible_qid;
    cs_int8 pool_name[16];

    if (queue_id == NULL || queue_name == NULL) {
        osal_printf("\r\n Create queue failed cause some parameter is NULL");
        return CS_E_OSAL_INVALID_POINTER;
    }

    if(queue_depth == 0 || data_size == 0) {
        osal_printf("\r\n Create queue with queue_depth %d and data_size %d",queue_depth,data_size);
        return CS_E_OSAL_ERR;
    }

    /* we don't want to allow names too long*/
    /* if truncated, two names might be the same */
    if (strlen(queue_name) >= CS_OSAL_MAX_API_NAME) {
        osal_printf("\r\n queue name is too long");
        return CS_E_OSAL_ERR_NAME_TOO_LONG;
    }

    /* Check Parameters */
    cyg_mutex_lock(&osal_queue_table_mutex);
    for (possible_qid = 0; possible_qid < CS_OSAL_MAX_QUEUE; possible_qid++) {
        if (osal_queue_table[possible_qid].free == TRUE)
            break;
    }

    if (possible_qid >= CS_OSAL_MAX_QUEUE || osal_queue_table[possible_qid].free != TRUE) {
        osal_printf("\r\n no free queue slot");
        cyg_mutex_unlock(&osal_queue_table_mutex);
        return CS_E_OSAL_ERR_NO_FREE_IDS;
    }

#if 0 /*Remove it for new requirement 2009-12-28*/
    /* Check to see if the name is already taken */
    for (i = 0; i < CS_OSAL_MAX_QUEUE; i++) {
        if ((osal_queue_table[i].free == FALSE) &&
                strcmp((cs_int8*)queue_name, osal_queue_table[i].name) == 0) {
            cyg_mutex_unlock(&osal_queue_table_mutex);
            return CS_E_OSAL_ERR_NAME_TAKEN;
        }
    }
#endif

    osal_queue_table[possible_qid].free = FALSE;
    cyg_mutex_unlock(&osal_queue_table_mutex);
    /* Create VxWorks Message Queue and memory pool */
    if(queue_depth > CYGNUM_KERNEL_SYNCH_MBOX_QUEUE_SIZE)
        queue_depth = CYGNUM_KERNEL_SYNCH_MBOX_QUEUE_SIZE;

    memset(pool_name , 0 , sizeof(pool_name));
    sprintf(pool_name , "queue_p%d",possible_qid);
    if (CS_E_OSAL_OK != cs_mempool_create(&osal_queue_table[possible_qid].mempool_id,
                                            pool_name,
                                            data_size + sizeof(cs_uint32),
                                            queue_depth)) {
        cyg_mutex_lock(&osal_queue_table_mutex);
        osal_queue_table[possible_qid].free = TRUE;
        cyg_mutex_unlock(&osal_queue_table_mutex);
        osal_printf("\r\n Create ecos queue's mempool failed");
        return CS_E_OSAL_ERR;
    }

    cyg_mbox_create(&osal_queue_table[possible_qid].id, &osal_queue_table[possible_qid].mbox);
    /* check if message Q create failed */
    /* Set the queue_id to the id that was found available*/
    /* Set the name of the queue, and the creator as well */
    *queue_id = possible_qid;

    cyg_mutex_lock(&osal_queue_table_mutex);
    osal_queue_table[*queue_id].free = FALSE;
    strcpy(osal_queue_table[*queue_id].name, (cs_int8*) queue_name);
    osal_queue_table[*queue_id].creator = cs_creator_find();
    osal_queue_table[*queue_id].depth = queue_depth;
    osal_queue_table[*queue_id].queue_size = data_size;
    osal_queue_table[*queue_id].queue_type = CS_NORMAL_QUEUE_TYPE;
    cyg_mutex_unlock(&osal_queue_table_mutex);
    return CS_E_OSAL_OK;

}


/*--------------------------------------------------------------------------------------
    Name: cs_queue_delete

    Purpose: Deletes the specified message queue.

    Returns: CS_E_OSAL_ERR_INVALID_ID if the id passed in does not exist
             CS_E_OSAL_ERR if the OS call to delete the queue fails
             CS_E_OSAL_OK if success

    Notes: If There are messages on the queue, they will be lost and any subsequent
           calls to QueueGet or QueuePut to this queue will result in errors
---------------------------------------------------------------------------------------*/

cs_int32 cs_queue_delete(cs_uint32 queue_id)
{
    /* Check to see if the queue_id given is valid */

    if (queue_id >= CS_OSAL_MAX_QUEUE || osal_queue_table[queue_id].free == TRUE ||
                    osal_queue_table[queue_id].queue_type != CS_NORMAL_QUEUE_TYPE) {
        osal_printf("\r\n can not delete an invalid queue");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    /* Try to delete the queue */
    if (CS_E_OSAL_OK != cs_mempool_destroy(osal_queue_table[queue_id].mempool_id)) {
        osal_printf("\r\n destroy queue's mempool failed");
        return CS_E_OSAL_ERR;
    }
    cs_mempool_destroy(osal_queue_table[queue_id].mempool_id);
    cyg_mbox_delete(osal_queue_table[queue_id].id);

    cyg_mutex_lock(&osal_queue_table_mutex);
    osal_queue_table[queue_id].free = TRUE;
    strcpy(osal_queue_table[queue_id].name, "");
    osal_queue_table[queue_id].creator = CS_OSAL_UNINIT;
    osal_queue_table[queue_id].id = 0;
    memset(&osal_queue_table[queue_id].mbox , 0 , sizeof(cyg_mbox));

    cyg_mutex_unlock(&osal_queue_table_mutex);


    return CS_E_OSAL_OK;

}


/*---------------------------------------------------------------------------------------
   Name: cs_queue_get

   Purpose: Receive a message on a message queue.  Will pend or timeout on the receive.
   Returns: CS_E_OSAL_ERR_INVALID_ID if the given ID does not exist
            OSAL_ERR_INVALID_POINTER if a pointer passed in is NULL
            CS_E_OSAL_QUEUE_EMPTY if the Queue has no messages on it to be recieved
            CS_E_OSAL_QUEUE_TIMEOUT if the timeout was OSAL_PEND and the time expired
            CS_E_OSAL_QUEUE_INVALID_SIZE if the size copied from the queue was not correct
            CS_E_OSAL_OK if success
---------------------------------------------------------------------------------------*/

cs_int32 cs_queue_get(cs_uint32 queue_id, void *data, cs_uint32 size, cs_uint32 *size_copied, cs_int32 timeout)
{
    cs_uint8 *buf_ptr = 0;
    cs_uint8 *buf_will_be_cpy = NULL;
    cs_uint32 cpy_len = 0;
    cs_uint32 sys_ticks;

    /* Check Parameters */

    if (queue_id >= CS_OSAL_MAX_QUEUE || osal_queue_table[queue_id].free == TRUE ||
                    osal_queue_table[queue_id].queue_type != CS_NORMAL_QUEUE_TYPE) {
        osal_printf("\r\n can not get msg from an invalid queue");
        return CS_E_OSAL_ERR_INVALID_ID;
    } else {
        if ((data == NULL) || (size_copied == NULL)) {
            osal_printf("\r\n get msg failed cause some parameter is NULL");
            osal_queue_table[queue_id].get_data_error++;
            osal_queue_table[queue_id].get_error++;
            return CS_E_OSAL_INVALID_POINTER;
        }
    }

    /* Get Message From VxWorks Message Queue */

    if (timeout == CS_OSAL_WAIT_FOREVER) {
        buf_ptr = (cs_uint8 *)cyg_mbox_get(osal_queue_table[queue_id].id);
        if(buf_ptr == NULL)
            osal_queue_table[queue_id].get_error++;
    } else {
        if (timeout == CS_OSAL_NO_WAIT) {
            buf_ptr = (cs_uint8 *)cyg_mbox_tryget(osal_queue_table[queue_id].id);
            if(buf_ptr == NULL)
                osal_queue_table[queue_id].get_error++;
        } else {
            sys_ticks = cs_milli_to_ticks(timeout);
            buf_ptr = (cs_uint8 *)cyg_mbox_timed_get(osal_queue_table[queue_id].id , cyg_current_time() + sys_ticks);
            if(buf_ptr == NULL)
            {
                osal_queue_table[queue_id].get_timeout_error++;
                osal_queue_table[queue_id].get_error++;
            }
        }
    }

    if (buf_ptr == NULL) {
        *size_copied = 0;
        return CS_E_OSAL_ERR;
    } else {
        buf_will_be_cpy = (cs_uint8 *)buf_ptr;
        cpy_len = *(cs_uint32 *)buf_will_be_cpy;
        if (cpy_len > size) {
            *size_copied = 0;
            cs_mem_free(buf_will_be_cpy);
            osal_queue_table[queue_id].get_data_long++;
            osal_queue_table[queue_id].get_error++;
            return CS_E_OSAL_ERR;
        }
        memcpy(data , buf_will_be_cpy + sizeof(cs_uint32) , cpy_len);
        *size_copied = cpy_len;
        cs_mem_free(buf_will_be_cpy);
    }

    return CS_E_OSAL_OK;
}


/*---------------------------------------------------------------------------------------
   Name: cs_queue_put

   Purpose: Put a message on a message queue.

   Returns: CS_E_OSAL_ERR_INVALID_ID if the queue id passed in is not a valid queue
            CS_E_OSAL_INVALID_POINTER if the data pointer is NULL
            CS_E_OSAL_QUEUE_FULL if the queue cannot accept another message
            CS_E_OSAL_ERR if the OS call returns an error
            CS_E_OSAL_OK if SUCCESS

   Notes: The flags parameter is not used.  The message put is always configured to
            immediately return an error if the receiving message queue is full.
---------------------------------------------------------------------------------------*/

cs_int32 cs_queue_put(cs_uint32 queue_id, void *data, cs_uint32 size, cs_int32 timeout, cs_uint32 flags)
{
    cs_uint8 *pbuf = NULL;
    cs_uint32 sys_ticks;
    cs_int32 queue_count = CS_OSAL_INVALID_QUEUE;

    /* Check Parameters */

    if (queue_id >= CS_OSAL_MAX_QUEUE || osal_queue_table[queue_id].free == TRUE ||
                    osal_queue_table[queue_id].queue_type != CS_NORMAL_QUEUE_TYPE) {
        osal_printf("\r\n can not put msg into an invalid queue");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    if (data == NULL) {
        osal_printf("\r\n can not put NULL msg into the queue");
        osal_queue_table[queue_id].put_data_error++;
        osal_queue_table[queue_id].put_error++;
        return CS_E_OSAL_INVALID_POINTER;
    }

    /* Send Message to VxWorks Message Queue */

    if(size > osal_queue_table[queue_id].queue_size) {
        osal_printf("\r\n data size is greater than the queue's size");
        osal_queue_table[queue_id].put_data_long++;
        osal_queue_table[queue_id].put_error++;
        return CS_E_OSAL_ERR;
    }

    pbuf = cs_mem_malloc(osal_queue_table[queue_id].mempool_id);
    if (pbuf == NULL) {
        osal_queue_table[queue_id].put_full_error++;
        osal_queue_table[queue_id].put_error++;
        return CS_E_OSAL_ERR;
    }
    memcpy(pbuf + sizeof(cs_uint32), data , size);
    *(cs_uint32 *)pbuf = size;

    if (timeout == CS_OSAL_WAIT_FOREVER) {
        cyg_mbox_put(osal_queue_table[queue_id].id, (void *)pbuf);
    } else {
        if (timeout == CS_OSAL_NO_WAIT) {
            if (!cyg_mbox_tryput(osal_queue_table[queue_id].id, (void *)pbuf)) {
                cs_mem_free(pbuf);
                osal_queue_table[queue_id].put_full_error++;
                osal_queue_table[queue_id].put_error++;
                osal_queue_table[queue_id].peek_value = osal_queue_table[queue_id].depth;
                return CS_E_OSAL_ERR;
            }
        } else {
            sys_ticks = cs_milli_to_ticks(timeout);

            if (!cyg_mbox_timed_put(osal_queue_table[queue_id].id, (void *)pbuf , cyg_current_time() + sys_ticks)) {
                cs_mem_free(pbuf);
                osal_queue_table[queue_id].put_timeout_error++;
                osal_queue_table[queue_id].put_error++;
                osal_queue_table[queue_id].peek_value = osal_queue_table[queue_id].depth;
                return CS_E_OSAL_ERR;
            }
        }
    }


    queue_count = cs_queue_count(queue_id);
    if(osal_queue_table[queue_id].peek_value <= queue_count)
        osal_queue_table[queue_id].peek_value = queue_count;

    return CS_E_OSAL_OK;
}
#else
int cs_queue_create
	(
    unsigned int 	*queue_id,
    const char		*queue_name,
    unsigned int 	queue_depth,
    unsigned int 	data_size,
    unsigned int 	flags
	)
{
    unsigned int possible_qid;
    struct mq_attr queue_attr;
    int i;

    if (queue_id == NULL || queue_name == NULL) {
        cs_printf("\r\n Create queue failed cause some parameter is NULL");
        return CS_E_OSAL_INVALID_POINTER;
    }

    if (queue_depth == 0 || data_size == 0) {
        cs_printf("\r\n Create queue with queue_depth %d and data_size %d", queue_depth, data_size);
        return CS_E_OSAL_ERR;
    }

    if (strlen(queue_name) >= CS_OSAL_MAX_API_NAME) {
        cs_printf("\r\n queue name is too long");
        return CS_E_OSAL_ERR_NAME_TOO_LONG;
    }

    pthread_mutex_lock(&osal_queue_table_mut);
    for (possible_qid = 0; possible_qid < CS_OSAL_MAX_QUEUE; possible_qid++) {
        if (osal_queue_table[possible_qid].free == TRUE)
            break;
    }

    if (possible_qid >= CS_OSAL_MAX_QUEUE) {
        cs_printf("\r\n no free queue slot");
        pthread_mutex_unlock(&osal_queue_table_mut);
        return CS_E_OSAL_ERR_NO_FREE_IDS;
    }

    for (i = 0; i < CS_OSAL_MAX_QUEUE; i++) {
        if ((osal_queue_table[i].free == FALSE) &&
                strcmp((char*)queue_name, osal_queue_table[i].name) == 0) {
            pthread_mutex_unlock(&osal_queue_table_mut);
            return CS_E_OSAL_ERR_NAME_TAKEN;
        }
    }

    osal_queue_table[possible_qid].free = FALSE;

	memset(&queue_attr , 0 ,sizeof(struct mq_attr));
    queue_attr.mq_maxmsg = queue_depth;
    queue_attr.mq_msgsize = data_size;
    osal_queue_table[possible_qid].id = mq_open(queue_name , O_CREAT | O_RDWR , 0777 , &queue_attr);
    if (osal_queue_table[possible_qid].id == NULL) {
        memset(&osal_queue_table[possible_qid] , 0 , sizeof(osal_queue_record_t));
        osal_queue_table[possible_qid].free = TRUE;
        pthread_mutex_unlock(&osal_queue_table_mut);
        return CS_E_OSAL_ERR;
    }
    *queue_id = possible_qid;

    osal_queue_table[*queue_id].free = FALSE;
    strcpy(osal_queue_table[*queue_id].name, (char*) queue_name);
    osal_queue_table[*queue_id].depth = queue_depth;
    osal_queue_table[*queue_id].queue_size = data_size;
	osal_queue_table[*queue_id].queue_type = CS_NORMAL_QUEUE_TYPE;
    pthread_mutex_unlock(&osal_queue_table_mut);
    return CS_E_OSAL_OK;
}

int cs_queue_delete
	(
    unsigned int queue_id
	)
{
    if (queue_id >= CS_OSAL_MAX_QUEUE || osal_queue_table[queue_id].free == TRUE) {
        cs_printf("\r\n can not delete an invalid queue");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    pthread_mutex_lock(&osal_queue_table_mut);

    mq_unlink(osal_queue_table[queue_id].name);
    memset(&osal_queue_table[queue_id] , 0 , sizeof(osal_queue_record_t));
    osal_queue_table[queue_id].free = TRUE;
    pthread_mutex_unlock(&osal_queue_table_mut);

    return CS_E_OSAL_OK;
}

int cs_queue_get
	(
    unsigned int 	queue_id,
    void 			*data,
    unsigned int 	size,
    unsigned int 	*size_copied,
    int 			timeout
	)
{
    unsigned int msg_pri = 0;
    ssize_t recv_len = -1;
    struct timespec tv;

    if (queue_id >= CS_OSAL_MAX_QUEUE || osal_queue_table[queue_id].free == TRUE) {
        cs_printf("\r\n can not get msg from an invalid queue");
        return CS_E_OSAL_ERR_INVALID_ID;
    } else {
        if ((data == NULL) || (size_copied == NULL)) {
            cs_printf("\r\n get msg failed cause some parameter is NULL");
            osal_queue_table[queue_id].get_error++;
            osal_queue_table[queue_id].get_data_error++;
            return CS_E_OSAL_INVALID_POINTER;
        }
    }

    if(timeout != -1)
    {
        cs_uint64 tim = 0;
        tim = cs_current_time();
        tim += timeout/10;
        tv.tv_sec = tim/100;
        tv.tv_nsec = (tim%100)*10*1000*1000;
    }

    if(timeout == CS_OSAL_WAIT_FOREVER)
        recv_len = mq_receive(osal_queue_table[queue_id].id , (char *)data , size , &msg_pri);
    else if(timeout == CS_OSAL_NO_WAIT)
        recv_len = mq_timedreceive(osal_queue_table[queue_id].id , (char *)data , size , &msg_pri, NULL);
    else
        recv_len = mq_timedreceive(osal_queue_table[queue_id].id , (char *)data , size , &msg_pri,&tv);
    
    if (recv_len == -1) {
        osal_queue_table[queue_id].get_error++;
        osal_queue_table[queue_id].get_data_error++;
        return CS_E_OSAL_ERR;
    }
	
    *size_copied = (unsigned int)recv_len;
	if(osal_queue_table[queue_id].cur_num > 0)
		osal_queue_table[queue_id].cur_num--;

    return CS_E_OSAL_OK;
}

int cs_queue_put
	(
    unsigned int	queue_id,
    void 			*data,
    unsigned int 	size,
    int 			timeout,
    unsigned int 	priority
	)
{
    int ret = -1;

    if (queue_id >= CS_OSAL_MAX_QUEUE || osal_queue_table[queue_id].free == TRUE) {
        cs_printf("\r\n can not put msg into an invalid queue");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    if (data == NULL) {
        cs_printf("\r\n can not put NULL msg into the queue");
        osal_queue_table[queue_id].put_error++;
        osal_queue_table[queue_id].put_data_error++;
        return CS_E_OSAL_INVALID_POINTER;
    }

    if (size > osal_queue_table[queue_id].queue_size) {
        cs_printf("\r\n data size is greater than the queue's size");
        osal_queue_table[queue_id].put_error++;
        osal_queue_table[queue_id].put_data_long++;
        return CS_E_OSAL_ERR;
    }

    #if 0
    ret = mq_send(osal_queue_table[queue_id].id , (char *)data , size , priority);
    #else
    ret = mq_timedsend(osal_queue_table[queue_id].id , (char *)data , size , priority,NULL);
    #endif
    if (ret == -1) {
		osal_queue_table[queue_id].put_full_error++;
        return CS_E_OSAL_ERR;
    }

	osal_queue_table[queue_id].cur_num++;
	if(osal_queue_table[queue_id].peek_value <= osal_queue_table[queue_id].cur_num)
        osal_queue_table[queue_id].peek_value = osal_queue_table[queue_id].cur_num;
	
    return CS_E_OSAL_OK;
}

#endif

void cs_queue_show_specific(cs_uint32 qid)
{
    osal_queue_record_t *pQueue=NULL;

    if(qid >= CS_OSAL_MAX_QUEUE)
        return;

    pQueue = &osal_queue_table[qid];
    if(pQueue->free == TRUE || pQueue->queue_type != CS_NORMAL_QUEUE_TYPE)
        return;

    cs_printf("\r\n%-3d %-16s %-5d %-4d %-5d %-5d %-8d",qid,pQueue->name,pQueue->depth,pQueue->queue_size,
                                            cs_queue_count(qid),pQueue->peek_value,pQueue->put_full_error);
    return;
}

void cs_queue_show(cs_uint32 queue_id)
{
    cs_int32 qid = 0;

    cs_printf("\r\n====================================================");
    cs_printf("\r\n%-3s %-16s %-5s %-4s %-5s %-5s %-8s","Id","Name","Depth","Size","Used","Peek","PutFail");
    cs_printf("\r\n----------------------------------------------------");
    if(CS_OSAL_INVALID_QUEUE == queue_id)
    {
        for(qid = 0 ; qid < CS_OSAL_MAX_QUEUE ; qid++)
        {
            if(osal_queue_table[qid].free == TRUE || osal_queue_table[qid].queue_type != CS_NORMAL_QUEUE_TYPE)
                continue;
            cs_queue_show_specific(qid);
        }
        cs_printf("\r\n====================================================");
    }
    else
    {
        qid = queue_id;
        if(osal_queue_table[qid].free != TRUE && osal_queue_table[qid].queue_type == CS_NORMAL_QUEUE_TYPE)
        {
            cs_queue_show_specific(qid);
            cs_printf("\r\n====================================================");
            cs_printf("\r\n>Details:");
            cs_printf("\r\n Mempool Id      : %d",osal_queue_table[qid].mempool_id);
            cs_printf("\r\n Mempool Name    : %s",cs_pool_name_get(osal_queue_table[qid].mempool_id));
            cs_printf("\r\n Put Error       : %d",osal_queue_table[qid].put_error);
            cs_printf("\r\n   Full          : %d",osal_queue_table[qid].put_full_error);
            cs_printf("\r\n   Timeout       : %d",osal_queue_table[qid].put_timeout_error);
            cs_printf("\r\n   Data-is-NULL  : %d",osal_queue_table[qid].put_data_error);
            cs_printf("\r\n   Size-too-long : %d",osal_queue_table[qid].put_data_long);
            cs_printf("\r\n Get Error       : %d",osal_queue_table[qid].get_error);
            cs_printf("\r\n   Timeout       : %d",osal_queue_table[qid].get_timeout_error);
            cs_printf("\r\n   Data-is-NULL  : %d",osal_queue_table[qid].get_data_error);
            cs_printf("\r\n   Size-too-long : %d",osal_queue_table[qid].get_data_long);
        }
    }
    cs_printf("\r\n");
    return;
}

cs_uint32 cs_queue_number()
{
    cs_int32 i;
    cs_uint32 count=0;

    pthread_mutex_lock(&osal_queue_table_mut);
    for(i = 0; i < CS_OSAL_MAX_QUEUE; i++)
    {
        if(osal_queue_table[i].free == FALSE && osal_queue_table[i].queue_type == CS_NORMAL_QUEUE_TYPE)
            count++;
    }
    pthread_mutex_unlock(&osal_queue_table_mut);

    return count;
}


/*---------------------------------------------------------------------------------------
    Name: cs_queue_count

    Purpose: This function will pass back a pointer to structure that contains
             all of the relevant info (name and creator) about the specified queue.

    Returns: CS_E_OSAL_INVALID_POINTER if queue_prop is NULL
             CS_E_OSAL_ERR_INVALID_ID if the ID given is not  a valid queue
             CS_E_OSAL_OK if the info was copied over correctly
---------------------------------------------------------------------------------------*/

cs_int32 cs_queue_count(cs_uint32 queue_id)
{
    /* Check to see that the id given is valid */
    cs_int32 count = 0;

    if (queue_id >= CS_OSAL_MAX_QUEUE || osal_queue_table[queue_id].free == TRUE ||
                osal_queue_table[queue_id].queue_type != CS_NORMAL_QUEUE_TYPE)
        return CS_OSAL_INVALID_QUEUE;

    count = osal_queue_table[queue_id].cur_num;

    return count;

}

cs_int32 cs_queue_type_get(cs_uint32 queue_id)
{
    if (queue_id >= CS_OSAL_MAX_QUEUE || osal_queue_table[queue_id].free == TRUE)
        return CS_NORMAL_QUEUE_TYPE;

    return osal_queue_table[queue_id].queue_type;
}

/****************************************************************************************
                                PRIORITY MESSAGE QUEUE API
****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: cs_pri_queue_create

   Purpose: Create a priority message queue which can be refered to by name or ID

   Returns:
---------------------------------------------------------------------------------------*/

cs_int32 cs_pri_queue_create (cs_uint32 *queue_id, const cs_int8 *queue_name, cs_uint32 queue_depth, cs_uint32 data_size, cs_int32 pri_num)
{
    cs_uint32 possible_qid;
    cs_int8 pri_queue_name[CS_OSAL_MAX_API_NAME];
    cs_int8 pri_queue_sem_name[CS_OSAL_MAX_API_NAME];
    cs_int32 i;

    if ( queue_id == NULL || queue_name == NULL) {
        osal_printf("\r\n Create pri queue failed cause some parameter is NULL");
        return CS_E_OSAL_INVALID_POINTER;
    }

    if(queue_depth == 0 || data_size == 0) {
        osal_printf("\r\n Create pri queue with queue_depth %d and data_size %d",queue_depth,data_size);
        return CS_E_OSAL_ERR;
    }

    if(pri_num <= 0 || pri_num > CS_OSAL_MAX_QUEUE_PRI)
    {
        osal_printf("\r\n priority is out of range");
        return CS_E_OSAL_ERR;
    }

    /* we don't want to allow names too long*/
    /* if truncated, two names might be the same */

    if (strlen(queue_name) >= CS_OSAL_MAX_API_NAME) {
        osal_printf("\r\n pri queue name is too long");
        return CS_E_OSAL_ERR_NAME_TOO_LONG;
    }

   /* Check Parameters */

   pthread_mutex_lock(&osal_queue_table_mut);
    for(possible_qid = 0; possible_qid < CS_OSAL_MAX_QUEUE; possible_qid++)
    {
        if (osal_queue_table[possible_qid].free == TRUE)
            break;
    }

    if( possible_qid >= CS_OSAL_MAX_QUEUE || osal_queue_table[possible_qid].free != TRUE)
    {
        osal_printf("\r\n no free queue slot");
        pthread_mutex_unlock(&osal_queue_table_mut);
        return CS_E_OSAL_ERR_NO_FREE_IDS;
    }

    memset(pri_queue_name , 0 , sizeof(pri_queue_name));
    snprintf(pri_queue_name , CS_OSAL_MAX_API_NAME, "pq_%s" , queue_name);
    if(CS_E_OSAL_OK != cs_mempool_create(
                                            &osal_queue_table[possible_qid].mempool_id,
                                            pri_queue_name,
                                            data_size+sizeof(cs_node)+sizeof(cs_uint32),
                                            queue_depth*pri_num))
    {
        osal_printf("\r\n create queue memory pool failed");
        pthread_mutex_unlock(&osal_queue_table_mut);
        return CS_E_OSAL_ERR;
    }

    memset(pri_queue_sem_name , 0 , sizeof(pri_queue_sem_name));
    snprintf(pri_queue_sem_name , CS_OSAL_MAX_API_NAME, "pq_sem_%d",possible_qid);
    if(CS_E_OSAL_OK != cs_semaphore_init(&osal_queue_table[possible_qid].cnt_sem_id, pri_queue_sem_name, 0,0))
    {
        cs_mempool_destroy(osal_queue_table[possible_qid].mempool_id);
        pthread_mutex_unlock(&osal_queue_table_mut);
        return CS_E_OSAL_ERR;
    }

    for(i = 0 ; i < pri_num ; i++)
    {
        cs_int8 mutex_name[CS_OSAL_MAX_API_NAME];
        cs_lst_init(&osal_queue_table[possible_qid].queue_list[i], NULL);
        memset(mutex_name , 0 , sizeof(mutex_name));
        snprintf(mutex_name ,CS_OSAL_MAX_API_NAME,  "mx_%s_%d",queue_name , i);
        cs_mutex_init(&osal_queue_table[possible_qid].queue_mutex[i], mutex_name , 0);
    }
    osal_queue_table[possible_qid].free = FALSE;
    pthread_mutex_unlock(&osal_queue_table_mut);

    /* Create VxWorks Message Queue */

    osal_queue_table[possible_qid].id = (mqd_t)possible_qid;
    osal_queue_table[possible_qid].pri_num = pri_num;

    /* Set the queue_id to the id that was found available*/
    /* Set the name of the queue, and the creator as well */

    *queue_id = possible_qid;

    pthread_mutex_lock(&osal_queue_table_mut);

    osal_queue_table[*queue_id].free = FALSE;
    strcpy( osal_queue_table[*queue_id].name, (cs_int8*) queue_name);
    osal_queue_table[*queue_id].creator = cs_creator_find();
    osal_queue_table[*queue_id].queue_size = data_size;
    osal_queue_table[*queue_id].depth = queue_depth;
    osal_queue_table[*queue_id].queue_type = CS_PRI_QUEUE_TYPE;
    pthread_mutex_unlock(&osal_queue_table_mut);

    return CS_E_OSAL_OK;

}

cs_int32 cs_pri_queue_put (cs_uint32 queue_id, void *data, cs_uint32 size, cs_int32 timeout, cs_int32 priority)
{
    cs_uint8 *pbuf = NULL;
    cs_uint32 total = 0;

    /* Check Parameters */

    if(queue_id >= CS_OSAL_MAX_QUEUE || osal_queue_table[queue_id].free == TRUE ||
                            osal_queue_table[queue_id].queue_type != CS_PRI_QUEUE_TYPE) {
        osal_printf("\r\n can not put msg into an invalid queue");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    if (data == NULL) {
        osal_printf("\r\n can not put NULL msg into the queue");
        osal_queue_table[queue_id].put_data_error++;
        osal_queue_table[queue_id].put_error++;
        return CS_E_OSAL_INVALID_POINTER;
    }

    if(size > osal_queue_table[queue_id].queue_size) {
        osal_printf("\r\n data size is greater than the queue's size");
        osal_queue_table[queue_id].put_data_long++;
        osal_queue_table[queue_id].put_error++;
        return CS_E_OSAL_ERR;
    }

    if(priority < 0 || priority >= osal_queue_table[queue_id].pri_num)
    {
        osal_queue_table[queue_id].put_error++;
        return CS_E_OSAL_ERR;
    }

    if(cs_lst_count(&osal_queue_table[queue_id].queue_list[priority]) >= osal_queue_table[queue_id].depth)
    {
        osal_queue_table[queue_id].put_full_error++;
        osal_queue_table[queue_id].put_error++;
        return CS_E_OSAL_ERR;
    }

    pbuf = cs_mem_malloc(osal_queue_table[queue_id].mempool_id);
    if(pbuf == NULL)
    {
        osal_queue_table[queue_id].put_error++;
        return CS_E_OSAL_ERR;
    }
/*
    --------------------------------------------------
    |  cs_node(8bytes)    | size(4bytes)|         data('size'  bytes)  |
    --------------------------------------------------
*/
    *((cs_uint32 *)(pbuf+sizeof(cs_node))) = size;
    memcpy(pbuf + sizeof(cs_node)+sizeof(cs_uint32) , data , size);

    cs_mutex_lock(osal_queue_table[queue_id].queue_mutex[priority]);

    cs_lst_add(&osal_queue_table[queue_id].queue_list[priority] , (cs_node *)pbuf);

    if(osal_queue_table[queue_id].pri_queue_peek[priority] <
        cs_lst_count(&osal_queue_table[queue_id].queue_list[priority]))
        osal_queue_table[queue_id].pri_queue_peek[priority] = cs_lst_count(&osal_queue_table[queue_id].queue_list[priority]);

    total = cs_pri_queue_count(queue_id);
    if(total > osal_queue_table[queue_id].peek_value)
        osal_queue_table[queue_id].peek_value = total;

    cs_mutex_unlock(osal_queue_table[queue_id].queue_mutex[priority]);

    cs_semaphore_post(osal_queue_table[queue_id].cnt_sem_id);

    return CS_E_OSAL_OK;
}


cs_int32 cs_pri_queue_get (cs_uint32 queue_id, void *data, cs_uint32 size, cs_uint32 *size_copied, cs_int32 timeout)
{
    cs_uint8 *pbuf = NULL;
    cs_int32 ret = CS_E_OSAL_ERR;
    cs_int32 i;

    /* Check Parameters */

    if(queue_id >= CS_OSAL_MAX_QUEUE || osal_queue_table[queue_id].free == TRUE ||
                        osal_queue_table[queue_id].queue_type != CS_PRI_QUEUE_TYPE) {
        osal_printf("\r\n can not put msg into an invalid queue");
        return CS_E_OSAL_ERR_INVALID_ID;
    }

    if (data == NULL) {
        osal_printf("\r\n can not put NULL msg into the queue");
        osal_queue_table[queue_id].get_data_error++;
        osal_queue_table[queue_id].get_error++;
        return CS_E_OSAL_INVALID_POINTER;
    }

    ret = cs_semaphore_wait(osal_queue_table[queue_id].cnt_sem_id, timeout);
    if(ret != CS_E_OSAL_OK)
    {
        osal_queue_table[queue_id].get_error++;
        return CS_E_OSAL_ERR;
    }

    for(i = osal_queue_table[queue_id].pri_num -1 ; i >= 0 ; i--)
    {
        if(cs_lst_count(&osal_queue_table[queue_id].queue_list[i]) == 0)
            continue;

        cs_mutex_lock(osal_queue_table[queue_id].queue_mutex[i]);
        pbuf = (cs_uint8 *)cs_lst_get(&osal_queue_table[queue_id].queue_list[i]);
        cs_mutex_unlock(osal_queue_table[queue_id].queue_mutex[i]);

        *size_copied = *((int *)(pbuf + sizeof(cs_node)));
        if(*size_copied > size)
        {
            osal_queue_table[queue_id].get_data_long++;
            osal_queue_table[queue_id].get_error++;
            return CS_E_OSAL_ERR;
        }
        memcpy(data , pbuf + sizeof(cs_node)+sizeof(cs_uint32) , *size_copied);
        cs_mem_free(pbuf);

        return CS_E_OSAL_OK;
    }

    osal_queue_table[queue_id].get_error++;
    return CS_E_OSAL_ERR;
}

cs_uint32 cs_pri_queue_number()
{
    cs_int32 i;
    cs_uint32 count=0;

    pthread_mutex_lock(&osal_queue_table_mut);
    for(i = 0; i < CS_OSAL_MAX_QUEUE; i++)
    {
        if(osal_queue_table[i].free == FALSE && osal_queue_table[i].queue_type == CS_PRI_QUEUE_TYPE)
            count++;
    }
    pthread_mutex_unlock(&osal_queue_table_mut);

    return count;
}

cs_int32 cs_pri_queue_count (cs_uint32 queue_id)
{
    /* Check to see that the id given is valid */
    cs_int32 count = 0;
    cs_int32 i;

    if (queue_id >= CS_OSAL_MAX_QUEUE || osal_queue_table[queue_id].free == TRUE ||
                            osal_queue_table[queue_id].queue_type != CS_PRI_QUEUE_TYPE)
    {
        return CS_OSAL_INVALID_QUEUE;
    }

    for(i = 0 ; i < osal_queue_table[queue_id].pri_num ; i++)
    {
        count += cs_lst_count(&osal_queue_table[queue_id].queue_list[i]);
    }

    return count;
}

cs_int32 cs_pri_queue_max_priority(cs_uint32 queue_id)
{
    if (queue_id >= CS_OSAL_MAX_QUEUE || osal_queue_table[queue_id].free == TRUE ||
                            osal_queue_table[queue_id].queue_type != CS_PRI_QUEUE_TYPE)
    {
        return 0;
    }

    return osal_queue_table[queue_id].pri_num-1;
}

void cs_pri_queue_show(cs_uint32 queue_id)
{
    cs_int32 i;
    osal_queue_record_t *pPriQueue = NULL;


    cs_printf("\r\n=============================================================");
    cs_printf("\r\n%-3s %-16s %-5s %-4s %-7s %-7s %-5s %-10s","Id","Name","Depth","Size","Pri-Cnt","Current","Peek","PutFail");
    cs_printf("\r\n-------------------------------------------------------------");
    if(queue_id == CS_OSAL_INVALID_QUEUE)
    {
        for(i = 0 ; i < CS_OSAL_MAX_QUEUE ; i++)
        {
            pPriQueue = &osal_queue_table[i];
            if(pPriQueue->free == TRUE || pPriQueue->queue_type != CS_PRI_QUEUE_TYPE)
                continue;
            cs_printf("\r\n%-3d %-16s %-5d %-4d %-7d %-7d %-5d %-10d",
                                    i , pPriQueue->name,pPriQueue->depth,pPriQueue->queue_size , pPriQueue->pri_num,
                                    cs_pri_queue_count(i),pPriQueue->peek_value,pPriQueue->put_error);
        }

    }
    else
    {
        pPriQueue = &osal_queue_table[queue_id];
        if(pPriQueue->free == TRUE || pPriQueue->queue_type != CS_PRI_QUEUE_TYPE)
            return;

        cs_printf("\r\n%-3d %-16s %-5d %-4d %-7d %-7d %-5d %-10d",
                                    queue_id , pPriQueue->name,pPriQueue->depth,pPriQueue->queue_size , pPriQueue->pri_num,
                                    cs_pri_queue_count(queue_id),pPriQueue->peek_value,pPriQueue->put_error);
        for(i = 0 ; i < pPriQueue->pri_num ; i++)
        {
            cs_printf("\r\n%-3s %-16s %-5s %-4s :%-6d %-7d %-5d %-10s",
                                "","","","",i,cs_lst_count(&pPriQueue->queue_list[i]),
                                pPriQueue->pri_queue_peek[i],"");
        }
    }
    cs_printf("\r\n=============================================================\r\n");
}


/****************************************************************************************
                                    INFO API
****************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: cs_current_time

   Purpose: This function returns the system time in tick.
---------------------------------------------------------------------------------------*/

cs_uint64 cs_current_time(void)
{
    return (cs_uint64) cyg_current_time();
}

void cs_timestamp_print()
{
	cs_uint64 current_time = 0;
	
	current_time = cs_current_time();
	cs_printf("[%12lld]", current_time*10);
}

/*---------------------------------------------------------------------------------------
   Name: cs_milli_to_ticks

   Purpose: This function accepts a time interval in milli_seconds as input an
            returns the tick equivalent is o.s. system clock ticks. The tick
            value is rounded up.  This algorthim should change to use a integer divide.
---------------------------------------------------------------------------------------*/

cs_int32 cs_milli_to_ticks(cs_uint32 milli_seconds)
{
#if 0
    cs_uint32 sys_ticks = milli_seconds / 10;

    if(0 == sys_ticks)
        sys_ticks = 1;

    return sys_ticks;
#else
    return milli_seconds;
#endif
    /*
     * this function can be modified - it gives a good approx without any
     * floating point (">>10" ~= "/1000")
    */
}


/*---------------------------------------------------------------------------------------
   Name: cs_tick_to_micros

   Purpose: This function returns the duration of a system tick in micro seconds.
---------------------------------------------------------------------------------------*/

cs_int32 cs_tick_to_micros()
{
    return 1000;

}


/*---------------------------------------------------------------------------------------
 * Name: cs_local_time_get
 *
 * Purpose: This functions get the local time of the machine its on
 * ------------------------------------------------------------------------------------*/
cs_int32 cs_local_time_get(osal_time_t *time_struct)
{
#if 0
    cs_int32 status;
    struct  timespec  time;

    if (time_struct == NULL)
        return CS_E_OSAL_INVALID_POINTER;

    status = clock_gettime(CLOCK_REALTIME, &time);
    if (status != OK)
        return CS_E_OSAL_ERR;

    time_struct->seconds = time.tv_sec;
    time_struct->microsecs = time.tv_nsec / 1000;
#endif

    return CS_E_OSAL_OK;
}

/*---------------------------------------------------------------------------------------
 * Name: cs_local_time_set
 *
 * Purpose: This function sets the local time of the machine its on
 * ------------------------------------------------------------------------------------*/

cs_int32 cs_local_time_set(osal_time_t *time_struct)
{

#if 0
    cs_int32 status;

    struct  timespec  time;

    if (time_struct == NULL)
        return CS_E_OSAL_INVALID_POINTER;

    time.tv_sec = time_struct->seconds;
    time.tv_nsec = (time_struct->microsecs) * 1000;

    status = clock_settime(CLOCK_REALTIME, &time);
    if (status != 0)
        return CS_E_OSAL_ERR;
#endif

    return CS_E_OSAL_OK;
}

/*---------------------------------------------------------------------------------------
 *  Name: cs_err_name_get()
---------------------------------------------------------------------------------------*/
#if 0
cs_int32 cs_err_name_get(cs_int32 error_num, osal_err_name_t * err_name)
{
    osal_err_name_t local_name;
    cs_uint32 return_code;

    return_code = CS_E_OSAL_OK;

    if(err_name == NULL)
        return return_code;
    switch (error_num) {
    case CS_E_OSAL_OK:
        strcpy(local_name, "CS_E_OSAL_OK");
        break;
    case CS_E_OSAL_ERR:
        strcpy(local_name, "CS_E_OSAL_ERR");
        break;
    case CS_E_OSAL_INVALID_POINTER:
        strcpy(local_name, "CS_E_OSAL_INVALID_POINTER");
        break;
    case CS_E_OSAL_ERR_ADDR_MISALIGNED:
        strcpy(local_name, "OSAL_ADDRESS_MISALIGNED");
        break;
    case CS_E_OSAL_ERR_TIMEOUT:
        strcpy(local_name, "CS_E_OSAL_ERR_TIMEOUT");
        break;
    case CS_E_OSAL_INVALID_INT_NUM:
        strcpy(local_name, "CS_E_OSAL_INVALID_INT_NUM");
        break;
    case CS_E_OSAL_SEM_FAILURE:
        strcpy(local_name, "CS_E_OSAL_SEM_FAILURE");
        break;
    case CS_E_OSAL_SEM_TIMEOUT:
        strcpy(local_name, "CS_E_OSAL_SEM_TIMEOUT");
        break;
    case CS_E_OSAL_QUEUE_EMPTY:
        strcpy(local_name, "CS_E_OSAL_QUEUE_EMPTY");
        break;
    case CS_E_OSAL_QUEUE_FULL:
        strcpy(local_name, "CS_E_OSAL_QUEUE_FULL");
        break;
    case CS_E_OSAL_QUEUE_TIMEOUT:
        strcpy(local_name, "CS_E_OSAL_QUEUE_TIMEOUT");
        break;
    case CS_E_OSAL_QUEUE_INVALID_SIZE:
        strcpy(local_name, "CS_E_OSAL_QUEUE_INVALID_SIZE");
        break;
    case CS_E_OSAL_QUEUE_ID_ERROR:
        strcpy(local_name, "CS_E_OSAL_QUEUE_ID_ERROR");
        break;
    case CS_E_OSAL_ERR_NAME_TOO_LONG:
        strcpy(local_name, "CS_E_OSAL_ERR_NAME_TOO_LONG");
        break;
    case CS_E_OSAL_ERR_NO_FREE_IDS:
        strcpy(local_name, "CS_E_OSAL_ERR_NO_FREE_IDS");
        break;
    case CS_E_OSAL_ERR_NAME_TAKEN:
        strcpy(local_name, "CS_E_OSAL_ERR_NAME_TAKEN");
        break;
    case CS_E_OSAL_ERR_INVALID_ID:
        strcpy(local_name, "CS_E_OSAL_ERR_INVALID_ID");
        break;
    case CS_E_OSAL_ERR_NAME_NOT_FOUND:
        strcpy(local_name, "CS_E_OSAL_ERR_NAME_NOT_FOUND");
        break;
    case CS_E_OSAL_ERR_SEM_NOT_FULL:
        strcpy(local_name, "CS_E_OSAL_ERR_SEM_NOT_FULL");
        break;
    case CS_E_OSAL_ERR_INVALID_PRIORITY:
        strcpy(local_name, "CS_E_OSAL_ERR_INVALID_PRIORITY");
        break;

    default:
        strcpy(local_name, "ERROR_UNKNOWN");
        return_code = CS_E_OSAL_ERR;
    }

    strcpy((cs_int8*) err_name, local_name);


    return return_code;
}
#endif
/*---------------------------------------------------------------------------------------
 * cs_creator_find()
 *  This function will return the OSAL ID of the task that created the calling function;
 *  This is an internal call, not to be used by the user.
---------------------------------------------------------------------------------------*/
cs_uint32 cs_creator_find()
{
    cyg_handle_t thread_id;
    cs_int32 i;
    thread_id = cyg_thread_self();

    for (i = 0; i < CS_OSAL_MAX_THREAD; i++) {
        if (thread_id == osal_thread_table[i].id)
            break;
    }

    return i;
}


/* ---------------------------------------------------------------------------
 * Name: cs_printf
 *
 * Purpose: This function abstracts out the printf type statements. This is
 *          useful for using OS- specific thats that will allow non-polled
 *          print statements for the real time systems.
 *
 * Note:    This function uses a utility task that gets passed the print
 *          messages on a queue. This allows that task to block (if
 *          necessary), so the calling task does not.
 *
 ---------------------------------------------------------------------------*/

void cs_printf(const cs_int8 *String, ...)
{
    va_list ap;
    int ret;
	
    extern int diag_vprintf(const char *fmt, va_list ap);

    va_start(ap, String);
    ret = diag_vprintf(String, ap);
    va_end(ap);

    return;
}

cs_uint32 cs_memory_usage()
{
    extern struct mallinfo mallinfo( void );
    struct mallinfo minfo;
    minfo = mallinfo();

    return (cs_uint32)(((minfo.arena - minfo.maxfree) *100)/minfo.arena);
}

#if 1
extern int tolower( int );
extern void (*_putc)(char c, void **param);
extern int _vprintf(void (*putc)(char c, void **param), void **param, const char *fmt, va_list ap);
extern bool mon_read_char_timeout(char *c);

void halt_do_help()
{
    cs_printf("\r\n%-5s- %-48s","d","display halt string");
    cs_printf("\r\n%-5s- %-48s","q","go on...");
    cs_printf("\r\n%-5s- %-48s","r","reset system");
}

cs_int32 halt_flag = 0;
cs_int32 is_halt = 0;
extern cyg_bool imst_getc_nonblock(cyg_uint8* ch);
cs_int32 cs_halt(cs_int8 *String,...)
{
    va_list ap;
    //int ret;
    char c;
    cs_int8 *halt_prompt = "System-Halt->";
    char reason[256];

    if(halt_flag)
        return 1;

    is_halt = 1;
    cs_printf("\r\n***************************************");
    cs_printf("\r\n*                                     *");
    cs_printf("\r\n*         WARNING:System HALT!        *");
    cs_printf("\r\n*                                     *");
    cs_printf("\r\n***************************************");
    cs_printf("\r\n\r\nHalt Reason: ");
    memset(reason , 0 ,sizeof(reason));
    va_start(ap, String);
    vsprintf(reason , String, ap);
    va_end(ap);
    cs_printf("%s",reason);
    cs_printf("\n%s", halt_prompt);
    while (1)
    {
        c = 0;
        #if 0
        if(!mon_read_char_timeout(&c))
            continue;
        #else
        if(!imst_getc_nonblock(&c))
            continue;
        #endif

        switch(tolower(c))
        {
            case 'd':
                cs_printf("\r\n\r\nHalt Reason: ");
                cs_printf("%s\n",reason);
                break;
            case 'r':
                HAL_PLATFORM_RESET();
                break;
            case 'q':
                is_halt = 0;
                return 1;
            default:
                halt_do_help();
                break;
        }

        cs_printf("\n%s", halt_prompt);
    }

    return 1;
}

#endif

