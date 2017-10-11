/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include "cs_types.h"
#if 0
#ifndef EPON_REMOVE_SC_EVENT_COMPATIBILITY
#define EPON_REQ_RECORD_PROCESS                                         \
        epon_req_record_t *req;                                         \
        cs_callback_context_t context;                                \
        epon_onu_record_t *onu_rec;                                         \
        req = epon_req_record_find(ntohl(msg->req_id));                 \
        if (req) {                                                      \
                cbfunc = req->cbfunc;                                   \
                context = req->context;                                 \
                onu_rec = (epon_onu_record_t *)req->onu;                    \
                epon_req_record_free(req);                              \
        }                                                               \
        else {                                                          \
                return;                                                 \
        }

#else
#define EPON_REQ_RECORD_PROCESS                                         \
        epon_req_record_t *req;                                         \
        cs_callback_context_t context;                                \
        req = epon_req_record_find(ntohl(msg->req_id));                 \
        if (req) {                                                      \
                cbfunc = req->cbfunc;                                   \
                context = req->context;                                 \
                epon_req_record_free(req);                              \
        }                                                               \
        else {                                                          \
                return;                                                 \
        }
#endif

/* common for messages received and sent by SC */

/* request id */
typedef cs_uint32           epon_req_id_t;
/* sequence num */
typedef cs_uint32           epon_seq_num_t;
/* message type */
typedef cs_uint32           epon_msg_type_t;

#define EPON_SEQ_NUM_RESET      0xFFFFFFFF

/* ===> event connection control message */
#define EPON_EVENT_MSG_CONTROL          1

/* ===> messages exchanged within SC <=== */
#define EPON_SC_MSG_BASE                2        
/* frame received from management ethernet */
#define EPON_SC_MSG_ETHER_FRAME         EPON_SC_MSG_BASE
#define EPON_SC_MSG_EVENT_REG           (EPON_SC_MSG_BASE + 1)
#define EPON_SC_MSG_EVENT_DEREG         (EPON_SC_MSG_BASE + 2)
#define EPON_SC_MSG_IROS_ENABLE         (EPON_SC_MSG_BASE + 3)
#define EPON_SC_MSG_IROS_DISABLE        (EPON_SC_MSG_BASE + 4)
#define EPON_SC_MSG_ONU_UPDATE            (EPON_SC_MSG_BASE + 5)
#define EPON_SC_MSG_OAM_TX                 (EPON_SC_MSG_BASE + 6)

