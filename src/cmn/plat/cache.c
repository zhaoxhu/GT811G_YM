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
 * cache.c
 *
 * $Id: cache.c,v 1.1.4.2.2.4.16.2.2.4 2011/08/02 03:10:11 ljin Exp $
 */

/****************************************************************

OVERVIEW

This file contains cache/mmu access interface

****************************************************************/
#include <stdlib.h>
#include <string.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_mmu.h>
#include <cyg/hal/hal_cache.h>
#include <cyg/kernel/kapi.h>
#include "cs_types.h"

// -------------------------------------------------------------------------
// MMU initialization:
//
// These structures are laid down in memory to define the translation
// table.
//
#define HAL_DCACHE_PURGE_ALL() HAL_DCACHE_SYNC()

// ARM Translation Table Base Bit Masks
#define ARM_TRANSLATION_TABLE_MASK               0xFFFFC000

// ARM Domain Access Control Bit Masks
#define ARM_ACCESS_TYPE_NO_ACCESS(domain_num)    (0x0 << (domain_num)*2)
#define ARM_ACCESS_TYPE_CLIENT(domain_num)       (0x1 << (domain_num)*2)
#define ARM_ACCESS_TYPE_MANAGER(domain_num)      (0x3 << (domain_num)*2)

struct ARM_MMU_FIRST_LEVEL_FAULT {
    int id : 2;
    int sbz : 30;
};
#define ARM_MMU_FIRST_LEVEL_FAULT_ID 0x0

struct ARM_MMU_FIRST_LEVEL_PAGE_TABLE {
    int id : 2;
    int imp : 2;
    int domain : 4;
    int sbz : 1;
    int base_address : 23;
};
#define ARM_MMU_FIRST_LEVEL_PAGE_TABLE_ID 0x1

struct ARM_MMU_FIRST_LEVEL_SECTION {
    int id : 2;
    int b : 1;
    int c : 1;
    int imp : 1;
    int domain : 4;
    int sbz0 : 1;
    int ap : 2;
    int sbz1 : 8;
    int base_address : 12;
};
#define ARM_MMU_FIRST_LEVEL_SECTION_ID 0x2

struct ARM_MMU_FIRST_LEVEL_RESERVED {
    int id : 2;
    int sbz : 30;
};
#define ARM_MMU_FIRST_LEVEL_RESERVED_ID 0x3

#define ARM_MMU_FIRST_LEVEL_DESCRIPTOR_ADDRESS(ttb_base, table_index) \
   (unsigned long *)((unsigned long)(ttb_base) + ((table_index) << 2))

#define ARM_FIRST_LEVEL_PAGE_TABLE_SIZE 0x4000

#define ARM_MMU_SECTION(ttb_base, actual_base, virtual_base,              \
                        cacheable, bufferable, perm)                      \
    CYG_MACRO_START                                                       \
        register union ARM_MMU_FIRST_LEVEL_DESCRIPTOR desc;               \
                                                                          \
        desc.word = 0;                                                    \
        desc.section.id = ARM_MMU_FIRST_LEVEL_SECTION_ID;                 \
        desc.section.imp = 1;                                             \
        desc.section.domain = 0;                                          \
        desc.section.c = (cacheable);                                     \
        desc.section.b = (bufferable);                                    \
        desc.section.ap = (perm);                                         \
        desc.section.base_address = (actual_base);                        \
        *ARM_MMU_FIRST_LEVEL_DESCRIPTOR_ADDRESS(ttb_base, (virtual_base)) \
                            = desc.word;                                  \
    CYG_MACRO_END

#define X_ARM_MMU_SECTION(abase,vbase,size,cache,buff,access)      \
    { int i; int j = abase; int k = vbase;                         \
      for (i = size; i > 0 ; i--,j++,k++)                          \
      {                                                            \
        ARM_MMU_SECTION(ttb_base, j, k, cache, buff, access);      \
      }                                                            \
    }

union ARM_MMU_FIRST_LEVEL_DESCRIPTOR {
    unsigned long word;
    struct ARM_MMU_FIRST_LEVEL_FAULT fault;
    struct ARM_MMU_FIRST_LEVEL_PAGE_TABLE page_table;
    struct ARM_MMU_FIRST_LEVEL_SECTION section;
    struct ARM_MMU_FIRST_LEVEL_RESERVED reserved;
};

#define ARM_UNCACHEABLE                         0
#define ARM_CACHEABLE                           1
#define ARM_UNBUFFERABLE                        0
#define ARM_BUFFERABLE                          1

// access permition: privlieged perm, user perm
#define ARM_ACCESS_PERM_NONE_NONE               0
#define ARM_ACCESS_PERM_RO_NONE                 0
#define ARM_ACCESS_PERM_RO_RO                   0
#define ARM_ACCESS_PERM_RW_NONE                 1
#define ARM_ACCESS_PERM_RW_RO                   2
#define ARM_ACCESS_PERM_RW_RW                   3


