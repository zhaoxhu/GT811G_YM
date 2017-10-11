#ifndef CYGONCE_HAL_PLATFORM_INTS_H
#define CYGONCE_HAL_PLATFORM_INTS_H
//==========================================================================
//
//      hal_platform_ints.h
//
//      HAL Interrupt and clock support
//
// Date:         January 10, 2005
// Purpose:      Define Interrupt support
// Description:  The interrupt details for the IMST are defined here.
// Usage:
//               #include <cyg/hal/hal_platform_ints.h>
//               ...
//==========================================================================

#include <pkgconf/system.h>
#include <cyg/hal/hal_immenstar.h>

#include CYGHWR_MEMORY_LAYOUT_H


#define CYGNUM_HAL_INTERRUPT_SOFT                     0
#define CYGNUM_HAL_INTERRUPT_TIMER1                   1
#define CYGNUM_HAL_INTERRUPT_TIMER2                   2
#define CYGNUM_HAL_INTERRUPT_TIMER3                   3
#define CYGNUM_HAL_INTERRUPT_TIMER4                   4
#define CYGNUM_HAL_INTERRUPT_TIMER5                   5
#define CYGNUM_HAL_INTERRUPT_UART0                    6
#define CYGNUM_HAL_INTERRUPT_UART1                    7
#define CYGNUM_HAL_INTERRUPT_SSP                      8
#define CYGNUM_HAL_INTERRUPT_GPIO                     9
#define CYGNUM_HAL_INTERRUPT_I2C                     10
#define CYGNUM_HAL_INTERRUPT_WDT                     11
#define CYGNUM_HAL_INTERRUPT_MDIO                    12
#define CYGNUM_HAL_INTERRUPT_AHB                     13
#define CYGNUM_HAL_INTERRUPT_PCM                     14
#define CYGNUM_HAL_INTERRUPT_DMA1                    15
#define CYGNUM_HAL_INTERRUPT_DMA0                    16
#define CYGNUM_HAL_INTERRUPT_RESERVED17              17
#define CYGNUM_HAL_INTERRUPT_RESERVED18              18
#define CYGNUM_HAL_INTERRUPT_RESERVED19              19
#define CYGNUM_HAL_INTERRUPT_GLB                     20
#define CYGNUM_HAL_INTERRUPT_MA                      21
#define CYGNUM_HAL_INTERRUPT_RESERVED22              22
#define CYGNUM_HAL_INTERRUPT_RESERVED23              23
#define CYGNUM_HAL_INTERRUPT_BM                      24
#define CYGNUM_HAL_INTERRUPT_FE                      25
#define CYGNUM_HAL_INTERRUPT_RESERVED26              26
#define CYGNUM_HAL_INTERRUPT_NI                      27
#define CYGNUM_HAL_INTERRUPT_EPON                    28
#define CYGNUM_HAL_INTERRUPT_UART2                   29
#define CYGNUM_HAL_INTERRUPT_UART3                   30
#define CYGNUM_HAL_INTERRUPT_UART4                   31


#define CYGNUM_HAL_ISR_MIN                            1
#define CYGNUM_HAL_ISR_MAX                           31
#define CYGNUM_HAL_ISR_COUNT                         32

// Spurious interrupt (no interrupt source could be found)
#define CYGNUM_HAL_INTERRUPT_NONE                    -1

// The vector used by the Real time clock
#define CYGNUM_HAL_INTERRUPT_RTC                    CYGNUM_HAL_INTERRUPT_TIMER1

// Microsecond delay support.
extern void hal_delay_us(cyg_uint32 usecs);
#define HAL_DELAY_US(n)                             hal_delay_us(n);

// Reset.
// Do a chip reset including asic and arm core reset.
#define HAL_PLATFORM_RESET_ENTRY                    IMST_ROM_ORIGIN

#define HAL_PLATFORM_RESET()			                                \
{						                                                \
    unsigned long cur_interrupt_state;                                  \
    HAL_DISABLE_INTERRUPTS(cur_interrupt_state);                        \
    HAL_WRITE_UINT32(IROS_GLB_GLOBAL_SW_CHIP_RST, ASIC_RESET_PATTERN);  \
    (( void (*) (void))HAL_PLATFORM_RESET_ENTRY)();	                    \
    for (;;); /* if system reset fail, it hangs */                      \
}


#endif // CYGONCE_HAL_PLATFORM_INTS_H

