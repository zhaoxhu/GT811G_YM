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


#include "stdio.h"
#include "osal_common.h"
#include "plat_config.h"
#include "cs_timer.h"
#include "log.h"
#include <network.h>

#ifdef HAVE_LOG_FILE
#include "sys/stat.h"
#include "dirent.h"
#include "iros_flash_oper.h"

cs_sys_log_t *pSysLog = NULL;
cs_uint32 sys_log_mutex = 0;
cs_int32 log_is_changed = 0;
void cs_sys_log_timer_proc(void *data);
#endif

cs_uint32 logger_count = 0;
cs_int32 log_debug_flag = 0;
cs_log_ctrl_t sys_log_ctrl;

void cs_log_entry();

cs_int32 cs_log_init(cs_uint32 log_num)
{
    cs_int32 i;

    memset(&sys_log_ctrl , 0 , sizeof(cs_log_ctrl_t));
    if(log_num == 0)
    {
        sys_log_ctrl.max_log_num = CS_DFLT_LOG_NUM;
    }
    else
    {
        sys_log_ctrl.max_log_num = log_num;
    }

    for(i = 0 ; i < CS_MAX_MODULE_NUMBER ; i++)
    {
        sys_log_ctrl.mod_info[i].print_level = CS_DFLT_LOG_PRINT_LEVEL;
        sys_log_ctrl.mod_info[i].record_level = CS_DFLT_LOG_RECORD_LEVEL;
        sys_log_ctrl.mod_info[i].remote_level = MAX_LOG_LEVEL;
    }
    sys_log_ctrl.remote_log_server_ip = DEF_REMOTE_SYSLOG_IP; 
#ifdef HAVE_LOG_THREAD
    if(CS_E_OSAL_OK != cs_mempool_create(&sys_log_ctrl.mempool_id ,
                                      "sys_log_pool", sizeof(cs_log_t) , sys_log_ctrl.max_log_num+CS_LOG_QUEUE_DEPTH))
    {
        log_debug("\r\n Create log memory pool failed");
        return CS_LOG_ERROR;
    }

    if(CS_E_OSAL_OK != cs_mutex_init(&sys_log_ctrl.log_list_mutex, "log_lst_mutex", 0))
    {
        log_debug("\r\n Create log-list mutex failed");
        cs_mempool_destroy(sys_log_ctrl.mempool_id);
        return CS_LOG_ERROR;
    }

    if(CS_E_OSAL_OK != cs_queue_create(&sys_log_ctrl.log_queue_id,
                                            CS_LOG_QUEUE_NAME,
                                            CS_LOG_QUEUE_DEPTH,
                                            CS_LOG_QUEUE_SIZE, 0))
    {
        log_debug("\r\n Create log queue failed");
        cs_mempool_destroy(sys_log_ctrl.mempool_id);
        cs_mutex_destroy(sys_log_ctrl.log_list_mutex);
        return CS_LOG_ERROR;
    }

    if(CS_E_OSAL_OK != cs_thread_create(&sys_log_ctrl.log_thread_id,
                                            LOG_THREAD_NAME,
                                            cs_log_entry, (void *)0,
                                            LOG_THREAD_STACKSIZE,
                                            LOG_THREAD_PRIORITY, 0))
    {
        log_debug("\r\n Log thread create failed");
        cs_mempool_destroy(sys_log_ctrl.mempool_id);
        cs_mutex_destroy(sys_log_ctrl.log_list_mutex);
        cs_queue_delete(sys_log_ctrl.log_queue_id);
        return CS_LOG_ERROR;
    }

#ifdef HAVE_LOG_FILE
    cs_sys_log_init();
#endif
#endif
    cs_lst_init(&sys_log_ctrl.log_list , NULL);

    return CS_LOG_OK;
}

cs_uint32 cs_log_count()
{
    return (cs_uint32)cs_lst_count(&sys_log_ctrl.log_list);
}

void cs_log_show(cs_log_t *log)
{
    cs_int8 buf[2*CS_LOG_SIZE];
    cs_int32 i;

    memset(buf , 0 , sizeof(buf));
    if(log->log_type == CS_LOG_MSG_TYPE)
    {
        for(i = 0 ; i < strlen(log->a.log_buf) ; i++)
        {
            if(log->a.log_buf[i] != '\r' && log->a.log_buf[i] != '\n')
                break;
            buf[i] = log->a.log_buf[i];
        }

        sprintf(buf+i , "[%08x:%s:%d] ",log->time,cs_sys_mod_desc_get(log->module_id) ,log->log_level);
        memcpy(buf+strlen(buf) , log->a.log_buf+i , strlen(log->a.log_buf)-i);
        cs_printf("%s",buf);
        return;
    }

    if(log->log_type == CS_LOG_HEX_TYPE)
    {
        cs_printf("\n\n#%s ",log->a.b.comment);
        for(i = 0 ; i < log->a.b.len ; i++)
        {
            if(i%16 == 0)
            {
                cs_printf("\n");
                cs_printf("%06x  ", i);
            }

            cs_printf("%02x ",log->a.b.buf[i]);
        }
        return;
    }
}

