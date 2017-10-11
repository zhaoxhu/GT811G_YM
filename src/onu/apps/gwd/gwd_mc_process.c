#include "gwd_mc_process.h"
#include "plat_common.h"
#include "app_oam_gwd.h"
#include "mc_api.h"
#include "iros_config.h"

#ifdef HAVE_GWD_MC_TVM

#ifndef GWD_RETURN_OK
#define GWD_RETURN_OK		(0)
#endif

#ifndef GWD_RETURN_ERR
#define GWD_RETURN_ERR		(-1)
#endif

#ifndef IGMP_TVM_REQ
#define  IGMP_TVM_REQ		(0x16)
#endif

#ifndef IGMP_TVM_RESP
#define  IGMP_TVM_RESP		(0x17)
#endif

#ifndef HAVE_IGMG_CONTROL_TABLE_LOCK
#define HAVE_IGMG_CONTROL_TABLE_LOCK
#endif

#define VLAN_TAG_LEN	(4)

#define printf cs_printf


typedef enum
{
	GW_E_OK = 0,
	GW_E_ERROR = -1,
}gw_status_t;


typedef enum
{
	IGMP_PKT_START = 0,
	IGMP_PKT_REPORT = 0,
	IGMP_PKT_LEAVE ,
	IGMP_PKT_QUERY ,
	IGMP_PKT_NOT ,
	IGMP_PKT_END ,
}IGMP_PKT_TYPE_T;


typedef enum
{
	GW_TVM_ADD_PKT = 1,
	GW_TVM_IP_DEL_PKT = 2,
	GW_TVM_SYN_PKT = 3,
	GW_TVM_VLAN_DEL_PKT =4,
}GW_TVM_PKT_T;


typedef struct igmp_relation_table
{
	unsigned char vlan_id[2];
	unsigned char pon_id[2];
	unsigned char start_ip[4];
	unsigned char end_ip[4];
	unsigned char ulIfindex[4];
	unsigned char llid[4];
}igmp_relation_table_t;


typedef struct oam_through_vlan_igmp
{
	unsigned char enable[2];
	unsigned char type[2];
	unsigned char crc[4];
	unsigned char count[2];

	//解决字节对齐的问题
	//igmp_relation_table_t
	unsigned char VID[2];
	unsigned char pon_id[2];
	unsigned char start_ip[4];
	unsigned char end_ip[4];
	unsigned char ulIfindex[4];
	unsigned char llid[4];
}oam_through_vlan_igmp_t;




typedef struct Through_Vlan_Group {
	unsigned short	IVid;		/*vlan id*/
	unsigned short	PonId;		/*onu pon id*/
	unsigned long	GroupSt;	/*连续组播对应表的第一项*/
	unsigned long	GroupEnd;	/*连续组播对应表的最后一项*/
	unsigned long	ulIfIndex;	/*ONU端口号，预留*/
	unsigned long	llid;		/*onu llid recerved*/
	struct Through_Vlan_Group *next;
} Through_Vlan_Group_t;

typedef struct TVM_Cont_Head{
	int TVMCOUNT;
	int CrcResult;
	Through_Vlan_Group_t *Through_Vlan_Group_head;
}TVM_Cont_Head_t;

typedef struct
{
	unsigned char id_code;
	IGMP_PKT_TYPE_T pkt_type;
}igmp_type_identify_t;



typedef struct addr
{
	unsigned char DA[6];
	unsigned char SA[6];
}eth_head_t;

typedef struct
{
	cs_uint32 TPID		:16;
	cs_uint32 vlan_id_h	:4;
	cs_uint32 Priority	:3;
	cs_uint32 CFI		:1;
	cs_uint32 vlan_id_l	:4;
	cs_uint32 vlan_id_m	:4;
}tag_t;

typedef cs_uint16 tpid_t;


typedef struct
{
	unsigned char head[4];
	unsigned char ip[4];
}igmp_t;




/*+++++++++++++++++++++++++++++++++++++++
 *需要外部提供的接口
 *++++++++++++++++++++++++++++++++++++++++*/

//添加vlan 和端口
extern int vlan_port_add( int vlan_id, int port_id );

//删除vlan
extern void pop_vlan( cs_uint8 *in, cs_uint8 *out,cs_uint32 *len );

//添加vlan
extern void push_vlan( cs_uint16 vlan, cs_uint8 *in, cs_uint8 *out,cs_uint32* len );

//向olt 发送报文
extern int CommOnuMsgSend(unsigned char GwOpcode, unsigned int SendSerNo, unsigned char *pSentData,const unsigned short SendDataSize, unsigned char  *pSessionIdfield);

extern int pkt_vlan_tag_check(char *eth_pkt, cs_uint32 len);

extern cs_status vlan_add_all_port(int vlan);

extern cs_uint16 port_def_vlan_id_get(cs_port_id_t port_id);




/*+++++++++++++++++++++++++++++++++++++++
 *模块内实现的接口
 *++++++++++++++++++++++++++++++++++++++++*/

/*------------------------------------
 *组播报文处理的相关接口
 *------------------------------------*/

//gw 跨vlan 组播报文处理
extern void gwd_mc_tvm_process(cs_pkt_t *pkt, mc_node_t *node);

//gw 跨vlan 组播报文处理
extern int gw_igmp_tvm_pkt_proc( char *igmp_pkt, int *len, int port_id, int *vlan_oper, int *vlan_id_cur );

//通过以太网报文提取组播报文
static char *eth_pkt_extract_igmp_pkt(char *eth_pkt, int len);

//获取组播报文的类型
static gw_status_t igmp_pkt_type_get( char *pkt, int len, IGMP_PKT_TYPE_T *igmp_pkt_type );

//组播report 报文处理
static gw_status_t igmp_report_pkt_proc( char *igmp_pkt, int *len , int port_id, int *vlan_oper, int *vlan_id_cur );

//组播leave  报文处理
static gw_status_t igmp_leave_pkt_proc( char *igmp_pkt, int *len , int port_id, int *vlan_oper, int *vlan_id_cur );

//组播query  报文处理
static gw_status_t igmp_query_pkt_proc( char *igmp_pkt, int *len , int port_id, int *vlan_oper, int *vlan_id_cur );

//获取组播报文的组播ip
static gw_status_t igmp_pkt_get_igmp_ip( char *pkt, int len , cs_uint32 *igmp_ip);

//改变以太网报文的vlan id
static gw_status_t igmp_pkt_change_vlan( char *pkt, int len , int new_vlan_id );



/*------------------------------------
 *组播控制表的相关接口
 *------------------------------------*/


//组播控制表添加组播ip 与vlan的对应关系
static gw_status_t igmp_control_table_add( cs_uint32 ip_start, cs_uint32 ip_end, cs_uint16 vlan_id );


//组播控制表以组播ip 为索引查找vlan
static gw_status_t igmp_control_table_search_by_ip( cs_uint32 ip, cs_uint16 *vlan_id );


//组播控制表以组播ip 为索引删除对应关系
static gw_status_t igmp_control_table_del_by_ip( cs_uint32 ip_start, cs_uint32 ip_end );


//组播控制表以组播vlan 为索引删除对应关系
static gw_status_t igmp_control_table_del_by_vlan( cs_uint16 vlan_id );

//组播控制表清空
static gw_status_t igmp_control_table_clear(void);

//组播控制表同步
static cs_uint32 igmp_control_table_checkout(void);

//组播控制表打印
extern void igmp_control_table_dump(void);

//获取组播控制表的校验值
static cs_uint32 igmp_control_table_get_crc(void);

