#include "cs_types.h"
#include "plat_config.h"
#include "osal_common.h"

#ifdef HAVE_CTC_OAM
typedef struct
{
    void (*handler)();   
}cs_polling_t;

#define CS_MAX_POLLING_NUM 4
cs_polling_t poll_lst[CS_MAX_POLLING_NUM];
void cs_polling_thread(cs_uint32 data)
{
    cs_int32 i;

    for(i = 0 ; i < CS_MAX_POLLING_NUM; i++)
    {
        poll_lst[i].handler = NULL;
    }
    
    while(1)
    {
        cs_thread_delay(100);
        for(i = 0 ; i < CS_MAX_POLLING_NUM; i++)
        {
            if(poll_lst[i].handler != NULL)
            {
                poll_lst[i].handler();
            }
        }
    }

    return;
}

void cs_polling_init()
{
    cs_uint32 polling_thread_id = 0;
    cs_thread_create(&polling_thread_id, POLLING_THREAD_NAME, cs_polling_thread, NULL, POLLING_THREAD_STACKSIZE, POLLING_THREAD_PRIORITY, 0);
}

void cs_polling_handler_reg
    (
    void (*handler)()
    )
{
    cs_int32 i;

    for(i = 0 ; i < CS_MAX_POLLING_NUM; i++)
    {
        if(poll_lst[i].handler == NULL)
        {
            poll_lst[i].handler = handler;
            return;
        }
    }

    return;
}
#else
void cs_polling_init()
{

}

void cs_polling_handler_reg
    (
    void (*handler)()
    )
{

}
#endif
