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

DEFINITIONS:  "DEVICE" means the Cortina Systems?Daytona SDK product.
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
 * cpuload.c
 *
 * $Id: cpu_load.c,v 1.1.4.2.6.3.2.1.38.1.2.4 2011/03/23 11:01:26 zsun Exp $
 */
#include "iros_config.h"
#include "plat_common.h"
#include "interrupt.h"
#include "cpu_load.h"

static  cyg_uint32 calibration;
static  cyg_cpuload_t cpuload;
static  cyg_handle_t cpuload_handle;
static  cyg_uint32 average_point1s;
static  cyg_uint32 average_1s;
static  cyg_uint32 average_10s;

#define CPU_LOAD_INFO_LEN   128

#ifdef HAVE_CPULOAD_PER_THREAD
#define CS_OSAL_MAX_CPU_LOAD 32
typedef struct
{
    cs_node node;
    cs_uint32 sampling[CS_OSAL_MAX_THREAD];
}cpu_load_t;

typedef struct
{
    cs_uint32 sampling[CS_OSAL_MAX_THREAD];
    cs_list sampling_list;
}thread_cpu_load_t;

static thread_cpu_load_t thread_load;
#endif


void do_cpuload_test(void)
{
    cyg_uint32 i,j;
    cyg_cpuload_calibrate(&calibration);
    cyg_cpuload_create(&cpuload,calibration,&cpuload_handle);

    /* Busy loop doing useless work. This will cause the cpu load to be
       100%. After all these iterations, check the measured load really
       is 100%. */

    for (i=0; i < 2; i++)
    {
        for (j=0; j < calibration*10; j++)
        ;
        cyg_cpuload_get(cpuload_handle,&average_point1s,&average_1s,&average_10s);
    }

#ifdef HAVE_CPULOAD_PER_THREAD
    cpu_load_t *pNode = NULL;
    memset(&thread_load , 0 , sizeof(thread_cpu_load_t));
    for(i = 0; i < CS_OSAL_MAX_CPU_LOAD; i++)
    {
        pNode = (cpu_load_t *)iros_malloc(IROS_MID_CPULOAD, sizeof(cpu_load_t));
        if(pNode == NULL)
            continue;
        memset(pNode , 0 , sizeof(cpu_load_t));
        cs_lst_add(&thread_load.sampling_list, (cs_node *)pNode);
    }
#endif

}


void dump_cpuload_info(cs_io_hdl_t o_stream)
{
    char output_buf[CPU_LOAD_INFO_LEN] = {0};

    cyg_cpuload_get(cpuload_handle,&average_point1s,&average_1s,&average_10s);

    diag_snprintf(output_buf, CPU_LOAD_INFO_LEN-1,
        "cpu load (last 0.10 sec): %2d%% busy\n"
        "cpu load (last 1.00 sec): %2d%% busy\n"
        "cpu load (last 10.0 sec): %2d%% busy\n"
        ,average_point1s
        ,average_1s
        ,average_10s
        );

    output_buf[CPU_LOAD_INFO_LEN-1] = 0;
    if(o_stream != NULL)
    {
        (*o_stream)(output_buf, strlen(output_buf)+1);
    }
    else
    {
        cs_printf("%s", output_buf);
    }
}

// to retrieve cpuload from other module
void cpuload_get(cs_uint32 *aver_cpuload_point1s,
                 cs_uint32 *aver_cpuload_1s,
                 cs_uint32 *aver_cpuload_10s)
{
    if (!(aver_cpuload_point1s && aver_cpuload_1s && aver_cpuload_10s))
        return;

    cyg_cpuload_get(cpuload_handle,&average_point1s,&average_1s,&average_10s);
    *aver_cpuload_point1s = average_point1s;
    *aver_cpuload_1s = average_1s;
    *aver_cpuload_10s = average_10s;
}

void cs_cpuload_warning(void *data)
{
    cs_uint32 v1=0,v2=0,v3=0;

    cpuload_get(&v1,&v2,&v3);
    if(v3 > 95)
        IROS_LOG_CRI(IROS_MID_OSAL,"Warning: CPU load peek 95%\n");

    return;
}

#ifdef HAVE_CPULOAD_PER_THREAD

cs_uint16 cs_thread_id_get(cyg_handle_t thread_handle)
{
    cyg_handle_t thread=0;
    cyg_uint16 id=0;
    cyg_thread_info info;

    memset(&info, 0, sizeof(cyg_thread_info));
    while( cyg_thread_get_next(&thread, &id))
    {
        if(thread_handle == thread)
            return id;
    }

    return 0;
}

void cs_thread_sampling_plus(cs_uint16 thread_id)
{
    if(thread_id >= CS_OSAL_MAX_THREAD)
        return;

    thread_load.sampling[thread_id]++;
}

