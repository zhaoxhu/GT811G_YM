//==========================================================================
//
//      immenstar_misc.c
//
//      HAL misc board support code for ARM IMST7
//
//===========================================================================*/

#include <pkgconf/hal.h>

#include <cyg/infra/cyg_type.h>         // base types
#include <cyg/infra/cyg_trac.h>         // tracing macros
#include <cyg/infra/cyg_ass.h>          // assertion macros

#include <cyg/hal/hal_io.h>             // IO macros
#include <cyg/hal/hal_arch.h>           // Register state info
#include <cyg/hal/hal_diag.h>
#include <cyg/hal/hal_intr.h>           // necessary?
#include <cyg/hal/hal_immenstar.h>

// -------------------------------------------------------------------------
// MMU initialization:
//
// These structures are laid down in memory to define the translation
// table.
//

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

#define ARM_ACCESS_PERM_NONE_NONE               0
#define ARM_ACCESS_PERM_RO_NONE                 0
#define ARM_ACCESS_PERM_RO_RO                   0
#define ARM_ACCESS_PERM_RW_NONE                 1
#define ARM_ACCESS_PERM_RW_RO                   2
#define ARM_ACCESS_PERM_RW_RW                   3
#if 0
void
hal_mmu_init(void)
{
    unsigned long ttb_base = 0x00004000;
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
    X_ARM_MMU_SECTION(0x100,  0x100,    256,  ARM_CACHEABLE,    ARM_BUFFERABLE,    ARM_ACCESS_PERM_RW_RW);  // SDRAM
    X_ARM_MMU_SECTION(0x200,  0x200,    160,  ARM_CACHEABLE,    ARM_BUFFERABLE,    ARM_ACCESS_PERM_RW_RW);  // SRAM
    X_ARM_MMU_SECTION(0x2B0,  0x2B0,     16,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // LPB MEM
    X_ARM_MMU_SECTION(0x2C0,  0x2C0,      1,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // DBA MEM
    X_ARM_MMU_SECTION(0x2C3,  0x2C3,      1,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // REGULAR REG
    X_ARM_MMU_SECTION(0x2D0,  0x2D0,      1,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // Internal ROM
    X_ARM_MMU_SECTION(0x300,  0x300,     16,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // Flash
    X_ARM_MMU_SECTION(0x400,  0x400,     16,  ARM_UNCACHEABLE,  ARM_UNBUFFERABLE,  ARM_ACCESS_PERM_RW_RW);  // Serial Flash
}
#endif
/*------------------------------------------------------------------------*/
// On-board timer
/*------------------------------------------------------------------------*/

// forward declarations
void hal_if_init(void);

// declarations
static cyg_uint32 _period;

void hal_clock_initialize(cyg_uint32 period)
{
    HAL_WRITE_UINT32(CYG_DEVICE_TIMER_CONTROL, CTL_DISABLE);    // Turn off

    HAL_WRITE_UINT32(CYG_DEVICE_TIMER_LOAD, period);

    HAL_WRITE_UINT32(CYG_DEVICE_TIMER_CONTROL, CTL_ENABLE | CTL_PERIODIC | CTL_SCALE_1);

    HAL_WRITE_UINT32(CYG_DEVICE_TIMER_IEN, CTL_IENABLE);

    _period = period;
}

void hal_clock_reset(cyg_uint32 vector, cyg_uint32 period)
{
    HAL_WRITE_UINT32(CYG_DEVICE_TIMER_CLEAR, 1);
    _period = period;
}

void hal_clock_read(cyg_uint32 *pvalue)
{
    cyg_uint32 value;

    HAL_READ_UINT32(CYG_DEVICE_TIMER_CURRENT, value);

    *pvalue = _period - (value & 0xFFFF);   // Note: counter is only 16 bits and decreases
}

// Delay for some usecs.
void hal_delay_us(cyg_uint32 delay)
{
    cyg_uint32 ticks = 0;

    // Divide by 1000000 in two steps to preserve precision.
    cyg_uint32 wait_ticks = (((CYG_HAL_ARM_IMST_DEV_FREQ/100000)*delay)/CTL_CLK_SEL/10);
    cyg_int32 val, prev, diff;

    // Read Timer1's current value
    HAL_READ_UINT32(CYG_DEVICE_TIMER_CURRENT, prev);
    prev &= 0xFFFF;

    while (ticks < wait_ticks) {
        while (true) {
            // Read Timer1's current value
            HAL_READ_UINT32(CYG_DEVICE_TIMER_CURRENT, val);
            val &= 0xFFFF;

            diff = prev - val;
            if (diff != 0) {
                if(diff < 0)
                    diff += (CYGNUM_HAL_RTC_PERIOD+1);

                break;  // atleast 1 tick has passed
            }
        }
        prev = val;
        ticks += diff;
    }
}

#if !defined(BUILD_MIN_LOADER)
#if defined(CYGPKG_HAL_ARM_IMST_ARM7)
void hal_hardware_init(void)
#elif defined(CYGPKG_HAL_ARM_IMST_ARM9)
void plf_hardware_init(void)
#endif
{
    // Any hardware/platform initialization that needs to be done.

    // Clear all interrupt sources
    HAL_WRITE_UINT32(CYG_DEVICE_IRQ_EnableSet, 0x0);

#ifndef CYGFUN_HAL_COMMON_KERNEL_SUPPORT
    HAL_CLOCK_INITIALIZE(CYGNUM_HAL_RTC_PERIOD);
#endif

    // for ARM9, hal_if_init is called by arm9 hal_hardware_init,
    // should not be called twice.
#if defined(CYGSEM_HAL_VIRTUAL_VECTOR_SUPPORT) && defined(CYGPKG_HAL_ARM_IMST) && defined (IROSBOOT) && !defined(CYGPKG_HAL_ARM_IMST_ARM9)
    hal_if_init();
#endif

}

//
// This routine is called to respond to a hardware interrupt (IRQ).  It
// should interrogate the hardware and return the IRQ vector number.

int hal_IRQ_handler(void)
{
    int irq_status, irq_mask, vector;

    HAL_READ_UINT32(CYG_DEVICE_IRQ_Status, irq_status);
    HAL_READ_UINT32(CYG_DEVICE_IRQ_Enable, irq_mask);
    irq_status &= irq_mask; // mask top-level disable IRQ status

    for (vector = CYGNUM_HAL_ISR_MIN;
        vector <= CYGNUM_HAL_ISR_MAX;  vector++) {
        if (irq_status & (1<<vector)) {
            return vector;
        }
    }

    return CYGNUM_HAL_INTERRUPT_NONE;
}

//
// Interrupt control
//

void hal_interrupt_mask(int vector)
{

    int irq_mask;

    HAL_READ_UINT32(CYG_DEVICE_IRQ_EnableSet, irq_mask);

    irq_mask &=  ~(1<<vector); // clean irq enable
    HAL_WRITE_UINT32(CYG_DEVICE_IRQ_EnableSet, irq_mask);

    switch(vector)
    {
        case CYGNUM_HAL_INTERRUPT_TIMER1:
            HAL_WRITE_UINT32(IROS_PER_TMR_INT1, 1);
            break;
        case CYGNUM_HAL_INTERRUPT_TIMER2:
            HAL_WRITE_UINT32(IROS_PER_TMR_INT2, 1);
            break;
        case CYGNUM_HAL_INTERRUPT_TIMER3:
            HAL_WRITE_UINT32(IROS_PER_TMR_INT3, 1);
            break;
        case CYGNUM_HAL_INTERRUPT_TIMER4:
            HAL_WRITE_UINT32(IROS_PER_TMR_INT4, 1);
            break;
        case CYGNUM_HAL_INTERRUPT_TIMER5:
            HAL_WRITE_UINT32(IROS_PER_TMR_INT5, 1);
            break;
        default:
            break;
    }
}



void hal_interrupt_unmask(int vector)
{
    int irq_status;

    HAL_READ_UINT32(CYG_DEVICE_IRQ_EnableSet, irq_status);
    irq_status |=  (1<<vector);
    HAL_WRITE_UINT32(CYG_DEVICE_IRQ_EnableSet, irq_status);
}

void hal_interrupt_acknowledge(int vector)
{
//    diag_init();  diag_printf("ack intr(%d)\n", vector);
}

void hal_interrupt_configure(int vector, int level, int up)
{
//    diag_init();  diag_printf("cfg intr(%d,%d,%d)\n", vector, level, up);
}

void hal_interrupt_set_level(int vector, int level)
{
//    diag_init();  diag_printf("level intr(%d,%d)\n", vector, level);
}

void hal_show_IRQ(int vector, int data, int handler)
{
//    diag_init(); diag_printf("IRQ - vector: %x, data: %x, handler: %x\n", vector, data, handler);
}

#ifndef CYGSEM_HAL_VIRTUAL_VECTOR_SUPPORT
cyg_uint32
hal_default_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data)
{
        return 0;
}
#endif



/*---------------------------------------------------------------------------*/

__externC void cyg_plf_pci_init(void)
{
}
#endif /* BUILD_MIN_LOADER */

/*---------------------------------------------------------------------------*/
/* End of hal_misc.c */
