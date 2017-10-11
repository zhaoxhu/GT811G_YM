/*****************************************************************************/
/* This file contains unpublished documentation and software                 */
/* proprietary to Cortina Systems Incorporated. Any use or disclosure,       */
/* in whole or in part, of the information in this file without a            */
/* written consent of an officer of Cortina Systems Incorporated is          */
/* strictly prohibited.                                                      */
/* Copyright (c) 2002 by Cortina Systems Incorporated.                       */
/*****************************************************************************/
/*
 * interop.h
 */
#ifndef _IROS_API_IOP_H_
#define _IROS_API_IOP_H_

#ifndef HAVE_KT_EXT
#define HAVE_KT_EXT
#endif

extern int isLLIDValid(int llid);

#define EPON_IOP_MAX_RSP_SIZE   256

#define VERIFYONUINDEX              if(isIfUp() == 0) { \
                  imst_errno = EPON_RESULT_CONTROL_IF_DOWN;\
                    return;\
                }\
                if ((index < 0) || (index >= TEST_MAX_ONU) || !test_onu_list[index].valid) {\
                    epon_env_error("invalid onu index 0x%x\n", index);\
                    imst_errno = EPON_RESULT_INV_PARAM; \
                    return;\
                }\
                imst_errno = EPON_RESULT_SUCCESS;

#define VERIFYOLTINDEX    if ((index < 0) || (index >= TEST_MAX_OLT) || !test_olt_list[index].valid) {\
                    epon_env_error("Invalid OLT index\n");\
                    return;\
                }\
                imst_errno = EPON_RESULT_SUCCESS;

#define VERIFYONUPARAM(name, val, min, max) if (val < min || val > max) {\
                epon_env_error("invalid onu parameter %s\n", name);\
                imst_errno = EPON_RESULT_INV_PARAM; \
                return; }\
                imst_errno = EPON_RESULT_SUCCESS;\

#define VERIFYONUPARAM2(name, val, min, max) if (val != min && val != max) {\
                epon_env_error("invalid onu parameter %s\n", name);\
                imst_errno = EPON_RESULT_INV_PARAM; \
                return; }\
                imst_errno = EPON_RESULT_SUCCESS;\

#define VERIFYLLID(llid)        if(!isLLIDValid(llid)) {\
                        epon_env_error("invalid llid 0x%x\n", llid);\
                        imst_errno = EPON_RESULT_INV_PARAM; \
                        return; }


/**************** PMC IOP stuff start ******************************/
#define SET_PMC_OUI(a)      a[0] = 0; a[1] = 0x0c; a[2] = 0xd5;
#define IS_PMC_OUI(a)       if(a[0] == 0 && a[1] == 0x0c && a[2] == 0xd5) return 1; return 0;

typedef struct {
    cs_uint8 oui[3];
    cs_uint8 magic;
    cs_uint32 mid;
    cs_uint16 pdu_type;
    cs_uint32 opcode;
    cs_uint8 data[1];
}pmc_oam_frame_t;

typedef struct {
    cs_uint8 oui[3];
    cs_uint8 magic;
//    cs_uint32 mid;
    cs_uint8 magic1;
    cs_uint8 magic2;
    cs_uint8 magic3;
    cs_uint8 magic4;
//    cs_uint16 pdu_type;
//    cs_uint32 opcode;
    cs_uint8 magic5;
    cs_uint8 magic6;
    cs_uint8 magic8;
    cs_uint8 magic9;
    cs_uint8 magic10;
    cs_uint8 magic11;
    cs_uint8 data[1];
}pmc_oam_frame_t_hw;

#define PMC_PDU_TYPE_REQUEST                0x00
#define PMC_PDU_TYPE_RESPONSE               0x01
#define PMC_PDU_TYPE_EVENT                  0x02
   
#define PMC_OPCODE_ENCRYP_CONFIG_SET        0x00000002
typedef struct {
    cs_uint32 mirror_enable;
    cs_uint32 bypass_enable;
}pmc_oam_encryption_config_set_t;

#define PMC_OPCODE_ENCRYP_CONFIG_ENBALE     0x00000003
typedef struct {
    cs_uint32 type;
}pmc_oam_encryption_config_enable_t;

#define PMC_OPCODE_ENCRYP_CONFIG_GET        0x00000004
#define PMC_OPCODE_ENCRYP_CONFIG_GET_RSP    0x00000004
typedef struct {
    cs_uint32 result;
    cs_uint32 encryption_type;
}pmc_oam_encryption_config_get_rsp_t;

/*INSERT PMC IOP STRUCT */
    
/**************** PMC IOP stuff end ******************************/

/**************** TK IOP stuff start ******************************/
#define SET_TK_OUI(a)       a[0] = 0; a[1] = 0x0d; a[2] = 0xb6;
#define IS_TK_OUI(a)        if(a[0] == 0 && a[1] == 0x0d && a[2] == 0xb6) return 1; return 0;

typedef struct {
    cs_uint16 tk_iop_port_id;
    cs_uint16 tk_iop_lport_id;
    cs_uint16 tk_iop_queue_id;
    cs_uint16 tk_iop_bridge_id;
    cs_uint16 tk_iop_bridge_port_id;
    cs_uint8  tk_iop_opcode;
    cs_uint8 response[EPON_IOP_MAX_RSP_SIZE];
    cs_uint8 rsp_len;
} oam_context_t;

