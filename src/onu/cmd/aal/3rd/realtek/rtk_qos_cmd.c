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

#include "rtk_api_ext.h"
#include "plat_common.h"
#include "cs_utils.h"

extern int diag_printf(const char *fmt, ...);

#define __RTK_QOS_PRINT_CMD_HELP(func,cmd,help)  diag_printf("%s %s %-20s --- %s\n","sw",(func),(cmd),(help))

void __sw_cmd_rtk_meter_rate_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_meter_id_t meter_id;
    rtk_rate_t rate;
    rtk_enable_t ifg;
    
    meter_id = (rtk_meter_id_t)iros_strtol(argv[0]);
    rate = (rtk_rate_t)iros_strtol(argv[1]);
    ifg = (rtk_enable_t)iros_strtol(argv[2]);
    
    rt = rtk_rate_shareMeter_set(meter_id,rate,ifg);
    diag_printf("configure return %x\n", rt);     
}

void __sw_cmd_rtk_meter_rate_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_meter_id_t meter_id;
    rtk_rate_t rate;
    rtk_enable_t ifg;
 
    meter_id = (rtk_meter_id_t)iros_strtol(argv[0]); 
    rt = rtk_rate_shareMeter_get(meter_id,&rate,&ifg);    
    diag_printf("configure return %x\n", rt);     
    diag_printf("meter id %d rate %d ifg %s\n", meter_id, rate, ifg ? "enable":"disable");
}

void __sw_cmd_rtk_ingress_rate_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_rate_t rate;
    rtk_enable_t ifg, fc_en;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);
    rate = (rtk_rate_t)iros_strtol(argv[1]);
    ifg = (rtk_enable_t)iros_strtol(argv[2]);
    fc_en = (rtk_enable_t)iros_strtol(argv[3]);    
    rt = rtk_rate_igrBandwidthCtrlRate_set(port_id,rate,ifg,fc_en);
    diag_printf("configure return %x\n", rt);     
}

void __sw_cmd_rtk_ingress_rate_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_rate_t rate;
    rtk_enable_t ifg, fc_en;

    for(port_id = 0; port_id <4; port_id++){
        rt = rtk_rate_igrBandwidthCtrlRate_get(port_id,&rate,&ifg,&fc_en);
        diag_printf("port id %d rate %d ifg %s flow control %s\n", port_id, rate, ifg ? "enable":"disable", fc_en ? "enable":"disable");
    }
}

void __sw_cmd_rtk_egress_rate_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_rate_t rate;
    rtk_enable_t ifg;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);
    rate = (rtk_rate_t)iros_strtol(argv[1]);
    ifg = (rtk_enable_t)iros_strtol(argv[2]);
    
    rt = rtk_rate_egrBandwidthCtrlRate_set(port_id,rate,ifg);
    diag_printf("configure return %x\n", rt);     
}

void __sw_cmd_rtk_egress_rate_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_rate_t rate;
    rtk_enable_t ifg;
    for(port_id = 0; port_id <4; port_id++){
        rt = rtk_rate_egrBandwidthCtrlRate_get(port_id,&rate,&ifg);
        diag_printf("port id %d rate %d ifg %s\n", port_id, rate, ifg ? "enable":"disable");     
    }
}

void __sw_cmd_rtk_egress_queue_bw_ctrl_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_qid_t que_num;
    rtk_enable_t en;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);
    que_num = RTK_WHOLE_SYSTEM;
    en = (rtk_enable_t)iros_strtol(argv[1]);
    rt = rtk_rate_egrQueueBwCtrlEnable_set(port_id,que_num,en);
    diag_printf("configure return %x\n", rt);     
}

void __sw_cmd_rtk_egress_queue_bw_ctrl_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_enable_t en;

    for (port_id = 0; port_id < 8; port_id++) {
        rt = rtk_rate_egrQueueBwCtrlEnable_get(port_id,RTK_WHOLE_SYSTEM,&en);
        diag_printf("port id %d  egress queue bw ctrl %s\n", port_id, en ? "enable":"disable");     
    }
}
void __sw_cmd_rtk_egress_queue_bw_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_qid_t que_id;
    rtk_meter_id_t meter_id;

    port_id = (rtk_port_t)iros_strtol(argv[0]);
    que_id = iros_strtol(argv[1]);
    meter_id = iros_strtol(argv[2]);
    rt = rtk_rate_egrQueueBwCtrlRate_set(port_id, que_id, meter_id);
    diag_printf("configure return %x\n", rt);
}

