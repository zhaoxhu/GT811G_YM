#define _GPIO_DESC_
#include "cs_types.h"
#include "plat_common.h"
#include "sdl_gpio.h"
#include "app_gpio.h"
#include "sdl_api.h"
#include "registers.h"
#include "aal_util.h"

cs_gpio_pin_t gpio_pin[CS_GPIO_MAX_NUM];

void cs_gpio_app_start(void);

static void gpio_pin_desc_parser(cs_int8 *desc, cs_int32 *argc, cs_int8 **argv)
{
    cs_int8 *c = NULL;
    cs_int32 parse_status = 0;
    cs_int32 cnt = 0;

    c = desc;
    while(c != '\0')
    {
        if(parse_status == 0)
        {
            if(*c == '.')
            {
                cnt = 0;
                return;
            }

            argv[cnt++] = c;
            parse_status = 1;
            c++;
            continue;
        }

        if(parse_status == 1 && *c == '.')
        {
            *c = '\0';
            parse_status = 0;
        }

        if(parse_status == 1 && *(c+1) == '\0')
        {
            break;
        }

        c++;
    }
    *argc = cnt;
    return;
}

static cs_int32 gpio_pin_type_get(cs_int32 argc, cs_int8 **argv)
{
    cs_int32 desc_num;

    for(desc_num = 0; desc_num < PIN_TYPE_MAX; desc_num++)
    {
        if(cmd_cmp(argv[0], pin_desc[desc_num].type_str))
        {
            return pin_desc[desc_num].pin_type;
        }
    }

    return INVALID_PIN_TYPE;
}

static void led_type_desc_parser(cs_gpio_pin_t *pPin, cs_int32 argc, cs_int8 **argv)
{
    cs_int32 i;
    
    if(argc != 5)
    {
        pPin->pin_type = INVALID_PIN_TYPE;
        return;
    }

    /*LED TYPE*/
    for(i = 0; i < LED_TYPE_MAX; i++)
    {
        if(cmd_cmp(led_desc[i].type_str, argv[1]))
        {
            pPin->pin_desc.led.led_type = led_desc[i].led_type;
            break;
        }
    }
    if(i == LED_TYPE_MAX)
    {
        pPin->pin_type = INVALID_PIN_TYPE;
        return;
    }

    /*LED COLOR TYPE*/
    for(i = 0; i < LED_COLOR_MAX; i++)
    {
        if(cmd_cmp(color_desc[i].type_str, argv[2]))
        {
            pPin->pin_desc.led.led_color = color_desc[i].color_type;
            break;
        }
    }
    if(i == LED_COLOR_MAX)
    {
        pPin->pin_type = INVALID_PIN_TYPE;
        return;
    }

    /*LED ACTION TYPE*/
    for(i = 0; i < LED_ACTION_MAX; i++)
    {
        if(cmd_cmp(action_desc[i].type_str, argv[3]))
        {
            pPin->pin_desc.led.deflt_action = action_desc[i].action_type;
            break;
        }
    }
    if(i == LED_ACTION_MAX)
    {
        pPin->pin_type = INVALID_PIN_TYPE;
        return;
    }

    pPin->pin_desc.led.led_on_bit = (cs_atoi(argv[4])>0)?1:0;
    
    return;
}

static void output_type_desc_parser(cs_gpio_pin_t *pPin, cs_int32 argc, cs_int8 **argv)
{
    if(argc != 2)
    {
        pPin->pin_type = INVALID_PIN_TYPE;
        return;
    }

    pPin->pin_desc.ovalue = (cs_atoi(argv[1])>0)?1:0;
    return;
}

static void trigger_type_desc_parser(cs_gpio_pin_t *pPin, cs_int32 argc, cs_int8 **argv)
{
    cs_int32 i;
    
    if(argc != 3)
    {
        pPin->pin_type = INVALID_PIN_TYPE;
        return;
    }

    /*TRIGGER TYPE*/
    for(i = 0; i < TRIGGER_ACTION_MAX; i++)
    {
        if(cmd_cmp(trigger_desc[i].type_str, argv[1]))
        {
            pPin->pin_desc.trigger.trigger_type = trigger_desc[i].trigger_type;
            break;
        }
    }
    if(i == TRIGGER_ACTION_MAX)
    {
        pPin->pin_type = INVALID_PIN_TYPE;
        return;
    }

    /*TRIGGER POLARITY*/
    for(i = 0; i < TRIGGER_POLARITY_MAX; i++)
    {
        if(cmd_cmp(polarity_desc[i].type_str, argv[2]))
        {
            pPin->pin_desc.trigger.polarity = polarity_desc[i].polarity_type;
            break;
        }
    }
    if(i == TRIGGER_POLARITY_MAX)
    {
        pPin->pin_type = INVALID_PIN_TYPE;
        return;
    }

    return;
}

