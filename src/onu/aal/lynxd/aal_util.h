/****************************************************************************
            Software License for Customer Use of Cortina Software
                          Grant Terms and Conditions
 
IMPORTANT NOTICE - READ CAREFULLY: This Software License for Customer Use
of Cortina Software ("LICENSE") is the agreement which governs use of
software of Cortina Systems, Inc. and its subsidiaries ("CORTINA"),
including computer software (source code and object code) and associated
printed materials ("SOFTWARE").  The SOFTWARE is protected by copyright laws
and international copyright treaties, as well as other intellectual property
laws and treaties.  The SOFTWARE is not sold, and instead is only licensed
for use, strictly in accordance with this document.  Any hardware sold by
CORTINA is protected by various patents, and is sold but this LICENSE does
not cover that sale, since it may not necessarily be sold as a package with
the SOFTWARE.  This LICENSE sets forth the terms and conditions of the
SOFTWARE LICENSE only.  By downloading, installing, copying, or otherwise
using the SOFTWARE, you agree to be bound by the terms of this LICENSE.
If you do not agree to the terms of this LICENSE, then do not download the
SOFTWARE.
 
DEFINITIONS:  "DEVICE" means the Cortina Systems LynxD product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems software.
 
GRANT OF LICENSE:  Subject to the restrictions below, CORTINA hereby grants
CUSTOMER a non-exclusive, non-assignable, non-transferable, royalty-free,
perpetual copyright license to (1) install and use the SOFTWARE for
reference only with the DEVICE; and (2) copy the SOFTWARE for your internal
use only for use with the DEVICE.
 
RESTRICTIONS:  The SOFTWARE must be used solely in conjunction with the
DEVICE and solely with Your own products that incorporate the DEVICE.  You
may not distribute the SOFTWARE to any third party.  You may not modify
the SOFTWARE or make derivatives of the SOFTWARE without assigning any and
all rights in such modifications and derivatives to CORTINA.  You shall not
through incorporation, modification or distribution of the SOFTWARE cause
it to become subject to any open source licenses.  You may not
reverse-assemble, reverse-compile, or otherwise reverse-engineer any
SOFTWARE provided in binary or machine readable form.  You may not
distribute the SOFTWARE to your customers without written permission
from CORTINA.
 
OWNERSHIP OF SOFTWARE AND COPYRIGHTS. All title and copyrights in and the
SOFTWARE and any accompanying printed materials, and copies of the SOFTWARE,
are owned by CORTINA. The SOFTWARE protected by the copyright laws of the
United States and other countries, and international treaty provisions.
You may not remove any copyright notices from the SOFTWARE.  Except as
otherwise expressly provided, CORTINA grants no express or implied right
under CORTINA patents, copyrights, trademarks, or other intellectual
property rights.
 
DISCLAIMER OF WARRANTIES. THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY
EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING ANY IMPLIED WARRANTIES
OF MERCHANTABILITY, NONINFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE,
TITLE, AND NON-INFRINGEMENT.  CORTINA does not warrant or assume
responsibility for the accuracy or completeness of any information, text,
graphics, links or other items contained within the SOFTWARE.  Without
limiting the foregoing, you are solely responsible for determining and
verifying that the SOFTWARE that you obtain and install is the appropriate
version for your purpose.
 
LIMITATION OF LIABILITY. IN NO EVENT SHALL CORTINA OR ITS SUPPLIERS BE
LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, LOST
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING CS_OUT
OF THE USE OF OR INABILITY TO USE OF OR INABILITY TO USE THE SOFTWARE, EVEN
IF CORTINA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
TERMINATION OF THIS LICENSE. This LICENSE will automatically terminate if
You fail to comply with any of the terms and conditions hereof. Upon
termination, You will immediately cease use of the SOFTWARE and destroy all
copies of the SOFTWARE or return all copies of the SOFTWARE in your control
to CORTINA.  IF you commence or participate in any legal proceeding against
CORTINA, then CORTINA may, in its sole discretion, suspend or terminate all
license grants and any other rights provided under this LICENSE during the
pendency of such legal proceedings.
APPLICABLE LAWS. Claims arising under this LICENSE shall be governed by the
laws of the State of California, excluding its principles of conflict of
laws.  The United Nations Convention on Contracts for the International Sale
of Goods is specifically disclaimed.  You shall not export the SOFTWARE
without first obtaining any required export license or other approval from
the applicable governmental entity, if required.  This is the entire
agreement and understanding between You and CORTINA relating to this subject
matter.
GOVERNMENT RESTRICTED RIGHTS. The SOFTWARE is provided with "RESTRICTED
RIGHTS." Use, duplication, or disclosure by the Government is subject to
restrictions as set forth in FAR52.227-14 and DFAR252.227-7013 et seq. or
its successor. Use of the SOFTWARE by the Government constitutes
acknowledgment of CORTINA's proprietary rights therein. Contractor or
Manufacturer is CORTINA.
 
Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/

#ifndef __AAL_UTIL_H__
#define __AAL_UTIL_H__

#include "cs_types.h"
#include "registers.h"
#include "plat_common.h"

#if defined(__IROS_DEBUG_VERSION__)
#define __AAL_DEBUG__
#endif

#ifdef __AAL_DEBUG__
#define DBG_PRINT(fmt,...)    do { cs_printf("%s_%d:"fmt, __FUNCTION__,__LINE__,##__VA_ARGS__);}while(0)
#define AAL_FUNC_ENTER        do { cs_printf("%s() was called\n", __FUNCTION__);}while(0)
#define AAL_FUNC_LEAVE(x)     do { cs_printf("%s() was leaved in line %d with return %d\n", \
                                                __FUNCTION__, __LINE__, x);}while(0)

#ifndef  AAL_ASSERT_RET
#define  AAL_ASSERT_RET(x)    do{ \
                                if (!(x))  { \
                                    cs_printf("%s(): wrong param\n", __FUNCTION__); \
                                    return (CS_ERROR ); \
                                } \
                              }while(0)
#endif

#ifndef  AAL_ASSERT_EXT
#define  AAL_ASSERT_EXT(x)    do{ \
                                if (!(x))  { \
                                    cs_printf("%s(): wrong param\n", __FUNCTION__); \
                                    exit (-1); \
                                } \
                              }while(0)
#endif

#else
#define DBG_PRINT(fmt,...)
#define AAL_FUNC_ENTER
#define AAL_FUNC_LEAVE(x)

#ifndef  AAL_ASSERT_RET
#define  AAL_ASSERT_RET(x)
#endif

#ifndef  AAL_ASSERT_EXT
#define  AAL_ASSERT_EXT(x)
#endif
#endif


 /* dst port ID */
 typedef enum {
     AAL_DPID_GE               = 0,  /* GE port;                  */
     AAL_DPID_PON              = 1,  /* PON port;                 */
     AAL_DPID_CE_L2            = 2,  /* Reserved port for CE indicating the DPID should be further */
                                     /* retrieved from L2 engine; for other blocks instead CE, this is same as BLACKHOLE */
     
     AAL_DPID_BLACKHOLE        = 3,  /* Blackhole port (dropped); */
     AAL_DPID_CPU              = 4,  /* CPU port;                 */
     AAL_DPID_MII0             = 5,  /* MII0 port (VoIP) port;    */
     AAL_DPID_MII1             = 6,  /* MII1 port port;           */
     AAL_DPID_FE               = 7   /* Reserved ports for Packet resolution indicating the DPID */
                                     /* should be further decided from forwarding engine; for other blocks instead CE, this is same as BLACKHOLE */
 }cs_aal_dpid_t;


 /* Forwarding Engine ID */
 typedef enum {
     AAL_FE_GE_ID  = 0,
     AAL_FE_PON_ID = 1,
     AAL_FE_MA_ID  = 2,
    
 }cs_aal_fe_instance_t;

 
