#include <stdio.h>
#include "cgi_cmn.h"
#include "sdl_api.h"
#include "sdl_l2.h"
#include "oam_ctc_pdu.h"
#include "oam_ctc_pdu.h"
#include "app_oam_ctc_adapt.h"
#include "app_oam_ctc_adapt.h"
#include "cgi_cs_port_config.h"
#ifdef HAVE_LOOP_DETECT
#include "loop_detect.h"
#endif
#include "port_stats.h"
#ifdef HAVE_PPPOE
#include "pppoe_api.h"
#endif
#include "db_instance.h"
#ifdef HAVE_ONU_RSTP
#include "stp_api.h"
#endif
extern cs_status epon_request_onu_ip_mtu_set(cs_uint32 mtu); 
extern cs_status epon_request_onu_ip_mtu_get(cs_uint32 *mtu);

cs_status cgi_cs_port_work_mode_get(cs_port_id_t port, cs_uint8 *mode)
{
    cs_sdl_port_speed_cfg_t   cfg ;
    cs_callback_context_t context;
    cs_status ret = CS_E_OK;

    ret = epon_request_onu_port_status_get(context, 
        ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, &cfg);


    if(ret != CS_E_OK){
        CGI_LOG("get port %d status failed, ret = %d \n",port, ret);
        return CS_E_ERROR;
    }

    switch (cfg) {
        case SDL_PORT_10_FULL:
             *mode = CGI_CTC_PORT_MODE_10M_FULL;
             break;
          case SDL_PORT_10_HALF:
             *mode = CGI_CTC_PORT_MODE_10M_HALF;
             break;
          case SDL_PORT_100_FULL:
             *mode = CGI_CTC_PORT_MODE_100M_FULL;
             break;
          case SDL_PORT_100_HALF:
             *mode = CGI_CTC_PORT_MODE_100M_HALF;
             break;
          case SDL_PORT_1000_FULL:
             *mode = CGI_CTC_PORT_MODE_1000M_FULL;
             break;
          case SDL_PORT_AUTO_10_100:
          case SDL_PORT_AUTO_10_100_1000:
             *mode = CGI_CTC_PORT_MODE_AUTO;
             break;
        default:
            return CS_E_ERROR;
    }

    return CS_E_OK;
}

