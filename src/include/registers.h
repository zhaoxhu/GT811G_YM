/*
 * $Id: registers.h,v 1.1.4.2 2010/03/25 09:01:07 ylv Exp $
 */
#ifndef _REGISTERS_H_
#define _REGISTERS_H_
#include "iros_config.h"


#ifdef _FPGA

#else
#include "lynxd_registers.h"
#endif

#define LYNXD_REG_BASE   0x2c300000

#ifdef _SI_NAV_HELPER_
extern LYNXD_t *g_lynxd_reg_ptr;
#else
#define g_lynxd_reg_ptr  ((LYNXD_t *)LYNXD_REG_BASE)
#endif

#endif /* #ifndef _REGISTERS_H_ */

