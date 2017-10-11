#include "cs_types.h"
#include "plat_common.h"
#include "registers.h"
#include "gpio.h"
#include "led.h"
#include "sdl_pon.h"
#include "sdl_port.h"
#include "sdl_event_cmn.h"
#ifdef HAVE_NTT_OAM
#include "app_oam_ntt.h"
#endif
#include "frame.h"
#include "event.h"
#include "sdl.h"

void onu_single_led_proc(cs_uint32 gpio_id, cs_uint8 action, cs_uint32 *tmr);
void onu_double_led_proc(cs_uint32 map, cs_uint8 action, cs_uint32 code, cs_uint32 *tmr);
#ifdef HAVE_KT_OAM
void onu_start_detect_pon_act();
void onu_stop_detect_pon_act();
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
        onu_led_proc(ONU_LED_PON, PON_LED_ON_GREEN);
        }
    #elif HAVE_KT_OAM    
        onu_led_proc(ONU_LED_PON_LNK, ONU_LED_TURN_ON);
        onu_start_detect_pon_act();
    #else
        onu_led_proc(ONU_LED_PON_LNK, ONU_LED_TURN_ON);
    #endif
    }
    else
    {
    #ifdef HAVE_NTT_OAM
        ntt_dl_info_type_t  stats;
        ntt_dl_result_type_t result;
        onu_ntt_upgrade_info_get(&stats, &result);
        if(stats != NTT_DL_RST_WAIT) {
            onu_led_proc(ONU_LED_PON, PON_LED_OFF);
        }

        onu_led_proc(ONU_LED_AUTH, ONU_LED_TURN_OFF);
    #elif HAVE_KT_OAM    
        onu_led_proc(ONU_LED_PON_LNK, ONU_LED_TURN_OFF);
        onu_stop_detect_pon_act();
    #else
        onu_led_proc(ONU_LED_PON_LNK, ONU_LED_TURN_OFF);
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
            IROS_LOG_CRI(IROS_MID_EVENT, "PON port link up\n");
            cs_printf("PON port link up\n");
        }
        else
        {
            IROS_LOG_CRI(IROS_MID_EVENT, "PON port link down\n");
            cs_printf("PON port link down\n");
        }
    }

    if(pMsg->port >= CS_UNI_PORT_ID1 && pMsg->port <= UNI_PORT_MAX)
    {
        if(pMsg->link)
        {
            IROS_LOG_CRI(IROS_MID_EVENT, "UNI port-%d link up\n",pMsg->port);
            cs_printf("UNI port-%d link up\n",pMsg->port);
        }
        else
        {
            IROS_LOG_CRI(IROS_MID_EVENT, "UNI port-%d link down\n",pMsg->port);
            cs_printf("UNI port-%d link down\n",pMsg->port);
        }
    }
}

#ifdef HAVE_NTT_OAM

/* auth led */
cs_uint32 g_auth_led_gpio_id = 0;
cs_uint32 g_auth_led_tmr = 0;

cs_status onu_auth_led_init(cs_uint32 map)
{
    cs_uint32 i;
    for(i=0; i<ONU_GPIO_MAX_NUM; i++) {
        if(map & (1<<i) ) {
            g_auth_led_gpio_id = i;
            return CS_E_OK;
        }
    }

    return CS_E_NOT_FOUND;
}

/* uni led */
#if 0
cs_uint32 g_uni_led_gpio_id = 0;

cs_uint32 g_uni_led_tmr = 0;


cs_status onu_uni_led_init(cs_uint32 map)
{
    cs_uint32 i;
    for(i=0; i<ONU_GPIO_MAX_NUM; i++) {
        if(map & (1<<i) ) {
            LED_CUSTOM_C_CFG_t led_c_cfg;
            
            g_uni_led_gpio_id = i;

            led_c_cfg = g_lynxd_reg_ptr->led.CUSTOM_C_CFG;                   

            /* set LED_CUSTOM_C_CFG */        
            led_c_cfg.bf.cst_en = 1;
            led_c_cfg.bf.cst_on_val = 1;
            led_c_cfg.bf.pin_sel = (g_uni_led_gpio_id - 10);       
        
            g_lynxd_reg_ptr->led.CUSTOM_C_CFG = led_c_cfg;

            /* enable UNI LED */
            onu_uni_led_enable(1);
            return CS_E_OK;
        }
    }

    return CS_E_NOT_FOUND;
}

