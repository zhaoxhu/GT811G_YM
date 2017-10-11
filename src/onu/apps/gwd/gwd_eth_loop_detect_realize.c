#include "gwd_eth_loop_detect_realize.h"
#include "cs_cmn.h"
#include "cs_types.h"
#include "packet.h"
#include "osal_api_core.h"
#include "sdl_vlan.h"
#include "sdl_ma.h"
#include "aal_mpcp.h"
#include "sdl_fdb.h"
#include "sdl_port.h"
#include "gpio.h"
#include "app_oam_gwd.h"
#include "sdl_l2.h"

#define ONU_ETH_LOOP_ALARM_TYPE	(11)
#define LOOP_OCCUR	(1)
#define LOOP_CANCEL	(2)
//#define TEST

/*环路告警报文格式*/
//oam 环路告警类型
typedef struct
{
	gw_uint8	alarmFlag;					//告警类型：11－ONU以太网端口环路告警
	gw_uint8	portNum[4];					//下挂交换机ID:1B, 交换机端口号:1B, ONU槽位号:1B, ONU端口号:1B
	gw_uint8	loopstate;					//环路状态：1－环回；2－环回取消；
	gw_uint16	vlanid;						//VLAN ID：在IVL模式下，表示发生环路的端口所属VLAN，在SVL模式下，用0表示
	gw_uint8	switchMac[GWD_MACADDR_LEN];	//交换机MAC地址：表示ONU下挂交换机的系统MAC地址，只有在"下挂交换机ID"非0时有效
	gw_uint8	externFlag;					//环路信息标志：标志是否带有扩展的环路信息，0：不带扩展信息，1：带有扩展信息
	    gw_uint8	oltType;				//OLT类型：表示环路对端ONU所在的OLT类型，1：代表GFA6100，2：代表GFA6700；以后增加新的OLT类型可以依次增加，作为扩展
	    gw_uint8	oltMac[GWD_MACADDR_LEN];	//OLT MAC：表示环路对端ONU所在的OLT MAC地址
	    gw_uint8	onuLocation[4];			//ONU位置：表示环路对端ONU在OLT的Pon口位置，第一个字节表示slot号，第二个字节标志port号
	    gw_uint8	onuType;				//ONU类型：表示环路对端ONU的产品类型
	    gw_uint8	onuMac[GWD_MACADDR_LEN];	//ONU MAC：表示环路对端ONU的系统MAC地址
	    gw_uint8	onuPort[4];				//ONU端口索引：表示发生环路的对端ONU端口索引，对没有使能Host MII端口的多端口ONU（GT811，GT812此值为0）
} __attribute__ ((packed)) gwd_alarm_loop_t;



extern int CommOnuMsgSend(unsigned char GwOpcode, unsigned int SendSerNo, unsigned char *pSentData,const unsigned short SendDataSize, unsigned char  *pSessionIdfield);
extern cs_status uni_port_num_get(int *num);
void getOltMacAddress(epon_macaddr_t * olt_mac);
int GwGetOltType(unsigned char *mac, GWD_OLT_TYPE *type);
int GwGetPonSlotPort(unsigned char *mac, GWD_OLT_TYPE type, unsigned long *slot, unsigned long *port);
cs_status 
onu_mac_get(
	CS_OUT	cs_mac_t *mac
	);
cs_status 
onu_mac_get(
	CS_OUT	cs_mac_t *mac
	);
cs_uint8 onu_product_ID_get();

#if 1
/*需要实现的接口*/

//环路检测报文方向和目的设置
gwd_status
gwd_loop_detect_pkt_dst_set(
	GWD_IN	gwd_direction_t		gwd_direction,
	GWD_IN	gwd_sdl_pkt_dst_t	gwd_dst
)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_ERROR;
	
	cs_callback_context_t context;
	cs_direction_t direction;
    cs_pkt_type_t pkt_type;
    cs_sdl_pkt_state_t state;

	switch(gwd_direction)
	{
		case GWD_DOWN_STREAM:
			direction = CS_DOWN_STREAM;
			break;
		case GWD_UP_STREAM:
			direction = CS_UP_STREAM;
			break;
		default:
			ret = GWD_E_ERROR;
			goto end;
			break;
	}

	switch(gwd_dst)
	{
		case GWD_DST_FE:
			state = DST_FE;
			break;
		case GWD_DST_CPU:
			state = DST_CPU;
			break;
		case GWD_DST_DROP:
			state = DST_DROP;
			break;
		default:
			ret = GWD_E_ERROR;
			goto end;
			break;
	}
	
	pkt_type = CS_PKT_GWD_LOOPDETECT;
	#ifdef TEST
	gwd_printf("in %s, line :%d, direction :0x%x, pkt_type :0x%x, state :0x%x\n", __func__, __LINE__, direction, pkt_type, state);
	#endif
	if(CS_E_OK == epon_request_onu_spec_pkt_state_set(context, 0, 0, direction,pkt_type,state))
	{
		ret = GWD_E_OK;
	}
	else
	{
		ret = GWD_E_ERROR;
	}

