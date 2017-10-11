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
 * mempool.c
 *
 * $Id: mempool.c,v 1.1.2.1.2.1.8.5.8.1 2011/03/02 07:19:31 zsun Exp $
 */

#include "cs_types.h"
#include "osal_common.h"
#include "mempool.h"

cs_mempool_t *sys_mem_pool = NULL;
cs_int32 sys_mempool_num = 0;
cs_uint32 sys_mempool_mutex = 0;
cs_int32 mempool_debug=0;
cs_uint32 pkt_pool_id = CS_INVALID_POOL_ID;

#ifdef MEMPOOL_EX
sys_pool_cfg_t sys_pool_cfg[] = {
    {64,            800},
    {128,           800},
    {256,           200},
    {512,           100},
    {1024,         200},
    {1600,         200},
    {4096,         4 },
    {8192,         2 },
    {16384,        2 },
};

cs_uint32 sys_module_cfg_mutex = 0;
sys_module_desc_t sys_module_cfg[IROS_MID_MAX];

cs_uint32 *sys_pool_ex_list = NULL;
cs_uint32 sys_pool_ex_num = 0;
cs_boolean sys_pool_ex_more_flag = TRUE;
cs_int32 sys_pool_ex_init(cs_boolean more_mem_flag);

#ifdef HAVE_MEM_TRACE
cs_uint32 mem_stat_mutex = 0;
sys_mem_stat_t mem_stat[MAX_SYS_MEM_STAT];
#endif

#endif


extern int  sprintf(char *, const char *, ...);

/***********************************************************************
* NAME : cs_mempool_init
* DESC :
*             Initialize memory-pool's headers of the whole system, allocate a pool's mutex and
*             the memory of all memory-pool's header.
*             NOTICE:
*                  For ECOS system , If you want to initialize 10 memory-pools for APP, and you
*                  also have 10 queues already, you must allocate 20 memory-pools at least.
*                  Because each ECOS's queue requires only one queue to store the message.
* ARGUMENT:
*             sys_max_pool_num - the total number of whole memory-pools
* RETURN :
*             If initialize successfully then return CS_E_OSAL_OK , else return CS_E_OSAL_ERR.
***********************************************************************/
cs_int32 cs_mempool_init(cs_int32 sys_max_pool_num)
{
    cs_uint32 total_size;
    cs_int32 i,pool_index;

    if (CS_E_OSAL_OK != cs_mutex_init(&sys_mempool_mutex, "sys_pool_mutex", 0)) {
        iros_mempool_log("\r\n initialize system pool mutex failed ");
        return CS_E_OSAL_ERR;
    }

    total_size = sys_max_pool_num * sizeof(cs_mempool_t);
    sys_mem_pool = (cs_mempool_t *)malloc(total_size);
    if (sys_mem_pool == NULL) {
        iros_mempool_log("\r\n initialize system pool failed ");
        cs_mutex_destroy(sys_mempool_mutex);
        return CS_E_OSAL_ERR;
    }

    for (i = 0 ; i < sys_max_pool_num ; i++) {
        memset(&sys_mem_pool[i] , 0 , sizeof(cs_mempool_t));
        sys_mem_pool[i].pool_id = i + 1;
        sys_mem_pool[i].free = MEM_IS_FREE;
    }

    sys_mempool_num = sys_max_pool_num;

#ifdef MEMPOOL_EX
#ifndef HAVE_SYS_MEMPOOL
    if(sys_pool_ex_init(TRUE) != CS_E_OSAL_OK) {
        iros_mempool_log("\r\n initialize system pool failed ");
        cs_mutex_destroy(sys_mempool_mutex);
        cs_mutex_destroy(sys_module_cfg_mutex);
        free((void *)sys_mem_pool);
        return CS_E_OSAL_ERR;
    }
#else
    if(CS_E_OSAL_OK != cs_mempool_create(&pkt_pool_id, "Pkt", MAX_PKT_LEN, MAX_PKT_NUM))
        cs_printf("\r\n Create pkt pool failed.");
#endif
    if(CS_E_OSAL_OK != cs_mutex_init(&sys_module_cfg_mutex, "sys_module_mut", 0)) {
        iros_mempool_log("\r\n initialize sys_module_cfg_mutex failed ");
        return CS_E_OSAL_ERR;
    }

    for(i = 0 ; i < IROS_MID_MAX ; i++)
        sys_module_cfg[i].module_id = CS_INVALID_MODULE_ID;

    for(i = 0 ; i < sys_mod_number; i++) {
        memset(&sys_module_cfg[i] , 0 , sizeof(sys_module_desc_t));
        sys_module_cfg[i].module_id = sys_mod_desc[i].mod_id;
        if(sys_pool_ex_num > 0) {
            sys_module_cfg[i].pool_counter = (pool_counter_t *)malloc(sys_pool_ex_num*sizeof(pool_counter_t));
            if(sys_module_cfg[i].pool_counter == NULL) {
                iros_mempool_log("\r\n create module's poolcounter failed");
                return CS_E_OSAL_ERR;
            }
            memset((void *)sys_module_cfg[i].pool_counter , 0 , sys_pool_ex_num*sizeof(pool_counter_t));
            for(pool_index = 0 ; pool_index < sys_pool_ex_num ; pool_index++) {
                sys_module_cfg[i].pool_counter[pool_index].pool_id = (cs_uint16)sys_pool_ex_list[pool_index];
            }
        }
    }

#ifdef HAVE_MEM_TRACE
    if(cs_mutex_init(&mem_stat_mutex , "mem_stat_mutex", 0) != CS_E_OSAL_OK) {
        iros_mempool_log("\r\n create mem_stat_mutex failed ");
        return CS_E_OSAL_ERR;
    }

    for (i = 0 ; i < MAX_SYS_MEM_STAT ; i++) {
        memset(&mem_stat[i] , 0 , sizeof(sys_mem_stat_t));
        mem_stat[i].module_id = CS_INVALID_MODULE_ID;
        mem_stat[i].free = MEM_IS_FREE;
    }
#endif
#endif

    return CS_E_OSAL_OK;
}

