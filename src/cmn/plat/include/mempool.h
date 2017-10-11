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
/*
 *
 * mempool.h
 *
 * $Id: mempool.h,v 1.1.2.1.2.1.8.4.8.2 2011/04/18 02:26:34 zsun Exp $
 */

#ifndef _MEMPOOL_H_
#define _MEMPOOL_H_

#include "cs_types.h"

#define MEMPOOL_EX
#define MAX_SYS_MODULE_NUMBER 32
#define CS_INVALID_MODULE_ID 0xFFFF

#define MAX_PKT_LEN     1600
#define MAX_PKT_NUM     80

#ifdef MEMPOOL_EX
#define MAX_MODULE_NAME_LEN  32

typedef struct {
    cs_uint16 pool_id;
    cs_uint16 u2Rsvd;
    cs_uint32 count;
} pool_counter_t;

typedef struct {
    cs_uint32  module_id;
    pool_counter_t  *pool_counter;
    cs_uint32  count_64bytes;
    cs_uint32  count_65_128bytes;
    cs_uint32  count_129_256bytes;
    cs_uint32  count_257_512bytes;
    cs_uint32  count_513_1024bytes;
    cs_uint32  count_1k_2kbytes;
    cs_uint32  count_2k_4kbytes;
    cs_uint32  count_4k_8kbytes;
    cs_uint32  count_8kbytes; /*greater than 8k*/
} sys_module_desc_t;

typedef struct {
    cs_int32 elem_size;
    cs_int32 elem_num;
}sys_pool_cfg_t;

#ifdef HAVE_MEM_TRACE
#define MAX_SYS_MEM_STAT 256
typedef struct {
    cs_uint32 free;
    cs_uint16 module_id;
    cs_uint16 alloc_line;
    cs_int8     *func_name;
    cs_uint32 count;
} sys_mem_stat_t;
#endif

extern cs_uint8 *cs_sys_pool_malloc(cs_uint32 module_id , cs_int32 size , cs_int8 *func_name , cs_int32 line,cs_int32 trace_flag);
extern void cs_sys_pool_free(cs_uint8 *ptr , cs_int8 *func_name , cs_int32 line,cs_int32 trace_flag);
extern void cs_sys_pool_show(cs_uint32 pool_id);
extern void cs_sys_module_show(cs_uint32 module_id);

#define iros_malloc(module_id , size) (void *)cs_sys_pool_malloc(module_id , size , (cs_int8 *)__FUNCTION__ , __LINE__,1)
#define iros_free(ptr) cs_sys_pool_free((cs_uint8 *)ptr , (cs_int8 *)__FUNCTION__ , __LINE__,1)

#define iros_malloc_no_trace(module_id , size) (void *)cs_sys_pool_malloc(module_id , size , (cs_int8 *)__FUNCTION__ , __LINE__,0)
#define iros_free_no_trace(ptr) cs_sys_pool_free((cs_uint8 *)ptr , (cs_int8 *)__FUNCTION__ , __LINE__,0)

#else
#define iros_malloc(module_id , size) malloc(size)
#define iros_free(ptr) free(ptr)
#endif

#define MAX_MEMPOOL_NAME_LEN 16

typedef struct {
    cs_uint16  seq_id;
    cs_uint16 alloc_line;
    cs_uint16  module_id;
    cs_uint16 mem_pool_id;
    cs_uint32 mem_size;
    cs_int8    *func_name;
} cs_mem_rsvd_header;

typedef struct {
    cs_uint16 pool_id;
    cs_uint16 free;
    cs_uint32 element_num;
    cs_uint32 element_size;
    cs_uint32 real_element_size;
    cs_uint32 free_elem_num;
    cs_uint32 peek_value;
    cs_uint32 mutex_id;
    cs_int8     pool_name[MAX_MEMPOOL_NAME_LEN];
    cs_uint32   *buf_ptr_list;
    cs_uint8   *mem_bit_map;
} cs_mempool_t;


#define MEM_IS_FREE 0
#define MEM_IS_BUSY 1

#define CS_INVALID_POOL_ID 0

#define iros_mempool_log if(mempool_debug) cs_printf

extern cs_int32 cs_mempool_init(cs_int32 sys_max_pool_num);
extern cs_int32 cs_mempool_create(cs_uint32 *pool_id ,cs_int8 *pool_name , cs_int32 elem_size ,cs_int32 elem_num);
extern cs_uint8 *cs_mem_malloc(cs_uint32 pool_id);
extern void cs_mem_free(cs_uint8 *pbuf);
extern cs_int32 cs_mempool_destroy(cs_uint32 pool_id);
extern void cs_pool_show(cs_uint32 pool_id);
extern cs_uint32 cs_pool_element_size_get(cs_uint32 pool_id);
extern cs_int8 *cs_pool_name_get(cs_uint32 pool_id);
extern cs_uint32 cs_pool_number();

#endif

