#include "cs_types.h"
#include "plat_common.h"
#include "sdl_gpio.h"
#include "sdl_pon.h"
#include "app_gpio.h"
#include "oam_api.h"
#include "app_alarm_ctc.h"
#include "sdl_event_cmn.h"

cs_uint32 feed_circle = 0;
cs_uint32 feed_wd_timer = CS_E_ERROR;
cs_int32 gpio_reset_flag = 0;

#if 1
//added by zhuxh,pon tran rx power is hanged up,gpio 13 is used for loop led,gpio 5 is not used,so use gpio 5 instead of gpio 13.
#define __PON_TRAN_RX_POWER_CTRL_GPIO    (0x5)
#else
//modified by zhuxh
#define __PON_TRAN_RX_POWER_CTRL_GPIO    (0xd)
#endif
#ifdef HAVE_GWD_PON_LASER_CONTROL
#define __PON_TRAN_TX_POWER_CTRL_GPIO    (0x7)
#else
#define __PON_TRAN_TX_POWER_CTRL_GPIO    (0x6)
#endif


cs_status cs_gpio_pon_tx_set(
        CS_IN   cs_callback_context_t           context,
        CS_IN   cs_dev_id_t                     device,
        CS_IN   cs_llid_t                       llidport,
        CS_IN   cs_uint8                        data)
{
    cs_status ret;
    
    ret = cs_gpio_write(__PON_TRAN_TX_POWER_CTRL_GPIO, data);
    
    if(ret==CS_OK)
        cs_led_los_handle();

    return ret;
}

cs_status cs_gpio_pon_rx_set(
        CS_IN   cs_callback_context_t           context,
        CS_IN   cs_dev_id_t                     device,
        CS_IN   cs_llid_t                       llidport,
        CS_IN   cs_uint8                        data)
{
    return cs_gpio_write(__PON_TRAN_RX_POWER_CTRL_GPIO, data);
}

cs_status cs_gpio_pon_tx_get(
        CS_IN   cs_callback_context_t           context,
        CS_IN   cs_dev_id_t                     device,
        CS_IN   cs_llid_t                       llidport,
        CS_IN   cs_uint8*                        data)
{
    return cs_gpio_read(__PON_TRAN_TX_POWER_CTRL_GPIO, data);;
}

cs_status cs_gpio_pon_rx_get(
        CS_IN   cs_callback_context_t           context,
        CS_IN   cs_dev_id_t                     device,
        CS_IN   cs_llid_t                       llidport,
        CS_IN   cs_uint8*                        data)
{
    return cs_gpio_read(__PON_TRAN_RX_POWER_CTRL_GPIO, data);
}


static void onu_feed_watchdog(void *data)
{
    cs_callback_context_t context;
    if(gpio_reset_flag)
        return;

    if(feed_circle%2 == 0)
        cs_plat_gpio_write(context, 0, 0, GPIO_PIN11, 1);
    else
        cs_plat_gpio_write(context, 0, 0, GPIO_PIN11, 0);

    feed_circle++;
}

void onu_watchdog_init()
{
    cs_callback_context_t context;
    cs_plat_gpio_write(context, 0, 0, GPIO_PIN11, 1);
    cs_thread_delay(10);
    cs_plat_gpio_write(context, 0, 0, GPIO_PIN11, 0);
    cs_thread_delay(10);
    cs_plat_gpio_write(context, 0, 0, GPIO_PIN11, 1);

    cs_plat_gpio_write(context, 0, 0, GPIO_PIN10, 0);
    feed_wd_timer = cs_hw_circle_timer_add(5000, onu_feed_watchdog, NULL);
    if(feed_wd_timer == CS_E_ERROR)
    {
        cs_printf("Create wd timer failed\n");
        cs_plat_gpio_write(context, 0, 0, GPIO_PIN10, 1);
        return;
    }

    cs_printf("Enable watch-dog!\n");
}

cs_uint8 hw_ver[2] = {0};
void onu_hw_version_get(cs_uint8 *hw_vstr, cs_int32 len)
{
    cs_int8 ver_str[128];
    cs_uint8  sw_type; 
    
    memset(ver_str, 0 , sizeof(ver_str));

    if(hw_ver[0] == 0 && hw_ver[1] == 0)
    {
        strcpy(ver_str, "V1.0_D");
    }

    if(hw_ver[0] == 0 && hw_ver[1] == 1)
    {
        strcpy(ver_str, "V1.0");
    }
    
    startup_config_read(CFG_ID_SWITCH_TYPE, 1, &sw_type);
    if(1 == sw_type) /*1: RTL8365, 0: RTL8305  */
    {
    if(hw_ver[0] == 1 && hw_ver[1] == 0)
    {
        strcpy(ver_str, "V1.0_U");
    }

    if(hw_ver[0] == 1 && hw_ver[1] == 1)
    {
        strcpy(ver_str, "V1.0_E");
    }
    }
    else  /* RTL8305 */
    {
    if(hw_ver[0] == 1 && hw_ver[1] == 0)
    {
        strcpy(ver_str, "V2.0_U");
    }

    if(hw_ver[0] == 1 && hw_ver[1] == 1)
    {
        strcpy(ver_str, "V2.0_E");
    }
    }

    memcpy(hw_vstr, ver_str ,strlen(ver_str));
}

