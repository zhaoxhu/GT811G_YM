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
#include "aal_flow.h"
//extern int diag_printf(const char *fmt, ...);
void aal_cmd_dotlp_map_set(int argc, char **argv)
{
    cs_aal_port_id_t port;
    cs_aal_flow_mapping_msk_t mask;
    cs_aal_flow_mapping_t cfg;

    cs_uint8 index;
    cs_status rt;

    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    mask.u = iros_strtol(argv[5]);
    memset(&cfg, 0, sizeof(cfg));
    for(index = 0; index<8; index++){
        cfg.dot1p.u |= (iros_strtol(argv[6+index]) << (3*index));
        cfg.cos.u |= (iros_strtol(argv[6+index]) << (3*index));
        if(index<5){
            cfg.dscp.dscp_tc0.u |= (iros_strtol(argv[6+index]) << (6*index));
            cfg.tc.dscp_tc0.u |= (iros_strtol(argv[6+index]) << (6*index));
        }else{
            cfg.dscp.dscp_tc1.u |= (iros_strtol(argv[6+index]) << (6*(index-5)));
            cfg.tc.dscp_tc1.u |= (iros_strtol(argv[6+index]) << (6*(index-5)));
        }
        
        if(index<6){
            cfg.flow_id.flow0.u |= (iros_strtol(argv[6+index]) << (5*index));
        }else{
            cfg.flow_id.flow1.u |= (iros_strtol(argv[6+index]) << (5*(index-6)));
        }
    } 

    rt = aal_flow_dot1p_mapping_set(port, &mask, &cfg);
    diag_printf("configure return %x\n", rt);
}

void aal_cmd_dotlp_map_get(int argc, char **argv)
{
    cs_aal_port_id_t port;
    cs_aal_flow_mapping_t cfg;
    cs_uint8 index;
    cs_status rt;

    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    memset(&cfg, 0, sizeof(cfg));
   

    rt = aal_flow_dot1p_mapping_get(port, &cfg);
    diag_printf("configure return %x\n", rt);

    diag_printf("\n8021p to 8021p mapping  ");
    for(index = 0; index<8; index++){
         diag_printf(" %d ", (cfg.dot1p.u>>(3*index))&7);
    }
    diag_printf("\n8021p to cos mapping  ");
    for(index = 0; index<8; index++){
         diag_printf(" %d ", (cfg.cos.u>>(3*index))&7);
    }
    
    diag_printf("\n8021p to flow id mapping  ");
    for(index = 0; index<6; index++){
         diag_printf(" %d ", (cfg.flow_id.flow0.u>>(5*index))&0x1f);
    }   
    for(index = 0; index<2; index++){
         diag_printf(" %d ", (cfg.flow_id.flow1.u>>(5*index))&0x1f);
    }
        
    diag_printf("\n8021p to dscp mapping  ");
    for(index = 0; index<5; index++){
         diag_printf(" %d ", (cfg.dscp.dscp_tc0.u>>(6*index))&0x3f);
    }    
    for(index = 0; index<3; index++){
         diag_printf(" %d ", (cfg.dscp.dscp_tc1.u>>(6*index))&0x3f);
    }
    
    diag_printf("\n");
}

