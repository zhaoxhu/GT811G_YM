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
 
Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/

#ifndef _DHCP_H_
#define _DHCP_H_

#define DHCP_OPTION_254_DATA_LEN   12
#define DHCP_ETHER_MACADDR_LEN      6

#define DHCP_INFO_END_OPTION  0xff

#define MAC_HEX_TO_ASC(hex)       ((hex<=9) ? ('0' + hex) : ('a' + hex - 0xa))

#define DHCP_CLIENT_UDP_PORT   68
#define DHCP_SERVER_UDP_PORT   67

typedef struct
{
    cs_uint8 tag;
    cs_uint8 len;
    cs_uint8 data[DHCP_OPTION_254_DATA_LEN];
} __attribute__((packed)) dhcp_option_254_t;

typedef struct 
{
    cs_mac_t          dst; 
    cs_mac_t          src; 
    cs_uint16          ethertype;    
} __attribute__((packed)) dhcp_ether_header_t;

typedef struct 
{
    cs_uint8   version_hl;
    cs_uint8   tos;
    cs_uint16   tot_len;
    cs_uint16  id;
    cs_uint16  frag_off;
    cs_uint8    ttl;
    cs_uint8    protocol;
    cs_uint16  check_sum;
    cs_uint32  src;
    cs_uint32  dst;      
} __attribute__((packed)) dhcp_ip_header_t;

typedef struct 
{
    cs_uint16  src_port; 
    cs_uint16  dst_port; 
    cs_uint16  len;    
    cs_uint16  check_sum; 
} __attribute__((packed)) dhcp_udp_header_t;

typedef struct 
{
    cs_uint8  type; 
    cs_uint8  len;  
    cs_uint8  data[0]; 

} __attribute__((packed)) dhcp_option_hdr_t;

typedef struct 
{
    cs_uint8  op;                     
    cs_uint8  htype;                 
    cs_uint8  hlen;                   
    cs_uint8  hops;                    
    cs_uint32 xid;                  
    cs_uint16 secs;                    
    cs_uint16 flags;                  
    cs_uint32 ciaddr;                 
    cs_uint32 yiaddr;                 
    cs_uint32 siaddr;                  
    cs_uint32 giaddr;                 
    cs_uint8  chaddr[16];              
    cs_uint8  sname[64];             
    cs_uint8  file[128];               
    cs_uint32 magic;                   
    dhcp_option_hdr_t options[0];          
} __attribute__((packed)) dhcp_pkt_header_t;

typedef struct
{
    cs_uint8    msg_type;
    cs_uint8    version;
    cs_uint16   len;
    cs_uint16   port;
    cs_uint16   offset;
    cs_uint16   svlan;
    cs_uint16   cvlan;
    cs_uint16   stpid;
    cs_uint16   ctpid;
    cs_uint16   frame_type;
    cs_uint16   eth_type;
    cs_uint16   tag_num;
    cs_uint16   u2Reserved;
    cs_uint8    data[283];
}dhcp_pakt_t;

void dhcp_enable(cs_uint8 enable);
void dhcp_init();
cs_uint8 dhcp_pkt_recv(cs_pkt_t* hdr);
void dhcp_show();
void dhcp_test(cs_uint8 type);

#endif
