/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

//==========================================================================
//
//      io.c
//
//      Customized for iROSBoot from RedBoot io.c
//      demo app may borrow this file for io
//
//==========================================================================

#include "iros_config.h"
#include <pkgconf/system.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_immenstar.h>
#include <cyg/hal/hal_diag.h>
#include <cyg/infra/diag.h>
#include <pkgconf/isoinfra.h>
#include <cyg/kernel/kapi.h>

#define _GETS_TIMEOUT -1
#define _GETS_CTRLC   -2
#define _GETS_GDB      0
#define _GETS_OK       1


#define CYGNUM_IROSBOOT_CMD_LINE_EDITING 24
#define CYGPKG_IROSBOOT_MAX_CMD_LINE 128


bool const console_echo = true;
bool const cmd_history = true;

extern char *strcpy( char *s, const char *t);
static void
do_idle(bool is_idle)
{
    // do nothing
    return;
}
extern  cyg_bool
cyg_hal_plf_serial_getc_nonblock(void* __ch_data, cyg_uint8* ch);
extern channel_data_t immenstar_ser_channels[IMST_NUM_OF_UARTS];
extern void
cyg_hal_plf_serial_putc(void *__ch_data, char c);


static void
mon_write_char(char c)
{
#if 0
    hal_virtual_comm_table_t *__chan;

    __chan = CYGACC_CALL_IF_CONSOLE_PROCS();
    if (__chan)
        CYGACC_COMM_IF_PUTC(*__chan, c);
    else {
        __chan = CYGACC_CALL_IF_DEBUG_PROCS();
        CYGACC_COMM_IF_PUTC(*__chan, c);
    }
}
#endif
//    diag_printf("enter mon_wrt_char\n");
    return cyg_hal_plf_serial_putc((void *) &immenstar_ser_channels[0], c);

}



cyg_bool
imst_getc_nonblock(cyg_uint8* ch)
{
#if 0

    cyg_uint32 status ;
    int timeout = 2;  // ... just try twice

    do {
        status = GET_STATUS(IMST_UART0_BASE);
        if (--timeout == 0) return false ;
    } while (!RX_DATA(status)); // wait until ready

    *ch = GET_CHAR(IMST_UART0_BASE);

    return true;
#endif


    //diag_printf("entry  imst_getc_nonblock\n");
    return cyg_hal_plf_serial_getc_nonblock((void *) &immenstar_ser_channels[0], ch);

}

cyg_uint8
imst_getc(void)
{
    cyg_uint8 ch;
    while(!imst_getc_nonblock(&ch)) {
        cyg_thread_delay(5); // delay 0.05 seconds
    };
    return ch;
}

static int imst_getc_delay_count = 1;

cyg_bool
imst_getc_timeout(cyg_uint8* ch)
{
    int delay_count;
    cyg_bool res;

    delay_count = imst_getc_delay_count;

    for(;;) {
        res = imst_getc_nonblock(ch);
        if (res || 0 == delay_count--)
            break;
        cyg_thread_delay(20); // delay 20 ticks, that is, 0.2 seconds
    }

    return res;
}


static void
mon_read_char(char *c)
{
#if !defined(IROSBOOT)
    // read a char directly from UART0
    *c = imst_getc();
#else
    hal_virtual_comm_table_t* __chan = CYGACC_CALL_IF_CONSOLE_PROCS();

    if (__chan)
        *c = CYGACC_COMM_IF_GETC(*__chan);
    else {
        __chan = CYGACC_CALL_IF_DEBUG_PROCS();
        *c = CYGACC_COMM_IF_GETC(*__chan);
    }
#endif
}


bool
mon_read_char_with_timeout(char *c)
{
        bool res = false;

#if !defined(IROSBOOT)
        res = imst_getc_timeout(c);
#else

        hal_virtual_comm_table_t *__chan;

        {
                __chan = CYGACC_CALL_IF_CONSOLE_PROCS();
                if (__chan)
                        res = CYGACC_COMM_IF_GETC_TIMEOUT(*__chan, c);
                else {
                        __chan = CYGACC_CALL_IF_DEBUG_PROCS();
                        res = CYGACC_COMM_IF_GETC_TIMEOUT(*__chan, c);
                }
        }
#endif
        return res;
}


