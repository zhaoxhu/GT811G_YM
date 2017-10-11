/*
 * gw_log.h
 *
 *  Created on: 2012-11-2
 *      Author: tommy
 */

#ifndef GW_LOG_H_
#define GW_LOG_H_

#if 1
#include "plat_common.h"
#include "gwd_types.h"
#endif



#define LOG_NEWLINE          "\r\n"
#define LOG_NULLSTR          ""

#if(FOR_BCM_ONU)
#define GW_MAX_EVENT_LOG_NUM	1024	 //maximum of record event msg
#else
//#define GW_MAX_EVENT_LOG_NUM	64	 //maximum of record event msg
//#define GW_MAX_EVENT_LOG_NUM	1024	 //maximum of record event msg
#define GW_MAX_EVENT_LOG_NUM	12	 //maximum of record event msg

#endif
#define GW_EVENT_LOG_LENGTH		128	//length of record msg

enum{
	GW_LOG_LEVEL_FATAL  = 0,
	GW_LOG_LEVEL_ALERT  = 1,
	GW_LOG_LEVEL_CRI    = 2,
	GW_LOG_LEVEL_ERROR  = 3,
	GW_LOG_LEVEL_MAJOR  = 3,
	GW_LOG_LEVEL_WARN   = 4,
	GW_LOG_LEVEL_MINOR  = 4,
	GW_LOG_LEVEL_NOTICE = 5,
	GW_LOG_LEVEL_EVENT  = 5,
	GW_LOG_LEVEL_INFO   = 6,
	GW_LOG_LEVEL_DEBUG  = 7,
	GW_LOG_LEVEL_MAX
};

gw_int32 getGwlogLevel();
gw_int32 setGwLogLevel(gw_int32 lv);
gw_int32 gw_syslog(gw_int32 level, const gw_int8 *String, ...);

gw_int32 func_pointer_error_syslog(const gw_int8 *String,...);


gw_int8 * gw_log_get_record(gw_int32 slot);
gw_int32 gw_log_get_current_msg_slot();
gw_int8 * gw_log_getnext_record(gw_int32 slot, gw_int32 *nextslot);

#ifdef HAVE_EVENT_SHOW_CMD
#define gw_log(lv, log...) gw_syslog(lv, log)
#else
#define gw_log(lv, log...) if(lv <= getGwlogLevel()) printf (log)
#endif


#ifdef _RELEASE_
#define GW_ASSERT(x)        
#else
#define GW_ASSERT(x)        if ( !(x) ) printf("\r\n[ASSERT]File:%s, Line: %lu\r\n", __FILE__, __LINE__ ) 
#endif

#endif /* GW_LOG_H_ */