static void gpio_pin_init(cs_gpio_pin_t *pPin, cs_int8 *desc)
{
    cs_int32 argc = 0;
    cs_int8 *argv[16] = {NULL};
    cs_int32 pin_type = INVALID_PIN_TYPE;

    gpio_pin_desc_parser(desc, &argc, &argv[0]);
    if(argc == 0)
    {
        return;
    }

    pin_type = gpio_pin_type_get(argc, &argv[0]);
    pPin->pin_type = pin_type;
    switch(pin_type)
    {
    case LED_TYPE:
        led_type_desc_parser(pPin, argc, &argv[0]);
        break;
    case INPUT_TYPE:
        break;
    case OUTPUT_TYPE:
        output_type_desc_parser(pPin, argc, &argv[0]);
        break;
    case TRIGGER_TYPE:
        trigger_type_desc_parser(pPin, argc, &argv[0]);
        break;
    default:
        break;
    }
    
    return;
}

static cs_int8 *gpio_pin_type_str_get(cs_int32 type)
{
    cs_int32 i;

    for(i = 0 ; i < PIN_TYPE_MAX ; i++)
    {
        if(pin_desc[i].pin_type == type)
            return pin_desc[i].type_str;
    }

    return NULL;
}

static cs_int8 *led_type_str_get(cs_int32 type)
{
    cs_int32 i;

    for(i = 0 ; i < LED_TYPE_MAX ; i++)
    {
        if(led_desc[i].led_type == type)
            return led_desc[i].type_str;
    }

    return NULL;
}

static cs_int8 *led_color_type_str_get(cs_int32 type)
{
    cs_int32 i;

    for(i = 0 ; i < LED_COLOR_MAX ; i++)
    {
        if(color_desc[i].color_type == type)
            return color_desc[i].type_str;
    }

    return NULL;
}

static cs_int8 *led_action_type_str_get(cs_int32 type)
{
    cs_int32 i;

    for(i = 0 ; i < LED_ACTION_MAX ; i++)
    {
        if(action_desc[i].action_type == type)
            return action_desc[i].type_str;
    }

    return NULL;
}

static cs_int8 *trigger_action_type_str_get(cs_int32 type)
{
    cs_int32 i;

    for(i = 0 ; i < TRIGGER_ACTION_MAX ; i++)
    {
        if(trigger_desc[i].trigger_type == type)
            return trigger_desc[i].type_str;
    }

    return NULL;
}

static cs_int8 *polarity_type_str_get(cs_int32 type)
{
    cs_int32 i;

    for(i = 0 ; i < TRIGGER_POLARITY_MAX ; i++)
    {
        if(polarity_desc[i].polarity_type == type)
            return polarity_desc[i].type_str;
    }

    return NULL;
}

