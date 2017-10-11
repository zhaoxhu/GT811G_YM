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
#include <pkgconf/system.h>
#include <network.h>
#ifdef CYGPKG_LWIP_IPV4_SUPPORT
#include <lwip/inet.h>
#else

#include <sys/socket.h>
#include <sys/sysctl.h>
#include <sys/sockio.h>
#include <net/if.h>
#include <net/route.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <errno.h>

#endif
#include "plat_common.h"
#include "cs_cmd.h"

#include "osal_api_core.h"
#include "app_ip_init.h"
#include "app_ip_api.h"
#include "app_ip_util.h"
#include "app_ip_mgnt.h"

#ifdef DHCP_OPTION_PPPOE_PLUS_SUPPORT
extern dhcp_option_cb_t dhcp_option_cb;
extern cs_uint8 g_onu_identifier[128];
#endif
#define IPINTF_ALL_PORT (~0)
extern void clt_info_dump(void) ;
extern void clt_info_clear(void);

void app_ipintf_trace_enable(cs_uint32 debug)
{
    ipintf_info.debug = debug;
    cs_printf("%s ip debug\n", ipintf_info.debug?"enable":"disable");
}

/* ip set commands */
cs_uint32 convert_len2_mask(cs_uint32 len) 
{
    cs_uint32 mask = (~0);
    /* len should be lower 32 */
    return (mask >> (32-len)) << (32-len);
}


void app_ipintf_port_statistics(cs_uint32 port)
{
    cs_printf("%-20s: %d\n","port id",port);
    cs_printf("%-20s: %d\n","receive packet",PORT_STATIS(port).recv);
    cs_printf("%-20s: %d\n","recv ip",PORT_STATIS(port).ip);
    cs_printf("%-20s: %d\n","recv arp",PORT_STATIS(port).arp);
    cs_printf("%-20s: %d\n","send packet",PORT_STATIS(port).send);
    cs_printf("%-20s: %d\n","forward packet",PORT_STATIS(port).fwd);
    cs_printf("%-20s: %d\n","drop packet",PORT_STATIS(port).drop);
    cs_printf("\n");
}
void app_ipintf_show_statistics(cs_uint32 port)
{
    if(port == IPINTF_ALL_PORT) {
        cs_uint32 i = 0;
        for(; i<ipintf_info.maxport; i++) {
            app_ipintf_port_statistics(i);
        }
    }
    else {
        if(port >= ipintf_info.maxport) {
            cs_printf("Invalid port id %d\n", port);
            return;
        }
        app_ipintf_port_statistics(port);
    }
}
void app_ipintf_show_status(void)
{
    cs_printf("%-20s: 0x%x\n","ONU ip address",ipintf_info.localip);
    cs_printf("%-20s: 0x%x\n","ONU ip mask",ipintf_info.ipmask);
    cs_printf("%-20s: 0x%x\n","Gateway ip address",ipintf_info.gateway);
    cs_printf("%-20s: %d\n","Mtu",ipintf_info.mtu);
    cs_printf("%-20s: %d\n","Vlan id",ipintf_info.vlanid);
    cs_printf("%-20s: %d\n","Priority",ipintf_info.pri);
    cs_printf("%-20s: %d sec\n","Mac aging timer",ipintf_info.ageinterval);
    cs_printf("%-20s: %u sec\n","Arp keepalive timer",ipintf_info.keepalive);
    cs_printf("%-20s: %s\n","Management mode", ipintf_info.inband?"Inband":"Outband");
    cs_printf("%-20s: 0x%x\n","Remote ip address",ipintf_info.remoteip);
    cs_printf("%-20s: 0x%x\n","Remote ip mask",ipintf_info.remotemask);
}

