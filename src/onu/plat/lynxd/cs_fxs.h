/*
 * notes regarding registering callback to a device event, not
 * a line event.. such as thermal fault..
 * Since the abstraction layer hides the physical device from
 * application, the application does not know which device is
 * encountering error, it does know which line's device is
 * encountering error.  Therefore, when designing callback to
 * handle those device-wise fault, assign to its line.  That
 * would make still work.  The abstraction layer will notify
 * all the callbacks registered to that event.
 */
#ifndef __CS_FXS_H__
#define __CS_FXS_H__

/* KAR */

/* definition for those various types */
#define CS_SLIC_DEV "/dev/cs_slic"
#define CS_FXS_EVENT_DATA_SIZE_MAX  4

#define DMA_HDR_LEN   4
#define MAC_ADD_LEN   12
#define VLAN_HDR_LEN  4
#define ETHERTYPE_LEN 2
#define VOIP_HDR_LEN  54
#define CRC_LEN       4

#define VLAN_TPID1 0x8100
#define VLAN_TPID2 0x88a8
#define VLAN_TPID3 0x9100

#define DEV_ID_NULL     0
#define LLID_NULL       0

#define NUM_DEVICES 1
#define NUM_LINES   2
#define CHANNEL_0   0
#define CHANNEL_1   1
#define CHANNEL_2   2
#define CHANNEL_3   3
 
#define VOIP_THREAD_DELAY 1
#define VOIP_ALARM_TIMER  10
#define VOIP_APP_TIMER    10 // (PCM_SAMPLE_SIZE/8)


#define VOIP_UDP_PORT     0x1234

#define DMA_BUFFER_0        0
#define DMA_BUFFER_1        1
#define DMA_BUFFER_COUNT    2
#define DMA_BUF_LEN         10240
#define RX_SAMPLE_BUF_SEND  8
#define TOKEN_TO_BYTE       4 // 1 TOKEN = 4 BYTE
#define TOTAL_PER_CHANNEL_XRAM_SIZE (PCM_SAMPLE_SIZE*8)

#define BYTES_TO_TOKEN(a)   ((a+3) / 4)
#define TOKEN_TO_BYTES(a)   (a * 4)

#if 0
struct cs_app_fxs_line;
struct cs_fxs_dev;

/* callback profile and its handle definitions */
typedef struct cs_fxs_profile_user_s {
    struct cs_fxs_profile_user_s *next;
    struct cs_app_fxs_line *user;
} cs_fxs_profile_user_t;

typedef struct cs_fxs_callback_profile_s {
    struct cs_fxs_callback_profile_s *next;
    unsigned int id;    /* a unique ID (>= 1) */
    void (*callback)(cs_dev_id_t dev_id, cs_port_id_t port_id,
            cs_slic_event_t event, cs_uint8 *data_ptr,
            cs_uint32 data_len);
    cs_fxs_profile_user_t *user_list;
    /* user_list is to keep the list of user (line) who is
     * currently using this profile.
     * If a profile is used, it is better not to modify the
     * data of this profile */
} cs_fxs_callback_profile_t;

struct cs_fxs_callback
{
    struct cs_fxs_callback *next;
    cs_slic_event_t event;
    cs_fxs_callback_profile_t *profile;
};

typedef struct cs_app_fxs_line_info {
    int dev_id;
    int port_id;
    cs_slic_status_t status;    //?? need it? we are getting the info from HAL already
    cs_slic_state_t state;  //?? need it?
    cs_slic_polarity_t polarity;
    int ac_id;
    int dc_id;
    unsigned int ring_id;
    unsigned int cid1_id;
    unsigned int cid2_id;
    unsigned int meter_id;
    cs_slic_loopback_t loopback_mode;
    cs_slic_event_t support_event;
    cs_slic_event_t handle_event;
} cs_app_fxs_line_info_t;