/***********************************************************************
* NAME : cs_mempool_create
* DESC :
*             Allocate a memory-pool's header from the pool of the memory-pool's header and
*             allocate a line buffer for the memory-pool. The real size of the line buffer is not
*             (elem_size*elem_num) , but is (elem_size + sizeof(cs_mem_rsvd_header)) * elem_num.
*             The reserved header of each memory block records some useful information.
*             Reserved header of each memory is invisible to you.
*             Finally , the function returns an availability pool-id
* ARGUMENT:
*             pool_id - pointer of the pool_id
*             elem_size - size of each memory block
*             elem_num - total number of whole memory block
* RETURN :
*             If create successfully then return CS_E_OSAL_OK , else return CS_E_OSAL_ERR.
***********************************************************************/
cs_int32 cs_mempool_create(
    cs_uint32 *pool_id ,
    cs_int8 *pool_name,
    cs_int32 elem_size ,
    cs_int32 elem_num)
{
    cs_int32 i,j;
    cs_mem_rsvd_header *rsvd_header = NULL;
    cs_int8 mutex_name[32];
    cs_uint32 pool_name_len=0;
    cs_int8 *ptr = NULL;
    cs_uint32 real_size=0;
    cs_uint32 bitmap_len=0;

    if(pool_id == NULL || pool_name == NULL) {
        iros_mempool_log("\r\n cs_mempool_create failed , cause some parameter is null");
        return CS_E_OSAL_ERR;
    }

    cs_mutex_lock(sys_mempool_mutex);
    for (i = 0 ; i < sys_mempool_num ; i++) {
        if (sys_mem_pool[i].free == MEM_IS_FREE) {
            break;
        }
    }

    if (i >= sys_mempool_num) {
        cs_mutex_unlock(sys_mempool_mutex);
        iros_mempool_log("\r\n there is no free mempool");
        return CS_E_OSAL_ERR;
    }

    *pool_id = i + 1;
    memset(mutex_name , 0 , sizeof(mutex_name));
    sprintf(mutex_name, "pool_mutex_%d" , (cs_uint32)*pool_id);
    if (CS_E_OSAL_OK != cs_mutex_init(&sys_mem_pool[*pool_id-1].mutex_id, mutex_name, 0)) {
        cs_mutex_unlock(sys_mempool_mutex);
        iros_mempool_log("\r\n mempool's mutex create failed");
        return CS_E_OSAL_ERR;
    }

    sys_mem_pool[*pool_id-1].buf_ptr_list = (cs_uint32 *)malloc(elem_num*sizeof(cs_uint32));
    if(sys_mem_pool[*pool_id-1].buf_ptr_list == NULL) {
        iros_mempool_log("\r\n pool %d ptr list allocate failed",*pool_id);
        cs_mutex_unlock(sys_mempool_mutex);
        return CS_E_OSAL_ERR;
    }
    memset(sys_mem_pool[*pool_id-1].buf_ptr_list , 0 ,elem_num*sizeof(cs_uint32));

    real_size = elem_size;
    if((real_size%4) != 0) {
        real_size = ((real_size+0x3)>>2)<<2;
    }

    if(elem_num%32 == 0)
        bitmap_len = elem_num/8;
    else
        bitmap_len = (elem_num/32)*4 + 4;

    sys_mem_pool[*pool_id-1].mem_bit_map = (cs_uint8 *)malloc(bitmap_len);
    if(sys_mem_pool[*pool_id-1].mem_bit_map == NULL)
    {
        cs_mutex_unlock(sys_mempool_mutex);
        return CS_E_OSAL_ERR;
    }
    memset(sys_mem_pool[*pool_id-1].mem_bit_map , 0xFF , bitmap_len);

    for(i = 0 ; i < elem_num ; i++) {
        ptr = (cs_int8 *)malloc(real_size + sizeof(cs_mem_rsvd_header));
        if(ptr == NULL) {
            for(j = 0 ; j < elem_num ; j++) {
                if(sys_mem_pool[*pool_id-1].buf_ptr_list[i] != 0) {
                    free((void *)sys_mem_pool[*pool_id-1].buf_ptr_list[i]);
                }
            }
            iros_mempool_log("\r\n Fail to alloc mem for pool %d, elem %d, No. %d", *pool_id, elem_size, i);
            cs_mutex_unlock(sys_mempool_mutex);
            return CS_E_OSAL_ERR;
        }
        memset(ptr , 0 , real_size + sizeof(cs_mem_rsvd_header));
        sys_mem_pool[*pool_id-1].buf_ptr_list[i] = (cs_uint32)ptr;
        ptr = NULL;
    }

    sys_mem_pool[*pool_id-1].free = MEM_IS_BUSY;
    sys_mem_pool[*pool_id-1].element_num = elem_num;
    sys_mem_pool[*pool_id-1].element_size = elem_size;
    sys_mem_pool[*pool_id-1].real_element_size = real_size;
    sys_mem_pool[*pool_id-1].free_elem_num = elem_num;
    memset(sys_mem_pool[*pool_id-1].pool_name , 0 , MAX_MEMPOOL_NAME_LEN);
    pool_name_len = (strlen(pool_name) > MAX_MEMPOOL_NAME_LEN-1)?(MAX_MEMPOOL_NAME_LEN-1):strlen(pool_name);
    memcpy(sys_mem_pool[*pool_id-1].pool_name , pool_name , pool_name_len);
    for (i = 0 ; i < elem_num ; i++) {
        rsvd_header = (cs_mem_rsvd_header *)sys_mem_pool[*pool_id-1].buf_ptr_list[i];
        /*rsvd_header->free = MEM_IS_FREE;*/
        rsvd_header->seq_id = i;
        rsvd_header->mem_pool_id = (cs_uint16)(*pool_id);
    }

    cs_mutex_unlock(sys_mempool_mutex);

    return CS_E_OSAL_OK;
}

