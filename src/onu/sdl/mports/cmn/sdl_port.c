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

DEFINITIONS:  "DEVICE" means the Cortina Systems? Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems? SDK software.

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

Copyright (c) 2010 by Cortina Systems Incorporated
****************************************************************************/
#include "plat_common.h"
#include "rtk_api_ext.h"
#include "sdl_port.h"
#include "sdl_init.h"
#include "aal_pon.h"
#include "aal_flow.h"
#include "aal_uni.h"
#include "aal_l2.h"
#include "sdl_int.h"
#include "sdl.h"
#include "sdl_event_cmn.h"
#include "aal_util.h"
#include "aal_cls.h"
#include "aal_cls_util.h"
#include <rtk_api.h>
#include <rtk_api_ext.h>
#include <rtl8367b_asicdrv_vlan.h>

#define __SDL_PORT_AUTO_NEGO_FAIL              0
#define __SDL_PORT_AUTO_NEGO_SUCCESS           1
#define __SDL_AUTO_NEG_FAIL_CHECK_INTVL        500        /*500 ms*/
#define __SDL_AUTO_NEG_FAIL_CHECK_START_INTVL  1000       /*1s*/
#define __SDL_PORT_MTU_MAX                     1552       
#define __PHY_REGISTER_19                      19

#define MIN_BURST_SIZE              10
#define MAX_BURST_SIZE              500
#define SWITCH_UPLINK_PORT_ID       6
#define MIRROR_MAX_PORT_MASK        0x1f
#define PORT_STORM_METER_ID_START   10
#define PORT_STORM_METER_ID_END     14
#define MAX_PORT_NUMBER              4
#define MIN_STORM_RATE              8
#define STORM_GRANULARITY           8

#define BURST_CONTROL_REG 0x1203

extern void cs_polling_handler_reg(void (*handler)());
extern rtk_api_ret_t rtk_chip_id_get(rtk_chip_id_t *chip);

/*Port mask map from mirror port mask to switch port mask*/
#define MSK_MIRROR_TO_SWITCH(mirror_msk, switch_msk)  do{           \
    if(mirror_msk & 0x10){                                          \
        switch_msk = mirror_msk & (~(0x10));                        \
        switch_msk |= 0x40;                                         \
    }                                                               \
    else{                                                           \
        switch_msk = mirror_msk;                                    \
    }                                                               \
                                                                    \
}while(0)


/*Port mask map from switch port mask to mirror port mask*/
#define MSK_SWITCH_TO_MIRROR(switch_msk, mirror_msk)                \
do{                                                                 \
    if(switch_msk & 0x40 ){                                         \
        mirror_msk = switch_msk & (~(0x40));                        \
        mirror_msk |= 0x10;                                         \
    }                                                               \
    else{                                                           \
        mirror_msk = switch_msk;                                    \
    }                                                               \
                                                                    \
}while(0)


cs_uint32 __pir = 0;
cs_sdl_policy_t __us_rate_limit[UNI_PORT_MAX];
cs_sdl_policy_t __ds_rate_limit[UNI_PORT_MAX];
cs_sdl_policy_t __sc_rate_limit[UNI_PORT_MAX];

cs_boolean __port_isolation = 0;
cs_sdl_storm_ctrl_e __storm_type[UNI_PORT_MAX] = {SDL_STORM_TYPE_DISABLE, SDL_STORM_TYPE_DISABLE, SDL_STORM_TYPE_DISABLE, SDL_STORM_TYPE_DISABLE};

cs_sdl_port_admin_t       __pon_admin;
//cs_pri_remark_t __dot1p_remark[4];
//cs_uint8 __acl_occupy[4][8] = {{0,},};
//cs_uint32 __acl_avl = 0;

cs_uint32 max_port_traffic_rate =1000000;



cs_sdl_port_loopback_t __pon_loopback_mode = SDL_PORT_LOOPBACK_NONE;
cs_sdl_port_loopback_t __uni_loopback_mode[UNI_PORT_MAX] = {SDL_PORT_LOOPBACK_NONE, SDL_PORT_LOOPBACK_NONE, SDL_PORT_LOOPBACK_NONE, SDL_PORT_LOOPBACK_NONE};
//cs_sdl_uni_loopback_depth_t __uni_loopback_depth = SDL_UNI_MAC_LOOPBACK;

typedef struct {
    cs_sdl_port_speed_cfg_t port_cfg;
    cs_sdl_port_admin_t     uni_admin;
    cs_boolean              flow_ctrl_en; 
}__sdl_port_config_t;


cs_sdl_port_drop_cfg_t __drop_pon = SDL_PORT_NO_DROP;
cs_sdl_port_drop_cfg_t __drop_ge = SDL_PORT_NO_DROP;

__sdl_port_config_t __port_cfg[UNI_PORT_MAX];

rtk_port_linkStatus_t __uni_link_ststus[UNI_PORT_MAX] = {PORT_LINKDOWN, PORT_LINKDOWN, PORT_LINKDOWN, PORT_LINKDOWN};

extern sdl_init_cfg_t sdl_int_cfg;
cs_boolean g_pkt_remote_capture_enable;


/*****************************************************************************/
/*                    Local Functions                                        */
/*****************************************************************************/
#ifdef HAVE_SDL_CTC
static void __sdl_port_auto_neg_polling_handler(void)
{
    sdl_event_port_auto_nego_t auto_nego_event;
    rtk_port_phy_data_t phy_data;
    static cs_uint8 counter = 20;
    rtk_port_t port;
    rtk_api_ret_t rtk_ret = 0;
    
    if(counter>0){
        counter--;
        return;
    }

    for(port=0; port<UNI_PORT_MAX; port++) {
        rtk_ret = rtk_port_phyReg_get(port, __PHY_REGISTER_19, &phy_data);
        if(RT_ERR_OK != rtk_ret)
           return;
           
        if (phy_data & (0x1 << 15)){
            //cs_printf("port %d, auto nego failed! Reg value: 0x%x\n", P2L_PORT(port), phy_data);
            auto_nego_event.port = P2L_PORT(port);
            auto_nego_event.status = __SDL_PORT_AUTO_NEGO_FAIL;
            sdl_event_send(EPON_EVENT_PORT_AUTO_NEGO_STATUS, sizeof(sdl_event_port_auto_nego_t), &auto_nego_event);
        }
    }
    
    counter = 20;
    
    return;

}
#endif
void __sdl_port_int_process(void)
{
    rtk_int_status_t status;
    rtk_port_linkStatus_t link_status;
    rtk_port_t port;
    rtk_data_t speed;
    rtk_data_t duplex;
    rtk_data_t nego;
    sdl_event_port_link_t link_event;
    rtk_port_phy_data_t phy_data;
    
#ifdef HAVE_SDL_CTC
    sdl_event_port_auto_nego_t auto_nego_event;
#endif

    rtk_int_control_set(INT_TYPE_LINK_STATUS, 0);
    rtk_int_status_get(&status);
    if (status.value[INT_TYPE_LINK_STATUS]) {
        for (port=0; port <UNI_PORT_MAX; port++) {
            if(RT_ERR_OK != rtk_port_phyReg_get(port, __PHY_REGISTER_19, &phy_data))
               return;
            
#if 0 /*removed: some time link change event's missed*/
            if ((phy_data & (0x1 << 10))==0){
                continue;
            }
#endif
            
#ifdef HAVE_SDL_CTC
            if (phy_data & (0x1 << 11)){
                //cs_printf("port %d, auto nego success! Reg value: 0x%x\n", P2L_PORT(port), phy_data);
                auto_nego_event.port = P2L_PORT(port);
                auto_nego_event.status = __SDL_PORT_AUTO_NEGO_SUCCESS;
                sdl_event_send(EPON_EVENT_PORT_AUTO_NEGO_STATUS, sizeof(sdl_event_port_auto_nego_t), &auto_nego_event);
            }
            else if (phy_data & (0x1 << 15)){
                //cs_printf("port %d, auto nego fail! Reg value: 0x%x\n", P2L_PORT(port), phy_data);
                auto_nego_event.port = P2L_PORT(port);
                auto_nego_event.status = __SDL_PORT_AUTO_NEGO_FAIL;
                sdl_event_send(EPON_EVENT_PORT_AUTO_NEGO_STATUS, sizeof(sdl_event_port_auto_nego_t), &auto_nego_event);
            }
#endif

            rtk_port_phyStatus_get(port, &link_status, &speed, &duplex, &nego);
            if (link_status == __uni_link_ststus[port]) {
                continue;
            }
            
            __uni_link_ststus[port] = link_status;
            //SDL_MIN_LOG("port %d, %s\n", port + CS_UNI_PORT_ID1, (link_statu == PORT_LINKUP) ? "link up" : "link down");
            link_event.link = link_status;
            link_event.port = P2L_PORT(port);
            sdl_event_send(EPON_EVENT_PORT_LINK_CHANGE, sizeof(sdl_event_port_link_t), &link_event);

        }
    }

    memset(&status, 0, sizeof(status));
    status.value[INT_TYPE_LINK_STATUS] = 1;
    rtk_int_status_set(status);
    rtk_int_control_set(INT_TYPE_LINK_STATUS, 1);

    return;

}

/****************************************************************/

cs_status epon_request_onu_port_mtu_get(
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_int32                  device_id,
    CS_IN  cs_int32                  llidport,
    CS_IN  cs_port_id_t              port_id,
    CS_OUT cs_uint16                 *mtu
)
{
    cs_aal_pon_mac_cfg_t cfg;
    cs_uint32            uni_mtu;
    rtk_api_ret_t        rtk_ret = 0; 

    if (NULL == mtu) {
        SDL_MIN_LOG("MTU is NULL pointer!\n");
        return CS_E_PARAM;
    }
    
    if (port_id > UNI_PORT_MAX){
        SDL_MIN_LOG("Port ID is Invalid\n");
        return CS_E_PARAM;
    }
    
    if (port_id == CS_PON_PORT_ID) {
            aal_pon_mac_cfg_get(&cfg);
            *mtu = cfg.mtu;
    }
    else{
        rtk_ret = rtk_switch_portMaxPktLen_get(port_id-1, &uni_mtu);
        *mtu = uni_mtu & 0xffff;
    }
        
    return CS_E_OK;

}

cs_status epon_request_onu_port_mtu_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_uint16                 mtu
)
{
    cs_aal_pon_mac_cfg_t    cfg;
    cs_aal_pon_mac_msk_t    mask;
    rtk_api_ret_t           rtk_ret = 0; 
    cs_status               rt = CS_E_OK;
    
    if (mtu > __SDL_PORT_MTU_MAX) {
        SDL_MIN_LOG(" mtu %d is not supported!\n", , mtu);
        return CS_E_PARAM;
    }
    
    if (port_id > UNI_PORT_MAX){
        SDL_MIN_LOG("Port ID is Invalid\n");
        return CS_E_PARAM;
    }
    
    if (port_id == CS_PON_PORT_ID) {
        mask.u32 = 0;
        mask.s.mtu = 1;
        mask.s.tx_max_size = 1;
        mask.s.mtu_tag_en = 1;

        cfg.mtu = mtu;
        cfg.tx_max_size = mtu;
        cfg.mtu_tag_en = sdl_int_cfg.mtu_tag_exclude;
        rt = aal_pon_mac_cfg_set(mask, &cfg);
        if (rt) {
            SDL_MIN_LOG("aal_pon_mac_cfg_set return  %d\n", rt);
            return rt;
        }
    }
    else{
        if (mtu <= 1522) {
            mtu = 1522;
        } 
        else if ((mtu > 1522) && (mtu <= 1536)) {
            mtu = 1536;
        } 
        else {
            mtu = 1552;
        }

        rtk_ret = rtk_switch_portMaxPktLen_set(port_id - 1, mtu);
        if (RT_ERR_OK != rtk_ret) {
            SDL_MIN_LOG("rtk_switch_portMaxPktLen_set return %dd\n", rtk_ret);
            return CS_E_ERROR;
        }
    }
        
    return CS_E_OK;
    
}


