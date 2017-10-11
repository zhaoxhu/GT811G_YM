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
#include "plat_common.h"
#include "aal.h"
#include "aal_util.h"
#include "aal_bm_util.h"
#include "aal_bm.h"
#include "aal_flow.h"
#include "aal_uni.h"
#include "aal_pon.h"


typedef struct {
    cs_uint32 offset;
    cs_uint8 *name;
    cs_uint32 default_value;
}__register_dump_t;

typedef struct {
    cs_uint32 offset;
    cs_uint8 *name;
} __counter_entry_t;


typedef enum{
    NORMAL,
    BPDU,
    I8021X,
    IEEE_REV_MAC,
    MY_MAC,
    MC_A,
    MC_B,
    MC_C,
    IGMP_MLD,
    ARP,
    OAM,
    MPCP,
    DHCP,
    PPPOE_D,
    PPPOE_S,
    HELLO,
    IPV6_NDP,
    USER_TYPE0,
    USER_TYPE1,
    USER_TYPE2,
    USER_TYPE3,  
}__special_packet_type_t;
typedef struct{
    __special_packet_type_t pkt_type;
    cs_uint8    *name;
}__packet_type_t;

__packet_type_t __dump_packet_type[21] = {
        {NORMAL, "NORMAL"},
        {BPDU, "BPDU"},
        {I8021X, "I8021X"},
        {IEEE_REV_MAC, "IEEE_REV_MAC"},
        {MY_MAC, "MY_MAC"},
        {MC_A, "MC_A"},
        {MC_B, "MC_B"},
        {MC_C, "MC_C"},
        {IGMP_MLD, "IGMP_MLD"},
        {ARP, "ARP"},
        {OAM, "OAM"},
        {MPCP, "MPCP"},
        {DHCP, "DHCP"},
        {PPPOE_D, "PPPOE_D"},
        {PPPOE_S, "PPPOE_S"},
        {HELLO, "HELLO"},
        {IPV6_NDP, "IPV6_NDP"},
        {USER_TYPE0, "USER_TYPE0"},
        {USER_TYPE1, "USER_TYPE1"},
        {USER_TYPE2, "USER_TYPE2"},
        {USER_TYPE3, "USER_TYPE3"}
};