void cs_gpio_cfg_dump(void)
{
    cs_int32 i;
    cs_gpio_pin_t *pPin = NULL;
    cs_int8 tmp_buf[16] = {0};
    cs_uint8 status = 0;
    cs_callback_context_t context;
    
    cs_printf("%-4s%-8s%-10s%-10s%-13s%-9s%s\n","PIN","TYPE","LED_TYPE","LED_COLOR","ACTION","POLARITY","DEF_VALUE");
    for(i = 0; i < CS_GPIO_MAX_NUM; i++)
    {
        pPin = (cs_gpio_pin_t *)&gpio_pin[i];
        switch(pPin->pin_type)
        {
        case LED_TYPE:
            memset(tmp_buf, 0, sizeof(tmp_buf));
            sprintf(tmp_buf, "%s(%d)",gpio_pin_type_str_get(LED_TYPE),pPin->pin_desc.led.led_on_bit);
            cs_printf("%-4d%-8s%-10s%-10s%-13s%-9s%s\n",i,
                tmp_buf,
                led_type_str_get(pPin->pin_desc.led.led_type),
                led_color_type_str_get(pPin->pin_desc.led.led_color),
                led_action_type_str_get(pPin->pin_desc.led.deflt_action),
                "--",
                "--");
            break;
        case INPUT_TYPE:
            cs_plat_gpio_read(context, 0, 0, i, &status);
            cs_printf("%-4d%-8s%-10s%-10s%-13s%-9s%d\n",i,
                gpio_pin_type_str_get(INPUT_TYPE),
                "--","--","--","--",status);
            break;
        case OUTPUT_TYPE:
            cs_printf("%-4d%-8s%-10s%-10s%-13s%-9s%d\n",i,
                gpio_pin_type_str_get(OUTPUT_TYPE),
                "--","--","--","--",
                pPin->pin_desc.ovalue);
            break;
        case TRIGGER_TYPE:
            cs_printf("%-4d%-8s%-10s%-10s%-13s%-9s%-s\n",i,
                gpio_pin_type_str_get(TRIGGER_TYPE),
                "--","--",
                trigger_action_type_str_get(pPin->pin_desc.trigger.trigger_type),
                polarity_type_str_get(pPin->pin_desc.trigger.polarity),
                "--");
            break;
        default:
            cs_printf("%-4d%-8s%-10s%-10s%-13s%-9s%s\n",i,
                "--","--","--","--","--","--");
            break;
        }
    }

    return;
}

void cs_gpio_init(void)
{
    cs_int32 i;
    cs_int32 cfg_id = 0;
    cs_int8 buff[64];

    for(i = 0; i < CS_GPIO_MAX_NUM; i++)
    {
        memset(&gpio_pin[i], 0, sizeof(cs_gpio_pin_t));
    }

    for(i = 0; i < CS_GPIO_MAX_NUM; i++)
    {
        cfg_id = CFG_ID_GPIO_PIN0 + i;
        memset(buff, 0 , sizeof(buff));
        if(CS_OK != startup_config_read(cfg_id, sizeof(buff), buff))
        {
            continue;
        }

        gpio_pin_init(&gpio_pin[i], buff);
    }

    cs_gpio_cfg_dump();

    /*GPIO application init*/
    cs_gpio_app_start();
    /*GPIO application init end*/

    return;
}

/****************************************************************************
*                                                                           *
*                         >> GPIO  APPLICATION <<                           *
*                                                                           *
****************************************************************************/
static void gpio_polling_timer_handler(void *data)
{
    cs_int32 i;
    cs_callback_context_t context;
    cs_led_t *pLed = NULL;

    for(i = 0; i < CS_GPIO_MAX_NUM; i++)
    {
        pLed = (cs_led_t *)&gpio_pin[i].pin_desc.led;
        switch(gpio_pin[i].pin_type)
        {
        case LED_TYPE:
            switch(pLed->action)
            {
            case LED_ON:
                if(pLed->current_action == LED_OFF)
                {
                    cs_plat_gpio_write(context,0,0,i,pLed->led_on_bit);
                    pLed->current_action = LED_ON;
                }
                break;
            case LED_OFF:
                if(pLed->current_action == LED_ON)
                {
                    cs_plat_gpio_write(context,0,0,i,1-pLed->led_on_bit);
                    pLed->current_action = LED_OFF;
                }
                break;
            case LED_TWINKLE:
            case LED_FAST_TWINKLE:
                if(pLed->current_circle < pLed->delay_circle)
                {
                    pLed->current_circle++;
                    break;
                }
                
                if(pLed->current_action == LED_OFF)
                {
                    cs_plat_gpio_write(context,0,0,i,pLed->led_on_bit);
                    pLed->current_action = LED_ON;
                }
                else if(pLed->current_action == LED_ON)
                {
                    cs_plat_gpio_write(context,0,0,i,1-pLed->led_on_bit);
                    pLed->current_action = LED_OFF;
                }
                else
                    ;

                pLed->current_circle = 0;
                break;
            default:
                break;
            }
            break;
        case INPUT_TYPE:
            if(gpio_pin[i].handler == NULL)
            {
                gpio_pin[i].handler = (void *)1;
                break;
            }
            if((cs_uint32)gpio_pin[i].handler != 1)
            {
                cs_uint8 status = 0;
                cs_plat_gpio_read(context, 0, 0, i, &status);
                gpio_pin[i].handler(i, status);
            }
            break;
        default:
            break;
        }
    }

    return;
}

