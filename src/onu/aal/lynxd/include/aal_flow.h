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

/*

    This file is used for flow management code, such as flow control, queue map, rate limit, shaper.
*/


#ifndef __AAL_FLOW_H__
#define __AAL_FLOW_H__

#include "cs_types.h"
#include "aal.h"

#ifndef PORT_OFFSET
#define PORT_OFFSET (4*FE_LE_STRIDE)
#endif

#define AAL_ALL_FIELD_MSK 0xffffffff
#define AAL_MAX_DSCP 64
#define AAL_MAX_RATE 1000000
#define AAL_MAX_CBS 500
#define AAL_MAX_PRI_NUM 8


#define REG_INDIR_READ_2DATA(port,reg, index, data1, data2) do{\
                                                        reg##_t reg_access; \
                                                        cs_uint16 counter = 0; \
                                                        reg_access.wrd = 0;\
                                                        reg_access.bf.access = 1;\
                                                        reg_access.bf.rbw = 0;\
                                                        reg_access.wrd |= index;\
                                                        *(volatile cs_uint32*)(LYNXD_REG_BASE+PORT_OFFSET*port+reg*4) = reg_access.wrd;\
                                                        for(counter = 0; counter <10000; counter++){\
                                                            reg_access.wrd = *(volatile cs_uint32*)(LYNXD_REG_BASE+PORT_OFFSET*port+reg*4);\
                                                            if(reg_access.bf.access == 0){\
                                                                break;\
                                                            }\
                                                        }\
                                                        data1 = *(volatile cs_uint32*)(LYNXD_REG_BASE+PORT_OFFSET*port+(reg+2)*4);\
                                                        data2 = *(volatile cs_uint32*)(LYNXD_REG_BASE+PORT_OFFSET*port+(reg+1)*4);\
                                                        }while(0)

#define REG_INDIR_WRITE_2DATA(port,reg, index, mskdata1, data1, mskdata2, data2) do{ \
                                                            reg##_t reg_access; \
                                                            cs_uint16 counter = 0; \
                                                            cs_uint32 temp_data1, temp_data2; \
                                                            cs_mutex_lock(aal_reg_mutex);\
                                                            reg_access.wrd = 0;\
                                                            reg_access.bf.access = 1;\
                                                            reg_access.bf.rbw = 0;\
                                                            reg_access.wrd |= index;\
                                                            *(volatile cs_uint32*)(LYNXD_REG_BASE+PORT_OFFSET*port+reg*4) = reg_access.wrd;\
                                                            for(counter = 0; counter <10000; counter++){\
                                                                reg_access.wrd = *(volatile cs_uint32*)(LYNXD_REG_BASE+PORT_OFFSET*port+reg*4);\
                                                                if(reg_access.bf.access == 0){\
                                                                    break;\
                                                                }\
                                                            }\
                                                            temp_data1 = *(volatile cs_uint32*)(LYNXD_REG_BASE+PORT_OFFSET*port+(reg+2)*4);\
                                                            temp_data2 = *(volatile cs_uint32*)(LYNXD_REG_BASE+PORT_OFFSET*port+(reg+1)*4);\
                                                            *(volatile cs_uint32*)(LYNXD_REG_BASE+PORT_OFFSET*port+(reg+2)*4) = ((temp_data1&(~mskdata1))|(data1&mskdata1));\
                                                            *(volatile cs_uint32*)(LYNXD_REG_BASE+PORT_OFFSET*port+(reg+1)*4) = ((temp_data2&(~mskdata2))|(data2&mskdata2));\
                                                            reg_access.wrd = 0;\
                                                            reg_access.bf.access = 1;\
                                                            reg_access.bf.rbw = 1;\
                                                            reg_access.wrd |= index;\
                                                            *(volatile cs_uint32*)(LYNXD_REG_BASE+PORT_OFFSET*port+reg*4) = reg_access.wrd;\
                                                            for(counter = 0; counter <10000; counter++){\
                                                                reg_access.wrd = *(volatile cs_uint32*)(LYNXD_REG_BASE+PORT_OFFSET*port+reg*4);\
                                                                if(reg_access.bf.access == 0){\
                                                                    break;\
                                                                }\
                                                            }\
                                                            cs_mutex_unlock(aal_reg_mutex);\
                                                            }while(0)



typedef union{
    struct{
        cs_uint8    dot1p       :1,
                    cos         :1,
                    dscp        :1,
                    tc          :1,
                    flow_id     :1,
                    rev         :3;
    }s;
    cs_uint8 u;
}cs_aal_flow_mapping_msk_t;