void ip_show_cmd_help()
{
    cs_printf("show ip status - show ip configuraion status\n");
    cs_printf("show ip counter [port]- show packet statistics(per port)\n");
    cs_printf("show ip arp - show arp table\n");
}
void ip_cmd_help()
{
    cs_printf("ip debug [bitmap]-- enable/disable trace & debug\n");
    cs_printf("ip mtu [mtu] -- set network mtu\n");
    cs_printf("ip keepalive [interval] -- set arp keepalive interval\n");
    cs_printf("ip addr [localip] mask [mask] gw [gateway] -- "
                                                "set local ip address/mask and gateway ip \n");
    cs_printf("ip inband -- set inband/outband mode\n");
    cs_printf("ip arp -- show arp table\n");
    cs_printf("ip status -- show ip interface status\n");
    cs_printf("ip counter -- show ip interface counter\n");
    cs_printf("ip mac -- show mac address table\n");
#ifdef HAVE_ZTE_SIJIE
    cs_printf("eoc reboot-- eoc reset \n");
    cs_printf("eoc vlanip set ip mask gw vid-- eoc vlan ip set\n");
#endif   /*END_HAVE_ZTE_SIJIE*/ 
}
static void ip_inband_cmd_help(void)
{
    cs_printf("ip inband enable localip mask gateway vlan priority\n");
    cs_printf("\t %-10s 0:disable. 1:enable\n", "enable");
    cs_printf("\t %-10s x.x.x.x \n", "localip");
    cs_printf("\t %-10s x.x.x.x \n", "mask");
    cs_printf("\t %-10s x.x.x.x \n", "gateway");
    cs_printf("\t %-10s vlanid(0~4095)\n", "vlan");
    cs_printf("\t %-10s priority(0~7)\n", "priority");
    cs_printf("\t e.g. ip inband 1 192.168.0.2 255.255.255.0 192.168.0.1 100 7\n");
    cs_printf("\t e.g. ip inband 1 192.168.0.2/24 192.168.0.1 100 7\n");
    cs_printf("\t e.g. ip inband 0\n");
}
void dhcp_option_help(void)
{
    cs_printf("ip dhcp 1 -- send DHCPV4 to PON port\n");
    cs_printf("ip dhcp 2 -- send DHCPV4 (with option 82 overwrite) to PON port\n");
    cs_printf("ip dhcp 3 -- send PPPOE plus to PON port\n");
    cs_printf("ip dhcp 4 -- send DHCPV6 to PON port\n");
    cs_printf("ip dhcp 5 -- send BCMP packet to PON port\n");
    cs_printf("ip dhcp 6 -- BCMP node set init\n");
    cs_printf("ip dhcp 7 -- DHCP/PPPoE/BCMP enable\n");
    cs_printf("ip dhcp 8 -- DHCP/PPPoE/BCMP init\n");
}
#ifdef DHCP_OPTION_PPPOE_PLUS_SUPPORT

extern cs_int32 dhcp_option82_add(cs_pkt_t *pkt);
extern cs_int32 pppoe_plus_add(cs_pkt_t *pkt);
extern void dhcp_option_init();
extern cs_int32 dhcp_option_enable(const dhcp_option_enable_t * enable);

extern cs_int32 dhcpv4_add(cs_pkt_t *pkt);
extern cs_int32 dhcpv6_add(cs_pkt_t *pkt);
extern cs_uint32 cli_bcmp_l2_hdr_build(cs_uint8 *data);
extern cs_uint8 bcmp_pkt_proc(cs_pkt_t *pkt);




