#include "mc_core.h"
#include "mc_mgmt.h"
#include "mc_devm.h"
#include "mc_fdb.h"
#include "mc_adapter.h"

void mc_static_timer_handler()
{
    MC_LOG(IROS_LOG_LEVEL_DBG0, "mc tmr heartbeat\n");
    
    mc_node_fdb_age(g_mc_dev);
}

cs_status mc_init()
{
    if(mc_dev_init()) {
        MC_LOG(IROS_LOG_LEVEL_CRI, "mc_init() fail!\n");
        return CS_E_ERROR;
    }

    mc_timer_init(mc_static_timer_handler);

    return CS_E_OK;
}

