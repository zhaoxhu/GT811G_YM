#ifndef CYGONCE_HAL_REG_LYNXD_H
#define CYGONCE_HAL_REG_LYNXD_H

//      hal_reg_lynxd.h
//
//      HAL Description of IMST board - LynxD
//
#include <pkgconf/system.h>

#define IMST_IMAGE_DOWNLOAD         IROS_GLB_GLOBAL_PIN_STS
#define IMAGE_DOWNLOAD_BITS         0x30              /* bit 4, 5    */
#define IMAGE_FROM_ETH              0x30              /*      : 11   */
#define IMAGE_FROM_SERIAL_FLASH     0x20              /*      : 10   */
#define IMAGE_FROM_FLASH            0x10              /*      : 01   */
#define IMAGE_FROM_UART             0x00              /*      : 00   */


#define IMST_EXT_RAM_START          0x0
#define IMST_EXT_RAM_MAX_SIZE       0x01000000       /* max 16 MB */



// use MISC LED on ARM core modules for IMST evaluation boards
#if defined(CYG_HAL_ARM_IMST_FPGA) || defined(CYG_HAL_ARM_IMST_CM)
#if defined(IMST_USE_CM_MISC_LED)
#define CM_HDR_BASE                 0x10000000
#define CM_HDR_CTRL_OFFSET          0x0C
#define CM_HDR_DBG_MISC_LED         0x02               /* bit 1 */
#define CM_HDR_CTRL_REG             (CM_HDR_BASE + CM_HDR_CTRL_OFFSET)
#endif /* IMST_USE_CM_MISC_LED */
#endif /* CYG_HAL_ARM_IMST_FPGA || CYG_HAL_ARM_IMST_CM */

// uart0 and uart1

#if defined(IMST_UART)
    #define IMST_UART0_BASE         IROS_PER_UART0_CFG        /*  UART 0, we have only one UART on chip */
    #define IMST_UART1_BASE         IROS_PER_UART1_CFG        /*  UART 1, we have two on evaluation board */
    #define IMST_UART2_BASE         (IROS_PER_BASE + 0x00154)
    #define IMST_UART3_BASE         (IROS_PER_BASE + 0x00178)
    #define IMST_UART4_BASE         (IROS_PER_BASE + 0x0019c)

    #define ARM_IMST_BAUD_DIVISOR   (CYG_HAL_ARM_IMST_DEV_FREQ/CYGNUM_HAL_VIRTUAL_VECTOR_CONSOLE_CHANNEL_BAUD)
    #define IMST_UART_CFG_VAL       (IMST_UARTCR_EN | ((ARM_IMST_BAUD_DIVISOR - 1) << 14) | IMST_UARTCR_RXEN | IMST_UARTCR_TXEN | IMST_UARTCR_WLEN_8)
    #define IMST_UART_SAMPLE_VAL    ((ARM_IMST_BAUD_DIVISOR - 1) / 2)
    #define IMST_UART_TUNE_VAL      (((ARM_IMST_BAUD_DIVISOR/48) << 16) | (ARM_IMST_BAUD_DIVISOR/48))
#endif /* IMST_UART */


#define CYG_DEVICE_TIMER_BASE       IROS_PER_TMR_LD1
#define CYG_DEVICE_TIMER_CONTROL    IROS_PER_TMR_CTRL1
#define CYG_DEVICE_TIMER_LOAD       IROS_PER_TMR_LD1
// Current value, read
#define CYG_DEVICE_TIMER_CURRENT    IROS_PER_TMR_CNT1
#define CYG_DEVICE_TIMER_CLEAR      IROS_PER_TMR_INT1
#define CYG_DEVICE_TIMER_IEN        IROS_PER_TMR_IE1


