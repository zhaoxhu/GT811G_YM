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
 * exception.c
 *
 * $Id: exception.c,v 1.1.2.2.12.1.2.1.2.2.2.1.24.1.8.1.2.2.2.1 2011/10/09 07:19:56 zsun Exp $
 */

#include "iros_config.h"
#include "plat_common.h"
#include "cpu_load.h"


#define MAX_OUTPUT_BUF_LEN  512 /*256 is the least unit*/ 
#define CPSR_THUMB_MODE     0x00000020
#define MAX_EXCEPTION_COUNT 1
#define MAX_CORE_SIZE       0x8000 /* 32KB */

extern char const iros_version[];

cs_uint8 exception_count = 0;
char CORE_MAGIC[4] = {'C', 'O', 'R', 'E'};
cs_uint8* core_addr = NULL;

void append_str_to_flash(cs_uint8* _buf, cs_uint32 _len)
{
    static cs_uint32 flash_offset = 0;
    static cs_uint8 core_str[MAX_OUTPUT_BUF_LEN]={0};
    cs_uint32 str_offset=flash_offset%MAX_OUTPUT_BUF_LEN;

    if((flash_offset + _len >= MAX_CORE_SIZE)||(_len>MAX_OUTPUT_BUF_LEN))
    {
        cs_printf("core dump exceed max size\n");
        return;
    }
    if(!_buf)
    {
        flash_write_core_force(core_addr + (flash_offset&(~(MAX_OUTPUT_BUF_LEN-1))), 
                                                        core_str, MAX_OUTPUT_BUF_LEN);
        flash_offset=0;
        memset(core_str,0,MAX_OUTPUT_BUF_LEN);   
        return;
    }
    if(str_offset + _len < MAX_OUTPUT_BUF_LEN)
    {
        memcpy(core_str+str_offset,_buf,_len);
    }else
    {
        int write_len=MAX_OUTPUT_BUF_LEN-str_offset;
        memcpy(core_str+str_offset,_buf,write_len);
        flash_write_core_force(core_addr + (flash_offset&(~(MAX_OUTPUT_BUF_LEN-1))), 
                                                        core_str, MAX_OUTPUT_BUF_LEN);
        memset(core_str,0,MAX_OUTPUT_BUF_LEN);
        if(_len-write_len < MAX_OUTPUT_BUF_LEN)
            memcpy(core_str,_buf+write_len,_len-write_len);
    }
    cs_printf("%s",_buf);
    flash_offset += _len;
    return;
}

void show_core_in_flash(void)
{
    char output_line[MAX_OUTPUT_BUF_LEN];
    char* buf_cursor = NULL;
    int line_len = 0;

    cs_uint32 part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_COREDUMP_ID);
    if(IROS_FLASH_PARTITION_INDEX_ANY == part_index)
    {
        IROS_LOG_CRI(IROS_MID_EXCEPT, "no partition for core dump\n");
        return;
    }

    char* flash_addr = flash_dev.info.super_block.part_tab[part_index].part_loc;
    cs_uint32 max_len= flash_dev.info.super_block.part_tab[part_index].part_size;

    buf_cursor = (char *)flash_addr;
    if(0 != memcmp(buf_cursor, CORE_MAGIC, sizeof(CORE_MAGIC)))
    {
        cs_printf("no core dump data\n");
        return;
    }

    buf_cursor = (char *)flash_addr + sizeof(CORE_MAGIC);
    while( buf_cursor != NULL
        && *buf_cursor != 0
        && (buf_cursor < (char *)(flash_addr + max_len)))
    {
        line_len = strlen(buf_cursor);
        if(line_len >= MAX_OUTPUT_BUF_LEN)
        {
            IROS_LOG_CRI(IROS_MID_EXCEPT, "core data corrupt\n");
            break;
        }
        strncpy(output_line, buf_cursor, MAX_OUTPUT_BUF_LEN-1);
        output_line[MAX_OUTPUT_BUF_LEN-1] = 0;
        cs_printf("%s", output_line);
        cs_thread_delay(10);

        // skip EOL
        buf_cursor += line_len + 1;
    }

    return;
}