void __sw_cmd_rtk_egress_queue_bw_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_qid_t que_id;
    rtk_meter_id_t meter_id;
    port_id = (rtk_port_t)iros_strtol(argv[0]);
    que_id = iros_strtol(argv[1]);
    rt = rtk_rate_egrQueueBwCtrlRate_get(port_id, que_id, &meter_id);
    diag_printf("configure return %x\n", rt);
    diag_printf("meter id =  %d\n", meter_id);

}

void __sw_cmd_rtk_port_storm_ctrl_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_rate_storm_group_t storm_type;    
    rtk_rate_t rate;
    rtk_enable_t ifg;
    rtk_mode_t mode; 
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);
    storm_type = (rtk_rate_storm_group_t)iros_strtol(argv[1]);
    rate = (rtk_rate_t)iros_strtol(argv[2]);
    ifg = (rtk_enable_t)iros_strtol(argv[3]);
    mode = (rtk_mode_t)iros_strtol(argv[4]);
    rt = rtk_storm_controlRate_set(port_id,storm_type,rate,ifg,mode);
    diag_printf("configure return %x\n", rt);     
}

void __sw_cmd_rtk_port_storm_ctrl_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
 //   rtk_rate_storm_group_t storm_type;    
    rtk_rate_t rate;
    rtk_enable_t ifg;
    rtk_mode_t mode;
    mode = (rtk_mode_t)iros_strtol(argv[0]);

    for(port_id = 0; port_id <4; port_id++){
        rt = rtk_storm_controlRate_get(port_id,STORM_GROUP_UNKNOWN_UNICAST,&rate,&ifg,mode);
        diag_printf("port id %d  STORM_GROUP_UNKNOWN_UNICAST rate %d, ifg %s\n", port_id, rate,ifg ? "enable":"disable");     
        rt = rtk_storm_controlRate_get(port_id,STORM_GROUP_UNKNOWN_MULTICAST,&rate,&ifg,mode);
        diag_printf("port id %d  STORM_GROUP_UNKNOWN_MULTICAST rate %d, ifg %s\n", port_id, rate,ifg ? "enable":"disable");     
        rt = rtk_storm_controlRate_get(port_id,STORM_GROUP_MULTICAST,&rate,&ifg,mode);
        diag_printf("port id %d  STORM_GROUP_MULTICAST rate %d, ifg %s\n", port_id, rate,ifg ? "enable":"disable");     
        rt = rtk_storm_controlRate_get(port_id,STORM_GROUP_BROADCAST,&rate,&ifg,mode);
        diag_printf("port id %d  STORM_GROUP_BROADCAST rate %d, ifg %s\n\n", port_id, rate,ifg ? "enable":"disable");     
    }
}


void __sw_cmd_rtk_qos_init(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_queue_num_t que_num;
    que_num = (rtk_port_t)iros_strtol(argv[0]);
    rt = rtk_qos_init(que_num);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_qos_pri_slt_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_priority_select_t pri_scl;
    
    pri_scl.port_pri = (rtk_uint32)iros_strtol(argv[0]);    
    pri_scl.dot1q_pri = (rtk_uint32)iros_strtol(argv[1]);
    pri_scl.acl_pri = (rtk_uint32)iros_strtol(argv[2]);
    pri_scl.dscp_pri = (rtk_uint32)iros_strtol(argv[3]);
    pri_scl.cvlan_pri = (rtk_uint32)iros_strtol(argv[4]);
    pri_scl.svlan_pri = (rtk_uint32)iros_strtol(argv[5]);
    pri_scl.dmac_pri = (rtk_uint32)iros_strtol(argv[6]);
    pri_scl.smac_pri = (rtk_uint32)iros_strtol(argv[7]);
    rt = rtk_qos_priSel_set(&pri_scl);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_qos_pri_slt_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_priority_select_t pri_scl;
    rt = rtk_qos_priSel_get(&pri_scl);    
    diag_printf("configure return %x\n", rt);
    diag_printf("port_pri = %d\n", pri_scl.port_pri);
    diag_printf("dot1q_pri = %d\n", pri_scl.dot1q_pri);
    diag_printf("acl_pri = %d\n", pri_scl.acl_pri);
    diag_printf("dscp_pri = %d\n", pri_scl.dscp_pri);
    diag_printf("cvlan_pri = %d\n", pri_scl.cvlan_pri);
    diag_printf("svlan_pri = %d\n", pri_scl.svlan_pri);
    diag_printf("dmac_pri = %d\n", pri_scl.dmac_pri);
    diag_printf("smac_pri = %d\n", pri_scl.smac_pri);    
}

