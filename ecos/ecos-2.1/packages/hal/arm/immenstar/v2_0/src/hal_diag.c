/*=============================================================================
//
//      hal_diag.c
//
//      HAL diagnostic output code
//
//=============================================================================
//
// Date:          January 10, 2005
// Purpose:       HAL diagnostic output
// Description:   Implementations of HAL diagnostic output support.
//
//
//===========================================================================*/

#include <pkgconf/hal.h>
#include <pkgconf/hal_arm_immenstar.h> // board specifics

#include <cyg/infra/cyg_type.h>         // base types
#include <cyg/infra/cyg_trac.h>         // tracing macros
#include <cyg/infra/cyg_ass.h>          // assertion macros

#include <cyg/hal/hal_arch.h>           // basic machine info
#include <cyg/hal/hal_intr.h>           // interrupt macros
#include <cyg/hal/hal_io.h>             // IO macros
#include <cyg/hal/hal_if.h>             // interface API
#include <cyg/hal/hal_misc.h>           // Helper functions
#include <cyg/hal/hal_diag.h>
#include <cyg/hal/drv_api.h>
#include <cyg/hal/hal_immenstar.h>        // Hardware definitions
/*---------------------------------------------------------------------------*/



#if defined(IMST_UART)
#define ARM_IMST_BUAD_DIVISOR_FINE_TUNE_TX  ((CYG_HAL_ARM_IMST_DEV_FREQ/CYGNUM_HAL_VIRTUAL_VECTOR_CONSOLE_CHANNEL_BAUD/48) << 24)
#define ARM_IMST_BUAD_DIVISOR_FINE_TUNE_RCV (3<<16)
#else
#error "TBD: what type of UART?"
#endif

//static void
void
cyg_hal_plf_serial_init_channel(void* __ch_data)
{
    // cyg_uint8* base = ((channel_data_t*)__ch_data)->base;

#if defined(IMST_UART)

    // first, disable everything
    // HAL_WRITE_UINT32(base + IMST_UARTCR, 0);

    // set baud rate
    // set the UART to be 8 bits, 1 stop bit, no parity, fifo enabled
    // finally, enable the uart
    // HAL_WRITE_UINT32(base + IMST_UARTCR,
      //               ARM_IMST_BAUD_DIVISOR |
      //               IMST_UARTCR_WLEN_8 |
      //               IMST_UARTCR_TXEN |
      //               IMST_UARTCR_RXEN |
	  //             IMST_UARTCR_EN);

    //set rx sample
   // HAL_WRITE_UINT32(base + 0x4, ARM_IMST_BAUD_DIVISOR/2);
    //set tun
   // HAL_WRITE_UINT32(base + 0x8, 0x01210003);


#else
#error "TBD: What type of UART ?"
#endif /* IMST_UART */

}

void
cyg_hal_plf_serial_putc(void *__ch_data, char c)
{
        channel_data_t* chan = (channel_data_t*)__ch_data;
        cyg_uint8* base = chan->base;
        cyg_uint32 status;
        int i;

        CYGARC_HAL_SAVE_GP();

        i = 10000;
        do {
                status = GET_STATUS(base);
                i--;
                if (i < 0) break;
        } while (!TX_READY(status));	// wait until ready

        PUT_CHAR(base, c);

        if (c == '\n') {
                i = 10000;
                do {
                        status = GET_STATUS(base);
                        i--;
                        if (i < 0) break;
                } while (!TX_READY(status));	// wait until ready

                PUT_CHAR(base, '\r');
        }

        //hal_delay_us(2);
        //for (i = 0 ; i < 20480; i++);

        //hal_delay_us(1);
        //CYGACC_CALL_IF_DELAY_US(2);

        CYGARC_HAL_RESTORE_GP();
}

/*Don't add '\r' after '\n' for Putc*/
void
cyg_serial_putc(void *__ch_data, char c)
{
        channel_data_t* chan = (channel_data_t*)__ch_data;
        cyg_uint8* base = chan->base;
        cyg_uint32 status;
        int i;

        CYGARC_HAL_SAVE_GP();

        i = 10000;
        do {
                status = GET_STATUS(base);
                i--;
                if (i < 0) break;
        } while (!TX_READY(status));	// wait until ready

        PUT_CHAR(base, c);

        CYGARC_HAL_RESTORE_GP();
}

cyg_bool
cyg_hal_plf_serial_getc_nonblock(void* __ch_data, cyg_uint8* ch)
{
        cyg_uint8* base = ((channel_data_t*)__ch_data)->base;
        cyg_uint32 status ;
        long timeout = 2;  // A long time...

        do {
                status = GET_STATUS(base);
                if (--timeout == 0) return false ;
        } while (!RX_DATA(status));	// wait until ready
        *ch = GET_CHAR(base);
        return true;
}

cyg_uint8
cyg_hal_plf_serial_getc(void* __ch_data)
{
    cyg_uint8 ch;
    CYGARC_HAL_SAVE_GP();

    while(!cyg_hal_plf_serial_getc_nonblock(__ch_data, &ch));

    CYGARC_HAL_RESTORE_GP();
    return ch;
}