/***********************************************************************
* NAME : cs_mem_malloc
* DESC :
*             Allocate a memory block from the specific pool which ID is pool_id.
*
*                                        --> the pointer return to you
*                                        |
*                 ------------------------------
*                |  rsvd header |      line buffer            |
*                 ------------------------------
*
* ARGUMENT:
*             pool_id - Identification of a memory-pool
* RETURN :
*             If malloc successfully then return the pointer of a line buffer , else return NULL
***********************************************************************/
cs_uint8 *cs_mem_malloc(cs_uint32 pool_id)
{
    cs_mempool_t *pool = NULL;
    cs_mem_rsvd_header *rsvd_header = NULL;
    cs_int32 i;

    if (pool_id > sys_mempool_num || pool_id == CS_INVALID_POOL_ID) {
        iros_mempool_log("\r\n mem alloc failed, cause pool_id is invalid(%d)",pool_id);
        return (cs_uint8 *)NULL;
    }

    pool = (cs_mempool_t *) & sys_mem_pool[pool_id-1];
    if (pool->free == MEM_IS_FREE) {
        iros_mempool_log("\r\n memory pool which id is pool_id is free");
        return (cs_uint8 *)NULL;
    }

    cs_mutex_lock(pool->mutex_id);
    if (pool->free_elem_num <= 0) {
        iros_mempool_log("\r\n no memory is free in the pool(%d)",pool_id);
        cs_mutex_unlock(pool->mutex_id);
        return (cs_uint8 *)NULL;
    }

#if 0
    for (i = 0 ; i < pool->element_num ; i++) {
        rsvd_header = (cs_mem_rsvd_header *)pool->buf_ptr_list[i];
        if (rsvd_header->free == MEM_IS_FREE) {
            break;
        }
    }
#endif

    if((i = cs_bit_get_1((cs_int8 *)pool->mem_bit_map , pool->element_num)) == -1)
    {
        iros_mempool_log("\r\n no memory is free in the pool(%d).",pool_id);
        cs_mutex_unlock(pool->mutex_id);
        return (cs_uint8 *)0;
    }
    cs_bit_clr((cs_int8 *)pool->mem_bit_map, i);

    rsvd_header = (cs_mem_rsvd_header *)pool->buf_ptr_list[i];

    /*rsvd_header->free = MEM_IS_BUSY;*/
    rsvd_header->module_id = CS_INVALID_MODULE_ID;
    pool->free_elem_num--;
    if(pool->element_num - pool->free_elem_num > pool->peek_value) {
        pool->peek_value = pool->element_num - pool->free_elem_num;
    }
    cs_mutex_unlock(pool->mutex_id);

    return (cs_uint8 *)&rsvd_header[1];
}

/***********************************************************************
* NAME : cs_mem_free
* DESC :
*             Free the memory which you allocated.
* ARGUMENT:
*             pbuf - pointer of the line buffer
* RETURN :
*             N/A
***********************************************************************/

void cs_mem_free(cs_uint8 *pbuf)
{
    cs_mempool_t *pool = NULL;
    cs_mem_rsvd_header *rsvd_header = NULL;
    cs_int32 pool_id;

    if (pbuf == NULL) {
        iros_mempool_log("\r\n can not free a NULL pointer");
        return;
    }

    rsvd_header = (cs_mem_rsvd_header *)(pbuf - sizeof(cs_mem_rsvd_header));
    pool_id = rsvd_header->mem_pool_id;
    if (pool_id > sys_mempool_num || pool_id == CS_INVALID_POOL_ID) {
        iros_mempool_log("\r\n free a invalid pool(%d)",pool_id);
        return ;
    }

    pool = (cs_mempool_t *) & sys_mem_pool[pool_id-1];
    if (pool->free == MEM_IS_FREE) {
        iros_mempool_log("\r\n return the free memory to an invalid pool");
        return ;
    }

    /*rsvd_header->free = MEM_IS_FREE;*/
    cs_mutex_lock(pool->mutex_id);
    pool->free_elem_num++;
    cs_bit_set((cs_int8 *)pool->mem_bit_map , rsvd_header->seq_id);
    cs_mutex_unlock(pool->mutex_id);

    return ;
}