#define ADJUST_RESP_PTR     (oam_context.response + oam_context.rsp_len)

typedef struct {
    cs_uint8 branch;
    cs_uint16 leaf;
    cs_uint8 len;
    cs_uint8 data[1];
}tk_oam_tlv_t;

typedef struct {
    cs_uint8 oui[3];
    cs_uint8 opcode;
    tk_oam_tlv_t oam_tlv;
}tk_oam_frame_t;

#if 0
#define TK_OPCODE_EXT_INFO        0x00000000
#define TK_OPCODE_GET             0x00000001
#define TK_OPCODE_GET_RSP         0x00000002
#define TK_OPCODE_SET             0x00000003
#define TK_OPCODE_SET_RSP         0x00000004
#define TK_OPCODE_MC_REQ          0x00000005
#define TK_OPCODE_MC_REG          0x00000006
#define TK_OPCODE_MC_REG_RSP      0x00000007
#endif

#define TK_OPCODE_EXT_INFO        0x00
#define TK_OPCODE_GET             0x01
#define TK_OPCODE_GET_RSP         0x02
#define TK_OPCODE_SET             0x03
#define TK_OPCODE_SET_RSP         0x04
#define TK_OPCODE_MC_REQ          0x05
#define TK_OPCODE_MC_REG          0x06
#define TK_OPCODE_MC_REG_RSP      0x07

#define TK_OPCODE_KEY_EXCH        0x00000008
#define TK_OPCODE_FILE_READ       0x00000009
#define TK_OPCODE_FILE_WRITE      0x0000000A
#define TK_OPCODE_FILE_DATA       0x0000000B
#define TK_OPCODE_FILE_ACK        0x0000000C

#define TK_OAM_BRANCH_NAME                  0x06
#define TK_OAM_BRANCH_ATTRIB                0x07
#define TK_OAM_BRANCH_ACTION                0x09

#define TK_VAR_RSP_CODE_OK                  0x80
#define TK_VAR_RSP_CODE_TOO_LONG            0x81
#define TK_VAR_RSP_CODE_UNDETERM            0xA0
#define TK_VAR_RSP_CODE_UNSUPPORTED         0xA1
#define TK_VAR_RSP_CODE_CORRUPTED           0xA2
#define TK_VAR_RSP_CODE_HW_FAILED           0xA3
#define TK_VAR_RSP_CODE_OVERFLOW            0xA4
#define TK_VAR_RSP_CODE_BAD_PARAM           0xA8
#define TK_VAR_RSP_CODE_NO_RES              0xA9
#define TK_VAR_RSP_CODE_TO_BE_CONT          0xAA

#define TK_OAM_LEAF_LPORT_OBJ               0x0001
#define TK_OAM_LEAF_PORT_OBJ                0x0003
#define TK_OAM_LEAF_QUEUE_OBJ               0x0004
#define TK_OAM_LEAF_BRIDGE_OBJ              0x0082
#define TK_OAM_LEAF_BRIDGE_PORT_OBJ         0x0083

#define TK_OAM_PORT_COMMAND_DYNC_MAC_GET    0x00A4    
#define TK_CLASSIFIER_MAX_NUM               8

typedef struct {
    cs_uint8 fselect;
    cs_uint8 mac[OAM_MACADDR_LEN];
    cs_uint8 oper;
}tk_oam_classifier_rule_t;


typedef struct {
    cs_uint8 dir;
    cs_uint8 dest[3];
    cs_uint8 prio;
    cs_uint8 action;
    cs_uint8 count;
    tk_oam_classifier_rule_t rules[TK_CLASSIFIER_MAX_NUM];
}tk_oam_classifier_t;


typedef struct {
    cs_uint8 max;   /*min=0, max=25*/
    cs_uint8 min;   
}tk_oam_rate_t;

/*INSERT TK LEAF HERE*/

/**************** TK IOP stuff end ******************************/

/**************** DASAN IOP stuff start ******************************/
#define SET_DASAN_OUI(a)    a[0] = 0; a[1] = 0xd0; a[2] = 0xcb;
#define IS_DASAN_OUI(a)     if(a[0] == 0 && a[1] == 0xd0 && a[2] == 0xcb) return 1; return 0;

typedef struct {
    cs_uint8 oui[3];
    cs_uint16 flag;
    cs_uint16 llid;
    cs_uint32 opcode;
}dasan_oam_frame_t;

#define DASAN_OAM_FLAG_FW    0x0907

typedef struct {
    cs_uint16 len;
    cs_uint8 data[32];
}dasan_oam_fw_get_frame_t;




/**************** DASAN IOP stuff end ******************************/



epon_return_code_t
epon_request_pmc_oam_encryption_config_set(
        CS_IN cs_callback_context_t context,
        CS_IN epon_olt_device_id_t device,
        CS_IN epon_macaddr_t olt_mac,
        CS_IN unsigned int llid,
        CS_IN pmc_oam_encryption_config_set_t *config);

epon_return_code_t
epon_request_tk_oam_rate_set(
        CS_IN cs_callback_context_t context,
        CS_IN epon_olt_device_id_t device,
        CS_IN epon_macaddr_t olt_mac,
        CS_IN unsigned int llid,
        CS_IN tk_oam_rate_t *config);

#endif /* _IROS_API_IOP_H_ */