end:	
	return ret;
}

static gwd_pkt_handler_t gwd_loop_back_pkt_handler = NULL;

cs_uint8 cs_loop_back_pkt_handler(cs_pkt_t *pPkt)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	cs_uint8 ret = 0;
	gw_uint8 *pdata = NULL;
	gw_uint16 data_len = 0;
	gwd_port_id_t port_id;
	
	pdata = (gw_uint8 *)(pPkt->data+pPkt->offset);
	data_len = pPkt->len;
	port_id = pPkt->port;

	if(NULL != gwd_loop_back_pkt_handler)
	{
		ret =  (*gwd_loop_back_pkt_handler)(pdata, data_len, port_id);
	}
	else
	{
		ret = 0;
	}

	return ret;
}

//注册环路检测报文处理函数
gwd_status
gwd_loop_detect_pkt_handler_reg(
	GWD_IN	gwd_pkt_handler_t handler
)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_ERROR;
	
	gwd_loop_back_pkt_handler = handler;
	if(CS_E_OK == app_pkt_reg_handler(CS_PKT_GWD_LOOPDETECT, cs_loop_back_pkt_handler))
	{
		ret = GWD_E_OK;
	}
	else
	{
		ret = GWD_E_ERROR;
	}
	
	return ret;
}

static gwd_pkt_parser_t	gwd_loop_back_pkt_parse = NULL;

cs_status cs_loop_back_pkt_parse(cs_pkt_t *pPkt)
{
	cs_status ret = CS_E_OK;
	gw_uint8 *pdata = NULL;
	gw_uint16 data_len = 0;
	gwd_port_id_t port_id;
	
	pdata = (gw_uint8 *)(pPkt->data+pPkt->offset);
	data_len = pPkt->len;
	port_id = pPkt->port;
	
	if(NULL != gwd_loop_back_pkt_parse)
	{
		if(GWD_E_OK == (*gwd_loop_back_pkt_parse)(pdata, data_len, port_id))
		{
			pPkt->pkt_type = CS_PKT_GWD_LOOPDETECT;
		}
		else
		{
			ret = CS_E_ERROR;
		}
	}
	else
	{
		ret = CS_E_ERROR;
	}

	return ret;
}

//注册环路检测报文解析函数
gwd_status
gwd_loop_detect_pkt_parser_reg(
	GWD_IN	gwd_pkt_parser_t parser
)
{
	#ifdef TEST
	gwd_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_ERROR;
	gwd_loop_back_pkt_parse = parser;
	
	if(CS_E_OK == app_pkt_reg_parser(CS_PKT_GWD_LOOPDETECT, cs_loop_back_pkt_parse))
	{
		ret = GWD_E_OK;
	}
	else
	{
		ret = GWD_E_ERROR;
	}
	
	return ret;
}


