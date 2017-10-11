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

#ifndef __TERM_SERVER_H
#define __TERM_SERVER_H

/* support UART 1 ~ 4*/
#ifdef UART_NUM_2
#define MAX_TERM_SERV_NUM       2
#define TS_MAX_UART_NUM         2
#else
#define MAX_TERM_SERV_NUM       4
#define TS_MAX_UART_NUM         4
#endif

//#define MAX_TERM_SERV_NUM       4
#define TS_MIN_UART_NUM         1
//#define TS_MAX_UART_NUM         4

/* UART definitions */
#define MIN_UART_BAUD_RATE      300
#define MAX_UART_BAUD_RATE      115200
#define MAX_UART_DATA_BIT       8
#define MIN_UART_STOP_BIT       1
#define MAX_UART_STOP_BIT       2

/* protocol definitions */
#define MIN_SOCKET_PORT         1024
#define MAX_SOCKET_PORT         65535


#define UART_TO_TERM_SERV(x)    ((x) - TS_MIN_UART_NUM)
#define TS_MAX_PAYLOAD_FLOOR         1024
#define TS_MAX_PAYLOAD_CEILING         1400
//#define TS_MAX_BUF_SIZE         1460
//#define TS_MIN_BUF_SIZE         512
#if 1
#define TS_BUF_SIZE             1024
#else
#define TS_BUF_SIZE             500
#endif
#define TS_UART_BUF_MAX         2048
#define TS_MIN_PAD_SIZE         40
#define TS_MIN_RSP_TIME         25
#define TS_MAX_RSP_TIME         1000
#define MAX_RING_BUF_NUM        16
#define MAX_REMOTE_TIMEOUT      65535
//#define TS_MAX_PAYLOAD          1460

#define VALID_CONFIG_FLAG       0xA5
#define TERM_SERV_ENABLE_FLAG   0x5A

/* message queue */
#define TS_UART_RX_QUEUE_DEPTH  16
#define TS_UART_RX_QUEUE_SIZE   4
#define TS_IP_RX_QUEUE_DEPTH    64


typedef struct {
    cs_uint8        uart;
    cs_uint16       len;
    cs_uint8        data[0];
}uart_msg_t;
#if 0
typedef struct {
    uart_msg_t *    act_buf;
    uart_msg_t *    pp_msg[2];
}ping_pong_buf_t;
#endif
typedef struct
{
    cs_uint8     uart_num;
    cs_uint32    length;
    cs_uint8     net_data[0];
}net_msg_t;

typedef struct
{
    cs_uint32    pIngress;
    cs_uint32    pEgress;
    //cs_uint32   pkt_nun;
    cs_uint32   wr_count;
    cs_uint32   rd_count;
    net_msg_t * pBuf[MAX_RING_BUF_NUM];
}ring_buf_t;

typedef struct{
	uart_config_t uart_cfg; 		
	cs_uint32 proto_type;	  /* protocol type TCP 6/UDP 1*/
	cs_uint32 proto_port;		 /* L4 protocol port 1~65535 */
	cs_uint32 client_timeout;  /* remote client timeout 0:never thoer:1~65535s*/
	cs_uint32 max_payload;	 /* 512 ~1400 bytes */
	cs_uint32 min_payload;	  /* 40 ~ max_payload */
	cs_uint32 max_res_time; /* max response time 25~1000ms */
	cs_int32 server_fd;   /* server socket FD */
	cs_int32 client_fd;    /* client socket FD */
	cs_uint32 client_ip;
	cs_uint32 client_port;
	cs_uint32 client_closed_flag; /* check client connection */
	cs_uint32 term_serv_rx_ip_packets;
	cs_uint32 term_serv_tx_ip_packets;
	cs_uint32 term_serv_tx_lost_packets;
	cs_uint32 term_serv_rx_ip_cnt;
	cs_uint32 term_serv_tx_ip_cnt;
	cs_uint32 term_serv_tx_lost_cnt;
	cs_uint32 uart_rx_char_cnt;
	cs_uint32 uart_tx_char_cnt;
	cs_uint32 enable_flag;	/* terminal server enable/disable*/
	cs_uint64 last_act_ticks;
	cs_uint8  init_flag;  /* used to check flash area has valid config data   VALID_CONFIG_FLAG*/ 
	ring_buf_t ip_rx_buf;
	#if 0
	ping_pong_buf_t    uart_msg; /* store received UART data */
	#endif
	
	#if 1
	cs_uint8 server_client_mode;	//0-server mode, 1-client mode
	cs_uint8 client_enable;
	cs_uint8 server_enable;
	cs_uint8 socket_enable; 	/*socket 的状态。1:已创建socket;0:未创建socket*/
	cs_uint8 socket_control;	//1:要创建socket;0:不创建socket
	#endif

	#if 1
	cs_uint32 remote_server_ip;
	cs_uint16 remote_server_port;
	#endif
	cs_uint32 reserved[1];
} term_server_config_t;

