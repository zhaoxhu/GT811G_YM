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

DEFINITIONS:  "DEVICE" means the Cortina Systems?Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems?SDK software.

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
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING OUT
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

#include "sdl_qos.h"
#include "sdl_init.h"
#include "sdl_int.h"
#include "aal_uni.h"
#include "aal_cls.h"
#include "aal_l2.h"
#include "aal_bm.h"
#include "aal_flow.h"
#include "aal_mpcp.h"
#include "aal_sys.h"
#include "cs_types.h"
#include "sdl.h"
#include "sdl_event_cmn.h"
#include "plat_common.h"
#include "aal_util.h"
#ifdef HAVE_MPORTS
#include "rtk_api_ext.h"
#include <rtl8367b_asicdrv_vlan.h>
#endif
#define DSCP_MAX        64
#define TOS_MAX         8

#define MIN_QUE_BUFFER_SIZE    1488
#define US_PORT_BUF_SIZE 0x2380*48
#define DS_PORT_BUF_SIZE 0x1320*48

#define CONGEST_ENENT_TIME_OUT 1500

cs_uint16 __que_len[2][MAX_PRI_NUM] = {
    {DS_MAX_QUE_BUFFER_SIZE, DS_MAX_QUE_BUFFER_SIZE, DS_MAX_QUE_BUFFER_SIZE, DS_MAX_QUE_BUFFER_SIZE, DS_MAX_QUE_BUFFER_SIZE, DS_MAX_QUE_BUFFER_SIZE, DS_MAX_QUE_BUFFER_SIZE, DS_MAX_QUE_BUFFER_SIZE},
    {US_MAX_QUE_BUFFER_SIZE, US_MAX_QUE_BUFFER_SIZE, US_MAX_QUE_BUFFER_SIZE, US_MAX_QUE_BUFFER_SIZE, US_MAX_QUE_BUFFER_SIZE, US_MAX_QUE_BUFFER_SIZE, US_MAX_QUE_BUFFER_SIZE, US_MAX_QUE_BUFFER_SIZE}
};

static cs_uint32 __port_buf_size[2] = {US_PORT_BUF_SIZE,DS_PORT_BUF_SIZE};

extern void aal_bm_de_register_handler(void);
extern void aal_bm_register_handler(void);
void __sdl_us_congest_check(void *data)
{
    cs_uint32 cnt = 0;
    
    static cs_boolean congest_status = FALSE; 

    sdl_event_congest_t event;
    
    cnt = aal_bm_drp_cnt_get();
    if(cnt != 0){
        if(congest_status == TRUE){
            return;
        }    
        congest_status = TRUE;
        
        event.port = CS_UNI_PORT_ID1;
        event.congest = TRUE;
        sdl_event_send(EPON_EVENT_UPSTREAM_CONGEST, sizeof(sdl_event_congest_t),&event);
        SDL_MIN_LOG("upstream congestion!\n");
    }else{
        if(congest_status == FALSE){
            return;
        }    
        congest_status = FALSE;       
        event.port = CS_UNI_PORT_ID1;
        event.congest = FALSE;
        sdl_event_send(EPON_EVENT_UPSTREAM_CONGEST, sizeof(sdl_event_congest_t),&event);
    }
}

void sdl_bm_reg_change_handler()
{
    cs_boolean reg;
    cs_aal_module_reset_mask_t  reset_mask;

    aal_mpcp_reg_status_get(&reg);
    if(reg){
        aal_bm_register_handler();
    }else{
        aal_bm_de_register_handler();

        reset_mask.u32 = 0;
        reset_mask.s.reset_pon = 1;
        aal_sys_reset_ctrl_set(reset_mask,0); /* bug 35979 */
        hal_delay_us(1000);
        aal_sys_reset_ctrl_set(reset_mask,1);
    }
    return;
}