void cs_remote_log(cs_log_t *log)
{
    cs_int8 buf[2*CS_LOG_SIZE];
    cs_int32 i;
    int s_source;
    struct sockaddr_in dest_addr;
    
    /*format the buff*/
    memset(buf , 0 , sizeof(buf));
    if(log->log_type == CS_LOG_MSG_TYPE)
    {
        for(i = 0 ; i < strlen(log->a.log_buf) ; i++)
        {
            if(log->a.log_buf[i] != '\r' && log->a.log_buf[i] != '\n')
                break;
            buf[i] = log->a.log_buf[i];
        }

        sprintf(buf+i , "[%08x:%s:%d] ",log->time,cs_sys_mod_desc_get(log->module_id) ,log->log_level);
        memcpy(buf+strlen(buf) , log->a.log_buf+i , strlen(log->a.log_buf)-i);
    }
    
    /*send out the formatted log by UDP*/
    s_source = socket(AF_INET, SOCK_DGRAM, 0);
    if (s_source < 0) {
        diag_printf("UDP socket failed\n");
    }
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_len = sizeof(dest_addr);
    dest_addr.sin_port = htons(REMOTE_SYSLOG_PORT);
    dest_addr.sin_addr.s_addr = htonl(sys_log_ctrl.remote_log_server_ip);
    if (sendto(s_source,buf,strlen(buf),0,(struct sockaddr *)&dest_addr,sizeof(dest_addr)) < 0 ) {
        diag_printf("Error writing buffer");
    }
    close(s_source); 
}
int sys_log_write = 0;
void cs_log_process(cs_log_t *log)
{
    cs_log_t *del_log = NULL;

    if(log == NULL)
        return;

#ifdef HAVE_LOG_FILE
    if(log->log_level == IROS_LOG_LEVEL_CRI)
        log->log_type = CS_SYS_LOG_TYPE;

    if(log->log_type == CS_SYS_LOG_TYPE)
        goto IS_SYS_LOG;
#endif

    if((log->log_level >= sys_log_ctrl.mod_info[log->module_id].print_level &&
        log->log_type == CS_LOG_MSG_TYPE )||
        log->log_type == CS_LOG_HEX_TYPE)
    {
        cs_log_show(log);
    }
    if( log->log_level >= sys_log_ctrl.mod_info[log->module_id].remote_level &&
        log->log_type == CS_LOG_MSG_TYPE )
    {
        cs_remote_log(log);
    }

    if(log->log_level < sys_log_ctrl.mod_info[log->module_id].record_level)
    {
        cs_mem_free((cs_uint8 *)log);
        return;
    }

    if(sys_log_ctrl.max_log_num > cs_log_count())
    {
        cs_list_lock();
        cs_lst_add(&sys_log_ctrl.log_list , (cs_node *)log);
        cs_list_unlock();
        return;
    }

    cs_list_lock();
    del_log = (cs_log_t *)cs_lst_get(&sys_log_ctrl.log_list);
    cs_mem_free((cs_uint8 *)del_log);

    cs_lst_add(&sys_log_ctrl.log_list , (cs_node *)log);
    cs_list_unlock();
    return;

#ifdef HAVE_LOG_FILE
IS_SYS_LOG:
#if 1
{
    #if 0
    fwrite(log->a.log_buf, strlen(log->a.log_buf), 1, (FILE *)sys_log_ctrl.log_fp);
    fp_cursor += strlen(log->a.log_buf);
    if(fp_cursor >= MAX_LOG_FILE_SIZE)
    {
        fp_cursor = 0;
        fseek((FILE *)sys_log_ctrl.log_fp , 0 ,SEEK_SET);
    }
    else
        fseek((FILE *)sys_log_ctrl.log_fp , 0 ,SEEK_CUR);

    cs_mem_free((cs_uint8 *)log);
    #else
    cs_uint32 part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_LOG_ID);
    char *log_addr = NULL;
    int written_len = 0;
    int log_len = 0;
    int block_num = 0;
    int cursor = pSysLog->log_cursor;
    cs_mutex_lock(sys_log_mutex);
    log_addr = flash_dev.info.super_block.part_tab[part_index].part_loc;
    log_len = strlen(log->a.log_buf);
    block_num = (log_len%MAX_SYS_LOG_LEN)?(log_len/MAX_SYS_LOG_LEN + 1):(log_len/MAX_SYS_LOG_LEN);
    if(block_num + cursor > MAX_SYS_LOG_NUM)
    {
        int peek_num = (block_num + cursor)%MAX_SYS_LOG_NUM;
        int temp_cursor = 0;

        if(cursor < MAX_SYS_LOG_LEN)
        {
            memset(pSysLog->log_buf[cursor] , 0 , (MAX_SYS_LOG_NUM-cursor)*MAX_SYS_LOG_LEN);
            memcpy(pSysLog->log_buf[cursor] , log->a.log_buf , (MAX_SYS_LOG_NUM-cursor)*MAX_SYS_LOG_LEN);
            temp_cursor = cursor;
            cursor = 0;
            memset(pSysLog->log_buf[cursor] , 0 , peek_num*MAX_SYS_LOG_LEN);
            memcpy(pSysLog->log_buf[cursor] , log->a.log_buf+(MAX_SYS_LOG_NUM-temp_cursor)*MAX_SYS_LOG_LEN,
                                                    log_len- (MAX_SYS_LOG_NUM-temp_cursor)*MAX_SYS_LOG_LEN);
            cursor = peek_num;
        }
    }
    else
    {
        if(cursor < MAX_SYS_LOG_LEN)
        {
            memset(pSysLog->log_buf[cursor] , 0 , MAX_SYS_LOG_LEN*block_num);
            memcpy(pSysLog->log_buf[cursor] , (char *)log->a.log_buf,log_len);
            cursor = cursor + block_num;
        }
    }

    log_is_changed = 1;
    if(sys_log_write>=1024)
    {
        flash_part_write_init(part_index, log_addr);
        written_len = flash_write(log_addr, (char *)pSysLog, sizeof(cs_sys_log_t));
        flash_part_write_done(part_index);
        sys_log_write = 0;
        if(written_len != sizeof(cs_sys_log_t))
        {
            cs_printf("write log error\n");
            cs_mem_free((cs_uint8 *)log);
            cs_mutex_unlock(sys_log_mutex);
            return;
        }
        log_is_changed = 0;
    }

    pSysLog->log_cursor = cursor;
    cs_mem_free((cs_uint8 *)log);
    sys_log_write += log_len;
    cs_mutex_unlock(sys_log_mutex);
    #endif
}
#else
    cs_mutex_lock(sys_log_mutex);
    if(sys_log_ctrl.log_fp == NULL)
    {
        char fn[32];
        memset(fn, 0,sizeof(fn));
        sprintf(fn,"/log/log%d.txt",0);
        fp_cursor = 0;
        sys_log_ctrl.log_fp = (void *)fopen(fn,"w");
        if(sys_log_ctrl.log_fp == NULL)
        {
            cs_mem_free((cs_uint8 *)log);
            cs_mutex_unlock(sys_log_mutex);
            return;
        }
        fseek((FILE *)sys_log_ctrl.log_fp , 0 ,SEEK_SET);
    }

    fwrite(log->a.log_buf , strlen(log->a.log_buf) , 1 , (FILE *)sys_log_ctrl.log_fp);
    fp_cursor += strlen(log->a.log_buf);
    if(fp_cursor >= MAX_LOG_FILE_SIZE)
    {
        char fn[32];
        fp_cursor = 0;
        fclose((FILE *)sys_log_ctrl.log_fp);
        cs_printf("Close file %d file\n",current_file);
        current_file++;
        current_file = current_file%MAX_LOG_FILE_NUM;
        memset(fn , 0 , sizeof(fn));
        sprintf(fn , "/log/log%d.txt",current_file);
        remove(fn);
        sys_log_ctrl.log_fp = (void *)fopen(fn,"w");
        cs_printf("Open file %s\n",fn);
        if(sys_log_ctrl.log_fp == NULL)
        {
            cs_printf("Open file %s failed, discard log\n",fn);
            cs_mem_free((cs_uint8 *)log);
            cs_mutex_unlock(sys_log_mutex);
            return;
        }
        fseek((FILE *)sys_log_ctrl.log_fp , 0 ,SEEK_SET);
    }
    else
        fseek((FILE *)sys_log_ctrl.log_fp , 0 ,SEEK_CUR);
    #if 0
{
    char temp_str[4];
    rec_fp = (void *)fopen(sys_log_rec , "w");
    if(rec_fp != NULL)
    {
        memset(temp_str, 0 , sizeof(temp_str));
        sprintf(temp_str , "%d",current_file);
        fwrite(temp_str, strlen(temp_str), 1, (FILE *)rec_fp);
        fseek((FILE *)rec_fp , 0 , SEEK_CUR);
        fclose((FILE *)rec_fp);
    }
}
    #endif
    cs_mem_free((cs_uint8 *)log);
    cs_mutex_unlock(sys_log_mutex);
