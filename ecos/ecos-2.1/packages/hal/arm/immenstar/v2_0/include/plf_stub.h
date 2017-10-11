#ifndef CYGONCE_HAL_PLF_STUB_H
#define CYGONCE_HAL_PLF_STUB_H

//=============================================================================
//
//      plf_stub.h
//
//      Platform header for GDB stub support.
//
//=============================================================================

#include <pkgconf/hal.h>
#include <pkgconf/hal_arm_immenstar.h>

#ifdef CYGDBG_HAL_DEBUG_GDB_INCLUDE_STUBS

#include <cyg/infra/cyg_type.h>         // CYG_UNUSED_PARAM

#include <cyg/hal/arm_stub.h>           // architecture stub support

//----------------------------------------------------------------------------
// Define some platform specific communication details. This is mostly
// handled by hal_if now, but we need to make sure the comms tables are
// properly initialized.

externC void cyg_hal_plf_comms_init(void);

#define HAL_STUB_PLATFORM_INIT_SERIAL()       cyg_hal_plf_comms_init()

#define HAL_STUB_PLATFORM_SET_BAUD_RATE(baud) CYG_UNUSED_PARAM(int, (baud))
#define HAL_STUB_PLATFORM_INTERRUPTIBLE       0
#define HAL_STUB_PLATFORM_INIT_BREAK_IRQ()    CYG_EMPTY_STATEMENT

extern cyg_uint8 cyg_hal_plf_serial_getc(void* __ch_data);
extern void cyg_hal_plf_serial_putc(void *__ch_data, char c);

#ifndef CYGSEM_HAL_VIRTUAL_VECTOR_SUPPORT
#error "TBD: how to remove VT?"
#define HAL_STUB_PLATFORM_PUT_CHAR(c)         cyg_hal_plf_serial_putc(0,(c))
#define HAL_STUB_PLATFORM_GET_CHAR()          cyg_hal_plf_serial_getc(0)
#endif

//----------------------------------------------------------------------------
// Stub initializer.

#define HAL_STUB_PLATFORM_INIT()              CYG_EMPTY_STATEMENT

//----------------------------------------------------------------------------
// Memory access checks.

// Addresses in the range 0x30000000-0x3fffffff cause the CPU to just
// hang - no timeout, no exception. These macros allow GDB to avoid
// making accidental accesses to this area.

#define CYG_HAL_STUB_PERMIT_DATA_READ(_addr_, _count_) \
( ((CYG_ADDRESS)(_addr_) > 0x3fffffff) || ((((CYG_ADDRESS)(_addr_))+(_count_))< 0x30000000) )

#define CYG_HAL_STUB_PERMIT_DATA_WRITE(_addr_, __count_) \
        CYG_HAL_STUB_PERMIT_DATA_READ(_addr_, _count_)


#endif // ifdef CYGDBG_HAL_DEBUG_GDB_INCLUDE_STUBS

//-----------------------------------------------------------------------------
#endif // CYGONCE_HAL_PLF_STUB_H
// End of plf_stub.h