void clear_core_in_flash(void)
{
    cs_uint32 erase_len = 0;
    cs_uint32 part_len, blk_len;

    cs_uint32 part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_COREDUMP_ID);
    if(IROS_FLASH_PARTITION_INDEX_ANY == part_index)
    {
        IROS_LOG_CRI(IROS_MID_EXCEPT, "no partition for core dump\n");
        return;
    }

    char* flash_addr = flash_dev.info.super_block.part_tab[part_index].part_loc;
    part_len = flash_dev.info.super_block.part_tab[part_index].part_size;
    blk_len  = flash_dev.info.super_block.block_size;

    while(erase_len < part_len)
    {
        if (flash_part_write_init(part_index, flash_addr))
        {
            flash_erase_block_main((unsigned int)flash_addr);
            IROS_LOG_MAJ(IROS_MID_EXCEPT, "Block erased %p\n", flash_addr);
            flash_part_write_done(part_index);
        }
        erase_len  += blk_len;
        flash_addr += blk_len;

        cs_thread_delay(10);
    }

    cs_printf("core dump data cleared\n");
    return;
}


// help debugging when some thread misbehaving such as busy loop
void show_threads_info(cs_io_hdl_t o_stream)
{
    cyg_thread_info info;
    cyg_handle_t thread = 0;
    cs_uint16 id = 0;
    cs_uint32 stack_ptr=0;
    int i;
    i = 1;
    int len = 0;

    char output_buf[MAX_OUTPUT_BUF_LEN] = {0};

    while(cyg_thread_get_next(&thread, &id))
    {
        if (!cyg_thread_get_info(thread, id, &info))
            break;

        stack_ptr = info.stack_base + info.stack_size - info.stack_used;

        diag_snprintf(output_buf, MAX_OUTPUT_BUF_LEN-1,
            "Thread %d\n"
            "  thread id        : %d\n"
            "  thread state     : %d\n"
            "  thread name      : %s\n"
            "  thread set_pri   : %d\n"
            "  thread cur_pri   : %d\n"
            "  thread stack_base: 0x%08x\n"
            "  thread stack_size: 0x%08x\n"
            "  thread stack_used: 0x%08x\n"
            "  thread stack_ptr : 0x%08x\n",
            i++,
            info.id,
            info.state,
            info.name,
            info.set_pri,
            info.cur_pri,
            info.stack_base,
            info.stack_size,
            info.stack_used,
            stack_ptr
        );
        output_buf[MAX_OUTPUT_BUF_LEN-1] = 0;

        if(o_stream != NULL)
        {
            (*o_stream)(output_buf, strlen(output_buf)+1);
        }
        else
        {
            cs_printf("%s", output_buf);
            cs_thread_delay(10);

            len = info.stack_used/sizeof(int);
            data_dump_w((char*)stack_ptr, len, (char*)stack_ptr);
            cs_thread_delay(10);
        }
        memset(output_buf, 0, MAX_OUTPUT_BUF_LEN);
    }
}