//请求下发组播控制表
static gw_status_t igmp_control_table_request(GWTT_OAM_MESSAGE_NODE *pRequest);

//跨vlan 组播使能状态设置
static int igmp_tvm_enable_status_set(GW_TVM_ENABLE_T enable);

#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
//组播控制表互斥锁，用于保护数据
static gw_status_t igmp_control_tabel_lock_init(void);
static gw_status_t igmp_control_tabel_lock(void);
static gw_status_t igmp_control_tabel_unlock(void);
#endif

//oam 跨vlan 组播报文处理
static gw_status_t oam_through_vlan_igmp_proc(GWTT_OAM_MESSAGE_NODE *message_input);

//oam 报文控制组播控制表添加组播ip 与vlan的对应关系
static gw_status_t oam_igmp_control_table_add( oam_through_vlan_igmp_t *message );

//oam 报文控制组播控制表以组播ip 为索引删除对应关系
static gw_status_t oam_igmp_control_table_del_by_ip( oam_through_vlan_igmp_t *message );

//oam 报文控制组播控制表以组播vlan 为索引删除对应关系
static gw_status_t oam_igmp_control_table_del_by_vlan( oam_through_vlan_igmp_t *message );

//oam 报文控制组播控制表同步
static gw_status_t oam_igmp_control_tabel_synchronism(GWTT_OAM_MESSAGE_NODE *message_input);




/*封装的接口*/
//删除vlan
extern void del_vlan( cs_uint8 *in, cs_uint8 *out,cs_uint32 *len );

//添加vlan
extern void add_vlan( cs_uint16 vlan, cs_uint8 *in, cs_uint8 *out,cs_uint32* len );

//向olt 发送消息
static int gw_onu_msg_send( unsigned char GwOpcode, unsigned int SendSerNo, unsigned char *pSentData,const unsigned short SendDataSize, unsigned char  *pSessionIdfield );

//添加vlan 和端口
static int gw_vlan_port_add( int vlan_id, int port_id );

static cs_uint16 gw_port_def_vlan_id_get(int port_id);

//全局变量
TVM_Cont_Head_t TVM_Cont_Head_information;
TVM_Cont_Head_t *TVM_Cont_Head_Info = &TVM_Cont_Head_information;		//用于维护组播对应关系表检验值和组播对应关系表的首个表项




/*具体实现*/
extern void gwd_mc_tvm_process(cs_pkt_t *pkt, mc_node_t *node)
{
	GW_TVM_ENABLE_T enable;
	igmp_tvm_enable_status_get(&enable);
	if(GW_TVM_ENABLE == enable)
	{
		
		//gw方式的跨vlan 组播工作在组播snooping 模式下
		mc_mode_t mc_mode;
		mc_mode_get(0, &mc_mode);
		if(MC_SNOOPING == mc_mode)
		{
			char *eth_pkt = NULL;
			int len = 0;
			int port_id;
			int vlan_oper = 0;
			int vlan_id_cur =0;
			eth_pkt = (char *)(pkt->data + pkt->offset);
			len = pkt->len;
			port_id = (int)(node->port[pkt->port]->portid);
			gw_igmp_tvm_pkt_proc(eth_pkt, &len, port_id, &vlan_oper, &vlan_id_cur);
			pkt->len = len;
			pkt->tag_num += vlan_oper;
			pkt->svlan = vlan_id_cur;			
		}
		else
		{
			cs_printf("gw igmp tvm must wort at igmp snooping mode!please set igmp snooping mode!\n");
		}
	}
}

extern int gw_igmp_tvm_pkt_proc( char *igmp_pkt, int *len, int port_id, int *vlan_oper, int *vlan_id_cur )
{
	gw_status_t ret = GW_E_OK;
	IGMP_PKT_TYPE_T igmp_pkt_type = IGMP_PKT_REPORT;
	
	//入口规则检查
	if(NULL == igmp_pkt)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	
	if(NULL == len)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}

	if(NULL == vlan_oper)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	*vlan_oper = 0;
	//untag 报文添加端口的默认vlan
	if(0 == pkt_vlan_tag_check(igmp_pkt, *len))
	{
		cs_uint16 def_vlan = 0;
		def_vlan = gw_port_def_vlan_id_get(port_id);
		add_vlan(def_vlan, igmp_pkt, igmp_pkt, len);
		*vlan_oper += 1;
	}
	
	//获取报文类型
	if(GW_E_OK != igmp_pkt_type_get(igmp_pkt, *len, &igmp_pkt_type))
	{
		printf("in %s, line :%d, get igmp type failed\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	
	//根据报文类型进行分类处理
	switch(igmp_pkt_type)
	{
		//report 报文的处理
		case IGMP_PKT_REPORT :
			ret = igmp_report_pkt_proc(igmp_pkt, len, port_id, vlan_oper, vlan_id_cur);
			break;

		//leave 报文的处理
		case IGMP_PKT_LEAVE :
			ret = igmp_leave_pkt_proc(igmp_pkt, len, port_id, vlan_oper, vlan_id_cur);
			break;

		//query 报文的处理
		case IGMP_PKT_QUERY :
			ret = igmp_query_pkt_proc(igmp_pkt, len, port_id, vlan_oper, vlan_id_cur);
			break;
			
		//异常处理
		default :
			printf("in %s, line :%d, wrong type :%d\n", __func__, __LINE__, igmp_pkt_type);
			break;
	}
	
	return ret;
}

extern long GwOamTvmRequestRecv( void *pRequest_input )
{
	GWTT_OAM_MESSAGE_NODE *pRequest = (GWTT_OAM_MESSAGE_NODE *)pRequest_input;
	//入口规则检查
	if(NULL == pRequest)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GWD_RETURN_ERR;
	}

	if(IGMP_TVM_REQ != pRequest->GwOpcode)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GWD_RETURN_ERR;
	}

	if(NULL == pRequest->pPayLoad)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GWD_RETURN_ERR;
	}

	//oam 跨vlan 组播报文处理
	oam_through_vlan_igmp_proc(pRequest);

	return GWD_RETURN_OK;
}

GW_TVM_ENABLE_T g_gw_enable_status = GW_TVM_DISABLE;

//跨vlan 组播使能状态设置
static int igmp_tvm_enable_status_set(GW_TVM_ENABLE_T enable)
{
	g_gw_enable_status = enable;
	return 0;
}

//跨vlan 组播使能状态获取
extern int igmp_tvm_enable_status_get(GW_TVM_ENABLE_T *enable)
{
	//入口规则检查
	if(NULL == enable)
	{
		return -1;
	}

	*enable = g_gw_enable_status;
	
	return 0;
}

#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
static cyg_sem_t igmp_relation_table_sem;
static gw_status_t igmp_control_tabel_lock_init(void)
{
	cyg_semaphore_init(&igmp_relation_table_sem,1);
	return GW_E_OK;
}
static gw_status_t igmp_control_tabel_lock(void)
{
	cyg_semaphore_wait(&igmp_relation_table_sem);
	return CS_E_OK;
}
static gw_status_t igmp_control_tabel_unlock(void)
{
	cyg_semaphore_post(&igmp_relation_table_sem);
	return CS_E_OK;
}
#endif

