#ifdef HAVE_ONU_L2FTP
#include "plat_common.h"
#include "iros_config.h"
#include "cs_types.h"
#include "osal_common.h"
#include "log.h"
#include "cs_timer.h"
#include "osal_api_packet.h"
#include "cs_l2ftp.h"
#include "cs_hello.h"
#include "onu_hello.h"
#include "sdl_api.h"

l2ftp_record_t l2ftp_record;

#define ONU_MAX_L2FTP_RETRY     10
#define ONU_MAX_L2FTP_PAYLOAD   50
static struct {
    cs_ether_header_t   ether_header;
    cs_layer3_header_t  layer3_header;
    cs_uint8            payload[ONU_MAX_L2FTP_PAYLOAD];
} l2ftp_frame;

static cs_int32 image_len;
static cs_uint16 block_expected;
static cs_uint8 start;
static cs_uint32 retry;

static cs_uint8 macaddr_sc[CS_MACADDR_LEN]={0x01, 0x13, 0x25, 0x00, 0x00, 0x02};
cs_port_id_t active_update_port = 0;

cs_uint8 updateInProgress = EPON_FALSE;

static void l2ftp_fill_header()
{
    cs_callback_context_t context;
    memcpy(l2ftp_frame.ether_header.dst.addr, onu_node.olt.macaddr.addr,
                                                CS_MACADDR_LEN);
#ifdef HAVE_OOB_MGMT
    if(active_update_port != CS_PON_PORT_ID)
        memcpy(l2ftp_frame.ether_header.dst.addr, onu_node.sc.macaddr.addr,
                                                        CS_MACADDR_LEN);
#endif
    /*memcpy(l2ftp_frame.ether_header.src, onu_node.macaddr,
                                                    CS_MACADDR_LEN);*/
    epon_request_onu_pon_mac_addr_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, (cs_mac_t *)&l2ftp_frame.ether_header.src);
    l2ftp_frame.ether_header.ethertype = htons(CS_ETHERTYPE_IROS);
    memcpy(l2ftp_frame.layer3_header.dst.addr, macaddr_sc,
                                                    CS_MACADDR_LEN);
    memcpy(l2ftp_frame.layer3_header.src.addr, l2ftp_frame.ether_header.src.addr,
                                                    CS_MACADDR_LEN);
    l2ftp_frame.layer3_header.layer3type = htons(CS_LAYER3_TYPE_FTP);
}


static void
l2ftp_send_rrq()
{
    cs_int32 len;
    cs_l2ftp_pdu_rrq_t *rrq;

    l2ftp_fill_header();

    rrq = (cs_l2ftp_pdu_rrq_t *)l2ftp_frame.payload;
    rrq->header.opcode = htons(CS_L2FTP_PDU_OPCODE_RRQ_ONU);

    len = CS_FRAME_HEADER_LEN + sizeof(cs_l2ftp_pdu_rrq_t);

    #if 0
    if(active_update_port)
        onu_aal_frame_send_ex((cs_uint8 *)&l2ftp_frame, len, active_update_port, ONU_CPU_PKT_PRIO_6, 0);
    #endif
}

static void l2ftp_send_ack()
{
    cs_int32 len;
    cs_l2ftp_pdu_ack_t *ack;
    cs_uint16 block = block_expected - 1;

    l2ftp_fill_header();

    ack = (cs_l2ftp_pdu_ack_t *)l2ftp_frame.payload;
    ack->header.opcode = htons(CS_L2FTP_PDU_OPCODE_ACK_ONU);
    ack->block = htons(block);

    len = CS_FRAME_HEADER_LEN + sizeof(cs_l2ftp_pdu_ack_t);

    #if 0
    if(active_update_port)
        onu_aal_frame_send_ex((cs_uint8 *)&l2ftp_frame, len, active_update_port, ONU_CPU_PKT_PRIO_6, 0);
    #endif
}