//
// Read a line of input from the user
// Return:
//        _GETS_OK: 'n' valid characters received
//       _GETS_GDB: '$' (GDB lead-in)
//   _GETS_TIMEOUT: No input before timeout
//     _GETS_CTRLC: ^C typed
//
static int
_rb_gets_preloaded(char *buf, int buflen, int timeout)
{
        char *ip = buf;   // Insertion point
        char *eol = buf;  // End of line
        char c;
        bool res = false;
        static char last_ch = '\0';

#if CYGNUM_IROSBOOT_CMD_LINE_EDITING != 0
//
// Command line history support
//    ^P - Select previous line from history
//    ^N - Select next line from history
//    ^A - Move insertion [cursor] to start of line
//    ^E - Move cursor to end of line
//    ^B - Move cursor back [previous character]
//    ^F - Move cursor forward [next character]
//
#define _CL_NUM_LINES CYGNUM_IROSBOOT_CMD_LINE_EDITING       // Number of lines to keep
        static char _cl_lines[_CL_NUM_LINES][CYGPKG_IROSBOOT_MAX_CMD_LINE];
        static int  _cl_index = -1;      // Last known command line
        static int  _cl_max_index = -1;  // Last command in buffers
        int _index = _cl_index;  // Last saved line
        char *xp;
#endif

        // Display current buffer data
        while (*eol) {
                mon_write_char(*eol++);
        }
        ip = eol;

        while (true) {
                if ((timeout > 0) && (eol == buf)) {
                    imst_getc_delay_count = 5; /* 0.2 seconds per count */
                    while (timeout > 0) {
                            res = mon_read_char_with_timeout(&c);
                            if (res) {
                                    // Got a character
                                    do_idle(false);
                                    break;
                            }
                    }
                    if (res == false) {
                            do_idle(true);
                            return _GETS_TIMEOUT;  // Input timed out
                    }
                } else {
                        mon_read_char(&c);
                }
                *eol = '\0';
                switch (c) {
#define CTRL(c) ((c)&0x1F)
#if CYGNUM_IROSBOOT_CMD_LINE_EDITING != 0
                        case CTRL('P'):
                                // Fetch the previous line into the buffer
                                if (_index >= 0) {
                                        // Erase the previous line [crude]
                                        while (ip != buf) {
                                                mon_write_char('\b');
                                                mon_write_char(' ');
                                                mon_write_char('\b');
                                                ip--;
                                        }
                                        strcpy(buf, _cl_lines[_index]);
                                        while (*ip) {
                                                mon_write_char(*ip++);
                                        }
                                        eol = ip;
                                        // Move to previous line
                                        _index--;
                                        if (_index < 0) {
                                                _index = _cl_max_index;
                                        }
                                } else {
                                        mon_write_char(0x07);  // Audible bell on most devices
                                }
                                break;
                        case CTRL('N'):
                                // Fetch the next line into the buffer
                                if (_index >= 0) {
                                        if (++_index > _cl_max_index) _index = 0;
                                        // Erase the previous line [crude]
                                        while (ip != buf) {
                                                mon_write_char('\b');
                                                mon_write_char(' ');
                                                mon_write_char('\b');
                                                ip--;
                                        }
                                        strcpy(buf, _cl_lines[_index]);
                                        while (*ip) {
                                                mon_write_char(*ip++);
                                        }
                                        eol = ip;
                                } else {
                                        mon_write_char(0x07);  // Audible bell on most devices
                                }
                                break;
                        case CTRL('B'):
                                // Move insertion point backwards
                                if (ip != buf) {
                                        mon_write_char('\b');
                                        ip--;
                                }
                                break;
                        case CTRL('F'):
                                // Move insertion point forwards
                                if (ip != eol) {
                                        mon_write_char(*ip++);
                                }
                                break;
                        case CTRL('E'):
                                // Move insertion point to end of line
                                while (ip != eol) {
                                        mon_write_char(*ip++);
                                }
                                break;
                        case CTRL('S'):
                                // Move insertion point to beginning of line
                                if (ip != buf) {
                                        xp = eol;
                                        while (xp-- != buf) {
                                                mon_write_char('\b');
                                        }
                                }
                                ip = buf;
                                break;
                        case CTRL('K'):
                                // Kill to the end of line
                                if (ip != eol) {
                                        xp = ip;
                                        while (xp++ != eol) {
                                                mon_write_char(' ');
                                        }
                                        while (--xp != ip) {
                                                mon_write_char('\b');
                                        }
                                        eol = ip;
                                }
                                break;
                        case CTRL('D'):
                                // Erase the character under the cursor
                                if (ip != eol) {
                                        xp = ip;
                                        eol--;
                                        while (xp != eol) {
                                                *xp = *(xp+1);
                                                mon_write_char(*xp++);
                                        }
                                        mon_write_char(' ');  // Erases last character
                                        mon_write_char('\b');
                                        while (xp-- != ip) {
                                                mon_write_char('\b');
                                        }
                                }
                                break;
#endif // CYGNUM_IROSBOOT_CMD_LINE_EDITING
                        case CTRL('C'): // ^C
                                // Abort current input
                                diag_printf("^C\n");
                                *buf = '\0';  // Nothing useful in buffer
                                return _GETS_CTRLC;
                        case '\n':
                        case '\r':
                                // If previous character was the "other" end-of-line, ignore this one
                                if (((c == '\n') && (last_ch == '\r')) ||
                                    ((c == '\r') && (last_ch == '\n'))) {
                                        c = '\0';
                                        break;
                                }
                                // End of line
                                if (console_echo) {
                                        mon_write_char('\r');
                                        mon_write_char('\n');
                                }
                                last_ch = c;
#if CYGNUM_IROSBOOT_CMD_LINE_EDITING != 0
                                if (cmd_history && (buf != eol)) {
                                        // Save current line - only when enabled
                                        if (++_cl_index == _CL_NUM_LINES) _cl_index = 0;
                                        if (_cl_index > _cl_max_index) _cl_max_index = _cl_index;
                                        strcpy(_cl_lines[_cl_index], buf);
                                }
#endif
                                return _GETS_OK;
                        case '\b':
                        case 0x7F:  // DEL
                                if (ip != buf) {
#if CYGNUM_IROSBOOT_CMD_LINE_EDITING != 0
                                        if (ip != eol) {
                                                ip--;
                                                mon_write_char('\b');
                                                xp = ip;
                                                while (xp != (eol-1)) {
                                                        *xp = *(xp+1);
                                                        mon_write_char(*xp++);
                                                }
                                                mon_write_char(' ');  // Erases last character
                                                mon_write_char('\b');
                                                while (xp-- != ip) {
                                                        mon_write_char('\b');
                                                }
                                        } else {
                                                if (console_echo) {
                                                        mon_write_char('\b');
                                                        mon_write_char(' ');
                                                        mon_write_char('\b');
                                                }
                                                ip--;
                                        }
                                        eol--;
#else
                                        if (console_echo) {
                                                mon_write_char('\b');
                                                mon_write_char(' ');
                                                mon_write_char('\b');
                                        }
                                        ip--;
                                        eol--;
#endif
                                }
                                break;
                        default:
#if CYGNUM_IROSBOOT_CMD_LINE_EDITING != 0
                                // If the insertion point is not at the end of line, make space for it
                                if (ip != eol) {
                                        xp = eol;
                                        *++eol = '\0';
                                        while (xp != ip) {
                                                *xp = *(xp-1);
                                                xp--;
                                        }
                                }
#endif
                                if (console_echo) {
                                        mon_write_char(c);
                                }
                                if (ip == eol) {
                                        // Advance both pointers
                                        *ip++ = c;
                                        eol = ip;
#if CYGNUM_IROSBOOT_CMD_LINE_EDITING != 0
                                } else {
                                        // Just insert the character
                                        *ip++ = c;
                                        xp = ip;
                                        while (xp != eol) {
                                                mon_write_char(*xp++);
                                        }
                                        while (xp-- != ip) {
                                                mon_write_char('\b');
                                        }
#endif
                                }
                                break;
                }
                last_ch = c;
                if (ip == buf + buflen - 1) { // Buffer full
                        *ip = '\0';
                        return buflen;
                }
        }
}

int
_rb_gets(char *buf, int buflen, int timeout)
{
        *buf = '\0';  // Empty buffer
        return _rb_gets_preloaded(buf, buflen, timeout);
}


void mon_write_chars(char* s, int n)
{
    int count = 0;
    for(count = n; count > 0; count--)
        mon_write_char(*s++);
}

bool mon_read_char_timeout(char *c)
{
    cyg_bool res;
    int delay_count = 2;

    for(;;)
    {
        res = imst_getc_nonblock(c);
        if (res || 0 == delay_count)
            break;

        cyg_thread_delay(20); // delay 20 ticks, that is, 0.2 seconds
        delay_count--;
    }

    return res;
}