void aal_cmd_dscp_map_set(int argc, char **argv)
{
    cs_aal_dscp_map_t msk,dscp_cfg[64];
    cs_aal_port_id_t port;
    cs_uint8 dscp;    
    cs_status rt = CS_E_OK;
    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    rt = aal_flow_dscp_mapping_get(port,dscp_cfg);
    dscp = iros_strtol(argv[5]); 
    if(argc == 6){
        diag_printf(" mask, 0x0001--- dscp to dscp mapping\n");
        diag_printf(" mask, 0x0040--- dscp to dot1p mapping\n");
        diag_printf(" mask, 0x0200--- dscp to cos mapping\n");
        diag_printf(" mask, 0x1000--- dscp to flow id mapping\n");
    }
    dscp &= 0x3f;
    msk.u = iros_strtol(argv[6]);
    if(argc == 7){
        diag_printf(" dscp, dscp to dscp mapping\n");      
    }
    dscp_cfg[dscp].s.dscp_map = iros_strtol(argv[7]);    
    if(argc == 8){
        diag_printf("dot1p, dscp to dot1p mapping\n");      
    }
    dscp_cfg[dscp].s.dot1p_map = iros_strtol(argv[8]);
    if(argc == 9){
        diag_printf("cos, dscp to cos mapping\n");      
    }
    dscp_cfg[dscp].s.cos_map = iros_strtol(argv[9]);
    
    if(argc == 10){
        diag_printf("flowid, dscp to flow id mapping\n");      
    }
    dscp_cfg[dscp].s.flowid_map = iros_strtol(argv[10]);
    rt |= aal_flow_dscp_mapping_set(port,&msk,dscp_cfg);
    diag_printf("configure return %x\n", rt);
    
}
void aal_cmd_dscp_map_get(int argc, char **argv)
{
    cs_aal_dscp_map_t dscp_cfg[64];
    cs_aal_port_id_t port;    
    cs_status rt = CS_E_OK;
    cs_uint8 dscp;
    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    rt = aal_flow_dscp_mapping_get(port,dscp_cfg);
    diag_printf("configure return %x\n", rt);
    for(dscp =0; dscp <64; dscp++){
        if(dscp %4 == 0){
            diag_printf("\n");
            //HAL_DELAY_US(1000);
            cs_thread_delay(10);
        }
        diag_printf("dscp           : %d \n", dscp);
        diag_printf("todscp map     : %d \n", dscp_cfg[dscp].s.dscp_map);
        diag_printf("todot1p map    : %d \n", dscp_cfg[dscp].s.dot1p_map);
        diag_printf("tocos map      : %d \n", dscp_cfg[dscp].s.cos_map);
        diag_printf("toflowid map   : %d \n", dscp_cfg[dscp].s.flowid_map);        
    }
}

void aal_cmd_tc_map_set(int argc, char **argv)
{
    cs_aal_dscp_map_t msk,dscp_cfg[64];
    cs_aal_port_id_t port;
    cs_uint8 dscp;    
    cs_status rt = CS_E_OK;
    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    rt = aal_flow_tc_mapping_get(port,dscp_cfg);
    dscp = iros_strtol(argv[5]); 
    if(argc == 6){
        diag_printf(" mask, 0x0001--- dscp to dscp mapping\n");
        diag_printf(" mask, 0x0040--- dscp to dot1p mapping\n");
        diag_printf(" mask, 0x0200--- dscp to cos mapping\n");
        diag_printf(" mask, 0x1000--- dscp to flow id mapping\n");
    }
    msk.u = iros_strtol(argv[6]);
    if(argc == 7){
        diag_printf(" tc, tc to dscp mapping\n");      
    }
    dscp_cfg[dscp].s.dscp_map = iros_strtol(argv[7]);    
    if(argc == 8){
        diag_printf("dot1p, tc to dot1p mapping\n");      
    }
    dscp_cfg[dscp].s.dot1p_map = iros_strtol(argv[8]);
    if(argc == 9){
        diag_printf("cos, tc to cos mapping\n");      
    }
    dscp_cfg[dscp].s.cos_map = iros_strtol(argv[9]);
    
    if(argc == 10){
        diag_printf("flowid, tc to flow id mapping\n");      
    }
    dscp_cfg[dscp].s.flowid_map = iros_strtol(argv[10]);

    rt = aal_flow_tc_mapping_set(port,&msk,dscp_cfg);
    diag_printf("configure return %x\n", rt);
    
}
void aal_cmd_tc_map_get(int argc, char **argv)
{
    cs_aal_dscp_map_t dscp_cfg[64];
    cs_aal_port_id_t port;    
    cs_status rt = CS_E_OK;
    cs_uint8 dscp;
    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    rt = aal_flow_tc_mapping_get(port,dscp_cfg);
    diag_printf("configure return %x\n", rt);
    for(dscp =0; dscp <64; dscp++){
        if(dscp %4 == 0){
            diag_printf("\n");
            //HAL_DELAY_US(1000);
            cs_thread_delay(10);
        }
        diag_printf("tc             : %d \n", dscp);
        diag_printf("totc map       : %d \n", dscp_cfg[dscp].s.dscp_map);
        diag_printf("todot1p map    : %d \n", dscp_cfg[dscp].s.dot1p_map);
        diag_printf("tocos map      : %d \n", dscp_cfg[dscp].s.cos_map);
        diag_printf("toflowid map   : %d \n", dscp_cfg[dscp].s.flowid_map);        
    }
}

