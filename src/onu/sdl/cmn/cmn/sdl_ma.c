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
/*
 * $Id: sdl_ma.c,v 1.1.2.2.2.25.6.1 2013/06/27 04:50:27 jsheng Exp $
 */

#include "sdl_ma.h"
#include "sdl_int.h"
#include "sdl_init.h"
#include "aal_ma.h"
#include "cs_types.h"
#include "cs_module.h"
#include "sdl.h"
#include "osal_api_packet.h"

extern void ethdrv_pkt_recv(cs_uint8* event_buf);
extern sdl_init_cfg_t sdl_int_cfg;

#define __MA_RX_PKT_LEN_MIN 60
#define __MA_RX_PKT_LEN_MAX 1522

typedef enum
{
    __MA_SPID_MII0  = 0,
    __MA_SPID_MII1  = 1,
    __MA_SPID_BM    = 2,
}__ma_spid_t;

typedef enum {
    __MA_HEADER_SPID__GE               = 0,  /* GE port                   */
    __MA_HEADER_SPID__PON              = 1,  /* PON port                  */
    __MA_HEADER_SPID__RSVD0            = 2,  /* Reserved port             */
    __MA_HEADER_SPID__RSVD1            = 3,  /* Reserved port             */
    __MA_HEADER_SPID__CPU              = 4,  /* CPU port                  */
    __MA_HEADER_SPID__MII0             = 5,  /* MII0 port                 */
    __MA_HEADER_SPID__DMA              = 6,  /* VOIP port                 */
    __MA_HEADER_SPID__RSVD2            = 7,  /* Reserved port             */
}__ma_header_spid_t;

typedef struct 
{
#ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd                : 7;
    cs_uint32 spid                 : 3;  /* bits 24:22 */
    cs_uint32 pkt_type             : 5;  /* bits 21:17 */
    cs_uint32 dpid                 : 3;  /* bits 16:14 */
    cs_uint32 cos                  : 3;  /* bits 13:11 */
    cs_uint32 len                  : 11; /* bits 10:0 */
#else /* CS_LITTLE_ENDIAN */
    cs_uint32 len                  : 11; /* bits 10:0 */
    cs_uint32 cos                  : 3;  /* bits 13:11 */
    cs_uint32 dpid                 : 3;  /* bits 16:14 */
    cs_uint32 pkt_type             : 5;  /* bits 21:17 */
    cs_uint32 spid                 : 3;  /* bits 24:22 */
    cs_uint32 rsrvd                : 7;
#endif    
}__ma_hdr_d_t;

typedef struct {
#ifdef CS_BIG_ENDIAN 
    cs_uint32 rsrvd1    :12,
              spid      :4,      /* Source port ID */
              pkt_type  :4,      /* MA parsing pkt type:
                                    [0] : Normal Packet enable
                                    [1] : BPDU Packet enable
                                    [2] : 802.1X packet enable
                                    [3] : Reserved
                                    [4] : IGMP packet enable
                                    [5] : ARP Packet enable
                                    [6] : OAM Packet enable
                                    [7] : MPCP Packet enable
                                    [8] : My Mac
                                    [9] : IPV4 packet enable
                                    [10] : IPV6 enable
                                    [11] : Hello Packet enable
                                    [12] : Reserved
                                    [13] : Reserved
                                    [14] : Reserved
                                    [15] : Reserved
                                */
              dpid      :1,     /* Destination port ID */
              len       :11;    /* packet length, include FCS */
#else /* CS_LITTLE_ENDIAN */      
    cs_uint32 len       :11,
              dpid      :1,
              pkt_type  :4,
              spid      :4,
              rsrvd1    :12;
#endif    
}__ma_hdr_c_t;


/* the packet rx callback for app */
cs_sdl_pkt_rx_f __pkt_rx_func;

/* the callback for external switch */
cs_sdl_sw_pkt_rx_f __sw_pkt_rx_func = NULL;
cs_sdl_sw_pkt_tx_f __sw_pkt_tx_func = NULL;