cs_status sdl_qos_init(sdl_init_cfg_t *cfg)
{
    cs_aal_flowid_map_t cos_map;
    cs_aal_flow_mapping_t flow_map;
    cs_aal_flow_mapping_msk_t flow_map_msk;
    cs_aal_dscp_map_t dscp_map[AAL_MAX_DSCP],dscp_map_msk;
    cs_aal_rate_limit_type_t flow_id;
    cs_aal_rate_limit_msk_t rl_msk;
    cs_aal_rate_limit_t rate_limit;
    cs_uint8 index;
    cs_aal_cl_def_fib_data_t cl_def_rule;
    cs_aal_rate_limit_t shp;

    memset (&rate_limit, 0, sizeof (cs_aal_rate_limit_t));
    memset (&rl_msk, 0, sizeof (cs_aal_rate_limit_msk_t));

    cl_def_rule.mask.u32 = 0;
       cl_def_rule.mask.s.cos = 1;
       cl_def_rule.mask.s.rate_limit = 1;
       
       cl_def_rule.flowId_sel = AAL_CL_FLOWID_SEL_TX_COS;/*****flow id is from COS id******/
       cl_def_rule.cos_sel = AAL_CL_COS_SEL_MAP_RX_DOT1P;/******set QoS mode to DSCP*******/
    
       aal_cl_default_fib_set(AAL_PORT_ID_GE, &cl_def_rule);
       
       cl_def_rule.flowId_sel = AAL_CL_FLOWID_SEL_NO;/*****flow id is from reserved ******/
       aal_cl_default_fib_set(AAL_PORT_ID_PON, &cl_def_rule);

    /*****jianguang removed after review***********/
//    flow_map.dot1p.wrd = 0xfac688;
    //ctc mode
    //  flow_map.cos.wrd = 0xfac688;
    /******change the above code to following for good understanding**************/
    flow_map.dot1p.u = 0;
    flow_map.dot1p.s.map0 = 0;
    flow_map.dot1p.s.map1 = 1;
    flow_map.dot1p.s.map2 = 2;
    flow_map.dot1p.s.map3 = 3;
    flow_map.dot1p.s.map4 = 4;
    flow_map.dot1p.s.map5 = 5;
    flow_map.dot1p.s.map6 = 6;
    flow_map.dot1p.s.map7 = 7;

    //ctc mode
    flow_map.cos.u = 0;
    flow_map.cos.s.map0 = 0;
    flow_map.cos.s.map1 = 1;
    flow_map.cos.s.map2 = 2;
    flow_map.cos.s.map3 = 3;
    flow_map.cos.s.map4 = 4;
    flow_map.cos.s.map5 = 5;
    flow_map.cos.s.map6 = 6;
    flow_map.cos.s.map7 = 7;
    /*****jianguang removed after review***********/
    // flow_map.dscp.dscp_tc0.wrd = 0x20610200;
    // flow_map.dscp.dscp_tc1.wrd = 0x38c28;

    //flow_map.tc.dscp_tc0.wrd = 0x20610200;
    //flow_map.tc.dscp_tc1.wrd = 0x38c28;
    //flow_map.flow_id.flow0.wrd = 0xc418820;
    //flow_map.flow_id.flow1.wrd = 0xe6;
    /******change the above code to following for good understanding**************/
    flow_map.dscp.dscp_tc0.u = 0;
    flow_map.dscp.dscp_tc1.u = 0;
    flow_map.dscp.dscp_tc0.s.map0 = 0;
    flow_map.dscp.dscp_tc0.s.map1 = 8;
    flow_map.dscp.dscp_tc0.s.map2 = 16;
    flow_map.dscp.dscp_tc0.s.map3 = 24;
    flow_map.dscp.dscp_tc0.s.map4 = 32;
    flow_map.dscp.dscp_tc1.s.map5 = 40;
    flow_map.dscp.dscp_tc1.s.map6 = 48;
    flow_map.dscp.dscp_tc1.s.map7 = 56;

    flow_map.tc.dscp_tc0.u = 0;
    flow_map.tc.dscp_tc1.u = 0;
    flow_map.tc.dscp_tc0.s.map0 = 0;
    flow_map.tc.dscp_tc0.s.map1 = 8;
    flow_map.tc.dscp_tc0.s.map2 = 16;
    flow_map.tc.dscp_tc0.s.map3 = 24;
    flow_map.tc.dscp_tc0.s.map4 = 32;
    flow_map.tc.dscp_tc1.s.map5 = 40;
    flow_map.tc.dscp_tc1.s.map6 = 48;
    flow_map.tc.dscp_tc1.s.map7 = 56;
    flow_map.flow_id.flow0.u = 0;
    flow_map.flow_id.flow1.u = 0;

    flow_map.flow_id.flow0.s.map0 = 0;
    flow_map.flow_id.flow0.s.map1 = 1;
    flow_map.flow_id.flow0.s.map2 = 2;
    flow_map.flow_id.flow0.s.map3 = 3;
    flow_map.flow_id.flow0.s.map4 = 4;
    flow_map.flow_id.flow0.s.map5 = 5;
    flow_map.flow_id.flow1.s.map6 = 6;
    flow_map.flow_id.flow1.s.map7 = 7;

    flow_map_msk.u = 0;
    flow_map_msk.s.dot1p = 1;
    flow_map_msk.s.cos = 1;
    flow_map_msk.s.dscp = 1;
    flow_map_msk.s.tc = 1;
    flow_map_msk.s.flow_id = 1;
    /************dot1p map setting******************/
    aal_flow_dot1p_mapping_set(AAL_PORT_ID_GE, &flow_map_msk, &flow_map);
    aal_flow_dot1p_mapping_set(AAL_PORT_ID_PON, &flow_map_msk, &flow_map);
    /*****jianguang removed after review***********/

    //cos_map.flow0.wrd = 0xc418820;
    //cos_map.flow1.wrd = 0xe6;
    /******change the above code to following for good understanding**************/
    cos_map.flow0.u = 0;
    cos_map.flow1.u = 0;
    cos_map.flow0.s.map0 = 0;
    cos_map.flow0.s.map1 = 1;
    cos_map.flow0.s.map2 = 2;
    cos_map.flow0.s.map3 = 3;
    cos_map.flow0.s.map4 = 4;
    cos_map.flow0.s.map5 = 5;
    cos_map.flow1.s.map6 = 6;
    cos_map.flow1.s.map7 = 7;
    /********cos map set*********/
    aal_flow_cos_mapping_set(AAL_PORT_ID_GE, &cos_map);
    aal_flow_cos_mapping_set(AAL_PORT_ID_PON, &cos_map);
    dscp_map_msk.u = 0;
    dscp_map_msk.s.dscp_map = 1;
    dscp_map_msk.s.dot1p_map = 1;
    dscp_map_msk.s.cos_map = 1;
    dscp_map_msk.s.flowid_map = 1;
    for (index = 0; index < AAL_MAX_DSCP; index++) {
        dscp_map[index].s.dscp_map = index;
        dscp_map[index].s.dot1p_map = index / 8;
        //ctc mode
        dscp_map[index].s.cos_map = index / 8;
        dscp_map[index].s.flowid_map = index / 8;
    }

    aal_flow_dscp_mapping_set(AAL_PORT_ID_GE, &dscp_map_msk, dscp_map);
    aal_flow_dscp_mapping_set(AAL_PORT_ID_PON, &dscp_map_msk, dscp_map);
    aal_flow_tc_mapping_set(AAL_PORT_ID_GE, &dscp_map_msk, dscp_map);
    aal_flow_tc_mapping_set(AAL_PORT_ID_PON, &dscp_map_msk, dscp_map);

    rl_msk.u = 0;
    rl_msk.s.fwdcnt = 1;
    rl_msk.s.pircnt = 1;
    rl_msk.s.tdcnt = 1;
    rate_limit.ctrl.s.fwdcntmod = 1;
    rate_limit.ctrl.s.pircntmod = 1;
    rate_limit.ctrl.s.tdcntmod = 1;
    for (flow_id = AAL_RATE_LIMIT_GE_SP;flow_id <= AAL_RATE_LIMIT_CPU_DEST;flow_id++) {
        aal_flow_rate_limit_set(flow_id, &rl_msk, &rate_limit);
    }

    // set rate limit for mirrored packets
    memset(&shp, 0, sizeof(shp));
    shp.ctrl.s.enable = 1;
    shp.rate = 20;   // kbps
    shp.cbs = 30;   
    aal_flow_shaper_set(AAL_PORT_ID_PON, &shp); // TE_MIR_SHP_CTRL

    aal_bm_ma_dpid_cos_set(4, 0, 2);
    aal_bm_ma_dpid_cos_set(4, 1, 2);
    aal_bm_ma_dpid_cos_set(4, 2, 3);
    aal_bm_ma_dpid_cos_set(4, 3, 3);
    aal_bm_ma_dpid_cos_set(4, 4, 4);
    aal_bm_ma_dpid_cos_set(4, 5, 4);
    aal_bm_ma_dpid_cos_set(4, 6, 7);
    aal_bm_ma_dpid_cos_set(4, 7, 7);
    for(index = 0; index<8; index++){
        if(index<4){
            aal_bm_ma_dpid_cos_set(6,index,5);
            aal_bm_ma_dpid_cos_set(5,index,1);
            }else{
            aal_bm_ma_dpid_cos_set(6,index,6);
            aal_bm_ma_dpid_cos_set(5,index,1);
        }
    }
    aal_bm_share_mode_init();

    sdl_int_handler_reg(SDL_INT_REG_CHANGE, sdl_bm_reg_change_handler);
    
    cs_circle_timer_add(CONGEST_ENENT_TIME_OUT, __sdl_us_congest_check, NULL);

    return CS_E_OK;
}


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_mib_que_info_set        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_dot1p_map_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_pri_cos_map_t          *queue_map
)