void onu_hw_version()
{
    cs_uint8 status = 0;
    cs_callback_context_t context;

    cs_plat_gpio_mode_set(context, 0,0, GPIO_PIN12, GPIO_MODE_INPUT_POLL);
    cs_plat_gpio_mode_set(context, 0,0, GPIO_PIN13, GPIO_MODE_INPUT_POLL);
    cs_plat_gpio_read(context, 0,0, GPIO_PIN12, &status);
    if(status)
        hw_ver[0] = 1;
    else
        hw_ver[0] = 0;

    status = 0;
    cs_plat_gpio_read(context, 0,0, GPIO_PIN13, &status);
    if(status)
        hw_ver[1] = 1;
    else
        hw_ver[1] = 0;
}

cs_uint64 dgasp_prev_time = 0;
cs_uint8 dgasp_cnt = 0;
cs_uint8 dying_gasp_cnt = 0;
void cs_gpio_dying_gasp(cs_int32 gpio_pin, cs_uint8 status)
{
    cs_uint64 cur_time = 0;

    cur_time = cs_current_time();
    if(cur_time - 100 >= dgasp_prev_time)
    {
        dgasp_cnt = 0;
    }
    else
    {
        dgasp_cnt++;
    }
    dgasp_prev_time = cur_time;
    if(dgasp_cnt > dying_gasp_cnt)
    {
        return;
    }
    oam_send_dying_gasp(0);
    return;
}

cs_uint8 battery_log[5] = {1,1,1,1,1};
void battery_logical_set(int id, cs_uint8 log)
{
    if(id < 0 || id >= 5)
        return;

    battery_log[id] = log?1:0;
    return;
}

extern void db_restore_to_factory();
cs_uint8 restore_counter = 0;
cs_uint8 prev_lao_status = 0;
cs_uint8 prev_a_status = 0;
cs_uint8 prev_b_status = 0;
cs_uint8 prev_ups_status = 0;
cs_uint8 prev_ac_status = 0;
cs_uint8 prev_ups_volt_status = 0;
cs_uint8 ups_valid = 0;
void cs_gpio_pin_polling(cs_int32 gpio_pin, cs_uint8 status)
{
    switch(gpio_pin)
    {
    case GPIO_PIN2:
        if(status && !prev_lao_status)
        {
            sdl_event_laseron_t laseron_evt;        
            laseron_evt.laseron=1;
            if(sdl_event_send(EPON_EVENT_PON_LASERON, sizeof(sdl_event_laseron_t), &laseron_evt)!=CS_E_OK)
                return;
            //oam_build_ctc_lao_report(0);    /*Send warning.*/
            prev_lao_status = 1;
        }
        else if(!status && prev_lao_status)
        {
            sdl_event_laseron_t laseron_evt;
            laseron_evt.laseron=0;
            if(sdl_event_send(EPON_EVENT_PON_LASERON, sizeof(sdl_event_laseron_t), &laseron_evt)!=CS_E_OK)
                return;
            //oam_build_ctc_lao_report(1);    /*send resume*/
            prev_lao_status = 0;
        }
        else
            ;
        break;
    case GPIO_PIN3:
        if(status == 0)
        {
            restore_counter++;
            if(restore_counter >= 3000/GPIO_POLLING_TMR_INTERVAL)
            {
                cs_printf("Restore to factory\n");
                cs_thread_delay(10);
                db_restore_to_factory();
                iros_system_reset(RESET_TYPE_FORCE);
                restore_counter = 0;
            }
        }
        else
        {
            restore_counter = 0;
        }
        break;
    case GPIO_PIN4:
        if(!ups_valid)
            break;
        if(status&& (!prev_ups_volt_status))
        {
            /*send alarm*/
            #ifdef HAVE_CTC_OAM
            ctc_oam_build_alarm_event(CTC_ONU_ALARM, 0, CTC_ALARM_BATT_VOLTLOW,
                                                       CTC_ALARM_RAISE, 0);
            #endif
            cs_led_proc(LED_UPS, LED_GREEN,LED_FAST_TWINKLE);
            prev_ups_volt_status = 1;
        }
        else if((!status) && prev_ups_volt_status)
        {
            /*send resume*/
            #ifdef HAVE_CTC_OAM
            ctc_oam_build_alarm_event(CTC_ONU_ALARM, 0, CTC_ALARM_BATT_VOLTLOW,
                                                       CTC_ALARM_CLEAR, 0);
            #endif
            cs_led_proc(LED_UPS, LED_GREEN,LED_ON);
            prev_ups_volt_status = 0;
        }
        else
            ;
        break;
    case GPIO_PIN5:
        if(status == battery_log[0] && !prev_ac_status)
        {
            /*send alarm*/
            #ifdef HAVE_CTC_OAM
            ctc_oam_build_alarm_event(CTC_ONU_ALARM, 0, CTC_ALARM_POWER,
                                                       CTC_ALARM_RAISE, 0);
            #endif
            prev_ac_status = 1;
        }
        else if(status != battery_log[0] && prev_ac_status)
        {
            /*send resume*/
            #ifdef HAVE_CTC_OAM
            ctc_oam_build_alarm_event(CTC_ONU_ALARM, 0, CTC_ALARM_POWER,
                                                       CTC_ALARM_CLEAR, 0);
            #endif
            prev_ac_status = 0;
        }
        else
            ;
        break;
    case GPIO_PIN7:
        if(status == battery_log[1] && !prev_ups_status)
        {
            cs_led_proc(LED_UPS, LED_GREEN, LED_OFF);
            #ifdef HAVE_CTC_OAM
            ctc_oam_build_alarm_event(CTC_ONU_ALARM, 0, CTC_ALARM_BATT_MISSING,
                                                       CTC_ALARM_RAISE, 0);
            #endif
            ups_valid = 0;
            prev_ups_status = 1;
        }
        else if(status != battery_log[1] && prev_ups_status)
        {
            #ifdef HAVE_CTC_OAM
            ctc_oam_build_alarm_event(CTC_ONU_ALARM, 0, CTC_ALARM_BATT_MISSING,
                                                       CTC_ALARM_CLEAR, 0);
            #endif
            ups_valid = 1;
            prev_ups_status = 0;
        }
        else
            ;
        break;
    case GPIO_PIN8:
        if(status == battery_log[3] && !prev_a_status)
        {
            //oam_build_ctc_dry_contactA_alarm(1);
            prev_a_status = 1;
        }
        else if(status != battery_log[3] && prev_a_status)
        {
            //oam_build_ctc_dry_contactA_alarm(0);
            prev_a_status = 0;
        }
        else
            ;
        break;
    case GPIO_PIN9:
        if(status == battery_log[4] && !prev_b_status)
        {
            //oam_build_ctc_dry_contactB_alarm(1);
            prev_b_status = 1;
        }
        else if(status != battery_log[4] && prev_b_status)
        {
            //oam_build_ctc_dry_contactB_alarm(0);
            prev_b_status = 0;
        }
        else
            ;
        break;
    default:
        break;
    }
    return;
}

