/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#ifndef _ONU_MSG_H_
#define _ONU_MSG_H_
#if 0
#include "iros_config.h"
#include "iros_api.h"

void setControlPktL2Dest(unsigned char *buffer, unsigned int len);

#define EPON_ONU_SET_CTRL_DEST(a, b)    setControlPktL2Dest(a, b)

/* define the messages exchanged between SC and ONU */
#ifdef HAVE_OOB_MGMT
#define EPON_ONU_CMD_MSG_BUILD    EPON_OOB_ONU_MSG_BUILD_BEGIN
#else
#define EPON_ONU_CMD_MSG_BUILD    EPON_ONU_MSG_BUILD_BEGIN
#endif

#define EPON_ONU_MSG_BUILD_BEGIN(type, code)                            \
        epon_result_code_t result = EPON_RESULT_SUCCESS;                \
        epon_req_record_t *req;                                         \
        type *onu_msg=NULL;                                                  \
        epon_ether_frame_t *frame;                                      \
        epon_onu_record_t *onu;                                         \
        onu = epon_onu_record_find_by_device_id(msg->device);           \
        if (onu == NULL) {                                              \
                result = EPON_RESULT_NO_ONU_MAC;                        \
        }                                                               \
        else if ((frame = epon_frame_alloc(&onu->olt->macaddr[0],       \
                                           &onu->macaddr[0],            \
                                           EPON_LAYER3_TYPE_MSG)) == NULL) { \
                result = EPON_RESULT_NO_MEMORY;                         \
        }                                                               \
        else if ((req = epon_req_record_alloc(msg->msg_type, msg->cbfunc, \
                                                 msg->context)) == NULL) { \
                result = EPON_RESULT_NO_REQ_RECORD;                     \
                epon_env_free(frame);                \
        }                                                               \
        else {                                                          \
                onu_msg = (type *)((cs_uint8 *)frame +              \
                                   EPON_FRAME_HEADER_LEN);              \
                onu_msg->msg_type = htonl(code);                        \
                onu_msg->req_id = htonl(req->req_id);

#define EPON_ONU_MSG_BUILD_END(type)                                    \
                epon_env_ether_send((char *)frame,                      \
                                    EPON_FRAME_HEADER_LEN + sizeof(type)); \
                epon_env_free(frame);                                   \
        }