//通过以太网报文提取组播报文
static char *eth_pkt_extract_igmp_pkt(char *eth_pkt, int len)
{
	char *igmp_pkt = NULL;
	unsigned char * posation = NULL;
	tag_t *tag = NULL;
	tpid_t *pTpid;
	int ip_head_len = 0;
	//入口规则检查
	if(NULL == eth_pkt)
	{
		cs_printf("in %s, line :%d pointer is NULL!\n");
		return NULL;
	}

	posation = (unsigned char *)eth_pkt;
	//跳过以太网头
	posation += sizeof(eth_head_t);
	
	//跳过vlan tag
	tag = (tag_t *)posation;
	while(0x8100 == ntohs(tag->TPID))
	{	
		tag++;
	}
	posation = (unsigned char *)tag;
	
	//跳过ip 报文tag
	pTpid = (tpid_t *)posation;
	if(0x0800 == ntohs(*pTpid))
	{
		pTpid++;
	}
	else
	{
		//不是ip 报文，返回错误
		return NULL;
	}
	posation = (unsigned char *)pTpid;
	
	//跳过ip 报文头
	ip_head_len = (*posation)&0x0f;
	posation += (ip_head_len*4);
	igmp_pkt = posation;

	return igmp_pkt;
}



static gw_status_t igmp_pkt_type_get( char *pkt, int len, IGMP_PKT_TYPE_T *igmp_pkt_type )
{
	igmp_t *igmp_pkt;
	IGMP_PKT_TYPE_T pkt_type;
	unsigned char type = 0;
	int i = 0;
	//获取组播报文的类型
	//定义类型识别
	static igmp_type_identify_t igmp_type_identify[] = {
							{0x12, IGMP_PKT_REPORT}, {0x16, IGMP_PKT_REPORT}, 
							{0x22, IGMP_PKT_REPORT}, {0x17, IGMP_PKT_LEAVE}, 
							{0x11, IGMP_PKT_QUERY},
						};
	//入口规则检查
	if(NULL == pkt)
	{
		cs_printf("in %s, line :%d pointer is NULL!\n");
		return GW_E_ERROR;
	}
	
	if(NULL == igmp_pkt_type)
	{
		cs_printf("in %s, line :%d pointer is NULL!\n");
		return GW_E_ERROR;
	}
	igmp_pkt = (igmp_t *)eth_pkt_extract_igmp_pkt(pkt, len);
	//获取组播报文类型码
	type = igmp_pkt->head[0];

	//根据类型码查找类型
	pkt_type = IGMP_PKT_NOT;
	for(i=0; i<sizeof(igmp_type_identify); i++)
	{
		if(type == igmp_type_identify[i].id_code)
		{
			pkt_type = igmp_type_identify[i].pkt_type;
			break;
		}
	}
	
	*igmp_pkt_type = pkt_type;
	return GW_E_OK;
}

//组播report 报文处理
static gw_status_t igmp_report_pkt_proc( char *igmp_pkt, int *len , int port_id, int *vlan_oper, int *vlan_id_cur )
{
	gw_status_t ret = 0;
	cs_uint32 igmp_ip = 0;
	cs_uint16 vlan_id = 0;

	//入口规则检查
	if(NULL == igmp_pkt)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}

	if(NULL == len)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	
	if(NULL == vlan_oper)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	
	if(NULL == vlan_id_cur)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	
	//获取组播ip
	if(GW_E_OK != igmp_pkt_get_igmp_ip(igmp_pkt, *len, &igmp_ip))
	{
		printf("in %s, line :%d, get igmp ip failed\n", __func__, __LINE__);
		return GW_E_ERROR;
	}

	//根据组播ip 到组播控制表查找组播vlan
	if(GW_E_OK == igmp_control_table_search_by_ip(igmp_ip, &vlan_id))
	{
		//找到,更改报文vlan
		igmp_pkt_change_vlan(igmp_pkt, *len, vlan_id);
		*vlan_id_cur = vlan_id;
		//创建vlan, 添加端口
		gw_vlan_port_add(vlan_id, port_id);
		//正常退出
		ret = GW_E_OK;
	}
	else
	{
		//未找到,异常退出
		ret = GW_E_ERROR;
	}

	return ret;
}

//组播leave  报文处理
static gw_status_t igmp_leave_pkt_proc( char *igmp_pkt, int *len , int port_id, int *vlan_oper, int *vlan_id_cur )
{
	gw_status_t ret = 0;
	cs_uint32 igmp_ip = 0;
	cs_uint16 vlan_id = 0;

	//入口规则检查
	if(NULL == igmp_pkt)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}

	if(NULL == len)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}

	if(NULL == vlan_oper)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	
	if(NULL == vlan_id_cur)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	
	//获取组播ip
	if(GW_E_OK != igmp_pkt_get_igmp_ip(igmp_pkt, *len, &igmp_ip))
	{
		printf("in %s, line :%d, get igmp ip failed\n", __func__, __LINE__);
		return GW_E_ERROR;
	}

	
	//根据组播ip 到组播控制表查找组播vlan
	if(GW_E_OK == igmp_control_table_search_by_ip(igmp_ip, &vlan_id))
	{
		//找到, 更改报文vlan
		igmp_pkt_change_vlan(igmp_pkt, *len, vlan_id);

		*vlan_id_cur = vlan_id;
		
		//正常退出
		ret = GW_E_OK;
	}
	else
	{
		//未找到,异常退出
		ret = GW_E_ERROR;
	}
		
	return ret;
}


//组播query  报文处理
static gw_status_t igmp_query_pkt_proc( char *igmp_pkt, int *len , int port_id, int *vlan_oper, int *vlan_id_cur )
{
	//入口规则检查
	if(NULL == igmp_pkt)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}

	if(NULL == len)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}

	if(NULL == vlan_oper)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	
	if(NULL == vlan_id_cur)
	{
		printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return GW_E_ERROR;
	}

	//删除vlan
	del_vlan(igmp_pkt, igmp_pkt, len);
	*vlan_oper -= 1;
	*vlan_id_cur = 0;
	
	return GW_E_OK;
}


#define IGMP_V3_HEAD_LEN	8
//获取组播报文的组播ip
static gw_status_t igmp_pkt_get_igmp_ip( char *pkt, int len , cs_uint32 *igmp_ip)
{
	unsigned char *igmp_pkt = NULL;
	igmp_t *igmp_infor = NULL;
	cs_uint32 ip = 0;
	int offset = 0;
	//入口规则检查
	if(NULL == pkt)
	{
		return GW_E_ERROR;
	}

	if(NULL == igmp_ip)
	{
		return GW_E_ERROR;
	}

	igmp_pkt = (unsigned char *)eth_pkt_extract_igmp_pkt(pkt, len);

	//igmp v3 头有8个字节
	if(0x22 == *igmp_pkt)
	{
		offset = IGMP_V3_HEAD_LEN;
	}
	else
	{
		offset = 0;
	}

	igmp_infor = (igmp_t *)(igmp_pkt + offset);
	memcpy(&ip, igmp_infor->ip, sizeof(igmp_infor->ip));
	ip = ntohl(ip);

	*igmp_ip = ip;
	return GW_E_OK;
}

//改变以太网报文的vlan id
static gw_status_t igmp_pkt_change_vlan( char *pkt, int len , int new_vlan_id )
{
	//入口规则检查
	if(NULL == pkt)
	{
		return GW_E_ERROR;
	}

	//删除vlan
	del_vlan(pkt, pkt, &len);

	//添加vlan
	add_vlan(new_vlan_id, pkt, pkt, &len);
	return GW_E_OK;
}



/*------------------------------------
*组播控制表的相关接口
*------------------------------------*/

#if 1
typedef unsigned long ULONG;