cs_uint64 sys_current_time = 0;
void cs_cpuload_sampling(void *data)
{
    cs_uint64 systime = cyg_current_time();
    cyg_handle_t thread_handle=0;
    cs_uint16 thread_id=0;
    cpu_load_t *pNode = NULL;

    thread_handle = cyg_thread_self();
    thread_id = cs_thread_id_get(thread_handle);
    if(thread_id > 0 && thread_id < CS_OSAL_MAX_THREAD)
    {
        if(sys_current_time == 0)
            sys_current_time = systime;

        if(systime - sys_current_time >= 100)
        {
            cs_thread_sampling_plus(thread_id);
            #if 0
            if(cs_lst_count(&thread_load.sampling_list) >= CS_OSAL_MAX_CPU_LOAD)
                pNode = (cpu_load_t *)cs_lst_get(&thread_load.sampling_list);
            else
                pNode = (cpu_load_t *)iros_malloc(IROS_MID_OSAL , sizeof(cpu_load_t));
            #else
            pNode = (cpu_load_t *)cs_lst_get(&thread_load.sampling_list);
            if(pNode == NULL)
                return;
            #endif

            memcpy(&pNode->sampling[0] , &thread_load.sampling[0] , 4*CS_OSAL_MAX_THREAD);
            cs_lst_add(&thread_load.sampling_list, (cs_node *)pNode);
            memset(&thread_load.sampling[0] , 0 , 4*CS_OSAL_MAX_THREAD);
            sys_current_time = systime;
        }
        else
        {
            cs_thread_sampling_plus(thread_id);
        }
    }
}

cs_uint32 cpu_load_percent(cs_uint16 thread_id , cs_uint32 seconds)
{
    cs_uint32 total_hit=0;
    cs_uint32 thread_hit=0;
    cs_uint32 i;
    cpu_load_t *pNode = NULL;

    pNode = (cpu_load_t *)cs_lst_last(&thread_load.sampling_list);
    while(pNode != NULL && seconds-- > 0)
    {
        for(i = 0 ; i < CS_OSAL_MAX_THREAD ; i++)
            total_hit += pNode->sampling[i];

        thread_hit += pNode->sampling[thread_id];
        pNode = (cpu_load_t *)cs_lst_prev((cs_node *)pNode);
    }
    return (cs_uint32)((thread_hit*10000)/total_hit);
}

typedef struct
{
    cs_node node;
    cyg_thread_info info;
}thread_display_t;

void cs_thread_cpu_load_show()
{
    cyg_handle_t thread=0;
    cyg_uint16 id=0;
    thread_display_t info[32];
    cs_uint32 thread_num = 0;
    cs_list display_list;
    thread_display_t *pNode = NULL;
    int addflag = 0;
    cs_uint32 one_load = 0,five_load = 0,ten_load=0,thirty_load=0;
    cs_uint8 one_str[8],five_str[8],ten_str[8],thirty_str[8];

    onu_timer_int_mask(hw_cpuload_timer_id);

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

    //diag_printf("\r\n=========================================================================");
    diag_printf("\r\n%-3s %-25s %-7s %-7s %-6s %-6s %-6s %-6s","ID","NAME","CUR_PRI","SET_PRI","1s",
                                                                            "5s","10s","30s");
    //diag_printf("\r\n-------------------------------------------------------------------------");
    cs_lst_scan(&display_list , pNode , thread_display_t *)
    {
        one_load = cpu_load_percent(pNode->info.id,1);
        five_load = cpu_load_percent(pNode->info.id,5);
        ten_load = cpu_load_percent(pNode->info.id,10);
        thirty_load = cpu_load_percent(pNode->info.id,30);
        memset(one_str , 0 , sizeof(one_str));
        memset(five_str , 0 , sizeof(five_str));
        memset(ten_str , 0 , sizeof(ten_str));
        memset(thirty_str , 0 , sizeof(thirty_str));
        if(one_load%100 < 10)
            diag_sprintf(one_str , "%d.0%d" , one_load/100 , one_load%100);
        else
            diag_sprintf(one_str , "%d.%d" , one_load/100 , one_load%100);

        if(five_load%100 < 10)
            diag_sprintf(five_str , "%d.0%d" , five_load/100 , five_load%100);
        else
            diag_sprintf(five_str , "%d.%d" , five_load/100 , five_load%100);

        if(ten_load%100 < 10)
            diag_sprintf(ten_str , "%d.0%d" , ten_load/100 , ten_load%100);
        else
            diag_sprintf(ten_str , "%d.%d" , ten_load/100 , ten_load%100);

        if(thirty_load%100 < 10)
            diag_sprintf(thirty_str , "%d.0%d" , thirty_load/100 , thirty_load%100);
        else
            diag_sprintf(thirty_str , "%d.%d" , thirty_load/100 , thirty_load%100);
        diag_printf("\r\n%-3d %-25s %-7d %-7d %-6s %-6s %-6s %-6s",pNode->info.id,
            pNode->info.name,pNode->info.cur_pri,pNode->info.set_pri,
            one_str , five_str , ten_str , thirty_str);

        cyg_thread_delay(1);
    }
    //diag_printf("\r\n=========================================================================\r\n");

    onu_timer_int_unmask(hw_cpuload_timer_id);

    return;
}

#endif

