/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */
#include "iros_config.h"
#include "plat_common.h"
#include "cs_types.h"
#include "frame.h"
#include "packet.h"
#include "event.h"
#include "onu_msg.h"
#include "onu_hello.h"

#include "cs_module.h"
#include "log.h"
#include "event.h"
#include "gpio.h"
#include "mdio.h"
#include "cli.h"
#ifdef HAVE_NTT_OAM
#include "startup_cfg.h"
#include "watchdog.h"
#endif
#include "sdl_api.h"
#ifdef GEESUN_REMOVED 
cs_callback_context_t     config_context;
cs_uint32 key_request_counter = 0;
cs_uint32 key_response_counter = 0;

#ifdef HAVE_CTC_OAM
extern cs_uint8 pdu[EPON_MAX_OLT_OAMPDU_LEN];
extern cs_uint32 oam_pdu_len ;
#endif
#ifdef HAVE_NTT_OAM
#ifdef HAVE_ONU_8021X
extern void ntt_user_pass_updated();
#endif
#endif


void onu_msg_proc_xcmd(epon_onu_xmsg *msg)
{
    epon_ether_frame_t frame;
    epon_onu_xmsg *rsp = (epon_onu_xmsg *)frame.payload;
    epon_result_code_t result = EPON_RESULT_SUCCESS;
    cs_uint32 cmd;
    cs_callback_context_t context;
#ifdef HAVE_CTC_OAM
    epon_onu_encryption_config_t *pEncryptionConfig = NULL;
    //oam_if_t *oam_intf = oam_intf_find(CS_PON_PORT_ID);
#endif
    epon_onu_debug_t *p_debug_set = NULL;

    //do sub-commands here
    cmd = ntohl(msg->command);
    memcpy(rsp, msg, ONUXHEADERSIZE + ntohl(msg->data_size));
    /*memcpy(frame.ether_header.src, onu_node.macaddr, EPON_MACADDR_LEN);*/
    epon_request_onu_pon_mac_addr_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, (cs_mac_t *)frame.ether_header.src);
    memcpy(frame.ether_header.dst, onu_node.olt.macaddr, EPON_MACADDR_LEN);
    frame.ether_header.ethertype = htons(EPON_ETHERTYPE_IROS);
    memcpy(frame.layer3_header.src, frame.ether_header.src, EPON_MACADDR_LEN);
    memcpy(frame.layer3_header.dst, onu_node.sc.macaddr, EPON_MACADDR_LEN);
    frame.layer3_header.layer3type = htons(EPON_LAYER3_TYPE_MSG);

    rsp->msg_type = htonl(EPON_ONU_MSG_XCMD_RSP);
    rsp->result =  (cs_int8)result;
    rsp->req_id = msg->req_id;
    rsp->data_size = msg->data_size;

    diag_printf("cmd = 0x%08x, size = %d\n", cmd, ntohl(msg->data_size));

    switch(cmd)
    {

#ifdef HAVE_NTT_OAM

        case EPON_ONU_NTT_USRID_SET:
            rsp->result = startup_config_write(CFG_ID_8021x_USERNAME, ntohl(msg->data_size), msg->data);
#ifdef HAVE_ONU_8021X
            ntt_user_pass_updated();
#endif
            break;

        case EPON_ONU_NTT_USRID_GET:
            rsp->result = startup_config_read(CFG_ID_8021x_USERNAME, msg->data[0], rsp->data);
            rsp->data_size = msg->data[0];
            rsp->data_size = htonl(rsp->data_size);
            break;

        case EPON_ONU_NTT_PWD_SET:
            rsp->result = startup_config_write(CFG_ID_8021x_PWD, ntohl(msg->data_size), msg->data);
#ifdef HAVE_ONU_8021X	
            ntt_user_pass_updated();
#endif
            break;

        case EPON_ONU_NTT_PWD_GET:
            rsp->result = startup_config_read(CFG_ID_8021x_PWD, msg->data[0], rsp->data);
            rsp->data_size = msg->data[0];
            rsp->data_size = htonl(rsp->data_size);
            break;

        case EPON_ONU_NTT_WTD_TMR_SET:
            {
                cs_uint32 tmr;
                memcpy(&tmr, msg->data, 4);
                tmr = ntohl(tmr);
                if(tmr < 1 || tmr > 3600)
                    rsp->result = EPON_RETURN_INV_PARAM;
                else
                {
                    watchdog_set_time(tmr);
                    rsp->result = EPON_RETURN_SUCCESS;
                }
            }
            break;

        case EPON_ONU_NTT_WTD_TMR_GET:
            {
                cs_uint32 *value = (cs_uint32 *)rsp->data;
                cs_uint32 tmr = 0;
                watchdog_get_time(&tmr);
                rsp->result = EPON_RETURN_SUCCESS;
                rsp->data_size = htonl(sizeof(cs_uint32));
                memcpy(value, &tmr, 4);
            }
            break;

#endif
#ifdef HAVE_CTC_OAM
        /*case EPON_CTC_ONU_OOB_CMD:
        {
            oam_intf = oam_intf_find((cs_uint32)(msg->context.apps_data));
            oam_intf->oam_src = OAM_SOURCE_SC_OOB;
            if(EPON_PORTID_TO_TYPE(oam_intf->port)!=EPON_PORT_LLID)
                oam_intf->port = 0x60000000;
            oam_ind_oami(oam_intf, (oam_pdu_hdr_t*)(msg->data), ntohl(msg->data_size));
            if(0 == oam_pdu_len)
            {
                return;
            }
            rsp->result = EPON_RESULT_SUCCESS;
            rsp->data_size = htonl(oam_pdu_len);
            memcpy(rsp->data, pdu, oam_pdu_len);
            break;
        }*/
        case EPON_ONU_ENCRYP_SET:
            pEncryptionConfig = (epon_onu_encryption_config_t *)msg->data;
            pEncryptionConfig->mode = ntohl(pEncryptionConfig->mode);
            rsp->result = epon_request_onu_enc_mode_set(context, ONU_DEVICEID_FOR_API, 
				ONU_LLIDPORT_FOR_API, pEncryptionConfig->mode,pEncryptionConfig->encryption_enable);

            if(msg->cb)
            {
                msg->cb(msg->context, EPON_ONU_ENCRYP_SET,rsp->result, msg->data,msg->data_size);
            }
            break;

        case EPON_ONU_ENCRYP_GET:
            pEncryptionConfig = (epon_onu_encryption_config_t *)rsp->data;
            rsp->result = epon_request_onu_enc_mode_get(context,ONU_DEVICEID_FOR_API, 
				ONU_LLIDPORT_FOR_API,(cs_int8 *)&pEncryptionConfig->mode,(cs_boolean *)&pEncryptionConfig->encryption_enable);

            pEncryptionConfig->mode = htonl(pEncryptionConfig->mode);
            rsp->data_size = htonl(sizeof(epon_onu_encryption_config_t));
            if(msg->cb)
            {
                msg->cb(msg->context, EPON_ONU_ENCRYP_GET,rsp->result, msg->data,rsp->data_size);
            }
            break;
#endif
#ifdef HAVE_OOB_MGMT
        case EPON_ONU_DEBUG_SET:
            p_debug_set = (epon_onu_debug_t *)msg->data;
            p_debug_set->test_num = ntohl(p_debug_set->test_num);
            p_debug_set->para1 = ntohl(p_debug_set->para1);
            p_debug_set->para2 = ntohl(p_debug_set->para2);
            p_debug_set->para3 = ntohl(p_debug_set->para3);
            p_debug_set->para4 = ntohl(p_debug_set->para4);
            p_debug_set->para5 = ntohl(p_debug_set->para5);
            p_debug_set->para6 = ntohl(p_debug_set->para6);
            
            do_asic_set(p_debug_set->test_num, p_debug_set->para1, p_debug_set->para2, 
            p_debug_set->para3, p_debug_set->para4, p_debug_set->para5, p_debug_set->para6);
            
            if(msg->cb)
            {
                msg->cb(msg->context, EPON_ONU_DEBUG_SET, 0, msg->data,msg->data_size);
            }
            break;
        case EPON_ONU_DEBUG_GET:
            {
                
            do_asic_get();
            
            }
            break;
#endif
        default:
            //diag_printf("UNK cmd 0x%x\n", cmd);
            rsp->result= EPON_RESULT_DEVICE_NOT_SUPPORTED;
            break;
    }

    if(!msg->cb)
    {
        #ifdef HAVE_OOB_MGMT
            EPON_ONU_SET_CTRL_DEST((unsigned char *)&frame, EPON_FRAME_HEADER_LEN + ONUXHEADERSIZE + ntohl(rsp->data_size));
        #else
            epon_request_onu_frame_send(config_context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, (char *)&frame, EPON_FRAME_HEADER_LEN + ONUXHEADERSIZE + ntohl(rsp->data_size), CS_PON_PORT_ID, ONU_CPU_PKT_PRIO_6, 0);
        #endif

    }

}