#endif
#endif

    return;
}

void cs_log_entry()
{
    cs_log_t *log = NULL;
    cs_uint32 size_copy=0;

    while(1)
    {
        logger_count++;
        log = NULL;
        cs_queue_get(sys_log_ctrl.log_queue_id,
                                                (void *)&log,
                                                sizeof(cs_uint32),
                                                &size_copy,
                                                CS_OSAL_WAIT_FOREVER);

        if(log == NULL)
            continue;

#ifdef HAVE_LOG_FILE
        if((cs_uint32)log == 1)
        {
            cs_sys_log_timer_proc(NULL);
            continue;
        }
#endif

        cs_log_process(log);
    }
}

void cs_log_msg(cs_uint32 level , cs_uint32 module_id , const cs_int8 *String, ...)
{
    va_list   ptr;
    cs_int8 msg_buffer [4*CS_LOG_SIZE];
#ifdef HAVE_LOG_THREAD
    cs_log_t *log;
    cs_int32 len=0;
    cs_int32 time_str_len=0;
#endif

    if(module_id >= CS_MAX_MODULE_NUMBER || level >= MAX_LOG_LEVEL)
        return;

#ifdef HAVE_LOG_THREAD
    if( level < sys_log_ctrl.mod_info[module_id].record_level &&
        level < sys_log_ctrl.mod_info[module_id].print_level  &&
        level < sys_log_ctrl.mod_info[module_id].remote_level )
        return;

    log = (cs_log_t *)cs_mem_malloc(sys_log_ctrl.mempool_id);
    if(log == NULL)
        return;
    memset(log , 0 , sizeof(cs_log_t));
    log->log_level = level;
    log->module_id = module_id;
    log->time = cs_current_time();
    log->log_type = CS_LOG_MSG_TYPE;
#endif

    memset(msg_buffer , 0 , sizeof(msg_buffer));
    va_start(ptr,String);
    vsprintf(msg_buffer , String, ptr);
    va_end(ptr);

#ifdef HAVE_LOG_THREAD
    if(log->log_level == IROS_LOG_LEVEL_CRI)
    {
        cs_int32 hour,min,sec;
        hour = log->time/(100*3600);
        min = (log->time/(100*60))%60;
        sec = (log->time/100)%60;
        sprintf(log->a.log_buf,"[%dh%dm%ds] ",hour,min,sec);
    }
    time_str_len = strlen(log->a.log_buf);
    #if 1
    len = (strlen(msg_buffer) >= CS_LOG_SIZE-time_str_len)?CS_LOG_SIZE-time_str_len-1:strlen(msg_buffer);
    #else
    len = (strlen(msg_buffer) >= MAX_SYS_LOG_LEN-time_str_len)?MAX_SYS_LOG_LEN-time_str_len-1:strlen(msg_buffer);
    #endif
    if(time_str_len + len >=  CS_LOG_SIZE)
    {
        cs_mem_free((cs_uint8 *)log);
        return;
    }
    
    memcpy(log->a.log_buf+time_str_len , msg_buffer , len);

    if(CS_E_OSAL_OK != cs_queue_put(sys_log_ctrl.log_queue_id, (void *)&log, sizeof(cs_uint32) , CS_OSAL_NO_WAIT , 0))
    {
        cs_mem_free((cs_uint8 *)log);
        return;
    }
#else
    if(level >= sys_log_ctrl.mod_info[module_id].print_level)
        cs_printf("\r\n[%lld:%s:%d] %s",cs_current_time(),cs_sys_mod_desc_get(module_id),level, msg_buffer);
#endif

    return;
}

