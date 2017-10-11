#ifndef CYGONCE_HAL_DIAG_H
#define CYGONCE_HAL_DIAG_H

/*=============================================================================
//
//      hal_diag.h
//
//      HAL Support for Kernel Diagnostic Routines
//
//===========================================================================*/

#include <pkgconf/hal.h>

#include <cyg/infra/cyg_type.h>

#if defined(CYGSEM_HAL_VIRTUAL_VECTOR_DIAG)

#include <cyg/hal/hal_if.h>

#if defined(IMST_UART)
    #define IO_READ(p)              ((*(volatile unsigned int *)(p)))
    #define IO_WRITE(p, c)          (*(volatile unsigned int *)(p) = (c))
    #define GET_STATUS(p)           (IO_READ((p) + IMST_UARTFR))
    #define GET_CHAR(p)             ((IO_READ((p) + IMST_UARTDR)) & 0xff)
    #define PUT_CHAR(p, c)          (IO_WRITE(((p) + IMST_UARTDW), (c)))
    #define RX_DATA(s)              (((s) & IMST_UARTFR_DA) == 0)
    #define TX_READY(s)             (((s) & IMST_UARTFR_TXFF) == 0)
#else
    #error "TBD: what UART?"
#endif

// Define the serial registers.

//-----------------------------------------------------------------------------
typedef struct {
    cyg_uint8* base;
    cyg_int32 msec_timeout;
    int isr_vector;
} channel_data_t;

//-----------------------------------------------------------------------------

#define HAL_DIAG_INIT() hal_if_diag_init()
#define HAL_DIAG_WRITE_CHAR(_c_) hal_if_diag_write_char(_c_)
#define HAL_DIAG_READ_CHAR(_c_) hal_if_diag_read_char(&_c_)

#else // everything by steam

/*---------------------------------------------------------------------------*/
/* functions implemented in hal_diag.c                                       */

externC void hal_diag_init(void);
externC void hal_diag_write_char(char c);
externC void hal_diag_read_char(char *c);

/*---------------------------------------------------------------------------*/

#define HAL_DIAG_INIT() hal_diag_init()

#define HAL_DIAG_WRITE_CHAR(_c_) hal_diag_write_char(_c_)

#define HAL_DIAG_READ_CHAR(_c_) hal_diag_read_char(&_c_)

#endif // CYGSEM_HAL_VIRTUAL_VECTOR_DIAG

/*---------------------------------------------------------------------------*/
// LED

externC void hal_diag_led(int n);

/*---------------------------------------------------------------------------*/
/* end of hal_diag.h                                                         */
#endif /* CYGONCE_HAL_DIAG_H */