struct cs_app_fxs_line
{
    int dev_id;
    int port_id;
    cs_slic_status_t status;    //?? need it? we are getting the info from HAL already
    cs_slic_state_t state;  //?? need it?
    cs_slic_polarity_t polarity;
    int ac_id;
    int dc_id;
    unsigned int ring_id;
    unsigned int cid1_id;
    unsigned int cid2_id;
    unsigned int meter_id;
    cs_slic_loopback_t loopback_mode;
    cs_slic_event_t support_event;
    cs_slic_event_t handle_event;
    struct cs_fxs_callback *callback_list;
    struct cs_fxs_dev *dev;
    struct cs_app_fxs_line *next;
};

struct cs_fxs_dev
{
    int dev_id;
    int num_line;
    struct cs_app_fxs_line *line_head;
};

void app_handle_outbound_rtp(cs_uint8 chanid, cs_uint8 *buf);
void app_handle_inbound_rtp(cs_uint8 chanid);

/* module initialization */
cs_status epon_request_onu_cs_fxs_init(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport
);

/***************** INITIALIZATIONS ************************/
/* open FXS device. */
cs_status epon_request_onu_open_dev(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_dev_id_t                 dev_id
);

/* open a line and associates it with a device */
cs_status epon_request_onu_open_line(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_dev_id_t                 dev_id,
    CS_IN cs_port_id_t                port_id
);

/* close FXS device */
cs_status epon_request_onu_close_dev(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_dev_id_t                 dev_id
);

/* close a line of a FXS device */
cs_status epon_request_onu_close_line(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_dev_id_t                 dev_id,
    CS_IN cs_port_id_t                port_id
);

/* reset FXS device */
cs_status epon_request_onu_reset_dev(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_dev_id_t                 dev_id
);

/* reset a line of FXS device */
cs_status epon_request_onu_reset_line(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_dev_id_t                 dev_id,
    CS_IN cs_port_id_t                port_id
);

/* initialize FXS device */
cs_status epon_request_onu_init_dev(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_dev_id_t                 dev_id
);

/* initialize a line of a FXS device */
cs_status epon_request_onu_init_line(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_dev_id_t                 dev_id,
    CS_IN cs_port_id_t                port_id
);

/* get FXS device info */
cs_status epon_request_onu_get_dev_info(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN struct cs_fxs_dev           *dev
);

/* get the info of a line of a FXS device */
cs_status epon_request_onu_get_line_info(
    CS_IN cs_callback_context_t        context,
    CS_IN cs_int32                   device,
    CS_IN cs_int32                   llidport,
    CS_IN cs_dev_id_t                    dev_id,
    CS_IN cs_port_id_t                   port_id,
    CS_IN struct cs_app_fxs_line         *line
);

/* read in the ring/tone/meter/cid profile configuration file and parse
 * the information into a table. */
cs_status epon_request_onu_read_conf(char *conf_name);

/***************** CONFIGURATIONS ************************/
/* set PCM codec for SLIC line */
cs_status epon_request_onu_set_pcm_codec(
    CS_IN cs_callback_context_t        context,
    CS_IN cs_int32                   device,
    CS_IN cs_int32                   llidport,
    CS_IN cs_dev_id_t                    dev_id,
    CS_IN cs_port_id_t                   port_id,
    CS_IN cs_slic_pcm_codec_t            codec);

/* get PCM codec for SLIC line */
cs_status epon_request_onu_get_pcm_codec(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_slic_pcm_codec_t                *codec_ptr);

/* set the tone to the specific line */
cs_status epon_request_onu_set_line_tone(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_slic_tone_t                     tone_type,
    CS_IN cs_uint16                          numSegs,
    CS_IN cs_slic_tone_seg_t                 *tone_seg);

/* set gain level on TX path of SLIC line */
cs_status epon_request_onu_set_tx_gain_level(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_uint16                          level);

/* set gain level on RX path of SLIC line */
cs_status epon_request_onu_set_rx_gain_level(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_uint16                          level);

/* assigned a ring profile (with a ring profile ID) to the specific line */
cs_status epon_request_onu_set_line_ring(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_slic_ring_code_t                ring_id,
    CS_IN cs_slic_ring_cfg_t                 *ring_cfg);