void cs_buffer_dump(cs_int8 *comment , cs_int8 *buf , cs_int32 len)
{
#ifdef HAVE_LOG_THREAD
    cs_log_t *log;
    cs_int32 comment_len=0;
    cs_int32 buf_len = 0;

    log = (cs_log_t *)cs_mem_malloc(sys_log_ctrl.mempool_id);
    if(log == NULL)
        return;
    memset(log , 0 , sizeof(cs_log_t));
    log->log_level = -1;
    log->module_id = -1;
    log->time = cs_current_time();
    log->log_type = CS_LOG_HEX_TYPE;

    comment_len = (strlen(comment) >= CS_LOG_COMMENT_LEN)?CS_LOG_COMMENT_LEN-1:strlen(comment);
    buf_len = (len >= CS_LOG_SIZE-CS_LOG_COMMENT_LEN-4)?CS_LOG_SIZE-CS_LOG_COMMENT_LEN-4-1:len;
    log->a.b.len = buf_len;
    memcpy(log->a.b.comment , comment , comment_len);
    memcpy(log->a.b.buf , buf , buf_len);

    if(CS_E_OSAL_OK != cs_queue_put(sys_log_ctrl.log_queue_id, (void *)&log, sizeof(cs_uint32) , CS_OSAL_NO_WAIT , 0))
    {
        cs_mem_free((cs_uint8 *)log);
        return;
    }
#else
    int i;
    cs_printf("\n\n#%s ",comment);
    for(i = 0 ; i < len ; i++)
    {
        if(i%16 == 0)
        {
            cs_printf("\n");
            cs_printf("%06x  ", i);
        }

        cs_printf("%02x ",buf[i]);
    }
#endif

    return;
}


void cs_log_dump_all()
{
    cs_log_t *log = NULL;
    cs_int32 count=0;

    cs_list_lock();
    cs_lst_scan(&sys_log_ctrl.log_list , log , cs_log_t *)
    {
        count++;
        cs_log_show(log);
        if(count%50)
            cs_thread_delay(10);
    }
    cs_list_unlock();

    return;
}

void cs_log_dump_latest(cs_uint32 number)
{
    cs_log_t *log = NULL;
    cs_int32 count=0;

    cs_list_lock();
    log = (cs_log_t *)cs_lst_last(&sys_log_ctrl.log_list);
    while(log != NULL && count < number)
    {
        cs_log_show(log);
        count++;
        if(count%50)
            cs_thread_delay(10);
        log = (cs_log_t *)cs_lst_prev((cs_node *)log);
    }
    cs_list_unlock();

    return;
}

void cs_log_dump_oldest(cs_uint32 number)
{
    cs_log_t *log = NULL;
    cs_int32 count=0;

    cs_list_lock();
    log = (cs_log_t *)cs_lst_first(&sys_log_ctrl.log_list);
    while(log != NULL && count < number)
    {
        cs_log_show(log);
        count++;
        if(count%50)
            cs_thread_delay(10);
        log = (cs_log_t *)cs_lst_next((cs_node *)log);
    }
    cs_list_unlock();

    return;
}