#define EPON_OOB_ONU_MSG_BUILD_BEGIN(type, code)                            \
        epon_result_code_t result = EPON_RESULT_SUCCESS;                \
        epon_req_record_t *req;                                         \
        type *onu_msg=NULL;                                                  \
        epon_ether_frame_t *frame;                                      \
        epon_hello_onu_info_t *onu;                                         \
        onu = epon_hello_onu_info_find_by_macaddr(msg->device);           \
        if (onu == NULL) {                                              \
                epon_env_error("ONU record not found\n"); \
                result = EPON_RESULT_NO_ONU_MAC;                        \
        }                                                               \
        else if(!isOOBChip(&onu->chip_version) && \
                     onu->state != EPON_HELLO_PEER_STATE_DOWNLOADING) \
        {\
                epon_env_error("Inv ONU chip\n"); \
                result = EPON_RESULT_INV_CHIP_VERSION;                        \
        }\
    else if(onu->peer_state != EPON_HELLO_PEER_STATE_ONUCONN && \
               onu->state != EPON_HELLO_PEER_STATE_DOWNLOADING) \
    { \
                epon_env_error("inv onu state (0x%x)\n", onu->peer_state); \
                result = EPON_RESULT_FAIL;                         \
    } \
        else if ((frame = epon_frame_alloc(&onu->device[0],       \
                                           &onu->device[0],            \
                                           EPON_LAYER3_TYPE_MSG)) == NULL) { \
                epon_env_error("frame malloc failed\n"); \
                result = EPON_RESULT_NO_MEMORY;                         \
        }                                                               \
        else if ((req = epon_req_record_alloc(msg->msg_type, msg->cbfunc, \
                                                 msg->context)) == NULL) { \
                epon_env_error("req malloc failed\n"); \
                epon_env_free(frame);                \
                result = EPON_RESULT_NO_REQ_RECORD;                     \
        }                                                               \
        else {                                                          \
                onu_msg = (type *)((cs_uint8 *)frame +              \
                                   EPON_FRAME_HEADER_LEN);              \
                onu_msg->msg_type = htonl(code);                        \
                onu_msg->req_id = htonl(req->req_id);            \


/* fields common to all responses from ONU to SC */
typedef struct {
        epon_msg_type_t                 msg_type;
        epon_req_id_t                   req_id;
        epon_result_code_t              code;
} __attribute__((packed)) epon_onu_msg_common_rsp_t;

/* ===> Registration and Configuration <=== */

/* ONU reset request from SC to ONU */



typedef struct {
        epon_msg_type_t                 msg_type;
        epon_seq_num_t                  seq_num;
        epon_onu_device_id_t            device;
        epon_alarm_type_t               type;
        unsigned int                    port;
        unsigned int                    len;
        unsigned char                   oampdu[EPON_FRAME_PAYLOAD_LEN];
} __attribute__((packed)) epon_onu_msg_oam_post_t;


typedef struct {
        epon_msg_type_t                 msg_type;
        epon_seq_num_t                  seq_num;
        epon_onu_device_id_t            device;
        epon_alarm_type_t               type;
        unsigned int                    port;
        unsigned int                    len;
        unsigned char                   dot1xpdu[EPON_FRAME_PAYLOAD_LEN];
} __attribute__((packed)) epon_onu_msg_dot1x_post_t;

typedef struct {
        epon_msg_type_t                 msg_type;
        epon_seq_num_t                  seq_num;
        epon_onu_device_id_t            device;
        unsigned int                    port;
        unsigned char                   status;
} __attribute__((packed)) epon_onu_msg_loopb_post_t;

#ifdef HAVE_OOB_MGMT
/* ONU status event message */
typedef struct {
        epon_msg_type_t                 msg_type;
        epon_seq_num_t                  seq_num;
        epon_onu_device_id_t            device;
        epon_onu_status_t               status;
        epon_onu_status_reason_t        reason;
        epon_macaddr_t                  olt_mac;
        cs_port_id_t                  llid;
} __attribute__((packed)) epon_msg_onu_status_t;

typedef struct {
        epon_msg_type_t                 msg_type;
        epon_seq_num_t                  seq_num;
        epon_onu_device_id_t            device;
        epon_onu_reset_reason_t         reason;
        cs_boolean                  req_cfg;
} __attribute__((packed)) epon_msg_onu_reset_ind_t;
typedef struct {
        epon_msg_type_t                 msg_type;
        epon_seq_num_t                  seq_num;
        epon_onu_device_id_t            device;
        epon_onu_pon_status_t           status;
} __attribute__((packed)) epon_msg_onu_pon_abnormal_ind_t;
typedef struct {
        epon_msg_type_t                 msg_type;
        epon_seq_num_t                  seq_num;
        epon_onu_device_id_t            device;
        unsigned char                   port;
        unsigned char                   status;
} __attribute__((packed)) epon_msg_onu_oam_voip_set_t;
#endif


typedef struct {
        epon_msg_type_t                  msg_type;
        epon_seq_num_t                   seq_num;
        epon_onu_device_id_t            device;
        epon_onu_ctc_oam_status_t   status;
} __attribute__((packed)) epon_msg_onu_ctc_oam_status_t;



/* ===> System Maintenance, Monitoring and Testing <=== */



typedef struct {
        epon_msg_type_t                 msg_type;
        epon_seq_num_t                  seq_num;
        epon_alarm_type_t               type;
        epon_alarm_info_t               info;
} __attribute__((packed)) epon_onu_msg_alarm_raised_t;


#endif
#endif