/* port from cs_aal_port_id_t */
#define FE_PORT(port)      ((port) > 1 ? 2 : (port))

#if 0
#ifndef BITON
#define BITON(m, b)       (((m) & (1<<(b))) != 0)
#endif

#ifndef SETBIT
#define SETBIT(m, b)      ((m) |= (1<<(b)))
#endif

#ifndef CLRBIT
#define CLRBIT(m, b)      ((m) &= ~(1<<(b)))
#endif

#ifndef NEGBIT
#define NEGBIT(m, b)      ((m) ^= (1<<(b)))
#endif

#endif

#ifndef MAX
  #define MAX(a, b)      ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
  #define MIN(a, b)      ((a) < (b) ? (a) : (b))
#endif

#ifndef SWAP_INT_DATA
#define SWAP_INT_DATA(a,b)   do{(a) ^= (b); (b) ^= (a); (a) ^= (b);}while(0)
#endif


/*****************************************************************************/

extern cs_uint32  aal_reg_mutex;


/* reg: register name defined by reg.db */ 
/* field: field name defined by reg.db */  
/* mask: bit you want to set, for example, bit0 and bit1, mask==0x3 */

#define FILL_MSK    (~0)

#define PORT_OFFSET (4*FE_LE_STRIDE)


#define REG_FIELD_READ(reg, field, val)                  do{\
                                                         reg##_t reg_data;\
                                                         reg_data.wrd = *(volatile cs_uint32*)(LYNXD_REG_BASE+ (reg)*4);\
                                                         (val) = reg_data.bf.field;\
                                                         }while(0)

