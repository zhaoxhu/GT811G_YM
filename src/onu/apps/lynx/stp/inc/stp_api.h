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

#ifndef __STP_API_H
#define __STP_API_H

/* bit mask for STP bridge config*/
#define ONU_MASK_STP_BRIDGE_ENABLE                     0x00000001
#define ONU_MASK_STP_BRIDGE_PRIORITY               0x00000002
#define ONU_MASK_STP_BRIDGE_MAX_AGE                0x00000004
#define ONU_MASK_STP_BRIDGE_HELLO_TIME                0x00000008
#define ONU_MASK_STP_BRIDGE_FORWARD_DELAY      0x00000010
#define ONU_MASK_STP_BRIDGE_FORCE_VERSION      0x00000020
#define ONU_MASK_STP_BRIDGE_HOLD_TIME              0x00000040

#define ONU_MASK_STP_BRIDGE_ALL                0x7f
/* bit mask for port STP config*/
#define ONU_MASK_PORT_STP_PRIORITY                     0x00000001
#define ONU_MASK_PORT_STP_PATH_COST                0x00000002
#define ONU_MASK_PORT_STP_POINT2POINT          0x00000004
#define ONU_MASK_PORT_STP_EDGE                         0x00000008
#define ONU_MASK_PORT_STP_NON_STP              0x00000010

#define ONU_MASK_PORT_STP_ALL                      0x0000001f

typedef enum {
    ONU_SPECIAL_PACKET_PASS = 1,
    ONU_SPECIAL_PACKET_DROP = 2,
    ONU_SPECIAL_PACKET_CPU = 3,
    ONU_SPECIAL_PACKET_SC = 4,
    ONU_SPECIAL_PACKET_NONE = 5
}cs_special_packet_action_t;

typedef struct {
    cs_uint32          mask;
    cs_uint8         enable;
    cs_uint32        bridge_priority;
    cs_uint32        max_age;
    cs_uint32        hello_time;
    cs_uint32        forward_delay;
    cs_int32          force_version;
    cs_uint32        hold_time;
} __attribute__((packed)) cs_onu_stp_config_t;

typedef struct {
    cs_uint32         mask;
    cs_uint8        stp_enable;
    cs_uint8        stp_mode;
    cs_uint16       bridge_prio;
    cs_uint8        bridge_addr[6];
    cs_uint16       root_bridge_prio;
    cs_uint8        root_bridge_addr[6];
    cs_uint32       max_age;
    cs_uint32       hello_time;
    cs_uint32       forward_delay;
    cs_uint64       root_path_cost;
    cs_uint16       root_port;
} __attribute__((packed)) cs_onu_stp_state_config_t;

typedef struct {
    cs_uint32        port;
    cs_uint32        mask;
    cs_uint32        port_priority;
    cs_uint64        admin_pcost;
    cs_uint8         admin_p2p;
    cs_uint8         admin_edge;
    cs_uint8         admin_non_stp;
} __attribute__((packed)) cs_onu_port_stp_config_t;

typedef struct {
    cs_uint8       state;
    cs_uint32      stp_enable;
    cs_uint8       role;
    cs_uint64      path_cost;
    cs_uint32      port_prio;
    cs_uint16      designated_bridge_prio;
    cs_uint8       designated_bridge_addr[6];
    cs_uint8       oper_point2point;
    cs_uint8       oper_edge;
    cs_uint64      rx_cfg_bpdu_cnt;
    cs_uint64      rx_rstp_bpdu_cnt;
    cs_uint64      rx_tcn_bpdu_cnt;
    cs_uint64      tx_cfg_bpdu_cnt;
    cs_uint64      tx_rstp_bpdu_cnt;
    cs_uint64      tx_tcn_bpdu_cnt;
} __attribute__((packed)) cs_onu_port_stp_state_config_t;

cs_status  app_stp_bridge_cfg_set (
    CS_IN cs_onu_stp_config_t * bridge_cfg);

cs_status  app_stp_bridge_cfg_get (
    CS_OUT cs_onu_stp_config_t   *  bridge_cfg );  

cs_status  app_stp_bridge_state_get (
    CS_OUT cs_onu_stp_state_config_t * bridge_state );  

cs_status  app_stp_port_cfg_set (
    CS_IN cs_port_id_t   port,
    CS_IN cs_onu_port_stp_config_t *port_cfg );  

cs_status  app_stp_port_cfg_get (
    CS_IN cs_port_id_t   port,
    CS_OUT cs_onu_port_stp_config_t *port_cfg);  

cs_status  app_stp_port_state_get (
    CS_IN cs_port_id_t   port ,
    CS_OUT cs_onu_port_stp_state_config_t * port_state);  

cs_status stp_init (void );

#endif /* __STP_MGNT_H */

