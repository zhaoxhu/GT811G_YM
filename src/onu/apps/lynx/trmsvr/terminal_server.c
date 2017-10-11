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

#ifdef HAVE_TERMINAL_SERVER
#include <network.h>
#include "plat_common.h"
#include "terminal_server.h"
#include "terminal_server_adapt.h"
#ifdef HAVE_DB_MANAGMENT
#include "db_instance.h"
#endif
#include "gpio.h"
#include "cli_common.h"
#include "gwd_flash_oper.h"
#include "sdl_uart.h"

extern cs_status socket_status_set(term_server_config_t* pTerm_serv_cfg);
extern cs_status socket_status_clear(term_server_config_t* pTerm_serv_cfg);

extern cs_uint8 socket_status_get(term_server_config_t* pTerm_serv_cfg);
extern cs_status buf_print(cs_uint8 *buf, cs_uint16 len);

#if 0
static cs_status uart_enable_semaphore_init(cs_uint8 uart);
static cs_status uart_enable_semaphore_post(cs_uint8 uart);
static cs_status uart_enable_semaphore_wait(cs_uint8 uart);
#endif

#if 1
extern int serial_port_config_runing_format_switch_to_save_format(const term_server_config_t* running_format, serial_port_config_t *save_format);
extern int serial_port_config_save_format_switch_to_runing_format(term_server_config_t* running_format, serial_port_config_t *save_format);
extern int serial_port_config_save_format_set_default(cs_uint8 uart, serial_port_config_t *save_format);
extern int serial_port_config_save_format_default_check(cs_uint8 uart, serial_port_config_t *save_format);
extern term_server_config_t* serial_port_running_config_get(int uart);

#endif


/*****************************************************************************
 *
 *           Local Macros/Constant
 *
 ****************************************************************************
 */ 

/* Marco Functions */
cs_uint32 STOP_UART_SEND_RCE = 1;
cs_uint32 term_serv_dbg = 0;
#define TERM_DBG_PRINT(fmt,...)  do { if (term_serv_dbg) \
                                    cs_printf("\n%s():"fmt, __FUNCTION__,##__VA_ARGS__); \
                                  }while(0)

#define TS_DBG(fmt...)  do { if (term_serv_dbg) \
                                    cs_printf(fmt); \
                                  }while(0)

#define TS_ENABLED(ts) (terminal_server_cfg[ts]->enable_flag == TERM_SERV_ENABLE_FLAG )
#define TS_WR_BUF_FULL(ts) \
    ((ts_uart_ring[ts].wr_pos+1)%TERM_SRV_UART_MAX_SIZE == ts_uart_ring[ts].rd_pos)

#define TS_CFG_GET(uart) terminal_server_cfg[UART_TO_TERM_SERV(uart)]
    
/*used to inform IP layer one UART packet received */
cs_uint32 msgq_ts_uart_rx = 0;
/* used to inform UART one IP packet received */
cs_uint32 msgq_ts_net_rx = 0;

/* Terminal Server Task/Thread parameters */
cs_uint32 ip_rx0_thread_id = 0;
cs_uint32 ip_rx1_thread_id = 0;
cs_uint32 ip_rx2_thread_id = 0;
cs_uint32 ip_rx3_thread_id = 0;
cs_uint32 uart2net_thread_id = 0;
cs_uint32 net2uart_thread_id = 0;

term_server_config_t * terminal_server_cfg[MAX_TERM_SERV_NUM];
ts_uart_ring_t ts_uart_ring[MAX_TERM_SERV_NUM];
#if 0
ts_uart_ring_t text_uart_date[MAX_TERM_SERV_NUM];
#endif
onu_slow_path_cfg_cfg_t   g_slow_path_ip_cfg;

//volatile 
static void ts_def_uart_rx_func(cs_int32 uart, unsigned char *data, cs_int32 len);
cs_int32 ts_client_fd_close(term_server_config_t* pTs_cfg)
{
    cs_int32 client_fd;
    //add sem to protect
    if((client_fd = pTs_cfg->client_fd) >= 0) {
        pTs_cfg->client_fd = -1;
        close(client_fd);
        TS_DBG("%s, close client fd %d, %d\n", __func__, client_fd, __LINE__);
    }
    else {
        TS_DBG("%s, no client fd active, %d\n", __func__, __LINE__);
    }
    
    //add sem to protect
    return 0;
}

cs_int32 ts_server_fd_close(term_server_config_t* pTs_cfg)
{
    cs_int32 server_fd;
    //add sem to protect
    if((server_fd = pTs_cfg->server_fd) >= 0) {
        pTs_cfg->server_fd = -1;
		#if 1
        close(server_fd);
		#endif
        TS_DBG("%s, close server fd %d, %d\n", __func__, server_fd, __LINE__);
    }
    else {
        TS_DBG("%s, server fd is not active, %d\n", __func__, __LINE__);
    }
    //add sem to protect
    return 0;
}

cs_uint32 ts_send_data_2net(cs_uint32 ts_id, cs_uint8 *data, cs_uint32 len)
{
    cs_uint32 offset = 0;
    cs_uint32 term_mtu = terminal_server_cfg[ts_id]->max_payload;
    while(len/term_mtu) {
        ts_def_uart_rx_func(ts_id + 1,&data[offset],term_mtu);
        offset += term_mtu;
        len -= term_mtu;
    }
    if(len > 0) {
        ts_def_uart_rx_func(ts_id + 1,&data[offset],len);
        offset += len;
    }
    
    return offset;
 }
cs_int32 ts_uart2net_proc(cs_uint32 ts_id)
{
#if 0
	cs_printf("in ts_uart2net_proc\n");
#endif
    cs_uint32 wrpos = ts_uart_ring[ts_id].wr_pos;
    cs_uint32 rdpos = ts_uart_ring[ts_id].rd_pos;
    cs_uint32 wr_count = ts_uart_ring[ts_id].wr_count;
    cs_uint32 rd_count = ts_uart_ring[ts_id].rd_count;
    cs_uint32 len = 0;
    
   if(rdpos == wrpos){
    /* empty */
        return -1;
   }
   len = (wrpos>rdpos)? (wrpos-rdpos):(wrpos+TERM_SRV_UART_MAX_SIZE-rdpos);
   
    TERM_DBG_PRINT("UART[%d] len %d, wr_count 0x%x, rd_count 0x%x, wrpos %d, rd_pos %d\n", 
        ts_id, len, wr_count, rd_count,wrpos, rdpos);  
    
    /* process data here */
    if(rdpos < wrpos) {
        ts_send_data_2net(ts_id, &ts_uart_ring[ts_id].data[rdpos], len);
		#if 0
		int i = 0;
		for(i=0;i<len;i++)
		{
			cs_printf("0x%2x ,", ts_uart_ring[ts_id].data[rdpos+i]);
		}
		#endif
        ts_uart_ring[ts_id].rd_count += len;
        ts_uart_ring[ts_id].rd_pos = (rdpos + len) % TERM_SRV_UART_MAX_SIZE;//Can not be
    }
    else {
            //assert(len > (TERM_SRV_UART_MAX_SIZE - rdpos -1));
            ts_send_data_2net(ts_id, &ts_uart_ring[ts_id].data[rdpos], TERM_SRV_UART_MAX_SIZE - rdpos -1);
            ts_send_data_2net(ts_id, &ts_uart_ring[ts_id].data[0], len - (TERM_SRV_UART_MAX_SIZE - rdpos -1));
            ts_uart_ring[ts_id].rd_count += len;
            ts_uart_ring[ts_id].rd_pos = wrpos;
    }
    
    return 0;
    
}

/* return 1 if valid UART ID */
cs_status ts_uart_id_check(cs_uint8 uart)
{
    if((uart >= TS_MIN_UART_NUM) && (uart <= TS_MAX_UART_NUM)){
        return CS_E_OK;
    }
    else {
        return CS_E_PARAM;
    }
}

static cs_status check_uart_cfg(
    uart_config_t *uart_cfg
)
{
    if (NULL == uart_cfg) {
		cs_printf("uart_cfg");
        return CS_E_PARAM;
    }

    if(ts_uart_id_check(uart_cfg->uart_id)) {
        TS_DBG("Wrong uart id %d!\n", uart_cfg->uart_id);
        return CS_E_NOT_SUPPORT;
    }
    
    if(uart_cfg->baud_rate > MAX_UART_BAUD_RATE || uart_cfg->baud_rate < MIN_UART_BAUD_RATE ) {
        TS_DBG("Wrong baud rate!\n");
        return CS_E_NOT_SUPPORT;
    }
    if(uart_cfg->data_bits != MAX_UART_DATA_BIT) {
        TS_DBG("Wrong data bits!\n");
        return CS_E_NOT_SUPPORT;
    }
    if(uart_cfg->stop_bits > MAX_UART_STOP_BIT || uart_cfg->stop_bits < MIN_UART_STOP_BIT) {
        TS_DBG("Wrong stop bits!\n");
        return CS_E_NOT_SUPPORT;
    }
    
    if (uart_cfg->parity > UART_PARITY_EVEN) {
        TS_DBG("Wrong parity!\n");
        return CS_E_NOT_SUPPORT;
    }

    if (UART_DUPLEX_HALF != uart_cfg->duplex){
        TS_DBG("Wrong duplex!\n");
        return CS_E_NOT_SUPPORT;
    }

    if (UART_MODE_INTR != uart_cfg->mode){
        TERM_DBG_PRINT("Wrong mode should be interrupt mode!\n");
        return CS_E_NOT_SUPPORT;
    }    
    
    return CS_E_OK;
    
}

static net_msg_t * ring_buffer_get(term_server_config_t* pTerm_serv_cfg)
{
    cs_uint16 pos;

    if(pTerm_serv_cfg->ip_rx_buf.wr_count - pTerm_serv_cfg->ip_rx_buf.rd_count < MAX_RING_BUF_NUM){
        pTerm_serv_cfg->ip_rx_buf.wr_count ++;
        pos = pTerm_serv_cfg->ip_rx_buf.pIngress;
        pTerm_serv_cfg->ip_rx_buf.pIngress = 
            (pTerm_serv_cfg->ip_rx_buf.pIngress + 1) % MAX_RING_BUF_NUM ; 
        
        return (pTerm_serv_cfg->ip_rx_buf.pBuf[pos]);
    }
    else {
        TS_DBG("The receive buffer full, uart[%d], wr_count %d, rd_count %d, pIngress\n", 
            pTerm_serv_cfg->uart_cfg.uart_id, pTerm_serv_cfg->ip_rx_buf.wr_count, 
            pTerm_serv_cfg->ip_rx_buf.rd_count, pTerm_serv_cfg->ip_rx_buf.pIngress);
        return NULL;
    }
}
#if 0
/* Get standby ping pong buffer */
uart_msg_t * ts_pingpong_buf_get(term_server_config_t* pTerm_serv_cfg)
{
    
   return (pTerm_serv_cfg->uart_msg.act_buf == 
        pTerm_serv_cfg->uart_msg.pp_msg[0] ? 
        pTerm_serv_cfg->uart_msg.pp_msg[1] : 
        pTerm_serv_cfg->uart_msg.pp_msg[0]);
}
#endif
static cs_uint8 ts_net_msg_proc(net_msg_t * pMsg)
{
#if 0
	cs_printf("in ts_net_msg_proc\n");
	cs_printf("\n*************************\n");
	int i = 0;
	for(i=0;i<pMsg->length;i++)
	{
		cs_printf("0x%x, ", pMsg->net_data[i]);
	}
	cs_printf("\n*************************\n");

#endif
    cs_int32 count = 0;
    cs_uint8 uart = pMsg->uart_num;

    if(CS_E_OK == ts_uart_id_check(uart)){
        for(count = 0; count < pMsg->length; count++){
            ts_putc(uart,pMsg->net_data[count]);
        }
        terminal_server_cfg[UART_TO_TERM_SERV(uart)]->uart_tx_char_cnt += pMsg->length;
        terminal_server_cfg[UART_TO_TERM_SERV(uart)]->ip_rx_buf.rd_count++;
    }
    else {
        TS_DBG("Invalid uart ID from message, uart[%d]\n", uart);
    }

    return 1;
}

static void ts_net_msg_send(net_msg_t * pMsg)
{
#if 0
	cs_printf("in %s\n", __func__);
#endif
	if(STOP_UART_SEND_RCE)
		{
		    if(CS_E_OSAL_OK != cs_queue_put(msgq_ts_net_rx, (void *)&pMsg, sizeof(cs_uint32) , CS_OSAL_NO_WAIT , 0)){ 
		        TS_DBG("ts_net_msg_send put msg failed\n");
		        return;
		    }
		}
}
#if 0
void put_queue_delay(void*date)
{
	cs_printf("put queue delay\n");
	static uart_msg_t uart_msg;
    uart_msg_t * msg = &uart_msg;
    cs_queue_put(msgq_ts_uart_rx, (void *)&msg, sizeof(cs_uint32) , CS_OSAL_NO_WAIT, 0);

}
#endif

extern cyg_sem_t uart2net_sem;

cs_int32 ts_uart_intr_handler(cs_int32 uart_id,void *data)
{
#if 0
	cs_printf("in ts_uart_intr_handler\n");
#endif
    cs_uint8 term_id;
    cs_uint8 *buffer = NULL;
    cs_uint32 offset = 0;
	#if 0
	static uart_msg_t uart_msg;
    uart_msg_t * msg = &uart_msg;
	#endif
    term_server_config_t *pTsCfg = TS_CFG_GET(uart_id);

    
    term_id = UART_TO_TERM_SERV(uart_id);
    if((ts_uart_ring[term_id].wr_pos+1)%TERM_SRV_UART_MAX_SIZE == ts_uart_ring[term_id].rd_pos) {
        //full 
        buffer = NULL;
        TS_DBG("UART[%d] full. wrpos %d, rdpos %d,wrcount %d, rdcount %d\n", uart_id,
                                                    ts_uart_ring[term_id].wr_pos, ts_uart_ring[term_id].rd_pos,
                                                    ts_uart_ring[term_id].wr_count, ts_uart_ring[term_id].rd_count);  
    }
    else {
        buffer = (cs_uint8 *)&ts_uart_ring[term_id].data[0];
        offset = ts_uart_ring[term_id].wr_pos;
    }
#if 0
    if(buffer) {
		if(STOP_UART_SEND_RCE)
			{
			cs_printf("enable send rce\n");
		        while(CS_E_OK == ts_getc(uart_id,&buffer[offset])) {
				//	cs_printf("recv...\n");
		            pTsCfg->uart_rx_char_cnt++;
		            ts_uart_ring[term_id].wr_count ++;
		            offset = (offset + 1) % TERM_SRV_UART_MAX_SIZE;			
		        }
				cs_printf("offset:%d\n",offset);
		        ts_uart_ring[term_id].wr_pos = offset;
			}
    }
    else {
        cs_uint8 temp;
		if(STOP_UART_SEND_RCE)
			{
		        while(CS_E_OK == ts_getc(uart_id,&temp)) {
		        }
			}
    }
#else
	 if(buffer) {
		        while(CS_E_OK == ts_getc(uart_id,&buffer[offset])) {
				//	cs_printf("recv...\n");
		            pTsCfg->uart_rx_char_cnt++;
		            ts_uart_ring[term_id].wr_count ++;
		            offset = (offset + 1) % TERM_SRV_UART_MAX_SIZE;			
		        }
		        ts_uart_ring[term_id].wr_pos = offset;
				}
    else {
        cs_uint8 temp;
        while(CS_E_OK == ts_getc(uart_id,&temp)) {
        }
    }
#endif

    TS_DBG("UART[%d] wr_pos %d, rd_pos %d\n", uart_id,
                                                    ts_uart_ring[term_id].wr_pos, ts_uart_ring[term_id].rd_pos);  

    if(TS_ENABLED(term_id)){
       pTsCfg->last_act_ticks = cyg_current_time(); 
       if(ts_uart_ring[term_id].wr_count - ts_uart_ring[term_id].rd_count > 0){
          TS_DBG("UART[%d] send message to tx thread \n", uart_id);
			//cs_printf("uart buf stat send......\n");
			#if 0
			cs_queue_put(msgq_ts_uart_rx, (void *)&msg, sizeof(cs_uint32) , CS_OSAL_NO_WAIT, 0);
			#else
			cyg_semaphore_post(&uart2net_sem);
			#endif	
       }
    }

   return CS_E_OK;
   
}

#if 0
cs_int32 text_uart_intr_handler(cs_uint8 uart_id,void *data)
{
    cs_uint8 term_id;
    cs_uint8 *buffer = NULL;
    cs_uint32 offset = 0;
   // static uart_msg_t uart_msg;
  //  uart_msg_t * msg = &uart_msg;
    term_server_config_t *pTsCfg = TS_CFG_GET(uart_id);

    
    term_id = UART_TO_TERM_SERV(uart_id);
    if((text_uart_date[term_id].wr_pos+1)%TERM_SRV_UART_MAX_SIZE == text_uart_date[term_id].rd_pos) {
        //full 
        buffer = NULL;
        TS_DBG("UART[%d] full. wrpos %d, rdpos %d,wrcount %d, rdcount %d\n", uart_id,
                                                    text_uart_date[term_id].wr_pos, ts_uart_ring[term_id].rd_pos,
                                                    text_uart_date[term_id].wr_count, ts_uart_ring[term_id].rd_count);  
    }
    else {
        buffer = (cs_uint8 *)&text_uart_date[term_id].data[0];
        offset = text_uart_date[term_id].wr_pos;
    }
    
    if(buffer) {
        while(CS_E_OK == ts_getc(uart_id,&buffer[offset])) {
            pTsCfg->uart_rx_char_cnt++;
            text_uart_date[term_id].wr_count ++;
            offset = (offset + 1) % TERM_SRV_UART_MAX_SIZE;
        }
        text_uart_date[term_id].wr_pos = offset;
    }
    else {
        cs_uint8 temp;
        while(CS_E_OK == ts_getc(uart_id,&temp)) {
        }
    }  
   return CS_E_OK;
   
}
#endif


extern cs_status uart_ip_info_get_from_global(cs_uint32 *uart_ip, cs_uint32 *uart_mask, cs_uint32 *uart_gateway, cs_uint16 *uart_vlan);