void onu_uni_led_enable(cs_uint8 enable)
{
    GLOBAL_CTRL_t glb_ctrl;    
    
    glb_ctrl = g_lynxd_reg_ptr->glb.CTRL;
    glb_ctrl.bf.led_gpio_sel &= ~(1<<(g_uni_led_gpio_id - 10));
    
    if(enable) {
        /* choose gpio 13 as uni led */
        glb_ctrl.bf.led_gpio_sel |= 1<<(g_uni_led_gpio_id - 10); 
    }

    g_lynxd_reg_ptr->glb.CTRL = glb_ctrl;
}
#endif


/* 
 * pon led 
 */
 
/* 01 - green   */
/* 10 - red      */
/* 00 - yellow  */

cs_uint32 g_pon_led_gpio_map = 0;
cs_uint32 g_pon_led_tmr = 0;
cs_uint32 g_pon_led_code[2];

cs_status onu_pon_led_init(cs_uint32 map)
{
    cs_uint8 gpio_id[2];    /* 0 - green, 1 - red */
    cs_uint8 i;
    cs_uint8 num = 0;
    
    /* init gpio map */
    g_pon_led_gpio_map = map;

    /* init code */
    for(i=0; i<ONU_GPIO_MAX_NUM; i++) {
        if(map & (1<<i)) {
            gpio_id[num++] = i;
            if(num > 2) {
                break;
            }
        }
    }
    
    if(num < 2) {
        return CS_E_NOT_FOUND;
    }

    g_pon_led_code[0] = 1 << gpio_id[1]; /* green led */
    g_pon_led_code[1] = 1 << gpio_id[0]; /* red led */

    /* init pattern */
    onu_gpio_set_pattern(gpio_id[0], 0x2, 2);
    onu_gpio_set_pattern(gpio_id[1], 0x2, 2);
    
    return CS_E_OK;
}

void onu_led_pon_proc(cs_uint8 action)
{
    cs_uint32 code;
    
    switch(action) {
        case PON_LED_OFF:
            action = ONU_LED_TURN_OFF;
            code = -1;
            break;
            
        case PON_LED_ON_GREEN:
            action = ONU_LED_TURN_ON;
            code = g_pon_led_code[ONU_LED_GREEN-1];
            break;

        case PON_LED_ON_RED:
            action = ONU_LED_TURN_ON;
            code = g_pon_led_code[ONU_LED_RED-1];
            break;

        case PON_LED_ON_YELLOW:
            action = ONU_LED_TURN_ON;
            code = 0;
            break;

        case PON_LED_TWINKLE_YELLOW:
            action = ONU_LED_TWINKLE;
            break;

        default:
            return;
    }
    
    onu_double_led_proc(g_pon_led_gpio_map, action, code, &g_pon_led_tmr);
}

/* power led */
cs_uint32 g_pwr_led_gpio_map = 0;
cs_uint32 g_pwr_led_tmr = 0;
cs_uint32 g_pwr_led_code[2];

cs_status onu_pwr_led_init(cs_uint32 map)
{
    cs_uint8 gpio_id[2];
    cs_uint8 i;
    cs_uint8 num = 0;

    /* init map */
    g_pwr_led_gpio_map = map;

    /* init code */
    for(i=0; i<ONU_GPIO_MAX_NUM; i++) {
        if(map & (1<<i)) {
            gpio_id[num++] = i;
            if(num > 2) {
                break;
            }
        }
    }
    
    if(num < 2) {
        return CS_E_NOT_FOUND;
    }

    g_pwr_led_code[0] = 1 << gpio_id[1]; /* green led */
    g_pwr_led_code[1] = 1 << gpio_id[0]; /* red led */

    /* init pattern */
    onu_gpio_set_pattern(gpio_id[0], 0x3, 2);   /* green is off */
    onu_gpio_set_pattern(gpio_id[1], 0x2, 2);   /* red twinkle */

    return CS_E_OK;
}

void onu_led_pwr_proc(cs_uint8 action)
{
    cs_uint32 code;
    
    switch(action) {
        case PWR_LED_OFF:
            action = ONU_LED_TURN_OFF;
            code = -1;
            break;
            
        case PWR_LED_ON_GREEN:
            action = ONU_LED_TURN_ON;
            code = g_pwr_led_code[ONU_LED_GREEN-1];
            break;

        case PWR_LED_ON_RED:
            action = ONU_LED_TURN_ON;
            code = g_pwr_led_code[ONU_LED_RED-1];
            break;

        case PWR_LED_TWINKLE_RED:
            action = ONU_LED_TWINKLE;
            break;

        default:
            return;
    }

    onu_double_led_proc(g_pwr_led_gpio_map, action, code, &g_pwr_led_tmr);
} 