cs_status epon_request_onu_port_speed_get(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_port_ether_speed_t    *speed
)
{
    rtk_port_linkStatus_t  uni_link;
    rtk_data_t             rtk_speed;
    rtk_data_t             duplex;
    rtk_data_t             nego;
    rtk_port_t             port;
    rtk_api_ret_t          rtk_ret = 0; 
    
    if (NULL == speed) {
        SDL_MIN_LOG("Speed is NULL pointer!\n", );
        return CS_E_PARAM;
    }

    UNI_PORT_CHECK(port_id);
    port = L2P_PORT(port_id);

    /** PHY link status */
    rtk_ret = rtk_port_phyStatus_get(port, &uni_link, &rtk_speed, &duplex, &nego);
    if (RT_ERR_OK != rtk_ret) {
        SDL_MIN_LOG("rtk_port_phyStatus_get return %d\n", rtk_ret);
        return CS_E_ERROR;
    }
    
    *speed = rtk_speed;

    return CS_E_OK;
    
}

cs_status epon_request_onu_port_duplex_get(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_port_ether_duplex_t   *duplex
)
{
    rtk_port_linkStatus_t  uni_link;
    rtk_data_t             rtk_speed;
    rtk_data_t             rtk_duplex;
    rtk_data_t             nego;
    rtk_port_t             port;
    rtk_api_ret_t          rtk_ret  = 0;
    
    if (NULL == duplex) {
        SDL_MIN_LOG("duplex is NULL pointer\n", );
        return CS_E_PARAM;
    }
    
    UNI_PORT_CHECK(port_id);
    
    port = L2P_PORT(port_id);

    /** PHY link status */
    rtk_ret = rtk_port_phyStatus_get(port, &uni_link, &rtk_speed, &rtk_duplex, &nego);
    if (RT_ERR_OK != rtk_ret) {
        SDL_MIN_LOG("rtk_port_phyStatus_get return %d\n", rtk_ret);
        return CS_E_ERROR;
    }

    if(rtk_duplex)
        *duplex = SDL_PORT_DUPLEX_FULL;
    else
        *duplex = SDL_PORT_DUPLEX_HALF;

    return CS_E_OK;
    
}

cs_status epon_request_onu_port_auto_neg_get(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_port_autoneg_status_t *auto_neg
)

{
    cs_sdl_port_speed_cfg_t port_cfg;

    if (NULL == auto_neg) {
        SDL_MIN_LOG("auto_neg is NULL pointer!\n", );
        return CS_E_PARAM;
    }

    UNI_PORT_CHECK(port_id);
    
    port_cfg = __port_cfg[port_id -1].port_cfg;

    if ((port_cfg == SDL_PORT_AUTO_10_100)
        || (port_cfg == SDL_PORT_AUTO_10_100_1000) 
        || (port_cfg == SDL_PORT_1000_FULL)){
        *auto_neg = SDL_PORT_AUTONEG_UP;
    } 
    else {
        *auto_neg = SDL_PORT_AUTONEG_DOWN;
    }

    return CS_E_OK;
}