void do_dump_bm_register(void)
{
    __register_dump_t bm_setting[] = {
        {
            BM_SCH_CFG, "BM_SCH_CFG ", BM_SCH_CFG_dft
        },
        {
            BM_SHARED_BUFFER_CFG, "BM_SHARED_BUFFER_CFG ", BM_SHARED_BUFFER_CFG_dft
        },
        {
            BM_GE_PORT_BUFFER_CFG, "BM_GE_PORT_BUFFER_CFG ", BM_GE_PORT_BUFFER_CFG_dft
        },
        {
            BM_PON_PORT_BUFFER_CFG, "BM_PON_PORT_BUFFER_CFG ", BM_PON_PORT_BUFFER_CFG_dft
        },
        {
            BM_CPU_PORT_BUFFER_CFG, "BM_CPU_PORT_BUFFER_CFG ", BM_CPU_PORT_BUFFER_CFG_dft
        },
        {
            BM_SHARED_BUFFER_THRESHOLD, "BM_SHARED_BUFFER_THRESHOLD ", BM_SHARED_BUFFER_THRESHOLD_dft
        },
        {
            BM_GE_QUEUE_LENGTH_LIMIT_COS7_4, "BM_GE_QUEUE_LENGTH_LIMIT_COS7_4 ", BM_GE_QUEUE_LENGTH_LIMIT_COS7_4_dft
        },
        {
            BM_GE_QUEUE_LENGTH_LIMIT_COS3_0, "BM_GE_QUEUE_LENGTH_LIMIT_COS3_0 ", BM_GE_QUEUE_LENGTH_LIMIT_COS3_0_dft
        },
        {
            BM_PON_QUEUE_LENGTH_LIMIT_COS7_4, "BM_PON_QUEUE_LENGTH_LIMIT_COS7_4 ", BM_PON_QUEUE_LENGTH_LIMIT_COS7_4_dft
        },
        {
            BM_PON_QUEUE_LENGTH_LIMIT_COS3_0, "BM_PON_QUEUE_LENGTH_LIMIT_COS3_0 ", BM_PON_QUEUE_LENGTH_LIMIT_COS3_0_dft
        },
        {
            BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0, "BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0 ", BM_CPU_QUEUE_LENGTH_LIMIT_COS7_0_dft
        },
        {
            BM_PC_CFG1, "BM_PC_CFG1 ", BM_PC_CFG1_dft
        },
        {
            BM_PC_CFG0, "BM_PC_CFG0 ", BM_PC_CFG0_dft
        },
        {
            BM_PORT_TX_CTRL, "BM_PORT_TX_CTRL ", BM_PORT_TX_CTRL_dft
        },
        {
            BM_DRR_WEIGHT_BASE, "BM_DRR_WEIGHT_BASE ", BM_DRR_WEIGHT_BASE_dft
        },
        {
            BM_GE_WEIGHT_RATIO_COS7_4, "BM_GE_WEIGHT_RATIO_COS7_4 ", BM_GE_WEIGHT_RATIO_COS7_4_dft
        },
        {
            BM_GE_WEIGHT_RATIO_COS3_0, "BM_GE_WEIGHT_RATIO_COS3_0 ", BM_GE_WEIGHT_RATIO_COS3_0_dft
        },
        {
            BM_PON_WEIGHT_RATIO_COS7_4, "BM_PON_WEIGHT_RATIO_COS7_4 ", BM_PON_WEIGHT_RATIO_COS7_4_dft
        },
        {
            BM_PON_WEIGHT_RATIO_COS3_0, "BM_PON_WEIGHT_RATIO_COS3_0 ", BM_PON_WEIGHT_RATIO_COS3_0_dft
        },
        {
            BM_ABR_CFG, "BM_ABR_CFG ", BM_ABR_CFG_dft
        },
        {
            BM_DP_CFG, "BM_DP_CFG ", BM_DP_CFG_dft
        },
        {
            BM_DP_CFG_STAGE2_1, "BM_DP_CFG_STAGE2_1 ", BM_DP_CFG_STAGE2_1_dft
        },
        {
            BM_DP_CFG_STAGE4_3, "BM_DP_CFG_STAGE4_3 ", BM_DP_CFG_STAGE4_3_dft
        },
        {
            BM_DP_CFG_STAGE6_5, "BM_DP_CFG_STAGE6_5 ", BM_DP_CFG_STAGE6_5_dft
        },

        {
            BM_DP_CFG_STAGE8_7, "BM_DP_CFG_STAGE8_7 ", BM_DP_CFG_STAGE8_7_dft
        },

        {
            BM_MA_DPID4_COS_CFG, "BM_MA_DPID4_COS_CFG ",  BM_MA_DPID4_COS_CFG_dft
        },
        {
            BM_MA_DPID5_COS_CFG, "BM_MA_DPID5_COS_CFG ",  BM_MA_DPID5_COS_CFG_dft
        },
        {
            BM_MA_DPID6_COS_CFG, "BM_MA_DPID6_COS_CFG ",  BM_MA_DPID6_COS_CFG_dft
        },
        {
            BM_MA_COS_DPID_CFG, "BM_MA_COS_DPID_CFG ", BM_MA_COS_DPID_CFG_dft
        },

        {
            BM_FLUSH_CTRL, "BM_FLUSH_CTRL ", BM_FLUSH_CTRL_dft
        },

        {
            BM_LINKLIST_SIZE, "BM_LINKLIST_SIZE ", BM_LINKLIST_SIZE_dft
        },
        {
            BM_PORT_SHAPER_CFG,  "BM_PORT_SHAPER_CFG", BM_PORT_SHAPER_CFG_dft
        },

        {
            BM_US_Q0_END, "BM_US_Q0_END ", BM_US_Q0_END_dft
        },
        {
            BM_US_Q0_START, "BM_US_Q0_START ", BM_US_Q0_START_dft
        },
        {
            BM_US_Q1_END, "BM_US_Q1_END ", BM_US_Q1_END_dft
        },
        {
            BM_US_Q1_START, "BM_US_Q1_START ", BM_US_Q1_START_dft
        },

        {
            BM_US_Q2_END, "BM_US_Q2_END ", BM_US_Q2_END_dft
        },
        {
            BM_US_Q2_START, "BM_US_Q2_START ", BM_US_Q2_START_dft
        },

        {
            BM_US_Q3_END, "BM_US_Q3_END ", BM_US_Q3_END_dft
        },

        {
            BM_US_Q3_START, "BM_US_Q3_START ", BM_US_Q3_START_dft
        },
        {
            BM_US_Q4_END, "BM_US_Q4_END ", BM_US_Q4_END_dft
        },
        {
            BM_US_Q4_START, "BM_US_Q4_START ", BM_US_Q4_START_dft
        },
        {
            BM_DS_Q0_END, "BM_DS_Q0_END ", BM_DS_Q0_END_dft
        },


        {
            BM_DS_Q0_START, "BM_DS_Q0_START ", BM_DS_Q0_START_dft
        },
        {
            BM_DS_Q1_END, "BM_DS_Q1_END ", BM_DS_Q1_END_dft
        },
        {
            BM_DS_Q1_START, "BM_DS_Q1_START ", BM_DS_Q1_START_dft
        },
        {
            BM_DS_Q2_END, "BM_DS_Q2_END ", BM_DS_Q2_END_dft
        },
        {
            BM_DS_Q2_START, "BM_DS_Q2_START ", BM_DS_Q2_START_dft
        },

        {
            BM_DS_Q3_END, "BM_DS_Q3_END ", BM_DS_Q3_END_dft
        },

        {
            BM_DS_Q3_START, "BM_DS_Q3_START ", BM_DS_Q3_START_dft
        },


        {
            BM_DS_Q4_END, "BM_DS_Q4_END ", BM_DS_Q4_END_dft
        },
        {
            BM_DS_Q4_START, "BM_DS_Q4_START ", BM_DS_Q4_START_dft
        },
        {
            BM_MA_Q0_END, "BM_MA_Q0_END ", BM_MA_Q0_END_dft
        },
        {
            BM_MA_Q1_END, "BM_MA_Q1_END ", BM_MA_Q1_END_dft
        },
        {
            BM_MA_Q2_END, "BM_MA_Q2_END ", BM_MA_Q2_END_dft
        },
        {
            BM_MA_Q3_END, "BM_MA_Q3_END ", BM_MA_Q3_END_dft
        },
        {
            BM_MA_Q4_END, "BM_MA_Q4_END ", BM_MA_Q4_END_dft
        },
        {
            BM_MA_Q5_END, "BM_MA_Q5_END ", BM_MA_Q5_END_dft
        },

        {
            BM_MA_Q6_END, "BM_MA_Q6_END ", BM_MA_Q6_END_dft
        },
        {
            BM_MA_Q7_END, "BM_MA_Q7_END_END ", BM_MA_Q7_END_dft
        },
        {
            BM_MA_Q7_START, "BM_MA_Q7_START ", BM_MA_Q7_START_dft
        },

        {
            BM_CPU_ACCESS_OFFSET, "BM_CPU_ACCESS_OFFSET ", BM_CPU_ACCESS_OFFSET_dft
        },
        {
            BM_US_Q0_FLOW_CTRL, "BM_US_Q0_FLOW_CTRL ", BM_US_Q0_FLOW_CTRL_dft
        },
        {
            BM_US_Q1_FLOW_CTRL, "BM_US_Q1_FLOW_CTRL ", BM_US_Q1_FLOW_CTRL_dft
        },
        {
            BM_US_Q2_FLOW_CTRL, "BM_US_Q2_FLOW_CTRL ", BM_US_Q2_FLOW_CTRL_dft
        },
        {
            BM_US_Q3_FLOW_CTRL, "BM_US_Q3_FLOW_CTRL ", BM_US_Q3_FLOW_CTRL_dft
        },
        {
            BM_US_Q4_FLOW_CTRL, "BM_US_Q4_FLOW_CTRL ", BM_US_Q4_FLOW_CTRL_dft
        },

        {
            BM_GE_PORT_TX_BUFFER_LMT, "BM_GE_PORT_TX_BUFFER_LMT ", BM_GE_PORT_TX_BUFFER_LMT_dft
        },
        {
            BM_PON_PORT_TX_BUFFER_LMT, "BM_PON_PORT_TX_BUFFER_LMT ",
            BM_PON_PORT_TX_BUFFER_LMT_dft
        },
        {
            BM_MA_PORT_TX_BUFFER_LMT, "BM_MA_PORT_TX_BUFFER_LMT ",
            BM_MA_PORT_TX_BUFFER_LMT_dft
        },

        {
            BM_OAM_PKT_EN, "BM_OAM_PKT_EN ", BM_OAM_PKT_EN_dft
        },
        {
            BM_OAM_PKT_HEADER, "BM_OAM_PKT_HEADER ", BM_OAM_PKT_HEADER_dft
        },
        {
            BM_Q_FLUSH_EN, "BM_Q_FLUSH_EN ", BM_Q_FLUSH_EN_dft
        },
        {
            BM_Q_FLUSH_CTRL, "BM_Q_FLUSH_CTRL ", BM_Q_FLUSH_CTRL_dft
        },
        {
            BM_INTERRUPT_E, "BM_INTERRUPT_E ", BM_INTERRUPT_E_dft
        },
        {
            BM_INTERRUPT, "BM_INTERRUPT ", BM_INTERRUPT_dft
        },

        {
            TE_CTRL, "TE_CTRL ", TE_CTRL_dft
        },

        {
            TE_CPU_TD_CFG, "TE_CPU_TD_CFG ", TE_CPU_TD_CFG_dft
        },

        {
            TE_GE_TD_CFG1, "TE_GE_TD_CFG1 ", TE_GE_TD_CFG1_dft
        },

        {
            TE_GE_TD_CFG0, "TE_GE_TD_CFG0 ", TE_GE_TD_CFG0_dft
        },

        {
            TE_PON_TD_CFG1, "TE_PON_TD_CFG1 ", TE_PON_TD_CFG1_dft
        },
        {
            TE_PON_TD_CFG0, "TE_PON_TD_CFG0 ", TE_PON_TD_CFG0_dft
        },

    };

    static cs_uint32 size = sizeof(bm_setting) / sizeof(__register_dump_t);

    cs_uint32 addr = 0;
    int i;
    diag_printf("                                        register-name       default         current         address \n");


    for (i = 0; i < size; i++) {
        cs_uint32 valu32;
        if (i % 10 == 0) {
//            cyg_thread_delay(10);
            //HAL_DELAY_US(1000);
            cs_thread_delay(10);

        }
        addr = LYNXD_REG_BASE + bm_setting[i].offset * 4;

        REG_READ(bm_setting[i].offset, valu32);

        diag_printf("%52s:      0x%08x     0x%08x     0x%08x\n", bm_setting[i].name, bm_setting[i].default_value, valu32, addr);
    }


    return;
}