/***********************************************************************
* NAME : cs_mempool_destroy
* DESC :
*             Delete the memory-pool which identification is 'pool_id'
*             NOTICE:
*                 Even if only one memory block is allocated by some APP , you can not destory this
*                 pool.
* ARGUMENT:
*             pool_id - Identification of the specific memory-pool
* RETURN :
*             N/A
***********************************************************************/
cs_int32 cs_mempool_destroy(cs_uint32 pool_id)
{
    cs_mempool_t *pool = NULL;
    cs_int32 i;

    if (pool_id >  sys_mempool_num || pool_id == CS_INVALID_POOL_ID) {
        iros_mempool_log("\r\n can not destroy a invalid pool(%d)",pool_id);
        return CS_E_OSAL_ERR;
    }

    pool = &sys_mem_pool[pool_id-1];
    if (pool->free == MEM_IS_FREE) {
        return CS_E_OSAL_OK;
    }

    if (pool->free_elem_num != pool->element_num) {
        iros_mempool_log("\r\n If just one memory is allocated from pool, you cann't destroy it");
        return CS_E_OSAL_ERR;
    }

    cs_mutex_lock(sys_mempool_mutex);
    for(i = 0 ; i < pool->element_num ; i++) {
        free((void *)pool->buf_ptr_list[i]);
    }
    free((void *)pool->buf_ptr_list);

    pool->free = MEM_IS_FREE;
    cs_mutex_destroy(pool->mutex_id);
    pool->element_num = 0;
    pool->element_size = 0;
    pool->free_elem_num = 0;
    cs_mutex_unlock(sys_mempool_mutex);

    return CS_E_OSAL_OK;
}

cs_int8 *cs_pool_name_get(cs_uint32 pool_id)
{

    if(pool_id == CS_INVALID_POOL_ID || pool_id > sys_mempool_num) {
        return NULL;
    }

    return (cs_int8 *)sys_mem_pool[pool_id - 1].pool_name;
}

cs_uint32 cs_pool_element_size_get(cs_uint32 pool_id)
{
    cs_mempool_t *pool = NULL;

    if (pool_id >  sys_mempool_num || pool_id == CS_INVALID_POOL_ID) {
        return 0;
    }

    pool = &sys_mem_pool[pool_id-1];
    if (pool->free == MEM_IS_FREE) {
        return 0;
    }

    return pool->element_size;
}

void cs_pool_show(cs_uint32 pool_id)
{
    cs_mempool_t *pPool = NULL;
    cs_int32 i;
    cs_uint32 pid,elem_num, elem_size,free_num,peek_num;
    cs_uint8  *begin_addr = NULL, *end_addr = NULL;
    cs_int8 begin_str[12];
    cs_int8 end_str[12];

    if(pool_id > sys_mempool_num) {
        return ;
    }

    cs_printf("\r\n===============================================================================");
    cs_printf("\r\n Pid = pool id , E-n = element number , E-s = element size");
    cs_printf("\r\n Used = used number , Free = free number , Peek = peek value");
    cs_printf("\r\n Addr-B = start address , Addr-E = end address , Name = pool's name");
    cs_printf("\r\n===============================================================================");
    cs_printf("\r\n %-5s%-16s%-8s%-6s%-6s%-6s%-6s%-13s%-13s","Pid","Name","E-s","E-n","Free","Used","Peek","Addr-B","Addr-E");
    if(pool_id == CS_INVALID_POOL_ID)
    {
        for(i = 0 ; i < sys_mempool_num ; i++)
        {
            pPool = &sys_mem_pool[i];
            if(pPool->free == MEM_IS_FREE) {
                continue;
            }

            pid = i+1;
            elem_num = pPool->element_num;
            elem_size = pPool->element_size;
            free_num = pPool->free_elem_num;
            peek_num = pPool->peek_value;
            begin_addr = (cs_uint8 *)pPool->buf_ptr_list;
            end_addr = (cs_uint8 *)pPool->buf_ptr_list + elem_num*sizeof(cs_uint32);
            memset(begin_str , 0 , sizeof(begin_str));
            memset(end_str , 0 , sizeof(end_str));
            sprintf(begin_str , "0x%08x", (cs_uint32)begin_addr);
            sprintf(end_str , "0x%08x", (cs_uint32)end_addr);
            cs_printf("\r\n %-5d%-16s%-8d%-6d%-6d%-6d%-6d%-13s%-13s",pid,pPool->pool_name,elem_size,
                                                            elem_num,free_num,elem_num-free_num,peek_num,begin_str,end_str);
        }
    }
    else
    {
        pPool = &sys_mem_pool[pool_id-1];
        if(pPool->free == MEM_IS_FREE) {
            return;
        }

        pid = pool_id;
        elem_num = pPool->element_num;
        elem_size = pPool->element_size;
        free_num = pPool->free_elem_num;
        peek_num = pPool->peek_value;
        begin_addr = (cs_uint8 *)pPool->buf_ptr_list;
        end_addr = (cs_uint8 *)pPool->buf_ptr_list + elem_num*sizeof(cs_uint32);
        memset(begin_str , 0 , sizeof(begin_str));
        memset(end_str , 0 , sizeof(end_str));
        sprintf(begin_str , "0x%08x", (cs_uint32)begin_addr);
        sprintf(end_str , "0x%08x", (cs_uint32)end_addr);
        cs_printf("\r\n %-5d%-16s%-8d%-6d%-6d%-6d%-6d%-13s%-13s",pid,pPool->pool_name,elem_size,
                                                        elem_num,free_num,elem_num-free_num,peek_num,begin_str,end_str);
    }
    cs_printf("\r\n===============================================================================");

    cs_printf("\r\n\r\n");
}