cs_status epon_request_onu_port_autoneg_restart(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id
)
{
    rtk_port_t      port;
    rtk_uint32      phyData;
    rtk_api_ret_t   rtk_ret  = 0;
    cs_status       rt = CS_E_OK;
    
    UNI_PORT_CHECK(port_id);

    port = L2P_PORT(port_id);
    
    rtk_ret = rtk_port_phyReg_get(port, PHY_CONTROL_REG, &phyData);
    if(RT_ERR_OK != rtk_ret){
        SDL_MIN_LOG("rtk_port_phyReg_get return %d\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }
    
    phyData = phyData | (1 << 9);
    rtk_ret = rtk_port_phyReg_set(port, PHY_CONTROL_REG, phyData);
    if(RT_ERR_OK != rtk_ret){
        SDL_MIN_LOG("rtk_port_phyReg_set return %d\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }

END:
    return rt;
}


cs_status epon_request_onu_port_mdi_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_sdl_port_mdi_t         mdi
)
{

    return CS_E_NOT_SUPPORT;
}

cs_status epon_request_onu_port_mdi_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_sdl_port_mdi_t        *mdi
)
{
    return CS_E_NOT_SUPPORT;
}
cs_status epon_request_onu_port_lpbk_set(
    CS_IN cs_callback_context_t             context,
    CS_IN cs_int32                          device_id,
    CS_IN cs_int32                          llidport,
    CS_IN cs_port_id_t                      port_id,
    CS_IN cs_sdl_port_loopback_t            loopback
)
{
    rtk_port_t                     port;
    rtk_uint32                     phyData;
    static cs_sdl_port_speed_cfg_t port_cfg[UNI_PORT_MAX];
    rtk_api_ret_t                  rtk_ret  = 0;
    cs_status                      rt = CS_E_OK;
    
    if (port_id > UNI_PORT_MAX){
        SDL_MIN_LOG("Port ID is Invalid\n");
        return CS_E_PARAM;
    }
    
    if (port_id == CS_PON_PORT_ID) {
        if (__pon_loopback_mode == loopback) {
            rt =  CS_E_OK;
            goto END;
        }

        switch (loopback) {
            case SDL_PORT_LOOPBACK_NONE: {
                rt = aal_pon_loopback_set(EPON_FALSE);
                if (rt) {
                    SDL_MIN_LOG("aal_pon_loopback_set return %d\n", rt);
                    goto END;
                }

                rt = epon_request_onu_port_traffic_set(context, device_id, llidport, CS_UNI_PORT_ID1, SDL_PORT_NO_DROP);
                if (rt) {
                    SDL_MIN_LOG("epon_request_onu_port_traffic_set return %d\n", rt);
                    goto END;
                }
            }
            break;

            case SDL_PORT_LOOPBACK_TX2RX: {
                rt = aal_pon_loopback_set(EPON_TRUE);
                if (rt) {
                    SDL_MIN_LOG("aal_pon_loopback_set fail!return %d\n", rt);
                    goto END;
                }

                rt = epon_request_onu_port_traffic_set(context, device_id, llidport, CS_UNI_PORT_ID1, SDL_PORT_DROP_ALL);
                if (rt) {
                    SDL_MIN_LOG("epon_request_onu_port_traffic_set return %d\n", rt);
                    goto END;
                }
            }
            break;

            case SDL_PORT_LOOPBACK_RX2TX:
            default: {
                SDL_MIN_LOG("loopback %d is not supported\n", loopback);
                return CS_E_PARAM;
            }
        }

        __pon_loopback_mode = loopback;

    } 
    else {
        port = L2P_PORT(port_id);
        if (__uni_loopback_mode[port] == loopback) {
            rt =  CS_E_OK;
            goto END;
        }

        switch (loopback) {
            case SDL_PORT_LOOPBACK_NONE: {
                rt = epon_request_onu_port_status_set(context, device_id, llidport, port_id, port_cfg[port]);
                if (rt) {
                    SDL_MIN_LOG("epon_request_onu_port_status_set return %d\n", rt);
                    goto END;
                }
                
                rtk_ret = rtk_port_phyReg_get(port, PHY_CONTROL_REG, &phyData);
                if(RT_ERR_OK != rtk_ret){
                    SDL_MIN_LOG("rtk_port_phyReg_get return %d\n", rtk_ret);
                    rt = CS_E_ERROR;
                    goto END;
                }
                
                phyData = phyData & (~(0x1 << 14));
                rtk_ret = rtk_port_phyReg_set(port, PHY_CONTROL_REG, phyData);
                if(RT_ERR_OK != rtk_ret){
                    SDL_MIN_LOG("rtk_port_phyReg_set return %d\n", rtk_ret);
                    rt = CS_E_ERROR;
                    goto END;
                }
            }
            break;

            case SDL_PORT_LOOPBACK_TX2RX: {
                port_cfg[port] = __port_cfg[port].port_cfg;
                rt = epon_request_onu_port_status_set(context, device_id, llidport, port_id, SDL_PORT_100_FULL);
                if (rt) {
                    SDL_MIN_LOG("epon_request_onu_port_status_set return %d!\n", rt);
                    goto END;
                }
                
                rtk_ret = rtk_port_phyReg_get(port, PHY_CONTROL_REG, &phyData);
                if(RT_ERR_OK != rtk_ret){
                    SDL_MIN_LOG("rtk_port_phyReg_get return %d\n", rtk_ret);
                    rt = CS_E_ERROR;
                    goto END;
                }

                phyData = phyData | (0x1 << 14);
                
                rtk_ret = rtk_port_phyReg_set(port, PHY_CONTROL_REG, phyData);
                if(RT_ERR_OK != rtk_ret){
                    SDL_MIN_LOG("rtk_port_phyReg_set return %d\n", rtk_ret);
                    rt = CS_E_ERROR;
                    goto END;
                }

            }
            break;

            case SDL_PORT_LOOPBACK_RX2TX:
            default: {
                SDL_MIN_LOG(" loopback %d is not supported\n", , loopback);
                rt = CS_E_PARAM;
                goto END;
            }
        }

        __uni_loopback_mode[port] = loopback;
    }

END:
    return rt;
}

cs_status epon_request_onu_port_lpbk_get(
    CS_IN cs_callback_context_t             context,
    CS_IN cs_int32                          device_id,
    CS_IN cs_int32                          llidport,
    CS_IN cs_port_id_t                      port_id,
    CS_OUT cs_sdl_port_loopback_t           *loopback
)
{

    if (NULL == loopback) {
        SDL_MIN_LOG("loopback is NULL pointer\n");
        return CS_E_PARAM;
    }

    if (port_id >UNI_PORT_MAX){
        SDL_MIN_LOG("Port ID is Invalid\n");
        return CS_E_PARAM;
    }
    
    if (port_id == CS_PON_PORT_ID) {
        *loopback = __pon_loopback_mode;
    }
    else{
        *loopback = __uni_loopback_mode[port_id-1];
    } 

    return CS_E_OK;
}

cs_status epon_request_onu_port_status_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_sdl_port_speed_cfg_t   speed_cfg
)
{
    rtk_port_t             port;
    rtk_port_phy_ability_t phy_abi;
    rtk_port_mac_ability_t mac_force_abi;
    rtk_chip_id_t          chip_id;
    rtk_api_ret_t          rtk_ret  = 0;
    cs_status              rt = CS_E_OK;
    
    UNI_PORT_CHECK(port_id);
    
    port = L2P_PORT(port_id);
    if (__port_cfg[port].port_cfg == speed_cfg) {
        goto END;
    }

    if ((max_port_traffic_rate == 100000) && (speed_cfg == SDL_PORT_AUTO_10_100_1000)) {
        speed_cfg = SDL_PORT_AUTO_10_100;
    }

    memset(&phy_abi, 0 , sizeof(rtk_port_phy_ability_t));
    memset(&mac_force_abi, 0 , sizeof(rtk_port_mac_ability_t));
    
    rtk_ret = rtk_port_macForceLink_get(port, &mac_force_abi);
    if(RT_ERR_OK != rtk_ret){
        SDL_MIN_LOG("rtk_port_macForceLink_get return %d\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }
    
    switch (speed_cfg) {
        case SDL_PORT_AUTO_10_100:{
            phy_abi.AutoNegotiation = 1;
            phy_abi.Half_10 = 1;
            phy_abi.Half_100 = 1;
            phy_abi.Full_10 = 1;
            phy_abi.Full_100 = 1;
            mac_force_abi.forcemode = 0;
            mac_force_abi.duplex = 0;
            mac_force_abi.speed = 0;
            mac_force_abi.link = 0; 
        }
        break;

        case SDL_PORT_AUTO_10_100_1000:{
            phy_abi.AutoNegotiation = 1;
            phy_abi.Half_10 = 1;
            phy_abi.Half_100 = 1;
            phy_abi.Full_10 = 1;
            phy_abi.Full_100 = 1;
            phy_abi.Full_1000 = 1;
            mac_force_abi.forcemode = 0;
            mac_force_abi.duplex = 0;
            mac_force_abi.speed = 0;
            mac_force_abi.link = 0;
        }
        break;

        case SDL_PORT_10_FULL:{
            phy_abi.Full_10 = 1;
            mac_force_abi.forcemode = 1;
            mac_force_abi.duplex = 1;
            mac_force_abi.speed = 0;
            mac_force_abi.link = 1;
        }
        break;

        case SDL_PORT_10_HALF:{
            phy_abi.Half_10 = 1;
            mac_force_abi.forcemode = 1;
            mac_force_abi.duplex = 0;
            mac_force_abi.speed = 0;
            mac_force_abi.link = 1;
        }
        break;

        case SDL_PORT_100_FULL:{
            phy_abi.Full_100 = 1;
            mac_force_abi.forcemode = 1;
            mac_force_abi.duplex = 1;
            mac_force_abi.speed = 1;
            mac_force_abi.link = 1;
        }
        break;

        case SDL_PORT_100_HALF:{
            phy_abi.Half_100 = 1;
            mac_force_abi.forcemode = 1;
            mac_force_abi.duplex = 0;
            mac_force_abi.speed = 1;
            mac_force_abi.link = 1;
        }
        break;
#if 1  /* to support 1000M full mode */
        case SDL_PORT_1000_FULL:{
            /* For 1000M, autoneg is always required */
            phy_abi.Full_1000 = 1;
            phy_abi.AutoNegotiation = 1;
            mac_force_abi.forcemode = 0;
            mac_force_abi.duplex = 0;
            mac_force_abi.speed = 0;
            mac_force_abi.link = 0;
        }
        break;
#endif
        //case SDL_PORT_1000_FULL:
        default:{
            rt = CS_E_PARAM;
            goto END;
        }
    }

    /*Jianguang added: 
    For chip 8365, if Auto nego is enable, turn on MDX auto mode,else use force mode
    for fixing link up long time issue.
    */
    rtk_chip_id_get(&chip_id);
    if(chip_id != RTK_CHIP_8305){
        if (phy_abi.AutoNegotiation == 0) {
            rtk_ret = rtk_port_phyMdx_set(port, PHY_FORCE_MDIX_MODE);
            if(RT_ERR_OK != rtk_ret){
                SDL_MIN_LOG("rtk_port_phyMdx_set return %d\n", rtk_ret);
                rt = CS_E_ERROR;
                goto END;
            }
        } 
        else {
            rtk_ret = rtk_port_phyMdx_set(port, PHY_AUTO_CROSSOVER_MODE);
            if(RT_ERR_OK != rtk_ret){
                SDL_MIN_LOG("rtk_port_phyMdx_set return %d\n", rtk_ret);
                rt = CS_E_ERROR;
                goto END;
            }
        }
    }
    
    if (__port_cfg[port].flow_ctrl_en)
        phy_abi.FC = 1;

    rtk_ret = rtk_port_macForceLink_set(port, &mac_force_abi);
    if(RT_ERR_OK != rtk_ret){
        SDL_MIN_LOG("rtk_port_macForceLink_set return %d\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }
    

    rtk_ret = rtk_port_phyAutoNegoAbility_set(port, &phy_abi);
    if(RT_ERR_OK != rtk_ret){
        SDL_MIN_LOG("rtk_port_phyAutoNegoAbility_set return %d\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }

    __port_cfg[port].port_cfg = speed_cfg;

END:
    return rt;
}

cs_status epon_request_onu_port_status_get(
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_int32                  device_id,
    CS_IN  cs_int32                  llidport,
    CS_IN  cs_port_id_t              port_id,
    CS_OUT cs_sdl_port_speed_cfg_t   *speed_cfg
)
{
    if (NULL == speed_cfg) {
        SDL_MIN_LOG("speed_cfg is a NULL pointer\n", );
        return CS_E_PARAM;
    }

    UNI_PORT_CHECK(port_id);

    *speed_cfg = __port_cfg[port_id -1].port_cfg;

    return CS_E_OK;
}

cs_status epon_request_onu_port_admin_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_sdl_port_admin_t       admin
)
{
    rtk_port_t      port;
    rtk_enable_t    enable;
    rtk_api_ret_t   rtk_ret  = 0;
    cs_status       rt = CS_E_OK;
    
    UNI_PORT_CHECK(port_id);
    
    port = L2P_PORT(port_id);
    if (__port_cfg[port].uni_admin == admin) {
        goto END;
    }

    if (admin == SDL_PORT_ADMIN_DOWN) {
        enable = 0;
    } else {
        enable = 1;
    }
    
    rtk_ret = rtk_port_adminEnable_set(port, enable);
    if(RT_ERR_OK != rtk_ret){
        SDL_MIN_LOG("rtk_port_adminEnable_set return %d\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }

    __port_cfg[port].uni_admin = admin;

END:
    return rt;
}


cs_status epon_request_onu_port_admin_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_sdl_port_admin_t      *admin
)
{
    if (NULL == admin) {
        SDL_MIN_LOG("admin is a NULL pointer\n", );
        return CS_E_PARAM;
    }
    
    UNI_PORT_CHECK(port_id);
    
    *admin = __port_cfg[port_id-1].uni_admin;

    return CS_E_OK;
}

cs_status epon_request_onu_port_stats_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_boolean                read_clear,
    CS_OUT cs_sdl_port_uni_stats_t  *uni_stats
)
{
    rtk_port_t           port;
    rtk_stat_port_cntr_t port_cnt;
    cs_uint32            val[2] = {0, 0};
    rtk_api_ret_t        rtk_ret = 0;
    cs_status            rt = CS_E_OK;
    
    if (NULL == uni_stats) {
        SDL_MIN_LOG("uni_stats is a NULL pointer!!\n");
        rt = CS_E_PARAM;
        goto END;
    }
    
    if ((port_id < CS_UNI_PORT_ID1) || (port_id > CS_UPLINK_PORT)) {
        SDL_MIN_LOG("port_id %d is not supported\n", port_id);
        rt = CS_E_PARAM;
        goto END;
    }

    memset((cs_uint8 *)uni_stats, 0, sizeof(cs_sdl_port_uni_stats_t));
    memset((cs_uint8*)&port_cnt, 0, sizeof(port_cnt));

    if (port_id != CS_DOWNLINK_PORT) {
        port = L2P_PORT(port_id);
        rtk_ret = rtk_stat_port_getAll(port, &port_cnt);
        if (RT_ERR_OK != rtk_ret) {
            SDL_MIN_LOG("rtk_stat_port_getAll return %d\n", rtk_ret);
            rt = CS_E_ERROR;
            goto END;
        }

        uni_stats->rxucfrm_cnt = port_cnt.ifInUcastPkts;
        if(port_cnt.etherStatsMcastPkts > port_cnt.ifOutMulticastPkts){
            uni_stats->rxmcfrm_cnt = port_cnt.etherStatsMcastPkts - port_cnt.ifOutMulticastPkts;
        }else{
            uni_stats->rxmcfrm_cnt = 0;
        }
        if(port_cnt.etherStatsBcastPkts > port_cnt.ifOutBrocastPkts){
            uni_stats->rxbcfrm_cnt = port_cnt.etherStatsBcastPkts - port_cnt.ifOutBrocastPkts;
        }else{
            uni_stats->rxbcfrm_cnt = 0;
        }
        uni_stats->rxpausefrm_cnt = port_cnt.dot3InPauseFrames;
        uni_stats->rxoamfrm_cnt = port_cnt.inOampduPkts;
        uni_stats->rxcrcerrfrm_cnt = port_cnt.dot3StatsFCSErrors;
        uni_stats->fcs_errors = port_cnt.dot3StatsFCSErrors;
        uni_stats->rxunknownopfrm_cnt = port_cnt.dot3ControlInUnknownOpcodes;
        uni_stats->rxundersizefrm_cnt = port_cnt.etherStatsUndersizePkts;
        uni_stats->rxoversizefrm_cnt = port_cnt.etherStatsOversizePkts;
        uni_stats->rxjabberfrm_cnt = port_cnt.etherStatsJabbers;
        uni_stats->rxbyte_cnt = port_cnt.ifInOctets;
        uni_stats->txbyte_cnt = port_cnt.ifOutOctets;
        uni_stats->txucfrm_cnt = port_cnt.ifOutUcastPkts;
        uni_stats->txmcfrm_cnt = port_cnt.ifOutMulticastPkts;
        uni_stats->txbcfrm_cnt = port_cnt.ifOutBrocastPkts;
        uni_stats->txoamfrm_cnt = port_cnt.outOampduPkts;
        uni_stats->txpausefrm_cnt = port_cnt.dot3OutPauseFrames;
        uni_stats->txsinglecolfrm_cnt = port_cnt.dot3StatsSingleCollisionFrames;
        uni_stats->txmulticolfrm_cnt = port_cnt.dot3StatsMultipleCollisionFrames;
        uni_stats->txlatecolfrm_cnt = port_cnt.dot3StatsLateCollisions;
        uni_stats->txexesscolfrm_cnt = port_cnt.etherStatsCollisions;        
    } 
    else {
        aal_uni_mib_get(AAL_MIB_RxUCPktCnt, read_clear, &val[1], &val[0]);
        uni_stats->rxucfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxMCFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->rxmcfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxBCFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->rxbcfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxOAMFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->rxoamfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxPauseFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->rxpausefrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxUnKnownOCFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->rxunknownopfrm_cnt = val[0];
        
        aal_uni_mib_get(AAL_MIB_RxCrcErrFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->rxcrcerrfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxUndersizeFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->rxundersizefrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxRuntFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->rxruntfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxOvSizeFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->rxoversizefrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxJabberFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->rxjabberfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxStatsFrm64Oct, read_clear, &val[1], &val[0]);
        uni_stats->rxstatsfrm64_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxStatsFrm65to127Oct, read_clear, &val[1], &val[0]);
        uni_stats->rxstatsfrm65_127_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxStatsFrm128to255Oct, read_clear, &val[1], &val[0]);
        uni_stats->rxstatsfrm128_255_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxStatsFrm256to511Oct, read_clear, &val[1], &val[0]);
        uni_stats->rxstatsfrm256_511_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxStatsFrm512to1023Oct, read_clear, &val[1], &val[0]);
        uni_stats->rxstatsfrm512_1023_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxStatsFrm1024to1518Oct, read_clear, &val[1], &val[0]);
        uni_stats->rxstatsfrm1024_1518_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxStatsFrm1519toMaxOct, read_clear, &val[1], &val[0]);
        uni_stats->rxstatsfrm1519_Max_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_RxByteCount, read_clear, &val[1], &val[0]);
        uni_stats->rxbyte_cnt = *(cs_uint64 *)val; // 64 bits

        aal_uni_mib_get(AAL_MIB_RxFrmCount, read_clear, &val[1], &val[0]);
        uni_stats->rxfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxUCPktCnt, read_clear, &val[1], &val[0]);
        uni_stats->txucfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxMCFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->txmcfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxBCFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->txbcfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxOAMFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->txoamfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxPauseFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->txpausefrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxCrcErrFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->txcrcerrfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxOvSizeFrmCnt, read_clear, &val[1], &val[0]);
        uni_stats->txoversizefrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxSingleColFrm, read_clear, &val[1], &val[0]);
        uni_stats->txsinglecolfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxMultiColFrm, read_clear, &val[1], &val[0]);
        uni_stats->txmulticolfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxLateColFrm, read_clear, &val[1], &val[0]);
        uni_stats->txlatecolfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxExessColFrm, read_clear, &val[1], &val[0]);
        uni_stats->txexesscolfrm_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxEStatsFrm64Oct, read_clear, &val[1], &val[0]);
        uni_stats->txstatsfrm64_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxEStatsFrm65to127Oct, read_clear, &val[1], &val[0]);
        uni_stats->txstatsfrm65_127_cnt = val[0];
        
        aal_uni_mib_get(AAL_MIB_TxEStatsFrm128to255Oct, read_clear, &val[1], &val[0]);
        uni_stats->txstatsfrm128_255_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxEStatsFrm256to511Oct, read_clear, &val[1], &val[0]);
        uni_stats->txstatsfrm256_511_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxEStatsFrm512to1023Oct, read_clear, &val[1], &val[0]);
        uni_stats->txstatsfrm512_1023_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxEStatsFrm1024to1518Oct, read_clear, &val[1], &val[0]);
        uni_stats->txstatsfrm1024_1518_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxEStatsFrm1519toMaxOct, read_clear, &val[1], &val[0]);
        uni_stats->txstatsfrm1519_Max_cnt = val[0];

        aal_uni_mib_get(AAL_MIB_TxByteCount, read_clear, &val[1], &val[0]);
        uni_stats->txbyte_cnt = *(cs_uint64 *)val; // 64 bits

        aal_uni_mib_get(AAL_MIB_TxFrmCount, read_clear, &val[1], &val[0]);
        uni_stats->txfrm_cnt = val[0];

    }
    