//环路告警
gwd_status
gwd_loop_back_alarm(
	GWD_IN	gw_uint16				vlan_id,
	GWD_IN	loop_back_port_info_t	*port_infor,
	GWD_IN	loop_back_port_info_t	*extern_port_infor,
	GWD_IN	gwd_loop_status_t		loop_status
	)
{
	//入口规则检查
	if(NULL == port_infor)
	{
		return CS_E_ERROR;
	}
	//extern_port_infor 可以为NULL

	
	gwd_alarm_loop_t gwd_alarm_loop;
	cs_uint8	session[8] = {0};
	
	memset(&gwd_alarm_loop, 0, sizeof(gwd_alarm_loop_t));
	
	gwd_alarm_loop.alarmFlag = ONU_ETH_LOOP_ALARM_TYPE;
	gwd_alarm_loop.portNum[0] = 0;
	gwd_alarm_loop.portNum[2] = 0;
	gwd_alarm_loop.portNum[3] = port_infor->port_id;
	if(GWD_LOOP_OCCUR == loop_status)
	{
		gwd_alarm_loop.loopstate = LOOP_OCCUR;
	}
	else
	{
		gwd_alarm_loop.loopstate = LOOP_CANCEL;
	}

	gwd_alarm_loop.vlanid = htons(vlan_id);

	#ifdef	HAVE_LOOP_DETECT_ALARM_BASE_OLT
	/*olt only 6100 and 6700 can alarm loop with extern information*/
	if((GWD_OLT_GFA6100 == port_infor->olt_type)||(GWD_OLT_GFA6700 == port_infor->olt_type)||(olt_inform_onu_local_infor_enable_get()))
	{
		if(NULL != extern_port_infor)
		{
			gwd_alarm_loop.externFlag = 1;
			gwd_alarm_loop.oltType = extern_port_infor->olt_type;
			memcpy(gwd_alarm_loop.oltMac, extern_port_infor->olt_mac, CS_MACADDR_LEN);
			gwd_alarm_loop.onuLocation[0] = 0;
			gwd_alarm_loop.onuLocation[1] = extern_port_infor->onu_slot;
			gwd_alarm_loop.onuLocation[2] = extern_port_infor->onu_pon;
			gwd_alarm_loop.onuLocation[3] = extern_port_infor->onu_llid;
			gwd_alarm_loop.onuType = extern_port_infor->onu_type;
			memcpy(gwd_alarm_loop.onuMac, extern_port_infor->onu_mac, sizeof(gwd_alarm_loop.onuMac));
			gwd_alarm_loop.onuPort[2] = extern_port_infor->slot_id;
			gwd_alarm_loop.onuPort[3] = extern_port_infor->port_id;
		}
		else
		{
			gwd_alarm_loop.externFlag = 0;
		}
	}
	#else
	if(NULL != extern_port_infor)
	{
		gwd_alarm_loop.externFlag = 1;
		gwd_alarm_loop.oltType = extern_port_infor->olt_type;
		memcpy(gwd_alarm_loop.oltMac, extern_port_infor->olt_mac, CS_MACADDR_LEN);
		gwd_alarm_loop.onuLocation[0] = 0;
		gwd_alarm_loop.onuLocation[1] = extern_port_infor->onu_slot;
		gwd_alarm_loop.onuLocation[2] = extern_port_infor->onu_pon;
		gwd_alarm_loop.onuLocation[3] = extern_port_infor->onu_llid;
		gwd_alarm_loop.onuType = extern_port_infor->onu_type;
		memcpy(gwd_alarm_loop.onuMac, extern_port_infor->onu_mac, sizeof(gwd_alarm_loop.onuMac));
		gwd_alarm_loop.onuPort[2] = extern_port_infor->slot_id;
		gwd_alarm_loop.onuPort[3] = extern_port_infor->port_id;
	}
	else
	{
		gwd_alarm_loop.externFlag = 0;
	}
	#endif

	if(GWD_RETURN_OK != CommOnuMsgSend(ALARM_REQ,0,(cs_uint8 *)&gwd_alarm_loop,sizeof(gwd_alarm_loop_t), session))
	{
		cs_printf("in %s, CommOnuMsgSend failed\n", __func__);
	}
	return CS_E_OK;
}


static cs_uint32 thread_id =0;
//创建环路检测线程
gwd_status
gwd_loop_detect_thread_create(
	GWD_IN	void *gwd_loop_detect_task
)
{
	gwd_status ret = CS_E_OK;
	char thread_name[] = "loop detect";
	cs_uint32 stack_size = 4*1024;
	cs_uint32 priority = 25;
	cs_uint32 flags = 0;
	if(CS_E_OSAL_OK == cs_thread_create(&thread_id, thread_name, gwd_loop_detect_task, NULL, stack_size,priority, flags))
	{
		gwd_printf("loop detect thread create success!\n", __func__);
		ret = CS_E_OK;
	}
	else
	{
		gwd_printf("in %s, cs_thread_create failed\n", __func__);
		ret = CS_E_ERROR;
	}
	return ret;
}

//删除环路检测线程
gwd_status
gwd_loop_detect_thread_delete(
	void
)
{
	gwd_status ret = CS_E_OK;
	if(CS_E_OSAL_OK == cs_thread_delete(thread_id))
	{
		gwd_printf("loop detect thread delete success!\n", __func__);
		ret = CS_E_OK;
	}
	else
	{
		gwd_printf("loop detect thread delete failed!\n", __func__);
		ret = CS_E_ERROR;
	}
	
	return ret;
}