cs_uint32 cs_pool_number()
{
    cs_mempool_t *pPool = NULL;
    cs_int32 i;
    cs_uint32 count=0;

    for(i = 0 ; i < sys_mempool_num ; i++)
    {
        pPool = &sys_mem_pool[i];
        if(pPool->free == MEM_IS_BUSY)
            count++;
    }

    return count;
}

#ifdef MEMPOOL_EX
cs_int32 sys_pool_ex_init(cs_boolean more_mem_flag)
{
    cs_int32 i;
    cs_int8 pool_name[16];

    sys_pool_ex_num = sizeof(sys_pool_cfg)/sizeof(sys_pool_cfg[0]);
    sys_pool_ex_list = (cs_uint32 *)malloc(sizeof(cs_uint32)*sys_pool_ex_num);
    if(sys_pool_ex_list == NULL)
    {
        cs_printf("\r\n Create system pool-ex failed");
        return CS_E_OSAL_ERR;
    }

    for(i = 0 ; i < sys_pool_ex_num ; i++)
    {
        memset(pool_name , 0 , sizeof(pool_name));
        sprintf(pool_name , "sys_pool_%d", i+1);
        if(CS_E_OSAL_OK != cs_mempool_create(&sys_pool_ex_list[i], pool_name,
                                                        sys_pool_cfg[i].elem_size, sys_pool_cfg[i].elem_num))
        {
            cs_printf("\r\n Create %d bytes pool which total num is %d failed",
                                            sys_pool_cfg[i].elem_size,sys_pool_cfg[i].elem_num);
            return CS_E_OSAL_ERR;
        }
    }

    sys_pool_ex_more_flag = more_mem_flag;
    return CS_E_OSAL_OK;
}

sys_module_desc_t *cs_module_get_by_id(cs_uint32 module_id)
{
    cs_int32 i;

    for(i = 0 ; i < IROS_MID_MAX ; i++)
    {
        if(sys_module_cfg[i].module_id == module_id) {
            return (sys_module_desc_t *)&sys_module_cfg[i];
        }
    }

    return (sys_module_desc_t *)NULL;
}

#ifdef HAVE_MEM_TRACE
void cs_updata_mem_stat(cs_mem_rsvd_header *pHeader ,cs_int32 flag)
{
    cs_int32 i;
    cs_int32 free_idx = -1;

    /*cs_mutex_lock(mem_stat_mutex);*/
    for (i = 0 ; i < MAX_SYS_MEM_STAT ; i++) {
        if(free_idx == -1 && mem_stat[i].free == MEM_IS_FREE) {
            free_idx = i;
        }

        if(mem_stat[i].module_id == pHeader->module_id &&
            mem_stat[i].func_name == pHeader->func_name &&
            mem_stat[i].alloc_line == pHeader->alloc_line &&
            mem_stat[i].free == MEM_IS_BUSY) {
            if(flag)
                mem_stat[i].count++;
            else {
                if(mem_stat[i].count)
                    mem_stat[i].count--;

                if(mem_stat[i].count == 0) {
                    memset(&mem_stat[i] , 0 , sizeof(sys_mem_stat_t));
                    mem_stat[i].free = MEM_IS_FREE;
                    mem_stat[i].module_id = CS_INVALID_MODULE_ID;
                }
            }

            /*cs_mutex_unlock(mem_stat_mutex);*/
            return;
        }
    }

    if(free_idx == -1) {
        iros_mempool_log("\r\n no free mem_stat slot");
        /*cs_mutex_unlock(mem_stat_mutex);*/
        return;
    }

    if(!flag) {
        iros_mempool_log("\r\n no alloc , but you try to free");
        /*cs_mutex_unlock(mem_stat_mutex);*/
        return;
    }
    mem_stat[free_idx].free = MEM_IS_BUSY;
    mem_stat[free_idx].module_id = pHeader->module_id;
    mem_stat[free_idx].alloc_line = pHeader->alloc_line;
    mem_stat[free_idx].func_name = pHeader->func_name;
    mem_stat[free_idx].count = 1;

    /*cs_mutex_unlock(mem_stat_mutex);*/

    return;
}

void cs_module_mem_stat_show(cs_uint16 module_id)
{
    cs_int32 i;

    if(module_id >= IROS_MID_MAX)
        return;

    /*cs_mutex_lock(mem_stat_mutex);*/
    for(i = 0 ; i < MAX_SYS_MEM_STAT ; i++) {
        if(mem_stat[i].free == MEM_IS_FREE)
            continue;

        if(mem_stat[i].module_id != module_id)
            continue;

        cs_printf("\r\n   %32s(%-5d) : %-8d",mem_stat[i].func_name,mem_stat[i].alloc_line,mem_stat[i].count);
    }
    /*cs_mutex_unlock(mem_stat_mutex);*/
}
#endif