#define EPON_ONU_MSG_BASE                       512
/* generic ONU event ack message */
#define EPON_ONU_MSG_EVENT_ACK                  EPON_ONU_MSG_BASE
/* ONU reset request */
#define EPON_ONU_MSG_RESET                      (EPON_ONU_MSG_BASE + 1)
/* ONU reset response */
#define EPON_ONU_MSG_RESET_RSP                  (EPON_ONU_MSG_BASE + 2)
/* reporting a frame captured from an ONU */
#define EPON_ONU_MSG_FRAME_CAPTURED             (EPON_ONU_MSG_BASE + 3)
/* ONU l2ping request */
#define EPON_ONU_MSG_L2PING                     (EPON_ONU_MSG_BASE + 4)
/* ONU l2ping response */
#define EPON_ONU_MSG_L2PING_RSP                 (EPON_ONU_MSG_BASE + 5)
#define EPON_ONU_MSG_PORT_STATUS                (EPON_ONU_MSG_BASE + 6)
#define EPON_ONU_MSG_SET                        (EPON_ONU_MSG_BASE + 7)
#define EPON_ONU_MSG_SET_RSP                    (EPON_ONU_MSG_BASE + 8)
#define EPON_ONU_MSG_GET                        (EPON_ONU_MSG_BASE + 9 + 0x000020000 /*version 2 from version 1*/)
#define EPON_ONU_MSG_GET_RSP                    (EPON_ONU_MSG_BASE + 10 + 0x000020000 /*version 2 from version 1*/)
#define EPON_ONU_MSG_GET1                       (EPON_ONU_MSG_BASE + 9 + 0x000010000 /*version 1 from version 0*/)
#define EPON_ONU_MSG_GET_RSP1                   (EPON_ONU_MSG_BASE + 10 + 0x000010000 /*version 1 from version 0*/)
#define EPON_ONU_MSG_PORT_SET                   (EPON_ONU_MSG_BASE + 11)
#define EPON_ONU_MSG_PORT_SET_RSP               (EPON_ONU_MSG_BASE + 12)
#define EPON_ONU_MSG_PORT_GET                   (EPON_ONU_MSG_BASE + 13)
#define EPON_ONU_MSG_PORT_GET_RSP               (EPON_ONU_MSG_BASE + 14)
#define EPON_ONU_MSG_PORT_STATS_GET             (EPON_ONU_MSG_BASE + 15)
#define EPON_ONU_MSG_PORT_STATS_GET_RSP         (EPON_ONU_MSG_BASE + 16)
#define EPON_ONU_MSG_FDB_SET                    (EPON_ONU_MSG_BASE + 17)
#define EPON_ONU_MSG_FDB_SET_RSP                (EPON_ONU_MSG_BASE + 18)
#define EPON_ONU_MSG_FDB_GET                    (EPON_ONU_MSG_BASE + 19)
#define EPON_ONU_MSG_FDB_GET_RSP                (EPON_ONU_MSG_BASE + 20)
#define EPON_ONU_MSG_PORT_FDB_SET               (EPON_ONU_MSG_BASE + 21)
#define EPON_ONU_MSG_PORT_FDB_SET_RSP           (EPON_ONU_MSG_BASE + 22)
#define EPON_ONU_MSG_PORT_FDB_GET               (EPON_ONU_MSG_BASE + 23)
#define EPON_ONU_MSG_PORT_FDB_GET_RSP           (EPON_ONU_MSG_BASE + 24)
#define EPON_ONU_MSG_FDB_ENTRY_ADD              (EPON_ONU_MSG_BASE + 25)
#define EPON_ONU_MSG_FDB_ENTRY_ADD_RSP          (EPON_ONU_MSG_BASE + 26)
#define EPON_ONU_MSG_FDB_ENTRY_DEL              (EPON_ONU_MSG_BASE + 27)
#define EPON_ONU_MSG_FDB_ENTRY_DEL_RSP          (EPON_ONU_MSG_BASE + 28)
#define EPON_ONU_MSG_FDB_ENTRY_CLEAR            (EPON_ONU_MSG_BASE + 29)
#define EPON_ONU_MSG_FDB_ENTRY_CLEAR_RSP        (EPON_ONU_MSG_BASE + 30)
#define EPON_ONU_MSG_FDB_ENTRY_GET              (EPON_ONU_MSG_BASE + 31)
#define EPON_ONU_MSG_FDB_ENTRY_GET_RSP          (EPON_ONU_MSG_BASE + 32)
/* ONU generic QoS config response */
#define EPON_ONU_MSG_QOS_SET_RSP                (EPON_ONU_MSG_BASE + 33)
/* ONU QOS mode request */
#define EPON_ONU_MSG_QOS_MODE                   (EPON_ONU_MSG_BASE + 34)
/* ONU QOS mode get request */
#define EPON_ONU_MSG_QOS_MODE_GET               (EPON_ONU_MSG_BASE + 35)
/* ONU QOS mode get response */
#define EPON_ONU_MSG_QOS_MODE_RSP               (EPON_ONU_MSG_BASE + 36)
/* ONU DSCP QUEUE request */
#define EPON_ONU_MSG_DSCP_QUEUE                 (EPON_ONU_MSG_BASE + 37)
/* ONU DSCP QUEUE get request */
#define EPON_ONU_MSG_DSCP_QUEUE_GET             (EPON_ONU_MSG_BASE + 38)
/* ONU DSCP_QUEUE get response */
#define EPON_ONU_MSG_DSCP_QUEUE_RSP             (EPON_ONU_MSG_BASE + 39)
/* ONU default CoS/priority request */
#define EPON_ONU_MSG_DEFAULT_COS                (EPON_ONU_MSG_BASE + 40)
/* ONU default CoS/priority get request */
#define EPON_ONU_MSG_DEFAULT_COS_GET            (EPON_ONU_MSG_BASE + 41)
/* ONU default CoS/priority get response */
#define EPON_ONU_MSG_DEFAULT_COS_RSP            (EPON_ONU_MSG_BASE + 42)
/* ONU classifier rules set request */
#define EPON_ONU_MSG_CLASSIFIER                 (EPON_ONU_MSG_BASE + 43)
/* ONU classifier rules  get request */
#define EPON_ONU_MSG_CLASSIFIER_GET             (EPON_ONU_MSG_BASE + 44)
/* ONU classifier rules  get response */
#define EPON_ONU_MSG_CLASSIFIER_RSP             (EPON_ONU_MSG_BASE + 45)
/* ONU DOT1P QUEUE request */
#define EPON_ONU_MSG_DOT1P_QUEUE                (EPON_ONU_MSG_BASE + 46)
/* ONU DOT1P QUEUE get request */
#define EPON_ONU_MSG_DOT1P_QUEUE_GET            (EPON_ONU_MSG_BASE + 47)
/* ONU DOT1P_QUEUE get response */
#define EPON_ONU_MSG_DOT1P_QUEUE_RSP            (EPON_ONU_MSG_BASE + 48)
/* ONU SCHEDULER request */
#define EPON_ONU_MSG_SCHEDULER                  (EPON_ONU_MSG_BASE + 49)
/* ONU SCHEDULER get request */
#define EPON_ONU_MSG_SCHEDULER_GET              (EPON_ONU_MSG_BASE + 50)
/* ONU SCHEDULER get response */
#define EPON_ONU_MSG_SCHEDULER_RSP              (EPON_ONU_MSG_BASE + 51)
/* ONU FLOWCONTROL request */
#define EPON_ONU_MSG_FLOWCONTROL                (EPON_ONU_MSG_BASE + 52)
/* ONU FLOWCONTROL get request */
#define EPON_ONU_MSG_FLOWCONTROL_GET            (EPON_ONU_MSG_BASE + 53)
/* ONU FLOWCONTROL get response */
#define EPON_ONU_MSG_FLOWCONTROL_RSP            (EPON_ONU_MSG_BASE + 54)
/* ONU SERVICE_TAGGING request */
#define EPON_ONU_MSG_SERVICE_TAGGING            (EPON_ONU_MSG_BASE + 55)
/* ONU SERVICE_TAGGING get request */
#define EPON_ONU_MSG_SERVICE_TAGGING_GET        (EPON_ONU_MSG_BASE + 56)
/* ONU SERVICE_TAGGING get response */
#define EPON_ONU_MSG_SERVICE_TAGGING_RSP        (EPON_ONU_MSG_BASE + 57)
#define EPON_ONU_MSG_AUTHENTICATION_SET         (EPON_ONU_MSG_BASE + 58)
#define EPON_ONU_MSG_AUTHENTICATION_SET_RSP     (EPON_ONU_MSG_BASE + 59)
#define EPON_ONU_MSG_AUTHENTICATION_GET         (EPON_ONU_MSG_BASE + 60)
#define EPON_ONU_MSG_AUTHENTICATION_GET_RSP     (EPON_ONU_MSG_BASE + 61)
#define EPON_ONU_MSG_PORT_AUTHENTICATION_SET            (EPON_ONU_MSG_BASE + 62)
#define EPON_ONU_MSG_PORT_AUTHENTICATION_SET_RSP        (EPON_ONU_MSG_BASE + 63)
#define EPON_ONU_MSG_PORT_AUTHENTICATION_GET            (EPON_ONU_MSG_BASE + 64)
#define EPON_ONU_MSG_PORT_AUTHENTICATION_GET_RSP        (EPON_ONU_MSG_BASE + 65)
#define EPON_ONU_MSG_VLAN_SET                   (EPON_ONU_MSG_BASE + 66)
#define EPON_ONU_MSG_VLAN_SET_RSP               (EPON_ONU_MSG_BASE + 67)
#define EPON_ONU_MSG_VLAN_GET                   (EPON_ONU_MSG_BASE + 68)
#define EPON_ONU_MSG_VLAN_GET_RSP               (EPON_ONU_MSG_BASE + 69)
#define EPON_ONU_MSG_PORT_VLAN_SET              (EPON_ONU_MSG_BASE + 70)
#define EPON_ONU_MSG_PORT_VLAN_SET_RSP          (EPON_ONU_MSG_BASE + 71)
#define EPON_ONU_MSG_PORT_VLAN_GET              (EPON_ONU_MSG_BASE + 72)
#define EPON_ONU_MSG_PORT_VLAN_GET_RSP          (EPON_ONU_MSG_BASE + 73)
#define EPON_ONU_MSG_ALARM_SET                  (EPON_ONU_MSG_BASE + 74)
#define EPON_ONU_MSG_ALARM_SET_RSP              (EPON_ONU_MSG_BASE + 75)
#define EPON_ONU_MSG_ALARM_GET                  (EPON_ONU_MSG_BASE + 76)
#define EPON_ONU_MSG_ALARM_GET_RSP              (EPON_ONU_MSG_BASE + 77)
#define EPON_ONU_MSG_ALARM_RAISED               (EPON_ONU_MSG_BASE + 78)
#define EPON_ONU_MSG_IGMP_SNOOPING_SET          (EPON_ONU_MSG_BASE + 79)
#define EPON_ONU_MSG_IGMP_SNOOPING_SET_RSP      (EPON_ONU_MSG_BASE + 80)
#define EPON_ONU_MSG_IGMP_SNOOPING_GET          (EPON_ONU_MSG_BASE + 81)
#define EPON_ONU_MSG_IGMP_SNOOPING_GET_RSP      (EPON_ONU_MSG_BASE + 82)
#define EPON_ONU_MSG_PORT_IGMP_SNOOPING_SET     (EPON_ONU_MSG_BASE + 83)
#define EPON_ONU_MSG_PORT_IGMP_SNOOPING_SET_RSP (EPON_ONU_MSG_BASE + 84)
#define EPON_ONU_MSG_PORT_IGMP_SNOOPING_GET     (EPON_ONU_MSG_BASE + 85)
#define EPON_ONU_MSG_PORT_IGMP_SNOOPING_GET_RSP (EPON_ONU_MSG_BASE + 86)
#define EPON_ONU_MSG_MCAST_DISALLOW             (EPON_ONU_MSG_BASE + 87)
#define EPON_ONU_MSG_PORT_LOOPBACK_SET          (EPON_ONU_MSG_BASE + 104)
#define EPON_ONU_MSG_PORT_LOOPBACK_SET_RSP      (EPON_ONU_MSG_BASE + 105)
#define EPON_ONU_MSG_PORT_LOOPBACK_GET          (EPON_ONU_MSG_BASE + 106)
#define EPON_ONU_MSG_PORT_LOOPBACK_GET_RSP      (EPON_ONU_MSG_BASE + 107)
#define EPON_ONU_MSG_UPDATE                     (EPON_ONU_MSG_BASE + 108)
#define EPON_ONU_MSG_UPDATE_RSP                 (EPON_ONU_MSG_BASE + 109)
#define EPON_ONU_MSG_STATUS                     (EPON_ONU_MSG_BASE + 110)
#define EPON_ONU_MSG_MAC_LRN_SET                (EPON_ONU_MSG_BASE + 111)
#define EPON_ONU_MSG_MAC_LRN_SET_RSP            (EPON_ONU_MSG_BASE + 112)
#define EPON_ONU_MSG_MAC_LRN_GET                (EPON_ONU_MSG_BASE + 113)
#define EPON_ONU_MSG_MAC_LRN_GET_RSP            (EPON_ONU_MSG_BASE + 114)
#define EPON_ONU_MSG_MEM_SET                    (EPON_ONU_MSG_BASE + 115)
#define EPON_ONU_MSG_MEM_SET_RSP                (EPON_ONU_MSG_BASE + 116)
#define EPON_ONU_MSG_MEM_DUMP                   (EPON_ONU_MSG_BASE + 117)
#define EPON_ONU_MSG_MEM_DUMP_RSP               (EPON_ONU_MSG_BASE + 118)
#define EPON_ONU_MSG_OAMPDU_TX                  (EPON_ONU_MSG_BASE + 119)
#define EPON_ONU_MSG_OAMPDU_TX_RSP              (EPON_ONU_MSG_BASE + 120)
#define EPON_ONU_MSG_OAM_POST                   (EPON_ONU_MSG_BASE + 121)
#define EPON_ONU_MSG_PORT_OAM_GET               (EPON_ONU_MSG_BASE + 122)
#define EPON_ONU_MSG_PORT_OAM_GET_RSP           (EPON_ONU_MSG_BASE + 123)
#define EPON_ONU_MSG_PORT_OAM_STATS_GET         (EPON_ONU_MSG_BASE + 124)
#define EPON_ONU_MSG_PORT_OAM_STATS_GET_RSP     (EPON_ONU_MSG_BASE + 125)
#define EPON_ONU_MSG_OPAQUE                     (EPON_ONU_MSG_BASE + 126)
#define EPON_ONU_MSG_OPAQUE_RSP                 (EPON_ONU_MSG_BASE + 127)
#define EPON_ONU_MSG_DOT1XPDU_TX                (EPON_ONU_MSG_BASE + 128)
#define EPON_ONU_MSG_DOT1XPDU_TX_RSP            (EPON_ONU_MSG_BASE + 129)
#define EPON_ONU_MSG_DOT1X_POST                 (EPON_ONU_MSG_BASE + 130)
#define EPON_ONU_MSG_LOOPB_POST                 (EPON_ONU_MSG_BASE + 131)
#define EPON_ONU_MSG_DEV_SET                    (EPON_ONU_MSG_BASE + 132)
#define EPON_ONU_MSG_DEV_SET_RSP                (EPON_ONU_MSG_BASE + 133)
#define EPON_ONU_MSG_DEV_GET                    (EPON_ONU_MSG_BASE + 134)
#define EPON_ONU_MSG_DEV_GET_RSP                (EPON_ONU_MSG_BASE + 135)
#define EPON_ONU_MSG_BOOTPARAM_SET              (EPON_ONU_MSG_BASE + 136)
#define EPON_ONU_MSG_BOOTPARAM_SET_RSP          (EPON_ONU_MSG_BASE + 137)
#define EPON_ONU_MSG_BOOTPARAM_GET              (EPON_ONU_MSG_BASE + 138)
#define EPON_ONU_MSG_BOOTPARAM_GET_RSP          (EPON_ONU_MSG_BASE + 139)
#define EPON_ONU_MSG_AUTH_TIMER_SET             (EPON_ONU_MSG_BASE + 140)
#define EPON_ONU_MSG_AUTH_TIMER_SET_RSP         (EPON_ONU_MSG_BASE + 141)
#define EPON_ONU_MSG_AUTH_TIMER_GET             (EPON_ONU_MSG_BASE + 142)
#define EPON_ONU_MSG_AUTH_TIMER_GET_RSP         (EPON_ONU_MSG_BASE + 143)
#define EPON_ONU_MSG_PORT_STAT_POLL_SET         (EPON_ONU_MSG_BASE + 144)
#define EPON_ONU_MSG_PORT_STAT_POLL_SET_RSP     (EPON_ONU_MSG_BASE + 145)
#define EPON_ONU_MSG_PORT_STAT_POLL_GET         (EPON_ONU_MSG_BASE + 146)
#define EPON_ONU_MSG_PORT_STAT_POLL_GET_RSP     (EPON_ONU_MSG_BASE + 147)
#define EPON_ONU_MSG_RESET_POST                 (EPON_ONU_MSG_BASE + 148)
#define EPON_ONU_MSG_FDB_LIMIT_REACHED            (EPON_ONU_MSG_BASE + 149)
#define EPON_ONU_MSG_RESET_IND                  (EPON_ONU_MSG_BASE + 150)
#define EPON_ONU_MSG_PORT_ADMIN_SET                  (EPON_ONU_MSG_BASE + 151)
#define EPON_ONU_MSG_MAC_CLA_ENTRY_SET              (EPON_ONU_MSG_BASE + 152)
#define EPON_ONU_MSG_MAC_CLA_ENTRY_SET_RSP          (EPON_ONU_MSG_BASE + 153)
#define EPON_ONU_MSG_MAC_CLA_ENTRY_GET              (EPON_ONU_MSG_BASE + 154)
#define EPON_ONU_MSG_MAC_CLA_ENTRY_GET_RSP          (EPON_ONU_MSG_BASE + 155)
#define EPON_ONU_MSG_SPECIAL_PKT_ENTRY_SET              (EPON_ONU_MSG_BASE + 156)
#define EPON_ONU_MSG_SPECIAL_PKT_ENTRY_SET_RSP          (EPON_ONU_MSG_BASE + 157)
#define EPON_ONU_MSG_SPECIAL_PKT_ENTRY_GET              (EPON_ONU_MSG_BASE + 158)
#define EPON_ONU_MSG_SPECIAL_PKTA_ENTRY_GET_RSP          (EPON_ONU_MSG_BASE + 159)
#define EPON_ONU_MSG_MC_FILTER_ADD                      (EPON_ONU_MSG_BASE + 160)
#define EPON_ONU_MSG_MC_FILTER_ADD_RSP                  (EPON_ONU_MSG_BASE + 161)
#define EPON_ONU_MSG_MC_FILTER_DEL                      (EPON_ONU_MSG_BASE + 162)
#define EPON_ONU_MSG_MC_FILTER_DEL_RSP                  (EPON_ONU_MSG_BASE + 163)
#define EPON_ONU_MSG_MC_FILTER_GET                      (EPON_ONU_MSG_BASE + 164)
#define EPON_ONU_MSG_MC_FILTER_GET_RSP                  (EPON_ONU_MSG_BASE + 165)
#define EPON_ONU_MSG_BM_AUTORESET_ENABLE                (EPON_ONU_MSG_BASE + 166)
#define EPON_ONU_MSG_BM_AUTORESET_ENABLE_RSP            (EPON_ONU_MSG_BASE + 167)
#define EPON_ONU_MSG_BM_AUTORESET_GET                   (EPON_ONU_MSG_BASE + 168)
#define EPON_ONU_MSG_BM_AUTORESET_GET_RSP               (EPON_ONU_MSG_BASE + 169)
#define EPON_ONU_MSG_IGMP_CONFIG_SET                    (EPON_ONU_MSG_BASE + 170)
#define EPON_ONU_MSG_IGMP_CONFIG_SET_RSP                (EPON_ONU_MSG_BASE + 171)
#define EPON_ONU_MSG_IGMP_CONFIG_GET                    (EPON_ONU_MSG_BASE + 172)
#define EPON_ONU_MSG_IGMP_CONFIG_GET_RSP                (EPON_ONU_MSG_BASE + 173)
#define EPON_ONU_MSG_PON_ABNORMAL_IND                   (EPON_ONU_MSG_BASE + 174)