//线程延时，以毫秒为单位
gwd_status
gwd_thread_delay(
	GWD_IN	gw_uint32	milli_second
)
{
	cs_thread_delay(milli_second);
	return GWD_E_OK;
}

//获取端口的所有vlan ，包括default vlan
gwd_status
gwd_onu_port_vlan_get(
	GWD_IN	gwd_port_id_t		port_id,
	GWD_OUT	gwd_sdl_vlan_mode_t *vlan_mode,
	GWD_OUT	gw_uint16			*vlan_def,
	GWD_OUT	gw_uint16			*vlan,
	GWD_OUT	gw_uint16			*vlan_num
)
{
	cs_callback_context_t context;
	cs_port_id_t port;
	cs_sdl_vlan_tag_t def_vlan;
	cs_sdl_vlan_mode_t mode;
	cs_sdl_vlan_cfg_t vlan_cfg[__VLAN_RULE_PER_PORT_MAX];
	cs_uint16 cfg_nums;
	int i = 0;
	
	port = port_id;
	epon_request_onu_vlan_get(context, 0, 0, port, &def_vlan, &mode, vlan_cfg, &cfg_nums);
	*vlan_mode = mode;
	*vlan_def = def_vlan.vid;
	
	*vlan_num = cfg_nums;
	for(i=0; i<cfg_nums; i++)
	{
		*vlan = vlan_cfg[i].c_vlan.vid;
	}
	
	return GWD_E_OK;
}

//获取端口数量
gwd_status gwd_uni_port_num_get(
	GWD_IN	int		*num
	)
{
	uni_port_num_get(num);
	return GWD_E_OK;
}

//发送环路测试报文
gwd_status
loop_back_detect_pkt_send(
	GWD_IN	gwd_port_id_t				port_id,
	GWD_IN	gwd_loop_detect_frame_t		*frame
	)
{
	#ifdef TEST
	gwd_printf("****in %s, line :%d****\n", __func__, __LINE__);
	#endif
	gwd_status ret = GWD_E_OK;
	//入口规则检查
	if(NULL == frame)
	{
		return GWD_E_ERROR;
	}
	
	cs_callback_context_t context;

	if(CS_E_OK == epon_request_onu_frame_send(context, 0, 0, (cs_uint8 *)(frame), sizeof(gwd_loop_detect_frame_t), port_id, 0))
	{
		ret = GWD_E_OK;
	}
	else
	{
		ret = GWD_E_ERROR;
		gwd_printf("****in %s, line :%d, epon_request_onu_frame_send failed****\n", __func__, __LINE__);
	}
	
	return ret;
}

//获取fdb 表中指定端口学习到特定mac 的vlan
gwd_status
gwd_fdb_port_mac_get_vlan(
	GWD_IN	gwd_port_id_t	port_id,
	GWD_IN	gwd_mac_t		mac,
	GWD_OUT	gw_uint16		*vlan,
	GWD_OUT	gw_uint16		*vlan_num
)
{
	cs_callback_context_t context;
	cs_uint16 offset;
	cs_sdl_fdb_entry_t entry;
	cs_uint16 next;
	int count = 0;
	
	memset(&entry, 0, sizeof(entry));

	//查找指定端口学习到的所有fdb 
	while(CS_OK == epon_request_onu_fdb_entry_getnext_per_port(context, 0, 0, port_id, SDL_FDB_ENTRY_GET_MODE_ALL,
			offset, &next, &entry))
	{
		offset = next;

		#if 0
		cs_printf("%02x:%02x:%02x:%02x:%02x:%02x\t %-6d\t %-2d\t %-2d\n",
        entry.mac.addr[0],
        entry.mac.addr[1],
        entry.mac.addr[2],
        entry.mac.addr[3],
        entry.mac.addr[4],
        entry.mac.addr[5],
        entry.vlan_id,
        entry.port,
        entry.type);
		#endif
		//端口学到的mac 与配置的mac 进行比较 
		if(0 == memcmp(mac.addr, entry.mac.addr, CS_MACADDR_LEN))
		{
			vlan[count++] = entry.vlan_id;
		}
	}		
	*vlan_num = count;
	
	return GWD_E_OK;
}