// Slow timer
#define SLOW_TIMER_LOAD             ((volatile cyg_uint32 *) IROS_PER_TMR_LD2)
#define SLOW_TIMER_CURRENT          ((volatile cyg_uint32 *) IROS_PER_TMR_CNT2)
#define SLOW_TIMER_CONTROL          ((volatile cyg_uint32 *) IROS_PER_TMR_CTRL2)
#define SLOW_TIMER_CLEAR            ((volatile cyg_uint32 *) IROS_PER_TMR_INT2)


// Clock/timer control register
#define CTL_ENABLE      0x80            // Bit   7: 1 - counter enabled
#define CTL_DISABLE     0x00            //          0 - counter disabled
#define CTL_FREERUN     0x00            // Bit   6: 0 - free running counter
#define CTL_PERIODIC    0x40            //          1 - periodic timer mode
#define CTL_SCALE_1     0x00            // Bits3,2: 00 - Scale clock by 1
#define CTL_SCALE_64    0x04            //          01 - Scale by 64
#define CTL_SCALE_256   0x08            //          10 - Scale by 256, not support
#define CTL_SCALE_1024  0x0c            //          11 - Scale by 1024
#define CTL_CLK_SEL     1               // used by counter divider, current scale by 1
#define CTL_IENABLE     0x01            // Bit   0: Interrupt enable

// Interrupt controller registers
#define CYG_DEVICE_ICTL_BASE              IROS_PER_IRQ
#define CYG_DEVICE_IRQ_Status             ((volatile cyg_uint32 *) CYG_DEVICE_ICTL_BASE)
#define CYG_DEVICE_IRQ_Enable             ((volatile cyg_uint32 *) IROS_PER_IRQ_ENABLE)
#define CYG_DEVICE_IRQ_EnableSet          ((volatile cyg_uint32 *) IROS_PER_IRQ_ENABLE)
#define CYG_DEVICE_FIQ_Status             ((volatile cyg_uint32 *) IROS_PER_FIQ)
#define CYG_DEVICE_FIQ_Enable             ((volatile cyg_uint32 *) IROS_PER_FIQ_ENABLE)



#if defined(IMST_UART)


//UART configure   38400 8N1 (UART_EN  | baud(TBD)  | CHAR_CNT_8BIT )
#define UART_EN           (1<<31)
#define RX_SM_ENABLE      (1<<6)
#define TX_SM_ENABLE      (1<<5)
#define UART_PARITY_EN    (1<<4)
#define EVEN_PARITY_EVEN  (1<<3)
#define UART_STOP_2BI     (1<<2)
#define CHAR_CNT_6BIT     (0x1)
#define CHAR_CNT_7BIT     (0x2)
#define CHAR_CNT_8BIT     (0x3)

#define RX_DATA_VLD       (1<<8)
#define TX_FIFO_FULL      (1<<2)   //PER_UART0_INFO 0x188

    //base 0x2c300174
    #define IMST_UARTCR                     0x00    /* 32 bits Control Register */
    #define IMST_UART_RX_SAMPLE             0x04    /* offset 0x04 PER_UART0_RX_SAMPLE */
    #define IMST_UART_TUN                   0x08    /* offset 0x08 PER_UART0_TUN */
    #define IMST_UARTFR                     0x14    /* 0x00188 PER_UART0_INFO */
    #define IMST_UARTDW                     0x0C    /* 32 bits 7:0 Data written from the interface 0x00180 PER_UART0_TXDAT*/
    #define IMST_UARTDR                     0x10    /* 32 bits 7:0 Data read from the interface    0x00184 PER_UART0_RXDAT*/
    #define IMST_UARTIE                     0x18    /* Interrupt enable control register*/
    #define IMST_UARTINT                     0x1c    /* Interrupt status register*/

    #define IMST_UARTFR_DA                  0x02      /* Bit 1: UART read FIFO has data to read (Read) */
    #define IMST_UARTFR_TXFF                0x04      /* Bit 2: Tansmit FIFO Full (Read) */


    #define IMST_UARTCR_WLEN_8              0x03    /* Bit 0, 1: Word length (Read/Write) 11 = 8 bits */
    #define IMST_UARTCR_WLEN_7              0x02    /* Bit 0, 1:                          10 = 7 bits */
    #define IMST_UARTCR_WLEN_6              0x01    /* Bit 0, 1:                          01 = 6 bits */
    #define IMST_UARTCR_WLEN_5              0x00    /* Bit 0, 1:                          00 = 5 bits */

    #define IMST_UARTCR_TXEN                0x20    /* Bit5 Transmit Enable (Read/Write) */
    #define IMST_UARTCR_RXEN                0x40    /* Bit6 Receive Enable  (Read/Write) */
    #define IMST_UARTCR_EN                  0x80000000 /* Bit 31: UART Enable (Read/Write) */

    #define RX_FIFO_UNDERRUNEIEN            0x10     /* Bit 4 */
    #define RX_FIFO_OVERRUNEIEN             0x08     /* Bit 3 */
    #define RX_PARITY_ERREIEN               0x04     /* Bit 2 */
    #define RX_STOP_ERREIEN                 0x02     /* Bit 1 */
    #define TX_FIFO_OVERRUNEIEN             0x01     /* Bit 0 */

    #define IMST_UARTCR_IEN                 0x1E    /*Bit 1 - 4 rec interrupt */
