/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#ifndef _L2FTP_H_
#define _L2FTP_H_

#define EPON_L2FTP_BLOCK_SIZE           1024

#define EPON_L2FTP_PDU_OPCODE_RRQ       1
#define EPON_L2FTP_PDU_OPCODE_DATA      3
#define EPON_L2FTP_PDU_OPCODE_ACK       4
#define EPON_L2FTP_PDU_OPCODE_ERROR     5
#define EPON_L2FTP_PDU_OPCODE_RRQ_ONU   6
#define EPON_L2FTP_PDU_OPCODE_ACK_ONU   7
#define EPON_L2FTP_PDU_OPCODE_ERROR_ONU 8

#define EPON_L2FTP_ERROR_IMAGE_SIZE     1
#define EPON_L2FTP_ERROR_MAX_RETRY      2
#define EPON_L2FTP_ERROR_IMAGE_READ     3

/* ftp control header */
typedef struct {
        cs_uint16           opcode;
        epon_olt_device_id_t    device;
        epon_olt_version_t      version;
} __attribute__((packed)) epon_l2ftp_header_t;

/* olt to sc pdu: start file transfer */
typedef struct {
        epon_l2ftp_header_t     header;
} __attribute__((packed)) epon_l2ftp_pdu_rrq_t;

/* sc to olt pdu: contain a file block */
typedef struct {
        epon_l2ftp_header_t     header;
        cs_uint16           block;
        cs_uint16           length;
        cs_uint8           data[EPON_L2FTP_BLOCK_SIZE];
} __attribute__((packed)) epon_l2ftp_pdu_data_t;

/* olt to sc pdu: acknowledge a file block */
typedef struct {
        epon_l2ftp_header_t     header;
        cs_uint16           block;
} __attribute__((packed)) epon_l2ftp_pdu_ack_t;

/* olt to sc or sc to olt pdu: report an error */
typedef struct {
        epon_l2ftp_header_t     header;
        cs_uint32           reason;
} __attribute__((packed)) epon_l2ftp_pdu_error_t;

void epon_l2ftp_proc(epon_ether_frame_t *frame);
#ifdef GEESUN_REMOVED
void epon_l2ftp_image_ready(void *, cs_uint8 *,
                            cs_uint32, epon_olt_startup_config_t *);
#endif
extern int isFTPInProgress();
#endif