//申请内存
void *
gwd_malloc(
	GWD_IN	gw_uint32	size
	)
{
	return iros_malloc(IROS_MID_APP,size);
}

//释放内存
void
gwd_free(
	GWD_IN	void 	*ptr
	)
{
	iros_free(ptr);
}

//设置端口的使能状态
gwd_status
gwd_port_admin_set(
	GWD_IN	gwd_port_id_t			port_id,
	GWD_IN	gwd_sdl_port_admin_t	admin
	)
{
	gwd_status ret = GWD_E_OK;
	cs_callback_context_t context;
	if(CS_E_OK == epon_request_onu_port_admin_set(context, 0, 0, port_id, admin))
	{
		ret = GWD_E_OK;
	}
	else
	{
		ret = GWD_E_ERROR;
	}
	return ret;
}

//获取端口的使能状态
gwd_status
gwd_port_admin_get(
	GWD_IN	gwd_port_id_t			port_id,
	GWD_OUT	gwd_sdl_port_admin_t	*admin
	)
{
	if(NULL == admin)
	{
		return GWD_E_ERROR;
	}
	
	gwd_status ret = GWD_E_OK;
	cs_callback_context_t context;
	cs_sdl_port_admin_t cs_admin;
	if(CS_E_OK == epon_request_onu_port_admin_get(context, 0, 0, port_id, &cs_admin))
	{
		*admin = cs_admin;
		ret = GWD_E_OK;
	}
	else
	{
		ret = GWD_E_ERROR;
	}
	return ret;
}

//获取onu mac
gwd_status
gwd_onu_mac_get(
	GWD_OUT	gwd_mac_t	*onu_mac
)
{
	cs_mac_t mac;
	onu_mac_get(&mac);
	memcpy(onu_mac->addr, mac.addr, CS_MACADDR_LEN);
	
	return GWD_E_OK;
}

//获取逻辑链路标识
gwd_status
gwd_mpcp_llid_get(
	GWD_OUT	gw_uint16 *gwd_llid
)
{
	aal_mpcp_llid_get(gwd_llid);
	return GWD_E_OK;
}

//获取olt mac
gwd_status
gwd_olt_mac_get(
	GWD_OUT	gwd_mac_t *olt_mac
	)
{
	epon_macaddr_t mac;
	getOltMacAddress(&mac);

	memcpy(olt_mac->addr, mac, CS_MACADDR_LEN);
	return GWD_E_OK;
}

//获取olt 类型
gwd_status
gwd_olt_type_get(
	GWD_OUT	gwd_olt_type_t	*olt_type
	)
{
	epon_macaddr_t olt_mac_addr;
	GWD_OLT_TYPE type;
	
	getOltMacAddress(&olt_mac_addr);
	GwGetOltType(olt_mac_addr, &type);
	*olt_type = type;
	return GWD_E_OK;
}

//获取pon 所在的槽位号和端口号
gwd_status
gwd_pon_slot_port_get(
	GWD_OUT	gw_uint32	*pon_slot,
	GWD_OUT	gw_uint32	*pon_port
	)
{
	epon_macaddr_t olt_mac_addr;
	GWD_OLT_TYPE olt_type;
	unsigned long onu_slot = 0;
	unsigned long onu_pon = 0;
	
	getOltMacAddress(&olt_mac_addr);
	GwGetOltType(olt_mac_addr, &olt_type);
	GwGetPonSlotPort(olt_mac_addr, olt_type, &onu_slot, &onu_pon);
	*pon_slot = onu_slot;
	*pon_port = onu_pon;
	
	return GWD_E_OK;
}

//获取onu 产品id
gw_uint8
gwd_onu_product_id_get(
	void
	)
{
	return onu_product_ID_get();
}

#define LOOP_LED_GPIO		(13)
gwd_status
loop_back_led_proc(
	GWD_IN	gwd_loop_status_t	loop_status
)
{
	cs_uint8 level;
	
	if(loop_status == GWD_LOOP_OCCUR)
	{
		level = 1;
	}
	else
	{
		level = 0;
	}
	
	cs_gpio_mode_set(LOOP_LED_GPIO, GPIO_OUTPUT);
	cs_gpio_write(LOOP_LED_GPIO, level);
	
	return GWD_E_OK;
}


static cyg_sem_t loop_back_status_list_sem;
void
loop_back_status_list_lock_init(
	void
	)
{
	cyg_semaphore_init(&loop_back_status_list_sem,1);
}