#endif

#ifdef HAVE_KT_OAM

/*
 * PON LNK LED
 */
 
cs_uint32 g_pon_lnk_led_gpio_id = 0;
cs_uint32 g_pon_lnk_led_tmr = 0;

cs_status onu_pon_lnk_led_init(cs_uint32 map)
{
    cs_uint32 i;
    for(i=0; i<ONU_GPIO_MAX_NUM; i++) {
        if(map & (1<<i) ) {
            g_pon_lnk_led_gpio_id = i;
            return CS_E_OK;
        }
    }

    return CS_E_NOT_FOUND;
}

/*
 * PON ACT LED
 */
 
cs_uint32 g_pon_act_led_gpio_id = 0;
cs_uint32 g_pon_act_led_tmr = 0;
cs_uint32 g_pon_act_detect_tmr = 0;
cs_uint32 g_pon_act_led_blink = 0;
cs_uint32 g_pon_act_stats = 0;
cs_uint32 g_uni_act_stats = 0;

cs_status onu_pon_act_led_init(cs_uint32 map)
{
    cs_uint32 i;
    for(i=0; i<ONU_GPIO_MAX_NUM; i++) {
        if(map & (1<<i) ) {
            g_pon_act_led_gpio_id = i;
            return CS_E_OK;
        }
    }
    return CS_E_NOT_FOUND;
}

void onu_detect_pon_active(void *data)
{
    cs_callback_context_t context;
    cs_pon_stats_t stats;
    cs_sdl_port_uni_stats_t uni_stat;
    cs_uint32 pon_sum = 0;
    cs_uint32 uni_sum = 0;

    #ifdef HAVE_MPORTS
    epon_request_onu_port_stats_get(context,0,0,CS_DOWNLINK_PORT,0,
                               (cs_sdl_port_uni_stats_t *)&uni_stat);
    #else
    epon_request_onu_port_stats_get(context,0,0,CS_UNI_PORT_ID1,0,
                               (cs_sdl_port_uni_stats_t *)&uni_stat);
    #endif
    epon_request_onu_pon_stats_get(context, 0, 0, 0, &stats);

    pon_sum = stats.pon_bcframe_cnt + stats.pon_mcframe_cnt + stats.pon_frame_cnt +
        stats.pon_txframe_cnt + stats.pon_txmcframe_cnt + stats.pon_txbcframe_cnt;
    uni_sum = uni_stat.rxbcfrm_cnt + uni_stat.rxmcfrm_cnt + uni_stat.rxucfrm_cnt +
        uni_stat.txbcfrm_cnt + uni_stat.txmcfrm_cnt+ uni_stat.txucfrm_cnt;
    
    if((pon_sum != g_pon_act_stats) && (uni_sum != g_uni_act_stats)){
        if(!g_pon_act_led_blink) {
            g_pon_act_led_blink = 1;
            onu_led_proc(ONU_LED_PON_ACT, ONU_LED_TWINKLE);
        }
    }
    else {
        if(g_pon_act_led_blink) {
            g_pon_act_led_blink = 0;
            onu_led_proc(ONU_LED_PON_ACT, ONU_LED_TURN_OFF);
        }
    }

    g_pon_act_stats = pon_sum;
    g_uni_act_stats = uni_sum;
}

void onu_start_detect_pon_act()
{
    cs_callback_context_t context;
    cs_pon_stats_t stats;
    
    epon_request_onu_pon_stats_get(context, 0, 0, 0, &stats);
    g_pon_act_stats = stats.pon_bcframe_cnt + stats.pon_mcframe_cnt + stats.pon_frame_cnt +
        stats.pon_txframe_cnt + stats.pon_txmcframe_cnt + stats.pon_txbcframe_cnt;

    if(g_pon_act_detect_tmr) {
        cs_timer_restart(g_pon_act_detect_tmr);
    }
    else {
        g_pon_act_detect_tmr = cs_circle_timer_add(PON_ACT_LED_DETECT_INTERVAL, 
            onu_detect_pon_active, NULL);
    }
}

void onu_stop_detect_pon_act()
{
    cs_timer_stop(g_pon_act_detect_tmr);
}

#endif  /* #ifdef HAVE_KT_OAM */

#ifdef HAVE_CTC_OAM
/* power led */
cs_uint32 g_pwr_led_gpio_map = 0;
cs_uint32 g_pwr_led_tmr = 0;
cs_uint32 g_pwr_led_code[2];

