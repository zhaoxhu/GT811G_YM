#ifndef _API_TMP_H_
#define _API_TMP_H_

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

typedef struct {
        cs_uint32 holdover_state;
        cs_uint32 holdover_time;
} __attribute__((packed))  epon_onu_ctc_holdover_t;

typedef struct {
        cs_boolean enable;
} __attribute__((packed))  epon_onu_ctc_eth_pause_t;

 typedef struct { 
    unsigned char   ipv6_supported;
    unsigned char   onu_pw_ctrl;
 } __attribute__((packed))  epon_onu_ctc_chip_cap3_t;

 typedef struct { 
    unsigned char   maxgrp;
 } __attribute__((packed))  epon_onu_ctc_grp_nummax_t;

#define EPON_CTC_VENDER_ID_LEN               4
#define EPON_CTC_MODEL_ID_LEN                4
#define EPON_CTC_ONU_ID_LEN                  6
#define EPON_CTC_ONU_HW_LEN                  8
#define EPON_CTC_ONU_SW_LEN                  16
 typedef struct { 
        cs_uint8 venderId[EPON_CTC_VENDER_ID_LEN];
        cs_uint8 model[EPON_CTC_MODEL_ID_LEN];
        cs_uint8 ONUId[EPON_CTC_ONU_ID_LEN];
        cs_uint8 hwVer[EPON_CTC_ONU_HW_LEN];
        cs_uint8 swVer[EPON_CTC_ONU_SW_LEN];
 } __attribute__((packed))  epon_onu_ctc_sn_t;

 typedef struct { 
    unsigned char   fmVersion[32];
 } __attribute__((packed))  epon_onu_ctc_frm_ver_t;

 typedef struct { 
    unsigned char   venderId[2];
    unsigned char   chipModel[2];
    unsigned char   revision;
    unsigned char   date[3];
 } __attribute__((packed))  epon_onu_ctc_chip_id_t;

/* CTC_OAM_CODE_GEN_INSERT_TAG_STRUCT, Don't remove it */

epon_return_code_t epon_request_iros_init ();
epon_return_code_t epon_request_iros_init2 (char *ifName);


epon_return_code_t epon_request_event_register (
                CS_IN epon_callback_common_t          cbfunc,
                CS_IN cs_callback_context_t         context,
                CS_IN epon_event_type_t               event,
                CS_IN epon_event_handler_t            handler,
                CS_IN epon_event_context_t            event_context);

epon_return_code_t epon_request_iros_enable (
                CS_IN epon_callback_common_t          cbfunc,
                CS_IN cs_callback_context_t         context,
                CS_IN epon_iros_config_t              *config);

epon_return_code_t epon_request_iros_disable (
                CS_IN epon_callback_common_t          cbfunc,
                CS_IN cs_callback_context_t         context);

epon_return_code_t epon_request_onu_ctc_holdover_get (
    CS_IN   cs_callback_context_t  context,
    CS_IN   epon_onu_device_id_t       device,
    CS_OUT epon_onu_ctc_holdover_t  *cfg);

epon_return_code_t epon_request_onu_ctc_holdover_set (
    CS_IN   cs_callback_context_t  context,
    CS_IN   epon_onu_device_id_t       device,
    CS_IN   epon_onu_ctc_holdover_t  *cfg);

epon_return_code_t epon_request_onu_ctc_eth_pause_get (
    CS_IN   cs_callback_context_t  context,
    CS_IN   epon_onu_device_id_t       device,
    CS_IN   cs_port_id_t     port_id,
    CS_OUT epon_onu_ctc_eth_pause_t    *cfg);

epon_return_code_t epon_request_onu_ctc_eth_pause_set (
    CS_IN   cs_callback_context_t  context,
    CS_IN   epon_onu_device_id_t      device,
    CS_IN   cs_port_id_t     port_id,
    CS_IN epon_onu_ctc_eth_pause_t    *cfg);

epon_return_code_t
epon_request_onu_ctc_chip_cap3_get(
               CS_IN cs_callback_context_t context,
               CS_IN epon_onu_device_id_t device,
               CS_OUT epon_onu_ctc_chip_cap3_t *cfg);

epon_return_code_t
epon_request_onu_ctc_grp_nummax_get(
               CS_IN cs_callback_context_t context,
               CS_IN epon_onu_device_id_t device,
               CS_IN cs_port_id_t port_id,
               CS_OUT epon_onu_ctc_grp_nummax_t *cfg);

epon_return_code_t
epon_request_onu_ctc_grp_nummax_set(
               CS_IN cs_callback_context_t context,
               CS_IN epon_onu_device_id_t device,
               CS_IN cs_port_id_t port_id,
               CS_IN epon_onu_ctc_grp_nummax_t *cfg);

epon_return_code_t
epon_request_onu_ctc_sn_get(
               CS_IN cs_callback_context_t context,
               CS_IN epon_onu_device_id_t device,
               CS_OUT epon_onu_ctc_sn_t *cfg);

epon_return_code_t
epon_request_onu_ctc_frm_ver_get(
               CS_IN cs_callback_context_t context,
               CS_IN epon_onu_device_id_t device,
               CS_OUT epon_onu_ctc_frm_ver_t *cfg);

epon_return_code_t
epon_request_onu_ctc_chip_id_get(
               CS_IN cs_callback_context_t context,
               CS_IN epon_onu_device_id_t device,
               CS_OUT epon_onu_ctc_chip_id_t *cfg);

/* CTC_OAM_CODE_GEN_INSERT_TAG_API, Don't remove it */


#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif /* _API_TMP_H_ */