void __sw_cmd_rtk_qos_dot1p_map_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_pri_t ing_1p,egr_1p ;
    
    ing_1p = (rtk_pri_t)iros_strtol(argv[0]);    
    egr_1p = (rtk_pri_t)iros_strtol(argv[1]);    
 
    rt = rtk_qos_1pPriRemap_set(ing_1p,egr_1p);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_qos_dot1p_map_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_pri_t ing_1p,egr_1p ;
    
    ing_1p = (rtk_pri_t)iros_strtol(argv[0]);    
    rt = rtk_qos_1pPriRemap_get(ing_1p,&egr_1p);    
    diag_printf("configure return %x\n", rt);  
    diag_printf("8021p pri value = %d, internal pri %d\n", ing_1p,egr_1p);
}

void __sw_cmd_rtk_qos_dscp_map_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_pri_t ing_dscp,egr_1p ;
    
    ing_dscp = (rtk_pri_t)iros_strtol(argv[0]);    
    egr_1p = (rtk_pri_t)iros_strtol(argv[1]);    
 
    rt = rtk_qos_dscpPriRemap_set(ing_dscp,egr_1p);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_qos_dscp_map_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_pri_t ing_dscp,egr_1p ;
    
    ing_dscp = (rtk_pri_t)iros_strtol(argv[0]);    
    rt = rtk_qos_dscpPriRemap_get(ing_dscp,&egr_1p);    
    diag_printf("configure return %x\n", rt);  
    diag_printf("dscp value = %d, internal pri %d\n", ing_dscp,egr_1p);
}

void __sw_cmd_rtk_qos_port_pri_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_pri_t egr_1p ;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    egr_1p = (rtk_pri_t)iros_strtol(argv[1]);    
 
    rt = rtk_qos_portPri_set(port_id,egr_1p);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_qos_port_pri_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_pri_t egr_1p ;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    rt = rtk_qos_portPri_get(port_id,&egr_1p);    
    diag_printf("configure return %x\n", rt);  
    diag_printf("port = %d, internal pri %d\n", port_id,egr_1p);
}

void __sw_cmd_rtk_qos_que_num_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_queue_num_t que_num;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    que_num = (rtk_queue_num_t)iros_strtol(argv[1]);    
 
    rt = rtk_qos_queueNum_set(port_id,que_num);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_qos_que_num_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_queue_num_t que_num;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    rt = rtk_qos_queueNum_get(port_id,&que_num);    
    diag_printf("configure return %x\n", rt);  
    diag_printf("port = %d, queue number %d\n", port_id,que_num);
}

void __sw_cmd_rtk_qos_pri_map_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_queue_num_t que_num;
    rtk_qos_pri2queue_t pri2que;
    rtk_uint32 index;
    
    que_num = (rtk_queue_num_t)iros_strtol(argv[0]);    
    for(index = 0; index < RTK_MAX_NUM_OF_PRIORITY; index++){
        pri2que.pri2queue[index] = iros_strtol(argv[1+index]);
    }

    rt = rtk_qos_priMap_set(que_num,&pri2que);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_qos_pri_map_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_queue_num_t que_num;
    rtk_qos_pri2queue_t pri2que;
    rtk_uint32 index;

    
    que_num = (rtk_queue_num_t)iros_strtol(argv[0]);    
    rt = rtk_qos_priMap_get(que_num,&pri2que);    
    diag_printf("configure return %x\n", rt);  
    for(index = 0; index < RTK_MAX_NUM_OF_PRIORITY; index++){
        diag_printf("priority [%d] queue %d\n", index,pri2que.pri2queue[index]);
    }
}