#endif /* IMST_UART */



#define SFLASH_BASE                      0x40000000
#define SFLASH_SIZE                         0x1000000
#define IMST_PARALLEL_FLASH_BASE        0x30000000
#ifdef HAVE_SERIAL_FLASH
#define FLASH_BASE                      SFLASH_BASE
#define FLASH_MAX_SIZE                  SFLASH_SIZE
#else
#define FLASH_BASE                      IMST_PARALLEL_FLASH_BASE
#define FLASH_MAX_SIZE                  0x08000000
#endif
#define EXTERNAL_SDRAM_START            0x10000000
#define EXTERNAL_SRAM_START             0x20000000
#define INTERNAL_SRAM_START             0x2c000000

#define RAM_START_BEFORE_REMAP          EXTERNAL_SDRAM_START  /* sram */
#define IMST_REMAP_CRTL_REG             IROS_AHB_RPC    /* remap */


#define IROS_REG_BASE               0x2c300000
#define IROS_PER_BASE               IROS_REG_BASE
#define IROS_AHB_BASE               (IROS_REG_BASE + 0x04000)
#define IROS_CPD_BASE               (IROS_REG_BASE + 0x08000)
#define IROS_GLB_BASE               (IROS_REG_BASE + 0x0c000)
#define IROS_MA_BASE                (IROS_REG_BASE + 0x14000)
#define IROS_FE_BASE                (IROS_REG_BASE + 0x18000)
#define IROS_GE_BASE                (IROS_REG_BASE + 0x1c000)
#define IROS_BM_BASE                (IROS_REG_BASE + 0x24000)