END:
    return rt;
}

cs_status epon_request_onu_port_stats_clr(
    CS_IN cs_callback_context_t             context,
    CS_IN cs_int32                          device_id,
    CS_IN cs_int32                          llidport,
    CS_IN cs_port_id_t                      port_id
)
{
    cs_status rt = CS_E_OK;
    rtk_port_t port;
    rtk_api_ret_t   rtk_ret  = 0;
    cs_aal_uni_cfg_t uni_cfg;
    cs_aal_uni_cfg_mask_t uni_msk;

    if ((port_id < CS_UNI_PORT_ID1) || (port_id > CS_UPLINK_PORT)) {
        SDL_MIN_LOG("port_id %d is not supported\n", port_id);
        rt = CS_E_PARAM;
        goto END;
    }

    if (port_id == CS_DOWNLINK_PORT) {
        uni_msk.u32 = 0;
        uni_msk.s.mib_clr_all = 1;
        uni_cfg.mib_clr_all = 1;
        rt = aal_uni_mac_cfg_set(uni_msk, uni_cfg);
        if (rt) {
            SDL_MIN_LOG("aal_uni_mac_cfg_set return %d\n", rt);
            goto END;
        }
    } 
    else {
        port = L2P_PORT(port_id);
        rtk_ret = rtk_stat_port_reset(port);
        if (RT_ERR_OK != rtk_ret) {
            SDL_MIN_LOG("rtk_stat_port_reset return %d\n", rtk_ret);
            rt = CS_E_ERROR;
            goto END;
        }
    }
    
END:
    return rt;
}

cs_status epon_request_onu_port_link_status_get(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_port_link_status_t    *link_status
)
{
    rtk_port_t     port;
    rtk_data_t     uni_link;
    rtk_data_t     speed;
    rtk_data_t     duplex;
    rtk_data_t     nego;
    rtk_api_ret_t  rtk_ret  = 0;
    
    if (NULL == link_status) {
        SDL_MIN_LOG("link_status is NULL pointer\n");
        return CS_E_PARAM;
    }

    UNI_PORT_CHECK(port_id);
    port = L2P_PORT(port_id);
    rtk_ret = rtk_port_phyStatus_get(port, &uni_link, &speed, &duplex, &nego);
    if (RT_ERR_OK != rtk_ret) {
        SDL_MIN_LOG("rtk_port_phyStatus_get return %d\n", rtk_ret);
        return CS_E_ERROR;
    }
    
    if (uni_link) {
        *link_status = SDL_PORT_LINK_STATUS_UP;
    } 
    else {
        *link_status = SDL_PORT_LINK_STATUS_DOWN;
    }
    
    return CS_E_OK;
}

/* This func is to set ONU GE port's RX flow-control and switch's uplink port's flow-control.
 * TX flow-control is not touched.
 */
cs_status uni_ge_rx_flow_ctrl_set(cs_boolean enable)
{
    cs_status rc = CS_E_OK;
    rtk_port_mac_ability_t mac_ability;
    rtk_mode_ext_t ext_mode;

    //set CS8030 UNI rx port flow control
    rc = aal_rx_flow_control_set(enable);
    if (rc) {
        SDL_MIN_LOG("In function:%s,line:%d invoke aal_rx_flow_control_set fail!\n", __FUNCTION__, __LINE__);
        goto end;
    }

    // set uplink port flow control
    rtk_port_macForceLinkExt0_get(&ext_mode, &mac_ability);
    mac_ability.forcemode = 1;
    mac_ability.txpause = enable;
    rc = rtk_port_macForceLinkExt0_set(ext_mode, &mac_ability);
    if (rc) {
        SDL_MIN_LOG("In function:%s,line:%d invoke rtk_port_macForceLinkExt0_set fail!\n", __FUNCTION__, __LINE__);
        goto end;
    }

end:
    return rc;

}

cs_status uni_ge_rx_flow_ctrl_get(cs_boolean *enable)
{
    if (NULL == enable) {
        SDL_MIN_LOG("In %s, enable is NULL pointer\n", __FUNCTION__);
        return CS_E_PARAM;
    }

    aal_rx_flow_control_get(enable);

    return CS_E_OK;

}


cs_status epon_request_onu_port_flow_ctrl_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_boolean                enable
)
{
    cs_aal_flow_control_t cfg[4];
    cs_status rc = CS_E_OK;
    rtk_port_mac_ability_t mac_ability;
    rtk_mode_ext_t ext_mode;
    rtk_port_t port;
    rtk_rate_t us_rate;
    rtk_enable_t pIfg_include;
    rtk_enable_t pFc_enable;
    rtk_port_phy_ability_t advertisement;
    cs_boolean internal_enable;

    if (enable > 1) {
        SDL_MIN_LOG("In %s, port_id %d  enable(%d) is not supported\n", __FUNCTION__, port_id, enable);
        rc = CS_E_PARAM;
        goto end;
    }

    if (port_id == CS_UPLINK_PORT || port_id == CS_DOWNLINK_PORT) {
        rc = uni_ge_rx_flow_ctrl_set(enable);
        goto end;
    }

    UNI_PORT_CHECK(port_id);

    memset(cfg, 0, sizeof(cs_aal_flow_control_t)*4);

    port = port_id - 1;
    
    if (__port_cfg[port].flow_ctrl_en == enable) {
        SDL_MIN_LOG("set flow control(port: %d): is same( %d) as before\n", port, enable);
        goto end;
    }

    /*turn on port flow control*/
    rc = rtk_port_macForceLink_get(port, &mac_ability);
    if (rc) {
        SDL_MIN_LOG("In function:%s,line:%d invoke rtk_port_macForceLink_get fail!\n", __FUNCTION__, __LINE__);
        goto end;
    }
    mac_ability.rxpause = enable;
    mac_ability.txpause = enable;
    rc = rtk_port_macForceLink_set(port, &mac_ability);
    if (rc) {
        SDL_MIN_LOG("In function:%s,line:%d invoke rtk_port_macForceLink_set fail!\n", __FUNCTION__, __LINE__);
        goto end;
    }

    /*turn on/off uplink port flow control*/
    rc = rtk_port_macForceLinkExt0_get(&ext_mode, &mac_ability);
    if (rc) {
        SDL_MIN_LOG("In function:%s,line:%d invoke rtk_port_macForceLinkExt0_get fail!\n", __FUNCTION__, __LINE__);
        goto end;
    }
    mac_ability.forcemode = 1;
    
    __port_cfg[port].flow_ctrl_en = enable;

    if (__port_cfg[0].flow_ctrl_en || __port_cfg[1].flow_ctrl_en ||
            __port_cfg[2].flow_ctrl_en || __port_cfg[3].flow_ctrl_en) {
        cfg[0].mem_fc.tx_en = 1;
        mac_ability.rxpause = 1;
    } else {
        cfg[0].mem_fc.tx_en = 0;
        mac_ability.rxpause = 0;
    }

    rc = rtk_port_macForceLinkExt0_set(ext_mode, &mac_ability);
    if (rc) {
        SDL_MIN_LOG("In function:%s,line:%d invoke rtk_port_macForceLinkExt0_set fail!\n", __FUNCTION__, __LINE__);
        goto end;
    }


    /*set bm flow control threshold, turn on CS8030 UNI port flow control*/
    cfg[0].fc_msk.u = 0;
    cfg[0].fc_msk.s.mem = 1;

    /*get internal flow ctrl, and keep it untouch*/
    uni_ge_rx_flow_ctrl_get(&internal_enable);
    cfg[0].mem_fc.rx_en = internal_enable;

    rc = aal_flow_control_set(cfg);
    if (rc) {
        SDL_MIN_LOG("In function:%s,line:%d invoke aal_flow_control_set fail!\n", __FUNCTION__, __LINE__);
        goto end;
    }

    rc = rtk_port_phyAutoNegoAbility_get(port, &advertisement);
    if (rc) {
        SDL_MIN_LOG("In function:%s,line:%d invoke rtk_port_macForceLinkExt0_get fail!\n", __FUNCTION__, __LINE__);
        goto end;
    }
    //SDL_MIN_LOG("set flow control %d enable %d\n", port, enable);

    advertisement.FC = enable;
    rc = rtk_port_phyAutoNegoAbility_set(port, &advertisement);
    if (rc) {
        SDL_MIN_LOG("In function:%s,line:%d invoke rtk_port_phyAutoNegoAbility_set fail!\n", __FUNCTION__, __LINE__);
        goto end;
    }
    rc = rtk_rate_igrBandwidthCtrlRate_get(port, &us_rate, &pIfg_include, &pFc_enable);
    if (rc) {
        SDL_MIN_LOG("In function:%s,line:%d invoke rtk_rate_igrBandwidthCtrlRate_get fail!\n", __FUNCTION__, __LINE__);
        goto end;
    }
    pFc_enable = enable;

    rc = rtk_rate_igrBandwidthCtrlRate_set(port, us_rate, pIfg_include, pFc_enable);
    if (rc) {
        SDL_MIN_LOG("In function:%s,line:%d invoke rtk_rate_igrBandwidthCtrlRate_set fail!\n", __FUNCTION__, __LINE__);
        goto end;
    }

end:
    return rc;

}