#define EPON_ONU_MSG_STP_SET                          (EPON_ONU_MSG_BASE + 175)
#define EPON_ONU_MSG_STP_SET_RSP                      (EPON_ONU_MSG_BASE + 176)
#define EPON_ONU_MSG_STP_GET                          (EPON_ONU_MSG_BASE + 177)
#define EPON_ONU_MSG_STP_GET_RSP                      (EPON_ONU_MSG_BASE + 178)

#define EPON_ONU_MSG_STP_STATE_GET                      (EPON_ONU_MSG_BASE + 179)
#define EPON_ONU_MSG_STP_STATE_GET_RSP              (EPON_ONU_MSG_BASE + 180)

#define EPON_ONU_MSG_PORT_STP_SET                      (EPON_ONU_MSG_BASE + 181)
#define EPON_ONU_MSG_PORT_STP_SET_RSP              (EPON_ONU_MSG_BASE + 182)
#define EPON_ONU_MSG_PORT_STP_GET                      (EPON_ONU_MSG_BASE + 183)
#define EPON_ONU_MSG_PORT_STP_STATE_GET              (EPON_ONU_MSG_BASE + 184)

#define EPON_ONU_MSG_SERVICE_TAGGING2                (EPON_ONU_MSG_BASE + 185)
#define EPON_ONU_MSG_SERVICE_TAGGING_GET2        (EPON_ONU_MSG_BASE + 186)
#define EPON_ONU_MSG_SERVICE_TAGGING_RSP2               (EPON_ONU_MSG_BASE + 187)
#define EPON_ONU_MSG_VOIP_SET                           (EPON_ONU_MSG_BASE + 188)

