#ifndef	__GWD_MC_PROCESS_H__
#define __GWD_MC_PROCESS_H__
#include "cs_cmn.h"
#include "mc_core.h"

typedef enum
{
	GW_TVM_ENABLE = 1,
	GW_TVM_DISABLE = 2,
}GW_TVM_ENABLE_T;


//对外提供的接口
extern void gwd_mc_tvm_process(cs_pkt_t *pkt, mc_node_t *node);
extern int gw_igmp_tvm_pkt_proc( char *igmp_pkt, int *len, int port_id, int *vlan_oper, int *vlan_id_cur );
extern long GwOamTvmRequestRecv( void *pRequest_input );
extern int igmp_tvm_enable_status_get(GW_TVM_ENABLE_T *enable);
extern int igmp_control_table_init(void);
extern void igmp_control_table_dump(void);

#endif	/*end of the file*/
