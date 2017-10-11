/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#ifndef _CORE_H_
#define _CORE_H_

#include "iros_config.h"
#include "message.h"
#include "hello.h"

/* maximum number of outstanding requests from SC */
#ifdef NO_SC_FULL
#define EPON_MAX_REQUEST                16
#else
#define EPON_MAX_REQUEST                512
#endif

/* number of ticks before a request times out */
#define EPON_REQUEST_TIMEOUT            6000
#define EPON_FW_UPDATE_REQUEST_TIMEOUT  12000

/* number of ticks before a ctc request times out */
#define EPON_CTC_REQUEST_TIMEOUT 1000

#define EPON_APPS_BOOT_PARM_SIZE        128

#define EPON_API_QUOTA                  1000

#define SC_PRESENT_MASK(b)         (0x00000001<<(b))
#define SC_BITON(m, b)                   (((m) & SC_PRESENT_MASK(b)) != 0)
#define SC_SETBIT(m, b)                  ((m) |= SC_PRESENT_MASK(b))
#define SC_CLRBIT(m, b)                  ((m) &= SC_PRESENT_MASK(b))
#ifdef GEESUN_REMOVED
typedef struct {
    cs_uint32    timer;
    cs_int32    device;
    cs_int32    llid;
}ctc_onu_upgrade_timer_t;

typedef struct {
        cs_uint32 upgrade_status;
        cs_uint32 send_len;
        cs_uint16 block;
        cs_uint16 opcode;
        ctc_onu_upgrade_timer_t timer;
        cs_uint16 timeout;
        cs_uint8 count;
        epon_olt_oampdu_info_t packet;
} epon_ctc_onu_upgrade_t;

/* information regarding a ctc oam command */
typedef struct {
        cs_uint32 timer;
        cs_boolean valid;
        cs_port_id_t llid;
        cs_uint32 cmdCode;
        unsigned char branch;
        unsigned short leaf;
        cs_callback_context_t context;
        cs_uint32 event;
        epon_olt_device_id_t olt_device;
        cs_uint8 uni_port;
        epon_ctc_onu_upgrade_t upgrade;
} epon_ctc_cmd_record_t;


typedef struct {
        epon_event_handler_t    handler;
        epon_event_context_t    context;
} epon_events_t;

/* information regarding an outstanding request */
typedef struct {
        /* the timer structure */
        cs_uint32                    timer;
        /* whether this record is in use */
        cs_boolean                  valid;
        /* system wide unique request id */
        epon_req_id_t                   req_id;
        /* type of message associated with this request */
        epon_msg_type_t                 msg_type;
        /* the call-back function pointer */
        void                            *cbfunc;
        /* the call-back context */
        cs_callback_context_t         context;

#ifndef EPON_REMOVE_SC_EVENT_COMPATIBILITY
        /* onu record */
        void                            *onu;
#endif

} epon_req_record_t;

struct epon_olt_record_t;
struct epon_onu_record_t;
struct epon_hello_onu_info_t;

/* information regarding olt image download thru l2ftp */
typedef struct epon_l2ftp_record_t {
        /* the timer structure */
        cs_uint32                    timer;
        /* number of the block to send */
        cs_uint16                   block_next;
        /* number of retries */
        cs_uint32                   retry;
        /* back pointer to the olt record */
        struct epon_olt_record_t        *olt;

        /* back pointer to the onu hello  record */
        struct epon_hello_onu_info_t  *onu_hello;

        /* image file descriptor: to be deprecated */
        cs_int32                    fd_olt_image;

        /* image location in memory */
        cs_uint8                    *image_ptr;
        /* image length in memory */
        cs_uint32                   image_len;

        /* end of image indication */
        cs_uint8                    end_of_image;
        /* application boot parms */
        cs_uint8                    parm[EPON_APPS_BOOT_PARM_SIZE];
        /* offset into boot parms */
        cs_uint8                    offset;
        /* back pointer to the onu record */
        struct epon_onu_record_t        *onu;
        /* rrq received for onu */
        cs_uint8                    onu_rrq_rx;
} epon_l2ftp_record_t;

/* information regarding onu register status */
typedef struct epon_onu_reg_status_t {
        epon_onu_status_t               status;
        epon_onu_status_reason_t        reason;
        cs_uint32                   recv_version;
} epon_onu_reg_status_t;