cs_uint8 dhcp_packet_op82[] = {
0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x0b,0x82,0x01,0xfc,0x42,0x08,0x00,0x45,0x00,
0x01,0x47,0xa8,0x36,0x00,0x00,0xfa,0x11,0x17,0x70,0x00,0x00,0x00,0x00,0xff,0xff,
0xff,0xff,0x00,0x44,0x00,0x43,0x01,0x33,0x90,0x28,0x01,0x01,0x06,0x00,0x00,0x00,
0x3d,0x1d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0b,0x82,0x01,0xfc,0x42,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x63,0x82,0x53,0x63,0x35,0x01,0x01,0x3d,0x07,0x01,
0x00,0x0b,0x82,0x01,0xfc,0x42,0x32,0x04,0x00,0x00,0x00,0x00,0x37,0x04,0x01,0x03,
0x06,0x2a,0x52,0x19,0x01,0x15,0x30,0x30,0x61,0x61,0x62,0x62,0x30,0x30,0x30,0x30,
0x30,0x65,0x20,0x31,0x2f,0x31,0x2f,0x31,0x20,0x45,0x50,0x02,0x00,0xff,0x00,0x00,
0x00,0x00,0x00,0x00,0x00
};
 cs_uint8 pppoe_packet[] = {
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x10,0xA4,0x97,0xC1,0xD9,0x88,0x63,0x11,0x09,
0x00,0x00,0x00,0x0C,0x01,0x03,0x00,0x04,0x01,0x00,0x00,0x00,0x01,0x01,0x00,0x00};
//0x33,0x33,0x00,0x01,0x00,0x02,
 cs_uint8 dhcpv6_packet[] = {
0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x0b,0x82,0x01,0xfc,0x42,0x86,0xdd,0x60,0x00,
0x00,0x00,0x00,0x7a,0x11,0x01,0xff,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x00,0x02,0xfe,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0xcc,0x87,
0x19,0xd6,0xc5,0xa8,0x46,0x99,0x02,0x22,0x02,0x23,0x00,0x7a,0xf0,0xb1,0x02,0x74,
0x89,0x67,0x00,0x08,0x00,0x02,0x02,0xbc,0x00,0x01,0x00,0x0e,0x00,0x01,0x00,0x01,
0x19,0x30,0x8c,0x34,0x08,0x9e,0x01,0x31,0x69,0x8a,0x00,0x03,0x00,0x0c,0x0e,0x08,
0x9e,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x27,0x00,0x24,0x00,0x0a,
0x53,0x48,0x2d,0x4c,0x2d,0x53,0x57,0x41,0x4e,0x47,0x08,0x68,0x6f,0x6e,0x67,0x6b,
0x6f,0x6e,0x67,0x07,0x63,0x6f,0x72,0x74,0x69,0x6e,0x61,0x05,0x6c,0x6f,0x63,0x61,
0x6c,0x00,0x00,0x10,0x00,0x0e,0x00,0x00,0x01,0x37,0x00,0x08,0x4d,0x53,0x46,0x54,
0x20,0x35,0x2e,0x30,0x00,0x06,0x00,0x08,0x00,0x18,0x00,0x17,0x00,0x11,0x00,0x27
};
#endif
static sal_cmd_result_t ip_inband_cmd_proc(int argc , char **argv)
{
    cs_uint32 localip, mask, gw, enable,pri, vlan;
    if(argc < 2){
        ip_inband_cmd_help();
        return SAL_CMD_OK;
    }
      
    enable = atoi(argv[1]);
    if(enable == 0) {
        /* enable outband */
        epon_request_onu_ip_config_set(IPINTF_DFT_IP_ADDR, IPINTF_DFT_IP_MASK, 0);
        epon_request_onu_inband_ip_config_set(APP_IPINTF_OUTBNAD_MGNT, 0, 0, 0, 0,0);
        cs_printf("Set to outband mode\n");
        return SAL_CMD_OK;
    }
    if(argc == 7) {
        localip = ntohl(inet_addr(argv[2]));
        mask = ntohl(inet_addr(argv[3]));
        gw = ntohl(inet_addr(argv[4]));
        vlan = atoi(argv[5]);
        pri = atoi(argv[6]);
    }
    else if(argc == 6) {
        cs_uint8 flash = '/', len = 0, mask_len = 0;
        char *p = NULL;
        
        p = strchr(argv[2], flash);
        if(p == NULL) {
            cs_printf("invalid parameter\n");
            return SAL_CMD_FAIL;
        }
        len = p - argv[2];
        if(len >= 16) {
            cs_printf("invalid parameter : %s\n", argv[2]);
            return SAL_CMD_FAIL;
        }
        
        *(p) = 0;
        localip = ntohl(inet_addr(argv[2]));
        mask_len = atoi(++p);

        mask = convert_len2_mask(mask_len);
        gw = ntohl(inet_addr(argv[3]));
        vlan = atoi(argv[4]);
        pri = atoi(argv[5]);
    }   

    cs_printf("localip :0x%x\n", localip);
    cs_printf("mask :0x%x\n", mask);
    cs_printf("gw :0x%x\n", gw);
    cs_printf("vlan :0x%x\n", vlan);
    cs_printf("pri :0x%x\n", pri);
    if(app_ipintf_check_ip_address(localip, mask, gw) == 0 && 
                                                    (vlan< 4096)&&
                                                    (pri>=0 && pri<=7)) {
        epon_request_onu_ip_config_set(localip, mask, gw);
        epon_request_onu_inband_ip_config_set(APP_IPINTF_INBNAD_MGNT, pri, vlan, 0, 0,0);
        cs_printf("Set to inband mode\n");
        return 0;
    }
    else {
        ip_inband_cmd_help();
    }
    return 1;
}
sal_cmd_result_t ip_cmd_proc(int argc , char **argv)
{
    cs_status ret = CS_E_OK;
        argc--;
    argv++;
    if(!strcmp(argv[0], "keepalive") && (argc == 2)) {
        ret = epon_request_onu_arp_keepalive_set(atoi(argv[1]));
    }
    else if(!strcmp(argv[0], "debug")) {
        if(argc != 2) {
            cs_printf("ip debug [bitmap]: \n\r\te.g. \n\r\tip debug 1 ---dump packet\n\r\tip debug 255 --- dump all\n");
            return SAL_CMD_FAIL;
        }
        app_ipintf_trace_enable(atoi(argv[1]));
    }
    else if(!strcmp(argv[0], "status")) {
        app_ipintf_show_status();
    }
    else if(!strcmp(argv[0], "counter")) {
        if(argc == 1) {
            app_ipintf_show_statistics(IPINTF_ALL_PORT);
        }
        else if(argc == 2) {
            app_ipintf_show_statistics(atoi(argv[1]));
        }
        else {
            cs_printf("invalid parameter\n");
        }
    }
    else if(!strcmp(argv[0], "mac")) {
        app_ipintf_mac_table_dump(IPINTF_ALL_PORT);
    }
    else if(!strcmp(argv[0], "arp")) {
        if(argc == 2 && !strcmp(argv[1], "show")) {
            app_ipintf_arp_show(NULL);
        }
        else if(argc == 3 && !strcmp(argv[1], "del")) {
            app_ipintf_arp_del(argv[2]);
        }
        else {
            cs_printf("Invalid commands, e.g. \n");
            cs_printf("\t%s\n", "ip arp show");
            cs_printf("\t%s\n", "ip arp delete 192.168.l.100");
            return SAL_CMD_FAIL;
        }
    }
    else if(!strcmp(argv[0], "mtu") && (argc == 2)) {
        cs_uint32 mtu;
        
        ret = epon_request_onu_ip_mtu_get(&mtu);
        cs_printf("%20s :%d\n", "current mtu", mtu);
        mtu = atoi(argv[1]);
        cs_printf("%20s :%d\n", "new mtu", mtu);
        ret = epon_request_onu_ip_mtu_set(mtu);
    }
    else if(!strcmp(argv[0], "inband")) {
        return ip_inband_cmd_proc(argc, argv);
    }
    else if(!strcmp(argv[0], "addr")) {
        if(argc == 6) {
            cs_uint32 localip, mask, gw;
            if(strcmp(argv[2], "mask") || strcmp(argv[4], "gw")) {
                return SAL_CMD_FAIL;
            }
            localip = ntohl(inet_addr(argv[1]));
            mask = ntohl(inet_addr(argv[3]));
            gw = ntohl(inet_addr(argv[5]));
            ret = epon_request_onu_ip_config_set(localip, mask, gw);
        }
        else if(argc == 4) {
            cs_uint8 flash = '/', len = 0, mask_len = 0;
            char *p = NULL;
            cs_uint32 localip, mask, gw;
            
            if(strcmp(argv[2], "gw")) {
                return SAL_CMD_FAIL;
            }
            p = strchr(argv[1], flash);
            if(p == NULL) {
                return SAL_CMD_FAIL;
            }
            len = p - argv[1];
            if(len >= 16) {
                return SAL_CMD_FAIL;
            }
            
            *(p) = 0;
            localip = ntohl(inet_addr(argv[1]));
            mask_len = atoi(++p);

            mask = convert_len2_mask(mask_len);
            gw = ntohl(inet_addr(argv[3]));
            ret = epon_request_onu_ip_config_set(localip, mask, gw);
        }
    }
    else if(!strcmp(argv[0], "eoc")) {
        if(argc == 2 && !strcmp(argv[1], "set")) {
        }
#ifdef HAVE_ZTE_SIJIE        
        else if(argc == 2 && !strcmp(argv[1], "reboot")){
            //eoc reboot
            epon_request_onu_reset_set();
        }
        else if(argc == 7 && !strcmp(argv[1], "vlanip")){
            //eoc vlanip set IP mask gw vid
            cs_uint32 localip, mask, gw;
            cs_int16 vlan;
            localip = ntohl(inet_addr(argv[3]));
            mask = ntohl(inet_addr(argv[4]));
            gw = ntohl(inet_addr(argv[5]));
            vlan = ntohl(inet_addr(argv[6]));
            ret = epon_request_onu_eoc_vlanip_set(vlan, localip, mask, gw);
        }
#endif  /*END_HAVE_ZTE_SIJIE*/      
        else {
            ip_cmd_help();
        }
    }
    else {
        ip_cmd_help();
    }

    return ret == CS_E_OK?SAL_CMD_OK:SAL_CMD_FAIL;
    
}