void cs_updata_module_cfg(cs_mem_rsvd_header *pHeader ,cs_int32 flag)
{
    cs_int32 i;
    sys_module_desc_t *pModule = NULL;
    cs_uint32 module_id;
    cs_uint32 pool_id;
    cs_uint32 size;

    if(pHeader == NULL) {
        return;
    }

    module_id = pHeader->module_id;
    pool_id = pHeader->mem_pool_id;
    size = pHeader->mem_size;
    if(module_id >= IROS_MID_MAX) {
        iros_mempool_log("\r\n can not update an invalid module's counter");
        return ;
    }

    cs_mutex_lock(sys_module_cfg_mutex);
    pModule = cs_module_get_by_id(module_id);
    if(pModule == NULL) {
        cs_mutex_unlock(sys_module_cfg_mutex);
        return ;
    }

    if(size <= 64) {
        if(flag)
            pModule->count_64bytes++;
        else
            pModule->count_64bytes--;
    }
    else if(size > 64 && size <= 128){
        if(flag)
            pModule->count_65_128bytes++;
        else
            pModule->count_65_128bytes--;
    }
     else if(size > 128 && size <= 256){
        if(flag)
            pModule->count_129_256bytes++;
        else
            pModule->count_129_256bytes--;
    }
     else if(size > 256 && size <= 512){
        if(flag)
            pModule->count_257_512bytes++;
        else
            pModule->count_257_512bytes--;
    }
     else if(size > 512 && size <= 1024){
        if(flag)
            pModule->count_513_1024bytes++;
        else
            pModule->count_513_1024bytes--;
    }
     else if(size > 1024 && size <= 2048){
        if(flag)
            pModule->count_1k_2kbytes++;
        else
            pModule->count_1k_2kbytes--;
    }
     else if(size > 2048 && size <= 4096){
        if(flag)
            pModule->count_2k_4kbytes++;
        else
            pModule->count_2k_4kbytes--;
    }
     else if(size > 4096 && size <= 8192){
        if(flag)
            pModule->count_4k_8kbytes++;
        else
            pModule->count_4k_8kbytes--;
    }
     else{
        if(flag)
            pModule->count_8kbytes++;
        else
            pModule->count_8kbytes--;
    }

#ifdef HAVE_MEM_TRACE
    cs_updata_mem_stat(pHeader, flag);
#endif

    if(pool_id == CS_INVALID_POOL_ID || pModule->pool_counter == NULL) {
        cs_mutex_unlock(sys_module_cfg_mutex);
        return;
    }

    for(i = 0 ; i < sys_pool_ex_num ; i++)
    {
        if(pModule->pool_counter[i].pool_id != (cs_uint16)pool_id) {
            continue;
        }

        if(flag) {
            pModule->pool_counter[i].count++;
        }
        else {
            pModule->pool_counter[i].count--;
        }

        cs_mutex_unlock(sys_module_cfg_mutex);
        return;
    }
    cs_mutex_unlock(sys_module_cfg_mutex);

    return;
}

cs_uint8 *cs_sys_pool_malloc(cs_uint32 module_id , cs_int32 size , cs_int8 *func_name , cs_int32 line ,cs_int32 trace_flag)
{
    cs_int32 i;
    cs_uint8 *pBuf = NULL;
    cs_mem_rsvd_header *pHeader = NULL;
    cs_int32 real_size = 0;

    if(size <= 0 || module_id >= IROS_MID_MAX)
    {
        iros_mempool_log("\r\n cs_sys_pool_malloc failed(%d , %d)",module_id , size);
        return NULL;
    }

#ifdef HAVE_SYS_MEMPOOL
    if(module_id == IROS_MID_PACKET)
        pBuf = (cs_uint8 *)cs_mem_malloc(pkt_pool_id);
    else
        goto ALLOCATE_FOR_ECOS;

    if(pBuf == NULL)
        return NULL;

    pHeader = (cs_mem_rsvd_header *)(pBuf - sizeof(cs_mem_rsvd_header));
    pHeader->alloc_line = line;
    pHeader->module_id = (cs_uint16)module_id;
    pHeader->func_name = func_name;
    pHeader->mem_size = size;
    if(trace_flag)
        cs_updata_module_cfg(pHeader,1);
    return (cs_uint8 *)pBuf;
#endif

    for(i = 0 ; i < sys_pool_ex_num ; i++)
    {
        if(size > cs_pool_element_size_get(sys_pool_ex_list[i])) {
            continue;
        }

        pBuf = (cs_uint8 *)cs_mem_malloc(sys_pool_ex_list[i]);
        if(pBuf == NULL)
        {
            continue;
        }

        pHeader = (cs_mem_rsvd_header *)(pBuf - sizeof(cs_mem_rsvd_header));
        pHeader->alloc_line = line;
        pHeader->module_id = (cs_uint16)module_id;
        pHeader->func_name = func_name;
        pHeader->mem_size = size;
        if(trace_flag)
            cs_updata_module_cfg(pHeader,1);
        return (cs_uint8 *)pBuf;
    }

    if(!sys_pool_ex_more_flag) {
        /*need add log information*/
        iros_mempool_log("\r\n sys_pool_ex_more_flag is %d",sys_pool_ex_more_flag);
        return NULL;
    }

#ifdef HAVE_SYS_MEMPOOL
ALLOCATE_FOR_ECOS:
#endif
    real_size = size + sizeof(cs_mem_rsvd_header);
    pBuf = (cs_uint8 *)malloc(real_size);
    if(pBuf != NULL)
    {
        pHeader = (cs_mem_rsvd_header *)pBuf;
        pHeader->alloc_line = line;
        pHeader->func_name = func_name;
        pHeader->mem_size = size;
        pHeader->module_id = (cs_uint16)module_id;
        pHeader->mem_pool_id = CS_INVALID_POOL_ID;
        /*pHeader->free = MEM_IS_BUSY;*/
        pBuf += sizeof(cs_mem_rsvd_header);
        if(trace_flag)
            cs_updata_module_cfg(pHeader , 1);
    }

    return (cs_uint8 *)pBuf;
}