void aal_cmd_cos_map_set(int argc, char **argv)
{
    cs_aal_port_id_t port;
    cs_aal_flowid_map_t cfg;

    cs_uint8 index;
    cs_status rt;

    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    memset(&cfg, 0, sizeof(cfg));
    for(index = 0; index<8; index++){     
        if(index<6){
            cfg.flow0.u |= (iros_strtol(argv[5+index]) << (5*index));
        }else{
            cfg.flow1.u |= (iros_strtol(argv[5+index]) << (5*(index-6)));
        }
    }
   
    rt = aal_flow_cos_mapping_set(port, &cfg);
    diag_printf("configure return %x\n", rt);
}

void aal_cmd_cos_map_get(int argc, char **argv)
{
    cs_aal_port_id_t port;
    cs_aal_flowid_map_t cfg;

    cs_uint8 index;
    cs_status rt;
    
    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    rt = aal_flow_cos_mapping_get(port, &cfg);
    diag_printf("configure return %x\n", rt);

    for(index = 0; index<6; index++){     
        diag_printf(" %d ", (cfg.flow0.u>>(5*index))&0x1f);
    }
   
    for(index = 0; index<2; index++){
        diag_printf(" %d ", (cfg.flow1.u>>(5*index))&0x1f);
    }
    diag_printf(" \n ");
}

void aal_cmd_rate_limit_set(int argc, char **argv)
{
    cs_aal_rate_limit_type_t rl_flow;
    cs_aal_rate_limit_msk_t mask;
    cs_aal_rate_limit_t cfg;

    cs_status rt;
    
    memset(&cfg, 0, sizeof(cfg));
    
    rl_flow = (cs_aal_rate_limit_type_t)iros_strtol(argv[4]);
    if(argc == 5){
        diag_printf(" mask, 0x0001--- rlmode rate limit mode\n");
        diag_printf(" mask, 0x0002--- rate, set the limited rate\n");
        diag_printf(" mask, 0x0004--- cbs, set burst size\n");
        diag_printf(" mask, 0x0008--- acc_msk_a\n");
        diag_printf(" mask, 0x0010--- acc_msk_b\n");
        diag_printf(" mask, 0x0020--- acc_msk_c\n");
        diag_printf(" mask, 0x0040--- acc_msk_d\n");
        diag_printf(" mask, 0x0080--- enable\n");
        diag_printf(" mask, 0x0100--- forward counter mode\n");
        diag_printf(" mask, 0x0200--- pir drop counter mode\n");
        diag_printf(" mask, 0x0400--- tail drop counter mode\n");
    }
    mask.u = iros_strtol(argv[5]);
    if(argc ==6){
        diag_printf(" rate limit mode, 1---pps, 0---bps\n");        
    }
    cfg.ctrl.s.rlmode = iros_strtol(argv[6]);
    if(argc ==7){
        diag_printf(" rate limit enable, 1---enable, 0---disable\n");        
    }
    cfg.ctrl.s.enable = iros_strtol(argv[7]);
    
    if(argc ==8){
        diag_printf(" rate limit rate, 0~1000000\n");        
    }
    cfg.rate = iros_strtol(argv[8]);
    
    if(argc ==9){
        diag_printf(" rate limit burst size, 0~1000\n");        
    }
    cfg.cbs = iros_strtol(argv[9]);  
    
    if(argc ==10){
        diag_printf("forward counter mode, 0- octet mode, 1- packet mode\n");        
    }
    cfg.ctrl.s.fwdcntmod = iros_strtol(argv[10]); 
    
    if(argc ==11){
        diag_printf("forward counter mode, 0- octet mode, 1- packet mode\n");        
    }
    cfg.ctrl.s.pircntmod = iros_strtol(argv[11]);  
    
    if(argc ==12){
        diag_printf("tail drop counter mode, 0- octet mode, 1- packet mode\n");        
    }
    cfg.ctrl.s.tdcntmod = iros_strtol(argv[12]); 
    
    rt = aal_flow_rate_limit_set(rl_flow, &mask, &cfg);
    diag_printf("configure return %x\n", rt);
}

