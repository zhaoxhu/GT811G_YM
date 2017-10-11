/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#ifndef _TIMER_H_
#define _TIMER_H_

#include "cs_types.h"

/* assume 1 day at 1ms tick */
#define EPON_TICK_DIFF_MAX      (1000 * 60 * 60 * 24)

struct epon_timer_t;

typedef void (* epon_timer_func_t) (struct epon_timer_t *timer);

/* currently, we implemented timer based sorted absolution tick to
   expire. we may implement delta timer in the future */
struct epon_timer_t {
        /* the tick to expire */
        cs_uint32                   tick;
        /* timer double link list */
        struct epon_timer_t             *prev;
        struct epon_timer_t             *next;
        /* timer function */
        epon_timer_func_t               tmfunc;
        void                            * opaque;
};

typedef struct epon_timer_t epon_timer_t;

void epon_timer_add(epon_timer_t *, epon_timer_func_t, cs_uint32);
void epon_timer_add_with_jitter(epon_timer_t *, epon_timer_func_t,
        cs_uint32);
void epon_timer_del(epon_timer_t *);
void epon_timer_tick();

#endif