cs_status epon_request_onu_port_flow_ctrl_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_boolean                *enable
)
{
    cs_status rc = CS_E_OK;
    rtk_port_t port;

    if (NULL == enable) {
        rc = CS_E_PARAM;
        goto end;
    }

    if (port_id == CS_UPLINK_PORT || port_id == CS_DOWNLINK_PORT) {
        rc = uni_ge_rx_flow_ctrl_get(enable);
        goto end;
    }

    UNI_PORT_CHECK(port_id);

    port = port_id - 1;
    *enable = __port_cfg[port].flow_ctrl_en;

end:
    return rc;
}

cs_status epon_request_onu_port_ds_rate_limit_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_sdl_policy_t           *rate
)
{
    cs_status rc = CS_E_OK;
    rtk_port_t port;
    rtk_rate_t ds_rate;
    rtk_enable_t pIfg_include;
    cs_aal_rate_limit_t shp;

    if (NULL == rate) {
        SDL_MIN_LOG("In %s, rate is NULL pointer\n", __FUNCTION__);
        return CS_E_PARAM;
    }
//diag_printf("port id = 0x%x\n",port_id);
    if (port_id != CS_DOWNLINK_PORT) {
        UNI_PORT_CHECK(port_id);
        
        if (rate->rate > max_port_traffic_rate) {
            SDL_MIN_LOG("In %s, rate->rate is error!\n", __FUNCTION__);
            return CS_E_PARAM;
        }   
        
        port = (rtk_port_t)(port_id - 1);

        rc = rtk_rate_egrBandwidthCtrlRate_get(port, &ds_rate, &pIfg_include);
        if (rc) {
            SDL_MIN_LOG("In function:%s,line:%d invoke rtk_rate_egrBandwidthCtrlRate_get fail!\n", __FUNCTION__, __LINE__);
            goto end;
        }
        if (rate->enable) {
            ds_rate = rate->rate;
        } else {
            ds_rate = max_port_traffic_rate;
        }
        rc = rtk_rate_egrBandwidthCtrlRate_set(port, ds_rate, pIfg_include);
        if (rc) {
            SDL_MIN_LOG("In function:%s,line:%d invoke rtk_rate_egrBandwidthCtrlRate_set fail!\n", __FUNCTION__, __LINE__);
            goto end;
        }

        __ds_rate_limit[port].enable = rate->enable;
        __ds_rate_limit[port].rate = rate->rate;
        __ds_rate_limit[port].cbs = rate->cbs;
        __ds_rate_limit[port].ebs = rate->ebs;
    }else{
        memset(&shp, 0, sizeof(shp));

        shp.ctrl.s.enable = rate->enable;
        // diag_printf(" rate %d, CBS %d\n", rate->rate, rate->cbs);

        shp.rate = rate->rate;
        shp.cbs = rate->cbs / 1000;

        if (shp.cbs < MIN_BURST_SIZE) {
            shp.cbs = MIN_BURST_SIZE;
        }

        if (shp.cbs > MAX_BURST_SIZE) {
            shp.cbs = MAX_BURST_SIZE;
        }

        if (aal_flow_shaper_set(AAL_PORT_ID_GE, &shp)) {
            rc = CS_E_PARAM;
            goto end;
        }

        __pir = rate->cbs;

    }
    

end:
    return rc;
}

cs_status epon_request_onu_port_ds_rate_limit_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_sdl_policy_t           *rate
)
{
    rtk_port_t port;
    cs_aal_rate_limit_t shp;
    if (NULL == rate) {
        SDL_MIN_LOG("In %s, rate is NULL pointer\n", __FUNCTION__);
        return CS_E_PARAM;
    }

    if (port_id != CS_DOWNLINK_PORT) {
        UNI_PORT_CHECK(port_id);

        port = (rtk_port_t)(port_id - 1);

        rate->enable = __ds_rate_limit[port].enable ;
        rate->rate = __ds_rate_limit[port].rate;
        rate->cbs = __ds_rate_limit[port].cbs;
    } else {
        aal_flow_shaper_get(AAL_PORT_ID_GE, &shp);
        rate->enable = shp.ctrl.s.enable;
        rate->rate = shp.rate;
        rate->cbs = __pir;
    }

    return CS_E_OK;

}

cs_status epon_request_onu_port_policy_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_sdl_policy_t           *policy
)
{
    cs_status rc = CS_E_OK;
    rtk_port_t port;
    rtk_rate_t us_rate;
    rtk_enable_t pIfg_include;
    rtk_enable_t pFc_enable;

    if (NULL == policy) {
        SDL_MIN_LOG("In %s, NULL pointer!\n", __FUNCTION__);
        return CS_E_PARAM;
    }

    UNI_PORT_CHECK(port_id);    

    if (policy->rate > max_port_traffic_rate) {
        SDL_MIN_LOG("In %s, policy->rate is error!\n", __FUNCTION__);
        return CS_E_PARAM;
    }
    port = (rtk_port_t)(port_id - 1);

    rc = rtk_rate_igrBandwidthCtrlRate_get(port, &us_rate, &pIfg_include, &pFc_enable);
    if (rc) {
        SDL_MIN_LOG("In function:%s,line:%d invoke rtk_rate_igrBandwidthCtrlRate_get fail!\n", __FUNCTION__, __LINE__);
        return rc;
    }

    if (policy->enable) {
        us_rate = policy->rate ;
    } else {
        us_rate = max_port_traffic_rate;
    }
    pIfg_include = 1;
    /*****jianguang changed**********/
    pFc_enable = __port_cfg[port].flow_ctrl_en;

    rc = rtk_rate_igrBandwidthCtrlRate_set(port, us_rate, pIfg_include, pFc_enable);
    if (rc) {
        SDL_MIN_LOG("In function:%s,line:%d invoke rtk_rate_igrBandwidthCtrlRate_set fail!\n", __FUNCTION__, __LINE__);
        return rc;
    }

    __us_rate_limit[port].enable = policy->enable;
    __us_rate_limit[port].rate = policy->rate;
    __us_rate_limit[port].cbs = policy->cbs;
    __us_rate_limit[port].ebs = policy->ebs;


    return CS_E_OK;
}
cs_status epon_request_onu_port_policy_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_sdl_policy_t          *policy
)
{
    rtk_port_t port;

    if (NULL == policy) {
        SDL_MIN_LOG("In %s, NULL pointer!\n", __FUNCTION__);
        return CS_E_PARAM;
    }

    UNI_PORT_CHECK(port_id);    

    port = (rtk_port_t)(port_id - 1);

    policy->enable = __us_rate_limit[port].enable;
    policy->rate = __us_rate_limit[port].rate;
    policy->cbs  = __us_rate_limit[port].cbs;
    policy->ebs = __us_rate_limit[port].ebs;


    return CS_E_OK;

}


cs_status epon_request_onu_port_traffic_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_sdl_port_drop_cfg_t   is_drop
)
{
    cs_status rc = CS_E_OK;

    cs_aal_spec_pkt_ctrl_msk_t pkt_ctrl_msk;
    cs_aal_spec_pkt_ctrl_t pkt_ctrl;
    cs_aal_port_id_t port;
    memset(&pkt_ctrl, 0 , sizeof(pkt_ctrl));


    if (port_id == CS_PON_PORT_ID) {
        if (is_drop == __drop_pon) {
            goto end;
        }
        __drop_pon = is_drop;
        port = AAL_PORT_ID_PON;
    } else {
        if (is_drop == __drop_ge) {
            goto end;
        }
        __drop_ge = is_drop;
        port = AAL_PORT_ID_GE;
    }

    if (is_drop == SDL_PORT_NO_DROP) {

        pkt_ctrl_msk.u32 = 0;
        pkt_ctrl_msk.s.dpid = 1;
        pkt_ctrl_msk.s.pri = 1;
        pkt_ctrl.pri = 1;
        pkt_ctrl.dpid.dst_op = AAL_SPEC_DST_FE;

        
        rc = aal_special_pkt_behavior_set(port, AAL_PKT_NORMAL, pkt_ctrl_msk, &pkt_ctrl);
        if (rc) {
            goto end;
        }

        pkt_ctrl_msk.u32 = 0;
        pkt_ctrl_msk.s.pri = 1;
        pkt_ctrl.pri = 30;
        rc = aal_special_pkt_behavior_set(port, AAL_PKT_8021X, pkt_ctrl_msk, &pkt_ctrl);
        if (rc) {
            goto end;
        }

    } else if (is_drop == SDL_PORT_DROP_ALL) {
        pkt_ctrl_msk.u32 = 0;
        pkt_ctrl_msk.s.dpid = 1;
        pkt_ctrl_msk.s.pri = 1;
        pkt_ctrl.pri = 30;
        pkt_ctrl.dpid.dst_op = AAL_SPEC_DST_DROP;
        
        rc = aal_special_pkt_behavior_set(port, AAL_PKT_NORMAL, pkt_ctrl_msk, &pkt_ctrl);
        if (rc) {
            goto end;
        }

        pkt_ctrl_msk.u32 = 0;
        pkt_ctrl_msk.s.pri = 1;
        pkt_ctrl.pri = 1;
        rc = aal_special_pkt_behavior_set(port, AAL_PKT_8021X, pkt_ctrl_msk, &pkt_ctrl);
        if (rc) {
            goto end;
        }
    }


end:
    return rc;
}

cs_status epon_request_onu_port_traffic_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_sdl_port_drop_cfg_t    *is_drop
)
{

    if (NULL == is_drop) {
        SDL_MIN_LOG("In %s, NULL pointer!\n", __FUNCTION__);
        return CS_E_PARAM;
    }
    if (port_id == CS_PON_PORT_ID) {
        *is_drop = __drop_pon;
    } else if (port_id == CS_UNI_PORT_ID1 || port_id == CS_DOWNLINK_PORT) {
        *is_drop = __drop_ge;
    } else {
        return CS_E_PARAM;
    }
    return CS_E_OK;
}

cs_status uni_min_pkt_size_set(
    CS_IN cs_uint16 min
)
{
    cs_status rc = CS_E_OK;
    cs_aal_uni_cfg_t uni_cfg;
    cs_aal_uni_cfg_mask_t uni_msk;

    if (min > 127 || min < 64)
        return CS_E_PARAM;

    uni_msk.u32 = 0;
    uni_msk.s.min_size = 1;
    uni_cfg.min_size = min;

    rc = aal_uni_mac_cfg_set(uni_msk, uni_cfg);

    return rc;
}

cs_status uni_min_pkt_size_get(
    CS_OUT cs_uint16 *min
)
{
    cs_status rc = CS_E_OK;
    cs_aal_uni_cfg_t uni_cfg;

    if (NULL == min) {
        SDL_MIN_LOG("In %s, NULL pointer!\n", __FUNCTION__);
        return CS_E_PARAM;
    }

    aal_uni_mac_cfg_get(&uni_cfg);
    *min = uni_cfg.min_size;

    return rc;
}


