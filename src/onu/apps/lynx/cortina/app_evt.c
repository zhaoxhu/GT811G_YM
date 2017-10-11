#include "iros_config.h"
#include "plat_common.h"
#include "packet.h"
#include "cs_msg.h"
#include "cs_event.h"
#include "event.h"
#include "cs_hello.h"
#include "onu_hello.h"
#include "sdl_api.h"
#include "app_oam_cs.h"
#include "app_gpio.h"
#include "sdl.h"
#ifdef HAVE_NTT_OAM
#include "app_oam_ntt.h"
#endif
#ifdef HAVE_CTC_OAM
#include "oam_ctc_client.h"
#endif
#ifdef HAVE_EVENT_SHOW_CMD
#include "../../gwd/gwd_log.h"
#endif

onu_events_t onu_events[EPON_EVENT_TYPE_MAX];

cs_int32 onu_evt_reg(cs_int32 evt_type , void *func , void *cookie)
{
    cs_int32 i;
    if (func == NULL)
        return CS_E_ERROR;

    for(i = 0 ; i < MAX_HANDLER_NUM ; i++)
    {
        if(onu_events[evt_type].handler[i] == func)
            return CS_E_OK;

        if(onu_events[evt_type].handler[i] == NULL)
            break;
    }

    if(i >= MAX_HANDLER_NUM)
        return CS_E_ERROR;
    
    onu_events[evt_type].handler[i] = func;

    return CS_E_OK;
}

cs_status event_detect(cs_int32 type, cs_uint16 msg_len, void* msg)
{
    onu_event_msg_t *pMsg = NULL;
    cs_int32 ret;

    pMsg = (onu_event_msg_t *)iros_malloc(IROS_MID_EVENT, sizeof(onu_event_msg_t) - 1 + msg_len);
    if (!pMsg) {
        diag_printf("event_detect iros_malloc failed.\n");
        return CS_E_ERROR;
    }

    pMsg->type = IROS_MSG_EVENT;
    pMsg->evt.type = type;
    pMsg->evt.relay = 0;
    pMsg->evt.msg_len = msg_len;
    memcpy(pMsg->evt.data , msg , msg_len);
    IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3 , IROS_MID_EVENT , "detect %s event\n",
            (type == EPON_EVENT_PORT_LINK_CHANGE)?"link":
            (type == EPON_EVENT_REG_CHANGE)?"reg_change":
            (type == EPON_EVENT_BM_CRC)?"BM crc error":
            (type == EPON_EVENT_ENC_ENCRYPTED)?"DS encrypted-frame-recv":"unkonwn");

    if(type == EPON_EVENT_BM_CRC)
    {
        IROS_LOG_CRI(IROS_MID_EVENT, "detect BM CRC error\n");
    }
    ret = cs_pri_queue_put(app_msg_q , (void *) & pMsg, sizeof(cs_uint32), CS_OSAL_NO_WAIT, APP_QUEUE_PRI_3);
    if (ret) {
        IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3, IROS_MID_EVENT, "event_detect cs_queue_put failed %d\n", ret);
        iros_free(pMsg);
        return CS_E_ERROR;
    }

    return CS_E_OK;
}

void event_send_sc(cs_int32 type, cs_uint16 msg_len, void* msg)
{
    onu_event_msg_t *pMsg = NULL;
    cs_int32 ret;

    pMsg = (onu_event_msg_t *)iros_malloc(IROS_MID_EVENT, sizeof(onu_event_msg_t) - 1 + msg_len);
    if (!pMsg) {
        diag_printf("event_detect iros_malloc failed.\n");
        return;
    }

    pMsg->type = IROS_MSG_EVENT;
    pMsg->evt.type = type;
    pMsg->evt.relay = 1;
    pMsg->evt.msg_len = msg_len;
    memcpy(pMsg->evt.data , msg , msg_len);
    IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3 , IROS_MID_EVENT , "detect %s event\n",
            (type == EPON_EVENT_PORT_LINK_CHANGE)?"link":
            (type == EPON_EVENT_REG_CHANGE)?"reg_change":
            (type == EPON_EVENT_BM_CRC)?"BM crc error":
            (type == EPON_EVENT_ENC_ENCRYPTED)?"DS encrypted-frame-recv":"unkonwn");

    if(type == EPON_EVENT_BM_CRC)
    {
        IROS_LOG_CRI(IROS_MID_EVENT, "detect BM CRC error\n");
    }
    ret = cs_pri_queue_put(app_msg_q , (void *) & pMsg, sizeof(cs_uint32), CS_OSAL_NO_WAIT, APP_QUEUE_PRI_3);
    if (ret) {
        IROS_LOG_MSG(IROS_LOG_LEVEL_DBG3, IROS_MID_EVENT, "event_detect cs_queue_put failed %d\n", ret);
        iros_free(pMsg);
        return;
    }

    return;
}


