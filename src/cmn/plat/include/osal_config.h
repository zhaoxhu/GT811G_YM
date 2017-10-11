/***********************************************************************/
/* This file contains unpublished documentation and software                                            */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,                              */
/* in whole or in part, of the information in this file without a                                              */
/* written consent of an officer of Cortina Systems Incorporated is                                     */
/* strictly prohibited.                                                                                                             */
/* Copyright (c) 2002-2010 by Cortina Systems Incorporated.                                            */
/***********************************************************************/

#ifndef _osal_confige_h_
#define _osal_confige_h_

/*
** Platform Configuration Parameters for the OS API
*/
#define CS_OSAL_MAX_THREAD              32
#define CS_OSAL_MAX_COUNT_SEM           50
#define CS_OSAL_MAX_PRI_QUEUE           2
#define CS_OSAL_MAX_QUEUE               10
#define CS_OSAL_MAX_MEMPOOL             (20+CS_OSAL_MAX_QUEUE)
#define CS_OSAL_MAX_MUTEX               (30+CS_OSAL_MAX_MEMPOOL)

#define CS_NORMAL_QUEUE_TYPE 0
#define CS_PRI_QUEUE_TYPE 1
/*
** Maximum number for timer
*/
#define CS_OSAL_MAX_TIMER               128

/*
** Maximum length for an absolute path name
*/
#define CS_OSAL_MAX_PATH_LEN     64

/*
** The maxium length allowed for a object (task,queue....) name
*/
#define CS_OSAL_MAX_API_NAME     32

#define CS_OSAL_MAX_QUEUE_PRI  8
/*
** The maximum length for a file name
*/
#define CS_OSAL_MAX_FILE_NAME    20

/*
** These defines are for OS_printf
*/
#define CS_OSAL_BUFFER_SIZE         172
#define CS_OSAL_BUFFER_MSG_DEPTH    100



#endif /* _osal_confige_h_ */