cs_status cgi_cs_port_work_mode_set(cs_port_id_t port, cs_uint8 mode)
{
    cs_status ret;
    cs_sdl_port_speed_cfg_t cfg = 0;
    cs_callback_context_t context;

    switch (mode) {
        case CGI_CTC_PORT_MODE_AUTO:
            cfg = SDL_PORT_AUTO_10_100_1000;
            break;
        case CGI_CTC_PORT_MODE_10M_HALF:
            cfg = SDL_PORT_10_HALF;
            break;
        case CGI_CTC_PORT_MODE_10M_FULL:
            cfg = SDL_PORT_10_FULL;
            break;
        case CGI_CTC_PORT_MODE_100M_HALF:
            cfg = SDL_PORT_100_HALF;
            break;
        case CGI_CTC_PORT_MODE_100M_FULL:
            cfg = SDL_PORT_100_FULL;
            break;
        case CGI_CTC_PORT_MODE_1000M_FULL:
            cfg = SDL_PORT_1000_FULL;
            break;
        default:
            return CS_E_PARAM;
    }            

    ret = epon_request_onu_port_status_set(context, 
        ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, cfg);

    return ret;

}
cgi_status cgi_cs_port_config_get(cgi_handler_param_t * p)
{
    cs_int8 buff[CGI_MAX_BUFF_LEN];
    cs_int8 tmpBuff[CGI_MAX_BUFF_LEN];
    cs_status ret = CS_E_OK;
    cs_uint32 i = 0;
	cs_uint8 sw_type;  

    CGI_DEBUG("enter \n");

    memset(buff, 0, sizeof(buff));
    sprintf(buff,"[");
    for(i = 0; i < CGI_MAX_UNI_PORT; i++){
        cs_sdl_port_admin_t          port_admin = 0;
        cs_sdl_port_link_status_t    link_status = 0;
        cs_sdl_port_autoneg_status_t auto_neg = 0;
        cs_uint8 work_mode;
        cs_boolean pause;
        cs_uint8 stp_state;
        cs_callback_context_t context;
        cs_uint8 port = i + 1;
        cs_sdl_stp_state_t stp;

        memset(tmpBuff,0x00,sizeof(tmpBuff));

        ret = epon_request_onu_port_link_status_get(context, 
                ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, &link_status);
        if(ret != CS_E_OK){
            CGI_LOG("get port %d Link status failed, ret = %d \n",port,ret);
            return CS_E_ERROR;
        }

        ret = epon_request_onu_port_admin_get(context, 
                ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, &port_admin);
        if(ret != CS_E_OK){
            CGI_LOG("get port %d admin status failed, ret = %d \n",port, ret);
            return CS_E_ERROR;
        }

        ret = epon_request_onu_port_auto_neg_get(context, 
                ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, &auto_neg);
        if(ret != CS_E_OK){
            CGI_LOG("get port %d auto admin status failed, ret = %d \n",port, ret);
            return CS_E_ERROR;
        }


        ret = cgi_cs_port_work_mode_get(port,&work_mode);
        if(ret != CS_E_OK){
            CGI_LOG("get port %d work mode failed, ret = %d \n",port, ret);
            return CS_E_ERROR;
        }
    
        
        ret = epon_request_onu_port_flow_ctrl_get(context, 
                ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, &pause);
        if(ret != CS_E_OK){
            CGI_LOG("get port %d pause failed, ret = %d \n",port, ret);
            return CS_E_ERROR;
        }
        
        if(SDL_PORT_LINK_STATUS_DOWN == link_status){
            stp_state = 0;
        }else{
            ret = epon_request_onu_stp_get(context, 
                    ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port,&stp);
            if(ret != CS_E_OK){
                CGI_LOG("get port %d stp failed, ret = %d \n",port, ret);
                return CS_E_ERROR;
            }

            switch(stp){
                case STP_S_DISABLED: 
                    stp_state = 1;
                    break;
                case STP_S_BLOCKING: 
                    stp_state = 2;
                    break;
                case STP_S_LEARNING: 
                    stp_state = 4;
                    break;
                case STP_S_FORWARDING: 
                    stp_state = 5;
                    break;
                default:
                    stp_state = 6;
            }
        }
        if(i != 0){
            strcat(buff,",");
        }

		startup_config_read(CFG_ID_SWITCH_TYPE, 1, &sw_type);

        sprintf(tmpBuff,"{port:%d, link:%d, phy:%d, auto:%d, mode:%d,mode_max:%d,pause:%d, stpstate:%d}",
                port,link_status,port_admin,auto_neg,work_mode,
#if  defined(HAVE_MPORTS)
                sw_type,
#else
                CGI_CTC_PORT_MODE_1000M_FULL,
#endif
                pause,stp_state);
        strcat(buff,tmpBuff);
    }

    strcat(buff,"]");

    cgi_send_str(p, buff,strlen(buff)); 

    return CS_E_OK;

}


cs_status cgi_cs_port_auto_set(cs_port_id_t port, cs_uint8 auto_neg)
{
    cs_status ret;
    cs_sdl_port_speed_cfg_t cfg = 0;
    cs_callback_context_t context;

    ret = epon_request_onu_port_status_get(context, 
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, &cfg);
    if (ret) {
        return ret;
    }

    if (auto_neg == SDL_PORT_AUTONEG_DOWN) {
        if (cfg == SDL_PORT_AUTO_10_100_1000 || 
                cfg == SDL_PORT_AUTO_10_100 || 
                cfg == SDL_PORT_1000_FULL) {
            cfg = SDL_PORT_100_FULL;
            ret = epon_request_onu_port_status_set(context, 
                    ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, cfg);
        }
    } else {
        if (cfg != SDL_PORT_AUTO_10_100_1000 && 
                cfg != SDL_PORT_AUTO_10_100 &&
                cfg != SDL_PORT_1000_FULL) {
            cfg = SDL_PORT_AUTO_10_100_1000;
            ret = epon_request_onu_port_status_set(context, 
                    ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, cfg);
        }
    }

    return ret;

}