cs_boolean ma_debug = 0;
__cpu_rx_stats_t __cpu_rx_stats;


void __cpu_packet_dump(cs_uint8 *buf, cs_uint16 len)
{
    int i;
    cs_printf("---------------------------------------------------------");
    for(i = 0; i<len; ++i)
    {
        if(0 == (i&0xf)) cs_printf("\n%04x: ", i);
        if(0 == (i&0x7)) cs_printf(" ");
        cs_printf("%02x ", buf[i]);
    }
    cs_printf(".\n---------------------------------------------------------\n");
}

cs_status __ma_rx_parse(cs_aal_ma_rx_buf_t *buf, cs_uint16 len, __ma_spid_t spid)
{
    __ma_hdr_d_t  *hdr_d = NULL; /*< from PON, GE to CPU */
    __ma_hdr_c_t  *hdr_c = NULL; /*< from MII to CPU */
    cs_uint8 frame_type, src_port, s_port;
    cs_uint32 head = buf->head;
    cs_port_id_t port_id;
    cs_uint32 *pkt_counter = (cs_uint32 *)&__cpu_rx_stats;
    cs_uint16 pkt_len = len - MA_RX_BUF_HEAD_LEN;
    cs_boolean is_switch_trap2cpu = FALSE;
    cs_status ret = CS_E_OK;

    /* spid to type */
    switch (spid) 
    {
    case __MA_SPID_MII0:
    case __MA_SPID_MII1:
        hdr_c = (__ma_hdr_c_t *)&head;
        /* Traslate the MA pkt type to global pkt type */
        frame_type = (hdr_c->pkt_type < 15) ? hdr_c->pkt_type : SDL_PKT_NORMAL;
        src_port   = hdr_c->spid;  /** Get the real SPID */
  
        break;
    case __MA_SPID_BM:
        hdr_d = (__ma_hdr_d_t *)&head;
        frame_type = hdr_d->pkt_type;
        src_port   = hdr_d->spid; /* Get the real SPID */  


        /* if packet is from external switch, embedded cpu-tag should removed */
        if (__sw_pkt_rx_func && src_port == __MA_HEADER_SPID__GE )
        {
            /* external switch callback is registed and packet comes from giga port */
            cs_uint8 *pkt = &(buf->pkt->data[0]);
            if(0==__sw_pkt_rx_func(pkt + CS_PKT_OFFSET, pkt_len, &pkt_len, &s_port))
            {
                frame_type = SDL_PKT_SWT;
                is_switch_trap2cpu = TRUE;
            }
        }else {
            /*receive mirror frame from __MA_HEADER_SPID__DMA */
        }
        
        break;
    default:
        SDL_MAJ_LOG("SPID %u error.\n");
       
        return CS_E_ERROR;
    }

    switch(src_port)
    {
        case __MA_HEADER_SPID__GE: 
            /* if external switch callback is installed, real switch port no will be putted into s_port */
            if (is_switch_trap2cpu)
                port_id = s_port + 1;   /* physical to logical port */
            else
                port_id = CS_UNI_PORT_ID1;
            break;
        case __MA_HEADER_SPID__PON:
            port_id = CS_PON_PORT_ID;
            break;
        case __MA_HEADER_SPID__MII0: 
            port_id = CS_MGMT_PORT_ID;
            break;
        case __MA_HEADER_SPID__DMA: 
            port_id = CS_VOIP_PORT_ID;
            break;
        default:
            SDL_MAJ_LOG("src port %u error.\n", src_port);
            return CS_E_ERROR;
    }

    ++pkt_counter[frame_type];

    buf->pkt->offset = CS_PKT_OFFSET;
    buf->pkt->frame_type= frame_type;
    buf->pkt->port = port_id;
    buf->pkt->len  = pkt_len;

    // Call APP handler
    if(NULL!=__pkt_rx_func)
    {
        ret = __pkt_rx_func(buf->pkt);
        if(ret == CS_E_RESOURCE){
            cs_thread_delay(10);
        }
        return ret;
    }

    return CS_E_OK;
}