#define EPON_ONU_MSG_CTC_OAM_STATUS         0x03030001
#define EPON_ONU_MSG_GPIO_TRIGGER              0x03030002

#define EPON_PON_MSG_BASE                               3072
#define EPON_PON_MSG_UC_KEY_REQ                         (EPON_PON_MSG_BASE + 1)
#define EPON_PON_MSG_UC_KEY_RSP                         (EPON_PON_MSG_BASE + 2)
#define EPON_PON_MSG_MC_KEY_REQ                         (EPON_PON_MSG_BASE + 3)
#define EPON_PON_MSG_MC_KEY_RSP                         (EPON_PON_MSG_BASE + 4)

typedef struct {
        epon_msg_type_t                 msg_type;
        cs_uint16                   cipher_counter;     
        epon_encryption_mode_t          mode;
        cs_uint8                    key_index;
        cs_boolean                  update_cipher_counter;
}__attribute__((packed)) epon_pon_itf_uc_key_req_t;

typedef struct {
        epon_msg_type_t                 msg_type;
        epon_onu_device_id_t            device;
        cs_uint32                   result;
        cs_uint32                   key[4];
}__attribute__((packed)) epon_pon_itf_uc_key_rsp_t;



typedef struct {
        epon_msg_type_t                 msg_type;
        epon_seq_num_t                  seq_num;
} __attribute__((packed)) epon_cmn_msg_event_header_t;


typedef struct {
        epon_msg_type_t                 msg_type;
        epon_seq_num_t                  seq_num;
} __attribute__((packed)) epon_cmn_msg_event_ack_t;

typedef struct {
        epon_msg_type_t                 msg_type;
        epon_seq_num_t                  seq_num;
        epon_macaddr_t                  macaddr;
} __attribute__((packed)) epon_event_msg_control_t;

#endif
#endif
