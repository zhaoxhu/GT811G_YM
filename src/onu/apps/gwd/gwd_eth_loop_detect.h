#ifndef __GWD_LOOP_DETECT_H__
#define __GWD_LOOP_DETECT_H__

#include "gwd_eth_loop_cfg.h"

#if 1
//外部可见

#ifndef GWD_IN
#define GWD_IN
#endif

#ifndef GWD_OUT
#define GWD_OUT
#endif

#ifndef GWD_IN_OUT
#define GWD_IN_OUT
#endif

typedef enum {
    GWD_E_OK               = 0,
    GWD_E_RESOURCE         = 1,
    GWD_E_PARAM            = 2,
    GWD_E_NOT_FOUND        = 3,
    GWD_E_CONFLICT         = 4,
    GWD_E_TIMEOUT          = 5,
    GWD_E_NOT_SUPPORT      = 6,
    GWD_E_ERROR            = 0xffffffff
} gwd_status;

typedef struct
{
	gw_uint8	enable;
	gw_uint16	vid;
	gw_uint8	smac[GWD_MACADDR_LEN];
	gw_uint16	interval;
	gw_uint16	policy;
	gw_uint16	waitforwakeup;
	gw_uint16	maxwakeup;
	gw_uint16	alarm_interval;			//端口环路后的告警间隔
	gw_uint16	clear_interval;			//指定周期内未发现环路则清除环路信息
	gw_uint16	opposite_aging_time;	//包含环路对端的信息的老化时间
}__attribute__((packed))loop_control_config_t;

typedef enum {
    GWD_DOWN_STREAM     = 0x00,
    GWD_UP_STREAM       = 0x01,
} gwd_direction_t;

typedef enum {
    GWD_DST_FE   = 0,
    GWD_DST_DROP,
    GWD_DST_CPU,
} gwd_sdl_pkt_dst_t;

typedef gw_uint8 (*gwd_pkt_handler_t)(gw_uint8 *, gw_uint16, gwd_port_id_t);

typedef gwd_status (*gwd_pkt_parser_t)(gw_uint8 *, gw_uint16, gwd_port_id_t);

typedef struct
{
	gw_uint8	port_id;		//端口号
	gw_uint8	slot_id;		//槽位号
	gw_uint8	onu_slot;		//onu 槽位号: onu注册在olt上的pon板的slot(based 1)
	gw_uint8	onu_pon;		//onu pon口号: onu注册在olt上的pon板的PON端口号(based 1)
	gw_uint8	onu_llid;		//onu 逻辑链路标识: onu ID号(based 1)
	gw_uint8	onu_type;		//ONU类型：表示环路ONU的产品类型
	gw_uint8	onu_mac[GWD_MACADDR_LEN];		//ONU MAC：表示环路ONU的系统MAC地址
	gw_uint8	olt_type;		//OLT类型：表示环路ONU所在的OLT类型，1：代表GFA6100，2：代表GFA6700；以后增加新的OLT类型可以依次增加，作为扩展
	gw_uint8	olt_mac[GWD_MACADDR_LEN];		//OLT MAC：表示环路ONU所在的OLT MAC地址
}__attribute__((packed))loop_back_port_info_t;

//onu 环路测试类型
typedef struct
{
	gw_uint8	dest_mac[GWD_MACADDR_LEN];	//目的mac
	gw_uint8	src_mac[GWD_MACADDR_LEN];	//源mac
	gw_uint16	tpid;						//tpid
	gw_uint16	vid;						//vlan id
	gw_uint16	eth_type;					//以太网类型
	gw_uint16	loop_flag;					//环路标志
	gw_uint8	olt_type;					//olt 类型
	gw_uint8	onu_type;					//onu 类型
	gw_uint8	onu_location[4];			//
	gw_uint8	onu_mac[GWD_MACADDR_LEN];	//onu mac
	gw_uint16	onu_vid;					//onu vlan id
	gw_uint32	onu_if_index;				//
} __attribute__ ((packed)) gwd_loop_detect_frame_t;

typedef struct {
    gw_uint8       addr[GWD_MACADDR_LEN];
}__attribute__((packed)) gwd_mac_t;

typedef enum {
    GWD_SDL_PORT_ADMIN_DOWN = 0,  /* Admin down */
    GWD_SDL_PORT_ADMIN_UP   = 1,  /* Admin up   */
} gwd_sdl_port_admin_t;

typedef enum
{
	GWD_LOOP_OCCUR = 0,		/*发生环路*/
	GWD_LOOP_CNACEL,		/*清除环路*/
}gwd_loop_status_t;

