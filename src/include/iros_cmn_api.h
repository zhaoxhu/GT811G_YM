#ifndef __IROS_CMN_API_H__
#define __IROS_CMN_API_H__

#if 0
typedef enum {
    EPON_PORT_OPER_DOWN = 0,
    EPON_PORT_OPER_UP,
} epon_port_oper_status_t;


typedef enum {
    EPON_ONU_SPECIAL_PACKET_PASS = 1,
    EPON_ONU_SPECIAL_PACKET_DROP = 2,
    EPON_ONU_SPECIAL_PACKET_CPU = 3,
    EPON_ONU_SPECIAL_PACKET_SC = 4,
    EPON_ONU_SPECIAL_PACKET_NONE = 5
}epon_onu_special_packet_action_t;


/* bit mask for STP bridge config*/
#define EPON_MASK_STP_BRIDGE_ENABLE                     0x00000001
#define EPON_MASK_STP_BRIDGE_PRIORITY               0x00000002
#define EPON_MASK_STP_BRIDGE_MAX_AGE                0x00000004
#define EPON_MASK_STP_BRIDGE_HELLO_TIME                0x00000008
#define EPON_MASK_STP_BRIDGE_FORWARD_DELAY      0x00000010
#define EPON_MASK_STP_BRIDGE_FORCE_VERSION      0x00000020
#define EPON_MASK_STP_BRIDGE_HOLD_TIME              0x00000040

#define EPON_MASK_STP_BRIDGE_ALL                0x7f
/* bit mask for port STP config*/
#define EPON_MASK_PORT_STP_PRIORITY                     0x00000001
#define EPON_MASK_PORT_STP_PATH_COST                0x00000002
#define EPON_MASK_PORT_STP_POINT2POINT          0x00000004
#define EPON_MASK_PORT_STP_EDGE                         0x00000008
#define EPON_MASK_PORT_STP_NON_STP              0x00000010

#define EPON_MASK_PORT_STP_ALL                      0x0000001f


/* SET */
typedef struct {
    epon_mask_t          mask;
    cs_uint8         enable;
    cs_uint32        bridge_priority;
    cs_uint32        max_age;
    cs_uint32        hello_time;
    cs_uint32        forward_delay;
    cs_int32          force_version;
    cs_uint32        hold_time;
} __attribute__((packed)) epon_onu_stp_config_t;


/* GET RSP */
typedef struct {
    epon_mask_t          mask;
    cs_uint8         enable;
    cs_uint32        bridge_priority;
    cs_uint32        max_age;
    cs_uint32        hello_time;
    cs_uint32        forward_delay;
    cs_int32          force_version;
    cs_uint32        hold_time;
} __attribute__((packed)) epon_onu_stp_get_rsp_t;

/* SET */
typedef struct {
    epon_mask_t         mask;
    cs_uint8        stp_enable;
    cs_uint8        stp_mode;
    cs_uint16       bridge_prio;
    cs_uint8        bridge_addr[6];
    cs_uint16       root_bridge_prio;
    cs_uint8        root_bridge_addr[6];
    cs_uint32       max_age;
    cs_uint32       hello_time;
    cs_uint32       forward_delay;
    cs_uint64       root_path_cost;
    cs_uint16       root_port;
} __attribute__((packed)) epon_onu_stp_state_config_t;


/* GET RSP */
typedef struct {
    epon_mask_t         mask;
    cs_uint8        stp_enable;
    cs_uint8        stp_mode;
    cs_uint16       bridge_prio;
    cs_uint8        bridge_addr[6];
    cs_uint16       root_bridge_prio;
    cs_uint8        root_bridge_addr[6];
    cs_uint32       max_age;
    cs_uint32       hello_time;
    cs_uint32       forward_delay;
    cs_uint64       root_path_cost;
    cs_uint16       root_port;
} __attribute__((packed)) epon_onu_stp_state_get_rsp_t;

/* SET */
typedef struct {
    cs_uint32        port;
    cs_uint32        mask;
    cs_uint32        port_priority;
    cs_uint64        admin_port_path_cost;
    cs_uint8         admin_point2point;
    cs_uint8         admin_edge;
    cs_uint8         admin_non_stp;
} __attribute__((packed)) epon_onu_port_stp_config_t;

/* GET */
typedef struct {
    cs_uint32           port;
} __attribute__((packed)) epon_onu_port_stp_get_t;

/* GET RSP */
typedef struct {
    cs_uint32        mask;
    cs_uint32        port_priority;
    cs_uint64        admin_port_path_cost;
    cs_uint8         admin_point2point;
    cs_uint8         admin_edge;
    cs_uint8         admin_non_stp;
} __attribute__((packed)) epon_onu_port_stp_get_rsp_t;

/* SET */
typedef struct {
    cs_uint8       state;
    cs_uint32      stp_enable;
    cs_uint8       role;
    cs_uint64      path_cost;
    cs_uint32      port_prio;
    cs_uint16      designated_bridge_prio;
    cs_uint8       designated_bridge_addr[6];
    cs_uint8       oper_point2point;
    cs_uint8       oper_edge;
    cs_uint64      rx_cfg_bpdu_cnt;
    cs_uint64      rx_rstp_bpdu_cnt;
    cs_uint64      rx_tcn_bpdu_cnt;
    cs_uint64      tx_cfg_bpdu_cnt;
    cs_uint64      tx_rstp_bpdu_cnt;
    cs_uint64      tx_tcn_bpdu_cnt;
} __attribute__((packed)) epon_onu_port_stp_state_config_t;

/* GET */
typedef struct {
    cs_uint32           port;
} __attribute__((packed)) epon_onu_port_stp_state_get_t;

/* GET RSP */
typedef struct {
    cs_uint8       state;
    cs_uint32      stp_enable;
    cs_uint8       role;
    cs_uint64      path_cost;
    cs_uint32      port_prio;
    cs_uint16      designated_bridge_prio;
    cs_uint8       designated_bridge_addr[6];
    cs_uint8       oper_point2point;
    cs_uint8       oper_edge;
    cs_uint64      rx_cfg_bpdu_cnt;
    cs_uint64      rx_rstp_bpdu_cnt;
    cs_uint64      rx_tcn_bpdu_cnt;
    cs_uint64      tx_cfg_bpdu_cnt;
    cs_uint64      tx_rstp_bpdu_cnt;
    cs_uint64      tx_tcn_bpdu_cnt;
} __attribute__((packed)) epon_onu_port_stp_state_get_rsp_t;

typedef enum {
    EPON_FDB_CLEAR_ALL = 1,
    EPON_FDB_CLEAR_UNICAST,
    EPON_FDB_CLEAR_MULTICAST,
    EPON_FDB_CLEAR_DYNAMIC,
    EPON_FDB_CLEAR_STATIC,
    EPON_FDB_CLEAR_PORT,
    EPON_FDB_CLEAR_PORT_DYNAMIC,
    EPON_FDB_CLEAR_SA_BIND,
    EPON_FDB_CLEAR_SA_FILTER
} epon_fdb_clear_option_t;
#endif

#endif