cs_status sdl_port_init(
    CS_IN const sdl_init_cfg_t   *cfg
)
{
    cs_callback_context_t     context;
    rtk_port_phy_ability_t    phy_abi;
    cs_sdl_port_speed_cfg_t   speed_cfg;
    cs_aal_uni_cfg_t          uni_cfg;
    cs_aal_uni_cfg_mask_t     uni_msk;
    cs_aal_uni_cfg_ext_t      uni_mac_ext;
    cs_aal_uni_cfg_ext_mask_t uni_mac_ext_mask;
    rtk_mode_ext_t            mod;
    rtk_port_mac_ability_t    mac_abi;
    rtk_switch_maxPktLen_t    switch_mtu = MAXPKTLEN_1552B;
    rtk_api_ret_t             rtk_ret = 0; 
	rtk_chip_id_t             chip_id;
    cs_status                 rt = CS_E_OK;
    cs_uint8                  index;
    cs_aal_mirr_msk_t         cfg_msk;
    cs_aal_mirr_cfg_t         mir_cfg;
    
    
    if (NULL == cfg) {
        SDL_MIN_LOG(" NULL pointer!\n", );
        return CS_E_PARAM;
    }

    memset(&cfg_msk, 0, sizeof(cs_aal_mirr_msk_t));
    memset(&mir_cfg, 0, sizeof(cs_aal_mirr_cfg_t));
    
    cfg_msk.u32 = 0;
    cfg_msk.s.gl_mirror_en =1 ;
    
    mir_cfg.gl_mirror_en = 1;
    aal_mirror_cfg_set(cfg_msk, &mir_cfg);
    
    /*config Lynxd uni port MTU to __SDL_PORT_MTU_MAX*/
    uni_msk.u32 = 0;
    uni_msk.s.max_size = 1;
    uni_cfg.max_size = __SDL_PORT_MTU_MAX;

    rt = aal_uni_mac_cfg_set(uni_msk, uni_cfg);
    if (rt) {
        SDL_MIN_LOG("aal_uni_mac_cfg_set return %d\n", rt);
        goto END;
    }
    
    /*config switch MTU to __SDL_PORT_MTU_MAX*/
    rtk_ret = rtk_switch_maxPktLen_set(switch_mtu);
    if (RT_ERR_OK != rtk_ret) {
        SDL_MIN_LOG("rtk_switch_maxPktLen_set return %d\n", rt);
        rt = CS_E_ERROR;
        goto END;
    }

    phy_abi.AutoNegotiation = 1;
    phy_abi.Half_10 = 1;
    phy_abi.Half_100 = 1;
    phy_abi.Full_10 = 1;
    phy_abi.Full_100 = 1;
    phy_abi.Full_1000 = 1;

    rtk_chip_id_get(&chip_id);

/* added by wujm@2016-07-25,rtk_chip_id_get can't get 8365 when it's 811G */
#if (PRODUCT_CLASS == PRODUCTS_GT811G)        
    chip_id = RTK_CHIP_8365;
#else
    chip_id = RTK_CHIP_8305;
#endif

    if(chip_id == RTK_CHIP_8305){
        speed_cfg = SDL_PORT_AUTO_10_100;
        phy_abi.Full_1000 = 0;
        max_port_traffic_rate= 100000;
    } else {
        speed_cfg = SDL_PORT_AUTO_10_100_1000;
        phy_abi.Full_1000 = 1;
        max_port_traffic_rate=1000000;
    }
    
    phy_abi.FC = 0;
    phy_abi.AsyFC = 0;


    for (index=0; index<UNI_PORT_MAX; index++) {
        __port_cfg[index].port_cfg = speed_cfg;
        __port_cfg[index].uni_admin = SDL_PORT_ADMIN_UP;
        __port_cfg[index].flow_ctrl_en = 0;
        __us_rate_limit[index].enable = 0;
        __us_rate_limit[index].rate = max_port_traffic_rate;
        __us_rate_limit[index].cbs = 10000;
        __us_rate_limit[index].ebs = 1000;
        __ds_rate_limit[index].enable = 0;
        __ds_rate_limit[index].rate = max_port_traffic_rate;
        __ds_rate_limit[index].cbs = 10000;
        __ds_rate_limit[index].ebs = 1000;
        __sc_rate_limit[index].enable = 0;
        __sc_rate_limit[index].rate = max_port_traffic_rate;
        __sc_rate_limit[index].cbs = 10000;
        __sc_rate_limit[index].ebs = 1000;


        rtk_ret = rtk_port_phyAutoNegoAbility_set(index, &phy_abi);
        if(RT_ERR_OK != rtk_ret){
            SDL_MIN_LOG("rtk_port_phyAutoNegoAbility_set return %d\n", rtk_ret);
            rt =  CS_E_ERROR;
            goto END;
        }


        //epon_request_onu_port_status_set(context, 0, 0, CS_UNI_PORT_ID1+index, speed_cfg);

        /*By default, flow control is disable*/
        rt = epon_request_onu_port_flow_ctrl_set(context, 0, 0, 1 + index, 0);
        if(rt){
            SDL_MIN_LOG("epon_request_onu_port_flow_ctrl_set return %d\n", rt);
            goto END;
        }
        
        /*By default, loopback is disable*/
        rt = epon_request_onu_port_lpbk_set(context, 0, 0, 1 + index, 0);
        if(rt){
            SDL_MIN_LOG("epon_request_onu_port_lpbk_set return %d\n", rt);
            goto END;
        }
        
    }

    /*Port Isolation is enabled by default*/
    rt = epon_request_onu_port_isolation_set(context, 0, 0, 1);
    if(rt){
        SDL_MIN_LOG("epon_request_onu_port_isolation_set return %d\n", rt);
        goto END;
    }

    /*By default, PON port MTU is 1536*/
    rt = epon_request_onu_port_mtu_set(context, 0, 0, CS_PON_PORT_ID, 1536);
    if(rt){
        SDL_MIN_LOG("epon_request_onu_port_mtu_set return %d\n", rt);
        goto END;
    }
     /*********if storm control is disable, flooding packet from all 4 UNI ports will be limited by this rate limiter**/
     /*********it should be bigger than 400Mbps************************************************************************/   
    rtk_ret = rtk_rate_shareMeter_set(PORT_STORM_METER_ID_END, 1000000, DISABLED);
    if(RT_ERR_OK != rtk_ret){
        SDL_MIN_LOG("rtk_rate_shareMeter_set return %d\n", rtk_ret);
        rt =  CS_E_ERROR;
        goto END;
    }
    
    uni_mac_ext_mask.u32 = 0;
    uni_mac_ext.txfifo_thrshld = sdl_int_cfg.uni_tx_fifo_threshould;
    uni_mac_ext.fifo_thrshld_low = sdl_int_cfg.uni_tx_fifo_ready_low;
    uni_mac_ext.fifo_thrshld_high = sdl_int_cfg.uni_tx_fifo_ready_high;
    uni_mac_ext.phy_mode = sdl_int_cfg.uni_phy_mod;
    uni_mac_ext.tag_exclude = sdl_int_cfg.mtu_tag_exclude;

    uni_mac_ext_mask.s.txfifo_thrshld = TRUE;
    uni_mac_ext_mask.s.fifo_thrshld_low = TRUE;
    uni_mac_ext_mask.s.fifo_thrshld_high = TRUE;
    uni_mac_ext_mask.s.phy_mode = TRUE;
    uni_mac_ext_mask.s.tag_exclude = TRUE;
    
    rt = aal_uni_mac_cfg_ext_set(uni_mac_ext_mask, uni_mac_ext);
    if(rt){
        SDL_MIN_LOG("aal_uni_mac_cfg_ext_set return %d\n", rt);
        goto END;
    }
    
    if (sdl_int_cfg.uni_phy_mod) {
        mod = MODE_EXT_RGMII;
        mac_abi.forcemode = 1;
        mac_abi.speed = 2;
        mac_abi.duplex = 1;
        mac_abi.link = 1;
        mac_abi.nway = 1;
        mac_abi.txpause = 0;
        mac_abi.rxpause = 0;
        
        rtk_ret = rtk_port_macForceLinkExt0_set(mod, &mac_abi);
        if(RT_ERR_OK != rtk_ret){
            SDL_MIN_LOG("rtk_port_macForceLinkExt0_set return %d\n", rtk_ret);
            rt =  CS_E_ERROR;
            goto END;
        }
        
        /* added by wujm@2016-07-25,811G need LED_MODE_2 */      
        if(chip_id == RTK_CHIP_8305)
        {
            rtk_ret = rtk_led_mode_set(LED_MODE_1);
        }
        else
        {
            rtk_ret = rtk_led_mode_set(LED_MODE_2);
        }
        if(RT_ERR_OK != rtk_ret){
            SDL_MIN_LOG("rtk_led_mode_set return %d\n", rtk_ret);
            rt =  CS_E_ERROR;
            goto END;
        }
        
        /*bug30336, make led work for both 100M and 10M*/
        if(chip_id == RTK_CHIP_8305)
        {
            rtk_ret = rtk_led_groupConfig_set(LED_GROUP_1, LED_CONFIG_LINK_ACT);
        }
        else
        {
            rtk_ret = rtk_led_groupConfig_set(LED_GROUP_2, LED_CONFIG_LINK_ACT);
        }
        
        if(RT_ERR_OK != rtk_ret){
            SDL_MIN_LOG("rtk_led_groupConfig_set return %d\n", rtk_ret);
            rt =  CS_E_ERROR;
            goto END;
        }
        
        rtk_ret = rtk_port_rgmiiDelayExt0_set(0, 5);
        if(RT_ERR_OK != rtk_ret){
            SDL_MIN_LOG("rtk_port_rgmiiDelayExt0_set return %d\n", rtk_ret);
            rt =  CS_E_ERROR;
            goto END;
        }
    }


    rtk_ret = rtk_int_control_set(INT_TYPE_LINK_STATUS, 1);
    if(RT_ERR_OK != rtk_ret){
        SDL_MIN_LOG("rtk_int_control_set return %d\n", rtk_ret);
        rt =  CS_E_ERROR;
        goto END;
    }
    
    sdl_int_handler_reg(SDL_INT_UNI_LINK_CHANGE, __sdl_port_int_process);
    
#ifdef HAVE_SDL_CTC
    cs_polling_handler_reg(__sdl_port_auto_neg_polling_handler);
#endif

END:
    return rt;
    
}

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_storm_control_set                  */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SAL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_storm_ctrl_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_sdl_rl_mod_e           storm_mod,
    CS_IN cs_sdl_storm_ctrl_e       storm_type,
    CS_IN cs_sdl_policy_t           *rate
)
{
    cs_status rc = CS_E_OK;
    cs_aal_rate_limit_t storm;
    cs_aal_rate_limit_msk_t st_msk;
    rtk_port_t port;
    rtk_rate_t rtk_rate;

    if (NULL == rate) {
        SDL_MIN_LOG("In %s, rate is NULL pointer\n", __FUNCTION__);
        rc = CS_E_PARAM;
        goto end;
    }

    if (!((storm_mod == SDL_RL_MOD_BPS) || (storm_mod == SDL_RL_MOD_PPS))) {
        SDL_MIN_LOG("In %s, storm_mod %d is not supported\n", __FUNCTION__, storm_mod);
        rc = CS_E_PARAM;
        goto end;
    }

    if ((storm_type < SDL_STORM_TYPE_BC) || (storm_type > SDL_STORM_TYPE_DISABLE)) {
        SDL_MIN_LOG("In %s, storm_type %d is not supported\n", __FUNCTION__, storm_type);
        rc = CS_E_PARAM;
        goto end;
    }
    if (port_id != CS_DOWNLINK_PORT) {
        UNI_PORT_CHECK(port_id); 
        
        port = (rtk_port_t)(port_id - 1);
        rtk_rate = ((rtk_rate_t)rate->rate) / STORM_GRANULARITY * STORM_GRANULARITY;
        if(rtk_rate < MIN_STORM_RATE){
            rtk_rate = MIN_STORM_RATE;
        }

        rc = rtk_rate_shareMeter_set(PORT_STORM_METER_ID_START + port, rtk_rate, ENABLED);
        if (rc) {
            goto end;
        }
        switch (storm_type) {
        case SDL_STORM_TYPE_DISABLE:
            rc = rtk_storm_controlRate_set(port, STORM_GROUP_BROADCAST, PORT_STORM_METER_ID_END, 1, MODE1);
            if (rc) {
                goto end;
            }
            rc = rtk_storm_controlRate_set(port, STORM_GROUP_UNKNOWN_MULTICAST, PORT_STORM_METER_ID_END, 1, MODE1);
            if (rc) {
                goto end;
            }
            rc = rtk_storm_controlRate_set(port, STORM_GROUP_UNKNOWN_UNICAST, PORT_STORM_METER_ID_END, 1, MODE1);
            if (rc) {
                goto end;
            }
            break;
        case SDL_STORM_TYPE_BC:
            rc = rtk_storm_controlRate_set(port, STORM_GROUP_BROADCAST, PORT_STORM_METER_ID_START + port, 1, MODE1);
            if (rc) {
                goto end;
            }
            rc = rtk_storm_controlRate_set(port, STORM_GROUP_UNKNOWN_MULTICAST, PORT_STORM_METER_ID_END, 1, MODE1);
            if (rc) {
                goto end;
            }
            rc = rtk_storm_controlRate_set(port, STORM_GROUP_UNKNOWN_UNICAST, PORT_STORM_METER_ID_END, 1, MODE1);
            if (rc) {
                goto end;
            }
            break;

        case SDL_STORM_TYPE_BC_UMC:

            rc = rtk_storm_controlRate_set(port, STORM_GROUP_BROADCAST, PORT_STORM_METER_ID_START + port, 1, MODE1);
            if (rc) {
                goto end;
            }
            rc = rtk_storm_controlRate_set(port, STORM_GROUP_UNKNOWN_MULTICAST, PORT_STORM_METER_ID_START + port, 1, MODE1);
            if (rc) {
                goto end;
            }
            rc = rtk_storm_controlRate_set(port, STORM_GROUP_UNKNOWN_UNICAST, PORT_STORM_METER_ID_END, 1, MODE1);
            break;
        case SDL_STORM_TYPE_BC_UMC_UUC:
            rc = rtk_storm_controlRate_set(port, STORM_GROUP_BROADCAST, PORT_STORM_METER_ID_START + port, 1, MODE1);
            if (rc) {
                goto end;
            }
            rc = rtk_storm_controlRate_set(port, STORM_GROUP_UNKNOWN_MULTICAST, PORT_STORM_METER_ID_START + port, 1, MODE1);
            if (rc) {
                goto end;
            }
            rc = rtk_storm_controlRate_set(port, STORM_GROUP_UNKNOWN_UNICAST, PORT_STORM_METER_ID_START + port, 1, MODE1);
            if (rc) {
                goto end;
            }
            break;
        default:
            break;
        }
        __storm_type[port] = storm_type;
        __sc_rate_limit[port].rate = rate->rate;
    } else {
        memset(&storm, 0, sizeof(storm));
        st_msk.u = 0;
        storm.ctrl.s.enable = rate->enable;
        storm.ctrl.s.rlmode = storm_mod;
        storm.rate = rate->rate;
        storm.cbs = MAX_BURST_SIZE;
        st_msk.s.rlmode = 1;
        st_msk.s.rate = 1;
        st_msk.s.cbs = 1;
        st_msk.s.enable = 1;

        rc = aal_flow_rate_limit_set(AAL_RATE_LIMIT_GE_UMC, &st_msk, &storm);
        if (rc) {
            SDL_MIN_LOG("In function:%s,line:%d invoke aal_flow_rate_limit_set fail!\n", __FUNCTION__, __LINE__);
            goto end;
        }

        rc = aal_flow_storm_ctrl_set(AAL_PORT_ID_GE, (cs_aal_storm_ctrl_mod_t)storm_type);
        if (rc) {
            SDL_MIN_LOG("In function:%s,line:%d invoke aal_flow_storm_ctrl_set fail!\n", __FUNCTION__, __LINE__);
            goto end;
        }
    }
end:
    return rc;
}

