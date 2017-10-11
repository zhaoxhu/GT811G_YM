/*
 * REGISTERS.h
 */
#ifndef __LYNXD_REGISTERS_H__
#define __LYNXD_REGISTERS_H__

#include "cs_types.h"

/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * per
 */
  



#define PER_IRQ                                  0x00000
#define PER_IRQ_dft                              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 uart4i               :  1 ; /* bits 31:31 */
    cs_uint32 uart3i               :  1 ; /* bits 30:30 */
    cs_uint32 uart2i               :  1 ; /* bits 29:29 */
    cs_uint32 eponi                :  1 ; /* bits 28:28 */
    cs_uint32 niis                 :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 fei                  :  1 ; /* bits 25:25 */
    cs_uint32 bmi                  :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 mai                  :  1 ; /* bits 21:21 */
    cs_uint32 glbi                 :  1 ; /* bits 20:20 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 dma0i                :  1 ; /* bits 16:16 */
    cs_uint32 dma1i                :  1 ; /* bits 15:15 */
    cs_uint32 pcmi                 :  1 ; /* bits 14:14 */
    cs_uint32 ahbi                 :  1 ; /* bits 13:13 */
    cs_uint32 mdioi                :  1 ; /* bits 12:12 */
    cs_uint32 wdti                 :  1 ; /* bits 11:11 */
    cs_uint32 biwi                 :  1 ; /* bits 10:10 */
    cs_uint32 gpioi                :  1 ; /* bits 9:9 */
    cs_uint32 sspi                 :  1 ; /* bits 8:8 */
    cs_uint32 uart1i               :  1 ; /* bits 7:7 */
    cs_uint32 uart0i               :  1 ; /* bits 6:6 */
    cs_uint32 tmr5i                :  1 ; /* bits 5:5 */
    cs_uint32 tmr4i                :  1 ; /* bits 4:4 */
    cs_uint32 tmr3i                :  1 ; /* bits 3:3 */
    cs_uint32 tmr2i                :  1 ; /* bits 2:2 */
    cs_uint32 tmr1i                :  1 ; /* bits 1:1 */
    cs_uint32 softi                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 softi                :  1 ; /* bits 0:0 */
    cs_uint32 tmr1i                :  1 ; /* bits 1:1 */
    cs_uint32 tmr2i                :  1 ; /* bits 2:2 */
    cs_uint32 tmr3i                :  1 ; /* bits 3:3 */
    cs_uint32 tmr4i                :  1 ; /* bits 4:4 */
    cs_uint32 tmr5i                :  1 ; /* bits 5:5 */
    cs_uint32 uart0i               :  1 ; /* bits 6:6 */
    cs_uint32 uart1i               :  1 ; /* bits 7:7 */
    cs_uint32 sspi                 :  1 ; /* bits 8:8 */
    cs_uint32 gpioi                :  1 ; /* bits 9:9 */
    cs_uint32 biwi                 :  1 ; /* bits 10:10 */
    cs_uint32 wdti                 :  1 ; /* bits 11:11 */
    cs_uint32 mdioi                :  1 ; /* bits 12:12 */
    cs_uint32 ahbi                 :  1 ; /* bits 13:13 */
    cs_uint32 pcmi                 :  1 ; /* bits 14:14 */
    cs_uint32 dma1i                :  1 ; /* bits 15:15 */
    cs_uint32 dma0i                :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 glbi                 :  1 ; /* bits 20:20 */
    cs_uint32 mai                  :  1 ; /* bits 21:21 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 bmi                  :  1 ; /* bits 24:24 */
    cs_uint32 fei                  :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 niis                 :  1 ; /* bits 27:27 */
    cs_uint32 eponi                :  1 ; /* bits 28:28 */
    cs_uint32 uart2i               :  1 ; /* bits 29:29 */
    cs_uint32 uart3i               :  1 ; /* bits 30:30 */
    cs_uint32 uart4i               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_IRQ_t;





#define PER_IRQ_ENABLE                           0x00001
#define PER_IRQ_ENABLE_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 uart4e               :  1 ; /* bits 31:31 */
    cs_uint32 uart3e               :  1 ; /* bits 30:30 */
    cs_uint32 uart2e               :  1 ; /* bits 29:29 */
    cs_uint32 epone                :  1 ; /* bits 28:28 */
    cs_uint32 nie                  :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 fee                  :  1 ; /* bits 25:25 */
    cs_uint32 bme                  :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 mae                  :  1 ; /* bits 21:21 */
    cs_uint32 glbe                 :  1 ; /* bits 20:20 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 dma0e                :  1 ; /* bits 16:16 */
    cs_uint32 dma1e                :  1 ; /* bits 15:15 */
    cs_uint32 pcme                 :  1 ; /* bits 14:14 */
    cs_uint32 ahbe                 :  1 ; /* bits 13:13 */
    cs_uint32 mdioe                :  1 ; /* bits 12:12 */
    cs_uint32 wdte                 :  1 ; /* bits 11:11 */
    cs_uint32 biwe                 :  1 ; /* bits 10:10 */
    cs_uint32 gpioe                :  1 ; /* bits 9:9 */
    cs_uint32 sspe                 :  1 ; /* bits 8:8 */
    cs_uint32 uart1e               :  1 ; /* bits 7:7 */
    cs_uint32 uart0e               :  1 ; /* bits 6:6 */
    cs_uint32 tmr5e                :  1 ; /* bits 5:5 */
    cs_uint32 tmr4e                :  1 ; /* bits 4:4 */
    cs_uint32 tmr3e                :  1 ; /* bits 3:3 */
    cs_uint32 tmr2e                :  1 ; /* bits 2:2 */
    cs_uint32 tmr1e                :  1 ; /* bits 1:1 */
    cs_uint32 softe                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 softe                :  1 ; /* bits 0:0 */
    cs_uint32 tmr1e                :  1 ; /* bits 1:1 */
    cs_uint32 tmr2e                :  1 ; /* bits 2:2 */
    cs_uint32 tmr3e                :  1 ; /* bits 3:3 */
    cs_uint32 tmr4e                :  1 ; /* bits 4:4 */
    cs_uint32 tmr5e                :  1 ; /* bits 5:5 */
    cs_uint32 uart0e               :  1 ; /* bits 6:6 */
    cs_uint32 uart1e               :  1 ; /* bits 7:7 */
    cs_uint32 sspe                 :  1 ; /* bits 8:8 */
    cs_uint32 gpioe                :  1 ; /* bits 9:9 */
    cs_uint32 biwe                 :  1 ; /* bits 10:10 */
    cs_uint32 wdte                 :  1 ; /* bits 11:11 */
    cs_uint32 mdioe                :  1 ; /* bits 12:12 */
    cs_uint32 ahbe                 :  1 ; /* bits 13:13 */
    cs_uint32 pcme                 :  1 ; /* bits 14:14 */
    cs_uint32 dma1e                :  1 ; /* bits 15:15 */
    cs_uint32 dma0e                :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 glbe                 :  1 ; /* bits 20:20 */
    cs_uint32 mae                  :  1 ; /* bits 21:21 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 bme                  :  1 ; /* bits 24:24 */
    cs_uint32 fee                  :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 nie                  :  1 ; /* bits 27:27 */
    cs_uint32 epone                :  1 ; /* bits 28:28 */
    cs_uint32 uart2e               :  1 ; /* bits 29:29 */
    cs_uint32 uart3e               :  1 ; /* bits 30:30 */
    cs_uint32 uart4e               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_IRQ_ENABLE_t;





#define PER_IRQ_SOFT                             0x00002
#define PER_IRQ_SOFT_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 softiSet             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 softiSet             :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_IRQ_SOFT_t;





#define PER_FIQ                                  0x00003
#define PER_FIQ_dft                              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 uart4i               :  1 ; /* bits 31:31 */
    cs_uint32 uart3i               :  1 ; /* bits 30:30 */
    cs_uint32 uart2i               :  1 ; /* bits 29:29 */
    cs_uint32 eponi                :  1 ; /* bits 28:28 */
    cs_uint32 niis                 :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd4               :  1 ;
    cs_uint32 fei                  :  1 ; /* bits 25:25 */
    cs_uint32 bmi                  :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd3               :  2 ;
    cs_uint32 mai                  :  1 ; /* bits 21:21 */
    cs_uint32 glbi                 :  1 ; /* bits 20:20 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 dma0i                :  1 ; /* bits 16:16 */
    cs_uint32 dma1i                :  1 ; /* bits 15:15 */
    cs_uint32 pcmi                 :  1 ; /* bits 14:14 */
    cs_uint32 ahbi                 :  1 ; /* bits 13:13 */
    cs_uint32 mdioi                :  1 ; /* bits 12:12 */
    cs_uint32 wdti                 :  1 ; /* bits 11:11 */
    cs_uint32 biwi                 :  1 ; /* bits 10:10 */
    cs_uint32 gpioi                :  1 ; /* bits 9:9 */
    cs_uint32 sspi                 :  1 ; /* bits 8:8 */
    cs_uint32 uart1i               :  1 ; /* bits 7:7 */
    cs_uint32 uart0i               :  1 ; /* bits 6:6 */
    cs_uint32 tmr5i                :  1 ; /* bits 5:5 */
    cs_uint32 tmr4i                :  1 ; /* bits 4:4 */
    cs_uint32 tmr3i                :  1 ; /* bits 3:3 */
    cs_uint32 tmr2i                :  1 ; /* bits 2:2 */
    cs_uint32 tmr1i                :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 tmr1i                :  1 ; /* bits 1:1 */
    cs_uint32 tmr2i                :  1 ; /* bits 2:2 */
    cs_uint32 tmr3i                :  1 ; /* bits 3:3 */
    cs_uint32 tmr4i                :  1 ; /* bits 4:4 */
    cs_uint32 tmr5i                :  1 ; /* bits 5:5 */
    cs_uint32 uart0i               :  1 ; /* bits 6:6 */
    cs_uint32 uart1i               :  1 ; /* bits 7:7 */
    cs_uint32 sspi                 :  1 ; /* bits 8:8 */
    cs_uint32 gpioi                :  1 ; /* bits 9:9 */
    cs_uint32 biwi                 :  1 ; /* bits 10:10 */
    cs_uint32 wdti                 :  1 ; /* bits 11:11 */
    cs_uint32 mdioi                :  1 ; /* bits 12:12 */
    cs_uint32 ahbi                 :  1 ; /* bits 13:13 */
    cs_uint32 pcmi                 :  1 ; /* bits 14:14 */
    cs_uint32 dma1i                :  1 ; /* bits 15:15 */
    cs_uint32 dma0i                :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 glbi                 :  1 ; /* bits 20:20 */
    cs_uint32 mai                  :  1 ; /* bits 21:21 */
    cs_uint32 rsrvd3               :  2 ;
    cs_uint32 bmi                  :  1 ; /* bits 24:24 */
    cs_uint32 fei                  :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd4               :  1 ;
    cs_uint32 niis                 :  1 ; /* bits 27:27 */
    cs_uint32 eponi                :  1 ; /* bits 28:28 */
    cs_uint32 uart2i               :  1 ; /* bits 29:29 */
    cs_uint32 uart3i               :  1 ; /* bits 30:30 */
    cs_uint32 uart4i               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_FIQ_t;





#define PER_FIQ_ENABLE                           0x00004
#define PER_FIQ_ENABLE_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 uart4e               :  1 ; /* bits 31:31 */
    cs_uint32 uart3e               :  1 ; /* bits 30:30 */
    cs_uint32 uart2e               :  1 ; /* bits 29:29 */
    cs_uint32 epone                :  1 ; /* bits 28:28 */
    cs_uint32 nie                  :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd4               :  1 ;
    cs_uint32 fee                  :  1 ; /* bits 25:25 */
    cs_uint32 bme                  :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd3               :  2 ;
    cs_uint32 mae                  :  1 ; /* bits 21:21 */
    cs_uint32 glbe                 :  1 ; /* bits 20:20 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 dma0e                :  1 ; /* bits 16:16 */
    cs_uint32 dma1e                :  1 ; /* bits 15:15 */
    cs_uint32 pcme                 :  1 ; /* bits 14:14 */
    cs_uint32 ahbe                 :  1 ; /* bits 13:13 */
    cs_uint32 mdioe                :  1 ; /* bits 12:12 */
    cs_uint32 wdte                 :  1 ; /* bits 11:11 */
    cs_uint32 biwe                 :  1 ; /* bits 10:10 */
    cs_uint32 gpioe                :  1 ; /* bits 9:9 */
    cs_uint32 sspe                 :  1 ; /* bits 8:8 */
    cs_uint32 uart1e               :  1 ; /* bits 7:7 */
    cs_uint32 uart0e               :  1 ; /* bits 6:6 */
    cs_uint32 tmr5e                :  1 ; /* bits 5:5 */
    cs_uint32 tmr4e                :  1 ; /* bits 4:4 */
    cs_uint32 tmr3e                :  1 ; /* bits 3:3 */
    cs_uint32 tmr2e                :  1 ; /* bits 2:2 */
    cs_uint32 tmr1e                :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 tmr1e                :  1 ; /* bits 1:1 */
    cs_uint32 tmr2e                :  1 ; /* bits 2:2 */
    cs_uint32 tmr3e                :  1 ; /* bits 3:3 */
    cs_uint32 tmr4e                :  1 ; /* bits 4:4 */
    cs_uint32 tmr5e                :  1 ; /* bits 5:5 */
    cs_uint32 uart0e               :  1 ; /* bits 6:6 */
    cs_uint32 uart1e               :  1 ; /* bits 7:7 */
    cs_uint32 sspe                 :  1 ; /* bits 8:8 */
    cs_uint32 gpioe                :  1 ; /* bits 9:9 */
    cs_uint32 biwe                 :  1 ; /* bits 10:10 */
    cs_uint32 wdte                 :  1 ; /* bits 11:11 */
    cs_uint32 mdioe                :  1 ; /* bits 12:12 */
    cs_uint32 ahbe                 :  1 ; /* bits 13:13 */
    cs_uint32 pcme                 :  1 ; /* bits 14:14 */
    cs_uint32 dma1e                :  1 ; /* bits 15:15 */
    cs_uint32 dma0e                :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 glbe                 :  1 ; /* bits 20:20 */
    cs_uint32 mae                  :  1 ; /* bits 21:21 */
    cs_uint32 rsrvd3               :  2 ;
    cs_uint32 bme                  :  1 ; /* bits 24:24 */
    cs_uint32 fee                  :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd4               :  1 ;
    cs_uint32 nie                  :  1 ; /* bits 27:27 */
    cs_uint32 epone                :  1 ; /* bits 28:28 */
    cs_uint32 uart2e               :  1 ; /* bits 29:29 */
    cs_uint32 uart3e               :  1 ; /* bits 30:30 */
    cs_uint32 uart4e               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_FIQ_ENABLE_t;





#define PER_WDT_CTRL                             0x00005
#define PER_WDT_CTRL_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 clksel               :  1 ; /* bits 2:2 */
    cs_uint32 rsten                :  1 ; /* bits 1:1 */
    cs_uint32 wdten                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 wdten                :  1 ; /* bits 0:0 */
    cs_uint32 rsten                :  1 ; /* bits 1:1 */
    cs_uint32 clksel               :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_WDT_CTRL_t;





#define PER_WDT_PS                               0x00006
#define PER_WDT_PS_dft                           0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 wdtps                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 wdtps                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_WDT_PS_t;





#define PER_WDT_DIV                              0x00007
#define PER_WDT_DIV_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 div_load             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 div_load             : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_WDT_DIV_t;





#define PER_WDT_LD                               0x00008
#define PER_WDT_LD_dft                           0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 wdtld                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 wdtld                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_WDT_LD_t;





#define PER_WDT_LOADE                            0x00009
#define PER_WDT_LOADE_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 update_pre           :  1 ; /* bits 1:1 */
    cs_uint32 update_wdt           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 update_wdt           :  1 ; /* bits 0:0 */
    cs_uint32 update_pre           :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_WDT_LOADE_t;





#define PER_WDT_CNT                              0x0000a
#define PER_WDT_CNT_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 wdtcount             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 wdtcount             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_WDT_CNT_t;





#define PER_WDT_IE                               0x0000b
#define PER_WDT_IE_dft                           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 wdte                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 wdte                 :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_WDT_IE_t;





#define PER_WDT_INT                              0x0000c
#define PER_WDT_INT_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 wdti                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 wdti                 :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_WDT_INT_t;





#define PER_WDT_STAT                             0x0000d
#define PER_WDT_STAT_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 wdts                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 wdts                 :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_WDT_STAT_t;





#define PER_WDT_TST                              0x0000e
#define PER_WDT_TST_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 testreset            :  1 ; /* bits 2:2 */
    cs_uint32 testclken            :  1 ; /* bits 1:1 */
    cs_uint32 testmode             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 testmode             :  1 ; /* bits 0:0 */
    cs_uint32 testclken            :  1 ; /* bits 1:1 */
    cs_uint32 testreset            :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_WDT_TST_t;





#define PER_TMR_LD1                              0x0000f
#define PER_TMR_LD1_dft                          0x0000ffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 load1                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 load1                : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_LD1_t;





#define PER_TMR_CTRL1                            0x00010
#define PER_TMR_CTRL1_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 24 ;
    cs_uint32 timen1               :  1 ; /* bits 7:7 */
    cs_uint32 rlmode1              :  1 ; /* bits 6:6 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 clksel1              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd1               :  2 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 clksel1              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 rlmode1              :  1 ; /* bits 6:6 */
    cs_uint32 timen1               :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd3               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_CTRL1_t;





#define PER_TMR_CNT1                             0x00011
#define PER_TMR_CNT1_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 count1               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 count1               : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_CNT1_t;





#define PER_TMR_IE1                              0x00012
#define PER_TMR_IE1_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmre1                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmre1                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_IE1_t;





#define PER_TMR_INT1                             0x00013
#define PER_TMR_INT1_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmri1                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmri1                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_INT1_t;





#define PER_TMR_STAT1                            0x00014
#define PER_TMR_STAT1_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmrs1                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmrs1                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_STAT1_t;





#define PER_TMR_LD2                              0x00015
#define PER_TMR_LD2_dft                          0x0000ffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 load2                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 load2                : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_LD2_t;





#define PER_TMR_CTRL2                            0x00016
#define PER_TMR_CTRL2_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 24 ;
    cs_uint32 timen2               :  1 ; /* bits 7:7 */
    cs_uint32 rlmode2              :  1 ; /* bits 6:6 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 clksel2              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd1               :  2 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 clksel2              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 rlmode2              :  1 ; /* bits 6:6 */
    cs_uint32 timen2               :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd3               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_CTRL2_t;





#define PER_TMR_CNT2                             0x00017
#define PER_TMR_CNT2_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 count2               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 count2               : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_CNT2_t;





#define PER_TMR_IE2                              0x00018
#define PER_TMR_IE2_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmre2                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmre2                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_IE2_t;





#define PER_TMR_INT2                             0x00019
#define PER_TMR_INT2_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmri2                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmri2                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_INT2_t;





#define PER_TMR_STAT2                            0x0001a
#define PER_TMR_STAT2_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmrs2                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmrs2                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_STAT2_t;





#define PER_TMR_LD3                              0x0001b
#define PER_TMR_LD3_dft                          0x0000ffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 load3                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 load3                : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_LD3_t;





#define PER_TMR_CTRL3                            0x0001c
#define PER_TMR_CTRL3_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 24 ;
    cs_uint32 timen3               :  1 ; /* bits 7:7 */
    cs_uint32 rlmode3              :  1 ; /* bits 6:6 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 clksel3              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd1               :  2 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 clksel3              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 rlmode3              :  1 ; /* bits 6:6 */
    cs_uint32 timen3               :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd3               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_CTRL3_t;





#define PER_TMR_CNT3                             0x0001d
#define PER_TMR_CNT3_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 count3               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 count3               : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_CNT3_t;





#define PER_TMR_IE3                              0x0001e
#define PER_TMR_IE3_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmre3                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmre3                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_IE3_t;





#define PER_TMR_INT3                             0x0001f
#define PER_TMR_INT3_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmri3                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmri3                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_INT3_t;





#define PER_TMR_STAT3                            0x00020
#define PER_TMR_STAT3_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmrs3                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmrs3                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_STAT3_t;





#define PER_TMR_LD4                              0x00021
#define PER_TMR_LD4_dft                          0x0000ffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 load4                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 load4                : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_LD4_t;





#define PER_TMR_CTRL4                            0x00022
#define PER_TMR_CTRL4_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 24 ;
    cs_uint32 timen4               :  1 ; /* bits 7:7 */
    cs_uint32 rlmode4              :  1 ; /* bits 6:6 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 clksel4              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd1               :  2 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 clksel4              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 rlmode4              :  1 ; /* bits 6:6 */
    cs_uint32 timen4               :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd3               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_CTRL4_t;





#define PER_TMR_CNT4                             0x00023
#define PER_TMR_CNT4_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 count4               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 count4               : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_CNT4_t;





#define PER_TMR_IE4                              0x00024
#define PER_TMR_IE4_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmre4                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmre4                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_IE4_t;





#define PER_TMR_INT4                             0x00025
#define PER_TMR_INT4_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmri4                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmri4                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_INT4_t;





#define PER_TMR_STAT4                            0x00026
#define PER_TMR_STAT4_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmrs4                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmrs4                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_STAT4_t;





#define PER_TMR_LD5                              0x00027
#define PER_TMR_LD5_dft                          0x0000ffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 load5                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 load5                : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_LD5_t;





#define PER_TMR_CTRL5                            0x00028
#define PER_TMR_CTRL5_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 24 ;
    cs_uint32 timen5               :  1 ; /* bits 7:7 */
    cs_uint32 rlmode5              :  1 ; /* bits 6:6 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 clksel5              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd1               :  2 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 clksel5              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 rlmode5              :  1 ; /* bits 6:6 */
    cs_uint32 timen5               :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd3               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_CTRL5_t;





#define PER_TMR_CNT5                             0x00029
#define PER_TMR_CNT5_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 count5               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 count5               : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_CNT5_t;





#define PER_TMR_IE5                              0x0002a
#define PER_TMR_IE5_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmre5                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmre5                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_IE5_t;





#define PER_TMR_INT5                             0x0002b
#define PER_TMR_INT5_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmri5                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmri5                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_INT5_t;





#define PER_TMR_STAT5                            0x0002c
#define PER_TMR_STAT5_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmrs5                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmrs5                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_STAT5_t;





#define PER_TMR_LOADE                            0x0002d
#define PER_TMR_LOADE_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 26 ;
    cs_uint32 update_tmr5          :  1 ; /* bits 5:5 */
    cs_uint32 update_tmr4          :  1 ; /* bits 4:4 */
    cs_uint32 update_tmr3          :  1 ; /* bits 3:3 */
    cs_uint32 update_tmr2          :  1 ; /* bits 2:2 */
    cs_uint32 update_tmr1          :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 update_tmr1          :  1 ; /* bits 1:1 */
    cs_uint32 update_tmr2          :  1 ; /* bits 2:2 */
    cs_uint32 update_tmr3          :  1 ; /* bits 3:3 */
    cs_uint32 update_tmr4          :  1 ; /* bits 4:4 */
    cs_uint32 update_tmr5          :  1 ; /* bits 5:5 */
    cs_uint32 rsrvd2               : 26 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_LOADE_t;





#define PER_TMR_TST                              0x0002e
#define PER_TMR_TST_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 26 ;
    cs_uint32 test5                :  1 ; /* bits 5:5 */
    cs_uint32 test4                :  1 ; /* bits 4:4 */
    cs_uint32 test3                :  1 ; /* bits 3:3 */
    cs_uint32 test2                :  1 ; /* bits 2:2 */
    cs_uint32 test1                :  1 ; /* bits 1:1 */
    cs_uint32 testclksel           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 testclksel           :  1 ; /* bits 0:0 */
    cs_uint32 test1                :  1 ; /* bits 1:1 */
    cs_uint32 test2                :  1 ; /* bits 2:2 */
    cs_uint32 test3                :  1 ; /* bits 3:3 */
    cs_uint32 test4                :  1 ; /* bits 4:4 */
    cs_uint32 test5                :  1 ; /* bits 5:5 */
    cs_uint32 rsrvd1               : 26 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TMR_TST_t;





#define PER_MDIO_CFG                             0x0002f
#define PER_MDIO_CFG_dft                         0x00000001




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mdio_pre_scale       : 16 ; /* bits 31:16 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 mdio_manual          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mdio_manual          :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 mdio_pre_scale       : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_MDIO_CFG_t;





#define PER_MDIO_ADDR                            0x00030
#define PER_MDIO_ADDR_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 16 ;
    cs_uint32 mdio_rd_wr           :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 mdio_offset          :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 mdio_addr            :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mdio_addr            :  5 ; /* bits 4:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 mdio_offset          :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 mdio_rd_wr           :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd3               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_MDIO_ADDR_t;





#define PER_MDIO_WRDATA                          0x00031
#define PER_MDIO_WRDATA_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 mdio_wrdata          : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mdio_wrdata          : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_MDIO_WRDATA_t;





#define PER_MDIO_RDDATA                          0x00032
#define PER_MDIO_RDDATA_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 mdio_rddata          : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mdio_rddata          : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_MDIO_RDDATA_t;





#define PER_MDIO_CTRL                            0x00033
#define PER_MDIO_CTRL_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 24 ;
    cs_uint32 mdiostart            :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 mdiodone             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mdiodone             :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 mdiostart            :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd2               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_MDIO_CTRL_t;





#define PER_MDIO_AUTO_CFG                        0x00034
#define PER_MDIO_AUTO_CFG_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 11 ;
    cs_uint32 auto_mdio_reg_to_read :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 auto_mdio_phy_end_addr :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 auto_mdio_phy_start_addr :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 auto_mdio_phy_start_addr :  5 ; /* bits 4:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 auto_mdio_phy_end_addr :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 auto_mdio_reg_to_read :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd3               : 11 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_MDIO_AUTO_CFG_t;





#define PER_MDIO_AUTO_INTV                       0x00035
#define PER_MDIO_AUTO_INTV_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 poll_wait_cycles     : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 poll_wait_cycles     : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_MDIO_AUTO_INTV_t;





#define PER_MDIO_AUTO_RM                         0x00036
#define PER_MDIO_AUTO_RM_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 auto_mdio_bit_mask   : 16 ; /* bits 31:16 */
    cs_uint32 auto_mdio_bit_invert : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 auto_mdio_bit_invert : 16 ; /* bits 15:0 */
    cs_uint32 auto_mdio_bit_mask   : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_MDIO_AUTO_RM_t;





#define PER_MDIO_AUTO_AADDR                      0x00037
#define PER_MDIO_AUTO_AADDR_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 auto_mdio_rddata     : 16 ; /* bits 31:16 */
    cs_uint32 rsrvd1               : 11 ;
    cs_uint32 auto_mdio_phy_alert_addr :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 auto_mdio_phy_alert_addr :  5 ; /* bits 4:0 */
    cs_uint32 rsrvd1               : 11 ;
    cs_uint32 auto_mdio_rddata     : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_MDIO_AUTO_AADDR_t;





#define PER_MDIO_IE                              0x00038
#define PER_MDIO_IE_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 autopolle            :  1 ; /* bits 1:1 */
    cs_uint32 mdioe                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mdioe                :  1 ; /* bits 0:0 */
    cs_uint32 autopolle            :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_MDIO_IE_t;





#define PER_MDIO_INT                             0x00039
#define PER_MDIO_INT_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 autopolli            :  1 ; /* bits 1:1 */
    cs_uint32 mdioi                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mdioi                :  1 ; /* bits 0:0 */
    cs_uint32 autopolli            :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_MDIO_INT_t;





#define PER_MDIO_STAT                            0x0003a
#define PER_MDIO_STAT_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 autopolls            :  1 ; /* bits 1:1 */
    cs_uint32 mdios                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mdios                :  1 ; /* bits 0:0 */
    cs_uint32 autopolls            :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_MDIO_STAT_t;





#define PER_BIW0_CFG                             0x0003b
#define PER_BIW0_CFG_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 prer                 : 16 ; /* bits 31:16 */
    cs_uint32 biw_srt_adr          : 14 ; /* bits 15:2 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 core_en              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 core_en              :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 biw_srt_adr          : 14 ; /* bits 15:2 */
    cs_uint32 prer                 : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_BIW0_CFG_t;





#define PER_BIW0_CTRL                            0x0003c
#define PER_BIW0_CTRL_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 24 ;
    cs_uint32 start                :  1 ; /* bits 7:7 */
    cs_uint32 stop                 :  1 ; /* bits 6:6 */
    cs_uint32 read                 :  1 ; /* bits 5:5 */
    cs_uint32 write                :  1 ; /* bits 4:4 */
    cs_uint32 ack_in               :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 biwdone              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 biwdone              :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ack_in               :  1 ; /* bits 3:3 */
    cs_uint32 write                :  1 ; /* bits 4:4 */
    cs_uint32 read                 :  1 ; /* bits 5:5 */
    cs_uint32 stop                 :  1 ; /* bits 6:6 */
    cs_uint32 start                :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd2               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_BIW0_CTRL_t;





#define PER_BIW0_TXR                             0x0003d
#define PER_BIW0_TXR_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 txr                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 txr                  :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_BIW0_TXR_t;





#define PER_BIW0_RXR                             0x0003e
#define PER_BIW0_RXR_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 rxr                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rxr                  :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_BIW0_RXR_t;





#define PER_BIW0_ACK                             0x0003f
#define PER_BIW0_ACK_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 ack_out              :  1 ; /* bits 2:2 */
    cs_uint32 biw_busy             :  1 ; /* bits 1:1 */
    cs_uint32 al                   :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 al                   :  1 ; /* bits 0:0 */
    cs_uint32 biw_busy             :  1 ; /* bits 1:1 */
    cs_uint32 ack_out              :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_BIW0_ACK_t;





#define PER_BIW_IE                               0x00040
#define PER_BIW_IE_dft                           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 biw1e                :  1 ; /* bits 1:1 */
    cs_uint32 biw0e                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 biw0e                :  1 ; /* bits 0:0 */
    cs_uint32 biw1e                :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_BIW_IE_t;





#define PER_BIW_INT                              0x00041
#define PER_BIW_INT_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 biw0i                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 biw0i                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_BIW_INT_t;





#define PER_BIW_STAT                             0x00042
#define PER_BIW_STAT_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 biw0s                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 biw0s                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_BIW_STAT_t;





#define PER_UART0_CFG                            0x00043
#define PER_UART0_CFG_dft                        0x00000003




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 uart_en              :  1 ; /* bits 31:31 */
    cs_uint32 uart_tod_space       :  1 ; /* bits 30:30 */
    cs_uint32 baud_count           : 16 ; /* bits 29:14 */
    cs_uint32 baud_count_hi        :  6 ; /* bits 13:8 */
    cs_uint32 msb_tx_en            :  1 ; /* bits 7:7 */
    cs_uint32 rx_sm_enable         :  1 ; /* bits 6:6 */
    cs_uint32 tx_sm_enable         :  1 ; /* bits 5:5 */
    cs_uint32 uart_parity_en       :  1 ; /* bits 4:4 */
    cs_uint32 even_parity_sel      :  1 ; /* bits 3:3 */
    cs_uint32 uart_stop_2bit       :  1 ; /* bits 2:2 */
    cs_uint32 char_cnt             :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 char_cnt             :  2 ; /* bits 1:0 */
    cs_uint32 uart_stop_2bit       :  1 ; /* bits 2:2 */
    cs_uint32 even_parity_sel      :  1 ; /* bits 3:3 */
    cs_uint32 uart_parity_en       :  1 ; /* bits 4:4 */
    cs_uint32 tx_sm_enable         :  1 ; /* bits 5:5 */
    cs_uint32 rx_sm_enable         :  1 ; /* bits 6:6 */
    cs_uint32 msb_tx_en            :  1 ; /* bits 7:7 */
    cs_uint32 baud_count_hi        :  6 ; /* bits 13:8 */
    cs_uint32 baud_count           : 16 ; /* bits 29:14 */
    cs_uint32 uart_tod_space       :  1 ; /* bits 30:30 */
    cs_uint32 uart_en              :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART0_CFG_t;





#define PER_UART0_RX_SAMPLE                      0x00044
#define PER_UART0_RX_SAMPLE_dft                  0x0000000f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 rx_center            : 22 ; /* bits 21:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_center            : 22 ; /* bits 21:0 */
    cs_uint32 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART0_RX_SAMPLE_t;





#define PER_UART0_TUN                            0x00045
#define PER_UART0_TUN_dft                        0x00000800




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tx_fine_tune_ctl_hi  :  4 ; /* bits 31:28 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 uart_half_blk_lsn    :  1 ; /* bits 25:25 */
    cs_uint32 tx_tune_add          :  1 ; /* bits 24:24 */
    cs_uint32 tx_fine_tune_ctl     :  8 ; /* bits 23:16 */
    cs_uint32 rx_fine_tune_ctl_hi  :  4 ; /* bits 15:12 */
    cs_uint32 uart_rxd_re_pol      :  1 ; /* bits 11:11 */
    cs_uint32 uart_txd_de_pol      :  1 ; /* bits 10:10 */
    cs_uint32 uart_half_duplex     :  1 ; /* bits 9:9 */
    cs_uint32 rx_tune_add          :  1 ; /* bits 8:8 */
    cs_uint32 rx_fine_tune_ctl     :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_fine_tune_ctl     :  8 ; /* bits 7:0 */
    cs_uint32 rx_tune_add          :  1 ; /* bits 8:8 */
    cs_uint32 uart_half_duplex     :  1 ; /* bits 9:9 */
    cs_uint32 uart_txd_de_pol      :  1 ; /* bits 10:10 */
    cs_uint32 uart_rxd_re_pol      :  1 ; /* bits 11:11 */
    cs_uint32 rx_fine_tune_ctl_hi  :  4 ; /* bits 15:12 */
    cs_uint32 tx_fine_tune_ctl     :  8 ; /* bits 23:16 */
    cs_uint32 tx_tune_add          :  1 ; /* bits 24:24 */
    cs_uint32 uart_half_blk_lsn    :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 tx_fine_tune_ctl_hi  :  4 ; /* bits 31:28 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART0_TUN_t;





#define PER_UART0_TXDAT                          0x00046
#define PER_UART0_TXDAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_data              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_data              :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART0_TXDAT_t;





#define PER_UART0_RXDAT                          0x00047
#define PER_UART0_RXDAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 rx_data_vld          :  1 ; /* bits 8:8 */
    cs_uint32 rx_data_lat          :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_data_lat          :  8 ; /* bits 7:0 */
    cs_uint32 rx_data_vld          :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART0_RXDAT_t;





#define PER_UART0_INFO                           0x00048
#define PER_UART0_INFO_dft                       0x0000000a




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 3:3 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_empty        :  1 ; /* bits 1:1 */
    cs_uint32 rx_fifo_full         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_fifo_full         :  1 ; /* bits 0:0 */
    cs_uint32 rx_fifo_empty        :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 2:2 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART0_INFO_t;





#define PER_UART0_IE                             0x00049
#define PER_UART0_IE_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_fifo_emptye       :  1 ; /* bits 7:7 */
    cs_uint32 tx_fifo_fulle        :  1 ; /* bits 6:6 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 rx_fifo_underrune    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overrune     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_erre       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_erre         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overrune     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overrune     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_erre         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_erre       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overrune     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underrune    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 tx_fifo_fulle        :  1 ; /* bits 6:6 */
    cs_uint32 tx_fifo_emptye       :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART0_IE_t;





#define PER_UART0_INT                            0x0004a
#define PER_UART0_INT_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 7:7 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 6:6 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 rx_fifo_underruni    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overruni     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_erri       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_erri         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overruni     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overruni     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_erri         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_erri       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overruni     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underruni    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 6:6 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART0_INT_t;





#define PER_UART0_STAT                           0x0004b
#define PER_UART0_STAT_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 27 ;
    cs_uint32 rx_fifo_underruns    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overruns     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_errs       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_errs         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overruns     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overruns     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_errs         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_errs       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overruns     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underruns    :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               : 27 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART0_STAT_t;





#define PER_UART1_CFG                            0x0004c
#define PER_UART1_CFG_dft                        0x00000003




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 uart_en              :  1 ; /* bits 31:31 */
    cs_uint32 uart_tod_space       :  1 ; /* bits 30:30 */
    cs_uint32 baud_count           : 16 ; /* bits 29:14 */
    cs_uint32 baud_count_hi        :  6 ; /* bits 13:8 */
    cs_uint32 msb_tx_en            :  1 ; /* bits 7:7 */
    cs_uint32 rx_sm_enable         :  1 ; /* bits 6:6 */
    cs_uint32 tx_sm_enable         :  1 ; /* bits 5:5 */
    cs_uint32 uart_parity_en       :  1 ; /* bits 4:4 */
    cs_uint32 even_parity_sel      :  1 ; /* bits 3:3 */
    cs_uint32 uart_stop_2bit       :  1 ; /* bits 2:2 */
    cs_uint32 char_cnt             :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 char_cnt             :  2 ; /* bits 1:0 */
    cs_uint32 uart_stop_2bit       :  1 ; /* bits 2:2 */
    cs_uint32 even_parity_sel      :  1 ; /* bits 3:3 */
    cs_uint32 uart_parity_en       :  1 ; /* bits 4:4 */
    cs_uint32 tx_sm_enable         :  1 ; /* bits 5:5 */
    cs_uint32 rx_sm_enable         :  1 ; /* bits 6:6 */
    cs_uint32 msb_tx_en            :  1 ; /* bits 7:7 */
    cs_uint32 baud_count_hi        :  6 ; /* bits 13:8 */
    cs_uint32 baud_count           : 16 ; /* bits 29:14 */
    cs_uint32 uart_tod_space       :  1 ; /* bits 30:30 */
    cs_uint32 uart_en              :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART1_CFG_t;





#define PER_UART1_RX_SAMPLE                      0x0004d
#define PER_UART1_RX_SAMPLE_dft                  0x0000000f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 rx_center            : 22 ; /* bits 21:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_center            : 22 ; /* bits 21:0 */
    cs_uint32 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART1_RX_SAMPLE_t;





#define PER_UART1_TUN                            0x0004e
#define PER_UART1_TUN_dft                        0x00000800




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tx_fine_tune_ctl_hi  :  4 ; /* bits 31:28 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 uart_half_blk_lsn    :  1 ; /* bits 25:25 */
    cs_uint32 tx_tune_add          :  1 ; /* bits 24:24 */
    cs_uint32 tx_fine_tune_ctl     :  8 ; /* bits 23:16 */
    cs_uint32 rx_fine_tune_ctl_hi  :  4 ; /* bits 15:12 */
    cs_uint32 uart_rxd_re_pol      :  1 ; /* bits 11:11 */
    cs_uint32 uart_txd_de_pol      :  1 ; /* bits 10:10 */
    cs_uint32 uart_half_duplex     :  1 ; /* bits 9:9 */
    cs_uint32 rx_tune_add          :  1 ; /* bits 8:8 */
    cs_uint32 rx_fine_tune_ctl     :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_fine_tune_ctl     :  8 ; /* bits 7:0 */
    cs_uint32 rx_tune_add          :  1 ; /* bits 8:8 */
    cs_uint32 uart_half_duplex     :  1 ; /* bits 9:9 */
    cs_uint32 uart_txd_de_pol      :  1 ; /* bits 10:10 */
    cs_uint32 uart_rxd_re_pol      :  1 ; /* bits 11:11 */
    cs_uint32 rx_fine_tune_ctl_hi  :  4 ; /* bits 15:12 */
    cs_uint32 tx_fine_tune_ctl     :  8 ; /* bits 23:16 */
    cs_uint32 tx_tune_add          :  1 ; /* bits 24:24 */
    cs_uint32 uart_half_blk_lsn    :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 tx_fine_tune_ctl_hi  :  4 ; /* bits 31:28 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART1_TUN_t;





#define PER_UART1_TXDAT                          0x0004f
#define PER_UART1_TXDAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_data              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_data              :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART1_TXDAT_t;





#define PER_UART1_RXDAT                          0x00050
#define PER_UART1_RXDAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 rx_data_vld          :  1 ; /* bits 8:8 */
    cs_uint32 rx_data_lat          :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_data_lat          :  8 ; /* bits 7:0 */
    cs_uint32 rx_data_vld          :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART1_RXDAT_t;





#define PER_UART1_INFO                           0x00051
#define PER_UART1_INFO_dft                       0x0000000a




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 3:3 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_empty        :  1 ; /* bits 1:1 */
    cs_uint32 rx_fifo_full         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_fifo_full         :  1 ; /* bits 0:0 */
    cs_uint32 rx_fifo_empty        :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 2:2 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART1_INFO_t;





#define PER_UART1_IE                             0x00052
#define PER_UART1_IE_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_fifo_emptye       :  1 ; /* bits 7:7 */
    cs_uint32 tx_fifo_fulle        :  1 ; /* bits 6:6 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 rx_fifo_underrune    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overrune     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_erre       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_erre         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overrune     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overrune     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_erre         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_erre       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overrune     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underrune    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 tx_fifo_fulle        :  1 ; /* bits 6:6 */
    cs_uint32 tx_fifo_emptye       :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART1_IE_t;





#define PER_UART1_INT                            0x00053
#define PER_UART1_INT_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 7:7 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 6:6 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 rx_fifo_underruni    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overruni     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_erri       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_erri         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overruni     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overruni     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_erri         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_erri       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overruni     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underruni    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 6:6 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART1_INT_t;





#define PER_UART1_STAT                           0x00054
#define PER_UART1_STAT_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 27 ;
    cs_uint32 rx_fifo_underruns    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overruns     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_errs       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_errs         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overruns     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overruns     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_errs         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_errs       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overruns     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underruns    :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               : 27 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART1_STAT_t;





#define PER_UART2_CFG                            0x00055
#define PER_UART2_CFG_dft                        0x00000003




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 uart_en              :  1 ; /* bits 31:31 */
    cs_uint32 uart_tod_space       :  1 ; /* bits 30:30 */
    cs_uint32 baud_count           : 16 ; /* bits 29:14 */
    cs_uint32 baud_count_hi        :  6 ; /* bits 13:8 */
    cs_uint32 msb_tx_en            :  1 ; /* bits 7:7 */
    cs_uint32 rx_sm_enable         :  1 ; /* bits 6:6 */
    cs_uint32 tx_sm_enable         :  1 ; /* bits 5:5 */
    cs_uint32 uart_parity_en       :  1 ; /* bits 4:4 */
    cs_uint32 even_parity_sel      :  1 ; /* bits 3:3 */
    cs_uint32 uart_stop_2bit       :  1 ; /* bits 2:2 */
    cs_uint32 char_cnt             :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 char_cnt             :  2 ; /* bits 1:0 */
    cs_uint32 uart_stop_2bit       :  1 ; /* bits 2:2 */
    cs_uint32 even_parity_sel      :  1 ; /* bits 3:3 */
    cs_uint32 uart_parity_en       :  1 ; /* bits 4:4 */
    cs_uint32 tx_sm_enable         :  1 ; /* bits 5:5 */
    cs_uint32 rx_sm_enable         :  1 ; /* bits 6:6 */
    cs_uint32 msb_tx_en            :  1 ; /* bits 7:7 */
    cs_uint32 baud_count_hi        :  6 ; /* bits 13:8 */
    cs_uint32 baud_count           : 16 ; /* bits 29:14 */
    cs_uint32 uart_tod_space       :  1 ; /* bits 30:30 */
    cs_uint32 uart_en              :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART2_CFG_t;





#define PER_UART2_RX_SAMPLE                      0x00056
#define PER_UART2_RX_SAMPLE_dft                  0x0000000f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 rx_center            : 22 ; /* bits 21:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_center            : 22 ; /* bits 21:0 */
    cs_uint32 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART2_RX_SAMPLE_t;





#define PER_UART2_TUN                            0x00057
#define PER_UART2_TUN_dft                        0x00000800




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tx_fine_tune_ctl_hi  :  4 ; /* bits 31:28 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 uart_half_blk_lsn    :  1 ; /* bits 25:25 */
    cs_uint32 tx_tune_add          :  1 ; /* bits 24:24 */
    cs_uint32 tx_fine_tune_ctl     :  8 ; /* bits 23:16 */
    cs_uint32 rx_fine_tune_ctl_hi  :  4 ; /* bits 15:12 */
    cs_uint32 uart_rxd_re_pol      :  1 ; /* bits 11:11 */
    cs_uint32 uart_txd_de_pol      :  1 ; /* bits 10:10 */
    cs_uint32 uart_half_duplex     :  1 ; /* bits 9:9 */
    cs_uint32 rx_tune_add          :  1 ; /* bits 8:8 */
    cs_uint32 rx_fine_tune_ctl     :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_fine_tune_ctl     :  8 ; /* bits 7:0 */
    cs_uint32 rx_tune_add          :  1 ; /* bits 8:8 */
    cs_uint32 uart_half_duplex     :  1 ; /* bits 9:9 */
    cs_uint32 uart_txd_de_pol      :  1 ; /* bits 10:10 */
    cs_uint32 uart_rxd_re_pol      :  1 ; /* bits 11:11 */
    cs_uint32 rx_fine_tune_ctl_hi  :  4 ; /* bits 15:12 */
    cs_uint32 tx_fine_tune_ctl     :  8 ; /* bits 23:16 */
    cs_uint32 tx_tune_add          :  1 ; /* bits 24:24 */
    cs_uint32 uart_half_blk_lsn    :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 tx_fine_tune_ctl_hi  :  4 ; /* bits 31:28 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART2_TUN_t;





#define PER_UART2_TXDAT                          0x00058
#define PER_UART2_TXDAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_data              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_data              :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART2_TXDAT_t;





#define PER_UART2_RXDAT                          0x00059
#define PER_UART2_RXDAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 rx_data_vld          :  1 ; /* bits 8:8 */
    cs_uint32 rx_data_lat          :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_data_lat          :  8 ; /* bits 7:0 */
    cs_uint32 rx_data_vld          :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART2_RXDAT_t;





#define PER_UART2_INFO                           0x0005a
#define PER_UART2_INFO_dft                       0x0000000a




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 3:3 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_empty        :  1 ; /* bits 1:1 */
    cs_uint32 rx_fifo_full         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_fifo_full         :  1 ; /* bits 0:0 */
    cs_uint32 rx_fifo_empty        :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 2:2 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART2_INFO_t;





#define PER_UART2_IE                             0x0005b
#define PER_UART2_IE_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_fifo_emptye       :  1 ; /* bits 7:7 */
    cs_uint32 tx_fifo_fulle        :  1 ; /* bits 6:6 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 rx_fifo_underrune    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overrune     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_erre       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_erre         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overrune     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overrune     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_erre         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_erre       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overrune     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underrune    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 tx_fifo_fulle        :  1 ; /* bits 6:6 */
    cs_uint32 tx_fifo_emptye       :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART2_IE_t;





#define PER_UART2_INT                            0x0005c
#define PER_UART2_INT_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 7:7 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 6:6 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 rx_fifo_underruni    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overruni     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_erri       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_erri         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overruni     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overruni     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_erri         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_erri       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overruni     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underruni    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 6:6 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART2_INT_t;





#define PER_UART2_STAT                           0x0005d
#define PER_UART2_STAT_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 27 ;
    cs_uint32 rx_fifo_underruns    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overruns     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_errs       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_errs         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overruns     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overruns     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_errs         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_errs       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overruns     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underruns    :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               : 27 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART2_STAT_t;





#define PER_UART3_CFG                            0x0005e
#define PER_UART3_CFG_dft                        0x00000003




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 uart_en              :  1 ; /* bits 31:31 */
    cs_uint32 uart_tod_space       :  1 ; /* bits 30:30 */
    cs_uint32 baud_count           : 16 ; /* bits 29:14 */
    cs_uint32 baud_count_hi        :  6 ; /* bits 13:8 */
    cs_uint32 msb_tx_en            :  1 ; /* bits 7:7 */
    cs_uint32 rx_sm_enable         :  1 ; /* bits 6:6 */
    cs_uint32 tx_sm_enable         :  1 ; /* bits 5:5 */
    cs_uint32 uart_parity_en       :  1 ; /* bits 4:4 */
    cs_uint32 even_parity_sel      :  1 ; /* bits 3:3 */
    cs_uint32 uart_stop_2bit       :  1 ; /* bits 2:2 */
    cs_uint32 char_cnt             :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 char_cnt             :  2 ; /* bits 1:0 */
    cs_uint32 uart_stop_2bit       :  1 ; /* bits 2:2 */
    cs_uint32 even_parity_sel      :  1 ; /* bits 3:3 */
    cs_uint32 uart_parity_en       :  1 ; /* bits 4:4 */
    cs_uint32 tx_sm_enable         :  1 ; /* bits 5:5 */
    cs_uint32 rx_sm_enable         :  1 ; /* bits 6:6 */
    cs_uint32 msb_tx_en            :  1 ; /* bits 7:7 */
    cs_uint32 baud_count_hi        :  6 ; /* bits 13:8 */
    cs_uint32 baud_count           : 16 ; /* bits 29:14 */
    cs_uint32 uart_tod_space       :  1 ; /* bits 30:30 */
    cs_uint32 uart_en              :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART3_CFG_t;





#define PER_UART3_RX_SAMPLE                      0x0005f
#define PER_UART3_RX_SAMPLE_dft                  0x0000000f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 rx_center            : 22 ; /* bits 21:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_center            : 22 ; /* bits 21:0 */
    cs_uint32 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART3_RX_SAMPLE_t;





#define PER_UART3_TUN                            0x00060
#define PER_UART3_TUN_dft                        0x00000800




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tx_fine_tune_ctl_hi  :  4 ; /* bits 31:28 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 uart_half_blk_lsn    :  1 ; /* bits 25:25 */
    cs_uint32 tx_tune_add          :  1 ; /* bits 24:24 */
    cs_uint32 tx_fine_tune_ctl     :  8 ; /* bits 23:16 */
    cs_uint32 rx_fine_tune_ctl_hi  :  4 ; /* bits 15:12 */
    cs_uint32 uart_rxd_re_pol      :  1 ; /* bits 11:11 */
    cs_uint32 uart_txd_de_pol      :  1 ; /* bits 10:10 */
    cs_uint32 uart_half_duplex     :  1 ; /* bits 9:9 */
    cs_uint32 rx_tune_add          :  1 ; /* bits 8:8 */
    cs_uint32 rx_fine_tune_ctl     :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_fine_tune_ctl     :  8 ; /* bits 7:0 */
    cs_uint32 rx_tune_add          :  1 ; /* bits 8:8 */
    cs_uint32 uart_half_duplex     :  1 ; /* bits 9:9 */
    cs_uint32 uart_txd_de_pol      :  1 ; /* bits 10:10 */
    cs_uint32 uart_rxd_re_pol      :  1 ; /* bits 11:11 */
    cs_uint32 rx_fine_tune_ctl_hi  :  4 ; /* bits 15:12 */
    cs_uint32 tx_fine_tune_ctl     :  8 ; /* bits 23:16 */
    cs_uint32 tx_tune_add          :  1 ; /* bits 24:24 */
    cs_uint32 uart_half_blk_lsn    :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 tx_fine_tune_ctl_hi  :  4 ; /* bits 31:28 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART3_TUN_t;





#define PER_UART3_TXDAT                          0x00061
#define PER_UART3_TXDAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_data              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_data              :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART3_TXDAT_t;





#define PER_UART3_RXDAT                          0x00062
#define PER_UART3_RXDAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 rx_data_vld          :  1 ; /* bits 8:8 */
    cs_uint32 rx_data_lat          :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_data_lat          :  8 ; /* bits 7:0 */
    cs_uint32 rx_data_vld          :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART3_RXDAT_t;





#define PER_UART3_INFO                           0x00063
#define PER_UART3_INFO_dft                       0x0000000a




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 3:3 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_empty        :  1 ; /* bits 1:1 */
    cs_uint32 rx_fifo_full         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_fifo_full         :  1 ; /* bits 0:0 */
    cs_uint32 rx_fifo_empty        :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 2:2 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART3_INFO_t;





#define PER_UART3_IE                             0x00064
#define PER_UART3_IE_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_fifo_emptye       :  1 ; /* bits 7:7 */
    cs_uint32 tx_fifo_fulle        :  1 ; /* bits 6:6 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 rx_fifo_underrune    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overrune     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_erre       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_erre         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overrune     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overrune     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_erre         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_erre       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overrune     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underrune    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 tx_fifo_fulle        :  1 ; /* bits 6:6 */
    cs_uint32 tx_fifo_emptye       :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART3_IE_t;





#define PER_UART3_INT                            0x00065
#define PER_UART3_INT_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 7:7 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 6:6 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 rx_fifo_underruni    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overruni     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_erri       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_erri         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overruni     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overruni     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_erri         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_erri       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overruni     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underruni    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 6:6 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART3_INT_t;





#define PER_UART3_STAT                           0x00066
#define PER_UART3_STAT_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 27 ;
    cs_uint32 rx_fifo_underruns    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overruns     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_errs       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_errs         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overruns     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overruns     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_errs         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_errs       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overruns     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underruns    :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               : 27 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART3_STAT_t;





#define PER_UART4_CFG                            0x00067
#define PER_UART4_CFG_dft                        0x00000003




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 uart_en              :  1 ; /* bits 31:31 */
    cs_uint32 uart_tod_space       :  1 ; /* bits 30:30 */
    cs_uint32 baud_count           : 16 ; /* bits 29:14 */
    cs_uint32 baud_count_hi        :  6 ; /* bits 13:8 */
    cs_uint32 msb_tx_en            :  1 ; /* bits 7:7 */
    cs_uint32 rx_sm_enable         :  1 ; /* bits 6:6 */
    cs_uint32 tx_sm_enable         :  1 ; /* bits 5:5 */
    cs_uint32 uart_parity_en       :  1 ; /* bits 4:4 */
    cs_uint32 even_parity_sel      :  1 ; /* bits 3:3 */
    cs_uint32 uart_stop_2bit       :  1 ; /* bits 2:2 */
    cs_uint32 char_cnt             :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 char_cnt             :  2 ; /* bits 1:0 */
    cs_uint32 uart_stop_2bit       :  1 ; /* bits 2:2 */
    cs_uint32 even_parity_sel      :  1 ; /* bits 3:3 */
    cs_uint32 uart_parity_en       :  1 ; /* bits 4:4 */
    cs_uint32 tx_sm_enable         :  1 ; /* bits 5:5 */
    cs_uint32 rx_sm_enable         :  1 ; /* bits 6:6 */
    cs_uint32 msb_tx_en            :  1 ; /* bits 7:7 */
    cs_uint32 baud_count_hi        :  6 ; /* bits 13:8 */
    cs_uint32 baud_count           : 16 ; /* bits 29:14 */
    cs_uint32 uart_tod_space       :  1 ; /* bits 30:30 */
    cs_uint32 uart_en              :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART4_CFG_t;





#define PER_UART4_RX_SAMPLE                      0x00068
#define PER_UART4_RX_SAMPLE_dft                  0x0000000f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 rx_center            : 22 ; /* bits 21:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_center            : 22 ; /* bits 21:0 */
    cs_uint32 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART4_RX_SAMPLE_t;





#define PER_UART4_TUN                            0x00069
#define PER_UART4_TUN_dft                        0x00000800




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tx_fine_tune_ctl_hi  :  4 ; /* bits 31:28 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 uart_half_blk_lsn    :  1 ; /* bits 25:25 */
    cs_uint32 tx_tune_add          :  1 ; /* bits 24:24 */
    cs_uint32 tx_fine_tune_ctl     :  8 ; /* bits 23:16 */
    cs_uint32 rx_fine_tune_ctl_hi  :  4 ; /* bits 15:12 */
    cs_uint32 uart_rxd_re_pol      :  1 ; /* bits 11:11 */
    cs_uint32 uart_txd_de_pol      :  1 ; /* bits 10:10 */
    cs_uint32 uart_half_duplex     :  1 ; /* bits 9:9 */
    cs_uint32 rx_tune_add          :  1 ; /* bits 8:8 */
    cs_uint32 rx_fine_tune_ctl     :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_fine_tune_ctl     :  8 ; /* bits 7:0 */
    cs_uint32 rx_tune_add          :  1 ; /* bits 8:8 */
    cs_uint32 uart_half_duplex     :  1 ; /* bits 9:9 */
    cs_uint32 uart_txd_de_pol      :  1 ; /* bits 10:10 */
    cs_uint32 uart_rxd_re_pol      :  1 ; /* bits 11:11 */
    cs_uint32 rx_fine_tune_ctl_hi  :  4 ; /* bits 15:12 */
    cs_uint32 tx_fine_tune_ctl     :  8 ; /* bits 23:16 */
    cs_uint32 tx_tune_add          :  1 ; /* bits 24:24 */
    cs_uint32 uart_half_blk_lsn    :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 tx_fine_tune_ctl_hi  :  4 ; /* bits 31:28 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART4_TUN_t;





#define PER_UART4_TXDAT                          0x0006a
#define PER_UART4_TXDAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_data              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_data              :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART4_TXDAT_t;





#define PER_UART4_RXDAT                          0x0006b
#define PER_UART4_RXDAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 rx_data_vld          :  1 ; /* bits 8:8 */
    cs_uint32 rx_data_lat          :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_data_lat          :  8 ; /* bits 7:0 */
    cs_uint32 rx_data_vld          :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART4_RXDAT_t;





#define PER_UART4_INFO                           0x0006c
#define PER_UART4_INFO_dft                       0x0000000a




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 3:3 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_empty        :  1 ; /* bits 1:1 */
    cs_uint32 rx_fifo_full         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_fifo_full         :  1 ; /* bits 0:0 */
    cs_uint32 rx_fifo_empty        :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 2:2 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART4_INFO_t;





#define PER_UART4_IE                             0x0006d
#define PER_UART4_IE_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_fifo_emptye       :  1 ; /* bits 7:7 */
    cs_uint32 tx_fifo_fulle        :  1 ; /* bits 6:6 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 rx_fifo_underrune    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overrune     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_erre       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_erre         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overrune     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overrune     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_erre         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_erre       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overrune     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underrune    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 tx_fifo_fulle        :  1 ; /* bits 6:6 */
    cs_uint32 tx_fifo_emptye       :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART4_IE_t;





#define PER_UART4_INT                            0x0006e
#define PER_UART4_INT_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 7:7 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 6:6 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 rx_fifo_underruni    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overruni     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_erri       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_erri         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overruni     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overruni     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_erri         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_erri       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overruni     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underruni    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_nempty       :  1 ; /* bits 5:5 */
    cs_uint32 tx_fifo_full         :  1 ; /* bits 6:6 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART4_INT_t;





#define PER_UART4_STAT                           0x0006f
#define PER_UART4_STAT_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 27 ;
    cs_uint32 rx_fifo_underruns    :  1 ; /* bits 4:4 */
    cs_uint32 rx_fifo_overruns     :  1 ; /* bits 3:3 */
    cs_uint32 rx_parity_errs       :  1 ; /* bits 2:2 */
    cs_uint32 rx_stop_errs         :  1 ; /* bits 1:1 */
    cs_uint32 tx_fifo_overruns     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_fifo_overruns     :  1 ; /* bits 0:0 */
    cs_uint32 rx_stop_errs         :  1 ; /* bits 1:1 */
    cs_uint32 rx_parity_errs       :  1 ; /* bits 2:2 */
    cs_uint32 rx_fifo_overruns     :  1 ; /* bits 3:3 */
    cs_uint32 rx_fifo_underruns    :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               : 27 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_UART4_STAT_t;





#define PER_GPIO_CFG                             0x00070
#define PER_GPIO_CFG_dft                         0x0000ffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 iopmod               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 iopmod               : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_GPIO_CFG_t;





#define PER_GPIO_OUT                             0x00071
#define PER_GPIO_OUT_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 dataout              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dataout              : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_GPIO_OUT_t;





#define PER_GPIO_IN                              0x00072
#define PER_GPIO_IN_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 iopdata              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 iopdata              : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_GPIO_IN_t;





#define PER_GPIO_LVL                             0x00073
#define PER_GPIO_LVL_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 level                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 level                : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_GPIO_LVL_t;





#define PER_GPIO_IE                              0x00074
#define PER_GPIO_IE_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 gpioe                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 gpioe                : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_GPIO_IE_t;





#define PER_GPIO_INT                             0x00075
#define PER_GPIO_INT_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 gpioi                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 gpioi                : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_GPIO_INT_t;





#define PER_BIW_MEM                              0x00080
#define PER_BIW_MEM_STRIDE                       1
#define PER_BIW_MEM_COUNT                        32
#define PER_BIW_MEM_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 biw_bst_mem          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 biw_bst_mem          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_BIW_MEM_t;





#define PER_BIW_MODE0                            0x000a0
#define PER_BIW_MODE0_dft                        0x0000a000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 div_id               :  8 ; /* bits 15:8 */
    cs_uint32 biw_word_cnt         :  4 ; /* bits 7:4 */
    cs_uint32 blk_adr_len          :  1 ; /* bits 3:3 */
    cs_uint32 blk_rd_en            :  1 ; /* bits 2:2 */
    cs_uint32 biw_trg_sel          :  1 ; /* bits 1:1 */
    cs_uint32 biw_burst            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 biw_burst            :  1 ; /* bits 0:0 */
    cs_uint32 biw_trg_sel          :  1 ; /* bits 1:1 */
    cs_uint32 blk_rd_en            :  1 ; /* bits 2:2 */
    cs_uint32 blk_adr_len          :  1 ; /* bits 3:3 */
    cs_uint32 biw_word_cnt         :  4 ; /* bits 7:4 */
    cs_uint32 div_id               :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_BIW_MODE0_t;





#define PER_TOD_CTRL                             0x000a1
#define PER_TOD_CTRL_dft                         0x0000002f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 26 ;
    cs_uint32 tod_counter          :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tod_counter          :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 26 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PER_TOD_CTRL_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* per */

typedef struct {
  PER_IRQ_t                                IRQ;                           /* addr: 0x00000 */ 
  PER_IRQ_ENABLE_t                         IRQ_ENABLE;                    /* addr: 0x00001 */ 
  PER_IRQ_SOFT_t                           IRQ_SOFT;                      /* addr: 0x00002 */ 
  PER_FIQ_t                                FIQ;                           /* addr: 0x00003 */ 
  PER_FIQ_ENABLE_t                         FIQ_ENABLE;                    /* addr: 0x00004 */ 
  PER_WDT_CTRL_t                           WDT_CTRL;                      /* addr: 0x00005 */ 
  PER_WDT_PS_t                             WDT_PS;                        /* addr: 0x00006 */ 
  PER_WDT_DIV_t                            WDT_DIV;                       /* addr: 0x00007 */ 
  PER_WDT_LD_t                             WDT_LD;                        /* addr: 0x00008 */ 
  PER_WDT_LOADE_t                          WDT_LOADE;                     /* addr: 0x00009 */ 
  PER_WDT_CNT_t                            WDT_CNT;                       /* addr: 0x0000a */ 
  PER_WDT_IE_t                             WDT_IE;                        /* addr: 0x0000b */ 
  PER_WDT_INT_t                            WDT_INT;                       /* addr: 0x0000c */ 
  PER_WDT_STAT_t                           WDT_STAT;                      /* addr: 0x0000d */ 
  PER_WDT_TST_t                            WDT_TST;                       /* addr: 0x0000e */ 
  PER_TMR_LD1_t                            TMR_LD1;                       /* addr: 0x0000f */ 
  PER_TMR_CTRL1_t                          TMR_CTRL1;                     /* addr: 0x00010 */ 
  PER_TMR_CNT1_t                           TMR_CNT1;                      /* addr: 0x00011 */ 
  PER_TMR_IE1_t                            TMR_IE1;                       /* addr: 0x00012 */ 
  PER_TMR_INT1_t                           TMR_INT1;                      /* addr: 0x00013 */ 
  PER_TMR_STAT1_t                          TMR_STAT1;                     /* addr: 0x00014 */ 
  PER_TMR_LD2_t                            TMR_LD2;                       /* addr: 0x00015 */ 
  PER_TMR_CTRL2_t                          TMR_CTRL2;                     /* addr: 0x00016 */ 
  PER_TMR_CNT2_t                           TMR_CNT2;                      /* addr: 0x00017 */ 
  PER_TMR_IE2_t                            TMR_IE2;                       /* addr: 0x00018 */ 
  PER_TMR_INT2_t                           TMR_INT2;                      /* addr: 0x00019 */ 
  PER_TMR_STAT2_t                          TMR_STAT2;                     /* addr: 0x0001a */ 
  PER_TMR_LD3_t                            TMR_LD3;                       /* addr: 0x0001b */ 
  PER_TMR_CTRL3_t                          TMR_CTRL3;                     /* addr: 0x0001c */ 
  PER_TMR_CNT3_t                           TMR_CNT3;                      /* addr: 0x0001d */ 
  PER_TMR_IE3_t                            TMR_IE3;                       /* addr: 0x0001e */ 
  PER_TMR_INT3_t                           TMR_INT3;                      /* addr: 0x0001f */ 
  PER_TMR_STAT3_t                          TMR_STAT3;                     /* addr: 0x00020 */ 
  PER_TMR_LD4_t                            TMR_LD4;                       /* addr: 0x00021 */ 
  PER_TMR_CTRL4_t                          TMR_CTRL4;                     /* addr: 0x00022 */ 
  PER_TMR_CNT4_t                           TMR_CNT4;                      /* addr: 0x00023 */ 
  PER_TMR_IE4_t                            TMR_IE4;                       /* addr: 0x00024 */ 
  PER_TMR_INT4_t                           TMR_INT4;                      /* addr: 0x00025 */ 
  PER_TMR_STAT4_t                          TMR_STAT4;                     /* addr: 0x00026 */ 
  PER_TMR_LD5_t                            TMR_LD5;                       /* addr: 0x00027 */ 
  PER_TMR_CTRL5_t                          TMR_CTRL5;                     /* addr: 0x00028 */ 
  PER_TMR_CNT5_t                           TMR_CNT5;                      /* addr: 0x00029 */ 
  PER_TMR_IE5_t                            TMR_IE5;                       /* addr: 0x0002a */ 
  PER_TMR_INT5_t                           TMR_INT5;                      /* addr: 0x0002b */ 
  PER_TMR_STAT5_t                          TMR_STAT5;                     /* addr: 0x0002c */ 
  PER_TMR_LOADE_t                          TMR_LOADE;                     /* addr: 0x0002d */ 
  PER_TMR_TST_t                            TMR_TST;                       /* addr: 0x0002e */ 
  PER_MDIO_CFG_t                           MDIO_CFG;                      /* addr: 0x0002f */ 
  PER_MDIO_ADDR_t                          MDIO_ADDR;                     /* addr: 0x00030 */ 
  PER_MDIO_WRDATA_t                        MDIO_WRDATA;                   /* addr: 0x00031 */ 
  PER_MDIO_RDDATA_t                        MDIO_RDDATA;                   /* addr: 0x00032 */ 
  PER_MDIO_CTRL_t                          MDIO_CTRL;                     /* addr: 0x00033 */ 
  PER_MDIO_AUTO_CFG_t                      MDIO_AUTO_CFG;                 /* addr: 0x00034 */ 
  PER_MDIO_AUTO_INTV_t                     MDIO_AUTO_INTV;                /* addr: 0x00035 */ 
  PER_MDIO_AUTO_RM_t                       MDIO_AUTO_RM;                  /* addr: 0x00036 */ 
  PER_MDIO_AUTO_AADDR_t                    MDIO_AUTO_AADDR;               /* addr: 0x00037 */ 
  PER_MDIO_IE_t                            MDIO_IE;                       /* addr: 0x00038 */ 
  PER_MDIO_INT_t                           MDIO_INT;                      /* addr: 0x00039 */ 
  PER_MDIO_STAT_t                          MDIO_STAT;                     /* addr: 0x0003a */ 
  PER_BIW0_CFG_t                           BIW0_CFG;                      /* addr: 0x0003b */ 
  PER_BIW0_CTRL_t                          BIW0_CTRL;                     /* addr: 0x0003c */ 
  PER_BIW0_TXR_t                           BIW0_TXR;                      /* addr: 0x0003d */ 
  PER_BIW0_RXR_t                           BIW0_RXR;                      /* addr: 0x0003e */ 
  PER_BIW0_ACK_t                           BIW0_ACK;                      /* addr: 0x0003f */ 
  PER_BIW_IE_t                             BIW_IE;                        /* addr: 0x00040 */ 
  PER_BIW_INT_t                            BIW_INT;                       /* addr: 0x00041 */ 
  PER_BIW_STAT_t                           BIW_STAT;                      /* addr: 0x00042 */ 
  PER_UART0_CFG_t                          UART0_CFG;                     /* addr: 0x00043 */ 
  PER_UART0_RX_SAMPLE_t                    UART0_RX_SAMPLE;               /* addr: 0x00044 */ 
  PER_UART0_TUN_t                          UART0_TUN;                     /* addr: 0x00045 */ 
  PER_UART0_TXDAT_t                        UART0_TXDAT;                   /* addr: 0x00046 */ 
  PER_UART0_RXDAT_t                        UART0_RXDAT;                   /* addr: 0x00047 */ 
  PER_UART0_INFO_t                         UART0_INFO;                    /* addr: 0x00048 */ 
  PER_UART0_IE_t                           UART0_IE;                      /* addr: 0x00049 */ 
  PER_UART0_INT_t                          UART0_INT;                     /* addr: 0x0004a */ 
  PER_UART0_STAT_t                         UART0_STAT;                    /* addr: 0x0004b */ 
  PER_UART1_CFG_t                          UART1_CFG;                     /* addr: 0x0004c */ 
  PER_UART1_RX_SAMPLE_t                    UART1_RX_SAMPLE;               /* addr: 0x0004d */ 
  PER_UART1_TUN_t                          UART1_TUN;                     /* addr: 0x0004e */ 
  PER_UART1_TXDAT_t                        UART1_TXDAT;                   /* addr: 0x0004f */ 
  PER_UART1_RXDAT_t                        UART1_RXDAT;                   /* addr: 0x00050 */ 
  PER_UART1_INFO_t                         UART1_INFO;                    /* addr: 0x00051 */ 
  PER_UART1_IE_t                           UART1_IE;                      /* addr: 0x00052 */ 
  PER_UART1_INT_t                          UART1_INT;                     /* addr: 0x00053 */ 
  PER_UART1_STAT_t                         UART1_STAT;                    /* addr: 0x00054 */ 
  PER_UART2_CFG_t                          UART2_CFG;                     /* addr: 0x00055 */ 
  PER_UART2_RX_SAMPLE_t                    UART2_RX_SAMPLE;               /* addr: 0x00056 */ 
  PER_UART2_TUN_t                          UART2_TUN;                     /* addr: 0x00057 */ 
  PER_UART2_TXDAT_t                        UART2_TXDAT;                   /* addr: 0x00058 */ 
  PER_UART2_RXDAT_t                        UART2_RXDAT;                   /* addr: 0x00059 */ 
  PER_UART2_INFO_t                         UART2_INFO;                    /* addr: 0x0005a */ 
  PER_UART2_IE_t                           UART2_IE;                      /* addr: 0x0005b */ 
  PER_UART2_INT_t                          UART2_INT;                     /* addr: 0x0005c */ 
  PER_UART2_STAT_t                         UART2_STAT;                    /* addr: 0x0005d */ 
  PER_UART3_CFG_t                          UART3_CFG;                     /* addr: 0x0005e */ 
  PER_UART3_RX_SAMPLE_t                    UART3_RX_SAMPLE;               /* addr: 0x0005f */ 
  PER_UART3_TUN_t                          UART3_TUN;                     /* addr: 0x00060 */ 
  PER_UART3_TXDAT_t                        UART3_TXDAT;                   /* addr: 0x00061 */ 
  PER_UART3_RXDAT_t                        UART3_RXDAT;                   /* addr: 0x00062 */ 
  PER_UART3_INFO_t                         UART3_INFO;                    /* addr: 0x00063 */ 
  PER_UART3_IE_t                           UART3_IE;                      /* addr: 0x00064 */ 
  PER_UART3_INT_t                          UART3_INT;                     /* addr: 0x00065 */ 
  PER_UART3_STAT_t                         UART3_STAT;                    /* addr: 0x00066 */ 
  PER_UART4_CFG_t                          UART4_CFG;                     /* addr: 0x00067 */ 
  PER_UART4_RX_SAMPLE_t                    UART4_RX_SAMPLE;               /* addr: 0x00068 */ 
  PER_UART4_TUN_t                          UART4_TUN;                     /* addr: 0x00069 */ 
  PER_UART4_TXDAT_t                        UART4_TXDAT;                   /* addr: 0x0006a */ 
  PER_UART4_RXDAT_t                        UART4_RXDAT;                   /* addr: 0x0006b */ 
  PER_UART4_INFO_t                         UART4_INFO;                    /* addr: 0x0006c */ 
  PER_UART4_IE_t                           UART4_IE;                      /* addr: 0x0006d */ 
  PER_UART4_INT_t                          UART4_INT;                     /* addr: 0x0006e */ 
  PER_UART4_STAT_t                         UART4_STAT;                    /* addr: 0x0006f */ 
  PER_GPIO_CFG_t                           GPIO_CFG;                      /* addr: 0x00070 */ 
  PER_GPIO_OUT_t                           GPIO_OUT;                      /* addr: 0x00071 */ 
  PER_GPIO_IN_t                            GPIO_IN;                       /* addr: 0x00072 */ 
  PER_GPIO_LVL_t                           GPIO_LVL;                      /* addr: 0x00073 */ 
  PER_GPIO_IE_t                            GPIO_IE;                       /* addr: 0x00074 */ 
  PER_GPIO_INT_t                           GPIO_INT;                      /* addr: 0x00075 */ 
  cs_uint32                                rsrvd1[10];                   
  PER_BIW_MEM_t                            BIW_MEM;                       /* addr: 0x00080 */ 
  cs_uint32                                rsrvd2[31];                   
  PER_BIW_MODE0_t                          BIW_MODE0;                     /* addr: 0x000a0 */ 
  PER_TOD_CTRL_t                           TOD_CTRL;                      /* addr: 0x000a1 */ 
} PER_t;

  

/*
 * ahb
 */
  



#define AHB_CMD_IE                               0x01000
#define AHB_CMD_IE_dft                           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 reg_acce             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 reg_acce             :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_CMD_IE_t;





#define AHB_CMD_INT                              0x01001
#define AHB_CMD_INT_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 reg_acci             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 reg_acci             :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_CMD_INT_t;





#define AHB_FLASH_ACC_CTRL                       0x01002
#define AHB_FLASH_ACC_CTRL_dft                   0x1f83ec93




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 wait_async_lpb_cmd   :  8 ; /* bits 31:24 */
    cs_uint32 sync_cmd2cmd_wait    :  3 ; /* bits 23:21 */
    cs_uint32 wait_flash_cmd       :  8 ; /* bits 20:13 */
    cs_uint32 wait_back_porch      :  3 ; /* bits 12:10 */
    cs_uint32 wait_async_sram_cmd  :  7 ; /* bits 9:3 */
    cs_uint32 wait_front_porch     :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 wait_front_porch     :  3 ; /* bits 2:0 */
    cs_uint32 wait_async_sram_cmd  :  7 ; /* bits 9:3 */
    cs_uint32 wait_back_porch      :  3 ; /* bits 12:10 */
    cs_uint32 wait_flash_cmd       :  8 ; /* bits 20:13 */
    cs_uint32 sync_cmd2cmd_wait    :  3 ; /* bits 23:21 */
    cs_uint32 wait_async_lpb_cmd   :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_FLASH_ACC_CTRL_t;





#define AHB_SRAM_CFG                             0x01003
#define AHB_SRAM_CFG_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 sram_bus_width       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sram_bus_width       :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SRAM_CFG_t;





#define AHB_REG_TIMEOUT                          0x01004
#define AHB_REG_TIMEOUT_dft                      0x000004e2




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 timeout_load         : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 timeout_load         : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_REG_TIMEOUT_t;





#define AHB_SFSH_CLK                             0x01005
#define AHB_SFSH_CLK_dft                         0x00270001




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 sfsh_ssp_addr_sel    :  3 ; /* bits 25:23 */
    cs_uint32 sfsh_cmd_cnt_ld      :  7 ; /* bits 22:16 */
    cs_uint32 sfsh_counter_load    : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sfsh_counter_load    : 16 ; /* bits 15:0 */
    cs_uint32 sfsh_cmd_cnt_ld      :  7 ; /* bits 22:16 */
    cs_uint32 sfsh_ssp_addr_sel    :  3 ; /* bits 25:23 */
    cs_uint32 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SFSH_CLK_t;





#define AHB_DDR_TO_LIM                           0x01006
#define AHB_DDR_TO_LIM_dft                       0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rd_timeout_lim       : 16 ; /* bits 31:16 */
    cs_uint32 wr_timeout_lim       : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 wr_timeout_lim       : 16 ; /* bits 15:0 */
    cs_uint32 rd_timeout_lim       : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_DDR_TO_LIM_t;





#define AHB_DDR_TO_EN                            0x01007
#define AHB_DDR_TO_EN_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 rd_timeout_en        :  1 ; /* bits 1:1 */
    cs_uint32 wr_timeout_en        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 wr_timeout_en        :  1 ; /* bits 0:0 */
    cs_uint32 rd_timeout_en        :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_DDR_TO_EN_t;

#define     rd_timeout_en_DIS                             0x0
#define     rd_timeout_en_ENA                             0x1
#define     wr_timeout_en_DIS                             0x0
#define     wr_timeout_en_ENA                             0x1




#define AHB_RPC                                  0x01008
#define AHB_RPC_dft                              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 remap                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 remap                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_RPC_t;





#define AHB_AHB_TIMEOUT                          0x01009
#define AHB_AHB_TIMEOUT_dft                      0x0000ffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ahb                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ahb                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_AHB_TIMEOUT_t;





#define AHB_REG_ACC_RCD                          0x0100a
#define AHB_REG_ACC_RCD_dft                      0x00000003




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 addr                 : 22 ; /* bits 23:2 */
    cs_uint32 reg_acc_b2b_delay    :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 reg_acc_b2b_delay    :  2 ; /* bits 1:0 */
    cs_uint32 addr                 : 22 ; /* bits 23:2 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_REG_ACC_RCD_t;





#define AHB_REG_IF_ST                            0x01010
#define AHB_REG_IF_ST_STRIDE                     1
#define AHB_REG_IF_ST_COUNT                      8
#define AHB_REG_IF_ST_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 reg_if               : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 reg_if               : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_REG_IF_ST_t;





#define AHB_SSP_CLK                              0x01018
#define AHB_SSP_CLK_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 counter_load         : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 counter_load         : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_CLK_t;





#define AHB_SSP_CFG                              0x01019
#define AHB_SSP_CFG_dft                          0x001f0900




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 sel_ssp_cs           :  4 ; /* bits 31:28 */
    cs_uint32 micro_wire_cs_sel    :  4 ; /* bits 27:24 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 pre_ssp_dat_cnt      :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 ssp_cmd_cnt          :  7 ; /* bits 14:8 */
    cs_uint32 ssp_burst_en         :  1 ; /* bits 7:7 */
    cs_uint32 cont_clk             :  1 ; /* bits 6:6 */
    cs_uint32 tdat_cpha            :  1 ; /* bits 5:5 */
    cs_uint32 idat_mode            :  1 ; /* bits 4:4 */
    cs_uint32 datin_cmds           :  1 ; /* bits 3:3 */
    cs_uint32 edge_align           :  1 ; /* bits 2:2 */
    cs_uint32 read_write           :  1 ; /* bits 1:1 */
    cs_uint32 command_cyc          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 command_cyc          :  1 ; /* bits 0:0 */
    cs_uint32 read_write           :  1 ; /* bits 1:1 */
    cs_uint32 edge_align           :  1 ; /* bits 2:2 */
    cs_uint32 datin_cmds           :  1 ; /* bits 3:3 */
    cs_uint32 idat_mode            :  1 ; /* bits 4:4 */
    cs_uint32 tdat_cpha            :  1 ; /* bits 5:5 */
    cs_uint32 cont_clk             :  1 ; /* bits 6:6 */
    cs_uint32 ssp_burst_en         :  1 ; /* bits 7:7 */
    cs_uint32 ssp_cmd_cnt          :  7 ; /* bits 14:8 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 pre_ssp_dat_cnt      :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 micro_wire_cs_sel    :  4 ; /* bits 27:24 */
    cs_uint32 sel_ssp_cs           :  4 ; /* bits 31:28 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_CFG_t;





#define AHB_SSP_CTRL                             0x0101a
#define AHB_SSP_CTRL_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 24 ;
    cs_uint32 sspstart             :  1 ; /* bits 7:7 */
    cs_uint32 sspbststart          :  1 ; /* bits 6:6 */
    cs_uint32 sspbststart_ext      :  1 ; /* bits 5:5 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 sspdone              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sspdone              :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 sspbststart_ext      :  1 ; /* bits 5:5 */
    cs_uint32 sspbststart          :  1 ; /* bits 6:6 */
    cs_uint32 sspstart             :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd2               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_CTRL_t;





#define AHB_SSP_CA0                              0x0101b
#define AHB_SSP_CA0_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ssp_cmd_addr0        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ssp_cmd_addr0        : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_CA0_t;





#define AHB_SSP_CA1                              0x0101c
#define AHB_SSP_CA1_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ssp_cmd_addr1        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ssp_cmd_addr1        : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_CA1_t;





#define AHB_SSP_CA2                              0x0101d
#define AHB_SSP_CA2_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ssp_cmd_addr2        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ssp_cmd_addr2        : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_CA2_t;





#define AHB_SSP_WDAT                             0x0101e
#define AHB_SSP_WDAT_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ssp_wdata            : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ssp_wdata            : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_WDAT_t;





#define AHB_SSP_RDAT                             0x0101f
#define AHB_SSP_RDAT_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ssp_rdata            : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ssp_rdata            : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_RDAT_t;





#define AHB_SSP_IE                               0x01020
#define AHB_SSP_IE_dft                           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 write_completee      :  1 ; /* bits 3:3 */
    cs_uint32 sspbsterre           :  1 ; /* bits 2:2 */
    cs_uint32 sspbste              :  1 ; /* bits 1:1 */
    cs_uint32 sspe                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sspe                 :  1 ; /* bits 0:0 */
    cs_uint32 sspbste              :  1 ; /* bits 1:1 */
    cs_uint32 sspbsterre           :  1 ; /* bits 2:2 */
    cs_uint32 write_completee      :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_IE_t;





#define AHB_SSP_INT                              0x01021
#define AHB_SSP_INT_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 write_complete       :  1 ; /* bits 3:3 */
    cs_uint32 sspbsterr            :  1 ; /* bits 2:2 */
    cs_uint32 sspbsti              :  1 ; /* bits 1:1 */
    cs_uint32 sspi                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sspi                 :  1 ; /* bits 0:0 */
    cs_uint32 sspbsti              :  1 ; /* bits 1:1 */
    cs_uint32 sspbsterr            :  1 ; /* bits 2:2 */
    cs_uint32 write_complete       :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_INT_t;





#define AHB_SSP_STAT                             0x01022
#define AHB_SSP_STAT_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 write_complete       :  1 ; /* bits 3:3 */
    cs_uint32 sspbsterr            :  1 ; /* bits 2:2 */
    cs_uint32 sspbsts              :  1 ; /* bits 1:1 */
    cs_uint32 ssps                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ssps                 :  1 ; /* bits 0:0 */
    cs_uint32 sspbsts              :  1 ; /* bits 1:1 */
    cs_uint32 sspbsterr            :  1 ; /* bits 2:2 */
    cs_uint32 write_complete       :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_STAT_t;





#define AHB_SSP_BST0                             0x01023
#define AHB_SSP_BST0_dft                         0x4f0107ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 sbst_acc_cmd_cnt     :  7 ; /* bits 31:25 */
    cs_uint32 sbst_acc_read_write  :  1 ; /* bits 24:24 */
    cs_uint32 sbst_micwire_cs_sel  :  4 ; /* bits 23:20 */
    cs_uint32 sbst_reg_sel_ssp_cs  :  4 ; /* bits 19:16 */
    cs_uint32 sbst_reg_ssp_dat_cnt : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sbst_reg_ssp_dat_cnt : 16 ; /* bits 15:0 */
    cs_uint32 sbst_reg_sel_ssp_cs  :  4 ; /* bits 19:16 */
    cs_uint32 sbst_micwire_cs_sel  :  4 ; /* bits 23:20 */
    cs_uint32 sbst_acc_read_write  :  1 ; /* bits 24:24 */
    cs_uint32 sbst_acc_cmd_cnt     :  7 ; /* bits 31:25 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_BST0_t;





#define AHB_SSP_BST1                             0x01024
#define AHB_SSP_BST1_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 sbst_cmd_cnt_sel     :  1 ; /* bits 31:31 */
    cs_uint32 sbst_ssp_addr_sel    :  3 ; /* bits 30:28 */
    cs_uint32 sbst_ssp_start_adr   : 28 ; /* bits 27:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sbst_ssp_start_adr   : 28 ; /* bits 27:0 */
    cs_uint32 sbst_ssp_addr_sel    :  3 ; /* bits 30:28 */
    cs_uint32 sbst_cmd_cnt_sel     :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_BST1_t;





#define AHB_SSP_BST2                             0x01025
#define AHB_SSP_BST2_dft                         0x2c000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 sbst_ssp_dest_adr    : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sbst_ssp_dest_adr    : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_BST2_t;





#define AHB_SSP_BST3                             0x01026
#define AHB_SSP_BST3_dft                         0x000fffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 sbst_wrcomplete_cnt  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sbst_wrcomplete_cnt  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_BST3_t;





#define AHB_SSP_BST_STAT                         0x01027
#define AHB_SSP_BST_STAT_dft                     0x00000020




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 wcomplete_cnt_zero   :  1 ; /* bits 7:7 */
    cs_uint32 sbst_wdata_fifo_st   :  1 ; /* bits 6:6 */
    cs_uint32 sbst_sel_idle        :  1 ; /* bits 5:5 */
    cs_uint32 wdata_req_sm_sav     :  2 ; /* bits 4:3 */
    cs_uint32 sbst_sel_sav         :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sbst_sel_sav         :  3 ; /* bits 2:0 */
    cs_uint32 wdata_req_sm_sav     :  2 ; /* bits 4:3 */
    cs_uint32 sbst_sel_idle        :  1 ; /* bits 5:5 */
    cs_uint32 sbst_wdata_fifo_st   :  1 ; /* bits 6:6 */
    cs_uint32 wcomplete_cnt_zero   :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} AHB_SSP_BST_STAT_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* ahb */

typedef struct {
  AHB_CMD_IE_t                             CMD_IE;                        /* addr: 0x00000 */ 
  AHB_CMD_INT_t                            CMD_INT;                       /* addr: 0x00001 */ 
  AHB_FLASH_ACC_CTRL_t                     FLASH_ACC_CTRL;                /* addr: 0x00002 */ 
  AHB_SRAM_CFG_t                           SRAM_CFG;                      /* addr: 0x00003 */ 
  AHB_REG_TIMEOUT_t                        REG_TIMEOUT;                   /* addr: 0x00004 */ 
  AHB_SFSH_CLK_t                           SFSH_CLK;                      /* addr: 0x00005 */ 
  AHB_DDR_TO_LIM_t                         DDR_TO_LIM;                    /* addr: 0x00006 */ 
  AHB_DDR_TO_EN_t                          DDR_TO_EN;                     /* addr: 0x00007 */ 
  AHB_RPC_t                                RPC;                           /* addr: 0x00008 */ 
  AHB_AHB_TIMEOUT_t                        AHB_TIMEOUT;                   /* addr: 0x00009 */ 
  AHB_REG_ACC_RCD_t                        REG_ACC_RCD;                   /* addr: 0x0000a */ 
  cs_uint32                                rsrvd1[5];                    
  AHB_REG_IF_ST_t                          REG_IF_ST;                     /* addr: 0x00010 */ 
  cs_uint32                                rsrvd2[7];                    
  AHB_SSP_CLK_t                            SSP_CLK;                       /* addr: 0x00018 */ 
  AHB_SSP_CFG_t                            SSP_CFG;                       /* addr: 0x00019 */ 
  AHB_SSP_CTRL_t                           SSP_CTRL;                      /* addr: 0x0001a */ 
  AHB_SSP_CA0_t                            SSP_CA0;                       /* addr: 0x0001b */ 
  AHB_SSP_CA1_t                            SSP_CA1;                       /* addr: 0x0001c */ 
  AHB_SSP_CA2_t                            SSP_CA2;                       /* addr: 0x0001d */ 
  AHB_SSP_WDAT_t                           SSP_WDAT;                      /* addr: 0x0001e */ 
  AHB_SSP_RDAT_t                           SSP_RDAT;                      /* addr: 0x0001f */ 
  AHB_SSP_IE_t                             SSP_IE;                        /* addr: 0x00020 */ 
  AHB_SSP_INT_t                            SSP_INT;                       /* addr: 0x00021 */ 
  AHB_SSP_STAT_t                           SSP_STAT;                      /* addr: 0x00022 */ 
  AHB_SSP_BST0_t                           SSP_BST0;                      /* addr: 0x00023 */ 
  AHB_SSP_BST1_t                           SSP_BST1;                      /* addr: 0x00024 */ 
  AHB_SSP_BST2_t                           SSP_BST2;                      /* addr: 0x00025 */ 
  AHB_SSP_BST3_t                           SSP_BST3;                      /* addr: 0x00026 */ 
  AHB_SSP_BST_STAT_t                       SSP_BST_STAT;                  /* addr: 0x00027 */ 
} AHB_t;

  

/*
 * cpd
 */
  



#define CPD_DRAM_CNT_TRG                         0x02000
#define CPD_DRAM_CNT_TRG_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 traiing_st_trig      :  1 ; /* bits 1:1 */
    cs_uint32 init_sdram_req       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 init_sdram_req       :  1 ; /* bits 0:0 */
    cs_uint32 traiing_st_trig      :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} CPD_DRAM_CNT_TRG_t;





#define CPD_DRAM_CONTROL0                        0x02001
#define CPD_DRAM_CONTROL0_dft                    0x1c842126




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 data_en_one_clk_dly  :  1 ; /* bits 31:31 */
    cs_uint32 memory_bank_cnt      :  1 ; /* bits 30:30 */
    cs_uint32 memory_bank_sz       :  2 ; /* bits 29:28 */
    cs_uint32 memory_page_size     :  2 ; /* bits 27:26 */
    cs_uint32 MemtRPCtl            :  3 ; /* bits 25:23 */
    cs_uint32 MemtRCDCtl           :  3 ; /* bits 22:20 */
    cs_uint32 MemtRCCtl            :  4 ; /* bits 19:16 */
    cs_uint32 MemDqsEn             :  1 ; /* bits 15:15 */
    cs_uint32 cpd_ff_sw_rst        :  1 ; /* bits 14:14 */
    cs_uint32 ODFRttSel            :  2 ; /* bits 13:12 */
    cs_uint32 AddiyLat             :  3 ; /* bits 11:9 */
    cs_uint32 ddr_drv_strength     :  1 ; /* bits 8:8 */
    cs_uint32 ddr_dll_disable      :  1 ; /* bits 7:7 */
    cs_uint32 WRecoveryLat         :  3 ; /* bits 6:4 */
    cs_uint32 sd_cat_lat           :  3 ; /* bits 3:1 */
    cs_uint32 BurstType            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 BurstType            :  1 ; /* bits 0:0 */
    cs_uint32 sd_cat_lat           :  3 ; /* bits 3:1 */
    cs_uint32 WRecoveryLat         :  3 ; /* bits 6:4 */
    cs_uint32 ddr_dll_disable      :  1 ; /* bits 7:7 */
    cs_uint32 ddr_drv_strength     :  1 ; /* bits 8:8 */
    cs_uint32 AddiyLat             :  3 ; /* bits 11:9 */
    cs_uint32 ODFRttSel            :  2 ; /* bits 13:12 */
    cs_uint32 cpd_ff_sw_rst        :  1 ; /* bits 14:14 */
    cs_uint32 MemDqsEn             :  1 ; /* bits 15:15 */
    cs_uint32 MemtRCCtl            :  4 ; /* bits 19:16 */
    cs_uint32 MemtRCDCtl           :  3 ; /* bits 22:20 */
    cs_uint32 MemtRPCtl            :  3 ; /* bits 25:23 */
    cs_uint32 memory_page_size     :  2 ; /* bits 27:26 */
    cs_uint32 memory_bank_sz       :  2 ; /* bits 29:28 */
    cs_uint32 memory_bank_cnt      :  1 ; /* bits 30:30 */
    cs_uint32 data_en_one_clk_dly  :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} CPD_DRAM_CONTROL0_t;





#define CPD_DRAM_CONTROL1                        0x02002
#define CPD_DRAM_CONTROL1_dft                    0x80060004




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 sdram_use_manual_page :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd3               : 11 ;
    cs_uint32 rtn_dat_sel          :  3 ; /* bits 19:17 */
    cs_uint32 rsrvd2               :  8 ;
    cs_uint32 debug_pat_type       :  2 ; /* bits 8:7 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 dba0_sche_en         :  1 ; /* bits 3:3 */
    cs_uint32 op_to_dv_sel         :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 op_to_dv_sel         :  3 ; /* bits 2:0 */
    cs_uint32 dba0_sche_en         :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 debug_pat_type       :  2 ; /* bits 8:7 */
    cs_uint32 rsrvd2               :  8 ;
    cs_uint32 rtn_dat_sel          :  3 ; /* bits 19:17 */
    cs_uint32 rsrvd3               : 11 ;
    cs_uint32 sdram_use_manual_page :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} CPD_DRAM_CONTROL1_t;





#define CPD_DRAM_REFRESH_CONTROL                 0x02003
#define CPD_DRAM_REFRESH_CONTROL_dft             0xa000079c




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 auto_refresh_enable  :  1 ; /* bits 31:31 */
    cs_uint32 refresh_limit        : 15 ; /* bits 30:16 */
    cs_uint32 refresh_cnt          : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 refresh_cnt          : 16 ; /* bits 15:0 */
    cs_uint32 refresh_limit        : 15 ; /* bits 30:16 */
    cs_uint32 auto_refresh_enable  :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} CPD_DRAM_REFRESH_CONTROL_t;





#define CPD_DRAM_PERFORMANCE_CTL                 0x02004
#define CPD_DRAM_PERFORMANCE_CTL_dft             0x08802000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 MemtRRDCtl           :  2 ; /* bits 31:30 */
    cs_uint32 MemtRFCCtl           :  7 ; /* bits 29:23 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 read_op_dly_prg      :  2 ; /* bits 19:18 */
    cs_uint32 write_op_dly_prg     :  2 ; /* bits 17:16 */
    cs_uint32 MemtWTPCtl           :  3 ; /* bits 15:13 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 db_start_adr         : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 db_start_adr         : 12 ; /* bits 11:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 MemtWTPCtl           :  3 ; /* bits 15:13 */
    cs_uint32 write_op_dly_prg     :  2 ; /* bits 17:16 */
    cs_uint32 read_op_dly_prg      :  2 ; /* bits 19:18 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 MemtRFCCtl           :  7 ; /* bits 29:23 */
    cs_uint32 MemtRRDCtl           :  2 ; /* bits 31:30 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} CPD_DRAM_PERFORMANCE_CTL_t;





#define CPD_ARB_ERR_CLR                          0x02005
#define CPD_ARB_ERR_CLR_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 pre_charge_timeout_clr :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pre_charge_timeout_clr :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} CPD_ARB_ERR_CLR_t;





#define CPD_ARBITER_ERR                          0x02006
#define CPD_ARBITER_ERR_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 27 ;
    cs_uint32 training_complete_lat :  1 ; /* bits 4:4 */
    cs_uint32 training_complete    :  1 ; /* bits 3:3 */
    cs_uint32 training_bist_pass   :  2 ; /* bits 2:1 */
    cs_uint32 pre_charge_timeout   :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pre_charge_timeout   :  1 ; /* bits 0:0 */
    cs_uint32 training_bist_pass   :  2 ; /* bits 2:1 */
    cs_uint32 training_complete    :  1 ; /* bits 3:3 */
    cs_uint32 training_complete_lat :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               : 27 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} CPD_ARBITER_ERR_t;





#define CPD_ARB_CONTROL0                         0x02007
#define CPD_ARB_CONTROL0_dft                     0x08081020




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 db0_d                :  8 ; /* bits 31:24 */
    cs_uint32 db0_i                :  8 ; /* bits 23:16 */
    cs_uint32 cp0_d                :  8 ; /* bits 15:8 */
    cs_uint32 cp0_i                :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cp0_i                :  8 ; /* bits 7:0 */
    cs_uint32 cp0_d                :  8 ; /* bits 15:8 */
    cs_uint32 db0_i                :  8 ; /* bits 23:16 */
    cs_uint32 db0_d                :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} CPD_ARB_CONTROL0_t;





#define CPD_ARB_CONTROL1                         0x02008
#define CPD_ARB_CONTROL1_dft                     0x00001010




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 sbst                 :  8 ; /* bits 15:8 */
    cs_uint32 db1_d                :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 db1_d                :  8 ; /* bits 7:0 */
    cs_uint32 sbst                 :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} CPD_ARB_CONTROL1_t;





#define CPD_DEBUG_ADDR_LMT                       0x02009
#define CPD_DEBUG_ADDR_LMT_dft                   0x00000020




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 debug_upper_lmt      : 26 ; /* bits 25:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 debug_upper_lmt      : 26 ; /* bits 25:0 */
    cs_uint32 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} CPD_DEBUG_ADDR_LMT_t;





#define CPD_DEBUG_CTRL                           0x0200a
#define CPD_DEBUG_CTRL_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 debug_start_addr     : 26 ; /* bits 25:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 debug_start_addr     : 26 ; /* bits 25:0 */
    cs_uint32 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} CPD_DEBUG_CTRL_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* cpd */

typedef struct {
  CPD_DRAM_CNT_TRG_t                       DRAM_CNT_TRG;                  /* addr: 0x00000 */ 
  CPD_DRAM_CONTROL0_t                      DRAM_CONTROL0;                 /* addr: 0x00001 */ 
  CPD_DRAM_CONTROL1_t                      DRAM_CONTROL1;                 /* addr: 0x00002 */ 
  CPD_DRAM_REFRESH_CONTROL_t               DRAM_REFRESH_CONTROL;          /* addr: 0x00003 */ 
  CPD_DRAM_PERFORMANCE_CTL_t               DRAM_PERFORMANCE_CTL;          /* addr: 0x00004 */ 
  CPD_ARB_ERR_CLR_t                        ARB_ERR_CLR;                   /* addr: 0x00005 */ 
  CPD_ARBITER_ERR_t                        ARBITER_ERR;                   /* addr: 0x00006 */ 
  CPD_ARB_CONTROL0_t                       ARB_CONTROL0;                  /* addr: 0x00007 */ 
  CPD_ARB_CONTROL1_t                       ARB_CONTROL1;                  /* addr: 0x00008 */ 
  CPD_DEBUG_ADDR_LMT_t                     DEBUG_ADDR_LMT;                /* addr: 0x00009 */ 
  CPD_DEBUG_CTRL_t                         DEBUG_CTRL;                    /* addr: 0x0000a */ 
} CPD_t;

  

/*
 * glb
 */
  



#define GLOBAL_JTAG_ID                           0x03000
#define GLOBAL_JTAG_ID_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 jtag_id              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 jtag_id              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_JTAG_ID_t;





#define GLOBAL_ONU_MAC_ADDR1                     0x03001
#define GLOBAL_ONU_MAC_ADDR1_dft                 0x00000001




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 sa                   : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sa                   : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_ONU_MAC_ADDR1_t;





#define GLOBAL_ONU_MAC_ADDR0                     0x03002
#define GLOBAL_ONU_MAC_ADDR0_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 sa                   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sa                   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_ONU_MAC_ADDR0_t;





#define GLOBAL_CTRL                              0x03003
#define GLOBAL_CTRL_dft                          0x00018001




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 10 ;
    cs_uint32 pdown_gpio6_sel      :  1 ; /* bits 21:21 */
    cs_uint32 ge_clk_sel           :  1 ; /* bits 20:20 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 bm_sram_cfg          :  2 ; /* bits 17:16 */
    cs_uint32 cpu_sel_bm           :  1 ; /* bits 15:15 */
    cs_uint32 led_gpio_sel         :  6 ; /* bits 14:9 */
    cs_uint32 ptp_en               :  1 ; /* bits 8:8 */
    cs_uint32 ptp_clk_sel          :  1 ; /* bits 7:7 */
    cs_uint32 fwd_mac_error_pkt    :  1 ; /* bits 6:6 */
    cs_uint32 mir_enable           :  1 ; /* bits 5:5 */
    cs_uint32 ge_lcl_lpbk          :  1 ; /* bits 4:4 */
    cs_uint32 ge_rmte_lpbk         :  1 ; /* bits 3:3 */
    cs_uint32 rd_clr               :  1 ; /* bits 2:2 */
    cs_uint32 ma_hdr_mode          :  1 ; /* bits 1:1 */
    cs_uint32 legacy_mode          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 legacy_mode          :  1 ; /* bits 0:0 */
    cs_uint32 ma_hdr_mode          :  1 ; /* bits 1:1 */
    cs_uint32 rd_clr               :  1 ; /* bits 2:2 */
    cs_uint32 ge_rmte_lpbk         :  1 ; /* bits 3:3 */
    cs_uint32 ge_lcl_lpbk          :  1 ; /* bits 4:4 */
    cs_uint32 mir_enable           :  1 ; /* bits 5:5 */
    cs_uint32 fwd_mac_error_pkt    :  1 ; /* bits 6:6 */
    cs_uint32 ptp_clk_sel          :  1 ; /* bits 7:7 */
    cs_uint32 ptp_en               :  1 ; /* bits 8:8 */
    cs_uint32 led_gpio_sel         :  6 ; /* bits 14:9 */
    cs_uint32 cpu_sel_bm           :  1 ; /* bits 15:15 */
    cs_uint32 bm_sram_cfg          :  2 ; /* bits 17:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ge_clk_sel           :  1 ; /* bits 20:20 */
    cs_uint32 pdown_gpio6_sel      :  1 ; /* bits 21:21 */
    cs_uint32 rsrvd2               : 10 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_CTRL_t;





#define GLOBAL_INIT_STS                          0x03004
#define GLOBAL_INIT_STS_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 te_init_done         :  1 ; /* bits 2:2 */
    cs_uint32 cpll_lock            :  1 ; /* bits 1:1 */
    cs_uint32 bm_init_done         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 bm_init_done         :  1 ; /* bits 0:0 */
    cs_uint32 cpll_lock            :  1 ; /* bits 1:1 */
    cs_uint32 te_init_done         :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_INIT_STS_t;





#define GLOBAL_RST_CTRL                          0x03005
#define GLOBAL_RST_CTRL_dft                      0x0003f7ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 14 ;
    cs_uint32 pcm_rst_n            :  1 ; /* bits 17:17 */
    cs_uint32 arm_rst_n            :  1 ; /* bits 16:16 */
    cs_uint32 arm_peri_rst_n       :  1 ; /* bits 15:15 */
    cs_uint32 sdm_rst_n            :  1 ; /* bits 14:14 */
    cs_uint32 cpll_rst_n           :  1 ; /* bits 13:13 */
    cs_uint32 te_rst_n             :  1 ; /* bits 12:12 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 mii0_rst_n           :  1 ; /* bits 10:10 */
    cs_uint32 ma_rst_n             :  1 ; /* bits 9:9 */
    cs_uint32 fe_rst_n             :  1 ; /* bits 8:8 */
    cs_uint32 bm_rst_n             :  1 ; /* bits 7:7 */
    cs_uint32 ge_tx_rst_n          :  1 ; /* bits 6:6 */
    cs_uint32 ge_rx_rst_n          :  1 ; /* bits 5:5 */
    cs_uint32 ge_rst_n             :  1 ; /* bits 4:4 */
    cs_uint32 sds_rst_n            :  1 ; /* bits 3:3 */
    cs_uint32 pon_tx_rst_n         :  1 ; /* bits 2:2 */
    cs_uint32 pon_rx_rst_n         :  1 ; /* bits 1:1 */
    cs_uint32 pon_rst_n            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pon_rst_n            :  1 ; /* bits 0:0 */
    cs_uint32 pon_rx_rst_n         :  1 ; /* bits 1:1 */
    cs_uint32 pon_tx_rst_n         :  1 ; /* bits 2:2 */
    cs_uint32 sds_rst_n            :  1 ; /* bits 3:3 */
    cs_uint32 ge_rst_n             :  1 ; /* bits 4:4 */
    cs_uint32 ge_rx_rst_n          :  1 ; /* bits 5:5 */
    cs_uint32 ge_tx_rst_n          :  1 ; /* bits 6:6 */
    cs_uint32 bm_rst_n             :  1 ; /* bits 7:7 */
    cs_uint32 fe_rst_n             :  1 ; /* bits 8:8 */
    cs_uint32 ma_rst_n             :  1 ; /* bits 9:9 */
    cs_uint32 mii0_rst_n           :  1 ; /* bits 10:10 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 te_rst_n             :  1 ; /* bits 12:12 */
    cs_uint32 cpll_rst_n           :  1 ; /* bits 13:13 */
    cs_uint32 sdm_rst_n            :  1 ; /* bits 14:14 */
    cs_uint32 arm_peri_rst_n       :  1 ; /* bits 15:15 */
    cs_uint32 arm_rst_n            :  1 ; /* bits 16:16 */
    cs_uint32 pcm_rst_n            :  1 ; /* bits 17:17 */
    cs_uint32 rsrvd2               : 14 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_RST_CTRL_t;





#define GLOBAL_SW_CHIP_RST                       0x03006
#define GLOBAL_SW_CHIP_RST_dft                   0x00000001




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 ASIC_rst_n           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ASIC_rst_n           :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_SW_CHIP_RST_t;





#define GLOBAL_PIN_CTRL                          0x03007
#define GLOBAL_PIN_CTRL_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 override_mode        :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd3               :  4 ;
    cs_uint32 glb_share_pin_gp4    :  1 ; /* bits 26:26 */
    cs_uint32 glb_share_pin_gp3    :  1 ; /* bits 25:25 */
    cs_uint32 glb_share_pin_gp2    :  1 ; /* bits 24:24 */
    cs_uint32 glb_share_pin_gp1    :  1 ; /* bits 23:23 */
    cs_uint32 glb_ptp_1pps_seln    :  1 ; /* bits 22:22 */
    cs_uint32 cp_f_sd_en           :  1 ; /* bits 21:21 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 mii0_if_mode         :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 int_clk_sel          :  2 ; /* bits 10:9 */
    cs_uint32 xcpu_sel             :  1 ; /* bits 8:8 */
    cs_uint32 mem_addr_sel         :  2 ; /* bits 7:6 */
    cs_uint32 image_download_mode  :  2 ; /* bits 5:4 */
    cs_uint32 ge_if_mode           :  1 ; /* bits 3:3 */
    cs_uint32 pon_if_mode          :  1 ; /* bits 2:2 */
    cs_uint32 boot_mode            :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 boot_mode            :  2 ; /* bits 1:0 */
    cs_uint32 pon_if_mode          :  1 ; /* bits 2:2 */
    cs_uint32 ge_if_mode           :  1 ; /* bits 3:3 */
    cs_uint32 image_download_mode  :  2 ; /* bits 5:4 */
    cs_uint32 mem_addr_sel         :  2 ; /* bits 7:6 */
    cs_uint32 xcpu_sel             :  1 ; /* bits 8:8 */
    cs_uint32 int_clk_sel          :  2 ; /* bits 10:9 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 mii0_if_mode         :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 cp_f_sd_en           :  1 ; /* bits 21:21 */
    cs_uint32 glb_ptp_1pps_seln    :  1 ; /* bits 22:22 */
    cs_uint32 glb_share_pin_gp1    :  1 ; /* bits 23:23 */
    cs_uint32 glb_share_pin_gp2    :  1 ; /* bits 24:24 */
    cs_uint32 glb_share_pin_gp3    :  1 ; /* bits 25:25 */
    cs_uint32 glb_share_pin_gp4    :  1 ; /* bits 26:26 */
    cs_uint32 rsrvd3               :  4 ;
    cs_uint32 override_mode        :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_PIN_CTRL_t;





#define GLOBAL_PIN_STS                           0x03008
#define GLOBAL_PIN_STS_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd4               : 11 ;
    cs_uint32 ext_clk_sel          :  1 ; /* bits 20:20 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 sdm_sram_share       :  1 ; /* bits 18:18 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 mii0_if_mode         :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 int_clk_sel          :  2 ; /* bits 10:9 */
    cs_uint32 xcpu_sel             :  1 ; /* bits 8:8 */
    cs_uint32 mem_addr_sel         :  2 ; /* bits 7:6 */
    cs_uint32 image_download_mode  :  2 ; /* bits 5:4 */
    cs_uint32 ge_if_mode           :  1 ; /* bits 3:3 */
    cs_uint32 pon_if_mode          :  1 ; /* bits 2:2 */
    cs_uint32 boot_mode            :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 boot_mode            :  2 ; /* bits 1:0 */
    cs_uint32 pon_if_mode          :  1 ; /* bits 2:2 */
    cs_uint32 ge_if_mode           :  1 ; /* bits 3:3 */
    cs_uint32 image_download_mode  :  2 ; /* bits 5:4 */
    cs_uint32 mem_addr_sel         :  2 ; /* bits 7:6 */
    cs_uint32 xcpu_sel             :  1 ; /* bits 8:8 */
    cs_uint32 int_clk_sel          :  2 ; /* bits 10:9 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 mii0_if_mode         :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 sdm_sram_share       :  1 ; /* bits 18:18 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 ext_clk_sel          :  1 ; /* bits 20:20 */
    cs_uint32 rsrvd4               : 11 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_PIN_STS_t;





#define GLOBAL_SYNC_CTRL                         0x03009
#define GLOBAL_SYNC_CTRL_dft                     0x00083d09




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 12 ;
    cs_uint32 en                   :  1 ; /* bits 19:19 */
    cs_uint32 sel                  :  1 ; /* bits 18:18 */
    cs_uint32 dir_cfg              :  2 ; /* bits 17:16 */
    cs_uint32 div                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 div                  : 16 ; /* bits 15:0 */
    cs_uint32 dir_cfg              :  2 ; /* bits 17:16 */
    cs_uint32 sel                  :  1 ; /* bits 18:18 */
    cs_uint32 en                   :  1 ; /* bits 19:19 */
    cs_uint32 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_SYNC_CTRL_t;





#define GLOBAL_DBG_CTRL                          0x0300a
#define GLOBAL_DBG_CTRL_dft                      0x000003c0




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 dbg_sel              : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dbg_sel              : 10 ; /* bits 9:0 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_DBG_CTRL_t;





#define GLOBAL_DBGOE_CTRL1                       0x0300b
#define GLOBAL_DBGOE_CTRL1_dft                   0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 dbg_oen              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dbg_oen              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_DBGOE_CTRL1_t;





#define GLOBAL_DBGOE_CTRL0                       0x0300c
#define GLOBAL_DBGOE_CTRL0_dft                   0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 dbg_oen              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dbg_oen              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_DBGOE_CTRL0_t;





#define GLOBAL_DBG_DATA1                         0x0300d
#define GLOBAL_DBG_DATA1_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 dbg_bus              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dbg_bus              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_DBG_DATA1_t;





#define GLOBAL_DBG_DATA0                         0x0300e
#define GLOBAL_DBG_DATA0_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 dbg_bus              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dbg_bus              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_DBG_DATA0_t;





#define GLOBAL_ONU_CTRL                          0x0300f
#define GLOBAL_ONU_CTRL_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 tck_sel_los          :  1 ; /* bits 2:2 */
    cs_uint32 tck_inv              :  1 ; /* bits 1:1 */
    cs_uint32 tck_sel              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tck_sel              :  1 ; /* bits 0:0 */
    cs_uint32 tck_inv              :  1 ; /* bits 1:1 */
    cs_uint32 tck_sel_los          :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_ONU_CTRL_t;





#define GLOBAL_KM_SDS_CTRL                       0x03010
#define GLOBAL_KM_SDS_CTRL_dft                   0x00000012




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 27 ;
    cs_uint32 rstz_fifo            :  1 ; /* bits 4:4 */
    cs_uint32 plben_10b            :  1 ; /* bits 3:3 */
    cs_uint32 en_cdet              :  1 ; /* bits 2:2 */
    cs_uint32 rstz_prbs            :  1 ; /* bits 1:1 */
    cs_uint32 clr_pass_prbs        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 clr_pass_prbs        :  1 ; /* bits 0:0 */
    cs_uint32 rstz_prbs            :  1 ; /* bits 1:1 */
    cs_uint32 en_cdet              :  1 ; /* bits 2:2 */
    cs_uint32 plben_10b            :  1 ; /* bits 3:3 */
    cs_uint32 rstz_fifo            :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               : 27 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_KM_SDS_CTRL_t;





#define GLOBAL_KM_SDS_STS                        0x03011
#define GLOBAL_KM_SDS_STS_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 16 ;
    cs_uint32 serdes_status        :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 pll_lock             :  1 ; /* bits 4:4 */
    cs_uint32 com_det              :  1 ; /* bits 3:3 */
    cs_uint32 pass_rx_prbs         :  1 ; /* bits 2:2 */
    cs_uint32 sync_err_8to10       :  1 ; /* bits 1:1 */
    cs_uint32 sync_err_10to8       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sync_err_10to8       :  1 ; /* bits 0:0 */
    cs_uint32 sync_err_8to10       :  1 ; /* bits 1:1 */
    cs_uint32 pass_rx_prbs         :  1 ; /* bits 2:2 */
    cs_uint32 com_det              :  1 ; /* bits 3:3 */
    cs_uint32 pll_lock             :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 serdes_status        :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd2               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_KM_SDS_STS_t;





#define GLOBAL_KM_PLL_CTRL                       0x03012
#define GLOBAL_KM_PLL_CTRL_dft                   0x01224f00




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd4               :  6 ;
    cs_uint32 bypass               :  1 ; /* bits 25:25 */
    cs_uint32 fse                  :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 range                :  3 ; /* bits 22:20 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 divq                 :  3 ; /* bits 18:16 */
    cs_uint32 divf                 :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 divr                 :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 divr                 :  5 ; /* bits 4:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 divf                 :  8 ; /* bits 15:8 */
    cs_uint32 divq                 :  3 ; /* bits 18:16 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 range                :  3 ; /* bits 22:20 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 fse                  :  1 ; /* bits 24:24 */
    cs_uint32 bypass               :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd4               :  6 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_KM_PLL_CTRL_t;





#define GLOBAL_PTP_LOCAL_TIMER_REF               0x03013
#define GLOBAL_PTP_LOCAL_TIMER_REF_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 val                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 val                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_PTP_LOCAL_TIMER_REF_t;





#define GLOBAL_PTP_GET_TIMESTAMP                 0x03014
#define GLOBAL_PTP_GET_TIMESTAMP_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 val                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 val                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_PTP_GET_TIMESTAMP_t;





#define GLOBAL_PTP_GET_MPCP_TIMESTAMP            0x03015
#define GLOBAL_PTP_GET_MPCP_TIMESTAMP_dft        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 val                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 val                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_PTP_GET_MPCP_TIMESTAMP_t;





#define GLOBAL_PTP_GET_TIMESTAMP_CMD             0x03016
#define GLOBAL_PTP_GET_TIMESTAMP_CMD_dft         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 done                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 done                 :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_PTP_GET_TIMESTAMP_CMD_t;





#define GLOBAL_PTP_SET_TIMESTAMP                 0x03017
#define GLOBAL_PTP_SET_TIMESTAMP_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 val                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 val                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_PTP_SET_TIMESTAMP_t;





#define GLOBAL_PTP_SET_TIMESTAMP_CMD             0x03018
#define GLOBAL_PTP_SET_TIMESTAMP_CMD_dft         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 set                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 set                  :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_PTP_SET_TIMESTAMP_CMD_t;





#define GLOBAL_PTP_INTERRUPT                     0x03019
#define GLOBAL_PTP_INTERRUPT_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 set_ts_intI          :  1 ; /* bits 1:1 */
    cs_uint32 get_ts_intI          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 get_ts_intI          :  1 ; /* bits 0:0 */
    cs_uint32 set_ts_intI          :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_PTP_INTERRUPT_t;





#define GLOBAL_PTP_INTENABLE                     0x0301a
#define GLOBAL_PTP_INTENABLE_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 set_ts_intE          :  1 ; /* bits 1:1 */
    cs_uint32 get_ts_intE          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 get_ts_intE          :  1 ; /* bits 0:0 */
    cs_uint32 set_ts_intE          :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GLOBAL_PTP_INTENABLE_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* glb */

typedef struct {
  GLOBAL_JTAG_ID_t                         JTAG_ID;                       /* addr: 0x00000 */ 
  GLOBAL_ONU_MAC_ADDR1_t                   ONU_MAC_ADDR1;                 /* addr: 0x00001 */ 
  GLOBAL_ONU_MAC_ADDR0_t                   ONU_MAC_ADDR0;                 /* addr: 0x00002 */ 
  GLOBAL_CTRL_t                            CTRL;                          /* addr: 0x00003 */ 
  GLOBAL_INIT_STS_t                        INIT_STS;                      /* addr: 0x00004 */ 
  GLOBAL_RST_CTRL_t                        RST_CTRL;                      /* addr: 0x00005 */ 
  GLOBAL_SW_CHIP_RST_t                     SW_CHIP_RST;                   /* addr: 0x00006 */ 
  GLOBAL_PIN_CTRL_t                        PIN_CTRL;                      /* addr: 0x00007 */ 
  GLOBAL_PIN_STS_t                         PIN_STS;                       /* addr: 0x00008 */ 
  GLOBAL_SYNC_CTRL_t                       SYNC_CTRL;                     /* addr: 0x00009 */ 
  GLOBAL_DBG_CTRL_t                        DBG_CTRL;                      /* addr: 0x0000a */ 
  GLOBAL_DBGOE_CTRL1_t                     DBGOE_CTRL1;                   /* addr: 0x0000b */ 
  GLOBAL_DBGOE_CTRL0_t                     DBGOE_CTRL0;                   /* addr: 0x0000c */ 
  GLOBAL_DBG_DATA1_t                       DBG_DATA1;                     /* addr: 0x0000d */ 
  GLOBAL_DBG_DATA0_t                       DBG_DATA0;                     /* addr: 0x0000e */ 
  GLOBAL_ONU_CTRL_t                        ONU_CTRL;                      /* addr: 0x0000f */ 
  GLOBAL_KM_SDS_CTRL_t                     KM_SDS_CTRL;                   /* addr: 0x00010 */ 
  GLOBAL_KM_SDS_STS_t                      KM_SDS_STS;                    /* addr: 0x00011 */ 
  GLOBAL_KM_PLL_CTRL_t                     KM_PLL_CTRL;                   /* addr: 0x00012 */ 
  GLOBAL_PTP_LOCAL_TIMER_REF_t             PTP_LOCAL_TIMER_REF;           /* addr: 0x00013 */ 
  GLOBAL_PTP_GET_TIMESTAMP_t               PTP_GET_TIMESTAMP;             /* addr: 0x00014 */ 
  GLOBAL_PTP_GET_MPCP_TIMESTAMP_t          PTP_GET_MPCP_TIMESTAMP;        /* addr: 0x00015 */ 
  GLOBAL_PTP_GET_TIMESTAMP_CMD_t           PTP_GET_TIMESTAMP_CMD;         /* addr: 0x00016 */ 
  GLOBAL_PTP_SET_TIMESTAMP_t               PTP_SET_TIMESTAMP;             /* addr: 0x00017 */ 
  GLOBAL_PTP_SET_TIMESTAMP_CMD_t           PTP_SET_TIMESTAMP_CMD;         /* addr: 0x00018 */ 
  GLOBAL_PTP_INTERRUPT_t                   PTP_INTERRUPT;                 /* addr: 0x00019 */ 
  GLOBAL_PTP_INTENABLE_t                   PTP_INTENABLE;                 /* addr: 0x0001a */ 
} GLB_t;

  

/*
 * led
 */
  



#define LED_CTRL                                 0x04000
#define LED_CTRL_dft                             0x00000002




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 10 ;
    cs_uint32 off_val              :  6 ; /* bits 21:16 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 on_val               :  6 ; /* bits 13:8 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 mode                 :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mode                 :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 on_val               :  6 ; /* bits 13:8 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 off_val              :  6 ; /* bits 21:16 */
    cs_uint32 rsrvd3               : 10 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} LED_CTRL_t;





#define LED_CFG_0                                0x04001
#define LED_CFG_0_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 off_tim_ld           :  8 ; /* bits 15:8 */
    cs_uint32 on_tim_ld            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 on_tim_ld            :  8 ; /* bits 7:0 */
    cs_uint32 off_tim_ld           :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} LED_CFG_0_t;





#define LED_CFG_1                                0x04002
#define LED_CFG_1_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 off_tim_ld           :  8 ; /* bits 15:8 */
    cs_uint32 on_tim_ld            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 on_tim_ld            :  8 ; /* bits 7:0 */
    cs_uint32 off_tim_ld           :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} LED_CFG_1_t;





#define LED_CFG_2                                0x04003
#define LED_CFG_2_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 off_tim_ld           :  8 ; /* bits 15:8 */
    cs_uint32 on_tim_ld            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 on_tim_ld            :  8 ; /* bits 7:0 */
    cs_uint32 off_tim_ld           :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} LED_CFG_2_t;





#define LED_CFG_3                                0x04004
#define LED_CFG_3_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 off_tim_ld           :  8 ; /* bits 15:8 */
    cs_uint32 on_tim_ld            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 on_tim_ld            :  8 ; /* bits 7:0 */
    cs_uint32 off_tim_ld           :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} LED_CFG_3_t;





#define LED_CFG_4                                0x04005
#define LED_CFG_4_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 off_tim_ld           :  8 ; /* bits 15:8 */
    cs_uint32 on_tim_ld            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 on_tim_ld            :  8 ; /* bits 7:0 */
    cs_uint32 off_tim_ld           :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} LED_CFG_4_t;





#define LED_CFG_5                                0x04006
#define LED_CFG_5_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 off_tim_ld           :  8 ; /* bits 15:8 */
    cs_uint32 on_tim_ld            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 on_tim_ld            :  8 ; /* bits 7:0 */
    cs_uint32 off_tim_ld           :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} LED_CFG_5_t;





#define LED_LOAD                                 0x04007
#define LED_LOAD_dft                             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 26 ;
    cs_uint32 enb                  :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 enb                  :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 26 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} LED_LOAD_t;





#define LED_CUSTOM_C_CFG                         0x04008
#define LED_CUSTOM_C_CFG_dft                     0x000061a8




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cst_en               :  1 ; /* bits 31:31 */
    cs_uint32 cst_on_val           :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 11 ;
    cs_uint32 pin_sel              :  3 ; /* bits 18:16 */
    cs_uint32 cst_cnt              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cst_cnt              : 16 ; /* bits 15:0 */
    cs_uint32 pin_sel              :  3 ; /* bits 18:16 */
    cs_uint32 rsrvd1               : 11 ;
    cs_uint32 cst_on_val           :  1 ; /* bits 30:30 */
    cs_uint32 cst_en               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} LED_CUSTOM_C_CFG_t;





#define LED_DEBUG                                0x04009
#define LED_DEBUG_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 fast_clk             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 fast_clk             :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} LED_DEBUG_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* led */

typedef struct {
  LED_CTRL_t                               CTRL;                          /* addr: 0x00000 */ 
  LED_CFG_0_t                              CFG_0;                         /* addr: 0x00001 */ 
  LED_CFG_1_t                              CFG_1;                         /* addr: 0x00002 */ 
  LED_CFG_2_t                              CFG_2;                         /* addr: 0x00003 */ 
  LED_CFG_3_t                              CFG_3;                         /* addr: 0x00004 */ 
  LED_CFG_4_t                              CFG_4;                         /* addr: 0x00005 */ 
  LED_CFG_5_t                              CFG_5;                         /* addr: 0x00006 */ 
  LED_LOAD_t                               LOAD;                          /* addr: 0x00007 */ 
  LED_CUSTOM_C_CFG_t                       CUSTOM_C_CFG;                  /* addr: 0x00008 */ 
  LED_DEBUG_t                              DEBUG;                         /* addr: 0x00009 */ 
} LED_t;

  

/*
 * ma
 */
  



#define MA_MII0_TX_CTRL                          0x05000
#define MA_MII0_TX_CTRL_dft                      0x00000005




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 24 ;
    cs_uint32 ipg_sel              :  2 ; /* bits 7:6 */
    cs_uint32 crc_bypass           :  1 ; /* bits 5:5 */
    cs_uint32 rmt_lpbk             :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 hd_speed             :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 tx_en                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_en                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 hd_speed             :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 rmt_lpbk             :  1 ; /* bits 4:4 */
    cs_uint32 crc_bypass           :  1 ; /* bits 5:5 */
    cs_uint32 ipg_sel              :  2 ; /* bits 7:6 */
    cs_uint32 rsrvd3               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MII0_TX_CTRL_t;





#define MA_MII0_TX_CFG                           0x05001
#define MA_MII0_TX_CFG_dft                       0x01000090




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               :  7 ;
    cs_uint32 smii_phy_mode        :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd2               : 16 ;
    cs_uint32 txdat_en             :  1 ; /* bits 7:7 */
    cs_uint32 flow_off_req         :  1 ; /* bits 6:6 */
    cs_uint32 flow_on_req          :  1 ; /* bits 5:5 */
    cs_uint32 tx_flow_en           :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               :  4 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 tx_flow_en           :  1 ; /* bits 4:4 */
    cs_uint32 flow_on_req          :  1 ; /* bits 5:5 */
    cs_uint32 flow_off_req         :  1 ; /* bits 6:6 */
    cs_uint32 txdat_en             :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd2               : 16 ;
    cs_uint32 smii_phy_mode        :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd3               :  7 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MII0_TX_CFG_t;





#define MA_MII0_FLOW_SA0                         0x05002
#define MA_MII0_FLOW_SA0_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MII0_FLOW_SA0_t;





#define MA_MII0_FLOW_SA1                         0x05003
#define MA_MII0_FLOW_SA1_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 slot                 : 16 ; /* bits 31:16 */
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
    cs_uint32 slot                 : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MII0_FLOW_SA1_t;





#define MA_MII0_TX_STAT0                         0x05004
#define MA_MII0_TX_STAT0_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 fifo_ur              :  1 ; /* bits 27:27 */
    cs_uint32 pkt_err              :  1 ; /* bits 26:26 */
    cs_uint32 ctrl_pkt             :  1 ; /* bits 25:25 */
    cs_uint32 mc_pkt               :  1 ; /* bits 24:24 */
    cs_uint32 flow_gnt             :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 byte_cnt             : 22 ; /* bits 21:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 byte_cnt             : 22 ; /* bits 21:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 flow_gnt             :  1 ; /* bits 23:23 */
    cs_uint32 mc_pkt               :  1 ; /* bits 24:24 */
    cs_uint32 ctrl_pkt             :  1 ; /* bits 25:25 */
    cs_uint32 pkt_err              :  1 ; /* bits 26:26 */
    cs_uint32 fifo_ur              :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd2               :  4 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MII0_TX_STAT0_t;





#define MA_MII0_TX_STAT1                         0x05005
#define MA_MII0_TX_STAT1_dft                     0x00100000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 11 ;
    cs_uint32 fifo_rdy             :  1 ; /* bits 20:20 */
    cs_uint32 tc_done              :  1 ; /* bits 19:19 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 pkt_cnt              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_cnt              : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 tc_done              :  1 ; /* bits 19:19 */
    cs_uint32 fifo_rdy             :  1 ; /* bits 20:20 */
    cs_uint32 rsrvd2               : 11 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MII0_TX_STAT1_t;





#define MA_MII0_RX_CTRL                          0x05006
#define MA_MII0_RX_CTRL_dft                      0x983080c0




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rxen                 :  1 ; /* bits 31:31 */
    cs_uint32 mtu_pcnt_en          :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 flow_ctrl            :  1 ; /* bits 28:28 */
    cs_uint32 ipg_set              :  1 ; /* bits 27:27 */
    cs_uint32 ipg_sel              :  3 ; /* bits 26:24 */
    cs_uint32 lcal_lpbk            :  1 ; /* bits 23:23 */
    cs_uint32 unknown_ctrl_drop    :  1 ; /* bits 22:22 */
    cs_uint32 unknown_op_drop_en   :  1 ; /* bits 21:21 */
    cs_uint32 ud_size_drop_en      :  1 ; /* bits 20:20 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 flow_off_th          :  8 ; /* bits 15:8 */
    cs_uint32 flow_on_th           :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 flow_on_th           :  8 ; /* bits 7:0 */
    cs_uint32 flow_off_th          :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 ud_size_drop_en      :  1 ; /* bits 20:20 */
    cs_uint32 unknown_op_drop_en   :  1 ; /* bits 21:21 */
    cs_uint32 unknown_ctrl_drop    :  1 ; /* bits 22:22 */
    cs_uint32 lcal_lpbk            :  1 ; /* bits 23:23 */
    cs_uint32 ipg_sel              :  3 ; /* bits 26:24 */
    cs_uint32 ipg_set              :  1 ; /* bits 27:27 */
    cs_uint32 flow_ctrl            :  1 ; /* bits 28:28 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 mtu_pcnt_en          :  1 ; /* bits 30:30 */
    cs_uint32 rxen                 :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MII0_RX_CTRL_t;





#define MA_MII0_RX_CTRL1                         0x05007
#define MA_MII0_RX_CTRL1_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 rmii_smii            :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rmii_smii            :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MII0_RX_CTRL1_t;





#define MA_MII0_RX_CNT                           0x05008
#define MA_MII0_RX_CNT_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 err_pkt              : 16 ; /* bits 31:16 */
    cs_uint32 pkt                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt                  : 16 ; /* bits 15:0 */
    cs_uint32 err_pkt              : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MII0_RX_CNT_t;





#define MA_MII0_RX_STAT                          0x05009
#define MA_MII0_RX_STAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 usr_drp_pkt          :  8 ; /* bits 31:24 */
    cs_uint32 smii                 :  4 ; /* bits 23:20 */
    cs_uint32 fifo_byte_cnt        : 13 ; /* bits 19:7 */
    cs_uint32 fifo_pkt_cnt         :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 fifo_pkt_cnt         :  7 ; /* bits 6:0 */
    cs_uint32 fifo_byte_cnt        : 13 ; /* bits 19:7 */
    cs_uint32 smii                 :  4 ; /* bits 23:20 */
    cs_uint32 usr_drp_pkt          :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MII0_RX_STAT_t;





#define MA_MII0_RX_STAT1                         0x0500a
#define MA_MII0_RX_STAT1_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 27 ;
    cs_uint32 pkt_oversize         :  1 ; /* bits 4:4 */
    cs_uint32 pkt_undersize        :  1 ; /* bits 3:3 */
    cs_uint32 pkt_offsize          :  1 ; /* bits 2:2 */
    cs_uint32 ff_overflow_drop     :  1 ; /* bits 1:1 */
    cs_uint32 pkt_drop             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_drop             :  1 ; /* bits 0:0 */
    cs_uint32 ff_overflow_drop     :  1 ; /* bits 1:1 */
    cs_uint32 pkt_offsize          :  1 ; /* bits 2:2 */
    cs_uint32 pkt_undersize        :  1 ; /* bits 3:3 */
    cs_uint32 pkt_oversize         :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               : 27 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MII0_RX_STAT1_t;





#define MA_INT_EN                                0x0500b
#define MA_INT_EN_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cpu_rx_buff1_rdy     :  1 ; /* bits 31:31 */
    cs_uint32 cpu_rx_buff0_rdy     :  1 ; /* bits 30:30 */
    cs_uint32 cpu_tx_buff_rdy      :  1 ; /* bits 29:29 */
    cs_uint32 rsrvd2               :  5 ;
    cs_uint32 mii0_rx_pkt_os       :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 auto_xfer_done       :  1 ; /* bits 12:12 */
    cs_uint32 dma1_tx_done_i       :  1 ; /* bits 11:11 */
    cs_uint32 dma0_tx_done_i       :  1 ; /* bits 10:10 */
    cs_uint32 mib_msb_int_set      :  1 ; /* bits 9:9 */
    cs_uint32 pkt_xfer_done        :  1 ; /* bits 8:8 */
    cs_uint32 pkt_xfer2mem_done    :  1 ; /* bits 7:7 */
    cs_uint32 mii0_rx_ff_ol        :  1 ; /* bits 6:6 */
    cs_uint32 mii0_tx_stop_done    :  1 ; /* bits 5:5 */
    cs_uint32 mii0_rx_ff_full      :  1 ; /* bits 4:4 */
    cs_uint32 mii0_tx_stop         :  1 ; /* bits 3:3 */
    cs_uint32 mii0_rx_pkt_drop     :  1 ; /* bits 2:2 */
    cs_uint32 mii0_tx_ff_full      :  1 ; /* bits 1:1 */
    cs_uint32 mii0_tx_pkt_done     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mii0_tx_pkt_done     :  1 ; /* bits 0:0 */
    cs_uint32 mii0_tx_ff_full      :  1 ; /* bits 1:1 */
    cs_uint32 mii0_rx_pkt_drop     :  1 ; /* bits 2:2 */
    cs_uint32 mii0_tx_stop         :  1 ; /* bits 3:3 */
    cs_uint32 mii0_rx_ff_full      :  1 ; /* bits 4:4 */
    cs_uint32 mii0_tx_stop_done    :  1 ; /* bits 5:5 */
    cs_uint32 mii0_rx_ff_ol        :  1 ; /* bits 6:6 */
    cs_uint32 pkt_xfer2mem_done    :  1 ; /* bits 7:7 */
    cs_uint32 pkt_xfer_done        :  1 ; /* bits 8:8 */
    cs_uint32 mib_msb_int_set      :  1 ; /* bits 9:9 */
    cs_uint32 dma0_tx_done_i       :  1 ; /* bits 10:10 */
    cs_uint32 dma1_tx_done_i       :  1 ; /* bits 11:11 */
    cs_uint32 auto_xfer_done       :  1 ; /* bits 12:12 */
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 mii0_rx_pkt_os       :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd2               :  5 ;
    cs_uint32 cpu_tx_buff_rdy      :  1 ; /* bits 29:29 */
    cs_uint32 cpu_rx_buff0_rdy     :  1 ; /* bits 30:30 */
    cs_uint32 cpu_rx_buff1_rdy     :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_INT_EN_t;





#define MA_INT                                   0x0500c
#define MA_INT_dft                               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cpu_rx_buff1_rdy     :  1 ; /* bits 31:31 */
    cs_uint32 cpu_rx_buff0_rdy     :  1 ; /* bits 30:30 */
    cs_uint32 cpu_tx_buff_rdy      :  1 ; /* bits 29:29 */
    cs_uint32 rsrvd2               :  5 ;
    cs_uint32 mii0_rx_pkt_os       :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 auto_xfer_done       :  1 ; /* bits 12:12 */
    cs_uint32 dma1_tx_done_i       :  1 ; /* bits 11:11 */
    cs_uint32 dma0_tx_done_i       :  1 ; /* bits 10:10 */
    cs_uint32 mib_msb_int_set      :  1 ; /* bits 9:9 */
    cs_uint32 pkt_xfer_done        :  1 ; /* bits 8:8 */
    cs_uint32 pkt_xfer2mem_done    :  1 ; /* bits 7:7 */
    cs_uint32 mii0_rx_ff_ol        :  1 ; /* bits 6:6 */
    cs_uint32 mii0_tx_stop_done    :  1 ; /* bits 5:5 */
    cs_uint32 mii0_rx_ff_full      :  1 ; /* bits 4:4 */
    cs_uint32 mii0_tx_stop         :  1 ; /* bits 3:3 */
    cs_uint32 mii0_rx_pkt_drop     :  1 ; /* bits 2:2 */
    cs_uint32 mii0_tx_ff_full      :  1 ; /* bits 1:1 */
    cs_uint32 mii0_tx_pkt_done     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mii0_tx_pkt_done     :  1 ; /* bits 0:0 */
    cs_uint32 mii0_tx_ff_full      :  1 ; /* bits 1:1 */
    cs_uint32 mii0_rx_pkt_drop     :  1 ; /* bits 2:2 */
    cs_uint32 mii0_tx_stop         :  1 ; /* bits 3:3 */
    cs_uint32 mii0_rx_ff_full      :  1 ; /* bits 4:4 */
    cs_uint32 mii0_tx_stop_done    :  1 ; /* bits 5:5 */
    cs_uint32 mii0_rx_ff_ol        :  1 ; /* bits 6:6 */
    cs_uint32 pkt_xfer2mem_done    :  1 ; /* bits 7:7 */
    cs_uint32 pkt_xfer_done        :  1 ; /* bits 8:8 */
    cs_uint32 mib_msb_int_set      :  1 ; /* bits 9:9 */
    cs_uint32 dma0_tx_done_i       :  1 ; /* bits 10:10 */
    cs_uint32 dma1_tx_done_i       :  1 ; /* bits 11:11 */
    cs_uint32 auto_xfer_done       :  1 ; /* bits 12:12 */
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 mii0_rx_pkt_os       :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd2               :  5 ;
    cs_uint32 cpu_tx_buff_rdy      :  1 ; /* bits 29:29 */
    cs_uint32 cpu_rx_buff0_rdy     :  1 ; /* bits 30:30 */
    cs_uint32 cpu_rx_buff1_rdy     :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_INT_t;





#define MA_RX_BUF0_STAT                          0x0500d
#define MA_RX_BUF0_STAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pkt_rdy              :  1 ; /* bits 31:31 */
    cs_uint32 turn                 :  1 ; /* bits 30:30 */
    cs_uint32 len_err              :  1 ; /* bits 29:29 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 spid                 :  3 ; /* bits 13:11 */
    cs_uint32 pkt_len              : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_len              : 11 ; /* bits 10:0 */
    cs_uint32 spid                 :  3 ; /* bits 13:11 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 len_err              :  1 ; /* bits 29:29 */
    cs_uint32 turn                 :  1 ; /* bits 30:30 */
    cs_uint32 pkt_rdy              :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_RX_BUF0_STAT_t;





#define MA_RX_BUF1_STAT                          0x0500e
#define MA_RX_BUF1_STAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pkt_rdy              :  1 ; /* bits 31:31 */
    cs_uint32 turn                 :  1 ; /* bits 30:30 */
    cs_uint32 len_err              :  1 ; /* bits 29:29 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 spid                 :  3 ; /* bits 13:11 */
    cs_uint32 pkt_len              : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_len              : 11 ; /* bits 10:0 */
    cs_uint32 spid                 :  3 ; /* bits 13:11 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 len_err              :  1 ; /* bits 29:29 */
    cs_uint32 turn                 :  1 ; /* bits 30:30 */
    cs_uint32 pkt_rdy              :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_RX_BUF1_STAT_t;





#define MA_TX_BUF_CTRL                           0x0500f
#define MA_TX_BUF_CTRL_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pkt_rdy              :  1 ; /* bits 31:31 */
    cs_uint32 ptp_pkt              :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd2               : 17 ;
    cs_uint32 dpid                 :  1 ; /* bits 12:12 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 pkt_len              : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_len              : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 dpid                 :  1 ; /* bits 12:12 */
    cs_uint32 rsrvd2               : 17 ;
    cs_uint32 ptp_pkt              :  1 ; /* bits 30:30 */
    cs_uint32 pkt_rdy              :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_TX_BUF_CTRL_t;





#define MA_PORT_RDY                              0x05010
#define MA_PORT_RDY_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 12 ;
    cs_uint32 es_rx                :  1 ; /* bits 19:19 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 mii0_rx              :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 mii0_tx              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mii0_tx              :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 mii0_rx              :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 es_rx                :  1 ; /* bits 19:19 */
    cs_uint32 rsrvd3               : 12 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_PORT_RDY_t;





#define MA_CPU_DST_EN_MII0                       0x05011
#define MA_CPU_DST_EN_MII0_dft                   0x01800000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  7 ;
    cs_uint32 te_mii_pen0          :  1 ; /* bits 24:24 */
    cs_uint32 mii0                 :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 tpid_drp             :  1 ; /* bits 19:19 */
    cs_uint32 ev2pt                :  1 ; /* bits 18:18 */
    cs_uint32 vlan                 :  1 ; /* bits 17:17 */
    cs_uint32 da                   :  1 ; /* bits 16:16 */
    cs_uint32 pkt_type             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_type             : 16 ; /* bits 15:0 */
    cs_uint32 da                   :  1 ; /* bits 16:16 */
    cs_uint32 vlan                 :  1 ; /* bits 17:17 */
    cs_uint32 ev2pt                :  1 ; /* bits 18:18 */
    cs_uint32 tpid_drp             :  1 ; /* bits 19:19 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 mii0                 :  1 ; /* bits 23:23 */
    cs_uint32 te_mii_pen0          :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd2               :  7 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_CPU_DST_EN_MII0_t;





#define MA_CPU_DST0_MII0                         0x05012
#define MA_CPU_DST0_MII0_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 da                   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 da                   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_CPU_DST0_MII0_t;





#define MA_CPU_DST1_MII0                         0x05013
#define MA_CPU_DST1_MII0_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pkt_typ_drop         : 16 ; /* bits 31:16 */
    cs_uint32 da                   : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 da                   : 16 ; /* bits 15:0 */
    cs_uint32 pkt_typ_drop         : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_CPU_DST1_MII0_t;





#define MA_CPU_DST2_MII0                         0x05014
#define MA_CPU_DST2_MII0_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 vlan_id              : 12 ; /* bits 27:16 */
    cs_uint32 ev2pt                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ev2pt                : 16 ; /* bits 15:0 */
    cs_uint32 vlan_id              : 12 ; /* bits 27:16 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_CPU_DST2_MII0_t;





#define MA_VLAN_TYP_MII0                         0x05015
#define MA_VLAN_TYP_MII0_dft                     0x000088a8




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tpid_udef            : 16 ; /* bits 31:16 */
    cs_uint32 tpid_det             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tpid_det             : 16 ; /* bits 15:0 */
    cs_uint32 tpid_udef            : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_VLAN_TYP_MII0_t;





#define MA_TPID_DRP_MII0                         0x05016
#define MA_TPID_DRP_MII0_dft                     0x000088a8




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 tpid                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tpid                 : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_TPID_DRP_MII0_t;





#define MA_PTP_CTRL0                             0x05017
#define MA_PTP_CTRL0_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 ptp_en               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ptp_en               :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_PTP_CTRL0_t;





#define MA_PTP_DPID0                             0x05018
#define MA_PTP_DPID0_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 sp0                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sp0                  :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_PTP_DPID0_t;





#define MA_PTP_DPID1                             0x05019
#define MA_PTP_DPID1_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 sp0                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sp0                  :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_PTP_DPID1_t;





#define MA_PTP_BKID                              0x0501a
#define MA_PTP_BKID_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 sp0                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sp0                  :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_PTP_BKID_t;





#define MA_CPU_CTRL                              0x0501b
#define MA_CPU_CTRL_dft                          0x00000004




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ptp_enable           :  1 ; /* bits 31:31 */
    cs_uint32 rx_byte_swap_en      :  1 ; /* bits 30:30 */
    cs_uint32 tx_byte_swap_en      :  1 ; /* bits 29:29 */
    cs_uint32 mir_cos              :  3 ; /* bits 28:26 */
    cs_uint32 mir_dpid             :  3 ; /* bits 25:23 */
    cs_uint32 cpu_cos              :  3 ; /* bits 22:20 */
    cs_uint32 cpu_dpid             :  3 ; /* bits 19:17 */
    cs_uint32 dma_cos              :  3 ; /* bits 16:14 */
    cs_uint32 dma_dpid             :  3 ; /* bits 13:11 */
    cs_uint32 mii0_cos             :  3 ; /* bits 10:8 */
    cs_uint32 mii0_dpid            :  3 ; /* bits 7:5 */
    cs_uint32 es                   :  1 ; /* bits 4:4 */
    cs_uint32 bypass               :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 bypass               :  4 ; /* bits 3:0 */
    cs_uint32 es                   :  1 ; /* bits 4:4 */
    cs_uint32 mii0_dpid            :  3 ; /* bits 7:5 */
    cs_uint32 mii0_cos             :  3 ; /* bits 10:8 */
    cs_uint32 dma_dpid             :  3 ; /* bits 13:11 */
    cs_uint32 dma_cos              :  3 ; /* bits 16:14 */
    cs_uint32 cpu_dpid             :  3 ; /* bits 19:17 */
    cs_uint32 cpu_cos              :  3 ; /* bits 22:20 */
    cs_uint32 mir_dpid             :  3 ; /* bits 25:23 */
    cs_uint32 mir_cos              :  3 ; /* bits 28:26 */
    cs_uint32 tx_byte_swap_en      :  1 ; /* bits 29:29 */
    cs_uint32 rx_byte_swap_en      :  1 ; /* bits 30:30 */
    cs_uint32 ptp_enable           :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_CPU_CTRL_t;





#define MA_BM_MA_FF_CTRL                         0x0501c
#define MA_BM_MA_FF_CTRL_dft                     0x4e000100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 ma_fe_ipg_lmt        :  5 ; /* bits 30:26 */
    cs_uint32 ff_upper_lmt         : 10 ; /* bits 25:16 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 ff_lower_lmt         : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ff_lower_lmt         : 10 ; /* bits 9:0 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 ff_upper_lmt         : 10 ; /* bits 25:16 */
    cs_uint32 ma_fe_ipg_lmt        :  5 ; /* bits 30:26 */
    cs_uint32 rsrvd2               :  1 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_BM_MA_FF_CTRL_t;





#define MA_RXBUF0                                0x05200
#define MA_RXBUF0_STRIDE                         1
#define MA_RXBUF0_COUNT                          512
#define MA_RXBUF0_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 data                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 data                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_RXBUF0_t;





#define MA_RXBUF1                                0x05400
#define MA_RXBUF1_STRIDE                         1
#define MA_RXBUF1_COUNT                          512
#define MA_RXBUF1_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 data                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 data                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_RXBUF1_t;





#define MA_TXBUF                                 0x05600
#define MA_TXBUF_STRIDE                          1
#define MA_TXBUF_COUNT                           512
#define MA_TXBUF_dft                             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 data                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 data                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_TXBUF_t;





#define MA_ESDMA_CTRL                            0x05800
#define MA_ESDMA_CTRL_dft                        0x00100000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pkt_rdy              :  1 ; /* bits 31:31 */
    cs_uint32 pkt_len_clr          :  1 ; /* bits 30:30 */
    cs_uint32 pkt_xfer_en          :  1 ; /* bits 29:29 */
    cs_uint32 auto_xfer_done_st    :  1 ; /* bits 28:28 */
    cs_uint32 int_err              :  1 ; /* bits 27:27 */
    cs_uint32 clear_buf_en         :  1 ; /* bits 26:26 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 axfer_toc_bucket     : 14 ; /* bits 24:11 */
    cs_uint32 pkt_len              : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_len              : 11 ; /* bits 10:0 */
    cs_uint32 axfer_toc_bucket     : 14 ; /* bits 24:11 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 clear_buf_en         :  1 ; /* bits 26:26 */
    cs_uint32 int_err              :  1 ; /* bits 27:27 */
    cs_uint32 auto_xfer_done_st    :  1 ; /* bits 28:28 */
    cs_uint32 pkt_xfer_en          :  1 ; /* bits 29:29 */
    cs_uint32 pkt_len_clr          :  1 ; /* bits 30:30 */
    cs_uint32 pkt_rdy              :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_ESDMA_CTRL_t;





#define MA_ESDMA_ADRC                            0x05801
#define MA_ESDMA_ADRC_dft                        0x00800200




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pkt_axfer_en         :  1 ; /* bits 31:31 */
    cs_uint32 axfer_load_tok       :  1 ; /* bits 30:30 */
    cs_uint32 fifo_mode_en         :  1 ; /* bits 29:29 */
    cs_uint32 byte_swap_en         :  1 ; /* bits 28:28 */
    cs_uint32 upper_limit_addr     : 14 ; /* bits 27:14 */
    cs_uint32 axfer_tok            : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 axfer_tok            : 14 ; /* bits 13:0 */
    cs_uint32 upper_limit_addr     : 14 ; /* bits 27:14 */
    cs_uint32 byte_swap_en         :  1 ; /* bits 28:28 */
    cs_uint32 fifo_mode_en         :  1 ; /* bits 29:29 */
    cs_uint32 axfer_load_tok       :  1 ; /* bits 30:30 */
    cs_uint32 pkt_axfer_en         :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_ESDMA_ADRC_t;





#define MA_ESDMA_BADDR                           0x05802
#define MA_ESDMA_BADDR_dft                       0x10000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 baddr                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 baddr                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_ESDMA_BADDR_t;





#define MA_DMA0_BUF_CTRL                         0x05803
#define MA_DMA0_BUF_CTRL_dft                     0x40000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pkt_rdy              :  1 ; /* bits 31:31 */
    cs_uint32 xfer_done            :  1 ; /* bits 30:30 */
    cs_uint32 we                   :  1 ; /* bits 29:29 */
    cs_uint32 hd_sel               :  1 ; /* bits 28:28 */
    cs_uint32 byte_swap_en         :  1 ; /* bits 27:27 */
    cs_uint32 eop_byte_swap_en     :  1 ; /* bits 26:26 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 pkt_len              : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_len              : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 eop_byte_swap_en     :  1 ; /* bits 26:26 */
    cs_uint32 byte_swap_en         :  1 ; /* bits 27:27 */
    cs_uint32 hd_sel               :  1 ; /* bits 28:28 */
    cs_uint32 we                   :  1 ; /* bits 29:29 */
    cs_uint32 xfer_done            :  1 ; /* bits 30:30 */
    cs_uint32 pkt_rdy              :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_DMA0_BUF_CTRL_t;





#define MA_DMA1_BUF_CTRL                         0x05804
#define MA_DMA1_BUF_CTRL_dft                     0x40000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pkt_rdy              :  1 ; /* bits 31:31 */
    cs_uint32 xfer_done            :  1 ; /* bits 30:30 */
    cs_uint32 we                   :  1 ; /* bits 29:29 */
    cs_uint32 hd_sel               :  1 ; /* bits 28:28 */
    cs_uint32 byte_swap_en         :  1 ; /* bits 27:27 */
    cs_uint32 eop_byte_swap_en     :  1 ; /* bits 26:26 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 pkt_len              : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_len              : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 eop_byte_swap_en     :  1 ; /* bits 26:26 */
    cs_uint32 byte_swap_en         :  1 ; /* bits 27:27 */
    cs_uint32 hd_sel               :  1 ; /* bits 28:28 */
    cs_uint32 we                   :  1 ; /* bits 29:29 */
    cs_uint32 xfer_done            :  1 ; /* bits 30:30 */
    cs_uint32 pkt_rdy              :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_DMA1_BUF_CTRL_t;





#define MA_PKT_LEN_CONFIG                        0x05805
#define MA_PKT_LEN_CONFIG_dft                    0x00400600




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mib_op_mode          :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd2               :  7 ;
    cs_uint32 min_size             :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 max_size             : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 max_size             : 12 ; /* bits 11:0 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 min_size             :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd2               :  7 ;
    cs_uint32 mib_op_mode          :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_PKT_LEN_CONFIG_t;





#define MA_INIT                                  0x05806
#define MA_INIT_dft                              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 mib                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mib                  :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_INIT_t;





#define MA_MIB_ACCESS                            0x05807
#define MA_MIB_ACCESS_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 op_code              :  2 ; /* bits 29:28 */
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 counter              :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 counter              :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 op_code              :  2 ; /* bits 29:28 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MIB_ACCESS_t;

#define     counter_RxUCPktCnt                            0x0
#define     counter_RxMCFrmCnt                            0x1
#define     counter_RxBCFrmCnt                            0x2
#define     counter_RxOAMFrmCnt                           0x3
#define     counter_RxPauseFrmCnt                         0x4
#define     counter_RxUnKnownOCFrmCnt                     0x5
#define     counter_RxCrcErrFrmCnt                        0x6
#define     counter_RxUndersizeFrmCnt                     0x7
#define     counter_RxRuntFrmCnt                          0x8
#define     counter_RxOvSizeFrmCnt                        0x9
#define     counter_RxJabberFrmCnt                        0xa
#define     counter_RxStatsFrm64Oct                       0xb
#define     counter_RxStatsFrm65to127Oct                  0xc
#define     counter_RxStatsFrm128to255Oct                 0xd
#define     counter_RxStatsFrm256to511Oct                 0xe
#define     counter_RxStatsFrm512to1023Oct                0xf
#define     counter_RxStatsFrm1024to1518Oct               0x10
#define     counter_RxStatsFrm1519toMaxOct                0x11
#define     counter_RxByteCount                           0x12
#define     counter_RxFrmCount                            0x14
#define     counter_TxUCPktCnt                            0x20
#define     counter_TxMCFrmCnt                            0x21
#define     counter_TxBCFrmCnt                            0x22
#define     counter_TxOAMFrmCnt                           0x23
#define     counter_TxPauseFrmCnt                         0x24
#define     counter_TxCrcErrFrmCnt                        0x25
#define     counter_TxOvSizeFrmCnt                        0x26
#define     counter_TxEStatsFrm64Oct                      0x2b
#define     counter_TxEStatsFrm65to127Oct                 0x2c
#define     counter_TxEStatsFrm128to255Oct                0x2d
#define     counter_TxEStatsFrm256to511Oct                0x2e
#define     counter_TxEStatsFrm512to1023Oct               0x2f
#define     counter_TxEStatsFrm1024to1518Oct              0x30
#define     counter_TxEStatsFrm1519toMaxOct               0x31
#define     counter_TxByteCount                           0x32
#define     counter_TxFrmCount                            0x34




#define MA_MIB_DATA1                             0x05808
#define MA_MIB_DATA1_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt_val_hi           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt_val_hi           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MIB_DATA1_t;





#define MA_MIB_DATA0                             0x05809
#define MA_MIB_DATA0_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt_val_lo           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt_val_lo           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} MA_MIB_DATA0_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* ma */

typedef struct {
  MA_MII0_TX_CTRL_t                        MII0_TX_CTRL;                  /* addr: 0x00000 */ 
  MA_MII0_TX_CFG_t                         MII0_TX_CFG;                   /* addr: 0x00001 */ 
  MA_MII0_FLOW_SA0_t                       MII0_FLOW_SA0;                 /* addr: 0x00002 */ 
  MA_MII0_FLOW_SA1_t                       MII0_FLOW_SA1;                 /* addr: 0x00003 */ 
  MA_MII0_TX_STAT0_t                       MII0_TX_STAT0;                 /* addr: 0x00004 */ 
  MA_MII0_TX_STAT1_t                       MII0_TX_STAT1;                 /* addr: 0x00005 */ 
  MA_MII0_RX_CTRL_t                        MII0_RX_CTRL;                  /* addr: 0x00006 */ 
  MA_MII0_RX_CTRL1_t                       MII0_RX_CTRL1;                 /* addr: 0x00007 */ 
  MA_MII0_RX_CNT_t                         MII0_RX_CNT;                   /* addr: 0x00008 */ 
  MA_MII0_RX_STAT_t                        MII0_RX_STAT;                  /* addr: 0x00009 */ 
  MA_MII0_RX_STAT1_t                       MII0_RX_STAT1;                 /* addr: 0x0000a */ 
  MA_INT_EN_t                              INT_EN;                        /* addr: 0x0000b */ 
  MA_INT_t                                 INT;                           /* addr: 0x0000c */ 
  MA_RX_BUF0_STAT_t                        RX_BUF0_STAT;                  /* addr: 0x0000d */ 
  MA_RX_BUF1_STAT_t                        RX_BUF1_STAT;                  /* addr: 0x0000e */ 
  MA_TX_BUF_CTRL_t                         TX_BUF_CTRL;                   /* addr: 0x0000f */ 
  MA_PORT_RDY_t                            PORT_RDY;                      /* addr: 0x00010 */ 
  MA_CPU_DST_EN_MII0_t                     CPU_DST_EN_MII0;               /* addr: 0x00011 */ 
  MA_CPU_DST0_MII0_t                       CPU_DST0_MII0;                 /* addr: 0x00012 */ 
  MA_CPU_DST1_MII0_t                       CPU_DST1_MII0;                 /* addr: 0x00013 */ 
  MA_CPU_DST2_MII0_t                       CPU_DST2_MII0;                 /* addr: 0x00014 */ 
  MA_VLAN_TYP_MII0_t                       VLAN_TYP_MII0;                 /* addr: 0x00015 */ 
  MA_TPID_DRP_MII0_t                       TPID_DRP_MII0;                 /* addr: 0x00016 */ 
  MA_PTP_CTRL0_t                           PTP_CTRL0;                     /* addr: 0x00017 */ 
  MA_PTP_DPID0_t                           PTP_DPID0;                     /* addr: 0x00018 */ 
  MA_PTP_DPID1_t                           PTP_DPID1;                     /* addr: 0x00019 */ 
  MA_PTP_BKID_t                            PTP_BKID;                      /* addr: 0x0001a */ 
  MA_CPU_CTRL_t                            CPU_CTRL;                      /* addr: 0x0001b */ 
  MA_BM_MA_FF_CTRL_t                       BM_MA_FF_CTRL;                 /* addr: 0x0001c */ 
  cs_uint32                                rsrvd1[483];                  
  MA_RXBUF0_t                              RXBUF0;                        /* addr: 0x00200 */ 
  cs_uint32                                rsrvd2[511];                  
  MA_RXBUF1_t                              RXBUF1;                        /* addr: 0x00400 */ 
  cs_uint32                                rsrvd3[511];                  
  MA_TXBUF_t                               TXBUF;                         /* addr: 0x00600 */ 
  cs_uint32                                rsrvd4[511];                  
  MA_ESDMA_CTRL_t                          ESDMA_CTRL;                    /* addr: 0x00800 */ 
  MA_ESDMA_ADRC_t                          ESDMA_ADRC;                    /* addr: 0x00801 */ 
  MA_ESDMA_BADDR_t                         ESDMA_BADDR;                   /* addr: 0x00802 */ 
  MA_DMA0_BUF_CTRL_t                       DMA0_BUF_CTRL;                 /* addr: 0x00803 */ 
  MA_DMA1_BUF_CTRL_t                       DMA1_BUF_CTRL;                 /* addr: 0x00804 */ 
  MA_PKT_LEN_CONFIG_t                      PKT_LEN_CONFIG;                /* addr: 0x00805 */ 
  MA_INIT_t                                INIT;                          /* addr: 0x00806 */ 
  MA_MIB_ACCESS_t                          MIB_ACCESS;                    /* addr: 0x00807 */ 
  MA_MIB_DATA1_t                           MIB_DATA1;                     /* addr: 0x00808 */ 
  MA_MIB_DATA0_t                           MIB_DATA0;                     /* addr: 0x00809 */ 
} MA_t;

  

/*
 * fe
 */
  
  
/*
 * top
 */
  



#define FE_TOP_L2E_LRN_AGING_CTRL_0              0x06000
#define FE_TOP_L2E_LRN_AGING_CTRL_0_dft          0x1000fffe




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 sw_l2_pkt_mirror_en  :  1 ; /* bits 28:28 */
    cs_uint32 age_en               :  1 ; /* bits 27:27 */
    cs_uint32 station_move_en      :  1 ; /* bits 26:26 */
    cs_uint32 learn_dot1p_vld_mii1 :  1 ; /* bits 25:25 */
    cs_uint32 learn_dot1p_vld_mii0 :  1 ; /* bits 24:24 */
    cs_uint32 learn_dot1p_vld_cpu  :  1 ; /* bits 23:23 */
    cs_uint32 learn_dot1p_vld_pon  :  1 ; /* bits 22:22 */
    cs_uint32 learn_dot1p_vld_ge   :  1 ; /* bits 21:21 */
    cs_uint32 learn_vlan_vld_mii1  :  1 ; /* bits 20:20 */
    cs_uint32 learn_vlan_vld_mii0  :  1 ; /* bits 19:19 */
    cs_uint32 learn_vlan_vld_cpu   :  1 ; /* bits 18:18 */
    cs_uint32 learn_vlan_vld_pon   :  1 ; /* bits 17:17 */
    cs_uint32 learn_vlan_vld_ge    :  1 ; /* bits 16:16 */
    cs_uint32 learn_mac_vld_mii1   :  1 ; /* bits 15:15 */
    cs_uint32 learn_mac_vld_mii0   :  1 ; /* bits 14:14 */
    cs_uint32 learn_mac_vld_cpu    :  1 ; /* bits 13:13 */
    cs_uint32 learn_mac_vld_pon    :  1 ; /* bits 12:12 */
    cs_uint32 learn_mac_vld_ge     :  1 ; /* bits 11:11 */
    cs_uint32 mii1_permit          :  2 ; /* bits 10:9 */
    cs_uint32 mii0_permit          :  2 ; /* bits 8:7 */
    cs_uint32 cpu_permit           :  2 ; /* bits 6:5 */
    cs_uint32 ge_permit            :  2 ; /* bits 4:3 */
    cs_uint32 pon_permit           :  2 ; /* bits 2:1 */
    cs_uint32 learn_mode           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 learn_mode           :  1 ; /* bits 0:0 */
    cs_uint32 pon_permit           :  2 ; /* bits 2:1 */
    cs_uint32 ge_permit            :  2 ; /* bits 4:3 */
    cs_uint32 cpu_permit           :  2 ; /* bits 6:5 */
    cs_uint32 mii0_permit          :  2 ; /* bits 8:7 */
    cs_uint32 mii1_permit          :  2 ; /* bits 10:9 */
    cs_uint32 learn_mac_vld_ge     :  1 ; /* bits 11:11 */
    cs_uint32 learn_mac_vld_pon    :  1 ; /* bits 12:12 */
    cs_uint32 learn_mac_vld_cpu    :  1 ; /* bits 13:13 */
    cs_uint32 learn_mac_vld_mii0   :  1 ; /* bits 14:14 */
    cs_uint32 learn_mac_vld_mii1   :  1 ; /* bits 15:15 */
    cs_uint32 learn_vlan_vld_ge    :  1 ; /* bits 16:16 */
    cs_uint32 learn_vlan_vld_pon   :  1 ; /* bits 17:17 */
    cs_uint32 learn_vlan_vld_cpu   :  1 ; /* bits 18:18 */
    cs_uint32 learn_vlan_vld_mii0  :  1 ; /* bits 19:19 */
    cs_uint32 learn_vlan_vld_mii1  :  1 ; /* bits 20:20 */
    cs_uint32 learn_dot1p_vld_ge   :  1 ; /* bits 21:21 */
    cs_uint32 learn_dot1p_vld_pon  :  1 ; /* bits 22:22 */
    cs_uint32 learn_dot1p_vld_cpu  :  1 ; /* bits 23:23 */
    cs_uint32 learn_dot1p_vld_mii0 :  1 ; /* bits 24:24 */
    cs_uint32 learn_dot1p_vld_mii1 :  1 ; /* bits 25:25 */
    cs_uint32 station_move_en      :  1 ; /* bits 26:26 */
    cs_uint32 age_en               :  1 ; /* bits 27:27 */
    cs_uint32 sw_l2_pkt_mirror_en  :  1 ; /* bits 28:28 */
    cs_uint32 rsrvd1               :  3 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_LRN_AGING_CTRL_0_t;





#define FE_TOP_L2E_LRN_AGING_CTRL_1              0x06001
#define FE_TOP_L2E_LRN_AGING_CTRL_1_dft          0x00f00fff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 age_stm_delta_val    : 12 ; /* bits 23:12 */
    cs_uint32 age_start_val        : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 age_start_val        : 12 ; /* bits 11:0 */
    cs_uint32 age_stm_delta_val    : 12 ; /* bits 23:12 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_LRN_AGING_CTRL_1_t;





#define FE_TOP_L2E_FRWD_CTRL                     0x06002
#define FE_TOP_L2E_FRWD_CTRL_dft                 0x0000001f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 l2_mirror_group_id   :  5 ; /* bits 26:22 */
    cs_uint32 stn_mov_drop_en      :  1 ; /* bits 21:21 */
    cs_uint32 st_loop_det_drop     :  1 ; /* bits 20:20 */
    cs_uint32 sa_lrn_max_limit_drop_en_mii1 :  1 ; /* bits 19:19 */
    cs_uint32 sa_lrn_max_limit_drop_en_mii0 :  1 ; /* bits 18:18 */
    cs_uint32 sa_lrn_max_limit_drop_en_cpu :  1 ; /* bits 17:17 */
    cs_uint32 sa_lrn_max_limit_drop_en_pon :  1 ; /* bits 16:16 */
    cs_uint32 sa_lrn_max_limit_drop_en_ge :  1 ; /* bits 15:15 */
    cs_uint32 fwd_dot1p_vld_mii1   :  1 ; /* bits 14:14 */
    cs_uint32 fwd_dot1p_vld_mii0   :  1 ; /* bits 13:13 */
    cs_uint32 fwd_dot1p_vld_cpu    :  1 ; /* bits 12:12 */
    cs_uint32 fwd_dot1p_vld_pon    :  1 ; /* bits 11:11 */
    cs_uint32 fwd_dot1p_vld_ge     :  1 ; /* bits 10:10 */
    cs_uint32 fwd_vlan_vld_mii1    :  1 ; /* bits 9:9 */
    cs_uint32 fwd_vlan_vld_mii0    :  1 ; /* bits 8:8 */
    cs_uint32 fwd_vlan_vld_cpu     :  1 ; /* bits 7:7 */
    cs_uint32 fwd_vlan_vld_pon     :  1 ; /* bits 6:6 */
    cs_uint32 fwd_vlan_vld_ge      :  1 ; /* bits 5:5 */
    cs_uint32 fwd_mac_vld_mii1     :  1 ; /* bits 4:4 */
    cs_uint32 fwd_mac_vld_mii0     :  1 ; /* bits 3:3 */
    cs_uint32 fwd_mac_vld_cpu      :  1 ; /* bits 2:2 */
    cs_uint32 fwd_mac_vld_pon      :  1 ; /* bits 1:1 */
    cs_uint32 fwd_mac_vld_ge       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 fwd_mac_vld_ge       :  1 ; /* bits 0:0 */
    cs_uint32 fwd_mac_vld_pon      :  1 ; /* bits 1:1 */
    cs_uint32 fwd_mac_vld_cpu      :  1 ; /* bits 2:2 */
    cs_uint32 fwd_mac_vld_mii0     :  1 ; /* bits 3:3 */
    cs_uint32 fwd_mac_vld_mii1     :  1 ; /* bits 4:4 */
    cs_uint32 fwd_vlan_vld_ge      :  1 ; /* bits 5:5 */
    cs_uint32 fwd_vlan_vld_pon     :  1 ; /* bits 6:6 */
    cs_uint32 fwd_vlan_vld_cpu     :  1 ; /* bits 7:7 */
    cs_uint32 fwd_vlan_vld_mii0    :  1 ; /* bits 8:8 */
    cs_uint32 fwd_vlan_vld_mii1    :  1 ; /* bits 9:9 */
    cs_uint32 fwd_dot1p_vld_ge     :  1 ; /* bits 10:10 */
    cs_uint32 fwd_dot1p_vld_pon    :  1 ; /* bits 11:11 */
    cs_uint32 fwd_dot1p_vld_cpu    :  1 ; /* bits 12:12 */
    cs_uint32 fwd_dot1p_vld_mii0   :  1 ; /* bits 13:13 */
    cs_uint32 fwd_dot1p_vld_mii1   :  1 ; /* bits 14:14 */
    cs_uint32 sa_lrn_max_limit_drop_en_ge :  1 ; /* bits 15:15 */
    cs_uint32 sa_lrn_max_limit_drop_en_pon :  1 ; /* bits 16:16 */
    cs_uint32 sa_lrn_max_limit_drop_en_cpu :  1 ; /* bits 17:17 */
    cs_uint32 sa_lrn_max_limit_drop_en_mii0 :  1 ; /* bits 18:18 */
    cs_uint32 sa_lrn_max_limit_drop_en_mii1 :  1 ; /* bits 19:19 */
    cs_uint32 st_loop_det_drop     :  1 ; /* bits 20:20 */
    cs_uint32 stn_mov_drop_en      :  1 ; /* bits 21:21 */
    cs_uint32 l2_mirror_group_id   :  5 ; /* bits 26:22 */
    cs_uint32 rsrvd1               :  5 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_FRWD_CTRL_t;





#define FE_TOP_L2E_LEARN_CFG_0                   0x06003
#define FE_TOP_L2E_LEARN_CFG_0_dft               0x00a15010




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 mii0_sa_max_learn_limit :  7 ; /* bits 27:21 */
    cs_uint32 cpu_sa_max_learn_limit :  7 ; /* bits 20:14 */
    cs_uint32 pon_sa_max_learn_limit :  7 ; /* bits 13:7 */
    cs_uint32 ge_sa_max_learn_limit :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ge_sa_max_learn_limit :  7 ; /* bits 6:0 */
    cs_uint32 pon_sa_max_learn_limit :  7 ; /* bits 13:7 */
    cs_uint32 cpu_sa_max_learn_limit :  7 ; /* bits 20:14 */
    cs_uint32 mii0_sa_max_learn_limit :  7 ; /* bits 27:21 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_LEARN_CFG_0_t;





#define FE_TOP_L2E_LEARN_CFG_1                   0x06004
#define FE_TOP_L2E_LEARN_CFG_1_dft               0x00000005




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 25 ;
    cs_uint32 mii1_sa_max_learn_limit :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mii1_sa_max_learn_limit :  7 ; /* bits 6:0 */
    cs_uint32 rsrvd1               : 25 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_LEARN_CFG_1_t;





#define FE_TOP_L2E_AGING_CFG                     0x06005
#define FE_TOP_L2E_AGING_CFG_dft                 0x00100000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 aging_timer          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 aging_timer          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_AGING_CFG_t;





#define FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_0       0x06006
#define FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_0_dft   0x00010000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 12 ;
    cs_uint32 vlan_limit_valid     :  1 ; /* bits 19:19 */
    cs_uint32 mac_limit            :  7 ; /* bits 18:12 */
    cs_uint32 mac_limited_vlan     : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_limited_vlan     : 12 ; /* bits 11:0 */
    cs_uint32 mac_limit            :  7 ; /* bits 18:12 */
    cs_uint32 vlan_limit_valid     :  1 ; /* bits 19:19 */
    cs_uint32 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_0_t;





#define FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_1       0x06007
#define FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_1_dft   0x00010000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 12 ;
    cs_uint32 vlan_limit_valid     :  1 ; /* bits 19:19 */
    cs_uint32 mac_limit            :  7 ; /* bits 18:12 */
    cs_uint32 mac_limited_vlan     : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_limited_vlan     : 12 ; /* bits 11:0 */
    cs_uint32 mac_limit            :  7 ; /* bits 18:12 */
    cs_uint32 vlan_limit_valid     :  1 ; /* bits 19:19 */
    cs_uint32 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_1_t;





#define FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_2       0x06008
#define FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_2_dft   0x00010000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 12 ;
    cs_uint32 vlan_limit_valid     :  1 ; /* bits 19:19 */
    cs_uint32 mac_limit            :  7 ; /* bits 18:12 */
    cs_uint32 mac_limited_vlan     : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_limited_vlan     : 12 ; /* bits 11:0 */
    cs_uint32 mac_limit            :  7 ; /* bits 18:12 */
    cs_uint32 vlan_limit_valid     :  1 ; /* bits 19:19 */
    cs_uint32 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_2_t;





#define FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_3       0x06009
#define FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_3_dft   0x00010000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 12 ;
    cs_uint32 vlan_limit_valid     :  1 ; /* bits 19:19 */
    cs_uint32 mac_limit            :  7 ; /* bits 18:12 */
    cs_uint32 mac_limited_vlan     : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_limited_vlan     : 12 ; /* bits 11:0 */
    cs_uint32 mac_limit            :  7 ; /* bits 18:12 */
    cs_uint32 vlan_limit_valid     :  1 ; /* bits 19:19 */
    cs_uint32 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_3_t;





#define FE_TOP_L2E_LEARN_STS_0                   0x0600a
#define FE_TOP_L2E_LEARN_STS_0_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 mii0_learn_cnt       :  7 ; /* bits 27:21 */
    cs_uint32 cpu_learn_cnt        :  7 ; /* bits 20:14 */
    cs_uint32 pon_learn_cnt        :  7 ; /* bits 13:7 */
    cs_uint32 ge_learn_cnt         :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ge_learn_cnt         :  7 ; /* bits 6:0 */
    cs_uint32 pon_learn_cnt        :  7 ; /* bits 13:7 */
    cs_uint32 cpu_learn_cnt        :  7 ; /* bits 20:14 */
    cs_uint32 mii0_learn_cnt       :  7 ; /* bits 27:21 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_LEARN_STS_0_t;





#define FE_TOP_L2E_LEARN_STS_1                   0x0600b
#define FE_TOP_L2E_LEARN_STS_1_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 25 ;
    cs_uint32 mii1_learn_cnt       :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mii1_learn_cnt       :  7 ; /* bits 6:0 */
    cs_uint32 rsrvd1               : 25 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_LEARN_STS_1_t;





#define FE_TOP_L2E_LEARN_STS_2                   0x0600c
#define FE_TOP_L2E_LEARN_STS_2_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 ge_vln3_learn_cnt    :  7 ; /* bits 27:21 */
    cs_uint32 ge_vln2_learn_cnt    :  7 ; /* bits 20:14 */
    cs_uint32 ge_vln1_learn_cnt    :  7 ; /* bits 13:7 */
    cs_uint32 ge_vln0_learn_cnt    :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ge_vln0_learn_cnt    :  7 ; /* bits 6:0 */
    cs_uint32 ge_vln1_learn_cnt    :  7 ; /* bits 13:7 */
    cs_uint32 ge_vln2_learn_cnt    :  7 ; /* bits 20:14 */
    cs_uint32 ge_vln3_learn_cnt    :  7 ; /* bits 27:21 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_LEARN_STS_2_t;





#define FE_TOP_L2E_AGING_STS                     0x0600d
#define FE_TOP_L2E_AGING_STS_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 26 ;
    cs_uint32 aging_index          :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 aging_index          :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 26 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_AGING_STS_t;





#define FE_TOP_L2E_STN_MOV_IDX_STS               0x0600e
#define FE_TOP_L2E_STN_MOV_IDX_STS_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 mii1_station_move_index :  6 ; /* bits 29:24 */
    cs_uint32 mii0_station_move_index :  6 ; /* bits 23:18 */
    cs_uint32 cpu_station_move_index :  6 ; /* bits 17:12 */
    cs_uint32 pon_station_move_index :  6 ; /* bits 11:6 */
    cs_uint32 ge_station_move_index :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ge_station_move_index :  6 ; /* bits 5:0 */
    cs_uint32 pon_station_move_index :  6 ; /* bits 11:6 */
    cs_uint32 cpu_station_move_index :  6 ; /* bits 17:12 */
    cs_uint32 mii0_station_move_index :  6 ; /* bits 23:18 */
    cs_uint32 mii1_station_move_index :  6 ; /* bits 29:24 */
    cs_uint32 rsrvd1               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_STN_MOV_IDX_STS_t;





#define FE_TOP_L2E_TABLE_MEM_ACCESS              0x0600f
#define FE_TOP_L2E_TABLE_MEM_ACCESS_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 address              :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 address              :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_TABLE_MEM_ACCESS_t;





#define FE_TOP_L2E_TABLE_MEM_DATA2               0x06010
#define FE_TOP_L2E_TABLE_MEM_DATA2_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 age_cnt              : 12 ; /* bits 28:17 */
    cs_uint32 mii1_sa_pd           :  1 ; /* bits 16:16 */
    cs_uint32 mii1_da_pd           :  1 ; /* bits 15:15 */
    cs_uint32 mii0_sa_pd           :  1 ; /* bits 14:14 */
    cs_uint32 mii0_da_pd           :  1 ; /* bits 13:13 */
    cs_uint32 cpu_sa_pd            :  1 ; /* bits 12:12 */
    cs_uint32 cpu_da_pd            :  1 ; /* bits 11:11 */
    cs_uint32 pon_sa_pd            :  1 ; /* bits 10:10 */
    cs_uint32 pon_da_pd            :  1 ; /* bits 9:9 */
    cs_uint32 ge_sa_pd             :  1 ; /* bits 8:8 */
    cs_uint32 ge_da_pd             :  1 ; /* bits 7:7 */
    cs_uint32 pid                  :  3 ; /* bits 6:4 */
    cs_uint32 static_dyn           :  1 ; /* bits 3:3 */
    cs_uint32 valid                :  1 ; /* bits 2:2 */
    cs_uint32 dot1p_vld            :  1 ; /* bits 1:1 */
    cs_uint32 vlanid_vld           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 vlanid_vld           :  1 ; /* bits 0:0 */
    cs_uint32 dot1p_vld            :  1 ; /* bits 1:1 */
    cs_uint32 valid                :  1 ; /* bits 2:2 */
    cs_uint32 static_dyn           :  1 ; /* bits 3:3 */
    cs_uint32 pid                  :  3 ; /* bits 6:4 */
    cs_uint32 ge_da_pd             :  1 ; /* bits 7:7 */
    cs_uint32 ge_sa_pd             :  1 ; /* bits 8:8 */
    cs_uint32 pon_da_pd            :  1 ; /* bits 9:9 */
    cs_uint32 pon_sa_pd            :  1 ; /* bits 10:10 */
    cs_uint32 cpu_da_pd            :  1 ; /* bits 11:11 */
    cs_uint32 cpu_sa_pd            :  1 ; /* bits 12:12 */
    cs_uint32 mii0_da_pd           :  1 ; /* bits 13:13 */
    cs_uint32 mii0_sa_pd           :  1 ; /* bits 14:14 */
    cs_uint32 mii1_da_pd           :  1 ; /* bits 15:15 */
    cs_uint32 mii1_sa_pd           :  1 ; /* bits 16:16 */
    cs_uint32 age_cnt              : 12 ; /* bits 28:17 */
    cs_uint32 rsrvd1               :  3 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_TABLE_MEM_DATA2_t;





#define FE_TOP_L2E_TABLE_MEM_DATA1               0x06011
#define FE_TOP_L2E_TABLE_MEM_DATA1_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mac_vld              :  1 ; /* bits 31:31 */
    cs_uint32 dot1p                :  3 ; /* bits 30:28 */
    cs_uint32 vid                  : 12 ; /* bits 27:16 */
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
    cs_uint32 vid                  : 12 ; /* bits 27:16 */
    cs_uint32 dot1p                :  3 ; /* bits 30:28 */
    cs_uint32 mac_vld              :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_TABLE_MEM_DATA1_t;





#define FE_TOP_L2E_TABLE_MEM_DATA0               0x06012
#define FE_TOP_L2E_TABLE_MEM_DATA0_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_L2E_TABLE_MEM_DATA0_t;





#define FE_TOP_GE_PORT_CTRL_0                    0x06013
#define FE_TOP_GE_PORT_CTRL_0_dft                0x0280000e




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 sa_drop_en           :  1 ; /* bits 25:25 */
    cs_uint32 untagged_vlan_en     :  1 ; /* bits 24:24 */
    cs_uint32 cpu_header_pkt_type  :  1 ; /* bits 23:23 */
    cs_uint32 l2_fwd_vlan_sel      :  1 ; /* bits 22:22 */
    cs_uint32 vlan_en              :  1 ; /* bits 21:21 */
    cs_uint32 ipv6_tc_mask         :  6 ; /* bits 20:15 */
    cs_uint32 ipv4_dscp_mask       :  6 ; /* bits 14:9 */
    cs_uint32 dbg_da_sa_swap_en    :  1 ; /* bits 8:8 */
    cs_uint32 oam_lpbk_en          :  1 ; /* bits 7:7 */
    cs_uint32 l2_vlan_sel          :  2 ; /* bits 6:5 */
    cs_uint32 block_pkt_fwd        :  1 ; /* bits 4:4 */
    cs_uint32 block_dpid_drop_lrn  :  1 ; /* bits 3:3 */
    cs_uint32 block_dpid_cpu_lrn   :  1 ; /* bits 2:2 */
    cs_uint32 l2_learn_lru_en      :  1 ; /* bits 1:1 */
    cs_uint32 block_pkt_l2_lrn     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 block_pkt_l2_lrn     :  1 ; /* bits 0:0 */
    cs_uint32 l2_learn_lru_en      :  1 ; /* bits 1:1 */
    cs_uint32 block_dpid_cpu_lrn   :  1 ; /* bits 2:2 */
    cs_uint32 block_dpid_drop_lrn  :  1 ; /* bits 3:3 */
    cs_uint32 block_pkt_fwd        :  1 ; /* bits 4:4 */
    cs_uint32 l2_vlan_sel          :  2 ; /* bits 6:5 */
    cs_uint32 oam_lpbk_en          :  1 ; /* bits 7:7 */
    cs_uint32 dbg_da_sa_swap_en    :  1 ; /* bits 8:8 */
    cs_uint32 ipv4_dscp_mask       :  6 ; /* bits 14:9 */
    cs_uint32 ipv6_tc_mask         :  6 ; /* bits 20:15 */
    cs_uint32 vlan_en              :  1 ; /* bits 21:21 */
    cs_uint32 l2_fwd_vlan_sel      :  1 ; /* bits 22:22 */
    cs_uint32 cpu_header_pkt_type  :  1 ; /* bits 23:23 */
    cs_uint32 untagged_vlan_en     :  1 ; /* bits 24:24 */
    cs_uint32 sa_drop_en           :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_GE_PORT_CTRL_0_t;





#define FE_TOP_GE_PORT_CTRL_1                    0x06014
#define FE_TOP_GE_PORT_CTRL_1_dft                0x00039e79




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 umc_mirror_src_sel   :  1 ; /* bits 31:31 */
    cs_uint32 umc_mirror_groupid   :  5 ; /* bits 30:26 */
    cs_uint32 umc_mirror_en        :  1 ; /* bits 25:25 */
    cs_uint32 uuc_mirror_src_sel   :  1 ; /* bits 24:24 */
    cs_uint32 uuc_mirror_groupid   :  5 ; /* bits 23:19 */
    cs_uint32 uuc_mirror_en        :  1 ; /* bits 18:18 */
    cs_uint32 mc_fwd_cos           :  3 ; /* bits 17:15 */
    cs_uint32 mc_dpid              :  3 ; /* bits 14:12 */
    cs_uint32 bc_fwd_cos           :  3 ; /* bits 11:9 */
    cs_uint32 bc_dpid              :  3 ; /* bits 8:6 */
    cs_uint32 uuc_fwd_cos          :  3 ; /* bits 5:3 */
    cs_uint32 uuc_dpid             :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 uuc_dpid             :  3 ; /* bits 2:0 */
    cs_uint32 uuc_fwd_cos          :  3 ; /* bits 5:3 */
    cs_uint32 bc_dpid              :  3 ; /* bits 8:6 */
    cs_uint32 bc_fwd_cos           :  3 ; /* bits 11:9 */
    cs_uint32 mc_dpid              :  3 ; /* bits 14:12 */
    cs_uint32 mc_fwd_cos           :  3 ; /* bits 17:15 */
    cs_uint32 uuc_mirror_en        :  1 ; /* bits 18:18 */
    cs_uint32 uuc_mirror_groupid   :  5 ; /* bits 23:19 */
    cs_uint32 uuc_mirror_src_sel   :  1 ; /* bits 24:24 */
    cs_uint32 umc_mirror_en        :  1 ; /* bits 25:25 */
    cs_uint32 umc_mirror_groupid   :  5 ; /* bits 30:26 */
    cs_uint32 umc_mirror_src_sel   :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_GE_PORT_CTRL_1_t;





#define FE_TOP_GE_PORT_CTRL_2                    0x06015
#define FE_TOP_GE_PORT_CTRL_2_dft                0x001fffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 bc_mirror_src_sel    :  1 ; /* bits 27:27 */
    cs_uint32 bc_mirror_groupid    :  5 ; /* bits 26:22 */
    cs_uint32 bc_mirror_en         :  1 ; /* bits 21:21 */
    cs_uint32 sp_pkt_permit        : 21 ; /* bits 20:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sp_pkt_permit        : 21 ; /* bits 20:0 */
    cs_uint32 bc_mirror_en         :  1 ; /* bits 21:21 */
    cs_uint32 bc_mirror_groupid    :  5 ; /* bits 26:22 */
    cs_uint32 bc_mirror_src_sel    :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_GE_PORT_CTRL_2_t;





#define FE_TOP_GE_PORT_DFLT_CFG_0                0x06016
#define FE_TOP_GE_PORT_DFLT_CFG_0_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 dscp                 :  6 ; /* bits 23:18 */
    cs_uint32 Eight021p            :  3 ; /* bits 17:15 */
    cs_uint32 cos                  :  3 ; /* bits 14:12 */
    cs_uint32 vlanid               : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 vlanid               : 12 ; /* bits 11:0 */
    cs_uint32 cos                  :  3 ; /* bits 14:12 */
    cs_uint32 Eight021p            :  3 ; /* bits 17:15 */
    cs_uint32 dscp                 :  6 ; /* bits 23:18 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_GE_PORT_DFLT_CFG_0_t;





#define FE_TOP_GE_PORT_DFLT_CFG_1                0x06017
#define FE_TOP_GE_PORT_DFLT_CFG_1_dft            0x00008100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 tpid                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tpid                 : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_GE_PORT_DFLT_CFG_1_t;





#define FE_TOP_PON_PORT_CTRL_0                   0x06018
#define FE_TOP_PON_PORT_CTRL_0_dft               0x0280000e




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 sa_drop_en           :  1 ; /* bits 25:25 */
    cs_uint32 untagged_vlan_en     :  1 ; /* bits 24:24 */
    cs_uint32 cpu_header_pkt_type  :  1 ; /* bits 23:23 */
    cs_uint32 l2_fwd_vlan_sel      :  1 ; /* bits 22:22 */
    cs_uint32 vlan_en              :  1 ; /* bits 21:21 */
    cs_uint32 ipv6_tc_mask         :  6 ; /* bits 20:15 */
    cs_uint32 ipv4_dscp_mask       :  6 ; /* bits 14:9 */
    cs_uint32 dbg_da_sa_swap_en    :  1 ; /* bits 8:8 */
    cs_uint32 oam_lpbk_en          :  1 ; /* bits 7:7 */
    cs_uint32 l2_vlan_sel          :  2 ; /* bits 6:5 */
    cs_uint32 block_pkt_fwd        :  1 ; /* bits 4:4 */
    cs_uint32 block_dpid_drop_lrn  :  1 ; /* bits 3:3 */
    cs_uint32 block_dpid_cpu_lrn   :  1 ; /* bits 2:2 */
    cs_uint32 l2_learn_lru_en      :  1 ; /* bits 1:1 */
    cs_uint32 block_pkt_l2_lrn     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 block_pkt_l2_lrn     :  1 ; /* bits 0:0 */
    cs_uint32 l2_learn_lru_en      :  1 ; /* bits 1:1 */
    cs_uint32 block_dpid_cpu_lrn   :  1 ; /* bits 2:2 */
    cs_uint32 block_dpid_drop_lrn  :  1 ; /* bits 3:3 */
    cs_uint32 block_pkt_fwd        :  1 ; /* bits 4:4 */
    cs_uint32 l2_vlan_sel          :  2 ; /* bits 6:5 */
    cs_uint32 oam_lpbk_en          :  1 ; /* bits 7:7 */
    cs_uint32 dbg_da_sa_swap_en    :  1 ; /* bits 8:8 */
    cs_uint32 ipv4_dscp_mask       :  6 ; /* bits 14:9 */
    cs_uint32 ipv6_tc_mask         :  6 ; /* bits 20:15 */
    cs_uint32 vlan_en              :  1 ; /* bits 21:21 */
    cs_uint32 l2_fwd_vlan_sel      :  1 ; /* bits 22:22 */
    cs_uint32 cpu_header_pkt_type  :  1 ; /* bits 23:23 */
    cs_uint32 untagged_vlan_en     :  1 ; /* bits 24:24 */
    cs_uint32 sa_drop_en           :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_PON_PORT_CTRL_0_t;





#define FE_TOP_PON_PORT_CTRL_1                   0x06019
#define FE_TOP_PON_PORT_CTRL_1_dft               0x00038e38




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 umc_mirror_src_sel   :  1 ; /* bits 31:31 */
    cs_uint32 umc_mirror_groupid   :  5 ; /* bits 30:26 */
    cs_uint32 umc_mirror_en        :  1 ; /* bits 25:25 */
    cs_uint32 uuc_mirror_src_sel   :  1 ; /* bits 24:24 */
    cs_uint32 uuc_mirror_groupid   :  5 ; /* bits 23:19 */
    cs_uint32 uuc_mirror_en        :  1 ; /* bits 18:18 */
    cs_uint32 mc_fwd_cos           :  3 ; /* bits 17:15 */
    cs_uint32 mc_dpid              :  3 ; /* bits 14:12 */
    cs_uint32 bc_fwd_cos           :  3 ; /* bits 11:9 */
    cs_uint32 bc_dpid              :  3 ; /* bits 8:6 */
    cs_uint32 uuc_fwd_cos          :  3 ; /* bits 5:3 */
    cs_uint32 uuc_dpid             :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 uuc_dpid             :  3 ; /* bits 2:0 */
    cs_uint32 uuc_fwd_cos          :  3 ; /* bits 5:3 */
    cs_uint32 bc_dpid              :  3 ; /* bits 8:6 */
    cs_uint32 bc_fwd_cos           :  3 ; /* bits 11:9 */
    cs_uint32 mc_dpid              :  3 ; /* bits 14:12 */
    cs_uint32 mc_fwd_cos           :  3 ; /* bits 17:15 */
    cs_uint32 uuc_mirror_en        :  1 ; /* bits 18:18 */
    cs_uint32 uuc_mirror_groupid   :  5 ; /* bits 23:19 */
    cs_uint32 uuc_mirror_src_sel   :  1 ; /* bits 24:24 */
    cs_uint32 umc_mirror_en        :  1 ; /* bits 25:25 */
    cs_uint32 umc_mirror_groupid   :  5 ; /* bits 30:26 */
    cs_uint32 umc_mirror_src_sel   :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_PON_PORT_CTRL_1_t;





#define FE_TOP_PON_PORT_CTRL_2                   0x0601a
#define FE_TOP_PON_PORT_CTRL_2_dft               0x001fffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 bc_mirror_src_sel    :  1 ; /* bits 27:27 */
    cs_uint32 bc_mirror_groupid    :  5 ; /* bits 26:22 */
    cs_uint32 bc_mirror_en         :  1 ; /* bits 21:21 */
    cs_uint32 sp_pkt_permit        : 21 ; /* bits 20:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sp_pkt_permit        : 21 ; /* bits 20:0 */
    cs_uint32 bc_mirror_en         :  1 ; /* bits 21:21 */
    cs_uint32 bc_mirror_groupid    :  5 ; /* bits 26:22 */
    cs_uint32 bc_mirror_src_sel    :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_PON_PORT_CTRL_2_t;





#define FE_TOP_PON_PORT_DFLT_CFG_0               0x0601b
#define FE_TOP_PON_PORT_DFLT_CFG_0_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 dscp                 :  6 ; /* bits 23:18 */
    cs_uint32 Eight021p            :  3 ; /* bits 17:15 */
    cs_uint32 cos                  :  3 ; /* bits 14:12 */
    cs_uint32 vlanid               : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 vlanid               : 12 ; /* bits 11:0 */
    cs_uint32 cos                  :  3 ; /* bits 14:12 */
    cs_uint32 Eight021p            :  3 ; /* bits 17:15 */
    cs_uint32 dscp                 :  6 ; /* bits 23:18 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_PON_PORT_DFLT_CFG_0_t;





#define FE_TOP_PON_PORT_DFLT_CFG_1               0x0601c
#define FE_TOP_PON_PORT_DFLT_CFG_1_dft           0x00008100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 tpid                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tpid                 : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_PON_PORT_DFLT_CFG_1_t;





#define FE_TOP_CPU_PORT_CTRL_0                   0x0601d
#define FE_TOP_CPU_PORT_CTRL_0_dft               0x0280000e




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 sa_drop_en           :  1 ; /* bits 25:25 */
    cs_uint32 untagged_vlan_en     :  1 ; /* bits 24:24 */
    cs_uint32 cpu_header_pkt_type  :  1 ; /* bits 23:23 */
    cs_uint32 l2_fwd_vlan_sel      :  1 ; /* bits 22:22 */
    cs_uint32 vlan_en              :  1 ; /* bits 21:21 */
    cs_uint32 ipv6_tc_mask         :  6 ; /* bits 20:15 */
    cs_uint32 ipv4_dscp_mask       :  6 ; /* bits 14:9 */
    cs_uint32 dbg_da_sa_swap_en    :  1 ; /* bits 8:8 */
    cs_uint32 oam_lpbk_en          :  1 ; /* bits 7:7 */
    cs_uint32 l2_vlan_sel          :  2 ; /* bits 6:5 */
    cs_uint32 block_pkt_fwd        :  1 ; /* bits 4:4 */
    cs_uint32 block_dpid_drop_lrn  :  1 ; /* bits 3:3 */
    cs_uint32 block_dpid_cpu_lrn   :  1 ; /* bits 2:2 */
    cs_uint32 l2_learn_lru_en      :  1 ; /* bits 1:1 */
    cs_uint32 block_pkt_l2_lrn     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 block_pkt_l2_lrn     :  1 ; /* bits 0:0 */
    cs_uint32 l2_learn_lru_en      :  1 ; /* bits 1:1 */
    cs_uint32 block_dpid_cpu_lrn   :  1 ; /* bits 2:2 */
    cs_uint32 block_dpid_drop_lrn  :  1 ; /* bits 3:3 */
    cs_uint32 block_pkt_fwd        :  1 ; /* bits 4:4 */
    cs_uint32 l2_vlan_sel          :  2 ; /* bits 6:5 */
    cs_uint32 oam_lpbk_en          :  1 ; /* bits 7:7 */
    cs_uint32 dbg_da_sa_swap_en    :  1 ; /* bits 8:8 */
    cs_uint32 ipv4_dscp_mask       :  6 ; /* bits 14:9 */
    cs_uint32 ipv6_tc_mask         :  6 ; /* bits 20:15 */
    cs_uint32 vlan_en              :  1 ; /* bits 21:21 */
    cs_uint32 l2_fwd_vlan_sel      :  1 ; /* bits 22:22 */
    cs_uint32 cpu_header_pkt_type  :  1 ; /* bits 23:23 */
    cs_uint32 untagged_vlan_en     :  1 ; /* bits 24:24 */
    cs_uint32 sa_drop_en           :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_CPU_PORT_CTRL_0_t;





#define FE_TOP_CPU_PORT_CTRL_1                   0x0601e
#define FE_TOP_CPU_PORT_CTRL_1_dft               0x00038e38




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 umc_mirror_src_sel   :  1 ; /* bits 31:31 */
    cs_uint32 umc_mirror_groupid   :  5 ; /* bits 30:26 */
    cs_uint32 umc_mirror_en        :  1 ; /* bits 25:25 */
    cs_uint32 uuc_mirror_src_sel   :  1 ; /* bits 24:24 */
    cs_uint32 uuc_mirror_groupid   :  5 ; /* bits 23:19 */
    cs_uint32 uuc_mirror_en        :  1 ; /* bits 18:18 */
    cs_uint32 mc_fwd_cos           :  3 ; /* bits 17:15 */
    cs_uint32 mc_dpid              :  3 ; /* bits 14:12 */
    cs_uint32 bc_fwd_cos           :  3 ; /* bits 11:9 */
    cs_uint32 bc_dpid              :  3 ; /* bits 8:6 */
    cs_uint32 uuc_fwd_cos          :  3 ; /* bits 5:3 */
    cs_uint32 uuc_dpid             :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 uuc_dpid             :  3 ; /* bits 2:0 */
    cs_uint32 uuc_fwd_cos          :  3 ; /* bits 5:3 */
    cs_uint32 bc_dpid              :  3 ; /* bits 8:6 */
    cs_uint32 bc_fwd_cos           :  3 ; /* bits 11:9 */
    cs_uint32 mc_dpid              :  3 ; /* bits 14:12 */
    cs_uint32 mc_fwd_cos           :  3 ; /* bits 17:15 */
    cs_uint32 uuc_mirror_en        :  1 ; /* bits 18:18 */
    cs_uint32 uuc_mirror_groupid   :  5 ; /* bits 23:19 */
    cs_uint32 uuc_mirror_src_sel   :  1 ; /* bits 24:24 */
    cs_uint32 umc_mirror_en        :  1 ; /* bits 25:25 */
    cs_uint32 umc_mirror_groupid   :  5 ; /* bits 30:26 */
    cs_uint32 umc_mirror_src_sel   :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_CPU_PORT_CTRL_1_t;





#define FE_TOP_CPU_PORT_CTRL_2                   0x0601f
#define FE_TOP_CPU_PORT_CTRL_2_dft               0x001fffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 bc_mirror_src_sel    :  1 ; /* bits 27:27 */
    cs_uint32 bc_mirror_groupid    :  5 ; /* bits 26:22 */
    cs_uint32 bc_mirror_en         :  1 ; /* bits 21:21 */
    cs_uint32 sp_pkt_permit        : 21 ; /* bits 20:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sp_pkt_permit        : 21 ; /* bits 20:0 */
    cs_uint32 bc_mirror_en         :  1 ; /* bits 21:21 */
    cs_uint32 bc_mirror_groupid    :  5 ; /* bits 26:22 */
    cs_uint32 bc_mirror_src_sel    :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_CPU_PORT_CTRL_2_t;





#define FE_TOP_CPU_PORT_DFLT_CFG_0               0x06020
#define FE_TOP_CPU_PORT_DFLT_CFG_0_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 dscp                 :  6 ; /* bits 23:18 */
    cs_uint32 Eight021p            :  3 ; /* bits 17:15 */
    cs_uint32 cos                  :  3 ; /* bits 14:12 */
    cs_uint32 vlanid               : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 vlanid               : 12 ; /* bits 11:0 */
    cs_uint32 cos                  :  3 ; /* bits 14:12 */
    cs_uint32 Eight021p            :  3 ; /* bits 17:15 */
    cs_uint32 dscp                 :  6 ; /* bits 23:18 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_CPU_PORT_DFLT_CFG_0_t;





#define FE_TOP_CPU_PORT_DFLT_CFG_1               0x06021
#define FE_TOP_CPU_PORT_DFLT_CFG_1_dft           0x00008100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 tpid                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tpid                 : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_CPU_PORT_DFLT_CFG_1_t;





#define FE_TOP_MII0_PORT_CTRL_0                  0x06022
#define FE_TOP_MII0_PORT_CTRL_0_dft              0x0280000e




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 sa_drop_en           :  1 ; /* bits 25:25 */
    cs_uint32 untagged_vlan_en     :  1 ; /* bits 24:24 */
    cs_uint32 cpu_header_pkt_type  :  1 ; /* bits 23:23 */
    cs_uint32 l2_fwd_vlan_sel      :  1 ; /* bits 22:22 */
    cs_uint32 vlan_en              :  1 ; /* bits 21:21 */
    cs_uint32 ipv6_tc_mask         :  6 ; /* bits 20:15 */
    cs_uint32 ipv4_dscp_mask       :  6 ; /* bits 14:9 */
    cs_uint32 dbg_da_sa_swap_en    :  1 ; /* bits 8:8 */
    cs_uint32 oam_lpbk_en          :  1 ; /* bits 7:7 */
    cs_uint32 l2_vlan_sel          :  2 ; /* bits 6:5 */
    cs_uint32 block_pkt_fwd        :  1 ; /* bits 4:4 */
    cs_uint32 block_dpid_drop_lrn  :  1 ; /* bits 3:3 */
    cs_uint32 block_dpid_cpu_lrn   :  1 ; /* bits 2:2 */
    cs_uint32 l2_learn_lru_en      :  1 ; /* bits 1:1 */
    cs_uint32 block_pkt_l2_lrn     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 block_pkt_l2_lrn     :  1 ; /* bits 0:0 */
    cs_uint32 l2_learn_lru_en      :  1 ; /* bits 1:1 */
    cs_uint32 block_dpid_cpu_lrn   :  1 ; /* bits 2:2 */
    cs_uint32 block_dpid_drop_lrn  :  1 ; /* bits 3:3 */
    cs_uint32 block_pkt_fwd        :  1 ; /* bits 4:4 */
    cs_uint32 l2_vlan_sel          :  2 ; /* bits 6:5 */
    cs_uint32 oam_lpbk_en          :  1 ; /* bits 7:7 */
    cs_uint32 dbg_da_sa_swap_en    :  1 ; /* bits 8:8 */
    cs_uint32 ipv4_dscp_mask       :  6 ; /* bits 14:9 */
    cs_uint32 ipv6_tc_mask         :  6 ; /* bits 20:15 */
    cs_uint32 vlan_en              :  1 ; /* bits 21:21 */
    cs_uint32 l2_fwd_vlan_sel      :  1 ; /* bits 22:22 */
    cs_uint32 cpu_header_pkt_type  :  1 ; /* bits 23:23 */
    cs_uint32 untagged_vlan_en     :  1 ; /* bits 24:24 */
    cs_uint32 sa_drop_en           :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MII0_PORT_CTRL_0_t;





#define FE_TOP_MII0_PORT_CTRL_1                  0x06023
#define FE_TOP_MII0_PORT_CTRL_1_dft              0x00038e38




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 umc_mirror_src_sel   :  1 ; /* bits 31:31 */
    cs_uint32 umc_mirror_groupid   :  5 ; /* bits 30:26 */
    cs_uint32 umc_mirror_en        :  1 ; /* bits 25:25 */
    cs_uint32 uuc_mirror_src_sel   :  1 ; /* bits 24:24 */
    cs_uint32 uuc_mirror_groupid   :  5 ; /* bits 23:19 */
    cs_uint32 uuc_mirror_en        :  1 ; /* bits 18:18 */
    cs_uint32 mc_fwd_cos           :  3 ; /* bits 17:15 */
    cs_uint32 mc_dpid              :  3 ; /* bits 14:12 */
    cs_uint32 bc_fwd_cos           :  3 ; /* bits 11:9 */
    cs_uint32 bc_dpid              :  3 ; /* bits 8:6 */
    cs_uint32 uuc_fwd_cos          :  3 ; /* bits 5:3 */
    cs_uint32 uuc_dpid             :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 uuc_dpid             :  3 ; /* bits 2:0 */
    cs_uint32 uuc_fwd_cos          :  3 ; /* bits 5:3 */
    cs_uint32 bc_dpid              :  3 ; /* bits 8:6 */
    cs_uint32 bc_fwd_cos           :  3 ; /* bits 11:9 */
    cs_uint32 mc_dpid              :  3 ; /* bits 14:12 */
    cs_uint32 mc_fwd_cos           :  3 ; /* bits 17:15 */
    cs_uint32 uuc_mirror_en        :  1 ; /* bits 18:18 */
    cs_uint32 uuc_mirror_groupid   :  5 ; /* bits 23:19 */
    cs_uint32 uuc_mirror_src_sel   :  1 ; /* bits 24:24 */
    cs_uint32 umc_mirror_en        :  1 ; /* bits 25:25 */
    cs_uint32 umc_mirror_groupid   :  5 ; /* bits 30:26 */
    cs_uint32 umc_mirror_src_sel   :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MII0_PORT_CTRL_1_t;





#define FE_TOP_MII0_PORT_CTRL_2                  0x06024
#define FE_TOP_MII0_PORT_CTRL_2_dft              0x001fffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 bc_mirror_src_sel    :  1 ; /* bits 27:27 */
    cs_uint32 bc_mirror_groupid    :  5 ; /* bits 26:22 */
    cs_uint32 bc_mirror_en         :  1 ; /* bits 21:21 */
    cs_uint32 sp_pkt_permit        : 21 ; /* bits 20:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sp_pkt_permit        : 21 ; /* bits 20:0 */
    cs_uint32 bc_mirror_en         :  1 ; /* bits 21:21 */
    cs_uint32 bc_mirror_groupid    :  5 ; /* bits 26:22 */
    cs_uint32 bc_mirror_src_sel    :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MII0_PORT_CTRL_2_t;





#define FE_TOP_MII0_PORT_DFLT_CFG_0              0x06025
#define FE_TOP_MII0_PORT_DFLT_CFG_0_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 dscp                 :  6 ; /* bits 23:18 */
    cs_uint32 Eight021p            :  3 ; /* bits 17:15 */
    cs_uint32 cos                  :  3 ; /* bits 14:12 */
    cs_uint32 vlanid               : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 vlanid               : 12 ; /* bits 11:0 */
    cs_uint32 cos                  :  3 ; /* bits 14:12 */
    cs_uint32 Eight021p            :  3 ; /* bits 17:15 */
    cs_uint32 dscp                 :  6 ; /* bits 23:18 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MII0_PORT_DFLT_CFG_0_t;





#define FE_TOP_MII0_PORT_DFLT_CFG_1              0x06026
#define FE_TOP_MII0_PORT_DFLT_CFG_1_dft          0x00008100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 tpid                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tpid                 : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MII0_PORT_DFLT_CFG_1_t;





#define FE_TOP_MII1_PORT_CTRL_0                  0x06027
#define FE_TOP_MII1_PORT_CTRL_0_dft              0x0280000e




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 sa_drop_en           :  1 ; /* bits 25:25 */
    cs_uint32 untagged_vlan_en     :  1 ; /* bits 24:24 */
    cs_uint32 cpu_header_pkt_type  :  1 ; /* bits 23:23 */
    cs_uint32 l2_fwd_vlan_sel      :  1 ; /* bits 22:22 */
    cs_uint32 vlan_en              :  1 ; /* bits 21:21 */
    cs_uint32 ipv6_tc_mask         :  6 ; /* bits 20:15 */
    cs_uint32 ipv4_dscp_mask       :  6 ; /* bits 14:9 */
    cs_uint32 dbg_da_sa_swap_en    :  1 ; /* bits 8:8 */
    cs_uint32 oam_lpbk_en          :  1 ; /* bits 7:7 */
    cs_uint32 l2_vlan_sel          :  2 ; /* bits 6:5 */
    cs_uint32 block_pkt_fwd        :  1 ; /* bits 4:4 */
    cs_uint32 block_dpid_drop_lrn  :  1 ; /* bits 3:3 */
    cs_uint32 block_dpid_cpu_lrn   :  1 ; /* bits 2:2 */
    cs_uint32 l2_learn_lru_en      :  1 ; /* bits 1:1 */
    cs_uint32 block_pkt_l2_lrn     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 block_pkt_l2_lrn     :  1 ; /* bits 0:0 */
    cs_uint32 l2_learn_lru_en      :  1 ; /* bits 1:1 */
    cs_uint32 block_dpid_cpu_lrn   :  1 ; /* bits 2:2 */
    cs_uint32 block_dpid_drop_lrn  :  1 ; /* bits 3:3 */
    cs_uint32 block_pkt_fwd        :  1 ; /* bits 4:4 */
    cs_uint32 l2_vlan_sel          :  2 ; /* bits 6:5 */
    cs_uint32 oam_lpbk_en          :  1 ; /* bits 7:7 */
    cs_uint32 dbg_da_sa_swap_en    :  1 ; /* bits 8:8 */
    cs_uint32 ipv4_dscp_mask       :  6 ; /* bits 14:9 */
    cs_uint32 ipv6_tc_mask         :  6 ; /* bits 20:15 */
    cs_uint32 vlan_en              :  1 ; /* bits 21:21 */
    cs_uint32 l2_fwd_vlan_sel      :  1 ; /* bits 22:22 */
    cs_uint32 cpu_header_pkt_type  :  1 ; /* bits 23:23 */
    cs_uint32 untagged_vlan_en     :  1 ; /* bits 24:24 */
    cs_uint32 sa_drop_en           :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MII1_PORT_CTRL_0_t;





#define FE_TOP_MII1_PORT_CTRL_1                  0x06028
#define FE_TOP_MII1_PORT_CTRL_1_dft              0x00038e38




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 umc_mirror_src_sel   :  1 ; /* bits 31:31 */
    cs_uint32 umc_mirror_groupid   :  5 ; /* bits 30:26 */
    cs_uint32 umc_mirror_en        :  1 ; /* bits 25:25 */
    cs_uint32 uuc_mirror_src_sel   :  1 ; /* bits 24:24 */
    cs_uint32 uuc_mirror_groupid   :  5 ; /* bits 23:19 */
    cs_uint32 uuc_mirror_en        :  1 ; /* bits 18:18 */
    cs_uint32 mc_fwd_cos           :  3 ; /* bits 17:15 */
    cs_uint32 mc_dpid              :  3 ; /* bits 14:12 */
    cs_uint32 bc_fwd_cos           :  3 ; /* bits 11:9 */
    cs_uint32 bc_dpid              :  3 ; /* bits 8:6 */
    cs_uint32 uuc_fwd_cos          :  3 ; /* bits 5:3 */
    cs_uint32 uuc_dpid             :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 uuc_dpid             :  3 ; /* bits 2:0 */
    cs_uint32 uuc_fwd_cos          :  3 ; /* bits 5:3 */
    cs_uint32 bc_dpid              :  3 ; /* bits 8:6 */
    cs_uint32 bc_fwd_cos           :  3 ; /* bits 11:9 */
    cs_uint32 mc_dpid              :  3 ; /* bits 14:12 */
    cs_uint32 mc_fwd_cos           :  3 ; /* bits 17:15 */
    cs_uint32 uuc_mirror_en        :  1 ; /* bits 18:18 */
    cs_uint32 uuc_mirror_groupid   :  5 ; /* bits 23:19 */
    cs_uint32 uuc_mirror_src_sel   :  1 ; /* bits 24:24 */
    cs_uint32 umc_mirror_en        :  1 ; /* bits 25:25 */
    cs_uint32 umc_mirror_groupid   :  5 ; /* bits 30:26 */
    cs_uint32 umc_mirror_src_sel   :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MII1_PORT_CTRL_1_t;





#define FE_TOP_MII1_PORT_CTRL_2                  0x06029
#define FE_TOP_MII1_PORT_CTRL_2_dft              0x001fffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 bc_mirror_src_sel    :  1 ; /* bits 27:27 */
    cs_uint32 bc_mirror_groupid    :  5 ; /* bits 26:22 */
    cs_uint32 bc_mirror_en         :  1 ; /* bits 21:21 */
    cs_uint32 sp_pkt_permit        : 21 ; /* bits 20:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sp_pkt_permit        : 21 ; /* bits 20:0 */
    cs_uint32 bc_mirror_en         :  1 ; /* bits 21:21 */
    cs_uint32 bc_mirror_groupid    :  5 ; /* bits 26:22 */
    cs_uint32 bc_mirror_src_sel    :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MII1_PORT_CTRL_2_t;





#define FE_TOP_MII1_PORT_DFLT_CFG_0              0x0602a
#define FE_TOP_MII1_PORT_DFLT_CFG_0_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 dscp                 :  6 ; /* bits 23:18 */
    cs_uint32 Eight021p            :  3 ; /* bits 17:15 */
    cs_uint32 cos                  :  3 ; /* bits 14:12 */
    cs_uint32 vlanid               : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 vlanid               : 12 ; /* bits 11:0 */
    cs_uint32 cos                  :  3 ; /* bits 14:12 */
    cs_uint32 Eight021p            :  3 ; /* bits 17:15 */
    cs_uint32 dscp                 :  6 ; /* bits 23:18 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MII1_PORT_DFLT_CFG_0_t;





#define FE_TOP_MII1_PORT_DFLT_CFG_1              0x0602b
#define FE_TOP_MII1_PORT_DFLT_CFG_1_dft          0x00008100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 tpid                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tpid                 : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MII1_PORT_DFLT_CFG_1_t;





#define FE_TOP_MISC_CTRL                         0x0602c
#define FE_TOP_MISC_CTRL_dft                     0x00000100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 21 ;
    cs_uint32 dbg_wait_sa_lkup_dis :  1 ; /* bits 10:10 */
    cs_uint32 dbg_l2_port_extn_en  :  1 ; /* bits 9:9 */
    cs_uint32 dbg_cntr_clr_on_read :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd1               :  8 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 dbg_cntr_clr_on_read :  1 ; /* bits 8:8 */
    cs_uint32 dbg_l2_port_extn_en  :  1 ; /* bits 9:9 */
    cs_uint32 dbg_wait_sa_lkup_dis :  1 ; /* bits 10:10 */
    cs_uint32 rsrvd2               : 21 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MISC_CTRL_t;





#define FE_TOP_STS_INTENABLE                     0x0602d
#define FE_TOP_STS_INTENABLE_dft                 0xfffe0000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ma_pp_pkt_len_mismatch_err_intE :  1 ; /* bits 31:31 */
    cs_uint32 pon_pp_pkt_len_mismatch_err_intE :  1 ; /* bits 30:30 */
    cs_uint32 ge_pp_pkt_len_mismatch_err_intE :  1 ; /* bits 29:29 */
    cs_uint32 ma_pp_pkt_len_runt_err_intE :  1 ; /* bits 28:28 */
    cs_uint32 pon_pp_pkt_len_runt_err_intE :  1 ; /* bits 27:27 */
    cs_uint32 ge_pp_pkt_len_runt_err_intE :  1 ; /* bits 26:26 */
    cs_uint32 ce_ma_mac_filter_aged_out_intE :  1 ; /* bits 25:25 */
    cs_uint32 ce_pon_mac_filter_aged_out_intE :  1 ; /* bits 24:24 */
    cs_uint32 ce_ge_mac_filter_aged_out_intE :  1 ; /* bits 23:23 */
    cs_uint32 ma_pkt_len_err_intE  :  1 ; /* bits 22:22 */
    cs_uint32 pon_pkt_len_err_intE :  1 ; /* bits 21:21 */
    cs_uint32 ge_pkt_len_err_intE  :  1 ; /* bits 20:20 */
    cs_uint32 lat_fifo_ma_overflow_intE :  1 ; /* bits 19:19 */
    cs_uint32 lat_fifo_pon_overflow_intE :  1 ; /* bits 18:18 */
    cs_uint32 lat_fifo_ge_overflow_intE :  1 ; /* bits 17:17 */
    cs_uint32 l2_entry_aged_out_intE :  1 ; /* bits 16:16 */
    cs_uint32 l2_no_avail_dyn_entry_learn_intE :  1 ; /* bits 15:15 */
    cs_uint32 l2_ovr_max_limit_mii1_intE :  1 ; /* bits 14:14 */
    cs_uint32 l2_ovr_max_limit_mii0_intE :  1 ; /* bits 13:13 */
    cs_uint32 l2_ovr_max_limit_cpu_intE :  1 ; /* bits 12:12 */
    cs_uint32 l2_ovr_max_limit_pon_intE :  1 ; /* bits 11:11 */
    cs_uint32 l2_ovr_max_limit_ge_intE :  1 ; /* bits 10:10 */
    cs_uint32 l2_stn_mov_mii1_intE :  1 ; /* bits 9:9 */
    cs_uint32 l2_stn_mov_mii0_intE :  1 ; /* bits 8:8 */
    cs_uint32 l2_stn_mov_cpu_intE  :  1 ; /* bits 7:7 */
    cs_uint32 l2_stn_mov_pon_intE  :  1 ; /* bits 6:6 */
    cs_uint32 l2_stn_mov_ge_intE   :  1 ; /* bits 5:5 */
    cs_uint32 l2_learn_mii1_intE   :  1 ; /* bits 4:4 */
    cs_uint32 l2_learn_mii0_intE   :  1 ; /* bits 3:3 */
    cs_uint32 l2_learn_cpu_intE    :  1 ; /* bits 2:2 */
    cs_uint32 l2_learn_pon_intE    :  1 ; /* bits 1:1 */
    cs_uint32 l2_learn_ge_intE     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 l2_learn_ge_intE     :  1 ; /* bits 0:0 */
    cs_uint32 l2_learn_pon_intE    :  1 ; /* bits 1:1 */
    cs_uint32 l2_learn_cpu_intE    :  1 ; /* bits 2:2 */
    cs_uint32 l2_learn_mii0_intE   :  1 ; /* bits 3:3 */
    cs_uint32 l2_learn_mii1_intE   :  1 ; /* bits 4:4 */
    cs_uint32 l2_stn_mov_ge_intE   :  1 ; /* bits 5:5 */
    cs_uint32 l2_stn_mov_pon_intE  :  1 ; /* bits 6:6 */
    cs_uint32 l2_stn_mov_cpu_intE  :  1 ; /* bits 7:7 */
    cs_uint32 l2_stn_mov_mii0_intE :  1 ; /* bits 8:8 */
    cs_uint32 l2_stn_mov_mii1_intE :  1 ; /* bits 9:9 */
    cs_uint32 l2_ovr_max_limit_ge_intE :  1 ; /* bits 10:10 */
    cs_uint32 l2_ovr_max_limit_pon_intE :  1 ; /* bits 11:11 */
    cs_uint32 l2_ovr_max_limit_cpu_intE :  1 ; /* bits 12:12 */
    cs_uint32 l2_ovr_max_limit_mii0_intE :  1 ; /* bits 13:13 */
    cs_uint32 l2_ovr_max_limit_mii1_intE :  1 ; /* bits 14:14 */
    cs_uint32 l2_no_avail_dyn_entry_learn_intE :  1 ; /* bits 15:15 */
    cs_uint32 l2_entry_aged_out_intE :  1 ; /* bits 16:16 */
    cs_uint32 lat_fifo_ge_overflow_intE :  1 ; /* bits 17:17 */
    cs_uint32 lat_fifo_pon_overflow_intE :  1 ; /* bits 18:18 */
    cs_uint32 lat_fifo_ma_overflow_intE :  1 ; /* bits 19:19 */
    cs_uint32 ge_pkt_len_err_intE  :  1 ; /* bits 20:20 */
    cs_uint32 pon_pkt_len_err_intE :  1 ; /* bits 21:21 */
    cs_uint32 ma_pkt_len_err_intE  :  1 ; /* bits 22:22 */
    cs_uint32 ce_ge_mac_filter_aged_out_intE :  1 ; /* bits 23:23 */
    cs_uint32 ce_pon_mac_filter_aged_out_intE :  1 ; /* bits 24:24 */
    cs_uint32 ce_ma_mac_filter_aged_out_intE :  1 ; /* bits 25:25 */
    cs_uint32 ge_pp_pkt_len_runt_err_intE :  1 ; /* bits 26:26 */
    cs_uint32 pon_pp_pkt_len_runt_err_intE :  1 ; /* bits 27:27 */
    cs_uint32 ma_pp_pkt_len_runt_err_intE :  1 ; /* bits 28:28 */
    cs_uint32 ge_pp_pkt_len_mismatch_err_intE :  1 ; /* bits 29:29 */
    cs_uint32 pon_pp_pkt_len_mismatch_err_intE :  1 ; /* bits 30:30 */
    cs_uint32 ma_pp_pkt_len_mismatch_err_intE :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_STS_INTENABLE_t;





#define FE_TOP_STS_INTENABLE_1                   0x0602e
#define FE_TOP_STS_INTENABLE_1_dft               0x00000001




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 l2_stn_mov_loop_det_drop_intE :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 l2_stn_mov_loop_det_drop_intE :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_STS_INTENABLE_1_t;





#define FE_TOP_STS_INTERRUPT                     0x0602f
#define FE_TOP_STS_INTERRUPT_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ma_pp_pkt_len_mismatch_err_intI :  1 ; /* bits 31:31 */
    cs_uint32 pon_pp_pkt_len_mismatch_err_intI :  1 ; /* bits 30:30 */
    cs_uint32 ge_pp_pkt_len_mismatch_err_intI :  1 ; /* bits 29:29 */
    cs_uint32 ma_pp_pkt_len_runt_err_intI :  1 ; /* bits 28:28 */
    cs_uint32 pon_pp_pkt_len_runt_err_intI :  1 ; /* bits 27:27 */
    cs_uint32 ge_pp_pkt_len_runt_err_intI :  1 ; /* bits 26:26 */
    cs_uint32 ce_ma_mac_filter_aged_out_intI :  1 ; /* bits 25:25 */
    cs_uint32 ce_pon_mac_filter_aged_out_intI :  1 ; /* bits 24:24 */
    cs_uint32 ce_ge_mac_filter_aged_out_intI :  1 ; /* bits 23:23 */
    cs_uint32 ma_pkt_len_err_intI  :  1 ; /* bits 22:22 */
    cs_uint32 pon_pkt_len_err_intI :  1 ; /* bits 21:21 */
    cs_uint32 ge_pkt_len_err_intI  :  1 ; /* bits 20:20 */
    cs_uint32 lat_fifo_ma_overflow_intI :  1 ; /* bits 19:19 */
    cs_uint32 lat_fifo_pon_overflow_intI :  1 ; /* bits 18:18 */
    cs_uint32 lat_fifo_ge_overflow_intI :  1 ; /* bits 17:17 */
    cs_uint32 l2_entry_aged_out_intI :  1 ; /* bits 16:16 */
    cs_uint32 l2_no_avail_dyn_entry_learn_intI :  1 ; /* bits 15:15 */
    cs_uint32 l2_ovr_max_limit_mii1_intI :  1 ; /* bits 14:14 */
    cs_uint32 l2_ovr_max_limit_mii0_intI :  1 ; /* bits 13:13 */
    cs_uint32 l2_ovr_max_limit_cpu_intI :  1 ; /* bits 12:12 */
    cs_uint32 l2_ovr_max_limit_pon_intI :  1 ; /* bits 11:11 */
    cs_uint32 l2_ovr_max_limit_ge_intI :  1 ; /* bits 10:10 */
    cs_uint32 l2_stn_mov_mii1_intI :  1 ; /* bits 9:9 */
    cs_uint32 l2_stn_mov_mii0_intI :  1 ; /* bits 8:8 */
    cs_uint32 l2_stn_mov_cpu_intI  :  1 ; /* bits 7:7 */
    cs_uint32 l2_stn_mov_pon_intI  :  1 ; /* bits 6:6 */
    cs_uint32 l2_stn_mov_ge_intI   :  1 ; /* bits 5:5 */
    cs_uint32 l2_learn_mii1_intI   :  1 ; /* bits 4:4 */
    cs_uint32 l2_learn_mii0_intI   :  1 ; /* bits 3:3 */
    cs_uint32 l2_learn_cpu_intI    :  1 ; /* bits 2:2 */
    cs_uint32 l2_learn_pon_intI    :  1 ; /* bits 1:1 */
    cs_uint32 l2_learn_ge_intI     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 l2_learn_ge_intI     :  1 ; /* bits 0:0 */
    cs_uint32 l2_learn_pon_intI    :  1 ; /* bits 1:1 */
    cs_uint32 l2_learn_cpu_intI    :  1 ; /* bits 2:2 */
    cs_uint32 l2_learn_mii0_intI   :  1 ; /* bits 3:3 */
    cs_uint32 l2_learn_mii1_intI   :  1 ; /* bits 4:4 */
    cs_uint32 l2_stn_mov_ge_intI   :  1 ; /* bits 5:5 */
    cs_uint32 l2_stn_mov_pon_intI  :  1 ; /* bits 6:6 */
    cs_uint32 l2_stn_mov_cpu_intI  :  1 ; /* bits 7:7 */
    cs_uint32 l2_stn_mov_mii0_intI :  1 ; /* bits 8:8 */
    cs_uint32 l2_stn_mov_mii1_intI :  1 ; /* bits 9:9 */
    cs_uint32 l2_ovr_max_limit_ge_intI :  1 ; /* bits 10:10 */
    cs_uint32 l2_ovr_max_limit_pon_intI :  1 ; /* bits 11:11 */
    cs_uint32 l2_ovr_max_limit_cpu_intI :  1 ; /* bits 12:12 */
    cs_uint32 l2_ovr_max_limit_mii0_intI :  1 ; /* bits 13:13 */
    cs_uint32 l2_ovr_max_limit_mii1_intI :  1 ; /* bits 14:14 */
    cs_uint32 l2_no_avail_dyn_entry_learn_intI :  1 ; /* bits 15:15 */
    cs_uint32 l2_entry_aged_out_intI :  1 ; /* bits 16:16 */
    cs_uint32 lat_fifo_ge_overflow_intI :  1 ; /* bits 17:17 */
    cs_uint32 lat_fifo_pon_overflow_intI :  1 ; /* bits 18:18 */
    cs_uint32 lat_fifo_ma_overflow_intI :  1 ; /* bits 19:19 */
    cs_uint32 ge_pkt_len_err_intI  :  1 ; /* bits 20:20 */
    cs_uint32 pon_pkt_len_err_intI :  1 ; /* bits 21:21 */
    cs_uint32 ma_pkt_len_err_intI  :  1 ; /* bits 22:22 */
    cs_uint32 ce_ge_mac_filter_aged_out_intI :  1 ; /* bits 23:23 */
    cs_uint32 ce_pon_mac_filter_aged_out_intI :  1 ; /* bits 24:24 */
    cs_uint32 ce_ma_mac_filter_aged_out_intI :  1 ; /* bits 25:25 */
    cs_uint32 ge_pp_pkt_len_runt_err_intI :  1 ; /* bits 26:26 */
    cs_uint32 pon_pp_pkt_len_runt_err_intI :  1 ; /* bits 27:27 */
    cs_uint32 ma_pp_pkt_len_runt_err_intI :  1 ; /* bits 28:28 */
    cs_uint32 ge_pp_pkt_len_mismatch_err_intI :  1 ; /* bits 29:29 */
    cs_uint32 pon_pp_pkt_len_mismatch_err_intI :  1 ; /* bits 30:30 */
    cs_uint32 ma_pp_pkt_len_mismatch_err_intI :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_STS_INTERRUPT_t;





#define FE_TOP_STS_INTERRUPT_1                   0x06030
#define FE_TOP_STS_INTERRUPT_1_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 l2_stn_mov_loop_det_drop_intI :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 l2_stn_mov_loop_det_drop_intI :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_STS_INTERRUPT_1_t;





#define FE_TOP_PRT_SP_PKT_TABLE_ACCESS           0x06031
#define FE_TOP_PRT_SP_PKT_TABLE_ACCESS_dft       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 address              :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 address              :  7 ; /* bits 6:0 */
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_PRT_SP_PKT_TABLE_ACCESS_t;





#define FE_TOP_PRT_SP_PKT_TABLE_DATA             0x06032
#define FE_TOP_PRT_SP_PKT_TABLE_DATA_dft         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 sp_mirror_pkt_src_sel :  1 ; /* bits 21:21 */
    cs_uint32 sp_mirror_en         :  1 ; /* bits 20:20 */
    cs_uint32 sp_flow_id           :  5 ; /* bits 19:15 */
    cs_uint32 sp_flow_en           :  1 ; /* bits 14:14 */
    cs_uint32 sp_pkt_pri_sel       :  5 ; /* bits 13:9 */
    cs_uint32 sp_policer_bypass    :  1 ; /* bits 8:8 */
    cs_uint32 sp_sa_limit_fwd_permit :  1 ; /* bits 7:7 */
    cs_uint32 sp_learn_dis         :  1 ; /* bits 6:6 */
    cs_uint32 sp_cos               :  3 ; /* bits 5:3 */
    cs_uint32 sp_dpid              :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sp_dpid              :  3 ; /* bits 2:0 */
    cs_uint32 sp_cos               :  3 ; /* bits 5:3 */
    cs_uint32 sp_learn_dis         :  1 ; /* bits 6:6 */
    cs_uint32 sp_sa_limit_fwd_permit :  1 ; /* bits 7:7 */
    cs_uint32 sp_policer_bypass    :  1 ; /* bits 8:8 */
    cs_uint32 sp_pkt_pri_sel       :  5 ; /* bits 13:9 */
    cs_uint32 sp_flow_en           :  1 ; /* bits 14:14 */
    cs_uint32 sp_flow_id           :  5 ; /* bits 19:15 */
    cs_uint32 sp_mirror_en         :  1 ; /* bits 20:20 */
    cs_uint32 sp_mirror_pkt_src_sel :  1 ; /* bits 21:21 */
    cs_uint32 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_PRT_SP_PKT_TABLE_DATA_t;





#define FE_TOP_MIRROR_CTRL                       0x06033
#define FE_TOP_MIRROR_CTRL_dft                   0x04040400




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 drr_ma_weight        :  8 ; /* bits 27:20 */
    cs_uint32 drr_pon_weight       :  8 ; /* bits 19:12 */
    cs_uint32 drr_ge_weight        :  8 ; /* bits 11:4 */
    cs_uint32 drr_weight_base      :  2 ; /* bits 3:2 */
    cs_uint32 drr_delay_opt        :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 drr_delay_opt        :  1 ; /* bits 1:1 */
    cs_uint32 drr_weight_base      :  2 ; /* bits 3:2 */
    cs_uint32 drr_ge_weight        :  8 ; /* bits 11:4 */
    cs_uint32 drr_pon_weight       :  8 ; /* bits 19:12 */
    cs_uint32 drr_ma_weight        :  8 ; /* bits 27:20 */
    cs_uint32 rsrvd2               :  4 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MIRROR_CTRL_t;





#define FE_TOP_MIR_0_DROP_CNT                    0x06034
#define FE_TOP_MIR_0_DROP_CNT_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MIR_0_DROP_CNT_t;





#define FE_TOP_MIR_1_DROP_CNT                    0x06035
#define FE_TOP_MIR_1_DROP_CNT_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MIR_1_DROP_CNT_t;





#define FE_TOP_MIR_2_DROP_CNT                    0x06036
#define FE_TOP_MIR_2_DROP_CNT_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_TOP_MIR_2_DROP_CNT_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* top */

typedef struct {
  FE_TOP_L2E_LRN_AGING_CTRL_0_t            L2E_LRN_AGING_CTRL_0;          /* addr: 0x00000 */ 
  FE_TOP_L2E_LRN_AGING_CTRL_1_t            L2E_LRN_AGING_CTRL_1;          /* addr: 0x00001 */ 
  FE_TOP_L2E_FRWD_CTRL_t                   L2E_FRWD_CTRL;                 /* addr: 0x00002 */ 
  FE_TOP_L2E_LEARN_CFG_0_t                 L2E_LEARN_CFG_0;               /* addr: 0x00003 */ 
  FE_TOP_L2E_LEARN_CFG_1_t                 L2E_LEARN_CFG_1;               /* addr: 0x00004 */ 
  FE_TOP_L2E_AGING_CFG_t                   L2E_AGING_CFG;                 /* addr: 0x00005 */ 
  FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_0_t     L2E_GE_VLAN_MAC_LIMIT_CFG_0;   /* addr: 0x00006 */ 
  FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_1_t     L2E_GE_VLAN_MAC_LIMIT_CFG_1;   /* addr: 0x00007 */ 
  FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_2_t     L2E_GE_VLAN_MAC_LIMIT_CFG_2;   /* addr: 0x00008 */ 
  FE_TOP_L2E_GE_VLAN_MAC_LIMIT_CFG_3_t     L2E_GE_VLAN_MAC_LIMIT_CFG_3;   /* addr: 0x00009 */ 
  FE_TOP_L2E_LEARN_STS_0_t                 L2E_LEARN_STS_0;               /* addr: 0x0000a */ 
  FE_TOP_L2E_LEARN_STS_1_t                 L2E_LEARN_STS_1;               /* addr: 0x0000b */ 
  FE_TOP_L2E_LEARN_STS_2_t                 L2E_LEARN_STS_2;               /* addr: 0x0000c */ 
  FE_TOP_L2E_AGING_STS_t                   L2E_AGING_STS;                 /* addr: 0x0000d */ 
  FE_TOP_L2E_STN_MOV_IDX_STS_t             L2E_STN_MOV_IDX_STS;           /* addr: 0x0000e */ 
  FE_TOP_L2E_TABLE_MEM_ACCESS_t            L2E_TABLE_MEM_ACCESS;          /* addr: 0x0000f */ 
  FE_TOP_L2E_TABLE_MEM_DATA2_t             L2E_TABLE_MEM_DATA2;           /* addr: 0x00010 */ 
  FE_TOP_L2E_TABLE_MEM_DATA1_t             L2E_TABLE_MEM_DATA1;           /* addr: 0x00011 */ 
  FE_TOP_L2E_TABLE_MEM_DATA0_t             L2E_TABLE_MEM_DATA0;           /* addr: 0x00012 */ 
  FE_TOP_GE_PORT_CTRL_0_t                  GE_PORT_CTRL_0;                /* addr: 0x00013 */ 
  FE_TOP_GE_PORT_CTRL_1_t                  GE_PORT_CTRL_1;                /* addr: 0x00014 */ 
  FE_TOP_GE_PORT_CTRL_2_t                  GE_PORT_CTRL_2;                /* addr: 0x00015 */ 
  FE_TOP_GE_PORT_DFLT_CFG_0_t              GE_PORT_DFLT_CFG_0;            /* addr: 0x00016 */ 
  FE_TOP_GE_PORT_DFLT_CFG_1_t              GE_PORT_DFLT_CFG_1;            /* addr: 0x00017 */ 
  FE_TOP_PON_PORT_CTRL_0_t                 PON_PORT_CTRL_0;               /* addr: 0x00018 */ 
  FE_TOP_PON_PORT_CTRL_1_t                 PON_PORT_CTRL_1;               /* addr: 0x00019 */ 
  FE_TOP_PON_PORT_CTRL_2_t                 PON_PORT_CTRL_2;               /* addr: 0x0001a */ 
  FE_TOP_PON_PORT_DFLT_CFG_0_t             PON_PORT_DFLT_CFG_0;           /* addr: 0x0001b */ 
  FE_TOP_PON_PORT_DFLT_CFG_1_t             PON_PORT_DFLT_CFG_1;           /* addr: 0x0001c */ 
  FE_TOP_CPU_PORT_CTRL_0_t                 CPU_PORT_CTRL_0;               /* addr: 0x0001d */ 
  FE_TOP_CPU_PORT_CTRL_1_t                 CPU_PORT_CTRL_1;               /* addr: 0x0001e */ 
  FE_TOP_CPU_PORT_CTRL_2_t                 CPU_PORT_CTRL_2;               /* addr: 0x0001f */ 
  FE_TOP_CPU_PORT_DFLT_CFG_0_t             CPU_PORT_DFLT_CFG_0;           /* addr: 0x00020 */ 
  FE_TOP_CPU_PORT_DFLT_CFG_1_t             CPU_PORT_DFLT_CFG_1;           /* addr: 0x00021 */ 
  FE_TOP_MII0_PORT_CTRL_0_t                MII0_PORT_CTRL_0;              /* addr: 0x00022 */ 
  FE_TOP_MII0_PORT_CTRL_1_t                MII0_PORT_CTRL_1;              /* addr: 0x00023 */ 
  FE_TOP_MII0_PORT_CTRL_2_t                MII0_PORT_CTRL_2;              /* addr: 0x00024 */ 
  FE_TOP_MII0_PORT_DFLT_CFG_0_t            MII0_PORT_DFLT_CFG_0;          /* addr: 0x00025 */ 
  FE_TOP_MII0_PORT_DFLT_CFG_1_t            MII0_PORT_DFLT_CFG_1;          /* addr: 0x00026 */ 
  FE_TOP_MII1_PORT_CTRL_0_t                MII1_PORT_CTRL_0;              /* addr: 0x00027 */ 
  FE_TOP_MII1_PORT_CTRL_1_t                MII1_PORT_CTRL_1;              /* addr: 0x00028 */ 
  FE_TOP_MII1_PORT_CTRL_2_t                MII1_PORT_CTRL_2;              /* addr: 0x00029 */ 
  FE_TOP_MII1_PORT_DFLT_CFG_0_t            MII1_PORT_DFLT_CFG_0;          /* addr: 0x0002a */ 
  FE_TOP_MII1_PORT_DFLT_CFG_1_t            MII1_PORT_DFLT_CFG_1;          /* addr: 0x0002b */ 
  FE_TOP_MISC_CTRL_t                       MISC_CTRL;                     /* addr: 0x0002c */ 
  FE_TOP_STS_INTENABLE_t                   STS_INTENABLE;                 /* addr: 0x0002d */ 
  FE_TOP_STS_INTENABLE_1_t                 STS_INTENABLE_1;               /* addr: 0x0002e */ 
  FE_TOP_STS_INTERRUPT_t                   STS_INTERRUPT;                 /* addr: 0x0002f */ 
  FE_TOP_STS_INTERRUPT_1_t                 STS_INTERRUPT_1;               /* addr: 0x00030 */ 
  FE_TOP_PRT_SP_PKT_TABLE_ACCESS_t         PRT_SP_PKT_TABLE_ACCESS;       /* addr: 0x00031 */ 
  FE_TOP_PRT_SP_PKT_TABLE_DATA_t           PRT_SP_PKT_TABLE_DATA;         /* addr: 0x00032 */ 
  FE_TOP_MIRROR_CTRL_t                     MIRROR_CTRL;                   /* addr: 0x00033 */ 
  FE_TOP_MIR_0_DROP_CNT_t                  MIR_0_DROP_CNT;                /* addr: 0x00034 */ 
  FE_TOP_MIR_1_DROP_CNT_t                  MIR_1_DROP_CNT;                /* addr: 0x00035 */ 
  FE_TOP_MIR_2_DROP_CNT_t                  MIR_2_DROP_CNT;                /* addr: 0x00036 */ 
} FE_TOP_t;

  

/*
 * le[0..2]
 */
  



#define FE_LE_ENGINE_CTRL                        0x06100
#define FE_LE_ENGINE_CTRL_dft                    0x00bcc003




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 dbg_ce_max_entry     :  6 ; /* bits 23:18 */
    cs_uint32 top_vlan_l2_en       :  1 ; /* bits 17:17 */
    cs_uint32 dbg_ingr_vlan_map_en :  1 ; /* bits 16:16 */
    cs_uint32 l2_loop_loop_deny    :  1 ; /* bits 15:15 */
    cs_uint32 ce_msk_cfi_flg       :  1 ; /* bits 14:14 */
    cs_uint32 mac_adr_loop_chk_en  :  1 ; /* bits 13:13 */
    cs_uint32 loop_vlan_chk_en     :  1 ; /* bits 12:12 */
    cs_uint32 loop_deny            :  1 ; /* bits 11:11 */
    cs_uint32 tag_filter_deny      :  5 ; /* bits 10:6 */
    cs_uint32 parser_tpid_valid    :  5 ; /* bits 5:1 */
    cs_uint32 ce_tos_ecn_msk_en    :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ce_tos_ecn_msk_en    :  1 ; /* bits 0:0 */
    cs_uint32 parser_tpid_valid    :  5 ; /* bits 5:1 */
    cs_uint32 tag_filter_deny      :  5 ; /* bits 10:6 */
    cs_uint32 loop_deny            :  1 ; /* bits 11:11 */
    cs_uint32 loop_vlan_chk_en     :  1 ; /* bits 12:12 */
    cs_uint32 mac_adr_loop_chk_en  :  1 ; /* bits 13:13 */
    cs_uint32 ce_msk_cfi_flg       :  1 ; /* bits 14:14 */
    cs_uint32 l2_loop_loop_deny    :  1 ; /* bits 15:15 */
    cs_uint32 dbg_ingr_vlan_map_en :  1 ; /* bits 16:16 */
    cs_uint32 top_vlan_l2_en       :  1 ; /* bits 17:17 */
    cs_uint32 dbg_ce_max_entry     :  6 ; /* bits 23:18 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_ENGINE_CTRL_t;





#define FE_LE_ENGINE_TPID_CFG_0                  0x06101
#define FE_LE_ENGINE_TPID_CFG_0_dft              0x81008100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ingr_inner_tpid      : 16 ; /* bits 31:16 */
    cs_uint32 ingr_outer_tpid1     : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ingr_outer_tpid1     : 16 ; /* bits 15:0 */
    cs_uint32 ingr_inner_tpid      : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_ENGINE_TPID_CFG_0_t;





#define FE_LE_ENGINE_TPID_CFG_1                  0x06102
#define FE_LE_ENGINE_TPID_CFG_1_dft              0x00009100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 ingr_outer_tpid2     : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ingr_outer_tpid2     : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_ENGINE_TPID_CFG_1_t;





#define FE_LE_ENGINE_INGRESS_PKT_CNT             0x06103
#define FE_LE_ENGINE_INGRESS_PKT_CNT_dft         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pkt_cnt              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_cnt              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_ENGINE_INGRESS_PKT_CNT_t;





#define FE_LE_ENGINE_EGRESS_PKT_CNT              0x06104
#define FE_LE_ENGINE_EGRESS_PKT_CNT_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pkt_cnt              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_cnt              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_ENGINE_EGRESS_PKT_CNT_t;





#define FE_LE_PP_PARSER_CTRL                     0x06105
#define FE_LE_PP_PARSER_CTRL_dft                 0x00001080




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 19 ;
    cs_uint32 extr_ipv6_en         :  1 ; /* bits 12:12 */
    cs_uint32 spcl_hdr_mode_sel    :  1 ; /* bits 11:11 */
    cs_uint32 spcl_hdr_after_l2hdr_en :  1 ; /* bits 10:10 */
    cs_uint32 spcl_hdr_after_iphdr_en :  1 ; /* bits 9:9 */
    cs_uint32 spcl_hdr_after_tcphdr_en :  1 ; /* bits 8:8 */
    cs_uint32 spcl_hdr_parsing_en  :  1 ; /* bits 7:7 */
    cs_uint32 spcl_glb_hdr_strt_idx :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 spcl_glb_hdr_strt_idx :  7 ; /* bits 6:0 */
    cs_uint32 spcl_hdr_parsing_en  :  1 ; /* bits 7:7 */
    cs_uint32 spcl_hdr_after_tcphdr_en :  1 ; /* bits 8:8 */
    cs_uint32 spcl_hdr_after_iphdr_en :  1 ; /* bits 9:9 */
    cs_uint32 spcl_hdr_after_l2hdr_en :  1 ; /* bits 10:10 */
    cs_uint32 spcl_hdr_mode_sel    :  1 ; /* bits 11:11 */
    cs_uint32 extr_ipv6_en         :  1 ; /* bits 12:12 */
    cs_uint32 rsrvd1               : 19 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_PARSER_CTRL_t;





#define FE_LE_PP_SPCL_EV2PT_CFG_0                0x06106
#define FE_LE_PP_SPCL_EV2PT_CFG_0_dft            0x08010802




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ev2pt_1              : 16 ; /* bits 31:16 */
    cs_uint32 ev2pt_0              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ev2pt_0              : 16 ; /* bits 15:0 */
    cs_uint32 ev2pt_1              : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_SPCL_EV2PT_CFG_0_t;





#define FE_LE_PP_SPCL_EV2PT_CFG_1                0x06107
#define FE_LE_PP_SPCL_EV2PT_CFG_1_dft            0x08030804




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ev2pt_3              : 16 ; /* bits 31:16 */
    cs_uint32 ev2pt_2              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ev2pt_2              : 16 ; /* bits 15:0 */
    cs_uint32 ev2pt_3              : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_SPCL_EV2PT_CFG_1_t;





#define FE_LE_PP_SPCL_EV2PT_CFG_2                0x06108
#define FE_LE_PP_SPCL_EV2PT_CFG_2_dft            0x00000805




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 ev2pt_4              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ev2pt_4              : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_SPCL_EV2PT_CFG_2_t;





#define FE_LE_PP_SPCL_PKT_MAC_0_ADDR1            0x06109
#define FE_LE_PP_SPCL_PKT_MAC_0_ADDR1_dft        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 mac_cmp_en           :  1 ; /* bits 16:16 */
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
    cs_uint32 mac_cmp_en           :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_SPCL_PKT_MAC_0_ADDR1_t;





#define FE_LE_PP_SPCL_PKT_MAC_0_ADDR0            0x0610a
#define FE_LE_PP_SPCL_PKT_MAC_0_ADDR0_dft        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_SPCL_PKT_MAC_0_ADDR0_t;





#define FE_LE_PP_SPCL_PKT_MAC_1_ADDR1            0x0610b
#define FE_LE_PP_SPCL_PKT_MAC_1_ADDR1_dft        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 mac_cmp_en           :  1 ; /* bits 16:16 */
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
    cs_uint32 mac_cmp_en           :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_SPCL_PKT_MAC_1_ADDR1_t;





#define FE_LE_PP_SPCL_PKT_MAC_1_ADDR0            0x0610c
#define FE_LE_PP_SPCL_PKT_MAC_1_ADDR0_dft        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_SPCL_PKT_MAC_1_ADDR0_t;





#define FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR1        0x0610d
#define FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR1_dft    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR1_t;





#define FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR0        0x0610e
#define FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR0_dft    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR0_t;





#define FE_LE_PP_MAC_DA_RANGE_HIGH1              0x0610f
#define FE_LE_PP_MAC_DA_RANGE_HIGH1_dft          0x00003333




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_MAC_DA_RANGE_HIGH1_t;





#define FE_LE_PP_MAC_DA_RANGE_HIGH0              0x06110
#define FE_LE_PP_MAC_DA_RANGE_HIGH0_dft          0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_MAC_DA_RANGE_HIGH0_t;





#define FE_LE_PP_MAC_DA_RANGE_LOW1               0x06111
#define FE_LE_PP_MAC_DA_RANGE_LOW1_dft           0x00003333




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_MAC_DA_RANGE_LOW1_t;





#define FE_LE_PP_MAC_DA_RANGE_LOW0               0x06112
#define FE_LE_PP_MAC_DA_RANGE_LOW0_dft           0x00010000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_addr             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_MAC_DA_RANGE_LOW0_t;





#define FE_LE_PP_NI_INTERFACE_CAPTURE_0          0x06113
#define FE_LE_PP_NI_INTERFACE_CAPTURE_0_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_NI_INTERFACE_CAPTURE_0_t;





#define FE_LE_PP_NI_INTERFACE_CAPTURE_1          0x06114
#define FE_LE_PP_NI_INTERFACE_CAPTURE_1_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_NI_INTERFACE_CAPTURE_1_t;





#define FE_LE_PP_PR_INTERFACE_CAPTURE_2          0x06115
#define FE_LE_PP_PR_INTERFACE_CAPTURE_2_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_PR_INTERFACE_CAPTURE_2_t;





#define FE_LE_PP_PR_INTERFACE_CAPTURE_3          0x06116
#define FE_LE_PP_PR_INTERFACE_CAPTURE_3_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PP_PR_INTERFACE_CAPTURE_3_t;





#define FE_LE_VE_EGRESS_VLAN_TBL_VLD_CFG         0x06117
#define FE_LE_VE_EGRESS_VLAN_TBL_VLD_CFG_dft     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 egress_vld           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 egress_vld           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_VE_EGRESS_VLAN_TBL_VLD_CFG_t;





#define FE_LE_VE_INGRESS_VLAN_TBL_VLD_CFG        0x06118
#define FE_LE_VE_INGRESS_VLAN_TBL_VLD_CFG_dft    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ingress_vld          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ingress_vld          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_VE_INGRESS_VLAN_TBL_VLD_CFG_t;





#define FE_LE_VE_MAP_VLAN_TBL_CFG                0x06119
#define FE_LE_VE_MAP_VLAN_TBL_CFG_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ingress_vld          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ingress_vld          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_VE_MAP_VLAN_TBL_CFG_t;





#define FE_LE_VE_VLAN_TABLE_MEM_ACCESS           0x0611a
#define FE_LE_VE_VLAN_TABLE_MEM_ACCESS_dft       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 26 ;
    cs_uint32 address              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 address              :  4 ; /* bits 3:0 */
    cs_uint32 rsrvd1               : 26 ;
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_VE_VLAN_TABLE_MEM_ACCESS_t;





#define FE_LE_VE_VLAN_TABLE_MEM_DATA3            0x0611b
#define FE_LE_VE_VLAN_TABLE_MEM_DATA3_dft        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 e3_permit_mii1       :  1 ; /* bits 15:15 */
    cs_uint32 e3_permit_mii0       :  1 ; /* bits 14:14 */
    cs_uint32 e3_permit_pon        :  1 ; /* bits 13:13 */
    cs_uint32 e3_permit_ge         :  1 ; /* bits 12:12 */
    cs_uint32 e3_vlanid_hi         : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 e3_vlanid_hi         : 12 ; /* bits 11:0 */
    cs_uint32 e3_permit_ge         :  1 ; /* bits 12:12 */
    cs_uint32 e3_permit_pon        :  1 ; /* bits 13:13 */
    cs_uint32 e3_permit_mii0       :  1 ; /* bits 14:14 */
    cs_uint32 e3_permit_mii1       :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_VE_VLAN_TABLE_MEM_DATA3_t;





#define FE_LE_VE_VLAN_TABLE_MEM_DATA2            0x0611c
#define FE_LE_VE_VLAN_TABLE_MEM_DATA2_dft        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 e3_vlanid_lo         : 12 ; /* bits 31:20 */
    cs_uint32 e2_permit_mii1       :  1 ; /* bits 19:19 */
    cs_uint32 e2_permit_mii0       :  1 ; /* bits 18:18 */
    cs_uint32 e2_permit_pon        :  1 ; /* bits 17:17 */
    cs_uint32 e2_permit_ge         :  1 ; /* bits 16:16 */
    cs_uint32 e2_vlanid_hi         : 12 ; /* bits 15:4 */
    cs_uint32 e2_vlanid_lo         :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 e2_vlanid_lo         :  4 ; /* bits 3:0 */
    cs_uint32 e2_vlanid_hi         : 12 ; /* bits 15:4 */
    cs_uint32 e2_permit_ge         :  1 ; /* bits 16:16 */
    cs_uint32 e2_permit_pon        :  1 ; /* bits 17:17 */
    cs_uint32 e2_permit_mii0       :  1 ; /* bits 18:18 */
    cs_uint32 e2_permit_mii1       :  1 ; /* bits 19:19 */
    cs_uint32 e3_vlanid_lo         : 12 ; /* bits 31:20 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_VE_VLAN_TABLE_MEM_DATA2_t;





#define FE_LE_VE_VLAN_TABLE_MEM_DATA1            0x0611d
#define FE_LE_VE_VLAN_TABLE_MEM_DATA1_dft        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 e2_vlanid_lo         :  8 ; /* bits 31:24 */
    cs_uint32 e1_permit_mii1       :  1 ; /* bits 23:23 */
    cs_uint32 e1_permit_mii0       :  1 ; /* bits 22:22 */
    cs_uint32 e1_permit_pon        :  1 ; /* bits 21:21 */
    cs_uint32 e1_permit_ge         :  1 ; /* bits 20:20 */
    cs_uint32 e1_vlanid_hi         : 12 ; /* bits 19:8 */
    cs_uint32 e1_vlanid_lo         :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 e1_vlanid_lo         :  8 ; /* bits 7:0 */
    cs_uint32 e1_vlanid_hi         : 12 ; /* bits 19:8 */
    cs_uint32 e1_permit_ge         :  1 ; /* bits 20:20 */
    cs_uint32 e1_permit_pon        :  1 ; /* bits 21:21 */
    cs_uint32 e1_permit_mii0       :  1 ; /* bits 22:22 */
    cs_uint32 e1_permit_mii1       :  1 ; /* bits 23:23 */
    cs_uint32 e2_vlanid_lo         :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_VE_VLAN_TABLE_MEM_DATA1_t;





#define FE_LE_VE_VLAN_TABLE_MEM_DATA0            0x0611e
#define FE_LE_VE_VLAN_TABLE_MEM_DATA0_dft        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 e1_vlanid_lo         :  4 ; /* bits 31:28 */
    cs_uint32 e0_permit_mii1       :  1 ; /* bits 27:27 */
    cs_uint32 e0_permit_mii0       :  1 ; /* bits 26:26 */
    cs_uint32 e0_permit_pon        :  1 ; /* bits 25:25 */
    cs_uint32 e0_permit_ge         :  1 ; /* bits 24:24 */
    cs_uint32 e0_vlanid_hi         : 12 ; /* bits 23:12 */
    cs_uint32 e0_vlanid_lo         : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 e0_vlanid_lo         : 12 ; /* bits 11:0 */
    cs_uint32 e0_vlanid_hi         : 12 ; /* bits 23:12 */
    cs_uint32 e0_permit_ge         :  1 ; /* bits 24:24 */
    cs_uint32 e0_permit_pon        :  1 ; /* bits 25:25 */
    cs_uint32 e0_permit_mii0       :  1 ; /* bits 26:26 */
    cs_uint32 e0_permit_mii1       :  1 ; /* bits 27:27 */
    cs_uint32 e1_vlanid_lo         :  4 ; /* bits 31:28 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_VE_VLAN_TABLE_MEM_DATA0_t;





#define FE_LE_CE_CTRL                            0x0611f
#define FE_LE_CE_CTRL_dft                        0x0000002f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  9 ;
    cs_uint32 mac_filter_aging_en  :  1 ; /* bits 22:22 */
    cs_uint32 deny_no_type15_match :  1 ; /* bits 21:21 */
    cs_uint32 deny_no_type14_match :  1 ; /* bits 20:20 */
    cs_uint32 deny_no_type13_match :  1 ; /* bits 19:19 */
    cs_uint32 deny_no_type12_match :  1 ; /* bits 18:18 */
    cs_uint32 deny_no_type11_match :  1 ; /* bits 17:17 */
    cs_uint32 deny_no_type10_match :  1 ; /* bits 16:16 */
    cs_uint32 deny_no_type9_match  :  1 ; /* bits 15:15 */
    cs_uint32 deny_no_type8_match  :  1 ; /* bits 14:14 */
    cs_uint32 deny_no_type7_match  :  1 ; /* bits 13:13 */
    cs_uint32 deny_no_type6_match  :  1 ; /* bits 12:12 */
    cs_uint32 deny_no_type5_match  :  1 ; /* bits 11:11 */
    cs_uint32 deny_no_type4_match  :  1 ; /* bits 10:10 */
    cs_uint32 deny_no_type3_match  :  1 ; /* bits 9:9 */
    cs_uint32 deny_no_type2_match  :  1 ; /* bits 8:8 */
    cs_uint32 deny_no_type1_match  :  1 ; /* bits 7:7 */
    cs_uint32 deny_no_type0_match  :  1 ; /* bits 6:6 */
    cs_uint32 cl_start_entry       :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cl_start_entry       :  6 ; /* bits 5:0 */
    cs_uint32 deny_no_type0_match  :  1 ; /* bits 6:6 */
    cs_uint32 deny_no_type1_match  :  1 ; /* bits 7:7 */
    cs_uint32 deny_no_type2_match  :  1 ; /* bits 8:8 */
    cs_uint32 deny_no_type3_match  :  1 ; /* bits 9:9 */
    cs_uint32 deny_no_type4_match  :  1 ; /* bits 10:10 */
    cs_uint32 deny_no_type5_match  :  1 ; /* bits 11:11 */
    cs_uint32 deny_no_type6_match  :  1 ; /* bits 12:12 */
    cs_uint32 deny_no_type7_match  :  1 ; /* bits 13:13 */
    cs_uint32 deny_no_type8_match  :  1 ; /* bits 14:14 */
    cs_uint32 deny_no_type9_match  :  1 ; /* bits 15:15 */
    cs_uint32 deny_no_type10_match :  1 ; /* bits 16:16 */
    cs_uint32 deny_no_type11_match :  1 ; /* bits 17:17 */
    cs_uint32 deny_no_type12_match :  1 ; /* bits 18:18 */
    cs_uint32 deny_no_type13_match :  1 ; /* bits 19:19 */
    cs_uint32 deny_no_type14_match :  1 ; /* bits 20:20 */
    cs_uint32 deny_no_type15_match :  1 ; /* bits 21:21 */
    cs_uint32 mac_filter_aging_en  :  1 ; /* bits 22:22 */
    cs_uint32 rsrvd1               :  9 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_CTRL_t;





#define FE_LE_CE_KEY_VLD1                        0x06120
#define FE_LE_CE_KEY_VLD1_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 cl_entry_vld         : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cl_entry_vld         : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_VLD1_t;





#define FE_LE_CE_KEY_VLD0                        0x06121
#define FE_LE_CE_KEY_VLD0_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cl_entry_vld         : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cl_entry_vld         : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_VLD0_t;





#define FE_LE_CE_RULE_MODE_CTRL1                 0x06122
#define FE_LE_CE_RULE_MODE_CTRL1_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 cl_rule_or_en        : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cl_rule_or_en        : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_RULE_MODE_CTRL1_t;





#define FE_LE_CE_RULE_MODE_CTRL0                 0x06123
#define FE_LE_CE_RULE_MODE_CTRL0_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cl_rule_or_en        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cl_rule_or_en        : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_RULE_MODE_CTRL0_t;





#define FE_LE_CE_MAC_FLTR_AGNG_TMR_CFG           0x06124
#define FE_LE_CE_MAC_FLTR_AGNG_TMR_CFG_dft       0x000007a2




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 aging_timer          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 aging_timer          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_MAC_FLTR_AGNG_TMR_CFG_t;





#define FE_LE_CE_L4_CTRL                         0x06125
#define FE_LE_CE_L4_CTRL_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tcp_dp_chk_vld       :  8 ; /* bits 31:24 */
    cs_uint32 udp_dp_chk_vld       :  8 ; /* bits 23:16 */
    cs_uint32 tcp_sp_chk_vld       :  8 ; /* bits 15:8 */
    cs_uint32 udp_sp_chk_vld       :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 udp_sp_chk_vld       :  8 ; /* bits 7:0 */
    cs_uint32 tcp_sp_chk_vld       :  8 ; /* bits 15:8 */
    cs_uint32 udp_dp_chk_vld       :  8 ; /* bits 23:16 */
    cs_uint32 tcp_dp_chk_vld       :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_L4_CTRL_t;





#define FE_LE_CE_MAC_FILTER_AGING_STS            0x06126
#define FE_LE_CE_MAC_FILTER_AGING_STS_dft        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 26 ;
    cs_uint32 aging_index          :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 aging_index          :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 26 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_MAC_FILTER_AGING_STS_t;





#define FE_LE_CE_MAC_FLTR_AGNG_VLD_STS1          0x06127
#define FE_LE_CE_MAC_FLTR_AGNG_VLD_STS1_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 vld                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 vld                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_MAC_FLTR_AGNG_VLD_STS1_t;





#define FE_LE_CE_MAC_FLTR_AGNG_VLD_STS0          0x06128
#define FE_LE_CE_MAC_FLTR_AGNG_VLD_STS0_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 vld                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 vld                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_MAC_FLTR_AGNG_VLD_STS0_t;





#define FE_LE_CE_DEBUG_STS                       0x06129
#define FE_LE_CE_DEBUG_STS_dft                   0x07ffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 hit_index_2          :  9 ; /* bits 26:18 */
    cs_uint32 hit_index_1          :  9 ; /* bits 17:9 */
    cs_uint32 hit_index_0          :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 hit_index_0          :  9 ; /* bits 8:0 */
    cs_uint32 hit_index_1          :  9 ; /* bits 17:9 */
    cs_uint32 hit_index_2          :  9 ; /* bits 26:18 */
    cs_uint32 rsrvd1               :  5 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_DEBUG_STS_t;





#define FE_LE_CE_DEBUG_ENT_HIT_CNT_ADR_CFG       0x0612a
#define FE_LE_CE_DEBUG_ENT_HIT_CNT_ADR_CFG_dft   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 26 ;
    cs_uint32 key_addr             :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key_addr             :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 26 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_DEBUG_ENT_HIT_CNT_ADR_CFG_t;





#define FE_LE_CE_DEBUG_ENT_HIT_CNT               0x0612b
#define FE_LE_CE_DEBUG_ENT_HIT_CNT_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 hit_cnt              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 hit_cnt              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_DEBUG_ENT_HIT_CNT_t;





#define FE_LE_CE_DEBUG_PKT_STS_0                 0x0612c
#define FE_LE_CE_DEBUG_PKT_STS_0_dft             0x07ffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 vlan_rslt_addr       :  9 ; /* bits 26:18 */
    cs_uint32 dscp_rslt_addr       :  9 ; /* bits 17:9 */
    cs_uint32 dpid_rslt_addr       :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dpid_rslt_addr       :  9 ; /* bits 8:0 */
    cs_uint32 dscp_rslt_addr       :  9 ; /* bits 17:9 */
    cs_uint32 vlan_rslt_addr       :  9 ; /* bits 26:18 */
    cs_uint32 rsrvd1               :  5 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_DEBUG_PKT_STS_0_t;





#define FE_LE_CE_DEBUG_PKT_STS_1                 0x0612d
#define FE_LE_CE_DEBUG_PKT_STS_1_dft             0x07ffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 cos_rslt_addr        :  9 ; /* bits 26:18 */
    cs_uint32 l2_lrn_rslt_addr     :  9 ; /* bits 17:9 */
    cs_uint32 dot1p_rslt_addr      :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dot1p_rslt_addr      :  9 ; /* bits 8:0 */
    cs_uint32 l2_lrn_rslt_addr     :  9 ; /* bits 17:9 */
    cs_uint32 cos_rslt_addr        :  9 ; /* bits 26:18 */
    cs_uint32 rsrvd1               :  5 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_DEBUG_PKT_STS_1_t;





#define FE_LE_CE_DEBUG_PKT_STS_2                 0x0612e
#define FE_LE_CE_DEBUG_PKT_STS_2_dft             0x0003ffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 permit_priority_rslt_addr :  9 ; /* bits 17:9 */
    cs_uint32 rate_lmt_rslt_addr   :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rate_lmt_rslt_addr   :  9 ; /* bits 8:0 */
    cs_uint32 permit_priority_rslt_addr :  9 ; /* bits 17:9 */
    cs_uint32 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_DEBUG_PKT_STS_2_t;





#define FE_LE_CE_DEBUG_ENT_3_0_HIT_CNT           0x0612f
#define FE_LE_CE_DEBUG_ENT_3_0_HIT_CNT_dft       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 hit_cnt_3            :  8 ; /* bits 31:24 */
    cs_uint32 hit_cnt_2            :  8 ; /* bits 23:16 */
    cs_uint32 hit_cnt_1            :  8 ; /* bits 15:8 */
    cs_uint32 hit_cnt_0            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 hit_cnt_0            :  8 ; /* bits 7:0 */
    cs_uint32 hit_cnt_1            :  8 ; /* bits 15:8 */
    cs_uint32 hit_cnt_2            :  8 ; /* bits 23:16 */
    cs_uint32 hit_cnt_3            :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_DEBUG_ENT_3_0_HIT_CNT_t;





#define FE_LE_CE_DEBUG_ENT_7_4_HIT_CNT           0x06130
#define FE_LE_CE_DEBUG_ENT_7_4_HIT_CNT_dft       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 hit_cnt_3            :  8 ; /* bits 31:24 */
    cs_uint32 hit_cnt_2            :  8 ; /* bits 23:16 */
    cs_uint32 hit_cnt_1            :  8 ; /* bits 15:8 */
    cs_uint32 hit_cnt_0            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 hit_cnt_0            :  8 ; /* bits 7:0 */
    cs_uint32 hit_cnt_1            :  8 ; /* bits 15:8 */
    cs_uint32 hit_cnt_2            :  8 ; /* bits 23:16 */
    cs_uint32 hit_cnt_3            :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_DEBUG_ENT_7_4_HIT_CNT_t;





#define FE_LE_CE_DEBUG_ENT_11_8_HIT_CNT          0x06131
#define FE_LE_CE_DEBUG_ENT_11_8_HIT_CNT_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 hit_cnt_3            :  8 ; /* bits 31:24 */
    cs_uint32 hit_cnt_2            :  8 ; /* bits 23:16 */
    cs_uint32 hit_cnt_1            :  8 ; /* bits 15:8 */
    cs_uint32 hit_cnt_0            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 hit_cnt_0            :  8 ; /* bits 7:0 */
    cs_uint32 hit_cnt_1            :  8 ; /* bits 15:8 */
    cs_uint32 hit_cnt_2            :  8 ; /* bits 23:16 */
    cs_uint32 hit_cnt_3            :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_DEBUG_ENT_11_8_HIT_CNT_t;





#define FE_LE_CE_DEBUG_ENT_15_12_HIT_CNT         0x06132
#define FE_LE_CE_DEBUG_ENT_15_12_HIT_CNT_dft     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 hit_cnt_3            :  8 ; /* bits 31:24 */
    cs_uint32 hit_cnt_2            :  8 ; /* bits 23:16 */
    cs_uint32 hit_cnt_1            :  8 ; /* bits 15:8 */
    cs_uint32 hit_cnt_0            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 hit_cnt_0            :  8 ; /* bits 7:0 */
    cs_uint32 hit_cnt_1            :  8 ; /* bits 15:8 */
    cs_uint32 hit_cnt_2            :  8 ; /* bits 23:16 */
    cs_uint32 hit_cnt_3            :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_DEBUG_ENT_15_12_HIT_CNT_t;





#define FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_ACCESS 0x06133
#define FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_ACCESS_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 address              :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 address              :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_ACCESS_t;





#define FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_DATA  0x06134
#define FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_DATA_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 26 ;
    cs_uint32 cl_next_entry        :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cl_next_entry        :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 26 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_DATA_t;





#define FE_LE_CE_KEY_MEM_ACCESS                  0x06135
#define FE_LE_CE_KEY_MEM_ACCESS_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 address              :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 address              :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_MEM_ACCESS_t;





#define FE_LE_CE_KEY_MEM_DATA9                   0x06136
#define FE_LE_CE_KEY_MEM_DATA9_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 kt0_rsvd             :  4 ; /* bits 27:24 */
    cs_uint32 kt0_msk_spid_fld     :  1 ; /* bits 23:23 */
    cs_uint32 kt0_msk_p_tag_fld    :  1 ; /* bits 22:22 */
    cs_uint32 kt0_msk_tpid_enc_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt0_msk_igmp_type_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt0_msk_icmp_type_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt0_msk_tcp_flg_fld  :  1 ; /* bits 18:18 */
    cs_uint32 kt0_msk_l4_port_fld  :  1 ; /* bits 17:17 */
    cs_uint32 kt0_msk_ip_opt_flg_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt0_msk_dscp_fld     :  1 ; /* bits 15:15 */
    cs_uint32 kt0_msk_ippt_fld     :  1 ; /* bits 14:14 */
    cs_uint32 kt0_msk_ip_sa_fld    :  6 ; /* bits 13:8 */
    cs_uint32 kt0_msk_ip_da_fld    :  6 ; /* bits 7:2 */
    cs_uint32 kt0_msk_vlan_cnt_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt0_msk_prsr_len_excd_fld :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt0_msk_prsr_len_excd_fld :  1 ; /* bits 0:0 */
    cs_uint32 kt0_msk_vlan_cnt_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt0_msk_ip_da_fld    :  6 ; /* bits 7:2 */
    cs_uint32 kt0_msk_ip_sa_fld    :  6 ; /* bits 13:8 */
    cs_uint32 kt0_msk_ippt_fld     :  1 ; /* bits 14:14 */
    cs_uint32 kt0_msk_dscp_fld     :  1 ; /* bits 15:15 */
    cs_uint32 kt0_msk_ip_opt_flg_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt0_msk_l4_port_fld  :  1 ; /* bits 17:17 */
    cs_uint32 kt0_msk_tcp_flg_fld  :  1 ; /* bits 18:18 */
    cs_uint32 kt0_msk_icmp_type_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt0_msk_igmp_type_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt0_msk_tpid_enc_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt0_msk_p_tag_fld    :  1 ; /* bits 22:22 */
    cs_uint32 kt0_msk_spid_fld     :  1 ; /* bits 23:23 */
    cs_uint32 kt0_rsvd             :  4 ; /* bits 27:24 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf0 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 kt2_0_rsvd           : 22 ; /* bits 26:5 */
    cs_uint32 kt2_0_msk_ipv6_flow_label_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt2_0_ipv6_flow_label :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_0_ipv6_flow_label :  4 ; /* bits 3:0 */
    cs_uint32 kt2_0_msk_ipv6_flow_label_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt2_0_rsvd           : 22 ; /* bits 26:5 */
    cs_uint32 rsrvd1               :  5 ;
#   endif
  } bf1 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 kt2_rsvd             :  1 ; /* bits 27:27 */
    cs_uint32 kt2_1_rsvd           :  3 ; /* bits 26:24 */
    cs_uint32 kt2_1_msk_spid_fld   :  1 ; /* bits 23:23 */
    cs_uint32 kt2_1_msk_p_tag_fld  :  1 ; /* bits 22:22 */
    cs_uint32 kt2_1_msk_tpid_enc_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt2_1_msk_igmp_type_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt2_1_msk_icmp_type_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt2_1_msk_tcp_flg_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt2_1_msk_l4_port_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt2_1_msk_ip_opt_flg_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt2_1_msk_dscp_fld   :  1 ; /* bits 15:15 */
    cs_uint32 kt2_1_msk_ippt_fld   :  1 ; /* bits 14:14 */
    cs_uint32 kt2_1_rsvd2          :  6 ; /* bits 13:8 */
    cs_uint32 kt2_1_rsvd3          :  6 ; /* bits 7:2 */
    cs_uint32 kt2_1_msk_vlan_cnt_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt2_1_msk_prsr_len_excd_fld :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_1_msk_prsr_len_excd_fld :  1 ; /* bits 0:0 */
    cs_uint32 kt2_1_msk_vlan_cnt_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt2_1_rsvd3          :  6 ; /* bits 7:2 */
    cs_uint32 kt2_1_rsvd2          :  6 ; /* bits 13:8 */
    cs_uint32 kt2_1_msk_ippt_fld   :  1 ; /* bits 14:14 */
    cs_uint32 kt2_1_msk_dscp_fld   :  1 ; /* bits 15:15 */
    cs_uint32 kt2_1_msk_ip_opt_flg_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt2_1_msk_l4_port_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt2_1_msk_tcp_flg_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt2_1_msk_icmp_type_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt2_1_msk_igmp_type_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt2_1_msk_tpid_enc_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt2_1_msk_p_tag_fld  :  1 ; /* bits 22:22 */
    cs_uint32 kt2_1_msk_spid_fld   :  1 ; /* bits 23:23 */
    cs_uint32 kt2_1_rsvd           :  3 ; /* bits 26:24 */
    cs_uint32 kt2_rsvd             :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf2 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 kt1_rsvd             :  8 ; /* bits 27:20 */
    cs_uint32 kt1_0_msk_prsr_len_excd_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt1_0_prsr_len_excd  :  1 ; /* bits 18:18 */
    cs_uint32 kt1_1_msk_prsr_len_excd_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt1_1_prsr_len_excd  :  1 ; /* bits 16:16 */
    cs_uint32 kt1_2_msk_prsr_len_excd_fld :  1 ; /* bits 15:15 */
    cs_uint32 kt1_2_prsr_len_excd  :  1 ; /* bits 14:14 */
    cs_uint32 kt1_3_msk_prsr_len_excd_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt1_3_prsr_len_excd  :  1 ; /* bits 12:12 */
    cs_uint32 kt1_0_msk_dscp_tos_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt1_0_msk_ippt_fld   :  1 ; /* bits 10:10 */
    cs_uint32 kt1_0_msk_l4_port_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt1_0_msk_ip_addr_fld :  6 ; /* bits 8:3 */
    cs_uint32 kt1_0_l4_port_type   :  1 ; /* bits 2:2 */
    cs_uint32 kt1_0_ip_addr_type   :  1 ; /* bits 1:1 */
    cs_uint32 kt1_0_dscp_tos       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt1_0_dscp_tos       :  1 ; /* bits 0:0 */
    cs_uint32 kt1_0_ip_addr_type   :  1 ; /* bits 1:1 */
    cs_uint32 kt1_0_l4_port_type   :  1 ; /* bits 2:2 */
    cs_uint32 kt1_0_msk_ip_addr_fld :  6 ; /* bits 8:3 */
    cs_uint32 kt1_0_msk_l4_port_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt1_0_msk_ippt_fld   :  1 ; /* bits 10:10 */
    cs_uint32 kt1_0_msk_dscp_tos_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt1_3_prsr_len_excd  :  1 ; /* bits 12:12 */
    cs_uint32 kt1_3_msk_prsr_len_excd_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt1_2_prsr_len_excd  :  1 ; /* bits 14:14 */
    cs_uint32 kt1_2_msk_prsr_len_excd_fld :  1 ; /* bits 15:15 */
    cs_uint32 kt1_1_prsr_len_excd  :  1 ; /* bits 16:16 */
    cs_uint32 kt1_1_msk_prsr_len_excd_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt1_0_prsr_len_excd  :  1 ; /* bits 18:18 */
    cs_uint32 kt1_0_msk_prsr_len_excd_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt1_rsvd             :  8 ; /* bits 27:20 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf3 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 kt3_rsvd             : 28 ; /* bits 27:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt3_rsvd             : 28 ; /* bits 27:0 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf4 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 kt4_rsvd             :  4 ; /* bits 27:24 */
    cs_uint32 kt4_7_mirror_pkt_flag :  1 ; /* bits 23:23 */
    cs_uint32 kt4_7_rsvd           : 15 ; /* bits 22:8 */
    cs_uint32 kt4_7_msk_mirror_group_id_fld :  1 ; /* bits 7:7 */
    cs_uint32 kt4_7_mirror_group_id :  5 ; /* bits 6:2 */
    cs_uint32 kt4_7_msk_vlanid_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt4_7_vlanid         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt4_7_vlanid         :  1 ; /* bits 0:0 */
    cs_uint32 kt4_7_msk_vlanid_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt4_7_mirror_group_id :  5 ; /* bits 6:2 */
    cs_uint32 kt4_7_msk_mirror_group_id_fld :  1 ; /* bits 7:7 */
    cs_uint32 kt4_7_rsvd           : 15 ; /* bits 22:8 */
    cs_uint32 kt4_7_mirror_pkt_flag :  1 ; /* bits 23:23 */
    cs_uint32 kt4_rsvd             :  4 ; /* bits 27:24 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf5 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 kt5_rsvd             :  4 ; /* bits 27:24 */
    cs_uint32 kt5_0_age_tmr_offset :  6 ; /* bits 23:18 */
    cs_uint32 kt5_0_msk_802_1p_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt5_0_msk_p_tag_flag_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt5_0_msk_vlan_cnt_fld :  1 ; /* bits 15:15 */
    cs_uint32 kt5_0_msk_vlanid_fld :  1 ; /* bits 14:14 */
    cs_uint32 kt5_0_msk_mac_addr_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt5_0_vl_802_1p      :  3 ; /* bits 12:10 */
    cs_uint32 kt5_0_p_tag_flag     :  1 ; /* bits 9:9 */
    cs_uint32 kt5_0_vlan_cnt       :  2 ; /* bits 8:7 */
    cs_uint32 kt5_0_vlanid         :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt5_0_vlanid         :  7 ; /* bits 6:0 */
    cs_uint32 kt5_0_vlan_cnt       :  2 ; /* bits 8:7 */
    cs_uint32 kt5_0_p_tag_flag     :  1 ; /* bits 9:9 */
    cs_uint32 kt5_0_vl_802_1p      :  3 ; /* bits 12:10 */
    cs_uint32 kt5_0_msk_mac_addr_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt5_0_msk_vlanid_fld :  1 ; /* bits 14:14 */
    cs_uint32 kt5_0_msk_vlan_cnt_fld :  1 ; /* bits 15:15 */
    cs_uint32 kt5_0_msk_p_tag_flag_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt5_0_msk_802_1p_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt5_0_age_tmr_offset :  6 ; /* bits 23:18 */
    cs_uint32 kt5_rsvd             :  4 ; /* bits 27:24 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf6 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 kt6_0_msk_mac_da_range_flag_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt6_0_msk_tpid_enc_fld :  1 ; /* bits 26:26 */
    cs_uint32 kt6_0_msk_ev2pt_fld  :  1 ; /* bits 25:25 */
    cs_uint32 kt6_0_msk_mac_addr_fld :  1 ; /* bits 24:24 */
    cs_uint32 kt6_0_mac_da_range_flag :  1 ; /* bits 23:23 */
    cs_uint32 kt6_0_tpid_enc       :  3 ; /* bits 22:20 */
    cs_uint32 kt6_0_age_tmr_offset :  6 ; /* bits 19:14 */
    cs_uint32 kt6_0_mac_type       :  1 ; /* bits 13:13 */
    cs_uint32 kt6_0_ev2pt          : 13 ; /* bits 12:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt6_0_ev2pt          : 13 ; /* bits 12:0 */
    cs_uint32 kt6_0_mac_type       :  1 ; /* bits 13:13 */
    cs_uint32 kt6_0_age_tmr_offset :  6 ; /* bits 19:14 */
    cs_uint32 kt6_0_tpid_enc       :  3 ; /* bits 22:20 */
    cs_uint32 kt6_0_mac_da_range_flag :  1 ; /* bits 23:23 */
    cs_uint32 kt6_0_msk_mac_addr_fld :  1 ; /* bits 24:24 */
    cs_uint32 kt6_0_msk_ev2pt_fld  :  1 ; /* bits 25:25 */
    cs_uint32 kt6_0_msk_tpid_enc_fld :  1 ; /* bits 26:26 */
    cs_uint32 kt6_0_msk_mac_da_range_flag_fld :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf7 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 kt7_rsvd             :  8 ; /* bits 27:20 */
    cs_uint32 kt7_3_msk_ev2pt_fld  :  1 ; /* bits 19:19 */
    cs_uint32 kt7_3_spl_pkt_vec    : 19 ; /* bits 18:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt7_3_spl_pkt_vec    : 19 ; /* bits 18:0 */
    cs_uint32 kt7_3_msk_ev2pt_fld  :  1 ; /* bits 19:19 */
    cs_uint32 kt7_rsvd             :  8 ; /* bits 27:20 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf8 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 kt8_rsvd             : 28 ; /* bits 27:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt8_rsvd             : 28 ; /* bits 27:0 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf9 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 kt9_rsvd             :  4 ; /* bits 27:24 */
    cs_uint32 kt9_7_msk_ev2pt_enc_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt9_7_ev2pt_enc      :  4 ; /* bits 22:19 */
    cs_uint32 kt9_7_msk_mc_flag_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt9_7_mc_flag        :  1 ; /* bits 17:17 */
    cs_uint32 kt9_7_msk_bc_flag_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt9_7_bc_flag        :  1 ; /* bits 15:15 */
    cs_uint32 kt9_7_inner_vlan_en  :  1 ; /* bits 14:14 */
    cs_uint32 kt9_7_rsvd           :  1 ; /* bits 13:13 */
    cs_uint32 kt9_7_msk_vlan_cnt_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt9_7_vlan_cnt       :  2 ; /* bits 11:10 */
    cs_uint32 kt9_7_msk_vlanid_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt9_7_vlanid_hi      :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt9_7_vlanid_hi      :  9 ; /* bits 8:0 */
    cs_uint32 kt9_7_msk_vlanid_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt9_7_vlan_cnt       :  2 ; /* bits 11:10 */
    cs_uint32 kt9_7_msk_vlan_cnt_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt9_7_rsvd           :  1 ; /* bits 13:13 */
    cs_uint32 kt9_7_inner_vlan_en  :  1 ; /* bits 14:14 */
    cs_uint32 kt9_7_bc_flag        :  1 ; /* bits 15:15 */
    cs_uint32 kt9_7_msk_bc_flag_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt9_7_mc_flag        :  1 ; /* bits 17:17 */
    cs_uint32 kt9_7_msk_mc_flag_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt9_7_ev2pt_enc      :  4 ; /* bits 22:19 */
    cs_uint32 kt9_7_msk_ev2pt_enc_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt9_rsvd             :  4 ; /* bits 27:24 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf10 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 kt10_0_cfi_flg       :  1 ; /* bits 27:27 */
    cs_uint32 kt10_1_cfi_flg       :  1 ; /* bits 26:26 */
    cs_uint32 kt10_2_cfi_flg       :  1 ; /* bits 25:25 */
    cs_uint32 kt10_3_cfi_flg       :  1 ; /* bits 24:24 */
    cs_uint32 kt10_0_msk_prsr_len_excd_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt10_0_prsr_len_excd :  1 ; /* bits 22:22 */
    cs_uint32 kt10_1_msk_prsr_len_excd_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt10_1_prsr_len_excd :  1 ; /* bits 20:20 */
    cs_uint32 kt10_2_msk_prsr_len_excd_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt10_2_prsr_len_excd :  1 ; /* bits 18:18 */
    cs_uint32 kt10_3_msk_prsr_len_excd_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt10_3_prsr_len_excd :  1 ; /* bits 16:16 */
    cs_uint32 kt10_3_msk_tpid_enc_fld :  1 ; /* bits 15:15 */
    cs_uint32 kt10_3_msk_mac_da_flags_fld :  1 ; /* bits 14:14 */
    cs_uint32 kt10_3_msk_ippt_fld  :  1 ; /* bits 13:13 */
    cs_uint32 kt10_3_msk_icmp_type_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt10_3_msk_mac_da_low_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt10_3_msk_ev2pt_fld :  1 ; /* bits 10:10 */
    cs_uint32 kt10_3_tpid_enc      :  3 ; /* bits 9:7 */
    cs_uint32 kt10_3_rsvd_mac_da_flag :  1 ; /* bits 6:6 */
    cs_uint32 kt10_3_mcst_ip_flag  :  1 ; /* bits 5:5 */
    cs_uint32 kt10_3_mymac_flag    :  1 ; /* bits 4:4 */
    cs_uint32 kt10_3_ippt          :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt10_3_ippt          :  4 ; /* bits 3:0 */
    cs_uint32 kt10_3_mymac_flag    :  1 ; /* bits 4:4 */
    cs_uint32 kt10_3_mcst_ip_flag  :  1 ; /* bits 5:5 */
    cs_uint32 kt10_3_rsvd_mac_da_flag :  1 ; /* bits 6:6 */
    cs_uint32 kt10_3_tpid_enc      :  3 ; /* bits 9:7 */
    cs_uint32 kt10_3_msk_ev2pt_fld :  1 ; /* bits 10:10 */
    cs_uint32 kt10_3_msk_mac_da_low_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt10_3_msk_icmp_type_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt10_3_msk_ippt_fld  :  1 ; /* bits 13:13 */
    cs_uint32 kt10_3_msk_mac_da_flags_fld :  1 ; /* bits 14:14 */
    cs_uint32 kt10_3_msk_tpid_enc_fld :  1 ; /* bits 15:15 */
    cs_uint32 kt10_3_prsr_len_excd :  1 ; /* bits 16:16 */
    cs_uint32 kt10_3_msk_prsr_len_excd_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt10_2_prsr_len_excd :  1 ; /* bits 18:18 */
    cs_uint32 kt10_2_msk_prsr_len_excd_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt10_1_prsr_len_excd :  1 ; /* bits 20:20 */
    cs_uint32 kt10_1_msk_prsr_len_excd_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt10_0_prsr_len_excd :  1 ; /* bits 22:22 */
    cs_uint32 kt10_0_msk_prsr_len_excd_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt10_3_cfi_flg       :  1 ; /* bits 24:24 */
    cs_uint32 kt10_2_cfi_flg       :  1 ; /* bits 25:25 */
    cs_uint32 kt10_1_cfi_flg       :  1 ; /* bits 26:26 */
    cs_uint32 kt10_0_cfi_flg       :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf11 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt_cl_key_type       :  4 ; /* bits 31:28 */
    cs_uint32 kt11_rsvd            :  8 ; /* bits 27:20 */
    cs_uint32 kt11_0_msk_prsr_len_excd_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt11_0_prsr_len_excd :  1 ; /* bits 18:18 */
    cs_uint32 kt11_1_msk_prsr_len_excd_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt11_1_prsr_len_excd :  1 ; /* bits 16:16 */
    cs_uint32 kt11_1_group_id      :  6 ; /* bits 15:10 */
    cs_uint32 kt11_1_group_id_vld  :  1 ; /* bits 9:9 */
    cs_uint32 kt11_1_msk_vlanid_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt11_1_vlanid        :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt11_1_vlanid        :  8 ; /* bits 7:0 */
    cs_uint32 kt11_1_msk_vlanid_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt11_1_group_id_vld  :  1 ; /* bits 9:9 */
    cs_uint32 kt11_1_group_id      :  6 ; /* bits 15:10 */
    cs_uint32 kt11_1_prsr_len_excd :  1 ; /* bits 16:16 */
    cs_uint32 kt11_1_msk_prsr_len_excd_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt11_0_prsr_len_excd :  1 ; /* bits 18:18 */
    cs_uint32 kt11_0_msk_prsr_len_excd_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt11_rsvd            :  8 ; /* bits 27:20 */
    cs_uint32 kt_cl_key_type       :  4 ; /* bits 31:28 */
#   endif
  } bf12 ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_MEM_DATA9_t;





#define FE_LE_CE_KEY_MEM_DATA8                   0x06137
#define FE_LE_CE_KEY_MEM_DATA8_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt0_msk_802_1p_fld   :  1 ; /* bits 31:31 */
    cs_uint32 kt0_msk_vlanid_fld   :  1 ; /* bits 30:30 */
    cs_uint32 kt0_msk_ev2pt_fld    :  1 ; /* bits 29:29 */
    cs_uint32 kt0_msk_mac_sa_fld   :  1 ; /* bits 28:28 */
    cs_uint32 kt0_msk_mac_da_fld   :  1 ; /* bits 27:27 */
    cs_uint32 kt0_spid             :  3 ; /* bits 26:24 */
    cs_uint32 kt0_vlan_cnt         :  2 ; /* bits 23:22 */
    cs_uint32 kt0_p_tag_flag       :  1 ; /* bits 21:21 */
    cs_uint32 kt0_tpid_enc         :  3 ; /* bits 20:18 */
    cs_uint32 kt0_igmp_type        :  8 ; /* bits 17:10 */
    cs_uint32 kt0_icmp_type        :  8 ; /* bits 9:2 */
    cs_uint32 kt0_tcp_flg          :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt0_tcp_flg          :  2 ; /* bits 1:0 */
    cs_uint32 kt0_icmp_type        :  8 ; /* bits 9:2 */
    cs_uint32 kt0_igmp_type        :  8 ; /* bits 17:10 */
    cs_uint32 kt0_tpid_enc         :  3 ; /* bits 20:18 */
    cs_uint32 kt0_p_tag_flag       :  1 ; /* bits 21:21 */
    cs_uint32 kt0_vlan_cnt         :  2 ; /* bits 23:22 */
    cs_uint32 kt0_spid             :  3 ; /* bits 26:24 */
    cs_uint32 kt0_msk_mac_da_fld   :  1 ; /* bits 27:27 */
    cs_uint32 kt0_msk_mac_sa_fld   :  1 ; /* bits 28:28 */
    cs_uint32 kt0_msk_ev2pt_fld    :  1 ; /* bits 29:29 */
    cs_uint32 kt0_msk_vlanid_fld   :  1 ; /* bits 30:30 */
    cs_uint32 kt0_msk_802_1p_fld   :  1 ; /* bits 31:31 */
#   endif
  } bf0 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_0_ipv6_flow_label : 16 ; /* bits 31:16 */
    cs_uint32 kt2_0_msk_ip_sa_fld  :  8 ; /* bits 15:8 */
    cs_uint32 kt2_0_msk_ip_da_fld  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_0_msk_ip_da_fld  :  8 ; /* bits 7:0 */
    cs_uint32 kt2_0_msk_ip_sa_fld  :  8 ; /* bits 15:8 */
    cs_uint32 kt2_0_ipv6_flow_label : 16 ; /* bits 31:16 */
#   endif
  } bf1 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_1_msk_802_1p_fld :  1 ; /* bits 31:31 */
    cs_uint32 kt2_1_msk_vlanid_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt2_1_msk_ev2pt_fld  :  1 ; /* bits 29:29 */
    cs_uint32 kt2_1_msk_mac_sa_fld :  1 ; /* bits 28:28 */
    cs_uint32 kt2_1_msk_mac_da_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt2_1_spid           :  3 ; /* bits 26:24 */
    cs_uint32 kt2_1_vlan_cnt       :  2 ; /* bits 23:22 */
    cs_uint32 kt2_1_p_tag_flag     :  1 ; /* bits 21:21 */
    cs_uint32 kt2_1_tpid_enc       :  3 ; /* bits 20:18 */
    cs_uint32 kt2_1_igmp_type      :  8 ; /* bits 17:10 */
    cs_uint32 kt2_1_icmp_type      :  8 ; /* bits 9:2 */
    cs_uint32 kt2_1_tcp_flg        :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_1_tcp_flg        :  2 ; /* bits 1:0 */
    cs_uint32 kt2_1_icmp_type      :  8 ; /* bits 9:2 */
    cs_uint32 kt2_1_igmp_type      :  8 ; /* bits 17:10 */
    cs_uint32 kt2_1_tpid_enc       :  3 ; /* bits 20:18 */
    cs_uint32 kt2_1_p_tag_flag     :  1 ; /* bits 21:21 */
    cs_uint32 kt2_1_vlan_cnt       :  2 ; /* bits 23:22 */
    cs_uint32 kt2_1_spid           :  3 ; /* bits 26:24 */
    cs_uint32 kt2_1_msk_mac_da_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt2_1_msk_mac_sa_fld :  1 ; /* bits 28:28 */
    cs_uint32 kt2_1_msk_ev2pt_fld  :  1 ; /* bits 29:29 */
    cs_uint32 kt2_1_msk_vlanid_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt2_1_msk_802_1p_fld :  1 ; /* bits 31:31 */
#   endif
  } bf2 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt1_0_dscp_tos       :  7 ; /* bits 31:25 */
    cs_uint32 kt1_0_ippt           :  8 ; /* bits 24:17 */
    cs_uint32 kt1_0_l4_port        : 16 ; /* bits 16:1 */
    cs_uint32 kt1_0_ip_addr        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt1_0_ip_addr        :  1 ; /* bits 0:0 */
    cs_uint32 kt1_0_l4_port        : 16 ; /* bits 16:1 */
    cs_uint32 kt1_0_ippt           :  8 ; /* bits 24:17 */
    cs_uint32 kt1_0_dscp_tos       :  7 ; /* bits 31:25 */
#   endif
  } bf3 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt3_rsvd             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt3_rsvd             : 32 ; /* bits 31:0 */
#   endif
  } bf4 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt4_7_vlanid         : 11 ; /* bits 31:21 */
    cs_uint32 kt4_7_msk_spid_fld   :  1 ; /* bits 20:20 */
    cs_uint32 kt4_7_spid           :  3 ; /* bits 19:17 */
    cs_uint32 kt4_6_mirror_pkt_flag :  1 ; /* bits 16:16 */
    cs_uint32 kt4_6_rsvd           : 15 ; /* bits 15:1 */
    cs_uint32 kt4_6_msk_mirror_group_id_fld :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt4_6_msk_mirror_group_id_fld :  1 ; /* bits 0:0 */
    cs_uint32 kt4_6_rsvd           : 15 ; /* bits 15:1 */
    cs_uint32 kt4_6_mirror_pkt_flag :  1 ; /* bits 16:16 */
    cs_uint32 kt4_7_spid           :  3 ; /* bits 19:17 */
    cs_uint32 kt4_7_msk_spid_fld   :  1 ; /* bits 20:20 */
    cs_uint32 kt4_7_vlanid         : 11 ; /* bits 31:21 */
#   endif
  } bf5 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt5_0_vlanid         :  5 ; /* bits 31:27 */
    cs_uint32 kt5_0_mac_type       :  1 ; /* bits 26:26 */
    cs_uint32 kt5_0_mac_addr       : 26 ; /* bits 25:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt5_0_mac_addr       : 26 ; /* bits 25:0 */
    cs_uint32 kt5_0_mac_type       :  1 ; /* bits 26:26 */
    cs_uint32 kt5_0_vlanid         :  5 ; /* bits 31:27 */
#   endif
  } bf6 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt6_0_ev2pt          :  3 ; /* bits 31:29 */
    cs_uint32 kt6_0_mac_addr       : 29 ; /* bits 28:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt6_0_mac_addr       : 29 ; /* bits 28:0 */
    cs_uint32 kt6_0_ev2pt          :  3 ; /* bits 31:29 */
#   endif
  } bf7 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt7_3_spl_pkt_vec    :  2 ; /* bits 31:30 */
    cs_uint32 kt7_3_spl_pkt_en     :  1 ; /* bits 29:29 */
    cs_uint32 kt7_3_msk_mac_da_fld :  6 ; /* bits 28:23 */
    cs_uint32 kt7_3_mac_da         : 23 ; /* bits 22:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt7_3_mac_da         : 23 ; /* bits 22:0 */
    cs_uint32 kt7_3_msk_mac_da_fld :  6 ; /* bits 28:23 */
    cs_uint32 kt7_3_spl_pkt_en     :  1 ; /* bits 29:29 */
    cs_uint32 kt7_3_spl_pkt_vec    :  2 ; /* bits 31:30 */
#   endif
  } bf8 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt8_rsvd             :  8 ; /* bits 31:24 */
    cs_uint32 kt8_7_msk_tpid_enc_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt8_7_tpid_enc       :  3 ; /* bits 22:20 */
    cs_uint32 kt8_7_msk_ev2pt_enc_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt8_7_ev2pt_enc      :  4 ; /* bits 18:15 */
    cs_uint32 kt8_7_msk_bc_llid_fld :  1 ; /* bits 14:14 */
    cs_uint32 kt8_7_bc_llid        :  1 ; /* bits 13:13 */
    cs_uint32 kt8_7_msk_mc_flag_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt8_7_mc_flag        :  1 ; /* bits 11:11 */
    cs_uint32 kt8_7_msk_bc_flag_fld :  1 ; /* bits 10:10 */
    cs_uint32 kt8_7_bc_flag        :  1 ; /* bits 9:9 */
    cs_uint32 kt8_7_msk_mac_da_flag_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt8_7_rsvd_mac_da_flag :  1 ; /* bits 7:7 */
    cs_uint32 kt8_7_msk_p_tag_flag_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt8_7_p_tag_flag     :  1 ; /* bits 5:5 */
    cs_uint32 kt8_7_msk_vlan_cnt_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt8_7_vlan_cnt       :  2 ; /* bits 3:2 */
    cs_uint32 kt8_7_msk_vlanid_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt8_7_vlanid         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt8_7_vlanid         :  1 ; /* bits 0:0 */
    cs_uint32 kt8_7_msk_vlanid_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt8_7_vlan_cnt       :  2 ; /* bits 3:2 */
    cs_uint32 kt8_7_msk_vlan_cnt_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt8_7_p_tag_flag     :  1 ; /* bits 5:5 */
    cs_uint32 kt8_7_msk_p_tag_flag_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt8_7_rsvd_mac_da_flag :  1 ; /* bits 7:7 */
    cs_uint32 kt8_7_msk_mac_da_flag_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt8_7_bc_flag        :  1 ; /* bits 9:9 */
    cs_uint32 kt8_7_msk_bc_flag_fld :  1 ; /* bits 10:10 */
    cs_uint32 kt8_7_mc_flag        :  1 ; /* bits 11:11 */
    cs_uint32 kt8_7_msk_mc_flag_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt8_7_bc_llid        :  1 ; /* bits 13:13 */
    cs_uint32 kt8_7_msk_bc_llid_fld :  1 ; /* bits 14:14 */
    cs_uint32 kt8_7_ev2pt_enc      :  4 ; /* bits 18:15 */
    cs_uint32 kt8_7_msk_ev2pt_enc_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt8_7_tpid_enc       :  3 ; /* bits 22:20 */
    cs_uint32 kt8_7_msk_tpid_enc_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt8_rsvd             :  8 ; /* bits 31:24 */
#   endif
  } bf9 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt9_7_vlanid_hi      :  3 ; /* bits 31:29 */
    cs_uint32 kt9_7_vlanid_lo      : 12 ; /* bits 28:17 */
    cs_uint32 kt9_6_msk_ev2pt_enc_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt9_6_ev2pt_enc      :  4 ; /* bits 15:12 */
    cs_uint32 kt9_6_msk_mc_flag_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt9_6_mc_flag        :  1 ; /* bits 10:10 */
    cs_uint32 kt9_6_msk_bc_flag_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt9_6_bc_flag        :  1 ; /* bits 8:8 */
    cs_uint32 kt9_6_inner_vlan_en  :  1 ; /* bits 7:7 */
    cs_uint32 kt9_6_rsvd           :  1 ; /* bits 6:6 */
    cs_uint32 kt9_6_msk_vlan_cnt_fld :  1 ; /* bits 5:5 */
    cs_uint32 kt9_6_vlan_cnt       :  2 ; /* bits 4:3 */
    cs_uint32 kt9_6_msk_vlanid_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt9_6_vlanid_hi      :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt9_6_vlanid_hi      :  2 ; /* bits 1:0 */
    cs_uint32 kt9_6_msk_vlanid_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt9_6_vlan_cnt       :  2 ; /* bits 4:3 */
    cs_uint32 kt9_6_msk_vlan_cnt_fld :  1 ; /* bits 5:5 */
    cs_uint32 kt9_6_rsvd           :  1 ; /* bits 6:6 */
    cs_uint32 kt9_6_inner_vlan_en  :  1 ; /* bits 7:7 */
    cs_uint32 kt9_6_bc_flag        :  1 ; /* bits 8:8 */
    cs_uint32 kt9_6_msk_bc_flag_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt9_6_mc_flag        :  1 ; /* bits 10:10 */
    cs_uint32 kt9_6_msk_mc_flag_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt9_6_ev2pt_enc      :  4 ; /* bits 15:12 */
    cs_uint32 kt9_6_msk_ev2pt_enc_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt9_7_vlanid_lo      : 12 ; /* bits 28:17 */
    cs_uint32 kt9_7_vlanid_hi      :  3 ; /* bits 31:29 */
#   endif
  } bf10 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt10_3_ippt          :  4 ; /* bits 31:28 */
    cs_uint32 kt10_3_icmp_type     :  8 ; /* bits 27:20 */
    cs_uint32 kt10_3_l4_dest_hit_map :  8 ; /* bits 19:12 */
    cs_uint32 kt10_3_l4_src_hit_map :  8 ; /* bits 11:4 */
    cs_uint32 kt10_3_spcl_hit_map  :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt10_3_spcl_hit_map  :  4 ; /* bits 3:0 */
    cs_uint32 kt10_3_l4_src_hit_map :  8 ; /* bits 11:4 */
    cs_uint32 kt10_3_l4_dest_hit_map :  8 ; /* bits 19:12 */
    cs_uint32 kt10_3_icmp_type     :  8 ; /* bits 27:20 */
    cs_uint32 kt10_3_ippt          :  4 ; /* bits 31:28 */
#   endif
  } bf11 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt11_1_vlanid        :  4 ; /* bits 31:28 */
    cs_uint32 kt11_1_msk_addr_fld  :  1 ; /* bits 27:27 */
    cs_uint32 kt11_1_src_addr_vld  :  1 ; /* bits 26:26 */
    cs_uint32 kt11_1_addr_type     :  2 ; /* bits 25:24 */
    cs_uint32 kt11_1_addr          : 24 ; /* bits 23:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt11_1_addr          : 24 ; /* bits 23:0 */
    cs_uint32 kt11_1_addr_type     :  2 ; /* bits 25:24 */
    cs_uint32 kt11_1_src_addr_vld  :  1 ; /* bits 26:26 */
    cs_uint32 kt11_1_msk_addr_fld  :  1 ; /* bits 27:27 */
    cs_uint32 kt11_1_vlanid        :  4 ; /* bits 31:28 */
#   endif
  } bf12 ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_MEM_DATA8_t;





#define FE_LE_CE_KEY_MEM_DATA7                   0x06138
#define FE_LE_CE_KEY_MEM_DATA7_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt0_tcp_flg          :  4 ; /* bits 31:28 */
    cs_uint32 kt0_spcl_hit_map     :  8 ; /* bits 27:20 */
    cs_uint32 kt0_l4_dest_hit_map  :  8 ; /* bits 19:12 */
    cs_uint32 kt0_l4_src_hit_map   :  8 ; /* bits 11:4 */
    cs_uint32 kt0_l4_port_type     :  1 ; /* bits 3:3 */
    cs_uint32 kt0_l4_port          :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt0_l4_port          :  3 ; /* bits 2:0 */
    cs_uint32 kt0_l4_port_type     :  1 ; /* bits 3:3 */
    cs_uint32 kt0_l4_src_hit_map   :  8 ; /* bits 11:4 */
    cs_uint32 kt0_l4_dest_hit_map  :  8 ; /* bits 19:12 */
    cs_uint32 kt0_spcl_hit_map     :  8 ; /* bits 27:20 */
    cs_uint32 kt0_tcp_flg          :  4 ; /* bits 31:28 */
#   endif
  } bf0 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_0_ip_sa          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_0_ip_sa          : 32 ; /* bits 31:0 */
#   endif
  } bf1 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_1_tcp_flg        :  4 ; /* bits 31:28 */
    cs_uint32 kt2_1_spcl_hit_map   :  8 ; /* bits 27:20 */
    cs_uint32 kt2_1_l4_dest_hit_map :  8 ; /* bits 19:12 */
    cs_uint32 kt2_1_l4_src_hit_map :  8 ; /* bits 11:4 */
    cs_uint32 kt2_1_l4_port_type   :  1 ; /* bits 3:3 */
    cs_uint32 kt2_1_l4_port        :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_1_l4_port        :  3 ; /* bits 2:0 */
    cs_uint32 kt2_1_l4_port_type   :  1 ; /* bits 3:3 */
    cs_uint32 kt2_1_l4_src_hit_map :  8 ; /* bits 11:4 */
    cs_uint32 kt2_1_l4_dest_hit_map :  8 ; /* bits 19:12 */
    cs_uint32 kt2_1_spcl_hit_map   :  8 ; /* bits 27:20 */
    cs_uint32 kt2_1_tcp_flg        :  4 ; /* bits 31:28 */
#   endif
  } bf2 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt1_0_ip_addr        : 31 ; /* bits 31:1 */
    cs_uint32 kt1_1_msk_dscp_tos_fld :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt1_1_msk_dscp_tos_fld :  1 ; /* bits 0:0 */
    cs_uint32 kt1_0_ip_addr        : 31 ; /* bits 31:1 */
#   endif
  } bf3 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt3_rsvd             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt3_rsvd             : 32 ; /* bits 31:0 */
#   endif
  } bf4 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt4_6_mirror_group_id :  5 ; /* bits 31:27 */
    cs_uint32 kt4_6_msk_vlanid_fld :  1 ; /* bits 26:26 */
    cs_uint32 kt4_6_vlanid         : 12 ; /* bits 25:14 */
    cs_uint32 kt4_6_msk_spid_fld   :  1 ; /* bits 13:13 */
    cs_uint32 kt4_6_spid           :  3 ; /* bits 12:10 */
    cs_uint32 kt4_5_mirror_pkt_flag :  1 ; /* bits 9:9 */
    cs_uint32 kt4_5_rsvd           :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt4_5_rsvd           :  9 ; /* bits 8:0 */
    cs_uint32 kt4_5_mirror_pkt_flag :  1 ; /* bits 9:9 */
    cs_uint32 kt4_6_spid           :  3 ; /* bits 12:10 */
    cs_uint32 kt4_6_msk_spid_fld   :  1 ; /* bits 13:13 */
    cs_uint32 kt4_6_vlanid         : 12 ; /* bits 25:14 */
    cs_uint32 kt4_6_msk_vlanid_fld :  1 ; /* bits 26:26 */
    cs_uint32 kt4_6_mirror_group_id :  5 ; /* bits 31:27 */
#   endif
  } bf5 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt5_0_mac_addr       : 22 ; /* bits 31:10 */
    cs_uint32 kt5_1_age_tmr_offset :  6 ; /* bits 9:4 */
    cs_uint32 kt5_1_msk_802_1p_fld :  1 ; /* bits 3:3 */
    cs_uint32 kt5_1_msk_p_tag_flag_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt5_1_msk_vlan_cnt_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt5_1_msk_vlanid_fld :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt5_1_msk_vlanid_fld :  1 ; /* bits 0:0 */
    cs_uint32 kt5_1_msk_vlan_cnt_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt5_1_msk_p_tag_flag_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt5_1_msk_802_1p_fld :  1 ; /* bits 3:3 */
    cs_uint32 kt5_1_age_tmr_offset :  6 ; /* bits 9:4 */
    cs_uint32 kt5_0_mac_addr       : 22 ; /* bits 31:10 */
#   endif
  } bf6 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt6_0_mac_addr       : 19 ; /* bits 31:13 */
    cs_uint32 kt6_1_msk_mac_da_range_flag_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt6_1_msk_tpid_enc_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt6_1_msk_ev2pt_fld  :  1 ; /* bits 10:10 */
    cs_uint32 kt6_1_msk_mac_addr_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt6_1_mac_da_range_flag :  1 ; /* bits 8:8 */
    cs_uint32 kt6_1_tpid_enc       :  3 ; /* bits 7:5 */
    cs_uint32 kt6_1_age_tmr_offset :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt6_1_age_tmr_offset :  5 ; /* bits 4:0 */
    cs_uint32 kt6_1_tpid_enc       :  3 ; /* bits 7:5 */
    cs_uint32 kt6_1_mac_da_range_flag :  1 ; /* bits 8:8 */
    cs_uint32 kt6_1_msk_mac_addr_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt6_1_msk_ev2pt_fld  :  1 ; /* bits 10:10 */
    cs_uint32 kt6_1_msk_tpid_enc_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt6_1_msk_mac_da_range_flag_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt6_0_mac_addr       : 19 ; /* bits 31:13 */
#   endif
  } bf7 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt7_3_mac_da         : 25 ; /* bits 31:7 */
    cs_uint32 kt7_2_msk_ev2pt_fld  :  1 ; /* bits 6:6 */
    cs_uint32 kt7_2_spl_pkt_vec    :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt7_2_spl_pkt_vec    :  6 ; /* bits 5:0 */
    cs_uint32 kt7_2_msk_ev2pt_fld  :  1 ; /* bits 6:6 */
    cs_uint32 kt7_3_mac_da         : 25 ; /* bits 31:7 */
#   endif
  } bf8 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt8_7_vlanid         : 11 ; /* bits 31:21 */
    cs_uint32 kt8_6_msk_tpid_enc_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt8_6_tpid_enc       :  3 ; /* bits 19:17 */
    cs_uint32 kt8_6_msk_ev2pt_enc_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt8_6_ev2pt_enc      :  4 ; /* bits 15:12 */
    cs_uint32 kt8_6_msk_bc_llid_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt8_6_bc_llid        :  1 ; /* bits 10:10 */
    cs_uint32 kt8_6_msk_mc_flag_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt8_6_mc_flag        :  1 ; /* bits 8:8 */
    cs_uint32 kt8_6_msk_bc_flag_fld :  1 ; /* bits 7:7 */
    cs_uint32 kt8_6_bc_flag        :  1 ; /* bits 6:6 */
    cs_uint32 kt8_6_msk_mac_da_flag_fld :  1 ; /* bits 5:5 */
    cs_uint32 kt8_6_rsvd_mac_da_flag :  1 ; /* bits 4:4 */
    cs_uint32 kt8_6_msk_p_tag_flag_fld :  1 ; /* bits 3:3 */
    cs_uint32 kt8_6_p_tag_flag     :  1 ; /* bits 2:2 */
    cs_uint32 kt8_6_msk_vlan_cnt_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt8_6_vlan_cnt       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt8_6_vlan_cnt       :  1 ; /* bits 0:0 */
    cs_uint32 kt8_6_msk_vlan_cnt_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt8_6_p_tag_flag     :  1 ; /* bits 2:2 */
    cs_uint32 kt8_6_msk_p_tag_flag_fld :  1 ; /* bits 3:3 */
    cs_uint32 kt8_6_rsvd_mac_da_flag :  1 ; /* bits 4:4 */
    cs_uint32 kt8_6_msk_mac_da_flag_fld :  1 ; /* bits 5:5 */
    cs_uint32 kt8_6_bc_flag        :  1 ; /* bits 6:6 */
    cs_uint32 kt8_6_msk_bc_flag_fld :  1 ; /* bits 7:7 */
    cs_uint32 kt8_6_mc_flag        :  1 ; /* bits 8:8 */
    cs_uint32 kt8_6_msk_mc_flag_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt8_6_bc_llid        :  1 ; /* bits 10:10 */
    cs_uint32 kt8_6_msk_bc_llid_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt8_6_ev2pt_enc      :  4 ; /* bits 15:12 */
    cs_uint32 kt8_6_msk_ev2pt_enc_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt8_6_tpid_enc       :  3 ; /* bits 19:17 */
    cs_uint32 kt8_6_msk_tpid_enc_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt8_7_vlanid         : 11 ; /* bits 31:21 */
#   endif
  } bf9 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt9_6_vlanid_hi      : 10 ; /* bits 31:22 */
    cs_uint32 kt9_6_vlanid_lo      : 12 ; /* bits 21:10 */
    cs_uint32 kt9_5_msk_ev2pt_enc_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt9_5_ev2pt_enc      :  4 ; /* bits 8:5 */
    cs_uint32 kt9_5_msk_mc_flag_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt9_5_mc_flag        :  1 ; /* bits 3:3 */
    cs_uint32 kt9_5_msk_bc_flag_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt9_5_bc_flag        :  1 ; /* bits 1:1 */
    cs_uint32 kt9_5_inner_vlan_en  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt9_5_inner_vlan_en  :  1 ; /* bits 0:0 */
    cs_uint32 kt9_5_bc_flag        :  1 ; /* bits 1:1 */
    cs_uint32 kt9_5_msk_bc_flag_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt9_5_mc_flag        :  1 ; /* bits 3:3 */
    cs_uint32 kt9_5_msk_mc_flag_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt9_5_ev2pt_enc      :  4 ; /* bits 8:5 */
    cs_uint32 kt9_5_msk_ev2pt_enc_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt9_6_vlanid_lo      : 12 ; /* bits 21:10 */
    cs_uint32 kt9_6_vlanid_hi      : 10 ; /* bits 31:22 */
#   endif
  } bf10 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt10_3_spcl_hit_map  :  4 ; /* bits 31:28 */
    cs_uint32 kt10_3_mac_da_low    :  8 ; /* bits 27:20 */
    cs_uint32 kt10_3_ev2pt         : 16 ; /* bits 19:4 */
    cs_uint32 kt10_2_msk_tpid_enc_fld :  1 ; /* bits 3:3 */
    cs_uint32 kt10_2_msk_mac_da_flags_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt10_2_msk_ippt_fld  :  1 ; /* bits 1:1 */
    cs_uint32 kt10_2_msk_icmp_type_fld :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt10_2_msk_icmp_type_fld :  1 ; /* bits 0:0 */
    cs_uint32 kt10_2_msk_ippt_fld  :  1 ; /* bits 1:1 */
    cs_uint32 kt10_2_msk_mac_da_flags_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt10_2_msk_tpid_enc_fld :  1 ; /* bits 3:3 */
    cs_uint32 kt10_3_ev2pt         : 16 ; /* bits 19:4 */
    cs_uint32 kt10_3_mac_da_low    :  8 ; /* bits 27:20 */
    cs_uint32 kt10_3_spcl_hit_map  :  4 ; /* bits 31:28 */
#   endif
  } bf11 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt11_1_addr          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt11_1_addr          : 32 ; /* bits 31:0 */
#   endif
  } bf12 ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_MEM_DATA7_t;





#define FE_LE_CE_KEY_MEM_DATA6                   0x06139
#define FE_LE_CE_KEY_MEM_DATA6_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt0_l4_port          : 13 ; /* bits 31:19 */
    cs_uint32 kt0_ip_fragment_state :  2 ; /* bits 18:17 */
    cs_uint32 kt0_ip_opt_flg       :  1 ; /* bits 16:16 */
    cs_uint32 kt0_dscp_tos         :  8 ; /* bits 15:8 */
    cs_uint32 kt0_ippt             :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt0_ippt             :  8 ; /* bits 7:0 */
    cs_uint32 kt0_dscp_tos         :  8 ; /* bits 15:8 */
    cs_uint32 kt0_ip_opt_flg       :  1 ; /* bits 16:16 */
    cs_uint32 kt0_ip_fragment_state :  2 ; /* bits 18:17 */
    cs_uint32 kt0_l4_port          : 13 ; /* bits 31:19 */
#   endif
  } bf0 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_0_ip_sa          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_0_ip_sa          : 32 ; /* bits 31:0 */
#   endif
  } bf1 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_1_l4_port        : 13 ; /* bits 31:19 */
    cs_uint32 kt2_1_ip_fragment_state :  2 ; /* bits 18:17 */
    cs_uint32 kt2_1_ip_opt_flg     :  1 ; /* bits 16:16 */
    cs_uint32 kt2_1_dscp_tos       :  8 ; /* bits 15:8 */
    cs_uint32 kt2_1_ippt           :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_1_ippt           :  8 ; /* bits 7:0 */
    cs_uint32 kt2_1_dscp_tos       :  8 ; /* bits 15:8 */
    cs_uint32 kt2_1_ip_opt_flg     :  1 ; /* bits 16:16 */
    cs_uint32 kt2_1_ip_fragment_state :  2 ; /* bits 18:17 */
    cs_uint32 kt2_1_l4_port        : 13 ; /* bits 31:19 */
#   endif
  } bf2 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt1_1_msk_ippt_fld   :  1 ; /* bits 31:31 */
    cs_uint32 kt1_1_msk_l4_port_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt1_1_msk_ip_addr_fld :  6 ; /* bits 29:24 */
    cs_uint32 kt1_1_l4_port_type   :  1 ; /* bits 23:23 */
    cs_uint32 kt1_1_ip_addr_type   :  1 ; /* bits 22:22 */
    cs_uint32 kt1_1_dscp_tos       :  8 ; /* bits 21:14 */
    cs_uint32 kt1_1_ippt           :  8 ; /* bits 13:6 */
    cs_uint32 kt1_1_l4_port        :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt1_1_l4_port        :  6 ; /* bits 5:0 */
    cs_uint32 kt1_1_ippt           :  8 ; /* bits 13:6 */
    cs_uint32 kt1_1_dscp_tos       :  8 ; /* bits 21:14 */
    cs_uint32 kt1_1_ip_addr_type   :  1 ; /* bits 22:22 */
    cs_uint32 kt1_1_l4_port_type   :  1 ; /* bits 23:23 */
    cs_uint32 kt1_1_msk_ip_addr_fld :  6 ; /* bits 29:24 */
    cs_uint32 kt1_1_msk_l4_port_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt1_1_msk_ippt_fld   :  1 ; /* bits 31:31 */
#   endif
  } bf3 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt3_rsvd             : 28 ; /* bits 31:4 */
    cs_uint32 kt3_0_msk_prsr_len_excd_fld :  1 ; /* bits 3:3 */
    cs_uint32 kt3_0_prsr_len_excd  :  1 ; /* bits 2:2 */
    cs_uint32 kt3_0_msk_ipv6_flow_label_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt3_0_ipv6_flow_label :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt3_0_ipv6_flow_label :  1 ; /* bits 0:0 */
    cs_uint32 kt3_0_msk_ipv6_flow_label_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt3_0_prsr_len_excd  :  1 ; /* bits 2:2 */
    cs_uint32 kt3_0_msk_prsr_len_excd_fld :  1 ; /* bits 3:3 */
    cs_uint32 kt3_rsvd             : 28 ; /* bits 31:4 */
#   endif
  } bf4 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt4_5_rsvd           :  6 ; /* bits 31:26 */
    cs_uint32 kt4_5_msk_mirror_group_id_fld :  1 ; /* bits 25:25 */
    cs_uint32 kt4_5_mirror_group_id :  5 ; /* bits 24:20 */
    cs_uint32 kt4_5_msk_vlanid_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt4_5_vlanid         : 12 ; /* bits 18:7 */
    cs_uint32 kt4_5_msk_spid_fld   :  1 ; /* bits 6:6 */
    cs_uint32 kt4_5_spid           :  3 ; /* bits 5:3 */
    cs_uint32 kt4_4_mirror_pkt_flag :  1 ; /* bits 2:2 */
    cs_uint32 kt4_4_rsvd           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt4_4_rsvd           :  2 ; /* bits 1:0 */
    cs_uint32 kt4_4_mirror_pkt_flag :  1 ; /* bits 2:2 */
    cs_uint32 kt4_5_spid           :  3 ; /* bits 5:3 */
    cs_uint32 kt4_5_msk_spid_fld   :  1 ; /* bits 6:6 */
    cs_uint32 kt4_5_vlanid         : 12 ; /* bits 18:7 */
    cs_uint32 kt4_5_msk_vlanid_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt4_5_mirror_group_id :  5 ; /* bits 24:20 */
    cs_uint32 kt4_5_msk_mirror_group_id_fld :  1 ; /* bits 25:25 */
    cs_uint32 kt4_5_rsvd           :  6 ; /* bits 31:26 */
#   endif
  } bf5 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt5_1_msk_mac_addr_fld :  1 ; /* bits 31:31 */
    cs_uint32 kt5_1_vl_802_1p      :  3 ; /* bits 30:28 */
    cs_uint32 kt5_1_p_tag_flag     :  1 ; /* bits 27:27 */
    cs_uint32 kt5_1_vlan_cnt       :  2 ; /* bits 26:25 */
    cs_uint32 kt5_1_vlanid         : 12 ; /* bits 24:13 */
    cs_uint32 kt5_1_mac_type       :  1 ; /* bits 12:12 */
    cs_uint32 kt5_1_mac_addr       : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt5_1_mac_addr       : 12 ; /* bits 11:0 */
    cs_uint32 kt5_1_mac_type       :  1 ; /* bits 12:12 */
    cs_uint32 kt5_1_vlanid         : 12 ; /* bits 24:13 */
    cs_uint32 kt5_1_vlan_cnt       :  2 ; /* bits 26:25 */
    cs_uint32 kt5_1_p_tag_flag     :  1 ; /* bits 27:27 */
    cs_uint32 kt5_1_vl_802_1p      :  3 ; /* bits 30:28 */
    cs_uint32 kt5_1_msk_mac_addr_fld :  1 ; /* bits 31:31 */
#   endif
  } bf6 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt6_1_age_tmr_offset :  1 ; /* bits 31:31 */
    cs_uint32 kt6_1_mac_type       :  1 ; /* bits 30:30 */
    cs_uint32 kt6_1_ev2pt          : 16 ; /* bits 29:14 */
    cs_uint32 kt6_1_mac_addr       : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt6_1_mac_addr       : 14 ; /* bits 13:0 */
    cs_uint32 kt6_1_ev2pt          : 16 ; /* bits 29:14 */
    cs_uint32 kt6_1_mac_type       :  1 ; /* bits 30:30 */
    cs_uint32 kt6_1_age_tmr_offset :  1 ; /* bits 31:31 */
#   endif
  } bf7 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt7_2_spl_pkt_vec    : 15 ; /* bits 31:17 */
    cs_uint32 kt7_2_spl_pkt_en     :  1 ; /* bits 16:16 */
    cs_uint32 kt7_2_msk_mac_da_fld :  6 ; /* bits 15:10 */
    cs_uint32 kt7_2_mac_da         : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt7_2_mac_da         : 10 ; /* bits 9:0 */
    cs_uint32 kt7_2_msk_mac_da_fld :  6 ; /* bits 15:10 */
    cs_uint32 kt7_2_spl_pkt_en     :  1 ; /* bits 16:16 */
    cs_uint32 kt7_2_spl_pkt_vec    : 15 ; /* bits 31:17 */
#   endif
  } bf8 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt8_6_vlan_cnt       :  1 ; /* bits 31:31 */
    cs_uint32 kt8_6_msk_vlanid_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt8_6_vlanid         : 12 ; /* bits 29:18 */
    cs_uint32 kt8_5_msk_tpid_enc_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt8_5_tpid_enc       :  3 ; /* bits 16:14 */
    cs_uint32 kt8_5_msk_ev2pt_enc_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt8_5_ev2pt_enc      :  4 ; /* bits 12:9 */
    cs_uint32 kt8_5_msk_bc_llid_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt8_5_bc_llid        :  1 ; /* bits 7:7 */
    cs_uint32 kt8_5_msk_mc_flag_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt8_5_mc_flag        :  1 ; /* bits 5:5 */
    cs_uint32 kt8_5_msk_bc_flag_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt8_5_bc_flag        :  1 ; /* bits 3:3 */
    cs_uint32 kt8_5_msk_mac_da_flag_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt8_5_rsvd_mac_da_flag :  1 ; /* bits 1:1 */
    cs_uint32 kt8_5_msk_p_tag_flag_fld :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt8_5_msk_p_tag_flag_fld :  1 ; /* bits 0:0 */
    cs_uint32 kt8_5_rsvd_mac_da_flag :  1 ; /* bits 1:1 */
    cs_uint32 kt8_5_msk_mac_da_flag_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt8_5_bc_flag        :  1 ; /* bits 3:3 */
    cs_uint32 kt8_5_msk_bc_flag_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt8_5_mc_flag        :  1 ; /* bits 5:5 */
    cs_uint32 kt8_5_msk_mc_flag_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt8_5_bc_llid        :  1 ; /* bits 7:7 */
    cs_uint32 kt8_5_msk_bc_llid_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt8_5_ev2pt_enc      :  4 ; /* bits 12:9 */
    cs_uint32 kt8_5_msk_ev2pt_enc_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt8_5_tpid_enc       :  3 ; /* bits 16:14 */
    cs_uint32 kt8_5_msk_tpid_enc_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt8_6_vlanid         : 12 ; /* bits 29:18 */
    cs_uint32 kt8_6_msk_vlanid_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt8_6_vlan_cnt       :  1 ; /* bits 31:31 */
#   endif
  } bf9 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt9_5_rsvd           :  1 ; /* bits 31:31 */
    cs_uint32 kt9_5_msk_vlan_cnt_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt9_5_vlan_cnt       :  2 ; /* bits 29:28 */
    cs_uint32 kt9_5_msk_vlanid_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt9_5_vlanid_hi      : 12 ; /* bits 26:15 */
    cs_uint32 kt9_5_vlanid_lo      : 12 ; /* bits 14:3 */
    cs_uint32 kt9_4_msk_ev2pt_enc_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt9_4_ev2pt_enc      :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt9_4_ev2pt_enc      :  2 ; /* bits 1:0 */
    cs_uint32 kt9_4_msk_ev2pt_enc_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt9_5_vlanid_lo      : 12 ; /* bits 14:3 */
    cs_uint32 kt9_5_vlanid_hi      : 12 ; /* bits 26:15 */
    cs_uint32 kt9_5_msk_vlanid_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt9_5_vlan_cnt       :  2 ; /* bits 29:28 */
    cs_uint32 kt9_5_msk_vlan_cnt_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt9_5_rsvd           :  1 ; /* bits 31:31 */
#   endif
  } bf10 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt10_2_msk_mac_da_low_fld :  1 ; /* bits 31:31 */
    cs_uint32 kt10_2_msk_ev2pt_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt10_2_tpid_enc      :  3 ; /* bits 29:27 */
    cs_uint32 kt10_2_rsvd_mac_da_flag :  1 ; /* bits 26:26 */
    cs_uint32 kt10_2_mcst_ip_flag  :  1 ; /* bits 25:25 */
    cs_uint32 kt10_2_mymac_flag    :  1 ; /* bits 24:24 */
    cs_uint32 kt10_2_ippt          :  8 ; /* bits 23:16 */
    cs_uint32 kt10_2_icmp_type     :  8 ; /* bits 15:8 */
    cs_uint32 kt10_2_l4_dest_hit_map :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt10_2_l4_dest_hit_map :  8 ; /* bits 7:0 */
    cs_uint32 kt10_2_icmp_type     :  8 ; /* bits 15:8 */
    cs_uint32 kt10_2_ippt          :  8 ; /* bits 23:16 */
    cs_uint32 kt10_2_mymac_flag    :  1 ; /* bits 24:24 */
    cs_uint32 kt10_2_mcst_ip_flag  :  1 ; /* bits 25:25 */
    cs_uint32 kt10_2_rsvd_mac_da_flag :  1 ; /* bits 26:26 */
    cs_uint32 kt10_2_tpid_enc      :  3 ; /* bits 29:27 */
    cs_uint32 kt10_2_msk_ev2pt_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt10_2_msk_mac_da_low_fld :  1 ; /* bits 31:31 */
#   endif
  } bf11 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt11_1_addr          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt11_1_addr          : 32 ; /* bits 31:0 */
#   endif
  } bf12 ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_MEM_DATA6_t;





#define FE_LE_CE_KEY_MEM_DATA5                   0x0613a
#define FE_LE_CE_KEY_MEM_DATA5_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt0_ip_sa            : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt0_ip_sa            : 32 ; /* bits 31:0 */
#   endif
  } bf0 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_0_ip_sa          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_0_ip_sa          : 32 ; /* bits 31:0 */
#   endif
  } bf1 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_1_rsvd1          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_1_rsvd1          : 32 ; /* bits 31:0 */
#   endif
  } bf2 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt1_1_l4_port        : 10 ; /* bits 31:22 */
    cs_uint32 kt1_1_ip_addr        : 22 ; /* bits 21:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt1_1_ip_addr        : 22 ; /* bits 21:0 */
    cs_uint32 kt1_1_l4_port        : 10 ; /* bits 31:22 */
#   endif
  } bf3 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt3_0_ipv6_flow_label : 19 ; /* bits 31:13 */
    cs_uint32 kt3_0_msk_dscp_tos_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt3_0_msk_ippt_fld   :  1 ; /* bits 11:11 */
    cs_uint32 kt3_0_msk_l4_port_fld :  1 ; /* bits 10:10 */
    cs_uint32 kt3_0_msk_ip_addr_fld :  8 ; /* bits 9:2 */
    cs_uint32 kt3_0_l4_port_type   :  1 ; /* bits 1:1 */
    cs_uint32 kt3_0_ip_addr_type   :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt3_0_ip_addr_type   :  1 ; /* bits 0:0 */
    cs_uint32 kt3_0_l4_port_type   :  1 ; /* bits 1:1 */
    cs_uint32 kt3_0_msk_ip_addr_fld :  8 ; /* bits 9:2 */
    cs_uint32 kt3_0_msk_l4_port_fld :  1 ; /* bits 10:10 */
    cs_uint32 kt3_0_msk_ippt_fld   :  1 ; /* bits 11:11 */
    cs_uint32 kt3_0_msk_dscp_tos_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt3_0_ipv6_flow_label : 19 ; /* bits 31:13 */
#   endif
  } bf4 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt4_4_rsvd           : 13 ; /* bits 31:19 */
    cs_uint32 kt4_4_msk_mirror_group_id_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt4_4_mirror_group_id :  5 ; /* bits 17:13 */
    cs_uint32 kt4_4_msk_vlanid_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt4_4_vlanid         : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt4_4_vlanid         : 12 ; /* bits 11:0 */
    cs_uint32 kt4_4_msk_vlanid_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt4_4_mirror_group_id :  5 ; /* bits 17:13 */
    cs_uint32 kt4_4_msk_mirror_group_id_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt4_4_rsvd           : 13 ; /* bits 31:19 */
#   endif
  } bf5 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt5_1_mac_addr       : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt5_1_mac_addr       : 32 ; /* bits 31:0 */
#   endif
  } bf6 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt6_1_mac_addr       : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt6_1_mac_addr       : 32 ; /* bits 31:0 */
#   endif
  } bf7 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt7_2_mac_da         : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt7_2_mac_da         : 32 ; /* bits 31:0 */
#   endif
  } bf8 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt8_5_p_tag_flag     :  1 ; /* bits 31:31 */
    cs_uint32 kt8_5_msk_vlan_cnt_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt8_5_vlan_cnt       :  2 ; /* bits 29:28 */
    cs_uint32 kt8_5_msk_vlanid_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt8_5_vlanid         : 12 ; /* bits 26:15 */
    cs_uint32 kt8_4_msk_tpid_enc_fld :  1 ; /* bits 14:14 */
    cs_uint32 kt8_4_tpid_enc       :  3 ; /* bits 13:11 */
    cs_uint32 kt8_4_msk_ev2pt_enc_fld :  1 ; /* bits 10:10 */
    cs_uint32 kt8_4_ev2pt_enc      :  4 ; /* bits 9:6 */
    cs_uint32 kt8_4_msk_bc_llid_fld :  1 ; /* bits 5:5 */
    cs_uint32 kt8_4_bc_llid        :  1 ; /* bits 4:4 */
    cs_uint32 kt8_4_msk_mc_flag_fld :  1 ; /* bits 3:3 */
    cs_uint32 kt8_4_mc_flag        :  1 ; /* bits 2:2 */
    cs_uint32 kt8_4_msk_bc_flag_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt8_4_bc_flag        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt8_4_bc_flag        :  1 ; /* bits 0:0 */
    cs_uint32 kt8_4_msk_bc_flag_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt8_4_mc_flag        :  1 ; /* bits 2:2 */
    cs_uint32 kt8_4_msk_mc_flag_fld :  1 ; /* bits 3:3 */
    cs_uint32 kt8_4_bc_llid        :  1 ; /* bits 4:4 */
    cs_uint32 kt8_4_msk_bc_llid_fld :  1 ; /* bits 5:5 */
    cs_uint32 kt8_4_ev2pt_enc      :  4 ; /* bits 9:6 */
    cs_uint32 kt8_4_msk_ev2pt_enc_fld :  1 ; /* bits 10:10 */
    cs_uint32 kt8_4_tpid_enc       :  3 ; /* bits 13:11 */
    cs_uint32 kt8_4_msk_tpid_enc_fld :  1 ; /* bits 14:14 */
    cs_uint32 kt8_5_vlanid         : 12 ; /* bits 26:15 */
    cs_uint32 kt8_5_msk_vlanid_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt8_5_vlan_cnt       :  2 ; /* bits 29:28 */
    cs_uint32 kt8_5_msk_vlan_cnt_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt8_5_p_tag_flag     :  1 ; /* bits 31:31 */
#   endif
  } bf9 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt9_4_ev2pt_enc      :  2 ; /* bits 31:30 */
    cs_uint32 kt9_4_msk_mc_flag_fld :  1 ; /* bits 29:29 */
    cs_uint32 kt9_4_mc_flag        :  1 ; /* bits 28:28 */
    cs_uint32 kt9_4_msk_bc_flag_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt9_4_bc_flag        :  1 ; /* bits 26:26 */
    cs_uint32 kt9_4_inner_vlan_en  :  1 ; /* bits 25:25 */
    cs_uint32 kt9_4_rsvd           :  1 ; /* bits 24:24 */
    cs_uint32 kt9_4_msk_vlan_cnt_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt9_4_vlan_cnt       :  2 ; /* bits 22:21 */
    cs_uint32 kt9_4_msk_vlanid_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt9_4_vlanid_hi      : 12 ; /* bits 19:8 */
    cs_uint32 kt9_4_vlanid_lo      :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt9_4_vlanid_lo      :  8 ; /* bits 7:0 */
    cs_uint32 kt9_4_vlanid_hi      : 12 ; /* bits 19:8 */
    cs_uint32 kt9_4_msk_vlanid_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt9_4_vlan_cnt       :  2 ; /* bits 22:21 */
    cs_uint32 kt9_4_msk_vlan_cnt_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt9_4_rsvd           :  1 ; /* bits 24:24 */
    cs_uint32 kt9_4_inner_vlan_en  :  1 ; /* bits 25:25 */
    cs_uint32 kt9_4_bc_flag        :  1 ; /* bits 26:26 */
    cs_uint32 kt9_4_msk_bc_flag_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt9_4_mc_flag        :  1 ; /* bits 28:28 */
    cs_uint32 kt9_4_msk_mc_flag_fld :  1 ; /* bits 29:29 */
    cs_uint32 kt9_4_ev2pt_enc      :  2 ; /* bits 31:30 */
#   endif
  } bf10 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt10_2_l4_src_hit_map :  8 ; /* bits 31:24 */
    cs_uint32 kt10_2_spcl_hit_map  :  8 ; /* bits 23:16 */
    cs_uint32 kt10_2_mac_da_low    :  8 ; /* bits 15:8 */
    cs_uint32 kt10_2_ev2pt         :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt10_2_ev2pt         :  8 ; /* bits 7:0 */
    cs_uint32 kt10_2_mac_da_low    :  8 ; /* bits 15:8 */
    cs_uint32 kt10_2_spcl_hit_map  :  8 ; /* bits 23:16 */
    cs_uint32 kt10_2_l4_src_hit_map :  8 ; /* bits 31:24 */
#   endif
  } bf11 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt11_1_addr          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt11_1_addr          : 32 ; /* bits 31:0 */
#   endif
  } bf12 ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_MEM_DATA5_t;





#define FE_LE_CE_KEY_MEM_DATA4                   0x0613b
#define FE_LE_CE_KEY_MEM_DATA4_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt0_ip_da            : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt0_ip_da            : 32 ; /* bits 31:0 */
#   endif
  } bf0 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_0_ip_sa          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_0_ip_sa          : 32 ; /* bits 31:0 */
#   endif
  } bf1 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_1_rsvd0          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_1_rsvd0          : 32 ; /* bits 31:0 */
#   endif
  } bf2 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt1_1_ip_addr        : 10 ; /* bits 31:22 */
    cs_uint32 kt1_2_msk_dscp_tos_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt1_2_msk_ippt_fld   :  1 ; /* bits 20:20 */
    cs_uint32 kt1_2_msk_l4_port_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt1_2_msk_ip_addr_fld :  6 ; /* bits 18:13 */
    cs_uint32 kt1_2_l4_port_type   :  1 ; /* bits 12:12 */
    cs_uint32 kt1_2_ip_addr_type   :  1 ; /* bits 11:11 */
    cs_uint32 kt1_2_dscp_tos       :  8 ; /* bits 10:3 */
    cs_uint32 kt1_2_ippt           :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt1_2_ippt           :  3 ; /* bits 2:0 */
    cs_uint32 kt1_2_dscp_tos       :  8 ; /* bits 10:3 */
    cs_uint32 kt1_2_ip_addr_type   :  1 ; /* bits 11:11 */
    cs_uint32 kt1_2_l4_port_type   :  1 ; /* bits 12:12 */
    cs_uint32 kt1_2_msk_ip_addr_fld :  6 ; /* bits 18:13 */
    cs_uint32 kt1_2_msk_l4_port_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt1_2_msk_ippt_fld   :  1 ; /* bits 20:20 */
    cs_uint32 kt1_2_msk_dscp_tos_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt1_1_ip_addr        : 10 ; /* bits 31:22 */
#   endif
  } bf3 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt3_0_dscp_tos       :  8 ; /* bits 31:24 */
    cs_uint32 kt3_0_ippt           :  8 ; /* bits 23:16 */
    cs_uint32 kt3_0_l4_port        : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt3_0_l4_port        : 16 ; /* bits 15:0 */
    cs_uint32 kt3_0_ippt           :  8 ; /* bits 23:16 */
    cs_uint32 kt3_0_dscp_tos       :  8 ; /* bits 31:24 */
#   endif
  } bf4 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt4_4_msk_spid_fld   :  1 ; /* bits 31:31 */
    cs_uint32 kt4_4_spid           :  3 ; /* bits 30:28 */
    cs_uint32 kt4_3_mirror_pkt_flag :  1 ; /* bits 27:27 */
    cs_uint32 kt4_3_rsvd           : 15 ; /* bits 26:12 */
    cs_uint32 kt4_3_msk_mirror_group_id_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt4_3_mirror_group_id :  5 ; /* bits 10:6 */
    cs_uint32 kt4_3_msk_vlanid_fld :  1 ; /* bits 5:5 */
    cs_uint32 kt4_3_vlanid         :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt4_3_vlanid         :  5 ; /* bits 4:0 */
    cs_uint32 kt4_3_msk_vlanid_fld :  1 ; /* bits 5:5 */
    cs_uint32 kt4_3_mirror_group_id :  5 ; /* bits 10:6 */
    cs_uint32 kt4_3_msk_mirror_group_id_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt4_3_rsvd           : 15 ; /* bits 26:12 */
    cs_uint32 kt4_3_mirror_pkt_flag :  1 ; /* bits 27:27 */
    cs_uint32 kt4_4_spid           :  3 ; /* bits 30:28 */
    cs_uint32 kt4_4_msk_spid_fld   :  1 ; /* bits 31:31 */
#   endif
  } bf5 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt5_1_mac_addr       :  4 ; /* bits 31:28 */
    cs_uint32 kt5_2_age_tmr_offset :  6 ; /* bits 27:22 */
    cs_uint32 kt5_2_msk_802_1p_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt5_2_msk_p_tag_flag_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt5_2_msk_vlan_cnt_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt5_2_msk_vlanid_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt5_2_msk_mac_addr_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt5_2_vl_802_1p      :  3 ; /* bits 16:14 */
    cs_uint32 kt5_2_p_tag_flag     :  1 ; /* bits 13:13 */
    cs_uint32 kt5_2_vlan_cnt       :  2 ; /* bits 12:11 */
    cs_uint32 kt5_2_vlanid         : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt5_2_vlanid         : 11 ; /* bits 10:0 */
    cs_uint32 kt5_2_vlan_cnt       :  2 ; /* bits 12:11 */
    cs_uint32 kt5_2_p_tag_flag     :  1 ; /* bits 13:13 */
    cs_uint32 kt5_2_vl_802_1p      :  3 ; /* bits 16:14 */
    cs_uint32 kt5_2_msk_mac_addr_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt5_2_msk_vlanid_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt5_2_msk_vlan_cnt_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt5_2_msk_p_tag_flag_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt5_2_msk_802_1p_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt5_2_age_tmr_offset :  6 ; /* bits 27:22 */
    cs_uint32 kt5_1_mac_addr       :  4 ; /* bits 31:28 */
#   endif
  } bf6 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt6_1_mac_addr       :  2 ; /* bits 31:30 */
    cs_uint32 kt6_2_msk_mac_da_range_flag_fld :  1 ; /* bits 29:29 */
    cs_uint32 kt6_2_msk_tpid_enc_fld :  1 ; /* bits 28:28 */
    cs_uint32 kt6_2_msk_ev2pt_fld  :  1 ; /* bits 27:27 */
    cs_uint32 kt6_2_msk_mac_addr_fld :  1 ; /* bits 26:26 */
    cs_uint32 kt6_2_mac_da_range_flag :  1 ; /* bits 25:25 */
    cs_uint32 kt6_2_tpid_enc       :  3 ; /* bits 24:22 */
    cs_uint32 kt6_2_age_tmr_offset :  6 ; /* bits 21:16 */
    cs_uint32 kt6_2_mac_type       :  1 ; /* bits 15:15 */
    cs_uint32 kt6_2_ev2pt          : 15 ; /* bits 14:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt6_2_ev2pt          : 15 ; /* bits 14:0 */
    cs_uint32 kt6_2_mac_type       :  1 ; /* bits 15:15 */
    cs_uint32 kt6_2_age_tmr_offset :  6 ; /* bits 21:16 */
    cs_uint32 kt6_2_tpid_enc       :  3 ; /* bits 24:22 */
    cs_uint32 kt6_2_mac_da_range_flag :  1 ; /* bits 25:25 */
    cs_uint32 kt6_2_msk_mac_addr_fld :  1 ; /* bits 26:26 */
    cs_uint32 kt6_2_msk_ev2pt_fld  :  1 ; /* bits 27:27 */
    cs_uint32 kt6_2_msk_tpid_enc_fld :  1 ; /* bits 28:28 */
    cs_uint32 kt6_2_msk_mac_da_range_flag_fld :  1 ; /* bits 29:29 */
    cs_uint32 kt6_1_mac_addr       :  2 ; /* bits 31:30 */
#   endif
  } bf7 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt7_2_mac_da         :  6 ; /* bits 31:26 */
    cs_uint32 kt7_1_msk_ev2pt_fld  :  1 ; /* bits 25:25 */
    cs_uint32 kt7_1_spl_pkt_vec    : 21 ; /* bits 24:4 */
    cs_uint32 kt7_1_spl_pkt_en     :  1 ; /* bits 3:3 */
    cs_uint32 kt7_1_msk_mac_da_fld :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt7_1_msk_mac_da_fld :  3 ; /* bits 2:0 */
    cs_uint32 kt7_1_spl_pkt_en     :  1 ; /* bits 3:3 */
    cs_uint32 kt7_1_spl_pkt_vec    : 21 ; /* bits 24:4 */
    cs_uint32 kt7_1_msk_ev2pt_fld  :  1 ; /* bits 25:25 */
    cs_uint32 kt7_2_mac_da         :  6 ; /* bits 31:26 */
#   endif
  } bf8 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt8_4_msk_mac_da_flag_fld :  1 ; /* bits 31:31 */
    cs_uint32 kt8_4_rsvd_mac_da_flag :  1 ; /* bits 30:30 */
    cs_uint32 kt8_4_msk_p_tag_flag_fld :  1 ; /* bits 29:29 */
    cs_uint32 kt8_4_p_tag_flag     :  1 ; /* bits 28:28 */
    cs_uint32 kt8_4_msk_vlan_cnt_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt8_4_vlan_cnt       :  2 ; /* bits 26:25 */
    cs_uint32 kt8_4_msk_vlanid_fld :  1 ; /* bits 24:24 */
    cs_uint32 kt8_4_vlanid         : 12 ; /* bits 23:12 */
    cs_uint32 kt8_3_msk_tpid_enc_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt8_3_tpid_enc       :  3 ; /* bits 10:8 */
    cs_uint32 kt8_3_msk_ev2pt_enc_fld :  1 ; /* bits 7:7 */
    cs_uint32 kt8_3_ev2pt_enc      :  4 ; /* bits 6:3 */
    cs_uint32 kt8_3_msk_bc_llid_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt8_3_bc_llid        :  1 ; /* bits 1:1 */
    cs_uint32 kt8_3_msk_mc_flag_fld :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt8_3_msk_mc_flag_fld :  1 ; /* bits 0:0 */
    cs_uint32 kt8_3_bc_llid        :  1 ; /* bits 1:1 */
    cs_uint32 kt8_3_msk_bc_llid_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt8_3_ev2pt_enc      :  4 ; /* bits 6:3 */
    cs_uint32 kt8_3_msk_ev2pt_enc_fld :  1 ; /* bits 7:7 */
    cs_uint32 kt8_3_tpid_enc       :  3 ; /* bits 10:8 */
    cs_uint32 kt8_3_msk_tpid_enc_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt8_4_vlanid         : 12 ; /* bits 23:12 */
    cs_uint32 kt8_4_msk_vlanid_fld :  1 ; /* bits 24:24 */
    cs_uint32 kt8_4_vlan_cnt       :  2 ; /* bits 26:25 */
    cs_uint32 kt8_4_msk_vlan_cnt_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt8_4_p_tag_flag     :  1 ; /* bits 28:28 */
    cs_uint32 kt8_4_msk_p_tag_flag_fld :  1 ; /* bits 29:29 */
    cs_uint32 kt8_4_rsvd_mac_da_flag :  1 ; /* bits 30:30 */
    cs_uint32 kt8_4_msk_mac_da_flag_fld :  1 ; /* bits 31:31 */
#   endif
  } bf9 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt9_4_vlanid_lo      :  4 ; /* bits 31:28 */
    cs_uint32 kt9_3_msk_ev2pt_enc_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt9_3_ev2pt_enc      :  4 ; /* bits 26:23 */
    cs_uint32 kt9_3_msk_mc_flag_fld :  1 ; /* bits 22:22 */
    cs_uint32 kt9_3_mc_flag        :  1 ; /* bits 21:21 */
    cs_uint32 kt9_3_msk_bc_flag_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt9_3_bc_flag        :  1 ; /* bits 19:19 */
    cs_uint32 kt9_3_inner_vlan_en  :  1 ; /* bits 18:18 */
    cs_uint32 kt9_3_rsvd           :  1 ; /* bits 17:17 */
    cs_uint32 kt9_3_msk_vlan_cnt_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt9_3_vlan_cnt       :  2 ; /* bits 15:14 */
    cs_uint32 kt9_3_msk_vlanid_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt9_3_vlanid_hi      : 12 ; /* bits 12:1 */
    cs_uint32 kt9_3_vlanid_lo      :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt9_3_vlanid_lo      :  1 ; /* bits 0:0 */
    cs_uint32 kt9_3_vlanid_hi      : 12 ; /* bits 12:1 */
    cs_uint32 kt9_3_msk_vlanid_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt9_3_vlan_cnt       :  2 ; /* bits 15:14 */
    cs_uint32 kt9_3_msk_vlan_cnt_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt9_3_rsvd           :  1 ; /* bits 17:17 */
    cs_uint32 kt9_3_inner_vlan_en  :  1 ; /* bits 18:18 */
    cs_uint32 kt9_3_bc_flag        :  1 ; /* bits 19:19 */
    cs_uint32 kt9_3_msk_bc_flag_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt9_3_mc_flag        :  1 ; /* bits 21:21 */
    cs_uint32 kt9_3_msk_mc_flag_fld :  1 ; /* bits 22:22 */
    cs_uint32 kt9_3_ev2pt_enc      :  4 ; /* bits 26:23 */
    cs_uint32 kt9_3_msk_ev2pt_enc_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt9_4_vlanid_lo      :  4 ; /* bits 31:28 */
#   endif
  } bf10 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt10_2_ev2pt         :  8 ; /* bits 31:24 */
    cs_uint32 kt10_1_msk_tpid_enc_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt10_1_msk_mac_da_flags_fld :  1 ; /* bits 22:22 */
    cs_uint32 kt10_1_msk_ippt_fld  :  1 ; /* bits 21:21 */
    cs_uint32 kt10_1_msk_icmp_type_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt10_1_msk_mac_da_low_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt10_1_msk_ev2pt_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt10_1_tpid_enc      :  3 ; /* bits 17:15 */
    cs_uint32 kt10_1_rsvd_mac_da_flag :  1 ; /* bits 14:14 */
    cs_uint32 kt10_1_mcst_ip_flag  :  1 ; /* bits 13:13 */
    cs_uint32 kt10_1_mymac_flag    :  1 ; /* bits 12:12 */
    cs_uint32 kt10_1_ippt          :  8 ; /* bits 11:4 */
    cs_uint32 kt10_1_icmp_type     :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt10_1_icmp_type     :  4 ; /* bits 3:0 */
    cs_uint32 kt10_1_ippt          :  8 ; /* bits 11:4 */
    cs_uint32 kt10_1_mymac_flag    :  1 ; /* bits 12:12 */
    cs_uint32 kt10_1_mcst_ip_flag  :  1 ; /* bits 13:13 */
    cs_uint32 kt10_1_rsvd_mac_da_flag :  1 ; /* bits 14:14 */
    cs_uint32 kt10_1_tpid_enc      :  3 ; /* bits 17:15 */
    cs_uint32 kt10_1_msk_ev2pt_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt10_1_msk_mac_da_low_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt10_1_msk_icmp_type_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt10_1_msk_ippt_fld  :  1 ; /* bits 21:21 */
    cs_uint32 kt10_1_msk_mac_da_flags_fld :  1 ; /* bits 22:22 */
    cs_uint32 kt10_1_msk_tpid_enc_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt10_2_ev2pt         :  8 ; /* bits 31:24 */
#   endif
  } bf11 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt11_1_addr          :  8 ; /* bits 31:24 */
    cs_uint32 kt11_0_group_id      :  6 ; /* bits 23:18 */
    cs_uint32 kt11_0_group_id_vld  :  1 ; /* bits 17:17 */
    cs_uint32 kt11_0_msk_vlanid_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt11_0_vlanid        : 12 ; /* bits 15:4 */
    cs_uint32 kt11_0_msk_addr_fld  :  1 ; /* bits 3:3 */
    cs_uint32 kt11_0_src_addr_vld  :  1 ; /* bits 2:2 */
    cs_uint32 kt11_0_addr_type     :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt11_0_addr_type     :  2 ; /* bits 1:0 */
    cs_uint32 kt11_0_src_addr_vld  :  1 ; /* bits 2:2 */
    cs_uint32 kt11_0_msk_addr_fld  :  1 ; /* bits 3:3 */
    cs_uint32 kt11_0_vlanid        : 12 ; /* bits 15:4 */
    cs_uint32 kt11_0_msk_vlanid_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt11_0_group_id_vld  :  1 ; /* bits 17:17 */
    cs_uint32 kt11_0_group_id      :  6 ; /* bits 23:18 */
    cs_uint32 kt11_1_addr          :  8 ; /* bits 31:24 */
#   endif
  } bf12 ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_MEM_DATA4_t;





#define FE_LE_CE_KEY_MEM_DATA3                   0x0613c
#define FE_LE_CE_KEY_MEM_DATA3_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt0_ev2pt            : 16 ; /* bits 31:16 */
    cs_uint32 kt0_mac_sa           : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt0_mac_sa           : 16 ; /* bits 15:0 */
    cs_uint32 kt0_ev2pt            : 16 ; /* bits 31:16 */
#   endif
  } bf0 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_0_ip_da          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_0_ip_da          : 32 ; /* bits 31:0 */
#   endif
  } bf1 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_1_ev2pt          : 16 ; /* bits 31:16 */
    cs_uint32 kt2_1_mac_sa         : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_1_mac_sa         : 16 ; /* bits 15:0 */
    cs_uint32 kt2_1_ev2pt          : 16 ; /* bits 31:16 */
#   endif
  } bf2 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt1_2_ippt           :  5 ; /* bits 31:27 */
    cs_uint32 kt1_2_l4_port        : 16 ; /* bits 26:11 */
    cs_uint32 kt1_2_ip_addr        : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt1_2_ip_addr        : 11 ; /* bits 10:0 */
    cs_uint32 kt1_2_l4_port        : 16 ; /* bits 26:11 */
    cs_uint32 kt1_2_ippt           :  5 ; /* bits 31:27 */
#   endif
  } bf3 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt3_0_ip_addr        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt3_0_ip_addr        : 32 ; /* bits 31:0 */
#   endif
  } bf4 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt4_3_vlanid         :  7 ; /* bits 31:25 */
    cs_uint32 kt4_3_msk_spid_fld   :  1 ; /* bits 24:24 */
    cs_uint32 kt4_3_spid           :  3 ; /* bits 23:21 */
    cs_uint32 kt4_2_mirror_pkt_flag :  1 ; /* bits 20:20 */
    cs_uint32 kt4_2_rsvd           : 15 ; /* bits 19:5 */
    cs_uint32 kt4_2_msk_mirror_group_id_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt4_2_mirror_group_id :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt4_2_mirror_group_id :  4 ; /* bits 3:0 */
    cs_uint32 kt4_2_msk_mirror_group_id_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt4_2_rsvd           : 15 ; /* bits 19:5 */
    cs_uint32 kt4_2_mirror_pkt_flag :  1 ; /* bits 20:20 */
    cs_uint32 kt4_3_spid           :  3 ; /* bits 23:21 */
    cs_uint32 kt4_3_msk_spid_fld   :  1 ; /* bits 24:24 */
    cs_uint32 kt4_3_vlanid         :  7 ; /* bits 31:25 */
#   endif
  } bf5 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt5_2_vlanid         :  1 ; /* bits 31:31 */
    cs_uint32 kt5_2_mac_type       :  1 ; /* bits 30:30 */
    cs_uint32 kt5_2_mac_addr       : 30 ; /* bits 29:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt5_2_mac_addr       : 30 ; /* bits 29:0 */
    cs_uint32 kt5_2_mac_type       :  1 ; /* bits 30:30 */
    cs_uint32 kt5_2_vlanid         :  1 ; /* bits 31:31 */
#   endif
  } bf6 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt6_2_ev2pt          :  1 ; /* bits 31:31 */
    cs_uint32 kt6_2_mac_addr       : 31 ; /* bits 30:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt6_2_mac_addr       : 31 ; /* bits 30:0 */
    cs_uint32 kt6_2_ev2pt          :  1 ; /* bits 31:31 */
#   endif
  } bf7 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt7_1_msk_mac_da_fld :  3 ; /* bits 31:29 */
    cs_uint32 kt7_1_mac_da         : 29 ; /* bits 28:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt7_1_mac_da         : 29 ; /* bits 28:0 */
    cs_uint32 kt7_1_msk_mac_da_fld :  3 ; /* bits 31:29 */
#   endif
  } bf8 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt8_3_mc_flag        :  1 ; /* bits 31:31 */
    cs_uint32 kt8_3_msk_bc_flag_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt8_3_bc_flag        :  1 ; /* bits 29:29 */
    cs_uint32 kt8_3_msk_mac_da_flag_fld :  1 ; /* bits 28:28 */
    cs_uint32 kt8_3_rsvd_mac_da_flag :  1 ; /* bits 27:27 */
    cs_uint32 kt8_3_msk_p_tag_flag_fld :  1 ; /* bits 26:26 */
    cs_uint32 kt8_3_p_tag_flag     :  1 ; /* bits 25:25 */
    cs_uint32 kt8_3_msk_vlan_cnt_fld :  1 ; /* bits 24:24 */
    cs_uint32 kt8_3_vlan_cnt       :  2 ; /* bits 23:22 */
    cs_uint32 kt8_3_msk_vlanid_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt8_3_vlanid         : 12 ; /* bits 20:9 */
    cs_uint32 kt8_2_msk_tpid_enc_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt8_2_tpid_enc       :  3 ; /* bits 7:5 */
    cs_uint32 kt8_2_msk_ev2pt_enc_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt8_2_ev2pt_enc      :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt8_2_ev2pt_enc      :  4 ; /* bits 3:0 */
    cs_uint32 kt8_2_msk_ev2pt_enc_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt8_2_tpid_enc       :  3 ; /* bits 7:5 */
    cs_uint32 kt8_2_msk_tpid_enc_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt8_3_vlanid         : 12 ; /* bits 20:9 */
    cs_uint32 kt8_3_msk_vlanid_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt8_3_vlan_cnt       :  2 ; /* bits 23:22 */
    cs_uint32 kt8_3_msk_vlan_cnt_fld :  1 ; /* bits 24:24 */
    cs_uint32 kt8_3_p_tag_flag     :  1 ; /* bits 25:25 */
    cs_uint32 kt8_3_msk_p_tag_flag_fld :  1 ; /* bits 26:26 */
    cs_uint32 kt8_3_rsvd_mac_da_flag :  1 ; /* bits 27:27 */
    cs_uint32 kt8_3_msk_mac_da_flag_fld :  1 ; /* bits 28:28 */
    cs_uint32 kt8_3_bc_flag        :  1 ; /* bits 29:29 */
    cs_uint32 kt8_3_msk_bc_flag_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt8_3_mc_flag        :  1 ; /* bits 31:31 */
#   endif
  } bf9 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt9_3_vlanid_lo      : 11 ; /* bits 31:21 */
    cs_uint32 kt9_2_msk_ev2pt_enc_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt9_2_ev2pt_enc      :  4 ; /* bits 19:16 */
    cs_uint32 kt9_2_msk_mc_flag_fld :  1 ; /* bits 15:15 */
    cs_uint32 kt9_2_mc_flag        :  1 ; /* bits 14:14 */
    cs_uint32 kt9_2_msk_bc_flag_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt9_2_bc_flag        :  1 ; /* bits 12:12 */
    cs_uint32 kt9_2_inner_vlan_en  :  1 ; /* bits 11:11 */
    cs_uint32 kt9_2_rsvd           :  1 ; /* bits 10:10 */
    cs_uint32 kt9_2_msk_vlan_cnt_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt9_2_vlan_cnt       :  2 ; /* bits 8:7 */
    cs_uint32 kt9_2_msk_vlanid_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt9_2_vlanid_hi      :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt9_2_vlanid_hi      :  6 ; /* bits 5:0 */
    cs_uint32 kt9_2_msk_vlanid_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt9_2_vlan_cnt       :  2 ; /* bits 8:7 */
    cs_uint32 kt9_2_msk_vlan_cnt_fld :  1 ; /* bits 9:9 */
    cs_uint32 kt9_2_rsvd           :  1 ; /* bits 10:10 */
    cs_uint32 kt9_2_inner_vlan_en  :  1 ; /* bits 11:11 */
    cs_uint32 kt9_2_bc_flag        :  1 ; /* bits 12:12 */
    cs_uint32 kt9_2_msk_bc_flag_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt9_2_mc_flag        :  1 ; /* bits 14:14 */
    cs_uint32 kt9_2_msk_mc_flag_fld :  1 ; /* bits 15:15 */
    cs_uint32 kt9_2_ev2pt_enc      :  4 ; /* bits 19:16 */
    cs_uint32 kt9_2_msk_ev2pt_enc_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt9_3_vlanid_lo      : 11 ; /* bits 31:21 */
#   endif
  } bf10 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt10_1_icmp_type     :  4 ; /* bits 31:28 */
    cs_uint32 kt10_1_l4_dest_hit_map :  8 ; /* bits 27:20 */
    cs_uint32 kt10_1_l4_src_hit_map :  8 ; /* bits 19:12 */
    cs_uint32 kt10_1_spcl_hit_map  :  8 ; /* bits 11:4 */
    cs_uint32 kt10_1_mac_da_low    :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt10_1_mac_da_low    :  4 ; /* bits 3:0 */
    cs_uint32 kt10_1_spcl_hit_map  :  8 ; /* bits 11:4 */
    cs_uint32 kt10_1_l4_src_hit_map :  8 ; /* bits 19:12 */
    cs_uint32 kt10_1_l4_dest_hit_map :  8 ; /* bits 27:20 */
    cs_uint32 kt10_1_icmp_type     :  4 ; /* bits 31:28 */
#   endif
  } bf11 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt11_0_addr          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt11_0_addr          : 32 ; /* bits 31:0 */
#   endif
  } bf12 ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_MEM_DATA3_t;





#define FE_LE_CE_KEY_MEM_DATA2                   0x0613d
#define FE_LE_CE_KEY_MEM_DATA2_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt0_mac_sa           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt0_mac_sa           : 32 ; /* bits 31:0 */
#   endif
  } bf0 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_0_ip_da          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_0_ip_da          : 32 ; /* bits 31:0 */
#   endif
  } bf1 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_1_mac_sa         : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_1_mac_sa         : 32 ; /* bits 31:0 */
#   endif
  } bf2 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt1_2_ip_addr        : 21 ; /* bits 31:11 */
    cs_uint32 kt1_3_msk_dscp_tos_fld :  1 ; /* bits 10:10 */
    cs_uint32 kt1_3_msk_ippt_fld   :  1 ; /* bits 9:9 */
    cs_uint32 kt1_3_msk_l4_port_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt1_3_msk_ip_addr_fld :  6 ; /* bits 7:2 */
    cs_uint32 kt1_3_l4_port_type   :  1 ; /* bits 1:1 */
    cs_uint32 kt1_3_ip_addr_type   :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt1_3_ip_addr_type   :  1 ; /* bits 0:0 */
    cs_uint32 kt1_3_l4_port_type   :  1 ; /* bits 1:1 */
    cs_uint32 kt1_3_msk_ip_addr_fld :  6 ; /* bits 7:2 */
    cs_uint32 kt1_3_msk_l4_port_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt1_3_msk_ippt_fld   :  1 ; /* bits 9:9 */
    cs_uint32 kt1_3_msk_dscp_tos_fld :  1 ; /* bits 10:10 */
    cs_uint32 kt1_2_ip_addr        : 21 ; /* bits 31:11 */
#   endif
  } bf3 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt3_0_ip_addr        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt3_0_ip_addr        : 32 ; /* bits 31:0 */
#   endif
  } bf4 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt4_2_mirror_group_id :  1 ; /* bits 31:31 */
    cs_uint32 kt4_2_msk_vlanid_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt4_2_vlanid         : 12 ; /* bits 29:18 */
    cs_uint32 kt4_2_msk_spid_fld   :  1 ; /* bits 17:17 */
    cs_uint32 kt4_2_spid           :  3 ; /* bits 16:14 */
    cs_uint32 kt4_1_mirror_pkt_flag :  1 ; /* bits 13:13 */
    cs_uint32 kt4_1_rsvd           : 13 ; /* bits 12:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt4_1_rsvd           : 13 ; /* bits 12:0 */
    cs_uint32 kt4_1_mirror_pkt_flag :  1 ; /* bits 13:13 */
    cs_uint32 kt4_2_spid           :  3 ; /* bits 16:14 */
    cs_uint32 kt4_2_msk_spid_fld   :  1 ; /* bits 17:17 */
    cs_uint32 kt4_2_vlanid         : 12 ; /* bits 29:18 */
    cs_uint32 kt4_2_msk_vlanid_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt4_2_mirror_group_id :  1 ; /* bits 31:31 */
#   endif
  } bf5 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt5_2_mac_addr       : 18 ; /* bits 31:14 */
    cs_uint32 kt5_3_age_tmr_offset :  6 ; /* bits 13:8 */
    cs_uint32 kt5_3_msk_802_1p_fld :  1 ; /* bits 7:7 */
    cs_uint32 kt5_3_msk_p_tag_flag_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt5_3_msk_vlan_cnt_fld :  1 ; /* bits 5:5 */
    cs_uint32 kt5_3_msk_vlanid_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt5_3_msk_mac_addr_fld :  1 ; /* bits 3:3 */
    cs_uint32 kt5_3_vl_802_1p      :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt5_3_vl_802_1p      :  3 ; /* bits 2:0 */
    cs_uint32 kt5_3_msk_mac_addr_fld :  1 ; /* bits 3:3 */
    cs_uint32 kt5_3_msk_vlanid_fld :  1 ; /* bits 4:4 */
    cs_uint32 kt5_3_msk_vlan_cnt_fld :  1 ; /* bits 5:5 */
    cs_uint32 kt5_3_msk_p_tag_flag_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt5_3_msk_802_1p_fld :  1 ; /* bits 7:7 */
    cs_uint32 kt5_3_age_tmr_offset :  6 ; /* bits 13:8 */
    cs_uint32 kt5_2_mac_addr       : 18 ; /* bits 31:14 */
#   endif
  } bf6 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt6_2_mac_addr       : 17 ; /* bits 31:15 */
    cs_uint32 kt6_3_msk_mac_da_range_flag_fld :  1 ; /* bits 14:14 */
    cs_uint32 kt6_3_msk_tpid_enc_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt6_3_msk_ev2pt_fld  :  1 ; /* bits 12:12 */
    cs_uint32 kt6_3_msk_mac_addr_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt6_3_mac_da_range_flag :  1 ; /* bits 10:10 */
    cs_uint32 kt6_3_tpid_enc       :  3 ; /* bits 9:7 */
    cs_uint32 kt6_3_age_tmr_offset :  6 ; /* bits 6:1 */
    cs_uint32 kt6_3_mac_type       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt6_3_mac_type       :  1 ; /* bits 0:0 */
    cs_uint32 kt6_3_age_tmr_offset :  6 ; /* bits 6:1 */
    cs_uint32 kt6_3_tpid_enc       :  3 ; /* bits 9:7 */
    cs_uint32 kt6_3_mac_da_range_flag :  1 ; /* bits 10:10 */
    cs_uint32 kt6_3_msk_mac_addr_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt6_3_msk_ev2pt_fld  :  1 ; /* bits 12:12 */
    cs_uint32 kt6_3_msk_tpid_enc_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt6_3_msk_mac_da_range_flag_fld :  1 ; /* bits 14:14 */
    cs_uint32 kt6_2_mac_addr       : 17 ; /* bits 31:15 */
#   endif
  } bf7 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt7_1_mac_da         : 19 ; /* bits 31:13 */
    cs_uint32 kt7_0_msk_ev2pt_fld  :  1 ; /* bits 12:12 */
    cs_uint32 kt7_0_spl_pkt_vec    : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt7_0_spl_pkt_vec    : 12 ; /* bits 11:0 */
    cs_uint32 kt7_0_msk_ev2pt_fld  :  1 ; /* bits 12:12 */
    cs_uint32 kt7_1_mac_da         : 19 ; /* bits 31:13 */
#   endif
  } bf8 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt8_2_msk_bc_llid_fld :  1 ; /* bits 31:31 */
    cs_uint32 kt8_2_bc_llid        :  1 ; /* bits 30:30 */
    cs_uint32 kt8_2_msk_mc_flag_fld :  1 ; /* bits 29:29 */
    cs_uint32 kt8_2_mc_flag        :  1 ; /* bits 28:28 */
    cs_uint32 kt8_2_msk_bc_flag_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt8_2_bc_flag        :  1 ; /* bits 26:26 */
    cs_uint32 kt8_2_msk_mac_da_flag_fld :  1 ; /* bits 25:25 */
    cs_uint32 kt8_2_rsvd_mac_da_flag :  1 ; /* bits 24:24 */
    cs_uint32 kt8_2_msk_p_tag_flag_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt8_2_p_tag_flag     :  1 ; /* bits 22:22 */
    cs_uint32 kt8_2_msk_vlan_cnt_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt8_2_vlan_cnt       :  2 ; /* bits 20:19 */
    cs_uint32 kt8_2_msk_vlanid_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt8_2_vlanid         : 12 ; /* bits 17:6 */
    cs_uint32 kt8_1_msk_tpid_enc_fld :  1 ; /* bits 5:5 */
    cs_uint32 kt8_1_tpid_enc       :  3 ; /* bits 4:2 */
    cs_uint32 kt8_1_msk_ev2pt_enc_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt8_1_ev2pt_enc      :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt8_1_ev2pt_enc      :  1 ; /* bits 0:0 */
    cs_uint32 kt8_1_msk_ev2pt_enc_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt8_1_tpid_enc       :  3 ; /* bits 4:2 */
    cs_uint32 kt8_1_msk_tpid_enc_fld :  1 ; /* bits 5:5 */
    cs_uint32 kt8_2_vlanid         : 12 ; /* bits 17:6 */
    cs_uint32 kt8_2_msk_vlanid_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt8_2_vlan_cnt       :  2 ; /* bits 20:19 */
    cs_uint32 kt8_2_msk_vlan_cnt_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt8_2_p_tag_flag     :  1 ; /* bits 22:22 */
    cs_uint32 kt8_2_msk_p_tag_flag_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt8_2_rsvd_mac_da_flag :  1 ; /* bits 24:24 */
    cs_uint32 kt8_2_msk_mac_da_flag_fld :  1 ; /* bits 25:25 */
    cs_uint32 kt8_2_bc_flag        :  1 ; /* bits 26:26 */
    cs_uint32 kt8_2_msk_bc_flag_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt8_2_mc_flag        :  1 ; /* bits 28:28 */
    cs_uint32 kt8_2_msk_mc_flag_fld :  1 ; /* bits 29:29 */
    cs_uint32 kt8_2_bc_llid        :  1 ; /* bits 30:30 */
    cs_uint32 kt8_2_msk_bc_llid_fld :  1 ; /* bits 31:31 */
#   endif
  } bf9 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt9_2_vlanid_hi      :  6 ; /* bits 31:26 */
    cs_uint32 kt9_2_vlanid_lo      : 12 ; /* bits 25:14 */
    cs_uint32 kt9_1_msk_ev2pt_enc_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt9_1_ev2pt_enc      :  4 ; /* bits 12:9 */
    cs_uint32 kt9_1_msk_mc_flag_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt9_1_mc_flag        :  1 ; /* bits 7:7 */
    cs_uint32 kt9_1_msk_bc_flag_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt9_1_bc_flag        :  1 ; /* bits 5:5 */
    cs_uint32 kt9_1_inner_vlan_en  :  1 ; /* bits 4:4 */
    cs_uint32 kt9_1_rsvd           :  1 ; /* bits 3:3 */
    cs_uint32 kt9_1_msk_vlan_cnt_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt9_1_vlan_cnt       :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt9_1_vlan_cnt       :  2 ; /* bits 1:0 */
    cs_uint32 kt9_1_msk_vlan_cnt_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt9_1_rsvd           :  1 ; /* bits 3:3 */
    cs_uint32 kt9_1_inner_vlan_en  :  1 ; /* bits 4:4 */
    cs_uint32 kt9_1_bc_flag        :  1 ; /* bits 5:5 */
    cs_uint32 kt9_1_msk_bc_flag_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt9_1_mc_flag        :  1 ; /* bits 7:7 */
    cs_uint32 kt9_1_msk_mc_flag_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt9_1_ev2pt_enc      :  4 ; /* bits 12:9 */
    cs_uint32 kt9_1_msk_ev2pt_enc_fld :  1 ; /* bits 13:13 */
    cs_uint32 kt9_2_vlanid_lo      : 12 ; /* bits 25:14 */
    cs_uint32 kt9_2_vlanid_hi      :  6 ; /* bits 31:26 */
#   endif
  } bf10 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt10_1_mac_da_low    :  4 ; /* bits 31:28 */
    cs_uint32 kt10_1_ev2pt         : 16 ; /* bits 27:12 */
    cs_uint32 kt10_0_msk_tpid_enc_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt10_0_msk_mac_da_flags_fld :  1 ; /* bits 10:10 */
    cs_uint32 kt10_0_msk_ippt_fld  :  1 ; /* bits 9:9 */
    cs_uint32 kt10_0_msk_icmp_type_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt10_0_msk_mac_da_low_fld :  1 ; /* bits 7:7 */
    cs_uint32 kt10_0_msk_ev2pt_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt10_0_tpid_enc      :  3 ; /* bits 5:3 */
    cs_uint32 kt10_0_rsvd_mac_da_flag :  1 ; /* bits 2:2 */
    cs_uint32 kt10_0_mcst_ip_flag  :  1 ; /* bits 1:1 */
    cs_uint32 kt10_0_mymac_flag    :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt10_0_mymac_flag    :  1 ; /* bits 0:0 */
    cs_uint32 kt10_0_mcst_ip_flag  :  1 ; /* bits 1:1 */
    cs_uint32 kt10_0_rsvd_mac_da_flag :  1 ; /* bits 2:2 */
    cs_uint32 kt10_0_tpid_enc      :  3 ; /* bits 5:3 */
    cs_uint32 kt10_0_msk_ev2pt_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt10_0_msk_mac_da_low_fld :  1 ; /* bits 7:7 */
    cs_uint32 kt10_0_msk_icmp_type_fld :  1 ; /* bits 8:8 */
    cs_uint32 kt10_0_msk_ippt_fld  :  1 ; /* bits 9:9 */
    cs_uint32 kt10_0_msk_mac_da_flags_fld :  1 ; /* bits 10:10 */
    cs_uint32 kt10_0_msk_tpid_enc_fld :  1 ; /* bits 11:11 */
    cs_uint32 kt10_1_ev2pt         : 16 ; /* bits 27:12 */
    cs_uint32 kt10_1_mac_da_low    :  4 ; /* bits 31:28 */
#   endif
  } bf11 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt11_0_addr          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt11_0_addr          : 32 ; /* bits 31:0 */
#   endif
  } bf12 ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_MEM_DATA2_t;





#define FE_LE_CE_KEY_MEM_DATA1                   0x0613e
#define FE_LE_CE_KEY_MEM_DATA1_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt0_prsr_len_excd    :  1 ; /* bits 31:31 */
    cs_uint32 kt0_vl_802_1p        :  3 ; /* bits 30:28 */
    cs_uint32 kt0_vlanid           : 12 ; /* bits 27:16 */
    cs_uint32 kt0_mac_da           : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt0_mac_da           : 16 ; /* bits 15:0 */
    cs_uint32 kt0_vlanid           : 12 ; /* bits 27:16 */
    cs_uint32 kt0_vl_802_1p        :  3 ; /* bits 30:28 */
    cs_uint32 kt0_prsr_len_excd    :  1 ; /* bits 31:31 */
#   endif
  } bf0 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_0_ip_da          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_0_ip_da          : 32 ; /* bits 31:0 */
#   endif
  } bf1 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_1_prsr_len_excd  :  1 ; /* bits 31:31 */
    cs_uint32 kt2_1_vl_802_1p      :  3 ; /* bits 30:28 */
    cs_uint32 kt2_1_vlanid         : 12 ; /* bits 27:16 */
    cs_uint32 kt2_1_mac_da         : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_1_mac_da         : 16 ; /* bits 15:0 */
    cs_uint32 kt2_1_vlanid         : 12 ; /* bits 27:16 */
    cs_uint32 kt2_1_vl_802_1p      :  3 ; /* bits 30:28 */
    cs_uint32 kt2_1_prsr_len_excd  :  1 ; /* bits 31:31 */
#   endif
  } bf2 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt1_3_dscp_tos       :  8 ; /* bits 31:24 */
    cs_uint32 kt1_3_ippt           :  8 ; /* bits 23:16 */
    cs_uint32 kt1_3_l4_port        : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt1_3_l4_port        : 16 ; /* bits 15:0 */
    cs_uint32 kt1_3_ippt           :  8 ; /* bits 23:16 */
    cs_uint32 kt1_3_dscp_tos       :  8 ; /* bits 31:24 */
#   endif
  } bf3 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt3_0_ip_addr        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt3_0_ip_addr        : 32 ; /* bits 31:0 */
#   endif
  } bf4 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt4_1_rsvd           :  2 ; /* bits 31:30 */
    cs_uint32 kt4_1_msk_mirror_group_id_fld :  1 ; /* bits 29:29 */
    cs_uint32 kt4_1_mirror_group_id :  5 ; /* bits 28:24 */
    cs_uint32 kt4_1_msk_vlanid_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt4_1_vlanid         : 12 ; /* bits 22:11 */
    cs_uint32 kt4_1_msk_spid_fld   :  1 ; /* bits 10:10 */
    cs_uint32 kt4_1_spid           :  3 ; /* bits 9:7 */
    cs_uint32 kt4_0_mirror_pkt_flag :  1 ; /* bits 6:6 */
    cs_uint32 kt4_0_rsvd           :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt4_0_rsvd           :  6 ; /* bits 5:0 */
    cs_uint32 kt4_0_mirror_pkt_flag :  1 ; /* bits 6:6 */
    cs_uint32 kt4_1_spid           :  3 ; /* bits 9:7 */
    cs_uint32 kt4_1_msk_spid_fld   :  1 ; /* bits 10:10 */
    cs_uint32 kt4_1_vlanid         : 12 ; /* bits 22:11 */
    cs_uint32 kt4_1_msk_vlanid_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt4_1_mirror_group_id :  5 ; /* bits 28:24 */
    cs_uint32 kt4_1_msk_mirror_group_id_fld :  1 ; /* bits 29:29 */
    cs_uint32 kt4_1_rsvd           :  2 ; /* bits 31:30 */
#   endif
  } bf5 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt5_3_p_tag_flag     :  1 ; /* bits 31:31 */
    cs_uint32 kt5_3_vlan_cnt       :  2 ; /* bits 30:29 */
    cs_uint32 kt5_3_vlanid         : 12 ; /* bits 28:17 */
    cs_uint32 kt5_3_mac_type       :  1 ; /* bits 16:16 */
    cs_uint32 kt5_3_mac_addr       : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt5_3_mac_addr       : 16 ; /* bits 15:0 */
    cs_uint32 kt5_3_mac_type       :  1 ; /* bits 16:16 */
    cs_uint32 kt5_3_vlanid         : 12 ; /* bits 28:17 */
    cs_uint32 kt5_3_vlan_cnt       :  2 ; /* bits 30:29 */
    cs_uint32 kt5_3_p_tag_flag     :  1 ; /* bits 31:31 */
#   endif
  } bf6 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt6_3_ev2pt          : 16 ; /* bits 31:16 */
    cs_uint32 kt6_3_mac_addr       : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt6_3_mac_addr       : 16 ; /* bits 15:0 */
    cs_uint32 kt6_3_ev2pt          : 16 ; /* bits 31:16 */
#   endif
  } bf7 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt7_0_spl_pkt_vec    :  9 ; /* bits 31:23 */
    cs_uint32 kt7_0_spl_pkt_en     :  1 ; /* bits 22:22 */
    cs_uint32 kt7_0_msk_mac_da_fld :  6 ; /* bits 21:16 */
    cs_uint32 kt7_0_mac_da         : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt7_0_mac_da         : 16 ; /* bits 15:0 */
    cs_uint32 kt7_0_msk_mac_da_fld :  6 ; /* bits 21:16 */
    cs_uint32 kt7_0_spl_pkt_en     :  1 ; /* bits 22:22 */
    cs_uint32 kt7_0_spl_pkt_vec    :  9 ; /* bits 31:23 */
#   endif
  } bf8 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt8_1_ev2pt_enc      :  3 ; /* bits 31:29 */
    cs_uint32 kt8_1_msk_bc_llid_fld :  1 ; /* bits 28:28 */
    cs_uint32 kt8_1_bc_llid        :  1 ; /* bits 27:27 */
    cs_uint32 kt8_1_msk_mc_flag_fld :  1 ; /* bits 26:26 */
    cs_uint32 kt8_1_mc_flag        :  1 ; /* bits 25:25 */
    cs_uint32 kt8_1_msk_bc_flag_fld :  1 ; /* bits 24:24 */
    cs_uint32 kt8_1_bc_flag        :  1 ; /* bits 23:23 */
    cs_uint32 kt8_1_msk_mac_da_flag_fld :  1 ; /* bits 22:22 */
    cs_uint32 kt8_1_rsvd_mac_da_flag :  1 ; /* bits 21:21 */
    cs_uint32 kt8_1_msk_p_tag_flag_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt8_1_p_tag_flag     :  1 ; /* bits 19:19 */
    cs_uint32 kt8_1_msk_vlan_cnt_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt8_1_vlan_cnt       :  2 ; /* bits 17:16 */
    cs_uint32 kt8_1_msk_vlanid_fld :  1 ; /* bits 15:15 */
    cs_uint32 kt8_1_vlanid         : 12 ; /* bits 14:3 */
    cs_uint32 kt8_0_msk_tpid_enc_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt8_0_tpid_enc       :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt8_0_tpid_enc       :  2 ; /* bits 1:0 */
    cs_uint32 kt8_0_msk_tpid_enc_fld :  1 ; /* bits 2:2 */
    cs_uint32 kt8_1_vlanid         : 12 ; /* bits 14:3 */
    cs_uint32 kt8_1_msk_vlanid_fld :  1 ; /* bits 15:15 */
    cs_uint32 kt8_1_vlan_cnt       :  2 ; /* bits 17:16 */
    cs_uint32 kt8_1_msk_vlan_cnt_fld :  1 ; /* bits 18:18 */
    cs_uint32 kt8_1_p_tag_flag     :  1 ; /* bits 19:19 */
    cs_uint32 kt8_1_msk_p_tag_flag_fld :  1 ; /* bits 20:20 */
    cs_uint32 kt8_1_rsvd_mac_da_flag :  1 ; /* bits 21:21 */
    cs_uint32 kt8_1_msk_mac_da_flag_fld :  1 ; /* bits 22:22 */
    cs_uint32 kt8_1_bc_flag        :  1 ; /* bits 23:23 */
    cs_uint32 kt8_1_msk_bc_flag_fld :  1 ; /* bits 24:24 */
    cs_uint32 kt8_1_mc_flag        :  1 ; /* bits 25:25 */
    cs_uint32 kt8_1_msk_mc_flag_fld :  1 ; /* bits 26:26 */
    cs_uint32 kt8_1_bc_llid        :  1 ; /* bits 27:27 */
    cs_uint32 kt8_1_msk_bc_llid_fld :  1 ; /* bits 28:28 */
    cs_uint32 kt8_1_ev2pt_enc      :  3 ; /* bits 31:29 */
#   endif
  } bf9 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt9_1_msk_vlanid_fld :  1 ; /* bits 31:31 */
    cs_uint32 kt9_1_vlanid_hi      : 12 ; /* bits 30:19 */
    cs_uint32 kt9_1_vlanid_lo      : 12 ; /* bits 18:7 */
    cs_uint32 kt9_0_msk_ev2pt_enc_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt9_0_ev2pt_enc      :  4 ; /* bits 5:2 */
    cs_uint32 kt9_0_msk_mc_flag_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt9_0_mc_flag        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt9_0_mc_flag        :  1 ; /* bits 0:0 */
    cs_uint32 kt9_0_msk_mc_flag_fld :  1 ; /* bits 1:1 */
    cs_uint32 kt9_0_ev2pt_enc      :  4 ; /* bits 5:2 */
    cs_uint32 kt9_0_msk_ev2pt_enc_fld :  1 ; /* bits 6:6 */
    cs_uint32 kt9_1_vlanid_lo      : 12 ; /* bits 18:7 */
    cs_uint32 kt9_1_vlanid_hi      : 12 ; /* bits 30:19 */
    cs_uint32 kt9_1_msk_vlanid_fld :  1 ; /* bits 31:31 */
#   endif
  } bf10 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt10_0_ippt          :  8 ; /* bits 31:24 */
    cs_uint32 kt10_0_icmp_type     :  8 ; /* bits 23:16 */
    cs_uint32 kt10_0_l4_dest_hit_map :  8 ; /* bits 15:8 */
    cs_uint32 kt10_0_l4_src_hit_map :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt10_0_l4_src_hit_map :  8 ; /* bits 7:0 */
    cs_uint32 kt10_0_l4_dest_hit_map :  8 ; /* bits 15:8 */
    cs_uint32 kt10_0_icmp_type     :  8 ; /* bits 23:16 */
    cs_uint32 kt10_0_ippt          :  8 ; /* bits 31:24 */
#   endif
  } bf11 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt11_0_addr          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt11_0_addr          : 32 ; /* bits 31:0 */
#   endif
  } bf12 ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_MEM_DATA1_t;





#define FE_LE_CE_KEY_MEM_DATA0                   0x0613f
#define FE_LE_CE_KEY_MEM_DATA0_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt0_mac_da           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt0_mac_da           : 32 ; /* bits 31:0 */
#   endif
  } bf0 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_0_ip_da          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_0_ip_da          : 32 ; /* bits 31:0 */
#   endif
  } bf1 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt2_1_mac_da         : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt2_1_mac_da         : 32 ; /* bits 31:0 */
#   endif
  } bf2 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt1_3_ip_addr        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt1_3_ip_addr        : 32 ; /* bits 31:0 */
#   endif
  } bf3 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt3_0_ip_addr        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt3_0_ip_addr        : 32 ; /* bits 31:0 */
#   endif
  } bf4 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt4_0_rsvd           :  9 ; /* bits 31:23 */
    cs_uint32 kt4_0_msk_mirror_group_id_fld :  1 ; /* bits 22:22 */
    cs_uint32 kt4_0_mirror_group_id :  5 ; /* bits 21:17 */
    cs_uint32 kt4_0_msk_vlanid_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt4_0_vlanid         : 12 ; /* bits 15:4 */
    cs_uint32 kt4_0_msk_spid_fld   :  1 ; /* bits 3:3 */
    cs_uint32 kt4_0_spid           :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt4_0_spid           :  3 ; /* bits 2:0 */
    cs_uint32 kt4_0_msk_spid_fld   :  1 ; /* bits 3:3 */
    cs_uint32 kt4_0_vlanid         : 12 ; /* bits 15:4 */
    cs_uint32 kt4_0_msk_vlanid_fld :  1 ; /* bits 16:16 */
    cs_uint32 kt4_0_mirror_group_id :  5 ; /* bits 21:17 */
    cs_uint32 kt4_0_msk_mirror_group_id_fld :  1 ; /* bits 22:22 */
    cs_uint32 kt4_0_rsvd           :  9 ; /* bits 31:23 */
#   endif
  } bf5 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt5_3_mac_addr       : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt5_3_mac_addr       : 32 ; /* bits 31:0 */
#   endif
  } bf6 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt6_3_mac_addr       : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt6_3_mac_addr       : 32 ; /* bits 31:0 */
#   endif
  } bf7 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt7_0_mac_da         : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt7_0_mac_da         : 32 ; /* bits 31:0 */
#   endif
  } bf8 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt8_0_tpid_enc       :  1 ; /* bits 31:31 */
    cs_uint32 kt8_0_msk_ev2pt_enc_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt8_0_ev2pt_enc      :  4 ; /* bits 29:26 */
    cs_uint32 kt8_0_msk_bc_llid_fld :  1 ; /* bits 25:25 */
    cs_uint32 kt8_0_bc_llid        :  1 ; /* bits 24:24 */
    cs_uint32 kt8_0_msk_mc_flag_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt8_0_mc_flag        :  1 ; /* bits 22:22 */
    cs_uint32 kt8_0_msk_bc_flag_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt8_0_bc_flag        :  1 ; /* bits 20:20 */
    cs_uint32 kt8_0_msk_mac_da_flag_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt8_0_rsvd_mac_da_flag :  1 ; /* bits 18:18 */
    cs_uint32 kt8_0_msk_p_tag_flag_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt8_0_p_tag_flag     :  1 ; /* bits 16:16 */
    cs_uint32 kt8_0_msk_vlan_cnt_fld :  1 ; /* bits 15:15 */
    cs_uint32 kt8_0_vlan_cnt       :  2 ; /* bits 14:13 */
    cs_uint32 kt8_0_msk_vlanid_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt8_0_vlanid         : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt8_0_vlanid         : 12 ; /* bits 11:0 */
    cs_uint32 kt8_0_msk_vlanid_fld :  1 ; /* bits 12:12 */
    cs_uint32 kt8_0_vlan_cnt       :  2 ; /* bits 14:13 */
    cs_uint32 kt8_0_msk_vlan_cnt_fld :  1 ; /* bits 15:15 */
    cs_uint32 kt8_0_p_tag_flag     :  1 ; /* bits 16:16 */
    cs_uint32 kt8_0_msk_p_tag_flag_fld :  1 ; /* bits 17:17 */
    cs_uint32 kt8_0_rsvd_mac_da_flag :  1 ; /* bits 18:18 */
    cs_uint32 kt8_0_msk_mac_da_flag_fld :  1 ; /* bits 19:19 */
    cs_uint32 kt8_0_bc_flag        :  1 ; /* bits 20:20 */
    cs_uint32 kt8_0_msk_bc_flag_fld :  1 ; /* bits 21:21 */
    cs_uint32 kt8_0_mc_flag        :  1 ; /* bits 22:22 */
    cs_uint32 kt8_0_msk_mc_flag_fld :  1 ; /* bits 23:23 */
    cs_uint32 kt8_0_bc_llid        :  1 ; /* bits 24:24 */
    cs_uint32 kt8_0_msk_bc_llid_fld :  1 ; /* bits 25:25 */
    cs_uint32 kt8_0_ev2pt_enc      :  4 ; /* bits 29:26 */
    cs_uint32 kt8_0_msk_ev2pt_enc_fld :  1 ; /* bits 30:30 */
    cs_uint32 kt8_0_tpid_enc       :  1 ; /* bits 31:31 */
#   endif
  } bf9 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt9_0_msk_bc_flag_fld :  1 ; /* bits 31:31 */
    cs_uint32 kt9_0_bc_flag        :  1 ; /* bits 30:30 */
    cs_uint32 kt9_0_inner_vlan_en  :  1 ; /* bits 29:29 */
    cs_uint32 kt9_0_rsvd           :  1 ; /* bits 28:28 */
    cs_uint32 kt9_0_msk_vlan_cnt_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt9_0_vlan_cnt       :  2 ; /* bits 26:25 */
    cs_uint32 kt9_0_msk_vlanid_fld :  1 ; /* bits 24:24 */
    cs_uint32 kt9_0_vlanid_hi      : 12 ; /* bits 23:12 */
    cs_uint32 kt9_0_vlanid_lo      : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt9_0_vlanid_lo      : 12 ; /* bits 11:0 */
    cs_uint32 kt9_0_vlanid_hi      : 12 ; /* bits 23:12 */
    cs_uint32 kt9_0_msk_vlanid_fld :  1 ; /* bits 24:24 */
    cs_uint32 kt9_0_vlan_cnt       :  2 ; /* bits 26:25 */
    cs_uint32 kt9_0_msk_vlan_cnt_fld :  1 ; /* bits 27:27 */
    cs_uint32 kt9_0_rsvd           :  1 ; /* bits 28:28 */
    cs_uint32 kt9_0_inner_vlan_en  :  1 ; /* bits 29:29 */
    cs_uint32 kt9_0_bc_flag        :  1 ; /* bits 30:30 */
    cs_uint32 kt9_0_msk_bc_flag_fld :  1 ; /* bits 31:31 */
#   endif
  } bf10 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt10_0_spcl_hit_map  :  8 ; /* bits 31:24 */
    cs_uint32 kt10_0_mac_da_low    :  8 ; /* bits 23:16 */
    cs_uint32 kt10_0_ev2pt         : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt10_0_ev2pt         : 16 ; /* bits 15:0 */
    cs_uint32 kt10_0_mac_da_low    :  8 ; /* bits 23:16 */
    cs_uint32 kt10_0_spcl_hit_map  :  8 ; /* bits 31:24 */
#   endif
  } bf11 ;
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 kt11_0_addr          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 kt11_0_addr          : 32 ; /* bits 31:0 */
#   endif
  } bf12 ;
  cs_uint32     wrd ;
} FE_LE_CE_KEY_MEM_DATA0_t;





#define FE_LE_CE_FIB_MEM_ACCESS                  0x06140
#define FE_LE_CE_FIB_MEM_ACCESS_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 21 ;
    cs_uint32 address              :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 address              :  9 ; /* bits 8:0 */
    cs_uint32 rsrvd1               : 21 ;
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_FIB_MEM_ACCESS_t;





#define FE_LE_CE_FIB_MEM_DATA2                   0x06141
#define FE_LE_CE_FIB_MEM_DATA2_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 mirror_en            :  1 ; /* bits 25:25 */
    cs_uint32 mirror_pkt_src_sel   :  1 ; /* bits 24:24 */
    cs_uint32 valid                :  1 ; /* bits 23:23 */
    cs_uint32 permit_filter_dis    :  1 ; /* bits 22:22 */
    cs_uint32 permit               :  1 ; /* bits 21:21 */
    cs_uint32 permit_priority      :  1 ; /* bits 20:20 */
    cs_uint32 rate_lmt_rslt_valid  :  1 ; /* bits 19:19 */
    cs_uint32 rate_lmt_bypass      :  1 ; /* bits 18:18 */
    cs_uint32 flow_id_sel          :  3 ; /* bits 17:15 */
    cs_uint32 flow_id              :  5 ; /* bits 14:10 */
    cs_uint32 dpid_rslt_valid      :  1 ; /* bits 9:9 */
    cs_uint32 dpid                 :  3 ; /* bits 8:6 */
    cs_uint32 dscp_rslt_valid      :  1 ; /* bits 5:5 */
    cs_uint32 dscp_sel             :  3 ; /* bits 4:2 */
    cs_uint32 dscp                 :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dscp                 :  2 ; /* bits 1:0 */
    cs_uint32 dscp_sel             :  3 ; /* bits 4:2 */
    cs_uint32 dscp_rslt_valid      :  1 ; /* bits 5:5 */
    cs_uint32 dpid                 :  3 ; /* bits 8:6 */
    cs_uint32 dpid_rslt_valid      :  1 ; /* bits 9:9 */
    cs_uint32 flow_id              :  5 ; /* bits 14:10 */
    cs_uint32 flow_id_sel          :  3 ; /* bits 17:15 */
    cs_uint32 rate_lmt_bypass      :  1 ; /* bits 18:18 */
    cs_uint32 rate_lmt_rslt_valid  :  1 ; /* bits 19:19 */
    cs_uint32 permit_priority      :  1 ; /* bits 20:20 */
    cs_uint32 permit               :  1 ; /* bits 21:21 */
    cs_uint32 permit_filter_dis    :  1 ; /* bits 22:22 */
    cs_uint32 valid                :  1 ; /* bits 23:23 */
    cs_uint32 mirror_pkt_src_sel   :  1 ; /* bits 24:24 */
    cs_uint32 mirror_en            :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_FIB_MEM_DATA2_t;





#define FE_LE_CE_FIB_MEM_DATA1                   0x06142
#define FE_LE_CE_FIB_MEM_DATA1_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 dscp                 :  4 ; /* bits 31:28 */
    cs_uint32 vlan_rslt_valid      :  1 ; /* bits 27:27 */
    cs_uint32 egr_chk_vlan_sel     :  2 ; /* bits 26:25 */
    cs_uint32 inner_tpid_sel       :  3 ; /* bits 24:22 */
    cs_uint32 inner_vlan_cmd       :  2 ; /* bits 21:20 */
    cs_uint32 inner_vlanid         : 12 ; /* bits 19:8 */
    cs_uint32 top_tpid_sel         :  3 ; /* bits 7:5 */
    cs_uint32 top_vlanid_cmd       :  2 ; /* bits 4:3 */
    cs_uint32 top_vlanid_sel       :  1 ; /* bits 2:2 */
    cs_uint32 top_vlanid           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 top_vlanid           :  2 ; /* bits 1:0 */
    cs_uint32 top_vlanid_sel       :  1 ; /* bits 2:2 */
    cs_uint32 top_vlanid_cmd       :  2 ; /* bits 4:3 */
    cs_uint32 top_tpid_sel         :  3 ; /* bits 7:5 */
    cs_uint32 inner_vlanid         : 12 ; /* bits 19:8 */
    cs_uint32 inner_vlan_cmd       :  2 ; /* bits 21:20 */
    cs_uint32 inner_tpid_sel       :  3 ; /* bits 24:22 */
    cs_uint32 egr_chk_vlan_sel     :  2 ; /* bits 26:25 */
    cs_uint32 vlan_rslt_valid      :  1 ; /* bits 27:27 */
    cs_uint32 dscp                 :  4 ; /* bits 31:28 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_FIB_MEM_DATA1_t;





#define FE_LE_CE_FIB_MEM_DATA0                   0x06143
#define FE_LE_CE_FIB_MEM_DATA0_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 top_vlanid           : 10 ; /* bits 31:22 */
    cs_uint32 dot1p_rslt_valid     :  1 ; /* bits 21:21 */
    cs_uint32 inner_802_1p_sel     :  3 ; /* bits 20:18 */
    cs_uint32 inner_802_1p         :  3 ; /* bits 17:15 */
    cs_uint32 top_802_1p_sel       :  3 ; /* bits 14:12 */
    cs_uint32 top_802_1p           :  3 ; /* bits 11:9 */
    cs_uint32 l2_rslt_valid        :  1 ; /* bits 8:8 */
    cs_uint32 l2_sa_limit_fwd_permit :  1 ; /* bits 7:7 */
    cs_uint32 learn_dis            :  1 ; /* bits 6:6 */
    cs_uint32 cos_rslt_valid       :  1 ; /* bits 5:5 */
    cs_uint32 cos_sel              :  2 ; /* bits 4:3 */
    cs_uint32 cos                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cos                  :  3 ; /* bits 2:0 */
    cs_uint32 cos_sel              :  2 ; /* bits 4:3 */
    cs_uint32 cos_rslt_valid       :  1 ; /* bits 5:5 */
    cs_uint32 learn_dis            :  1 ; /* bits 6:6 */
    cs_uint32 l2_sa_limit_fwd_permit :  1 ; /* bits 7:7 */
    cs_uint32 l2_rslt_valid        :  1 ; /* bits 8:8 */
    cs_uint32 top_802_1p           :  3 ; /* bits 11:9 */
    cs_uint32 top_802_1p_sel       :  3 ; /* bits 14:12 */
    cs_uint32 inner_802_1p         :  3 ; /* bits 17:15 */
    cs_uint32 inner_802_1p_sel     :  3 ; /* bits 20:18 */
    cs_uint32 dot1p_rslt_valid     :  1 ; /* bits 21:21 */
    cs_uint32 top_vlanid           : 10 ; /* bits 31:22 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_FIB_MEM_DATA0_t;





#define FE_LE_CE_SPCL_KEY_MEM_ACCESS             0x06144
#define FE_LE_CE_SPCL_KEY_MEM_ACCESS_dft         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 27 ;
    cs_uint32 address              :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 address              :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               : 27 ;
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_SPCL_KEY_MEM_ACCESS_t;





#define FE_LE_CE_SPCL_KEY_MEM_DATA1              0x06145
#define FE_LE_CE_SPCL_KEY_MEM_DATA1_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 spcl_key_ms_vld      :  1 ; /* bits 2:2 */
    cs_uint32 spcl_hdr_type        :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 spcl_hdr_type        :  2 ; /* bits 1:0 */
    cs_uint32 spcl_key_ms_vld      :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_SPCL_KEY_MEM_DATA1_t;





#define FE_LE_CE_SPCL_KEY_MEM_DATA0              0x06146
#define FE_LE_CE_SPCL_KEY_MEM_DATA0_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 spcl_msk             : 16 ; /* bits 31:16 */
    cs_uint32 spcl_key             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 spcl_key             : 16 ; /* bits 15:0 */
    cs_uint32 spcl_msk             : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_SPCL_KEY_MEM_DATA0_t;





#define FE_LE_CE_L4_PORT_KEY_MEM_ACCESS          0x06147
#define FE_LE_CE_L4_PORT_KEY_MEM_ACCESS_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 25 ;
    cs_uint32 address              :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 address              :  5 ; /* bits 4:0 */
    cs_uint32 rsrvd1               : 25 ;
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_L4_PORT_KEY_MEM_ACCESS_t;





#define FE_LE_CE_L4_PORT_KEY_MEM_DATA            0x06148
#define FE_LE_CE_L4_PORT_KEY_MEM_DATA_dft        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 l4_port_high         : 16 ; /* bits 31:16 */
    cs_uint32 l4_port_low          : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 l4_port_low          : 16 ; /* bits 15:0 */
    cs_uint32 l4_port_high         : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_L4_PORT_KEY_MEM_DATA_t;





#define FE_LE_CE_MAC_FLTR_AGNG_MEM_ACCESS        0x06149
#define FE_LE_CE_MAC_FLTR_AGNG_MEM_ACCESS_dft    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 address              :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 address              :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_MAC_FLTR_AGNG_MEM_ACCESS_t;





#define FE_LE_CE_MAC_FLTR_AGNG_MEM_DATA          0x0614a
#define FE_LE_CE_MAC_FLTR_AGNG_MEM_DATA_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  7 ;
    cs_uint32 aging_vld            :  1 ; /* bits 24:24 */
    cs_uint32 max_value            : 24 ; /* bits 23:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 max_value            : 24 ; /* bits 23:0 */
    cs_uint32 aging_vld            :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd1               :  7 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_CE_MAC_FLTR_AGNG_MEM_DATA_t;





#define FE_LE_PR_DPID_PRMS_CTRL                  0x0614b
#define FE_LE_PR_DPID_PRMS_CTRL_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 dpid_redirect1_en    :  1 ; /* bits 13:13 */
    cs_uint32 redirect1_fwd_dpid   :  3 ; /* bits 12:10 */
    cs_uint32 redirect1_compare_dpid :  3 ; /* bits 9:7 */
    cs_uint32 dpid_redirect0_en    :  1 ; /* bits 6:6 */
    cs_uint32 redirect0_fwd_dpid   :  3 ; /* bits 5:3 */
    cs_uint32 redirect0_compare_dpid :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 redirect0_compare_dpid :  3 ; /* bits 2:0 */
    cs_uint32 redirect0_fwd_dpid   :  3 ; /* bits 5:3 */
    cs_uint32 dpid_redirect0_en    :  1 ; /* bits 6:6 */
    cs_uint32 redirect1_compare_dpid :  3 ; /* bits 9:7 */
    cs_uint32 redirect1_fwd_dpid   :  3 ; /* bits 12:10 */
    cs_uint32 dpid_redirect1_en    :  1 ; /* bits 13:13 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_DPID_PRMS_CTRL_t;





#define FE_LE_PR_VLAN_PRMS_CTRL                  0x0614c
#define FE_LE_PR_VLAN_PRMS_CTRL_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 24 ;
    cs_uint32 dbg_ignore_inner_vlan_cmd_update_dis :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               :  7 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  7 ;
    cs_uint32 dbg_ignore_inner_vlan_cmd_update_dis :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd2               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_VLAN_PRMS_CTRL_t;





#define FE_LE_PR_DEBUG_STS                       0x0614d
#define FE_LE_PR_DEBUG_STS_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 vlan_rsltn_st        : 10 ; /* bits 31:22 */
    cs_uint32 dscp_rsltn_st        :  3 ; /* bits 21:19 */
    cs_uint32 cos_rsltn_st         :  5 ; /* bits 18:14 */
    cs_uint32 Eight021p_rsltn_st   :  9 ; /* bits 13:5 */
    cs_uint32 dpid_rsltn_st        :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dpid_rsltn_st        :  5 ; /* bits 4:0 */
    cs_uint32 Eight021p_rsltn_st   :  9 ; /* bits 13:5 */
    cs_uint32 cos_rsltn_st         :  5 ; /* bits 18:14 */
    cs_uint32 dscp_rsltn_st        :  3 ; /* bits 21:19 */
    cs_uint32 vlan_rsltn_st        : 10 ; /* bits 31:22 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_DEBUG_STS_t;





#define FE_LE_PR_RX_8021P_TX_8021P_MAP           0x0614e
#define FE_LE_PR_RX_8021P_TX_8021P_MAP_dft       0x00fac688




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 Eight021p_map7       :  3 ; /* bits 23:21 */
    cs_uint32 Eight021p_map6       :  3 ; /* bits 20:18 */
    cs_uint32 Eight021p_map5       :  3 ; /* bits 17:15 */
    cs_uint32 Eight021p_map4       :  3 ; /* bits 14:12 */
    cs_uint32 Eight021p_map3       :  3 ; /* bits 11:9 */
    cs_uint32 Eight021p_map2       :  3 ; /* bits 8:6 */
    cs_uint32 Eight021p_map1       :  3 ; /* bits 5:3 */
    cs_uint32 Eight021p_map0       :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 Eight021p_map0       :  3 ; /* bits 2:0 */
    cs_uint32 Eight021p_map1       :  3 ; /* bits 5:3 */
    cs_uint32 Eight021p_map2       :  3 ; /* bits 8:6 */
    cs_uint32 Eight021p_map3       :  3 ; /* bits 11:9 */
    cs_uint32 Eight021p_map4       :  3 ; /* bits 14:12 */
    cs_uint32 Eight021p_map5       :  3 ; /* bits 17:15 */
    cs_uint32 Eight021p_map6       :  3 ; /* bits 20:18 */
    cs_uint32 Eight021p_map7       :  3 ; /* bits 23:21 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_RX_8021P_TX_8021P_MAP_t;





#define FE_LE_PR_RX_8021P_TX_COS_MAP             0x0614f
#define FE_LE_PR_RX_8021P_TX_COS_MAP_dft         0x00fac688




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 cos_map7             :  3 ; /* bits 23:21 */
    cs_uint32 cos_map6             :  3 ; /* bits 20:18 */
    cs_uint32 cos_map5             :  3 ; /* bits 17:15 */
    cs_uint32 cos_map4             :  3 ; /* bits 14:12 */
    cs_uint32 cos_map3             :  3 ; /* bits 11:9 */
    cs_uint32 cos_map2             :  3 ; /* bits 8:6 */
    cs_uint32 cos_map1             :  3 ; /* bits 5:3 */
    cs_uint32 cos_map0             :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cos_map0             :  3 ; /* bits 2:0 */
    cs_uint32 cos_map1             :  3 ; /* bits 5:3 */
    cs_uint32 cos_map2             :  3 ; /* bits 8:6 */
    cs_uint32 cos_map3             :  3 ; /* bits 11:9 */
    cs_uint32 cos_map4             :  3 ; /* bits 14:12 */
    cs_uint32 cos_map5             :  3 ; /* bits 17:15 */
    cs_uint32 cos_map6             :  3 ; /* bits 20:18 */
    cs_uint32 cos_map7             :  3 ; /* bits 23:21 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_RX_8021P_TX_COS_MAP_t;





#define FE_LE_PR_RX_8021P_FLOW_ID_MAP_0          0x06150
#define FE_LE_PR_RX_8021P_FLOW_ID_MAP_0_dft      0x0a418820




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 flow_id_map5         :  5 ; /* bits 29:25 */
    cs_uint32 flow_id_map4         :  5 ; /* bits 24:20 */
    cs_uint32 flow_id_map3         :  5 ; /* bits 19:15 */
    cs_uint32 flow_id_map2         :  5 ; /* bits 14:10 */
    cs_uint32 flow_id_map1         :  5 ; /* bits 9:5 */
    cs_uint32 flow_id_map0         :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 flow_id_map0         :  5 ; /* bits 4:0 */
    cs_uint32 flow_id_map1         :  5 ; /* bits 9:5 */
    cs_uint32 flow_id_map2         :  5 ; /* bits 14:10 */
    cs_uint32 flow_id_map3         :  5 ; /* bits 19:15 */
    cs_uint32 flow_id_map4         :  5 ; /* bits 24:20 */
    cs_uint32 flow_id_map5         :  5 ; /* bits 29:25 */
    cs_uint32 rsrvd1               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_RX_8021P_FLOW_ID_MAP_0_t;





#define FE_LE_PR_RX_8021P_FLOW_ID_MAP_1          0x06151
#define FE_LE_PR_RX_8021P_FLOW_ID_MAP_1_dft      0x000000e6




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 flow_id_map7         :  5 ; /* bits 9:5 */
    cs_uint32 flow_id_map6         :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 flow_id_map6         :  5 ; /* bits 4:0 */
    cs_uint32 flow_id_map7         :  5 ; /* bits 9:5 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_RX_8021P_FLOW_ID_MAP_1_t;





#define FE_LE_PR_COS_FLOW_ID_MAP_0               0x06152
#define FE_LE_PR_COS_FLOW_ID_MAP_0_dft           0x0a418820




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 flow_id_map5         :  5 ; /* bits 29:25 */
    cs_uint32 flow_id_map4         :  5 ; /* bits 24:20 */
    cs_uint32 flow_id_map3         :  5 ; /* bits 19:15 */
    cs_uint32 flow_id_map2         :  5 ; /* bits 14:10 */
    cs_uint32 flow_id_map1         :  5 ; /* bits 9:5 */
    cs_uint32 flow_id_map0         :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 flow_id_map0         :  5 ; /* bits 4:0 */
    cs_uint32 flow_id_map1         :  5 ; /* bits 9:5 */
    cs_uint32 flow_id_map2         :  5 ; /* bits 14:10 */
    cs_uint32 flow_id_map3         :  5 ; /* bits 19:15 */
    cs_uint32 flow_id_map4         :  5 ; /* bits 24:20 */
    cs_uint32 flow_id_map5         :  5 ; /* bits 29:25 */
    cs_uint32 rsrvd1               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_COS_FLOW_ID_MAP_0_t;





#define FE_LE_PR_COS_FLOW_ID_MAP_1               0x06153
#define FE_LE_PR_COS_FLOW_ID_MAP_1_dft           0x000000e6




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 flow_id_map7         :  5 ; /* bits 9:5 */
    cs_uint32 flow_id_map6         :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 flow_id_map6         :  5 ; /* bits 4:0 */
    cs_uint32 flow_id_map7         :  5 ; /* bits 9:5 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_COS_FLOW_ID_MAP_1_t;





#define FE_LE_PR_RX_8021P_TX_DSCP_MAP_0          0x06154
#define FE_LE_PR_RX_8021P_TX_DSCP_MAP_0_dft      0x040c2040




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 dscp_map4            :  6 ; /* bits 29:24 */
    cs_uint32 dscp_map3            :  6 ; /* bits 23:18 */
    cs_uint32 dscp_map2            :  6 ; /* bits 17:12 */
    cs_uint32 dscp_map1            :  6 ; /* bits 11:6 */
    cs_uint32 dscp_map0            :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dscp_map0            :  6 ; /* bits 5:0 */
    cs_uint32 dscp_map1            :  6 ; /* bits 11:6 */
    cs_uint32 dscp_map2            :  6 ; /* bits 17:12 */
    cs_uint32 dscp_map3            :  6 ; /* bits 23:18 */
    cs_uint32 dscp_map4            :  6 ; /* bits 29:24 */
    cs_uint32 rsrvd1               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_RX_8021P_TX_DSCP_MAP_0_t;





#define FE_LE_PR_RX_8021P_TX_DSCP_MAP_1          0x06155
#define FE_LE_PR_RX_8021P_TX_DSCP_MAP_1_dft      0x00007185




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 dscp_map7            :  6 ; /* bits 17:12 */
    cs_uint32 dscp_map6            :  6 ; /* bits 11:6 */
    cs_uint32 dscp_map5            :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dscp_map5            :  6 ; /* bits 5:0 */
    cs_uint32 dscp_map6            :  6 ; /* bits 11:6 */
    cs_uint32 dscp_map7            :  6 ; /* bits 17:12 */
    cs_uint32 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_RX_8021P_TX_DSCP_MAP_1_t;





#define FE_LE_PR_RX_8021P_TX_TC_MAP_0            0x06156
#define FE_LE_PR_RX_8021P_TX_TC_MAP_0_dft        0x040c2040




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 tc_map4              :  6 ; /* bits 29:24 */
    cs_uint32 tc_map3              :  6 ; /* bits 23:18 */
    cs_uint32 tc_map2              :  6 ; /* bits 17:12 */
    cs_uint32 tc_map1              :  6 ; /* bits 11:6 */
    cs_uint32 tc_map0              :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tc_map0              :  6 ; /* bits 5:0 */
    cs_uint32 tc_map1              :  6 ; /* bits 11:6 */
    cs_uint32 tc_map2              :  6 ; /* bits 17:12 */
    cs_uint32 tc_map3              :  6 ; /* bits 23:18 */
    cs_uint32 tc_map4              :  6 ; /* bits 29:24 */
    cs_uint32 rsrvd1               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_RX_8021P_TX_TC_MAP_0_t;





#define FE_LE_PR_RX_8021P_TX_TC_MAP_1            0x06157
#define FE_LE_PR_RX_8021P_TX_TC_MAP_1_dft        0x00007185




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 tc_map7              :  6 ; /* bits 17:12 */
    cs_uint32 tc_map6              :  6 ; /* bits 11:6 */
    cs_uint32 tc_map5              :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tc_map5              :  6 ; /* bits 5:0 */
    cs_uint32 tc_map6              :  6 ; /* bits 11:6 */
    cs_uint32 tc_map7              :  6 ; /* bits 17:12 */
    cs_uint32 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_RX_8021P_TX_TC_MAP_1_t;





#define FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_ACCESS    0x06158
#define FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_ACCESS_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 address              :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 address              :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_ACCESS_t;





#define FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_DATA1     0x06159
#define FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_DATA1_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 rx_tc_flow_id_map    :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_tc_flow_id_map    :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_DATA1_t;





#define FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_DATA0     0x0615a
#define FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_DATA0_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rx_tc_flow_id_map    :  3 ; /* bits 31:29 */
    cs_uint32 rx_tc_tx_cos_map     :  3 ; /* bits 28:26 */
    cs_uint32 rx_tc_tx_802_1p_map  :  3 ; /* bits 25:23 */
    cs_uint32 rx_tc_tx_tc_map      :  6 ; /* bits 22:17 */
    cs_uint32 rx_dscp_flow_id_map  :  5 ; /* bits 16:12 */
    cs_uint32 rx_dscp_tx_cos_map   :  3 ; /* bits 11:9 */
    cs_uint32 rx_dscp_tx_802_1p_map :  3 ; /* bits 8:6 */
    cs_uint32 rx_dscp_tx_dscp_map  :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_dscp_tx_dscp_map  :  6 ; /* bits 5:0 */
    cs_uint32 rx_dscp_tx_802_1p_map :  3 ; /* bits 8:6 */
    cs_uint32 rx_dscp_tx_cos_map   :  3 ; /* bits 11:9 */
    cs_uint32 rx_dscp_flow_id_map  :  5 ; /* bits 16:12 */
    cs_uint32 rx_tc_tx_tc_map      :  6 ; /* bits 22:17 */
    cs_uint32 rx_tc_tx_802_1p_map  :  3 ; /* bits 25:23 */
    cs_uint32 rx_tc_tx_cos_map     :  3 ; /* bits 28:26 */
    cs_uint32 rx_tc_flow_id_map    :  3 ; /* bits 31:29 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_DATA0_t;





#define FE_LE_PE_CTRL                            0x0615b
#define FE_LE_PE_CTRL_dft                        0x00f81000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               :  8 ;
    cs_uint32 pe_mii1_en           :  1 ; /* bits 23:23 */
    cs_uint32 pe_mii0_en           :  1 ; /* bits 22:22 */
    cs_uint32 pe_pon_en            :  1 ; /* bits 21:21 */
    cs_uint32 pe_ge_en             :  1 ; /* bits 20:20 */
    cs_uint32 mask_rxmac_drop      :  1 ; /* bits 19:19 */
    cs_uint32 rsrvd2               :  6 ;
    cs_uint32 ingress_pkt_len      :  1 ; /* bits 12:12 */
    cs_uint32 dbg_pe_drop_undrsz_pkt_disable :  1 ; /* bits 11:11 */
    cs_uint32 inner_default_8021p  :  3 ; /* bits 10:8 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 top_default_8021p    :  3 ; /* bits 6:4 */
    cs_uint32 txpad_en             :  1 ; /* bits 3:3 */
    cs_uint32 stomp_crc            :  1 ; /* bits 2:2 */
    cs_uint32 keep_fcs             :  1 ; /* bits 1:1 */
    cs_uint32 pe_cpu_en            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pe_cpu_en            :  1 ; /* bits 0:0 */
    cs_uint32 keep_fcs             :  1 ; /* bits 1:1 */
    cs_uint32 stomp_crc            :  1 ; /* bits 2:2 */
    cs_uint32 txpad_en             :  1 ; /* bits 3:3 */
    cs_uint32 top_default_8021p    :  3 ; /* bits 6:4 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 inner_default_8021p  :  3 ; /* bits 10:8 */
    cs_uint32 dbg_pe_drop_undrsz_pkt_disable :  1 ; /* bits 11:11 */
    cs_uint32 ingress_pkt_len      :  1 ; /* bits 12:12 */
    cs_uint32 rsrvd2               :  6 ;
    cs_uint32 mask_rxmac_drop      :  1 ; /* bits 19:19 */
    cs_uint32 pe_ge_en             :  1 ; /* bits 20:20 */
    cs_uint32 pe_pon_en            :  1 ; /* bits 21:21 */
    cs_uint32 pe_mii0_en           :  1 ; /* bits 22:22 */
    cs_uint32 pe_mii1_en           :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd3               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PE_CTRL_t;





#define FE_LE_PE_VLAN_CMD_ERR_CNT                0x0615c
#define FE_LE_PE_VLAN_CMD_ERR_CNT_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 cnt                  : 24 ; /* bits 23:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 24 ; /* bits 23:0 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PE_VLAN_CMD_ERR_CNT_t;





#define FE_LE_PE_PR_INTERFACE_CAPTURE_0          0x0615d
#define FE_LE_PE_PR_INTERFACE_CAPTURE_0_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PE_PR_INTERFACE_CAPTURE_0_t;





#define FE_LE_PE_PR_INTERFACE_CAPTURE_1          0x0615e
#define FE_LE_PE_PR_INTERFACE_CAPTURE_1_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PE_PR_INTERFACE_CAPTURE_1_t;





#define FE_LE_PE_PR_INTERFACE_CAPTURE_2          0x0615f
#define FE_LE_PE_PR_INTERFACE_CAPTURE_2_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PE_PR_INTERFACE_CAPTURE_2_t;





#define FE_LE_PE_PR_INTERFACE_CAPTURE_3          0x06160
#define FE_LE_PE_PR_INTERFACE_CAPTURE_3_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PE_PR_INTERFACE_CAPTURE_3_t;





#define FE_LE_PE_PR_INTERFACE_CAPTURE_4          0x06161
#define FE_LE_PE_PR_INTERFACE_CAPTURE_4_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PE_PR_INTERFACE_CAPTURE_4_t;





#define FE_LE_PE_PR_INTERFACE_CAPTURE_5          0x06162
#define FE_LE_PE_PR_INTERFACE_CAPTURE_5_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PE_PR_INTERFACE_CAPTURE_5_t;





#define FE_LE_PE_PR_INTERFACE_CAPTURE_6          0x06163
#define FE_LE_PE_PR_INTERFACE_CAPTURE_6_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PE_PR_INTERFACE_CAPTURE_6_t;





#define FE_LE_PE_PR_INTERFACE_CAPTURE_7          0x06164
#define FE_LE_PE_PR_INTERFACE_CAPTURE_7_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PE_PR_INTERFACE_CAPTURE_7_t;





#define FE_LE_PE_BM_INTERFACE_CAPTURE_8          0x06165
#define FE_LE_PE_BM_INTERFACE_CAPTURE_8_dft      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PE_BM_INTERFACE_CAPTURE_8_t;





#define FE_LE_PE_CAPTURE                         0x06166
#define FE_LE_PE_CAPTURE_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 26 ;
    cs_uint32 dpid_st              :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dpid_st              :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 26 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} FE_LE_PE_CAPTURE_t;



    
  
  
  
#define FE_LE_STRIDE                             256
#define FE_LE_COUNT                              3
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* le */

typedef struct {
  FE_LE_ENGINE_CTRL_t                      ENGINE_CTRL;                   /* addr: 0x00000 */ 
  FE_LE_ENGINE_TPID_CFG_0_t                ENGINE_TPID_CFG_0;             /* addr: 0x00001 */ 
  FE_LE_ENGINE_TPID_CFG_1_t                ENGINE_TPID_CFG_1;             /* addr: 0x00002 */ 
  FE_LE_ENGINE_INGRESS_PKT_CNT_t           ENGINE_INGRESS_PKT_CNT;        /* addr: 0x00003 */ 
  FE_LE_ENGINE_EGRESS_PKT_CNT_t            ENGINE_EGRESS_PKT_CNT;         /* addr: 0x00004 */ 
  FE_LE_PP_PARSER_CTRL_t                   PP_PARSER_CTRL;                /* addr: 0x00005 */ 
  FE_LE_PP_SPCL_EV2PT_CFG_0_t              PP_SPCL_EV2PT_CFG_0;           /* addr: 0x00006 */ 
  FE_LE_PP_SPCL_EV2PT_CFG_1_t              PP_SPCL_EV2PT_CFG_1;           /* addr: 0x00007 */ 
  FE_LE_PP_SPCL_EV2PT_CFG_2_t              PP_SPCL_EV2PT_CFG_2;           /* addr: 0x00008 */ 
  FE_LE_PP_SPCL_PKT_MAC_0_ADDR1_t          PP_SPCL_PKT_MAC_0_ADDR1;       /* addr: 0x00009 */ 
  FE_LE_PP_SPCL_PKT_MAC_0_ADDR0_t          PP_SPCL_PKT_MAC_0_ADDR0;       /* addr: 0x0000a */ 
  FE_LE_PP_SPCL_PKT_MAC_1_ADDR1_t          PP_SPCL_PKT_MAC_1_ADDR1;       /* addr: 0x0000b */ 
  FE_LE_PP_SPCL_PKT_MAC_1_ADDR0_t          PP_SPCL_PKT_MAC_1_ADDR0;       /* addr: 0x0000c */ 
  FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR1_t      PP_SPCL_HELLO_PKT_MAC_ADDR1;   /* addr: 0x0000d */ 
  FE_LE_PP_SPCL_HELLO_PKT_MAC_ADDR0_t      PP_SPCL_HELLO_PKT_MAC_ADDR0;   /* addr: 0x0000e */ 
  FE_LE_PP_MAC_DA_RANGE_HIGH1_t            PP_MAC_DA_RANGE_HIGH1;         /* addr: 0x0000f */ 
  FE_LE_PP_MAC_DA_RANGE_HIGH0_t            PP_MAC_DA_RANGE_HIGH0;         /* addr: 0x00010 */ 
  FE_LE_PP_MAC_DA_RANGE_LOW1_t             PP_MAC_DA_RANGE_LOW1;          /* addr: 0x00011 */ 
  FE_LE_PP_MAC_DA_RANGE_LOW0_t             PP_MAC_DA_RANGE_LOW0;          /* addr: 0x00012 */ 
  FE_LE_PP_NI_INTERFACE_CAPTURE_0_t        PP_NI_INTERFACE_CAPTURE_0;     /* addr: 0x00013 */ 
  FE_LE_PP_NI_INTERFACE_CAPTURE_1_t        PP_NI_INTERFACE_CAPTURE_1;     /* addr: 0x00014 */ 
  FE_LE_PP_PR_INTERFACE_CAPTURE_2_t        PP_PR_INTERFACE_CAPTURE_2;     /* addr: 0x00015 */ 
  FE_LE_PP_PR_INTERFACE_CAPTURE_3_t        PP_PR_INTERFACE_CAPTURE_3;     /* addr: 0x00016 */ 
  FE_LE_VE_EGRESS_VLAN_TBL_VLD_CFG_t       VE_EGRESS_VLAN_TBL_VLD_CFG;    /* addr: 0x00017 */ 
  FE_LE_VE_INGRESS_VLAN_TBL_VLD_CFG_t      VE_INGRESS_VLAN_TBL_VLD_CFG;   /* addr: 0x00018 */ 
  FE_LE_VE_MAP_VLAN_TBL_CFG_t              VE_MAP_VLAN_TBL_CFG;           /* addr: 0x00019 */ 
  FE_LE_VE_VLAN_TABLE_MEM_ACCESS_t         VE_VLAN_TABLE_MEM_ACCESS;      /* addr: 0x0001a */ 
  FE_LE_VE_VLAN_TABLE_MEM_DATA3_t          VE_VLAN_TABLE_MEM_DATA3;       /* addr: 0x0001b */ 
  FE_LE_VE_VLAN_TABLE_MEM_DATA2_t          VE_VLAN_TABLE_MEM_DATA2;       /* addr: 0x0001c */ 
  FE_LE_VE_VLAN_TABLE_MEM_DATA1_t          VE_VLAN_TABLE_MEM_DATA1;       /* addr: 0x0001d */ 
  FE_LE_VE_VLAN_TABLE_MEM_DATA0_t          VE_VLAN_TABLE_MEM_DATA0;       /* addr: 0x0001e */ 
  FE_LE_CE_CTRL_t                          CE_CTRL;                       /* addr: 0x0001f */ 
  FE_LE_CE_KEY_VLD1_t                      CE_KEY_VLD1;                   /* addr: 0x00020 */ 
  FE_LE_CE_KEY_VLD0_t                      CE_KEY_VLD0;                   /* addr: 0x00021 */ 
  FE_LE_CE_RULE_MODE_CTRL1_t               CE_RULE_MODE_CTRL1;            /* addr: 0x00022 */ 
  FE_LE_CE_RULE_MODE_CTRL0_t               CE_RULE_MODE_CTRL0;            /* addr: 0x00023 */ 
  FE_LE_CE_MAC_FLTR_AGNG_TMR_CFG_t         CE_MAC_FLTR_AGNG_TMR_CFG;      /* addr: 0x00024 */ 
  FE_LE_CE_L4_CTRL_t                       CE_L4_CTRL;                    /* addr: 0x00025 */ 
  FE_LE_CE_MAC_FILTER_AGING_STS_t          CE_MAC_FILTER_AGING_STS;       /* addr: 0x00026 */ 
  FE_LE_CE_MAC_FLTR_AGNG_VLD_STS1_t        CE_MAC_FLTR_AGNG_VLD_STS1;     /* addr: 0x00027 */ 
  FE_LE_CE_MAC_FLTR_AGNG_VLD_STS0_t        CE_MAC_FLTR_AGNG_VLD_STS0;     /* addr: 0x00028 */ 
  FE_LE_CE_DEBUG_STS_t                     CE_DEBUG_STS;                  /* addr: 0x00029 */ 
  FE_LE_CE_DEBUG_ENT_HIT_CNT_ADR_CFG_t     CE_DEBUG_ENT_HIT_CNT_ADR_CFG;  /* addr: 0x0002a */ 
  FE_LE_CE_DEBUG_ENT_HIT_CNT_t             CE_DEBUG_ENT_HIT_CNT;          /* addr: 0x0002b */ 
  FE_LE_CE_DEBUG_PKT_STS_0_t               CE_DEBUG_PKT_STS_0;            /* addr: 0x0002c */ 
  FE_LE_CE_DEBUG_PKT_STS_1_t               CE_DEBUG_PKT_STS_1;            /* addr: 0x0002d */ 
  FE_LE_CE_DEBUG_PKT_STS_2_t               CE_DEBUG_PKT_STS_2;            /* addr: 0x0002e */ 
  FE_LE_CE_DEBUG_ENT_3_0_HIT_CNT_t         CE_DEBUG_ENT_3_0_HIT_CNT;      /* addr: 0x0002f */ 
  FE_LE_CE_DEBUG_ENT_7_4_HIT_CNT_t         CE_DEBUG_ENT_7_4_HIT_CNT;      /* addr: 0x00030 */ 
  FE_LE_CE_DEBUG_ENT_11_8_HIT_CNT_t        CE_DEBUG_ENT_11_8_HIT_CNT;     /* addr: 0x00031 */ 
  FE_LE_CE_DEBUG_ENT_15_12_HIT_CNT_t       CE_DEBUG_ENT_15_12_HIT_CNT;    /* addr: 0x00032 */ 
  FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_ACCESS_t CE_KEY_NXT_PTR_LINK_LIST_TBL_ACCESS; /* addr: 0x00033 */ 
  FE_LE_CE_KEY_NXT_PTR_LINK_LIST_TBL_DATA_t CE_KEY_NXT_PTR_LINK_LIST_TBL_DATA; /* addr: 0x00034 */ 
  FE_LE_CE_KEY_MEM_ACCESS_t                CE_KEY_MEM_ACCESS;             /* addr: 0x00035 */ 
  FE_LE_CE_KEY_MEM_DATA9_t                 CE_KEY_MEM_DATA9;              /* addr: 0x00036 */ 
  FE_LE_CE_KEY_MEM_DATA8_t                 CE_KEY_MEM_DATA8;              /* addr: 0x00037 */ 
  FE_LE_CE_KEY_MEM_DATA7_t                 CE_KEY_MEM_DATA7;              /* addr: 0x00038 */ 
  FE_LE_CE_KEY_MEM_DATA6_t                 CE_KEY_MEM_DATA6;              /* addr: 0x00039 */ 
  FE_LE_CE_KEY_MEM_DATA5_t                 CE_KEY_MEM_DATA5;              /* addr: 0x0003a */ 
  FE_LE_CE_KEY_MEM_DATA4_t                 CE_KEY_MEM_DATA4;              /* addr: 0x0003b */ 
  FE_LE_CE_KEY_MEM_DATA3_t                 CE_KEY_MEM_DATA3;              /* addr: 0x0003c */ 
  FE_LE_CE_KEY_MEM_DATA2_t                 CE_KEY_MEM_DATA2;              /* addr: 0x0003d */ 
  FE_LE_CE_KEY_MEM_DATA1_t                 CE_KEY_MEM_DATA1;              /* addr: 0x0003e */ 
  FE_LE_CE_KEY_MEM_DATA0_t                 CE_KEY_MEM_DATA0;              /* addr: 0x0003f */ 
  FE_LE_CE_FIB_MEM_ACCESS_t                CE_FIB_MEM_ACCESS;             /* addr: 0x00040 */ 
  FE_LE_CE_FIB_MEM_DATA2_t                 CE_FIB_MEM_DATA2;              /* addr: 0x00041 */ 
  FE_LE_CE_FIB_MEM_DATA1_t                 CE_FIB_MEM_DATA1;              /* addr: 0x00042 */ 
  FE_LE_CE_FIB_MEM_DATA0_t                 CE_FIB_MEM_DATA0;              /* addr: 0x00043 */ 
  FE_LE_CE_SPCL_KEY_MEM_ACCESS_t           CE_SPCL_KEY_MEM_ACCESS;        /* addr: 0x00044 */ 
  FE_LE_CE_SPCL_KEY_MEM_DATA1_t            CE_SPCL_KEY_MEM_DATA1;         /* addr: 0x00045 */ 
  FE_LE_CE_SPCL_KEY_MEM_DATA0_t            CE_SPCL_KEY_MEM_DATA0;         /* addr: 0x00046 */ 
  FE_LE_CE_L4_PORT_KEY_MEM_ACCESS_t        CE_L4_PORT_KEY_MEM_ACCESS;     /* addr: 0x00047 */ 
  FE_LE_CE_L4_PORT_KEY_MEM_DATA_t          CE_L4_PORT_KEY_MEM_DATA;       /* addr: 0x00048 */ 
  FE_LE_CE_MAC_FLTR_AGNG_MEM_ACCESS_t      CE_MAC_FLTR_AGNG_MEM_ACCESS;   /* addr: 0x00049 */ 
  FE_LE_CE_MAC_FLTR_AGNG_MEM_DATA_t        CE_MAC_FLTR_AGNG_MEM_DATA;     /* addr: 0x0004a */ 
  FE_LE_PR_DPID_PRMS_CTRL_t                PR_DPID_PRMS_CTRL;             /* addr: 0x0004b */ 
  FE_LE_PR_VLAN_PRMS_CTRL_t                PR_VLAN_PRMS_CTRL;             /* addr: 0x0004c */ 
  FE_LE_PR_DEBUG_STS_t                     PR_DEBUG_STS;                  /* addr: 0x0004d */ 
  FE_LE_PR_RX_8021P_TX_8021P_MAP_t         PR_RX_8021P_TX_8021P_MAP;      /* addr: 0x0004e */ 
  FE_LE_PR_RX_8021P_TX_COS_MAP_t           PR_RX_8021P_TX_COS_MAP;        /* addr: 0x0004f */ 
  FE_LE_PR_RX_8021P_FLOW_ID_MAP_0_t        PR_RX_8021P_FLOW_ID_MAP_0;     /* addr: 0x00050 */ 
  FE_LE_PR_RX_8021P_FLOW_ID_MAP_1_t        PR_RX_8021P_FLOW_ID_MAP_1;     /* addr: 0x00051 */ 
  FE_LE_PR_COS_FLOW_ID_MAP_0_t             PR_COS_FLOW_ID_MAP_0;          /* addr: 0x00052 */ 
  FE_LE_PR_COS_FLOW_ID_MAP_1_t             PR_COS_FLOW_ID_MAP_1;          /* addr: 0x00053 */ 
  FE_LE_PR_RX_8021P_TX_DSCP_MAP_0_t        PR_RX_8021P_TX_DSCP_MAP_0;     /* addr: 0x00054 */ 
  FE_LE_PR_RX_8021P_TX_DSCP_MAP_1_t        PR_RX_8021P_TX_DSCP_MAP_1;     /* addr: 0x00055 */ 
  FE_LE_PR_RX_8021P_TX_TC_MAP_0_t          PR_RX_8021P_TX_TC_MAP_0;       /* addr: 0x00056 */ 
  FE_LE_PR_RX_8021P_TX_TC_MAP_1_t          PR_RX_8021P_TX_TC_MAP_1;       /* addr: 0x00057 */ 
  FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_ACCESS_t  PR_DSCP_TC_MAP_TABLE_MEM_ACCESS; /* addr: 0x00058 */ 
  FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_DATA1_t   PR_DSCP_TC_MAP_TABLE_MEM_DATA1; /* addr: 0x00059 */ 
  FE_LE_PR_DSCP_TC_MAP_TABLE_MEM_DATA0_t   PR_DSCP_TC_MAP_TABLE_MEM_DATA0; /* addr: 0x0005a */ 
  FE_LE_PE_CTRL_t                          PE_CTRL;                       /* addr: 0x0005b */ 
  FE_LE_PE_VLAN_CMD_ERR_CNT_t              PE_VLAN_CMD_ERR_CNT;           /* addr: 0x0005c */ 
  FE_LE_PE_PR_INTERFACE_CAPTURE_0_t        PE_PR_INTERFACE_CAPTURE_0;     /* addr: 0x0005d */ 
  FE_LE_PE_PR_INTERFACE_CAPTURE_1_t        PE_PR_INTERFACE_CAPTURE_1;     /* addr: 0x0005e */ 
  FE_LE_PE_PR_INTERFACE_CAPTURE_2_t        PE_PR_INTERFACE_CAPTURE_2;     /* addr: 0x0005f */ 
  FE_LE_PE_PR_INTERFACE_CAPTURE_3_t        PE_PR_INTERFACE_CAPTURE_3;     /* addr: 0x00060 */ 
  FE_LE_PE_PR_INTERFACE_CAPTURE_4_t        PE_PR_INTERFACE_CAPTURE_4;     /* addr: 0x00061 */ 
  FE_LE_PE_PR_INTERFACE_CAPTURE_5_t        PE_PR_INTERFACE_CAPTURE_5;     /* addr: 0x00062 */ 
  FE_LE_PE_PR_INTERFACE_CAPTURE_6_t        PE_PR_INTERFACE_CAPTURE_6;     /* addr: 0x00063 */ 
  FE_LE_PE_PR_INTERFACE_CAPTURE_7_t        PE_PR_INTERFACE_CAPTURE_7;     /* addr: 0x00064 */ 
  FE_LE_PE_BM_INTERFACE_CAPTURE_8_t        PE_BM_INTERFACE_CAPTURE_8;     /* addr: 0x00065 */ 
  FE_LE_PE_CAPTURE_t                       PE_CAPTURE;                    /* addr: 0x00066 */ 
} FE_LE_t;

  

  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* fe */

typedef struct {
  FE_TOP_t                                 top;                           /* addr: 0x00000 */ 
  cs_uint32                                rsrvd1[201];                  
  FE_LE_t                                  le;                            /* addr: 0x00100 */ 
} FE_t;


/*
 * ge
 */
  



#define GE_ETH_MAC_CONFIG0                       0x07000
#define GE_ETH_MAC_CONFIG0_dft                   0x00100800




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 tx_update_crc        :  1 ; /* bits 29:29 */
    cs_uint32 pass_thru_hdre       :  1 ; /* bits 28:28 */
    cs_uint32 phy_intr_inv         :  1 ; /* bits 27:27 */
    cs_uint32 tx_auto_xon          :  1 ; /* bits 26:26 */
    cs_uint32 txfifo_thrshld       :  9 ; /* bits 25:17 */
    cs_uint32 txfifo_mode          :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 tx_drain             :  1 ; /* bits 13:13 */
    cs_uint32 tx_flow_disable      :  1 ; /* bits 12:12 */
    cs_uint32 tx_ipg_sel           :  3 ; /* bits 11:9 */
    cs_uint32 rx_flow_disable      :  1 ; /* bits 8:8 */
    cs_uint32 link_active_mask     :  1 ; /* bits 7:7 */
    cs_uint32 link_stat_mask       :  1 ; /* bits 6:6 */
    cs_uint32 link_status          :  1 ; /* bits 5:5 */
    cs_uint32 crs_polarity         :  1 ; /* bits 4:4 */
    cs_uint32 tx_en                :  1 ; /* bits 3:3 */
    cs_uint32 rx_en                :  1 ; /* bits 2:2 */
    cs_uint32 duplex               :  1 ; /* bits 1:1 */
    cs_uint32 speed                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 speed                :  1 ; /* bits 0:0 */
    cs_uint32 duplex               :  1 ; /* bits 1:1 */
    cs_uint32 rx_en                :  1 ; /* bits 2:2 */
    cs_uint32 tx_en                :  1 ; /* bits 3:3 */
    cs_uint32 crs_polarity         :  1 ; /* bits 4:4 */
    cs_uint32 link_status          :  1 ; /* bits 5:5 */
    cs_uint32 link_stat_mask       :  1 ; /* bits 6:6 */
    cs_uint32 link_active_mask     :  1 ; /* bits 7:7 */
    cs_uint32 rx_flow_disable      :  1 ; /* bits 8:8 */
    cs_uint32 tx_ipg_sel           :  3 ; /* bits 11:9 */
    cs_uint32 tx_flow_disable      :  1 ; /* bits 12:12 */
    cs_uint32 tx_drain             :  1 ; /* bits 13:13 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 txfifo_mode          :  1 ; /* bits 16:16 */
    cs_uint32 txfifo_thrshld       :  9 ; /* bits 25:17 */
    cs_uint32 tx_auto_xon          :  1 ; /* bits 26:26 */
    cs_uint32 phy_intr_inv         :  1 ; /* bits 27:27 */
    cs_uint32 pass_thru_hdre       :  1 ; /* bits 28:28 */
    cs_uint32 tx_update_crc        :  1 ; /* bits 29:29 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_ETH_MAC_CONFIG0_t;





#define GE_ETH_MAC_CONFIG1                       0x07001
#define GE_ETH_MAC_CONFIG1_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 mib_op_mode          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mib_op_mode          :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_ETH_MAC_CONFIG1_t;





#define GE_PKT_LEN_CONFIG                        0x07002
#define GE_PKT_LEN_CONFIG_dft                    0x004005ee




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  9 ;
    cs_uint32 min_size             :  7 ; /* bits 22:16 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 max_size             : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 max_size             : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 min_size             :  7 ; /* bits 22:16 */
    cs_uint32 rsrvd2               :  9 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_PKT_LEN_CONFIG_t;





#define GE_INTF_RST_CONFIG                       0x07003
#define GE_INTF_RST_CONFIG_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 27 ;
    cs_uint32 phy_mode             :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 rgmii_speed          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rgmii_speed          :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 phy_mode             :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd2               : 27 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_INTF_RST_CONFIG_t;





#define GE_RX_MAC_CONFIG0                        0x07004
#define GE_RX_MAC_CONFIG0_dft                    0x81000003




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tag_type0            : 16 ; /* bits 31:16 */
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 tag_exclude          :  1 ; /* bits 1:1 */
    cs_uint32 rx_pause_uc          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_pause_uc          :  1 ; /* bits 0:0 */
    cs_uint32 tag_exclude          :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 tag_type0            : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_RX_MAC_CONFIG0_t;





#define GE_RX_MAC_CONFIG1                        0x07005
#define GE_RX_MAC_CONFIG1_dft                    0x88a89100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tag_type2            : 16 ; /* bits 31:16 */
    cs_uint32 tag_type1            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tag_type1            : 16 ; /* bits 15:0 */
    cs_uint32 tag_type2            : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_RX_MAC_CONFIG1_t;





#define GE_RX_CNTRL_CONFIG0                      0x07006
#define GE_RX_CNTRL_CONFIG0_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 15 ;
    cs_uint32 bypass_cos           :  3 ; /* bits 16:14 */
    cs_uint32 bypass_dpid          :  3 ; /* bits 13:11 */
    cs_uint32 bypass_enable        :  1 ; /* bits 10:10 */
    cs_uint32 oam_drop_dis         :  1 ; /* bits 9:9 */
    cs_uint32 ukopcode_drop_dis    :  1 ; /* bits 8:8 */
    cs_uint32 flow_drop_dis        :  1 ; /* bits 7:7 */
    cs_uint32 udr_drop_dis         :  1 ; /* bits 6:6 */
    cs_uint32 ovr_drop_dis         :  1 ; /* bits 5:5 */
    cs_uint32 crc_drop_dis         :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 ff_overrun_drop_dis  :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 rxctrl_dis           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rxctrl_dis           :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 ff_overrun_drop_dis  :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 crc_drop_dis         :  1 ; /* bits 4:4 */
    cs_uint32 ovr_drop_dis         :  1 ; /* bits 5:5 */
    cs_uint32 udr_drop_dis         :  1 ; /* bits 6:6 */
    cs_uint32 flow_drop_dis        :  1 ; /* bits 7:7 */
    cs_uint32 ukopcode_drop_dis    :  1 ; /* bits 8:8 */
    cs_uint32 oam_drop_dis         :  1 ; /* bits 9:9 */
    cs_uint32 bypass_enable        :  1 ; /* bits 10:10 */
    cs_uint32 bypass_dpid          :  3 ; /* bits 13:11 */
    cs_uint32 bypass_cos           :  3 ; /* bits 16:14 */
    cs_uint32 rsrvd3               : 15 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_RX_CNTRL_CONFIG0_t;





#define GE_TX_CNTRL_CONFIG0                      0x07007
#define GE_TX_CNTRL_CONFIG0_dft                  0x010000f8




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  5 ;
    cs_uint32 rdy_high_thld        : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 rdy_low_thld         : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rdy_low_thld         : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 rdy_high_thld        : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd2               :  5 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_TX_CNTRL_CONFIG0_t;





#define GE_ETH_RCV_STAT                          0x07008
#define GE_ETH_RCV_STAT_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 rcv_lnk_stat         :  1 ; /* bits 3:3 */
    cs_uint32 rcv_duplex           :  1 ; /* bits 2:2 */
    cs_uint32 rcv_speed            :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rcv_speed            :  2 ; /* bits 1:0 */
    cs_uint32 rcv_duplex           :  1 ; /* bits 2:2 */
    cs_uint32 rcv_lnk_stat         :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_ETH_RCV_STAT_t;





#define GE_INTERRUPT                             0x07009
#define GE_INTERRUPT_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 phy_intr             :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd3               : 13 ;
    cs_uint32 ptp_eg_ltsI          :  1 ; /* bits 17:17 */
    cs_uint32 ptp_ig_ltsI          :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd2               : 10 ;
    cs_uint32 mib_msb_int_setI     :  1 ; /* bits 5:5 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 rm_ff_overrunI       :  1 ; /* bits 3:3 */
    cs_uint32 txfifo_overrunI      :  1 ; /* bits 2:2 */
    cs_uint32 txfifo_underrunI     :  1 ; /* bits 1:1 */
    cs_uint32 link_stat_chgI       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 link_stat_chgI       :  1 ; /* bits 0:0 */
    cs_uint32 txfifo_underrunI     :  1 ; /* bits 1:1 */
    cs_uint32 txfifo_overrunI      :  1 ; /* bits 2:2 */
    cs_uint32 rm_ff_overrunI       :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 mib_msb_int_setI     :  1 ; /* bits 5:5 */
    cs_uint32 rsrvd2               : 10 ;
    cs_uint32 ptp_ig_ltsI          :  1 ; /* bits 16:16 */
    cs_uint32 ptp_eg_ltsI          :  1 ; /* bits 17:17 */
    cs_uint32 rsrvd3               : 13 ;
    cs_uint32 phy_intr             :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_INTERRUPT_t;





#define GE_INTENABLE                             0x0700a
#define GE_INTENABLE_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 phy_intrE            :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd3               : 13 ;
    cs_uint32 ptp_eg_ltsIE         :  1 ; /* bits 17:17 */
    cs_uint32 ptp_ig_ltsIE         :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd2               : 10 ;
    cs_uint32 mib_msb_int_setIE    :  1 ; /* bits 5:5 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 rm_ff_overrunIE      :  1 ; /* bits 3:3 */
    cs_uint32 txfifo_overrunIE     :  1 ; /* bits 2:2 */
    cs_uint32 txfifo_underrunIE    :  1 ; /* bits 1:1 */
    cs_uint32 link_stat_chgIE      :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 link_stat_chgIE      :  1 ; /* bits 0:0 */
    cs_uint32 txfifo_underrunIE    :  1 ; /* bits 1:1 */
    cs_uint32 txfifo_overrunIE     :  1 ; /* bits 2:2 */
    cs_uint32 rm_ff_overrunIE      :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 mib_msb_int_setIE    :  1 ; /* bits 5:5 */
    cs_uint32 rsrvd2               : 10 ;
    cs_uint32 ptp_ig_ltsIE         :  1 ; /* bits 16:16 */
    cs_uint32 ptp_eg_ltsIE         :  1 ; /* bits 17:17 */
    cs_uint32 rsrvd3               : 13 ;
    cs_uint32 phy_intrE            :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_INTENABLE_t;





#define GE_DEBUG_1                               0x0700b
#define GE_DEBUG_1_dft                           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 21 ;
    cs_uint32 hd_backoff_dbg       :  1 ; /* bits 10:10 */
    cs_uint32 hd_backoff_cnt       : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 hd_backoff_cnt       : 10 ; /* bits 9:0 */
    cs_uint32 hd_backoff_dbg       :  1 ; /* bits 10:10 */
    cs_uint32 rsrvd1               : 21 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_DEBUG_1_t;





#define GE_INIT                                  0x0700c
#define GE_INIT_dft                              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 mib                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mib                  :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_INIT_t;





#define GE_MIB_ACCESS                            0x0700d
#define GE_MIB_ACCESS_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 op_code              :  2 ; /* bits 29:28 */
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 counter              :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 counter              :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 op_code              :  2 ; /* bits 29:28 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_MIB_ACCESS_t;

#define     counter_TxSingleColFrm                        0x27
#define     counter_TxMultiColFrm                         0x28
#define     counter_TxLateColFrm                          0x29
#define     counter_TxExessColFrm                         0x2a




#define GE_MIB_DATA1                             0x0700e
#define GE_MIB_DATA1_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt_val_hi           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt_val_hi           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_MIB_DATA1_t;





#define GE_MIB_DATA0                             0x0700f
#define GE_MIB_DATA0_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt_val_lo           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt_val_lo           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_MIB_DATA0_t;





#define GE_PTP_CONFIG                            0x07010
#define GE_PTP_CONFIG_dft                        0x0060000f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  9 ;
    cs_uint32 tx_bypass            :  1 ; /* bits 22:22 */
    cs_uint32 rx_bypass            :  1 ; /* bits 21:21 */
    cs_uint32 eg_lts_bswap         :  1 ; /* bits 20:20 */
    cs_uint32 ig_lts_bswap         :  1 ; /* bits 19:19 */
    cs_uint32 packet_cos           :  3 ; /* bits 18:16 */
    cs_uint32 process_en           : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 process_en           : 16 ; /* bits 15:0 */
    cs_uint32 packet_cos           :  3 ; /* bits 18:16 */
    cs_uint32 ig_lts_bswap         :  1 ; /* bits 19:19 */
    cs_uint32 eg_lts_bswap         :  1 ; /* bits 20:20 */
    cs_uint32 rx_bypass            :  1 ; /* bits 21:21 */
    cs_uint32 tx_bypass            :  1 ; /* bits 22:22 */
    cs_uint32 rsrvd1               :  9 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_PTP_CONFIG_t;





#define GE_PTP_ASYM_DELAY                        0x07011
#define GE_PTP_ASYM_DELAY_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 sign                 :  1 ; /* bits 15:15 */
    cs_uint32 correction           : 15 ; /* bits 14:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 correction           : 15 ; /* bits 14:0 */
    cs_uint32 sign                 :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_PTP_ASYM_DELAY_t;





#define GE_PTP_ING_LATCH                         0x07012
#define GE_PTP_ING_LATCH_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 local_time           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 local_time           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_PTP_ING_LATCH_t;





#define GE_PTP_PROCESS                           0x07013
#define GE_PTP_PROCESS_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 cf_offset            :  6 ; /* bits 9:4 */
    cs_uint32 message_type         :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 message_type         :  4 ; /* bits 3:0 */
    cs_uint32 cf_offset            :  6 ; /* bits 9:4 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_PTP_PROCESS_t;





#define GE_PTP_EG_LATCH                          0x07014
#define GE_PTP_EG_LATCH_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 local_time           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 local_time           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_PTP_EG_LATCH_t;





#define GE_PTP_ING_FWD                           0x07015
#define GE_PTP_ING_FWD_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 local_time           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 local_time           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_PTP_ING_FWD_t;





#define GE_PTP_EG_STS                            0x07016
#define GE_PTP_EG_STS_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 lt_valid             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 lt_valid             :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_PTP_EG_STS_t;





#define GE_EEE_CONFIG                            0x07017
#define GE_EEE_CONFIG_dft                        0x00000020




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 24 ;
    cs_uint32 tck_extra            :  4 ; /* bits 7:4 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 tck_off              :  1 ; /* bits 1:1 */
    cs_uint32 enable               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 enable               :  1 ; /* bits 0:0 */
    cs_uint32 tck_off              :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 tck_extra            :  4 ; /* bits 7:4 */
    cs_uint32 rsrvd2               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} GE_EEE_CONFIG_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* ge */

typedef struct {
  GE_ETH_MAC_CONFIG0_t                     ETH_MAC_CONFIG0;               /* addr: 0x00000 */ 
  GE_ETH_MAC_CONFIG1_t                     ETH_MAC_CONFIG1;               /* addr: 0x00001 */ 
  GE_PKT_LEN_CONFIG_t                      PKT_LEN_CONFIG;                /* addr: 0x00002 */ 
  GE_INTF_RST_CONFIG_t                     INTF_RST_CONFIG;               /* addr: 0x00003 */ 
  GE_RX_MAC_CONFIG0_t                      RX_MAC_CONFIG0;                /* addr: 0x00004 */ 
  GE_RX_MAC_CONFIG1_t                      RX_MAC_CONFIG1;                /* addr: 0x00005 */ 
  GE_RX_CNTRL_CONFIG0_t                    RX_CNTRL_CONFIG0;              /* addr: 0x00006 */ 
  GE_TX_CNTRL_CONFIG0_t                    TX_CNTRL_CONFIG0;              /* addr: 0x00007 */ 
  GE_ETH_RCV_STAT_t                        ETH_RCV_STAT;                  /* addr: 0x00008 */ 
  GE_INTERRUPT_t                           INTERRUPT;                     /* addr: 0x00009 */ 
  GE_INTENABLE_t                           INTENABLE;                     /* addr: 0x0000a */ 
  GE_DEBUG_1_t                             DEBUG_1;                       /* addr: 0x0000b */ 
  GE_INIT_t                                INIT;                          /* addr: 0x0000c */ 
  GE_MIB_ACCESS_t                          MIB_ACCESS;                    /* addr: 0x0000d */ 
  GE_MIB_DATA1_t                           MIB_DATA1;                     /* addr: 0x0000e */ 
  GE_MIB_DATA0_t                           MIB_DATA0;                     /* addr: 0x0000f */ 
  GE_PTP_CONFIG_t                          PTP_CONFIG;                    /* addr: 0x00010 */ 
  GE_PTP_ASYM_DELAY_t                      PTP_ASYM_DELAY;                /* addr: 0x00011 */ 
  GE_PTP_ING_LATCH_t                       PTP_ING_LATCH;                 /* addr: 0x00012 */ 
  GE_PTP_PROCESS_t                         PTP_PROCESS;                   /* addr: 0x00013 */ 
  GE_PTP_EG_LATCH_t                        PTP_EG_LATCH;                  /* addr: 0x00014 */ 
  GE_PTP_ING_FWD_t                         PTP_ING_FWD;                   /* addr: 0x00015 */ 
  GE_PTP_EG_STS_t                          PTP_EG_STS;                    /* addr: 0x00016 */ 
  GE_EEE_CONFIG_t                          EEE_CONFIG;                    /* addr: 0x00017 */ 
} GE_t;

  

/*
 * epon
 */
  



#define ONU_MAC_INTERRUPT_SOURCE                 0x08000
#define ONU_MAC_INTERRUPT_SOURCE_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 PPS_CAPi             :  1 ; /* bits 31:31 */
    cs_uint32 SIG_LOSSi            :  1 ; /* bits 30:30 */
    cs_uint32 DISCOVERY_FRMi       :  1 ; /* bits 29:29 */
    cs_uint32 enc_en_trigi         :  1 ; /* bits 28:28 */
    cs_uint32 GRANT_DROP_TMi       :  1 ; /* bits 27:27 */
    cs_uint32 GRANT_DROP_TSi       :  1 ; /* bits 26:26 */
    cs_uint32 GRANT_DROP_TFi       :  1 ; /* bits 25:25 */
    cs_uint32 RTT_RXi              :  1 ; /* bits 24:24 */
    cs_uint32 MPCP_TIMER_EXPIRATIONi :  1 ; /* bits 23:23 */
    cs_uint32 GATE_TIMER_EXPIRATIONi :  1 ; /* bits 22:22 */
    cs_uint32 TIMESTAMP_DRIFTi     :  1 ; /* bits 21:21 */
    cs_uint32 MPCP_GRANT_FIFO_ACCESS_ERRi :  1 ; /* bits 20:20 */
    cs_uint32 MPCP_GRANT_FIFO_EXCEEDINGi :  1 ; /* bits 19:19 */
    cs_uint32 RX_REGISTER_FRAMEi   :  1 ; /* bits 18:18 */
    cs_uint32 REGISTRATION_STATUS_CHANGEi :  1 ; /* bits 17:17 */
    cs_uint32 REPORT_TIMER_EXPIRATIONi :  1 ; /* bits 16:16 */
    cs_uint32 BM_PON_TX_MONi       :  1 ; /* bits 15:15 */
    cs_uint32 ptp_ig_ltsI          :  1 ; /* bits 14:14 */
    cs_uint32 TX_PACKET_UFLOWi     :  1 ; /* bits 13:13 */
    cs_uint32 ptp_eg_ltsI          :  1 ; /* bits 12:12 */
    cs_uint32 TX_FAILi             :  1 ; /* bits 11:11 */
    cs_uint32 TX_OVERRUNi          :  1 ; /* bits 10:10 */
    cs_uint32 TX_PKT_CUTi          :  1 ; /* bits 9:9 */
    cs_uint32 TX_NON_EMPTYi        :  1 ; /* bits 8:8 */
    cs_uint32 MIBi                 :  1 ; /* bits 7:7 */
    cs_uint32 PCS_RX_SYNCi         :  1 ; /* bits 6:6 */
    cs_uint32 pps_timeout          :  1 ; /* bits 5:5 */
    cs_uint32 CWB_FULLi            :  1 ; /* bits 4:4 */
    cs_uint32 CMD_FULLi            :  1 ; /* bits 3:3 */
    cs_uint32 PRTY_FULLi           :  1 ; /* bits 2:2 */
    cs_uint32 PACK_FULLi           :  1 ; /* bits 1:1 */
    cs_uint32 CNTR_HALF_FULLi      :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 CNTR_HALF_FULLi      :  1 ; /* bits 0:0 */
    cs_uint32 PACK_FULLi           :  1 ; /* bits 1:1 */
    cs_uint32 PRTY_FULLi           :  1 ; /* bits 2:2 */
    cs_uint32 CMD_FULLi            :  1 ; /* bits 3:3 */
    cs_uint32 CWB_FULLi            :  1 ; /* bits 4:4 */
    cs_uint32 pps_timeout          :  1 ; /* bits 5:5 */
    cs_uint32 PCS_RX_SYNCi         :  1 ; /* bits 6:6 */
    cs_uint32 MIBi                 :  1 ; /* bits 7:7 */
    cs_uint32 TX_NON_EMPTYi        :  1 ; /* bits 8:8 */
    cs_uint32 TX_PKT_CUTi          :  1 ; /* bits 9:9 */
    cs_uint32 TX_OVERRUNi          :  1 ; /* bits 10:10 */
    cs_uint32 TX_FAILi             :  1 ; /* bits 11:11 */
    cs_uint32 ptp_eg_ltsI          :  1 ; /* bits 12:12 */
    cs_uint32 TX_PACKET_UFLOWi     :  1 ; /* bits 13:13 */
    cs_uint32 ptp_ig_ltsI          :  1 ; /* bits 14:14 */
    cs_uint32 BM_PON_TX_MONi       :  1 ; /* bits 15:15 */
    cs_uint32 REPORT_TIMER_EXPIRATIONi :  1 ; /* bits 16:16 */
    cs_uint32 REGISTRATION_STATUS_CHANGEi :  1 ; /* bits 17:17 */
    cs_uint32 RX_REGISTER_FRAMEi   :  1 ; /* bits 18:18 */
    cs_uint32 MPCP_GRANT_FIFO_EXCEEDINGi :  1 ; /* bits 19:19 */
    cs_uint32 MPCP_GRANT_FIFO_ACCESS_ERRi :  1 ; /* bits 20:20 */
    cs_uint32 TIMESTAMP_DRIFTi     :  1 ; /* bits 21:21 */
    cs_uint32 GATE_TIMER_EXPIRATIONi :  1 ; /* bits 22:22 */
    cs_uint32 MPCP_TIMER_EXPIRATIONi :  1 ; /* bits 23:23 */
    cs_uint32 RTT_RXi              :  1 ; /* bits 24:24 */
    cs_uint32 GRANT_DROP_TFi       :  1 ; /* bits 25:25 */
    cs_uint32 GRANT_DROP_TSi       :  1 ; /* bits 26:26 */
    cs_uint32 GRANT_DROP_TMi       :  1 ; /* bits 27:27 */
    cs_uint32 enc_en_trigi         :  1 ; /* bits 28:28 */
    cs_uint32 DISCOVERY_FRMi       :  1 ; /* bits 29:29 */
    cs_uint32 SIG_LOSSi            :  1 ; /* bits 30:30 */
    cs_uint32 PPS_CAPi             :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_INTERRUPT_SOURCE_t;





#define ONU_MAC_INTERRUPT_ENABLE                 0x08001
#define ONU_MAC_INTERRUPT_ENABLE_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 PPS_CAPe             :  1 ; /* bits 31:31 */
    cs_uint32 SIG_LOSSe            :  1 ; /* bits 30:30 */
    cs_uint32 DISCOVERY_FRMe       :  1 ; /* bits 29:29 */
    cs_uint32 enc_en_trige         :  1 ; /* bits 28:28 */
    cs_uint32 grant_drop_tme       :  1 ; /* bits 27:27 */
    cs_uint32 grant_drop_tse       :  1 ; /* bits 26:26 */
    cs_uint32 grant_drop_tfe       :  1 ; /* bits 25:25 */
    cs_uint32 rtt_rxe              :  1 ; /* bits 24:24 */
    cs_uint32 mpcp_timer_expiratione :  1 ; /* bits 23:23 */
    cs_uint32 gate_timer_expiratione :  1 ; /* bits 22:22 */
    cs_uint32 timestamp_drifte     :  1 ; /* bits 21:21 */
    cs_uint32 MPCP_grant_FIFO_Access_Erre :  1 ; /* bits 20:20 */
    cs_uint32 MPCP_grant_FIFO_Exceedinge :  1 ; /* bits 19:19 */
    cs_uint32 rx_register_framee   :  1 ; /* bits 18:18 */
    cs_uint32 registration_status_changee :  1 ; /* bits 17:17 */
    cs_uint32 report_timer_expiratione :  1 ; /* bits 16:16 */
    cs_uint32 BM_PON_TX_MONe       :  1 ; /* bits 15:15 */
    cs_uint32 ptp_ig_ltsIE         :  1 ; /* bits 14:14 */
    cs_uint32 TX_PACKET_UFLOWe     :  1 ; /* bits 13:13 */
    cs_uint32 ptp_eg_ltsIE         :  1 ; /* bits 12:12 */
    cs_uint32 TX_FAILe             :  1 ; /* bits 11:11 */
    cs_uint32 TX_OVERRUNe          :  1 ; /* bits 10:10 */
    cs_uint32 TX_PKT_CUTe          :  1 ; /* bits 9:9 */
    cs_uint32 TX_NON_EMPTYe        :  1 ; /* bits 8:8 */
    cs_uint32 MIBe                 :  1 ; /* bits 7:7 */
    cs_uint32 PCS_RX_SYNCe         :  1 ; /* bits 6:6 */
    cs_uint32 pps_timeoute         :  1 ; /* bits 5:5 */
    cs_uint32 CWB_FULLe            :  1 ; /* bits 4:4 */
    cs_uint32 CMD_FULLe            :  1 ; /* bits 3:3 */
    cs_uint32 PRTY_FULLe           :  1 ; /* bits 2:2 */
    cs_uint32 PACK_FULLe           :  1 ; /* bits 1:1 */
    cs_uint32 CNTR_HALF_FULLe      :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 CNTR_HALF_FULLe      :  1 ; /* bits 0:0 */
    cs_uint32 PACK_FULLe           :  1 ; /* bits 1:1 */
    cs_uint32 PRTY_FULLe           :  1 ; /* bits 2:2 */
    cs_uint32 CMD_FULLe            :  1 ; /* bits 3:3 */
    cs_uint32 CWB_FULLe            :  1 ; /* bits 4:4 */
    cs_uint32 pps_timeoute         :  1 ; /* bits 5:5 */
    cs_uint32 PCS_RX_SYNCe         :  1 ; /* bits 6:6 */
    cs_uint32 MIBe                 :  1 ; /* bits 7:7 */
    cs_uint32 TX_NON_EMPTYe        :  1 ; /* bits 8:8 */
    cs_uint32 TX_PKT_CUTe          :  1 ; /* bits 9:9 */
    cs_uint32 TX_OVERRUNe          :  1 ; /* bits 10:10 */
    cs_uint32 TX_FAILe             :  1 ; /* bits 11:11 */
    cs_uint32 ptp_eg_ltsIE         :  1 ; /* bits 12:12 */
    cs_uint32 TX_PACKET_UFLOWe     :  1 ; /* bits 13:13 */
    cs_uint32 ptp_ig_ltsIE         :  1 ; /* bits 14:14 */
    cs_uint32 BM_PON_TX_MONe       :  1 ; /* bits 15:15 */
    cs_uint32 report_timer_expiratione :  1 ; /* bits 16:16 */
    cs_uint32 registration_status_changee :  1 ; /* bits 17:17 */
    cs_uint32 rx_register_framee   :  1 ; /* bits 18:18 */
    cs_uint32 MPCP_grant_FIFO_Exceedinge :  1 ; /* bits 19:19 */
    cs_uint32 MPCP_grant_FIFO_Access_Erre :  1 ; /* bits 20:20 */
    cs_uint32 timestamp_drifte     :  1 ; /* bits 21:21 */
    cs_uint32 gate_timer_expiratione :  1 ; /* bits 22:22 */
    cs_uint32 mpcp_timer_expiratione :  1 ; /* bits 23:23 */
    cs_uint32 rtt_rxe              :  1 ; /* bits 24:24 */
    cs_uint32 grant_drop_tfe       :  1 ; /* bits 25:25 */
    cs_uint32 grant_drop_tse       :  1 ; /* bits 26:26 */
    cs_uint32 grant_drop_tme       :  1 ; /* bits 27:27 */
    cs_uint32 enc_en_trige         :  1 ; /* bits 28:28 */
    cs_uint32 DISCOVERY_FRMe       :  1 ; /* bits 29:29 */
    cs_uint32 SIG_LOSSe            :  1 ; /* bits 30:30 */
    cs_uint32 PPS_CAPe             :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_INTERRUPT_ENABLE_t;





#define ONU_MAC_INTERRUPT_ISAS                   0x08002
#define ONU_MAC_INTERRUPT_ISAS_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 PPS_CAPiZ            :  1 ; /* bits 31:31 */
    cs_uint32 SIG_LOSSiZ           :  1 ; /* bits 30:30 */
    cs_uint32 DISCOVERY_FRMiZ      :  1 ; /* bits 29:29 */
    cs_uint32 enc_en_trigiZ        :  1 ; /* bits 28:28 */
    cs_uint32 GRANT_DROP_TMiZ      :  1 ; /* bits 27:27 */
    cs_uint32 GRANT_DROP_TSiZ      :  1 ; /* bits 26:26 */
    cs_uint32 GRANT_DROP_TFiZ      :  1 ; /* bits 25:25 */
    cs_uint32 RTT_RXiZ             :  1 ; /* bits 24:24 */
    cs_uint32 MPCP_TIMER_EXPIRATIONiZ :  1 ; /* bits 23:23 */
    cs_uint32 GATE_TIMER_EXPIRATIONiZ :  1 ; /* bits 22:22 */
    cs_uint32 TIMESTAMP_DRIFTiZ    :  1 ; /* bits 21:21 */
    cs_uint32 MPCP_GRANT_FIFO_ACCESS_ERRiZ :  1 ; /* bits 20:20 */
    cs_uint32 MPCP_GRANT_FIFO_EXCEEDINGiZ :  1 ; /* bits 19:19 */
    cs_uint32 RX_REGISTER_FRAMEiZ  :  1 ; /* bits 18:18 */
    cs_uint32 REGISTRATION_STATUS_CHANGEiZ :  1 ; /* bits 17:17 */
    cs_uint32 REPORT_TIMER_EXPIRATIONiZ :  1 ; /* bits 16:16 */
    cs_uint32 BM_PON_TX_MONiZ      :  1 ; /* bits 15:15 */
    cs_uint32 ptp_ig_ltsIZ         :  1 ; /* bits 14:14 */
    cs_uint32 TX_PACKET_UFLOWiZ    :  1 ; /* bits 13:13 */
    cs_uint32 ptp_eg_ltsIZ         :  1 ; /* bits 12:12 */
    cs_uint32 TX_FAILiZ            :  1 ; /* bits 11:11 */
    cs_uint32 TX_OVERRUNiZ         :  1 ; /* bits 10:10 */
    cs_uint32 TX_PKT_CUTiZ         :  1 ; /* bits 9:9 */
    cs_uint32 TX_NON_EMPTYiZ       :  1 ; /* bits 8:8 */
    cs_uint32 MIBiZ                :  1 ; /* bits 7:7 */
    cs_uint32 PCS_RX_SYNCiZ        :  1 ; /* bits 6:6 */
    cs_uint32 pps_timeoutZ         :  1 ; /* bits 5:5 */
    cs_uint32 CWB_FULLiZ           :  1 ; /* bits 4:4 */
    cs_uint32 CMD_FULLiZ           :  1 ; /* bits 3:3 */
    cs_uint32 PRTY_FULLiZ          :  1 ; /* bits 2:2 */
    cs_uint32 PACK_FULLiZ          :  1 ; /* bits 1:1 */
    cs_uint32 CNTR_HALF_FULLiZ     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 CNTR_HALF_FULLiZ     :  1 ; /* bits 0:0 */
    cs_uint32 PACK_FULLiZ          :  1 ; /* bits 1:1 */
    cs_uint32 PRTY_FULLiZ          :  1 ; /* bits 2:2 */
    cs_uint32 CMD_FULLiZ           :  1 ; /* bits 3:3 */
    cs_uint32 CWB_FULLiZ           :  1 ; /* bits 4:4 */
    cs_uint32 pps_timeoutZ         :  1 ; /* bits 5:5 */
    cs_uint32 PCS_RX_SYNCiZ        :  1 ; /* bits 6:6 */
    cs_uint32 MIBiZ                :  1 ; /* bits 7:7 */
    cs_uint32 TX_NON_EMPTYiZ       :  1 ; /* bits 8:8 */
    cs_uint32 TX_PKT_CUTiZ         :  1 ; /* bits 9:9 */
    cs_uint32 TX_OVERRUNiZ         :  1 ; /* bits 10:10 */
    cs_uint32 TX_FAILiZ            :  1 ; /* bits 11:11 */
    cs_uint32 ptp_eg_ltsIZ         :  1 ; /* bits 12:12 */
    cs_uint32 TX_PACKET_UFLOWiZ    :  1 ; /* bits 13:13 */
    cs_uint32 ptp_ig_ltsIZ         :  1 ; /* bits 14:14 */
    cs_uint32 BM_PON_TX_MONiZ      :  1 ; /* bits 15:15 */
    cs_uint32 REPORT_TIMER_EXPIRATIONiZ :  1 ; /* bits 16:16 */
    cs_uint32 REGISTRATION_STATUS_CHANGEiZ :  1 ; /* bits 17:17 */
    cs_uint32 RX_REGISTER_FRAMEiZ  :  1 ; /* bits 18:18 */
    cs_uint32 MPCP_GRANT_FIFO_EXCEEDINGiZ :  1 ; /* bits 19:19 */
    cs_uint32 MPCP_GRANT_FIFO_ACCESS_ERRiZ :  1 ; /* bits 20:20 */
    cs_uint32 TIMESTAMP_DRIFTiZ    :  1 ; /* bits 21:21 */
    cs_uint32 GATE_TIMER_EXPIRATIONiZ :  1 ; /* bits 22:22 */
    cs_uint32 MPCP_TIMER_EXPIRATIONiZ :  1 ; /* bits 23:23 */
    cs_uint32 RTT_RXiZ             :  1 ; /* bits 24:24 */
    cs_uint32 GRANT_DROP_TFiZ      :  1 ; /* bits 25:25 */
    cs_uint32 GRANT_DROP_TSiZ      :  1 ; /* bits 26:26 */
    cs_uint32 GRANT_DROP_TMiZ      :  1 ; /* bits 27:27 */
    cs_uint32 enc_en_trigiZ        :  1 ; /* bits 28:28 */
    cs_uint32 DISCOVERY_FRMiZ      :  1 ; /* bits 29:29 */
    cs_uint32 SIG_LOSSiZ           :  1 ; /* bits 30:30 */
    cs_uint32 PPS_CAPiZ            :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_INTERRUPT_ISAS_t;





#define ONU_MAC_OPTICAL_XCR_CTRL                 0x08003
#define ONU_MAC_OPTICAL_XCR_CTRL_dft             0x000003d5




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 21 ;
    cs_uint32 laser_transmitter_power_control_mode :  1 ; /* bits 10:10 */
    cs_uint32 tx_oen               :  1 ; /* bits 9:9 */
    cs_uint32 sig_detect_polarity  :  1 ; /* bits 8:8 */
    cs_uint32 transmit_fail_polarity :  1 ; /* bits 7:7 */
    cs_uint32 disable_laser_transmitter_power_polarity :  1 ; /* bits 6:6 */
    cs_uint32 disable_laser_transmitter_power :  1 ; /* bits 5:5 */
    cs_uint32 disable_laser_transmitter_polarity :  1 ; /* bits 4:4 */
    cs_uint32 disable_laser_transmitter :  1 ; /* bits 3:3 */
    cs_uint32 laser_burst_polarity :  1 ; /* bits 2:2 */
    cs_uint32 laser_control        :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 laser_control        :  2 ; /* bits 1:0 */
    cs_uint32 laser_burst_polarity :  1 ; /* bits 2:2 */
    cs_uint32 disable_laser_transmitter :  1 ; /* bits 3:3 */
    cs_uint32 disable_laser_transmitter_polarity :  1 ; /* bits 4:4 */
    cs_uint32 disable_laser_transmitter_power :  1 ; /* bits 5:5 */
    cs_uint32 disable_laser_transmitter_power_polarity :  1 ; /* bits 6:6 */
    cs_uint32 transmit_fail_polarity :  1 ; /* bits 7:7 */
    cs_uint32 sig_detect_polarity  :  1 ; /* bits 8:8 */
    cs_uint32 tx_oen               :  1 ; /* bits 9:9 */
    cs_uint32 laser_transmitter_power_control_mode :  1 ; /* bits 10:10 */
    cs_uint32 rsrvd1               : 21 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_OPTICAL_XCR_CTRL_t;





#define ONU_MAC_OPTICAL_XCR_STATUS               0x08004
#define ONU_MAC_OPTICAL_XCR_STATUS_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 tx_fail              :  1 ; /* bits 1:1 */
    cs_uint32 sig_det              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sig_det              :  1 ; /* bits 0:0 */
    cs_uint32 tx_fail              :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_OPTICAL_XCR_STATUS_t;





#define ONU_MAC_SDS_SIG_LOSS_CNT                 0x08005
#define ONU_MAC_SDS_SIG_LOSS_CNT_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 lsron_cnt            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 lsron_cnt            : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_SDS_SIG_LOSS_CNT_t;





#define ONU_MAC_RSSI                             0x08006
#define ONU_MAC_RSSI_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 27 ;
    cs_uint32 pulse_ctrl2          :  1 ; /* bits 4:4 */
    cs_uint32 pulse_ctrl1          :  1 ; /* bits 3:3 */
    cs_uint32 comdet_en            :  1 ; /* bits 2:2 */
    cs_uint32 pulse_on             :  1 ; /* bits 1:1 */
    cs_uint32 pulse_ctrl           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pulse_ctrl           :  1 ; /* bits 0:0 */
    cs_uint32 pulse_on             :  1 ; /* bits 1:1 */
    cs_uint32 comdet_en            :  1 ; /* bits 2:2 */
    cs_uint32 pulse_ctrl1          :  1 ; /* bits 3:3 */
    cs_uint32 pulse_ctrl2          :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               : 27 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RSSI_t;





#define ONU_MAC_ONU_LaserOn_LaserOff_Time        0x08007
#define ONU_MAC_ONU_LaserOn_LaserOff_Time_dft    0x00004040




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 laserOnTime          : 10 ; /* bits 17:8 */
    cs_uint32 laserOffTime         :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 laserOffTime         :  8 ; /* bits 7:0 */
    cs_uint32 laserOnTime          : 10 ; /* bits 17:8 */
    cs_uint32 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_ONU_LaserOn_LaserOff_Time_t;





#define ONU_MAC_SERDES_LATENCY                   0x08008
#define ONU_MAC_SERDES_LATENCY_dft               0x00000204




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 tx_latency           :  8 ; /* bits 15:8 */
    cs_uint32 rx_latency           :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_latency           :  8 ; /* bits 7:0 */
    cs_uint32 tx_latency           :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_SERDES_LATENCY_t;





#define ONU_MAC_GLOBAL_GLB_SYS_CFG               0x08009
#define ONU_MAC_GLOBAL_GLB_SYS_CFG_dft           0x00040c00




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pwr_dly              : 16 ; /* bits 31:16 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 pon_fec_sync_en      :  1 ; /* bits 11:11 */
    cs_uint32 pon_10balign_en      :  1 ; /* bits 10:10 */
    cs_uint32 rsrvd1               : 10 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 pon_10balign_en      :  1 ; /* bits 10:10 */
    cs_uint32 pon_fec_sync_en      :  1 ; /* bits 11:11 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 pwr_dly              : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_GLOBAL_GLB_SYS_CFG_t;





#define ONU_MAC_GLOBAL_GLB_PCS                   0x0800a
#define ONU_MAC_GLOBAL_GLB_PCS_dft               0x00085000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 12 ;
    cs_uint32 PCS_LOOP_CNT_HI      :  4 ; /* bits 19:16 */
    cs_uint32 PCS_LOOP_CNT_LO      :  4 ; /* bits 15:12 */
    cs_uint32 rsrvd1               : 12 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               : 12 ;
    cs_uint32 PCS_LOOP_CNT_LO      :  4 ; /* bits 15:12 */
    cs_uint32 PCS_LOOP_CNT_HI      :  4 ; /* bits 19:16 */
    cs_uint32 rsrvd2               : 12 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_GLOBAL_GLB_PCS_t;





#define ONU_MAC_AN_Ctrl_Reg                      0x0800b
#define ONU_MAC_AN_Ctrl_Reg_dft                  0x00000023




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 26 ;
    cs_uint32 DIS_AUTO_NEG         :  1 ; /* bits 5:5 */
    cs_uint32 RESTART_AUTO_NEG     :  1 ; /* bits 4:4 */
    cs_uint32 An_RF2               :  1 ; /* bits 3:3 */
    cs_uint32 An_RF1               :  1 ; /* bits 2:2 */
    cs_uint32 An_PS2               :  1 ; /* bits 1:1 */
    cs_uint32 An_ps1               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 An_ps1               :  1 ; /* bits 0:0 */
    cs_uint32 An_PS2               :  1 ; /* bits 1:1 */
    cs_uint32 An_RF1               :  1 ; /* bits 2:2 */
    cs_uint32 An_RF2               :  1 ; /* bits 3:3 */
    cs_uint32 RESTART_AUTO_NEG     :  1 ; /* bits 4:4 */
    cs_uint32 DIS_AUTO_NEG         :  1 ; /* bits 5:5 */
    cs_uint32 rsrvd1               : 26 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_AN_Ctrl_Reg_t;





#define ONU_MAC_PCS_Error_Symbol_Counter         0x0800c
#define ONU_MAC_PCS_Error_Symbol_Counter_dft     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 Pcs_sym_err_cnt      : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 Pcs_sym_err_cnt      : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PCS_Error_Symbol_Counter_t;





#define ONU_MAC_PCS_OutofSync_Cnt                0x0800d
#define ONU_MAC_PCS_OutofSync_Cnt_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 Pcs_out_sync_cnt     : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 Pcs_out_sync_cnt     : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PCS_OutofSync_Cnt_t;





#define ONU_MAC_PCS_Status_Reg                   0x0800e
#define ONU_MAC_PCS_Status_Reg_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 PCS_Rx_Sync          :  1 ; /* bits 7:7 */
    cs_uint32 An_done              :  1 ; /* bits 6:6 */
    cs_uint32 Link_partner_duplex  :  1 ; /* bits 5:5 */
    cs_uint32 An_RF2               :  1 ; /* bits 4:4 */
    cs_uint32 An_RF1               :  1 ; /* bits 3:3 */
    cs_uint32 An_PS2               :  1 ; /* bits 2:2 */
    cs_uint32 An_ps1               :  1 ; /* bits 1:1 */
    cs_uint32 Link_status          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 Link_status          :  1 ; /* bits 0:0 */
    cs_uint32 An_ps1               :  1 ; /* bits 1:1 */
    cs_uint32 An_PS2               :  1 ; /* bits 2:2 */
    cs_uint32 An_RF1               :  1 ; /* bits 3:3 */
    cs_uint32 An_RF2               :  1 ; /* bits 4:4 */
    cs_uint32 Link_partner_duplex  :  1 ; /* bits 5:5 */
    cs_uint32 An_done              :  1 ; /* bits 6:6 */
    cs_uint32 PCS_Rx_Sync          :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PCS_Status_Reg_t;





#define ONU_MAC_AES_MODE_CTL                     0x0800f
#define ONU_MAC_AES_MODE_CTL_dft                 0x00200000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 AesModeSel           :  2 ; /* bits 31:30 */
    cs_uint32 loc_cnt_debug        :  1 ; /* bits 29:29 */
    cs_uint32 tx_port_decryptonly_en :  1 ; /* bits 28:28 */
    cs_uint32 csr_port_encrypt_index :  1 ; /* bits 27:27 */
    cs_uint32 port_encrypt_index_sel :  1 ; /* bits 26:26 */
    cs_uint32 tx_crc8_gen_disable  :  1 ; /* bits 25:25 */
    cs_uint32 tx_ReverseCrc8Out    :  1 ; /* bits 24:24 */
    cs_uint32 tx_ReverseCrc8En     :  1 ; /* bits 23:23 */
    cs_uint32 rx_enc_en_cnt_lmt    :  3 ; /* bits 22:20 */
    cs_uint32 tx_port_encrypt_en   :  1 ; /* bits 19:19 */
    cs_uint32 rx_enc_en_cnt_clr    :  1 ; /* bits 18:18 */
    cs_uint32 tx_CntIncDec         :  1 ; /* bits 17:17 */
    cs_uint32 tx_CntDiff           :  8 ; /* bits 16:9 */
    cs_uint32 CntIncDec            :  1 ; /* bits 8:8 */
    cs_uint32 CntDiff              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 CntDiff              :  8 ; /* bits 7:0 */
    cs_uint32 CntIncDec            :  1 ; /* bits 8:8 */
    cs_uint32 tx_CntDiff           :  8 ; /* bits 16:9 */
    cs_uint32 tx_CntIncDec         :  1 ; /* bits 17:17 */
    cs_uint32 rx_enc_en_cnt_clr    :  1 ; /* bits 18:18 */
    cs_uint32 tx_port_encrypt_en   :  1 ; /* bits 19:19 */
    cs_uint32 rx_enc_en_cnt_lmt    :  3 ; /* bits 22:20 */
    cs_uint32 tx_ReverseCrc8En     :  1 ; /* bits 23:23 */
    cs_uint32 tx_ReverseCrc8Out    :  1 ; /* bits 24:24 */
    cs_uint32 tx_crc8_gen_disable  :  1 ; /* bits 25:25 */
    cs_uint32 port_encrypt_index_sel :  1 ; /* bits 26:26 */
    cs_uint32 csr_port_encrypt_index :  1 ; /* bits 27:27 */
    cs_uint32 tx_port_decryptonly_en :  1 ; /* bits 28:28 */
    cs_uint32 loc_cnt_debug        :  1 ; /* bits 29:29 */
    cs_uint32 AesModeSel           :  2 ; /* bits 31:30 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_AES_MODE_CTL_t;





#define ONU_MAC_UC_KEY03                         0x08010
#define ONU_MAC_UC_KEY03_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_KEY03_t;





#define ONU_MAC_UC_KEY02                         0x08011
#define ONU_MAC_UC_KEY02_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_KEY02_t;





#define ONU_MAC_UC_KEY01                         0x08012
#define ONU_MAC_UC_KEY01_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_KEY01_t;





#define ONU_MAC_UC_KEY00                         0x08013
#define ONU_MAC_UC_KEY00_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_KEY00_t;





#define ONU_MAC_UC_KEY13                         0x08014
#define ONU_MAC_UC_KEY13_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_KEY13_t;





#define ONU_MAC_UC_KEY12                         0x08015
#define ONU_MAC_UC_KEY12_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_KEY12_t;





#define ONU_MAC_UC_KEY11                         0x08016
#define ONU_MAC_UC_KEY11_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_KEY11_t;





#define ONU_MAC_UC_KEY10                         0x08017
#define ONU_MAC_UC_KEY10_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_KEY10_t;





#define ONU_MAC_PKT_MON03                        0x08018
#define ONU_MAC_PKT_MON03_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 recv_pkt_r           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 recv_pkt_r           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PKT_MON03_t;





#define ONU_MAC_PKT_MON02                        0x08019
#define ONU_MAC_PKT_MON02_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 recv_pkt_r           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 recv_pkt_r           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PKT_MON02_t;





#define ONU_MAC_PKT_MON01                        0x0801a
#define ONU_MAC_PKT_MON01_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 recv_pkt_r           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 recv_pkt_r           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PKT_MON01_t;





#define ONU_MAC_PKT_MON00                        0x0801b
#define ONU_MAC_PKT_MON00_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 recv_pkt_r           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 recv_pkt_r           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PKT_MON00_t;





#define ONU_MAC_PKT_MON13                        0x0801c
#define ONU_MAC_PKT_MON13_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 decrypt_pkt_r        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 decrypt_pkt_r        : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PKT_MON13_t;





#define ONU_MAC_PKT_MON12                        0x0801d
#define ONU_MAC_PKT_MON12_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 decrypt_pkt_r        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 decrypt_pkt_r        : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PKT_MON12_t;





#define ONU_MAC_PKT_MON11                        0x0801e
#define ONU_MAC_PKT_MON11_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 decrypt_pkt_r        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 decrypt_pkt_r        : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PKT_MON11_t;





#define ONU_MAC_PKT_MON10                        0x0801f
#define ONU_MAC_PKT_MON10_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 decrypt_pkt_r        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 decrypt_pkt_r        : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PKT_MON10_t;





#define ONU_MAC_PKT_MON2                         0x08020
#define ONU_MAC_PKT_MON2_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  8 ;
    cs_uint32 key_ind_tog_cnt      :  8 ; /* bits 23:16 */
    cs_uint32 rx_aes_flag          :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 RCC_b0               :  1 ; /* bits 1:1 */
    cs_uint32 last_rx_key_index    :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 last_rx_key_index    :  1 ; /* bits 0:0 */
    cs_uint32 RCC_b0               :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 rx_aes_flag          :  8 ; /* bits 15:8 */
    cs_uint32 key_ind_tog_cnt      :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd2               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PKT_MON2_t;





#define ONU_MAC_PKT_MON31                        0x08021
#define ONU_MAC_PKT_MON31_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mpcp_cnt_recode_err  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mpcp_cnt_recode_err  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PKT_MON31_t;





#define ONU_MAC_PKT_MON30                        0x08022
#define ONU_MAC_PKT_MON30_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mpcp_cnt_recode_err  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mpcp_cnt_recode_err  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PKT_MON30_t;





#define ONU_MAC_KEYREQUEST_OAM_CNT               0x08023
#define ONU_MAC_KEYREQUEST_OAM_CNT_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 keyrequest_oam_cnt   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 keyrequest_oam_cnt   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_KEYREQUEST_OAM_CNT_t;





#define ONU_MAC_UC_DEC_DBG_CNT                   0x08024
#define ONU_MAC_UC_DEC_DBG_CNT_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mpcp_cnt_dbg         : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mpcp_cnt_dbg         : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_DEC_DBG_CNT_t;





#define ONU_MAC_JAES_TIMER                       0x08025
#define ONU_MAC_JAES_TIMER_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 H32                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 H32                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_JAES_TIMER_t;





#define ONU_MAC_AES_DBG_CNT0                     0x08026
#define ONU_MAC_AES_DBG_CNT0_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_AES_DBG_CNT0_t;





#define ONU_MAC_AES_DBG_GOOD_CNT0                0x08027
#define ONU_MAC_AES_DBG_GOOD_CNT0_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_AES_DBG_GOOD_CNT0_t;





#define ONU_MAC_AES_DBG_CNT1                     0x08028
#define ONU_MAC_AES_DBG_CNT1_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_AES_DBG_CNT1_t;





#define ONU_MAC_AES_DBG_GOOD_CNT1                0x08029
#define ONU_MAC_AES_DBG_GOOD_CNT1_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_AES_DBG_GOOD_CNT1_t;





#define ONU_MAC_UC_TX_KEY03                      0x0802a
#define ONU_MAC_UC_TX_KEY03_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_TX_KEY03_t;





#define ONU_MAC_UC_TX_KEY02                      0x0802b
#define ONU_MAC_UC_TX_KEY02_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_TX_KEY02_t;





#define ONU_MAC_UC_TX_KEY01                      0x0802c
#define ONU_MAC_UC_TX_KEY01_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_TX_KEY01_t;





#define ONU_MAC_UC_TX_KEY00                      0x0802d
#define ONU_MAC_UC_TX_KEY00_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_TX_KEY00_t;





#define ONU_MAC_UC_TX_KEY13                      0x0802e
#define ONU_MAC_UC_TX_KEY13_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_TX_KEY13_t;





#define ONU_MAC_UC_TX_KEY12                      0x0802f
#define ONU_MAC_UC_TX_KEY12_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_TX_KEY12_t;





#define ONU_MAC_UC_TX_KEY11                      0x08030
#define ONU_MAC_UC_TX_KEY11_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_TX_KEY11_t;





#define ONU_MAC_UC_TX_KEY10                      0x08031
#define ONU_MAC_UC_TX_KEY10_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_TX_KEY10_t;





#define ONU_MAC_UC_TX_KEY_CTRL                   0x08032
#define ONU_MAC_UC_TX_KEY_CTRL_dft               0x00000002




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 tx_rcc_b0_sel        :  1 ; /* bits 1:1 */
    cs_uint32 tx_key_sel           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_key_sel           :  1 ; /* bits 0:0 */
    cs_uint32 tx_rcc_b0_sel        :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_UC_TX_KEY_CTRL_t;





#define ONU_MAC_FEC_CFG                          0x08033
#define ONU_MAC_FEC_CFG_dft                      0x0000c5f5




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 16 ;
    cs_uint32 tx_fec8B             :  1 ; /* bits 15:15 */
    cs_uint32 tx_ipg_adj           :  3 ; /* bits 14:12 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 rx_window_on         :  1 ; /* bits 10:10 */
    cs_uint32 in_valid_cg          :  1 ; /* bits 9:9 */
    cs_uint32 tfec_rep             :  1 ; /* bits 8:8 */
    cs_uint32 rx_rep_mode          :  1 ; /* bits 7:7 */
    cs_uint32 tx_rep_mode          :  1 ; /* bits 6:6 */
    cs_uint32 tx_1st_preamble_mode :  1 ; /* bits 5:5 */
    cs_uint32 rx_1st_preamble_mode :  1 ; /* bits 4:4 */
    cs_uint32 thrshld              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 thrshld              :  4 ; /* bits 3:0 */
    cs_uint32 rx_1st_preamble_mode :  1 ; /* bits 4:4 */
    cs_uint32 tx_1st_preamble_mode :  1 ; /* bits 5:5 */
    cs_uint32 tx_rep_mode          :  1 ; /* bits 6:6 */
    cs_uint32 rx_rep_mode          :  1 ; /* bits 7:7 */
    cs_uint32 tfec_rep             :  1 ; /* bits 8:8 */
    cs_uint32 in_valid_cg          :  1 ; /* bits 9:9 */
    cs_uint32 rx_window_on         :  1 ; /* bits 10:10 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 tx_ipg_adj           :  3 ; /* bits 14:12 */
    cs_uint32 tx_fec8B             :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd2               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_FEC_CFG_t;





#define ONU_MAC_FEC_TX_ERRINJ                    0x08034
#define ONU_MAC_FEC_TX_ERRINJ_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 err_en               :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd4               :  3 ;
    cs_uint32 err_sel              :  1 ; /* bits 27:27 */
    cs_uint32 err_burst            :  1 ; /* bits 26:26 */
    cs_uint32 rsrvd3               :  2 ;
    cs_uint32 err_pktcnt           :  8 ; /* bits 23:16 */
    cs_uint32 err_bitcnt           :  2 ; /* bits 15:14 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 err_bytecnt          :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 err_tfec2            :  1 ; /* bits 4:4 */
    cs_uint32 err_prty             :  1 ; /* bits 3:3 */
    cs_uint32 err_tfec1            :  1 ; /* bits 2:2 */
    cs_uint32 err_data             :  1 ; /* bits 1:1 */
    cs_uint32 err_sfec             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 err_sfec             :  1 ; /* bits 0:0 */
    cs_uint32 err_data             :  1 ; /* bits 1:1 */
    cs_uint32 err_tfec1            :  1 ; /* bits 2:2 */
    cs_uint32 err_prty             :  1 ; /* bits 3:3 */
    cs_uint32 err_tfec2            :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 err_bytecnt          :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 err_bitcnt           :  2 ; /* bits 15:14 */
    cs_uint32 err_pktcnt           :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd3               :  2 ;
    cs_uint32 err_burst            :  1 ; /* bits 26:26 */
    cs_uint32 err_sel              :  1 ; /* bits 27:27 */
    cs_uint32 rsrvd4               :  3 ;
    cs_uint32 err_en               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_FEC_TX_ERRINJ_t;





#define ONU_MAC_SFEC_CNT                         0x08035
#define ONU_MAC_SFEC_CNT_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 sfecCnt              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sfecCnt              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_SFEC_CNT_t;





#define ONU_MAC_TFEC_CNT                         0x08036
#define ONU_MAC_TFEC_CNT_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tfecCnt              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tfecCnt              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_TFEC_CNT_t;





#define ONU_MAC_SPKT_CNT                         0x08037
#define ONU_MAC_SPKT_CNT_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 spktCnt              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 spktCnt              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_SPKT_CNT_t;





#define ONU_MAC_EPKT_CNT                         0x08038
#define ONU_MAC_EPKT_CNT_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 epktCnt              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 epktCnt              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_EPKT_CNT_t;





#define ONU_MAC_FEC_DEFINE_PKTSIZE               0x08039
#define ONU_MAC_FEC_DEFINE_PKTSIZE_dft           0x000327f8




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 min_pkt_size         : 12 ; /* bits 23:12 */
    cs_uint32 max_pkt_size         : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 max_pkt_size         : 12 ; /* bits 11:0 */
    cs_uint32 min_pkt_size         : 12 ; /* bits 23:12 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_FEC_DEFINE_PKTSIZE_t;





#define ONU_MAC_FEC_DEFINE_LATENCY               0x0803a
#define ONU_MAC_FEC_DEFINE_LATENCY_dft           0x0014c7fb




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 11 ;
    cs_uint32 fdec_latency         :  9 ; /* bits 20:12 */
    cs_uint32 pckt_latency         : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pckt_latency         : 12 ; /* bits 11:0 */
    cs_uint32 fdec_latency         :  9 ; /* bits 20:12 */
    cs_uint32 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_FEC_DEFINE_LATENCY_t;





#define ONU_MAC_FEC_CTRL                         0x0803b
#define ONU_MAC_FEC_CTRL_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  9 ;
    cs_uint32 fec_dbg_sel          :  3 ; /* bits 22:20 */
    cs_uint32 rsrvd1               :  9 ;
    cs_uint32 rsv10                :  1 ; /* bits 10:10 */
    cs_uint32 halt_cor             :  1 ; /* bits 9:9 */
    cs_uint32 halt_unc             :  1 ; /* bits 8:8 */
    cs_uint32 rsv7                 :  1 ; /* bits 7:7 */
    cs_uint32 cor_ctr_clr          :  1 ; /* bits 6:6 */
    cs_uint32 unc_ctr_clr          :  1 ; /* bits 5:5 */
    cs_uint32 frx_sta_clr          :  1 ; /* bits 4:4 */
    cs_uint32 fec_tx_clr           :  1 ; /* bits 3:3 */
    cs_uint32 fec_rx_clr           :  1 ; /* bits 2:2 */
    cs_uint32 fec_encode_enable    :  1 ; /* bits 1:1 */
    cs_uint32 fec_decode_enable    :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 fec_decode_enable    :  1 ; /* bits 0:0 */
    cs_uint32 fec_encode_enable    :  1 ; /* bits 1:1 */
    cs_uint32 fec_rx_clr           :  1 ; /* bits 2:2 */
    cs_uint32 fec_tx_clr           :  1 ; /* bits 3:3 */
    cs_uint32 frx_sta_clr          :  1 ; /* bits 4:4 */
    cs_uint32 unc_ctr_clr          :  1 ; /* bits 5:5 */
    cs_uint32 cor_ctr_clr          :  1 ; /* bits 6:6 */
    cs_uint32 rsv7                 :  1 ; /* bits 7:7 */
    cs_uint32 halt_unc             :  1 ; /* bits 8:8 */
    cs_uint32 halt_cor             :  1 ; /* bits 9:9 */
    cs_uint32 rsv10                :  1 ; /* bits 10:10 */
    cs_uint32 rsrvd1               :  9 ;
    cs_uint32 fec_dbg_sel          :  3 ; /* bits 22:20 */
    cs_uint32 rsrvd2               :  9 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_FEC_CTRL_t;





#define ONU_MAC_FEC_MISC_STATUS                  0x0803c
#define ONU_MAC_FEC_MISC_STATUS_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 12 ;
    cs_uint32 fec_pkt_st           :  4 ; /* bits 19:16 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 fec_dacor_ack_q      :  1 ; /* bits 12:12 */
    cs_uint32 fec_uncorr           :  1 ; /* bits 11:11 */
    cs_uint32 fec_errors           :  4 ; /* bits 10:7 */
    cs_uint32 los_tfec2            :  1 ; /* bits 6:6 */
    cs_uint32 los_tfec1            :  1 ; /* bits 5:5 */
    cs_uint32 pipe_stall           :  1 ; /* bits 4:4 */
    cs_uint32 cwb_full             :  1 ; /* bits 3:3 */
    cs_uint32 cmd_full             :  1 ; /* bits 2:2 */
    cs_uint32 prty_full            :  1 ; /* bits 1:1 */
    cs_uint32 pack_full            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pack_full            :  1 ; /* bits 0:0 */
    cs_uint32 prty_full            :  1 ; /* bits 1:1 */
    cs_uint32 cmd_full             :  1 ; /* bits 2:2 */
    cs_uint32 cwb_full             :  1 ; /* bits 3:3 */
    cs_uint32 pipe_stall           :  1 ; /* bits 4:4 */
    cs_uint32 los_tfec1            :  1 ; /* bits 5:5 */
    cs_uint32 los_tfec2            :  1 ; /* bits 6:6 */
    cs_uint32 fec_errors           :  4 ; /* bits 10:7 */
    cs_uint32 fec_uncorr           :  1 ; /* bits 11:11 */
    cs_uint32 fec_dacor_ack_q      :  1 ; /* bits 12:12 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 fec_pkt_st           :  4 ; /* bits 19:16 */
    cs_uint32 rsrvd2               : 12 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_FEC_MISC_STATUS_t;





#define ONU_MAC_FEC_Counter_Control              0x0803d
#define ONU_MAC_FEC_Counter_Control_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 FEC_CNTR_rd_clr_mode :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 FEC_CNTR_rd_clr_mode :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_FEC_Counter_Control_t;





#define ONU_MAC_FEC_Block_Correctable_Error_Counter 0x0803e
#define ONU_MAC_FEC_Block_Correctable_Error_Counter_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 FEC_RCNT             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 FEC_RCNT             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_FEC_Block_Correctable_Error_Counter_t;





#define ONU_MAC_FEC_Block_Uncorrectable_Error_Counter 0x0803f
#define ONU_MAC_FEC_Block_Uncorrectable_Error_Counter_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 FEC_UCNT             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 FEC_UCNT             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_FEC_Block_Uncorrectable_Error_Counter_t;





#define ONU_MAC_FEC_Block_No_Error_Counter       0x08040
#define ONU_MAC_FEC_Block_No_Error_Counter_dft   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 FEC_NOERR            : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 FEC_NOERR            : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_FEC_Block_No_Error_Counter_t;





#define ONU_MAC_FEC_Block_Counter                0x08041
#define ONU_MAC_FEC_Block_Counter_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 FEC_BLK_CNT          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 FEC_BLK_CNT          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_FEC_Block_Counter_t;





#define ONU_MAC_FEC_Corr_Byte_Counter            0x08042
#define ONU_MAC_FEC_Corr_Byte_Counter_dft        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 FEC_CORR_BYTE_CNT    : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 FEC_CORR_BYTE_CNT    : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_FEC_Corr_Byte_Counter_t;





#define ONU_MAC_LLID_FILTERING_CFG               0x08043
#define ONU_MAC_LLID_FILTERING_CFG_dft           0x00000008




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 disable_crc8_checking :  1 ; /* bits 8:8 */
    cs_uint32 CRC8_order_select    :  2 ; /* bits 7:6 */
    cs_uint32 PON_Port_only_dis_unicast_llid :  1 ; /* bits 5:5 */
    cs_uint32 PON_Port_only_dis_bcast_llid :  1 ; /* bits 4:4 */
    cs_uint32 PON_Port_only_rx_7FFF_llid_disable :  1 ; /* bits 3:3 */
    cs_uint32 disable_p2p_llid_filtering :  1 ; /* bits 2:2 */
    cs_uint32 enable_cs_llid_filtering :  1 ; /* bits 1:1 */
    cs_uint32 disable_LLID_filtering :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 disable_LLID_filtering :  1 ; /* bits 0:0 */
    cs_uint32 enable_cs_llid_filtering :  1 ; /* bits 1:1 */
    cs_uint32 disable_p2p_llid_filtering :  1 ; /* bits 2:2 */
    cs_uint32 PON_Port_only_rx_7FFF_llid_disable :  1 ; /* bits 3:3 */
    cs_uint32 PON_Port_only_dis_bcast_llid :  1 ; /* bits 4:4 */
    cs_uint32 PON_Port_only_dis_unicast_llid :  1 ; /* bits 5:5 */
    cs_uint32 CRC8_order_select    :  2 ; /* bits 7:6 */
    cs_uint32 disable_crc8_checking :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_LLID_FILTERING_CFG_t;





#define ONU_MAC_CS0_LLID_Filtering_Pattern_Mask  0x08044
#define ONU_MAC_CS0_LLID_Filtering_Pattern_Mask_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CS0_LLID_Filtering_Pattern_Mask_t;





#define ONU_MAC_CS1_LLID_Filtering_Pattern_Mask  0x08045
#define ONU_MAC_CS1_LLID_Filtering_Pattern_Mask_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CS1_LLID_Filtering_Pattern_Mask_t;





#define ONU_MAC_CS2_LLID_Filtering_Pattern_Mask  0x08046
#define ONU_MAC_CS2_LLID_Filtering_Pattern_Mask_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CS2_LLID_Filtering_Pattern_Mask_t;





#define ONU_MAC_CS3_LLID_Filtering_Pattern_Mask  0x08047
#define ONU_MAC_CS3_LLID_Filtering_Pattern_Mask_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CS3_LLID_Filtering_Pattern_Mask_t;





#define ONU_MAC_CS4_LLID_Filtering_Pattern_Mask  0x08048
#define ONU_MAC_CS4_LLID_Filtering_Pattern_Mask_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CS4_LLID_Filtering_Pattern_Mask_t;





#define ONU_MAC_CS5_LLID_Filtering_Pattern_Mask  0x08049
#define ONU_MAC_CS5_LLID_Filtering_Pattern_Mask_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CS5_LLID_Filtering_Pattern_Mask_t;





#define ONU_MAC_CS6_LLID_Filtering_Pattern_Mask  0x0804a
#define ONU_MAC_CS6_LLID_Filtering_Pattern_Mask_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CS6_LLID_Filtering_Pattern_Mask_t;





#define ONU_MAC_CS7_LLID_Filtering_Pattern_Mask  0x0804b
#define ONU_MAC_CS7_LLID_Filtering_Pattern_Mask_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 CS_LLID_pattern      : 16 ; /* bits 15:0 */
    cs_uint32 CS_LLID_MSK          : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CS7_LLID_Filtering_Pattern_Mask_t;





#define ONU_MAC_RX_MAXPKTSIZE                    0x0804c
#define ONU_MAC_RX_MAXPKTSIZE_dft                0x000005ee




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 21 ;
    cs_uint32 rx_max_pkt_size      : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_max_pkt_size      : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               : 21 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RX_MAXPKTSIZE_t;





#define ONU_MAC_MAC_DROP_CNT                     0x0804d
#define ONU_MAC_MAC_DROP_CNT_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mac_drop_cnt         : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mac_drop_cnt         : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MAC_DROP_CNT_t;





#define ONU_MAC_IPG_DROP_CNT                     0x0804e
#define ONU_MAC_IPG_DROP_CNT_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ipg_drp_cnt          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ipg_drp_cnt          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_IPG_DROP_CNT_t;





#define ONU_MAC_CRC8_ERR_CNT                     0x0804f
#define ONU_MAC_CRC8_ERR_CNT_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 crc8ErrCnt           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 crc8ErrCnt           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CRC8_ERR_CNT_t;





#define ONU_MAC_SLD_ERR_CNT                      0x08050
#define ONU_MAC_SLD_ERR_CNT_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 sldErrCnt            : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sldErrCnt            : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_SLD_ERR_CNT_t;





#define ONU_MAC_RXCTRL                           0x08051
#define ONU_MAC_RXCTRL_dft                       0x0e002100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 bypass               :  1 ; /* bits 31:31 */
    cs_uint32 dpid                 :  3 ; /* bits 30:28 */
    cs_uint32 cos                  :  3 ; /* bits 27:25 */
    cs_uint32 dis_bypass_oam       :  1 ; /* bits 24:24 */
    cs_uint32 dis_bypass_extn      :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 drop_mask            : 11 ; /* bits 21:11 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 max_mtu_tag_en       :  1 ; /* bits 8:8 */
    cs_uint32 sfd_mode2            :  1 ; /* bits 7:7 */
    cs_uint32 gmii                 :  1 ; /* bits 6:6 */
    cs_uint32 ipg_cnt              :  3 ; /* bits 5:3 */
    cs_uint32 mpcp_to_cpu          :  1 ; /* bits 2:2 */
    cs_uint32 rx_pause_enable      :  1 ; /* bits 1:1 */
    cs_uint32 RX_EN                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 RX_EN                :  1 ; /* bits 0:0 */
    cs_uint32 rx_pause_enable      :  1 ; /* bits 1:1 */
    cs_uint32 mpcp_to_cpu          :  1 ; /* bits 2:2 */
    cs_uint32 ipg_cnt              :  3 ; /* bits 5:3 */
    cs_uint32 gmii                 :  1 ; /* bits 6:6 */
    cs_uint32 sfd_mode2            :  1 ; /* bits 7:7 */
    cs_uint32 max_mtu_tag_en       :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 drop_mask            : 11 ; /* bits 21:11 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 dis_bypass_extn      :  1 ; /* bits 23:23 */
    cs_uint32 dis_bypass_oam       :  1 ; /* bits 24:24 */
    cs_uint32 cos                  :  3 ; /* bits 27:25 */
    cs_uint32 dpid                 :  3 ; /* bits 30:28 */
    cs_uint32 bypass               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RXCTRL_t;





#define ONU_MAC_RXMAC_EXTENSION_CFG              0x08052
#define ONU_MAC_RXMAC_EXTENSION_CFG_dft          0x8808fffe




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ethertype            : 16 ; /* bits 31:16 */
    cs_uint32 opcode               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 opcode               : 16 ; /* bits 15:0 */
    cs_uint32 ethertype            : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RXMAC_EXTENSION_CFG_t;





#define ONU_MAC_RXMAC_EXTENSION_OUI              0x08053
#define ONU_MAC_RXMAC_EXTENSION_OUI_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  6 ;
    cs_uint32 check_oui            :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 oui                  : 24 ; /* bits 23:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 oui                  : 24 ; /* bits 23:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 check_oui            :  1 ; /* bits 25:25 */
    cs_uint32 rsrvd2               :  6 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RXMAC_EXTENSION_OUI_t;





#define ONU_MAC_RXMAC_EXTENSION_DA1              0x08054
#define ONU_MAC_RXMAC_EXTENSION_DA1_dft          0x00000180




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 extda                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 extda                : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RXMAC_EXTENSION_DA1_t;





#define ONU_MAC_RXMAC_EXTENSION_DA0              0x08055
#define ONU_MAC_RXMAC_EXTENSION_DA0_dft          0xc2000001




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 extda                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 extda                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RXMAC_EXTENSION_DA0_t;





#define ONU_MAC_RXCTRL_BYPASS_CFG                0x08056
#define ONU_MAC_RXCTRL_BYPASS_CFG_dft            0x9df98000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 oam_dpid             :  3 ; /* bits 31:29 */
    cs_uint32 oam_cos              :  3 ; /* bits 28:26 */
    cs_uint32 mpcp_dpid            :  3 ; /* bits 25:23 */
    cs_uint32 mpcp_cos             :  3 ; /* bits 22:20 */
    cs_uint32 extn_dpid            :  3 ; /* bits 19:17 */
    cs_uint32 extn_cos             :  3 ; /* bits 16:14 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 dbg1                 :  1 ; /* bits 8:8 */
    cs_uint32 bypass_mask          :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 bypass_mask          :  8 ; /* bits 7:0 */
    cs_uint32 dbg1                 :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 extn_cos             :  3 ; /* bits 16:14 */
    cs_uint32 extn_dpid            :  3 ; /* bits 19:17 */
    cs_uint32 mpcp_cos             :  3 ; /* bits 22:20 */
    cs_uint32 mpcp_dpid            :  3 ; /* bits 25:23 */
    cs_uint32 oam_cos              :  3 ; /* bits 28:26 */
    cs_uint32 oam_dpid             :  3 ; /* bits 31:29 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RXCTRL_BYPASS_CFG_t;





#define ONU_MAC_RXMAC_CFG                        0x08057
#define ONU_MAC_RXMAC_CFG_dft                    0x00ffff40




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  8 ;
    cs_uint32 mtu_tag_pattern      : 16 ; /* bits 23:8 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 filtering_threshold  :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 filtering_threshold  :  7 ; /* bits 6:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 mtu_tag_pattern      : 16 ; /* bits 23:8 */
    cs_uint32 rsrvd2               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RXMAC_CFG_t;





#define ONU_MAC_MPCP_ENABLE_CONTROL              0x08058
#define ONU_MAC_MPCP_ENABLE_CONTROL_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 ONU_local_offline    :  1 ; /* bits 1:1 */
    cs_uint32 mpcp_registration_init :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mpcp_registration_init :  1 ; /* bits 0:0 */
    cs_uint32 ONU_local_offline    :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_ENABLE_CONTROL_t;





#define ONU_MAC_MPCP_CONTROL                     0x08059
#define ONU_MAC_MPCP_CONTROL_dft                 0x0000000c




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 clr_pmit             :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd8               :  2 ;
    cs_uint32 register_request_frame_LLID_select :  1 ; /* bits 28:28 */
    cs_uint32 rsrvd7               :  1 ;
    cs_uint32 HW_RSV26             :  1 ; /* bits 26:26 */
    cs_uint32 gate_timer_expiration_deregister :  1 ; /* bits 25:25 */
    cs_uint32 HW_RSV24             :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd6               :  2 ;
    cs_uint32 onu_mac_address_checking :  1 ; /* bits 21:21 */
    cs_uint32 rsrvd5               :  2 ;
    cs_uint32 timestamp_drift_deregister :  1 ; /* bits 18:18 */
    cs_uint32 rsrvd4               :  2 ;
    cs_uint32 disable_mpcp_frame_length_checking :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd3               :  2 ;
    cs_uint32 allow_direct_registration :  1 ; /* bits 12:12 */
    cs_uint32 strict_starttime_check :  1 ; /* bits 11:11 */
    cs_uint32 rtt_comp_enable      :  1 ; /* bits 10:10 */
    cs_uint32 disable_mpcp_timer_deregister :  1 ; /* bits 9:9 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 en_rpt_data          :  1 ; /* bits 7:7 */
    cs_uint32 disable_minimum_processing_time_check :  1 ; /* bits 6:6 */
    cs_uint32 timestamp_update_location :  2 ; /* bits 5:4 */
    cs_uint32 mpcp_frame_tx_first  :  1 ; /* bits 3:3 */
    cs_uint32 mpcp_mode            :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               :  2 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 mpcp_mode            :  1 ; /* bits 2:2 */
    cs_uint32 mpcp_frame_tx_first  :  1 ; /* bits 3:3 */
    cs_uint32 timestamp_update_location :  2 ; /* bits 5:4 */
    cs_uint32 disable_minimum_processing_time_check :  1 ; /* bits 6:6 */
    cs_uint32 en_rpt_data          :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 disable_mpcp_timer_deregister :  1 ; /* bits 9:9 */
    cs_uint32 rtt_comp_enable      :  1 ; /* bits 10:10 */
    cs_uint32 strict_starttime_check :  1 ; /* bits 11:11 */
    cs_uint32 allow_direct_registration :  1 ; /* bits 12:12 */
    cs_uint32 rsrvd3               :  2 ;
    cs_uint32 disable_mpcp_frame_length_checking :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd4               :  2 ;
    cs_uint32 timestamp_drift_deregister :  1 ; /* bits 18:18 */
    cs_uint32 rsrvd5               :  2 ;
    cs_uint32 onu_mac_address_checking :  1 ; /* bits 21:21 */
    cs_uint32 rsrvd6               :  2 ;
    cs_uint32 HW_RSV24             :  1 ; /* bits 24:24 */
    cs_uint32 gate_timer_expiration_deregister :  1 ; /* bits 25:25 */
    cs_uint32 HW_RSV26             :  1 ; /* bits 26:26 */
    cs_uint32 rsrvd7               :  1 ;
    cs_uint32 register_request_frame_LLID_select :  1 ; /* bits 28:28 */
    cs_uint32 rsrvd8               :  2 ;
    cs_uint32 clr_pmit             :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_CONTROL_t;





#define ONU_MAC_MPCP_CFG                         0x0805a
#define ONU_MAC_MPCP_CFG_dft                     0xff000804




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rpt_pri              :  8 ; /* bits 31:24 */
    cs_uint32 dbg_sel              :  4 ; /* bits 23:20 */
    cs_uint32 rereg_mode           :  1 ; /* bits 19:19 */
    cs_uint32 rpt_mode             :  3 ; /* bits 18:16 */
    cs_uint32 guardThresholdONU    :  8 ; /* bits 15:8 */
    cs_uint32 mpcp_ipg             :  4 ; /* bits 7:4 */
    cs_uint32 pending_grant_select :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pending_grant_select :  4 ; /* bits 3:0 */
    cs_uint32 mpcp_ipg             :  4 ; /* bits 7:4 */
    cs_uint32 guardThresholdONU    :  8 ; /* bits 15:8 */
    cs_uint32 rpt_mode             :  3 ; /* bits 18:16 */
    cs_uint32 rereg_mode           :  1 ; /* bits 19:19 */
    cs_uint32 dbg_sel              :  4 ; /* bits 23:20 */
    cs_uint32 rpt_pri              :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_CFG_t;





#define ONU_MAC_MPCP_PKT_SIZE_CFG                0x0805b
#define ONU_MAC_MPCP_PKT_SIZE_CFG_dft            0x00002a2a




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 min_grant_size       :  8 ; /* bits 15:8 */
    cs_uint32 discovery_mpcp_pkt_size :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 discovery_mpcp_pkt_size :  8 ; /* bits 7:0 */
    cs_uint32 min_grant_size       :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_PKT_SIZE_CFG_t;





#define ONU_MAC_MPCP_TIMERS_CFG                  0x0805c
#define ONU_MAC_MPCP_TIMERS_CFG_dft              0x003203e8




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 gate_timer_enable    :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 gate_timer_threshold_value_select : 12 ; /* bits 27:16 */
    cs_uint32 mpcp_timer_enable    :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 mpcp_timer_threshold_value_select : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mpcp_timer_threshold_value_select : 12 ; /* bits 11:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 mpcp_timer_enable    :  1 ; /* bits 15:15 */
    cs_uint32 gate_timer_threshold_value_select : 12 ; /* bits 27:16 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 gate_timer_enable    :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_TIMERS_CFG_t;





#define ONU_MAC_MPCP_REPORT_TIMER_CFG            0x0805d
#define ONU_MAC_MPCP_REPORT_TIMER_CFG_dft        0x00008032




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 16 ;
    cs_uint32 report_timer_enable  :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 report_timer_threshold_value_select : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 report_timer_threshold_value_select : 12 ; /* bits 11:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 report_timer_enable  :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd2               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_REPORT_TIMER_CFG_t;





#define ONU_MAC_AES_CHURNING_CTRL                0x0805e
#define ONU_MAC_AES_CHURNING_CTRL_dft            0x00000080




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 23 ;
    cs_uint32 rcc_correct_dis      :  1 ; /* bits 8:8 */
    cs_uint32 HW_key_mode          :  1 ; /* bits 7:7 */
    cs_uint32 revser_sel           :  1 ; /* bits 6:6 */
    cs_uint32 key_rdout_sel        :  1 ; /* bits 5:5 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 reverse_bit_order_of_random_generated_key :  1 ; /* bits 2:2 */
    cs_uint32 enable_single_de_churning_function :  1 ; /* bits 1:1 */
    cs_uint32 enable_decryption_function_churning_or_AES :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 enable_decryption_function_churning_or_AES :  1 ; /* bits 0:0 */
    cs_uint32 enable_single_de_churning_function :  1 ; /* bits 1:1 */
    cs_uint32 reverse_bit_order_of_random_generated_key :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 key_rdout_sel        :  1 ; /* bits 5:5 */
    cs_uint32 revser_sel           :  1 ; /* bits 6:6 */
    cs_uint32 HW_key_mode          :  1 ; /* bits 7:7 */
    cs_uint32 rcc_correct_dis      :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd2               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_AES_CHURNING_CTRL_t;





#define ONU_MAC_MPCP_Configuration               0x0805f
#define ONU_MAC_MPCP_Configuration_dft           0x0f040400




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mpcp_frame_fec_overhead :  8 ; /* bits 31:24 */
    cs_uint32 transmit_window_starttime_adjust :  8 ; /* bits 23:16 */
    cs_uint32 drain_window_guard_time :  8 ; /* bits 15:8 */
    cs_uint32 reserved             :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 reserved             :  8 ; /* bits 7:0 */
    cs_uint32 drain_window_guard_time :  8 ; /* bits 15:8 */
    cs_uint32 transmit_window_starttime_adjust :  8 ; /* bits 23:16 */
    cs_uint32 mpcp_frame_fec_overhead :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_Configuration_t;





#define ONU_MAC_MPCP_LLID_SyncTime               0x08060
#define ONU_MAC_MPCP_LLID_SyncTime_dft           0x7fff0000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 LLID                 : 16 ; /* bits 31:16 */
    cs_uint32 MPCP_SYNC            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 MPCP_SYNC            : 16 ; /* bits 15:0 */
    cs_uint32 LLID                 : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_LLID_SyncTime_t;





#define ONU_MAC_MPCP_MISC_CTRL                   0x08061
#define ONU_MAC_MPCP_MISC_CTRL_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mpcp_frm_timer_enable :  1 ; /* bits 31:31 */
    cs_uint32 mpcp_rpt_timer_enable :  1 ; /* bits 30:30 */
    cs_uint32 mpcp_ack_timer_enable :  1 ; /* bits 29:29 */
    cs_uint32 mpcp_reg_req_timer_enable :  1 ; /* bits 28:28 */
    cs_uint32 rsrvd1               : 28 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 mpcp_reg_req_timer_enable :  1 ; /* bits 28:28 */
    cs_uint32 mpcp_ack_timer_enable :  1 ; /* bits 29:29 */
    cs_uint32 mpcp_rpt_timer_enable :  1 ; /* bits 30:30 */
    cs_uint32 mpcp_frm_timer_enable :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_MISC_CTRL_t;





#define ONU_MAC_MPCP_GRANT_PROCESS_TIME          0x08062
#define ONU_MAC_MPCP_GRANT_PROCESS_TIME_dft      0x0fffff00




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 min_processing_time  : 10 ; /* bits 27:18 */
    cs_uint32 max_future_grant_time : 10 ; /* bits 17:8 */
    cs_uint32 rsrvd1               :  8 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 max_future_grant_time : 10 ; /* bits 17:8 */
    cs_uint32 min_processing_time  : 10 ; /* bits 27:18 */
    cs_uint32 rsrvd2               :  4 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_GRANT_PROCESS_TIME_t;





#define ONU_MAC_MPCP_STATUS                      0x08063
#define ONU_MAC_MPCP_STATUS_dft                  0x08000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 onureg               :  1 ; /* bits 31:31 */
    cs_uint32 retry                :  1 ; /* bits 30:30 */
    cs_uint32 permit               :  1 ; /* bits 29:29 */
    cs_uint32 prereg               :  1 ; /* bits 28:28 */
    cs_uint32 empty_grt            :  1 ; /* bits 27:27 */
    cs_uint32 full_grt             :  1 ; /* bits 26:26 */
    cs_uint32 normal_gate_before_reged :  1 ; /* bits 25:25 */
    cs_uint32 force_report_gate_before_reged :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 retry_cnt            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 retry_cnt            : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 force_report_gate_before_reged :  1 ; /* bits 24:24 */
    cs_uint32 normal_gate_before_reged :  1 ; /* bits 25:25 */
    cs_uint32 full_grt             :  1 ; /* bits 26:26 */
    cs_uint32 empty_grt            :  1 ; /* bits 27:27 */
    cs_uint32 prereg               :  1 ; /* bits 28:28 */
    cs_uint32 permit               :  1 ; /* bits 29:29 */
    cs_uint32 retry                :  1 ; /* bits 30:30 */
    cs_uint32 onureg               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_STATUS_t;





#define ONU_MAC_MPCP_REGISTER_PARAMETERS1        0x08064
#define ONU_MAC_MPCP_REGISTER_PARAMETERS1_dft    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 flag                 :  8 ; /* bits 31:24 */
    cs_uint32 pending_grant        :  8 ; /* bits 23:16 */
    cs_uint32 synctime             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 synctime             : 16 ; /* bits 15:0 */
    cs_uint32 pending_grant        :  8 ; /* bits 23:16 */
    cs_uint32 flag                 :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_REGISTER_PARAMETERS1_t;





#define ONU_MAC_MPCP_REGISTER_PARAMETERS2        0x08065
#define ONU_MAC_MPCP_REGISTER_PARAMETERS2_dft    0x00007fff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 llid                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 llid                 : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_REGISTER_PARAMETERS2_t;





#define ONU_MAC_MPCP_OLTMAC1                     0x08066
#define ONU_MAC_MPCP_OLTMAC1_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 olt_mac              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 olt_mac              : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_OLTMAC1_t;





#define ONU_MAC_MPCP_OLTMAC0                     0x08067
#define ONU_MAC_MPCP_OLTMAC0_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 olt_mac              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 olt_mac              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_OLTMAC0_t;





#define ONU_MAC_MPCP_RPT_SCAN_CFG                0x08068
#define ONU_MAC_MPCP_RPT_SCAN_CFG_dft            0x000001e7




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 20 ;
    cs_uint32 scan_start_ahead_7p8us : 12 ; /* bits 11:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 scan_start_ahead_7p8us : 12 ; /* bits 11:0 */
    cs_uint32 rsrvd1               : 20 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_RPT_SCAN_CFG_t;





#define ONU_MAC_MPCP_MONITOR_CNT                 0x08069
#define ONU_MAC_MPCP_MONITOR_CNT_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 reged_cnt            :  8 ; /* bits 31:24 */
    cs_uint32 gate_timer_exp_cnt   :  8 ; /* bits 23:16 */
    cs_uint32 mp_timer_exp_cnt     :  8 ; /* bits 15:8 */
    cs_uint32 timestamp_drift_cnt  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 timestamp_drift_cnt  :  8 ; /* bits 7:0 */
    cs_uint32 mp_timer_exp_cnt     :  8 ; /* bits 15:8 */
    cs_uint32 gate_timer_exp_cnt   :  8 ; /* bits 23:16 */
    cs_uint32 reged_cnt            :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_MONITOR_CNT_t;





#define ONU_MAC_MPCP_REG_REQ_RESV0               0x0806a
#define ONU_MAC_MPCP_REG_REQ_RESV0_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_REG_REQ_RESV0_t;





#define ONU_MAC_MPCP_REG_REQ_RESV1               0x0806b
#define ONU_MAC_MPCP_REG_REQ_RESV1_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_REG_REQ_RESV1_t;





#define ONU_MAC_MPCP_REG_REQ_RESV2               0x0806c
#define ONU_MAC_MPCP_REG_REQ_RESV2_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_REG_REQ_RESV2_t;





#define ONU_MAC_MPCP_REG_REQ_RESV3               0x0806d
#define ONU_MAC_MPCP_REG_REQ_RESV3_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_REG_REQ_RESV3_t;





#define ONU_MAC_MPCP_REG_REQ_RESV4               0x0806e
#define ONU_MAC_MPCP_REG_REQ_RESV4_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_REG_REQ_RESV4_t;





#define ONU_MAC_MPCP_REG_REQ_RESV5               0x0806f
#define ONU_MAC_MPCP_REG_REQ_RESV5_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_REG_REQ_RESV5_t;





#define ONU_MAC_MPCP_REG_REQ_RESV6               0x08070
#define ONU_MAC_MPCP_REG_REQ_RESV6_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_REG_REQ_RESV6_t;





#define ONU_MAC_MPCP_REG_REQ_RESV7               0x08071
#define ONU_MAC_MPCP_REG_REQ_RESV7_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_REG_REQ_RESV7_t;





#define ONU_MAC_MPCP_REG_REQ_RESV8               0x08072
#define ONU_MAC_MPCP_REG_REQ_RESV8_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_REG_REQ_RESV8_t;





#define ONU_MAC_MPCP_REG_REQ_RESV9               0x08073
#define ONU_MAC_MPCP_REG_REQ_RESV9_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_REG_REQ_RESV9_t;





#define ONU_MAC_MPCP_ACK_RESV0                   0x08074
#define ONU_MAC_MPCP_ACK_RESV0_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_ACK_RESV0_t;





#define ONU_MAC_MPCP_ACK_RESV1                   0x08075
#define ONU_MAC_MPCP_ACK_RESV1_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_ACK_RESV1_t;





#define ONU_MAC_MPCP_ACK_RESV2                   0x08076
#define ONU_MAC_MPCP_ACK_RESV2_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_ACK_RESV2_t;





#define ONU_MAC_MPCP_ACK_RESV3                   0x08077
#define ONU_MAC_MPCP_ACK_RESV3_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_ACK_RESV3_t;





#define ONU_MAC_MPCP_ACK_RESV4                   0x08078
#define ONU_MAC_MPCP_ACK_RESV4_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_ACK_RESV4_t;





#define ONU_MAC_MPCP_ACK_RESV5                   0x08079
#define ONU_MAC_MPCP_ACK_RESV5_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_ACK_RESV5_t;





#define ONU_MAC_MPCP_ACK_RESV6                   0x0807a
#define ONU_MAC_MPCP_ACK_RESV6_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_ACK_RESV6_t;





#define ONU_MAC_MPCP_ACK_RESV7                   0x0807b
#define ONU_MAC_MPCP_ACK_RESV7_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_ACK_RESV7_t;





#define ONU_MAC_MPCP_ACK_RESV8                   0x0807c
#define ONU_MAC_MPCP_ACK_RESV8_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 value                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_ACK_RESV8_t;





#define ONU_MAC_Debug_MPCP_Gate_Counter1         0x0807d
#define ONU_MAC_Debug_MPCP_Gate_Counter1_dft     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 Good_gate            : 16 ; /* bits 31:16 */
    cs_uint32 Dis_gate             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 Dis_gate             : 16 ; /* bits 15:0 */
    cs_uint32 Good_gate            : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_Debug_MPCP_Gate_Counter1_t;





#define ONU_MAC_Debug_MPCP_Gate_Counter2         0x0807e
#define ONU_MAC_Debug_MPCP_Gate_Counter2_dft     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 Force_report_gate    : 16 ; /* bits 31:16 */
    cs_uint32 Normal_gate          : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 Normal_gate          : 16 ; /* bits 15:0 */
    cs_uint32 Force_report_gate    : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_Debug_MPCP_Gate_Counter2_t;





#define ONU_MAC_Debug_MPCP_Register_Counter      0x0807f
#define ONU_MAC_Debug_MPCP_Register_Counter_dft  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 Reg_ack              :  8 ; /* bits 31:24 */
    cs_uint32 Reg_nack             :  8 ; /* bits 23:16 */
    cs_uint32 Reg_dereg            :  8 ; /* bits 15:8 */
    cs_uint32 Reg_rereg            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 Reg_rereg            :  8 ; /* bits 7:0 */
    cs_uint32 Reg_dereg            :  8 ; /* bits 15:8 */
    cs_uint32 Reg_nack             :  8 ; /* bits 23:16 */
    cs_uint32 Reg_ack              :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_Debug_MPCP_Register_Counter_t;





#define ONU_MAC_LOCAL_TIMER                      0x08080
#define ONU_MAC_LOCAL_TIMER_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 local_timer          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 local_timer          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_LOCAL_TIMER_t;





#define ONU_MAC_DEQUE                            0x08081
#define ONU_MAC_DEQUE_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 en                   :  1 ; /* bits 3:3 */
    cs_uint32 mpcp_id              :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mpcp_id              :  3 ; /* bits 2:0 */
    cs_uint32 en                   :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_DEQUE_t;





#define ONU_MAC_MPCP_DEREG_CAUSE                 0x08082
#define ONU_MAC_MPCP_DEREG_CAUSE_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 code                 :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 code                 :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_DEREG_CAUSE_t;





#define ONU_MAC_MPCP_DISCOVERY_PARAMETER1        0x08083
#define ONU_MAC_MPCP_DISCOVERY_PARAMETER1_dft    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 discovery_period     : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 discovery_period     : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_DISCOVERY_PARAMETER1_t;





#define ONU_MAC_MPCP_DISCOVERY_PARAMETER2        0x08084
#define ONU_MAC_MPCP_DISCOVERY_PARAMETER2_dft    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 12 ;
    cs_uint32 starttime_ahead      : 20 ; /* bits 19:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 starttime_ahead      : 20 ; /* bits 19:0 */
    cs_uint32 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_DISCOVERY_PARAMETER2_t;





#define ONU_MAC_MPCP_DISCOVERY_PARAMETER3        0x08085
#define ONU_MAC_MPCP_DISCOVERY_PARAMETER3_dft    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 synctime             :  8 ; /* bits 23:16 */
    cs_uint32 length               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 length               : 16 ; /* bits 15:0 */
    cs_uint32 synctime             :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_DISCOVERY_PARAMETER3_t;





#define ONU_MAC_MPCP_TX_ELAPSE                   0x08086
#define ONU_MAC_MPCP_TX_ELAPSE_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tx_elapse            : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_elapse            : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_TX_ELAPSE_t;





#define ONU_MAC_MPCP_RX_ELAPSE                   0x08087
#define ONU_MAC_MPCP_RX_ELAPSE_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rx_elapse            : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_elapse            : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_RX_ELAPSE_t;





#define ONU_MAC_MPCP_DEREG_HISTORY               0x08088
#define ONU_MAC_MPCP_DEREG_HISTORY_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 last_dereg_code      :  4 ; /* bits 23:20 */
    cs_uint32 first_dereg_code     :  4 ; /* bits 19:16 */
    cs_uint32 dereg_cnt            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dereg_cnt            : 16 ; /* bits 15:0 */
    cs_uint32 first_dereg_code     :  4 ; /* bits 19:16 */
    cs_uint32 last_dereg_code      :  4 ; /* bits 23:20 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_DEREG_HISTORY_t;





#define ONU_MAC_MPCP_DRIFT_FIRST_LT              0x08089
#define ONU_MAC_MPCP_DRIFT_FIRST_LT_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 local_timer          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 local_timer          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_DRIFT_FIRST_LT_t;





#define ONU_MAC_MPCP_DRIFT_FIRST_TS              0x0808a
#define ONU_MAC_MPCP_DRIFT_FIRST_TS_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 timestamp            : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 timestamp            : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_DRIFT_FIRST_TS_t;





#define ONU_MAC_MPCP_DRIFT_LAST_LT               0x0808b
#define ONU_MAC_MPCP_DRIFT_LAST_LT_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 local_timer          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 local_timer          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_DRIFT_LAST_LT_t;





#define ONU_MAC_MPCP_DRIFT_LAST_TS               0x0808c
#define ONU_MAC_MPCP_DRIFT_LAST_TS_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 timestamp            : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 timestamp            : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_DRIFT_LAST_TS_t;





#define ONU_MAC_CHURN_CONFIG_CONTROL             0x0808d
#define ONU_MAC_CHURN_CONFIG_CONTROL_dft         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd4               : 16 ;
    cs_uint32 rsv15                :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd3               :  6 ;
    cs_uint32 key_sel_mode         :  1 ; /* bits 8:8 */
    cs_uint32 SW_sel               :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 enable_Transmit_churning_OAM :  1 ; /* bits 4:4 */
    cs_uint32 select_sw_key_as_decryption_key :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               :  3 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 select_sw_key_as_decryption_key :  1 ; /* bits 3:3 */
    cs_uint32 enable_Transmit_churning_OAM :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 SW_sel               :  1 ; /* bits 7:7 */
    cs_uint32 key_sel_mode         :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd3               :  6 ;
    cs_uint32 rsv15                :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd4               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CHURN_CONFIG_CONTROL_t;





#define ONU_MAC_HW_GEN_KEY03                     0x0808e
#define ONU_MAC_HW_GEN_KEY03_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_HW_GEN_KEY03_t;





#define ONU_MAC_HW_GEN_KEY02                     0x0808f
#define ONU_MAC_HW_GEN_KEY02_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_HW_GEN_KEY02_t;





#define ONU_MAC_HW_GEN_KEY01                     0x08090
#define ONU_MAC_HW_GEN_KEY01_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_HW_GEN_KEY01_t;





#define ONU_MAC_HW_GEN_KEY00                     0x08091
#define ONU_MAC_HW_GEN_KEY00_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key0                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_HW_GEN_KEY00_t;





#define ONU_MAC_HW_GEN_KEY13                     0x08092
#define ONU_MAC_HW_GEN_KEY13_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_HW_GEN_KEY13_t;





#define ONU_MAC_HW_GEN_KEY12                     0x08093
#define ONU_MAC_HW_GEN_KEY12_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_HW_GEN_KEY12_t;





#define ONU_MAC_HW_GEN_KEY11                     0x08094
#define ONU_MAC_HW_GEN_KEY11_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_HW_GEN_KEY11_t;





#define ONU_MAC_HW_GEN_KEY10                     0x08095
#define ONU_MAC_HW_GEN_KEY10_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key1                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_HW_GEN_KEY10_t;





#define ONU_MAC_KEY_REQUEST_OAM                  0x08096
#define ONU_MAC_KEY_REQUEST_OAM_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 20 ;
    cs_uint32 churning_key_update_counter :  4 ; /* bits 11:8 */
    cs_uint32 key_index            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 key_index            :  8 ; /* bits 7:0 */
    cs_uint32 churning_key_update_counter :  4 ; /* bits 11:8 */
    cs_uint32 rsrvd1               : 20 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_KEY_REQUEST_OAM_t;





#define ONU_MAC_TXCTRL                           0x08097
#define ONU_MAC_TXCTRL_dft                       0x0001e000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tx_pause_enable      :  1 ; /* bits 31:31 */
    cs_uint32 max_pkt_size_limit   :  1 ; /* bits 30:30 */
    cs_uint32 tx_fec_ipg_delay     :  4 ; /* bits 29:26 */
    cs_uint32 disable_crc32_re_generation :  1 ; /* bits 25:25 */
    cs_uint32 bf                   :  1 ; /* bits 24:24 */
    cs_uint32 crc8_calculation_order_select :  2 ; /* bits 23:22 */
    cs_uint32 ipg_sel              :  2 ; /* bits 21:20 */
    cs_uint32 laser_delay          :  8 ; /* bits 19:12 */
    cs_uint32 reserved_11          :  1 ; /* bits 11:11 */
    cs_uint32 reserved_10          :  1 ; /* bits 10:10 */
    cs_uint32 reserved_9           :  1 ; /* bits 9:9 */
    cs_uint32 reserved_8           :  1 ; /* bits 8:8 */
    cs_uint32 reserved_7           :  1 ; /* bits 7:7 */
    cs_uint32 reserved_6           :  1 ; /* bits 6:6 */
    cs_uint32 reserved_5           :  1 ; /* bits 5:5 */
    cs_uint32 reserved_4           :  1 ; /* bits 4:4 */
    cs_uint32 drain                :  1 ; /* bits 3:3 */
    cs_uint32 reserved_2           :  1 ; /* bits 2:2 */
    cs_uint32 reserved_1           :  1 ; /* bits 1:1 */
    cs_uint32 TX_EN                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 TX_EN                :  1 ; /* bits 0:0 */
    cs_uint32 reserved_1           :  1 ; /* bits 1:1 */
    cs_uint32 reserved_2           :  1 ; /* bits 2:2 */
    cs_uint32 drain                :  1 ; /* bits 3:3 */
    cs_uint32 reserved_4           :  1 ; /* bits 4:4 */
    cs_uint32 reserved_5           :  1 ; /* bits 5:5 */
    cs_uint32 reserved_6           :  1 ; /* bits 6:6 */
    cs_uint32 reserved_7           :  1 ; /* bits 7:7 */
    cs_uint32 reserved_8           :  1 ; /* bits 8:8 */
    cs_uint32 reserved_9           :  1 ; /* bits 9:9 */
    cs_uint32 reserved_10          :  1 ; /* bits 10:10 */
    cs_uint32 reserved_11          :  1 ; /* bits 11:11 */
    cs_uint32 laser_delay          :  8 ; /* bits 19:12 */
    cs_uint32 ipg_sel              :  2 ; /* bits 21:20 */
    cs_uint32 crc8_calculation_order_select :  2 ; /* bits 23:22 */
    cs_uint32 bf                   :  1 ; /* bits 24:24 */
    cs_uint32 disable_crc32_re_generation :  1 ; /* bits 25:25 */
    cs_uint32 tx_fec_ipg_delay     :  4 ; /* bits 29:26 */
    cs_uint32 max_pkt_size_limit   :  1 ; /* bits 30:30 */
    cs_uint32 tx_pause_enable      :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_TXCTRL_t;





#define ONU_MAC_TX_MAXPKTSIZE                    0x08098
#define ONU_MAC_TX_MAXPKTSIZE_dft                0x00000600




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 tx_max_pkt_size      : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_max_pkt_size      : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_TX_MAXPKTSIZE_t;





#define ONU_MAC_TXMAC_CFG                        0x08099
#define ONU_MAC_TXMAC_CFG_dft                    0x03bb6384




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 ptp_lsr_on_adj       :  8 ; /* bits 29:22 */
    cs_uint32 usr_fifo_high_threshold : 10 ; /* bits 21:12 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 usr_fifo_low_threshold : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 usr_fifo_low_threshold : 10 ; /* bits 9:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 usr_fifo_high_threshold : 10 ; /* bits 21:12 */
    cs_uint32 ptp_lsr_on_adj       :  8 ; /* bits 29:22 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_TXMAC_CFG_t;





#define ONU_MAC_PACKET_DRAIN_CNT                 0x0809a
#define ONU_MAC_PACKET_DRAIN_CNT_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pkt_drain_cnt        : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_drain_cnt        : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PACKET_DRAIN_CNT_t;





#define ONU_MAC_TX_CRCERR_CNT                    0x0809b
#define ONU_MAC_TX_CRCERR_CNT_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 txcrcErrCnt          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 txcrcErrCnt          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_TX_CRCERR_CNT_t;





#define ONU_MAC_CHURNING_OAM_OUI                 0x0809c
#define ONU_MAC_CHURNING_OAM_OUI_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 oam_rpt_len          :  8 ; /* bits 31:24 */
    cs_uint32 Churn_oam_OUI        : 24 ; /* bits 23:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 Churn_oam_OUI        : 24 ; /* bits 23:0 */
    cs_uint32 oam_rpt_len          :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CHURNING_OAM_OUI_t;





#define ONU_MAC_CHURNING_OAM_FLAG                0x0809d
#define ONU_MAC_CHURNING_OAM_FLAG_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 clr_reg_cnt          :  1 ; /* bits 21:21 */
    cs_uint32 clr_gat_exp_cnt      :  1 ; /* bits 20:20 */
    cs_uint32 clr_mp_exp_cnt       :  1 ; /* bits 19:19 */
    cs_uint32 clr_norm_gat         :  1 ; /* bits 18:18 */
    cs_uint32 clr_frpt_gat         :  1 ; /* bits 17:17 */
    cs_uint32 clr_dft_cnt          :  1 ; /* bits 16:16 */
    cs_uint32 churning_oam_flag    : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 churning_oam_flag    : 16 ; /* bits 15:0 */
    cs_uint32 clr_dft_cnt          :  1 ; /* bits 16:16 */
    cs_uint32 clr_frpt_gat         :  1 ; /* bits 17:17 */
    cs_uint32 clr_norm_gat         :  1 ; /* bits 18:18 */
    cs_uint32 clr_mp_exp_cnt       :  1 ; /* bits 19:19 */
    cs_uint32 clr_gat_exp_cnt      :  1 ; /* bits 20:20 */
    cs_uint32 clr_reg_cnt          :  1 ; /* bits 21:21 */
    cs_uint32 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CHURNING_OAM_FLAG_t;





#define ONU_MAC_AES_TIMER                        0x0809e
#define ONU_MAC_AES_TIMER_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 AES_TIMER_47_16      : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 AES_TIMER_47_16      : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_AES_TIMER_t;





#define ONU_MAC_AES_TIMER2                       0x0809f
#define ONU_MAC_AES_TIMER2_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 AES_TIMER_47_32      : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 AES_TIMER_47_32      : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_AES_TIMER2_t;





#define ONU_MAC_MIB_CFG                          0x080a0
#define ONU_MAC_MIB_CFG_dft                      0x0001e5ee




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 too_short_limit      :  6 ; /* bits 17:12 */
    cs_uint32 mib_mode             :  1 ; /* bits 11:11 */
    cs_uint32 rmon_top             : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rmon_top             : 11 ; /* bits 10:0 */
    cs_uint32 mib_mode             :  1 ; /* bits 11:11 */
    cs_uint32 too_short_limit      :  6 ; /* bits 17:12 */
    cs_uint32 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MIB_CFG_t;





#define ONU_MAC_MIB_CFG2                         0x080a1
#define ONU_MAC_MIB_CFG2_dft                     0x00018000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 type_sel             : 18 ; /* bits 17:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 type_sel             : 18 ; /* bits 17:0 */
    cs_uint32 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MIB_CFG2_t;





#define ONU_MAC_MISC                             0x080a2
#define ONU_MAC_MISC_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 mib_status           : 14 ; /* bits 15:2 */
    cs_uint32 misc                 :  1 ; /* bits 1:1 */
    cs_uint32 init_done            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 init_done            :  1 ; /* bits 0:0 */
    cs_uint32 misc                 :  1 ; /* bits 1:1 */
    cs_uint32 mib_status           : 14 ; /* bits 15:2 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MISC_t;





#define ONU_MAC_MIB_RESET                        0x080a3
#define ONU_MAC_MIB_RESET_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 clear                :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 mpcp                 :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mpcp                 :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 clear                :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MIB_RESET_t;





#define ONU_MAC_MIB_ACCESS                       0x080a4
#define ONU_MAC_MIB_ACCESS_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 op_code              :  2 ; /* bits 29:28 */
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 counter_id           : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 counter_id           : 10 ; /* bits 9:0 */
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 op_code              :  2 ; /* bits 29:28 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MIB_ACCESS_t;

#define     counter_id_PON_BYTECNT                        0x0
#define     counter_id_PON_TXBYTECNT                      0x1
#define     counter_id_UCFRAMECNT                         0x2
#define     counter_id_TXUCFRAMECNT                       0x3
#define     counter_id_PON_CRCERRCNT                      0x4
#define     counter_id_PON_MCFRAMECNT                     0x5
#define     counter_id_PON_BCFRAMECNT                     0x6
#define     counter_id_PON_TXMCFRAMECNT                   0x7
#define     counter_id_PON_TXBCFRAMECNT                   0x8
#define     counter_id_PON_CTRLFRAMECNT                   0x9
#define     counter_id_PON_TXCTRLFRAMECNT                 0xa
#define     counter_id_PON_PAUSEFRAMECNT                  0xb
#define     counter_id_PON_UNKNOWNOPCNT                   0xc
#define     counter_id_PON_RUNTCNT                        0xd
#define     counter_id_PON_OVERSIZECNT                    0xe
#define     counter_id_PON_RMON64CNT                      0xf
#define     counter_id_PON_RMON65_127CNT                  0x10
#define     counter_id_PON_RMON128_255CNT                 0x11
#define     counter_id_PON_RMON256_511CNT                 0x12
#define     counter_id_PON_RMON512_1023CNT                0x13
#define     counter_id_PON_RMON1024_1518CNT               0x14
#define     counter_id_PON_TXRMON64CNT                    0x15
#define     counter_id_PON_TXRMON65_127CNT                0x16
#define     counter_id_PON_TXRMON128_255CNT               0x17
#define     counter_id_PON_TXRMON256_511CNT               0x18
#define     counter_id_PON_TXRMON512_1023CNT              0x19
#define     counter_id_PON_TXRMON1024_1518CNT             0x1a
#define     counter_id_PON_MPCP_COUNTER_MPCP_UC_REV_CNT      0x1b
#define     counter_id_PON_MPCP_COUNTER_MPCP_UC_REG_ACK      0x1c
#define     counter_id_PON_MPCP_COUNTER_MPCP_UC_REG_NACK      0x1d
#define     counter_id_PON_MPCP_COUNTER_MPCP_UC_REG_DEREG      0x1e
#define     counter_id_PON_MPCP_COUNTER_MPCP_UC_REG_REREG      0x1f
#define     counter_id_PON_MPCP_COUNTER_MPCP_UC_REG_REG_REV      0x20
#define     counter_id_PON_MPCP_COUNTER_MPCP_UC_GAT_REV      0x21
#define     counter_id_PON_MPCP_COUNTER_MPCP_UC_GAT_NORM      0x22
#define     counter_id_PON_MPCP_COUNTER_MPCP_UC_GAT_FRPT      0x23
#define     counter_id_PON_MPCP_COUNTER_MPCP_UC_GAT_UDIS      0x24
#define     counter_id_PON_MPCP_COUNTER_MPCP_UC_GAT_BDIS      0x25
#define     counter_id_PON_MPCP_COUNTER_MPCP_MC_REV_CNT      0x26
#define     counter_id_PON_MPCP_COUNTER_MPCP_MC_REG_ACK      0x27
#define     counter_id_PON_MPCP_COUNTER_MPCP_MC_REG_NACK      0x28
#define     counter_id_PON_MPCP_COUNTER_MPCP_MC_REG_DEREG      0x29
#define     counter_id_PON_MPCP_COUNTER_MPCP_MC_REG_REREG      0x2a
#define     counter_id_PON_MPCP_COUNTER_MPCP_MC_REG_REG_REV      0x2b
#define     counter_id_PON_MPCP_COUNTER_MPCP_MC_GAT_REV      0x2c
#define     counter_id_PON_MPCP_COUNTER_MPCP_MC_GAT_NORM      0x2d
#define     counter_id_PON_MPCP_COUNTER_MPCP_MC_GAT_FRPT      0x2e
#define     counter_id_PON_MPCP_COUNTER_MPCP_MC_GAT_UDIS      0x2f
#define     counter_id_PON_MPCP_COUNTER_MPCP_MC_GAT_BDIS      0x30
#define     counter_id_PON_MPCP_COUNTER_MPCP_BC_REV_CNT      0x31
#define     counter_id_PON_MPCP_COUNTER_MPCP_BC_REG_ACK      0x32
#define     counter_id_PON_MPCP_COUNTER_MPCP_BC_REG_NACK      0x33
#define     counter_id_PON_MPCP_COUNTER_MPCP_BC_REG_DEREG      0x34
#define     counter_id_PON_MPCP_COUNTER_MPCP_BC_REG_REREG      0x35
#define     counter_id_PON_MPCP_COUNTER_MPCP_BC_REG_REG_REV      0x36
#define     counter_id_PON_MPCP_COUNTER_MPCP_BC_GAT_REV      0x37
#define     counter_id_PON_MPCP_COUNTER_MPCP_BC_GAT_NORM      0x38
#define     counter_id_PON_MPCP_COUNTER_MPCP_BC_GAT_FRPT      0x39
#define     counter_id_PON_MPCP_COUNTER_MPCP_BC_GAT_UDIS      0x3a
#define     counter_id_PON_MPCP_COUNTER_MPCP_BC_GAT_BDIS      0x3b
#define     counter_id_PON_MPCP_COUNTER_MPCP_TX_REQ_DEREG      0x3c
#define     counter_id_PON_MPCP_COUNTER_MPCP_TX_REQ_REG      0x3d
#define     counter_id_PON_MPCP_COUNTER_MPCP_TX_ACK_ACK      0x3e
#define     counter_id_PON_MPCP_COUNTER_MPCP_TX_ACK_NACK      0x3f
#define     counter_id_PON_MPCP_COUNTER_MPCP_TX_RPT       0x40
#define     counter_id_PON_MPCP_COUNTER_MPCP_TX_OAM       0x41
#define     counter_id_PON_MPCP_COUNTER_MPCP_DA_MC_NSLF      0x42
#define     counter_id_PON_MPCP_COUNTER_MPCP_DA_MC_SLF      0x43
#define     counter_id_PON_MPCP_COUNTER_MPCP_DA_UC        0x44
#define     counter_id_PON_MPCP_COUNTER_MPCP_DA_BC        0x45
#define     counter_id_PON_FRAMECNT                       0x46
#define     counter_id_PON_TXOVERSIZECNT                  0x47
#define     counter_id_PON_RX1519_MAXCNT                  0x48
#define     counter_id_PON_TX1519_MAXCNT                  0x49
#define     counter_id_PON_TXFRAMECNT                     0x4a
#define     counter_id_TXPAUSECNT                         0x4b
#define     counter_id_UNDERSIZECNT                       0x4c
#define     counter_id_JABBERCNT                          0x4d
#define     counter_id_EXTENSIONCNT                       0x4e
#define     counter_id_TXEXTENSIONCNT                     0x4f




#define ONU_MAC_MIB_DATA1                        0x080a5
#define ONU_MAC_MIB_DATA1_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt_val_hi           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt_val_hi           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MIB_DATA1_t;





#define ONU_MAC_MIB_DATA0                        0x080a6
#define ONU_MAC_MIB_DATA0_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt_val_lo           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt_val_lo           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MIB_DATA0_t;





#define ONU_MAC_REFCLK_FCW                       0x080a7
#define ONU_MAC_REFCLK_FCW_dft                   0x00020000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 refclk_oe            :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 fcw                  : 21 ; /* bits 20:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 fcw                  : 21 ; /* bits 20:0 */
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 refclk_oe            :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_REFCLK_FCW_t;





#define ONU_MAC_REFCLK_FCW_MAX                   0x080a8
#define ONU_MAC_REFCLK_FCW_MAX_dft               0x000f4240




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 11 ;
    cs_uint32 fcw_max              : 21 ; /* bits 20:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 fcw_max              : 21 ; /* bits 20:0 */
    cs_uint32 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_REFCLK_FCW_MAX_t;





#define ONU_MAC_FCW_VALUE                        0x080a9
#define ONU_MAC_FCW_VALUE_dft                    0x000003e8




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 fcw_value            : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 fcw_value            : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_FCW_VALUE_t;





#define ONU_MAC_PHASE_ADJUST_VALUE1              0x080aa
#define ONU_MAC_PHASE_ADJUST_VALUE1_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 phase_adjust_value   : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 phase_adjust_value   : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PHASE_ADJUST_VALUE1_t;





#define ONU_MAC_PHASE_ADJUST_VALUE0              0x080ab
#define ONU_MAC_PHASE_ADJUST_VALUE0_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 phase_adjust_value   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 phase_adjust_value   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PHASE_ADJUST_VALUE0_t;





#define ONU_MAC_PULSE_WIDTH                      0x080ac
#define ONU_MAC_PULSE_WIDTH_dft                  0x0000001f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 clk_sel              :  1 ; /* bits 31:31 */
    cs_uint32 pulse_source_sel     :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 pulse_width          : 28 ; /* bits 27:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pulse_width          : 28 ; /* bits 27:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 pulse_source_sel     :  1 ; /* bits 30:30 */
    cs_uint32 clk_sel              :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PULSE_WIDTH_t;





#define ONU_MAC_PHASE_ADJUST_ENABLE              0x080ad
#define ONU_MAC_PHASE_ADJUST_ENABLE_dft          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 phase_adjust_trigger :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 phase_adjust_trigger :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PHASE_ADJUST_ENABLE_t;





#define ONU_MAC_CAPTURE_FCW                      0x080ae
#define ONU_MAC_CAPTURE_FCW_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 fcw_value            : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 fcw_value            : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CAPTURE_FCW_t;





#define ONU_MAC_CAPTURE_LOCALTIME1               0x080af
#define ONU_MAC_CAPTURE_LOCALTIME1_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 localtime            : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 localtime            : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CAPTURE_LOCALTIME1_t;





#define ONU_MAC_CAPTURE_LOCALTIME0               0x080b0
#define ONU_MAC_CAPTURE_LOCALTIME0_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 localtime            : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 localtime            : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_CAPTURE_LOCALTIME0_t;





#define ONU_MAC_TOD_S_PPS                        0x080b1
#define ONU_MAC_TOD_S_PPS_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 s_pps                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 s_pps                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_TOD_S_PPS_t;





#define ONU_MAC_TOD_PPS_WID                      0x080b2
#define ONU_MAC_TOD_PPS_WID_dft                  0x0000000f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 pps_width            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pps_width            : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_TOD_PPS_WID_t;





#define ONU_MAC_TOD_S_TOD                        0x080b3
#define ONU_MAC_TOD_S_TOD_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 s_tod                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 s_tod                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_TOD_S_TOD_t;





#define ONU_MAC_DBG_WIRE                         0x080b4
#define ONU_MAC_DBG_WIRE_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 sel1                 :  8 ; /* bits 15:8 */
    cs_uint32 sel0                 :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sel0                 :  8 ; /* bits 7:0 */
    cs_uint32 sel1                 :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_DBG_WIRE_t;





#define ONU_MAC_HW_DEBUG_OUT                     0x080b5
#define ONU_MAC_HW_DEBUG_OUT_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 sel                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sel                  : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_HW_DEBUG_OUT_t;





#define ONU_MAC_RXMAC_DEBUG                      0x080b6
#define ONU_MAC_RXMAC_DEBUG_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 cnt                  : 24 ; /* bits 23:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 24 ; /* bits 23:0 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RXMAC_DEBUG_t;





#define ONU_MAC_INPCS_CNT                        0x080b7
#define ONU_MAC_INPCS_CNT_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 is                   : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 is                   : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_INPCS_CNT_t;





#define ONU_MAC_OUTPCS_CNT                       0x080b8
#define ONU_MAC_OUTPCS_CNT_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 is                   : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 is                   : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_OUTPCS_CNT_t;





#define ONU_MAC_OUTMAC_CNT                       0x080b9
#define ONU_MAC_OUTMAC_CNT_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 is                   : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 is                   : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_OUTMAC_CNT_t;





#define ONU_MAC_OUTLSR_CNT                       0x080ba
#define ONU_MAC_OUTLSR_CNT_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 is                   : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 is                   : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_OUTLSR_CNT_t;





#define ONU_MAC_REVISION                         0x080bb
#define ONU_MAC_REVISION_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 year                 :  8 ; /* bits 31:24 */
    cs_uint32 month                :  8 ; /* bits 23:16 */
    cs_uint32 day                  :  8 ; /* bits 15:8 */
    cs_uint32 revision             :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 revision             :  8 ; /* bits 7:0 */
    cs_uint32 day                  :  8 ; /* bits 15:8 */
    cs_uint32 month                :  8 ; /* bits 23:16 */
    cs_uint32 year                 :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_REVISION_t;





#define ONU_MAC_DATAPATH_BYPASS                  0x080bc
#define ONU_MAC_DATAPATH_BYPASS_dft              0x00000022




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 20 ;
    cs_uint32 fec_tx_delay_en      :  1 ; /* bits 11:11 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 rxpath_sw_mode       :  1 ; /* bits 9:9 */
    cs_uint32 txpath_bypass        :  5 ; /* bits 8:4 */
    cs_uint32 rxpath_bypass        :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rxpath_bypass        :  4 ; /* bits 3:0 */
    cs_uint32 txpath_bypass        :  5 ; /* bits 8:4 */
    cs_uint32 rxpath_sw_mode       :  1 ; /* bits 9:9 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 fec_tx_delay_en      :  1 ; /* bits 11:11 */
    cs_uint32 rsrvd2               : 20 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_DATAPATH_BYPASS_t;





#define ONU_MAC_DATAPATH_STATUS                  0x080bd
#define ONU_MAC_DATAPATH_STATUS_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 23 ;
    cs_uint32 txpath1              :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 rxpath1              :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rxpath1              :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 txpath1              :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd2               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_DATAPATH_STATUS_t;





#define ONU_MAC_PON_PORT_CLEAR                   0x080be
#define ONU_MAC_PON_PORT_CLEAR_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 clear                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 clear                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PON_PORT_CLEAR_t;





#define ONU_MAC_SIM_CTRL                         0x080bf
#define ONU_MAC_SIM_CTRL_dft                     0x00000001




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 false_rpt_value      :  1 ; /* bits 1:1 */
    cs_uint32 fast_1ms_clk_sel     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 fast_1ms_clk_sel     :  1 ; /* bits 0:0 */
    cs_uint32 false_rpt_value      :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_SIM_CTRL_t;





#define ONU_MAC_GRANT_MONITOR                    0x080c0
#define ONU_MAC_GRANT_MONITOR_dft                0x0000c350




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 en                   :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 timer                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 timer                : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 en                   :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_GRANT_MONITOR_t;





#define ONU_MAC_GRANT_MONITOR_STATUS             0x080c1
#define ONU_MAC_GRANT_MONITOR_STATUS_dft         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 flag                 :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 flag                 :  4 ; /* bits 3:0 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_GRANT_MONITOR_STATUS_t;





#define ONU_MAC_BM_PON_TX_STATUS                 0x080c2
#define ONU_MAC_BM_PON_TX_STATUS_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 flag                 :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 flag                 :  4 ; /* bits 3:0 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_BM_PON_TX_STATUS_t;





#define ONU_MAC_INTERRUPT_SOURCE2                0x080c3
#define ONU_MAC_INTERRUPT_SOURCE2_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 irq2_src_31          :  1 ; /* bits 31:31 */
    cs_uint32 irq2_src_30          :  1 ; /* bits 30:30 */
    cs_uint32 irq2_src_29          :  1 ; /* bits 29:29 */
    cs_uint32 irq2_src_28          :  1 ; /* bits 28:28 */
    cs_uint32 irq2_src_27          :  1 ; /* bits 27:27 */
    cs_uint32 irq2_src_26          :  1 ; /* bits 26:26 */
    cs_uint32 irq2_src_25          :  1 ; /* bits 25:25 */
    cs_uint32 irq2_src_24          :  1 ; /* bits 24:24 */
    cs_uint32 irq2_src_23          :  1 ; /* bits 23:23 */
    cs_uint32 irq2_src_22          :  1 ; /* bits 22:22 */
    cs_uint32 irq2_src_21          :  1 ; /* bits 21:21 */
    cs_uint32 irq2_src_20          :  1 ; /* bits 20:20 */
    cs_uint32 irq2_src_19          :  1 ; /* bits 19:19 */
    cs_uint32 irq2_src_18          :  1 ; /* bits 18:18 */
    cs_uint32 irq2_src_17          :  1 ; /* bits 17:17 */
    cs_uint32 irq2_src_16          :  1 ; /* bits 16:16 */
    cs_uint32 irq2_src_15          :  1 ; /* bits 15:15 */
    cs_uint32 irq2_src_14          :  1 ; /* bits 14:14 */
    cs_uint32 irq2_src_13          :  1 ; /* bits 13:13 */
    cs_uint32 irq2_src_12          :  1 ; /* bits 12:12 */
    cs_uint32 irq2_src_11          :  1 ; /* bits 11:11 */
    cs_uint32 irq2_src_10          :  1 ; /* bits 10:10 */
    cs_uint32 irq2_src_09          :  1 ; /* bits 9:9 */
    cs_uint32 irq2_src_08          :  1 ; /* bits 8:8 */
    cs_uint32 irq2_src_07          :  1 ; /* bits 7:7 */
    cs_uint32 irq2_src_06          :  1 ; /* bits 6:6 */
    cs_uint32 irq2_src_05          :  1 ; /* bits 5:5 */
    cs_uint32 irq2_src_04          :  1 ; /* bits 4:4 */
    cs_uint32 irq2_src_03          :  1 ; /* bits 3:3 */
    cs_uint32 irq2_src_02          :  1 ; /* bits 2:2 */
    cs_uint32 irq2_src_01          :  1 ; /* bits 1:1 */
    cs_uint32 irq2_src_00          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 irq2_src_00          :  1 ; /* bits 0:0 */
    cs_uint32 irq2_src_01          :  1 ; /* bits 1:1 */
    cs_uint32 irq2_src_02          :  1 ; /* bits 2:2 */
    cs_uint32 irq2_src_03          :  1 ; /* bits 3:3 */
    cs_uint32 irq2_src_04          :  1 ; /* bits 4:4 */
    cs_uint32 irq2_src_05          :  1 ; /* bits 5:5 */
    cs_uint32 irq2_src_06          :  1 ; /* bits 6:6 */
    cs_uint32 irq2_src_07          :  1 ; /* bits 7:7 */
    cs_uint32 irq2_src_08          :  1 ; /* bits 8:8 */
    cs_uint32 irq2_src_09          :  1 ; /* bits 9:9 */
    cs_uint32 irq2_src_10          :  1 ; /* bits 10:10 */
    cs_uint32 irq2_src_11          :  1 ; /* bits 11:11 */
    cs_uint32 irq2_src_12          :  1 ; /* bits 12:12 */
    cs_uint32 irq2_src_13          :  1 ; /* bits 13:13 */
    cs_uint32 irq2_src_14          :  1 ; /* bits 14:14 */
    cs_uint32 irq2_src_15          :  1 ; /* bits 15:15 */
    cs_uint32 irq2_src_16          :  1 ; /* bits 16:16 */
    cs_uint32 irq2_src_17          :  1 ; /* bits 17:17 */
    cs_uint32 irq2_src_18          :  1 ; /* bits 18:18 */
    cs_uint32 irq2_src_19          :  1 ; /* bits 19:19 */
    cs_uint32 irq2_src_20          :  1 ; /* bits 20:20 */
    cs_uint32 irq2_src_21          :  1 ; /* bits 21:21 */
    cs_uint32 irq2_src_22          :  1 ; /* bits 22:22 */
    cs_uint32 irq2_src_23          :  1 ; /* bits 23:23 */
    cs_uint32 irq2_src_24          :  1 ; /* bits 24:24 */
    cs_uint32 irq2_src_25          :  1 ; /* bits 25:25 */
    cs_uint32 irq2_src_26          :  1 ; /* bits 26:26 */
    cs_uint32 irq2_src_27          :  1 ; /* bits 27:27 */
    cs_uint32 irq2_src_28          :  1 ; /* bits 28:28 */
    cs_uint32 irq2_src_29          :  1 ; /* bits 29:29 */
    cs_uint32 irq2_src_30          :  1 ; /* bits 30:30 */
    cs_uint32 irq2_src_31          :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_INTERRUPT_SOURCE2_t;





#define ONU_MAC_INTERRUPT_ENABLE2                0x080c4
#define ONU_MAC_INTERRUPT_ENABLE2_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 irq2_en_31           :  1 ; /* bits 31:31 */
    cs_uint32 irq2_en_30           :  1 ; /* bits 30:30 */
    cs_uint32 irq2_en_29           :  1 ; /* bits 29:29 */
    cs_uint32 irq2_en_28           :  1 ; /* bits 28:28 */
    cs_uint32 irq2_en_27           :  1 ; /* bits 27:27 */
    cs_uint32 irq2_en_26           :  1 ; /* bits 26:26 */
    cs_uint32 irq2_en_25           :  1 ; /* bits 25:25 */
    cs_uint32 irq2_en_24           :  1 ; /* bits 24:24 */
    cs_uint32 irq2_en_23           :  1 ; /* bits 23:23 */
    cs_uint32 irq2_en_22           :  1 ; /* bits 22:22 */
    cs_uint32 irq2_en_21           :  1 ; /* bits 21:21 */
    cs_uint32 irq2_en_20           :  1 ; /* bits 20:20 */
    cs_uint32 irq2_en_19           :  1 ; /* bits 19:19 */
    cs_uint32 irq2_en_18           :  1 ; /* bits 18:18 */
    cs_uint32 irq2_en_17           :  1 ; /* bits 17:17 */
    cs_uint32 irq2_en_16           :  1 ; /* bits 16:16 */
    cs_uint32 irq2_en_15           :  1 ; /* bits 15:15 */
    cs_uint32 irq2_en_14           :  1 ; /* bits 14:14 */
    cs_uint32 irq2_en_13           :  1 ; /* bits 13:13 */
    cs_uint32 irq2_en_12           :  1 ; /* bits 12:12 */
    cs_uint32 irq2_en_11           :  1 ; /* bits 11:11 */
    cs_uint32 irq2_en_10           :  1 ; /* bits 10:10 */
    cs_uint32 irq2_en_09           :  1 ; /* bits 9:9 */
    cs_uint32 irq2_en_08           :  1 ; /* bits 8:8 */
    cs_uint32 irq2_en_07           :  1 ; /* bits 7:7 */
    cs_uint32 irq2_en_06           :  1 ; /* bits 6:6 */
    cs_uint32 irq2_en_05           :  1 ; /* bits 5:5 */
    cs_uint32 irq2_en_04           :  1 ; /* bits 4:4 */
    cs_uint32 irq2_en_03           :  1 ; /* bits 3:3 */
    cs_uint32 irq2_en_02           :  1 ; /* bits 2:2 */
    cs_uint32 irq2_en_01           :  1 ; /* bits 1:1 */
    cs_uint32 irq2_en_00           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 irq2_en_00           :  1 ; /* bits 0:0 */
    cs_uint32 irq2_en_01           :  1 ; /* bits 1:1 */
    cs_uint32 irq2_en_02           :  1 ; /* bits 2:2 */
    cs_uint32 irq2_en_03           :  1 ; /* bits 3:3 */
    cs_uint32 irq2_en_04           :  1 ; /* bits 4:4 */
    cs_uint32 irq2_en_05           :  1 ; /* bits 5:5 */
    cs_uint32 irq2_en_06           :  1 ; /* bits 6:6 */
    cs_uint32 irq2_en_07           :  1 ; /* bits 7:7 */
    cs_uint32 irq2_en_08           :  1 ; /* bits 8:8 */
    cs_uint32 irq2_en_09           :  1 ; /* bits 9:9 */
    cs_uint32 irq2_en_10           :  1 ; /* bits 10:10 */
    cs_uint32 irq2_en_11           :  1 ; /* bits 11:11 */
    cs_uint32 irq2_en_12           :  1 ; /* bits 12:12 */
    cs_uint32 irq2_en_13           :  1 ; /* bits 13:13 */
    cs_uint32 irq2_en_14           :  1 ; /* bits 14:14 */
    cs_uint32 irq2_en_15           :  1 ; /* bits 15:15 */
    cs_uint32 irq2_en_16           :  1 ; /* bits 16:16 */
    cs_uint32 irq2_en_17           :  1 ; /* bits 17:17 */
    cs_uint32 irq2_en_18           :  1 ; /* bits 18:18 */
    cs_uint32 irq2_en_19           :  1 ; /* bits 19:19 */
    cs_uint32 irq2_en_20           :  1 ; /* bits 20:20 */
    cs_uint32 irq2_en_21           :  1 ; /* bits 21:21 */
    cs_uint32 irq2_en_22           :  1 ; /* bits 22:22 */
    cs_uint32 irq2_en_23           :  1 ; /* bits 23:23 */
    cs_uint32 irq2_en_24           :  1 ; /* bits 24:24 */
    cs_uint32 irq2_en_25           :  1 ; /* bits 25:25 */
    cs_uint32 irq2_en_26           :  1 ; /* bits 26:26 */
    cs_uint32 irq2_en_27           :  1 ; /* bits 27:27 */
    cs_uint32 irq2_en_28           :  1 ; /* bits 28:28 */
    cs_uint32 irq2_en_29           :  1 ; /* bits 29:29 */
    cs_uint32 irq2_en_30           :  1 ; /* bits 30:30 */
    cs_uint32 irq2_en_31           :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_INTERRUPT_ENABLE2_t;





#define ONU_MAC_INTERRUPT_ISAS2                  0x080c5
#define ONU_MAC_INTERRUPT_ISAS2_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 irq2_src_31Z         :  1 ; /* bits 31:31 */
    cs_uint32 irq2_src_30Z         :  1 ; /* bits 30:30 */
    cs_uint32 irq2_src_29Z         :  1 ; /* bits 29:29 */
    cs_uint32 irq2_src_28Z         :  1 ; /* bits 28:28 */
    cs_uint32 irq2_src_27Z         :  1 ; /* bits 27:27 */
    cs_uint32 irq2_src_26Z         :  1 ; /* bits 26:26 */
    cs_uint32 irq2_src_25Z         :  1 ; /* bits 25:25 */
    cs_uint32 irq2_src_24Z         :  1 ; /* bits 24:24 */
    cs_uint32 irq2_src_23Z         :  1 ; /* bits 23:23 */
    cs_uint32 irq2_src_22Z         :  1 ; /* bits 22:22 */
    cs_uint32 irq2_src_21Z         :  1 ; /* bits 21:21 */
    cs_uint32 irq2_src_20Z         :  1 ; /* bits 20:20 */
    cs_uint32 irq2_src_19Z         :  1 ; /* bits 19:19 */
    cs_uint32 irq2_src_18Z         :  1 ; /* bits 18:18 */
    cs_uint32 irq2_src_17Z         :  1 ; /* bits 17:17 */
    cs_uint32 irq2_src_16Z         :  1 ; /* bits 16:16 */
    cs_uint32 irq2_src_15Z         :  1 ; /* bits 15:15 */
    cs_uint32 irq2_src_14Z         :  1 ; /* bits 14:14 */
    cs_uint32 irq2_src_13Z         :  1 ; /* bits 13:13 */
    cs_uint32 irq2_src_12Z         :  1 ; /* bits 12:12 */
    cs_uint32 irq2_src_11Z         :  1 ; /* bits 11:11 */
    cs_uint32 irq2_src_10Z         :  1 ; /* bits 10:10 */
    cs_uint32 irq2_src_09Z         :  1 ; /* bits 9:9 */
    cs_uint32 irq2_src_08Z         :  1 ; /* bits 8:8 */
    cs_uint32 irq2_src_07Z         :  1 ; /* bits 7:7 */
    cs_uint32 irq2_src_06Z         :  1 ; /* bits 6:6 */
    cs_uint32 irq2_src_05Z         :  1 ; /* bits 5:5 */
    cs_uint32 irq2_src_04Z         :  1 ; /* bits 4:4 */
    cs_uint32 irq2_src_03Z         :  1 ; /* bits 3:3 */
    cs_uint32 irq2_src_02Z         :  1 ; /* bits 2:2 */
    cs_uint32 irq2_src_01Z         :  1 ; /* bits 1:1 */
    cs_uint32 irq2_src_00Z         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 irq2_src_00Z         :  1 ; /* bits 0:0 */
    cs_uint32 irq2_src_01Z         :  1 ; /* bits 1:1 */
    cs_uint32 irq2_src_02Z         :  1 ; /* bits 2:2 */
    cs_uint32 irq2_src_03Z         :  1 ; /* bits 3:3 */
    cs_uint32 irq2_src_04Z         :  1 ; /* bits 4:4 */
    cs_uint32 irq2_src_05Z         :  1 ; /* bits 5:5 */
    cs_uint32 irq2_src_06Z         :  1 ; /* bits 6:6 */
    cs_uint32 irq2_src_07Z         :  1 ; /* bits 7:7 */
    cs_uint32 irq2_src_08Z         :  1 ; /* bits 8:8 */
    cs_uint32 irq2_src_09Z         :  1 ; /* bits 9:9 */
    cs_uint32 irq2_src_10Z         :  1 ; /* bits 10:10 */
    cs_uint32 irq2_src_11Z         :  1 ; /* bits 11:11 */
    cs_uint32 irq2_src_12Z         :  1 ; /* bits 12:12 */
    cs_uint32 irq2_src_13Z         :  1 ; /* bits 13:13 */
    cs_uint32 irq2_src_14Z         :  1 ; /* bits 14:14 */
    cs_uint32 irq2_src_15Z         :  1 ; /* bits 15:15 */
    cs_uint32 irq2_src_16Z         :  1 ; /* bits 16:16 */
    cs_uint32 irq2_src_17Z         :  1 ; /* bits 17:17 */
    cs_uint32 irq2_src_18Z         :  1 ; /* bits 18:18 */
    cs_uint32 irq2_src_19Z         :  1 ; /* bits 19:19 */
    cs_uint32 irq2_src_20Z         :  1 ; /* bits 20:20 */
    cs_uint32 irq2_src_21Z         :  1 ; /* bits 21:21 */
    cs_uint32 irq2_src_22Z         :  1 ; /* bits 22:22 */
    cs_uint32 irq2_src_23Z         :  1 ; /* bits 23:23 */
    cs_uint32 irq2_src_24Z         :  1 ; /* bits 24:24 */
    cs_uint32 irq2_src_25Z         :  1 ; /* bits 25:25 */
    cs_uint32 irq2_src_26Z         :  1 ; /* bits 26:26 */
    cs_uint32 irq2_src_27Z         :  1 ; /* bits 27:27 */
    cs_uint32 irq2_src_28Z         :  1 ; /* bits 28:28 */
    cs_uint32 irq2_src_29Z         :  1 ; /* bits 29:29 */
    cs_uint32 irq2_src_30Z         :  1 ; /* bits 30:30 */
    cs_uint32 irq2_src_31Z         :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_INTERRUPT_ISAS2_t;





#define ONU_MAC_MPCP_RTT_COMPENSATION_MODE       0x080c6
#define ONU_MAC_MPCP_RTT_COMPENSATION_MODE_dft   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 mode                 :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mode                 :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_RTT_COMPENSATION_MODE_t;





#define ONU_MAC_MPCP_RTT_COMPENSATION            0x080c7
#define ONU_MAC_MPCP_RTT_COMPENSATION_dft        0x1e1e001e




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ts_compensation      :  8 ; /* bits 31:24 */
    cs_uint32 output_compensation  :  8 ; /* bits 23:16 */
    cs_uint32 input_compensation   : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 input_compensation   : 16 ; /* bits 15:0 */
    cs_uint32 output_compensation  :  8 ; /* bits 23:16 */
    cs_uint32 ts_compensation      :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_RTT_COMPENSATION_t;





#define ONU_MAC_MPCP_RTT_COMPENSATION2           0x080c8
#define ONU_MAC_MPCP_RTT_COMPENSATION2_dft       0x2b1e1e11




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 input_compensation2  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 input_compensation2  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_MPCP_RTT_COMPENSATION2_t;





#define ONU_MAC_AES_FEC_COMP                     0x080c9
#define ONU_MAC_AES_FEC_COMP_dft                 0x80000300




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 fec_comp_enable      :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 fec_comp_incdec      :  1 ; /* bits 16:16 */
    cs_uint32 fec_comp_cnt         : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 fec_comp_cnt         : 16 ; /* bits 15:0 */
    cs_uint32 fec_comp_incdec      :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 fec_comp_enable      :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_AES_FEC_COMP_t;





#define ONU_MAC_TX_POWER_CTRL_WAKEUP_TIME        0x080ca
#define ONU_MAC_TX_POWER_CTRL_WAKEUP_TIME_dft    0x00000020




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 wakeup_time          : 24 ; /* bits 23:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 wakeup_time          : 24 ; /* bits 23:0 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_TX_POWER_CTRL_WAKEUP_TIME_t;





#define ONU_MAC_TX_POWER_CTRL_SHUTDOWN_TIME      0x080cb
#define ONU_MAC_TX_POWER_CTRL_SHUTDOWN_TIME_dft  0x00000010




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 shutdown_time        : 24 ; /* bits 23:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 shutdown_time        : 24 ; /* bits 23:0 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_TX_POWER_CTRL_SHUTDOWN_TIME_t;





#define ONU_MAC_TX_POWER_CTRL_KEEP_TIME          0x080cc
#define ONU_MAC_TX_POWER_CTRL_KEEP_TIME_dft      0x000a0000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 keep_time            : 28 ; /* bits 27:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 keep_time            : 28 ; /* bits 27:0 */
    cs_uint32 rsrvd1               :  4 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_TX_POWER_CTRL_KEEP_TIME_t;





#define ONU_MAC_TX_POWER_CTRL_MISC               0x080cd
#define ONU_MAC_TX_POWER_CTRL_MISC_dft           0x00001e08




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 disable_report_grant :  1 ; /* bits 16:16 */
    cs_uint32 apply_queue_status   :  1 ; /* bits 15:15 */
    cs_uint32 sel_eee_pwr_ctrl     :  1 ; /* bits 14:14 */
    cs_uint32 path_dly_time        :  6 ; /* bits 13:8 */
    cs_uint32 off_adj_time         :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 off_adj_time         :  8 ; /* bits 7:0 */
    cs_uint32 path_dly_time        :  6 ; /* bits 13:8 */
    cs_uint32 sel_eee_pwr_ctrl     :  1 ; /* bits 14:14 */
    cs_uint32 apply_queue_status   :  1 ; /* bits 15:15 */
    cs_uint32 disable_report_grant :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_TX_POWER_CTRL_MISC_t;





#define ONU_MAC_PTP_CONFIG                       0x080ce
#define ONU_MAC_PTP_CONFIG_dft                   0x0400000f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 packet_dpid          :  3 ; /* bits 26:24 */
    cs_uint32 ptp_tx_crc_recal_en  :  1 ; /* bits 23:23 */
    cs_uint32 ptp_tx_en            :  1 ; /* bits 22:22 */
    cs_uint32 ptp_rx_en            :  1 ; /* bits 21:21 */
    cs_uint32 eg_lts_bswap         :  1 ; /* bits 20:20 */
    cs_uint32 ig_lts_bswap         :  1 ; /* bits 19:19 */
    cs_uint32 packet_cos           :  3 ; /* bits 18:16 */
    cs_uint32 process_en           : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 process_en           : 16 ; /* bits 15:0 */
    cs_uint32 packet_cos           :  3 ; /* bits 18:16 */
    cs_uint32 ig_lts_bswap         :  1 ; /* bits 19:19 */
    cs_uint32 eg_lts_bswap         :  1 ; /* bits 20:20 */
    cs_uint32 ptp_rx_en            :  1 ; /* bits 21:21 */
    cs_uint32 ptp_tx_en            :  1 ; /* bits 22:22 */
    cs_uint32 ptp_tx_crc_recal_en  :  1 ; /* bits 23:23 */
    cs_uint32 packet_dpid          :  3 ; /* bits 26:24 */
    cs_uint32 rsrvd1               :  5 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PTP_CONFIG_t;





#define ONU_MAC_PTP_ASYM_DELAY                   0x080cf
#define ONU_MAC_PTP_ASYM_DELAY_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 sign                 :  1 ; /* bits 15:15 */
    cs_uint32 correction           : 15 ; /* bits 14:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 correction           : 15 ; /* bits 14:0 */
    cs_uint32 sign                 :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PTP_ASYM_DELAY_t;





#define ONU_MAC_PTP_ING_LATCH                    0x080d0
#define ONU_MAC_PTP_ING_LATCH_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 local_time           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 local_time           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PTP_ING_LATCH_t;





#define ONU_MAC_PTP_PROCESS                      0x080d1
#define ONU_MAC_PTP_PROCESS_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 cf_offset            :  6 ; /* bits 9:4 */
    cs_uint32 message_type         :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 message_type         :  4 ; /* bits 3:0 */
    cs_uint32 cf_offset            :  6 ; /* bits 9:4 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PTP_PROCESS_t;





#define ONU_MAC_PTP_EG_LATCH                     0x080d2
#define ONU_MAC_PTP_EG_LATCH_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 local_time           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 local_time           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PTP_EG_LATCH_t;





#define ONU_MAC_PTP_ING_FWD                      0x080d3
#define ONU_MAC_PTP_ING_FWD_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 local_time           : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 local_time           : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PTP_ING_FWD_t;





#define ONU_MAC_PTP_EG_STS                       0x080d4
#define ONU_MAC_PTP_EG_STS_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 lt_valid             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 lt_valid             :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_PTP_EG_STS_t;





#define ONU_MAC_RX_MAC_CONFIG0                   0x080d5
#define ONU_MAC_RX_MAC_CONFIG0_dft               0x81000003




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tag_type0            : 16 ; /* bits 31:16 */
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 tag_exclude          :  1 ; /* bits 1:1 */
    cs_uint32 rx_pause_uc          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx_pause_uc          :  1 ; /* bits 0:0 */
    cs_uint32 tag_exclude          :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 14 ;
    cs_uint32 tag_type0            : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RX_MAC_CONFIG0_t;





#define ONU_MAC_RX_MAC_CONFIG1                   0x080d6
#define ONU_MAC_RX_MAC_CONFIG1_dft               0x88a89100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tag_type2            : 16 ; /* bits 31:16 */
    cs_uint32 tag_type1            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tag_type1            : 16 ; /* bits 15:0 */
    cs_uint32 tag_type2            : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RX_MAC_CONFIG1_t;





#define ONU_MAC_DBG_SEL                          0x080d7
#define ONU_MAC_DBG_SEL_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 16 ;
    cs_uint32 dbg_sel_fec          :  3 ; /* bits 15:13 */
    cs_uint32 dbg_sel_h            :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 dbg_sel_l            :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dbg_sel_l            :  5 ; /* bits 4:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 dbg_sel_h            :  5 ; /* bits 12:8 */
    cs_uint32 dbg_sel_fec          :  3 ; /* bits 15:13 */
    cs_uint32 rsrvd2               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_DBG_SEL_t;





#define ONU_MAC_RSV_CFG1                         0x080d8
#define ONU_MAC_RSV_CFG1_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsv_cfg1             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsv_cfg1             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RSV_CFG1_t;





#define ONU_MAC_RSV_CFG2                         0x080d9
#define ONU_MAC_RSV_CFG2_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsv_cfg2             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsv_cfg2             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RSV_CFG2_t;





#define ONU_MAC_RSV_STATUS1                      0x080da
#define ONU_MAC_RSV_STATUS1_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsv_status1          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsv_status1          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RSV_STATUS1_t;





#define ONU_MAC_RSV_STATUS2                      0x080db
#define ONU_MAC_RSV_STATUS2_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsv_status2          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsv_status2          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RSV_STATUS2_t;





#define ONU_MAC_RSV_CNT1                         0x080dc
#define ONU_MAC_RSV_CNT1_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsv_cnt1             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsv_cnt1             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RSV_CNT1_t;





#define ONU_MAC_RSV_CNT2                         0x080dd
#define ONU_MAC_RSV_CNT2_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsv_cnt2             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsv_cnt2             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} ONU_MAC_RSV_CNT2_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* epon */

typedef struct {
  ONU_MAC_INTERRUPT_SOURCE_t               INTERRUPT_SOURCE;              /* addr: 0x00000 */ 
  ONU_MAC_INTERRUPT_ENABLE_t               INTERRUPT_ENABLE;              /* addr: 0x00001 */ 
  ONU_MAC_INTERRUPT_ISAS_t                 INTERRUPT_ISAS;                /* addr: 0x00002 */ 
  ONU_MAC_OPTICAL_XCR_CTRL_t               OPTICAL_XCR_CTRL;              /* addr: 0x00003 */ 
  ONU_MAC_OPTICAL_XCR_STATUS_t             OPTICAL_XCR_STATUS;            /* addr: 0x00004 */ 
  ONU_MAC_SDS_SIG_LOSS_CNT_t               SDS_SIG_LOSS_CNT;              /* addr: 0x00005 */ 
  ONU_MAC_RSSI_t                           RSSI;                          /* addr: 0x00006 */ 
  ONU_MAC_ONU_LaserOn_LaserOff_Time_t      ONU_LaserOn_LaserOff_Time;     /* addr: 0x00007 */ 
  ONU_MAC_SERDES_LATENCY_t                 SERDES_LATENCY;                /* addr: 0x00008 */ 
  ONU_MAC_GLOBAL_GLB_SYS_CFG_t             GLOBAL_GLB_SYS_CFG;            /* addr: 0x00009 */ 
  ONU_MAC_GLOBAL_GLB_PCS_t                 GLOBAL_GLB_PCS;                /* addr: 0x0000a */ 
  ONU_MAC_AN_Ctrl_Reg_t                    AN_Ctrl_Reg;                   /* addr: 0x0000b */ 
  ONU_MAC_PCS_Error_Symbol_Counter_t       PCS_Error_Symbol_Counter;      /* addr: 0x0000c */ 
  ONU_MAC_PCS_OutofSync_Cnt_t              PCS_OutofSync_Cnt;             /* addr: 0x0000d */ 
  ONU_MAC_PCS_Status_Reg_t                 PCS_Status_Reg;                /* addr: 0x0000e */ 
  ONU_MAC_AES_MODE_CTL_t                   AES_MODE_CTL;                  /* addr: 0x0000f */ 
  ONU_MAC_UC_KEY03_t                       UC_KEY03;                      /* addr: 0x00010 */ 
  ONU_MAC_UC_KEY02_t                       UC_KEY02;                      /* addr: 0x00011 */ 
  ONU_MAC_UC_KEY01_t                       UC_KEY01;                      /* addr: 0x00012 */ 
  ONU_MAC_UC_KEY00_t                       UC_KEY00;                      /* addr: 0x00013 */ 
  ONU_MAC_UC_KEY13_t                       UC_KEY13;                      /* addr: 0x00014 */ 
  ONU_MAC_UC_KEY12_t                       UC_KEY12;                      /* addr: 0x00015 */ 
  ONU_MAC_UC_KEY11_t                       UC_KEY11;                      /* addr: 0x00016 */ 
  ONU_MAC_UC_KEY10_t                       UC_KEY10;                      /* addr: 0x00017 */ 
  ONU_MAC_PKT_MON03_t                      PKT_MON03;                     /* addr: 0x00018 */ 
  ONU_MAC_PKT_MON02_t                      PKT_MON02;                     /* addr: 0x00019 */ 
  ONU_MAC_PKT_MON01_t                      PKT_MON01;                     /* addr: 0x0001a */ 
  ONU_MAC_PKT_MON00_t                      PKT_MON00;                     /* addr: 0x0001b */ 
  ONU_MAC_PKT_MON13_t                      PKT_MON13;                     /* addr: 0x0001c */ 
  ONU_MAC_PKT_MON12_t                      PKT_MON12;                     /* addr: 0x0001d */ 
  ONU_MAC_PKT_MON11_t                      PKT_MON11;                     /* addr: 0x0001e */ 
  ONU_MAC_PKT_MON10_t                      PKT_MON10;                     /* addr: 0x0001f */ 
  ONU_MAC_PKT_MON2_t                       PKT_MON2;                      /* addr: 0x00020 */ 
  ONU_MAC_PKT_MON31_t                      PKT_MON31;                     /* addr: 0x00021 */ 
  ONU_MAC_PKT_MON30_t                      PKT_MON30;                     /* addr: 0x00022 */ 
  ONU_MAC_KEYREQUEST_OAM_CNT_t             KEYREQUEST_OAM_CNT;            /* addr: 0x00023 */ 
  ONU_MAC_UC_DEC_DBG_CNT_t                 UC_DEC_DBG_CNT;                /* addr: 0x00024 */ 
  ONU_MAC_JAES_TIMER_t                     JAES_TIMER;                    /* addr: 0x00025 */ 
  ONU_MAC_AES_DBG_CNT0_t                   AES_DBG_CNT0;                  /* addr: 0x00026 */ 
  ONU_MAC_AES_DBG_GOOD_CNT0_t              AES_DBG_GOOD_CNT0;             /* addr: 0x00027 */ 
  ONU_MAC_AES_DBG_CNT1_t                   AES_DBG_CNT1;                  /* addr: 0x00028 */ 
  ONU_MAC_AES_DBG_GOOD_CNT1_t              AES_DBG_GOOD_CNT1;             /* addr: 0x00029 */ 
  ONU_MAC_UC_TX_KEY03_t                    UC_TX_KEY03;                   /* addr: 0x0002a */ 
  ONU_MAC_UC_TX_KEY02_t                    UC_TX_KEY02;                   /* addr: 0x0002b */ 
  ONU_MAC_UC_TX_KEY01_t                    UC_TX_KEY01;                   /* addr: 0x0002c */ 
  ONU_MAC_UC_TX_KEY00_t                    UC_TX_KEY00;                   /* addr: 0x0002d */ 
  ONU_MAC_UC_TX_KEY13_t                    UC_TX_KEY13;                   /* addr: 0x0002e */ 
  ONU_MAC_UC_TX_KEY12_t                    UC_TX_KEY12;                   /* addr: 0x0002f */ 
  ONU_MAC_UC_TX_KEY11_t                    UC_TX_KEY11;                   /* addr: 0x00030 */ 
  ONU_MAC_UC_TX_KEY10_t                    UC_TX_KEY10;                   /* addr: 0x00031 */ 
  ONU_MAC_UC_TX_KEY_CTRL_t                 UC_TX_KEY_CTRL;                /* addr: 0x00032 */ 
  ONU_MAC_FEC_CFG_t                        FEC_CFG;                       /* addr: 0x00033 */ 
  ONU_MAC_FEC_TX_ERRINJ_t                  FEC_TX_ERRINJ;                 /* addr: 0x00034 */ 
  ONU_MAC_SFEC_CNT_t                       SFEC_CNT;                      /* addr: 0x00035 */ 
  ONU_MAC_TFEC_CNT_t                       TFEC_CNT;                      /* addr: 0x00036 */ 
  ONU_MAC_SPKT_CNT_t                       SPKT_CNT;                      /* addr: 0x00037 */ 
  ONU_MAC_EPKT_CNT_t                       EPKT_CNT;                      /* addr: 0x00038 */ 
  ONU_MAC_FEC_DEFINE_PKTSIZE_t             FEC_DEFINE_PKTSIZE;            /* addr: 0x00039 */ 
  ONU_MAC_FEC_DEFINE_LATENCY_t             FEC_DEFINE_LATENCY;            /* addr: 0x0003a */ 
  ONU_MAC_FEC_CTRL_t                       FEC_CTRL;                      /* addr: 0x0003b */ 
  ONU_MAC_FEC_MISC_STATUS_t                FEC_MISC_STATUS;               /* addr: 0x0003c */ 
  ONU_MAC_FEC_Counter_Control_t            FEC_Counter_Control;           /* addr: 0x0003d */ 
  ONU_MAC_FEC_Block_Correctable_Error_Counter_t FEC_Block_Correctable_Error_Counter; /* addr: 0x0003e */ 
  ONU_MAC_FEC_Block_Uncorrectable_Error_Counter_t FEC_Block_Uncorrectable_Error_Counter; /* addr: 0x0003f */ 
  ONU_MAC_FEC_Block_No_Error_Counter_t     FEC_Block_No_Error_Counter;    /* addr: 0x00040 */ 
  ONU_MAC_FEC_Block_Counter_t              FEC_Block_Counter;             /* addr: 0x00041 */ 
  ONU_MAC_FEC_Corr_Byte_Counter_t          FEC_Corr_Byte_Counter;         /* addr: 0x00042 */ 
  ONU_MAC_LLID_FILTERING_CFG_t             LLID_FILTERING_CFG;            /* addr: 0x00043 */ 
  ONU_MAC_CS0_LLID_Filtering_Pattern_Mask_t CS0_LLID_Filtering_Pattern_Mask; /* addr: 0x00044 */ 
  ONU_MAC_CS1_LLID_Filtering_Pattern_Mask_t CS1_LLID_Filtering_Pattern_Mask; /* addr: 0x00045 */ 
  ONU_MAC_CS2_LLID_Filtering_Pattern_Mask_t CS2_LLID_Filtering_Pattern_Mask; /* addr: 0x00046 */ 
  ONU_MAC_CS3_LLID_Filtering_Pattern_Mask_t CS3_LLID_Filtering_Pattern_Mask; /* addr: 0x00047 */ 
  ONU_MAC_CS4_LLID_Filtering_Pattern_Mask_t CS4_LLID_Filtering_Pattern_Mask; /* addr: 0x00048 */ 
  ONU_MAC_CS5_LLID_Filtering_Pattern_Mask_t CS5_LLID_Filtering_Pattern_Mask; /* addr: 0x00049 */ 
  ONU_MAC_CS6_LLID_Filtering_Pattern_Mask_t CS6_LLID_Filtering_Pattern_Mask; /* addr: 0x0004a */ 
  ONU_MAC_CS7_LLID_Filtering_Pattern_Mask_t CS7_LLID_Filtering_Pattern_Mask; /* addr: 0x0004b */ 
  ONU_MAC_RX_MAXPKTSIZE_t                  RX_MAXPKTSIZE;                 /* addr: 0x0004c */ 
  ONU_MAC_MAC_DROP_CNT_t                   MAC_DROP_CNT;                  /* addr: 0x0004d */ 
  ONU_MAC_IPG_DROP_CNT_t                   IPG_DROP_CNT;                  /* addr: 0x0004e */ 
  ONU_MAC_CRC8_ERR_CNT_t                   CRC8_ERR_CNT;                  /* addr: 0x0004f */ 
  ONU_MAC_SLD_ERR_CNT_t                    SLD_ERR_CNT;                   /* addr: 0x00050 */ 
  ONU_MAC_RXCTRL_t                         RXCTRL;                        /* addr: 0x00051 */ 
  ONU_MAC_RXMAC_EXTENSION_CFG_t            RXMAC_EXTENSION_CFG;           /* addr: 0x00052 */ 
  ONU_MAC_RXMAC_EXTENSION_OUI_t            RXMAC_EXTENSION_OUI;           /* addr: 0x00053 */ 
  ONU_MAC_RXMAC_EXTENSION_DA1_t            RXMAC_EXTENSION_DA1;           /* addr: 0x00054 */ 
  ONU_MAC_RXMAC_EXTENSION_DA0_t            RXMAC_EXTENSION_DA0;           /* addr: 0x00055 */ 
  ONU_MAC_RXCTRL_BYPASS_CFG_t              RXCTRL_BYPASS_CFG;             /* addr: 0x00056 */ 
  ONU_MAC_RXMAC_CFG_t                      RXMAC_CFG;                     /* addr: 0x00057 */ 
  ONU_MAC_MPCP_ENABLE_CONTROL_t            MPCP_ENABLE_CONTROL;           /* addr: 0x00058 */ 
  ONU_MAC_MPCP_CONTROL_t                   MPCP_CONTROL;                  /* addr: 0x00059 */ 
  ONU_MAC_MPCP_CFG_t                       MPCP_CFG;                      /* addr: 0x0005a */ 
  ONU_MAC_MPCP_PKT_SIZE_CFG_t              MPCP_PKT_SIZE_CFG;             /* addr: 0x0005b */ 
  ONU_MAC_MPCP_TIMERS_CFG_t                MPCP_TIMERS_CFG;               /* addr: 0x0005c */ 
  ONU_MAC_MPCP_REPORT_TIMER_CFG_t          MPCP_REPORT_TIMER_CFG;         /* addr: 0x0005d */ 
  ONU_MAC_AES_CHURNING_CTRL_t              AES_CHURNING_CTRL;             /* addr: 0x0005e */ 
  ONU_MAC_MPCP_Configuration_t             MPCP_Configuration;            /* addr: 0x0005f */ 
  ONU_MAC_MPCP_LLID_SyncTime_t             MPCP_LLID_SyncTime;            /* addr: 0x00060 */ 
  ONU_MAC_MPCP_MISC_CTRL_t                 MPCP_MISC_CTRL;                /* addr: 0x00061 */ 
  ONU_MAC_MPCP_GRANT_PROCESS_TIME_t        MPCP_GRANT_PROCESS_TIME;       /* addr: 0x00062 */ 
  ONU_MAC_MPCP_STATUS_t                    MPCP_STATUS;                   /* addr: 0x00063 */ 
  ONU_MAC_MPCP_REGISTER_PARAMETERS1_t      MPCP_REGISTER_PARAMETERS1;     /* addr: 0x00064 */ 
  ONU_MAC_MPCP_REGISTER_PARAMETERS2_t      MPCP_REGISTER_PARAMETERS2;     /* addr: 0x00065 */ 
  ONU_MAC_MPCP_OLTMAC1_t                   MPCP_OLTMAC1;                  /* addr: 0x00066 */ 
  ONU_MAC_MPCP_OLTMAC0_t                   MPCP_OLTMAC0;                  /* addr: 0x00067 */ 
  ONU_MAC_MPCP_RPT_SCAN_CFG_t              MPCP_RPT_SCAN_CFG;             /* addr: 0x00068 */ 
  ONU_MAC_MPCP_MONITOR_CNT_t               MPCP_MONITOR_CNT;              /* addr: 0x00069 */ 
  ONU_MAC_MPCP_REG_REQ_RESV0_t             MPCP_REG_REQ_RESV0;            /* addr: 0x0006a */ 
  ONU_MAC_MPCP_REG_REQ_RESV1_t             MPCP_REG_REQ_RESV1;            /* addr: 0x0006b */ 
  ONU_MAC_MPCP_REG_REQ_RESV2_t             MPCP_REG_REQ_RESV2;            /* addr: 0x0006c */ 
  ONU_MAC_MPCP_REG_REQ_RESV3_t             MPCP_REG_REQ_RESV3;            /* addr: 0x0006d */ 
  ONU_MAC_MPCP_REG_REQ_RESV4_t             MPCP_REG_REQ_RESV4;            /* addr: 0x0006e */ 
  ONU_MAC_MPCP_REG_REQ_RESV5_t             MPCP_REG_REQ_RESV5;            /* addr: 0x0006f */ 
  ONU_MAC_MPCP_REG_REQ_RESV6_t             MPCP_REG_REQ_RESV6;            /* addr: 0x00070 */ 
  ONU_MAC_MPCP_REG_REQ_RESV7_t             MPCP_REG_REQ_RESV7;            /* addr: 0x00071 */ 
  ONU_MAC_MPCP_REG_REQ_RESV8_t             MPCP_REG_REQ_RESV8;            /* addr: 0x00072 */ 
  ONU_MAC_MPCP_REG_REQ_RESV9_t             MPCP_REG_REQ_RESV9;            /* addr: 0x00073 */ 
  ONU_MAC_MPCP_ACK_RESV0_t                 MPCP_ACK_RESV0;                /* addr: 0x00074 */ 
  ONU_MAC_MPCP_ACK_RESV1_t                 MPCP_ACK_RESV1;                /* addr: 0x00075 */ 
  ONU_MAC_MPCP_ACK_RESV2_t                 MPCP_ACK_RESV2;                /* addr: 0x00076 */ 
  ONU_MAC_MPCP_ACK_RESV3_t                 MPCP_ACK_RESV3;                /* addr: 0x00077 */ 
  ONU_MAC_MPCP_ACK_RESV4_t                 MPCP_ACK_RESV4;                /* addr: 0x00078 */ 
  ONU_MAC_MPCP_ACK_RESV5_t                 MPCP_ACK_RESV5;                /* addr: 0x00079 */ 
  ONU_MAC_MPCP_ACK_RESV6_t                 MPCP_ACK_RESV6;                /* addr: 0x0007a */ 
  ONU_MAC_MPCP_ACK_RESV7_t                 MPCP_ACK_RESV7;                /* addr: 0x0007b */ 
  ONU_MAC_MPCP_ACK_RESV8_t                 MPCP_ACK_RESV8;                /* addr: 0x0007c */ 
  ONU_MAC_Debug_MPCP_Gate_Counter1_t       Debug_MPCP_Gate_Counter1;      /* addr: 0x0007d */ 
  ONU_MAC_Debug_MPCP_Gate_Counter2_t       Debug_MPCP_Gate_Counter2;      /* addr: 0x0007e */ 
  ONU_MAC_Debug_MPCP_Register_Counter_t    Debug_MPCP_Register_Counter;   /* addr: 0x0007f */ 
  ONU_MAC_LOCAL_TIMER_t                    LOCAL_TIMER;                   /* addr: 0x00080 */ 
  ONU_MAC_DEQUE_t                          DEQUE;                         /* addr: 0x00081 */ 
  ONU_MAC_MPCP_DEREG_CAUSE_t               MPCP_DEREG_CAUSE;              /* addr: 0x00082 */ 
  ONU_MAC_MPCP_DISCOVERY_PARAMETER1_t      MPCP_DISCOVERY_PARAMETER1;     /* addr: 0x00083 */ 
  ONU_MAC_MPCP_DISCOVERY_PARAMETER2_t      MPCP_DISCOVERY_PARAMETER2;     /* addr: 0x00084 */ 
  ONU_MAC_MPCP_DISCOVERY_PARAMETER3_t      MPCP_DISCOVERY_PARAMETER3;     /* addr: 0x00085 */ 
  ONU_MAC_MPCP_TX_ELAPSE_t                 MPCP_TX_ELAPSE;                /* addr: 0x00086 */ 
  ONU_MAC_MPCP_RX_ELAPSE_t                 MPCP_RX_ELAPSE;                /* addr: 0x00087 */ 
  ONU_MAC_MPCP_DEREG_HISTORY_t             MPCP_DEREG_HISTORY;            /* addr: 0x00088 */ 
  ONU_MAC_MPCP_DRIFT_FIRST_LT_t            MPCP_DRIFT_FIRST_LT;           /* addr: 0x00089 */ 
  ONU_MAC_MPCP_DRIFT_FIRST_TS_t            MPCP_DRIFT_FIRST_TS;           /* addr: 0x0008a */ 
  ONU_MAC_MPCP_DRIFT_LAST_LT_t             MPCP_DRIFT_LAST_LT;            /* addr: 0x0008b */ 
  ONU_MAC_MPCP_DRIFT_LAST_TS_t             MPCP_DRIFT_LAST_TS;            /* addr: 0x0008c */ 
  ONU_MAC_CHURN_CONFIG_CONTROL_t           CHURN_CONFIG_CONTROL;          /* addr: 0x0008d */ 
  ONU_MAC_HW_GEN_KEY03_t                   HW_GEN_KEY03;                  /* addr: 0x0008e */ 
  ONU_MAC_HW_GEN_KEY02_t                   HW_GEN_KEY02;                  /* addr: 0x0008f */ 
  ONU_MAC_HW_GEN_KEY01_t                   HW_GEN_KEY01;                  /* addr: 0x00090 */ 
  ONU_MAC_HW_GEN_KEY00_t                   HW_GEN_KEY00;                  /* addr: 0x00091 */ 
  ONU_MAC_HW_GEN_KEY13_t                   HW_GEN_KEY13;                  /* addr: 0x00092 */ 
  ONU_MAC_HW_GEN_KEY12_t                   HW_GEN_KEY12;                  /* addr: 0x00093 */ 
  ONU_MAC_HW_GEN_KEY11_t                   HW_GEN_KEY11;                  /* addr: 0x00094 */ 
  ONU_MAC_HW_GEN_KEY10_t                   HW_GEN_KEY10;                  /* addr: 0x00095 */ 
  ONU_MAC_KEY_REQUEST_OAM_t                KEY_REQUEST_OAM;               /* addr: 0x00096 */ 
  ONU_MAC_TXCTRL_t                         TXCTRL;                        /* addr: 0x00097 */ 
  ONU_MAC_TX_MAXPKTSIZE_t                  TX_MAXPKTSIZE;                 /* addr: 0x00098 */ 
  ONU_MAC_TXMAC_CFG_t                      TXMAC_CFG;                     /* addr: 0x00099 */ 
  ONU_MAC_PACKET_DRAIN_CNT_t               PACKET_DRAIN_CNT;              /* addr: 0x0009a */ 
  ONU_MAC_TX_CRCERR_CNT_t                  TX_CRCERR_CNT;                 /* addr: 0x0009b */ 
  ONU_MAC_CHURNING_OAM_OUI_t               CHURNING_OAM_OUI;              /* addr: 0x0009c */ 
  ONU_MAC_CHURNING_OAM_FLAG_t              CHURNING_OAM_FLAG;             /* addr: 0x0009d */ 
  ONU_MAC_AES_TIMER_t                      AES_TIMER;                     /* addr: 0x0009e */ 
  ONU_MAC_AES_TIMER2_t                     AES_TIMER2;                    /* addr: 0x0009f */ 
  ONU_MAC_MIB_CFG_t                        MIB_CFG;                       /* addr: 0x000a0 */ 
  ONU_MAC_MIB_CFG2_t                       MIB_CFG2;                      /* addr: 0x000a1 */ 
  ONU_MAC_MISC_t                           MISC;                          /* addr: 0x000a2 */ 
  ONU_MAC_MIB_RESET_t                      MIB_RESET;                     /* addr: 0x000a3 */ 
  ONU_MAC_MIB_ACCESS_t                     MIB_ACCESS;                    /* addr: 0x000a4 */ 
  ONU_MAC_MIB_DATA1_t                      MIB_DATA1;                     /* addr: 0x000a5 */ 
  ONU_MAC_MIB_DATA0_t                      MIB_DATA0;                     /* addr: 0x000a6 */ 
  ONU_MAC_REFCLK_FCW_t                     REFCLK_FCW;                    /* addr: 0x000a7 */ 
  ONU_MAC_REFCLK_FCW_MAX_t                 REFCLK_FCW_MAX;                /* addr: 0x000a8 */ 
  ONU_MAC_FCW_VALUE_t                      FCW_VALUE;                     /* addr: 0x000a9 */ 
  ONU_MAC_PHASE_ADJUST_VALUE1_t            PHASE_ADJUST_VALUE1;           /* addr: 0x000aa */ 
  ONU_MAC_PHASE_ADJUST_VALUE0_t            PHASE_ADJUST_VALUE0;           /* addr: 0x000ab */ 
  ONU_MAC_PULSE_WIDTH_t                    PULSE_WIDTH;                   /* addr: 0x000ac */ 
  ONU_MAC_PHASE_ADJUST_ENABLE_t            PHASE_ADJUST_ENABLE;           /* addr: 0x000ad */ 
  ONU_MAC_CAPTURE_FCW_t                    CAPTURE_FCW;                   /* addr: 0x000ae */ 
  ONU_MAC_CAPTURE_LOCALTIME1_t             CAPTURE_LOCALTIME1;            /* addr: 0x000af */ 
  ONU_MAC_CAPTURE_LOCALTIME0_t             CAPTURE_LOCALTIME0;            /* addr: 0x000b0 */ 
  ONU_MAC_TOD_S_PPS_t                      TOD_S_PPS;                     /* addr: 0x000b1 */ 
  ONU_MAC_TOD_PPS_WID_t                    TOD_PPS_WID;                   /* addr: 0x000b2 */ 
  ONU_MAC_TOD_S_TOD_t                      TOD_S_TOD;                     /* addr: 0x000b3 */ 
  ONU_MAC_DBG_WIRE_t                       DBG_WIRE;                      /* addr: 0x000b4 */ 
  ONU_MAC_HW_DEBUG_OUT_t                   HW_DEBUG_OUT;                  /* addr: 0x000b5 */ 
  ONU_MAC_RXMAC_DEBUG_t                    RXMAC_DEBUG;                   /* addr: 0x000b6 */ 
  ONU_MAC_INPCS_CNT_t                      INPCS_CNT;                     /* addr: 0x000b7 */ 
  ONU_MAC_OUTPCS_CNT_t                     OUTPCS_CNT;                    /* addr: 0x000b8 */ 
  ONU_MAC_OUTMAC_CNT_t                     OUTMAC_CNT;                    /* addr: 0x000b9 */ 
  ONU_MAC_OUTLSR_CNT_t                     OUTLSR_CNT;                    /* addr: 0x000ba */ 
  ONU_MAC_REVISION_t                       REVISION;                      /* addr: 0x000bb */ 
  ONU_MAC_DATAPATH_BYPASS_t                DATAPATH_BYPASS;               /* addr: 0x000bc */ 
  ONU_MAC_DATAPATH_STATUS_t                DATAPATH_STATUS;               /* addr: 0x000bd */ 
  ONU_MAC_PON_PORT_CLEAR_t                 PON_PORT_CLEAR;                /* addr: 0x000be */ 
  ONU_MAC_SIM_CTRL_t                       SIM_CTRL;                      /* addr: 0x000bf */ 
  ONU_MAC_GRANT_MONITOR_t                  GRANT_MONITOR;                 /* addr: 0x000c0 */ 
  ONU_MAC_GRANT_MONITOR_STATUS_t           GRANT_MONITOR_STATUS;          /* addr: 0x000c1 */ 
  ONU_MAC_BM_PON_TX_STATUS_t               BM_PON_TX_STATUS;              /* addr: 0x000c2 */ 
  ONU_MAC_INTERRUPT_SOURCE2_t              INTERRUPT_SOURCE2;             /* addr: 0x000c3 */ 
  ONU_MAC_INTERRUPT_ENABLE2_t              INTERRUPT_ENABLE2;             /* addr: 0x000c4 */ 
  ONU_MAC_INTERRUPT_ISAS2_t                INTERRUPT_ISAS2;               /* addr: 0x000c5 */ 
  ONU_MAC_MPCP_RTT_COMPENSATION_MODE_t     MPCP_RTT_COMPENSATION_MODE;    /* addr: 0x000c6 */ 
  ONU_MAC_MPCP_RTT_COMPENSATION_t          MPCP_RTT_COMPENSATION;         /* addr: 0x000c7 */ 
  ONU_MAC_MPCP_RTT_COMPENSATION2_t         MPCP_RTT_COMPENSATION2;        /* addr: 0x000c8 */ 
  ONU_MAC_AES_FEC_COMP_t                   AES_FEC_COMP;                  /* addr: 0x000c9 */ 
  ONU_MAC_TX_POWER_CTRL_WAKEUP_TIME_t      TX_POWER_CTRL_WAKEUP_TIME;     /* addr: 0x000ca */ 
  ONU_MAC_TX_POWER_CTRL_SHUTDOWN_TIME_t    TX_POWER_CTRL_SHUTDOWN_TIME;   /* addr: 0x000cb */ 
  ONU_MAC_TX_POWER_CTRL_KEEP_TIME_t        TX_POWER_CTRL_KEEP_TIME;       /* addr: 0x000cc */ 
  ONU_MAC_TX_POWER_CTRL_MISC_t             TX_POWER_CTRL_MISC;            /* addr: 0x000cd */ 
  ONU_MAC_PTP_CONFIG_t                     PTP_CONFIG;                    /* addr: 0x000ce */ 
  ONU_MAC_PTP_ASYM_DELAY_t                 PTP_ASYM_DELAY;                /* addr: 0x000cf */ 
  ONU_MAC_PTP_ING_LATCH_t                  PTP_ING_LATCH;                 /* addr: 0x000d0 */ 
  ONU_MAC_PTP_PROCESS_t                    PTP_PROCESS;                   /* addr: 0x000d1 */ 
  ONU_MAC_PTP_EG_LATCH_t                   PTP_EG_LATCH;                  /* addr: 0x000d2 */ 
  ONU_MAC_PTP_ING_FWD_t                    PTP_ING_FWD;                   /* addr: 0x000d3 */ 
  ONU_MAC_PTP_EG_STS_t                     PTP_EG_STS;                    /* addr: 0x000d4 */ 
  ONU_MAC_RX_MAC_CONFIG0_t                 RX_MAC_CONFIG0;                /* addr: 0x000d5 */ 
  ONU_MAC_RX_MAC_CONFIG1_t                 RX_MAC_CONFIG1;                /* addr: 0x000d6 */ 
  ONU_MAC_DBG_SEL_t                        DBG_SEL;                       /* addr: 0x000d7 */ 
  ONU_MAC_RSV_CFG1_t                       RSV_CFG1;                      /* addr: 0x000d8 */ 
  ONU_MAC_RSV_CFG2_t                       RSV_CFG2;                      /* addr: 0x000d9 */ 
  ONU_MAC_RSV_STATUS1_t                    RSV_STATUS1;                   /* addr: 0x000da */ 
  ONU_MAC_RSV_STATUS2_t                    RSV_STATUS2;                   /* addr: 0x000db */ 
  ONU_MAC_RSV_CNT1_t                       RSV_CNT1;                      /* addr: 0x000dc */ 
  ONU_MAC_RSV_CNT2_t                       RSV_CNT2;                      /* addr: 0x000dd */ 
} EPON_t;

  

/*
 * bm
 */
  



#define BM_SCH_CFG                               0x09000
#define BM_SCH_CFG_dft                           0x00300038




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  8 ;
    cs_uint32 dp_option            :  1 ; /* bits 23:23 */
    cs_uint32 dis_self_reset       :  1 ; /* bits 22:22 */
    cs_uint32 unknown_dpid         :  2 ; /* bits 21:20 */
    cs_uint32 ds_abr_en            :  1 ; /* bits 19:19 */
    cs_uint32 us_abr_en            :  1 ; /* bits 18:18 */
    cs_uint32 ds_dp_en             :  1 ; /* bits 17:17 */
    cs_uint32 us_dp_en             :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 kt_mode_rsvd         :  1 ; /* bits 9:9 */
    cs_uint32 burst_mode_rsvd      :  1 ; /* bits 8:8 */
    cs_uint32 dis_win_fit_nsp_rsvd :  1 ; /* bits 7:7 */
    cs_uint32 dis_win_fit_sp_rsvd  :  1 ; /* bits 6:6 */
    cs_uint32 pon_cpu_pkt_en       :  1 ; /* bits 5:5 */
    cs_uint32 ge_cpu_pkt_en        :  1 ; /* bits 4:4 */
    cs_uint32 voq8_en              :  1 ; /* bits 3:3 */
    cs_uint32 cpu_ql_en            :  1 ; /* bits 2:2 */
    cs_uint32 pon_ql_en            :  1 ; /* bits 1:1 */
    cs_uint32 ge_ql_en             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ge_ql_en             :  1 ; /* bits 0:0 */
    cs_uint32 pon_ql_en            :  1 ; /* bits 1:1 */
    cs_uint32 cpu_ql_en            :  1 ; /* bits 2:2 */
    cs_uint32 voq8_en              :  1 ; /* bits 3:3 */
    cs_uint32 ge_cpu_pkt_en        :  1 ; /* bits 4:4 */
    cs_uint32 pon_cpu_pkt_en       :  1 ; /* bits 5:5 */
    cs_uint32 dis_win_fit_sp_rsvd  :  1 ; /* bits 6:6 */
    cs_uint32 dis_win_fit_nsp_rsvd :  1 ; /* bits 7:7 */
    cs_uint32 burst_mode_rsvd      :  1 ; /* bits 8:8 */
    cs_uint32 kt_mode_rsvd         :  1 ; /* bits 9:9 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 us_dp_en             :  1 ; /* bits 16:16 */
    cs_uint32 ds_dp_en             :  1 ; /* bits 17:17 */
    cs_uint32 us_abr_en            :  1 ; /* bits 18:18 */
    cs_uint32 ds_abr_en            :  1 ; /* bits 19:19 */
    cs_uint32 unknown_dpid         :  2 ; /* bits 21:20 */
    cs_uint32 dis_self_reset       :  1 ; /* bits 22:22 */
    cs_uint32 dp_option            :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd2               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_SCH_CFG_t;





#define BM_SHARED_BUFFER_CFG                     0x09001
#define BM_SHARED_BUFFER_CFG_dft                 0xa4002500




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ppbm_en_cpu          :  1 ; /* bits 31:31 */
    cs_uint32 ppbm_en              :  1 ; /* bits 30:30 */
    cs_uint32 slmt_cpu             : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 slmt                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 slmt                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 slmt_cpu             : 14 ; /* bits 29:16 */
    cs_uint32 ppbm_en              :  1 ; /* bits 30:30 */
    cs_uint32 ppbm_en_cpu          :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_SHARED_BUFFER_CFG_t;





#define BM_GE_PORT_BUFFER_CFG                    0x09002
#define BM_GE_PORT_BUFFER_CFG_dft                0x00800010




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 private_lmt          : 14 ; /* bits 29:16 */
    cs_uint32 u_thrsld             :  2 ; /* bits 15:14 */
    cs_uint32 l_thrsld             : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 l_thrsld             : 14 ; /* bits 13:0 */
    cs_uint32 u_thrsld             :  2 ; /* bits 15:14 */
    cs_uint32 private_lmt          : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_PORT_BUFFER_CFG_t;





#define BM_PON_PORT_BUFFER_CFG                   0x09003
#define BM_PON_PORT_BUFFER_CFG_dft               0x00800010




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 private_lmt          : 14 ; /* bits 29:16 */
    cs_uint32 u_thrsld             :  2 ; /* bits 15:14 */
    cs_uint32 l_thrsld             : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 l_thrsld             : 14 ; /* bits 13:0 */
    cs_uint32 u_thrsld             :  2 ; /* bits 15:14 */
    cs_uint32 private_lmt          : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_PORT_BUFFER_CFG_t;





#define BM_CPU_PORT_BUFFER_CFG                   0x09004
#define BM_CPU_PORT_BUFFER_CFG_dft               0x00000080




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 private_lmt          : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 private_lmt          : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_PORT_BUFFER_CFG_t;





#define BM_SHARED_BUFFER_THRESHOLD               0x09005
#define BM_SHARED_BUFFER_THRESHOLD_dft           0x1e000800




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 u_thrsld             : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 l_thrsld             : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 l_thrsld             : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 u_thrsld             : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_SHARED_BUFFER_THRESHOLD_t;





#define BM_GE_QUEUE_LENGTH_LIMIT_COS7_4          0x09006
#define BM_GE_QUEUE_LENGTH_LIMIT_COS7_4_dft      0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ql_cos7              :  8 ; /* bits 31:24 */
    cs_uint32 ql_cos6              :  8 ; /* bits 23:16 */
    cs_uint32 ql_cos5              :  8 ; /* bits 15:8 */
    cs_uint32 ql_cos4              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ql_cos4              :  8 ; /* bits 7:0 */
    cs_uint32 ql_cos5              :  8 ; /* bits 15:8 */
    cs_uint32 ql_cos6              :  8 ; /* bits 23:16 */
    cs_uint32 ql_cos7              :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_QUEUE_LENGTH_LIMIT_COS7_4_t;





#define BM_GE_QUEUE_LENGTH_LIMIT_COS3_0          0x09007
#define BM_GE_QUEUE_LENGTH_LIMIT_COS3_0_dft      0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ql_cos3              :  8 ; /* bits 31:24 */
    cs_uint32 ql_cos2              :  8 ; /* bits 23:16 */
    cs_uint32 ql_cos1              :  8 ; /* bits 15:8 */
    cs_uint32 ql_cos0              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ql_cos0              :  8 ; /* bits 7:0 */
    cs_uint32 ql_cos1              :  8 ; /* bits 15:8 */
    cs_uint32 ql_cos2              :  8 ; /* bits 23:16 */
    cs_uint32 ql_cos3              :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_QUEUE_LENGTH_LIMIT_COS3_0_t;





#define BM_PON_QUEUE_LENGTH_LIMIT_COS7_4         0x09008
#define BM_PON_QUEUE_LENGTH_LIMIT_COS7_4_dft     0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ql_cos7              :  8 ; /* bits 31:24 */
    cs_uint32 ql_cos6              :  8 ; /* bits 23:16 */
    cs_uint32 ql_cos5              :  8 ; /* bits 15:8 */
    cs_uint32 ql_cos4              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ql_cos4              :  8 ; /* bits 7:0 */
    cs_uint32 ql_cos5              :  8 ; /* bits 15:8 */
    cs_uint32 ql_cos6              :  8 ; /* bits 23:16 */
    cs_uint32 ql_cos7              :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_QUEUE_LENGTH_LIMIT_COS7_4_t;





#define BM_PON_QUEUE_LENGTH_LIMIT_COS3_0         0x09009
#define BM_PON_QUEUE_LENGTH_LIMIT_COS3_0_dft     0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ql_cos3              :  8 ; /* bits 31:24 */
    cs_uint32 ql_cos2              :  8 ; /* bits 23:16 */
    cs_uint32 ql_cos1              :  8 ; /* bits 15:8 */
    cs_uint32 ql_cos0              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ql_cos0              :  8 ; /* bits 7:0 */
    cs_uint32 ql_cos1              :  8 ; /* bits 15:8 */
    cs_uint32 ql_cos2              :  8 ; /* bits 23:16 */
    cs_uint32 ql_cos3              :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_QUEUE_LENGTH_LIMIT_COS3_0_t;





#define BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0         0x0900a
#define BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0_dft     0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ql_cos7              :  4 ; /* bits 31:28 */
    cs_uint32 ql_cos6              :  4 ; /* bits 27:24 */
    cs_uint32 ql_cos5              :  4 ; /* bits 23:20 */
    cs_uint32 ql_cos4              :  4 ; /* bits 19:16 */
    cs_uint32 ql_cos3              :  4 ; /* bits 15:12 */
    cs_uint32 ql_cos2              :  4 ; /* bits 11:8 */
    cs_uint32 ql_cos1              :  4 ; /* bits 7:4 */
    cs_uint32 ql_cos0              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ql_cos0              :  4 ; /* bits 3:0 */
    cs_uint32 ql_cos1              :  4 ; /* bits 7:4 */
    cs_uint32 ql_cos2              :  4 ; /* bits 11:8 */
    cs_uint32 ql_cos3              :  4 ; /* bits 15:12 */
    cs_uint32 ql_cos4              :  4 ; /* bits 19:16 */
    cs_uint32 ql_cos5              :  4 ; /* bits 23:20 */
    cs_uint32 ql_cos6              :  4 ; /* bits 27:24 */
    cs_uint32 ql_cos7              :  4 ; /* bits 31:28 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0_t;





#define BM_GE_PORT_TX_BUFFER_LMT                 0x0900b
#define BM_GE_PORT_TX_BUFFER_LMT_dft             0x0000ab00




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 16 ;
    cs_uint32 tx_lmt_en            :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 tx_buffer_thrsld     : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_buffer_thrsld     : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 tx_lmt_en            :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd2               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_PORT_TX_BUFFER_LMT_t;





#define BM_PON_PORT_TX_BUFFER_LMT                0x0900c
#define BM_PON_PORT_TX_BUFFER_LMT_dft            0x0000aaaa




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 16 ;
    cs_uint32 tx_lmt_en            :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 tx_buffer_thrsld     : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_buffer_thrsld     : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 tx_lmt_en            :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd2               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_PORT_TX_BUFFER_LMT_t;





#define BM_MA_PORT_TX_BUFFER_LMT                 0x0900d
#define BM_MA_PORT_TX_BUFFER_LMT_dft             0x00000100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 16 ;
    cs_uint32 tx_lmt_en            :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 tx_buffer_thrsld     : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_buffer_thrsld     : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 tx_lmt_en            :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd2               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_PORT_TX_BUFFER_LMT_t;





#define BM_PC_CFG1                               0x0900e
#define BM_PC_CFG1_dft                           0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 entry                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 entry                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PC_CFG1_t;





#define BM_PC_CFG0                               0x0900f
#define BM_PC_CFG0_dft                           0xffffffe4




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 entry                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 entry                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PC_CFG0_t;





#define BM_LINKLIST_SIZE                         0x09010
#define BM_LINKLIST_SIZE_dft                     0x00003fff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 size                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 size                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_LINKLIST_SIZE_t;





#define BM_PORT_TX_CTRL                          0x09011
#define BM_PORT_TX_CTRL_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 stop_cpu_rx          :  1 ; /* bits 26:26 */
    cs_uint32 stop_pon_rx          :  1 ; /* bits 25:25 */
    cs_uint32 stop_ge_rx           :  1 ; /* bits 24:24 */
    cs_uint32 cpu_tx_en            :  8 ; /* bits 23:16 */
    cs_uint32 pon_tx_en            :  8 ; /* bits 15:8 */
    cs_uint32 ge_tx_en             :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ge_tx_en             :  8 ; /* bits 7:0 */
    cs_uint32 pon_tx_en            :  8 ; /* bits 15:8 */
    cs_uint32 cpu_tx_en            :  8 ; /* bits 23:16 */
    cs_uint32 stop_ge_rx           :  1 ; /* bits 24:24 */
    cs_uint32 stop_pon_rx          :  1 ; /* bits 25:25 */
    cs_uint32 stop_cpu_rx          :  1 ; /* bits 26:26 */
    cs_uint32 rsrvd1               :  5 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PORT_TX_CTRL_t;





#define BM_DRR_WEIGHT_BASE                       0x09012
#define BM_DRR_WEIGHT_BASE_dft                   0x00000001




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 base                 :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 base                 :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DRR_WEIGHT_BASE_t;





#define BM_GE_WEIGHT_RATIO_COS7_4                0x09013
#define BM_GE_WEIGHT_RATIO_COS7_4_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cos7                 :  8 ; /* bits 31:24 */
    cs_uint32 cos6                 :  8 ; /* bits 23:16 */
    cs_uint32 cos5                 :  8 ; /* bits 15:8 */
    cs_uint32 cos4                 :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cos4                 :  8 ; /* bits 7:0 */
    cs_uint32 cos5                 :  8 ; /* bits 15:8 */
    cs_uint32 cos6                 :  8 ; /* bits 23:16 */
    cs_uint32 cos7                 :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_WEIGHT_RATIO_COS7_4_t;





#define BM_GE_WEIGHT_RATIO_COS3_0                0x09014
#define BM_GE_WEIGHT_RATIO_COS3_0_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cos3                 :  8 ; /* bits 31:24 */
    cs_uint32 cos2                 :  8 ; /* bits 23:16 */
    cs_uint32 cos1                 :  8 ; /* bits 15:8 */
    cs_uint32 cos0                 :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cos0                 :  8 ; /* bits 7:0 */
    cs_uint32 cos1                 :  8 ; /* bits 15:8 */
    cs_uint32 cos2                 :  8 ; /* bits 23:16 */
    cs_uint32 cos3                 :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_WEIGHT_RATIO_COS3_0_t;





#define BM_PON_WEIGHT_RATIO_COS7_4               0x09015
#define BM_PON_WEIGHT_RATIO_COS7_4_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cos7                 :  8 ; /* bits 31:24 */
    cs_uint32 cos6                 :  8 ; /* bits 23:16 */
    cs_uint32 cos5                 :  8 ; /* bits 15:8 */
    cs_uint32 cos4                 :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cos4                 :  8 ; /* bits 7:0 */
    cs_uint32 cos5                 :  8 ; /* bits 15:8 */
    cs_uint32 cos6                 :  8 ; /* bits 23:16 */
    cs_uint32 cos7                 :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_WEIGHT_RATIO_COS7_4_t;





#define BM_PON_WEIGHT_RATIO_COS3_0               0x09016
#define BM_PON_WEIGHT_RATIO_COS3_0_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cos3                 :  8 ; /* bits 31:24 */
    cs_uint32 cos2                 :  8 ; /* bits 23:16 */
    cs_uint32 cos1                 :  8 ; /* bits 15:8 */
    cs_uint32 cos0                 :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cos0                 :  8 ; /* bits 7:0 */
    cs_uint32 cos1                 :  8 ; /* bits 15:8 */
    cs_uint32 cos2                 :  8 ; /* bits 23:16 */
    cs_uint32 cos3                 :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_WEIGHT_RATIO_COS3_0_t;





#define BM_ABR_CFG                               0x09017
#define BM_ABR_CFG_dft                           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 ds_thrsld            : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 us_thrsld            : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 us_thrsld            : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ds_thrsld            : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_ABR_CFG_t;





#define BM_DP_CFG                                0x09018
#define BM_DP_CFG_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 ds_thrsld            : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 us_thrsld            : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 us_thrsld            : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ds_thrsld            : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DP_CFG_t;





#define BM_DP_CFG_STAGE2_1                       0x09019
#define BM_DP_CFG_STAGE2_1_dft                   0x23002200




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 dp_thrsld_2          : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 dp_thrsld_1          : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dp_thrsld_1          : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 dp_thrsld_2          : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DP_CFG_STAGE2_1_t;





#define BM_DP_CFG_STAGE4_3                       0x0901a
#define BM_DP_CFG_STAGE4_3_dft                   0x23002200




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 dp_thrsld_4          : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 dp_thrsld_3          : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dp_thrsld_3          : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 dp_thrsld_4          : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DP_CFG_STAGE4_3_t;





#define BM_DP_CFG_STAGE6_5                       0x0901b
#define BM_DP_CFG_STAGE6_5_dft                   0x23002200




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 dp_thrsld_6          : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 dp_thrsld_5          : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dp_thrsld_5          : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 dp_thrsld_6          : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DP_CFG_STAGE6_5_t;





#define BM_DP_CFG_STAGE8_7                       0x0901c
#define BM_DP_CFG_STAGE8_7_dft                   0x25002400




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 dp_thrsld_8          : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 dp_thrsld_7          : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dp_thrsld_7          : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 dp_thrsld_8          : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DP_CFG_STAGE8_7_t;





#define BM_MA_DPID4_COS_CFG                      0x0901d
#define BM_MA_DPID4_COS_CFG_dft                  0x77776666




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd8               :  1 ;
    cs_uint32 dpid_cos7            :  3 ; /* bits 30:28 */
    cs_uint32 rsrvd7               :  1 ;
    cs_uint32 dpid_cos6            :  3 ; /* bits 26:24 */
    cs_uint32 rsrvd6               :  1 ;
    cs_uint32 dpid_cos5            :  3 ; /* bits 22:20 */
    cs_uint32 rsrvd5               :  1 ;
    cs_uint32 dpid_cos4            :  3 ; /* bits 18:16 */
    cs_uint32 rsrvd4               :  1 ;
    cs_uint32 dpid_cos3            :  3 ; /* bits 14:12 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 dpid_cos2            :  3 ; /* bits 10:8 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 dpid_cos1            :  3 ; /* bits 6:4 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 dpid_cos0            :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dpid_cos0            :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 dpid_cos1            :  3 ; /* bits 6:4 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 dpid_cos2            :  3 ; /* bits 10:8 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 dpid_cos3            :  3 ; /* bits 14:12 */
    cs_uint32 rsrvd4               :  1 ;
    cs_uint32 dpid_cos4            :  3 ; /* bits 18:16 */
    cs_uint32 rsrvd5               :  1 ;
    cs_uint32 dpid_cos5            :  3 ; /* bits 22:20 */
    cs_uint32 rsrvd6               :  1 ;
    cs_uint32 dpid_cos6            :  3 ; /* bits 26:24 */
    cs_uint32 rsrvd7               :  1 ;
    cs_uint32 dpid_cos7            :  3 ; /* bits 30:28 */
    cs_uint32 rsrvd8               :  1 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_DPID4_COS_CFG_t;





#define BM_MA_DPID5_COS_CFG                      0x0901e
#define BM_MA_DPID5_COS_CFG_dft                  0x55554444




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd8               :  1 ;
    cs_uint32 dpid_cos7            :  3 ; /* bits 30:28 */
    cs_uint32 rsrvd7               :  1 ;
    cs_uint32 dpid_cos6            :  3 ; /* bits 26:24 */
    cs_uint32 rsrvd6               :  1 ;
    cs_uint32 dpid_cos5            :  3 ; /* bits 22:20 */
    cs_uint32 rsrvd5               :  1 ;
    cs_uint32 dpid_cos4            :  3 ; /* bits 18:16 */
    cs_uint32 rsrvd4               :  1 ;
    cs_uint32 dpid_cos3            :  3 ; /* bits 14:12 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 dpid_cos2            :  3 ; /* bits 10:8 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 dpid_cos1            :  3 ; /* bits 6:4 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 dpid_cos0            :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dpid_cos0            :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 dpid_cos1            :  3 ; /* bits 6:4 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 dpid_cos2            :  3 ; /* bits 10:8 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 dpid_cos3            :  3 ; /* bits 14:12 */
    cs_uint32 rsrvd4               :  1 ;
    cs_uint32 dpid_cos4            :  3 ; /* bits 18:16 */
    cs_uint32 rsrvd5               :  1 ;
    cs_uint32 dpid_cos5            :  3 ; /* bits 22:20 */
    cs_uint32 rsrvd6               :  1 ;
    cs_uint32 dpid_cos6            :  3 ; /* bits 26:24 */
    cs_uint32 rsrvd7               :  1 ;
    cs_uint32 dpid_cos7            :  3 ; /* bits 30:28 */
    cs_uint32 rsrvd8               :  1 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_DPID5_COS_CFG_t;





#define BM_MA_DPID6_COS_CFG                      0x0901f
#define BM_MA_DPID6_COS_CFG_dft                  0x33332222




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd8               :  1 ;
    cs_uint32 dpid_cos7            :  3 ; /* bits 30:28 */
    cs_uint32 rsrvd7               :  1 ;
    cs_uint32 dpid_cos6            :  3 ; /* bits 26:24 */
    cs_uint32 rsrvd6               :  1 ;
    cs_uint32 dpid_cos5            :  3 ; /* bits 22:20 */
    cs_uint32 rsrvd5               :  1 ;
    cs_uint32 dpid_cos4            :  3 ; /* bits 18:16 */
    cs_uint32 rsrvd4               :  1 ;
    cs_uint32 dpid_cos3            :  3 ; /* bits 14:12 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 dpid_cos2            :  3 ; /* bits 10:8 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 dpid_cos1            :  3 ; /* bits 6:4 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 dpid_cos0            :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dpid_cos0            :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 dpid_cos1            :  3 ; /* bits 6:4 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 dpid_cos2            :  3 ; /* bits 10:8 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 dpid_cos3            :  3 ; /* bits 14:12 */
    cs_uint32 rsrvd4               :  1 ;
    cs_uint32 dpid_cos4            :  3 ; /* bits 18:16 */
    cs_uint32 rsrvd5               :  1 ;
    cs_uint32 dpid_cos5            :  3 ; /* bits 22:20 */
    cs_uint32 rsrvd6               :  1 ;
    cs_uint32 dpid_cos6            :  3 ; /* bits 26:24 */
    cs_uint32 rsrvd7               :  1 ;
    cs_uint32 dpid_cos7            :  3 ; /* bits 30:28 */
    cs_uint32 rsrvd8               :  1 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_DPID6_COS_CFG_t;





#define BM_MA_COS_DPID_CFG                       0x09020
#define BM_MA_COS_DPID_CFG_dft                   0x44556666




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd8               :  1 ;
    cs_uint32 dpid_cos7            :  3 ; /* bits 30:28 */
    cs_uint32 rsrvd7               :  1 ;
    cs_uint32 dpid_cos6            :  3 ; /* bits 26:24 */
    cs_uint32 rsrvd6               :  1 ;
    cs_uint32 dpid_cos5            :  3 ; /* bits 22:20 */
    cs_uint32 rsrvd5               :  1 ;
    cs_uint32 dpid_cos4            :  3 ; /* bits 18:16 */
    cs_uint32 rsrvd4               :  1 ;
    cs_uint32 dpid_cos3            :  3 ; /* bits 14:12 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 dpid_cos2            :  3 ; /* bits 10:8 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 dpid_cos1            :  3 ; /* bits 6:4 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 dpid_cos0            :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 dpid_cos0            :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 dpid_cos1            :  3 ; /* bits 6:4 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 dpid_cos2            :  3 ; /* bits 10:8 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 dpid_cos3            :  3 ; /* bits 14:12 */
    cs_uint32 rsrvd4               :  1 ;
    cs_uint32 dpid_cos4            :  3 ; /* bits 18:16 */
    cs_uint32 rsrvd5               :  1 ;
    cs_uint32 dpid_cos5            :  3 ; /* bits 22:20 */
    cs_uint32 rsrvd6               :  1 ;
    cs_uint32 dpid_cos6            :  3 ; /* bits 26:24 */
    cs_uint32 rsrvd7               :  1 ;
    cs_uint32 dpid_cos7            :  3 ; /* bits 30:28 */
    cs_uint32 rsrvd8               :  1 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_COS_DPID_CFG_t;





#define BM_MPCP_CFG                              0x09021
#define BM_MPCP_CFG_dft                          0x00000006




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 27 ;
    cs_uint32 kt_rpt_mode_en       :  1 ; /* bits 4:4 */
    cs_uint32 kt_rpt_option        :  1 ; /* bits 3:3 */
    cs_uint32 edwrr_en_rsvd        :  1 ; /* bits 2:2 */
    cs_uint32 win_chk_en_rsvd      :  1 ; /* bits 1:1 */
    cs_uint32 rpt_en_rsvd          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rpt_en_rsvd          :  1 ; /* bits 0:0 */
    cs_uint32 win_chk_en_rsvd      :  1 ; /* bits 1:1 */
    cs_uint32 edwrr_en_rsvd        :  1 ; /* bits 2:2 */
    cs_uint32 kt_rpt_option        :  1 ; /* bits 3:3 */
    cs_uint32 kt_rpt_mode_en       :  1 ; /* bits 4:4 */
    cs_uint32 rsrvd1               : 27 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_CFG_t;





#define BM_MPCP_RPT_COS_MAPPING                  0x09022
#define BM_MPCP_RPT_COS_MAPPING_dft              0x76543210




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cos7                 :  4 ; /* bits 31:28 */
    cs_uint32 cos6                 :  4 ; /* bits 27:24 */
    cs_uint32 cos5                 :  4 ; /* bits 23:20 */
    cs_uint32 cos4                 :  4 ; /* bits 19:16 */
    cs_uint32 cos3                 :  4 ; /* bits 15:12 */
    cs_uint32 cos2                 :  4 ; /* bits 11:8 */
    cs_uint32 cos1                 :  4 ; /* bits 7:4 */
    cs_uint32 cos0                 :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cos0                 :  4 ; /* bits 3:0 */
    cs_uint32 cos1                 :  4 ; /* bits 7:4 */
    cs_uint32 cos2                 :  4 ; /* bits 11:8 */
    cs_uint32 cos3                 :  4 ; /* bits 15:12 */
    cs_uint32 cos4                 :  4 ; /* bits 19:16 */
    cs_uint32 cos5                 :  4 ; /* bits 23:20 */
    cs_uint32 cos6                 :  4 ; /* bits 27:24 */
    cs_uint32 cos7                 :  4 ; /* bits 31:28 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_RPT_COS_MAPPING_t;





#define BM_MPCP_OFFSET                           0x09023
#define BM_MPCP_OFFSET_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rpt_total_offset     : 16 ; /* bits 31:16 */
    cs_uint32 rpt_q2_offset        :  8 ; /* bits 15:8 */
    cs_uint32 rpt_fix              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rpt_fix              :  8 ; /* bits 7:0 */
    cs_uint32 rpt_q2_offset        :  8 ; /* bits 15:8 */
    cs_uint32 rpt_total_offset     : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_OFFSET_t;





#define BM_MPCP_Q2_THRESHOLD_7_6                 0x09024
#define BM_MPCP_Q2_THRESHOLD_7_6_dft             0x80008000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rpt_thrsld_cos7      : 16 ; /* bits 31:16 */
    cs_uint32 rpt_thrsld_cos6      : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rpt_thrsld_cos6      : 16 ; /* bits 15:0 */
    cs_uint32 rpt_thrsld_cos7      : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_Q2_THRESHOLD_7_6_t;





#define BM_MPCP_Q2_THRESHOLD_5_4                 0x09025
#define BM_MPCP_Q2_THRESHOLD_5_4_dft             0x80008000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rpt_thrsld_cos5      : 16 ; /* bits 31:16 */
    cs_uint32 rpt_thrsld_cos4      : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rpt_thrsld_cos4      : 16 ; /* bits 15:0 */
    cs_uint32 rpt_thrsld_cos5      : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_Q2_THRESHOLD_5_4_t;





#define BM_MPCP_Q2_THRESHOLD_3_2                 0x09026
#define BM_MPCP_Q2_THRESHOLD_3_2_dft             0x80008000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rpt_thrsld_cos3      : 16 ; /* bits 31:16 */
    cs_uint32 rpt_thrsld_cos2      : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rpt_thrsld_cos2      : 16 ; /* bits 15:0 */
    cs_uint32 rpt_thrsld_cos3      : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_Q2_THRESHOLD_3_2_t;





#define BM_MPCP_Q2_THRESHOLD_1_0                 0x09027
#define BM_MPCP_Q2_THRESHOLD_1_0_dft             0x80008000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rpt_thrsld_cos1      : 16 ; /* bits 31:16 */
    cs_uint32 rpt_thrsld_cos0      : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rpt_thrsld_cos0      : 16 ; /* bits 15:0 */
    cs_uint32 rpt_thrsld_cos1      : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_Q2_THRESHOLD_1_0_t;





#define BM_MPCP_Q2_MAX_7_6                       0x09028
#define BM_MPCP_Q2_MAX_7_6_dft                   0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rpt_max_cos7         : 16 ; /* bits 31:16 */
    cs_uint32 rpt_max_cos6         : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rpt_max_cos6         : 16 ; /* bits 15:0 */
    cs_uint32 rpt_max_cos7         : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_Q2_MAX_7_6_t;





#define BM_MPCP_Q2_MAX_5_4                       0x09029
#define BM_MPCP_Q2_MAX_5_4_dft                   0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rpt_max_cos5         : 16 ; /* bits 31:16 */
    cs_uint32 rpt_max_cos4         : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rpt_max_cos4         : 16 ; /* bits 15:0 */
    cs_uint32 rpt_max_cos5         : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_Q2_MAX_5_4_t;





#define BM_MPCP_Q2_MAX_3_2                       0x0902a
#define BM_MPCP_Q2_MAX_3_2_dft                   0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rpt_max_cos3         : 16 ; /* bits 31:16 */
    cs_uint32 rpt_max_cos2         : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rpt_max_cos2         : 16 ; /* bits 15:0 */
    cs_uint32 rpt_max_cos3         : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_Q2_MAX_3_2_t;





#define BM_MPCP_Q2_MAX_1_0                       0x0902b
#define BM_MPCP_Q2_MAX_1_0_dft                   0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rpt_max_cos1         : 16 ; /* bits 31:16 */
    cs_uint32 rpt_max_cos0         : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rpt_max_cos0         : 16 ; /* bits 15:0 */
    cs_uint32 rpt_max_cos1         : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_Q2_MAX_1_0_t;





#define BM_FLUSH_CTRL                            0x0902c
#define BM_FLUSH_CTRL_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 mpcp_fifo_flush_en   :  1 ; /* bits 3:3 */
    cs_uint32 ma_flush_en          :  1 ; /* bits 2:2 */
    cs_uint32 pon_flush_en         :  1 ; /* bits 1:1 */
    cs_uint32 ge_flush_en          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ge_flush_en          :  1 ; /* bits 0:0 */
    cs_uint32 pon_flush_en         :  1 ; /* bits 1:1 */
    cs_uint32 ma_flush_en          :  1 ; /* bits 2:2 */
    cs_uint32 mpcp_fifo_flush_en   :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_FLUSH_CTRL_t;





#define BM_FLUSH_STATUS                          0x0902d
#define BM_FLUSH_STATUS_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 cpu_flush_done       :  1 ; /* bits 2:2 */
    cs_uint32 pon_flush_done       :  1 ; /* bits 1:1 */
    cs_uint32 ge_flush_done        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ge_flush_done        :  1 ; /* bits 0:0 */
    cs_uint32 pon_flush_done       :  1 ; /* bits 1:1 */
    cs_uint32 cpu_flush_done       :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_FLUSH_STATUS_t;





#define BM_PORT_SHAPER_CFG                       0x0902e
#define BM_PORT_SHAPER_CFG_dft                   0x00000014




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 cpu_cnt_option_rsvd  :  1 ; /* bits 8:8 */
    cs_uint32 cpu_shaper_en        :  1 ; /* bits 7:7 */
    cs_uint32 pon_shaper_en        :  1 ; /* bits 6:6 */
    cs_uint32 ge_shaper_en         :  1 ; /* bits 5:5 */
    cs_uint32 w_rate               :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 w_rate               :  5 ; /* bits 4:0 */
    cs_uint32 ge_shaper_en         :  1 ; /* bits 5:5 */
    cs_uint32 pon_shaper_en        :  1 ; /* bits 6:6 */
    cs_uint32 cpu_shaper_en        :  1 ; /* bits 7:7 */
    cs_uint32 cpu_cnt_option_rsvd  :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PORT_SHAPER_CFG_t;





#define BM_LYNXD_MPCP_CFG                        0x0902f
#define BM_LYNXD_MPCP_CFG_dft                    0x00004801




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 15 ;
    cs_uint32 single_window_en     :  1 ; /* bits 16:16 */
    cs_uint32 ignor_rpt_en         :  1 ; /* bits 15:15 */
    cs_uint32 fill_unrpt_pkt_en    :  1 ; /* bits 14:14 */
    cs_uint32 force_quick_rpt_en   :  1 ; /* bits 13:13 */
    cs_uint32 rpt_order            :  1 ; /* bits 12:12 */
    cs_uint32 rpt_number           :  2 ; /* bits 11:10 */
    cs_uint32 rpt_format           :  2 ; /* bits 9:8 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 agg_qs_thrshld_en    :  1 ; /* bits 6:6 */
    cs_uint32 zeroed_qs_en         :  5 ; /* bits 5:1 */
    cs_uint32 window_fit_en        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 window_fit_en        :  1 ; /* bits 0:0 */
    cs_uint32 zeroed_qs_en         :  5 ; /* bits 5:1 */
    cs_uint32 agg_qs_thrshld_en    :  1 ; /* bits 6:6 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 rpt_format           :  2 ; /* bits 9:8 */
    cs_uint32 rpt_number           :  2 ; /* bits 11:10 */
    cs_uint32 rpt_order            :  1 ; /* bits 12:12 */
    cs_uint32 force_quick_rpt_en   :  1 ; /* bits 13:13 */
    cs_uint32 fill_unrpt_pkt_en    :  1 ; /* bits 14:14 */
    cs_uint32 ignor_rpt_en         :  1 ; /* bits 15:15 */
    cs_uint32 single_window_en     :  1 ; /* bits 16:16 */
    cs_uint32 rsrvd2               : 15 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_LYNXD_MPCP_CFG_t;





#define BM_MPCP_RPT_BIT_MAP                      0x09030
#define BM_MPCP_RPT_BIT_MAP_dft                  0x00001f1f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 q_set1               :  8 ; /* bits 15:8 */
    cs_uint32 q_set2               :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 q_set2               :  8 ; /* bits 7:0 */
    cs_uint32 q_set1               :  8 ; /* bits 15:8 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_RPT_BIT_MAP_t;





#define BM_US_Q0_START                           0x09031
#define BM_US_Q0_START_dft                       0x3ae09d00




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q0_START_t;





#define BM_US_Q0_END                             0x09032
#define BM_US_Q0_END_dft                         0x4afec7ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q0_END_t;





#define BM_US_Q1_START                           0x09033
#define BM_US_Q1_START_dft                       0x2ac07200




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q1_START_t;





#define BM_US_Q1_END                             0x09034
#define BM_US_Q1_END_dft                         0x3ade9cff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q1_END_t;





#define BM_US_Q2_START                           0x09035
#define BM_US_Q2_START_dft                       0x1aa04700




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q2_START_t;





#define BM_US_Q2_END                             0x09036
#define BM_US_Q2_END_dft                         0x2abe71ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q2_END_t;





#define BM_US_Q3_START                           0x09037
#define BM_US_Q3_START_dft                       0x0a801c00




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q3_START_t;





#define BM_US_Q3_END                             0x09038
#define BM_US_Q3_END_dft                         0x1a9e46ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q3_END_t;





#define BM_US_Q4_START                           0x09039
#define BM_US_Q4_START_dft                       0x07801400




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q4_START_t;





#define BM_US_Q4_END                             0x0903a
#define BM_US_Q4_END_dft                         0x0a7e1bff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q4_END_t;





#define BM_DS_Q0_START                           0x0903b
#define BM_DS_Q0_START_dft                       0x7e615100




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DS_Q0_START_t;





#define BM_DS_Q0_END                             0x0903c
#define BM_DS_Q0_END_dft                         0x8e7f7bff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DS_Q0_END_t;





#define BM_DS_Q1_START                           0x0903d
#define BM_DS_Q1_START_dft                       0x6e412600




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DS_Q1_START_t;





#define BM_DS_Q1_END                             0x0903e
#define BM_DS_Q1_END_dft                         0x7e5f50ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DS_Q1_END_t;





#define BM_DS_Q2_START                           0x0903f
#define BM_DS_Q2_START_dft                       0x5e20fb00




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DS_Q2_START_t;





#define BM_DS_Q2_END                             0x09040
#define BM_DS_Q2_END_dft                         0x6e3f25ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DS_Q2_END_t;





#define BM_DS_Q3_START                           0x09041
#define BM_DS_Q3_START_dft                       0x4e00d000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DS_Q3_START_t;





#define BM_DS_Q3_END                             0x09042
#define BM_DS_Q3_END_dft                         0x5e1efaff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DS_Q3_END_t;





#define BM_DS_Q4_START                           0x09043
#define BM_DS_Q4_START_dft                       0x4b00c800




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DS_Q4_START_t;





#define BM_DS_Q4_END                             0x09044
#define BM_DS_Q4_END_dft                         0x4dfecfff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DS_Q4_END_t;





#define BM_MA_Q0_END                             0x09045
#define BM_MA_Q0_END_dft                         0x077e13ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_Q0_END_t;





#define BM_MA_Q1_END                             0x09046
#define BM_MA_Q1_END_dft                         0x06be11ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_Q1_END_t;





#define BM_MA_Q2_END                             0x09047
#define BM_MA_Q2_END_dft                         0x05fe0fff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_Q2_END_t;





#define BM_MA_Q3_END                             0x09048
#define BM_MA_Q3_END_dft                         0x053e0dff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_Q3_END_t;





#define BM_MA_Q4_END                             0x09049
#define BM_MA_Q4_END_dft                         0x047e0bff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_Q4_END_t;





#define BM_MA_Q5_END                             0x0904a
#define BM_MA_Q5_END_dft                         0x03be09ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_Q5_END_t;





#define BM_MA_Q6_END                             0x0904b
#define BM_MA_Q6_END_dft                         0x02fe07ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_Q6_END_t;





#define BM_MA_Q7_END                             0x0904c
#define BM_MA_Q7_END_dft                         0x023e05ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_Q7_END_t;





#define BM_MA_Q7_START                           0x0904d
#define BM_MA_Q7_START_dft                       0x01800400




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MA_Q7_START_t;





#define BM_CPU_ACCESS_OFFSET                     0x0904e
#define BM_CPU_ACCESS_OFFSET_dft                 0x00010000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 addr                 : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 addr                 : 17 ; /* bits 16:0 */
    cs_uint32 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_ACCESS_OFFSET_t;





#define BM_US_Q0_FLOW_CTRL                       0x0904f
#define BM_US_Q0_FLOW_CTRL_dft                   0x00400000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 en                   :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 u_thrsld             : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 l_thrsld             : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 l_thrsld             : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 u_thrsld             : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 en                   :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q0_FLOW_CTRL_t;





#define BM_US_Q1_FLOW_CTRL                       0x09050
#define BM_US_Q1_FLOW_CTRL_dft                   0x00400000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 en                   :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 u_thrsld             : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 l_thrsld             : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 l_thrsld             : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 u_thrsld             : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 en                   :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q1_FLOW_CTRL_t;





#define BM_US_Q2_FLOW_CTRL                       0x09051
#define BM_US_Q2_FLOW_CTRL_dft                   0x00400000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 en                   :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 u_thrsld             : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 l_thrsld             : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 l_thrsld             : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 u_thrsld             : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 en                   :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q2_FLOW_CTRL_t;





#define BM_US_Q3_FLOW_CTRL                       0x09052
#define BM_US_Q3_FLOW_CTRL_dft                   0x00400000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 en                   :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 u_thrsld             : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 l_thrsld             : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 l_thrsld             : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 u_thrsld             : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 en                   :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q3_FLOW_CTRL_t;





#define BM_US_Q4_FLOW_CTRL                       0x09053
#define BM_US_Q4_FLOW_CTRL_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 en                   :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 u_thrsld             : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 l_thrsld             : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 l_thrsld             : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 u_thrsld             : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 en                   :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_US_Q4_FLOW_CTRL_t;





#define BM_OAM_PKT_EN                            0x09054
#define BM_OAM_PKT_EN_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 oam_en               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 oam_en               :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_OAM_PKT_EN_t;





#define BM_OAM_PKT_HEADER                        0x09055
#define BM_OAM_PKT_HEADER_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  7 ;
    cs_uint32 header               : 25 ; /* bits 24:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 header               : 25 ; /* bits 24:0 */
    cs_uint32 rsrvd1               :  7 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_OAM_PKT_HEADER_t;





#define BM_Q_FLUSH_EN                            0x09056
#define BM_Q_FLUSH_EN_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 11 ;
    cs_uint32 ds_ptr_updt          :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 us_ptr_updt          :  5 ; /* bits 12:8 */
    cs_uint32 ma_ptr_updt          :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               :  7 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  7 ;
    cs_uint32 ma_ptr_updt          :  1 ; /* bits 7:7 */
    cs_uint32 us_ptr_updt          :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 ds_ptr_updt          :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd3               : 11 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_Q_FLUSH_EN_t;





#define BM_Q_FLUSH_CTRL                          0x09057
#define BM_Q_FLUSH_CTRL_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 26 ;
    cs_uint32 pid                  :  3 ; /* bits 5:3 */
    cs_uint32 queue                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 queue                :  3 ; /* bits 2:0 */
    cs_uint32 pid                  :  3 ; /* bits 5:3 */
    cs_uint32 rsrvd1               : 26 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_Q_FLUSH_CTRL_t;





#define BM_MPCP_QSET_2                           0x09058
#define BM_MPCP_QSET_2_dft                       0xffff0800




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 zqs_thrshld          : 16 ; /* bits 31:16 */
    cs_uint32 rpt_thrshld          : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rpt_thrshld          : 16 ; /* bits 15:0 */
    cs_uint32 zqs_thrshld          : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_QSET_2_t;





#define BM_TEST0                                 0x09059
#define BM_TEST0_dft                             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 test1                : 16 ; /* bits 31:16 */
    cs_uint32 test0                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 test0                : 16 ; /* bits 15:0 */
    cs_uint32 test1                : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_TEST0_t;





#define BM_TEST1                                 0x0905a
#define BM_TEST1_dft                             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 test1                : 16 ; /* bits 31:16 */
    cs_uint32 test0                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 test0                : 16 ; /* bits 15:0 */
    cs_uint32 test1                : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_TEST1_t;





#define BM_INTERRUPT_E                           0x0905b
#define BM_INTERRUPT_E_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pon_rx_no_buf_drop_intE :  1 ; /* bits 31:31 */
    cs_uint32 pon_rx_ql_drop_intE  :  1 ; /* bits 30:30 */
    cs_uint32 pon_rx_dp_lmt_drop_intE :  1 ; /* bits 29:29 */
    cs_uint32 pon_rx_non_cpu_drop_intE :  1 ; /* bits 28:28 */
    cs_uint32 ge_rx_no_buf_drop_intE :  1 ; /* bits 27:27 */
    cs_uint32 ge_rx_ql_drop_intE   :  1 ; /* bits 26:26 */
    cs_uint32 ge_rx_dp_lmt_drop_intE :  1 ; /* bits 25:25 */
    cs_uint32 ge_rx_non_cpu_drop_intE :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 pon_eee_wake_up_intE :  1 ; /* bits 20:20 */
    cs_uint32 pon_eee_sleep_intE   :  1 ; /* bits 19:19 */
    cs_uint32 eee_wake_up_intE     :  1 ; /* bits 18:18 */
    cs_uint32 eee_sleep_intE       :  1 ; /* bits 17:17 */
    cs_uint32 header_error_corrected_intE :  1 ; /* bits 16:16 */
    cs_uint32 unexpected_quick_report_dectection_intE :  1 ; /* bits 15:15 */
    cs_uint32 hdmem_collision_dectection_intE :  1 ; /* bits 14:14 */
    cs_uint32 q2mem_collision_dectection_intE :  1 ; /* bits 13:13 */
    cs_uint32 ge_rmu_non_idle_aft_eop_err_intE :  1 ; /* bits 12:12 */
    cs_uint32 ge_rmu_fsm_non_idle_err_intE :  1 ; /* bits 11:11 */
    cs_uint32 ge_rmu_collision_err_intE :  1 ; /* bits 10:10 */
    cs_uint32 pon_rmu_non_idle_aft_eop_err_intE :  1 ; /* bits 9:9 */
    cs_uint32 pon_rmu_fsm_non_idle_err_intE :  1 ; /* bits 8:8 */
    cs_uint32 pon_rmu_collision_err_intE :  1 ; /* bits 7:7 */
    cs_uint32 cpu_rmu_non_idle_aft_eop_err_intE :  1 ; /* bits 6:6 */
    cs_uint32 cpu_rmu_fsm_non_idle_err_intE :  1 ; /* bits 5:5 */
    cs_uint32 cpu_rmu_collision_err_intE :  1 ; /* bits 4:4 */
    cs_uint32 header_error_intE    :  1 ; /* bits 3:3 */
    cs_uint32 cpu_intE             :  1 ; /* bits 2:2 */
    cs_uint32 pon_intE             :  1 ; /* bits 1:1 */
    cs_uint32 ge_intE              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ge_intE              :  1 ; /* bits 0:0 */
    cs_uint32 pon_intE             :  1 ; /* bits 1:1 */
    cs_uint32 cpu_intE             :  1 ; /* bits 2:2 */
    cs_uint32 header_error_intE    :  1 ; /* bits 3:3 */
    cs_uint32 cpu_rmu_collision_err_intE :  1 ; /* bits 4:4 */
    cs_uint32 cpu_rmu_fsm_non_idle_err_intE :  1 ; /* bits 5:5 */
    cs_uint32 cpu_rmu_non_idle_aft_eop_err_intE :  1 ; /* bits 6:6 */
    cs_uint32 pon_rmu_collision_err_intE :  1 ; /* bits 7:7 */
    cs_uint32 pon_rmu_fsm_non_idle_err_intE :  1 ; /* bits 8:8 */
    cs_uint32 pon_rmu_non_idle_aft_eop_err_intE :  1 ; /* bits 9:9 */
    cs_uint32 ge_rmu_collision_err_intE :  1 ; /* bits 10:10 */
    cs_uint32 ge_rmu_fsm_non_idle_err_intE :  1 ; /* bits 11:11 */
    cs_uint32 ge_rmu_non_idle_aft_eop_err_intE :  1 ; /* bits 12:12 */
    cs_uint32 q2mem_collision_dectection_intE :  1 ; /* bits 13:13 */
    cs_uint32 hdmem_collision_dectection_intE :  1 ; /* bits 14:14 */
    cs_uint32 unexpected_quick_report_dectection_intE :  1 ; /* bits 15:15 */
    cs_uint32 header_error_corrected_intE :  1 ; /* bits 16:16 */
    cs_uint32 eee_sleep_intE       :  1 ; /* bits 17:17 */
    cs_uint32 eee_wake_up_intE     :  1 ; /* bits 18:18 */
    cs_uint32 pon_eee_sleep_intE   :  1 ; /* bits 19:19 */
    cs_uint32 pon_eee_wake_up_intE :  1 ; /* bits 20:20 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 ge_rx_non_cpu_drop_intE :  1 ; /* bits 24:24 */
    cs_uint32 ge_rx_dp_lmt_drop_intE :  1 ; /* bits 25:25 */
    cs_uint32 ge_rx_ql_drop_intE   :  1 ; /* bits 26:26 */
    cs_uint32 ge_rx_no_buf_drop_intE :  1 ; /* bits 27:27 */
    cs_uint32 pon_rx_non_cpu_drop_intE :  1 ; /* bits 28:28 */
    cs_uint32 pon_rx_dp_lmt_drop_intE :  1 ; /* bits 29:29 */
    cs_uint32 pon_rx_ql_drop_intE  :  1 ; /* bits 30:30 */
    cs_uint32 pon_rx_no_buf_drop_intE :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_INTERRUPT_E_t;





#define BM_INTERRUPT                             0x0905c
#define BM_INTERRUPT_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pon_rx_no_buf_drop_intI :  1 ; /* bits 31:31 */
    cs_uint32 pon_rx_ql_drop_intI  :  1 ; /* bits 30:30 */
    cs_uint32 pon_rx_dp_lmt_drop_intI :  1 ; /* bits 29:29 */
    cs_uint32 pon_rx_non_cpu_drop_intI :  1 ; /* bits 28:28 */
    cs_uint32 ge_rx_no_buf_drop_intI :  1 ; /* bits 27:27 */
    cs_uint32 ge_rx_ql_drop_intI   :  1 ; /* bits 26:26 */
    cs_uint32 ge_rx_dp_lmt_drop_intI :  1 ; /* bits 25:25 */
    cs_uint32 ge_rx_non_cpu_drop_intI :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 pon_eee_wake_up_intI :  1 ; /* bits 20:20 */
    cs_uint32 pon_eee_sleep_intI   :  1 ; /* bits 19:19 */
    cs_uint32 eee_wake_up_intI     :  1 ; /* bits 18:18 */
    cs_uint32 eee_sleep_intI       :  1 ; /* bits 17:17 */
    cs_uint32 header_error_corrected_intI :  1 ; /* bits 16:16 */
    cs_uint32 unexpected_quick_report_dectection_intI :  1 ; /* bits 15:15 */
    cs_uint32 hdmem_collision_dectection_intI :  1 ; /* bits 14:14 */
    cs_uint32 q2mem_collision_dectection_intI :  1 ; /* bits 13:13 */
    cs_uint32 ge_rmu_non_idle_aft_eop_err_intI :  1 ; /* bits 12:12 */
    cs_uint32 ge_rmu_fsm_non_idle_err_intI :  1 ; /* bits 11:11 */
    cs_uint32 ge_rmu_collision_err_intI :  1 ; /* bits 10:10 */
    cs_uint32 pon_rmu_non_idle_aft_eop_err_intI :  1 ; /* bits 9:9 */
    cs_uint32 pon_rmu_fsm_non_idle_err_intI :  1 ; /* bits 8:8 */
    cs_uint32 pon_rmu_collision_err_intI :  1 ; /* bits 7:7 */
    cs_uint32 cpu_rmu_non_idle_aft_eop_err_intI :  1 ; /* bits 6:6 */
    cs_uint32 cpu_rmu_fsm_non_idle_err_intI :  1 ; /* bits 5:5 */
    cs_uint32 cpu_rmu_collision_err_intI :  1 ; /* bits 4:4 */
    cs_uint32 header_error_intI    :  1 ; /* bits 3:3 */
    cs_uint32 cpu_intI             :  1 ; /* bits 2:2 */
    cs_uint32 pon_intI             :  1 ; /* bits 1:1 */
    cs_uint32 ge_intI              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ge_intI              :  1 ; /* bits 0:0 */
    cs_uint32 pon_intI             :  1 ; /* bits 1:1 */
    cs_uint32 cpu_intI             :  1 ; /* bits 2:2 */
    cs_uint32 header_error_intI    :  1 ; /* bits 3:3 */
    cs_uint32 cpu_rmu_collision_err_intI :  1 ; /* bits 4:4 */
    cs_uint32 cpu_rmu_fsm_non_idle_err_intI :  1 ; /* bits 5:5 */
    cs_uint32 cpu_rmu_non_idle_aft_eop_err_intI :  1 ; /* bits 6:6 */
    cs_uint32 pon_rmu_collision_err_intI :  1 ; /* bits 7:7 */
    cs_uint32 pon_rmu_fsm_non_idle_err_intI :  1 ; /* bits 8:8 */
    cs_uint32 pon_rmu_non_idle_aft_eop_err_intI :  1 ; /* bits 9:9 */
    cs_uint32 ge_rmu_collision_err_intI :  1 ; /* bits 10:10 */
    cs_uint32 ge_rmu_fsm_non_idle_err_intI :  1 ; /* bits 11:11 */
    cs_uint32 ge_rmu_non_idle_aft_eop_err_intI :  1 ; /* bits 12:12 */
    cs_uint32 q2mem_collision_dectection_intI :  1 ; /* bits 13:13 */
    cs_uint32 hdmem_collision_dectection_intI :  1 ; /* bits 14:14 */
    cs_uint32 unexpected_quick_report_dectection_intI :  1 ; /* bits 15:15 */
    cs_uint32 header_error_corrected_intI :  1 ; /* bits 16:16 */
    cs_uint32 eee_sleep_intI       :  1 ; /* bits 17:17 */
    cs_uint32 eee_wake_up_intI     :  1 ; /* bits 18:18 */
    cs_uint32 pon_eee_sleep_intI   :  1 ; /* bits 19:19 */
    cs_uint32 pon_eee_wake_up_intI :  1 ; /* bits 20:20 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 ge_rx_non_cpu_drop_intI :  1 ; /* bits 24:24 */
    cs_uint32 ge_rx_dp_lmt_drop_intI :  1 ; /* bits 25:25 */
    cs_uint32 ge_rx_ql_drop_intI   :  1 ; /* bits 26:26 */
    cs_uint32 ge_rx_no_buf_drop_intI :  1 ; /* bits 27:27 */
    cs_uint32 pon_rx_non_cpu_drop_intI :  1 ; /* bits 28:28 */
    cs_uint32 pon_rx_dp_lmt_drop_intI :  1 ; /* bits 29:29 */
    cs_uint32 pon_rx_ql_drop_intI  :  1 ; /* bits 30:30 */
    cs_uint32 pon_rx_no_buf_drop_intI :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_INTERRUPT_t;





#define BM_GE_RX_PKT_CNTR                        0x0905d
#define BM_GE_RX_PKT_CNTR_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_RX_PKT_CNTR_t;





#define BM_PON_RX_PKT_CNTR                       0x0905e
#define BM_PON_RX_PKT_CNTR_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_RX_PKT_CNTR_t;





#define BM_CPU_RX_PKT_CNTR                       0x0905f
#define BM_CPU_RX_PKT_CNTR_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_RX_PKT_CNTR_t;





#define BM_GE_TX_PKT_CNTR                        0x09060
#define BM_GE_TX_PKT_CNTR_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_TX_PKT_CNTR_t;





#define BM_PON_TX_PKT_CNTR                       0x09061
#define BM_PON_TX_PKT_CNTR_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_TX_PKT_CNTR_t;





#define BM_CPU_TX_PKT_CNTR                       0x09062
#define BM_CPU_TX_PKT_CNTR_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_TX_PKT_CNTR_t;





#define BM_GE_DROP_PKT_CNTR                      0x09063
#define BM_GE_DROP_PKT_CNTR_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_DROP_PKT_CNTR_t;





#define BM_PON_DROP_PKT_CNTR                     0x09064
#define BM_PON_DROP_PKT_CNTR_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_DROP_PKT_CNTR_t;





#define BM_CPU_DROP_PKT_CNTR                     0x09065
#define BM_CPU_DROP_PKT_CNTR_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_DROP_PKT_CNTR_t;





#define BM_GE_FE_DROP_PKT_CNTR                   0x09066
#define BM_GE_FE_DROP_PKT_CNTR_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_FE_DROP_PKT_CNTR_t;





#define BM_PON_FE_DROP_PKT_CNTR                  0x09067
#define BM_PON_FE_DROP_PKT_CNTR_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_FE_DROP_PKT_CNTR_t;





#define BM_CPU_FE_DROP_PKT_CNTR                  0x09068
#define BM_CPU_FE_DROP_PKT_CNTR_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_FE_DROP_PKT_CNTR_t;





#define BM_GE_ABR_DROP_PKT_CNTR                  0x09069
#define BM_GE_ABR_DROP_PKT_CNTR_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_ABR_DROP_PKT_CNTR_t;





#define BM_PON_ABR_DROP_PKT_CNTR                 0x0906a
#define BM_PON_ABR_DROP_PKT_CNTR_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_ABR_DROP_PKT_CNTR_t;





#define BM_MPCP_DROP_PKT_CNTR                    0x0906b
#define BM_MPCP_DROP_PKT_CNTR_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_DROP_PKT_CNTR_t;





#define BM_SHARED_BUFFER_CNTR                    0x0906c
#define BM_SHARED_BUFFER_CNTR_dft                0x3fff0000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 eq_cntr              : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 eq_cntr              : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_SHARED_BUFFER_CNTR_t;





#define BM_GE_TX_BUFFER_CNTR                     0x0906d
#define BM_GE_TX_BUFFER_CNTR_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_TX_BUFFER_CNTR_t;





#define BM_PON_TX_BUFFER_CNTR                    0x0906e
#define BM_PON_TX_BUFFER_CNTR_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_TX_BUFFER_CNTR_t;





#define BM_CPU_TX_BUFFER_CNTR                    0x0906f
#define BM_CPU_TX_BUFFER_CNTR_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_TX_BUFFER_CNTR_t;





#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS8      0x09070
#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS8_dft  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS8_t;





#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS7      0x09071
#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS7_dft  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS7_t;





#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS6      0x09072
#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS6_dft  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS6_t;





#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS5      0x09073
#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS5_dft  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS5_t;





#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS4      0x09074
#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS4_dft  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS4_t;





#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS3      0x09075
#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS3_dft  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS3_t;





#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS2      0x09076
#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS2_dft  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS2_t;





#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS1      0x09077
#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS1_dft  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS1_t;





#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS0      0x09078
#define BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS0_dft  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS0_t;





#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS8     0x09079
#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS8_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS8_t;





#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS7     0x0907a
#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS7_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS7_t;





#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS6     0x0907b
#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS6_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS6_t;





#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS5     0x0907c
#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS5_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS5_t;





#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS4     0x0907d
#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS4_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS4_t;





#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS3     0x0907e
#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS3_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS3_t;





#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS2     0x0907f
#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS2_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS2_t;





#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS1     0x09080
#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS1_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS1_t;





#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS0     0x09081
#define BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS0_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS0_t;





#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS8     0x09082
#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS8_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS8_t;





#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS7     0x09083
#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS7_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS7_t;





#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS6     0x09084
#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS6_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS6_t;





#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS5     0x09085
#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS5_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS5_t;





#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS4     0x09086
#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS4_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS4_t;





#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS3     0x09087
#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS3_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS3_t;





#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS2     0x09088
#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS2_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS2_t;





#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS1     0x09089
#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS1_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS1_t;





#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS0     0x0908a
#define BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS0_dft 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 18 ;
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               : 18 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS0_t;





#define BM_GE_HD_ERR_CNTR                        0x0908b
#define BM_GE_HD_ERR_CNTR_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 cntr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_HD_ERR_CNTR_t;





#define BM_PON_HD_ERR_CNTR                       0x0908c
#define BM_PON_HD_ERR_CNTR_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 cntr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_HD_ERR_CNTR_t;





#define BM_CPU_HD_ERR_CNTR                       0x0908d
#define BM_CPU_HD_ERR_CNTR_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 cntr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_HD_ERR_CNTR_t;





#define BM_GE_PACKET_HEADER_INFO                 0x0908e
#define BM_GE_PACKET_HEADER_INFO_dft             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_PACKET_HEADER_INFO_t;





#define BM_PON_PACKET_HEADER_INFO                0x0908f
#define BM_PON_PACKET_HEADER_INFO_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_PACKET_HEADER_INFO_t;





#define BM_CPU_PACKET_HEADER_INFO                0x09090
#define BM_CPU_PACKET_HEADER_INFO_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 info                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_CPU_PACKET_HEADER_INFO_t;





#define BM_MPCP_WINDOW_STS                       0x09091
#define BM_MPCP_WINDOW_STS_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mpcp_win_size        : 16 ; /* bits 31:16 */
    cs_uint32 mpcp_win_valid       :  1 ; /* bits 15:15 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 mpcp_win_avl         : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mpcp_win_avl         : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 mpcp_win_valid       :  1 ; /* bits 15:15 */
    cs_uint32 mpcp_win_size        : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_MPCP_WINDOW_STS_t;





#define BM_RMU_PT0_DROP_CNT1                     0x09092
#define BM_RMU_PT0_DROP_CNT1_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_RMU_PT0_DROP_CNT1_t;





#define BM_RMU_PT0_DROP_CNT2                     0x09093
#define BM_RMU_PT0_DROP_CNT2_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_RMU_PT0_DROP_CNT2_t;





#define BM_RMU_PT0_DROP_CNT3                     0x09094
#define BM_RMU_PT0_DROP_CNT3_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_RMU_PT0_DROP_CNT3_t;





#define BM_RMU_PT1_DROP_CNT1                     0x09095
#define BM_RMU_PT1_DROP_CNT1_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_RMU_PT1_DROP_CNT1_t;





#define BM_RMU_PT1_DROP_CNT2                     0x09096
#define BM_RMU_PT1_DROP_CNT2_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_RMU_PT1_DROP_CNT2_t;





#define BM_RMU_PT1_DROP_CNT3                     0x09097
#define BM_RMU_PT1_DROP_CNT3_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_RMU_PT1_DROP_CNT3_t;





#define BM_RMU_PT2_DROP_CNT1                     0x09098
#define BM_RMU_PT2_DROP_CNT1_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_RMU_PT2_DROP_CNT1_t;





#define BM_RMU_PT2_DROP_CNT2                     0x09099
#define BM_RMU_PT2_DROP_CNT2_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_RMU_PT2_DROP_CNT2_t;





#define BM_RMU_PT2_DROP_CNT3                     0x0909a
#define BM_RMU_PT2_DROP_CNT3_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt                  : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_RMU_PT2_DROP_CNT3_t;





#define BM_LYNXD_TMU_STATUS                      0x0909b
#define BM_LYNXD_TMU_STATUS_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               :  8 ;
    cs_uint32 status               : 24 ; /* bits 23:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 status               : 24 ; /* bits 23:0 */
    cs_uint32 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_LYNXD_TMU_STATUS_t;





#define BM_P1_Q0_START                           0x0909c
#define BM_P1_Q0_START_dft                       0x3ae09d00




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_P1_Q0_START_t;





#define BM_P1_Q0_END                             0x0909d
#define BM_P1_Q0_END_dft                         0x4afec7ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_P1_Q0_END_t;





#define BM_P1_Q1_START                           0x0909e
#define BM_P1_Q1_START_dft                       0x2ac07200




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_P1_Q1_START_t;





#define BM_P1_Q1_END                             0x0909f
#define BM_P1_Q1_END_dft                         0x3ade9cff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_P1_Q1_END_t;





#define BM_P1_Q2_START                           0x090a0
#define BM_P1_Q2_START_dft                       0x1aa04700




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_P1_Q2_START_t;





#define BM_P1_Q2_END                             0x090a1
#define BM_P1_Q2_END_dft                         0x2abe71ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_P1_Q2_END_t;





#define BM_P1_Q3_START                           0x090a2
#define BM_P1_Q3_START_dft                       0x0a801c00




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_P1_Q3_START_t;





#define BM_P1_Q3_END                             0x090a3
#define BM_P1_Q3_END_dft                         0x1a9e46ff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_P1_Q3_END_t;





#define BM_P1_Q4_START                           0x090a4
#define BM_P1_Q4_START_dft                       0x07801400




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_P1_Q4_START_t;





#define BM_P1_Q4_END                             0x090a5
#define BM_P1_Q4_END_dft                         0x0a7e1bff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 header               : 15 ; /* bits 31:17 */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 packet               : 17 ; /* bits 16:0 */
    cs_uint32 header               : 15 ; /* bits 31:17 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_P1_Q4_END_t;





#define BM_GRANT_MONITOR                         0x090a6
#define BM_GRANT_MONITOR_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 grant_size           : 16 ; /* bits 31:16 */
    cs_uint32 gg_cur_agg           : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 gg_cur_agg           : 16 ; /* bits 15:0 */
    cs_uint32 grant_size           : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GRANT_MONITOR_t;





#define BM_EEE_ECFG                              0x090a7
#define BM_EEE_ECFG_dft                          0x000000a0




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 23 ;
    cs_uint32 inv_sleep            :  1 ; /* bits 8:8 */
    cs_uint32 cmd_mode             :  4 ; /* bits 7:4 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 eee_on               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 eee_on               :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 cmd_mode             :  4 ; /* bits 7:4 */
    cs_uint32 inv_sleep            :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd2               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_ECFG_t;





#define BM_EEE_SW_CTRL                           0x090a8
#define BM_EEE_SW_CTRL_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 sw_wake              :  1 ; /* bits 1:1 */
    cs_uint32 sw_sleep             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sw_sleep             :  1 ; /* bits 0:0 */
    cs_uint32 sw_wake              :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_SW_CTRL_t;





#define BM_EEE_STAT                              0x090a9
#define BM_EEE_STAT_dft                          0x00010200




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               :  8 ;
    cs_uint32 sleep                :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 eee_sm_stat          :  6 ; /* bits 21:16 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 rx_in_pause          :  1 ; /* bits 11:11 */
    cs_uint32 in_pause             :  1 ; /* bits 10:10 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 9:9 */
    cs_uint32 surpass_thres8       :  1 ; /* bits 8:8 */
    cs_uint32 surpass_thres7       :  1 ; /* bits 7:7 */
    cs_uint32 surpass_thres6       :  1 ; /* bits 6:6 */
    cs_uint32 surpass_thres5       :  1 ; /* bits 5:5 */
    cs_uint32 surpass_thres4       :  1 ; /* bits 4:4 */
    cs_uint32 surpass_thres3       :  1 ; /* bits 3:3 */
    cs_uint32 surpass_thres2       :  1 ; /* bits 2:2 */
    cs_uint32 surpass_thres1       :  1 ; /* bits 1:1 */
    cs_uint32 surpass_thres0       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 surpass_thres0       :  1 ; /* bits 0:0 */
    cs_uint32 surpass_thres1       :  1 ; /* bits 1:1 */
    cs_uint32 surpass_thres2       :  1 ; /* bits 2:2 */
    cs_uint32 surpass_thres3       :  1 ; /* bits 3:3 */
    cs_uint32 surpass_thres4       :  1 ; /* bits 4:4 */
    cs_uint32 surpass_thres5       :  1 ; /* bits 5:5 */
    cs_uint32 surpass_thres6       :  1 ; /* bits 6:6 */
    cs_uint32 surpass_thres7       :  1 ; /* bits 7:7 */
    cs_uint32 surpass_thres8       :  1 ; /* bits 8:8 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 9:9 */
    cs_uint32 in_pause             :  1 ; /* bits 10:10 */
    cs_uint32 rx_in_pause          :  1 ; /* bits 11:11 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 eee_sm_stat          :  6 ; /* bits 21:16 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 sleep                :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd3               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_STAT_t;





#define BM_EEE_SLEEP_DELAY                       0x090aa
#define BM_EEE_SLEEP_DELAY_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  8 ;
    cs_uint32 eee_min_sleep_time   :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 eee_sleep_timeout    : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 eee_sleep_timeout    : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 eee_min_sleep_time   :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd2               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_SLEEP_DELAY_t;





#define BM_EEE_WAKE_DELAY                        0x090ab
#define BM_EEE_WAKE_DELAY_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  8 ;
    cs_uint32 eee_min_wake_time    :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 eee_wake_timeout     : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 eee_wake_timeout     : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 eee_min_wake_time    :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd2               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_WAKE_DELAY_t;





#define BM_EEE_QUEUE0                            0x090ac
#define BM_EEE_QUEUE0_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_QUEUE0_t;





#define BM_EEE_QUEUE1                            0x090ad
#define BM_EEE_QUEUE1_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_QUEUE1_t;





#define BM_EEE_QUEUE2                            0x090ae
#define BM_EEE_QUEUE2_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_QUEUE2_t;





#define BM_EEE_QUEUE3                            0x090af
#define BM_EEE_QUEUE3_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_QUEUE3_t;





#define BM_EEE_QUEUE4                            0x090b0
#define BM_EEE_QUEUE4_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_QUEUE4_t;





#define BM_EEE_QUEUE5                            0x090b1
#define BM_EEE_QUEUE5_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_QUEUE5_t;





#define BM_EEE_QUEUE6                            0x090b2
#define BM_EEE_QUEUE6_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_QUEUE6_t;





#define BM_EEE_QUEUE7                            0x090b3
#define BM_EEE_QUEUE7_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_QUEUE7_t;





#define BM_EEE_QUEUE8                            0x090b4
#define BM_EEE_QUEUE8_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_QUEUE8_t;





#define BM_EEE_THRESHOLD1                        0x090b5
#define BM_EEE_THRESHOLD1_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 eee_thres            :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 eee_thres            :  9 ; /* bits 8:0 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_THRESHOLD1_t;





#define BM_EEE_THRESHOLD2                        0x090b6
#define BM_EEE_THRESHOLD2_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 eee_thres            :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 eee_thres            :  9 ; /* bits 8:0 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_THRESHOLD2_t;





#define BM_EEE_THRESHOLD3                        0x090b7
#define BM_EEE_THRESHOLD3_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 eee_thres            :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 eee_thres            :  9 ; /* bits 8:0 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_EEE_THRESHOLD3_t;





#define BM_PON_EEE_ECFG                          0x090b8
#define BM_PON_EEE_ECFG_dft                      0x000000a0




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 23 ;
    cs_uint32 inv_sleep            :  1 ; /* bits 8:8 */
    cs_uint32 cmd_mode             :  4 ; /* bits 7:4 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 eee_on               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 eee_on               :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 cmd_mode             :  4 ; /* bits 7:4 */
    cs_uint32 inv_sleep            :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd2               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_ECFG_t;





#define BM_PON_EEE_SW_CTRL                       0x090b9
#define BM_PON_EEE_SW_CTRL_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 sw_wake              :  1 ; /* bits 1:1 */
    cs_uint32 sw_sleep             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sw_sleep             :  1 ; /* bits 0:0 */
    cs_uint32 sw_wake              :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_SW_CTRL_t;





#define BM_PON_EEE_STAT                          0x090ba
#define BM_PON_EEE_STAT_dft                      0x00010200




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               :  8 ;
    cs_uint32 sleep                :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 eee_sm_stat          :  6 ; /* bits 21:16 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 rx_in_pause          :  1 ; /* bits 11:11 */
    cs_uint32 in_pause             :  1 ; /* bits 10:10 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 9:9 */
    cs_uint32 surpass_thres8       :  1 ; /* bits 8:8 */
    cs_uint32 surpass_thres7       :  1 ; /* bits 7:7 */
    cs_uint32 surpass_thres6       :  1 ; /* bits 6:6 */
    cs_uint32 surpass_thres5       :  1 ; /* bits 5:5 */
    cs_uint32 surpass_thres4       :  1 ; /* bits 4:4 */
    cs_uint32 surpass_thres3       :  1 ; /* bits 3:3 */
    cs_uint32 surpass_thres2       :  1 ; /* bits 2:2 */
    cs_uint32 surpass_thres1       :  1 ; /* bits 1:1 */
    cs_uint32 surpass_thres0       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 surpass_thres0       :  1 ; /* bits 0:0 */
    cs_uint32 surpass_thres1       :  1 ; /* bits 1:1 */
    cs_uint32 surpass_thres2       :  1 ; /* bits 2:2 */
    cs_uint32 surpass_thres3       :  1 ; /* bits 3:3 */
    cs_uint32 surpass_thres4       :  1 ; /* bits 4:4 */
    cs_uint32 surpass_thres5       :  1 ; /* bits 5:5 */
    cs_uint32 surpass_thres6       :  1 ; /* bits 6:6 */
    cs_uint32 surpass_thres7       :  1 ; /* bits 7:7 */
    cs_uint32 surpass_thres8       :  1 ; /* bits 8:8 */
    cs_uint32 tx_fifo_empty        :  1 ; /* bits 9:9 */
    cs_uint32 in_pause             :  1 ; /* bits 10:10 */
    cs_uint32 rx_in_pause          :  1 ; /* bits 11:11 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 eee_sm_stat          :  6 ; /* bits 21:16 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 sleep                :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd3               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_STAT_t;





#define BM_PON_EEE_SLEEP_DELAY                   0x090bb
#define BM_PON_EEE_SLEEP_DELAY_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  8 ;
    cs_uint32 eee_min_sleep_time   :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 eee_sleep_timeout    : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 eee_sleep_timeout    : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 eee_min_sleep_time   :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd2               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_SLEEP_DELAY_t;





#define BM_PON_EEE_WAKE_DELAY                    0x090bc
#define BM_PON_EEE_WAKE_DELAY_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  8 ;
    cs_uint32 eee_min_wake_time    :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 eee_wake_timeout     : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 eee_wake_timeout     : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 eee_min_wake_time    :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd2               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_WAKE_DELAY_t;





#define BM_PON_EEE_QUEUE0                        0x090bd
#define BM_PON_EEE_QUEUE0_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_QUEUE0_t;





#define BM_PON_EEE_QUEUE1                        0x090be
#define BM_PON_EEE_QUEUE1_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_QUEUE1_t;





#define BM_PON_EEE_QUEUE2                        0x090bf
#define BM_PON_EEE_QUEUE2_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_QUEUE2_t;





#define BM_PON_EEE_QUEUE3                        0x090c0
#define BM_PON_EEE_QUEUE3_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_QUEUE3_t;





#define BM_PON_EEE_QUEUE4                        0x090c1
#define BM_PON_EEE_QUEUE4_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_QUEUE4_t;





#define BM_PON_EEE_QUEUE5                        0x090c2
#define BM_PON_EEE_QUEUE5_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_QUEUE5_t;





#define BM_PON_EEE_QUEUE6                        0x090c3
#define BM_PON_EEE_QUEUE6_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_QUEUE6_t;





#define BM_PON_EEE_QUEUE7                        0x090c4
#define BM_PON_EEE_QUEUE7_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_QUEUE7_t;





#define BM_PON_EEE_QUEUE8                        0x090c5
#define BM_PON_EEE_QUEUE8_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 30 ;
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qthres_sel           :  2 ; /* bits 1:0 */
    cs_uint32 rsrvd1               : 30 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_QUEUE8_t;





#define BM_PON_EEE_THRESHOLD1                    0x090c6
#define BM_PON_EEE_THRESHOLD1_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 eee_thres            :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 eee_thres            :  9 ; /* bits 8:0 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_THRESHOLD1_t;





#define BM_PON_EEE_THRESHOLD2                    0x090c7
#define BM_PON_EEE_THRESHOLD2_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 eee_thres            :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 eee_thres            :  9 ; /* bits 8:0 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_THRESHOLD2_t;





#define BM_PON_EEE_THRESHOLD3                    0x090c8
#define BM_PON_EEE_THRESHOLD3_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 eee_thres            :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 eee_thres            :  9 ; /* bits 8:0 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_EEE_THRESHOLD3_t;





#define BM_BM_HEADER_MEMORY_ACCESS               0x090c9
#define BM_BM_HEADER_MEMORY_ACCESS_dft           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 addr                 : 15 ; /* bits 14:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 addr                 : 15 ; /* bits 14:0 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_BM_HEADER_MEMORY_ACCESS_t;





#define BM_BM_HEADER_MEMORY_DATA1                0x090ca
#define BM_BM_HEADER_MEMORY_DATA1_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 ecc                  :  7 ; /* bits 9:3 */
    cs_uint32 pkt_type             :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_type             :  3 ; /* bits 2:0 */
    cs_uint32 ecc                  :  7 ; /* bits 9:3 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_BM_HEADER_MEMORY_DATA1_t;





#define BM_BM_HEADER_MEMORY_DATA0                0x090cb
#define BM_BM_HEADER_MEMORY_DATA0_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 pkt_type             :  2 ; /* bits 31:30 */
    cs_uint32 buffer_ptr_hi        :  4 ; /* bits 29:26 */
    cs_uint32 pkt_spid             :  3 ; /* bits 25:23 */
    cs_uint32 sop                  :  1 ; /* bits 22:22 */
    cs_uint32 eop                  :  1 ; /* bits 21:21 */
    cs_uint32 buffer_ptr_lo        : 10 ; /* bits 20:11 */
    cs_uint32 pkt_length           : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pkt_length           : 11 ; /* bits 10:0 */
    cs_uint32 buffer_ptr_lo        : 10 ; /* bits 20:11 */
    cs_uint32 eop                  :  1 ; /* bits 21:21 */
    cs_uint32 sop                  :  1 ; /* bits 22:22 */
    cs_uint32 pkt_spid             :  3 ; /* bits 25:23 */
    cs_uint32 buffer_ptr_hi        :  4 ; /* bits 29:26 */
    cs_uint32 pkt_type             :  2 ; /* bits 31:30 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_BM_HEADER_MEMORY_DATA0_t;





#define BM_DP_STAGE_MONITOR                      0x090cc
#define BM_DP_STAGE_MONITOR_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 stage                :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 stage                :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_DP_STAGE_MONITOR_t;





#define BM_GE_DP_MONITOR1_0                      0x090cd
#define BM_GE_DP_MONITOR1_0_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 ql_c1                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c0                : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ql_c0                : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c1                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_DP_MONITOR1_0_t;





#define BM_GE_DP_MONITOR3_2                      0x090ce
#define BM_GE_DP_MONITOR3_2_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 ql_c3                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c2                : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ql_c2                : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c3                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_DP_MONITOR3_2_t;





#define BM_GE_DP_MONITOR5_4                      0x090cf
#define BM_GE_DP_MONITOR5_4_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 ql_c5                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c4                : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ql_c4                : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c5                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_DP_MONITOR5_4_t;





#define BM_GE_DP_MONITOR7_6                      0x090d0
#define BM_GE_DP_MONITOR7_6_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 ql_c7                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c6                : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ql_c6                : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c7                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_DP_MONITOR7_6_t;





#define BM_PON_DP_MONITOR1_0                     0x090d1
#define BM_PON_DP_MONITOR1_0_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 ql_c1                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c0                : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ql_c0                : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c1                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_DP_MONITOR1_0_t;





#define BM_PON_DP_MONITOR3_2                     0x090d2
#define BM_PON_DP_MONITOR3_2_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 ql_c3                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c2                : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ql_c2                : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c3                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_DP_MONITOR3_2_t;





#define BM_PON_DP_MONITOR5_4                     0x090d3
#define BM_PON_DP_MONITOR5_4_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 ql_c5                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c4                : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ql_c4                : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c5                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_DP_MONITOR5_4_t;





#define BM_PON_DP_MONITOR7_6                     0x090d4
#define BM_PON_DP_MONITOR7_6_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 ql_c7                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c6                : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ql_c6                : 14 ; /* bits 13:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 ql_c7                : 14 ; /* bits 29:16 */
    cs_uint32 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_DP_MONITOR7_6_t;





#define BM_GE_ABR_DROP_COS7_PKT_CNTR             0x090d5
#define BM_GE_ABR_DROP_COS7_PKT_CNTR_dft         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 cntr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_GE_ABR_DROP_COS7_PKT_CNTR_t;





#define BM_PON_ABR_DROP_COS7_PKT_CNTR            0x090d6
#define BM_PON_ABR_DROP_COS7_PKT_CNTR_dft        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 cntr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cntr                 : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} BM_PON_ABR_DROP_COS7_PKT_CNTR_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* bm */

typedef struct {
  BM_SCH_CFG_t                             SCH_CFG;                       /* addr: 0x00000 */ 
  BM_SHARED_BUFFER_CFG_t                   SHARED_BUFFER_CFG;             /* addr: 0x00001 */ 
  BM_GE_PORT_BUFFER_CFG_t                  GE_PORT_BUFFER_CFG;            /* addr: 0x00002 */ 
  BM_PON_PORT_BUFFER_CFG_t                 PON_PORT_BUFFER_CFG;           /* addr: 0x00003 */ 
  BM_CPU_PORT_BUFFER_CFG_t                 CPU_PORT_BUFFER_CFG;           /* addr: 0x00004 */ 
  BM_SHARED_BUFFER_THRESHOLD_t             SHARED_BUFFER_THRESHOLD;       /* addr: 0x00005 */ 
  BM_GE_QUEUE_LENGTH_LIMIT_COS7_4_t        GE_QUEUE_LENGTH_LIMIT_COS7_4;  /* addr: 0x00006 */ 
  BM_GE_QUEUE_LENGTH_LIMIT_COS3_0_t        GE_QUEUE_LENGTH_LIMIT_COS3_0;  /* addr: 0x00007 */ 
  BM_PON_QUEUE_LENGTH_LIMIT_COS7_4_t       PON_QUEUE_LENGTH_LIMIT_COS7_4; /* addr: 0x00008 */ 
  BM_PON_QUEUE_LENGTH_LIMIT_COS3_0_t       PON_QUEUE_LENGTH_LIMIT_COS3_0; /* addr: 0x00009 */ 
  BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0_t       CPU_QUEUE_LENGTH_LIMIT_COS7_0; /* addr: 0x0000a */ 
  BM_GE_PORT_TX_BUFFER_LMT_t               GE_PORT_TX_BUFFER_LMT;         /* addr: 0x0000b */ 
  BM_PON_PORT_TX_BUFFER_LMT_t              PON_PORT_TX_BUFFER_LMT;        /* addr: 0x0000c */ 
  BM_MA_PORT_TX_BUFFER_LMT_t               MA_PORT_TX_BUFFER_LMT;         /* addr: 0x0000d */ 
  BM_PC_CFG1_t                             PC_CFG1;                       /* addr: 0x0000e */ 
  BM_PC_CFG0_t                             PC_CFG0;                       /* addr: 0x0000f */ 
  BM_LINKLIST_SIZE_t                       LINKLIST_SIZE;                 /* addr: 0x00010 */ 
  BM_PORT_TX_CTRL_t                        PORT_TX_CTRL;                  /* addr: 0x00011 */ 
  BM_DRR_WEIGHT_BASE_t                     DRR_WEIGHT_BASE;               /* addr: 0x00012 */ 
  BM_GE_WEIGHT_RATIO_COS7_4_t              GE_WEIGHT_RATIO_COS7_4;        /* addr: 0x00013 */ 
  BM_GE_WEIGHT_RATIO_COS3_0_t              GE_WEIGHT_RATIO_COS3_0;        /* addr: 0x00014 */ 
  BM_PON_WEIGHT_RATIO_COS7_4_t             PON_WEIGHT_RATIO_COS7_4;       /* addr: 0x00015 */ 
  BM_PON_WEIGHT_RATIO_COS3_0_t             PON_WEIGHT_RATIO_COS3_0;       /* addr: 0x00016 */ 
  BM_ABR_CFG_t                             ABR_CFG;                       /* addr: 0x00017 */ 
  BM_DP_CFG_t                              DP_CFG;                        /* addr: 0x00018 */ 
  BM_DP_CFG_STAGE2_1_t                     DP_CFG_STAGE2_1;               /* addr: 0x00019 */ 
  BM_DP_CFG_STAGE4_3_t                     DP_CFG_STAGE4_3;               /* addr: 0x0001a */ 
  BM_DP_CFG_STAGE6_5_t                     DP_CFG_STAGE6_5;               /* addr: 0x0001b */ 
  BM_DP_CFG_STAGE8_7_t                     DP_CFG_STAGE8_7;               /* addr: 0x0001c */ 
  BM_MA_DPID4_COS_CFG_t                    MA_DPID4_COS_CFG;              /* addr: 0x0001d */ 
  BM_MA_DPID5_COS_CFG_t                    MA_DPID5_COS_CFG;              /* addr: 0x0001e */ 
  BM_MA_DPID6_COS_CFG_t                    MA_DPID6_COS_CFG;              /* addr: 0x0001f */ 
  BM_MA_COS_DPID_CFG_t                     MA_COS_DPID_CFG;               /* addr: 0x00020 */ 
  BM_MPCP_CFG_t                            MPCP_CFG;                      /* addr: 0x00021 */ 
  BM_MPCP_RPT_COS_MAPPING_t                MPCP_RPT_COS_MAPPING;          /* addr: 0x00022 */ 
  BM_MPCP_OFFSET_t                         MPCP_OFFSET;                   /* addr: 0x00023 */ 
  BM_MPCP_Q2_THRESHOLD_7_6_t               MPCP_Q2_THRESHOLD_7_6;         /* addr: 0x00024 */ 
  BM_MPCP_Q2_THRESHOLD_5_4_t               MPCP_Q2_THRESHOLD_5_4;         /* addr: 0x00025 */ 
  BM_MPCP_Q2_THRESHOLD_3_2_t               MPCP_Q2_THRESHOLD_3_2;         /* addr: 0x00026 */ 
  BM_MPCP_Q2_THRESHOLD_1_0_t               MPCP_Q2_THRESHOLD_1_0;         /* addr: 0x00027 */ 
  BM_MPCP_Q2_MAX_7_6_t                     MPCP_Q2_MAX_7_6;               /* addr: 0x00028 */ 
  BM_MPCP_Q2_MAX_5_4_t                     MPCP_Q2_MAX_5_4;               /* addr: 0x00029 */ 
  BM_MPCP_Q2_MAX_3_2_t                     MPCP_Q2_MAX_3_2;               /* addr: 0x0002a */ 
  BM_MPCP_Q2_MAX_1_0_t                     MPCP_Q2_MAX_1_0;               /* addr: 0x0002b */ 
  BM_FLUSH_CTRL_t                          FLUSH_CTRL;                    /* addr: 0x0002c */ 
  BM_FLUSH_STATUS_t                        FLUSH_STATUS;                  /* addr: 0x0002d */ 
  BM_PORT_SHAPER_CFG_t                     PORT_SHAPER_CFG;               /* addr: 0x0002e */ 
  BM_LYNXD_MPCP_CFG_t                      LYNXD_MPCP_CFG;                /* addr: 0x0002f */ 
  BM_MPCP_RPT_BIT_MAP_t                    MPCP_RPT_BIT_MAP;              /* addr: 0x00030 */ 
  BM_US_Q0_START_t                         US_Q0_START;                   /* addr: 0x00031 */ 
  BM_US_Q0_END_t                           US_Q0_END;                     /* addr: 0x00032 */ 
  BM_US_Q1_START_t                         US_Q1_START;                   /* addr: 0x00033 */ 
  BM_US_Q1_END_t                           US_Q1_END;                     /* addr: 0x00034 */ 
  BM_US_Q2_START_t                         US_Q2_START;                   /* addr: 0x00035 */ 
  BM_US_Q2_END_t                           US_Q2_END;                     /* addr: 0x00036 */ 
  BM_US_Q3_START_t                         US_Q3_START;                   /* addr: 0x00037 */ 
  BM_US_Q3_END_t                           US_Q3_END;                     /* addr: 0x00038 */ 
  BM_US_Q4_START_t                         US_Q4_START;                   /* addr: 0x00039 */ 
  BM_US_Q4_END_t                           US_Q4_END;                     /* addr: 0x0003a */ 
  BM_DS_Q0_START_t                         DS_Q0_START;                   /* addr: 0x0003b */ 
  BM_DS_Q0_END_t                           DS_Q0_END;                     /* addr: 0x0003c */ 
  BM_DS_Q1_START_t                         DS_Q1_START;                   /* addr: 0x0003d */ 
  BM_DS_Q1_END_t                           DS_Q1_END;                     /* addr: 0x0003e */ 
  BM_DS_Q2_START_t                         DS_Q2_START;                   /* addr: 0x0003f */ 
  BM_DS_Q2_END_t                           DS_Q2_END;                     /* addr: 0x00040 */ 
  BM_DS_Q3_START_t                         DS_Q3_START;                   /* addr: 0x00041 */ 
  BM_DS_Q3_END_t                           DS_Q3_END;                     /* addr: 0x00042 */ 
  BM_DS_Q4_START_t                         DS_Q4_START;                   /* addr: 0x00043 */ 
  BM_DS_Q4_END_t                           DS_Q4_END;                     /* addr: 0x00044 */ 
  BM_MA_Q0_END_t                           MA_Q0_END;                     /* addr: 0x00045 */ 
  BM_MA_Q1_END_t                           MA_Q1_END;                     /* addr: 0x00046 */ 
  BM_MA_Q2_END_t                           MA_Q2_END;                     /* addr: 0x00047 */ 
  BM_MA_Q3_END_t                           MA_Q3_END;                     /* addr: 0x00048 */ 
  BM_MA_Q4_END_t                           MA_Q4_END;                     /* addr: 0x00049 */ 
  BM_MA_Q5_END_t                           MA_Q5_END;                     /* addr: 0x0004a */ 
  BM_MA_Q6_END_t                           MA_Q6_END;                     /* addr: 0x0004b */ 
  BM_MA_Q7_END_t                           MA_Q7_END;                     /* addr: 0x0004c */ 
  BM_MA_Q7_START_t                         MA_Q7_START;                   /* addr: 0x0004d */ 
  BM_CPU_ACCESS_OFFSET_t                   CPU_ACCESS_OFFSET;             /* addr: 0x0004e */ 
  BM_US_Q0_FLOW_CTRL_t                     US_Q0_FLOW_CTRL;               /* addr: 0x0004f */ 
  BM_US_Q1_FLOW_CTRL_t                     US_Q1_FLOW_CTRL;               /* addr: 0x00050 */ 
  BM_US_Q2_FLOW_CTRL_t                     US_Q2_FLOW_CTRL;               /* addr: 0x00051 */ 
  BM_US_Q3_FLOW_CTRL_t                     US_Q3_FLOW_CTRL;               /* addr: 0x00052 */ 
  BM_US_Q4_FLOW_CTRL_t                     US_Q4_FLOW_CTRL;               /* addr: 0x00053 */ 
  BM_OAM_PKT_EN_t                          OAM_PKT_EN;                    /* addr: 0x00054 */ 
  BM_OAM_PKT_HEADER_t                      OAM_PKT_HEADER;                /* addr: 0x00055 */ 
  BM_Q_FLUSH_EN_t                          Q_FLUSH_EN;                    /* addr: 0x00056 */ 
  BM_Q_FLUSH_CTRL_t                        Q_FLUSH_CTRL;                  /* addr: 0x00057 */ 
  BM_MPCP_QSET_2_t                         MPCP_QSET_2;                   /* addr: 0x00058 */ 
  BM_TEST0_t                               TEST0;                         /* addr: 0x00059 */ 
  BM_TEST1_t                               TEST1;                         /* addr: 0x0005a */ 
  BM_INTERRUPT_E_t                         INTERRUPT_E;                   /* addr: 0x0005b */ 
  BM_INTERRUPT_t                           INTERRUPT;                     /* addr: 0x0005c */ 
  BM_GE_RX_PKT_CNTR_t                      GE_RX_PKT_CNTR;                /* addr: 0x0005d */ 
  BM_PON_RX_PKT_CNTR_t                     PON_RX_PKT_CNTR;               /* addr: 0x0005e */ 
  BM_CPU_RX_PKT_CNTR_t                     CPU_RX_PKT_CNTR;               /* addr: 0x0005f */ 
  BM_GE_TX_PKT_CNTR_t                      GE_TX_PKT_CNTR;                /* addr: 0x00060 */ 
  BM_PON_TX_PKT_CNTR_t                     PON_TX_PKT_CNTR;               /* addr: 0x00061 */ 
  BM_CPU_TX_PKT_CNTR_t                     CPU_TX_PKT_CNTR;               /* addr: 0x00062 */ 
  BM_GE_DROP_PKT_CNTR_t                    GE_DROP_PKT_CNTR;              /* addr: 0x00063 */ 
  BM_PON_DROP_PKT_CNTR_t                   PON_DROP_PKT_CNTR;             /* addr: 0x00064 */ 
  BM_CPU_DROP_PKT_CNTR_t                   CPU_DROP_PKT_CNTR;             /* addr: 0x00065 */ 
  BM_GE_FE_DROP_PKT_CNTR_t                 GE_FE_DROP_PKT_CNTR;           /* addr: 0x00066 */ 
  BM_PON_FE_DROP_PKT_CNTR_t                PON_FE_DROP_PKT_CNTR;          /* addr: 0x00067 */ 
  BM_CPU_FE_DROP_PKT_CNTR_t                CPU_FE_DROP_PKT_CNTR;          /* addr: 0x00068 */ 
  BM_GE_ABR_DROP_PKT_CNTR_t                GE_ABR_DROP_PKT_CNTR;          /* addr: 0x00069 */ 
  BM_PON_ABR_DROP_PKT_CNTR_t               PON_ABR_DROP_PKT_CNTR;         /* addr: 0x0006a */ 
  BM_MPCP_DROP_PKT_CNTR_t                  MPCP_DROP_PKT_CNTR;            /* addr: 0x0006b */ 
  BM_SHARED_BUFFER_CNTR_t                  SHARED_BUFFER_CNTR;            /* addr: 0x0006c */ 
  BM_GE_TX_BUFFER_CNTR_t                   GE_TX_BUFFER_CNTR;             /* addr: 0x0006d */ 
  BM_PON_TX_BUFFER_CNTR_t                  PON_TX_BUFFER_CNTR;            /* addr: 0x0006e */ 
  BM_CPU_TX_BUFFER_CNTR_t                  CPU_TX_BUFFER_CNTR;            /* addr: 0x0006f */ 
  BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS8_t    GE_TX_PER_QUEUE_BUFFER_CNTR_COS8; /* addr: 0x00070 */ 
  BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS7_t    GE_TX_PER_QUEUE_BUFFER_CNTR_COS7; /* addr: 0x00071 */ 
  BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS6_t    GE_TX_PER_QUEUE_BUFFER_CNTR_COS6; /* addr: 0x00072 */ 
  BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS5_t    GE_TX_PER_QUEUE_BUFFER_CNTR_COS5; /* addr: 0x00073 */ 
  BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS4_t    GE_TX_PER_QUEUE_BUFFER_CNTR_COS4; /* addr: 0x00074 */ 
  BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS3_t    GE_TX_PER_QUEUE_BUFFER_CNTR_COS3; /* addr: 0x00075 */ 
  BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS2_t    GE_TX_PER_QUEUE_BUFFER_CNTR_COS2; /* addr: 0x00076 */ 
  BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS1_t    GE_TX_PER_QUEUE_BUFFER_CNTR_COS1; /* addr: 0x00077 */ 
  BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS0_t    GE_TX_PER_QUEUE_BUFFER_CNTR_COS0; /* addr: 0x00078 */ 
  BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS8_t   PON_TX_PER_QUEUE_BUFFER_CNTR_COS8; /* addr: 0x00079 */ 
  BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS7_t   PON_TX_PER_QUEUE_BUFFER_CNTR_COS7; /* addr: 0x0007a */ 
  BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS6_t   PON_TX_PER_QUEUE_BUFFER_CNTR_COS6; /* addr: 0x0007b */ 
  BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS5_t   PON_TX_PER_QUEUE_BUFFER_CNTR_COS5; /* addr: 0x0007c */ 
  BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS4_t   PON_TX_PER_QUEUE_BUFFER_CNTR_COS4; /* addr: 0x0007d */ 
  BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS3_t   PON_TX_PER_QUEUE_BUFFER_CNTR_COS3; /* addr: 0x0007e */ 
  BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS2_t   PON_TX_PER_QUEUE_BUFFER_CNTR_COS2; /* addr: 0x0007f */ 
  BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS1_t   PON_TX_PER_QUEUE_BUFFER_CNTR_COS1; /* addr: 0x00080 */ 
  BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS0_t   PON_TX_PER_QUEUE_BUFFER_CNTR_COS0; /* addr: 0x00081 */ 
  BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS8_t   CPU_TX_PER_QUEUE_BUFFER_CNTR_COS8; /* addr: 0x00082 */ 
  BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS7_t   CPU_TX_PER_QUEUE_BUFFER_CNTR_COS7; /* addr: 0x00083 */ 
  BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS6_t   CPU_TX_PER_QUEUE_BUFFER_CNTR_COS6; /* addr: 0x00084 */ 
  BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS5_t   CPU_TX_PER_QUEUE_BUFFER_CNTR_COS5; /* addr: 0x00085 */ 
  BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS4_t   CPU_TX_PER_QUEUE_BUFFER_CNTR_COS4; /* addr: 0x00086 */ 
  BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS3_t   CPU_TX_PER_QUEUE_BUFFER_CNTR_COS3; /* addr: 0x00087 */ 
  BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS2_t   CPU_TX_PER_QUEUE_BUFFER_CNTR_COS2; /* addr: 0x00088 */ 
  BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS1_t   CPU_TX_PER_QUEUE_BUFFER_CNTR_COS1; /* addr: 0x00089 */ 
  BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS0_t   CPU_TX_PER_QUEUE_BUFFER_CNTR_COS0; /* addr: 0x0008a */ 
  BM_GE_HD_ERR_CNTR_t                      GE_HD_ERR_CNTR;                /* addr: 0x0008b */ 
  BM_PON_HD_ERR_CNTR_t                     PON_HD_ERR_CNTR;               /* addr: 0x0008c */ 
  BM_CPU_HD_ERR_CNTR_t                     CPU_HD_ERR_CNTR;               /* addr: 0x0008d */ 
  BM_GE_PACKET_HEADER_INFO_t               GE_PACKET_HEADER_INFO;         /* addr: 0x0008e */ 
  BM_PON_PACKET_HEADER_INFO_t              PON_PACKET_HEADER_INFO;        /* addr: 0x0008f */ 
  BM_CPU_PACKET_HEADER_INFO_t              CPU_PACKET_HEADER_INFO;        /* addr: 0x00090 */ 
  BM_MPCP_WINDOW_STS_t                     MPCP_WINDOW_STS;               /* addr: 0x00091 */ 
  BM_RMU_PT0_DROP_CNT1_t                   RMU_PT0_DROP_CNT1;             /* addr: 0x00092 */ 
  BM_RMU_PT0_DROP_CNT2_t                   RMU_PT0_DROP_CNT2;             /* addr: 0x00093 */ 
  BM_RMU_PT0_DROP_CNT3_t                   RMU_PT0_DROP_CNT3;             /* addr: 0x00094 */ 
  BM_RMU_PT1_DROP_CNT1_t                   RMU_PT1_DROP_CNT1;             /* addr: 0x00095 */ 
  BM_RMU_PT1_DROP_CNT2_t                   RMU_PT1_DROP_CNT2;             /* addr: 0x00096 */ 
  BM_RMU_PT1_DROP_CNT3_t                   RMU_PT1_DROP_CNT3;             /* addr: 0x00097 */ 
  BM_RMU_PT2_DROP_CNT1_t                   RMU_PT2_DROP_CNT1;             /* addr: 0x00098 */ 
  BM_RMU_PT2_DROP_CNT2_t                   RMU_PT2_DROP_CNT2;             /* addr: 0x00099 */ 
  BM_RMU_PT2_DROP_CNT3_t                   RMU_PT2_DROP_CNT3;             /* addr: 0x0009a */ 
  BM_LYNXD_TMU_STATUS_t                    LYNXD_TMU_STATUS;              /* addr: 0x0009b */ 
  BM_P1_Q0_START_t                         P1_Q0_START;                   /* addr: 0x0009c */ 
  BM_P1_Q0_END_t                           P1_Q0_END;                     /* addr: 0x0009d */ 
  BM_P1_Q1_START_t                         P1_Q1_START;                   /* addr: 0x0009e */ 
  BM_P1_Q1_END_t                           P1_Q1_END;                     /* addr: 0x0009f */ 
  BM_P1_Q2_START_t                         P1_Q2_START;                   /* addr: 0x000a0 */ 
  BM_P1_Q2_END_t                           P1_Q2_END;                     /* addr: 0x000a1 */ 
  BM_P1_Q3_START_t                         P1_Q3_START;                   /* addr: 0x000a2 */ 
  BM_P1_Q3_END_t                           P1_Q3_END;                     /* addr: 0x000a3 */ 
  BM_P1_Q4_START_t                         P1_Q4_START;                   /* addr: 0x000a4 */ 
  BM_P1_Q4_END_t                           P1_Q4_END;                     /* addr: 0x000a5 */ 
  BM_GRANT_MONITOR_t                       GRANT_MONITOR;                 /* addr: 0x000a6 */ 
  BM_EEE_ECFG_t                            EEE_ECFG;                      /* addr: 0x000a7 */ 
  BM_EEE_SW_CTRL_t                         EEE_SW_CTRL;                   /* addr: 0x000a8 */ 
  BM_EEE_STAT_t                            EEE_STAT;                      /* addr: 0x000a9 */ 
  BM_EEE_SLEEP_DELAY_t                     EEE_SLEEP_DELAY;               /* addr: 0x000aa */ 
  BM_EEE_WAKE_DELAY_t                      EEE_WAKE_DELAY;                /* addr: 0x000ab */ 
  BM_EEE_QUEUE0_t                          EEE_QUEUE0;                    /* addr: 0x000ac */ 
  BM_EEE_QUEUE1_t                          EEE_QUEUE1;                    /* addr: 0x000ad */ 
  BM_EEE_QUEUE2_t                          EEE_QUEUE2;                    /* addr: 0x000ae */ 
  BM_EEE_QUEUE3_t                          EEE_QUEUE3;                    /* addr: 0x000af */ 
  BM_EEE_QUEUE4_t                          EEE_QUEUE4;                    /* addr: 0x000b0 */ 
  BM_EEE_QUEUE5_t                          EEE_QUEUE5;                    /* addr: 0x000b1 */ 
  BM_EEE_QUEUE6_t                          EEE_QUEUE6;                    /* addr: 0x000b2 */ 
  BM_EEE_QUEUE7_t                          EEE_QUEUE7;                    /* addr: 0x000b3 */ 
  BM_EEE_QUEUE8_t                          EEE_QUEUE8;                    /* addr: 0x000b4 */ 
  BM_EEE_THRESHOLD1_t                      EEE_THRESHOLD1;                /* addr: 0x000b5 */ 
  BM_EEE_THRESHOLD2_t                      EEE_THRESHOLD2;                /* addr: 0x000b6 */ 
  BM_EEE_THRESHOLD3_t                      EEE_THRESHOLD3;                /* addr: 0x000b7 */ 
  BM_PON_EEE_ECFG_t                        PON_EEE_ECFG;                  /* addr: 0x000b8 */ 
  BM_PON_EEE_SW_CTRL_t                     PON_EEE_SW_CTRL;               /* addr: 0x000b9 */ 
  BM_PON_EEE_STAT_t                        PON_EEE_STAT;                  /* addr: 0x000ba */ 
  BM_PON_EEE_SLEEP_DELAY_t                 PON_EEE_SLEEP_DELAY;           /* addr: 0x000bb */ 
  BM_PON_EEE_WAKE_DELAY_t                  PON_EEE_WAKE_DELAY;            /* addr: 0x000bc */ 
  BM_PON_EEE_QUEUE0_t                      PON_EEE_QUEUE0;                /* addr: 0x000bd */ 
  BM_PON_EEE_QUEUE1_t                      PON_EEE_QUEUE1;                /* addr: 0x000be */ 
  BM_PON_EEE_QUEUE2_t                      PON_EEE_QUEUE2;                /* addr: 0x000bf */ 
  BM_PON_EEE_QUEUE3_t                      PON_EEE_QUEUE3;                /* addr: 0x000c0 */ 
  BM_PON_EEE_QUEUE4_t                      PON_EEE_QUEUE4;                /* addr: 0x000c1 */ 
  BM_PON_EEE_QUEUE5_t                      PON_EEE_QUEUE5;                /* addr: 0x000c2 */ 
  BM_PON_EEE_QUEUE6_t                      PON_EEE_QUEUE6;                /* addr: 0x000c3 */ 
  BM_PON_EEE_QUEUE7_t                      PON_EEE_QUEUE7;                /* addr: 0x000c4 */ 
  BM_PON_EEE_QUEUE8_t                      PON_EEE_QUEUE8;                /* addr: 0x000c5 */ 
  BM_PON_EEE_THRESHOLD1_t                  PON_EEE_THRESHOLD1;            /* addr: 0x000c6 */ 
  BM_PON_EEE_THRESHOLD2_t                  PON_EEE_THRESHOLD2;            /* addr: 0x000c7 */ 
  BM_PON_EEE_THRESHOLD3_t                  PON_EEE_THRESHOLD3;            /* addr: 0x000c8 */ 
  BM_BM_HEADER_MEMORY_ACCESS_t             BM_HEADER_MEMORY_ACCESS;       /* addr: 0x000c9 */ 
  BM_BM_HEADER_MEMORY_DATA1_t              BM_HEADER_MEMORY_DATA1;        /* addr: 0x000ca */ 
  BM_BM_HEADER_MEMORY_DATA0_t              BM_HEADER_MEMORY_DATA0;        /* addr: 0x000cb */ 
  BM_DP_STAGE_MONITOR_t                    DP_STAGE_MONITOR;              /* addr: 0x000cc */ 
  BM_GE_DP_MONITOR1_0_t                    GE_DP_MONITOR1_0;              /* addr: 0x000cd */ 
  BM_GE_DP_MONITOR3_2_t                    GE_DP_MONITOR3_2;              /* addr: 0x000ce */ 
  BM_GE_DP_MONITOR5_4_t                    GE_DP_MONITOR5_4;              /* addr: 0x000cf */ 
  BM_GE_DP_MONITOR7_6_t                    GE_DP_MONITOR7_6;              /* addr: 0x000d0 */ 
  BM_PON_DP_MONITOR1_0_t                   PON_DP_MONITOR1_0;             /* addr: 0x000d1 */ 
  BM_PON_DP_MONITOR3_2_t                   PON_DP_MONITOR3_2;             /* addr: 0x000d2 */ 
  BM_PON_DP_MONITOR5_4_t                   PON_DP_MONITOR5_4;             /* addr: 0x000d3 */ 
  BM_PON_DP_MONITOR7_6_t                   PON_DP_MONITOR7_6;             /* addr: 0x000d4 */ 
  BM_GE_ABR_DROP_COS7_PKT_CNTR_t           GE_ABR_DROP_COS7_PKT_CNTR;     /* addr: 0x000d5 */ 
  BM_PON_ABR_DROP_COS7_PKT_CNTR_t          PON_ABR_DROP_COS7_PKT_CNTR;    /* addr: 0x000d6 */ 
} BM_t;

  

/*
 * pcm
 */
  
  
/*
 * PCMS
 */
  



#define PCM_PCMS_RESET                           0x0a000
#define PCM_PCMS_RESET_dft                       0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 all_soft_reset       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 all_soft_reset       :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_RESET_t;





#define PCM_PCMS_CTL                             0x0a001
#define PCM_PCMS_CTL_dft                         0x0000001f




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 FSCFreeRun           :  1 ; /* bits 7:7 */
    cs_uint32 pcm_load             :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 pcm_load             :  7 ; /* bits 6:0 */
    cs_uint32 FSCFreeRun           :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_CTL_t;

#define     FSCFreeRun_DIS                                0x0
#define     FSCFreeRun_ENA                                0x1




#define PCM_PCMS_SI_CFG                          0x0a002
#define PCM_PCMS_SI_CFG_dft                      0x00000080




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               :  8 ;
    cs_uint32 def_txData           :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 spdifEn              :  1 ; /* bits 12:12 */
    cs_uint32 mergeFSEdge          :  1 ; /* bits 11:11 */
    cs_uint32 FSEdge               :  1 ; /* bits 10:10 */
    cs_uint32 FSPol                :  1 ; /* bits 9:9 */
    cs_uint32 slotSyncSel          :  1 ; /* bits 8:8 */
    cs_uint32 rxEdge               :  1 ; /* bits 7:7 */
    cs_uint32 txHzEdge             :  1 ; /* bits 6:6 */
    cs_uint32 txEdge               :  1 ; /* bits 5:5 */
    cs_uint32 lsbFirst             :  1 ; /* bits 4:4 */
    cs_uint32 doubleClk            :  1 ; /* bits 3:3 */
    cs_uint32 sClkOffOnIdle        :  1 ; /* bits 2:2 */
    cs_uint32 sClkPhase            :  1 ; /* bits 1:1 */
    cs_uint32 sClkPol              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sClkPol              :  1 ; /* bits 0:0 */
    cs_uint32 sClkPhase            :  1 ; /* bits 1:1 */
    cs_uint32 sClkOffOnIdle        :  1 ; /* bits 2:2 */
    cs_uint32 doubleClk            :  1 ; /* bits 3:3 */
    cs_uint32 lsbFirst             :  1 ; /* bits 4:4 */
    cs_uint32 txEdge               :  1 ; /* bits 5:5 */
    cs_uint32 txHzEdge             :  1 ; /* bits 6:6 */
    cs_uint32 rxEdge               :  1 ; /* bits 7:7 */
    cs_uint32 slotSyncSel          :  1 ; /* bits 8:8 */
    cs_uint32 FSPol                :  1 ; /* bits 9:9 */
    cs_uint32 FSEdge               :  1 ; /* bits 10:10 */
    cs_uint32 mergeFSEdge          :  1 ; /* bits 11:11 */
    cs_uint32 spdifEn              :  1 ; /* bits 12:12 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 def_txData           :  8 ; /* bits 23:16 */
    cs_uint32 rsrvd2               :  8 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_SI_CFG_t;

#define     spdifEn_PCM                                   0x0
#define     spdifEn_SPC                                   0x1
#define     mergeFSEdge_FS                                0x0
#define     mergeFSEdge_DT                                0x1
#define     FSEdge_RE                                     0x0
#define     FSEdge_FE                                     0x1
#define     FSPol_HI                                      0x0
#define     FSPol_LO                                      0x1
#define     slotSyncSel_FR                                0x0
#define     slotSyncSel_SL                                0x1
#define     rxEdge_RE                                     0x0
#define     rxEdge_FE                                     0x1
#define     txHzEdge_SAME                                 0x0
#define     txHzEdge_HALF                                 0x1
#define     txEdge_RE                                     0x0
#define     txEdge_FE                                     0x1
#define     lsbFirst_MSB                                  0x0
#define     lsbFirst_LSB                                  0x1
#define     doubleClk_ONE                                 0x0
#define     doubleClk_TWO                                 0x1
#define     sClkOffOnIdle_DIS                             0x0
#define     sClkOffOnIdle_ENA                             0x1




#define PCM_PCMS_FR_CFG                          0x0a003
#define PCM_PCMS_FR_CFG_dft                      0x00038007




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 FS2Slot              :  4 ; /* bits 31:28 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 slot2Slot            :  3 ; /* bits 26:24 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 numOfSlots           :  7 ; /* bits 22:16 */
    cs_uint32 slot2FS              :  4 ; /* bits 15:12 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 FSLen                : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 FSLen                : 10 ; /* bits 9:0 */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 slot2FS              :  4 ; /* bits 15:12 */
    cs_uint32 numOfSlots           :  7 ; /* bits 22:16 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 slot2Slot            :  3 ; /* bits 26:24 */
    cs_uint32 rsrvd3               :  1 ;
    cs_uint32 FS2Slot              :  4 ; /* bits 31:28 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_FR_CFG_t;





#define PCM_PCMS_FR_CFG2                         0x0a004
#define PCM_PCMS_FR_CFG2_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 17 ;
    cs_uint32 frame2Frame          :  3 ; /* bits 14:12 */
    cs_uint32 rsrvd1               : 12 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               : 12 ;
    cs_uint32 frame2Frame          :  3 ; /* bits 14:12 */
    cs_uint32 rsrvd2               : 17 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_FR_CFG2_t;





#define PCM_PCMS_TS3                             0x0a005
#define PCM_PCMS_TS3_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 txSlotValid          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 txSlotValid          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TS3_t;

#define     txSlotValid_DIS                               0x0
#define     txSlotValid_ENA                               0x1




#define PCM_PCMS_TS2                             0x0a006
#define PCM_PCMS_TS2_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 txSlotValid          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 txSlotValid          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TS2_t;





#define PCM_PCMS_TS1                             0x0a007
#define PCM_PCMS_TS1_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 txSlotValid          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 txSlotValid          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TS1_t;





#define PCM_PCMS_TS0                             0x0a008
#define PCM_PCMS_TS0_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 txSlotValid          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 txSlotValid          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TS0_t;





#define PCM_PCMS_RS3                             0x0a009
#define PCM_PCMS_RS3_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rxSlotValid          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rxSlotValid          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_RS3_t;

#define     rxSlotValid_DIS                               0x0
#define     rxSlotValid_ENA                               0x1




#define PCM_PCMS_RS2                             0x0a00a
#define PCM_PCMS_RS2_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rxSlotValid          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rxSlotValid          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_RS2_t;





#define PCM_PCMS_RS1                             0x0a00b
#define PCM_PCMS_RS1_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rxSlotValid          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rxSlotValid          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_RS1_t;





#define PCM_PCMS_RS0                             0x0a00c
#define PCM_PCMS_RS0_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rxSlotValid          : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rxSlotValid          : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_RS0_t;





#define PCM_PCMS_TMR_LD1                         0x0a00d
#define PCM_PCMS_TMR_LD1_dft                     0x0000ffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 load1                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 load1                : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TMR_LD1_t;





#define PCM_PCMS_TMR_CTRL1                       0x0a00e
#define PCM_PCMS_TMR_CTRL1_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 24 ;
    cs_uint32 timen1               :  1 ; /* bits 7:7 */
    cs_uint32 rlmode1              :  1 ; /* bits 6:6 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 clksel1              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd1               :  2 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 clksel1              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 rlmode1              :  1 ; /* bits 6:6 */
    cs_uint32 timen1               :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd3               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TMR_CTRL1_t;

#define     timen1_DIS                                    0x0
#define     timen1_ENA                                    0x1
#define     rlmode1_ROLL                                  0x0
#define     rlmode1_LOAD                                  0x1
#define     clksel1_direct                                0x0
#define     clksel1_dv64                                  0x1
#define     clksel1_dv1024                                0x2
#define     clksel1_dv4096                                0x3




#define PCM_PCMS_TMR_CNT1                        0x0a00f
#define PCM_PCMS_TMR_CNT1_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 count1               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 count1               : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TMR_CNT1_t;





#define PCM_PCMS_TMR_IE1                         0x0a010
#define PCM_PCMS_TMR_IE1_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmr1e                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmr1e                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TMR_IE1_t;





#define PCM_PCMS_TMR_INT1                        0x0a011
#define PCM_PCMS_TMR_INT1_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmr1i                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmr1i                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TMR_INT1_t;





#define PCM_PCMS_TMR_STAT1                       0x0a012
#define PCM_PCMS_TMR_STAT1_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmr1s                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmr1s                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TMR_STAT1_t;





#define PCM_PCMS_TMR_LD2                         0x0a013
#define PCM_PCMS_TMR_LD2_dft                     0x0000ffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 load2                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 load2                : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TMR_LD2_t;





#define PCM_PCMS_TMR_CTRL2                       0x0a014
#define PCM_PCMS_TMR_CTRL2_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 24 ;
    cs_uint32 timen2               :  1 ; /* bits 7:7 */
    cs_uint32 rlmode2              :  1 ; /* bits 6:6 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 clksel2              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd1               :  2 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  2 ;
    cs_uint32 clksel2              :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd2               :  2 ;
    cs_uint32 rlmode2              :  1 ; /* bits 6:6 */
    cs_uint32 timen2               :  1 ; /* bits 7:7 */
    cs_uint32 rsrvd3               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TMR_CTRL2_t;

#define     timen2_DIS                                    0x0
#define     timen2_ENA                                    0x1
#define     rlmode2_ROLL                                  0x0
#define     rlmode2_LOAD                                  0x1
#define     clksel2_direct                                0x0
#define     clksel2_dv64                                  0x1
#define     clksel2_dv1024                                0x2
#define     clksel2_dv4096                                0x3




#define PCM_PCMS_TMR_CNT2                        0x0a015
#define PCM_PCMS_TMR_CNT2_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 16 ;
    cs_uint32 count2               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 count2               : 16 ; /* bits 15:0 */
    cs_uint32 rsrvd1               : 16 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TMR_CNT2_t;





#define PCM_PCMS_TMR_IE2                         0x0a016
#define PCM_PCMS_TMR_IE2_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmr2e                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmr2e                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TMR_IE2_t;





#define PCM_PCMS_TMR_INT2                        0x0a017
#define PCM_PCMS_TMR_INT2_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmr2i                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmr2i                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TMR_INT2_t;





#define PCM_PCMS_TMR_STAT2                       0x0a018
#define PCM_PCMS_TMR_STAT2_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 tmr2s                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tmr2s                :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TMR_STAT2_t;





#define PCM_PCMS_TMR_LOADE                       0x0a019
#define PCM_PCMS_TMR_LOADE_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 29 ;
    cs_uint32 update_tmr2          :  1 ; /* bits 2:2 */
    cs_uint32 update_tmr1          :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 update_tmr1          :  1 ; /* bits 1:1 */
    cs_uint32 update_tmr2          :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd2               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_TMR_LOADE_t;





#define PCM_PCMS_SPMODE                          0x0a01a
#define PCM_PCMS_SPMODE_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 31 ;
    cs_uint32 early_fsync          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 early_fsync          :  1 ; /* bits 0:0 */
    cs_uint32 rsrvd1               : 31 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_SPMODE_t;





#define PCM_PCMS_DEBUG                           0x0a01b
#define PCM_PCMS_DEBUG_dft                       0x0000d188




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 bypass               :  1 ; /* bits 31:31 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 pproc_shift          :  4 ; /* bits 15:12 */
    cs_uint32 proc_shift           :  4 ; /* bits 11:8 */
    cs_uint32 rl_shift             :  4 ; /* bits 7:4 */
    cs_uint32 tl_shift             :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tl_shift             :  4 ; /* bits 3:0 */
    cs_uint32 rl_shift             :  4 ; /* bits 7:4 */
    cs_uint32 proc_shift           :  4 ; /* bits 11:8 */
    cs_uint32 pproc_shift          :  4 ; /* bits 15:12 */
    cs_uint32 rsrvd1               : 15 ;
    cs_uint32 bypass               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_DEBUG_t;





#define PCM_PCMS_FR_FIFO_DEBUG                   0x0a01c
#define PCM_PCMS_FR_FIFO_DEBUG_dft               0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 fr_tx_rdepth         :  2 ; /* bits 3:2 */
    cs_uint32 fr_rx_wdepth         :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 fr_rx_wdepth         :  2 ; /* bits 1:0 */
    cs_uint32 fr_tx_rdepth         :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMS_FR_FIFO_DEBUG_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* PCMS */

typedef struct {
  PCM_PCMS_RESET_t                         RESET;                         /* addr: 0x00000 */ 
  PCM_PCMS_CTL_t                           CTL;                           /* addr: 0x00001 */ 
  PCM_PCMS_SI_CFG_t                        SI_CFG;                        /* addr: 0x00002 */ 
  PCM_PCMS_FR_CFG_t                        FR_CFG;                        /* addr: 0x00003 */ 
  PCM_PCMS_FR_CFG2_t                       FR_CFG2;                       /* addr: 0x00004 */ 
  PCM_PCMS_TS3_t                           TS3;                           /* addr: 0x00005 */ 
  PCM_PCMS_TS2_t                           TS2;                           /* addr: 0x00006 */ 
  PCM_PCMS_TS1_t                           TS1;                           /* addr: 0x00007 */ 
  PCM_PCMS_TS0_t                           TS0;                           /* addr: 0x00008 */ 
  PCM_PCMS_RS3_t                           RS3;                           /* addr: 0x00009 */ 
  PCM_PCMS_RS2_t                           RS2;                           /* addr: 0x0000a */ 
  PCM_PCMS_RS1_t                           RS1;                           /* addr: 0x0000b */ 
  PCM_PCMS_RS0_t                           RS0;                           /* addr: 0x0000c */ 
  PCM_PCMS_TMR_LD1_t                       TMR_LD1;                       /* addr: 0x0000d */ 
  PCM_PCMS_TMR_CTRL1_t                     TMR_CTRL1;                     /* addr: 0x0000e */ 
  PCM_PCMS_TMR_CNT1_t                      TMR_CNT1;                      /* addr: 0x0000f */ 
  PCM_PCMS_TMR_IE1_t                       TMR_IE1;                       /* addr: 0x00010 */ 
  PCM_PCMS_TMR_INT1_t                      TMR_INT1;                      /* addr: 0x00011 */ 
  PCM_PCMS_TMR_STAT1_t                     TMR_STAT1;                     /* addr: 0x00012 */ 
  PCM_PCMS_TMR_LD2_t                       TMR_LD2;                       /* addr: 0x00013 */ 
  PCM_PCMS_TMR_CTRL2_t                     TMR_CTRL2;                     /* addr: 0x00014 */ 
  PCM_PCMS_TMR_CNT2_t                      TMR_CNT2;                      /* addr: 0x00015 */ 
  PCM_PCMS_TMR_IE2_t                       TMR_IE2;                       /* addr: 0x00016 */ 
  PCM_PCMS_TMR_INT2_t                      TMR_INT2;                      /* addr: 0x00017 */ 
  PCM_PCMS_TMR_STAT2_t                     TMR_STAT2;                     /* addr: 0x00018 */ 
  PCM_PCMS_TMR_LOADE_t                     TMR_LOADE;                     /* addr: 0x00019 */ 
  PCM_PCMS_SPMODE_t                        SPMODE;                        /* addr: 0x0001a */ 
  PCM_PCMS_DEBUG_t                         DEBUG;                         /* addr: 0x0001b */ 
  PCM_PCMS_FR_FIFO_DEBUG_t                 FR_FIFO_DEBUG;                 /* addr: 0x0001c */ 
} PCM_PCMS_t;

  

/*
 * PCMF
 */
  



#define PCM_PCMF_MODE                            0x0a080
#define PCM_PCMF_MODE_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 29 ;
    cs_uint32 mode1                :  1 ; /* bits 2:2 */
    cs_uint32 mode0                :  1 ; /* bits 1:1 */
    cs_uint32 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 mode0                :  1 ; /* bits 1:1 */
    cs_uint32 mode1                :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd2               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_MODE_t;

#define     mode0_DIS                                     0x0
#define     mode0_ASYM                                    0x1
#define     mode0_S32                                     0x2
#define     mode0_S64                                     0x3




#define PCM_PCMF_DEF_DATA                        0x0a081
#define PCM_PCMF_DEF_DATA_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 use_ldata            :  1 ; /* bits 8:8 */
    cs_uint32 def_txData_fast      :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 def_txData_fast      :  8 ; /* bits 7:0 */
    cs_uint32 use_ldata            :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_DEF_DATA_t;

#define     use_ldata_IDLE                                0x0
#define     use_ldata_LDAT                                0x1




#define PCM_PCMF_TX_Control_0                    0x0a082
#define PCM_PCMF_TX_Control_0_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 tx0_iload            :  2 ; /* bits 9:8 */
    cs_uint32 tx0_en               :  1 ; /* bits 7:7 */
    cs_uint32 tx0_slot             :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx0_slot             :  7 ; /* bits 6:0 */
    cs_uint32 tx0_en               :  1 ; /* bits 7:7 */
    cs_uint32 tx0_iload            :  2 ; /* bits 9:8 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_Control_0_t;





#define PCM_PCMF_TX_PTR_0                        0x0a083
#define PCM_PCMF_TX_PTR_0_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 n                    :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 n                    :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_PTR_0_t;





#define PCM_PCMF_TX_DESC_0                       0x0a084
#define PCM_PCMF_TX_DESC_0_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_DESC_0_t;

#define     HW_own_SW                                     0x0
#define     HW_own_HW                                     0x1




#define PCM_PCMF_TX_Control_1                    0x0a085
#define PCM_PCMF_TX_Control_1_dft                0x00000001




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 tx1_iload            :  2 ; /* bits 9:8 */
    cs_uint32 tx1_en               :  1 ; /* bits 7:7 */
    cs_uint32 tx1_slot             :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx1_slot             :  7 ; /* bits 6:0 */
    cs_uint32 tx1_en               :  1 ; /* bits 7:7 */
    cs_uint32 tx1_iload            :  2 ; /* bits 9:8 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_Control_1_t;





#define PCM_PCMF_TX_PTR_1                        0x0a086
#define PCM_PCMF_TX_PTR_1_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 n                    :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 n                    :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_PTR_1_t;





#define PCM_PCMF_TX_DESC_1                       0x0a087
#define PCM_PCMF_TX_DESC_1_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_DESC_1_t;





#define PCM_PCMF_TX_Control_2                    0x0a088
#define PCM_PCMF_TX_Control_2_dft                0x00000002




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 tx2_iload            :  2 ; /* bits 9:8 */
    cs_uint32 tx2_en               :  1 ; /* bits 7:7 */
    cs_uint32 tx2_slot             :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx2_slot             :  7 ; /* bits 6:0 */
    cs_uint32 tx2_en               :  1 ; /* bits 7:7 */
    cs_uint32 tx2_iload            :  2 ; /* bits 9:8 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_Control_2_t;





#define PCM_PCMF_TX_PTR_2                        0x0a089
#define PCM_PCMF_TX_PTR_2_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 n                    :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 n                    :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_PTR_2_t;





#define PCM_PCMF_TX_DESC_2                       0x0a08a
#define PCM_PCMF_TX_DESC_2_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_DESC_2_t;





#define PCM_PCMF_TX_Control_3                    0x0a08b
#define PCM_PCMF_TX_Control_3_dft                0x00000003




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 tx3_iload            :  2 ; /* bits 9:8 */
    cs_uint32 tx3_en               :  1 ; /* bits 7:7 */
    cs_uint32 tx3_slot             :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx3_slot             :  7 ; /* bits 6:0 */
    cs_uint32 tx3_en               :  1 ; /* bits 7:7 */
    cs_uint32 tx3_iload            :  2 ; /* bits 9:8 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_Control_3_t;





#define PCM_PCMF_TX_PTR_3                        0x0a08c
#define PCM_PCMF_TX_PTR_3_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 n                    :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 n                    :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_PTR_3_t;





#define PCM_PCMF_TX_DESC_3                       0x0a08d
#define PCM_PCMF_TX_DESC_3_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_DESC_3_t;





#define PCM_PCMF_TX_N                            0x0a08e
#define PCM_PCMF_TX_N_dft                        0x00000050




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 val                  : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 val                  : 10 ; /* bits 9:0 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_N_t;





#define PCM_PCMF_RX_Control_0                    0x0a08f
#define PCM_PCMF_RX_Control_0_dft                0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 rx0_iload            :  2 ; /* bits 9:8 */
    cs_uint32 rx0_en               :  1 ; /* bits 7:7 */
    cs_uint32 rx0_slot             :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx0_slot             :  7 ; /* bits 6:0 */
    cs_uint32 rx0_en               :  1 ; /* bits 7:7 */
    cs_uint32 rx0_iload            :  2 ; /* bits 9:8 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_Control_0_t;





#define PCM_PCMF_RX_PTR_0                        0x0a090
#define PCM_PCMF_RX_PTR_0_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 n                    :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 n                    :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_PTR_0_t;





#define PCM_PCMF_RX_DESC_0                       0x0a091
#define PCM_PCMF_RX_DESC_0_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_DESC_0_t;





#define PCM_PCMF_RX_Control_1                    0x0a092
#define PCM_PCMF_RX_Control_1_dft                0x00000001




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 rx1_iload            :  2 ; /* bits 9:8 */
    cs_uint32 rx1_en               :  1 ; /* bits 7:7 */
    cs_uint32 rx1_slot             :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx1_slot             :  7 ; /* bits 6:0 */
    cs_uint32 rx1_en               :  1 ; /* bits 7:7 */
    cs_uint32 rx1_iload            :  2 ; /* bits 9:8 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_Control_1_t;





#define PCM_PCMF_RX_PTR_1                        0x0a093
#define PCM_PCMF_RX_PTR_1_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 n                    :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 n                    :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_PTR_1_t;





#define PCM_PCMF_RX_DESC_1                       0x0a094
#define PCM_PCMF_RX_DESC_1_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_DESC_1_t;





#define PCM_PCMF_RX_Control_2                    0x0a095
#define PCM_PCMF_RX_Control_2_dft                0x00000002




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 rx2_iload            :  2 ; /* bits 9:8 */
    cs_uint32 rx2_en               :  1 ; /* bits 7:7 */
    cs_uint32 rx2_slot             :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx2_slot             :  7 ; /* bits 6:0 */
    cs_uint32 rx2_en               :  1 ; /* bits 7:7 */
    cs_uint32 rx2_iload            :  2 ; /* bits 9:8 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_Control_2_t;





#define PCM_PCMF_RX_PTR_2                        0x0a096
#define PCM_PCMF_RX_PTR_2_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 n                    :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 n                    :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_PTR_2_t;





#define PCM_PCMF_RX_DESC_2                       0x0a097
#define PCM_PCMF_RX_DESC_2_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_DESC_2_t;





#define PCM_PCMF_RX_Control_3                    0x0a098
#define PCM_PCMF_RX_Control_3_dft                0x00000003




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 rx3_iload            :  2 ; /* bits 9:8 */
    cs_uint32 rx3_en               :  1 ; /* bits 7:7 */
    cs_uint32 rx3_slot             :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx3_slot             :  7 ; /* bits 6:0 */
    cs_uint32 rx3_en               :  1 ; /* bits 7:7 */
    cs_uint32 rx3_iload            :  2 ; /* bits 9:8 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_Control_3_t;





#define PCM_PCMF_RX_PTR_3                        0x0a099
#define PCM_PCMF_RX_PTR_3_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 n                    :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 n                    :  3 ; /* bits 2:0 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_PTR_3_t;





#define PCM_PCMF_RX_DESC_3                       0x0a09a
#define PCM_PCMF_RX_DESC_3_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 HW_own               :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_DESC_3_t;





#define PCM_PCMF_RX_N                            0x0a09b
#define PCM_PCMF_RX_N_dft                        0x00000050




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 val                  : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 val                  : 10 ; /* bits 9:0 */
    cs_uint32 rsrvd1               : 22 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_N_t;





#define PCM_PCMF_TX_BUFFER_BASE_0                0x0a09c
#define PCM_PCMF_TX_BUFFER_BASE_0_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 21 ;
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               : 21 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_BUFFER_BASE_0_t;





#define PCM_PCMF_TX_BUFFER_BASE_1                0x0a09d
#define PCM_PCMF_TX_BUFFER_BASE_1_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 21 ;
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               : 21 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_BUFFER_BASE_1_t;





#define PCM_PCMF_TX_BUFFER_BASE_2                0x0a09e
#define PCM_PCMF_TX_BUFFER_BASE_2_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 21 ;
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               : 21 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_BUFFER_BASE_2_t;





#define PCM_PCMF_TX_BUFFER_BASE_3                0x0a09f
#define PCM_PCMF_TX_BUFFER_BASE_3_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 21 ;
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               : 21 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_BUFFER_BASE_3_t;





#define PCM_PCMF_RX_BUFFER_BASE_0                0x0a0a0
#define PCM_PCMF_RX_BUFFER_BASE_0_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 21 ;
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               : 21 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_BUFFER_BASE_0_t;





#define PCM_PCMF_RX_BUFFER_BASE_1                0x0a0a1
#define PCM_PCMF_RX_BUFFER_BASE_1_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 21 ;
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               : 21 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_BUFFER_BASE_1_t;





#define PCM_PCMF_RX_BUFFER_BASE_2                0x0a0a2
#define PCM_PCMF_RX_BUFFER_BASE_2_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 21 ;
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               : 21 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_BUFFER_BASE_2_t;





#define PCM_PCMF_RX_BUFFER_BASE_3                0x0a0a3
#define PCM_PCMF_RX_BUFFER_BASE_3_dft            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 21 ;
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 addr                 : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               : 21 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_BUFFER_BASE_3_t;





#define PCM_PCMF_N                               0x0a0a4
#define PCM_PCMF_N_dft                           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 frameNum             : 16 ; /* bits 31:16 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 rxslotNum            :  7 ; /* bits 14:8 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 txslotNum            :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 txslotNum            :  7 ; /* bits 6:0 */
    cs_uint32 rsrvd1               :  1 ;
    cs_uint32 rxslotNum            :  7 ; /* bits 14:8 */
    cs_uint32 rsrvd2               :  1 ;
    cs_uint32 frameNum             : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_N_t;





#define PCM_PCMF_DESC_MASK                       0x0a0a5
#define PCM_PCMF_DESC_MASK_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd2               : 19 ;
    cs_uint32 rx_mask_mode         :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 tx_mask_mode         :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx_mask_mode         :  5 ; /* bits 4:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 rx_mask_mode         :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd2               : 19 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_DESC_MASK_t;

#define     rx_mask_mode_DIS                              0x0
#define     rx_mask_mode_STRICT_BUFF1                     0x1
#define     rx_mask_mode_STRICT_BUFF2                     0x2
#define     rx_mask_mode_STRICT_BUFF3                     0x3
#define     rx_mask_mode_STRICT_BUFF4                     0x4
#define     rx_mask_mode_STRICT_BUFF5                     0x5
#define     rx_mask_mode_STRICT_BUFF6                     0x6
#define     rx_mask_mode_STRICT_BUFF7                     0x7
#define     rx_mask_mode_BUFF1                            0x8
#define     rx_mask_mode_BUFF2                            0x9
#define     rx_mask_mode_BUFF3                            0xa
#define     rx_mask_mode_BUFF4                            0xb
#define     rx_mask_mode_BUFF5                            0xc
#define     rx_mask_mode_BUFF6                            0xd
#define     rx_mask_mode_LBUFF1                           0xe
#define     rx_mask_mode_LBUFF2                           0xf
#define     rx_mask_mode_LBUFF3                           0x10
#define     rx_mask_mode_LBUFF4                           0x11
#define     rx_mask_mode_LBUFF5                           0x12
#define     tx_mask_mode_DIS                              0x0
#define     tx_mask_mode_STRICT_BUFF1                     0x1
#define     tx_mask_mode_STRICT_BUFF2                     0x2
#define     tx_mask_mode_STRICT_BUFF3                     0x3
#define     tx_mask_mode_STRICT_BUFF4                     0x4
#define     tx_mask_mode_STRICT_BUFF5                     0x5
#define     tx_mask_mode_STRICT_BUFF6                     0x6
#define     tx_mask_mode_STRICT_BUFF7                     0x7
#define     tx_mask_mode_BUFF1                            0x8
#define     tx_mask_mode_BUFF2                            0x9
#define     tx_mask_mode_BUFF3                            0xa
#define     tx_mask_mode_BUFF4                            0xb
#define     tx_mask_mode_BUFF5                            0xc
#define     tx_mask_mode_BUFF6                            0xd
#define     tx_mask_mode_LBUFF1                           0xe
#define     tx_mask_mode_LBUFF2                           0xf
#define     tx_mask_mode_LBUFF3                           0x10
#define     tx_mask_mode_LBUFF4                           0x11
#define     tx_mask_mode_LBUFF5                           0x12




#define PCM_PCMF_TX_FAILED_DESC                  0x0a0a6
#define PCM_PCMF_TX_FAILED_DESC_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 tx3_failed_desc      :  8 ; /* bits 31:24 */
    cs_uint32 tx2_failed_desc      :  8 ; /* bits 23:16 */
    cs_uint32 tx1_failed_desc      :  8 ; /* bits 15:8 */
    cs_uint32 tx0_failed_desc      :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx0_failed_desc      :  8 ; /* bits 7:0 */
    cs_uint32 tx1_failed_desc      :  8 ; /* bits 15:8 */
    cs_uint32 tx2_failed_desc      :  8 ; /* bits 23:16 */
    cs_uint32 tx3_failed_desc      :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_TX_FAILED_DESC_t;





#define PCM_PCMF_RX_FAILED_DESC                  0x0a0a7
#define PCM_PCMF_RX_FAILED_DESC_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rx3_failed_desc      :  8 ; /* bits 31:24 */
    cs_uint32 rx2_failed_desc      :  8 ; /* bits 23:16 */
    cs_uint32 rx1_failed_desc      :  8 ; /* bits 15:8 */
    cs_uint32 rx0_failed_desc      :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rx0_failed_desc      :  8 ; /* bits 7:0 */
    cs_uint32 rx1_failed_desc      :  8 ; /* bits 15:8 */
    cs_uint32 rx2_failed_desc      :  8 ; /* bits 23:16 */
    cs_uint32 rx3_failed_desc      :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_RX_FAILED_DESC_t;





#define PCM_PCMF_PCM_IE                          0x0a0a8
#define PCM_PCMF_PCM_IE_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rx3_bad_maske        :  1 ; /* bits 31:31 */
    cs_uint32 rx2_bad_maske        :  1 ; /* bits 30:30 */
    cs_uint32 rx1_bad_maske        :  1 ; /* bits 29:29 */
    cs_uint32 rx0_bad_maske        :  1 ; /* bits 28:28 */
    cs_uint32 tx3_bad_maske        :  1 ; /* bits 27:27 */
    cs_uint32 tx2_bad_maske        :  1 ; /* bits 26:26 */
    cs_uint32 tx1_bad_maske        :  1 ; /* bits 25:25 */
    cs_uint32 tx0_bad_maske        :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 tmr2e                :  1 ; /* bits 17:17 */
    cs_uint32 tmr1e                :  1 ; /* bits 16:16 */
    cs_uint32 rx3erre              :  1 ; /* bits 15:15 */
    cs_uint32 rx2erre              :  1 ; /* bits 14:14 */
    cs_uint32 rx1erre              :  1 ; /* bits 13:13 */
    cs_uint32 rx0erre              :  1 ; /* bits 12:12 */
    cs_uint32 tx3erre              :  1 ; /* bits 11:11 */
    cs_uint32 tx2erre              :  1 ; /* bits 10:10 */
    cs_uint32 tx1erre              :  1 ; /* bits 9:9 */
    cs_uint32 tx0erre              :  1 ; /* bits 8:8 */
    cs_uint32 rx3e                 :  1 ; /* bits 7:7 */
    cs_uint32 rx2e                 :  1 ; /* bits 6:6 */
    cs_uint32 rx1e                 :  1 ; /* bits 5:5 */
    cs_uint32 rx0e                 :  1 ; /* bits 4:4 */
    cs_uint32 tx3e                 :  1 ; /* bits 3:3 */
    cs_uint32 tx2e                 :  1 ; /* bits 2:2 */
    cs_uint32 tx1e                 :  1 ; /* bits 1:1 */
    cs_uint32 tx0e                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx0e                 :  1 ; /* bits 0:0 */
    cs_uint32 tx1e                 :  1 ; /* bits 1:1 */
    cs_uint32 tx2e                 :  1 ; /* bits 2:2 */
    cs_uint32 tx3e                 :  1 ; /* bits 3:3 */
    cs_uint32 rx0e                 :  1 ; /* bits 4:4 */
    cs_uint32 rx1e                 :  1 ; /* bits 5:5 */
    cs_uint32 rx2e                 :  1 ; /* bits 6:6 */
    cs_uint32 rx3e                 :  1 ; /* bits 7:7 */
    cs_uint32 tx0erre              :  1 ; /* bits 8:8 */
    cs_uint32 tx1erre              :  1 ; /* bits 9:9 */
    cs_uint32 tx2erre              :  1 ; /* bits 10:10 */
    cs_uint32 tx3erre              :  1 ; /* bits 11:11 */
    cs_uint32 rx0erre              :  1 ; /* bits 12:12 */
    cs_uint32 rx1erre              :  1 ; /* bits 13:13 */
    cs_uint32 rx2erre              :  1 ; /* bits 14:14 */
    cs_uint32 rx3erre              :  1 ; /* bits 15:15 */
    cs_uint32 tmr1e                :  1 ; /* bits 16:16 */
    cs_uint32 tmr2e                :  1 ; /* bits 17:17 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 tx0_bad_maske        :  1 ; /* bits 24:24 */
    cs_uint32 tx1_bad_maske        :  1 ; /* bits 25:25 */
    cs_uint32 tx2_bad_maske        :  1 ; /* bits 26:26 */
    cs_uint32 tx3_bad_maske        :  1 ; /* bits 27:27 */
    cs_uint32 rx0_bad_maske        :  1 ; /* bits 28:28 */
    cs_uint32 rx1_bad_maske        :  1 ; /* bits 29:29 */
    cs_uint32 rx2_bad_maske        :  1 ; /* bits 30:30 */
    cs_uint32 rx3_bad_maske        :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_PCM_IE_t;





#define PCM_PCMF_PCM_INT                         0x0a0a9
#define PCM_PCMF_PCM_INT_dft                     0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rx3_bad_maski        :  1 ; /* bits 31:31 */
    cs_uint32 rx2_bad_maski        :  1 ; /* bits 30:30 */
    cs_uint32 rx1_bad_maski        :  1 ; /* bits 29:29 */
    cs_uint32 rx0_bad_maski        :  1 ; /* bits 28:28 */
    cs_uint32 tx3_bad_maski        :  1 ; /* bits 27:27 */
    cs_uint32 tx2_bad_maski        :  1 ; /* bits 26:26 */
    cs_uint32 tx1_bad_maski        :  1 ; /* bits 25:25 */
    cs_uint32 tx0_bad_maski        :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 tmr2i                :  1 ; /* bits 17:17 */
    cs_uint32 tmr1i                :  1 ; /* bits 16:16 */
    cs_uint32 rx3erri              :  1 ; /* bits 15:15 */
    cs_uint32 rx2erri              :  1 ; /* bits 14:14 */
    cs_uint32 rx1erri              :  1 ; /* bits 13:13 */
    cs_uint32 rx0erri              :  1 ; /* bits 12:12 */
    cs_uint32 tx3erri              :  1 ; /* bits 11:11 */
    cs_uint32 tx2erri              :  1 ; /* bits 10:10 */
    cs_uint32 tx1erri              :  1 ; /* bits 9:9 */
    cs_uint32 tx0erri              :  1 ; /* bits 8:8 */
    cs_uint32 rx3i                 :  1 ; /* bits 7:7 */
    cs_uint32 rx2i                 :  1 ; /* bits 6:6 */
    cs_uint32 rx1i                 :  1 ; /* bits 5:5 */
    cs_uint32 rx0i                 :  1 ; /* bits 4:4 */
    cs_uint32 tx3i                 :  1 ; /* bits 3:3 */
    cs_uint32 tx2i                 :  1 ; /* bits 2:2 */
    cs_uint32 tx1i                 :  1 ; /* bits 1:1 */
    cs_uint32 tx0i                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx0i                 :  1 ; /* bits 0:0 */
    cs_uint32 tx1i                 :  1 ; /* bits 1:1 */
    cs_uint32 tx2i                 :  1 ; /* bits 2:2 */
    cs_uint32 tx3i                 :  1 ; /* bits 3:3 */
    cs_uint32 rx0i                 :  1 ; /* bits 4:4 */
    cs_uint32 rx1i                 :  1 ; /* bits 5:5 */
    cs_uint32 rx2i                 :  1 ; /* bits 6:6 */
    cs_uint32 rx3i                 :  1 ; /* bits 7:7 */
    cs_uint32 tx0erri              :  1 ; /* bits 8:8 */
    cs_uint32 tx1erri              :  1 ; /* bits 9:9 */
    cs_uint32 tx2erri              :  1 ; /* bits 10:10 */
    cs_uint32 tx3erri              :  1 ; /* bits 11:11 */
    cs_uint32 rx0erri              :  1 ; /* bits 12:12 */
    cs_uint32 rx1erri              :  1 ; /* bits 13:13 */
    cs_uint32 rx2erri              :  1 ; /* bits 14:14 */
    cs_uint32 rx3erri              :  1 ; /* bits 15:15 */
    cs_uint32 tmr1i                :  1 ; /* bits 16:16 */
    cs_uint32 tmr2i                :  1 ; /* bits 17:17 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 tx0_bad_maski        :  1 ; /* bits 24:24 */
    cs_uint32 tx1_bad_maski        :  1 ; /* bits 25:25 */
    cs_uint32 tx2_bad_maski        :  1 ; /* bits 26:26 */
    cs_uint32 tx3_bad_maski        :  1 ; /* bits 27:27 */
    cs_uint32 rx0_bad_maski        :  1 ; /* bits 28:28 */
    cs_uint32 rx1_bad_maski        :  1 ; /* bits 29:29 */
    cs_uint32 rx2_bad_maski        :  1 ; /* bits 30:30 */
    cs_uint32 rx3_bad_maski        :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_PCM_INT_t;





#define PCM_PCMF_PCM_STAT                        0x0a0aa
#define PCM_PCMF_PCM_STAT_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rx3_bad_masks        :  1 ; /* bits 31:31 */
    cs_uint32 rx2_bad_masks        :  1 ; /* bits 30:30 */
    cs_uint32 rx1_bad_masks        :  1 ; /* bits 29:29 */
    cs_uint32 rx0_bad_masks        :  1 ; /* bits 28:28 */
    cs_uint32 tx3_bad_masks        :  1 ; /* bits 27:27 */
    cs_uint32 tx2_bad_masks        :  1 ; /* bits 26:26 */
    cs_uint32 tx1_bad_masks        :  1 ; /* bits 25:25 */
    cs_uint32 tx0_bad_masks        :  1 ; /* bits 24:24 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 tmr2s                :  1 ; /* bits 17:17 */
    cs_uint32 tmr1s                :  1 ; /* bits 16:16 */
    cs_uint32 rx3errs              :  1 ; /* bits 15:15 */
    cs_uint32 rx2errs              :  1 ; /* bits 14:14 */
    cs_uint32 rx1errs              :  1 ; /* bits 13:13 */
    cs_uint32 rx0errs              :  1 ; /* bits 12:12 */
    cs_uint32 tx3errs              :  1 ; /* bits 11:11 */
    cs_uint32 tx2errs              :  1 ; /* bits 10:10 */
    cs_uint32 tx1errs              :  1 ; /* bits 9:9 */
    cs_uint32 tx0errs              :  1 ; /* bits 8:8 */
    cs_uint32 rx3s                 :  1 ; /* bits 7:7 */
    cs_uint32 rx2s                 :  1 ; /* bits 6:6 */
    cs_uint32 rx1s                 :  1 ; /* bits 5:5 */
    cs_uint32 rx0s                 :  1 ; /* bits 4:4 */
    cs_uint32 tx3s                 :  1 ; /* bits 3:3 */
    cs_uint32 tx2s                 :  1 ; /* bits 2:2 */
    cs_uint32 tx1s                 :  1 ; /* bits 1:1 */
    cs_uint32 tx0s                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tx0s                 :  1 ; /* bits 0:0 */
    cs_uint32 tx1s                 :  1 ; /* bits 1:1 */
    cs_uint32 tx2s                 :  1 ; /* bits 2:2 */
    cs_uint32 tx3s                 :  1 ; /* bits 3:3 */
    cs_uint32 rx0s                 :  1 ; /* bits 4:4 */
    cs_uint32 rx1s                 :  1 ; /* bits 5:5 */
    cs_uint32 rx2s                 :  1 ; /* bits 6:6 */
    cs_uint32 rx3s                 :  1 ; /* bits 7:7 */
    cs_uint32 tx0errs              :  1 ; /* bits 8:8 */
    cs_uint32 tx1errs              :  1 ; /* bits 9:9 */
    cs_uint32 tx2errs              :  1 ; /* bits 10:10 */
    cs_uint32 tx3errs              :  1 ; /* bits 11:11 */
    cs_uint32 rx0errs              :  1 ; /* bits 12:12 */
    cs_uint32 rx1errs              :  1 ; /* bits 13:13 */
    cs_uint32 rx2errs              :  1 ; /* bits 14:14 */
    cs_uint32 rx3errs              :  1 ; /* bits 15:15 */
    cs_uint32 tmr1s                :  1 ; /* bits 16:16 */
    cs_uint32 tmr2s                :  1 ; /* bits 17:17 */
    cs_uint32 rsrvd1               :  6 ;
    cs_uint32 tx0_bad_masks        :  1 ; /* bits 24:24 */
    cs_uint32 tx1_bad_masks        :  1 ; /* bits 25:25 */
    cs_uint32 tx2_bad_masks        :  1 ; /* bits 26:26 */
    cs_uint32 tx3_bad_masks        :  1 ; /* bits 27:27 */
    cs_uint32 rx0_bad_masks        :  1 ; /* bits 28:28 */
    cs_uint32 rx1_bad_masks        :  1 ; /* bits 29:29 */
    cs_uint32 rx2_bad_masks        :  1 ; /* bits 30:30 */
    cs_uint32 rx3_bad_masks        :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_PCM_STAT_t;





#define PCM_PCMF_CNT_FIFO_DEBUG                  0x0a0ab
#define PCM_PCMF_CNT_FIFO_DEBUG_dft              0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 28 ;
    cs_uint32 cnt_tx_wdepth        :  2 ; /* bits 3:2 */
    cs_uint32 cnt_rx_rdepth        :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cnt_rx_rdepth        :  2 ; /* bits 1:0 */
    cs_uint32 cnt_tx_wdepth        :  2 ; /* bits 3:2 */
    cs_uint32 rsrvd1               : 28 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} PCM_PCMF_CNT_FIFO_DEBUG_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* PCMF */

typedef struct {
  PCM_PCMF_MODE_t                          MODE;                          /* addr: 0x00000 */ 
  PCM_PCMF_DEF_DATA_t                      DEF_DATA;                      /* addr: 0x00001 */ 
  PCM_PCMF_TX_Control_0_t                  TX_Control_0;                  /* addr: 0x00002 */ 
  PCM_PCMF_TX_PTR_0_t                      TX_PTR_0;                      /* addr: 0x00003 */ 
  PCM_PCMF_TX_DESC_0_t                     TX_DESC_0;                     /* addr: 0x00004 */ 
  PCM_PCMF_TX_Control_1_t                  TX_Control_1;                  /* addr: 0x00005 */ 
  PCM_PCMF_TX_PTR_1_t                      TX_PTR_1;                      /* addr: 0x00006 */ 
  PCM_PCMF_TX_DESC_1_t                     TX_DESC_1;                     /* addr: 0x00007 */ 
  PCM_PCMF_TX_Control_2_t                  TX_Control_2;                  /* addr: 0x00008 */ 
  PCM_PCMF_TX_PTR_2_t                      TX_PTR_2;                      /* addr: 0x00009 */ 
  PCM_PCMF_TX_DESC_2_t                     TX_DESC_2;                     /* addr: 0x0000a */ 
  PCM_PCMF_TX_Control_3_t                  TX_Control_3;                  /* addr: 0x0000b */ 
  PCM_PCMF_TX_PTR_3_t                      TX_PTR_3;                      /* addr: 0x0000c */ 
  PCM_PCMF_TX_DESC_3_t                     TX_DESC_3;                     /* addr: 0x0000d */ 
  PCM_PCMF_TX_N_t                          TX_N;                          /* addr: 0x0000e */ 
  PCM_PCMF_RX_Control_0_t                  RX_Control_0;                  /* addr: 0x0000f */ 
  PCM_PCMF_RX_PTR_0_t                      RX_PTR_0;                      /* addr: 0x00010 */ 
  PCM_PCMF_RX_DESC_0_t                     RX_DESC_0;                     /* addr: 0x00011 */ 
  PCM_PCMF_RX_Control_1_t                  RX_Control_1;                  /* addr: 0x00012 */ 
  PCM_PCMF_RX_PTR_1_t                      RX_PTR_1;                      /* addr: 0x00013 */ 
  PCM_PCMF_RX_DESC_1_t                     RX_DESC_1;                     /* addr: 0x00014 */ 
  PCM_PCMF_RX_Control_2_t                  RX_Control_2;                  /* addr: 0x00015 */ 
  PCM_PCMF_RX_PTR_2_t                      RX_PTR_2;                      /* addr: 0x00016 */ 
  PCM_PCMF_RX_DESC_2_t                     RX_DESC_2;                     /* addr: 0x00017 */ 
  PCM_PCMF_RX_Control_3_t                  RX_Control_3;                  /* addr: 0x00018 */ 
  PCM_PCMF_RX_PTR_3_t                      RX_PTR_3;                      /* addr: 0x00019 */ 
  PCM_PCMF_RX_DESC_3_t                     RX_DESC_3;                     /* addr: 0x0001a */ 
  PCM_PCMF_RX_N_t                          RX_N;                          /* addr: 0x0001b */ 
  PCM_PCMF_TX_BUFFER_BASE_0_t              TX_BUFFER_BASE_0;              /* addr: 0x0001c */ 
  PCM_PCMF_TX_BUFFER_BASE_1_t              TX_BUFFER_BASE_1;              /* addr: 0x0001d */ 
  PCM_PCMF_TX_BUFFER_BASE_2_t              TX_BUFFER_BASE_2;              /* addr: 0x0001e */ 
  PCM_PCMF_TX_BUFFER_BASE_3_t              TX_BUFFER_BASE_3;              /* addr: 0x0001f */ 
  PCM_PCMF_RX_BUFFER_BASE_0_t              RX_BUFFER_BASE_0;              /* addr: 0x00020 */ 
  PCM_PCMF_RX_BUFFER_BASE_1_t              RX_BUFFER_BASE_1;              /* addr: 0x00021 */ 
  PCM_PCMF_RX_BUFFER_BASE_2_t              RX_BUFFER_BASE_2;              /* addr: 0x00022 */ 
  PCM_PCMF_RX_BUFFER_BASE_3_t              RX_BUFFER_BASE_3;              /* addr: 0x00023 */ 
  PCM_PCMF_N_t                             N;                             /* addr: 0x00024 */ 
  PCM_PCMF_DESC_MASK_t                     DESC_MASK;                     /* addr: 0x00025 */ 
  PCM_PCMF_TX_FAILED_DESC_t                TX_FAILED_DESC;                /* addr: 0x00026 */ 
  PCM_PCMF_RX_FAILED_DESC_t                RX_FAILED_DESC;                /* addr: 0x00027 */ 
  PCM_PCMF_PCM_IE_t                        PCM_IE;                        /* addr: 0x00028 */ 
  PCM_PCMF_PCM_INT_t                       PCM_INT;                       /* addr: 0x00029 */ 
  PCM_PCMF_PCM_STAT_t                      PCM_STAT;                      /* addr: 0x0002a */ 
  PCM_PCMF_CNT_FIFO_DEBUG_t                CNT_FIFO_DEBUG;                /* addr: 0x0002b */ 
} PCM_PCMF_t;

  

  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* pcm */

typedef struct {
  PCM_PCMS_t                               PCMS;                          /* addr: 0x00000 */ 
  cs_uint32                                rsrvd1[99];                   
  PCM_PCMF_t                               PCMF;                          /* addr: 0x00080 */ 
} PCM_t;


/*
 * l_dma
 */
  



#define L_DMA_CTL0                               0x0b000
#define L_DMA_CTL0_dft                           0x00000030




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 25 ;
    cs_uint32 npkt_n               :  1 ; /* bits 6:6 */
    cs_uint32 dai                  :  1 ; /* bits 5:5 */
    cs_uint32 sai                  :  1 ; /* bits 4:4 */
    cs_uint32 instr                :  2 ; /* bits 3:2 */
    cs_uint32 autoi                :  1 ; /* bits 1:1 */
    cs_uint32 en                   :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 en                   :  1 ; /* bits 0:0 */
    cs_uint32 autoi                :  1 ; /* bits 1:1 */
    cs_uint32 instr                :  2 ; /* bits 3:2 */
    cs_uint32 sai                  :  1 ; /* bits 4:4 */
    cs_uint32 dai                  :  1 ; /* bits 5:5 */
    cs_uint32 npkt_n               :  1 ; /* bits 6:6 */
    cs_uint32 rsrvd1               : 25 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_CTL0_t;

#define     npkt_n_NWP                                    0x0
#define     npkt_n_DAT                                    0x1
#define     dai_DEC                                       0x0
#define     dai_INC                                       0x1
#define     sai_DEC                                       0x0
#define     sai_INC                                       0x1
#define     instr_READREG                                 0x0
#define     instr_WRITEREG                                0x1
#define     instr_COPY                                    0x2
#define     instr_CRC                                     0x3
#define     autoi_CHAIN                                   0x0
#define     autoi_AUTOI                                   0x1
#define     en_DIS                                        0x0
#define     en_ENA                                        0x1




#define L_DMA_VCTRBASE0                          0x0b001
#define L_DMA_VCTRBASE0_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 vctrbase             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 vctrbase             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_VCTRBASE0_t;





#define L_DMA_WRPTR0                             0x0b002
#define L_DMA_WRPTR0_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 25 ;
    cs_uint32 wr_ptr               :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 wr_ptr               :  7 ; /* bits 6:0 */
    cs_uint32 rsrvd1               : 25 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_WRPTR0_t;





#define L_DMA_RDPTR0                             0x0b003
#define L_DMA_RDPTR0_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 25 ;
    cs_uint32 rd_ptr               :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rd_ptr               :  7 ; /* bits 6:0 */
    cs_uint32 rsrvd1               : 25 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_RDPTR0_t;





#define L_DMA_REGA0                              0x0b004
#define L_DMA_REGA0_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rega                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rega                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_REGA0_t;





#define L_DMA_MA0                                0x0b005
#define L_DMA_MA0_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ma                   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ma                   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_MA0_t;





#define L_DMA_NW0                                0x0b006
#define L_DMA_NW0_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 nw                   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 nw                   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_NW0_t;





#define L_DMA_VACS0                              0x0b007
#define L_DMA_VACS0_dft                          0x00008000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 vacs                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 vacs                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_VACS0_t;





#define L_DMA_IT_INC0                            0x0b008
#define L_DMA_IT_INC0_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 it                   : 16 ; /* bits 31:16 */
    cs_uint32 inc                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 inc                  : 16 ; /* bits 15:0 */
    cs_uint32 it                   : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_IT_INC0_t;





#define L_DMA_PER0                               0x0b009
#define L_DMA_PER0_dft                           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 poll_per             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 poll_per             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_PER0_t;





#define L_DMA_SA0                                0x0b00a
#define L_DMA_SA0_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 sa                   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sa                   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_SA0_t;





#define L_DMA_DA0                                0x0b00b
#define L_DMA_DA0_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 da                   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 da                   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_DA0_t;





#define L_DMA_NB0                                0x0b00c
#define L_DMA_NB0_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 nb                   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 nb                   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_NB0_t;





#define L_DMA_CRCSD0                             0x0b00d
#define L_DMA_CRCSD0_dft                         0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 crcsd                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 crcsd                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_CRCSD0_t;





#define L_DMA_CRCRES0                            0x0b00e
#define L_DMA_CRCRES0_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 crcres               : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 crcres               : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_CRCRES0_t;





#define L_DMA_IE0                                0x0b00f
#define L_DMA_IE0_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 ahb_erre             :  1 ; /* bits 2:2 */
    cs_uint32 bce                  :  1 ; /* bits 1:1 */
    cs_uint32 tce                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tce                  :  1 ; /* bits 0:0 */
    cs_uint32 bce                  :  1 ; /* bits 1:1 */
    cs_uint32 ahb_erre             :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_IE0_t;





#define L_DMA_INT0                               0x0b010
#define L_DMA_INT0_dft                           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 ahb_erri             :  1 ; /* bits 2:2 */
    cs_uint32 bci                  :  1 ; /* bits 1:1 */
    cs_uint32 tci                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tci                  :  1 ; /* bits 0:0 */
    cs_uint32 bci                  :  1 ; /* bits 1:1 */
    cs_uint32 ahb_erri             :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_INT0_t;





#define L_DMA_STAT0                              0x0b011
#define L_DMA_STAT0_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 ahb_errs             :  1 ; /* bits 2:2 */
    cs_uint32 bcs                  :  1 ; /* bits 1:1 */
    cs_uint32 tcs                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tcs                  :  1 ; /* bits 0:0 */
    cs_uint32 bcs                  :  1 ; /* bits 1:1 */
    cs_uint32 ahb_errs             :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_STAT0_t;





#define L_DMA_SPARE0                             0x0b012
#define L_DMA_SPARE0_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 spare                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 spare                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_SPARE0_t;





#define L_DMA_CTL1                               0x0b013
#define L_DMA_CTL1_dft                           0x00000030




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 25 ;
    cs_uint32 npkt_n               :  1 ; /* bits 6:6 */
    cs_uint32 dai                  :  1 ; /* bits 5:5 */
    cs_uint32 sai                  :  1 ; /* bits 4:4 */
    cs_uint32 instr                :  2 ; /* bits 3:2 */
    cs_uint32 autoi                :  1 ; /* bits 1:1 */
    cs_uint32 en                   :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 en                   :  1 ; /* bits 0:0 */
    cs_uint32 autoi                :  1 ; /* bits 1:1 */
    cs_uint32 instr                :  2 ; /* bits 3:2 */
    cs_uint32 sai                  :  1 ; /* bits 4:4 */
    cs_uint32 dai                  :  1 ; /* bits 5:5 */
    cs_uint32 npkt_n               :  1 ; /* bits 6:6 */
    cs_uint32 rsrvd1               : 25 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_CTL1_t;





#define L_DMA_VCTRBASE1                          0x0b014
#define L_DMA_VCTRBASE1_dft                      0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 vctrbase             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 vctrbase             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_VCTRBASE1_t;





#define L_DMA_WRPTR1                             0x0b015
#define L_DMA_WRPTR1_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 25 ;
    cs_uint32 wr_ptr               :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 wr_ptr               :  7 ; /* bits 6:0 */
    cs_uint32 rsrvd1               : 25 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_WRPTR1_t;





#define L_DMA_RDPTR1                             0x0b016
#define L_DMA_RDPTR1_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 25 ;
    cs_uint32 rd_ptr               :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rd_ptr               :  7 ; /* bits 6:0 */
    cs_uint32 rsrvd1               : 25 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_RDPTR1_t;





#define L_DMA_REGA1                              0x0b017
#define L_DMA_REGA1_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rega                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rega                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_REGA1_t;





#define L_DMA_MA1                                0x0b018
#define L_DMA_MA1_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 ma                   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 ma                   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_MA1_t;





#define L_DMA_NW1                                0x0b019
#define L_DMA_NW1_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 nw                   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 nw                   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_NW1_t;





#define L_DMA_VACS1                              0x0b01a
#define L_DMA_VACS1_dft                          0x00008000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 vacs                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 vacs                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_VACS1_t;





#define L_DMA_IT_INC1                            0x0b01b
#define L_DMA_IT_INC1_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 it                   : 16 ; /* bits 31:16 */
    cs_uint32 inc                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 inc                  : 16 ; /* bits 15:0 */
    cs_uint32 it                   : 16 ; /* bits 31:16 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_IT_INC1_t;





#define L_DMA_PER1                               0x0b01c
#define L_DMA_PER1_dft                           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 poll_per             : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 poll_per             : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_PER1_t;





#define L_DMA_SA1                                0x0b01d
#define L_DMA_SA1_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 sa                   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sa                   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_SA1_t;





#define L_DMA_DA1                                0x0b01e
#define L_DMA_DA1_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 da                   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 da                   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_DA1_t;





#define L_DMA_NB1                                0x0b01f
#define L_DMA_NB1_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 nb                   : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 nb                   : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_NB1_t;





#define L_DMA_CRCSD1                             0x0b020
#define L_DMA_CRCSD1_dft                         0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 crcsd                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 crcsd                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_CRCSD1_t;





#define L_DMA_CRCRES1                            0x0b021
#define L_DMA_CRCRES1_dft                        0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 crcres               : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 crcres               : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_CRCRES1_t;





#define L_DMA_IE1                                0x0b022
#define L_DMA_IE1_dft                            0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 ahb_erre             :  1 ; /* bits 2:2 */
    cs_uint32 bce                  :  1 ; /* bits 1:1 */
    cs_uint32 tce                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tce                  :  1 ; /* bits 0:0 */
    cs_uint32 bce                  :  1 ; /* bits 1:1 */
    cs_uint32 ahb_erre             :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_IE1_t;





#define L_DMA_INT1                               0x0b023
#define L_DMA_INT1_dft                           0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 ahb_erri             :  1 ; /* bits 2:2 */
    cs_uint32 bci                  :  1 ; /* bits 1:1 */
    cs_uint32 tci                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tci                  :  1 ; /* bits 0:0 */
    cs_uint32 bci                  :  1 ; /* bits 1:1 */
    cs_uint32 ahb_erri             :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_INT1_t;





#define L_DMA_STAT1                              0x0b024
#define L_DMA_STAT1_dft                          0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 29 ;
    cs_uint32 ahb_errs             :  1 ; /* bits 2:2 */
    cs_uint32 bcs                  :  1 ; /* bits 1:1 */
    cs_uint32 tcs                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tcs                  :  1 ; /* bits 0:0 */
    cs_uint32 bcs                  :  1 ; /* bits 1:1 */
    cs_uint32 ahb_errs             :  1 ; /* bits 2:2 */
    cs_uint32 rsrvd1               : 29 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_STAT1_t;





#define L_DMA_SPARE1                             0x0b025
#define L_DMA_SPARE1_dft                         0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 spare                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 spare                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} L_DMA_SPARE1_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* l_dma */

typedef struct {
  L_DMA_CTL0_t                             CTL0;                          /* addr: 0x00000 */ 
  L_DMA_VCTRBASE0_t                        VCTRBASE0;                     /* addr: 0x00001 */ 
  L_DMA_WRPTR0_t                           WRPTR0;                        /* addr: 0x00002 */ 
  L_DMA_RDPTR0_t                           RDPTR0;                        /* addr: 0x00003 */ 
  L_DMA_REGA0_t                            REGA0;                         /* addr: 0x00004 */ 
  L_DMA_MA0_t                              MA0;                           /* addr: 0x00005 */ 
  L_DMA_NW0_t                              NW0;                           /* addr: 0x00006 */ 
  L_DMA_VACS0_t                            VACS0;                         /* addr: 0x00007 */ 
  L_DMA_IT_INC0_t                          IT_INC0;                       /* addr: 0x00008 */ 
  L_DMA_PER0_t                             PER0;                          /* addr: 0x00009 */ 
  L_DMA_SA0_t                              SA0;                           /* addr: 0x0000a */ 
  L_DMA_DA0_t                              DA0;                           /* addr: 0x0000b */ 
  L_DMA_NB0_t                              NB0;                           /* addr: 0x0000c */ 
  L_DMA_CRCSD0_t                           CRCSD0;                        /* addr: 0x0000d */ 
  L_DMA_CRCRES0_t                          CRCRES0;                       /* addr: 0x0000e */ 
  L_DMA_IE0_t                              IE0;                           /* addr: 0x0000f */ 
  L_DMA_INT0_t                             INT0;                          /* addr: 0x00010 */ 
  L_DMA_STAT0_t                            STAT0;                         /* addr: 0x00011 */ 
  L_DMA_SPARE0_t                           SPARE0;                        /* addr: 0x00012 */ 
  L_DMA_CTL1_t                             CTL1;                          /* addr: 0x00013 */ 
  L_DMA_VCTRBASE1_t                        VCTRBASE1;                     /* addr: 0x00014 */ 
  L_DMA_WRPTR1_t                           WRPTR1;                        /* addr: 0x00015 */ 
  L_DMA_RDPTR1_t                           RDPTR1;                        /* addr: 0x00016 */ 
  L_DMA_REGA1_t                            REGA1;                         /* addr: 0x00017 */ 
  L_DMA_MA1_t                              MA1;                           /* addr: 0x00018 */ 
  L_DMA_NW1_t                              NW1;                           /* addr: 0x00019 */ 
  L_DMA_VACS1_t                            VACS1;                         /* addr: 0x0001a */ 
  L_DMA_IT_INC1_t                          IT_INC1;                       /* addr: 0x0001b */ 
  L_DMA_PER1_t                             PER1;                          /* addr: 0x0001c */ 
  L_DMA_SA1_t                              SA1;                           /* addr: 0x0001d */ 
  L_DMA_DA1_t                              DA1;                           /* addr: 0x0001e */ 
  L_DMA_NB1_t                              NB1;                           /* addr: 0x0001f */ 
  L_DMA_CRCSD1_t                           CRCSD1;                        /* addr: 0x00020 */ 
  L_DMA_CRCRES1_t                          CRCRES1;                       /* addr: 0x00021 */ 
  L_DMA_IE1_t                              IE1;                           /* addr: 0x00022 */ 
  L_DMA_INT1_t                             INT1;                          /* addr: 0x00023 */ 
  L_DMA_STAT1_t                            STAT1;                         /* addr: 0x00024 */ 
  L_DMA_SPARE1_t                           SPARE1;                        /* addr: 0x00025 */ 
} L_DMA_t;

  

/*
 * te
 */
  



#define TE_CTRL                                  0x0c000
#define TE_CTRL_dft                              0x60800000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd4               :  1 ;
    cs_uint32 ge_td_drop_en        :  1 ; /* bits 30:30 */
    cs_uint32 pon_td_drop_en       :  1 ; /* bits 29:29 */
    cs_uint32 voip_td_drop_en      :  1 ; /* bits 28:28 */
    cs_uint32 ge_pause_frame_en    :  1 ; /* bits 27:27 */
    cs_uint32 pon_pause_frame_en   :  1 ; /* bits 26:26 */
    cs_uint32 voip_pause_frame_en  :  1 ; /* bits 25:25 */
    cs_uint32 mii_cpup_en          :  1 ; /* bits 24:24 */
    cs_uint32 cpu_cos7_td_drop_dis :  1 ; /* bits 23:23 */
    cs_uint32 rsrvd3               :  2 ;
    cs_uint32 pir_wrate            :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 shaper_wrate         :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 subtok_en            :  1 ; /* bits 3:3 */
    cs_uint32 addtok_en            :  1 ; /* bits 2:2 */
    cs_uint32 td_all_buffer_sel    :  1 ; /* bits 1:1 */
    cs_uint32 qos_gen_en           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 qos_gen_en           :  1 ; /* bits 0:0 */
    cs_uint32 td_all_buffer_sel    :  1 ; /* bits 1:1 */
    cs_uint32 addtok_en            :  1 ; /* bits 2:2 */
    cs_uint32 subtok_en            :  1 ; /* bits 3:3 */
    cs_uint32 rsrvd1               :  4 ;
    cs_uint32 shaper_wrate         :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 pir_wrate            :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd3               :  2 ;
    cs_uint32 cpu_cos7_td_drop_dis :  1 ; /* bits 23:23 */
    cs_uint32 mii_cpup_en          :  1 ; /* bits 24:24 */
    cs_uint32 voip_pause_frame_en  :  1 ; /* bits 25:25 */
    cs_uint32 pon_pause_frame_en   :  1 ; /* bits 26:26 */
    cs_uint32 ge_pause_frame_en    :  1 ; /* bits 27:27 */
    cs_uint32 voip_td_drop_en      :  1 ; /* bits 28:28 */
    cs_uint32 pon_td_drop_en       :  1 ; /* bits 29:29 */
    cs_uint32 ge_td_drop_en        :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_CTRL_t;





#define TE_GE_SPEC_CFG                           0x0c001
#define TE_GE_SPEC_CFG_dft                       0x00040201




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 11 ;
    cs_uint32 spec_mask3           :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 spec_mask2           :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 spec_mask1           :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 spec_mask1           :  5 ; /* bits 4:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 spec_mask2           :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 spec_mask3           :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd3               : 11 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_GE_SPEC_CFG_t;





#define TE_PON_SPEC_CFG                          0x0c002
#define TE_PON_SPEC_CFG_dft                      0x00040201




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 11 ;
    cs_uint32 spec_mask3           :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 spec_mask2           :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 spec_mask1           :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 spec_mask1           :  5 ; /* bits 4:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 spec_mask2           :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 spec_mask3           :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd3               : 11 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_PON_SPEC_CFG_t;





#define TE_VOIP_SPEC_CFG                         0x0c003
#define TE_VOIP_SPEC_CFG_dft                     0x00040201




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd3               : 11 ;
    cs_uint32 spec_mask3           :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 spec_mask2           :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 spec_mask1           :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 spec_mask1           :  5 ; /* bits 4:0 */
    cs_uint32 rsrvd1               :  3 ;
    cs_uint32 spec_mask2           :  5 ; /* bits 12:8 */
    cs_uint32 rsrvd2               :  3 ;
    cs_uint32 spec_mask3           :  5 ; /* bits 20:16 */
    cs_uint32 rsrvd3               : 11 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_VOIP_SPEC_CFG_t;





#define TE_CPU_TD_CFG                            0x0c004
#define TE_CPU_TD_CFG_dft                        0x00000040




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 cpu_thr              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 cpu_thr              :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 24 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_CPU_TD_CFG_t;





#define TE_GE_TD_CFG1                            0x0c005
#define TE_GE_TD_CFG1_dft                        0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 drop_thrshld_7       :  8 ; /* bits 31:24 */
    cs_uint32 drop_thrshld_6       :  8 ; /* bits 23:16 */
    cs_uint32 drop_thrshld_5       :  8 ; /* bits 15:8 */
    cs_uint32 drop_thrshld_4       :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 drop_thrshld_4       :  8 ; /* bits 7:0 */
    cs_uint32 drop_thrshld_5       :  8 ; /* bits 15:8 */
    cs_uint32 drop_thrshld_6       :  8 ; /* bits 23:16 */
    cs_uint32 drop_thrshld_7       :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_GE_TD_CFG1_t;





#define TE_GE_TD_CFG0                            0x0c006
#define TE_GE_TD_CFG0_dft                        0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 drop_thrshld_3       :  8 ; /* bits 31:24 */
    cs_uint32 drop_thrshld_2       :  8 ; /* bits 23:16 */
    cs_uint32 drop_thrshld_1       :  8 ; /* bits 15:8 */
    cs_uint32 drop_thrshld_0       :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 drop_thrshld_0       :  8 ; /* bits 7:0 */
    cs_uint32 drop_thrshld_1       :  8 ; /* bits 15:8 */
    cs_uint32 drop_thrshld_2       :  8 ; /* bits 23:16 */
    cs_uint32 drop_thrshld_3       :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_GE_TD_CFG0_t;





#define TE_PON_TD_CFG1                           0x0c007
#define TE_PON_TD_CFG1_dft                       0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 drop_thrshld_7       :  8 ; /* bits 31:24 */
    cs_uint32 drop_thrshld_6       :  8 ; /* bits 23:16 */
    cs_uint32 drop_thrshld_5       :  8 ; /* bits 15:8 */
    cs_uint32 drop_thrshld_4       :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 drop_thrshld_4       :  8 ; /* bits 7:0 */
    cs_uint32 drop_thrshld_5       :  8 ; /* bits 15:8 */
    cs_uint32 drop_thrshld_6       :  8 ; /* bits 23:16 */
    cs_uint32 drop_thrshld_7       :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_PON_TD_CFG1_t;





#define TE_PON_TD_CFG0                           0x0c008
#define TE_PON_TD_CFG0_dft                       0xffffffff




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 drop_thrshld_3       :  8 ; /* bits 31:24 */
    cs_uint32 drop_thrshld_2       :  8 ; /* bits 23:16 */
    cs_uint32 drop_thrshld_1       :  8 ; /* bits 15:8 */
    cs_uint32 drop_thrshld_0       :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 drop_thrshld_0       :  8 ; /* bits 7:0 */
    cs_uint32 drop_thrshld_1       :  8 ; /* bits 15:8 */
    cs_uint32 drop_thrshld_2       :  8 ; /* bits 23:16 */
    cs_uint32 drop_thrshld_3       :  8 ; /* bits 31:24 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_PON_TD_CFG0_t;





#define TE_GE_SHP_CTRL                           0x0c009
#define TE_GE_SHP_CTRL_dft                       0x80000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 enb                  :  2 ; /* bits 31:30 */
    cs_uint32 cbs                  : 10 ; /* bits 29:20 */
    cs_uint32 rate_m               : 10 ; /* bits 19:10 */
    cs_uint32 rate_k               : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rate_k               : 10 ; /* bits 9:0 */
    cs_uint32 rate_m               : 10 ; /* bits 19:10 */
    cs_uint32 cbs                  : 10 ; /* bits 29:20 */
    cs_uint32 enb                  :  2 ; /* bits 31:30 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_GE_SHP_CTRL_t;





#define TE_GE_SHP_TIM_STS                        0x0c00a
#define TE_GE_SHP_TIM_STS_dft                    0x00100000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 11 ;
    cs_uint32 tbc                  : 21 ; /* bits 20:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tbc                  : 21 ; /* bits 20:0 */
    cs_uint32 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_GE_SHP_TIM_STS_t;





#define TE_VOIP_SHP_CTRL                         0x0c00b
#define TE_VOIP_SHP_CTRL_dft                     0x80000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 enb                  :  2 ; /* bits 31:30 */
    cs_uint32 cbs                  : 10 ; /* bits 29:20 */
    cs_uint32 rate_m               : 10 ; /* bits 19:10 */
    cs_uint32 rate_k               : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rate_k               : 10 ; /* bits 9:0 */
    cs_uint32 rate_m               : 10 ; /* bits 19:10 */
    cs_uint32 cbs                  : 10 ; /* bits 29:20 */
    cs_uint32 enb                  :  2 ; /* bits 31:30 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_VOIP_SHP_CTRL_t;





#define TE_VOIP_SHP_TIM_STS                      0x0c00c
#define TE_VOIP_SHP_TIM_STS_dft                  0x00100000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 11 ;
    cs_uint32 tbc                  : 21 ; /* bits 20:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tbc                  : 21 ; /* bits 20:0 */
    cs_uint32 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_VOIP_SHP_TIM_STS_t;





#define TE_MIR_SHP_CTRL                          0x0c00d
#define TE_MIR_SHP_CTRL_dft                      0x80000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 enb                  :  2 ; /* bits 31:30 */
    cs_uint32 cbs                  : 10 ; /* bits 29:20 */
    cs_uint32 rate_m               : 10 ; /* bits 19:10 */
    cs_uint32 rate_k               : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rate_k               : 10 ; /* bits 9:0 */
    cs_uint32 rate_m               : 10 ; /* bits 19:10 */
    cs_uint32 cbs                  : 10 ; /* bits 29:20 */
    cs_uint32 enb                  :  2 ; /* bits 31:30 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_MIR_SHP_CTRL_t;





#define TE_MIR_SHP_TIM_STS                       0x0c00e
#define TE_MIR_SHP_TIM_STS_dft                   0x00100000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 11 ;
    cs_uint32 tbc                  : 21 ; /* bits 20:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tbc                  : 21 ; /* bits 20:0 */
    cs_uint32 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_MIR_SHP_TIM_STS_t;





#define TE_ACC_FLOW_A_MSK                        0x0c00f
#define TE_ACC_FLOW_A_MSK_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mask                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mask                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_ACC_FLOW_A_MSK_t;





#define TE_ACC_FLOW_B_MSK                        0x0c010
#define TE_ACC_FLOW_B_MSK_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mask                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mask                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_ACC_FLOW_B_MSK_t;





#define TE_ACC_FLOW_C_MSK                        0x0c011
#define TE_ACC_FLOW_C_MSK_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mask                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mask                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_ACC_FLOW_C_MSK_t;





#define TE_ACC_FLOW_D_MSK                        0x0c012
#define TE_ACC_FLOW_D_MSK_dft                    0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 mask                 : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 mask                 : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_ACC_FLOW_D_MSK_t;





#define TE_FLOW_CNT_ACCESS                       0x0c013
#define TE_FLOW_CNT_ACCESS_dft                   0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 sel                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sel                  :  8 ; /* bits 7:0 */
    cs_uint32 rsrvd1               : 22 ;
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_FLOW_CNT_ACCESS_t;





#define TE_FLOW_CNT_COUNTER1                     0x0c014
#define TE_FLOW_CNT_COUNTER1_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 23 ;
    cs_uint32 cntmode              :  1 ; /* bits 8:8 */
    cs_uint32 counter              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 counter              :  8 ; /* bits 7:0 */
    cs_uint32 cntmode              :  1 ; /* bits 8:8 */
    cs_uint32 rsrvd1               : 23 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_FLOW_CNT_COUNTER1_t;





#define TE_FLOW_CNT_COUNTER0                     0x0c015
#define TE_FLOW_CNT_COUNTER0_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 counter              : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 counter              : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_FLOW_CNT_COUNTER0_t;





#define TE_PIR_BUCKET_ACCESS                     0x0c016
#define TE_PIR_BUCKET_ACCESS_dft                 0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 access               :  1 ; /* bits 31:31 */
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 sel                  :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 sel                  :  6 ; /* bits 5:0 */
    cs_uint32 rsrvd1               : 24 ;
    cs_uint32 rbw                  :  1 ; /* bits 30:30 */
    cs_uint32 access               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_PIR_BUCKET_ACCESS_t;





#define TE_PIR_BUCKET_DATA1                      0x0c017
#define TE_PIR_BUCKET_DATA1_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1               : 10 ;
    cs_uint32 rlmode               :  1 ; /* bits 21:21 */
    cs_uint32 enb                  :  2 ; /* bits 20:19 */
    cs_uint32 cbs                  : 10 ; /* bits 18:9 */
    cs_uint32 rate_m               :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 rate_m               :  9 ; /* bits 8:0 */
    cs_uint32 cbs                  : 10 ; /* bits 18:9 */
    cs_uint32 enb                  :  2 ; /* bits 20:19 */
    cs_uint32 rlmode               :  1 ; /* bits 21:21 */
    cs_uint32 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_PIR_BUCKET_DATA1_t;





#define TE_PIR_BUCKET_DATA0                      0x0c018
#define TE_PIR_BUCKET_DATA0_dft                  0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 rate_m               :  1 ; /* bits 31:31 */
    cs_uint32 rate_k               : 10 ; /* bits 30:21 */
    cs_uint32 tbc                  : 21 ; /* bits 20:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 tbc                  : 21 ; /* bits 20:0 */
    cs_uint32 rate_k               : 10 ; /* bits 30:21 */
    cs_uint32 rate_m               :  1 ; /* bits 31:31 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_PIR_BUCKET_DATA0_t;





#define TE_SPARE                                 0x0c019
#define TE_SPARE_dft                             0x00000000




typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint32 spare                : 32 ; /* bits 31:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint32 spare                : 32 ; /* bits 31:0 */
#   endif
  } bf ;
  cs_uint32     wrd ;
} TE_SPARE_t;



    
  
  
  
/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* te */

typedef struct {
  TE_CTRL_t                                CTRL;                          /* addr: 0x00000 */ 
  TE_GE_SPEC_CFG_t                         GE_SPEC_CFG;                   /* addr: 0x00001 */ 
  TE_PON_SPEC_CFG_t                        PON_SPEC_CFG;                  /* addr: 0x00002 */ 
  TE_VOIP_SPEC_CFG_t                       VOIP_SPEC_CFG;                 /* addr: 0x00003 */ 
  TE_CPU_TD_CFG_t                          CPU_TD_CFG;                    /* addr: 0x00004 */ 
  TE_GE_TD_CFG1_t                          GE_TD_CFG1;                    /* addr: 0x00005 */ 
  TE_GE_TD_CFG0_t                          GE_TD_CFG0;                    /* addr: 0x00006 */ 
  TE_PON_TD_CFG1_t                         PON_TD_CFG1;                   /* addr: 0x00007 */ 
  TE_PON_TD_CFG0_t                         PON_TD_CFG0;                   /* addr: 0x00008 */ 
  TE_GE_SHP_CTRL_t                         GE_SHP_CTRL;                   /* addr: 0x00009 */ 
  TE_GE_SHP_TIM_STS_t                      GE_SHP_TIM_STS;                /* addr: 0x0000a */ 
  TE_VOIP_SHP_CTRL_t                       VOIP_SHP_CTRL;                 /* addr: 0x0000b */ 
  TE_VOIP_SHP_TIM_STS_t                    VOIP_SHP_TIM_STS;              /* addr: 0x0000c */ 
  TE_MIR_SHP_CTRL_t                        MIR_SHP_CTRL;                  /* addr: 0x0000d */ 
  TE_MIR_SHP_TIM_STS_t                     MIR_SHP_TIM_STS;               /* addr: 0x0000e */ 
  TE_ACC_FLOW_A_MSK_t                      ACC_FLOW_A_MSK;                /* addr: 0x0000f */ 
  TE_ACC_FLOW_B_MSK_t                      ACC_FLOW_B_MSK;                /* addr: 0x00010 */ 
  TE_ACC_FLOW_C_MSK_t                      ACC_FLOW_C_MSK;                /* addr: 0x00011 */ 
  TE_ACC_FLOW_D_MSK_t                      ACC_FLOW_D_MSK;                /* addr: 0x00012 */ 
  TE_FLOW_CNT_ACCESS_t                     FLOW_CNT_ACCESS;               /* addr: 0x00013 */ 
  TE_FLOW_CNT_COUNTER1_t                   FLOW_CNT_COUNTER1;             /* addr: 0x00014 */ 
  TE_FLOW_CNT_COUNTER0_t                   FLOW_CNT_COUNTER0;             /* addr: 0x00015 */ 
  TE_PIR_BUCKET_ACCESS_t                   PIR_BUCKET_ACCESS;             /* addr: 0x00016 */ 
  TE_PIR_BUCKET_DATA1_t                    PIR_BUCKET_DATA1;              /* addr: 0x00017 */ 
  TE_PIR_BUCKET_DATA0_t                    PIR_BUCKET_DATA0;              /* addr: 0x00018 */ 
  TE_SPARE_t                               SPARE;                         /* addr: 0x00019 */ 
} TE_t;

  


/******************************************************************
 *
 * Block declaration
 *
 ******************************************************************/

/* LynxD */

typedef struct {
  PER_t                                    per;                           /* addr: 0x00000 */ 
  cs_uint32                                rsrvd1[3934];                 
  AHB_t                                    ahb;                           /* addr: 0x01000 */ 
  cs_uint32                                rsrvd2[4056];                 
  CPD_t                                    cpd;                           /* addr: 0x02000 */ 
  cs_uint32                                rsrvd3[4085];                 
  GLB_t                                    glb;                           /* addr: 0x03000 */ 
  cs_uint32                                rsrvd4[4069];                 
  LED_t                                    led;                           /* addr: 0x04000 */ 
  cs_uint32                                rsrvd5[4086];                 
  MA_t                                     ma;                            /* addr: 0x05000 */ 
  cs_uint32                                rsrvd6[2038];                 
  FE_t                                     fe;                            /* addr: 0x06000 */ 
  cs_uint32                                rsrvd7[3737];                 
  GE_t                                     ge;                            /* addr: 0x07000 */ 
  cs_uint32                                rsrvd8[4072];                 
  EPON_t                                   epon;                          /* addr: 0x08000 */ 
  cs_uint32                                rsrvd9[3874];                 
  BM_t                                     bm;                            /* addr: 0x09000 */ 
  cs_uint32                                rsrvd10[3881];                
  PCM_t                                    pcm;                           /* addr: 0x0a000 */ 
  cs_uint32                                rsrvd11[3924];                
  L_DMA_t                                  l_dma;                         /* addr: 0x0b000 */ 
  cs_uint32                                rsrvd12[4058];                
  TE_t                                     te;                            /* addr: 0x0c000 */ 
} LYNXD_t;


#endif