/*----------------------------------------------
**
**         TCP Connection
**
**---------------------------------------------*/
static cs_status ts_tcp_socket_setup(term_server_config_t* pTerm_serv_cfg)
{
    struct sockaddr_in serv_addr;
    cs_uint32 index;
    cs_int32 on = 1;
    cs_int32 ret = 0;
    ret = get_userdata_from_flash((unsigned char *)&g_slow_path_ip_cfg, GWD_PRODUCT_CFG_OFFSET_W, sizeof(g_slow_path_ip_cfg));
	if(ret)
		{
			cs_printf("get ipdate fail..\n");
		}
    term_server_config_t *pTsCfg = TS_CFG_GET(pTerm_serv_cfg->uart_cfg.uart_id);
    
    index = UART_TO_TERM_SERV(pTerm_serv_cfg->uart_cfg.uart_id);
    pTsCfg->client_fd = -1;
    /* create socket*/
    if((pTsCfg->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        TS_DBG("socket failed\n");
        return CS_E_ERROR;
    }

	#if 0
	cs_printf("pTerm_serv_cfg->uart_cfg.uart_id :0x%x\n", pTerm_serv_cfg->uart_cfg.uart_id);
	cs_printf("pTsCfg->server_fd :0x%x\n", pTsCfg->server_fd);
	#endif
    setsockopt(pTsCfg->server_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

	cs_uint32	uart_ip;
	cs_uint32	uart_mask;
	cs_uint32	uart_gateway;
	cs_uint16	uart_vlan;
	uart_ip_info_get_from_global(&uart_ip, &uart_mask, &uart_gateway, &uart_vlan);
	serv_addr.sin_addr.s_addr = htonl(uart_ip);
	
	#if 0
	cs_printf("in ts_tcp_socket_setup, uart_ip :0x%x\n", uart_ip);
	cs_printf("in ts_tcp_socket_setup, port :%d\n", pTerm_serv_cfg->proto_port);
	#endif
	
    serv_addr.sin_port = htons(pTerm_serv_cfg->proto_port);
	#if 1
	int opt=1;
	setsockopt(pTsCfg->server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	#endif
    ret = bind(pTsCfg->server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret < 0){
        TS_DBG("bind failed, ret %d, uart[%d], server fd %d, port %d,\n", ret,
            pTerm_serv_cfg->uart_cfg.uart_id, pTsCfg->server_fd, serv_addr.sin_port);
        ts_server_fd_close(pTsCfg);
		#if 1
		cs_printf("bind failed\n");
		#endif
        return CS_E_ERROR;
    }

   /* only accept one client */
    if(listen(pTsCfg->server_fd, 1) < 0){
        close(pTsCfg->server_fd);
        pTsCfg->server_fd = -1;
        TS_DBG("listen failed\n");
		#if 1
		cs_printf("listen failed\n");
		#endif
        return CS_E_ERROR;
    }
    
    return CS_E_OK;

}
cyg_uint32 LOCAL_IP = (cyg_uint32)0xc0a80008;
static cs_status tc_tcp_socket_setup(term_server_config_t* pTerm_serv_cfg)
{
    struct sockaddr_in serv_addr;
    cs_uint32 index;
    cs_int32 on = 1;
    cs_int32 ret = 0;
    ret = get_userdata_from_flash((unsigned char *)&g_slow_path_ip_cfg, GWD_PRODUCT_CFG_OFFSET_W, sizeof(g_slow_path_ip_cfg));
	if(ret)
		{
			cs_printf("get ipdate fail..\n");
		}
    term_server_config_t *pTsCfg = TS_CFG_GET(pTerm_serv_cfg->uart_cfg.uart_id);
    
    index = UART_TO_TERM_SERV(pTerm_serv_cfg->uart_cfg.uart_id);
    pTsCfg->server_fd = -1;
    /* create socket*/
    if((pTsCfg->client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        TS_DBG("socket failed\n");
        return CS_E_ERROR;
    }
    setsockopt(pTsCfg->client_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(pTsCfg->remote_server_ip);
	serv_addr.sin_port = htons(pTerm_serv_cfg->remote_server_port);

	#if 1
	if(connect(pTsCfg->client_fd,(struct sockaddr *)(&serv_addr),sizeof(struct sockaddr)) == -1)
		{
			cs_printf("Connect Error:%s\a\n",strerror(errno));
			cs_printf("..................................(547)connect error\n");
			cs_thread_delay(5000);
			return CS_E_ERROR;
		}
	cs_printf("...................(547)client create success\n");
	#else
	int connect_status =0;
	while(1)
	{
		connect_status = connect(pTsCfg->client_fd,(struct sockaddr *)(&serv_addr),sizeof(struct sockaddr));
		if(-1 == connect_status)
		{
			cs_printf("Connect Error *********\n");
		}
		else
		{
			cs_printf("connect seccess***********\n");
			break;
		}
	}
	#endif
	return CS_E_OK;

}
#if 0
static cs_uint8 ts_dummy_buf[TS_BUF_SIZE];
static void ts_tcp_receive(term_server_config_t* pTerm_serv_cfg)
{
   socklen_t cli_len;
   fd_set in_fds;
   struct timeval   timeout;
   int len,sr;
   struct timeval * p_timeout = NULL; /* default never timeout*/
   net_msg_t * buffer;
   int dbg_loop;
   unsigned int index;
   struct sockaddr_in client_addr;  // connector's address information  

    while (1)
    {
       if(TERM_SERV_ENABLE_FLAG != pTerm_serv_cfg->enable_flag){
            cs_thread_delay(1000);
            TERM_DBG_PERROR("setsockopt SO_SNDTIMEO");  
            break;
       }
      
       memset(&client_addr, 0, sizeof(client_addr));
       cli_len = sizeof(struct sockaddr_in );
       if ((pTerm_serv_cfg->client_fd = accept(pTerm_serv_cfg->server_fd,
                            (struct sockaddr *)&(client_addr),&cli_len))<0) {
           cs_printf("accept error \n"); 
            break;
       }
       else
       {
            pTerm_serv_cfg->client_ip   = ntohl(client_addr.sin_addr.s_addr);
            pTerm_serv_cfg->client_port = ntohs(client_addr.sin_port);
            pTerm_serv_cfg->client_closed_flag = 0;
            
            while(1)
            {
                if(TERM_SERV_ENABLE_FLAG != pTerm_serv_cfg->enable_flag){
                    break;
                }
                
                FD_ZERO(&in_fds);
                FD_SET(pTerm_serv_cfg->client_fd, &in_fds); 
                /* client timeout setting */
                if(pTerm_serv_cfg->client_timeout > 0){
                    timeout.tv_sec = pTerm_serv_cfg->client_timeout;
                    timeout.tv_usec = 0;
                    p_timeout = (struct timeval *)&timeout;
                }
                index = UART_TO_TERM_SERV(pTerm_serv_cfg->uart_cfg.uart_id);
                              
                if ((sr = select(pTerm_serv_cfg->client_fd + 1, &in_fds, NULL, NULL, p_timeout)) < 0){
                     if (errno == EINTR){
                            continue;
                      }
    
                     cs_printf("select\n");
                     break;
                }
        
                if (0 == sr){
                     cs_printf("select, socket closed \n");
                    break;
                }
        
                if((buffer = ring_buffer_get(pTerm_serv_cfg)) == NULL){
                    //cs_thread_delay(10);
                    //continue;
                    buffer = (net_msg_t *)ts_dummy_buf;
                }
                
                memset(buffer, 0, TS_BUF_SIZE);
                if((len = read(pTerm_serv_cfg->client_fd, buffer->net_data, 
                                      TS_BUF_SIZE-sizeof(net_msg_t))) <= 0){
                     cs_printf("read, socket closed \n");
                     break;
                }

                pTerm_serv_cfg->term_serv_rx_ip_packets ++;
                pTerm_serv_cfg->term_serv_rx_ip_cnt += len;
                if(term_serv_dbg){
                    cs_printf("Server %d received Message : ",index);
                    for( dbg_loop = 0; dbg_loop < len; dbg_loop++)   	
                    cs_printf("%x ",(buffer->net_data[dbg_loop]));
                    cs_printf(" \n");
                }
                //  ts_uart_send(pTerm_serv_cfg->uart_cfg.uart_id,buffer,len);
                if(buffer != (net_msg_t *)ts_dummy_buf) {
                    buffer->length = len;
                    buffer->uart_num = pTerm_serv_cfg->uart_cfg.uart_id;
                    ts_net_msg_send(buffer);
                }
                else {
                    //ts_net_msg_send(pTerm_serv_cfg.);
                }
            }
            cs_semaphore_wait(ts_sem_id[UART_TO_TERM_SERV(pTerm_serv_cfg->uart_cfg.uart_id)], CS_OSAL_WAIT_FOREVER);
            if(pTerm_serv_cfg->client_fd >= 0) {
                   pTerm_serv_cfg->client_fd = -1; 
                   cs_printf("%s, %d \n", __func__, __LINE__); 
                    close(pTerm_serv_cfg->client_fd);
                   cs_printf("%s, %d \n", __func__, __LINE__); 
            }
            cs_semaphore_post(ts_sem_id[UART_TO_TERM_SERV(pTerm_serv_cfg->uart_cfg.uart_id)]);

        
            pTerm_serv_cfg->client_closed_flag = 1;
            pTerm_serv_cfg->term_serv_rx_ip_packets = 0;
            pTerm_serv_cfg->term_serv_rx_ip_cnt = 0;
            pTerm_serv_cfg->uart_rx_char_cnt = 0;
            pTerm_serv_cfg->uart_tx_char_cnt = 0;
            pTerm_serv_cfg->term_serv_tx_ip_packets = 0;
            pTerm_serv_cfg->term_serv_tx_lost_packets = 0;
            pTerm_serv_cfg->term_serv_tx_ip_cnt = 0;
            pTerm_serv_cfg->term_serv_tx_lost_cnt = 0;
       }

    }
}
#else
cs_int32 ts_new_conn_proc(term_server_config_t* pTs_cfg)
{
    socklen_t cli_len;
    struct sockaddr_in client_addr;  // connector's address information  
    
    memset(&client_addr, 0, sizeof(client_addr));
    cli_len = sizeof(struct sockaddr_in);
    pTs_cfg->client_fd = accept(pTs_cfg->server_fd,(struct sockaddr *)&(client_addr),&cli_len);
    if(pTs_cfg->client_fd < 0){
        TS_DBG("server fd %d accept error\n", pTs_cfg->server_fd); 
        return -1;
    }
    pTs_cfg->client_ip = ntohl(client_addr.sin_addr.s_addr);
    pTs_cfg->client_port = ntohs(client_addr.sin_port);
    pTs_cfg->client_closed_flag = 0;
    ts_uart_ring[UART_TO_TERM_SERV(pTs_cfg->uart_cfg.uart_id)].rd_count = 0;
    ts_uart_ring[UART_TO_TERM_SERV(pTs_cfg->uart_cfg.uart_id)].wr_count = 0;
    ts_uart_ring[UART_TO_TERM_SERV(pTs_cfg->uart_cfg.uart_id)].rd_pos = 0;
    ts_uart_ring[UART_TO_TERM_SERV(pTs_cfg->uart_cfg.uart_id)].wr_pos = 0;
    pTs_cfg->ip_rx_buf.pEgress = 0;
    pTs_cfg->ip_rx_buf.pIngress = 0;
    pTs_cfg->ip_rx_buf.wr_count = 0;
    pTs_cfg->ip_rx_buf.rd_count = 0;

    TS_DBG("server fd %d accept new connect fd %d\n", pTs_cfg->server_fd, pTs_cfg->client_fd); 
    return 0;
}
static cs_uint8 ts_dummy_buf[TS_BUF_SIZE];
#define TS_MAX(a, b) ((a) > (b))?(a):(b) 
cs_int32 ts_client_data_proc(term_server_config_t* pTs_cfg)
{
    cs_int32 len;
    net_msg_t * buffer;
    if((buffer = ring_buffer_get(pTs_cfg)) == NULL){
		#if 1
		cs_printf("buffer = (net_msg_t *)ts_dummy_buf;\n");
		#endif
        buffer = (net_msg_t *)ts_dummy_buf;
    }
    len = read(pTs_cfg->client_fd, buffer->net_data, TS_BUF_SIZE-sizeof(net_msg_t));
    if(len <= 0){
        TS_DBG("read, uart[%d] socket(client) fd %d closed \n", pTs_cfg->uart_cfg.uart_id, pTs_cfg->client_fd);
        return -1;
    }
    pTs_cfg->term_serv_rx_ip_packets ++;
    pTs_cfg->term_serv_rx_ip_cnt += len;

#if 0
	cs_printf("\n*************************\n");
	int i = 0;
	for(i=0;i<len;i++)
	{
		cs_printf("0x%x, ", buffer->net_data[i]);
	}
	cs_printf("\n*************************\n");
#endif

    if(buffer != (net_msg_t *)ts_dummy_buf) {
        buffer->length = len;
        buffer->uart_num = pTs_cfg->uart_cfg.uart_id;
        ts_net_msg_send(buffer);
    }
    
    return 0;
    
}
static void ts_tcp_receive(term_server_config_t* pTerm_serv_cfg)
{
    cs_int32 fdmax;
    cs_int32 sr;
    fd_set rfds;
    struct timeval  tv;
    struct timeval  *p_timeout = NULL;

	#if 1
	cs_uint8 uart_id = 0;
	uart_id = pTerm_serv_cfg->uart_cfg.uart_id;
	#if 0
	cs_printf("uart_id :0x%x\n", uart_id);
	#endif
	pTerm_serv_cfg = TS_CFG_GET(uart_id);
	#if 0
	cs_printf("pTerm_serv_cfg->uart_cfg.uart_id :0x%x\n", pTerm_serv_cfg->uart_cfg.uart_id);
	cs_printf("pTerm_serv_cfg->server_fd :0x%x\n", pTerm_serv_cfg->server_fd);
	#endif
	
	#endif
	
    while (pTerm_serv_cfg->server_fd >= 0)
    {
        FD_ZERO(&rfds);
        FD_SET(pTerm_serv_cfg->server_fd, &rfds);
        fdmax = pTerm_serv_cfg->server_fd;
        TS_DBG("%s, uart[%d], server fd %d, %d\n", __func__, pTerm_serv_cfg->uart_cfg.uart_id, 
                                                                    pTerm_serv_cfg->server_fd, __LINE__);
        if((sr = select(fdmax + 1, &rfds, NULL, NULL, NULL)) <= 0){
            if(errno == EINTR){//Interrupt
                continue;
            }
            TS_DBG("select(for server) error or timeout\n");
            ts_server_fd_close(pTerm_serv_cfg);
            break;
        }

        if(FD_ISSET(pTerm_serv_cfg->server_fd, &rfds)) {
            if(ts_new_conn_proc(pTerm_serv_cfg) < 0) {
                ts_server_fd_close(pTerm_serv_cfg);
            }
            break;
        }
    }
    /**/
    
    while (pTerm_serv_cfg->client_fd >= 0)
    {
        FD_ZERO(&rfds);
        FD_SET(pTerm_serv_cfg->client_fd, &rfds);
        fdmax = pTerm_serv_cfg->client_fd;
        if(pTerm_serv_cfg->client_timeout > 0){
            tv.tv_sec = pTerm_serv_cfg->client_timeout;
            tv.tv_usec = 0;
            p_timeout = &tv;
        }
        if((sr = select(fdmax + 1, &rfds, NULL, NULL, p_timeout)) <= 0){
            if(errno == EINTR ){//Interrupt 
                continue;
            }
            TS_DBG("select(for client) error or timeout\n");
            if((cyg_current_time() - pTerm_serv_cfg->last_act_ticks)/100 < pTerm_serv_cfg->client_timeout) {
                TS_DBG("uart[%d] still active, server fd %d, client fd %d\n", 
                    pTerm_serv_cfg->uart_cfg.uart_id, pTerm_serv_cfg->server_fd, pTerm_serv_cfg->client_fd);
                continue;
            }
            ts_client_fd_close(pTerm_serv_cfg);
            break;
        }
		#if 0
        while(pTerm_serv_cfg->client_fd >= 0) {
            if(ts_client_data_proc(pTerm_serv_cfg) < 0){
                ts_client_fd_close(pTerm_serv_cfg);
                break;
            }
        }
		#else
		if(FD_ISSET(pTerm_serv_cfg->client_fd, &rfds))
		{
			if(ts_client_data_proc(pTerm_serv_cfg) < 0){
                ts_client_fd_close(pTerm_serv_cfg);
                break;
            }
		}
		#endif
    }
    
    TS_DBG("%s , uart[%d], client closed.\n", __func__, pTerm_serv_cfg->uart_cfg.uart_id);  
    pTerm_serv_cfg->client_closed_flag = 1;
    pTerm_serv_cfg->term_serv_rx_ip_packets = 0;
    pTerm_serv_cfg->term_serv_rx_ip_cnt = 0;
    pTerm_serv_cfg->term_serv_tx_ip_packets = 0;
    pTerm_serv_cfg->term_serv_tx_ip_cnt = 0;
    pTerm_serv_cfg->term_serv_tx_lost_packets = 0;
    pTerm_serv_cfg->term_serv_tx_lost_cnt = 0;
    pTerm_serv_cfg->uart_rx_char_cnt = 0;
    pTerm_serv_cfg->uart_tx_char_cnt = 0;
    
}
cs_int32 ts_server_data_proc(term_server_config_t* pTs_cfg)
{
    cs_int32 len;
    net_msg_t * buffer;
	//cs_printf("server date proc(721)\n");
    if((buffer = ring_buffer_get(pTs_cfg)) == NULL){
        buffer = (net_msg_t *)ts_dummy_buf;
    }
	//cs_printf("client_fd:%d\n",pTs_cfg->client_fd);
    len = read(pTs_cfg->client_fd, buffer->net_data, TS_BUF_SIZE-sizeof(net_msg_t));
    if(len <= 0){
        TS_DBG("read, uart[%d] socket(client) fd %d closed \n", pTs_cfg->uart_cfg.uart_id, pTs_cfg->client_fd);
        return -1;
    }
    pTs_cfg->term_serv_rx_ip_packets ++;
    pTs_cfg->term_serv_rx_ip_cnt += len;

    if(buffer != (net_msg_t *)ts_dummy_buf) {
        buffer->length = len;
        buffer->uart_num = pTs_cfg->uart_cfg.uart_id;
        ts_net_msg_send(buffer);
    }
    
    return 0;
    
}

extern cs_status tc_disable(cs_uint8 uart);
static void tc_tcp_receive(term_server_config_t* pTerm_serv_cfg)
{
	#if 1
//	cs_printf("in tc_tcp_receive\n");
	
	cs_uint8 uart_id = 0;
	uart_id = pTerm_serv_cfg->uart_cfg.uart_id;
//	cs_printf("uart_id :0x%x\n", uart_id);
	pTerm_serv_cfg = TS_CFG_GET(uart_id);
//	cs_printf("pTerm_serv_cfg->enable_flag :0x%x\n", pTerm_serv_cfg->enable_flag);
	#endif
	
	#if 0
	
	#else
    cs_int32 fdmax;
    cs_int32 sr;
    fd_set rfds;
    struct timeval  tv;
    struct timeval  *p_timeout = NULL;	
	#endif

	

    while(pTerm_serv_cfg->client_fd >= 0) 
	{
		#if 0
        if(ts_server_data_proc(pTerm_serv_cfg) < 0)
		{
            ts_client_fd_close(pTerm_serv_cfg);
            break;
        }
		#else
		FD_ZERO(&rfds);
        FD_SET(pTerm_serv_cfg->client_fd, &rfds);
        fdmax = pTerm_serv_cfg->client_fd;
        if(pTerm_serv_cfg->client_timeout > 0){
            tv.tv_sec = pTerm_serv_cfg->client_timeout;
            tv.tv_usec = 0;
            p_timeout = &tv;
        }
        if((sr = select(fdmax + 1, &rfds, NULL, NULL, p_timeout)) <= 0)
		{
            if(errno == EINTR )
			{//Interrupt 
                continue;
            }
            TS_DBG("select(for client) error or timeout\n");
            if((cyg_current_time() - pTerm_serv_cfg->last_act_ticks)/100 < pTerm_serv_cfg->client_timeout) 
			{
                TS_DBG("uart[%d] still active, server fd %d, client fd %d\n", 
                    pTerm_serv_cfg->uart_cfg.uart_id, pTerm_serv_cfg->server_fd, pTerm_serv_cfg->client_fd);
                continue;
            }
            ts_client_fd_close(pTerm_serv_cfg);
            break;
        }
        if(FD_ISSET(pTerm_serv_cfg->client_fd, &rfds))
		{
            if(ts_server_data_proc(pTerm_serv_cfg) < 0)
			{
                ts_client_fd_close(pTerm_serv_cfg);
                break;
            }
        }
		#endif
    }
	tc_disable(pTerm_serv_cfg->uart_cfg.uart_id);
    TS_DBG("%s , uart[%d], client closed.\n", __func__, pTerm_serv_cfg->uart_cfg.uart_id);  
    pTerm_serv_cfg->client_closed_flag = 1;
    pTerm_serv_cfg->term_serv_rx_ip_packets = 0;
    pTerm_serv_cfg->term_serv_rx_ip_cnt = 0;
    pTerm_serv_cfg->term_serv_tx_ip_packets = 0;
    pTerm_serv_cfg->term_serv_tx_ip_cnt = 0;
    pTerm_serv_cfg->term_serv_tx_lost_packets = 0;
    pTerm_serv_cfg->term_serv_tx_lost_cnt = 0;
    pTerm_serv_cfg->uart_rx_char_cnt = 0;
    pTerm_serv_cfg->uart_tx_char_cnt = 0;	  
}

#endif

extern cs_status uart_ip_info_get_from_global(cs_uint32 *uart_ip, cs_uint32 *uart_mask, cs_uint32 *uart_gateway, cs_uint16 *uart_vlan);

/*----------------------------------------------
**
**           UDP Connection
**
**---------------------------------------------*/
static cs_status ts_udp_socket_setup(term_server_config_t* pTerm_serv_cfg)
{
    struct sockaddr_in serv_addr;
    //cs_uint32 index;
    struct timeval tv;
    cs_int32 on = 1;

	#if 1
	cs_uint32	uart_ip = 0;
	cs_uint32	uart_mask = 0;
	cs_uint32	uart_gateway = 0;
	cs_uint16	uart_vlan = 0;
	uart_ip_info_get_from_global(&uart_ip, &uart_mask, &uart_gateway, &uart_vlan);
	#endif
	
	#if 0
	cs_printf("pTerm_serv_cfg->uart_cfg.uart_id :0x%x\n", pTerm_serv_cfg->uart_cfg.uart_id);
	#endif
    term_server_config_t *pTsCfg = TS_CFG_GET(pTerm_serv_cfg->uart_cfg.uart_id);

	#if 1
	pTerm_serv_cfg = TS_CFG_GET(pTerm_serv_cfg->uart_cfg.uart_id);
	#endif
    
    // setup socket
    if((pTsCfg->server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        TS_DBG("ts socket setup failed");
        return CS_E_ERROR;
    }
    
   setsockopt(pTsCfg->server_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    /* set time out option */    
    tv.tv_sec  = pTsCfg->max_res_time / 1000;
    tv.tv_usec = (pTsCfg->max_res_time % 1000) * 1000;
    if (setsockopt(pTsCfg->server_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < -1) {  
        TS_DBG("setsockopt SO_RCVTIMEO");  
       // return (-1);  
    }  
    
    tv.tv_sec  = 1;
    tv.tv_usec = 0;    
    if (setsockopt(pTsCfg->server_fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < -1) {  
        TS_DBG("setsockopt SO_SNDTIMEO");  
       // return (-1);  
    }  

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_len = sizeof(serv_addr);
	#if 0
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	#else
	serv_addr.sin_addr.s_addr = htonl(uart_ip);
	#endif
    serv_addr.sin_port = htons(pTerm_serv_cfg->proto_port);
    if (bind(pTsCfg->server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        TS_DBG("bind failed, uart[%d], server fd %d, port %d,\n", 
            pTerm_serv_cfg->uart_cfg.uart_id, pTsCfg->server_fd, serv_addr.sin_port);
        close(pTsCfg->server_fd);
        pTsCfg->server_fd = -1;
        return CS_E_ERROR;
    }

	#if 0
	socket_status_set(pTerm_serv_cfg);
	pTsCfg->enable_flag = TERM_SERV_ENABLE_FLAG;
	pTsCfg->client_ip = 0;
	pTsCfg->ip_rx_buf.pEgress = 0;
    pTsCfg->ip_rx_buf.pIngress = 0;
    pTsCfg->ip_rx_buf.wr_count = 0;
    pTsCfg->ip_rx_buf.rd_count = 0;
	#endif
	
    return CS_E_OK;

}
/**/
static void ts_udp_receive(term_server_config_t* pTerm_serv_cfg)
{
	#if 0
	cs_printf("in ts_udp_receive************\n");
	#endif
	socklen_t cli_len;
	fd_set in_fds;
	struct timeval   timeout;
	cs_int32 len,sr;
	struct timeval * p_timeout = NULL; /* default never timeout*/
	net_msg_t * buffer;
	cs_int32 dbg_loop;
	cs_uint32 index;
	struct sockaddr_in client_addr;

	 
	#if 1
	cs_uint8 uart_id = 0;
	uart_id = pTerm_serv_cfg->uart_cfg.uart_id;
	pTerm_serv_cfg = TS_CFG_GET(uart_id); 
	#endif

	#if 0
	cs_printf("pTerm_serv_cfg->server_fd :0x%x\n", pTerm_serv_cfg->server_fd);
	#endif
	while(pTerm_serv_cfg->server_fd >= 0)
	{
		if(TERM_SERV_ENABLE_FLAG != pTerm_serv_cfg->enable_flag)
		{
			#if 0
			cs_printf("TERM_SERV_ENABLE_FLAG != pTerm_serv_cfg->enable_flag \n");
			#endif
			break;
		}

		index = UART_TO_TERM_SERV(pTerm_serv_cfg->uart_cfg.uart_id);

		cli_len = sizeof(struct sockaddr_in);

		/* client timeout setting */
		if(pTerm_serv_cfg->client_timeout > 0)
		{
			timeout.tv_sec = pTerm_serv_cfg->client_timeout;
			timeout.tv_usec = 0;
			p_timeout = (struct timeval *)&timeout;
		}

		if((buffer = ring_buffer_get(pTerm_serv_cfg)) == NULL)
		{
			//cs_thread_delay(10);
			//continue;
			#if 0
			cs_printf(" buffer = (net_msg_t *)ts_dummy_buf; \n");
			#endif
			buffer = (net_msg_t *)ts_dummy_buf;
		}

		memset(buffer, 0, TS_BUF_SIZE);
		memset(&client_addr, 0, sizeof(client_addr));

		
		FD_ZERO(&in_fds);
		FD_SET(pTerm_serv_cfg->server_fd, &in_fds); 
		sr = select(pTerm_serv_cfg->server_fd + 1, &in_fds, NULL, NULL, p_timeout);
		if(sr < 0)
		{
			/* select error */
			if (errno == EINTR)
			{
				continue;
			}

			TS_DBG("select(UDP for client) error\n");
			/* error kick off client */
			break;
		}
		else if (0 == sr)
		{
			/* timeout kick off client */
			TS_DBG("select(UDP for client) timeout\n");
			break;
		}
		else
		{
			#if 0
			cs_printf("sr :0x%x\n", sr);
			#endif
			if(FD_ISSET(pTerm_serv_cfg->server_fd, &in_fds))
			{
				if ((len = recvfrom(pTerm_serv_cfg->server_fd, buffer->net_data,
							TS_BUF_SIZE-sizeof(net_msg_t),0,
							(struct sockaddr *)&client_addr,&cli_len)) < 0	) 
				 {
					   TS_DBG("recvfrom fd %d error %d\n", pTerm_serv_cfg->server_fd, len);
					   #if 0
					   cs_printf("recvfrom error\n");
					   #endif
					   break;
				 }
				 else
				 {
				 	#if 0
					cs_printf("recvfrom success, len :0x%x\n", len);
					cs_printf("pTerm_serv_cfg->client_ip :0x%x\n", pTerm_serv_cfg->client_ip);
					cs_printf("pTerm_serv_cfg->client_port :0x%x\n", pTerm_serv_cfg->client_port);
					cs_printf("ntohl(client_addr.sin_addr.s_addr) :0x%x\n", ntohl(client_addr.sin_addr.s_addr));
					cs_printf("ntohs(client_addr.sin_port) :0x%x\n", ntohs(client_addr.sin_port));
					#endif
					 /* First client or equal to first client */
					 if((0 == pTerm_serv_cfg->client_ip) ||
						((ntohl(client_addr.sin_addr.s_addr) == pTerm_serv_cfg->client_ip) &&
						 (ntohs(client_addr.sin_port) == pTerm_serv_cfg->client_port))){
						 #if 0
						 cs_printf("Client connection setup update flag \n");
						 #endif
						 /* Client connection setup update flag */
						 pTerm_serv_cfg->client_closed_flag = 0;
						 pTerm_serv_cfg->client_ip	 = ntohl(client_addr.sin_addr.s_addr);
						 pTerm_serv_cfg->client_port = ntohs(client_addr.sin_port);
						 pTerm_serv_cfg->term_serv_rx_ip_packets ++;
						 pTerm_serv_cfg->term_serv_rx_ip_cnt += len;
						 if(term_serv_dbg){
							 cs_printf("Server %d received Message : ",index);
							 for( dbg_loop = 0; dbg_loop < len; dbg_loop++)
							  cs_printf("%x ",(buffer->net_data[dbg_loop]));
							 cs_printf(" \n");
						 }
						 //ts_uart_send(pTerm_serv_cfg->uart_cfg.uart_id,buffer, len);
						 if(buffer != (net_msg_t *)ts_dummy_buf) {
							 buffer->length = len;
							 buffer->uart_num = pTerm_serv_cfg->uart_cfg.uart_id;
							 ts_net_msg_send(buffer);
						 }
						 //  cs_thread_delay(50);
					 }
				  }
			
			}
		}

	}

	#if 0
	cs_printf("after udp recvfrom*****\n");
	#endif
	#if 0
    /* error or timeout kick off client */
    ts_client_fd_close(pTerm_serv_cfg);
	#else
	//ts_server_fd_close(pTerm_serv_cfg);
	#endif
    /* Client closed update flag */
    pTerm_serv_cfg->client_closed_flag = 1;
    pTerm_serv_cfg->term_serv_rx_ip_packets = 0;
    pTerm_serv_cfg->term_serv_rx_ip_cnt = 0;
    pTerm_serv_cfg->uart_rx_char_cnt = 0;
    pTerm_serv_cfg->uart_tx_char_cnt = 0;
    pTerm_serv_cfg->term_serv_tx_ip_packets = 0;
    pTerm_serv_cfg->term_serv_tx_lost_packets = 0;
    pTerm_serv_cfg->term_serv_tx_ip_cnt = 0;
    pTerm_serv_cfg->term_serv_tx_lost_cnt = 0;
     
}

/* Send data from UART to socket */
static cs_int32 sock_send(term_server_config_t * pTerm_serv_cfg,cs_uint8 *buffer, cs_int32 length)
{
#if 0
	cs_printf("in sock_send\n");
	cs_printf("pTerm_serv_cfg->proto_type :0x%x\n", pTerm_serv_cfg->proto_type);
	cs_printf("pTerm_serv_cfg->server_enable :0x%x\n", pTerm_serv_cfg->server_enable);
	cs_printf("pTerm_serv_cfg->server_fd :0x%x\n", pTerm_serv_cfg->server_fd);
	cs_printf("pTerm_serv_cfg->uart_cfg.uart_id :0x%x\n", pTerm_serv_cfg->uart_cfg.uart_id);
	buf_print(buffer, length);
#endif
    cs_int32 count = 0;
    struct sockaddr_in   client_addr;

#if 0    
    if (NULL == pTerm_serv_cfg || NULL == buffer || length < 1) 
	{
        return 0;
    }
	if(pTerm_serv_cfg->server_enable)
	{
	    if (IPPROTO_TCP == pTerm_serv_cfg->proto_type && pTerm_serv_cfg->server_fd >= 0) 
		{
	        count = send(pTerm_serv_cfg->server_fd, buffer, length, 0); // echo
	    }
		else
		{
			//do nothing
		}
	}
	else
	{
	    if (IPPROTO_TCP == pTerm_serv_cfg->proto_type && pTerm_serv_cfg->client_fd >= 0) 
		{
	        count = send(pTerm_serv_cfg->client_fd, buffer, length, 0); // echo
	    }
		else
		{
			//do nothing
		}
	}
	
    if(IPPROTO_UDP == pTerm_serv_cfg->proto_type)
	{
    
        client_addr.sin_family = AF_INET;  
        client_addr.sin_port   = htons(pTerm_serv_cfg->client_port); 
        client_addr.sin_addr.s_addr = htonl(pTerm_serv_cfg->client_ip); 
        memset(client_addr.sin_zero, 0, sizeof(client_addr.sin_zero));

        count = sendto(pTerm_serv_cfg->server_fd, buffer, length, 0,
                (struct sockaddr*)&client_addr, sizeof(client_addr));
    }
#endif
	if(IPPROTO_TCP == pTerm_serv_cfg->proto_type)
	{
		if(pTerm_serv_cfg->server_enable)
		{
			if(pTerm_serv_cfg->server_fd >= 0)
			{
				#if 1
				count = send(pTerm_serv_cfg->client_fd, buffer, length, 0); // echo
				#else
				count = send(pTerm_serv_cfg->server_fd, buffer, length, 0); // echo
				#endif
				
			}
			else
			{
				// do nothing
			}
		}
		else if(pTerm_serv_cfg->client_enable)
		{
			if(pTerm_serv_cfg->client_fd >= 0)
			{
				count = send(pTerm_serv_cfg->client_fd, buffer, length, 0); // echo
			}
			else
			{
				// do nothing
			}
		}
		else
		{
			// do nothing
		}
	}
	else if(IPPROTO_UDP == pTerm_serv_cfg->proto_type)
	{
		client_addr.sin_family = AF_INET;  
        client_addr.sin_port   = htons(pTerm_serv_cfg->client_port); 
        client_addr.sin_addr.s_addr = htonl(pTerm_serv_cfg->client_ip); 
        memset(client_addr.sin_zero, 0, sizeof(client_addr.sin_zero));

        count = sendto(pTerm_serv_cfg->server_fd, buffer, length, 0,
                (struct sockaddr*)&client_addr, sizeof(client_addr));
	}
	else
	{
		// do nothing
	}

    return count;
    
}

static cs_status check_sock_cfg(
    term_server_config_t *pTerm_serv_cfg
)
{
   // cs_uint32 i = 0;
    
    if (NULL == pTerm_serv_cfg) {
        return CS_E_PARAM;
    }
    
    if (IPPROTO_TCP != pTerm_serv_cfg->proto_type 
        && IPPROTO_UDP != pTerm_serv_cfg->proto_type) {
        return CS_E_NOT_SUPPORT;
    }

    /*0-reserved 21-FTP 22-SSH 23-Telnet 25-SMTP 80-HTTP 161-SNMP 513-login*/
    if (pTerm_serv_cfg->proto_port > MAX_SOCKET_PORT 
          || pTerm_serv_cfg->proto_port < MIN_SOCKET_PORT){
        TERM_DBG_PRINT("Reserved TCP/UDP port %d!\n", pTerm_serv_cfg->proto_port);
        return CS_E_NOT_SUPPORT;
    }
    
    if (0 == pTerm_serv_cfg->client_timeout) {
        TERM_DBG_PRINT("Client will be never timeout!\n");
    }

    if (pTerm_serv_cfg->client_timeout > MAX_REMOTE_TIMEOUT) {
        TERM_DBG_PRINT("Remote timeout time %d is out of range(0 ~ %d) second!\n", 
            pTerm_serv_cfg->client_timeout, MAX_REMOTE_TIMEOUT);
        return CS_E_NOT_SUPPORT;
    }
        
    if (pTerm_serv_cfg->max_payload > TS_MAX_PAYLOAD_CEILING
        || pTerm_serv_cfg->max_payload < TS_MAX_PAYLOAD_FLOOR) {
        TERM_DBG_PRINT("MAX payload %d is out of range(%d ~ %d)!\n", 
            pTerm_serv_cfg->max_payload, TS_MAX_PAYLOAD_FLOOR, TS_MAX_PAYLOAD_CEILING);
        return CS_E_NOT_SUPPORT;
    }
    
    if (pTerm_serv_cfg->min_payload > pTerm_serv_cfg->max_payload
        || pTerm_serv_cfg->min_payload < TS_MIN_PAD_SIZE) {
        
        TERM_DBG_PRINT("MIN payload %d is out of range(%d ~ max_pad)!\n", 
            pTerm_serv_cfg->min_payload, TS_MIN_PAD_SIZE);
        return CS_E_NOT_SUPPORT;
    }
    
    if (pTerm_serv_cfg->max_res_time < TS_MIN_RSP_TIME
        || pTerm_serv_cfg->max_res_time > TS_MAX_RSP_TIME ) {
        TERM_DBG_PRINT("MAX response time %d is out of range(%d ~ %d)!\n", 
            pTerm_serv_cfg->max_res_time, TS_MIN_RSP_TIME, TS_MAX_RSP_TIME);        
        return CS_E_NOT_SUPPORT;
    }

    /* Check if other terminal is using this port */
	#ifdef CLIENT_TCP
    for(i = 0; i < MAX_TERM_SERV_NUM; i ++){
        if((terminal_server_cfg[i]->proto_port == pTerm_serv_cfg->proto_port) 
            && (terminal_server_cfg[i]->proto_type == pTerm_serv_cfg->proto_type)){
            cs_printf("Port %d is already inused\r\n",pTerm_serv_cfg->proto_port);
            return CS_E_CONFLICT;
        }
    }
    #endif
    return CS_E_OK;
    
}

extern cs_status tc_enable(term_server_config_t* pTerm_serv_cfg);

/*----------------------------------------------
**
**         Terminal server threads
**
**---------------------------------------------*/
static void ts_net_rx_process(term_server_config_t* pTerm_serv_cfg)
{
#if 0
	//cs_status ret;
    while (true)
    {
        if(TERM_SERV_ENABLE_FLAG != pTerm_serv_cfg->enable_flag){
          cyg_thread_delay(600); // delay 600 ticks, that is, 6 seconds

          pTerm_serv_cfg->ret = tc_enable(pTerm_serv_cfg);

           // TS_DBG("ts_net_rx_process uard %d\n", pTerm_serv_cfg->uart_cfg.uart_id);  
        }
        else
        {
            if(IPPROTO_TCP == pTerm_serv_cfg->proto_type && (!pTerm_serv_cfg->ret)){
				cyg_thread_delay(200);
                tc_tcp_receive(pTerm_serv_cfg);
				if(1 == 0)
				{
					ts_tcp_receive(pTerm_serv_cfg);
				}
            }
            else if(IPPROTO_UDP == pTerm_serv_cfg->proto_type && (!pTerm_serv_cfg->ret)){
                ts_udp_receive(pTerm_serv_cfg);
            }
        }
    }
#else
	#if 0
	uart_enable_semaphore_init(pTerm_serv_cfg->uart_cfg.uart_id);
	#endif
//	cyg_thread_delay(600); // delay 600 ticks, that is, 6 seconds
//	cs_printf("cyg_thread_delay(600)\n");
	cs_thread_delay(2000);
	while(1)
	{
		if(1 == socket_status_get(pTerm_serv_cfg))		/*已建立socket*/
		{
			#if 1
			cs_printf("has created socket.\n");
			#endif
			if(IPPROTO_TCP == pTerm_serv_cfg->proto_type)	/*tcp 协议*/
			{
				if(0 != pTerm_serv_cfg->server_enable)	/* sever  服务器模式*/
				{
					#if 1
					cs_printf("ts_tcp_receive\n");
					#endif
					ts_tcp_receive(pTerm_serv_cfg);
					#if 1
					cs_printf("after ts_tcp_receive\n");
					#endif
				}
				else if(0 != pTerm_serv_cfg->client_enable)		/*client 客户端模式*/
				{
					#if 1
					cs_printf("tc_tcp_receive\n");
					#endif
					tc_tcp_receive(pTerm_serv_cfg);
					#if 1
					cs_printf("after tc_tcp_receive\n");
					#endif
				}
				else
				{
					// do nothing
					#if 1
					cs_printf("wrong pTerm_serv_cfg server/client\n");
					socket_status_clear(pTerm_serv_cfg);
					cs_thread_delay(2000);
					#endif
				}
			}
			else if(IPPROTO_UDP == pTerm_serv_cfg->proto_type)	/*udp 协议*/
			{
				#if 1
				cs_printf("ts_udp_receive\n");
				#endif
				ts_udp_receive(pTerm_serv_cfg);
				#if 1
				cs_printf("after ts_udp_receive\n");
				#endif
				#if 1
				cs_thread_delay(2000);
				#endif
			}
			else
			{
				//do nothing
				cs_thread_delay(2000); // delay 300 ticks, that is, 3 seconds
				#if 0
				cs_printf("cs_thread_delay(2000) 1257\n");
				cs_printf("wrong pTerm_serv_cfg->proto_type\n");
				#endif
			}			
		}
		else									/*未建立socket,需建立*/
		{
//			cs_printf("cs_thread_delay(2000) 1264\n");
//			cs_thread_delay(2000);
			//cs_printf("create socket. 1266\n");
			#if 0
			if(1 == pTerm_serv_cfg->uart_cfg.uart_id)
			{
				cs_printf("pTerm_serv_cfg->socket_control :0x%x\n", pTerm_serv_cfg->socket_control);
			}
			#endif
			
			#if 1
			if(0 == pTerm_serv_cfg->socket_control)		//是否要创建socket
			{
				#if 0
				if(1 == pTerm_serv_cfg->uart_cfg.uart_id)
				{
					cs_printf("***uart 1 dont create socket\n");
				}
				else
				{
				
				}
				#endif
				cs_thread_delay(10000);
				continue;
			}
			else
			{
				//do nothing
			}
			#endif
			#if 0
			uart_enable_semaphore_wait(pTerm_serv_cfg->uart_cfg.uart_id);
			cs_printf("pTerm_serv_cfg->uart_cfg.uart_id :0x%x\n", pTerm_serv_cfg->uart_cfg.uart_id);
			cs_printf("pTerm_serv_cfg->proto_type :0x%x\n", pTerm_serv_cfg->proto_type);
			cs_printf("pTerm_serv_cfg->server_enable :0x%x\n", pTerm_serv_cfg->server_enable);
			cs_printf("pTerm_serv_cfg->client_enable :0x%x\n", pTerm_serv_cfg->client_enable);
			#endif
			if(IPPROTO_TCP == pTerm_serv_cfg->proto_type)
			{
				#if 0
				if(1 == pTerm_serv_cfg->uart_cfg.uart_id)
				{
					cs_printf("pTerm_serv_cfg->proto_type :0x%x (1590)\n", pTerm_serv_cfg->proto_type);
				}
				#endif
				
				if(0 != pTerm_serv_cfg->server_enable)
				{
					#if 0
					cs_printf("ts_enable. 1271\n");
					#endif
					cs_status status = CS_E_OK;
					status = ts_enable(pTerm_serv_cfg);	/*建立 tcp server socket*/
					if(CS_E_OK == status)
					{
						//do nothing
						#if 1
						cs_printf("ts_enable success\n");
						cs_thread_delay(2000);
						#endif
					}
					else
					{
						cs_printf("ts_enable failed\n");
						cs_thread_delay(2000);
					}
				}
				else if(0 != pTerm_serv_cfg->client_enable)
				{
					#if 1
					cs_printf("tc_enable.\n");
					#endif
					cs_status status = CS_E_OK;
					status = tc_enable(pTerm_serv_cfg);	/*建立 tcp client socket*/
					if(CS_E_OK == status)
					{
						//do nothing
						cs_printf("tc_enable success\n");
						cs_thread_delay(2000);
					}
					else
					{
						cs_printf("tc_enable failed\n");
						cs_thread_delay(2000);
					}
				}
				else
				{
					// do nothing
					//cs_printf("wrong pTerm_serv_cfg server/client 1282\n");
					cs_thread_delay(2000);
				}
			}
			else if(IPPROTO_UDP == pTerm_serv_cfg->proto_type)
			{
				
				#if 0
				if(1 == pTerm_serv_cfg->uart_cfg.uart_id)
				{
					cs_printf("pTerm_serv_cfg->proto_type :0x%x (1647)\n", pTerm_serv_cfg->proto_type);
				}
				#endif
				if(0 != pTerm_serv_cfg->server_enable)
				{

					cs_status status = CS_E_OK;
					#if 0
					status = ts_udp_socket_setup(pTerm_serv_cfg);	/*建立 udp socket*/
					#else
					status = ts_enable(pTerm_serv_cfg);
					#endif
					if(CS_E_OK == status)
					{
						//do nothing
						cs_printf("ts_enable success\n");
						cs_thread_delay(2000);
					}
					else
					{
						cs_printf("ts_enable failed\n");
						cs_thread_delay(2000);
					}
					
				}
				else if(0 != pTerm_serv_cfg->client_enable)
				{
			
				}
				else
				{
					cs_thread_delay(2000);
				}
				

			}
			else
			{
				
				#if 0
				if(1 == pTerm_serv_cfg->uart_cfg.uart_id)
				{
					cs_printf("pTerm_serv_cfg->proto_type :0x%x (1678)\n", pTerm_serv_cfg->proto_type);
				}
				#endif
				
				//do nothing
//			cs_printf("cs_thread_delay(2000) 1293\n");
//				cs_thread_delay(2000);
				#if 1
			//	cs_printf("wrong pTerm_serv_cfg->proto_type 1295\n");
				cs_thread_delay(2000);
				#endif
			}
		}

	}
#endif
       
}

static void ts_net_rx_thread(cs_uint32 data )
{
    ts_net_rx_process(terminal_server_cfg[data]);  
}

static void ts_def_uart_rx_func
(
    cs_int32 uart, unsigned char *data, cs_int32 len
)
{
#if 0
	cs_printf("in ts_def_uart_rx_func, uart :0x%x\n", uart);
	buf_print(data, len);
#endif
    term_server_config_t  *ps = NULL;

    if (ts_uart_id_check(uart)) {
        return ;
    }
      
    ps = terminal_server_cfg[UART_TO_TERM_SERV(uart)];
    
    if (TERM_SERV_ENABLE_FLAG != ps->enable_flag){
        TERM_DBG_PRINT("Term is not enable!\n");
        return;
    }
    
    TERM_DBG_PRINT("Uart %d send to client len %d!\n", uart, len);
    if (sock_send(ps, data, len) > 0) {
        ps->term_serv_tx_ip_packets ++;
        ps->term_serv_tx_ip_cnt += len;
    }
    else {
        ps->term_serv_tx_lost_packets ++;
        ps->term_serv_tx_lost_cnt += len;
    }
    
    return;
    
}

#if 0
cs_uint16 ts_uart2net_proc_status = 0;		//0-没有在处理串口信息；1-在处理串口信息
extern cs_status ts_uart2net_proc_status_set(cs_uint16 status)
{
	cs_status ret = CS_E_OK;
	if(0 == status)
	{
		ts_uart2net_proc_status = 0;
	}
	else
	{
		ts_uart2net_proc_status = 1;
	}
	
	return ret;
}

extern cs_status ts_uart2net_proc_status_get(cs_uint16 *status)
{
	cs_status ret = CS_E_OK;
	if(0 == ts_uart2net_proc_status)
	{
		*status = 0;
	}
	else
	{
		*status = 1;
	}
	
	return ret;
}
#endif

cyg_sem_t uart2net_sem;

/* process transimit packets from uart to network */
static void ts_uart2net_thread(void)
{
    cs_uint8 term_id = 0;
	#if 0
    uart_msg_t *msg = NULL;
    cs_uint32 ret = 0;
    cs_uint32 size_copy=0;
	#endif

	#if 1
	cyg_semaphore_init(&uart2net_sem,0);
	#endif
    while(1)
    {
    	#if 0
        msg = NULL;
        ret = cs_queue_get(msgq_ts_uart_rx,(void *)&msg,
                sizeof(cs_uint32),&size_copy,CS_OSAL_WAIT_FOREVER);
		#endif

		#if 1
		cyg_semaphore_wait(&uart2net_sem);
		#endif
        /* time out case */
        for(term_id=0; term_id<MAX_TERM_SERV_NUM; term_id++) 
		{
            if(TS_ENABLED(term_id))
			{
                if(ts_uart_ring[term_id].wr_count - ts_uart_ring[term_id].rd_count > 0)
				{
					//cs_printf("uart to  net sed....................................\n");
                    ts_uart2net_proc(term_id);
                }
            }
        }

    }
}

/* process transimit packets from network to uart */
static void ts_net2uart_thread(void)
{
    uart_msg_t *msg = NULL;
    cs_uint32 size_copy = 0;
    
    while(1)
    {
    	#if 1
        msg = NULL;
        cs_queue_get(msgq_ts_net_rx,(void *)&msg,
                             sizeof(cs_uint32),&size_copy,CS_OSAL_WAIT_FOREVER);
        
        if(NULL == msg){
            continue;
        }
		#endif
		#if 0
        if(STOP_UART_SEND_RCE)
        	{
		        cs_queue_get(msgq_ts_net_rx,(void *)&msg,
	                 sizeof(cs_uint32),&size_copy,CS_OSAL_WAIT_FOREVER);
		        
		        if(NULL == msg){
		            continue;
		        }
        		ts_net_msg_proc((net_msg_t *)msg);
        	}
		else
			{
				cs_printf(".......................................................1\n");
				cyg_thread_delay(40);
			}
		#else
		ts_net_msg_proc((net_msg_t *)msg);
		#endif
    }
}
static void ts_thread_create(void)
{
    /* The tasks process receiveing IP packets */
    cs_thread_create(&ip_rx0_thread_id, "Term Serv Rx0",
            ts_net_rx_thread,(void *)0,TERM_SERV_THREAD_STACKSIZE,
            TERM_SERV_IP_RX_THREAD_PRIORITY, 0);        
    cs_printf("term_server0_rx_thread created\n");

	
    cs_thread_create(&ip_rx1_thread_id, "Term Serv Rx1",
            ts_net_rx_thread,(void *)1,TERM_SERV_THREAD_STACKSIZE,
            TERM_SERV_IP_RX_THREAD_PRIORITY, 0);        
    cs_printf("term_server1_rx_thread created\n");

	#if 1
    cs_thread_create(&ip_rx2_thread_id, "Term Serv Rx2",
            ts_net_rx_thread,(void *)2,TERM_SERV_THREAD_STACKSIZE,
            TERM_SERV_IP_RX_THREAD_PRIORITY, 0);        
    cs_printf("term_server2_rx_thread created\n");

    cs_thread_create(&ip_rx3_thread_id, "Term Serv Rx3",
            ts_net_rx_thread,(void *)3,TERM_SERV_THREAD_STACKSIZE,
            TERM_SERV_IP_RX_THREAD_PRIORITY, 0);        
    cs_printf("term_server3_rx_thread created\n");
	#endif

    /* The task process transmitting UART data */
    cs_thread_create(&uart2net_thread_id, "Term Serv uart2net",
            ts_uart2net_thread,NULL,TERM_SERV_THREAD_STACKSIZE,
            TERM_SERV_UART2NET_THREAD_PRIORITY, 0);        
    cs_printf("ts_uart2net_thread created\n");

    /* The task process transmitting net data to uart*/
    cs_thread_create(&net2uart_thread_id, "Term Serv net2uart",
            ts_net2uart_thread,NULL,TERM_SERV_THREAD_STACKSIZE,
            TERM_SERV_NET2UART_THREAD_PRIORITY, 0);        
    cs_printf("ts_net2uart_thread created\n");
    
}

/*----------------------------------------------
**
**         Terminal Server Management
**
**---------------------------------------------*/
/* Save current configuration into flash use data area. */
void ts_cfg_save(cs_uint8 uart)
{
#ifdef HAVE_DB_MANAGMENT
    db_ts_cfg_t db_ts_cfg;
    term_server_config_t *pTsCfg = NULL;
    

    if(CS_E_OK != ts_uart_id_check(uart)){
        cs_printf("Invalid UART%d \n",uart);
        return;
    }
    pTsCfg = TS_CFG_GET(uart);

   /* Check if uart has been initialized */
    if(VALID_CONFIG_FLAG != pTsCfg->init_flag){
        cs_printf("Invalid configure for UART%d \n",uart);
        return;
    }
    
    /* uart config */
    db_ts_cfg.uart_id = pTsCfg->uart_cfg.uart_id;
    db_ts_cfg.data_bits = pTsCfg->uart_cfg.data_bits;
    db_ts_cfg.stop_bits = pTsCfg->uart_cfg.stop_bits;
    db_ts_cfg.flow_control = pTsCfg->uart_cfg.flow_control;
    db_ts_cfg.parity  = pTsCfg->uart_cfg.parity;
    db_ts_cfg.duplex = pTsCfg->uart_cfg.duplex;
    db_ts_cfg.mode = pTsCfg->uart_cfg.mode;
    db_ts_cfg.baud_rate = pTsCfg->uart_cfg.baud_rate;
    
    /* protocol config*/
    db_ts_cfg.proto_type = pTsCfg->proto_type;
    db_ts_cfg.proto_port = pTsCfg->proto_port;
    db_ts_cfg.client_timeout = pTsCfg->client_timeout;
    db_ts_cfg.max_payload = pTsCfg->max_payload;
    db_ts_cfg.min_payload = pTsCfg->min_payload;
    db_ts_cfg.max_res_time = pTsCfg->max_res_time;
	#if 1
    db_ts_cfg.init_flag = VALID_CONFIG_FLAG;
	db_ts_cfg.remote_server_ip = pTsCfg->remote_server_ip;
	db_ts_cfg.remote_server_port = pTsCfg->remote_server_port;
	#endif
	
	#if 1
	db_ts_cfg.model_flag = pTsCfg->client_enable;
	cs_printf("db_ts_cfg.model_flag : 0x%x\n", db_ts_cfg.model_flag);
	cs_printf("db_ts_cfg.remote_server_ip : 0x%x\n", db_ts_cfg.remote_server_ip);
	cs_printf("db_ts_cfg.remote_server_port : 0x%x\n", db_ts_cfg.remote_server_port);
	#endif
    ts_save_config(uart,(cs_uint8 *)&db_ts_cfg, sizeof(db_ts_cfg_t));
#endif
}

/* Clear these terminal servers saving configuration to default. */
void ts_cfg_clear(cs_uint8 uart)
{
#ifdef HAVE_DB_MANAGMENT
    db_ts_cfg_t db_ts_cfg;

    memset(&db_ts_cfg, 0x00,sizeof(db_ts_cfg));
    ts_save_config(uart,(cs_uint8 *)&db_ts_cfg, sizeof(db_ts_cfg_t));
#endif    
}

extern cs_status Pterm_serv_cheak(term_server_config_t* pTerm_serv_cfg,term_server_config_t *pTsCfg);

/* Create a terminal server and attach it with a given uart. */
cs_status ts_enable(term_server_config_t* pTerm_serv_cfg)
{
#if 0
	cs_printf("in ts_enable****\n");
#endif
    cs_status status = CS_E_OK;
 //   cs_uint32 i = 0;
    term_server_config_t *pTsCfg = NULL;
    cs_status ret = CS_E_OK;

	#if 1
	cs_uint8 uart = 0;
	uart = pTerm_serv_cfg->uart_cfg.uart_id;
	#if 0
	cs_printf("uart :0x%x\n", uart);
	#endif
	pTerm_serv_cfg = TS_CFG_GET(uart);
	#if 0
	cs_printf("pTerm_serv_cfg->enable_flag :0x%x\n", pTerm_serv_cfg->enable_flag);
	#endif
	if(TERM_SERV_ENABLE_FLAG == pTerm_serv_cfg->enable_flag)
	{
		cs_printf("uart server/client has enabled\n");
		socket_status_set(pTerm_serv_cfg);
		return CS_E_OK;
	}
	else
	{
		//do nothing
	}
	#endif

	
	ret = Pterm_serv_cheak(pTerm_serv_cfg,pTsCfg);
	if(CS_E_OK != ret)
	{
		cs_printf("ret = 0x%x\n", ret);
		cs_printf("in ts_enable\n");
		cs_printf("Pterm_serv_cheak error\n");
		return CS_E_ERROR;
	}
	else
	{
		// do nothing
	}

	#if 1
	pTsCfg = TS_CFG_GET(uart);
	#endif
	
    ts_uart_init((uart_config_t *)&pTerm_serv_cfg->uart_cfg);

    if(IPPROTO_TCP == pTerm_serv_cfg->proto_type)
	{
		#if 0
		cs_printf("tcp\n");
		#endif
        status = ts_tcp_socket_setup(pTerm_serv_cfg);
    }
    else if(IPPROTO_UDP == pTerm_serv_cfg->proto_type)
	{
		#if 0
		cs_printf("udp\n");
		#endif
        status = ts_udp_socket_setup(pTerm_serv_cfg);
    }
	else
	{
		cs_printf("wrong pTerm_serv_cfg.\n");
	}

    if(CS_E_OK == status)
	{
		pTsCfg->client_closed_flag = 0;
        pTsCfg->enable_flag = TERM_SERV_ENABLE_FLAG;
		pTsCfg->server_enable = 1;
		pTsCfg->client_enable = 0;
		socket_status_set(pTerm_serv_cfg);
    }
	else
	{
		ts_disable(pTerm_serv_cfg->uart_cfg.uart_id);
	}
    return status;

}

extern cs_status socket_status_clear(term_server_config_t* pTerm_serv_cfg);


/* Disable the service completely and free the related IP/UART */
/* communication resource. */
void ts_disable(cs_uint8 uart)
{
    cs_uint32 i;
	cs_printf("uart :0x%x\n", uart);
    term_server_config_t *pTsCfg = TS_CFG_GET(uart);

	#if 0
	cs_printf("pTsCfg->enable_flag :0x%x\n", pTsCfg->enable_flag);
	#endif

#if 0	
	if(TERM_SERV_ENABLE_FLAG != pTsCfg->enable_flag)
	{
		cs_printf("uart server has been disabled\n");
	}
	else
	{
		//do nothing
	}
#endif
	
	#if 1
	socket_status_clear(pTsCfg);
	#endif
    /* Disable UART */
    ts_onu_uart_disable(uart);
    
    
     /* TCP socket: kick off client connection */
    if(IPPROTO_TCP == pTsCfg->proto_type){
        ts_client_fd_close(pTsCfg);
    }
    
    ts_server_fd_close(pTsCfg);
#if 1
    for(i=0; i<MAX_RING_BUF_NUM; i++)
	{
        iros_free(pTsCfg->ip_rx_buf.pBuf[i]);
		pTsCfg->ip_rx_buf.pBuf[i] = NULL;
    }
#endif 
   
//    memset(pTsCfg,0,sizeof(term_server_config_t));
    pTsCfg->client_fd = -1;
    pTsCfg->server_fd = -1;
	pTsCfg->server_enable = 0;
    /* Client closed update flag */
    pTsCfg->client_closed_flag = 1;
	pTsCfg->enable_flag = 0;
}

/* Terminate the current remote connect and reset the service to */
/* wait for new connections. */
void ts_reset(cs_uint8 uart)
{
    term_server_config_t *pTsCfg = TS_CFG_GET(uart);
    /* TCP socket: kick off client connection */
    if(IPPROTO_TCP == pTsCfg->proto_type){
        ts_client_fd_close(pTsCfg);
    }
    /* Client closed update flag */
    pTsCfg->term_serv_rx_ip_packets = 0;
    pTsCfg->term_serv_tx_ip_packets = 0;
    pTsCfg->term_serv_tx_lost_packets = 0;
    pTsCfg->term_serv_rx_ip_cnt = 0;
    pTsCfg->term_serv_tx_ip_cnt = 0;
    pTsCfg->term_serv_tx_lost_cnt = 0;
    /* clear the client information */
    pTsCfg->client_closed_flag = 1;
    pTsCfg->client_ip = 0;
    pTsCfg->client_port = 0;

    pTsCfg->uart_rx_char_cnt = 0;
    pTsCfg->uart_tx_char_cnt = 0;
    
}

/* Dump terminal server status (running or waiting), uart configure */
/* parameters, rx/tx IP packets and remote IP & port numbers.*/
void ts_show(cs_uint8 uart)
{
    cs_uint32 index = 0;
    struct in_addr ipv4;
    uart_config_t *pUart = NULL;
    term_server_config_t *pTsCfg = TS_CFG_GET(uart);
    
    index = UART_TO_TERM_SERV(uart);
    pUart = (uart_config_t *)&(pTsCfg->uart_cfg);
    ipv4.s_addr = htonl(pTsCfg->client_ip);
    if(TERM_SERV_ENABLE_FLAG !=pTsCfg->enable_flag) {
        cs_printf("%-30s: %s\n", "Terminal Server Status", "Disabled");
    }
    else if(0 == pTsCfg->client_closed_flag)
    {
        cs_printf("%-30s: %s\n", "Terminal Server Status", "Running");
        cs_printf("%-30s: %s\n", "Remote connect IP",  inet_ntoa(ipv4));
        cs_printf("%-30s: %d\n", "Remote connect port", ntohs(pTsCfg->client_port));
    }
    else {
        cs_printf("%-30s: %s %d\n", "Terminal Server Status", "Waiting at port",
                                                                pTsCfg->proto_port);
    }
    cs_printf("%-30s: %d\n", "Term Server Rx IP packets", pTsCfg->term_serv_rx_ip_packets);
    cs_printf("%-30s: %d\n", "Term Server Rx IP count", pTsCfg->term_serv_rx_ip_cnt);
    cs_printf("%-30s: %d\n", "Term Server Tx IP packets", pTsCfg->term_serv_tx_ip_packets);
    cs_printf("%-30s: %d\n", "Term Server Tx IP count", pTsCfg->term_serv_tx_ip_cnt);
    cs_printf("%-30s: %d\n", "Term Server Tx lost packets", pTsCfg->term_serv_tx_lost_packets);
    cs_printf("%-30s: %d\n", "Term Server ip_rx_buf.wr_count", pTsCfg->ip_rx_buf.wr_count);
    cs_printf("%-30s: %d\n", "Term Server ip_rx_buf.rd_count", pTsCfg->ip_rx_buf.rd_count);
    cs_printf("%-30s: %d\n", "Term Server ip_rx_buf.pIngress", pTsCfg->ip_rx_buf.pIngress);
    cs_printf("%-30s: %d\n", "Term Server ip_rx_buf.pEgress", pTsCfg->ip_rx_buf.pEgress);


    cs_printf("%-30s:\n", "Term Server Config");
    cs_printf("%30s: %s%d\n", "Uart ID", "uart",pUart->uart_id);
    cs_printf("%30s: %d\n", "Uart baudrate", pUart->baud_rate);
    cs_printf("%30s: %d\n", "Uart databits", pUart->data_bits);
    cs_printf("%30s: %ld\n", "Uart received bytes: ", pTsCfg->uart_rx_char_cnt);
    cs_printf("%30s: %ld\n", "Uart transimitted bytes: ", pTsCfg->uart_tx_char_cnt);
    switch(pUart->parity) {
            case UART_PARITY_NONE:
        {
            cs_printf("%30s: %s\n", "Uart parity", "None");
            break;
        }
            case UART_PARITY_ODD:
        {
            cs_printf("%30s: %s\n", "Uart parity", "Odd");
            break;
        }
            case  UART_PARITY_EVEN:
        {
            cs_printf("%30s: %s\n", "Uart parity", "Even");
            break;
        }
        default:
            break;
    }
    if(pTsCfg->proto_type == IPPROTO_TCP) {
        cs_printf("%30s: %s\n", "Connect protocol ","TCP");
    }
    else if(pTsCfg->proto_type == IPPROTO_UDP)  {
        cs_printf("%30s: %s\n", "Connect protocol ","UDP");
    }
    else {
        cs_printf("%30s: %s\n", "Connect protocol ","Not set");
    }
    cs_printf("%30s: %d\n", "Connect port", pTsCfg->proto_port);
    cs_printf("%30s: %d s\n", "Connect timeout", pTsCfg->client_timeout);
    cs_printf("%30s: %d \n", "Packet min payload", pTsCfg->min_payload);
    cs_printf("%30s: %d\n", "Packet max payload", pTsCfg->max_payload);
    cs_printf("%30s: %d ms\n", "Server max response time", pTsCfg->max_res_time);
    cs_printf("%30s: %d \n", "Server fd", pTsCfg->server_fd);
    cs_printf("%30s: %d \n", "client fd", pTsCfg->client_fd);
    cs_printf("%-30s: %d\n", "Term Server wr_pos", ts_uart_ring[index].wr_pos);
    cs_printf("%-30s: %d\n", "Term Server wr_count", ts_uart_ring[index].wr_count);
    cs_printf("%-30s: %d\n", "Term Server rd_pos", ts_uart_ring[index].rd_pos);
    cs_printf("%-30s: %d\n", "Term Server rd_count", ts_uart_ring[index].rd_count);
}

/* Auto create terminal server base on user data area */
cs_status Pterm_serv_cheak(term_server_config_t* pTerm_serv_cfg,term_server_config_t *pTsCfg)
{
	cs_uint32 i = 0;
    
    if (NULL == pTerm_serv_cfg) {
		#if 1
		cs_printf("NULL == pTerm_serv_cfg\n");
		#endif
        return  CS_E_PARAM;
    }

	cs_uint8 uart_id = 0;
	uart_id = pTerm_serv_cfg->uart_cfg.uart_id;
    pTsCfg = TS_CFG_GET(uart_id);
	#if 1
	cs_printf("uart_id :0x%x\n", uart_id);
	#endif
    if(TERM_SERV_ENABLE_FLAG == pTsCfg->enable_flag){
        cs_printf("Uart %d has been enabled\r\n",pTerm_serv_cfg->uart_cfg.uart_id);
        return CS_E_CONFLICT;
    }
	//ts_onu_uart_enable(pTerm_serv_cfg->uart_cfg.uart_id);
    if(CS_E_OK != check_uart_cfg(&pTerm_serv_cfg->uart_cfg)){
        cs_printf("Wrong uart Configuration !\n");
        return CS_E_PARAM;
    }
    
    if(CS_E_OK != check_sock_cfg(pTerm_serv_cfg)){
        cs_printf("Wrong TCP/IP Configuration !\n");
        return CS_E_PARAM;
    }
    memcpy(pTsCfg, pTerm_serv_cfg, sizeof(term_server_config_t));

	#if 0
	cs_printf("pTerm_serv_cfg->server_enable :0x%x\n", pTerm_serv_cfg->server_enable);
	#endif
    /* init IP receive ring buffer */
	if(pTerm_serv_cfg->server_enable)
	{
	    for(i=0; i<MAX_RING_BUF_NUM; i++)
		{
	        if((pTsCfg->ip_rx_buf.pBuf[i] = iros_malloc(IROS_MID_SYS,TS_BUF_SIZE)) == NULL)
			{
	            cs_printf("Terminal server enable failed for no memory\r\n");
				
				#if 1
				int j = 0;
				for(j=0;j<i;j++)
				{
					iros_free(pTsCfg->ip_rx_buf.pBuf[j]);
				}
				#endif
				
	            return CS_E_RESOURCE;
	        }
			
			#if 0
			cs_printf("pTsCfg->ip_rx_buf.pBuf[%d] :0x%x\n", i, pTsCfg->ip_rx_buf.pBuf[i]);
			#endif
	    }
	}
	else
	{
		pTerm_serv_cfg->server_enable = 0;
		#if 0
		cs_printf("pTerm_serv_cfg->server_enable = 0x%x\n", pTerm_serv_cfg->server_enable);
		#endif
	}

	return CS_E_OK;
}

cs_status Pterm_client_cheak(term_server_config_t* pTerm_serv_cfg,term_server_config_t *pTsCfg)
{
	cs_uint32 i = 0;
    
    if (NULL == pTerm_serv_cfg)
	{
		#if 1
		cs_printf("NULL == pTerm_serv_cfg\n");
		#endif
        return  CS_E_PARAM;
    }

	cs_uint8 uart_id = 0;
	uart_id = pTerm_serv_cfg->uart_cfg.uart_id;
    pTsCfg = TS_CFG_GET(uart_id);
	#if 0
	cs_printf("uart_id :0x%x\n", uart_id);
	#endif
    if(TERM_SERV_ENABLE_FLAG == pTsCfg->enable_flag)
	{
        cs_printf("Uart %d has been enabled\r\n",pTerm_serv_cfg->uart_cfg.uart_id);
        return CS_E_CONFLICT;
    }
	
	//ts_onu_uart_enable(pTerm_serv_cfg->uart_cfg.uart_id);
    if(CS_E_OK != check_uart_cfg(&pTerm_serv_cfg->uart_cfg))
	{
        cs_printf("Wrong uart Configuration !\n");
        return CS_E_PARAM;
    }
    
    if(CS_E_OK != check_sock_cfg(pTerm_serv_cfg))
	{
        cs_printf("Wrong TCP/IP Configuration !\n");
        return CS_E_PARAM;
    }
	
    memcpy(pTsCfg, pTerm_serv_cfg, sizeof(term_server_config_t));

    /* init IP receive ring buffer */
	if(pTerm_serv_cfg->client_enable)
	{
	    for(i=0; i<MAX_RING_BUF_NUM; i++)
		{
	        if((pTsCfg->ip_rx_buf.pBuf[i] = iros_malloc(IROS_MID_SYS,TS_BUF_SIZE)) == NULL)
			{
	            cs_printf("Terminal server enable failed for no memory\r\n");
				#if 1
				int j = 0;
				for(j=0;j<i;j++)
				{
					iros_free(pTsCfg->ip_rx_buf.pBuf[j]);
				}
				#endif
	            return CS_E_RESOURCE;
	        }
			#if 0
			cs_printf("pTsCfg->ip_rx_buf.pBuf[%d] :0x%x\n", i, pTsCfg->ip_rx_buf.pBuf[i]);
			#endif
	    }
	}
	else
	{
//		pTerm_serv_cfg->client_enable = 0;
		#if 0
		cs_printf("pTerm_serv_cfg->client_enable = 0x%x\n", pTerm_serv_cfg->client_enable);
		#endif
	}

	return CS_E_OK;	
}


extern cs_status tc_disable(cs_uint8 uart)
{
#if 1
	cs_printf("in tc_disable\n");
#endif

	cs_status ret = CS_E_OK;
    cs_uint32 i;
    term_server_config_t *pTsCfg = TS_CFG_GET(uart);

	#if 0
	cs_printf("pTsCfg->enable_flag :0x%x\n", pTsCfg->enable_flag);
	#endif

	#if 0
	if(TERM_SERV_ENABLE_FLAG != pTsCfg->enable_flag)
	{
		cs_printf("uart client has been disabled\n");
		return CS_E_OK;
	}
	else
	{
		//do nothing
	}
	#endif
	
	#if 1
	socket_status_clear(pTsCfg);
	#endif
    /* Disable UART */
    ts_onu_uart_disable(uart);
    
    
     /* TCP socket: kick off client connection */
    if(IPPROTO_TCP == pTsCfg->proto_type){
        ts_client_fd_close(pTsCfg);
    }
    
    ts_server_fd_close(pTsCfg);
#if 1
    for(i=0; i<MAX_RING_BUF_NUM; i++)
	{
        iros_free(pTsCfg->ip_rx_buf.pBuf[i]);
		pTsCfg->ip_rx_buf.pBuf[i] = NULL;
		#if 0
		cs_printf("pTsCfg->ip_rx_buf.pBuf[%d] :0x%x\n", i, pTsCfg->ip_rx_buf.pBuf[i]);
		#endif
    }
#endif 
   
  //  memset(pTsCfg,0,sizeof(term_server_config_t));
    pTsCfg->client_fd = -1;
    pTsCfg->server_fd = -1;
    /* Client closed update flag */
    pTsCfg->client_closed_flag = 1;
    pTsCfg->enable_flag = 0;
	
	return ret;
}

extern cs_status tc_enable(term_server_config_t* pTerm_serv_cfg)
{
    cs_status status;
//    cs_uint32 i = 0;
    term_server_config_t *pTsCfg = NULL;
    cs_status ret = CS_E_OK;
	
	#if 1
	cs_uint8 uart = 0;
	uart = pTerm_serv_cfg->uart_cfg.uart_id;
//	cs_printf("uart :0x%x\n", uart);
	pTerm_serv_cfg = TS_CFG_GET(uart);
	if(TERM_SERV_ENABLE_FLAG == pTerm_serv_cfg->enable_flag)
	{
		cs_printf("uart server/client has enabled\n");
		socket_status_set(pTerm_serv_cfg);
		return CS_E_OK;
	}
	else
	{
		//do nothing
	}
	#endif
	pTsCfg = TS_CFG_GET(uart);
	ret = Pterm_client_cheak(pTerm_serv_cfg,pTsCfg);
	if(CS_E_OK != ret)
	{
		#if 1
		cs_printf("Pterm_serv_cheak error\n");
		#endif
		return CS_E_ERROR;
	}
    
    ts_uart_init((uart_config_t *)&pTerm_serv_cfg->uart_cfg);

    if(IPPROTO_TCP == pTerm_serv_cfg->proto_type)
	{
		#if 0
		cs_printf("tcp\n");
		#endif
        status = tc_tcp_socket_setup(pTerm_serv_cfg);
    }
    else if(IPPROTO_UDP == pTerm_serv_cfg->proto_type)
	{
		#if 0
		cs_printf("udp\n");
		#endif
        status = ts_udp_socket_setup(pTerm_serv_cfg);
    }
	else
	{
		cs_printf("wrong pTerm_serv_cfg->proto_type\n");
	}

	#if 0
	cs_printf("status :0x%x\n", status);
	#endif
    if(CS_E_OK == status)
	{
		pTsCfg->client_closed_flag = 0;
        pTsCfg->enable_flag = TERM_SERV_ENABLE_FLAG;
//		pTsCfg->client_enable = 1;
		pTsCfg->server_enable = 0;
		socket_status_set(pTerm_serv_cfg);
    }
	else
	{
		tc_disable(pTerm_serv_cfg->uart_cfg.uart_id);
	}
    return status;

}

static void pTemp_set_default(cs_uint8 uart_id, db_ts_cfg_t * pTemp);
#define SERVER_MODEL	0
#define CLIENT_MODEL	1
cs_status pConfig_set(cs_uint32 uart_num, term_server_config_t *pConfig)
{
#if 0
	cs_printf("in pConfig_set\n");
#endif
	if(NULL == pConfig)
	{
		return CS_E_ERROR;
	}
	db_ts_cfg_t * pTemp = NULL;
	pTemp = iros_malloc(IROS_MID_SYS,sizeof(db_ts_cfg_t));
	if(NULL == pTemp)
	{
        cs_printf("Terminal server create failed for no memory\r\n");
        iros_free(pConfig);
        return CS_E_ERROR;
    }
    
    memset(pConfig, 0, sizeof(term_server_config_t));
    memset(pTemp, 0, sizeof(db_ts_cfg_t));
    
    ts_get_config(uart_num,(cs_uint8 *)pTemp,sizeof(db_ts_cfg_t));


	/*check saved configuration */
	#if 0
	cs_printf("pTemp->init_flag :0x%x\n", pTemp->init_flag);
	#endif
	if(VALID_CONFIG_FLAG == pTemp->init_flag)
	{
		#if 0
		cs_printf("VALID_CONFIG_FLAG\n");
		#endif
	}
	else
	{
		#if 1
		cs_printf("pTemp_set_default\n");
		#endif
		cs_uint8 uart_id = 0;
		uart_id = uart_num;
		pTemp_set_default(uart_id, pTemp);
		ts_save_config(pTemp->uart_id, (cs_uint8 *)pTemp,sizeof(db_ts_cfg_t));		
	}
	//cs_printf("***pTemp->uart_id :0x%x\n", pTemp->uart_id);
   /* uart config */

	#if 0
	cs_printf("***uart_num :0x%x\n", uart_num);
	#endif
	pConfig->uart_cfg.uart_id = uart_num;
	pConfig->uart_cfg.data_bits = pTemp->data_bits;
	pConfig->uart_cfg.stop_bits = pTemp->stop_bits;
	pConfig->uart_cfg.flow_control = pTemp->flow_control;
	pConfig->uart_cfg.parity = pTemp->parity;
	pConfig->uart_cfg.duplex = pTemp->duplex;
	pConfig->uart_cfg.mode = pTemp->mode;
	pConfig->uart_cfg.baud_rate = pTemp->baud_rate;

	/* protocol config*/
	pConfig->proto_type = pTemp->proto_type;
	pConfig->proto_port = pTemp->proto_port;
	pConfig->client_timeout = pTemp->client_timeout;
	pConfig->max_payload = pTemp->max_payload;
	pConfig->min_payload = pTemp->min_payload;
	pConfig->max_res_time = pTemp->max_res_time;

	pConfig->init_flag = pTemp->init_flag;

	#if 1
	pConfig->remote_server_ip = pTemp->remote_server_ip;
	pConfig->remote_server_port = pTemp->remote_server_port;
	#endif
	
#if 1
//	cs_printf("pTemp->model_flag :0x%x\n", pTemp->model_flag);
	if(SERVER_MODEL == pTemp->model_flag)
	{
		pConfig->server_enable = 1;
		pConfig->client_enable = 0;
	}
	else if(CLIENT_MODEL == pTemp->model_flag)
	{
		pConfig->server_enable = 0;
		pConfig->client_enable = 1;
	}
	else
	{
		//do nothing
		//cs_printf("wrong pTemp->model_flag.\n");
	}
	
#endif
   
	/* clear status saved before */
	pConfig->server_fd = -1;
	pConfig->client_fd = -1;
	pConfig->client_ip= 0;
	pConfig->client_port= 0;
	pConfig->client_closed_flag = 1;
	pConfig->term_serv_rx_ip_packets = 0;
	pConfig->term_serv_rx_ip_cnt = 0;
	pConfig->uart_rx_char_cnt = 0;
	pConfig->uart_tx_char_cnt = 0;
	pConfig->term_serv_tx_ip_packets = 0;
	pConfig->term_serv_tx_lost_packets = 0;
	pConfig->term_serv_tx_ip_cnt = 0;
	pConfig->term_serv_tx_lost_cnt = 0;
	pConfig->enable_flag = 0;
	//pConfig->last_act_ticks = 0;
	

	iros_free(pTemp);
	#if 0
	cs_printf("quit pConfig_set\n");
	#endif
	
	return CS_E_OK;
	
}
extern cs_uint8 socket_status_get(term_server_config_t* pTerm_serv_cfg);

cs_status uart_server_enable(cs_uint8 uart)
{
#if 0
	uart_enable_semaphore_post(uart);
#endif
    term_server_config_t * pConfig = NULL;
    pConfig = TS_CFG_GET(uart);
	if(0 == socket_status_get(pConfig))
	{
		//do nothing
		socket_status_clear(pConfig);
		pConfig->server_enable = 1;
		pConfig->client_enable = 0;
		#if 1
		cs_printf("uart :0x%x\n", uart);
		#endif
		return CS_E_OK;
	}
	else
	{
		#if 1
		cs_printf("uart %d server/client has been enabled\n", uart);
		#endif
		return CS_E_ERROR;
	}
}

extern cs_status uart_server_disable(cs_uint8 uart)
{
	cs_status ret = CS_E_OK;

	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(uart);
	if(1 == pConfig->server_enable)
	{
		pConfig->server_enable = 0;
		pConfig->client_enable = 0;
		ts_disable(uart);
		ret = CS_E_OK;
	}
	else
	{
		ret = CS_ERROR;
	}

	
	return ret;
}


extern cs_uint8 socket_status_get(term_server_config_t* pTerm_serv_cfg);

extern cs_status uart_client_enable(cs_uint8 uart)
{
#if 0
	uart_enable_semaphore_post(uart);
#endif

	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(uart);
	if(0 == socket_status_get(pConfig))
	{
		//do nothing
		socket_status_clear(pConfig);
		pConfig->client_enable = 1;
		pConfig->server_enable = 0;
		#if 1
		cs_printf("uart :0x%x\n", uart);
		#endif
		return CS_E_OK;
	}
	else
	{
		#if 1
		cs_printf("uart %d server/client has been enabled\n", uart);
		#endif
		return CS_E_ERROR;
	}
	
	return CS_E_OK;
}

extern cs_status uart_client_disable(cs_uint8 uart)
{
	cs_status ret = CS_E_OK;

	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(uart);
	if(1 == pConfig->client_enable)
	{
		pConfig->server_enable = 0;
		pConfig->client_enable = 0;
		tc_disable(uart);
		ret = CS_E_OK;
	}
	else
	{
		ret = CS_E_ERROR;
	}

	return ret;	
}

#if 0
static void ts_auto_create(void)
{
#if 1
	//cs_printf("in ts_auto_create\n");
#endif
#ifdef HAVE_DB_MANAGMENT
    cs_uint32 num;
   // db_ts_cfg_t * pTemp = NULL;
    term_server_config_t * pConfig = NULL;
    
    /* Auto create terminal server according to saved data */
    if((pConfig = iros_malloc(IROS_MID_SYS,sizeof(term_server_config_t))) == NULL){
            cs_printf("Terminal server create failed for no memory\r\n");
            return;
    }
	#if	0
    cs_printf("2059\n");
	#endif
    for(num = TS_MIN_UART_NUM; num <= MAX_TERM_SERV_NUM; num++)
    {
    	//cs_printf("num :0x%x\n", num);
            /*check saved configuration */
        memset(pConfig, 0, sizeof(term_server_config_t));

		pConfig_set(num,pConfig);
		//cs_printf("2067\n");
        term_server_config_t *pTsCfg = NULL;
		#if 0
		cs_printf("pConfig->uart_cfg.uart_id :0x%x\n", pConfig->uart_cfg.uart_id);
		#endif
        pTsCfg = TS_CFG_GET(pConfig->uart_cfg.uart_id);
		//cs_printf("pConfig->uart_cfg.uart_id :0x%x\n", pConfig->uart_cfg.uart_id);
		//cs_printf("2070\n");
				
 	   	memcpy(pTsCfg, pConfig, sizeof(term_server_config_t));
	  	//cs_printf("2072\n");
        #if 0
         if(ts_enable(pConfig)){
                 cs_printf("Terminal Server %d create failed\n\r",UART_TO_TERM_SERV(num));
         }
         else {
                 cs_printf("Terminal Server %d create sucess\n\r",UART_TO_TERM_SERV(num));
         }
 	   #endif
    }
    //cs_printf("2082\n");
    iros_free(pConfig);
	#if 1
	//cs_printf("ts_auto_create end\n");
	#endif
	
#endif    
}
#endif

extern cs_status cs_gpio_mode_set(cs_uint8 gpio_id, gpio_mode_t mode);
extern cs_status cs_gpio_write(cs_uint8 gpio_id, cs_uint8 data);
extern int serial_port_module_default_config_recover();
int set_uart_mode_cfg(cs_uint32 uart_port,cs_uint32 act)
{
	cs_callback_context_t   context;
	cs_plat_uart_duplex_set(context,0,0,uart_port,act);
	return 1;
}
int set_uart_cfg(cs_uint32 uart_port,cs_uint32 act)
{
	cs_callback_context_t   context;
    cs_plat_uart_disable(context,0,0,uart_port);
    cs_plat_uart_duplex_set(context,0,0,uart_port,act);
    cs_plat_uart_enable(context,0,0,uart_port);
	return 1;
}

void ts_init(void)
{
    cs_int32 ret = 0;
    cs_uint32 num = 0;
	cs_uint8 gpio_id = 0;
	for(num = 1;num <=MAX_TERM_SERV_NUM; num++)
		{
			switch(num)
				{
					case 1:
						gpio_id = 14;
						break;
					case 2:
						gpio_id = 12;
						break;
					case 3:
						gpio_id = 10;
						break;
					case 4:
						gpio_id = 13;
						break;
						default:
							cs_printf("uart number input error  only <uartnum:1-4>\n");
							return ;
				}
			
			if ( CS_E_OK == cs_gpio_mode_set(gpio_id, GPIO_OUTPUT))
			{		


			   if(cs_gpio_write(gpio_id, 0) == EPON_RETURN_SUCCESS)
				{

					ret=set_uart_mode_cfg(num,1);

				}
									
			}
		}


    for(num = 0; num < MAX_TERM_SERV_NUM; num++)
    {
        if((terminal_server_cfg[num] = iros_malloc(IROS_MID_SYS,sizeof(term_server_config_t))) == NULL){
            cs_printf("Terminal server init failed for no memory\r\n");
            return;
        }
        memset(terminal_server_cfg[num], 0, sizeof(term_server_config_t));
    }

    /* create queue for uart frame RX */
    ret = cs_queue_create(&msgq_ts_uart_rx, "TS_UART_RX_Q", 
             TS_UART_RX_QUEUE_DEPTH, TS_UART_RX_QUEUE_SIZE, 0);
    
    if(CS_E_OSAL_OK != ret) {
        cs_printf("msgq_ts_uart_rx q create failed, 0x%x\n", ret);
        return;
    }

    /* create queue for ethernet frame RX */
    ret = cs_queue_create(&msgq_ts_net_rx, "TS_IP_RX_Q", 
             TS_IP_RX_QUEUE_DEPTH, TS_UART_RX_QUEUE_SIZE, 0);
    
    if(CS_E_OSAL_OK != ret) {
        cs_printf("msgq_ts_net_rx q create failed, 0x%x\n", ret);
        return;
    }

	#if 1
	serial_port_module_default_config_recover();
	#endif
	#if 0
	ts_auto_create();
	#endif
	
    ts_thread_create();

	#if 0
    ts_auto_create();
	#endif
}


/************* Debug ***************************/
void ts_electricity_meter_test(cs_uint8 uart)
{
    static cs_uint8 command[100] = {0xfe,0xfe,0x68,0x99,0x99,0x99,0x99,0x99,0x99,0x68,0x11,0x04,0x33,0x33,0x34,0x33,0x48,0x16};

    ts_uart_send(uart, (cs_uint8 *)command, sizeof(command));
}

/***********End of Debug************************/

static void pTemp_set_default(cs_uint8 uart_id, db_ts_cfg_t * pTemp)
{
	/* uart config */
	pTemp->init_flag = 0xA5;
	pTemp->uart_id = uart_id;
	pTemp->data_bits = 0x8;
	pTemp->stop_bits = 0x0;
	pTemp->flow_control = 0;
	pTemp->parity = 0;
	pTemp->duplex = 0;
	pTemp->mode = 0;
	pTemp->baud_rate = 0;

	/* protocol config*/
	pTemp->proto_type = 0;
	pTemp->proto_port = 0;
	pTemp->client_timeout = 0;
	pTemp->max_payload = 0;
	pTemp->max_res_time = 0;
   	pTemp->model_flag = SERVER_MODEL;
}

#define SOCKET_ENABLE	0x1
#define SOCKET_DISABLE	0x0
/**********************************************************************************************************************************************
*函数名：socket_status_set
*函数功能描述：设置socket 的状态为使能
*函数参数：term_server_config_t* pTerm_serv_cfg
*函数返回值：枚举类型cs_status: CS_E_OK(0)，CS_E_RESOURCE(1),CS_E_PARAM(2),CS_E_NOT_FOUND(3),CS_E_CONFLICT(4),CS_E_TIMEOUT(5)
	,CS_E_NOT_SUPPORT(6),CS_E_ERROR(0xffffffff)
*作者：朱晓辉
*函数创建日期：2013.3.8
*函数修改日期：尚未修改
*修改人：尚未修改
*修改原因：尚未修改
*版本：1.0
*历史版本：无
**********************************************************************************************************************************************/
extern cs_status socket_status_set(term_server_config_t* pTerm_serv_cfg)
{
	cs_status ret = CS_E_OK;
	pTerm_serv_cfg->socket_enable = SOCKET_ENABLE;

#if 0
	if(1 == pTerm_serv_cfg->uart_cfg.uart_id)
	{
		cs_printf("in socket_status_set\n");
		cs_printf("pTerm_serv_cfg->uart_cfg.uart_id :0x%x\n", pTerm_serv_cfg->uart_cfg.uart_id);
		cs_printf("pTerm_serv_cfg->socket_enable :0x%x\n", pTerm_serv_cfg->socket_enable);
		cs_printf("ret :0x%x\n", ret);
	}
	else
	{
		//do nothing
	}
#endif


	
	return ret;
}

/**********************************************************************************************************************************************
*函数名：socket_status_clear
*函数功能描述：清除 socket 的使能状态
*函数参数：term_server_config_t* pTerm_serv_cfg
*函数返回值：枚举类型cs_status: CS_E_OK(0)，CS_E_RESOURCE(1),CS_E_PARAM(2),CS_E_NOT_FOUND(3),CS_E_CONFLICT(4),CS_E_TIMEOUT(5)
	,CS_E_NOT_SUPPORT(6),CS_E_ERROR(0xffffffff)
*作者：朱晓辉
*函数创建日期：2013.3.8
*函数修改日期：尚未修改
*修改人：尚未修改
*修改原因：尚未修改
*版本：1.0
*历史版本：无
**********************************************************************************************************************************************/
extern cs_status socket_status_clear(term_server_config_t* pTerm_serv_cfg)
{
	cs_status ret = CS_E_OK;
	pTerm_serv_cfg->socket_enable = SOCKET_DISABLE;
	
	#if 0
	if(1 == pTerm_serv_cfg->uart_cfg.uart_id)
	{
		cs_printf("in socket_status_clear\n");
		cs_printf("pTerm_serv_cfg->uart_cfg.uart_id :0x%x\n", pTerm_serv_cfg->uart_cfg.uart_id);
		cs_printf("pTerm_serv_cfg->socket_enable :0x%x\n", pTerm_serv_cfg->socket_enable);
		cs_printf("ret :0x%x\n", ret);
	}
	else
	{
		//do nothing
	}
	#endif
	
	return ret;
}


/**********************************************************************************************************************************************
*函数名：socket_status_get
*函数功能描述：获取 socket 的使能状态
*函数参数：term_server_config_t* pTerm_serv_cfg
*函数返回值：socket 的创建状态. cs_uint8, 0-socket 未创建, 1-socket 已创建
*作者：朱晓辉
*函数创建日期：2013.3.8
*函数修改日期：尚未修改
*修改人：尚未修改
*修改原因：尚未修改
*版本：1.0
*历史版本：无
**********************************************************************************************************************************************/
extern cs_uint8 socket_status_get(term_server_config_t* pTerm_serv_cfg)
{
	cs_uint8 ret = 0;
	if(SOCKET_ENABLE == pTerm_serv_cfg->socket_enable)
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}


#if 0
	if(1 == pTerm_serv_cfg->uart_cfg.uart_id)
	{
		cs_printf("in socket_status_get\n");
		cs_printf("pTerm_serv_cfg->uart_cfg.uart_id :0x%x\n", pTerm_serv_cfg->uart_cfg.uart_id);
		cs_printf("pTerm_serv_cfg->socket_enable :0x%x\n", pTerm_serv_cfg->socket_enable);
		cs_printf("ret :0x%x\n", ret);
	}
	else
	{
		//do nothing
	}
#endif




	return ret;
}

extern void ts_cfg_save(cs_uint8 uart);




#if 1

typedef struct uart_ip
{
	cs_uint32 uart_ip;
	cs_uint32 uart_mask;
	cs_uint32 uart_gateway;
	cs_uint16 uart_vlan;
}uart_ip_infor_t;



uart_ip_infor_t uart_ip_infor;

extern cs_status uart_ip_info_save_to_global(cs_uint32 uart_ip, cs_uint32 uart_mask, cs_uint32 uart_gateway, cs_uint16 uart_vlan)
{
	cs_status ret = CS_E_OK;
	uart_ip_infor.uart_ip = uart_ip;
	uart_ip_infor.uart_mask = uart_mask;
	uart_ip_infor.uart_gateway = uart_gateway;
	uart_ip_infor.uart_vlan = uart_vlan;
	
	return ret;
}

extern cs_status uart_ip_info_get_from_global(cs_uint32 *uart_ip, cs_uint32 *uart_mask, cs_uint32 *uart_gateway, cs_uint16 *uart_vlan)
{
	cs_status ret = CS_E_OK;
	if(uart_ip != NULL)
	{
		*uart_ip = uart_ip_infor.uart_ip;
	}
	else
	{
		//do nothing
	}

	if(uart_mask != NULL)
	{
		*uart_mask = uart_ip_infor.uart_mask;
	}
	else
	{
		//do nothing
	}
	
	if(uart_gateway != NULL)
	{
		*uart_gateway = uart_ip_infor.uart_gateway;
	}
	else
	{
		//do nothing
	}

	if(uart_vlan != NULL)
	{
		*uart_vlan = uart_ip_infor.uart_vlan;
	}
	else
	{
		//do nothing
	}
	
	return ret;	
}





/**********************************************************************************************************************************************
*函数名：uart_ip_info_save_to_flash
*函数功能描述：将串口ip 信息保存到flash
*函数参数：
*函数返回值：int ,成功返回0
*作者：朱晓辉
*函数创建日期：2013.5.27
*函数修改日期：尚未修改
*修改人：尚未修改
*修改原因：尚未修改
*版本：1.0
*历史版本：无
**********************************************************************************************************************************************/
extern cs_status uart_ip_info_save_to_flash(cs_uint32 ip, cs_uint32 mask, cs_uint32	gateway, cs_uint16 vlan)
{
	cs_status ret = CS_E_OK;
	onu_slow_path_cfg_cfg_t ip_cfg;
	ret = get_userdata_from_flash((char *)&ip_cfg, GWD_PRODUCT_CFG_OFFSET_W, sizeof(ip_cfg));
	if(0 != ret)
	{
		cs_printf("get ipdate fail..\n");
	}
	else
	{
		//do nothing
		#if 0
		cs_printf("get ip data success\n");
		#endif
	}

	ip_cfg.uart_ip = ip;
	ip_cfg.uart_mask = mask;
	ip_cfg.uart_gateway = gateway;
	ip_cfg.uart_vlan = vlan;

	ret = save_userdata_to_flash((char *)&ip_cfg, GWD_PRODUCT_CFG_OFFSET_W, sizeof(ip_cfg));
	if(0 != ret)
	{
		cs_printf("save_userdata_to_flash failed\n");
	}
	else
	{
		#if 0
		cs_printf("save_userdata_to_flash success\n");
		#endif
	}

	return ret;

}



/**********************************************************************************************************************************************
*函数名：uart_ip_info_get_from_flash
*函数功能描述：从flash 中获取串口ip 信息
*函数参数：
*函数返回值：int ,成功返回0
*作者：朱晓辉
*函数创建日期：2013.5.27
*函数修改日期：尚未修改
*修改人：尚未修改
*修改原因：尚未修改
*版本：1.0
*历史版本：无
**********************************************************************************************************************************************/

extern cs_status uart_ip_info_get_from_flash(cs_uint32 *ip, cs_uint32 *mask, cs_uint32 *gateway, cs_uint16 *vlan)
{
	cs_status ret = CS_E_OK;
	onu_slow_path_cfg_cfg_t ip_cfg;
	ret = get_userdata_from_flash((char *)&ip_cfg, GWD_PRODUCT_CFG_OFFSET_W, sizeof(ip_cfg));
	if(0 != ret)
	{
		cs_printf("get ipdate fail..\n");
	}
	else
	{
		//do nothing
		#if 0
		cs_printf("get ip data success\n");
		#endif
	}

	*ip = ip_cfg.uart_ip;
	*mask = ip_cfg.uart_mask;
	*gateway = ip_cfg.uart_gateway;
	*vlan = ip_cfg.uart_vlan;

	return ret;

}



#endif

#if 1
extern cs_status uart_ip_info_save_to_global(cs_uint32 uart_ip, cs_uint32 uart_mask, cs_uint32 uart_gateway, cs_uint16 uart_vlan);
extern cs_status ip_info_check(cs_uint32 ip, cs_uint32 mask, cs_uint32 gateway, cs_uint16 vlan);
extern cs_status app_ipintf_add_ip_address(cs_uint32 localIp, cs_uint32 gwIp, cs_uint32 mask);

extern cs_status ip_mode_set(int mode);


extern cs_status uart_ip_init(void);

static cs_status uart_ip_set_default(cs_uint32 *ip, cs_uint32 *mask, cs_uint32 *gateway, cs_uint16 *vlan);





/**********************************************************************************************************************************************
*函数名:		uart_ip_init
*函数功能描述：串口ip 的初始化(flash 读取失败后，采用默认配置)
*函数参数：
*函数返回值：cs_status
*作者：朱晓辉
*函数创建日期：2013-5-24
*函数修改日期：尚未修改
*修改人：尚未修改
*修改原因：尚未修改
*版本：1.0
*历史版本：无
**********************************************************************************************************************************************/
extern cs_status uart_ip_init(void)
{
#if 0
	cs_printf("in uart_ip_init\n");
#endif

	cs_uint32 uart_ip = 0;
	cs_uint32 uart_mask = 0;
	cs_uint32 uart_gateway = 0;
	cs_uint16 uart_vlan = 0;

	cs_status ret = CS_E_OK;

	int status = 0;
	status = get_userdata_from_flash((unsigned char *)&g_slow_path_ip_cfg, GWD_PRODUCT_CFG_OFFSET_W, sizeof(g_slow_path_ip_cfg));
	
	if(0 != status)
	{
		cs_printf("get ipdate fail..\n");

		#if 0
		memset(&g_slow_path_ip_cfg, 0, sizeof(g_slow_path_ip_cfg));
		char ip[] = "192.168.0.1";
		char mask[] = "255.255.255.0";
		char gateway[] = "192.168.0.1";
		g_slow_path_ip_cfg.uart_ip = ntohl(inet_addr(ip));
		g_slow_path_ip_cfg.uart_mask = ntohl(inet_addr(mask));
		g_slow_path_ip_cfg.uart_gateway = ntohl(inet_addr(gateway));
		#endif

		uart_ip_set_default(&uart_ip, &uart_mask, &uart_gateway, &uart_vlan);
		g_slow_path_ip_cfg.uart_ip = uart_ip;
		g_slow_path_ip_cfg.uart_mask = uart_mask;
		g_slow_path_ip_cfg.uart_gateway = uart_gateway;
		g_slow_path_ip_cfg.uart_vlan = uart_vlan;
		int status_save = 0;
		status_save = save_userdata_to_flash((unsigned char *)&g_slow_path_ip_cfg, GWD_PRODUCT_CFG_OFFSET_W, sizeof(g_slow_path_ip_cfg));
		if(0 != status_save)
		{
			cs_printf("save_userdata_to_flash failed\n");
		}
		else
		{
			//do nothing
			#if 1
			cs_printf("save_userdata_to_flash success\n");
			#endif
		}
	}
	else
	{
		//do nothing
		uart_ip = g_slow_path_ip_cfg.uart_ip;
		uart_mask = g_slow_path_ip_cfg.uart_mask;
		uart_gateway = g_slow_path_ip_cfg.uart_gateway;
		uart_vlan = g_slow_path_ip_cfg.uart_vlan;
		
		ret = ip_info_check(uart_ip, uart_mask, uart_gateway, uart_vlan);
		if(CS_E_OK == ret)
		{
			//do nothing
			#if 0
			cs_printf("ip_info_check success, uart ip :0x%x\n", uart_ip);
			#endif
		}
		else
		{
			#if 1
			cs_printf("ip_info_check failed, uart ip :0x%x\n", uart_ip);
			cs_printf("wrong ip configure!\n");
			#endif
			
			uart_ip_set_default(&uart_ip, &uart_mask, &uart_gateway, &uart_vlan);
			g_slow_path_ip_cfg.uart_ip = uart_ip;
			g_slow_path_ip_cfg.uart_mask = uart_mask;
			g_slow_path_ip_cfg.uart_gateway = uart_gateway;
			g_slow_path_ip_cfg.uart_vlan = uart_vlan;
			int status_save = 0;
			status_save = save_userdata_to_flash((unsigned char *)&g_slow_path_ip_cfg, GWD_PRODUCT_CFG_OFFSET_W, sizeof(g_slow_path_ip_cfg));
			if(0 != status_save)
			{
				cs_printf("save_userdata_to_flash failed\n");
			}
			else
			{
				//do nothing
				#if 1
				cs_printf("save_userdata_to_flash success\n");
				#endif
			}
		}

		
	}

	#if 1

	uart_ip = g_slow_path_ip_cfg.uart_ip;
	uart_mask = g_slow_path_ip_cfg.uart_mask;
	uart_gateway = g_slow_path_ip_cfg.uart_gateway;
	uart_vlan = g_slow_path_ip_cfg.uart_vlan;
	//uart_ip_info_save_to_global(uart_ip, uart_mask, uart_gateway, uart_vlan);
	ip_mode_set(0);
	app_ipintf_add_ip_address(uart_ip, uart_gateway, uart_mask);
	ip_mode_set(1);
	#endif

	#if 0
	cs_printf("uart_ip :0x%x, uart_gateway :0x%x, uart_mask :0x%x\n", uart_ip, uart_gateway, uart_mask);
	#endif
	
	return ret;
}

/**********************************************************************************************************************************************
*函数名:		uart_ip_set_default
*函数功能描述：将串口 ip 相关信息设置成默认配置
*函数参数：
*函数返回值：cs_status
*作者：朱晓辉
*函数创建日期：2013-5-24
*函数修改日期：尚未修改
*修改人：尚未修改
*修改原因：尚未修改
*版本：1.0
*历史版本：无
**********************************************************************************************************************************************/
static cs_status uart_ip_set_default(cs_uint32 *ip, cs_uint32 *mask, cs_uint32 *gateway, cs_uint16 *vlan)
{
	cs_status ret = CS_E_OK;
	#if 1
	cs_printf("uart ip set default\n");
	#endif
	char ip_default[] = "192.168.0.1";
	char mask_default[] = "255.255.255.0";
	char gateway_default[] = "192.168.0.1";
	*ip =  ntohl(inet_addr(ip_default));
	*mask = ntohl(inet_addr(mask_default));
	*gateway = ntohl(inet_addr(gateway_default));
	
	return ret;
}


#endif

#if 0
static cyg_sem_t uart_enable_sem[4];
static cs_status uart_enable_semaphore_init(cs_uint8 uart)
{
	cs_status ret = CS_E_OK;
	cyg_count32 sem_default = 0;
	cs_uint8 uart_id = 0;
	uart_id = uart - 1;
	cyg_semaphore_init(&uart_enable_sem[uart_id], sem_default);
#if 1
	cs_printf("in uart_enable_semaphore_init********\n");
#endif

	return ret;
}

static cs_status uart_enable_semaphore_post(cs_uint8 uart)
{
	cs_status ret = CS_E_OK;
	cs_uint8 uart_id = 0;
	uart_id = uart - 1;
	cyg_semaphore_post(&uart_enable_sem[uart_id]);
	#if 1
	cs_printf("in uart_enable_semaphore_post********\n");
	#endif
	return ret;
}

static cs_status uart_enable_semaphore_wait(cs_uint8 uart)
{
	cs_status ret = CS_E_OK;
	cs_uint8 uart_id = 0;
	uart_id = uart - 1;
	cyg_semaphore_wait(&uart_enable_sem[uart_id]);
	#if 1
	cs_printf("in uart_enable_semaphore_wait********\n");
	#endif
	return ret;
}
#endif

#if 1
extern cs_status gw_serial_port_baud_set(cs_uint16 serial_port_id, int baud)
{
	#if 0
	cs_printf("in gw_serial_port_baud_set\n");
	cs_printf("serial_port_id :0x%x\n", serial_port_id);
	cs_printf("baud :%d\n", baud);
	#endif
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	pConfig->uart_cfg.baud_rate = baud;

	return ret;
}

cs_status serial_port_buf_malloc(cs_uint16 serial_port_id)
{
	#if 0
	cs_printf("in serial_port_buf_free\n");
	#endif
	
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);

	int i = 0;
	for(i=0; i<MAX_RING_BUF_NUM; i++)
	{
		if((pConfig->ip_rx_buf.pBuf[i] = iros_malloc(IROS_MID_SYS,TS_BUF_SIZE)) == NULL)
		{
			cs_printf("Terminal server enable failed for no memory\r\n");
			int j = 0;
			for(j=0;j<i;j++)
			{
				iros_free(pConfig->ip_rx_buf.pBuf[j]);
			}
			ret = CS_E_ERROR;
			break;
		}
		else
		{
			ret = CS_E_OK;
			continue;
		}

	}
	return ret;
}
cs_status serial_port_buf_free(cs_uint16 serial_port_id)
{
	#if 0
	cs_printf("in serial_port_buf_free\n");
	#endif
	
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);

	cs_uint16 i = 0;
	for(i=0; i<MAX_RING_BUF_NUM; i++)
	{
		iros_free(pConfig->ip_rx_buf.pBuf[i]);
		pConfig->ip_rx_buf.pBuf[i] = NULL;
	}
	ret = CS_E_OK;
	
	return ret;
}
cs_status tcp_server_disable(cs_uint16 serial_port_id)
{
	#if 0
	cs_printf("in tcp_server_disable\n");
	#endif


	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);

	pConfig->enable_flag = 0;
	pConfig->server_enable = 0;
	pConfig->client_enable = 0;

	socket_status_clear(pConfig);
	ts_onu_uart_disable(serial_port_id);
	ts_client_fd_close(pConfig);
	ts_server_fd_close(pConfig);
	#if 1
	pConfig->socket_control = 0;
	#endif

	#if 1
	ret = serial_port_buf_free(serial_port_id);
	#endif

	return ret;
}

cs_status tcp_client_disable(cs_uint16 serial_port_id)
{
	#if 0
	cs_printf("in tcp_client_disable\n");
	#endif
	
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	pConfig->enable_flag = 0;
	pConfig->server_enable = 0;
	pConfig->client_enable = 0;
	
	socket_status_clear(pConfig);
	ts_onu_uart_disable(serial_port_id);
	ts_client_fd_close(pConfig);
	ts_server_fd_close(pConfig);
	#if 1
	pConfig->socket_control = 0;
	#endif
	ret = serial_port_buf_free(serial_port_id);
	
	return ret;	
}

cs_status udp_disable(cs_uint16 serial_port_id)
{
	#if 0
	cs_printf("in udp_disable\n");
	#endif
	
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	pConfig->enable_flag = 0;
	pConfig->server_enable = 0;
	pConfig->client_enable = 0;
	
	socket_status_clear(pConfig);
	ts_onu_uart_disable(serial_port_id);
	ts_server_fd_close(pConfig);
	#if 1
	pConfig->socket_control = 0;
	#endif
	ret = serial_port_buf_free(serial_port_id);

	return ret;		
}

extern cs_status serial_port_connect_disable(cs_uint16 serial_port_id)
{
	#if 0
	cs_printf("in serial_port_connect_disable\n");
	#endif

	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	if(1 == socket_status_get(pConfig))
	{
		if(IPPROTO_TCP == pConfig->proto_type)
		{
			if(0 != pConfig->server_enable)
			{
				//tcp server disable
				ret = tcp_server_disable(serial_port_id);
				if(CS_E_OK == ret)
				{
					cs_printf("tcp_server_disable success\n");
				}
				else
				{
					cs_printf("tcp_server_disable failed\n");
				}
			}
			else if(0 != pConfig->client_enable)
			{
				//tcp client disable
				ret = tcp_client_disable(serial_port_id);
				if(CS_E_OK == ret)
				{
					cs_printf("tcp_client_disable success\n");
				}
				else
				{
					cs_printf("tcp_client_disable failed\n");
				}
			}
			else
			{
				ret = CS_E_ERROR;
			}
		}
		else if(IPPROTO_UDP == pConfig->proto_type)
		{
			//udp disable
			ret = udp_disable(serial_port_id);
			if(CS_E_OK == ret)
			{
				cs_printf("tcp_client_disable success\n");
			}
			else
			{
				cs_printf("tcp_client_disable failed\n");
			}
		}
		else
		{
			ret = CS_E_ERROR;
		}
	}
	else
	{
		pConfig->server_enable = 0;
		pConfig->client_enable = 0;
		cs_printf("serial port %d socket has been disabled\n", serial_port_id);
		ret = CS_E_ERROR;
	}

	return ret;
}

cs_status serial_port_connect_status_show(cs_uint16 serial_port_id)
{
	#if 0
	cs_printf("in serial_port_connect_status_show\n");
	#endif
	
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);

	if(1 == socket_status_get(pConfig))
	{
		if(IPPROTO_TCP == pConfig->proto_type)
		{
			if(0 == pConfig->server_client_mode)
			{
				cs_printf("serial port %d, tcp server has been enabled!\n", serial_port_id);
				
			}
			else if(1 == pConfig->client_enable)
			{
				cs_printf("serial port %d, tcp client has been enabled!\n", serial_port_id);
			}
			else
			{
				//do nothing
			}
		}
		else if(IPPROTO_UDP == pConfig->proto_type)
		{
			cs_printf("serial port %d, udp has been enabled!\n", serial_port_id);
		}
		else
		{
			//do nothing
		}
	}
	else
	{
		cs_printf("serial port %d, socket disabled!\n", serial_port_id);
	}

	return ret;
}
cs_status tcp_server_enable(cs_uint16 serial_port_id)
{
	#if 0
	cs_printf("in tcp_server_enable\n");
	#endif
	
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	if(1 == socket_status_get(pConfig))
	{
		serial_port_connect_status_show(serial_port_id);
		ret = CS_E_ERROR;
	}
	else
	{
		pConfig->server_enable = 1;
		pConfig->client_enable = 0;
		#if 1
		pConfig->socket_control = 1;
		#endif
		ret = CS_E_OK;
	}
	
	return ret;
}

cs_status tcp_client_enable(cs_uint16 serial_port_id)
{
	#if 0
	cs_printf("in tcp_client_enable\n");
	#endif
	
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	if(1 == socket_status_get(pConfig))
	{
		serial_port_connect_status_show(serial_port_id);
		ret = CS_E_ERROR;
	}
	else
	{
		pConfig->server_enable = 0;
		pConfig->client_enable = 1;
		
		#if 1
		pConfig->socket_control = 1;
		#endif

		ret = CS_E_OK;
	}
	
	return ret;
}

cs_status udp_enable(cs_uint16 serial_port_id)
{
	#if 0
	cs_printf("in udp_enable\n");
	#endif
	
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	if(1 == socket_status_get(pConfig))
	{
		serial_port_connect_status_show(serial_port_id);
		ret = CS_E_ERROR;
	}
	else
	{
		pConfig->server_enable = 1;
		pConfig->client_enable = 0;
		#if 1
		pConfig->socket_control = 1;
		#endif
		ret = CS_E_OK;
	}

	return ret;
}



extern cs_status serial_port_connect_enable(cs_uint16 serial_port_id)
{
	#if 1
	cs_printf("in serial_port_connect_enable\n");
	#endif
	
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	if(0 == socket_status_get(pConfig))
	{
		if(IPPROTO_TCP == pConfig->proto_type)
		{
			if(0 == pConfig->server_client_mode)
			{
				//tcp server enable
				ret = tcp_server_enable(serial_port_id);
				if(CS_E_OK == ret)
				{
					cs_printf("tcp_server_enable start...\n");
				}
				else
				{
					cs_printf("tcp_server_enable failed\n");
				}
				
			}
			else if(1 == pConfig->server_client_mode)
			{
				//tcp client enable
				ret = tcp_client_enable(serial_port_id);
				if(CS_E_OK == ret)
				{
					cs_printf("tcp_client_enable start...\n");
				}
				else
				{
					cs_printf("tcp_client_enable failed\n");
				}
				
			}
			else
			{
				ret = CS_E_ERROR;
			}
		}
		else if(IPPROTO_UDP == pConfig->proto_type)
		{
			//udp enable
			ret = udp_enable(serial_port_id);
			if(CS_E_OK == ret)
			{
				cs_printf("udp_enable start...\n");
			}
			else
			{
				cs_printf("udp_enable failed\n");
			}
		}
		else
		{
			ret = CS_E_ERROR;
		}		
	}
	else
	{
		ret = CS_E_ERROR;
	}

	return ret;
}


extern cs_status serial_port_connect_mode_tcp_server_set(cs_uint16 serial_port_id)
{
	#if 0
	cs_printf("in serial_port_connect_mode_tcp_server_set\n");
	#endif
	
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	pConfig->uart_cfg.uart_id = serial_port_id;
	pConfig->proto_type = IPPROTO_TCP;
	pConfig->server_client_mode = 0;
	return ret;

}
extern cs_status serial_port_connect_mode_tcp_client_set(cs_uint16 serial_port_id)
{
#if 0
	cs_printf("in serial_port_connect_mode_tcp_client_set\n");
#endif
	
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	pConfig->uart_cfg.uart_id = serial_port_id;
	pConfig->proto_type = IPPROTO_TCP;
	pConfig->server_client_mode = 1;
	return ret;

}

extern cs_status serial_port_connect_mode_udp_set(cs_uint16 serial_port_id)
{
#if 0
	cs_printf("in serial_port_connect_mode_udp_set\n");
#endif
	
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	pConfig->uart_cfg.uart_id = serial_port_id;
	pConfig->proto_type = IPPROTO_UDP;
	return ret;

}


extern cs_status gw_serial_port_data_size_set(cs_uint16 serial_port_id, cs_uint8 data_size)
{
	#if 0
	cs_printf("in gw_serial_port_data_size_set\n");
	#endif
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	pConfig->uart_cfg.uart_id = serial_port_id;
	pConfig->uart_cfg.data_bits = data_size;
	return ret;
}

extern cs_status gw_serial_port_local_port_set(cs_uint16 serial_port_id, cs_uint32 local_port)
{
	#if 0
	cs_printf("in gw_serial_port_local_port_set, serial_port_id :%d, local_port :%d\n", serial_port_id, local_port);
	#endif
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	pConfig->uart_cfg.uart_id = serial_port_id;
	pConfig->proto_port= local_port;
	#if 0
	cs_printf("pConfig->proto_port :%d\n", pConfig->proto_port);
	#endif
	return ret;
}

extern cs_status gw_serial_port_parity_set(cs_uint16 serial_port_id, char *parity)
{
	#if 0
	cs_printf("in gw_serial_port_parity_set\n");
	#endif
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	pConfig->uart_cfg.uart_id = serial_port_id;
	pConfig->uart_cfg.parity = UART_PARITY_NONE;
	if(0 == strcmp(parity, "none"))
	{
		pConfig->uart_cfg.parity = UART_PARITY_NONE;
		ret = CS_E_OK;
	}
	else if(0 == strcmp(parity, "odd"))
	{
		pConfig->uart_cfg.parity = UART_PARITY_ODD;
		ret = CS_E_OK;
	}
	else if(0 == strcmp(parity, "even"))
	{
		pConfig->uart_cfg.parity = UART_PARITY_EVEN;
		ret = CS_E_OK;
	}
	else
	{
		ret = CS_E_ERROR;
	}
	return ret;
}


extern cs_status gw_serial_port_purge_buf_set(cs_uint16 serial_port_id)
{
	#if 0
	cs_printf("in gw_serial_port_purge_buf_set\n");
	#endif
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	pConfig->uart_cfg.uart_id = serial_port_id;
	//purge buf
	return ret;
}

extern cs_status gw_serial_port_remote_ip_set(cs_uint16 serial_port_id, cs_uint32 remote_ip)
{
	#if 0
	cs_printf("in gw_serial_port_remote_ip_set\n");
	#endif
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	pConfig->uart_cfg.uart_id = serial_port_id;
	pConfig->remote_server_ip = remote_ip;
	return ret;	
}

extern cs_status gw_serial_port_remote_port_set(cs_uint16 serial_port_id, cs_uint32 remote_port)
{
	#if 0
	cs_printf("in gw_serial_port_remote_port_set\n");
	#endif
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	pConfig->uart_cfg.uart_id = serial_port_id;
	pConfig->remote_server_port = remote_port;
	return ret;		
}


extern cs_status gw_serial_port_stop_bit_set(cs_uint16 serial_port_id, cs_uint16 stop_bit)
{
	#if 0
	cs_printf("in gw_serial_port_stop_bit_set\n");
	#endif
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	pConfig->uart_cfg.uart_id = serial_port_id;
	pConfig->uart_cfg.stop_bits= stop_bit;
	return ret;	
}


extern cs_status serial_port_information_show(cs_uint16 serial_port_id)
{
#if 0
	cs_printf("in serial_port_information_show\n");
#endif
	
	cs_status ret = CS_E_OK;
	term_server_config_t * pConfig = NULL;
	pConfig = TS_CFG_GET(serial_port_id);
	char net_connect_status[10] = {0};
	if(1 == pConfig->socket_enable)
	{
		strncpy(net_connect_status, "enable", sizeof(net_connect_status));
	}
	else
	{
		strncpy(net_connect_status, "disable", sizeof(net_connect_status));
	}

	char connect_mode[15] = {"not set"};
	if(IPPROTO_TCP == pConfig->proto_type)
	{
		if(0 == pConfig->server_client_mode)
		{
			strncpy(connect_mode, "tcp_server", sizeof(connect_mode));
		}
		else if(1 == pConfig->server_client_mode)
		{
			strncpy(connect_mode, "tcp_client", sizeof(connect_mode));
		}
		else
		{
			//do nothing
			strncpy(connect_mode, "not set", sizeof(connect_mode));
		}
	}
	else if(IPPROTO_UDP == pConfig->proto_type)
	{
		strncpy(connect_mode, "udp", sizeof(connect_mode));
	}
	else
	{
		//do nothing
		strncpy(connect_mode, "not set", sizeof(connect_mode));
	}

	struct sockaddr_in addrp[1];
	addrp[0].sin_addr.s_addr = htonl(pConfig->remote_server_ip);

	char connect_status[15] = {0};
	if(pConfig->client_fd >= 0)
	{
		strncpy(connect_status, "connected", sizeof(connect_status));
	}
	else
	{
		strncpy(connect_status, "disconnected", sizeof(connect_status));
	}
				
	cs_printf("serial port number   :%d\n", serial_port_id);
	//cs_printf("serial port mode          :%d\n", );
	cs_printf("baud rate            :%d\n", pConfig->uart_cfg.baud_rate);
	cs_printf("data size            :%d\n", pConfig->uart_cfg.data_bits);
	cs_printf("parity               :%d\n", pConfig->uart_cfg.parity);
	cs_printf("stop bit             :%d\n", pConfig->uart_cfg.stop_bits);
	cs_printf("flow control         :%d\n", pConfig->uart_cfg.flow_control);
	cs_printf("net connect          :%s\n", net_connect_status);
	cs_printf("connect mode         :%s\n", connect_mode);
	cs_printf("local tcp/udp port   :%d\n", pConfig->proto_port);
	cs_printf("remote tcp/udp port  :%d\n", pConfig->remote_server_port);
	cs_printf("remote ip address    :%s\n", inet_ntoa(addrp[0].sin_addr));
	cs_printf("connect state        :%s\n", connect_status);
	cs_printf("bytes in serial port :%d\n", pConfig->uart_rx_char_cnt);
	cs_printf("bytes out of port    :%d\n", pConfig->uart_tx_char_cnt);

	
	
	
	return ret;
}


#endif
extern int serial_port_config_show(term_server_config_t* pConfig);
extern int serial_port_default_config_check(cs_uint8 uart, term_server_config_t* pConfig);


extern int serial_port_tlv_infor_get(int *len, char **data, int *free_need)
{	
#if 0
	cs_printf("in %s, sizeof(term_server_config_t):%d\n", __func__, sizeof(term_server_config_t));
#endif
	int ret = 0;
	term_server_config_t *term_server_config_p = NULL;
	serial_port_config_t *srial_port_config_save = NULL;
	serial_port_config_t *p = NULL;
	int i = 0;
	int uart = 0;
	int count = 0;
	serial_port_config_t *save_format = NULL;
	serial_port_config_t serial_save_format;
	cs_uint32 size = 0;
	if(NULL == len)
	{
		goto error;
	}
	else
	{
		*len = 0;
	}
	
	if(NULL == data)
	{
		goto error;
	}
	else
	{
		*data = NULL;
	}

	if(NULL == free_need)
	{
		goto error;
	}
	else
	{
		*free_need = 0;
	}
	
	size = sizeof(serial_port_config_t) * MAX_TERM_SERV_NUM;;
	srial_port_config_save = (serial_port_config_t *)iros_malloc(IROS_MID_APP, size);
	memset(srial_port_config_save, 0, size);
	*free_need = 1;
	save_format  = &serial_save_format;
	
	//获取串口配置
	p = srial_port_config_save;
	for(i=0;i<MAX_TERM_SERV_NUM;i++)
	{
		uart = i + TS_MIN_UART_NUM;
		term_server_config_p = serial_port_running_config_get(uart);
		serial_port_config_runing_format_switch_to_save_format(term_server_config_p, save_format);
		if(serial_port_config_save_format_default_check(uart, save_format) != 0)
		{
			memcpy(p, save_format, sizeof(serial_port_config_t));
			p++;
			count++;
		}
		else
		{
			//do nothing
		}
	}
	
	*len = sizeof(term_server_config_t) * count;
	*data = (char *)srial_port_config_save;

	ret = 0;
	goto end;
	
error:
	ret = -1;
end:
	if((0 == *len)&&(NULL != srial_port_config_save))
	{
		iros_free(srial_port_config_save);
		srial_port_config_save = NULL;
	}
	return ret;
}


extern int serial_port_running_config_show(void);
extern int serial_port_default_config_set(cs_uint8 uart, term_server_config_t* pConfig);
extern term_server_config_t* serial_port_running_config_get(int uart);
extern int serial_port_config_show(term_server_config_t* pConfig);





extern term_server_config_t* serial_port_running_config_get(int uart)
{
	return TS_CFG_GET(uart);
}


extern int serial_port_config_show(term_server_config_t* pConfig)
{
	#if 0
	cs_printf("in %s\n", __func__);
	#endif
	
	cs_uint8 serial_port_id = pConfig->uart_cfg.uart_id;
	char net_connect_status[10] = {0};
	if(1 == pConfig->socket_enable)
	{
		strncpy(net_connect_status, "enable", sizeof(net_connect_status));
	}
	else
	{
		strncpy(net_connect_status, "disable", sizeof(net_connect_status));
	}

	char connect_mode[15] = {"not set"};
	if(IPPROTO_TCP == pConfig->proto_type)
	{
		if(0 == pConfig->server_client_mode)
		{
			strncpy(connect_mode, "tcp_server", sizeof(connect_mode));
		}
		else if(1 == pConfig->server_client_mode)
		{
			strncpy(connect_mode, "tcp_client", sizeof(connect_mode));
		}
		else
		{
			//do nothing
			strncpy(connect_mode, "not set", sizeof(connect_mode));
		}
	}
	else if(IPPROTO_UDP == pConfig->proto_type)
	{
		strncpy(connect_mode, "udp", sizeof(connect_mode));
	}
	else
	{
		//do nothing
		strncpy(connect_mode, "not set", sizeof(connect_mode));
	}

	struct sockaddr_in addrp[1];
	addrp[0].sin_addr.s_addr = htonl(pConfig->remote_server_ip);

	char connect_status[15] = {0};
	if(pConfig->client_fd >= 0)
	{
		strncpy(connect_status, "connected", sizeof(connect_status));
	}
	else
	{
		strncpy(connect_status, "disconnected", sizeof(connect_status));
	}

	cs_printf("\n----------------------------------------------------------\n");
	cs_printf("serial port number   :%d\n", serial_port_id);
	//cs_printf("serial port mode          :%d\n", );
	cs_printf("baud rate            :%d\n", pConfig->uart_cfg.baud_rate);
	cs_printf("data size            :%d\n", pConfig->uart_cfg.data_bits);
	cs_printf("parity               :%d\n", pConfig->uart_cfg.parity);
	cs_printf("stop bit             :%d\n", pConfig->uart_cfg.stop_bits);
	cs_printf("flow control         :%d\n", pConfig->uart_cfg.flow_control);
	cs_printf("net connect          :%s\n", net_connect_status);
	cs_printf("connect mode         :%s\n", connect_mode);
	cs_printf("local tcp/udp port   :%d\n", pConfig->proto_port);
	cs_printf("remote tcp/udp port  :%d\n", pConfig->remote_server_port);
	cs_printf("remote ip address    :%s\n", inet_ntoa(addrp[0].sin_addr));
	cs_printf("connect state        :%s\n", connect_status);
	cs_printf("----------------------------------------------------------\n");

	return 0;
}

extern int serial_port_running_config_show(void)
{
	term_server_config_t* term_server_config_p = NULL;
	serial_port_config_t* save_format_p = NULL;
	serial_port_config_t save_format;
	int i = 0;
	int uart = 0;
	save_format_p = &save_format;
	for(i=0;i<MAX_TERM_SERV_NUM;i++)
	{
		uart = i + TS_MIN_UART_NUM;
		term_server_config_p = serial_port_running_config_get(uart);
		serial_port_config_runing_format_switch_to_save_format(term_server_config_p, save_format_p);
		if(0 != serial_port_config_save_format_default_check(uart, save_format_p))
		{
			serial_port_config_show(term_server_config_p);
		}
	}
	return 0;
}

extern int serial_port_default_config_set(cs_uint8 uart, term_server_config_t* pConfig)
{
	//设置串口默认配置
	memset(pConfig,0,sizeof(term_server_config_t));

	#if 1
	pConfig->uart_cfg.uart_id = uart;
	pConfig->uart_cfg.baud_rate = 115200;
	pConfig->uart_cfg.data_bits = 8;
	pConfig->uart_cfg.stop_bits = 1;
	pConfig->uart_cfg.flow_control = 0;
	pConfig->uart_cfg.parity = 0;
	pConfig->uart_cfg.mode = UART_MODE_INTR;
	pConfig->uart_cfg.duplex = UART_DUPLEX_HALF;
	pConfig->proto_type = IPPROTO_TCP;
	pConfig->proto_port = 6000 + uart;
	pConfig->server_client_mode = 0;
	pConfig->client_enable = 0;
	pConfig->server_enable = 1;
	pConfig->socket_enable = 0;
	pConfig->socket_control = 1;
	#endif
	pConfig->init_flag = VALID_CONFIG_FLAG;
	pConfig->client_fd = -1;
	pConfig->server_fd = -1;
	pConfig->client_closed_flag = 1;
	pConfig->client_timeout = 1000;
	pConfig->min_payload = 40;
	pConfig->max_payload = 1024;
	pConfig->max_res_time = 30;
	
	return 0;
}

extern int serial_port_config_runing_format_switch_to_save_format(const term_server_config_t* running_format, serial_port_config_t *save_format)
{
	int ret = 0;
	if(NULL == running_format)
	{
		goto error;
	}

	if(NULL == save_format)
	{
		goto error;
	}

	save_format->init_flag = VALID_CONFIG_FLAG;
	memcpy(&save_format->uart_cfg, &running_format->uart_cfg, sizeof(uart_config_t));
	save_format->proto_type = running_format->proto_type;
	save_format->proto_port = running_format->proto_port;
	save_format->client_timeout = running_format->client_timeout;
	save_format->max_payload = running_format->max_payload;
	save_format->min_payload = running_format->min_payload;
	save_format->max_res_time = running_format->max_res_time;
	save_format->server_client_mode = running_format->server_client_mode;
	save_format->client_enable = running_format->client_enable;
	save_format->server_enable = running_format->server_enable;
	save_format->socket_control = running_format->socket_control;
	save_format->remote_server_ip = running_format->remote_server_ip;
	save_format->remote_server_port = running_format->remote_server_port;
	ret = 0;
	goto end;
	
error:
	ret = -1;
	
end:
	return ret;
}

extern int serial_port_config_save_format_switch_to_runing_format(term_server_config_t* running_format, serial_port_config_t *save_format)
{
	int ret = 0;
	if(NULL == running_format)
	{
		goto error;
	}

	if(NULL == save_format)
	{
		goto error;
	}

	memset(running_format, 0, sizeof(term_server_config_t));
	
	running_format->init_flag = VALID_CONFIG_FLAG;
	memcpy(&running_format->uart_cfg, &save_format->uart_cfg, sizeof(uart_config_t));
	running_format->proto_type = save_format->proto_type;
	running_format->proto_port = save_format->proto_port;
	running_format->client_timeout = save_format->client_timeout;
	running_format->max_payload = save_format->max_payload;
	running_format->min_payload = save_format->min_payload;
	running_format->max_res_time = save_format->max_res_time;
	running_format->server_client_mode = save_format->server_client_mode;
	running_format->client_enable = save_format->client_enable;
	running_format->server_enable = save_format->server_enable;
	running_format->socket_control = save_format->socket_control;
	running_format->remote_server_ip = save_format->remote_server_ip;
	running_format->remote_server_port = save_format->remote_server_port;
	ret = 0;
	goto end;
	
error:
	ret = -1;
	
end:
	return ret;
}

extern int serial_port_config_save_format_set_default(cs_uint8 uart, serial_port_config_t *save_format)
{
	int ret = 0;
	
	if(NULL == save_format)
	{
		goto error;
	}

	memset(save_format, 0, sizeof(serial_port_config_t));
	save_format->init_flag = VALID_CONFIG_FLAG;
	
	save_format->uart_cfg.uart_id = uart;
	save_format->uart_cfg.baud_rate = 115200;
	save_format->uart_cfg.data_bits = 8;
	save_format->uart_cfg.stop_bits = 1;
	save_format->uart_cfg.flow_control = 0;
	save_format->uart_cfg.parity = 0;
	save_format->uart_cfg.mode = UART_MODE_INTR;
	save_format->uart_cfg.duplex = UART_DUPLEX_HALF;
	
	
	save_format->proto_type = IPPROTO_TCP;
	save_format->proto_port = 6000 + uart;
	save_format->server_client_mode = 0;
	save_format->client_enable = 0;
	save_format->server_enable = 1;
	save_format->socket_control = 1;
	save_format->remote_server_ip = 0;
	save_format->remote_server_port = 0;
	save_format->client_timeout = 1000;
	save_format->min_payload = 40;
	save_format->max_payload = 1024;
	save_format->max_res_time = 30;
	
	goto end;
	
error:
	ret = -1;
end:
	return ret;
}

//#define SERIAL_PORT_DEBUG	1

extern int serial_port_config_save_format_default_check(cs_uint8 uart, serial_port_config_t *save_format)
{
	int ret = 0;
	serial_port_config_t *save_format_default = NULL;
	serial_port_config_t config_default;
	if(CS_E_OK != ts_uart_id_check(uart))
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(NULL == save_format)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}

	save_format_default = &config_default;
	serial_port_config_save_format_set_default(uart, save_format_default);
	
	if(save_format->init_flag != save_format_default->init_flag)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	
	if(save_format->uart_cfg.uart_id != save_format_default->uart_cfg.uart_id)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->uart_cfg.data_bits != save_format_default->uart_cfg.data_bits)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->uart_cfg.stop_bits != save_format_default->uart_cfg.stop_bits)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->uart_cfg.flow_control != save_format_default->uart_cfg.flow_control)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->uart_cfg.parity != save_format_default->uart_cfg.parity)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->uart_cfg.duplex != save_format_default->uart_cfg.duplex)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->uart_cfg.mode != save_format_default->uart_cfg.mode)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->uart_cfg.baud_rate != save_format_default->uart_cfg.baud_rate)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}

	if(save_format->proto_type != save_format_default->proto_type)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		
		#endif
		goto error;
	}
	if(save_format->proto_port != save_format_default->proto_port)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->client_timeout != save_format_default->client_timeout)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->max_payload != save_format_default->max_payload)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->min_payload != save_format_default->min_payload)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->max_res_time != save_format_default->max_res_time)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->server_client_mode != save_format_default->server_client_mode)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->client_enable != save_format_default->client_enable)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->server_enable != save_format_default->server_enable)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->socket_control != save_format_default->socket_control)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->remote_server_ip != save_format_default->remote_server_ip)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}
	if(save_format->remote_server_port != save_format_default->remote_server_port)
	{
		#ifdef SERIAL_PORT_DEBUG
		cs_printf("in %s, line: %d\n", __func__, __LINE__);
		#endif
		goto error;
	}

	ret = 0;
	goto end;
	
