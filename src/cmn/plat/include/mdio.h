/*
 * $Id: mdio.h,v 1.1.4.2.30.1.6.2 2011/08/05 00:57:11 ljin Exp $:
 */
/* Copyright: (c) 2007 Cortina Systems Inc. All Rights Reserved. */

/* MDIO REG definitions */

#define ARM_MDIO_CFG_REG         IROS_PER_MDIO_CFG
#define ARM_MDIO_ADDR_REG        IROS_PER_MDIO_ADDR
#define ARM_MDIO_WRDATA_REG      IROS_PER_MDIO_WRDATA
#define ARM_MDIO_RDDATA_REG      IROS_PER_MDIO_RDDATA
#define ARM_MDIO_CTRL_REG        IROS_PER_MDIO_CTRL

#define ARM_MDIO_AUTO_CFG_REG    IROS_PER_MDIO_AUTO_CFG
#define ARM_MDIO_AUTO_INTV_REG   IROS_PER_MDIO_AUTO_INTV
#define ARM_MDIO_AUTO_RM_REG     IROS_PER_MDIO_AUTO_RM
#define ARM_MDIO_AUTO_AADDR_REG  IROS_PER_MDIO_AUTO_AADDR
#define ARM_MDIO_IE_REG          IROS_PER_MDIO_IE
#define ARM_MDIO_INT_REG         IROS_PER_MDIO_INT
#define ARM_MDIO_STAT_REG        IROS_PER_MDIO_STAT

/*
*   MDIO clock frequency = (1/2)*PER_CLK_t*(mdio_pre_scale+1).
*/
#define MDIO_MODE_MANUAL           0x1
#define MDIO_ADDR_RD               (1<<15)
#define MDIO_ADDR_WR               (0<<15)
//mdio ctrl
#define MDIO_CTRL_START            (1<<7)
#define MDIO_CTRL_DONE              0x1

#define MDIO_TIMEOUT               2500

/* interface definition */
void mdio_init(cs_uint32 mdio_Mhz);
extern int mdio_read(cs_uint8 device,cs_uint8 reg, cs_uint16 *data);
extern int mdio_write(cs_uint8 device,cs_uint8 reg,cs_uint16 data);
extern cs_status mdio_clock_set(cs_uint32 clk_Mhz);
extern cs_status mdio_clock_get(cs_uint32 *clk_Mhz);