static void show_regs_info(HAL_SavedRegisters* regs, cs_io_hdl_t o_stream)
{
/*
    // These are common to all saved states
    cyg_uint32  d[HAL_NUM_THREAD_CONTEXT_REGS] ;  // Data regs (r0..r10)
    cyg_uint32  fp;                               // (r11) Frame pointer
    cyg_uint32  ip;                               // (r12)
    cyg_uint32  sp;                               // (r13) Stack pointer
    cyg_uint32  lr;                               // (r14) Link Reg
    cyg_uint32  pc;                               // (r15) PC place holder
                                                  //       (never used)
    cyg_uint32  cpsr;                             // Condition Reg
    // These are only saved for exceptions and interrupts
    cyg_uint32  vector;                           // Vector number
    cyg_uint32  svc_lr;                           // saved svc mode lr
    cyg_uint32  svc_sp;                           // saved svc mode sp
*/

    int i;
    char output_buf[MAX_OUTPUT_BUF_LEN] = {0};

    for (i=0; i<HAL_NUM_THREAD_CONTEXT_REGS; i++)
    {
        output_buf[0] = 0;
        diag_snprintf(output_buf, MAX_OUTPUT_BUF_LEN-1,
            " r%02d: 0x%08x\n", i, regs->d[i]);
        output_buf[MAX_OUTPUT_BUF_LEN-1] = 0;

        if(o_stream != NULL)
        {
            (*o_stream)(output_buf, strlen(output_buf)+1);
        }
        else
        {
            cs_printf("%s", output_buf);
        }
    }

    output_buf[0] = 0;
    diag_snprintf(output_buf, MAX_OUTPUT_BUF_LEN-1,
        "    fp: 0x%08x\n"
        "    ip: 0x%08x\n"
        "    sp: 0x%08x\n"
        "    lr: 0x%08x\n"
        "    pc: 0x%08x\n"
        "  cpsr: 0x%08x\n"
        "vector: 0x%08x\n"
        "svc_lr: 0x%08x\n"
        "svc_sp: 0x%08x\n",
        regs->fp,
        regs->ip,
        regs->sp,
        regs->lr,
        regs->pc,
        regs->cpsr,
        regs->vector,
        regs->svc_lr,
        regs->svc_sp
    );
    output_buf[MAX_OUTPUT_BUF_LEN-1] = 0;

    if(o_stream != NULL)
    {
        (*o_stream)(output_buf, strlen(output_buf)+1);
    }
    else
    {
        cs_printf("%s", output_buf);
    }
}


static void exception_handler_main(cyg_addrword_t data, cyg_code_t number, cyg_addrword_t info)
{
    cs_uint64 time_val;
    char output_buf[MAX_OUTPUT_BUF_LEN] = {0};

    // avoid too much exception info
    if(exception_count >= MAX_EXCEPTION_COUNT)
    {
        return;
    }
    exception_count++;

    HAL_SavedRegisters *regs = (HAL_SavedRegisters*)info;
    time_val = cs_current_time();


    cs_uint32 part_index = flash_part_id_to_part_index(IROS_FLASH_PARTITION_COREDUMP_ID);
    if(IROS_FLASH_PARTITION_INDEX_ANY == part_index)
    {
        cs_printf("no partition for core dump\n");
        return;
    }

    core_addr = flash_dev.info.super_block.part_tab[part_index].part_loc;

    // write magic no. as header
    // Dumping time/version/reg/thread/cpuload to flash

    diag_snprintf(output_buf, MAX_OUTPUT_BUF_LEN-1,
        "%s"
        "Exception detected, time: %lld, number: %d, pc: 0x%08x\n",
        iros_version, time_val, number, regs->pc);
    append_str_to_flash(CORE_MAGIC, sizeof(CORE_MAGIC));
    append_str_to_flash(output_buf, strlen(output_buf));

    show_regs_info(regs, (cs_io_hdl_t)append_str_to_flash);
    show_threads_info((cs_io_hdl_t)append_str_to_flash);
    dump_cpuload_info((cs_io_hdl_t)append_str_to_flash);
    append_str_to_flash(NULL, 0);/*Exception End*/


    // correct pc, try to recover
    if (((regs->cpsr) & CPSR_THUMB_MODE) != 0)
    {
        regs->pc = regs->pc - 2;
    }
    else
    {
        regs->pc = regs->pc - 4;
    }
}

void install_exception_handlers(void)
{
    cyg_exception_handler_t *old_handler;
    cyg_addrword_t old_data;

    cyg_exception_set_handler(
        CYGNUM_HAL_VECTOR_ABORT_DATA,
        &exception_handler_main,
        0,
        &old_handler,
        &old_data);

    cyg_exception_set_handler(
        CYGNUM_HAL_VECTOR_ABORT_PREFETCH,
        &exception_handler_main,
        0,
        &old_handler,
        &old_data);

    cyg_exception_set_handler(
        CYGNUM_HAL_VECTOR_UNDEF_INSTRUCTION,
        &exception_handler_main,
        0,
        &old_handler,
        &old_data);
}