void __sw_cmd_rtk_qos_que_schd_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_qos_queue_weights_t que_weight;
    rtk_uint32 index;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    for(index = 0; index < RTK_MAX_NUM_OF_PRIORITY; index++){
        que_weight.weights[index] = iros_strtol(argv[1+index]);
    }

    rt = rtk_qos_schedulingQueue_set(port_id,&que_weight);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_qos_que_schd_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_qos_queue_weights_t que_weight;
    rtk_uint32 index;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    rt = rtk_qos_schedulingQueue_get(port_id,&que_weight);    
    diag_printf("configure return %x\n", rt);  
    for(index = 0; index < RTK_MAX_NUM_OF_PRIORITY; index++){
        diag_printf("queue [%d] weight %d\n", index,que_weight.weights[index]);
    }
}


void __sw_cmd_rtk_qos_1p_remark_en_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_enable_t enable;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    enable = (rtk_enable_t)iros_strtol(argv[1]); 

    rt = rtk_qos_1pRemarkEnable_set(port_id,enable);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_qos_1p_remark_en_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_enable_t enable;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    rt = rtk_qos_1pRemarkEnable_get(port_id,&enable);    
    diag_printf("configure return %x\n", rt);    
    diag_printf("%s\n", enable ? "enable":"disable");
}

void __sw_cmd_rtk_qos_1p_remark_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_pri_t int_pri,dot1p_pri;
    
    int_pri = (rtk_pri_t)iros_strtol(argv[0]);    
    dot1p_pri = (rtk_pri_t)iros_strtol(argv[1]); 

    rt = rtk_qos_1pRemark_set(int_pri,dot1p_pri);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_qos_1p_remark_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_pri_t int_pri,dot1p_pri;
    
    int_pri = (rtk_pri_t)iros_strtol(argv[0]);    
    rt = rtk_qos_1pRemark_get(int_pri,&dot1p_pri);
    
    diag_printf("configure return %x\n", rt);    
    diag_printf("int_pri %d, dot1p_pri %d\n",int_pri,dot1p_pri);
}

void __sw_cmd_rtk_qos_dscp_remark_en_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_enable_t enable;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    enable = (rtk_enable_t)iros_strtol(argv[1]); 

    rt = rtk_qos_dscpRemarkEnable_set(port_id,enable);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_qos_dscp_remark_en_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_port_t port_id;
    rtk_enable_t enable;
    
    port_id = (rtk_port_t)iros_strtol(argv[0]);    
    rt = rtk_qos_dscpRemarkEnable_get(port_id,&enable);    
    diag_printf("configure return %x\n", rt);    
    diag_printf(" %s\n", enable ? "enable":"disable");
}

void __sw_cmd_rtk_qos_dscp_remark_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_pri_t int_pri;
    rtk_dscp_t dscp;
    
    int_pri = (rtk_pri_t)iros_strtol(argv[0]);    
    dscp = (rtk_dscp_t)iros_strtol(argv[1]); 

    rt = rtk_qos_dscpRemark_set(int_pri,dscp);    
    diag_printf("configure return %x\n", rt);  
}

void __sw_cmd_rtk_qos_dscp_remark_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_pri_t int_pri;
    rtk_dscp_t dscp;
    
    int_pri = (rtk_pri_t)iros_strtol(argv[0]);    
    rt = rtk_qos_dscpRemark_get(int_pri,&dscp);
    
    diag_printf("configure return %x\n", rt);    
    diag_printf("int_pri %d, dscp %d\n", int_pri,dscp);
}

void __sw_cmd_rtk_qos_meter_burst_set(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_meter_id_t index;
    rtk_uint32 size;
    
    index = (rtk_meter_id_t)iros_strtol(argv[0]);  
    size = iros_strtol(argv[1]);  
    
    rt = rtk_rate_shareMeterBucket_set(index,size);
    
    diag_printf("configure return %x\n", rt);    
}

