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


#ifndef __LOG_H__
#define __LOG_H__

#include "cs_types.h"

typedef enum {
    IROS_LOG_LEVEL_DBG0 = 0,
    IROS_LOG_LEVEL_DBG1 = 1,
    IROS_LOG_LEVEL_DBG2 = 2,
    IROS_LOG_LEVEL_DBG3 = 3,
    IROS_LOG_LEVEL_INF = 4,
    IROS_LOG_LEVEL_MIN = 5,
    IROS_LOG_LEVEL_MAJ = 6,
    IROS_LOG_LEVEL_CRI = 7,
    MAX_LOG_LEVEL
}SYS_LOG_LEVEL;

#define CS_LOG_ERROR        -1
#define CS_LOG_OK           0

#define CS_LOG_SIZE         256
#define CS_LOG_COMMENT_LEN  48
#ifndef HAVE_MINI_TARGET
#define CS_DFLT_LOG_NUM     32
#else
#define CS_DFLT_LOG_NUM     5
#endif


#define CS_DEFT_LOG_ENABLE_LEVEL    IROS_LOG_LEVEL_MAJ
#define CS_DFLT_LOG_PRINT_LEVEL     IROS_LOG_LEVEL_MAJ
#define CS_DFLT_LOG_RECORD_LEVEL    IROS_LOG_LEVEL_MIN


#define CS_LOG_QUEUE_NAME   "sys_log_queue"
#define CS_LOG_QUEUE_DEPTH  16
#define CS_LOG_QUEUE_SIZE   4

#define CS_INVALID_LEVEL    0xFFFFFFFF

#ifdef HAVE_LOG_FILE
#define MAX_LOG_FILE_SIZE   (16*1024) /*must be less than JFFS2_SIZE-CS_LOG_SIZE */
#define MAX_LOG_FILE_NUM    4
#endif

typedef enum
{
    CS_LOG_MSG_TYPE = 0,
    CS_SYS_LOG_TYPE = 1,
    CS_LOG_HEX_TYPE = 2,
}CS_LOG_TYPE_ENUM;

typedef struct {
    cs_node node;
    cs_uint32 time;
    cs_uint32 log_level;
    cs_uint32 module_id;
    cs_uint32 log_type;
    union {
        cs_int8     log_buf[CS_LOG_SIZE];
        struct _log_hex_buf{
            cs_int8 comment[CS_LOG_COMMENT_LEN];
            cs_int32 len;
            cs_int8 buf[CS_LOG_SIZE-CS_LOG_COMMENT_LEN-4];
        }b;
    }a;
}cs_log_t;

typedef struct {
        cs_uint32 print_level;
        cs_uint32 record_level;
        cs_uint32 remote_level;
}cs_log_mod_info;

#define CS_MAX_MODULE_NUMBER  IROS_MID_MAX
typedef struct {
    cs_uint32   max_log_num;
    cs_uint32   mempool_id;
    cs_uint32   log_list_mutex;
    cs_uint32   log_thread_id;
    cs_uint32   log_queue_id;
    cs_log_mod_info mod_info[CS_MAX_MODULE_NUMBER];
    cs_list        log_list;
 #ifdef HAVE_LOG_FILE
    void *log_fp;
 #endif
    cs_uint32   remote_log_server_ip; 
}cs_log_ctrl_t;

#ifdef HAVE_LOG_FILE
#define MAX_SYS_LOG_NUM 32
#define MAX_SYS_LOG_LEN 64
#define CS_LOG_WRITE_INTERVAL 10 /*seconds*/

typedef struct {
    cs_uint32   log_magic;
    cs_uint32   log_cursor;
    cs_int8     log_buf[MAX_SYS_LOG_NUM][MAX_SYS_LOG_LEN];
} cs_sys_log_t;
#endif

#define cs_list_lock()      cs_mutex_lock(sys_log_ctrl.log_list_mutex)
#define cs_list_unlock()    cs_mutex_unlock(sys_log_ctrl.log_list_mutex)

#define log_debug if(log_debug_flag) cs_printf
#define IROS_DPRINTF(args...)
#define IROS_LOG_MSG cs_log_msg


#define IROS_LOG_CRI(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_CRI,  modid, args)