/****************************************************************************/
/* $rtn_hdr_start  epon_request_onu_port_storm_control_get                  */
/* CATEGORY   : Device                                                      */
/* ACCESS     : Public                                                      */
/* BLOCK      : SAL                                                         */
/* CHIP       : 8030                                                        */
cs_status epon_request_onu_port_storm_ctrl_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_sdl_rl_mod_e          *storm_mod,
    CS_OUT cs_sdl_storm_ctrl_e      *storm_type,
    CS_OUT cs_sdl_policy_t          *rate
)
{
    cs_aal_rate_limit_t storm;
    cs_aal_storm_ctrl_mod_t type = AAL_STORM_CTRL_MODE_DIS;
    cs_aal_rate_limit_msk_t rl_msk;
    cs_status rc = CS_E_OK;


    if (!storm_mod || !storm_type || !storm_type) {
        SDL_MIN_LOG("In %s,  Input parameter is NULL pointer\n", __FUNCTION__);
        rc = CS_E_PARAM;
        goto end;
    }

    if (port_id != CS_DOWNLINK_PORT) {
        UNI_PORT_CHECK(port_id);    

        rate->rate = __sc_rate_limit[port_id -1].rate;
        *storm_type = __storm_type[port_id -1];
        *storm_mod = SDL_RL_MOD_BPS;

    } else {
        rl_msk.u = 0;
        rl_msk.s.rate = 1;

        rc = aal_flow_rate_limit_get(AAL_RATE_LIMIT_GE_UMC, &rl_msk, &storm);
        if (rc) {
            goto end;
        }
        rc = aal_flow_storm_ctrl_get(AAL_PORT_ID_GE, &type);
        if (rc) {
            goto end;
        }

        *storm_mod = (cs_sdl_rl_mod_e)storm.ctrl.s.rlmode;
        *storm_type = (cs_sdl_storm_ctrl_e)type;

        rate->enable = storm.ctrl.s.enable;
        rate->rate = storm.rate;
    }
end:
    return rc;
}



cs_status epon_request_onu_port_isolation_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                enable
)
{
    rtk_port_t     port;
    rtk_portmask_t portmask;
    rtk_api_ret_t  rtk_ret  = 0;
    cs_port_id_t   port_id;
    cs_status      rt = CS_E_OK;
    
    if (enable > 1) {
        SDL_MIN_LOG("error param\n");
        rt = CS_E_PARAM;
        goto END;
    }

    for (port_id = CS_UNI_PORT_ID1; port_id <= UNI_PORT_MAX; port_id++) {
        port = L2P_PORT(port_id);
        if (enable) {
            portmask.bits[0] = 0xf0;
        } else {
            portmask.bits[0] = 0xff;
        }

        rtk_ret = rtk_port_isolation_set(port, portmask);
        if (RT_ERR_OK != rtk_ret) {
            SDL_MIN_LOG("rtk_port_isolation_set return %d\n", rtk_ret);
            rt = CS_E_ERROR;
            goto END;
        }

    }

    __port_isolation = enable;

END:
    return rt;
}

cs_status epon_request_onu_port_isolation_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_OUT cs_boolean               *enable
)
{
    if (NULL == enable) {
        SDL_MIN_LOG(" enable is a NULL pointer\n");
        return CS_E_PARAM;
    }

    *enable = __port_isolation;

    return CS_E_OK;
}

cs_status epon_request_onu_port_hello_mac_set(
    CS_IN cs_callback_context_t      context,
    CS_IN cs_int32                   device_id,
    CS_IN cs_int32                   llidport,
    CS_IN cs_port_id_t               port_id,
    CS_IN cs_mac_t             *mac
)
{
    cs_aal_pkt_spec_udf_msk_t msk;
    cs_aal_pkt_spec_udf_t     cfg;
    cs_status                 ret = CS_E_OK;

    if (NULL == mac) {
        SDL_MIN_LOG("In %s, mac is NULL pointer\n", __FUNCTION__);
        return CS_E_PARAM;
    }

    if (port_id > CS_VOIP_PORT_ID) {
        SDL_MIN_LOG("In %s, port_id %d is not supported\n", __FUNCTION__, port_id);
        return CS_E_PARAM;
    }

    memset(&cfg, 0, sizeof(cs_aal_pkt_spec_udf_t));
    msk.u32 = 0;
    msk.s.hello_da = 1;
    cfg.hello_da.addr[0] = mac->addr[0];
    cfg.hello_da.addr[1] = mac->addr[1];
    cfg.hello_da.addr[2] = mac->addr[2];
    cfg.hello_da.addr[3] = mac->addr[3];
    cfg.hello_da.addr[4] = mac->addr[4];
    cfg.hello_da.addr[5] = mac->addr[5];

    switch (port_id) {
    case CS_PON_PORT_ID:
        ret = aal_pkt_special_udf_set(AAL_PORT_ID_PON, msk, &cfg);
        break;

    case CS_UNI_PORT_ID1:
        ret = aal_pkt_special_udf_set(AAL_PORT_ID_GE, msk, &cfg);
        break;

    case CS_MGMT_PORT_ID:
        ret = aal_pkt_special_udf_set(AAL_PORT_ID_MII0, msk, &cfg);
        break;

    default:
        SDL_MIN_LOG("In %s, port_id %d is not supported\n", __FUNCTION__, port_id);
        ret = CS_E_PARAM;
    }

    return ret;
}

#if 0

/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_dot1p_remark_set        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_dot1p_remark_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_pri_remark_t          *remark
)

{
    rtk_port_t port;
    rtk_api_ret_t rt;
    cs_uint8 index, index1;
    cs_uint8 filter_id;
    cs_uint8 port_msk = 0;
    cs_uint8 temp_pri;

    cs_status ret = CS_E_OK;
    if (!remark) {
        SDL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        ret = CS_E_PARAM;
        goto end;
    }
    if (port_id < CS_UNI_PORT_ID1 || port_id > UNI_PORT_MAX) {
        SDL_MIN_LOG("In %s, port_id %d is not supported!\n", __FUNCTION__, port_id);
        return CS_E_PARAM;
    }
    port = (rtk_port_t)(port_id - 1);

    for (index = 0; index < 8; index++) {
        filter_id = __acl_occupy[port][index];
        if (filter_id != 0) {
            rt = rtk_filter_igrAcl_cfg_del(filter_id);
            if (rt) {
                ret = (cs_status)rt;
                goto end;
            }
            __acl_occupy[port][index] = 0;
            for (index1 = 0; index1 < 4; index1++) {
                if (__acl_occupy[index1][index] == filter_id) {
                    port_msk |= (1 << index1);
                    temp_pri = __dot1p_remark[index1].priority[index];
                }
            }
            if (port_msk != 0) {
                ret = __add_pri_remark_acl(filter_id, port_msk, index, temp_pri);
                if (ret) {
                    goto end;
                }
            } else {
                __acl_avl &= ~(1 << filter_id);

            }
        }

    }

    if (remark->en == 1) {

        for (index = 0; index < 8; index++) {

            if ((index&7) == remark->priority[index&7]) {
                continue;
            }
            port_msk = 0;
            for (index1 = 0; index1 < 4; index1++) {

                //cs_printf("index1 = %d,index = %d, filter_id = %d\n", index1, index,filter_id);
                if (__acl_occupy[index1][index&7] != 0) {
                    filter_id = __acl_occupy[index1][index&7];
                    // cs_printf("__dot1p_remark[%d].priority[%d] = %d\n",index1, index&7,__dot1p_remark[index1].priority[index&7]);
                    //cs_printf("remark->priority[%d] = %d\n",index&7,remark->priority[index&7]);

                    if (__dot1p_remark[index1].priority[index&7] == remark->priority[index&7]) {
                        rt = rtk_filter_igrAcl_cfg_del(filter_id);
                        if (rt) {
                            ret = (cs_status)rt;
                            goto end;
                        }
                        port_msk |= (1 << index1);
                    }
                }
            }
            if (port_msk == 0) {
                filter_id = __check_free_id();
            }
            port_msk |= (1 << port);
            //__acl_avl = 0;
            temp_pri = remark->priority[index&7];
            ret = __add_pri_remark_acl(filter_id, port_msk, index, temp_pri);
            if (ret) {
                goto end;
            }
            __acl_avl |= 1 << filter_id;

            __acl_occupy[port][index&7] = filter_id;
        }
    }

    __dot1p_remark[port].en = remark->en;

    // cs_printf("set remark->en = %d\n",remark->en);
    for (index = 0; index < 8; index++) {
        __dot1p_remark[port].priority[index] = remark->priority[index];
    }
end:
    return ret;
}