typedef volatile union {
  struct {
    cs_uint32 map0              :  3 ; /* bits 2:0 */
    cs_uint32 map1              :  3 ; /* bits 5:3 */
    cs_uint32 map2              :  3 ; /* bits 8:6 */
    cs_uint32 map3              :  3 ; /* bits 11:9 */
    cs_uint32 map4              :  3 ; /* bits 14:12 */
    cs_uint32 map5              :  3 ; /* bits 17:15 */
    cs_uint32 map6              :  3 ; /* bits 20:18 */
    cs_uint32 map7              :  3 ; /* bits 23:21 */
    cs_uint32 rsrvd1            :  8 ;
  } s ;
  cs_uint32     u ;
} cs_aal_cos_dot1p_map_t;

typedef volatile union {
  struct {
    cs_uint32 map0              :  5 ; /* bits 4:0 */
    cs_uint32 map1              :  5 ; /* bits 9:5 */
    cs_uint32 map2              :  5 ; /* bits 14:10 */
    cs_uint32 map3              :  5 ; /* bits 19:15 */
    cs_uint32 map4              :  5 ; /* bits 24:20 */
    cs_uint32 map5              :  5 ; /* bits 29:25 */
    cs_uint32 rsrvd1            :  2 ;
  } s ;
  cs_uint32     u ;
} cs_aal_flowid_map0_t;

typedef volatile union {
  struct {
    cs_uint32 map6              :  5 ; /* bits 4:0 */
    cs_uint32 map7              :  5 ; /* bits 9:5 */
    cs_uint32 rsrvd1            : 22 ;
  } s ;
  cs_uint32     u ;
} cs_aal_flowid_map1_t;

typedef struct{
    cs_aal_flowid_map0_t flow0;
    cs_aal_flowid_map1_t flow1;
}cs_aal_flowid_map_t;

typedef volatile union {
  struct {
    cs_uint32 map0              :  6 ; /* bits 5:0 */
    cs_uint32 map1              :  6 ; /* bits 11:6 */
    cs_uint32 map2              :  6 ; /* bits 17:12 */
    cs_uint32 map3              :  6 ; /* bits 23:18 */
    cs_uint32 map4              :  6 ; /* bits 29:24 */
    cs_uint32 rsrvd1            :  2 ;
  } s ;
  cs_uint32     u ;
} cs_aal_dscp_tc_map0_t;

typedef volatile union {
  struct {
    cs_uint32 map5              :  6 ; /* bits 5:0 */
    cs_uint32 map6              :  6 ; /* bits 11:6 */
    cs_uint32 map7              :  6 ; /* bits 17:12 */
    cs_uint32 rsrvd1            : 14 ;
  } s ;
  cs_uint32     u ;
} cs_aal_dscp_tc_map1_t;

typedef struct{
    cs_aal_dscp_tc_map0_t dscp_tc0;
    cs_aal_dscp_tc_map1_t dscp_tc1;
}cs_aal_dscp_tc_map_t;

typedef struct{
    cs_aal_cos_dot1p_map_t dot1p;
    cs_aal_cos_dot1p_map_t cos;
    cs_aal_dscp_tc_map_t dscp;
    cs_aal_dscp_tc_map_t tc;
    cs_aal_flowid_map_t flow_id;
}cs_aal_flow_mapping_t;


typedef volatile union {
  struct {
    cs_uint32 dscp_map          :  6 ; /* bits 5:0 */
    cs_uint32 dot1p_map         :  3 ; /* bits 8:6 */
    cs_uint32 cos_map           :  3 ; /* bits 11:9 */
    cs_uint32 flowid_map        :  5 ; /* bits 16:12 */    
    cs_uint32 rsrvd1            : 15 ;
  } s ;
  cs_uint32     u ;
} cs_aal_dscp_map_t;

