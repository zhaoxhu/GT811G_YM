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

#ifndef _PPPOE_H_
#define _PPPOE_H_
#include "osal_api_packet.h"
#include "app_timer.h"

#define PPPOE_TAG_MAX_LEN  256

#define PPPOE_QUEUE_NAME   "pppoe_queue"
#define PPPOE_QUEUE_DEPTH  64
#define PPPOE_QUEUE_SIZE   4


#define  PPPOE_ASSERT(x)    \
    do{ \
        if (!(x))  { \
            cs_printf("%s() _%d_: assert fail\n", __FUNCTION__, __LINE__); \
            return ; \
        } \
    }while(0)

#define  PPPOE_ASSERT_RET(x,ret)    \
            do{ \
                if (!(x))  { \
                    cs_printf("%s() _%d_: assert fail\n", __FUNCTION__, __LINE__); \
                    return (ret); \
                } \
            }while(0)
        

#define PPPOE_LOG(arg,...)  \
           cs_log_msg(IROS_LOG_LEVEL_MAJ, IROS_MID_PPPOE, "%s(): "arg, __FUNCTION__, ##__VA_ARGS__)
           
#define PPPOE_LOG_INFO(arg,...)  \
           cs_log_msg(IROS_LOG_LEVEL_INF, IROS_MID_PPPOE, "%s(): "arg, __FUNCTION__, ##__VA_ARGS__)   
           
#define PPPOE_LOG_MIN(arg,...)  \
           cs_log_msg(IROS_LOG_LEVEL_MIN, IROS_MID_PPPOE, "%s(): "arg, __FUNCTION__, ##__VA_ARGS__)
           
#define PPPOE_LOG_CRI(arg,...) \
           cs_log_msg(IROS_LOG_LEVEL_CRI, IROS_MID_PPPOE,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)
           
#define PPPOE_LOG_DEBUG(arg,...) \
           cs_log_msg(IROS_LOG_LEVEL_DBG0, IROS_MID_PPPOE,"%s(): "arg, __FUNCTION__,##__VA_ARGS__)

#define PPPOE_TEST_START         1 /* ZTE PPPOE OAM indication: START */
#define PPPOE_TEST_STOP          0 /* ZTE PPPOE OAM indication: STOP  */

/* Initial timeout for PADO/PADS */
#define PPPOE_TIMEOUT_INTERVAL 30000

/*PADI/PADR send retry nums*/
#define PPPOE_SEDN_TRY_MAX_NUM  5

#define PPPOE_MAX_HDR_SZIE  28
#define PPPOE_MAX_PAYLOAD  (CS_ETHER_MTU-PPPOE_MAX_HDR_SZIE)
#define PPP_MAX_PAYLOAD  (PPPOE_MAX_PAYLOAD-2)

/* PPPoE codes */
#define PPPOE_CODE_PADI           0x09
#define PPPOE_CODE_PADO           0x07
#define PPPOE_CODE_PADR           0x19
#define PPPOE_CODE_PADS           0x65
#define PPPOE_CODE_PADT           0xA7

#define PPPOE_VER          1
#define PPPOE_TYPE        1
#define PPPOE_CODE        0

/* PPPoE Tags */
#define TAG_END_OF_LIST        0x0000
#define TAG_SERVICE_NAME       0x0101
#define TAG_AC_NAME            0x0102
#define TAG_HOST_UNIQ          0x0103
#define TAG_AC_COOKIE          0x0104
#define TAG_VENDOR_SPECIFIC    0x0105
#define TAG_RELAY_SESSION_ID   0x0110
#define TAG_SERVICE_NAME_ERROR 0x0201
#define TAG_AC_SYSTEM_ERROR    0x0202
#define TAG_GENERIC_ERROR      0x0203

#define PPPOE_HOSTID_LEN    8
#define TAG_HDR_SIZE 4

#ifdef HAVE_MPORTS
#define PPPOE_ONU_NAME                    "F400M"
#else
#define PPPOE_ONU_NAME                    "F401M"
#endif

#define PPPOE_PLUS_TAG_VENDOR_ID    (0x0105)
#define PPPOE_PLUS_VENDOR_ID           (0x000de9)

#define PPPOE_TIMER_PADI  1
#define PPPOE_TIMER_PADR  2

#define PPP_USER_MAX_LEN  64
#define PPP_PWD_MAX_LEN   32

#define PPPOE_RESTORE_INTERVAL        300000

/* error code */
typedef enum {
/*
0x00:    Success
0x01:    Unkown error
0x02:    Hardware not support
0x03:    Unconfig vlan
0x04£ºPADI timeout
0x05£ºPADR  timeout
0x06£ºLCP nogociate fail
0x07£ºauth fail
0x08£ºIP negociate fail
*/
    PPPOE_TEST_RET_OK = 0,
    PPPOE_TEST_RET_SDL_ERR ,
    PPPOE_TEST_RET_NOT_SUPPORT,
    PPPOE_TEST_RET_VLAN_NOT_EXIST,
    PPPOE_TEST_RET_PADI_TIMEOUT,
    PPPOE_TEST_RET_PADR_TIMEOUT,
    PPPOE_TEST_RET_LCP_NEG_FAIL,
    PPPOE_TEST_RET_AUTH_FAIL,
    PPPOE_TEST_RET_IPCP_NEG_FAIL
} pppoe_test_ret_e;

typedef enum {
    PPPOE_TEST_NOT_RUN = 0,
    PPPOE_TEST_RUNNING,
    PPPOE_TEST_OK,
    PPPOE_TEST_FAIL
}PPPOE_TEST_RESULT_E;

typedef enum{
    PPPOE_DISCOVERY_PADI = 0,
    PPPOE_DISCOVERY_PADO,
    PPPOE_DISCOVERY_PADR,
    PPPOE_DISCOVERY_PADS,
    PPPOE_DISCOVERY_PADT
}pppoe_discovery_packet_e;

/* state */
typedef enum {
    PPPOE_STATE_INIT        = 0,
    PPPOE_STATE_PADI       = 1,
    PPPOE_STATE_PADO      = 2,
    PPPOE_STATE_PADR      = 3,
    PPPOE_STATE_PADS      = 4,
	PPPOE_STATE_SESSION	  	  = 5,
	PPPOE_STATE_INVALID	  = 6
}pppoe_state_e;

typedef enum{
    PPPOE_PACKET_UP = 0,
    PPPOE_PACKET_DOWN,
    PPPOE_PACKET_BOTH
}pppoe_packet_direction_e;

typedef enum{
    PPPOE_PACKET_TO_CPU = 0,
    PPPOE_PACKET_NOT_TO_CPU,
}pppoe_packet_action_e;

typedef enum{
    PPP_LCP_DOWN = 0,        /* Lcp negotiation fail */
    PPP_AUTH_FAIL,        /* Authentication fail */
    PPP_IPCP_NEG_FAIL,       /* IPCP negotiation fail */
    PPP_IPCP_OK        /* IPCP successful */
}ppp_negoStatus;

typedef struct{
    cs_uint16 type;
    cs_uint16 len;
    cs_uint8 payload[PPPOE_TAG_MAX_LEN];
} __attribute__((packed))pppoe_tag_t;


typedef struct{
    cs_ether_header_t  eth_hdr;       /* Ethernet header */  
    cs_uint8 type:4;        /* PPPoE Type (must be 1) */
    cs_uint8 ver:4;         /* PPPoE Version (must be 1) */
    cs_uint8  code;        /* PPPoE code */
    cs_uint16 session;    /* PPPoE session */
    cs_uint16 length;     /* Payload length */
    cs_uint8  payload[PPPOE_MAX_PAYLOAD]; /* A bit of room to spare */
}  __attribute__((packed)) pppoe_pkt_t;

typedef struct{
    cs_uint16 len;
    cs_uint8  data[PPP_MAX_PAYLOAD];
} __attribute__((packed))ppp_pkt_t;

typedef enum{
    PPPOE_TAG_AC_NAME = 0,
    PPPOE_TAG_SERV_NAME,
    PPPOE_TAG_COOKIE,
    PPPOE_TAG_RELAY_ID,
    PPPOE_TAG_MAX
}PPPOE_TAG_TYPE_E;

typedef struct{
    cs_mac_t peer_mac;
    cs_uint16 session;
    cs_uint16 vlan;
    cs_port_id_t port;
    cs_boolean pads_error;
    cs_boolean host_uniq_tag;
    pppoe_tag_t tags[PPPOE_TAG_MAX];
}pppoe_connection_t;

typedef struct{
    cs_port_id_t port;
    cs_uint64 time;  
    cs_uint8 auth_mode; 
    cs_uint8 name[PPP_USER_MAX_LEN+1];
    cs_uint8 pwd[PPP_PWD_MAX_LEN+1];
    cs_uint8 disc_rslt;
    cs_uint8 simul_rslt;
    cs_uint8 reason;
    cs_mac_t serv_mac;
    cs_uint32 ipv4;
} __attribute__((packed)) pppoe_test_result_t;

typedef enum  {
    AUTH_AUTO,
    AUTH_PAP,
    AUTH_CHAP
} pppAuthMode;

typedef struct {
        cs_uint8 name[PPP_USER_MAX_LEN+1];
        cs_int8 pwd[PPP_PWD_MAX_LEN+1];
        cs_int8 mode;
} __attribute__((packed)) pppoe_auth_t;

typedef struct{
    app_timer_msg_t msg;
    cs_uint64 start_time;
    cs_int32 timer_handler;
    cs_port_id_t port;
    cs_uint8 state;  /*PPPOE state*/
    cs_boolean disc_over;
    cs_boolean simul_over;
    cs_boolean disc_rslt;
    cs_boolean simul_rslt;
    cs_boolean padt_recv;
    cs_uint8 test_id; /*to uniquely a connection, generate host-uniq*/
    cs_uint64 use_time;
    cs_uint32 ip_addr;
    pppoe_auth_t auth;
    cs_uint8 code;
    pppoe_connection_t conn; /*PPPOE connection information*/ 
}pppoe_port_info_t;

typedef struct{
    cs_port_id_t port;
    cs_uint8 max_port;
    cs_boolean loop_status;
    cs_mac_t my_mac;
    cs_boolean plus_enable;
    cs_int32 timer_handler;
    pppoe_auth_t auth;
    pppoe_port_info_t *ports_info;
}pppoe_cb_t;

/* Function passed to parsePacket */
typedef void (*pppoe_parse_func_t)(pppoe_tag_t* tag, void *extra);
/* Packet RX callback functions */
typedef void (*app_ppp_pkt_tx_f)(cs_uint8 *frame,cs_uint32 length);


/*pppoe+ definitions*/
#define PPPOE_PLUS_SUB_OPT_NUM  2
#define PPPOE_PLUS_SUB_MAX_PAD   256

typedef struct {
    cs_uint8  opt;
    cs_uint8  pad_len;
    cs_uint8  pad[PPPOE_PLUS_SUB_MAX_PAD];  
} __attribute__((packed))pppoe_plus_subopt_t;

typedef struct{
    cs_uint16 type;
    cs_uint16 len;
    cs_uint32 vendor;
    pppoe_plus_subopt_t   pad[PPPOE_PLUS_SUB_OPT_NUM];
} __attribute__((packed))pppoe_plus_tag_t;


void pppoe_init();
cs_status pppoe_discovery_test_result_get(
        cs_port_id_t port,
        cs_uint8* status,
        cs_uint8* code);
cs_status pppoe_discovery_test_start(
        cs_port_id_t port,
        cs_uint16 vlan);
cs_status pppoe_discovery_test_stop(
        cs_port_id_t port);
cs_status pppoe_plus_enable(cs_boolean enable);
cs_boolean pppoe_plus_status_get();
void pppoe_cmd(
        cs_uint32 para1,
        cs_uint32 para2,
        cs_uint32 para3,
        cs_uint32 para4);
void pppoe_timer_handle(app_timer_msg_t *msg);
cs_status pppoe_packet_send(
        cs_uint8 *frame,
        cs_uint32 len,
        cs_port_id_t port,
        cs_uint8 prio);
cs_uint8 pppoe_state_get(cs_port_id_t port);
void pppoe_state_change(
        cs_port_id_t port,
        cs_uint8 state,
        cs_boolean disc_over,
        cs_boolean simul_over,
        cs_uint8 code);

cs_boolean pppoe_discovery_completed(
    cs_port_id_t *port);
cs_status pppoe_discovery_packet_send(
        cs_uint8 type,
        pppoe_connection_t *conn);
void pppoe_test_result_set(
    cs_port_id_t port,
    cs_uint8 result);

void pppoe_simul_timer_stop();

void app_pppoe_queue_create();

extern cs_uint32 g_pppoe_msgQ;
extern pppoe_cb_t  g_pppoe_info;
#endif
