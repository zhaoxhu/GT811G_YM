#ifndef __IROS_CMN_MSG_H__
#define __IROS_CMN_MSG_H__

/* request id */
typedef cs_uint32           epon_req_id_t;
/* sequence num */
typedef cs_uint32           epon_seq_num_t;
/* message type */
typedef cs_uint32           epon_msg_type_t;

#define EPON_SEQ_NUM_RESET      0xFFFFFFFF

#define EPON_EVENT_MSG_CONTROL          1


#define EPON_ONU_MSG_BASE                       512

#define EPON_ONU_MSG_STATUS                     (EPON_ONU_MSG_BASE + 110)

#define EPON_ONU_MSG_RESET_IND                  (EPON_ONU_MSG_BASE + 150)

/* ONU status */
typedef enum {
        /* ONU registered */
        EPON_ONU_STATUS_REGISTERED = 1,
        /* ONU deregistered */
        EPON_ONU_STATUS_DEREGISTERED,
        /* ONU discovered */
        EPON_ONU_STATUS_DISCOVERED,
        /* ONU lost */
        EPON_ONU_STATUS_LOST
} epon_onu_status_t;


/* reason leading to onu status */
typedef enum {
        EPON_ONU_REG_NORMAL = 1,
        /* ONU deregistered normally */
        EPON_ONU_DEREG_NORMAL,
        /* ONU deregistered due to MPCP report timed out */
        EPON_ONU_DEREG_MPCP_DOWN,
        /* ONU deregistered due to OAM info timed out */
        EPON_ONU_DEREG_OAM_DOWN,
        /* ONU ready for firmware download */
        EPON_ONU_DEREG_FW_DOWNLOAD,
        /* ONU deregistered due to illegal MAC */
        EPON_ONU_DEREG_ILLEGAL,
        /* ONU deregistered due to LLID admin down */
        EPON_ONU_DEREG_LLID_ADMIN_DOWN
} epon_onu_status_reason_t;


typedef enum {
        /* normal startup */
        EPON_ONU_RESET_NORMAL = 1,
        /* ONU reset on request of OLT */
        EPON_ONU_RESET_ON_REQ_OLT,
        /* ONU reset on request of SC OOB */
        EPON_ONU_RESET_ON_REQ_OOB
} epon_onu_reset_reason_t;


typedef struct {
    cs_uint8           chip_type;    /* 1 OLT IS8020; 2 OLT IS8030; 3 ONU IS8010; 4 IS8015 */
    cs_uint8           major;      /* not used by ONU */
    cs_uint8           minor;
} __attribute__((packed)) epon_chip_version_t;

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


#endif