#if defined(CYGSEM_HAL_VIRTUAL_VECTOR_DIAG)  || defined(CYGPRI_HAL_IMPLEMENTS_IF_SERVICES)

channel_data_t immenstar_ser_channels[IMST_NUM_OF_UARTS] = {
    { (cyg_uint8*)IMST_UART0_BASE, 1000, CYGNUM_HAL_INTERRUPT_UART0 },
  #if IMST_NUM_OF_UARTS >= 2
    { (cyg_uint8*)IMST_UART1_BASE, 1000, CYGNUM_HAL_INTERRUPT_UART1 }
  #endif
  };

void
cyg_hal_plf_serial_write(void* __ch_data, const cyg_uint8* __buf,
                         cyg_uint32 __len)
{
    CYGARC_HAL_SAVE_GP();

    while(__len-- > 0)
        cyg_hal_plf_serial_putc(__ch_data, *__buf++);

    CYGARC_HAL_RESTORE_GP();
}

void
cyg_hal_plf_serial_read(void* __ch_data, cyg_uint8* __buf, cyg_uint32 __len)
{
    CYGARC_HAL_SAVE_GP();

    while(__len-- > 0)
        *__buf++ = cyg_hal_plf_serial_getc(__ch_data);

    CYGARC_HAL_RESTORE_GP();
}

cyg_bool
cyg_hal_plf_serial_getc_timeout(void* __ch_data, cyg_uint8* ch)
{
    int delay_count;
    channel_data_t* chan = (channel_data_t*)__ch_data;
    cyg_bool res;
    CYGARC_HAL_SAVE_GP();

    delay_count = chan->msec_timeout * 10; // delay in .1 ms steps
    //delay_count = 100;

    for(;;) {
        res = cyg_hal_plf_serial_getc_nonblock(__ch_data, ch);
        if (res || 0 == delay_count--)
            break;

        CYGACC_CALL_IF_DELAY_US(100);
    }

    CYGARC_HAL_RESTORE_GP();
    return res;
}

// Problem: two chans but here we only have one irq_state ?????
int
cyg_hal_plf_serial_control(void *__ch_data, __comm_control_cmd_t __func, ...)
{
        //static int irq_state = 1 /* was 0, set one for testing REDBOOT only */;
        //cyg_uint32 status;
        channel_data_t* chan = (channel_data_t*)__ch_data;
        int ret = 0;

        CYGARC_HAL_SAVE_GP();

        switch (__func) {
                case __COMMCTL_IRQ_ENABLE:
#if 0
                        irq_state = 1;

                        // Ensure that only Receive ints are generated.
                        status = IO_READ(chan->base + IMST_UARTIE);
                        status |= (RX_FIFO_UNDERRUNEIEN  | RX_FIFO_OVERRUNEIEN | RX_PARITY_ERREIEN  | RX_STOP_ERREIEN );
                        HAL_WRITE_UINT32(chan->base + IMST_UARTIE, status);

                        HAL_INTERRUPT_UNMASK(chan->isr_vector);
#endif
                        break;
                case __COMMCTL_IRQ_DISABLE:
#if 0
                        ret = irq_state;
                        irq_state = 0;

                        // Ensure that only Receive ints are generated.
                        status = IO_READ(chan->base + IMST_UARTIE);
                        status &= ~ (RX_FIFO_UNDERRUNEIEN  | RX_FIFO_OVERRUNEIEN | RX_PARITY_ERREIEN  | RX_STOP_ERREIEN | TX_FIFO_OVERRUNEIEN );
                        HAL_WRITE_UINT32(chan->base + IMST_UARTIE, status);

                        HAL_INTERRUPT_MASK(chan->isr_vector);
#endif
                        break;
                case __COMMCTL_DBG_ISR_VECTOR:
                        ret = chan->isr_vector;
                        break;
                case __COMMCTL_SET_TIMEOUT:
                {
                        va_list ap;

                        va_start(ap, __func);

                        ret = chan->msec_timeout;
                        chan->msec_timeout = va_arg(ap, cyg_uint32);

                        va_end(ap);
                }
                default:
                        break;
        }
        CYGARC_HAL_RESTORE_GP();
        return ret;
}

#if !defined(BUILD_MIN_LOADER)
int
cyg_hal_plf_serial_isr(void *__ch_data, int* __ctrlc,
                       CYG_ADDRWORD __vector, CYG_ADDRWORD __data)
{
        int res = 0;
        channel_data_t* chan = (channel_data_t*)__ch_data;
        char c;
        cyg_uint32 status;
        CYGARC_HAL_SAVE_GP();

        cyg_drv_interrupt_acknowledge(chan->isr_vector);

        *__ctrlc = 0;
        status = GET_STATUS(chan->base);

        if ( RX_DATA(status) ) {
                c = GET_CHAR(chan->base);

                if( cyg_hal_is_break( &c , 1 ) )
                        *__ctrlc = 1;

                res = CYG_ISR_HANDLED;
        }

        CYGARC_HAL_RESTORE_GP();
        return res;
}
#endif