void hal_mmu_init(void)
{
    unsigned long ttb_base = 0x8000;
    unsigned long i;

    // Set the TTB register
    asm volatile ("mcr  p15,0,%0,c2,c0,0" : : "r"(ttb_base) /*:*/);

    // Set the Domain Access Control Register
    i = ARM_ACCESS_TYPE_MANAGER(0)    |
        ARM_ACCESS_TYPE_NO_ACCESS(1)  |
        ARM_ACCESS_TYPE_NO_ACCESS(2)  |
        ARM_ACCESS_TYPE_NO_ACCESS(3)  |
        ARM_ACCESS_TYPE_NO_ACCESS(4)  |
        ARM_ACCESS_TYPE_NO_ACCESS(5)  |
        ARM_ACCESS_TYPE_NO_ACCESS(6)  |
        ARM_ACCESS_TYPE_NO_ACCESS(7)  |
        ARM_ACCESS_TYPE_NO_ACCESS(8)  |
        ARM_ACCESS_TYPE_NO_ACCESS(9)  |
        ARM_ACCESS_TYPE_NO_ACCESS(10) |
        ARM_ACCESS_TYPE_NO_ACCESS(11) |
        ARM_ACCESS_TYPE_NO_ACCESS(12) |
        ARM_ACCESS_TYPE_NO_ACCESS(13) |
        ARM_ACCESS_TYPE_NO_ACCESS(14) |
        ARM_ACCESS_TYPE_NO_ACCESS(15);
    asm volatile ("mcr  p15,0,%0,c3,c0,0" : : "r"(i) /*:*/);

    // First clear all TT entries - ie Set them to Faulting
    memset((void *)ttb_base, 0, ARM_FIRST_LEVEL_PAGE_TABLE_SIZE);

    // Memory layout after MMU is turned on
    //

    //               Actual  Virtual  Size   Attributes                                                  Function
    //               Base     Base     MB     cached?          buffered?         access permissions
    //             xxx00000  xxx00000
    X_ARM_MMU_SECTION(0x000,  0x000,    256,  ARM_CACHEABLE,    ARM_BUFFERABLE,    ARM_ACCESS_PERM_RW_RW);  // SYSTEM
    X_ARM_MMU_SECTION(0x100,  0x100,      8,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // SDRAM raw
    X_ARM_MMU_SECTION(0x200,  0x200,     16,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // SRAM raw
    X_ARM_MMU_SECTION(0x2B0,  0x2B0,     16,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // LPB MEM
    X_ARM_MMU_SECTION(0x2C0,  0x2C0,      1,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // Internal SRAM
    X_ARM_MMU_SECTION(0x2C3,  0x2C3,      1,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // REGULAR REG
    X_ARM_MMU_SECTION(0x2C5,  0x2C5,      1,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // PCM buffer
    X_ARM_MMU_SECTION(0x2C6,  0x2C6,      1,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // MA DMA/ESDMA buffer
    X_ARM_MMU_SECTION(0x2D0,  0x2D0,      1,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RO_RO);  // Internal ROM
    X_ARM_MMU_SECTION(0x300,  0x300,      2,  ARM_CACHEABLE,    ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RO_RO);  // Flash 1: RO w/ cache
    X_ARM_MMU_SECTION(0x302,  0x302,      6,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // Flash 2: RW w/o cache
    X_ARM_MMU_SECTION(0x400,  0x400,     16,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // Serial Flash


    // Enable MMU
    // asm volatile ("mcr  p15,0,%0,c2,c0,0" : : "r"(ttb_base) /*:*/);
    // asm volatile ("ldr %0,0x71" : "r"(ttb_base) /*:*/);
    // asm volatile ("mcr p15,0,%0,c1,c0" : : "r"(ttb_base) /*:*/);
    asm volatile ("mov %0,#0x71" : : "r"(ttb_base) );
    asm volatile ("mcr p15,0,%0,c1,c0" : : "r"(ttb_base) );
    asm(
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
    );

#if 0
    ldr r1,=MMU_Control_Init|MMU_Control_M
    mcr MMU_CP,0,r1,MMU_Control,c0
#endif
}

void enable_I_D_cache(void){
    HAL_DCACHE_SYNC();
    HAL_ICACHE_INVALIDATE_ALL();
    HAL_DCACHE_INVALIDATE_ALL();
    HAL_ICACHE_ENABLE();
    HAL_DCACHE_ENABLE();
}

void disable_I_D_cache(void)
{
    HAL_ICACHE_DISABLE();
    HAL_DCACHE_DISABLE();
}

void enable_I_cache(void){
    HAL_ICACHE_INVALIDATE_ALL();
    HAL_ICACHE_ENABLE();
}

void disable_I_cache(void)
{
    HAL_ICACHE_INVALIDATE_ALL();
    HAL_ICACHE_DISABLE();
}

// Write dirty cache lines to memory and invalidate the cache entries
// for the given address range.
void cache_flush_data(cs_uint32 base, cs_uint32 size)
{
    HAL_DCACHE_FLUSH(base, size);
}

// Invalidate cache lines in the given range without writing to memory.
void cache_invalidate_data(cs_uint32 base, cs_uint32 size)
{
    HAL_DCACHE_INVALIDATE(base, size);
}

void cache_init(void)
{
    register CYG_INTERRUPT_STATE oldints;
    hal_mmu_init();

    HAL_DISABLE_INTERRUPTS(oldints);
    HAL_DCACHE_PURGE_ALL();
    HAL_ICACHE_INVALIDATE_ALL();
    HAL_DCACHE_INVALIDATE_ALL();
    HAL_ICACHE_ENABLE();
    HAL_DCACHE_ENABLE();
    HAL_RESTORE_INTERRUPTS(oldints);
}