{
    cs_aal_flow_mapping_msk_t qos_msk;
    cs_aal_flow_mapping_t qos_cfg;
    cs_uint8 index;

    cs_status ret = CS_E_OK;

    if (!queue_map) {
        SDL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        ret = CS_E_PARAM;
        goto end;
    }
    for(index = 0; index < MAX_PRI_NUM; index++){
        if(queue_map->cos[index] > (MAX_PRI_NUM-1)){
            goto end;
        }
    }

    qos_msk.u = 0;
    qos_msk.s.cos = 1;
    qos_cfg.cos.s.map0 = queue_map->cos[0];
    qos_cfg.cos.s.map1 = queue_map->cos[1];
    qos_cfg.cos.s.map2 = queue_map->cos[2];
    qos_cfg.cos.s.map3 = queue_map->cos[3];
    qos_cfg.cos.s.map4 = queue_map->cos[4];
    qos_cfg.cos.s.map5 = queue_map->cos[5];
    qos_cfg.cos.s.map6 = queue_map->cos[6];
    qos_cfg.cos.s.map7 = queue_map->cos[7];

    ret = aal_flow_dot1p_mapping_set(AAL_PORT_ID_GE, &qos_msk, &qos_cfg);
    if (ret) {
        goto end;
    }

    ret = aal_flow_dot1p_mapping_set(AAL_PORT_ID_PON, &qos_msk, &qos_cfg); 
    if(ret)
    {
        goto end;
    }
    
end:
    return ret;
}


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_dot1p_map_get        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_dot1p_map_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_pri_cos_map_t         *queue_map
)

