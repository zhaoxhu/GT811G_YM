/*
 * gw_log.c
 *
 *  Created on: 2012-11-2
 *      Author: tommy
 */


#include "gwd_log.h"
#include <unistd.h>

#ifdef HAVE_EVENT_SHOW_CMD

#if 1
#define GW_OK				CS_E_OK
#define gw_printf(arg...)	cs_printf(arg)
#endif

static gw_int32 log_level = GW_LOG_LEVEL_MINOR;

static gw_int32 log_record_level = GW_LOG_LEVEL_MAJOR;
static gw_int32 log_current_slot = 0;

typedef struct{
	gw_int8 valid;
	gw_int8 text[1];
}log_entry_s;

typedef union{
	gw_int8 buff[GW_EVENT_LOG_LENGTH];
	log_entry_s entry;
}log_entry_t;

log_entry_t g_eventmsgqueue[GW_MAX_EVENT_LOG_NUM];

gw_int32 setGwLogLevel(gw_int32 lv)
{
	log_level = lv;
	return log_level;
}

gw_int32 getGwlogLevel()
{
	return log_level;
}

gw_int32 setGwLogRecordLevel(gw_int32 lv)
{
	if(lv >= GW_LOG_LEVEL_DEBUG && lv <= GW_LOG_LEVEL_CRI)
		log_record_level = lv;
	return log_record_level;
}

gw_int32 getGwLogRecordLevel()
{
	return log_record_level;
}

gw_int32 gw_log_add_record(gw_int8 * rec, gw_int32 len)
{
	log_entry_t * pmsg = NULL;	
	if(len > 0 && rec)
	{

		pmsg = (log_current_slot < GW_MAX_EVENT_LOG_NUM-1)? g_eventmsgqueue+log_current_slot+1:g_eventmsgqueue;

		memset(pmsg, 0, sizeof(log_entry_t));

		pmsg->entry.valid = 1;

		if(len<GW_EVENT_LOG_LENGTH-2)
			memcpy(pmsg->entry.text, rec, len);
		else
			memcpy(pmsg->entry.text, rec, GW_EVENT_LOG_LENGTH-2);

		if(log_current_slot == GW_MAX_EVENT_LOG_NUM-1)
			log_current_slot = 0;
		else
			log_current_slot++;

	}

	return log_current_slot;

}

gw_int8 * gw_log_get_record(gw_int32 slot)
{
	gw_int8 * ret = NULL;
	log_entry_t *pmsg = NULL;

	if(!(slot < 0 || slot >= GW_MAX_EVENT_LOG_NUM))
	{
		pmsg = g_eventmsgqueue+slot;
		if(pmsg->entry.valid)
			ret = pmsg->entry.text;
	}

	return ret;
}

gw_int8 * gw_log_getnext_record(gw_int32 slot, gw_int32 *nextslot)
{
	gw_int8 * ret = NULL;
	log_entry_t *pmsg = NULL;
	gw_int32 start = 0, i = 0;

	if(!nextslot)
		return ret;

	if(!(slot < 0 || slot >= GW_MAX_EVENT_LOG_NUM))
	{
		start = (slot == GW_MAX_EVENT_LOG_NUM-1)?0:slot+1;
	}
	else
		start = log_current_slot;

	i = start;	
	do{
		pmsg = g_eventmsgqueue+i;
		if(pmsg->entry.valid)
		{
			ret = pmsg->entry.text;
			break;
		}
		if(i == GW_MAX_EVENT_LOG_NUM-1)
			i = 0;
		else
			i++;
	}while(1);

	*nextslot = i;

	return ret;
}

gw_int32 gw_log_get_current_msg_slot()
{
	return log_current_slot;
}

gw_int32 func_pointer_error_syslog(const gw_int8 *String,...)
{
	char data[256] = "";
	int ret = 0;
	va_list ap;
	
	va_start(ap, String);
    ret = diag_vsprintf(data, String, ap);
    va_end(ap);
	
	gw_syslog(GW_LOG_LEVEL_MAJOR,data);
	return GW_OK;
}

time_t gw_time_get(time_t *timer);
gw_int32 gw_syslog(gw_int32 level, const gw_int8 *String, ...)
{
    va_list ap;
    int ret = 0;

    int strlen = 0;
    char data[300] = "";

    char * buf = data;
    memset(data,'\0',sizeof(data));

    if(buf)
    {

    	if(level <= log_record_level)
    	{
#if  (!OS_CYG_LINUX)
    time_t now;
    struct tm *timenow;
	#if 0
    time(&now);
	#else
	gw_time_get(&now);
	#endif
    timenow = localtime(&now);
    strlen = sprintf(buf,"%s", asctime(timenow));
    buf +=strlen;
#endif
    	}

    	va_start(ap, String);
    	ret = diag_vsprintf(buf, String, ap);
    	va_end(ap);

    	{
    	if(level <= log_level)
    		diag_printf("%s",data);
//    		sleep(1);
    	}

    	if(level <= log_record_level)
    		gw_log_add_record(data, (ret+strlen));
    }
    else
    {
    	gw_printf("gw_syslog:    malloc fail!\r\n");
    }

    return ret;
}

#endif