cgi_status cgi_cs_port_config_set(cgi_handler_param_t * p)
{
    cs_uint8 port, admin,auto_neg,mode, pause;
    cs_status ret = CS_E_OK;
    cs_callback_context_t context;
    db_port_t   port_info;


    if(CS_E_OK != cgi_get_int8_by_key(p,"port",&port)
            || CS_E_OK != cgi_get_int8_by_key(p,"phy",&admin)
            || CS_E_OK != cgi_get_int8_by_key(p,"auto",&auto_neg)
            || CS_E_OK != cgi_get_int8_by_key(p,"mode",&mode)
            || CS_E_OK != cgi_get_int8_by_key(p,"pause",&pause)){
        CGI_LOG("get parameters failed from URL");
        cgi_send_int(p,CGI_ERROR_PARAM);
        return CS_E_ERROR;
    }

    CGI_DEBUG("port = %d ,admin = %d ,auto = %d ,mode = %d ,pause = %d \n",
            port,admin,auto_neg,mode,pause);

    ret = epon_request_onu_port_admin_set(context, 
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, admin);
    if(ret != CS_E_OK){
        CGI_LOG("set port admin status failed ,ret = %d \n",ret);
        cgi_send_int(p,CGI_SDL_FAILED);
        return CS_E_ERROR;
    }

    ret = cgi_cs_port_auto_set(port,auto_neg);
    if(ret != CS_E_OK){
        CGI_LOG("set port auto status failed ,ret = %d \n",ret);
        cgi_send_int(p,CGI_SDL_FAILED);
        return CS_E_ERROR;
    }

    ret = cgi_cs_port_work_mode_set(port,mode);
    if(ret != CS_E_OK){
        CGI_LOG("set port working mode failed ,ret = %d \n",ret);
        cgi_send_int(p,CGI_SDL_FAILED);
        return CS_E_ERROR;
    }

    ret = epon_request_onu_port_flow_ctrl_set(context, 
            ONU_DEVICEID_FOR_API, ONU_LLIDPORT_FOR_API, port, pause);
    if(ret != CS_E_OK){
        CGI_LOG("set port pause failed ,ret = %d \n",ret);
        cgi_send_int(p,CGI_SDL_FAILED);
        return CS_E_ERROR;
    }

    memset(&port_info,0x00,sizeof(port_info));
    db_read(DB_PORT_INFO_ID,(cs_uint8 *) &port_info, sizeof(port_info));
    port_info.port[port - 1].admin= admin;
    port_info.port[port - 1].auto_neg = auto_neg;
    switch (mode) {
        case CGI_CTC_PORT_MODE_AUTO:
            port_info.port[port - 1].work_mode = SDL_PORT_AUTO_10_100_1000;
            break;
        case CGI_CTC_PORT_MODE_10M_HALF:
            port_info.port[port - 1].work_mode = SDL_PORT_10_HALF;
            break;
        case CGI_CTC_PORT_MODE_10M_FULL:
            port_info.port[port - 1].work_mode = SDL_PORT_10_FULL;
            break;
        case CGI_CTC_PORT_MODE_100M_HALF:
            port_info.port[port - 1].work_mode = SDL_PORT_100_HALF;
            break;
        case CGI_CTC_PORT_MODE_100M_FULL:
            port_info.port[port - 1].work_mode = SDL_PORT_100_FULL;
            break;
        case CGI_CTC_PORT_MODE_1000M_FULL:
            port_info.port[port - 1].work_mode = SDL_PORT_1000_FULL;
            break;
        default:
            return CS_E_PARAM;
    }     
    port_info.port[port - 1].pause = pause;
    db_write(DB_PORT_INFO_ID,(cs_uint8 *) &port_info, sizeof(port_info));




    cs_thread_delay(2000);
    cgi_send_int(p,CGI_OK);

    return CS_E_OK;

}