#ifdef HAVE_CTC_OAM
void onu_msg_proc_uc_key_req(epon_pon_itf_uc_key_req_t *msg)
{
    epon_ether_frame_t frame;
    epon_pon_itf_uc_key_rsp_t *key_rsp;
    cs_uint8 mode;
    cs_boolean enable;
    cs_uint32 result = 0;
    cs_uint32 len;
    cs_uint8 key_index;
    cs_uint32 key[4];
    cs_callback_context_t context;
    cs_uint8 macaddr[EPON_MACADDR_LEN];
    
    epon_request_onu_enc_mode_get(context, 
        ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, &mode, &enable);
    if (mode != ntohl(msg->mode)) {
        mode = ntohl(msg->mode);
        epon_request_onu_enc_mode_set(context, 
        ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, mode, enable);
    }

    IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ,IROS_MID_OAM,"received encryption mode = %d, cipher counter = %d, key index = %d\n", ntohl(msg->mode), ntohs(msg->cipher_counter), msg->key_index);

    if (msg->update_cipher_counter){
	  epon_request_onu_enc_cnt_set(context, 
	  ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, ntohs(msg->cipher_counter) );
    }
	
    /*startup_config_read(CFG_ID_MAC_ADDRESS, EPON_MACADDR_LEN, macaddr);*/
    epon_request_onu_pon_mac_addr_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, (cs_mac_t *)macaddr);
    memcpy(frame.ether_header.src, macaddr, EPON_MACADDR_LEN);
    memcpy(frame.ether_header.dst, epon_macaddr_all_olt, EPON_MACADDR_LEN);
    //memcpy(frame.ether_header.src, onu_node.macaddr, EPON_MACADDR_LEN);
    //memcpy(frame.ether_header.dst, onu_node.olt.macaddr, EPON_MACADDR_LEN);
    frame.ether_header.ethertype = htons(EPON_ETHERTYPE_IROS);
	
    memcpy(frame.layer3_header.src, macaddr, EPON_MACADDR_LEN);
    memcpy(frame.layer3_header.dst, epon_macaddr_all_olt, EPON_MACADDR_LEN);
	
    //memcpy(frame.layer3_header.src, onu_node.macaddr, EPON_MACADDR_LEN);
    //memcpy(frame.layer3_header.dst, onu_node.olt.macaddr, EPON_MACADDR_LEN);
    frame.layer3_header.layer3type = htons(EPON_LAYER3_TYPE_MSG);

    key_rsp = (epon_pon_itf_uc_key_rsp_t *)frame.payload;
    key_rsp->msg_type = htonl(EPON_PON_MSG_UC_KEY_RSP);

    memcpy(key_rsp->device, macaddr, EPON_MACADDR_LEN);
    //memcpy(key_rsp->device, onu_node.macaddr, EPON_MACADDR_LEN);
    
    key_index = msg->key_index;

    if (result == 0){
        key_rsp->result = epon_request_onu_enc_key_get(context, 
        ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API,key_index, key);
    }
    else{
        key_rsp->result = result;
    }

    memcpy(key_rsp->key, &key, 4*sizeof(cs_uint32));
    
    IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ,IROS_MID_OAM,"Key from AAL key= 0x%x.0x%x.0x%x.0x%x \n",
            key_rsp->key[0],
            key_rsp->key[1],
            key_rsp->key[2],
            key_rsp->key[3]);

    DUMPPKT("snt pkt", (unsigned char *) &frame,
            (EPON_FRAME_HEADER_LEN + sizeof(epon_pon_itf_uc_key_rsp_t)));
    
    len = EPON_FRAME_HEADER_LEN +
            sizeof(epon_pon_itf_uc_key_rsp_t);
    
    len = len < 60? 60:len;
    
    result = epon_request_onu_frame_send(context, 
        ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, (char *)&frame,
        len, CS_PON_PORT_ID, ONU_CPU_PKT_PRIO);
    if(result != CS_OK)
        {
        IROS_LOG_MSG(IROS_LOG_LEVEL_MAJ,IROS_MID_OAM,"Send failed \n");
        }
    key_response_counter ++;
}
#endif
#endif