#define	MODULE_RPU_IGMPSNOOP	0
#define LOG_TYPE_IGMP	0
#define IGMP_SNOOP_TVM_DEBUG(str) if( gulDebugIgmpTvm){ printf str ;}
#define VOS_MemZero(s, l)		memset(s, 0, l)
#define VOS_Free(size)	free(size)
#define VOS_Malloc(size, module_id)		malloc(size)
#define VOS_SysLog(log_type, log_alarm, str)	printf(str);
#define VOS_ERROR GW_E_ERROR
#define VOS_OK GW_E_OK
#define VOS_YES 1
#define VOS_NO 0
#define LOG_ALERT       1

ULONG   gulDebugIgmpTvm = VOS_NO;				//标识是否是能了跨vlan组播调试开关



/*dellet the map. search for the igmp-group between the ip_start and ip_end. if has, delete it. if the vlan don't has more groups, delete the vlan.*/
int Del_Tvm_maps(unsigned long ip_start, unsigned long ip_end, unsigned int vid)
{
    return VOS_OK;
}


int addIgmpSnoopTvmItem(ULONG ip_start, ULONG ip_end, unsigned int vid )
{
	Through_Vlan_Group_t *group_temp = NULL;/*在全局链表内移动的组*/
	Through_Vlan_Group_t *group_temp_pre = NULL;/*group_temp的前一个组*/
	Through_Vlan_Group_t *group_temp_new = NULL;/*如果需要，新建的组*/
	Through_Vlan_Group_t *group_temp_new_new = NULL;/*新建的第二个组，老的组被分割后需要*/
	//int temp_ip = 0;
	unsigned long ip_del_start;
	unsigned long ip_del_end;
	unsigned long vid_del;

	IGMP_SNOOP_TVM_DEBUG(("\r\n Add item to the chain. ip_start=0x%x, ip_end=0x%x, vid=%d\r\n", ip_start, ip_end, vid));

    group_temp = TVM_Cont_Head_Info->Through_Vlan_Group_head;
    
    while(NULL != group_temp)
    {
        /*find where the start ip should be*/
        /*可以衔接*/
        if ((ip_start == group_temp->GroupEnd + 1)&&(vid == group_temp->IVid))
        {
            group_temp_new = group_temp;
            group_temp_new->GroupEnd = 0;
        }
        /*移动到下一个节点*/
        else if (ip_start > group_temp->GroupEnd)
        {
            group_temp_pre = group_temp;
            group_temp = group_temp->next;
            
            continue;
        }
        /*before the node, malloc the new node.*/
        else if (ip_start <= group_temp->GroupSt)
        {
            group_temp_new = (Through_Vlan_Group_t *)VOS_Malloc(sizeof(Through_Vlan_Group_t), MODULE_RPU_IGMPSNOOP);
            if(NULL == group_temp_new)
            {
                VOS_SysLog( LOG_TYPE_IGMP, LOG_ALERT, "Thr_Vlan_temp_new Alloc msg failed " );
                return VOS_ERROR;
            }

            VOS_MemZero(group_temp_new, sizeof(Through_Vlan_Group_t));
            TVM_Cont_Head_Info->TVMCOUNT++;
            
            group_temp_new->GroupSt = ip_start;
            group_temp_new->IVid = vid;

            if (NULL == group_temp_pre)
            {
                TVM_Cont_Head_Info->Through_Vlan_Group_head = group_temp_new;
            }
            else
            {
                group_temp_pre->next = group_temp_new;
            }
            group_temp_new->next = group_temp;
        }
        /*one part conflict*/
        else/*ip_start > group_temp->GroupSt*/
        {

            group_temp_new = (Through_Vlan_Group_t *)VOS_Malloc(sizeof(Through_Vlan_Group_t), MODULE_RPU_IGMPSNOOP);
            if(NULL == group_temp_new)
            {
                VOS_SysLog( LOG_TYPE_IGMP, LOG_ALERT, "Thr_Vlan_temp_new Alloc msg failed " );
                return VOS_ERROR;
            }

            VOS_MemZero(group_temp_new, sizeof(Through_Vlan_Group_t));
            TVM_Cont_Head_Info->TVMCOUNT++;

            group_temp_new->IVid = vid;

            if (NULL == group_temp_pre)
            {
                TVM_Cont_Head_Info->Through_Vlan_Group_head = group_temp_new;
            }
            else
            {
                group_temp_pre->next = group_temp_new;
            }
            group_temp_new->next = group_temp;

            if (vid != group_temp->IVid)/*vid conflict*/
            {
                group_temp_new->GroupSt = ip_start;
            
                /*the second part of the old node.*/
                group_temp_new_new = (Through_Vlan_Group_t *)VOS_Malloc(sizeof(Through_Vlan_Group_t), MODULE_RPU_IGMPSNOOP);
                if(NULL == group_temp_new_new)
                {
                    VOS_SysLog( LOG_TYPE_IGMP, LOG_ALERT, "Thr_Vlan_temp_new Alloc msg failed " );
                    return VOS_ERROR;
                }

                VOS_MemZero(group_temp_new_new, sizeof(Through_Vlan_Group_t));
                TVM_Cont_Head_Info->TVMCOUNT++;

                group_temp_new_new->GroupSt = ip_start;
                group_temp_new_new->GroupEnd = group_temp->GroupEnd;
                group_temp_new_new->IVid = group_temp->IVid;

                group_temp->GroupEnd = ip_start - 1;

                group_temp_new_new->next = group_temp_new->next;
                group_temp_new->next = group_temp_new_new;
            }
            else/*vid conflict*/
            {
                group_temp_new->GroupSt = group_temp->GroupSt;
            }
        }

        group_temp = group_temp_new->next;
        group_temp_pre = group_temp_new;
        
        /*find where the end ip should be. in the group_temp here, group_temp->GroupSt >= group_temp_new->GroupSt*/
        while(NULL != group_temp)
        {
            /*衔接*/
            if ((ip_end == group_temp->GroupSt - 1)&&(vid == group_temp->IVid))
            {
                group_temp_new->GroupEnd = group_temp->GroupEnd;

                group_temp_pre->next = group_temp->next;

                VOS_Free(group_temp);

                TVM_Cont_Head_Info->TVMCOUNT--;
                
                return VOS_OK;
            }
            else if (ip_end < group_temp->GroupSt)
            {
                group_temp_new->GroupEnd = ip_end;

                return VOS_OK;
            }
            /*eat the old node.*/
            else if (ip_end >= group_temp->GroupEnd)
            {
                /*just eat it.*/
                if (vid == group_temp->IVid)
                {
                    group_temp_pre->next = group_temp->next;
                    VOS_Free(group_temp);
                    group_temp = group_temp_pre->next;

                    TVM_Cont_Head_Info->TVMCOUNT--;
                }
                /*vid conflict, delete the old node's groups*/
                else 
                {
                    ip_del_start = group_temp->GroupSt;
                    ip_del_end = group_temp->GroupEnd;
                    vid_del = group_temp->IVid;
                        
                    group_temp_pre->next = group_temp->next;
                    VOS_Free(group_temp);
                    Del_Tvm_maps(ip_del_start, ip_del_end, vid_del);
                    
                    group_temp = group_temp_pre->next;
                    
                    TVM_Cont_Head_Info->TVMCOUNT--;
                }

                continue;
            }
            else /*ip_end < group_tmep->GroupEnd*/
            {
                /*merge the new and the old node.*/
                if (vid == group_temp->IVid)
                {
                    group_temp_new->GroupEnd = group_temp->GroupEnd;
                    
                    group_temp_pre->next = group_temp->next;
                    VOS_Free(group_temp);
                    group_temp = group_temp_pre->next;

                    TVM_Cont_Head_Info->TVMCOUNT--;
                }
                /*delete the conflicted part in the old node.*/
                else
                {
                    ip_del_start = group_temp->GroupSt;
                    ip_del_end = ip_end;
                    vid_del = group_temp->IVid;
                    group_temp->GroupSt = ip_end + 1;

                    Del_Tvm_maps(ip_del_start, ip_del_end, vid_del);
                        
                    group_temp_new->GroupEnd = ip_end;
                }
                
                return VOS_OK;
            }
        }
        
        /*end ip is the biggest num in che whole chain.*/
        
        group_temp_new->GroupEnd = ip_end;

        return VOS_OK;
    }
    
    /*加在全局链表的尾部*/
    group_temp_new = (Through_Vlan_Group_t *)VOS_Malloc(sizeof(Through_Vlan_Group_t), MODULE_RPU_IGMPSNOOP);
    if(group_temp_new == NULL)
    {
        VOS_SysLog( LOG_TYPE_IGMP, LOG_ALERT, "Thr_Vlan_temp_new Alloc msg failed " );
        return VOS_ERROR;	
    }

    VOS_MemZero(group_temp_new, sizeof(Through_Vlan_Group_t));
    TVM_Cont_Head_Info->TVMCOUNT++;

    group_temp_new->GroupSt = ip_start;
    group_temp_new->GroupEnd = ip_end;
    group_temp_new->IVid = vid;

    if (NULL == group_temp_pre)
    {
        TVM_Cont_Head_Info->Through_Vlan_Group_head = group_temp_new;
    }
    else
    {
        group_temp_pre->next = group_temp_new;
    }
    
	return VOS_OK;
}