cs_status __ma_rx_process(cs_aal_ma_rx_buf_id_t buf_id, cs_aal_ma_rx_buf_status_t *buf_status)
{
    cs_aal_ma_rx_buf_t buf;
    cs_uint16 pkt_alloc_len;
    cs_status rt = CS_E_OK;
    

    if(NULL == buf_status)
    {
        SDL_MIN_LOG("buf_status %u is NULL.\n", buf_id);
        rt = CS_E_PARAM;
        goto end;
    }

    if (buf_status->len_err)
    {
        SDL_MAJ_LOG("MA RX buffer %u length error.\n", buf_id);
        rt = CS_E_ERROR;
        goto end;
    }

    if ((buf_status->pkt_len < (__MA_RX_PKT_LEN_MIN+4)) || (buf_status->pkt_len > (__MA_RX_PKT_LEN_MAX+4)) )
    {
        SDL_MIN_LOG("MA RX buffer %u length %u error.\n", buf_id, buf_status->pkt_len);
        rt = CS_E_ERROR;
        goto end;
    }

    pkt_alloc_len = CS_PKT_OFFSET+(sizeof(cs_pkt_t)-1)+(buf_status->pkt_len-4);

    if(NULL == (buf.pkt = (cs_pkt_t *)iros_malloc(IROS_MID_PACKET, pkt_alloc_len)))
    {
        SDL_MAJ_LOG("MA RX buffer %u allocte memory %u failed.\n", buf_id, pkt_alloc_len);
        rt = CS_E_RESOURCE;
        cs_thread_delay(10);
        goto end;
    }
    memset(buf.pkt,0,pkt_alloc_len);
    aal_ma_rx_buf_read(buf_id, &buf, buf_status->pkt_len);

    if(ma_debug)// DEBUG: dump packet
    {
        cs_printf("CPU PACKET RECEIVED\n");
        __cpu_packet_dump(buf.pkt->data+CS_PKT_OFFSET, buf_status->pkt_len-4);
    }

    if(CS_E_OK!=(rt=__ma_rx_parse(&buf, buf_status->pkt_len, (__ma_spid_t)buf_status->spid)))
    {
        SDL_MIN_LOG("__ma_rx_parse() return error %u.\n", rt);
        iros_free(buf.pkt);
        goto end;
    }
end:
    aal_ma_rx_buf_clear(buf_id);
    return rt;
}


cs_uint32 __ma_rx_parser_hook_reg(cs_sdl_sw_pkt_rx_f rx_hook)
{
    __sw_pkt_rx_func = rx_hook;

    return 0;
}

cs_uint32 __ma_tx_parser_hook_reg(cs_sdl_sw_pkt_tx_f tx_hook)
{
    __sw_pkt_tx_func = tx_hook;

    return 0;
}


void __int_cpu_rx_buff_rdy_handler(void)
{
    cs_aal_ma_rx_buf_status_t buf0_status,buf1_status;

    aal_ma_rx_buf_status_get(0, &buf0_status);
    aal_ma_rx_buf_status_get(1, &buf1_status);

    if(buf0_status.pkt_rdy && buf1_status.pkt_rdy)
    {
        if(buf1_status.turn)  // In ASIC logic, turn==1 indicates two buffers' ready
        {
            __ma_rx_process(1, &buf1_status);
            __ma_rx_process(0, &buf0_status);
        }
        else
        {
            __ma_rx_process(0, &buf0_status);
            __ma_rx_process(1, &buf1_status);
        }
    }
    else if(buf1_status.pkt_rdy)
    {
        __ma_rx_process(1, &buf1_status);
    }
    else if(buf0_status.pkt_rdy)
    {
        __ma_rx_process(0, &buf0_status);
    }
    else
    {
        //do nothing
    }
}

