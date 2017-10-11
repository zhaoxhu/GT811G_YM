/* Copyright: (c) 2005 ImmenStar Inc. All Rights Reserved. */

#include <stdio.h>
#include <stdlib.h>
#if defined (APPS_ONU) || defined (APPS_OLT)
#include <cyg/kernel/kapi.h>
#endif
#include "timer.h"
#include "cs_types.h"

static cs_uint32 jiffies = 0;
static epon_timer_t *timeq = NULL;

#if defined (APPS_ONU) || defined (APPS_OLT)
cyg_mutex_t timerMutex;
#endif

/* check if t2 is greater than t1, considering wrap-around */
static cs_boolean
epon_timer_tick_greater(cs_uint32 t1, cs_uint32 t2)
{
        if ((t1 < t2) || ((t1 - t2) > EPON_TICK_DIFF_MAX)) {
                return EPON_TRUE;
        }
        else {
                return EPON_FALSE;
        }
}

cs_uint32 epon_timer_getTick()
{
	return jiffies;
}


/* add a one-shot timer to the time delta queue */
void
epon_timer_add(epon_timer_t *timer, epon_timer_func_t tmfunc,
               cs_uint32 interval)
{
#if defined (APPS_ONU) || defined (APPS_OLT)
        cyg_mutex_lock(&timerMutex);
#endif
        epon_timer_t *prev = NULL;
        epon_timer_t *curr = timeq;

        if (interval == 0) {
#if defined (APPS_ONU) || defined (APPS_OLT)
                cyg_mutex_unlock(&timerMutex);
#endif
                return;
        }

        if (timer->prev || timer->next) {
#if defined (APPS_ONU) || defined (APPS_OLT)
                cyg_mutex_unlock(&timerMutex);
#endif
                return;
        }

        timer->tmfunc = tmfunc;
        timer->tick = jiffies + interval;

        while (curr && (epon_timer_tick_greater(curr->tick, timer->tick))) {
                prev = curr;
                curr = curr->next;
        }

        if (prev == NULL) {
                timeq = timer;
        }
        else {
                prev->next = timer;
        }

        timer->prev = prev;
        timer->next = curr;

        if (curr) {
                curr->prev = timer;
        }
#if defined (APPS_ONU) || defined (APPS_OLT)
        cyg_mutex_unlock(&timerMutex);
#endif
}

void
epon_timer_add_with_jitter(epon_timer_t *timer, epon_timer_func_t tmfunc,
               cs_uint32 interval)
{
#if defined (APPS_ONU) || defined (APPS_OLT)
    cyg_mutex_lock(&timerMutex);
#endif
    epon_timer_t *prev = NULL;
    epon_timer_t *curr = timeq;

    if (interval < 2) {
#if defined (APPS_ONU) || defined (APPS_OLT)
        cyg_mutex_unlock(&timerMutex);
#endif
        return;
    }

    if (timer->prev || timer->next) {
#if defined (APPS_ONU) || defined (APPS_OLT)
        cyg_mutex_unlock(&timerMutex);
#endif
        return;
    }

    timer->tmfunc = tmfunc;
    timer->tick = jiffies + interval - 1; /* Jitter */

    while (curr && (epon_timer_tick_greater(curr->tick, timer->tick))) {
        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL) {
        timeq = timer;
    }
    else {
        prev->next = timer;
    }

    timer->prev = prev;
    timer->next = curr;

    if (curr) {
        curr->prev = timer;
    }
#if defined (APPS_ONU) || defined (APPS_OLT)
    cyg_mutex_unlock(&timerMutex);
#endif
}

/* delete a timer from the time delta queue */
void
epon_timer_del(epon_timer_t *timer)
{
#if defined (APPS_ONU) || defined (APPS_OLT)
        cyg_mutex_lock(&timerMutex);
#endif
        if ((timer->prev == NULL) && (timer->next == NULL) &&
            (timeq != timer)) {
#if defined (APPS_ONU) || defined (APPS_OLT)
                cyg_mutex_unlock(&timerMutex);
#endif
                return;
        }

        if (timer->prev) {
                timer->prev->next = timer->next;
        }
        else {
                timeq = timer->next;
        }

        if (timer->next) {
                timer->next->prev = timer->prev;
        }

        timer->prev = timer->next = NULL;
#if defined (APPS_ONU) || defined (APPS_OLT)
        cyg_mutex_unlock(&timerMutex);
#endif
}

/* process all timer queues */
void
epon_timer_tick()
{
        epon_timer_t *curr = timeq;

        jiffies++;

        while (curr && (curr->tick == jiffies)) {
                epon_timer_del(curr);
                curr->tmfunc(curr);
                curr = timeq;
        }
}

#if defined (APPS_ONU) || defined (APPS_OLT)
extern int diag_printf (const char *fmt, ...);
void
dump_time_list()
{
        epon_timer_t *curr = timeq;
        diag_printf("*** timer list ***\n");

        while(curr)
        {
                diag_printf("timer 0x%x\n", curr);
                curr = curr->next;
        }
}
#endif