#define REG_FIELD_WRITE(reg, field, val)                 do{\
                                                         reg##_t reg_data;\
                                                         cs_mutex_lock(aal_reg_mutex);\
                                                         reg_data.wrd = *(volatile cs_uint32*)(LYNXD_REG_BASE+ (reg)*4);\
                                                         reg_data.bf.field = (val);\
                                                         *(volatile cs_uint32*)(LYNXD_REG_BASE+ (reg)*4) = reg_data.wrd;\
                                                         cs_mutex_unlock(aal_reg_mutex);\
                                                         }while(0)

#define REG_FIELD_WRITE_NO_MUTEX(reg, field, val)                 do{\
                                                         reg##_t reg_data;\
                                                         reg_data.wrd = *(volatile cs_uint32*)(LYNXD_REG_BASE+ (reg)*4);\
                                                         reg_data.bf.field = (val);\
                                                         *(volatile cs_uint32*)(LYNXD_REG_BASE+ (reg)*4) = reg_data.wrd;\
                                                         }while(0)

#define REG_READ(reg, val)                               do{\
                                                         (val) = *(volatile cs_uint32*)(LYNXD_REG_BASE+ (reg)*4);\
                                                         }while(0)

#define REG_WRITE(reg, val)                              do{\
                                                         *(volatile cs_uint32*)(LYNXD_REG_BASE+ (reg)*4) = (val);\
                                                         }while(0)

#define REG_MASK_WRITE(reg, mask, val)                   do{\
                                                         cs_uint32 reg_data;\
                                                         cs_mutex_lock(aal_reg_mutex);\
                                                         reg_data = *(volatile cs_uint32*)(LYNXD_REG_BASE+ (reg)*4);\
                                                         reg_data &= (~mask);\
                                                         reg_data |= (val&mask);\
                                                         *(volatile cs_uint32*)(LYNXD_REG_BASE+ (reg)*4) = reg_data;\
                                                         cs_mutex_unlock(aal_reg_mutex);\
                                                         }while(0)


#define REG_FIELD_READ_PORT(reg, port, field, val)      do{\
                                                        reg##_t reg_data;\
                                                        reg_data.wrd = *(volatile cs_uint32*)(LYNXD_REG_BASE+port*PORT_OFFSET+ (reg)*4);\
                                                        (val) = reg_data.bf.field;\
                                                        }while(0)

#define REG_FIELD_WRITE_PORT(reg, port, field, val)     do{\
                                                        reg##_t reg_data;\
                                                        cs_mutex_lock(aal_reg_mutex);\
                                                        reg_data.wrd = *(volatile cs_uint32*)(LYNXD_REG_BASE+port*PORT_OFFSET+ (reg)*4);\
                                                        reg_data.bf.field = (val);\
                                                        *(volatile cs_uint32*)(LYNXD_REG_BASE+port*PORT_OFFSET+ (reg)*4) = reg_data.wrd;\
                                                        cs_mutex_unlock(aal_reg_mutex);\
                                                        }while(0)


#define REG_READ_PORT(reg, port, val)                   do{\
                                                        (val) = *(volatile cs_uint32*)(LYNXD_REG_BASE+port*PORT_OFFSET+ (reg)*4);\
                                                        }while(0)

#define REG_WRITE_PORT(reg, port, val)                  do{\
                                                        *(volatile cs_uint32*)(LYNXD_REG_BASE+port*PORT_OFFSET+ (reg)*4) = (val);\
                                                        }while(0)

#define REG_MASK_WRITE_PORT(reg, port, mask, val)       do{\
                                                        cs_uint32 reg_data;\
                                                        cs_mutex_lock(aal_reg_mutex);\
                                                        reg_data = *(volatile cs_uint32*)(LYNXD_REG_BASE+port*PORT_OFFSET+ (reg)*4);\
                                                        reg_data &= (~mask);\
                                                        reg_data |= (val&mask);\
                                                        *(volatile cs_uint32*)(LYNXD_REG_BASE+port*PORT_OFFSET+ (reg)*4) = reg_data;\
                                                        cs_mutex_unlock(aal_reg_mutex);\
                                                        }while(0)

#define REG_ADDR_GET(reg, addr)                         do{\
                                                        (addr) = LYNXD_REG_BASE+(reg)*4;\
                                                        }while(0)




#endif /* __AAL_UTIL_H__ */