void aal_cmd_rate_limit_get(int argc, char **argv)
{
    cs_aal_rate_limit_type_t rl_flow;
    cs_aal_rate_limit_msk_t mask;
    cs_aal_rate_limit_t cfg;

    cs_status rt;
    
    memset(&cfg, 0, sizeof(cfg));
    
    rl_flow = (cs_aal_rate_limit_type_t)iros_strtol(argv[4]);
    if(argc == 5){
        diag_printf(" mask, 0x0001--- rlmode rate limit mode\n");
        diag_printf(" mask, 0x0002--- rate, get the limited rate\n");
        diag_printf(" mask, 0x0004--- cbs, get burst size\n");
        diag_printf(" mask, 0x0008--- acc_msk_a\n");
        diag_printf(" mask, 0x0010--- acc_msk_b\n");
        diag_printf(" mask, 0x0020--- acc_msk_c\n");
        diag_printf(" mask, 0x0040--- acc_msk_d\n");
        diag_printf(" mask, 0x0080--- enable\n");
        diag_printf(" mask, 0x0100--- forward counter\n");
        diag_printf(" mask, 0x0200--- pir drop counter\n");
        diag_printf(" mask, 0x0400--- tail drop counter\n");
        diag_printf(" mask, 0x0800--- tail drop counter\n");
    }
    mask.u = iros_strtol(argv[5]);   
    rt = aal_flow_rate_limit_get(rl_flow, &mask, &cfg);    
    diag_printf("configure return %x\n", rt);
    
    if(mask.s.rlmode){
        diag_printf("rate limit mode    = %s\n", cfg.ctrl.s.rlmode ? "pps":"bps");
    }
    if(mask.s.enable){
        diag_printf("rate limit enable  = %d\n", cfg.ctrl.s.enable);
    }
    if(mask.s.rate){
        diag_printf("rate limit rate    = %d\n", cfg.rate);
    }
    if(mask.s.cbs){
        diag_printf("rate limit cbs     = %d\n", cfg.cbs);
    }
    if(mask.s.tbc){
        diag_printf("rate limit tbc     = 0x%x\n", cfg.tbc);
    }   
    
    if(mask.s.fwdcnt){
        diag_printf("forward counter    = %lld %s\n", cfg.fwdcnt, cfg.ctrl.s.fwdcntmod ? "packets":"octets");
    }    
    if(mask.s.pircnt){
        diag_printf("over pir counter   = %lld %s\n", cfg.pircnt, cfg.ctrl.s.pircntmod ? "packets":"octets");
    }    
    if(mask.s.tdcnt){
        diag_printf("td  drop counter   = %lld %s\n", cfg.tdcnt, cfg.ctrl.s.tdcntmod ? "packets":"octets");
    }    
}