void event_test()
{
    int i;

    for(i = 0; i < 20 ; i++)
    {
        event_send_sc(i, 4, &i);
    }
}

#ifdef HAVE_KT_OAM
extern void onu_start_detect_pon_act();
extern void onu_stop_detect_pon_act();
#endif
void onu_evt_changed_led_proc(cs_uint16 dev, cs_int32 evt, void *msg)
{
    sdl_event_reg_t *pMsg = (sdl_event_reg_t *)msg;

    if(msg == NULL)
        return;

    if(pMsg->reg)
    {
    #ifdef HAVE_NTT_OAM
        ntt_dl_info_type_t  stats;
        ntt_dl_result_type_t result;
        onu_ntt_upgrade_info_get(&stats, &result);
        if(stats != NTT_DL_RST_WAIT) {
        cs_led_proc(LED_PON, LED_GREEN,LED_ON);
        }
    #elif HAVE_KT_OAM    
        cs_led_proc(LED_PON_LNK, LED_GREEN,LED_ON);
        /*onu_start_detect_pon_act();*/
    #else
        cs_led_proc(LED_PON_LNK, LED_GREEN,LED_ON);
    #endif
    }
    else
    {
    #ifdef HAVE_NTT_OAM
        ntt_dl_info_type_t  stats;
        ntt_dl_result_type_t result;
        onu_ntt_upgrade_info_get(&stats, &result);
        if(stats != NTT_DL_RST_WAIT) {
            cs_led_proc(LED_PON, LED_GREEN,LED_OFF);
        }

        cs_led_proc(LED_AUTH, LED_GREEN,LED_OFF);
    #elif HAVE_KT_OAM    
        cs_led_proc(LED_PON, LED_GREEN,LED_OFF);
        /*onu_stop_detect_pon_act();*/
    #else
        cs_led_proc(LED_PON_LNK, LED_GREEN,LED_OFF);
    #endif    
    }

    return;
}

void onu_link_changed_led_proc(cs_uint16 dev, cs_int32 evt, void *msg)
{
    sdl_event_port_link_t *pMsg = (sdl_event_port_link_t *)msg;

    if(pMsg == NULL)
        return;

    if(pMsg->port == CS_PON_PORT_ID)
    {
        if(pMsg->link)
        {
            cs_uint8 status;
            IROS_LOG_CRI(IROS_MID_EVENT, "PON port link up\n");
            //cs_printf("PON port link up\n");
            #ifdef HAVE_CTC_OAM
            oam_ctc_get_oam_ext_status(&status);
            if(status==OAM_CTC_EXT_ONU_COMPLETED)
                cs_led_proc(LED_PON, LED_GREEN,LED_ON);
            else
                cs_led_proc(LED_PON, LED_GREEN,LED_TWINKLE);
            
            cs_led_los_handle();
            #endif
			
			#ifdef HAVE_EVENT_SHOW_CMD
			gw_syslog(GW_LOG_LEVEL_MAJOR, "PON port link up\n");
			#endif
        }
        else
        {
            IROS_LOG_CRI(IROS_MID_EVENT, "PON port link down\n");
            //cs_printf("PON port link down\n");
            #ifdef HAVE_CTC_OAM
            cs_led_proc(LED_RUN, LED_GREEN, LED_TWINKLE);
            cs_led_proc(LED_PON, LED_GREEN,LED_OFF);
            cs_led_los_handle();
            #endif

			#ifdef HAVE_EVENT_SHOW_CMD
			gw_syslog(GW_LOG_LEVEL_MAJOR, "PON port link down\n");
			#endif
        }
    }

    if(pMsg->port >= CS_UNI_PORT_ID1 && pMsg->port <= UNI_PORT_MAX)
    {
        if(pMsg->link)
        {
            IROS_LOG_CRI(IROS_MID_EVENT, "UNI port-%d link up\n",pMsg->port);
            cs_printf("UNI port-%d link up\n",pMsg->port);

			#ifdef HAVE_EVENT_SHOW_CMD
			gw_syslog(GW_LOG_LEVEL_MAJOR, "UNI port-%d link up\n", pMsg->port);
			#endif
        }
        else
        {
            IROS_LOG_CRI(IROS_MID_EVENT, "UNI port-%d link down\n",pMsg->port);
            cs_printf("UNI port-%d link down\n",pMsg->port);

			#ifdef HAVE_EVENT_SHOW_CMD
			gw_syslog(GW_LOG_LEVEL_MAJOR, "UNI port-%d link down\n", pMsg->port);
			#endif
        }
    }
}