void
loop_back_status_list_lock(
	void
	)
{
	cyg_semaphore_wait(&loop_back_status_list_sem);
}

void
loop_back_status_list_unlock(
	void
	)
{
	cyg_semaphore_post(&loop_back_status_list_sem);
}


extern void gw_lst_init(gw_list * pLst , gw_int32 (*compare)(void * , gw_uint32))
{
	cs_lst_init(pLst, compare);
	return;
}
extern void gw_lst_insert(gw_list * pLst, gw_node * pPrevious, gw_node * pNode)
{
	cs_lst_insert(pLst, pPrevious, pNode);
	return;
}

extern void gw_lst_add(gw_list * pLst, gw_node * pNode)
{
	cs_lst_add(pLst, pNode);
	return;
}

gw_node *gw_lst_remove(gw_list * pLst , gw_uint32 key)
{
	gw_node *node = cs_lst_remove(pLst , key);

	return node;
}

extern void gw_lst_delete(gw_list * pLst, gw_node * pNode)
{
	cs_lst_delete(pLst, pNode);
	return;
}

extern gw_node * gw_lst_first(gw_list * pLst)
{
	gw_node *node = cs_lst_first(pLst);
	return node;
}

extern gw_node * gw_lst_last(gw_list * pLst)
{
	gw_node *node = cs_lst_last(pLst);
	return node;
}

extern gw_node * gw_lst_get(gw_list * pLst)
{
	gw_node *node = cs_lst_get(pLst);
	return node;
}

extern gw_node * gw_lst_nth(gw_list * pLst, int nodeNum)
{
	gw_node *node = cs_lst_nth(pLst, nodeNum);
	return node;
}

gw_node *gw_lst_find(gw_list *pLst , gw_uint32 key)
{
	gw_node *node = cs_lst_find(pLst ,key);
	return node;
}

extern gw_node * gw_lst_prev(gw_node * pNode)
{
	gw_node *node = cs_lst_prev(pNode);
	return node;
}

extern gw_node * gw_lst_next(gw_node * pNode)
{
	gw_node *node = cs_lst_next(pNode);
	return node;
}

extern gw_uint32 gw_lst_count(gw_list * pLst)
{
	cs_uint32 count = cs_lst_count(pLst);
	return count;
}

extern gw_list *gw_lst_concat(gw_list *pDst , gw_list *pSrc)
{
	gw_list *list = cs_lst_concat(pDst , pSrc);
	return list;
}

#ifdef	HAVE_LOOP_DETECT_ALARM_BASE_OLT
static bool g_olt_inform_onu_local_infor_enable = false;
void olt_inform_onu_local_infor_enable_set(bool enable)
{
	g_olt_inform_onu_local_infor_enable = enable;
}

bool olt_inform_onu_local_infor_enable_get(void)
{
	return g_olt_inform_onu_local_infor_enable;
}

static onu_local_info_t g_onu_local_info;
gw_status olt_inform_onu_local_infor_set(onu_local_info_t *onu_local_info)
{
	gw_status ret = GW_E_ERROR;

	if(NULL != onu_local_info)
	{
		memcpy(&g_onu_local_info, onu_local_info, sizeof(onu_local_info_t));

		ret = GW_E_OK;
	}

	return ret;
}

gw_status olt_inform_onu_local_infor_get(onu_local_info_t *onu_local_info)
{
	cs_status ret = GW_E_ERROR;

	if(NULL != onu_local_info)
	{
		*onu_local_info = g_onu_local_info;

		ret = GW_E_OK;
	}

	return ret;
}

gw_cell_tlv_t *gw_tlv_list_get_elem(gw_uint8 *tlv_list, gw_uint16 len, gw_uint8 id)
{
	gw_cell_tlv_t *addr_ret = NULL;
	gw_cell_tlv_t *cell_tlv = NULL;
	int offset = 0;
	gw_uint8 * const head = (gw_uint8 * const )tlv_list;

	addr_ret = NULL;
	while(offset < len)
	{
		cell_tlv = (gw_cell_tlv_t *)&head[offset];

		if(cell_tlv->cell_id == id)
		{
			addr_ret = cell_tlv;
		}

		offset += sizeof(cell_tlv->cell_id) + sizeof(cell_tlv->cell_len) + ntohs(cell_tlv->cell_len);
	}

	return addr_ret;
}
#endif

#endif

