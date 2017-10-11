// Author(s):    Henry Zhu
// Date:         November 30, 2004
// Purpose:      IMST debugging support using ARM946ES core module MISC LED
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

/*------------------------------------------------------------------------*/
// On-board timer
/*------------------------------------------------------------------------*/

#if 1
#define IMST_LED_NUM_REPEAT_SHOWING  3
#define IMST_LED_LONG_ON            10
#define IMST_LED_SHORT_ON            2
#define IMST_LED_SIG_GAP             2
#define IMST_LED_NUM_GAP            10
#define IMST_LED_GROUP_GAP          40
#endif

#if 0
#define IMST_LED_NUM_REPEAT_SHOWING  1
#define IMST_LED_LONG_ON            0
#define IMST_LED_SHORT_ON            0
#define IMST_LED_SIG_GAP             0
#define IMST_LED_NUM_GAP            0
#define IMST_LED_GROUP_GAP          0
#endif


#define hal_immenstar_led_on() \
CYG_MACRO_START \
   asm volatile (          \
     "ldr    r0,=0x1000000C;" \
     "ldr    r1,=1;" \
     "strb   r1,[r0];" \
     :  \
     :  \
     : "r1" \
     ); \
CYG_MACRO_END

#define hal_immenstar_led_off() \
CYG_MACRO_START \
   asm volatile (          \
     "ldr    r0,=0x1000000C;" \
     "ldr    r1,=0;" \
     "strb   r1,[r0];" \
     :  \
     :  \
     : "r1" \
     ); \
CYG_MACRO_END

void hal_immenstar_led_show_gap(unsigned int num_sec_tenth) {
  int i;
  int loops;
  loops = num_sec_tenth * 100000; // 10 MHz
  for (i = 0; i < loops; i++);
  return;
}

void hal_immenstar_led_show_long(void) {
  volatile unsigned char b;
  HAL_READ_UINT8(0x1000000C, b);
  HAL_WRITE_UINT8(0x1000000C, (b | 0x01));
  hal_immenstar_led_show_gap(IMST_LED_LONG_ON);
  HAL_WRITE_UINT8(0x1000000C, (b & 0xfe));

}

void hal_immenstar_led_show_short(void) {
  volatile unsigned char b;
  HAL_READ_UINT8(0x1000000C, b);
  HAL_WRITE_UINT8(0x1000000C, (b | 0x01));
  hal_immenstar_led_show_gap(IMST_LED_SHORT_ON);
  HAL_WRITE_UINT8(0x1000000C, (b & 0xfe));
}

void hal_immenstar_led_show_once(unsigned int num) {
  ((unsigned char *)0x0C000000)[0] = num;
  if (num &0x8) {
    hal_immenstar_led_show_long();
  } else {
    hal_immenstar_led_show_short();
  }

  hal_immenstar_led_show_gap(IMST_LED_SIG_GAP);

  if (num &0x4) {
    hal_immenstar_led_show_long();
  } else {
    hal_immenstar_led_show_short();
  }

  hal_immenstar_led_show_gap(IMST_LED_SIG_GAP);

  if (num &0x2) {
    hal_immenstar_led_show_long();
  } else {
    hal_immenstar_led_show_short();
  }
  hal_immenstar_led_show_gap(IMST_LED_SIG_GAP); 

  if (num &0x1) {
    hal_immenstar_led_show_long();
  } else {
    hal_immenstar_led_show_short();
  }

  return;
}


void hal_immenstar_led_show_num(unsigned int num)
{
  unsigned int repeat;
  for (repeat = 0; repeat < IMST_LED_NUM_REPEAT_SHOWING; repeat++) {
    hal_immenstar_led_show_once(num);
    hal_immenstar_led_show_gap(IMST_LED_NUM_GAP); 
  }
  hal_immenstar_led_show_gap(IMST_LED_GROUP_GAP);
  return;
}

void test_main(void) {
  hal_immenstar_led_show_num(7);
  hal_immenstar_led_show_num(3);
}














