#ifndef _MC_TIMER_H
#define _MC_TIMER_H

#include "mc_type.h"

typedef enum {
    MC_TIMER_STOP_FLAG,   
    MC_TIMER_START_FLAG,
    MC_TIMER_DELAY_STOP_FLAG 
} mc_timer_flag_t;

typedef struct  {
    cs_uint8   static_flag;
    cs_uint8   active_flag;
    cs_uint16  age;
} mc_age_t;


void mc_timer_age(mc_age_t *tmr);
cs_boolean mc_timer_is_aged(mc_age_t *tmr);
void mc_timer_set(mc_age_t *timer, cs_uint16  age, cs_uint16  oper_flag);
void mc_timer_set_static(mc_age_t *timer, cs_uint8 static_flag);

#endif /* #ifndef _MC_TIMER_H */