/* set loopback */
cs_status epon_request_onu_set_line_loopback(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_slic_loopback_t                 option);

/* reset loopback */
cs_status epon_request_onu_reset_line_loopback(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id);

/* get loopback status*/
cs_status epon_request_onu_get_line_loopback(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_slic_loopback_t                 *option);

/* assigned a caller ID profile to the specific line */
cs_status epon_request_onu_set_line_cid(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_slic_cid_std_t                  cid_id,
    CS_IN cs_slic_cid_t                      type,
    CS_IN cs_slic_cid_cfg_t                  *cid_cfg);

/* assigned a meter profile to the specific line */
cs_status epon_request_onu_set_line_meter(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_uint32                          meter_id,
    CS_IN cs_slic_meter_cfg_t                *meter_cfg);

cs_status epon_request_onu_set_flashhook_timeout(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_uint16                          min,
    CS_IN cs_uint16                          max);

/********************* STATES ****************************/
/* get hook status of FXS line */
cs_status epon_request_onu_get_hook_status(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_slic_status_t                   *result_ptr);

/* get the complete FXS line status */
cs_status epon_request_onu_get_line_status(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_slic_status_t                   *result_ptr);

/* get the complete FXS line state */
cs_status epon_request_onu_get_line_state(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_slic_state_t                    *result_ptr);

/* set the complete FXS line state */
cs_status epon_request_onu_set_line_state(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_slic_state_t                    state);

/* set FXS line to NORMAL polarity */
cs_status epon_request_onu_set_line_normal(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id);

/* set FXS line to REVERSE polarity */
cs_status epon_request_onu_set_line_reverse(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id);

/* set FXS line to on-hook transmission */
cs_status epon_request_onu_set_onhook_txn(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id);

/* get the last digit when pulse digit event occur */
cs_status epon_request_onu_get_event_data(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_slic_event_t                    event,
    CS_IN cs_uint32                          *data_ptr,
    CS_IN cs_uint32                          *data_len);

/* register the callback function for FXS line event(s) */
cs_status epon_request_onu_register_event_callback(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN void                               *callback,
    CS_IN cs_slic_event_t                    event);

/* unregister the callback function for FXS line event(s) */
cs_status epon_request_onu_unregister_event_callback(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN void                               *callback,
    CS_IN cs_slic_event_t                    event);

/******************** CONTROLS  ***************************/
/* FXS device / line ring start */
cs_status epon_request_onu_ring_start(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id
);

/* FXS device / line ring stop */
cs_status epon_request_onu_ring_stop(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id
);

/* initialize Caller ID data before start ringing on FXS line
 * A must if caller ID sequence is enabled */
cs_status epon_request_onu_init_cid_data(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_uint32                          length,
    CS_IN cs_uint8                           *cid_data);

/* start a called ID sequence for a FXS line without waiting for a ring state
 * It's used for type 2 caller ID transmission. (profile should've been set
 * before calling this function or it won't be sent). */
cs_status epon_request_onu_send_line_cid(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_uint32                          length,
    CS_IN cs_uint8                           *cid_data);

/* FXS line meter start */
cs_status epon_request_onu_line_meter_start(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_uint32                          min_delay,
    CS_IN cs_uint32                          onTime,
    CS_IN cs_uint32                          offTime,
    CS_IN cs_uint16                          numMeters);

/* FXS line meter stop */
cs_status epon_request_onu_line_meter_stop(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id
);

/* enable/disable PCM TX/RX pathes of SLIC line, when in line states of
 * CS_SLIC_STATE_TALK or CS_SLIC_STATE_OHT (On-Hook Talk) */
cs_status epon_request_onu_fxslic_PCM_TXRX_ctrl(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport,
    CS_IN cs_dev_id_t                        dev_id,
    CS_IN cs_port_id_t                       port_id,
    CS_IN cs_slic_pcm_ctrl_t                 ctrl_opt);

/* event polling function which user has to call */
cs_status epon_request_onu_fxs_poll_event(
    CS_IN cs_callback_context_t            context,
    CS_IN cs_int32                       device,
    CS_IN cs_int32                       llidport
);