/* information regarding a registered ONU */
typedef struct epon_onu_record_t {
        /* whether this record is in use */
        cs_boolean                  valid;
        /* back pointer to the olt record */
        struct epon_olt_record_t        *olt;
        /* olt llid based logical port id */
        cs_port_id_t                  port;
        /* the ONU device id */
        epon_onu_device_id_t            device;
        /* the ONU logical id */
        epon_onu_logical_id_t       logical;
        /* the ONU device-wide MAC address */
        epon_macaddr_t                  macaddr;
        /* ONU to SC communication enabled */
        cs_boolean                  enabled;
        /* the expected receiving seqnum for ONU to SC communication */
        epon_seq_num_t                  seq_next;
        /* l2ftp record */
        epon_l2ftp_record_t             l2ftp;
        /* ONU register info */
        epon_onu_reg_info_t             reg_info;
        /* ONU register status */
        epon_onu_reg_status_t           reg_status;
        /* the ONU version */
        epon_image_version_t            version;
} epon_onu_record_t;

/* information regarding a registered OLT */
typedef struct epon_olt_record_t {
        /* whether this record is in use */
        cs_boolean                  valid;
        /* the OLT device id */
        epon_olt_device_id_t            device;
        /* the OLT device id */
        epon_olt_version_t              version;
        /* the OLT status */
        epon_olt_status_t               status;
        /* the OLT device-wide MAC address */
        epon_macaddr_t                  macaddr;
        /* OLT to SC communication enabled */
        cs_boolean                  enabled;
        /* the expected receiving seqnum for OLT to SC communication */
        epon_seq_num_t                  seq_next;
        /* ONU registered to this OLT */
        epon_onu_record_t               onu[EPON_MAX_LLIDS_PER_OLT_DEVICE];
        /* l2ftp record */
        epon_l2ftp_record_t             l2ftp;
        /* hello state */
        epon_hello_state_t              hello_state;
        /* hello timeout tick */
        cs_uint16                   hello_expire;
} epon_olt_record_t;

/* information regarding hello protocol */
typedef struct {
        /* the timer structure */
        cs_uint32                    timer;
} epon_hello_record_t;

#define ONU_MAX_TRACE_ENTRY 128
/* onu traces buffer */
typedef struct {
        int write;
        int buffers[ONU_MAX_TRACE_ENTRY][16];
} epon_onu_trace_t;

typedef struct epon_hello_onu_info_t {
        cs_boolean          valid;
        /* onu device id */
        epon_onu_device_id_t        device;
        /* onu mac addr */
        epon_macaddr_t          macaddr;
        /* the ONU logical id */
        epon_logical_link_id_t      llid;
        /* hello state */
        epon_hello_state_t          state;
        /* hello timeout tick*/
        cs_uint16           hello_expire;

        cs_uint16               peer_state;

        /* registration status */
        epon_onu_status_t       status;

        /* olt device */
        epon_macaddr_t          olt_mac;

        /* port */
        cs_port_id_t          port;

        /*l2 ftp for image download */
        epon_l2ftp_record_t     l2ftp;

        /* slot, module */
        unsigned int            index;
        unsigned int            slot;
        unsigned int            module;

        /* version */
        unsigned int            version;

        /* back pointer to onu record in olt*/
        void                *onu_in_olt;

        /* ONU to SC communication enabled */
        cs_boolean                  enabled;
        /* the expected receiving seqnum for ONU to SC communication */
        epon_seq_num_t                  seq_next;

        int                     onu_update_in_progress;
    epon_chip_version_t     chip_version;
    unsigned char       authenticationState;
} epon_hello_onu_info_t;

/* SC global info */
typedef struct {
        /* whether iROS SC is enabled or not */
        cs_boolean                  enabled;
        /* the management port MAC address */
        epon_macaddr_t                  macaddr;

        /* the next request id to use */
        epon_req_id_t                   req_next;
        /* records for outstanding requests */
        epon_req_record_t               req_record[EPON_MAX_REQUEST];

        /* the registered olt */
        epon_olt_record_t               olt[EPON_MAX_OLT_DEVICES];

        /* the hello protocol */
        epon_hello_record_t             hello_record;

        /* whether to enable ONU registration flapping suppression */
        cs_boolean                  onu_reg_flapping_suppression;
        /* SC-OLT hello interval */
        cs_uint32                   hello_interval;
        /* SC-OLT dead interval */
        cs_uint32                   dead_interval;

        /* SC sw version */
        cs_uint32                   sc_version;

        /* onu hello records */
        epon_hello_onu_info_t       onu_hello[EPON_MAX_ONU_DEVICES];

        /* API rate control */
        int                             api_quota;

        /* CTC cmd record */
        epon_ctc_cmd_record_t       ctc_cmd_record[EPON_MAX_REQUEST];
} epon_sc_node_t;