/****************************************************************/
/* $rtn_hdr_start  epon_request_onu_dot1p_remark_get        */
/* CATEGORY   : Device                                          */
/* ACCESS     : Public                                          */
/* BLOCK      : SAL                                             */
/* CHIP       : 8030                                            */
cs_status epon_request_onu_dot1p_remark_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device_id,
    CS_IN cs_int32                llidport,
    CS_IN cs_port_id_t              port_id,
    CS_OUT cs_pri_remark_t         *remark
)

{

    rtk_port_t port;
    cs_uint8 index;
    cs_status ret = CS_E_OK;

    if (!remark) {
        SDL_MIN_LOG("NULL pointer. FILE: %s, LINE: %d", __FILE__, __LINE__);
        ret = CS_E_PARAM;
        goto end;
    }
    if (port_id < CS_UNI_PORT_ID1 || port_id > UNI_PORT_MAX) {
        SDL_MIN_LOG("In %s, port_id %d is not supported!\n", __FUNCTION__, port_id);
        ret = CS_E_PARAM;
        goto end;
    }
    port = (rtk_port_t)(port_id - 1);

    remark->en = __dot1p_remark[port].en;
// cs_printf("get remark->en = %d\n",remark->en);
    for (index = 0; index < 8; index++) {
        remark->priority[index] = __dot1p_remark[port].priority[index];
    }
end:
    return ret;

}
#endif

cs_status epon_request_onu_port_mirror_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              mirror_port,
    CS_IN cs_uint32                 rx_port_msk,
    CS_IN cs_uint32                 tx_port_msk
)
{
    rtk_portmask_t rx_msk;
    rtk_portmask_t tx_msk;
    rtk_port_t     port;
    rtk_api_ret_t  rtk_ret  = 0;
    cs_status      rt = CS_E_OK;
    
    if ((mirror_port < CS_UNI_PORT_ID1) ||
            ((mirror_port > UNI_PORT_MAX) && (mirror_port != CS_UPLINK_PORT))) {
        SDL_MIN_LOG("port_id %d is not supported!\n", mirror_port);
        rt = CS_E_PARAM;
        goto END;
    }

    if (rx_port_msk > MIRROR_MAX_PORT_MASK) {
        SDL_MIN_LOG("rx_port_msk: %d is not supported!\n", rx_port_msk);
        rt = CS_E_PARAM;
        goto END;
    }

    if (tx_port_msk > MIRROR_MAX_PORT_MASK) {
        SDL_MIN_LOG("tx_port_msk %d is not supported!\n", tx_port_msk);
        rt = CS_E_PARAM;
        goto END;
    }

    /*rx_port_msk should equal to tx_port_msk if neither of them is 0*/
    if ((rx_port_msk != tx_port_msk) && (tx_port_msk != 0) && (rx_port_msk != 0)) {
        SDL_MIN_LOG("port_mask is set uncorrectly!\n");
        rt = CS_E_PARAM;
        goto END;
    }

    /*mirror port != source port*/
    port = L2P_PORT(mirror_port);
    if (mirror_port == CS_UPLINK_PORT) {
        if (((tx_port_msk &(1 << 4)) > 0) || ((rx_port_msk &(1 << 4)) > 0)) {
            SDL_MIN_LOG("mirror_port %d should not be included into source port!\n", mirror_port);
            rt = CS_E_PARAM;
            goto END;
        }
    }
    else{
        if (((tx_port_msk &(1 << port)) > 0) || ((rx_port_msk &(1 << port)) > 0)) {
            SDL_MIN_LOG("mirror_port %d should not be included into source port!\n", , mirror_port);
            rt = CS_E_PARAM;
            goto END;
        }
    }

    MSK_MIRROR_TO_SWITCH(tx_port_msk, tx_msk.bits[0]);
    MSK_MIRROR_TO_SWITCH(rx_port_msk, rx_msk.bits[0]);

    rtk_ret = rtk_mirror_portBased_set(port, &rx_msk, &tx_msk);
    if (RT_ERR_OK != rtk_ret) {
        SDL_MIN_LOG("rtk_mirror_portBased_set return %d\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }

END:
    return rt;

}

cs_status epon_request_onu_port_mirror_get(
    CS_IN  cs_callback_context_t     context,
    CS_IN  cs_int32                  device_id,
    CS_IN  cs_int32                  llidport,
    CS_OUT cs_port_id_t              *mirror_port,
    CS_OUT cs_uint32                 *rx_port_msk,
    CS_OUT cs_uint32                 *tx_port_msk
)
{
    rtk_portmask_t rx_msk;
    rtk_portmask_t tx_msk;
    rtk_port_t     port;
    cs_status rt = CS_E_OK;
    rtk_api_ret_t   rtk_ret  = 0;
    
    if ((NULL == mirror_port) || (NULL == rx_port_msk) || (NULL == tx_port_msk)) {
        SDL_MIN_LOG("NULL pointer\n");
        rt = CS_E_PARAM;
        goto END;
    }

    rtk_ret = rtk_mirror_portBased_get(&port, &rx_msk, &tx_msk);
    if (RT_ERR_OK != rtk_ret) {
        SDL_MIN_LOG("rtk_mirror_portBased_get return %d\n", rtk_ret);
        rt = CS_E_ERROR;
        goto END;
    }

    *mirror_port = P2L_PORT(port);
    MSK_SWITCH_TO_MIRROR(tx_msk.bits[0], *tx_port_msk);
    MSK_SWITCH_TO_MIRROR(rx_msk.bits[0], *rx_port_msk);

END:
    return rt;
}

cs_status epon_request_onu_big_burst_size_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                enable
)
{
    cs_status ret = CS_E_OK;

    if (enable == TRUE) {
        ret = (cs_status)cs_plat_ssp_switch_write(context, 0, 0, BURST_CONTROL_REG, 0x12);
    } else if (enable == FALSE) {
        ret = (cs_status)cs_plat_ssp_switch_write(context, 0, 0, BURST_CONTROL_REG, 0xed);
    } else {
        ret = CS_E_PARAM;
    }
    return ret;
}

cs_status epon_request_onu_big_burst_size_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                *enable
)
{
    cs_status ret = CS_E_OK;
    cs_uint32 valu32;

    if (!enable) {
        ret =  CS_E_PARAM;
        goto end;
    }
    ret = (cs_status)cs_plat_ssp_switch_read(context, 0, 0, BURST_CONTROL_REG, &valu32);
    if (ret) {
        goto end;
    }
    if (valu32 == 0xed) {
        *enable = FALSE;
    } else {
        *enable = TRUE;
    }
end:
    return ret;
}

cs_status epon_request_onu_port_stats_get_pwrsv(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_boolean                read_clear,
    CS_OUT cs_sdl_port_uni_stats_t  *uni_stats
)
{
    return 0;
}

cs_status epon_request_onu_pkt_remote_capture_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                enable
)
{
    cs_aal_port_id_t        port;
    cs_aal_cl_def_fib_data_t    cl_def_rule;
    cl_key_entry_t                   ce_entry;
    cl_fib_data_t                     fibdata;
    cs_status                         ret = CS_E_OK;

    if(enable)
    {
        // enable mirror and set mirror group id in GE and PON classification engine
        for(port = AAL_PORT_ID_GE; port <= AAL_PORT_ID_PON; port++)
        {
            memset(&cl_def_rule, 0, sizeof(cs_aal_cl_def_fib_data_t));
            aal_cl_default_fib_get(port, &cl_def_rule);
            cl_def_rule.cl_mirror_en = 1;
            cl_def_rule.cl_mirror_src_sel = 0;
            cl_def_rule.flowId_sel = AAL_CL_FLOWID_SEL_CL_RSLT;
            cl_def_rule.flowid = AAL_RATE_LIMIT_FLOW_10;
            cl_def_rule.mask.s.dpid = 1;
            cl_def_rule.mask.s.rate_limit = 1;
            ret = aal_cl_default_fib_set(port, &cl_def_rule);
        }
        // set mirror dpid in MA classification engine
        memset( &ce_entry, 0, sizeof(cl_key_entry_t));
        memset( &fibdata, 0, sizeof(cl_fib_data_t));

        ce_entry.entryvalid    = 1;
        ce_entry.keytype = AAL_CL_MIRROR_KEY;    
        ce_entry.u.cl_mirror_key[0].mirror_group_id = AAL_RATE_LIMIT_FLOW_10;
        ce_entry.u.cl_mirror_key[0].mirror_pkt_flag = 1;
        ce_entry.u.cl_mirror_key[0].mask           = CL_KEY_MASK_MIRROR_GROUP; 
        ret = cl_key_entry_set( AAL_FE_MA_ID, 0, &ce_entry);
         
        fibdata.valid             = 1;
        fibdata.permit_filter_dis = 1;
        fibdata.permit            = 1;
        fibdata.permit_pri        = 1;
        fibdata.dpid              = 4;
        fibdata.flowId_sel = AAL_CL_FLOWID_SEL_CL_RSLT;
        fibdata.flowID= AAL_RATE_LIMIT_FLOW_10;
        fibdata.mask              = CL_RSLT_MASK_DPID;
        ret += cl_fib_table_set(AAL_FE_MA_ID, CL_SUB_KEY_NUM*0, &fibdata);
    }
    else
    {
        for(port = AAL_PORT_ID_GE; port <= AAL_PORT_ID_PON; port++)
        {
            memset(&cl_def_rule, 0, sizeof(cs_aal_cl_def_fib_data_t));
            aal_cl_default_fib_get(port, &cl_def_rule);
            cl_def_rule.cl_mirror_en = 0;
            cl_def_rule.cl_mirror_src_sel = 0;
            cl_def_rule.flowId_sel = AAL_CL_FLOWID_SEL_NO;
            cl_def_rule.flowid = 0;
            cl_def_rule.mask.s.dpid = 1;
            cl_def_rule.mask.s.rate_limit = 1;
            ret = aal_cl_default_fib_set(port, &cl_def_rule);
        }   
        memset( &ce_entry, 0, sizeof(cl_key_entry_t));
        memset( &fibdata, 0, sizeof(cl_fib_data_t));
        ret = cl_key_entry_set( AAL_FE_MA_ID, 0, &ce_entry);
        ret += cl_fib_table_set(AAL_FE_MA_ID, CL_SUB_KEY_NUM*0, &fibdata);
    }

    g_pkt_remote_capture_enable = enable;
    return ret;
}

cs_status epon_request_onu_pkt_remote_capture_get(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean              *enable
)
{
    *enable = g_pkt_remote_capture_enable;
	
    return CS_E_OK;
}


cs_status epon_request_onu_phy_loopback_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device_id,
    CS_IN cs_int32                  llidport,
    CS_IN cs_boolean                enable
)
{
    cs_status   rc = CS_E_OK;
    return rc;
}