void do_dump_mpcp_eee_register(void)
{
    __register_dump_t mpcp_eee_setting[] = {

        {
            BM_MPCP_CFG, "BM_MPCP_CFG ", BM_MPCP_CFG_dft
        },
        {
            BM_MPCP_OFFSET, "BM_MPCP_OFFSET ", BM_MPCP_OFFSET_dft
        },
        {
            BM_MPCP_Q2_THRESHOLD_7_6, "BM_MPCP_Q2_THRESHOLD_7_6 ", BM_MPCP_Q2_THRESHOLD_7_6_dft
        },
        {
            BM_MPCP_Q2_THRESHOLD_5_4, "BM_MPCP_Q2_THRESHOLD_5_4 ", BM_MPCP_Q2_THRESHOLD_5_4_dft
        },
        {
            BM_MPCP_Q2_THRESHOLD_3_2, "BM_MPCP_Q2_THRESHOLD_3_2 ", BM_MPCP_Q2_THRESHOLD_3_2_dft
        },
        {
            BM_MPCP_Q2_THRESHOLD_1_0, "BM_MPCP_Q2_THRESHOLD_1_0 ", BM_MPCP_Q2_THRESHOLD_1_0_dft
        },
        {
            BM_MPCP_Q2_MAX_7_6, "BM_MPCP_Q2_MAX_7_6 ", BM_MPCP_Q2_MAX_7_6_dft
        },
        {
            BM_MPCP_Q2_MAX_5_4, "BM_MPCP_Q2_MAX_5_4 ", BM_MPCP_Q2_MAX_5_4_dft
        },
        {
            BM_MPCP_Q2_MAX_3_2, "BM_MPCP_Q2_MAX_3_2 ", BM_MPCP_Q2_MAX_3_2_dft
        },
        {
            BM_MPCP_Q2_MAX_1_0, "BM_MPCP_Q2_MAX_1_0 ", BM_MPCP_Q2_MAX_1_0_dft
        },
        {
            BM_MPCP_QSET_2, "BM_MPCP_QSET_2 ", BM_MPCP_QSET_2_dft
        },
        {
            BM_LYNXD_MPCP_CFG, "BM_LYNXD_MPCP_CFG ", BM_LYNXD_MPCP_CFG_dft
        },
        {
            BM_EEE_ECFG,
            "BM_EEE_ECFG",
            BM_EEE_ECFG_dft
        },
        {
            BM_EEE_SW_CTRL,
            " BM_EEE_SW_CTRL",
            BM_EEE_SW_CTRL_dft
        },
        {
            BM_EEE_STAT,
            "BM_EEE_STAT",
            BM_EEE_STAT_dft
        },
        {
            BM_EEE_SLEEP_DELAY,
            " BM_EEE_SLEEP_DELAY",
            BM_EEE_SLEEP_DELAY_dft
        },
        {
            BM_EEE_WAKE_DELAY,
            "BM_EEE_WAKE_DELAY",
            BM_EEE_WAKE_DELAY_dft
        },
        {
            BM_EEE_QUEUE0,
            " BM_EEE_QUEUE0",
            BM_EEE_QUEUE0_dft
        },
        {
            BM_EEE_QUEUE1,
            "BM_EEE_QUEUE1",
            BM_EEE_QUEUE1_dft
        },
        {
            BM_EEE_QUEUE2,
            "BM_EEE_QUEUE2",
            BM_EEE_QUEUE2_dft
        },
        {
            BM_EEE_QUEUE3,
            "BM_EEE_QUEUE3",
            BM_EEE_QUEUE3_dft
        },
        {
            BM_EEE_QUEUE4,
            "BM_EEE_QUEUE4",
            BM_EEE_QUEUE4_dft
        },
        {
            BM_EEE_QUEUE5,
            "BM_EEE_QUEUE5",
            BM_EEE_QUEUE5_dft
        },
        {
            BM_EEE_QUEUE6,
            "BM_EEE_QUEUE6",
            BM_EEE_QUEUE6_dft
        },

        {
            BM_EEE_QUEUE7,
            "BM_EEE_QUEUE7",
            BM_EEE_QUEUE7_dft
        },
        {
            BM_EEE_QUEUE8,
            "BM_EEE_QUEUE8",
            BM_EEE_QUEUE8_dft
        },
        {
            BM_EEE_THRESHOLD1,
            "BM_EEE_THRESHOLD1",
            BM_EEE_THRESHOLD1_dft
        },
        {
            BM_EEE_THRESHOLD2,
            "BM_EEE_THRESHOLD2",
            BM_EEE_THRESHOLD2_dft
        },
        {
            BM_EEE_THRESHOLD3,
            "BM_EEE_THRESHOLD3",
            BM_EEE_THRESHOLD3_dft
        },
        {
            BM_EEE_THRESHOLD3,
            "BM_EEE_THRESHOLD3",
            BM_EEE_THRESHOLD3_dft
        },
        {
            BM_PON_EEE_ECFG,
            "BM_PON_EEE_ECFG",
            BM_PON_EEE_ECFG_dft
        },
        {
            BM_PON_EEE_SW_CTRL,
            " BM_PON_EEE_SW_CTRL",
            BM_PON_EEE_SW_CTRL_dft
        },
        {
            BM_PON_EEE_STAT,
            "BM_PON_EEE_STAT",
            BM_PON_EEE_STAT_dft
        },
        {
            BM_PON_EEE_SLEEP_DELAY,
            " BM_PON_EEE_SLEEP_DELAY",
            BM_PON_EEE_SLEEP_DELAY_dft
        },
        {
            BM_PON_EEE_WAKE_DELAY,
            "BM_PON_EEE_WAKE_DELAY",
            BM_PON_EEE_WAKE_DELAY_dft
        },
        {
            BM_PON_EEE_QUEUE0,
            " BM_PON_EEE_QUEUE0",
            BM_PON_EEE_QUEUE0_dft
        },
        {
            BM_PON_EEE_QUEUE1,
            "BM_PON_EEE_QUEUE1",
            BM_PON_EEE_QUEUE1_dft
        },
        {
            BM_PON_EEE_QUEUE2,
            "BM_PON_EEE_QUEUE2",
            BM_PON_EEE_QUEUE2_dft
        },
        {
            BM_PON_EEE_QUEUE3,
            "BM_PON_EEE_QUEUE3",
            BM_PON_EEE_QUEUE3_dft
        },
        {
            BM_PON_EEE_QUEUE4,
            "BM_PON_EEE_QUEUE4",
            BM_PON_EEE_QUEUE4_dft
        },
        {
            BM_PON_EEE_QUEUE5,
            "BM_PON_EEE_QUEUE5",
            BM_PON_EEE_QUEUE5_dft
        },
        {
            BM_PON_EEE_QUEUE6,
            "BM_PON_EEE_QUEUE6",
            BM_PON_EEE_QUEUE6_dft
        },

        {
            BM_PON_EEE_QUEUE7,
            "BM_PON_EEE_QUEUE7",
            BM_PON_EEE_QUEUE7_dft
        },
        {
            BM_PON_EEE_QUEUE8,
            "BM_PON_EEE_QUEUE8",
            BM_PON_EEE_QUEUE8_dft
        },
        {
            BM_PON_EEE_THRESHOLD1,
            "BM_PON_EEE_THRESHOLD1",
            BM_PON_EEE_THRESHOLD1_dft
        },
        {
            BM_PON_EEE_THRESHOLD2,
            "BM_PON_EEE_THRESHOLD2",
            BM_PON_EEE_THRESHOLD2_dft
        },
        {
            BM_PON_EEE_THRESHOLD3,
            "BM_PON_EEE_THRESHOLD3",
            BM_PON_EEE_THRESHOLD3_dft
        },
        {
            BM_PON_EEE_THRESHOLD3,
            "BM_PON_EEE_THRESHOLD3",
            BM_PON_EEE_THRESHOLD3_dft
        },

    };

    static cs_uint32 size = sizeof(mpcp_eee_setting) / sizeof(__register_dump_t);

    cs_uint32 addr = 0;
    int i;
    diag_printf("                                        register-name       default         current         address \n");


    for (i = 0; i < size; i++) {
        cs_uint32 valu32;
        if (i % 10 == 0) {
//            cyg_thread_delay(10);
            //HAL_DELAY_US(1000);
            cs_thread_delay(10);
        }
        addr = LYNXD_REG_BASE + mpcp_eee_setting[i].offset * 4;

        REG_READ(mpcp_eee_setting[i].offset, valu32);

        diag_printf("%52s:      0x%08x     0x%08x     0x%08x\n", mpcp_eee_setting[i].name, mpcp_eee_setting[i].default_value, valu32, addr);
    }
}
void do_dump_bm_counter(void)
{

    __counter_entry_t bm_counter[] = {
        { BM_GE_RX_PKT_CNTR, "BM_GE_RX_PKT_CNTR" },
        { BM_PON_RX_PKT_CNTR, "BM_PON_RX_PKT_CNTR" },
        { BM_CPU_RX_PKT_CNTR, "BM_CPU_RX_PKT_CNTR" },
        { BM_GE_TX_PKT_CNTR, "BM_GE_TX_PKT_CNTR" },
        { BM_PON_TX_PKT_CNTR, "BM_PON_TX_PKT_CNTR" },
        { BM_CPU_TX_PKT_CNTR, "BM_CPU_TX_PKT_CNTR" },
        { BM_GE_DROP_PKT_CNTR, "BM_GE_DROP_PKT_CNTR" },
        { BM_PON_DROP_PKT_CNTR, "BM_PON_DROP_PKT_CNTR" },
        { BM_CPU_DROP_PKT_CNTR, "BM_CPU_DROP_PKT_CNTR" },
        { BM_GE_FE_DROP_PKT_CNTR, "BM_GE_FE_DROP_PKT_CNTR" },
        { BM_PON_FE_DROP_PKT_CNTR, "BM_PON_FE_DROP_PKT_CNTR" },
        { BM_CPU_FE_DROP_PKT_CNTR, "BM_CPU_FE_DROP_PKT_CNTR" },
        { BM_GE_ABR_DROP_PKT_CNTR, "BM_GE_ABR_DROP_PKT_CNTR" },
        { BM_PON_ABR_DROP_PKT_CNTR, "BM_PON_ABR_DROP_PKT_CNTR" },
        { BM_MPCP_DROP_PKT_CNTR, "BM_MPCP_DROP_PKT_CNTR" },
        { BM_SHARED_BUFFER_CNTR, "BM_SHARED_BUFFER_CNTR" },
        { BM_GE_TX_BUFFER_CNTR, "BM_GE_TX_BUFFER_CNTR" },
        { BM_PON_TX_BUFFER_CNTR, "BM_PON_TX_BUFFER_CNTR" },
        { BM_CPU_TX_BUFFER_CNTR, "BM_CPU_TX_BUFFER_CNTR" },
        { BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS8, "BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS8" },
        { BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS7, "BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS7" },
        { BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS6, "BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS6" },
        { BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS5, "BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS5" },
        { BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS4, "BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS4" },
        { BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS3, "BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS3" },
        { BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS2, "BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS2" },
        { BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS1, "BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS1" },
        { BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS0, "BM_GE_TX_PER_QUEUE_BUFFER_CNTR_COS0" },
        { BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS8, "BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS8" },
        { BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS7, "BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS7" },
        { BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS6, "BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS6" },
        { BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS5, "BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS5" },
        { BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS4, "BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS4" },
        { BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS3, "BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS3" },
        { BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS2, "BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS2" },
        { BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS1, "BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS1" },
        { BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS0, "BM_PON_TX_PER_QUEUE_BUFFER_CNTR_COS0" },
        { BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS8, "BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS8" },
        { BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS7, "BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS7" },
        { BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS6, "BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS6" },
        { BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS5, "BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS5" },
        { BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS4, "BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS4" },
        { BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS3, "BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS3" },
        { BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS2, "BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS2" },
        { BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS1, "BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS1" },
        { BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS0, "BM_CPU_TX_PER_QUEUE_BUFFER_CNTR_COS0" },
        // { BM_MPCP_FIFO_STS, "BM_MPCP_FIFO_STS" },
        { BM_GE_HD_ERR_CNTR, "BM_GE_HD_ERR_CNTR" },
        { BM_PON_HD_ERR_CNTR, "BM_PON_HD_ERR_CNTR" },
        { BM_CPU_HD_ERR_CNTR, "BM_CPU_HD_ERR_CNTR" },
        {BM_RMU_PT0_DROP_CNT1, "BM_RMU_PT0_DROP_CNT1"},
        {BM_RMU_PT0_DROP_CNT2, "BM_RMU_PT0_DROP_CNT2"},
        {BM_RMU_PT0_DROP_CNT3, "BM_RMU_PT0_DROP_CNT3"},
        {BM_RMU_PT1_DROP_CNT1, "BM_RMU_PT1_DROP_CNT1"},
        {BM_RMU_PT1_DROP_CNT2, "BM_RMU_PT1_DROP_CNT2"},
        {BM_RMU_PT1_DROP_CNT3, "BM_RMU_PT1_DROP_CNT3"},
        {BM_RMU_PT2_DROP_CNT1, "BM_RMU_PT2_DROP_CNT1"},
        {BM_RMU_PT2_DROP_CNT2, "BM_RMU_PT2_DROP_CNT2"},
        {BM_RMU_PT2_DROP_CNT3, "BM_RMU_PT2_DROP_CNT3"},
        {BM_LYNXD_TMU_STATUS, "BM_TMU_STATUS"},
        {BM_P1_Q0_START, "BM_P1_Q0_START"},
        {BM_P1_Q0_END, "BM_P1_Q0_END"},
        {BM_P1_Q1_START, "BM_P1_Q1_START"},
        {BM_P1_Q1_END, "BM_P1_Q1_END"},
        {BM_P1_Q2_START, "BM_P1_Q2_START"},
        {BM_P1_Q2_END, "BM_P1_Q2_END"},
        {BM_P1_Q3_START, "BM_P1_Q3_START"},
        {BM_P1_Q3_END, "BM_P1_Q3_END"},
        {BM_P1_Q4_START, "BM_P1_Q4_START"},
        {BM_P1_Q4_END, "BM_P1_Q4_END"},
        {BM_GRANT_MONITOR, "BM_GRANT_MONITOR"},
        {BM_EEE_STAT, "BM_EEE_STAT"},
        {BM_PON_EEE_STAT, "BM_PON_EEE_STAT"},
        {BM_DP_STAGE_MONITOR, "BM_DP_STAGE_MONITOR"},
        {BM_GE_DP_MONITOR1_0, "BM_GE_DP_MONITOR1_0"},
        {BM_GE_DP_MONITOR3_2, "BM_GE_DP_MONITOR3_2"},
        {BM_GE_DP_MONITOR5_4, "BM_GE_DP_MONITOR5_4"},
        {BM_GE_DP_MONITOR7_6, "BM_GE_DP_MONITOR7_6"},
        {BM_PON_DP_MONITOR1_0, "BM_PON_DP_MONITOR1_0"},
        {BM_PON_DP_MONITOR3_2, "BM_PON_DP_MONITOR3_2"},
        {BM_PON_DP_MONITOR5_4, "BM_PON_DP_MONITOR5_4"},
        {BM_PON_DP_MONITOR7_6, "BM_PON_DP_MONITOR7_6"},
        {BM_GE_ABR_DROP_COS7_PKT_CNTR, "BM_GE_ABR_DROP_COS7_PKT_CNTR"},
        {BM_PON_ABR_DROP_COS7_PKT_CNTR, "BM_PON_ABR_DROP_COS7_PKT_CNTR"},
        { BM_GE_PACKET_HEADER_INFO, "BM_GE_PACKET_HEADER_INFO" },
        { BM_PON_PACKET_HEADER_INFO, "BM_PON_PACKET_HEADER_INFO" },
        { BM_CPU_PACKET_HEADER_INFO, "BM_CPU_PACKET_HEADER_INFO" },
        { FE_LE_PE_CAPTURE, "FE_LE_PE_CAPTURE_GE" },
        { FE_LE_PE_CAPTURE + FE_LE_STRIDE, "FE_LE_PE_CAPTURE_PON" },
        { FE_LE_PE_CAPTURE + 2*FE_LE_STRIDE, "FE_LE_PE_CAPTURE_CPU" },
    };

    static cs_uint32 size = sizeof(bm_counter) / sizeof(__counter_entry_t);

    cs_uint32 addr = 0;
    int i;

    for (i = 0; i < size; i++) {
        cs_uint32 cnt;

        if (i % 10 == 0) {
            //HAL_DELAY_US(1000);
            cs_thread_delay(10);
        }
        addr = LYNXD_REG_BASE + bm_counter[i].offset * 4;

        REG_READ(bm_counter[i].offset, cnt);


        diag_printf("%s:0x%08x\n", bm_counter[i].name, cnt);

        if ((BM_GE_PACKET_HEADER_INFO == bm_counter[i].offset || BM_PON_PACKET_HEADER_INFO == bm_counter[i].offset
                || BM_CPU_PACKET_HEADER_INFO == bm_counter[i].offset) && (cnt != 0)) {
            cs_uint32 packet_type, packet_length;
            cs_uint8 dpid, cos, ptp_flag, mir_flag, grp_id, spid;
            ptp_flag = (cnt >> 31)&1;
            diag_printf("PTP_FLAG       :   %d\n", ptp_flag);
            mir_flag = (cnt>>30)&1;
            diag_printf("MIR_FLAG       :   %d\n", mir_flag);         
            grp_id   = (cnt>>25)&0x1f;
            diag_printf("GROUP_ID       :   %d\n", grp_id);                    
            spid     = (cnt>>22)&7;
            diag_printf("SOURCE_ID      :   %d\n", spid);                                
            packet_type = cnt >> 17 & 0x1f;
            
            diag_printf("packet type    :   %s\n", __dump_packet_type[packet_type].name);
            dpid = (cnt >> 14) & 7;
            cos = (cnt >> 11) & 7;
            packet_length = (cnt & 0x7ff);
            diag_printf("DPID           : %d\n", dpid);
            diag_printf("COS            : %d\n", cos);
            diag_printf("packet length  : %d\n", packet_length);

        }
        if (FE_LE_PE_CAPTURE == bm_counter[i].offset  || FE_LE_PE_CAPTURE + FE_LE_STRIDE == bm_counter[i].offset
                || FE_LE_PE_CAPTURE + 2*FE_LE_STRIDE == bm_counter[i].offset) {
            if ((cnt >> 5)&1) {
                switch (cnt&0x1f) {
                case 0:
                    diag_printf("special_pkt_permit = 0\n");
                    break;
                case 2:
                    diag_printf("tpid_drop_flag = 1\n");
                    break;
                case 3:
                    diag_printf("vlan_check_permit = 0\n");
                    break;
                case 4:
                    diag_printf("classification permit =0\n");
                    break;
                case 5:
                    diag_printf("classification DPID = 3 or mirror\n");
                    break;
                case 7:
                    diag_printf(" l2_permit = 0\n");
                    break;
                case 16:
                    diag_printf("SA limit drop\n");
                    break;
                case 17:
                    diag_printf("sa_permit_drop\n");
                    break;
                case 18:
                    diag_printf("egr_vlnchk_drop\n");
                    break;
                case 19:
                    diag_printf("loop drop\n");
                    break;
                case 20:
                    diag_printf("block forwarding drop\n");
                    break;
                case 28:
                    diag_printf("egress runt packet drop\n");
                    break;
                case 29:
                    diag_printf("TE drop\n");
                    break;
                case 30:
                    diag_printf(" rx_mac drop\n");
                    break;
                case 31:
                    diag_printf(" other drop\n");
                    break;
                default:
                    diag_printf("no design\n");
                    break;
                }
            }
        }

    }
    return;
}