void aal_cmd_port_shaper_set(int argc, char **argv)
{
    cs_aal_rate_limit_t cfg;
    cs_aal_port_id_t port;
    cs_status rt;
    
    memset(&cfg, 0, sizeof(cfg));
    
    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    cfg.ctrl.s.enable = iros_strtol(argv[5]);
    cfg.rate = iros_strtol(argv[6]);
    cfg.cbs = iros_strtol(argv[7]);

    rt = aal_flow_shaper_set(port, &cfg);
    diag_printf("configure return %x\n", rt);
}

void aal_cmd_port_shaper_get(int argc, char **argv)
{
    cs_aal_rate_limit_t cfg;
    cs_aal_port_id_t port;
    cs_status rt;

    memset(&cfg, 0, sizeof(cfg));
    port = (cs_aal_port_id_t)iros_strtol(argv[4]);
    rt = aal_flow_shaper_get(port, &cfg);
    diag_printf("configure return %x\n", rt);
    
    diag_printf("shaper enable = %d\n", cfg.ctrl.s.enable);
    diag_printf("shaper rate = %d\n", cfg.rate);
    diag_printf("shaper cbs = %d\n", cfg.cbs);
    diag_printf("shaper tbc = %x\n", cfg.tbc);
}


void aal_flow_cmd(int argc, char **argv)
{
    if (argc == 2) {
        diag_printf("\n aal flow 1pmap set [port][msk][map]\n");
        diag_printf("\n aal flow 1pmap get [port]\n");

        diag_printf("\n aal flow cosmap set [port][map]\n");
        diag_printf("\n aal flow cosmap get [port]\n");
        
        diag_printf("\n aal flow dscpmap set [port][map]\n");
        diag_printf("\n aal flow dscpmap get [port]\n");
        
        diag_printf("\n aal flow tcmap set [port][msk][map]\n");
        diag_printf("\n aal flow tcmap get [port]\n");
        
        diag_printf("\n aal flow rl set [type][msk][cfg]\n");
        diag_printf("\n aal flow rl get [type][msk]\n");
        
        diag_printf("\n aal flow shp set [port][en][rate][burst]\n");
        diag_printf("\n aal flow shp get [port]\n");        
    }else{
        if (cmd_cmp(argv[2], "1pmap")) {
            if (cmd_cmp(argv[3], "set")) {
                return aal_cmd_dotlp_map_set(argc, argv);
            }else if(cmd_cmp(argv[3], "get")) {
                return aal_cmd_dotlp_map_get(argc, argv);
            }
        }
        if(cmd_cmp(argv[2], "dscpmap")){
            if (cmd_cmp(argv[3], "set")) {
                return aal_cmd_dscp_map_set(argc, argv); 
            }else if(cmd_cmp(argv[3], "get")){
                return aal_cmd_dscp_map_get(argc, argv); 
            }
        }
        if(cmd_cmp(argv[2], "tcmap")){
            if (cmd_cmp(argv[3], "set")) {
                return aal_cmd_tc_map_set(argc, argv); 
            }else if(cmd_cmp(argv[3], "get")){
                return aal_cmd_tc_map_get(argc, argv); 
            }
        }
        if(cmd_cmp(argv[2], "cosmap")){
            if (cmd_cmp(argv[3], "set")) {
                return aal_cmd_cos_map_set(argc, argv); 
            }else if(cmd_cmp(argv[3], "get")){
                return aal_cmd_cos_map_get(argc, argv); 
            }
        }
        
        if(cmd_cmp(argv[2], "rl")){
            if (cmd_cmp(argv[3], "set")) {
                return aal_cmd_rate_limit_set(argc, argv); 
            }else if(cmd_cmp(argv[3], "get")){
                return aal_cmd_rate_limit_get(argc, argv); 
            }
        }

        
        if(cmd_cmp(argv[2], "shp")){
            if (cmd_cmp(argv[3], "set")) {
                return aal_cmd_port_shaper_set(argc, argv); 
            }else if(cmd_cmp(argv[3], "get")){
                return aal_cmd_port_shaper_get(argc, argv); 
            }
        }
    }
}