/*
 * To control this SLIC HAL, we use different states to determine what
 * the driver should be doing at certain stage.  This HAL is based on
 * Zarlink / Legerity API, so the state works somehow similar to them.
 * There are several possible state changes as following:
 * 1) Standby -> Ringing -> Talk / OHT (On-Hook Talk) -> Standby
 * 2) Standby -> Active -> Talk / OHT -> Standby
 * 3) Disconnect -> Standby
 * 4) Standby -> Ringing -> Standby
 * 5) Standby -> Active -> Standby
 *
 * At the state Talk and OHT, PCM will interact with SLIC to send and
 * receive data.  To control mute and such, please use cs_fxs_PCM_TXRX_ctrl.
 */
/*
 * Profile handling!!
 *
 */
 cs_status epon_request_onu_pcm_init (
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport
);

cs_status epon_request_onu_is_pcm_channel_rx_enable(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_int32                ch_id,
    CS_IN cs_int8                 *enable
);

cs_status epon_request_onu_is_pcm_channel_tx_enable(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_int32                ch_id,
    CS_IN cs_int8                 *enable
);

cs_status epon_request_onu_pcm_tx(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_int32                ch_id,
    CS_IN cs_int8                 *buf
);

cs_status epon_request_onu_pcm_channel_tx_enable(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_int32                ch_id,
    CS_IN cs_int8                 enable
);

cs_status epon_request_onu_pcm_channel_rx_enable(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_int32                ch_id,
    CS_IN cs_int8                 enable
);

cs_status epon_request_onu_l4_port_rule_set(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint16               l4port,
    CS_IN cs_uint8                *p_rule_id
);

cs_status epon_request_onu_l4_prot_rule_enable(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint8                rule_id,
    CS_IN cs_uint8                enable
);

cs_status epon_request_onu_esdma_setup(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint8                *p_dma_buffer1,
    CS_IN cs_uint32               sizeof_dma_buffer1,
    CS_IN cs_uint8                *p_dma_buffer2,
    CS_IN cs_uint32               sizeof_dma_buffer2
);

cs_status epon_request_onu_pon_dma_receive(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint8                *p_receive_buffer,
    CS_IN cs_uint32               *p_sizeof_receive_buffer
);

cs_status epon_request_onu_pon_dma_send(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint8                *p_source_buffer,
    CS_IN cs_uint32               sizeof_source_buffer
);

cs_status epon_request_onu_dma_get_token(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint8                    *p_current_read_buffer_idx,
    CS_IN cs_uint32                   *p_token_used
);

cs_status epon_request_onu_set_rtp_data_ctrl(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint8                session_id,
    CS_IN cs_uint16              l4_port,
    CS_IN cs_uint32                ctrl
);

cs_status epon_request_onu_get_rtp_data_ctrl(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_uint8                session_id,
    CS_OUT cs_uint16              *l4_port,
    CS_OUT cs_uint32                *ctrl
);

cs_status epon_request_onu_get_pcm_sample(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_int32                ch_id,
    CS_IN cs_int8                 *buf
);

cs_status epon_request_onu_set_pcm_sample(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_int32                ch_id,
    CS_IN cs_int8                 *dma,
    CS_IN cs_int32                offset
);


cs_status epon_request_onu_g711_alawToLinear(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_int8                 alaw[],
    CS_IN cs_int16                linear[],
    CS_IN cs_int16                length
);

cs_status epon_request_onu_g711_linearToAlaw(
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_int16                linear[],
    CS_IN cs_int8                 alaw[],
    CS_IN cs_int16                length
);

cs_status epon_request_onu_ec_push_data_to_fifo( 
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_int8                 chanid,
    CS_IN cs_int8                 *buf
);

cs_status epon_request_onu_ec_ec_pop_data_from_fifo( 
    CS_IN cs_callback_context_t     context,
    CS_IN cs_int32                device,
    CS_IN cs_int32                llidport,
    CS_IN cs_int8                 chanid,
    CS_IN cs_int8                 *buf
);
#endif

#endif /* __CS_FXS_H__ */

