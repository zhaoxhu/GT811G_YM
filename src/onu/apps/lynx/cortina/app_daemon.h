#ifndef _APP_DAEMON_H
#define _APP_DAEMON_H

typedef enum{
    DAEMON_DB_PERIOD_SYN_ID = 0,
    DAEMON_DB_FORCE_SYN_IMMEDIATE_ID ,
    DAEMON_UPGRADE_WIRTE_FLASH_ID ,
    DAEMON_UPGRADE_COMMIT_ID ,
    DAEMON_ONU_RESET_ID,
    DAEMON_TASK_MAX_ID
}app_daemon_task_id_e;

typedef struct{
    app_daemon_task_id_e task_id;
    void* data;
}app_daemon_msg_t;

typedef void (*app_daemon_task_handler)(void*);

void app_daemon_init(void);
cs_status app_daemon_msg_send(void* data, cs_uint32 size);
void app_daemon_thread(cyg_addrword_t data);
cs_status app_daemon_reg_task_handler(app_daemon_task_id_e  task_id, app_daemon_task_handler handler);
void app_daemon_queue_create();

#endif