#if 1
void aal_cmd_set_max_que_len(int argc, char **argv)
{
    cs_aal_port_id_t port;
    cs_aal_bm_queue_msk_t mask;
    cs_aal_bm_queue_val_t cfg;
    cs_uint8 index;
    cs_status rt;

    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    mask.u32 = iros_strtol(argv[5]);
    for(index = 0; index<8; index++){
        if((mask.u32>>index)&1){
            break;
        }
    }
    if(index == 8){
        return;
    }else{
        cfg.val[index] = iros_strtol(argv[6]);
    }

    rt = aal_bm_share_queue_max_length_set(port, mask, &cfg);
    diag_printf("configure return %x\n", rt);
}

void aal_cmd_get_max_que_len(int argc, char **argv)
{
    cs_aal_port_id_t port;
    cs_aal_bm_queue_val_t cfg;
    cs_status rt;
    cs_uint8 index;

    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    rt = aal_bm_share_queue_max_length_get(port, &cfg);

    diag_printf("configure return %x\n", rt);  
    for(index=0; index<8; index++){
        diag_printf("q%x size = %x\n",index, cfg.val[index]);
    }

}

void aal_cmd_set_schd(int argc, char **argv)
{
    cs_aal_port_id_t port;
    cs_aal_bm_weight_t weight;
    cs_uint8 index;
    cs_status rt;
    
    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    weight.mode = (cs_aal_bm_sched_type_t)iros_strtol(argv[5]);
    for(index= 0; index<8; index++){
        weight.weight[index]= iros_strtol(argv[6+index]);
    }
    
    rt = aal_bm_share_scheduler_set(port, &weight);
    diag_printf("configure return %x\n", rt);  
}