#if CS_DEFT_LOG_ENABLE_LEVEL == IROS_LOG_LEVEL_CRI
#define IROS_LOG_MAJ(modid, args...)
#define IROS_LOG_MIN(modid, args...)
#define IROS_LOG_INF(modid, args...)
#define IROS_LOG_DBG3(modid, args...)
#define IROS_LOG_DBG2(modid, args...)
#define IROS_LOG_DBG1(modid, args...)
#define IROS_LOG_DBG0(modid, args...)
#elif CS_DEFT_LOG_ENABLE_LEVEL == IROS_LOG_LEVEL_MAJ
#define IROS_LOG_MAJ(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_MAJ,  modid, args)
#define IROS_LOG_MIN(modid, args...)
#define IROS_LOG_INF(modid, args...)
#define IROS_LOG_DBG3(modid, args...)
#define IROS_LOG_DBG2(modid, args...)
#define IROS_LOG_DBG1(modid, args...)
#define IROS_LOG_DBG0(modid, args...)
#elif CS_DEFT_LOG_ENABLE_LEVEL == IROS_LOG_LEVEL_MIN
#define IROS_LOG_MAJ(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_MAJ,  modid, args)
#define IROS_LOG_MIN(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_MIN,  modid, args)
#define IROS_LOG_INF(modid, args...)
#define IROS_LOG_DBG3(modid, args...)
#define IROS_LOG_DBG2(modid, args...)
#define IROS_LOG_DBG1(modid, args...)
#define IROS_LOG_DBG0(modid, args...)
#elif CS_DEFT_LOG_ENABLE_LEVEL == IROS_LOG_LEVEL_INF
#define IROS_LOG_MAJ(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_MAJ,  modid, args)
#define IROS_LOG_MIN(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_MIN,  modid, args)
#define IROS_LOG_INF(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_INF,  modid, args)
#define IROS_LOG_DBG3(modid, args...)
#define IROS_LOG_DBG2(modid, args...)
#define IROS_LOG_DBG1(modid, args...)
#define IROS_LOG_DBG0(modid, args...)
#elif CS_DEFT_LOG_ENABLE_LEVEL == IROS_LOG_LEVEL_DBG3
#define IROS_LOG_MAJ(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_MAJ,  modid, args)
#define IROS_LOG_MIN(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_MIN,  modid, args)
#define IROS_LOG_INF(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_INF,  modid, args)
#define IROS_LOG_DBG3(modid, args...)   cs_log_msg(IROS_LOG_LEVEL_DBG3, modid, args)
#define IROS_LOG_DBG2(modid, args...)
#define IROS_LOG_DBG1(modid, args...)
#define IROS_LOG_DBG0(modid, args...)
#elif CS_DEFT_LOG_ENABLE_LEVEL == IROS_LOG_LEVEL_DBG2
#define IROS_LOG_MAJ(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_MAJ,  modid, args)
#define IROS_LOG_MIN(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_MIN,  modid, args)
#define IROS_LOG_INF(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_INF,  modid, args)
#define IROS_LOG_DBG3(modid, args...)   cs_log_msg(IROS_LOG_LEVEL_DBG3, modid, args)
#define IROS_LOG_DBG2(modid, args...)   cs_log_msg(IROS_LOG_LEVEL_DBG2, modid, args)
#define IROS_LOG_DBG1(modid, args...)
#define IROS_LOG_DBG0(modid, args...)
#elif CS_DEFT_LOG_ENABLE_LEVEL == IROS_LOG_LEVEL_DBG1
#define IROS_LOG_MAJ(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_MAJ,  modid, args)
#define IROS_LOG_MIN(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_MIN,  modid, args)
#define IROS_LOG_INF(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_INF,  modid, args)
#define IROS_LOG_DBG3(modid, args...)   cs_log_msg(IROS_LOG_LEVEL_DBG3, modid, args)
#define IROS_LOG_DBG2(modid, args...)   cs_log_msg(IROS_LOG_LEVEL_DBG2, modid, args)
#define IROS_LOG_DBG1(modid, args...)   cs_log_msg(IROS_LOG_LEVEL_DBG1, modid, args)
#define IROS_LOG_DBG0(modid, args...)
#elif CS_DEFT_LOG_ENABLE_LEVEL == IROS_LOG_LEVEL_DBG0
#define IROS_LOG_MAJ(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_MAJ,  modid, args)
#define IROS_LOG_MIN(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_MIN,  modid, args)
#define IROS_LOG_INF(modid, args...)    cs_log_msg(IROS_LOG_LEVEL_INF,  modid, args)
#define IROS_LOG_DBG3(modid, args...)   cs_log_msg(IROS_LOG_LEVEL_DBG3, modid, args)
#define IROS_LOG_DBG2(modid, args...)   cs_log_msg(IROS_LOG_LEVEL_DBG2, modid, args)
#define IROS_LOG_DBG1(modid, args...)   cs_log_msg(IROS_LOG_LEVEL_DBG1, modid, args)
#define IROS_LOG_DBG0(modid, args...)   cs_log_msg(IROS_LOG_LEVEL_DBG0, modid, args)
#else
#error CS_DEFT_LOG_ENABLE_LEVEL legal range 0 (DBG0) to 7 (CRI)
#endif


