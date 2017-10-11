/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#include "cs_types.h"
#include "frame.h"
#include "packet.h"
#include "onu_hello.h"
#include "sdl_api.h"  
#include "osal_api_packet.h"
#include "cs_cmn.h"
#include "cs_event.h"
#include "event.h"
#include "startup_cfg.h"

#ifndef  HAVE_IROS_SPECIAL_MAC
unsigned char epon_macaddr_all_sc[CS_MACADDR_LEN] =
                        {0x01, 0x13, 0x25, 0x00, 0x00, 0x02};

unsigned char epon_macaddr_all_onu[CS_MACADDR_LEN] =
                        {0x01, 0x13, 0x25, 0x00, 0x00, 0x03};
#else
unsigned char epon_macaddr_all_onu[CS_MACADDR_LEN] =
                        {0x01,0x80,0xc2,0x00,0x00,0x02};

unsigned char epon_macaddr_all_sc[CS_MACADDR_LEN] =
                        {0x01,0x80,0xc2,0x00,0x00,0x02};

#endif

cs_port_id_t active_mgmt_port = INVALID_PORT_ID;

extern void l2ftp_frame_proc(epon_ether_frame_t *, unsigned short, cs_port_id_t);
extern void onu_hello_proc(cs_uint8 *frame, int len,cs_port_id_t port);

void onu_msg_proc(void *msg)
{
#ifdef HAVE_ONU_EVENT
    cs_uint32 msg_type = ntohl(*((cs_uint32 *)msg));

    IROS_LOG_DBG1(IROS_MID_OOBCMD, "=>SC ONU msg 0x%x\n", msg_type);

    switch (msg_type) {
    case CS_ESS_SDK_EVENT_ACK:
        break;
    default:
        IROS_LOG_CRI(IROS_MID_APP,
                     "unknown message type 0x%x\n", msg_type);
        break;
    }
#endif
}

void onu_frame_proc(epon_ether_frame_t *frame, int len, cs_port_id_t port)
{
	unsigned short ether_type;
	unsigned char *tmp = (unsigned char *)frame;
	unsigned char macaddr[EPON_MACADDR_LEN];
    cs_callback_context_t     context;

	if(!frame) return;

	ether_type = *(unsigned short *)(tmp + EPON_MACADDR_LEN + EPON_MACADDR_LEN);
	ether_type = ntohs(ether_type);

       if (ether_type == EPON_ETHERTYPE_DOT1Q) {

            memcpy(tmp + EPON_MACADDR_LEN +EPON_MACADDR_LEN,
                   tmp + EPON_MACADDR_LEN + EPON_MACADDR_LEN + 4,
                   len - EPON_MACADDR_LEN -EPON_MACADDR_LEN - 4);
   	}

	/*startup_config_read(CFG_ID_MAC_ADDRESS, EPON_MACADDR_LEN, macaddr);*/
    epon_request_onu_pon_mac_addr_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, (cs_mac_t *)macaddr);
        /* decide whether to forward or not */
        if (!memcmp(frame->layer3_header.dst, macaddr,
                    EPON_MACADDR_LEN) || !memcmp(frame->layer3_header.dst,
                    epon_macaddr_all_onu, EPON_MACADDR_LEN)) {

#ifdef HAVE_OOB_MGMT
		   /* for OOB, remember port for the incomming msg*/
                 if(active_mgmt_port != port)
                 	active_mgmt_port = port;
#endif

                /* it is local, process it */
                switch (ntohs(frame->layer3_header.layer3type)) {
#ifdef HAVE_ONU_L2FTP
                case EPON_LAYER3_TYPE_FTP:
                        l2ftp_frame_proc(frame, len, port);
                        break;
#endif

                default:
                        IROS_LOG_INF(IROS_MID_APP,
                                        "unkown layer3 type 0x%x\n",
                                        ntohs(frame->layer3_header.layer3type));
                        break;
                }
        }
        else {
                /* should drop it */
                IROS_LOG_INF(IROS_MID_APP,
                        "onu_frame_proc: frame dropped\n");
        }
}

cs_status iros_pkt_parser(cs_pkt_t *pPkt)
{
    if(pPkt->eth_type == EPON_ETHERTYPE_IROS) {
        pPkt->pkt_type = CS_PKT_IROS;
        return CS_E_OK;
    }
    else {
        return CS_E_NOT_SUPPORT;
    }
}