typedef struct
{
	cs_uint8  init_flag;  /* used to check flash area has valid config data    VALID_CONFIG_FLAG*/ 
	uart_config_t uart_cfg; 		
	cs_uint32 proto_type;	  /* protocol type TCP 6/UDP 1*/
	cs_uint32 proto_port;		 /* L4 protocol port 1~65535 */
	cs_uint32 client_timeout;  /* remote client timeout 0:never thoer:1~65535s*/
	cs_uint32 max_payload;	 /* 512 ~1400 bytes */
	cs_uint32 min_payload;	  /* 40 ~ max_payload */
	cs_uint32 max_res_time; /* max response time 25~1000ms */
	
	cs_uint8 server_client_mode;	//0-server mode, 1-client mode
	cs_uint8 client_enable;
	cs_uint8 server_enable;
	cs_uint8 socket_control;	//1:要创建socket;0:不创建socket

	cs_uint32 remote_server_ip;
	cs_uint16 remote_server_port;
}serial_port_config_t;
//#define TERM_SRV_UART_MAX_SIZE  8192
#define TERM_SRV_UART_MAX_SIZE  3072

#define TERM_SRV_MTU 1460
typedef struct _ts_uart_ring_t
{
    cs_uint32 rd_pos;
    cs_uint32 rd_count;
    cs_uint32 wr_pos;
    cs_uint32 wr_count;
//    cs_uint64 last_act_ticks;
    cs_uint8 data[TERM_SRV_UART_MAX_SIZE];
}ts_uart_ring_t;

typedef struct {
        cs_int32    magic_flag;            /* magic value=0x53ee*/
        cs_int16    direction ;             /* 0 for GE side only, 1 for PON side only*/

        cs_int16    outband_vlanid ;        
        cs_uint32   outband_ip;
        cs_uint32   outband_ip_mask;
        cs_uint32   outband_gateway;      

       	cs_uint32   inband_ip;
        cs_uint32   inband_ip_mask;
        cs_uint32   inband_gateway;  
		cs_int16	inband_vlanid;

		cs_uint32   pty_ip;
		cs_uint32   pty_mask;
		cs_uint32   pty_gateway;
        cs_int16    pty_vlanid ;

		#if 1
		cs_uint32	uart_ip;		//采用本地字节序
		cs_uint32	uart_mask;		//采用本地字节序
		cs_uint32	uart_gateway;	//采用本地字节序
		cs_uint16	uart_vlan;		//采用本地字节序
		#endif
		
		#if 1
		cs_uint32	device_ip;		//采用本地字节序
		cs_uint32	device_mask;	//采用本地字节序
		cs_uint32	device_gateway;	//采用本地字节序
		cs_uint16	device_vlan;	//采用本地字节序
		#endif

		#if 1
		int			mc_mode;	//0:MC_SNOOPING, 1:MC_MANUAL, 2:MC_PROXY, 3:MC_DISABLE
		#endif
} onu_slow_path_cfg_cfg_t;

typedef struct{
    cs_uint8    init_flag;
    cs_uint8    uart_id;        /* UART number 0,1,2,3,4*/
    cs_uint8    data_bits;      /* 7/8  */
    cs_uint8    stop_bits;      /* 1/2 */
    cs_uint8    flow_control;   /* None/XONXOFF/HardWare */
    cs_uint8    parity;         /* None/Odd/Even */
    cs_uint8    duplex;         /* full-duplex or half-duplex*/
    cs_uint8    mode;           /* Polling or Interrupt */    
    cs_uint32   baud_rate;          
    cs_uint32   proto_type;     /* protocol type TCP 6/UDP 1*/
    cs_uint32   proto_port;     /* L4 protocol port 1~65535 */
    cs_uint32   client_timeout; /* remote client timeout 0:never thoer:1~65535s*/
    cs_uint32   max_payload;    /* 512 ~1400 bytes */
    cs_uint32   min_payload;    /* 40 ~ max_payload */
    cs_uint32   max_res_time;   /* max response time 25~1000ms */
	#if 1
	cs_uint8	model_flag;		/*0:server model; 1:client model*/
	cs_uint32	remote_server_ip;
	cs_uint16	remote_server_port;
	#endif
}db_ts_cfg_t;

extern onu_slow_path_cfg_cfg_t   g_slow_path_ip_cfg;

extern cs_uint32 term_serv_dbg;

extern cs_status ts_enable(term_server_config_t* pTerm_serv_cfg);
extern void ts_disable(cs_uint8 uart);
extern void ts_reset(cs_uint8 uart);
extern void ts_show(cs_uint8 uart);
extern void ts_cfg_save(cs_uint8 uart);
extern void ts_cfg_clear(cs_uint8 uart);
extern cs_int32 ts_uart_intr_handler(cs_int32 uart_id,void *data);
extern cs_status ts_uart_id_check(cs_uint8 uart);
extern void ts_init(void);
extern void ts_electricity_meter_test(cs_uint8 uart);
extern term_server_config_t *terminal_server_cfg[MAX_TERM_SERV_NUM];

#endif