int DelIgmpSnoopTvmItem(ULONG ip_start, ULONG ip_end)
{
	Through_Vlan_Group_t *group_temp = NULL;/*在全局链表内移动的组*/
	Through_Vlan_Group_t *group_temp_pre = NULL;/*当前组的前一个组*/
	Through_Vlan_Group_t *group_temp_new = NULL;/*如果需要，新建的组*/
	Through_Vlan_Group_t *group_temp_temp = NULL;/*临时需要的组*/
	unsigned long ip_del_start;
	unsigned long ip_del_end;
	unsigned long vid_del;
	
	IGMP_SNOOP_TVM_DEBUG(("\r\n delete item to the chain. ip_start=0x%x, ip_end=0x%x\r\n", ip_start, ip_end));

    group_temp = TVM_Cont_Head_Info->Through_Vlan_Group_head;
    
    while (NULL != group_temp)
    {
        /*find where the start ip should be*/
        if (ip_start > group_temp->GroupEnd)
        {
            group_temp_pre = group_temp;
            group_temp = group_temp->next;
            
            continue;
        }
        else if (ip_start >= group_temp->GroupSt)
        {
            if (ip_end < group_temp->GroupEnd)
            {
                group_temp_new = (Through_Vlan_Group_t *)VOS_Malloc(sizeof(Through_Vlan_Group_t), MODULE_RPU_IGMPSNOOP);
                if(NULL == group_temp_new)
                {
                    VOS_SysLog( LOG_TYPE_IGMP, LOG_ALERT, "Thr_Vlan_temp_new Alloc msg failed " );
                    return VOS_ERROR;
                }

                VOS_MemZero(group_temp_new, sizeof(Through_Vlan_Group_t));
                TVM_Cont_Head_Info->TVMCOUNT++;

                group_temp_new->GroupSt = ip_end + 1;
                group_temp_new->IVid = group_temp->IVid;
                group_temp_new->GroupEnd = group_temp->GroupEnd;

                group_temp->GroupEnd = ip_end;

                group_temp_new->next = group_temp->next;
                group_temp->next = group_temp_new;
                    
            }
            
            ip_del_start = ip_start;
            ip_del_end = group_temp->GroupEnd;
            vid_del = group_temp->IVid;
            group_temp->GroupEnd = ip_start - 1;
            
            if (group_temp->GroupSt > group_temp->GroupEnd)
            {
                if (NULL == group_temp_pre)
                {
                    TVM_Cont_Head_Info->Through_Vlan_Group_head = group_temp->next;
                }
                else
                {
                    group_temp_pre->next = group_temp->next;
                }
                group_temp_temp = group_temp->next;
                VOS_Free(group_temp);
                TVM_Cont_Head_Info->TVMCOUNT--;
            }

            Del_Tvm_maps(ip_del_start, ip_del_end, vid_del);
            
            /*this node is handled already.*/
            group_temp = group_temp_temp;

        }
        else/*ip_start < group_temp->GroupSt*/
        {
            /*do nothing.*/
        }

        /*find where the end ip should be.*/
        while (NULL != group_temp)
        {
            if (ip_end < group_temp->GroupSt)
            {
                return VOS_OK;
            }
            /*delete the node.*/
            else if (ip_end >= group_temp->GroupEnd)
            {
                ip_del_start = group_temp->GroupSt;
                ip_del_end = group_temp->GroupEnd;
                vid_del = group_temp->IVid;
                
                if (NULL == group_temp_pre)
                {
                    TVM_Cont_Head_Info->Through_Vlan_Group_head = group_temp->next;
                }
                else
                {
                    group_temp_pre->next = group_temp->next;
                }
                group_temp_temp = group_temp->next;
                VOS_Free(group_temp);
                
                Del_Tvm_maps(ip_del_start, ip_del_end, vid_del);
                TVM_Cont_Head_Info->TVMCOUNT--;

                group_temp = group_temp_temp;
                continue;

            }
            else
            {
                ip_del_start = group_temp->GroupSt;
                ip_del_end = ip_end;
                vid_del = group_temp->IVid;
                group_temp->GroupSt = ip_end + 1;
                
                Del_Tvm_maps(ip_start, ip_end, vid_del);
            }
        }
        return VOS_OK;
        
    }
    
	return VOS_OK;

}

//组播控制表以组播ip 为索引查找vlan
static gw_status_t SearchIgmpSnoopTvmItemByIp( cs_uint32 ip, cs_uint16 *vlan_id )
{
	Through_Vlan_Group_t *head = TVM_Cont_Head_Info->Through_Vlan_Group_head;	
	gw_status_t ret = GW_E_ERROR;
	*vlan_id = 0;
	if(NULL == head)
	{
		printf("gmp_relation_tabel is NULL\n");
		ret = GW_E_ERROR;
	}
	else
	{
		Through_Vlan_Group_t *node = NULL;

		for(node=head; NULL!=node; node=node->next)
		{
			if((ip >= node->GroupSt)&&(ip <= node->GroupEnd))
			{
				*vlan_id = node->IVid;
				ret = GW_E_OK;
				break;
			}
			else
			{
				//do nothing
			}
		}
	}
	
	return ret;
}


//组播控制表以组播vlan 为索引删除对应关系
static gw_status_t DelIgmpSnoopTvmItemByVlan( cs_uint16 vlan_id )
{
	Through_Vlan_Group_t *head = TVM_Cont_Head_Info->Through_Vlan_Group_head;
	
	//链表为空
	if(NULL == head)
	{
		//打印提示
		cs_printf("in %s, list is empty!\n");
	}
	else
	{
		Through_Vlan_Group_t *front = NULL;		//前面的节点
		Through_Vlan_Group_t *rear = NULL;		//后面的节点
		
		//遍历链表(以rear 为基准)
		for(front = head, rear=front; NULL!=rear; front=rear, rear=rear->next)
		{
			//若找到节点
			if(rear->IVid == vlan_id)
			{
				//在链表内删除节点
				TVM_Cont_Head_Info->TVMCOUNT--;
				front->next = rear->next;

				//释放节点
				free(rear);
				break;
			}
		}
	}
	
	return GW_E_OK;
}