#ifdef GEESUN_REMOVED 
void onu_build_reponse (epon_ether_frame_t *frame, int len, epon_msg_type_t msg_type,
                   epon_req_id_t req_id, epon_result_code_t result)
{
    epon_onu_msg_common_rsp_t *rsp =
        (epon_onu_msg_common_rsp_t *)&frame->payload[0];
    cs_callback_context_t     context;

    /*memcpy(frame->ether_header.src, onu_node.macaddr, EPON_MACADDR_LEN);*/
    epon_request_onu_pon_mac_addr_get(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, (cs_mac_t *)frame->ether_header.src);
    memcpy(frame->ether_header.dst, onu_node.olt.macaddr, EPON_MACADDR_LEN);
    frame->ether_header.ethertype = htons(EPON_ETHERTYPE_IROS);
    memcpy(frame->layer3_header.src, frame->ether_header.src, EPON_MACADDR_LEN);
    memcpy(frame->layer3_header.dst, onu_node.sc.macaddr, EPON_MACADDR_LEN);
    frame->layer3_header.layer3type = htons(EPON_LAYER3_TYPE_MSG);
    rsp = (epon_onu_msg_common_rsp_t *)&frame->payload[0];
    rsp->msg_type = htonl(msg_type);
    rsp->req_id = req_id;
    rsp->code = htonl(result);

#ifdef HAVE_OOB_MGMT
    setControlPktL2Dest((unsigned char *)frame, len);
#else
    epon_request_onu_frame_send(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, (char *)frame, len, CS_PON_PORT_ID, ONU_CPU_PKT_PRIO_6, 0);
#endif

}
#endif