error:
	ret = -1;
end:
	return ret;
}
extern int serial_port_default_config_check(cs_uint8 uart, term_server_config_t* pConfig)
{
	int ret = 0;
	if(pConfig != NULL)
	{
		//do nothing
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}
	
	if(uart == pConfig->uart_cfg.uart_id)
	{
		//do nothing
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}
	
	if(115200 == pConfig->uart_cfg.baud_rate)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}
	
	if(8 == pConfig->uart_cfg.data_bits)
	{
		
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}
	
	if(1 == pConfig->uart_cfg.stop_bits)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	if(0 == pConfig->uart_cfg.flow_control)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	if(0 == pConfig->uart_cfg.parity)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	if(UART_MODE_INTR == pConfig->uart_cfg.mode)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	if(UART_DUPLEX_HALF == pConfig->uart_cfg.duplex)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	if(IPPROTO_TCP == pConfig->proto_type)
	{
		
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}
	
	if(6000 + uart == pConfig->proto_port)
	{
		
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	if(0 == pConfig->server_client_mode)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}
	
	if(0 == pConfig->client_enable)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}
	
	if(1 == pConfig->server_enable)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	#if 0
	if(0 == pConfig->socket_enable)
	{
	
	}
	else
	{
		#if 1
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}
	#endif

	if(1 == pConfig->socket_control)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	if(VALID_CONFIG_FLAG == pConfig->init_flag)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	#if 0
	if(-1 == pConfig->client_fd)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	if(-1 == pConfig->server_fd)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	if(1 == pConfig->client_closed_flag)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}
	#endif

	if(1000 == pConfig->client_timeout )
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	
	if(40 == pConfig->min_payload)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	if(1024 == pConfig->max_payload)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}

	if(30 == pConfig->max_res_time)
	{
	
	}
	else
	{
		#if 0
		cs_printf("line :0x%x\n", __LINE__);
		#endif
		ret = -1;
		goto end;
	}
	ret = 0;