static sal_cmd_reg_struct_t ip_cmd_list[] = 
{
    {ip_cmd_proc,  "debug",     "enable/disable trace & debug",     NULL, 0},
    {ip_cmd_proc,  "mtu",     "set mtu",     NULL, 0},
    {ip_cmd_proc,  "keepalive",     "set keepalive interval",     NULL, 0},
    {ip_cmd_proc,  "addr",         "set local ip address/mask and gateway ip",     NULL, 0},
    {ip_cmd_proc,  "inband",       "set inband/outband mode",     NULL, 0},
    {ip_cmd_proc,  "arp",     "show arp table",     NULL, 0},
    {ip_cmd_proc,  "status",         "show ip interface status",     NULL, 0},
    {ip_cmd_proc,  "counter",         "show ip interface counter",     NULL, 0},
    {ip_cmd_proc,  "mac",         "show mac address table",     NULL, 0},
    {ip_cmd_proc,  "eoc",         "eoc",     NULL, 0},
};


sal_cmd_cfg_t ip_cmd_cfg = {"ip", "ip cmd", ip_cmd_list, sizeof(ip_cmd_list)/sizeof(sal_cmd_reg_struct_t)};


/* show ipintf */
sal_cmd_result_t show_cmd_ip(int argc, char **argv)
{
    argc--;
    argv++;

    if(!strcmp(argv[1], "status") && argc == 2) {
        app_ipintf_show_status();
    }
    else if(!strcmp(argv[1], "counter")) {
        if(argc == 2) {
            app_ipintf_show_statistics(IPINTF_ALL_PORT);
        }
        else if(argc == 3) {
            app_ipintf_show_statistics(atoi(argv[2]));
        }
        else {
            cs_printf("invalid parameter\n");
        }
    }
    else if(!strcmp(argv[1], "arp")) {
        if(argc == 2) {
            app_ipintf_mac_table_dump(IPINTF_ALL_PORT);
        }
        else {
            cs_printf("invalid parameter\n");
        }
    }
    else {
        ip_show_cmd_help();
    }
        
    return SAL_CMD_OK;
}

cs_status ipintf_cmd_reg()
{
    sal_cmd_reg(&ip_cmd_cfg);
    return CS_E_OK;
}
#endif
