#ifndef _APP_GPIO_H_
#define _APP_GPIO_H_

typedef enum
{
    INVALID_PIN_TYPE = 0,
    LED_TYPE,
    INPUT_TYPE,
    OUTPUT_TYPE,
    TRIGGER_TYPE,
    PIN_TYPE_MAX
}GPIO_PIN_TYPE_ENUM;

typedef enum
{
    LED_AUTH,
    LED_PON,
    LED_UNI,
    LED_PWR,
    LED_PON_LNK,
    LED_PON_ACT,
    LED_UPS,
    LED_RUN,
    LED_LOS,
    LED_PHONE1,
    LED_PHONE2,
    LED_LOOP,
    LED_TYPE_MAX
} LED_TYPE_ENUM;

typedef enum
{
    LED_YELLOW,
    LED_GREEN,
    LED_RED,
    LED_COLOR_MAX
} LED_COLOR_TYPE;

typedef enum
{
    LED_OFF,
    LED_ON,
    LED_TWINKLE,
    LED_FAST_TWINKLE,
    LED_NORMAL,
    LED_HW_TWINKLE,
    LED_ACTION_MAX
} LED_ACTION_TYPE;
#define TWINKLE_CIRCLE      3
#define FAST_TWINKLE_CIRCLE 1

typedef enum
{
    DYING_GASP,
    PON_TX_FAULT,
    TRIGGER_ACTION_MAX
} TRIGGER_ACTION_TYPE;

typedef enum
{
    FALLING_EDGE,
    RISING_EDGE,
    TRIGGER_POLARITY_MAX
} TRIGGER_POLARITY_TYPE;

#define GPIO_POLLING_TMR_INTERVAL 100  /*ms*/

typedef struct
{
    cs_int8 pin_type;
    cs_int8 type_str[8];
}pin_type_desc_t;

typedef struct
{
    cs_int8 led_type;
    cs_int8 type_str[8];
}led_type_desc_t;

typedef struct
{
    cs_int8 color_type;
    cs_int8 type_str[8];
}color_type_desc_t;

typedef struct
{
    cs_int8 action_type;
    cs_int8 type_str[16];
}led_action_type_desc_t;

typedef struct
{
    cs_int8 trigger_type;
    cs_int8 type_str[16];
}trigger_action_type_desc_t;

typedef struct
{
    cs_int8 polarity_type;
    cs_int8 type_str[16];
}trigger_polarity_type_desc_t;

typedef struct
{
    cs_uint8 led_type;
    cs_uint8 led_color;
    cs_uint8 deflt_action;
    cs_uint8 led_on_bit;
    cs_uint8 action;
    cs_uint8 current_action;
    cs_uint8 delay_circle;
    cs_uint8 current_circle;
}cs_led_t;

typedef struct
{
    cs_int8 pin_type;
    union {
        cs_led_t led;
        cs_uint8 ovalue;   /*Default output value*/
        struct {
        cs_uint8 polarity; /*Trigger mode polarity*/
        cs_uint8 trigger_type; 
        }trigger;
    }pin_desc;
    void (*handler)(cs_int32 gpio_pin, cs_uint8 status);
} cs_gpio_pin_t;

#ifdef _GPIO_DESC_
pin_type_desc_t pin_desc[PIN_TYPE_MAX] =
                {
                {INVALID_PIN_TYPE,  "INVALID"},
                {LED_TYPE,          "LED"},
                {INPUT_TYPE,        "INPUT"},
                {OUTPUT_TYPE,       "OUTPUT"},
                {TRIGGER_TYPE,      "TRIGGER"},
                };
led_type_desc_t led_desc[LED_TYPE_MAX] =
                {
                {LED_AUTH,          "AUTH"},
                {LED_PON,           "PON"},
                {LED_UNI,           "UNI"},
                {LED_PWR,           "PWR"},
                {LED_PON_LNK,       "PON_LINK"},
                {LED_PON_ACT,       "ACT"},
                {LED_UPS,           "UPS"},
                {LED_RUN,           "RUN"},
                {LED_LOS,           "LOS"},
                {LED_PHONE1,        "PHONE1"},
                {LED_PHONE2,        "PHONE2"},
                {LED_LOOP,        	"LOOP"},
                };
color_type_desc_t color_desc[LED_COLOR_MAX] =
                {
                {LED_YELLOW,        "YELLOW"},
                {LED_GREEN,         "GREEN"},
                {LED_RED,           "RED"},
                };
led_action_type_desc_t action_desc[LED_ACTION_MAX] =
                {
                {LED_OFF,           "OFF"},
                {LED_ON,            "ON"},
                {LED_TWINKLE,       "TWINKLE"},
                {LED_FAST_TWINKLE,  "FAST_TWINKLE"},
                };
trigger_action_type_desc_t trigger_desc[TRIGGER_ACTION_MAX] =
                {
                {DYING_GASP,        "DYING_GASP"},
                {PON_TX_FAULT,      "PON_TX_FAULT"},
                };
trigger_polarity_type_desc_t polarity_desc[TRIGGER_POLARITY_MAX] =
                {
                {FALLING_EDGE,      "FALLING"},
                {RISING_EDGE,       "RISING"},
                };
#else
extern pin_type_desc_t pin_desc[];
extern led_type_desc_t led_desc[];
extern color_type_desc_t color_desc[];
extern led_action_type_desc_t action_desc[];
extern trigger_action_type_desc_t trigger_desc[];
extern trigger_polarity_type_desc_t polarity_desc[];
#endif

extern void cs_gpio_handler_reg(cs_int32 pin, void (*handler)(cs_int32 , cs_uint8 ));
extern void cs_gpio_trigger_reg(cs_int32 trigger_type, void (*handler)(cs_int32 , cs_uint8 ));
extern void cs_led_proc(cs_uint8 led_type, cs_uint8 led_color, cs_uint8 led_action);
extern void cs_led_los_handle();
extern cs_status cs_led_fast_twinkle(cs_uint8 enable, cs_uint8 led_type, cs_uint8 led_color);
extern void cs_gpio_org_init(void);
extern void cs_gpio_app_hook(void);
extern void cs_led_all_set(cs_uint8 led_action);

#endif
