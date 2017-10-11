/************************************************************************
 * RSTP library - Rapid Spanning Tree (802.1t, 802.1w)
 * Copyright (C) 2001-2003 Optical Access
 * Author: Alex Rozin
 *
 * This file is part of RSTP library.
 *
 * RSTP library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; version 2.1
 *
 * RSTP library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with RSTP library; see the file COPYING.  If not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 **********************************************************************/

/* Mutual RSTP definitions */

#ifndef _STP_BASE_H__
#define _STP_BASE_H__

#include "iros_config.h"

#include <stdio.h>
#include <stdlib.h>
#include "plat_common.h"
#include "cs_types.h"
#include "bitmap.h"
#include "uid_stp.h"
#include "stp_bpdu.h"
#include "vector.h"
#include "times.h"

#define Print cs_printf

#define  STP_DEF_VLAN                       0
#define  STP_BPDU_LEN                       128
#define  STP_DEBUG_NONE                  0x00000000
#define  STP_DEBUG_ERROR                0x00000001
#define  STP_DEBUG_EVENT                0x00000002
#define  STP_DEBUG_PACKET              0x00000004
#define  STP_DEBUG_DETAIL               0x00000008
#define  STP_DEBUG_ALL                     0xffffffff
#define  STP_LOCAL_LOOP_DELAY       60

#define __LINUX__
#define STP_DBG 1

#ifndef INOUT
#  define IN      /* consider as comments near 'input' parameters */
#  define OUT     /* consider as comments near 'output' parameters */
#  define INOUT   /* consider as comments near 'input/output' parameters */
#endif

#ifndef Bool
#  define Bool        int
#  define False       0
#  define True        1
#endif


#define STP_LOG(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_DBG0,IROS_MID_STP,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define STP_STACK_LOG(arg,...) \
    cs_log_msg(IROS_LOG_LEVEL_DBG0,IROS_MID_STACK_RSTP,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)
    
#define RSTP_ERRORS { \
  CHOOSE(STP_OK),                                       \
  CHOOSE(STP_Cannot_Find_Vlan),         \
  CHOOSE(STP_Imlicite_Instance_Create_Failed),          \
  CHOOSE(STP_Small_Bridge_Priority),                    \
  CHOOSE(STP_Large_Bridge_Priority),                    \
  CHOOSE(STP_Small_Hello_Time),                         \
  CHOOSE(STP_Large_Hello_Time),                         \
  CHOOSE(STP_Small_Max_Age),                            \
  CHOOSE(STP_Large_Max_Age),                            \
  CHOOSE(STP_Small_Forward_Delay),                      \
  CHOOSE(STP_Large_Forward_Delay),                      \
  CHOOSE(STP_Forward_Delay_And_Max_Age_Are_Inconsistent),\
  CHOOSE(STP_Hello_Time_And_Max_Age_Are_Inconsistent),  \
  CHOOSE(STP_Vlan_Had_Not_Yet_Been_Created),            \
  CHOOSE(STP_Port_Is_Absent_In_The_Vlan),               \
  CHOOSE(STP_Big_len8023_Format),                       \
  CHOOSE(STP_Small_len8023_Format),                     \
  CHOOSE(STP_len8023_Format_Gt_Len),                    \
  CHOOSE(STP_Not_Proper_802_3_Packet),                  \
  CHOOSE(STP_Invalid_Protocol),                         \
  CHOOSE(STP_Invalid_Version),                          \
  CHOOSE(STP_Had_Not_Yet_Been_Enabled_On_The_Vlan),     \
  CHOOSE(STP_Cannot_Create_Instance_For_Vlan),          \
  CHOOSE(STP_Cannot_Create_Instance_For_Port),          \
  CHOOSE(STP_Invalid_Bridge_Priority),                  \
  CHOOSE(STP_There_Are_No_Ports),                       \
  CHOOSE(STP_Cannot_Compute_Bridge_Prio),               \
  CHOOSE(STP_Another_Error),                            \
  CHOOSE(STP_Nothing_To_Do),                            \
  CHOOSE(STP_LAST_DUMMY),                               \
}

#define CHOOSE(a) a
typedef enum RSTP_ERRORS RSTP_ERRORS_T;
#undef CHOOSE

#define STP_FATAL(TXT, MSG, EXCOD)                      \
      {STP_LOG ("FATAL:%s failed: %s:%d", TXT, MSG, EXCOD);  \
      exit (EXCOD);}

#define STP_MALLOC(PTR, TYPE, MSG)               \
  {                                              \
    PTR = (TYPE*) iros_malloc(IROS_MID_STP, sizeof (TYPE)); \
    if (! PTR) {                                 \
      STP_FATAL("malloc", MSG, -6);              \
    }                                            \
  }

#define STP_FREE(PTR, MSG)              \
  {                                     \
    if (! PTR) {                        \
      STP_FATAL("free", MSG, -66);      \
    }                                   \
    iros_free (PTR);                         \
    PTR = NULL;                         \
  }

#define STP_STRDUP(PTR, SRC, MSG)       \
  {                                     \
    PTR = stp_strdup (SRC);                 \
    if (! PTR) {                        \
      STP_FATAL("stp_strdup", MSG, -7);     \
    }                                   \
  }

#define STP_NEW_IN_LIST(WHAT, TYPE, LIST, MSG)  \
  {                                             \
    STP_MALLOC(WHAT, TYPE, MSG);                \
    WHAT->next = LIST;                          \
    LIST = WHAT;                                \
  }

typedef enum {
  RSTP_PORT_EN_T,
  RSTP_PORT_DIS_T,
  RSTP_PORT_SPEED_T,
  RSTP_PORT_DPLEX_T,
  RSTP_PORT_RX_T,
  RSTP_PORT_TIME_T,
  RSTP_PORT_MNGR_T,
  RSTP_EVENT_LAST_DUMMY

} RSTP_EVENT_T;


typedef enum {
STP_PON_PORT_TYPE = 1,
STP_UNI_PORT_TYPE ,
STP_SW_PORT_TYPE ,
STP_UNKOWN_PORT_TYPE=100
} STP_PORT_TYPE_T;

#endif /*  _STP_BASE_H__ */