static void
l2ftp_send_error(cs_uint32 reason)
{
    cs_int32 len;
    cs_l2ftp_pdu_error_t *error;

    l2ftp_fill_header();

    error = (cs_l2ftp_pdu_error_t *)l2ftp_frame.payload;
    error->header.opcode = htons(CS_L2FTP_PDU_OPCODE_ERROR_ONU);
    error->reason = htonl(reason);

    len = CS_FRAME_HEADER_LEN + sizeof(cs_l2ftp_pdu_error_t);

    #if 0
    if(active_update_port)
        onu_aal_frame_send_ex((cs_uint8 *)&l2ftp_frame, len, active_update_port, ONU_CPU_PKT_PRIO_6, 0);
    #endif
}

static void l2ftp_timeout()
{
    if (start) {
        IROS_LOG_INF(IROS_MID_L2FTP,
                    "l2ftp: timed-out, resend rrq\n");
        l2ftp_send_rrq();
    }
    else {
        IROS_LOG_INF(IROS_MID_L2FTP,
            "l2ftp: timed-out, resend ack\n");
        l2ftp_send_ack();
    }
}

static cs_uint8 dot_count = 0;
void l2ftp_frame_proc(cs_ether_frame_t *frame, cs_uint16 framesz, cs_port_id_t port)
{
    cs_l2ftp_header_t *header;
    cs_l2ftp_pdu_data_t *l2ftp_pdu_p;
    cs_uint16 l2ftp_pdu_len, block, opcode;
    cs_uint16 minsz;
    cs_uint8 *ptr;
    cs_callback_context_t context;
    cs_uint8 mac[CS_MACADDR_LEN];
    bool last_data_frame = false;

    minsz = CS_FRAME_HEADER_LEN + sizeof(cs_l2ftp_header_t);

    if (framesz < minsz) {
        IROS_LOG_INF(IROS_MID_L2FTP,
        "l2ftp: too short for headers\n");
        return;
    }

#ifdef HAVE_OOB_MGMT
    if(port != active_update_port)
    {
        diag_printf("l2ftp error: active_update_port 0x%x, port 0x%x\n",
        active_update_port, port);
        /*l2ftp_send_error_to_port(EPON_L2FTP_ERROR_WRONG_PORT, port);*/
        return;
    }
#endif

    ptr = (cs_uint8 *)frame;

    epon_request_onu_pon_mac_addr_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, (cs_mac_t *)mac);
    if (memcmp(frame->ether_header.dst.addr,mac,CS_MACADDR_LEN)||
        memcmp(frame->layer3_header.dst.addr,mac,CS_MACADDR_LEN)||
        (frame->ether_header.ethertype != htons(CS_ETHERTYPE_IROS)) ||
        (frame->layer3_header.layer3type != htons(CS_LAYER3_TYPE_FTP))) {
            IROS_LOG_INF(IROS_MID_L2FTP,"l2ftp: not a l2ftp frame\n");
            return;
    }

    header = (cs_l2ftp_header_t *)frame->payload;
    opcode = ntohs(header->opcode);

    switch (opcode) {
        case CS_L2FTP_PDU_OPCODE_DATA:
            IROS_LOG_DBG0(IROS_MID_L2FTP,
                            "l2ftp: rx an data pdu\n");
            diag_printf(".");
            dot_count++;
            if (dot_count >= 40) {
                dot_count = 0;
                diag_printf("\n");
            }
            minsz += sizeof(cs_uint16) + sizeof(cs_uint16);
            if (framesz < minsz) {
                IROS_LOG_INF(IROS_MID_L2FTP,
                "l2ftp: too short for data headers\n");
                return;
            }
            l2ftp_pdu_p = (cs_l2ftp_pdu_data_t *)frame->payload;
            block = ntohs(l2ftp_pdu_p->block);
            l2ftp_pdu_len = ntohs(l2ftp_pdu_p->length);
            minsz += l2ftp_pdu_len;
            if (framesz < minsz) {
                IROS_LOG_INF(IROS_MID_L2FTP,
                "l2ftp: too short for data payload\n");
                return;
            }
            if (block == block_expected) {
                block_expected++;
                if (start) {
                    start = 0;
                }
                retry = 0;

                
                if (l2ftp_pdu_len < CS_L2FTP_BLOCK_SIZE)
                    last_data_frame = true;

#if 0
                cs_int32 return_status = 0;
                mif_upgrade_state_t mif_engine_status;
                memset(&mif_engine_status, 0, sizeof(mif_engine_status));

                extern cs_int32 mif_upgrade (cs_uint8 module, cs_uint8 * input_buff, cs_int32 length, mif_upgrade_state_t * cur_state, bool end_of_file);
                return_status = mif_upgrade (1, l2ftp_pdu_p->data, l2ftp_pdu_len, &mif_engine_status, last_data_frame);

                if (return_status ==0) {  //write successfully
                    if (last_data_frame == true) {   //for the last frame
                        cs_timer_del(l2ftp_record.timer);
                        l2ftp_send_ack();
                        l2ftp_send_ack();
                        //IROS_LOG_CRI(IROS_MID_L2FTP,
                        //        "\nl2ftp done: len %d\n", image_len);
                        //app_update_file_transfer_done(image_len);
                        updateInProgress = EPON_FALSE;
                        active_update_port = 0;

                        //if blob partition is updated, reset the CPU
                        if (mif_engine_status.tlv_update_successed & 0x1) {
                            diag_printf("reboot\n");
                            HAL_PLATFORM_RESET();
                        }
                    }

                }
                else {
                    diag_printf("error encounted durng update, error code: %d\n", return_status);
                    l2ftp_send_error(CS_L2FTP_ERROR_IMAGE_SIZE);
                    // image_len = 0;
                    cs_timer_del(l2ftp_record.timer);
                    updateInProgress = EPON_FALSE;
                    active_update_port = 0;
                    return;
                }
#endif                
            }
            else {
                IROS_LOG_INF(IROS_MID_L2FTP,
                "l2ftp: duplicated data pdu\n");
            }
            
            l2ftp_send_ack();
            break;

        case CS_L2FTP_PDU_OPCODE_ERROR:
            IROS_LOG_INF(IROS_MID_L2FTP,
                        "l2ftp: rx an error pdu\n");
            minsz += sizeof(cs_uint32);
            if (framesz < minsz) {
                IROS_LOG_INF(IROS_MID_L2FTP,
                "l2ftp: too short for error reason\n");
                return;
            }
            retry = 0;
            //image_len = 0;
            cs_timer_del(l2ftp_record.timer);
            //app_update_file_transfer_done(image_len);
            updateInProgress = EPON_FALSE;
            active_update_port = 0;
            break;

        default:
            IROS_LOG_INF(IROS_MID_L2FTP,
            "l2ftp: unexpected opcode 0x%x\n", opcode);
            break;
        }
}

static void onu_tmfunc_l2ftp(void *data)
{
    if (retry < ONU_MAX_L2FTP_RETRY) {
        retry++;
        l2ftp_timeout();
    }
    else {
        IROS_LOG_CRI(IROS_MID_L2FTP,
                "l2ftp: max retry reached\n");
        l2ftp_send_error(CS_L2FTP_ERROR_MAX_RETRY);
        //image_len = 0;
        //app_update_file_transfer_done(image_len);
        updateInProgress = EPON_FALSE;
        active_update_port = 0;
        return;
    }

    l2ftp_record.timer = cs_timer_add(ONU_L2FTP_INTERVAL*1000,onu_tmfunc_l2ftp , NULL);
}

cs_int32 app_update_file_transfer_init()
{
    updateInProgress = EPON_TRUE;
    block_expected = 1;
    start = 1;
    retry = 0;
    //image_len = 0;

    l2ftp_record.timer = cs_timer_add(ONU_L2FTP_INTERVAL*1000,onu_tmfunc_l2ftp , NULL);

    return 1;
}

cs_int32 isFTPInProgress()
{
    if(image_len) 
        return EPON_TRUE;

    return EPON_FALSE;
}


#endif

