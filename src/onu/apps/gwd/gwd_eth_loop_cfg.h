#ifndef __GWD_LOOP_CFG_H__
#define __GWD_LOOP_CFG_H__

#include "app_oam_gwd.h"
#include <string.h>
#include "sdl_vlan.h"
#include "sdl.h"
#include "gwd_types.h"
#if 1
//外部配置
//#define TEST	1	//打印调试开关

#define	VOS_LITTLE_ENDIAN 1

#define CPU_PKT_TO_SPECIFIC_UNI_PORT	1	// 1:芯片支持向指定的uni 口发送报文; 0:不支持

//mac 地址长度
#define GWD_MACADDR_LEN	CS_MACADDR_LEN

//uni 口数量
#define GWD_UNI_PORT_NUM	UNI_PORT_MAX

//单个端口的最大vlan数(包括默认vlan)
#define PER_PORT_MAX_VLAN	(__VLAN_RULE_PER_PORT_MAX+1)	//端口的所有vlan 数量，包括default vlan

//环路测试报文标识
#define GWD_ETH_TYPE_LOOP_DETECT (0x0800)
#define GWD_LOOP_DETECT_CHECK (0x0080)

//调试打印函数
#define gwd_printf cs_printf


typedef int gwd_olt_type_t;

typedef cs_port_id_t gwd_port_id_t;

typedef cs_node gw_node;

typedef cs_list gw_list;



#endif	/*end of extern config*/


#endif	/*end of file*/