//static
void
cyg_hal_plf_serial_init(void)
{

        hal_virtual_comm_table_t* comm;
        int cur = CYGACC_CALL_IF_SET_CONSOLE_COMM(CYGNUM_CALL_IF_SET_COMM_ID_QUERY_CURRENT);

        // Disable interrupts.
        HAL_INTERRUPT_MASK(immenstar_ser_channels[0].isr_vector);
#if IMST_NUM_OF_UARTS >= 2
        HAL_INTERRUPT_MASK(immenstar_ser_channels[1].isr_vector);
#endif

        // Init channels
        cyg_hal_plf_serial_init_channel(&immenstar_ser_channels[0]);
#if IMST_NUM_OF_UARTS >= 2
        cyg_hal_plf_serial_init_channel(&immenstar_ser_channels[1]);
#endif

        // Setup procs in the vector table

        // Set channel 0 for comm_channels[1]
        CYGACC_CALL_IF_SET_CONSOLE_COMM(0);
        comm = CYGACC_CALL_IF_CONSOLE_PROCS();
        CYGACC_COMM_IF_CH_DATA_SET(*comm, &immenstar_ser_channels[0]);
        CYGACC_COMM_IF_WRITE_SET(*comm, cyg_hal_plf_serial_write);
        CYGACC_COMM_IF_READ_SET(*comm, cyg_hal_plf_serial_read);
        CYGACC_COMM_IF_PUTC_SET(*comm, cyg_hal_plf_serial_putc);
        CYGACC_COMM_IF_GETC_SET(*comm, cyg_hal_plf_serial_getc);
        CYGACC_COMM_IF_CONTROL_SET(*comm, cyg_hal_plf_serial_control);
#if !defined(BUILD_MIN_LOADER)
        CYGACC_COMM_IF_DBG_ISR_SET(*comm, cyg_hal_plf_serial_isr);
#endif
        CYGACC_COMM_IF_GETC_TIMEOUT_SET(*comm, cyg_hal_plf_serial_getc_timeout);

#if IMST_NUM_OF_UARTS >= 2
        // Set channel 1 for comm_channels[2]
        CYGACC_CALL_IF_SET_CONSOLE_COMM(1);
        comm = CYGACC_CALL_IF_CONSOLE_PROCS();
        CYGACC_COMM_IF_CH_DATA_SET(*comm, &immenstar_ser_channels[1]);
        CYGACC_COMM_IF_WRITE_SET(*comm, cyg_hal_plf_serial_write);
        CYGACC_COMM_IF_READ_SET(*comm, cyg_hal_plf_serial_read);
        CYGACC_COMM_IF_PUTC_SET(*comm, cyg_hal_plf_serial_putc);
        CYGACC_COMM_IF_GETC_SET(*comm, cyg_hal_plf_serial_getc);
        CYGACC_COMM_IF_CONTROL_SET(*comm, cyg_hal_plf_serial_control);
#if !defined(BUILD_MIN_LOADER)
        CYGACC_COMM_IF_DBG_ISR_SET(*comm, cyg_hal_plf_serial_isr);
#endif
        CYGACC_COMM_IF_GETC_TIMEOUT_SET(*comm, cyg_hal_plf_serial_getc_timeout);

#endif

        // Restore original console
        CYGACC_CALL_IF_SET_CONSOLE_COMM(cur);
        //CYGACC_CALL_IF_SET_CONSOLE_COMM(0); // set console comm id 0 for comm_channels[1]

}

void
cyg_hal_plf_comms_init(void)
{
        static int initialized = 0;
//        cyg_uint32 status;

        if (initialized)
                return;

        initialized = 1;

        cyg_hal_plf_serial_init();

#ifdef CYGDBG_HAL_DEBUG_GDB_BREAK_SUPPORT2
        status = IO_READ(IMST_UART0_BASE + IMST_UARTCR);
        status |= (IMST_UARTCR_RXIE | IMST_UARTCR_IEN);
        HAL_WRITE_UINT32(IMST_UART0_BASE + IMST_UARTCR, status);
        HAL_ENABLE_INTERRUPTS();
        // cyg_hal_gdb_isr_attach();	// FIXME, hack to get CTRLC working
#endif
}

#if !defined(BUILD_MIN_LOADER)
void
cyg_hal_plf_comms_init_stub(void)
{
#ifdef CYGDBG_HAL_DEBUG_GDB_BREAK_SUPPORT
        cyg_uint32 status;
        status = IO_READ(IMST_UART0_BASE + IMST_UARTCR);
        status |= (IMST_UARTCR_RXIE | IMST_UARTCR_IEN);
        HAL_WRITE_UINT32(IMST_UART0_BASE + IMST_UARTCR, status);
        HAL_ENABLE_INTERRUPTS();
        // cyg_hal_gdb_isr_attach();	// FIXME, hack to get CTRLC working
#endif
}
#endif


#endif // CYGSEM_HAL_VIRTUAL_VECTOR_DIAG || CYGPRI_HAL_IMPLEMENTS_IF_SERVICES

/* End of hal_diag.c */
