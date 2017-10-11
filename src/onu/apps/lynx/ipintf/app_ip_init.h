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
#ifndef _APP_IP_INIT_H_
#define _APP_IP_INIT_H_

#define IPINTF_MAC_AGING_INTERVAL                  (300) /* 300 seconds */
#define IPINTF_ARP_KEEPALIVE_DFT                     (~0) /* disable arp keepalive */
#define IPINTF_ARP_KEEPALIVE_ZTE                     (180) /* 180 seconds */
#define IPINTF_MAC_ENTRY_PER_PORT                 (20)
#define IPINTF_NAME_ETH0        "eth0"
#define IPINTF_NAME_ETH1        "eth1"
#define IPINTF_TM_TICKS                                     (10) /* every 10 seconds */

/* Default local ip address and mask defination */

#ifdef HAVE_ZTE_SIJIE
#define IPINTF_DFT_IP_ADDR  0x02020214   
#define IPINTF_DFT_IP_MASK  0xffffff00   //255.255.255.0
#define IPINTF_DFT_IP_GW  0x02020201   
#else /*END_HAVE_ZTE_SIJIEC*/
#define IPINTF_DFT_IP_ADDR  0xc0a80101   //192.168.1.1
#define IPINTF_DFT_IP_MASK  0xffffff00   //255.255.255.0
#define IPINTF_DFT_IP_GW  0xc0a80101   //192.168.1.1
#endif /*END_NHAVE_ZTE_SIJIEC*/


/* MTU defination */
#define IPINTF_DFT_MTU  1500
#define IPINTF_MIN_MTU   1440
#define IPINTF_MAX_MTU IPINTF_DFT_MTU

typedef struct _ipintf_statistics_t_ {
    cs_uint32 recv;
    cs_uint32 send;
    cs_uint32 drop;	
    cs_uint32 fwd;	
    cs_uint32 ip;	
    cs_uint32 arp;	
}ipintf_statistics_t;

typedef struct _ipintf_mac_entry_t {
    cs_uint8    used;
    cs_uint8    hit;
    cs_uint16  vlanid;
    cs_uint16  dpid;
    cs_uint8    mac[6];
}ipintf_mac_entry_t;

typedef struct _ipintf_port_info_t_ {
    cs_uint8   pri;
    cs_uint16 vlan;
    cs_uint32 port;
    ipintf_statistics_t statistics;
}ipintf_port_info_t;

typedef struct _ipintf_info_t_ {
    cs_uint32 debug;
    cs_uint8 inband;/* default false , ZTE only */
    cs_uint8 pri;
    cs_uint16 vlanid;
    cs_uint8 mac[6];
    cs_uint32 localip;
    cs_uint32 ipmask;
    cs_uint32 remoteip;
    cs_uint32 remotemask;
    cs_uint32 gateway;
    cs_uint32 mtu;
    cs_uint32 ageinterval;
    cs_uint32 keepalive;/* arp */
    cs_uint32 maxport;
    ipintf_port_info_t *port;
    ipintf_mac_entry_t *mactbl;
}ipintf_info_t;


extern ipintf_info_t ipintf_info;
#define MACTBL ipintf_info.mactbl
#define PTABLE ipintf_info.port
#define PORT_STATIS(portId) ipintf_info.port[portId].statistics
#define LOCAL_IPADDR ipintf_info.localip
#define LOCAL_IPMASK ipintf_info.ipmask
#define GATEWAY_IPADDR ipintf_info.gateway
#define IPINTF_MAC_ENTRY_MAX (ipintf_info.maxport * IPINTF_MAC_ENTRY_PER_PORT)

#define IPINTF_STATIS_INC(type, dport) \
    do{\
        if((dport) >= ipintf_info.maxport) {\
            APP_IPINTF_LOG(APP_IPINTF_LOG_DBG, "%s, invalid port %d\n", __func__, (dport));\
            return -1;\
        }\
        IPINTF_STATIS_INC_##type(dport);\
    }while(0)

#define IPINTF_STATIS_INC_DROP(dport) (ipintf_info.port[(dport)].statistics.drop)++
#define IPINTF_STATIS_INC_RECV(dport) (ipintf_info.port[(dport)].statistics.recv)++
#define IPINTF_STATIS_INC_SEND(dport) (ipintf_info.port[(dport)].statistics.send)++
#define IPINTF_STATIS_INC_FWD(dport)  (ipintf_info.port[(dport)].statistics.fwd)++
#define IPINTF_STATIS_INC_ARP(dport)   (ipintf_info.port[(dport)].statistics.arp)++
#define IPINTF_STATIS_INC_IP(dport)      (ipintf_info.port[(dport)].statistics.ip)++



extern const cs_uint8 *app_ipintf_get_my_macaddr(void);

extern void app_ipintf_device_init(ipintf_info_t *eth_info);

#endif/* _APP_IP_INIT_H_ */