//组播控制表清空
static gw_status_t ClearIgmpSnoopTvmItem(void)
{
	Through_Vlan_Group_t *head = TVM_Cont_Head_Info->Through_Vlan_Group_head;
	gw_status_t ret = 0;

	if(NULL == head)
	{
		ret = GW_E_OK;
	}
	else
	{
		Through_Vlan_Group_t *front = NULL;
		Through_Vlan_Group_t *rear = NULL;
		front = head;
		rear = front->next;
		
		while(NULL != rear)
		{
			TVM_Cont_Head_Info->TVMCOUNT--;
			free(front);
			front = rear;
			rear = rear->next;
		}
		TVM_Cont_Head_Info->TVMCOUNT--;
		free(front);
		head = NULL;
		
		ret = GW_E_OK;
	}
	TVM_Cont_Head_Info->TVMCOUNT = 0;
	TVM_Cont_Head_Info->Through_Vlan_Group_head = NULL;
	
	return ret;
}



//组播控制表打印
static gw_status_t DumpIgmpSnoopTvmItem(void)
{
	GW_TVM_ENABLE_T enable;
	Through_Vlan_Group_t *head = NULL;
	cs_uint16 num = 0;
	igmp_tvm_enable_status_get(&enable);
	if(GW_TVM_DISABLE == enable)
	{
		printf("igmp-tvm disabled \n");
		return 0;
	}
	else
	{
		//do nothing
	}
	
	head = TVM_Cont_Head_Info->Through_Vlan_Group_head;
	num = TVM_Cont_Head_Info->TVMCOUNT;
	printf("num :%u\n", num);

	if(NULL == head)
	{
		printf("igmp_relation_tabel is NULL\n");
	}
	else
	{

		int i = 0;
		Through_Vlan_Group_t *front = NULL;
		front = head;
		printf("NO.	GroupStart	GroupEnd	IVid\n");
		printf("------------------------------------------------\n");
		for(front=head; front!=NULL; front = front ->next)
		{
			i++;
			printf("%-4d 	0x%x	0x%x	%u\n", i, front->GroupSt, front->GroupEnd, front->IVid);
		}		
	}
	
	return GW_E_OK;
}

static gw_status_t GetIgmpSnoopTvmItemCrc(void)
{
	return TVM_Cont_Head_Info->CrcResult;
}


typedef unsigned short USHORT;
typedef struct Tvm_Pkt{
	USHORT IVid;
	unsigned short PonId;
	ULONG   GROUPS;
	ULONG   GROUPE;
	ULONG  ulIfIndex;
	ULONG  llid;
}__attribute__((packed))Tvm_Pkt_t;


#define CRC32_POLYNOMIAL 0x04c11db7
/*
 * Build auxiliary table for parallel byte-at-a-time CRC-32.
 */
static void FTPC_BuildCRCTable(int *CRCTable)
{
	int     i;
	int     j;
	int     crc;

	for (i = 0; i < 256; i++)
	{
		crc = i;
		for (j = 8; j > 0; j--)
		{
			if (crc & 0x80)
				crc = (crc << 1) ^ CRC32_POLYNOMIAL;
			else
				crc <<= 1;
		}

		CRCTable[i] = crc;
	}

	return;
}

static int FTPC_CalCrc32(unsigned char *buffer, int length)
{
	unsigned char *pCurByte;
	unsigned char TopByte;
	int     crc32, CRCTable[256];

	FTPC_BuildCRCTable(CRCTable);

	crc32 = (int) 0xffffffff;		/* preload shift register, per CRC-32 spec */

	/*calculate crc for each byte */
	for (pCurByte = buffer; length > 0; ++pCurByte, --length)
	{
		TopByte = (unsigned char) (crc32 >> 24);
		TopByte = ((TopByte ^ (*pCurByte)) & 0xFF);

		crc32 <<= 8;
		crc32 ^= CRCTable[TopByte];
	}

	/* transmit complement, per CRC-32 spec */
	crc32 ^= 0xffffffff;

	return crc32;
}


static int Tvm_Table_Crc()
{
	int i;
	char *buftemp = NULL;
	Tvm_Pkt_t *Tvm_Pkt_temp = NULL;
	Through_Vlan_Group_t  *Thr_Vlan_temp = NULL;

	Thr_Vlan_temp = TVM_Cont_Head_Info->Through_Vlan_Group_head;

//    IGMP_SNOOP_TVM_DEBUG(("\r\n Tvm check crc!\r\n"));

	if (TVM_Cont_Head_Info->TVMCOUNT == 0)
	{
		TVM_Cont_Head_Info->CrcResult = 0;
		
	}
	else
	{
		buftemp = (char *)malloc((TVM_Cont_Head_Info->TVMCOUNT)*sizeof(Tvm_Pkt_t));
		
		if(buftemp == NULL)
		{
		    //VOS_SysLog( LOG_TYPE_IGMP, LOG_ALERT, "Alloc msg failed " );
	        return VOS_ERROR;	
		}

		Tvm_Pkt_temp = (Tvm_Pkt_t *)buftemp;

		memset(buftemp, 0, (TVM_Cont_Head_Info->TVMCOUNT)*sizeof(Tvm_Pkt_t));
		for(i = 1; i <= TVM_Cont_Head_Info->TVMCOUNT; i++)
		{
			if (NULL == Thr_Vlan_temp)
			{
			    //IGMP_SNOOP_TVM_DEBUG(("\r\n pointer is null!\r\n"))
        		free(buftemp);
			    return VOS_ERROR;
			}
			Tvm_Pkt_temp->IVid = htons(Thr_Vlan_temp->IVid); 
			Tvm_Pkt_temp->GROUPS = htonl(Thr_Vlan_temp->GroupSt);
			Tvm_Pkt_temp->GROUPE = htonl(Thr_Vlan_temp->GroupEnd);
			Tvm_Pkt_temp->llid = htons(Thr_Vlan_temp->llid);
			Tvm_Pkt_temp->PonId = htons(Thr_Vlan_temp->PonId);
			Tvm_Pkt_temp->ulIfIndex = htonl(Thr_Vlan_temp->ulIfIndex);
			
			Thr_Vlan_temp = Thr_Vlan_temp->next;

			if (NULL != Thr_Vlan_temp)
			{
    			Tvm_Pkt_temp += 1;
			}
		}
        
		TVM_Cont_Head_Info->CrcResult = FTPC_CalCrc32(buftemp, TVM_Cont_Head_Info->TVMCOUNT*sizeof(Tvm_Pkt_t));
		
        free(buftemp);
	}
	   
	return VOS_OK;	
}


#endif


//组播控制表初始化
extern int igmp_control_table_init(void)
{
	if(NULL != TVM_Cont_Head_Info)
	{
		TVM_Cont_Head_Info->TVMCOUNT = 0;
		TVM_Cont_Head_Info->CrcResult = 0;
		TVM_Cont_Head_Info->Through_Vlan_Group_head = NULL;
	}
	
	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_lock_init();
	#endif
	
	return 0;
}

//组播控制表添加组播ip 与vlan的对应关系
static gw_status_t igmp_control_table_add( cs_uint32 ip_start, cs_uint32 ip_end, cs_uint16 vlan_id )
{
	gw_status_t ret = GW_E_ERROR;
	
	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_lock();
	#endif
	
	ret = addIgmpSnoopTvmItem(ip_start, ip_end, vlan_id);
	
	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_unlock();
	#endif
	
	return ret;
}