#endif
typedef struct {
        epon_onu_device_id_t    device;
        epon_hello_state_t          state;
} __attribute__((packed)) epon_oob_onu_hello_info_t;
typedef struct {
        /* SC-ONU hello interval */
        cs_uint32           hello_interval;
        /* SC-ONU dead interval */
        cs_uint32           dead_interval;
        /* SC sw version */
        cs_uint32           sc_version;
        /* chip version */
        epon_chip_version_t  chip_version;
        /* number of onus in the hello */
        cs_uint16           onu_number;
        /* list of onu info */
        epon_oob_onu_hello_info_t   onu_info[1];
} __attribute__((packed)) epon_sc_onu_hello_pdu_t;

typedef struct {
    /* onu device id */
    epon_onu_device_id_t    device;
    /* hello state */
    epon_hello_state_t      state;
    /* sc mac address */
    epon_macaddr_t          sc_macaddr;
    /* onu version */
    epon_onu_version_t      version;
   /* chip version */
   epon_chip_version_t  chip_version;

   cs_uint8 registrationState;
   cs_uint8 authenticationState;
} __attribute__((packed)) epon_onu_hello_pdu_t;


#ifdef GEESUN_REMOVED
extern epon_sc_node_t   sc_node;
extern epon_events_t    epon_events[];

/* function prototype */
epon_ctc_cmd_record_t *epon_ctc_record_alloc (
        epon_olt_device_id_t, cs_port_id_t, cs_uint32,
        unsigned char, unsigned short, cs_callback_context_t,
        cs_uint32, cs_uint8);
void epon_ctc_record_free (epon_ctc_cmd_record_t *);
epon_ctc_cmd_record_t *epon_ctc_record_find_with_cmd (epon_olt_device_id_t, cs_port_id_t,
        cs_uint32, unsigned char, unsigned short, unsigned char);
epon_ctc_cmd_record_t *epon_ctc_record_find_with_resp (cs_port_id_t,
        cs_uint32, unsigned char, unsigned short, unsigned char);
void epon_ctc_request_timeout (void *);
cs_boolean
epon_ctc_record_find_with_image_upgrade();

epon_return_code_t epon_sc_init();
epon_req_record_t *epon_req_record_alloc(epon_msg_type_t, void *,
                                         cs_callback_context_t);
epon_req_id_t epon_req_id_alloc();
void epon_req_record_free(epon_req_record_t *);
epon_req_record_t *epon_req_record_find(epon_req_id_t);
void epon_tmfunc_req_record(void *);
epon_onu_record_t *epon_onu_record_alloc(epon_olt_record_t *);
void epon_onu_record_free(epon_onu_record_t *);
epon_onu_record_t *epon_onu_record_find_by_device_id(epon_onu_device_id_t);
epon_onu_record_t *epon_onu_record_find_by_olt_port(epon_olt_device_id_t,
                                                    cs_port_id_t);
cs_boolean epon_oob_onu_seq_num_check(epon_onu_device_id_t, epon_seq_num_t,
                                      epon_hello_onu_info_t **);
epon_port_type_t epon_olt_port_type(cs_port_id_t);

epon_hello_onu_info_t *epon_hello_onu_info_find_by_macaddr(epon_macaddr_t macaddr);
epon_hello_onu_info_t *epon_hello_onu_info_alloc();
void epon_hello_onu_info_free(epon_hello_onu_info_t *onu_hello);
epon_hello_onu_info_t *epon_hello_onu_info_find_by_index(int index);
void epon_ctc_record_show();

#ifdef HAVE_OOB_MGMT
extern void addOnuTraces(int index, int type, int parm1, int parm2, int parm3, int parm4, int parm5, int parm6);
#define CHECKONUCMDRESULT(a, b, c, d, e, f, g, h, i)       addOnuTraces(a, b, c, d, e, f, g, h);\
        if (i != EPON_RESULT_SUCCESS) \
                epon_env_error("SC=>ONU CMD FAILED 0x%x\n", i);
#else
#define CHECKONUCMDRESULT(a, b, c, d, e, f, g, h, i)
#endif

#endif
#endif