//PER
#define IROS_PER_IRQ                (IROS_PER_BASE + 0x00000)
#define IROS_PER_IRQ_ENABLE         (IROS_PER_BASE + 0x00004)
#define IROS_PER_IRQ_SOFT           (IROS_PER_BASE + 0x00008)
#define IROS_PER_FIQ                (IROS_PER_BASE + 0x0000c)
#define IROS_PER_FIQ_ENABLE         (IROS_PER_BASE + 0x00010)
#define IROS_PER_WDT_CTRL           (IROS_PER_BASE + 0x00014)
#define IROS_PER_WDT_PS             (IROS_PER_BASE + 0x00018)
#define IROS_PER_WDT_DIV            (IROS_PER_BASE + 0x0001c)
#define IROS_PER_WDT_LD             (IROS_PER_BASE + 0x00020)
#define IROS_PER_WDT_LOADE          (IROS_PER_BASE + 0x00024)
#define IROS_PER_WDT_CNT            (IROS_PER_BASE + 0x00028)
#define IROS_PER_WDT_IE             (IROS_PER_BASE + 0x0002c)
#define IROS_PER_WDT_INT            (IROS_PER_BASE + 0x00030)
#define IROS_PER_WDT_STAT           (IROS_PER_BASE + 0x00034)
#define IROS_PER_WDT_TST            (IROS_PER_BASE + 0x00038)
#define IROS_PER_TMR_LD1            (IROS_PER_BASE + 0x0003c)
#define IROS_PER_TMR_CTRL1          (IROS_PER_BASE + 0x00040)
#define IROS_PER_TMR_CNT1           (IROS_PER_BASE + 0x00044)
#define IROS_PER_TMR_IE1            (IROS_PER_BASE + 0x00048)
#define IROS_PER_TMR_INT1           (IROS_PER_BASE + 0x0004c)
#define IROS_PER_TMR_STAT1          (IROS_PER_BASE + 0x00050)
#define IROS_PER_TMR_LD2            (IROS_PER_BASE + 0x00054)
#define IROS_PER_TMR_CTRL2          (IROS_PER_BASE + 0x00058)
#define IROS_PER_TMR_CNT2           (IROS_PER_BASE + 0x0005c)
#define IROS_PER_TMR_IE2            (IROS_PER_BASE + 0x00060)
#define IROS_PER_TMR_INT2           (IROS_PER_BASE + 0x00064)
#define IROS_PER_TMR_STAT2          (IROS_PER_BASE + 0x00068)
#define IROS_PER_TMR_LD3            (IROS_PER_BASE + 0x0006c)
#define IROS_PER_TMR_CTRL3          (IROS_PER_BASE + 0x00070)
#define IROS_PER_TMR_CNT3           (IROS_PER_BASE + 0x00074)
#define IROS_PER_TMR_IE3            (IROS_PER_BASE + 0x00078)
#define IROS_PER_TMR_INT3           (IROS_PER_BASE + 0x0007c)
#define IROS_PER_TMR_STAT3          (IROS_PER_BASE + 0x00080)
#define IROS_PER_TMR_LD4            (IROS_PER_BASE + 0x00084)
#define IROS_PER_TMR_CTRL4          (IROS_PER_BASE + 0x00088)
#define IROS_PER_TMR_CNT4           (IROS_PER_BASE + 0x0008c)
#define IROS_PER_TMR_IE4            (IROS_PER_BASE + 0x00090)
#define IROS_PER_TMR_INT4           (IROS_PER_BASE + 0x00094)
#define IROS_PER_TMR_STAT4          (IROS_PER_BASE + 0x00098)
#define IROS_PER_TMR_LD5            (IROS_PER_BASE + 0x0009c)
#define IROS_PER_TMR_CTRL5          (IROS_PER_BASE + 0x000a0)
#define IROS_PER_TMR_CNT5           (IROS_PER_BASE + 0x000a4)
#define IROS_PER_TMR_IE5            (IROS_PER_BASE + 0x000a8)
#define IROS_PER_TMR_INT5           (IROS_PER_BASE + 0x000ac)
#define IROS_PER_TMR_STAT5          (IROS_PER_BASE + 0x000b0)

#define IROS_PER_MDIO_CFG           (IROS_PER_BASE + 0x000bc)
#define IROS_PER_MDIO_ADDR          (IROS_PER_BASE + 0x000c0)
#define IROS_PER_MDIO_WRDATA        (IROS_PER_BASE + 0x000c4)
#define IROS_PER_MDIO_RDDATA        (IROS_PER_BASE + 0x000c8)
#define IROS_PER_MDIO_CTRL          (IROS_PER_BASE + 0x000cc)
#define IROS_PER_MDIO_AUTO_CFG      (IROS_PER_BASE + 0x000d0)
#define IROS_PER_MDIO_AUTO_INTV     (IROS_PER_BASE + 0x000d4)
#define IROS_PER_MDIO_AUTO_RM       (IROS_PER_BASE + 0x000d8)
#define IROS_PER_MDIO_AUTO_AADDR    (IROS_PER_BASE + 0x000dc)
#define IROS_PER_MDIO_IE            (IROS_PER_BASE + 0x000e0)
#define IROS_PER_MDIO_INT           (IROS_PER_BASE + 0x000e4)
#define IROS_PER_MDIO_STAT          (IROS_PER_BASE + 0x000e8)