static void cs_gpio_polling_timer_init(void)
{
    cs_int32 i;
    cs_int32 tmr_is_need = 0;

    for(i = 0; i < CS_GPIO_MAX_NUM; i++)
    {
        if(gpio_pin[i].pin_type == INPUT_TYPE ||
            gpio_pin[i].pin_type == LED_TYPE)
        {
            tmr_is_need = 1;
            break;
        }
    }

    if(tmr_is_need == 0)
    {
        return;
    }

    if(cs_circle_timer_add(GPIO_POLLING_TMR_INTERVAL,
                gpio_polling_timer_handler, NULL) == CS_INVALID_TIMER)
    {
        cs_printf("Start gpio polling timer failed.\n");
        return;
    }
    
    return;
}

static void cs_gpio_trigger_handler(void *data)
{
    cs_uint32 pin = 0;
    pin = (cs_uint32)data;
    if(pin >= CS_GPIO_MAX_NUM)
        return;

    if(gpio_pin[pin].handler)
        gpio_pin[pin].handler(pin, 0);
    return;
}

static void cs_gpio_pin_mode_init(void)
{
    cs_int32 i;
    cs_callback_context_t context;
    cs_led_t *pLed = NULL;

    for(i = 0; i < CS_GPIO_MAX_NUM; i++)
    {
        switch(gpio_pin[i].pin_type)
        {
        case LED_TYPE:
            pLed = (cs_led_t *)&gpio_pin[i].pin_desc.led;
            cs_plat_gpio_mode_set(context, 0, 0, i, GPIO_MODE_OUTPUT);
            pLed->action = pLed->deflt_action;
            if(pLed->action == LED_ON)
            {
                cs_plat_gpio_write(context, 0, 0, i, pLed->led_on_bit);
                pLed->current_action = LED_ON;
            }
            if(pLed->action == LED_OFF)
            {
                cs_plat_gpio_write(context, 0, 0, i, 1-pLed->led_on_bit);
                pLed->current_action = LED_OFF;
            }
            if(pLed->action == LED_TWINKLE)
            {
                pLed->delay_circle = TWINKLE_CIRCLE;
                pLed->current_circle = 0;
                cs_plat_gpio_write(context, 0, 0, i, pLed->led_on_bit);
                pLed->current_action = LED_ON;
            }
            if(pLed->action == LED_FAST_TWINKLE)
            {
                pLed->delay_circle = FAST_TWINKLE_CIRCLE;
                pLed->current_circle = 0;
                cs_plat_gpio_write(context, 0, 0, i, pLed->led_on_bit);
                pLed->current_action = LED_ON;
            }
            break;
        case INPUT_TYPE:
            cs_plat_gpio_mode_set(context, 0, 0, i, GPIO_MODE_INPUT_POLL);
            break;
        case OUTPUT_TYPE:
            /*Output mode, set data first*/
            cs_plat_gpio_write(context, 0, 0, i, gpio_pin[i].pin_desc.ovalue);
            cs_plat_gpio_mode_set(context, 0, 0, i, GPIO_MODE_OUTPUT);
            break;
        case TRIGGER_TYPE:
            if(gpio_pin[i].pin_desc.trigger.polarity == FALLING_EDGE)
            {
                cs_plat_gpio_mode_set(context, 0, 0, i, GPIO_MODE_INPUT_INTR_FALLING);
            }

            if(gpio_pin[i].pin_desc.trigger.polarity == RISING_EDGE)
            {
                cs_plat_gpio_mode_set(context, 0, 0, i, GPIO_MODE_INPUT_INTR_RISING);
            }
            cs_plat_gpio_intr_disable(context, 0, 0, i);
            cs_plat_gpio_intr_handler_set(context, 0, 0, i, (void *)cs_gpio_trigger_handler, (void *)i);
            cs_plat_gpio_intr_enable(context, 0, 0, i);
            break;
        default:
            break;
        }
    }

    return;
}