void cs_log_dump_by_modid(cs_uint32 module_id)
{
    cs_log_t *log = NULL;
    cs_int32 count=0;

    cs_list_lock();
    cs_lst_scan(&sys_log_ctrl.log_list , log , cs_log_t *)
    {
        if(log->module_id != module_id)
            continue;
        count++;
        cs_log_show(log);
        if(count%50)
            cs_thread_delay(10);
    }
    cs_list_unlock();

    return;
}

void cs_log_dump_by_level(cs_uint32 level)
{
    cs_log_t *log = NULL;
    cs_int32 count=0;

    cs_list_lock();
    cs_lst_scan(&sys_log_ctrl.log_list , log , cs_log_t *)
    {
        if(log->log_level != level)
            continue;
        count++;
        if(count%50)
            cs_thread_delay(10);
        cs_log_show(log);
    }
    cs_list_unlock();

    return;
}

void cs_log_remoteip_set(cs_uint32 ip)
{
    sys_log_ctrl.remote_log_server_ip= ip;
}

void cs_log_remoteip_get(cs_uint32 *ip)
{
    *ip = sys_log_ctrl.remote_log_server_ip;
}

void cs_log_remote_level_set(cs_uint32 module_id , cs_uint32 level)
{
    if(level >= MAX_LOG_LEVEL || module_id >= CS_MAX_MODULE_NUMBER)
        return;

    sys_log_ctrl.mod_info[module_id].remote_level= level;
}

void cs_log_remote_level_set_all(cs_uint32 level)
{
    cs_uint32 module_id;
    if(level >= MAX_LOG_LEVEL)
        return;

    for(module_id = 0 ; module_id < CS_MAX_MODULE_NUMBER ; module_id++)
    {
        sys_log_ctrl.mod_info[module_id].remote_level= level;
    }
}


void cs_log_print_level_set(cs_uint32 module_id , cs_uint32 level)
{
    if(level >= MAX_LOG_LEVEL || module_id >= CS_MAX_MODULE_NUMBER)
        return;

    sys_log_ctrl.mod_info[module_id].print_level = level;
}

void cs_log_print_level_set_all(cs_uint32 level)
{
    cs_uint32 module_id;
    if(level >= MAX_LOG_LEVEL)
        return;

    for(module_id = 0 ; module_id < CS_MAX_MODULE_NUMBER ; module_id++)
    {
        sys_log_ctrl.mod_info[module_id].print_level = level;
    }
}

cs_uint32 cs_log_print_level_get(cs_uint32 module_id)
{
    if(module_id >= CS_MAX_MODULE_NUMBER)
        return CS_INVALID_LEVEL;

    return sys_log_ctrl.mod_info[module_id].print_level;
}

void cs_log_record_level_set(cs_uint32 module_id , cs_uint32 level)
{
    if(level >= MAX_LOG_LEVEL || module_id >= CS_MAX_MODULE_NUMBER)
        return;

    sys_log_ctrl.mod_info[module_id].record_level = level;
}

void cs_log_record_level_set_all(cs_uint32 level)
{
    cs_uint32 module_id;
    if(level >= MAX_LOG_LEVEL)
        return;

    for(module_id = 0 ; module_id < CS_MAX_MODULE_NUMBER ; module_id++)
    {
        sys_log_ctrl.mod_info[module_id].record_level = level;
    }
}

cs_uint32 cs_log_record_level_get(cs_uint32 module_id)
{
    if(module_id >= CS_MAX_MODULE_NUMBER)
        return CS_INVALID_LEVEL;

    return sys_log_ctrl.mod_info[module_id].record_level;
}

void cs_log_clear_all()
{
    cs_log_t *log = NULL;

    cs_list_lock();
    while((log = (cs_log_t *)cs_lst_get(&sys_log_ctrl.log_list)) != NULL)
    {
        cs_mem_free((cs_uint8 *)log);
        log = NULL;
    }
    cs_list_unlock();

    return;
}

void cs_log_clear_by_modid(cs_uint32 module_id)
{
    cs_log_t *log = NULL;
    cs_log_t *prev = NULL;

    cs_list_lock();
    for(log = (cs_log_t *)cs_lst_first(&sys_log_ctrl.log_list) ; log ; )
    {
        if(log->module_id != module_id)
        {
            log = (cs_log_t *)cs_lst_next((cs_node *)log);
            continue;
        }

        prev = (cs_log_t *)cs_lst_prev((cs_node *)log);
        cs_lst_delete(&sys_log_ctrl.log_list , (cs_node *)log);
        cs_mem_free((cs_uint8 *)log);
        if(prev != NULL)
            log = (cs_log_t *)cs_lst_next((cs_node *)prev);
        else
            log = (cs_log_t *)cs_lst_first(&sys_log_ctrl.log_list);
    }
    cs_list_unlock();

    return;
}