#define IROS_PER_BIW0_CFG           (IROS_PER_BASE + 0x000ec)
#define IROS_PER_BIW0_CTRL          (IROS_PER_BASE + 0x000f0)
#define IROS_PER_BIW0_TXR           (IROS_PER_BASE + 0x000f4)
#define IROS_PER_BIW0_RXR           (IROS_PER_BASE + 0x000f8)
#define IROS_PER_BIW0_ACK           (IROS_PER_BASE + 0x000fc)
#define IROS_PER_BIW_IE             (IROS_PER_BASE + 0x00100)
#define IROS_PER_BIW_INT            (IROS_PER_BASE + 0x00104)
#define IROS_PER_BIW_STAT           (IROS_PER_BASE + 0x00108)

#define IROS_PER_UART0_CFG          (IROS_PER_BASE + 0x0010c)
#define IROS_PER_UART0_RX_SAMPLE    (IROS_PER_BASE + 0x00110)
#define IROS_PER_UART0_TUN          (IROS_PER_BASE + 0x00114)
#define IROS_PER_UART0_TXDAT        (IROS_PER_BASE + 0x00118)
#define IROS_PER_UART0_RXDAT        (IROS_PER_BASE + 0x0011c)
#define IROS_PER_UART0_INFO         (IROS_PER_BASE + 0x00120)
#define IROS_PER_UART0_IE           (IROS_PER_BASE + 0x00124)
#define IROS_PER_UART0_INT          (IROS_PER_BASE + 0x00128)
#define IROS_PER_UART0_STAT         (IROS_PER_BASE + 0x0012c)

#define IROS_PER_UART1_CFG          (IROS_PER_BASE + 0x00130)
#define IROS_PER_UART1_RX_SAMPLE    (IROS_PER_BASE + 0x00134)
#define IROS_PER_UART1_TUN          (IROS_PER_BASE + 0x00138)
#define IROS_PER_UART1_TXDAT        (IROS_PER_BASE + 0x0013c)
#define IROS_PER_UART1_RXDAT        (IROS_PER_BASE + 0x00140)
#define IROS_PER_UART1_INFO         (IROS_PER_BASE + 0x00144)
#define IROS_PER_UART1_IE           (IROS_PER_BASE + 0x00148)
#define IROS_PER_UART1_INT          (IROS_PER_BASE + 0x0014c)
#define IROS_PER_UART1_STAT         (IROS_PER_BASE + 0x00150)

#define IROS_PER_GPIO_CFG           (IROS_PER_BASE + 0x001c0)
#define IROS_PER_GPIO_OUT           (IROS_PER_BASE + 0x001c4)
#define IROS_PER_GPIO_IN            (IROS_PER_BASE + 0x001c8)
#define IROS_PER_GPIO_LVL           (IROS_PER_BASE + 0x001cc)
#define IROS_PER_GPIO_IE            (IROS_PER_BASE + 0x001d0)
#define IROS_PER_GPIO_INT           (IROS_PER_BASE + 0x001d4)

#define IROS_PER_BIW_MEM            (IROS_PER_BASE + 0x00200)
#define IROS_PER_BIW_MODE0          (IROS_PER_BASE + 0x00280)

#define IROS_PER_TOD_CTRL           (IROS_PER_BASE + 0x00284)


//AHB
#define IROS_AHB_RPC                (IROS_AHB_BASE + 0x00020)