{
    cs_aal_flow_mapping_t qos_cfg;
    cs_status ret = CS_E_OK;

    if (!queue_map) {
        SDL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        ret = CS_E_PARAM;
        goto end;
    }
    ret = aal_flow_dot1p_mapping_get(AAL_PORT_ID_GE, &qos_cfg);
    if (ret) {
        goto end;
    }
    queue_map->cos[0] = qos_cfg.cos.s.map0;
    queue_map->cos[1] = qos_cfg.cos.s.map1;
    queue_map->cos[2] = qos_cfg.cos.s.map2;
    queue_map->cos[3] = qos_cfg.cos.s.map3;
    queue_map->cos[4] = qos_cfg.cos.s.map4;
    queue_map->cos[5] = qos_cfg.cos.s.map5;
    queue_map->cos[6] = qos_cfg.cos.s.map6;
    queue_map->cos[7] = qos_cfg.cos.s.map7;
end:
    return ret;
}

/********************************************************************************/
/* $rtn_hdr_start  epon_request_onu__tos_map_set                                */
/* CATEGORY   : Device                                                          */
/* ACCESS     : Public                                                          */
/* BLOCK      : SAL                                                             */
/* CHIP       : LynxD                                                           */
cs_status epon_request_onu_tos_map_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_tos_pri_map_t          *tos_map
)

{
    cs_uint8 i;
    cs_aal_dscp_map_t qos_cfg[DSCP_MAX];
    cs_aal_dscp_map_t qos_msk;
    cs_pri_cos_map_t  queue_map;
    cs_status ret = CS_E_OK;

    if (!tos_map) {
        SDL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        ret = CS_E_PARAM;
        goto end;
    }

    memset(&queue_map, 0, sizeof(cs_pri_cos_map_t));
    memset((cs_uint8 *)qos_cfg, 0, sizeof(cs_aal_dscp_map_t)*DSCP_MAX);

    ret  = epon_request_onu_dot1p_map_get(context, 0, 0, &queue_map);
    if (ret) {
        goto end;
    }

    qos_msk.u = 0;
    qos_msk.s.dot1p_map = 1;
    qos_msk.s.cos_map   = 1;

    /***********workaround for dscp->cos mapping, which should match 8021p to cos mapping***************/
    for (i = 0; i < DSCP_MAX; i++) {
        qos_cfg[i].s.cos_map = queue_map.cos[tos_map->pri[i/(DSCP_MAX/TOS_MAX)]];
    }

#if 1
    for (i = 0; i < DSCP_MAX; i++) {
        qos_cfg[i].s.dot1p_map = tos_map->pri[i/(DSCP_MAX/TOS_MAX)];
    }
#endif

    ret = aal_flow_dscp_mapping_set(AAL_PORT_ID_GE, &qos_msk, qos_cfg);
    if (ret) {
        goto end;
    }

    ret = aal_flow_dscp_mapping_set(AAL_PORT_ID_PON, &qos_msk, qos_cfg);
    if (ret) {
        goto end;
    }

    ret = aal_flow_tc_mapping_set(AAL_PORT_ID_GE, &qos_msk, qos_cfg);
    if (ret) {
        goto end;
    }

    ret = aal_flow_tc_mapping_set(AAL_PORT_ID_PON, &qos_msk, qos_cfg);
    if (ret) {
        goto end;
    }

end:
    return ret;
}