void onu_pon_laseron_proc(cs_uint16 dev, cs_int32 evt, void *msg)
{
    sdl_event_laseron_t *pMsg = (sdl_event_laseron_t *)msg;

    if(pMsg == NULL)
        return;

    if(pMsg->laseron)
    {
        cs_callback_context_t context;
        /*Ensure lao event oam has been sent*/
        cs_thread_delay(500);
        /*Need stop mpcp grand and BM TX scheduler ?*/
        cs_gpio_pon_tx_set(context,0,0,0);
        //cs_printf("Pon TX Detect ON\n");
    }
    else
    {
        //cs_printf("Pon TX Detect OFF\n");
        /*Need start mpcp grand and BM TX scheduler ?*/
    }
    return;
}


#ifdef HAVE_MPCP_DEREGISTER_REST

#ifdef HAVE_RESET_CONTROL
cs_boolean onu_reset_enable_get(void);
#endif

#define DELAY_RESET_TIME	(10*1000)		//unit: millisecond
void onu_reset(void *data)
{
	iros_system_reset(RESET_TYPE_FORCE);
}

void onu_evt_reg_changed_reset_proc(cs_uint16 dev, cs_int32 evt, void *msg)
{
    sdl_event_reg_t *pMsg = (sdl_event_reg_t *)msg;

	#ifdef HAVE_ONU_DELAY_OFFLINE_RESET
	static cs_int32 reg_off_timer = 0;
	#endif

    if(msg == NULL)
    {
        return;
    }

    if(pMsg->reg)
    {
    	#ifdef HAVE_ONU_DELAY_OFFLINE_RESET
		{
			if(reg_off_timer)
			{
				cs_timer_del(reg_off_timer);
				reg_off_timer = 0;
			}
		}
		#endif
    }
    else
    {
    	#ifdef HAVE_RESET_CONTROL
    	if(onu_reset_enable_get())
    	{
    		#ifdef HAVE_ONU_DELAY_OFFLINE_RESET
			reg_off_timer = cs_timer_add(DELAY_RESET_TIME, onu_reset, NULL);
			#else
    		//reset onu
    		iros_system_reset(RESET_TYPE_FORCE);
			#endif
    	}
		#else
			#ifdef HAVE_ONU_DELAY_OFFLINE_RESET
			reg_off_timer = cs_timer_add(DELAY_RESET_TIME, onu_reset, NULL);
			#else
			//reset onu
	    	iros_system_reset(RESET_TYPE_FORCE);
			#endif
		#endif
    	
    }

    return;
}
#endif	/*(#ifdef HAVE_MPCP_DEREGISTER_REST)*/