void __sw_cmd_rtk_qos_meter_burst_get(int argc, char **argv)
{
    rtk_api_ret_t rt;
    rtk_meter_id_t index;
    rtk_uint32 size;
    
    index = (rtk_meter_id_t)iros_strtol(argv[0]);  
    
    rt = rtk_rate_shareMeterBucket_get(index,&size);
    
    diag_printf("configure return %x\n", rt);   
    diag_printf("bucket size = %d\n",size);
}

typedef void (*rtk_qos_cmd_func_t)(int, char **);

static struct {
    cs_uint8 *name;
    rtk_qos_cmd_func_t      exe_func;
    int                     argc;
    cs_uint8                *para;
    cs_uint8                *help;
    }rtk_qos_cmds[] = {
    {"mrate-set" , __sw_cmd_rtk_meter_rate_set, 3, "meter_id(0~63), rate(8~1000000),[ifg]0-disable 1-enable","set merter rate"},
    {"mrate-get" , __sw_cmd_rtk_meter_rate_get, 1 , "meter_id(0)", "get meter rate"},
    {"prate-set" , __sw_cmd_rtk_ingress_rate_set, 4 , "port_id(0~3), rate(8~1000000),[ifg]0-disable 1-enable,[fc]0-disable 1-enable","set port ingress rate"},
    {"prate-get" , __sw_cmd_rtk_ingress_rate_get, 0 , "cr","get port ingress rate"},
    {"erate-set" , __sw_cmd_rtk_egress_rate_set, 3 , "port_id(0~3), rate(8~1000000),[ifg]0-disable 1-enable","set port egress rate"},
    {"erate-get" , __sw_cmd_rtk_egress_rate_get, 0 , "cr","get port egress rate"},
    {"ebwctr-set" , __sw_cmd_rtk_egress_queue_bw_ctrl_set, 2 , "port_id(0~3), [enable]0-disable 1-enable","set port egress bw ctrl"},
    {"ebwctr-get" , __sw_cmd_rtk_egress_queue_bw_ctrl_get, 0 , "cr","get port egress bw ctrl"},
    {"equebw-set" , __sw_cmd_rtk_egress_queue_bw_set, 3 , "[port][que][meter]", "set port que egress bw"},
    {"equebw-get" , __sw_cmd_rtk_egress_queue_bw_get, 2 , "[port][que]", "get port que egress bw"},
    {"storm-set" , __sw_cmd_rtk_port_storm_ctrl_set, 5 , "port_id(0~3),[storm_type]0-UUC 1-UMC 2-MC 3-BC,rate(8~1000000),[ifg]0-disable 1-enable, [mode]","set port egress rate"},
    {"storm-get" , __sw_cmd_rtk_port_storm_ctrl_get, 1 , "[mode]","get storm control"},
    {"init" , __sw_cmd_rtk_qos_init, 1 , "[queue number]","qos init"},
    {"prislt-set" , __sw_cmd_rtk_qos_pri_slt_set, 8 , "[port][dot1p][acl][dscp][cvlan][svlan][dmac][smac]","set pri select"},
    {"prislt-get" , __sw_cmd_rtk_qos_pri_slt_get, 0 , "cr","get pri select"},
    {"1premap-set" , __sw_cmd_rtk_qos_dot1p_map_set, 2 , "[in 1p][out 1p]","8021p remark"},
    {"1premap-get" , __sw_cmd_rtk_qos_dot1p_map_get, 1 , "[in 1p]","get 8021p remark"},
    {"dscpremap-set" , __sw_cmd_rtk_qos_dscp_map_set, 2 , "[in dscp][out 1p]","dscp remark"},
    {"dscpremap-get" , __sw_cmd_rtk_qos_dscp_map_get, 1 , "[in dscp]","get dscp remark"},    
    {"portpri-set" , __sw_cmd_rtk_qos_port_pri_set, 2 , "[port][pri]","port default priority"},
    {"portpri-get" , __sw_cmd_rtk_qos_port_pri_get, 1 , "[port]","get port default priority"},
    {"quenum-set" , __sw_cmd_rtk_qos_que_num_set, 2 , "[port id][queue_num]","queue num"},
    {"quenum-get" , __sw_cmd_rtk_qos_que_num_get, 1 , "[port id]","queue num"},
    {"quemap-set" , __sw_cmd_rtk_qos_pri_map_set, 9 , "[port id][pri0->que][pri1->que][pri2->que]...","queue mapping"},
    {"quemap-get" , __sw_cmd_rtk_qos_pri_map_get, 1 , "[port id]","get queue mapping"},
    {"quewgt-set" , __sw_cmd_rtk_qos_que_schd_set, 9 , "[port id][que0 weight][que1 wei][que wei]...","queue weight"},
    {"quewgt-get" , __sw_cmd_rtk_qos_que_schd_get, 1 , "[port id]","get queue weight"},
    {"1premarken-set" , __sw_cmd_rtk_qos_1p_remark_en_set, 2 , "[port id][remark enable]","set 8021p remark enable"},
    {"1premarken-get" , __sw_cmd_rtk_qos_1p_remark_en_get, 1 , "[port id]","get 8021p remark enable"},
    {"1premark-set" , __sw_cmd_rtk_qos_1p_remark_set, 2 , "[in 8021p][8021p]","set 8021p remark"},
    {"1premark-get" , __sw_cmd_rtk_qos_1p_remark_get, 1 , "[in 8021p]","get 8021p remark"},
    {"dscpremarken-set" , __sw_cmd_rtk_qos_dscp_remark_en_set, 2 , "[port id][remark enable]","set dscp remark enable"},
    {"dscpremarken-get" , __sw_cmd_rtk_qos_dscp_remark_en_get, 1 , "[port id]","get dscp remark enable"},  
    {"dscpremark1p-set" , __sw_cmd_rtk_qos_dscp_remark_set, 2 , "[in dscp][8021p]","set dscp remark"},
        {"dscpremark1p-get" , __sw_cmd_rtk_qos_dscp_remark_get, 1 , "[in dscp]","get dscp remark"},
        {"bucket-set" , __sw_cmd_rtk_qos_meter_burst_set, 2 , "[index][size]","set bucket size"},
        {"bucket-get" , __sw_cmd_rtk_qos_meter_burst_get, 1 , "[index]","get bucket size"},
    };