/*******************************************************************************/
/* $rtn_hdr_start  epon_request_onu_tos_map_get                                 */
/* CATEGORY   : Device                                                          */
/* ACCESS     : Public                                                          */
/* BLOCK      : SAL                                                             */
/* CHIP       : LynxD                                                           */
cs_status epon_request_onu_tos_map_get (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_OUT cs_tos_pri_map_t         *tos_map
)

{
    cs_uint8 i;
    cs_aal_dscp_map_t qos_cfg[DSCP_MAX];
    cs_status ret = CS_E_OK;
    
    if(!tos_map){
        SDL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        ret = CS_E_PARAM;
        goto end;
    }
    
   ret = aal_flow_dscp_mapping_get(AAL_PORT_ID_GE, qos_cfg);
   if(ret){
       goto end;
   }

    for(i=0; i<TOS_MAX; i++) {
        tos_map->pri[i] = qos_cfg[i*(DSCP_MAX/TOS_MAX)].s.dot1p_map;
    }
end:    
    return ret;
}


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_que_size_set        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_que_size_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_uint8                  que,
    CS_IN cs_uint16                 size
)

{
    cs_aal_bm_queue_msk_t que_msk;
    cs_aal_bm_queue_val_t que_len;
    cs_uint8 port;
    cs_sdl_sched_type_t sche_mode;

    cs_status ret = CS_E_OK;
    

    switch (port_id) {
    case CS_PON_PORT_ID:
        port = 0;        
        if (size > DS_MAX_QUE_BUFFER_SIZE) {
            ret = CS_E_PARAM;
            goto end;
        }
        break;
    case CS_UNI_PORT_ID1:
        port = 1;
        if (size > US_MAX_QUE_BUFFER_SIZE) {
            ret = CS_E_PARAM;
            goto end;
        }
        
        break;
    default:
        ret = CS_E_PARAM;
        goto end;
    }

    if (que > (MAX_PRI_NUM-1)) {
        ret = CS_E_PARAM;
        goto end;
    }

    que_msk.u32 = 0;
    que_msk.s.queue = 1 << que;

    __que_len[port][que] = size;

    ret = epon_request_onu_schedule_mode_get(context, device, llidport, &sche_mode);
    if (ret != CS_E_OK) {
        goto end;
    }
    if (sche_mode == SDL_SCHD_WRR_MODE) {
        goto end;
    }

    que_len.val[que] = (size * 1024 / 48 - 31) / 32;

    ret = aal_bm_share_queue_max_length_set(port, que_msk, &que_len);
end:
    return ret;
}


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_que_size_get        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_que_size_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_uint8                  que,
    CS_OUT cs_uint16                 *size
)

{
    cs_status ret = CS_E_OK;
    cs_uint8 port = 0;

    if (!size) {
        ret = CS_E_PARAM;
        goto end;
    }
    if (que > (MAX_PRI_NUM-1)) {
        ret = CS_E_PARAM;
        goto end;
    }


    switch (port_id) {
    case CS_PON_PORT_ID:
        port = 0;
        break;
    case CS_UNI_PORT_ID1:
        port = 1;
        break;
    default:
        ret = CS_E_PARAM;
        goto end;
    }

    *size = __que_len[port][que];
end:
    return ret;
}


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_buf_size_set        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_buf_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_uint32                 size
)

