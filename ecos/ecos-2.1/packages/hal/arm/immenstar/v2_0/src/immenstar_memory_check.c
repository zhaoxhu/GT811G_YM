// Author(s):    Henry Zhu
// Date:         Decemeber 1, 2004
// Purpose:      IMST memory check
//
//####DESCRIPTIONEND####
//
//===========================================================================*/

#include <pkgconf/hal.h>

#include <cyg/infra/cyg_type.h>         // base types
#include <cyg/infra/cyg_ass.h>          // assertion macros

#include <cyg/hal/hal_io.h>             // IO macros
#include <cyg/hal/hal_arch.h>           // Register state info
#include <cyg/hal/hal_immenstar.h>

extern hal_immenstar_led_show_num(unsigned int);

void immenstar_memory_check(unsigned int * p, unsigned int len) {
  unsigned int i;
  unsigned int r = 1; // good


  for (i=0; i<len; i++) {
    p[i] = 0xaaaaaaaa;
  }

  for (i=0; i<len; i++) {
    if (!(p[i] ^ 0xaaaaaaaa)) {
      r = 0; // bad
      break;
    }
  }

  if (r) {
    hal_immenstar_led_show_num(0xf);
  } else {
    hal_immenstar_led_show_num(0);
  }
  
}