#ifdef HAVE_MPCP_REG_TIMEOUT_RESET
extern cs_status mpcp_reg_status_setter(int enable);
void onu_evt_reg_proc(cs_uint16 dev, cs_int32 evt, void *msg)
{
    sdl_event_reg_t *pMsg = (sdl_event_reg_t *)msg;
	int enable = 0;
	
    if(msg == NULL)
    {
        return;
    }

    if(pMsg->reg)
    {
    	enable = 1;
    }
    else
    {
    	enable = 0;
    }

	mpcp_reg_status_setter(enable);
	
    return;
}
#endif


void event_init()
{
    cs_callback_context_t     context;
    IROS_LOG_MSG(IROS_LOG_LEVEL_INF, IROS_MID_EVENT, "EVENT: init\n");
#ifdef HAVE_ONU_EVENT    
    cs_event_init();
#endif
    memset(onu_events, 0, EPON_EVENT_TYPE_MAX*sizeof(onu_events_t));
    epon_request_onu_event_callback_reg(context, ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, 
        (sdl_event_cb_f)event_detect);
    onu_evt_reg(EPON_EVENT_REG_CHANGE, (void *)onu_evt_changed_led_proc, NULL);
    onu_evt_reg(EPON_EVENT_PORT_LINK_CHANGE,(void *)onu_link_changed_led_proc, NULL);
    onu_evt_reg(EPON_EVENT_PON_LASERON,(void *)onu_pon_laseron_proc, NULL);
	#ifdef HAVE_MPCP_DEREGISTER_REST
	onu_evt_reg(EPON_EVENT_REG_CHANGE, (void *)onu_evt_reg_changed_reset_proc, NULL);
	#endif
	#ifdef HAVE_MPCP_REG_TIMEOUT_RESET
	onu_evt_reg(EPON_EVENT_REG_CHANGE, (void *)onu_evt_reg_proc, NULL);
	#endif
}

void event_main_proc(cs_int32 type, cs_uint32 msg_len, void* msg, cs_boolean relay)
{
#ifdef HAVE_ONU_EVENT
    evt_msg_t *frame = NULL;
    cs_sdk_event_msg_t *evt;
    cs_int32 pkt_len =0;
    cs_uint32 size;
#endif    
    onu_event_handler_t handler = NULL;
    cs_int32 i;
    /*onu_event_hton_t hton_func = onu_events[type].hton_func;*/

    if (!msg || msg_len == 0) {
        return;
    }

    if (!relay) {
        for(i = 0 ; i < MAX_HANDLER_NUM ; i++)
        {
            handler = onu_events[type].handler[i];
            if(handler == NULL)
                continue;

            handler(0, type , msg);
        }
        return;
    }

#ifdef HAVE_ONU_EVENT
    size = sizeof(cs_oam_pdu_hdr_t) + sizeof(cs_sdk_event_msg_t) - 1 + msg_len;
    frame = (evt_msg_t *)evt_msg_alloc(size);
    
    if (!frame) {
        return;
    }

    pkt_len = cs_oam_hdr_build((cs_uint8 *)frame->evt_frame, &onu_node.sc.macaddr, CS_OAM_OPCODE_EVENT, CS_OUI);
    evt = (cs_sdk_event_msg_t *)((cs_uint8 *)frame->evt_frame+pkt_len);
    evt->msg_type = htonl(CS_ESS_SDK_EVENT);
    memcpy(evt->device.addr , onu_node.device.addr , 6);
    evt->event = htonl(type);
    evt->len = htonl(msg_len);

    /*hton_func(0, type , msg);*/
    memcpy(evt->data, msg, msg_len);
    cs_event_add(frame, size);
#endif
    return;
}


cs_uint8 onu_evt_msg_proc(iros_msg_t *msg)
{
    onu_event_msg_t *pEvtMsg = (onu_event_msg_t *)msg;
    onu_event_hdr_t *evt = (onu_event_hdr_t *)&pEvtMsg->evt;

    event_main_proc(evt->type, evt->msg_len, evt->data, evt->relay);

    return 1;
}

cs_int32 event_hton_reg(cs_int32 evt_type , void *func , void *cookie)
{

    if (func == NULL)
        return CS_E_ERROR;

    onu_events[evt_type].hton_func = func;

    return CS_E_OK;
}