void cs_sys_pool_free(cs_uint8 *ptr , cs_int8 *func_name , cs_int32 line , cs_int32 trace_flag)
{
    cs_mem_rsvd_header *pHeader = NULL;

    if(ptr == NULL) {
        return ;
    }

    pHeader = (cs_mem_rsvd_header *)(ptr-sizeof(cs_mem_rsvd_header));
    if(pHeader->mem_pool_id == CS_INVALID_POOL_ID) {
        /*need add log infromation*/
        if(trace_flag)
            cs_updata_module_cfg(pHeader,0);
        free((void *)pHeader);
        return;
    }

    if(trace_flag)
        cs_updata_module_cfg(pHeader,0);
    cs_mem_free(ptr);
    return;
}

static cs_uint32 cs_module_pool_counter(sys_module_desc_t *pModule , cs_uint32 pool_id)
{
    int i ;

    if(pModule == NULL) {
        return 0;
    }

    for(i = 0 ; i < sys_pool_ex_num ; i++)
    {
        if(pModule->pool_counter[i].pool_id == (cs_uint16)pool_id) {
            return pModule->pool_counter[i].count;
        }
    }

    return 0;
}

static void cs_sys_pool_show_cmd(cs_uint32 pool_id)
{
    int i;
    cs_uint32 counter=0;

    if(pool_id == CS_INVALID_POOL_ID || pool_id > sys_mempool_num) {
        return ;
    }

    cs_printf("\r\n --> %s (id:%d elem_size:%d elem_num:%d)" ,sys_mem_pool[pool_id-1].pool_name,pool_id,
                            sys_mem_pool[pool_id-1].element_size , sys_mem_pool[pool_id-1].element_num);
    cs_printf("\r\n ------------------------------------------------------");
    for(i = 0 ; i < IROS_MID_MAX ; i++)
    {
        counter = cs_module_pool_counter(&sys_module_cfg[i], pool_id);
        if(counter != 0)
            cs_printf("\r\n  %20s(%2d) : %-10d" , cs_sys_mod_desc_get(sys_module_cfg[i].module_id),
                                                                            sys_module_cfg[i].module_id,
                                                                            counter);
    }
    cs_printf("\r\n");
}

void cs_sys_pool_show(cs_uint32 pool_id)
{
    int i;

    cs_printf("\r\n System mempool's information:");
    cs_printf("\r\n===============================================================================");
    if(sys_pool_ex_list == NULL || sys_pool_ex_num == 0) {
        cs_printf("\r\n !!There isn't any system pool for current system");
        return;
    }
    if(pool_id == CS_INVALID_POOL_ID)
    {
        for(i = 0 ; i < sys_pool_ex_num ; i++)
        {
            cs_sys_pool_show_cmd(sys_pool_ex_list[i]);
            cs_thread_delay(10);
        }
    }
    else {
        cs_sys_pool_show_cmd(pool_id);
    }

    return;
}