cs_uint8 iros_pkt_proc(cs_pkt_t *pPkt)
{
    cs_uint8 *frame_ptr;
    cs_uint32 len;
    cs_port_id_t port;

    frame_ptr = pPkt->data + pPkt->offset;
    len = pPkt->len;
    port = pPkt->port;
    
    onu_frame_proc((epon_ether_frame_t *)frame_ptr, len, port);
    return 1;
}

void iros_pkt_init()
{
    app_pkt_reg_handler(CS_PKT_IROS, iros_pkt_proc);
    app_pkt_reg_parser(CS_PKT_IROS, iros_pkt_parser);
}

cs_status onu_frame_send(cs_uint8 *frame,
                          cs_uint32 frame_len,
                          cs_port_id_t port_id,
                          cs_uint8  priority,
                          cs_uint8  mode)
{
    cs_status rc = 0;
    cs_callback_context_t     context;

    epon_tagged_ether_frame_t outputFrame;
    epon_ether_frame_t *frame_buf = (epon_ether_frame_t *)frame;
    cs_uint16 vlanId = 0;

    IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ, IROS_MID_APP, "frame=%p, len=%d, port=%08x, pri=%d, mode=%d\n", 
        frame, frame_len, port_id, priority,mode);
    
    startup_config_read(
		CFG_ID_ONU_CTRL_VLAN, sizeof(vlanId), (cs_uint8*)&vlanId);

    if( vlanId && port_id != CS_PON_PORT_ID)
    {
        IROS_LOG_DBG1(IROS_MID_AAL, "=>sending tagged control packet %d\n", vlanId);
        DUMPPKT("snt tagged pkt", frame, frame_len);
        /* received tagged frame, send tagged frame in response */
        memset(&outputFrame, 0, sizeof(epon_tagged_ether_frame_t));
        memcpy(&outputFrame.l2dst, frame_buf->ether_header.dst, sizeof(epon_macaddr_t));
        memcpy(&outputFrame.l2src, frame_buf->ether_header.src, sizeof(epon_macaddr_t));

        /* add type (8100) and tag */
        outputFrame.tpid = htons(EPON_ETHERTYPE_DOT1Q);
        outputFrame.tag_id = htons(vlanId|(0x7<<13));


        outputFrame.ethertype = frame_buf->ether_header.ethertype;
        memcpy(&outputFrame.l3dst, frame_buf->layer3_header.dst, sizeof(epon_macaddr_t));
        memcpy(&outputFrame.l3src, frame_buf->layer3_header.src, sizeof(epon_macaddr_t));
        outputFrame.layer3type = frame_buf->layer3_header.layer3type;
        memcpy(&outputFrame.payload[0], frame_buf->payload,
                frame_len - sizeof(frame_buf->ether_header) - sizeof(frame_buf->layer3_header));

        frame_len += 2 * sizeof(cs_uint16);
        if(frame_len < 60) {
            frame_len = 60;
        }
        epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
                  (cs_uint8 *)&outputFrame,
                  frame_len,
                  port_id,
                  priority);
    }
    else
    {
        DUMPPKT("snt pkt", frame, frame_len);
        /* received untagged frame, just regular send */
        if(frame_len < 60) {
            frame_len = 60;
        }
        epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
                  (cs_uint8 *)frame_buf,
                  frame_len,
                  port_id,
                  priority);
    }
    return rc;
}

void setControlPktL2Dest(cs_uint8 *buffer, cs_uint32 len)
{
    cs_ether_frame_t *frame = (cs_ether_frame_t *)buffer;

    if(!frame || !len || len < EPON_FRAME_HEADER_LEN)
            return;

    IROS_LOG_DBG0(IROS_MID_OOBCMD, "=>SC RESP port 0x%x\n", active_mgmt_port);
    if(active_mgmt_port == CS_PON_PORT_ID)
    {
        memcpy(frame->ether_header.dst.addr, onu_node.olt.macaddr.addr, CS_MACADDR_LEN);
        memcpy(frame->layer3_header.dst.addr, epon_macaddr_all_sc, CS_MACADDR_LEN);
    }
    else
        memcpy(frame->ether_header.dst.addr, onu_node.sc.macaddr.addr, CS_MACADDR_LEN);

    onu_frame_send((cs_uint8 *)frame, len, active_mgmt_port, ONU_CPU_PKT_PRIO_6, 0);

    return;
}