#define IROS_AHB_SSP_CLK            (IROS_AHB_BASE + 0x00060)
#define IROS_AHB_SSP_CFG            (IROS_AHB_BASE + 0x00064)
#define IROS_AHB_SSP_CTRL           (IROS_AHB_BASE + 0x00068)
#define IROS_AHB_SSP_CA0            (IROS_AHB_BASE + 0x0006c)
#define IROS_AHB_SSP_CA1            (IROS_AHB_BASE + 0x00070)
#define IROS_AHB_SSP_CA2            (IROS_AHB_BASE + 0x00074)
#define IROS_AHB_SSP_WDAT           (IROS_AHB_BASE + 0x00078)
#define IROS_AHB_SSP_RDAT           (IROS_AHB_BASE + 0x0007c)
#define IROS_AHB_SSP_IE             (IROS_AHB_BASE + 0x00080)
#define IROS_AHB_SSP_INT            (IROS_AHB_BASE + 0x00084)
#define IROS_AHB_SSP_STAT           (IROS_AHB_BASE + 0x00088)
#define IROS_AHB_SSP_BST0           (IROS_AHB_BASE + 0x0008C)
#define IROS_AHB_SSP_BST1           (IROS_AHB_BASE + 0x00090)
#define IROS_AHB_SSP_BST2           (IROS_AHB_BASE + 0x00094)
#define IROS_AHB_SSP_BST3           (IROS_AHB_BASE + 0x00098)
#define IROS_AHB_SSP_BST_STAT       (IROS_AHB_BASE + 0x0009C)

//CPD
#define IROS_CPD_DRAM_CNT_TRG               (IROS_CPD_BASE + 0x00000)
#define IROS_CPD_DRAM_CONTROL0              (IROS_CPD_BASE + 0x00004)
#define IROS_CPD_DRAM_CONTROL1              (IROS_CPD_BASE + 0x00008)
#define IROS_CPD_DRAM_REFRESH_CONTROL       (IROS_CPD_BASE + 0x0000c)
#define IROS_CPD_DRAM_PERFORMANCE_CTL       (IROS_CPD_BASE + 0x00010)
#define IROS_CPD_ARB_ERR_CLR                (IROS_CPD_BASE + 0x00014)
#define IROS_CPD_ARBITER_ERR                (IROS_CPD_BASE + 0x00018)
#define IROS_CPD_ARB_CONTROL0               (IROS_CPD_BASE + 0x0001c)


//GLB
#define IROS_GLB_GLOBAL_JTAG_ID                                 (IROS_GLB_BASE + 0x00000)
#define IROS_GLB_GLOBAL_ONU_MAC_ADDR1                           (IROS_GLB_BASE + 0x00004)
#define IROS_GLB_GLOBAL_ONU_MAC_ADDR0                           (IROS_GLB_BASE + 0x00008)
#define IROS_GLB_GLOBAL_CTRL                                    (IROS_GLB_BASE + 0x0000c)
#define GLB_CTRL_LEGACY_MODE                                     0x1
#define IROS_GLB_GLOBAL_INIT_STS                                (IROS_GLB_BASE + 0x00010)
#define IROS_GLB_GLOBAL_RST_CTRL                                (IROS_GLB_BASE + 0x00014)
#define GLB_RESET_ALL                                            0xffffffff
#define ASIC_RESET_PATTERN                                       0x13579ACE
#define IROS_GLB_GLOBAL_SW_CHIP_RST                             (IROS_GLB_BASE + 0x00018)
#define IROS_GLB_GLOBAL_PIN_CTRL                                (IROS_GLB_BASE + 0x0001c)
#define IROS_GLB_GLOBAL_PIN_STS                                 (IROS_GLB_BASE + 0x00020)
#define IROS_GLB_GLOBAL_SYNC_CTRL                               (IROS_GLB_BASE + 0x00024)



#endif //CYGONCE_HAL_REG_LYNXD_H