typedef enum {
    AAL_RATE_LIMIT_GE_SP  ,
    AAL_RATE_LIMIT_GE_UMC ,
    AAL_RATE_LIMIT_GE_BC,
    AAL_RATE_LIMIT_GE_UUC ,
    AAL_RATE_LIMIT_PON_SP  ,
    AAL_RATE_LIMIT_PON_UMC ,
    AAL_RATE_LIMIT_PON_BC,
    AAL_RATE_LIMIT_PON_UUC ,
    AAL_RATE_LIMIT_VOIP_SP  ,
    AAL_RATE_LIMIT_VOIP_UMC ,
    AAL_RATE_LIMIT_VOIP_BC,
    AAL_RATE_LIMIT_VOIP_UUC ,
    AAL_RATE_LIMIT_FLOW_0  ,  /* AAL_RATE_LIMIT_FLOW_0 to AAL_RATE_LIMIT_FLOW_4 must be continuous */
    AAL_RATE_LIMIT_FLOW_1 ,
    AAL_RATE_LIMIT_FLOW_2,
    AAL_RATE_LIMIT_FLOW_3  ,
    AAL_RATE_LIMIT_FLOW_4 ,
    AAL_RATE_LIMIT_FLOW_5,
    AAL_RATE_LIMIT_FLOW_6  ,
    AAL_RATE_LIMIT_FLOW_7,
    AAL_RATE_LIMIT_FLOW_8,
    AAL_RATE_LIMIT_FLOW_9,
    AAL_RATE_LIMIT_FLOW_10  ,
    AAL_RATE_LIMIT_FLOW_11 ,
    AAL_RATE_LIMIT_FLOW_12,
    AAL_RATE_LIMIT_FLOW_13  ,
    AAL_RATE_LIMIT_FLOW_14 ,
    AAL_RATE_LIMIT_FLOW_15,
    AAL_RATE_LIMIT_FLOW_16  ,
    AAL_RATE_LIMIT_FLOW_17,
    AAL_RATE_LIMIT_FLOW_18,
    AAL_RATE_LIMIT_FLOW_19,
    AAL_RATE_LIMIT_FLOW_20  ,
    AAL_RATE_LIMIT_FLOW_21 ,
    AAL_RATE_LIMIT_FLOW_22,
    AAL_RATE_LIMIT_FLOW_23  ,
    AAL_RATE_LIMIT_FLOW_24 ,
    AAL_RATE_LIMIT_FLOW_25,
    AAL_RATE_LIMIT_FLOW_26  ,
    AAL_RATE_LIMIT_FLOW_27,
    AAL_RATE_LIMIT_FLOW_28,
    AAL_RATE_LIMIT_FLOW_29,
    AAL_RATE_LIMIT_FLOW_30  ,
    AAL_RATE_LIMIT_FLOW_31 ,
    AAL_RATE_LIMIT_ACC_FLOW_A ,
    AAL_RATE_LIMIT_ACC_FLOW_B ,
    AAL_RATE_LIMIT_ACC_FLOW_C ,
    AAL_RATE_LIMIT_ACC_FLOW_D ,
    AAL_RATE_LIMIT_CPU_DEST
} cs_aal_rate_limit_type_t;

typedef struct {
    union{
        struct{
            cs_uint32 rlmode        :1;
            cs_uint32 enable        :1;
            cs_uint32 fwdcntmod     :1;
            cs_uint32 pircntmod     :1;
            cs_uint32 tdcntmod      :1;
            cs_uint32 reserve       :27;            
        }s;
        cs_uint32 u;
    }ctrl;
    cs_uint32 rate;                     /*unit kbps, from 0-1000000*/
    cs_uint32 cbs;                      /*unit KB, from 0-512, it's better bigger than 10*/
    cs_uint32 tbc;                      /**token bucket counter * */ 
    cs_uint32 acc_msk_a;                /**Accumulated  mask a * */ 
    cs_uint32 acc_msk_b;                /**Accumulated  mask b * */ 
    cs_uint32 acc_msk_c;                /**Accumulated  mask c * */ 
    cs_uint32 acc_msk_d;                /**Accumulated  mask d * */ 
    cs_uint64 fwdcnt;
    cs_uint64 pircnt;
    cs_uint64 tdcnt;
} cs_aal_rate_limit_t;

typedef union{
    struct{
        cs_uint32   rlmode      :1,
                    rate        :1,
                    cbs         :1,
                    acc_msk_a   :1,                    
                    acc_msk_b   :1,
                    acc_msk_c   :1,
                    acc_msk_d   :1,
                    enable      :1,
                    fwdcnt      :1,
                    pircnt      :1,
                    tdcnt       :1,
                    tbc         :1,
                    reserve     :20;
    }s;
    cs_uint32 u;
}cs_aal_rate_limit_msk_t;

typedef enum {
    AAL_STORM_CTRL_MODE_BC          = 0, /* storm control is only valid for broadcast */
    AAL_STORM_CTRL_MODE_BC_UMC      = 1, /* storm control is valid for broadcast+ unknown multicast*/    
    AAL_STORM_CTRL_MODE_BC_UMC_UUC  = 2, /* storm control is valid for broadcast+ unknown multicast +unknown unicast */
    AAL_STORM_CTRL_MODE_DIS  = 3, /* storm control is invalid */
} cs_aal_storm_ctrl_mod_t;

typedef enum {
    AAL_RATE_LIMIT_MODE_BPS          = 0, /* bits per second */
    AAL_RATE_LIMIT_MODE_PPS          = 1 /* packets per second*/    
} cs_aal_rate_limit_mod_t;
typedef union {
    struct {
        cs_uint32   pir_wrate:1;
        cs_uint32   shaper_wrate:1;
        cs_uint32   mii_cpup_en:1; 
        cs_uint32   cpu_cos7_td_drop_dis:1;        
    }s;
    cs_uint32 u;
} cs_aal_te_ctrl_mask_t;