void cs_log_clear_by_level(cs_uint32 level)
{
    cs_log_t *log = NULL;
    cs_log_t *prev = NULL;

    cs_list_lock();
    for(log = (cs_log_t *)cs_lst_first(&sys_log_ctrl.log_list) ; log ; )
    {
        if(log->log_level != level)
        {
            log = (cs_log_t *)cs_lst_next((cs_node *)log);
            continue;
        }

        prev = (cs_log_t *)cs_lst_prev((cs_node *)log);
        cs_lst_delete(&sys_log_ctrl.log_list , (cs_node *)log);
        cs_mem_free((cs_uint8 *)log);
        if(prev != NULL)
            log = (cs_log_t *)cs_lst_next((cs_node *)prev);
        else
            log = (cs_log_t *)cs_lst_first(&sys_log_ctrl.log_list);
    }
    cs_list_unlock();

    return;
}

void cs_buf_print(cs_int8 *pbuf , cs_int32 len)
{
    cs_int32 i , j=0 ,k;
    cs_int8 strbuf[16];
    cs_int32 remain_len=0;

    cs_printf("\r\n==============================================================================");
    for(i = 0 ; i < len ; i++) {
        if(i%16 == 0) {
            j = 0;
            cs_printf("\r\n%08x: ",(cs_uint32)pbuf+i);
            memset(strbuf , 0 , sizeof(strbuf));
            remain_len = len - i;
        }
        strbuf[i%16] =  *((cs_int8 *)(pbuf+i));
        cs_printf("%02x ",*((cs_int8 *)(pbuf+i)));

        if(++j == 8)
            cs_printf(" ");

        if(remain_len == j && (remain_len%16) != 0)
        {
            if(j < 8) cs_printf(" ");
            for( ; j < 16 ; j++)
            {
                cs_printf("   ");
            }
        }

        if(j == 16)
        {
            remain_len = 0;
            cs_printf("; ");
            for(k = 0 ; k < 16 ; k++)
            {
                if(strbuf[k] >= ' ' && strbuf[k]<='~')
                    cs_printf("%c",strbuf[k]);
                else
                    cs_printf(".",strbuf[k]);
            }
        }
    }

    printf("\r\n\n");
    return;
}

void cs_buf_print_mem(cs_uint32 level , cs_uint32 module_id , cs_int8 *pbuf , cs_int32 len)
{
    cs_uint32 time = (cs_uint32)cs_current_time();

    cs_printf("\r\n==============================================================================");
    cs_printf("\r\n%08x: module=%s , level=%d",time,cs_sys_mod_desc_get(module_id),level);
    cs_buf_print(pbuf, len);
}

void iros_log_level_proc(int level, int modid)
{
    int i = 0;

    if(level == -1)
    {
        for(i = 0; i < CS_MAX_MODULE_NUMBER; i++)
        {
            cs_printf("mod %02d: logging print level %1d\n", i, sys_log_ctrl.mod_info[i].print_level);
        }
        return;
    }

    if(modid == -1)
    {
        if(level >= 0 && level <= 7)
        {
            for (i = 0; i < CS_MAX_MODULE_NUMBER; i++)
            {
                sys_log_ctrl.mod_info[i].print_level = level;
            }
        }
        else
        {
            cs_printf("invalid logging print level %1d\n", level);
        }
        return;
    }

    if(modid < 0 || modid >= CS_MAX_MODULE_NUMBER || level < 0 || level > 7)
    {
        cs_printf("invalid mod id and level: %d %d\n", modid, level);
        return;
    }

    sys_log_ctrl.mod_info[modid].print_level = level;
    return;
}

#ifdef HAVE_LOG_FILE
#if 1
const cs_int8 *sys_log_file = "/log/log.txt";
cs_uint32 fp_cursor = 0;
void cs_sys_log_timer_notify(void *data)
{
    cs_uint32 write_log = 1;

    cs_queue_put(sys_log_ctrl.log_queue_id, (void *)&write_log, sizeof(cs_uint32) , CS_OSAL_NO_WAIT , 0);
}

void cs_sys_log_timer_proc(void *data)
{
    cs_uint32 part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_LOG_ID);
    char *log_addr = NULL;
    int written_len = 0;

    if(log_is_changed == 0)   /*sys log is changed*/
        return;

    cs_mutex_lock(sys_log_mutex);

    log_addr = flash_dev.info.super_block.part_tab[part_index].part_loc;
    flash_part_write_init(part_index, log_addr);
    written_len = flash_write(log_addr, (char *)pSysLog, sizeof(cs_sys_log_t));
    flash_part_write_done(part_index);
    sys_log_write = 0;
    log_is_changed = 0;
    if(written_len != sizeof(cs_sys_log_t))
    {
        cs_printf("write log error\n");
        cs_mutex_unlock(sys_log_mutex);
        return;
    }

    cs_mutex_unlock(sys_log_mutex);
    return;
}