void __rtk_qos_cmd_help(void)
{
    int i;
    for (i = 0; i < sizeof(rtk_qos_cmds) / sizeof(rtk_qos_cmds[0]); i++) {  
        __RTK_QOS_PRINT_CMD_HELP("qos", rtk_qos_cmds[i].name, rtk_qos_cmds[i].help);    
    }
}

static rtk_qos_cmd_func_t  __rtk_qos_find_cmd(
    const char *cmd_str,
    int   argc
)
{
    int i = 0;

    if (NULL == cmd_str) {
        return NULL;
    }

    for (i = 0;
        i < (sizeof(rtk_qos_cmds)/ sizeof(rtk_qos_cmds[0]));
        ++i) {
        if (cmd_cmp(rtk_qos_cmds[i].name, cmd_str)){

            if ((argc - 3) == rtk_qos_cmds[i].argc) {
                return rtk_qos_cmds[i].exe_func;
            }
            else {
                diag_printf("Please input %d arguments for this cmd.\n", rtk_qos_cmds[i].argc);
                diag_printf("parameters:  %s\n", rtk_qos_cmds[i].para);
                return (NULL);
            }
        }
    }

    diag_printf("Not exist this cmd!\n");
    __rtk_qos_cmd_help();

    return (NULL);

}
    
 void rtk_qos_cmd(int argc, char **argv)
 {
    rtk_qos_cmd_func_t  p_cmd_func = NULL;

    if (argc > 2) { /**< the cmd format is : " aal fdb  ..."*/

        p_cmd_func = __rtk_qos_find_cmd(argv[2], argc);

    }
    else {
        __rtk_qos_cmd_help();
        return;
    }

    if (NULL == p_cmd_func) {
        return;
    }

    (*p_cmd_func)(argc - 3, &argv[3]);
    return;
    
}