void cs_sys_module_show(cs_uint32 module_id)
{
    int i=0,j=0,index=1;
    cs_uint32 pool_id = CS_INVALID_POOL_ID;
    sys_module_desc_t *pModule = NULL;
    cs_int8 *pModuleName = NULL;
    cs_uint32 total_num = 0;

    if(module_id == CS_INVALID_MODULE_ID)
    {
        for(i = 0 ; i < IROS_MID_MAX ; i++)
        {
            index = 1;
            module_id = sys_module_cfg[i].module_id;
            pModule = &sys_module_cfg[i];
            if(pModule == NULL || module_id == CS_INVALID_MODULE_ID)
                continue;

            pModuleName = cs_sys_mod_desc_get(module_id);
            if(pModuleName != NULL)
            {
                total_num = 0;
                total_num += pModule->count_64bytes +
                            pModule->count_65_128bytes +
                            pModule->count_129_256bytes +
                            pModule->count_257_512bytes +
                            pModule->count_513_1024bytes +
                            pModule->count_1k_2kbytes +
                            pModule->count_2k_4kbytes +
                            pModule->count_4k_8kbytes +
                            pModule->count_8kbytes;
                if(total_num == 0)
                    continue;
                cs_printf("\r\n ======================================================");
                cs_printf("\r\n [%s(%d)]", pModuleName,module_id);
                if(sys_pool_ex_num > 0)
                    cs_printf("\r\n  %d. Allocation in SYS-POOL Overview",index++);
                for(j = 0 ; j < sys_pool_ex_num ; j++) {
                    pool_id = sys_pool_ex_list[j];
                    cs_printf("\r\n   %20s(%10d bytes) : %-20d",sys_mem_pool[pool_id-1].pool_name,
                        sys_mem_pool[pool_id-1].element_size , cs_module_pool_counter(pModule, pool_id));
                }

                cs_printf("\r\n  %d. Allocation Length Details (bytes)",index++);
                if(pModule->count_64bytes)
                    cs_printf("\r\n  %20s : %d", "[0   ~       64]",pModule->count_64bytes);
                if(pModule->count_65_128bytes)
                    cs_printf("\r\n  %20s : %d", "[65  ~      128]",pModule->count_65_128bytes);
                if(pModule->count_129_256bytes)
                    cs_printf("\r\n  %20s : %d", "[129 ~      256]",pModule->count_129_256bytes);
                if(pModule->count_257_512bytes)
                    cs_printf("\r\n  %20s : %d", "[257 ~      512]",pModule->count_257_512bytes);
                if(pModule->count_513_1024bytes)
                    cs_printf("\r\n  %20s : %d", "[513 ~     1024]",pModule->count_513_1024bytes);
                if(pModule->count_1k_2kbytes)
                    cs_printf("\r\n  %20s : %d", "[1k  ~       2k]",pModule->count_1k_2kbytes);
                if(pModule->count_2k_4kbytes)
                    cs_printf("\r\n  %20s : %d", "[2k  ~       4k]",pModule->count_2k_4kbytes);
                if(pModule->count_4k_8kbytes)
                    cs_printf("\r\n  %20s : %d", "[4k  ~       8k]",pModule->count_4k_8kbytes);
                if(pModule->count_8kbytes)
                    cs_printf("\r\n  %20s : %d", "[8k  ~ Infinite]",pModule->count_8kbytes);

                #ifdef HAVE_MEM_TRACE
                cs_printf("\r\n  %d. Allocation Details -  function(line) : count",index++);
                cs_module_mem_stat_show(module_id);
                #endif

                cs_thread_delay(10);
            }
        }
    }
    else
    {
        index = 1;
        pModule = cs_module_get_by_id(module_id);
        if(pModule == NULL)
            return;

        pModuleName = cs_sys_mod_desc_get(pModule->module_id);
        if(pModuleName == NULL)
        {
            cs_printf("\r\n Invalid module");
            return;
        }

        total_num = 0;
        total_num += pModule->count_64bytes +
                    pModule->count_65_128bytes +
                    pModule->count_129_256bytes +
                    pModule->count_257_512bytes +
                    pModule->count_513_1024bytes +
                    pModule->count_1k_2kbytes +
                    pModule->count_2k_4kbytes +
                    pModule->count_4k_8kbytes +
                    pModule->count_8kbytes;
        if(total_num == 0)
            return;
        cs_printf("\r\n ======================================================");
        cs_printf("\r\n [%s(%d)]", pModuleName,pModule->module_id);
        if(sys_pool_ex_num > 0)
                cs_printf("\r\n  %d. Allocation in SYS-POOL Overview",index++);
        for(j = 0 ; j < sys_pool_ex_num ; j++) {
            pool_id = sys_pool_ex_list[j];
            cs_printf("\r\n   %20s(%10d bytes) : %-20d",sys_mem_pool[pool_id-1].pool_name,
                sys_mem_pool[pool_id-1].element_size , cs_module_pool_counter(pModule, pool_id));
        }

        cs_printf("\r\n  %d. Allocation Length Details (bytes)",index++);
        if(pModule->count_64bytes)
            cs_printf("\r\n  %20s : %d", "[0   ~       64]",pModule->count_64bytes);
        if(pModule->count_65_128bytes)
            cs_printf("\r\n  %20s : %d", "[65  ~      128]",pModule->count_65_128bytes);
        if(pModule->count_129_256bytes)
            cs_printf("\r\n  %20s : %d", "[129 ~      256]",pModule->count_129_256bytes);
        if(pModule->count_257_512bytes)
            cs_printf("\r\n  %20s : %d", "[257 ~      512]",pModule->count_257_512bytes);
        if(pModule->count_513_1024bytes)
            cs_printf("\r\n  %20s : %d", "[513 ~     1024]",pModule->count_513_1024bytes);
        if(pModule->count_1k_2kbytes)
            cs_printf("\r\n  %20s : %d", "[1k  ~       2k]",pModule->count_1k_2kbytes);
        if(pModule->count_2k_4kbytes)
            cs_printf("\r\n  %20s : %d", "[2k  ~       4k]",pModule->count_2k_4kbytes);
        if(pModule->count_4k_8kbytes)
            cs_printf("\r\n  %20s : %d", "[4k  ~       8k]",pModule->count_4k_8kbytes);
        if(pModule->count_8kbytes)
            cs_printf("\r\n  %20s : %d", "[8k  ~ Infinite]",pModule->count_8kbytes);


        #ifdef HAVE_MEM_TRACE
        cs_printf("\r\n  %d. Allocation Details -  function(line) : count",index++);
        cs_module_mem_stat_show(pModule->module_id);
        #endif
    }
}

#endif