//组播控制表以组播ip 为索引删除对应关系
static gw_status_t igmp_control_table_del_by_ip( cs_uint32 ip_start, cs_uint32 ip_end )
{
	gw_status_t ret = GW_E_ERROR;
	
	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_lock();
	#endif
	
	ret = DelIgmpSnoopTvmItem(ip_start, ip_end);

	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_unlock();
	#endif
	
	return ret;
}


//组播控制表以组播ip 为索引查找vlan
static gw_status_t igmp_control_table_search_by_ip( cs_uint32 ip, cs_uint16 *vlan_id )
{
	gw_status_t ret = GW_E_ERROR;
	
	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_lock();
	#endif
	
	ret = SearchIgmpSnoopTvmItemByIp(ip, vlan_id);

	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_unlock();
	#endif
	
	return ret;
}

//组播控制表以组播vlan 为索引删除对应关系
static gw_status_t igmp_control_table_del_by_vlan( cs_uint16 vlan_id )
{
	gw_status_t ret = GW_E_ERROR;
	
	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_lock();
	#endif
	
	ret = DelIgmpSnoopTvmItemByVlan(vlan_id);

	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_unlock();
	#endif
	
	return ret;
}


//组播控制表清空
static gw_status_t igmp_control_table_clear(void)
{
	gw_status_t ret = GW_E_ERROR;
	
	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_lock();
	#endif
	
	ret = ClearIgmpSnoopTvmItem();

	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_unlock();
	#endif
	
	return ret;
}


//组播控制表校验计算
static cs_uint32 igmp_control_table_checkout(void)
{
	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_lock();
	#endif
	
	Tvm_Table_Crc();

	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_unlock();
	#endif
	
	return 0;
}


//组播控制表打印
extern void igmp_control_table_dump(void)
{
	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_lock();
	#endif
	
	DumpIgmpSnoopTvmItem();

	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_unlock();
	#endif
	
	return;
}

//获取组播控制表的校验值
static cs_uint32 igmp_control_table_get_crc(void)
{
	cs_uint32 crc = 0;
	
	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_lock();
	#endif
	
	crc = GetIgmpSnoopTvmItemCrc();

	#ifdef HAVE_IGMG_CONTROL_TABLE_LOCK
	igmp_control_tabel_unlock();
	#endif
	return crc;
}

static gw_status_t igmp_control_table_request(GWTT_OAM_MESSAGE_NODE *pRequest)
{
	unsigned char Response[100]={'\0'};
	unsigned char *ptr = NULL;
	int ResLen=0;
	
	ptr = Response;
	/* Payload */
	cs_uint16 *enable_olt = NULL;
	enable_olt = (cs_uint16 *)ptr;
	cs_uint16 enable_onu = 1;
	*enable_olt = htons(enable_onu);
	ptr = ptr + sizeof(cs_uint16);
	cs_uint16 *type_olt = NULL;
	type_olt = (cs_uint16 *)ptr;
	cs_uint16 type_onu = 5;
	*type_olt = htons(type_onu);
	ptr = ptr + sizeof(cs_uint16);

	
	// 向olt 发送  message_output 
	ResLen = ((unsigned long)ptr-(unsigned long)Response);	
	return gw_onu_msg_send(IGMP_TVM_RESP, pRequest->SendSerNo, Response, ResLen, pRequest->SessionID);
}


static int oam_tvm_enable_get( oam_through_vlan_igmp_t *message, GW_TVM_ENABLE_T *enable)
{
	cs_uint16 status = 0;
	
	//入口规则检查
	if(NULL == message)
	{
		printf("in %s, line :%d, pointer is NULL\n", __func__, __LINE__);
		return -1;
	}

	if(NULL == enable)
	{
		printf("in %s, line :%d, pointer is NULL\n", __func__, __LINE__);
		return -1;
	}
	
	memcpy(&status, message->enable, sizeof(message->enable));
	status = ntohs(status);
	if(GW_TVM_ENABLE == status)
	{
		*enable = GW_TVM_ENABLE;
	}
	else
	{
		*enable = GW_TVM_DISABLE;
	}

	return 0;
}

static int oam_tvm_pkt_type_get( oam_through_vlan_igmp_t *message, GW_TVM_PKT_T *pkt_type )
{
	int ret = 0;
	cs_uint16 type = 0;
	//入口规则检查
	if(NULL == message)
	{
		printf("in %s, line :%d, pointer is NULL\n", __func__, __LINE__);
		return -1;
	}

	if(NULL == pkt_type)
	{
		printf("in %s, line :%d, pointer is NULL\n", __func__, __LINE__);
		return -1;
	}

	ret = 0;
	memcpy(&type, message->type, sizeof(message->type));
	type = ntohs(type);
	
	switch(type)
	{
		case GW_TVM_ADD_PKT:
			*pkt_type = GW_TVM_ADD_PKT;
			break;
		case GW_TVM_IP_DEL_PKT:
			*pkt_type = GW_TVM_IP_DEL_PKT;
			break;
		case GW_TVM_VLAN_DEL_PKT:
			*pkt_type = GW_TVM_VLAN_DEL_PKT;
			break;
		case GW_TVM_SYN_PKT:
			*pkt_type = GW_TVM_SYN_PKT;
			break;
		default:
			printf("unknown tvm type :%d\n", type);
			ret = -1;
			break;
	}
	
	return ret;
}