end:
	#if 0
	cs_printf("ret :0x%x\n", ret);
	#endif
	return ret;
}

extern int serial_port_config_recover(term_server_config_t *uart_config)
{
	#if 0
	cs_printf("in %s\n", __func__);
	#endif
	int ret = 0;
	term_server_config_t *config_running = NULL;
	int uart = 0;
	if(NULL == uart_config)
	{
		ret = -1;
		goto end;
	}
	
	
	uart = uart_config->uart_cfg.uart_id;
	serial_port_connect_disable(uart);
	config_running = serial_port_running_config_get(uart);

	memcpy(config_running, uart_config, sizeof(term_server_config_t));
	ret = 0;
end:
	return ret;
}

extern int serial_port_module_default_config_recover()
{
	#if 1
	cs_printf("in %s\n", __func__);
	#endif
	int ret = 0;
	cs_uint8 uart = 0;
	term_server_config_t serial_port_default_config;
	serial_port_config_t save_format;
	int i = 0;
	for(i=0;i<MAX_TERM_SERV_NUM;i++)
	{
		uart = i + TS_MIN_UART_NUM;
		#if 0
		serial_port_default_config_set(uart, &serial_port_default_config);
		serial_port_config_recover(&serial_port_default_config);
		#endif
		serial_port_config_save_format_set_default(uart, &save_format);
		serial_port_config_save_format_switch_to_runing_format(&serial_port_default_config, &save_format);
		serial_port_config_recover(&serial_port_default_config);
	}
	return ret;
}