cs_status epon_request_onu_pkt_rcv_handler_reg (
    CS_IN   cs_callback_context_t   context,
    CS_IN   cs_int32              device,
    CS_IN   cs_int32              llidport,
    CS_IN   cs_sdl_pkt_rx_f           rx_func)
{
    __pkt_rx_func = rx_func; // NULL function is allowed
    return CS_E_OK;
}

cs_status epon_request_onu_frame_send (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_uint8                  *frame_buf,
    CS_IN cs_uint16                 frame_len,
    CS_IN cs_port_id_t              port_id,
    CS_IN cs_uint8                  priority)
{
    cs_aal_port_id_t port;
    cs_uint8 *pkt_buf = frame_buf;
    cs_uint16 pkt_len = frame_len;
    cs_status ret = CS_ERROR;
    cs_uint8 aal_priority;
    cs_uint8 pkt_buf_tmp[CS_ETHER_MTU + 8];

    switch(port_id)
    {
    case CS_PON_PORT_ID:
        port = AAL_PORT_ID_PON;
        break;
    case CS_UNI_PORT_ID1:
    case CS_UNI_PORT_ID2:
    case CS_UNI_PORT_ID3:
    case CS_UNI_PORT_ID4:
    case CS_ALL_UNI_PORT_ID:
        /* if there have external switch callback registered, just insert speical cpu header */
        if (__sw_pkt_tx_func)
        {
            pkt_len  = pkt_len + 8;
            /*
             * xyu: use local variable instead of iros_malloc, 
             * according to Jun, Shike & Zhijun
             */
            pkt_buf = pkt_buf_tmp;

            /* 0xff means frame shall be transmitted to all uni ports */
            if (port_id == CS_ALL_UNI_PORT_ID)
                __sw_pkt_tx_func(frame_buf, frame_len, pkt_buf, &pkt_len, port_id);
            else
                __sw_pkt_tx_func(frame_buf, frame_len, pkt_buf, &pkt_len, port_id - 1);
        }
        port = AAL_PORT_ID_GE;
        break;
    case CS_MGMT_PORT_ID:
        port = AAL_PORT_ID_MII0;
        break;
    default:
        SDL_MIN_LOG("port id %u invalid.\n");
        return CS_E_PARAM;
    }

    if(ma_debug)// DEBUG: dump packet
    {
        cs_printf("CPU PACKET SENT\n");
        __cpu_packet_dump(frame_buf, frame_len);
    }

    // priority is not used and use __CPU_COS instead
    //return aal_ma_tx(frame_buf, frame_len, port, ((sdl_int_cfg.max_queue_num==4) ? 4 : 7));
    if(sdl_int_cfg.max_queue_num==4){
        aal_priority = priority/2;
    }
    else{
        aal_priority = priority;
   }
    ret = aal_ma_tx(pkt_buf, pkt_len, port, aal_priority);
    
    return ret;
}

cs_status epon_request_onu_frame_send_to_pon_no_mutex (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                  device,
    CS_IN cs_int32                  llidport,
    CS_IN cs_uint8                  *frame_buf,
    CS_IN cs_uint16                 frame_len,
    CS_IN cs_uint8                  priority)
{
    cs_uint8 aal_priority;

    if(ma_debug)// DEBUG: dump packet
    {
        cs_printf("*CPU PACKET SENT\n");
        __cpu_packet_dump(frame_buf, frame_len);
    }

    if(sdl_int_cfg.max_queue_num==4){
	aal_priority = priority/2;				
    }
    else{
	aal_priority = priority;				
   }
    return  aal_ma_tx_no_mutex(frame_buf, frame_len, AAL_PORT_ID_PON, aal_priority);
}


cs_status sdl_ma_init(
    CS_IN const sdl_init_cfg_t  *cfg
)
{
    sdl_int_handler_reg(SDL_INT_CPU_RX_BUFF_RDY, __int_cpu_rx_buff_rdy_handler);
    return CS_E_OK;
}