//oam 跨vlan 组播报文处理
static gw_status_t oam_through_vlan_igmp_proc(GWTT_OAM_MESSAGE_NODE *message_input)
{
	gw_status_t ret = GW_E_ERROR;
	oam_through_vlan_igmp_t *message = NULL;
	GW_TVM_ENABLE_T enable = 0;
	GW_TVM_PKT_T pkt_type = 0;

	//入口规则检查
	if(NULL == message_input)
	{
		printf("in %s, line :%d, pointer is NULL\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	message = (oam_through_vlan_igmp_t *)(message_input->pPayLoad);
	//获取使能状态
	if(GW_E_OK != oam_tvm_enable_get(message, &enable))
	{
		printf("in %s, line :%d, pointer is NULL\n", __func__, __LINE__);
		return GW_E_ERROR;
	}

	
	//获取报文类型
	if(GW_E_OK != oam_tvm_pkt_type_get(message, &pkt_type))
	{
		printf("in %s, line :%d, pointer is NULL\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	
	//使能状态处理
	igmp_tvm_enable_status_set(enable);

	if(GW_TVM_ENABLE == enable)
	{
		#if 0
		printf("before olt oper!\n");
		igmp_control_table_dump();
		cs_printf("pkt_type :%d\n", pkt_type);
		#endif
		//将报文分类处理
		switch(pkt_type)
		{
			case GW_TVM_ADD_PKT:
				ret = oam_igmp_control_table_add(message);
				igmp_control_table_checkout();
				break;
			case GW_TVM_IP_DEL_PKT:
				ret = oam_igmp_control_table_del_by_ip(message);
				igmp_control_table_checkout();
				break;
			case GW_TVM_SYN_PKT:
				ret = oam_igmp_control_tabel_synchronism(message_input);
				break;
			case GW_TVM_VLAN_DEL_PKT:
				ret = oam_igmp_control_table_del_by_vlan(message);
				igmp_control_table_checkout();
				break;
			default:
				printf("unknown tvm pkt_type!\n");
				break;
		}
		#if 0
		printf("after olt oper!\n");
		cs_thread_delay(10);
		igmp_control_table_dump();
		#endif
	}
	else
	{
		//do nothing
		ret = GW_E_OK;
	}

	//退出
	return ret;
}



//oam 报文控制组播控制表添加组播ip 与vlan的对应关系
static gw_status_t oam_igmp_control_table_add( oam_through_vlan_igmp_t *message )
{
	gw_status_t ret = GW_E_ERROR;
	cs_uint16 count = 0;
	cs_uint16 i = 0;
	cs_uint16 vlan_id = 0;
	cs_uint32 start_ip = 0;
	cs_uint32 end_ip = 0;
	igmp_relation_table_t * entry = NULL;
	
	//入口规则检查
	if(NULL == message)
	{
		printf("in %s, line :%d, pointer is NULL\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	
	//获取配置数量
	memcpy(&count, message->count, sizeof(message->count));
	count = ntohs(count);
	
	//获取第一条配置
	entry = (igmp_relation_table_t *)(message->VID);
	
	for( i=0; i<count; i++, entry++)
	{
		//获取网络信息
		memcpy(&vlan_id, entry->vlan_id, sizeof(entry->vlan_id));
		memcpy(&start_ip, entry->start_ip, sizeof(entry->start_ip));
		memcpy(&end_ip, entry->end_ip, sizeof(entry->end_ip));

		//转换为本地信息
		vlan_id = ntohs(vlan_id);
		start_ip = ntohl(start_ip);
		end_ip = ntohl(end_ip);

		#if 0
		cs_printf("in %s, start_ip :0x%x, end_ip:0x%x\n", __func__, start_ip, end_ip);
		#endif
		//将配置保存
		if(GW_E_OK != (ret = igmp_control_table_add(start_ip, end_ip, vlan_id)))
		{
			printf("in %s, line :%d, igmp_control_table_add failed\n", __func__, __LINE__);
		}
	}
	
	return ret;
}


//oam 报文控制组播控制表以组播ip 为索引删除对应关系
static gw_status_t oam_igmp_control_table_del_by_ip( oam_through_vlan_igmp_t *message )
{
	gw_status_t ret = GW_E_ERROR;
	cs_uint16 count = 0;
	
	//入口规则检查
	if(NULL == message)
	{
		printf("in %s, line :%d, pointer is NULL\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	
	//获取条目数量
	memcpy(&count, message->count, sizeof(message->count));
	count = ntohs(count);

	if(0 == count)
	{
		//清空条目处理
		igmp_control_table_clear();
	}
	else
	{
		cs_uint16 i = 0;
		cs_uint32 start_ip = 0;
		cs_uint32 end_ip = 0;
		igmp_relation_table_t * entry = NULL;
		
		//获取第一条配置
		entry = (igmp_relation_table_t *)(message->VID);
		
		//删除条目处理
		for( i=0; i<count; i++, entry++)
		{
			//获取网络信息
			memcpy(&start_ip, entry->start_ip, sizeof(entry->start_ip));
			memcpy(&end_ip, entry->end_ip, sizeof(entry->end_ip));

			//转换为本地信息
			start_ip = ntohl(start_ip);
			end_ip = ntohl(end_ip);
			
			//将配置删除
			#if 0
			cs_printf("in %s, start_ip :0x%x, end_ip:0x%x\n", __func__, start_ip, end_ip);
			#endif
			if(GW_E_OK 
!= (ret = igmp_control_table_del_by_ip(start_ip, end_ip)))
			{
				printf("in %s, line :%d, igmp_control_table_del_by_ip failed\n", __func__, __LINE__);
			}
		}	
	}
	
	return ret;
}


//oam 报文控制组播控制表以组播vlan 为索引删除对应关系
static gw_status_t oam_igmp_control_table_del_by_vlan( oam_through_vlan_igmp_t *message )
{
	gw_status_t ret = GW_E_ERROR;
	cs_uint16 count = 0;
	cs_uint16 i = 0;
	cs_uint16 vlan_id = 0;
	igmp_relation_table_t * entry = NULL;
	//入口规则检查
	if(NULL == message)
	{
		printf("in %s, line :%d, pointer is NULL\n", __func__, __LINE__);
		return GW_E_ERROR;
	}
	
	//获取条目数量
	memcpy(&count, message->count, sizeof(message->count));
	count = ntohs(count);

	//获取第一条配置
	entry = (igmp_relation_table_t *)(message->VID);
	
	//删除条目处理
	for( i=0; i<count; i++, entry++)
	{
		//获取网络信息
		memcpy(&vlan_id, entry->vlan_id, sizeof(entry->vlan_id));

		//转换为本地信息
		vlan_id = ntohs(vlan_id);
		
		//将配置删除
		if(GW_E_OK 
!= (ret = igmp_control_table_del_by_vlan(vlan_id)))
		{
			printf("in %s, line :%d, igmp_control_table_del_by_ip failed\n", __func__, __LINE__);
		}
	}
	return ret;

}


//oam 报文控制组播控制表同步
static gw_status_t oam_igmp_control_tabel_synchronism(GWTT_OAM_MESSAGE_NODE *message_input)
{
	gw_status_t ret = GW_E_OK;
	cs_uint32 olt_crc = 0;
	cs_uint32 onu_crc = 0;
	oam_through_vlan_igmp_t *message = NULL;
	//入口规则检查
	if(NULL == message_input)
	{
		printf("in %s, line :%d, pointer is NULL\n", __func__, __LINE__);
		return GW_E_ERROR;
	}

	message = (oam_through_vlan_igmp_t *)(message_input->pPayLoad);
	//获取olt  校验值
	memcpy(&olt_crc, message->crc, 4);
	olt_crc = ntohl(olt_crc);
	
	//获取onu 校验值
	onu_crc = igmp_control_table_get_crc();

	#if 0
	cs_printf("olt_crc :%u, onu_crc :%u\n", olt_crc, onu_crc);
	#endif
	//比较olt  校验值与onu 校验值
	if(olt_crc != onu_crc)
	{
		//清空表项
		if(GW_E_OK != igmp_control_table_clear())
		{
			printf("in %s, line :%d, igmp_control_table_clear failed\n", __func__, __LINE__);
			ret = GW_E_ERROR;
		}
		//请求下发所有表项
		if(GW_E_OK != igmp_control_table_request(message_input))
		{
			printf("in %s, line :%d, igmp_control_table_request failed\n", __func__, __LINE__);
			ret = GW_E_ERROR;
		}
	}
	else
	{
		//do nothing
	}
			
	return ret;
}


//删除vlan
extern void del_vlan( cs_uint8 *in, cs_uint8 *out,cs_uint32 *len )
{
	pop_vlan(in ,out, len);
	return;
}


//添加vlan
extern void add_vlan( cs_uint16 vlan, cs_uint8 *in, cs_uint8 *out,cs_uint32* len )
{
	push_vlan(vlan, in, out, len);
	return;
}

//向olt 发送报文
static int gw_onu_msg_send( unsigned char GwOpcode, unsigned int SendSerNo, unsigned char *pSentData,const unsigned short SendDataSize, unsigned char  *pSessionIdfield )
{
	return CommOnuMsgSend(GwOpcode, SendSerNo, pSentData, SendDataSize, pSessionIdfield);
}

static int gw_vlan_port_add( int vlan_id, int port_id )
{
	return vlan_port_add(vlan_id, port_id);
}

static cs_uint16 gw_port_def_vlan_id_get(int port_id)
{
	return port_def_vlan_id_get(port_id);
}


//外部接口
/*---------------------------------------------------*/

//添加vlan 和端口
extern int vlan_port_add( int vlan_id, int port_id )
{
	vlan_add_all_port(vlan_id);
	return 0;
}

/*---------------------------------------------------*/
#endif	/*end of zhe file*/