#endif	/*end of externally visible */

/*环路控制报文格式*/
/*begin:
added by wangxiaoyu 2008-05-05
环回测试OAM 包
*/
typedef struct Oam_Onu_Lpb_Detect_Frame{
	gw_uint8	type;		//功能类型，4表示为环回检测
	gw_uint8	result;		//测试结果，由ONU端添加
	gw_uint8	enable;		//是否使能功能，由OLT侧添加
	gw_uint16	vid;			//进行测试的VLAN, 0:ONU本地所有VLAN
	gw_uint8	smac[GWD_MACADDR_LEN];	//测试用的源MAC
	gw_uint16	interval;		//OLT发测试帧的间隔时间s
	gw_uint16	policy;		//规则，即是否关闭端口
	/*added by wangxiaoyu 2009-03-11*/
	gw_uint16  waitforwakeup; //等待苏醒的周期，为查询周期的倍数
	gw_uint16  maxwakeup;		//最大苏醒重试次数
}__attribute__((packed))OAM_ONU_LPB_DETECT_FRAME;
/*end*/

/*added by wangxiaoyu 2009-03-11*/
#define LPB_OLD_VER_LEN	(sizeof(OAM_ONU_LPB_DETECT_FRAME)-4)


/*环路检测报文格式*/
typedef struct
{
	gw_uint16	eth_type;					//以太网类型
	gw_uint16	loop_flag;					//环路标志
	gw_uint8	olt_type;					//olt 类型
	gw_uint8	onu_type;					//onu 类型
	gw_uint8	onu_location[4];			//
	gw_uint8	onu_mac[GWD_MACADDR_LEN];	//onu mac
	gw_uint16	onu_vid;					//onu vlan id
	gw_uint32	onu_if_index;
}__attribute__ ((packed)) gwd_loop_detect_frame_data_t;

typedef union
{
    struct
    {
#if VOS_LITTLE_ENDIAN
        unsigned subif:12;
        unsigned port:6;
        unsigned slot:5;
        unsigned subcard:3;
        unsigned type:6;
#else
        unsigned type:6;
        unsigned subcard:3;
        unsigned slot:5;
        unsigned port:6;
        unsigned subif:12;
#endif
    }phy_slot_port;
    gw_uint32 ulPhyIfIndex;
} onu_if_index_t;


typedef enum
{
    GWD_SDL_VLAN_MODE_TRANSPARENT = 0,		/* transparent mode */
    GWD_SDL_VLAN_MODE_TAG,			        /* tag mode */
    GWD_SDL_VLAN_MODE_TRANSLATION,			/* translation mode, VLAN A <-> VLAN B */
    GWD_SDL_VLAN_MODE_AGGREGATION,			/* 1:N aggregation mode */
    GWD_SDL_VLAN_MODE_TRUNK,			    /* trunk mode */
    GWD_SDL_VLAN_MODE_STACKING,             /* zte stacking mode */
    GWD_SDL_VLAN_MODE_END
}gwd_sdl_vlan_mode_t;


#if 1
/*对外提供的接口*/
void
loop_back_detect_init(
	void
	);
gwd_status
loop_back_control_pkt_proc(
	GWD_IN	gw_uint8	*pkt,
	GWD_IN	gw_uint16	len
	);

//设置环路检测是否使能
gwd_status
loop_back_detect_config_enable(
	GWD_IN	loop_control_config_t *config
	);

gwd_status
	loop_back_detect_config_get(
	GWD_OUT	loop_control_config_t *config
	);
#endif



#if 1
/*需要实现的接口*/

//环路检测报文方向和目的设置
gwd_status
gwd_loop_detect_pkt_dst_set(
	GWD_IN	gwd_direction_t		gwd_direction,
	GWD_IN	gwd_sdl_pkt_dst_t	gwd_dst
);

//注册环路检测报文处理函数
gwd_status
gwd_loop_detect_pkt_handler_reg(
	GWD_IN	gwd_pkt_handler_t handler
);

//注册环路检测报文解析函数
gwd_status
gwd_loop_detect_pkt_parser_reg(
	GWD_IN	gwd_pkt_parser_t parser
);


//环路告警
gwd_status
gwd_loop_back_alarm(
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_port_info_t	*port_infor,
	GWD_IN	loop_back_port_info_t	*extern_port_infor,
	GWD_IN	gwd_loop_status_t		loop_status
	);