void aal_cmd_get_schd(int argc, char **argv)
{
    cs_aal_port_id_t port;
    cs_aal_bm_weight_t weight;
    cs_uint8 index;
    cs_status rt;
    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    
    rt = aal_bm_share_scheduler_get(port, &weight);

    diag_printf("configure return %x\n", rt);  
    
    for(index= 0; index<8; index++){
        diag_printf("que%x weight %d\n",index,weight.weight[index]);
    }
}
extern cs_status __bm_reset(void);
void aal_cmd_bm_reset(void)
{
    __bm_reset();

}
void aal_bm_cmd(int argc, char **argv)
{
    if (argc == 2) {
        diag_printf("\n aal bm max_qlen set [msk][length]\n");
        diag_printf("\n aal bm max_qlen get [msk][length]\n");

        diag_printf("\n aal bm sche set [port][mod][w0][w1][w2][w3][w4][w5][w6][w7]\n");
        diag_printf("\n aal bm sche get [port]\n");
        
        diag_printf("\n aal bm reg\n");
        diag_printf("\n aal bm mpcp\n");
                  
        diag_printf("\n aal bm reset\n");        
    }
    else{
        if (cmd_cmp(argv[2], "max_qlen")) {
            if (cmd_cmp(argv[3], "set")) {
                return aal_cmd_set_max_que_len(argc, argv);
            }else if(cmd_cmp(argv[3], "get")) {
                return aal_cmd_get_max_que_len(argc, argv);
            }
        }
        if (cmd_cmp(argv[2], "sche")) {
            if (cmd_cmp(argv[3], "set")) {
                return aal_cmd_set_schd(argc, argv);
            }else if(cmd_cmp(argv[3], "get")) {
                return aal_cmd_get_schd(argc, argv);
            }
        }
        
        if (cmd_cmp(argv[2], "reg")) {
            return do_dump_bm_register();     
        }

        if (cmd_cmp(argv[2], "mpcp")) {
            return do_dump_mpcp_eee_register();     
        }
        if (cmd_cmp(argv[2], "reset")) {  
            aal_cmd_bm_reset();         
        }        
    }
    
}
#endif