extern int serial_port_default_config_check(cs_uint8 uart, term_server_config_t* pConfig);


extern int serial_port_tlv_infor_handle(int len, char *data, int opcode)
{
	#if 0
	cs_printf("in %s, len:%d\n", __func__, len);
	#endif
	int ret = 0;
	int uart = 0;
	int i = 0;
	int j = 0;
	int uart_config_num = 0;

	if(0 != len)
	{
		//do nothing
	}
	else
	{
		goto error;
	}
	
	if(NULL != data)
	{
		//do nothing
	}
	else
	{
		goto error;
	}

	uart_config_num = len / sizeof(term_server_config_t);
	#if 0
	cs_printf("len :%d, uart_config_num :%d\n", len, uart_config_num);
	#endif
	serial_port_config_t *uart_config = NULL;
	uart_config = (serial_port_config_t *)data;
	term_server_config_t running_config;
	if(DATA_RECOVER == opcode)
	{
		for(i=0;i<MAX_TERM_SERV_NUM;i++)
		{
			uart = i + TS_MIN_UART_NUM;
			
			for(j=0;j<uart_config_num;j++)
			{
				#if 0
				cs_printf("uart_config[j].uart_cfg.uart_id :0x%x\n", uart_config[j].uart_cfg.uart_id);
				#endif
				if(uart == uart_config[j].uart_cfg.uart_id)
				{
					break;
				}
			}
			if(uart == uart_config[j].uart_cfg.uart_id)
			{
				serial_port_config_save_format_switch_to_runing_format(&running_config, &uart_config[j]);
				serial_port_config_recover(&running_config);
			}
			else
			{
			}
		}	
	}
	else if(DATA_SHOW == opcode)
	{
		#if 0
		for(i=0;i<MAX_TERM_SERV_NUM;i++)
		{
			uart = i + TS_MIN_UART_NUM;
			
			for(j=0;j<uart_config_num;j++)
			{
				if(uart == uart_config[j].uart_cfg.uart_id)
				{
					break;
				}
			}
			if(uart == uart_config[j].uart_cfg.uart_id)
			{
				if(serial_port_default_config_check(uart, &uart_config[j]) != 0)
				{
					serial_port_config_show(&uart_config[j]);
				}
				else
				{
					//do nothing
				}
				
			}
			else
			{
			}
		}	
		#endif
		
		#if 1
		for(i=0;i<uart_config_num;i++)
		{
			uart = uart_config[i].uart_cfg.uart_id;		
			#if 0
			cs_printf("uart :0x%x\n", uart);
			#endif
			if((uart >= TS_MIN_UART_NUM)&&(uart <= MAX_TERM_SERV_NUM))
			{
				serial_port_config_save_format_switch_to_runing_format(&running_config, &uart_config[j]);
				serial_port_config_show(&running_config);
			}
			else
			{
				//do nothing
			}
		}
		#endif
	}
	else
	{
		cs_printf("in %s\n", __func__);
		cs_printf("wrong uart opcode\n");
	}
	ret = 0;
	goto end;
	
error:
	ret = -1;

end:
	return ret;	
}

int serial_port_num_list_get(int *list)
{
	int ret = 0;
	int i = 0;
	for(i=0;i<MAX_TERM_SERV_NUM;i++)
	{
		list[i] = i + TS_MIN_UART_NUM;
	}
	return ret;
}

int serial_port_num_string_get(char *string)
{
	int ret = 0;
	int i =0;
	int list[MAX_TERM_SERV_NUM] = {0};
	serial_port_num_list_get(list);
	for(i=0;i<MAX_TERM_SERV_NUM;i++)
	{
		string[2*i] = list[i] + '0';
		string[2*i+1] = ',';
	}
	string[2*MAX_TERM_SERV_NUM-1] = 0;
	return ret;
}

#endif

