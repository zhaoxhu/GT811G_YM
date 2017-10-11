#ifdef HAVE_TERMINAL_SERVER
#include "plat_common.h"
#include "terminal_server.h"
#include "terminal_server_api.h"

cs_status epon_request_onu_ts_enable(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32              device,
    CS_IN cs_int32              llidport,
    CS_IN cs_uint8              uart_id,
    CS_IN term_server_config_t  *cfg
    )
{
    cs_int32 ret = CS_E_OK;
    
    if(cfg == NULL)
    {
        return CS_E_PARAM;
    }

    if(cfg->uart_cfg.uart_id != uart_id)
    {
        return CS_E_PARAM;
    }

    ret = ts_enable(cfg);
    if(ret != CS_E_OK)
        return CS_E_PARAM;

    return CS_E_OK;
}

cs_status epon_request_onu_ts_disable(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32              device,
    CS_IN cs_int32              llidport,
    CS_IN cs_uint8              uart_id
    )
{
    ts_disable(uart_id);

    return CS_E_OK;
}

cs_status epon_request_onu_ts_cfg_set(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32              device,
    CS_IN cs_int32              llidport,
    CS_IN cs_uint32             mask,
    CS_IN cs_uint8              uart_id,
    CS_IN cs_uint32             value
    )
{
    cs_int32 bit = 0;
    cs_uint32 set_type = 0;
    cs_int32 term_id=0;
    term_server_config_t *p = NULL;
    term_server_config_t tmp_cfg;
    term_server_config_t original_cfg;

    term_id = UART_TO_TERM_SERV(uart_id);
    if(term_id >= MAX_TERM_SERV_NUM)
    {
        return CS_E_PARAM;
    }
    
    p = (term_server_config_t *)&terminal_server_cfg[term_id];

    if(p->enable_flag != TERM_SERV_ENABLE_FLAG)
    {
        return CS_E_ERROR;
    }

    memcpy(&tmp_cfg, p, sizeof(term_server_config_t));
    memcpy(&original_cfg, p, sizeof(term_server_config_t));

    for(bit = 0; bit < 32; bit++)
    {
        set_type = (1<<bit)&mask;
        if(set_type == 0)
            continue;

        switch(set_type)
        {
        case TS_CFG_UART_DATA_BITS:
            ts_disable(uart_id);
            tmp_cfg.uart_cfg.data_bits = (cs_uint8)value;
            break;
        case TS_CFG_UART_STOP_BITS:
            ts_disable(uart_id);
            tmp_cfg.uart_cfg.stop_bits = (cs_uint8)value;
            break;
        case TS_CFG_UART_FLOW_CTRL:
            ts_disable(uart_id);
            tmp_cfg.uart_cfg.flow_control = (cs_uint8)value;
            break;
        case TS_CFG_UART_PARITY:
            ts_disable(uart_id);
            tmp_cfg.uart_cfg.parity = value;
            break;
        case TS_CFG_UART_DUPLEX:
            ts_disable(uart_id);
            tmp_cfg.uart_cfg.duplex = value;
            break;
        case TS_CFG_UART_MODE:
            ts_disable(uart_id);
            tmp_cfg.uart_cfg.mode = value;
            break;
        case TS_CFG_UART_BAUD_RATE:
            ts_disable(uart_id);
            tmp_cfg.uart_cfg.baud_rate = value;
            break;
        case TS_CFG_PROTO_TYPE:
            ts_disable(uart_id);
            tmp_cfg.proto_type = value;
            break;
        case TS_CFG_PROTO_PORT:
            ts_disable(uart_id);
            tmp_cfg.proto_port = value;
            break;
        case TS_CFG_CLIENT_TIMEOUT:
            ts_disable(uart_id);
            tmp_cfg.client_timeout = value;
            break;
        case TS_CFG_MAX_PAYLOAD:
            ts_disable(uart_id);
            tmp_cfg.max_payload = value;
            break;
        case TS_CFG_MIN_PAYLOAD:
            ts_disable(uart_id);
            tmp_cfg.min_payload = value;
            break;
        case TS_CFG_MAX_RES_TIME:
            ts_disable(uart_id);
            tmp_cfg.max_res_time = value;
            break;
        default:
            return CS_E_PARAM;
        }

        if(ts_enable(&tmp_cfg) != 0)
        {
            ts_enable(&original_cfg);
            return CS_E_PARAM;
        }

        break;            
    }
    
    return CS_E_OK;
}

cs_status epon_request_onu_ts_cfg_get(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32              device,
    CS_IN cs_int32              llidport,
    CS_IN cs_uint32             mask,
    CS_IN cs_uint8              uart_id,
    CS_IN cs_uint32             *value
    )
{
    cs_int32 bit = 0;
    cs_uint32 get_type = 0;
    cs_int32 term_id=0;
    term_server_config_t *p = NULL;

    term_id = UART_TO_TERM_SERV(uart_id);
    if(term_id >= MAX_TERM_SERV_NUM)
    {
        return CS_E_PARAM;
    }
    
    p = (term_server_config_t *)&terminal_server_cfg[term_id];

    if(p->enable_flag != TERM_SERV_ENABLE_FLAG)
    {
        return CS_E_ERROR;
    }

    for(bit = 0; bit < 32; bit++)
    {
        get_type = (1<<bit)&mask;
        if(get_type == 0)
            continue;
        
        switch(get_type)
        {
        case TS_CFG_UART_DATA_BITS:
            *value = (cs_uint32)p->uart_cfg.data_bits;
            break;
        case TS_CFG_UART_STOP_BITS:
            *value = (cs_uint32)p->uart_cfg.stop_bits;
            break;
        case TS_CFG_UART_FLOW_CTRL:
            *value = (cs_uint32)p->uart_cfg.flow_control;
            break;
        case TS_CFG_UART_PARITY:
            *value = (cs_uint32)p->uart_cfg.parity;
            break;
        case TS_CFG_UART_DUPLEX:
            *value = (cs_uint32)p->uart_cfg.duplex;
            break;
        case TS_CFG_UART_MODE:
            *value = (cs_uint32)p->uart_cfg.mode;
            break;
        case TS_CFG_UART_BAUD_RATE:
            *value = (cs_uint32)p->uart_cfg.baud_rate;
            break;
        case TS_CFG_PROTO_TYPE:
            *value = (cs_uint32)p->proto_type;
            break;
        case TS_CFG_PROTO_PORT:
            *value = (cs_uint32)p->proto_port;
            break;
        case TS_CFG_CLIENT_TIMEOUT:
            *value = (cs_uint32)p->client_timeout;
            break;
        case TS_CFG_MAX_PAYLOAD:
            *value = (cs_uint32)p->max_payload;
            break;
        case TS_CFG_MIN_PAYLOAD:
            *value = (cs_uint32)p->min_payload;
            break;
        case TS_CFG_MAX_RES_TIME:
            *value = (cs_uint32)p->max_res_time;
            break;
        default:
            return CS_E_PARAM;
        }
    }

    return CS_E_OK;
}

cs_status epon_request_onu_ts_cfg_save(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32              device,
    CS_IN cs_int32              llidport,
    CS_IN cs_uint8              uart_id
    )
{
    ts_cfg_save(uart_id);
    
    return CS_E_OK;
}

cs_status epon_request_onu_ts_cfg_clear(
    CS_IN cs_callback_context_t context,
    CS_IN cs_int32              device,
    CS_IN cs_int32              llidport,
    CS_IN cs_uint8              uart_id
    )
{
    ts_cfg_clear(uart_id);

    return CS_E_OK;
}

#endif