typedef struct {
    cs_uint8 pir_wrate;    // 0~31(default=20): All PIR rate limit's compensation in unit of byte
                                        // For pure packet size rate limit, leave it as zero;
                                        // For line speed rate limt, use 20 (8preamble+12IPG)
    cs_uint8 shaper_wrate; // 0~31(default=20): All shaper's compensation in unit of byte
                                        // For pure packet size rate limit, leave it as zero;
                                        // For line speed rate limt, use 20 (8preamble+12IPG)
    cs_boolean mii_cpup_en; // (default=true) When enabled the packets egress to the MII1 port will be treated as packet to CPU 
    cs_boolean cpu_cos7_td_drop_dis; // (default=true), True, disable tail drop packet to CPU port with COS =7. 
                                                        // False, enable tail drop packet to CPU port with COS = 7.  
}cs_aal_te_ctrl_t;

typedef volatile union {
  struct {
    cs_uint32 l_thrsld             : 11 ; /* bits 10:0 */
    cs_uint32 rsrvd1               :  5 ;
    cs_uint32 u_thrsld             : 11 ; /* bits 26:16 */
    cs_uint32 rsrvd2               :  4 ;
    cs_uint32 en                   :  1 ; /* bits 31:31 */

  } s ;
  cs_uint32     u ;
} cs_aal_flow_control_fifo_t;

typedef volatile union {
  struct {
      cs_uint32 mem                 : 1 ;
      cs_uint32 fifo                : 1 ;       
      cs_uint32 te_pause            : 1 ;
      cs_uint32 rsrvd               : 29 ;
  } s ;
  cs_uint32     u ;
} cs_aal_flow_control_msk_t;

typedef struct{
    cs_uint16 rx_en;
    cs_uint16 tx_en;
}cs_aal_flow_control_mem_t;

typedef struct{
    cs_aal_flow_control_msk_t fc_msk;
    cs_aal_flow_control_fifo_t fifo_fc;
    cs_aal_flow_control_mem_t mem_fc;
    cs_boolean   te_pause;
}cs_aal_flow_control_t;

extern cs_status aal_flow_control_set(cs_aal_flow_control_t *cfg);
extern cs_status aal_flow_control_get(cs_aal_flow_control_t *cfg);
extern cs_status aal_flow_init(void);
extern cs_status aal_flow_te_ctrl_get(cs_aal_te_ctrl_t *cfg);
extern cs_status aal_flow_te_ctrl_set(cs_aal_te_ctrl_mask_t *msk, cs_aal_te_ctrl_t *cfg);
extern cs_status aal_flow_shaper_get(cs_aal_port_id_t port_id,cs_aal_rate_limit_t *rate);
extern cs_status aal_flow_shaper_set(cs_aal_port_id_t port_id,cs_aal_rate_limit_t *rate);
extern cs_status aal_flow_storm_ctrl_get(cs_aal_port_id_t port_id, cs_aal_storm_ctrl_mod_t *mod);
extern cs_status aal_flow_storm_ctrl_set(cs_aal_port_id_t port_id, cs_aal_storm_ctrl_mod_t mod);
extern cs_status aal_flow_rate_limit_get(cs_aal_rate_limit_type_t flow_id,cs_aal_rate_limit_msk_t *msk, cs_aal_rate_limit_t *cfg);
extern cs_status aal_flow_rate_limit_set(cs_aal_rate_limit_type_t flow_id,cs_aal_rate_limit_msk_t *msk, cs_aal_rate_limit_t *cfg);
extern cs_status aal_flow_tc_mapping_get(cs_aal_port_id_t port_id, cs_aal_dscp_map_t *mapping);
extern cs_status aal_flow_tc_mapping_set(cs_aal_port_id_t port_id,cs_aal_dscp_map_t *msk, cs_aal_dscp_map_t *mapping);
extern cs_status aal_flow_dscp_mapping_get(cs_aal_port_id_t port_id, cs_aal_dscp_map_t *mapping);
extern cs_status aal_flow_dscp_mapping_set(cs_aal_port_id_t port_id,cs_aal_dscp_map_t *msk, cs_aal_dscp_map_t *mapping);
extern cs_status aal_flow_cos_mapping_get(cs_aal_port_id_t port_id, cs_aal_flowid_map_t *mapping);
extern cs_status aal_flow_cos_mapping_set(cs_aal_port_id_t port_id, cs_aal_flowid_map_t *mapping);
extern cs_status aal_flow_dot1p_mapping_get(cs_aal_port_id_t port_id, cs_aal_flow_mapping_t *mapping);
extern cs_status aal_flow_dot1p_mapping_set(cs_aal_port_id_t port_id, cs_aal_flow_mapping_msk_t *msk,cs_aal_flow_mapping_t *mapping);
extern cs_status aal_rx_flow_control_set(cs_boolean enable);
extern cs_status aal_rx_flow_control_get(cs_boolean *enable);

#endif /* __AAL_FLOW_H__ */