{
 
    cs_aal_port_id_t port;
    cs_status ret = CS_E_OK;
    cs_uint16 port_len;
    
    switch (port_id) {
    case CS_PON_PORT_ID:
        port = AAL_PORT_ID_PON;        
        if(size>US_PORT_BUF_SIZE){
            ret = CS_E_PARAM;
            goto end;            
        }
        break;
    case CS_UNI_PORT_ID1:
        port = AAL_PORT_ID_GE;
        if(size>DS_PORT_BUF_SIZE){
            ret = CS_E_PARAM;
            goto end;            
        }        
        break;
    default:
        ret = CS_E_PARAM;
        goto end;
    }
    
    port_len = size/48+1;
    
    ret = aal_bm_port_buf_size_set(port, port_len);
    __port_buf_size[port] = size;
end:
    return ret;
}


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_buf_get        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_buf_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_uint32                 *size
)

{
    cs_aal_port_id_t port;

    cs_status ret = CS_E_OK;

    if (!size) {
        ret = CS_E_PARAM;
        goto end;
    }


    switch (port_id) {
    case CS_PON_PORT_ID:
        port = AAL_PORT_ID_PON;
        break;
    case CS_UNI_PORT_ID1:
        port = AAL_PORT_ID_GE;
        break;
    default:
        ret = CS_E_PARAM;
        goto end;
    }

    *size = __port_buf_size[port];
end:
    return ret;
}

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_que_size_get        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_port_buf_ability_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_buf_abi_t                 *abi
)

{
    cs_status ret = CS_E_OK;

    if (!abi) {
        ret = CS_E_PARAM;
        goto end;
    }
    switch (port_id) {
    case CS_PON_PORT_ID:
        abi->min_buf_abi = MIN_QUE_BUFFER_SIZE;
        abi->max_buf_abi = US_MAX_QUE_BUFFER_SIZE*1024;
        break;
    case CS_UNI_PORT_ID1:
        abi->min_buf_abi = MIN_QUE_BUFFER_SIZE;
        abi->max_buf_abi = DS_MAX_QUE_BUFFER_SIZE*1024;
        break;
    default:
        ret = CS_E_PARAM;
        goto end;
    }

end:
    return ret;
}


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_schedule_mode_set        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_schedule_mode_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_sdl_sched_type_t       sche_mode
)

{
    cs_aal_bm_weight_t sche;
    cs_uint32 index;
#ifdef HAVE_MPORTS
    rtk_qos_queue_weights_t rtk_que_wei;
#endif

    cs_status ret = CS_E_OK;

    if (sche_mode == SDL_SCHD_SP_MODE) {
        sche.mode = ONU_BM_SCHED_SP;

        for (index = 0; index < MAX_PRI_NUM; index++) {
            sche.weight[index] = 0;
        }
    } else if (sche_mode == SDL_SCHD_WRR_MODE) {
        sche.mode = ONU_BM_SCHED_WRR;
        sche.weight[0] = 1;
        for (index = 1; index < MAX_PRI_NUM; index++) {
            sche.weight[index] = (sche.weight[index-1]) * 2;
        }
    } else {
        ret = CS_E_PARAM;
        goto end;
    }

    ret = aal_bm_share_scheduler_set(AAL_PORT_ID_PON, &sche);
    if (ret) {
        goto end;
    }
    ret = aal_bm_share_scheduler_set(AAL_PORT_ID_GE, &sche);
    if (ret) {
        goto end;
    }
#ifdef HAVE_MPORTS
    for (index = 0; index < MAX_PRI_NUM -1; index++) {
        rtk_que_wei.weights[index] = sche.weight[index];
    }
    if (sche_mode == SDL_SCHD_SP_MODE) {
        rtk_que_wei.weights[MAX_PRI_NUM -1] = 0;
    } else {
        rtk_que_wei.weights[MAX_PRI_NUM -1] = 127;
    }
    for (index = 0; index < 4; index++) {
        ret = rtk_qos_schedulingQueue_set(index, &rtk_que_wei);
        if (ret) {
            goto end;
        }
    }
#endif
end:
    return ret;
}

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_schedule_mode_get        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_schedule_mode_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_OUT cs_sdl_sched_type_t      *sche_mode
)

{
    cs_aal_bm_weight_t sche;

    cs_status ret = CS_E_OK;
    
    if(sche_mode == NULL){
        ret = CS_E_PARAM;
        goto end;
    }

    ret = aal_bm_share_scheduler_get(AAL_PORT_ID_PON, &sche);
    if(ret){
        goto end;
    }
    if(sche.mode == ONU_BM_SCHED_SP){
        *sche_mode = SDL_SCHD_SP_MODE;
    }else if(sche.mode == ONU_BM_SCHED_WRR){
        *sche_mode = SDL_SCHD_WRR_MODE;
    }else{
        ret = CS_E_PARAM;
    }
    
end:
    return ret;
}