cs_int32 cs_sys_log_init()
{
#if 0
    struct stat file_stat;
    DIR *pDir = NULL;

    pDir = opendir("/log");
    if(pDir == NULL)
    {
        if(mkdir("/log", 0) != 0)
        {
            cs_printf("\r\n Open log directory failed.");
            return 0;
        }
    }

    sys_log_ctrl.log_fp = (void *)fopen(sys_log_file,"r+");
    if(sys_log_ctrl.log_fp == NULL)
    {
        sys_log_ctrl.log_fp = (void *)fopen(sys_log_file,"w+");
    }
    fstat(fileno((FILE *)sys_log_ctrl.log_fp), &file_stat);

    if(file_stat.st_size >= MAX_LOG_FILE_SIZE)
    {
        fseek((FILE *)sys_log_ctrl.log_fp , 0 , SEEK_SET);
        fp_cursor = 0;
    }
    else
    {
        fseek((FILE *)sys_log_ctrl.log_fp , 0 , SEEK_END);
        fp_cursor = file_stat.st_size;
    }

    return 0;
#else
    cs_uint32 part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_LOG_ID);
    pSysLog = (cs_sys_log_t *)malloc(sizeof(cs_sys_log_t));
    if(pSysLog == NULL)
    {
        cs_printf("Allocate sys log memory failed.\n");
        return 0;
    }
    memcpy(pSysLog , flash_dev.info.super_block.part_tab[part_index].part_loc , sizeof(cs_sys_log_t));
    if(pSysLog->log_magic != 0x12345678)
    {
        memset(pSysLog , 0 , sizeof(cs_sys_log_t));
        pSysLog->log_magic = 0x12345678;
        pSysLog->log_cursor = 0;
    }
    cs_mutex_init(&sys_log_mutex, "syslogmutex", 0);
    cs_circle_timer_add(CS_LOG_WRITE_INTERVAL*1000, cs_sys_log_timer_notify, NULL);
    return 0;
#endif
}

void cs_sys_log(const cs_int8 *String, ...)
{
    va_list   ptr;
    cs_int8 msg_buffer [4*CS_LOG_SIZE];
    cs_log_t *log;
    cs_int32 len=0;

    log = (cs_log_t *)cs_mem_malloc(sys_log_ctrl.mempool_id);
    if(log == NULL)
        return;
    memset(log , 0 , sizeof(cs_log_t));
    log->time = 0;
    log->log_type = CS_SYS_LOG_TYPE;

    memset(msg_buffer , 0 , sizeof(msg_buffer));
    va_start(ptr,String);
    vsprintf(msg_buffer , String, ptr);
    va_end(ptr);

    len = (strlen(msg_buffer) >= CS_LOG_SIZE)?CS_LOG_SIZE-1:strlen(msg_buffer);
    if(len >= CS_LOG_SIZE)
    {
        cs_mem_free((cs_uint8 *)log);
        return;
    }
    
    memcpy(log->a.log_buf , msg_buffer , len);

    if(CS_E_OSAL_OK != cs_queue_put(sys_log_ctrl.log_queue_id, (void *)&log, sizeof(cs_uint32) , CS_OSAL_NO_WAIT , 0))
    {
        cs_mem_free((cs_uint8 *)log);
        return;
    }

    return;
}

void cs_sys_log_dump()
{
#if 0
    FILE *fp = NULL;
    cs_int8 str[2*CS_LOG_SIZE];
    cs_uint32 count_str = 0;

    fp = fopen(sys_log_file , "r");
    if(fp == NULL)
        return;

    memset(str , 0 , sizeof(str));
    while(fgets(str, sizeof(str), fp) != NULL)
    {
        cs_printf("%s",str);
        count_str++;
        if(count_str >= 30)
        {
            cs_thread_delay(10);
            count_str = 0;
        }
        memset(str , 0 , sizeof(str));
    }
    fclose(fp);
    return;
#else
    char str[MAX_SYS_LOG_LEN+1];
    int i;
    int cursor = 0;

    cs_mutex_lock(sys_log_mutex);
    for(i = 0 ; i < MAX_SYS_LOG_NUM ; i++)
    {
        cursor = (pSysLog->log_cursor + i)%MAX_SYS_LOG_NUM;
        if(pSysLog->log_buf[cursor][0] == '\0')
            continue;

        memset(str , 0 , MAX_SYS_LOG_LEN+1);
        memcpy(str , pSysLog->log_buf[cursor] , MAX_SYS_LOG_LEN);
        cs_printf("%s",str);
    }
    cs_mutex_unlock(sys_log_mutex);
    return;
#endif
}

void cs_sys_log_clr()
{
#if 0
    fclose(sys_log_ctrl.log_fp);
    remove(sys_log_file);
    sys_log_ctrl.log_fp = (void *)fopen(sys_log_file,"w+");
    fseek((FILE *)sys_log_ctrl.log_fp , 0 , SEEK_END);
    fp_cursor = 0;
#else
    cs_uint32 part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_LOG_ID);
    char *log_addr = NULL;


    if(pSysLog == NULL)
        return;
    cs_mutex_lock(sys_log_mutex);
    memset(pSysLog , 0 , sizeof(cs_sys_log_t));
    pSysLog->log_magic = 0x12345678;
    pSysLog->log_cursor = 0;
    log_addr = flash_dev.info.super_block.part_tab[part_index].part_loc;
    flash_part_write_init(part_index, log_addr);
    flash_write(log_addr, (char *)pSysLog, sizeof(cs_sys_log_t));
    flash_part_write_done(part_index);
    cs_mutex_unlock(sys_log_mutex);
#endif
    return;
}