cs_int32 cs_gpio_cmd_handler(cs_int32 argc, cs_int8 **argv)
{
    if(argc == 2 && cmd_cmp(argv[0], "gpio") && cmd_cmp(argv[1],"show"))
    {
        cs_gpio_cfg_dump();
        return 1;
    }

    return 0;
}

void cs_gpio_app_start(void)
{
    cs_gpio_pin_mode_init();
    cs_gpio_polling_timer_init();
    cs_gpio_org_init();
}

void cs_gpio_handler_reg(cs_int32 pin, void (*handler)(cs_int32 , cs_uint8 ))
{
    if(handler == NULL)
        return;

    if(pin < 0 || pin >= CS_GPIO_MAX_NUM)
        return;

    gpio_pin[pin].handler = handler;

    return;
}

void cs_gpio_trigger_reg(cs_int32 trigger_type, void (*handler)(cs_int32 , cs_uint8 ))
{
    cs_int32 pin;
    
    if(handler == NULL)
        return;

    for(pin = 0; pin < CS_GPIO_MAX_NUM; pin++)
    {
        if(gpio_pin[pin].pin_type != TRIGGER_TYPE)
        {
            continue;
        }

        if(gpio_pin[pin].pin_desc.trigger.trigger_type == trigger_type)
        {
            gpio_pin[pin].handler = handler;
        }
    }

    return;
}

void cs_led_proc(cs_uint8 led_type, cs_uint8 led_color, cs_uint8 led_action)
{
    cs_int32 i;
    cs_led_t *pLed = NULL;
    cs_int32 led_pin[2] = {0,0};
    cs_int32 led_cnt = 0;
    cs_int32 led=0;

    for(i = 0 ; i < CS_GPIO_MAX_NUM; i++)
    {
        if(gpio_pin[i].pin_type != LED_TYPE)
        {
            continue;
        }

        pLed = (cs_led_t *)&gpio_pin[i].pin_desc.led;
        if(pLed->led_type != led_type)
        {
            continue;
        }
        if(led_cnt < 2)
            led_pin[led_cnt++] = i;
        else
            break;
    }

    if(led_cnt == 0)
    {
        return;
    }

    cs_led_fast_twinkle(FALSE, led_type, led_color);

    if(led_action == LED_HW_TWINKLE)
    {
        led_action = LED_OFF;
        if(cs_led_fast_twinkle(TRUE, led_type, led_color)!=CS_E_OK)
            led_action = LED_FAST_TWINKLE;
    }

    if(led_cnt == 1)
    {
        led = led_pin[0];
        pLed = (cs_led_t *)&gpio_pin[led].pin_desc.led;
        if(pLed->action == led_action)
            return;
        pLed->action = led_action;
        pLed->current_circle = 0;
        if(led_action == LED_TWINKLE)
        {
            pLed->delay_circle = TWINKLE_CIRCLE;
        }

        if(led_action == LED_FAST_TWINKLE)
        {
            pLed->delay_circle = FAST_TWINKLE_CIRCLE;
        }
    }
    else
    {
        cs_int32 led_pin0=0,led_pin1=0;
        cs_led_t *pLed0 = NULL, *pLed1 = NULL;
        led_pin0 = led_pin[0];
        led_pin1 = led_pin[1];
        pLed0 = (cs_led_t *)&gpio_pin[led_pin0].pin_desc.led;
        pLed1 = (cs_led_t *)&gpio_pin[led_pin1].pin_desc.led;
        if(pLed0->led_color == led_color && 
            pLed1->led_color != led_color)
        {
            pLed1->action = LED_OFF;
            if(pLed0->action == led_action)
                return;
            pLed0->action = led_action;
            pLed0->current_circle = 0;
            if(led_action == LED_TWINKLE)
            {
                pLed0->delay_circle = TWINKLE_CIRCLE;
            }

            if(led_action == LED_FAST_TWINKLE)
            {
                pLed0->delay_circle = FAST_TWINKLE_CIRCLE;
            }
        }
        else if(pLed1->led_color == led_color && 
            pLed0->led_color != led_color)
        {
            pLed0->action = LED_OFF;
            if(pLed1->action == led_action)
                return;
            pLed1->action = led_action;
            pLed1->current_circle = 0;
            if(led_action == LED_TWINKLE)
            {
                pLed1->delay_circle = TWINKLE_CIRCLE;
            }

            if(led_action == LED_FAST_TWINKLE)
            {
                pLed1->delay_circle = FAST_TWINKLE_CIRCLE;
            }
        }
        else
        {
            pLed0->action = led_action;
            pLed1->action = led_action;
            pLed0->current_circle = 0;
            pLed1->current_circle = 0;
            if(led_action == LED_TWINKLE)
            {
                pLed0->delay_circle = TWINKLE_CIRCLE;
                pLed1->delay_circle = TWINKLE_CIRCLE;
            }

            if(led_action == LED_FAST_TWINKLE)
            {
                pLed0->delay_circle = FAST_TWINKLE_CIRCLE;
                pLed1->delay_circle = FAST_TWINKLE_CIRCLE;
            }
        }
    }

    return;
}

