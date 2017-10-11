#include "mc_core.h"
#include "mc_fdb.h"
#include "mc_grp.h"
#include "mc_src.h"
#include "mc_timer.h"
#include "mc_adapter.h"

void mc_timer_age(mc_age_t *tmr)
{
    if(!tmr->static_flag || tmr->active_flag == MC_TIMER_DELAY_STOP_FLAG) {
        if(tmr->age) {
            tmr->age--;
        }
        
        if(0 == tmr->age) {
            tmr->active_flag = MC_TIMER_STOP_FLAG;
        }
    }
}

cs_boolean mc_timer_is_aged(mc_age_t *tmr)
{
    return (MC_TIMER_STOP_FLAG == tmr->active_flag) ? TRUE : FALSE;
}

void mc_timer_set_static(mc_age_t *timer, cs_uint8 static_flag)
{
    timer->static_flag = static_flag;
}

void mc_timer_set(mc_age_t *timer, cs_uint16  age, cs_uint16  oper_flag)
{        
    
    if(oper_flag == MC_TIMER_DELAY_STOP_FLAG) {
        if(timer->active_flag != MC_TIMER_DELAY_STOP_FLAG && age < timer->age) {   
            MC_LOG(IROS_LOG_LEVEL_INF, "lower timer to %d\n", age);
            timer->age = age;
        }
        
        timer->active_flag = oper_flag;
    }
    else {
        timer->age = age;
        timer->active_flag = oper_flag;
    }
        
}

cs_status mc_group_timer_update( 
    mc_port_group_state_list_t *port_state , 
    mc_port_group_state_list_t *  new_port_state)
{
    mc_node_t  *mc_node = NULL;
    mc_port_t   *port = NULL;
    cs_list        *dst_list = NULL;
    mc_source_record_list_t *src = NULL;
    mc_source_record_list_t *out_src = NULL;
     
    if(port_state == NULL || new_port_state == NULL) {
        return CS_E_ERROR;
    }

    MC_LOG(IROS_LOG_LEVEL_INF, "[mc_group_timer_update]\n");

    port = (mc_port_t * )port_state->entry.port;
    mc_node = (mc_node_t * )port->mc_node;

    /* set group timer */
    port_state->entry.grp_timer.age = new_port_state->entry.grp_timer.age;
    port_state->entry.grp_timer.active_flag = new_port_state->entry.grp_timer.active_flag;        

    /* set src timer */
    dst_list = &port_state->entry.src_list;        
    cs_lst_scan(dst_list, src, mc_source_record_list_t *) {
        if(CS_E_ERROR !=
            mc_src_find (&new_port_state ->entry.src_list, src, &out_src)) {
            src->entry.timer.active_flag = out_src->entry.timer.active_flag;
            src->entry.timer.age = out_src->entry.timer.age;
        }
    }  
                
    return CS_E_OK;

}


