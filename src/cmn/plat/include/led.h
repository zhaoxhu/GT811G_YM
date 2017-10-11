#ifndef _LED_H_
#define _LED_H_

typedef void (*cs_led_handler_t)(cs_uint8, cs_uint8);

enum {
    ONU_LED_AUTH,
    ONU_LED_PON,
    ONU_LED_UNI,
    ONU_LED_PWR,
    ONU_LED_PON_LNK,
    ONU_LED_PON_ACT,
    ONU_LED_UPS,
    ONU_LED_RUN,
    ONU_LED_LOS,
    ONU_LED_END
} ;

#define ONU_LED_TWINKLE_PERIOD  200

/* LED REG */
#define ONU_LED_GLB_CTRL    0x2c30c00c
#define ONU_LED_CFG
#define ONU_LED_CUSTOME_CFG 0x2c310020

/* general LED actions */
#define    ONU_LED_TURN_OFF     0
#define    ONU_LED_TURN_ON      1
#define    ONU_LED_TWINKLE      2
#define    ONU_LED_TWINKLE_FAST 3

/* PON LED actions */
#define PON_LED_OFF               0x10
#define PON_LED_ON_GREEN    0x11
#define PON_LED_ON_RED          0x12
#define PON_LED_TWINKLE_YELLOW  0x13
#define PON_LED_ON_YELLOW  0x14

/* PWR LED actions */
#define PWR_LED_OFF               0x30
#define PWR_LED_ON_GREEN    0x31
#define PWR_LED_ON_RED          0x32
#define PWR_LED_TWINKLE_RED  0x33

#define PON_ACT_LED_DETECT_INTERVAL 500

/* double LED code */
enum {
    ONU_LED_YELLOW,
    ONU_LED_GREEN,
    ONU_LED_RED
};

cs_status onu_auth_led_init(cs_uint32 map);
cs_status onu_uni_led_init(cs_uint32 map);
cs_status onu_pon_led_init(cs_uint32 map);
cs_status onu_pwr_led_init(cs_uint32 map);
void onu_led_proc(cs_uint8 led, cs_uint8 action);
void onu_uni_led_enable(cs_uint8 enable);

cs_status onu_pon_lnk_led_init(cs_uint32 map);
cs_status onu_pon_act_led_init(cs_uint32 map);

#endif /* #ifndef _LED_H_ */