#if 0/*CS_LOG_ENABLE_LEVEL==1*/
#define IROS_LOG_MEM_DUMP_CRI(modid, src, len)       cs_buf_print_mem(IROS_LOG_LEVEL_CRI, modid, (cs_int8 *)src, len)
#define IROS_LOG_MEM_DUMP_MAJ(modid, src, len)      cs_buf_print_mem(IROS_LOG_LEVEL_MAJ, modid, (cs_int8 *)src, len)
#define IROS_LOG_MEM_DUMP_MIN(modid, src, len)      cs_buf_print_mem(IROS_LOG_LEVEL_MIN, modid, (cs_int8 *)src, len)
#define IROS_LOG_MEM_DUMP_INF(modid, src, len)       cs_buf_print_mem(IROS_LOG_LEVEL_INF, modid, (cs_int8 *)src, len)
#define IROS_LOG_MEM_DUMP_DBG3(modid, src, len)   cs_buf_print_mem(IROS_LOG_LEVEL_DBG3, modid, (cs_int8 *)src, len)
#define IROS_LOG_MEM_DUMP_DBG2(modid, src, len)   cs_buf_print_mem(IROS_LOG_LEVEL_DBG2, modid, (cs_int8 *)src, len)
#define IROS_LOG_MEM_DUMP_DBG1(modid, src, len)   cs_buf_print_mem(IROS_LOG_LEVEL_DBG1, modid, (cs_int8 *)src, len)
#define IROS_LOG_MEM_DUMP_DBG0(modid, src, len)   cs_buf_print_mem(IROS_LOG_LEVEL_DBG0, modid, (cs_int8 *)src, len)
#else
#define IROS_LOG_MEM_DUMP_CRI(modid, src, len)
#define IROS_LOG_MEM_DUMP_MAJ(modid, src, len)
#define IROS_LOG_MEM_DUMP_MIN(modid, src, len)
#define IROS_LOG_MEM_DUMP_INF(modid, src, len)
#define IROS_LOG_MEM_DUMP_DBG3(modid, src, len)
#define IROS_LOG_MEM_DUMP_DBG2(modid, src, len)
#define IROS_LOG_MEM_DUMP_DBG1(modid, src, len)
#define IROS_LOG_MEM_DUMP_DBG0(modid, src, len)
#endif

extern cs_int32 cs_log_init(cs_uint32 log_num);
extern cs_uint32 cs_log_count();
extern void cs_log_msg(cs_uint32 level , cs_uint32 module_id , const cs_int8 *String, ...);
extern void cs_log_dump_all();
extern void cs_log_dump_latest(cs_uint32 number);
extern void cs_log_dump_oldest(cs_uint32 number);
extern void cs_log_dump_by_modid(cs_uint32 module_id);
extern void cs_log_dump_by_level(cs_uint32 level);
extern void cs_log_print_level_set(cs_uint32 module_id , cs_uint32 level);
extern void cs_log_print_level_set_all(cs_uint32 level);
extern void cs_log_remote_level_set(cs_uint32 module_id , cs_uint32 level);
extern void cs_log_remote_level_set_all(cs_uint32 level);
extern cs_uint32 cs_log_print_level_get(cs_uint32 module_id);
extern void cs_log_record_level_set(cs_uint32 module_id , cs_uint32 level);
extern void cs_log_record_level_set_all(cs_uint32 level);
extern cs_uint32 cs_log_record_level_get(cs_uint32 module_id);
extern void cs_log_clear_all();
extern void cs_log_clear_by_modid(cs_uint32 module_id);
extern void cs_log_clear_by_level(cs_uint32 level);
extern void cs_buf_print(cs_int8 *pbuf , cs_int32 len);
extern void cs_buf_print_mem(cs_uint32 level , cs_uint32 module_id , cs_int8 *pbuf , cs_int32 len);
extern void iros_log_level_proc(int level, int modid);
extern void cs_buffer_dump(cs_int8 *comment , cs_int8 *buf , cs_int32 len);
#ifdef HAVE_LOG_FILE
#if 1
extern const cs_int8 *sys_log_file;
extern cs_uint32 fp_cursor;
extern void cs_sys_log_dump();
extern void cs_sys_log_clr();
extern cs_int32 cs_sys_log_init();

#else
extern cs_uint32 fp_cursor;
extern cs_uint32 current_file;
extern cs_uint32 sys_log_mutex;
extern cs_int32 cs_sys_log_init();
extern void cs_sys_log_dump();
extern void cs_sys_log_clr();
#endif
#endif
extern void cs_sys_log(const cs_int8 *String, ...);

/*support remoete sys log*/
#define REMOTE_SYSLOG_PORT (514)
/*192.168.1.123*/
#define DEF_REMOTE_SYSLOG_IP   (0xC0A8017B)

extern void cs_log_remoteip_set(cs_uint32 ip);
extern void cs_log_remoteip_get(cs_uint32 *ip);

#endif