#else
cs_uint32 fp_cursor = 0;
cs_uint32 current_file = 0;
cs_uint32 sys_log_mutex = 0;
cs_int32 cs_sys_log_init()
{
    DIR *pDir = NULL;
    char fn[32];
    struct stat file_stat;
    int i;
    FILE *fp=NULL;
    int small_flag = 0;

    pDir = opendir("/log");
    if(pDir == NULL)
    {
        if(mkdir("/log", 0) != 0)
        {
            cs_printf("\r\n Open log directory failed.");
            return 0;
        }
    }

    for(i = 0 ; i < MAX_LOG_FILE_NUM ; i++)
    {
        memset(fn, 0 , sizeof(fn));
        sprintf(fn,"/log/log%d.txt",i);
        fp = fopen(fn,"r");
        if(fp == NULL)
        {
            current_file = i;
            break;
        }

        fstat(fileno(fp), &file_stat);
        if(file_stat.st_size >= MAX_LOG_FILE_SIZE)
        {
            fclose(fp);
            continue;
        }

        fclose(fp);
        current_file = i;
        small_flag = 1;
        fp_cursor = file_stat.st_size;
        break;
    }

    if(i >= MAX_LOG_FILE_NUM)
        current_file = 0;

    cs_printf("Current log file number is %d\n",current_file);
    memset(fn, 0 , sizeof(fn));
    sprintf(fn,"/log/log%d.txt",current_file);
    if(small_flag)
    {
        sys_log_ctrl.log_fp = (void *)fopen(fn,"a");
        fseek((FILE *)sys_log_ctrl.log_fp , 0 ,SEEK_END);
    }
    else
    {
        sys_log_ctrl.log_fp = (void *)fopen(fn,"w");
        fseek((FILE *)sys_log_ctrl.log_fp , 0 ,SEEK_SET);
    }
    cs_mutex_init(&sys_log_mutex, "sys_log_mut", 0);

    return 0;
}

void cs_sys_log(const cs_int8 *String, ...)
{
    va_list   ptr;
    cs_int8 msg_buffer [2*CS_LOG_SIZE];
    cs_log_t *log;
    cs_int32 len=0;
    cs_int32 hour=0,min=0,sec=0;

    log = (cs_log_t *)cs_mem_malloc(sys_log_ctrl.mempool_id);
    if(log == NULL)
        return;
    memset(log , 0 , sizeof(cs_log_t));
    log->time = cs_current_time();
    log->log_type = CS_SYS_LOG_TYPE;

    memset(msg_buffer , 0 , sizeof(msg_buffer));
    va_start(ptr,String);
    vsprintf(msg_buffer , String, ptr);
    va_end(ptr);
    /*12 bytes for timestamp*/
    len = (strlen(msg_buffer) >= CS_LOG_SIZE-12)?CS_LOG_SIZE-12-1:strlen(msg_buffer);
    #if 1
    hour = log->time/(100*3600);
    min = (log->time/(100*60))%60;
    sec = (log->time/100)%60;
    memset(log->a.log_buf , 0 , CS_LOG_SIZE);
    sprintf(log->a.log_buf,"[%dh%dm%ds] ",hour,min,sec);
    #endif
    memcpy(log->a.log_buf+strlen(log->a.log_buf) , msg_buffer , len);

    if(CS_E_OSAL_OK != cs_queue_put(sys_log_ctrl.log_queue_id, (void *)&log, sizeof(cs_uint32) , CS_OSAL_NO_WAIT , 0))
    {
        cs_mem_free((cs_uint8 *)log);
        return;
    }

    return;
}

void cs_sys_log_dump()
{
    FILE *fp = NULL;
    cs_int8 str[2*CS_LOG_SIZE];
    cs_uint32 count_str = 0;
    int i;
    char fn[32];

    cs_mutex_lock(sys_log_mutex);
    for(i = 0 ; i < MAX_LOG_FILE_NUM ; i++)
    {
        memset(fn,0,sizeof(fn));
        sprintf(fn,"/log/log%d.txt",i);
        fp = fopen(fn , "r");
        if(fp == NULL)
            continue;

        cs_printf("==== %s ====\n",fn);
        memset(str , 0 , sizeof(str));
        while(fgets(str, sizeof(str), fp) != NULL)
        {
            cs_printf("%s",str);
            count_str++;
            if(count_str >= 30)
            {
                cs_thread_delay(10);
                count_str = 0;
            }
            memset(str , 0 , sizeof(str));
        }
        fclose(fp);
    }
    cs_mutex_unlock(sys_log_mutex);

    return;
}

void cs_sys_log_clr()
{
    int i;
    char fn[32];
    cs_mutex_lock(sys_log_mutex);
    if(sys_log_ctrl.log_fp != NULL)
    {
        fclose((FILE *)sys_log_ctrl.log_fp);
        cs_printf("Close current file\n");
        sys_log_ctrl.log_fp = NULL;
    }
    for(i = 0 ; i < MAX_LOG_FILE_NUM ; i++)
    {
        memset(fn,0,sizeof(fn));
        sprintf(fn,"/log/log%d.txt",i);
        remove(fn);
        cs_printf("Remove file %s\n",fn);
    }
    current_file = 0;
    fp_cursor = 0;
    cs_mutex_unlock(sys_log_mutex);
    return;
}


#endif
#else
void cs_sys_log(const cs_int8 *String, ...)
{
    return;
}
#endif

