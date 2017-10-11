#include "plat_common.h"
#include "boot_seq.h"

static cs_init_seq_t cs_init_seq[CS_MAX_INIT_SEQ_PRI];

void cs_init_seq_init()
{
    int i , j;
    for(i = 0 ; i < CS_MAX_INIT_SEQ_PRI ; i++)
    {
        cs_init_seq[i].init_flag   = 0;
        cs_init_seq[i].registered  = 0;
        for(j = 0 ; j < CS_MAX_DEP_PRI ; j++)
            cs_init_seq[i].dep_pri[j] = -1;
    }
    
    return;
}

void cs_reg_init_seq(int pri, int dep_pri)
{
    int i;
    if(pri >= CS_MAX_INIT_SEQ_PRI || pri < 0 || dep_pri >= CS_MAX_INIT_SEQ_PRI || dep_pri < 0)
        return;

    if(pri == dep_pri)
        return;

    cs_init_seq[pri].init_flag     = 0;
    cs_init_seq[pri].registered    = 1;
    for(i = 0 ; i < CS_MAX_DEP_PRI ; i++)
    {
        if(cs_init_seq[pri].dep_pri[i] == -1 || (cs_init_seq[pri].dep_pri[i] == dep_pri))
        {
            cs_init_seq[pri].dep_pri[i] = dep_pri;
            return;
        }
    }

    cs_printf("Max dependent priority is 8\n");
    return;
}

void cs_init_seq_done(int pri)
{
    if(pri >= CS_MAX_INIT_SEQ_PRI || pri < 0)
        return;

    cs_init_seq[pri].init_flag     = 1;
    return;
}

void cs_init_seq_wait(int pri)
{
    int dep_pri = -1;
    int i,wait_flag=1;
    
    if(pri >= CS_MAX_INIT_SEQ_PRI || pri < 0)
        return;

    if(!cs_init_seq[pri].registered)
        return;
    
    for(i = 0 ; i < CS_MAX_DEP_PRI ; i++)
    {
        dep_pri = cs_init_seq[pri].dep_pri[i];
        if(dep_pri == -1)
            continue;

        if(dep_pri >= CS_MAX_INIT_SEQ_PRI || dep_pri < 0)
            continue;

        wait_flag = wait_flag * cs_init_seq[dep_pri].init_flag;
    }

    while(!wait_flag)
    {

        cs_thread_delay(10);
        wait_flag = 1;
        for(i = 0 ; i < CS_MAX_DEP_PRI ; i++)
        {
            dep_pri = cs_init_seq[pri].dep_pri[i];
            if(dep_pri == -1)
                continue;

            if(dep_pri >= CS_MAX_INIT_SEQ_PRI || dep_pri < 0)
                continue;

            wait_flag = wait_flag * cs_init_seq[dep_pri].init_flag;
        }
    }
    
    return;
}