void cs_gpio_org_init(void)
{
    cs_callback_context_t context;

		cs_printf("line 363 reset via GPIO 2\n");
		cs_plat_gpio_write(context, 0, 0, 2, 0);
		hal_delay_us(50000);
		cs_plat_gpio_write(context, 0, 0, 2, 1);

    if(CS_OK != startup_config_read(CFG_ID_DYING_GASP_NUM, 1, &dying_gasp_cnt))
    {
        dying_gasp_cnt = 3;
    }


}

void cs_gpio_app_hook(void)
{
    cs_uint8 hasBattery = 0;
    cs_uint8 batteryStatus = 1;
    cs_callback_context_t context;
    
    if(startup_config_read(CFG_ID_BAK_BATTERY_MODE,1 , &hasBattery)==CS_OK)
    {
        cs_plat_gpio_read(context, 0,0, GPIO_PIN7, &batteryStatus);
        if((hasBattery == 1) && (!batteryStatus))
        {
            cs_printf("UPS Detect! \n");
            ups_valid = 1;

        }

    }
	
    cs_gpio_trigger_reg(DYING_GASP, cs_gpio_dying_gasp);
    //cs_gpio_handler_reg(GPIO_PIN2, cs_gpio_pin_polling);
    cs_gpio_handler_reg(GPIO_PIN3, cs_gpio_pin_polling);
    cs_gpio_handler_reg(GPIO_PIN4, cs_gpio_pin_polling);
    cs_gpio_handler_reg(GPIO_PIN5, cs_gpio_pin_polling);
    cs_gpio_handler_reg(GPIO_PIN7, cs_gpio_pin_polling);
    cs_gpio_handler_reg(GPIO_PIN8, cs_gpio_pin_polling);
    cs_gpio_handler_reg(GPIO_PIN9, cs_gpio_pin_polling);

}

void cs_led_all_set(cs_uint8 led_action)
{

    if(led_action == LED_NORMAL)
    {
        cs_callback_context_t context;
        cs_boolean pon_link=0;
        epon_request_onu_pon_link_get(context, 0, 0, &pon_link);

        if(pon_link)
            cs_led_proc(LED_PON, LED_GREEN,LED_ON);
        else
            cs_led_proc(LED_PON, LED_GREEN,LED_OFF);
        
        cs_led_los_handle();
        
        cs_led_proc(LED_RUN, LED_GREEN,LED_TWINKLE);

        if(!ups_valid)
            cs_led_proc(LED_UPS, LED_GREEN,LED_OFF);
        
    }
    else
    {
        cs_led_proc(LED_LOS, LED_RED,  led_action);
        cs_led_proc(LED_PON, LED_GREEN,led_action);
        cs_led_proc(LED_RUN, LED_GREEN,led_action);
        cs_led_proc(LED_UPS, LED_GREEN,led_action);
    }

    return;
}