cs_status onu_pwr_led_init(cs_uint32 map)
{
    cs_uint8 gpio_id[2];
    cs_uint8 i;
    cs_uint8 num = 0;

    /* init map */
    g_pwr_led_gpio_map = map;

    /* init code */
    for(i=0; i<ONU_GPIO_MAX_NUM; i++) {
        if(map & (1<<i)) {
            gpio_id[num++] = i;
            if(num > 2) {
                break;
            }
        }
    }
    
    if(num < 2) {
        return CS_E_NOT_FOUND;
    }

    g_pwr_led_code[0] = 1 << gpio_id[1]; /* green led */
    g_pwr_led_code[1] = 1 << gpio_id[0]; /* red led */

    /* init pattern */
    onu_gpio_set_pattern(gpio_id[0], 0x3, 2);   /* green is off */
    onu_gpio_set_pattern(gpio_id[1], 0x2, 2);   /* red twinkle */

    return CS_E_OK;
}

cs_uint32 g_auth_led_gpio_id = 0;
cs_uint32 g_auth_led_tmr = 0;

cs_status onu_auth_led_init(cs_uint32 map)
{
    cs_uint32 i;
    for(i=0; i<ONU_GPIO_MAX_NUM; i++) {
        if(map & (1<<i) ) {
            g_auth_led_gpio_id = i;
            return CS_E_OK;
        }
    }

    return CS_E_NOT_FOUND;
}

cs_uint32 g_pon_led_gpio_map = 0;
cs_uint32 g_pon_led_tmr = 0;
cs_uint32 g_pon_led_code[2];

cs_status onu_pon_led_init(cs_uint32 map)
{
    cs_uint8 gpio_id[2];    /* 0 - green, 1 - red */
    cs_uint8 i;
    cs_uint8 num = 0;
    
    /* init gpio map */
    g_pon_led_gpio_map = map;

    /* init code */
    for(i=0; i<ONU_GPIO_MAX_NUM; i++) {
        if(map & (1<<i)) {
            gpio_id[num++] = i;
            if(num > 2) {
                break;
            }
        }
    }
    
    if(num < 2) {
        return CS_E_NOT_FOUND;
    }

    g_pon_led_code[0] = 1 << gpio_id[1]; /* green led */
    g_pon_led_code[1] = 1 << gpio_id[0]; /* red led */

    /* init pattern */
    onu_gpio_set_pattern(gpio_id[0], 0x2, 2);
    onu_gpio_set_pattern(gpio_id[1], 0x2, 2);

    return CS_E_OK;
}

void onu_led_pon_proc(cs_uint8 action)
{
    cs_uint32 code;
    
    switch(action) {
        case PON_LED_OFF:
            action = ONU_LED_TURN_OFF;
            code = -1;
            break;
            
        case PON_LED_ON_GREEN:
            action = ONU_LED_TURN_ON;
            code = g_pon_led_code[ONU_LED_GREEN-1];
            break;

        case PON_LED_ON_RED:
            action = ONU_LED_TURN_ON;
            code = g_pon_led_code[ONU_LED_RED-1];
            break;

        case PON_LED_ON_YELLOW:
            action = ONU_LED_TURN_ON;
            code = 0;
            break;

        case PON_LED_TWINKLE_YELLOW:
            action = ONU_LED_TWINKLE;
            break;

        default:
            return;
    }
    
    onu_double_led_proc(g_pon_led_gpio_map, action, code, &g_pon_led_tmr);
}

void onu_led_pwr_proc(cs_uint8 action)
{
    cs_uint32 code;
    
    switch(action) {
        case PWR_LED_OFF:
            action = ONU_LED_TURN_OFF;
            code = -1;
            break;
            
        case PWR_LED_ON_GREEN:
            action = ONU_LED_TURN_ON;
            code = g_pwr_led_code[ONU_LED_GREEN-1];
            break;

        case PWR_LED_ON_RED:
            action = ONU_LED_TURN_ON;
            code = g_pwr_led_code[ONU_LED_RED-1];
            break;

        case PWR_LED_TWINKLE_RED:
            action = ONU_LED_TWINKLE;
            break;

        default:
            return;
    }

    onu_double_led_proc(g_pwr_led_gpio_map, action, code, &g_pwr_led_tmr);
}

#endif

