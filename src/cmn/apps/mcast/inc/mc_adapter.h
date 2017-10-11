#ifndef _MC_ADAPTER_H
#define _MC_ADAPTER_H

#include <stdio.h>
#include <string.h>

typedef enum {
    IROS_LOG_LEVEL_DBG0 = 0,
    IROS_LOG_LEVEL_DBG1 = 1,
    IROS_LOG_LEVEL_DBG2 = 2,
    IROS_LOG_LEVEL_DBG3 = 3,
    IROS_LOG_LEVEL_INF = 4,
    IROS_LOG_LEVEL_MIN = 5,
    IROS_LOG_LEVEL_MAJ = 6,
    IROS_LOG_LEVEL_CRI = 7,
    MAX_LOG_LEVEL
} SYS_LOG_LEVEL;

#define MC_LOG_MID_IGMP     15

/*
 * adapt APIs
 */
 
void * mc_malloc(cs_uint32 size);
void mc_free(void * ptr);
cs_uint8  mc_device_port_num_get(cs_dev_id_t dev);
cs_uint16 mc_device_grp_cap(cs_dev_id_t dev);
cs_uint16 mc_device_port_grp_cap(cs_dev_id_t dev);
cs_status mc_ctrl_enable(cs_dev_id_t dev, cs_boolean enable);
cs_status mc_timer_init(mc_tmr_handler_t hdl);
cs_status mc_frame_send(cs_dev_id_t dev, cs_port_id_t port_id, cs_uint8 *frame, cs_uint16 len);
cs_status  mc_port_fdb_add(mc_fdb_entry_t *entry);
cs_status  mc_port_fdb_del(mc_fdb_entry_t *entry);
cs_status  mc_port_fdb_update(mc_fdb_entry_t *old_entry, mc_fdb_entry_t *new_entry);

/*
 * OSAL APIs
 */
cs_uint64 cs_current_time(void);
void cs_printf( const cs_int8 *String, ...);
int cs_mutex_init(unsigned int *mut_id, const char *mut_name, unsigned int options);
int cs_mutex_lock(unsigned int mut_id);
int cs_mutex_unlock(unsigned int mut_id);
void cs_log_msg(cs_uint32 level , cs_uint32 module_id , const cs_int8 *String, ...);
#define MC_LOG(level,String...)     cs_log_msg(level, MC_LOG_MID_IGMP, String)

#endif /* #ifndef _MC_ADAPTER_H */

