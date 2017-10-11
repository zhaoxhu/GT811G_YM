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

DEFINITIONS:  "DEVICE" means the Cortina Systems?LynxD SDK product.
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
#ifdef HAVE_IP_STACK
#include "plat_common.h"
#include "app_ip_util.h"
#include "app_ip_types.h"
#include "app_ip_adapter.h"

/* Interface to other modules */
cs_status epon_request_onu_ip_config_set(
                                                        cs_uint32 local_ip, 
                                                        cs_uint32 ip_mask, 
                                                        cs_uint32 gw_ip)
{
   
    return app_ipintf_ip_config_set(local_ip, ip_mask, gw_ip);
}
cs_status epon_request_onu_inband_ip_config_set(
                                                    cs_uint8 enable, 
                                                    cs_uint8 pri,
                                                    cs_uint16 vlanid,
                                                    cs_uint32 remote_subnet, 
                                                    cs_uint32 subnet_mask,
                                                    cs_uint32 gw)
{
    return app_ipintf_set_wan_cfg(enable, pri, vlanid, remote_subnet, subnet_mask, gw);
}

cs_status epon_request_onu_ip_mtu_set(cs_uint32 mtu)
{
    return app_ipintf_set_mtu(mtu);
}

cs_status epon_request_onu_ip_config_get(
                                                    cs_uint32 *local_ip, 
                                                    cs_uint32 *mask, 
                                                    cs_uint32 *gw_ip)
{    
    return app_ipintf_get_ip_address(local_ip, mask, gw_ip);
}

cs_status epon_request_onu_inband_ip_config_get(
                                                    cs_uint8 *enable, 
                                                    cs_uint8 *pri,
                                                    cs_uint16 *vlanid,
                                                    cs_uint32 *remote_subnet, 
                                                    cs_uint32 *remote_mask,
                                                    cs_uint32 *gw)
{
    return app_ipintf_get_wan_cfg(enable, pri, vlanid, remote_subnet, remote_mask, gw);
}

cs_status epon_request_onu_ip_mtu_get(cs_uint32 *mtu)
{
    
    return app_ipintf_get_mtu(mtu);
    
}

cs_status epon_request_onu_check_ip_address(
                                                    cs_uint32 ip, 
                                                    cs_uint32 mask, 
                                                    cs_uint32 gw)
{
    return app_ipintf_check_ip_address(ip, mask, gw);
}

cs_status epon_request_onu_arp_keepalive_set(cs_uint32 interval)
{

    return app_ipintf_arp_keepalive_interval_set(interval);
    
}

cs_status epon_request_onu_arp_show(cs_uint8 *host)
{

    app_ipintf_arp_show(host);
    return 0;
    
}
cs_status epon_request_onu_arp_del(cs_uint8 *host)
{

    app_ipintf_arp_del(host);
    return 0;
    
}

static cs_boolean g_ip_set = FALSE;
static cs_uint32  g_curr_ip ;
static cs_uint32  g_curr_mask ;

cs_status epon_request_onu_cur_ip_save(cs_uint32 ip,cs_uint32 mask)
{

    if(!g_ip_set){
        g_ip_set = TRUE;
        g_curr_ip = ip ;
        g_curr_mask = mask ;
    }

    return 0; 
}

cs_status epon_request_onu_cur_ip_get(cs_uint32* ip, cs_uint32* mask)
{

    if(g_ip_set){
        *ip = g_curr_ip;
        *mask = g_curr_mask;
        return CS_OK; 
    }else{
        *ip = 0;
        *mask = 0;
        return CS_E_NOT_FOUND; 
    }


}

cs_status epon_request_onu_ipoe_test_start(
        cs_uint16 port,
        cs_uint16 vlan,
        cs_uint8  mothed,
        cs_int8  *name,
        cs_int8  *pwd,
        cs_int8  *mac,
        cs_uint8 *option60,
        cs_uint8  timeout)
{
    app_dhcp_start(port, vlan, option60,timeout);
    return CS_OK; 
}

cs_status epon_request_onu_ipoe_test_stop(cs_uint16 port)
{
    app_ipoe_stop(port);
    return CS_OK;
}

cs_status epon_request_onu_ipoe_test_result_get(ipoe_state_t *state)
{
    app_dhcp_get_state(state);
    return CS_OK;  
}

#ifdef DHCP_OPTION_PPPOE_PLUS_SUPPORT
cs_status epon_request_onu_dhcp_option_enable(const dhcp_option_enable_t *option)
{
    if(!option)
        return -1;
    
    return dhcp_option_enable(option);
}
cs_status epon_request_onu_bcmp_ani_set(const dhcp_access_node_id_t *ani)
{
    if(!ani)
        return -1;
    
    return bcmp_node_set(ani);
}
cs_status epon_request_onu_dhcp_option_status_get(dhcp_option_enable_t *option)
{
    if(!option)
        return -1;
    
    return dhcp_option_enable_get(option);
}
cs_status epon_request_onu_bcmp_ani_get(dhcp_access_node_id_t *ani)
{
    if(!ani)
        return -1;
    
    return bcmp_node_get(ani);
}

#endif

#ifdef HAVE_ZTE_SIJIE
cs_status epon_request_onu_eoc_vlanip_set(cs_uint16 vid, cs_uint32 ip, cs_uint32 mask, cs_uint32 gw)
{
    eoc_pon_vlanip_set(vid, ip, mask, gw);
    return 0;

}
cs_status epon_request_onu_reset_set(void)
{
    eoc_pon_reset_set();
    return 0;

}
#endif /*END_HAVE_ZTE_SIJIE*/

#ifdef HAVE_TCP_UDP_NETWORK_CARD
//added by zhuxh
cs_status epon_request_onu_eth1_ip_config_set(
                                                        cs_uint32 local_ip, 
                                                        cs_uint32 ip_mask, 
                                                        cs_uint32 gw_ip)
{
 	return ipintf_eth1_add_ip_address(local_ip, ip_mask, gw_ip);
}

cs_status epon_request_onu_eth1_ip_config_get(
                                                    cs_uint32 *local_ip, 
                                                    cs_uint32 *mask, 
                                                    cs_uint32 *gw_ip)
{    
    return network_card_eth1_ip_get(local_ip, mask, gw_ip);
}
#endif

#endif
