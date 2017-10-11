
/***********************************************************************/
/* This file contains unpublished documentation and software           */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
/* in whole or in part, of the information in this file without a      */
/* written consent of an officer of Cortina Systems Incorporated is    */
/* strictly prohibited.                                                */
/* Copyright (c) 2002 - 2010 by Cortina Systems Incorporated.          */
/***********************************************************************/

#ifndef __CS_CONFIG_H__
#define __CS_CONFIG_H__

#include "iros_config.h"



#ifdef HAVE_TARGET_FW
/* Definition for Firmware */

/* Timer thread */
#define CS_TIMER_THREAD_NAME            "Timer Thread"
#define CS_TIMER_THREAD_STACK_SIZE      (8  * 1024)
#define CS_TIMER_THREAD_PRIORITY        5

/* Logger thread */
#define CS_LOG_THREAD_NAME             "Logger Thread"
#define CS_LOG_THREAD_STACK_SIZE       (16 * 1024)
#define CS_LOG_THREAD_PRIORITY         16


#else


/* Definition for SC */

/* Timer thread */
#define CS_TIMER_THREAD_NAME            "Timer Thread"
#define CS_TIMER_THREAD_STACK_SIZE      (8  * 1024)
#define CS_TIMER_THREAD_PRIORITY        5

/* Tx thread */
#define CS_PKT_TX_THREAD_NAME           "SC_PKT_TX"
#define CS_PKT_TX_THREAD_STACK_SIZE     8192
#define CS_PKT_TX_THREAD_PRIORITY       7

/* Rx thread */
#define CS_PKT_RX_THREAD_NAME           "SC_PKT_RX"
#define CS_PKT_RX_THREAD_STACK_SIZE     8192
#define CS_PKT_RX_THREAD_PRIORITY       8

/* Core thread */
#define CS_CORE_THREAD_NAME             "SC_CORE"
#define CS_CORE_THREAD_STACK_SIZE       8192
#define CS_CORE_THREAD_PRIORITY         9

/* Logger thread */
#define CS_LOG_THREAD_NAME             "Logger Thread"
#define CS_LOG_THREAD_STACK_SIZE       (16 * 1024)
#define CS_LOG_THREAD_PRIORITY         15


/* ESS Thread */
#define CS_ESS_THREAD_NAME           "ESS_APP"
#define CS_ESS_THREAD_STACK_SIZE     0x2000
#define CS_ESS_THREAD_PRIORITY       15


/* Management Interface Name */
#define CS_MGMT_PORT_NAME_0	        "smc0"
#define CS_MGMT_PORT_NAME_1	        "smc1"

#ifdef HAVE_DUAL_MGMT_PORT
#define CS_MGMT_PORT_NUM        2
#else
#define CS_MGMT_PORT_NUM        1
#endif



#endif

#endif