void cs_led_los_handle()
{
    cs_callback_context_t context;
    cs_uint8 tx_pwr=0;
    cs_boolean pon_link;
    cs_sdl_port_loopback_t loopback = 0;
    epon_request_onu_pon_link_get(context, 0, 0, &pon_link);

    cs_gpio_pon_tx_get(context, 0, 0,&tx_pwr);
    
    /*cs_printf("cs_led_los los %d pon %d loop %d tx_pwr %d\n",los,pon_link,loopback,tx_pwr);*/
    
    if((loopback)||(!tx_pwr))
        cs_led_proc(LED_LOS, LED_RED, LED_ON); 
    else if(!pon_link)
        cs_led_proc(LED_LOS, LED_RED, LED_TWINKLE); 
    else
        cs_led_proc(LED_LOS, LED_RED, LED_OFF); 

}


/*Led ID(0-5) map to GPIO(10-15) */
cs_status cs_led_fast_twinkle(cs_uint8 enable, cs_uint8 led_type, cs_uint8 led_color)
{
    LED_CTRL_t led_ctrl;
    LED_CFG_0_t led_cfg;
    GLOBAL_CTRL_t glb_ctrl;
    LED_LOAD_t led_load;
    cs_int32 i;
    cs_led_t *pLed = NULL;
    cs_int32 led_pin = 0;
    #if 1
	//added by zhuxh
	int led_found = 0;
	#endif
	
    for(i = 0 ; i < CS_GPIO_MAX_NUM; i++)
    {
        if(gpio_pin[i].pin_type != LED_TYPE)
        {
            continue;
        }

        pLed = (cs_led_t *)&gpio_pin[i].pin_desc.led;
        if((pLed->led_type == led_type)&&(pLed->led_color == led_color))
        {
        	#if 1
			//added by zhuxh
			led_found = 1;
			#endif
            led_pin = i;
            break;
        }
    }

	#if 1
	//added by zhuxh
	if(0 == led_found)
	{
		return CS_E_RESOURCE;
	}
	#endif
	
    if(led_pin<10)
    {
        cs_printf("Led function unsupported for GPIO%d \n",led_pin);
        return CS_E_RESOURCE;
    }

    /*Map to led0-5*/
    led_pin = led_pin - 10;
    REG_READ(LED_LOAD,led_load.wrd);
    REG_READ(GLOBAL_CTRL,glb_ctrl.wrd);
    
    if(enable)
    {
        REG_READ(LED_CTRL,led_ctrl.wrd);
        led_ctrl.bf.off_val |= 1<<led_pin;
        led_ctrl.bf.mode = 0;
        led_cfg.wrd = 0;
        led_cfg.bf.on_tim_ld = 0x20;
        led_cfg.bf.off_tim_ld = 0x20;
        REG_WRITE(LED_CTRL,led_ctrl.wrd);
        REG_WRITE((LED_CFG_0+led_pin),led_cfg.wrd);
        led_load.bf.enb |= 1<<led_pin;
        glb_ctrl.bf.led_gpio_sel |= 1<<led_pin;
    }
    else
    {

        led_load.bf.enb &= (~(1<<led_pin));
        glb_ctrl.bf.led_gpio_sel &= (~(1<<led_pin));

    }
    
    REG_WRITE(GLOBAL_CTRL,glb_ctrl.wrd);
    REG_WRITE(LED_LOAD,led_load.wrd);

    return CS_E_OK;
}


