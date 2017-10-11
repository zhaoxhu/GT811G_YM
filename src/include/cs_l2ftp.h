#ifndef _CS_L2FTP_H_
#define _CS_L2FTP_H_

#define CS_L2FTP_BLOCK_SIZE           1024

#define CS_L2FTP_PDU_OPCODE_RRQ       1
#define CS_L2FTP_PDU_OPCODE_WRQ       2
#define CS_L2FTP_PDU_OPCODE_DATA      3
#define CS_L2FTP_PDU_OPCODE_ACK       4
#define CS_L2FTP_PDU_OPCODE_ERROR     5
#define CS_L2FTP_PDU_OPCODE_RRQ_ONU   6
#define CS_L2FTP_PDU_OPCODE_ACK_ONU   7
#define CS_L2FTP_PDU_OPCODE_ERROR_ONU 8


#define CS_L2FTP_ERROR_IMAGE_SIZE     1
#define CS_L2FTP_ERROR_MAX_RETRY      2
#define CS_L2FTP_ERROR_IMAGE_READ     3

/* ftp control header */
typedef struct {
    cs_uint16   opcode;
    cs_uint32   device;
    cs_uint32   version;
} __attribute__((packed)) cs_l2ftp_header_t;

/* olt to sc pdu: start file transfer */
typedef struct {
    cs_l2ftp_header_t     header;
} __attribute__((packed)) cs_l2ftp_pdu_rrq_t;

typedef enum {
    SC_REQUEST_MEMORY = 0,
    SC_REQUEST_FILE   = 1,
    SC_REUQEST_LOG    = 2,
} SC_REQUEST_ENUM;

typedef struct {
    cs_l2ftp_header_t header;
    cs_int32          req_type;
    cs_int32          req_len;
} __attribute__((packed)) cs_l2ftp_sc_rrq_t;

typedef struct {
    cs_l2ftp_header_t header;
    cs_int32          ack_len;
} __attribute__((packed)) cs_l2ftp_dev_sc_ack_t;

/* sc to olt pdu: contain a file block */
typedef struct {
    cs_l2ftp_header_t   header;
    cs_uint16           block;
    cs_uint16           length;
    cs_uint8            data[CS_L2FTP_BLOCK_SIZE];
} __attribute__((packed)) cs_l2ftp_pdu_data_t;
#define CS_L2FTP_PDU_HDR_LEN (sizeof(cs_l2ftp_header_t)+4)

/* olt to sc pdu: acknowledge a file block */
typedef struct {
    cs_l2ftp_header_t   header;
    cs_uint16           block;
} __attribute__((packed)) cs_l2ftp_pdu_ack_t;

/* olt to sc or sc to olt pdu: report an error */
typedef struct {
    cs_l2ftp_header_t   header;
    cs_uint32   reason;
} __attribute__((packed)) cs_l2ftp_pdu_error_t;

#endif