void onu_single_led_proc(cs_uint32 gpio_id, cs_uint8 action, cs_uint32 *tmr) 
{
    switch(action) {
        case ONU_LED_TURN_OFF:
            if(*tmr) {
                onu_gpio_stop_timer(*tmr);
                *tmr=0;
            }
            onu_gpio_set_status(1<<gpio_id, -1);
            break;

        case ONU_LED_TURN_ON:
            if(*tmr) {
                onu_gpio_stop_timer(*tmr);
                *tmr=0;
            }
            onu_gpio_set_status(1<<gpio_id, 0);
            break;

        case ONU_LED_TWINKLE:
            if(tmr) {
                onu_gpio_set_pattern(gpio_id, 0x38, 6);
                if(*tmr) {
                    cs_gpio_timer_t *gpio_tmr = (cs_gpio_timer_t *)tmr;
                    int i;
                    
                    for(i = 0; i < ONU_GPIO_MAX_NUM; i++)
                    {
                        if(gpio_tmr->map & (1<<i))
                            break;
                    }
                    
                    if(i >= ONU_GPIO_MAX_NUM)
                    {
                        break;
                    }
                    
                    gpio_tmr->cfg.ptn_ctrl.len = g_gpio_dev.cfg[i].info.pattern.len;
                    gpio_tmr->cfg.ptn_ctrl.iter = 0;
                    /*onu_gpio_restart_timer(*tmr);*/
                }
                else {
                    *tmr = onu_gpio_start_timer(1<<gpio_id, ONU_LED_TWINKLE_PERIOD);
                }
            }
            break;
        case ONU_LED_TWINKLE_FAST:
            if(tmr) {
                onu_gpio_set_pattern(gpio_id, 0x2, 2);
                if(*tmr) {
                    cs_gpio_timer_t *gpio_tmr = (cs_gpio_timer_t *)tmr;
                    int i;
                    
                    for(i = 0; i < ONU_GPIO_MAX_NUM; i++)
                    {
                        if(gpio_tmr->map & (1<<i))
                            break;
                    }
                    
                    if(i >= ONU_GPIO_MAX_NUM)
                    {
                        break;
                    }
                    
                    gpio_tmr->cfg.ptn_ctrl.len = g_gpio_dev.cfg[i].info.pattern.len;
                    gpio_tmr->cfg.ptn_ctrl.iter = 0;
                    /*onu_gpio_restart_timer(*tmr);*/
                }
                else {
                    *tmr = onu_gpio_start_timer(1<<gpio_id, ONU_LED_TWINKLE_PERIOD);
                }
            }
            break;
        default:
            break;
    }
    
}

/* basic double led precess */
void onu_double_led_proc(cs_uint32 map, cs_uint8 action, cs_uint32 code, cs_uint32 *tmr) 
{
    switch(action) {
        case ONU_LED_TURN_OFF:
            if(*tmr) {
                onu_gpio_stop_timer(*tmr);
            }
            onu_gpio_set_status(map, -1);
            break;

        case ONU_LED_TURN_ON:
            if(*tmr) {
                onu_gpio_stop_timer(*tmr);
            }
            onu_gpio_set_status(map, code);
            break;

        case ONU_LED_TWINKLE:
            if(*tmr) {
                onu_gpio_restart_timer(*tmr);
            }
            else {
                *tmr = onu_gpio_start_timer(map, ONU_LED_TWINKLE_PERIOD);
            }
            break;

        default:
            break;
    }
    
}


void onu_led_pwr_proc(cs_uint8 action);

void onu_led_proc(cs_uint8 led, cs_uint8 action)
{
    switch(led) {

#ifdef HAVE_NTT_OAM
        case ONU_LED_AUTH:
        onu_single_led_proc(g_auth_led_gpio_id, action, &g_auth_led_tmr);
        break;
    case ONU_LED_PON:
        onu_led_pon_proc(action);
        break;
    case ONU_LED_PWR:
        onu_led_pwr_proc(action);
        break;
#endif  /* #ifdef HAVE_NTT_OAM */

#ifdef HAVE_KT_OAM
    case ONU_LED_PON_LNK:
        onu_single_led_proc(g_pon_lnk_led_gpio_id, action, &g_pon_lnk_led_tmr);
        break;
    case ONU_LED_PON_ACT:
        onu_single_led_proc(g_pon_act_led_gpio_id, action, &g_pon_act_led_tmr);
        break;
#endif  /* #ifdef HAVE_KT_OAM */

#ifdef HAVE_CTC_OAM 
    case ONU_LED_PWR:
        onu_led_pwr_proc(action);
        break;
    case ONU_LED_AUTH:
        onu_single_led_proc(g_auth_led_gpio_id, action, &g_auth_led_tmr);
        break;    
#if 0        
    case ONU_LED_PON:
        onu_led_pon_proc(action);
        break;    
#endif        
#endif


    default:
        break;
    }

    return ;
}