//创建环路检测线程
gwd_status
gwd_loop_detect_thread_create(
	GWD_IN	void *gwd_loop_detect_task
);

//删除环路检测线程
gwd_status
gwd_loop_detect_thread_delete(
	void
);

//线程延时，以毫秒为单位
gwd_status
gwd_thread_delay(
	GWD_IN	gw_uint32	milli_second
);

//获取端口的所有vlan ，包括default vlan
gwd_status
gwd_onu_port_vlan_get(
	GWD_IN	gwd_port_id_t		port_id,
	GWD_OUT	gwd_sdl_vlan_mode_t *vlan_mode,
	GWD_OUT	gw_uint16			*vlan_def,
	GWD_OUT	gw_uint16			*vlan,
	GWD_OUT	gw_uint16			*vlan_num
);

//获取端口数量
gwd_status gwd_uni_port_num_get(
	GWD_IN	int		*num
	);

//发送环路测试报文
gwd_status
loop_back_detect_pkt_send(
	GWD_IN	gwd_port_id_t				port_id,
	GWD_IN	gwd_loop_detect_frame_t		*frame
	);

//获取fdb 表中指定端口学习到特定mac 的vlan
gwd_status
gwd_fdb_port_mac_get_vlan(
	GWD_IN	gwd_port_id_t	port_id,
	GWD_IN	gwd_mac_t		mac,
	GWD_OUT	gw_uint16		*vlan,
	GWD_OUT	gw_uint16		*vlan_num
);

//申请内存
void *
gwd_malloc(
	GWD_IN	gw_uint32	size
	);

//释放内存
void
gwd_free(
	GWD_IN	void 	*ptr
	);

//设置端口的使能状态
gwd_status
gwd_port_admin_set(
	GWD_IN	gwd_port_id_t			port_id,
	GWD_IN	gwd_sdl_port_admin_t	admin
	);

//获取端口的使能状态
gwd_status
gwd_port_admin_get(
	GWD_IN	gwd_port_id_t			port_id,
	GWD_OUT	gwd_sdl_port_admin_t	*admin
	);

//获取onu mac
gwd_status
gwd_onu_mac_get(
	GWD_OUT	gwd_mac_t	*onu_mac
);

//获取逻辑链路标识
gwd_status
gwd_mpcp_llid_get(
	GWD_OUT	gw_uint16 *gwd_llid
);

//获取olt mac
gwd_status
gwd_olt_mac_get(
	GWD_OUT	gwd_mac_t *olt_mac
	);

//获取olt 类型
gwd_status
gwd_olt_type_get(
	GWD_OUT	gwd_olt_type_t	*olt_type
	);

//获取pon 所在的槽位号和端口号
gwd_status
gwd_pon_slot_port_get(
	GWD_OUT	gw_uint32	*pon_slot,
	GWD_OUT	gw_uint32	*pon_port
	);

//获取onu 产品id
gw_uint8
gwd_onu_product_id_get(
	void
	);

gwd_status
loop_back_led_proc(
	GWD_IN	gwd_loop_status_t	loop_status
	);

void
loop_back_status_list_lock_init(
	void
	);

void
loop_back_status_list_lock(
	void
	);

void
loop_back_status_list_unlock(
	void
	);

extern void gw_lst_init(gw_list * pLst , gw_int32 (*compare)(void * , gw_uint32));

extern void gw_lst_insert(gw_list * pLst, gw_node * pPrevious, gw_node * pNode);

extern void gw_lst_add(gw_list * pLst, gw_node * pNode);

gw_node *gw_lst_remove(gw_list * pLst , gw_uint32 key);

extern void gw_lst_delete(gw_list * pLst, gw_node * pNode);

extern gw_node * gw_lst_first(gw_list * pLst);

extern gw_node * gw_lst_last(gw_list * pLst);

extern gw_node * gw_lst_get(gw_list * pLst);

extern gw_node * gw_lst_nth(gw_list * pLst, int nodeNum);

gw_node *gw_lst_find(gw_list *pLst , gw_uint32 key);

extern gw_node * gw_lst_prev(gw_node * pNode);

extern gw_node * gw_lst_next(gw_node * pNode);

extern gw_uint32 gw_lst_count(gw_list * pLst);

extern gw_list *gw_lst_concat(gw_list *pDst , gw_list *pSrc);

#define gw_lst_scan(pList , pNode , type) for(pNode=(type)gw_lst_first(pList);pNode;pNode=(type)gw_lst_next((gw_node *)pNode))

#endif

#endif /*end of file*/

